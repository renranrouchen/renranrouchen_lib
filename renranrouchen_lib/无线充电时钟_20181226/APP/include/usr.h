#ifndef _USR_H_
#define	_USR_H_


#include "ca51f3sfr.h"
#include "ca51f3xsfr.h"
#include "gpiodef_f3.h"
#include "FD612Drv_double.H"
#include "FD612Drv_demo.h"
#include "ds1302.H"
#include "usr.h"

sbit SET = P3^4;
sbit UP  = P0^6;
sbit DOWN= P0^5;
sbit RESET1= P0^7;

sbit buzzer = P1^6;
sbit voice_key =P0^3;
sbit vibration_key =P0^4;
sbit power_key=P0^1;

//RTCIF定义
#define RTC_MF		(1<<2)
#define RTC_HF		(1<<1)
#define RTC_AF		(1<<0)

#define sec_w_addr 0x80
#define min_w_addr 0x82
#define hour_w_addr 0x84
#define day_w_addr 0x86
#define mon_w_addr 0x88
#define week_w_addr 0x8A
#define year_w_addr 0x8C
#define rom1_w_addr 0xC0
#define rom2_w_addr 0xC2

#define sec_r_addr 0x81
#define min_r_addr 0x83
#define hour_r_addr 0x85
#define day_r_addr 0x87
#define mon_r_addr 0x89
#define week_r_addr 0x8B
#define year_r_addr 0x8D
#define rom1_r_addr 0xC1
#define rom2_r_addr 0xC3

#define year_init  18
#define	month_init  1
#define	mday_init  1
#define	week_init  1
#define	hour_init  12
#define	minute_init  00
#define	second_init  00


#define over_time 10

#define alarm_time 1
#define sleep_time 5

#define up_down_delay_time 250
#define temp_refresh_time 10

#define voice_wake_up_time 5

struct DATA_TIME  //年月日周时分秒的定义
{
UINT8 	 year ;
UINT8 	 month ;
UINT8 	 mday ;
UINT8 	 week ;
UINT8 	 hour ;
UINT8	   minute ;
UINT8 	 second ;	
};

struct DATA_ALARM  //年月日周时分秒的定义
{
UINT8 	 hour ;
UINT8  	 minute ;
UINT8		alarm_pm_flag;
UINT8 	alarm_on_dispaly;
UINT8   alarm_flag;
};
UINT8 bcd_decimal(UINT8 bcd);
void display_all(void);
void display(void);
void key_statue(void);
void key_into_init(void);
void alarm_time_init(void);
UINT8  leep_year(UINT16  year);

void alarm_key_short_up_down(UINT8 i);
void  display_12_24_blink(UINT8 i);
void set_time_year_blink(UINT8 i);
void alarm_bilnk_display(UINT8 i,UINT8 j,UINT8 k,struct DATA_ALARM alarm);
void set_time_hour_blink(UINT8 i);
void after_arrive_write_1302(void);
void set_time_minute_blink(UINT8 i);
void set_time_day_blink(UINT8 i);
void set_time_mon_blink(UINT8 i);

void key_display_up_down(void);
void hour_long_up_down(UINT8 i);
void min_long_up_down(UINT8 i) ;
void year_long_up_down(UINT8 i);
void mon_long_up_down(UINT8 i);
void mday_long_up_down(UINT8 i);
void alarm_hour_long_up_down(UINT8 alarm_seclect,UINT8 up_down_seclect,struct DATA_ALARM alarm);
void alarm_min_long_up_down(UINT8 alarm_seclect,UINT8 up_down_seclect,struct DATA_ALARM alarm);

void  ADC_Convert(void);

void Alarm_display(void);
void alarm1_sleep(void);
void alarm2_sleep(void);
void alarm3_sleep(void);

void de_week(void) ;
void Week_display(UINT8 value);

void sd_mode (void);
void voice_control(void);

void InitDS1302(void);

void bright_contrl(void);

void display_init(void);
void power_detect(void);

void hour_chage_input(UINT8 i);

void hour_min_display(void);
	
void from_ds1302_read_time_data(void);

void mon_over_judge(void);
void mday_over_judge(void);

void alarm_12switch24(struct  DATA_ALARM* alarm);
void alarm_24switch12(struct  DATA_ALARM* alarm);
void hour_over_judge(UINT8 add_redue_flag);
void min_over_judge(UINT8 add_redue_flag);
void alarm_on_off_set(UINT8 alarm_num,struct  DATA_ALARM* alarm );
void alarm_hour_blink(UINT8 blink_enable,struct  DATA_ALARM* alarm);
void alarm_hour_set(struct  DATA_ALARM* alarm);
void alarm_hour_up_down(UINT8 long_key_enble,UINT8 up_down_seclect,struct DATA_ALARM * alarm);
void alarm_min_blink(UINT8 blink_enable,struct  DATA_ALARM* alarm);
void alarm_min_set(struct  DATA_ALARM* alarm);
void alarm_min_up_down(UINT8 long_key_enble,UINT8 up_down_seclect,struct DATA_ALARM* alarm);
void alarm_sleep_judge(struct DATA_ALARM* alarm);
void key_off_alarm_clock(struct  DATA_ALARM* alarm);
#endif