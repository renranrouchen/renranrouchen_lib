/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      get_local_temp.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-07-12 15:08:08
 * ModifiedTime:  2017-07-12 15:08:08
 */
#include <common.h>
#include <gpio.h>
#include <math.h>
#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#endif
#include <cfg_api.h>
#include <madc.h>
#include "event_process.h"
#include <ui_display.h>
#include <sdk_config.h>
//#include <log/log.h>
extern sdk_param sdkParam;
#if (VERSION_MUSHAN == 1)
#define ADC_PIN 1
#endif
#if (CUSTOMER_VERSION == 1)
#define ADC_PIN 0
#endif
int localTemp = 0x1121;

void get_local_tmp()
{
#if (VERSION_MUSHAN == true)
	int B = 3435;
#endif
#if (VERSION_MUSHAN == false)
	int B = 3950;
#endif
	double calculate = 0.000952939;
	double adcValue = 0.0, vcc = 4.6, temp = 0.00;
	int value = 0;
	char localTemperatureString[5] = {
		0,
	};
	value = analog_read(ADC_PIN);
	 printf("****************************************************adc read %d \n",value);
	adcValue = value * calculate;
	temp = 1 / ((log(adcValue / (vcc - adcValue))) / B + 1 / (273.15 + 25));

	printf("\r\n adc value %d \n",adcValue);
	localTemp = (temp - 273.15 );
	printf(" \r\n temp value %d \n", localTemp);
	
	/*Notice unsigend int is 4Bytes */
#if (CUSTOMER_VERSION == false)
	strcpy(&noData, "CCC");
#if (VERSION_MUSHAN == false)
	if (sdkParam.temperature == TEMP_F)
	{
		localTemp = 32 + localTemp * 1.8;
		strcpy(&noData, "FFF");
	}
#endif
	itoaw(localTemp, localTemperatureString, 10);
	ui_setup(&noData, LOCAL_TEMPERATURE_DATA);
	ui_setup(localTemperatureString, LOCAL_TEMPERATURE_DATA);
#endif
	// printf(" \r\n temp value %d",localTemp);
}

#if (GET_RH_BY_PWM == 1)
#define RH_PWM_IO 17
#define RH_IO_SET(x) (digital_write(RH_PWM_IO, x))

static void init_RH_PWM()
{
	pin_mode(RH_PWM_IO, GPIO_OUT);
	gpio_enable(RH_PWM_IO, ENABLE);
	RH_IO_SET(LOW);
}


char humidity_search_table(char temperature,unsigned int humidityResister)
{
	
}



