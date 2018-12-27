/******************************************************************************
 * @file     FD612DRV.H
 * @brief    FD612������ͷ�ļ�
 * @version  1B3
 * @author   ywaby
 * @date     2014-03-31
 * @note
 * @copyright  Copyright (c)2013 by ������΢�������޹�˾��FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD��
 ******************************************************************************/
#ifndef _FD612DRV_H_
#define	_FD612DRV_H_
  #ifdef  FD612DRV_GROBLE
  #define FD612_EXT
  #else
  #define FD612_EXT  extern
  #endif
/** @addtogroup FD612_DRIVER FD612 ����
  * @{
*/
/** @addtogroup  FD612_REMOVE  ��ֲ��֪
  * @{
*/
/** @addtogroup  FD612_REMOVE_CHANGE  ��Ҫ�޸�
  * ����ƽ̨�������Ҫ������Ӧ���޸�
  * @{
*/
#define FD628_NEGA_DISP       0///<1Ϊ��������ܣ�0Ϊ���������
#define	FD612_DECODE_TAB_EN   1///<�Ƿ���FD612_DecodeTab[]���� 0Ϊ�ر�,�رոù��ܿ���ʡCODE
#define	FD612_COMBINA_SW_EN     1 ///<�Ƿ�����ϼ����� 0Ϊ�ر�,�رոù��ܿ���ʡRAM


typedef  unsigned char BOOLEAN ;            ///<���岼������
typedef  unsigned char INT8U ;  ///<�����޷���8λ������
typedef  unsigned char INT32U ;  ///<�����޷���32λ������
#define	CONST  const       ///<���峣��
/********FD612.C���ڲ��궨�� *********/
/*����IO�����ã�����ƽ̨IO������*/
#if (PRODUCT_VERSION_KEYI == 1)
#define SDA					8
#define SCK					9
#define FD612_SDA			(BIT(SDA) | BIT(3))
#define FD612_SCL			BIT(SCK)

#define  FD612_PORT_OUT     JL_PORTB->OUT
#define  FD612_PORT_IN      JL_PORTB->IN
#define  FD612_PORT_DIR     JL_PORTB->DIR
#define  FD612_PORT_PD      JL_PORTB->PD
#define  FD612_PORT_PU      JL_PORTB->PU
#endif
#if (ALL_FUNCTION_VERSION == 1)
#define SDA					4
#define SCK					5
#define FD612_SCL			BIT(SCK)
#define FD612_SDA			BIT(SDA)

#define  FD612_PORT_OUT     JL_PORTC->OUT
#define  FD612_PORT_IN      JL_PORTC->IN
#define  FD612_PORT_DIR     JL_PORTC->DIR
#define  FD612_PORT_PD      JL_PORTC->PD
#define  FD612_PORT_PU      JL_PORTC->PU
#endif


#define delay_1us(x)	delay((x * 120))

#define		FD612_SCL_D_OUT		(FD612_PORT_DIR &= ~FD612_SCL)
#define		FD612_SDA_D_OUT     (FD612_PORT_DIR &= ~FD612_SDA)                ///< ����SDA�ӿ�Ϊ��������������Ϊ��©��������������ó�Ϊ���������
#define		FD612_SDA_D_IN		(FD612_PORT_DIR |= FD612_SDA)			      ///< ����SDA�ӿ�Ϊ���뷽���������Ϊ�������룬���������ó�Ϊ���������
#define		FD612_SDA_IN		((FD612_PORT_IN) & FD612_SDA)			   	///< ��SDA��Ϊ���뷽��ʱ����ȡ���ĵ�ƽֵ
#define		FD612_SCL_SET		(FD612_PORT_OUT |= FD612_SCL)
#define		FD612_SCL_CLR		(FD612_PORT_OUT &= ~FD612_SCL)
#define		FD612_SDA_SET		(FD612_PORT_OUT |= FD612_SDA)	///< ��SDA����Ϊ�ߵ�ƽ
#define		FD612_SDA_CLR		(FD612_PORT_OUT &= ~FD612_SDA)	//< ��SDA����Ϊ�͵�ƽ
#define  	FD612_DELAY_1US  	delay_1us(2)   	              ///< ��ʱ�ĺ궨�壬��С��ʱʱ��Ϊ1uS
/* @} FD612_REMOVE_CHANGE */
/**
 * @addtogroup  FD612_REMOVE_CHANGEABLE   �����޸�
 * ����Ӧ����Ҫ������Ӧ���޸�
 * �����޸ĵĺ��� FD612_Init();FD612_Refresh();
 * @{
 */
