
#include "usr.h"


struct DATA_ALARM alarm1,alarm2,alarm3;//ƒ÷÷”≥ı º÷µ
struct DATA_ALARM_ch ch_alarm1,ch_alarm2,ch_alarm3;
struct DATA_TIME time;									// ±º‰≥ı º÷µ
struct time_char time_ch;								// ±º‰œ‘ æ≥ı º÷µ



unsigned char second,time1_second=0,long_flag;
unsigned int delaytime =40,dc2=0,delaytime1=10,dc1=0; //20¥Œ1S
unsigned char SET_Long_Flag=0,SET_Short_Flag=0,UP_Long_Flag=0,UP_Short_Flag=0,DOWN_Short_Flag=0,DOWN_Long_Flag=0;
unsigned char flag_display=0,display_flag_hur_min=1,SET_Short_Flag_hur_min=0;
const unsigned char  dis[10] = {FD612_DISP_0,FD612_DISP_1,FD612_DISP_2,FD612_DISP_3,FD612_DISP_4,FD612_DISP_5,FD612_DISP_6,FD612_DISP_7,FD612_DISP_8,FD612_DISP_9 };
//unsigned char  TEM_VALUE =1;
unsigned char  ADC_TIME_Flag=0,dc_ADC=0,delaytime_ADC=20;//20¥Œ1S
unsigned char display_flag_mon_day=1,SET_Short_Flag_mon_day=0,SET_Short_Flag_alarm1_flag=0,SET_Short_Flag_year=0;
unsigned char  flag_year_leap=0;//»ÚƒÍµƒ¥˙±Ì
//unsigned char alarm_on_flag=0,alarm1_on_dispaly=0,alarm2_on_dispaly=0,alarm3_on_dispaly=0;
unsigned char  back_TIME_Flag=0,dc_back=0,delaytime_back=20,back_first=1,voice_back=0,back_voice_Flag=0,alarm_clock_flag=0,dc_alarm=0;
//unsigned char alarm_flag=0,alarm_voice_flag=0,alarm3_voice_flag=0,alarm2_voice_flag=0,alarm3_flag=0,alarm2_flag=0;
unsigned char voice_flag=0,voice_fisrt=1,alarm_voice_flag=0;
unsigned char sd_flag=0,dislay_all_flag=0,sd_mode_flag=0,sd_select_first=1;
float   TEM2,TEM4;
double	 TEM;				//∏°µ„¿‡–Õ
unsigned char  ADC_Display=0,ADC_Display1=0,ADC_ONE=1;
unsigned char key_first=1;
unsigned char mode_select_first=1,mode_select_flag=2,hor_min_delay_time=0,mon_day_delay_time=0,mode_select=0,temp_delay_time=0;
UINT8 long_add_flag=0,up_down_flag=0;
//UINT8 alarm_hour_long_up_down_flag=0,alarm_min_long_up_down_flag=0;
UINT8 blink_flag=0,blink3_flag=0;
UINT8 hour_chage_state=0,hour_12_24_flag=0,hour_pm_flag=0,alarm1_pm_flag=0,alarm2_pm_flag=0,alarm3_pm_flag=0;
void	mTimer0Interrupt( void ) interrupt INT_NO_TMR0 using 1                //timer0÷–∂œ∑˛ŒÒ≥Ã–Ú, π”√ºƒ¥Ê∆˜◊È1
{                                                                           //∑Ω Ω3 ±£¨TH0 π”√Timer1µƒ÷–∂œ◊ ‘¥

				TH0=0XFf;
				TL0=0X01;
				buzzer=~buzzer;



}

void	mTimer1Interrupt( void ) interrupt INT_NO_TMR1 using 2                //timer1÷–∂œ∑˛ŒÒ≥Ã–Ú, π”√ºƒ¥Ê∆˜◊È2
{                                                                           //∑Ω Ω3 ±£¨Timer1Õ£÷π
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
#if mode_select_enable
		mode_select++;
		if(mode_select==delaytime_back)
		{
			if(flag_display==1)
			hor_min_delay_time++;
			if(flag_display==0)
				mon_day_delay_time++;
//			if(flag_display==5)
//			temp_delay_time++;
				mode_select=0;
			
		}
#endif
}


/**
 *  @brief  BCD◊™ ÆΩ¯÷∆
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
 *  @brief   ÆΩ¯÷∆◊™BCD
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
 *  @brief  ≈–∂œ «∑Ò «»ÚƒÍ
 *  @param  year
 *  @return 1¥˙±Ì»ÚƒÍ 0¥˙±Ì≤ª «»ÚƒÍ
 *  @note    
 *  @note    
 */


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
		//buzzer=0;
}

void time0_setup(void)
{
		long_flag=0;
		back_first=1;
		delaytime =40;
		second=0;
		dc2=0;


		back_TIME_Flag=0;
#if mode_select_enable
	if(mode_select_flag==0)
	{
//		temp_delay_time=0; //DP1º∆ ˝«Â0
		mon_day_delay_time=0;
		hor_min_delay_time=0;	
	}
#endif
	  TR0=1;
		mDelaymS(70);
		TR0=0;
		buzzer=0;

		back_voice_Flag=0; //ªΩ–—π¶ƒ‹—” ±÷ÿ–¬º∆ ˝
			
}

