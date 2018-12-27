/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      event_loop.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-19 10:03:20
 * ModifiedTime:  2017-06-19 10:03:20
 */
#include <common.h>
 
#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#endif 
#include <log/log.h>
#include <event_loop.h>

#define EVENT_QUEUE_SIZE	10
#define EVENT_QUEUE_DELAY	10

static QueueHandle_t eventQueue;
event_func eventFuncTable[EVENT_MAX];

inline void add_event(int event,event_func cbFunc)	
{
	eventFuncTable[event] = cbFunc;	
}
inline void remove_event(int event,event_func cbFunc)
{
	eventFuncTable[event] = NULL;
}

void send_event(Eevent event)
{
	Tevent evt; 
	evt.event = event;
	if(eventQueue != NULL){
		if(xQueueSendToBack(eventQueue,&evt,EVENT_QUEUE_DELAY) == pdFAIL){
			printf("event queue send fail==========\r\n");
			;
		}
	}
}

void event_loop(void* prama)
{
	while(1){
		Tevent evt;
		if (xQueueReceive(eventQueue,&evt,EVENT_QUEUE_DELAY) == pdPASS) {
		//default event process
			if (eventFuncTable[evt.event] != NULL) {
				eventFuncTable[evt.event](0);
			}
		}
		sys_msleep(50);
	}
}

char event_init()
{
	eventQueue = xQueueCreate(EVENT_QUEUE_SIZE,sizeof(Tevent));
	xTaskCreate(event_loop,"evtloop",1024 * 2,NULL,6,NULL);
}


