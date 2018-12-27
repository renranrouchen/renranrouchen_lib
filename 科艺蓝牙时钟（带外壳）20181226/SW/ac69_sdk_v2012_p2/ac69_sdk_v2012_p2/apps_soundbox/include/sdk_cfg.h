/*********************************************************************************************
    *   Filename        : sdk_cfg.h

    *   Description     : Config for Sound Box Case

    *   Author          : Bingquan

    *   Email           : bingquan_cai@zh-jieli.com

    *   Last modifiled  : 2016-12-01 15:36

    *   Copyright:(c)JIELI  2011-2016  @ , All Rights Reserved.
*********************************************************************************************/

#ifndef _CONFIG_
#define _CONFIG_

#include "includes.h"


///�����ջ��С���ò���
#define VM_TASK_STACK_SIZE          (1024 * 1)
#define MAIN_TASK_STACK_SIZE        (1024 * 2) //(1024 * 1)����
#define MUSIC_TASK_STACK_SIZE       (1024 * 4)
#define MUSIC_PHY_TASK_STACK_SIZE   (1024 * 4)
#define TONE_TASK_STACK_SIZE        (1024 * 4)
#define TONE_PHY_TASK_STACK_SIZE    (1024 * 2)
#define UI_TASK_STACK_SIZE          (1024 * 2)

/*
*product version config
*/


//���ö���
///< ��������ʹ��,������ʱ������.ble,��̨��app
#define   BT_TWS                      0
///����ӻ������Ƿ���뱻���ӻ��ǻ���״̬
 #define    TWS_SLAVE_WAIT_CON         1
///���������Ƿ�һ��ػ�
#define   BT_TWS_POWEROFF_TOGETHER    0

///����û���ӳɹ�֮ǰ��ͨ����������ɷ���ʹ��,������ͬʱ������԰����Ž������
#define    BT_TWS_SCAN_ENBLE         0

/********************************************************************************/
/*
 *          --------����������
 */
////<����ϵͳ��ӡ���Թ���
 #define __DEBUG

///<LEDָʾʹ��
#define LED_EN                  1
///<��������
#define UART_UPDATA_EN          0
///<�������
#define SYS_LVD_EN              1

#if (BT_TWS == 1)
	#define BT_KTV_EN			0
#else
///<����K�豦
	#define BT_KTV_EN			0
#endif

///<����
#define PITCH_EN				0	///>���������Х������ͬʱʹ��

///<Х������
#define HOWLING_SUPPRESSION_EN	0	///>Х�в�������ͬʱʹ��

/********************************************************************************/

/********************************************************************************/
/*
 *           --------�ֻ�app���� ����
 */
#if (BT_TWS == 1)
   #define SUPPORT_APP_RCSP_EN    0
   #define RCSP_LIGHT_HW	      0
   #define BLE_FINGER_SPINNER_EN  0
#else
///�����Ƿ�֧���ֻ�app���ܣ�  (1-֧��,0-��֧��)
   #define SUPPORT_APP_RCSP_EN   0
///�����Ƿ�֧��������Ӳ������ (1-֧��,0-��֧��)
   #define RCSP_LIGHT_HW	     0
///�����Ƿ�֧��ָ������BLE���ݴ��� (1-֧��,0-��֧��) //�����ú� SUPPORT_APP_RCSP_EN����ͬʱ��
   #define BLE_FINGER_SPINNER_EN 0
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------��Դ������
 */
///   0:  no change
#define    PWR_NO_CHANGE        0
///   1:  LDOIN 5v -> VDDIO 3.3v -> DVDD 1.2v
#define    PWR_LDO33            1
///   2:  LDOIN 5v -> LDO   1.5v -> DVDD 1.2v, support bluetooth
#define    PWR_LDO15            2
///   3:  LDOIN 5v -> DCDC  1.5v -> DVDD 1.2v, support bluetooth
#define    PWR_DCDC15           3