void key_statue(void)
{
		from_ds1302_read_time_data( );
				
if(alarm1.alarm_flag==0 && alarm2.alarm_flag==0&& alarm3.alarm_flag==0)	{
//set∞¥º¸
	if(SET==0)
	{
		time0_setup( );
		while(!SET)
		{
	//		pm_12_24_alarm_display();
			/*SET∞¥œ¬ ±√Îªπ‘⁄Ã¯*/
			if(flag_display==0&&SET_Long_Flag==0)
			{
				point_blink();
			}
			

			if(second==1)
			{
				
				long_flag=1;
				SET_Long_Flag=1;
				if(dislay_all_flag==1){dislay_all_flag=0;sd_select_first=1;}
				switch(flag_display)
				{
					case 0:
					{
						display_12_24_blink( );
						break;
					}
					case 1:
					{
								ui_mon_display();
		
								all_blink(0,1,time.mday);
					}
					case 2:
					{
							year_blink(0)	;					
							break;
					}
					case 3:
					{
            alarm_long_blink(alarm1)	;				
						break;
					}
					case 4:
					{
						alarm_long_blink(alarm2)	;
						break;
					}
					case 5:
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
//up∞¥º¸
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
				return;
				long_flag=1;
				UP_Long_Flag=1;
				long_add_flag=1;//≥§∞¥ ±œ‘ æ
				
				if(SET_Long_Flag==1)
				{
					back_TIME_Flag=0;
					if(mode_select_flag==0)
					{
						temp_delay_time=0; //DP1º∆ ˝«Â0
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
								mday_long_up_down(0);
						 }	
					   if(SET_Short_Flag_mon_day==1)
						 {
							 mon_long_up_down(0);
							
						 }						 
						break;
					}
					case 2:
					{
						if(SET_Short_Flag_year==0)
						year_long_up_down(0);
						break;
					}
					case 3:
					{
							if(SET_Short_Flag_alarm1_flag==0)	
							{
								alarm_hour_up_down2(1,0,&alarm1);

							}		
							if(SET_Short_Flag_alarm1_flag==1)	
							{
								  alarm_min_up_down2(1,0,&alarm1);
//								 alarm_min_long_up_down(1,1,alarm1);
							}						
						break;
					}
					case 4:
					{
						if(SET_Short_Flag_alarm1_flag==0)	
						{
							alarm_hour_up_down2(1,0,&alarm2);
						}
						if(SET_Short_Flag_alarm1_flag==1)	
						{
							alarm_min_up_down2(1,0,&alarm2);
//							 alarm_min_long_up_down(2,1,alarm2);
						}							
						break;
					}
					case 5:
					{
						if(SET_Short_Flag_alarm1_flag==0)	
						{
							alarm_hour_up_down2(1,0,&alarm3);
						}
						if(SET_Short_Flag_alarm1_flag==1)	
						{
							alarm_min_up_down2(1,0,&alarm3);
//							 alarm_min_long_up_down(3,1, alarm3);
						}						
						break;
					}		
				}

							
				}
			}
	
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

//down∞¥º¸
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
						temp_delay_time=0; //DP1º∆ ˝«Â0
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
							mday_long_up_down(1);
						 }
					   if(SET_Short_Flag_mon_day==1)
						 {
							
							 mon_long_up_down(1);
						 }						 
						
							break;
						}
							case 2:
							{
								if(SET_Short_Flag_year==0)
								year_long_up_down(1);
								break;
							}						
						case 3:
						{
							if(SET_Short_Flag_alarm1_flag==0)	
							{

								alarm_hour_up_down2(1,1,&alarm1);
								
							}
							if(SET_Short_Flag_alarm1_flag==1)	
							{
//								 alarm_min_long_up_down(1,2,alarm1);
								alarm_min_up_down2(1,1,&alarm1);
							}								
							break;
						}
						case 4:
						{
							if(SET_Short_Flag_alarm1_flag==0)	
							{

								alarm_hour_up_down2(1,1,&alarm2);;
							}
							if(SET_Short_Flag_alarm1_flag==1)	
							{
//								 alarm_min_long_up_down(2,2, alarm2);
								alarm_min_up_down2(1,1,&alarm2);
							}							
							break;
						}
						case 5:
						{
							if(SET_Short_Flag_alarm1_flag==0)	
							{

								alarm_hour_up_down2(1,1,&alarm3);
							}
							if(SET_Short_Flag_alarm1_flag==1)	
							{
//								 alarm_min_long_up_down(3,2, alarm3);
								alarm_min_up_down2(1,1,&alarm3);
							}												
							break;
						}
						default :
						break;
												
					}

						
				}	
				if(SET_Long_Flag==0&&ADC_ONE==1)
				{
					if(ADC_Display==0)ADC_Display=1;
					else ADC_Display=0;
					  adc_switch( );

					ADC_ONE=0;
				}
			}
	
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
key_off_alarm_clock(&alarm1);
key_off_alarm_clock(&alarm2);
key_off_alarm_clock(&alarm3);



}
void display(void)
{
//	UINT8 pm_flag;
		temper_display();
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

			}
		}

		}

		switch(flag_display)
		{
			case 0:
				{

							if(display_flag_hur_min)
							{
									point_blink();																														
							}
							if(SET_Long_Flag)
							{
								key_first_into();
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
								switch(SET_Short_Flag_hur_min)
								{
									case 0:
									{
										display_12_24_blink( );	
										if(UP_Short_Flag==1)
										{
											key_12and24_switch();
											UP_Short_Flag=0;																																									
										}										
										break;
									}
									case 1:
									{
										FD612_DISPLAY_2DIG_input(2, dis[time.minute/10] ,dis[time.minute%10]);	
										all_blink(0,1,time.hour);
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
									case 2:
									{
											FD612_DispBuff.DispData[12]|=1<<2;
											FD612_DISPLAY_2DIG_input(1 ,dis[time.hour/10]  ,dis[time.hour%10]);
											all_blink(0,2,time.minute);
											if(UP_Short_Flag == 1)
											{
													time.minute++;
												if(time.minute >=60)
												{
													time.minute = 0;
												}
													UP_Short_Flag = 0;
												
												Write_addr_byte_ds1302(sec_w_addr,0);

											}
											if(DOWN_Short_Flag == 1)
											{
													time.minute--;
												if(time.minute>60)
												{
														time.minute = time.minute -196; //“Á≥ˆ÷µ
												}
													DOWN_Short_Flag = 0;
													Write_addr_byte_ds1302(sec_w_addr,0);


											}
											time.minute = decimal_bcd(time.minute);										
											Write_addr_byte_ds1302(min_w_addr,time.minute);										
											break;
									}
									case 3:
									{
										display_flag_hur_min=1;
										SET_Long_Flag=0;
										SET_Short_Flag_hur_min=0;
										key_first=1;
										break;
									}									
								}
																								
								timeout_exit();
															
							}
							action_remove();
							break;					
				}

				case 1:
					{
						if(display_flag_mon_day)
							{
								ui_mon_display();
								FD612_DISPLAY_2DIG_input(1,dis[time.mday/10],dis[time.mday% 10]);
								
							}										
					if(SET_Long_Flag)
					{
						key_first_into();						
						display_flag_mon_day=0;
					
					if(SET_Short_Flag)
								{
									SET_Short_Flag_mon_day++;
									SET_Short_Flag=0;
									if(SET_Short_Flag_mon_day>2)
									{
										SET_Short_Flag_mon_day=0;
									}
								}
						switch(SET_Short_Flag_mon_day)
							{
								case 0:
								{
//										FD612_DISPLAY_2DIG_input(2 ,dis[time.month/10] ,dis[time.month%10] );
												ui_mon_display();
//												mday_blink(0);
									
										all_blink(0,1,time.mday);						 
										if(UP_Short_Flag >= 1)
										{
												time.mday++;
																	           
												mon_over_judge();
											UP_Short_Flag = 0;						
										}	
										if(DOWN_Short_Flag >= 1)
										{
												time.mday--;
										    mon_over_judge();											
																																									
												DOWN_Short_Flag = 0;
										}					
										time.mday = decimal_bcd(time.mday);
										Write_addr_byte_ds1302(day_w_addr,time.mday);									
										break;
								}
								case 1:
								{
										FD612_DISPLAY_2DIG_input(1,dis[time.mday/10] ,dis[time.mday%10] );
										if(time1_second)
										{
										if(blink_flag)
										{
											blink_flag=0;	
												ui_mon_display();
										}
										else
										{
											blink_flag=1;
											FD612_DISPLAY_2DIG_input(2 ,0x00  ,0x00 );
										}
										time1_second=0;
										}
										if(UP_Short_Flag == 1)
										{
												time.month++;
												mday_over_judge();
												UP_Short_Flag = 0;
										}
										if(DOWN_Short_Flag == 1)
										{
												time.month--;
												mday_over_judge();
												DOWN_Short_Flag = 0;
										}	
										time.month = decimal_bcd(time.month);

										Write_addr_byte_ds1302(mon_w_addr,time.month);									
										break;
								}
								case 2:
								{
										display_flag_mon_day=1;
										SET_Long_Flag=0;
										SET_Short_Flag_mon_day=0;
										key_first=1;
										break;
								}								
							}

								timeout_exit();							
							}
						if(SET_Long_Flag==0)
						{
							timeout_exit();	
						}							

						action_remove();								
						break;						
					}
				case 2:
				{
					if(SET_Long_Flag==1)
					{
						key_first_into();
						year_blink(0);
					if(SET_Short_Flag)
						{
							SET_Short_Flag_year++;
							SET_Short_Flag=0;
							if(SET_Short_Flag_year>1)
							{
								SET_Short_Flag_year=0;
							}
						}	
						switch(SET_Short_Flag_year)
						{
							case 0:
							{
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
								break;
							}
							case 1:
							{									
									SET_Long_Flag=0;
									SET_Short_Flag_year=0;
									key_first=1;

								break;
								
							}
						}
						time.year= decimal_bcd(time.year);
						Write_addr_byte_ds1302(year_w_addr,time.year);
						timeout_exit();						
					}
					else
					{
						FD612_display_4DIG_input( dis[2] ,dis[0] ,dis[time.year/10]  ,dis[time.year%10] );
						timeout_exit();
					}
					action_remove();
					break;
					
				}					
				case 3:
					{
						
						alarm_set_display(1, &alarm1);
						break;	
					}
				case 4:
					{

						alarm_set_display(2, &alarm2);
						break;	
					}
				case 5:
					{

						alarm_set_display(3, &alarm3);
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
		unsigned int B_value = 3950;//ø∆“’¥´∏–∆˜

		ADC_CHAN1 =0;
		ADC_CHAN0=0;
		P1_DIR_PU &= ~bAIN0;	
		ADC_START = 1;  
		while(ADC_START);                                                      //ADC_START±‰Œ™0 ±£¨±Ì æ≤…—˘ÕÍ≥…
		ADC_VALUE = ADC_DATA * VCC / 255; // ∂¡»°◊™ªªµƒAD÷µ		
	
		TEM = 1/(log(ADC_VALUE/(VCC - ADC_VALUE))/B_value + 1/(273.15 + 25));
			
		TEM4=TEM2*1.8+32;
		TEM2=TEM- 273.15;
		
}

#if 0
void alarm1_sleep(void)
{
		UINT8 alarm1_value1,alarm1_value2,alarm1_value3,alarm1_value4,alarm1_value5;
		if(time.second==0 && time.hour==alarm1.hour && time.minute==alarm1.minute&&hour_pm_flag==alarm1.alarm_pm_flag)
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
		if(time.second==0 && time.hour==alarm2.hour && time.minute==alarm2.minute&&hour_pm_flag==alarm2.alarm_pm_flag)
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
		if(time.second==0 && time.hour==alarm3.hour && time.minute==alarm3.minute&&hour_pm_flag==alarm3.alarm_pm_flag)
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
#endif
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

void alarm_display (void)
{
	 if(alarm1.alarm_on_dispaly)
 {
//		alarm1_sleep( );
	   alarm_sleep_judge(&alarm1);
 }
 	 if(alarm2.alarm_on_dispaly)
 {
//		alarm2_sleep( );
	 alarm_sleep_judge(&alarm2);
 }
 	 if(alarm3.alarm_on_dispaly)
 {
//		alarm3_sleep( );
	 alarm_sleep_judge(&alarm3);
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
		up_down_flag=1;//up_downº¸”Îƒ£ Ω—°‘Ò≥ÂÕª
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

			dislay_all_flag=0;
			back_TIME_Flag=0;
			back_first=1;	
			UP_Short_Flag=0;
			SET_Short_Flag=0;
//			mode_select_first=1;
			sd_select_first=1;
//			up_down_flag=0;
			

		}					
}

#if mode_select_enable
void Display_Mode_Select(void)
{
		if(UP_Short_Flag)
		{
				up_down_flag=1;//up_downº¸”Îƒ£ Ω—°‘Ò≥ÂÕª
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
				flag_display=0;
				hor_min_delay_time=0;
				
			}
			if(mon_day_delay_time==over_time_10s)
			{
				flag_display=1;
				mon_day_delay_time=0;
			}
//			if(temp_delay_time==over_time_2s)
//			{
//				flag_display=0;
//				temp_delay_time=0;
//			}	
		}
			
		}
}

#endif

void year_long_up_down(UINT8 i)
{
//			ResetDS1302();
//			WriteByteDS1302(year_r_addr); //∂¡»°∑÷µƒµÿ÷∑				
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
//			WriteByteDS1302(year_w_addr); //–¥»Î ±µƒµÿ÷∑
//			WriteByteDS1302(time.year);
//			ResetDS1302();	
			mDelaymS(up_down_delay_time);	
			time.year = bcd_decimal(time.year);
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
					hour_over_judge(i);
				
				}
				if(i==1)
				{ 
						time.hour--;
						hour_over_judge(i);					
					
				}
				if(hour_12_24_flag){time.hour<<=3;time.hour>>=3;}
				time.hour = bcd_decimal(time.hour);
				FD612_DISPLAY_2DIG_input(1 ,dis[time.hour/10]  ,dis[time.hour% 10] );

			 

			 mDelaymS(up_down_delay_time);
			
				

	
}

