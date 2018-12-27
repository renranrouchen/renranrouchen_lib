/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      ui_display.c
 * Author:        zwxf
 * Email:         zwxf.cw@gmail.com
 * DateTime:      2017-10-18 09:48:37
 * ModifiedTime:  2017-10-18 09:48:37
 */
#include <wifi_clock_cfg.h>
#include <ui_display.h>
#include <FD612demo_app.h>
#if UI_ENABLE

// weather list
char *weatherList[] = {
	"晴",
	"阴",
	"多云",
	"雷阵雨",
	"雾",
	"阵雨",
	"小雨",
	"中雨",
	"大雨",
	"雪",
	"阵雪",
	"小雪",
	"中雪",
	"大雪",
	"暴雪",
	"雨夹雪",
	"霾",
	"浮尘",
	"扬沙",
	"雨",
};

/* single limit */
static const char singleCode[5] = {
	1,
	3,
	5,
	7,
	9,
};

/* TOP BUFF*/
static unsigned char buffOne[12] = {0};
/* Base buff */
static unsigned char buffTwo[12] = {0};
#define FLASH_BIT 0x80
unsigned int noData = 0;

static inline transform_charNumber_to_hex(const char *data, char *transResult)
{
	uint32 len = strlen(data), loop = 0;
	for (; loop < len; loop++)
	{
		transResult[loop] = data[loop] - '0';
	}
}
/** 
 * @brief  avoid MSB changed by assignment like buffOne = value;
 * @note   use this interface to assign buff value
 * @param  buff: 
 * @param  value: 
 * @retval 
 */
static void inline buff_write_byte(char *buff, char value)
{
	*buff &= 0x80;
	*buff |= value & 0x7f;
}

static unsigned char get_flash_bit()
{
	if (is_half_second())
	{
		return FLASH_BIT;
	}
	else
	{
		return (~FLASH_BIT) & FLASH_BIT;
	}
}

/* ui setup interface ,reserve for furture */
#if (VERSION_MUSHAN || VERSION_DEMO)
typedef void (*pFunc)(char *data);
static pFunc ui_setup_pFunc = NULL;

