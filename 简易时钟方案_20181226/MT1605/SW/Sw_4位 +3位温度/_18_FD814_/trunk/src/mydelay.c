/******************************************************************************
* @file 	   mydelay.C   ��ʱ���� Դ�ļ�
* @brief       mydelay.C  	��ʱ���� Դ�ļ�
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
 *  @brief	   1������ʱ
 *
 *  @paramt	   None
 *
 *  @return    None
 *
 *  @par Example
 *  @code
 *  delay1ms(1);	//��ʱ1ms
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
 *  @brief	   ��������ʱ
 *
 *  @paramt[in]	  t:��ʱ��ʱ�� 
 *
 *  @return    None
 *
 *  @par Example
 *  @code
 *  delayms(10);	//��ʱ10ms
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
 *  @brief	  	 1us��ʱ
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
 *  @brief	  	 ��us��ʱ
 *
 *  @paramt[in]	  t:��ʱ��ʱ�� 
 *
 *  @return    None
 *
 *  @par Example
 *  @code
 *  delayus(10);	//��ʱ10us
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
