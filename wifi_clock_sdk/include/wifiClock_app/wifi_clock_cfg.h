/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      wifi_clock_cfg.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-23 10:20:54
 * ModifiedTime:  2017-06-23 10:20:54
 */
#ifndef WIFI_CLOCK_CFG_H
#define WIFI_CLOCK_CFG_H
#include <common.h>
#include <log/log.h>
#include <lwip/sockets.h>
#include <wifi_weather.h>
#include <wifi_time.h>
#include <FD612Drv.h>
//#include <wifi_local_tick.h>
#include <app_config.h>
#include <wifi_pm25.h>
#include <event_process.h>
#include <webserver.h>
#include <rtc_time.h>
#include <nowtime.h>
#include <http_server.h>
#include <wifi_local_tick.h>
#include <sdk_config.h>
#include <web_register.h>
#include "FreeRTOS.h"
#include "task.h"

#define DEBUG_TASK_OVERFLOW()	do{		\
	printf("task left stack size is %d",uxTaskGetStackHighWaterMark(xTaskGetCurrentTaskHandle()));	\
}while(0)
enum {
	CONNECT = 1,	
};
#define update_time(s)		(s * 1000)
#if (VERSION_DEMO || CUSTOMER_VERSION)
#define CONFIG_PIN 5
#endif
#if VERSION_MUSHAN
#define CONFIG_PIN 18	
#endif
extern void get_local_tmp();
extern void cfg_count_zero(void* param);
#if (GET_RH_BY_PWM == 1)
extern void create_PWM(void* param);
#endif



#endif