void pm2_5_data(char *data)
{
#if VERSION_DEMO
	char tempBuff[10] = {
		0,
	};
	unsigned char *pm25 = (unsigned char *)data;
	tempBuff[0] = DispGetCode(pm25[0]);
	tempBuff[1] = DispGetCode(pm25[1]);
	if (wifiPm25.aqi[2] != '\0')
	{
		tempBuff[2] = DispGetCode(pm25[2]);
	}
	else
	{
		tempBuff[2] = 0x00;
	}

	buffTwo[1] = tempBuff[0];
	buffTwo[2] = tempBuff[1];
	buffTwo[3] = tempBuff[2];
#endif

#if VERSION_MUSHAN
	char tempBuff[10] = {
		0,
	};
	unsigned int pm25 = (*(unsigned int *)data);
	tempBuff[0] = DISP_TAB[pm25 / 100];
	tempBuff[1] = DISP_TAB[pm25 / 10 % 10];
	tempBuff[2] = DISP_TAB[pm25 % 100 % 10];

	buff_write_byte(&buffTwo[2], tempBuff[0]);
	buff_write_byte(&buffTwo[1], tempBuff[1]);
	buff_write_byte(&buffTwo[0], tempBuff[2]);
	// buffTwo[2] = tempBuff[0];
	// buffTwo[1] = tempBuff[1];
	// buffTwo[0] = tempBuff[2];
#endif
}
void pm2_5_char_data(char *data)
{
	unsigned int value = (*(unsigned int *)(data));
	// printf("--------------------> PM25 value %d",value);
#if VERSION_MUSHAN
	/* Buff[7]  1100_0000 Buff[6] 1111_1111b*/
	buffTwo[7] |= 0xc0;
	buffTwo[6] |= 0xFF;
	// buffTwo[8] |= 0x40;
	// 	0011_1111B
	buffTwo[5] &= ~0x3f;
	buffTwo[4] &= ~0x3f;
	buffTwo[3] &= ~0x3f;

	if (value >= 0 && value < 35)
	{
		buffTwo[5] |= 0x3F;
	}
	else if (value >= 35 && value < 75)
	{
		// buffTwo[5] |= 0x3F;
		// if (get_flash_bit())
		// {
		// 	buffTwo[5] |= 0x3F;
		// }
		// else
		// {
		// 	// Do not flash chart
		// 	// buffTwo[5] &= (~0x3F);
		// }
		buffTwo[4] |= 0x3F;
	}
	else if (value >= 75 )
	{
		// buffTwo[4] |= 0x3F;
		buffTwo[3] |= 0x3F;
	}
	// else if (value >= 115 && value < 150)
	// {
	// 	buffTwo[4] |= 0x3F;
	// 	if (get_flash_bit())
	// 	{
	// 		buffTwo[4] |= 0x3F;
	// 	}
	// 	else
	// 	{
	// 		// buffTwo[4] &= (~0x3F);
	// 	}
	// }
	// else if (value >= 150 && value < 250)
	// {
	// 	buffTwo[3] |= 0x3F;
	// }
	// else
	// {
	// 	buffTwo[3] |= 0x3F;
	// 	if (get_flash_bit())
	// 	{
	// 		buffTwo[3] |= 0x3F;
	// 	}
	// 	else
	// 	{
	// 		// buffTwo[3] &= (~0x3F);
	// 	}
	// }

#endif
}
void year_data(char *data)
{
	char tempBuff[10] = {
		0,
	};
	unsigned int *year = (unsigned int *)data;
	//				printf("UI_SETUP YEAR DATA %d\n",*year);
	tempBuff[0] = DISP_TAB[*year / 1000];
	tempBuff[1] = DISP_TAB[*year / 10 / 10 % 10];
	tempBuff[2] = DISP_TAB[*year % 100 / 10];
	tempBuff[3] = DISP_TAB[*year % 10];
#if VERSION_DEMO
	buffOne[0] = tempBuff[0];
	buffOne[1] = tempBuff[1];
	buffOne[2] = tempBuff[2];
	buffOne[3] = tempBuff[3];
#endif
#if VERSION_MUSHAN
	buffOne[0] = tempBuff[0];
	buffOne[1] = tempBuff[1];
	buffOne[2] = tempBuff[2];
	buffOne[3] = tempBuff[3];
#endif
}
void month_data(char *data)
{
	char tempBuff[10] = {
		0,
	};
	unsigned int *month = (unsigned int *)data;
	//				printf("UI_SETUP MONTH DATA %d\n",month[0]);
	tempBuff[0] = DISP_TAB[month[0] / 10];
	tempBuff[1] = DISP_TAB[month[0] % 10];
#if VERSION_DEMO
	buffOne[8] = tempBuff[0];
	buffOne[9] = tempBuff[1];
#endif
}
void day_data(char *data)
{
	char tempBuff[10] = {
		0,
	};
	unsigned int *day = (unsigned int *)data;
	//				printf("UI_SETUP DAY DATA %d\n",*day);
	tempBuff[0] = DISP_TAB[*day / 10];
	tempBuff[1] = DISP_TAB[*day % 10];
#if VERSION_DEMO
	buffOne[10] = tempBuff[0];
	buffOne[11] = tempBuff[1];
#endif
}
void week_data(char *data)
{
	char tempBuff[10] = {
		0,
	};
	unsigned int *week = (unsigned int *)data;
	//				printf("UI_SETUP WEEK DATA %d\n",week[0]);
	tempBuff[0] = DISP_TAB[*week % 10];
#if VERSION_DEMO
	buffTwo[11] = tempBuff[0];
#endif
}
void hour_data(char *data)
{
	char tempBuff[2] = {
		0,
	};
	tempBuff[0] = DISP_TAB[*(unsigned int *)data / 10];
	tempBuff[1] = DISP_TAB[*(unsigned int *)data % 10];
#if VERSION_MUSHAN
	/*BASE == A TOP == B*/
	buff_write_byte(&buffTwo[8], tempBuff[0]);
	buff_write_byte(&buffTwo[9], tempBuff[1]);
	// buffTwo[8] = tempBuff[0];
	// buffTwo[9]= tempBuff[1];
#endif
#if VERSION_DEMO
	buffOne[4] = tempBuff[0];
	buffOne[5] = tempBuff[1];
#endif
}
void min_data(char *data)
{
	char tempBuff[10] = {
		0,
	};
	tempBuff[0] = DISP_TAB[*(unsigned int *)data / 10];
	tempBuff[1] = DISP_TAB[*(unsigned int *)data % 10];
#if VERSION_MUSHAN
	buff_write_byte(&buffTwo[10], tempBuff[0]);
	buff_write_byte(&buffTwo[11], tempBuff[1]);
	// buffTwo[10] = tempBuff[0];
	// buffTwo[11] = tempBuff[1];
#endif
#if VERSION_DEMO
	buffOne[6] = tempBuff[0];
	buffOne[7] = tempBuff[1];
#endif
}
#define __FUNC__ __func__
void sunny_data(char *data)
{
	printf("%s \n", __FUNC__);
}
void rainy_data(char *data)
{
	printf("%s \n", __FUNC__);
}
void min_rain_data(char *data)
{
	printf("%s \n", __FUNC__);
}
void heavy_rain_data(char *data)
{
	printf("%s \n", __FUNC__);
}

