/***************************************************
* @Copyright(c) 2016-2100.  hwl.  All rights reserved.
* @FileName: Event_process.c
* @Author:   hwl 
* @Description:  Event handle when Event happened
* @Version:
* @History:         
      <author>  <time>      <version >   <desc>
				hwl      18/9/7       1.0     
****************************************************/

#include "M451Series.h"
#include "Event_process.h"
#include "DMX512_API.H"
#include <stdarg.h>
#include "ST7567.H"
#include "DMX512.H"
#include "semphr.h"





event_func eventFuncTable[EVENT_MAX];
static xQueueHandle eventQueue;
extern DMX512_arg_t DMX512_arg_switch_data;
extern xTaskHandle DMX_DisplayHandle;
extern xTaskHandle check_DisplayHandle;
extern xTaskHandle ui_display_DisplayHandle;
extern xTaskHandle xgradual_displayHandle;
extern xTaskHandle RACELAMP_displayHandle;;
extern xTaskHandle Run_manually_pointHandle;
extern const unsigned int frequency_arg[8];


DMX512_arg_t SM1751_arg ;
chip_param_t chip_param_sm17512;

chip_param_t chip_param_sm165XX;
SM165XX_arg_t SM165XX_arg_set;

DMX512_arg_t SM17500_arg;
chip_param_t chip_param_sm17500;

RZ_arg_t chip_param_sm16703P;
RZ_arg_t chip_param_sm16804P;
	
Protocol_output_parameter_t Equipment_set_param;

menu_selection_t  menu_selection =
{
	.munu_select = First_level_menu,
//	.munu_posit  = menu_first,
//	.munu_set_select = lamps_coding,
//	.munu_set_coding_select = coding_addr,
//	.munu_set_param_select_rgbset =0,
//	.chip_model = SM165XX,
	//.Color_num = RED,
//	.refresh_rate = 1,
//	.save_flag = 0,
	.Run_manually_point_flag=0,
};

//Protocol_output_parameter_t Equipment_set_param =
//{
//	.On_load_channe_param=16,
//	.Output_frequency_param=Freq_baud_250k,
//	.Play_speed_param=1,
//	.colour_num_param=3,
//	.Protocol_type_param =RTZ,
//	.RZ_chip_param =RZ_SM16804,
//	.Red_data=255,
//	.Green_data=0,
//	.Blue_data=255,
//	.Write_data=0,
//	.Yellow_data=255,
//};







void ui_display(void);
void buad_uart_switch(unsigned int arg)
{
	vTaskSuspend( DMX_DisplayHandle );
	vTaskDelay( 50 / portTICK_RATE_MS );
	UART2_Open(UART2, arg);
	vTaskResume( DMX_DisplayHandle );
}
void uart_switch(unsigned char arg)
{
	vTaskSuspend( DMX_DisplayHandle );
	vTaskDelay( 100 / portTICK_RATE_MS );
	switch(arg)
	{
		case baud_250k:
		{
			UART2_Open(UART2, 250000);
			printf("baud_250k---\r\n");
			break;
		}
		case baud_500k:
		{
			UART2_Open(UART2, 500000);
			printf("baud_500k---\r\n");
			break;
		}
		case baud_1000k:
		{
			UART2_Open(UART2, 1000000);
			printf("baud_1000k---\r\n");
			break;
		}	
		default:
			break;
	}
	vTaskResume( DMX_DisplayHandle );
}
extern char blink_flag;
void data_handle_fuc(int event)
{
	menu_selection_t *menu_arg = &menu_selection;
	/*返回主菜单*/
	#if 0
	//menu_arg->munu_posit = menu_first;
	menu_arg->munu_select = First_level_menu ;
	menu_arg->munu_set_coding_select = 0;
	menu_arg->munu_set_select = lamps_coding;
	menu_arg->munu_set_param_select = Auto_Addr;
	menu_arg->munu_set_param_select_rgbset=0;
	#endif
	
	menu_arg->munu_select = First_level_menu ;
	menu_arg->First_level_menu_selection=First_level_menu_main_submenu	;
	menu_arg->Second_level_menu_selection=Second_level_menu_Equipment_first_submenu ;
	menu_arg->Third_level_menu_selection=Third_level_menu_first_submenu ;
	menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
	menu_arg->Fifth_level_menu_selection=Fifth_level_menu_first_submenu ;
	/*这边有挂起，如果没有挂起要考虑，按键3、4是否DMX需要挂起，串口速率转换时，是否需要挂起*/
	//vTaskResume( DMX_DisplayHandle );
	/*取消挂起渐变*/
	if(Equipment_set_param.Color_num == gradual_change)
	{
		 if(eSuspended==eTaskGetState(xgradual_displayHandle))vTaskResume(xgradual_displayHandle);
	}
	if(Equipment_set_param.Color_num == RACE_LAMP)
	{
		 if(eSuspended==eTaskGetState(RACELAMP_displayHandle))vTaskResume(RACELAMP_displayHandle);
	}
	
	blink_flag = 0;
	ui_display();
	vTaskSuspend( ui_display_DisplayHandle );//退出挂起UI
	//color_setup((color_type)key_type_staute.Color_num);//会导致函数冲突，考虑解决
#if debug
	printf("data_handle_fuc-------->>\r\n");
#endif
}
#if 0
void sm17500_save_data(void)
{
	vTaskSuspend( DMX_DisplayHandle );
	vTaskSuspend( ui_display_DisplayHandle );
	PB1=0;
	clear_screen();	
	display_string_8x16(5,1,"SM17500_load ...");
	
	
	PB1=1;
	vTaskResume( DMX_DisplayHandle );
	vTaskResume( ui_display_DisplayHandle );
}
#endif

char save_data(void)
{
	menu_selection_t *menu_arg = &menu_selection;
	
		//保存数据到flash中
		write_all_data();
	
	//if(menu_arg->save_flag)
	//{
		//vTaskSuspend( check_DisplayHandle );
	//手动跑点、传输速度不需要写码
	switch(menu_arg->Second_level_menu_selection)
	{
		case Second_level_menu_Chip_first_submenu:
		{
			switch(menu_arg->Third_level_menu_selection)
			{
				case Third_level_menu_third_submenu:
				{
					return 0;
				}
				case Third_level_menu_fourth_submenu:
				{
					return 0;
				}
			}
			break;
		}
		case Second_level_menu_Chip_second_submenu:
		{
			switch(menu_arg->Third_level_menu_selection)
			{
				case Third_level_menu_second_submenu:
				{
					return 0;
				}
				case Third_level_menu_third_submenu:
				{
					return 0;
				}
			}
			break;
		}
		case Second_level_menu_Chip_third_submenu:
		{
			switch(menu_arg->Third_level_menu_selection)
			{
				case Third_level_menu_fifth_submenu:
				{
					return 0;
				}
				case Third_level_menu_sixth_submenu:
				{
					return 0;
				}
			}
			break;
		}
	}
	

		
		vTaskSuspend( DMX_DisplayHandle );
		vTaskSuspend( ui_display_DisplayHandle );
		PB1=0;
		clear_screen();
		
		switch(menu_arg->First_level_menu_selection)
		{
			case First_level_menu_third_submenu:
			{
				switch(menu_arg->Second_level_menu_selection)
				{
					//SM17512X
					case Second_level_menu_Chip_first_submenu:
					{
						//写码时速率的选择
						uart_switch(chip_param_sm17512.Rate_num);
						switch(menu_arg->Third_level_menu_selection)
						{
							//灯具写码
							case Third_level_menu_first_submenu:
							{
								display_string_8x16(5,1,"SM1751X_lamps ...");
								sm1751x_Write_addr(chip_param_sm17512.lamps_num,chip_param_sm17512.coding_form_num,chip_param_sm17512.chip_chanle_num);
								break;
							}
							//通道写码
							case Third_level_menu_second_submenu:
							{
								display_string_8x16(5,1,"SM1751X_Channel ...");
								sm1751x_Write_addr(chip_param_sm17512.Channel_num,chip_param_sm17512.Channel_form,chip_param_sm17512.chip_chanle_num);
								break;
							}
							//参数设置
							case Third_level_menu_fifth_submenu:
							{
								display_string_8x16(5,1,"SM1751X_Param ...");
								sm1751x_write_arg(&SM1751_arg);
								sm1751x_write_arg(&SM1751_arg);
								vTaskDelay( 1000 / portTICK_RATE_MS );
								break;
							}
							default:
								break;
						}
						break;
					}
					//SM165XX
					case Second_level_menu_Chip_second_submenu:
					{
						//写码时速率的选择
						uart_switch(chip_param_sm165XX.Rate_num);
						switch(menu_arg->Third_level_menu_selection)
						{
							//灯具写码
							case Third_level_menu_first_submenu:
							{
								display_string_8x16(5,1,"SM165XX_lamps ...");
								SM165XX_Send_addr(chip_param_sm165XX.lamps_num,chip_param_sm165XX.chip_chanle_num ,chip_param_sm165XX.coding_form_num,0xd2,SM165XX_arg_set.Power_Up_Color);
								vTaskDelay( 1000 / portTICK_RATE_MS );
								SM165XX_Send_addr(chip_param_sm165XX.lamps_num,chip_param_sm165XX.chip_chanle_num ,chip_param_sm165XX.coding_form_num,0,SM165XX_arg_set.Power_Up_Color);
								vTaskDelay( 1000 / portTICK_RATE_MS );
								break;
							}
							
							//参数设置
							case Third_level_menu_fourth_submenu:
							{
								display_string_8x16(5,1,"SM165XX_Param ...");
								SM165XX_Send_addr(chip_param_sm165XX.lamps_num,chip_param_sm165XX.chip_chanle_num ,chip_param_sm165XX.coding_form_num,0xd2,SM165XX_arg_set.Power_Up_Color);							
								vTaskDelay( 1000 / portTICK_RATE_MS );
								SM165XX_Send_addr(chip_param_sm165XX.lamps_num,chip_param_sm165XX.chip_chanle_num ,chip_param_sm165XX.coding_form_num,0,SM165XX_arg_set.Power_Up_Color);
								vTaskDelay( 1000 / portTICK_RATE_MS );
								break;
							}
							default:
								break;
						}
						break;
					}
					//SM17500
					case Second_level_menu_Chip_third_submenu:
					{
						//写码时速率的选择
						uart_switch(chip_param_sm17500.Rate_num);
						switch(menu_arg->Third_level_menu_selection)
						{
							//通道写码
							case Third_level_menu_first_submenu:
							{
								display_string_8x16(1,16,"SM17500_LOAD");
								display_string_8x16(5,16,"Channel ...");
								sm17500x_send_add(chip_param_sm17500.Channel_num,chip_param_sm17500.Channel_form,chip_param_sm17500.chip_chanle_num);
								vTaskDelay( 1000 / portTICK_RATE_MS );
								
								break;
							}
							//自通道写码
							case Third_level_menu_second_submenu:
							{
								display_string_8x16(1,16,"SM17500_LOAD");
								display_string_8x16(5,1,"Self_Channel ...");
								sm17500x_send_self_add(chip_param_sm17500.Self_Channel_num,chip_param_sm17500.chip_chanle_num);
								break;
							}
							//电流增益
							case Third_level_menu_third_submenu:
							{
								display_string_8x16(1,16,"SM17500_LOAD");
								display_string_8x16(5,1,"Current_Gain ...");
								sm17500x_write_elect_arg(&SM17500_arg);
								break;
							}
							//自通道数
							case Third_level_menu_fourth_submenu:
							{
								display_string_8x16(1,16,"SM17500_LOAD");
								display_string_8x16(5,1,"Self_Channel_Num..");
								sm17500x_send_self_channel(chip_param_sm17500.chip_chanle_num,&SM17500_arg);
								break;
							}
							//参数设置
							case Third_level_menu_seventh_submenu:
							{
								display_string_8x16(1,16,"SM17500_LOAD");
								display_string_8x16(5,16,"Param_set..");
								sm17500x_write_arg(&SM17500_arg);
								break;
							}
							default:
								break;
						}
						break;
					}
					
				}
				break;
			}
		}
#if 0			
		switch(menu_arg->munu_posit)
		{
			case menu_set:
			{
				switch(menu_arg->chip_model)
				{
					case SM1751X:
					{
						switch(menu_arg->munu_set_select)
						{
							case Param_set:
							{
								display_string_8x16(5,1,"SM1751X_Param ...");	
								sm1751x_write_arg(&SM1751_arg);
								sm1751x_write_arg(&SM1751_arg);
					       vTaskDelay( 1000 / portTICK_RATE_MS );
								
								break;
							}
							case lamps_coding:
							{
								display_string_8x16(5,1,"SM1751X_lamps ...");
								sm1751x_Write_addr(chip_param_sm17512.lamps_num,chip_param_sm17512.coding_form_num,chip_param_sm17512.chip_chanle_num);
								break;
							}
							case Channel_coding:
							{
								display_string_8x16(5,1,"SM1751X_Channel ...");
								sm1751x_Write_addr(chip_param_sm17512.Channel_num,chip_param_sm17512.Channel_form,chip_param_sm17512.chip_chanle_num);
								
								break;
							}							
							default:
								break;
						}
						break;
					}
					case SM165XX:
					{
						switch(menu_arg->munu_set_select)
						{
							case SM165xx_Param_set:
							{
								display_string_8x16(5,1,"SM165XX_Param ...");								
								SM165XX_Send_addr(chip_param_sm165XX.lamps_num,chip_param_sm165XX.chip_chanle_num ,chip_param_sm165XX.coding_form_num,0xd2,SM165XX_arg_set.Power_Up_Color);							
								vTaskDelay( 1000 / portTICK_RATE_MS );
								SM165XX_Send_addr(chip_param_sm165XX.lamps_num,chip_param_sm165XX.chip_chanle_num ,chip_param_sm165XX.coding_form_num,0,SM165XX_arg_set.Power_Up_Color);
								vTaskDelay( 1000 / portTICK_RATE_MS );
								
								break;
							}
							case SM165xx_lamps_coding:
							{
								display_string_8x16(5,1,"SM165XX_lamps ...");								
								SM165XX_Send_addr(chip_param_sm165XX.lamps_num,chip_param_sm165XX.chip_chanle_num ,chip_param_sm165XX.coding_form_num,0xd2,SM165XX_arg_set.Power_Up_Color);
								vTaskDelay( 1000 / portTICK_RATE_MS );
								SM165XX_Send_addr(chip_param_sm165XX.lamps_num,chip_param_sm165XX.chip_chanle_num ,chip_param_sm165XX.coding_form_num,0,SM165XX_arg_set.Power_Up_Color);
								vTaskDelay( 1000 / portTICK_RATE_MS );
								break;
							}
						
							default:
								break;
						}				
						break;
					}
					case SM17500:
					{
						switch(menu_arg->munu_set_select)
						{
							case SM17500_lamps_coding :
							{
								display_string_8x16(1,16,"SM17500_LOAD");
								display_string_8x16(5,16,"Channel ...");
								sm17500x_send_add(chip_param_sm17500.Channel_num,chip_param_sm17500.Channel_form,chip_param_sm17500.chip_chanle_num);
								vTaskDelay( 1000 / portTICK_RATE_MS );
								vTaskDelay( 1000 / portTICK_RATE_MS );
								break;
							}
							case SM17500_self_lamps_coding :
							{
								display_string_8x16(1,16,"SM17500_LOAD");
								display_string_8x16(5,1,"Self_Channel ...");
								sm17500x_send_self_add(chip_param_sm17500.Self_Channel_num,chip_param_sm17500.chip_chanle_num);
								break;
							}
							case Current_Gain :
							{
								display_string_8x16(1,16,"SM17500_LOAD");
								display_string_8x16(5,1,"Current_Gain ...");
								sm17500x_write_elect_arg(&SM17500_arg);
								break;
							}
							case Self_Lamps_Num :
							{
								display_string_8x16(1,16,"SM17500_LOAD");
								display_string_8x16(5,1,"Self_Lamps_Num..");
								sm17500x_send_self_channel(chip_param_sm17500.chip_chanle_num,&SM17500_arg);
								break;
							}
							case SM17500_Param_set :
							{
								display_string_8x16(1,16,"SM17500_LOAD");
								display_string_8x16(5,16,"Param_set..");
								sm17500x_write_arg(&SM17500_arg);
								break;
							}							
						}
						break;
					}	
					 case  SM16703:
					 {
						 display_string_8x16(3,16,"SM16703_LOAD");
						 break;
					 }
					 case  SM16804:
					 {
						 display_string_8x16(3,16,"SM16804_LOAD");
						 break;
					 }					 
					default:
						break;
			}				
				break;
			}
		}
#endif


		//vTaskResume( check_DisplayHandle );
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//写完码后的速率选择
		buad_uart_switch(frequency_arg[Equipment_set_param.Output_frequency_param]);
		vTaskResume( DMX_DisplayHandle );		
		vTaskResume( ui_display_DisplayHandle );	
		
		PB1=1;
		return 1;
	//}
//	return 0;
}




