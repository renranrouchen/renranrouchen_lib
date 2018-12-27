
#include "usr.h"


struct DATA_ALARM alarm1,alarm2,alarm3;//闹钟初始值
struct DATA_ALARM_ch ch_alarm1,ch_alarm2,ch_alarm3;
struct DATA_TIME time;									//时间初始值
struct time_char time_ch;								//时间显示初始值



unsigned char second,time1_second=0,long_flag;
unsigned int delaytime =40,dc2=0,delaytime1=10,dc1=0; //20次1S
unsigned char SET_Long_Flag=0,SET_Short_Flag=0,UP_Long_Flag=0,UP_Short_Flag=0,DOWN_Short_Flag=0,DOWN_Long_Flag=0;
unsigned char flag_display=0,display_flag_hur_min=1,SET_Short_Flag_hur_min=0;
const unsigned char  dis[10] = {FD612_DISP_0,FD612_DISP_1,FD612_DISP_2,FD612_DISP_3,FD612_DISP_4,FD612_DISP_5,FD612_DISP_6,FD612_DISP_7,FD612_DISP_8,FD612_DISP_9 };
unsigned char  TEM_VALUE =1;
unsigned char  ADC_TIME_Flag=0,dc_ADC=0,delaytime_ADC=20,ADC_fisrt=1;//20次1S
unsigned char display_flag_mon_day=1,SET_Short_Flag_mon_day=0,SET_Short_Flag_alarm1_flag=0;
unsigned char  flag_year_leap=0;//闰年的代表
unsigned char alarm_on_flag=0,alarm1_on_dispaly=0,alarm2_on_dispaly=0,alarm3_on_dispaly=0;
unsigned char  back_TIME_Flag=0,dc_back=0,delaytime_back=20,back_first=1,voice_back=0,back_voice_Flag=0,alarm_clock_flag=0,dc_alarm=0;
unsigned char alarm_flag=0,alarm_voice_flag=0,alarm3_voice_flag=0,alarm2_voice_flag=0,alarm3_flag=0,alarm2_flag=0;
unsigned char voice_flag=0,voice_fisrt=1;
unsigned char sd_flag=0,dislay_all_flag=0,sd_mode_flag=0,sd_select_first=1;
float   TEM2,TEM4;
double	 TEM;				//浮点类型
unsigned char  ADC_Display=0,ADC_Display1=0,ADC_ONE=1;
unsigned char key_first=1;
unsigned char mode_select_first=1,mode_select_flag=0,hor_min_delay_time=0,mon_day_delay_time=0,mode_select=0,temp_delay_time=0;
UINT8 long_add_flag=0,up_down_flag=0;
UINT8 alarm_hour_long_up_down_flag=0,alarm_min_long_up_down_flag=0;
UINT8 blink_flag=0,blink3_flag=0;
UINT8 hour_chage_state=0,hour_12_24_flag=0,hour_pm_flag=0,alarm1_pm_flag=0,alarm2_pm_flag=0,alarm3_pm_flag=0;
void	mTimer0Interrupt( void ) interrupt INT_NO_TMR0 using 1                //timer0中断服务程序,使用寄存器组1
{                                                                           //方式3时，TH0使用Timer1的中断资源

				TH0=0XFf;
				TL0=0X90;
				buzzer=~buzzer;
//			dc2++;
//		if(dc2==delaytime){
//			second=1;	
//			dc2=0;
//		}


}

void	mTimer1Interrupt( void ) interrupt INT_NO_TMR1 using 2                //timer1中断服务程序,使用寄存器组2
{                                                                           //方式3时，Timer1停止
    TH1 =0X3C;
		TL1 =0XB0; 
			dc2++;
		if(dc2==delaytime){
			second=1;	
			dc2=0;
		}  
		dc1++;
		if(dc1==delaytime1)
		{
			time1_second=1;
			dc1=0;
		}
		dc_ADC++;
		if(dc_ADC==delaytime_ADC)
		{
			ADC_TIME_Flag++;
			dc_ADC=0;
		}
		dc_back++;
		if(dc_back==delaytime_back)
		{
			back_TIME_Flag++;
			dc_back=0;
		}
		voice_back++;
		if(voice_back==delaytime_back)
		{
			back_voice_Flag++;
			voice_back=0;
		}
		dc_alarm++;
		if(dc_alarm==delaytime1)
		{
			alarm_clock_flag=1;
			dc_alarm=0;
			
		}
		mode_select++;
		if(mode_select==delaytime_back)
		{
			if(flag_display==1)
			hor_min_delay_time++;
			if(flag_display==0)
				mon_day_delay_time++;
			if(flag_display==5)
			temp_delay_time++;
				mode_select=0;
			
		}
}


/**
 *  @brief  BCD转十进制
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
 *  @brief  十进制转BCD
 *  @param  decimal
 *  @return bcd
 *  @note    
 *  @note    
 */
UINT8 decimal_bcd(UINT8 decimal)
{
return  (decimal+(decimal / 10)*6);
}


/**
 *  @brief  判断是否是闰年
 *  @param  year
 *  @return 1代表闰年 0代表不是闰年
 *  @note    
 *  @note    
 */


void alarm_time_init(void)
{
		alarm1.hour  =0;
		alarm1.minute=0;
		alarm2.hour  =0;
		alarm2.minute=0;
		alarm3.hour  =0;
		alarm3.minute=0;
		//buzzer=0;
}

void time0_setup(void)
{
		long_flag=0;
		back_first=1;
		delaytime =40;
		second=0;
		dc2=0;

//		TH0 =0X3C;
//		TL0 =0XB0;
//		TR0=1;
		back_TIME_Flag=0;
	if(mode_select_flag==0)
	{
		temp_delay_time=0; //DP1计数清0
		mon_day_delay_time=0;
		hor_min_delay_time=0;	
	}
	  TR0=1;
		mDelaymS(70);
		TR0=0;
		buzzer=0;

		back_voice_Flag=0; //唤醒功能延时重新计数
			
}

