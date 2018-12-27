/******************************************************************************
* @file 	   mydelay.C   延时函数 源文件
* @brief       mydelay.C  	延时函数 源文件
* @author      LH
* @version     1A0 
* @date 	   2014-03-14    
* @note
* @details  
* @copyright Copyright (c) 2014 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.              
******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include	"mydelay.h"
#include <intrins.h>

/* Private functions ---------------------------------------------------------*/
 /**
 *  @brief	   1毫秒延时
 *
 *  @paramt	   None
 *
 *  @return    None
 *
 *  @par Example
 *  @code
 *  delay1ms(1);	//延时1ms
 *  @endcode
 *
 */
void delay1ms()		//@22.1184MHz
{
	u8 i, j;

	i = 4;
	j = 128;
	do
	{
		while (--j);
	} while (--i);

}

 /**
 *  @brief	   几毫秒延时
 *
 *  @paramt[in]	  t:延时的时间 
 *
 *  @return    None
 *
 *  @par Example
 *  @code
 *  delayms(10);	//延时10ms
 *  @endcode
 *
 */
 void delayms(u16 t)
 {	
	while(t--)
	{
		delay1ms();
	}
 }

  /**
 *  @brief	  	 1us延时
 *
 *  @paramt	   None
 *
 *  @return    None
 *
 *  @par Example
 *  @code
 *  delay1us();
 *  @endcode
 *
 */
 void delay1us()		//@22.1184MHz
{
	  u8 i;
	  
	  _nop_();
	  _nop_();
	  i = 2;
	  while (--i);

}

  /**
 *  @brief	  	 几us延时
 *
 *  @paramt[in]	  t:延时的时间 
 *
 *  @return    None
 *
 *  @par Example
 *  @code
 *  delayus(10);	//延时10us
 *  @endcode
 *
 */
 void delayus(u16 t)
 {	
	while(t--)
	{
		delay1us();
	}
 }

/******************* (C) COPYRIGHT 2014 FDHISI *****END OF FILE****/
