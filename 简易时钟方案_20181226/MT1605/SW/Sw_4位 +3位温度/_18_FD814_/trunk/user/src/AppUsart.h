#ifndef _AppUsart_H_
#define _AppUsart_H_

#include "myinclude.h"

// #define I2C_CONTINUOUS_MODE	//I2C����дģʽ
 #define STC12LE5A60S2

 #define DIS_LENGTH 0x08
 #define WAVE_NUM 12				//��ʾ������
 #define I2C_ADDER_TYPE0	0x00	//�ʺ�FD6xxϵ��, ��ַ+�Ĵ���, ��ַ�ۼ�2
 #define I2C_ADDER_TYPE1	0x01	//�ʺ�FD5XXϵ��, ������ַ+�Ĵ�����ַ+�Ĵ���, ��ַ�ۼ�1
 
 // #define FD5012_NEW
 #define FD5012_OLD
 
 #ifdef FD5012_NEW
 #define FD5012_ADDR_CNTR0 0x08
 #define FD5012_ADDR_CNTR1 0x09
 #define FD5012_ADDR_WRPOLY 0x0a
 #define FD5012_ADDR_RDPOLY 0x0b
 #define FD5012_ADDR_MSDIFF 0x0c
 #define FD5012_ADDR_BDV 0x0d
 #define FD5012_ADDR_KEYBUF 0x0e
 
 #define FD5012_ADDER_KDV0  0x10	//������ֵ
 #define FD5012_ADDER_KDV1  0x11
 #define FD5012_ADDER_KDV2  0x12
 #define FD5012_ADDER_KDV3  0x13
 #define FD5012_ADDER_KDV4  0x14
 #define FD5012_ADDER_KDV5  0x15
 #define FD5012_ADDER_KDV6  0x16
 #define FD5012_ADDER_KDV7  0x17
 #define FD5012_ADDER_KDV8  0x18
 #define FD5012_ADDER_KDV9  0x19
 #define FD5012_ADDER_KDV10 0x1a
 #define FD5012_ADDER_KDV11 0x1b
 #define FD5012_ADDER_KDV12 0x1c
 #define FD5012_ADDER_KDV13 0x1d
 #define FD5012_ADDER_KDV14 0x1e
 #define FD5012_ADDER_KDV15 0x1f 
 
 #define FD5012_ADDER_KUV0  0x20	//�ɿ���ֵ
 #define FD5012_ADDER_KUV1  0x21
 #define FD5012_ADDER_KUV2  0x22
 #define FD5012_ADDER_KUV3  0x23
 #define FD5012_ADDER_KUV4  0x24
 #define FD5012_ADDER_KUV5  0x25
 #define FD5012_ADDER_KUV6  0x26
 #define FD5012_ADDER_KUV7  0x27
 #define FD5012_ADDER_KUV8  0x28
 #define FD5012_ADDER_KUV9  0x29
 #define FD5012_ADDER_KUV10 0x2a
 #define FD5012_ADDER_KUV11 0x2b
 #define FD5012_ADDER_KUV12 0x2c
 #define FD5012_ADDER_KUV13 0x2d
 #define FD5012_ADDER_KUV14 0x2e
 #define FD5012_ADDER_KUV15 0x2f
 
 #define FD5012_ADDER_KAV0  0x30	//�Զ�У׼ֵ
 #define FD5012_ADDER_KAV1  0x31
 #define FD5012_ADDER_KAV2  0x32
 #define FD5012_ADDER_KAV3  0x33
 #define FD5012_ADDER_KAV4  0x34
 #define FD5012_ADDER_KAV5  0x35
 #define FD5012_ADDER_KAV6  0x36
 #define FD5012_ADDER_KAV7  0x37
 #define FD5012_ADDER_KAV8  0x38
 #define FD5012_ADDER_KAV9  0x39
 #define FD5012_ADDER_KAV10 0x3a
 #define FD5012_ADDER_KAV11 0x3b
 #define FD5012_ADDER_KAV12 0x3c
 #define FD5012_ADDER_KAV13 0x3d
 #define FD5012_ADDER_KAV14 0x3e
 #define FD5012_ADDER_KAV15 0x3f
 
 #define FD5012_ADDER_KCV0  0x40	//��׼����ֵ
 #define FD5012_ADDER_KCV1  0x41
 #define FD5012_ADDER_KCV2  0x42
 #define FD5012_ADDER_KCV3  0x43
 #define FD5012_ADDER_KCV4  0x44
 #define FD5012_ADDER_KCV5  0x45
 #define FD5012_ADDER_KCV6  0x46
 #define FD5012_ADDER_KCV7  0x47
 #define FD5012_ADDER_KCV8  0x48
 #define FD5012_ADDER_KCV9  0x49
 #define FD5012_ADDER_KCV10 0x4a
 #define FD5012_ADDER_KCV11 0x4b
 #define FD5012_ADDER_KCV12 0x4c
 #define FD5012_ADDER_KCV13 0x4d
 #define FD5012_ADDER_KCV14 0x4e
 #define FD5012_ADDER_KCV15 0x4f
 
 #define FD5012_ADDER_KPV0  0x50	//��ǰ����ֵ
 #define FD5012_ADDER_KPV1  0x51
 #define FD5012_ADDER_KPV2  0x52
 #define FD5012_ADDER_KPV3  0x53
 #define FD5012_ADDER_KPV4  0x54
 #define FD5012_ADDER_KPV5  0x55
 #define FD5012_ADDER_KPV6  0x56
 #define FD5012_ADDER_KPV7  0x57
 #define FD5012_ADDER_KPV8  0x58
 #define FD5012_ADDER_KPV9  0x59
 #define FD5012_ADDER_KPV10 0x5a
 #define FD5012_ADDER_KPV11 0x5b
 #define FD5012_ADDER_KPV12 0x5c
 #define FD5012_ADDER_KPV13 0x5d
 #define FD5012_ADDER_KPV14 0x5e
 #define FD5012_ADDER_KPV15 0x5f
 #endif
 
 #ifdef FD5012_OLD
 #define FD5012_ADDR_CNTR0 0x08
 #define FD5012_ADDR_CNTR1 0x09
 #define FD5012_ADDR_WRPOLY 0x0a
 
 #define FD5012_ADDER_KDV0  0x0b	//������ֵ
 #define FD5012_ADDER_KDV1  0x0c
 #define FD5012_ADDER_KDV2  0x0d
 #define FD5012_ADDER_KDV3  0x0e
 #define FD5012_ADDER_KDV4  0x0f
 #define FD5012_ADDER_KDV5  0x10
 #define FD5012_ADDER_KDV6  0x11
 #define FD5012_ADDER_KDV7  0x12
 #define FD5012_ADDER_KDV8  0x13
 #define FD5012_ADDER_KDV9  0x14
 #define FD5012_ADDER_KDV10 0x15
 #define FD5012_ADDER_KDV11 0x16
 
 #define FD5012_ADDER_KUV0  0x17	//�ɿ���ֵ
 #define FD5012_ADDER_KUV1  0x18
 #define FD5012_ADDER_KUV2  0x19
 #define FD5012_ADDER_KUV3  0x1a
 #define FD5012_ADDER_KUV4  0x1b
 #define FD5012_ADDER_KUV5  0x1c
 #define FD5012_ADDER_KUV6  0x1d
 #define FD5012_ADDER_KUV7  0x1e
 #define FD5012_ADDER_KUV8  0x1f
 #define FD5012_ADDER_KUV9  0x20
 #define FD5012_ADDER_KUV10 0x21
 #define FD5012_ADDER_KUV11 0x22
 
 #define FD5012_ADDER_KAV0  0x23	//�Զ�У׼ֵ
 #define FD5012_ADDER_KAV1  0x24
 #define FD5012_ADDER_KAV2  0x25
 #define FD5012_ADDER_KAV3  0x26
 #define FD5012_ADDER_KAV4  0x27
 #define FD5012_ADDER_KAV5  0x28
 #define FD5012_ADDER_KAV6  0x29
 #define FD5012_ADDER_KAV7  0x2a
 #define FD5012_ADDER_KAV8  0x2b
 #define FD5012_ADDER_KAV9  0x2c
 #define FD5012_ADDER_KAV10 0x2d
 #define FD5012_ADDER_KAV11 0x2e
 #define FD5012_ADDR_BDV	0x2f

 #define FD5012_ADDER_KCV0  0x30	//��׼����ֵ
 #define FD5012_ADDER_KCV1  0x31
 #define FD5012_ADDER_KCV2  0x32
 #define FD5012_ADDER_KCV3  0x33
 #define FD5012_ADDER_KCV4  0x34
 #define FD5012_ADDER_KCV5  0x35
 #define FD5012_ADDER_KCV6  0x36
 #define FD5012_ADDER_KCV7  0x37
 #define FD5012_ADDER_KCV8  0x38
 #define FD5012_ADDER_KCV9  0x39
 #define FD5012_ADDER_KCV10 0x3a
 #define FD5012_ADDER_KCV11 0x3b

 
 #define FD5012_ADDER_KPV0  0x3c	//��ǰ����ֵ
 #define FD5012_ADDER_KPV1  0x3d
 #define FD5012_ADDER_KPV2  0x3e
 #define FD5012_ADDER_KPV3  0x3f
 #define FD5012_ADDER_KPV4  0x40
 #define FD5012_ADDER_KPV5  0x41
 #define FD5012_ADDER_KPV6  0x42
 #define FD5012_ADDER_KPV7  0x43
 #define FD5012_ADDER_KPV8  0x44
 #define FD5012_ADDER_KPV9  0x45
 #define FD5012_ADDER_KPV10 0x46
 #define FD5012_ADDER_KPV11 0x47

