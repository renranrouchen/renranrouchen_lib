
#include <common.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <log/log.h>
#include <cfg_api.h>
#include <wla_api.h>

#include <webserver.h>
#include <wifi_clock_cfg.h>
extern sdk_param sdkParam;
extern sdk_param g_defparam;

#define SSID		"SSID="
#define	PWSD		"PSWD="
#define TIMEZONE	"timezone="
#define	WTH_UPDATE	"wthupdate="
#define	WIFI_TIME	"wifitime="
#define	NOWTIME		"nowtime="

TclientMsg clientMessage;
static unsigned char clientIndex = 0;
static int operateClientIndex;

char is_request(char* buff)
{
	if(strncmp("GET",buff,3) == 0){
		return IS_GET;
	}else if(strncmp("POST",buff,4) == 0){
		return IS_POST;
	}
	return -1;
}
inline char* parse_uri(char* buff)
{
	return (buff + 5);
}
static int Http_HeadLen(char* httpbuf )
{
   char* p_start = NULL;
   char* p_end = NULL;
   int headlen= 0;
   p_start = (char *)httpbuf;

   p_end = strstr((char *)httpbuf,CRLF_ENDLINE);

   if( p_end == NULL )
   {
       printf("Can't not find the http head!");
       return 0;
   }
   p_end=p_end+strlen(CRLF_ENDLINE);
   headlen = (p_end-p_start);
   return headlen;
}

char* get_param_addr(char* searcher,char* lookfor)
{
	char* addr = NULL;
	char strLen;
	addr = strstr(searcher,lookfor);
	if(addr == NULL){
		return NULL;
	}
	strLen = strlen(lookfor);
	return (addr + strLen);
}
char get_param_len(char* buff)
{
	char lenth = 0;
	while(*buff++ != '&'){
		lenth++;
	}
	return lenth;
}
char* parse_post_data(char* buff)
{
	char* ssid,*pswd,*timeZone,*wthUpdate,*wifiTime,*nowTime;
	struct sdk* tmpParam = (sdk_param* )malloc(sizeof(sdk_param));
	config_get(tmpParam);
	buff += Http_HeadLen(buff);	
	ssid = get_param_addr(buff,SSID);

	memcpy(&sdkParam,&g_defparam,sizeof(sdk_param));
	strcpy(sdkParam.sta_param[2].ssid,tmpParam->sta_param[1].ssid);
	strcpy(sdkParam.sta_param[2].key,tmpParam->sta_param[1].key);

	strcpy(sdkParam.sta_param[1].ssid,tmpParam->sta_param[0].ssid);
	strcpy(sdkParam.sta_param[1].key,tmpParam->sta_param[0].key);

	free(tmpParam);
	if(ssid != NULL)
	{
		memset(sdkParam.sta_param[0].ssid,0,32);
		memcpy(sdkParam.sta_param[0].ssid,ssid,get_param_len(ssid));
	}
	pswd = get_param_addr(buff,PWSD);
	if(pswd != NULL)
	{
		memset(sdkParam.sta_param[0].key,0,64);
		memcpy(sdkParam.sta_param[0].key,pswd,get_param_len(pswd));
	}
	timeZone = get_param_addr(buff,TIMEZONE);
	if(timeZone != NULL)
	{
		sdkParam.web_param.timeZone = atoi(timeZone);
	}
	wthUpdate = get_param_addr(buff,WTH_UPDATE);
	if(wthUpdate != NULL)
	{
		sdkParam.wth_parma.updateInterval = atoi(wthUpdate);
	}
	wifiTime = get_param_addr(buff,WIFI_TIME);
	if(wifiTime != NULL)
	{
		sdkParam.web_param.updateInterver = atoi(wifiTime);
	}

	sdkParam.work_mode = OPMODE_STA;
	sdkParam.flags |= CFG_AUTO_CONN;
	config_set(&sdkParam);
	config_submit();
	/* dump the param */
	config_dump();
//	debug("\r\n===========zwxf modify default param===============\r\n");
	return buff;
}

