#ifndef _USR_H_
#define	_USR_H_

#include "FD814.H" 
#include "ds1302.H"
#include "Debug.H"
#include "FD612Drv.H"
#include <math.h>

sbit SET = P3^3;
sbit UP  = P3^7;
sbit DOWN= P1^7;
sbit buzzer = P3^4;
sbit voice_key =P3^2;

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

#define over_time 15

#define alarm_time 1
#define sleep_time 5

#define over_time_10s 10
#define over_time_2s  2

#define voice_wake_up_time 10

#define up_down_delay_time 250

#define temp_refresh_time 30
struct DATA_ALARM  //年月日周时分秒的定义
{
UINT8 	 hour ;
UINT8  	 minute ;
};

struct DATA_ALARM_ch  //
{
UINT8  	 ch_hour[2] ;
UINT8  	 ch_minute[2] ;
};

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

struct time_char   //time的字符型
{
UINT8 	 ch_year[2] ;
UINT8 	 ch_month[2] ;
UINT8 	 ch_mday[2] ;
UINT8 	 ch_week[2] ;
UINT8 	 ch_hour[2] ;
UINT8	   ch_minute[2] ;
UINT8 	 ch_second[2] ;	
};






UINT8 bcd_decimal(UINT8 bcd);
UINT8 decimal_bcd(UINT8 decimal);
void alarm_time_init(void);
void time0_setup(void);
void key_statue(void);
unsigned char bcd_decimal(unsigned char bcd);
unsigned char decimal_bcd(unsigned char decimal);
void  ADC_Convert(void)	;
void display(void);
void InitDS1302();
void alarm_time_init(void);
void alarm1_sleep(void);
void alarm2_sleep(void);
void alarm3_sleep(void);

void alarm_display (void);
void voice_control(void);
void sd_mode (void);
void Display_Mode_Select(void);
void mode1_select_dislay(void);
void year_long_up_down(UINT8 i);
void hour_long_up_down(UINT8 i);
void min_long_up_down(UINT8 i) ;
void mon_long_up_down(UINT8 i);
void mday_long_up_down(UINT8 i);
void alarm_set_display(UINT8 i,struct DATA_ALARM alarm);
void alarm_hour_long_up_down(UINT8 alarm_seclect,UINT8 up_down_seclect,struct DATA_ALARM alarm);
void alarm_min_long_up_down(UINT8 alarm_seclect,UINT8 up_down_seclect,struct DATA_ALARM alarm);
void alarm_hour_display(struct DATA_ALARM alarm);
void alarm_min_display(struct DATA_ALARM alarm);
void alarm_long_blink(struct DATA_ALARM alarm);
void dp_mode_exit(void);
void  pm_12_24_alarm_display(void);
void key_dislpay_up_down(void);
void FD612_display_4DIG_input(UINT8 dig1,UINT8 dig2,UINT8 dig3,UINT8 dig4);
void FD612_DISPLAY_2DIG_input(UINT8 i ,UINT8 dig1,UINT8 dig2);
void hour_chage_input(UINT8 i);
void temper_display(void);	
void after_arrive_write_1302(void);

void display_12_24_blink(void);
void year_blink(UINT8 i);
void from_ds1302_read_time_data(void);
void point_blink(void);
void adc_switch(void);

#endif