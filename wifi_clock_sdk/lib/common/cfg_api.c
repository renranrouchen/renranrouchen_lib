/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/


/**
 * @file cfg_api.c
 * @brief Config APIs
 *
 */
#include <lynx_dev.h>
#include <arch/ba22/chip.h>
#include <mt_types.h>
#include <common.h>
#include <lynx_debug.h>
#include <cfg_api.h>
#include <wla_api.h>


/**
 * Config load data to memory, initialize config read/write data process.
 * Call config_load before read/write config data.
 * @return config memory default length 0x400, 0: error
 */
static sdk_param s_sdk_param;
static int s_load = 0;
extern sdk_param g_defparam;

extern void wlan_set_myaddr(wlan_if_types type, char *myaddr) __attribute__((weak));
extern char *wlan_get_myaddr(wlan_if_types type) __attribute__((weak));

static void config_update_macaddr(void)
{
	unsigned char *addr;

	addr = s_sdk_param.mac.sta_mac;
	if ((addr[0]|addr[1]|addr[2]|addr[3]|addr[4]|addr[5]) == 0 && wlan_get_myaddr != NULL) // sta's mac is all zero
		memcpy(addr, wlan_get_myaddr(STATION), 6);
	else if(wlan_set_myaddr)
		wlan_set_myaddr(STATION, addr);

	addr = s_sdk_param.mac.ap_mac;
	if ((addr[0]|addr[1]|addr[2]|addr[3]|addr[4]|addr[5]) == 0 && wlan_get_myaddr != NULL) // ap's mac is all zero
		memcpy(addr, wlan_get_myaddr(SOFT_AP), 6);
	else if(wlan_set_myaddr)
		wlan_set_myaddr(SOFT_AP, addr);
}

void config_dump(void)
{
	char alarm_loop = 0;
	if (s_load == 0)
		return;

	serial_printf("\n\n");
	serial_printf(" cfg_magic:%x\n", *(unsigned int *)s_sdk_param.cfg_magic);
	serial_printf(" flags:%x\n", s_sdk_param.flags);
	serial_printf(" chksum:%x\n", s_sdk_param.chksum);
	serial_printf(" led_value:%d\n", s_sdk_param.led_value);
	serial_printf(" work_mode:%d\n", s_sdk_param.work_mode);
	serial_printf(" dhcp_mode:%d\n", s_sdk_param.dhcp_mode);
	serial_printf(" phy_mode:%d\n", s_sdk_param.phy_mode);
	serial_printf(" txpower:%d\n", s_sdk_param.txpower);
	serial_printf(" ps_mode:%d\n", s_sdk_param.ps_mode);
	serial_printf(" temperature:%d\n", s_sdk_param.temperature);
	serial_printf(" brightness:%d\n", s_sdk_param.brightness);
	/*
	serial_printf(" sm_param:%d,%d\n",
					s_sdk_param.sm_param.enable,
					s_sdk_param.sm_param.angle);
	serial_printf(" dm_param:%d,%d\n",
					s_sdk_param.dm_param.enable,
					s_sdk_param.dm_param.duty);
	serial_printf(" ledrgb_param:%d,%d,%d,%d\n",
					s_sdk_param.ledrgb_param.enable,
					s_sdk_param.ledrgb_param.r,
					s_sdk_param.ledrgb_param.g,
					s_sdk_param.ledrgb_param.b);
	*/
	serial_printf(" sta mac:%s\n", ether_ntoa(s_sdk_param.mac.sta_mac));
	serial_printf(" ap mac:%s\n", ether_ntoa(s_sdk_param.mac.ap_mac));
	serial_printf(" uart:%d,%d,%d,%d,%d\n",
					s_sdk_param.ur_param.baudrate,
					s_sdk_param.ur_param.databits,
					s_sdk_param.ur_param.stopbits,
					s_sdk_param.ur_param.parity,
					s_sdk_param.ur_param.flowctrl);
	serial_printf(" sta_param:%s,%s,%s,%s,%s\n",
					s_sdk_param.sta_param[0].ssid,
					s_sdk_param.sta_param[0].key,
					s_sdk_param.sta_param[0].ip,
					s_sdk_param.sta_param[0].mask,
					s_sdk_param.sta_param[0].gw);
	serial_printf(" sta_param:%s,%s,%s,%s,%s\n",
					s_sdk_param.sta_param[1].ssid,
					s_sdk_param.sta_param[1].key,
					s_sdk_param.sta_param[1].ip,
					s_sdk_param.sta_param[1].mask,
					s_sdk_param.sta_param[1].gw);
	serial_printf(" sta_param:%s,%s,%s,%s,%s\n",
					s_sdk_param.sta_param[2].ssid,
					s_sdk_param.sta_param[2].key,
					s_sdk_param.sta_param[2].ip,
					s_sdk_param.sta_param[2].mask,
					s_sdk_param.sta_param[2].gw);
	serial_printf(" softap_param:%s,%s,%s,%s,%s,%d,%d,%d\n",
					s_sdk_param.ap_param.ssid,
					s_sdk_param.ap_param.key,
					s_sdk_param.ap_param.ip,
					s_sdk_param.ap_param.mask,
					s_sdk_param.ap_param.gw,
					s_sdk_param.ap_param.enc,
					s_sdk_param.ap_param.max_con,
					s_sdk_param.ap_param.hidden_ssid);
	serial_printf(" dhs_param:%s,%s,%d\n",
					s_sdk_param.dhs_param.startip,
					s_sdk_param.dhs_param.endip,
					s_sdk_param.dhs_param.leave_time);
	/*
	serial_printf(" ts_param:%d,%s,%d,%s,%d\n",
					s_sdk_param.ts_param.enable,
					s_sdk_param.ts_param.ip,
					s_sdk_param.ts_param.port,
					s_sdk_param.ts_param.type,
					s_sdk_param.ts_param.time_alive);
	*/	
	serial_printf(" webtime_param: %d,%d,%d,%d\n",
					s_sdk_param.web_param.isOpen,
					s_sdk_param.web_param.timeZone,
					s_sdk_param.web_param.updateInterver / 60 / 60,
					s_sdk_param.web_param.updateTime / 60 / 60);
#if (VERSION_MUSHAN == 0)
	for(;alarm_loop < 21;alarm_loop++){
	serial_printf(" alram default time: day:%d,default:%d,alarm:%d,snooze:%d,snooze open %d,isopen:%d\n",
					s_sdk_param.alm_param[alarm_loop].alarmDay,
					s_sdk_param.alm_param[alarm_loop].defaultAlarmTime,
					s_sdk_param.alm_param[alarm_loop].alarmTime,
					s_sdk_param.alm_param[alarm_loop].snoozeDelay,
					s_sdk_param.alm_param[alarm_loop].isOpenSnooze,
					s_sdk_param.alm_param[alarm_loop].isOpen);
	}
#endif	
#if VERSION_MUSHAN 
	serial_printf(" V2 alarm default time: %d, isopen %d \n",
					s_sdk_param.v2_alarm_param.defaultAlarmTime,
					s_sdk_param.v2_alarm_param.isOpenAlarm);
	serial_printf(" acid %s register statue %d config mode %d device id %s\n",
					s_sdk_param.acid,
					s_sdk_param.webRegisterStatue,
					s_sdk_param.configMode,
					s_sdk_param.deviceId);				
#endif
	serial_printf(" weather %d,cityId %d,update interval %d\n",
					s_sdk_param.wth_parma.isOpen,
					s_sdk_param.wth_parma.cityId,
					s_sdk_param.wth_parma.updateInterval);
	serial_printf(" nowtime %d\n",s_sdk_param.nt_param.isOpen);
}
int config_load(void)
{
	int ret, src = CFG_FLASH_MEM_START, load_sdk_cfg;
	u32 sf_id, size;
	char magic[4] = CFG_MAGIC;

	PMUREG(PKG_MODE_CTRL) |= STATION_MODE;
	sflash_init();
#if (CONFIG_ROM_VER > 1)
	sf_id = ldev->sf.fdb.id;
#else
	sf_id = ldev->sf.id;
#endif
	if (sf_id == 0) {
		DBG_PRINTF(ERROR, "sflash initial error!!!\n");
		return 0;
	}

	size = sizeof(sdk_param);
	DBG_PRINTF(INFO, "config_load size=%d\n", size);
	if (size >= CFG_FLASH_MEM_LENGTH) {
		DBG_PRINTF(ERROR, "config size error, max=%d!!!\n", CFG_FLASH_MEM_LENGTH);
		return 0;
	}

	if (flash_read(src, (unsigned int)&s_sdk_param, size)) {
		DBG_PRINTF(ERROR, "config read error!!!\n");
		return 0;
	}

	s_load = 1;
	load_sdk_cfg = 0;
	if (memcmp(&s_sdk_param.cfg_magic, magic, 4)) {
		load_sdk_cfg = 1;
		DBG_PRINTF(ERROR, "config magic error!!!\n");
	} else if (s_sdk_param.flags & CFG_CHKSUM_EN) {
		int rc;
		rc = ip_cksum(&s_sdk_param, size);
		if (rc) {
			DBG_PRINTF(ERROR, "config chksum error:%04x\n", rc);
			load_sdk_cfg = 1;
		}
	}

	if (load_sdk_cfg) {
		DBG_PRINTF(INFO, "load default config\n");
		memcpy(&s_sdk_param, &g_defparam, size);
		config_update_macaddr();
		config_submit();
	} else {
		config_update_macaddr();
	}
	config_dump();

	return 1;
}

