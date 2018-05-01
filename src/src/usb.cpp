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


#include <string.h>

#include "main.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "json.h"

#define RXBUFSIZE 512

uint32_t packet_sent = 1;
uint32_t packet_receive = 1;

static uint8_t rxbuf[RXBUFSIZE];
static uint32_t rxbufrdptr = 0;
static uint32_t rxbufwrptr = 0;
static char usbrxbuf[256];
static char* usbwrptr;

extern "C" void usb_CDCReceiveCallback(uint8_t* Receive_Buffer, uint32_t Receive_length) {
	for (uint32_t i = 0; i < Receive_length; i++) {
		rxbuf[rxbufwrptr % RXBUFSIZE] = Receive_Buffer[i];
		rxbufwrptr++;
	}
}

// TODO move to json and make it better
extern "C" void usb_transmitString(char* msg) {
	char strbuf[256] = { 0 };
	if (strlen(msg) > sizeof(strbuf) - 5)	// not likely to happen
		return;
	memcpy((void*) strbuf, (void*) msg, strlen(msg));
	char* p = &strbuf[strlen(msg)];
	*p++ = '\r';
	*p++ = '\n';
	*p = 0;
	uint32_t tosend = strlen(strbuf);
	uint8_t* cptr = (uint8_t*) strbuf;
	uint32_t maxsize = VIRTUAL_COM_PORT_DATA_SIZE - 1;
	while (tosend) {
		int chunklen = min(maxsize, tosend);
		while (GetEPTxStatus(ENDP1) != EP_TX_NAK)
			;
		CDC_Send_DATA(cptr, chunklen);
		cptr += chunklen;
		tosend -= chunklen;
	}
}



uint8_t parseCmd(char* rxbuf) {
	return json_process(rxbuf);
}

void usb_process() {
	CDC_Receive_DATA();
	for (; rxbufrdptr < rxbufwrptr; rxbufrdptr++) {
		*usbwrptr = rxbuf[rxbufrdptr % RXBUFSIZE];
		if (*usbwrptr == '\n')
			continue;
		if (*usbwrptr == '\r') {
			*usbwrptr = 0;
			if (!parseCmd(usbrxbuf))
				usb_transmitString("error");
			usbwrptr = &usbrxbuf[0];
		} else {
			usbwrptr++;
			if ((int) usbwrptr - (int) &usbrxbuf > (int) sizeof(usbrxbuf) - 2) {
				memset(usbrxbuf, 0, sizeof(usbrxbuf));
				usbwrptr = &usbrxbuf[0];
				usb_transmitString("error");
			}
		}
	}
}

uint8_t usb_isConfigured() {
	return bDeviceState == CONFIGURED;
}

void usb_init() {
	Set_System();
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();

	usbwrptr = &usbrxbuf[0];
}
