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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "diag/Trace.h"

#include "Timer.h"

#include "gpio.h"
#include "tim.h"
#include "i2c.h"
#include "lcd64x32.h"
#include "rtc.h"

#include "usb.h"

#include "ugui.h"
#include "ugui_config.h"
#include "icons.h"

#include "json.h"
#include "irmp.h"
#include "ir.h"
#include "rotenc.h"

extern const UG_FONT FONT_12X16;
//extern const UG_FONT FONT_6X10;

const UG_FONT* font = &FONT_12X16;
//const UG_FONT* fonts = &FONT_6X10;

static UG_GUI gui;

#define ADJUST_NOP	0
#define ADJUST_BRIGHTNESS 1
#define ADJUST_CONTRAST 2
#define ADJUST_TIME	3
#define ADJUST_DATE 4
#define ADJUST_ICON 5
#define ADJUST_TIMER 6

#define DISPLAY_TIME	0
#define DISPLAY_ICON	1
#define DISPLAY_ADJ_TIMEDATE	2
#define DISPLAY_TIMER 3
#define DISPLAY_CONTRAST 4
#define DISPLAY_BRIGHTNESS 5

#define LED_OFF	0
#define LED_ON	1

#define NUM_ICONS 6
#define ICON_TIME 0
#define ICON_DATE 1
#define ICON_TIMER 2
#define ICON_BRIGHTNESS 3
#define ICON_CONTRAST 4
#define ICON_BACK 5

extern "C" void __disable_irq();
extern "C" void __enable_irq();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wwrite-strings"



extern volatile timer_ticks_t ticks;
RTC_TimeTypeDef_H24 RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;

IRMP_DATA irmp_data;

