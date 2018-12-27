#ifndef WEBSERVER_H
#define WEBSERVER_H

enum{
	VALID_URI = 0X00,
	FAVICON_REQUEST = 0x01,
	NOT_FOUND = 0x02,
	GET_DATA = 0x03,
};

enum {
	IS_GET = 0X01,
	IS_POST = 0X02,	
};
#define SEND_MSG	"<!DOCTYPE html>\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\
<meta content=\"width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no\" name=\"viewport\">\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
<html>\
<body>\
<p>      This is wifi clock control copyright fdhisi .co     </p>\
<form action=\"/ \" method=\"POST\">\
<br>AP name:</br>\
<input type=\"text\" name=\"SSID\">\
<br>\
password:<br>\
<input type=\"text\" name=\"PSWD\">\
<br>timezone</br>\
<select name = \"timezone\">\
<option value=\"1\">UTC+1</option>\
<option value=\"2\">UTC+2</option>\
<option value=\"3\">UTC+3</option>\
<option value=\"4\">UTC+4</option>\
<option value=\"5\">UTC+5</option>\
<option value=\"6\">UTC+6</option>\
<option value=\"7\">UTC+7</option>\
<option value=\"8\">UTC+8</option>\
<option value=\"9\">UTC+9</option>\
<option value=\"10\">UTC+10</option>\
<option value=\"11\">UTC+11</option>\
<option value=\"12\">UTC+12</option>\
<option value=\"-1\">UTC-1</option>\
<option value=\"-2\">UTC-2</option>\
<option value=\"-3\">UTC-3</option>\
<option value=\"-4\">UTC-4</option>\
<option value=\"-5\">UTC-5</option>\
<option value=\"-6\">UTC-6</option>\
<option value=\"-7\">UTC-7</option>\
<option value=\"-8\">UTC-8</option>\
<option value=\"-9\">UTC-9</option>\
<option value=\"-10\">UTC-10</option>\
<option value=\"-11\">UTC-11</option>\
<option value=\"-12\">UTC-12</option>\
</select>\
<br>weather update interval</br>\
<select name=\"wthupdate\">\
<option value=\"0\">off</option>\
<option value=\"3\">3 hour</option>\
<option value=\"6\">6 hour</option>\
<option value=\"9\">9 hour</option>\
<option value=\"12\">12 hour</option>\
<option value=\"15\">15 hour</option>\
<option value=\"18\">18 hour</option>\
<option value=\"21\">21 hour</option>\
<option value=\"24\">24 hour</option>\
</select>\
<br>wifitime update interval</br>\
<select name=\"wifitime\">\
<option value=\"0\">off</option>\
<option value=\"1\">1 hour</option>\
<option value=\"3\">3 hour</option>\
<option value=\"6\">6 hour</option>\
<option value=\"9\">9 hour</option>\
<option value=\"12\">12 hour</option>\
<option value=\"15\">15 hour</option>\
<option value=\"18\">18 hour</option>\
<option value=\"21\">21 hour</option>\
<option value=\"24\">24 hour</option>\
</select>\
<br>nowtime alarm</br>\
<input type=\"radio\" name=\"nowtime\" value=\"1\" checked>on\
<br>\
<input type=\"radio\" name=\"nowtime\" value=\"0\">off\
monday:<input type=\"checkbox\" name=monday value=\"1\">\
<br><br>\
<input type=\"submit\" value=\"submit\">\
</form>\
</body>\
</html>"

#define MODIFY_OK_RESPONSE	"OK"

#define RESPONSE_OK	"HTTP/1.1 200 OK\r\n\
Server: ESP-32/1.0\r\n\
Content-Type: text/html\r\n\
Content-Length: %d\r\n\
Connection: keep-alive\r\n\
\r\n"
//%s"	
#define RESPONSE_NOT_FOUND "HTTP/1.1 404 Not Found\r\n"

#define WEBSRV_PORT	80
#define CLIENT_NUM_MAX	255
//#define SSID "fjjxxy"
//#define PASSWD "wtlwzwhxq"
#define CRLF_ENDLINE	"\r\n\r\n"

typedef void (*data_handle_cb)(int* operateClient,char* buff,int len);
typedef struct clientMsg{
	int clientFd[255];
	int clientFdIndex;
}TclientMsg;

extern TclientMsg clientMessage;
extern void start_server_up();
//void http_client_select(fd_set* readFd,TclientMsg* clientMsg,data_handle_cb cb);

#endif