void min_long_up_down(UINT8 i) 
{

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
					time.minute = time.minute -196; //“Á≥ˆ÷µ
			}	
			}

			FD612_DISPLAY_2DIG_input(2,dis[time.minute/10] ,dis[ time.minute% 10] );
			time.minute = decimal_bcd(time.minute);

				Write_addr_byte_ds1302(min_w_addr,time.minute);
				Write_addr_byte_ds1302(sec_w_addr,0);
		

			time.minute = bcd_decimal(time.minute);
		  mDelaymS(up_down_delay_time);
}
void mon_long_up_down(UINT8 i)
{

			time.month=Read_addr_byte_ds1302(mon_r_addr);
			time.month = bcd_decimal(time.month);	
				time.mday=Read_addr_byte_ds1302(day_r_addr);
			time.mday = bcd_decimal(time.mday);
	
			if(i==0)
			{
				time.month++;

				mday_over_judge();
			}
			if(i==1)
			{
			time.month--;

			mday_over_judge();	
			}	
	
			ui_mon_display();
//			FD612_DISPLAY_2DIG_input(1 ,dis[time.month/10] , dis[time.month% 10]);
			time.month = decimal_bcd(time.month);

				Write_addr_byte_ds1302(mon_w_addr,time.month);



			time.month = bcd_decimal(time.month);			
		  mDelaymS(up_down_delay_time);			
}

