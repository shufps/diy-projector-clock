/*
 * i2c.h
 *
 *  Created on: 26.10.2017
 *      Author: thomas
 */

#ifndef I2C_H_
#define I2C_H_

void i2c_init();
void I2C_write(uint8_t Register_Adress, uint8_t Data);
uint8_t I2C_read(uint8_t Register_Adress);


#endif /* I2C_H_ */
