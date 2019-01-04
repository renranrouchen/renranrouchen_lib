/**
* @file 		  FD612Drv.C   	�������ʾ����оƬ �ͻ�����
* @brief       FD612 ������
* @author      ywaby
* @version     1B3
* @date 			2014-03-31
* @note
* @details
* @copyright Copyright (c) 2013 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.
*/
#define FD612DRV_GROBLE
#include "sdk_cfg.h"
#include <common/common.h>
#include "FD612Drv.H"
/** @addtogroup FD612_DRIVER FD612 ����
  @{
*/
#define 	FD612_DELAY_LOW		     	FD612_DELAY_1US         ///<ʱ�ӵ͵�ƽ��ʱ
#define		FD612_DELAY_HIGH     	 	FD612_DELAY_1US 	   		///<ʱ�Ӹߵ�ƽ��ʱ
#define  	FD612_DELAY_WAIT				FD612_DELAY_1US					///<���ݶ�ȡ�ȴ�ʱ��
#define		FD612_DELAY_SU_STA      FD612_DELAY_1US					///<��ʼ�źŽ���ʱ��
#define		FD612_DELAY_HD_STA      FD612_DELAY_1US					///<��ʼ�źű���ʱ��
#define		FD612_DELAY_SU_STO      FD612_DELAY_1US					///<ֹͣ�źŽ���ʱ��
										   /**
 * @brief �������ֵ��������,���ζ�Ӧ����ʾ��0,1,2,3,4,5,6,7,8,9,A,b,C,d,E,F
 */
#if 0
	CONST INT8U DISP_TAB[0X10] = {0x3f, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7d, 0x07, 0x7F, 0x6F, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};
#else //upside down
	CONST INT8U DISP_TAB[0X10] = {0x3F, 0x30, 0x5B, 0x79, 0x74, 0x6D, 0x6F, 0x38, 0x7F, 0x7D, 0x7E, 0x67, 0x0F, 0x73, 0X4F, 0x4E};
#endif

CONST INT8U DISP_POSITIVE_TABLE[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};
/** @addtogroup  FD612_FUNCTION  ����
  @{
*/
/** @addtogroup  FD612_COMMUNICATION  ͨ��ʱ��
  @{
*/
/**
 *  @brief  ����FD612ͨ��
 *  @param  void
 *  @return void
 *  @note   ��SCL�ߵ�ƽ�ڼ䲶��SDA���½��أ�FD612��ʼͨ��
 */
static void FD612_Start(void)
{
	FD612_SDA_D_OUT; //����SDAΪ�������
	FD612_SCL_D_OUT; //����SCLΪ�������
	FD612_SDA_SET;
	FD612_SCL_SET;
	FD612_DELAY_SU_STA;
	FD612_SDA_CLR; //������ʼ�ź�
	FD612_DELAY_HD_STA;
	FD612_SCL_CLR;
	FD612_DELAY_LOW;
}

/**
 *  @brief  ֹͣFD612ͨ��
 *  @param  void
 *  @return void
 *  @note   ��SCL�ߵ�ƽ�ڼ䲶��SDA�������أ�FD612ֹͣͨ��
 */
static void FD612_Stop(void)
{
	FD612_SCL_SET;		/* ����SCLΪ�ߵ�ƽ */
	FD612_SDA_D_OUT;	/* ����SDAΪ������� */
	FD612_SDA_CLR;		/* ����SDAΪ�͵�ƽ */
	FD612_DELAY_SU_STO; /* ֹͣ�źŽ���ʱ��: */
	FD612_SDA_SET;		/* ����SDAΪ�ߵ�ƽ */
	FD612_SDA_D_IN;		/* ����SDAΪ���뷽�� */
	FD612_DELAY_SU_STO;
}
/**
 *  @brief  ����һ���ֽ����ݸ�FD612,����ȡӦ��λ
 *  @param  dat ���͵�һ�ֽ�����
 *  @retval  BOOLEAN
 *  @return ��ȡ��ACKӦ��λ
 *  @note   ���ݴ����λ��ǰ����λ�ں�
 */
