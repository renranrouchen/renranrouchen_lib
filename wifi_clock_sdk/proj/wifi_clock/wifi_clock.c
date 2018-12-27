/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      wifi_clock.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-24 11:34:37
 * ModifiedTime:  2017-06-16 09:27:45
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
#include <alarm.h>

#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#if defined(CONFIG_LWIP)
#include <net_api.h>
#endif
#ifdef CONFIG_OMNICONFIG
#include <omniconfig.h>
/* smart link config use for broadcast message to OMNICONFIG APP */
extern char omni_ipaddr[64];
extern char omni_mac_str[16];
extern int state;
#endif

#ifdef CONFIG_NTP
#include "ntp/ntpd.h"
#endif
#ifdef CONFIG_LOG
#include "log/log.h"
#endif
#ifdef CONFIG_JSON
#include <cJSON.h>
#endif
#include <wifi_clock_cfg.h>
#if (RTC_TIME == 1)
#include "wifi_time.h"
#endif

#define TEST_UART 0 //Lynx UART1
 
#define my_debug 0

#define UART_RECV_TIMEOUT 0
#define UART_ONE_PACKAGE_LENGTH 1024
#define UART_BAUD_RATE 115200

static network_info apcfg;
network_info_adv stacfgadv;

static int led_sts = 0;
const char *sw_build_time = (char *)SW_BUILD_TIME;
const char *sw_build_sdk = (char *)MT_SDK_VER_STR;
const int sw_build_count = SW_BUILD_COUNT;

extern sdk_param g_atParam;
sdk_param sdkParam;

#define UART_BR_TB 16
extern unsigned int br_tb[UART_BR_TB];
char s_linkup = 0;
void (*sta_start_func)(char *, char *, unsigned char *);
void (*ap_start_func)(char *, char *, int);

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
	for (id = UART_BR_TB - 1; id >= 0; id--)
	{
		if (br_tb[id] > baud)
			continue;
		break;
	}
	if (id < 0)
		id = 0;
	serial_conf(id, parity, stop_bits, TEST_UART, threshold);
	serial_init((int)TEST_UART);

#ifdef CONFIG_UR_FLOW_CONTROL
	pin_mode(CONFIG_UR_CTS, 0);
	digital_write(CONFIG_UR_RTS, 0);									   //RTS active
	pin_en_intr(CONFIG_UR_CTS, 0, (gpio_func_cb)uart_flow_cts_stat, NULL); //0: rising
	pin_en_intr(CONFIG_UR_CTS, 1, (gpio_func_cb)uart_flow_cts_stat, NULL); //1: falling
	pin_mode(CONFIG_UR_RTS, 1);
#endif
}

char get_saved_ap_index(struct _aplist_adv *ap_dev)
{
	unsigned char loop = 0;
	for (loop = 0; loop < 3; loop++)
	{
		if (strcmp(ap_dev->ssid, sdkParam.sta_param[loop].ssid) == 0)
			return loop;
	}
	return -1;
}

/*FIXME:
 *this notify callback function is readd to notify at Smartconfig.c function.so never be invoked.
 */
static char staup = -1;
static void notify_wifi_scanHandler(scan_result *pApList)
{
	struct _aplist_adv *ap_adv;
	char apPower[3] = {100, 100, 100};
	char apPowerIndex = -1;
	int i = 0;
	char buf[33];
	printf("Station Scan Done\n");
	//printf("step2-------------->>\r\n");
	if (pApList == NULL)
		return;

	ap_adv = (struct _aplist_adv *)pApList->aplist;
	for (i = 0; i < pApList->ap_num; i++)
	{
		memset(buf, 0, sizeof(buf));
		memcpy(buf, ap_adv->ssid, sizeof(ap_adv->ssid));
		apPowerIndex = get_saved_ap_index((struct _aplist_adv *)ap_adv);
		if (apPowerIndex != -1)
		{
			apPower[apPowerIndex] = -(ap_adv->power);
			printf("ap power %d ", apPower[apPowerIndex]);
		}
		printf("\n");
		printf("<ecn>:%d ", ap_adv->security);
		printf("<ssid>:%s ", buf);
		printf("<rssi>:%d ", ap_adv->power);
		printf("<mac>:%s ", ether_ntoa(ap_adv->bssid));
		printf("<ch>:%d ", ap_adv->channel);
		printf("\n");
		ap_adv++;
	}
	if (apPower[0] <= apPower[1])
	{
		if (apPower[0] <= apPower[2])
		{
			apPowerIndex = 0;
		}
		else
		{
			apPowerIndex = 2;
		}
	}
	else
	{
		if (apPower[1] <= apPower[2])
		{
			apPowerIndex = 1;
		}
		else
		{
			apPowerIndex = 2;
		}
	}
	if (apPowerIndex != -1 && staup == -1)
	{
		extern void sta_start(char *ssid, char *pwd, unsigned char *bssid);
		sta_start(sdkParam.sta_param[apPowerIndex].ssid,
				  sdkParam.sta_param[apPowerIndex].key, NULL);
		staup = 1;
	}
}

