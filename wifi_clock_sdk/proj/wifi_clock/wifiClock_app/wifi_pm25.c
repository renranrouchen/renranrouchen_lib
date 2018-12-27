/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      wifi_pm25.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-12 08:41:44
 * ModifiedTime:  2017-06-12 08:41:44
 */
#include <common.h>
#include <string.h>
	 
#include "lwip/sockets.h"

	 /* obtain sdk_param structure */
#include <cfg_api.h>
#if CONFIG_FREERTOS
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#endif
#ifdef CONFIG_JSON
#include <cJSON.h>
#endif
#include "log/log.h"
//#include <wifi_local_tick.h>
#include "wifi_pm25.h"
#include "wifi_weather.h"
#include <rtc_time.h>
#include <wifi_clock_cfg.h>
//#include <cfg_api.h>
#include <ui_display.h>

TwifiPm25 wifiPm25;
extern sdk_param sdkParam;

/**Using k780 api
 * 
 * 
 * 
 * 
 */ 
#if (K780_API == 1)
#define PM25_URI	"/?app=weather.pm25&weaid=%s&appkey=27629&sign=02c63dea6b6cf510665ce4905b73cbcb&format=json"
#define PM25_HOST	"api.k780.com"
#define PM25_HOST_ADDR	"61.142.111.188"
#define PM25_PORT	80	

void dump_pm25()
{
	printf("cityname %s,aqi %s,aqi_levid %s,aqi_levnm %s,aqi_scope %s",
			wifiPm25.cityName,
			wifiPm25.aqi,
			wifiPm25.aqiLevelId,
			wifiPm25.aqiLevelName,
			wifiPm25.aqiScope
			);
}

void fillin_pm25_callback(cJSON* child)
{
	strcpy(wifiPm25.cityName,get_json_string(child,"citynm"));
	strcpy(wifiPm25.aqiLevelId,get_json_string(child,"aqi_levid"));
	strcpy(wifiPm25.aqiLevelName,get_json_string(child,"aqi_levnm"));
	strcpy(wifiPm25.aqi,get_json_string(child,"aqi"));
	strcpy(wifiPm25.aqiScope,get_json_string(child,"aqi_scope"));
#if (CUSTOMER_VERSION == 0)
	ui_setup(wifiPm25.aqi,PM2_5_DATA);
	ui_setup(&noData,PM2_5_CHAR_DATA);
#endif	
	xSemaphoreGive(weather.JsonParseMutex);
	dump_pm25();
}
#endif /* (K780_API == 1) */

/**MUSHAN api 
 * 
 * 
 * 
 */
#if (MU_SHAN_API == 1)
#define PM25_URI	"/msapi/pm25/%s"
#define PM25_HOST	"pgyep.com"
#define PM25_HOST_ADDR	"120.55.41.172"
#define PM25_PORT	80	

void dump_pm25(int data)
{
	printf("-------------------------->mu shan api pm25 %d\n",data);
}

void fillin_pm25_callback(cJSON* child)
{
#if (CUSTOMER_VERSION == 0)
	// ui_setup(wifiPm25.aqi,PM2_5_DATA);
	// ui_setup(&noData,PM2_5_CHAR_DATA);
#endif
#if (VERSION_MUSHAN == 1)
	int pm25 = -1;	
	pm25 = get_json_value(child,"pm2.5");

	if (pm25 != -1) {
		ui_setup(&pm25,PM2_5_DATA);
		wifiPm25.aqiValue = pm25;
	}
	dump_pm25(pm25);
#endif	
	xSemaphoreGive(weather.JsonParseMutex);
}
#endif

/* callback function */
static int PM25_data_handle(char* data,int len)
{
	int ret = -1;
	char* tmp = NULL;
	int httpHeadLen = head_check(data);
	if (httpHeadLen < 0) {
		return -1;
	}
	tmp = data = data + httpHeadLen;
	len = len - httpHeadLen;
#if (K780_API == 1)	
	tmp = strstr(data,"{");
	len -= (unsigned int) (tmp - data);
	data = tmp;
#endif
	while(len--) {
		printf("%c",*tmp++);
	}
	/* TODO:
	*add json parses because websites have different json structure
	*/
	return excute_json_parse(data,fillin_pm25_callback);
}

static void init_pm25_param()
{
	char ret = 0;
	char deviceId[20] = {0};
	if (weather.JsonParseMutex == NULL) {
		vSemaphoreCreateBinary(weather.JsonParseMutex);
	}
	/* update interval setting */	
//	wifiPm25.update.updateInterval = sdkParam.wth_parma.updateInterval;
	wifiPm25.update.updateInterval = PM25_UPDATE_INTERVAL;
#if (VERSION_MUSHAN == 1)	
	wifiPm25.wifiPM25.uri = malloc(strlen(PM25_URI) + 20);
	sprintf(wifiPm25.wifiPM25.uri,PM25_URI,sdkParam.deviceId);
#endif
#if (VERSION_DEMO == 1) 
	wifiPm25.wifiPM25.uri = malloc(strlen(PM25_URI) + 10);
	itoaw(sdkParam.wth_parma.cityId,cityId,10);
	sprintf(wifiPm25.wifiPM25.uri,cityId);
#endif
	wifiPm25.wifiPM25.host = PM25_HOST;
	wifiPm25.wifiPM25.iaddr = PM25_HOST_ADDR;
	wifiPm25.wifiPM25.port = PM25_PORT;
	wifiPm25.aqiValue = -1;
	wifiPm25.update.firstEntryCheck = FIRST_ENTRY;
}

void get_wifi_pm25()
{
	UPDATE_TYPE updateType = KILL_UPDATE;
	init_pm25_param();
	while(1) {
		{
#if RTC_TIME				
			updateType = is_update_time((TupDateControl* )&wifiPm25.update,&rtcTime.currentSeconds);
#else
			updateType = is_update_time((TupDateControl* )&wifiPm25.update,&localTime.currentSeconds);
#endif
			switch (updateType) {
			case START_UPDATE:
				if (wifiPm25.update.firstEntryCheck != FIRST_ENTRY) {
					//EXIT_POWER_SAVE();
				}
				start_update(&wifiPm25.update,&wifiPm25.wifiPM25,PM25_data_handle);
				break;	
			case WAIT_UPDATE:
				sys_msleep(520);
				break;
			case KILL_UPDATE:
				/* kill this task */
				vTaskDelete(NULL);
				break;
			default:
				break;
			}
		}
		sys_msleep(2000);
	}
}


