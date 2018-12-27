/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      app_config.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-06-02 09:03:28
 * ModifiedTime:  2017-06-02 09:03:28
 */
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

//#include <cfg_api.h>
#include <lwip/sockets.h>
#include <webserver.h>


#define MAX_SERVER_NUM	4


typedef struct {
	int skaddrLen;
	int srvFd;
	unsigned int srvPort;
	struct sockaddr_in srvAddr;

	TclientMsg clientMsg;
	unsigned int cltPort;
	struct sockaddr_in cltAddr;
}socket_param;

typedef struct {
	socket_param skPam;
}app_config_param;

void config_param();
#endif


