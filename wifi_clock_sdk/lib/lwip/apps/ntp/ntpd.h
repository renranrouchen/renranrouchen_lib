/*
 *  * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 *   */
/*
 *  * FileName:      ntpd.h
 *   * Author:        zwxf
 *    * Email:         zhiwei1007@qq.com
 *     * DateTime:      2017-05-22 15:26:55
 *      * ModifiedTime:  2017-05-22 15:26:55
 *       */

#ifndef NTPD_H
#define NTPD_H
#include "ntp_time.h"


/* for judge webtime */
//extern sdk_param g_atParam;


#define SECS_24_HOUR	86400
#define NTP_UPDATE_INTERVAL 3600
#define NTP_SERVER_PORT 123
#define SOCKET_ERROR -1
#define NTPFRAC(x) (4294*(x)+((1981*(x))>>11))
#define JAN_1970 0x83aa7e80  /* 2208988800 1900 - 1970 in seconds  MSB == 1*/
#define JAN_2036 0x7c558180  /* 2085976496 1970 - 2036 in seconds MSB == 0 */

#define USEC(x) ( ( (x) >> 12 ) - 759 * ( ( ( (x) >> 10 ) + 32768 ) >> 16 ) )
enum{
	GET_TIME_FAIL = 0,
	GET_TIME_SUCC,
};

typedef void(*time_process_callback)(sntp_tm time,long timeVal);

int getUTCtime(char *server_name,time_process_callback); 

#endif