void key_statue(void)
{
		from_ds1302_read_time_data( );
				
if(alarm_flag==0 && alarm2_flag==0&& alarm3_flag==0)	{
//set按键
	if(SET==0)
	{
		time0_setup( );
		while(!SET)
		{
	//		pm_12_24_alarm_display();
			/*SET按下时秒还在跳*/
			if(flag_display==0&&SET_Long_Flag==0)
			{
//					if(time1_second==1)
//					{
//					if(blink3_flag==1)
//						{		
//							blink3_flag=0;
//							FD612_DispBuff.DispData[FD612_DIG12_ADDR] &= ~(1<<1);
//							FD612_Refresh();	
//						}
//						else
//						{
//							blink3_flag=1;
//							FD612_DispBuff.DispData[FD612_DIG12_ADDR] |= 1<<1;
//							FD612_Refresh();	
//						}
//						time1_second=0;
//					}
				point_blink();
			}
			

			if(second==1)
			{
				
				long_flag=1;
				SET_Long_Flag=1;
				switch(flag_display)
				{
					case 0:
					{
						display_12_24_blink( );
						break;
					}
				
					case 1:
					{
							year_blink(0)	;					
							break;
					}
					case 2:
					{
            alarm_long_blink(alarm1)	;				
						break;
					}
					case 3:
					{
						alarm_long_blink(alarm2)	;
						break;
					}
					case 4:
					{
						alarm_long_blink(alarm3)	;
						break;
					}					
				}				
				
				
			}
		}
		second=0;
		while(!SET);
		if(long_flag==0)
		{
			SET_Short_Flag=1;
			second=0;
		}
	}
//up按键
	if(UP==0)
	{
		
		time0_setup( );
		while(!UP)
		{
		//	pm_12_24_alarm_display();
			if(SET_Long_Flag==1)
			{
			 key_dislpay_up_down( );
			}
			if(second==1)
			{
				
				long_flag=1;
				UP_Long_Flag=1;
				long_add_flag=1;//长按时显示
				
				if(SET_Long_Flag==1)
				{
					back_TIME_Flag=0;
					if(mode_select_flag==0)
					{
						temp_delay_time=0; //DP1计数清0
						mon_day_delay_time=0;
						hor_min_delay_time=0;	
					}
					switch (flag_display)
					{
					case 0:
					{
					   if(SET_Short_Flag_hur_min==1)
						 {
							 hour_long_up_down(0);
						 }
					   if(SET_Short_Flag_hur_min==2)
						 {

             min_long_up_down(0) ;
						 }						 
						break;
					}
					case 1:
					{
					   if(SET_Short_Flag_mon_day==0)
						 {
							year_long_up_down(0);
						 }
					   if(SET_Short_Flag_mon_day==1)
						 {
							mon_long_up_down(0);
						 }	
					   if(SET_Short_Flag_mon_day==2)
						 {
							mday_long_up_down(0);
						 }						 
						break;
					}
					case 2:
					{
							if(SET_Short_Flag_alarm1_flag==0)	
							{
								 alarm_hour_long_up_down(1,1,alarm1);

							}		
							if(SET_Short_Flag_alarm1_flag==1)	
							{
								 alarm_min_long_up_down(1,1,alarm1);
							}						
						break;
					}
					case 3:
					{
						if(SET_Short_Flag_alarm1_flag==0)	
						{
							 alarm_hour_long_up_down(2,1, alarm2);
						}
						if(SET_Short_Flag_alarm1_flag==1)	
						{
							 alarm_min_long_up_down(2,1,alarm2);
						}							
						break;
					}
					case 4:
					{
						if(SET_Short_Flag_alarm1_flag==0)	
						{
							 alarm_hour_long_up_down(3,1, alarm3);
						}
						if(SET_Short_Flag_alarm1_flag==1)	
						{
							 alarm_min_long_up_down(3,1, alarm3);
						}						
						break;
					}		
				}

							
				}
			}
	
		}

/*用于ALARM自加自减时多加减一 */	
		switch(alarm_hour_long_up_down_flag)
		{
			case 1:
			{
				alarm1.hour--;
			 if(hour_12_24_flag)
				{
					if(alarm1.hour==0)alarm1.hour=12;
				}
				else 
				{
					if(alarm1.hour>=24)alarm1.hour=23;
				}
				alarm_hour_long_up_down_flag=0;	
				alarm_hour_long_up_down_flag=0;				
				break;
			}
			case 2:
			{
				alarm2.hour--;
				if(hour_12_24_flag)
				{
					if(alarm2.hour==0)alarm2.hour=12;
				}
				else 
				{
					if(alarm2.hour>=24)alarm2.hour=23;
				}					
				alarm_hour_long_up_down_flag=0;				
				break;
			}			
			case 3:
			{
				alarm3.hour--;
			   if(hour_12_24_flag)
				{
					if(alarm3.hour==0)alarm3.hour=12;
				}
				else 
				{
					if(alarm3.hour>=24)alarm3.hour=23;
				}				
				alarm_hour_long_up_down_flag=0;				
				break;
			}			
		
			default :
			break;			
		}
		switch(alarm_min_long_up_down_flag)
		{
			case 1:
			{
				alarm1.minute--;
				if(alarm1.minute>=60)alarm1.minute=59;
				alarm_min_long_up_down_flag=0;				
				break;
			}
			case 2:
			{
				alarm2.minute--;
				if(alarm2.minute>=60)alarm2.minute=59;
				alarm_min_long_up_down_flag=0;				
				break;
			}			
			case 3:
			{
				alarm3.minute--;
				if(alarm3.minute>=60)alarm3.minute=59;
				alarm_min_long_up_down_flag=0;				
				break;
			}			
			
			default :
			break;			
		}
		long_add_flag=0;
		second=0;
		while(!UP);
		if(long_flag==0)
		{
			UP_Short_Flag=1;
			second=0;
		}
	}

//down按键
	if(DOWN==0)
	{

		time0_setup( );
		while(!DOWN)
		{
		//	pm_12_24_alarm_display();
			if(SET_Long_Flag==1)
			{
			 key_dislpay_up_down( );
			}
			if(second==1)
			{
					back_TIME_Flag=0;
					if(mode_select_flag==0)
					{
						temp_delay_time=0; //DP1计数清0
						mon_day_delay_time=0;
						hor_min_delay_time=0;	
					}
					
				long_add_flag=1;
				
				long_flag=1;
				DOWN_Long_Flag=1;
				if(SET_Long_Flag==1)
				{
					switch (flag_display)
					{
						case 0:
						{
					   if(SET_Short_Flag_hur_min==1)
						 {
               hour_long_up_down(1);
						 }	
						  if(SET_Short_Flag_hur_min==2)
						 {
              min_long_up_down(1) ;
						 }						
							break;
						}
						case 1:
						{
					   if(SET_Short_Flag_mon_day==0)
						 {
							year_long_up_down(1);
						 }
					   if(SET_Short_Flag_mon_day==1)
						 {
							mon_long_up_down(1);
						 }
					   if(SET_Short_Flag_mon_day==2)
						 {
							mday_long_up_down(1);
						 }						 
						
							break;
						}
						case 2:
						{
							if(SET_Short_Flag_alarm1_flag==0)	
							{
								 alarm_hour_long_up_down(1,2,  alarm1);
								
							}
							if(SET_Short_Flag_alarm1_flag==1)	
							{
								 alarm_min_long_up_down(1,2,alarm1);
							}								
							break;
						}
						case 3:
						{
							if(SET_Short_Flag_alarm1_flag==0)	
							{
								 alarm_hour_long_up_down(2,2,alarm2);
							}
							if(SET_Short_Flag_alarm1_flag==1)	
							{
								 alarm_min_long_up_down(2,2, alarm2);
							}							
							break;
						}
						case 4:
						{
							if(SET_Short_Flag_alarm1_flag==0)	
							{
								 alarm_hour_long_up_down(3,2, alarm3);
							}
							if(SET_Short_Flag_alarm1_flag==1)	
							{
								 alarm_min_long_up_down(3,2, alarm3);
							}												
							break;
						}
						default :
						break;
												
					}

						
				}	
				if(flag_display==5&&ADC_ONE==1)
				{
					if(ADC_Display==0)ADC_Display=1;
					else ADC_Display=0;
					  adc_switch( );
//					switch (ADC_Display)
//					 {
//						 
//						 case 0:
//						 {
//								
//							 
//							 //FD612_display_4DIG_input(0x00,dis[(unsigned char)TEM2/10],dis[(unsigned char)TEM2%10],FD612_DISP_C);	 
//								FD612_PotiveTNage(FD612_DIG1_ADDR,0x00);
//								FD612_PotiveTNage(FD612_DIG2_ADDR,dis[(unsigned char)TEM2/10]);
//								FD612_PotiveTNage(FD612_DIG3_ADDR,dis[(unsigned char)TEM2%10]);
//								FD612_PotiveTNage(FD612_DIG4_ADDR,FD612_DISP_cc);
//								FD612_DispBuff.DispData[FD612_DIG12_ADDR]|= 1<<6 ; 
//								FD612_Refresh();
//							 ADC_Display1=0;
//							 break;
//						 }
//							

//							case 1:
//							{
//							ADC_AVG_NUM[2]   = (unsigned char)TEM4/100;
//							ADC_AVG_NUM[1]   = ((unsigned char)TEM4-ADC_AVG_NUM[2]*100 )/10;
//							ADC_AVG_NUM[0]   = ((unsigned char)TEM4-ADC_AVG_NUM[2]*100 )%10;
////							FD612_display_4DIG_input(dis[ADC_AVG_NUM[2]],dis[ADC_AVG_NUM[1]],dis[ADC_AVG_NUM[0]],FD612_DISP_F);
//							FD612_PotiveTNage(FD612_DIG1_ADDR,dis[ADC_AVG_NUM[2]]);
//							FD612_PotiveTNage(FD612_DIG2_ADDR,dis[ADC_AVG_NUM[1]]);
//							FD612_PotiveTNage(FD612_DIG3_ADDR,dis[ADC_AVG_NUM[0]] );
//								FD612_PotiveTNage(FD612_DIG4_ADDR,FD612_DISP_F);

//							FD612_Refresh();	
//								ADC_Display1=1;
////								ADC_Display=0;
//								break;
//							}	
//								default :
//								break;
//					}
					ADC_ONE=0;
				}
			}
	
		}
		switch(alarm_hour_long_up_down_flag)
		{
			case 4:
			{
				alarm1.hour++;
				if(hour_12_24_flag)
				{
					if(alarm1.hour==13)alarm1.hour=1;
				}
				else 
				{
					if(alarm1.hour>=24)alarm1.hour=0;
				}				
				alarm_hour_long_up_down_flag=0;				
				break;
			}			
			case 5:
			{
				alarm2.hour++;
				if(hour_12_24_flag)
				{
					if(alarm2.hour==13)alarm2.hour=1;
				}
				else 
				{
					if(alarm2.hour>=24)alarm2.hour=0;
				}					
				alarm_hour_long_up_down_flag=0;				
				break;
			}
			case 6:
			{
				alarm3.hour++;
				if(hour_12_24_flag)
				{
					if(alarm3.hour==13)alarm3.hour=1;
				}
				else 
				{
					if(alarm3.hour>=24)alarm3.hour=0;
				}					
				alarm_hour_long_up_down_flag=0;				
				break;
			}
			default :
			break;			
		}
		switch(alarm_min_long_up_down_flag)
		{
			case 4:
			{
				alarm1.minute++;
				if(alarm1.minute==60)alarm1.minute=0;
				alarm_min_long_up_down_flag=0;				
				break;
			}			
			case 5:
			{
				alarm2.minute++;
				if(alarm2.minute==60)alarm2.minute=0;
				alarm_min_long_up_down_flag=0;				
				break;
			}
			case 6:
			{
				alarm3.minute++;
				if(alarm3.minute==60)alarm3.minute=0;
				alarm_min_long_up_down_flag=0;				
				break;
			}			
			default :
			break;			
		}
		long_add_flag=0;
		ADC_ONE=1;
		second=0;
		while(!DOWN);
		if(long_flag==0)
		{
			DOWN_Short_Flag=1;
			second=0;
		}
	}
}
if(alarm_flag==1)
{
	if(SET==0||UP==0||DOWN==0)
	{
		alarm_flag=0;
		PWM_CTRL &= ~(1<<3);
	  alarm_voice_flag=0;		
	}
}
if(alarm2_flag==1)
{
	if(SET==0||UP==0||DOWN==0)
	{
		alarm2_flag=0;
		PWM_CTRL &= ~(1<<3);
	  alarm2_voice_flag=0;
	}
}
if(alarm3_flag==1)
{
	if(SET==0||UP==0||DOWN==0)
	{
		alarm3_flag=0;
		PWM_CTRL &= ~(1<<3);
		alarm3_voice_flag=0;		
	}
}

}
void display(void)
{
//	UINT8 pm_flag;
//		temper_display();
	  from_ds1302_read_time_data( );
		if(SET_Long_Flag==0)
		{
		if(SET_Short_Flag)
		{
			flag_display++;
			SET_Short_Flag=0;
			if(flag_display>5)
			{
				flag_display=0;
				ADC_fisrt=1;
			}
		}
//		if(time.hour>=13&&hour_12_24_flag==1)
//		{
//			time.hour=time.hour-12;
//			if(hour_pm_flag)hour_pm_flag=0;
//			else hour_pm_flag=1;
//			time.hour = decimal_bcd(time.hour);
//		 ResetDS1302();
//		 WriteByteDS1302(hour_w_addr); //写入时的地址
//		 WriteByteDS1302(time.hour);																	
//		time.hour = bcd_decimal(time.hour);			
//		}
		}

		switch(flag_display)
		{
			case 0:
				{



							if(display_flag_hur_min)
							{
									point_blink();
																

							
							

//							after_arrive_write_1302();	
							}
							if(SET_Long_Flag)
							{
								if(key_first)
								{
									key_first=0;
									UP_Short_Flag=0;
									DOWN_Short_Flag = 0;
								}
								display_flag_hur_min=0;
								if(SET_Short_Flag)
								{
									SET_Short_Flag_hur_min++;
									SET_Short_Flag=0;
									if(SET_Short_Flag_hur_min>4)
									{
										SET_Short_Flag_hur_min=0;
									}
								}
								if(SET_Short_Flag_hur_min==0)
								{
										display_12_24_blink( );	
										if(UP_Short_Flag==1)
										{
												if(hour_12_24_flag)
												{
													hour_12_24_flag=0;
													hour_chage_input(hour_12_24_flag);
													if(alarm1_pm_flag)
											{
												if(alarm1.hour!=12)
												{
													alarm1.hour=alarm1.hour+12;
												}
												alarm1_pm_flag=0;
											}
											else if(alarm1.hour==12)alarm1.hour=0;
											
											if(alarm2_pm_flag)
											{
												if(alarm2.hour!=12)
												{
													alarm2.hour=alarm2.hour+12;
												}
												alarm2_pm_flag=0;
											}
											else if(alarm2.hour==12)alarm2.hour=0;
											
											if(alarm3_pm_flag)
											{
												if(alarm3.hour!=12)
												{
													alarm3.hour=alarm3.hour+12;
												}
												alarm3_pm_flag=0;
											}
											else if(alarm3.hour==12)alarm3.hour=0;	
												}
												
				                else	
												{
													hour_12_24_flag=1;
													hour_chage_input(hour_12_24_flag);		
											if(alarm1.hour>12)
											{
												alarm1.hour=alarm1.hour-12;
												alarm1_pm_flag=1;
											}
											if(alarm1.hour==12)alarm1_pm_flag=1;
											if(alarm1.hour==0){alarm1.hour=12;alarm1_pm_flag=0;}
											
											if(alarm2.hour>12)
											{
												alarm2.hour=alarm2.hour-12;
												alarm2_pm_flag=1;
											}
											if(alarm2.hour==12)alarm2_pm_flag=1;
											if(alarm2.hour==0){alarm2.hour=12;alarm2_pm_flag=0;}
											
											if(alarm3.hour>12)
											{
												alarm3.hour=alarm3.hour-12;
												alarm3_pm_flag=1;
											}
											if(alarm3.hour==12)alarm3_pm_flag=1;
											if(alarm3.hour==0){alarm3.hour=12;alarm3_pm_flag=0;}													
												}
												UP_Short_Flag=0;
																																									
									}
								}
								if(SET_Short_Flag_hur_min==1)
								{
//									if(hour_12_24_flag==1)
//									{
//										time.hour<<=3;
//										time.hour>>=3;
//										time.hour &=~(1<<7);
//										time.hour &=~(1<<5);
//									}
								FD612_DISPLAY_2DIG_input(2, dis[time.minute/10] ,dis[time.minute%10]);	
								if(time1_second)
								{
								if(blink_flag)
								{	
									blink_flag=0;
									FD612_DISPLAY_2DIG_input(1 ,dis[time.hour/10] , dis[time.hour%10]);
								}
								else
								{
									blink_flag=1;	
									FD612_DISPLAY_2DIG_input(1 ,0x00 ,0x00  );
								}
								time1_second=0;
								}
								if(UP_Short_Flag==1)
								{										
										time.hour++;
									if(hour_12_24_flag){
									   if(time.hour>=13)
										 {
											 time.hour = 1;									 
											 
										 }
									if(time.hour==12)	after_arrive_write_1302();
										}
									else
									{
										if(time.hour >=24)
										{
											time.hour = 0;
										}
									}
										UP_Short_Flag=0;
								}
									if(DOWN_Short_Flag == 1)
								{
										time.hour--;	
									if(hour_12_24_flag){
									   if(time.hour>=13||time.hour==0)
										 {
												time.hour = time.hour -244; //溢出值
									 
											 
										 }
									if(time.hour==11)	after_arrive_write_1302();									 
										}
									else
									{
										if(time.hour >=24)
										{
											time.hour = time.hour -232; //溢出值
										}
									}									

										DOWN_Short_Flag = 0;
								}
								time.hour = decimal_bcd(time.hour);
//								ResetDS1302();
//								WriteByteDS1302(hour_w_addr); //写入时的地址
								if(hour_12_24_flag)
								{
									time.hour |=1<<7;
									if(hour_pm_flag)time.hour |=1<<5;
									else time.hour &=~(1<<5);
								}
								else time.hour &=~(1<<7);
								Write_addr_byte_ds1302(hour_w_addr,time.hour);
//								ResetDS1302();
//								WriteByteDS1302(hour_w_addr); //写入时的地址								
//								WriteByteDS1302(time.hour);
//								ResetDS1302();
								}
								if(SET_Short_Flag_hur_min==2)
								{
									FD612_DispBuff.DispData[12]|=1<<2;
									FD612_DISPLAY_2DIG_input(1 ,dis[time.hour/10]  ,dis[time.hour%10]);
									if(time1_second)
									{
										if(blink_flag)
										{
											blink_flag=0;
											
											FD612_PotiveTNage(FD612_DIG3_ADDR,dis[time.minute/10]);
											FD612_PotiveTNage(FD612_DIG4_ADDR,dis[time.minute%10]);
											pm_12_24_alarm_display();
											FD612_Refresh();
										}
									else
										{
											blink_flag=1;
											FD612_PotiveTNage(FD612_DIG3_ADDR,0x00);
											FD612_PotiveTNage(FD612_DIG4_ADDR,0x00);
											pm_12_24_alarm_display();
											FD612_Refresh();
											
										}
										time1_second=0;
									 }
										if(UP_Short_Flag == 1)
										{
												time.minute++;
											if(time.minute >=60)
											{
												time.minute = 0;
											}
												UP_Short_Flag = 0;
											
											Write_addr_byte_ds1302(sec_w_addr,0);
//											ResetDS1302();
//											WriteByteDS1302(sec_w_addr); //写入秒的地址
//											WriteByteDS1302(0);
										}
										if(DOWN_Short_Flag == 1)
										{
												time.minute--;
											if(time.minute>60)
											{
													time.minute = time.minute -196; //溢出值
											}
												DOWN_Short_Flag = 0;
												Write_addr_byte_ds1302(sec_w_addr,0);
//												ResetDS1302();
//												WriteByteDS1302(sec_w_addr); //写入秒的地址
//												WriteByteDS1302(0);	

										}
										time.minute = decimal_bcd(time.minute);
										
										Write_addr_byte_ds1302(min_w_addr,time.minute);
//										ResetDS1302();
//										WriteByteDS1302(min_w_addr); //写入秒的地址
//										WriteByteDS1302(time.minute);
								}
								if(SET_Short_Flag_hur_min==3)
								{
									display_flag_hur_min=1;
									SET_Long_Flag=0;
									SET_Short_Flag_hur_min=0;
									key_first=1;
								}
								if(back_first)
								{
									back_TIME_Flag=0;
									back_first=0;
								}
								if(back_TIME_Flag==over_time)
								{
									display_flag_hur_min=1;	
									SET_Long_Flag=0;
									SET_Short_Flag_hur_min=0;
									back_TIME_Flag=0;
									back_first=1;
									key_first=1;
							    dp_mode_exit();
									if(mode_select_flag==2)	flag_display=0;
								}
							
								
							}
							break;					
				}
				case 1:
					{

							
				


					if(SET_Long_Flag)
					{
								if(key_first)
								{
									key_first=0;
									UP_Short_Flag=0;
									DOWN_Short_Flag = 0;
								}						
						display_flag_mon_day=0;
					
					if(SET_Short_Flag)
								{
									SET_Short_Flag_mon_day++;
									SET_Short_Flag=0;
									if(SET_Short_Flag_mon_day>4)
									{
										SET_Short_Flag_mon_day=0;
									}
								}
					if(SET_Short_Flag_mon_day==0)
					{
					year_blink(0);
					if(UP_Short_Flag == 1)
					{
							time.year++;	
							if(time.year>99)time.year=0;
							UP_Short_Flag = 0;
					}
					if(DOWN_Short_Flag == 1)
					{
							time.year--;
							DOWN_Short_Flag = 0;
					}	
					time.year= decimal_bcd(time.year);
					Write_addr_byte_ds1302(year_w_addr,time.year);
//					ResetDS1302();
//					WriteByteDS1302(year_w_addr); //写入秒的地址
//					WriteByteDS1302(time.year);
					
					}
					if(SET_Short_Flag_mon_day==1)
					{
									
								FD612_DISPLAY_2DIG_input(2,dis[time.mday/10] ,dis[time.mday%10] );
								if(time1_second)
								{
								if(blink_flag)
								{
									blink_flag=0;	
									FD612_DISPLAY_2DIG_input(1 ,dis[time.month/10] ,dis[time.month%10] );
								}
								else
								{
									blink_flag=1;
									FD612_DISPLAY_2DIG_input(1 ,0x00  ,0x00 );
								}
								time1_second=0;
								}
					if(UP_Short_Flag == 1)
					{
							time.month++;
						
						if(time.month >=13)
						{
							time.month = 1;
						}
							UP_Short_Flag = 0;
					}
					if(DOWN_Short_Flag == 1)
					{
							time.month--;
							
						if(time.month==0 || time.month>13)
						{
								time.month = time.month -244; //溢出值
						}
							DOWN_Short_Flag = 0;
					}	
					time.month = decimal_bcd(time.month);

					Write_addr_byte_ds1302(mon_w_addr,time.month);
//					ResetDS1302();
//					WriteByteDS1302(mon_w_addr); //写入秒的地址
//					WriteByteDS1302(time.month);
					}
					if(SET_Short_Flag_mon_day==2)
								{
									
									
									FD612_DISPLAY_2DIG_input(1 ,dis[time.month/10] ,dis[time.month%10] );
									if(time1_second)
									{
										if(blink_flag)
										{
											blink_flag=0;	
											FD612_DISPLAY_2DIG_input(2,dis[time.mday/10] ,dis[time.mday%10] );
										}
									else
										{
											blink_flag=1;	
											FD612_DISPLAY_2DIG_input(2,0x00 ,0x00 );
										}
										time1_second=0;
									 }
								 
					if(UP_Short_Flag >= 1)
					{
							time.mday++;
												 
							if(time.mday>=32)   
							{          						    
										time.mday=1;
							}          
        
						UP_Short_Flag = 0;						
					}	
					if(DOWN_Short_Flag >= 1)
					{
							time.mday--;
							if(time.mday==0 || time.mday >=32)
								{								
							
							  time.mday = time.mday -225;
								}          
								         							    															
							DOWN_Short_Flag = 0;
					}					
					time.mday = decimal_bcd(time.mday);
					Write_addr_byte_ds1302(day_w_addr,time.mday);
//					ResetDS1302();
//					WriteByteDS1302(day_w_addr); //写入秒的地址
//					WriteByteDS1302(time.mday);
					
					}
								 
								if(SET_Short_Flag_mon_day==3)
								{
									display_flag_mon_day=1;
									SET_Long_Flag=0;
									SET_Short_Flag_mon_day=0;
									key_first=1;
								}
								if(back_first)
								{
									back_TIME_Flag=0;
									back_first=0;
								}
								if(back_TIME_Flag==over_time)
								{
									display_flag_mon_day=1;	
									SET_Long_Flag=0;
									SET_Short_Flag_mon_day=0;
									back_TIME_Flag=0;
									back_first=1;
									key_first=1;
									dp_mode_exit();										
								}									
							}
							
						if(display_flag_mon_day)
							{
								
								FD612_display_4DIG_input( dis[time.month/10] ,dis[time.month% 10] ,dis[time.mday/10] , dis[time.mday% 10]);
							}
							
							
//							if(SET_Long_Flag==0){
//							if(back_first)
//								{
//									back_TIME_Flag=0;
//									back_first=0;
//								}
//								if(back_TIME_Flag==over_time)
//								{
//									display_flag_mon_day=1;	
//									SET_Long_Flag=0;
//									SET_Short_Flag_mon_day=0;
//									back_TIME_Flag=0;
//									back_first=1;
//									key_first=1;
//									if(mode_select_flag==2)	flag_display=0;
//								}	
//							}
						break;						
					}
				case 2:
					{

						alarm_set_display(1, alarm1);
						break;	
					}
				case 3:
					{

						alarm_set_display(2, alarm2);
						break;	
					}
				case 4:
					{

						alarm_set_display(3, alarm3);
						break;	
					}
			
				case 5:
				{
					if(SET_Long_Flag==1)SET_Long_Flag=0;
					if(UP_Short_Flag)UP_Short_Flag=0;
					if(ADC_fisrt)
					{
						ADC_Convert( );
						ADC_fisrt=0;
						ADC_TIME_Flag=0;
						
					}
					temper_display();
//					if(ADC_TIME_Flag==temp_refresh_time)
//					{
//						ADC_fisrt=1;
//						ADC_TIME_Flag=0;
//					}
//				
//			if(ADC_Display1==0)
//			{			
//			FD612_display_4DIG_input( 0x00 ,dis[(unsigned char)TEM2/10] ,dis[(unsigned char)TEM2%10] ,FD612_DISP_C );
//			}
//			else
//			{
//			ADC_AVG_NUM[2]   = (unsigned char)TEM4/100;
//			ADC_AVG_NUM[1]   = ((unsigned char)TEM4-ADC_AVG_NUM[2]*100 )/10;
//			ADC_AVG_NUM[0]   = ((unsigned char)TEM4-ADC_AVG_NUM[2]*100 )%10;

//			FD612_display_4DIG_input(dis[ADC_AVG_NUM[2]]  , dis[ADC_AVG_NUM[1]],dis[ADC_AVG_NUM[0]]  ,FD612_DISP_F );
//			}				
					if(back_first)
					{
						back_TIME_Flag=0;
						back_first=0;
					}
					if(back_TIME_Flag==over_time)
					{																										
						back_TIME_Flag=0;
						back_first=1;
						if(mode_select_flag==2)	flag_display=0;
						dp_mode_exit();											
					}
					break;	
				}
				default :
				break;
		}
}






