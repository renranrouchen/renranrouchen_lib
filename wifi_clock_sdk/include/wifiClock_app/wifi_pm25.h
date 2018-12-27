/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      wifi_pm25.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-12 08:42:43
 * ModifiedTime:  2017-06-12 08:42:43
 */
#ifndef WIFI_PM25_H
#define WIFI_PM25_H
 
#include "http/webclient.h"
#include "wifi_weather.h"

typedef struct {
	TupDateControl update;
	struct httpClient wifiPM25;
	char firstEntryCheck;
	char cityName[30];
	char aqi[5];
	char aqiScope[10];
	char aqiLevelId[4];
	char aqiLevelName[30];
	int aqiValue; 
}TwifiPm25;
extern TwifiPm25 wifiPm25;
extern void get_wifi_pm25();

#endif


