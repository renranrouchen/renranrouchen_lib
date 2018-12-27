/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf.cw@gmail.com
*@Brief			
*@FileName		pwm_alarm.c
*@Data			2017/9/13 15:36:59
-------------------------------------------------------------------------*/
#include <common.h>
#ifdef CONFIG_FREERTOS
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#endif
#include <alarm.h>
#include <pwm.h>
#include <gpio.h>
#include <event_process.h>
#if (VERSION_MUSHAN == 1)
#define PWM_PIN		2
#endif

#if (VERSION_DEMO == 1 || CUSTOMER_VERSION == 1)
#define PWM_PIN		18
#define SOUND_PIN	0
#define VIBRATE_PIN	17
#endif

/* 4KHZ */
#define HZ		5000
#define SOUND_DUTY_TIME(hz)		((1 * 1000 * 1000) / (hz * 2))
char isAlarm = 0;

void pwm_init(unsigned char pin)
{
	pin_mode(pin,GPIO_OUT);
	gpio_enable(pin,ENABLE);
	digital_write(pin,LOW);
}

void start_pwm_alarm(void* param)
{
	printf(" \r\n start alarm");
#define SOUND_DELAY			60	
	int alarmDelay = SOUND_DELAY;
	isAlarm = 1;
	while (alarmDelay--) {
//		digital_write(PWM_PIN,LOW);
//		udelay(SOUND_DUTY_TIME(HZ));
		digital_write(PWM_PIN,HIGH);
		sys_msleep(100);
		digital_write(PWM_PIN,LOW);
		sys_msleep(100);
		digital_write(PWM_PIN,HIGH);
		sys_msleep(100);
		digital_write(PWM_PIN,LOW);
		sys_msleep(600);		
//		udelay(SOUND_DUTY_TIME(HZ));
		printf("alarm \n");
	}
	isAlarm = 0;
	printf(" \r\n end alarm");
	/*Notic to reset to NULL */
#if VERSION_MUSHAN
	v2Alarm.pAlarmHandle = NULL;
#endif
#if (VERSION_DEMO || CUSTOMER_VERSION)
	alarm.pHandle = NULL;	
#endif
	vTaskDelete(NULL);
}