#define	FD5012_ADDR_KEYBUF	0x48
#define FD5012_ADDR_RDPOLY	0x49
#define FD5012_ADDR_MSDIFF  0x4a

 #endif


//+++++++++++++++++++++++++++++++++++++++++++++++++++++
#define TYPE_MCU 		  0x00	//��Ƭ����������
#define TYPE_FDXX_REG_BIT_SET 0x10	//�Ĵ���λ��λ
#define TYPE_FDXX_REG_BIT_CLR 0x11	//�Ĵ���λ����
#define TYPE_FDXX_REG	  0x1E	//FDоƬ��Ӧ�ļĴ���
#define TYPE_FDXX_DISPLAY 0x20	//��ʾ��ʾ
#define TYPE_FDXX_KEY	  0x30	//������������
#define TYPE_FDXXX_MODE	  0x31	//��λ��:ģʽ��������
#define TYPE_FDXXX_FUN	0x32	//���ܿ���
#define TYPE_FDXXX_CHIP_TYPE 0x33 //оƬ����
#define TYPE_FDXXX_WAVE_VIEW 0x34	//������ʾ
#define TYPE_MY_CMD		  0x70	//�Զ�������


//��λ��:�����Ʋ��ֶ�Ӧ�ļĴ���: TYPE_MCU
#define CMD_CPU_RST	0x00	//��Ƭ����λָ��

