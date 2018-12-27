/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      wifi_weather.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-26 15:32:14
 * ModifiedTime:  2017-05-26 15:32:14
 */
#ifndef WIFI_WEATHER_H 
#define WIFI_WEATHER_H

#ifdef CONFIG_JSON
#include <cJSON.h>
#endif
#if CONFIG_FREERTOS
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#endif
#include "http/webclient.h"
#include <sdk_config.h>
enum weatherStatue {
	SUNNY    = 0X01,
	CLOUDY   = 0x02,
	OVERCAST = 0x03,
	SHOWER   = 0x04,
	THUNDER  = 0x05,
	THUNDERSTORM = 0x06,
	SHOW_AND_RAIN = 0x07,
	DRIZZLE       = 0X08,
	MODERATE_RAIN = 0x09,
	HEAVY_RAIN    = 0x0A,
	RAINSTORM     = 0x0B,
	DOWNPOUR	  = 0x0C,
	HEAVY_DOWNPOUR= 0x0D,
	SNOW          = 0x0E,
	LIGHT_SNOW    = 0x0F,
	NODERATE_SNOW = 0x10,
	GREAT_SNOW    = 0x11,
	BLIZZARD      = 0x12,
	MIST		  = 0x13,	
	ICE_RAIN      = 0x14,
	SANDSTORM	  = 0x15,
	SHOWER_MID_RAIN= 0x16,
	MID_HEAVY_RAIN = 0x17,
	HEAVY_STORM_RAIN= 0x18,
	STORM_DP_RAIN= 0x19,
	DP_HEAVYDP_RAIN = 0x1A,
	LIGHT_MID_SNOW  = 0x1B,
	NID_HEAVY_SNOW  = 0x1C,
	HEAVY_BLIZZARD_SNOW = 0x1D,
	SINK             = 0x1E,
	RAISE_DUST       = 0x1F,
	HEAVY_SANDSTORM = 0x20,
	HAZE            = 0x21,
};
enum {
	SUCCESS = 0,	
	FAIL = 1,
};
enum {
	FIRST_ENTRY = 0,
	NOT_FIRST_ENTRY = 1,	
};

typedef struct {
	/* control data */
	long updateTime;
	unsigned int updateInterval;
	char firstEntryCheck;
	unsigned int updateFail;
}TupDateControl;

typedef struct {
	TupDateControl update;
	struct httpClient webClient;
	SemaphoreHandle_t JsonParseMutex;
	char firstEntryCheck;
	/* fetch data */
#if (K780_API == 1)	
	char cityId[15];
	char cityName[10];

	char tempHigh[5];
	char tempLow[5];
	char tempCurr[10];

	char humidity[5];
	char wind[10];

	char weather[20];
	char weatherCurr[10];
	char weatid[5];
#endif
#if (MU_SHAN_API == 1)
	// char time[30];
	char weather[5];
	char forbidplate[3];
#endif
}Tweather;
extern Tweather weather;

typedef void (*fillin_data_cb)(cJSON* );
typedef int (*handle_data_CB)(char*,int);
Tweather get_weather();



extern void get_wifi_weather();
int excute_json_parse(char* str,fillin_data_cb fillin_cb);
char* get_json_string(cJSON* json,char* str);
int get_json_value(cJSON* json,char* str);

extern UPDATE_TYPE is_update_time(void* argc,unsigned long* totalSeconds);

extern int start_get_data(struct httpClient* client);
extern int start_read_data(struct httpClient* webClient,int (*data_handle_cb)(char*,int));
extern void start_update(TupDateControl* update,struct httpClient* webClient,handle_data_CB cb);
extern char *itoaw(int num, char *str, int radix);
#endif

