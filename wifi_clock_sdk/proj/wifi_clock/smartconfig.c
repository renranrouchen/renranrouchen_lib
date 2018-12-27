/*=============================================================================+
|                                                                              |
| Copyright 2015                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file app_init.c
*   \brief main entry
*   \author Montage
*/

/*=============================================================================+
| Included Files                                                               |
+=============================================================================*/
#include <event.h>
#include <common.h>
#include <os_api.h>
#include <net_api.h>
#include <wla_api.h>
#include <gpio.h>
#include <omniconfig.h>
#include <omniconfig_func.h>
#include "string.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "cfg_api.h"
#include <wifi_clock_cfg.h>

#if (WEB_CONFIG == 0)
//#include "log/log.h"
#undef printf
#define printf(fmt,args...)		serial_printf(fmt ,##args)
static int time_cost			= 0; /*1ms*/


extern sdk_param sdkParam;
extern sdk_param g_defparam;


char omni_ipaddr[64] = {0};
char omni_mac_str[16] = {0};
int state = OMNI_STATE_NONE;
static network_info stacfg;

static void notify_wifi_scanHandler(scan_result *pApList)
{
	struct _aplist_adv *ap_adv;
	int i = 0;
	char buf[33];

	printf("Station Scan Done\n");

	if (pApList == NULL)
		return;

	ap_adv = (struct _aplist_adv *)pApList->aplist;
	for (i = 0; i < pApList->ap_num; i++) {
		memset(buf, 0, sizeof(buf));
		memcpy(buf, ap_adv->ssid, sizeof(ap_adv->ssid));
		printf("\n");
		printf("<ecn>:%d ", ap_adv->security);
		printf("<ssid>:%s ", buf);
		printf("<rssi>:%d ", ap_adv->power);
		printf("<mac>:%s ", ether_ntoa(ap_adv->bssid));
		printf("<ch>:%d ", ap_adv->channel);
		printf("\n");
		ap_adv++;
	}

}

static void notify_DNS_handler(char *str, int len)
{
	printf("[%s]\n", str);
}

static void notify_connectfailed_handler(int err)
{
	//wlan_start(&stacfg);
	printf("Connect err=%d\n", err);
}
static void flash_modify_config(char* ssid,char* key)
{
	printf("\r\n==============START FILLIN G_ATPARAM :===========\r\n");
	/*  rewrite param to defalut */
	
	struct sdk* tmpParam = (sdk_param* )malloc(sizeof(sdk_param));
	memcpy(&sdkParam,&g_defparam,sizeof(sdk_param));
	config_get(tmpParam);
	strcpy(sdkParam.sta_param[2].ssid,tmpParam->sta_param[1].ssid);
	strcpy(sdkParam.sta_param[2].key,tmpParam->sta_param[1].key);

	strcpy(sdkParam.sta_param[1].ssid,tmpParam->sta_param[0].ssid);
	strcpy(sdkParam.sta_param[1].key,tmpParam->sta_param[0].key);

	free(tmpParam);
	
	strcpy(sdkParam.sta_param[0].ssid,ssid);
	strcpy(sdkParam.sta_param[0].key,key);
//	sdkParam.dhcp_mode = 3;
	sdkParam.work_mode = OPMODE_STA;
	sdkParam.flags |= CFG_AUTO_CONN;
	config_set(&sdkParam);
	config_submit();
	/* dump the param */
	config_dump();
}

static void omniconfig_done(char *ssid, char *key, int mode,int error)
{
	/* check omnicfg time cost*/
	time_cost = sys_now() - time_cost;
	unsigned char *mac;
	
	printf("\r\n============ IOT_DEMO OMNICONFIG CALLBACK Result : ====\r\n");
	printf("OMNICONFIG Time Cost : %d (ms)\n",time_cost);
	
	if (OMNI_RET_SUCCESS == error) {
		
		printf("WiFi SSID : %s\n", ssid);
		printf("WiFi PWD  : %s\n", key);
		/*OMNICFG modified str*/
		/* modify default param */
		if (strcmp(ssid,sdkParam.sta_param[0].ssid) != 0 || strcmp(key,sdkParam.sta_param[0].key) != 0) {
			flash_modify_config(ssid,key);
		}
#if 1
		memset(omni_mac_str, 0x0, sizeof(omni_mac_str));
		mac = wlan_get_myaddr(STATION);
		sprintf(omni_mac_str, "%02x%02x%02x%02x%02x%02x",
				mac[0], mac[1], mac[2],
				mac[3], mac[4], mac[5]);
		
		/* Initialize wlan parameteres */
		memset(&stacfg, 0x0, sizeof(stacfg));
		memcpy(stacfg.wifi_ssid, ssid, sizeof(stacfg.wifi_ssid));	/* wlan ssid string */
		memcpy(stacfg.wifi_key, key, sizeof(stacfg.wifi_key));		/* wlan key string or hex data */
		
		stacfg.wifi_mode = STATION;							/* wlan mode */
		stacfg.dhcp_mode = DHCP_CLIENT;						/* Fetch Ip address from DHCP server */
		
		stacfg.wifi_retry_interval = 1000;					/* Retry interval after a failure connection */
		wlan_set_reconnect(1);

		/* Connect Now! */
		/* calculate wifi scan/connect time*/
		time_cost = sys_now() ;
		printf("\r\n============ START IOT_DEMO WIFI Connection : =========\r\n");
		wlan_start(&stacfg);
#endif
	} else if (OMNI_RET_TIMEOUT == error) {
		printf("Result    : OMNICONFIG Timeout\n");
	} else if (OMNI_RET_ERR_SSID == error) {
		printf("Result    : SSID Length is too long ! \n");
		printf("          : SSID Length need be small than 32 .\n");
	} else if (OMNI_RET_ERR_PWD == error) {
		printf("Result    : PWD Length is too long ! \n");
		printf("          : PWD Length need be small than 64.\n");
	} else if (OMNI_RET_ERR_UNKNOW == error) {
		printf("Result    : INTERNAL ERROR\n");
	}
}


