/***************************************************
* @Copyright(c) 2016-2100.  hwl.  All rights reserved.
* @FileName: DMX512.c
* @Author:   hwl 
* @Description:  DMX512 Driver  function interface
* @Version:
* @History:         
      <author>  <time>      <version >   <desc>
				hwl      18/9/7       1.0     
****************************************************/

#include "M451Series.h"
#include "DMX512.H"
#include "event_process.h"

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#define IO_TXD PC3
#define IO_Start_TIME   delay_nus(3);
#define IO_Bit_TIME     delay_nus(3);
#define IO_Stop_TIME     delay_nus(6);

#define DMX512_BREAK_TIME   delay_us(90);//Delay100us();//延时88us
#define DMX512_MAB_TIME     delay_us(6);// delay_nus(4); //延时8us
#define DMX512_STOP_TIME    delay_us(10);//Delay10us(); //延时10us
#define DMX512_BIT_TIME		  delay_us(8);//Delay4us()	;

#define DMX512_ADDR_BREAK_TIME  delay_ms(1800);//delay_nms(1650);//延时1.8s
#define DMX512_ADDR_BREAK_TIME_1_5  delay_ms(1500);//delay_nms(1650);//延时1.5s
#define DMX512_ADDR_MAB_TIME    delay_us(8);//delay_nus(8);

#define DMX512_ADDR PC2
#define DMX512_DATA PC2
#define RZ_DATA PC2

#define NS_300		 __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();//delay_ns(6);
#define NS_900		 delay_ns(18);
#define US_80      delay_us(80);
  

#define Delay_Us_8    delay_us(8);
#define Delay_Us_139  delay_us(139);
#define Delay_Us_32   delay_us(32);
#define Delay_Us_115  delay_us(115);

#define sm1751x_addr_arg_write 1
//#define DMX512_DATA_OUT P3M1=;P3M0
DMX512_arg_t DMX512_arg_switch_data;
char CLear_data[2048]={0};
char Dmx512_write_data[2048]={0};

/***************************************************** 
 * @Function:  delay_nus
 * @Description:   rough delay us
 * @param  time 
 * @retval void
******************************************************/



void delay_us(unsigned short nTimer)
{
	unsigned short i=0;
	for(i=0;i<nTimer;i++){
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	}
	
}

void delay_ms(unsigned short time)
{ 
	while(time--)
	{
		delay_us(1000);
	}
}
void delay_ns(unsigned int nTimer)
{
	while(nTimer--);
 
};



//char TXDData[LENGTH]; //接收缓冲区200个数据
//unsigned char TXDADDR[LENGTH];
//uint8 RXDData[LENGTH]; //接收缓冲区200个数据


/***************************************************** 
 * @Function:  DMX_DataDecode
 * @Description:   DMX512 Date change to array
 * @param  red
 * @param  green 
 * @param  blue 
 * @retval void
******************************************************/
void DMX_DataDecode(chip_param_t* chip_arg)
{
	unsigned int i;
	/*512、1024、2048次*/
	for(i = 0; i<((chip_arg->chip_chanle_num+2)/3); i++)
  {
     Dmx512_write_data[i*3+0] = chip_arg->red_data;
		 Dmx512_write_data[i*3+1] = chip_arg->green_data;
		 Dmx512_write_data[i*3+2] = chip_arg->blue_data;
		//printf("lengthghfh----------->>\r\n");
  }
}


void Run_manually_point_fuc(uint16_t num)
{
	uint16_t i;
	/*512、1024、2048次*/
	for(i = 0; i<512; i++)
  {
     Dmx512_write_data[i] = 0;
		//printf("lengthghfh----------->>\r\n");
  }	
	for(i = 0; i<num; i++)
  {
     Dmx512_write_data[i] = 0xff;
		//printf("lengthghfh----------->>\r\n");
  }	
	DMX_SendPacket(Dmx512_write_data,512);	
}
/***************************************************** 
 * @Function:  DMX_SendPacket
 * @Description:   DMX512 send Packet
 * @param  data
 * @param  length 
 * @retval void
******************************************************/
void DMX_SendPacket(char *data,unsigned int length) //发送DMX512数据
{
	taskENTER_CRITICAL();
	unsigned int i = 0;
//	unsigned int j = 0;
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_GPIO);
	DMX512_DATA = 0 ;
	DMX512_BREAK_TIME;
	DMX512_DATA = 1 ;
	DMX512_MAB_TIME;
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_UART2_TXD);
	UART_WRITE(UART2, 0X00);
	 while(!(UART2->FIFOSTS&0x10000000));//发送标志1时发送完成
		for(i=0;i<length ;i++)
		{
		UART_WRITE(UART2, data[i]);
		if(UART2->FIFOSTS&0x00800000)	while(!(UART2->FIFOSTS&0x10000000));//等待数据16位发送完成
		}
	 while(!(UART2->FIFOSTS&0x10000000));//等待数据全发送完成
	//DMX512_BIT_TIME;
//		for(i=0;i<length ;i++)
//		{
//		UART_WRITE(UART2, data[i]);
//		delay_nus(800);
//		}	
//	if(length<16)
//	{
//		for(i=0;i<length ;i++)
//		{
//		UART_WRITE(UART2, data[i]);
//			
//		}
//		//delay_us(1000);//delay_nus(800);//等待FIFO16位发送完成	
//		while(!(UART2->FIFOSTS&0x10000000));//发送标志1时发送完成
//		
//	}
//	else
//	{
//		for(j =0 ; j<(length/16);j++)
//		{
//			
//			for(i=0;i<16 ;i++)
//				{
//					UART_WRITE(UART2, data[i+j*16]);
//				}
//				
//				//delay_us(1000);//等待FIFO16位发送完成	
//			while(!(UART2->FIFOSTS&0x10000000));//发送标志1时发送完成
//		}
//		
//		for(j =length-(length%16) ;j< length ; j++)
//		{
//			UART_WRITE(UART2, data[j]);
//		}
//		
//	}
		SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
		SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);
		DMX512_DATA=1;
		DMX512_STOP_TIME;	
		SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
		SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_UART2_TXD);

	taskEXIT_CRITICAL();
}







