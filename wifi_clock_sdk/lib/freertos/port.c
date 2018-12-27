/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Processor constants. */
#include <arch/chip.h>
#include <arch/cpu.h>
#include <lynx_dev.h>
#include <event.h>
#include <common.h>

extern int force_reset;  
SemaphoreHandle_t dsr_sem= NULL;
//unsigned int time_base;
unsigned long utctime_g = 0;

/* Setup the timer to generate the tick interrupts. */
static void prvSetupTimerInterrupt( void );

typedef struct {
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t r13;
    uint32_t r14;
    uint32_t r15;
    uint32_t r16;
    uint32_t r17;
    uint32_t r18;
    uint32_t r19;
    uint32_t r20;
    uint32_t r21;
    uint32_t r22;
    uint32_t r23;
    uint32_t r24;
    uint32_t r25;
    uint32_t r26;
    uint32_t r27;
    uint32_t r28;
    uint32_t r29;
    uint32_t r30;
    uint32_t r31;
    uint32_t rEPCR;
    uint32_t rESP;
    uint32_t rEEAR;
} switch_frame;

/*----------------------------------------------------------------*/
/**
 *  \private
 *  \brief Enter critical section by disable interrupt
 *  \param None
 *  \return None
 */
/*----------------------------------------------------------------*/
void os_enter_critical(void)
{
    __asm__ __volatile__
    (
        "b.di\n\t"
    );
}

/*----------------------------------------------------------------*/
/**
 *  \private
 *  \brief Exit critical section by enable interrupt
 *  \param None
 *  \return None
 */
/*----------------------------------------------------------------*/
void os_exit_critical(void)
{
    __asm__ __volatile__
    (
        "b.ei\n\t"
    );
}

/*----------------------------------------------------------------*/
/**
 *  \private
 *  \brief Context Switch
 *  \param None
 *  \return None
 */
/*----------------------------------------------------------------*/
void switch_thread(void)
{
	volatile unsigned int *top_stack = (volatile unsigned int *)xTaskGetCurrentTaskHandle();

	*top_stack = ldev->current_sp;
	vTaskSwitchContext();
	top_stack = xTaskGetCurrentTaskHandle();

    ldev->current_sp = *top_stack;
}

/* 
 * Initialise the stack of a task to look exactly as if a call to 
 * portSAVE_CONTEXT had been called. Context layout is described in
 * portmarco.h
 *
 * See header file for description. 
 */
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{
	switch_frame *sf;

	// allocate redzone	
	pxTopOfStack -= REDZONE_SIZE/4;			
	sf = (switch_frame *)((unsigned int)pxTopOfStack - sizeof(switch_frame));

	sf->rEPCR = (uint32_t)pxCode;
	sf->rESP = SPR_SR_SM|SPR_SR_IEE|SPR_SR_FO;
    sf->r9 = (uint32_t)pxCode;
    sf->r3 = (uint32_t)pvParameters;
    sf->r2 = (uint32_t)ldev;
    sf->r1 = (uint32_t)sf;

	return (portSTACK_TYPE *)sf;
}

portBASE_TYPE xPortStartScheduler( void )
{
	volatile unsigned int *top_stack = (volatile unsigned int *)xTaskGetCurrentTaskHandle();

	/* FIXME:Start the timer that generates the tick ISR.  Interrupts are disabled
		here already. */

	ldev->current_sp = *top_stack;
	/* Start the first task. */
	asm("b.j  restore_reg"); 
			
	/* Should not get here! */
	return 0;
}

void vPortEndScheduler( void )
{
}

char in_interrupt_context()
{
	if(MFSPR(SPR_SR) & SPR_SR_IEE)
		return false;
	else
		return true;
}
#if ((CONFIG_ROM_VER > 1)||defined(CONFIG_FLASH_BOOT))
/*----------------------------------------------------------------*/
/**
 *  \brief 
 *  \param None
 *	\return None
 */
