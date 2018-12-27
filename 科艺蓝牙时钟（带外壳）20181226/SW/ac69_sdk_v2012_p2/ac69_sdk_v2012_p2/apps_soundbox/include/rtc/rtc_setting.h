#ifndef _RTC_SETTING_H_
#define _RTC_SETTING_H_

#include "includes.h"
#include "sdk_cfg.h"
#include "rtc/rtc_api.h"
#if (PRODUCT_VERSION_KEYI == 0)
#define RTC_SETTING_CNT   10///RTC ���ó�ʱʱ�䣬500unit
#define ALM_SETTING_CNT  10///ALARM ���ó�ʱʱ�䣬500unit

///1--once,2--every day,3-- one day for every week
#define ALARM_MODE_ONCE                      1
#define ALARM_MODE_EVERY_DAY              2
#define ALARM_MODE_EVERY_WEEK            3

//RTC��ʾ����
typedef enum
{
    RTC_DISPLAY = 0,    //RTC��ʾ
    RTC_SET_MODE,        //R������ʾ
    ALM_SET_MODE,      //����������ʾ
	TIME_SET_MODE,		//add for keyi product by zwxf    
}ENUM_RTC_UI;

typedef struct _ALM_SET_
{
#if 1
    RTC_TIME *curr_alm_time;   //��ǰ����ʱ��
#endif
    u8 coordinate;            //��������λ
    u8 alm_set_cnt;           //�������ó�ʱ����

    u8 alarm_flag;          //�������ֱ�־
    u8 alarm_sw;            //���ӿ���
    u8 bAlarmOnCnt ;          //�������ּ�ʱ
	/* use  7:4 to check */
    u8 current_alarm_mode;   ///1--once,2--every day,3-- one day for every week
//    u8 alm_ring_mode;         //��������ģʽ
//    u8 alm_music_type;        //����������ʾ������
}ALM_SET;

typedef struct _RTC_SET_
{
    RTC_TIME *curr_rtc_time;   //��ǰ������ʱ��
    u8 coordinate;            //RTC����λ
    u8 rtc_set_cnt;           //rtc���ó�ʱ����
}RTC_SET;

typedef struct _RTC_SET_MODE_
{
#if ALARM_ORIGNAL
    ALM_SET alarm_set;          //��������
#endif
#if ALARM_NUM_THREE
	ALM_SET alarm_set[3];
#endif
#if ALARM_NUM_THREE
	u8 alarmIndex;
#endif
	RTC_SET calendar_set;       //����������
    u8 rtc_set_mode;            //RTC����ģʽ
}RTC_SETTING;


/*
**********************************************************************
*                           VARIABLES DECLARE
**********************************************************************
*/
extern RTC_SETTING rtc_set;
/*
**********************************************************************
*                           FUNCTIONS DECLARE
**********************************************************************
*/
extern void rtc_setting(void);
#endif
#endif
