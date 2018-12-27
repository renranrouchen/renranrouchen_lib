/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf.cw@gmail.com
*@Brief			
*@FileName		alarm_setting.c
*@Data			2017/10/19 15:23:37
-------------------------------------------------------------------------*/
/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      alarm_setting.c
 * Author:        zwxf
 * Email:         zwxf.cw@gmail.com
 * DateTime:      2017-10-18 08:41:46
 * ModifiedTime:  2017-10-18 08:41:46
 */
#include <ui_display.h> 
#include <key_process.h>
#include <alarm_setting.h>
#include <alarm.h>

#if VERSION_MUSHAN
t_alarmseting_config_param alarmSettingParam;



void alarm_setting_init()
{
	alarmSettingParam.configCountDown = 0;
	alarmSettingParam.configModeFlag = ALARM_NORMAL;
}


void alarm_setting()
{
	/* Auto exit in CONFIG_COUNT_DOWM_TIME time if without key pressed msg */
	if (!keyPressFlag) {
		alarmSettingParam.configCountDown++;
		if (alarmSettingParam.configCountDown > CONFIG_COUNT_DOWM_TIME) {		
			// reinit_alarm_param_to_default();
			alarmSettingParam.configModeFlag = ALARM_NORMAL;
			alarmSettingParam.configCountDown = 0;
			return;
		}
	} else {
				
		alarmSettingParam.configCountDown = 0;
		/* we need clear the flag by myself */
		keyPressFlag = false;
	}
	

	switch (alarmSettingParam.configModeFlag) {
		case ALARM_CONFIG_HOUR:
			if (is_half_second()) {
				ui_setup(&v2Alarm.alarmMin,MIN_DATA);
				ui_setup(&v2Alarm.alarmHour,HOUR_DATA);
			} else {
				/* Buff 8 - 9*/
				noData = 8;
				ui_setup(&noData,DISPLAY_OFF);
				noData = 9;
				ui_setup(&noData,DISPLAY_OFF);
			}
			break;
		case ALARM_CONFIG_MIN:
			if (is_half_second()) {
				ui_setup(&v2Alarm.alarmHour,HOUR_DATA);
				ui_setup(&v2Alarm.alarmMin,MIN_DATA);
			} else {
				/* Buff 10 - 11*/
				noData = 10;
				ui_setup(&noData,DISPLAY_OFF);
				noData = 11;
				ui_setup(&noData,DISPLAY_OFF);
			}			
			break;		
		default:
			break;
	}
}

#endif
