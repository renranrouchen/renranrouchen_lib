#ifndef	__CONFIG_H__
#define __CONFIG_H__

#define		BS(SFR,NUM)		(SFR|=(1<<NUM))             //位置1
#define		BC(SFR,NUM)		(SFR&=~(1<<NUM))		 				 //位清0
#include "ca51f3sfr.h"
#include "ca51f3xsfr.h"
#include "gpiodef_f3.h"

#define AST(N) (N<<7) //N=0~1
#define ADIE(N) (N<<6) //N=0~1
#define ADIF (1<<5) //中断标志
#define HTME(N) (N<<2) //N=0~7 //采样时间
#define VSEL(N) (N) //N=0~3 //参考电压
#define ACKD(N) (N<<5) //N=0~7
#define ADCALE(N) (N<<4) //N=0~1
#define ADCHS(N) (N) //N=0~15 //



void gpio_init(void);
void CfgFsys(void);
void RTC_init(void);
void adc_init(void);
void DS1302_INIT(void);
void Timer0_init(void);
void display_init_first(void);
#endif