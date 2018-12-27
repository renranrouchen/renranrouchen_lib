/***************************************************
* @Copyright(c) 2016-2100.  hwl.  All rights reserved.
* @FileName: DMX512_API.c
* @Author:   hwl 
* @Description:  DMX512 Application functions
* @Version:
* @History:         
      <author>  <time>      <version >   <desc>
				hwl      18/9/7       1.0     
****************************************************/


#include "M451Series.h"
#include "DMX512.H"
#include "DMX512_API.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"



#define DELAYMS	vTaskDelay( 20 / portTICK_RATE_MS )
//extern char TXDData[LENGTH]; //接收缓冲区200个数据
typedef void (* dmx_fuc)(void);
extern chip_param_t chip_param_sm17512;
extern chip_param_t chip_param_sm165XX;
extern chip_param_t chip_param_sm17500;

extern RZ_arg_t chip_param_sm16703P;
extern RZ_arg_t chip_param_sm16804P;
extern menu_selection_t  menu_selection;

extern Protocol_output_parameter_t Equipment_set_param;

xTaskHandle xgradual_displayHandle;
xTaskHandle RACELAMP_displayHandle;
xTaskHandle Run_manually_pointHandle;
xTaskHandle DMX_DisplayHandle;



void gradual_display(void* prama);
void DM512_loop_lamp_DISPLAY(void);
void RACE_LAMP_display(void* prama);
void Run_manually_point_display(void* prama);
void DMX_Display(void* prama)
{
	
	for(;;)
	{
		//不是跑马时，挂起
//		if(Equipment_set_param.Color_num != RACE_LAMP)
//		{
//			if(eSuspended!=eTaskGetState(RACELAMP_displayHandle))vTaskSuspend(RACELAMP_displayHandle);
//		}		
	//	printf("DMX_Display--->>>\r\n");
		color_setup((color_type)Equipment_set_param.Color_num);	
	}
}
	


/***************************************************** 
 * @Function:  DMX_Init
 * @Description:   DMX512 init
 * @param  void 
 * @retval void
******************************************************/

void DMX_Init(void) //DMX512初始化
{

	xTaskCreate(DMX_Display, (signed portCHAR *) "DMX_Display", ( unsigned short ) 100, NULL, 2, &DMX_DisplayHandle);
	xTaskCreate(gradual_display, (signed portCHAR *) "gradual_display", ( unsigned short ) 100, gradual_change_display, 2, &xgradual_displayHandle);
	xTaskCreate(RACE_LAMP_display, (signed portCHAR *) "RACE_LAMP_display", ( unsigned short ) 100, RACE_LAMP_fuc, 2, &RACELAMP_displayHandle);
	xTaskCreate(Run_manually_point_display, (signed portCHAR *) "Run_manually_point_display", ( unsigned short ) 50, NULL, 2, &Run_manually_pointHandle);
}