///��ѡ���ã�PWR_NO_CHANGE/PWR_LDO33/PWR_LDO15/PWR_DCDC15
#define PWR_MODE_SELECT         PWR_LDO15

//DCDCʱBTAVDD,��LDOģʽʱ���Ժ��� 0:1.61v  1:1.51v  2:1.43v  3:1.35v
//AC6904��װҪ��1.51v,��Ӧ����Ϊ1
#define VDC15_LEV     3

///�����������Զ��ػ���ʱ��u16���ͣ�0��ʾ���Զ��ػ�
#define AUTO_SHUT_DOWN_TIME     0

///<����˫������
#define KEY_DOUBLE_CLICK        0

///<��ص����ͣ��Ƿ��л���Դ�������
#define SWITCH_PWR_CONFIG		0
/*
	SYS_LDO_Level:3.53v-3.34v-3.18v-3.04v-2.87v-2.73v-2.62v-2.52v
	FM_LDO_Level:3.3v-3.04v-2.76v-2.5v
*/
///<Normal LDO level
#define SYS_LDO_NORMAL_LEVEL	1	//range:0~7:FM_LDO��VDDIO��һ�𣬽���:level=2
#define FM_LDO_NORMAL_LEVEL		0	//range:0~3
///<Reduce LDO level
#define SYS_LDO_REDUCE_LEVEL	3	//range:0~7
#define FM_LDO_REDUCE_LEVEL		1	//range:0~3

/********************************************************************************/

/********************************************************************************/
/*
 *           --------��Ч������
 */
///<EQģ�鿪��
#define EQ_EN			        1
//���EQ,��1������0�رգ���ϵͳʱ��Ҫ��ߣ�һ��Ҫ192M��
//����󲻼�����ǰ��eq�ļ������µ�EQ���ߣ�ע��eq�ļ�����Ϊ"cfg_eq.bin"
//�������eq�ٶ�Ҫ��ߣ���֧�������ʽ����֧�ֶ���
#define USE_SOFTWARE_EQ   		0
///<EQ uart online debug
#define EQ_UART_BDG	    		0
///<dac�����ϲ�
#define DAC_SOUNDTRACK_COMPOUND 0
///<�Զ�mute
#if BT_KTV_EN
	#define DAC_AUTO_MUTE_EN		0
#else
	#define DAC_AUTO_MUTE_EN		1
#endif
///<������
#define KEY_TONE_EN     	    0
///<��0��ʾʹ��Ĭ������
#define SYS_DEFAULT_VOL         0
///<��������
#define VOCAL_REMOVER			0


/********************************************************************************/


/********************************************************************************/
/*
 *           --------DAC VCOMO ����
 */
///�Ƿ�ѡ��VCMOֱ�ƶ���
#define VCOMO_EN 	            0
///��ѡ���ã�DAC_L_R_CHANNEL/DAC_L_CHANNEL/DAC_R_CHANNEL
#define DAC_CHANNEL_SLECT       DAC_L_R_CHANNEL
//DAC ���������IIS
#define DAC2IIS_EN                      0      //1:ʹ��DAC�����IIS�Թ̶�������44.1K�����
  #if(DAC2IIS_EN == 1)
     #define DAC2IIS_OUTCLK_AUTO_CLOSE  O    //1:��û������ʱ���Զ��ر�MCLK,SCLK,LRCLKʱ������� 0:���ر�
     #define IISCHIP_WM8978_EN          1    //1: ʹ��WM8978 IISоƬ��
  #else
     #define DAC2IIS_OUTCLK_AUTO_CLOSE  0    //1:��û������ʱ���Զ��ر�MCLK,SCLK,LRCLKʱ������� 0:���ر�
     #define IISCHIP_WM8978_EN          0    //1: ʹ��WM8978 IISоƬ��
  #endif
/********************************************************************************/

/********************************************************************************/
/*
 *           --------����������
 */
 #if (BT_TWS == 1)
	#define SDMMC0_EN           0
	#define SDMMC1_EN           0
	#define USB_DISK_EN         0
	#define USB_PC_EN           0