void  ADC_Convert(void)	
{
		float VCC = 4.77;													 //
	  float ADC_VALUE;													 //
													 //
//		unsigned int B_value = 3450;
		unsigned int B_value = 3950;//科艺传感器

		ADC_CHAN1 =0;
		ADC_CHAN0=0;
		P1_DIR_PU &= ~bAIN0;	
		ADC_START = 1;  
		while(ADC_START);                                                      //ADC_START变为0时，表示采样完成
		ADC_VALUE = ADC_DATA * VCC / 255; // 读取转换的AD值		
	
		TEM = 1/(log(ADC_VALUE/(VCC - ADC_VALUE))/B_value + 1/(273.15 + 25));
			
		TEM4=TEM2*1.8+32;
		TEM2=TEM- 273.15;
		
}


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
				if((alarm1_value2-60)==time.minute)
				{
					alarm_voice_flag=1;
				}				
			}
			else if(alarm1_value2==time.minute)
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
			  if((alarm1_value4-60)==time.minute)
				{
					alarm_voice_flag=1;
				}				
			}
			else if((alarm1_value4)==time.minute)
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
				 if((alarm2_value2-60)==time.minute)
				{
					alarm2_voice_flag=1;
				}
			}
			else if((alarm2_value2)==time.minute)
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
				if((alarm2_value4-60)==time.minute)
				alarm2_voice_flag=1;
			}			
			else if((alarm2_value4)==time.minute)
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
				if((alarm3_value2-60)==time.minute)
				{
					alarm3_voice_flag=1;
				}				
			}
			else if((alarm3_value2)==time.minute)
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
				if((alarm3_value4-60)==time.minute)
				{
					alarm3_voice_flag=1;
				}				
			}
			else if((alarm3_value4)==time.minute)
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

