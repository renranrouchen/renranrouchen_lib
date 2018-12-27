/*=============================================================================+
|                                                                              |
| Copyright 2015                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file dhcp_server.c
*   \brief main entry
*   \author Montage
*/

/*=============================================================================+
| Included Files                                                               |
+=============================================================================*/
#include <stdio.h>
#include <stdint.h>
#include <common.h>

#include <lwip/opt.h>
#include <lwip/sockets.h>
#include <lwip/inet_chksum.h>
#include <netif/etharp.h>
#include <lwip/ip.h>
#include <lwip/arch.h>
#include <lwip/mem.h>
#include "dhcp_server.h"

/* DHCP server option */

extern unsigned char iot_dhcpd_addr[4];
/* allocated client ip range */
#ifndef DHCPD_CLIENT_IP_MIN
#define DHCPD_CLIENT_IP_MIN     2
#endif
#ifndef DHCPD_CLIENT_IP_MAX
#define DHCPD_CLIENT_IP_MAX     254
#endif
#ifndef DHCPD_CLIENT_IP_LEAVETIME
#define DHCPD_CLIENT_IP_LEAVETIME     0x15180
#endif

/* the DHCP server address */
#ifndef DHCPD_SERVER_IPADDR0
#define DHCPD_SERVER_IPADDR0      iot_dhcpd_addr[0]
#define DHCPD_SERVER_IPADDR1      iot_dhcpd_addr[1]
#define DHCPD_SERVER_IPADDR2      iot_dhcpd_addr[2]
#define DHCPD_SERVER_IPADDR3      iot_dhcpd_addr[3]
#endif

#ifdef CONFIG_WLA_DEBUG_MSG
#define DEBUG_PRINTF        serial_printf("[DHCP] "); serial_printf
#else
#define DEBUG_PRINTF(...)
#endif /* CONFIG_WLA_DEBUG_MSG */

#define RT_ASSERT			LWIP_ASSERT
/* we need some routines in the DHCP of lwIP */
#undef  LWIP_DHCP
#define LWIP_DHCP   1
#include <lwip/dhcp.h>

/* buffer size for receive DHCP packet */
#define BUFSZ               1024
static int dhcpd_is_running = 0;
static int dhcpd_sig = 0;


typedef struct 
{
	unsigned char mac[6];
	unsigned char ip;
}map_ip;
static map_ip *s_ip_map = NULL;
static int s_ip_num;

static uint8_t s_client_ip_min = DHCPD_CLIENT_IP_MIN;
static uint8_t s_client_ip_max = DHCPD_CLIENT_IP_MAX;
static uint8_t next_client_ip = DHCPD_CLIENT_IP_MIN;
static int s_leavetime = DHCPD_CLIENT_IP_LEAVETIME;


void* xCreatedTask = NULL;

static err_t _low_level_dhcp_send(struct netif *netif,
                                     const void *buffer,
                                     size_t size)
{
    struct pbuf *p;
    struct eth_hdr *ethhdr;
    struct ip_hdr *iphdr;
    struct udp_hdr *udphdr;
	err_t result = ERR_OK;

    p = pbuf_alloc(PBUF_LINK,
                   SIZEOF_ETH_HDR + sizeof(struct ip_hdr)
                   + sizeof(struct udp_hdr) + size,
                   PBUF_RAM);
    if (p == NULL) return -ENOMEM;

    ethhdr = (struct eth_hdr *)p->payload;
    iphdr  = (struct ip_hdr *)((char *)ethhdr + SIZEOF_ETH_HDR);
    udphdr = (struct udp_hdr *)((char *)iphdr + sizeof(struct ip_hdr));

    ETHADDR32_COPY(&ethhdr->dest, (struct eth_addr *)&ethbroadcast);
    ETHADDR16_COPY(&ethhdr->src, netif->hwaddr);
    ethhdr->type = PP_HTONS(ETHTYPE_IP);

    iphdr->src.addr  = 0x00000000; /* src: 0.0.0.0 */
    iphdr->dest.addr = 0xFFFFFFFF; /* src: 255.255.255.255 */

    IPH_VHL_SET(iphdr, 4, IP_HLEN / 4);
    IPH_TOS_SET(iphdr, 0x00);
    IPH_LEN_SET(iphdr, htons(IP_HLEN + sizeof(struct udp_hdr) + size));
    IPH_ID_SET(iphdr, htons(2));
    IPH_OFFSET_SET(iphdr, 0);
    IPH_TTL_SET(iphdr, 255);
    IPH_PROTO_SET(iphdr, IP_PROTO_UDP);
    IPH_CHKSUM_SET(iphdr, 0);
    IPH_CHKSUM_SET(iphdr, inet_chksum(iphdr, IP_HLEN));

    udphdr->src = htons(DHCP_SERVER_PORT);
    udphdr->dest = htons(DHCP_CLIENT_PORT);
    udphdr->len = htons(sizeof(struct udp_hdr) + size);
    udphdr->chksum = 0;

    memcpy((char *)udphdr + sizeof(struct udp_hdr),
           buffer, size);

	result = netif->linkoutput(netif, p);
	pbuf_free(p);
	p = NULL;
    return result;
}

