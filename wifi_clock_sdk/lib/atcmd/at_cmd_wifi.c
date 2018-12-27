#include "at_cmd.h"
#include "at_cmd_func.h"
#include <common.h>
#include <cfg_api.h>
#include <event.h>
#include <lynx_dev.h>
#include <wla_api.h>
#include <net_api.h>

void (*sta_start_func)(char *ssid, char *pwd,unsigned char *bssid) = NULL;
void (*ap_start_func)(char *ssid, char *pwd, int ch) = NULL;

sdk_param g_atParam;

/*!-----------------------------------------------------------------------------
 * function: at_wifi_mode
 *
 *  \brief:    Set or query wifi operation mode
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  0:STA 1:AP 2:AP+STA
 *  \return:   check result
 +----------------------------------------------------------------------------*/
void wifi_restart(void)
{
	int ap, sta, sm;
	sta_param *sta_ptr;
	softap_param *ap_ptr;

	if ((sm = wlan_get_ifs_sm(SOFT_AP)) != STATE_IDLE)
		ap = 1;
	else
		ap = 0;
	if ((sm = wlan_get_ifs_sm(STATION)) != STATE_IDLE)
		sta = 1;
	else
		sta = 0;

	wlan_suspend();

	if (sta && (g_atParam.work_mode != OPMODE_AP)) {
		sta_ptr = &g_atParam.sta_param;
		if (sta_start_func)
			sta_start_func(sta_ptr->ssid, sta_ptr->key, NULL);
	}
	if (ap && (g_atParam.work_mode != OPMODE_STA)) {
		ap_ptr = &g_atParam.ap_param;
		if (ap_start_func)
			ap_start_func(ap_ptr->ssid, ap_ptr->key, ap_ptr->channel);
	}
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_mode
 *
 *  \brief:    Set or query wifi operation mode
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  0:STA 1:AP 2:AP+STA(do not support)
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_mode(int argc, char *argv[], char *op[])		//wmode
{
	if (!strcmp(op,"=")) {
		int val;
		if (argc != 1)
			return at_ERR_HELP;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;
		val = atoi(argv[0]);

		if ((val < OPMODE_STA) || (val > OPMODE_APSTA))
			return at_ERR_PARM;
		if (g_atParam.work_mode != val) {
			g_atParam.work_mode = val;
			save_at_param();
			wifi_restart();
		}
	} else if (!strcmp(op,"=?")) {
		if (argc != 0)
			return at_ERR_HELP;
		at_printf("\n<%d>\n", g_atParam.work_mode);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_scan
 *
 *  \brief:    Network scan, returns a list of found networks
 *  \operator: NULL execute
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_scan(int argc, char *argv[], char *op[])		//wscan
{
	if (strcmp(op,NUL))
		return at_ERR_OP;

	wlan_start_scan();
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_phy
 *
 *  \brief:    Set or query wifi physical mode
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  physical mode [1-7]
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_phy(int argc, char *argv[], char *op[])		//wphy
{
	if (!strcmp(op,"=")) {
		int ret;

		if (argc != 1)
			return at_ERR_HELP;
		if ((atoi(argv[0]) < 1) || (atoi(argv[0]) > 7))
			return at_ERR_PARM;
		if ((ret = wlan_set_phy(atoi(argv[0]))) < 0) {
			if (ret == -2)
				serial_printf("device is busy\n");
			return at_ERR_PARM;
		}

		g_atParam.phy_mode = atoi(argv[0]);
		save_at_param();
	} else if (!strcmp(op,"=?")) {
		if (argc != 0)
			return at_ERR_HELP;
		at_printf("\n<%d>\n", g_atParam.phy_mode);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_txpwr
 *
 *  \brief:    Set or query tx power level
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  tx power level [0-12]
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_txpwr(int argc, char *argv[], char *op[])		//wtxpwr
{
	if (!strcmp(op,"=")) {
		if (argc != 1)
			return at_ERR_HELP;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;
		if (wlan_set_txpwr(atoi(argv[0])) < 0)
			return at_ERR_PARM;
		g_atParam.txpower = atoi(argv[0]);
		save_at_param();
	} else if (!strcmp(op,"=?")) {
		if (argc != 0)
			return at_ERR_HELP;
		at_printf("\n<%d>\n", g_atParam.txpower);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_staup
 *
 *  \brief:    Start wireless network
 *  \operator: "="set
 *  \param 0:  ssid
 *  \param 1:  password
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_staup(int argc, char *argv[], char *op[])		//wstaup
{
	char *ssid, *pwd;

	if (!strcmp(op,"=")) {
		int len;

		if (argc != 2)
			return at_ERR_HELP;

		len = strlen(argv[0]);
		if ((len == 0) || (len > sizeof(g_atParam.sta_param.ssid)))
			return at_ERR_PARM;

		if (strchr(argv[0], ' '))
			return at_ERR_PARM;

		len = strlen(argv[1]);
		if (len > sizeof(g_atParam.sta_param.key))
			return at_ERR_PARM;

		ssid = argv[0];
		pwd = argv[1];
		strcpy(g_atParam.sta_param.ssid, ssid);
		strcpy(g_atParam.sta_param.key, pwd);
		save_at_param();
		if (g_atParam.work_mode == OPMODE_AP) {
			serial_printf("<%d>: invalid work mode\n",
							g_atParam.work_mode);
			return at_ERR_HELP;
		}
		if (sta_start_func)
			sta_start_func(ssid, pwd,NULL);
	}
	else if (!strcmp(op,"=?"))
	{
		link_sts linkStatus;
		if (argc != 0)
			return at_ERR_HELP;

		wlan_get_link_sts(&linkStatus, STATION);
		if (linkStatus.is_connected) {
			at_printf("\n<%s>,<%s>,<%d>,<%d>\n",
						linkStatus.ssid,
						ether_ntoa(linkStatus.bssid),
						linkStatus.channel,
						linkStatus.wifi_strength);
		} else {
			at_printf("link down\n");
		}
	}
	else
	{
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_stadn
 *
 *  \brief:    Stop station
 *  \operator: NULL execute
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_stadn(int argc, char *argv[], char *op[])		//wstadn
{
	if (strcmp(op,NUL))
		return at_ERR_OP;
	wlan_suspend_station();
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_autoconn
 *
 *  \brief:    Start wireless network automatically
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  enable or disable
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_autoconn(int argc, char *argv[], char *op[])		//wautoconn
{
	if (!strcmp(op,"=")) {
		int val;
		if (argc != 1)
			return at_ERR_HELP;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;
		val = atoi(argv[0]);
		if ((val != 0) && (val != 1))
			return at_ERR_PARM;
		if (val)
			g_atParam.flags |= CFG_AUTO_CONN;
		else
			g_atParam.flags &= ~CFG_AUTO_CONN;
		save_at_param();
	} else if (!strcmp(op,"=?")) {
		if (argc != 0)
			return at_ERR_HELP;
		at_printf("\n<%d>\n", (g_atParam.flags & CFG_AUTO_CONN) ? 1 : 0);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_reconn
 *
 *  \brief:    Reconnect to target AP if connection is failed
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  enable or disable
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_reconn(int argc, char *argv[], char *op[])		//wreconn
{
	if (!strcmp(op,"=")) {
		int val, reconn;
		if (argc != 1)
			return at_ERR_HELP;

		if (strchr(argv[0], ' '))
			return at_ERR_PARM;
		val = atoi(argv[0]);
		if ((val != 0) && (val != 1))
			return at_ERR_PARM;
		reconn = (g_atParam.flags & CFG_RECONN) ? 1 : 0;
		if (reconn == val)
			return at_ERR_OK;
		if (val)
			g_atParam.flags |= CFG_RECONN;
		else
			g_atParam.flags &= ~CFG_RECONN;
		save_at_param();
		wifi_restart();
	} else if (!strcmp(op,"=?")) {
		if (argc != 0)
			return at_ERR_HELP;
		at_printf("\n<%d>\n", (g_atParam.flags & CFG_RECONN) ? 1 : 0);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_omcfgon
 *
 *  \brief:    start omniconfig
 *  \operator: "="set
 *  \param 0:  timeout seconds
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_omcfgon(int argc, char *argv[], char *op[])		//womcfgon
{
	int sec = 360;	/* 360 sec (6min)*/
	if (!strcmp(op,"=")) {
		if (argc > 1)
			return at_ERR_HELP;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;
		sec = atoi(argv[0]);
	} else if (strcmp(op,NUL)) {
		return at_ERR_OP;
	}
#ifdef CONFIG_OMNICONFIG
	omnicfg_start(sec);
#endif
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_omcfgoff
 *
 *  \brief:    stop omniconfig
 *  \operator: NULL execute
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_omcfgoff(int argc, char *argv[], char *op[])	//womcfgoff
{
	if (strcmp(op,NUL))
		return at_ERR_OP;
#ifdef CONFIG_OMNICONFIG
	omnicfg_stop();
#endif
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_apup
 *
 *  \brief:    start softap
 *  \operator: "="set, "?"query the current setting
 *  \param 0:  ssid
 *  \param 1:  pwd
 *  \param 2:  ch
 *  \param 3:  [max connect]
 *  \param 4:  [ssid hidden]
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_apup(int argc, char *argv[], char *op[])
{
	char *ssid, *pwd;
	int ch, enc;
	softap_param *ap_ptr;

	ap_ptr = &g_atParam.ap_param;
	if (!strcmp(op,"=")) {
		int len;

		if (argc < 3)
			return at_ERR_HELP;

		len = strlen(argv[0]);
		if ((len == 0) || (len > sizeof(g_atParam.ap_param.ssid)))
			return at_ERR_PARM;

		len = strlen(argv[1]);
		if (len > sizeof(g_atParam.ap_param.key))
			return at_ERR_PARM;

		ch = atoi(argv[2]);
#if defined(CONFIG_CH14)
		if ((ch <= 0) || (ch > 14))
#else
		if ((ch <= 0) || (ch > 13))
#endif
			return at_ERR_PARM;

		ssid = argv[0];
		pwd = argv[1];
		strcpy(ap_ptr->ssid, ssid);
		strcpy(ap_ptr->key, pwd);
		ap_ptr->channel = ch;
		if (argc == 4)
			ap_ptr->max_con = atoi(argv[3]);
		if (argc == 5)
			ap_ptr->hidden_ssid = atoi(argv[4]);

		save_at_param();
		if (g_atParam.work_mode == OPMODE_STA) {
			serial_printf("<%d>: invalid work mode\n",
							g_atParam.work_mode);
			return at_ERR_HELP;
		}
		if (ap_start_func)
			ap_start_func(ssid, pwd, ch);
	}
	else if (!strcmp(op,"=?"))
	{
		at_printf("\n<ssid:%s><pwd:%s><ch:%d><max_conn:%d><ssid hidden:%d>\n",
						ap_ptr->ssid,
						ap_ptr->key,
						ap_ptr->channel,
						ap_ptr->max_con,
						ap_ptr->hidden_ssid);
	}
	else
	{
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_apdn
 *
 *  \brief:    Stop softap
 *  \operator: NULL execute
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_apdn(int argc, char *argv[], char *op[])
{
	if (strcmp(op,NUL))
		return at_ERR_OP;
#if defined(CONFIG_AP)
	wlan_suspend_softap();
#endif
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_link
 *
 *  \brief:    Query the current Wi-Fi link status.
 *  \operator: "=?"query the current setting
 *  \param 0:  Wi-Fi interface
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_link(int argc, char *argv[], char *op[])		//wlink
{
	if (!strcmp(op,"=?")) {
		link_sts linkStatus;

		if (argc != 1)
			return at_ERR_HELP;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;

		if (wlan_get_link_sts(&linkStatus, atoi(argv[0])) < 0)
			return at_ERR_PARM;
		else
			at_printf("\n<%d>\n", linkStatus.is_connected);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_sta
 *
 *  \brief:    show the station info which connect this ap
 *  \operator: NULL execute
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_sta(int argc, char *argv[], char *op[])
{
	if (strcmp(op,NUL))
		return at_ERR_OP;
	wlan_get_sta_info(net_get_client_info);
	return at_ERR_OK;
}

#if 0
/*!-----------------------------------------------------------------------------
 * function: at_wifi_wps
 *
 *  \brief:    start or stop wps
 *  \operator: "="set, "
 *  \param 0:  enable or disable
 *  \return:   check result
 +----------------------------------------------------------------------------*/
//extern int wlan_start_wps(int timeout);
//extern int wlan_stop_wps(void);
int at_wifi_wps(int argc, char *argv[], char *op[])		//wreconn
{
	if (!strcmp(op,"="))
	{
		if (argc != 1)
		{
			return at_ERR_HELP;
		}
		if(atoi(argv[0]) == 1)
		{
			//wlan_start_wps(3000);
			serial_printf("\n <%s> start\n",__FUNCTION__);
		}
		else if(atoi(argv[0]) == 0)
		{
			//wlan_stop_wps();
			serial_printf("\n <%s> stop\n",__FUNCTION__);
		}
		else
		{
			return at_ERR_PARM;
		}
	}
	else
	{
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_wifi_mdns
 *
 *  \brief:    start or stop the mdns
 *  \operator: "="set, "
 *  \param 0:  enable or disable
 *  \param 1:  hostname
 *  \param 2:  server_name
 *  \param 3:  port
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_mdns(int argc, char *argv[], char *op[])
{
	if (!strcmp(op,"="))
	{
		serial_printf("\n <%s> :<%d><%s><%s><%d>\n",
						__FUNCTION__,atoi(argv[0]),argv[1],argv[2],atoi(argv[3]));
	}
	else
	{
		return at_ERR_OP;
	}
	return at_ERR_OK;
}
#endif

/*!-----------------------------------------------------------------------------
 * function: at_wifi_sleep
 *
 *  \brief:    Set sleep mode
 *  \operator: "="set, "?"query the current setting
 *  \param 0:  0:disable; 1:light sleep; 2:deep sleep
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_wifi_sleep(int argc, char *argv[], char *op[])		//wsleep
{
	if (!strcmp(op,"=")) {
		link_sts linkStatus;
		int val;

		if (argc != 1)
			return at_ERR_HELP;

		val = atoi(argv[0]);
		if ((val < 0) || (val > 2))
			return at_ERR_PARM;

		if (g_atParam.ps_mode == val)
			return at_ERR_OK;

		if (val == 2) {			// deep sleep
			wlan_power_off();
			at_printf("caution! disable deep sleep before Wi-Fi operations\n");
		} else if (val == 1) {	// light sleep
			if (wlan_enable_powersave() < 0) {
				at_printf("err! deep sleep is active\n");
				return at_ERR_OK;
			}
		} else if (val == 0) {	// no power-saving
			wlan_power_on();
			wlan_disable_powersave();
		}
		g_atParam.ps_mode = val;
		save_at_param();
	} else if (!strcmp(op,"=?")) {
		if (argc != 0)
			return at_ERR_HELP;
		at_printf("\n<%d>\n", g_atParam.ps_mode);
	} else {
		return at_ERR_OP;
	}
	return at_ERR_OK;
}

