#include "lwip/sockets.h"
#include "http.h"
#include "string.h"
#include <lynx_debug.h>
#include <net_api.h>

#if 0
int32 server_connect( int32 iSocketId, uint16 port,
                        int8 *ServerIpAddr,int8 flag)
{
	if(ServerIpAddr == NULL)
	{
		DBG_PRINTF(ERROR," GAgent_connect error\n");
		return RET_FAILED;
	}

    int8 ret=0;
    
    struct sockaddr_in Msocket_address;
    DBG_PRINTF(INFO,"do connect ip:%s port=%d\n",ServerIpAddr,port );

    Msocket_address.sin_family = AF_INET;
    Msocket_address.sin_port= htons(port);
    Msocket_address.sin_addr.s_addr = inet_addr(ServerIpAddr);

	ret = connect(iSocketId,(struct sockaddr *)&Msocket_address, sizeof( struct sockaddr_in)); 

	DBG_PRINTF(INFO,"server_connect ret:%d\n",ret );
    if( ret==0)
    {
        return ret;
    }
    else 
    {
    	return  -1;
    }

}

int32 initSocket( int8 *p_szServerIPAddr,int32 port,int8 flag )
{
    int32 ret=0;
    uint8 reconnectTime=0;
	int32 iSocketId;
	int32 value = 0;
    ret = strlen( p_szServerIPAddr );
    
    if( ret<=0 || ret> 17 )
    {
        return RET_FAILED;
    }

    DBG_PRINTF(INFO,"socket connect ip:%s\n",p_szServerIPAddr);

    if( (iSocketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        DBG_PRINTF(ERROR," socket init fail:%d\n",iSocketId);
        return RET_FAILED;
    }

    DBG_PRINTF(INFO, "New socketID [%d]\n",iSocketId);
    while( reconnectTime<3 )
    {
        value = server_connect( iSocketId, port, p_szServerIPAddr,flag );
        if( value>=0 )
        {
            break;
        }
        DBG_PRINTF( INFO,"connect fail , do it again !!!\n");
        reconnectTime++;
        sys_msleep(100);
    }
    if ( value <0 )
    {
        iSocketId=-1;
        DBG_PRINTF(ERROR, "socket connect fail:%d\n", value);
        return -3;
    }
    return iSocketId;
}
int8 isValidIP(char *ip)
{
    if(ip==NULL)
    {
        return 0;
    }
    char temp[4];
    int count=0;
    while(true)
	{
        int index=0;
        while(*ip!='\0' && *ip!='.' && count<4)
		{
            temp[index++]=*ip;
            ip++;
        }
        if(index==4)
        {
            return 0;
        }
        temp[index]='\0';
        int num=atoi(temp);
        if(!(num>=0 && num<=255))
        {
            return 0;
        }
        count++;
        if(*ip=='\0')
		{
            if(count==4)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
		else
		{
            ip++;
		}
    }
}

void ota_finished(int result)
{
    switch(result) {
    case RET_SUCCESS:
        DBG_PRINTF(INFO,"OTA SUCCESS. Rebooting...\r\n");
        reboot(1);//reboot after 1 sec
        break;
    case RET_FAILED:
        DBG_PRINTF(INFO,"OTA FAIL.\r\n");
        break;
    default:
        break;
    }
}

int32 Firmware_WIFIOTAByUrl(int8 *szdownloadUrl,int port )
{

    int32 ret = 0;
    int32 http_socketid = -1;
    int8 OTA_IP[16]={0};
    int8 *url = NULL;
    int8 *host = NULL;   

    if(RET_FAILED == Http_GetHost( szdownloadUrl, &host, &url ) )
    {
    	DBG_PRINTF(ERROR,"Http_GetHost failed!\n");
        return RET_FAILED;
    }
	DBG_PRINTF(INFO,"host:%s, url:%s\n",host,url);
	if(isValidIP(host) == false)
	{
		net_get_hostbyname((const char*)host,OTA_IP);
	}
	else
	{
		memcpy(OTA_IP, host,16);
	}
	
    DBG_PRINTF(INFO,"OTA_IP: %s\n",OTA_IP);

    http_socketid = initSocket(OTA_IP, port, 0 );
	if(http_socketid < 0)
	{
	    if(NULL!=host)
	    {
	       free(host);
	       host = NULL;
	    }
	    if(NULL!=url)
	    {
	       free(url);
	       url = NULL;
	    }	
		ota_finished(RET_FAILED);
		closesocket(http_socketid);
		DBG_PRINTF(ERROR,"initSocket error: %d\n",http_socketid);
        return RET_FAILED;
	}


    ret = Http_ReqGetFirmware( url, host, http_socketid );
    if( RET_SUCCESS == ret )
    {
        ret = Http_ResGetFirmware(http_socketid );
		closesocket(http_socketid);
		if(ret == RET_SUCCESS)
		{
			ota_finished(RET_SUCCESS);
		}
		else
		{
			DBG_PRINTF(ERROR,"req get Firmware failed!\n");
			ota_finished(RET_FAILED);
		}
        
        return ret;
    }
    else
    {
        DBG_PRINTF(ERROR,"req get Firmware failed!\n");
        closesocket(http_socketid);
		ota_finished(RET_FAILED);
        return RET_FAILED;
    }

}
#endif
