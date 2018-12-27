/***************************************************
* @Copyright(c) 2016-2100.  hwl.  All rights reserved.
* @FileName: config.c
* @Author:   hwl 
* @Description:   Event handle header file
* @Version:
* @History:         
      <author>  <time>      <version >   <desc>
				hwl      18/9/7       1.0     
****************************************************/


#ifndef _EVENT_PROCESS_H
#define _EVENT_PROCESS_H

#define EVENT_QUEUE_SIZE	10
#define EVENT_QUEUE_DELAY	10

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

typedef enum event_type {
	key1	= 0x00,
	key2	,
	key3	,
	key4  ,
	key1_long,
	key2_long,
	key3_long,
	key4_long,
	Date_handle_event,
	EVENT_MAX,
}Eevent;




/*****************************

meum_main  主显示


effect_choice 效果显示
chip_choice   芯片显示

													menu_first菜单设置在主显示
menu_setup   菜单设置--->>menu_set 菜单设置在主显示确认后的设置菜单
													menu_coding_set 菜单设置里的写码设置

*****************************/
/*主菜单显示类型*/
//typedef enum key_event {
//	meum_main	= 0x00,	
//	effect_choice  ,
//	chip_choice	,
//	menu_setup,
//	key_event_max,
//}key_event;

typedef enum First_level_menu_type {
	First_level_menu_main_submenu	= 0x00,
	First_level_menu_first_submenu,
	First_level_menu_second_submenu  ,
	First_level_menu_third_submenu	,
	First_level_menu_max_submenu,
}First_level_menu_type;

typedef enum Second_level_menu_Equipment_type {
	Second_level_menu_Equipment_first_submenu = 0x00,
	Second_level_menu_Equipment_second_submenu  ,
	Second_level_menu_Equipment_third_submenu	,
	Second_level_menu_Equipment_fourth_submenu	,
	Second_level_menu_Equipment_fifth_submenu ,
	Second_level_menu_Equipment_sixth_submenu ,
	Second_level_menu_Equipment_max_submenu,
}Second_level_menu_Equipment_type;

typedef enum Second_level_menu_Effect_type {
	Second_level_menu_Effect_first_submenu = 0x00,
	Second_level_menu_Effect_second_submenu ,
	Second_level_menu_Effect_max_submenu,
}Second_level_menu_Effect_type;

typedef enum Second_level_menu_Chip_type {
	Second_level_menu_Chip_first_submenu = 0x00,
	Second_level_menu_Chip_second_submenu ,
	Second_level_menu_Chip_third_submenu	,
	Second_level_menu_Chip_fourth_submenu	,	
	Second_level_menu_Chip_fifth_submenu	,
	Second_level_menu_Chip_max_submenu,
}Second_level_menu_Chip_type;

typedef enum Third_level_menu_type {
	Third_level_menu_first_submenu = 0x00,
	Third_level_menu_second_submenu ,
	Third_level_menu_third_submenu	,
	Third_level_menu_fourth_submenu	,	
	Third_level_menu_fifth_submenu	,
	Third_level_menu_sixth_submenu	,	
	Third_level_menu_seventh_submenu	,
	Third_level_menu_max_submenu,
}Third_level_menu_type;

typedef enum Fourth_level_menu_type {
	Fourth_level_menu_first_submenu = 0x00,
	Fourth_level_menu_second_submenu ,
	Fourth_level_menu_third_submenu	,
	Fourth_level_menu_fourth_submenu	,	
	Fourth_level_menu_fifth_submenu	,
	Fourth_level_menu_sixth_submenu	,	
	Fourth_level_menu_max_submenu ,	
}Fourth_level_menu_type;

typedef enum Fifth_level_menu_type {
	Fifth_level_menu_first_submenu = 0x00,
	Fifth_level_menu_second_submenu ,
	Fifth_level_menu_third_submenu	,
	Fifth_level_menu_fourth_submenu	,		
	Fifth_level_menu_max_submenu ,	
}Fifth_level_menu_type;

/*写码菜单里的类型*/
//typedef enum menu_coding_set_type {
//	coding_addr = 0x00,
//	coding_form,
//	coding_chip,
//	menu_coding_set_max,
//}menu_coding_set_type;