static BOOLEAN FD612_WrByte(INT8U dat)
{
	INT8U i;				        /* ��λд����λ���Ĵ��� */
	BOOLEAN ACKTemp = 0;
	FD612_SDA_D_OUT; /* ����SDAΪ������� */
	FD612_DELAY_LOW;
	for (i = 0; i != 8; i++) /* ���8 bit������ */
	{
		if (dat & 0x80)
		{
			FD612_SDA_SET; /* ����SDAΪ�ߵ�ƽ */
		}
		else
		{
			FD612_SDA_CLR; /* ����SDAΪ�͵�ƽ */
		}
		FD612_DELAY_LOW;
		FD612_SCL_SET;	/* ����SCLΪ�ߵ�ƽ */
		dat <<= 1;		  /* �����������һλ�����ݴӸߵ��͵���� */
		FD612_DELAY_HIGH; /* SCLʱ�Ӹߵ�ƽʱ�䣺*/
		FD612_SCL_CLR;	/* ����SCLΪ�͵�ƽ */
		FD612_DELAY_LOW;
	}
	//////��ȡACKλ/////
	FD612_SDA_SET;  /* ����SDAΪ�ߵ�ƽ */
	FD612_DELAY_LOW;
	FD612_SDA_D_IN; /* ����SDAΪ���뷽�� */
	FD612_SCL_SET;  /* SCLʱ�ӵĸߵ�ƽʱ�䣺*/
	if (FD612_SDA_IN == 0)
		ACKTemp = 0; /* ����1 bitֵ */
	else
		ACKTemp = 1;
	FD612_DELAY_HIGH; /* SCLʱ�Ӹߵ�ƽʱ��*/
	FD612_SCL_CLR;	/* SCLʱ�ӵĵ͵�ƽ*/
	FD612_DELAY_LOW;
	return ACKTemp;
}
/**
 *  @brief  ��FD612��ȡһ���ֽ�����
 *  @param  MSACK ���͵�ACKֵ ���Ͳ�Ӧ��λ ACK=1������Ӧ��λ ACK=0
 *  @retval  INT8U
 *  @return ��ȡ��һ�ֽ�����
 *  @note   ���ݴ����λ��ǰ����λ�ں�
 */
static INT8U FD612_RdByte(BOOLEAN MSACK)
{
	INT8U i;
	INT8U dat=0;                   /* ��λ�����λ���Ĵ���i */
	FD612_SDA_SET;		             /* ����SDAΪ�ߵ�ƽ */
	FD612_SDA_D_IN;		             /* ����SDAΪ���뷽�� */
	for( i = 0; i != 8; i++ )		 /* ����8 bit������ */
	{
		FD612_SCL_SET;	 /* ����SCLΪ�ߵ�ƽ */
		FD612_DELAY_HIGH; /* SCLʱ�Ӹߵ�ƽʱ�䣺*/
		dat <<= 1;		  /* ������������һλ�����ݴӸߵ��͵Ķ��� */
		if (FD612_SDA_IN)
			dat++;	 /* ����1 bitֵ */
		FD612_SCL_CLR; /* ����SCLΪ�͵�ƽ */
		FD612_DELAY_LOW;
	}
   //////����ACKλ/////
  if(MSACK==0)             /* ����ACKӦ��λ*/
		FD612_SDA_CLR;
	else
		FD612_SDA_SET;
	FD612_SDA_D_OUT;  /* ����SDAΪ������� */
	FD612_SCL_SET;	/* ����SCLΪ�ߵ�ƽ */
	FD612_DELAY_HIGH; /* SCL ʱ�Ӹߵ�ƽʱ�䣺*/
	FD612_SCL_CLR;	/* ����SCLΪ�͵�ƽ */
	FD612_DELAY_LOW;
	return dat;  /* ���ؽ��յ������� */
}
/* @} FD612_COMMUNICATION */
/** @addtogroup  FD612_API_FUNTION  Ӧ�ú���
  @{
*/
/**
 *  @brief  ��FD612����һ�ֽڵ�����
 *  @param  CMD ����һ�ֽڵ�����
 *  @return void
 *  @note   CMD�ǿ��������еĺ�
 *  @par Example
 *  @code
 *  FD612_Command(FD612_7SEG_CMD);
 *  @endcode
 */
