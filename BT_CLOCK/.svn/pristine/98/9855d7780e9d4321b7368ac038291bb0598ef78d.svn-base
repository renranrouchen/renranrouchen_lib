/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf.cw@gmail.com
*@Brief
*@FileName		rtc_setting.c
*@Data			2017/10/25 11:23:04
-------------------------------------------------------------------------*/
/*
*********************************************************************************************************
*                                             BC51
*
*                                             CODE
*
*                          (c) Copyright 2015-2016, ZHUHAI JIELI
*                                           All Rights Reserved
*
* File : *
* By   : jamin.li
* DATE : 11/11/2015 build this file
*********************************************************************************************************
*/
#include "rtc.h"
#include "rtc_setting.h"
#include "sys_detect.h"
#if RTC_CLK_EN
#if (ALL_FUNCTION_VERSION == 1)

RTC_SETTING rtc_set;
#if ALARM_NUM_THREE
#define isleap(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)
static const int time_mon_lengths[2][12] = {
  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
} ;

#define get_curr_month_day(year,month)	(time_mon_lengths[isleap(year)][month])

static void time_plus_process(RTC_TIME* time,TIME_MODE timeMode,u8* timeIndex)
{
	switch (timeMode) {
		case ALARM_NUM_INDEX:
			if (timeIndex == NULL) {
				return;
			}
			*timeIndex += 1;
			if (*timeIndex > 2) {
				*timeIndex = 0;
			}
			printf("num=%d plus process!!!\n",*timeIndex);
			break;
		case RTC_YEAR_SETTING:
			if (++time->dYear > 2100) {
				time->dYear = 2017;
			}
			printf("year=%d plus process!!!\n",time->dYear);
			break;
		case RTC_MONTH_SETTING:
			if (++time->bMonth > 12) {
				time->bMonth = 1;
			}
			printf("month=%d plus process!!!\n",time->bMonth);
			break;
		case RTC_DAT_SETTING:
			if (++time->bDay > get_curr_month_day(time->dYear,time->bMonth)) {
				time->bDay = 1;
			}
			printf("dat=%d plus process!!!\n",time->bDay);
			break;
		case RTC_HOUR_SETTING:
			if (++time->bHour > 23) {
				time->bHour = 0;
			}
			printf("hour=%d plus process!!!\n",time->bHour);
			break;
		case RTC_MIN_SETTING:
			if (++time->bMin > 59) {
				time->bMin = 0;
			}
			printf("min=%d plus process!!!\n",time->bMin);
			break;
		}
}
static void time_minus_process(RTC_TIME* time,TIME_MODE timeMode,u8* timeIndex)
{
	switch (timeMode) {
		case ALARM_NUM_INDEX:
			if (timeIndex == NULL) {
				return;
			}
			*timeIndex -= 1;
			if (*timeIndex > 2) {
				*timeIndex = 2;
			}
			printf("alarm num=%d minus process !!!\n",*timeIndex);
			break;
		case RTC_YEAR_SETTING:
			if (--time->dYear < 2010) {
				time->dYear = 2017;
			}
			printf("alarm year=%d minus process!!!\n",time->dYear);
			break;
		case RTC_MONTH_SETTING:
			if (--time->bMonth < 1) {
				time->bMonth = 12;
			}
			printf("alarm month=%d minus process!!!\n",time->bMonth);
			break;
		case RTC_DAT_SETTING:
			if (--time->bDay < 1) {
				time->bDay = get_curr_month_day(time->dYear,time->bMonth);
			}
			printf("alarm dat=%d minus process!!!\n",time->bDay);
			break;
		case RTC_HOUR_SETTING:
			if (--time->bHour > 24) {
				time->bHour = 23;
			}
			printf("alarm hour=%d minus process!!!\n",time->bHour);
			break;
		case RTC_MIN_SETTING:
			if (--time->bMin > 60) {
				time->bMin = 59;
			}
			printf("alarm min=%d minus process!!!\n",time->bMin);
			break;
		}
}

