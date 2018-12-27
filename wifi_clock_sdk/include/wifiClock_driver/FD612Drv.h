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
#include <sdk_config.h>
  #ifdef  FD612DRV_GROBLE
  #define FD612_EXT 
  #else
  #define FD612_EXT  extern
  #endif

  
#if VERSION_DEMO
#define SCL	8
#define SDA	9
#endif
#if CUSTOMER_VERSION
//#erro xxxxxxxx
#define SCL	9
#define SDA	8
#endif

#if (CUSTOMER_VERSION || VERSION_DEMO)
#define SCL_BASE	14
#define SDA_BASE	16
#define SCL_MIDDLE 1
#define SDA_MIDDLE 0
#endif

#if VERSION_MUSHAN
/* A */
#define SCL	9
#define SDA	8
#define SCL_B SCL
#define SDA_B SDA
#define SCL_BASE	16
#define SDA_BASE	14
#define SCL_A SCL_BASE
#define SDA_A SDA_BASE
#endif
#define FD628_NEGA_DISP       1///<1Ϊ��������ܣ�0Ϊ���������
#define	FD612_DECODE_TAB_EN   1///<�Ƿ���FD612_DecodeTab[]���� 0Ϊ�ر�,�رոù��ܿ���ʡCODE
#define	FD612_COMBINA_SW_EN     1 ///<�Ƿ�����ϼ����� 0Ϊ�ر�,�رոù��ܿ���ʡRAM

#undef BOOLEAN
typedef char BOOLEAN ;            ///<���岼������
#undef INT8U
typedef unsigned char INT8U;  ///<�����޷���8λ������
#undef INT32U
typedef  unsigned int INT32U ;  ///<�����޷���32λ������
#define CONST        const ///<���峣��
/********FD612.C���ڲ��궨�� *********/
/*����IO�����ã�����ƽ̨IO������*/

typedef void (*fd612_ops_cb)(void);
typedef int (*read_data_cb)();
typedef struct FD612_ops{
	fd612_ops_cb FD612_SCL_D_OUT;
	fd612_ops_cb FD612_SDA_D_OUT;
	fd612_ops_cb FD612_SDA_D_IN;
	read_data_cb FD612_SDA_IN;
	fd612_ops_cb FD612_SCL_SET;
	fd612_ops_cb FD612_SCL_CLR;
	fd612_ops_cb FD612_SDA_SET;
	fd612_ops_cb FD612_SDA_CLR;
}t_fd612_ops;

/*
#define		FD612_SCL_D_OUT				pin_mode(SCL,1)						///< ����SCL�ӿ�Ϊ��������������Ϊ��©��������������ó�Ϊ���������		
#define		FD612_SDA_D_OUT                 		pin_mode(SDA,1)   						 ///< ����SDA�ӿ�Ϊ��������������Ϊ��©��������������ó�Ϊ���������		
#define		FD612_SDA_D_IN					pin_mode(SDA,0)			      ///< ����SDA�ӿ�Ϊ���뷽���������Ϊ�������룬���������ó�Ϊ���������
#define		FD612_SDA_IN		   			digital_read(SDA)///< ��SDA��Ϊ���뷽��ʱ����ȡ���ĵ�ƽֵ
#define		FD612_SCL_SET		  			digital_write(SCL,1)///< ��SCL����Ϊ�ߵ�ƽ
#define		FD612_SCL_CLR	   				digital_write(SCL,0)						///< ��SCL����Ϊ�͵�ƽ
#define		FD612_SDA_SET		   			digital_write(SDA,1)						///< ��SDA����Ϊ�ߵ�ƽ
#define		FD612_SDA_CLR		  			digital_write(SDA,0)						///< ��SDA����Ϊ�͵�ƽ
*/
#define  	FD612_DELAY_1US  			    udelay(10)///< ��ʱ�ĺ궨�壬��С��ʱʱ��Ϊ1uS

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
extern const unsigned char intenseTable[8];
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

#define FD612_8SEG_MODE(ops,FD612_DispBuff)   do{ \
                          FD612_DispBuff->DispSEG_MODE=FD612_8SEG_CMD;   \
                          FD612_Command(ops,FD612_8SEG_CMD);  \
                          } while(0)
                     
#define FD612_7SEG_MODE(FD612_DispBuff) do{  \
                          (FD612_DispBuff)->DispSEG_MODE=FD612_7SEG_CMD;   \               \
                          FD612_Command(FD612_7SEG_CMD);  \
                          }while(0)
                       
#define FD612_DispStateWr(ops,FD612_DispBuff,DispStateTemp)  do{  \
                                        (FD612_DispBuff)->DispState=DispStateTemp; \
                                        FD612_Command(ops,DispStateTemp); \
                                        }while(0)

FD612_EXT	CONST  INT8U DISP_TAB[0x10]; ///<�������ֵ��
typedef struct {
	INT8U DispData[FD612_DISP_MAX_ADDR-FD612_DISP_MIN_ADDR+1]; ///<12λ��ʾ���ݻ�����
	INT8U DispState;      ///<��ʾ���ȺͿ���״̬������
	INT8U DispSEG_MODE;   ///<��ʾλ��ģʽ������
} FD612_Struct_DispBuff;

//FD612_EXT FD612_Struct_DispBuff FD612_DispBuff;
void FD612_Command(t_fd612_ops* ops,INT8U CMD ); //����һ�ֽڵ�����
void FD612_Init(t_fd612_ops* ops,FD612_Struct_DispBuff* FD612_DispBuff);  //��ʼ��FD612оƬ
#if FD612_DECODE_TAB_EN != 0
INT8U DispGetCode(char cTemp);  //����ܵĲ����
INT8U FD612_DispString( INT8U Addr,char *PStr); //ĳ����ʾ��ַ��ʼ��ʾ��Ӧ���ַ���
#endif
#if FD628_NEGA_DISP==1
void FD612_SingleDisp(INT8U addr,INT8U dat); //���������ĳ����ʾ��ַд��һ�ֽڵ���ʾ����
#else
void FD612_PotiveTNage(INT8U addr,INT8U dat); //���������ĳ����ʾ��ַд��һ�ֽڵ���ʾ����
#endif
void FD612_Base_Refresh(FD612_Struct_DispBuff* FD612_DispBuff,t_fd612_ops* ops); //ˢ��FD612���������
void FD612_Top_Refresh(FD612_Struct_DispBuff* FD612_DispBuff,t_fd612_ops* ops); //ˢ��FD612���������
void FD612_DispDataRefresh(FD612_Struct_DispBuff* FD612_DispBuff,t_fd612_ops* ops);  //ˢ��FD612����ʾ����



#endif
/******************* (C) COPYRIGHT 2013 FDHISI *****END OF FILE****/