/***************************************************** 
 * @Function:  gradual_change_display
 * @Description:   gradual_change_display
 * @param  void 
 * @retval void
******************************************************/
/*key1 PA0 KEY2 PB0 KEY3 PD2 KEY3 PD3*/
void gradual_change_display(void)
{
	unsigned char x = 0,i=0;
	uchar switch_data[3]={0};
#if debug
	printf("123\r\n");
#endif
	for(i=0;i<14;i++)
	{
		for(x=0;x<255;x=x+1)
		{
			switch (i)
			{
				case 0:
				{
					switch_data[0]=0;switch_data[1]=0;switch_data[2]=x;
					break;
				}
				case 1:
				{
					switch_data[0]=0;switch_data[1]=0;switch_data[2]=~x;
					break;
				}
				case 2:
				{
					switch_data[0]=0;switch_data[1]=x;switch_data[2]=0;
					break;
				}
				case 3:
				{
					switch_data[0]=0;switch_data[1]=~x;switch_data[2]=0;
					break;
				}
				case 4:
				{
					switch_data[0]=x;switch_data[1]=0;switch_data[2]=0;
					break;
				}
				case 5:
				{
					switch_data[0]=~x;switch_data[1]=0;switch_data[2]=0;
					break;
				}
				case 6:
				{
					switch_data[0]=0;switch_data[1]=x;switch_data[2]=x;
					break;
				}
				case 7:
				{
					switch_data[0]=0;switch_data[1]=~x;switch_data[2]=~x;
					break;
				}
				case 8:
				{
					switch_data[0]=x;switch_data[1]=0;switch_data[2]=x;
					break;
				}
				case 9:
				{
					switch_data[0]=~x;switch_data[1]=0;switch_data[2]=~x;
					break;
				}
				case 10:
				{
					switch_data[0]=x;switch_data[1]=x;switch_data[2]=0;
					break;
				}
				case 11:
				{
					switch_data[0]=~x;switch_data[1]=~x;switch_data[2]=0;
					break;
				}
				case 12:
				{
					switch_data[0]=x;switch_data[1]=x;switch_data[2]=x;
					break;
				}
				case 13:
				{
					switch_data[0]=~x;switch_data[1]=~x;switch_data[2]=~x;
					break;
				}				
			}
			if(( Equipment_set_param.Color_num != gradual_change))return;	
//			Protocol_output_Send_data(switch_data[0],switch_data[1],switch_data[2],0,0,&Equipment_set_param);
//			Protocol_output_Send_data(switch_data[0],switch_data[1],switch_data[2],0,0,&Equipment_set_param);	
		
			switch(Equipment_set_param.Protocol_type_param)
			{
				case DMX512:
				{
					Protocol_output_Send_data(switch_data[0],switch_data[1],switch_data[2],0,0,&Equipment_set_param);
					Protocol_output_Send_data(switch_data[0],switch_data[1],switch_data[2],0,0,&Equipment_set_param);	
					break;
				}
				case RTZ:
				{
					switch(Equipment_set_param.RZ_chip_param)
					{
						case RZ_SM16703:
						{
							SM16703P_Display(switch_data[0],switch_data[1],switch_data[2],&chip_param_sm16703P);
							break;
						}
						case RZ_SM16804:
						{
							SM16804P_Display(switch_data[0],switch_data[1],switch_data[2],&chip_param_sm16804P);
							break;
						}
					}
					break;
				}
				
			}			
			
			DELAYMS;
		}		
	}

}


extern char Dmx512_write_data[2048];
/***************************************************** 
 * @Function:  RACE_LAMP_fuc
 * @Description:    RACE LAMPS fuc
 * @param  void 
 * @retval void
******************************************************/
void RACE_LAMP_fuc(void)
{
	uchar k=0,l=0;
  unsigned int i=0,j=0;
	for(l=0;l<3;l++)
	{
		for(j=0;j<Equipment_set_param.On_load_channe_param;j=j+Equipment_set_param.colour_num_param)
		{
			for(i=0;i<Equipment_set_param.On_load_channe_param;i++)
			{
				Dmx512_write_data[i]=0;
			}
			Dmx512_write_data[j+l]=255;
			DMX_SendPacket(Dmx512_write_data,Equipment_set_param.On_load_channe_param);
			DMX_SendPacket(Dmx512_write_data,Equipment_set_param.On_load_channe_param);
			//printf("race_lamp\r\n");
			for(k= 0; k<15;k++)
			{
			if( (PA0==0 || PB0 == 0 ||Equipment_set_param.Color_num != RACE_LAMP))return;
			vTaskDelay( 10/ portTICK_RATE_MS )	;
			}
		}
	}
	//printf("race_lamp>>>>>>\r\n");
	
#if 0
	
	unsigned char j=0,k=0;
#if debug
	printf("RACE_LAMP_display========>>>>\r\n");
#endif
	//color_setup((color_type)color_count);

for(k=0;k<5;k++)
{
	DMX_SendPacket(run_data[k],15);
	DMX_SendPacket(run_data[k],15);
	for(j= 0; j<20;j++)
	{
	if(menu_selection.munu_select == effect_choice && (PA0==0 || PB0 == 0 ||menu_selection.Color_num != RACE_LAMP))return;
	vTaskDelay( 10/ portTICK_RATE_MS )	;
	}	
}
for(k=0;k<4;k++)
{
	DMX_SendPacket(run_data[3-k],15);
	DMX_SendPacket(run_data[3-k],15);
	for(j= 0; j<20;j++)
	{
	if(menu_selection.munu_select == effect_choice && (PA0==0 || PB0 == 0 ||menu_selection.Color_num != RACE_LAMP))return;
	vTaskDelay( 10/ portTICK_RATE_MS )	;
	}	
}
#endif
}
#if 0
void DMX_color_display(unsigned char (*arg)[3],color_type type)
{
			switch(menu_selection.chip_model)
		{
			case SM1751X:
			{
				DMX_Send_data(arg[type][0],arg[type][1],arg[type][2],&chip_param_sm17512);
				break;
			}
			case SM165XX:
			{
				DMX_Send_data(arg[type][0],arg[type][1],arg[type][2],&chip_param_sm165XX);
				break;
			}
			default:
			break;
		}	
	
}
#endif