void mday_long_up_down(UINT8 i)
{

			time.mday=Read_addr_byte_ds1302(day_r_addr);
			time.mday = bcd_decimal(time.mday);
			if(i==0)
			{
				time.mday++;
				mon_over_judge();
			}
			if(i==1)
			{
			time.mday--;
				mon_over_judge();	
			}	

				
				FD612_DISPLAY_2DIG_input(1,dis[time.mday/10] ,dis[time.mday% 10] );
				time.mday = decimal_bcd(time.mday);

				Write_addr_byte_ds1302(day_w_addr,time.mday);

	
				time.mday = bcd_decimal(time.mday);
				mDelaymS(up_down_delay_time);			
}

void alarm_hour_up_down2(UINT8 long_key_enble,UINT8 up_down_seclect,struct DATA_ALARM * alarm)
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
	 FD612_DISPLAY_2DIG_input(1 , dis[alarm->hour/10],dis[alarm->hour%10] );
//	FD612_DISPLAY_2DIG_input(2,0x00 ,dis[alarm->alarm_pm_flag] );
	 if(long_key_enble) mDelaymS(up_down_delay_time);
}
//void alarm_hour_long_up_down(UINT8 alarm_seclect,UINT8 up_down_seclect,struct DATA_ALARM * alarm)
//{
//	
//	if(up_down_seclect==1)
//	{
//				if(alarm_seclect==1)
//				{
//					
//					alarm->hour=alarm->hour+1;
//					if(hour_12_24_flag){
//						 if(alarm1.hour>=13)
//						 {
//							 alarm1.hour = 1;
//						 }
//						 if(alarm1.hour==1)
//						{
//							if(alarm1_pm_flag)alarm1_pm_flag=0;
//							else alarm1_pm_flag=1;						
//						}
//						}
//					else
//					{
//						if(alarm1.hour >=24)
//						{
//							alarm1.hour = 0;
//						}	
//					}
//					alarm_hour_long_up_down_flag=1;
//				}
//				if(alarm_seclect==2)
//				{
//					alarm2.hour++;
//					if(hour_12_24_flag){
//						 if(alarm2.hour>=13)
//						 {
//							 alarm2.hour = 1;

//						 }
//						if(alarm2.hour==1)
//						{
//							if(alarm2_pm_flag)alarm2_pm_flag=0;
//							else alarm2_pm_flag=1;						
//						}						 
//						}
//					else
//					{
//						if(alarm2.hour >=24)
//						{
//							alarm2.hour = 0;
//						}	
//					}
//					alarm_hour_long_up_down_flag=2;
//				}	
//				if(alarm_seclect==3)
//				{
//					alarm3.hour++;
//					if(hour_12_24_flag){
//						 if(alarm3.hour>=13)
//						 {
//							 alarm3.hour = 1;

//						 }
//						if(alarm3.hour==1)
//						{
//							if(alarm3_pm_flag)alarm3_pm_flag=0;
//							else alarm3_pm_flag=1;						
//						}							 
//						}
//					else
//					{
//						if(alarm3.hour >=24)
//						{
//							alarm3.hour = 0;
//						}	
//					}
//					alarm_hour_long_up_down_flag=3;
//				}
//		}
//		if(up_down_seclect==2)
//		{
//			if(alarm_seclect==1)
//			{
//				alarm1.hour--;					
//				if(hour_12_24_flag){
//					 if(alarm1.hour>=13||alarm1.hour==0)
//					 {
//							alarm1.hour = alarm1.hour -244; //“Á≥ˆ÷µ
//						
//					 }
//					 	if(alarm1.hour==10)
//						{
//							if(alarm1_pm_flag)alarm1_pm_flag=0;
//							else alarm1_pm_flag=1;						
//						}	
//					}
//				else
//				{
//					if(alarm1.hour >=24)
//					{
//						alarm1.hour = alarm1.hour -232; //“Á≥ˆ÷µ
//					}
//				}	
//			alarm_hour_long_up_down_flag=4;
//			}
//			if(alarm_seclect==2)
//			{
//				alarm2.hour--;					
//				if(hour_12_24_flag){
//					 if(alarm2.hour>=13||alarm2.hour==0)
//					 {
//							alarm2.hour = alarm2.hour -244; //“Á≥ˆ÷µ

//					 }
//					 		if(alarm2.hour==10)
//						{
//							if(alarm2_pm_flag)alarm2_pm_flag=0;
//							else alarm2_pm_flag=1;						
//						}	
//					}
//				else
//				{
//					if(alarm2.hour >=24)
//					{
//						alarm2.hour = alarm2.hour -232; //“Á≥ˆ÷µ
//					}
//				}	

//			alarm_hour_long_up_down_flag=5;
//			}
//			if(alarm_seclect==3)
//			{
//				alarm3.hour--;					
//				if(hour_12_24_flag){
//					 if(alarm3.hour>=13||alarm3.hour==0)
//					 {
//							alarm3.hour = alarm3.hour -244; //“Á≥ˆ÷µ
//					 }
//					 	if(alarm3.hour==10)
//						{
//							if(alarm3_pm_flag)alarm3_pm_flag=0;
//							else alarm3_pm_flag=1;						
//						}	
//					}
//				else
//				{
//					if(alarm3.hour >=24)
//					{
//						alarm3.hour = alarm3.hour -232; //“Á≥ˆ÷µ
//					}
//				}	
//			alarm_hour_long_up_down_flag=6;
//			}		
//		}

//					
//				FD612_DISPLAY_2DIG_input(1 , dis[alarm->hour/10],dis[alarm->hour%10] );
//				mDelaymS(up_down_delay_time);
//				
//}

