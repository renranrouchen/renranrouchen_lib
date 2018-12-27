/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      alarm_setting.h
 * Author:        zwxf
 * Email:         zwxf.cw@gmail.com
 * DateTime:      2017-10-18 08:43:07
 * ModifiedTime:  2017-10-18 08:43:07
 */
#ifndef ALARM_SETTING_H
#define ALARM_SETTING_H
#include <sdk_config.h>
#if VERSION_MUSHAN
typedef enum {
	ALARM_NORMAL = 0,
	ALARM_CONFIG_HOUR,
	ALARM_CONFIG_MIN,
	ALARM_CONFIG_COMFIRM,
	ALARM_CONFIG_MAX,
} ALARMSETTING_CONFIG_MODE;


#define CONFIG_COUNT_DOWM_TIME 20
typedef struct _config_param {
	unsigned int configCountDown;
	unsigned char configModeFlag;
} t_alarmseting_config_param;

extern t_alarmseting_config_param alarmSettingParam;


extern void alarm_setting();

#endif
#endif

