/*-------------------------------------------------------------------------
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief
*@FileName		keyi_rtc_app.h
*@Data			2017/11/9 17:06:30
-------------------------------------------------------------------------*/
#ifndef	KEYI_RTC_APP_H
#define KEYI_RTC_APP_H
#include "sdk_cfg.h"
#include "includes.h"
#include "common/app_cfg.h"
#include "rtos/os_api.h"
#include "rtos/os_cfg.h"
#include "common/error.h"
#include "common/msg.h"
#include "rtos/task_manage.h"
#include "rtc_key.h"
#include "dac/dac_api.h"
#include "play_sel/play_sel.h"
#include "key_drv/key.h"
#include "key_drv/key_voice.h"
#include "ui/ui_api.h"
#include "rtc_ui.h"
#include <keyi_rtc_setting.h>
#if (PRODUCT_VERSION_KEYI == 1)
#define _RTC_DEBUG_



#ifdef _RTC_DEBUG_
#define rtc_puts         puts
#define rtc_printf       printf
#else
#define rtc_puts(...)
#define rtc_printf(...)
#endif

#define RTC_TASK_NAME   "RTCTask"
typedef struct {
#define MAX_VOLUMN_DISPLAY_TIME     8    
    char volumnDisplayFlag;
    char volumnDisplayDelay;
}volumn_display_config_t;
extern volumn_display_config_t volumnDisplayConfig;
extern rtc_time_t rtcTime;
extern alarm_time_t alarmTime;
extern const struct task_info keyi_rtc_task_info;
extern rtc_time_t rtcTime;
extern alarm_time_t alarmTime;

/* Write alarm hour & min & alarm switch to flash */
//void rtc_module_write_alarm_datetime(  RTC_TIME *curr_time , u8 alarm_mode);
/* Read alarm hour & min */
//void rtc_module_read_alarm_datetime(  RTC_TIME *curr_time );

extern void rtc_hw_init(void);
extern void call_display_rtc_time(void);
extern void reset_alarm_check_time_default();
#endif
#endif
