/******************************************************************************
* @file 	   myconf.H   	ϵͳ����ͷ�ļ�
* @brief       myconf.H  	ϵͳ����ͷ�ļ�
* @author      LH
* @version     1A0 
* @date 	   2014-03-11    
* @note		   ���ڿ��������DEBUGͷ�ļ�
* @details  
* @copyright Copyright (c) 2014 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.              
******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__MYCONF_H 
#define __MYCONF_H


/* �Ƿ��ӡ�Զ��庯������ */
#define M1(a,b) a##b
#ifdef _DEBUG
  #define myprintf printf
#else
  #define myprintf M1(/,/)
#endif

#ifdef _DEBUG
  #define assert_param(expr) ((expr) ? (void)0 : myprintf("file %s line %u\n",(u8 *)__FILE__,(u16)__LINE__))
#else
  #define assert_param(expr) ((void)0)
#endif

#endif

/******************* (C) COPYRIGHT 2014 FDHISI *****END OF FILE****/