/***************************************************** 
 * @Function:  gradual_display
 * @Description:   thread task for  gradual chage display
 * @param  prama 
 * @retval void
******************************************************/
void gradual_display(void* prama)
{
	dmx_fuc dmx_fuc_t  = (dmx_fuc)prama;
	for( ; ;){
#if debug	
			printf("gradual_display--------------->>>\r\n");
#endif
			if(Equipment_set_param.Color_num != gradual_change)  
			{

				if(eSuspended!=eTaskGetState(xgradual_displayHandle))vTaskSuspend(xgradual_displayHandle);
				
			}

			
			dmx_fuc_t();
		
	}	
}
uchar dealay_dm512(color_type type)
{
		unsigned int j = 0;
		for(j = 0;j < (Equipment_set_param.Play_speed_param*20);j++)
		{
			if( Equipment_set_param.Color_num !=type )
			{
			return 1;
			}
			//printf("color_type%d",type);
			vTaskDelay( 50 / portTICK_RATE_MS );
		}	
		return 0;
}
/***************************************************** 
 * @Function:  RACE_LAMP_display
 * @Description:   thread task for RACE LAMPS display
 * @param  prama 
 * @retval void
******************************************************/
void RACE_LAMP_display(void* prama)
{
	dmx_fuc dmx_fuc_t  = (dmx_fuc)prama;
	for( ; ;){
#if debug	
			printf("RACE_LAMP_display--------------->>>\r\n");
#endif
			
			if(Equipment_set_param.Color_num != RACE_LAMP)  
			{

				if(eSuspended!=eTaskGetState(RACELAMP_displayHandle))vTaskSuspend(RACELAMP_displayHandle);
				
			}	
			dmx_fuc_t();
			
		 // vTaskDelay( 2000/ portTICK_RATE_MS );
	}	
}

void Run_manually_point_display(void* prama)
{
	unsigned int i =0;
	for( ; ;)
	{
			//printf("Run_manually_point_display--->>>\r\n");
			if(menu_selection.Run_manually_point_flag != 1)  
			{
				if(eSuspended==eTaskGetState(DMX_DisplayHandle))vTaskResume(Run_manually_pointHandle);
				if(eSuspended!=eTaskGetState(Run_manually_pointHandle))vTaskSuspend(Run_manually_pointHandle);
				
			}
			if(eSuspended!=eTaskGetState(DMX_DisplayHandle))vTaskSuspend(DMX_DisplayHandle);
			switch(menu_selection.Second_level_menu_selection)
			{
				case Second_level_menu_Chip_first_submenu:
				{
					Run_manually_point_fuc(chip_param_sm17512.Run_manually_point);
					Run_manually_point_fuc(chip_param_sm17512.Run_manually_point);
					break;
				}
				case Second_level_menu_Chip_second_submenu:
				{
					Run_manually_point_fuc(chip_param_sm165XX.Run_manually_point);
					Run_manually_point_fuc(chip_param_sm165XX.Run_manually_point);
					break;
				}
				case Second_level_menu_Chip_third_submenu:
				{
					Run_manually_point_fuc(chip_param_sm17500.Run_manually_point);
					Run_manually_point_fuc(chip_param_sm17500.Run_manually_point);
					break;
				}
			}
			
			for(i = 0;i < (Equipment_set_param.Play_speed_param*20);i++)
			{
				if( menu_selection.Run_manually_point_flag !=1 )
				{
				return ;
				}
				//printf("color_type%d",type);
				vTaskDelay( 50 / portTICK_RATE_MS );
			}
	}
	
}