int config_get(sdk_param *param)
{
	if (param == NULL)
		return 0;
	if (s_load == 0)
		return 0;

	memcpy(param, &s_sdk_param, sizeof(sdk_param));
	return 1;
}

int config_set(sdk_param *param)
{
	if (s_load == 0)
		return 0;

	DBG_PRINTF(INFO, "config_set\n");
	memcpy(&s_sdk_param, param, sizeof(sdk_param));
	return 1;
}

int config_submit(void)
{
	int dst;
	unsigned char *pbuf;
	unsigned int sum, i;
	u32 size;

	if (s_load == 0)
		return 0;
	
	s_sdk_param.chksum = 0;
	size = sizeof(sdk_param);
	pbuf = (unsigned char *)&s_sdk_param;
	sum = 0;
	for (i=0; i<size; i+=2)
		sum += (pbuf[i]<<8) | pbuf[i+1];
	if (size & 1)
		sum += *(unsigned char *)&pbuf[i-2];
	while (sum>>16)
		sum = (sum&0xffff) + (sum>>16);
	sum = (0xffff ^ sum);
	s_sdk_param.chksum = sum;

	dst = CFG_FLASH_MEM_START;
	DBG_PRINTF(INFO, "config_submit\n");

	flash_erase(CFG_FLASH_MEM_START, CFG_FLASH_MEM_LENGTH);
	flash_write(CFG_FLASH_MEM_START, (unsigned int)&s_sdk_param, sizeof(sdk_param));
	return 1;
}

int config_erase(void)
{
	int dst;

	if (s_load == 0)
		return 0;

	dst = CFG_FLASH_MEM_START;

	flash_erase(CFG_FLASH_MEM_START, CFG_FLASH_MEM_LENGTH);
	return 1;
}