void handle_post(char* buff)
{
	char* sendData = malloc(1024);	
	buff = parse_post_data(buff);

	printf("\r\n starting parse data length is %d\r\n",strlen(buff));
//	assert("%s",buff);
	sprintf(sendData,RESPONSE_OK,strlen(MODIFY_OK_RESPONSE));
	memcpy((sendData + strlen(sendData)),MODIFY_OK_RESPONSE,3);
	printf("%s %d",
			sendData,
			operateClientIndex);
	send(operateClientIndex,sendData,strlen(sendData),0);
	free(sendData);
}

char check_valid_request(char* buff)
{
	char* uri = NULL;
	uri = parse_uri(buff);
	if(strncmp("favicon",uri,6) == 0)
	{
		return FAVICON_REQUEST;
	}else if(strncmp("FDHISI",uri,6) == 0)
	{		
		return VALID_URI;
	}else if(*uri == ' ')
	{
		return VALID_URI;
	}else
	{
		return NOT_FOUND;
	}
}

void handle_get(char* buff)
{	
	char requestData;
	char* sendBuff = NULL;
	sendBuff = malloc(1024 * 4);
	if(sendBuff == NULL){
		printf("\r\n malloc fail \r\n");
		return;
	}	
	requestData = check_valid_request(buff);	
	memset(sendBuff,0,1024 * 4);
	switch (requestData){
		case VALID_URI:
			sprintf(sendBuff,RESPONSE_OK,strlen(SEND_MSG));
			memcpy((sendBuff + strlen(sendBuff)),SEND_MSG,2344);
			break;
		case FAVICON_REQUEST:	
			//sprintf(sendBuff,RESPONSE_OK,strlen(SEND_MSG),SEND_MSG);
			printf("%s",sendBuff);
			break;
		case NOT_FOUND:
			sprintf(sendBuff,RESPONSE_NOT_FOUND);
			break;
		default:
			break;
	}
	send(operateClientIndex,sendBuff,strlen(sendBuff),0);
	free(sendBuff);
}

static void handle_data_cb(int* clientIndex,char* buff,int len)
{
	int loop = 0;
	char checkRequest = 0;
	operateClientIndex = *clientIndex;
	/*
	while(len--){
		printf("%c",buff[loop++]);
	}
	*/
	checkRequest = is_request(buff);
	if(checkRequest > 0){
		if(checkRequest == IS_POST){
			//assert("is post \r\n");
			handle_post(buff);	
		}else{
			//assert("is get \r\n");
			handle_get(buff);
		}	
	}else{
		printf("\r\n not request or unimplement \r\n");
	}
}

void start_get_msg(int fd,data_handle_cb cb)
{
	char* buff = NULL;
	int ret = -1;
	buff = (char* )malloc(1024);
	if(buff == NULL){
		printf("\r\n malloc fail \r\n");
	}
	memset(buff,0,1024);
	ret = read(fd,buff,1024);
	if(ret < 0){
	//	printf("\r\n read erro \r\n");
		free(buff);
		return;
	}
	ret = strlen(buff);
	if(ret <= 0){
		goto done;
	}
	//printf("\r\n parse data start\r\n");
	cb(&fd,buff,ret);
	//printf("\r\n parse data end\r\n");
done:
	free(buff);
}

