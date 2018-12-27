/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*-----------------------------------------------------------------------------+
| Common Macros                                                                |
+-----------------------------------------------------------------------------*/

//for Boot from flash
//move the config flash memory start address to the last block of flash
#include <wifiClock_app/sdk_config.h>
#ifdef CONFIG_FLASH_BOOT
#define CFG_FLASH_MEM_START		0x70000
#else
#define CFG_FLASH_MEM_START		0x10000
#endif
#define CFG_FLASH_MEM_LENGTH	0x4000

#define CFG_MAGIC				{ 'M', 'C', 'F', 'G' }
#define CFG_CHKSUM_EN			(1<<31)
#define CFG_AUTO_CONN			(1<<30)
#define CFG_RECONN				(1<<29)
#define CFG_SAVE				(1<<28)
#define CFG_ALINK_DONE			(1<<27)
#define CFG_KONKE_DONE			(1<<26)


enum {
	TEMP_F = 0,
	TEMP_C = 1,
};
enum {
	BRIGHTNESS1 = 1,
	BRIGHTNESS2 = 2,
	BRIGHTNESS3 = 3,
	BRIGHTNESS4 = 4,
	BRIGHTNESS5 = 5,
	BRIGHTNESS6 = 6,
	BRIGHTNESS7 = 7,
	BRIGHTNESS8 = 8,
};

enum {
	FORMAT_24 = 0,
	FORMAT_12 = 1,	
};

#undef s8  // conflict with arch/cpu.h
typedef __signed char s8;
#undef u8
typedef unsigned char u8;
/*
typedef struct
{
	u8 enable;
	u8 duty;
}dc_moter_param;

typedef struct
{
	u8 enable;
	u8 angle;
}servo_moter_param;

typedef struct
{
	u8 enable;
	u8 r;
	u8 g;
	u8 b;
}rgbled_param;
*/
typedef struct
{
	long baudrate;
	unsigned char  databits;
	unsigned char  stopbits;
	unsigned char  parity;
	unsigned char  flowctrl;
}uart_param;

typedef struct
{
	char ssid[32];
	char key[64];
	char ip[16];		/**< Static IP configuration, Local IP address. */
	char mask[16];		/**< Static IP configuration, Netmask. */
	char gw[16];		/**< Static IP configuration, Router IP address. */
}sta_param;

typedef struct
{
	char ssid[32];
	char key[64];
	char channel;
	char enc;
	char max_con;
	char hidden_ssid;
	char ip[16];		/**< Static IP configuration, Local IP address. */
	char mask[16];		/**< Static IP configuration, Netmask. */
	char gw[16];		/**< Static IP configuration, Router IP address. */
}softap_param;

typedef struct
{
	unsigned char sta_mac[6];
	unsigned char ap_mac[6];
}mac_param;
enum {
	OPEN = 1,
};
typedef struct
{
	char startip[16];
	char endip[16];
	int leave_time;
}dhcps_param;
/*
typedef struct
{
	u8 enable;			// 0:cancel trans mode when start;1:start
	char ip[16];
	int port;
	char type[4];
	int time_alive;
}trans_param;
*/
typedef struct{
	u8 isOpen;
	char timeZone;
	/* update interver ,recalculate to secs */
	unsigned int updateInterver;
	/* update time ,recalculate to secs */
	unsigned int updateTime;
	/* FIXME:
	*current time ,recalculate to secs,remove for future 
	*/
	//int currentTime;
}webTime_param;
#if !VERSION_MUSHAN
#define ALARM_NUM	21
typedef struct {
	char alarmDay;
	char isOpen;
	int defaultAlarmTime;
	int alarmTime;
	char isOpenSnooze;
	int snoozeDelay;
}alarm_param;
#endif
#if VERSION_MUSHAN
typedef struct {
	char isOpenAlarm;
	unsigned int defaultAlarmTime;
}t_v2_alarm_param;
#endif

typedef struct {
	char isOpen;
	int cityId;
	/* remove for furture */
	//unsigned int updateTime;
	unsigned int updateInterval;
}weather_param;
typedef struct {
	char isOpen;
}nowtime_param;

typedef struct {
	char fmt;
}t_timeFmt;

typedef struct sdk {
	unsigned char cfg_magic[4];
	unsigned int flags;
	unsigned short chksum;
	u8 led_value;
	u8 work_mode;
	u8 dhcp_mode;
	u8 phy_mode;
	u8 txpower;
	u8 ps_mode;
	char temperature;
	u8 brightness;
//	servo_moter_param sm_param;
//	dc_moter_param dm_param;
//	rgbled_param ledrgb_param;
	mac_param mac;
	uart_param ur_param;
	sta_param sta_param[3];
	softap_param ap_param;
	dhcps_param dhs_param;
//	trans_param ts_param;
	webTime_param web_param;
#if !VERSION_MUSHAN
	alarm_param alm_param[ALARM_NUM];
#endif
#if VERSION_MUSHAN
	t_v2_alarm_param v2_alarm_param;
	char acid[30];
	char deviceId[30];
	unsigned char webRegisterStatue;
#endif
	unsigned char configMode;
	weather_param wth_parma;
	nowtime_param nt_param;
	t_timeFmt timeFmt_param;
}sdk_param;

/*-----------------------------------------------------------------------------+
| Function Prototypes                                                          |
+-----------------------------------------------------------------------------*/
int config_load(void);
int config_get(sdk_param *param);
int config_set(sdk_param *param);
int config_submit(void);
int config_erase(void);
void config_dump(void);