/*设置里的位置*/
//typedef enum menu_position{
//	menu_first = 0x00,
//	menu_set ,
//	menu_coding_set,
//	menu_coding_arg_set,
//	menu_max,
//}menu_position;









/*sm1751xx*/
/*设置菜单里的类型*/
//typedef enum menu_set_type {
//	lamps_coding = 0x00,
//	Channel_coding,
//	coding_rate,
//	//Refresh_rate,
//	Param_set,
//	menu_set_max,
//}menu_set_type;
/*参数的类型*/
//typedef enum Param_type{
//	Auto_Addr = 0x00,
//	No_Signal_switch ,
//	Channel_Mode,
//	Selfcheck_color,
//	Current_Regulation,
//	Param_type_Max,
//}Param_type;



/*sm1651x******************/
//typedef enum menu_SM165xx_set_type {

//	SM165xx_lamps_coding =0x00,
//	SM165xx_coding_rate,
//	SM165xx_Param_set,
//	SM165xx_menu_set_max,
//}menu_SM165xx_set_type;
//typedef enum Param_type_SM165XX{
//	SM165XX_Selfcheck_color=0x00,
//	SM165XX_Param_type_max,
//}Param_type_SM165XX;





/*sm17500******************/
//typedef enum Param_type_SM17500{
//	SM17500_self_check_color = 0x00,
//	SM17500_IC_select,
//	SM17500_No_Signal_switch ,
//	SM17500_lamp_color,
//	SM17500_RTZ_transcoding,
//	SM17500_Current_gain_mode,
//	SM17500_Param_type_Max,
//}Param_type_SM17500;

//typedef enum menu_SM17500_set_type {

//	SM17500_lamps_coding =0x00,
//	SM17500_self_lamps_coding,
//	Current_Gain,
//	Self_Lamps_Num,
//	SM17500_Coding_rate,
//	SM17500_Param_set,
//	//SM17500_Load,
//	SM17500_menu_set_max,
//}menu_SM17500_set_type;
typedef enum SM17500_curren_mode_type {

	SM17500_No_current_gain =0x00,
	SM17500_4bit_current_gain,
	SM17500_5bit_current_gain,
	SM17500_6bit_current_gain,
	SM17500__current_gaint_max,
}SM17500_curren_mode_type;


//typedef enum RZ_type{
//	CHIP_NUM = 0x00,
//	IOUT_NUM ,
//	RZ_type_MAX,	
//}RZ_type;


/*RGB的类型*/
typedef enum RGB_type{
	RGB = 0x03,
	RGBW ,
	RGB_TAPE_MAX,	
}RGB_type;

/*速率的类型*/
typedef enum Rate_num_type{
	baud_250k = 0x00,
	baud_500k ,
	baud_1000k,
	baud_max,
}Rate_num_type;

typedef enum Freq_num_type{
	Freq_baud_200k = 0x00,
	Freq_baud_250k ,
	Freq_baud_400k,
	Freq_baud_500k ,
	Freq_baud_660k,
	Freq_baud_750k ,
	Freq_baud_800k,
	Freq_baud_1000k,	
	Freq_baud_max,
}Freq_num_type;
typedef enum Colour_num_type{
	Colour_R = 0x01,
	Colour_RG ,
	Colour_RGB,
	Colour_RGBW ,
	Colour_RGBWY,
	Colour_max ,
}Colour_num_type;

typedef enum CH_type{
 CH1= 0X00,
 CH2,
 CH3,
 CH4,
 CH_MAX,
}CH_type;

typedef enum RGB_Color_type{
 RGB_R=0X00,
 RGB_G,
 RGB_B,
 RGB_W,	
 RGB_MAX,
}RGB_Color_type;






typedef enum SM165XX_Selfcheck_color_type{
	No_Bright_Lights=0x00,
	White_Light_50_Percent,
	White_Light_100_Percent,
	Blue_Light_50_Percent,
	SM165XX_Selfcheck_color_max,
}SM165XX_Selfcheck_color_type;

