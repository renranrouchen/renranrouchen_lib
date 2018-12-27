/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      httpclient.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-17 08:39:40
 * ModifiedTime:  2017-05-17 08:39:40
 */

//linux head file
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
#include <sys/time.h>
#include "webclient.h"
#include "webstring.h"
/*lwip head files

#include "lwip/sockets.h"
#include "lwip/inet.h"

#include "common.h" 

#include "webclient.h"
#include "webstring.h"
*/
//#define printf(fmt,args...)	serial_printf (fmt ,##args)

struct httpClient webClient;
static unsigned char *stringCopy(unsigned char *dest,
	    const unsigned char *src, unsigned char len)
{
  strncpy(dest, src, len);
  return dest + len;
}


char *strcat(char *dest, const char *src)
{
	char *tmp = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++) != '\0')
		;
	return tmp;
}

/*
static int strcmp(const char *cs, const char *ct)
{
	unsigned char c1, c2;

	while (1) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}
	return 0;
}
*/

/* Call back function */
void httpDataHandle(char* data,int len)
{
	while(len--){
		//printf(" \n");
		printf("%c",*data++);
	}
}



int httpGet(int clientfd,const char* uri,const char* host)
{
	int ret = -1;
	char buff[256];
	const char* tmp;
	
	memset(buff,0,256);
	/* method */
	strcat(buff,http_get);
	/* URI */
	strcat(buff,uri);
	/* space */
	strcat(buff,http_space);
	/* http version */
	strcat(buff,http_11);
	/* CRLF */
	strcat(buff,http_crnl);

	/* host */
	strcat(buff,http_host);
	strcat(buff,host);
	/* CRLF */
	strcat(buff,http_crnl);
	/* connection */
	//strcat(buff,http_connection);
	//strcat(buff,http_keep_alive);
	//strcat(buff,http_crnl);

	/* Cache-Control: max-age=0  */
	//strcat(buff,http_cache_control);
	//strcat(buff,http_crnl);

	//strcat(buff,http_accept);
	//strcat(buff,http_crnl);

	/* space line */
	/* CRLF */
	strcat(buff,http_crnl);
	
	printf("httpGet%s",buff);
	
	ret = write(clientfd,buff,strlen(buff));
	if(ret < 0){
		printf("write fail \n ");
		return -1;
	}
	return clientfd;
}

int httpPost(int clientfd,const char* host,const char* uri,const char* postData)
{
	int ret = -1;
	char* bp = NULL;
	char buff[512];
	bp = buff;
	if (buff == NULL) {
		printf("malloc fail \n");
		return -1;
	}
	/* post line */
	bp = stringCopy(bp,http_post,strlen(http_post));
	bp = stringCopy(bp,uri,strlen(uri));
	bp = stringCopy(bp,http_space,strlen(http_space));
	bp = stringCopy(bp,http_11,strlen(http_11));
	bp = stringCopy(bp,http_crnl,strlen(http_crnl));
	
	/* host */
	bp = stringCopy(bp,http_host,strlen(http_host));
	bp = stringCopy(bp,host,strlen(host));
	bp = stringCopy(bp,http_crnl,strlen(http_crnl));

	/* content lenth */
	bp = stringCopy(bp,http_content_len,strlen(http_content_len));
	ret = sprintf(bp,"%ld",strlen(webClient.postData));
	if (ret < 0) {
		printf("package post string erro \n");
		return ret;
	}
	bp += ret;
	bp = stringCopy(bp,http_crnl,strlen(http_crnl));
	/* new line */
	bp = stringCopy(bp,http_crnl,strlen(http_crnl));
	/* content */
	bp = stringCopy(bp,postData,strlen(postData));
	
	printf("%s",buff);
	printf("%d",clientfd);
	ret = write(clientfd,buff,strlen(buff));
	if (ret < 0) {
		printf("write fail \n");
		goto erro;
	}
	return clientfd;
erro:
	return -1;
}

int connectServer() 
{
	//char ch = 'A';
	struct sockaddr_in serverAddr;
	int ret = -1,clientfd = -1;
	clientfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(clientfd < 0){
		printf("create socket fail \n");
		return 0;
	}
	serverAddr.sin_addr.s_addr = inet_addr(webClient.iaddr);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(webClient.port);
	
	ret = connect(clientfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
	if(ret < 0){
		printf("connect error \n");
		return 0;
	}
	webClient.clientfd = clientfd;
	/*
	ret = write(clientfd,&ch,1);
	if(ret < 0){
		printf("write fail \n ");
	}
	*/
	return clientfd;
}


void writeData(char* str)
{
	int ret = -1;
	ret = write(webClient.clientfd,str,strlen(str));
	if(ret < 0){
		printf("write fail \n");
	}
}
void readData(int clientfd)
{
	int ret = -1;
	char revBuff[512];
	static fd_set fd_s;
	static struct timeval tv;
	/*
	revBuff = (char* )malloc(sizeof(char) * 128);
	if(revBuff == NULL){
		printf("alloc fail \n");
		return;	
	}
	*/	
	FD_ZERO(&fd_s);
	FD_SET(clientfd,&fd_s);
	printf("start select \n");
	while(1){
		//sys_msleep(1000);
		tv.tv_sec = 0;
		tv.tv_usec = 1000;
		ret = select(clientfd + 1,&fd_s,NULL,NULL,&tv);
		printf("select block \n");
		if(ret < 0){
			printf("select fail \n ");
			return;
		}
		else{
			memset(revBuff,0,512);
			ret = read(clientfd,revBuff,512);
			if(ret < 0){
				printf("read fail \n ");
				return;
			}
			return httpDataHandle(revBuff,strlen(revBuff));	
			//httpDataHandle(revBuff,64);
		}
	}
	free(revBuff);
}

void main(int argc,int*argv[])
{	
	int ret = -1;
	long long i;
	webClient.port = 80;
	webClient.iaddr = "121.201.5.79";
//	webClient.iaddr = "192.168.0.167";
	ret = connectServer();
	if(ret < 0){
		printf("connect fail \n");
		return;
	}
	webClient.host = "api.k780.com";
	webClient.uri = "?app=weather.today&weaid=100&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json";
	while(1){	
	httpGet(ret,webClient.uri,webClient.host);	
	readData(ret);
	for(i = 0;i < 1999999999;i++);
	}
}

