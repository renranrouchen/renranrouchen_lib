#include "http.h"
#include "string.h"
#include <lynx_debug.h>
#include "lwip/sockets.h"
#include <image.h>

#define SOCKET_RECBUFFER_LEN (1*600)
#define FL_LO 17
int g_DownloadSize = 0;


int32 Http_POST( int32 socketid, const int8 *host,const int8 *passcode,const int8 *mac,const int8 *product_key )
{
    int32 ret=0;
    uint8 *postBuf=NULL;
    int8 *url = "/dev/devices"; 
    int8 Content[100]={0};
    int32 ContentLen=0;
    int32 totalLen=0;
    
    postBuf = (uint8*)malloc(400);
    if (postBuf==NULL) return 1;
    //g_globalvar.http_sockettype =HTTP_GET_DID;//http_sockettype=1 :http_post type.

    sprintf(Content,"passcode=%s&mac=%s&product_key=%s",passcode,mac,product_key);
    ContentLen=strlen(Content);
    sprintf( (char *)postBuf,"%s %s %s%s%s %s%s%s %d%s%s%s%s%s",
              "POST" ,url,"HTTP/1.1",kCRLFNewLine,
              "Host:",host,kCRLFNewLine,
              "Content-Length:",ContentLen,kCRLFNewLine,
              "Content-Type: application/x-www-form-urlencoded",kCRLFNewLine,
              kCRLFNewLine,
              Content
        );
    totalLen = strlen( (char *)postBuf );
    DBG_PRINTF(INFO,"http_post:%s %d",postBuf,totalLen);    
    ret = send( socketid,postBuf,totalLen,0 );
    DBG_PRINTF(INFO,"http_post ret: %d",ret);    
    free( postBuf );
    return 0;
}

int32 Http_GET( const int8 *host,const int8 *did,int32 socketid )
{
    static int8 *getBuf=NULL;
    int32 totalLen=0;
    int32 ret=0;
    int8 *url = "/dev/devices/";

    getBuf = (int8*)malloc( 200 );
    if(getBuf == NULL)
    {
        return 1;
    }
    memset( getBuf,0,200 );
    //g_globalvar.http_sockettype =HTTP_PROVISION;//http get type.
    sprintf( getBuf,"%s %s%s %s%s%s %s%s%s%s%s",
              "GET",url,did,"HTTP/1.1",kCRLFNewLine,
              "Host:",host,kCRLFNewLine
              "Cache-Control: no-cache",kCRLFNewLine,
              "Content-Type: application/x-www-form-urlencoded",kCRLFLineEnding);
    totalLen =strlen( getBuf );
    ret = send( socketid, getBuf,totalLen,0 );
    DBG_PRINTF(INFO,"Sent provision:\n %s\n", getBuf);
    free(getBuf);
    getBuf = NULL;

    if(ret<=0 ) 
    {
        return 1;
    }
    else
    {
        return 0;
    }    
}
/******************************************************
 *
 *   FUNCTION        :   delete device did by http 
 *     
 *   return          :   0--send successful 
 *                       1--fail. 
 *   Add by Alex lin  --2014-12-16
 *
 ********************************************************/
