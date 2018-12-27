/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf.cw@gmail.com
*@Brief			
*@FileName		event_process.c
*@Data			2017/8/21 11:40:34
-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf.cw@gmail.com
*@Brief			
*@FileName		event_process.c
*@Data			2017/7/19 15:17:01
-------------------------------------------------------------------------*/
#include <common.h>
#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif

//#include <log/log.h>
#include <http.h>
#include "event_process.h"
#include "FD612demo_app.h"
#include <alarm.h>
#include <key_process.h>
#include <wifi_clock_cfg.h>
#include "cfg_api.h"

extern sdk_param sdkParam;
extern void start_pwm_alarm(void* param);
enum {
	POWER_SAVE_LOCKED = -1,
	POWER_SAVE_UNLOCKED = 0,
	FIRST_TIME_ENTRY = 2,
};
char powerSaveCount = POWER_SAVE_LOCKED;
char firstEntry = FIRST_TIME_ENTRY;

void vibrate_handle()
{
#if (VERSION_MUSHAN == 0)
	delete_alarm();
#endif
//	printf("\r\n handle vibrate \r\n");
}

void clap_one_handle()
{
	fd612_display_on();
	FD612Test.countToDispOff = COUNT_TO_OFF(20);
	FD612Test.countToDispModeOne = COUNT_TO_OFF(30);
//	printf(" \r\n handle clap one \r\n");
}


inline void  ENTER_POWER_SAVE()
{
	if (powerSaveCount == POWER_SAVE_LOCKED) {	
		powerSaveCount = POWER_SAVE_UNLOCKED;	
	}	
	if (powerSaveCount == POWER_SAVE_UNLOCKED) {	
		send_event(ENABLE_POWER_SAVE);	
	}	
	++powerSaveCount;	
}

inline void EXIT_POWER_SAVE()
{
	if (powerSaveCount > POWER_SAVE_UNLOCKED) {	
		--powerSaveCount;	
	}	
	if (powerSaveCount == POWER_SAVE_UNLOCKED) {	
		powerSaveCount = POWER_SAVE_LOCKED;	
		send_event(DISABLE_POWER_SAVE);	
	}	
}

void enter_power_save()
{
//	printf("enter power save \n!!!");
//	wlan_enable_powersave();	
	wlan_power_off();
//	wlan_start_scan();
}

void exit_power_save()
{
//	printf("exit power save \n!!!");
//	wlan_disable_powersave();
	wlan_power_on();
	wlan_start_scan();
}
void reboot_process()
{
	reboot(1);
}
void config_mode()
{
	sdkParam.configMode = true;
	config_set(&sdkParam);
	config_submit();
	reboot(1);
	printf("--------------------------------->config mode");
}

void event_process()
{
	event_init();
	//add_event(OTA_EVENT,start_ota);
#if (VERSION_MUSHAN == 0)
	add_event(VIBRATE_EVENT,vibrate_handle);
	add_event(CLAP_ONE_EVENT,clap_one_handle);
	//add_event(CONFIG_KEY_EVENT,config_key_handle);
#endif
#if (VERSION_MUSHAN == 1)
	add_event(SILENCE_KEY_EVENT,silence_key_process);
	add_event(BRIGHTNESS_KEY_EVENT,brightness_key_process);
	add_event(ALARM_SETTING_KEY_EVENT,alarm_setting_key_process);
	add_event(DOWN_KEY_EVENT,down_key_process);
	add_event(UP_KEY_EVENT,up_key_process);
	add_event(KEY_PRESSED_EVENT,key_pressed_process);	
	add_event(GET_LOCAL_TEMP_VALUE,get_local_tmp);
#endif	
#if POWER_SAVE_EN
	add_event(ENABLE_POWER_SAVE,enter_power_save);
	add_event(DISABLE_POWER_SAVE,exit_power_save);
#endif
	add_event(REBOOT_EVENT,reboot_process);
	add_event(CONFIG_MODE,config_mode);
}


