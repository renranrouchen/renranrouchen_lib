/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      alarm.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-25 14:46:55
 * ModifiedTime:  2017-05-25 14:46:55
 */
#include <common.h>
#ifdef CONFIG_FREERTOS
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#endif
#include <rtc_time.h>
#include <log/log.h>
#include <cfg_api.h>
#include "alarm.h"
#include <wifi_clock_cfg.h>
extern sdk_param sdkParam;

extern void start_pwm_alarm();
UPDATE_TYPE checkUpdate = KILL_UPDATE;
inline void excute_alarm(void *pHandle)
{
	/*FIXME:
	* notice pHandleIndex value ,we should take measure to avoid Concurrency
	*/
	xTaskCreate(start_pwm_alarm, "excSound", 1024 * 2, NULL, 0, pHandle);
}

#if !VERSION_MUSHAN
Talarm alarm;
void init_alarm()
{
	unsigned char loop = 0;
	alarm.alarmDayIndex = -1;
	alarm.hasOpenedAlarm = -1;
	/* hardware specify */
	alarm.snoozeKey = -1;

	alarm.pHandle = NULL;
	alarm.pHandleIndex = 0;

	for (loop = 0; loop < ALARM_NUM; loop++)
	{
		alarm.snoozeTimes[loop] = 0;
	}
}

Talarm get_alarm()
{
	return alarm;
}

void delete_alarm()
{
	unsigned char loop;
	/* reinit all alarm param */
	extern char isAlarm;
	isAlarm = 0;
	if (alarm.pHandle != NULL)
	{
		for (loop = 0; loop < ALARM_NUM; loop++)
		{
			alarm.snoozeTimes[loop] = 0;
			sdkParam.alm_param[loop].alarmTime = sdkParam.alm_param[loop].defaultAlarmTime;
		}
		/* delete all alarm task */
		printf("delet handle \r\n");
		alarm.pHandleIndex = 0;
		vTaskDelete(alarm.pHandle);
	}
}
void create_alarm(char alarmIndex)
{
	/*TODO:
	*add snooze time check,3 times
	*/
	if (alarm.pHandle != NULL)
	{
		vTaskDelete(alarm.pHandle);
		alarm.pHandle = NULL;
		//arm.snoozeTimes = 0;
	}
	if (sdkParam.alm_param[alarmIndex].isOpenSnooze == OPEN)
	{
		if (alarm.snoozeTimes[alarmIndex] < 3)
		{
			sdkParam.alm_param[alarmIndex].alarmTime += sdkParam.alm_param[alarmIndex].snoozeDelay;
			alarm.snoozeTimes[alarmIndex] += 1;
		}
		else
		{
			sdkParam.alm_param[alarmIndex].alarmTime = sdkParam.alm_param[alarmIndex].defaultAlarmTime;
		}
	}
	excute_alarm(&alarm.pHandle);
}
static inline void get_alarm_day(unsigned char week)
{
	/* day value default to 0-6 means a week */
	alarm.alarmDayIndex = (week)*3;
}

UPDATE_TYPE check_alarm_time(unsigned long *seconds)
{
	unsigned char loop = 0;
	unsigned char alarmDayMax = alarm.alarmDayIndex + 3;
	for (loop = alarm.alarmDayIndex; loop < alarmDayMax; loop++)
	{
		if (sdkParam.alm_param[loop].isOpen == ALARM_CLOSE)
		{
			continue;
		}
		//printf(" alarm time %d current time %d",sdkParam.alm_param[loop].alarmTime,rtcTime.currentSeconds);
		if (sdkParam.alm_param[loop].alarmTime == *seconds)
		{
			alarm.alarmPositionIndex = loop;
			return START_UPDATE;
		}
	}
	return WAIT_UPDATE;
}

void start_alarm()
{

	init_alarm();
	/* init DAC convert */

	UPDATE_TYPE checkUpdate = KILL_UPDATE;
	while (1)
	{
#if RTC_TIME
		get_alarm_day((rtcTime.getRtc->time.week - 1));
		checkUpdate = check_alarm_time(&rtcTime.currentSeconds);
#else
		get_alarm_day(localTime.time.tm_wday - 1);
		checkUpdate = check_alarm_time(&localTime.currentSeconds);
#endif
		switch (checkUpdate)
		{
		case START_UPDATE:
			create_alarm(alarm.alarmPositionIndex);
			break;
		case WAIT_UPDATE:
			/* sleep 520 secs */
			//printf("check wait \r\n");
			sys_msleep(520);
			break;
		case KILL_UPDATE:
			vTaskDelete(NULL);
			break;
		default:;
		}
	}
}
#endif
#if VERSION_MUSHAN
#define SOOOZE_DELAY_TIME 300
t_V2_alarm v2Alarm;

