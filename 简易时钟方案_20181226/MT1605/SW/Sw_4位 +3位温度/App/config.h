#ifndef	__CONFIG_H__
#define __CONFIG_H__

#include "FD814.H" 


#define		BS(SFR,NUM)		(SFR|=(1<<NUM))             //Î»ÖÃ1
#define		BC(SFR,NUM)		(SFR&=~(1<<NUM))		 				 //Î»Çå0

void Port1Cfg(UINT8 Mode,UINT8 Pin);
void Port3Cfg(UINT8 Mode,UINT8 Pin);
void gpio_init(void);
void ADC0_INIT(UINT8 div);
void time0_init(void);
void time1_init(void);
void PWM_init(void);
void DS1302_INIT(void);
void time2_init(void);
void display_init_first(void);

#endif