/***************************************************** 
 * @Function:  key1_fuc
 * @Description:   key1 short event handle
 * @param  event 
 * @retval void
******************************************************/


void key1_fuc(int event)
{

	blink_flag = 0;
	menu_selection_t *menu_arg = &menu_selection;

		switch(menu_arg->munu_select)
		{
			case First_level_menu:
			{
				/*判断渐变任务是否是挂起，如果不是就挂起*/
				if(eSuspended!=eTaskGetState(xgradual_displayHandle))vTaskSuspend(xgradual_displayHandle);
			  //跑马挂起
				if(eSuspended!=eTaskGetState(RACELAMP_displayHandle))vTaskSuspend(RACELAMP_displayHandle);
				vTaskResume( ui_display_DisplayHandle );//开始工作时刷新UI   
				menu_arg->First_level_menu_selection++;
				if(menu_arg->First_level_menu_selection>=First_level_menu_max_submenu)
				{
					menu_arg->First_level_menu_selection=First_level_menu_first_submenu;
				}
				break;
			}
			case Second_level_menu:
			{
				menu_arg->Second_level_menu_selection++;
				switch(menu_selection.First_level_menu_selection)
				{
					case First_level_menu_first_submenu :
					{
						 
						if(menu_arg->Second_level_menu_selection==Second_level_menu_Equipment_max_submenu)
						{
							menu_arg->Second_level_menu_selection=Second_level_menu_Equipment_first_submenu;
						}
						break;
					}
					case First_level_menu_second_submenu :
					{
						if(menu_arg->Second_level_menu_selection==Second_level_menu_Effect_max_submenu)
						{
							menu_arg->Second_level_menu_selection=Second_level_menu_Effect_first_submenu;
						}
						break;
					}
					case First_level_menu_third_submenu	 :
					{
						if(menu_arg->Second_level_menu_selection==Second_level_menu_Chip_max_submenu)
						{
							menu_arg->Second_level_menu_selection=Second_level_menu_Chip_first_submenu;
						}
						break;
					}					
				}
				
				break;
			}
			case Third_level_menu:
			{
				menu_arg->Third_level_menu_selection++;
				switch(menu_selection.First_level_menu_selection)
				{
					case First_level_menu_first_submenu :
					{
						 
						  
						break;
					}
					case First_level_menu_second_submenu :
					{
						switch(menu_selection.Second_level_menu_selection)
						 {
							
							case Second_level_menu_Effect_second_submenu:
							{
								if(menu_arg->Third_level_menu_selection==Third_level_menu_sixth_submenu)
								menu_arg->Third_level_menu_selection=Third_level_menu_first_submenu;
								break;
							}
						 }
						break;
					}
					case First_level_menu_third_submenu	 :
					{
						switch(menu_selection.Second_level_menu_selection)
						{
							/*SM1751X 5级*/
							case Second_level_menu_Chip_first_submenu:
							{
								if(menu_arg->Third_level_menu_selection==Third_level_menu_sixth_submenu)
								menu_arg->Third_level_menu_selection=Third_level_menu_first_submenu;
								break;
							}
							/*SM165XX 4级*/
							case Second_level_menu_Chip_second_submenu:
							{
								if(menu_arg->Third_level_menu_selection==Third_level_menu_fifth_submenu)
								menu_arg->Third_level_menu_selection=Third_level_menu_first_submenu;
								break;
							}
							/*SM17500 6级*/
							case Second_level_menu_Chip_third_submenu:
							{
								if(menu_arg->Third_level_menu_selection==Third_level_menu_max_submenu)
								menu_arg->Third_level_menu_selection=Third_level_menu_first_submenu;
								break;
							}
							/*SM16703P 1级*/
							case Second_level_menu_Chip_fourth_submenu:
							{
								if(menu_arg->Third_level_menu_selection==Third_level_menu_second_submenu)
								menu_arg->Third_level_menu_selection=Third_level_menu_first_submenu;
								break;
							}
							/*SM16804P 2级*/
							case Second_level_menu_Chip_fifth_submenu:
							{
								if(menu_arg->Third_level_menu_selection==Third_level_menu_third_submenu)
								menu_arg->Third_level_menu_selection=Third_level_menu_first_submenu;
								break;
							}							
						}
						break;
					}					
				}				
				break;
			}
			case Fourth_level_menu:
			{
				menu_arg->Fourth_level_menu_selection++;
				switch(menu_selection.First_level_menu_selection)
				{

					case First_level_menu_third_submenu	 :
					{
						switch(menu_selection.Second_level_menu_selection)
						{
							/*SM1751X 5级*/
							case Second_level_menu_Chip_first_submenu:
							{
								switch(menu_selection.Third_level_menu_selection)
								{
									case Third_level_menu_first_submenu:
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_fourth_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_second_submenu :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_fourth_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_third_submenu	:
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_second_submenu 	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_fourth_submenu		:
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_second_submenu 	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_fifth_submenu	:
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_sixth_submenu	 	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}										
								}
								break;
							}
							/*SM165XX 4级*/
							case Second_level_menu_Chip_second_submenu:
							{
								switch(menu_selection.Third_level_menu_selection)
								{
									case Third_level_menu_first_submenu :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_fourth_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_second_submenu  :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_second_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_third_submenu	 :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_second_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_fourth_submenu	 :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_second_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
								}
								break;
							}
							/*SM17500 7级*/
							case Second_level_menu_Chip_third_submenu:
							{
								switch(menu_selection.Third_level_menu_selection)
								{
									case Third_level_menu_first_submenu :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_fourth_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_second_submenu  :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_third_submenu		)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_third_submenu	 :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_fifth_submenu		)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_fourth_submenu	 :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_second_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_fifth_submenu		 :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_second_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_sixth_submenu		 :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_second_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_seventh_submenu		 :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_max_submenu 	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}									
								}
								break;
							}
							/*SM16703P 1级*/
							case Second_level_menu_Chip_fourth_submenu:
							{
								switch(menu_selection.Third_level_menu_selection)
								{
									case Third_level_menu_first_submenu :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_second_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									
								}
								break;
							}
							/*SM16804P 2级*/
							case Second_level_menu_Chip_fifth_submenu:
							{
								switch(menu_selection.Third_level_menu_selection)
								{
									case Third_level_menu_first_submenu :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_second_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
									case Third_level_menu_second_submenu  :
									{
										if(menu_arg->Fourth_level_menu_selection==Fourth_level_menu_fifth_submenu	)
											menu_arg->Fourth_level_menu_selection=Fourth_level_menu_first_submenu ;
										break;
									}
								
								}
								break;
							}							
						}
						break;
					}					
				}					
				break;
			}
			case Fifth_level_menu:
			{
				switch(menu_selection.First_level_menu_selection)
				{
					case First_level_menu_third_submenu	 :
					{
						switch(menu_selection.Second_level_menu_selection)
						{
							case Second_level_menu_Chip_first_submenu :
							{
								switch(menu_selection.Third_level_menu_selection)
								{
									case Third_level_menu_fifth_submenu	:
									{
										switch(menu_selection.Fourth_level_menu_selection)
										{
											case Fourth_level_menu_fourth_submenu	:
											{
												menu_arg->Fifth_level_menu_selection++;
												if(menu_arg->Fifth_level_menu_selection==Fifth_level_menu_max_submenu 	)
												menu_arg->Fifth_level_menu_selection=Fifth_level_menu_first_submenu  ;
												break;
											}
											case Fourth_level_menu_fifth_submenu		:
											{
												menu_arg->Fifth_level_menu_selection++;
												if(menu_arg->Fifth_level_menu_selection==Fifth_level_menu_max_submenu 	)
												menu_arg->Fifth_level_menu_selection=Fifth_level_menu_first_submenu  ;
												break;
											}
										}
										break;
									}
								}
								break;
							}
							
							case Second_level_menu_Chip_third_submenu	  :
							{
								switch(menu_selection.Third_level_menu_selection)
								{
									case Third_level_menu_seventh_submenu			:
									{
										switch(menu_selection.Fourth_level_menu_selection)
										{
											case Fourth_level_menu_first_submenu 	:
											{
												menu_arg->Fifth_level_menu_selection++;
												if(menu_arg->Fifth_level_menu_selection==Fifth_level_menu_max_submenu 	)
												menu_arg->Fifth_level_menu_selection=Fifth_level_menu_first_submenu  ;
												break;
											}
											
										}
										break;
									}
								}
								break;
							}
						}
						break;
					}						
				}
				break;
			}
			default:
				break;
		}	
#if 0	
	/*位置的选择*/
	switch(menu_selection.munu_posit)
	{
		/*第一层设置*/
		case menu_first:
		{
			/*判断渐变任务是否是挂起，如果不是就挂起*/
			if(eSuspended!=eTaskGetState(xgradual_displayHandle))vTaskSuspend(xgradual_displayHandle);

		//	vTaskSuspend( DMX_DisplayHandle );			
			vTaskResume( ui_display_DisplayHandle );//开始工作时刷新UI
			menu_arg->munu_select++;
			if(menu_arg->munu_select >= key_event_max)
			{
//				menu_arg->munu_select = meum_main;
//				send_event(Date_handle_event);
				menu_arg->munu_select = effect_choice;
			}
			break;
		}
		
		
		/*主设置菜单设置里面的第二层设置*/
		case menu_set:
		{			
			menu_arg->munu_set_select++;
			/*不同芯片的菜单页面不一样的设置*/
			switch(menu_arg->chip_model)
			{
				case SM1751X:
				{
					if(menu_arg->munu_set_select == menu_set_max) 
					{
						menu_arg->munu_set_select = lamps_coding;
						//menu_arg->munu_posit = menu_first;//在第二层转回第一层
						//menu_arg->munu_select = menu_setup;
					}					
					break;
				}
				case SM165XX:
				{
					if(menu_arg->munu_set_select == SM165xx_menu_set_max) 
					{
						menu_arg->munu_set_select = SM165xx_lamps_coding;
						//menu_arg->munu_posit = menu_first;
						//menu_arg->munu_select = menu_setup;
					}					
					break;
				}
				case SM17500:
				{
					if(menu_arg->munu_set_select == SM17500_menu_set_max) 
					{
						menu_arg->munu_set_select = SM17500_lamps_coding;
						//menu_arg->munu_posit = menu_first;
						//menu_arg->munu_select = menu_setup;
					}					
					break;
				}
				case SM16804:
				{
					if(menu_arg->munu_set_select == RZ_type_MAX) 
					{
						menu_arg->munu_set_select = CHIP_NUM;

					}					
					break;				
				}
				default:
					break;
			}

					
			break;
		}
		/*菜单设置*第三层设置*/
		case menu_coding_set:
		{
	
			switch(menu_selection.chip_model)
			{
				case SM1751X:
				{
					switch(menu_arg->munu_set_select)
					{
						case Param_set:
						{
							menu_arg->munu_set_param_select++;
						  if(menu_arg->munu_set_param_select == Param_type_Max)
							{
								menu_arg->munu_set_param_select = Auto_Addr;
								//menu_arg->munu_posit = menu_set;
							}
							break;
						}
						case lamps_coding:
						{
							menu_arg->munu_set_coding_select++;
							if(menu_arg->munu_set_coding_select == menu_coding_set_max)
							{
								menu_arg->munu_set_coding_select = coding_addr;
								//menu_arg->munu_posit = menu_set;
							}												
							break;
						}
						case Channel_coding:
						{
							menu_arg->munu_set_coding_select++;
							if(menu_arg->munu_set_coding_select == menu_coding_set_max)
							{
								menu_arg->munu_set_coding_select = coding_addr;
								//menu_arg->munu_posit = menu_set;
							}
							break;
						}
						default:
							break;
					}
					break;
				}
				case SM165XX:
				{
					switch(menu_arg->munu_set_select)
					{
						case SM165xx_lamps_coding:
						{
							menu_arg->munu_set_coding_select++;
							if(menu_arg->munu_set_coding_select == SM165xx_menu_set_max)
							{
								menu_arg->munu_set_coding_select = SM165xx_lamps_coding;
								//menu_arg->munu_posit = menu_set;
							}							
							break;
						}

						default:
							break;
					}
					break;
				}
				case SM17500:
				{
					switch(menu_arg->munu_set_select)
					{
						case SM17500_lamps_coding:
						{
							/*设置项的移动*/
							menu_arg->munu_set_coding_select++;
							if(menu_arg->munu_set_coding_select == menu_coding_set_max)
							{
								menu_arg->munu_set_coding_select = coding_addr;
								//menu_arg->munu_posit = menu_set;
							}							
							break;
						}
						case SM17500_self_lamps_coding:
						{
							/*设置项的移动*/
							menu_arg->munu_set_coding_select++;
							if(menu_arg->munu_set_coding_select == coding_chip)
							{
								menu_arg->munu_set_coding_select = coding_addr;
								//menu_arg->munu_posit = menu_set;
							}							
							break;
						}
						case Current_Gain:
						
						{
							/*设置项的移动*/
							menu_arg->munu_set_coding_select++;
							if(menu_arg->munu_set_coding_select == RGB_MAX)
							{
								menu_arg->munu_set_coding_select = RGB_R;
								//menu_arg->munu_posit = menu_set;
							}							
							break;
						}	

						case SM17500_Param_set:						
						{
							menu_arg->munu_set_param_select++;
						  if(menu_arg->munu_set_param_select == SM17500_Param_type_Max)
							{
								menu_arg->munu_set_param_select = SM17500_self_check_color;
								//menu_arg->munu_posit = menu_set;
							}
								break;
						}
							
						default:
							break;
					}
					break;
				}
				case SM16804:
				{
					switch(menu_arg->munu_set_select)
					{
						case IOUT_NUM:
						{
							menu_arg->munu_set_coding_select++;
							if(menu_arg->munu_set_coding_select == RGB_MAX)
							{
								menu_arg->munu_set_coding_select = RGB_R;
								//menu_arg->munu_posit = menu_set;
							}							
							break;
						}

						default:
							break;
					}
					break;
				}				
				default:
				break;
			}
			break;
		}
		/*参数设置*/
		case menu_coding_arg_set:
		{

			switch(menu_arg->chip_model)
			{
				case SM1751X:
				{
					if(menu_arg->munu_set_param_select == Selfcheck_color || menu_arg->munu_set_param_select == Current_Regulation)
					{
						menu_arg->munu_set_param_select_rgbset++;
						if(menu_arg->munu_set_param_select_rgbset==RGB_MAX)
						{
							menu_arg->munu_set_param_select_rgbset=RGB_R;
							//menu_arg->munu_posit = menu_coding_set;
						}
												
					}					
					break;
				}
				case SM165XX:
				{
					break;
				}
				case SM17500:
				{
					if(menu_arg->munu_set_param_select == SM17500_self_check_color)
					{
						menu_arg->munu_set_param_select_rgbset++;
						if(menu_arg->munu_set_param_select_rgbset==RGB_MAX)
						{
							menu_arg->munu_set_param_select_rgbset=RGB_R;
							//menu_arg->munu_posit = menu_coding_set;
						}
												
					}					
					break;
				}				
				default:
				break;
			}
			
			break;
		}
	}