void alarm_min_up_down2(UINT8 long_key_enble,UINT8 up_down_seclect,struct DATA_ALARM* alarm)
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
	FD612_DISPLAY_2DIG_input(2,dis[alarm->minute/10]  ,dis[alarm->minute%10] );
	if(long_key_enble)mDelaymS(up_down_delay_time);
}
//void alarm_min_long_up_down(UINT8 alarm_seclect,UINT8 up_down_seclect,struct DATA_ALARM alarm)
//{
//	if(up_down_seclect==1)
//	{
//				if(alarm_seclect==1)
//				{
//					alarm1.minute++;
//				if(alarm1.minute >=60)
//				{
//					alarm1.minute = 0;
//				}
//				alarm_min_long_up_down_flag=1;
//				}
//				if(alarm_seclect==2)
//				{
//					alarm2.minute++;
//				if(alarm2.minute >=60)
//				{
//					alarm2.minute = 0;
//				}
//				alarm_min_long_up_down_flag=2;				
//				}
//				if(alarm_seclect==3)
//				{
//					alarm3.minute++;
//				if(alarm3.minute >=60)
//				{
//					alarm3.minute = 0;
//				}
//				alarm_min_long_up_down_flag=3;				
//				}	
//		}
//		if(up_down_seclect==2)
//		{
//				if(alarm_seclect==1)
//				{
//					alarm1.minute--;
//				if(alarm1.minute>60)
//				{
//						alarm1.minute = alarm1.minute -196; //“Á≥ˆ÷µ
//				}
//				alarm_min_long_up_down_flag=4;				
//				}
//				if(alarm_seclect==2)
//				{
//					alarm2.minute--;
//				if(alarm2.minute>60)
//				{
//						alarm2.minute = alarm2.minute -196; //“Á≥ˆ÷µ
//				}
//				alarm_min_long_up_down_flag=5;				
//				}
//				if(alarm_seclect==3)
//				{
//					alarm3.minute--;
//				if(alarm3.minute>60)
//				{
//						alarm3.minute = alarm3.minute -196; //“Á≥ˆ÷µ
//				}
//				alarm_min_long_up_down_flag=6;				
//				}			
//		}

//				
//				FD612_DISPLAY_2DIG_input(2,dis[alarm.minute/10]  ,dis[alarm.minute%10] );
//				mDelaymS(up_down_delay_time);
//				
//}
void alarm_set_display(UINT8 alarm_flag,struct DATA_ALARM* alarm)
{
	if(SET_Long_Flag)
	{
		key_first_into();
		alarm->alarm_on_dispaly=1;
		if(SET_Short_Flag)
		{
			SET_Short_Flag_alarm1_flag++;
			if(SET_Short_Flag_alarm1_flag>3)	SET_Short_Flag_alarm1_flag=0;
			SET_Short_Flag=0;
		}
			switch(SET_Short_Flag_alarm1_flag)
			{
				case 0:
				{
					FD612_DISPLAY_2DIG_input(2,dis[alarm->minute/10] ,dis[alarm->minute%10] );
					all_blink(0,1,alarm->hour);
					if(UP_Short_Flag==1)
					{
						alarm->hour++;
						alarm_hour_up_down2(0,0,alarm);
						UP_Short_Flag=0;
					}
					if(DOWN_Short_Flag == 1)
					{
						alarm->hour--;
						alarm_hour_up_down2(0,1,alarm);
						DOWN_Short_Flag=0;
					}
					break;
				}
				case 1:
				{													
					FD612_DISPLAY_2DIG_input(1 ,dis[alarm->hour/10] ,dis[alarm->hour%10] );	
					all_blink(0,2,alarm->minute);	
					if(UP_Short_Flag==1)
					{
						alarm->minute++;
						alarm_min_up_down2(0,0,alarm);
						UP_Short_Flag=0;
					}
					if(DOWN_Short_Flag == 1)
					{
						alarm->minute--;
						alarm_min_up_down2(0,1,alarm);
						DOWN_Short_Flag=0;
					}					
					break;
				}
				case 2:
				{
					SET_Long_Flag=0;
				  SET_Short_Flag_alarm1_flag=0;
				  key_first=1;
					break;
				}				
			}
			timeout_exit();
	}
	else
	{																
			if(UP_Short_Flag==1)
			 {
					if(alarm->alarm_on_dispaly)alarm->alarm_on_dispaly=0;
					else alarm->alarm_on_dispaly=1;
					UP_Short_Flag=0;
			 }	
					switch(alarm_flag)
					{
							case 1:
							{
								if(alarm->alarm_on_dispaly==0)FD612_display_4DIG_input( 0x40 ,0x40 ,FD612_DISP_A ,FD612_DISP_1 );
								else FD612_display_4DIG_input( 0x5c ,0x54 ,FD612_DISP_A ,FD612_DISP_1 );
					
								break;
							}
							case 2:
							{
								if(alarm->alarm_on_dispaly==0)FD612_display_4DIG_input( 0x40 ,0x40 ,FD612_DISP_A ,FD612_DISP_2 );
								else FD612_display_4DIG_input( 0x5c ,0x54 ,FD612_DISP_A ,FD612_DISP_2 );
						
								break;
							}
							case 3:
							{
								if(alarm->alarm_on_dispaly==0)FD612_display_4DIG_input( 0x40 ,0x40 ,FD612_DISP_A ,FD612_DISP_3 );
								else FD612_display_4DIG_input( 0x5c ,0x54 ,FD612_DISP_A ,FD612_DISP_3 );
						
								break;
							}							
					
					}
				timeout_exit();		
	}
}
//void alarm_set_display(UINT8 i,struct DATA_ALARM alarm)
//{
//							if(SET_Long_Flag)
//						{
//							key_first_into();

//							if(i==1)alarm1_on_dispaly=1;
//							if(i==2)alarm2_on_dispaly=1;
//							if(i==3)alarm3_on_dispaly=1;	
//							if(SET_Short_Flag)
//								{
//									SET_Short_Flag_alarm1_flag++;
//									SET_Short_Flag=0;
//									if(SET_Short_Flag_alarm1_flag>3)
//									{
//										SET_Short_Flag_alarm1_flag=0;
//									}
//								}
//					if(SET_Short_Flag_alarm1_flag==0)
//						{
//					all_blink(0,1,alarm.hour);

//						
//						FD612_DISPLAY_2DIG_input(2,dis[alarm.minute/10] ,dis[alarm.minute%10] );
//								if(UP_Short_Flag==1)
//								{
//									if(i==1)
//									{
//										alarm1.hour++;
//										if(hour_12_24_flag){
//											 if(alarm1.hour>=13)
//											 {
//												 alarm1.hour = 1;

//											 }
//											 if(alarm1.hour==12)
//											 {
//												 if(alarm1_pm_flag)alarm1_pm_flag=0;
//												 else alarm1_pm_flag=1;
//												}
//											}
//										else
//										{
//											if(alarm1.hour >=24)
//											{
//												alarm1.hour = 0;
//											}	
//										}
//									}
//									if(i==2)
//									{
//										alarm2.hour++;
//										if(hour_12_24_flag){
//											 if(alarm2.hour>=13)
//											 {
//												 alarm2.hour = 1;