//��λ��:��λ�����ļĴ��� TYPE_FDXX_REG_BIT_SET   TYPE_FDXX_REG_BIT_CLR
#define CMD_CNTR_BIT_REG  0x00
#define CMD_DIS0_BIT_REG  0x01
#define CMD_DIS1_BIT_REG  0x02
#define CMD_DIS2_BIT_REG  0x03
#define CMD_DIS3_BIT_REG  0x04
#define CMD_DIS4_BIT_REG  0x05
#define CMD_DIS5_BIT_REG  0x06
#define CMD_DIS6_BIT_REG  0x07
#define CMD_DIS7_BIT_REG  0x08
#define CMD_DIS8_BIT_REG  0x09
#define CMD_CNTR1_BIT_REG 0x0a

//�Ĵ�������:TYPE_FDXX_REG
#define CMD_CNTR_REG 0x00
#define CMD_CNTR_LIGHT_REG 0x01	//�������⴦��
#define CMD_CNTR1_REG 0x02
#define CMD_MSDIFF 0x03
#define CMD_BDV 0x04

#define CMD_REG_KDV0 	0x20	//kdv0
#define CMD_REG_KDV1	0x21
#define CMD_REG_KDV2	0x22
#define CMD_REG_KDV3	0x23
#define CMD_REG_KDV4	0x24
#define CMD_REG_KDV5	0x25
#define CMD_REG_KDV6	0x26
#define CMD_REG_KDV7	0x27
#define CMD_REG_KDV8	0x28
#define CMD_REG_KDV9	0x29
#define CMD_REG_KDV10	0x2a
#define CMD_REG_KDV11	0x2b
#define CMD_REG_KDV12	0x2c
#define CMD_REG_KDV13	0x2d
#define CMD_REG_KDV14	0x2e
#define CMD_REG_KDV15	0x2f