void car_limit_data(char *data)
{

//	printf("%s \n",__FUNC__);
#if VERSION_MUSHAN
	/* Buff 10 0111_1111b */
	char transformData[2], loop = 0;
	buffOne[10] = 0xFF;
	transform_charNumber_to_hex(data, transformData);
	// Clear chart display
	buffOne[11] &= ~0x70;
	buffOne[11] &= ~0x0E;
	buff_write_byte(&buffOne[6], 0x00);
	buff_write_byte(&buffOne[7], 0x00);

	if (strlen(data) > 2) 
	{

		for (; loop < 5; loop++)
		{
			/* Double number buffOne[11] 0000_1110*/
			buffOne[11] |= 0x70;
			/* Single number buffTwo[11] 0111_0000 */
			if (singleCode[loop] == transformData[0])
			{
				buffOne[11] &= ~0x70;
				buffOne[11] |= 0x0E;
				break;
			}
		}
		buff_write_byte(&buffOne[6], 0x00);
		buff_write_byte(&buffOne[7], 0x00);
	} 
	else if (strlen(data) == 2)
	{
		/* display number buffTwo[6] buffTwo[7] */
		buff_write_byte(&buffOne[6], DISP_TAB[transformData[0]]);
		buff_write_byte(&buffOne[7], DISP_TAB[transformData[1]]);
	}
	// buffOne[6] |= DISP_TAB[transformData[0]];
	// buffOne[7] |= DISP_TAB[transformData[1]];
	printf("--------------->car limit data %s %d %d \n", data, transformData[0], transformData[1]);
#endif
}
void humidity_data(char *data)
{
	char tempBuff[10] = {
		0,
	};

	unsigned char *hum = (unsigned char *)data;
	//printf("humidity is %s",weather.humidity);
	tempBuff[0] = DispGetCode(hum[0]);
	tempBuff[1] = DispGetCode(hum[1]);
#if VERSION_MUSHAN

#endif
#if VERSION_DEMO
	buffTwo[7] = tempBuff[0];
	buffTwo[0] = tempBuff[1];
#endif
}
void online_temprature_data(char *data)
{
	char tempBuff[10] = {
		0,
	};

	unsigned char *temperature = (unsigned char *)data;
	tempBuff[0] = DispGetCode(temperature[0]);
	tempBuff[1] = DispGetCode(temperature[1]);
	tempBuff[2] = DispGetCode(temperature[2]);
#if VERSION_DEMO
	buffTwo[4] = tempBuff[0];
	buffTwo[5] = tempBuff[1];
	buffTwo[6] = tempBuff[2];
#endif
#if VERSION_MUSHAN

#endif
}
void local_temperature_data(char *data)
{
	char tempBuff[10] = {
		0,
	};
#if VERSION_MUSHAN
	// printf("-------------> show string %s \n",data);
	if (strcmp("CCC", data) == 0)
	{
		/* C Buffone[5] */
		buff_write_byte(&buffOne[5], DispGetCode('C'));
	}
	else if (strcmp("FFF", data) == 0)
	{
		buff_write_byte(&buffOne[5], DispGetCode('F'));
	}
	else
	{
		switch (strlen(data)) {
		case 1:
			buff_write_byte(&buffOne[4], DispGetCode(data[0]));
			buff_write_byte(&buffOne[3], 0x00);
			buff_write_byte(&buffOne[2], 0x00);
			break;
		case 2:
			buff_write_byte(&buffOne[4], DispGetCode(data[1]));
			buff_write_byte(&buffOne[3], DispGetCode(data[0]));
			buff_write_byte(&buffOne[2], 0x00);
			break;
		case 3:
			buff_write_byte(&buffOne[4], DispGetCode(data[2]));
			buff_write_byte(&buffOne[3], DispGetCode(data[1]));
			buff_write_byte(&buffOne[2], DispGetCode(data[0]));
			break;
		}
	}
	/* point Buff[11] 0000_0001B RH chart buffOne[2] buffOne[3] 1000_0000b*/
	buffOne[11] |= 0x01;
#endif
#if VERSION_DEMO
	buffTwo[8] = tempBuff[3];
	buffTwo[9] = tempBuff[4];
	buffTwo[10] = tempBuff[5];
#endif
}
void wifi_flag_data(char *data)
{
#if VERSION_MUSHAN
	/*0000_0011B */
	buffTwo[7] |= 0x03;
#endif
	//	printf("%s \n",__FUNC__);
}
void alarm_flag_data(char *data)
{
#if VERSION_MUSHAN
	/* 0000_1100 */
	if (*(unsigned int *)data == ON)
	{
		buffTwo[7] |= 0x0c;
	}
	else
	{
		buffTwo[7] &= 0xF3;
	}
#endif
	//	printf("%s \n",__FUNC__);
}
void display_brightness_data(char *data)
{
	printf("%s \n", __FUNC__);
}
void half_second_flash(char *data)
{
#if VERSION_DEMO
	//				printf("xxxxdata %d",get_flash_bit());
	buffOne[5] |= get_flash_bit();
#endif
#if VERSION_MUSHAN

	/* Buff9 - 0x80 Buff 10 - 0x80 */
	// Do not flash second chart
	buffTwo[9] |= 0x80;
	buffTwo[10] |= 0x80;
	// buffTwo[9] &= ~0x80;
	// buffTwo[10] &= ~0x80;
	// buffTwo[9] |= get_flash_bit();
	// buffTwo[10] |= get_flash_bit();
#endif
}
//char weatherIndex = 0;
void weather_char_data(char *data)
{
#if VERSION_DEMO
	display_weather_notify(buffOne, buffTwo);
#endif
#if (VERSION_MUSHAN == 1)
	char weatherIndex = -1, loop = 0;
	// serial_printf("----------------> %s local %s",data,weatherList[9]);
	//printf("sizeof(weatherList) value :%d",sizeof(weatherList) );
	//printf("sizeof(weatherList) / sizeof(char *) value :%d",sizeof(weatherList) / sizeof(char *));
	for (loop = 0; loop < sizeof(weatherList) / sizeof(char *); loop++)
	{
		if (strcmp(weatherList[loop], data) == 0)
		{
			serial_printf("--------------->weather data %s index %d\n", weatherList[loop], loop);
			weatherIndex = loop;
			break;
		}
	}
	
	// clear chart
	buffOne[4] &= ~0x80;
	buffOne[5] &= ~0x80;
	buffOne[6] &= ~0x80;
	buffOne[7] &= ~0x80;
	buffOne[8] = 0x00;
	buffOne[9] = 0x00;
	buffOne[11] &= ~0x80;
	//weatherIndex ++;
	//if(weatherIndex>=18)weatherIndex = 0;	
	//printf("---------------------------->>>>>>weatherIndex value %d",weatherIndex);
	switch (weatherIndex)
	{
	case 0:
		// qing buffone[4] - 7 (12) buffone[8](1)
		buffOne[4] |= 0x80;
		buffOne[5] |= 0x80;
		buffOne[6] |= 0x80;
		buffOne[7] |= 0x80;

		buffOne[11] |= 0x80;
		buffOne[8] = 0x01;
		break;
	case 1:
		// yin buffOne[8] (5678)
		buffOne[8] = 0xF0;
		break;
	case 2:
		// duo yun
		buffOne[4] |= 0x80;
		buffOne[5] |= 0x80;
		buffOne[6] |= 0x80;
		buffOne[7] |= 0x80;
		buffOne[11] |= 0x80;

		buffOne[8] = 0xF1;
		break;
	case 3:
		// dayu
		buffOne[8] = 0xF0;
		buffOne[9] = 0x07;
	case 4:
	case 5:
	case 6:
		// wu zhenyu xiaoyu
		buffOne[9] = 0x78;
		break;
	case 7:
		// zhongyu
		buffOne[9] = 0xf8;
		break;
	case 8:
		// dayu
		buffOne[8] = 0xF0;
		buffOne[9] = 0x07;
		break;
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		buffOne[8] = 0x0e;
		break;
	case 16:
	case 17:
	case 18:
		buffOne[8] = 0xF0;
		break;
	case 19:
		// zhongyu
		buffOne[9] = 0xf8;
		break;		
	default:
		break;
	}

#endif
}
void display_off_data(char *data)
{
#if VERSION_MUSHAN
	buffTwo[*(unsigned int *)data] = 0x00;
#endif
}
void local_humidity_data(char *data)
{
#if VERSION_MUSHAN
	buffOne[0] = DISP_TAB[*(unsigned int *)data / 10];
	buffOne[1] = DISP_TAB[*(unsigned int *)data % 10];
	/* chart display */
	// buffOne[0] buffOne[1] 1000_0000B
	buffOne[0] |= 0x80;
	buffOne[1] |= 0x80;
	buffOne[2] |= 0x80;
	buffOne[3] |= 0x80;
#endif
}

