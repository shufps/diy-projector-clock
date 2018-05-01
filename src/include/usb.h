/*
 * usb.h
 *
 *  Created on: 14.11.2017
 *      Author: ne23kaj2
 */

#ifndef USB_H_
#define USB_H_

#include <stdint.h>

extern "C" void usb_CDCReceiveCallback(uint8_t* Receive_Buffer, uint32_t Receive_length);
extern "C" void usb_transmitString(char* msg);

void usb_init();
void usb_process();
uint8_t usb_isConfigured();


#endif /* USB_H_ */
