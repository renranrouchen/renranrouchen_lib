/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file net_api.c
*   \brief  net API functions.
*   \author Montage
*/

/*=============================================================================+
| Included Files
+=============================================================================*/
#include <common.h>
#include <lynx_dev.h>
#include <wbuf.h>
#include <netif/etharp.h>
#include <lwip/tcpip.h>
#include <lwip/netif.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <net_api.h>
#include <wla_api.h>
#include <socket_api.h>

void net_drv_init(void)  __attribute__((section(".init")));

void (*notify_dhcp_complete)(void *) = NULL;
void (*notify_dns_resolve_completed)(char *, int) = NULL;
int (*net_drv_recv_cb)(int, unsigned short) = NULL;

unsigned char iot_dhcpd_addr[4];

#define IFNAME0 'w'
#define IFNAME1 'l'

struct net_dev *idev;
struct net_dev idev_g;

unsigned int ping_dip;
unsigned int ping_data_size = 0;
unsigned int ping_rcv_timeo = 3000;
unsigned int ping_iteration = 0;
unsigned int ping_interval = 1000;
unsigned int ping_tx = 0;
unsigned int ping_rx = 0;

#define NET_TO_IDX(_t)		((struct netif *)_t - ((struct netif *)idev->netif))
/*----------------------------------------------------------------*/
/**
 * The function uses DMA copy from pbuf to wbuf and invokes Wi-Fi
 * driver to send data packets.
 *
 * @param netif The netif.
 * @param p The data buffer.
 * @return err_t error code.
 */
/*----------------------------------------------------------------*/
static err_t net_drv_send(struct netif *netif, struct pbuf *p)
{
	struct hrx_frame_info *cur_hrx = &ldev->cur_hrx;
	buf_header *bhdr=NULL;
	int bhdr_idx;
	u32 len, plen;
	u8 offset;
	u8 *src, *dst, *buffp=NULL;

#if 0
	WLA_DUMP_BUF("tx pbuf:", p->payload, p->tot_len);
#endif
	if ((netif->flags & NETIF_FLAG_LINK_UP) == 0)
		goto fail;

	plen = p->tot_len;
	offset = sizeof(struct wtbuf);
	len = plen + offset;		/* reserved for Wi-Fi driver */
	buffp = dst = (u8 *)malloc(len);
	if (!dst) {
		serial_printf("run out buf\n");
		goto fail;
	}
	if ((bhdr = (buf_header *)bhdr_get_first(1)) == NULL) {
		serial_printf("run out bhdr, freecnt=%d\n", ldev->wmac.extra_bhdr_count);
		goto fail;
	}
	bhdr->bh_w0 = BH_EP | ((offset >> 6) << BH_SHIFT_OFFSETH) | (len & BH_LEN);
	bhdr->bh_w1 = ((offset & 0x3f) << BH_SHIFT_OFFSET) | (int)buffp;
	bhdr_idx = bhdr_to_idx(bhdr);
#ifdef CONFIG_WLA_DEBUG_SWBUF
	wmac_swbuf_update(bhdr_idx, 0x57);		// 'W'
#endif
	dst += offset;
	while (p) {
		src = p->payload;
		dma_copy((void *)dst, (void *)src, p->len, 0);
		dst += p->len;
		p = p->next;
	}
#if 0
	WLA_DUMP_BUF("buf:", DPTR_TO_WB(BHDR_DPTR(bhdr)), len);
	serial_printf("w0=0x%x w1=0x%x\n", bhdr->bh_w0, bhdr->bh_w1);
#endif
	cur_hrx->h2d_cmd.cmd_id = NET_TO_IDX(netif);
	cur_hrx->tail = bhdr_idx;
	cur_hrx->head = bhdr_idx;
	cur_hrx->h2d_cmd.len = plen;
	if (ldev->host_event_handler &&
		(ldev->host_event_handler() >= 0))
		return ERR_OK;
fail:
	if (buffp)
		free(buffp);
	if (bhdr)
		bhdr_insert_tail(bhdr_idx);
	return ERR_OK;
}

/*----------------------------------------------------------------*/
/**
 * The function uses DMA copy from wbuf to pbuf and invokes netif
 * input callback to receive data packets.
 *
 * @param idx The netif index.
 * @param head The wbuf head index.
 * @return 1 free wbuf.
 */