static const ui_setup_function_t ui_setup_ops = {
	.pm2_5_data = pm2_5_data,
	.pm2_5_char_data = pm2_5_char_data,
	.year_data = year_data,
	.month_data = month_data,
	.day_data = day_data,
	.week_data = week_data,
	.hour_data = hour_data,
	.min_data = min_data,
	.sunny_data = sunny_data,
	.rainy_data = rainy_data,
	.min_rain_data = min_rain_data,
	.heavy_rain_data = heavy_rain_data,
	.car_limit_data = car_limit_data,
	.humidity_data = humidity_data,
	.online_temprature_data = online_temprature_data,
	.local_temperature_data = local_temperature_data,
	.local_humidity_data = local_humidity_data,
	.wifi_flag_data = wifi_flag_data,
	.alarm_flag_data = alarm_flag_data,
	.display_brightness_data = display_brightness_data,
	.half_second_flash = half_second_flash,
	.weather_char_data = weather_char_data,
	.display_off = display_off_data,
};

/*TODO:
*add mutexlock for avoid cocurrence
*/
void ui_setup(void *data, DISPLAY_TYPE type)
{
	char tempBuff[10] = {
		0,
	};
	unsigned char loop = 0;
	if (data == NULL)
	{
		printf("UI_SETUP INPUT NULL DATA!!! \n");
		return;
	}
	/* call ui setup function 
	*NOTICE: add new api must be by according to type
	*/
	/* Get ops address */
	unsigned long *pFuncAddr = (unsigned long *)(&ui_setup_ops);
	/* Get function address */
	ui_setup_pFunc = (pFunc)pFuncAddr[type];
	/* Call the function */
	if (ui_setup_pFunc != NULL)
	{
		ui_setup_pFunc(data);
	}
}
static inline void display_mode()
{
	display_init_mode();
}

void ui_display()
{
	display_mode();
	display_data_toFD612(TOP, buffOne);
	display_data_toFD612(BASE, buffTwo);
}
void ui_display_init()
{
#if FD612_ENABLE
	FD612_app_init();
#endif
}
#endif /* VERSION_MUSHAN || VERSION_DEMO */
#endif
