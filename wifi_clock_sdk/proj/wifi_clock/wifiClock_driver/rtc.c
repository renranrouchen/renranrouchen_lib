/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf@gmail.com
*@Brief			
*@FileName		rtc.c
*@Data			2017/7/14 8:44:09
-------------------------------------------------------------------------*/
#include <common.h>

#include <rtc.h>
#include <wifi_time.h>
#include "ntp/ntpd.h"	
#include <gpio.h>

#undef uint8
#define uint8 unsigned char

#undef uint16
#define  uint16 unsigned short

#undef uint32
#define uint32 unsigned int


#define SCL_PIN 2
#define DIO_PIN	3
#define RST_PIN 4

#define RTC_SCL(value)		digital_write(SCL_PIN,value)
#define DIO_OUT(value)  	digital_write(DIO_PIN,value)
#define DIO_OUT_MODE		pin_mode(DIO_PIN,GPIO_OUT)
#define DIO_IN_MODE			pin_mode(DIO_PIN,GPIO_IN)
#define DIO_IN				digital_read(DIO_PIN)
#define RTC_RST(value) 		digital_write(RST_PIN,value)

#define DELAY_1US	udelay(1)

#define WRITE_RAM_ADDR_START	0xc0
#define WRITE_RAM_ADDR_END		0xfc
#define READ_RAM_ADDR_START		0xc1
#define READ_RAM_ADDR_END    	0xfd

#define BURST_WRITE_REG_CMD		0xbe
#define BURST_READ_REG_CMD 		0xbf
#define BURST_WRITE_ARM_CMD		0xFE
#define BURST_READ_RAM_CMD 		0xFF
#define ENABLE_WRITE	0x00

#define second1 0x00    
#define minute1 0x20		
#define hour1   0x16    
#define day1    0x1		
#define month1  0x02		
#define date1  	0x04		
#define year1   0x18		
t_rtc rtc;
enum {
	WRITE_SEC_REG = 0x80,
	READ_SEC_REG,
	WRITE_MIN_REG,
	READ_MIN_REG,
	WRITE_HOUR_REG,
	READ_HOUR_REG,
	WRITE_DAY_REG ,
	READ_DAY_REG,
	WRITE_MON_REG,
	READ_MON_REG,
	WRITE_WEAK_REG,
	READ_WEAK_REG,
	WRITE_YEAR_REG ,
	READ_YEAR_REG,
	WRITE_CONTROL_REG = 0x8e,
	RAM_READ_REG = 0xc1,
};

void reset_rtc()
{
	pin_mode(SCL_PIN,GPIO_OUT);	
	pin_mode(DIO_PIN,GPIO_OUT);	
	pin_mode(RST_PIN,GPIO_OUT);	

	//PULL UP resister
	//gpio_pullup(SCL_PIN,PULL_UP);
	//gpio_pullup(DIO_PIN,PULL_UP);
	//gpio_pullup(RST_PIN,PULL_UP);

	gpio_enable(SCL_PIN,ENABLE);
	gpio_enable(DIO_PIN,ENABLE);
	gpio_enable(RST_PIN,ENABLE);
	
}

unsigned char rtc_input_byte()
{
	uint8 loop = 0;
	uint8 readByte = 0;
	uint8 tempByte = 0;
	readByte = 0x00;
	DIO_OUT_MODE;
	DIO_OUT(HIGH);
	DELAY_1US;
	DIO_IN_MODE;
	DELAY_1US;

	for(loop = 0; loop < 8;loop++)
	{
		RTC_SCL(HIGH);
		DELAY_1US;
		RTC_SCL(LOW);
		DELAY_1US;
		tempByte = DIO_IN;
		tempByte <<= 7;
		readByte >>= 1;
		readByte |= tempByte;
	}
	return readByte;
}

void rtc_output_byte(unsigned char data)
{
	uint8 loop = 0 ;	
	DIO_OUT_MODE;
	DELAY_1US;
	
	for (loop = 0;loop < 8;loop++)
	{
		RTC_SCL(LOW);
		if((data >> loop) & 0x01)
			DIO_OUT(HIGH);
		else
			DIO_OUT(LOW);
		DELAY_1US;
		RTC_SCL(HIGH);
		DELAY_1US;
	}
}

