/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      rtc_app.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-25 14:49:03
 * ModifiedTime:  2017-05-25 14:49:03
 */

#include <common.h>

#if CONFIG_FREERTOS
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#endif

#include "rtc_time.h"
#include "wifi_time.h"
#include <log/log.h>
#include <ntp/ntpd.h>
t_rtctime rtcTime;
//extern sdk_param g_atParam;

#if (RTC_TIME == 1)
void dump_rtc_time(t_time *time)
{
	printf("-------> rtc time hour %d min %d \n",time->hour,time->min);
}

static void inline init_rtc_time()
{
	rtcTime.getRtc = &rtc;
	rtcTime.currentSeconds = 0;
}

inline void updata_rtc_time(void *time)
{
	printf(" \r\n starting update rtc time \r\n ");
	sntp_tm *wifiTime = time;
	update_rtc_time(wifiTime);
}

void get_current_time_task(void *param)
{
	/* according to rtc struct to get time */

	/*FIXME:
	*add semephore to lock read time operate to avoid muilty operate
	*/
	FD612_app_init();
	init_rtc_time();
	start_rtc();
	while (1)
	{
		read_rtc_time();
		dump_rtc_time(&rtcTime.getRtc->time);
		if (rtcTime.getRtc != NULL)
		{
			rtcTime.currentSeconds = rtcTime.getRtc->time.hour * 60 * 60 + rtcTime.getRtc->time.min * 60 + rtcTime.getRtc->time.second;
		}
		else
		{
			printf(" \r\n init rtc \r\n");
		}
		FD612_printf_test((void *)&rtcTime.getRtc->time);
		sys_msleep(500);
	}
}
#endif
