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

// partly used from:
// https://github.com/adafruit/ST7565-LCD/blob/master/c/stlcd.h

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "ugui.h"
#include "i2c.h"
#include "lcd64x32.h"

static uint8_t lcddata[4 * 64];
static uint8_t lcd2ndbuf[4 * 64];
extern unsigned char Font5x7[];

void lcd_setPixel(int x, int y) {
	lcddata[(y / 8) * 64 + x] |= (1 << (y % 8));
}

void lcd_clearPixel(int x, int y) {
	lcddata[(y / 8) * 64 + x] &= ~((1 << (y % 8)));
}

void lcd_drawPixel(UG_S16 x, UG_S16 y, UG_COLOR col) {
	if (col) {
		lcd_setPixel(x, y);
	} else {
		lcd_clearPixel(x, y);
	}
}

void lcd_UG_PutStringf(UG_S16 x, UG_S16 y, const char* format, ...) {
	char buf[80];
	memset(buf, 0, sizeof(buf));
	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf) - 1, format, args);
	UG_PutString(x, y, buf);

	va_end(args);
}

void lcd_putChar5x7(int x, int y, char c) {
	/*if (c>='a' && c<='z')
		c-='z'-'Z';*/

	for (int i=0;i<5;i++) {
		if (x+i<0 || x+i>=64)
			continue;
		char d = Font5x7[c*5+i];
		for (int j=0;j<8;j++) {
			if (y+j<0||y+j>=32)
				continue;
			if (d & (1<<j))
				lcd_setPixel(x+i,y+j);
			else
				lcd_clearPixel(x+i,y+j);
		}
	}
}

void lcd_putString5x7(int x, int y, char* buf) {
	for (int i=0;i<(int) strlen(buf);i++) {
		lcd_putChar5x7(x+i*6,y,buf[i]);
	}
}

void lcd_putString5x7f(int x, int y, const char* format, ...) {
	char buf[80];
	memset(buf, 0, sizeof(buf));
	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf) - 1, format, args);
	lcd_putString5x7(x, y, buf);

	va_end(args);
}

void lcd_putIcon(int x, int y, const char* data) {
	for (int i = 0; i < 24; i++) {
		for (int j = 0; j < 24; j++) {
			if (data[i * 24 + j] == '0') {
				lcd_setPixel(x + j, y + i);
			} else
				lcd_clearPixel(x + j, y + i);
		}
	}
}

void lcd_update() {
	int equal = 1;
	for (int i = 0; i < 64 * 4; i++) {
		if (lcd2ndbuf[i] != lcddata[i]) {
			equal = 0;
			break;
		}
	}

	if (equal)
		return;

	uint8_t coladr = 0x20 + 4;
	for (int y = 0; y < 4; y++) {
		I2C_write(0x80, CMD_SET_COLUMN_LOWER | (coladr & 0xf));
		I2C_write(0x80, CMD_SET_COLUMN_UPPER | ((coladr & 0xf0) >> 4));
		I2C_write(0x80, CMD_SET_PAGE | y); // page 0 ist unten
		for (int x = 0; x < 64; x++) {
			I2C_write(0xc0, lcddata[y * 64 + x]);
		}
	}
	memcpy(lcd2ndbuf, lcddata, sizeof(lcddata));
}

void lcd_clear(uint8_t update) {
	memset(lcddata, 0, sizeof(lcddata));
	if (update)
		lcd_update();
}

void lcd_setContrast(uint8_t val) {
	I2C_write(0x80, CMD_SET_VOLUME_FIRST);
	I2C_write(0x80, CMD_SET_VOLUME_SECOND | (val & 0x3f));
}

void lcd_init() {
	i2c_init();
	I2C_write(0x80, CMD_SET_BIAS_6);
	I2C_write(0x80, CMD_SET_ADC_REVERSE);
	//I2C_Write(0x80, CMD_SET_ADC_NORMAL);
	I2C_write(0x80, CMD_SET_COM_REVERSE);
	//I2C_Write(0x80, CMD_SET_COM_NORMAL);
	I2C_write(0x80, CMD_SET_DISP_START_LINE);
	I2C_write(0x80, CMD_SET_POWER_CONTROL | 0x4);
	I2C_write(0x80, CMD_SET_POWER_CONTROL | 0x6);
	I2C_write(0x80, CMD_SET_POWER_CONTROL | 0x7);
	I2C_write(0x80, CMD_SET_RESISTOR_RATIO | 0x6);
	I2C_write(0x80, CMD_DISPLAY_ON);
	I2C_write(0x80, CMD_SET_ALLPTS_NORMAL);
	I2C_write(0x80, CMD_SET_VOLUME_FIRST);
	I2C_write(0x80, CMD_SET_VOLUME_SECOND | (37 & 0x3f));

	I2C_write(0x80, CMD_SET_COLUMN_LOWER | 0x0);
	I2C_write(0x80, CMD_SET_COLUMN_UPPER | 0x0);
	I2C_write(0x80, CMD_SET_PAGE | 0x0);
}

