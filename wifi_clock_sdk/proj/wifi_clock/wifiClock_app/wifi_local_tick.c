/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      wifi_local_tick.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-06 09:36:02
 * ModifiedTime:  2017-06-06 09:36:02
 */
#include <common.h>
#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#include <wifi_clock_cfg.h>
#include <arch/ba22/chip.h>
#include <arch/irq.h>
#include <ui_display.h>
#include <alarm_setting.h>
#include <cfg_api.h>

#if !RTC_TIME
TlocalTime localTime;
#define CHECK_TIME	4
static void init_local_time()
{
	localTime.totalSeconds = 0;
	localTime.currentSeconds = 0;
	localTime.secondCheckTime = 0;
	memset(localTime.time,0,sizeof(sntp_tm));
}

static inline void check_localTime_overflow()
{
	if(localTime.currentSeconds > (86400 - 1)) {
		localTime.currentSeconds = 0;
	}
}
#if (VERSION_MUSHAN)
static inline time_setup()
{
extern sdk_param sdkParam;
	int temp;
	temp = localTime.time.tm_year + 1900;
#if (VERSION_DEMO || CUSTOMER_VERSION)
	ui_setup(&temp,YEAR_DATA);
	ui_setup(&localTime.time.tm_mon,MONTH_DATA);
	ui_setup(&localTime.time.tm_mday,DAY_DATA);
	ui_setup(&localTime.time.tm_wday,WEEK_DATA);
#endif
	if (sdkParam.timeFmt_param.fmt == FORMAT_24) {
		ui_setup(&localTime.time.tm_hour,HOUR_DATA);
	} else {
		temp = localTime.time.tm_hour;
		if (localTime.time.tm_hour > 12) {
			temp = localTime.time.tm_hour - 12;
		}
		ui_setup(&temp,HOUR_DATA);
	}
	ui_setup(&localTime.time.tm_min,MIN_DATA);
//	ui_setup(&noData,WEATHER_CHAR_DATA);
	/* Wifi link statue show up */
extern char s_linkup;
	if (s_linkup == CONNECT) {
		ui_setup(&noData,WIFI_FLAG_DATA);
	}
#if (MU_SHAN_API == 1)	
	ui_setup(&wifiPm25.aqiValue,PM2_5_CHAR_DATA);
#endif		
	ui_setup(&noData,HALF_SECOND_FLASH);		
}
#endif

void local_tick(void)
{
	check_localTime_overflow();
	if (localTime.secondCheckTime == CHECK_TIME) {
		localTime.secondCheckTime = 0;
		if (localTime.totalSeconds != 0) {
			localTime.totalSeconds++;
			localTime.currentSeconds++;
			parse_time(localTime.totalSeconds,&localTime.time);
			//dump_ntp_time(localTime.time);
			
#if (CUSTOMER_VERSION || VERSION_DEMO)
			FD612_printf_test(localTime.time);
#endif		
		}
		//printf("\r\n %d \r\n",localTime.totalSeconds);	
	}
#if (VERSION_MUSHAN)
	if (localTime.totalSeconds != 0) {
		if (alarmSettingParam.configModeFlag == ALARM_NORMAL) {
//			printf("time setup \n");
			time_setup();	
		} else {
			alarm_setting();
		}
		ui_display();
	}
#endif	
}


void get_local_time(void* prama)
{
	unsigned int delta_time;
	unsigned int current_rtc;

	printf("\r\n==========starting get local time =========\r\n");
	init_local_time();	
	ui_display_init();
	//hw_timer_start(US_PER_SEC,local_tick,0);
	while(1) {
		localTime.secondCheckTime++;
		local_tick();
		sys_msleep(250);
	}
}
#endif

