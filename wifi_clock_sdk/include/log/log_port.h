#ifndef LOG_PORT_H
#define LOG_PORT_H
#include <common.h>
#include <FreeRTOS.h>
#include <semphr.h>
//#define printf	serial_printf
/*
int getPid(void *task)
{
    //add OS pid
    return task.pid;
}
*/
/*
int getRunTime(void *task)
{
	//add OS pid
	return task.pid;
}
*/
/*
int getTaskName(void *task)
{
	//add OS pid
	return task.pid;
}
*/
/*
int getTaskStack(void *task)
{
	//add OS pid
	return task.pid;
}
*/

static SemaphoreHandle_t logMutex = NULL;
#define WAITING_TICK	30

void outPort(char* buff,int len)
{
	int lenth = 0;
    //printf to uart,you can printf to other terminal for instance flash
    //xSemaphoreTake(logMutex,WAITING_TICK);
	while(len--){
		serial_printf("%c",buff[lenth++]);
	}
	//xSemaphoreGive(logMutex);
	//printf(buff);
}

#define lockInit()	do{ \
		logMutex = xSemaphoreCreateBinary(); \
}while(0) \

#define logLock()	xSemaphoreTake(logMutex,WAITING_TICK)
#define logUnlock()	xSemaphoreGive(logMutex)

#endif // LOG_PORT_H
