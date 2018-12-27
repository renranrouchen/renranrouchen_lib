#include "at_cmd.h"
#include "at_cmd_func.h"
#include <event.h>
#include <lynx_dev.h>
#include <wla_api.h>
#include <net_api.h>
#include <socket_api.h>
#include "lwip/sockets.h"
#include "lwip/raw.h"

#include <cfg_api.h>
#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif


char trans_mode = 0;
int g_linkIdx = 0;
static char s_tsmodeEnable = 0;
static int s_dataLenth = -1;

#ifdef CONFIG_LWIP
extern sdk_param g_atParam;

extern unsigned int ping_dip;
extern unsigned int ping_data_size;
extern unsigned int ping_rcv_timeo;
extern unsigned int ping_iteration;
extern unsigned int ping_interval;
extern unsigned int ping_tx;
extern unsigned int ping_rx;

/*!-----------------------------------------------------------------------------
 * function: at_net_dhcp
 *
 *  \brief:    Set or query DHCP mode
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  0:all disable; 1:enable AP dhcps; 2:enable STA dhcpc; 3:all enable
 *  \param 1:  enable or disable DHCP
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_net_dhcp(int argc, char *argv[], char *op[])		//dhcp
{
	if (!strcmp(op,"=")) {
		int val;
		if (argc != 1)
			return at_ERR_HELP;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;

		val = atoi(argv[0]);
		if ((val < 0) || (val > 3))
			return at_ERR_HELP;

		if (g_atParam.dhcp_mode == val)
			return at_ERR_OK;

		g_atParam.dhcp_mode = val;
		save_at_param();
		wifi_restart();
	} else if (!strcmp(op,"=?")) {
		if (argc != 0)
			return at_ERR_HELP;
		at_printf("\n<%d>\n", g_atParam.dhcp_mode);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_net_dhcps
 *
 *  \brief:    Set or query the range of the ip when dhcp
 *  \operator: "="set, "?"query the current setting
 *  \param 0:  0:clear the param;1:set the param
 *  \param 1:  leave time [0-2880] minutes
 *  \param 2:  start ip
 *  \param 3:  end ip
 *  \return:   check result

 +----------------------------------------------------------------------------*/
