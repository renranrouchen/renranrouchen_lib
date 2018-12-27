/*************************************************
* @Copyright(c) 2016-2100.  hwl.  All rights reserved.
* @FileName: main.c
* @Author:   hwl 
* @Description:  Main function entry
* @Version:
* @History:         
      <author>  <time>      <version >   <desc>
				hwl      18/9/7       1.0     
****************************************************/





#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "Event_process.h"
#include "Get_event.h"
#include "DMX512.H"
#include "DMX512_API.H"
#include "config.h"
#include "ST7567_API.H"
/* Hardware and starter kit includes. */
#include "M451Series.h"

#define PLL_CLOCK           72000000

/* Priorities for the demo application tasks. */
#define mainFLASH_TASK_PRIORITY             ( tskIDLE_PRIORITY + 1UL )
#define mainQUEUE_POLL_PRIORITY             ( tskIDLE_PRIORITY + 2UL )
#define mainSEM_TEST_PRIORITY               ( tskIDLE_PRIORITY + 1UL )
#define mainBLOCK_Q_PRIORITY                ( tskIDLE_PRIORITY + 2UL )
#define mainCREATOR_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3UL )
#define mainFLOP_TASK_PRIORITY              ( tskIDLE_PRIORITY )
#define mainCHECK_TASK_PRIORITY             ( tskIDLE_PRIORITY + 3UL )

#define mainCHECK_TASK_STACK_SIZE           ( configMINIMAL_STACK_SIZE )

/* The time between cycles of the 'check' task. */
#define mainCHECK_DELAY                     ( ( portTickType ) 5000 / portTICK_RATE_MS )

/* The LED used by the check timer. */
#define mainCHECK_LED                       ( 3UL )

/* A block time of zero simply means "don't block". */
#define mainDONT_BLOCK                      ( 0UL )

/* The period after which the check timer will expire, in ms, provided no errors
have been reported by any of the standard demo tasks.  ms are converted to the
equivalent in ticks using the portTICK_RATE_MS constant. */
#define mainCHECK_TIMER_PERIOD_MS           ( 3000UL / portTICK_RATE_MS )

/* The period at which the check timer will expire, in ms, if an error has been
reported in one of the standard demo tasks.  ms are converted to the equivalent
in ticks using the portTICK_RATE_MS constant. */
#define mainERROR_CHECK_TIMER_PERIOD_MS     ( 200UL / portTICK_RATE_MS )

/* Set mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY to 1 to create a simple demo.
Set mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY to 0 to create a much more
comprehensive test application.  See the comments at the top of this file, and
the documentation page on the http://www.FreeRTOS.org web site for more
information. */
#define mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY     0

#define dedug_test 0
#define SIGNAL_LED PB1

/*-----------------------------------------------------------*/

/*
 * Set up the hardware ready to run this demo.
 */
static void prvSetupHardware(void);
/*-----------------------------------------------------------*/


static void ui_display_fuc(void *pvParameters);






/*-----------------------------------------------------------*/



void flash_data_addr_base(void);
/***************************************************** 
 * @Function:  prvSetupHardware
 * @Description:   Hardware drive initialize
 * @param  void 
 * @retval void
******************************************************/

static void prvSetupHardware(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HIRC clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Waiting for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Switch HCLK clock source to HIRC */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Configure PLL */
    CLK_EnablePLL(CLK_PLLCTL_PLLSRC_HIRC, PLL_CLOCK);

    /* Switch HCLK clock source to PLL */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL, CLK_CLKDIV0_HCLK(1));

    /* Select UART module clock source as HXT and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UARTSEL_HIRC, CLK_CLKDIV0_UART(1));
		CLK_SetModuleClock(UART2_MODULE, CLK_CLKSEL1_UARTSEL_HIRC, CLK_CLKDIV0_UART(1));
		
    /* Enable peripheral clock */
    CLK_EnableModuleClock(UART0_MODULE);
		CLK_EnableModuleClock(UART2_MODULE);
    CLK_EnableModuleClock(TMR0_MODULE);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();
		

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PD multi-function pins for UART0 RXD and TXD */
    SYS->GPD_MFPL &= ~(SYS_GPD_MFPL_PD0MFP_Msk | SYS_GPD_MFPL_PD1MFP_Msk);
    SYS->GPD_MFPL |= (SYS_GPD_MFPL_PD0MFP_UART0_RXD | SYS_GPD_MFPL_PD1MFP_UART0_TXD);
		
    /* Set PB multi-function pins for UART2 RXD(Pc.2) and TXD(Pc.3) */
    SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk | SYS_GPC_MFPL_PC3MFP_Msk);
   // SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC3MFP_UART2_RXD | SYS_GPC_MFPL_PC2MFP_UART2_TXD);
		SYS->GPC_MFPL |= ( SYS_GPC_MFPL_PC2MFP_UART2_TXD | SYS_GPC_MFPL_PC3MFP_GPIO );
    FMC_Open();
		FMC_ENABLE_AP_UPDATE();
		FMC_EnableConfigUpdate();
		
		//*(( uint32_t *)0x00300000)&=~(1<<0);
		//*(( uint32_t *)0x00300004) =0x0001f000;
//		FMC->ISPCTL &=  ~FMC_ISPCTL_ISPEN_Msk;
    /* Lock protected registers */
	//	flash_data_addr_base();
    SYS_LockReg();
		
    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 115200);

		UART2_Init();
		gpio_init( );
