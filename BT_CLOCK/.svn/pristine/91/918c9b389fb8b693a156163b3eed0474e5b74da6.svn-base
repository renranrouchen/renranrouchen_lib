/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      rtc.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-21 09:15:02
 * ModifiedTime:  2017-06-21 09:15:02
 */
#ifndef RTC_H
#define RTC_H

typedef struct rtc_time {
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char week;
	unsigned char hour;
	unsigned char min;
	unsigned char second;
}rtc_time_t;
void read_rtc_time(rtc_time_t* rtc);
void update_rtc_time(rtc_time_t* time);
unsigned char read_alarm_flag(void);
void read_alarm_time(rtc_time_t *alarm);
void update_alarm_flag(bool flag);
void update_alarm_time(rtc_time_t *alarm);
void start_rtc();
int read_fm_freq(void);
void update_fm_freq(int freq);
#endif


