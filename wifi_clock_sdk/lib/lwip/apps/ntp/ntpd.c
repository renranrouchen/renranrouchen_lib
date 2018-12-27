/*
 *	Copyright Montage.Inc 2017
 *	ntpclient to update time.
 */


#include <stdio.h>
#include <string.h>
#include <lwip/opt.h>
#include <lwip/sockets.h>
#include "lwip/netdb.h"
#include <time.h>
#include <cfg_api.h>
#include "ntpd.h"
//#include <wifiClock_driver/FD612Drv.h>

/*
	Todo: Add multiple server support.

	//good:
	time.asia.apple.com
	clock.via.net
	us.ntp.org.cn
	kr.ntp.org.cn

	//not good:
	de.ntp.org.cn 
	
	
*/
#define printf(fmt,args...)	serial_printf (fmt ,##args)
extern unsigned int utctime_g;

static time_process_callback wifiTimeCB;


int socket_data_available(int sock_fd,int timeOut) {
	fd_set socketReadSet;
	FD_ZERO(&socketReadSet);
	FD_SET(sock_fd,&socketReadSet);
	struct timeval tv;

	tv.tv_sec  = timeOut / 1000;
	tv.tv_usec = (timeOut % 1000) * 1000;

	if (select(sock_fd+1,&socketReadSet,0,0,&tv) == SOCKET_ERROR) {
		return 0;
	}
	return FD_ISSET(sock_fd,&socketReadSet) != 0;
}


static int send_ntp_packet(int socket_fd, struct sockaddr_in server_addr)
{
	//send 48 byte data
	unsigned int data[12] = {0};
	int flags = 0;
	int rc = -1;

#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4
#define PREC -6

	data[0] = htonl (
		( LI << 30 ) | ( VN << 27 ) | ( MODE << 24 ) |
		( STRATUM << 16) | ( POLL << 8 ) | ( PREC & 0xff ) );
	data[1] = htonl(1<<16);  /* Root Delay (seconds) */
	data[2] = htonl(1<<16);  /* Root Dispersion (seconds) */
	data[10] = htonl(JAN_1970); /* Transmit Timestamp coarse */
	data[11] = htonl(NTPFRAC(0)); /* Transmit Timestamp fine   */
	
	rc = lwip_sendto(socket_fd, data, sizeof(data), flags, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(rc < 0) {
		printf("cannot send data, rc = %d\n", rc);
		return -1;
	}
	
	return 0;

}



static int init_socket(char *server_name, int *out_sock_fd, struct sockaddr_in *out_server_addr)
{
	
	struct sockaddr_in local_addr;
	struct sockaddr_in server_addr;
	int sock_fd = -1;
	int rc = -1;
	struct hostent *hp;
	int ip;
	char *addr;

	if (NULL == (hp = lwip_gethostbyname(server_name)))
	{
		printf("can't resolute the host address \n");
		return -1;
	}
	ip = *(unsigned int *)(hp->h_addr);
	addr = inet_ntoa(*(unsigned int *)hp->h_addr);
	serial_printf("\nServer name %s  address: %s\n", server_name, addr);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = ip;
	server_addr.sin_port = htons(NTP_SERVER_PORT);

	/* socket creation */
	sock_fd = lwip_socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0) {
		printf("cannot open socket \n");
		return -1;
	}

	/* bind any port */
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(0);

	rc = lwip_bind(sock_fd, (struct sockaddr *) &local_addr, sizeof(local_addr));
	if(rc < 0) {
		printf("cannot bind port\n");
		return -1;
	}
	
	*out_sock_fd = sock_fd;
	*out_server_addr = server_addr;
	return 0;
}
void set_system_time(long* timeVal)
{
	unsigned char timeZone;
	timeZone = sntp_get_timezone();
	*timeVal += timeZone * 60* 60;
}

#if 1
static int set_local_time(unsigned int *ntp_data)
{
/* straight out of RFC-1305 Appendix A */
	int li, vn, mode;
	long time_v;
	char* buff;
	sntp_tm time;
	
#define Data(i) ntohl(((unsigned int *)ntp_data)[i])
	li      = Data(0) >> 30 & 0x03;
	vn      = Data(0) >> 27 & 0x07;
	mode    = Data(0) >> 24 & 0x07;

	if ((li == 3) || (vn < 3) || (mode != 4))
		return -1;

	/* it would be even better to subtract half the slop */
	time_v  = Data(10);
	bool is_1900_based = ((time_v & 0x80000000) != 0);
	time_v  = is_1900_based ? (time_v - JAN_1970):(time_v + JAN_2036);
	
	/* divide xmttime.fine by 4294.967296 */
	set_system_time(&time_v);
	buff = sntp_get_real_time(time_v);
	time = get_time_buff();
	/*TODO:
	* write data to RTC device 
	*/
	if(wifiTimeCB != NULL) {
		wifiTimeCB(time,time_v);
	}
	//printf("current time is %s \n",buff);

#undef Data
	return 0;
}
#endif
#if 1
int getUTCtime(char *server_name,void(*wifiTime_callBack)(sntp_tm,long)) {
	int sock_fd, n, echoLen;
	int checkFailTime = 0;
	char checkSendResult = 0;
	wifiTimeCB = wifiTime_callBack;
	
	struct sockaddr_in server_addr, echoServAddr;

	unsigned int receive_ntp_data[12] = {0};
	int flags = 0;
	int timeOut = 3000; // ms

	if (-1 == init_socket(server_name, &sock_fd, &server_addr)) {
		printf("init socket fails\n");
		return -1;
	}
	lwip_setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeOut, sizeof(int));

	while (1) {
			checkSendResult = send_ntp_packet(sock_fd, server_addr);
			if (checkSendResult < 0) {
				return -1;
			}
			memset(receive_ntp_data, 0, sizeof(receive_ntp_data));
			/* receive echoed message */
			echoLen = sizeof(echoServAddr);
			{
//				printf("\r\n==================starting update time=================\r\n");
				n = lwip_recvfrom(sock_fd, receive_ntp_data,  sizeof(receive_ntp_data), flags,(struct sockaddr *) &echoServAddr, &echoLen);
				if (n<=0) {
					if (checkFailTime++ > 1) {
						checkFailTime = 0;
						return -1;
					}
					//checkSendResult = send_ntp_packet(sock_fd, server_addr);
					//if(checkSendResult < 0){
					//	return -1;
					//}
					continue;
				}

				if (!set_local_time(receive_ntp_data)) {
					break;
				}
			}
	}
	if (sock_fd > 0) {
		lwip_close(sock_fd);
	}
	return 1;
}
#endif