//											 }
//										 if(alarm2.hour==12)
//											 {
//												 if(alarm2_pm_flag)alarm2_pm_flag=0;
//												 else alarm2_pm_flag=1;
//												}											 
//											}
//										else
//										{
//											if(alarm2.hour >=24)
//											{
//												alarm2.hour = 0;
//											}	
//										}
//									}	
//									if(i==3)
//									{
//										alarm3.hour++;
//										if(hour_12_24_flag){
//											 if(alarm3.hour>=13)
//											 {
//												 alarm3.hour = 1;
//											 }
//										 if(alarm3.hour==12)
//											 {
//												 if(alarm3_pm_flag)alarm3_pm_flag=0;
//												 else alarm3_pm_flag=1;
//												}											 
//											}
//										else
//										{
//											if(alarm3.hour >=24)
//											{
//												alarm3.hour = 0;
//											}	
//										}
//									}									
//										UP_Short_Flag=0;
//								}
//									if(DOWN_Short_Flag == 1)
//								{
//									if(i==1)
//									{
//										alarm1.hour--;					
//										if(hour_12_24_flag){
//											if(alarm1.hour==0)alarm1.hour=12;
////											 if(alarm1.hour>=13||alarm1.hour==0)
////											 {
////													alarm1.hour = alarm1.hour -244; //“Á≥ˆ÷µ
////											 }
//											if(alarm1.hour==11)
//											 {
//												 if(alarm1_pm_flag)alarm1_pm_flag=0;
//												 else alarm1_pm_flag=1;
//												}											 
//											}
//										else
//										{
//											if(alarm1.hour ==255)			alarm1.hour = 23; //“Á≥ˆ÷µ
//										}
//								  }
//									if(i==2)
//									{
//										alarm2.hour--;					
//										if(hour_12_24_flag){
//											if(alarm2.hour==0)alarm2.hour=12;
//											 if(alarm2.hour==11)
//											 {
//												 if(alarm2_pm_flag)alarm2_pm_flag=0;
//												 else alarm2_pm_flag=1;
//												}										 
//											}
//										else
//										{
//											if(alarm2.hour ==255)			alarm2.hour = 23;
//										}
//								  }
//									if(i==3)
//									{
//										alarm3.hour--;					
//										if(hour_12_24_flag){
//											 if(alarm3.hour==0)alarm3.hour=12;
//											if(alarm2.hour==11)
//											 {
//												 if(alarm2_pm_flag)alarm2_pm_flag=0;
//												 else alarm2_pm_flag=1;
//												}											 
//											}
//										else
//										{
//											if(alarm3.hour ==255)			alarm3.hour = 23;
//										}
//								  }									
//										DOWN_Short_Flag = 0;
//								}						
//						}
//					if(SET_Short_Flag_alarm1_flag==1)
//					{
//						all_blink(0,2,alarm.minute);
//						
//							
//							FD612_DISPLAY_2DIG_input(1 ,dis[alarm.hour/10] ,dis[alarm.hour%10] );
//								
//							if(UP_Short_Flag == 1)
//							{
//								if(i==1)
//								{
//									alarm1.minute++;
//								if(alarm1.minute >=60)
//								{
//									alarm1.minute = 0;
//								}
//								}
//								if(i==2)
//								{
//									alarm2.minute++;
//								if(alarm2.minute >=60)
//								{
//									alarm2.minute = 0;
//								}
//								}
//								if(i==3)
//								{
//									alarm3.minute++;
//								if(alarm3.minute >=60)
//								{
//									alarm3.minute = 0;
//								}
//								}								
//									UP_Short_Flag = 0;
//							}
//							if(DOWN_Short_Flag == 1)
//							{
//								if(i==1)
//								{
//									alarm1.minute--;
//								if(alarm1.minute==255)			alarm1.minute = 59; //“Á≥ˆ÷µ
//								}
//								if(i==2)
//								{
//									alarm2.minute--;
//								if(alarm2.minute==255)			alarm2.minute = 59;
//								}
//								if(i==3)
//								{
//									alarm3.minute--;
//								if(alarm3.minute==255)			alarm3.minute = 59;
//								}								
//									DOWN_Short_Flag = 0;
//							}
//					}	
//					if(SET_Short_Flag_alarm1_flag==2)	
//					{
//									
//									SET_Long_Flag=0;
//									SET_Short_Flag_alarm1_flag=0;
//									key_first=1;
//					
//					}
//							timeout_exit();
//						}
//						if(SET_Long_Flag==0)
//						{
//							switch(i)
//							{
//									case 1:
//									{
//										if(alarm1_on_dispaly==0)FD612_display_4DIG_input( 0x40 ,0x40 ,FD612_DISP_A ,FD612_DISP_1 );
//										else FD612_display_4DIG_input( 0x5c ,0x54 ,FD612_DISP_A ,FD612_DISP_1 );
//										if(UP_Short_Flag==1)
//										{
//											if(alarm1_on_dispaly)alarm1_on_dispaly=0;
//											else alarm1_on_dispaly=1;
//											UP_Short_Flag=0;
//										}								
//										break;
//									}
//									case 2:
//									{
//										if(alarm2_on_dispaly==0)FD612_display_4DIG_input( 0x40 ,0x40 ,FD612_DISP_A ,FD612_DISP_2 );
//										else FD612_display_4DIG_input( 0x5c ,0x54 ,FD612_DISP_A ,FD612_DISP_2 );
//										if(UP_Short_Flag==1)
//										{
//											if(alarm2_on_dispaly)alarm2_on_dispaly=0;
//											else alarm2_on_dispaly=1;
//											UP_Short_Flag=0;
//										}								
//										break;
//									}
//									case 3:
//									{
//										if(alarm3_on_dispaly==0)FD612_display_4DIG_input( 0x40 ,0x40 ,FD612_DISP_A ,FD612_DISP_3 );
//										else FD612_display_4DIG_input( 0x5c ,0x54 ,FD612_DISP_A ,FD612_DISP_3 );
//										if(UP_Short_Flag==1)
//										{
//											if(alarm3_on_dispaly)alarm3_on_dispaly=0;
//											else alarm3_on_dispaly=1;
//											UP_Short_Flag=0;
//										}								
//										break;
//									}							

//							}
//							timeout_exit();
//						}




//}

void alarm_hour_display(struct DATA_ALARM alarm)
{

					
				FD612_DISPLAY_2DIG_input(1 ,dis[alarm.hour/10] ,dis[alarm.hour%10] );
}

void alarm_min_display(struct DATA_ALARM alarm)
{

					
				FD612_DISPLAY_2DIG_input(2,dis[alarm.minute/10] ,dis[alarm.minute%10] );
}