void alarm_display (void)
{
	 if(alarm1_on_dispaly)
 {
		alarm1_sleep( );
 }
 	 if(alarm2_on_dispaly)
 {
		alarm2_sleep( );
 }
 	 if(alarm3_on_dispaly)
 {
		alarm3_sleep( );
 }
}



void voice_control(void)
{
	if(SET_Long_Flag==0)
	{
		if(voice_flag==0)
		{
		if(voice_key==1)
			{
	
				if(voice_key==1)
				{
					FD612_DispStateWr(FD612_INTENS8|FD612_DISP_ON);  
					FD612_Refresh();
				}

			}
				else
				{
					FD612_DispStateWr(FD612_DISP_OFF); 
					FD612_Refresh();
				}			
		}
		if(FD612_DispBuff.DispState&0x08)
				{
				
					voice_flag=1;
					if(voice_fisrt)
					{
						back_voice_Flag=0;
						voice_fisrt=0;
					}

					if(back_voice_Flag==voice_wake_up_time)
					{
						voice_flag=0;
						voice_fisrt=1;
						flag_display=0;
						FD612_DispStateWr(FD612_DISP_OFF); 
						FD612_Refresh();
						mDelayuS(150);
				
					}
				}
			}
}

void sd_mode (void)
{
	if(DOWN_Short_Flag)
	{
		up_down_flag=1;//up_down键与模式选择冲突
//		if(SET_Long_Flag==0&&flag_display !=5)
		if(SET_Long_Flag==0&&flag_display ==0)
		{
			dislay_all_flag=1;
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
						
						FD612_display_4DIG_input(0x40  ,0x40 ,FD612_DISP_S ,FD612_DISP_d  );
						sd_mode_flag=0;
						break;
			
					}
					case 1: 
					{
						
						FD612_display_4DIG_input(0x5c  ,0x54 ,FD612_DISP_S ,FD612_DISP_d );
						sd_mode_flag=1;
						break;
					}
						default :
						break;					
				}
		}
		up_down_flag=0;
		DOWN_Short_Flag=0;	
		mode_select_first=1;
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

			dislay_all_flag=0;
			back_TIME_Flag=0;
			back_first=1;	
			UP_Short_Flag=0;
			SET_Short_Flag=0;
			mode_select_first=1;
			sd_select_first=1;
//			up_down_flag=0;
			

		}					
}