/***************************************************** 
 * @Function:  DMX_Send_data
 * @Description:   DMX512 send data
 * @param  red
 * @param  green 
 * @param  blue 
 * @retval void
******************************************************/
void DMX_Send_data(unsigned char blue,unsigned char green,unsigned char red,chip_param_t* chip_arg)
{
	chip_arg->red_data =red;
	chip_arg->green_data =green;
	chip_arg->blue_data =blue;
	DMX_DataDecode(chip_arg);
	DMX_SendPacket(Dmx512_write_data,chip_arg->chip_chanle_num);
}


void Protocol_output_data(Protocol_output_parameter_t* Protocol_output_param)
{
	uint32_t i =0;
	switch(Protocol_output_param->colour_num_param)
	{
		/*R*/
		case 1:
		{
			for(i=0;i<Protocol_output_param->On_load_channe_param;i++)
			{
				Dmx512_write_data[i]=Protocol_output_param->Red_data;
			}
			
			break;
		}
		/*RG*/
		case 2:
		{
			for(i=0;i<Protocol_output_param->On_load_channe_param;i=i+2)
			{
				Dmx512_write_data[i]=Protocol_output_param->Red_data;
				Dmx512_write_data[i+1]=Protocol_output_param->Green_data;
			}
			break;
		}
		/*RGB*/
		case 3:
		{
			for(i=0;i<Protocol_output_param->On_load_channe_param;i=i+3)
			{
				Dmx512_write_data[i]=Protocol_output_param->Red_data;
				Dmx512_write_data[i+1]=Protocol_output_param->Green_data;
				Dmx512_write_data[i+2]=Protocol_output_param->Blue_data;
			}
			break;
		}
		/*RGBW*/
		case 4:
		{
			for(i=0;i<Protocol_output_param->On_load_channe_param;i=i+4)
			{
				Dmx512_write_data[i]=Protocol_output_param->Red_data;
				Dmx512_write_data[i+1]=Protocol_output_param->Green_data;
				Dmx512_write_data[i+2]=Protocol_output_param->Blue_data;
				Dmx512_write_data[i+3]=Protocol_output_param->Write_data;
			}
			break;
		}
		/*RGBWY*/
		case 5:
		{
			for(i=0;i<Protocol_output_param->On_load_channe_param;i=i+5)
			{
				Dmx512_write_data[i]=Protocol_output_param->Red_data;
				Dmx512_write_data[i+1]=Protocol_output_param->Green_data;
				Dmx512_write_data[i+2]=Protocol_output_param->Blue_data;
				Dmx512_write_data[i+3]=Protocol_output_param->Write_data;
				Dmx512_write_data[i+4]=Protocol_output_param->Yellow_data;
			}
			break;
		}
		default:
			break;
	}
	DMX_SendPacket(Dmx512_write_data,Protocol_output_param->On_load_channe_param);
		
};

void Protocol_output_Send_data(unsigned char red,unsigned char green,unsigned char blue,unsigned char write,unsigned char yellow,Protocol_output_parameter_t* Protocol_output_param)
{
	Protocol_output_param->Blue_data=blue;
	Protocol_output_param->Green_data=green;
	Protocol_output_param->Red_data=red;
	Protocol_output_param->Write_data=write;
	Protocol_output_param->Yellow_data=yellow;
	Protocol_output_data(Protocol_output_param);
}


uchar fanx(uchar i)//数据位低变高，高变低
{
	uchar A ,j;
	A=0;
	for(j=0;j<8;j++)
	{
		if(i&0x80) A=A |0x80;
		else A = A|0x00;
	 	if(j==7) A=A;
		else   A>>=1;	 
		i<<=1;
	}
	return A ;
}
//0000 1001 --->1001 0110
uchar fanx1(uchar i)
{
	uchar A ,j;
	A=0;
	for(j=0;j<4;j++)
	{
		if(i&0x08) A = A|0x00;
		else A=A |0x80;
    A>>=1;	 
		i<<=1;
	}
		for(j=0;j<4;j++)
	{
		if(i&0x80) A=A |0x80;
		else A = A|0x00;
	  if(j==3) A=A;
		else   A>>=1;
		i<<=1;
	}
	return A ;
}
/*前四个位取后四位的反*/
uchar fanxll(uchar i)
{
	uchar A=0 ,j=0;	
	A=i;
	A &= 0X0F;
//	printf("A-->>>%x\r\n",A);
	for(j=4;j<8;j++)
	{
		if(i&0x01) A &= ~(1<<j);
			else A |= (1<<j);
		i>>=1;
	}
//	printf("A-->>>%x\r\n",A);
	return A ;
}

void DMX512_Clear_init(void)
{
	
	DMX_SendPacket(CLear_data,2048);
	DMX_SendPacket(CLear_data,2048);
	DMX_SendPacket(CLear_data,2048);
	DMX_SendPacket(CLear_data,2048);
	//DMX_SendPacket(CLear_data,2048);
	//DMX_SendPacket(CLear_data,2048);
}

/**************************************/
/********SM1751x_API*******************/
/**************************************/
/**************************************/
#if SM1751_API_CONFIG

#if 0
/***************************************************** 
 * @Function:  DMX_Send
 * @Description:   DMX512 send 
 * @param  void
 * @retval void
******************************************************/
void DMX_Send(void)
{
	unsigned char i=0 ,j=0;
	unsigned char number =  LENGTH;
	
	DMX512_ADDR = 1 ;
	DMX512_ADDR = 0 ;
	DMX512_BREAK_TIME;
	DMX512_ADDR = 1 ;
	DMX512_MAB_TIME;
	
	DMX512_ADDR = 0 ;
	DMX512_BIT_TIME;
	
	for (i=0;i<8;i++)
	{
		DMX512_ADDR=0;
		DMX512_BIT_TIME;
	}
		DMX512_ADDR = 1;
		DMX512_STOP_TIME;		
	for(j=0;j<number;j++)
	{
		//数据帧的发送
		DMX512_ADDR=0;
		DMX512_BIT_TIME;
		for(i=0;i<8 ;i++)
		{
			if( TXDData[j]&0x01)
			{
				DMX512_ADDR=1;
				DMX512_BIT_TIME;
			}
			else
			{
				DMX512_ADDR=0;
				DMX512_BIT_TIME;
			}
			TXDData[j]>>=1;
			//dmxaddrbuf[i]>>=1;
		 }
		DMX512_ADDR = 1;
		DMX512_STOP_TIME;	
		
	}
}
#endif



