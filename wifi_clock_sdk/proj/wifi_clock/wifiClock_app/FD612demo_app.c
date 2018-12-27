/*-------------------------------------------------------------------------
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief			
*@FileName		FD612demo_app.c
*@Data			2017/11/2 14:45:41
-------------------------------------------------------------------------*/
/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      FD612demo_app.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-22 09:29:45
 * ModifiedTime:  2017-06-22 09:29:45
 */
#include <common.h>
#include <wla_api.h>
#include <log/log.h>
#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
//#include <timers.h>
#endif

#include <rtc.h>
#include "wifi_time.h"
#include "FD612Drv.h"
#include "wifi_local_tick.h"
#include "wifi_pm25.h"
#include "FD612demo_app.h"
#include <cfg_api.h>
#include <wifi_clock_cfg.h>

t_fd612DispTest FD612Test;
extern sdk_param sdkParam;

static t_fd612_ops fd612_top_ops;
static FD612_Struct_DispBuff FD612_top_DispBuff;
static t_fd612_ops fd612_base_ops;
static FD612_Struct_DispBuff FD612_base_DispBuff;
static t_fd612_ops fd612_middle_ops;
static FD612_Struct_DispBuff FD612_middle_DispBuff;

unsigned int displayIntense = FD612_INTENS1;
void display_init_mode()
{
	FD612_DispStateWr(&fd612_top_ops, &FD612_top_DispBuff, displayIntense | FD612_DISP_ON);
	FD612_DispStateWr(&fd612_base_ops, &FD612_base_DispBuff, displayIntense | FD612_DISP_ON);
}

inline void fd612_display_on()
{
	FD612Test.displayOn = ON;
}
inline void fd612_display_off()
{
	FD612Test.displayOn = OFF;
}
inline void fd612_display_mode_one()
{
	FD612Test.displayOn = MODE_ONE;
}

void inline all_light_debug()
{
	unsigned char loop = 0;
	unsigned char buff[12];
	for (loop = 0; loop < 12; loop++)
	{
		buff[loop] = 0xFF;
	}
	//FD612_app_init();
	display_data_toFD612(TOP, buff);
	display_data_toFD612(BASE, buff);
	display_data_toFD612(MIDDLE, buff);
}

