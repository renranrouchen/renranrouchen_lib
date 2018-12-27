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

/*linux head file
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
*/
//lwip head files
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "string.h"
#include "common.h" 

#include "lwip/netdb.h"
#include "webclient.h"
#include "webstring.h"
#include <log/log.h>
#define CURL		"User-Agent: curl/7.47.0"
#define TYPE_JSON	"Content-Type: application/json"
#define ACCEPT		"Accept: application/json"


int http_headLen(const char *httpbuf )
{
   char *p_start = NULL;
   char *p_end =NULL;
   int headlen=0;
   p_start = (char *)httpbuf;
   p_end = strstr((char *)httpbuf,http_crnlcrnl);
   if( p_end==NULL )
   {
   	   printf("\r\n=======can`t get head len=====\r\n");
       return 0;
   }
   p_end=p_end+strlen(http_crnlcrnl);
   headlen = (p_end-p_start);
   return headlen;
}

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

char* is_response_ok(const char* buff)
{
	return strstr(buff,http_200);
}

int head_check(const char* buff)
{
	int httpHeadLen = -1;
	if(is_response_ok(buff) == NULL){
		//warn("\r\n=======response fail ==============\r\n");
		return -1;
	}
	httpHeadLen = http_headLen(buff);
	if(httpHeadLen < 0){
		//warn("\r\n===========can`t get head lenth===========\r\n");
		return -1;
	}	
	return httpHeadLen;
}
#define AUTH		"authorization: Basic bXNhcGk6TSRBcDE="
int httpGet(int clientfd,const char* uri,const char* host)
{
	int ret = -1;
	char* buff = (char* )malloc(256);
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

	/* authorization */
	strcat(buff,AUTH);
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
	sys_msleep(10);
	printf("%s",buff);
  	//printf("start__write_buff--------------->>\r\n");
	ret = write(clientfd,buff,strlen(buff));
	//printf("over__write_buff--------------->>\r\n");
	sys_msleep(100);
	if(ret < 0){
		printf("write fail \n ");
		free(buff);
		//pbuf_free(buff);
		buff = NULL;
		return -1;
	}
	//pbuf_free(buff);
	free(buff);
	buff = NULL;
	return clientfd;
}

int httpPost(int clientfd,const char* host,const char* uri,const char* postData)
{	
	int ret = -1;
	char* bp = NULL;
	char* buff = NULL;
	printf("--------------->postdata %s",postData);
	bp = buff = (char* )malloc(1024);
	memset(buff,0,1024);
	if(buff == NULL){
		printf("malloc fail \n");
		return -1;
	}
	/* post line */
	bp = stringCopy(bp,http_post,sizeof(http_post) - 1);
	bp = stringCopy(bp,uri,strlen(uri));
	bp = stringCopy(bp,http_space,sizeof(http_space) - 1);
	bp = stringCopy(bp,http_11,sizeof(http_11) - 1);
	bp = stringCopy(bp,http_crnl,sizeof(http_crnl) - 1);
	
	/* host */
	bp = stringCopy(bp,http_host,sizeof(http_host) - 1);
	bp = stringCopy(bp,host,strlen(host));
	bp = stringCopy(bp,http_crnl,sizeof(http_crnl) - 1);

	/* CURL */
	bp = stringCopy(bp,CURL,strlen(CURL));
	bp = stringCopy(bp,http_crnl,sizeof(http_crnl) - 1);
	/* content type */
	bp = stringCopy(bp,TYPE_JSON,strlen(TYPE_JSON));
	bp = stringCopy(bp,http_crnl,sizeof(http_crnl) - 1);

	/* accept */
	bp = stringCopy(bp,ACCEPT,strlen(ACCEPT));
	bp = stringCopy(bp,http_crnl,sizeof(http_crnl) - 1);

	/* content lenth */
	bp = stringCopy(bp,http_content_len,sizeof(http_content_len) - 1);
	ret = sprintf(bp,"%d",strlen(postData));
	if (ret < 0) {
		printf("package post string erro \n");
		return ret;
	}
	bp += ret;
	bp = stringCopy(bp,http_crnl,sizeof(http_crnl) - 1);

	bp = stringCopy(bp,AUTH,strlen(AUTH));
	bp = stringCopy(bp,http_crnl,strlen(http_crnl));
	/* new line */
	bp = stringCopy(bp,http_crnl,sizeof(http_crnl) - 1);
	/* content */
	bp = stringCopy(bp,postData,strlen(postData));
	printf("-------->post %s",buff);
	// printf("%d %s",strlen(buff),*(bp - 1));
	
	ret = write(clientfd,buff,strlen(buff));
	if (ret < 0) { 		
		goto erro;
	}
	free(buff);
	return clientfd;
erro:
	printf("write fail \n");
	return -1;
}

int connectServer(struct httpClient* webClient) 
{
	struct sockaddr_in serverAddr;
	struct hostent *hp;
	unsigned int ip;
	char *addr;
	int ret = -1,clientfd = -1;

	clientfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (clientfd < 0) {
		printf("create socket fail \n");
		return -1;
	}
	
	if (NULL == (hp = lwip_gethostbyname(webClient->host)))
	{
		printf("can't resolute the host address \n");
		return -1;
	}
	ip = *(unsigned int *)(hp->h_addr);
	addr = inet_ntoa(*(unsigned int *)hp->h_addr);
	serial_printf("address: %s\n", addr);
	
	serverAddr.sin_addr.s_addr = ip;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(webClient->port);
	
	ret = connect(clientfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
	if(ret < 0){
		printf("connect error \n");
		return -1;
	}
	webClient->clientfd = clientfd;
	webClient->isopen = 1;
	
	return clientfd;
}
inline int readData(int clientfd,int (*data_handle_callback)(char*,int))
{
	int ret = -1;
	char* revBuff = (char* )malloc(1024);
	static fd_set fd_s;
	static struct timeval tv;
	
	FD_ZERO(&fd_s);
	FD_SET(clientfd,&fd_s);

	while(1) {
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		ret = select(clientfd + 1,&fd_s,NULL,NULL,&tv);
		if (ret <= 0) {
			printf("select fail \n ");
			goto fail;
		} else {
			memset(revBuff,0,1024);
			sys_msleep(100);
			//printf("start__read_buff--------------->>\r\n");
			ret = read(clientfd,revBuff,1024);
			//printf("over__read_buff--------------->>\r\n");
			mallinfo(0, 0);
			if (ret <= 0) {
				printf("read fail \n ");
				goto fail;
			}
			if (data_handle_callback == NULL) {
				printf("callback function is null\n");
				goto fail;
			} else {
				printf("revBuff_value:%s",revBuff);
				printf("=========================\r\n");
				ret = data_handle_callback(revBuff,ret);
				sys_msleep(10);
				//pbuf_free(revBuff);
				free(revBuff);
				revBuff = NULL;
				return ret;
			}
		}
	}
fail:
	//pbuf_free(revBuff);
	free(revBuff);
	revBuff = NULL;
	return -1;
}