void http_client_select(fd_set* readFd,TclientMsg* clientMsg,data_handle_cb cb)
{
	char ret = 0;
	int maxClient = -1;
	struct timeval tv;
	
	FD_ZERO(readFd);
	for(clientIndex = 0;clientIndex < clientMsg->clientFdIndex;clientIndex++){
		FD_CLR(clientMsg->clientFd[clientIndex],readFd);
	}
	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	for(clientIndex = 0;clientIndex < clientMsg->clientFdIndex;clientIndex++){
		FD_SET(clientMsg->clientFd[clientIndex],readFd);
		maxClient = clientMsg->clientFd[clientIndex] + 1;
		//printf("clientMessage.clientFd[clientIndex] %d",clientMsg->clientFd[clientIndex]);
	}
	if(maxClient > 0){
		ret = select(maxClient,readFd,NULL,NULL,&tv);
		if (ret > 0) {
			for(clientIndex = 0;clientIndex < clientMsg->clientFdIndex;clientIndex++){
				if(FD_ISSET(clientMsg->clientFd[clientIndex],readFd)){
					start_get_msg(clientMsg->clientFd[clientIndex],cb);	
				}
			}	
			//sys_msleep(100);
		} else if(ret == 0){
			//printf("time out \r\n");
			//DEBUG_TASK_OVERFLOW();
			return;
		}else{
			printf(" erro \r\n");
		}
	} else {
		sys_msleep(100);
	}	
}

void handle_client_msg()
{
	int ret = 0;
	fd_set readFd;
	unsigned char loop;
	//char maxClient;
	while(1){
		http_client_select(&readFd,&clientMessage,handle_data_cb);
#if 0
		for(loop = 0;loop < clientMessage.clientFdIndex;loop++){
			readFd = clientMessage.clientFd[loop];	
			start_get_msg(readFd,handle_data_cb);	
		}
		sys_msleep(100);
#endif	
	}

}

void dump_client_msg(struct sockaddr_in* msg)
{
	printf("client ip %s,client port %d",
			inet_ntoa(msg->sin_addr),
			ntohs(msg->sin_port)
		);	
}

int websrv_start_up(int* serverFd)
{
	static struct sockaddr_in sockAddr;
	*serverFd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(*serverFd < 0){
//		assert(LOG_TAG,"\r\n create socket fail \r\n");
	}
	memset(&sockAddr,0,sizeof(sockAddr));
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(WEBSRV_PORT);

	if(bind(*serverFd,(struct sockaddr* )&sockAddr,sizeof(struct sockaddr_in)) < 0){

//assert(LOG_TAG,"\r\n bind fail \r\n");
	}

	if(listen(*serverFd,100) < 0){
//		assert(LOG_TAG,"\r\n listen fial \r\n");
		;
	}
	return;
}

char server_select(int* serverFd)
{
	struct timeval tv;
	fd_set readFd;
	FD_ZERO(&readFd);
	FD_CLR(*serverFd,&readFd);
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	FD_SET(*serverFd,&readFd);

	if(select((*serverFd) + 1,&readFd,NULL,NULL,&tv) < 0){
		printf("\r\n select fail \r\n");
	}
	if(FD_ISSET(*serverFd,&readFd)){
		return 1;
	}else{
		//printf("server assert fail \r\n");
		return -1;
	}
}
void start_server_up()
{
	static int serverFd = -1,clientFd = -1;
	struct sockaddr_in clientAddr;
	socklen_t addrLen = sizeof(struct sockaddr_in);
	clientMessage.clientFdIndex = 0;
	memset(&clientAddr,0,sizeof(clientAddr));
	
	websrv_start_up(&serverFd);
	xTaskCreate(handle_client_msg,"hdlmsg",1024 * 4,NULL,5,NULL);

	printf("\r\n==================start server %d====================\r\n",serverFd);
	while(1){
		if(clientMessage.clientFdIndex == CLIENT_NUM_MAX){
			sys_msleep(2000);
			continue;
		}
		if(server_select(&serverFd) > 0){
			clientFd = accept(serverFd,(struct sockaddr* )&clientAddr,&addrLen);
			if(clientFd < 0){
				printf("\r\n accept fail \r\n");
			}else{
				clientMessage.clientFd[clientMessage.clientFdIndex++] = clientFd;
				printf("clientMessage.clientFd[clientMessage.clientFdIndex] %d",clientFd);
				dump_client_msg(&clientAddr);				
			}
		}
	}
}