//		flash_data_addr_base();
		Data_init();

		
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void)
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for(;;);
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void)
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for(;;);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void)
{
}
/*-----------------------------------------------------------*/


unsigned char ADDR_DATA2[2][2]=
{
	{0X00,0X0F},
	{0X10,0X0F},
};
/***************************************************** 
 * @Function:  vCheckTask2
 * @Description:   thread task for test
 * @param  void 
 * @retval void
******************************************************/
//void delay_nus(unsigned short time);
//extern RZ_arg_t chip_param_sm16804P;
//extern chip_param_t chip_param_sm17512;
//void test_addr(unsigned int addr_start);
//void RZ_Protocol(uchar data);
//uchar fanx1(uchar i);
//void SM16703P_write_data(RZ_arg_t* RZ_arg);

//void Protocol_output_data(Protocol_output_parameter_t* Protocol_output_param);
//char tes_dat[17*3] ={0,255,0,  0,0,0,  0,0,255,  0,0,255,  0,0,255,  0, 0,255,  255,255,255,  0,0,255,   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//char tes_dat2[15]={255,0,0,  255,255,255,  0,255,0,  0,255,0,  0,255,0};
#if 0
void vCheckTask2(void *pvParameters)
{
 
		for( ; ;)
		{
			//DMX512_Clear_init();
			//sm1751x_Write_addr( 2,ADDR_DATA2 );
			//test_addr(0x1234);
			 // test_arg( );
#if debug			
			//printf("test====================================>>>>>>>>>>>>>>>\r\n");
#endif
			
			//SM16703P_write_data(&chip_param_sm16804P);
			//DMX_Send_data(0,255,0);
			 // Run_manually_point_fuc(17);
			DMX_SendPacket(tes_dat2,6);
			DMX_SendPacket(tes_dat2,6);
			//Protocol_output_Send_data(0,255,0,0,0,&Equipment_set_param);
			//Protocol_output_Send_data(0,255,0,0,0,&Equipment_set_param);
			//send_add(0x0001,3,2048);
			//test_dalay();
			//RZ_Protocol(0x55);
			//SM165XX_Send_addr(0x0001,3,3,0xd2,3);
				//UART2_Open(UART2, Equipment_set_param.Output_frequency_param);
			  //Protocol_output_data(&Equipment_set_param);
			
			//DMX_Send_data(0,255,0,&chip_param_sm17512);
			vTaskDelay( 1000/ portTICK_RATE_MS );


			
			
		}
}
#endif
xTaskHandle ui_display_DisplayHandle;
xTaskHandle check_DisplayHandle;
extern xTaskHandle xgradual_displayHandle;
extern xTaskHandle RACELAMP_displayHandle;
extern Protocol_output_parameter_t Equipment_set_param;
/***************************************************** 
 * @Function:  ui_display_fuc
 * @Description:   thread task for Interface display 
 * @param  void 
 * @retval void
******************************************************/
static void ui_display_fuc(void *pvParameters)
{ 
		//开机界面与渐变的任务冲突
	  if(Equipment_set_param.Color_num == gradual_change)
		{
			if(eSuspended!=eTaskGetState(xgradual_displayHandle))vTaskSuspend(xgradual_displayHandle);
		}					
		//开机界面与跑马的任务冲突
		if(Equipment_set_param.Color_num == RACE_LAMP)
		{
			if(eSuspended!=eTaskGetState(RACELAMP_displayHandle))vTaskSuspend(RACELAMP_displayHandle);			
		}	
		
		ui_display_init();
		ui_display();
	 
		if(Equipment_set_param.Color_num == gradual_change)
		{
			if(eSuspended==eTaskGetState(xgradual_displayHandle))vTaskResume(xgradual_displayHandle);
		}
		if(Equipment_set_param.Color_num == RACE_LAMP)
		{
			if(eSuspended==eTaskGetState(RACELAMP_displayHandle))vTaskResume(RACELAMP_displayHandle);
		}
			
		vTaskSuspend( ui_display_DisplayHandle );//不需要刷新时挂起
		for( ; ;)
		{
			ui_display();

				
				vTaskDelay( 500 / portTICK_RATE_MS );

		}

}

/***************************************************** 
 * @Function:  ui_display_fuc
 * @Description:   thread task for Event detection 
 * @param  void 
 * @retval void
******************************************************/
void event_get_task(void *param)
{
	for( ; ;)
	{
		/*Key event detection*/
		check_key_press_event();
		vTaskDelay( 20 / portTICK_RATE_MS );
	}
}

/***************************************************** 
 * @Function:  main
 * @Description:    The entrance of  function 
 * @param  void 
 * @retval int
******************************************************/
int main(void)
{
    
    prvSetupHardware();	
		DMX_Init();
		event_process();

		//xTaskCreate(vCheckTask2, (signed portCHAR *) "Check2", mainCHECK_TASK_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, &check_DisplayHandle);
    xTaskCreate(ui_display_fuc, (signed portCHAR *) "ui_display_fuc", mainCHECK_TASK_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, &ui_display_DisplayHandle);		
		xTaskCreate(event_get_task, (signed portCHAR *)"eventGet", 100, NULL, 2, NULL);

   printf("FreeRTOS is starting ...\n");

    vTaskStartScheduler();
		
    for(;;);
}