/*----------------------------------------------------------------*/
static int net_drv_recv(int idx, unsigned short head)
{
	struct netif *netif = &idev->netif[idx];
	struct wbuf *wb = DPTR_TO_WB(BHDR_DPTR(idx_to_bhdr(head)));
	int len = WB_PKTLEN(wb);
	struct pbuf *p;

	if (netif->input == 0)
		goto drop_it;
	/* copy to p buffer */
	p = pbuf_alloc(PBUF_RAW, len, PBUF_RAM);
	if (!p) {
		serial_printf("run out pbuf\n");
	//	  struct pbuf *q = (struct pbuf *)p;
  	//		pbuf_free(q);
		goto drop_it;
	}

	bhdr_dma_copy(head, p->payload, WB_DATAOFF(wb), len);
	if (netif->input(p, netif) < 0)
		pbuf_free(p);
drop_it:
	// free buffer
	//pbuf_free(p);
	
	return 1;
}

/*----------------------------------------------------------------*/
/**
 * The function gets netif link status.
 *
 * @param idx The netif index.
 * @return 1: link up\n 0: link down
 */
/*----------------------------------------------------------------*/
static int net_is_link_up(int idx)
{
	struct netif *netif;

	netif = &idev->netif[idx];
	return netif_is_link_up(netif);
}

/*----------------------------------------------------------------*/
/**
 * The function gets netif interface status.
 *
 * @param idx The netif index.
 * @return 1: interface up\n 0: interface down
 */
/*----------------------------------------------------------------*/
static int net_is_if_up(int idx)
{
	struct netif *netif;

	netif = &idev->netif[idx];
	return netif_is_up(netif);
}

/*----------------------------------------------------------------*/
/**
 * The function gets netif DHCP status.
 *
 * @param idx The netif index.
 * @return 1: enabled\n 0: disabled
 */
/*----------------------------------------------------------------*/
static int net_is_dhcp(int idx)
{
	struct netif *netif;

	netif = &idev->netif[idx];
	return (netif->flags & NETIF_FLAG_DHCP) ? 1 : 0;
}

/*----------------------------------------------------------------*/
/**
 * The function gets netif by netif name.
 *
 * @param name The netif name.
 * @return netif.
 */
/*----------------------------------------------------------------*/
static struct netif *net_if_find(char *name)
{
	struct netif *netif;
	u8_t num;

	if (name == NULL) {
		return NULL;
	}

	num = name[2];

	for (netif = netif_list; netif != NULL; netif = netif->next) {
		if (num == netif->num &&
		   name[0] == netif->name[0] &&
		   name[1] == netif->name[1]) {
		  return netif;
		}
	}
	return NULL;
}

/*----------------------------------------------------------------*/
/**
 * The function sets DHCP server or DHCP client by netif index.
 * It sets DHCP client for netif index 0 or sets DHCP server
 * for netif index 1.
 *
 * @param idx The netif index.
 * @param en 1: enable\n 0: disable
 * @param mode:\n 0: disable DHCP\n 1: DHCP client\n 2: DHCP server
 * @return None.
 */
/*----------------------------------------------------------------*/
static void net_set_dhcp(int idx, int en, int mode)
{
	struct netif *netif;
	int stop_dhcp = 0, start_dhcp = 0;

	if (mode == 0)
		return;

	netif = &idev->netif[idx];
	if (!net_if_find(netif->name))
		return;

	if (en) {
		if (netif->flags & NETIF_FLAG_DHCP)
			stop_dhcp  = 1;
		start_dhcp = 1;
	} else {
		if ((netif->flags & NETIF_FLAG_DHCP) == 0)
			return;
		stop_dhcp  = 1;
	}
	if (mode == 2) {
#ifdef CONFIG_DHCPS
		dhcpd_stop(netif);
		netif->flags &= ~NETIF_FLAG_DHCP;
		if (start_dhcp) {
			*(unsigned int*)iot_dhcpd_addr = *(unsigned int *)&netif->ip_addr;
			dhcpd_start(netif);
			netif->flags |= NETIF_FLAG_DHCP;
		}
#endif
	} else if (mode == 1) {
#ifdef CONFIG_DHCPC
		if (stop_dhcp) {
			netifapi_dhcp_stop(netif);
			dhcp_cleanup(netif);
		}
		if (start_dhcp)
			netifapi_dhcp_start(netif);
#endif
	}
}

/*----------------------------------------------------------------*/
/**
 * The function dumps ping statistics.
 *
 * @param None.
 * @return None.
 */
/*----------------------------------------------------------------*/
static void net_ping_stat(void)
{
	if (ping_iteration) {
		add_timeout((FUNCPTR)net_ping_stat, 0, 1000);
	} else {
		serial_printf("tx=%d rx=%d\n", ping_tx, ping_rx);
	}
}

