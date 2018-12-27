/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      cfg_time_count.c
 * Author:        zwxf
 * Email:         zwxf.cw@gmail.com
 * DateTime:      2017-07-20 09:36:58
 * ModifiedTime:  2017-07-20 09:36:58
 */

#include <common.h>

#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#include <wifi_clock_cfg.h>
#include <FD612demo_app.h>
#include <sdk_config.h>
#include "omniconfig.h"

void FD612_disp_config_count(int configTime)
{
#if (CUSTOMER_VERSION)
	unsigned char buff[12] = {0,};
	buff[4] = DISP_TAB[configTime / 100];
	buff[5] = DISP_TAB[configTime % 100 / 10];
	buff[6] = DISP_TAB[configTime % 100 % 10];
	display_data_toFD612(TOP,buff);
#endif
#if VERSION_MUSHAN
#include <ui_display.h>
	ui_setup((void* )&configTime,HOUR_DATA);
	ui_display();
#endif

}

void cfg_count_zero(void* param)
{
	int countToZero = 60;
	FD612_app_init();	
	/* Define in smartconfig for exit counting state */
	do {
#if (WEB_CONFIG == 0)		
		if (state == OMNI_STATE_CONNECTING)
			break;
#endif			
		FD612_disp_config_count(countToZero--);
		sys_msleep(1000);
	} while (countToZero + 1);
	vTaskDelete(NULL);
}

