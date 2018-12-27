/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      alarm.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-25 14:47:04
 * ModifiedTime:  2017-05-25 14:47:04
 */

#ifndef ALARM_H
#define ALARM_H
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <sdk_config.h>
//#include <cfg_api.h>

enum {
	ALARM_CLOSE = 0,
	ALARM_OPEN = 1,
};
#if !VERSION_MUSHAN
typedef struct {
	char alarmDayIndex;
	char snoozeKey;
	char hasOpenedAlarm;

	unsigned char alarmPositionIndex;
	unsigned char snoozeTimes[21];

	SemaphoreHandle_t pHandleMutex;
	char pHandleIndex;
	TaskHandle_t pHandle;
	
	//char* rtcTime;
}Talarm;
extern Talarm alarm;
Talarm get_alarm();
void init_alarm();
extern void start_alarm();
extern void delete_alarm();
#endif

#if VERSION_MUSHAN
#define SNOOZE_COUNT_MAX	5
typedef struct {
	unsigned char snoozeCount;
	char isSnoozeState;
	char isOpenAlarm;
	unsigned int defaultAlarmTime;
	unsigned int alarmTime;
	int alarmHour;
	int alarmMin;
	TaskHandle_t pAlarmHandle;
}t_V2_alarm;
extern t_V2_alarm v2Alarm;
extern void reinit_alarm_param_to_default();
extern void update_alarm_param_to_flash();
extern void excute_alarm_check(void* param);
#endif


#endif
