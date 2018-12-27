/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      http_server.h
 * Author:        zwxf
 * Email:         zwxf.cw@gmail.com
 * DateTime:      2017-07-20 16:12:44
 * ModifiedTime:  2017-07-20 16:12:44
 */
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
/*
{
	"Demo_AP1", 			//ap ssid
	"12345678", 			//ap key
	11, 					//channel
	SECURITY_WPA2_MIXED,	//enc
	4,
	0,
	"192.168.169.1",		//ip
	"255.255.255.0",		//mask
	"192.168.169.1",		//gw
},
*/

#define WEB_SERVER_SSID		"Demo_AP1"
#define WEB_SERVER_KEY		"12345678"
#define WEB_SERVER_CHANNEL	2
#define WEB_SERVER_IP		"192.168.1.1"
#define WEB_SERVER_MASK		"255.255.255.0"
#define WEB_SERVER_GW		"192.168.1.1"
#define WEB_SERVER_HIDDEN_SSID	0

void http_server_start(void* param);






#endif