void FD612_Command(INT8U CMD)
{
	FD612_Start();
	FD612_WrByte(CMD);
	FD612_Stop();
}
/**
 *  @brief  ˢ��FD612����ʾ����
 *  @param  void
 *  @return void
 *  @note   ʹ�øú�����Ҫ�� FD612_DISP_BUFF_EN
 */
void FD612_DispDataRefresh(void)
{
	INT8U i;
	FD612_Command(FD612_ADDR_INC_DIGWR_CMD);
	FD612_Start();
	FD612_WrByte(FD612_DIGADDR_WRCMD | FD612_DISP_MIN_ADDR);
	for (i = FD612_DISP_MIN_ADDR; i <= FD612_DISP_MAX_ADDR; i++)
		FD612_WrByte(FD612_DispBuff.DispData[i]);
	FD612_Stop();
}
/**
 *  @brief  ˢ��FD612���������
 *  @param  void
 *  @return void
 *  @note
 *  @note   ʹ�øú�����Ҫ�� FD612_DISP_BUFF_EN
 */
void FD612_Refresh(void)
{
	FD612_Command(FD612_DispBuff.DispSEG_MODE);
	FD612_DispDataRefresh();
	FD612_Command(FD612_DispBuff.DispState);
}
/* @} FD612_API_FUNTION */
#if FD612_DECODE_TAB_EN != 0

#define FD612_DECODE_TAB_NUM 14*4 ///<FD612_DecodeTab[]���ַ�����
typedef struct
{
	INT8U Character; ///<�ַ�
	INT8U Bitmap;	///<�ַ���Ӧ����ֵ
} Struct_LED_Bitmap; ///<����ܵ���ֵ���ַ��Ķ�Ӧ�ṹ�壬���ڲ��
///<����ܵ���ֵ���ַ��Ķ�Ӧ������ڲ��
#if 0
CONST Struct_LED_Bitmap FD612_DecodeTab[FD612_DECODE_TAB_NUM] = {
	{'0', 0x3F}, {'1', 0x06}, {'2', 0x5B}, {'3', 0x4F}, {'4', 0x66}, {'5', 0x6D}, {'6', 0x7D}, {'7', 0x07}, {'8', 0x7F}, {'9', 0x6F}, {'a', 0x77}, {'A', 0x77}, {'b', 0x7C}, {'B', 0x7C}, {'c', 0x39}, {'C', 0x39}, {'d', 0x5E}, {'D', 0x5E}, {'e', 0x7b}, {'E', 0X79}, {'f', 0x71}, {'F', 0x71}, {'g', 0x6F}, {'G', 0x3D}, {'h', 0x74}, {'H', 0x76}, {'i', 0x04}, {'I', 0x06}, {'j', 0x1E}, {'J', 0x1E}, {'l', 0x38}, {'L', 0x38}, {'n', 0x54}, {'N', 0x37}, {'o', 0x5C}, {'O', 0x3F}, {'p', 0x73}, {'P', 0x73}, {'q', 0x67}, {'Q', 0x67}, {'r', 0x50}, {'R', 0x77}, {'s', 0x6D}, {'S', 0x6D}, {'t', 0x78}, {'T', 0x31}, {'u', 0x3E}, {'U', 0x3E}, {'y', 0x6E}, {'Y', 0x6E}, {'z', 0x5B}, {'Z', 0x5B}, {':', 0xfe}, {'-', 0x40}, {'_', 0x08}, {' ', 0x00}};
#else //updside down
CONST Struct_LED_Bitmap FD612_DecodeTab[FD612_DECODE_TAB_NUM] = {
	{'0', 0x3F}, {'1', 0x30}, {'2', 0x5B}, {'3', 0x79}, {'4', 0x74}, {'5', 0x6D}, {'6', 0x6F}, {'7', 0x38}, {'8', 0x7F}, {'9', 0x7D}, {'a', 0x7E}, {'A', 0x7E}, {'b', 0x67}, {'B', 0x67}, {'c', 0x0F}, {'C', 0x0F}, {'d', 0x73}, {'D', 0x73}, {'e', 0x5F}, {'E', 0X4F}, {'f', 0x4E}, {'F', 0x4E}, {'g', 0x7D}, {'G', 0x2F}, {'h', 0x66}, {'H', 0x76}, {'i', 0x20}, {'I', 0x30}, {'j', 0x33}, {'J', 0x33}, {'l', 0x07}, {'L', 0x07}, {'n', 0x62}, {'N', 0x3E}, {'o', 0x63}, {'O', 0x3F}, {'p', 0x5E}, {'P', 0x5E}, {'q', 0x7C}, {'Q', 0x7C}, {'r', 0x42}, {'R', 0x7E}, {'s', 0x6D}, {'S', 0x6D}, {'t', 0x47}, {'T', 0x0E}, {'u', 0x37}, {'U', 0x37}, {'y', 0x75}, {'Y', 0x75}, {'z', 0x5B}, {'Z', 0x5B}, {':', 0x80}, {'-', 0x40}, {'_', 0x01}, {' ', 0x00}};
