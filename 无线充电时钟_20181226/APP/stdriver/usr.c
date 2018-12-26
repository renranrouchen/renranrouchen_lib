#include "usr.h"
#include "ca51f3sfr.h"
#include "ca51f3xsfr.h"
#include "FD616.H"
#include "delay.h"
#include <math.h>
#include "config.h"
struct DATA_TIME time;
struct DATA_ALARM alarm1,alarm2,alarm3;


//float   TEM2,TEM4;
double	 TEM,TEM_F;	
unsigned char  ADC_AVG_NUM[3],ADC_Display=0,ADC_Display1=0,ADC_ONE=1;
extern t_fd612_ops fd612_base_ops; 
extern FD612_Struct_DispBuff FD612_base_DispBuff;
bit HalfSecFlag,time1_second;
bit blink_flag,blink2_flag,blink3_flag;
UINT8 dc=0,dc_over_flag=1,key_time_2second,key_time_2count,key_delaytime=4,back_TIME_Flag=0,dc_1second=0,delaytime_1second=2;
unsigned char SET_Long_Flag=0,SET_Short_Flag=0,UP_Long_Flag=0,UP_Short_Flag=0,DOWN_Short_Flag=0,DOWN_Long_Flag=0;
bit long_flag=0,back_first=1;
UINT8 set_status_flag=0;
unsigned long	flag_year_leap=0;
//UINT8 alarm_hour_long_up_down_flag=0,alarm_min_long_up_down_flag=0;
bit adc_first=1;

UINT8 hour_pm_flag=0,hour_12_24_flag=0,hour_chage_state=0;
//UINT8 alarm1_pm_flag=0,alarm2_pm_flag=0,alarm3_pm_flag=0;
unsigned char flag_display=0,display_flag_hur_min=1,SET_Short_Flag_hur_min=0;
unsigned char display_flag_mon_day=1,SET_Short_Flag_mon_day=0,display_flag_year=1;
//UINT8 switch_2to1_flag=0;
//unsigned char alarm_on_flag=0,alarm1_on_dispaly=0,alarm2_on_dispaly=0,alarm3_on_dispaly=0;
bit key_first=1;
bit  long_add_flag=0,up_down_flag=0;
unsigned char  ADC_TIME_Flag=0;
bit alarm_voice_flag=0;
//unsigned char alarm_flag=0,alarm_voice_flag=0,alarm3_voice_flag=0,alarm2_voice_flag=0,alarm3_flag=0,alarm2_flag=0;
unsigned char  back_voice_Flag=0,alarm_clock_flag=0;

unsigned char sd_flag=0,sd_select_first=1;
bit sd_mode_flag=0;
unsigned char voice_flag=0,voice_fisrt=1;
unsigned char power_delay_time=0,power_first=0;
unsigned char  SET_Short_Flag_display=0;
bit display_flag_week=1;

void TIMER0_ISR (void) interrupt 1
{
		TH0 =0XFf;
		TL0 =0Xba;
		buzzer=~buzzer;

}

void RTC_ISR (void) interrupt 13 	 
{
	if(RTCIF & RTC_MF)			//�����ж�
	{
		RTCIF = RTC_MF;			
	}
	if(RTCIF & RTC_HF)			//�����ж�
	{
		RTCIF = RTC_HF;
//		HalfSecFlag = 1;
		dc++;
		if(dc==dc_over_flag)
		{
		HalfSecFlag = 1;
		time1_second=1;	
		alarm_clock_flag=1;			
		dc=0;			
		}
		key_time_2count++;
		if(key_time_2count==key_delaytime)
		{
			key_time_2count=0;
			key_time_2second=1;
//			ADC_TIME_Flag++;
//			back_TIME_Flag++;
//			back_voice_Flag++;
		}
		dc_1second++;
		if(dc_1second==delaytime_1second)
		{
			dc_1second=0;
			 ADC_TIME_Flag++;
			back_TIME_Flag++;
			back_voice_Flag++;
			power_delay_time++;
		}
	}	

}	


const unsigned char  dis[10] = {FD612_DISP_0,FD612_DISP_1,FD612_DISP_2,FD612_DISP_3,FD612_DISP_4,FD612_DISP_5,FD612_DISP_6,FD612_DISP_7,FD612_DISP_8,FD612_DISP_9 };
/**
 *  @brief  BCDתʮ����
 *  @param  bcd
 *  @return decimal
 *  @note    
 *  @note    
 */
UINT8 bcd_decimal(UINT8 bcd)
{
return bcd-(bcd >> 4)*6;
}

/**
 *  @brief  ʮ����תBCD
 *  @param  decimal
 *  @return bcd
 *  @note    
 *  @note    
 */
UINT8 decimal_bcd(UINT8 decimal)
{
return  (decimal+(decimal / 10)*6);
}

UINT8  leep_year(UINT16  year)
{
	if((year%400 == 0) || year%4 == 0 && year%100!=0)
	return 1;
	else
	return 0;
}


void Alarm_display(void)
{
//	if(alarm1_on_dispaly||alarm2_on_dispaly||alarm3_on_dispaly)
//	{
//		 if(alarm1_on_dispaly)
//		 {
//				alarm1_sleep( );
//		 }
//		 	 if(alarm2_on_dispaly)
//		 {
//				alarm2_sleep( );
//		 }
//		 if(alarm3_on_dispaly)
//		 {
//				alarm3_sleep( );
//		 } 
//	}
		 if(alarm1.alarm_on_dispaly)
 {

	   alarm_sleep_judge(&alarm1);
 }
 	 if(alarm2.alarm_on_dispaly)
 {

	 alarm_sleep_judge(&alarm2);
 }
 	 if(alarm3.alarm_on_dispaly)
 {

	 alarm_sleep_judge(&alarm3);
 }
}


