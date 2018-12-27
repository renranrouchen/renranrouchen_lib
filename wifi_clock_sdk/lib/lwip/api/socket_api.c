/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file socket_api.c
*   \brief  Socket API functions.
*   \author Montage
*/

/*=============================================================================+
| Included Files
+=============================================================================*/
#include <common.h>
#include <lynx_dev.h>
#include <wbuf.h>
#include <event.h>
#include <netif/etharp.h>
#include <lwip/tcpip.h>
#include <net_api.h>
#include <socket_api.h>
#include <wla_api.h>

static int s_timeout = 0;
#define TCPSOCKET_BUFFERSIZE    (1*1024)
int conn_amount;                 // current connection amount  
static char tcpserver_running = 0;
static char tcpserver_sig = 0;
static char tcprx_running = 0;
char tcprx_sig = 0;

static int s_tcpServerSocHd = -1;
enum
{
	TASK_INIT = 1,
	TASK_DOWN = 2,
};

extern int at_cmd_loop(void);
static int s_mutiConn = 0;
static int s_dinfo = 0;
void tcp_rx_start();
void tcp_rx_stop(void);

/*----------------------------------------------------------------*/
/**
 * The function initials buffer and invokes net_socket_tx() to sends data.
 *
 * @param len The data length.
 * @return None.
 */
/*----------------------------------------------------------------*/
static void net_xmit_socket_data(int len)
{
	if ((len == 3) && !strncmp(idev->trans_rxb,"+++",3))
	{
		serial_printf("\nBack to AT\n");

		EVENT_REGISTER((evt_func)at_cmd_loop, EVT_UART1_RX_NUM);
	}
	idev->trans_rxlen = len;
	idev->trans_rxb[len] = '\0';
	net_socket_tx(0, idev->trans_rxb,len);
	idev->trans_rxlen = 0;
}

/*----------------------------------------------------------------*/
/**
 * The function sends data to the serial port.
 *
 * @param None.
 * @return None.
 */
/*----------------------------------------------------------------*/
void net_ur_tx(void)
{
	int stat;
	if ((idev->trans_txlen == 3) && !strncmp(idev->trans_txb,"+++",3)) {
		serial_printf("\nBack to AT\n");

		if (ldev->ur.chan == 3)//ldev->udev.cfg_len == 67
			EVENT_REGISTER((evt_func)at_cmd_loop, EVT_HOST_NUM);
		else
			EVENT_REGISTER((evt_func)at_cmd_loop, EVT_UART_RX_NUM);
	} 
	else 
	{
		stat = serial_write(ldev->ur.chan, idev->trans_txb, idev->trans_txlen);
		if (stat == 1) {
			if (serial_write(ldev->ur.chan, idev->trans_txb, idev->trans_txlen) == 1)
				serial_printf("tx buffer full\n");
		} else if (stat == -1) {
			serial_printf("Error\n");
		}
	}
	idev->trans_txlen = 0;
}

/*----------------------------------------------------------------*/
/**
 * The function polls data from then serial port and sends data to the socket.
 *
 * @param ev_num The event number.
 * @return 0: succeed\n -1: failed
 */
/*----------------------------------------------------------------*/
int net_ur_rx(int ev_num)
{
	int key, max_len;
	char *bp;// = idev->trans_rxb + idev->trans_rxlen;

	max_len = sizeof(idev->trans_rxb) - 1;
	while (1) {
		bp = idev->trans_rxb + idev->trans_rxlen;
		do {
			if (!ldev->serial_poll(ldev->ur.chan, &key)) {
				net_xmit_socket_data(bp - idev->trans_rxb);
				return -1;
			}
			*bp++ = key;
		} while ((bp - idev->trans_rxb) < max_len);

		net_xmit_socket_data(max_len);
	}
	return 0;
}

/*----------------------------------------------------------------*/
/**
 * The function dumps all sockets information.
 *
 * @param None.
 * @return None.
 */