int32 Http_Delete( int32 socketid, const int8 *host,const int8 *did,const int8 *passcode )
{
    int32 ret=0;
    int8 *sendBuf=NULL;
    int8 *url = "/dev/devices"; 
    int8 *Content = NULL;
    int32 ContentLen=0;
    int32 totalLen=0;
    int8 *DELETE=NULL;
    int8 *HOST=NULL;
    int8 Content_Length[20+1]={0};
    int8 *contentType="Content-Type: application/x-www-form-urlencoded\r\n\r\n";

    DELETE = (int8*)malloc(strlen("DELETE  HTTP/1.1\r\n")+strlen(url)+1);//+1 for sprintf
    if( DELETE ==NULL ) 
    {
        return 1;
    }
    HOST = (int8*)malloc(strlen("Host: \r\n")+strlen(host)+1);// +1 for sprintf
    if( HOST==NULL)
    {
      free(DELETE);
      return 1;
    }
    Content = (int8*)malloc(strlen("did=&passcode=")+strlen(did)+strlen(passcode)+1);// +1 for sprintf
    if( Content==NULL )
    {
      free(DELETE);
      free(HOST);
      return 1;      
    }
    memset( Content,0,strlen("did=&passcode=")+strlen(did)+strlen(passcode)+1 );
    sprintf(Content,"did=%s&passcode=%s",did,passcode);
    ContentLen=strlen(Content); 
    sprintf(DELETE,"DELETE %s HTTP/1.1\r\n",url);
    sprintf(HOST,"Host: %s\r\n",host);
    sprintf(Content_Length,"Content-Length: %d\r\n",ContentLen);
    sendBuf = (int8*)malloc(strlen(DELETE)+strlen(HOST)+strlen(Content_Length)+strlen(contentType)+ContentLen+1);//+1 for sprintf
    if (sendBuf==NULL) 
    {
      free(DELETE);
      free(HOST);
      free(Content);
      return 1;
    }
    sprintf(sendBuf,"%s%s%s%s%s",DELETE,HOST,Content_Length,contentType,Content);
    totalLen = strlen(sendBuf);
    ret = send( socketid, sendBuf,totalLen,0 );
    if(ret<=0)
    {
      DBG_PRINTF(ERROR," send fail %s %s %d",__FILE__,__FUNCTION__,__LINE__);
      return 1;
    }
    
    DBG_PRINTF( INFO , "totalLen = %d",totalLen);
    DBG_PRINTF(INFO,"%s",sendBuf);

    free(DELETE);
    free(HOST);
    free(Content);
    free(sendBuf);
    return 0;
}
/******************************************************
        functionname    :   Http_ReadSocket
        description     :   read data form socket
        socket          :   http server socket
        Http_recevieBuf :   data buf.
        bufLen          :   read data length 
        return          :   >0 the data length read form 
                            socket 
                            <0 error,and need to close 
                            the socket.
******************************************************/
int32 Http_ReadSocket( int32 socket,uint8 *Http_recevieBuf,int32 bufLen )
{
    int32 bytes_rcvd = 0; 
    if( socket < 0 || Http_recevieBuf == NULL)
    {
        return bytes_rcvd;
    }
    memset(Http_recevieBuf, 0, bufLen);  

    bytes_rcvd = recv(socket, Http_recevieBuf, bufLen, 0 );
    if(bytes_rcvd <= 0)
    {
        DBG_PRINTF(INFO,"Http_ReadSocket receive[%d].\n", bytes_rcvd);
        return -2;
    }
    return bytes_rcvd;
}
/******************************************************
 *
 *   Http_recevieBuf :   http receive data
 *   return          :   http response code
 *   Add by Alex lin  --2014-09-11
 *
 ********************************************************/
int32 Http_Response_Code( uint8 *Http_recevieBuf )
{
    int32 response_code=0;
    int8* p_start = NULL;
    int8* p_end =NULL; 
    int8 re_code[10] ={0};
    memset(re_code,0,sizeof(re_code));

    p_start = strstr( (char *)Http_recevieBuf," " );
    if(NULL == p_start)
    {
        return RET_FAILED;
    }
    p_end = strstr( ++p_start," " );
    if(p_end)
    {
        if(p_end - p_start > sizeof(re_code))
        {
            return RET_FAILED;
        }
        memcpy( re_code,p_start,(p_end-p_start) );
    }
    
    response_code = atoi(re_code); 
    return response_code;
}
int32 Http_Response_DID( uint8 *Http_recevieBuf,int8 *DID )
{
    int8 *p_start = NULL;
    memset(DID,0,DID_LEN);
    p_start = strstr( (char *)Http_recevieBuf,"did=");
    if( p_start==NULL )
        return 1;
    p_start = p_start+strlen("did=");
    memcpy(DID,p_start,DID_LEN);
    DID[DID_LEN - 2] ='\0';             
    return 0;    
}
int32 Http_getdomain_port( uint8 *Http_recevieBuf,int8 *domain,int32 *port )
{
    int8 *p_start = NULL;
    int8 *p_end =NULL;
    int8 Temp_port[10]={0};
    memset( domain,0,100 );
    p_start = strstr( (char *)Http_recevieBuf,"host=");
    if( p_start==NULL ) return 1;
    p_start = p_start+strlen("host=");
    p_end = strstr(p_start,"&");
    if( p_end==NULL )   return 1;
    memcpy( domain,p_start,( p_end-p_start ));
    domain[p_end-p_start] = '\0';
    p_start = strstr((++p_end),"port=");
    if( p_start==NULL ) return 1;
    p_start =p_start+strlen("port=");
    p_end = strstr( p_start,"&" ); 
    memcpy(Temp_port,p_start,( p_end-p_start));
    *port = atoi(Temp_port);
    return 0;
}
/******************************************************
 *   FUNCTION       :   sent the HTTP Get target data
 *                      to http server.
 *   host           :   http server host
 *   type           :   1-wifi ,2-mcu
 *   Add by Alex lin  --2014-10-29
 *
 ********************************************************/
