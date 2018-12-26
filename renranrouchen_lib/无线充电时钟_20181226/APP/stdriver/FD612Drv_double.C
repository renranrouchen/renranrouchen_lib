/** 
* @file 		  FD612Drv.C   	数码管显示驱动芯片 客户代码
* @brief       FD612 的驱动
* @author      ywaby
* @version     1B3 
* @date 			2014-03-31    
* @note
* @details  
* @copyright Copyright (c) 2013 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.              
*/
#define FD612DRV_GROBLE

#include "FD612Drv_double.H"
#include <stdio.h>
#include <intrins.h>


static void FD628_Delay10us()
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}

/** @addtogroup FD612_DRIVER FD612 驱动
  @{
*/
#define 	FD612_DELAY_LOW		     	FD612_DELAY_1US         ///<时钟低电平延时            		        
#define		FD612_DELAY_HIGH     	 	FD612_DELAY_1US 	   		///<时钟高电平延时    								 			          				  
#define  	FD612_DELAY_WAIT				FD612_DELAY_1US					///<数据读取等待时间
#define		FD612_DELAY_SU_STA      FD612_DELAY_1US					///<起始信号建立时间 
#define		FD612_DELAY_HD_STA      FD612_DELAY_1US					///<起始信号保持时间					 
#define		FD612_DELAY_SU_STO      FD612_DELAY_1US					///<停止信号建立时间  
 /** 
 * @brief 数码管码值编码数组,依次对应的显示：0,1,2,3,4,5,6,7,8,9,A,b,C,d,E,F 			
 */
CONST INT8U DISP_TAB[0X10]={0x3f,0x06,0x5B,0x4F,0x66,0x6D,0x7d,0x07,0x7F,0x6F,0x77,0x7c,0x39,0x5e,0x79,0x71};
/** @addtogroup  FD612_FUNCTION  函数
  @{
*/
/** @addtogroup  FD612_COMMUNICATION  通信时序
  @{
*/
/**
 *  @brief  启动FD612通信
 *  @param  void
 *  @return void
 *  @note   在SCL高电平期间捕获到SDA的下降沿，FD612开始通信   
 */
static void FD612_Start(t_fd612_ops * ops )		 	 
{
	ops->FD612_SDA_SET(); 				 
	ops->FD612_SCL_SET();
	ops->FD612_SDA_D_OUT();				 //设置SDA为输出方向 
	ops->FD612_SCL_D_OUT();				 //设置SCL为输出方向 	
	FD612_DELAY_SU_STA;
	ops->FD612_SDA_CLR();					 //发送起始信号
	FD612_DELAY_HD_STA;      
	ops->FD612_SCL_CLR();					 
	FD612_DELAY_LOW;
}		

/**
 *  @brief  停止FD612通信
 *  @param  void
 *  @return void
 *  @note   在SCL高电平期间捕获到SDA的上升沿，FD612停止通信  
 */
 static void FD612_Stop(t_fd612_ops* ops )
{  
 	ops->FD612_SCL_SET();					  /* 设置SCL为高电平 */ 
	ops->FD612_SDA_CLR();					  /* 设置SDA为低电平 */
	ops->FD612_SDA_D_OUT();					  /* 设置SDA为输出方向 */
	FD612_DELAY_SU_STO;				  /* 停止信号建立时间: */
	ops->FD612_SDA_SET();					  /* 设置SDA为高电平 */
	ops->FD612_SDA_D_IN();				  /* 设置SDA为输入方向 */
}
/**
 *  @brief  发送一个字节数据给FD612,并读取应答位
 *  @param  dat 发送的一字节数据
 *  @retval  BOOLEAN  
 *  @return 读取的ACK应答位
 *  @note   数据传输低位在前，高位在后
 */

 BOOLEAN FD612_WrByte( t_fd612_ops* ops,INT8U dat )
{
	INT8U i;				        /* 移位写出的位数寄存器 */
  BOOLEAN ACKTemp=0;
	ops->FD612_SDA_D_OUT();			        /* 设置SDA为输出方向 */
	for( i = 0; i != 8; i++ )	    /* 输出8 bit的数据 */        
	{		   
		if( dat & 0x80 ) 
		{
		    ops->FD612_SDA_SET();		    /* 设置SDA为高电平 */
		}
		else 
		{
		    ops->FD612_SDA_CLR();			/* 设置SDA为低电平 */
		}
  ops->FD612_SCL_SET();				/* 设置SCL为高电平 */
  dat <<= 1;					/* 输出数据右移一位，数据从高到低的输出 */
  FD612_DELAY_HIGH;           /* SCL时钟高电平时间：*/
  ops->FD612_SCL_CLR();				/* 设置SCL为低电平 */
  FD612_DELAY_LOW;	
	}	
  //////读取ACK位/////
  ops->FD612_SDA_SET();			         /* 设置SDA为高电平 */	    
  ops->FD612_SDA_D_IN();				    /* 设置SDA为输入方向 */ 
  ops->FD612_SCL_SET();					  /* SCL时钟的高电平时间：*/
  if(ops->FD612_SDA_IN()==0) ACKTemp=0; /* 读入1 bit值 */
  else ACKTemp=1; 
  FD612_DELAY_HIGH;                 /* SCL时钟高电平时间*/
  ops->FD612_SCL_CLR();					      /* SCL时钟的低电平*/
  FD612_DELAY_LOW;	
  return  ACKTemp ;  
}	