/*----------------------------------------------------------------*/
void net_socket_info(void)
{
	int i, len, val;
	char tmp[64], *str;
	struct sockaddr_in localaddr, *fromaddr;
	link_sts linkStatus;
	int length = 0;

	wlan_get_link_sts(&linkStatus, STATION);
	serial_printf(" STATUS=%d\n",linkStatus.is_connected);

	for (i=0; i<MEMP_NUM_NETCONN; i++)
	{
		if (idev->s[i] == -1)
		{
			continue;
		}
		fromaddr = &idev->fromaddr[i];
		len = 0;
		len += sprintf(&tmp[len], "%d ", i);
		val = idev->type[i];
		if (val == SOCK_STREAM)
			str = "TCP";
		else if (val == SOCK_DGRAM)
			str = "UDP";
		else
			str = "RAW";
		len += sprintf(&tmp[len], "%s ", str);
		len += sprintf(&tmp[len], "%s ",
					inet_ntoa(ntohl(fromaddr->sin_addr.s_addr)));
		len += sprintf(&tmp[len], "%d ", ntohs(fromaddr->sin_port));
		getsockname( idev->s[i], ( struct sockaddr* )&localaddr, &length );
		len += sprintf(&tmp[len], "%d", ntohs(localaddr.sin_port));
#ifdef CONFIG_AT_CMD_FUNC
		at_printf("%s\n", tmp);
#endif
	}
}

int check_socket_valiable(int idx)
{
	if (idev->s[idx] < 0)
	{
		return 0;
	}
	return 1;

}
int close_soctet(int socket)
{
	if(socket != -1)
	{
		closesocket( socket );
	}
	else
	{
		return -1;
	}
	return 0;
}
/*----------------------------------------------------------------*/
/**
 * The function adds a socket.
 *
 * @param argc Arguments count.
 * @param argv[0]: The socket index.
 * @param argv[1]: The socket type, TCP or UDP.
 * @param argv[2]: The IP address.
 * @param argv[3]: Destination port, 0~65535.
 * @param argv[4]: [TCP:keeplive,UDP:lport]
 * @return 0: succeed\n -1: failed
 */
/*----------------------------------------------------------------*/
int net_socket_add(int argc, char *argv[])
{
	unsigned int  r_port, l_port;
	int s, type, prot, idx, ret;
	struct sockaddr_in *localaddr, *fromaddr;
	char *conType = NULL;
	char *ip =NULL;
	int udpmode = 0;
	int value = 0;
	int keeplive = 0;

	if (s_mutiConn == 1)
	{
		idx = atoi(argv[0]);
		conType = argv[1];
		if (!strncmp(conType, "TCP",3))
		{
			type = SOCK_STREAM;
			prot = IPPROTO_TCP;
			ip = argv[2];
			r_port = atoi(argv[3]);
			if (argc == 5)
			{
				keeplive = atoi(argv[4]);
			}
		}
		else if (!strncmp(conType, "UDP",3))
		{
			type = SOCK_DGRAM;
			prot = IPPROTO_UDP;
			ip = argv[2];
			r_port = atoi(argv[3]);
			if (argc == 6)
			{
				l_port = atoi(argv[4]);
				udpmode = atoi(argv[5]);
			}
		}
	}
	else
	{
		idx = 0;
		conType = argv[0];
		if (!strncmp(conType, "TCP",3))
		{
			type = SOCK_STREAM;
			prot = IPPROTO_TCP;
			ip = argv[1];
			r_port = atoi(argv[2]);
			if (argc == 4)
			{
				keeplive = atoi(argv[3]);
			}
		}
		else if (!strncmp(conType, "UDP",3))
		{
			type = SOCK_DGRAM;
			prot = IPPROTO_UDP;
			ip = argv[1];
			r_port = atoi(argv[2]);
			if (argc == 5)
			{
				l_port = atoi(argv[3]);
				udpmode = atoi(argv[4]);
			}
		}
	}
	if ((idx < 0) || (idx >= MEMP_NUM_NETCONN))
		goto help;

	if (idev->s[idx] != -1)
	{
		serial_printf(" exist\n");
		goto help;
	}

	if ((r_port < 0) || (r_port > 65535))
		goto help;

	/* FIXME: link and network status */
	localaddr = &idev->localaddr[idx];
	fromaddr = &idev->fromaddr[idx];
	s = socket(AF_INET, type, prot);
	if (s < 0)
	{
		serial_printf(" get sock error\n");
		goto help;
	}	
	if (prot == IPPROTO_UDP)
	{
		localaddr->sin_family = AF_INET;
		localaddr->sin_port = htons(l_port);
		localaddr->sin_addr.s_addr = htonl(0x00000000UL);

		fromaddr->sin_family = AF_INET;
		fromaddr->sin_port = htons(r_port);
		fromaddr->sin_addr.s_addr = inet_addr(ip);
		ret = connect(s,(struct sockaddr *)fromaddr, sizeof( struct sockaddr_in));

	}
	else if (prot == IPPROTO_TCP)
	{		
		fromaddr->sin_family = AF_INET;
		fromaddr->sin_port= htons(r_port);
		fromaddr->sin_addr.s_addr = inet_addr(ip);
		ret = connect(s,(struct sockaddr *)fromaddr, sizeof( struct sockaddr_in));
		if (keeplive > 0 && keeplive < 7200)
		{
			int optval = 1;
			if (setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(int)) == -1)
			{
				serial_printf(" set sock error\n");
				return -1;
			}
			if (setsockopt(s, IPPROTO_TCP, TCP_KEEPIDLE, (char *) &keeplive, sizeof(int)) == -1)
			{
				serial_printf(" set sock error\n");
				return -1;
			}
		}
	}
	serial_printf(" ret=%d\n", ret);
	if (ret == 0)
	{
		idev->s[idx] = s;
		idev->type[idx] = type;
	}
	else if (s >= 0)
	{
		closesocket(s);
		idev->s[idx] = -1;
	}

	return ret;