#endif
/**************************************************************
*Description: RTC时间设置
*Arguments  : NULL
*Returns    : NULL
*Note       :
*By         : WGW
**************************************************************/
void rtc_setting(void)
{
    int msg[3];

    rtc_set.calendar_set.coordinate = RTC_COORDINATE_MIN;
	rtc_set.calendar_set.rtc_set_cnt = 0;
#if ALARM_ORIGNAL
    rtc_set.alarm_set.coordinate = COORDINATE_MIN;
    rtc_set.alarm_set.alm_set_cnt = 0;
#endif
#if ALARM_NUM_THREE
	unsigned char loop = 0;
	for (;loop < 3;loop++) {
		rtc_set.alarm_set[loop].coordinate = COORDINATE_MIN;
		rtc_set.alarm_set[loop].alm_set_cnt = 0;
	}
	rtc_set.alarmIndex = 0;
#endif

    if(rtc_set.rtc_set_mode == RTC_SET_MODE)
        UI_menu(MENU_RTC_MAIN);
    else
        UI_menu(MENU_ALM_SET);

    while(1)
    {
        memset(msg,0x00,sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        clear_wdt();
        if(MSG_HALF_SECOND!= msg[0])
        {
        	/* Reset count tp zero avoid qiut */	
            rtc_set.calendar_set.rtc_set_cnt = 0;
#if ALARM_ORIGNAL
			rtc_set.alarm_set.alm_set_cnt = 0;
#endif
#if ALARM_NUM_THREE
			rtc_set.alarm_set[rtc_set.alarmIndex].alm_set_cnt = 0;
#endif
        }

        switch(msg[0])
        {
//        case MSG_KEY_OK: //确定返回
//            rtc_printf("rtc set OK\n");
//            rtc_set.rtc_set_mode = RTC_DISPLAY;
////        ui_display(MSG_UI_RTC_TIME,&rtc_set,sizeof(RTC_SET),1);
//            goto _rtc_setting_exit;

        case MSG_RTC_SETTING://移动光标到下一项
            rtc_set.rtc_set_mode = RTC_SET_MODE;
            rtc_set.calendar_set.coordinate++;

            if(rtc_set.calendar_set.coordinate > COORDINATE_MAX)   //LED显示时只显示“时”、“分”，设置也只设置这两个
            {
                rtc_set.calendar_set.coordinate = RTC_COORDINATE_MIN;
            }
            UI_menu(MENU_RTC_MAIN);
            break;

        case MSG_ALM_SETTING://移动光标到下一项
        	printf("MSG ALARM SETTING \n");
            rtc_set.rtc_set_mode = ALM_SET_MODE;
#if ALARM_ORIGNAL
            rtc_set.alarm_set.coordinate++;
            if (rtc_set.alarm_set.coordinate > COORDINATE_MAX)   //LED显示时只显示“时”、“分”，设置也只设置这两个
            {
            	//TODO: start to burn message to flash
                rtc_set.alarm_set.coordinate = COORDINATE_MIN;
            }
#endif
#if ALARM_NUM_THREE
			rtc_set.alarm_set[rtc_set.alarmIndex].coordinate++;
			if (rtc_set.alarm_set[rtc_set.alarmIndex].coordinate > COORDINATE_MAX)	 //LED显示时只显示“时”、“分”，设置也只设置这两个
			{
				rtc_set.alarm_set[rtc_set.alarmIndex].coordinate = COORDINATE_MIN;
			}
			printf("alarm coordinate is %d",rtc_set.alarm_set[rtc_set.alarmIndex].coordinate);
#endif
            UI_menu(MENU_ALM_SET);
            break;

        case MSG_RTC_PLUS:
			printf("MSG plus \n");
            if (rtc_set.rtc_set_mode == RTC_SET_MODE)  //万年历
            {
#if RTC_DEFAULT_VERSION
                calendar_time_plus(rtc_set.calendar_set.curr_rtc_time,rtc_set.calendar_set.coordinate);
                rtc_module_write_rtc_datetime(rtc_set.calendar_set.curr_rtc_time);
#else
				time_plus_process(rtc_set.calendar_set.curr_rtc_time,rtc_set.calendar_set.coordinate,NULL);
#endif

				//rtc_clk_out(rtc_set.calendar_set.curr_rtc_time);
                UI_menu(MENU_RTC_MAIN);
            }
            else if(rtc_set.rtc_set_mode == ALM_SET_MODE)
            {
#if ALARM_ORIGNAL
                calendar_time_plus(rtc_set.alarm_set.curr_alm_time,rtc_set.alarm_set.coordinate);
                rtc_module_write_alarm_datetime(rtc_set.alarm_set.curr_alm_time , rtc_set.alarm_set.current_alarm_mode);
#endif
#if ALARM_NUM_THREE
				time_plus_process(rtc_set.alarm_set[rtc_set.alarmIndex].curr_alm_time,rtc_set.alarm_set[rtc_set.alarmIndex].coordinate,&rtc_set.alarmIndex);
#endif
                //rtc_clk_out(rtc_set.alarm_set.curr_alm_time);
                UI_menu(MENU_ALM_SET);
            }
            break;

        case MSG_RTC_MINUS:
			printf("MSG minus \n");
            if(rtc_set.rtc_set_mode == RTC_SET_MODE)  	//万年历
            {
#if RTC_DEFAULT_VERSION
                calendar_time_minus(rtc_set.calendar_set.curr_rtc_time,rtc_set.calendar_set.coordinate);
                rtc_module_write_rtc_datetime(rtc_set.calendar_set.curr_rtc_time);
#else
				time_plus_process(rtc_set.calendar_set.curr_rtc_time,rtc_set.calendar_set.coordinate,NULL);
#endif
                //rtc_clk_out(rtc_set.calendar_set.curr_rtc_time);
                UI_menu(MENU_RTC_MAIN);
            }
            else if(rtc_set.rtc_set_mode == ALM_SET_MODE)
            {
#if ALARM_ORIGNAL
                calendar_time_minus(rtc_set.alarm_set.curr_alm_time, rtc_set.alarm_set.coordinate);
                rtc_module_write_alarm_datetime(rtc_set.alarm_set.curr_alm_time , rtc_set.alarm_set.current_alarm_mode);
#endif
#if ALARM_NUM_THREE
				time_minus_process(rtc_set.alarm_set[rtc_set.alarmIndex].curr_alm_time,rtc_set.alarm_set[rtc_set.alarmIndex].coordinate,&rtc_set.alarmIndex);
#endif
                //rtc_clk_out(rtc_set.alarm_set.curr_alm_time);
                UI_menu(MENU_ALM_SET);
            }
            break;

        case MSG_ALM_SW:
            //按键开关闹钟时候，自动改为设置闹钟模式
            rtc_set.rtc_set_mode = ALM_SET_MODE;
#if ALARM_ORIGNAL
            if(rtc_set.alarm_set.alarm_sw)
            {
                rtc_set.alarm_set.alarm_sw = 0;
            }
            else
            {
                rtc_set.alarm_set.alarm_sw = 1;
            }
            rtc_module_alarm_switch(rtc_set.alarm_set.alarm_sw);
#endif
			UI_menu(MENU_ALM_SET);
            break;

        case MSG_HALF_SECOND:
            //时间设置计时
            if(rtc_set.rtc_set_mode == RTC_SET_MODE)
            {
	            UI_menu(MENU_RTC_MAIN);
                rtc_set.calendar_set.rtc_set_cnt++;
                //时间设置超时退出
                if(rtc_set.calendar_set.rtc_set_cnt == RTC_SETTING_CNT)
                {
                    rtc_set.calendar_set.rtc_set_cnt = 0;
                    rtc_printf("calendar_set timeout\n");
                    goto _rtc_setting_exit;
                }
            }

            if(rtc_set.rtc_set_mode == ALM_SET_MODE)
            {
#if ALARM_ORIGNAL
                rtc_set.alarm_set.alm_set_cnt++;
                if(rtc_set.alarm_set.alm_set_cnt == ALM_SETTING_CNT)
                {
                    rtc_set.alarm_set.alm_set_cnt = 0;
                    rtc_printf("Alarm Set TimeOut\n");
                    goto _rtc_setting_exit;
				}
#endif
#if ALARM_NUM_THREE
                rtc_set.alarm_set[rtc_set.alarmIndex].alm_set_cnt++;
                if(rtc_set.alarm_set[rtc_set.alarmIndex].alm_set_cnt == ALM_SETTING_CNT)
                {
                    rtc_set.alarm_set[rtc_set.alarmIndex].alm_set_cnt = 0;
                    rtc_printf("Alarm Set TimeOut\n");
                    goto _rtc_setting_exit;
                }
#endif
            }
//            UI_menu(MENU_MAIN);
//            UI_menu(MENU_HALF_SEC_REFRESH);
            break;

        default:
            rtc_printf("default : %d\n",msg[0]);
            if(msg[0] != MSG_RTC_POWER_DOWN)//MSG_RTC_POWER_DOWN消息返回不转发
                os_taskq_post(RTC_TASK_NAME, 1, msg[0]);//其他非设置的按键将转发回主线程
            goto _rtc_setting_exit;
            break;
        }
    }

_rtc_setting_exit:
    puts("_rtc_setting_exit\n");
#if ALARM_ORIGNAL
    rtc_set.alarm_set.curr_alm_time->bSec = 0;
    rtc_module_write_alarm_datetime(rtc_set.alarm_set.curr_alm_time , rtc_set.alarm_set.current_alarm_mode);
#endif
#if ALARM_NUM_THREE
	rtc_set.alarm_set[rtc_set.alarmIndex].curr_alm_time->bSec = 0;
#endif
    rtc_set.rtc_set_mode = RTC_DISPLAY;
    rtc_module_read_rtc_datetime(rtc_set.calendar_set.curr_rtc_time);

	UI_menu(MENU_RTC_MAIN);
    return;
}

#endif  /*RTC_CLK_EN*/
#endif /* (PRODUCT_VERSION_KEYI == 0)*/