int at_net_dhcps(int argc, char *argv[], char *op[])		//dhcps
{
#ifdef CONFIG_DHCPS
	dhcps_param *dhs_ptr;

	dhs_ptr = &g_atParam.dhs_param;
	if (!strcmp(op,"=")) {
		if (argc != 4)
			return at_ERR_HELP;
		if (strchr(argv[0], ' ') || strchr(argv[1], ' ') ||
			strchr(argv[2], ' ') || strchr(argv[3], ' '))
			return at_ERR_PARM;

		if (atoi(argv[0]) == 0) {
			reset_dhcp_ip_range();
		} else if(atoi(argv[0]) == 1) {
			int leavetime = atoi(argv[1]);
			char* startip = argv[2];
			char* endip = argv[3];
			int min = 0,max = 0;

			if (!strcmp(dhs_ptr->startip, startip) &&
				!strcmp(dhs_ptr->endip, endip) &&
				(dhs_ptr->leave_time == leavetime))
				return at_ERR_OK;

			min = checkIpValuable(startip);
			max = checkIpValuable(endip);
			serial_printf("<min:%d><max:%d>\n",min,max);

			if ((min < 0) || (max < 0))
				return at_ERR_PARM;
			if ((leavetime < 1) || (leavetime > 2880))
				return at_ERR_PARM;

			set_dhcp_ip_range((unsigned char)min,(unsigned char)max);
			set_dhcp_leavetime(leavetime);
			memcpy(dhs_ptr->startip,startip,16);
			memcpy(dhs_ptr->endip,endip,16);
			dhs_ptr->leave_time = leavetime;
			save_at_param();
			wifi_restart();
		} else {
			return at_ERR_HELP;
		}
	} else if(!strcmp(op,"=?")) {
		at_printf("<leave time:%d><start ip:%s><end ip:%s>\n",
						dhs_ptr->leave_time,
						dhs_ptr->startip,
						dhs_ptr->endip);
	} else {
		return at_ERR_OP;
	}
#endif
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_net_ip
 *
 *  \brief:    Set or query static IP
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  BSS index
 *  \param 1:  address&mask&gateway
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_net_ip(int argc, char *argv[], char *op[])		//ip
{
	char ip[16];
	char mask[16];
	char gw[16];
	char buf[128];
	sta_param *sta_ptr;
	softap_param *ap_ptr;
	int role;

	sta_ptr = &g_atParam.sta_param;
	ap_ptr = &g_atParam.ap_param;
	memset(ip,0x0,sizeof(ip));
	memset(mask,0x0,sizeof(mask));
	memset(gw,0x0,sizeof(gw));

	if (!strcmp(op,"=")) {
		if (argc != 2)
			return at_ERR_HELP;
		if (strchr(argv[0], ' ') || strchr(argv[1], ' '))
			return at_ERR_PARM;
		if (parseAddress(argv[1],ip,mask,gw) < 0)
			return at_ERR_HELP;

		role = atoi(argv[0]);
		if ((role != SOFT_AP) && (role != STATION))
			return at_ERR_PARM;

		if (role == SOFT_AP) {
			if (!strcmp(ap_ptr->ip, ip) &&
				!strcmp(ap_ptr->mask, mask) &&
				!strcmp(ap_ptr->gw, gw))
				return at_ERR_OK;
			memcpy(ap_ptr->ip,ip,sizeof(ip));
			memcpy(ap_ptr->mask,mask,sizeof(mask));
			memcpy(ap_ptr->gw,gw,sizeof(gw));
		}

		if (role == STATION) {
			if (!strcmp(sta_ptr->ip, ip) &&
				!strcmp(sta_ptr->mask, mask) &&
				!strcmp(sta_ptr->gw, gw))
				return at_ERR_OK;
			memcpy(sta_ptr->ip,ip,sizeof(ip));
			memcpy(sta_ptr->mask,mask,sizeof(mask));
			memcpy(sta_ptr->gw,gw,sizeof(gw));
		}

		save_at_param();
		wifi_restart();
	} else if (!strcmp(op,"=?")) {
		ip_sts outNetpara;
		if (argc != 1)
			return at_ERR_HELP;

		role = atoi(argv[0]);
		if ((role != SOFT_AP) && (role != STATION))
			return at_ERR_PARM;

		net_if_ip_sts((void *)&outNetpara, role);

		at_printf("\n<ip=%s&mask=%s&gw=%s>\n",
					outNetpara.ip,
					outNetpara.mask,
					outNetpara.gate);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_net_mac
 *
 *  \brief:    Set or query mac address
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  BSS index
 *  \param 1:  mac adress
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_net_mac(int argc, char *argv[], char *op[])		//ip
{
	unsigned char mac[6] = {0x0,};

	if (!strcmp(op,"=")) {
		int role;
		if (argc != 2)
			return at_ERR_HELP;
		if (strchr(argv[0], ' ') || strchr(argv[1], ' '))
			return at_ERR_PARM;
		macToArray(argv[1], mac);
		role = atoi(argv[0]);

		if ((role != SOFT_AP) && (role != STATION))
			return at_ERR_PARM;

		if (role == SOFT_AP) {
			if (memcmp(g_atParam.mac.ap_mac, mac, 6) == 0)
				return at_ERR_OK;
			memcpy(g_atParam.mac.ap_mac,mac,6);
		}

		if (role == STATION) {
			if (memcmp(g_atParam.mac.sta_mac, mac, 6) == 0)
				return at_ERR_OK;
			memcpy(g_atParam.mac.sta_mac, mac, 6);
		}

		wlan_set_myaddr(role, mac);
		save_at_param();
		wifi_restart();
	} else if (!strcmp(op,"=?")) {
		if (argc != 1)
			return at_ERR_HELP;
		at_printf("\n<mac>:%s\n", ether_ntoa(wlan_get_myaddr(atoi(argv[0]))));
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_net_ping
 *
 *  \brief:    Ping net address
 *  \operator: "="set
 *  \param 0:  destination address
 *  \param 1:  packet size
 *  \param 2:  iteration
 *  \param 3:  timeout
 *  \param 4:  interval
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_net_ping(int argc, char *argv[], char *op[])		//ping
{
	if (!strcmp(op,"=")) {
		unsigned int dip, size, iter, to, interval;

		if (argc <= 0)
			return at_ERR_HELP;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;
		size = iter = to = interval = 0;
		if ((argc > 0) && (inet_aton(argv[0], &dip) < 0))
			return at_ERR_HELP;
		if (argc > 1)
			size = atoi(argv[1]);
		if (argc > 2)
			iter = atoi(argv[2]);
		if (argc > 3)
			to = atoi(argv[3]);
		if (argc > 4)
			interval = atoi(argv[4]);
#if 0
		net_ping(dip, &size, &iter, &to, &interval);
#else
		if (size <= 0)
			size = 32;
		if (size > 1472)
			size = 1472;
		if (iter <= 0)
			iter = 4;
		if (to <= 0)
			to = 3;
		if (interval <= 0)
			interval = 10;

		ping_dip = dip;
		ping_data_size = size;
		ping_rcv_timeo = (to)*1000;
		ping_iteration = iter;
		//ping_interval = (interval)*100;
		ping_tx = ping_rx = 0;
		int s;
		static int timeout = 0;
		ip_addr_t ping_target;

		if ((s = lwip_socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP)) < 0) {
			return at_ERR_FAIL;
		}
		lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &ping_rcv_timeo, sizeof(int));

		while (1) {
			if (ping_iteration)
				ping_iteration--;
			else
				break;
			ping_target = *(ip_addr_t *)&ping_dip;

			if (ping_send(s, &ping_target) == ERR_OK) {
				serial_printf("ping: send %d to %s\n", ping_data_size, inet_ntoa(ping_dip));
				ping_tx++;
				//ping_time = sys_now();
				ping_recv(s);
			} else {
				serial_printf("ping: send %s - error\n", inet_ntoa(ping_dip));
			}
		}
		//serial_printf("Release Socket.\n");
		lwip_close(s);

		serial_printf("tx=%d rx=%d\n", ping_tx, ping_rx);
		if(ping_tx == ping_rx)
			return at_ERR_OK;
		else
			return at_ERR_FAIL;
#endif
	} else {
		return at_ERR_OP;
	}
}

/*!-----------------------------------------------------------------------------
 * function: at_net_dns
 *
 *  \brief:    Set or query DNS server
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  server ID, 0-1
 *  \param 1:  address
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_net_dns(int argc, char *argv[], char *op[])		//dns
{
	unsigned int ipaddr;

	if (!strcmp(op,"=")) {
		if (argc != 2)
			return at_ERR_HELP;
		if (inet_aton(argv[1], &ipaddr) < 0)
			return at_ERR_HELP;
		net_set_dns(atoi(argv[0]), &ipaddr);
	} else if (!strcmp(op,"=?")) {
		if (argc != 1)
			return at_ERR_HELP;
		at_printf("\n<%s>\n", net_get_dns(atoi(argv[0]), &ipaddr));
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_net_host
 *
 *  \brief:    Query host address
 *  \operator: "="set the current setting
 *  \param 0:  host name
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_net_host(int argc, char *argv[], char *op[])		//host
{
	if (!strcmp(op,"=")) {
		char hostname[256];
		char ipaddr[16]={0};

		memset(hostname, 0, sizeof(hostname));
		memset(ipaddr, 0, sizeof(ipaddr));
		if (argc != 1)
			return at_ERR_HELP;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;
		if (1 != sscanf(argv[0], "%s", hostname))
			return at_ERR_HELP;
		net_get_hostbyname(hostname, ipaddr);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_socket_info
 *
 *  \brief:    Information of all connection.
 *  \operator: NULL execute
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_socket_info(int argc, char *argv[], char *op[])	//sinfo
{
	if (strcmp(op,NUL))
		return at_ERR_OP;
	net_socket_info();
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_socket_add
 *
 *  \brief:    Establish a TCP or UDP connection.
 *  \operator: "="set the current setting
 *  \param 0:  socket ID
 *  \param 1:  socket type, TCP or UDP
 *  \param 2:  remote IP
 *  \param 3:  remote port, 0 ~ 65535
 *  \param 4:  [TCP:keeplive,UDP:lport]
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_socket_add(int argc, char *argv[], char *op[])	//sadd
{
	if (!strcmp(op,"=")) {
		if (argc < 5)
			return at_ERR_HELP;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;

		net_socket_mutiConn(1);
		if (net_socket_add(argc, argv) < 0)
			return at_ERR_PARM;
	} else {
		return at_ERR_OP;
	}
	tcp_rx_start();
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_socket_del
 *
 *  \brief:    Delete TCP or UDP connection
 *  \operator: "="set the current setting
 *  \param 0:  socket ID
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_socket_del(int argc, char *argv[], char *op[])		//sdel
{
	if (!strcmp(op,"=")) {
		if (argc != 1)
			return at_ERR_HELP;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;

		if (net_socket_del(atoi(argv[0])) < 0)
			return at_ERR_PARM;
	} else {
		return at_ERR_OP;
	}
	tcp_rx_stop();
	return at_ERR_OK;
}
int checkExitTransMode(int len)
{
    if(len >= s_dataLenth && s_dataLenth > 0)
		return 1;

	return 0;
}

int getSendLength(void)
{
	return s_dataLenth;
}

int setSendLength(int len)
{
	s_dataLenth = len;
}

int start_trans_mode(void)
{
	trans_mode = 1;
	return 0;
}

/*!-----------------------------------------------------------------------------
 * function: at_socket_send
 *
 *  \brief:    Send data
 *  \operator: "="set the current setting
 *  \param 0:  socket ID
 *  \param 1:  length
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_socket_send(int argc, char *argv[], char *op[])	//ssend
{
	if (!strcmp(op,"=")) {
		if (argc == 1) {
			g_linkIdx = atoi(argv[0]);
			setSendLength(-1);
		} else if (argc == 2) {
			g_linkIdx = atoi(argv[0]);
			setSendLength(atoi(argv[1]));
		} else {
			return at_ERR_HELP;
		}
	} else if (!strcmp(op,NUL)) {
		g_linkIdx = 0;
		setSendLength(-1);
	} else {
		return at_ERR_OP;
	}
	
	if (check_socket_valiable(g_linkIdx)&&s_tsmodeEnable)
		trans_mode = 1;
	else
	{
		serial_printf(" please enable the transmode or make sure socket is valid\n");
		return at_ERR_FAIL;
	}

	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: trans_mode
 *
 *  \brief:    Set module into transparent transmission mode
 *  \operator: NULL execute
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_trans_mode(int argc, char *argv[], char *op[])			//tmode
{
	if (!strcmp(op,"=")) {
		if (argc != 1)
			return at_ERR_PARM;

		s_tsmodeEnable = atoi(argv[0]);
		//trans_mode = atoi(argv[0]);
		if(s_tsmodeEnable)
			return at_ERR_TRANS;
	} else if (!strcmp(op,"=?")) {
		at_printf("\n<%d>\n",s_tsmodeEnable);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}
/*!-----------------------------------------------------------------------------
 * function: at_trans_savelink
 *
 *  \brief:    Send data
 *  \operator: "="set the current setting
 *  \param 0:  
 *  \param 1:  
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_trans_savelink(int argc, char *argv[], char *op[])
{
	trans_param *ts_ptr;

	if (!strcmp(op,"=")) {
		if (argc < 3)
			return at_ERR_HELP;

		ts_ptr = &g_atParam.ts_param;
		ts_ptr->enable = atoi(argv[0]);
		memcpy(ts_ptr->ip, argv[1],strlen(argv[1]));
		ts_ptr->port = atoi(argv[2]);
		memcpy(ts_ptr->type, "TCP",sizeof(ts_ptr->type));
		ts_ptr->time_alive = 0xffff;
		if (argc == 4) {
			if (strlen(argv[3]) > sizeof(ts_ptr->type))
				return at_ERR_PARM;

			memcpy(ts_ptr->type, argv[3],sizeof(ts_ptr->type));
		}
		if(argc == 5)
			ts_ptr->time_alive = atoi(argv[4]);

		save_at_param();
		serial_printf("enable:%d\n",ts_ptr->enable);
		serial_printf("ip:%s\n",ts_ptr->ip);
		serial_printf("port:%d\n",ts_ptr->port);
		serial_printf("type:%s\n",ts_ptr->type);
		serial_printf("time_alive:%d\n",ts_ptr->time_alive);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_update
 *
 *  \brief:    update the sdk
 *  \operator: "="set the current setting
 *  \param 0:  url
 *  \param 1:  port
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_update(int argc, char *argv[], char *op[])
{
	int ret = 0;
	if (!strcmp(op,"=")) {
		if (argc != 2)
			return at_ERR_HELP;

		ret = Firmware_WIFIOTAByUrl(argv[0],atoi(argv[1]));

		serial_printf("\n url:%s\n",argv[0]);
		serial_printf("\n port:%d\n",atoi(argv[1]));
	} else {
		return at_ERR_OP;
	}
	return ret;
}

/*!-----------------------------------------------------------------------------
 * function: at_socket_TCPServer
 *
 *  \brief:    creat the tcp server
 *  \operator: "="set the current setting
 *  \param 0:  mode;1:create the server;0:close the server
 *  \param 1:  port
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_socket_TCPServer(int argc, char *argv[], char *op[])
{
	int start = 0;
	unsigned short port = 0;
	if (!strcmp(op,"=")) {
		if (argc < 1)
			return at_ERR_HELP;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;

		if (atoi(argv[0]) == 1) {
			if (argc == 2)
				port = atoi(argv[1]);
			else
				port = 333;

			net_socket_mutiConn(1);
			tcpserver_start(port);
		} else {
			tcpserver_stop();
		}
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

#endif