help:
	return -1;
}

/*----------------------------------------------------------------*/
/**
 * The function deletes a socket.
 *
 * @param idx The socket index.
 * @return 0: succeed\n -1: failed
 */
/*----------------------------------------------------------------*/
int net_socket_del(int idx)
{
	int s;

	if ((idx < 0) || (idx >= MEMP_NUM_NETCONN))
	{
		goto help;
	}
	s = idev->s[idx];
	if (s == -1)
	{
		goto help;
	}
	closesocket(s);
	idev->s[idx] = -1;
	return 0;
help:
	return -1;
}

/*----------------------------------------------------------------*/
/**
 * The function sends data to the socket.
 *
 * @param idx The socket index.
 * @return 0: succeed\n -1: failed
 */
/*----------------------------------------------------------------*/
int net_socket_tx(int idx, char *data,int len)
{
	socklen_t socklen;
	int s;
	int ret;

	if ((idx < 0) || (idx >= MEMP_NUM_NETCONN))
		goto help;
	s = idev->s[idx];
	if (s == -1)
		goto help;
	//serial_printf("\n <%s><%d><%d>!!\n",__FUNCTION__,__LINE__,len);

	socklen = sizeof(struct sockaddr_in);
	ret = sendto(s, data, len, 0,
			(struct sockaddr *)&idev->fromaddr[idx], socklen);
	if(ret <= 0)
		serial_printf("[%s] sendto error %d\n", __FUNCTION__, ret);

	return 0;
help:
	return -1;
}

/*----------------------------------------------------------------*/
/**
 * The function gets data from all socket.
 *
 * @param idx The socket index.
 * @return 0: succeed\n -1: failed
 */
/*----------------------------------------------------------------*/
extern char trans_mode;
int net_socket_rx(void)
{
	socklen_t socklen;
	int ret = -1, s, i;
	int len, c_null, c_enter, max_len;

	socklen = sizeof(struct sockaddr_in);
	max_len = sizeof(idev->trans_txb);
	for (i=0; i<MEMP_NUM_NETCONN; i++) {
		if ((s = idev->s[i]) == -1)
			continue;
		len = recvfrom(s, &idev->trans_txb[idev->trans_txlen], 2048, 0,
						(struct sockaddr *)&idev->fromaddr[i], &socklen);
		if (len < 0) {
			continue;
		} else {
			idev->trans_txlen += len;
			c_enter = idev->trans_txlen;
			c_null = idev->trans_txlen + 1;
			idev->trans_txlen++;
			if (c_null >= max_len) {
				c_enter = max_len - 2;
				c_null = max_len - 1;
				idev->trans_txlen = max_len;
			}
	 		idev->trans_txb[c_enter] = '\r';
	 		idev->trans_txb[c_null] = '\0';
			ret = 0;
		}
	}

	return ret;
}


