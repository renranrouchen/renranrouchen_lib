#ifndef _MAIN_C_
#define _MAIN_C_

/*********************************************************************************************************************/
#include "ca51f_config.h"
#if (IC_TYPE == IC_TYPE_CA51F3X)
#include "ca51f3sfr.h"
#include "ca51f3xsfr.h"
#include "gpiodef_f3.h"
#endif
#include "system.h"
#include "uart.h"
#include <intrins.h>
#include "config.h"
#include "usr.h"
#include "FD616.H"
#include "ds1302.H"
#include "delay.h"
#include "math.h"
extern bit sd_mode_flag;


void main(void)
{
#ifdef UART0_EN
	Uart0_Initial(UART0_BAUTRATE);
#endif
#ifdef UART1_EN
	Uart1_Initial(UART1_BAUTRATE);
#endif
#ifdef UART2_EN
	Uart2_Initial(UART2_BAUTRATE);
#endif
	
#ifdef PRINT_EN
	uart_printf("Start\n");
#endif
	
		EA = 1;

		CfgFsys( );
	  RTC_init( );
		gpio_init( );
		adc_init();
		alarm_time_init();
		Timer0_init( );
		DS1302_INIT();
		FD616_Init();	
		FD612_app_init();
		DS1302_INIT();
		display_init_first( );
		bright_contrl();

		
	while(1)
	{
		
		
		key_statue();
		display();
		Alarm_display();
		sd_mode();
		
		if(sd_mode_flag)
		{
			voice_control();
		}
		else
		{	
			bright_contrl();	
		  power_detect( );
		}	
	}
}


#endif