/*----------------------------------------------------------------*/
/**
 * The function ping destination address.
 *
 * @param dip Destination address.
 * @param size Packet size.
 * @param iter Iteration.
 * @param to Timeout(seconds).
 * @param interval Packet interval(milliseconds).
 * @return None.
 */
/*----------------------------------------------------------------*/
void net_ping(unsigned int dip, unsigned int *size,
				unsigned int *iter, unsigned int *to, unsigned int *interval)
{
	if (*size <= 0)
		*size = 32;
	if (*iter <= 0)
		*iter = 4;
	if (*to <= 0)
		*to = 3;
	if (*interval <= 0)
		*interval = 10;
	ping_dip = dip;
	ping_data_size = *size;
	ping_rcv_timeo = (*to)*1000;
	ping_iteration = *iter;
	ping_interval = (*interval)*100;
	ping_tx = ping_rx = 0;
	del_timeout((FUNCPTR)net_ping_stat, 0);
	add_timeout((FUNCPTR)net_ping_stat, 0, 1000);
}

/*----------------------------------------------------------------*/
/**
 * The function sets DNS server by server index.
 *
 * @param idx The DNS server index.
 * @param ipaddr IP address.
 * @return None.
 */
/*----------------------------------------------------------------*/
void net_set_dns(int idx, unsigned int *ipaddr)
{
#ifdef CONFIG_DNS
	dns_setserver(idx, (ip_addr_t *)ipaddr);
#endif
}

/*----------------------------------------------------------------*/
/**
 * The function gets DNS server's IP address.
 *
 * @param idx The DNS server index.
 * @param ipaddr IP address.
 * @return IP string of DNS server.
 */
/*----------------------------------------------------------------*/
char *net_get_dns(int idx, unsigned int *ipaddr)
{
#ifdef CONFIG_DNS
	*ipaddr = (unsigned int)dns_getserver(idx);
#endif
	return inet_ntoa(*ipaddr);
}

/*----------------------------------------------------------------*/
/**
 * The function gets client information.
 *
 * @param mac The client's address.
 * @return err_t error code
 */
/*----------------------------------------------------------------*/
int net_get_client_info(const char *mac)
{
	unsigned int ip = 0;

#ifdef CONFIG_DHCPS
	dhcpd_get_client_ip(mac, &ip);
#endif
	serial_printf("(%s) at ", inet_ntoa(ip));
	serial_printf("%s [ether]\n", ether_ntoa(mac));
	return ERR_OK;
}

/*----------------------------------------------------------------*/
/**
 * The function gets netif name.
 *
 * @param idx The netif index.
 * @return Name string of netif.
 */
/*----------------------------------------------------------------*/
char *net_get_name(int idx)
{
	struct netif *netif;
	int len;

	netif = &idev->netif[idx];
	if (!net_if_find(netif->name))
		return NULL;
	len = sizeof(netif->name);
	memcpy(idev->buf, netif->name, len);
	idev->buf[len] = '\0';
	return idev->buf;
}

/*----------------------------------------------------------------*/
/**
 * The function queries host IP address by hostname. It also dumps
 * host's IP address.
 *
 * @param name The hostname.
 * @param ipaddr The host IP address.
 * @return 1: succeed\n 0: failed
 */
/*----------------------------------------------------------------*/
int net_get_hostbyname(const char *name, char *ipaddr)
{
#ifdef CONFIG_DNS
	struct hostent *hptr;
	char *addr;

	if ((hptr = gethostbyname(name)) == NULL)
		return 0;

	switch (hptr->h_addrtype) {
	case AF_INET:
		addr = inet_ntoa(*(unsigned int *)hptr->h_addr);
		memcpy(ipaddr, addr, 16);
		if (notify_dns_resolve_completed)
			notify_dns_resolve_completed(ipaddr, 16);
		break;
	default:
		return 0;
		break;
	}
#endif
	return 1;
}

/*----------------------------------------------------------------*/
/**
 * The function initials netif and ether output callback.
 *
 * @param netif The netif.
 * @return err_t error code
 */
/*----------------------------------------------------------------*/
static err_t net_if_init(struct netif *netif)
{
	netif->hwaddr_len = 6;
	netif->output = etharp_output;
	netif->linkoutput = net_drv_send;
	netif->mtu = 1500;
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_IGMP;
	return ERR_OK;
}

/*----------------------------------------------------------------*/
/**
 * The function reads current IP status on a network interface.
 *
 * @param data: Point to the buffer to store the IP address.
 * @param type: Specifies wlan interface.
 * @return None.
 */
