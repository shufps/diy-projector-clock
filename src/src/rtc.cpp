/* 
 * This file is part of DIY-Projector-Clock
 * www.microengineer.eu
 * 
 * If you like my work, please consider to donate IOTAs to:
 * 
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

#include "main.h"
#include "stm32f10x_rtc.h"
#include "rtc.h"

RTC_DateTypeDef date;
RTC_TimeTypeDef_H24 time;

void rtc_configuration(uint8_t deinit) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	if (deinit)
		BKP_DeInit();
	RCC_LSEConfig(RCC_LSE_ON);

	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {
	}

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();

	/* Enable the RTC Second */
//  RTC_ITConfig(RTC_IT_SEC, ENABLE);
	RTC_WaitForLastTask();
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	RTC_WaitForLastTask();
}


uint8_t rtc_isLeapYear(uint16_t nYear) {
	if ((nYear % 4) != 0) {
		return 0;
	}

	if ((nYear % 100) != 0) {
		return 1;
	}

	if ((nYear % 400) == 0) {
		return 1;
	} else {
		return 0;
	}
}

uint8_t rtc_daysPerMonth(int m, int y) {
	if (m<1 || m>12)
		return 0;

	uint8_t dpm[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	if (m==2)
		return dpm[m-1] + (rtc_isLeapYear(y) ? 1 : 0);
	else
		return dpm[m-1];
}

void rtc_adjustDate(RTC_DateTypeDef* date, int d) {
	if (!d) // NOP
		return;

	int day = date->RTC_Date;
	int month = date->RTC_Month;
	int year = date->RTC_Year + 2000;

	if (d>0) {
		d=1;
		day+=d;
		while (day>rtc_daysPerMonth(month, year)) {
			day-=rtc_daysPerMonth(month, year);
			month++;
			if (month>12) {
				month=1;
				year++;
			}
		}
	} else {
		d=-1;
		day+=d;
		while (day<1) {
			month--;
			if (month<1) {
				month=12;
				year--;
			}
			day+=rtc_daysPerMonth(month, year);
		}
	}

	date->RTC_Date = day;
	date->RTC_Month = month;
	date->RTC_Year = year - 2000;
}




void rtc_convert12to24(RTC_TimeTypeDef_H12 *time, RTC_TimeTypeDef_H24 *time24) {
	time24->RTC_Hours = time->RTC_Hours;
	time24->RTC_Minutes = time->RTC_Minutes;
	time24->RTC_Seconds = time->RTC_Seconds;

	switch (time->RTC_H12) {
	case RTC_H12_AM:
		// 12 becomes 0
		if (time->RTC_Hours == 12) {
			time24->RTC_Hours = 0;
		}
		break;
	case RTC_H12_PM:
		// 12 keeps 12
		if (time->RTC_Hours != 12) {
			time24->RTC_Hours += 12;
		}
		break;
	case RTC_H24:
		break;
	}
}

void rtc_convert24to12(RTC_TimeTypeDef_H24 *time24, RTC_TimeTypeDef_H12* time) {
	time->RTC_Hours = time24->RTC_Hours;
	time->RTC_Minutes = time24->RTC_Minutes;
	time->RTC_Seconds = time24->RTC_Seconds;

	if (/*time->RTC_Hours >=0 &&*/ time24->RTC_Hours <=11) // AM
	{
		if (time24->RTC_Hours == 0)
			time->RTC_Hours = 12;
		time->RTC_H12 = RTC_H12_AM;
	} else if (time24->RTC_Hours >=12 && time24->RTC_Hours <=23) {
		if (time24->RTC_Hours != 12)
			time->RTC_Hours -= 12;
		time->RTC_H12 = RTC_H12_PM;
	}
}

void rtc_setTime12(RTC_TimeTypeDef_H12* time) {
	RTC_TimeTypeDef_H24 time24;
	rtc_convert12to24(time, &time24);
	rtc_setTime24(&time24);
}


void rtc_getTime12(RTC_TimeTypeDef_H12* time) {
	RTC_TimeTypeDef_H24 time24;
	rtc_getTime24(&time24);
	rtc_convert24to12(&time24, time);
}

void rtc_setTime24(RTC_TimeTypeDef_H24* time24) {
	RTC_WaitForLastTask();
	RTC_SetCounter(time24->RTC_Hours * 3600 + time24->RTC_Minutes * 60 + time24->RTC_Seconds);
	RTC_WaitForLastTask();
}


void rtc_getTime24(RTC_TimeTypeDef_H24* time24) {
	RTC_WaitForLastTask();
	uint32_t cnt = RTC_GetCounter();
	RTC_WaitForLastTask();

	time24->RTC_Hours = cnt / 3600;
	cnt -= time24->RTC_Hours * 3600;
	time24->RTC_Minutes = cnt / 60;
	cnt -= time24->RTC_Minutes * 60;
	time24->RTC_Seconds = cnt;
}


void rtc_setDate(RTC_DateTypeDef* date) {
	BKP_WriteBackupRegister(BKP_DR2, (date->RTC_Date & 0xff) + ((date->RTC_Month & 0xff) << 8));
	BKP_WriteBackupRegister(BKP_DR3, date->RTC_Year & 0xffff);
}

void rtc_getDate(RTC_DateTypeDef* date) {
	uint16_t dr2 = BKP_ReadBackupRegister(BKP_DR2);
	uint16_t dr3 = BKP_ReadBackupRegister(BKP_DR3);

	date->RTC_Date = dr2 & 0xff;
	date->RTC_Month = (dr2 & 0xff00) >> 8;
	date->RTC_Year = dr3 & 0xffff;
}

void rtc_adjustTime(RTC_TimeTypeDef_H24* t, int d) {

	if (!d)	// nop
		return;



	int s = 0;
	int m = t->RTC_Minutes;
	int h = t->RTC_Hours;

	if (d > 0) {	// adjust forward
		d=1;
		m += d;
		while (m > 59) {
			m -= 60;
			h++;
			h %= 24;
		}
	} else { // adjust backward
		d=-1;
		m += d;
		while (m < 0) {
			m += 60;
			h--;
			if (h < 0)
				h = 23;
		}
	}
	t->RTC_Hours = h;
	t->RTC_Minutes = m;
	t->RTC_Seconds = s;
}

void rtc_checkUpdateDate(RTC_TimeTypeDef_H24* time, RTC_DateTypeDef* date) {
	RTC_WaitForLastTask();
	rtc_getDate(date);
	RTC_WaitForLastTask();
	rtc_getTime24(time);
	while (RTC_GetCounter() >= 86400) {
		RTC_WaitForLastTask();
		RTC_SetCounter(RTC_GetCounter()-86400);
		RTC_WaitForLastTask();
		rtc_getTime24(time); // should be 00:00:00

		switch (date->RTC_Date) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if (date->RTC_Date < 31) {
				date->RTC_Date++;
			} else if (date->RTC_Month != 12) {
				date->RTC_Month++;
				date->RTC_Date=1;
			} else {
				date->RTC_Month=1;
				date->RTC_Date=1;
				date->RTC_Year++;
			}
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if (date->RTC_Date < 30) {
				date->RTC_Date++;
			} else {
				date->RTC_Month++;
				date->RTC_Date=1;
			}
			break;
		case 2:
			if (date->RTC_Date < 28) {
				date->RTC_Date++;
			} else if (date->RTC_Date == 28) {
				if (rtc_isLeapYear(date->RTC_Year)) {
					date->RTC_Date++;
				} else {
					date->RTC_Month++;
					date->RTC_Date=1;
				}
			}
			if (date->RTC_Date == 29) {
				date->RTC_Month++;
				date->RTC_Date=1;
			}
			break;
		default:
			break;
		}
		RTC_WaitForLastTask();
		rtc_setDate(date);
	}
}