/*----------------------------------------------------------------*/
void os_raise_event(int event_bit)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	char in_isr;

	in_isr = in_interrupt_context();

	/* OS Tick */
	if(event_bit == TIMER_EVENT)
	{
		struct timeout_entry *entry = ldev->timeout_head;

		clock_next_event(portTICK_PERIOD_MS);

		if(TIME_AFTER_EQ(millis(), entry->expire_time))
		{
			ldev->all_event |= event_bit;
			if (in_isr)
				xSemaphoreGiveFromISR(dsr_sem, &xHigherPriorityTaskWoken);
			else
				xSemaphoreGive(dsr_sem);

		}

		if(xTaskIncrementTick() == pdTRUE )
			xHigherPriorityTaskWoken = pdTRUE;
		/* fall to trigger event */
	}
	else
	{
		ldev->all_event |= event_bit;
		if (in_isr)
			xSemaphoreGiveFromISR(dsr_sem, &xHigherPriorityTaskWoken);
		else
			xSemaphoreGive(dsr_sem);
	}

	if(in_isr && xHigherPriorityTaskWoken)
		portYIELD_FROM_ISR();
}
#else
void os_raise_event(int event_bit)
{
	BaseType_t xHigherPriorityTaskWoken;

	ldev->all_event |= event_bit;
	xSemaphoreGiveFromISR(dsr_sem, &xHigherPriorityTaskWoken);

	if(xHigherPriorityTaskWoken)
		portYIELD_FROM_ISR();
}
#endif

/*----------------------------------------------------------------*/
/**
 *  \brief 
 *  \param None
 *	\return None
 */
/*----------------------------------------------------------------*/
void dsr_thread(void *arg)
{
#if !((CONFIG_ROM_VER > 1)||defined(CONFIG_FLASH_BOOT))
	unsigned int i, ret, old, all_event;
	struct event_entry *evt_e;
#endif
	int wd_tmr = 5*32000;		// 5 seconds

	while(1)
    {
		if(!force_reset)
			/* XXX: reflash watchdog */
			PMUREG(SLP_WDOG_TMR_CTRL) = SLP_WDOG_TMR_EN|(wd_tmr);

#if ((CONFIG_ROM_VER > 1)||defined(CONFIG_FLASH_BOOT))
		handle_events();
#else
		old = irq_save();
		
		all_event = ldev->all_event;
		ldev->all_event = 0;
		
		irq_restore(old);

		while(all_event)
		{
			i = ffs(all_event);
			if(i)
			{
				i -= 1;
				evt_e = &((struct event_entry *)(ldev->event_table))[i];
				ldev->current_event = (u32)evt_e;
				if(evt_e->func)
				{
					ret = evt_e->func(i);

					/* unregister event if pt is exited */
					if(ret == PT_EXITED)
						evt_e->func = 0;	
				}

				all_event &= ~(1 << i);
			}
			else
				break;
		}
#endif	
		if(ldev->all_event == 0)
		{
			xSemaphoreTake(dsr_sem, -1);
		}
    }

}

/*----------------------------------------------------------------*/
/**
 *  \brief 
 *  \param None
 *	\return None
 */
/*----------------------------------------------------------------*/
void timer_trigger(void)
{
	add_timeout((FUNCPTR)timer_trigger, 0, 3000);
}


/*----------------------------------------------------------------*/
/**
 *  \brief 
 *  \param None
 *	\return None
 */
/*----------------------------------------------------------------*/
void os_init(void)
{
	ldev->raise_event = os_raise_event;
#if ((CONFIG_ROM_VER > 1)||defined(CONFIG_FLASH_BOOT))
	ldev->os_tick = 1;
	clock_next_event(portTICK_PERIOD_MS);
#endif

	dsr_sem = xSemaphoreCreateBinary();

	xTaskCreate(dsr_thread, "dsr", 4096, 0, configMAX_PRIORITIES-1, NULL);
	add_timeout((FUNCPTR)timer_trigger, 0, 3000);
	vTaskStartScheduler();
}

/*----------------------------------------------------------------*/
/**
 *  \brief 
 *  \param None
 *	\return None
 */
/*----------------------------------------------------------------*/
void vApplicationIdleHook( void )
{
	if (ldev->dsr_task_idle)
		ldev->dsr_task_idle();
}

/*----------------------------------------------------------------*/
/**
 *  \brief 
 *  \param None
 *	\return None
 */
/*----------------------------------------------------------------*/
unsigned long time(unsigned long val)
{
#if 0
	unsigned int current_rtc = PMUREG( RTC_TMR_CLK);
	unsigned int delta_time;

	delta_time = current_rtc/32000;

	PMUREG( RTC_TMR_CLK) = RTC_CLK_CLR|RTC_EN;

	time_base += delta_time;

	return (time_base);
#else
	unsigned long tmp = utctime_g + (xTaskGetTickCount()*portTICK_PERIOD_MS/1000);
	return  tmp;

#endif
}