#else
    #define SDMMC0_EN           1
	#define SDMMC1_EN           1
	#define USB_DISK_EN         1
	#define USB_PC_EN           1
#endif

//usb_sd���Ÿ��ã���Ҫ���Լ�����
#define USB_SD0_MULT_EN     0	//<��Ҫ���Լ�����
#define USB_SD1_MULT_EN		0	//<��Ҫ���Լ�����

#if(USB_SD0_MULT_EN == 1)||(USB_SD1_MULT_EN == 1)
	#undef USB_PC_EN
	#define USB_PC_EN       0
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------����������
 */
#include "bluetooth/bluetooth_api.h"

#define NFC_EN          0  ///<NFC ENABLE

///��ѡ���ã�NORMAL_MODE/TEST_BQB_MODE/TEST_FCC_MODE/TEST_FRE_OFF_MODE/TEST_BOX_MODE
#define BT_MODE        NORMAL_MODE		//TEST_BQB_MODE//

#if (BT_MODE == TEST_FCC_MODE)
  #undef UART_UPDATA_EN
  #define UART_UPDATA_EN    0
#endif

///ģ������
#define BT_ANALOG_CFG       0
#define BT_XOSC             0//

///<������̨
#if (SUPPORT_APP_RCSP_EN == 1)
#define BT_BACKMODE         1
#else
#define BT_BACKMODE         0
#endif
///dependency
#if (BT_BACKMODE == 0)
    ///<HID���յĶ���ģʽֻ֧�ַǺ�̨
    #define BT_HID_INDEPENDENT_MODE  0
#endif

///<���籨��
#define BT_PHONE_NUMBER     1


/*
 *           --------�����͹�������
 *   ʹ�ܸù��ܺ�ֻ���Ǵ��������ܣ�û����ʾ����
 */
///��ѡ���ã�SNIFF_EN/SNIFF_TOW_CONN_ENTER_POWERDOWN_EN
#define SNIFF_MODE_CONF   0//  SNIFF_EN

///��ѡ���ã�BT_POWER_DOWN_EN/BT_POWER_OFF_EN
#define BT_LOW_POWER_MODE  0// BT_POWER_DOWN_EN

#define BT_OSC              0
#define RTC_OSCH            1
#define RTC_OSCL            2

///��ѡ���ã�BT_OSC/RTC_OSCH/RTC_OSCL
#define LOWPOWER_OSC_TYPE   BT_OSC

///��ѡ���ã�32768L//24000000L
#define LOWPOWER_OSC_HZ     24000000L



///��ѡ���ã�BT_BREDR_EN/BT_BLE_EN/(BT_BREDR_EN|BT_BLE_EN)
#if (BT_TWS == 1)
	#define BLE_BREDR_MODE      (BT_BREDR_EN)
#elif (BT_KTV_EN == 1)
	#define BLE_BREDR_MODE      (BT_BREDR_EN)
#else
    #define BLE_BREDR_MODE      (BT_BREDR_EN|BT_BLE_EN)
	#define BLE_GATT_ROLE_CFG    0 ///0--SERVER,1--CLIENT
#endif

///TWS �̶����Ҷ�ѡ�� ��ѡ���ã�TWS_CHANNEL_LEFT/TWS_CHANNEL_RIGHT)
#define    BT_TWS_CHANNEL_SELECT         0////�̶����Ҷ�ʱ,���Ϊ��,�Ҷ��������������

/********************************************************************************/

/********************************************************************************/
/*
 *           --------оƬ��װ����
 */
///RTCVDD��û�а����Ҫ��1,Ŀǰ��Ӧ��װоƬAC6905
#define RTCVDD_TYPE              0
#define BTAVDD_TYPE              0

/********************************************************************************/


/********************************************************************************/
/*
 *           --------MUSIC MACRO
 */
