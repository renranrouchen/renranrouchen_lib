/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      wifi_local_tick.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-06 10:55:09
 * ModifiedTime:  2017-06-06 10:55:09
 */

#ifndef WIFI_LOCAL_TICK_H
#define WIFI_LOCAL_TICK_H
#include "ntp/ntpd.h" 

typedef struct local_time{
	sntp_tm time;
	sntp_time_t totalSeconds;
	unsigned char secondCheckTime;
	unsigned long currentSeconds;	
}TlocalTime;

extern TlocalTime localTime;

void get_local_time();


#endif