// 16kHz
extern "C" void TIM3_IRQHandler() {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		rotenc_irq_process();
		(void) irmp_ISR();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

uint16_t lutpwm[LUTPWMSIZE] = { 0 };

void calcPWMLUT() {
	for (int x = 0; x < LUTPWMSIZE; x++) {
		lutpwm[x] = powf(2.0f, (logf((float) SystemCoreClock / 16000.0f) / logf(2.0f)) * ((float) (x + 1)) / LUTPWMSIZE)
				- 1.0f;
	}
}

int switchPressed() {
	return GPIO_ReadInputDataBit(GPIO_SWITCH) == Bit_RESET;
}

int main(int argc, char* argv[]) {
	gpio_init();
	timer_init();
	rotenc_init();
	rtc_init();

	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	timer_start();

	rtc_getTime24(&RTC_TimeStructure);
	rtc_getDate(&RTC_DateStructure);

	usb_init();
	GPIO_SetBits(GPIO_DISC);

	GPIO_ResetBits(GPIO_LCD_RESET);
	timer_sleep(200);
	GPIO_SetBits(GPIO_LCD_RESET);
	timer_sleep(200);

	lcd_init();
	lcd_update();

	UG_Init(&gui, lcd_drawPixel, SCREEN_WIDTH, SCREEN_HEIGHT);
	UG_FillScreen(C_BLACK);
	UG_SetBackcolor(C_BLACK);
	UG_SetForecolor(C_WHITE);
	UG_Update();
	lcd_update();

	calcPWMLUT();
	int16_t pwmval = LUTPWMSIZE - 1;	//500;


// read settings and sanitize them
	pwmval = BKP_ReadBackupRegister(BKP_DR4);
	int16_t contrast = BKP_ReadBackupRegister(BKP_DR5);

	if (contrast<0)
		contrast=0;
	if (contrast>63)
		contrast=63;

	if (pwmval>LUTPWMSIZE-1)
		pwmval=LUTPWMSIZE-1;
	if (pwmval<600)
		pwmval=600;

	TIM3->CCR1 = lutpwm[pwmval]; //3000;

	lcd_setContrast(contrast);
	uint32_t lastticks = 0;

	int mode = ADJUST_NOP;
	int display = DISPLAY_TIME;
	int blink = 0;
	int update = 0;
	int blinktimeout = 0;
	int rotthreshold = 5;
	int rotthreshold_date = 3;
	int rotthreshold_time = 2;
	int rotcnt = 0;
	int sleeptmr = 12;
	int waitswitchrelease = 0;
	int ledstate = LED_ON;
	uint32_t ledonticks = ticks;

	int lpticks = 0;

	const char* icons[NUM_ICONS] = { icon_time, icon_date, icon_timer, icon_brightness, icon_contrast, icon_back };
	int selectedicon = 0;



	while (1) {
		if (usb_isConfigured()) {
			usb_process();
		}

		if (irmp_get_data(&irmp_data)) {
			if (!(irmp_data.flags & IRMP_FLAG_REPETITION)) {
				switch (irmp_data.command) {
				case CMD_QUICK:  // quick
					break;
				case CMD_FADE: // fade
					break;
				case CMD_SLOW:  // slow
					break;
				case CMD_RED:  // red
					break;
				case CMD_GREEN:  // green
					break;
				case CMD_FLASH: // flash
					break;
				case CMD_PLUS1:
				case CMD_PLUS2:
//					event |= EVENT_SHORTPRESS;
					break;
				case CMD_MINUS:
					break;
				case CMD_SET:	// play-button (settings)
//					event |= EVENT_LONGPRESS;
					break;
				}
			}
		}

		if (ticks - lastticks > 500) {
			lastticks = ticks;
			if (mode != ADJUST_TIME && mode != ADJUST_DATE) {
				rtc_checkUpdateDate(&RTC_TimeStructure, &RTC_DateStructure);
				rtc_getTime24(&RTC_TimeStructure);
				rtc_getDate(&RTC_DateStructure);
			}
			if (!blinktimeout)
				blink = 1 - blink;
			else
				blinktimeout--;
			update = 1;
		}

		if (switchPressed()) {
			do {
				if (waitswitchrelease) {
					timer_sleep(250);
					break;
				}
				switch (mode) {
				case ADJUST_NOP:
					if (!lpticks)
						lpticks = ticks;
					break;
				case ADJUST_ICON:
					switch (selectedicon) {
					case ICON_TIME:
						mode = ADJUST_TIME;
						display = DISPLAY_ADJ_TIMEDATE;
						rotcnt = 0;
						update = 1;
						break;
					case ICON_DATE:
						mode = ADJUST_DATE;
						display = DISPLAY_ADJ_TIMEDATE;
						rotcnt = 0;
						update = 1;
						break;
					case ICON_TIMER:
						mode = ADJUST_TIMER;
						display = DISPLAY_TIMER;
						rotcnt = 0;
						update = 1;
						break;
					case ICON_BRIGHTNESS:
						mode = ADJUST_BRIGHTNESS;
						display = DISPLAY_BRIGHTNESS;
						update = 1;
						break;
					case ICON_CONTRAST:
						mode = ADJUST_CONTRAST;
						display = DISPLAY_CONTRAST;
						rotcnt = 0;
						update = 1;
						break;
					case ICON_BACK:
						mode = ADJUST_NOP;
						display = DISPLAY_TIME;
						update = 1;
						break;
					}
					break;
				case ADJUST_TIMER:
					mode = ADJUST_ICON;
					display = DISPLAY_ICON;
					selectedicon = ICON_TIMER;
					update = 1;
					break;
				case ADJUST_BRIGHTNESS:
					mode = ADJUST_ICON;
					display = DISPLAY_ICON;
					selectedicon = ICON_BRIGHTNESS;
					BKP_WriteBackupRegister(BKP_DR4, pwmval);
					update = 1;
					break;
				case ADJUST_CONTRAST:
					mode = ADJUST_ICON;
					display = DISPLAY_ICON;
					selectedicon = ICON_CONTRAST;
					BKP_WriteBackupRegister(BKP_DR5, contrast);
					update = 1;
					break;
				case ADJUST_TIME:
					rtc_setTime24(&RTC_TimeStructure);
					mode = ADJUST_ICON;
					display = DISPLAY_ICON;
					selectedicon = ICON_TIME;
					break;
				case ADJUST_DATE:
					rtc_setDate(&RTC_DateStructure);
					selectedicon = ICON_DATE;
					display = DISPLAY_ICON;
					mode = ADJUST_ICON;
				}
				timer_sleep(250);	// debounce
			} while (0);
		} else {
			lpticks = 0;
			waitswitchrelease = 0;
		}

		if (lpticks && !switchPressed() && (ticks - lpticks) < 500) { // short press
			lpticks = 0;
			ledstate = 1 - ledstate;
			if (ledstate == LED_OFF) {
				TIM3->CCR1 = 0;
			} else {
				ledonticks = ticks;
				TIM3->CCR1 = lutpwm[pwmval];
			}
			// disable LED
		} else if (lpticks && (ticks - lpticks) > 2000) {	// long press
			lpticks = 0;
			if (mode == ADJUST_NOP && display == DISPLAY_TIME) {
				// long press -> goto setup
				mode = ADJUST_ICON;
				display = DISPLAY_ICON;
				rotcnt = 0;
				update = 1;
				waitswitchrelease = 1;
			}
		}

		// sleep timer
		if (sleeptmr && (ticks - ledonticks) > (uint32_t) sleeptmr * 3600 * 1000) // ticks is in ms
				{
			ledstate = LED_OFF;
			TIM3->CCR1 = 0;
		}

		if (ledstate == LED_OFF) {
			mode = ADJUST_NOP;
			display = DISPLAY_TIME;
			update = 1;
		}

		int8_t eval = rotenc_read1();
		eval *= -1; // reverse direction
		if (eval) {
			switch (mode) {
			case ADJUST_CONTRAST:
				rotcnt += eval;
				if (rotcnt > rotthreshold) {
					contrast++;
					rotcnt = 0;
				}
				if (rotcnt < -rotthreshold) {
					contrast--;
					rotcnt = 0;
				}
				if (contrast < 0)
					contrast = 0;
				if (contrast > 63)
					contrast = 63;

				I2C_write(0x80, CMD_SET_VOLUME_FIRST);
				I2C_write(0x80, CMD_SET_VOLUME_SECOND | (contrast & 0x3f));
				break;
			case ADJUST_BRIGHTNESS:
				pwmval += eval;
				if (pwmval < 500)
					pwmval = 500;
				if (pwmval >= LUTPWMSIZE)
					pwmval = LUTPWMSIZE - 1;
				if (pwmval == 500)
					TIM3->CCR1 = 0;
				else
					TIM3->CCR1 = lutpwm[pwmval];
				break;
			case ADJUST_TIME:
				blink = 1; // when adjusting don't blink
				update = 1;
				blinktimeout = 4;
				rotcnt += eval;
				if (rotcnt > rotthreshold_time) {
					rtc_adjustTime(&RTC_TimeStructure, 1);
					rotcnt = 0;
				}
				if (rotcnt < -rotthreshold_time) {
					rtc_adjustTime(&RTC_TimeStructure, -1);
					rotcnt = 0;
				}
				break;
			case ADJUST_DATE:
				blink = 1; // when adjusting don't blink
				update = 1;
				blinktimeout = 4;
				rotcnt += eval;
				if (rotcnt > rotthreshold_date) {
					rtc_adjustDate(&RTC_DateStructure, 1);
					rotcnt = 0;
				}
				if (rotcnt < -rotthreshold_date) {
					rtc_adjustDate(&RTC_DateStructure, -1);
					rotcnt = 0;
				}

				break;
			case ADJUST_TIMER:
				update = 1;
				rotcnt += eval;
				if (rotcnt > rotthreshold) {
					sleeptmr++;
					rotcnt = 0;
				}
				if (rotcnt < -rotthreshold) {
					sleeptmr--;
					rotcnt = 0;
				}
				if (sleeptmr > 24)
					sleeptmr = 24;
				if (sleeptmr < 0)
					sleeptmr = 0;
				break;
			case ADJUST_ICON:
				rotcnt += eval;
				if (rotcnt > rotthreshold) {
					selectedicon++;
					if (selectedicon >= NUM_ICONS)
						selectedicon = NUM_ICONS - 1;
					rotcnt = 0;
				} else if (rotcnt < -rotthreshold) {
					selectedicon--;
					if (selectedicon < 0)
						selectedicon = 0;
					rotcnt = 0;
				}
				update = 1;
				break;
			}
		}

		if (update) {
			update = 0;
			switch (display) {
			case DISPLAY_TIME:
			{
				UG_FillScreen(C_BLACK);
				UG_FontSelect(font);
				int xofs = (RTC_TimeStructure.RTC_Hours < 10) ? 6 : 0;
				lcd_UG_PutStringf(xofs, 2, "%d:", RTC_TimeStructure.RTC_Hours);
				lcd_UG_PutStringf(37 - xofs, 2, "%02d", RTC_TimeStructure.RTC_Minutes);
				int weekday = rtc_weekday(RTC_DateStructure.RTC_Year + 2000, RTC_DateStructure.RTC_Month,
						RTC_DateStructure.RTC_Date);
				//char dayNames[][12] = { "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So" };
				char dayNames[][12] = { "H", "K", "Sze", "Cs", "P", "Szo", "V" };
				int xpos = (64 - (strlen(dayNames[weekday]) + 3) * 7)/2;
				lcd_putString5x7f(xpos /*15*/, 18, "%s %02d", dayNames[weekday], RTC_DateStructure.RTC_Date);

				break;
			}
			case DISPLAY_ADJ_TIMEDATE:
			{
				UG_FillScreen(C_BLACK);
				if (mode == ADJUST_TIME) {
					UG_PutString(16, 0, "Time");

					UG_FontSelect(font);
					int xofs = (RTC_TimeStructure.RTC_Hours < 10) ? 6 : 0;
					lcd_putString5x7f(xofs, 10, "%d:", RTC_TimeStructure.RTC_Hours);
					lcd_putString5x7f(37 - xofs, 10, "%02d", RTC_TimeStructure.RTC_Minutes);
				}
				if (mode == ADJUST_DATE) {
					UG_PutString(16, 0, "Date");

					lcd_putString5x7f(0, 10, "%02d", RTC_DateStructure.RTC_Date);
					UG_PutString(12, 10, ".");
					lcd_putString5x7f(24 - 6, 10, "%02d", RTC_DateStructure.RTC_Month);
					UG_PutString(36 - 6, 10, ".");
					lcd_putString5x7f(48 - 6 - 6, 10, "%04d", RTC_DateStructure.RTC_Year + 2000);
				}
				break;
			}
			case DISPLAY_TIMER:
			{
				UG_FillScreen(C_BLACK);
				UG_PutString(16, 0, "Timer");
				UG_FontSelect(font);
				if (sleeptmr) {
					lcd_putString5x7f(15, 11, "%2dh", sleeptmr);
				} else
					lcd_putString5x7f(15, 11, "off");
				break;
			}
			case DISPLAY_CONTRAST:
			{
				UG_FillScreen(C_BLACK);
				UG_PutString(0, 0, "Contrast");
				UG_FontSelect(font);
				lcd_putString5x7f(15, 11, "%2d", contrast);
				break;
			}
			case DISPLAY_BRIGHTNESS:
			{
				UG_FillScreen(C_BLACK);
				lcd_putString5x7(0, 0, "Bright");
				UG_FontSelect(font);
				lcd_UG_PutStringf(0, 11, "%4d", pwmval);
				break;
			}
			case DISPLAY_ICON:
			{
				UG_FillScreen(C_BLACK);
				lcd_putIcon(SCREEN_WIDTH - 24, 4, icons[selectedicon]);
				switch (selectedicon) {
				case ICON_TIME:
					lcd_putString5x7(4, 12, "Time");
					break;
				case ICON_DATE:
					lcd_putString5x7(4, 12, "Date");
					break;
				case ICON_TIMER:
					lcd_putString5x7(0, 12, "Timer");
					break;
				case ICON_BRIGHTNESS:
					lcd_putString5x7(0, 12, "Bright");
					break;
				case ICON_CONTRAST:
					lcd_putString5x7(0, 12, "Contr");
					break;
				case ICON_BACK:
					lcd_putString5x7(4, 12, "Back");
					break;

				}
			}

			}
			lcd_update();
		}
	}
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