//SMP�����ļ�֧��
#define MUSIC_DECRYPT_EN 		0
//AB�ϵ�֧��
#define MUSIC_AB_RPT_EN 		0

///<MP3
#define DEC_TYPE_MP3_ENABLE     1
///<SBC
#define DEC_TYPE_SBC_ENABLE     1
///<AAC
#define DEC_TYPE_AAC_ENABLE		0

///<3K_code_space
#define DEC_TYPE_WAV_ENABLE     1
///<5K_code_space
#define DEC_TYPE_FLAC_ENABLE    0
///<8K_code_space
#define DEC_TYPE_APE_ENABLE     0
///<30K_code_space
#define DEC_TYPE_WMA_ENABLE     0
///<30K_code_space
#define DEC_TYPE_F1A_ENABLE     0

/********************************************************************************/


/********************************************************************************/
/*
 *           --------FM MACRO
 */
///<��׼SDK
#if (BT_TWS == 1)
	#define FM_RADIO_EN         0
#else
    #define FM_RADIO_EN         0
#endif

///dependency
#if (FM_RADIO_EN == 1)
    ///<RDA5807FM
    #define RDA5807                 0
    ///<BK1080FM
    #define BK1080                  0
    ///<QN8035FM
    #define QN8035                  0
    ///<оƬ�ڲ�FM
    #define FM_INSIDE               1
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------RTC MACRO
 */
///<��׼SDK RTCʱ��ģʽ
 #if (BT_TWS == 1)
    #define RTC_CLK_EN          0
#else
    #define RTC_CLK_EN          1
#endif
///dependency
#if (RTC_CLK_EN == 1)
    ///<RTC����ģʽ
    #define RTC_ALM_EN          1
	#define RTC_ORGINAL			1
	#define RTC_ALARM_THREE		1
	/*
	*add three alarm product,reserve orignal version
	*/
	#if (RTC_ALM_EN == 1)
		#define ALARM_ORIGNAL			1
		#if (ALARM_ORIGNAL != 1)
			#define ALARM_NUM_THREE		1
			#define RTC_DEFAULT_VERSION	0
		#else
			/* all disable */
			#define ALARM_NUM_THREE		0
			#define RTC_DEFAULT_VERSION	0
		#endif
	#endif
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------ECHO MACRO
 *			������죬����ر�	DAC_AUTO_MUTE_EN
 */
///dependency
#if (BT_BACKMODE == 1)
    ///<��֧����������̨�������
    #define ECHO_EN             0
    ///<����ģʽ ��׼SDK
    #define ECHO_TASK_EN        0
#else
	#if BT_KTV_EN
    	///<���칦�� KTV_SDK
    	#define ECHO_EN             1
	#else
    	///<���칦�� ��׼SDK
    	#define ECHO_EN             0
	#endif

    ///<����ģʽ ��׼SDK
    #define ECHO_TASK_EN        0
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------REC MACRO
 */
///dependency
#if (BT_BACKMODE == 1)
    ///<��֧����������̨����¼��
    #define REC_EN             0
#else
    ///<��׼SDK¼����
    #define REC_EN             0    ///<K�豦����ʹ������ͨ��
#endif

///>����MIC¼��ģʽ
#define REC_MODE_EN				0

///>¼��Դѡ��,	0:other		1:dac
#if (BT_KTV_EN == 1)
	#define REC_SOURCE			1
#else
	#define REC_SOURCE			1
#endif

#if (REC_EN == 1)
    ///<����¼��ʹ��
	#define BT_REC_EN		1
    ///<MIC¼��ʹ��
	#define MIC_REC_EN		1
    ///<FM¼��ʹ��
	#define FM_REC_EN		1
    ///<AUX¼��ʹ��
	#define AUX_REC_EN		1
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------UI MACRO
 */
///<UI_��ʾ
#if (RCSP_LIGHT_HW == 1)
#define UI_ENABLE                0
#else
#define UI_ENABLE                1
#endif
///dependency
#if (UI_ENABLE == 1)
    #define LCD_128X64_EN        0      ///<lcd ֧��
    #define LED_7_EN             0      ///<led ֧��
    #define FD628_EN			 0
	#define FD612_EN			 1