void alarm_long_blink(struct DATA_ALARM alarm)//ƒ÷÷”≥§∞¥…¡À∏
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
#if mode_select_enable
void dp_mode_exit(void)
{
		if(mode_select_flag==0){
			flag_display=0;
//			temp_delay_time=0;
			mon_day_delay_time=0;
			hor_min_delay_time=0;
		}																			
		
}
#endif
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

					if(SET_Short_Flag_mon_day==0)
					{
					FD612_DISPLAY_2DIG_input(1,dis[time.mday/10] ,dis[time.mday% 10] );
					
					}
					if(SET_Short_Flag_mon_day==1 )
					{
	
						ui_mon_display();
						
					}
					break;					
				}
				case 2:
				{
										
						if(SET_Short_Flag_year==0)
						FD612_display_4DIG_input( dis[2] ,dis[0] ,dis[time.year/10] ,dis[time.year%10] );
						
					
					break;
				}
				case 3:
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
				case 4:
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
				case 5:
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
							if(alarm1.alarm_pm_flag==1&&flag_display==3 )
							{

									FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
								
							}
							if(alarm1.alarm_pm_flag==0&&flag_display==3)
							{
									FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;
														
							}				


							if(alarm2.alarm_pm_flag==1&&flag_display==4)
							{

									FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
								
							}
							 if(alarm2.alarm_pm_flag==0&&flag_display==4)
							{
										FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;
														
							}	

							
							if(alarm3.alarm_pm_flag&&flag_display==5)
							{

									FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
								
							}	
							if(alarm3.alarm_pm_flag&&flag_display==5)
							{
								FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;
														
							}	
							if(flag_display==0)
							{
									if(SET_Short_Flag_hur_min==1||SET_Short_Flag_hur_min==2)FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<1) ;
							}
							 if(flag_display==3||flag_display==4||flag_display==5)
							{
									if(SET_Short_Flag_alarm1_flag==0||SET_Short_Flag_alarm1_flag==1)FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<1) ;
							}
							
						}
					
						if(SET_Long_Flag==0&&(flag_display==0||flag_display==2)&&(hour_pm_flag==1))
						{
								FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
						
						}
				
					}
					else
					{

					}
					if(alarm1.alarm_on_dispaly ||alarm2.alarm_on_dispaly ||alarm3.alarm_on_dispaly)
					{
						FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<3) ;
					}
					else
					{
						FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<3) ;
					}
					if(flag_display==1)FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
					FD612_Refresh();
}



void after_arrive_write_1302(void)
{

				hour_chage_state=Read_addr_byte_ds1302(hour_r_addr);

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

}

	void hour_chage_input(UINT8 i)
	{//24–° ±◊™12–° ±
		
		UINT8 hour_state_value;
		hour_state_value=Read_addr_byte_ds1302(hour_r_addr);
//		ResetDS1302();							
//		WriteByteDS1302(hour_r_addr); //∂¡»° ±µƒµÿ÷∑
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
			//12–° ±◊™24–° ±
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

	}
	
	void temper_display(void)
	{
					
		if(ADC_TIME_Flag==temp_refresh_time)
		{
			ADC_Convert( );
			ADC_TIME_Flag=0;
		}
			adc_switch( );

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
			WriteByteDS1302(0xbf);																					//“‘∂‡◊÷Ω⁄∑Ω Ω∂¡»° ±÷”ºƒ¥Ê∆˜ ˝æ›
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
//			WriteByteDS1302(0x8e);		                      //–¥±£ª§øÿ÷∆ºƒ¥Ê∆˜
//			WriteByteDS1302(0x0);														//‘ –Ì–¥»Î
			
			ResetDS1302();
			WriteByteDS1302(0x90);													//–¥‰∏¡˜≥‰µÁøÿ÷∆ºƒ¥Ê∆˜
			WriteByteDS1302(0xab);													//‘ –Ì≥‰µÁ
			
			ResetDS1302();
			WriteByteDS1302(0xbe);													//–¥∂‡◊÷Ω⁄Õª∑¢∑Ω Ωøÿ÷∆ºƒ¥Ê∆˜
			
			WriteByteDS1302(second);
			WriteByteDS1302(minute);
			WriteByteDS1302(hour);
			WriteByteDS1302(mday);
			WriteByteDS1302(month);
			WriteByteDS1302(week);
			WriteByteDS1302(year);
			WriteByteDS1302(0);															//∂‘–¥±£ª§øÿ÷∆ºƒ¥Ê∆˜–¥»Î0
			ResetDS1302();
			ds1302_w_off();
		
//			from_ds1302_read_time_data();
			
					
}



void point_blink(void)
{

																

								if(time1_second)
								{
//									pm_12_24_alarm_display();
									if(blink3_flag)
									{
										blink3_flag=0;
										FD612_PotiveTNage(FD612_DIG1_ADDR,dis[time.hour/10]);
										FD612_PotiveTNage(FD612_DIG2_ADDR,dis[time.hour% 10]);
										FD612_PotiveTNage(FD612_DIG3_ADDR,dis[time.minute/10]);
										FD612_PotiveTNage(FD612_DIG4_ADDR,dis[time.minute% 10]);
										FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<1);
										 if(hour_pm_flag==1)
										{												
												FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;
										}
										else 
										{											
												FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;										
										}
										if(alarm1.alarm_on_dispaly ||alarm2.alarm_on_dispaly ||alarm3.alarm_on_dispaly)
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
										FD612_PotiveTNage(FD612_DIG1_ADDR,dis[time.hour/10]);
										FD612_PotiveTNage(FD612_DIG2_ADDR,dis[time.hour% 10]);
										FD612_PotiveTNage(FD612_DIG3_ADDR,dis[time.minute/10]);
										FD612_PotiveTNage(FD612_DIG4_ADDR,dis[time.minute% 10]);
										FD612_DispBuff.DispData[FD612_DIG12_ADDR] |= (1<<1);
										 if(hour_pm_flag==1)
										{  
												FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<0) ;										
										}
										else 
										{											
												FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<0) ;										
										}
										if(alarm1.alarm_on_dispaly ||alarm2.alarm_on_dispaly ||alarm3.alarm_on_dispaly)
										{
											FD612_DispBuff.DispData[FD612_DIG12_ADDR] |=(1<<3) ;
										}
										else
										{
											FD612_DispBuff.DispData[FD612_DIG12_ADDR] &=~(1<<3) ;
										}										
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
															 
								FD612_PotiveTNage(FD612_DIG5_ADDR,dis[(unsigned char)TEM2/10]);
								FD612_PotiveTNage(FD612_DIG6_ADDR,dis[(unsigned char)TEM2%10]);
								FD612_PotiveTNage(FD612_DIG7_ADDR,FD612_DISP_cc);
								FD612_DispBuff.DispData[FD612_DIG12_ADDR]|= 1<<6 ; 
								FD612_Refresh();
							 ADC_Display1=0;
							 break;
						 }
							

							case 1:
							{
								

							FD612_PotiveTNage(FD612_DIG5_ADDR,dis[(unsigned char)TEM4/100]);
							FD612_PotiveTNage(FD612_DIG6_ADDR,dis[(unsigned char)TEM4%100/10]);
							FD612_PotiveTNage(FD612_DIG7_ADDR,dis[(unsigned char)TEM4%100%10]);
							FD612_Refresh();	
							ADC_Display1=1;
								break;
							}	
								default :
								break;
					}
}