int net_socket_reveive(int idx,int socketId)
{
	socklen_t socklen;
	int len, c_null, c_enter, max_len;
	int stat;

	socklen = sizeof(struct sockaddr_in);
	max_len = sizeof(idev->trans_txb);

	memset(idev->trans_txb,0x0,sizeof(idev->trans_txb));
	idev->trans_txlen = 0;

	
	if (socketId == -1)
	{
		return -1;
	}
	len = recvfrom(socketId, &idev->trans_txb[idev->trans_txlen], 2048, 0,
					(struct sockaddr *)&idev->fromaddr[idx], &socklen);
	if (len < 0) 
	{
		serial_printf("[%d][%s] idx %d len %d\n",__LINE__,__FUNCTION__, idx, len);
		net_socket_del(idx);
		tcp_rx_stop();
	} 
	else 
	{
		idev->trans_txlen += len;
#if 1
		if(idev->trans_txlen >= max_len)
			idev->trans_txlen = max_len;
#else
		c_enter = idev->trans_txlen;
		c_null = idev->trans_txlen + 1;
		idev->trans_txlen++;
		if (c_null >= max_len) 
		{
			c_enter = max_len - 2;
			c_null = max_len - 1;
			idev->trans_txlen = max_len;
		}
		idev->trans_txb[c_enter] = '\r';
		idev->trans_txb[c_null] = '\0';
#endif
		if(trans_mode)
		{
			//serial_printf("%s",idev->trans_txb);
		}
		else
		{
			serial_printf("\n IPD+,");
			if(s_mutiConn)
			{
				serial_printf("%d,",idx);
			}
			
			if(s_dinfo)
			{
				serial_printf("%d,",len);
				serial_printf("%s,",inet_ntoa(idev->fromaddr[idx].sin_addr));
				serial_printf("%d:",ntohs(idev->fromaddr[idx].sin_port));
			}
			else
			{
				serial_printf("%d:",len);

			}
			serial_printf(" %s\n",idev->trans_txb);
		}
		stat = serial_write(0, idev->trans_txb, idev->trans_txlen);
		if (stat == 1)
		{
			serial_printf("tx buffer full %d\n", idev->trans_txlen);
		}
		else if (stat == -1)
		{
			serial_printf("Error\n");
		}
	}

	//serial_write(0, idev->trans_txb, idev->trans_txlen);
	return 0;
}

int net_socket_mutiConn(int isMutiConn)
{
	s_mutiConn = isMutiConn;
	serial_printf(" s_mutiConn=%d\n",s_mutiConn);
}
int get_socket_mutiConn(void)
{
	return s_mutiConn;
}

int net_socket_dinfo(int mode)
{
	s_dinfo = mode;
	serial_printf("\n net_socket_dinfo:%d \n",mode);
}


int get_tcpserver_timeout(void)
{
	if(s_tcpServerSocHd == -1)
	{
		serial_printf("\n tcp server not created\n");
		return -1;
	}
	return s_timeout;

}

int set_tcpserver_timeout(int time)
{
	if(s_tcpServerSocHd == -1)
	{
		serial_printf("\n tcp server not created\n");
		return -1;
	}
	s_timeout = time/1000;
	setsockopt(s_tcpServerSocHd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&time,sizeof(int));
}

int close_all_socket(void)
{
	int i = 0;
	for (i = 0; i < MEMP_NUM_NETCONN; i++)	
	{  
		if (idev->s[i] != -1)  
		{  
			close_soctet(idev->s[i]);
			idev->s[i] = -1;	
		}  
	} 
	return 0;
}
void close_tcpserver(void)
{
	tcprx_sig = RECEIVE_DISENABLE;
	close_all_socket();
	if(s_tcpServerSocHd != -1)
	{
		close_soctet( s_tcpServerSocHd );
		s_tcpServerSocHd = -1;
	}
	serial_printf("<%s><%d>\n",__FUNCTION__,__LINE__);

}

