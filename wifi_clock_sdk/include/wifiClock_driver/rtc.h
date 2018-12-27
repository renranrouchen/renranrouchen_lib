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

typedef struct {
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char week;
	unsigned char hour;
	unsigned char min;
	unsigned char second;
}t_time;

typedef struct {
	t_time time;
}t_rtc;

extern t_rtc rtc;
void read_rtc_time();
void update_rtc_time(void* time);
void start_rtc();


#endif