void Display_Mode_Select(void)
{
		if(UP_Short_Flag)
		{
				up_down_flag=1;//up_down键与模式选择冲突
//				dislay_all_flag=1;

//				if(SET_Long_Flag==0&&flag_display !=2&&flag_display !=3&&flag_display !=4)
			if(SET_Long_Flag==0&&flag_display ==0)
				{
					dislay_all_flag=1;
						mode_select_flag++;
					if(mode_select_first==1)
					{
						mode_select_flag--;
						mode_select_first=0;
					}
					if(mode_select_flag>2)mode_select_flag=0;
					switch (mode_select_flag)
					{
						case 0:
							{
								
								FD612_display_4DIG_input( FD612_DISP_d ,FD612_DISP_P  ,0x40 ,FD612_DISP_1 );
								break;
							}
						case 1:
							{
									
								FD612_display_4DIG_input( FD612_DISP_d ,FD612_DISP_P ,0x40 ,FD612_DISP_2 );
								break;
							}
						case 2:
							{
								
								FD612_display_4DIG_input( FD612_DISP_d ,FD612_DISP_P ,0x40 ,FD612_DISP_3 );
								break;
							}	
								default :
								break;							
					}
										UP_Short_Flag=0;
										up_down_flag=0;
				}

				sd_select_first=1;
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
//		if(back_TIME_Flag==over_time ||SET_Short_Flag==1 )
//		{
//			FD612_DispBuff.DispData[FD612_DIG1_ADDR]=0x00 ; 
//			FD612_DispBuff.DispData[FD612_DIG2_ADDR]=0x00 ;
//			FD612_DispBuff.DispData[FD612_DIG3_ADDR]=0x00; 
//			FD612_DispBuff.DispData[FD612_DIG4_ADDR]=0x00 ;
//			FD612_Refresh();

//			dislay_all_flag=0;
//			back_TIME_Flag=0;
//			back_first=1;	
//			UP_Short_Flag=0;
//			SET_Short_Flag=0;
//			mode_select_first=1;
//			up_down_flag=1;
//		}

}
void mode1_select_dislay(void)
{
		if(mode_select_flag==0)
		{
			if(SET_Long_Flag==0)
			{
			if(hor_min_delay_time==over_time_2s)
			{
				flag_display=5;
				hor_min_delay_time=0;
				
			}
			if(mon_day_delay_time==over_time_10s)
			{
				flag_display=1;
				mon_day_delay_time=0;
			}
			if(temp_delay_time==over_time_2s)
			{
				flag_display=0;
				temp_delay_time=0;
			}	
		}
			
		}
}



void year_long_up_down(UINT8 i)
{
//			ResetDS1302();
//			WriteByteDS1302(year_r_addr); //读取分的地址				
//			time.year=ReadByteDS1302();
//			ResetDS1302();
			time.year=Read_addr_byte_ds1302(year_r_addr);	
			time.year = bcd_decimal(time.year);
      if(i==1)time.year--;
      if(i==0)
			{
				time.year++;
				if(time.year>99)time.year=0;
					
			}
							 
			FD612_display_4DIG_input( dis[2] ,dis[0] ,dis[time.year/10] , dis[ time.year% 10]);
			time.year = decimal_bcd(time.year);
			Write_addr_byte_ds1302(year_w_addr,time.year);
//			ResetDS1302();
//			WriteByteDS1302(year_w_addr); //写入时的地址
//			WriteByteDS1302(time.year);
//			ResetDS1302();	
			mDelaymS(up_down_delay_time);	
			time.year = bcd_decimal(time.year);
}

void hour_long_up_down(UINT8 i)
{
//				ResetDS1302();							
//				WriteByteDS1302(hour_r_addr); //读取时的地址
//				time.hour=ReadByteDS1302();
				time.hour=Read_addr_byte_ds1302(hour_r_addr);				
				if(hour_12_24_flag==1)
				{
					time.hour<<=3;
					time.hour>>=3;
//					time.hour &=~(1<<7);
//					time.hour &=~(1<<5);
				}
				time.hour = bcd_decimal(time.hour);
				if(i==0) 
				{
					time.hour++;
					if(hour_12_24_flag){
						 if(time.hour>=13)
						 {
							 time.hour = 1;
//							 ResetDS1302();							
//							 WriteByteDS1302(hour_r_addr); //读取时的地址
//							 hour_chage_state=ReadByteDS1302();
							
						 }
						 if(time.hour==12)
						 {
							 hour_chage_state=Read_addr_byte_ds1302(hour_r_addr);	
							 if(hour_chage_state&0x20)
							 {
//								 hour_chage_state &=~(1<<5);
								 hour_pm_flag=0;
							 }
							 else 
							 {
//								 hour_chage_state |=(1<<5);
								 hour_pm_flag=1;
							 }							 
							}
						}
					else
					{
						if(time.hour >=24)
						{
							time.hour = 0;
						}	
					}
				
				}
				if(i==1)
				{ 
						time.hour--;
						if(hour_12_24_flag){
							 if(time.hour>=13||time.hour==0)
							 {
								time.hour = time.hour -244; //溢出值
//							 ResetDS1302();							
//							 WriteByteDS1302(hour_r_addr); //读取时的地址
//							 hour_chage_state=ReadByteDS1302();

							 }
								 if(time.hour==11)
								 {
									 hour_chage_state=Read_addr_byte_ds1302(hour_r_addr);
									 if(hour_chage_state&0x20)
									 {
//										 hour_chage_state &=~(1<<5);
										 hour_pm_flag=0;
									 }
									 else 
									 {
//										 hour_chage_state |=(1<<5);
										 hour_pm_flag=1;
									 }							 
								}							 
							}
						else
						{
							if(time.hour >=24)
							{
								time.hour = time.hour -232; //溢出值
							}
						}					
					
				}
				FD612_DISPLAY_2DIG_input(1 ,dis[time.hour/10]  ,dis[time.hour% 10] );

			 time.hour = decimal_bcd(time.hour);
				if(hour_12_24_flag)
				{
					time.hour |=1<<7;
					if(hour_pm_flag)time.hour |=1<<5;
					else time.hour &=~(1<<5);
				}
				else time.hour &=~(1<<7);
				Write_addr_byte_ds1302(hour_w_addr,time.hour);
//			 ResetDS1302();
//			 WriteByteDS1302(hour_w_addr); //写入时的地址
//			 WriteByteDS1302(time.hour);
//			 ResetDS1302();
			 mDelaymS(up_down_delay_time);
			time.hour = bcd_decimal(time.hour);
				
//						if(hour_12_24_flag==1)
//					{
//						if(hour_pm_flag==1)
//						{
////								FD612_DispBuff.DispData[FD612_DIG5_ADDR]=FD612_DISP_P ;
//								FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<1) ;
//								FD612_Refresh();		
//						}
//						else 
//						{
////								FD612_DispBuff.DispData[FD612_DIG5_ADDR]=FD612_DISP_A ; 
//								FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<1) ;
//								FD612_Refresh();		
//						}
//					}
//	
}

void min_long_up_down(UINT8 i) 
{
//			ResetDS1302();
//			WriteByteDS1302(min_r_addr); //读取分的地址				
//			time.minute=ReadByteDS1302();
//			ResetDS1302();
			time.minute=Read_addr_byte_ds1302(min_r_addr);	
			time.minute = bcd_decimal(time.minute);
			if(i==0)
			{
				time.minute++;
				if(time.minute >=60)
				{
					time.minute = 0;
				}
			}
			if(i==1)
			{
			time.minute--;
			if(time.minute>60)
			{
					time.minute = time.minute -196; //溢出值
			}	
			}

			FD612_DISPLAY_2DIG_input(2,dis[time.minute/10] ,dis[ time.minute% 10] );
			time.minute = decimal_bcd(time.minute);

				Write_addr_byte_ds1302(min_w_addr,time.minute);
				Write_addr_byte_ds1302(sec_w_addr,0);
		
//				ResetDS1302();
//				WriteByteDS1302(min_w_addr); //写入分的地址
//				WriteByteDS1302(time.minute);
//				ResetDS1302();
//				WriteByteDS1302(sec_w_addr); //写入秒的地址
//				WriteByteDS1302(0);
//				ResetDS1302();
			time.minute = bcd_decimal(time.minute);
		  mDelaymS(up_down_delay_time);
}
void mon_long_up_down(UINT8 i)
{
//			ResetDS1302();
//			WriteByteDS1302(mon_r_addr); //读取分的地址				
//			time.month=ReadByteDS1302();
//			ResetDS1302();
			time.month=Read_addr_byte_ds1302(mon_r_addr);
			time.month = bcd_decimal(time.month);	
			if(i==0)
			{
				time.month++;
				if(time.month >=13)
				{
					time.month = 1;
				}
			}
			if(i==1)
			{
			time.month--;
			if(time.month==0 || time.month>13)
			{
						time.month = time.month -244; //溢出值
			}	
			}	
	
			
			FD612_DISPLAY_2DIG_input(1 ,dis[time.month/10] , dis[time.month% 10]);
			time.month = decimal_bcd(time.month);

				Write_addr_byte_ds1302(mon_w_addr,time.month);

//			ResetDS1302();
//			WriteByteDS1302(mon_w_addr); //写入分的地址
//			WriteByteDS1302(time.month);
//			ResetDS1302();

			time.month = bcd_decimal(time.month);			
		  mDelaymS(up_down_delay_time);			
}

void mday_long_up_down(UINT8 i)
{
//			ResetDS1302();
//			WriteByteDS1302(day_r_addr); //读取分的地址				
//			time.mday=ReadByteDS1302();
//			ResetDS1302();
			time.mday=Read_addr_byte_ds1302(day_r_addr);
			time.mday = bcd_decimal(time.mday);
			if(i==0)
			{
				time.mday++;
				if(time.mday >31)
				{
					time.mday = 1;
				}
			}
			if(i==1)
			{
			time.mday--;
			if(time.mday==0 || time.mday>31)
			{
						time.mday = time.mday -225; //溢出值
			}	
			}	

				
				FD612_DISPLAY_2DIG_input(2,dis[time.mday/10] ,dis[time.mday% 10] );
				time.mday = decimal_bcd(time.mday);

				Write_addr_byte_ds1302(day_w_addr,time.mday);

//				ResetDS1302();
//				WriteByteDS1302(day_w_addr); //写入日的地址
//				WriteByteDS1302(time.mday);		
				time.mday = bcd_decimal(time.mday);
				mDelaymS(up_down_delay_time);			
}
void alarm_hour_long_up_down(UINT8 alarm_seclect,UINT8 up_down_seclect,struct DATA_ALARM alarm)
{
	
	if(up_down_seclect==1)
	{
				if(alarm_seclect==1)
				{
					
					alarm1.hour++;
					if(hour_12_24_flag){
						 if(alarm1.hour>=13)
						 {
							 alarm1.hour = 1;
						 }
						 if(alarm1.hour==1)
//						if(alarm1.hour==12)
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
							alarm1.hour = alarm1.hour -244; //溢出值
						
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
						alarm1.hour = alarm1.hour -232; //溢出值
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
							alarm2.hour = alarm2.hour -244; //溢出值

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
						alarm2.hour = alarm2.hour -232; //溢出值
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
							alarm3.hour = alarm3.hour -244; //溢出值
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
						alarm3.hour = alarm3.hour -232; //溢出值
					}
				}	
			alarm_hour_long_up_down_flag=6;
			}		
		}

					
				FD612_DISPLAY_2DIG_input(1 , dis[alarm.hour/10],dis[alarm.hour%10] );
				mDelaymS(up_down_delay_time);
				
}


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
						alarm1.minute = alarm1.minute -196; //溢出值
				}
				alarm_min_long_up_down_flag=4;				
				}
				if(alarm_seclect==2)
				{
					alarm2.minute--;
				if(alarm2.minute>60)
				{
						alarm2.minute = alarm2.minute -196; //溢出值
				}
				alarm_min_long_up_down_flag=5;				
				}
				if(alarm_seclect==3)
				{
					alarm3.minute--;
				if(alarm3.minute>60)
				{
						alarm3.minute = alarm3.minute -196; //溢出值
				}
				alarm_min_long_up_down_flag=6;				
				}			
		}

				
				FD612_DISPLAY_2DIG_input(2,dis[alarm.minute/10]  ,dis[alarm.minute%10] );
				mDelaymS(up_down_delay_time);
				
}

