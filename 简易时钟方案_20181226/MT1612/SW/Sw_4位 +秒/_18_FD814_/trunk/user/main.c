#include "myinclude.h"
void	CfgFsys( )  
{
// 		SAFE_MOD = 0x55;
// 		SAFE_MOD = 0xAA;
//     CLOCK_CFG |= bOSC_EN_XT;                          //使能外部晶振
//     CLOCK_CFG &= ~bOSC_EN_INT;                        //关闭内部晶振    
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
#define	 FREQ_SYS	   12000000	         //系统主频12MHz
#define  UART0_BUAD    19200
#define  UART1_BUAD    19200

    UINT32 x;
    UINT8 x2; 

    SM0 = 0;
    SM1 = 1;
    SM2 = 0;                                                                   //串口0使用模式1
                                                                               //使用Timer1作为波特率发生器
    RCLK = 0;                                                                  //UART0接收时钟
    TCLK = 0;                                                                  //UART0发送时钟
    PCON |= SMOD;
    x = 10 * FREQ_SYS / UART0_BUAD / 16;                                       //如果更改主频，注意x的值不要溢出                            
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                       //四舍五入

    TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;              //0X20，Timer1作为8位自动重载定时器
    T2MOD = T2MOD | bTMR_CLK | bT1_CLK;                                        //Timer1时钟选择
    TH1 = 0-x;                                                                 //12MHz晶振,buad/12为实际需设置波特率
    TR1 = 1;                                                                   //启动定时器1
    TI = 1;
    REN = 1;                                                                   //串口0接收使能
	
	ES = 1;
	EA = 1;
}

char putchar (char ch)
{
	SBUF = ch;		//查询发送，中断方式可不用下面2条语句,但发送前需TI=0
	TI = 0;
	while(TI ==0);
	TI = 0;
	return 1;
}

void TouchKeyInit(void)
{
		P1_DIR_PU &= 0x0C;                                                         //所有触摸通道设置为浮空输入，用不到的通道可以不设置
    TouchKeyQueryCyl2ms();                                                     //TouchKey查询周期2ms	
}

void main(void)
{
	unsigned long i = 0;
	CfgFsys( );
	UartInit();
	TouchKeyInit();
	printf( "FD814 串口启动成功\n" );

    // while(1)                                                                   
    // {			
      // TouchKeyChannelQuery();                                                  //查询触摸按键状态
      // delayms(200);                                                           //延时无意义，模拟单片机干其他事				
    // }
	// while(1)	//发送接收到的数据
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
//  串口中断处理程序
//-----------------------------------------------------------------------------
void Uart_Isr() interrupt INT_NO_UART0 using 1
{
	if(RI)	//接收终端
	{
		RI = 0;	//清除标记位
		ReceiveDataBufe[ReceiveCnt].Data = SBUF;
		ReceiveDataBufe[ReceiveCnt].Flag = 0x01;//表示可读
		ReceiveCnt++;
		if(ReceiveCnt >= RECEIVE_LENGTH)
		{
			ReceiveCnt = 0;
		}
	}
}
