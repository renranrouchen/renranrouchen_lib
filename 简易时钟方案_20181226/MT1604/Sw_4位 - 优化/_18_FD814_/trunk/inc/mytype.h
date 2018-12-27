/******************************************************************************
* @file 	   mytype.H   	类型定义头文件
* @brief       mytype.H  	类型定义头文件
* @author      LH
* @version     1A0 
* @date 	   2014-03-11    
* @note
* @details  
* @copyright Copyright (c) 2014 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.              
******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__MYTYPE_H 
#define __MYTYPE_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef signed long  s32;
typedef signed int s16;
typedef signed char  s8;

typedef signed long  const sc32;  /* Read Only */
typedef signed int const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long  vs32;
typedef volatile signed int vs16;
typedef volatile signed char  vs8;

typedef unsigned long  u32;
typedef unsigned int u16;
typedef unsigned char  u8;

typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned int const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned long  vu32;
typedef volatile unsigned int vu16;
typedef volatile unsigned char  vu8;

//typedef enum {FALSE = 0, TRUE = !FALSE} bool;
//
//typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
//
//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
//#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
//
//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)

#endif


/******************* (C) COPYRIGHT 2014 FDHISI *****END OF FILE****/