int32 Http_GetTarget( const int8 *host, 
                      const int8 *product_key, 
                      const int8 *did,enum OTATYPE_T type,
                      const int8 *hard_version, 
                      const int8 *soft_version,
                      const int32 current_fid,int32 socketid )
{
    //int8 getBuf[500] = {0};
    int8 *getBuf=NULL;
    int32 totalLen=0;
    int32 ret=0;
    int8 *url = "/dev/ota/target_fid?";
    ;
    
    getBuf = (int8*)malloc(500);
    if(getBuf == NULL)
    {
        return 1;
    }
    
    
    memset( getBuf,0,500 );

    sprintf( getBuf,"%s %s%s%s%s%s%s%d%s%s%s%s%s%d%s%s%s%s%s%s%s",
             "GET",url,"did=",did,"&product_key=",product_key,
             "&type=",type,"&hard_version=",hard_version,
             "&soft_version=",soft_version,
             "&current_fid=",current_fid," HTTP/1.1",kCRLFNewLine,
             "Host: ",host,kCRLFNewLine,
             "Content-Type: application/text",kCRLFLineEnding );

    totalLen =strlen( getBuf );
    DBG_PRINTF(INFO,"totalLen=%d\r\n",totalLen);
    ret = send( socketid, getBuf,totalLen,0 );
    free(getBuf);
    if(ret>0) 
        return 0;

    return 1;
}

int32 CheckFirmwareUpgrade(const int8 *host, const int8 *did,enum OTATYPE_T type,
                                  const int8 *passcode,const int8 *hard_version, 
                                  const int8 *soft_version, int32 socketid )
{
    int8 *getBuf=NULL;
    int32 totalLen=0;
    int32 ret=0;
    int8 *url = "/dev/ota/v4.1/update_and_check";  
    int8 Content[100]={0};
    int32 ContentLen=0;
    getBuf = (int8*)malloc(500);
    if(getBuf == NULL)
    {
        return RET_FAILED;
    }
    sprintf(Content,"passcode=%s&type=%d&hard_version=%s&soft_version=%s",passcode,type,hard_version,soft_version);
    ContentLen=strlen(Content);
    memset( getBuf,0,500 );
    sprintf( getBuf,"%s %s%s%s %s%s%s%s%s%s%d%s%s%s%s%s",
             "POST",url,"/",did,"HTTP/1.1",kCRLFNewLine,
             "Host:",host,kCRLFNewLine,
             "Content-Length:",ContentLen,kCRLFNewLine,
             "Content-Type: application/x-www-form-urlencoded",kCRLFNewLine,
             kCRLFNewLine,
             Content
             );
    totalLen =strlen( getBuf );
    DBG_PRINTF(INFO,"Http_post_totalLen=%d\r\n",totalLen);
    ret = send( socketid, getBuf,totalLen,0 );
    DBG_PRINTF( INFO,"Req OTA Buf:\n%s",getBuf );
    free(getBuf);
    if(ret>0) 
        return RET_SUCCESS;

    return RET_FAILED;
}

/******************************************************
 *FUNCTION      :   get the http return softver 
 *                  and download url(for hf wifi)
 *  softver     :   softver
 *  download_url:   download_url 
 *          buf :   http receive data.
 *       return :  0-return ok, 1-return fail. 
 *   Add by Alex lin  --2014-10-29
 *
 ********************************************************/
int32 Http_GetSoftver_Url( int8 *download_url, int8 *softver, uint8 *buf )
{
    int8 *p_start = NULL;
    int8 *p_end =NULL;

    memset(softver, 0x0, 32);
    p_start = strstr( (char *)buf,"soft_ver=");
    if( p_start==NULL )  return 1;
    p_start =  p_start+(sizeof( "soft_ver=" )-1);
    p_end = strstr( p_start,"&");
    if( p_end==NULL )  return 1;
    memcpy( softver,p_start,(p_end-p_start));

    p_start = strstr(p_end,"download_url=");
    if(p_start==NULL ) return 1;
    p_start+=sizeof("download_url=")-1;
    p_end = strstr(p_start,kCRLFNewLine);
    if( p_end==NULL )  return 1;
    memcpy( download_url,p_start,(p_end-p_start));
    download_url[p_end-p_start] = '\0';
    
    return 0;
}

/******************************************************
 *
 *   FUNCTION       :   get the uuid by productkey in HTTP GET
 *      productkey  :   productkey
 *      host        :   host
 *      return      :   0-OK 1-fail
 *   Add by Alex lin  --2014-11-10
 *
 ********************************************************/