void write_rtc_register(unsigned char regAddr,unsigned char data)
{
	RTC_RST(LOW);
	RTC_SCL(LOW);
	DELAY_1US;
	RTC_RST(HIGH);

	rtc_output_byte(regAddr);
	rtc_output_byte(data);

	RTC_RST(LOW);
}

char read_rtc_register(char regAddr)
{
	char readByte;
	RTC_RST(LOW);
	RTC_SCL(LOW);
	DELAY_1US;
	RTC_RST(HIGH);
	
	rtc_output_byte(regAddr);
	readByte = rtc_input_byte();

	RTC_RST(LOW);
	return readByte;
}

static inline unsigned char hex_to_dec(unsigned char dat)
{
	return ((dat >> 4) * 10 + (dat & 0x0F));
}

void read_rtc_time()
{
#if 1	
	RTC_SCL(LOW);
	RTC_RST(LOW);
	RTC_RST(HIGH);
	DIO_OUT(LOW);
	DELAY_1US;
	
	rtc_output_byte(BURST_READ_REG_CMD);
	rtc.time.second = hex_to_dec(rtc_input_byte());
	rtc.time.min = hex_to_dec(rtc_input_byte());
	rtc.time.hour = hex_to_dec(rtc_input_byte());
	rtc.time.day = hex_to_dec(rtc_input_byte());
	rtc.time.month = hex_to_dec(rtc_input_byte());
	rtc.time.week = rtc_input_byte();
	rtc.time.year = hex_to_dec(rtc_input_byte());
	RTC_RST(LOW);
#endif

}

unsigned char ntp_to_rtc(unsigned char dat)
{
	return ((dat / 10 << 4) + (dat % 10));
}

void update_rtc_time(void* time)
{
	sntp_tm* ntpTime = time;
	write_rtc_register(WRITE_CONTROL_REG,ENABLE_WRITE);	
#if 1
	RTC_RST(HIGH);
	RTC_SCL(LOW);
	DIO_OUT(LOW);
	DELAY_1US;

	rtc_output_byte(BURST_WRITE_REG_CMD);						
	rtc_output_byte(ntp_to_rtc(ntpTime->tm_sec));
	rtc_output_byte(ntp_to_rtc(ntpTime->tm_min));
	rtc_output_byte(ntp_to_rtc(ntpTime->tm_hour));
	rtc_output_byte(ntp_to_rtc(ntpTime->tm_mday));
	rtc_output_byte(ntp_to_rtc(ntpTime->tm_mon + 1));
	rtc_output_byte(ntp_to_rtc(ntpTime->tm_wday));
	rtc_output_byte(ntp_to_rtc(ntpTime->tm_year % 100));

	RTC_RST(LOW);
#endif
	write_rtc_register(WRITE_CONTROL_REG,0x80);			
}

char rtc_time_config(void)
{
	char ramData = 0;
	ramData = read_rtc_register(READ_RAM_ADDR_START);	
	printf(" \r\n ramData %d",ramData);
	if (ramData != 0xaa) {
		printf(" \r\n starting write rtc time \r\n ");
		write_rtc_register(WRITE_CONTROL_REG,ENABLE_WRITE);	
		write_rtc_register(WRITE_RAM_ADDR_START,0xaa);
		// write_rtc_register(WRITE_SEC_REG,0x00);
		// write_rtc_register(WRITE_HOUR_REG,0x00);
		ramData = read_rtc_register(READ_RAM_ADDR_START);	
		printf(" \r\n ramData %d",ramData);
#if 1	
		RTC_SCL(LOW);
		RTC_RST(LOW);
		RTC_RST(HIGH);
		DIO_OUT(LOW);
		DELAY_1US;	
		rtc_output_byte(BURST_WRITE_REG_CMD);						
		rtc_output_byte(second1);
		rtc_output_byte(minute1);
		rtc_output_byte(hour1);
		rtc_output_byte(day1);
		rtc_output_byte(month1);
		rtc_output_byte(date1);
		rtc_output_byte(year1);
		RTC_RST(LOW);
#endif		
		write_rtc_register(WRITE_CONTROL_REG,0x80);		
	}
}

/* init rtc device */
void start_rtc()
{
	reset_rtc();
	//write_rtc_register(0x90,0xab);
	rtc_time_config();
}