/***************************************************** 
 * @Function:  DM512_color_display_task
 * @Description:   a kinds of color to display
 * @param  void 
 * @retval void
******************************************************/
void color_loop(color_type type);
void DM512_loop_lamp_DISPLAY(void)
{
	unsigned char i = 0 ;
	unsigned int j = 0;
	for(i=RED;i<gradual_change;i++)
	{
		color_loop((color_type)i);
//		if(menu_selection.munu_select != meum_main)	
//		{
			for(j = 0;j < (Equipment_set_param.Play_speed_param*20);j++)
			{
				if(  (Equipment_set_param.Color_num !=loop_lamp)  )
				{
				return ;
				}
				vTaskDelay( 50 / portTICK_RATE_MS );
			}	
//		}
	}
	//DMX_SendPacket(test_data,6);
}

//SM16512/SM17512数据需要发送两次相同的数据
void DM512_RED_DISPLAY(void)
{
	switch(Equipment_set_param.Protocol_type_param)
	{
		case DMX512:
		{
			Protocol_output_Send_data(255,0,0,0,0,&Equipment_set_param);
			Protocol_output_Send_data(255,0,0,0,0,&Equipment_set_param);
			break;
		}
		case RTZ:
		{
			switch(Equipment_set_param.RZ_chip_param)
			{
				case RZ_SM16703:
				{
					SM16703P_Display(255,0,0,&chip_param_sm16703P);
					break;
				}
				case RZ_SM16804:
				{
					SM16804P_Display(255,0,0,&chip_param_sm16804P);
					break;
				}
			}
			break;
		}
		
	}
	
	if(dealay_dm512(RED))return;	
	
#if 0	
	//DMX_Send_data(0,0,255);
	  switch(menu_selection.chip_model)
		{
			case SM1751X:
			{
				DMX_Send_data(0,0,255,&chip_param_sm17512);
				break;
			}
			case SM165XX:
			{
				DMX_Send_data(0,0,255,&chip_param_sm165XX);
				break;
			}
			case SM17500:
			{
				DMX_Send_data(0,0,255,&chip_param_sm17500);
				break;
			}
			case SM16703:
			{
				SM16703P_Display(255,0,0,&chip_param_sm16703P);
				break;
			}
			case SM16804:
			{
				SM16804P_Display(255,0,0,&chip_param_sm16804P);
				break;
			}				
			default:
			break;
		}
		if(dealay_dm512(RED))return;		
#endif	

}
void DM512_GREEN_DISPLAY(void)
{
	switch(Equipment_set_param.Protocol_type_param)
	{
		case DMX512:
		{
			Protocol_output_Send_data(0,255,0,0,0,&Equipment_set_param);
			Protocol_output_Send_data(0,255,0,0,0,&Equipment_set_param);
			break;
		}
		case RTZ:
		{
			switch(Equipment_set_param.RZ_chip_param)
			{
				case RZ_SM16703:
				{
					SM16703P_Display(0,255,0,&chip_param_sm16703P);
					break;
				}
				case RZ_SM16804:
				{
					SM16804P_Display(0,255,0,&chip_param_sm16804P);
					break;
				}
			}
			break;
		}
	}
		
	
		if(dealay_dm512(GREEN))return;
}
void DM512_BLUE_DISPLAY(void)
{
	switch(Equipment_set_param.Protocol_type_param)
	{
		case DMX512:
		{
			Protocol_output_Send_data(0,0,255,0,0,&Equipment_set_param);
			Protocol_output_Send_data(0,255,0,0,0,&Equipment_set_param);
			break;
		}
		case RTZ:
		{
			switch(Equipment_set_param.RZ_chip_param)
			{
				case RZ_SM16703:
				{
					SM16703P_Display(0,0,255,&chip_param_sm16703P);
					break;
				}
				case RZ_SM16804:
				{
					SM16804P_Display(0,0,255,&chip_param_sm16804P);
					break;
				}
			}
			break;
		}
	}
	
	
	if(dealay_dm512(BLUE))return;
}
void DM512_WHITE_DISPLAY(void)
{
	switch(Equipment_set_param.Protocol_type_param)
	{
		case DMX512:
		{
			Protocol_output_Send_data(255,255,255,0,0,&Equipment_set_param);
			Protocol_output_Send_data(255,255,255,0,0,&Equipment_set_param);
			break;
		}
		case RTZ:
		{
			switch(Equipment_set_param.RZ_chip_param)
			{
				case RZ_SM16703:
				{
					SM16703P_Display(255,255,255,&chip_param_sm16703P);
					break;
				}
				case RZ_SM16804:
				{
					SM16804P_Display(255,255,255,&chip_param_sm16804P);
					break;
				}
			}
			break;
		}
	}
	
	if(dealay_dm512(WHITE))return;
}
void DM512_CRAN_DISPLAY(void)
{
	switch(Equipment_set_param.Protocol_type_param)
	{
		case DMX512:
		{
			Protocol_output_Send_data(0,255,255,0,0,&Equipment_set_param);
			Protocol_output_Send_data(0,255,255,0,0,&Equipment_set_param);
			break;
		}
		case RTZ:
		{
			switch(Equipment_set_param.RZ_chip_param)
			{
				case RZ_SM16703:
				{
					SM16703P_Display(0,255,255,&chip_param_sm16703P);
					break;
				}
				case RZ_SM16804:
				{
					SM16804P_Display(0,255,255,&chip_param_sm16804P);
					break;
				}
			}
			break;
		}
	}
	
	if(dealay_dm512(CRAN))return;
}
void DM512_PUPRLE_DISPLAY(void)
{
	switch(Equipment_set_param.Protocol_type_param)
	{
		case DMX512:
		{
			Protocol_output_Send_data(255,0,255,0,0,&Equipment_set_param);
			Protocol_output_Send_data(255,0,255,0,0,&Equipment_set_param);
			break;
		}
		case RTZ:
		{
			switch(Equipment_set_param.RZ_chip_param)
			{
				case RZ_SM16703:
				{
					SM16703P_Display(255,0,255,&chip_param_sm16703P);
					break;
				}
				case RZ_SM16804:
				{
					SM16804P_Display(255,0,255,&chip_param_sm16804P);
					break;
				}
			}
			break;
		}
	}
	
	if(dealay_dm512(PURPLE))return;
}
void DM512_YELLOW_DISPLAY(void)
{
	switch(Equipment_set_param.Protocol_type_param)
	{
		case DMX512:
		{
			Protocol_output_Send_data(255,255,0,0,0,&Equipment_set_param);
			Protocol_output_Send_data(255,255,0,0,0,&Equipment_set_param);
			break;
		}
		case RTZ:
		{
			switch(Equipment_set_param.RZ_chip_param)
			{
				case RZ_SM16703:
				{
					SM16703P_Display(255,255,0,&chip_param_sm16703P);
					break;
				}
				case RZ_SM16804:
				{
					SM16804P_Display(255,255,0,&chip_param_sm16804P);
					break;
				}
			}
			break;
		}
	}
	
	if(dealay_dm512(YELLOW))return;
}
void DM512_gradual_change_DISPLAY(void)
{
	
	if(dealay_dm512(gradual_change))return;
}
void DM512_RACE_LAMP_DISPLAY(void)
{
//	if(Equipment_set_param.Color_num == RACE_LAMP)
//	{
//		 if(eSuspended==eTaskGetState(RACELAMP_displayHandle))
//		 {
//			 vTaskResume(RACELAMP_displayHandle);
//			 printf("vTaskResume_RACELAMP_displayHandle\r\n");
//		 }
//	}
	if(dealay_dm512(RACE_LAMP))return;

}