int32 Http_JD_Get_uuid_req( const int8 *host,const int8 *product_key )
{
    int8 *getBuf=NULL;
    int32 totalLen=0;
    int32 ret=0;
    int8 *url = "/dev/jd/product/";    
    if( strlen(product_key)<=0 )
    {   
        DBG_PRINTF(ERROR,"need a productkey to get uuid! ");
        return 1;
    }
    getBuf = (int8*)malloc(500);
    if (getBuf==NULL)  return 1;
    memset( getBuf,0,500 ); 
    //g_globalvar.http_sockettype =HTTP_GET_JD_UUID;//http get product_uuid for JD.

    sprintf( getBuf,"%s %s%s%s%s%s%s%s%s%s",
             "GET",url,product_key," HTTP/1.1",kCRLFNewLine,
             "Host: ",host,kCRLFNewLine,
             "Cache-Control: no-cache",kCRLFLineEnding );

    totalLen =strlen( getBuf );
    DBG_PRINTF(INFO,"totalLen=%d\r\n",totalLen);
    DBG_PRINTF(INFO,"Sent Http to get JD uuid.\n");
    DBG_PRINTF(INFO,"%s",getBuf );
    free(getBuf);
   
    if(ret>0) return 0; 
    else    return 1;
}

/******************************************************
 *
 *   FUNCTION           :       post the feedid and accesskey to http server
 *      feedid          :       feedid
 *      accesskey       :       accesskey
 *      return          :
 *   Add by Alex lin  --2014-11-07
 *
 ********************************************************/
int32 Http_JD_Post_Feed_Key_req( int32 socketid,int8 *feed_id,int8 *access_key,int8 *DId,int8 *host )
{
    int32 ret=0;
    uint8 *postBuf=NULL;
    int8 *url = "/dev/jd/"; 
    int8 Content[200]={0};
    int32 ContentLen=0;
    int32 totalLen=0;
    
    if(strlen(DId)<=0)
        return 1;

    postBuf = (uint8*)malloc(400);
    if (postBuf==NULL)
        return 1;
    sprintf(Content,"feed_id=%s&access_key=%s",feed_id,access_key);
    ContentLen = strlen( Content );

    sprintf( (char *)postBuf,"%s%s%s%s%s%s%s%s%s%s%s%s%s%d%s%s%s",
              "POST ",url,DId," HTTP/1.1",kCRLFNewLine,
              "Host: ",host,kCRLFNewLine,
              "Cache-Control: no-cache",kCRLFNewLine,
              "Content-Type: application/x-www-form-urlencoded",kCRLFNewLine,
              "Content-Length: ",ContentLen,kCRLFLineEnding,
              Content,kCRLFLineEnding                           
        );
    totalLen = strlen( (char *)postBuf );
    ret = send( socketid,postBuf,totalLen,0 );
    
    free( postBuf );    
    return 0;       
}
///*********************************************************************
// *
// *   FUNCTION           :    GAgent OTA by url OTA success will reboot
// *   download_url        :    download url 
// *   return             :     1-OTA fail.
// *   Add by Alex lin  --2014-12-01
// *
// **********************************************************************/
//int32 GAgent_DoOTAbyUrl( const int8 *download_url )
//{
//    int8 *GetBuf=NULL;
//    int8 *p_start = NULL;
//    int8 *p_end =NULL;
//    int32 GetBufLen=0,ret=0;
//    int8 url[100]={0};
//    int8 GET[100]={0};
//    int8 HOST[100]={0};
//    int8 *Type="Content-Type: application/text\r\n";
//    int8 *Control="Cache-Control: no-cache\r\n\r\n";

//    GetBuf = (int8*)malloc(500);
//    if( GetBuf==NULL)
//    {
//        GAgent_Printf(GAGENT_INFO," malloc fail %s %s %d",__FILE__,__FUNCTION__,__LINE__);
//        AS;
//        return 1;
//    }
//    memset(GetBuf,0,500);

//    p_start = strstr( download_url,"/dev/ota/");
//    if( p_start==NULL ) return 1;
//    p_end = strstr(download_url,".bin");
//    if( p_end==NULL )       return 1;
//    p_end=p_end+strlen(".bin");
//    memcpy( url,p_start,(p_end-p_start));
//    url[ (p_end-p_start)] ='\0';

//    sprintf(GET,"GET %s HTTP/1.1%s",url,kCRLFNewLine);
//    sprintf(HOST,"Host: %s%s",HTTP_SERVER,kCRLFNewLine);
//    sprintf( GetBuf,"%s%s%s%s",GET,HOST,Type,Control);

//    GetBufLen = strlen(GetBuf);

