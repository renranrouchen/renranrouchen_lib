/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf@gmail.com
*@Brief
*@FileName		rtc.c
*@Data			2017/7/14 8:44:09
-------------------------------------------------------------------------*/
#include <sdk_cfg.h>
#include <common/common.h>
#include <ds1302.h>
#if (RTC_DS1302 == 1)
enum {
	LOW = 0,
	HIGH = 1,
};
#define  SCL_PIN   7
#define  DIO_PIN   6
#define  RST_PIN   0
#define	 SCL	   (BIT(SCL_PIN) | BIT(2))
#define	 DIO 	   (BIT(DIO_PIN) | BIT(1))
#define	 RST	   (BIT(RST_PIN) | BIT(5))
#define  DS_PORT_OUT     JL_PORTB->OUT
#define  DS_PORT_IN      JL_PORTB->IN
#define  DS_PORT_DIR     JL_PORTB->DIR
#define  DS_PORT_PD      JL_PORTB->PD
#define  DS_PORT_PU      JL_PORTB->PU

#define RTC_SCL_OUT_MODE	(DS_PORT_DIR &= ~SCL)
#define RTC_SCL(value)		do {	\
	if (value == 1) {				\
		DS_PORT_OUT |= SCL;		\
	} else	{						\
		DS_PORT_OUT &= ~SCL;		\
	}								\
}while(0)
#define DIO_OUT(value) do {			\
	if (value == 1) {				\
		DS_PORT_OUT |= DIO;			\	
	} else	{						\
		DS_PORT_OUT &= ~DIO;		\
	}								\
}while(0)
#define DIO_OUT_MODE		(DS_PORT_DIR &= ~DIO)
#define DIO_IN_MODE			(DS_PORT_DIR |= DIO)
#define DIO_IN				(DS_PORT_IN & DIO)
#define RTC_RST_OUT_MODE	(DS_PORT_DIR &= ~RST)
#define RTC_RST(value) 		do {	\
	if (value == 1) {				\
		DS_PORT_OUT |= RST;			\
	} else	{						\
		DS_PORT_OUT &= ~RST;		\
	}								\
}while(0)

#define DELAY_1US	delay(240)

#define WRITE_RAM_ADDR_START	0xc0
#define WRITE_RAM_ADDR_END		0xfc
#define READ_RAM_ADDR_START		0xc1
#define READ_RAM_ADDR_END    	0xfd

#define BURST_WRITE_REG_CMD		0xbe
#define BURST_READ_REG_CMD 		0xbf
#define BURST_WRITE_ARM_CMD		0xFE
#define BURST_READ_RAM_CMD 		0xFF
#define ENABLE_WRITE	0x00

#define second1 0x11
#define minute1 0x15
#define hour1   0x0B
#define day1    0x15
#define month1  0x0B
#define date1  	0x02
#define year1   0x11
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
	RTC_SCL_OUT_MODE;
	RTC_RST_OUT_MODE;
}

unsigned char rtc_input_byte()
{
	u8 loop = 0;
	u8 readByte = 0;
	u8 tempByte = 0;
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
		if (DIO_IN) {
			tempByte = 1;
		} else {
			tempByte = 0;
		}
		tempByte <<= 7;
		readByte >>= 1;
		readByte |= tempByte;
	}
	return readByte;
}

void rtc_output_byte(unsigned char data)
{
	u8 loop = 0 ;
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

void read_rtc_time(rtc_time_t* rtc)
{
#if 1
	RTC_SCL(LOW);
	RTC_RST(LOW);
	RTC_RST(HIGH);
	DIO_OUT(LOW);
	DELAY_1US;

	rtc_output_byte(BURST_READ_REG_CMD);
	rtc->second = hex_to_dec(rtc_input_byte());
	rtc->min = hex_to_dec(rtc_input_byte());
	rtc->hour = hex_to_dec(rtc_input_byte());
	rtc->day = hex_to_dec(rtc_input_byte());
	rtc->month = hex_to_dec(rtc_input_byte());
	rtc->week = rtc_input_byte();
	rtc->year = hex_to_dec(rtc_input_byte());
	RTC_RST(LOW);
#endif

}

static unsigned char ntp_to_rtc(unsigned char dat)
{
	return ((dat / 10 << 4) + (dat % 10));
}

void update_rtc_time(rtc_time_t* time)
{
	write_rtc_register(WRITE_CONTROL_REG,ENABLE_WRITE);
#if 1
	RTC_RST(HIGH);
	RTC_SCL(LOW);
	DIO_OUT(LOW);
	DELAY_1US;

	rtc_output_byte(BURST_WRITE_REG_CMD);
	rtc_output_byte(ntp_to_rtc(time->second));
	rtc_output_byte(ntp_to_rtc(time->min));
	rtc_output_byte(ntp_to_rtc(time->hour));
	rtc_output_byte(ntp_to_rtc(time->day));
	rtc_output_byte(ntp_to_rtc(time->month));
	rtc_output_byte(ntp_to_rtc(time->week));
	rtc_output_byte(ntp_to_rtc(time->year % 100));
	RTC_RST(LOW);
#endif
	write_rtc_register(WRITE_CONTROL_REG,0x80);
}

void rtc_time_config(void)
{
	unsigned char ramData = 0;
	ramData = read_rtc_register(READ_RAM_ADDR_START);
	printf(" \r\n ramData %d",ramData);
	if(ramData != 0xaa) {
		printf(" \r\n starting write rtc time \r\n ");
		write_rtc_register(WRITE_CONTROL_REG,ENABLE_WRITE);
		write_rtc_register(WRITE_RAM_ADDR_START,0xaa);

//		write_rtc_register(WRITE_SEC_REG,second1);
//		write_rtc_register(WRITE_HOUR_REG,minute1);
//		write_rtc_register(WRITE_DAY_REG,hour1);
//		write_rtc_register(WRITE_MON_REG,day1);
//		write_rtc_register(WRITE_WEAK_REG,month1);
//		write_rtc_register(WRITE_YEAR_REG,date1);
//		write_rtc_register(WRITE_YEAR_REG,year1);
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

#endif