void key_statue(void)
{
	from_ds1302_read_time_data();

if(alarm1.alarm_flag==0 && alarm2.alarm_flag==0&& alarm3.alarm_flag==0){
			if(SET==0)
			{
									
				key_into_init( );
				while(!SET)
				{
//					if(SET_Long_Flag==0&&set_status_flag !=2) 
					if(SET_Short_Flag_display==0||SET_Short_Flag_display==1||SET_Short_Flag_display==2)
					{
					 if(HalfSecFlag)
						{
							if(blink_flag)		
							{
								blink_flag=0;														
								FD616_DispBuff.DispData[14] &= ~(1<<4);
								if(hour_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
								if(alarm1.alarm_on_dispaly||alarm2.alarm_on_dispaly||alarm3.alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
								FD616_Refresh();			
							}
							else 
							{
									blink_flag=1;	
									FD616_DispBuff.DispData[14] |= 1<<4;
																			
							}
							if(hour_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
							if(alarm1.alarm_on_dispaly||alarm2.alarm_on_dispaly||alarm3.alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
							FD616_Refresh();
							HalfSecFlag=0;	
						}	
										
					}
					//

					key_display_up_down( );
					if(key_time_2second==1)
					{
						long_flag=1;
						SET_Long_Flag=1;
						SET_Short_Flag_display=0;
						display_flag_year=0;
						display_flag_hur_min=1;
						display_flag_mon_day=1;
						display_flag_week=1;
						
						set_time_year_blink(0);
						display_all();
					}
				}				
				key_time_2second=0;
				while(!SET);
				if(long_flag==0)
				{
					SET_Short_Flag=1;
					key_time_2second=0;
				}
			}//set
	if(UP==0)
	{
		key_into_init( );
		while(!UP)
		{
			
     key_display_up_down( );
			if(key_time_2second==1)
			{
				//TR0=0;
				long_flag=1;
				UP_Long_Flag=1;			
				long_add_flag=1;//?????
				if(SET_Long_Flag==1)
				{
					switch(SET_Short_Flag_display)
						{
							case 0:
							{
								year_long_up_down(0);
								break;				

				
							}
							case 1:
							{
								mon_long_up_down(0);
								break;				

				
							}
							case 2:
							{
								mday_long_up_down(0);				
								break;				

				
							}
//							case 3:
//							{
//							
//								break;				

//				
//							}
							case 4:
							{
								hour_long_up_down(0);
								break;				

				
							}
							case 5:
							{
								min_long_up_down(0) ;
								break;				

				
							}
//							case 6:
//							{
//								
//								
//								break;				

//				
//							}
							case 7:
							{
								alarm_hour_up_down(1,0,&alarm1);
						
//								alarm_hour_long_up_down(1,1,alarm1);
								break;				

				
							}
							case 8:
							{
								alarm_min_up_down(1,0,&alarm1);
//								alarm_min_long_up_down(1,1,alarm1);
								break;				

				
							}							
//							case 9:
//							{
//								
//								break;				

//				
//							}
							case 10:
							{
								alarm_hour_up_down(1,0,&alarm2);
							
//							alarm_hour_long_up_down(2,1, alarm2);
								break;				

				
							}	
							case 11:
							{
//							alarm_min_long_up_down(2,1,alarm2);
								alarm_min_up_down(1,0,&alarm2);
								break;				

				
							}
//							case 12:
//							{
//								
//								break;				

//				
//							}
							case 13:
							{
								alarm_hour_up_down(1,0,&alarm3);
							
//								alarm_hour_long_up_down(3,1, alarm3);
								break;				

				
							}	
							case 14:
							{
//								alarm_min_long_up_down(3,1, alarm3);
								alarm_min_up_down(1,0,&alarm3);
								break;				

				
							}
								default:
								{

									break;
								}
						}
				}
				
			}	
		}
/*����ALARM�Լ��Լ�ʱ��Ӽ�һ */	
//		switch(alarm_hour_long_up_down_flag)
//		{
//			case 1:
//			{
//				alarm1.hour--;
//				if(hour_12_24_flag)
//				{
//					if(alarm1.hour==0)alarm1.hour=12;
//				}
//				else 
//				{
//					if(alarm1.hour>=24)alarm1.hour=23;
//				}
//				alarm_hour_long_up_down_flag=0;				
//				break;
//			}
//			case 2:
//			{
//				alarm2.hour--;
//				if(hour_12_24_flag)
//				{
//					if(alarm2.hour==0)alarm2.hour=12;
//				}
//				else 
//				{
//					if(alarm2.hour>=24)alarm2.hour=23;
//				}				
//				alarm_hour_long_up_down_flag=0;				
//				break;
//			}			
//			case 3:
//			{
//				alarm3.hour--;
//			   if(hour_12_24_flag)
//				{
//					if(alarm3.hour==0)alarm3.hour=12;
//				}
//				else 
//				{
//					if(alarm3.hour>=24)alarm3.hour=23;
//				}
//				alarm_hour_long_up_down_flag=0;				
//				break;
//			}			
//		
//			default :
//			break;			
//		}
//		switch(alarm_min_long_up_down_flag)
//		{
//			case 1:
//			{
//				alarm1.minute--;
//					if(alarm1.minute>=60)alarm1.minute=59;
//			
//				alarm_min_long_up_down_flag=0;				
//				break;
//			}
//			case 2:
//			{
//				alarm2.minute--;
//				if(alarm2.minute>=60)alarm2.minute=59;
//				alarm_min_long_up_down_flag=0;				
//				break;
//			}			
//			case 3:
//			{
//				alarm3.minute--;
//				if(alarm3.minute>=60)alarm3.minute=59;
//				alarm_min_long_up_down_flag=0;				
//				break;
//			}			
//			
//			default :
//			break;			
//		}		
			long_add_flag=0;
			key_time_2second=0;	
		while(!UP);
		if(long_flag==0)
		{
			UP_Short_Flag=1;
			key_time_2second=0;
		}
		}


				//down??
		if(DOWN==0)
		{
			key_into_init( );
			while(!DOWN)
			{
				  key_display_up_down( );
					if(key_time_2second==1)	
					{
						long_add_flag=1;
						TR0=0;
						long_flag=1;
						DOWN_Long_Flag=1;
						
						if(SET_Long_Flag==1)
						{
					switch(SET_Short_Flag_display)
						{
							case 0:
							{
								year_long_up_down(1);
								break;				

				
							}
							case 1:
							{
								mon_long_up_down(1);
								break;				

				
							}
							case 2:
							{
								mday_long_up_down(1);				
								break;				

				
							}
//							case 3:
//							{
//							
//								break;				

//				
//							}
							case 4:
							{
								hour_long_up_down(1);
								break;				

				
							}
							case 5:
							{
								min_long_up_down(1) ;
								break;				

				
							}
//							case 6:
//							{
//								
//								
//								break;				

//				
//							}
							case 7:
							{
								alarm_hour_up_down(1,1,&alarm1);
//								alarm_hour_long_up_down(1,2,alarm1);
								break;				

				
							}
							case 8:
							{
								alarm_min_up_down(1,1,&alarm1);
//								alarm_min_long_up_down(1,2,alarm1);
								break;				

				
							}							
//							case 9:
//							{
//								
//								break;				

//				
//							}
							case 10:
							{
								alarm_hour_up_down(1,1,&alarm2);
//							alarm_hour_long_up_down(2,2, alarm2);
								break;				

				
							}	
							case 11:
							{
								alarm_min_up_down(1,1,&alarm2);
//							alarm_min_long_up_down(2,2,alarm2);
								break;				

				
							}
//							case 12:
//							{
//								
//								break;				

//				
//							}
							case 13:
							{
								alarm_hour_up_down(1,1,&alarm3);
//								alarm_hour_long_up_down(3,2, alarm3);
								break;				

				
							}	
							case 14:
							{
								alarm_min_up_down(1,1,&alarm3);
//								alarm_min_long_up_down(3,2, alarm3);
								break;				

				
							}
								default:
								{

									break;
								}
						}
						}
						else 
						{
							if( ADC_ONE==1)
							{
								if(ADC_Display==0)ADC_Display=1;
								else ADC_Display=0;
								switch (ADC_Display)
								 {
									 case 0:
									 {
											FD612_PotiveTNage(FD612_DIG9_ADDR,dis[(unsigned char)TEM/10], &FD612_base_DispBuff);
											FD612_PotiveTNage(FD612_DIG10_ADDR,dis[(unsigned char)TEM%10], &FD612_base_DispBuff);	
										  FD612_PotiveTNage(FD612_DIG11_ADDR,FD612_DISP_cc, &FD612_base_DispBuff);
											FD612_base_DispBuff.DispData[FD612_DIG8_ADDR] |= 1<<6;
											FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
											ADC_Display1=0;
										 break;
									 }
										

										case 1:
										{
											ADC_AVG_NUM[2]   = (unsigned char)TEM_F/100;
											ADC_AVG_NUM[1]   = ((unsigned char)TEM_F-ADC_AVG_NUM[2]*100 )/10;
											ADC_AVG_NUM[0]   = ((unsigned char)TEM_F-ADC_AVG_NUM[2]*100 )%10;					
											FD612_PotiveTNage(FD612_DIG9_ADDR,dis[ADC_AVG_NUM[2]], &FD612_base_DispBuff);
											FD612_PotiveTNage(FD612_DIG10_ADDR,dis[ADC_AVG_NUM[1]], &FD612_base_DispBuff);	
											FD612_PotiveTNage(FD612_DIG11_ADDR,dis[ADC_AVG_NUM[0]], &FD612_base_DispBuff);
											FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
											ADC_Display1=1;
			//								ADC_Display=0;
											break;
										}	
											default :
											break;
								}
								ADC_ONE=0;
							}							
						}
					}
			}
			
//		switch(alarm_hour_long_up_down_flag)
//		{
//			case 4:
//			{
//				alarm1.hour++;
//				if(hour_12_24_flag)
//				{
//					if(alarm1.hour==13)alarm1.hour=1;
//				}
//				else 
//				{
//					if(alarm1.hour>=24)alarm1.hour=0;
//				}				
//				alarm_hour_long_up_down_flag=0;				
//				break;
//			}			
//			case 5:
//			{
//				alarm2.hour++;
//				if(hour_12_24_flag)
//				{
//					if(alarm2.hour==13)alarm2.hour=1;
//				}
//				else 
//				{
//					if(alarm2.hour>=24)alarm2.hour=0;
//				}				
//				alarm_hour_long_up_down_flag=0;				
//				break;
//			}
//			case 6:
//			{
//				alarm3.hour++;
//				if(hour_12_24_flag)
//				{
//					if(alarm3.hour==13)alarm3.hour=1;
//				}
//				else 
//				{
//					if(alarm3.hour>=24)alarm3.hour=0;
//				}					
//				alarm_hour_long_up_down_flag=0;				
//				break;
//			}
//			default :
//			break;			
//		}
//		switch(alarm_min_long_up_down_flag)
//		{
//			case 4:
//			{
//				alarm1.minute++;
//				if(alarm1.minute==60)alarm1.minute=0;
//				alarm_min_long_up_down_flag=0;				
//				break;
//			}			
//			case 5:
//			{
//				alarm2.minute++;
//				if(alarm2.minute==60)alarm2.minute=0;
//				alarm_min_long_up_down_flag=0;				
//				break;
//			}
//			case 6:
//			{
//				alarm3.minute++;
//				if(alarm3.minute==60)alarm3.minute=0;
//				alarm_min_long_up_down_flag=0;				
//				break;
//			}			
//			default :
//			break;			
//		}		
					ADC_ONE=1;
					long_add_flag=0;		
					key_time_2second=0;
					while(!DOWN);
				if(long_flag==0)
				{
					DOWN_Short_Flag=1;
					key_time_2second=0;
				}
		}
		//��λ����
				if(RESET1==0)
				{
					  TR0=1;
						Delay_ms(70);
						TR0=0;
						buzzer=0;
					  InitDS1302( );
					  FD616_DispBuff.DispState=FD616_INTENS3;
						FD616_Command(FD616_DispBuff.DispState);
						FD612_base_DispBuff.DispState=FD612_DISP_ON|FD612_INTENS3;
						FD612_Command(&fd612_base_ops,FD612_base_DispBuff.DispState);	
					
//						display_all();
						while(!RESET1);
				}
		
	}
//			if(alarm_flag==1)
//		{
//			if(SET==0||UP==0||DOWN==0)
//			{
//				alarm_flag=0;
//		//		PWM_CTRL &= ~(1<<3);
//				alarm_voice_flag=0;		
//				buzzer=0;
//			}
//		}
//		if(alarm2_flag==1)
//		{
//			if(SET==0||UP==0||DOWN==0)
//			{
//				alarm2_flag=0;
//		//		PWM_CTRL &= ~(1<<3);
//				alarm2_voice_flag=0;
//				buzzer=0;
//			}
//		}
//		if(alarm3_flag==1)
//		{
//			if(SET==0||UP==0||DOWN==0)
//			{
//				alarm3_flag=0;
//		//		PWM_CTRL &= ~(1<<3);
//				alarm3_voice_flag=0;	
//				buzzer=0;
//			}
//		}
key_off_alarm_clock(&alarm1);
key_off_alarm_clock(&alarm2);
key_off_alarm_clock(&alarm3);

}



void display(void)
{

		from_ds1302_read_time_data();
		display_all();
			
			if(SET_Long_Flag)
			{
					if(key_first)
					{
							key_first=0;
							UP_Short_Flag=0;
							DOWN_Short_Flag = 0;
		    	}
//	

					if(SET_Short_Flag)
					{
						SET_Short_Flag_display++;
						SET_Short_Flag=0;
					
						if(SET_Short_Flag_display>15)
						{
							SET_Short_Flag_display=0;
						}
					}
						
						switch(SET_Short_Flag_display)
						{
							case 0:
							{
//								display_flag_mon_day=1;
								display_flag_year=0;
								set_time_year_blink(0 );	
								if(UP_Short_Flag == 1)
								{
										time.year++;	
										if(time.year>99)time.year=0;
										UP_Short_Flag = 0;
								}
								if(DOWN_Short_Flag == 1)
								{
										time.year--;
									if(time.year==255)time.year=99;
										DOWN_Short_Flag = 0;
								}	
								time.year= decimal_bcd(time.year);
								Write_addr_byte_ds1302(year_w_addr,time.year);								
								break;								
							}
							case 1:
							{
								display_flag_mon_day=0;
								display_flag_year=1;
								
							
								set_time_mon_blink(0);
								if(UP_Short_Flag == 1)
								{
										time.month++;
										mon_over_judge();
										UP_Short_Flag = 0;
								}
								if(DOWN_Short_Flag == 1)
								{
										time.month--;
										
										mon_over_judge();
										DOWN_Short_Flag = 0;
								}	
								time.month = decimal_bcd(time.month);
								Write_addr_byte_ds1302(mon_w_addr,time.month);				
								break;								
							}
							case 2:
							{
								display_flag_mon_day=0;
								display_flag_year=1;										
								set_time_day_blink(0);
								if(UP_Short_Flag >= 1)
								{
										time.mday++;
										  mday_over_judge( ) ;  								 
										UP_Short_Flag = 0;						
								}	
								if(DOWN_Short_Flag >= 1)
								{
										time.mday--;
										mday_over_judge( ) ;
										DOWN_Short_Flag = 0;
								}					
								time.mday = decimal_bcd(time.mday);
								Write_addr_byte_ds1302(day_w_addr,time.mday);												
								break;								
							}
							case 3:
							{
								time.hour=Read_addr_byte_ds1302(hour_r_addr);
								time.minute=Read_addr_byte_ds1302(min_r_addr);

								if(hour_12_24_flag==1)
								{
										time.hour<<=3;
										time.hour>>=3;
										time.hour &=~(1<<7);
										time.hour &=~(1<<5);
								}
								time.hour = bcd_decimal(time.hour);
								time.minute = bcd_decimal(time.minute);					 
								display_flag_hur_min=0;
								display_flag_mon_day=1;								
								display_12_24_blink(0);	
								if(UP_Short_Flag==1)
								{
										if(hour_12_24_flag)
										{//12/24
										 
											hour_12_24_flag=0;
											hour_chage_input(hour_12_24_flag);
											alarm_12switch24(&alarm1);
											alarm_12switch24(&alarm2);
											alarm_12switch24(&alarm3);
																						

										}
										else	
										{//24/12
											hour_12_24_flag=1;
											hour_chage_input(hour_12_24_flag);
											alarm_24switch12(&alarm1);	
											alarm_24switch12(&alarm2);
											alarm_24switch12(&alarm3);
											

												
													
										}
										UP_Short_Flag=0;
								

										
							}								
								break;								
							}
							case 4:
							{
								display_flag_hur_min=0;
								if(hour_12_24_flag==1)
								{
									time.hour<<=3;
									time.hour>>=3;
									time.hour &=~(1<<7);
									time.hour &=~(1<<5);
								}
                set_time_hour_blink(0);
								if(UP_Short_Flag==1)
								{										
										time.hour++;

										hour_over_judge(0);
										UP_Short_Flag=0;
								}
									if(DOWN_Short_Flag == 1)
								{
										time.hour--;	
										hour_over_judge(1);								

										DOWN_Short_Flag = 0;
								}

							
								break;								
							}
							case 5:
							{
								display_flag_hur_min=0;
								set_time_minute_blink(	0);
								if(UP_Short_Flag == 1)
								{
										time.minute++;
										min_over_judge(0);
										UP_Short_Flag = 0;
									
								}
								if(DOWN_Short_Flag == 1)
								{
										time.minute--;
										min_over_judge(1);
										DOWN_Short_Flag = 0;
																				

								}
							
								break;								
							}
							case 6:
							{


								
								display_flag_week=0;
								display_flag_hur_min=0;
								alarm_on_off_set(1,&alarm1 );
																
								
								break;								
							}
							case 7:
							{
								alarm_hour_set(&alarm1);
							
								break;								
							}
							case 8:
							{
								alarm_min_set(&alarm1);
								
								break;								
							}
							case 9:
							{
								display_flag_week=0;
								display_flag_hur_min=0;
								alarm_on_off_set(2,&alarm2 );

							
								break;								
							}
							case 10:
							{
								alarm_hour_set(&alarm2);
								break;								
							}	
							case 11:
							{
								alarm_min_set(&alarm2);
									
								break;								
							}
							case 12:
							{
								display_flag_week=0;
								display_flag_hur_min=0;
								alarm_on_off_set(3,&alarm3 );
								
								break;								
							}
							case 13:
							{
								alarm_hour_set(&alarm3);

								break;								
							}	
							case 14:
							{
								alarm_min_set(&alarm3);

								break;								
							}
								default:
								{
									SET_Short_Flag_display=0;
									SET_Long_Flag=0;
									display_flag_mon_day=1;
									display_flag_hur_min=1;				
									display_flag_year=1;
									display_flag_week=1;
									break;
								}
						}
							if(back_TIME_Flag==over_time)
						{
							display_flag_mon_day=1;
							display_flag_hur_min=1;				
							display_flag_year=1;
							display_flag_week=1;
							SET_Long_Flag=0;
							back_TIME_Flag=0;
							back_first=1;
							key_first=1;
							SET_Short_Flag_display=0;																					
						}					
													
			} //set_long_flag
			
}

void alarm_time_init(void)
{
		alarm1.hour  =12;
		alarm1.minute=0;
		alarm1.alarm_pm_flag=0;
		alarm1.alarm_on_dispaly=0;
		alarm1.alarm_flag=0;
		alarm2.hour  =12;
		alarm2.minute=0;
		alarm2.alarm_pm_flag=0;
		alarm2.alarm_on_dispaly=0;
		alarm2.alarm_flag=0;
		alarm3.hour  =12;
		alarm3.minute=0;
		alarm3.alarm_pm_flag=0;
		alarm3.alarm_on_dispaly=0;
		alarm3.alarm_flag=0;
//		buzzer=0;
}


void key_into_init(void)
{
		long_flag=0;
		back_first=1;

		key_time_2second=0;
		key_time_2count=0;


		back_TIME_Flag=0;

	  TR0=1;
		Delay_ms(70);
		TR0=0;
		buzzer=0;


//		back_voice_Flag=0; //���ѹ�����ʱ���¼���

}

void  display_12_24_blink(UINT8 i)
{						
			if(time1_second==1)
			{

					if(blink2_flag)		
				{
						blink2_flag=0;		
					if(hour_12_24_flag)
					{

							anode_display(FD616_DIG4_ADDR,dis[1]);
							anode_display(FD616_DIG5_ADDR,dis[2]);
	
					}
					else
					{

							anode_display(FD616_DIG4_ADDR,dis[2]);
							anode_display(FD616_DIG5_ADDR,dis[4]);
					}
							anode_display(FD616_DIG6_ADDR,FD612_DISP_H);
							anode_display(FD616_DIG7_ADDR,FD612_DISP_);
							FD616_Refresh();

					
				}
				else 
				{
						blink2_flag=1;														
							if(i==0)
							{
							anode_display(FD616_DIG4_ADDR,0x00);
							anode_display(FD616_DIG5_ADDR,0x00);
							anode_display(FD616_DIG6_ADDR,0x00);
							anode_display(FD616_DIG7_ADDR,0x00);
							FD616_Refresh();
							}
				}			
				time1_second=0;
			}	
}


void set_time_year_blink(UINT8 i)
{
			if(time1_second==1)
			{

			if(blink3_flag)
			{
				blink3_flag=0;

				
					FD612_PotiveTNage(FD612_DIG1_ADDR,dis[2], &FD612_base_DispBuff);
					FD612_PotiveTNage(FD612_DIG2_ADDR,dis[0], &FD612_base_DispBuff);	
					FD612_PotiveTNage(FD612_DIG3_ADDR,dis[time.year/10], &FD612_base_DispBuff);	
					FD612_PotiveTNage(FD612_DIG4_ADDR,dis[time.year%10], &FD612_base_DispBuff);
					FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
			}
			else
			{
				blink3_flag=1;
				if(i==0){
				FD612_PotiveTNage(FD612_DIG1_ADDR,0x00, &FD612_base_DispBuff);
				FD612_PotiveTNage(FD612_DIG2_ADDR,0x00, &FD612_base_DispBuff);	
				FD612_PotiveTNage(FD612_DIG3_ADDR,0x00, &FD612_base_DispBuff);	
				FD612_PotiveTNage(FD612_DIG4_ADDR,0x00, &FD612_base_DispBuff);
				FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
				}
			}				
				time1_second=0;
			}	
}

#if 0 
void alarm_bilnk_display(UINT8 i,UINT8 j,UINT8 k,struct DATA_ALARM alarm)
{

					
		 
					if(j==1)
					{

						anode_display(FD616_DIG6_ADDR,dis[alarm.minute/10]);
						anode_display(FD616_DIG7_ADDR,dis[alarm.minute%10]);						
					if(time1_second==1)
					{
							if(blink2_flag)
							{
								anode_display(FD616_DIG4_ADDR,dis[alarm.hour/10]);
								anode_display(FD616_DIG5_ADDR,dis[alarm.hour%10]);
								if(i==1)
								{			if(alarm1_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
								if(i==2)
								{			if(alarm2_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
								if(i==3)
								{			if(alarm3_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}		
								if(alarm1_on_dispaly||alarm2_on_dispaly||alarm3_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
								blink2_flag=0;
								FD616_DispBuff.DispData[14] |= 1<<4;
								FD616_Refresh();

							}
							else
							{
								blink2_flag=1;
								if(k==0){

									anode_display(FD616_DIG4_ADDR,0x00);
									anode_display(FD616_DIG5_ADDR,0x00);
								if(i==1)
								{			if(alarm1_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
								if(i==2)
								{			if(alarm2_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
								if(i==3)
								{			if(alarm3_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}		
								if(alarm1_on_dispaly||alarm2_on_dispaly||alarm3_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
								FD616_DispBuff.DispData[14] |= 1<<4;	
								FD616_Refresh();
								}
							}
					
						time1_second=0;
					}
				}
				
				if(j==2)
				{
						anode_display(FD616_DIG4_ADDR,dis[alarm.hour/10]);
						anode_display(FD616_DIG5_ADDR,dis[alarm.hour%10]);
					

					if(time1_second==1)
					{



							if(blink2_flag)
							{
								blink2_flag=0;
								
	
								anode_display(FD616_DIG6_ADDR,dis[alarm.minute/10]);
								anode_display(FD616_DIG7_ADDR,dis[alarm.minute%10]);	
								if(i==1)
								{			if(alarm1_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
								if(i==2)
								{			if(alarm2_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
								if(i==3)
								{			if(alarm3_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}	
								if(alarm1_on_dispaly||alarm2_on_dispaly||alarm3_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
								blink_flag=0;
								FD616_DispBuff.DispData[14] |= 1<<4;
								FD616_Refresh();
								
							}
							else
							{
								blink2_flag=1;
								if(k==0){
									anode_display(FD616_DIG6_ADDR,0x00);
									anode_display(FD616_DIG7_ADDR,0x00);
								if(i==1)
								{			if(alarm1_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
								if(i==2)
								{			if(alarm2_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
								if(i==3)
								{			if(alarm3_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}	
								if(alarm1_on_dispaly||alarm2_on_dispaly||alarm3_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
								FD616_DispBuff.DispData[14] |= 1<<4;	
								FD616_Refresh();									
								}
							}						
						time1_second=0;
					}		
				}
}
#endif
void display_all(void)
{
		

					
				if(display_flag_hur_min==1)
				{
         hour_min_display();

			
				}
				if(display_flag_mon_day==1)
				{
				if(time.month>=10)
				{
				FD612_PotiveTNage(FD612_DIG7_ADDR,dis[time.month/10], &FD612_base_DispBuff);
				FD612_PotiveTNage(FD612_DIG8_ADDR,dis[time.month%10], &FD612_base_DispBuff);					
				}
				else if(time.month<=9)
				{
				FD612_PotiveTNage(FD612_DIG7_ADDR,0x00, &FD612_base_DispBuff);	
				FD612_PotiveTNage(FD612_DIG8_ADDR,dis[time.month], &FD612_base_DispBuff);					
				}
				FD612_PotiveTNage(FD612_DIG5_ADDR,dis[time.mday/10], &FD612_base_DispBuff);	
				FD612_PotiveTNage(FD612_DIG6_ADDR,dis[time.mday%10], &FD612_base_DispBuff);		
				FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;					
				FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
				}
				if(display_flag_year==1)
				{
				FD612_PotiveTNage(FD612_DIG1_ADDR,dis[2], &FD612_base_DispBuff);
				FD612_PotiveTNage(FD612_DIG2_ADDR,dis[0], &FD612_base_DispBuff);	
				FD612_PotiveTNage(FD612_DIG3_ADDR,dis[time.year/10], &FD612_base_DispBuff);	
				FD612_PotiveTNage(FD612_DIG4_ADDR,dis[time.year%10], &FD612_base_DispBuff);
				FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
				}
				if(adc_first==1)
				{
					ADC_Convert( );
					adc_first=0;
				}
				if(ADC_TIME_Flag==temp_refresh_time)
				{
					ADC_Convert( );
					ADC_TIME_Flag=0;
				}

				de_week();
				
				if(ADC_Display1==0)
				{	
					FD612_PotiveTNage(FD612_DIG9_ADDR,dis[(unsigned char)TEM/10], &FD612_base_DispBuff);
					FD612_PotiveTNage(FD612_DIG10_ADDR,dis[(unsigned char)TEM%10], &FD612_base_DispBuff);	
					FD612_PotiveTNage(FD612_DIG11_ADDR,FD612_DISP_cc, &FD612_base_DispBuff);
					FD612_base_DispBuff.DispData[FD612_DIG8_ADDR] |= 1<<6;
					
//					FD612_PotiveTNage(FD612_DIG9_ADDR,dis[SET_Short_Flag_display], &FD612_base_DispBuff);
//					FD612_PotiveTNage(FD612_DIG10_ADDR,dis[SET_Long_Flag], &FD612_base_DispBuff);
//					FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);					
				}
				else
				{
					ADC_AVG_NUM[2]   = (unsigned char)TEM_F/100;
					ADC_AVG_NUM[1]   = ((unsigned char)TEM_F-ADC_AVG_NUM[2]*100 )/10;
					ADC_AVG_NUM[0]   = ((unsigned char)TEM_F-ADC_AVG_NUM[2]*100 )%10;					
				  FD612_PotiveTNage(FD612_DIG9_ADDR,dis[ADC_AVG_NUM[2]], &FD612_base_DispBuff);
					FD612_PotiveTNage(FD612_DIG10_ADDR,dis[ADC_AVG_NUM[1]], &FD612_base_DispBuff);	
					FD612_PotiveTNage(FD612_DIG11_ADDR,dis[ADC_AVG_NUM[0]], &FD612_base_DispBuff);
					FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);

				}
			
			
}

void set_time_hour_blink(UINT8 i)
{
		
		if(time1_second)
		{


			anode_display(FD616_DIG6_ADDR,dis[time.minute/10]);
			anode_display(FD616_DIG7_ADDR,dis[time.minute%10]);
			if(blink2_flag)
			{
				if(i==0)blink2_flag=0;				
				 anode_display(FD616_DIG4_ADDR,dis[time.hour/10]);
			   anode_display(FD616_DIG5_ADDR,dis[time.hour%10]);
				
			}
			else
			{
				blink2_flag=1;				
				anode_display(FD616_DIG4_ADDR,0x00);
				anode_display(FD616_DIG5_ADDR,0x00);										
			}
		FD616_DispBuff.DispData[14] |= 1<<4;
		if(hour_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
		if(alarm1.alarm_on_dispaly||alarm2.alarm_on_dispaly||alarm3.alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
		FD616_Refresh();	
		time1_second=0;
		}
}


void after_arrive_write_1302(void)
{
	
//			 if(time.hour==12)
//		 {
			 ResetDS1302();							
			 WriteByteDS1302(hour_r_addr); //��ȡʱ�ĵ�ַ
			 hour_chage_state=ReadByteDS1302();
			 if(hour_chage_state&0x20)
			 {
				 hour_chage_state &=~(1<<5);
				 hour_pm_flag=0;
			 }
			 else 
			 {
				 hour_chage_state |=(1<<5);
				 hour_pm_flag=1;
			 }
				ds1302_w_on();		
			 ResetDS1302();
			 WriteByteDS1302(hour_w_addr); //д��ʱ�ĵ�ַ
			 WriteByteDS1302(hour_chage_state);	
				ds1302_w_off();			 
//			}
}

void set_time_minute_blink(UINT8 i)
{
		if(time1_second)
	{
				 anode_display(FD616_DIG4_ADDR,dis[time.hour/10]);
			   anode_display(FD616_DIG5_ADDR,dis[time.hour%10]);

			if(blink2_flag)
			{
				if(i==0)blink2_flag=0;

				 anode_display(FD616_DIG6_ADDR,dis[time.minute/10]);
			   anode_display(FD616_DIG7_ADDR,dis[time.minute%10]);
//				FD616_DispBuff.DispData[14] |= 1<<4;
//				if(hour_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
//				if(alarm1.alarm_on_dispaly||alarm2.alarm_on_dispaly||alarm3.alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
//					FD616_Refresh();
			}
			else
			{
				blink2_flag=1;				
				anode_display(FD616_DIG6_ADDR,0x00);
				anode_display(FD616_DIG7_ADDR,0x00);
//					if(hour_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
//					FD616_DispBuff.DispData[14] |= 1<<4;
//					if(alarm1_on_dispaly||alarm2_on_dispaly||alarm3_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
//					FD616_Refresh();
				
			}	
			FD616_DispBuff.DispData[14] |= 1<<4;
			if(hour_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
			if(alarm1.alarm_on_dispaly||alarm2.alarm_on_dispaly||alarm3.alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
			FD616_Refresh();			
			time1_second=0;
	 }
}


void set_time_mon_blink(UINT8 i)
{
					FD612_PotiveTNage(FD612_DIG5_ADDR,dis[time.mday/10], &FD612_base_DispBuff);	
					FD612_PotiveTNage(FD612_DIG6_ADDR,dis[time.mday%10], &FD612_base_DispBuff);
//				FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<5;
					FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;
						if(time1_second)
					{

						if(blink2_flag)
						{
							blink2_flag=0;
							if(time.month>=10)
							{
							FD612_PotiveTNage(FD612_DIG7_ADDR,dis[time.month/10], &FD612_base_DispBuff);
							FD612_PotiveTNage(FD612_DIG8_ADDR,dis[time.month%10], &FD612_base_DispBuff);	
FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;								
							}
							else if(time.month<=9)
							{
							FD612_PotiveTNage(FD612_DIG7_ADDR,0x00, &FD612_base_DispBuff);	
							FD612_PotiveTNage(FD612_DIG8_ADDR,dis[time.month], &FD612_base_DispBuff);		
FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;								
							}
//							FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;
							FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
							
						}
						else
						{
							blink2_flag=1;
							if(i==0){
							FD612_PotiveTNage(FD612_DIG7_ADDR,0x00, &FD612_base_DispBuff);
							FD612_PotiveTNage(FD612_DIG8_ADDR,0x00, &FD612_base_DispBuff);
								FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;
//								FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;
							FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
							}
						}						
						time1_second=0;
					}
}

void set_time_day_blink(UINT8 i)
{
						if(time.month>=10)
						{
						FD612_PotiveTNage(FD612_DIG7_ADDR,dis[time.month/10], &FD612_base_DispBuff);
						FD612_PotiveTNage(FD612_DIG8_ADDR,dis[time.month%10], &FD612_base_DispBuff);					
						}
						else if(time.month<=9)
						{
						FD612_PotiveTNage(FD612_DIG7_ADDR,0x00, &FD612_base_DispBuff);	
						FD612_PotiveTNage(FD612_DIG8_ADDR,dis[time.month], &FD612_base_DispBuff);					
						}
							if(time1_second)
						{

							if(blink2_flag)
							{
								blink2_flag=0;
								FD612_PotiveTNage(FD612_DIG5_ADDR,dis[time.mday/10], &FD612_base_DispBuff);	
								FD612_PotiveTNage(FD612_DIG6_ADDR,dis[time.mday%10], &FD612_base_DispBuff);
								FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;
//								FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;
								FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
								
							}
							else
							{
								blink2_flag=1;
								if(i==0){
								FD612_PotiveTNage(FD612_DIG5_ADDR,0x00, &FD612_base_DispBuff);
								FD612_PotiveTNage(FD612_DIG6_ADDR,0x00, &FD612_base_DispBuff);
									FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;
//									FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;
								FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
								}
							}							
							time1_second=0;
						 }
}

#if 0
void alarm_key_short_up_down(UINT8 i)
{
		if(UP_Short_Flag==1)
		{
			 switch(i)
			 {
				 case 1:
				 {
					 alarm1.hour++;
					 if(hour_12_24_flag)
						{
							 if(alarm1.hour>=13)
							 {
								 alarm1.hour = 1;

							 }
							 if(alarm1.hour==12)
							 {
								 if(alarm1_pm_flag)alarm1_pm_flag=0;
								 else alarm1_pm_flag=1;
							 }
						}
						else
						{
							if(alarm1.hour >=24)
							{
								alarm1.hour = 0;
							}
						}
						break;
				 }
				 case 2:
				 {
					 alarm2.hour++;
					 if(hour_12_24_flag)
						{
							 if(alarm2.hour>=13)
							 {
								 alarm2.hour = 1;

							 }
							 if(alarm2.hour==12)
							 {
								 if(alarm2_pm_flag)alarm2_pm_flag=0;
								 else alarm2_pm_flag=1;
							 }
						}
						else
						{
							if(alarm2.hour >=24)
							{
								alarm2.hour = 0;
							}
						}
						break;
				 }
				 case 3:
				 {
					 alarm3.hour++;
					 if(hour_12_24_flag)
						{
							 if(alarm3.hour>=13)
							 {
								 alarm3.hour = 1;

							 }
							 if(alarm3.hour==12)
							 {
								 if(alarm3_pm_flag)alarm3_pm_flag=0;
								 else alarm3_pm_flag=1;
							 }
						}
						else
						{
							if(alarm3.hour >=24)
							{
								alarm3.hour = 0;
							}
						}
						break;
				 }
				}			 
		 		
		UP_Short_Flag=0;		
	}
	if(DOWN_Short_Flag == 1)
	{

			 switch(i)
			 {
				 case 1:
				 {
					 alarm1.hour--;
					 if(hour_12_24_flag)
						{
							 if(alarm1.hour>=13||alarm1.hour==0)
							 {
								alarm1.hour = alarm1.hour -244; //���ֵ

							 }
							 if(alarm1.hour==11)
							 {
								 if(alarm1_pm_flag)alarm1_pm_flag=0;
								 else alarm1_pm_flag=1;
							 }
						}
						else
						{
							if(alarm1.hour >=24)
							{
								alarm1.hour = alarm1.hour -232; //���ֵ
							}
						}
						break;
				 }
				 case 2:
				 {
					 alarm2.hour--;
					 if(hour_12_24_flag)
						{
							 if(alarm2.hour>=13||alarm2.hour==0)
							 {
								alarm2.hour = alarm2.hour -244; //���ֵ

							 }
							 if(alarm2.hour==11)
							 {
								 if(alarm2_pm_flag)alarm2_pm_flag=0;
								 else alarm2_pm_flag=1;
							 }
						}
						else
						{
							if(alarm2.hour >=24)
							{
								alarm2.hour = alarm2.hour -232; //���ֵ
							}
						}
						break;
				 }
				 case 3:
				 {
					 alarm3.hour--;
					 if(hour_12_24_flag)
						{
							 if(alarm3.hour>=13||alarm3.hour==0)
							 {
								alarm3.hour = alarm3.hour -244; //���ֵ

							 }
							 if(alarm3.hour==11)
							 {
								 if(alarm3_pm_flag)alarm3_pm_flag=0;
								 else alarm3_pm_flag=1;
							 }
						}
						else
						{
							if(alarm3.hour >=24)
							{
								alarm3.hour = alarm3.hour -232; //���ֵ
							}
						}
						break;
				 }
				
				}
				DOWN_Short_Flag =0;
	}
}
#endif
void key_display_up_down(void)
{
			if(key_time_2second==0&&SET_Long_Flag==1)
			{
					switch(SET_Short_Flag_display)
						{
							case 0:
							{
								set_time_year_blink(1);
								break;								
							}
							case 1:
							{
								set_time_mon_blink(1);
								break;								
							}
							case 2:
							{
								set_time_day_blink(1);				
								break;								
							}
//							case 3:
//							{
//							
//								break;								
//							}
							case 4:
							{
								set_time_hour_blink(1);
								break;								
							}
							case 5:
							{
								set_time_minute_blink(1);
								break;								
							}
//							case 6:
//							{
//								
//								
//								break;								
//							}
							case 7:
							{
								alarm_hour_blink(1,&alarm1);
//								alarm_bilnk_display(1,1,1,alarm1);
								break;								
							}
						   case 8:
							{
//								alarm_bilnk_display(1,2,1,alarm1);
								alarm_min_blink(1,&alarm1);
								break;								
							}
//							case 9:
//							{
//								
//								break;								
//							}
							case 10:
							{
								alarm_hour_blink(1,&alarm2);
//							alarm_bilnk_display(2,1,1,alarm2);
								break;								
							}	
							case 11:
							{
									alarm_min_blink(1,&alarm2);
//								alarm_bilnk_display(2,2,1,alarm2);
								break;								
							}
//							case 12:
//							{
//							
//								break;								
//							}
							case 13:
							{
								alarm_hour_blink(1,&alarm3);
//								alarm_bilnk_display(3,1,1,alarm3);
								break;								
							}	
							case 14:
							{
								alarm_min_blink(1,&alarm3);
//								alarm_bilnk_display(3,2,1,alarm3);
								break;								
							}
								default:
								{

									break;
								}
						}
																																					
			}
}


void hour_long_up_down(UINT8 i)
{

				time.hour=Read_addr_byte_ds1302(hour_r_addr);
				if(hour_12_24_flag==1)
				{
					time.hour<<=3;
					time.hour>>=3;

				}
				time.hour = bcd_decimal(time.hour);
				if(i==0) 
				{
					time.hour++;
					hour_over_judge(0);

				
				}
				if(i==1)
				{ 
						time.hour--;
						hour_over_judge(1);
				}
				time.hour=Read_addr_byte_ds1302(hour_r_addr);
				if(hour_12_24_flag==1)
				{
					time.hour<<=3;
					time.hour>>=3;

				}
				time.hour = bcd_decimal(time.hour);
				anode_display(FD616_DIG4_ADDR,dis[time.hour/10]);
			  anode_display(FD616_DIG5_ADDR,dis[time.hour%10]);
				FD616_DispBuff.DispData[14] |= 1<<4;				
				if(hour_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
				if(alarm1.alarm_on_dispaly||alarm2.alarm_on_dispaly||alarm3.alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
				FD616_Refresh();
				bright_contrl();
			  Delay_ms(up_down_delay_time);
				back_TIME_Flag=0;					
	
}


void min_long_up_down(UINT8 i) 
{
			
			time.minute=Read_addr_byte_ds1302(min_r_addr);
			time.minute = bcd_decimal(time.minute);
			if(i==0)
			{
				time.minute++;
				min_over_judge(0);
			}
			if(i==1)
			{
			time.minute--;
			min_over_judge(1);
			}
				time.minute=Read_addr_byte_ds1302(min_r_addr);
				time.minute = bcd_decimal(time.minute);
				 anode_display(FD616_DIG6_ADDR,dis[time.minute/10]);
			   anode_display(FD616_DIG7_ADDR,dis[time.minute%10]);
			if(hour_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
			FD616_DispBuff.DispData[14] |= 1<<4;
			if(alarm1.alarm_on_dispaly||alarm2.alarm_on_dispaly||alarm3.alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
				 FD616_Refresh();

//			time.minute = decimal_bcd(time.minute);
//			ds1302_w_on();
//			ResetDS1302();
//			WriteByteDS1302(min_w_addr); //
//			WriteByteDS1302(time.minute);
//			ResetDS1302();
//			WriteByteDS1302(sec_w_addr); //д����ĵ�ַ
//			WriteByteDS1302(0);
//			ResetDS1302();
//			ds1302_w_off();
//			time.minute = bcd_decimal(time.minute);
		  Delay_ms(up_down_delay_time);
			back_TIME_Flag=0;
}

void year_long_up_down(UINT8 i)
{
			time.year=Read_addr_byte_ds1302(year_r_addr);
			time.year = bcd_decimal(time.year);
      if(i==1)
			{
				time.year--;
				if(time.year==255)time.year=99;
			}
      if(i==0)
			{
				time.year++;
				if(time.year>99)time.year=0;
					
			}
				FD612_PotiveTNage(FD612_DIG1_ADDR,dis[2], &FD612_base_DispBuff);
				FD612_PotiveTNage(FD612_DIG2_ADDR,dis[0], &FD612_base_DispBuff);	
				FD612_PotiveTNage(FD612_DIG3_ADDR,dis[time.year/10], &FD612_base_DispBuff);	
				FD612_PotiveTNage(FD612_DIG4_ADDR,dis[time.year%10], &FD612_base_DispBuff);
				FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);							 
			time.year = decimal_bcd(time.year);
			Write_addr_byte_ds1302(year_w_addr,time.year);
			Delay_ms(up_down_delay_time);	
			back_TIME_Flag=0;
}


void mon_long_up_down(UINT8 i)
{
		time.month=Read_addr_byte_ds1302(mon_r_addr);
//			ResetDS1302();
//			WriteByteDS1302(mon_r_addr); //��ȡ�ֵĵ�ַ				
//			time.month=ReadByteDS1302();
//			ResetDS1302();
			time.month = bcd_decimal(time.month);	
			if(i==0)
			{
				time.month++;
				mon_over_judge();
			}
			if(i==1)
			{
			time.month--;
			mon_over_judge();
			}	
	
				if(time.month>=10)
				{
				FD612_PotiveTNage(FD612_DIG7_ADDR,dis[time.month/10], &FD612_base_DispBuff);
				FD612_PotiveTNage(FD612_DIG8_ADDR,dis[time.month%10], &FD612_base_DispBuff);	
				FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;					
				}
				else if(time.month<=9)
				{
				FD612_PotiveTNage(FD612_DIG7_ADDR,0x00, &FD612_base_DispBuff);	
				FD612_PotiveTNage(FD612_DIG8_ADDR,dis[time.month], &FD612_base_DispBuff);	
				FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;					
				}

				FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);	

			time.month = decimal_bcd(time.month);
			Write_addr_byte_ds1302(mon_w_addr,time.month);	
		
		  Delay_ms(up_down_delay_time);
			back_TIME_Flag=0;			
}

void mday_long_up_down(UINT8 i)
{
			time.mday=Read_addr_byte_ds1302(day_r_addr);
			time.mday = bcd_decimal(time.mday);
			if(i==0)
			{
				time.mday++;
				mday_over_judge();
			}
			if(i==1)
			{
			time.mday--;
				mday_over_judge();	
			}	

				FD612_PotiveTNage(FD612_DIG5_ADDR,dis[time.mday/10], &FD612_base_DispBuff);	
				FD612_PotiveTNage(FD612_DIG6_ADDR,dis[time.mday%10], &FD612_base_DispBuff);
				FD612_base_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<4;
				FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);				
				time.mday = decimal_bcd(time.mday);
				Write_addr_byte_ds1302(day_w_addr,time.mday);
				time.mday = bcd_decimal(time.mday);
			
				Delay_ms(up_down_delay_time);	
				back_TIME_Flag=0;			
}

#if 0
void alarm_hour_long_up_down(UINT8 alarm_seclect,UINT8 up_down_seclect,struct DATA_ALARM alarm)
{
	if(up_down_seclect==1)
	{
				if(alarm_seclect==1&&UP==0)
				{
					
					alarm1.hour++;
					if(hour_12_24_flag){
						 if(alarm1.hour>=13)
						 {
							 alarm1.hour = 1;
						 }
						if(alarm1.hour==1)
						{
							if(alarm1_pm_flag)alarm1_pm_flag=0;
							else alarm1_pm_flag=1;						
						}
						}
					else
					{
						if(alarm1.hour >=24)
						{
							alarm1.hour = 0;
						}	
					}
					alarm_hour_long_up_down_flag=1;
				}
				if(alarm_seclect==2)
				{
					alarm2.hour++;
					if(hour_12_24_flag){
						 if(alarm2.hour>=13)
						 {
							 alarm2.hour = 1;

						 }
						if(alarm2.hour==1)
						{
							if(alarm2_pm_flag)alarm2_pm_flag=0;
							else alarm2_pm_flag=1;						
						}						 
						}
					else
					{
						if(alarm2.hour >=24)
						{
							alarm2.hour = 0;
						}	
					}
					alarm_hour_long_up_down_flag=2;
				}	
				if(alarm_seclect==3)
				{
					alarm3.hour++;
					if(hour_12_24_flag){
						 if(alarm3.hour>=13)
						 {
							 alarm3.hour = 1;

						 }
						if(alarm3.hour==1)
						{
							if(alarm3_pm_flag)alarm3_pm_flag=0;
							else alarm3_pm_flag=1;						
						}							 
						}
					else
					{
						if(alarm3.hour >=24)
						{
							alarm3.hour = 0;
						}	
					}
					alarm_hour_long_up_down_flag=3;
				}
		}
		if(up_down_seclect==2)
		{
			if(alarm_seclect==1)
			{
				alarm1.hour--;					
				if(hour_12_24_flag){
					 if(alarm1.hour>=13||alarm1.hour==0)
					 {
							alarm1.hour = alarm1.hour -244; //���ֵ
						
					 }
					 	if(alarm1.hour==10)
						{
							if(alarm1_pm_flag)alarm1_pm_flag=0;
							else alarm1_pm_flag=1;						
						}	
					}
				else
				{
					if(alarm1.hour >=24)
					{
						alarm1.hour = alarm1.hour -232; //���ֵ
					}
				}	
			alarm_hour_long_up_down_flag=4;
			}
			if(alarm_seclect==2)
			{
				alarm2.hour--;					
				if(hour_12_24_flag){
					 if(alarm2.hour>=13||alarm2.hour==0)
					 {
							alarm2.hour = alarm2.hour -244; //���ֵ

					 }
					 		if(alarm2.hour==10)
						{
							if(alarm2_pm_flag)alarm2_pm_flag=0;
							else alarm2_pm_flag=1;						
						}	
					}
				else
				{
					if(alarm2.hour >=24)
					{
						alarm2.hour = alarm2.hour -232; //���ֵ
					}
				}	

			alarm_hour_long_up_down_flag=5;
			}
			if(alarm_seclect==3)
			{
				alarm3.hour--;					
				if(hour_12_24_flag){
					 if(alarm3.hour>=13||alarm3.hour==0)
					 {
							alarm3.hour = alarm3.hour -244; //���ֵ
					 }
					 	if(alarm3.hour==10)
						{
							if(alarm3_pm_flag)alarm3_pm_flag=0;
							else alarm3_pm_flag=1;						
						}	
					}
				else
				{
					if(alarm3.hour >=24)
					{
						alarm3.hour = alarm3.hour -232; //���ֵ
					}
				}	
			alarm_hour_long_up_down_flag=6;
			}		
		}
				anode_display(FD616_DIG4_ADDR,dis[alarm.hour/10]);
				anode_display(FD616_DIG5_ADDR,dis[alarm.hour%10]);
				FD616_DispBuff.DispData[14] |= 1<<4;
				if(alarm_seclect==1)
				{			if(alarm1_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
				if(alarm_seclect==2)
				{			if(alarm2_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
				if(alarm_seclect==3)
				{			if(alarm3_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}		
if(alarm1_on_dispaly||alarm2_on_dispaly||alarm3_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;				
				FD616_Refresh();
				Delay_ms(up_down_delay_time);
				
				back_TIME_Flag=0;
				
}
#endif
#if 0
void alarm_min_long_up_down(UINT8 alarm_seclect,UINT8 up_down_seclect,struct DATA_ALARM alarm)
{
	if(up_down_seclect==1)
	{
				if(alarm_seclect==1)
				{
					alarm1.minute++;
				if(alarm1.minute >=60)
				{
					alarm1.minute = 0;
				}
				alarm_min_long_up_down_flag=1;
				}
				if(alarm_seclect==2)
				{
					alarm2.minute++;
				if(alarm2.minute >=60)
				{
					alarm2.minute = 0;
				}
				alarm_min_long_up_down_flag=2;				
				}
				if(alarm_seclect==3)
				{
					alarm3.minute++;
				if(alarm3.minute >=60)
				{
					alarm3.minute = 0;
				}
				alarm_min_long_up_down_flag=3;				
				}	
		}
		if(up_down_seclect==2)
		{
				if(alarm_seclect==1)
				{
					alarm1.minute--;
				if(alarm1.minute>60)
				{
						alarm1.minute = alarm1.minute -196; //���ֵ
				}
				alarm_min_long_up_down_flag=4;				
				}
				if(alarm_seclect==2)
				{
					alarm2.minute--;
				if(alarm2.minute>60)
				{
						alarm2.minute = alarm2.minute -196; //���ֵ
				}
				alarm_min_long_up_down_flag=5;				
				}
				if(alarm_seclect==3)
				{
					alarm3.minute--;
				if(alarm3.minute>60)
				{
						alarm3.minute = alarm3.minute -196; //���ֵ
				}
				alarm_min_long_up_down_flag=6;				
				}			
		}
					anode_display(FD616_DIG6_ADDR,dis[alarm.minute/10]);
					anode_display(FD616_DIG7_ADDR,dis[alarm.minute%10]);
				if(alarm_seclect==1)
				{			if(alarm1_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
				if(alarm_seclect==2)
				{			if(alarm2_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}
				if(alarm_seclect==3)
				{			if(alarm3_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;}		
				if(alarm1_on_dispaly||alarm2_on_dispaly||alarm3_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;				
				 FD616_DispBuff.DispData[14] |= 1<<4;	
					FD616_Refresh();

				Delay_ms(up_down_delay_time);
				back_TIME_Flag=0;
				
}
#endif
void  ADC_Convert(void)
{
		unsigned int AD_Value;
		float VCC = 3.3;													 
	  float ADC_VALUE;																										 
		unsigned int B_value = 3950;
		

	
		ADCON |= AST(1);	
		while(!(ADCON & 0x20));
		ADCON |= ADIF;
		
		AD_Value = ADCDH*256 + ADCDL;
		AD_Value >>= 4;

		ADC_VALUE= AD_Value*VCC/4096;

		TEM = 1/(log(ADC_VALUE/(VCC - ADC_VALUE))/B_value + 1/(273.15 + 25));
		TEM=TEM- 273.15;
		TEM_F=TEM*1.8+32;

	
}



void alarm_sleep_judge(struct DATA_ALARM* alarm)
{
		UINT8 alarm_value1,alarm_value2,alarm_value3,alarm_value4,alarm_value5;
		if(time.second==0 && time.hour==alarm->hour && time.minute==alarm->minute&&hour_pm_flag==alarm->alarm_pm_flag)
		{
			alarm->alarm_flag=1;
			alarm_voice_flag=1;			
		}
		if(alarm->alarm_flag)
		{
			alarm_value1=alarm->minute+alarm_time;
			alarm_value2=alarm->minute+alarm_time+sleep_time;
			alarm_value3=alarm->minute+(alarm_time*2)+sleep_time;
			alarm_value4=alarm->minute+(alarm_time*2)+(sleep_time*2);
			alarm_value5=alarm->minute+(alarm_time*3)+(sleep_time*2);		
			if(alarm_voice_flag)
			{				
				if(alarm_clock_flag)
				{
					if(TR0)
					{TR0=0;buzzer=0;}
					else TR0=1;
					alarm_clock_flag=0;
				}								
			}
			if((alarm_value1)>=60)
			{
				if((alarm_value1-60)==time.minute)
				{
				 TR0=0;
				 buzzer=0;
				 alarm_voice_flag=0;
				}
			}
			else if((alarm_value1)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm_voice_flag=0;
			}
			if((alarm_value2)>=60)
			{
				if((alarm_value2-60)==time.minute)
				{
					alarm_voice_flag=1;
				}				
			}
			else if(alarm_value2==time.minute)
			{
				alarm_voice_flag=1;
			}
			if((alarm_value3)>=60)
			{
				if((alarm_value3-60)==time.minute)
				{
					TR0=0;
					buzzer=0;
					alarm_voice_flag=0;
				}				
			}
			else if((alarm_value3)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm_voice_flag=0;
			}
			if((alarm_value4)>=60)
			{
			  if((alarm_value4-60)==time.minute)
				{
					alarm_voice_flag=1;
				}				
			}
			else if((alarm_value4)==time.minute)
			{
				alarm_voice_flag=1;
			}
			if((alarm_value5)>=60)
			{
				if((alarm_value5-60)==time.minute)
				{
					TR0=0;
					buzzer=0;
					alarm->alarm_flag=0;
					alarm_voice_flag=0;
				}			
			}
			else if((alarm_value5)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm->alarm_flag=0;
				alarm_voice_flag=0;
			}			
		}

		
}
#if 0
void alarm1_sleep(void)
{
		UINT8 alarm1_value1,alarm1_value2,alarm1_value3,alarm1_value4,alarm1_value5;
		if(time.second==0 && time.hour==alarm1.hour && time.minute==alarm1.minute&&hour_pm_flag==alarm1_pm_flag)
		{
			alarm_flag=1;
			alarm_voice_flag=1;
		}
		if(alarm_flag)
		{
			alarm1_value1=alarm1.minute+alarm_time;
			alarm1_value2=alarm1.minute+alarm_time+sleep_time;
			alarm1_value3=alarm1.minute+(alarm_time*2)+sleep_time;
			alarm1_value4=alarm1.minute+(alarm_time*2)+(sleep_time*2);
			alarm1_value5=alarm1.minute+(alarm_time*3)+(sleep_time*2);
			if(alarm_voice_flag)
			{

				if(alarm_clock_flag)
				{
					if(TR0)
					{TR0=0;buzzer=0;}
					else TR0=1;
					alarm_clock_flag=0;
				}				
			
			}
			GPIO_Init(P04F,INPUT|PU_EN);
			if(vibration_key==0)
			{
				TR0=0;
				buzzer=0;
				alarm_voice_flag=0;				
			}
	if((alarm1_value1)>=60)
			{
				if((alarm1_value1-60)==time.minute)
				{
				 TR0=0;
				 buzzer=0;
				 alarm_voice_flag=0;
				}
			}
			else if((alarm1_value1)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm_voice_flag=0;
			}
			if((alarm1_value2)>=60)
			{
				if((alarm1_value2-60)==time.minute&&time.second==0)
				{
					alarm_voice_flag=1;
				}				
			}
			else if(alarm1_value2==time.minute&&time.second==0)
			{
				alarm_voice_flag=1;
			}
			if((alarm1_value3)>=60)
			{
				if((alarm1_value3-60)==time.minute)
				{
					TR0=0;
					buzzer=0;
					alarm_voice_flag=0;
				}				
			}
			else if((alarm1_value3)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm_voice_flag=0;
			}
			if((alarm1_value4)>=60)
			{
			  if((alarm1_value4-60)==time.minute&&time.second==0)
				{
					alarm_voice_flag=1;
				}				
			}
			else if((alarm1_value4)==time.minute&&time.second==0)
			{
				alarm_voice_flag=1;
			}
			if((alarm1_value5)>=60)
			{
				if((alarm1_value5-60)==time.minute)
				{
					TR0=0;
					buzzer=0;
					alarm_voice_flag=0;
				}			
			}
			else if((alarm1_value5)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm_voice_flag=0;
			}

		}
}

void alarm2_sleep(void)
{
	UINT8 alarm2_value1,alarm2_value2,alarm2_value3,alarm2_value4,alarm2_value5;
		if(time.second==0 && time.hour==alarm2.hour && time.minute==alarm2.minute&&hour_pm_flag==alarm2_pm_flag)
		{
			alarm2_flag=1;
			alarm2_voice_flag=1;
		}
		if(alarm2_flag)
		{
		alarm2_value1=alarm2.minute+alarm_time;
			alarm2_value2=alarm2.minute+alarm_time+sleep_time;
			alarm2_value3=alarm2.minute+(alarm_time*2)+sleep_time;
			alarm2_value4=alarm2.minute+(alarm_time*2)+(sleep_time*2);
			alarm2_value5=alarm2.minute+(alarm_time*3)+(sleep_time*2);			
			if(alarm2_voice_flag)
			{
				if(alarm_clock_flag)
				{
					if(TR0)
					{TR0=0;buzzer=0;}
					else TR0=1;
					alarm_clock_flag=0;
				}
			}
			GPIO_Init(P04F,INPUT|PU_EN);
			if(vibration_key==0)
			{
				TR0=0;
				buzzer=0;
				alarm2_voice_flag=0;				
			}			
	if((alarm2_value1)>=60)
			{
				if((alarm2_value1-60)==time.minute)
				{
					TR0=0;
					buzzer=0;
					alarm2_voice_flag=0;
				}			
			}
			else if((alarm2_value1)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm2_voice_flag=0;
			}
			if((alarm2_value2)>=60)
			{
				 if((alarm2_value2-60)==time.minute&&time.second==0)
				{
					alarm2_voice_flag=1;
				}
			}
			else if((alarm2_value2)==time.minute&&time.second==0)
			{
				alarm2_voice_flag=1;
			}
			if((alarm2_value3)>=60)
			{
				if((alarm2_value3-60)>=time.minute)
				{
					TR0=0;
					buzzer=0;
					alarm2_voice_flag=0;
				}
			}			
			else if((alarm2_value3)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm2_voice_flag=0;
			}
			if((alarm2_value4)>=60)
			{
				if((alarm2_value4-60)==time.minute&&time.second==0)
				alarm2_voice_flag=1;
			}			
			else if((alarm2_value4)==time.minute&&time.second==0)
			{
				alarm2_voice_flag=1;
			}
			if((alarm2_value5)>=60)
			{
			if((alarm2_value5-60)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm2_voice_flag=0;
			}				
			}
			else if((alarm2_value5)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm2_voice_flag=0;
			}

		}

}

void alarm3_sleep(void)
{
	UINT8 alarm3_value1,alarm3_value2,alarm3_value3,alarm3_value4,alarm3_value5;
		if(time.second==0 && time.hour==alarm3.hour && time.minute==alarm3.minute&&hour_pm_flag==alarm3_pm_flag)
		{
			alarm3_flag=1;
			alarm3_voice_flag=1;
		}
		if(alarm3_flag)
		{
				alarm3_value1=alarm3.minute+alarm_time;
			alarm3_value2=alarm3.minute+alarm_time+sleep_time;
			alarm3_value3=alarm3.minute+(alarm_time*2)+sleep_time;
			alarm3_value4=alarm3.minute+(alarm_time*2)+(sleep_time*2);
			alarm3_value5=alarm3.minute+(alarm_time*3)+(sleep_time*2);
			if(alarm3_voice_flag)
			{
				if(alarm_clock_flag)
				{
					if(TR0)
					{TR0=0;buzzer=0;}
					else TR0=1;
					alarm_clock_flag=0;
				}
			}
			GPIO_Init(P04F,INPUT|PU_EN);
			if(vibration_key==0)
			{
				TR0=0;
				buzzer=0;
				alarm3_voice_flag=0;				
			}			
			if((alarm3.minute+alarm_time)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm3_voice_flag=0;
			}
			if((alarm3_value1)>=60)
			{
				if((alarm3_value1-60)==time.minute)
				{
					TR0=0;
					buzzer=0;
					alarm3_voice_flag=0;
				}				
			}
			else if((alarm3_value1)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm3_voice_flag=0;
			}
			
			if((alarm3_value2)>=60)
			{
				if((alarm3_value2-60)==time.minute&&time.second==0)
				{
					alarm3_voice_flag=1;
				}				
			}
			else if((alarm3_value2)==time.minute&&time.second==0)
			{
				alarm3_voice_flag=1;
			}
			if((alarm3_value3)>=60)
			{
				if((alarm3_value3-60)==time.minute)
				{
					TR0=0;
					buzzer=0;
					alarm3_voice_flag=0;
				}				
			}
			else if((alarm3_value3)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm3_voice_flag=0;
			}
			if((alarm3_value4)>=60)
			{
				if((alarm3_value4-60)==time.minute&&time.second==0)
				{
					alarm3_voice_flag=1;
				}				
			}
			else if((alarm3_value4)==time.minute&&time.second==0)
			{
				alarm3_voice_flag=1;
			}
			if((alarm3_value5)>=60)
			{
				if((alarm3_value5-60)==time.minute)
				{
					TR0=0;
					buzzer=0;
					alarm3_voice_flag=0;
				}				
			}
			else if((alarm3_value5)==time.minute)
			{
				TR0=0;
				buzzer=0;
				alarm3_voice_flag=0;
			}

		}

}
#endif

void Week_display(UINT8 value)
{
	switch (value)
	{
		case 1:
		{

			anode_display(FD616_DIG1_ADDR,FD616_DISP_M_DATA);
			anode_display(FD616_DIG2_ADDR,FD616_DISP_O_DATA );
			anode_display(FD616_DIG3_ADDR,FD616_DISP_N_DATA );			
			break;
		}
		case 2:
		{
			anode_display(FD616_DIG1_ADDR,FD616_DISP_T_DATA);
			anode_display(FD616_DIG2_ADDR,FD616_DISP_U_DATA );
			anode_display(FD616_DIG3_ADDR,FD616_DISP_E_DATA );
			break;
		}
		case 3:
		{
			anode_display(FD616_DIG1_ADDR,FD616_DISP_W_DATA);
			anode_display(FD616_DIG2_ADDR,FD616_DISP_E_DATA );
			anode_display(FD616_DIG3_ADDR,FD616_DISP_D_DATA );
			
			break;
		}
		case 4:
		{
			anode_display(FD616_DIG1_ADDR,FD616_DISP_T_DATA);
			anode_display(FD616_DIG2_ADDR,FD616_DISP_H_DATA );
			anode_display(FD616_DIG3_ADDR,FD616_DISP_U_DATA );
			
			break;
		}
		case 5:
		{
			anode_display(FD616_DIG1_ADDR,FD616_DISP_F_DATA);
			anode_display(FD616_DIG2_ADDR,FD616_DISP_R_DATA );
			anode_display(FD616_DIG3_ADDR,FD616_DISP_I_DATA );
			
			break;
		}
		case 6:
		{
			anode_display(FD616_DIG1_ADDR,FD616_DISP_S_DATA);
			anode_display(FD616_DIG2_ADDR,FD616_DISP_A_DATA );
			anode_display(FD616_DIG3_ADDR,FD616_DISP_T_DATA );
			
			break;
		}
		case 7:
		{
			anode_display(FD616_DIG1_ADDR,FD616_DISP_S_DATA);
			anode_display(FD616_DIG2_ADDR,FD616_DISP_U_DATA );
			anode_display(FD616_DIG3_ADDR,FD616_DISP_N_DATA );
			
			break;
		}
		default:
			break;
		
	}
}

//���ڵ��ж�
void de_week(void) 
{	
	unsigned long  i,year2,j,day=0; 
	unsigned char   mweek,month,mday;
	unsigned char   year;
		
	ResetDS1302();
	WriteByteDS1302(year_r_addr); //����ĵ�ַ
	year=ReadByteDS1302();
	ResetDS1302();
	WriteByteDS1302(mon_r_addr); //���µĵ�ַ
	month=ReadByteDS1302();
	ResetDS1302();
	WriteByteDS1302(day_r_addr); //���յĵ�ַ
	mday=ReadByteDS1302();
	
	year = bcd_decimal(year);
	month = bcd_decimal(month);
	mday = bcd_decimal(mday);
	
	year2 = year+2000;
	 for(i= 2000;i<year2;i++)
	{
		  if(leep_year(i))day+=366;        
		  else day+=365; 	
	}
	 for(j=1;j<month;j++)
	{
		  if(j==1 || j==3 || j==5 || j==7 || j==8 || j==10 || j==12)
		  day+=31;        
		  if(j==4 || j==6 || j==9 || j==11)       
		  day+=30; 
		  if(flag_year_leap && j==2) day+=29;    
		  if((!flag_year_leap)&&j==2) day+=28; 	
	}
	 day+=mday;   
	 mweek=day%7;  
	 switch(mweek) 
	{
		 case 0:time.week=5;break;    
		 case 1:time.week=6;break;     
		 case 2:time.week=7;break;    
		 case 3:time.week=1;break;      
		 case 4:time.week=2;break;      
		 case 5:time.week=3;break;      
		 case 6:time.week=4;break; 	
	}
	ds1302_w_on();
		ResetDS1302();
		WriteByteDS1302(week_w_addr); //д�����ڵĵ�ַ
		WriteByteDS1302(time.week);
		ResetDS1302();
	ds1302_w_off();
	if(display_flag_week==1) 
	Week_display(time.week);
} 



void sd_mode (void)
{
	if(DOWN_Short_Flag)
	{
		back_voice_Flag=0;
//		up_down_flag=1;//up_down����ģʽѡ���ͻ
		if(SET_Long_Flag==0)
		{
			display_flag_year=0;
//			dislay_all_flag=1;
			sd_flag++;
			if(sd_select_first==1)
					{
						sd_flag--;
						sd_select_first=0;
					}
					if(sd_flag>1)sd_flag=0;			
				switch(sd_flag)
				{
					case 0:
					{
					  FD612_PotiveTNage(FD612_DIG1_ADDR,0x40, &FD612_base_DispBuff);
						FD612_PotiveTNage(FD612_DIG2_ADDR,0x40, &FD612_base_DispBuff);
						FD612_PotiveTNage(FD612_DIG3_ADDR,FD612_DISP_S, &FD612_base_DispBuff);	
						FD612_PotiveTNage(FD612_DIG4_ADDR,FD612_DISP_d, &FD612_base_DispBuff);
						FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);							
//						FD612_PotiveTNage(FD612_DIG7_ADDR,FD612_DISP_S, &FD612_base_DispBuff);	
//						FD612_PotiveTNage(FD612_DIG8_ADDR,FD612_DISP_d, &FD612_base_DispBuff);
//						FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
						
//						FD612_display_4DIG_input(0x40  ,0x40 ,FD612_DISP_S ,FD612_DISP_d  );
						sd_mode_flag=0;
						break;
			
					}
					case 1: 
					{
					  FD612_PotiveTNage(FD612_DIG1_ADDR,0x5c, &FD612_base_DispBuff);
						FD612_PotiveTNage(FD612_DIG2_ADDR,0x54, &FD612_base_DispBuff);
						FD612_PotiveTNage(FD612_DIG3_ADDR,FD612_DISP_S, &FD612_base_DispBuff);	
						FD612_PotiveTNage(FD612_DIG4_ADDR,FD612_DISP_d, &FD612_base_DispBuff);
						FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);							
//						FD612_display_4DIG_input(0x5c  ,0x54 ,FD612_DISP_S ,FD612_DISP_d );
						sd_mode_flag=1;
						break;
					}
						default :
						break;
					
				}
		}
//		power_delay_time=0;
		DOWN_Short_Flag=0;	
//		mode_select_first=1;
	if(back_first)
		{
			back_TIME_Flag=0;
			back_first=0;
		}		
	}
	if(back_first)
		{
			back_TIME_Flag=0;
			back_first=0;
		}
		if(back_TIME_Flag==over_time  ||SET_Short_Flag==1)
		{
//			power_delay_time=0;
			display_flag_year=1;
//			dislay_all_flag=0;
			back_TIME_Flag=0;
			back_first=1;	
			UP_Short_Flag=0;
			SET_Short_Flag=0;
//			mode_select_first=1;
			sd_select_first=1;
			up_down_flag=0;
		}	
	  if(sd_mode_flag)
		{
			voice_control();
		}
//		else
//		{	
//			bright_contrl();
//		  power_detect( );
//		}	
}


void voice_control(void)
{
//	if(SET_Long_Flag==0)
//	{
		if(voice_flag==0)
		{
			GPIO_Init(P03F,INPUT|PD_EN);
		if(voice_key==1)
			{
	GPIO_Init(P03F,INPUT|PD_EN);
				if(voice_key==1)
				{
						

					bright_contrl();
					if(FD616_DispBuff.DispState==FD616_OFF||FD612_base_DispBuff.DispState==FD612_DISP_OFF)
					{
					FD616_DispBuff.DispState=FD616_INTENS8;
					FD616_Command(FD616_DispBuff.DispState);
					FD612_base_DispBuff.DispState=FD612_DISP_ON|FD612_INTENS8;
					FD612_Command(&fd612_base_ops,FD612_base_DispBuff.DispState);	
					bright_contrl();						
					}

				}

			}

				else
				{
				  FD616_DispBuff.DispState=FD616_OFF;
					FD616_Command(FD616_DispBuff.DispState);
					FD612_base_DispBuff.DispState=FD612_DISP_OFF;
					FD612_Command(&fd612_base_ops,FD612_base_DispBuff.DispState);

				}			
		}
		if(FD612_base_DispBuff.DispState&0x08||FD616_DispBuff.DispState&0x01)//
				{
				
					voice_flag=1;
					if(voice_fisrt)
					{
//						voice_flag=1;
						back_voice_Flag=0;
						voice_fisrt=0;
					}

					if(back_voice_Flag==voice_wake_up_time)
					{
						back_voice_Flag=0;
						voice_flag=0;
						voice_fisrt=1;
						
						FD616_DispBuff.DispState=FD616_OFF;
						FD616_Command(FD616_DispBuff.DispState);
						FD612_base_DispBuff.DispState=FD612_DISP_OFF;
						FD612_Command(&fd612_base_ops,FD612_base_DispBuff.DispState);						
//						FD612_DispStateWr(FD612_DISP_OFF); 
//						FD612_Refresh();
						Delay_ms(150);
						Delay_ms(150);
				
					}
				}
//			}
}



void InitDS1302(void)
{		
			unsigned char year,month,mday,week,hour,minute,second ;
		  year = decimal_bcd(year_init); 
			month =decimal_bcd(month_init);
			mday = decimal_bcd(mday_init);
			week = decimal_bcd(week_init);
			hour = decimal_bcd(hour_init);
			minute = decimal_bcd(minute_init);
			second = decimal_bcd(second_init);
			
			hour |= 1<<7;
			hour &=~ (1<<5);
			hour_12_24_flag=1;
			hour_pm_flag=0;
			ds1302_w_on();
//	    ResetDS1302();
//			WriteByteDS1302(0x8e);		                      //д�������ƼĴ���
//			WriteByteDS1302(0x0);														//����д��
			
			ResetDS1302();
			WriteByteDS1302(0x90);													//д��������ƼĴ���
			WriteByteDS1302(0xab);													//������
			
			ResetDS1302();
			WriteByteDS1302(0xbe);													//д���ֽ�ͻ����ʽ���ƼĴ���
			
			WriteByteDS1302(second);
			WriteByteDS1302(minute);
			WriteByteDS1302(hour);
			WriteByteDS1302(mday);
			WriteByteDS1302(month);
			WriteByteDS1302(week);
			WriteByteDS1302(year);
			WriteByteDS1302(0);															//��д�������ƼĴ���д��0
			ResetDS1302();
			ds1302_w_off();
//			alarm1_on_dispaly=0;alarm2_on_dispaly=0;alarm3_on_dispaly=0;
			set_status_flag=0;SET_Long_Flag=0;
			display_flag_hur_min=1,display_flag_mon_day=1,display_flag_year=1,display_flag_week=1;			
			sd_mode_flag=0;
			sd_flag=0;
//			display_init( );
//			from_ds1302_read_time_data();
			
					
}


void bright_contrl(void)
{
		UINT8 hour;

		ResetDS1302();							
		WriteByteDS1302(hour_r_addr); //��ȡʱ�ĵ�ַ
		hour=ReadByteDS1302();
//		ResetDS1302();							
//		WriteByteDS1302(min_r_addr); //��ȡʱ�ĵ�ַ
//		minute=ReadByteDS1302();
	if(hour_12_24_flag)
	{
		hour<<=3;
		hour>>=3;
		hour = bcd_decimal( hour);
		if(hour_pm_flag)//afternoon
		{
			if( hour>=1&&hour<=6||hour==12)
			{
				  FD616_DispBuff.DispState=FD616_INTENS8;
					FD616_Command(FD616_DispBuff.DispState);
					FD612_base_DispBuff.DispState=FD612_DISP_ON|FD612_INTENS8;
					FD612_Command(&fd612_base_ops,FD612_base_DispBuff.DispState);
			}
			if(hour>=7&&hour<=11)
			{
				  FD616_DispBuff.DispState=FD616_INTENS3;
					FD616_Command(FD616_DispBuff.DispState);
					FD612_base_DispBuff.DispState=FD612_DISP_ON|FD612_INTENS3;
					FD612_Command(&fd612_base_ops,FD612_base_DispBuff.DispState);				
			}
		}
		else
		{
			 if( hour>=7&&hour<=11)
			{
				  FD616_DispBuff.DispState=FD616_INTENS8;
					FD616_Command(FD616_DispBuff.DispState);
					FD612_base_DispBuff.DispState=FD612_DISP_ON|FD612_INTENS8;
					FD612_Command(&fd612_base_ops,FD612_base_DispBuff.DispState);
			}
			if(hour>=1&&hour<=6||hour==12)
			{
				  FD616_DispBuff.DispState=FD616_INTENS3;
					FD616_Command(FD616_DispBuff.DispState);
					FD612_base_DispBuff.DispState=FD612_DISP_ON|FD612_INTENS3;
					FD612_Command(&fd612_base_ops,FD612_base_DispBuff.DispState);				
			}			
		}
	}
	else
	{		
		 hour = bcd_decimal( hour);
			 if( hour>=7&&hour<19)
			{
				  FD616_DispBuff.DispState=FD616_INTENS8;
					FD616_Command(FD616_DispBuff.DispState);
					FD612_base_DispBuff.DispState=FD612_DISP_ON|FD612_INTENS8;
					FD612_Command(&fd612_base_ops,FD612_base_DispBuff.DispState);
			}
			else
			{
				  FD616_DispBuff.DispState=FD616_INTENS3;
					FD616_Command(FD616_DispBuff.DispState);
					FD612_base_DispBuff.DispState=FD612_DISP_ON|FD612_INTENS3;
					FD612_Command(&fd612_base_ops,FD612_base_DispBuff.DispState);
			}

	}

}


void display_init(void)
{
	
	UINT8 i,loop;
	Delay_ms(1000);
	for(i=0;i<10;i++)
	{

		 FD612_PotiveTNage(FD612_DIG1_ADDR,dis[i], &FD612_base_DispBuff);
	 	 FD612_PotiveTNage(FD612_DIG2_ADDR,dis[i], &FD612_base_DispBuff);	
		 FD612_PotiveTNage(FD612_DIG3_ADDR,dis[i], &FD612_base_DispBuff);	
		 FD612_PotiveTNage(FD612_DIG4_ADDR,dis[i], &FD612_base_DispBuff);
		 FD612_PotiveTNage(FD612_DIG5_ADDR,dis[i], &FD612_base_DispBuff);
		 FD612_PotiveTNage(FD612_DIG6_ADDR,dis[i], &FD612_base_DispBuff);	
		 FD612_PotiveTNage(FD612_DIG7_ADDR,dis[i], &FD612_base_DispBuff);	
		 FD612_PotiveTNage(FD612_DIG8_ADDR,dis[i], &FD612_base_DispBuff);
		 FD612_PotiveTNage(FD612_DIG9_ADDR,dis[i], &FD612_base_DispBuff);
		 FD612_PotiveTNage(FD612_DIG10_ADDR,dis[i], &FD612_base_DispBuff);	
		 FD612_PotiveTNage(FD612_DIG11_ADDR,dis[i], &FD612_base_DispBuff);	
		 FD612_PotiveTNage(FD612_DIG12_ADDR,dis[i], &FD612_base_DispBuff);		

		 anode_display(FD616_DIG4_ADDR,dis[i]);
		 anode_display(FD616_DIG5_ADDR,dis[i]);
		 anode_display(FD616_DIG6_ADDR,dis[i]);
		 anode_display(FD616_DIG7_ADDR,dis[i]);
			FD616_Refresh();			
		 FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);	
		Delay_ms(1000);
	}
		for(loop = 0;loop < 12;loop++){
		FD612_base_DispBuff.DispData[loop] = 0X00;
		}
		 for(i=0;i<16;i++)
		{
			FD616_DispBuff.DispData[i]=0X00;
		}
		FD616_Refresh();
		FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);	
	for(i=1;i<8;i++)
	{
		Week_display(i);
		FD616_Refresh();
		Delay_ms(1000);
	}
		  TR0=1;
		Delay_ms(1000);
		TR0=0;
		buzzer=0;
	
}

void power_detect(void)
{

	GPIO_Init(P01F,INPUT|PD_EN);
	if(power_key==0)
	{
		if(power_first==0)
		{
			power_delay_time=0;
			power_first=1;
		}
		
		if(power_delay_time>30)
		{
			sd_mode_flag=1;
			sd_flag=1;
			voice_flag=0;//���ؼ�ؿ���1����
			back_voice_Flag=0;
			power_delay_time=0;
			power_first=0;
		}
	}
	else
	{
		power_delay_time=0;
	}

}


	void hour_chage_input(UINT8 i)
	{//24Сʱת12Сʱ
		UINT8 hour_state_value;
		ResetDS1302();							
		WriteByteDS1302(hour_r_addr); //��ȡʱ�ĵ�ַ
		hour_state_value=ReadByteDS1302();
		if(i==1)		
		{

						
			hour_state_value = bcd_decimal(hour_state_value);				
		if(hour_state_value>12)
		{
			hour_state_value=hour_state_value-12;
			hour_state_value=decimal_bcd(hour_state_value);
			hour_state_value |=(1<<5);
			hour_state_value |= (1<<7);
			hour_pm_flag=1;
		}
		else if(hour_state_value==12)
		{
		 hour_pm_flag=1;
			hour_state_value=decimal_bcd(hour_state_value);
			hour_state_value |=(1<<5);
			hour_state_value |= (1<<7);
		}
			else if(hour_state_value<12)
			{
			hour_state_value=decimal_bcd(hour_state_value);
			hour_state_value |= (1<<7);
			}

				
		}
		if(i==0)		
		{
			//12Сʱת24Сʱ
			if(hour_state_value&0x20)
			{
				hour_state_value&=~(1<<7);
				hour_state_value &=~(1<<5);				
				hour_pm_flag=0;
				hour_state_value = bcd_decimal(hour_state_value);		
				if(hour_state_value!=12)
				hour_state_value=hour_state_value+12;
				hour_state_value=decimal_bcd(hour_state_value);
			}
			else
			{
			hour_state_value&=~(1<<7);	
			hour_state_value = bcd_decimal(hour_state_value);
			if(hour_state_value==12)hour_state_value=0;
			hour_state_value=decimal_bcd(hour_state_value);
			
			}
			
			
		}
		ds1302_w_on();
		ResetDS1302();
		WriteByteDS1302(hour_w_addr); //д��ʱ�ĵ�ַ
		WriteByteDS1302(hour_state_value);
		ds1302_w_off();		
	}
	
	
	void hour_min_display(void)
	{
			anode_display(FD616_DIG4_ADDR,dis[time.hour/10]);
			anode_display(FD616_DIG5_ADDR,dis[time.hour%10]);
			anode_display(FD616_DIG6_ADDR,dis[time.minute/10]);
			anode_display(FD616_DIG7_ADDR,dis[time.minute%10]);
		
			if(HalfSecFlag)
			{
				if(blink_flag)		
				{
					blink_flag=0;														
					FD616_DispBuff.DispData[14] &= ~(1<<4);
								
				}
				else 
				{
						blink_flag=1;	
						FD616_DispBuff.DispData[14] |= 1<<4;
																	
				}
				if(hour_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
				if(alarm1.alarm_on_dispaly||alarm2.alarm_on_dispaly||alarm3.alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
				FD616_Refresh();				
				HalfSecFlag=0;	
			}	
	}
	
void from_ds1302_read_time_data(void)
{
	//				UINT8	pm_flag;
			ResetDS1302();	
			WriteByteDS1302(0xbf);																					//�Զ��ֽڷ�ʽ��ȡʱ�ӼĴ�������
			time.second=ReadByteDS1302();
			time.minute=ReadByteDS1302();
			time.hour=ReadByteDS1302();
			time.mday=ReadByteDS1302();
			time.month=ReadByteDS1302();
			time.week=ReadByteDS1302();
			time.year=ReadByteDS1302();
			ResetDS1302();
			if(hour_12_24_flag==1)
			{
			 if((time.hour&0x20)&&(time.hour&0x80))
				 { 
					 hour_pm_flag=1;
				 }
				 else 
				 {								 
					 hour_pm_flag=0;
				 }
				time.hour<<=3;
				time.hour>>=3;
			
			}
			time.year = bcd_decimal(time.year); 
			time.month =bcd_decimal(time.month);
			time.mday = bcd_decimal(time.mday);
			time.week = bcd_decimal(time.week);
			time.hour = bcd_decimal(time.hour);
			time.minute = bcd_decimal(time.minute);
			time.second = bcd_decimal(time.second);
}



void mon_over_judge(void)
{
	if(time.month >=13)time.month=1;
	if(time.month ==0)time.month=12;
	if(time.month==4||time.month==6||time.month==9||time.month==11) 
	{if(time.mday==31)time.mday=1;time.mday = decimal_bcd(time.mday);Write_addr_byte_ds1302(day_w_addr,time.mday);}
	if(time.month==2) 
	{if(time.mday>=30)time.mday=1;time.mday = decimal_bcd(time.mday);Write_addr_byte_ds1302(day_w_addr,time.mday);}
}

void mday_over_judge(void)
{
			unsigned long year1;
			year1 = 2000+time.year;	
			flag_year_leap=leep_year(year1);//???????  
//			if(time.mday>=30)   
//		 {          
			if(time.month==1||time.month==3||time.month==5||time.month==7||time.month==8||time.month==10||time.month==12)  //?????31????£?1??3??5??7??8??10??12      
				 {     
						if(time.mday==32)  time.mday=1;
					 if(time.mday==0)    time.mday=31;
					 
				}          
			if(time.month==4||time.month==6||time.month==9||time.month==11)  
					{     
					 if(time.mday==31)   time.mday=1;
						if(time.mday==0)   time.mday=30;	      
					 }         
//			}     
			 if(time.month==2)    
				{      
					if(flag_year_leap&&(time.mday==30))     							 time.mday=1;
					if(flag_year_leap&&(time.mday==0))     							 time.mday=29;
					if((!flag_year_leap)&&(time.mday==29))  							 time.mday=1;
					if((!flag_year_leap)&&(time.mday==0))  							 time.mday=28;
				}
}


void alarm_12switch24(struct  DATA_ALARM* alarm)
{
	if(alarm->alarm_pm_flag)
	{
		if(alarm->hour!=12)alarm->hour+=12;
		alarm->alarm_pm_flag=0;
	}
	else if(alarm->hour==12)alarm->hour=0;
}

void alarm_24switch12(struct  DATA_ALARM* alarm)
{
	if(alarm->hour>12)
	{
		alarm->hour-=12;
		alarm->alarm_pm_flag=1;
	}
	if(alarm->hour==12)alarm->alarm_pm_flag=1;
	if(alarm->hour==0){alarm->hour=12;alarm->alarm_pm_flag=0;}
}

void hour_over_judge(UINT8 add_redue_flag)
{
		if(hour_12_24_flag)
		{

			switch(add_redue_flag)
			{
				case 0:
				{
					if(time.hour==12)	after_arrive_write_1302();
					if(time.hour>=13)							 time.hour = 1;
					break;
				}
				case 1:
				{
					if(time.hour==11)	after_arrive_write_1302();
					if(time.hour==0)							 time.hour = 12;
					break;
				}				
			}			
		}
		else
		{
			switch(add_redue_flag)
			{
				case 0:
				{
					if(time.hour >=24) time.hour = 0;
					break;
				}
				case 1:
				{
					if(time.hour ==255) time.hour = 23;
					break;
				}				
			}			

		}

			time.hour = decimal_bcd(time.hour);
			if(hour_12_24_flag)
			{
				time.hour |=1<<7;
				if(hour_pm_flag)time.hour |=1<<5;
				else time.hour &=~(1<<5);
			}
			else time.hour &=~(1<<7);
			Write_addr_byte_ds1302(hour_w_addr,time.hour);	
				
			
}
void min_over_judge(UINT8 add_redue_flag)
{
	switch(add_redue_flag)
	{
		case 0:
		{
			if(time.minute >=60)time.minute = 0;
			break;
		}
		case 1:
		{
			if(time.minute==255)time.minute=59;
			break;
		}		
	}
	Write_addr_byte_ds1302(sec_w_addr,0);
	time.minute = decimal_bcd(time.minute);										
	Write_addr_byte_ds1302(min_w_addr,time.minute);
}

void alarm_on_off_set(UINT8 alarm_num,struct  DATA_ALARM* alarm )
{
		anode_display(FD616_DIG1_ADDR,FD616_DISP_A_DATA);
		anode_display(FD616_DIG2_ADDR,FD616_DISP_L_DATA );
		if(alarm_num==1)anode_display(FD616_DIG3_ADDR,dis[alarm_num] );
		else anode_display(FD616_DIG3_ADDR,dis[alarm_num]|DISP_BIT_G2 );
		if(time1_second==1)
		{		
			if(blink2_flag)
			{
				if(alarm->alarm_on_dispaly==0)
			{

			
				anode_display(FD616_DIG4_ADDR,0x00);
				anode_display(FD616_DIG5_ADDR,0x5c);	
				anode_display(FD616_DIG6_ADDR,FD612_DISP_F);
				anode_display(FD616_DIG7_ADDR,FD612_DISP_F);										
			
			}
			else
			{
				anode_display(FD616_DIG4_ADDR,0x00);
				anode_display(FD616_DIG5_ADDR,0x5c);	
				anode_display(FD616_DIG6_ADDR,0x54);
				anode_display(FD616_DIG7_ADDR,0x00);																					
			}
				
				blink2_flag=0;
				
	//										FD616_Refresh();

			}
			else
			{
				anode_display(FD616_DIG4_ADDR,0x00);
				anode_display(FD616_DIG5_ADDR,0x00);	
				anode_display(FD616_DIG6_ADDR,0x00);
				anode_display(FD616_DIG7_ADDR,0x00);
				blink2_flag=1;

			}

		time1_second=0;
		}
		if(alarm->alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
		FD616_Refresh();	
		 if(UP_Short_Flag==1)
		{
			if(alarm->alarm_on_dispaly)alarm->alarm_on_dispaly=0;
			else alarm->alarm_on_dispaly=1;									
			UP_Short_Flag=0;
		}		
		
}

void alarm_hour_blink(UINT8 blink_enable,struct  DATA_ALARM* alarm)
{
	anode_display(FD616_DIG6_ADDR,dis[alarm->minute/10]);
  anode_display(FD616_DIG7_ADDR,dis[alarm->minute%10]);
	if(time1_second==1)
	{
			if(blink2_flag)
			{
				anode_display(FD616_DIG4_ADDR,dis[alarm->hour/10]);
				anode_display(FD616_DIG5_ADDR,dis[alarm->hour%10]);			
				if(blink_enable==0)blink2_flag=0;


			}
			else
			{
					blink2_flag=1;				
					anode_display(FD616_DIG4_ADDR,0x00);
					anode_display(FD616_DIG5_ADDR,0x00);				
			}
			if(alarm->alarm_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
			if(alarm->alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
			FD616_DispBuff.DispData[14] |= 1<<4;	
			FD616_Refresh();				
			time1_second=0;
	}	
}
void alarm_hour_set(struct  DATA_ALARM* alarm)
{
	alarm_hour_blink(0,alarm);
	if(UP_Short_Flag==1)
	{
		alarm->hour++;
		alarm_hour_up_down(0,0,alarm);
		
		UP_Short_Flag=0;
	}
	if(DOWN_Short_Flag == 1)
	{
		DOWN_Short_Flag =0;
		alarm->hour--;
		alarm_hour_up_down(0,1,alarm);
	}
	
}

void alarm_hour_up_down(UINT8 long_key_enble,UINT8 up_down_seclect,struct DATA_ALARM * alarm)
{
	if(up_down_seclect==0)
	{
		if(long_key_enble)alarm->hour++;
		if(hour_12_24_flag)
		{
			if(alarm->hour>=13)alarm->hour = 1;
			if(alarm->hour==12)
			{
				if(alarm->alarm_pm_flag)alarm->alarm_pm_flag=0;
				else alarm->alarm_pm_flag=1;
			}
		}
		else
		{
			if(alarm->hour >=24)			alarm->hour = 0;
		}
	}
	if(up_down_seclect==1)
	{
		if(long_key_enble)alarm->hour--;
		if(hour_12_24_flag)
		{
			if(alarm->hour==0)alarm->hour = 12;
			if(alarm->hour==11)
			{
				if(alarm->alarm_pm_flag)alarm->alarm_pm_flag=0;
				else alarm->alarm_pm_flag=1;
			}			
		}
		else
		{
			if(alarm->hour >=255)			alarm->hour = 23;
		}
	}
	 //FD612_DISPLAY_2DIG_input(1 , dis[alarm->hour/10],dis[alarm->hour%10] );
//	FD612_DISPLAY_2DIG_input(2,0x00 ,dis[alarm->alarm_pm_flag] );
	 if(long_key_enble)
	 {
		 		anode_display(FD616_DIG4_ADDR,dis[alarm->hour/10]);
				anode_display(FD616_DIG5_ADDR,dis[alarm->hour%10]);
		 		if(alarm->alarm_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
				if(alarm->alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
				FD616_DispBuff.DispData[14] |= 1<<4;	
				FD616_Refresh();								
				back_TIME_Flag=0;
		    Delay_ms(up_down_delay_time);
	 }
}


void alarm_min_blink(UINT8 blink_enable,struct  DATA_ALARM* alarm)
{
		anode_display(FD616_DIG4_ADDR,dis[alarm->hour/10]);
		anode_display(FD616_DIG5_ADDR,dis[alarm->hour%10]);					
		if(time1_second==1)
		{
			if(blink2_flag)
				{
					if(blink_enable==0)blink2_flag=0;								
					anode_display(FD616_DIG6_ADDR,dis[alarm->minute/10]);
					anode_display(FD616_DIG7_ADDR,dis[alarm->minute%10]);																													
				}
			else
			 {
					blink2_flag=1;								
					anode_display(FD616_DIG6_ADDR,0x00);
					anode_display(FD616_DIG7_ADDR,0x00);
																	
			 }
		
			if(alarm->alarm_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
			if(alarm->alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
			FD616_DispBuff.DispData[14] |= 1<<4;	
			FD616_Refresh();
			time1_second=0;

	 }
}
void alarm_min_set(struct  DATA_ALARM* alarm)
{
			alarm_min_blink(0, alarm);
	 		if(UP_Short_Flag == 1)
			{
				alarm->minute++;
				alarm_min_up_down(0,0, alarm);
				UP_Short_Flag =0;
			}
			if(DOWN_Short_Flag == 1)
			{
				alarm->minute--;
				alarm_min_up_down(0,1, alarm);
				DOWN_Short_Flag=0;
			}
}
void alarm_min_up_down(UINT8 long_key_enble,UINT8 up_down_seclect,struct DATA_ALARM* alarm)
{
	if(up_down_seclect==0)
	{
		if(long_key_enble)alarm->minute++;
		if(alarm->minute >=60)alarm->minute = 0;
	}
	else
	{
		if(long_key_enble)alarm->minute--;
		if(alarm->minute ==255)alarm->minute = 59;
	}
//	FD612_DISPLAY_2DIG_input(2,dis[alarm->minute/10]  ,dis[alarm->minute%10] );
	
	if(long_key_enble)
	 {
		 		anode_display(FD616_DIG6_ADDR,dis[alarm->minute/10]);
				anode_display(FD616_DIG7_ADDR,dis[alarm->minute%10]);
		 		if(alarm->alarm_pm_flag)	FD616_DispBuff.DispData[14] |= 1<<3;
				if(alarm->alarm_on_dispaly)				FD616_DispBuff.DispData[14] |= 1<<6;
				FD616_DispBuff.DispData[14] |= 1<<4;	
				FD616_Refresh();								
				back_TIME_Flag=0;
		    Delay_ms(up_down_delay_time);
	 }
}


void key_off_alarm_clock(struct  DATA_ALARM* alarm)
{
	if(alarm->alarm_flag==1)
{
	if(SET==0||UP==0||DOWN==0)
	{
		alarm->alarm_flag=0;
		buzzer=0;
		TR0=0;
	  alarm_voice_flag=0;		
	}
}
}