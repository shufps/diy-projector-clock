/*
 * lcd64x32.h
 *
 *  Created on: 26.10.2017
 *      Author: thomas
 */

#ifndef LCD64X32_H_
#define LCD64X32_H_

#include "ugui.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32


#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_DISP_START_LINE  0x40
#define CMD_SET_PAGE  0xB0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2
#define CMD_SET_BIAS_6 0xA2
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST  0x81
#define  CMD_SET_VOLUME_SECOND  0
#define CMD_SET_STATIC_OFF  0xAC
#define  CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define  CMD_SET_BOOSTER_5  1
#define  CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST  0xF0




void lcd_setPixel(int x, int y);
void lcd_clearPixel(int x, int y);
void lcd_drawPixel(UG_S16 x, UG_S16 y, UG_COLOR col);
void lcd_UG_PutStringf(UG_S16 x, UG_S16 y, const char* format, ...);
void lcd_update();
void lcd_clear(uint8_t update);
void lcd_init();
void lcd_putIcon(int x, int y, const char* data);
void lcd_setContrast(uint8_t val);

void lcd_putChar5x7(int x, int y, char c);
void lcd_putString5x7(int x, int y, char* buf);
void lcd_putString5x7f(int x, int y, const char* format, ...);


#endif /* LCD64X32_H_ */
