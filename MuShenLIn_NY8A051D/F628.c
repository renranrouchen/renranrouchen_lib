#include "F628.h"
#include <config.h>
#include <NY8A051D.h>

void delay(int count) //延时21.6us
{
	int i;
	for(i=1;i<=count;i++)
	;
}
void delay_us(unsigned int num)
{
	unsigned int kk = 0;
	for(kk = 0;kk<=num;kk++);
}

void Delay10us()		//@11.0592MHz
{
	delay_us(1);
}

void FD668_CHOOSE_STB_SET( )
{
	if (ChooseFlg) BS(PORTB,4); 
	else 		 			 BS(PORTB,2);
}

void FD668_CHOOSE_STB_CLR( )
{
	if (ChooseFlg) BC(PORTB,4);
	else 		 			 BC(PORTB,2);
}


uint8 FD668_DispData[7]={ 0X00};

static void FD668_Start( void )
{	
	FD668_STB_CLR;  				  /* 设置STB为低电平 */
	FD668_STB_D_OUT;				  /* 设置STB为输出方向 */	
	FD668_CLK_D_OUT;				  /* 设置CLK为输出方向 */	
	FD668_DELAY_STB;	
}

/****************************************************************
 *	函数的名称:				FD668_Stop
 *	描述:					FD668通信的结束准备
 *	参数：					void
 *	返回值:					void
****************************************************************/
static void FD668_Stop( void )
{  		
	FD668_CLK_SET;						  /* 设置CLK为高电平 */
	FD668_DELAY_STB;
	FD668_STB_SET;  					  /* 设置STB为高电平 */
	FD668_DIO_SET;						  /* 设置DIO为高电平 */
	//FD668_DIO_D_IN;						  /* 设置DIO为输入方向 */
	FD668_DELAY_BUF;					  /* 通信结束到下一次通信开始的间隔 */
}

/****************************************************************
 *	函数的名称:				FD668_WrByte
 *	描述:					向FD668写入一个字节的数据
 *	参数：					uint8  发送的数据
 *	返回值:					void
 *	注意:					数据从低位到高位传输  
****************************************************************/
 void FD668_WrByte( uint8 dat )
{
	unsigned char i;				        			/* 移位控制变量 */
	//FD668_DIO_D_OUT;		        			/* 设置DIO为输出方向 */
	for( i = 0; i < 8; i++ )	   				/* 输出8 bit的数据 */        
	{		
		FD668_CLK_CLR;					  		/* 设置CLK为低电平 */
		if( dat & 0x01 ) 						/* 数据从低位到高位输出 */
		{
		    FD668_DIO_SET;		    			/* 设置DIO为高电平 */
		}
		else 
		{
		    FD668_DIO_CLR;						/* 设置DIO为低电平 */
		}
		delay_us(1);
  	   // FD668_DELAY_LOW;							/* 时钟低电平时间 */	
		FD668_CLK_SET;							/* 设置SCL为高电平 上升沿写入*/
		dat >>= 1;								/* 输出数据右移一位，数据从低到高的输出 */
		delay_us(1);
		//FD668_DELAY_LOW;//FD668_DELAY_HIGH;          				/* 时钟高电平时间 */
	}	
}

/****************************************************************
 *	函数的名称:				FD668_RdByte
 *	描述:					从FD668读一个字节的数据
 *	参数：					void
 *	返回值:					uint8 读到的数据
 *	注意:					数据从低位到高位传输  
****************************************************************/

#if 0
static uint8  FD668_RdByte( void )
{
	uint8 i	,dat = 0;				 			/* 移位控制变量i;读取数据暂存变量dat */
	FD668_DIO_SET;		            			/* 设置DIO为高电平 */
	FD668_DIO_D_IN;		       		 			/* 设置DIO为输出方向 */
	for( i = 0; i != 8; i++ )	    			/* 输出8 bit的数据 */        
	{		
		FD668_CLK_CLR;					  		/* 设置CLK为低电平 */
 	    FD668_DELAY_LOW;						/* 时钟低电平时间 */
		dat >>= 1;					 			/* 读入数据右移一位，数据从低到高的读入 */
		if( FD668_DIO_IN ) dat|=0X80;			/* 读入1 bit值 */
		FD668_CLK_SET;							/* 设置CLK为高电平 */
		FD668_DELAY_HIGH;          				/* 时钟高电平时间 */
	}		
	return dat;						 			/* 返回接收到的数据 */
}	

#endif 



/****************************************FD668操作函数*********************************************/
/****************************************************************
 *	函数的名称:					    FD668_Command
 *	描述:							发送控制命令
 *	参数:		             		uint8 控制命令
 *	返回值:				    	    void
****************************************************************/
 void FD668_Command(uint8 CMD)
{
	FD668_Start();
	FD668_WrByte(CMD);
	FD668_Stop();
}

char FD668_WrDisp_AddrINC(uint8 Addr )
{
	uint8 i;
	if(Addr>14) return(1);
	FD668_Command(FD668_ADDR_INC_DIGWR_CMD);
	FD668_Start();
	FD668_WrByte(FD668_DIGADDR_WRCMD|Addr);
	for(i=Addr;i < 7;i++)
	{
		
		FD668_WrByte(FD668_DispData[i]);
		FD668_WrByte(0x00);
	}
	FD668_Stop();
	return(0);
}

#if 0
void FD668_Init(void )
{
	FD668_CLK_SET;						  /* 设置CLK为高电平 */
	FD668_STB_SET;  					  /* 设置STB为高电平 */
	FD668_DIO_SET;						  /* 设置DIO为高电平 */
	FD668_STB_D_OUT;				  	  /* 设置STB为输出方向 */	
	FD668_CLK_D_OUT;				      /* 设置CLK为输出方向 */	
	FD668_DIO_D_OUT;					  /* 设置DIO为输入方向 */
	FD668_DELAY_BUF;					  /* 通信结束到下一次通信开始的间隔 */
	FD668_7DIG_MODE;
	FD668_Disp_Brightness_SET(FD668_DISP_NORMAL);	
	FD668_DispData[0]=FD668_DISP_F_DATA;
	FD668_DispData[2]=FD668_DISP_D_DATA;
	FD668_DispData[4]=FD668_DISP_6_DATA;
	FD668_DispData[6]=FD668_DISP_2_DATA;
	FD668_DispData[8]=FD668_DISP_8_DATA;
	FD668_DispData[10]=FD668_DISP_NONE_DATA;
	FD668_DispData[12]=FD668_DISP_NONE_DATA;	
	FD668_WrDisp_AddrINC(0x00,14);			
}
#endif