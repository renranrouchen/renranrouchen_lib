
#include  <FD668.H>



void delay(int count) //延时21.6us
{
	int i;
	for(i=1;i<=count;i++)
	;
	
}

void Delay10us()		//@11.0592MHz
{
	delay(1);
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

uint8 FD668_DispData[14]={0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
/* 显示数据寄存器,调用FD668_WrDisp_AddrINC（）前，先将数据写入FD668_DispData[]的相应位置。*/  

											
/****************************************************************
 *	函数的名称:				FD668_Start
 *	描述:					FD668通信的起始准备
 *	参数：					void
 *	返回值:					void
****************************************************************/
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
	FD668_DIO_D_IN;						  /* 设置DIO为输入方向 */
	FD668_DELAY_BUF;					  /* 通信结束到下一次通信开始的间隔 */
}
/****************************************************************
 *	函数的名称:				FD668_WrByte
 *	描述:					向FD668写入一个字节的数据
 *	参数：					uint8  发送的数据
 *	返回值:					void
 *	注意:					数据从低位到高位传输  
****************************************************************/
static void FD668_WrByte( uint8 dat )
{
	uint8 i;				        			/* 移位控制变量 */
	FD668_DIO_D_OUT;		        			/* 设置DIO为输出方向 */
	for( i = 0; i != 8; i++ )	   				/* 输出8 bit的数据 */        
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
  	FD668_DELAY_LOW;							/* 时钟低电平时间 */	
		FD668_CLK_SET;							/* 设置SCL为高电平 上升沿写入*/
		dat >>= 1;								/* 输出数据右移一位，数据从低到高的输出 */
		FD668_DELAY_HIGH;          				/* 时钟高电平时间 */
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
/****************************************************************
 *	函数的名称:					    FD668_GetKey
 *	描述:							读按键码值
 *	参数:			             	void
 *	返回值:					        uint32 返回按键值 
 **************************************************************************************************************************************
返回的按键值编码  
				| 0			| 0			| 0			| 0			| 0			| 0			| KS10	| KS9		| KS8		| KS7		| KS6		| KS5		| KS4		| KS3		| KS2		| KS1		|
KEYI1 	| bit15	| bit14	| bit13	| bit12	| bit11	| bit10	| bit9	| bit8	| bit7	| bit6	| bit5	| bit4	| bit3	| bit2	| bit1	| bit0	| 
KEYI2 	| bit31	| bit30	| bit29	| bit28	| bit27	| bit26	| bit25	| bit24	| bit23	| bit22	| bit21	| bit20	| bit19	| bit18	| bit17	| bit16	|
***************************************************************************************************************************************/
//uint32 FD668_GetKey(void)
//{
//	uint8 i,KeyDataTemp;
//	uint32 FD668_KeyData=0;
//	FD668_Start();
//	FD668_WrByte(FD668_KEY_RDCMD);
//	for(i=0;i!=5;i++)
//	{
//		KeyDataTemp=FD668_RdByte();					   /*将5字节的按键码值转化成2字节的码值*/
//		if(KeyDataTemp&0x01)	 FD668_KeyData|=(0x00000001<<i*2);
//		if(KeyDataTemp&0x02)	 FD668_KeyData|=(0x00010000<<i*2);
//		if(KeyDataTemp&0x08)	 FD668_KeyData|=(0x00000002<<i*2);
//		if(KeyDataTemp&0x10)	 FD668_KeyData|=(0x00020000<<i*2);
//	}
//	FD668_Stop();
//	return(FD668_KeyData);
//}
/****************************************************************
 *	函数的名称:					    FD668_WrDisp_AddrINC
 *	描述:							以地址递增模式发送显示内容
 *	参数:		         			uint8 Addr发送显示内容的起始地址；具体地址和显示对应的表格见datasheet
 *									uint8 DataLen 发送显示内容的位数
 *	返回值:				        	boolean；如果地址超出将返回1；如果执行成功返回0。
 *  使用方法：						先将数据写入FD668_DispData[]的相应位置，再调用FD668_WrDisp_AddrINC（）函数。
****************************************************************/

char FD668_WrDisp_AddrINC(uint8 Addr,uint8 DataLen )
{
	uint8 i;
	if(DataLen+Addr>14) return(1);
	FD668_Command(FD668_ADDR_INC_DIGWR_CMD);
	FD668_Start();
	FD668_WrByte(FD668_DIGADDR_WRCMD|Addr);
	for(i=Addr;i!=(Addr+DataLen);i++)
	{
		FD668_WrByte(FD668_DispData[i]);
	}
	FD668_Stop();
	return(0);
}

#if 0
/****************************************************************
 *	函数的名称:				FD668_WrDisp_AddrStatic
 *	描述:					以地址固定模式发送显示内容 ;地址表看datasheet
 *	参数:		            uint8 Addr发送显示内容的地址；
 *							uint8 DIGData 写入显示内容
 *	返回值:				    boolean；如果地址超出将返回1；如果执行成功返回0。
****************************************************************/
 boolean FD668_WrDisp_AddrStatic(uint8 Addr,uint8 DIGData )
{
	if(Addr>=14) return(1);
	FD668_Command(FD668_ADDR_STATIC_DIGWR_CMD);
	FD668_Start();
	FD668_WrByte(FD668_DIGADDR_WRCMD|Addr);
	FD668_WrByte(DIGData);
	FD668_Stop();
	return(0);
}
#endif

/****************************************************************
 *	函数的名称:				FD668_Init
 *	描述:					FD668初始化，用户可以根据需要修改显示
 *	参数:		            void
 *	返回值:				    void
****************************************************************/ 
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