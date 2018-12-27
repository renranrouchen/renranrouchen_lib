#ifndef TOUCH_KEY_H
#define TOUCH_KEY_H
#include "myinclude.h"

#define TouchKeyOFF() {TKEY_CTRL &= 0xF8;}                                     //关闭电容检测，仅作1ms或者2ms定时中断
#define TouchKeyON_NoChannel() {TKEY_CTRL = TKEY_CTRL & 0xF8 | 7;}             //开启电容检测，但是不连接通道
#define TouchKeyQueryCyl1ms() {TKEY_CTRL &= ~bTKC_2MS;}                        //触摸按键采样周期设置1ms
#define TouchKeyQueryCyl2ms() {TKEY_CTRL |= bTKC_2MS;}                         //触摸按键采样周期设置2ms
extern u8 WaveBufe[6];

void TouchKeyChannelQuery();

#endif