#endif
	ui_display();
	
#if debug	
	//printf("key_flag_staute.key1_flag%d=======================1\r\n",key_type_staute.munu_select);
	printf("key_fuc=======================1\r\n");
#endif
}


/***************************************************** 
 * @Function:  key2_fuc
 * @Description:   key4 short event handle
 * @param  event 
 * @retval void
******************************************************/
void key2_fuc(int event)
{
blink_flag = 0;
menu_selection_t *menu_arg = &menu_selection;
	
	
	
		//无设置时，按到返回
		if(menu_arg->First_level_menu_selection==First_level_menu_main_submenu)
		{		
			return;
		}
		switch(menu_arg->munu_select)
		{
			case First_level_menu:
			{
				menu_arg->munu_select = Second_level_menu;				
				break;
			}
			//第二层要到第三层
			case Second_level_menu:
			{	
				switch(menu_arg->First_level_menu_selection)
				{
					//设备设置
					case First_level_menu_first_submenu:
					{
						
					//从flash读取数据
						read_all_data();
						break;
					}
					//效果设置
					case First_level_menu_second_submenu:
					{
						
						//从flash读取数据
						read_all_data();
						break;
					}
				}					
				menu_arg->munu_select = Third_level_menu;											
				break;
			}
			case Third_level_menu:
			{
				
				// 效果设置、设备设置3层
				
				switch(menu_arg->First_level_menu_selection)
				{
					//效果设置、设备设置只有三层
					case First_level_menu_first_submenu:
					{
						
						ui_display();
					 return;
					}
					case First_level_menu_second_submenu:
					{
						
						ui_display();
						return;
					}
					case First_level_menu_third_submenu:
					{
						switch(menu_arg->Second_level_menu_selection)
						{
							//SM1751X
							case Second_level_menu_Chip_first_submenu:
							{
								switch(menu_arg->Third_level_menu_selection)
								{
									//手动跑点（跑点任务的解除挂起）菜单3
									case Third_level_menu_third_submenu:
									{										
										//进入手动跑点标志位										
										menu_selection.Run_manually_point_flag=1;
										if(eSuspended==eTaskGetState(Run_manually_pointHandle))vTaskResume(Run_manually_pointHandle);
										break;
									}
								}
								break;
							}
							//SM165XX
							case Second_level_menu_Chip_second_submenu:
							{
									switch(menu_arg->Third_level_menu_selection)
									{
										//手动跑点（跑点任务的解除挂起）菜单2
										case Third_level_menu_second_submenu:
										{
											menu_selection.Run_manually_point_flag=1;
											if(eSuspended==eTaskGetState(Run_manually_pointHandle))vTaskResume(Run_manually_pointHandle);
											break;
										}
									}
								
								break;
							}
							//SM17500
							case Second_level_menu_Chip_third_submenu:
							{
								switch(menu_arg->Third_level_menu_selection)
									{
										//手动跑点（跑点任务的解除挂起）菜单5
										case Third_level_menu_fifth_submenu:
										{
											menu_selection.Run_manually_point_flag=1;
											if(eSuspended==eTaskGetState(Run_manually_pointHandle))vTaskResume(Run_manually_pointHandle);
											break;
										}
									}
								
								break;
							}
							
						}
						break;
					}
				}
				//从flash读取数据
				read_all_data();
				menu_arg->munu_select = Fourth_level_menu;
				break;
			}
			case Fourth_level_menu:
			{
								
				switch(menu_arg->Second_level_menu_selection)
				{
					//SM1751X
					case Second_level_menu_Chip_first_submenu :
					{
						//不是参数设置就保存数据
						if(menu_arg->Third_level_menu_selection !=Third_level_menu_fifth_submenu	)
						{
							save_data();							
							ui_display();
							return;
						}
						else
						{
							//从flash读取数据
							read_all_data();
						}
						break;
					}
					//SM165XX
					case Second_level_menu_Chip_second_submenu :
					{
						//不是参数设置就保存数据
						if(menu_arg->Third_level_menu_selection !=Third_level_menu_fourth_submenu	)
						{
							save_data();							
							ui_display();
							return;
						}
						else
						{
							//从flash读取数据
							read_all_data();
						}
						break;
					}
					//SM17500
					case Second_level_menu_Chip_third_submenu	:
					{
						//不是参数设置就保存数据
						if(menu_arg->Third_level_menu_selection !=Third_level_menu_seventh_submenu	)
						{
							save_data();						
							ui_display();
							return;
						}
						else
						{
							//从flash读取数据
							read_all_data();
						}
						break;
					}
					//SM16703P(按确定保存数据)
					case Second_level_menu_Chip_fourth_submenu		:
					{	
						//保存数据到flash中
						write_all_data();
						ui_display();	
						return;
					
					}
					//SM16804P(按确定保存数据)
					case Second_level_menu_Chip_fifth_submenu		:
					{	
						//保存数据到flash中
						write_all_data();
						ui_display();
						return;
						
					}
				}
				menu_arg->munu_select = Fifth_level_menu;
				break;
			}
			case Fifth_level_menu:
			{
				save_data();
				break;
			}
			default:
				break;
		}	
#if 0	
	//read_usr_data();
	if(menu_arg == NULL) 
	{
#if debug
		printf("key4_fuc_erro\n");
#endif
		return;
	}
	if(menu_arg->munu_select == menu_setup)
	{
		if(menu_arg->munu_posit == menu_first)
		{
			menu_arg->munu_posit = menu_set;
			ui_display();
			return;
		}
		if(menu_arg->munu_posit == menu_set)
		{
			//从flash读出数据
			read_all_data();
	
			menu_arg->munu_posit = menu_coding_set;
			ui_display();
			return;
		}
		if(menu_arg->munu_posit == menu_coding_set )
		{
			//从flash读出数据			
			//if(menu_arg->munu_posit != menu_coding_arg_set)read_all_data();
	
			switch(menu_arg->chip_model)
			{
				case SM1751X:
				{
					if( menu_arg->munu_set_select == Param_set)
					{
						menu_arg->munu_posit = menu_coding_arg_set;
			     return;
					}
					break;
				}
				case SM165XX:
				{
					if( menu_arg->munu_set_select == SM165xx_Param_set)
					{
						menu_arg->munu_posit = menu_coding_arg_set;
			     return;
					}					
					break;
				}
				case SM17500:
				{
					if( menu_arg->munu_set_select == SM17500_Param_set)
					{
						menu_arg->munu_posit = menu_coding_arg_set;
			     return;
					}					
					break;
				}				
				default:
				break;
			}			
			
		}
		
		ui_display();
		return;
	}
	if(menu_arg->munu_select != meum_main)
	{		
		send_event(Date_handle_event);
#if debug			
		printf("key4_fuc_ok\n");
#endif
	}
#endif	//全局

	ui_display();		
#if debug	
	printf("key4_fuc=======================4\r\n");
#endif
}

