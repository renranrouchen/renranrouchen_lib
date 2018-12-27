/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      event_loop.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-19 10:03:42
 * ModifiedTime:  2017-06-19 10:03:42
 */
#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H
#include <sdk_config.h>

typedef enum event_type {
	WLAN_UP_EVENT	= 0x00,
	WLAN_DOWN_EVENT,
	MEASURE_LOCAL_TEMP_EVENT,
	ALARM_START_EVENT,
	CONFIG_KEY_EVENT,
	KEY_PRESSED_EVENT,
	UP_KEY_EVENT,
	DOWN_KEY_EVENT,
	ALARM_SETTING_KEY_EVENT,
	BRIGHTNESS_KEY_EVENT,
	SILENCE_KEY_EVENT,
	CLAP_ONE_EVENT,
	VIBRATE_EVENT,
	CLAP_TWO_EVENT,
	APP_CONFIG_DONE,
	OTA_EVENT,
	ENABLE_POWER_SAVE,
	DISABLE_POWER_SAVE,
	EVENT_TEST,
	START_OTA_EVENT,
	REBOOT_EVENT,
	CONFIG_MODE,
#if (GET_RH_BY_PWM == 1) 	
	GET_RH_ADC_VALUE,
#endif
	GET_LOCAL_TEMP_VALUE,
	EVENT_MAX,	
}Eevent;
typedef void (*event_func)(int event);

typedef struct {
	Eevent event; 
}Tevent;

void send_event(Eevent event);
char event_init();
inline void add_event(int event,event_func cbFunc);
inline void remove_event(int event,event_func cbFunc);

#endif 