//    g_globalvar.waninfo.send2HttpLastTime = DRV_GAgent_GetTime_S();
//    g_globalvar.connect2Cloud=0; 
//    ret = Http_InitSocket(1);
//    ret = send( g_globalvar.waninfo.http_socketid, GetBuf,GetBufLen,0 );
//    g_globalvar.http_sockettype=HTTP_OTA;

//    free(GetBuf);
//}

/*********************************************************************
*
*   FUNCTION       :   TO get the http headlen
*     httpbuf      :   http receive buf 
*     return       :   the http headlen.
*   Add by Alex lin  --2014-12-02
*
**********************************************************************/
int32 Http_HeadLen( uint8 *httpbuf )
{
   int8 *p_start = NULL;
   int8 *p_end =NULL;
   int32 headlen=0;
   p_start = (char *)httpbuf;
   p_end = strstr( (char *)httpbuf,kCRLFLineEnding);
   if( p_end==NULL )
   {
       DBG_PRINTF(INFO,"Can't not find the http head!");
       return 0;
   }
   p_end=p_end+strlen(kCRLFLineEnding);
   headlen = (p_end-p_start);
   return headlen;
}
/*********************************************************************
*
*   FUNCTION       :   TO get the http bodylen
*      httpbuf     :   http receive buf 
*      return      :   the http bodylen.(0-error)
*   Add by Alex lin  --2014-12-02
*
**********************************************************************/
int32 Http_BodyLen( uint8 *httpbuf )
{
	int8 *p_start = NULL;
	int8 *p_end =NULL;
	int8 bodyLenbuf[10]={0};
	int32 bodylen=0;  //Content-Length: 
	p_start = strstr( (char *)httpbuf,"Content-Range: ");
	if( p_start==NULL )
	{
		p_start = strstr( (char *)httpbuf,"Content-Length: ");
		if( p_start==NULL ) return 0;
		p_start = p_start+strlen("Content-Length: ");
		p_end = strstr( p_start,kCRLFNewLine);
		if( p_end==NULL )	return 0;
	}
	else
	{
		 p_start = strstr( (char *)p_start,"/");
		 if( p_start==NULL ) return 0;
		 p_start = p_start+strlen("/");
		 p_end = strstr( p_start,kCRLFNewLine);
		 if( p_end==NULL )	 return 0;
	}

	memcpy( bodyLenbuf,p_start,(p_end-p_start));
	bodylen = atoi(bodyLenbuf);
	return bodylen;
}
int32 Http_GetHost( int8 *downloadurl,int8 **host,int8 **url )
{
   int8 *p_start = NULL;
   int8 *p_end =NULL; 
   int8 hostlen;

   p_start = strstr( downloadurl,"http://" );
   if(p_start==NULL) 
       return RET_FAILED;
   p_start = p_start + strlen("http://");
   p_end = strstr( p_start,"/");
   if(p_end==NULL)
       return RET_FAILED;
   *host = (char *)malloc(p_end-p_start+1);
   if( NULL == *host )
   {
       DBG_PRINTF(ERROR, "OTA host malloc fail!");
       return RET_FAILED; 
   }
   memcpy(&(*host)[0],p_start,p_end-p_start);
   (*host)[p_end-p_start] = '\0';
   hostlen = p_end-p_start;
   
   p_start = p_end;
   if(p_start==NULL) 
       return RET_FAILED;
   p_end = p_start + strlen(downloadurl) - hostlen;
   if(p_end==NULL)
       return RET_FAILED;
   *url = (char *)malloc(p_end-p_start+1);
   if( NULL == *url )
   {
       DBG_PRINTF(ERROR, "OTA url malloc fail!");
       return RET_FAILED;
   }
   memcpy(&(*url)[0],p_start,p_end-p_start);
   (*url)[p_end-p_start] = '\0';
  
   return RET_SUCCESS;
}