/***************************************************** 
 * @Function:  key3_fuc
 * @Description:   key2 short event handle
 * @param  event 
 * @retval void
******************************************************/
void key3_fuc(int event)
{
  blink_flag = 0;	
	
	Protocol_output_parameter_t *Equipment_set_arg =&Equipment_set_param;
	menu_selection_t *menu_arg = &menu_selection;
	chip_param_t *chip_param =NULL;
	DMX512_arg_t *SM512_arg_t = NULL;
	RZ_arg_t *RZ_arg = NULL;
	SM165XX_arg_t *SM165XX_arg_t = &SM165XX_arg_set;
	/*是否加载数据*/
	//if (save_data())return;
	
	//vTaskSuspend( DMX_DisplayHandle );
	
	switch(menu_arg->munu_select)
	{		
		//位置第三层
		case Third_level_menu:
		{
			switch(menu_arg->First_level_menu_selection)
			{
				case First_level_menu_first_submenu:
				{
					switch(menu_arg->Second_level_menu_selection)
					{
						//带载通道设置
						case Second_level_menu_Equipment_first_submenu :
						{
							if(Equipment_set_arg->On_load_channe_param>=2048)
							{
								Equipment_set_arg->On_load_channe_param=1;
								write_all_data();
								ui_display();
								return;
							}
							Equipment_set_arg->On_load_channe_param = Equipment_set_arg->On_load_channe_param*2;
							write_all_data();
							break;
						}
						//时钟频率设置
						case Second_level_menu_Equipment_second_submenu  :
						{
							Equipment_set_arg->Output_frequency_param++;
							if(Equipment_set_arg->Output_frequency_param==Freq_baud_max)
								Equipment_set_arg->Output_frequency_param=Freq_baud_200k;
							buad_uart_switch(frequency_arg[Equipment_set_arg->Output_frequency_param]);
							write_all_data();
							break;
						}
						//播放速率设置
						case Second_level_menu_Equipment_third_submenu	:  
						{
							Equipment_set_arg->Play_speed_param++;
							write_all_data();
							break;
						}
						//颜色数
						case Second_level_menu_Equipment_fourth_submenu	:
						{
							Equipment_set_arg->colour_num_param++;
							if(Equipment_set_arg->colour_num_param==Colour_max)
								Equipment_set_arg->colour_num_param=Colour_R;
							write_all_data();	
							break;
						}
						case Second_level_menu_Equipment_fifth_submenu	:
						{
							Equipment_set_arg->Protocol_type_param++;
							if(Equipment_set_arg->Protocol_type_param==Protocol_max)
								Equipment_set_arg->Protocol_type_param=DMX512;
							write_all_data();
							break;
						}
						case Second_level_menu_Equipment_sixth_submenu	:
						{
							Equipment_set_arg->RZ_chip_param++;
							if(Equipment_set_arg->RZ_chip_param==RZ_chip_max)
								Equipment_set_arg->RZ_chip_param=RZ_SM16703;
							write_all_data();
							break;
						}
					}
					break;
				}
				case First_level_menu_second_submenu  :
				{
					switch(menu_arg->Second_level_menu_selection)
					{
						//效果设置
						case Second_level_menu_Effect_first_submenu  :
						{
							Equipment_set_arg->Color_num++;
							if(Equipment_set_arg->Color_num >=color_max)Equipment_set_arg->Color_num = loop_lamp ;	
							write_all_data();							
							break;
						}
						//各通道灰度设置
						case Second_level_menu_Effect_second_submenu   :
						{
							switch(menu_arg->Third_level_menu_selection)
							{
								case Third_level_menu_first_submenu:
								{
									Equipment_set_arg->Adjust_Red_data++;
									break;
								}
								case Third_level_menu_second_submenu:
								{
									Equipment_set_arg->Adjust_Green_data++;
									break;
								}
								case Third_level_menu_third_submenu:
								{
									Equipment_set_arg->Adjust_Blue_data++;
									break;
								}
								case Third_level_menu_fourth_submenu:
								{
									Equipment_set_arg->Adjust_Write_data++;
									break;
								}
								case Third_level_menu_fifth_submenu:
								{
									Equipment_set_arg->Adjust_Yellow_data++;
									break;
								}
							}
							write_all_data();
							break;
						}						
					}
					break;
				}
				
			}
			break;
		}
		//位置在第四层
		case Fourth_level_menu  :
		{
			switch(menu_arg->First_level_menu_selection)
			{
				case First_level_menu_third_submenu	:
				{
					switch(menu_arg->Second_level_menu_selection)
					{
						case Second_level_menu_Chip_first_submenu  :
						{
							chip_param = &chip_param_sm17512;
							SM512_arg_t =&SM1751_arg;
							//SM1751X
							switch(menu_arg->Third_level_menu_selection)
							{
								//灯具写码
								case Third_level_menu_first_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										//起始地址
										case Fourth_level_menu_first_submenu :
										{
											chip_param->lamps_num++;
										  
											break;
										}
										//灯具类型
										case Fourth_level_menu_second_submenu  :
										{
											chip_param->coding_form_num++;
											if(chip_param->coding_form_num == RGB_TAPE_MAX)chip_param->coding_form_num = RGB;
											
											break;
										}
										//通道数
										case Fourth_level_menu_third_submenu	 :
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num*2;
											if(chip_param->chip_chanle_num>2048)chip_param->chip_chanle_num=512;
											break;
										}
									}
									break;
								}
								//通道写码
								case Third_level_menu_second_submenu  :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										//起始地址
										case Fourth_level_menu_first_submenu :
										{
											chip_param->Channel_num++;
											break;
										}
										//步进数
										case Fourth_level_menu_second_submenu  :
										{
											chip_param->Channel_form++;
											break;
										}
										//通道数
										case Fourth_level_menu_third_submenu	 :
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num*2;
											if(chip_param->chip_chanle_num>2048)chip_param->chip_chanle_num=512;
											break;
										}
									}
									break;
								}
								//手动跑点
								case Third_level_menu_third_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											chip_param->Run_manually_point++;
											break;
										}
										
									}
									break;
								}
								//传输速度
								case Third_level_menu_fourth_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											chip_param->Rate_num++;
											if(chip_param->Rate_num == baud_max)chip_param->Rate_num = baud_250k;
											//速率转化
											uart_switch(chip_param->Rate_num);
											break;
										}
										
									}
									break;
								}
								
							}
							break;
						}
						//SM165XX
						case Second_level_menu_Chip_second_submenu   :
						{
							chip_param = &chip_param_sm165XX;
							switch(menu_arg->Third_level_menu_selection)
							{
								//灯具写码
								case Third_level_menu_first_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										//起始地址
										case Fourth_level_menu_first_submenu :
										{
											chip_param->lamps_num++;
											break;
										}
										//灯具类型
										case Fourth_level_menu_second_submenu  :
										{
											chip_param->coding_form_num++;
											if(chip_param->coding_form_num == RGB_TAPE_MAX)chip_param->coding_form_num = RGB;
											break;
										}
										//通道数
										case Fourth_level_menu_third_submenu	 :
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num*2;
											if(chip_param->chip_chanle_num>2048)chip_param->chip_chanle_num=512;
											break;
										}
									}
									break;
								}
								//手动跑点
								case Third_level_menu_second_submenu  :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											chip_param->Run_manually_point++;
											break;
										}
										
									}
									break;
								}
								//传输速度
								case Third_level_menu_third_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											chip_param->Rate_num++;
											if(chip_param->Rate_num == baud_max)chip_param->Rate_num = baud_250k;
											uart_switch(chip_param->Rate_num);
											break;
										}
										
									}
									break;
								}
								
								
							}
							break;
						}
						//SM17500
						case Second_level_menu_Chip_third_submenu		:  
						{
							chip_param = &chip_param_sm17500;
							SM512_arg_t =&SM17500_arg;
							switch(menu_arg->Third_level_menu_selection)
							{
								//通道写码
								case Third_level_menu_first_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										//起始地址
										case Fourth_level_menu_first_submenu :
										{
											chip_param->Channel_num++;
											break;
										}
										//步进数
										case Fourth_level_menu_second_submenu  :
										{
											chip_param->Channel_form++;
											break;
										}
										//通道数
										case Fourth_level_menu_third_submenu	 :
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num*2;
											if(chip_param->chip_chanle_num>2048)chip_param->chip_chanle_num=512;
											break;
										}
									}
									break;
								}
								//自通道写码
								case Third_level_menu_second_submenu  :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										//起始地址
										case Fourth_level_menu_first_submenu :
										{
											chip_param->Self_Channel_num++;
											break;
										}
										//通道数
										case Fourth_level_menu_second_submenu  :
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num*2;
											if(chip_param->chip_chanle_num>2048)chip_param->chip_chanle_num=512;
											break;
										}
										
									}
									break;
								}
								//电流增益
								case Third_level_menu_third_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											SM512_arg_t->RGBW_IR++;
								
											switch(SM512_arg_t->EN0_EN1)
											{
												case SM17500_No_current_gain:
												{
													SM512_arg_t->RGBW_IR=0;
													break;
												}
												case SM17500_4bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IR>15)SM512_arg_t->RGBW_IR=0;
														
													break;
												}
												case SM17500_5bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IR>31)SM512_arg_t->RGBW_IR=0;
													break;
												}
												case SM17500_6bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IR>63)SM512_arg_t->RGBW_IR=0;
													break;
												}											
											}
											break;
										}
										case Fourth_level_menu_second_submenu :
										{
											SM512_arg_t->RGBW_IG++;
											switch(SM512_arg_t->EN0_EN1)
											{
												case SM17500_No_current_gain:
												{
													SM512_arg_t->RGBW_IG=0;
													break;
												}
												case SM17500_4bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IG>15)SM512_arg_t->RGBW_IG=0;
														
													break;
												}
												case SM17500_5bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IG>31)SM512_arg_t->RGBW_IG=0;
													break;
												}
												case SM17500_6bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IG>63)SM512_arg_t->RGBW_IG=0;
													break;
												}											
											}
											break;
										}
										case Fourth_level_menu_third_submenu :
										{
											SM512_arg_t->RGBW_IB++;
											switch(SM512_arg_t->EN0_EN1)
											{
												case SM17500_No_current_gain:
												{
													SM512_arg_t->RGBW_IB=0;
													break;
												}
												case SM17500_4bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IB>15)SM512_arg_t->RGBW_IB=0;
														
													break;
												}
												case SM17500_5bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IB>31)SM512_arg_t->RGBW_IB=0;
													break;
												}
												case SM17500_6bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IB>63)SM512_arg_t->RGBW_IB=0;
													break;
												}											
											}
											break;
										}
										case Fourth_level_menu_fourth_submenu :
										{
											SM512_arg_t->RGBW_IW++;
											switch(SM512_arg_t->EN0_EN1)
											{
												case SM17500_No_current_gain:
												{
													SM512_arg_t->RGBW_IW=0;
													break;
												}
												case SM17500_4bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IW>15)SM512_arg_t->RGBW_IW=0;
														
													break;
												}
												case SM17500_5bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IW>31)SM512_arg_t->RGBW_IW=0;
													break;
												}
												case SM17500_6bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IW>63)SM512_arg_t->RGBW_IW=0;
													break;
												}											
											}
											break;
										}
										
									}
									break;
								}
								//自通道数
								case Third_level_menu_fourth_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											SM512_arg_t->SELF_CHAN_LD++;
											break;
										}
										
									}
									break;
								}
								//手动跑点
								case Third_level_menu_fifth_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											chip_param->Run_manually_point++;
											break;
										}
										
									}
									break;
								}
								//传输速度
								case Third_level_menu_sixth_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											switch(menu_arg->Fourth_level_menu_selection)
											{
												case Fourth_level_menu_first_submenu :
												{
													chip_param->Rate_num++;
													if(chip_param->Rate_num == baud_max)chip_param->Rate_num = baud_250k;
													uart_switch(chip_param->Rate_num);
													break;
												}
												
											}
											break;
										}
									
									}
									break;
								}
								
							}
							break;
						}
						//SM16703P
						case Second_level_menu_Chip_fourth_submenu		:
						{
							RZ_arg = &chip_param_sm16703P;
							switch(menu_arg->Third_level_menu_selection)
							{
								//芯片数
								case Third_level_menu_first_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											RZ_arg->Chip_Num++;
											break;
										}
										
									}
									break;
								}
								
							}
							break;
						}
						//SM16804P
						case Second_level_menu_Chip_fifth_submenu			:
						{
							RZ_arg = &chip_param_sm16804P;
							switch(menu_arg->Third_level_menu_selection)
							{
								//芯片数
								case Third_level_menu_first_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											RZ_arg->Chip_Num++;
											break;
										}
										
									}
									break;
								}
								//电流增益
								case Third_level_menu_second_submenu  :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											RZ_arg->RGBW_IR++;
											if(RZ_arg->RGBW_IR>15)RZ_arg->RGBW_IR=0;
											break;
										}
										case Fourth_level_menu_second_submenu :
										{
											RZ_arg->RGBW_IG++;
											if(RZ_arg->RGBW_IG>15)RZ_arg->RGBW_IG=0;
											break;
										}
										case Fourth_level_menu_third_submenu :
										{
											RZ_arg->RGBW_IB++;
											if(RZ_arg->RGBW_IB>15)RZ_arg->RGBW_IB=0;
											break;
										}
										case Fourth_level_menu_fourth_submenu :
										{
											RZ_arg->RGBW_IW++;
											if(RZ_arg->RGBW_IW>15)RZ_arg->RGBW_IW=0;
											break;
										}
										
									}
									break;
								}
								
							}
							break;
						}
					}
					break;
				}
				
				
			}			
			break;
		}
		case Fifth_level_menu	:
		{
			switch(menu_arg->First_level_menu_selection)
			{
				case First_level_menu_third_submenu	:
				{
					switch(menu_arg->Second_level_menu_selection)
					{
						case Second_level_menu_Chip_first_submenu  :
						{
							//SM1751X
							switch(menu_arg->Third_level_menu_selection)
							{
								case Third_level_menu_fifth_submenu	:
								{
									SM512_arg_t =&SM1751_arg;
										switch(menu_arg->Fourth_level_menu_selection)
										{
											//自动编址使能
											case Fourth_level_menu_first_submenu :
											{
												SM512_arg_t->AUTO =SM512_arg_t->AUTO? 0:1;
												break;
											}
											//无信号切换
											case Fourth_level_menu_second_submenu  :
											{
												SM512_arg_t->REDIS = SM512_arg_t->REDIS?0:1;
												break;
											}
											//通道模式
											case Fourth_level_menu_third_submenu	 :
											{
												SM512_arg_t->CH1_CH0 ++;
												if(SM512_arg_t->CH1_CH0 >= CH_MAX)SM512_arg_t->CH1_CH0 =CH1;
												break;
											}
											//自检颜色
											case Fourth_level_menu_fourth_submenu	 :
											{
												switch(menu_arg->Fifth_level_menu_selection)
												{
													case Fifth_level_menu_first_submenu :
													{
														SM512_arg_t->RGBW_R++;
														break;
													}
													case Fifth_level_menu_second_submenu :
													{
														SM512_arg_t->RGBW_G++;
														break;
													}
													case Fifth_level_menu_third_submenu :
													{
														SM512_arg_t->RGBW_B++;
														break;
													}
													case Fifth_level_menu_fourth_submenu :
													{
														SM512_arg_t->RGBW_W++;
														break;
													}
												}
												break;
											}
											//电流增益
											case Fourth_level_menu_fifth_submenu	 :
											{
												switch(menu_arg->Fifth_level_menu_selection)
												{
													case Fifth_level_menu_first_submenu :
													{
														SM512_arg_t->RGBW_IR++;
														if(SM512_arg_t->RGBW_IR>15)SM512_arg_t->RGBW_IR=0;
														break;
													}
													case Fifth_level_menu_second_submenu :
													{
														SM512_arg_t->RGBW_IG++;
														if(SM512_arg_t->RGBW_IG>15)SM512_arg_t->RGBW_IG=0;
														break;
													}
													case Fifth_level_menu_third_submenu :
													{
														SM512_arg_t->RGBW_IB++;
														if(SM512_arg_t->RGBW_IB>15)SM512_arg_t->RGBW_IB=0;
														break;
													}
													case Fifth_level_menu_fourth_submenu :
													{
														SM512_arg_t->RGBW_IW++;
														if(SM512_arg_t->RGBW_IW>15)SM512_arg_t->RGBW_IW=0;
														break;
													}
												}
												break;
											}
										
										}
									break;
								}
							}
							
							

							break;
						}
						//SM165XX
						case Second_level_menu_Chip_second_submenu   :
						{
							  switch(menu_arg->Third_level_menu_selection)
								{
									case Third_level_menu_fourth_submenu		:
									{
										switch(menu_arg->Fourth_level_menu_selection)
										{
											//自检颜色
											case Fourth_level_menu_first_submenu :
											{
												SM165XX_arg_t->Power_Up_Color++;
												if(SM165XX_arg_t->Power_Up_Color>=SM165XX_Selfcheck_color_max)SM165XX_arg_t->Power_Up_Color=No_Bright_Lights;
												break;
											}
											
										
										}
										break;
									}
								}
							break;
						}
						//SM17500
						case Second_level_menu_Chip_third_submenu		:  
						{
							switch(menu_arg->Third_level_menu_selection)
							{
								case Third_level_menu_seventh_submenu			:
								{
									
									SM512_arg_t =&SM17500_arg;
									switch(menu_arg->Fourth_level_menu_selection)
										{
											//自检颜色
											case Fourth_level_menu_first_submenu :
											{
												switch(menu_arg->Fifth_level_menu_selection)
												{
													case Fifth_level_menu_first_submenu :
													{
														SM512_arg_t->RGBW_R++;
														break;
													}
													case Fifth_level_menu_second_submenu :
													{
														SM512_arg_t->RGBW_G++;
														break;
													}
													case Fifth_level_menu_third_submenu :
													{
														SM512_arg_t->RGBW_B++;
														break;
													}
													case Fifth_level_menu_fourth_submenu :
													{
														SM512_arg_t->RGBW_W++;
														break;
													}
												}
												break;
											}
											//切换16813
											case Fourth_level_menu_second_submenu  :
											{
												SM512_arg_t->IC_select = SM512_arg_t->IC_select?0:1;
												break;
											}
											//无信号切换
											case Fourth_level_menu_third_submenu	 :
											{
												SM512_arg_t->REDIS = SM512_arg_t->REDIS?0:1;
												break;
											}
											//灯珠颜色
											case Fourth_level_menu_fourth_submenu	 :
											{
												SM512_arg_t->CH1_CH0 = SM512_arg_t->CH1_CH0?0:1;
												break;
											}
											//RTZ转码
											case Fourth_level_menu_fifth_submenu	 :
											{
												SM512_arg_t->RTZ_select = SM512_arg_t->RTZ_select?0:1;
												break;
											}
											//电流增益模式
											case Fourth_level_menu_sixth_submenu	 :
											{
												SM512_arg_t->EN0_EN1++;
												if(SM512_arg_t->EN0_EN1>=SM17500__current_gaint_max)SM512_arg_t->EN0_EN1=SM17500_No_current_gain;
												break;
											}
										
										}
									break;
								}
							}
							break;
						}
																		
					}
					break;
				}
				
				
			}			
			break;
		}
	}
	
	

	#if 0	
	switch(menu_selection.chip_model)
	{
		case SM1751X:
		{
			chip_param = &chip_param_sm17512;
			SM512_arg_t =&SM1751_arg;
			break;
		}
		case SM165XX:
		{
			chip_param = &chip_param_sm165XX;
			
			break;
		}
		case SM17500:
		{
			chip_param = &chip_param_sm17500;
			SM512_arg_t =&SM17500_arg;
			break;
		}
		case SM16703:
		{
			RZ_arg = &chip_param_sm16703P;
			break;
		}
		case SM16804:
		{
			RZ_arg = &chip_param_sm16804P;
			break;
		}		
		default:
		break;
	}	

	/*主页面设置*/
	switch(menu_selection.munu_select)
	{
		case effect_choice:
		{
			menu_arg->Color_num++;
			if(menu_arg->Color_num >=color_max)menu_arg->Color_num = loop_lamp ;
			
			break;
		}
		case chip_choice:
		{
			menu_arg->chip_model++;
			if(menu_arg->chip_model >= chip_model_max)menu_arg->chip_model = SM1751X;
			
			break;
		}
		default:		
			break;		
	}
	
	

	
	/*菜单设置*/
	switch(menu_arg->munu_posit)
	{
		case menu_coding_set:
		{
			
			switch(menu_selection.chip_model)
			{
				case SM1751X:
				{
					switch(menu_arg->munu_set_select)
					{
							case lamps_coding:
							{
								
								switch(menu_arg->munu_set_coding_select)
								{
									case coding_addr:
									{
										chip_param->lamps_num++;
										chip_param->coding_data_change_flag = lamps;
										break;
									}
									case coding_form:
									{
										chip_param->coding_form_num++;
										if(chip_param->coding_form_num == RGB_TAPE_MAX)chip_param->coding_form_num = RGB;
										chip_param->coding_data_change_flag = lamps;
										break;
									}
									case coding_chip:
									{
										chip_param->chip_chanle_num = chip_param->chip_chanle_num*2;
										if(chip_param->chip_chanle_num>2048)chip_param->chip_chanle_num=512;
										chip_param->coding_data_change_flag = lamps;
										break;
									}
									default:
										break;
								}				
								break;
							}
							case Channel_coding:
							{
								switch(menu_arg->munu_set_coding_select)
								{
									case coding_addr:
									{
										chip_param->Channel_num++;
										chip_param->coding_data_change_flag = Channel;
										break;
									}
									case coding_form:
									{
										chip_param->Channel_form++;
										chip_param->coding_data_change_flag = Channel;
										//if(key_arg->Channel_form == RGB_TAPE_MAX)key_arg->Channel_form = RGB;						
										break;
									}
									case coding_chip:
									{
										chip_param->chip_chanle_num = chip_param->chip_chanle_num*2;
										if(chip_param->chip_chanle_num>2048)chip_param->chip_chanle_num=512;
										chip_param->coding_data_change_flag = Channel;
										break;
									}
									default:
										break;
								}	
									break;
							}
							case coding_rate:
							{
								
									chip_param->Rate_num++;
									if(chip_param->Rate_num == baud_max)chip_param->Rate_num = baud_250k;
									uart_switch(chip_param->Rate_num);												
								break;
							}
								default:
								break;				
					}					
					break;
				}
				case SM165XX:
				{
					switch(menu_arg->munu_set_select)
					{
						case SM165xx_lamps_coding:
						{
								switch(menu_arg->munu_set_coding_select)
								{
									case coding_addr:
									{
										chip_param->lamps_num++;
										chip_param->coding_data_change_flag = lamps;
										break;
									}
									case coding_form:
									{
										chip_param->coding_form_num++;
										if(chip_param->coding_form_num == RGB_TAPE_MAX)chip_param->coding_form_num = RGB;
										chip_param->coding_data_change_flag = lamps;
										break;
									}
									case coding_chip:
									{
										chip_param->chip_chanle_num = chip_param->chip_chanle_num*2;
										if(chip_param->chip_chanle_num>2048)chip_param->chip_chanle_num=512;
										chip_param->coding_data_change_flag = lamps;
										break;
									}
									default:
										break;
								}							
							break;
						}
						case SM165xx_coding_rate:
						{
							chip_param->Rate_num++;
							if(chip_param->Rate_num == baud_max)chip_param->Rate_num = baud_250k;
							uart_switch(chip_param->Rate_num);							
							break;
						}
//						case SM165xx_Param_set:
//						{
//							break;
//						}						
						default:
							break;
					}
					
					break;
				}
				case SM17500:
				{
					switch(menu_arg->munu_set_select)
					{

							case SM17500_lamps_coding :
							{
								switch(menu_arg->munu_set_coding_select)
								{
									case coding_addr:
									{
										chip_param->Channel_num++;
										chip_param->coding_data_change_flag = Channel;
										break;
									}
									case coding_form:
									{
										chip_param->Channel_form++;
										chip_param->coding_data_change_flag = Channel;
										//if(key_arg->Channel_form == RGB_TAPE_MAX)key_arg->Channel_form = RGB;						
										break;
									}
									case coding_chip:
									{
										chip_param->chip_chanle_num = chip_param->chip_chanle_num*2;
										if(chip_param->chip_chanle_num>2048)chip_param->chip_chanle_num=512;
										chip_param->coding_data_change_flag = Channel;
										break;
									}
									default:
										break;
								}	
									break;
							}
							case SM17500_self_lamps_coding:
							{
									switch(menu_arg->munu_set_coding_select)
									{
										case coding_addr:
										{
											chip_param->Self_Channel_num++;
											chip_param->coding_data_change_flag = Channel;
											break;
										}
										
										case coding_form:
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num*2;
											if(chip_param->chip_chanle_num>2048)chip_param->chip_chanle_num=512;
											chip_param->coding_data_change_flag = Channel;
											break;
										}
										default:
											break;
									}													
								break;
							}
							case Current_Gain:
							{
								switch(menu_arg->munu_set_coding_select)
								{
									case RGB_R:
									{
										SM512_arg_t->RGBW_IR++;
										/*增益位选择*/
										switch(SM512_arg_t->EN0_EN1)
										{
											case SM17500_No_current_gain:
											{
												SM512_arg_t->RGBW_IR=0;
												break;
											}
											case SM17500_4bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IR>15)SM512_arg_t->RGBW_IR=0;
													
												break;
											}
											case SM17500_5bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IR>31)SM512_arg_t->RGBW_IR=0;
												break;
											}
											case SM17500_6bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IR>63)SM512_arg_t->RGBW_IR=0;
												break;
											}											
										}
										break;
									}
									case RGB_G:
									{
										SM512_arg_t->RGBW_IG++;
										switch(SM512_arg_t->EN0_EN1)
										{
											case SM17500_No_current_gain:
											{
												SM512_arg_t->RGBW_IG=0;
												break;
											}
											case SM17500_4bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IG>15)SM512_arg_t->RGBW_IG=0;
													
												break;
											}
											case SM17500_5bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IG>31)SM512_arg_t->RGBW_IG=0;
												break;
											}
											case SM17500_6bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IG>63)SM512_arg_t->RGBW_IG=0;
												break;
											}											
										}										
										break;
									}
									case RGB_B:
									{
										SM512_arg_t->RGBW_IB++;
										switch(SM512_arg_t->EN0_EN1)
										{
											case SM17500_No_current_gain:
											{
												SM512_arg_t->RGBW_IB=0;
												break;
											}
											case SM17500_4bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IB>15)SM512_arg_t->RGBW_IB=0;
													
												break;
											}
											case SM17500_5bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IB>31)SM512_arg_t->RGBW_IB=0;
												break;
											}
											case SM17500_6bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IB>63)SM512_arg_t->RGBW_IB=0;
												break;
											}											
										}										
										break;
									}
									case RGB_W:
									{
										SM512_arg_t->RGBW_IW++;
										switch(SM512_arg_t->EN0_EN1)
										{
											case SM17500_No_current_gain:
											{
												SM512_arg_t->RGBW_IW=0;
												break;
											}
											case SM17500_4bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IW>15)SM512_arg_t->RGBW_IW=0;
													
												break;
											}
											case SM17500_5bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IW>31)SM512_arg_t->RGBW_IW=0;
												break;
											}
											case SM17500_6bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IW>63)SM512_arg_t->RGBW_IW=0;
												break;
											}											
										}										
										break;
									}									
								}
								break;
								
							}
							case Self_Lamps_Num:
							{
								SM512_arg_t->SELF_CHAN_LD++;
								break;
								
							}
							case SM17500_Coding_rate:
							{
									chip_param->Rate_num++;
									if(chip_param->Rate_num == baud_max)chip_param->Rate_num = baud_250k;
									uart_switch(chip_param->Rate_num);								
								break;
								
							}						
								default:
								break;				
					}					
					break;
				}
				case SM16703:
				{
					RZ_arg->Chip_Num++;
					break;
				}
				case SM16804:
				{
					switch(menu_arg->munu_set_select)
					{
						case CHIP_NUM:
						{
							RZ_arg->Chip_Num++;
							break;
						}
						case IOUT_NUM:
						{
							switch(menu_arg->munu_set_coding_select)
							{
								case RGB_R:
								{
									RZ_arg->RGBW_IR++;
									if(RZ_arg->RGBW_IR>15)RZ_arg->RGBW_IR=0;
									break;
								}
								case RGB_G:
								{
									RZ_arg->RGBW_IG++;
									if(RZ_arg->RGBW_IG>15)RZ_arg->RGBW_IG=0;
									break;
								}
								case RGB_B:
								{
									RZ_arg->RGBW_IB++;
									if(RZ_arg->RGBW_IB>15)RZ_arg->RGBW_IB=0;									
									break;
								}
								case RGB_W:
								{
									RZ_arg->RGBW_IW++;
									if(RZ_arg->RGBW_IW>15)RZ_arg->RGBW_IW=0;									
									break;
								}
								default:
									break;
							}
							break;
						}
						default:
							break;
					}
					
					break;
				}				
				default:
				break;
			}

			break;
		}
		case menu_coding_arg_set:
		{
			switch(menu_selection.chip_model)
			{
				case SM1751X:
				{
					switch(menu_arg->munu_set_param_select)
					{
						case Auto_Addr:
						{
							SM512_arg_t->AUTO =SM512_arg_t->AUTO? 0:1;
							break;
						}
						case No_Signal_switch:
						{
							SM512_arg_t->REDIS = SM512_arg_t->REDIS?0:1;
							break;
						}
						case Channel_Mode:
						{
							SM512_arg_t->CH1_CH0 ++;
							if(SM512_arg_t->CH1_CH0 >= CH_MAX)SM512_arg_t->CH1_CH0 =CH1;
							break;
						}
						case Selfcheck_color:
						{
							switch(menu_arg->munu_set_param_select_rgbset)
							{
								case RGB_R:
								{
									SM512_arg_t->RGBW_R++;
									break;
								}
								case RGB_G:
								{
									SM512_arg_t->RGBW_G++;
									break;
								}
								case RGB_B:
								{
									SM512_arg_t->RGBW_B++;
									break;
								}
								case RGB_W:
								{
									SM512_arg_t->RGBW_W++;
									break;
								}
								default:
									break;
							}
							break;
						}
						case Current_Regulation:
						{
							switch(menu_arg->munu_set_param_select_rgbset)
							{
								case RGB_R:
								{
									SM512_arg_t->RGBW_IR++;
									if(SM512_arg_t->RGBW_IR>15)SM512_arg_t->RGBW_IR=0;
									break;
								}
								case RGB_G:
								{
									SM512_arg_t->RGBW_IG++;
									if(SM512_arg_t->RGBW_IG>15)SM512_arg_t->RGBW_IG=0;
									break;
								}
								case RGB_B:
								{
									SM512_arg_t->RGBW_IB++;
									if(SM512_arg_t->RGBW_IB>15)SM512_arg_t->RGBW_IB=0;
									break;
								}
								case RGB_W:
								{
									SM512_arg_t->RGBW_IW++;
									if(SM512_arg_t->RGBW_IW>15)SM512_arg_t->RGBW_IW=0;
									break;
								}
								default:
									break;
							}					
							break;
						}
						default:
							break;
					}					
					break;
				}
				case SM165XX:
				{
					switch(menu_arg->munu_set_param_select)
					{
						case SM165XX_Selfcheck_color:
						{
							SM165XX_arg_t->Power_Up_Color++;
							if(SM165XX_arg_t->Power_Up_Color>=SM165XX_Selfcheck_color_max)SM165XX_arg_t->Power_Up_Color=No_Bright_Lights;
							break;
						}
						
					}
					break;
				}
				case SM17500:
				{
					switch(menu_arg->munu_set_param_select)
					{
						case SM17500_self_check_color :
						{
							switch(menu_arg->munu_set_param_select_rgbset)
							{
								case RGB_R:
								{
									SM512_arg_t->RGBW_R++;
									
									break;
								}
								case RGB_G:
								{
									SM512_arg_t->RGBW_G++;
									
									break;
								}
								case RGB_B:
								{
									SM512_arg_t->RGBW_B++;
									
									break;
								}
								case RGB_W:
								{
									SM512_arg_t->RGBW_W++;
									
									break;
								}
								default:
									break;
							}							
							break;
						}
						case SM17500_IC_select:
						{													
							SM512_arg_t->IC_select = SM512_arg_t->IC_select?0:1;
							break;
						}
						case SM17500_No_Signal_switch :
						{
							SM512_arg_t->REDIS = SM512_arg_t->REDIS?0:1;
							break;
						}
						case SM17500_lamp_color :
						{
							SM512_arg_t->CH1_CH0 = SM512_arg_t->CH1_CH0?0:1;
							break;
						}
						case SM17500_RTZ_transcoding :
						{
							SM512_arg_t->RTZ_select = SM512_arg_t->RTZ_select?0:1;
							break;
						}
						case SM17500_Current_gain_mode :
						{
							SM512_arg_t->EN0_EN1++;
							if(SM512_arg_t->EN0_EN1>=SM17500__current_gaint_max)SM512_arg_t->EN0_EN1=SM17500_No_current_gain;
							break;
						}						
						
					}
					break;
				}				
				default:
				break;
			}			

			break;
		}
		default:
			break;
	}
	#endif
