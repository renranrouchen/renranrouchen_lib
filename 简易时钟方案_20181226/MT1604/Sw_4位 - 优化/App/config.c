#include "config.h"
#include "usr.h"

extern UINT8 hour_12_24_flag,hour_pm_flag;

void gpio_init(void){
		PIN_FUNC  &=   ~ (1 << 7) ;	//��λΪ1��P36/P37����UDP/UDM
	
		Port1Cfg(3,4);   //DS1302	���ų�ʼ��  	                                                            //I/O��©�������
		Port1Cfg(3,5);
		Port1Cfg(3,6);

		Port3Cfg(3,0);   //FD612���ų�ʼ��
		Port3Cfg(3,1);
	
		Port3Cfg(0,2);	 //��������
	
		Port3Cfg(3,4);		//����������
		buzzer=0;
			
		Port1Cfg(3,7);		//��������
		Port3Cfg(3,7);
		Port3Cfg(3,3);	
	
}

void ADC0_INIT(UINT8 div){
    ADC_CFG &= ~bADC_CLK | div;
    ADC_CFG |= bADC_EN;                                                        //ADC��Դʹ��

	
	
//  ADC_START = 1;  
//  while(ADC_START);                                                      //ADC_START��Ϊ0ʱ����ʾ�������
//	ADC_ConvertedValueLocal =(float) ADC_DATA/255*5; // ��ȡת����ADֵ	
}

void DS1302_INIT(void)
{
		UINT8 ds1320_state=0,rom_value;
	Write_addr_byte_ds1302(0x90,0xab);
	rom_value=Read_addr_byte_ds1302(rom1_r_addr);
	 if(rom_value!=0xaa)
		{
			InitDS1302( );
			Write_addr_byte_ds1302(rom1_w_addr,0xaa);
			

		}
//		  ResetDS1302();
//			WriteByteDS1302(0x8e);		                      //д�������ƼĴ���
//			WriteByteDS1302(0x0);														//����д��
//			
//			ResetDS1302();
//			WriteByteDS1302(0x90);													//д��������ƼĴ���
//			WriteByteDS1302(0xab);	
			ds1320_state=Read_addr_byte_ds1302(hour_r_addr);

		if(ds1320_state&0x80)
		{
			hour_12_24_flag=1;
		}
		if(hour_12_24_flag==1)
		{
			if(ds1320_state&0x20)
			hour_pm_flag=1;
		}
}
void time0_init(void){
		BC(TMOD,3);   //bT0_GATE��0
		BC(TMOD,2);   //bT0_CT��0�����ڶ�ʱ��ʽ
		
		BC(TMOD,1);		//bTn_M1=0,bTn_M0=1,ģʽ1
		BS(TMOD,0);
		BC(T2MOD,4);
//		TH0 =0X3C;
//	  TL0 =0XB0;
		TH0=0XFf;
		TL0=0X90;
		
		ET0 =1;
		EA =1;
	//	TR0 =1;
}

void time1_init(void){
		BC(TMOD,7);   //bT1_GATE��0
		BC(TMOD,6);   //bT1_CT��0�����ڶ�ʱ��ʽ
		
		BC(TMOD,5);		//bTn_M1=0,bTn_M0=1,ģʽ1
		BS(TMOD,4);
		BC(T2MOD,5);
		TH1 =0X3C;
	  TL1 =0XB0;
		ET1 =1;
		EA =1;
		TR1 =1;
}
#if 0
void time2_init(void)
{
	T2MOD &= ~(bTMR_CLK | bT2_CLK);C_T2 = 0;
	RCLK = 0;TCLK = 0;CP_RL2 = 0;
	 TL2 =  0xff;                                               //16λ�Զ����ض�ʱ��
    TH2 =  0xff;
		ET2 = 1; 
//	TR2=1;
}
#endif

#if 0
void PWM_init(void)
{
    P3_MOD_OC &= ~( bPWM2);                                             //����PWM����Ϊ�������
    P3_DIR_PU |=  bPWM2;
		PWM_CK_SE=4;
	PWM_CTRL |= bPWM_CLR_ALL;
	PWM_CTRL &= ~bPWM_CLR_ALL;
	//PWM_CTRL |= bPWM2_OUT_EN;
	PWM_CTRL |= bPWM2_POLAR;
	PWM_DATA2 = 200;
}
#endif

/*******************************************************************************
* Function Name  : Port1Cfg()
* Description    : �˿�1����
* Input          : Mode  0 = �������룬������
                         1 = �����������
                         2 = ��©���������������
                         3 = ��51ģʽ����©������������������ڲ���·���Լ����ɵ͵��ߵĵ�ƽ����		
                   ,UINT8 Pin	(0-7)											 
* Output         : None
* Return         : None
*******************************************************************************/
void Port1Cfg(UINT8 Mode,UINT8 Pin)
{
  switch(Mode){
    case 0:
      P1_MOD_OC = P1_MOD_OC & ~(1<<Pin);
      P1_DIR_PU = P1_DIR_PU &	~(1<<Pin);	
      break;
    case 1:
      P1_MOD_OC = P1_MOD_OC & ~(1<<Pin);
      P1_DIR_PU = P1_DIR_PU |	(1<<Pin);				
      break;		
    case 2:
      P1_MOD_OC = P1_MOD_OC | (1<<Pin);
      P1_DIR_PU = P1_DIR_PU &	~(1<<Pin);				
      break;		
    case 3:
      P1_MOD_OC = P1_MOD_OC | (1<<Pin);
      P1_DIR_PU = P1_DIR_PU |	(1<<Pin);			
      break;
    default:
      break;			
  }
}

/*******************************************************************************
* Function Name  : Port3Cfg()
* Description    : �˿�3����
* Input          : Mode  0 = �������룬������
                         1 = �����������
                         2 = ��©���������������
                         3 = ��51ģʽ����©������������������ڲ���·���Լ����ɵ͵��ߵĵ�ƽ����		
                   ,UINT8 Pin	(0-7)											 
* Output         : None
* Return         : None
*******************************************************************************/

void Port3Cfg(UINT8 Mode,UINT8 Pin)
{
  switch(Mode){
    case 0:
      P3_MOD_OC = P3_MOD_OC & ~(1<<Pin);
      P3_DIR_PU = P3_DIR_PU &	~(1<<Pin);	
      break;
    case 1:
      P3_MOD_OC = P3_MOD_OC & ~(1<<Pin);
      P3_DIR_PU = P3_DIR_PU |	(1<<Pin);				
      break;		
    case 2:
      P3_MOD_OC = P3_MOD_OC | (1<<Pin);
      P3_DIR_PU = P3_DIR_PU &	~(1<<Pin);				
      break;		
    case 3:
      P3_MOD_OC = P3_MOD_OC | (1<<Pin);
      P3_DIR_PU = P3_DIR_PU |	(1<<Pin);			
      break;
    default:
      break;			
  }
}


void display_init_first(void)
{
	UINT8 rom2_value;
	rom2_value=Read_addr_byte_ds1302(rom2_r_addr);

		if(rom2_value!=0xaa)
		{ 
			FD612_loop( );
			FD612_Init( );
			Write_addr_byte_ds1302(rom2_w_addr,0xaa);			

			
		}	
}