#endif

/**
 *  @brief  ����ܵĲ����
 *  @param  cTemp ���ҵ��ַ�
 *  @retval INT8U
 *  @return �ַ���Ӧ���������ֵ
 *  @note   ʹ�øú�����Ҫ�� FD612_DECODE_TAB_EN
 *  @par Example
 *  @code
 *  char CTmp;
 *  CTmp=DispGetCode('S');
 *  @endcode
 */

INT8U DispGetCode(char cTemp)
{
	INT8U i, Bitmap = 0x00;
	for (i = 0; i < FD612_DECODE_TAB_NUM; i++)
	{
		if (FD612_DecodeTab[i].Character == cTemp)
		{
			Bitmap = FD612_DecodeTab[i].Bitmap;
			break;
		}
	}
	return Bitmap;
}
#endif
#if FD628_NEGA_DISP == 0
typedef struct
{
	INT8U NegaAddr; ///<��Ӧ�Ĺ�����������ݵ�ַ
	INT8U BitAddr;     ///<��Ӧ�ε�ַ
} Struct_PotiveTNage_Bitmap; ///<����ܵ���ֵ���ַ��Ķ�Ӧ�ṹ�壬���ڲ��
///<������Ӧ�����ı�����ڲ��,���ݾ����Ӧ�õ�·�����޸�
CONST Struct_PotiveTNage_Bitmap FD612_PotiveTNage_Bitmap[12][8] = {
	{{0x04, 0x01}, {0x05, 0x01}, {0x06, 0x01}, {0x07, 0x01}, {0x08, 0x01}, {0x09, 0x01}, {0x0a, 0x01}, {0x0b, 0x01}},
	{{0x04, 0x02}, {0x05, 0x02}, {0x06, 0x02}, {0x07, 0x02}, {0x08, 0x02}, {0x09, 0x02}, {0x0a, 0x02}, {0x0b, 0x02}},
	{{0x04, 0x04}, {0x05, 0x04}, {0x06, 0x04}, {0x07, 0x04}, {0x08, 0x04}, {0x09, 0x04}, {0x0a, 0x04}, {0x0b, 0x04}},
	{{0x04, 0x08}, {0x05, 0x08}, {0x06, 0x08}, {0x07, 0x08}, {0x08, 0x08}, {0x09, 0x08}, {0x0a, 0x08}, {0x0b, 0x08}},

	{{0x00, 0x01}, {0x01, 0x01}, {0x02, 0x01}, {0x03, 0x01}, {0x08, 0x10}, {0x09, 0x10}, {0x0a, 0x10}, {0x0b, 0x10}},
	{{0x00, 0x02}, {0x01, 0x02}, {0x02, 0x02}, {0x03, 0x02}, {0x08, 0x20}, {0x09, 0x20}, {0x0a, 0x20}, {0x0b, 0x20}},
	{{0x00, 0x04}, {0x01, 0x04}, {0x02, 0x04}, {0x03, 0x04}, {0x08, 0x40}, {0x09, 0x40}, {0x0a, 0x40}, {0x0b, 0x40}},
	{{0x00, 0x08}, {0x01, 0x08}, {0x02, 0x08}, {0x03, 0x08}, {0x08, 0x80}, {0x09, 0x80}, {0x0a, 0x80}, {0x0b, 0x80}},

	{{0x00, 0x10}, {0x01, 0x10}, {0x02, 0x10}, {0x03, 0x10}, {0x04, 0x10}, {0x05, 0x10}, {0x06, 0x10}, {0x07, 0x10}},
	{{0x00, 0x20}, {0x01, 0x20}, {0x02, 0x20}, {0x03, 0x20}, {0x04, 0x20}, {0x05, 0x20}, {0x06, 0x20}, {0x07, 0x20}},
	{{0x00, 0x40}, {0x01, 0x40}, {0x02, 0x40}, {0x03, 0x40}, {0x04, 0x40}, {0x05, 0x40}, {0x06, 0x40}, {0x07, 0x40}},
	{{0x00, 0x80}, {0x01, 0x80}, {0x02, 0x80}, {0x03, 0x80}, {0x04, 0x80}, {0x05, 0x80}, {0x06, 0x80}, {0x07, 0x80}}};