/***************************************************** 
 * @Function:  DMX512_Clear_init
 * @Description:   DMX512 clear 2048byte 
 * @param  void
 * @retval void
******************************************************/
/****************************
发送4帧DM512标准协议2048字节的全"0" 数据，用于清除灯具显示
*****************************/
#if sm1751x_addr_arg_write




/***************************************************** 
 * @Function:  sm1751x_write_addr
 * @Description:   sm1751x write addr 
 * @param  addr1 ...
 * @retval void
******************************************************/
/*************************************
写地址
**************************************/

#if 0
void test_addr(unsigned int addr_start)
{
	unsigned char add_L,add_H;
	add_H=fanx((uchar)(addr_start&0x00ff));
	printf("addr_start-->>>%x\r\n",(addr_start>>8));
	add_L=fanx(fanxll(addr_start>>8));
	printf("add_h-->>>%x\r\n",add_H);
	printf("add_l-->>>%x\r\n",add_L);
}
#endif



/***************************************************** 
 * @Function:  send_add
 * @Description:   sm1751x send_add 
 * @param  addr_start CH，n
 * @retval void
******************************************************/
void send_add(unsigned int addr_start,unsigned char CH,unsigned int n)
{
	unsigned int i;
	unsigned char add_L,add_H;
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	DMX512_DATA = 0 ;
	DMX512_ADDR_BREAK_TIME;
	DMX512_DATA = 1 ;
	DMX512_ADDR_MAB_TIME;	
	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);
	UART_WRITE(UART2, 0X00);
	//DMX512_BIT_TIME;
	//指令数据
	UART_WRITE(UART2, 0XA5);
	UART_WRITE(UART2, 0X5A);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);
	delay_us(1000);//等待FIFO16位发送完成	delay_nus(800);	
	/*n表示多少个字节*/
	//for(i=addr_start;i<(CH*n+addr_start);i=i+CH)
	for(i=addr_start;i<(n+addr_start);i=i+CH)
	{
		add_L=fanx((uchar)(i&0x00ff));
		add_H=fanx(fanxll(i>>8));	
		UART_WRITE(UART2,add_L);
		UART_WRITE(UART2,add_H);	
		UART_WRITE(UART2, 0XD2);
		UART_WRITE(UART2, 0X5A);
		delay_us(1000);//等待FIFO16位发送完成	delay_nus(800);		

	}
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_GPIO);
	DMX512_DATA=1;
	DMX512_STOP_TIME;	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);	
}



#if 1
void sm1751x_Write_addr(unsigned int addr_start,unsigned char CH,unsigned int n)
{
	DMX512_Clear_init();
	send_add(addr_start,CH,n);
	send_add(addr_start,CH,n);
	//printf("writer_addr_ok---\r\n");
}
#endif

/***************************************************** 
 * @Function:  sm1751x_write_arg
 * @Description:   sm1751x_write_arg 
 * @param  arg1 ...
 * @retval void
******************************************************/
/*************************************
写参数
**************************************/





void arg_switch_sm1751x(DMX512_arg_t *DMX512_arg)
{
	DMX512_arg_t *DMX512_arg_switch=&DMX512_arg_switch_data;
	DMX512_arg_switch->AUTO = 0x2b;
	if(DMX512_arg->AUTO)
	{
		DMX512_arg_switch->AUTO &= ~(1<<6);
		DMX512_arg_switch->AUTO |= (1<<7);
	}
	else
	{
		DMX512_arg_switch->AUTO |= (1<<6);
		DMX512_arg_switch->AUTO &= ~(1<<7);				
	}
	DMX512_arg_switch->AUTO=fanx(DMX512_arg_switch->AUTO);
	
	DMX512_arg_switch->REDIS_CH1_CH0=0x00;
	if(DMX512_arg->REDIS)DMX512_arg_switch->REDIS_CH1_CH0 |= (1<<2);
	else DMX512_arg_switch->REDIS_CH1_CH0 &= ~(1<<2);
	DMX512_arg_switch->REDIS_CH1_CH0 =DMX512_arg_switch->REDIS_CH1_CH0+DMX512_arg->CH1_CH0;
	DMX512_arg_switch->REDIS_CH1_CH0=fanx(DMX512_arg_switch->REDIS_CH1_CH0);
	
	DMX512_arg_switch->RGBW_R=0x00;
	DMX512_arg_switch->RGBW_R =fanx(DMX512_arg->RGBW_R);
	DMX512_arg_switch->RGBW_G=0x00;
	DMX512_arg_switch->RGBW_G =fanx(DMX512_arg->RGBW_G);
	DMX512_arg_switch->RGBW_B=0x00;
	DMX512_arg_switch->RGBW_B =fanx(DMX512_arg->RGBW_B);
	DMX512_arg_switch->RGBW_W=0x00;
	DMX512_arg_switch->RGBW_W =fanx(DMX512_arg->RGBW_W);
	
	DMX512_arg_switch->RGBW_IG_R =0X00;
	
	DMX512_arg_switch->RGBW_IG_R += DMX512_arg->RGBW_IG;
	DMX512_arg_switch->RGBW_IG_R<<=4;
	DMX512_arg_switch->RGBW_IG_R += DMX512_arg->RGBW_IR;	
	DMX512_arg_switch->RGBW_IG_R=fanx(DMX512_arg_switch->RGBW_IG_R);
	
	DMX512_arg_switch->RGBW_IW_B =0X00;
	DMX512_arg_switch->RGBW_IW_B += DMX512_arg->RGBW_IW;
	DMX512_arg_switch->RGBW_IW_B<<=4;
	DMX512_arg_switch->RGBW_IW_B += DMX512_arg->RGBW_IB;	
	DMX512_arg_switch->RGBW_IW_B=fanx(DMX512_arg_switch->RGBW_IW_B);	
	
	
}
void sm1751x_write_arg(DMX512_arg_t *DMX512_arg) //addr是12位地址
{
	arg_switch_sm1751x(DMX512_arg);
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	DMX512_DATA = 0 ;
	DMX512_ADDR_BREAK_TIME;
	DMX512_DATA = 1 ;
	DMX512_ADDR_MAB_TIME;

	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);
	UART_WRITE(UART2, 0X00);

	//指令数据
	UART_WRITE(UART2, 0XA3);
	UART_WRITE(UART2, 0X5C);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);
	delay_us(1000);//等待FIFO16位发送完成
	UART_WRITE(UART2, 0x00);
	UART_WRITE(UART2, 0x00);
	UART_WRITE(UART2, 0x00);
	UART_WRITE(UART2, 0x00);
	
	UART_WRITE(UART2, DMX512_arg_switch_data.AUTO);
	UART_WRITE(UART2, DMX512_arg_switch_data.REDIS_CH1_CH0);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_R);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_G);	
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_B);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_W);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_IG_R);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_IW_B);
	delay_us(1000);//等待FIFO16位发送完成
	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_GPIO);
	DMX512_DATA=1;
	DMX512_STOP_TIME;	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);	
}

