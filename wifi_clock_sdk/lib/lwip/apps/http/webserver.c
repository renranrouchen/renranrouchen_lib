/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      httpserver.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-17 08:39:48
 * ModifiedTime:  2017-05-17 08:39:48
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "webstring.h"
#include "webserver.h"

static webserver webServer;

webserver get_webserver(void)
{
	return webServer;
}

int parse_post(char* buff)
{
	while(1){
		printf("post %s",buff);
	}
}

int parse_get(char* buff)
{
	while(1){
		printf("get %s",buff);
	}
}
unsigned char* read_line(int clientfd,char* buff,int buffLen)
{
	unsigned char* revHead[10];
	int loop = 1,ret = -1;
	char* tmp = buff;
	revHead[0] = buff;
	
	ret = recv(clientfd,tmp,buffLen,0);
	if(ret < 0){
		printf("receive erro \n");
		return NULL;
	}
	printf("buff %s",buff);
		
	while(*tmp != '\0'){
		while(*tmp != '\n'){
			tmp++;
		}
		revHead[loop++] = ++tmp;
	}
	
	printf("buff %s",revHead[0]);
	return revHead[0];
}
static int accept_transform(int clientFd)
{
	char revBuff[512];
	char ret = -1;
	int strlen;
	
	read_line(clientFd,revBuff,sizeof(revBuff));
	bool is_post = strncmp(http_post,revBuff,sizeof(http_post) - 1);
	if(is_post == 0){
		parse_post(revBuff + sizeof(http_post) - 1);
		return 1;
	}
	bool is_get = strncmp(http_get,revBuff,sizeof(http_get) - 1);
	printf("revbuff %s %d",revBuff,is_get);
	if(is_get == 0){
		parse_get(revBuff + sizeof(http_get) - 1);
		return 1;
	}
	printf("unimplement \n");
	//unimplement();
	return -1;
	
}

int webserver_startup(void)
{
	struct sockaddr_in clientAddr,serverAddr;
	int clientLen = sizeof(clientAddr);
	int serverLen = sizeof(serverAddr);
	int clientFd = -1,ret = -1;
	webServer.port = 666;
	webServer.server_fd = -1;

	memset(&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(get_webserver().port);

	webServer.server_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(webServer.server_fd < 0){
		printf("get serverfd fail \n");
		goto fail;
	}
	
	ret = bind(webServer.server_fd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
	if(ret < 0){
		printf("bind fail \n");
		goto fail;
	}

	if(webServer.port == 0){
		if(getsockname(webServer.server_fd,(struct sockaddr*)&serverAddr,&serverLen) < 0){
			printf("get port fail \n");
			goto fail;
		}
		webServer.port = ntohs(serverAddr.sin_port);
	}
	
	if(listen(webServer.server_fd,5) < 0){
		printf("listen fail \n");
		goto fail;
	}
	printf("http sever is runing port %d",webServer.port);
	
	while(1){
		printf("\nstart polling \n");
		clientFd = accept(webServer.server_fd,(struct sockaddr*)&clientAddr,&clientLen);
		if(clientFd < 0){
			printf("accept fail \n");
			goto fail;
		}

		printf("client ipaddr is %s port is %d \n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));
		ret = accept_transform(clientFd);
		if(ret < 0){
			//close(clientFd);
		}
	}
fail:
	return -1;
}

int main(void)
{
	webserver_startup();	
}

