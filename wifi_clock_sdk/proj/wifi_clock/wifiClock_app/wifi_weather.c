/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      wifi_weather.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-26 15:32:05
 * ModifiedTime:  2017-05-26 15:32:05
 */
#include "common.h"
#include <string.h>

#include "lwip/sockets.h"
/* obtain sdk_param structure */
#include <cfg_api.h>
#include "log/log.h"
#include "http/webclient.h"
//#include <wifi_local_tick.h>
#include <wifi_clock_cfg.h>
#include <event_process.h>
#include <rtc_time.h>

#include <ui_display.h>
#include <wifi_weather.h>

#undef printf
#define printf(fmt, args...) serial_printf(fmt, ##args)

Tweather weather;
extern sdk_param sdkParam;

static char *mystrcpy(char *strDest, const char *strSrc)
{
	if (strDest == NULL || strSrc == NULL)
		return NULL;
	if (strDest == strSrc)
		return strDest;
	char *tempDest = strDest;
	while ((*strDest++ = *strSrc++) != '\0')
		;
	return tempDest;
}

static close_weather_client()
{
	send_event(WLAN_DOWN_EVENT);
}

int start_get_data(struct httpClient *client)
{
	return (httpGet(client->clientfd, client->uri, client->host));
}

int get_json_value(cJSON *json, char *str)
{
	cJSON *jsonIndex = NULL;
	jsonIndex = cJSON_GetObjectItem(json, str);
	return jsonIndex->valueint;
}

char *get_json_string(cJSON *json, char *str)
{
	cJSON *jsonIndex = NULL;
	jsonIndex = cJSON_GetObjectItem(json, str);
	return jsonIndex->valuestring;
}
#if (MU_SHAN_API == 1)
#undef WIFI_WEATHER
#define WIFI_WEATHER "/msapi/ext/%s"
#undef WEATHER_HOST
#define WEATHER_HOST "pgyep.com"
#undef WEATHER_ADDR
#define WEATHER_ADDR "120.55.41.172"
#undef WEATHER_PORT
#define WEATHER_PORT 80

#define FORBITPLATE "forbidplate"
#define WEATHER "weather"
#define TIME "time"

#include "utcstring_time_to_ntp.h"
#if 0
static void set_system_time()
{	
	long totalTime = 0;		
	sntp_tm time;
	totalTime = get_uinx_timestamp(weather.time);
	if (totalTime != 0) {
		char* timeString = sntp_get_real_time(get_uinx_timestamp("utcTime"));
    	time = get_time_buff();
		int wifiSeconds = time.tm_hour * 60 * 60 + time.tm_min * 60 + time.tm_sec;
		printf(" wifisecond %d rtcsecond %d",wifiSeconds,rtcTime.currentSeconds);
	} else {
		return;
	}

#if RTC_TIME
	if (rtcTime.currentSeconds != wifiSeconds) {
		updata_rtc_time((void* )&time);
	}
	rtcTime.currentSeconds = wifiSeconds;
#else
	localTime.currentSeconds =  time.tm_hour * 60 * 60 + time.tm_min * 60 + time.tm_sec;
	localTime.totalSeconds = totalTime;
#endif
	dump_ntp_time(time);
}
#endif

void dump_weather()
{
	printf("\r\n================dump weather===================\r\n");
	printf("------> forbidplate %s weather %s\r\n",
		   // weather.time,
		   weather.forbidplate,
		   weather.weather);
}

void fillin_weather_data(cJSON *child)
{
	/*TODO:
	*add semaphore to lock weather data avoid multi operate
	*care for json live time,
	*/
	// strcpy(weather.time,get_json_string(child,TIME));
	strcpy(weather.forbidplate, get_json_string(child, FORBITPLATE));
	printf("------------------------------->limit %s \n", weather.forbidplate);
	strcpy(weather.weather, get_json_string(child, WEATHER));
	printf("------------------------------->weather.weather%s\r\n",weather.weather);
	/* unlock Json data */
	xSemaphoreGive(weather.JsonParseMutex);
	printf("------------------------------->limit %s \n", weather.forbidplate);
	if (weather.forbidplate[0] != 0)
	{
		printf("------------------------------->limit %s \n", weather.forbidplate);
		ui_setup(get_json_string(child, FORBITPLATE), CAR_LIMIT_DATA);
	}
	else
	{
		weather.forbidplate[0] = ' ';
		weather.forbidplate[1] = ' ';
		ui_setup(get_json_string(child, FORBITPLATE), CAR_LIMIT_DATA);
	}
#if (ENABLE_MUSHAN_TIME == 1)
	set_system_time();
#endif
	ui_setup(weather.weather, WEATHER_CHAR_DATA);
	dump_weather();
}

