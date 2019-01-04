/* ***************************************************************************************** *
 *	��˾����	:		������΢�������޹�˾��FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD��
 *	������		��	Ԭ�ı�
 *	�ļ���		��	FD628_DRIVE.C
 *	��������	��	ʵ��FD628������
 *	����˵��	��	�������ݵĴ���ӵ�λ��ʼ��FD628�ڴ���ͨ�ŵ�ʱ�������ض�ȡ���ݣ��½����������
 *	����汾	��	V1B3��2012-10-17��
****************************************************************************************** */
#include "sdk_cfg.h"
#include <common/common.h>
#include "ui/FD628/FD628_Drive.H"
/* Check EN/DIS */
#if FD628_EN
unsigned char FD628_DispData [14] = {0x00};
/* ��ʾ���ݼĴ���,����FD628_WrDisp_AddrINC����ǰ���Ƚ�����д��FD628_DispData[]����Ӧλ�á�*/
//const  INT8U NEGA_Table[0x10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7c,0x39,0x5E,0x79,0x71};
#define FD612_DECODE_TAB_NUM (14 * 4) ///<FD612_DecodeTab[]���ַ�����
typedef struct
{
	unsigned char Character; ///<�ַ�
	unsigned char Bitmap;     ///<�ַ���Ӧ����ֵ
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
	FD628_STB_CLR;  				  /* ����STBΪ�͵�ƽ */
	FD628_STB_D_OUT;				  /* ����STBΪ������� */
	FD628_CLK_D_OUT;				  /* ����CLKΪ������� */
	FD628_DELAY_STB;
}
/****************************************************************
 *	����������:			FD628_Stop
 *	����:						FD628ͨ�ŵĽ���׼��
 *	������					void
 *	����ֵ:					void
****************************************************************/
static void FD628_Stop( void )
{
	FD628_CLK_SET;						  /* ����CLKΪ�ߵ�ƽ */
	FD628_DELAY_STB;
	FD628_STB_SET;  					  /* ����STBΪ�ߵ�ƽ */
	FD628_DIO_SET;						  /* ����DIOΪ�ߵ�ƽ */
	FD628_DIO_D_IN;						  /* ����DIOΪ���뷽�� */
	FD628_DELAY_BUF;					  /* ͨ�Ž�������һ��ͨ�ſ�ʼ�ļ�� */
}
/****************************************************************
 *	����������:			FD628_WrByte
 *	����:						��FD628д��һ���ֽڵ�����
 *	������					INT8U  ���͵�����
 *	����ֵ:					void
 *	ע��:						���ݴӵ�λ����λ����
****************************************************************/
static void FD628_WrByte( unsigned char dat )
{
	unsigned char i;				        			/* ��λ���Ʊ��� */
	FD628_DIO_D_OUT;		        	/* ����DIOΪ������� */
	for( i = 0; i != 8; i++ )	    /* ���8 bit������ */
	{
		FD628_CLK_CLR;					  	/* ����CLKΪ�͵�ƽ */
		if( dat & 0x01 ) 						/* ���ݴӵ�λ����λ��� */
		{
		    FD628_DIO_SET;		    	/* ����DIOΪ�ߵ�ƽ */
		}
		else
		{
		    FD628_DIO_CLR;					/* ����DIOΪ�͵�ƽ */
		}
  	FD628_DELAY_LOW;						/* ʱ�ӵ͵�ƽʱ�� */
		FD628_CLK_SET;							/* ����SCLΪ�ߵ�ƽ ������д��*/
		dat >>= 1;									/* �����������һλ�����ݴӵ͵��ߵ���� */
		FD628_DELAY_HIGH;          	/* ʱ�Ӹߵ�ƽʱ�� */
	}
}
/****************************************************************
 *	����������:			FD628_RdByte
 *	����:						��FD628��һ���ֽڵ�����
 *	������					void
 *	����ֵ:					INT8U ����������
 *	ע��:						���ݴӵ�λ����λ����
****************************************************************/
static unsigned char  FD628_RdByte( void )
{
	unsigned char i	,dat = 0;				 			/* ��λ���Ʊ���i;��ȡ�����ݴ����dat */
	FD628_DIO_SET;		            /* ����DIOΪ�ߵ�ƽ */
	FD628_DIO_D_IN;		       		  /* ����DIOΪ������� */
	for( i = 0; i != 8; i++ )	    /* ���8 bit������ */
	{
		FD628_CLK_SET;					  	/* ����CLKΪ�͵�ƽ */
 	  FD628_DELAY_LOW;						/* ʱ�ӵ͵�ƽʱ�� */
							 				/* ������������һλ�����ݴӵ͵��ߵĶ��� */
		if( FD628_DIO_IN ) dat|=0X80;		/* ����1 bitֵ */
		FD628_CLK_CLR;							/* ����CLKΪ�ߵ�ƽ */
		FD628_DELAY_HIGH;  					/* ʱ�Ӹߵ�ƽʱ�� */
		dat >>= 1;
	}
	return dat;						 				/* ���ؽ��յ������� */
}
/****************************************FD628��������*********************************************/
/****************************************************************
 *	����������:					    FD628_Command
 *	����:										���Ϳ�������
 *	����:		             		INT8U ��������
 *	����ֵ:				    	    void
****************************************************************/
 void FD628_Command(unsigned char CMD)
{
	FD628_Start();
	FD628_WrByte(CMD);
	FD628_Stop();
}
/****************************************************************
 *	����������:					    FD628_GetKey
 *	����:										��������ֵ
 *	����:			             	void
 *	����ֵ:					        INT32U ���ذ���ֵ
 **************************************************************************************************************************************
���صİ���ֵ����
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
		KeyDataTemp=FD628_RdByte();					   /*��5�ֽڵİ�����ֵת����2�ֽڵ���ֵ*/
		if(KeyDataTemp&0x01)	 FD628_KeyData|=(0x00000001<<i*2);
		if(KeyDataTemp&0x02)	 FD628_KeyData|=(0x00010000<<i*2);
		if(KeyDataTemp&0x08)	 FD628_KeyData|=(0x00000002<<i*2);
		if(KeyDataTemp&0x10)	 FD628_KeyData|=(0x00020000<<i*2);
	}
	FD628_Stop();
	return(FD628_KeyData);
}
/****************************************************************
 *	����������:					    FD628_WrDisp_AddrINC
 *	����:										�Ե�ַ����ģʽ������ʾ����
 *	����:		         				INT8U Addr������ʾ���ݵ���ʼ��ַ�������ַ����ʾ��Ӧ�ı���datasheet
 *													INT8U DataLen ������ʾ���ݵ�λ��
 *	����ֵ:				        	BOOLEAN�������ַ����������1�����ִ�гɹ�����0��
 *  ʹ�÷�����						�Ƚ�����д��FD628_DispData[]����Ӧλ�ã��ٵ���FD628_WrDisp_AddrINC����������
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
 *	����������:				FD628_WrDisp_AddrStatic
 *	����:							�Ե�ַ�̶�ģʽ������ʾ���� ;��ַ��datasheet
 *	����:		          INT8U Addr������ʾ���ݵĵ�ַ��
 *										INT8U DIGData д����ʾ����
 *	����ֵ:				    BOOLEAN�������ַ����������1�����ִ�гɹ�����0��
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
 *	����������:				FD628_Init
 *	����:							FD628��ʼ�����û����Ը�����Ҫ�޸���ʾ
 *	����:		          void
 *	����ֵ:				    void
****************************************************************/
void FD628_Init(void)
{
	FD628_CLK_SET;						    /* ����CLKΪ�ߵ�ƽ */
	FD628_STB_SET;  					    /* ����STBΪ�ߵ�ƽ */
	FD628_DIO_SET;						    /* ����DIOΪ�ߵ�ƽ */
	FD628_STB_D_OUT;				        /* ����STBΪ������� */
	FD628_CLK_D_OUT;				        /* ����CLKΪ������� */
	FD628_DIO_D_OUT;						/* ����DIOΪ���뷽�� */
	FD628_DELAY_BUF;					    /* ͨ�Ž�������һ��ͨ�ſ�ʼ�ļ�� */
	FD628_6DIG_MODE;
	FD628_Disp_Brightness_SET(FD628_DISP_NORMAL);
	FD628_WrDisp_AddrINC(0x00,14);
}
#endif
