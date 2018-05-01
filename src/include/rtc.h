/*
 * rtc.h
 *
 *  Created on: 10.10.2017
 *      Author: ne23kaj2
 */

#ifndef RTC_H_
#define RTC_H_

#define RTC_H12_AM                     ((uint8_t)0x00)
#define RTC_H12_PM                     ((uint8_t)0x40)
#define RTC_H24							((uint8_t) 0x80)


typedef struct {
	uint8_t RTC_Hours;
	uint8_t RTC_Minutes;
	uint8_t RTC_Seconds;
	uint8_t RTC_H12;

} RTC_TimeTypeDef_H12;

typedef struct {
	uint8_t RTC_Hours;
	uint8_t RTC_Minutes;
	uint8_t RTC_Seconds;
} RTC_TimeTypeDef_H24;

typedef struct {
	uint8_t RTC_WeekDay;
	uint8_t RTC_Month;
	uint8_t RTC_Date;
	uint8_t RTC_Year;
} RTC_DateTypeDef;

#ifdef __cplusplus
extern "C" {
#endif

void rtc_init();

void rtc_adjustTime(RTC_TimeTypeDef_H24* t, int d);
void rtc_adjustDate(RTC_DateTypeDef* date, int d);
uint8_t rtc_isLeapYear(uint16_t nYear);
void rtc_setTime12(RTC_TimeTypeDef_H12* time);
void rtc_getTime12(RTC_TimeTypeDef_H12* time);
void rtc_setTime24(RTC_TimeTypeDef_H24* time24);
void rtc_getTime24(RTC_TimeTypeDef_H24* time24);
void rtc_setDate(RTC_DateTypeDef* date);
void rtc_getDate(RTC_DateTypeDef* date);
void rtc_checkUpdateDate(RTC_TimeTypeDef_H24* time, RTC_DateTypeDef* date);
int rtc_weekday(int year, int month, int day);

#ifdef __cplusplus
}
#endif

#endif /* RTC_H_ */
