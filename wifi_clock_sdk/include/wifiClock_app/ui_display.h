/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      ui_display.h
 * Author:        zwxf
 * Email:         zwxf.cw@gmail.com
 * DateTime:      2017-10-18 09:47:58
 * ModifiedTime:  2017-10-18 09:47:58
 */

#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H
#include <wifi_local_tick.h>

typedef struct ui_setup_function {
	void (*pm2_5_data)(char* data);
	void (*pm2_5_char_data)(char* data);
	void (*year_data)(char* data);
	void (*month_data)(char* data);
	void (*day_data)(char* data);
	void (*week_data)(char* data);
	void (*hour_data)(char* data);
	void (*min_data)(char* data);
	void (*sunny_data)(char* data);
	void (*rainy_data)(char* data);
	void (*min_rain_data)(char* data);
	void (*heavy_rain_data)(char* data);
	void (*car_limit_data)(char* data);
	void (*humidity_data)(char* data);
	void (*online_temprature_data)(char* data);
	void (*local_temperature_data)(char* data);
	void (*local_humidity_data)(char* data);
	void (*wifi_flag_data)(char* data);
	void (*alarm_flag_data)(char* data);
	void (*display_brightness_data)(char* data);
	void (*half_second_flash)(char* data);
	void (*weather_char_data)(char* data);
	void (*ui_const_chart_setup)(char* data);
	void (*display_off)(char* data);
} ui_setup_function_t;


typedef enum {
	PM2_5_DATA = 0,
	PM2_5_CHAR_DATA,
	YEAR_DATA,
	MONTH_DATA,
	DAY_DATA,
	WEEK_DATA,
	HOUR_DATA,
	MIN_DATA,
	SUNNY_DATA,	
	RAINY_DATA,
	MIN_RAIN_DATA,
	HEAVY_RAIN_DATA,
	CAR_LIMIT_DATA,
	HUMUDITY_DATA,
	TEMPERATURE_DATA,
	LOCAL_TEMPERATURE_DATA,
	LOCAL_HUMIDITY_DATA,	
	WIFI_FLAG_DATA,
	ALARM_FLAG_DATA,
	DISPLAY_BRIGHTNESS,
	HALF_SECOND_FLASH,
	WEATHER_CHAR_DATA,
	UI_CONST_CHART_SETUP,
	DISPLAY_OFF,
}DISPLAY_TYPE;
 
#define is_half_second()		(localTime.secondCheckTime == 2)	
extern unsigned int noData;
extern char* weatherList[];
extern void ui_setup(void* data,DISPLAY_TYPE type);
extern void ui_display();
#endif

