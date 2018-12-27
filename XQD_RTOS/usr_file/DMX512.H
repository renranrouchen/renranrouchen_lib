/***************************************************
* @Copyright(c) 2016-2100.  hwl.  All rights reserved.
* @FileName: config.c
* @Author:   hwl 
* @Description:   DMX512 Driver header file
* @Version:
* @History:         
      <author>  <time>      <version >   <desc>
				hwl      18/9/7       1.0     
****************************************************/
#include "event_process.h"

#ifndef _DMX512_H_
#define	_DMX512_H_

#define LENGTH (50*3)
#define NUMBER (50)
#define SM165xx_check_code 0xD2
#define SM1751_API_CONFIG 1
#define SM165XX_API 1
#define SM1752XX_API 1
#define SM17500_API_CONFIG 1
#define SM16804P_write_data2_En 1
#define FD6603_write_data_en 0

					/*Ó²¼þÏà¹Ø*/
//typedef struct DMX512_func
//{
//	void (*pm2_5_data)(char* data);
//}DMX512_func_t;

typedef struct DMX512_arg
{
	uchar AUTO;
	uchar REDIS;
	uchar CH1_CH0;
	uchar REDIS_CH1_CH0;
	uchar RGBW_R;
	uchar RGBW_G;
	uchar RGBW_B;
	uchar RGBW_W;
	uchar RGBW_IR;
	uchar RGBW_IG;
	uchar RGBW_IG_R;
	uchar RGBW_IB;
	uchar RGBW_IW;
	uchar RGBW_IW_B;	
	uchar AUTO_STEP;
	
	uchar IC_select;
	uchar RTZ_select;
	uchar EN0_EN1;
	uchar SELF_CHAN_LD;
}DMX512_arg_t;

typedef struct SM165XX_arg
{
	uchar Power_Up_Color;
	
}SM165XX_arg_t;

typedef struct RZ_arg
{
	uchar RGBW_R;
	uchar RGBW_G;	
	uchar RGBW_B;
	uchar RGBW_W;
	uchar RGBW_IR;
	uchar RGBW_IG;
	uchar RGBW_IB;
	uchar RGBW_IW;
	uchar RGBW_IR_G;
	uchar RGBW_IB_W;
	uchar Chip_Num;
}RZ_arg_t;


void DMX_SendPacket(char *data,unsigned int length) ;
void Run_manually_point_fuc(uint16_t num);
void DMX_DataDecode(chip_param_t* chip_arg);
void DMX_Send(void);
void DMX_Send_data(unsigned char red,unsigned char green,unsigned char blue,chip_param_t* chip_arg);
void sm1751x_Write_addr(unsigned int addr_start,unsigned char CH,unsigned int n);
void IO_send_data(unsigned char data);
void send_add(unsigned int addr_start,unsigned char CH,unsigned int n);
void DMX512_Clear_init(void);
void arg_switch_sm1751x(DMX512_arg_t *DMX512_arg);
void sm1751x_write_arg(DMX512_arg_t *DMX512_arg);
void test_arg(void);
void delay_us(unsigned short nTimer);
void SM165XX_Send_addr(uint Start_addr,uint channel_num,uchar ch_step,uchar chk,uchar effect);
void sm1752x_send_add(unsigned int addr_start,unsigned char CH,unsigned int n);
void arg_switch_sm17500x(DMX512_arg_t *DMX512_arg);
void sm17500x_send_add(unsigned int addr_start,unsigned char CH,unsigned int n);
void sm17500x_write_arg(DMX512_arg_t *DMX512_arg);
void sm17500x_write_elect_arg(DMX512_arg_t *DMX512_arg);
void sm17500x_send_self_add(unsigned int addr_start,unsigned int n);
void sm17500x_send_self_channel(unsigned int n,DMX512_arg_t *DMX512_arg);

void SM16804P_write_data(RZ_arg_t* RZ_arg);
void SM16703P_Display(uchar RED,uchar GREEN,uchar BLUE,RZ_arg_t* RZ_arg);
void SM16804P_write_data2(RZ_arg_t* RZ_arg);
void SM16804P_Display(uchar RED,uchar GREEN,uchar BLUE,RZ_arg_t* RZ_arg);

void Protocol_output_Send_data(unsigned char red,unsigned char green,unsigned char blue,unsigned char write,unsigned char yellow,Protocol_output_parameter_t* Protocol_output_param);
#endif