int excute_json_parse(char *str, fillin_data_cb fillin_data_cd)
{
	static cJSON *root = NULL, *child = NULL;
	cJSON *jsonIndex = NULL;
	char *tmpStr;
	/* delete former data */
	if (weather.JsonParseMutex != NULL)
	{
		if (xSemaphoreTake(weather.JsonParseMutex, 100) == pdTRUE)
		{
			root = cJSON_Parse(str);
			if (root == NULL)
			{
				goto erro;
			}
			fillin_data_cd(root);
			
			cJSON_Delete(root);
		}
		else
		{
			//	erro("\r\n=============json mutex get fail=============\r\n");
		}
	}
	else
	{
		return 0;
	}
	return 1;
erro:
	xSemaphoreGive(weather.JsonParseMutex);
	return -1;
}
#endif /* (MU_SHAN_API == 1) */

#if (K780_API == 1) /* if use K780 API to get data please enable this macro */
#define WIFI_WEATHER "/?app=weather.today&weaid=%s&appkey=27629&sign=02c63dea6b6cf510665ce4905b73cbcb&format=json"
#define WEATHER_HOST "api.k780.com"
#define WEATHER_PORT 80
#define WEATHER_ADDR "61.142.111.188"
void dump_weather()
{
	printf("\r\n================dump weather===================\r\n");
	printf("cityname %s,cityid %s,humidity %s,temp_curr %s,temp_low %s,temp_high %s,weather curr %s,weather %s,wind %s weatid %s\r\n",
		   weather.cityName,
		   weather.cityId,
		   weather.humidity,
		   weather.tempCurr,
		   weather.tempLow,
		   weather.tempHigh,
		   weather.weatherCurr,
		   weather.weather,
		   weather.wind,
		   weather.weatid);
}

void fillin_weather_data(cJSON *child)
{
	/*TODO:
	*add semaphore to lock weather data avoid multi operate
	*care for json live time,
	*/
	strcpy(weather.cityName, get_json_string(child, "citynm"));
	strcpy(weather.cityId, get_json_string(child, "cityid"));
	strcpy(weather.humidity, get_json_string(child, "humidity"));
	strcpy(weather.tempCurr, get_json_string(child, "temperature_curr"));
	strcpy(weather.tempHigh, get_json_string(child, "temp_high"));
	strcpy(weather.tempLow, get_json_string(child, "temp_low"));
	strcpy(weather.weather, get_json_string(child, "weather"));
	strcpy(weather.weatherCurr, get_json_string(child, "weather_curr"));
	strcpy(weather.wind, get_json_string(child, "wind"));
	strcpy(weather.weatid, get_json_string(child, "weatid"));
	/* unlock Json data */
	xSemaphoreGive(weather.JsonParseMutex);
	// ui_setup(weather.humidity,HUMUDITY_DATA);
	// ui_setup(weather.tempCurr,TEMPERATURE_DATA);
	dump_weather();
}

int excute_json_parse(char *str, fillin_data_cb fillin_data_cd)
{
	static cJSON *root = NULL, *child = NULL;
	cJSON *jsonIndex = NULL;
	char *tmpStr;
	/* delete former data*/
	if (weather.JsonParseMutex != NULL)
	{
		if (xSemaphoreTake(weather.JsonParseMutex, 100) == pdTRUE)
		{
			root = cJSON_Parse(str);
			if (root == NULL)
			{
				goto erro;
			}
			jsonIndex = cJSON_GetObjectItem(root, "success");
			tmpStr = jsonIndex->valuestring;
			if (strcmp(tmpStr, "1") == 0)
			{
				child = cJSON_GetObjectItem(root, "result");
				if (child == NULL)
				{
					goto erro;
				}
				fillin_data_cd(child);
				if (root != NULL)
				{
					cJSON_Delete(root);
					root = NULL;
				}
				close_weather_client();
			}
			else
			{
				goto erro;
			}
			//		erro("\r\n======left sp is %d===========\r\n",uxTaskGetStackHighWaterMark(xTaskGetCurrentTaskHandle()));
		}
		else
		{
			//	erro("\r\n=============json mutex get fail=============\r\n");
		}
	}
	else
	{
		return 0;
	}
	return 1;
erro:
	xSemaphoreGive(weather.JsonParseMutex);
	return -1;
}
#endif
/* callback function */
static int weather_data_handle(char *data, int len)
{
	int ret = -1;
	char *tmp = NULL;
	
	//char *tmp  = (char* )malloc(1024);
	int httpHeadLen = head_check(data);
	
	
	printf("------------------data------------\r\n");
	if (httpHeadLen < 0)
	{
		return -1;
	}
	tmp = data = data + httpHeadLen;
	len = len - httpHeadLen;
	/*
	printf("sizeof %d",len);	
	while(len--){
		printf("%c",*tmp++);
	}
	*/
	tmp = strstr(data, "{");
	len -= (unsigned int)(tmp - data);
	data = tmp;
	//printf("tmp_value : %s\r\n",tmp);
	//printf("------------------tmp_value------------\r\n");
	while (len--)
	{
		printf("%c", *tmp++);
		
	}
	printf("debug1\r\n");
	ret = excute_json_parse(data, fillin_weather_data);
	if (ret < 0)
	{
				printf("\r\n=====excute fail========\r\n");
		
		return -1;
	}
	
}
/* nonblock reading data from webserver */
int start_read_data(struct httpClient *webClient, int (*data_handle_cb)(char *, int))
{
	int ret = 0;
	ret = readData(webClient->clientfd, data_handle_cb);
	return ret;
}

