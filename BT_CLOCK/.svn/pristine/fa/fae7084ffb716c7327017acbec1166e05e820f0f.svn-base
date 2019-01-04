/* ***************************************************************************************** *
 *	公司名称	:		福大海矽微电子有限公司（FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD）
 *	创建人		：	袁文斌
 *	文件名		：	FD628_DRIVE.C
 *	功能描述	：	实现FD628的驱动
 *	其他说明	：	串行数据的传输从低位开始，FD628在串行通信的时钟上升沿读取数据，下降沿输出数据
 *	软件版本	：	V1B3（2012-10-17）
****************************************************************************************** */
#include "sdk_cfg.h"
#include <common/common.h>
#include "ui/FD628/FD628_Drive.H"
/* Check EN/DIS */
#if FD628_EN
unsigned char FD628_DispData [14] = {0x00};
/* 显示数据寄存器,调用FD628_WrDisp_AddrINC（）前，先将数据写入FD628_DispData[]的相应位置。*/
//const  INT8U NEGA_Table[0x10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7c,0x39,0x5E,0x79,0x71};
#define FD612_DECODE_TAB_NUM (14 * 4) ///<FD612_DecodeTab[]的字符个数
typedef struct
{
	unsigned char Character; ///<字符
	unsigned char Bitmap;     ///<字符对应的码值
} Struct_LED_Bitmap;
const Struct_LED_Bitmap FD612_DecodeTab[FD612_DECODE_TAB_NUM] = {
 	{'0', 0x3F}, {'1', 0x06}, {'2', 0x5B}, {'3', 0x4F},
	{'4', 0x66}, {'5', 0x6D}, {'6', 0x7D}, {'7', 0x07},
	{'8', 0x7F}, {'9', 0x6F}, {'a', 0x77}, {'A', 0x77},
	{'b', 0x7C}, {'B', 0x7C}, {'c', 0x39}, {'C', 0x39},
	{'d', 0x5E}, {'D', 0x5E}, {'e', 0x7b}, {'E', 0X79},
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
unsigned char DispGetCode(char cTemp)
{
	unsigned char i, Bitmap=0x00;
	for (i=0; i<FD612_DECODE_TAB_NUM; i++)
	{
		if(FD612_DecodeTab[i].Character == cTemp)
		{
			Bitmap = FD612_DecodeTab[i].Bitmap;
			break;
		}
	}
	return Bitmap;
}

static void FD628_Start( void )
{
	FD628_STB_CLR;  				  /* 设置STB为低电平 */
	FD628_STB_D_OUT;				  /* 设置STB为输出方向 */
	FD628_CLK_D_OUT;				  /* 设置CLK为输出方向 */
	FD628_DELAY_STB;
}
/****************************************************************
 *	函数的名称:			FD628_Stop
 *	描述:						FD628通信的结束准备
 *	参数：					void
 *	返回值:					void
****************************************************************/
static void FD628_Stop( void )
{
	FD628_CLK_SET;						  /* 设置CLK为高电平 */
	FD628_DELAY_STB;
	FD628_STB_SET;  					  /* 设置STB为高电平 */
	FD628_DIO_SET;						  /* 设置DIO为高电平 */
	FD628_DIO_D_IN;						  /* 设置DIO为输入方向 */
	FD628_DELAY_BUF;					  /* 通信结束到下一次通信开始的间隔 */
}
/****************************************************************
 *	函数的名称:			FD628_WrByte
 *	描述:						向FD628写入一个字节的数据
 *	参数：					INT8U  发送的数据
 *	返回值:					void
 *	注意:						数据从低位到高位传输
****************************************************************/
static void FD628_WrByte( unsigned char dat )
{
	unsigned char i;				        			/* 移位控制变量 */
	FD628_DIO_D_OUT;		        	/* 设置DIO为输出方向 */
	for( i = 0; i != 8; i++ )	    /* 输出8 bit的数据 */
	{
		FD628_CLK_CLR;					  	/* 设置CLK为低电平 */
		if( dat & 0x01 ) 						/* 数据从低位到高位输出 */
		{
		    FD628_DIO_SET;		    	/* 设置DIO为高电平 */
		}
		else
		{
		    FD628_DIO_CLR;					/* 设置DIO为低电平 */
		}
  	FD628_DELAY_LOW;						/* 时钟低电平时间 */
		FD628_CLK_SET;							/* 设置SCL为高电平 上升沿写入*/
		dat >>= 1;									/* 输出数据右移一位，数据从低到高的输出 */
		FD628_DELAY_HIGH;          	/* 时钟高电平时间 */
	}
}
/****************************************************************
 *	函数的名称:			FD628_RdByte
 *	描述:						从FD628读一个字节的数据
 *	参数：					void
 *	返回值:					INT8U 读到的数据
 *	注意:						数据从低位到高位传输
****************************************************************/
static unsigned char  FD628_RdByte( void )
{
	unsigned char i	,dat = 0;				 			/* 移位控制变量i;读取数据暂存变量dat */
	FD628_DIO_SET;		            /* 设置DIO为高电平 */
	FD628_DIO_D_IN;		       		  /* 设置DIO为输出方向 */
	for( i = 0; i != 8; i++ )	    /* 输出8 bit的数据 */
	{
		FD628_CLK_SET;					  	/* 设置CLK为低电平 */
 	  FD628_DELAY_LOW;						/* 时钟低电平时间 */
							 				/* 读入数据右移一位，数据从低到高的读入 */
		if( FD628_DIO_IN ) dat|=0X80;		/* 读入1 bit值 */
		FD628_CLK_CLR;							/* 设置CLK为高电平 */
		FD628_DELAY_HIGH;  					/* 时钟高电平时间 */
		dat >>= 1;
	}
	return dat;						 				/* 返回接收到的数据 */
}
/****************************************FD628操作函数*********************************************/
/****************************************************************
 *	函数的名称:					    FD628_Command
 *	描述:										发送控制命令
 *	参数:		             		INT8U 控制命令
 *	返回值:				    	    void
****************************************************************/
 void FD628_Command(unsigned char CMD)
{
	FD628_Start();
	FD628_WrByte(CMD);
	FD628_Stop();
}
/****************************************************************
 *	函数的名称:					    FD628_GetKey
 *	描述:										读按键码值
 *	参数:			             	void
 *	返回值:					        INT32U 返回按键值
 **************************************************************************************************************************************
返回的按键值编码
				| 0			| 0			| 0			| 0			| 0			| 0			| KS10	| KS9		| KS8		| KS7		| KS6		| KS5		| KS4		| KS3		| KS2		| KS1		|
KEYI1 	| bit15	| bit14	| bit13	| bit12	| bit11	| bit10	| bit9	| bit8	| bit7	| bit6	| bit5	| bit4	| bit3	| bit2	| bit1	| bit0	|
KEYI2 	| bit31	| bit30	| bit29	| bit28	| bit27	| bit26	| bit25	| bit24	| bit23	| bit22	| bit21	| bit20	| bit19	| bit18	| bit17	| bit16	|
***************************************************************************************************************************************/
unsigned char FD628_GetKey(void)
{
	unsigned char i,KeyDataTemp;
	unsigned int FD628_KeyData=0;
	FD628_Start();
	FD628_WrByte(FD628_KEY_RDCMD);
	for(i=0;i!=5;i++)
	{
		KeyDataTemp=FD628_RdByte();					   /*将5字节的按键码值转化成2字节的码值*/
		if(KeyDataTemp&0x01)	 FD628_KeyData|=(0x00000001<<i*2);
		if(KeyDataTemp&0x02)	 FD628_KeyData|=(0x00010000<<i*2);
		if(KeyDataTemp&0x08)	 FD628_KeyData|=(0x00000002<<i*2);
		if(KeyDataTemp&0x10)	 FD628_KeyData|=(0x00020000<<i*2);
	}
	FD628_Stop();
	return(FD628_KeyData);
}
/****************************************************************
 *	函数的名称:					    FD628_WrDisp_AddrINC
 *	描述:										以地址递增模式发送显示内容
 *	参数:		         				INT8U Addr发送显示内容的起始地址；具体地址和显示对应的表格见datasheet
 *													INT8U DataLen 发送显示内容的位数
 *	返回值:				        	BOOLEAN；如果地址超出将返回1；如果执行成功返回0。
 *  使用方法：						先将数据写入FD628_DispData[]的相应位置，再调用FD628_WrDisp_AddrINC（）函数。
****************************************************************/
unsigned char  FD628_WrDisp_AddrINC(unsigned char Addr,unsigned char DataLen )
{
	unsigned char i;
	if(DataLen+Addr>14) return(1);
	FD628_Command(FD628_ADDR_INC_DIGWR_CMD);
	FD628_Start();
	FD628_WrByte(FD628_DIGADDR_WRCMD|Addr);
	for(i=Addr;i!=(Addr+DataLen);i++)
	{
		FD628_WrByte(FD628_DispData[i]);
	}
	FD628_Stop();
	return(0);
}

/****************************************************************
 *	函数的名称:				FD628_WrDisp_AddrStatic
 *	描述:							以地址固定模式发送显示内容 ;地址表看datasheet
 *	参数:		          INT8U Addr发送显示内容的地址；
 *										INT8U DIGData 写入显示内容
 *	返回值:				    BOOLEAN；如果地址超出将返回1；如果执行成功返回0。
****************************************************************/
unsigned char FD628_WrDisp_AddrStatic(unsigned char Addr,unsigned char DIGData )
{
	if(Addr>=14) return(1);
	FD628_Command(FD628_ADDR_STATIC_DIGWR_CMD);
	FD628_Start();
	FD628_WrByte(FD628_DIGADDR_WRCMD|Addr);
	FD628_WrByte(DIGData);
	FD628_Stop();
	return(0);
}

char FD628_write_string(unsigned char* str)
{
	unsigned char addr = 0;
	for (;addr < 14;addr++) {
		FD628_WrDisp_AddrStatic(addr,str[addr]);
	}
	return 1;
}

/****************************************************************
 *	函数的名称:				FD628_Init
 *	描述:							FD628初始化，用户可以根据需要修改显示
 *	参数:		          void
 *	返回值:				    void
****************************************************************/
void FD628_Init(void)
{
	FD628_CLK_SET;						    /* 设置CLK为高电平 */
	FD628_STB_SET;  					    /* 设置STB为高电平 */
	FD628_DIO_SET;						    /* 设置DIO为高电平 */
	FD628_STB_D_OUT;				        /* 设置STB为输出方向 */
	FD628_CLK_D_OUT;				        /* 设置CLK为输出方向 */
	FD628_DIO_D_OUT;						/* 设置DIO为输入方向 */
	FD628_DELAY_BUF;					    /* 通信结束到下一次通信开始的间隔 */
	FD628_6DIG_MODE;
	FD628_Disp_Brightness_SET(FD628_DISP_NORMAL);
	FD628_WrDisp_AddrINC(0x00,14);
}
#endif