#if (K780_API == 1)
void display_weather_notify(char *buffOne, char *buffTwo)
{
	char weatid = weather.weatid[0] - '0';
	printf("\r\n====weatid is %d,%d========\r\n",weatid,DispGetCode(weather.weatid[0]));
	/* Clear chart buff */
	buffTwo[11] &= 0x00;
	buffTwo[8]  &= ~(0x80);
	buffTwo[9]  &= ~(0x80);
	buffTwo[10] &= ~(0x80);
	buffTwo[0]  &= ~(0x80);
	buffTwo[1] &= ~(0x80);
	// buffOne[11] &= 0x00;
	buffTwo[5] &= ~0x80;
	buffTwo[6] &= ~0X80;
	buffTwo[7] &= ~0X80;	

	switch (weatid)
	{
	case SUNNY:
		//D2
#if CUSTOMER_VERSION
		buffTwo[11] |= 0xFC;
#else
		buffTwo[0] |= 0x80;
#endif
		break;
	case CLOUDY:
#if CUSTOMER_VERSION
		buffTwo[8] |= 0x80;
		buffTwo[9] |= 0x80;
		buffTwo[10] |= 0x80;
		buffTwo[11] |= 0x03;
#else
		buffTwo[4] |= 0x80;
#endif
		break;
	case OVERCAST:
#if CUSTOMER_VERSION
		buffTwo[8] |= 0x80;
		buffTwo[9] |= 0x80;
		buffTwo[10] |= 0x80;
		buffTwo[11] |= 0x03;
#else
		buffTwo[3] |= 0x80;
#endif
		break;
	case SHOWER:
	case THUNDER:
		buffTwo[0] |= 0x80;
		buffTwo[1] |= 0x80;
		buffOne[11] |= 0xE0;
	case THUNDERSTORM:
	case SHOW_AND_RAIN:
	case DRIZZLE:
#if CUSTOMER_VERSION
		buffTwo[8] |= 0x80;
		buffTwo[9] |= 0x80;
		buffTwo[10] |= 0x80;
		buffTwo[11] |= 0x03;
		buffOne[11] |= 0x20;
#else
		buffTwo[2] |= 0x80;
#endif
		break;
	case MODERATE_RAIN:
	case SHOWER_MID_RAIN:
#if CUSTOMER_VERSION
		buffTwo[8] |= 0x80;
		buffTwo[9] |= 0x80;
		buffTwo[10] |= 0x80;
		buffTwo[11] |= 0x03;
		buffOne[11] |= 0x60;
#else
		buffTwo[7] |= 0x80;
#endif

		break;
	case MID_HEAVY_RAIN:
	case HEAVY_STORM_RAIN:
	case STORM_DP_RAIN:
	case DP_HEAVYDP_RAIN:
	case HEAVY_RAIN:
	case RAINSTORM:
	case DOWNPOUR:
	case HEAVY_DOWNPOUR:
#if CUSTOMER_VERSION
		buffTwo[8] |= 0x80;
		buffTwo[9] |= 0x80;
		buffTwo[10] |= 0x80;
		buffTwo[11] |= 0x03;
		buffOne[11] |= 0xE0;
#else
		buffTwo[10] |= 0x80;
#endif

		break;
	case SNOW:
	case LIGHT_SNOW:
	case NODERATE_SNOW:
	case GREAT_SNOW:
	case BLIZZARD:
	case LIGHT_MID_SNOW:
	case NID_HEAVY_SNOW:
	case HEAVY_BLIZZARD_SNOW:
#if CUSTOMER_VERSION
		//D4
		buffOne[11] |= 0x1C;
#else
		buffTwo[8] |= 0x80;
#endif
		break;
	case MIST:
		//D9
#if CUSTOMER_VERSION
		buffTwo[5] |= 0x80;
		buffTwo[6] |= 0X80;
		buffTwo[7] |= 0X80;
#else
		buffTwo[11] |= 0x80;
#endif
		break;
	case HAZE:
		//D6
#if CUSTOMER_VERSION
		buffTwo[5] |= 0x80;
		buffTwo[6] |= 0X80;
		buffTwo[7] |= 0X80;
#else
		buffTwo[6] |= 0x80;
#endif

		break;
	//reserver for future
	case SINK:
	case RAISE_DUST:
	case HEAVY_SANDSTORM:
		break;
	default:
		break;
	}
}
#endif