void DM512_Gamma_LAMP_DISPLAY(void)
{
	Protocol_output_Send_data(Equipment_set_param.Adjust_Red_data,Equipment_set_param.Adjust_Green_data,Equipment_set_param.Adjust_Blue_data,Equipment_set_param.Adjust_Write_data,Equipment_set_param.Adjust_Yellow_data,&Equipment_set_param);
	Protocol_output_Send_data(Equipment_set_param.Adjust_Red_data,Equipment_set_param.Adjust_Green_data,Equipment_set_param.Adjust_Blue_data,Equipment_set_param.Adjust_Write_data,Equipment_set_param.Adjust_Yellow_data,&Equipment_set_param);
	printf("DM512_Gamma_LAMP_DISPLAY\r\n");
	if(dealay_dm512(Gamma_lamp))return;
}

color_display_cb color_setup_pFunc = NULL;
color_display_cb color_loop_pFunc = NULL;
/*选择用结构体初始化还是数组初始化*/
#if struct_func
color_setup_function_t color_setup_ops =
{
	.DM512_RED_DISPLAY_CB = DM512_RED_DISPLAY,
	.DM512_GREEN_DISPLAY_CB = DM512_GREEN_DISPLAY,
	.DM512_BLUE_DISPLAY_CB = DM512_BLUE_DISPLAY,
	.DM512_WHITE_DISPLAY_CB = DM512_WHITE_DISPLAY,
	.DM512_CRAN_DISPLAY_CB = DM512_CRAN_DISPLAY,
	.DM512_PUPRLE_DISPLAY_CB = DM512_PUPRLE_DISPLAY,
	.DM512_YELLOW_DISPLAY_CB = DM512_YELLOW_DISPLAY,
	.DM512_gradual_change_DISPLAY_CB = DM512_gradual_change_DISPLAY,
	.DM512_RACE_LAMP_DISPLAY_CB = DM512_RACE_LAMP_DISPLAY,
	.DM512_Gamma_LAMP_DISPLAY_CB = DM512_Gamma_LAMP_DISPLAY,
};
#else
color_display_cb cb_num[color_max] =
{
	DM512_loop_lamp_DISPLAY,
	DM512_RED_DISPLAY,
	DM512_GREEN_DISPLAY,
	DM512_BLUE_DISPLAY,
	DM512_WHITE_DISPLAY,
	DM512_CRAN_DISPLAY,
	DM512_PUPRLE_DISPLAY,
	DM512_YELLOW_DISPLAY,
	DM512_gradual_change_DISPLAY,
	DM512_RACE_LAMP_DISPLAY,
	DM512_Gamma_LAMP_DISPLAY,
};
#endif


/***************************************************** 
 * @Function:  color_setup
 * @Description:   task options
 * @param  type 
 * @retval void
******************************************************/
void color_setup(color_type type)
{

#if struct_func
	unsigned long *pFuncAddr = (unsigned long *)(&color_setup_ops);
	color_setup_pFunc = (color_display_cb)*(pFuncAddr+type);
#else
	color_setup_pFunc = cb_num[type];
#endif
	if(color_setup_pFunc != NULL)
	{
		color_setup_pFunc();
	}

}

void color_loop(color_type type)
{


	color_loop_pFunc = cb_num[type];

	if(color_loop_pFunc != NULL)
	{
		color_loop_pFunc();
	}

}