//	vTaskResume( DMX_DisplayHandle );
	ui_display();	
#if debug	
	printf("key2_fuc=======================2\r\n");
#endif
}


/***************************************************** 
 * @Function:  key4_fuc
 * @Description:   key3 short event handle
 * @param  event 
 * @retval void
******************************************************/
void key4_fuc(int event)
{
	blink_flag = 0;
	Protocol_output_parameter_t *Equipment_set_arg =&Equipment_set_param;
	menu_selection_t *menu_arg = &menu_selection;
	chip_param_t *chip_param =NULL;
	DMX512_arg_t *SM512_arg_t = NULL;
	RZ_arg_t *RZ_arg = NULL;
	SM165XX_arg_t *SM165XX_arg_t = &SM165XX_arg_set;
//	if(menu_arg->save_flag)
//	{
//		menu_arg->save_flag = 0;
//		return;
//	}
	switch(menu_arg->munu_select)
	{
		//位置第三层
		case Third_level_menu:
		{
			switch(menu_arg->First_level_menu_selection)
			{
				case First_level_menu_first_submenu:
				{
					switch(menu_arg->Second_level_menu_selection)
					{
						//带载通道设置
						case Second_level_menu_Equipment_first_submenu :
						{
							if(Equipment_set_arg->On_load_channe_param==1)
							{
								Equipment_set_arg->On_load_channe_param = 2048;
								write_all_data();
								ui_display();
								
								return;
							}							
							Equipment_set_arg->On_load_channe_param = Equipment_set_arg->On_load_channe_param/2;
							write_all_data();
							break;
						}
						//时钟频率设置
						case Second_level_menu_Equipment_second_submenu  :
						{
							
							if(Equipment_set_arg->Output_frequency_param==Freq_baud_200k)
							Equipment_set_arg->Output_frequency_param=Freq_baud_max;
							Equipment_set_arg->Output_frequency_param--;
							buad_uart_switch(frequency_arg[Equipment_set_arg->Output_frequency_param]);
							break;
						}
						//播放速率设置
						case Second_level_menu_Equipment_third_submenu	:  
						{
							if(Equipment_set_arg->Play_speed_param==1)
							{
								ui_display();
								write_all_data();
								return;
							}
							Equipment_set_arg->Play_speed_param--;
							write_all_data();
							break;
						}
						//颜色数
						case Second_level_menu_Equipment_fourth_submenu	:
						{
						
							if(Equipment_set_arg->colour_num_param==Colour_R)
								Equipment_set_arg->colour_num_param=Colour_max;
							Equipment_set_arg->colour_num_param--;
							write_all_data();	
							break;
						}
						//协议类型
						case Second_level_menu_Equipment_fifth_submenu	:
						{
							
							if(Equipment_set_arg->Protocol_type_param==DMX512)							
								Equipment_set_arg->Protocol_type_param=Protocol_max;
							Equipment_set_arg->Protocol_type_param--;
							write_all_data();
							break;
							
						}
						case Second_level_menu_Equipment_sixth_submenu	:
						{
							
							if(Equipment_set_arg->RZ_chip_param==RZ_SM16703)							
								Equipment_set_arg->RZ_chip_param=RZ_chip_max;
							Equipment_set_arg->RZ_chip_param--;
							write_all_data();
							break;
							
						}
					}
					break;
				}
				case First_level_menu_second_submenu  :
				{
					switch(menu_arg->Second_level_menu_selection)
					{
						//效果设置
						case Second_level_menu_Effect_first_submenu  :
						{
							if(Equipment_set_arg->Color_num == loop_lamp)Equipment_set_arg->Color_num = color_max;
							Equipment_set_arg->Color_num--;
							write_all_data();
							break;
						}
						//各通道灰度设置
						case Second_level_menu_Effect_second_submenu   :
						{
							switch(menu_arg->Third_level_menu_selection)
							{
								case Third_level_menu_first_submenu:
								{
									Equipment_set_arg->Adjust_Red_data--;
									break;
								}
								case Third_level_menu_second_submenu:
								{
									Equipment_set_arg->Adjust_Green_data--;
									break;
								}
								case Third_level_menu_third_submenu:
								{
									Equipment_set_arg->Adjust_Blue_data--;
									break;
								}
								case Third_level_menu_fourth_submenu:
								{
									Equipment_set_arg->Adjust_Write_data--;
									break;
								}
								case Third_level_menu_fifth_submenu:
								{
									Equipment_set_arg->Adjust_Yellow_data--;
									break;
								}
							}
							write_all_data();
							break;
						}						
					}
					break;
				}
				
			}
			break;
		}
		//位置在第四层
		case Fourth_level_menu  :
		{
			switch(menu_arg->First_level_menu_selection)
			{
				case First_level_menu_third_submenu	:
				{
					switch(menu_arg->Second_level_menu_selection)
					{
						case Second_level_menu_Chip_first_submenu  :
						{
							chip_param = &chip_param_sm17512;
							SM512_arg_t =&SM1751_arg;
							//SM1751X
							switch(menu_arg->Third_level_menu_selection)
							{
								//灯具写码
								case Third_level_menu_first_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										//起始地址
										case Fourth_level_menu_first_submenu :
										{
											
											if(chip_param->lamps_num == 1 )return;
											chip_param->lamps_num--;
										  
											break;
										}
										//灯具类型
										case Fourth_level_menu_second_submenu  :
										{
											if(chip_param->coding_form_num == RGB)chip_param->coding_form_num = RGB_TAPE_MAX;											
											chip_param->coding_form_num--;
											
											break;
										}
										//通道数
										case Fourth_level_menu_third_submenu	 :
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num/2;
											if(chip_param->chip_chanle_num<512)chip_param->chip_chanle_num=2048;
											chip_param->coding_data_change_flag = lamps;
											break;
										}
									}
									break;
								}
								//通道写码
								case Third_level_menu_second_submenu  :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										//起始地址
										case Fourth_level_menu_first_submenu :
										{
											if(chip_param->Channel_num == 1 )return;
											chip_param->Channel_num--;
											break;
										}
										//步进数
										case Fourth_level_menu_second_submenu  :
										{
											if(chip_param->Channel_form == 1)return;
											chip_param->Channel_form--;
											break;
										}
										//通道数
										case Fourth_level_menu_third_submenu	 :
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num/2;
											if(chip_param->chip_chanle_num<512)chip_param->chip_chanle_num=2048;
											chip_param->coding_data_change_flag = Channel;
											break;
										}
									}
									break;
								}
								//手动跑点
								case Third_level_menu_third_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											if(chip_param->Run_manually_point==0)return;
											chip_param->Run_manually_point--;
											break;
										}
										
									}
									break;
								}
								//传输速度
								case Third_level_menu_fourth_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											if(chip_param->Rate_num == baud_250k)chip_param->Rate_num = baud_max;
											chip_param->Rate_num--;
											uart_switch(chip_param->Rate_num);
											break;
										}
										
									}
									break;
								}
								
							}
							break;
						}
						//SM165XX
						case Second_level_menu_Chip_second_submenu   :
						{
							chip_param = &chip_param_sm165XX;
							switch(menu_arg->Third_level_menu_selection)
							{
								//灯具写码
								case Third_level_menu_first_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										//起始地址
										case Fourth_level_menu_first_submenu :
										{
											if(chip_param->lamps_num == 1 )return;
											chip_param->lamps_num--;
											break;
										}
										//灯具类型
										case Fourth_level_menu_second_submenu  :
										{
											if(chip_param->coding_form_num == RGB)chip_param->coding_form_num = RGB_TAPE_MAX;
											//if(key_arg->coding_form_num == 1)return;
											chip_param->coding_form_num--;
											break;
										}
										//通道数
										case Fourth_level_menu_third_submenu	 :
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num/2;
											if(chip_param->chip_chanle_num<512)chip_param->chip_chanle_num=2048;
											chip_param->coding_data_change_flag = lamps;
											break;
										}
									}
									break;
								}
								//手动跑点
								case Third_level_menu_second_submenu  :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											if(chip_param->Run_manually_point==0)return;
											chip_param->Run_manually_point--;
											break;
										}
										
									}
									break;
								}
								//传输速度
								case Third_level_menu_third_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											if(chip_param->Rate_num == baud_250k)chip_param->Rate_num = baud_max;
											chip_param->Rate_num--;
											uart_switch(chip_param->Rate_num);
											break;
										}
										
									}
									break;
								}
								
								
							}
							break;
						}
						//SM17500
						case Second_level_menu_Chip_third_submenu		:  
						{
							chip_param = &chip_param_sm17500;
							SM512_arg_t =&SM17500_arg;
							switch(menu_arg->Third_level_menu_selection)
							{
								//通道写码
								case Third_level_menu_first_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										//起始地址
										case Fourth_level_menu_first_submenu :
										{
											if(chip_param->Channel_num==1)return;
											chip_param->Channel_num--;
											break;
										}
										//步进数
										case Fourth_level_menu_second_submenu  :
										{
											if(chip_param->Channel_form==1)return;
											chip_param->Channel_form--;
											break;
										}
										//通道数
										case Fourth_level_menu_third_submenu	 :
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num/2;
											if(chip_param->chip_chanle_num<512)chip_param->chip_chanle_num=2048;
											break;
										}
									}
									break;
								}
								//自通道写码
								case Third_level_menu_second_submenu  :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										//起始地址
										case Fourth_level_menu_first_submenu :
										{
											if(chip_param->Self_Channel_num==1)return;
											chip_param->Self_Channel_num--;
											break;
										}
										//通道数
										case Fourth_level_menu_second_submenu  :
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num/2;
											if(chip_param->chip_chanle_num<512)chip_param->chip_chanle_num=2048;
											break;
										}
										
									}
									break;
								}
								//电流增益
								case Third_level_menu_third_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
										
								
											switch(SM512_arg_t->EN0_EN1)
											{
												case SM17500_No_current_gain:
												{
													SM512_arg_t->RGBW_IR=0;
													break;
												}
												case SM17500_4bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IR==0)SM512_arg_t->RGBW_IR=16;
													SM512_arg_t->RGBW_IR--;
														
													break;
												}
												case SM17500_5bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IR==0)SM512_arg_t->RGBW_IR=32;
													SM512_arg_t->RGBW_IR--;
													break;
												}
												case SM17500_6bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IR==0)SM512_arg_t->RGBW_IR=64;
													SM512_arg_t->RGBW_IR--;
													break;
												}											
											}
											break;
										}
										case Fourth_level_menu_second_submenu :
										{
											
											switch(SM512_arg_t->EN0_EN1)
											{
												case SM17500_No_current_gain:
												{
													SM512_arg_t->RGBW_IG=0;
													break;
												}
												case SM17500_4bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IG==0)SM512_arg_t->RGBW_IG=16;
													SM512_arg_t->RGBW_IG--;
														
													break;
												}
												case SM17500_5bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IG==0)SM512_arg_t->RGBW_IG=32;
													SM512_arg_t->RGBW_IG--;
													break;
												}
												case SM17500_6bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IG==0)SM512_arg_t->RGBW_IG=64;
													SM512_arg_t->RGBW_IG--;
													break;
												}											
											}
											break;
										}
										case Fourth_level_menu_third_submenu :
										{
										 
											switch(SM512_arg_t->EN0_EN1)
											{
												case SM17500_No_current_gain:
												{
													SM512_arg_t->RGBW_IB=0;
													break;
												}
												case SM17500_4bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IB==0)SM512_arg_t->RGBW_IB=16;
												SM512_arg_t->RGBW_IB--;
														
													break;
												}
												case SM17500_5bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IB==0)SM512_arg_t->RGBW_IB=32;
													SM512_arg_t->RGBW_IB--;
													break;
												}
												case SM17500_6bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IB==0)SM512_arg_t->RGBW_IB=64;
													SM512_arg_t->RGBW_IB--;
													break;
												}											
											}
											break;
										}
										case Fourth_level_menu_fourth_submenu :
										{
											 
											switch(SM512_arg_t->EN0_EN1)
											{
												case SM17500_No_current_gain:
												{
													SM512_arg_t->RGBW_IW=0;
													break;
												}
												case SM17500_4bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IW==0)SM512_arg_t->RGBW_IW=16;
												 SM512_arg_t->RGBW_IW--;
														
													break;
												}
												case SM17500_5bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IW==0)SM512_arg_t->RGBW_IW=32;
												SM512_arg_t->RGBW_IW--;
													break;
												}
												case SM17500_6bit_current_gain:
												{
													if(SM512_arg_t->RGBW_IW==0)SM512_arg_t->RGBW_IW=64;
												SM512_arg_t->RGBW_IW--;
													break;
												}											
											}
											break;
										}
										
									}
									break;
								}
								//自通道数
								case Third_level_menu_fourth_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											if(SM512_arg_t->SELF_CHAN_LD==1)return;
											SM512_arg_t->SELF_CHAN_LD--;
											break;
										}
										
									}
									break;
								}
								//手动跑点
								case Third_level_menu_fifth_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											if(chip_param->Run_manually_point==0)return;
											chip_param->Run_manually_point--;
											break;
										}
										
									}
									break;
								}
								//传输速度
								case Third_level_menu_sixth_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											switch(menu_arg->Fourth_level_menu_selection)
											{
												case Fourth_level_menu_first_submenu :
												{
													if(chip_param->Rate_num == baud_250k)chip_param->Rate_num = baud_max;
													chip_param->Rate_num--;
													uart_switch(chip_param->Rate_num);
													break;
												}
												
											}
											break;
										}
									
									}
									break;
								}
								
							}
							break;
						}
						//SM16703P
						case Second_level_menu_Chip_fourth_submenu		:
						{
							RZ_arg = &chip_param_sm16703P;
							switch(menu_arg->Third_level_menu_selection)
							{
								//芯片数
								case Third_level_menu_first_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											if(RZ_arg->Chip_Num==1)return;
											RZ_arg->Chip_Num--;
											break;
										}
										
									}
									break;
								}
								
							}
							break;
						}
						//SM16804P
						case Second_level_menu_Chip_fifth_submenu			:
						{
							RZ_arg = &chip_param_sm16804P;
							switch(menu_arg->Third_level_menu_selection)
							{
								//芯片数
								case Third_level_menu_first_submenu :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											if(RZ_arg->Chip_Num==1)return;
											RZ_arg->Chip_Num--;
											break;
										}
										
									}
									break;
								}
								//电流增益
								case Third_level_menu_second_submenu  :
								{
									switch(menu_arg->Fourth_level_menu_selection)
									{
										case Fourth_level_menu_first_submenu :
										{
											if(RZ_arg->RGBW_IR==0)RZ_arg->RGBW_IR=16;
											RZ_arg->RGBW_IR--;
											break;
										}
										case Fourth_level_menu_second_submenu :
										{
											if(RZ_arg->RGBW_IG==0)RZ_arg->RGBW_IG=16;
											RZ_arg->RGBW_IG--;
											break;
										}
										case Fourth_level_menu_third_submenu :
										{
											if(RZ_arg->RGBW_IB==0)RZ_arg->RGBW_IB=16;
											RZ_arg->RGBW_IB--;
											break;
										}
										case Fourth_level_menu_fourth_submenu :
										{
											if(RZ_arg->RGBW_IW==0)RZ_arg->RGBW_IW=16;
											RZ_arg->RGBW_IW--;
											break;
										}
										
									}
									break;
								}
								
							}
							break;
						}
					}
					break;
				}
				
				
			}			
			break;
		}
		case Fifth_level_menu	:
		{
			switch(menu_arg->First_level_menu_selection)
			{
				case First_level_menu_third_submenu	:
				{
					switch(menu_arg->Second_level_menu_selection)
					{
						case Second_level_menu_Chip_first_submenu  :
						{
							//SM1751X
							switch(menu_arg->Third_level_menu_selection)
							{
								case Third_level_menu_fifth_submenu	:
								{
									SM512_arg_t =&SM1751_arg;
										switch(menu_arg->Fourth_level_menu_selection)
										{
											//自动编址使能
											case Fourth_level_menu_first_submenu :
											{
												SM512_arg_t->AUTO =SM512_arg_t->AUTO? 0:1;
												break;
											}
											//无信号切换
											case Fourth_level_menu_second_submenu  :
											{
												SM512_arg_t->REDIS = SM512_arg_t->REDIS?0:1;
												break;
											}
											//通道模式
											case Fourth_level_menu_third_submenu	 :
											{
												if(SM512_arg_t->CH1_CH0 == CH1)SM512_arg_t->CH1_CH0 =CH_MAX;
												SM512_arg_t->CH1_CH0--;
												break;
											}
											//自检颜色
											case Fourth_level_menu_fourth_submenu	 :
											{
												switch(menu_arg->Fifth_level_menu_selection)
												{
													case Fifth_level_menu_first_submenu :
													{
														SM512_arg_t->RGBW_R--;
														break;
													}
													case Fifth_level_menu_second_submenu :
													{
														SM512_arg_t->RGBW_G--;
														break;
													}
													case Fifth_level_menu_third_submenu :
													{
														SM512_arg_t->RGBW_B--;
														break;
													}
													case Fifth_level_menu_fourth_submenu :
													{
														SM512_arg_t->RGBW_W--;
														break;
													}
												}
												break;
											}
											//电流增益
											case Fourth_level_menu_fifth_submenu	 :
											{
												switch(menu_arg->Fifth_level_menu_selection)
												{
													case Fifth_level_menu_first_submenu :
													{
														if(SM512_arg_t->RGBW_IR==0)SM512_arg_t->RGBW_IR=16;
														SM512_arg_t->RGBW_IR--;
														break;
													}
													case Fifth_level_menu_second_submenu :
													{
														if(SM512_arg_t->RGBW_IG==0)SM512_arg_t->RGBW_IG=16;
														SM512_arg_t->RGBW_IG--;
														break;
													}
													case Fifth_level_menu_third_submenu :
													{
														if(SM512_arg_t->RGBW_IB==0)SM512_arg_t->RGBW_IB=16;
														SM512_arg_t->RGBW_IB--;
														break;
													}
													case Fifth_level_menu_fourth_submenu :
													{
														if(SM512_arg_t->RGBW_IW==0)SM512_arg_t->RGBW_IW=16;
														SM512_arg_t->RGBW_IW--;
														break;
													}
												}
												break;
											}
										
										}
									break;
								}
							}
							
							

							break;
						}
						//SM165XX
						case Second_level_menu_Chip_second_submenu   :
						{
							  switch(menu_arg->Third_level_menu_selection)
								{
									case Third_level_menu_fourth_submenu		:
									{
										switch(menu_arg->Fourth_level_menu_selection)
										{
											//自检颜色
											case Fourth_level_menu_first_submenu :
											{
												if(SM165XX_arg_t->Power_Up_Color==No_Bright_Lights)SM165XX_arg_t->Power_Up_Color=SM165XX_Selfcheck_color_max;
												SM165XX_arg_t->Power_Up_Color--;
												break;
											}
											
										
										}
										break;
									}
								}
							break;
						}
						//SM17500
						case Second_level_menu_Chip_third_submenu		:  
						{
							switch(menu_arg->Third_level_menu_selection)
							{
								case Third_level_menu_seventh_submenu			:
								{
									
									SM512_arg_t =&SM17500_arg;
									switch(menu_arg->Fourth_level_menu_selection)
										{
											//自检颜色
											case Fourth_level_menu_first_submenu :
											{
												switch(menu_arg->Fifth_level_menu_selection)
												{
													case Fifth_level_menu_first_submenu :
													{
														SM512_arg_t->RGBW_R--;
														break;
													}
													case Fifth_level_menu_second_submenu :
													{
														SM512_arg_t->RGBW_G--;
														break;
													}
													case Fifth_level_menu_third_submenu :
													{
														SM512_arg_t->RGBW_B--;
														break;
													}
													case Fifth_level_menu_fourth_submenu :
													{
														SM512_arg_t->RGBW_W--;
														break;
													}
												}
												break;
											}
											//切换16813
											case Fourth_level_menu_second_submenu  :
											{
												SM512_arg_t->IC_select = SM512_arg_t->IC_select?0:1;
												break;
											}
											//无信号切换
											case Fourth_level_menu_third_submenu	 :
											{
												SM512_arg_t->REDIS = SM512_arg_t->REDIS?0:1;
												break;
											}
											//灯珠颜色
											case Fourth_level_menu_fourth_submenu	 :
											{
												SM512_arg_t->CH1_CH0 = SM512_arg_t->CH1_CH0?0:1;
												break;
											}
											//RTZ转码
											case Fourth_level_menu_fifth_submenu	 :
											{
												SM512_arg_t->RTZ_select = SM512_arg_t->RTZ_select?0:1;
												break;
											}
											//电流增益模式
											case Fourth_level_menu_sixth_submenu	 :
											{
												if(SM512_arg_t->EN0_EN1==SM17500_No_current_gain)SM512_arg_t->EN0_EN1=SM17500__current_gaint_max;
												SM512_arg_t->EN0_EN1--;
												break;
											}
										
										}
									break;
								}
							}
							break;
						}
																		
					}
					break;
				}
				
				
			}			
			break;
		}
	}
	
	
	