#define CMD_REG_KUV0	0x30	//kuv0
#define CMD_REG_KUV1	0x31
#define CMD_REG_KUV2	0x32
#define CMD_REG_KUV3	0x33
#define CMD_REG_KUV4	0x34
#define CMD_REG_KUV5	0x35
#define CMD_REG_KUV6	0x36
#define CMD_REG_KUV7	0x37
#define CMD_REG_KUV8	0x38
#define CMD_REG_KUV9	0x39
#define CMD_REG_KUV10	0x3a
#define CMD_REG_KUV11	0x3b
#define CMD_REG_KUV12	0x3c
#define CMD_REG_KUV13	0x3d
#define CMD_REG_KUV14	0x3e
#define CMD_REG_KUV15	0x3f

#define CMD_REG_KAV0	0x40	//kuv0
#define CMD_REG_KAV1	0x41
#define CMD_REG_KAV2	0x42
#define CMD_REG_KAV3	0x43
#define CMD_REG_KAV4	0x44
#define CMD_REG_KAV5	0x45
#define CMD_REG_KAV6	0x46
#define CMD_REG_KAV7	0x47
#define CMD_REG_KAV8	0x48
#define CMD_REG_KAV9	0x49
#define CMD_REG_KAV10	0x4a
#define CMD_REG_KAV11	0x4b
#define CMD_REG_KAV12	0x4c
#define CMD_REG_KAV13	0x4d
#define CMD_REG_KAV14	0x4e
#define CMD_REG_KAV15	0x4f

//��ʾ�Ĵ�������:TYPE_FDXX_DISPLAY
#define CMD_DIS0_REG 0x00
#define CMD_DIS1_REG 0x01
#define CMD_DIS2_REG 0x02
#define CMD_DIS3_REG 0x03
#define CMD_DIS4_REG 0x04
#define CMD_DIS5_REG 0x05
#define CMD_DIS6_REG 0x06
#define CMD_DIS7_REG 0x07
#define CMD_DIS8_REG 0x08
#define CMD_DIS9_REG 0x09

//������Ӧ������:TYPE_FDXX_KEY
#define CMD_KEY_REG 0x00

//ģʽ����:TYPE_FDXXX_MODE
#define CMD_FDXXX_MODE_DIS0 0x00	//��ʾ��ʽ0
#define CMD_FDXXX_MODE_DIS1 0x01	//��ʾ��ʽ1
#define CMD_FDXXX_MODE_KEY0 0x02	//������ʽ0
#define CMD_FDXXX_MODE_KEY1 0x03	//������ʽ1
#define CMD_FDXXX_MODE_OTHER 0x04	//����ģʽ
#define CMD_FDXXX_MODE_DIS_SYS_TIME 0x05	//��ʾϵͳʱ��
#define CMD_FDXXX_MODE_FILE_CMD 0x06	//��ʾϵͳʱ��

//���ܿ���: TYPE_FDXXX_FUN
#define CMD_FDXXX_FUN_REFRESH 0x00			//ˢ����ʾ
#define CMD_FDXXX_FUN_PRINTF_INFO 0x01		//��ӡ���е���Ϣ 
#define CMD_FDXXX_FUN_PRINFT_CDM_EN 0x02	//��ӡ����
#define CMD_FDXXX_FUN_PRINFT_CMD_DISABLE 0x03	//����ӡ����
#define CMD_FDXXX_FUN_CMD_I2C_SPEED 0x04	//i2c�ٶ�����
#define CMD_FDXXX_FUN_CMD_I2C_10K 0x05		//i2c�ٶ�����:10K
#define CMD_FDXXX_FUN_CMD_I2C_20K 0x06		//i2c�ٶ�����:20K
#define CMD_FDXXX_FUN_CMD_I2C_50K 0x07		//i2c�ٶ�����:50K
#define CMD_FDXXX_FUN_CMD_I2C_100K 0x08		//i2c�ٶ�����:100K
#define CMD_FDXXX_FUN_CMD_I2C_200K 0x09		//i2c�ٶ�����:200K
#define CMD_FDXXX_FUN_CMD_I2C_500K 0x0a		//i2c�ٶ�����:500K
#define CMD_FDXXX_FUN_CMD_I2C_1M 0x0b		//i2c�ٶ�����:1M
#define CMD_FDXXX_FUN_CMD_I2C_2M 0x0c		//i2c�ٶ�����:2M
#define CMD_FDXXX_FUN_CMD_I2C_4M 0x0d		//i2c�ٶ�����:4M
#define CMD_FDXXX_FUN_CMD_PRINTF_TIME_EN 0x0e	//��ӡϵͳʱ��:���յ���λ�����͵�ָ���,��ӡ��ǰʱ��
#define CMD_FDXXX_FUN_CMD_PRINTF_TIME_DISABLE 0x0f	//��ӡϵͳʱ��:���յ���λ�����͵�ָ���,��ӡ��ǰʱ��
#define CMD_FDXXX_FUN_CMD_I2C_CONTINUOUS_EN 0x10	//i2c����ģʽ
#define CMD_FDXXX_FUN_CMD_I2C_CONTINUOUS_DISABLE 0x11 //i2c������ģʽ