/**
 *  @brief  向FD612发送一字节的命令
 *  @param  CMD 发送一字节的命令
 *  @return void
 *  @note   CMD是控制命令中的宏
 *  @par Example 
 *  @code
 *  FD612_Command(FD612_7SEG_CMD);
 *  @endcode 
 */
void FD612_Command(t_fd612_ops* ops, INT8U CMD )		  		
{									
	FD612_Start(ops);
	FD612_WrByte(ops,CMD);
	FD612_Stop(ops);	
}
 /**
 *  @brief  刷新FD612的显示数据
 *  @param  void
 *  @return void
 *  @note   使用该函数需要打开 FD612_DISP_BUFF_EN
 */
void FD612_DispDataRefresh(FD612_Struct_DispBuff* FD612_DispBuff,t_fd612_ops* ops)
{
	INT8U i;
	FD612_Command(ops,FD612_ADDR_INC_DIGWR_CMD);	
	FD612_Start(ops);		
	FD612_WrByte(ops,FD612_DIGADDR_WRCMD|FD612_DISP_MIN_ADDR) ;
#if FD628_NEGA_DISP==1
	for(i=FD612_DISP_MIN_ADDR;i<=FD612_DISP_MAX_ADDR;i++)
		FD612_PotiveTNage(i,FD612_DispBuff->DispData[i], FD612_DispBuff);	
#endif
	for(i=FD612_DISP_MIN_ADDR;i<=FD612_DISP_MAX_ADDR;i++)
		FD612_WrByte(ops,FD612_DispBuff->DispData[i]) ;

	FD612_Stop(ops);
}
 /**
 *  @brief  刷新FD612的相关数据
 *  @param  void
 *  @return void
 *  @note    
 *  @note   使用该函数需要打开 FD612_DISP_BUFF_EN 
 */
void FD612_Base_Refresh(FD612_Struct_DispBuff* FD612_DispBuff,t_fd612_ops* ops)
{
	FD612_Command(ops,FD612_DispBuff->DispSEG_MODE);
	FD612_DispDataRefresh(FD612_DispBuff,ops);
	FD612_Command(ops,FD612_DispBuff->DispState);
} 
#if 0
void FD612_Top_Refresh(FD612_Struct_DispBuff* FD612_DispBuff,t_fd612_ops* ops)
{
	FD612_Command(ops,FD612_DispBuff->DispSEG_MODE);
	FD612_DispDataRefresh(FD612_DispBuff,ops);
	FD612_Command(ops,FD612_DispBuff->DispState);
}
#endif
/* @} FD612_API_FUNTION */
#if FD612_DECODE_TAB_EN!=0

#define FD612_DECODE_TAB_NUM 14*4 ///<FD612_DecodeTab[]的字符个数
typedef struct 
{
	INT8U Character; ///<字符
	INT8U Bitmap;     ///<字符对应的码值
} Struct_LED_Bitmap; ///<数码管的码值和字符的对应结构体，用于查表
///<数码管的码值和字符的对应表格，用于查表
CONST Struct_LED_Bitmap FD612_DecodeTab[FD612_DECODE_TAB_NUM] = {
 	{'0', 0x3F}, {'1', 0x06}, {'2', 0x5B}, {'3', 0x4F},
	{'4', 0x66}, {'5', 0x6D}, {'6', 0x7D}, {'7', 0x07},
	{'8', 0x7F}, {'9', 0x6F}, {'a', 0x77}, {'A', 0x77},
	{'b', 0x7C}, {'B', 0x7C}, {'c', 0x39}, {'C', 0x39},
	{'d', 0x5E}, {'D', 0x5E}, {'e', 0x79}, {'E', 0X7b},
	{'f', 0x71}, {'F', 0x71}, {'g', 0x6F}, {'G', 0x3D},
	{'h', 0x74}, {'H', 0x76}, {'i', 0x04}, {'I', 0x06},
	{'j', 0x1E}, {'J', 0x1E}, {'l', 0x38}, {'L', 0x38},
	{'n', 0x54}, {'N', 0x37}, {'o', 0x5C}, {'O', 0x3F},
	{'p', 0x73}, {'P', 0x73}, {'q', 0x67}, {'Q', 0x67},
	{'r', 0x50}, {'R', 0x77}, {'s', 0x6D}, {'S', 0x6D},
	{'t', 0x78}, {'T', 0x31}, {'u', 0x3E}, {'U', 0x3E},
	{'y', 0x6E}, {'Y', 0x6E}, {'z', 0x5B}, {'Z', 0x5B},
	{':', 0xfe}, {'-', 0x40}, {'_', 0x08}, {' ', 0x00}
};