#endif
#endif




#if 0
void IO_send_data(unsigned char data)
{
	char i = 0;
	/*起始信号4us低电平*/
	IO_TXD = 0;
	IO_Start_TIME;
	/*8位数据 每一位4us*/
	for(i = 0;i<8;i++)
	{
		IO_TXD = (data&0x01)? 1:0;
		IO_Bit_TIME;
		data>>=1;
	}	
	/*2个高电平停止位8us */
	IO_TXD = 1;
	IO_Stop_TIME;

}
void IO_send_length(unsigned char *data,unsigned int length)
{
	unsigned int i = 0;
	for(i=0;i<length;i++)
	{
		IO_send_data(data[i]);
	}
}

void IO_send_DM512Packet(unsigned char *data,unsigned int length)
{
	IO_TXD = 0 ;
	DMX512_BREAK_TIME;
	IO_TXD = 1 ;
	DMX512_MAB_TIME;
	IO_send_data(0X00);
	DMX512_BIT_TIME;
	IO_send_length(data,length);
	IO_TXD=1;
	DMX512_STOP_TIME;		
}
#endif

#if 0
unsigned char CLear_data1[2048]={0};
void sm1751x_Write_addr(unsigned int n,unsigned char (*arry)[2] )
{
	/*串口从低位开始发*/
	unsigned int i =0;
	/*发送4帔DM512标准协议2048字节*/
IO_send_DM512Packet(CLear_data1,2048);
IO_send_DM512Packet(CLear_data1,2048);
IO_send_DM512Packet(CLear_data1,2048);
IO_send_DM512Packet(CLear_data1,2048);
	
	
	IO_TXD = 0 ;
	DMX512_ADDR_BREAK_TIME;
	IO_TXD = 1 ;
	DMX512_ADDR_MAB_TIME;

	IO_send_data(0x00);
  DMX512_BIT_TIME;
	//指令数据
	IO_send_data(0XA5);
	IO_send_data(0X5A);
	IO_send_data(0XD2);
	IO_send_data(0X5A);
	DMX512_ADDR_MAB_TIME;
	for(i=0;i<n;i++)
	{
	IO_send_data( arry[i][0]);
	IO_send_data(arry[i][1]);
	IO_send_data( 0XD2);
	IO_send_data( 0X5A);	
	DMX512_ADDR_MAB_TIME;	
	}

	IO_TXD=1;
	DMX512_STOP_TIME;	
	printf("writer_addr_ok---\r\n");
}
#endif




/**************************************/
/********SM165xx_API*******************/
/**************************************/
/**************************************/

#if SM165XX_API
/*************************************
*SM165XX写址
*************************************/

void RZ_Protocol(uchar data)
{
	uchar i =0;
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	RZ_DATA = 0 ;
	for(i=0;i<8;i++)
	{
		if(data&0x80)
		{
			RZ_DATA = 1;
			Delay_Us_32;//delay_32us();
			RZ_DATA = 0;
			Delay_Us_115;//delay_115us();
		}
		else
		{
			RZ_DATA = 1;
			Delay_Us_8;//delay_8us();
			RZ_DATA = 0;
			Delay_Us_139;//delay_139us();			
		}
		data<<=1;
	}
	RZ_DATA = 1 ;
	//delay_us(100);
}


void SM165XX_Send_addr(uint Start_addr,uint channel_num,uchar ch_step,uchar chk,uchar effect)
{
	uchar i =0 ,cnt=0;
	uchar LA=0,HA =0;
	uint j = 0;
	
	if(chk == 0)
	{
		cnt=2;
		goto exti_rz;
	}
	else  cnt =3;
		
	/*4帧DMX512(500字节全0)*/
	for(i=0;i<4;i++)
	{
		DMX_SendPacket(CLear_data,500);
		delay_us(900);
	}
	
	/*1帧归零码3字节全0*/
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);
	RZ_DATA = 0;
	delay_us(1000);
	for(i=0;i<3;i++)
	{
	 RZ_Protocol(0x00);
	}
exti_rz:	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);
	/*连续3次归零码地址数据*/
	for(i=0;i<cnt;i++)
	{
	 RZ_DATA = 0;
	 delay_us(900);
		/*chip_num需要刷新两次？*/
		for(j=Start_addr;j<(Start_addr+channel_num);j=j+ch_step)
		{
			LA =(uchar)(j & 0x00ff);
			HA = j>>8;
			/*a15取a11反码*/
			if(HA&0X08)HA &= ~(1<<7);
			else HA |= 1<<7 ;
			/*通道选择*/
			if(ch_step == 3)HA |= 1<<4;
			else if(ch_step==4) HA &= ~(1<<4);
			/*上电颜色选择*/
			HA = HA + (effect<<5);
			
			RZ_Protocol(LA);
			RZ_Protocol(HA);
			RZ_Protocol(chk);
	taskENTER_CRITICAL();
	//printf("dat_la %x\r\n",LA);
//	printf("dat_ha %x\r\n",HA);
//	//
	taskEXIT_CRITICAL();			
		}
	}
	RZ_DATA = 1;
	delay_us(1000);
}

#endif





/**************************************/
/********SM1752x_API*******************/
/**************************************/
/**************************************/

#if SM1752XX_API 
/************************/
/*sm1752x写地址写参数*/