#if 0	
	switch(menu_selection.chip_model)
	{
		case SM1751X:
		{
			chip_param = &chip_param_sm17512;
			SM512_arg_t =&SM1751_arg;
			break;
		}
		case SM165XX:
		{
			chip_param = &chip_param_sm165XX;
			SM512_arg_t =&SM1751_arg;
			break;
		}
		case SM17500:
		{
			chip_param = &chip_param_sm17500;
			SM512_arg_t =&SM17500_arg;
			break;
		}
		case SM16703:
		{
			RZ_arg = &chip_param_sm16703P;
			break;
		}
		case SM16804:
		{
			RZ_arg = &chip_param_sm16804P;
			break;
		}			
		default:
		break;
	}	
	switch(menu_arg->munu_select)
	{
		case effect_choice:
		{
			if(menu_arg->Color_num == loop_lamp)menu_arg->Color_num = color_max;
			menu_arg->Color_num--;

			break;
		}
		case chip_choice:
		{
					
			if(menu_arg->chip_model == SM1751X)menu_arg->chip_model = chip_model_max;
			menu_arg->chip_model--;
			break;
		}

		default:		
			break;		
	}	
	
	/*菜单设置*/
	switch(menu_arg->munu_posit)
	{
		case menu_coding_set:
		{

				switch(menu_selection.chip_model)
				{
					case SM1751X:
					{
						switch(menu_arg->munu_set_select)
						{
								case lamps_coding:
								{
									switch(menu_arg->munu_set_coding_select)
									{
										case coding_addr:
										{
											chip_param->coding_data_change_flag = lamps;
											if(chip_param->lamps_num == 1 )return;
											chip_param->lamps_num--;
											
											break;
										}
										case coding_form:
										{
											if(chip_param->coding_form_num == RGB)chip_param->coding_form_num = RGB_TAPE_MAX;
											//if(key_arg->coding_form_num == 1)return;
											chip_param->coding_form_num--;
											chip_param->coding_data_change_flag = lamps;
											break;
										}
										case coding_chip:
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num/2;
											if(chip_param->chip_chanle_num<512)chip_param->chip_chanle_num=2048;
											chip_param->coding_data_change_flag = lamps;
											break;
										}							
										default:
											break;
									}				
									break;
								}
								case Channel_coding:
								{
									switch(menu_arg->munu_set_coding_select)
									{
										case coding_addr:
										{
											chip_param->coding_data_change_flag = Channel;
											if(chip_param->Channel_num == 1 )return;
											chip_param->Channel_num--;
											break;
										}
										case coding_form:
										{
											//if(key_arg->Channel_form == RGB)key_arg->Channel_form = RGB_TAPE_MAX;
											chip_param->coding_data_change_flag = Channel;
											if(chip_param->Channel_form == 1)return;
											chip_param->Channel_form--;
								
											break;
										}
										case coding_chip:
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num/2;
											if(chip_param->chip_chanle_num<512)chip_param->chip_chanle_num=2048;
											chip_param->coding_data_change_flag = Channel;
											break;
										}							
										default:
											break;
									}				
									break;
								}					
								case coding_rate:
								{
									
									if(chip_param->Rate_num == baud_250k)chip_param->Rate_num = baud_max;
											chip_param->Rate_num--;
											uart_switch(chip_param->Rate_num);	
											
									break;
								}
								
									default:
									break;				
						}						
						break;
					}
					case SM165XX:
					{
						switch(menu_arg->munu_set_select)
						{
							case SM165xx_lamps_coding:
							{
									switch(menu_arg->munu_set_coding_select)
									{
										case coding_addr:
										{
											chip_param->coding_data_change_flag = lamps;
											if(chip_param->lamps_num == 1 )return;
											chip_param->lamps_num--;
											
											break;
										}
										case coding_form:
										{
											if(chip_param->coding_form_num == RGB)chip_param->coding_form_num = RGB_TAPE_MAX;
											//if(key_arg->coding_form_num == 1)return;
											chip_param->coding_form_num--;
											chip_param->coding_data_change_flag = lamps;
											break;
										}
										case coding_chip:
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num/2;
											if(chip_param->chip_chanle_num<512)chip_param->chip_chanle_num=2048;
											chip_param->coding_data_change_flag = lamps;
											break;
										}							
										default:
											break;
									}								
								break;
							}
							case SM165xx_coding_rate:
							{
								if(chip_param->Rate_num == baud_250k)chip_param->Rate_num = baud_max;
											chip_param->Rate_num--;
											uart_switch(chip_param->Rate_num);
								break;
							}						
							default:
								break;
						}						
						break;
					}
				case SM17500:
				{
					switch(menu_arg->munu_set_select)
					{

							case SM17500_lamps_coding :
							{
								switch(menu_arg->munu_set_coding_select)
								{
									case coding_addr:
									{
										chip_param->coding_data_change_flag = Channel;
										if(chip_param->Channel_num==1)return;
										chip_param->Channel_num--;
										
										break;
									}
									case coding_form:
									{
										
										chip_param->coding_data_change_flag = Channel;
										if(chip_param->Channel_form==1)return;
										chip_param->Channel_form--;
										//if(key_arg->Channel_form == RGB_TAPE_MAX)key_arg->Channel_form = RGB;						
										break;
									}
									case coding_chip:
									{
										chip_param->chip_chanle_num = chip_param->chip_chanle_num/2;
											if(chip_param->chip_chanle_num<512)chip_param->chip_chanle_num=2048;
										chip_param->coding_data_change_flag = Channel;
										break;
									}
									default:
										break;
								}	
									break;
							}
							case SM17500_self_lamps_coding:
							{
									switch(menu_arg->munu_set_coding_select)
									{
										case coding_addr:
										{
																						
											chip_param->coding_data_change_flag = Channel;
											if(chip_param->Self_Channel_num==1)return;
											chip_param->Self_Channel_num--;
											break;
										}
										
										case coding_form:
										{
											chip_param->chip_chanle_num = chip_param->chip_chanle_num/2;
											if(chip_param->chip_chanle_num<512)chip_param->chip_chanle_num=2048;
											//chip_param->coding_data_change_flag = Channel;
											break;
										}
										default:
											break;
									}													
								break;
							}
							case Current_Gain:
							{
								switch(menu_arg->munu_set_coding_select)
								{
									case RGB_R:
									{
										//SM512_arg_t->RGBW_IR++;
										/*增益位选择*/
										switch(SM512_arg_t->EN0_EN1)
										{
											case SM17500_No_current_gain:
											{
												SM512_arg_t->RGBW_IR=0;
												break;
											}
											case SM17500_4bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IR==0)SM512_arg_t->RGBW_IR=16;
													SM512_arg_t->RGBW_IR--;
												break;
											}
											case SM17500_5bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IR==0)SM512_arg_t->RGBW_IR=32;
												SM512_arg_t->RGBW_IR--;
												break;
											}
											case SM17500_6bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IR==0)SM512_arg_t->RGBW_IR=64;
												SM512_arg_t->RGBW_IR--;
												break;
											}											
										}
										break;
									}
									case RGB_G:
									{
//										SM512_arg_t->RGBW_IG++;
										switch(SM512_arg_t->EN0_EN1)
										{
											case SM17500_No_current_gain:
											{
												SM512_arg_t->RGBW_IG=0;
												break;
											}
											case SM17500_4bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IG==0)SM512_arg_t->RGBW_IG=16;
													SM512_arg_t->RGBW_IG--;
												break;
											}
											case SM17500_5bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IG==0)SM512_arg_t->RGBW_IG=32;
												SM512_arg_t->RGBW_IG--;
												break;
											}
											case SM17500_6bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IG==0)SM512_arg_t->RGBW_IG=64;
												SM512_arg_t->RGBW_IG--;
												break;
											}											
										}										
										break;
									}
									case RGB_B:
									{
//										SM512_arg_t->RGBW_IB++;
										switch(SM512_arg_t->EN0_EN1)
										{
											case SM17500_No_current_gain:
											{
												SM512_arg_t->RGBW_IB=0;
												break;
											}
											case SM17500_4bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IB==0)SM512_arg_t->RGBW_IB=16;
												SM512_arg_t->RGBW_IB--;
													
												break;
											}
											case SM17500_5bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IB==0)SM512_arg_t->RGBW_IB=32;
												SM512_arg_t->RGBW_IB--;
												break;
											}
											case SM17500_6bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IB==0)SM512_arg_t->RGBW_IB=64;
												SM512_arg_t->RGBW_IB--;
												break;
											}											
										}										
										break;
									}
									case RGB_W:
									{
//										SM512_arg_t->RGBW_IW++;
										switch(SM512_arg_t->EN0_EN1)
										{
											case SM17500_No_current_gain:
											{
												SM512_arg_t->RGBW_IW=0;
												break;
											}
											case SM17500_4bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IW==0)SM512_arg_t->RGBW_IW=16;
												SM512_arg_t->RGBW_IW--;
													
												break;
											}
											case SM17500_5bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IW==0)SM512_arg_t->RGBW_IW=32;
												SM512_arg_t->RGBW_IW--;
												break;
											}
											case SM17500_6bit_current_gain:
											{
												if(SM512_arg_t->RGBW_IW==0)SM512_arg_t->RGBW_IW=64;
												SM512_arg_t->RGBW_IW--;
												break;
											}											
										}										
										break;
									}									
								}
								break;
								
							}
							case Self_Lamps_Num:
							{
								if(SM512_arg_t->SELF_CHAN_LD==1)return;
								SM512_arg_t->SELF_CHAN_LD--;
								break;
								
							}
							case SM17500_Coding_rate:
							{
									if(chip_param->Rate_num == baud_250k)chip_param->Rate_num = baud_max;
											chip_param->Rate_num--;
											uart_switch(chip_param->Rate_num);								
								break;
								
							}						
								default:
								break;				
					}					
					break;
				}					
				case SM16703:
				{
					if(RZ_arg->Chip_Num==1)return;
					RZ_arg->Chip_Num--;
					break;
				}
				case SM16804:
				{
					switch(menu_arg->munu_set_select)
					{
						case CHIP_NUM:
						{
							if(RZ_arg->Chip_Num==1)return;
							RZ_arg->Chip_Num--;
							break;
						}
						case IOUT_NUM:
						{
							switch(menu_arg->munu_set_coding_select)
							{
								case RGB_R:
								{
									
									if(RZ_arg->RGBW_IR==0)RZ_arg->RGBW_IR=16;
									RZ_arg->RGBW_IR--;
									break;
								}
								case RGB_G:
								{
									
									if(RZ_arg->RGBW_IG==0)RZ_arg->RGBW_IG=16;
									RZ_arg->RGBW_IG--;
									break;
								}
								case RGB_B:
								{
									
									if(RZ_arg->RGBW_IB==0)RZ_arg->RGBW_IB=16;
										RZ_arg->RGBW_IB--;
									break;
								}
								case RGB_W:
								{
									
									if(RZ_arg->RGBW_IW==0)RZ_arg->RGBW_IW=16;
									RZ_arg->RGBW_IW--;									
									break;
								}
								default:
									break;
							}
							break;
						}
						default:
							break;
					}
					
					break;
				}					
					default:
					break;
				}			

			break;
		}
		case menu_coding_arg_set:
		{
			switch(menu_selection.chip_model)
			{
				case SM1751X:
				{
						switch(menu_arg->munu_set_param_select)
						{
							case Auto_Addr:
							{
								SM512_arg_t->AUTO =SM512_arg_t->AUTO? 0:1;
								break;
							}
							case No_Signal_switch:
							{
								SM512_arg_t->REDIS = SM512_arg_t->REDIS?0:1;
								break;
							}
							case Channel_Mode:
							{
								
								if(SM512_arg_t->CH1_CH0 == CH1)SM512_arg_t->CH1_CH0 =CH_MAX;
								SM512_arg_t->CH1_CH0--;
								break;
							}
							case Selfcheck_color:
							{
								switch(menu_arg->munu_set_param_select_rgbset)
								{
									case RGB_R:
									{
										SM512_arg_t->RGBW_R--;
										break;
									}
									case RGB_G:
									{
										SM512_arg_t->RGBW_G--;
										break;
									}
									case RGB_B:
									{
										SM512_arg_t->RGBW_B--;
										break;
									}
									case RGB_W:
									{
										SM512_arg_t->RGBW_W--;
										break;
									}
									default:
										break;
								}
								break;
							}
							case Current_Regulation:
							{
								switch(menu_arg->munu_set_param_select_rgbset)
								{
									case RGB_R:
									{
										
										if(SM512_arg_t->RGBW_IR==0)SM512_arg_t->RGBW_IR=16;
										SM512_arg_t->RGBW_IR--;
										break;
									}
									case RGB_G:
									{
										
										if(SM512_arg_t->RGBW_IG==0)SM512_arg_t->RGBW_IG=16;
										SM512_arg_t->RGBW_IG--;
										break;
									}
									case RGB_B:
									{
										
										if(SM512_arg_t->RGBW_IB==0)SM512_arg_t->RGBW_IB=16;
										SM512_arg_t->RGBW_IB--;
										break;
									}
									case RGB_W:
									{
										
										if(SM512_arg_t->RGBW_IW==0)SM512_arg_t->RGBW_IW=16;
										SM512_arg_t->RGBW_IW--;
										break;
									}
									default:
										break;
								}					
								break;
							}
							default:
								break;
						}					
					break;
				}
				case SM165XX:
				{
					switch(menu_arg->munu_set_param_select)
					{
						case SM165XX_Selfcheck_color:
						{							
							if(SM165XX_arg_t->Power_Up_Color==No_Bright_Lights)SM165XX_arg_t->Power_Up_Color=SM165XX_Selfcheck_color_max;
							SM165XX_arg_t->Power_Up_Color--;
						}
						
					}					
					break;
				}
				case SM17500:
				{
					switch(menu_arg->munu_set_param_select)
					{
						case SM17500_self_check_color :
						{
							switch(menu_arg->munu_set_param_select_rgbset)
							{
								case RGB_R:
								{
									//if(SM512_arg_t->RGBW_R==1)return;
									SM512_arg_t->RGBW_R--;
									
									break;
								}
								case RGB_G:
								{
									//if(SM512_arg_t->RGBW_G==1)return;
									SM512_arg_t->RGBW_G--;
									
									break;
								}
								case RGB_B:
								{
									//if(SM512_arg_t->RGBW_B==1)return;
									SM512_arg_t->RGBW_B--;
									
									break;
								}
								case RGB_W:
								{
									//if(SM512_arg_t->RGBW_W==1)return;
									SM512_arg_t->RGBW_W--;
									
									break;
								}
								default:
									break;
							}							
							break;
						}
						case SM17500_IC_select:
						{													
							SM512_arg_t->IC_select = SM512_arg_t->IC_select?0:1;
							break;
						}
						case SM17500_No_Signal_switch :
						{
							SM512_arg_t->REDIS = SM512_arg_t->REDIS?0:1;
							break;
						}
						case SM17500_lamp_color :
						{
							SM512_arg_t->CH1_CH0 = SM512_arg_t->CH1_CH0?0:1;
							break;
						}
						case SM17500_RTZ_transcoding :
						{
							SM512_arg_t->RTZ_select = SM512_arg_t->RTZ_select?0:1;
							break;
						}
						case SM17500_Current_gain_mode :
						{
							
							if(SM512_arg_t->EN0_EN1==SM17500_No_current_gain)SM512_arg_t->EN0_EN1=SM17500__current_gaint_max;
							SM512_arg_t->EN0_EN1--;
							break;
						}						
						
					}
					break;
				}				
				default:
				break;
			}			

			break;
		}
		default:
			break;
	}