void display_data_toFD612(char pos, unsigned char *buff)
{
	// char count = 0;
	char loop = 0;
	if (pos == TOP)
	{
		for (loop = 0; loop < 12; loop++)
		{
			FD612_top_DispBuff.DispData[loop] = buff[loop];
		}
		FD612_Refresh(&FD612_top_DispBuff, &fd612_top_ops);
	}
	else if (pos == BASE)
	{
		for (loop = 0; loop < 12; loop++)
		{
			FD612_base_DispBuff.DispData[loop] = buff[loop];
		}
		FD612_Refresh(&FD612_base_DispBuff, &fd612_base_ops);
	} else if (pos == MIDDLE) 
	{
		for (loop = 0; loop < 12; loop++)
		{
			FD612_middle_DispBuff.DispData[loop] = 0xFF;
		}
		FD612_Refresh(&FD612_middle_DispBuff, &fd612_middle_ops);	
	}
}
#if 0
void FD612_printf_test(sntp_tm time)
{
	int year;
	unsigned char buff[12] = {
		0,
	};
	/* display current time */
	year = time.tm_year + 1900;

	/*TODO:
	*check display mode on/off ?
	*/
	buff[8] = DISP_TAB[time.tm_hour / 10];
	buff[9] = DISP_TAB[time.tm_hour % 10] | FD612Test.secBit;
	buff[10] = DISP_TAB[time.tm_min / 10];
	buff[11] = DISP_TAB[time.tm_min % 10];

	if (FD612Test.dispTimeDelay-- == 0)
	{
		FD612Test.dispTimeflag = ((~FD612Test.dispTimeflag) & 0x01);
		FD612Test.dispTimeDelay = 30;
	}
	if (FD612Test.dispTimeflag == 0)
	{
		buff[0] = DISP_TAB[year / 1000];
		buff[1] = DISP_TAB[year / 10 / 10 % 10];
		buff[2] = DISP_TAB[year % 100 / 10];
		buff[3] = DISP_TAB[year % 10];

		buff[4] = DISP_TAB[(time.tm_mon + 1) / 10];
		buff[5] = DISP_TAB[(time.tm_mon + 1) % 10];
		buff[6] = DISP_TAB[time.tm_mday / 10];
		buff[7] = DISP_TAB[time.tm_mday % 10];
	}
	else
	{
		if (weather.humidity != NULL)
		{
			buff[0] = DispGetCode(weather.humidity[0]);
			buff[1] = DispGetCode(weather.humidity[1]);
			if (weather.humidity[2] != '%')
			{
				buff[2] = DispGetCode(weather.humidity[2]);
			}
			else
			{
				buff[2] = 0x00;
			}
		}

		if (weather.tempCurr != NULL)
		{
			buff[3] = DispGetCode(weather.tempCurr[0]);
			buff[4] = DispGetCode(weather.tempCurr[1]);
		}
		if (wifiPm25.aqi != NULL)
		{
			buff[5] = DispGetCode(wifiPm25.aqi[0]);
			buff[6] = DispGetCode(wifiPm25.aqi[1]);
			if (wifiPm25.aqi[2] != '\0')
			{
				buff[7] = DispGetCode(wifiPm25.aqi[2]);
			}
			else
			{
				buff[7] = 0x00;
			}
		}
	}
	display_weather_notify(buff);

	FD612Test.secBit = (~FD612Test.secBit & 0x80);
	display_data_toFD612(buff);
}
#endif

#if (K780_API == 1)
inline void count_to_display_mode()
{
	if (FD612Test.displayOn == MODE_ONE && FD612Test.countToDispOff-- < 0)
	{
		fd612_display_off();
	}
	if (FD612Test.displayOn == ON && FD612Test.countToDispModeOne-- < 0)
	{
		fd612_display_mode_one();
	}
}
inline void display_mode_set()
{
#if (KEYI_SHOW_DISPLAY_DEMO == 0)
	count_to_display_mode();
#endif
	switch (FD612Test.displayOn)
	{
	case OFF:
		FD612_DispStateWr(&fd612_top_ops, &FD612_top_DispBuff, FD612_INTENS1 | FD612_DISP_OFF);
		FD612_DispStateWr(&fd612_base_ops, &FD612_base_DispBuff, FD612_INTENS1 | FD612_DISP_OFF);
		FD612_DispStateWr(&fd612_middle_ops, &FD612_middle_DispBuff, FD612_INTENS1 | FD612_DISP_OFF);
		break;
	case ON:
		// FD612_DispStateWr(&fd612_top_ops, &FD612_top_DispBuff, intenseTable[sdkParam.brightness] | FD612_DISP_ON);
		// FD612_DispStateWr(&fd612_base_ops, &FD612_base_DispBuff, intenseTable[sdkParam.brightness] | FD612_DISP_ON);
		// FD612_DispStateWr(&fd612_middle_ops, &FD612_middle_DispBuff, intenseTable[sdkParam.brightness] | FD612_DISP_ON);
		FD612_DispStateWr(&fd612_top_ops, &FD612_top_DispBuff, intenseTable[7] | FD612_DISP_ON);
		FD612_DispStateWr(&fd612_base_ops, &FD612_base_DispBuff, intenseTable[7] | FD612_DISP_ON);
		FD612_DispStateWr(&fd612_middle_ops, &FD612_middle_DispBuff, intenseTable[7] | FD612_DISP_ON);
		break;
	case MODE_ONE:
		FD612_DispStateWr(&fd612_top_ops, &FD612_top_DispBuff, FD612_INTENS1 | FD612_DISP_ON);
		FD612_DispStateWr(&fd612_base_ops, &FD612_base_DispBuff, FD612_INTENS1 | FD612_DISP_ON);
		FD612_DispStateWr(&fd612_middle_ops, &FD612_middle_DispBuff, FD612_INTENS1 | FD612_DISP_ON);
		break;
	default:
		break;
	}
}

