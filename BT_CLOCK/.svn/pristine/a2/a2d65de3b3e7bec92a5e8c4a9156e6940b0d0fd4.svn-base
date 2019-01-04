/*-------------------------------------------------------------------------
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief
*@FileName		keyi_rtc_setting.h
*@Data			2017/11/9 15:39:38
-------------------------------------------------------------------------*/
#ifndef KEYI_RTC_SETTING_H
#define KEYI_RTC_SETTING_H
#include <ds1302.h>
/* 10 seconds */
#define SETTING_TIME_MAX_COUNT	20
typedef rtc_time_t alarm_time_t;

typedef enum
{
    RTC_DISPLAY = 0,    //RTC显示
    RTC_SET_MODE,        //R设置显示
    ALM_SET_MODE,      //闹钟设置显示
	TIME_SET_MODE,		//add for keyi product by zwxf
}ENUM_RTC_UI;

typedef enum {
    TIME_DATE_DISPLAY = 0x0,
    TIME_YEAR_DISPLAY,
    TIME_TIME_DISPLAY,
    TIME_TEMP_DISPLAY,
    TIME_DISPLAY_MAX,
}DISPLAY_MODE;

typedef enum {
	TIME_YEAR_SETTING = 0x00,
	TIME_MONTH_SETTING,
	TIME_DAY_SETTING,
	TIME_HOUR_SETTING,
	TIME_MIN_SETTING,
	TIME_ALARM_ICON_SETTING,
	ALARM_HOUR_SETTING,
	ALARM_MIN_SETTING,
	TIME_SEETING_MAX,
}SETTING_MODE;
#define MAX_SNOOZE_TIME		2
/* half second decent one time total 60 seconds */
#define MAX_ALARM_SOUND_COUNT	120
typedef struct {
	alarm_time_t* currAlarm;
	u8 alarmSwitch;
	/* alarm is up or not*/
	u8 alarmUpFlag;
	/* Snooze state flag */
	u8 snoozeState;
	/* Alarm sound count time default 60 seconds */
	u8 alarmSoundCount;
	/* snooze time check defalut Max 2 times */
	u8 snoozeTimes;
	/* Use to enter alarm settng by long setting key pushed */
	u8 enterAlarmSettingFlag;
}alarm_config_t;
typedef struct _time_setting {
	struct rtc_time* currTime;
	alarm_config_t alarmConfig;
	/* type SETTING_MODE */
	u8 timeSettingPosition;
	u16 settingCount;
	u8 rtcMode;
	/* Display Mode*/
	u8 switchDdisplayFlag;
	u8 timeDisplayPosition;
	u16 displayCount;
}time_setting_t;
extern time_setting_t timeSetting;
extern void time_setting(void);
#define get_alarm_switch_state()	rtc_module_get_alarm_flag()
extern void alarm_switch(bool sw);
extern bool get_alarm_flag(void);
#endif
