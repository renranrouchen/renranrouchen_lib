#ifndef MSG_H
#define MSG_H

#include "typedef.h"

#define NO_MSG 0xff

enum
{
    MSG_0 = 0,
    MSG_1,
    MSG_2,
    MSG_3,
    MSG_4,
    MSG_5,
    MSG_6,
    MSG_7,
    MSG_8,
    MSG_9,
    MSG_ALM_ON,
    /* 24/12 time format config */
    MSG_TIME_FORMAT,
	MSG_VOL_UP,
	MSG_VOL_DOWN,
    MSG_VOL_STEREO,
	MSG_MUTE,
	MSG_POWER_OFF,
	MSG_POWER_OFF_HOLD,
	MSG_POWER_KEY_UP,
	MSG_POWER_OFF_KEY_MSG,
	MSG_POWER_ON,
    MSG_UPDATA,
	MSG_LOW_POWER,
	MSG_LOW_POWER_VOICE,

	///-------work_mode---------------
	MSG_CHANGE_WORKMODE,
	MSG_NEXT_WORKMODE,
	MSG_PRE_WORKMOD,
    MSG_LAST_WORKMOD,

    // UI MAIN
    MSG_ENTER_MENUMAIN,
	MSG_ENTER_SLEEP_MODE,
    MSG_APP_MAIN_MSGLIST,

	MSG_VOCAL_PP,
    ///-------main_msg，此消息前的消息发送到MAIN线程---------------
    MSG_MAIN_MAX,

	///-------music app---------------
	MSG_MUSIC_NEXT_FILE = 0x20,
    MSG_MUSIC_PREV_FILE,
    MSG_MUSIC_PP,
    MSG_MUSIC_PLAY,
    MSG_MUSIC_PAUSE,
    MSG_MUSIC_STOP,
    MSG_MUSIC_FF,
    MSG_MUSIC_FR,
    MSG_MUSIC_FFR_DONE,
    MSG_MUSIC_RPT,
    MSG_MUSIC_EQ,
    MSG_MUSIC_U_SD,
    MSG_MUSIC_DEL_FILE,
    MSG_MUSIC_SPC_FILE,
    MSG_MUSIC_AB_RPT,
    MSG_MUSIC_PLAY_SN,

    ///-------FM app---------------
    MSG_FM_SCAN_ALL_INIT = 0x40,
    MSG_FM_INIT,
	MSG_FM_SCAN_ALL,
	MSG_FM_PREV_STATION,
	MSG_FM_NEXT_STATION,
	MSG_FM_SCAN_ALL_DOWN,
	MSG_FM_SCAN_ALL_UP,
	MSG_FM_PREV_STEP,
	MSG_FM_NEXT_STEP,
	MSG_SRC,
	MSG_FM_SELECT_CHANNEL,
    MSG_FM_PP,
    MSG_FM_DEL_CHANNEL,
    MSG_FM_SAVE_CHANNEL,
    MSG_FM_SELECT_FREQ,
    //----Reset FM Voice
    MSG_FM_RESET_VOL,
    ///-------BT app---------------
    MSG_BT_NEXT_FILE = 0x50,
    MSG_BT_PREV_FILE,
    MSG_BT_PP,
    MSG_BT_CALL_LAST_NO,
    MSG_BT_CALL_CONTROL,
    MSG_BT_CALL_REJECT,
    MSG_BT_ANSWER_CALL,
    MSG_BT_SHUTTER,
    MSG_BT_RESET,
    MSG_BT_STACK_STATUS_CHANGE,
	MSG_TEST_HFP,
    MSG_DISCONNECT_ALL_LINK,
    MSG_BT_CONNECT_CTL,
    MSG_BT_CONNECT_STEREO_CTL,
    MSG_BT_HID_CTRL,
    MSG_BT_HID_TAKE_PIC,
    MSG_BT_MUSIC_EQ,
    MSG_BT_STEREO_EQ,
    MSG_BT_SYNC_STEREO_EQ,
    MSG_BT_PAGE_SCAN,
	MSG_BT_SPP_UPDATA,
    MSG_BT_STEREO_SEARCH_DEVICE,
	MSG_DISP_MENU,///自动刷新界面
	MSG_BT_FAST_TEST,
	MSG_BT_TWS_DELETE_ADDR,

    ///-------AUX app---------------
    MSG_AUX_INIT = 0x70,
    MSG_AUX_MUTE,

	MSG_ECHO_SET_PARM,
	MSG_PITCH_SET_PARM,

	///htk
    MSG_HTK_LOOP_CMD,