/*FIXME:
 *this notify callback function is readd to notify at Smartconfig.c function.so never be invoked.
 */
static void notify_DNS_handler(char *str, int len)
{
#ifdef CONFIG_AT_CMD_FUNC
	at_printf("[%s]\n", str);
#else
	printf("[%s]\n", str);
#endif
}

/*FIXME:
 *this notify callback function is readd to notify at Smartconfig.c function.so never be invoked.
 */
static void notify_wifi_status_handler(int status)
{
	switch (status)
	{
	case NOTIFY_STA_LINK_UP:
		s_linkup = 1;
		printf("Station up %d\n", s_linkup);
		//printf("step4-------------->>\r\n");
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
		/* use for reentry sta up */
		staup = -1;
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

/*FIXME:
 *this notify callback function is readd to notify at Smartconfig.c function.so never be invoked.
 */
static void notify_dhcp_handler(ip_sts *pnet)
{
	printf("IP=%s Gate=%s Mask=%s\r\nDNS=%s MAC=%s Bcast=%s\n",
		   pnet->ip, pnet->gate, pnet->mask,
		   pnet->dns, pnet->mac, pnet->broadcastip);
}

void sta_start(char *ssid, char *pwd, unsigned char *bssid)
{
	printf("SSID=%s,PASS=%s,\n", ssid, pwd);
	//printf("step3-------------->>\r\n");
	/* Initialize station parameteres */
	memset(&stacfgadv, 0x0, sizeof(stacfgadv));
	strcpy(stacfgadv.ap_info.ssid, ssid); /* wlan ssid string */
	/*
		WEP key length:			ASCII=5(64 bits) or 13(128 bits)
								HEX=10(64 bits) or 26(128 bits)
		WPA/WPA2 key length:	ASCII=8-63, HEX=64
	 */
	strcpy(stacfgadv.key, pwd); /* wlan key string or hex data */
	stacfgadv.key_len = strlen(pwd);
	stacfgadv.dhcp_mode = DHCP_DISABLE;			/* Fetch Ip address from DHCP server */
	stacfgadv.ap_info.security = SECURITY_AUTO; /* Wlan security mode */
	stacfgadv.ap_info.channel = 0;				/* Select chanel automatically */
	if ((sdkParam.dhcp_mode == 2) || (sdkParam.dhcp_mode == 3))
		stacfgadv.dhcp_mode = DHCP_CLIENT;
	strcpy((char *)stacfgadv.local_ip_addr, sdkParam.sta_param[0].ip);
	strcpy((char *)stacfgadv.net_mask, sdkParam.sta_param[0].mask);
	strcpy((char *)stacfgadv.gateway_ip_addr, sdkParam.sta_param[0].gw);
	strcpy((char *)stacfgadv.dnssvr_ip_addr, "8.8.8.8");
	if (bssid != NULL)
		strcpy(stacfgadv.ap_info.bssid, bssid); /* wlan bssid*/
	stacfgadv.wifi_retry_interval = 0000;		/* Retry interval after a failure connection */
	wlan_set_reconnect((sdkParam.flags & CFG_RECONN) ? 1 : 0);
	wlan_set_myaddr(STATION, sdkParam.mac.sta_mac);
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
	//	if ((sdkParam.dhcp_mode == 1) || (sdkParam.dhcp_mode == 3))
	if (1)
		apcfg.dhcp_mode = DHCP_SERVER;

	strcpy((char *)apcfg.local_ip_addr, WEB_SERVER_IP);
	strcpy((char *)apcfg.net_mask, WEB_SERVER_MASK);
	strcpy((char *)apcfg.gateway_ip_addr, WEB_SERVER_GW);
	strcpy((char *)apcfg.dnssvr_ip_addr, "8.8.8.8");
	wlan_set_hidden_ssid(0);
	//	if (WEB_SERVER_HIDDEN_SSID)
	//		wlan_set_hidden_ssid(1);
	apcfg.wifi_retry_interval = 100;
	//	wlan_set_myaddr(SOFT_AP, NULL);
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

	if (sm = wlan_get_ifs_sm(STATION))
	{
		switch (sm)
		{
		case STATE_SCAN:
		case STATE_SCAN_DONE:
		case STATE_LINK_DOWN:
			//s_linkup = 0;
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
	}
	else if ((sm = wlan_get_ifs_sm(SOFT_AP)) &&
			 wlan_get_sta_num(SOFT_AP))
	{
		pin_mode(WIFI_LED_PIN, 1);
		led_sts = !led_sts;
	}
	else
	{
		pin_mode(WIFI_LED_PIN, 1);
		led_sts = 0;
	}
	if (gpio_mode)
		digital_write(WIFI_LED_PIN, led_sts);
	return timeout;
}
void time_callback(void)
{
	int a = 1;
	a++;
	// printf("test \n");
}
/*   C L O C K _ A P P _ S T A R T   U P   */
/*-------------------------------------------------------------------------
*@param			
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief			
-------------------------------------------------------------------------*/
static inline void clock_app_startUp()
{

	event_process();
#if (RTC_TIME == 0)
	xTaskCreate(get_local_time, "localtm", 1024 * 6, NULL, configMAX_PRIORITIES - 1, NULL);
#else
	xTaskCreate(get_current_time_task, "rtcTime", 1024 * 6, NULL, configMAX_PRIORITIES - 1, NULL);
#endif /* (RTC_TIME == 0) */
#if (ENABLE_NTP_TIME == 1)
	xTaskCreate(get_ntp_time, "NtpTime", 1024 * 3, NULL, 4, NULL);
#endif
	xTaskCreate(get_wifi_weather, "WthMsg", 1024 * 4, NULL, 4, NULL);
	xTaskCreate(get_wifi_pm25, "PM25", 1024 * 3, NULL, 4, NULL);
#if 1
#if (VERSION_DEMO || CUSTOMER_VERSION)
	xTaskCreate(start_alarm_nowtime, "nowtime", 1024, NULL, 6, NULL);
#endif
#if (VERSION_DEMO || CUSTOMER_VERSION)
	xTaskCreate(start_alarm, "alarm", 1024 * 2, NULL, 6, NULL);
#endif
#if VERSION_MUSHAN
	xTaskCreate(register_id_to_web, "register", 2048, NULL, 8, NULL);
	xTaskCreate(create_PWM, "RHPower", 1024, NULL, configMAX_PRIORITIES - 1, NULL);
	xTaskCreate(excute_alarm_check, "alarm", 1024, NULL, 3, NULL);
#endif
	xTaskCreate(event_get_task, "eventGet", 1024, NULL, 6, NULL);
#endif
}

void init_wlan_up_gpio()
{
	gpio_enable(CONFIG_PIN, ENABLE);
	gpio_pullup(CONFIG_PIN, 0);
	pin_mode(CONFIG_PIN, GPIO_OUT);
	digital_write(CONFIG_PIN, HIGH);
	pin_mode(CONFIG_PIN, GPIO_IN);
}
static void inline onimconfig_enter(void)
{
#ifdef CONFIG_OMNICONFIG
	int try_time = 100;
	if (state == OMNI_STATE_CONNECTING)
	{
		sys_msleep(1000);
#if (VERSION_DEMO || CUSTOMER_VERSION)
		xTaskCreate(config_param, "cfgPrm", 1024 * 3, NULL, 6, NULL);
#endif
		do
		{
			/*TODO: Ping Test*/
			omnicfg_broadcast(OMNI_STATE_CONNECT_SUCCESS, omni_ipaddr, omni_mac_str, 1);
			serial_printf("------------------------->IP = %s MAC = %s \n", omni_ipaddr, omni_mac_str);
			try_time--;
			sys_msleep(10);
		} while (try_time > 0);
		state = OMNI_STATE_CONNECT_SUCCESS;
		//clock_app_startUp();
	}
#endif
	sys_msleep(300);
}

/*   S T A R T _ U P _ W L A N   */
/*-------------------------------------------------------------------------
*@param			
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief			
-------------------------------------------------------------------------*/
static void start_up_wlan()
{
	config_load();
#if CONFIG_LWIP
	tcpip_init(0, 0);
	net_init_notification();

	wlan_init_notification();
	wlan_init();
	uart_init();

	sta_start_func = sta_start;
	ap_start_func = ap_start;

	wlan_add_notification(NOTIFY_WIFI_LED_TIMEOUT, user_led_handler);
	/* if add smartlink config those notification will not work */
	wlan_add_notification(NOTIFY_WIFI_SCAN_COMPLETED, notify_wifi_scanHandler);
	wlan_add_notification(NOTIFY_WIFI_STATUS_CHANGED, notify_wifi_status_handler);
#if defined(CONFIG_LWIP)
	net_add_notification(NOTIFY_DHCP_COMPLETED, notify_dhcp_handler);
	net_add_notification(NOTIFY_DNS_RESOLVE_COMPLETED, notify_DNS_handler);
#endif
#endif
	init_wlan_up_gpio();
	config_get(&sdkParam);
	//	wlan_led_install();
	/* Log debug interface */
#if CONFIG_LOG
	logcatInit();
	setLogLevel(LOG_ASSERT);
	assert("build time %s,build count %d=======\r\n", SW_BUILD_TIME, SW_BUILD_COUNT);
	setLogLevel(LOG_NONE);
#endif
}
/*   U S E R _ T H R E A D   */
/*-------------------------------------------------------------------------
*@param			
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief			
-------------------------------------------------------------------------*/
int user_thread(void *arg)
{
	start_up_wlan();
	if (sdkParam.configMode == true)
	{
		wlan_suspend_station();
		sys_msleep(10);
		goto config;
	}

	wlan_start_scan();
	extern init_peripheral();
	init_peripheral();
	
	#if 0
	FD612_ZERO_INIT();
	sys_msleep(1000);
	sys_msleep(1000);
	sys_msleep(1000);
	sys_msleep(1000);
	sys_msleep(1000);
	sys_msleep(1000);
	FD612_app_init();
	printf("step6--------------->>\r\n");

	while(1)
	{
		sys_msleep(1000);
	}
	#endif
#if 1

#if (ALL_LIGHT_DEBUG == 1)
	all_light_debug();
	goto exit;
#endif

	if (digital_read(CONFIG_PIN) == 0)
	{
		wlan_suspend_station();
		sys_msleep(10);
		goto config;
	}
#if (TEST_CASE == 1)
	extern void start_test_case();
	start_test_case();
	goto exit;
#endif
	clock_app_startUp();
#if my_debug
	while(1)
	{
		printf("sleep------------->>>>\r\n");
		sys_msleep(1000);
	}
#endif
	goto exit;

#if (WEB_CONFIG == 1)
config:
	xTaskCreate(cfg_count_zero, "cntTime", 1024, NULL, 5, NULL);
	ap_start_func(WEB_SERVER_SSID, WEB_SERVER_KEY, WEB_SERVER_CHANNEL);
	xTaskCreate(http_server_start, "server", 1024 * 4, NULL, 3, NULL);
	vTaskDelete(NULL);
#else
config:
	xTaskCreate(cfg_count_zero, "cntTime", 1024, NULL, 5, NULL);
	omnicfg_start(120);
	while (1)
	{
		onimconfig_enter();
	}
#endif
exit:
	// while (1) {
	// 	// TODO:code clean for furture
	// 	if (digital_read(CONFIG_PIN) == 0) {
	// 			wlan_suspend_station();
	// 			sys_msleep(10);
	// 			goto config;
	// 	}
	// 	sys_msleep(300);
	// }

	vTaskDelete(NULL);

#endif
}