int32 Http_GetSV( uint8 *httpbuf,int8 *SV )
{
//Soft-Version: 
   int8 *p_start = NULL;
   int8 *p_end =NULL;
   int32 SV_Len=0;
   p_start = strstr( (char *)httpbuf,"Soft-Version: " );
   if(p_start==NULL) 
       return 0;
   p_start = p_start+strlen("Soft-Version: " );
   p_end = strstr( p_start,kCRLFNewLine);
   if(p_end==NULL)
       return 0;
   SV_Len = (p_end-p_start);
   memcpy(SV,p_start,SV_Len);
   SV[SV_Len]='\0';
   return SV_Len;

}
/*************************************************************
*
*   FUNCTION  :  get MD5 from http head.
*   httpbuf   :  http buf.
*   MD5       :  MD5 form http head(16b).
*           add by alex.lin ---2014-12-04
*************************************************************/
int32 Http_GetMD5( uint8 *httpbuf,uint8 *MD5,int8 *strMD5)
{
   int8 *p_start = NULL;
   int8 *p_end =NULL;
   int8 MD5_TMP[16];
   uint8 Temp[3]={0};
   int8 *str;
   int32 i=0,j=0;
   p_start = strstr( (char *)httpbuf,"Firmware-MD5: ");
   if(p_start==NULL)
       return 1;
   p_start = p_start+strlen("Firmware-MD5: ");
   p_end = strstr( p_start,kCRLFNewLine);
   if(p_end==NULL)
       return 1;
   if((p_end-p_start)!=32) return 1;
   memcpy( strMD5,p_start,32 );
   strMD5[32] = '\0';
   for(i=0;i<32;i=i+2)
   {
       Temp[0] = strMD5[i];
       Temp[1] = strMD5[i+1];
       Temp[2] = '\0';
       MD5_TMP[j]= strtol(Temp, &str,16);  
       j++;
   }
   memcpy(MD5,MD5_TMP,16);
   DBG_PRINTF(INFO," MD5 From HTTP:\n");
   
   for(j=0;j<16;j++)
       DBG_PRINTF(INFO,"%02x",MD5[j]);
   
   return 16;
}
int32 Http_GetFileType(uint8 *httpbuf)
{
    int8 *p_start = NULL;
    int8 *p_end =NULL;
    int8 fileExt_Len=0;
    int8 filename_Len = 0;
    int8 fileExt[4];
    int8 filename[100] = {0};
    p_start = strstr( (char *)httpbuf,"filename=" );
    if(p_start==NULL) 
        return 0;
    p_start = p_start+strlen("filename=" );
    p_end = strstr( p_start,kCRLFNewLine);
    if(p_end==NULL)
        return 0;
    filename_Len = (p_end-p_start);
    memcpy(filename,p_start,filename_Len);
    filename[filename_Len]='\0';
    
    p_start = strstr( (char *)filename,"." );
    if(p_start==NULL) 
        return 0;
    p_start = p_start+strlen(".");
    p_end = strstr( p_start,"\"");
    if(p_end==NULL)
        return 0;
  
    fileExt_Len = (p_end-p_start);
    memcpy(fileExt,p_start,fileExt_Len);
    fileExt[fileExt_Len]='\0';
    if( 0 == strcmp(fileExt,"hex") )
    {
        DBG_PRINTF(INFO,"download firmware type is hex.");
        return MCU_FIRMWARE_TYPE_HEX;
    }
    else
    {
        DBG_PRINTF(INFO,"download firmware type is bin.");
        return MCU_FIRMWARE_TYPE_BIN;
    }
}
char *itoaw(int num, char *str, int radix)	  
{	
	if(num==0)	 
	{  
		str[0]='0';str[1]='\0';  
		return str;  
	}  
	char  string[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";   
	char* ptr = str;   
	int i; int j;	
	int value=num;	
	if(num<0) num=-num;  
	while (num>=radix)	 
	{	
		*ptr++	= string[num % radix];	 
		num /= radix;	
	}	
	if (num)   
		{	
			*ptr++ = string[num];	
			*ptr  = '\0';	
		}	
	int n=j= ptr-str-1;   
	for (i=0; i <(ptr - str) / 2; i++)	 
	{	
		int temp = str[i]; str[i]  = str[j]; str[j--] = temp;	
	}	
	if(value<0)   
	{  
		for(j=n;j>=0;--j) str[j+1]=str[j];	
		str[0]='-';  
	}  
	str[n+2]='\0';	
	return str;   
}  

int32 Http_ReqGetFirmware( int8 *url,int8 *host,int32 socketid)
{
    static int8 *getBuf = NULL;
    int32 totalLen=0;
    int32 ret=0;
    getBuf = (int8*)malloc( 200 );
	char range[24];
	char sizeStr[24];
	
    if(getBuf == NULL)
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
        return RET_FAILED;
    }
    memset( getBuf,0,200 );
	itoaw(g_DownloadSize,sizeStr,10);
	sprintf(range,"%s%s",sizeStr,"-");
    sprintf( getBuf,"%s %s %s%s%s %s%s%s%s%s%s%s",
              "GET",url,"HTTP/1.1",kCRLFNewLine,
              "Host:",host,kCRLFNewLine,
              "Content-Type: application/text",kCRLFNewLine,
              "Range: bytes=",range,kCRLFLineEnding);


    totalLen =strlen( getBuf );
    ret = send( socketid, getBuf,totalLen,0 );
    DBG_PRINTF( INFO,"Req OTA len:%d send ret %d:\n%s",totalLen,ret ,getBuf );
    free(getBuf); 
    getBuf = NULL;

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
    if(ret<=0 ) 
    {
        return RET_FAILED;
    }
    else
    {
        return RET_SUCCESS;
    }    
}

