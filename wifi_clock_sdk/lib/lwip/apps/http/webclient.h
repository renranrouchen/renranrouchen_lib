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
#define HTTP_URI		"/?app=weather.today&weaid=100&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json"	
/*TODO:
*add init function to initialized thoes variable
*/
struct httpClient {	
	char isopen;
	char state;
	int port;
	int clientfd;
	char* iaddr;
	char* uri;	
	const char* host;
	char* postData;
};

//extern struct httpClient webClient;
typedef int (*data_handle_callback)(char* buff,int len);

int connectServer(); 
int readData(int clientfd,data_handle_callback);
//int httpPost();
int httpGet(int clientfd,const char* uri,const char* host);
int httpPost(int clientfd,const char* host,const char* uri,const char* postData);
void writeData(char* str);
int head_check(const char* buff);




#endif