/***********************/

/***************************************************** 
 * @Function:  send_add
 * @Description:   sm1752x send_add 
 * @param  addr_start CH，n
 * @retval void
******************************************************/
void sm1752x_send_add(unsigned int addr_start,unsigned char CH,unsigned int n)
{
	unsigned int i;
	unsigned char add_L,add_H;
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	DMX512_DATA = 0 ;
	DMX512_ADDR_BREAK_TIME_1_5;
	DMX512_DATA = 1 ;
	DMX512_ADDR_MAB_TIME;	
	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);
	UART_WRITE(UART2, 0X00);
	//DMX512_BIT_TIME;
	//指令数据
	UART_WRITE(UART2, 0XA5);
	UART_WRITE(UART2, 0X5A);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);
	delay_us(1000);//等待FIFO16位发送完成	delay_nus(800);	
	/*n表示多少个字节*/
	//for(i=addr_start;i<(CH*n+addr_start);i=i+CH)
	for(i=addr_start;i<(n+addr_start);i=i+CH)
	{
		add_L=(uchar)(i&0x00ff);
		add_H=fanx(fanx1(i>>8));	
		UART_WRITE(UART2,add_H);
		UART_WRITE(UART2,add_L);	
		UART_WRITE(UART2, 0XD2);
		UART_WRITE(UART2, 0X5A);
		delay_us(1000);//等待FIFO16位发送完成	delay_nus(800);		

	}
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_GPIO);
	DMX512_DATA=1;
	DMX512_STOP_TIME;	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);	
}

void arg_switch_sm1752x(DMX512_arg_t *DMX512_arg)
{
	DMX512_arg_t *DMX512_arg_switch=&DMX512_arg_switch_data;
	
	
	DMX512_arg_switch->RGBW_R =DMX512_arg->RGBW_R;
	
	DMX512_arg_switch->RGBW_G =DMX512_arg->RGBW_G;
	
	DMX512_arg_switch->RGBW_B =DMX512_arg->RGBW_B;
	
	DMX512_arg_switch->RGBW_W =DMX512_arg->RGBW_W;
	
	
	DMX512_arg_switch->AUTO_STEP =DMX512_arg->AUTO_STEP;
	
	DMX512_arg_switch->REDIS_CH1_CH0 = 0X00;
	DMX512_arg_switch->REDIS_CH1_CH0 = (DMX512_arg_switch->REDIS_CH1_CH0+DMX512_arg->CH1_CH0)<<2;//CH0_CH1第2，3位
	if(DMX512_arg->REDIS)DMX512_arg_switch->REDIS_CH1_CH0 |= (1<<1);//REDIS 第1位
	else DMX512_arg_switch->REDIS_CH1_CH0 &= ~(1<<1);
	if(DMX512_arg->AUTO)			DMX512_arg_switch->REDIS_CH1_CH0 ++;//AUTO第0位
	
	
	
	
	
	DMX512_arg_switch->RGBW_IR =0X00;	
	DMX512_arg_switch->RGBW_IR = DMX512_arg_switch->RGBW_IR + (DMX512_arg->RGBW_IR>>1);
	
	
	DMX512_arg_switch->RGBW_IG =0X00;	
	if(DMX512_arg->RGBW_IR &0x01) DMX512_arg_switch->RGBW_IG++;//ir0
	DMX512_arg_switch->RGBW_IG<<=5;
	DMX512_arg_switch->RGBW_IG = DMX512_arg_switch->RGBW_IG+DMX512_arg->RGBW_IG;//IG
	DMX512_arg_switch->RGBW_IG<<=2;
	DMX512_arg_switch->RGBW_IG = DMX512_arg_switch->RGBW_IG+((DMX512_arg->RGBW_IB>>3)&0X03);//IB4,IB3
	
	DMX512_arg_switch->RGBW_IW_B =0X00;
	DMX512_arg_switch->RGBW_IW_B=DMX512_arg_switch->RGBW_IW_B+(DMX512_arg->RGBW_IB&0X07);
	DMX512_arg_switch->RGBW_IW_B<<=3;
	DMX512_arg_switch->RGBW_IW_B = DMX512_arg_switch->RGBW_IW_B+ DMX512_arg->RGBW_IW;
		
}


void sm1752x_write_arg(DMX512_arg_t *DMX512_arg) //addr是12位地址
{
	arg_switch_sm1752x(DMX512_arg);
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	DMX512_DATA = 0 ;
	DMX512_ADDR_BREAK_TIME_1_5;
	DMX512_DATA = 1 ;
	DMX512_ADDR_MAB_TIME;

	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);
	UART_WRITE(UART2, 0X00);

	//指令数据
	UART_WRITE(UART2, 0XA3);
	UART_WRITE(UART2, 0X5C);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);
	delay_us(1000);//等待FIFO16位发送完成

	

	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_R);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_G);	
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_B);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_W);
	UART_WRITE(UART2, DMX512_arg_switch_data.AUTO_STEP);
	UART_WRITE(UART2, DMX512_arg_switch_data.REDIS_CH1_CH0);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);	
	delay_us(1000);//等待FIFO16位发送完成
	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_GPIO);
	DMX512_DATA=1;
	DMX512_STOP_TIME;	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);	
}


void sm1752x_write_elect_arg(DMX512_arg_t *DMX512_arg) //addr是12位地址
{
	arg_switch_sm1752x(DMX512_arg);
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	DMX512_DATA = 0 ;
	DMX512_ADDR_BREAK_TIME_1_5;
	DMX512_DATA = 1 ;
	DMX512_ADDR_MAB_TIME;

	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);
	UART_WRITE(UART2, 0X00);

	//指令数据
	UART_WRITE(UART2, 0XAC);
	UART_WRITE(UART2, 0X53);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);
	delay_us(1000);//等待FIFO16位发送完成

	

	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_IR);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_IG);	
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_IB);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);	
	delay_us(1000);//等待FIFO16位发送完成
	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_GPIO);
	DMX512_DATA=1;
	DMX512_STOP_TIME;	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);	
}

#endif