/*颜色的类型*/
typedef enum color_type {
	
	loop_lamp = 0x00 ,
	RED	,	
	GREEN  ,
	BLUE	,
	WHITE,
	CRAN,
	PURPLE,
	YELLOW,
	gradual_change,
	RACE_LAMP,
	Gamma_lamp,
	color_max,
}color_type;

typedef enum coding_data_change_type{
 lamps= 0X00,
 Channel,
}coding_data_change_type;

//typedef enum chip_model_type{
// SM1751X = 0X00,
// SM165XX,
// SM17500,
// SM16703,
// SM16804,
// chip_model_max,	
//}chip_model_type;

typedef enum Protocol_type{
 DMX512 = 0X00,
 RTZ,
 Protocol_max,	
}Protocol_type;

typedef enum RZ_chip_type{
 RZ_SM16703 = 0X00,
 RZ_SM16804,
 RZ_chip_max,	
}RZ_chip_type;

typedef void (*event_func)(int event);

typedef struct {
	Eevent event; 
}Tevent;


/*写码芯片的数据*/
typedef struct chip_param_t
{
	
/*灯具数据*/
volatile	unsigned int lamps_num ;
/*通道数据*/
volatile	unsigned int Channel_num;
volatile	unsigned int Self_Channel_num;	
volatile	unsigned int Channel_form;	
/*速率数据*/
volatile	unsigned char Rate_num;
/*RGB数据*/	
volatile	unsigned char coding_form_num;	
	
volatile	unsigned int chip_chanle_num;

volatile  unsigned char coding_data_change_flag;
	
volatile	unsigned int Run_manually_point	;
volatile  unsigned char red_data;
volatile  unsigned char green_data;
volatile  unsigned char blue_data;	
}chip_param_t;
    
/*协议输出设置参数*/
typedef struct Protocol_output_parameter_t
{
	 volatile	unsigned char Color_num;
	volatile uint32_t On_load_channe_param;
	volatile uint32_t Output_frequency_param;
	volatile uint32_t Play_speed_param;
	volatile  unsigned char colour_num_param;

	volatile  unsigned char Protocol_type_param;
	volatile  unsigned char RZ_chip_param;
	
	volatile  unsigned char Red_data;
	volatile  unsigned char Green_data;
	volatile  unsigned char Blue_data;
	volatile  unsigned char Write_data;
	volatile  unsigned char Yellow_data;
	
	volatile  unsigned char Adjust_Red_data;
	volatile  unsigned char Adjust_Green_data;
	volatile  unsigned char Adjust_Blue_data;
	volatile  unsigned char Adjust_Write_data;
	volatile  unsigned char Adjust_Yellow_data;

}Protocol_output_parameter_t;

typedef struct menu_selection_t
{
	volatile	unsigned char Run_manually_point_flag ;
	//volatile	unsigned char save_flag ;
	/*颜色数据*/	
//  volatile	unsigned char Color_num;
	volatile	char chip_model;
	
  /*菜单选择数据*/
	volatile	char munu_select;
//	volatile	char munu_set_select;	
//	volatile	char munu_set_coding_select;
//	volatile	char munu_set_param_select;
//	volatile	char munu_posit;
//	volatile	char munu_set_param_select_rgbset;	
	
  volatile	char	First_level_menu_selection;
	volatile	char	Second_level_menu_selection;
	volatile	char	Third_level_menu_selection;
	volatile	char	Fourth_level_menu_selection;
	volatile	char	Fifth_level_menu_selection;
	//volatile	unsigned char refresh_rate;	
}menu_selection_t;

//	typedef  struct _all_struct
//	{
//		chip_param_t chip_param_sm17501;
//		
//	}_all_struct_t;

typedef enum Menu_selection_leve_tpye{
 First_level_menu = 0X00,
 Second_level_menu,
 Third_level_menu,
 Fourth_level_menu,
 Fifth_level_menu,
 Menu_selection_leve_Max,	
}Menu_selection_leve_tpye;

void send_event(Eevent event);
void event_process(void);
char save_data(void);
//void serial_printf(char *fmt, ...);

#endif