UPDATE_TYPE is_update_time(void *argc, unsigned long *totalSeconds)
{
	TupDateControl *update = (TupDateControl *)argc;
	/* wifi time unvalue */
	if (
#if (VERSION_MUSHAN == 1)
		sdkParam.webRegisterStatue == false ||
#endif
#if (ENABLE_MUSHAN_TIME == 0)
		*totalSeconds == 0
#endif
	)
	{
		sys_msleep(1000);
		return WAIT_UPDATE;
	}

	if (update->updateTime == 0)
	{
#if (ENABLE_MUSHAN_TIME == 0)
		if (wifiTime.update.updateFail == SUCCESS)
#endif
		{
			update->updateTime = *totalSeconds + update->updateInterval;
			return START_UPDATE;
		}
		return WAIT_UPDATE;
	}
	/*FIXME: 
	*is it enought to bringup wlan within 5 secs 
	*/
	while (update->updateTime > *totalSeconds)
	{
		if (update->updateFail == FAIL)
		{
			sys_msleep(1000 * 30);
			printf("weather update fail \n");
			return START_UPDATE;
		}
		sys_msleep(500);
	}
	update->updateTime += update->updateInterval;
	if (update->updateTime > 86400)
	{
		update->updateTime -= 86400;
	}
	return START_UPDATE;
} 

static UPDATE_TYPE check_update_weather()
{
	if (sdkParam.wth_parma.isOpen)
	{
#if (RTC_TIME == 1)
		return is_update_time((Tweather *)&weather.update, &rtcTime.currentSeconds);
#else
		return is_update_time((Tweather *)&weather.update, &localTime.currentSeconds);
#endif
	}
	else
	{
		return KILL_UPDATE;
	}
}
int weather_update_success_count = 0;
int weather_update_fail_count = 0;
void start_update(TupDateControl *update, struct httpClient *webClient, handle_data_CB cb)
{
	int ret = -1;
	if (s_linkup != CONNECT)
	{
		
		
		goto erro;
	}
	ret = connectServer(webClient);
	if (ret < 0)
	{
		printf("================CONNECT erro\r\n");
		goto erro;
	}
	/* if connect fail when starting wifi */
	ret = start_get_data(webClient);
	if (ret < 0)
	{
		printf("================start_get_data erro\r\n");
		goto erro;
	}
	if (start_read_data(webClient, cb) < 0)
	{
		printf("================read erro\r\n");
		weather_update_fail_count++;
		printf("update_fail_count %d\r\n", weather_update_fail_count);
		goto erro;
	}
	if (update->firstEntryCheck != FIRST_ENTRY)
	{
		//ENTER_POWER_SAVE();
	}
	update->firstEntryCheck = NOT_FIRST_ENTRY;
	printf("update success %d", weather.update.updateFail);
	if(weather.update.updateFail == 0)
	{
		weather_update_success_count++;
		printf("update_success_count %d\r\n", weather_update_success_count);
	}
	close(webClient->clientfd);
	webClient->clientfd = -1;
	update->updateFail = SUCCESS;
	return;
erro:
	if (webClient->clientfd != -1)
	{
		close(webClient->clientfd);
		webClient->clientfd = -1;
	}
	update->updateFail = FAIL;
}
void init_weather_param()
{
	/* init update interval */
	char deviceId[20] = {0};
	//use config param
	//	weather.update.updateInterval = sdkParam.wth_parma.updateInterval;
	weather.update.updateInterval = WIFI_WEATHER_UPDATE_INTERVAL;
	if (weather.JsonParseMutex == NULL)
	{
		vSemaphoreCreateBinary(weather.JsonParseMutex);
		//erro("\r\n mutex");
	}

	weather.webClient.host = WEATHER_HOST;
	weather.update.updateFail = SUCCESS;
	weather.webClient.iaddr = WEATHER_ADDR;
	weather.webClient.port = WEATHER_PORT;
	weather.webClient.uri = malloc(strlen(WIFI_WEATHER) + 10);
	weather.update.firstEntryCheck = FIRST_ENTRY;
#if (VERSION_MUSHAN == 1)
	sprintf(weather.webClient.uri, WIFI_WEATHER, sdkParam.deviceId);
#endif
	weather.webClient.isopen = 1;
}
void get_wifi_weather()
{
	UPDATE_TYPE updateType = KILL_UPDATE;
	init_weather_param();
	while (1)
	{
		{
			updateType = check_update_weather();
			switch (updateType)
			{
			case START_UPDATE:
				if (weather.update.firstEntryCheck != FIRST_ENTRY)
				{
					EXIT_POWER_SAVE();
				}
				sys_msleep(500);
				start_update(&weather.update, &weather.webClient, weather_data_handle);
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