/*----------------------------------------------------------------*/
void net_if_ip_sts(void *data, int type)
{
	ip_sts *sts = (ip_sts *)data;
	int dhcp, idx;
	struct netif *netif;
	unsigned int dns_svr;

	if (type == STATION)
		idx = NETIF_STA;
	else
		idx = NETIF_AP;
	netif = &idev->netif[idx];

	memset(sts, 0, sizeof(ip_sts));
	dhcp = net_is_dhcp(idx);
	sts->dhcp = DHCP_DISABLE;
	if (dhcp) {
		if (idx == NETIF_STA)
			sts->dhcp = DHCP_CLIENT;
		else
			sts->dhcp = DHCP_SERVER;
	}
	strcpy(sts->ip, inet_ntoa(*(unsigned int *)&netif->ip_addr));
	strcpy(sts->mask, inet_ntoa(*(unsigned int *)&netif->netmask));
	strcpy(sts->gate, inet_ntoa(*(unsigned int *)&netif->gw));
	strcpy(sts->dns, net_get_dns(idx, &dns_svr));
	sprintf(sts->mac, "%02x%02x%02x%02x%02x%02x",
				netif->hwaddr[0], netif->hwaddr[1],
				netif->hwaddr[2], netif->hwaddr[3],
				netif->hwaddr[4], netif->hwaddr[5]);
	strcpy(sts->broadcastip, "255.255.255.255");
}

/*----------------------------------------------------------------*/
/**
 * The function invokes net_if_ip_sts to read current IP status on
 * a network interface. It's a callback when netif status is changed.
 *
 * @param netif The netif.
 * @return None.
 */
/*----------------------------------------------------------------*/
static void net_if_handler(struct netif *netif)
{
	int idx;
	ip_sts sts;
	int type;

	idx = NET_TO_IDX(netif);
	if (idx == NETIF_STA)
		type = STATION;
	else
		type = SOFT_AP;
	if (net_is_if_up(idx)) {
		net_if_ip_sts((void *)&sts, type);
		tcpip_refresh_port();
		if (notify_dhcp_complete)
			notify_dhcp_complete((void *)&sts);
	}
}

/*----------------------------------------------------------------*/
/**
 * The function sets link down and interface down and deletes netif
 * from netif list.
 *
 * @param idx The netif index.
 * @return None.
 */
/*----------------------------------------------------------------*/
static void net_del(int idx)
{
	struct netif *netif;

	netif = &idev->netif[idx];
	if (!net_if_find(netif->name))
		return;

	netif_set_link_callback(netif, 0);
	netif_set_status_callback(netif, 0);

	netif_set_link_down(netif);
	netifapi_netif_set_down(netif);
	netifapi_netif_remove(netif);
}

/*----------------------------------------------------------------*/
/**
 * The function adds netif to netif list and invokes link and
 * interface callback. It sets the netif as default route if
 * def_rt is true.
 *
 * @param idx The netif index.
 * @param link_callback The link callback function.
 * @param status_callback The interface callback function.
 * @param ip IP address.
 * @param mask Net mask.
 * @param gw Gateway address.
 * @param def_rt Is a default route.
 * @return None.
 */
/*----------------------------------------------------------------*/
static void net_add(int idx,
				unsigned char *ip,
				unsigned char *mask,
				unsigned char *gw,
				int def_rt)
{
	struct ip_addr ipaddr, netmask, gateway;
	struct netif *netif;

	netif = &idev->netif[idx];
	if (net_if_find(netif->name))
		return;
	
	IP4_ADDR(&gateway, gw[0],gw[1],gw[2],gw[3]);
	IP4_ADDR(&ipaddr, ip[0],ip[1],ip[2],ip[3]);
	IP4_ADDR(&netmask, mask[0],mask[1],mask[2],mask[3]);
	netifapi_netif_add(netif, &ipaddr, &netmask, &gateway, netif->state,
				net_if_init, tcpip_input);
	netif_set_link_callback(netif, (netif_status_callback_fn)NULL);
	netif_set_status_callback(netif, (netif_status_callback_fn)net_if_handler);

	if (def_rt)
		netifapi_netif_set_default(netif);
}

/*----------------------------------------------------------------*/
/**
 * The function initials LWIP device and all of netifs name.
 *
 * @param None.
 * @return None.
 */
/*----------------------------------------------------------------*/
void net_drv_init(void)
{
	struct netif *netif;
	int i;

	idev = &idev_g;
	memset(idev, 0, sizeof(idev_g));

	for (i=0; i<NET_MAX_NETIF; i++) {
		netif = &idev->netif[i];
		netif->name[0] = IFNAME0;
		netif->name[1] = 0x30+i;
	}
#ifdef CONFIG_SOCKET_API
	for (i=0; i<MEMP_NUM_NETCONN; i++)
		idev->s[i] = -1;
#endif
	net_drv_recv_cb = net_drv_recv;
}

