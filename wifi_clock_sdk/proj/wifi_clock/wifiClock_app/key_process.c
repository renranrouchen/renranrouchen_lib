/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      key_process.c
 * Author:        zwxf
 * Email:         zwxf.cw@gmail.com
 * DateTime:      2017-10-18 08:41:17
 * ModifiedTime:  2017-10-18 08:41:17
 */
#include <common.h>
#include <wifi_clock_cfg.h>
#include <key_process.h>
#include <alarm_setting.h>
#include <alarm.h>
#include <madc.h>
#include <FD612demo_app.h>
#include <ui_display.h>

#if VERSION_MUSHAN

char keyTable[4] = {KEY_UP_PIN,KEY_DOWN_PIN,KEY_ALARM_SETTING_PIN,KEY_BRIGHTNESS_PIN};
unsigned char keyPressFlag = false;
unsigned char silencePressed = false;

static void inline alarm_time_process(void)
{
	if (v2Alarm.alarmHour > 23) {
		v2Alarm.alarmHour = 0;
	} else if (v2Alarm.alarmHour < 0) {
		v2Alarm.alarmHour = 23;
	}
	if (v2Alarm.alarmMin > 59) {
		v2Alarm.alarmMin = 0;
	} else if (v2Alarm.alarmMin < 0) {
		v2Alarm.alarmMin = 59;
	}		
	v2Alarm.defaultAlarmTime = v2Alarm.alarmHour * 60 * 60 + v2Alarm.alarmMin * 60;
	v2Alarm.alarmTime = v2Alarm.defaultAlarmTime;
	printf("alarm time %d \n",v2Alarm.alarmTime);
}

void key_pressed_process()
{
extern char isAlarm;
	keyPressFlag = true;
	if (isAlarm == true) {
		delete_v2_alarm();
	}
	printf("key pressed process \n");
}
void silence_key_process()
{
	if (v2Alarm.isSnoozeState == false) 
	{
		if (silencePressed == true ) {
			v2Alarm.isOpenAlarm = ALARM_CLOSE;
			// printf("silence key pressed !!!\n");
			delete_v2_alarm();
			noData = OFF;
			ui_setup(&noData,ALARM_FLAG_DATA);
		} else {
			noData = ON;
			v2Alarm.isOpenAlarm = ALARM_OPEN;
			ui_setup(&noData,ALARM_FLAG_DATA);
		}
	}
	else 
	{
		v2Alarm.isOpenAlarm = ALARM_OPEN;
		if (is_half_second()) 
		{
			noData = ON;		
		}
		else 
		{
			noData = OFF;	
		}
		if (digital_read(KEY_SILENCE_PIN) == KEY_PRESSED){
			delete_v2_alarm();v2Alarm.isOpenAlarm = ALARM_CLOSE;
			v2Alarm.isSnoozeState =false;
			}
		ui_setup(&noData,ALARM_FLAG_DATA);
	} 
	// printf("silence key pressed \n");
}

void up_key_process()
{
	
	
	
	printf("up_key key pressed \n");
	
	switch (alarmSettingParam.configModeFlag) {
		case ALARM_CONFIG_HOUR:
			v2Alarm.alarmHour++;
			break;
		case ALARM_CONFIG_MIN:
			v2Alarm.alarmMin++;
			break;
		default:
			break;
	}
	alarm_time_process();
}

void down_key_process()
{
	
	
	
	printf("down_key key pressed \n");
	switch (alarmSettingParam.configModeFlag) {
		case ALARM_CONFIG_HOUR:
			v2Alarm.alarmHour--;
			break;
		case ALARM_CONFIG_MIN:
			v2Alarm.alarmMin--;	
			break;
		default:
			break;
	}
	alarm_time_process();
}
//static unsigned char  alarmSettingIndex = 0;
void alarm_setting_key_process()
{	
	extern char isAlarm;
	if (isAlarm == true) {
		return;
	}
	if (++alarmSettingParam.configModeFlag >= ALARM_CONFIG_COMFIRM) {
		update_alarm_param_to_flash();
		v2Alarm.snoozeCount = 0;
		v2Alarm.isSnoozeState = false;
		alarmSettingParam.configModeFlag = ALARM_NORMAL;
	}
	printf("alarm mode %d",alarmSettingParam.configModeFlag);
}
void brightness_key_process()
{
extern unsigned int displayIntense;
	static unsigned char brightnessMode = 1;
	printf("brightness key pressed \n");
	if (brightnessMode++ >= 4) {
		brightnessMode = 1;
	}
	switch (brightnessMode) {
		case 1:
			displayIntense = FD612_INTENS1;		
			break;
		case 2:	
			displayIntense = FD612_INTENS3;		
			break;
		case 3:
			displayIntense = FD612_INTENS5;	
			break;
		case 4:
			displayIntense = FD612_INTENS7; 
			break;
	}
}

#endif

