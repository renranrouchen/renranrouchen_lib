/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      FD612demo_app.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-22 09:30:19
 * ModifiedTime:  2017-06-22 09:30:19
 */
#ifndef FD612DEMO_APP_H
#define FD612DEMO_APP_H
#include <FD612Drv.h>
#define COUNT_TO_OFF(x)		(x * 2)
typedef struct FD612_disp{
	char secBit;
	char dispTimeflag;
	int dispTimeDelay;
	char displayOn;
	int countToDispOff;
	int countToDispModeOne;
}t_fd612DispTest; 	
enum{
	TOP,
	MIDDLE,
	BASE,	
};
enum{
	OFF = 0,
	ON = 1,
	MODE_ONE = 2,
	DONE,
};
#define TIMER_DELAY	500 
extern t_fd612DispTest FD612Test;
extern unsigned int displayIntense;
void FD612_printf_test(const void* const rtcTime);
void FD612_app_init();
inline void fd612_display_on();
inline void fd612_display_off();
void display_data_toFD612(char pos,unsigned char* buff);
extern void display_init_mode();
extern void display_weather_notify(char* buffOne,char* buffTwo);
extern void inline all_light_debug();
void FD612_ZERO_INIT();
#endif

