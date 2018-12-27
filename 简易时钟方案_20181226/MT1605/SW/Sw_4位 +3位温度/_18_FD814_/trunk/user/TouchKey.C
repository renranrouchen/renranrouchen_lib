
/********************************** (C) COPYRIGHT *******************************
* File Name          : TouchKey.C
* Author             : FD814
* Version            : V1.0
* Date               : 2017/01/20
* Description        : FD814 触摸按键采样间隔设置、通道选择和切换和中断处理函数   
*******************************************************************************/

#include "FD814.H"                                                       
#include "TouchKey.H"
#include "stdio.h"

u8 WaveBufe[6];

/*******************************************************************************
* Function Name  : TouchKeyChannelQuery()
* Description    : 触摸按键通道状态查询
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TouchKeyChannelQuery()
{
	UINT8	ch;
    UINT16 KeyData;

    while((TKEY_CTRL&bTKC_IF) == 0);                                          //bTKC_IF变为1时，本周期采样完成
    KeyData = TKEY_DAT;                                                       //保持87us,尽快取走
    ch = TKEY_CTRL&7;                                                         //获取当前采样通道
	// printf("ch = %0bu, KeyData = %0u\n", ch, KeyData);
	WaveBufe[ch - 1] = (u8) (KeyData / 64);
    if ( ch > 5 ){
       TKEY_CTRL = TKEY_CTRL & 0xF8 | 0x01;                              //从首通道开始采样
    }			
    else
    {
       TKEY_CTRL ++;                                                          //切换至下一个采样通道
    }
}
