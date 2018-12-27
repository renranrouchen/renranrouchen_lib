/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      webclient.h
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-17 16:07:31
 * ModifiedTime:  2017-05-17 16:07:31
 */

#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#define HTTP_HOST_NAME	"api.k780.com"
#define HTTP_URI		"app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json"	
struct httpClient {
	char state;
	int port;
	int clientfd;
	char* iaddr;
	char* uri;	
	char* host;
	char* postData;
};

extern struct httpClient webClient;

int connectServer(); 
void readData(int clientfd);
//int httpPost();
int httpGet(int clientfd,const char* uri,const char* host);
int httpPost(int clientfd,const char* host,const char* uri,const char* postData);
void writeData(char* str);



#endif