#else
    ///����֧��
    #define LCD_128X64_EN        0
    #define LED_7_EN             0
    #define FD628_EN			 0
	#define FD612_EN			 0
#endif
#if ((LCD_128X64_EN == 1) && (LED_7_EN == 1))
#error  "UI driver support only one"
#endif

#if (LCD_128X64_EN == 1)
    ///�˵���ʾ
    #define LCD_SUPPORT_MENU     1
    ///LRC�����ʾ
    #define LRC_LYRICS_EN        1
#else
    #define LCD_SUPPORT_MENU     0
    #define LRC_LYRICS_EN        0
#endif

/********************************************************************************/

 #if (BT_TWS == 1)
///<��������ʹ��,������ʱ������ble,��̨��app.
  #if ((BT_KTV_EN)||(SUPPORT_APP_RCSP_EN)||(BLE_BREDR_MODE&BT_BLE_EN))
  #error  "-------stereo not support this config ,please check sdk_cfg.h------------"
  #endif
#endif

#if (BT_LOW_POWER_MODE == BT_POWER_DOWN_EN)
  #if ((SDMMC0_EN)||(SDMMC1_EN)||(USB_DISK_EN)||(UI_ENABLE))
  #error  "-------powerdown not support this config ,please check sdk_cfg.h------------"
  #endif
#endif


#if (BT_MODE != NORMAL_MODE)
  #if ((SNIFF_MODE_CONF)||(BT_LOW_POWER_MODE))
  #error  "-------BT MODE not support this config ,please check sdk_cfg.h------------"
  #endif
#endif

#define PRODUCT_VERSION_KEYI	1
/* AOLINTONGHUA */
#define ALL_FUNCTION_VERSION    0



#if (PRODUCT_VERSION_KEYI == 1)
/*(1):Open key voice
* (2):Open bluethe open AUX open RTC
* (3):Open one alarm sleep 2 times
* (4):FD612 display two birghtness display mode 24T mode
* (5):
*/
///<������
#undef KEY_TONE_EN
#define KEY_TONE_EN     	    1
///<��0��ʾʹ��Ĭ������
#undef SYS_DEFAULT_VOL
#define SYS_DEFAULT_VOL         10

#undef FD612_EN
#define FD612_EN			1
/* SDMMC0 Disable */
#undef SDMMC0_EN
#define SDMMC0_EN			0
/* SDMMC1 Disable */
#undef SDMMC1_EN
#define SDMMC1_EN			0
/* USB disk Disable */
#undef USB_DISK_EN
#define USB_DISK_EN 		0
/* USB PC Disble */
#undef USB_PC_EN
#define USB_PC_EN			0
/* REC mode disable */
#undef REC_MODE_EN
#define REC_MODE_EN			0
/* Disable ECHO task */
#undef ECHO_TASK_EN
#define ECHO_TASK_EN		0
/* Disable EQ Mode */
#undef EQ_EN
#define EQ_EN			    0
/* Disable FM */
#undef FM_RADIO_EN
#define FM_RADIO_EN			0
#undef RTC_DEFAULT_VERSION
#define RTC_DEFAULT_VERSION	1
/* Verbrate Disable */
#define VIBRATE_EN		0
/* Time Format Disable Default to 24_Format */
#define TIME_FORMAT		0
#if (TIME_FORMAT == 1)
#define TIME_FORMAT_24_EN	1
#else
#define TIME_FORMAT_24_EN   0
#endif
/* Enable RTC CLK */
#undef 	RTC_CLK_EN
#define RTC_CLK_EN			1
#define RTC_DS1302			1

#undef DAC_SOUNDTRACK_COMPOUND
#define DAC_SOUNDTRACK_COMPOUND 1