#endif
	ui_display();
#if debug	
	printf("key3_fuc=======================3\r\n");
#endif
}




/***************************************************** 
 * @Function:  key1_long_fuc
 * @Description:   key1 long event handle
 * @param  event 
 * @retval void
******************************************************/


void key1_long_fuc(int event)
{
	//vTaskSuspend( ui_display_DisplayHandle );
	menu_selection_t *menu_arg = &menu_selection;
	switch(menu_arg->munu_select)
	{
		case First_level_menu:
		{
			menu_arg->First_level_menu_selection =First_level_menu_main_submenu	;
			data_handle_fuc(NULL);			
			break;
		}
		case Second_level_menu:
		{
			menu_arg->munu_select=First_level_menu;
			menu_arg->Second_level_menu_selection=0;
														
			break;
		}
		case Third_level_menu:
		{
			menu_arg->munu_select=Second_level_menu;
			menu_arg->Third_level_menu_selection=0;
			break;
		}
		case Fourth_level_menu:
		{
			//手动跑点任务与DMX任务相互切换
			if(menu_arg->Run_manually_point_flag==1)
			{
				if(eSuspended==eTaskGetState(DMX_DisplayHandle))vTaskResume(DMX_DisplayHandle);
				if(eSuspended!=eTaskGetState(Run_manually_pointHandle))vTaskSuspend(Run_manually_pointHandle);
				menu_arg->Run_manually_point_flag=0;
			}
	    menu_arg->munu_select = Third_level_menu;	
			menu_arg->Fourth_level_menu_selection =0;
			break;
		}
		case Fifth_level_menu:
		{
			menu_arg->munu_select = Fourth_level_menu;	
			menu_arg->Fifth_level_menu_selection =0;
			break;
		}
		default:
			break;
	}		
#if 0	
 switch(menu_arg->munu_posit)
 {
	 case menu_first:
	 {
			menu_arg->munu_select = meum_main;
			send_event(Date_handle_event);		 
		 break;
	 }
	 case menu_set:
	 {
		 menu_arg->munu_posit = menu_first;//在第二层转回第一层
		 menu_arg->munu_set_select = lamps_coding;
		 break;
	 }
	 case menu_coding_set:
	 {
			switch(menu_selection.chip_model)
			{
				case SM1751X:
				{
					if(menu_arg->munu_set_select != coding_rate)menu_arg->save_flag = 1;
					break;
				}
				case SM165XX:
				{
					if(menu_arg->munu_set_select != SM165xx_coding_rate)menu_arg->save_flag = 1;
					break;
				}
				case SM17500:
				{
					if(menu_arg->munu_set_select != SM17500_Coding_rate)menu_arg->save_flag = 1;
					break;
				}
				case SM16703:
				{
					menu_arg->save_flag = 1;
					break;
				}
				case SM16804:
				{
					menu_arg->save_flag = 1;
					break;
				}					
				default:
				break;
			}
			menu_arg->munu_set_coding_select = 0;			
			menu_arg->munu_posit = menu_set;		 
		 break;
	 }
	 case menu_coding_arg_set:
	 {
		 menu_arg->munu_posit = menu_coding_set;
		 //menu_arg->munu_set_param_select = Auto_Addr;
		 break;
	 }
	 default:
		 break;
 }
#endif
	ui_display();
	//vTaskResume( ui_display_DisplayHandle );
#if debug
	printf("key1_long_fuc \r\n");
#endif
	//vTaskResume( check_DisplayHandle );
}