void FD612_printf_test(const void *const rtcTime)
{
#if RTC_TIME
	const t_time *const time = rtcTime;
#else
	const sntp_tm *const time = rtcTime;
#endif
	int temperature = 0;
	char localTemperature[10] = {0};
	char tempCurr[10] = {0};
	int year = 0;
	unsigned char hour = 0;
	unsigned char buff[12] = {
		0,
	};
	unsigned char buffTwo[12] = {
		0,
	};

	/*TODO:
	*check display mode on/off ?
	*/
	display_mode_set();
#if RTC_TIME
	// #erro "xxx"
	year = time->year + 2000;
	/*TODO:
	*add 12/24 check
	*/

	if (sdkParam.timeFmt_param.fmt == FORMAT_24)
	{
#if CUSTOMER_VERSION
		buff[0] = DISP_TAB[time->hour / 10] | FD612Test.secBit;
		buff[1] = DISP_TAB[time->hour % 10] | FD612Test.secBit;
#else /*CUSTOMER_VERSION*/
		buff[4] = DISP_TAB[time->hour / 10];
		buff[5] = DISP_TAB[time->hour % 10] | FD612Test.secBit;
#endif
	}
	else
	{
		hour = time->hour;
		if (time->hour > 12)
		{
			hour = time->hour - 12;
		}
#if CUSTOMER_VERSION
		buff[0] = DISP_TAB[hour / 10] | FD612Test.secBit;
		buff[1] = DISP_TAB[hour % 10] | FD612Test.secBit;
#else /*CUSTOMER_VERSION*/
		buff[4] = DISP_TAB[hour / 10];
		buff[5] = DISP_TAB[hour % 10] | FD612Test.secBit;
#endif
	}
#if CUSTOMER_VERSION
	buff[2] = DISP_TAB[time->min / 10];
	buff[3] = DISP_TAB[time->min % 10];

	buff[4] = DISP_TAB[year % 100 / 10];
	buff[5] = DISP_TAB[year % 10];

	buff[6] = DISP_TAB[(time->month) / 10];
	buff[7] = DISP_TAB[(time->month) % 10];
	buff[8] = DISP_TAB[time->day / 10];
	buff[9] = DISP_TAB[time->day % 10];
	/* week */
	buff[10] = DISP_TAB[time->week % 10];
	
	/* Display 20 */
	buff[2] |= 0x80;
	buff[7] |= 0x80;
	buff[8] |= 0x80;
	buff[9] |= 0x80;
	buff[11] |= 0x03;
	buff[10] |= 0x80;
	buff[3] |= 0x80;
	buff[4] |= 0x80;
	buff[5] |= 0x80;
	buff[6] |= 0x80;
#else /*CUSTOMER_VERSION*/
	buff[6] = DISP_TAB[time->min / 10];
	buff[7] = DISP_TAB[time->min % 10];

	buff[0] = DISP_TAB[year / 1000];
	buff[1] = DISP_TAB[year / 10 / 10 % 10];
	buff[2] = DISP_TAB[year % 100 / 10];
	buff[3] = DISP_TAB[year % 10];

	buff[8] = DISP_TAB[(time->month) / 10];
	buff[9] = DISP_TAB[(time->month) % 10];
	buff[10] = DISP_TAB[time->day / 10];
	buff[11] = DISP_TAB[time->day % 10];
	/* week */
	buffTwo[11] = DISP_TAB[time->week % 10];
#endif

#else /* RTC_TIME*/

	year = time->tm_year + 1900;
	if (sdkParam.timeFmt_param.fmt == FORMAT_24)
	{
#if CUSTOMER_VERSION
		// #erro	"program test"
		buff[0] = DISP_TAB[time->tm_hour / 10] | FD612Test.secBit;
		buff[1] = DISP_TAB[time->tm_hour % 10] | FD612Test.secBit;
#else /*CUSTOMER_VERSION*/
		//#erro	"program test"
		buff[4] = DISP_TAB[time->tm_hour / 10];
		buff[5] = DISP_TAB[time->tm_hour % 10] | FD612Test.secBit;
#endif
	}
	else
	{
		hour = time->tm_hour;
		if (time->tm_hour > 12)
		{
			hour = time->tm_hour - 12;
		}
#if CUSTOMER_VERSION
		buff[0] = DISP_TAB[hour / 10] | FD612Test.secBit;
		buff[1] = DISP_TAB[hour % 10] | FD612Test.secBit;
#else /*CUSTOMER_VERSION*/
		buff[4] = DISP_TAB[hour / 10];
		buff[5] = DISP_TAB[hour % 10] | FD612Test.secBit;
#endif
	}
#if CUSTOMER_VERSION
	buff[2] = DISP_TAB[time->tm_min / 10];
	buff[3] = DISP_TAB[time->tm_min % 10];

	buff[4] = DISP_TAB[year % 100 / 10];
	buff[5] = DISP_TAB[year % 10];

	buff[6] = DISP_TAB[(time->tm_mon) / 10];
	buff[7] = DISP_TAB[(time->tm_mon) % 10];
	buff[8] = DISP_TAB[time->tm_mday / 10];
	buff[9] = DISP_TAB[time->tm_mday % 10];
	/* week */
	buff[10] = DISP_TAB[(time->tm_wday) % 10];
	/* Display 20 */
	buff[2] |= 0x80;
	buff[7] |= 0x80;
	buff[8] |= 0x80;
	buff[9] |= 0x80;
	buff[11] |= 0x03;
	buff[10] |= 0x80;
	buff[3] |= 0x80;
	buff[4] |= 0x80;
	buff[5] |= 0x80;
	buff[6] |= 0x80;
#else /*CUSTOMER_VERSION*/
	buff[6] = DISP_TAB[time->tm_min / 10];
	buff[7] = DISP_TAB[time->tm_min % 10];

	buff[0] = DISP_TAB[year / 1000];
	buff[1] = DISP_TAB[year / 10 / 10 % 10];
	buff[2] = DISP_TAB[year % 100 / 10];
	buff[3] = DISP_TAB[year % 10];

	buff[8] = DISP_TAB[(time->tm_mon) / 10];
	buff[9] = DISP_TAB[(time->tm_mon) % 10];
	buff[10] = DISP_TAB[time->tm_mday / 10];
	buff[11] = DISP_TAB[time->tm_mday % 10];
	/* week */
	buffTwo[11] = DISP_TAB[(time->tm_wday) % 10];
#endif
#endif /* RTC_TIME */

	if (weather.humidity != NULL)
	{
#if CUSTOMER_VERSION
		/* Disp wifi state */
		buffTwo[0] = DispGetCode(weather.humidity[0]);
		buffTwo[1] = DispGetCode(weather.humidity[1]);
#else
		buffTwo[7] = DispGetCode(weather.humidity[0]);
		buffTwo[0] = DispGetCode(weather.humidity[1]);
#endif
	}
	if (wifiPm25.aqi != NULL)
	{
#if CUSTOMER_VERSION
		buffTwo[2] = DispGetCode(wifiPm25.aqi[0]);
		buffTwo[3] = DispGetCode(wifiPm25.aqi[1]);
		if (wifiPm25.aqi[2] != '\0')
		{
			buffTwo[4] = DispGetCode(wifiPm25.aqi[2]);
		}
		else
		{
			buffTwo[4] = 0x00;
		}
#else /*CUSTOMER_VERSION*/
		buffTwo[1] = DispGetCode(wifiPm25.aqi[0]);
		buffTwo[2] = DispGetCode(wifiPm25.aqi[1]);
		if (wifiPm25.aqi[2] != '\0')
		{
			buffTwo[3] = DispGetCode(wifiPm25.aqi[2]);
		}
		else
		{
			buffTwo[3] = 0x00;
		}
#endif
	}

	/* local temperature */
	extern int localTemp;
	itoaw(localTemp, localTemperature, 10);

	if (sdkParam.temperature == TEMP_F)
	{
		if (weather.tempCurr != NULL)
		{
			temperature = atoi(weather.tempCurr);
			temperature = 32 + temperature * 1.8;
			itoaw(temperature, tempCurr, 10);
		}
	}
	else
	{
		memcpy(tempCurr, weather.tempCurr, sizeof(tempCurr));
	}
#if CUSTOMER_VERSION
	buffTwo[5] = DispGetCode(localTemperature[0]);
	buffTwo[6] = DispGetCode(localTemperature[1]);
	buffTwo[7] = DispGetCode(localTemperature[2]);
	buffTwo[8] = DispGetCode(tempCurr[0]);
	buffTwo[9] = DispGetCode(tempCurr[1]);
	buffTwo[10] = DispGetCode(tempCurr[2]);
	extern char s_linkup;
	if (s_linkup == CONNECT)
	{
		buffTwo[2] |= 0x80;
		buffTwo[3] |= 0x80;
	}
	else
	{
		buffTwo[2] &= ~0x80;
		buffTwo[3] &= ~0x80;
	}
	/* check alarm state */
	extern char isAlarm;
	if (isAlarm == 1)
	{
		buffTwo[4] |= 0x80;
	}
	else
	{
		buffTwo[4] &= ~0x80;
	}
#else /* CUSTOMER_VERSION */
	buffTwo[4] = DispGetCode(tempCurr[0]);
	buffTwo[5] = DispGetCode(tempCurr[1]);
	buffTwo[6] = DispGetCode(tempCurr[2]);

	buffTwo[8] = DispGetCode(localTemperature[0]);
	buffTwo[9] = DispGetCode(localTemperature[1]);
	buffTwo[10] = DispGetCode(localTemperature[2]);
#endif

#if (KEYI_SHOW_DISPLAY_DEMO == 1)
		/* Disp wifi state */
	buffTwo[0] = DispGetCode('3');
	buffTwo[1] = DispGetCode('5');

	buffTwo[2] = DispGetCode(' ');
	buffTwo[3] = DispGetCode('3');
	buffTwo[4] = DispGetCode('2');

	buffTwo[5] = DispGetCode(' ');
	buffTwo[6] = DispGetCode('2');
	buffTwo[7] = DispGetCode('0');
	buffTwo[8] = DispGetCode(' ');
	buffTwo[9] = DispGetCode('2');
	buffTwo[10] = DispGetCode('1');
	static unsigned int loop = 0,index = 0;
	static char weatherChartTable[] = {
		SUNNY,
		CLOUDY,
	    // OVERCAST,
	    THUNDER,
	    DRIZZLE,
	    SHOWER_MID_RAIN,
		HEAVY_DOWNPOUR,
		HEAVY_BLIZZARD_SNOW,
		MIST
		// HAZE,
		// HEAVY_SANDSTORM
	};
	weather.weatid[0] = '0' + weatherChartTable[index];
	if (loop++ > 6) 
	{
		if (++index >= sizeof(weatherChartTable) / sizeof(char))
		{
			index = 0;
		}
		loop = 0;	
	}
	display_weather_notify(buff, buffTwo);
#else
	display_weather_notify(buff, buffTwo);
#endif
	FD612Test.secBit = (~FD612Test.secBit & 0x80);
	display_data_toFD612(TOP, buff);
	display_data_toFD612(BASE, buffTwo);
	display_data_toFD612(MIDDLE, buffTwo);
}
#endif /* (K780_API == 1) */