static void dhcpd_thread_entry(void *parameter)
{
    struct netif *netif = NULL;
    int sock;
    int bytes_read;
    unsigned char *recv_data;
    uint32_t addr_len;
    struct sockaddr_in server_addr, client_addr;
    struct dhcp_msg *msg;
    int optval = 1;
	int timeout=1000;
	int i = 0;
	char isGot = 0;
	unsigned char mac[6] = {0,};
    /* get ethernet interface. */
    netif = (struct netif*) parameter;
    RT_ASSERT("netif != NULL", netif != NULL);

    /* our DHCP server information */
    DEBUG_PRINTF("DHCP server IP: %d.%d.%d.%d  client IP: %d.%d.%d.%d-%d\n",
                 DHCPD_SERVER_IPADDR0, DHCPD_SERVER_IPADDR1,
                 DHCPD_SERVER_IPADDR2, DHCPD_SERVER_IPADDR3,
                 DHCPD_SERVER_IPADDR0, DHCPD_SERVER_IPADDR1,
                 DHCPD_SERVER_IPADDR2, s_client_ip_min, s_client_ip_max);

	

    /* allocate buffer for receive */
    recv_data = (char *)mem_malloc(BUFSZ);
    if (recv_data == NULL)
    {
        /* No memory */
        DEBUG_PRINTF("Out of memory\n");
        return;
    }

    /* create a socket with UDP */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
#ifdef ERRNO
        DEBUG_PRINTF("create socket failed, errno = %d\n", errno);
#else
        DEBUG_PRINTF("create socket failed\n");
#endif
        mem_free(recv_data);
        return;
    }

    /* set to receive broadcast packet */
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(int));

    /* initialize server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DHCP_SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    /* bind socket to the server address */
    if (bind(sock, (struct sockaddr *)&server_addr,
             sizeof(struct sockaddr)) == -1)
    {
        /* bind failed. */
#ifdef ERRNO
        DEBUG_PRINTF("bind server address failed, errno=%d\n", errno);
#else
        DEBUG_PRINTF("bind server address failed\n");
#endif
        mem_free(recv_data);
        return;
    }

    addr_len = sizeof(struct sockaddr);
    DEBUG_PRINTF("DHCP server listen on port %d...\n", DHCP_SERVER_PORT);

    while (1)
    {
		switch (dhcpd_sig) {
		case DHCPD_INIT:
			break;
		case DHCPD_DOWN:
			goto exit;
		default:
			break;
		}
		sys_msleep(1000);
        bytes_read = recvfrom(sock, recv_data, BUFSZ - 1, 0,
                              (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
        if (bytes_read < DHCP_MSG_LEN)
        {
            continue;
        }

		memcpy(mac,recv_data+DHCP_CHADDR_OFS,6);
	
        msg = (struct dhcp_msg *)recv_data;
        /* check message type to make sure we can handle it */
        if ((msg->op != DHCP_BOOTREQUEST) || (msg->cookie != PP_HTONL(DHCP_MAGIC_COOKIE)))
        {
            continue;
        }

        /* handler. */
        {
            uint8_t *dhcp_opt;
            uint8_t option;
            uint8_t length;

            uint8_t message_type = 0;
            uint8_t finished = 0;
            uint32_t request_ip  = 0;

            dhcp_opt = (uint8_t *)msg + DHCP_OPTIONS_OFS;


            while (finished == 0)
            {
                option = *dhcp_opt;
                length = *(dhcp_opt + 1);

                switch (option)
                {
                case DHCP_OPTION_REQUESTED_IP:
                    request_ip = *(dhcp_opt + 2) << 24 | *(dhcp_opt + 3) << 16
                                 | *(dhcp_opt + 4) << 8 | *(dhcp_opt + 5);
                    break;

                case DHCP_OPTION_END:
                    finished = 1;
                    break;

                case DHCP_OPTION_MESSAGE_TYPE:
                    message_type = *(dhcp_opt + 2);
                    break;

                default:
                    break;
                } /* switch(option) */

                dhcp_opt += (2 + length);
            }

            /* reply. */
            dhcp_opt = (uint8_t *)msg + DHCP_OPTIONS_OFS;

            /* check. */
	    /* peer station may send DISCOVER message type with requested IP which is not in the IP pool */
            if (request_ip &&
				(message_type != DHCP_DISCOVER) &&
				(message_type != DHCP_REQUEST))
            {
                uint32_t client_ip = DHCPD_SERVER_IPADDR0 << 24 | DHCPD_SERVER_IPADDR1 << 16
                                     | DHCPD_SERVER_IPADDR2 << 8 | (next_client_ip);

                if (request_ip != client_ip)
                {
                    *dhcp_opt++ = DHCP_OPTION_MESSAGE_TYPE;
                    *dhcp_opt++ = DHCP_OPTION_MESSAGE_TYPE_LEN;
                    *dhcp_opt++ = DHCP_NAK;
                    *dhcp_opt++ = DHCP_OPTION_END;

                    DEBUG_PRINTF("requested IP invalid, reply DHCP_NAK\n");
                    if (netif != NULL)
                    {
                        int send_byte = (dhcp_opt - (uint8_t *)msg);
                        _low_level_dhcp_send(netif, msg, send_byte);
                        DEBUG_PRINTF("DHCP server send %d byte\n", send_byte);
                    }
                    next_client_ip++;
                    if (next_client_ip > s_client_ip_max)
                        next_client_ip = s_client_ip_min;

					
                    continue;
                }
            }

            if (message_type == DHCP_DISCOVER)
            {
                DEBUG_PRINTF("request DHCP_DISCOVER\n");
                DEBUG_PRINTF("reply   DHCP_OFFER\n");


                // DHCP_OPTION_MESSAGE_TYPE
                *dhcp_opt++ = DHCP_OPTION_MESSAGE_TYPE;
                *dhcp_opt++ = DHCP_OPTION_MESSAGE_TYPE_LEN;
                *dhcp_opt++ = DHCP_OFFER;

                // DHCP_OPTION_SERVER_ID
                *dhcp_opt++ = DHCP_OPTION_SERVER_ID;
                *dhcp_opt++ = 4;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR0;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR1;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR2;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR3;

                // DHCP_OPTION_LEASE_TIME
                *dhcp_opt++ = DHCP_OPTION_LEASE_TIME;
                *dhcp_opt++ = 4;
				*dhcp_opt++ = (uint8_t)(s_leavetime>>24)&0xff;
				*dhcp_opt++ = (uint8_t)(s_leavetime>>16)&0xff;
				*dhcp_opt++ = (uint8_t)(s_leavetime>>8)&0xff;
				*dhcp_opt++ = (uint8_t)s_leavetime&0xff;

				
            }
            else if (message_type == DHCP_REQUEST)
            {
                DEBUG_PRINTF("request DHCP_REQUEST\n");
                DEBUG_PRINTF("reply   DHCP_ACK\n");
				if(s_ip_num >= s_client_ip_max - s_client_ip_min + 1)
				{
					continue;
				}
				for(i = 0; i < s_ip_num; i++)
				{
					if(!strncmp(s_ip_map[i].mac,mac,6))
					{
						next_client_ip = s_ip_map[i].ip;
						isGot = 1;
						break;
					}
				}
				if(!isGot)
				{
					memcpy(s_ip_map[s_ip_num].mac,mac,6);
					s_ip_map[s_ip_num].ip = ++next_client_ip;
					s_ip_num++;
				}

                // DHCP_OPTION_MESSAGE_TYPE
                *dhcp_opt++ = DHCP_OPTION_MESSAGE_TYPE;
                *dhcp_opt++ = DHCP_OPTION_MESSAGE_TYPE_LEN;
                *dhcp_opt++ = DHCP_ACK;

                // DHCP_OPTION_SERVER_ID
                *dhcp_opt++ = DHCP_OPTION_SERVER_ID;
                *dhcp_opt++ = 4;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR0;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR1;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR2;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR3;

                // DHCP_OPTION_SUBNET_MASK
                *dhcp_opt++ = DHCP_OPTION_SUBNET_MASK;
                *dhcp_opt++ = 4;
                *dhcp_opt++ = 0xFF;
                *dhcp_opt++ = 0xFF;
                *dhcp_opt++ = 0xFF;
                *dhcp_opt++ = 0x00;

                // DHCP_OPTION_ROUTER
                *dhcp_opt++ = DHCP_OPTION_ROUTER;
                *dhcp_opt++ = 4;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR0;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR1;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR2;
                *dhcp_opt++ = DHCPD_SERVER_IPADDR3;

                // DHCP_OPTION_DNS_SERVER, use the default DNS server address in lwIP
                *dhcp_opt++ = DHCP_OPTION_DNS_SERVER;
                *dhcp_opt++ = 4;
                *dhcp_opt++ = 208;
                *dhcp_opt++ = 67;
                *dhcp_opt++ = 222;
                *dhcp_opt++ = 222;

                // DHCP_OPTION_LEASE_TIME
                *dhcp_opt++ = DHCP_OPTION_LEASE_TIME;
                *dhcp_opt++ = 4;
				*dhcp_opt++ = (uint8_t)(s_leavetime>>24)&0xff;
				*dhcp_opt++ = (uint8_t)(s_leavetime>>16)&0xff;
				*dhcp_opt++ = (uint8_t)(s_leavetime>>8)&0xff;
				*dhcp_opt++ = (uint8_t)s_leavetime&0xff;
            }
            else
            {
                DEBUG_PRINTF("un handle message:%d\n", message_type);
            }

            // append DHCP_OPTION_END
            *dhcp_opt++ = DHCP_OPTION_END;

            /* send reply. */
            if ((message_type == DHCP_DISCOVER) || (message_type == DHCP_REQUEST))
            {
                msg->op = DHCP_BOOTREPLY;
                IP4_ADDR(&msg->yiaddr,
                         DHCPD_SERVER_IPADDR0, DHCPD_SERVER_IPADDR1,
                         DHCPD_SERVER_IPADDR2, next_client_ip);
				DEBUG_PRINTF("\n ip:%s \n",inet_ntoa(msg->yiaddr));
                client_addr.sin_addr.s_addr = INADDR_BROADCAST;

                if (netif != NULL)
                {
                    int send_byte = (dhcp_opt - (uint8_t *)msg);
                    _low_level_dhcp_send(netif, msg, send_byte);
                    DEBUG_PRINTF("DHCP server send %d byte\n", send_byte);
                }
            }
        } /* handler. */
    }
exit:
	if (sock >= 0)
		closesocket(sock);

	if (recv_data) {
		mem_free(recv_data);
		recv_data = NULL;
	}
#if defined(CONFIG_FREERTOS)
	xCreatedTask = NULL;
	vTaskDelete(NULL);
#else
	os_thread_exit();
	os_thread_yield();
#endif
}

void dhcpd_start(struct netif *netif)
{
	uint32_t ipaddr = DHCPD_SERVER_IPADDR0 << 24 | DHCPD_SERVER_IPADDR1 << 16 |
						DHCPD_SERVER_IPADDR2 << 8 | DHCPD_SERVER_IPADDR3;
	uint32_t netmask = 0xFFFFFF00UL;

	if (dhcpd_is_running)
		return;

	next_client_ip = s_client_ip_min;

	if (s_ip_map == NULL) {
		s_ip_map = (map_ip*)malloc(sizeof(map_ip)*(s_client_ip_max-s_client_ip_min +1));
		if (s_ip_map == NULL)
			return;
	}
	while (xCreatedTask != NULL)
		sys_msleep(10);

	xCreatedTask = (void *)sys_thread_new("dhcpd", dhcpd_thread_entry, netif,
						DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
	if (!xCreatedTask) {
		DEBUG_PRINTF("fail to init DHCP server\n");
		return;
	}
	serial_printf("DHCP daemon is up\n");

	dhcpd_sig = DHCPD_INIT;
	dhcpd_is_running = 1;
}

void dhcpd_stop(struct netif *netif)
{
	if (dhcpd_is_running == 0)
		return;

	if (s_ip_map != NULL) {
		free(s_ip_map);
		s_ip_map = NULL;
	}
	s_ip_num = 0;
	dhcpd_sig = DHCPD_DOWN;
	dhcpd_is_running = 0;
}

void set_dhcp_ip_range(uint8_t min,uint8_t max)
{
	s_client_ip_max = max;
	s_client_ip_min = min;
	next_client_ip = s_client_ip_min;
}

void set_dhcp_leavetime(int minutes)
{
	s_leavetime = minutes*60;
}

void dhcpd_get_client_ip(char *mac, unsigned int *ip)
{
	int i;
	for (i=0; i<s_ip_num; i++) {
		if (memcmp(s_ip_map[i].mac,mac,6))
			continue;
		*ip = DHCPD_SERVER_IPADDR0 << 24 | DHCPD_SERVER_IPADDR1 << 16 |
				DHCPD_SERVER_IPADDR2 << 8 | s_ip_map[i].ip;
		break;
	}
}
