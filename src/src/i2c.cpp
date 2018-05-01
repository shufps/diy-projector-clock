/* 
 * This file is part of DIY-Projector-Clock
 * www.microengineer.eu
 * 
 * If you like my work, please consider to donate IOTAs to:
 * LLEYMHRKXWSPMGCMZFPKKTHSEMYJTNAZXSAYZGQUEXLXEEWPXUNWBFDWESOJVLHQHXOPQEYXGIRBYTLRWHMJAOSHUY
 *
 *
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "stm32f10x_i2c.h"

void i2c_init() {
	// Initialization struct
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	// Step 1: Initialize I2C
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0xfe;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C2, &I2C_InitStruct);
	I2C_Cmd(I2C2, ENABLE);

	// Step 2: Initialize GPIO as open drain alternate function
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

#define I2C2_SLAVE_ADDRESS7 (0x3f << 1)

void I2C_write(uint8_t Register_Adress, uint8_t Data) {

	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
		;

	/* Send I2C2 START condition */
	I2C_GenerateSTART(I2C2, ENABLE);

	/* Test on I2C2 EV5 and clear it */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
		;

	/* Send slave Address for write */
	I2C_Send7bitAddress(I2C2, I2C2_SLAVE_ADDRESS7, I2C_Direction_Transmitter);

	/* Test on I2C2 EV6 and clear it */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;

	/* Send I2C2 slave register address */
	I2C_SendData(I2C2, Register_Adress);

	/* Test on I2C2 EV8_2 and clear it */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

	/* Send I2C2 slave Register data */
	I2C_SendData(I2C2, Data);

	/* Test on I2C2 EV8_8 and clear it */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

	/* Send I2C2 STOP Condition */
	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t I2C_read(uint8_t Register_Adress) {

	uint8_t I2C_ReceivedData = 0;
	/*!< While the bus is busy */
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
		;

	/*!< Send START condition */
	I2C_GenerateSTART(I2C2, ENABLE);

	/*!< Test on EV5 and clear it */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
		;

	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(I2C2, I2C2_SLAVE_ADDRESS7, I2C_Direction_Transmitter);

	/*!< Test on EV6 and clear it */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;

//send the reg select COMMAND byte
	I2C_SendData(I2C2, Register_Adress);

	/*!< Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

//send START condition a second time
	I2C_GenerateSTART(I2C2, ENABLE);

	/*!< Test on EV5 and clear it */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
		;

	/*!< Send EEPROM address for read */
	I2C_Send7bitAddress(I2C2, I2C2_SLAVE_ADDRESS7, I2C_Direction_Receiver);

	/*!< Test on EV6 and clear it */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		;

//    /*!< Test on EV7 and clear it */
	if (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		/*!< Read a byte from the EEPROM */
		I2C_ReceivedData = I2C_ReceiveData(I2C2);
	}

	/*!< Disable Acknowledgement */
	I2C_AcknowledgeConfig(I2C2, DISABLE);

	/*!< Send STOP Condition */
	I2C_GenerateSTOP(I2C2, ENABLE);

	/*!< Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C2, ENABLE);

	return I2C_ReceivedData;
}
