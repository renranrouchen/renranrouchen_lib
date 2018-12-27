/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      nowtime.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-13 08:42:09
 * ModifiedTime:  2017-06-13 08:42:09
 */
#include <common.h>
#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#include <cfg_api.h>

#include <nowtime.h>
#include <rtc_time.h>
#include <wifi_clock_cfg.h>
extern void start_pwm_alarm(void *param);

extern sdk_param sdkParam;

static const int nowtineTable[24] = {3600, 7200, 10800, 14400, 18000, 21600, 25200, 28800, 32400,
									 36000, 39600, 43200, 46800, 50400, 54000, 57600, 61200, 64800,
									 68400, 72000, 75600, 79200, 82800, 86400};

inline void start_nowtime_alarm()
{
	xTaskCreate(start_pwm_alarm, "startNowtime", 1024, NULL, 0, NULL);
}

char check_nowtime(unsigned long *Seconds)
{
	char loop = 0;
	if (sdkParam.nt_param.isOpen == 0)
	{
		return -1;
	}
	{
		for (loop = 0; loop < 24; loop++)
		{
			if (nowtineTable[loop] == *Seconds)
			{
				return 1;
			}
		}
	}
	return 0;
}

void start_alarm_nowtime(void *param)
{
	char retVal = -2;
	while (1)
	{
#if RTC_TIME
		retVal = check_nowtime(&rtcTime.currentSeconds);
#else
		retVal = check_nowtime(&localTime.currentSeconds);
#endif
		if (retVal == 1)
		{
			start_nowtime_alarm();
			/* sleep 59 mins for next check */
			sys_msleep(1000 * 60 * 59);
		}
		else if (retVal == 0)
		{
			sys_msleep(520);
		}
		else if (retVal == -1)
		{
			vTaskDelete(NULL);
		}
	}
}