//оƬ����:TYPE_FDXXX_CHIP_TYPE
#define CMD_FDXXX_CHIP_TYPE_FD650 0x00 
#define CMD_FDXXX_CHIP_TYPE_FD651 0x01
#define CMD_FDXXX_CHIP_TYPE_FD638 0x02
#define CMD_FDXXX_CHIP_TYPE_FD632 0x03
#define CMD_FDXXX_CHIP_TYPE_FD5012 0x04

//������ʾ:TYPE_FDXXX_WAVE_VIEW
#define CMD_FDXXX_WAVE_0  0x00	//����0 
#define CMD_FDXXX_WAVE_1  0x01	//����1 
#define CMD_FDXXX_WAVE_2  0x02	//����2 
#define CMD_FDXXX_WAVE_3  0x03	//����3 
#define CMD_FDXXX_WAVE_4  0x04	//����4 
#define CMD_FDXXX_WAVE_5  0x05	//����5 
#define CMD_FDXXX_WAVE_6  0x06	//����6 
#define CMD_FDXXX_WAVE_7  0x07	//����7 
#define CMD_FDXXX_WAVE_8  0x08	//����8 
#define CMD_FDXXX_WAVE_9  0x09	//����9 
#define CMD_FDXXX_WAVE_10 0x0a	//����10
#define CMD_FDXXX_WAVE_11 0x0b	//����11
#define CMD_FDXXX_WAVE_12 0x0c	//����12
#define CMD_FDXXX_WAVE_13 0x0d	//����13
#define CMD_FDXXX_WAVE_14 0x0e	//����14
#define CMD_FDXXX_WAVE_15 0x0f	//����15

#define CMD_FDXXX_WAVE_KCV0  0x10		//�ο�����ֵ�Ĵ���
#define CMD_FDXXX_WAVE_KCV1  0x11
#define CMD_FDXXX_WAVE_KCV2  0x12
#define CMD_FDXXX_WAVE_KCV3  0x13
#define CMD_FDXXX_WAVE_KCV4  0x14
#define CMD_FDXXX_WAVE_KCV5  0x15
#define CMD_FDXXX_WAVE_KCV6  0x16
#define CMD_FDXXX_WAVE_KCV7  0x17
#define CMD_FDXXX_WAVE_KCV8  0x18
#define CMD_FDXXX_WAVE_KCV9  0x19
#define CMD_FDXXX_WAVE_KCV10 0x1a
#define CMD_FDXXX_WAVE_KCV11 0x1b
#define CMD_FDXXX_WAVE_KCV12 0x1c
#define CMD_FDXXX_WAVE_KCV13 0x1d
#define CMD_FDXXX_WAVE_KCV14 0x1e
#define CMD_FDXXX_WAVE_KCV15 0x1f


#define CMD_FDXXX_WAVE_ALL_EN 0x70	//֧�ֶ�·ͬʱ��ʾ,����0:wave0 ... ����n:waven
#define CMD_FDXXX_WAVE_ALL_DISABLE 0x71	//֧�ֶ�·ͬʱ��ʾ,����0:wave0 ... ����n:waven


//+++++++++++++++++++++++++++++++++++++++++++++++++++++


//�����궨���������
#define RECEIVE_LENGTH 200
#define WRITE_DATA_LENGTH 100
typedef struct
{
	u8 Flag;
	u8 Data;
} ReceiveData;

extern ReceiveData ReceiveDataBufe[RECEIVE_LENGTH];
extern u8 ReceiveCnt;	//�������ݻ������:������дָ��
extern u8 ReadCnt;
extern u16 I2cDelayCnt;
extern void AppUsart_I2cDelay(void);
extern u8 FD_I2cContinuousEna;
extern u8 FD_I2cAddrType;

void AppUsart_Init();
void AppUsart_UsartDataDeal(void);
void AppUsart_Run();
void AppUsart_I2cDelay(void);

#endif