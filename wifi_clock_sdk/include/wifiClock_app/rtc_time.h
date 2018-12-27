/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      rtc_app.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-25 14:49:14
 * ModifiedTime:  2017-05-25 14:49:14
 */

#ifndef RTC_TIME_H
#define RTC_TIME_H
#include <rtc.h>

enum{
	SUN = 0,
	MON,		
	TUE,
	WED,
	THU,
	FRI,
	SAT,
};
typedef struct {
	/* current tictock by secs*/
	unsigned long currentSeconds;
	t_rtc* getRtc;
	
}t_rtctime;

extern t_rtctime rtcTime;
/* get static rtc time */
void updata_rtc_time(void* time);
void get_current_time_task(void* param);




#endif

