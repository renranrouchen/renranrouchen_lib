
/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      wifi_time.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-26 16:47:49
 * ModifiedTime:  2017-05-26 16:47:49
 */
#include <common.h>
#include <wla_api.h>
#include "log/log.h"

#include "wifi_time.h"
#include "FD612Drv.h"
//#include "wifi_local_tick.h"
#include "wifi_pm25.h"
#include <rtc_time.h>
#include <wifi_clock_cfg.h>
#include "event_process.h"

//#include "wifiClock_app/rtc_time.h"
extern char s_linkup;
#undef printf
#define printf(fmt,args...)		serial_printf(fmt ,##args)
#if (ENABLE_NTP_TIME == 1 || ENABLE_MUSHAN_TIME == 1)
TwifiTime wifiTime;

void dump_ntp_time(sntp_tm time)
{
	printf("hour %d,min %d,secs %d,isdst %d,year %d,mday %d,yday %d,wday %d,mon %d",
			time.tm_hour,
			time.tm_min,
			time.tm_sec,
			time.tm_isdst,
			time.tm_year,
			time.tm_mday,
			time.tm_yday,
			time.tm_wday,
			time.tm_mon
			);	
}


void wifi_time_callback(sntp_tm time,long timeVal)
{	
	int wifiSeconds = time.tm_hour * 60 * 60 + time.tm_min * 60 + time.tm_sec;
	printf(" wifisecond %d rtcsecond %d",wifiSeconds,rtcTime.currentSeconds);
#if RTC_TIME
	if (rtcTime.currentSeconds != wifiSeconds) {
		updata_rtc_time((void* )&time);
	}
	rtcTime.currentSeconds = wifiSeconds;
#else
	localTime.currentSeconds =  time.tm_hour * 60 * 60 + time.tm_min * 60 + time.tm_sec;
	localTime.totalSeconds = timeVal;
#endif
	dump_ntp_time(time);
	//sys_msleep(1000*60);
}

static inline UPDATE_TYPE check_wifiTime_update()
{
	if (wifiTime.update.updateFail == SUCCESS) {
		sys_msleep(wifiTime.update.updateInterval);
	}
	return START_UPDATE;
}

static void start_get_time()
{
	char serverIndex = 0;
	/*TODO:
	*add multiple ntp server
	*/
	while(getUTCtime(ntpServerTb[serverIndex],wifi_time_callback) < 0) { 
		if (serverIndex > SERVER_NUM_MAX) {
			serverIndex = 0;
		}
		serverIndex++;
		wifiTime.update.updateFail = FAIL;
		sys_msleep(1000 * 3);
	}
	if (wifiTime.firstEntryCheck != FIRST_ENTRY) {
		//ENTER_POWER_SAVE();
	}
	
	wifiTime.update.firstEntryCheck = NOT_FIRST_ENTRY;
	wifiTime.update.updateFail = SUCCESS;
	sys_msleep(100);
}

void get_ntp_time()
{
	UPDATE_TYPE updateType = KILL_UPDATE;
	/* update interval seconds */
	wifiTime.update.updateInterval = NTP_TIME_UPDATE_INTERVAL;
	wifiTime.update.updateFail = FAIL;
	wifiTime.update.firstEntryCheck = FIRST_ENTRY;
#if 1
	printf("start get ntp time \n");
	/* check wifi is linking or not */
	while(1) {
		updateType = check_wifiTime_update();
		switch (updateType) {
			case START_UPDATE:
				if (wifiTime.update.firstEntryCheck != FIRST_ENTRY) {
					EXIT_POWER_SAVE();
				}
				if (s_linkup == CONNECT ) {
					start_get_time();
				} else {
					sys_msleep(2000);
				}
			case WAIT_UPDATE:
				//debug("\r\n===520=====\r\n");
				sys_msleep(520);
				break;
			case KILL_UPDATE:
				vTaskDelete(NULL);
				break;
		}
	}
#endif
}

#endif