void timeout_exit(void)
{
					if(back_first)
				{
					back_TIME_Flag=0;
					back_first=0;
				}
				if(back_TIME_Flag==over_time)
				{
					display_flag_mon_day=1;	
					display_flag_hur_min=1;	
					SET_Long_Flag=0;
					SET_Short_Flag_mon_day=0;
					SET_Short_Flag_hur_min=0;
					SET_Short_Flag_alarm1_flag=0;
					back_TIME_Flag=0;
					back_first=1;
					key_first=1;
					flag_display=0;
#if mode_select_enable									
					dp_mode_exit();	
#endif									
				}	
				

}

void ui_mon_display(void)
{
				if(time.month<=9)
			{
			FD612_PotiveTNage(FD612_DIG3_ADDR,0x00);
			FD612_PotiveTNage(FD612_DIG4_ADDR,dis[time.month%10]);
			pm_12_24_alarm_display();
			}
			else
			FD612_DISPLAY_2DIG_input(2 ,dis[time.month/10] ,dis[time.month%10] );
}

UINT8  leep_year(UINT16  year)
{
	if((year%400 == 0) || year%4 == 0 && year%100!=0)
	return 1;
	else
	return 0;
}

void mon_over_judge(void)
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

void mday_over_judge(void)
{
	if(time.month >=13)time.month=1;
	if(time.month ==0)time.month=12;
	if(time.month==4||time.month==6||time.month==9||time.month==11) 
	{if(time.mday==31)time.mday=1;time.mday = decimal_bcd(time.mday);Write_addr_byte_ds1302(day_w_addr,time.mday);}
	if(time.month==2) 
	{if(time.mday>=30)time.mday=1;time.mday = decimal_bcd(time.mday);Write_addr_byte_ds1302(day_w_addr,time.mday);}
}
#if 0
void mday_blink(UINT8 blink_set)
{
			if(time1_second)
		{
		if(blink_flag)
		{
			blink_flag=0;	
			FD612_DISPLAY_2DIG_input(1 ,dis[time.mday/10] ,dis[time.mday%10] );
		}
		else
		{
			blink_flag=1;
			if(blink_set==0)
			FD612_DISPLAY_2DIG_input(1 ,0x00  ,0x00 );
			else
			FD612_DISPLAY_2DIG_input(1 ,dis[time.mday/10] ,dis[time.mday%10] );
		}
		time1_second=0;
		}	
}
#endif
void key_12and24_switch(void)
{
						if(hour_12_24_flag)//12◊™≥…24
					{
						hour_12_24_flag=0;
						hour_chage_input(hour_12_24_flag);
						alarm_12switch24(&alarm1);
						alarm_12switch24(&alarm2);
						alarm_12switch24(&alarm3);
//						if(alarm1_pm_flag)
//					{
//						if(alarm1.hour!=12)
//						{
//							alarm1.hour=alarm1.hour+12;
//						}
//						alarm1_pm_flag=0;
//					}
//					else if(alarm1.hour==12)alarm1.hour=0;
//				
//					if(alarm2_pm_flag)
//					{
//						if(alarm2.hour!=12)
//						{
//							alarm2.hour=alarm2.hour+12;
//						}
//						alarm2_pm_flag=0;
//					}
//					else if(alarm2.hour==12)alarm2.hour=0;
//				
//					if(alarm3_pm_flag)
//					{
//						if(alarm3.hour!=12)
//						{
//							alarm3.hour=alarm3.hour+12;
//						}
//						alarm3_pm_flag=0;
//					}
//					else if(alarm3.hour==12)alarm3.hour=0;	
					}
					
					else	
					{
						hour_12_24_flag=1;
						hour_chage_input(hour_12_24_flag);
						alarm_24switch12(&alarm1);	
						alarm_24switch12(&alarm2);
						alarm_24switch12(&alarm3);
//						if(alarm1.hour>12)
//						{
//							alarm1.hour=alarm1.hour-12;
//							alarm1_pm_flag=1;
//						}
//						if(alarm1.hour==12)alarm1_pm_flag=1;
//						if(alarm1.hour==0){alarm1.hour=12;alarm1_pm_flag=0;}
//						
//						if(alarm2.hour>12)
//						{
//							alarm2.hour=alarm2.hour-12;
//							alarm2_pm_flag=1;
//						}
//						if(alarm2.hour==12)alarm2_pm_flag=1;
//						if(alarm2.hour==0){alarm2.hour=12;alarm2_pm_flag=0;}
//						
//						if(alarm3.hour>12)
//						{
//							alarm3.hour=alarm3.hour-12;
//							alarm3_pm_flag=1;
//						}
//						if(alarm3.hour==12)alarm3_pm_flag=1;
//						if(alarm3.hour==0){alarm3.hour=12;alarm3_pm_flag=0;}													
					}
}

void key_first_into(void)
{
		if(key_first)
	{
		key_first=0;
		UP_Short_Flag=0;
		DOWN_Short_Flag = 0;
	}
}
#if 0 
void hour_blink(UINT8 blink_set)
{
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
				if(blink_set==0)
				FD612_DISPLAY_2DIG_input(1 ,0x00 ,0x00  );
				else
				FD612_DISPLAY_2DIG_input(1 ,dis[time.hour/10] , dis[time.hour%10]);
			}
			time1_second=0;
			}
}
#endif
#if 0
void min_blink(UINT8 blink_set)
{
				if(time1_second)
			{
				if(blink_flag)
				{
					blink_flag=0;
					FD612_DISPLAY_2DIG_input(2 ,dis[time.minute/10] , dis[time.minute%10]);

				}
			else
				{
					blink_flag=1;
					if(blink_set==0)
					FD612_DISPLAY_2DIG_input(2 ,0x00 ,0x00  );
					else
					FD612_DISPLAY_2DIG_input(2 ,dis[time.minute/10] , dis[time.minute%10]);
					
				}
				time1_second=0;
			 }
}
#endif
void all_blink(UINT8 blink_set,UINT8 ui_set,UINT8 value)
{
			if(time1_second)
			{
				if(blink_flag)
				{
					blink_flag=0;
					
					FD612_DISPLAY_2DIG_input(ui_set ,dis[value/10] , dis[value%10]);

				}
			else
				{
					blink_flag=1;
					if(blink_set==0)
					FD612_DISPLAY_2DIG_input(ui_set ,0x00 ,0x00  );
					else
					FD612_DISPLAY_2DIG_input(ui_set ,dis[value/10] , dis[value%10]);
					
				}
				time1_second=0;
			 }
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

void key_off_alarm_clock(struct  DATA_ALARM* alarm)
{
	if(alarm->alarm_flag==1)
{
	if(SET==0||UP==0||DOWN==0)
	{
		alarm->alarm_flag=0;
		PWM_CTRL &= ~(1<<3);
	  alarm_voice_flag=0;		
	}
}
}

void action_remove(void)
{
	if(SET_Long_Flag==0)
	{
		if(UP_Short_Flag == 1)UP_Short_Flag=0;
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
	



