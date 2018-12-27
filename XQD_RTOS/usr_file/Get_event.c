/***************************************************
* @Copyright(c) 2016-2100.  hwl.  All rights reserved.
* @FileName: Get_event.c
* @Author:   hwl 
* @Description:  Get event functions
* @Version:
* @History:         
      <author>  <time>      <version >   <desc>
				hwl      18/9/7       1.0     
****************************************************/



#include "M451Series.h"
#include "Event_process.h"
#include "Get_event.h"


extern menu_selection_t  menu_selection;

/***************************************************** 
 * @Function:  digital_read
 * @Description:    read key pin statue
 * @param  arg 
 * @retval char
******************************************************/
char keyTable[4] = {KEY_PIN1,KEY_PIN2,KEY_PIN3,KEY_PIN4};

char digital_read( char  arg)
{
	switch(arg)
	{
		case KEY_PIN1:
		{
			if(PD3 == 0) return LOW;
			break;
		}
		case KEY_PIN2:
		{
			if(PD2 == 0) return LOW;
			break;
		}	
		case KEY_PIN3:
		{
			if(PB0 == 0) return LOW;
			break;
		}	
		case KEY_PIN4:
		{
			if(PA0 == 0) return LOW;
			break;
		}	
		default:
			break;
	}
	return HIGH;
}



/***************************************************** 
 * @Function:  check_key_press_event
 * @Description:    check key short or long to send event
 * @param  void 
 * @retval void
******************************************************/
unsigned char display_flag = 0;//控制长按显示跳动问题
extern xTaskHandle DMX_DisplayHandle;
void check_key_press_event()
{
	
	static unsigned char oldKeyNum = 0xFF, newKeyNum = 0xFF,countTime = 0;
	static unsigned char no_move_count = 0;
	unsigned char loop = 0;
	static char key_long_flag = 0;
	
	for (loop = 0; loop < 4; loop++)
	{
		if (digital_read(keyTable[loop]) == LOW )
		{
			no_move_count = 0;
			oldKeyNum = newKeyNum;
			newKeyNum = loop;
			oldKeyNum = oldKeyNum;
#if debug	
			printf("pressed key %d old key %d count[%d] loop[%d]\n", newKeyNum, oldKeyNum, countTime, loop);
#endif
			countTime++;
			/*长按*/
			if(countTime > MAX_COUNT_TIME && key_long_flag ==0 )
			{
#if debug	
				printf("key1_long+neykeynum%d",(key1_long + newKeyNum));
#endif
				//自加操作
			while(((key1_long + newKeyNum)==7 || (key1_long + newKeyNum)==6 )&& digital_read(keyTable[loop]) == LOW && menu_selection.munu_select != First_level_menu)
			{
				send_event((Eevent)( newKeyNum));
				display_flag =1;
				vTaskDelay( 200 / portTICK_RATE_MS );	
			}
//			blink_flag = 0;	
			send_event((Eevent)(key1_long + newKeyNum));			
			//countTime = 0;	
			key_long_flag = 1;
			return;
			}
			vTaskDelay( 100 / portTICK_RATE_MS );	
			return;			
		}


	}
	if(key_long_flag && countTime > MAX_COUNT_TIME )
	{
		key_long_flag=0;
		countTime = 0;
		display_flag =0;
		return;
	}
	/*短按*/
	if (countTime < MAX_COUNT_TIME && countTime > MIN_COUNT_TIME)
	{

#if debug	
		printf("key1+neykeynum%d",(key1 + newKeyNum));
#endif
	
		send_event((Eevent)(key1 + newKeyNum));
		countTime = 0;
		return;
	}
	/*无动作时返回*/
	//通道调节不返回
	if(menu_selection.First_level_menu_selection ==First_level_menu_second_submenu&&menu_selection.Second_level_menu_selection==Second_level_menu_Effect_second_submenu)
		return;
	//手动跑点任务不返回
		if(menu_selection.Run_manually_point_flag==1)
		{
			return;
		}
	if(menu_selection.First_level_menu_selection != First_level_menu_main_submenu)
	{
		no_move_count++;
		vTaskDelay( 100 / portTICK_RATE_MS );	
		if(no_move_count>150)
		{
			no_move_count = 0;
			send_event(Date_handle_event);
#if debug
			printf("no move happend=================>>>>>>>>\r\n");
#endif
		}
	}
}
