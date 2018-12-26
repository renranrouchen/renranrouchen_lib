#ifndef _ADC_C_
#define _ADC_C_

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
#include "../../Library/includes/adc.h"
#include <intrins.h>
/*********************************************************************************************************************/
/***********************************************************************************
������:		ADC_init										 	
������������ʼ��ADC����
���������adc_ref 	ADC�ο���ѹ����
					adc_int		ADC�ж�����
					amp_sw		�Ƿ����˷Ź���
					gain			�˷�����
����ֵ��  ��
***********************************************************************************/
void ADC_init(ADC_RefVolt_TypeDef adc_ref,InterruptSW_TypeDef adc_int,AmpSW_TypeDef amp_sw,AMP_Gain_TypeDef gain)							
{
	ADCON = AST(0) | ADIE(adc_int) | HTME(7) | VSEL(adc_ref);	
	ADCFGL = ACKD(7) | ADCALE(1) | ADCHS(0);	
#if (IC_TYPE == IC_TYPE_CA51F2X)
	ADCFGH = AOVE(0) | VTRIM(35);
	if(amp_sw == ENABLE)
	{
		OPACON = OPAM(2) | OPAS(gain);			
	}
#elif (IC_TYPE == IC_TYPE_CA51F3X)
	if(amp_sw == ENABLE)
	{
		ADCFGH = AOPS(gain) | VTRIM(35);
	}
	else
	{
		ADCFGH = AOPS(0) | VTRIM(35);
	}
#endif
	if(adc_int == ENABLE)
	{
		INT2EN = 1; 
	}
}
/***********************************************************************************
������:		ADC_ConfigChannel										 	
��������������ADC����ͨ��
���������channel 	ADCͨ��
����ֵ��  ��
***********************************************************************************/
void ADC_ConfigChannel(ADC_Channel_TypeDef channel)
{
	ADCFGL = (ADCFGL&0xF0) | ADCHS(channel);
}
/***********************************************************************************
������:		ADC_StartConversion										 	
��������������ADCת��
�����������
����ֵ��  ��
***********************************************************************************/
void ADC_StartConversion(void)
{
  ADCON |= AST(1);
}

/***********************************************************************************
������:		GetAdcValue										 	
������������ȡADCת����ֵ
�����������
����ֵ��  ADCֵ
***********************************************************************************/
unsigned int GetAdcValue(void)
{
	unsigned int ADC_Value;
	while(!(ADCON & ADIF));
	ADCON |= ADIF;
	ADC_Value = ADCDH*256 + ADCDL;
	ADC_Value >>= 4;
	return ADC_Value;
}

/*********************************************************************************************************************/
#endif