static void notify_wifi_para_handler(apinfo_adv *ap_info, char *key, int key_len)
{
	char buf[33];
	
	/* check wifi scan time cost*/
	time_cost = sys_now() - time_cost;
	printf("\r\nScan Use Time : %d (ms)\r\n",time_cost);

	memset(buf, 0, sizeof(buf));
	memcpy(buf, ap_info->ssid, sizeof(ap_info->ssid));
	printf("SSID=%s BSSID=%s CH=%d SEC=%d KEY=%s KEY_Len=%d\n",
				buf, ether_ntoa(ap_info->bssid),
				ap_info->channel, ap_info->security,
				key, key_len);

//	omniconfig_done(buf,key,0,OMNI_RET_SUCCESS);
//	wlan_del_notification(NOTIFY_WIFI_PARA_CHANGED);
	/* calculate Link time*/
	time_cost = sys_now();
}

static void notify_wifi_status_handler(int status)
{
	switch (status) {
	case NOTIFY_STA_LINK_UP:
		printf("\r\n==============================================================\r\n");
		/* check wifi link time cost*/
		time_cost = sys_now() - time_cost;
		printf("Link Use Time (Layer 2) : %d (ms)\r\n",time_cost);
		printf("Station up\n");
#if defined(CONFIG_LWIP)
		net_if_up(NETIF_STA, stacfg.dhcp_mode, wlan_get_myaddr(STATION),
				stacfg.local_ip_addr, stacfg.net_mask,
				stacfg.gateway_ip_addr, stacfg.dnssvr_ip_addr);
#endif
		break;
	case NOTIFY_STA_LINK_DOWN:
		printf("Station down\n");
#if defined(CONFIG_LWIP)
		net_if_down(NETIF_STA);
#endif
		break;
	}
	/* calculate DHCP time*/
	time_cost = sys_now();
}

static void notify_dhcp_handler(ip_sts *pnet)
{
	/* check DHCP time cost*/
	time_cost = sys_now() - time_cost;
	printf("==============================================================\r\n");
	printf("DHCP Use Time : %d (ms)\n",time_cost);
	printf("IP=%s Gate=%s Mask=%s\r\nDNS=%s MAC=%s Bcast=%s\n",
				pnet->ip, pnet->gate, pnet->mask,
				pnet->dns, pnet->mac, pnet->broadcastip);
	printf("==============================================================\r\n");
	time_cost = sys_now();

	/* Send OMNICFG Broadcast Msg*/
	memset(omni_ipaddr, 0x0, sizeof(omni_ipaddr));
	sprintf(omni_ipaddr, "%s", pnet->ip);
	state = OMNI_STATE_CONNECTING;
	printf("Send Broadcast report to OMNICFG APP\r\n");
	printf("==============================================================\r\n");
}
void config_done_test_callback(char *ssid, char *key, int mode,int error)
{
	printf("\r\n SSID %s,PASS %s \r\n",ssid,key);
}

void omnicfg_start(int sec)
{
	int timeout_ms = (sec*1000); /* 360 sec (6min)*/

	wlan_add_notification(NOTIFY_WIFI_SCAN_COMPLETED, notify_wifi_scanHandler);
	wlan_add_notification(NOTIFY_WIFI_STATUS_CHANGED, notify_wifi_status_handler);
	wlan_add_notification(NOTIFY_WIFI_PARA_CHANGED, notify_wifi_para_handler);
	wlan_add_notification(NOTIFY_WIFI_CONNECT_FAILED, notify_connectfailed_handler);
#ifdef CONFIG_LWIP
	net_add_notification(NOTIFY_DHCP_COMPLETED, notify_dhcp_handler);
	net_add_notification(NOTIFY_DNS_RESOLVE_COMPLETED, notify_DNS_handler);
#endif

	{
		printf("\r\n============ IOT_DEMO CALL OMNICONFIG API : ==================\r\n");
		/* calculate omnicfg time*/
		time_cost = sys_now() ;
		omnicfg_siso2_start(omniconfig_done,timeout_ms);
		//omnicfg_siso2_start(config_done_test_callback,timeout_ms);
		/*omnicfg_siso2_start(cb_func(char *ssid, char *key, int mode),u32 timeout_ms)*/
	}
}

void omnicfg_stop(void)
{
	omnicfg_siso2_stop();
}
#endif
