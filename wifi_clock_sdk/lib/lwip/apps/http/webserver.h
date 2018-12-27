/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      webserver.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-17 16:08:18
 * ModifiedTime:  2017-05-17 16:08:18
 */



typedef struct httpserver{
#define WEBSERVER_NAME	"zwxf";		
	int port;
	int server_fd;
	
}webserver;
int webserver_startup();
webserver get_webserver(void);