int erase_OTA_flash(void)
{
	unsigned int app_h, ota_h;
	unsigned int app_e, ota_e, src = 0;
	char magic[3] = IH_MAGIC;

	flash_read(FLASH_APP_STARTADDR, (unsigned int)&app_h, 4);
	flash_read(FLASH_OTA_STARTADDR, (unsigned int)&ota_h, 4);

	if((!memcmp(&app_h, &magic[0], 3)) && (!memcmp(&ota_h, &magic[0], 3)))
	{
		flash_read((FLASH_APP_STARTADDR + FLASH_OTA_LENGTH - 4), (unsigned int)&app_e, 4);
		flash_read((FLASH_OTA_STARTADDR + FLASH_OTA_LENGTH - 4), (unsigned int)&ota_e, 4);

		//if(app_e == 0xffffffff)
			src = FLASH_APP_STARTADDR;
		//else if(ota_e == 0xffffffff)
		//	src = FLASH_OTA_STARTADDR;
	}
	else if(!memcmp(&app_h, &magic[0], 3))
	{
		src = FLASH_OTA_STARTADDR;
		//serial_printf("OTA flash write\nsrc 0x%06x %08x\n", src, ota_h);
	}
	else if(!memcmp(&ota_h, &magic[0], 3))
	{
		src = FLASH_APP_STARTADDR;
		//serial_printf("OTA flash write\nsrc 0x%06x %08x\n", src, app_h);
	}

	if(!src)
		DBG_PRINTF(ERROR, "OTA src IH magic err:%08x %08x\n", app_h, ota_h);

	if(g_DownloadSize == 0 && src != 0)
		flash_erase(src, FLASH_OTA_LENGTH);

	return src;
}

uint32 Http_ResGetFirmware(int32 socketid )
{
    int ret;
    uint8 *httpReceiveBuf = NULL;
    int headlen = 0;
    int writelen = 0;
	int32 filelen = 0;
	int k = 0;
	int receiveLength = 0;
	char magic[3] = IH_MAGIC;
	struct img_head imgHead;
	unsigned int src, count;

    httpReceiveBuf = (uint8 *)malloc(SOCKET_RECBUFFER_LEN);
    if(httpReceiveBuf == NULL)
    {
        DBG_PRINTF(ERROR, "%s malloc fail!\n", __FUNCTION__);
        return RET_FAILED;
    }
	int timeout=30000;//30s timeout
	setsockopt(socketid,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(int));

    receiveLength = Http_ReadSocket( socketid, httpReceiveBuf, SOCKET_RECBUFFER_LEN );  
    if(receiveLength <=0 ) 
    { 
    	DBG_PRINTF(ERROR,"Http_ResGetFirmware Http_ReadSocket ret:%d\n",ret);
		goto fail;
    }
	serial_printf("\nHttp_ResGetFirmware:%s\n",httpReceiveBuf);
    ret = Http_Response_Code( httpReceiveBuf );
    if(200 != ret&&206 != ret)
    {
    	DBG_PRINTF(ERROR,"Http_ResGetFirmware Http_Response_Code ret:%d\n",ret);
		goto fail;
    }
    headlen = Http_HeadLen( httpReceiveBuf );
    filelen = Http_BodyLen( httpReceiveBuf );
	DBG_PRINTF(INFO,"Http_ResGetFirmware  filelen:%d\n",filelen);
	if(filelen > FLASH_OTA_LENGTH)
	{
		DBG_PRINTF(INFO,"%s, the ota file is too large!\n", __FUNCTION__);
		goto fail;
	}

	if(!(src = erase_OTA_flash()))
		goto end;

	if(receiveLength - headlen > 0)
	{
		ret = sflash_write(src, (unsigned int)httpReceiveBuf+headlen, receiveLength - headlen );
		if(ret < 0)
		{
			goto fail;
		}
		g_DownloadSize = receiveLength - headlen;
	}
    do
    {
        receiveLength = Http_ReadSocket( socketid, httpReceiveBuf, SOCKET_RECBUFFER_LEN );    
        if(receiveLength <= 0 )
        {
            DBG_PRINTF(INFO,"%s, socket recv ota file timeout!\n", __FUNCTION__);
			goto fail;
        }

		ret = sflash_write(src+g_DownloadSize, (unsigned int)httpReceiveBuf, receiveLength);
		if(ret < 0)
		{
			DBG_PRINTF(INFO, "%s OTA upgrade fail at off:0x%x", __FUNCTION__, g_DownloadSize);
			goto fail;
		}

		g_DownloadSize += receiveLength;
		DBG_PRINTF(INFO,"socket recv ota file offset:%d\n",g_DownloadSize);
		if(g_DownloadSize >= filelen)
		{
			DBG_PRINTF(INFO,"\n%d,%s, socket recv ota file completed!\n",__LINE__, __FUNCTION__);
			break;
		}
	}while(1);
	ret = flash_read(src, (unsigned int)&imgHead, sizeof(struct img_head));//read the image head
end:
	if (memcmp(&imgHead.magic[0], &magic[0], 3))
	{
		DBG_PRINTF(ERROR, "IH magic err:%08x\n", *((int*)&imgHead.magic[0]));
		g_DownloadSize = 0;
		goto fail;
	}

	if(src == FLASH_APP_STARTADDR)
		flash_read((unsigned int)FLASH_OTA_STARTADDR + FLASH_OTA_LENGTH - 4, (unsigned int)&count, 4);
	else
		flash_read((unsigned int)FLASH_APP_STARTADDR + FLASH_OTA_LENGTH - 4, (unsigned int)&count, 4);
	count += 1;
	flash_write(src + FLASH_OTA_LENGTH - 4, (unsigned int)&count, 4);
	//serial_printf("\n>>> Version count %08x <<<\n", count);

	free(httpReceiveBuf);
	DBG_PRINTF(INFO,"Http_ResGetFirmware success\n");
	return RET_SUCCESS;
fail:
	free(httpReceiveBuf);
	return RET_FAILED;
}

