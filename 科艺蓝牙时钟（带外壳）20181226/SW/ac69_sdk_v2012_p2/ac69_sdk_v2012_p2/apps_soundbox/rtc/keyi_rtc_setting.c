/*-------------------------------------------------------------------------
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief
*@FileName		keyi_rtc_setting.c
*@Data			2017/11/9 15:39:52
-------------------------------------------------------------------------*/
#include <sdk_cfg.h>
#include <sys_detect.h>
#include <ui_api.h>
#include <keyi_rtc_app.h>
#include <keyi_rtc_setting.h>
#include <rtc/rtc_api.h>

#if (PRODUCT_VERSION_KEYI == 1)
#if RTC_CLK_EN

time_setting_t timeSetting;
#define isleap(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)
static const int time_mon_lengths[2][12] = {
  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

#define get_curr_month_day(year,month)	(time_mon_lengths[isleap(year)][month - 1])

void alarm_switch(bool sw)
{
	timeSetting.alarmConfig.alarmSwitch	= sw;
	rtc_module_alarm_switch(sw);
}
static void time_plus_process()
{
	switch (timeSetting.timeSettingPosition) {
		case TIME_YEAR_SETTING:
			if (timeSetting.currTime->year++ > 100) {
				timeSetting.currTime->year = 11;
			}
			break;
		case TIME_MONTH_SETTING:
			if (++timeSetting.currTime->month > 12) {
				timeSetting.currTime->month = 1;
			}
 			break;
		case TIME_DAY_SETTING:
			if (++timeSetting.currTime->day > get_curr_month_day(timeSetting.currTime->year,timeSetting.currTime->month)) {
				timeSetting.currTime->day = 1;
			}
			break;
		case TIME_HOUR_SETTING:
			if (++timeSetting.currTime->hour > 23) {
				timeSetting.currTime->hour = 0;
			}
			break;
		case TIME_MIN_SETTING:
			if (++timeSetting.currTime->min > 59) {
				timeSetting.currTime->min = 0;
			}
			break;
		/* Display AL */
		case TIME_ALARM_ICON_SETTING:

			break;
		case ALARM_HOUR_SETTING:
			if (++timeSetting.alarmConfig.currAlarm->hour > 23) {
				timeSetting.alarmConfig.currAlarm->hour = 0;
			}
			break;
		case ALARM_MIN_SETTING:
			if (++timeSetting.alarmConfig.currAlarm->min > 59) {
				timeSetting.alarmConfig.currAlarm->min = 0;
			}
			break;
	}
}
static void time_minus_process()
{
	switch (timeSetting.timeSettingPosition) {
		case TIME_YEAR_SETTING:
			if (timeSetting.currTime->year-- < 11) {
				timeSetting.currTime->year = 11;
			}
			break;
		case TIME_MONTH_SETTING:
			if (--timeSetting.currTime->month < 1) {
				timeSetting.currTime->month = 12;
			}
			break;
		case TIME_DAY_SETTING:
			if (--timeSetting.currTime->day < 1){
				timeSetting.currTime->day = get_curr_month_day(timeSetting.currTime->year,timeSetting.currTime->month);
			}
			break;
		case TIME_HOUR_SETTING:
			if (--timeSetting.currTime->hour > 24) {
				timeSetting.currTime->hour = 23;
			}
			break;
		case TIME_MIN_SETTING:
			if (--timeSetting.currTime->min > 60) {
				timeSetting.currTime->min = 59;
			}
			break;
		/* Display AL */
		case TIME_ALARM_ICON_SETTING:

			break;
		case ALARM_HOUR_SETTING:
			if (--timeSetting.alarmConfig.currAlarm->hour > 24) {
				timeSetting.alarmConfig.currAlarm->hour = 23;
			}
			break;
		case ALARM_MIN_SETTING:
			if (--timeSetting.alarmConfig.currAlarm->min > 60) {
				timeSetting.alarmConfig.currAlarm->min = 59;
			}
			break;
	}

}
static void inline set_alarm_param()
{
#include <rtc/rtc_api.h>
	RTC_TIME temp;
	temp.bHour = timeSetting.alarmConfig.currAlarm->hour;
	temp.bMin  = timeSetting.alarmConfig.currAlarm->min;
	rtc_module_write_alarm_datetime(&temp,0);
}

/* Remap to alarm setting */
extern const u8 rtc_setting_ad_table[4][10];
extern void key_msg_register(char *task_name,const u8 ad_key_t[4][10],const u8 io_key_t[4][10], const u8 ir_key_t[4][21], const u8 touch_key_t[4][10]);
void time_setting(void)
{
    int msg[3];
	timeSetting.settingCount = 0;
	timeSetting.timeSettingPosition = TIME_YEAR_SETTING;
	timeSetting.rtcMode = TIME_SET_MODE;
	/* Set key Map to time setting mode */
	key_msg_register(RTC_TASK_NAME, rtc_setting_ad_table, rtc_io_table, rtc_ir_table, NULL);
    while(1)
    {
        memset(msg,0x00,sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        clear_wdt();
        if(MSG_HALF_SECOND != msg[0])
        {
        	/* Reset count t0 zero avoid quit */
            timeSetting.settingCount = 0;
        }

        switch(msg[0]) {
	        case MSG_TIME_SETTING:
	            timeSetting.timeSettingPosition++;
	            if (timeSetting.timeSettingPosition >= TIME_SEETING_MAX) {
					/*TODO:
					*process time setting ag,flash rtc time to rtc ,flash
					*alarm msg to rom
					*/
					alarm_switch(true);
	                timeSetting.timeSettingPosition = TIME_YEAR_SETTING;
					set_alarm_param();
					update_rtc_time(timeSetting.currTime);
					reset_alarm_check_time_default();
					goto _rtc_setting_exit;
				}
				printf("MSG_TIME_SETTING %d\n",timeSetting.timeSettingPosition);
	            UI_menu(MENU_RTC_MAIN);
	            break;
        case MSG_RTC_PLUS:
			time_plus_process();
            break;

        case MSG_RTC_MINUS:
			time_minus_process();
            break;

        case MSG_HALF_SECOND:
			SET_UI_MAIN(MENU_RTC_MAIN);
            UI_menu(MENU_RTC_MAIN);
            timeSetting.settingCount++;
            //时间设置超时退出
            if(timeSetting.settingCount >= SETTING_TIME_MAX_COUNT)
            {
	            rtc_printf("time_set timeout\n");
                timeSetting.settingCount = 0;
               	
                goto _rtc_setting_exit;
            }
            break;

        default:
            rtc_printf("default : %d\n",msg[0]);
//            if(msg[0] != MSG_RTC_POWER_DOWN);
//                os_taskq_post(RTC_TASK_NAME, 1, msg[0]);
//            goto _rtc_setting_exit;
            break;
        }
    }

_rtc_setting_exit:
	/* Reset key map to RTC mode */
	key_msg_register(RTC_TASK_NAME, rtc_ad_table, rtc_io_table, rtc_ir_table, NULL);
    puts("_rtc_setting_exit\n");
	/*  */
    timeSetting.rtcMode = RTC_DISPLAY;
	/*TODO:
	*reread RTC time to currTime
	*/
	SET_UI_MAIN(MENU_RTC_MAIN)
	UI_menu(MENU_RTC_MAIN);
    return;
}

#endif  /*RTC_CLK_EN*/
#endif