void FD612_SCL_D_OUT()
{
	pin_mode(SCL, 1);
}
void FD612_SDA_D_OUT()
{
	pin_mode(SDA, 1);
}
void FD612_SDA_D_IN()
{
	pin_mode(SDA, 0);
}

int FD612_SDA_IN()
{
	return digital_read(SDA);
}

void FD612_SCL_SET()
{
	digital_write(SCL, 1);
}

void FD612_SCL_CLR()
{
	digital_write(SCL, 0);
}

void FD612_SDA_SET()
{
	digital_write(SDA, 1);
}

void FD612_SDA_CLR()
{
	digital_write(SDA, 0);
}

void FD612_BASE_SCL_D_OUT()
{
	pin_mode(SCL_BASE, 1);
}
void FD612_BASE_SDA_D_OUT()
{
	pin_mode(SDA_BASE, 1);
}
void FD612_BASE_SDA_D_IN()
{
	pin_mode(SDA_BASE, 0);
}

int FD612_BASE_SDA_IN()
{
	return digital_read(SDA_BASE);
}

void FD612_BASE_SCL_SET()
{
	digital_write(SCL_BASE, 1);
}

void FD612_BASE_SCL_CLR()
{
	digital_write(SCL_BASE, 0);
}

void FD612_BASE_SDA_SET()
{
	digital_write(SDA_BASE, 1);
}