/** @addtogroup FD612_API_FUNTION
  @{
*/

/** @addtogroup FD612_PT_API ���������Ӧ�ú���
  @{
*/
/**
 *  @brief  ��������ܵ�ĳһλд����ʾ����
 *  @param  Addr ��������ܵ�λ
 *  @param  Dat   ��ʾ������
 *  @retval void
 *  @return
 *  @note   ʹ�øú�����Ҫ�� FD612_DECODE_TAB_EN FD612_DISP_BUFF_EN FD628_NEGA_DISP
 *  @par Example
 *  @code
 *  PotiveTNage(1,0X3F); //��һλ�������ʾ��0��
 *  @endcode
 */
void FD612_PotiveTNage(INT8U Addr, INT8U Dat)
{
	INT8U i;
	for (i = 0; i < 8; i++)
	{
		if (Dat & (0x01 << i))
			FD612_DispBuff.DispData[FD612_PotiveTNage_Bitmap[Addr][i].NegaAddr] |= FD612_PotiveTNage_Bitmap[Addr][i].BitAddr;
		else
			FD612_DispBuff.DispData[FD612_PotiveTNage_Bitmap[Addr][i].NegaAddr] &= (~FD612_PotiveTNage_Bitmap[Addr][i].BitAddr);
	}
}
	/**
 *  @brief FD612 ĳ����ʾ��ַ��ʼ��ʾ��Ӧ���ַ���
 *  @param  Addr �ַ�����ʾ����ʼ��ַ
 *  @param  PStr ָ����Ӧ���ַ���
 *  @retval INT8U
 *  @return ���غ���ִ�н��,0Ϊִ�гɹ���1Ϊ��ʼ��ַ��������ַ
 *  @note   ʹ�øú�����Ҫ�� FD612_DECODE_TAB_EN
 *  @par Example
 *  @code
 *  FD612_DispString(FD612_DIG3_ADDR,"FD612");
 *  @endcode
 */
#if FD612_DECODE_TAB_EN != 0
INT8U FD612_DispString(INT8U Addr, char *PStr)
{
	INT8U i;
	if (Addr > FD612_DISP_MAX_ADDR)
		return 1;
	for (i = 0; i + Addr <= FD612_DISP_MAX_ADDR; i++)
	{
		if (PStr[i] == '\0')
			break; //�ж��Ƿ񵽴��ַ�����β��
		FD612_PotiveTNage(i + Addr, DispGetCode(PStr[i]));
	}
	FD612_Refresh();
	return 0;
}
#endif
#else
/* @} FD612_PT_API */
/** @addtogroup FD612_NG_API ���������Ӧ�ú���
  @{
*/
/**
 *  @brief  ���������ĳ����ʾ��ַд��һ�ֽڵ���ʾ����
 *  @param  Addr      д�����ʾ��ַ
 *  @param  DispData  д�����ʾ����
 *  @return void
 *  @note ��ַ�̶���ʽд����ʾ����
 *  @par Example
 *  @code
 *  FD612_SingleDisp(FD612_DIG3_ADDR,FD612_DISP_0);
 *  @endcode
 */
