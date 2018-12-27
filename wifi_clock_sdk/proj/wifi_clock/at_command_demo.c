/*=============================================================================+
|                                                                              |
| Copyright 2015                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file at_command_demo.c
*   \brief main entry
*   \author Montage
*/

/*=============================================================================+
| Included Files                                                               |
+=============================================================================*/
#include <common.h>
#include <flash_api.h>
#include "lwip/sockets.h"

#include <gpio.h>
#include <serial.h>
#include <event.h>
#include <os_api.h>
#include <wla_api.h>
#include <gpio.h>
#include <version.h>
#include <built_info.h>
#include <cfg_api.h>

#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#if defined(CONFIG_LWIP)
#include <net_api.h>
#endif
#ifdef CONFIG_OMNICONFIG
#include <omniconfig.h>
//#include "/home/jerry/Desktop/AndroidM_SDK_S912/iot/lib/lwip/apps/ntp/ntpd.h"


extern char omni_ipaddr[64];
extern char omni_mac_str[16];
// ONIMCONFIG statue
extern int state;
#endif
#define TEST_UART   0//Lynx UART1

#undef printf
#define printf(fmt,args...)		serial_printf(fmt ,##args)
#define UART_RECV_TIMEOUT                   0
#define UART_ONE_PACKAGE_LENGTH             1024
#define UART_BAUD_RATE                      115200

static network_info apcfg;
static network_info_adv stacfgadv;

static int led_sts = 0;
const char *sw_build_time = (char *)SW_BUILD_TIME;
const char *sw_build_sdk = (char *)MT_SDK_VER_STR;
const int sw_build_count = SW_BUILD_COUNT;

extern sdk_param g_atParam;
#define UART_BR_TB 16
extern unsigned int br_tb[UART_BR_TB];
static char s_linkup = 0;
extern void (*sta_start_func)(char *, char *,unsigned char *);
extern void (*ap_start_func)(char *, char *, int);

/*----------------------------------------------------------------*/
/**
 * The function is called once application start-up. Users can initial
 * structures or global parameters here.
 *
 * @param None.
 * @return int Protothread state.
 */
/*----------------------------------------------------------------*/
int app_main(void)
{
#ifdef CONFIG_WLA_LED
	pin_mode(WIFI_LED_PIN, 1);
	digital_write(WIFI_LED_PIN, 0);
#endif
	/* Do not add any process here, user should add process in user_thread */
	hw_sys_init();
	return PT_EXITED;
}

void uart_init(void)
{
	int id;
	int baud = UART_BAUD_RATE;
	int parity = 0;
	int stop_bits = 1;
	int threshold = 0;
	for (id = UART_BR_TB -1; id >= 0; id--) {
		if (br_tb[id] > baud)
			continue;
		break;
	}
	if (id < 0) id = 0;
	serial_conf(id, parity, stop_bits, TEST_UART,threshold );
	serial_init((int)TEST_UART);

#ifdef CONFIG_UR_FLOW_CONTROL
	pin_mode(CONFIG_UR_CTS, 0);
	digital_write(CONFIG_UR_RTS, 0);//RTS active
	pin_en_intr(CONFIG_UR_CTS, 0, (gpio_func_cb)uart_flow_cts_stat, NULL);//0: rising
	pin_en_intr(CONFIG_UR_CTS, 1, (gpio_func_cb)uart_flow_cts_stat, NULL);//1: falling
	pin_mode(CONFIG_UR_RTS, 1);
#endif	
}

static void notify_wifi_scanHandler(scan_result *pApList)
{
	struct _aplist_adv *ap_adv;
	int i = 0;
	char buf[33];

	at_printf("Station Scan Done\n");

	if (pApList == NULL)
		return;

	ap_adv = (struct _aplist_adv *)pApList->aplist;
	for (i = 0; i < pApList->ap_num; i++) {
		memset(buf, 0, sizeof(buf));
		memcpy(buf, ap_adv->ssid, sizeof(ap_adv->ssid));
		at_printf("\n");
		at_printf("<ecn>:%d ", ap_adv->security);
		at_printf("<ssid>:%s ", buf);
		at_printf("<rssi>:%d ", ap_adv->power);
		at_printf("<mac>:%s ", ether_ntoa(ap_adv->bssid));
		at_printf("<ch>:%d ", ap_adv->channel);
		at_printf("\n");
		ap_adv++;
	}
}

static void notify_DNS_handler(char *str, int len)
{
	at_printf("[%s]\n", str);
}

