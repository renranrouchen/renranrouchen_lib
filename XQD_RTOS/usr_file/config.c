 /*************************************************
* @Copyright(c) 2016-2100.  hwl.  All rights reserved.
* @FileName: config.c
* @Author:   hwl 
* @Description:  Driver configuration gpio,UART2_Init
* @Version:
* @History:         
      <author>  <time>      <version >   <desc>
				hwl      18/9/7       1.0     
****************************************************/


#include "M451Series.h"
#include "stdio.h"
/***************************************************** 
 * @Function:  gpio_init
 * @Description:   GPIO drive initialize
 * @param  void 
 * @retval void
******************************************************/
void gpio_init(void)
{
	/******************************
	LCD_CSB PE0 24
	LCD_RES PC4 23
	LCD_A0  PD7 14
	LCD_SCK PC1 20
	LCD_SDA PC0 19
	
	******************************/
	GPIO_SetMode(PE, BIT0, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT4, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PD, BIT7, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT1, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT0, GPIO_MODE_OUTPUT);
	
	
	/***********************************
								100pin
	KY1 PA0        82
	KY2 PB0        91
	KEY3 PD2       16
	KEY4 PD3       17
	************************************/

	GPIO_SetMode(PA, BIT0, GPIO_MODE_INPUT);
	GPIO_SetMode(PB, BIT0, GPIO_MODE_INPUT);
	GPIO_SetMode(PD, BIT2, GPIO_MODE_INPUT);
	GPIO_SetMode(PD, BIT3, GPIO_MODE_INPUT);


	/***********************************
	TXD PC2 46  RXD PC3
	************************************/
	GPIO_SetMode(PC, BIT2, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT3, GPIO_MODE_OUTPUT);
	PC3=1;
		/***********************************
	SINGNAL PB1 
	************************************/
	GPIO_SetMode(PB, BIT1, GPIO_MODE_OUTPUT);
  PB1=1;
}

/***************************************************** 
 * @Function:  UART2_Init
 * @Description:   UART2 drive initialize
 * @param  void 
 * @retval void
******************************************************/
void UART2_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART1 module */
    SYS_ResetModule(UART2_RST);

    /* Configure UART1 and set UART1 Baudrate */
    UART2_Open(UART2, 250000);	
}
#if 0
void FMC_Init(void)
{
	uint32_t u32Data,u32Data2;
	SYS_UnlockReg();
	FMC_Open();
  FMC_ENABLE_AP_UPDATE();
	u32Data = FMC_Read(0x0001F000);
	u32Data2 = FMC_Read(0x0001F000+4);
	printf("u32Data____%x__%x\r\n",u32Data,u32Data2);
	FMC->ISPCTL &=  ~FMC_ISPCTL_ISPEN_Msk;
	SYS_LockReg();
}
#endif
void flash_data_addr_base(void)
{
	uint32_t  au32Config[2];
	 //SYS_UnlockReg();
	FMC_ReadConfig(au32Config, 2);
	printf("u32Data____%x__%x\r\n",au32Config[1],au32Config[0] );
	FMC_EnableConfigUpdate();
	au32Config[0] &= ~0x01;
	au32Config[1] =0x0001f000;
	FMC_Erase(FMC_CONFIG_BASE);
	
	FMC_WriteConfig(au32Config, 2);
	//SYS_LockReg();
}
