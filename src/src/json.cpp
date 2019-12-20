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

#include "stm32f10x.h"
#include "ArduinoJson.h"
#include <stdarg.h>
#include <stdio.h>
#include "rtc.h"
#include "main.h"
extern "C" void usb_transmitString(char* msg);

extern RTC_TimeTypeDef_H12 RTC_TimeStructure;
extern RTC_DateTypeDef RTC_DateStructure;

static char weekdays[8][4]={"", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};


static char txbuf[512];
static int txwrptr;

void getScreenString(char* buf) {
	*buf=0;
}

void add(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int written = vsnprintf(&txbuf[txwrptr], 512 - txwrptr, format, args);
	if (written < 512 - txwrptr && written > 0)
		txwrptr += written;
	va_end(args);
}

//static char dayNames[][12] = { "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So" };
static char dayNames[][12] = { "H", "K", "Sze", "Cs", "P", "Szo", "V" };

uint8_t json_process(char* rxbuf) {
	StaticJsonBuffer<512> jsonIn;
	JsonObject& root = jsonIn.parseObject(rxbuf);

	txwrptr = 0;
	memset(txbuf, 0, sizeof(txbuf));

	do {
		if (!root.success()) {
			add("{");
			add("'cmd':'status',");
			add("'status':'error'");
			add("}");
			//printf("%.*s", sbuf.pcount(), sbuf.str());
			usb_transmitString(txbuf);
			break;
		}

		add("{");
		add("'cmd':'%s',", (const char*) root["cmd"]);
		do {
			int weekday = rtc_weekday(RTC_DateStructure.RTC_Year + 2000, RTC_DateStructure.RTC_Month,
									RTC_DateStructure.RTC_Date);

			if (root["cmd"] == "status") {
				add("'time':'%d:%02d:%02d',",
						RTC_TimeStructure.RTC_Hours,
						RTC_TimeStructure.RTC_Minutes,
						RTC_TimeStructure.RTC_Seconds);
				add("'date':'%04d-%02d-%02d %s',",
						RTC_DateStructure.RTC_Year + 2000,
						RTC_DateStructure.RTC_Month,
						RTC_DateStructure.RTC_Date,
						dayNames[weekday]
						);
/*				add("'pwmw':%d,", fadectrw);
				add("'pwmr':%d,", fadectrr);
				add("'display':%d,", displaystate);
*/
				char screen[128]={0};
				getScreenString(screen);
				add("'screen':'%s',", screen);
			}
		} while (0);
		add("'status':'ok'");
		add("}");
		usb_transmitString(txbuf);

	} while (0);

	return 1;
}

extern "C" uint8_t process_json_c(char* rxbuf) {
	return json_process(rxbuf);
}