// This function will return week day number from 0 to 6
int rtc_weekday(int year, int month, int day) {
	int wday = 0;
	wday = (day + ((153 * (month + 12 * ((14 - month) / 12) - 3) + 2) / 5) + (365 * (year + 4800 - ((14 - month) / 12)))
			+ ((year + 4800 - ((14 - month) / 12)) / 4) - ((year + 4800 - ((14 - month) / 12)) / 100)
			+ ((year + 4800 - ((14 - month) / 12)) / 400) - 32045) % 7;
	return wday;
}


void rtc_init() {
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5A5A) {
		rtc_configuration(1);

		time.RTC_Hours = 13;
		time.RTC_Minutes = 37;
		time.RTC_Seconds = 0;

		rtc_setTime24(&time);

		date.RTC_Date = 10;
		date.RTC_Month = 10;
		date.RTC_Year = 17;

		rtc_setDate(&date);

		BKP_WriteBackupRegister(BKP_DR4, DEFAULT_PWM);
		BKP_WriteBackupRegister(BKP_DR5, DEFAULT_CONTRAST);
		BKP_WriteBackupRegister(BKP_DR1, 0x5A5A);
	} else {
#if 0
		/* Check if the Power On Reset flag is set */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			printf("\r\n\n Power On Reset occurred....");
		}
		/* Check if the Pin Reset flag is set */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			printf("\r\n\n External Reset occurred....");
		}
		printf("\r\n No need to configure RTC....");
#endif
		rtc_configuration(0);
		RTC_WaitForSynchro();

		/* Enable the RTC Second */
		//RTC_ITConfig(RTC_IT_SEC, ENABLE);

		rtc_checkUpdateDate(&time, &date);
		RTC_WaitForLastTask();
	}

}
