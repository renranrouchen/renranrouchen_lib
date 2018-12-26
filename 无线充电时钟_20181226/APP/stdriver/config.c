#include "config.h"
#include "delay.h"
#include "usr.h"
extern UINT8 hour_12_24_flag,hour_pm_flag;
void gpio_init(void)
{
	GPIO_Init(P12F,OUTPUT|PU_EN);  //FD612��������
	GPIO_Init(P13F,OUTPUT|PU_EN);
	GPIO_Init(P14F,OUTPUT|PU_EN); //fd616
	GPIO_Init(P15F,OUTPUT|PU_EN);
	
	GPIO_Init(P37F,OUTPUT);//ds1302
//	GPIO_Init(P01F,INPUT);
	GPIO_Init(P35F,OUTPUT);
	
	GPIO_Init(P06F,INPUT|PU_EN);//key
	GPIO_Init(P07F,INPUT|PU_EN);
	GPIO_Init(P34F,INPUT|PU_EN);
	GPIO_Init(P05F,INPUT|PU_EN);
	
	GPIO_Init(P03F,INPUT|PD_EN);//voice
	
	GPIO_Init(P04F,INPUT|PU_EN);//vibration_key �͵�ƽ��Ч
	
	GPIO_Init(P16F,OUTPUT|PU_EN); //buzzer
	  buzzer = 0;
	
}


void CfgFsys(void)
{

//	BS(CKCON,7); //IRCLʹ��
	BS(CKSEL,7);//RTCʱ��Դѡ��
//	Delay_50us(1);
}


/***********************************************************************************
��������		RTC_init
����˵���� ��ʼ��RTC���ƼĴ���
��������� rtce		RTC����ģ��ʹ�ܿ���
						mse		RTC�����ж�ʹ�ܿ���
						hse		RTC�����ж�ʹ�ܿ���
����ֵ��	��
***********************************************************************************/																												
void RTC_init(void)
{		 
	
	BS(RTCON,6); //RTC�����ж�ʹ��
	BS(RTCON,5); //RTC�����ж�ʹ��
	BS(RTCON,7); //RTCʱ��ʹ��
	
	BS(EXIE,6);
	EA=1;
}

void adc_init(void)
{		
	GPIO_Init(P10F,P10_ADC0_SETTING);
	ADCON = AST(0) | ADIE(0) | HTME(7) | VSEL(1);
	ADCFGL = ACKD(0) | ADCALE(1) ;
	ADCFGL = (ADCFGL&0xF0) | ADCHS(1);
	ADCON &= ~(1<<5);
//	ADCON |= AST(1); 
//	INT2EN = 1; 
}


void DS1302_INIT(void)
{
		UINT8 ds1320_state=0,rom_value;
			ds1302_w_on();
			ResetDS1302();
			WriteByteDS1302(0x90);													//д��������ƼĴ���
			WriteByteDS1302(0xab);
			ds1302_w_off();
	  ResetDS1302();							
		WriteByteDS1302(rom1_r_addr); //��ȡʱ�ĵ�ַ
		rom_value=ReadByteDS1302();
		if(rom_value!=0xaa)
		{
			InitDS1302( );
			ds1302_w_on();
			ResetDS1302();
			WriteByteDS1302(rom1_w_addr);		                      
			WriteByteDS1302(0xaa);	
			ds1302_w_off();
			
		}
	
//		unsigned char year,month,mday,week,hour,minute,second ;					
		ResetDS1302();							
		WriteByteDS1302(hour_r_addr); //��ȡʱ�ĵ�ַ
		ds1320_state=ReadByteDS1302();
		if(ds1320_state&0x80)
		{
			hour_12_24_flag=1;
		}
		if(hour_12_24_flag==1)
		{
			if(ds1320_state&0x20)
			hour_pm_flag=1;
		}

		

		
}
void display_init_first(void)
{
	UINT8 rom2_value;
	  ResetDS1302();							
		WriteByteDS1302(rom2_r_addr); //��ȡʱ�ĵ�ַ
		rom2_value=ReadByteDS1302();
		if(rom2_value!=0xaa)
		{
			display_init( );			
			ds1302_w_on();
			ResetDS1302();
			WriteByteDS1302(rom2_w_addr);		                      
			WriteByteDS1302(0xaa);	
			ds1302_w_off();
			
		}	
}

void Timer0_init(void)
{
	TMOD=(TMOD&0xFC)|0x01;
		TH0 =0XFF;
		TL0 =0Xba;
//	TR0 = 1;
	ET0 = 1;	
}