void reinit_alarm_param_to_default()
{
	printf("alarm config timeout \n");
	v2Alarm.alarmTime = sdkParam.v2_alarm_param.defaultAlarmTime;
	v2Alarm.defaultAlarmTime = sdkParam.v2_alarm_param.defaultAlarmTime;
	if (sdkParam.v2_alarm_param.defaultAlarmTime != 11210505)
	{
		v2Alarm.alarmHour = v2Alarm.alarmTime / 3600;
		v2Alarm.alarmMin = v2Alarm.alarmTime % 3600 / 60;
	}
	else
	{
		v2Alarm.alarmHour = 0;
		v2Alarm.alarmMin = 0;
	}
	printf("-------------------------->default alarm value %d \n", v2Alarm.defaultAlarmTime);
}
void update_alarm_param_to_flash()
{
	sdkParam.v2_alarm_param.defaultAlarmTime = v2Alarm.alarmTime;
	sdkParam.v2_alarm_param.isOpenAlarm = true;
	config_set(&sdkParam);
	config_submit();
	config_dump();
}

void create_v2_alarm()
{
	if (v2Alarm.pAlarmHandle != NULL)
	{
		vTaskDelete(v2Alarm.pAlarmHandle);
		v2Alarm.pAlarmHandle = NULL;
	}
	excute_alarm(&v2Alarm.pAlarmHandle);
}

void delete_v2_alarm()
{

	extern char isAlarm;
#if 1
	isAlarm = false;
	if (v2Alarm.pAlarmHandle != NULL)
	{
		printf("delete alarm ............\n");
		vTaskDelete(v2Alarm.pAlarmHandle);
		v2Alarm.pAlarmHandle = NULL;
	}
#endif
	extern void pwm_init(unsigned char pin);
	pwm_init(2);
}

void enter_snooze_state()
{
	v2Alarm.snoozeCount++;
	v2Alarm.alarmTime += SOOOZE_DELAY_TIME;
	v2Alarm.isSnoozeState = true;
	if (v2Alarm.snoozeCount >= SNOOZE_COUNT_MAX)
	{
		v2Alarm.snoozeCount = 0;
		v2Alarm.isSnoozeState = false;
		v2Alarm.alarmTime = v2Alarm.defaultAlarmTime;
	}
}

UPDATE_TYPE is_alarm_time(unsigned int currTime)
{
	/* Check update already */
	if (localTime.totalSeconds != 0)
	{
		if (v2Alarm.isOpenAlarm == ALARM_OPEN)
		{
			//			printf("current time %d,alarm time %d \n",currTime,v2Alarm.alarmTime);
			if (currTime == v2Alarm.alarmTime)
			{
				enter_snooze_state();
				return START_UPDATE;
			}
		}
		else
		{
			v2Alarm.snoozeCount = 0;
			v2Alarm.isSnoozeState = false;
			v2Alarm.alarmTime = v2Alarm.defaultAlarmTime;
			goto exit;
		}
		 
	}
exit:
	return WAIT_UPDATE;
	if (v2Alarm.isOpenAlarm == ALARM_CLOSE)delete_v2_alarm();
}

static void inline init_v2_alarm()
{
	v2Alarm.defaultAlarmTime = sdkParam.v2_alarm_param.defaultAlarmTime;
	v2Alarm.isSnoozeState = false;
	v2Alarm.snoozeCount = 0;
	v2Alarm.alarmTime = sdkParam.v2_alarm_param.defaultAlarmTime;
	if (sdkParam.v2_alarm_param.defaultAlarmTime != 11210505)
	{
		v2Alarm.alarmHour = v2Alarm.alarmTime / 3600;
		v2Alarm.alarmMin = v2Alarm.alarmTime % 3600 / 60;
	}
	else
	{
		v2Alarm.alarmHour = 0;
		v2Alarm.alarmMin = 0;
	}
	v2Alarm.isOpenAlarm = sdkParam.v2_alarm_param.isOpenAlarm;
	v2Alarm.pAlarmHandle = NULL;
	extern void pwm_init(unsigned char pin);
	pwm_init(2);
}

void excute_alarm_check(void *param)
{
	//	printf("alarm check \n");
	init_v2_alarm();
	// UPDATE_TYPE checkUpdate = KILL_UPDATE;
	while (1)
	{
		checkUpdate = is_alarm_time(localTime.currentSeconds);
		switch (checkUpdate)
		{
		case START_UPDATE:
			create_v2_alarm();
			break;
		case WAIT_UPDATE:
			/* sleep 520 secs */
			//printf("check wait \r\n");
			sys_msleep(520);
			break;
		case KILL_UPDATE:
			delete_v2_alarm();
			vTaskDelete(NULL);
			break;
		default:
			break;
		}
	}
}
#endif