void FD612_BASE_SDA_CLR()
{
	digital_write(SDA_BASE, 0);
}


#if (KEYI_SHOW_DISPLAY_DEMO == 1)
void FD612_MIDDLE_SCL_D_OUT()
{
	pin_mode(SCL_MIDDLE, 1);
}
void FD612_MIDDLE_SDA_D_OUT()
{
	pin_mode(SDA_MIDDLE, 1);
}
void FD612_MIDDLE_SDA_D_IN()
{
	pin_mode(SDA_MIDDLE, 0);
}

int FD612_MIDDLE_SDA_IN()
{
	return digital_read(SDA_MIDDLE);
}

void FD612_MIDDLE_SCL_SET()
{
	digital_write(SCL_MIDDLE, 1);
}

void FD612_MIDDLE_SCL_CLR()
{
	digital_write(SCL_MIDDLE, 0);
}

void FD612_MIDDLE_SDA_SET()
{
	digital_write(SDA_MIDDLE, 1);
}

void FD612_MIDDLE_SDA_CLR()
{
	digital_write(SDA_MIDDLE, 0);
}
#endif


static void inline operate_function_init()
{

}

static void display_init()
{
		/* GPIO PIN9 SCL GPIO PIN 8 SDA*/
	gpio_enable(SDA,1);
	gpio_enable(SCL,1);

	gpio_enable(SDA_BASE,1);
	gpio_enable(SCL_BASE,1);
#if (KEYI_SHOW_DISPLAY_DEMO == 1)	
	gpio_enable(SDA_MIDDLE,1);
	gpio_enable(SCL_MIDDLE,1);

	fd612_middle_ops.FD612_SCL_CLR = FD612_MIDDLE_SCL_CLR;
	fd612_middle_ops.FD612_SCL_D_OUT = FD612_MIDDLE_SCL_D_OUT;
	fd612_middle_ops.FD612_SCL_SET = FD612_MIDDLE_SCL_SET;
	fd612_middle_ops.FD612_SDA_CLR = FD612_MIDDLE_SDA_CLR;
	fd612_middle_ops.FD612_SDA_D_IN = FD612_MIDDLE_SDA_D_IN;
	fd612_middle_ops.FD612_SDA_D_OUT = FD612_MIDDLE_SDA_D_OUT;
	fd612_middle_ops.FD612_SDA_IN = FD612_MIDDLE_SDA_IN;
	fd612_middle_ops.FD612_SDA_SET = FD612_MIDDLE_SDA_SET;
#endif	
	fd612_top_ops.FD612_SCL_CLR = FD612_SCL_CLR;
	fd612_top_ops.FD612_SCL_D_OUT = FD612_SCL_D_OUT;
	fd612_top_ops.FD612_SCL_SET = FD612_SCL_SET;
	fd612_top_ops.FD612_SDA_CLR = FD612_SDA_CLR;
	fd612_top_ops.FD612_SDA_D_IN = FD612_SDA_D_IN;
	fd612_top_ops.FD612_SDA_D_OUT = FD612_SDA_D_OUT;
	fd612_top_ops.FD612_SDA_IN = FD612_SDA_IN;
	fd612_top_ops.FD612_SDA_SET = FD612_SDA_SET;

	fd612_base_ops.FD612_SCL_CLR = FD612_BASE_SCL_CLR;
	fd612_base_ops.FD612_SCL_D_OUT = FD612_BASE_SCL_D_OUT;
	fd612_base_ops.FD612_SCL_SET = FD612_BASE_SCL_SET;
	fd612_base_ops.FD612_SDA_CLR = FD612_BASE_SDA_CLR;
	fd612_base_ops.FD612_SDA_D_IN = FD612_BASE_SDA_D_IN;
	fd612_base_ops.FD612_SDA_D_OUT = FD612_BASE_SDA_D_OUT;
	fd612_base_ops.FD612_SDA_IN = FD612_BASE_SDA_IN;
	fd612_base_ops.FD612_SDA_SET = FD612_BASE_SDA_SET;


}