/**************************************/
/********SM17500_API*******************/
/**************************************/
/**************************************/
#if SM17500_API_CONFIG
/****************************SM17500写地址*********************/
/****************************SM17500*********************/
/****************************SM17500*********************/
/***************************************************** 
 * @Function:  send_add
 * @Description:   sm17500x send_add 
 * @param  addr_start CH，n
 * @retval void
******************************************************/
void sm17500x_send_add(unsigned int addr_start,unsigned char CH,unsigned int n)
{
	//taskENTER_CRITICAL();
	unsigned int i;
	unsigned char add_L,add_H;
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	DMX512_DATA = 0 ;
	DMX512_ADDR_BREAK_TIME_1_5;
	DMX512_DATA = 1 ;
	DMX512_ADDR_MAB_TIME;	
	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);
	UART_WRITE(UART2, 0X00);
	//DMX512_BIT_TIME;
	//指令数据
	UART_WRITE(UART2, 0XA5);
	UART_WRITE(UART2, 0X5A);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);
	delay_us(1000);//等待FIFO16位发送完成	delay_nus(800);	
	/*n表示多少个字节*/
	//for(i=addr_start;i<(CH*n+addr_start);i=i+CH)
	for(i=addr_start;i<(n+addr_start);i=i+CH)
	{
		add_L=(uchar)(i&0x00ff);
		add_H=fanx(fanx1(i>>8));
		
//    printf("addh%x\r\n",add_H);	
//    printf("addL%x\r\n",add_L);
		
		UART_WRITE(UART2,add_H);
		UART_WRITE(UART2,add_L);	
		UART_WRITE(UART2, 0XD2);
		UART_WRITE(UART2, 0X5A);
		delay_us(1000);//等待FIFO16位发送完成	delay_nus(800);		

	}
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_GPIO);
	DMX512_DATA=1;
	DMX512_STOP_TIME;	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);
//taskEXIT_CRITICAL();	
}

/***************************************************** 
 * @Function:  send_self_add
 * @Description:   sm17500x send_add 
 * @param  addr_start CH，n
 * @retval void
******************************************************/
void sm17500x_send_self_add(unsigned int addr_start,unsigned int n)
{
	unsigned int i;
	unsigned char add_L,add_H;
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	DMX512_DATA = 0 ;
	DMX512_ADDR_BREAK_TIME_1_5;
	DMX512_DATA = 1 ;
	DMX512_ADDR_MAB_TIME;	
	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);
	UART_WRITE(UART2, 0X00);
	//DMX512_BIT_TIME;
	//指令数据
	UART_WRITE(UART2, 0XA7);
	UART_WRITE(UART2, 0X58);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);
	delay_us(1000);//等待FIFO16位发送完成	delay_nus(800);	
	/*n表示多少个字节*/
	//for(i=addr_start;i<(CH*n+addr_start);i=i+CH)
	for(i=addr_start;i<(n+addr_start);i++)//地址加1
	{
		add_L=(uchar)(i&0x00ff);
		add_H=fanx(fanx1(i>>8));	
		UART_WRITE(UART2,add_H);
		UART_WRITE(UART2,add_L);	
		UART_WRITE(UART2, 0XD2);
		UART_WRITE(UART2, 0X5A);
		delay_us(1000);//等待FIFO16位发送完成	delay_nus(800);		

	}
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_GPIO);
	DMX512_DATA=1;
	DMX512_STOP_TIME;	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);	
}

void arg_switch_sm17500x(DMX512_arg_t *DMX512_arg)
{
	DMX512_arg_t *DMX512_arg_switch=&DMX512_arg_switch_data;
	
	
	DMX512_arg_switch->RGBW_R =DMX512_arg->RGBW_R;
	
	DMX512_arg_switch->RGBW_G =DMX512_arg->RGBW_G;
	
	DMX512_arg_switch->RGBW_B =DMX512_arg->RGBW_B;
	
	DMX512_arg_switch->RGBW_W =DMX512_arg->RGBW_W;
	
	//0000 000 IC
	DMX512_arg_switch->IC_select =0X00;
	if(DMX512_arg->IC_select)DMX512_arg_switch->IC_select++;
	
	// 0  EN1 EN0 RTZ CH 0 REDIS 0
	DMX512_arg_switch->REDIS_CH1_CH0 = 0X00;
	DMX512_arg_switch->REDIS_CH1_CH0 = DMX512_arg_switch->REDIS_CH1_CH0 + DMX512_arg->EN0_EN1;
	DMX512_arg_switch->REDIS_CH1_CH0<<=1;
	if(DMX512_arg->RTZ_select)DMX512_arg_switch->REDIS_CH1_CH0++;
	DMX512_arg_switch->REDIS_CH1_CH0<<=1;
	if(DMX512_arg->CH1_CH0)DMX512_arg_switch->REDIS_CH1_CH0++;
	DMX512_arg_switch->REDIS_CH1_CH0<<=2;
	if(DMX512_arg->REDIS)DMX512_arg_switch->REDIS_CH1_CH0++;
	DMX512_arg_switch->REDIS_CH1_CH0<<=1;
	

	DMX512_arg_switch->RGBW_IR =DMX512_arg->RGBW_IR;
	
	DMX512_arg_switch->RGBW_IG =DMX512_arg->RGBW_IG;
	
	DMX512_arg_switch->RGBW_IB =DMX512_arg->RGBW_IB;
	
	DMX512_arg_switch->RGBW_IW =DMX512_arg->RGBW_IW;	
		
}
/*SM17500参数设置*/
void sm17500x_write_arg(DMX512_arg_t *DMX512_arg) //addr是12位地址
{
	arg_switch_sm17500x(DMX512_arg);
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	DMX512_DATA = 0 ;
	DMX512_ADDR_BREAK_TIME_1_5;
	DMX512_DATA = 1 ;
	DMX512_ADDR_MAB_TIME;

	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);
	UART_WRITE(UART2, 0X00);

	//指令数据
	UART_WRITE(UART2, 0XA3);
	UART_WRITE(UART2, 0X5C);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);
	delay_us(1000);//等待FIFO16位发送完成

	

	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_R);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_G);	
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_B);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_W);
	UART_WRITE(UART2, DMX512_arg_switch_data.IC_select);
	UART_WRITE(UART2, DMX512_arg_switch_data.REDIS_CH1_CH0);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);	
	delay_us(1000);//等待FIFO16位发送完成
	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_GPIO);
	DMX512_DATA=1;
	DMX512_STOP_TIME;	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);	
}
/*SM17500电流增益*/
void sm17500x_write_elect_arg(DMX512_arg_t *DMX512_arg) //addr是12位地址
{
	arg_switch_sm17500x(DMX512_arg);
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	DMX512_DATA = 0 ;
	DMX512_ADDR_BREAK_TIME_1_5;
	DMX512_DATA = 1 ;
	DMX512_ADDR_MAB_TIME;

	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);
	UART_WRITE(UART2, 0X00);

	//指令数据
	UART_WRITE(UART2, 0XAC);
	UART_WRITE(UART2, 0X53);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);
	delay_us(1000);//等待FIFO16位发送完成

	

	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_IR);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_IG);	
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_IB);
	UART_WRITE(UART2, DMX512_arg_switch_data.RGBW_IW);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);	
	delay_us(1000);//等待FIFO16位发送完成
	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_GPIO);
	DMX512_DATA=1;
	DMX512_STOP_TIME;	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);	
}
/*自通道数协议*/
void sm17500x_send_self_channel(unsigned int n,DMX512_arg_t *DMX512_arg)
{
	unsigned int i;
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	DMX512_DATA = 0 ;
	DMX512_ADDR_BREAK_TIME_1_5;
	DMX512_DATA = 1 ;
	DMX512_ADDR_MAB_TIME;	
	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);
	UART_WRITE(UART2, 0X00);
	//DMX512_BIT_TIME;
	//指令数据
	UART_WRITE(UART2, 0XA6);
	UART_WRITE(UART2, 0X59);
	UART_WRITE(UART2, 0XD2);
	UART_WRITE(UART2, 0X5A);
	delay_us(1000);//等待FIFO16位发送完成	delay_nus(800);	
	/*n表示多少个字节*/
	//for(i=addr_start;i<(CH*n+addr_start);i=i+CH)
	for(i=0;i<n;i++)
	{
	
		UART_WRITE(UART2,DMX512_arg->SELF_CHAN_LD);
		UART_WRITE(UART2, 0XD2);
		UART_WRITE(UART2, 0X5A);
		delay_us(1000);//等待FIFO16位发送完成	delay_nus(800);		

	}
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC2MFP_GPIO);
	DMX512_DATA=1;
	DMX512_STOP_TIME;	
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk );
	SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD);	
}
#endif