    ///-------PC app---------------
    MSG_PC_MODE_INIT = 0x80,
    MSG_PC_CARD_READER,
    MSG_PC_SPK_MUTE,
    MSG_PC_SPK_UNMUTE,
    MSG_PC_SPK_VOL,
    MSG_PC_MUTE,
    MSG_PC_VOL_UP,
    MSG_PC_VOL_DOWN,
    MSG_PC_PP,
    MSG_PC_PLAY_NEXT,
    MSG_PC_PLAY_PREV,
    MSG_PC_UPDATA,
    MSG_PC_SPK_PP,

    ///-------RTC app---------------
    MSG_RTC_INIT = 0x90,
    MSG_RTC_SETTING,
    MSG_ALM_SETTING,
    MSG_ALM_SW,
//    MSG_ALM_ON,
    MSG_RTC_PLUS,
    MSG_RTC_MINUS,
    MSG_RTC_DISPLAY_MODE,
    MSG_RTC_POWER_DOWN,

	/* KEYI rtc setting msg */
	MSG_TIME_SETTING,
    ///-------ENCODE app---------------
    MSG_REC_INIT,
    MSG_REC_START,
    MSG_REC_STOP,
    MSG_REC_PP,
    MSG_REC_PLAY,
    MSG_ENCODE_ERR,

    MSG_ECHO_START,
    MSG_ECHO_STOP,

    ///-------IDLE app--------------
    MSG_IDLE_MODE,
    MSG_IDLE_CTL,
    MSG_ENTER_IDLE_TASK,
    MSG_EXIT_IDLE_TASK,
    MSG_IDLE_CNT,
    MSG_IDLE_CNT_LONG,
    MSG_IDLE_CNT_HOLD,
    MSG_IDLE_CNT_UP,

	MSG_PROMPT_PLAY,

    MSG_INPUT_TIMEOUT,
    MSG_HALF_SECOND,//0xab
    MSG_ENTER_MENULIST,
    MSG_APP_MSGLIST,
	MSG_DAC_ON,
	MSG_DAC_OFF,

	//解码器消息
	SYS_EVENT_BEGIN_DEC = 0xB0,
    SYS_EVENT_DEC_FR_END,
    SYS_EVENT_DEC_FF_END,
    SYS_EVENT_DEC_END,
    SYS_EVENT_DEC_DEVICE_ERR,

    SYS_EVENT_DEC_SET_END,

    SYS_EVENT_PLAY_SEL_END,

	MSG_NULL = 0xff,
};

enum{
//	SYS_MSG_0 = 0x40,

	SYS_EVENT_DEV_ONLINE = 0xc0,
	SYS_EVENT_DEV_OFFLINE,
    SYS_EVENT_LGDEV_ONLINE,
    SYS_EVENT_LGDEV_OFFLINE,
    SYS_EVENT_SYSCLK_NOTFIT_DEVMOUNT,   ///<当前系统时钟不满足设备枚举需求，收到此消息，切换进music
	SYS_EVENT_AUX_IN,           ///<AUX插入
	SYS_EVENT_AUX_OUT,          ///<AUX拔出
	SYS_EVENT_PC_IN,            ///<PC 插入
	SYS_EVENT_PC_OUT,           ///<PC 拔出
	SYS_EVENT_DECODE_END,
	SYS_EVENT_STORE_BP,
	SYS_EVENT_RESTORE_BP,
	SYS_EVENT_DEL_TASK,
	SYS_EVENT_TONE_END,
	SYS_EVENT_RES_MANAGE_RUN,
	SYS_EVENT_TASK_EXIT_REQ,
	SYS_EVENT_TASK_RUN_REQ,
    SYS_EVENT_BT_BACKCON,       ///<蓝牙在后台才连接成功，由用户选择处理
	SYS_EVENT_BT_BACKDISCON,    ///<蓝牙在后台断开成功，由用户选择处理
	SYS_EVENT_DEV_BEGIN_MOUNT,//oxd3
	SYS_EVENT_DEV_MOUNT_ERR,
 	SYS_EVENT_SYS_TIMER_DELAY,
};

enum
{
    MSG_RSCP_RECIEVE_DATA = 0x3001,    //
    MSG_RSCP_RECIEVE_WAIT,    //

	//pack send
    MSG_RSCP_CHANGE_WORDMODE_ACTION,
    MSG_RSCP_REPORT_FRAME,
    MSG_RSCP_REPORT_CTL_STATE,
    MSG_RSCP_REPORT_LIGHT_STATE,
    MSG_RSCP_REPORT_LONGFRAME,
    MSG_RSCP_REPORT_ID3_INFO,
    MSG_RSCP_SEND_OPT_DATA,
    MSG_RSCP_SEND_OPT_DATA_NO_ACK,
    // MSG_RSCP_REPORT_MODE_INFO,

};


struct event_head {
	const char *str;
	int len;
	char data[0];
};

#endif