void display(int value,char parm)
{
	ui_setup(&value, LOCAL_HUMIDITY_DATA);
	//printf("********int result RHValue = %d*********** \n",parm);
	
}
int RHValue = 0;
int RH_value_25_replace;
void RH_ADC_value_get()
{
#define RH_ADC_PIN 0
#define R_0 200
#define VCC (3.32)
	int RHAdcValue = analog_read(RH_ADC_PIN);
	double RHVolt = RHAdcValue * ADC_CALCULATE;
	//double ResistorVolt = R_0 * (VCC - RHVolt)*1.33 /2/ RHVolt;
	double ResistorVolt = R_0 * (VCC - RHVolt) / RHVolt/2.8;
	double RH_value_25 = 1.1264 * pow(ResistorVolt, -0.189);	
	double RH_VALUE_T = (RH_value_25 - 0.004 * (localTemp+2- 25));		
	RHValue = RH_VALUE_T*100 ;
	//printf("********RHAdcValue = %d*********** \n",RHAdcValue);
	//printf("********RHVolt = %d*********** \n",(int)(RHVolt*10));
	//printf("********ResistorVolt = %d*********** \n",(int)(10*ResistorVolt));
	//printf("********RH_value_25 = %d*********** \n",(int)(100*RH_value_25));
	//printf("********int result RHValue = %d*********** \n",RHValue);

	if (RHValue > 99) 
	{
		RHValue = 99;
	}
	
//	if(RHValue<24)
//	{
//		RHValue = 21;	
//	}

//	printf("******** RHValue_first = %d*********** \n",RHValue);
//	if(RHValue<=14){RH_value_25_replace=RHValue;display(RHValue,0);}
//	if(RHValue>=15&&RHValue<=29){RH_value_25_replace=RHValue;display(RH_value_25_replace,1);return;}
//	if(RHValue>=30&&RHValue<=33){RH_value_25_replace=RHValue;display(RH_value_25_replace,2);return;}
//	if(RHValue>=34&&RHValue<=37){RH_value_25_replace=RHValue;display(RH_value_25_replace,3);return;}
//	if(RHValue>=38&&RHValue<=41){RH_value_25_replace=RHValue;display(RH_value_25_replace,4);return;}
//	if(RHValue>=42&&RHValue<=45){RH_value_25_replace=RHValue;display(RH_value_25_replace,5);return;}
//       if(RHValue>=46&&RHValue<=60){RH_value_25_replace=RHValue;display(RH_value_25_replace,6);return;}
//	if(RHValue>=61&&RHValue<=65){RH_value_25_replace=RHValue;display(RH_value_25_replace,7);return;}
//	if(RHValue>=66&&RHValue<=70){RH_value_25_replace=RHValue;display(RH_value_25_replace,8);return;}
//	if(RHValue>=71&&RHValue<=74){RH_value_25_replace=RHValue;display(RH_value_25_replace,9);return;}
//	if(RHValue>=76&&RHValue<=80){RH_value_25_replace=RHValue;display(RH_value_25_replace,10);return;}
//	if(RHValue>=81&&RHValue<=99){RH_value_25_replace=RHValue;display(RH_value_25_replace,11);return;}
//	if(RHValue>=0&&RHValue<=14){RH_value_25_replace=RHValue-0 ;display(RH_value_25_replace,0);return;}
       if(RHValue>=0&&RHValue<=25){RH_value_25_replace=RHValue ;display(RH_value_25_replace,0);return;}
       if(RHValue>=15&&RHValue<=27){RH_value_25_replace=RHValue ;display(RH_value_25_replace,7);return;}
	if(RHValue>=28&&RHValue<=29){RH_value_25_replace=RHValue ;display(RH_value_25_replace,5);return;}
	if(RHValue>=30&&RHValue<=33){RH_value_25_replace=RHValue-1;display(RH_value_25_replace,2);return;}
	if(RHValue>=34&&RHValue<=37){RH_value_25_replace=RHValue;display(RH_value_25_replace,2);return;}
	if(RHValue>=38&&RHValue<=41){RH_value_25_replace=RHValue+2;display(RH_value_25_replace,2);return;}
	if(RHValue>=42&&RHValue<=45){RH_value_25_replace=RHValue+2;display(RH_value_25_replace,2);return;}
       if(RHValue>=46&&RHValue<=60){RH_value_25_replace=RHValue+2;display(RH_value_25_replace,2);return;}
	if(RHValue>=61&&RHValue<=65){RH_value_25_replace=RHValue+2 ;display(RH_value_25_replace,2);return;}
	if(RHValue>=66&&RHValue<=70){RH_value_25_replace=RHValue+1;display(RH_value_25_replace,1);return;}
	if(RHValue>=71&&RHValue<=89){RH_value_25_replace=RHValue+0;display(RH_value_25_replace,0);return;}
	if(RHValue>=90&&RHValue<=99){RH_value_25_replace=RHValue-3 ;display(RH_value_25_replace,3);return;}
	ui_setup(&RH_value_25_replace, LOCAL_HUMIDITY_DATA);	
	
//	if(RHValue<25){RH_value_25_replace=RHValue;}
//		 else if (RHValue<30){RH_value_25_replace=RHValue-12;}
//		 else if (RHValue<34){RH_value_25_replace=RHValue-10;}
//		 else if (RHValue<38){RH_value_25_replace=RHValue-4;}
//		 else if (RHValue<42){RH_value_25_replace=RHValue-2;}
//		 else if (RHValue<46){RH_value_25_replace=RHValue-2;}
//		 else if (RHValue<61){RH_value_25_replace=RHValue+0;}
//		 else if (RHValue<66){RH_value_25_replace=RHValue+1;}
//		 else if (RHValue<71){RH_value_25_replace=RHValue+2;}
//		 else if (RHValue<76){RH_value_25_replace=RHValue+3;}
//		 else if (RHValue<81){RH_value_25_replace=RHValue+4;}
//		 else if (RHValue<86){RH_value_25_replace=RHValue+5;}
//		 else if (RHValue<100){RH_value_25_replace=RHValue;}
//	ui_setup(&RH_value_25_replace, LOCAL_HUMIDITY_DATA);

}

void create_PWM(void *param)
{
	unsigned int time = 0;
	init_RH_PWM();
	while (1)
	{
		if (s_linkup == true)
		{
			RH_IO_SET(HIGH);
			// sys_msleep(10);
			udelay(100);
			//		RH_ADC_value_get();
			if (time++ > RH_UPDATE_TIME)
			{
				/* Check is it first entry */
				if (localTemp == 0x1121)
				{
					send_event(GET_LOCAL_TEMP_VALUE);
				}
				RH_ADC_value_get();
				time = 0;
			}
			udelay(100);
			// sys_msleep(10);
			RH_IO_SET(LOW);
			sys_msleep(10);
			udelay(200);
		}
		else
		{
			sys_msleep(1000);
		}
	}
}
#endif
