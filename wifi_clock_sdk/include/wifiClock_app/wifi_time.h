/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      wifi_time.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-02 09:04:00
 * ModifiedTime:  2017-06-02 09:04:00
 */
#ifndef WIFI_TIME_H
#define WIFI_TIME_H
#include "ntp/ntpd.h"

#include "wifi_weather.h"

#undef printf
#define printf(fmt,args...)		serial_printf(fmt ,##args)

typedef struct {
	TupDateControl update;
	sntp_tm time;
	char firstEntryCheck;
}TwifiTime;
extern TwifiTime wifiTime;




void get_ntp_time();
void dump_ntp_time(sntp_tm time);

//void FD612_printf_test(sntp_tm time);


#endif