static void notify_wifi_status_handler(int status)
{
	switch (status) {
	case NOTIFY_STA_LINK_UP:
		s_linkup = 1;
		printf("Station up\n");
#if defined(CONFIG_LWIP)
		net_if_up(NETIF_STA, stacfgadv.dhcp_mode, wlan_get_myaddr(STATION),
				stacfgadv.local_ip_addr, stacfgadv.net_mask,
				stacfgadv.gateway_ip_addr, stacfgadv.dnssvr_ip_addr);
#endif
		break;
	case NOTIFY_STA_LINK_DOWN:
		s_linkup = 0;
		printf("Station down\n");
#if defined(CONFIG_LWIP)
		net_if_down(NETIF_STA);
#endif
		break;
	case NOTIFY_AP_LINK_UP:
		printf("AP up\n");
#if defined(CONFIG_LWIP)
		net_if_up(NETIF_AP, apcfg.dhcp_mode, wlan_get_myaddr(SOFT_AP),
				apcfg.local_ip_addr, apcfg.net_mask,
				apcfg.gateway_ip_addr, apcfg.dnssvr_ip_addr);
#endif
		break;
	case NOTIFY_AP_LINK_DOWN:
		printf("AP down\n");
#if defined(CONFIG_LWIP)
		net_if_down(NETIF_AP);
#endif
		break;
	}
}

static void notify_dhcp_handler(ip_sts *pnet)
{
	printf("IP=%s Gate=%s Mask=%s\r\nDNS=%s MAC=%s Bcast=%s\n",
				pnet->ip, pnet->gate, pnet->mask,
				pnet->dns, pnet->mac, pnet->broadcastip);
}

void sta_start(char *ssid,char *pwd,unsigned char *bssid)
{
	printf("SSID=%s,PASS=%s,\n", ssid, pwd);
	/* Initialize station parameteres */
	memset(&stacfgadv, 0x0, sizeof(stacfgadv));
	strcpy(stacfgadv.ap_info.ssid, ssid);		/* wlan ssid string */
	/*
		WEP key length:			ASCII=5(64 bits) or 13(128 bits)
								HEX=10(64 bits) or 26(128 bits)
		WPA/WPA2 key length:	ASCII=8-63, HEX=64
	 */
	strcpy(stacfgadv.key, pwd);					/* wlan key string or hex data */
	stacfgadv.key_len = strlen(pwd);
	stacfgadv.dhcp_mode = DHCP_DISABLE;			/* Fetch Ip address from DHCP server */
	stacfgadv.ap_info.security = SECURITY_AUTO;	/* Wlan security mode */
	stacfgadv.ap_info.channel = 0;				/* Select chanel automatically */
	if ((g_atParam.dhcp_mode == 2) || (g_atParam.dhcp_mode == 3))
		stacfgadv.dhcp_mode = DHCP_CLIENT;
	strcpy((char *)stacfgadv.local_ip_addr, g_atParam.sta_param.ip);
	strcpy((char *)stacfgadv.net_mask, g_atParam.sta_param.mask);
	strcpy((char *)stacfgadv.gateway_ip_addr, g_atParam.sta_param.gw);
	strcpy((char *)stacfgadv.dnssvr_ip_addr, "8.8.8.8");
	if (bssid != NULL)
		strcpy(stacfgadv.ap_info.bssid, bssid); /* wlan bssid*/
	stacfgadv.wifi_retry_interval = 100;		/* Retry interval after a failure connection */
	wlan_set_reconnect((g_atParam.flags & CFG_RECONN) ? 1 : 0);
	wlan_set_myaddr(STATION, g_atParam.mac.sta_mac);
	/* Connect Now! */
	wlan_start_adv(&stacfgadv);
}

void ap_start(char *ssid, char *pwd, int ch)
{
	printf("SSID=%s,PASS=%s,CH=%d\n", ssid, pwd, ch);
	/* Initialize AP parameteres */
	/* Setup channel number */
	wlan_set_channel(ch);
	memset(&apcfg, 0x0, sizeof(apcfg));
	strcpy(apcfg.wifi_ssid, ssid);
	//memcpy(apcfg.wifi_ssid, "12345678901234567890123456789012", sizeof(apcfg.wifi_ssid));
	/*
		WEP key length: 		ASCII=5(64 bits) or 13(128 bits)
								HEX=10(64 bits) or 26(128 bits)
		WPA/WPA2 key length:	ASCII=8-63, HEX=64
	 */
	
	strcpy((char *)apcfg.wifi_key, pwd);
	apcfg.wifi_mode = SOFT_AP;
	apcfg.dhcp_mode = DHCP_DISABLE;
	if ((g_atParam.dhcp_mode == 1) || (g_atParam.dhcp_mode == 3))
		apcfg.dhcp_mode = DHCP_SERVER;

	strcpy((char *)apcfg.local_ip_addr, g_atParam.ap_param.ip);
	strcpy((char *)apcfg.net_mask, g_atParam.ap_param.mask);
	strcpy((char *)apcfg.gateway_ip_addr, g_atParam.ap_param.gw);
	strcpy((char *)apcfg.dnssvr_ip_addr, "8.8.8.8");
	wlan_set_hidden_ssid(0);
	if (g_atParam.ap_param.hidden_ssid)
		wlan_set_hidden_ssid(1);
	apcfg.wifi_retry_interval = 100;
	wlan_set_myaddr(SOFT_AP, g_atParam.mac.ap_mac);
	wlan_start(&apcfg);
}