void  createTcpServer( void *arg )
{
#ifdef CONFIG_FREERTOS
    struct sockaddr_in addr;
	struct sockaddr_in client_addr; // connector's address information 
    int bufferSize=0;;
    int new_fd=-1;	
	int i = 0;
	int ret = 0;
    unsigned int sin_size;  
	unsigned short tcp_port;
	if(arg == NULL)
	{
		tcp_port = 333;
	}
	else
	{
		tcp_port = *(unsigned short *)arg;
	}
	
    s_tcpServerSocHd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if( s_tcpServerSocHd < 0 )
    {
        s_tcpServerSocHd = -1;
        serial_printf("TCPServer socket create error\n");
		goto exit;
    }
    bufferSize = TCPSOCKET_BUFFERSIZE;
    setsockopt( s_tcpServerSocHd, SOL_SOCKET, SO_RCVBUF, &bufferSize, 4 );
    setsockopt( s_tcpServerSocHd, SOL_SOCKET, SO_SNDBUF, &bufferSize, 4 );
    memset(&addr, 0x0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(tcp_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if( bind( s_tcpServerSocHd, (struct sockaddr *)&addr, sizeof(addr)) == -1 )
    {
        serial_printf("TCPSrever socket bind error \n");
        closesocket(s_tcpServerSocHd);
        s_tcpServerSocHd = -1;
		goto exit;

    }

    if(listen( s_tcpServerSocHd, MEMP_NUM_NETCONN ) != 0 )
    {
        serial_printf("TCPServer socket listen error! \n");
        closesocket( s_tcpServerSocHd );
        s_tcpServerSocHd = -1;
		goto exit;

    }
	fd_set fdsr;  
    int maxsock;  
    struct timeval tv; 
    conn_amount = 0;  
    sin_size = sizeof(client_addr);  
    maxsock = s_tcpServerSocHd;  	
    while (1)  
    {  
		switch (tcpserver_sig) {
		case TASK_INIT:
			break;
		case TASK_DOWN:
			goto exit;
		default:
			break;
		}
		sys_msleep(100);	
        // initialize file descriptor set  
        FD_ZERO(&fdsr);  
        FD_SET(s_tcpServerSocHd, &fdsr);  
  
        // timeout setting  
        tv.tv_sec = 1;  
        tv.tv_usec = 0;  

        ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);  
        if (ret < 0)  
        {  
            serial_printf("<%s><%d>ret:%d! \n",__FUNCTION__,__LINE__,ret);  
            break;  
        }  
        else if (ret == 0)  
        {    
            continue;  
        }  
        // check whether a new connection comes  
        if (FD_ISSET(s_tcpServerSocHd, &fdsr))  
        {  
            new_fd = accept(s_tcpServerSocHd, (struct sockaddr *)&client_addr, &sin_size);  
            if (new_fd < 0)  
            {    
                continue;  
            }  
  
            // add to fd queue  
            if (conn_amount < MEMP_NUM_NETCONN)  
            {  
                idev->s[conn_amount++] = new_fd;  
                serial_printf("new connection client[%d] %s:%d\n", conn_amount, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));  
            }  
            else  
            {  
                serial_printf("max connections arrive, exit\n");  
            }  
			if(tcprx_sig != RECEIVE_ENABLE)
			{
				tcprx_sig = RECEIVE_ENABLE;
			}
        }  

    } 
exit:	
	close_tcpserver();
	vTaskDelete(NULL);
#endif	
}

void tcp_rx_start()
{
#ifdef CONFIG_FREERTOS
	if (tcprx_running)
		return;
	tcprx_sig = RECEIVE_ENABLE;
	tcprx_running = 1;
#endif	
}

void tcp_rx_stop(void)
{
	int i = 0;
	if (tcprx_running == 0)
		return;
	for (i = 0; i < MEMP_NUM_NETCONN; i++)
	{
		if (idev->s[i] != -1) 
		{
			return;
		}
	}

	tcprx_sig = RECEIVE_DISENABLE;
	tcprx_running = 0;
}

void tcpserver_start(unsigned short port)
{
#ifdef CONFIG_FREERTOS

	if (tcpserver_running)
		return;
	if (!xTaskCreate(createTcpServer, "tcpCreate", 4096, (void*)&port, 5,NULL)) {
		return;
	}

	tcpserver_sig = TASK_INIT;
	tcpserver_running = 1;
#endif	
}

void tcpserver_stop(void)
{
	if (tcpserver_running == 0)
		return;
	tcpserver_sig = TASK_DOWN;
	tcpserver_running = 0;
}