void RZ_Write_data(uchar data)
{
	uchar i =0;
//	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
//  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
//	RZ_DATA = 0 ;	
	for(i=0;i<8;i++)
	{
		if(data&0x80)
		{
			IO_TXD =1;
			NS_900;
			IO_TXD =0;
			NS_300;
		}
		else
		{
			IO_TXD =1;
			NS_300;
			IO_TXD =0;
			NS_900;			
		}
		data<<=1;
	}
}

void RZ_Write_Low_5_bit_data(uchar data)
{
	uchar i =0;
//	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
//  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
//	RZ_DATA = 0 ;	
	for(i=0;i<5;i++)
	{
		if(data&0x10)
		{
			IO_TXD =1;
			NS_900;
			IO_TXD =0;
			NS_300;
		}
		else
		{
			IO_TXD =1;
			NS_300;
			IO_TXD =0;
			NS_900;			
		}
		data<<=1;
	}
}
//考虑用函数一个字节一个字节的发，可能延时的时间不满足RZ时序
#if 0
void SM16804P_write_data(RZ_arg_t* RZ_arg)
{
	uchar i =0;
	/*IRGBW_SWITCH*/
	RZ_arg->RGBW_IR_G += RZ_arg->RGBW_IR;
	RZ_arg->RGBW_IR_G<<=4;
	RZ_arg->RGBW_IR_G = RZ_arg->RGBW_IR_G +(RZ_arg->RGBW_IG & 0XF0);
	RZ_arg->RGBW_IB_W += RZ_arg->RGBW_IB;
	RZ_arg->RGBW_IB_W<<=4;
	RZ_arg->RGBW_IB_W = RZ_arg->RGBW_IB_W +(RZ_arg->RGBW_IW & 0XF0);
	
//	
//	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
//  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	IO_TXD = 0 ;	
	US_80;
	for(i=0;i<RZ_arg->Chip_Num;i++)
	{
		RZ_Write_data(RZ_arg->RGBW_R);
		RZ_Write_data(RZ_arg->RGBW_G);
		RZ_Write_data(RZ_arg->RGBW_B);
		RZ_Write_data(RZ_arg->RGBW_W);
	}
		RZ_Write_data(RZ_arg->RGBW_IR_G);
		RZ_Write_data(RZ_arg->RGBW_IB_W);
		IO_TXD = 0 ;
		US_80;
		IO_TXD = 1 ;
}
#endif
void SM16804P_Display(uchar RED,uchar GREEN,uchar BLUE,RZ_arg_t* RZ_arg)
{
	RZ_arg->RGBW_R=RED;
	RZ_arg->RGBW_G=GREEN;
	RZ_arg->RGBW_B=BLUE;
	SM16804P_write_data2(RZ_arg);
}