void alarm_set_display(UINT8 i,struct DATA_ALARM alarm)
{
							if(SET_Long_Flag)
						{
								if(key_first)
								{
									key_first=0;
									UP_Short_Flag=0;
									DOWN_Short_Flag = 0;
								}

							if(i==1)alarm1_on_dispaly=1;
							if(i==2)alarm2_on_dispaly=1;
							if(i==3)alarm3_on_dispaly=1;	
							if(SET_Short_Flag)
								{
									SET_Short_Flag_alarm1_flag++;
									SET_Short_Flag=0;
									if(SET_Short_Flag_alarm1_flag>3)
									{
										SET_Short_Flag_alarm1_flag=0;
									}
								}
					if(SET_Short_Flag_alarm1_flag==0)
						{

					if(time1_second)
					{
					if(blink_flag)
						{
							blink_flag=0;
							FD612_DISPLAY_2DIG_input(1 ,dis[alarm.hour/10] ,dis[alarm.hour%10] );
						}
						else
						{
							blink_flag=1;
							FD612_DISPLAY_2DIG_input(1 ,0x00 , 0x00);
						}
						time1_second=0;
					}
						
						FD612_DISPLAY_2DIG_input(2,dis[alarm.minute/10] ,dis[alarm.minute%10] );
								if(UP_Short_Flag==1)
								{
									if(i==1)
									{
										alarm1.hour++;
										if(hour_12_24_flag){
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
									}
									if(i==2)
									{
										alarm2.hour++;
										if(hour_12_24_flag){
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
									}	
									if(i==3)
									{
										alarm3.hour++;
										if(hour_12_24_flag){
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
									}									
										UP_Short_Flag=0;
								}
									if(DOWN_Short_Flag == 1)
								{
									if(i==1)
									{
										alarm1.hour--;					
										if(hour_12_24_flag){
											 if(alarm1.hour>=13||alarm1.hour==0)
											 {
													alarm1.hour = alarm1.hour -244; //溢出值
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
												alarm1.hour = alarm1.hour -232; //溢出值
											}
										}
								  }
									if(i==2)
									{
										alarm2.hour--;					
										if(hour_12_24_flag){
											 if(alarm2.hour>=13||alarm2.hour==0)
											 {
													alarm2.hour = alarm2.hour -244; //溢出值
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
												alarm2.hour = alarm2.hour -232; //溢出值
											}
										}
								  }
									if(i==3)
									{
										alarm3.hour--;					
										if(hour_12_24_flag){
											 if(alarm3.hour>=13||alarm3.hour==0)
											 {
													alarm3.hour = alarm3.hour -244; //溢出值
											 }
											if(alarm2.hour==11)
											 {
												 if(alarm2_pm_flag)alarm2_pm_flag=0;
												 else alarm2_pm_flag=1;
												}											 
											}
										else
										{
											if(alarm3.hour >=24)
											{
												alarm3.hour = alarm3.hour -232; //溢出值
											}
										}
								  }									
										DOWN_Short_Flag = 0;
								}						
						}
					if(SET_Short_Flag_alarm1_flag==1)
					{

						if(time1_second)
						{
						if(blink_flag)
							{
								blink_flag=0;	
								FD612_DISPLAY_2DIG_input(2,dis[alarm.minute/10] ,dis[alarm.minute%10] );
							}
							else
							{
								blink_flag=1;
								FD612_DISPLAY_2DIG_input(2,0x00 ,0x00 );	
							}
							time1_second=0;
						}
							
							FD612_DISPLAY_2DIG_input(1 ,dis[alarm.hour/10] ,dis[alarm.hour%10] );
								
							if(UP_Short_Flag == 1)
							{
								if(i==1)
								{
									alarm1.minute++;
								if(alarm1.minute >=60)
								{
									alarm1.minute = 0;
								}
								}
								if(i==2)
								{
									alarm2.minute++;
								if(alarm2.minute >=60)
								{
									alarm2.minute = 0;
								}
								}
								if(i==3)
								{
									alarm3.minute++;
								if(alarm3.minute >=60)
								{
									alarm3.minute = 0;
								}
								}								
									UP_Short_Flag = 0;
							}
							if(DOWN_Short_Flag == 1)
							{
								if(i==1)
								{
									alarm1.minute--;
								if(alarm1.minute>60)
								{
										alarm1.minute = alarm1.minute -196; //溢出值
								}
								}
								if(i==2)
								{
									alarm2.minute--;
								if(alarm2.minute>60)
								{
										alarm2.minute = alarm2.minute -196; //溢出值
								}
								}
								if(i==3)
								{
									alarm3.minute--;
								if(alarm3.minute>60)
								{
										alarm3.minute = alarm3.minute -196; //溢出值
								}
								}								
									DOWN_Short_Flag = 0;
							}
					}	
					if(SET_Short_Flag_alarm1_flag==2)	
					{
									if(i==1)alarm1_on_dispaly=1;
									if(i==2)alarm2_on_dispaly=1;
									if(i==3)alarm3_on_dispaly=1;
									SET_Long_Flag=0;
									SET_Short_Flag_alarm1_flag=0;
									key_first=1;
					
					}
							if(back_first)
								{
									back_TIME_Flag=0;
									back_first=0;
								}
							if(back_TIME_Flag==over_time)
								{
									if(i==1)alarm1_on_dispaly=1;
									if(i==2)alarm2_on_dispaly=1;
									if(i==3)alarm3_on_dispaly=1;
									SET_Long_Flag=0;
									SET_Short_Flag_alarm1_flag=0;
									back_TIME_Flag=0;
									back_first=1;
									dp_mode_exit( );	
									//flag_display=0;
									key_first=1;

								}	
						}
						if(i==1)
						{
						if(alarm1_on_dispaly==0)
						{
					
							FD612_display_4DIG_input( 0x40 ,0x40 ,FD612_DISP_A ,FD612_DISP_1 );
								if(UP_Short_Flag==1)
								{
									alarm1_on_dispaly=1;
									UP_Short_Flag=0;
								}							
						}

						if(SET_Long_Flag==0)
						{
						if(alarm1_on_dispaly)
							{
									
								FD612_display_4DIG_input( 0x5c ,0x54 ,FD612_DISP_A ,FD612_DISP_1 );
								if(UP_Short_Flag==1)
								{
									alarm1_on_dispaly=0;
									UP_Short_Flag=0;
								}
							}
						}
						}
						if(i==2)
						{
						if(alarm2_on_dispaly==0)
						{
						
						FD612_display_4DIG_input( 0x40 , 0x40,FD612_DISP_A ,FD612_DISP_2 );
						if(UP_Short_Flag==1)
						{
							alarm2_on_dispaly=1;
							UP_Short_Flag=0;
						}							
						}

						if(SET_Long_Flag==0)
						{
						if(alarm2_on_dispaly)
							{
								
								FD612_display_4DIG_input(  0x5c,0x54,FD612_DISP_A ,FD612_DISP_2 );
								if(UP_Short_Flag==1)
								{
									alarm2_on_dispaly=0;
									UP_Short_Flag=0;
								}
							}
						}							
						}
						if(i==3)
						{
						if(alarm3_on_dispaly==0)
						{
						
							FD612_display_4DIG_input( 0x40 ,0x40 ,FD612_DISP_A ,FD612_DISP_3 );
						if(UP_Short_Flag==1)
						{
							alarm3_on_dispaly=1;
							UP_Short_Flag=0;
						}							
						}

						if(SET_Long_Flag==0)
						{
						if(alarm3_on_dispaly)
							{
									
								FD612_display_4DIG_input( 0x5c , 0x54,FD612_DISP_A ,FD612_DISP_3 );
								if(UP_Short_Flag==1)
								{
									alarm3_on_dispaly=0;
									UP_Short_Flag=0;
								}
							}
						}							
						}
						if(SET_Long_Flag==0)
						{
						if(back_first)
								{
									back_TIME_Flag=0;
									back_first=0;
								}
								if(back_TIME_Flag==over_time)
								{
								//	alarm1_on_dispaly=1;	
//									SET_Long_Flag=0;
//									SET_Short_Flag_alarm1_flag=0;
									back_TIME_Flag=0;
									back_first=1;
									if(mode_select_flag==2)	flag_display=0;
									dp_mode_exit()	;								
								}	
						}
}

void alarm_hour_display(struct DATA_ALARM alarm)
{

					
				FD612_DISPLAY_2DIG_input(1 ,dis[alarm.hour/10] ,dis[alarm.hour%10] );
}

void alarm_min_display(struct DATA_ALARM alarm)
{

					
				FD612_DISPLAY_2DIG_input(2,dis[alarm.minute/10] ,dis[alarm.minute%10] );
}

void alarm_long_blink(struct DATA_ALARM alarm)//闹钟长按闪烁
{
		if(time1_second==1)
	{

		if(blink_flag)
		{
			blink_flag=0;
			FD612_DISPLAY_2DIG_input(1 ,dis[alarm.hour/10],dis[alarm.hour%10]);
		}
		else
		{
			blink_flag=1;
			FD612_DISPLAY_2DIG_input(1 ,0x00,0x00);	
		}

		FD612_DISPLAY_2DIG_input(2 ,dis[alarm.minute/10],dis[alarm.minute%10]);
		time1_second=0;
	}
}

void dp_mode_exit(void)
{
		if(mode_select_flag==0){
			flag_display=0;
//			temp_delay_time=0;
			mon_day_delay_time=0;
			hor_min_delay_time=0;
		}																			
		
}

void key_dislpay_up_down(void)
{
			if(long_add_flag==0)
			{
				switch(flag_display)
			{
				case 0:
				{
					if(SET_Short_Flag_hur_min==1 )
					{

						
						FD612_DISPLAY_2DIG_input(1 ,dis[time.hour/10]  ,dis[time.hour% 10]);
					}
					if(SET_Short_Flag_hur_min==2 )
					{

					
						FD612_DISPLAY_2DIG_input(2,dis[time.minute/10] ,dis[time.minute% 10] );
					}	
					break;					
				}
				case 1:
				{
					if(SET_Short_Flag_mon_day==0 )
					{
							
						FD612_display_4DIG_input( dis[2] ,dis[0] ,dis[time.year/10] ,dis[time.year%10] );
					}
					if(SET_Short_Flag_mon_day==1&&long_add_flag==0 )
					{

					
					FD612_DISPLAY_2DIG_input(1 ,dis[time.month/10] ,dis[time.month% 10] );	
					}
					if(SET_Short_Flag_mon_day==2 )
					{
	
						
						FD612_DISPLAY_2DIG_input(2,dis[time.mday/10] ,dis[time.mday% 10] );
					}
					break;					
				}
				case 2:
				{
					if(SET_Short_Flag_alarm1_flag==0 )
					{
					 alarm_hour_display(alarm1);
					
					}
					if(SET_Short_Flag_alarm1_flag==1 )
					{
						alarm_min_display(alarm1)	;			
					}	
					break;
				}
				case 3:
				{
					if(SET_Short_Flag_alarm1_flag==0 )
					{
						alarm_hour_display( alarm2);				
					}	
					if(SET_Short_Flag_alarm1_flag==1 )
					{
						alarm_min_display(alarm2)	;
					}
					break;
				}
				case 4:
				{
						if(SET_Short_Flag_alarm1_flag==0 )
						{
							alarm_hour_display(alarm3);				
						}	
						if(SET_Short_Flag_alarm1_flag==1 )
						{
							alarm_min_display(alarm3)	;
						}	
					break;
				}				
			}
		}
}



void FD612_display_4DIG_input(UINT8 dig1,UINT8 dig2,UINT8 dig3,UINT8 dig4)
{
//			FD612_DispBuff.DispData[FD612_DIG1_ADDR]=dig1 ; 
//			FD612_DispBuff.DispData[FD612_DIG2_ADDR]=dig2;
//			FD612_DispBuff.DispData[FD612_DIG3_ADDR]=dig3 ; 
//			FD612_DispBuff.DispData[FD612_DIG4_ADDR]=dig4 ;
			FD612_PotiveTNage(FD612_DIG1_ADDR,dig1);
			FD612_PotiveTNage(FD612_DIG2_ADDR,dig2);
			FD612_PotiveTNage(FD612_DIG3_ADDR,dig3);
			FD612_PotiveTNage(FD612_DIG4_ADDR,dig4);
			pm_12_24_alarm_display();
			FD612_Refresh();	
}
void FD612_DISPLAY_2DIG_input(UINT8 i ,UINT8 dig1,UINT8 dig2)
{
		if(i==1)
		{
			FD612_PotiveTNage(FD612_DIG1_ADDR,dig1);
			FD612_PotiveTNage(FD612_DIG2_ADDR,dig2);
			pm_12_24_alarm_display();
//			FD612_Refresh();
		}
		if(i==2)
		{
			FD612_PotiveTNage(FD612_DIG3_ADDR,dig1);
			FD612_PotiveTNage(FD612_DIG4_ADDR,dig2);
			pm_12_24_alarm_display();
//			FD612_Refresh();
		}
}
void  pm_12_24_alarm_display(void)
{
					if(hour_12_24_flag==1)
					{
//						FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
					
						if(hour_pm_flag==1)
						{
								
								FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
						
						}
						else 
						{
							
								FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;
						
						}
						if(SET_Long_Flag==1)
						{
							if(alarm1_pm_flag==1&&flag_display==2 )
							{

									FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
								
							}
							if(alarm1_pm_flag==0&&flag_display==2)
							{
									FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;
														
							}				


							if(alarm2_pm_flag==1&&flag_display==3)
							{

									FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
								
							}
							 if(alarm2_pm_flag==0&&flag_display==3)
							{
										FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;
														
							}	

							
							if(alarm3_pm_flag==1&&flag_display==4)
							{

									FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
								
							}	
							if(alarm3_pm_flag==0&&flag_display==4)
							{
								FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;
														
							}	
							if(flag_display==0)
							{
									if(SET_Short_Flag_hur_min==1||SET_Short_Flag_hur_min==2)FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<1) ;
							}
							 if(flag_display==2||flag_display==3||flag_display==4)
							{
									if(SET_Short_Flag_alarm1_flag==0||SET_Short_Flag_alarm1_flag==1)FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<1) ;
							}
							
						}
//					 if(SET_Long_Flag==1&&(flag_display==2||flag_display==3||flag_display==4))
//					{
//								FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<1) ;
//												
//					}						
						if(SET_Long_Flag==0&&(flag_display==0||flag_display==1||flag_display==5)&&(hour_pm_flag==1))
						{
								FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
						
						}
				
					}
					else
					{

//								FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;
						//		FD612_Refresh();
					}
					if(alarm1_on_dispaly ||alarm2_on_dispaly ||alarm3_on_dispaly)
					{
						FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<3) ;
					}
					else
					{
						FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<3) ;
					}
