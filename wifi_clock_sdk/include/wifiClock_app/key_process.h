/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      key_process.h
 * Author:        zwxf
 * Email:         zwxf.cw@gmail.com
 * DateTime:      2017-10-18 08:42:31
 * ModifiedTime:  2017-10-18 08:42:31
 */

#ifndef KEY_PROCESS_H
#define KEY_PROCESS_H

#define KEY_PRESSED		0

#define KEY_UP_PIN					0
#define KEY_DOWN_PIN				1	
#define KEY_ALARM_SETTING_PIN		4
#define KEY_BRIGHTNESS_PIN			18
#define KEY_SILENCE_PIN				3


extern void silence_key_process();
extern void brightness_key_process();
extern void alarm_setting_key_process();
extern void down_key_process(); 
extern void up_key_process();
extern void key_pressed_process();
extern void RH_ADC_value_get();
extern unsigned char silencePressed;
extern unsigned char keyPressFlag;
extern char keyTable[4];

#endif