/*----------------------------------------------------------------*/
/**
 * The function handle status and flashes Wi-Fi LED.
 *
 * @param None.
 * @return int Millisecond to sleep.
 */
/*----------------------------------------------------------------*/
int user_led_handler(void)
{
	int timeout, gpio_mode, sm;

	timeout = 1000;
	gpio_mode = 1;

	if (sm = wlan_get_ifs_sm(STATION)) {
		switch (sm) {
		case STATE_SCAN:
		case STATE_SCAN_DONE:
		case STATE_LINK_DOWN:
			s_linkup = 0;
			pin_mode(WIFI_LED_PIN, 1);
			led_sts = !led_sts;
			timeout = 250;
			break;
		case STATE_LINK_UP:
			s_linkup = 1;
			gpio_enable(WIFI_LED_PIN, 0);
			gpio_mode = 0;
			break;
		}
	} else if ((sm = wlan_get_ifs_sm(SOFT_AP)) &&
				wlan_get_sta_num(SOFT_AP)) {
		pin_mode(WIFI_LED_PIN, 1);
		led_sts = !led_sts;
	} else {
		pin_mode(WIFI_LED_PIN, 1);
		led_sts = 0;
	}
	if (gpio_mode)
		digital_write(WIFI_LED_PIN, led_sts);
	return timeout;
}

int user_thread(void *arg )
{
	int recvlen;
	int i = 0;

	config_load();
#ifdef CONFIG_LWIP
	tcpip_init(0, 0);
	net_init_notification();
#endif
	wlan_init_notification();
	wlan_init();

	at_cmd_init();
	uart_init( );
	
	sta_start_func = sta_start;
	ap_start_func = ap_start;
	wlan_add_notification(NOTIFY_WIFI_SCAN_COMPLETED, notify_wifi_scanHandler);
	wlan_add_notification(NOTIFY_WIFI_STATUS_CHANGED, notify_wifi_status_handler);
	wlan_add_notification(NOTIFY_WIFI_LED_TIMEOUT, user_led_handler);
#if defined(CONFIG_LWIP)
	net_add_notification(NOTIFY_DHCP_COMPLETED, notify_dhcp_handler);
	net_add_notification(NOTIFY_DNS_RESOLVE_COMPLETED, notify_DNS_handler);
#endif

	wlan_led_install();
#if 1
	config_get(&g_atParam);

	if (g_atParam.flags & CFG_AUTO_CONN) {
		if (g_atParam.work_mode != OPMODE_AP) {
			printf("\r\n===============starting auto connect========\r\n");
			if (sta_start_func)
				sta_start_func(g_atParam.sta_param.ssid,
								g_atParam.sta_param.key,NULL);
		}
		if (g_atParam.work_mode != OPMODE_STA) {
			if (ap_start_func)
				ap_start_func(g_atParam.ap_param.ssid,
								g_atParam.ap_param.key,
								g_atParam.ap_param.channel);
		}
	}
#ifdef CONFIG_LWIP
	if (g_atParam.ts_param.enable) {
		printf("g_atParam.ts_param.enable 1\n");
		while (s_linkup == 0) {
			sys_msleep(20);
			i++;
			if (i > 1000) {
				printf("wait to connect the net\n");
				break;
			}
		}
		if (s_linkup == 1) {
			char str[6];
			sprintf(str, "%d", g_atParam.ts_param.port);
			char *argv[3] = {g_atParam.ts_param.type,g_atParam.ts_param.ip,str};
			net_socket_mutiConn(0);
			if (net_socket_add(3, argv) == 0) {
				char *argv1[1] = {"1"};
				tcp_rx_start();
				at_trans_mode(1,argv1,"=");
				start_trans_mode();
			}
		}
	}
#endif
	serial_printf("starting omni connect \n");
	/* waiting connecting 3 secs*/
	printf("waiting connecting \n");
	sys_msleep(10000);
	if(!s_linkup){
		omnicfg_start(360);
	}	
	xTaskCreate(get_ntp_time,"getNtpTime",4096,0,4,NULL);

	while (1)
	{
#ifdef CONFIG_OMNICONFIG
		int try_time = 30;
		if (state == OMNI_STATE_CONNECTING) {
			do {
				/*TODO: Ping Test*/
				printf("-------------------------------------ip=%s,mac=%s\n",
						omni_ipaddr,omni_mac_str);
				omnicfg_broadcast(OMNI_STATE_CONNECT_SUCCESS,omni_ipaddr,omni_mac_str,1);
				try_time--;
				sys_msleep(100);
			} while (try_time > 0);
			state = OMNI_STATE_CONNECT_SUCCESS;
		}
		
#endif
		sys_msleep(300);
	}
#endif
exit:
    vTaskDelete(NULL);

}

