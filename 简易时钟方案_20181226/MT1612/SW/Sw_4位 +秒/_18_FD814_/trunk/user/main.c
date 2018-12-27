#include "myinclude.h"
void	CfgFsys( )  
{
// 		SAFE_MOD = 0x55;
// 		SAFE_MOD = 0xAA;
//     CLOCK_CFG |= bOSC_EN_XT;                          //ʹ���ⲿ����
//     CLOCK_CFG &= ~bOSC_EN_INT;                        //�ر��ڲ�����    
		SAFE_MOD = 0x55;
		SAFE_MOD = 0xAA;
// 		CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x07;  // 32MHz	
// 		CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x06;  // 24MHz	
// 		CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x05;  // 16MHz	
		CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x04;  // 12MHz
// 		CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x03;  // 6MHz	
// 		CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x02;  // 3MHz	
// 		CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x01;  // 750KHz	
// 		CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x00;  // 187.5MHz		
		SAFE_MOD = 0x00;
}

void UartInit(void)		//19200bps@22.1184MHz
{
#define	 FREQ_SYS	   12000000	         //ϵͳ��Ƶ12MHz
#define  UART0_BUAD    19200
#define  UART1_BUAD    19200

    UINT32 x;
    UINT8 x2; 

    SM0 = 0;
    SM1 = 1;
    SM2 = 0;                                                                   //����0ʹ��ģʽ1
                                                                               //ʹ��Timer1��Ϊ�����ʷ�����
    RCLK = 0;                                                                  //UART0����ʱ��
    TCLK = 0;                                                                  //UART0����ʱ��
    PCON |= SMOD;
    x = 10 * FREQ_SYS / UART0_BUAD / 16;                                       //���������Ƶ��ע��x��ֵ��Ҫ���                            
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                       //��������

    TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;              //0X20��Timer1��Ϊ8λ�Զ����ض�ʱ��
    T2MOD = T2MOD | bTMR_CLK | bT1_CLK;                                        //Timer1ʱ��ѡ��
    TH1 = 0-x;                                                                 //12MHz����,buad/12Ϊʵ�������ò�����
    TR1 = 1;                                                                   //������ʱ��1
    TI = 1;
    REN = 1;                                                                   //����0����ʹ��
	
	ES = 1;
	EA = 1;
}

char putchar (char ch)
{
	SBUF = ch;		//��ѯ���ͣ��жϷ�ʽ�ɲ�������2�����,������ǰ��TI=0
	TI = 0;
	while(TI ==0);
	TI = 0;
	return 1;
}

void TouchKeyInit(void)
{
		P1_DIR_PU &= 0x0C;                                                         //���д���ͨ������Ϊ�������룬�ò�����ͨ�����Բ�����
    TouchKeyQueryCyl2ms();                                                     //TouchKey��ѯ����2ms	
}

void main(void)
{
	unsigned long i = 0;
	CfgFsys( );
	UartInit();
	TouchKeyInit();
	printf( "FD814 ���������ɹ�\n" );

    // while(1)                                                                   
    // {			
      // TouchKeyChannelQuery();                                                  //��ѯ��������״̬
      // delayms(200);                                                           //��ʱ�����壬ģ�ⵥƬ����������				
    // }
	// while(1)	//���ͽ��յ�������
	// {
		// if(ReceiveDataBufe[ReadCnt].Flag)
		// {
			// ReceiveDataBufe[ReadCnt].Flag = 0;
			// printf("%c", ReceiveDataBufe[ReadCnt].Data);

			// ReadCnt++;
			// if(ReadCnt >= RECEIVE_LENGTH)
			// {
				// ReadCnt = 0;
			// }
		// }
	// }
	AppUsart_Init();
	while(1)
	{
		TouchKeyChannelQuery();
		AppUsart_Run();
	}
}

//-----------------------------------------------------------------------------
//  �����жϴ������
//-----------------------------------------------------------------------------
void Uart_Isr() interrupt INT_NO_UART0 using 1
{
	if(RI)	//�����ն�
	{
		RI = 0;	//������λ
		ReceiveDataBufe[ReceiveCnt].Data = SBUF;
		ReceiveDataBufe[ReceiveCnt].Flag = 0x01;//��ʾ�ɶ�
		ReceiveCnt++;
		if(ReceiveCnt >= RECEIVE_LENGTH)
		{
			ReceiveCnt = 0;
		}
	}
}