/**
 *  @brief  数码管的查表函数
 *  @param  cTemp 查找的字符
 *  @retval INT8U  
 *  @return 字符对应的数码管码值
 *  @note   使用该函数需要打开 FD612_DECODE_TAB_EN 
 *  @par Example
 *  @code
 *  char CTmp;
 *  CTmp=DispGetCode('S');
 *  @endcode 
 */ 
 
INT8U DispGetCode(char cTemp)
{
	INT8U i, Bitmap=0x00;
	for(i=0; i<FD612_DECODE_TAB_NUM; i++)
	{
		if(FD612_DecodeTab[i].Character == cTemp)
		{
			Bitmap = FD612_DecodeTab[i].Bitmap;
			break;
		}
	}
	return Bitmap;
}
#endif

#if FD628_NEGA_DISP==0
typedef struct 
{
	INT8U NegaAddr; ///<对应的共阴数码管数据地址
	INT8U BitAddr;     ///<对应段地址
} Struct_PotiveTNage_Bitmap; ///<数码管的码值和字符的对应结构体，用于查表
///<共阳对应共阴的表格，用于查表,根据具体的应用电路进行修改
CONST Struct_PotiveTNage_Bitmap FD612_PotiveTNage_Bitmap[12][8] = {
    { {0x04,0x01},{0x05,0x01},{0x06,0x01},{0x07,0x01},  {0x08,0x01},{0x09,0x01},{0x0a,0x01},{0x0b,0x01}},    //位4到11     
    { {0x04,0x02},{0x05,0x02},{0x06,0x02},{0x07,0x02},  {0x08,0x02},{0x09,0x02},{0x0a,0x02},{0x0b,0x02}},  	//	
    { {0x04,0x04},{0x05,0x04},{0x06,0x04},{0x07,0x04},  {0x08,0x04},{0x09,0x04},{0x0a,0x04},{0x0b,0x04}}, 
    { {0x04,0x08},{0x05,0x08},{0x06,0x08},{0x07,0x08},  {0x08,0x08},{0x09,0x08},{0x0a,0x08},{0x0b,0x08}},

    { {0x00,0x01},{0x01,0x01},{0x02,0x01},{0x03,0x01},  {0x08,0x10},{0x09,0x10},{0x0a,0x10},{0x0b,0x10}},   //位0到3  8到11
    { {0x00,0x02},{0x01,0x02},{0x02,0x02},{0x03,0x02},  {0x08,0x20},{0x09,0x20},{0x0a,0x20},{0x0b,0x20}},
    { {0x00,0x04},{0x01,0x04},{0x02,0x04},{0x03,0x04},  {0x08,0x40},{0x09,0x40},{0x0a,0x40},{0x0b,0x40}},
    { {0x00,0x08},{0x01,0x08},{0x02,0x08},{0x03,0x08},  {0x08,0x80},{0x09,0x80},{0x0a,0x80},{0x0b,0x80}},

    { {0x00,0x10},{0x01,0x10},{0x02,0x10},{0x03,0x10},  {0x04,0x10},{0x05,0x10},{0x06,0x10},{0x07,0x10}},   //位0到7
    { {0x00,0x20},{0x01,0x20},{0x02,0x20},{0x03,0x20},  {0x04,0x20},{0x05,0x20},{0x06,0x20},{0x07,0x20}},
    { {0x00,0x40},{0x01,0x40},{0x02,0x40},{0x03,0x40},  {0x04,0x40},{0x05,0x40},{0x06,0x40},{0x07,0x40}},
    { {0x00,0x80},{0x01,0x80},{0x02,0x80},{0x03,0x80},  {0x04,0x80},{0x05,0x80},{0x06,0x80},{0x07,0x80}}
};

 /** @addtogroup FD612_API_FUNTION 
  @{
*/

