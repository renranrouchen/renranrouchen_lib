
/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2017/01/20
* Description        : GPIO 设置与使用和GPIO中断使用示例   
*******************************************************************************/
#include "FD814.H"                                                   
#include "Debug.H"
#include "config.h"
#include "FD612Drv.H"
#include "stdio.h"
#include <string.h>
#include "usr.h"
#pragma  NOAREGS

extern unsigned char dislay_all_flag,sd_mode_flag;



void main( ) 
{	

    CfgFsys( );                                                                //CH554时钟选择配置   
    mDelaymS(20);	
    gpio_init();
    time0_init( );
		time1_init( );
		alarm_time_init();
		ADC0_INIT(0);
		DS1302_INIT();
		ADC_Convert( );
		FD612_Init( );
		display_init_first( );
   while(1)
			 { 


					key_statue();
					if(dislay_all_flag==0)	display();
					alarm_display ( );

					if(sd_mode_flag)	voice_control();					 
					sd_mode ( );

				} 
	

									 
			 
						
}
			
		 
	