/*----------------------------------------------------------------*/
/**
 * The function initializes notification center.
 *
 * @param None.
 * @return NO_ERR.
 */
/*----------------------------------------------------------------*/
int net_init_notification(void)
{
	notify_dhcp_complete = 0;
	notify_dns_resolve_completed = 0;
	return NO_ERR;
}

/*----------------------------------------------------------------*/
/**
 * The function register notification and it's callback function.
 *
 * @param type system defined notifications.
 * @param functionAddress callback function.
 * @return NO_ERR.
 */
/*----------------------------------------------------------------*/
int net_add_notification(int type, void *functionAddress)
{
	switch (type) {
	case NOTIFY_DHCP_COMPLETED:
		notify_dhcp_complete = functionAddress;
		break;
	case NOTIFY_DNS_RESOLVE_COMPLETED:
		notify_dns_resolve_completed = functionAddress;
		break;
	}
	return NO_ERR;
}

/*----------------------------------------------------------------*/
/**
 * The function unregister notification and it's callback function.
 *
 * @param type system defined notifications.
 * @return NO_ERR.
 */
/*----------------------------------------------------------------*/
int net_del_notification(int type)
{
	switch (type) {
	case NOTIFY_DHCP_COMPLETED:
		notify_dhcp_complete = 0;
		break;
	case NOTIFY_DNS_RESOLVE_COMPLETED:
		notify_dns_resolve_completed = 0;
		break;
	}
	return NO_ERR;
}

/*----------------------------------------------------------------*/
/**
 * The function unregister all notification and callback functions.
 *
 * @param type system defined notifications.
 * @return NO_ERR.
 */
/*----------------------------------------------------------------*/
int net_del_notification_all(int type)
{
	net_init_notification();
	return NO_ERR;
}

/*----------------------------------------------------------------*/
/**
 * The function sets interface up and network configurations.
 *
 * @param idx The netif index.
 * @param dhcp DHCP mode.
 * @param mac MAC address.
 * @param _ip IP address.
 * @param _mask Net mask.
 * @param _gw Gateway address.
 * @param _dns DNS server address.
 * @return None.
 */
/*----------------------------------------------------------------*/
void net_if_up(int idx, unsigned char dhcp, char *mac,
				unsigned char *_ip, unsigned char *_mask,
				unsigned char *_gw, unsigned char *_dns)
{
	unsigned int ip, mask, gw, dns_svr, def_rt;
	struct netif *netif;
	//printf("net_if_up_step1--------------->\r\n");
	if ((idx == NETIF_STA) && (dhcp == DHCP_SERVER))
		return;
	if ((idx == NETIF_AP) && (dhcp == DHCP_CLIENT))
		return;
	netif = &idev->netif[idx];
	if (net_if_find(netif->name))
		return;
	//printf("net_if_up_step2--------------->\r\n");
	def_rt = 1;
	memcpy(netif->hwaddr, mac, NETIF_MAX_HWADDR_LEN);
	inet_aton(_ip, &ip);
	inet_aton(_mask, &mask);
	inet_aton(_gw, &gw);
	inet_aton(_dns, &dns_svr);
	if ((idx == NETIF_STA) && dhcp) {
		ip = 0;
		mask = 0;
		gw = 0;
	}
	net_add(idx, (u8 *)&ip, (u8 *)&mask, (u8 *)&gw, def_rt);//reboot?????
	net_set_dns(idx, &dns_svr);
	netif_set_link_up(netif);
	if (idx == NETIF_STA) {
		if (dhcp)
			net_set_dhcp(idx, 1, DHCP_CLIENT);
		else
			netifapi_netif_set_up(netif);
	} else {
		netifapi_netif_set_up(netif);
		if (dhcp)
			net_set_dhcp(idx, 1, DHCP_SERVER);
	}
}

/*----------------------------------------------------------------*/
/**
 * The function sets interface down.
 *
 * @param idx The netif index.
 * @return None.
 */
/*----------------------------------------------------------------*/
void net_if_down(int idx)
{
	struct netif *netif;

	netif = &idev->netif[idx];
	if (!net_if_find(netif->name))
		return;

	if (idx == NETIF_STA)
		net_set_dhcp(idx, 0, DHCP_CLIENT);
	else
		net_set_dhcp(idx, 0, DHCP_SERVER);
	net_del(idx);
}

