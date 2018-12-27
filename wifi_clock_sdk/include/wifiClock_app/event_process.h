/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      event_process.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-19 14:44:17
 * ModifiedTime:  2017-06-19 14:44:45
 */
#ifndef EVENT_PROCESS_H
#define EVENT_PROCESS_H
#include "event_loop.h"

extern char powerSaveCount;
extern inline void  ENTER_POWER_SAVE();
extern inline void EXIT_POWER_SAVE();

void event_process();
extern void event_get_task(void* param);
#endif