/** @addtogroup FD612_PT_API 共阳数码管应用函数
  @{
*/
  /**
 *  @brief  共阳数码管的某一位写入显示数据
 *  @param  Addr 共阳数码管的位
 *  @param  Dat   显示的数据
 *  @retval void  
 *  @return 
 *  @note   使用该函数需要打开 FD612_DECODE_TAB_EN FD612_DISP_BUFF_EN FD628_NEGA_DISP
 *  @par Example
 *  @code
 *  PotiveTNage(1,0X3F); //第一位数码管显示‘0’
 *  @endcode 
 */ 
void FD612_PotiveTNage(INT8U Addr,INT8U Dat,FD612_Struct_DispBuff* FD612_DispBuff){
  INT8U i,j;
  for(i=0;i<8;i++){
    if(Dat&(0x01<<i))
		{
			j=FD612_PotiveTNage_Bitmap[Addr][i].NegaAddr;
      FD612_DispBuff->DispData[FD612_PotiveTNage_Bitmap[Addr][i].NegaAddr]|=FD612_PotiveTNage_Bitmap[Addr][i].BitAddr;
		}
    else
      FD612_DispBuff->DispData[FD612_PotiveTNage_Bitmap[Addr][i].NegaAddr]&=(~FD612_PotiveTNage_Bitmap[Addr][i].BitAddr);   
  }
}
 


  
  /**
 *  @brief FD612 某个显示地址开始显示相应的字符串
 *  @param  Addr 字符串显示的起始地址
 *  @param  PStr 指向相应的字符串
 *  @retval INT8U  
 *  @return 返回函数执行结果,0为执行成功，1为起始地址超出最大地址
 *  @note   使用该函数需要打开 FD612_DECODE_TAB_EN 
 *  @par Example
 *  @code
 *  FD612_DispString(FD612_DIG3_ADDR,"FD612");
 *  @endcode 
 */ 
#if FD612_DECODE_TAB_EN!=0
INT8U FD612_DispString( INT8U Addr,char *PStr)
{
	INT8U i;
	if (Addr>FD612_DISP_MAX_ADDR)return 1;
	FD612_Command(FD612_ADDR_INC_DIGWR_CMD);	
	FD612_Start();		
	FD612_WrByte(FD612_DIGADDR_WRCMD|Addr) ; 
	for(i=0;i+Addr<=FD612_DISP_MAX_ADDR;i++)
	{
		if(PStr[i]=='\0')break;//判断是否到达字符串的尾部
		FD612_DispBuff.DispData[i+Addr]=DispGetCode(PStr[i]);
		FD612_WrByte(FD612_DispBuff.DispData[i+Addr]) ; 
	}
	FD612_Stop();
	return 0;
}
#endif
/* @} FD612_NG_API */
#endif
/**
 *  @brief  初始化FD612
 *  @param  void
 *  @return void
 *  @note 用户程序开始时调用该函数对FD612进行初始化
 *  @note 用户可以根据需要进行修改
 *  @par Example
 *  @code 共阴参考
  INT8U i;
  FD612_8SEG_MODE;
  for(i=0;i<12;i++){
    FD612_DispBuff.DispData[i]= 0X00;
  }
   FD612_DispBuff.DispData[FD612_DIG1_ADDR]=FD612_DISP_F; 
   FD612_DispBuff.DispData[FD612_DIG2_ADDR]=FD612_DISP_d;   
  FD612_DispString(FD612_DIG3_ADDR,"612");
  FD612_DispStateWr(FD612_INTENS8|FD612_DISP_ON);  
  FD612_Refresh();
 *   @endcode 
 *   @code 共阳参考
  INT8U i;
  FD612_8SEG_MODE;
  for(i=0;i<12;i++){
    FD612_DispBuff.DispData[i]= 0X00;
  }
 FD612_PotiveTNage(FD612_DIG1_ADDR,FD612_DISP_F);
 FD612_PotiveTNage(FD612_DIG2_ADDR,FD612_DISP_d);  
  FD612_DispString(FD612_DIG3_ADDR,"612");
  FD612_DispStateWr(FD612_INTENS8|FD612_DISP_ON);  
  FD612_Refresh();
 *  @endcode  
 */ 
 
  void FD612_Init( t_fd612_ops* ops,FD612_Struct_DispBuff* FD612_DispBuff)//<共阳参考
{
	INT8U i;
  FD612_8SEG_MODE(ops,FD612_DispBuff);
  for(i=0;i<12;i++){
    FD612_DispBuff->DispData[i]= 0Xff;
  }

  FD612_DispStateWr(ops,FD612_DispBuff,FD612_INTENS8|FD612_DISP_ON); 
	//FD612_Refresh();
}
/* @} FD612_API_FUNTION */
/* @} FD612_FUNTION */
/* @} FD612_DRIVER */


/******************* (C) COPYRIGHT 2013 FDHISI *****END OF FILE****/