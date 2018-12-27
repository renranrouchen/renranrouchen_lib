/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf.cw@gmail.com
*@Brief			
*@FileName		get_event.c
*@Data			2017/7/19 16:35:14
-------------------------------------------------------------------------*/
#include <common.h>

#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#include <wifi_clock_cfg.h>
#include <gpio.h>
#include <omniconfig.h>
#include <event_process.h>
#include <FD612demo_app.h>
#include <wifi_time.h>
#include <wifi_weather.h>
#include <wifi_pm25.h>
#include <event_process.h>
#include <key_process.h>

#if (VERSION_DEMO || CUSTOMER_VERSION)
#define SOUND_PIN 0
#define VIBRATE_PIN 17
#define EVENT_DELAY(ms) 1000 * ms
inline void check_sound_event()
{
	char retry = 0;
	int highVolCount = 0;
	if (digital_read(SOUND_PIN) == HIGH)
	{
		udelay(EVENT_DELAY(5));
		if (digital_read(SOUND_PIN) == HIGH && FD612Test.displayOn != ON)
		{
			if (FD612Test.displayOn == MODE_ONE)
			{
				udelay(EVENT_DELAY(2000));
				if (digital_read(SOUND_PIN) == HIGH)
				{
					send_event(CLAP_ONE_EVENT);
					printf(" \r\n handle clap one \r\n");
				}
			}
			else
			{
				send_event(CLAP_ONE_EVENT);
			}
		}
		else
		{
			sys_msleep(100);
			//udelay(EVENT_DELAY(100));
		}
	}
}

inline void check_vibrates_event()
{
	if (digital_read(VIBRATE_PIN) == LOW)
	{
		sys_msleep(800);
		//udelay(EVENT_DELAY(100));
		if (digital_read(VIBRATE_PIN) == LOW)
		{
			send_event(VIBRATE_EVENT);
		}
		printf("\r\n handle vibrate \r\n");
	}
}
inline void check_config_key_event()
{
	if (digital_read(CONFIG_KEY) == LOW)
	{
		send_event(CONFIG_KEY_EVENT);
	}
	// if (digital_read(CONFIG_KEY) == HIGH)
	// {
	// 	delete_v2_alarm();
	// }
}

void check_entry_poweroff()
{
	static char firstEntryCheck = FIRST_ENTRY;
	if (firstEntryCheck != FIRST_ENTRY)
	{
		return;
	}
	if (wifiTime.update.firstEntryCheck == NOT_FIRST_ENTRY &&
		weather.update.firstEntryCheck == NOT_FIRST_ENTRY &&
		wifiPm25.update.firstEntryCheck == NOT_FIRST_ENTRY)
	{
		//ENTER_POWER_SAVE();
		firstEntryCheck = NOT_FIRST_ENTRY;
	}
}

#endif /* (VERSION_DEMO || CUSTOMER_VERSION) */
void init_peripheral()
{
	unsigned char loop = 0;
#if (VERSION_DEMO || CUSTOMER_VERSION)
	pin_mode(SOUND_PIN, GPIO_IN);
	pin_mode(VIBRATE_PIN, GPIO_IN);
	pin_mode(CONFIG_KEY, GPIO_IN);

	gpio_pullup(SOUND_PIN, 0);
	gpio_pullup(VIBRATE_PIN, 0);
#endif

#if VERSION_MUSHAN
	for (; loop < 4; loop++)
	{
		gpio_enable(keyTable[loop], ENABLE);

		pin_mode(keyTable[loop], GPIO_OUT);
		digital_write(keyTable[loop], HIGH);
		pin_mode(keyTable[loop], GPIO_IN);
		// gpio_pullup(keyTable[loop],0);
		printf("io[%d] init \n", keyTable[loop]);
		udelay(10);
	}
	gpio_enable(KEY_SILENCE_PIN, ENABLE);
	gpio_pullup(KEY_SILENCE_PIN, 0);
	pin_mode(KEY_SILENCE_PIN, GPIO_OUT);
	digital_write(KEY_SILENCE_PIN, HIGH);
	pin_mode(KEY_SILENCE_PIN, GPIO_IN);

#endif /* VERSION_MUSHAN */
	   //gpio_pullup(CONFIG_KEY,0);
}

#if (VERSION_MUSHAN == 1)
void check_key_press_event()
{
#define MAX_COUNT_TIME 0
	static unsigned char eventSenderCount = 0;
	static unsigned char countTime = 0, silenceKeyCount = 0;
	static unsigned char oldKeyNum = 0xFF, newKeyNum = 0xFF;
	static unsigned char configTimeDelay = 0;
	unsigned char loop = 0;
	/*FIXME:
	*when alarm up will cause key press msg
	*/
	if (silenceKeyCount++ > 2)
	{
		if (digital_read(KEY_SILENCE_PIN) == KEY_PRESSED)
		{
			silencePressed = true;
			//			printf("silence key is true!!!\n");
		}
		else
		{
			silencePressed = false;
		}
		send_event(SILENCE_KEY_EVENT);
		silenceKeyCount = 0;
	}

	/* elimination dithering process */
	for (loop = 0; loop < 4; loop++)
	{
		if (digital_read(keyTable[loop]) == KEY_PRESSED )
		{
			oldKeyNum = newKeyNum;
			newKeyNum = loop;
			printf("pressed key %d old key %d count[%d] loop[%d]\n", newKeyNum, oldKeyNum, countTime, loop);
			if (newKeyNum != oldKeyNum && oldKeyNum != 0xFF)
			{
				countTime = 0;
			}
			else
			{
				countTime++;
				// entry config mode long keypress
				if (countTime > CINFIG_TIME_DELAY && keyTable[newKeyNum] == KEY_BRIGHTNESS_PIN)
				{
					goto configMode;
				}
				// alarm setting long key pressed
				if (countTime > MAX_COUNT_TIME + 5 && (keyTable[newKeyNum] == KEY_DOWN_PIN || keyTable[newKeyNum] == KEY_UP_PIN))
				{
					if (eventSenderCount++ > 3)
					{
						send_event(UP_KEY_EVENT + newKeyNum);
						send_event(KEY_PRESSED_EVENT);
						eventSenderCount = 0;
					}
				}
			}
			return;
		}
	}
	if (countTime > MAX_COUNT_TIME && countTime < MAX_COUNT_TIME + 5)
	{
		goto keyPressed;
	}
	countTime = 0;
	return;
keyPressed:
	send_event(UP_KEY_EVENT + newKeyNum);
	send_event(KEY_PRESSED_EVENT);
	countTime = 0;
	return;
configMode:
	send_event(CONFIG_MODE);
	countTime = 0;
	return;
}

void check_local_temperature_event(void)
{
	/* TODO:Check first entry */
	static unsigned int eventSender = 0;
	if (eventSender++ > LOCAL_TEMP_EVENT_SEND_DELAY)
	{
		send_event(GET_LOCAL_TEMP_VALUE);
		eventSender = 0;
	}
}
#endif

void event_get_task(void *param)
{
	init_peripheral();
	while (1)
	{
#if (VERSION_MUSHAN == 0)
		check_sound_event();
		check_vibrates_event();
		check_config_key_event();
		check_entry_poweroff();
#else
		check_key_press_event();
		check_local_temperature_event();
#endif
		sys_msleep(100);
	}
}