void FD612_ZERO_INIT()
{
		unsigned char loop;
	//FD612Test.dispTimeDelay = 30;
	FD612Test.dispTimeflag = 0;
	FD612Test.secBit = 0x00;
	fd612_display_on();
	FD612Test.countToDispOff = COUNT_TO_OFF(30);
	display_init();
	
	FD612_Init(&fd612_base_ops, &FD612_base_DispBuff);
	FD612_Init(&fd612_top_ops, &FD612_top_DispBuff);
#if (KEYI_SHOW_DISPLAY_DEMO == 1)	
	FD612_Init(&fd612_middle_ops,&FD612_middle_DispBuff);
#endif

#if 1
	for (loop = 0; loop < 12; loop++)
	{
		FD612_top_DispBuff.DispData[loop] = 0x00;
	}
#endif
	FD612_DispStateWr(&fd612_top_ops, &FD612_top_DispBuff, FD612_INTENS8 | FD612_DISP_ON);
	
	FD612_Refresh(&FD612_top_DispBuff, &fd612_top_ops);
#if 1

	for (loop = 0; loop < 12; loop++)
	{
		FD612_base_DispBuff.DispData[loop] = 0x00;
	}
#endif
FD612_DispStateWr(&fd612_base_ops, &FD612_base_DispBuff, FD612_INTENS8 | FD612_DISP_ON);
	FD612_Refresh(&FD612_base_DispBuff, &fd612_base_ops);

#if (KEYI_SHOW_DISPLAY_DEMO == 1)
#if 1
	for (loop = 0; loop < 12; loop++)
	{
		FD612_base_DispBuff.DispData[loop] = 0xFF;
	}
#endif
	FD612_Refresh(&FD612_middle_DispBuff, &fd612_middle_ops);
#endif
}
void FD612_app_init()
{
	unsigned char loop;
	//FD612Test.dispTimeDelay = 30;
	FD612Test.dispTimeflag = 0;
	FD612Test.secBit = 0x00;
	fd612_display_on();
	FD612Test.countToDispOff = COUNT_TO_OFF(30);
	display_init();
	
	FD612_Init(&fd612_base_ops, &FD612_base_DispBuff);
	FD612_Init(&fd612_top_ops, &FD612_top_DispBuff);
#if (KEYI_SHOW_DISPLAY_DEMO == 1)	
	FD612_Init(&fd612_middle_ops,&FD612_middle_DispBuff);
#endif

#if 1
	for (loop = 0; loop < 12; loop++)
	{
		FD612_top_DispBuff.DispData[loop] = 0x00;
	}
#endif
	//FD612_DispStateWr(&fd612_top_ops, &FD612_top_DispBuff, FD612_INTENS8 | FD612_DISP_ON);
	
	FD612_Refresh(&FD612_top_DispBuff, &fd612_top_ops);
#if 1

	for (loop = 0; loop < 12; loop++)
	{
		FD612_base_DispBuff.DispData[loop] = 0x00;
	}
#endif
//FD612_DispStateWr(&fd612_base_ops, &FD612_base_DispBuff, FD612_INTENS8 | FD612_DISP_ON);
	FD612_Refresh(&FD612_base_DispBuff, &fd612_base_ops);

#if (KEYI_SHOW_DISPLAY_DEMO == 1)
#if 1
	for (loop = 0; loop < 12; loop++)
	{
		FD612_base_DispBuff.DispData[loop] = 0xFF;
	}
#endif
	FD612_Refresh(&FD612_middle_DispBuff, &fd612_middle_ops);
#endif
}