void FD612_SingleDisp(INT8U Addr, INT8U DispData)
{
	FD612_Command(FD612_ADDR_STATIC_DIGWR_CMD);
	FD612_DispBuff.DispData[Addr] = DispData;
	FD612_Start();
	FD612_WrByte(FD612_DIGADDR_WRCMD | Addr);
	FD612_WrByte(DispData);
	FD612_Stop();
}

	/**
 *  @brief FD612 ĳ����ʾ��ַ��ʼ��ʾ��Ӧ���ַ���
 *  @param  Addr �ַ�����ʾ����ʼ��ַ
 *  @param  PStr ָ����Ӧ���ַ���
 *  @retval INT8U
 *  @return ���غ���ִ�н��,0Ϊִ�гɹ���1Ϊ��ʼ��ַ��������ַ
 *  @note   ʹ�øú�����Ҫ�� FD612_DECODE_TAB_EN
 *  @par Example
 *  @code
 *  FD612_DispString(FD612_DIG3_ADDR,"FD612");
 *  @endcode
 */
#if FD612_DECODE_TAB_EN != 0
INT8U FD612_DispString(INT8U Addr, char *PStr)
{
	INT8U i;
	if (Addr > FD612_DISP_MAX_ADDR)
		return 1;
	FD612_Command(FD612_ADDR_INC_DIGWR_CMD);
	FD612_Start();
	FD612_WrByte(FD612_DIGADDR_WRCMD | Addr);
	for (i = 0; i + Addr <= FD612_DISP_MAX_ADDR; i++)
	{
		if (PStr[i] == '\0')
			break; //�ж��Ƿ񵽴��ַ�����β��
		FD612_DispBuff.DispData[i + Addr] = DispGetCode(PStr[i]);
		FD612_WrByte(FD612_DispBuff.DispData[i + Addr]);
	}
	FD612_Stop();
	return 0;
}
#endif

/* @} FD612_NG_API */
#endif
char FD612_write_string(unsigned char Addr, unsigned char *str)
{
	INT8U i;
	if (Addr > FD612_DISP_MAX_ADDR)
		return 1;
	FD612_Command(FD612_ADDR_INC_DIGWR_CMD);
	FD612_Start();
	FD612_WrByte(FD612_DIGADDR_WRCMD | Addr);
	for (i = 0; i + Addr <= FD612_DISP_MAX_ADDR; i++)
	{
		FD612_DispBuff.DispData[i + Addr] = str[i];
		FD612_WrByte(FD612_DispBuff.DispData[i + Addr]);
	}
	FD612_Stop();
	return 0;
}
/**
 *  @brief  ��ʼ��FD612
 *  @param  void
 *  @return void
 *  @note �û�����ʼʱ���øú�����FD612���г�ʼ��
 *  @note �û����Ը�����Ҫ�����޸�
 *  @par Example
 *  @code �����ο�
  INT8U i;
  FD612_8SEG_MODE;
  for(i=0;i<12;i++) {
    FD612_DispBuff.DispData[i]= 0X00;
  }
  FD612_DispBuff.DispData[FD612_DIG1_ADDR]=FD612_DISP_F;
  FD612_DispBuff.DispData[FD612_DIG2_ADDR]=FD612_DISP_d;
  FD612_DispString(FD612_DIG3_ADDR,"612");
  FD612_DispStateWr(FD612_INTENS8|FD612_DISP_ON);
  FD612_Refresh();
 *   @endcode
 *   @code �����ο�
  INT8U i;
  FD612_8SEG_MODE;
  for(i=0;i<12;i++) {
    FD612_DispBuff.DispData[i]= 0X00;
  }
  FD612_PotiveTNage(FD612_DIG1_ADDR,FD612_DISP_F);
  FD612_PotiveTNage(FD612_DIG2_ADDR,FD612_DISP_d);
  FD612_DispString(FD612_DIG3_ADDR,"612");
  FD612_DispStateWr(FD612_INTENS8|FD612_DISP_ON);
  FD612_Refresh();
 *  @endcode
 */

  void FD612_Init( void)//<�����ο�
{
	INT8U i;
	FD612_8SEG_MODE;
	for (i = 0; i < 12; i++)
	{
		FD612_DispBuff.DispData[i] = 0x00;
	}
	FD612_DispStateWr(FD612_INTENS8 | FD612_DISP_ON);
	//	for(i=0;i<12;i++) {
	//    	FD612_SingleDisp(i,0xAA);
	//    }
	FD612_Refresh();
}
/* @} FD612_API_FUNTION */
/* @} FD612_FUNTION */
/* @} FD612_DRIVER */

/******************* (C) COPYRIGHT 2013 FDHISI *****END OF FILE****/