/* @} FD612_REMOVE_CHANGEABLE */
/* @} FD612_REMOVE */
/** @addtogroup  FD612_APP Ӧ�����
  * @{
*/
/** @addtogroup  FD612_APP_MACRO   �궨��
  * @{
*/
/* *******************************************�����********************************************************************************
 *            a
 *         -------
 *        |       |
 *      f |       | b
 *         ---g---
 *        |       |	c
 *      e |       |
 *         ---d---   dp
 * *************************************************************************************************************************************** *
 *����		| 0		| 1		| 2		| 3		| 4		| 5		| 6		| 7		| 8		| 9		| A		| b		| C 	| d		| E		| F		|
 *����		|0x3F	|0x06	|0x5B	|0x4F	|0x66	|0x6D	|0x7D	|0x07	|0x7F	|0x6F	|0x77	|0x7c	|0x39	|0x5E	|0x79	|0x71	|
 ************************************************************************************************************************************* */
 /* ********�������ʾ�ַ��궨�� ******** */
#define		FD612_DISP_NONE 0X00
#define		FD612_DISP_0 0X3F
#define		FD612_DISP_1 0x06
#define		FD612_DISP_2 0x5B
#define		FD612_DISP_3 0x4F
#define		FD612_DISP_4 0x66
#define		FD612_DISP_5 0X6d
#define		FD612_DISP_6 0x7D
#define		FD612_DISP_7 0x07
#define		FD612_DISP_8 0x7f
#define		FD612_DISP_9 0x6F
#define		FD612_DISP_A 0X77
#define		FD612_DISP_b 0x7c
#define		FD612_DISP_C 0X39
#define		FD612_DISP_c 0X58
#define		FD612_DISP_d 0x5E
#define		FD612_DISP_E 0X79
#define		FD612_DISP_e 0X7b
#define		FD612_DISP_F 0x71
#define		FD612_DISP_I 0X60
#define		FD612_DISP_L 0X38
#define		FD612_DISP_r 0X72
#define		FD612_DISP_n 0X54
#define		FD612_DISP_N 0X37
#define		FD612_DISP_O 0X3F
#define		FD612_DISP_P 0XF3
#define		FD612_DISP_S 0X6d
#define		FD612_DISP_y 0X6e
#define		FD612_DISP__ 0x08
/********ACKλ********/
#define FD612_NACK 1///<��Ӧ���ź�
#define FD612_ACK  0///<��Ӧ���ź�

#define	FD612_DISP_MAX_ADDR 11    ///<FD612��ʾ��ַ�����ֵ
#define	FD612_DISP_MIN_ADDR 0     ///<FD612��ʾ��ַ����Сֵ
/** @addtogroup  FD612_APP_MACRO_CMD    ��������
  * @{
*/
//��ʾģʽ��������
#define FD612_8SEG_CMD	0x00		///<8��12λ
#define FD612_7SEG_CMD	0x03		///<7��10λ
//������������
#define FD612_ADDR_INC_DIGWR_CMD	      	0x40		    ///< �Զ���ַ���ӣ�д����
#define FD612_ADDR_STATIC_DIGWR_CMD			  0x4c		    ///< �̶���ַģʽ��д��ʾ
//��ַ��������
/**
 *  @brief д����ʾ���ݵĵ�ַ������
 *  @note  ʹ�÷�����FD612_DIGADDR_WRCMD|��Ӧ�ĵ�ַ
 *  @par Example
 *  @code
 *  FD612_Command(FD612_DIGADDR_WRCMD|FD612_DIG5_ADDR);
 *  @endcode
 */
#define FD612_DIGADDR_WRCMD	0xc0
//��ʾλ��Ӧ�ĵ�ַ
#define FD612_DIG1_ADDR 0x00
#define FD612_DIG2_ADDR 0x01
#define FD612_DIG3_ADDR 0x02
#define FD612_DIG4_ADDR 0x03
#define FD612_DIG5_ADDR 0x04
#define FD612_DIG6_ADDR 0x05
#define FD612_DIG7_ADDR 0x06
#define FD612_DIG8_ADDR 0x07
#define FD612_DIG9_ADDR 0x08
#define FD612_DIG10_ADDR 0x09
#define FD612_DIG11_ADDR 0x0A
#define FD612_DIG12_ADDR 0x0B
//��ʾ���Ⱥ���ʾ����֮���û�Ĺ�ϵ
#define FD612_INTENS1		0x80		///< 1������
#define FD612_INTENS2		0x81		///< 2������
#define FD612_INTENS3		0x82		///< 3������
#define FD612_INTENS4		0x83		///< 4������
#define FD612_INTENS5		0x84  	///< 5������
#define FD612_INTENS6		0x85		///< 6������
#define FD612_INTENS7		0x86		///< 7������
#define FD612_INTENS8		0x87		///< 8������

#define FD612_DISP_ON   0x88	  ///<��FD612��ʾ
#define FD612_DISP_OFF  0x80		///<�ر�FD612��ʾ
/* @} FD612_APP_MACRO_CMD */

/** @brief  FD612 8��12λģʽ��ʾ */
#define FD612_8SEG_MODE   do { \
                          FD612_DispBuff.DispSEG_MODE=FD612_8SEG_CMD;   \
                          FD612_Command(FD612_8SEG_CMD);  \
                          } while(0)
/** @brief  FD612 7��10λģʽ��ʾ */
#define FD612_7SEG_MODE do {  \
                          FD612_DispBuff.DispSEG_MODE=FD612_7SEG_CMD;   \               \
                          FD612_Command(FD612_7SEG_CMD);  \
                          }while(0)
/**
 *  @brief  FD612��ʾ���Ⱥ���ʾ���ؿ���
 *  @note   ��ʾ���Ⱥ���ʾ����֮��ʹ�û�Ĺ�ϵ
 *  @par Example
 *  @code
 *  FD612_DispStateWr(FD612_INTENS6|FD612_DISP_ON);
 *  @endcode
 */
#define FD612_DispStateWr(DispStateTemp)  do{  \
                                        FD612_DispBuff.DispState=DispStateTemp; \
                                        FD612_Command(DispStateTemp); \
                                        }while(0)

/* @} FD612_APP_MACRO */
/** @addtogroup  FD612_APP_DATA   �������
  * @{
*/
FD612_EXT	CONST  INT8U DISP_TAB[0x10]; ///<�������ֵ��
/* @} FD612_APP_DATA */
/** @addtogroup  FD612_APP_FUNC   ��غ���
  * @{
*/
void  FD612_Command(INT8U CMD ); //����һ�ֽڵ�����
void FD612_Init(void);  //��ʼ��FD612оƬ
#if FD612_DECODE_TAB_EN!=0
INT8U DispGetCode(char cTemp);  //����ܵĲ����
INT8U FD612_DispString( INT8U Addr,char *PStr); //ĳ����ʾ��ַ��ʼ��ʾ��Ӧ���ַ���
#endif
#if FD628_NEGA_DISP==1
void FD612_SingleDisp(INT8U addr,INT8U dat); //���������ĳ����ʾ��ַд��һ�ֽڵ���ʾ����
#else
void FD612_PotiveTNage(INT8U addr,INT8U dat); //���������ĳ����ʾ��ַд��һ�ֽڵ���ʾ����
#endif
void FD612_Refresh(void); //ˢ��FD612���������
void  FD612_DispDataRefresh(void);  //ˢ��FD612����ʾ����
char FD612_write_string(unsigned char Addr,unsigned char* str);
/* @} FD612_APP_FUNC */
/**
 * @addtogroup  FD612_APP_DATA   �������
 * @{
 */
typedef struct {
	INT8U DispData[FD612_DISP_MAX_ADDR-FD612_DISP_MIN_ADDR+1]; ///<12λ��ʾ���ݻ�����
	INT8U DispState;      ///<��ʾ���ȺͿ���״̬������
	INT8U DispSEG_MODE;   ///<��ʾλ��ģʽ������
} FD612_Struct_DispBuff;
/**
 *  @brief  ��ʾ���ݺ�״̬������
 *  @note   ����ͨ����FD612_DISP_BUFF_EN����͹رոù���
 *  @note   ʹ��ˢ�¹��ܱ����ȴ򿪸ù���
 */
FD612_EXT   FD612_Struct_DispBuff FD612_DispBuff;
/* @} FD612_APP_DATA */
/* @} FD612_APP */
/* @} FD612_DRIVER */
#endif
/******************* (C) COPYRIGHT 2013 FDHISI *****END OF FILE****/