//					if()
					FD612_Refresh();
}



void after_arrive_write_1302(void)
{
//			 if(time.hour==12)
//		 {
				hour_chage_state=Read_addr_byte_ds1302(hour_r_addr);
//			ResetDS1302();							
//			 WriteByteDS1302(hour_r_addr); //读取时的地址
//			 hour_chage_state=ReadByteDS1302();
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
			Write_addr_byte_ds1302(hour_w_addr,hour_chage_state);
//			 ResetDS1302();
//			 WriteByteDS1302(hour_w_addr); //写入时的地址
//			 WriteByteDS1302(hour_chage_state);													 
//			}
}

	void hour_chage_input(UINT8 i)
	{//24小时转12小时
		
		UINT8 hour_state_value;
		hour_state_value=Read_addr_byte_ds1302(hour_r_addr);
//		ResetDS1302();							
//		WriteByteDS1302(hour_r_addr); //读取时的地址
//		hour_state_value=ReadByteDS1302();
		if(i==1)		
		{
//			hour_state_value<<=3;
//			hour_state_value>>=3;
						
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
//		if(hour_chage_state==12)
//		{
//		 hour_pm_flag=1;
//		hour_chage_state |=(1<<5);
//		}
//		hour_chage_state=decimal_bcd(hour_chage_state);
				
		}
		if(i==0)		
		{
			//12小时转24小时
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
		Write_addr_byte_ds1302(hour_w_addr,hour_state_value);
//		ResetDS1302();
//		WriteByteDS1302(hour_w_addr); //写入时的地址
//		WriteByteDS1302(hour_state_value);	
	}
	
	void temper_display(void)
	{
					
		if(ADC_TIME_Flag==temp_refresh_time)
		{
			ADC_Convert( );
			ADC_TIME_Flag=0;
		}
			adc_switch( );
//		if(ADC_Display1==0)
//		{			
////		FD612_display_4DIG_input( 0x00 ,dis[(unsigned char)TEM2/10] ,dis[(unsigned char)TEM2%10] ,FD612_DISP_C );
//			FD612_PotiveTNage(FD612_DIG1_ADDR,0x00);
//			FD612_PotiveTNage(FD612_DIG2_ADDR,dis[(unsigned char)TEM2/10]);
//			FD612_PotiveTNage(FD612_DIG3_ADDR,dis[(unsigned char)TEM2%10]);
//			FD612_PotiveTNage(FD612_DIG4_ADDR,FD612_DISP_cc);
//			FD612_DispBuff.DispData[FD612_DIG12_ADDR]|= 1<<6 ; 
//			FD612_Refresh();
//		}
//		else
//		{
//		ADC_AVG_NUM[2]   = (unsigned char)TEM4/100;
//		ADC_AVG_NUM[1]   = ((unsigned char)TEM4-ADC_AVG_NUM[2]*100 )/10;
//		ADC_AVG_NUM[0]   = ((unsigned char)TEM4-ADC_AVG_NUM[2]*100 )%10;

////		FD612_display_4DIG_input(dis[ADC_AVG_NUM[2]]  , dis[ADC_AVG_NUM[1]],dis[ADC_AVG_NUM[0]]  ,FD612_DISP_F );
//			FD612_PotiveTNage(FD612_DIG1_ADDR,dis[ADC_AVG_NUM[2]]);
//			FD612_PotiveTNage(FD612_DIG2_ADDR,dis[ADC_AVG_NUM[1]]);
//			FD612_PotiveTNage(FD612_DIG3_ADDR,dis[ADC_AVG_NUM[0]] );
//			FD612_PotiveTNage(FD612_DIG4_ADDR,FD612_DISP_F );

//			FD612_Refresh();
//			
//		}	
	}
	
void display_12_24_blink(void)
{
			if(time1_second==1)
		{
		if(blink_flag)					
			{
				blink_flag=0;
				if(hour_12_24_flag)
				{
				FD612_DISPLAY_2DIG_input(1 ,dis[1],dis[2]);
				}
				else
				{
				FD612_DISPLAY_2DIG_input(1 ,dis[2],dis[4]);
				}

				FD612_DISPLAY_2DIG_input(2,FD612_DISP_H ,FD612_DISP_);	
			}
			else
			{
				blink_flag=1;
				FD612_display_4DIG_input(0x00,0x00,0x00,0x00);
			}
			time1_second=0;
		}	
}

void year_blink(UINT8 i)
{
		if(time1_second==1)
		{
			if(blink_flag)
			{
			blink_flag=0;
				FD612_display_4DIG_input( dis[2] ,dis[0] ,dis[time.year/10]  ,dis[time.year%10] );
			}
			else
			{
				blink_flag=1;
				if(i==0)
				FD612_display_4DIG_input( 0x00 ,0x00 ,0x00 ,0x00 );
			}
			time1_second=0;
		}
}


void from_ds1302_read_time_data(void)
{
	//				UINT8	pm_flag;
			ResetDS1302();	
			WriteByteDS1302(0xbf);																					//以多字节方式读取时钟寄存器数据
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
//				time.hour &=~(1<<7);
//				time.hour &=~(1<<5);
//				 ResetDS1302();							
//				 WriteByteDS1302(hour_r_addr); //读取时的地址
//				 pm_flag=ReadByteDS1302();
//				 if((pm_flag&0x20)&&(pm_flag&0x80))
//				 { 
//					 hour_pm_flag=1;
//				 }
//				 else 
//				 {								 
//					 hour_pm_flag=0;
//				 }				
			}
			time.year = bcd_decimal(time.year); 
			time.month =bcd_decimal(time.month);
			time.mday = bcd_decimal(time.mday);
			time.week = bcd_decimal(time.week);
			time.hour = bcd_decimal(time.hour);
			time.minute = bcd_decimal(time.minute);
			time.second = bcd_decimal(time.second);
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
//			WriteByteDS1302(0x8e);		                      //写保护控制寄存器
//			WriteByteDS1302(0x0);														//允许写入
			
			ResetDS1302();
			WriteByteDS1302(0x90);													//写涓流充电控制寄存器
			WriteByteDS1302(0xab);													//允许充电
			
			ResetDS1302();
			WriteByteDS1302(0xbe);													//写多字节突发方式控制寄存器
			
			WriteByteDS1302(second);
			WriteByteDS1302(minute);
			WriteByteDS1302(hour);
			WriteByteDS1302(mday);
			WriteByteDS1302(month);
			WriteByteDS1302(week);
			WriteByteDS1302(year);
			WriteByteDS1302(0);															//对写保护控制寄存器写入0
			ResetDS1302();
			ds1302_w_off();
		
//			from_ds1302_read_time_data();
			
					
}