/***************************************************** 
 * @Function:  key2_long_fuc
 * @Description:   key2 long event handle
 * @param  event 
 * @retval void
******************************************************/


void key2_long_fuc(int event)
{
	#if 0
	vTaskSuspend( check_DisplayHandle );
	//vTaskSuspend( ui_display_DisplayHandle );
	clear_screen();
	display_string_8x16(5,34,"load111 ...");	
	 sm17500x_send_add(0x0004,3,1024);
	 // sm17500x_write_arg(&SM17500_arg);
	// sm17500x_send_self_channel(3,&SM17500_arg);

	vTaskDelay( 1000 / portTICK_RATE_MS );
	 // sm17500x_write_elect_arg(&SM17500_arg);
	vTaskDelay( 1000 / portTICK_RATE_MS );
	//sm17500x_send_self_add(0x0001,1024);
	vTaskDelay( 1000 / portTICK_RATE_MS );
	vTaskResume( check_DisplayHandle );
//	vTaskResume( DMX_DisplayHandle );
//	DMX512_Clear_init();
////	sm1751x_write_arg(&SM1751_arg);
////	sm1751x_write_arg(&SM1751_arg);
//	send_add(0x0001,3,5);
//	send_add(0x0001,3,5);
	//DMX512_Clear_init();
//vTaskResume( ui_display_DisplayHandle );
ui_display();
#endif
#if debug
	printf("key2_long_fuc \r\n");
#endif
}

/***************************************************** 
 * @Function:  key3_long_fuc
 * @Description:   key3 long event handle
 * @param  event 
 * @retval void
******************************************************/
//DMX512_arg_t SM1751_test_arg =
//{
//	.AUTO =1,
//	.REDIS = 0,
//	.CH1_CH0 =2,
//	.RGBW_R =128,
//	.RGBW_G =128,
//	.RGBW_B =128,
//  .RGBW_W =128,
//	.RGBW_IG = 15,
//	.RGBW_IR = 15,
//	.RGBW_IW = 15,
//	.RGBW_IB = 15,
//};
void key3_long_fuc(int event)
{
//	vTaskSuspend( check_DisplayHandle );
//	sm1751x_write_arg(&SM1751_test_arg);
//	sm1751x_write_arg(&SM1751_test_arg);
//	vTaskDelay( 1000 / portTICK_RATE_MS );
//	vTaskResume( check_DisplayHandle );
#if 0
	if(menu_selection.munu_select != meum_main)return;//限制长按加减冲突
	vTaskSuspend( DMX_DisplayHandle );
	vTaskSuspend( ui_display_DisplayHandle );
	PB1=0;
	clear_screen();
	display_string_8x16(5,34,"load ...");
		switch(menu_selection.chip_model)
		{
			case SM1751X:
			{
				  display_string_8x16(1,34,"SM1751X ");
					switch(chip_param_sm17512.coding_data_change_flag)
					{
						case lamps:
						{			
							sm1751x_Write_addr(chip_param_sm17512.lamps_num,chip_param_sm17512.coding_form_num,chip_param_sm17512.chip_chanle_num);
							break;
						}
						case Channel:
						{			
							sm1751x_Write_addr(chip_param_sm17512.Channel_num,chip_param_sm17512.Channel_form,chip_param_sm17512.chip_chanle_num);
							break;
						}
						default:
							break;
					}
					vTaskDelay( 1000 / portTICK_RATE_MS );
					vTaskDelay( 1000 / portTICK_RATE_MS );
					
					DMX512_Clear_init();
					sm1751x_write_arg(&SM1751_arg);
					sm1751x_write_arg(&SM1751_arg);
					vTaskDelay( 1000 / portTICK_RATE_MS );
					
				break;
			}
			case SM165XX:
			{
					display_string_8x16(1,34,"SM165XX ");
				  SM165XX_Send_addr(chip_param_sm165XX.lamps_num,chip_param_sm165XX.chip_chanle_num ,chip_param_sm165XX.coding_form_num,0xd2,SM165XX_arg_set.Power_Up_Color);
				//SM165XX_Send_addr(0x0002,2,3,0xd2,0);
					vTaskDelay( 1000 / portTICK_RATE_MS );
					SM165XX_Send_addr(chip_param_sm165XX.lamps_num,chip_param_sm165XX.chip_chanle_num ,chip_param_sm165XX.coding_form_num,0,SM165XX_arg_set.Power_Up_Color);
					vTaskDelay( 1000 / portTICK_RATE_MS );
				break;
			}
			default:
			break;
		}

	vTaskResume( DMX_DisplayHandle );
	vTaskResume( ui_display_DisplayHandle );
	PB1=1;
#endif
#if debug
	printf("key3_long_fuc \r\n");
#endif
}


/***************************************************** 
 * @Function:  key4_long_fuc
 * @Description:   key4 long event handle
 * @param  event 
 * @retval void
******************************************************/
void key4_long_fuc(int event)
{
//	vTaskSuspend( check_DisplayHandle );
//	sm1751x_Write_addr(0x01,3,512);
//		vTaskDelay( 1000 / portTICK_RATE_MS );
//	vTaskDelay( 1000 / portTICK_RATE_MS );
//	vTaskResume( check_DisplayHandle );
#if debug
	printf("key4_long_fuc \r\n");
#endif
}

/***************************************************** 
 * @Function:  add_event
 * @Description:   Event add of the list
 * @param  event  (event_list)
 * @param  cbFunc (Function pointer)
 * @retval void
******************************************************/
void add_event(int event,event_func cbFunc)	
{
	eventFuncTable[event] = cbFunc;	
}


/***************************************************** 
 * @Function:  remove_event
 * @Description:   Event remove of the list
 * @param  event  (event_list)
 * @param  cbFunc (Function pointer)
 * @retval void
******************************************************/
void remove_event(int event,event_func cbFunc)
{
	eventFuncTable[event] = NULL;
}



/***************************************************** 
 * @Function:  send_event
 * @Description:   Send event  to Queue
 * @param  event  (event_list)
 * @retval void
******************************************************/
void send_event(Eevent event)
{
	Tevent evt; 
	evt.event = event;
	if(eventQueue != NULL){
		if(xQueueSendToBack(eventQueue,&evt,EVENT_QUEUE_DELAY) == pdFAIL){
#if debug	
			printf("event queue send fail==========\r\n");
#endif			
		}
	}
}

/***************************************************** 
 * @Function:  event_loop
 * @Description:   thread task for Event loop to handld functions
 * @param  prama  
 * @retval void
******************************************************/
void event_loop(void* prama)
{
	for( ; ;){
		Tevent evt;
		if (xQueueReceive(eventQueue,&evt,EVENT_QUEUE_DELAY) == pdPASS) {
		//default event process
			if (eventFuncTable[evt.event] != NULL) {
#if debug					
				printf("eventFuncTable\r\n");
#endif
				eventFuncTable[evt.event](NULL);
			}
		}
		vTaskDelay( 50 / portTICK_RATE_MS );
	}
}


/***************************************************** 
 * @Function:  event_init
 * @Description:   event init to creat task and queue
 * @param  prama  
 * @retval void
******************************************************/
void event_init()
{
	eventQueue = xQueueCreate(EVENT_QUEUE_SIZE,sizeof(Tevent));
	
	xTaskCreate(event_loop, (signed portCHAR *) "event_loop", ( unsigned short ) 50, NULL, 4, NULL);
}


/***************************************************** 
 * @Function:  event_process
 * @Description:   event process begin
 * @param  prama  
 * @retval void
******************************************************/
void event_process()
{
	event_init();
	add_event(key1,key1_fuc);
	add_event(key2,key2_fuc);
	add_event(key3,key3_fuc);
	add_event(key4,key4_fuc);
	add_event(key1_long,key1_long_fuc);
	add_event(key2_long,key2_long_fuc);
	add_event(key3_long,key3_long_fuc);
	add_event(key4_long,key4_long_fuc);
	add_event(Date_handle_event,data_handle_fuc);
	
	vTaskDelay( 100 / portTICK_RATE_MS );
}