/****************************************************************
*       FunctionName     :      Http_Get3rdCloudInfo
*       Description      :      get 3rd cloud name and info form 
*                               buf.
*       buf              :      data afer provision.
*       szCloud3Name     :      3rd cloud name.
*       szCloudInfo      :      3rd cloud info.
*       return           :      1 need 3rd cloud
*                               0 only need gizwits
*       Add by Alex.lin   --2015-03-03
****************************************************************/
uint8 Http_Get3rdCloudInfo( int8 *szCloud3Name,int8 *szCloud3Info,uint8 *buf )
{
    int8 *p_start = NULL;
    int8 *p_end =NULL;
    int8 *cloudName = "3rd_cloud=";
    int8 *uuid = "product_uuid=";
    
    memset( szCloud3Name,0,10 );
    memset( szCloud3Info,0,32 );
    
    p_start = strstr( (char *)buf,cloudName );
    if( p_start==NULL )
        return 0;
    p_start+=strlen( cloudName );
    p_end = strstr( p_start,"&");
    if( p_end==NULL )
        return 0;
    memcpy( szCloud3Name,p_start,(p_end-p_start) );
    szCloud3Name[p_end-p_start]='\0';
    
    p_start = strstr( p_start,uuid );
    if( p_start==NULL )
        return 0;
    p_start +=strlen( uuid );
    p_end = strstr( p_start,kCRLFNewLine );
    if( p_end==NULL )
        return 0;
    memcpy( szCloud3Info,p_start,(p_end-p_start));
    szCloud3Info[p_end-p_start]='\0';
    return 1;
}

#define MCU_OTA_URL "/dev/ota/v4.1/latest"
int32 http_querymcuota(int32 socketid, int8 *buf, int8 *host, int8 *pk, int8 *did, int8 *hardver, int8 *softver, int8 type)
{
    int32 ret=0;
    uint8 *postBuf=NULL;
    int8 *url = MCU_OTA_URL;
    int8 Content[200]={0};
    int32 ContentLen=0;
    int32 totalLen=0;

    sprintf(Content,"?hard_version=%s&soft_version=%s&product_key=%s&type=%d",
            hardver, softver, pk, type);
    ContentLen = strlen( Content );

    sprintf( (char *)buf,"%s%s%s%s%s%s%s%s%s%s%s%s",
              "GET ",url, Content, " HTTP/1.1", kCRLFNewLine,
              "Host: ", host, kCRLFNewLine,
              "Cache-Control: no-cache", kCRLFNewLine,
              "Content-Type: application/x-www-form-urlencoded",kCRLFLineEnding
        );
    totalLen = strlen( (char *)buf );

    ret = send( socketid,buf,totalLen,0 );

    return 0;
}
