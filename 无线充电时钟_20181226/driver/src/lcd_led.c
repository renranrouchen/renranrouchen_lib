#ifndef _LCD_C_
#define _LCD_C_
/*********************************************************************************************************************/
#include "ca51f_config.h"
#if (IC_TYPE == IC_TYPE_CA51F2X)
#include "../../includes/ca51f2sfr.h"
#include "../../includes/ca51f2xsfr.h"
#include "../../includes/gpiodef_f2.h"
#elif (IC_TYPE == IC_TYPE_CA51F3X)
#include "../../includes/ca51f3sfr.h"
#include "../../includes/ca51f3xsfr.h"
#include "../../includes/gpiodef_f3.h"
#endif
#include "../../includes/system.h"
#include "../../Library/includes/lcd_led.h"
#include <intrins.h>
/*********************************************************************************************************************/
/***********************************************************************************
��������LCD_init
����˵����LCD�Ĵ�����ʼ��
��������� len		LCDʱ��Դѡ��
						dmod	��������ѡ��
						bias	ƫѹ����
						ldrv	������ѹ�ȼ�����
						lxdiv LCDʱ�ӷ�Ƶϵ��
����ֵ��	��						
***********************************************************************************/
void LCD_init(LEN_TypeDef len,DMOD_TypeDef dmod,BIAS_TypeDef bias,LDRV_TypeDef ldrv,unsigned int lxdiv)
{
	LXDIVH = (unsigned char)(lxdiv>>8);
	LXDIVL = (unsigned char)(lxdiv);	
	LXCFG =	 DMOD(dmod) | BIAS(bias) | LDRV(ldrv);
	LXCON =  LEN(len) | LMOD(LMOD_lcd);	 
}
/***********************************************************************************
��������LED_init
����˵����LED�Ĵ�����ʼ��
��������� len			LCDʱ��Դѡ��
						comhv	COM��Ч��ƽ����
						seghv	SEG��Ч��ƽ����
						lxdiv 	LEDʱ�ӷ�Ƶϵ��
����ֵ��	��						
***********************************************************************************/
void LED_init(LEN_TypeDef len,COMHV_TypeDef comhv,SEGHV_TypeDef seghv,LDRV_TypeDef ldrv,unsigned int lxdiv)
{
	LXDIVH = (unsigned char)(lxdiv>>8);
	LXDIVL = (unsigned char)(lxdiv);	
	LXCFG =	 COMHV(comhv) | SEGHV(seghv) | BLNK(0)|LDRV(ldrv);
	LXCON =  LEN(len) | LMOD(LMOD_led);	 
}
/***********************************************************************************
��������LCD_LED_WriteLram
����˵����д�����ݵ���ʾ����
��������� 	laddr	��ʾ�����ַ
						ldata	��ʾ��������
����ֵ��	��						
***********************************************************************************/
void LCD_LED_WriteLram(unsigned char laddr, unsigned char ldata)
{
	INDEX = laddr;
	LXDAT = ldata;
}

/***********************************************************************************
��������LCD_LED_ReadLram
����˵��������ʾ�����ȡ����
��������� 	laddr		��ʾ�����ַ
����ֵ��	��ʾ��������			
***********************************************************************************/
unsigned char LCD_LED_ReadLram(unsigned char laddr)
{
	INDEX = laddr;
	return LXDAT;
}
/*********************************************************************************************************************/
#endif