#if SM16804P_write_data2_En
void SM16804P_write_data2(RZ_arg_t* RZ_arg)
{
	uchar i =0,j=0;
	uchar datbuf;
	uchar datbitbuf[48];
	/*IRGBW_SWITCH*/
	RZ_arg->RGBW_IR_G += RZ_arg->RGBW_IR;
	RZ_arg->RGBW_IR_G<<=4;
	RZ_arg->RGBW_IR_G = RZ_arg->RGBW_IR_G +(RZ_arg->RGBW_IG & 0X0f);
	
	RZ_arg->RGBW_IB_W += RZ_arg->RGBW_IB;
	RZ_arg->RGBW_IB_W<<=4;
	RZ_arg->RGBW_IB_W = RZ_arg->RGBW_IB_W +(RZ_arg->RGBW_IW & 0X0f);
	
	datbuf=RZ_arg->RGBW_R;
	for(i=0;i<8;i++)
	{
	 datbitbuf[i]=datbuf&0x80 ? 1:0 ;
	 datbuf<<=1;
	}
	
		datbuf=RZ_arg->RGBW_G;
	for(i=8;i<16;i++)
	{
	 datbitbuf[i]=datbuf&0x80 ? 1:0 ;
	 datbuf<<=1;
	}
	
		datbuf=RZ_arg->RGBW_B;
	for(i=16;i<24;i++)
	{
	 datbitbuf[i]=datbuf&0x80 ? 1:0 ;
	 datbuf<<=1;
	}
	
		datbuf=RZ_arg->RGBW_W;
	for(i=24;i<32;i++)
	{
	 datbitbuf[i]=datbuf&0x80 ? 1:0 ;
	 datbuf<<=1;
	}
	
	datbuf=RZ_arg->RGBW_IR_G;
	for(i=32;i<40;i++)
	{
	 datbitbuf[i]=datbuf&0x80 ? 1:0 ;
	 datbuf<<=1;
	}
	
	datbuf=RZ_arg->RGBW_IB_W;
	for(i=40;i<48;i++)
	{
	 datbitbuf[i]=datbuf&0x80 ? 1:0 ;
	 datbuf<<=1;
	}		
	IO_TXD = 0 ;	
	US_80;
	for(j=0;j<RZ_arg->Chip_Num;j++)
	{
		for(i=0;i<32;i++)
		{
			if  ( datbitbuf[i] ) 
			{
				IO_TXD =1;
				NS_900;
				IO_TXD =0;
				NS_300;
			}
			else
			{
				IO_TXD =1;
				NS_300;
				IO_TXD =0;
				NS_900;
			}
		}
		
	}
		for(i=32;i<48;i++)
		{
			if  ( datbitbuf[i] ) 
			{
				IO_TXD =1;
				NS_900;
				IO_TXD =0;
				NS_300;
			}
			else
			{
				IO_TXD =1;
				NS_300;
				IO_TXD =0;
				NS_900;
			}
		}
		IO_TXD = 0 ;
		US_80;
		IO_TXD = 1 ;		
	
}
#endif


void SM16803P_write_data(RZ_arg_t* RZ_arg)
{
	uchar i =0;
	/*IRGBW_SWITCH*/

	RZ_arg->RGBW_IB_W += RZ_arg->RGBW_IB;
	RZ_arg->RGBW_IB_W<<=4;
	RZ_arg->RGBW_IB_W = RZ_arg->RGBW_IB_W +(RZ_arg->RGBW_IW & 0XF0);
	

	IO_TXD = 0 ;	
	US_80;
	for(i=0;i<RZ_arg->Chip_Num;i++)
	{
		RZ_Write_data(RZ_arg->RGBW_R);
		RZ_Write_data(RZ_arg->RGBW_G);
		RZ_Write_data(RZ_arg->RGBW_B);
	}
		RZ_Write_data(RZ_arg->RGBW_IR);
		RZ_Write_data(RZ_arg->RGBW_IB_W);
		IO_TXD = 0 ;
		US_80;
		IO_TXD = 1 ;
}

void SM16703P_write_data(RZ_arg_t* RZ_arg)
{
	uchar i =0;

	

	IO_TXD = 0 ;	
	US_80;
	for(i=0;i<RZ_arg->Chip_Num;i++)
	{
		RZ_Write_data(RZ_arg->RGBW_R);
		RZ_Write_data(RZ_arg->RGBW_G);
		RZ_Write_data(RZ_arg->RGBW_B);
	}
		IO_TXD = 0 ;
		US_80;
		IO_TXD = 1 ;
}

void SM16703P_Display(uchar RED,uchar GREEN,uchar BLUE,RZ_arg_t* RZ_arg)
{
	RZ_arg->RGBW_R=RED;
	RZ_arg->RGBW_G=GREEN;
	RZ_arg->RGBW_B=BLUE;
	SM16703P_write_data(RZ_arg);
}

void SM16714P_write_data(RZ_arg_t* RZ_arg)
{
	uchar i =0;
	/*IRGBW_SWITCH*/

	
//	
//	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
//  SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_GPIO);	
	IO_TXD = 0 ;	
	US_80;
	for(i=0;i<RZ_arg->Chip_Num;i++)
	{
		RZ_Write_data(RZ_arg->RGBW_R);
		RZ_Write_data(RZ_arg->RGBW_G);
		RZ_Write_data(RZ_arg->RGBW_B);
		RZ_Write_data(RZ_arg->RGBW_W);
	}
		RZ_Write_Low_5_bit_data(RZ_arg->RGBW_IR);
		RZ_Write_Low_5_bit_data(RZ_arg->RGBW_IG);
		RZ_Write_Low_5_bit_data(RZ_arg->RGBW_IB);
		RZ_Write_Low_5_bit_data(RZ_arg->RGBW_IW);	
		IO_TXD = 0 ;
		US_80;
		IO_TXD = 1 ;
}

#if FD6603_write_data_en
void FD6603_write_data(RZ_arg_t* RZ_arg)
{
	uchar i =0,j=0;
	uchar datbuf;
	uchar datbitbuf[24];
	/*IRGBW_SWITCH*/

	
	datbuf=RZ_arg->RGBW_R;
	for(i=0;i<8;i++)
	{
	 datbitbuf[i]=datbuf&0x80 ? 1:0 ;
	 datbuf<<=1;
	}
	
		datbuf=RZ_arg->RGBW_G;
	for(i=8;i<16;i++)
	{
	 datbitbuf[i]=datbuf&0x80 ? 1:0 ;
	 datbuf<<=1;
	}
	
		datbuf=RZ_arg->RGBW_B;
	for(i=16;i<24;i++)
	{
	 datbitbuf[i]=datbuf&0x80 ? 1:0 ;
	 datbuf<<=1;
	}
	

	
	IO_TXD = 0 ;	
	US_80;
	for(j=0;j<RZ_arg->Chip_Num;j++)
	{
		for(i=0;i<24;i++)
		{
			if  ( datbitbuf[i] ) 
			{
				IO_TXD =1;
				NS_900;
				IO_TXD =0;
				NS_300;
			}
			else
			{
				IO_TXD =1;
				NS_300;
				IO_TXD =0;
				NS_900;
			}
		}
		
	}

		IO_TXD = 0 ;
		US_80;
		IO_TXD = 1 ;		
	
}
#endif
