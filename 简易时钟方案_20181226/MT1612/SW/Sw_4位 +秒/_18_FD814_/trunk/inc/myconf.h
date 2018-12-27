/******************************************************************************
* @file 	   myconf.H   	系统配置头文件
* @brief       myconf.H  	系统配置头文件
* @author      LH
* @version     1A0 
* @date 	   2014-03-11    
* @note		   用于开启外设和DEBUG头文件
* @details  
* @copyright Copyright (c) 2014 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.              
******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__MYCONF_H 
#define __MYCONF_H


/* 是否打印自定义函数配置 */
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
