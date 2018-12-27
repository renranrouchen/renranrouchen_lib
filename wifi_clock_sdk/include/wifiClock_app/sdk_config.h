/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      sdk_config.h
 * Author:        zwxf
 * Email:         zwxf.cw@gmail.com
 * DateTime:      2017-10-19 08:54:20
 * ModifiedTime:  2017-10-19 08:54:20
 */
#ifndef SDK_CONFIG_H
#define SDK_CONFIG_H
/*
* Priority setting
*/
#define NTP_TIME_PRIORITY	
#define ALARM_PRIORITY

/*
* product version config 
* NOTICE if you change version config please make sure make lib firstly then make wifi_clock
*/
/* demo version */
#define VERSION_DEMO	    0
/*special for mushan */
#define VERSION_MUSHAN      1
/* Costomer release version */
#define CUSTOMER_VERSION 	0


/*
* DATA update interval ,seconds unit
*/
#define	PM25_UPDATE_INTERVAL		 5
//#define WIFI_WEATHER_UPDATE_INTERVAL 60 * 60
#define WIFI_WEATHER_UPDATE_INTERVAL 5
/* uint ms */
#define NTP_TIME_UPDATE_INTERVAL (600 * 1000)

/* 
* ENTRY CONFIG MODE
*/
#define CONFIG_KEY          5  
// second unit
#define CINFIG_TIME_DELAY   50


/*
*Display interface 
*/
#define UI_ENABLE		1
#if (UI_ENABLE == 1)
#define FD612_ENABLE	1
#else
#define FD612_ENABLE	0	
#endif

/*
* For debug light quality,light all leds 
*/
#define ALL_LIGHT_DEBUG		0
/*
* Special config.default set to zero
*/
#define RTC_TIME            0
#define WEB_CONFIG	        0
#define POWER_SAVE_EN	    0


/*
*WEB API
*/

/*
* Product version config  
*/
#if (VERSION_MUSHAN == 1)
/*
* (1):Enable webconfig for wifi passwd & ssid
* (2):Enable Fd612 display 	
* (3):Disable power save config
* (4):Disable rtc time use ntp time
* (5):
*/
#define GET_RH_BY_PWM		        1

/* test case */
#define TEST_CASE                   0
#define WEATHER_CHART_TEST          0
#define LOCAL_TEMPERATURE_DISPLAY   0


/* Use ntp time to update */	
#define ENABLE_NTP_TIME             1

#define ENABLE_MUSHAN_TIME          0
/* 0.5ms unit */
#define RH_UPDATE_TIME 		        50
/* local temperature updata interval 10ms units */
#define LOCAL_TEMP_EVENT_SEND_DELAY	600

#undef WEB_CONFIG
#define WEB_CONFIG			        1

#undef RTC_TIME
#define RTC_TIME 			        0

#undef POWER_SAVE_EN
#define POWER_SAVE_EN		        0

#undef FD612_ENABLE
#define FD612_ENABLE		        1

#undef ALL_LIGHT_DEBUG
#define ALL_LIGHT_DEBUG		        0

#undef K780_API
#define K780_API			        0
#define MU_SHAN_API			        1

#endif /* (VERSION_MUSHAN == 1) */

/* First demo version  */
#if (VERSION_DEMO == 1)

#undef K780_API
#define K780_API			1

#undef WEB_CONFIG
#define WEB_CONFIG			0
#undef POWER_SAVE_EN
#define POWER_SAVE_EN		0
#undef RTC_TIME
#define RTC_TIME 			0
#undef FD612_ENABLE
#define FD612_ENABLE		1
#endif

/* First customer version */
#if (CUSTOMER_VERSION == 1)
/* add a FD612 to display new chart special for keyi version */
#define KEYI_SHOW_DISPLAY_DEMO  1

#undef K780_API
#define K780_API			1

#define ENABLE_NTP_TIME     1

#undef WEB_CONFIG
#define WEB_CONFIG			0

#undef POWER_SAVE_EN
#define POWER_SAVE_EN		0

#undef RTC_TIME
#define RTC_TIME 			1

#undef FD612_ENABLE
#define FD612_ENABLE		1

#undef ALL_LIGHT_DEBUG
#define ALL_LIGHT_DEBUG		0
#endif




#endif /* SDK_CONFIG_H */