void point_blink(void)
{
											FD612_PotiveTNage(FD612_DIG1_ADDR,dis[time.hour/10]);
										FD612_PotiveTNage(FD612_DIG2_ADDR,dis[time.hour% 10]);
										FD612_PotiveTNage(FD612_DIG3_ADDR,dis[time.minute/10]);
										FD612_PotiveTNage(FD612_DIG4_ADDR,dis[time.minute% 10]);
																

								if(time1_second)
								{
//									pm_12_24_alarm_display();
									if(blink3_flag)
									{
										blink3_flag=0;
//										FD612_PotiveTNage(FD612_DIG1_ADDR,dis[time.hour/10]);
//										FD612_PotiveTNage(FD612_DIG2_ADDR,dis[time.hour% 10]);
//										FD612_PotiveTNage(FD612_DIG3_ADDR,dis[time.minute/10]);
//										FD612_PotiveTNage(FD612_DIG4_ADDR,dis[time.minute% 10]);
										FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<1);
										 if(hour_pm_flag==1)
										{												
												FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
										}
										else 
										{											
												FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;										
										}
										if(alarm1_on_dispaly ||alarm2_on_dispaly ||alarm3_on_dispaly)
										{
											FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<3) ;
										}
										else
										{
											FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<3) ;
										}										
//										FD612_Refresh();
									}
									else
									{
										blink3_flag=1;
//										FD612_PotiveTNage(FD612_DIG1_ADDR,dis[time.hour/10]);
//										FD612_PotiveTNage(FD612_DIG2_ADDR,dis[time.hour% 10]);
//										FD612_PotiveTNage(FD612_DIG3_ADDR,dis[time.minute/10]);
//										FD612_PotiveTNage(FD612_DIG4_ADDR,dis[time.minute% 10]);
										FD612_DispBuff.DispData[FD612_DIG12_ADDR] |= (1<<1);
										 if(hour_pm_flag==1)
										{  
												FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;										
										}
										else 
										{											
												FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;										
										}
										if(alarm1_on_dispaly ||alarm2_on_dispaly ||alarm3_on_dispaly)
										{
											FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<3) ;
										}
										else
										{
											FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<3) ;
										}										
//										FD612_Refresh();
									}
									FD612_Refresh();
									time1_second=0;
									
								}
}

void adc_switch(void)
{
						switch (ADC_Display)
					 {
						 
						 case 0:
						 {
								
							 
							 //FD612_display_4DIG_input(0x00,dis[(unsigned char)TEM2/10],dis[(unsigned char)TEM2%10],FD612_DISP_C);	 
								FD612_PotiveTNage(FD612_DIG1_ADDR,0x00);
								FD612_PotiveTNage(FD612_DIG2_ADDR,dis[(unsigned char)TEM2/10]);
								FD612_PotiveTNage(FD612_DIG3_ADDR,dis[(unsigned char)TEM2%10]);
								FD612_PotiveTNage(FD612_DIG4_ADDR,FD612_DISP_cc);
								FD612_DispBuff.DispData[FD612_DIG12_ADDR]|= 1<<6 ; 
								FD612_Refresh();
							 ADC_Display1=0;
							 break;
						 }
							

							case 1:
							{
								
//							ADC_AVG_NUM[2]   = (unsigned char)TEM4/100;
//							ADC_AVG_NUM[1]   = ((unsigned char)TEM4-ADC_AVG_NUM[2]*100 )/10;
//							ADC_AVG_NUM[0]   = ((unsigned char)TEM4-ADC_AVG_NUM[2]*100 )%10;
//							FD612_display_4DIG_input(dis[ADC_AVG_NUM[2]],dis[ADC_AVG_NUM[1]],dis[ADC_AVG_NUM[0]],FD612_DISP_F);
							FD612_PotiveTNage(FD612_DIG1_ADDR,dis[(unsigned char)TEM4/100]);
							FD612_PotiveTNage(FD612_DIG2_ADDR,dis[(unsigned char)TEM4%100/10]);
							FD612_PotiveTNage(FD612_DIG3_ADDR,dis[(unsigned char)TEM4%100%10]);
								FD612_PotiveTNage(FD612_DIG4_ADDR,FD612_DISP_F);

							FD612_Refresh();	
								ADC_Display1=1;
//								ADC_Display=0;
								break;
							}	
								default :
								break;
					}
}