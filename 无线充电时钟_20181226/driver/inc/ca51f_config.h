#ifndef CA51F_CONFIG_H
#define CA51F_CONFIG_H
/**************************************************************************************************************/
#define IC_TYPE_CA51F2X 0
#define IC_TYPE_CA51F3X 1

#define IRCH		0
#define IRCL		1
#define PLL			2
#define XOSCH		3
#define XOSCL		4

/**************************************************************************************************************/
#define IC_TYPE			IC_TYPE_CA51F3X     //оƬϵ��ѡ��
#define SYSCLK_SRC	IRCH								//оƬϵͳʱ��ѡ��
/**************************************************************************************************************/

/************ϵͳʱ��Ƶ�ʶ��壬��Ҫ��������UART������**********************************************************/
#if (SYSCLK_SRC == IRCH)
	#define FOSC		(3686400)
#elif (SYSCLK_SRC == PLL)
	#define PLL_Multiple			7			//PLL��Ƶ����
	#define FOSC		(3686400*PLL_Multiple)
#else
	#define FOSC		(3686400)
#endif
/***************************************************************************************************************/


/*************************UART���ܿ��غ궨��********************************************************************/
//#define UART0_EN			//���ʹ��UART0���򿪴˺궨��
//#define UART1_EN			//���ʹ��UART1���򿪴˺궨��
#if (IC_TYPE == IC_TYPE_CA51F2X)
//	#define UART2_EN 		//ֻ��F2ϵ�в���UART2�����ʹ��UART2���򿪴˺궨��
#endif

#define PRINT_EN			0	//ʹ��uart_printf������ӡʹ��

#ifdef PRINT_EN
	#define UART0_PRINT		0//���ʹ��UART0��ӡ���򿪴˺궨��
//	#define UART1_PRINT		//���ʹ��UART1��ӡ���򿪴˺궨��
	#if (IC_TYPE == IC_TYPE_CA51F2X)
//		#define UART2_PRINT		//���ʹ��UART2��ӡ���򿪴˺궨��
	#endif
			
	#ifdef UART0_PRINT
		#define UART0_EN   0
	#elif defined  UART1_PRINT
		#define UART1_EN
	#elif defined  UART2_PRINT
		#define UART2_EN
	#endif
#endif
#ifdef UART0_EN
	#define UART0_BAUTRATE		115200
#endif
#ifdef UART1_EN
	#define UART1_BAUTRATE		115200
#endif
#ifdef UART2_EN
	#define UART2_BAUTRATE		115200
#endif
/*********************************************************************************************************************/

#endif										