#define POWERUP_COUNTING_DOWN		1
#if (POWERUP_COUNTING_DOWN == 1)
#define POWERUP_COUNTING_DOWN_MAX	10
#endif

#undef DAC2IIS_EN
#define DAC2IIS_EN                      0

#undef MAX_SYS_VOL_L
#define MAX_SYS_VOL_L         26      ///<ϵͳ����������
#undef MAX_SYS_VOL_R
#define MAX_SYS_VOL_R         26

#define LED_BIT              BIT(8)
#endif /* (PRODUCT_VERSION_KEYI == 1) */

#if (ALL_FUNCTION_VERSION == 1)
/**
 * Enable BT LED ,IO is PB8
 * Enable SD1 function ,IO select SDB
 * Ebable USB function
 * Enable AD key ,ADC IO is PA9
 * Enable BAT detect ,IO is PB9
 * Enable AUX function ,select AUX0
 * Eable FD612 display,CLK io is PC5,DATA IO PC4
 * Eable MUTX ,IO is PA11,set to low
 * Eanble temperature detect ,IO is PC2
 * Eaable vibrate detect ,IO is PC1
 */
#undef KEY_TONE_EN
#define KEY_TONE_EN     	    1
///<��0��ʾʹ��Ĭ������
#undef SYS_DEFAULT_VOL
#define SYS_DEFAULT_VOL         10

#undef FD612_EN
#define FD612_EN			1
/* SDMMC0 Disable */
#undef SDMMC0_EN
#define SDMMC0_EN			0
/* SDMMC1 Disable */
#undef SDMMC1_EN
#define SDMMC1_EN			1
/* USB disk Disable */
#undef USB_DISK_EN
#define USB_DISK_EN 		1
/* USB PC Disble */
#undef USB_PC_EN
#define USB_PC_EN			0
/* REC mode disable */
#undef REC_MODE_EN
#define REC_MODE_EN			0
/* Disable ECHO task */
#undef ECHO_TASK_EN
#define ECHO_TASK_EN		0
/* Disable EQ Mode */
#undef EQ_EN
#define EQ_EN			    0
/* Disable FM */
#undef FM_RADIO_EN
#define FM_RADIO_EN			1
#undef RTC_DEFAULT_VERSION
#define RTC_DEFAULT_VERSION	1
/* Verbrate Disable */
#define VIBRATE_EN		0
/* Time Format Disable Default to 24_Format */
#define TIME_FORMAT		0
#if (TIME_FORMAT == 1)
#define TIME_FORMAT_24_EN	1
#else
#define TIME_FORMAT_24_EN   0
#endif
/* Enable RTC CLK */
#undef 	RTC_CLK_EN
#define RTC_CLK_EN			1
#define RTC_DS1302			0

#undef DAC_SOUNDTRACK_COMPOUND
#define DAC_SOUNDTRACK_COMPOUND 0

#define POWERUP_COUNTING_DOWN		0
#if (POWERUP_COUNTING_DOWN == 1)
#define POWERUP_COUNTING_DOWN_MAX	10
#endif

#undef DAC2IIS_EN
#define DAC2IIS_EN                      0

#undef MAX_SYS_VOL_L
#define MAX_SYS_VOL_L         26      ///<ϵͳ����������
#undef MAX_SYS_VOL_R
#define MAX_SYS_VOL_R         26

///<LEDָʾʹ��
#undef LED_EN
#define LED_EN                  0
///<��������
#undef UART_UPDATA_EN
#define UART_UPDATA_EN          0
///<�������
#undef SYS_LVD_EN
#define SYS_LVD_EN              1

// Disble
#define USB_SD0_MULT_EN     0	//<��Ҫ���Լ�����
#define USB_SD1_MULT_EN		0	//<��Ҫ���Լ�����
/* Enable LED display */
#undef LED_EN
#define LED_EN                  1

#define LED_BIT              BIT(8)
#endif /* (ALL_FUNCTION_VERSION == 1) */

//put file after product config
#include "production_config_param.h"
#endif

