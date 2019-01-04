/*-------------------------------------------------------------------------
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief
*@FileName		keyi_rtc_app.c
*@Data			2017/11/9 17:04:56
-------------------------------------------------------------------------*/
#include "dac/dac.h"
#include "keyi_rtc_app.h"
#include "ui/led/led.h"
#include "rtc_ui.h"
#include "sys_detect.h"
#include "timer.h"
#include <keyi_rtc_setting.h>
#include <rtc/rtc_api.h>

#if (PRODUCT_VERSION_KEYI == 1)
rtc_time_t rtcTime;
alarm_time_t alarmTime;
volumn_display_config_t volumnDisplayConfig;

static void inline set_alarm_sound_count2zero(void)
{
	timeSetting.alarmConfig.alarmSoundCount = 0;
}
/*   C A L L _ D I S P L A Y _ R T C _ T I M E   */
/*-------------------------------------------------------------------------
*@param
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief
-------------------------------------------------------------------------*/
void call_display_rtc_time(void)
{
    if (volumnDisplayConfig.volumnDisplayFlag == true) {
        if (volumnDisplayConfig.volumnDisplayDelay++ < MAX_VOLUMN_DISPLAY_TIME) {
            volumnDisplayConfig.volumnDisplayDelay = 0;
            volumnDisplayConfig.volumnDisplayFlag = false;
        }
    } else {
        ui_open_rtc(&timeSetting, sizeof(time_setting_t));
        read_rtc_time(&rtcTime);
        //fixme
        printf("%s,%d,rtcTime %d-%d-%d, %d:%d:%d\n", __FUNCTION__, __LINE__, rtcTime.year, rtcTime.month, rtcTime.day, rtcTime.hour, rtcTime.min, rtcTime.second);
        SET_UI_MAIN(MENU_RTC_MAIN);
        UI_menu(MENU_RTC_MAIN);
    }
}


static unsigned char min = 0;
static unsigned char hour = 0;
/*   R E S E T _ A L A R M _ C H E C K _ T I M E _ D E F A U L T   */
/*-------------------------------------------------------------------------
*@param
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief			reset alarm defualt time
-------------------------------------------------------------------------*/
void reset_alarm_check_time_default()
{
	min = alarmTime.min;
	hour = alarmTime.hour;
}

/*   S E T _ S N O O Z E _ U P   */
/*-------------------------------------------------------------------------
*@param
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief			check alarm overflow
-------------------------------------------------------------------------*/
static void set_snooze_up(void)
{
/* Snooze time 5 mins */
#define SNOOZE_TIME		5
	if (timeSetting.alarmConfig.snoozeTimes++ < MAX_SNOOZE_TIME) {
		if (min + SNOOZE_TIME > 59) {
			min += SNOOZE_TIME;
			min -= 60;
			if (hour + 1 > 23) {
				hour = 0;
			}
		} else {
			min += SNOOZE_TIME;
		}
		timeSetting.alarmConfig.snoozeState = true;
	} else {
		/* reset alarm time to default */
		timeSetting.alarmConfig.snoozeState = false;
		reset_alarm_check_time_default();
		timeSetting.alarmConfig.snoozeTimes = 0;
	}

}
/*   R T C _ I S R _ U S E R _ H A N D L E R   */
/*-------------------------------------------------------------------------
*@param
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief			process alarm msg
-------------------------------------------------------------------------*/
void alarm_isr_user_handler(void* argc)
{
	if (timeSetting.alarmConfig.alarmSwitch == true) {
		/* rtc time equal to alarm time & rtc time is first second now */
		if (rtcTime.hour == hour && rtcTime.min == min && rtcTime.second == 0) {
			timeSetting.alarmConfig.alarmUpFlag	= true;
			/* reset snooze state to false */
			timeSetting.alarmConfig.snoozeState = false;
		}
//		printf("rtcTime.hour=%d rtcTime.min=%d,hour=%d,min=%d",rtcTime.hour,rtcTime.min,hour,min);
	} else {
		reset_alarm_check_time_default();
		timeSetting.alarmConfig.alarmUpFlag = false;
//		printf("test------------------>\n");
	}
}
/*   G E T _ A L A R M _ P A R A M   */
/*-------------------------------------------------------------------------
*@param
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief			get alarm msg from flash
-------------------------------------------------------------------------*/
static void get_alarm_param(alarm_time_t* curr_alm)
{
	alarm_time_t test;
	unsigned char flag;
	//rtc_module_read_alarm_datetime(&test);
	read_alarm_time(&test);
	curr_alm->hour = test.hour;
	curr_alm->min  = test.min;
	//fixme
	printf("%s, read_alarm_time, %d:%d\n", __FUNCTION__, test.hour, test.min);

	flag = read_alarm_flag();
	if (flag == 0xaa)
    {
        alarm_switch(true);
    }
    else
    {
        alarm_switch(false);
    }

    printf("%s, read_alarm_flag %d\n", __FUNCTION__, flag==0xaa?1:0);
}
/*----------------------------------------------------------------------------*/
/**@brief 	复位RTC时间
   @param 	void
   @return  void
   @note  	void reset_rtc(void)，根据当前时钟有效值，决定是否复位时钟
*/
/*----------------------------------------------------------------------------*/
static void reset_rtc(rtc_time_t *curr_time,alarm_time_t *curr_alm)
{
	start_rtc();
	read_rtc_time(curr_time);
	get_alarm_param(curr_alm);
    timeSetting.currTime = curr_time;
	timeSetting.alarmConfig.currAlarm = curr_alm;
	//fixme
	printf("%s, curr_time %d-%d-%d, %d:%d:%d\n", __FUNCTION__, curr_time->year, curr_time->month, curr_time->day, curr_time->hour, curr_time->min, curr_time->second);
	/*TODO:init process
	*add get flag from flash
	*/
	timeSetting.alarmConfig.alarmUpFlag = false;
	//timeSetting.alarmConfig.alarmSwitch = false;
	timeSetting.alarmConfig.enterAlarmSettingFlag = false;
	set_alarm_sound_count2zero();
	timeSetting.alarmConfig.snoozeTimes = 0;
	reset_alarm_check_time_default();
}


/****************************************************
    note        :open rtc hardware
******************************************************/

void rtc_hw_init(void)
{
   /* PORTR_PU(PORTR0 , 0);
	PORTR_PU(PORTR0 , 0);
	PORTR_DIR(PORTR0 , 1);
	PORTR_DIE(PORTR0 , 1);*/
//    rtc_module_on(WAKE_UP_ENABLE , 0 ,  rtc_isr_user_handler);
    memset(&rtcTime, 0, sizeof(rtc_time_t));
    memset(&alarmTime, 0, sizeof(rtc_time_t));
	timer_reg_isr_fun(timer0_hl,225,alarm_isr_user_handler,NULL);
    reset_rtc(&rtcTime,&alarmTime);
}


/*----------------------------------------------------------------------------*/
/**@brief  RTC DAC通道选择，开启
   @param  无
   @return 无
   @note   void aux_dac_channel_on(void)
*/
/*----------------------------------------------------------------------------*/
static void rtc_dac_channel_on(void)
{
    dac_mute(1,1);
    dac_channel_on(RTC_CHANNEL, FADE_ON);
    os_time_dly(15);  //这个参数会影响到上电第一次进入line in的冲击声。可以根据样机调整
    dac_mute(0,1);

    set_sys_vol(dac_ctl.sys_vol_l , dac_ctl.sys_vol_r, FADE_ON);
}

/*----------------------------------------------------------------------------*/
/**@brief  RTC DAC通道选择，关闭
   @param  无
   @return 无
   @note   void aux_dac_channel_off(void)
*/
/*----------------------------------------------------------------------------*/
static void rtc_dac_channel_off(void)
{
    dac_channel_off(RTC_CHANNEL, FADE_ON);
    dac_mute(0,1);
}
/*----------------------------------------------------------------------------*/
/**@brief  RTC初始化
   @param  void
   @return 无
   @note   void rtc_info_init(void)
*/
/*----------------------------------------------------------------------------*/
static void keyi_rtc_info_init(void)
{
    rtc_dac_channel_on();
    ui_open_rtc(&timeSetting, sizeof(time_setting_t));
}

static void keyi_stop_alm(void)
{
    timeSetting.alarmConfig.alarmUpFlag = false;
	set_alarm_sound_count2zero();
	SET_UI_MAIN(MENU_RTC_MAIN);
    UI_menu(MENU_RTC_MAIN);
}
/*----------------------------------------------------------------------------*/
/**@brief  RTC 任务
   @param  p：任务间参数传递指针
   @return 无
   @note   static void rtc_task(void *p)
*/
/*----------------------------------------------------------------------------*/
static void keyi_rtc_task(void *p)
{
    int msg[6];
    timeSetting.switchDdisplayFlag = 0;
    timeSetting.displayCount = 0;
	timeSetting.timeDisplayPosition = TIME_TIME_DISPLAY;
    //int rtc_init_flag = 0;
	rtc_puts("\n*********************RTC TASK********************\n");
    if(timeSetting.alarmConfig.alarmUpFlag)
    {
         puts("alarm start...\n");
         os_taskq_post_msg(RTC_TASK_NAME, 1, MSG_RTC_INIT);
    } else {
    	 puts("------->enter rtc<---------\n");
         tone_play_by_name(RTC_TASK_NAME,1,MY_RTC_MP3);
    }

    while(1)
    {
    	if (timeSetting.alarmConfig.enterAlarmSettingFlag == true) {
			timeSetting.alarmConfig.enterAlarmSettingFlag = false;
			printf("post message ................\n");
			os_taskq_post_msg(RTC_TASK_NAME, 1, MSG_TIME_SETTING);
		}
        memset(msg,0x00,sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        clear_wdt();
        if (msg[0] != MSG_HALF_SECOND)
        {
            rtc_printf("rtc_msg: = 0x%x\n",msg[0]);
        }

        if(timeSetting.alarmConfig.alarmUpFlag == true) {
            if ((msg[0]==MSG_CHANGE_WORKMODE)
                    ||(msg[0]==MSG_ALM_SW)
                    ||(msg[0]==MSG_RTC_PLUS)
                    ||(msg[0]==NO_MSG))
            {
                rtc_puts("stop alarm_ui\n");
				/* Not entry snooze state if push key during alarm state */
				timeSetting.alarmConfig.snoozeState = false;
                keyi_stop_alm();
                continue;
            }
            else
            {
            	;
			}
        }
        switch(msg[0])
        {
        case SYS_EVENT_DEL_TASK:
            if (os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
            {
                if (timeSetting.alarmConfig.alarmUpFlag == true)
                {
                    timeSetting.alarmConfig.alarmUpFlag = false;
                    rtc_puts("del alarm\n");
                }
                ui_close_rtc();
                rtc_puts("RTC_SYS_EVENT_DEL_TASK\n");
                rtc_dac_channel_off();
                play_sel_stop(); //关闭提示音
                os_task_del_res_name(OS_TASK_SELF);
            }
            break;

        case SYS_EVENT_PLAY_SEL_END: //提示音结束
            rtc_puts("RTC_SYS_EVENT_PLAY_SEL_END\n");
            play_sel_stop(); //关闭提示音
            rtc_puts("RTC_play_tone_end\n");
            os_taskq_post_msg(RTC_TASK_NAME, 1, MSG_RTC_INIT); //触发RTC模块初始化
            break;

        case MSG_RTC_INIT:
            rtc_puts("MSG_RTC_INIT\n");
            keyi_rtc_info_init();
            break;
		case MSG_TIME_SETTING:
			printf("MSG_TIME_SETTING \n");
			time_setting();
			break;
        case MSG_ALM_SW:
			printf("MSG_ALM_SW \n");
            if(timeSetting.alarmConfig.alarmSwitch == false)
            {
                alarm_switch(true);
            } else {
                alarm_switch(false);
            }
			UI_menu(MENU_ALM_SET);
            break;

        case MSG_RTC_POWER_DOWN:
			puts("enter low power\n");
//			set_keep_osci_flag(1);
//			soft_poweroff();
            break;

        case MSG_LOW_POWER:
            puts("rtc_low_power\n");
            //tone_play_by_name(RTC_TASK_NAME, BPF_LOW_POWER_MP3);
            break;
        case MSG_RTC_PLUS:
            puts("rtc_plus\n");
            timeSetting.displayCount = 0;
            if (timeSetting.switchDdisplayFlag == 0){
                timeSetting.switchDdisplayFlag = 1;
                timeSetting.timeDisplayPosition = TIME_DATE_DISPLAY;
            } else {
                if (++timeSetting.timeDisplayPosition >= TIME_DISPLAY_MAX)
                    timeSetting.timeDisplayPosition = TIME_DATE_DISPLAY;
            }
            break;
        case MSG_HALF_SECOND:
            if (timeSetting.alarmConfig.alarmUpFlag == true) {
            	if (timeSetting.alarmConfig.alarmSoundCount++ > MAX_ALARM_SOUND_COUNT) {
					/* Entry snooze state 60s later*/
					set_snooze_up();
					timeSetting.alarmConfig.alarmUpFlag = false;
					timeSetting.alarmConfig.alarmSoundCount = 0;
				}
                SET_UI_MAIN(MENU_ALM_UP);
                UI_menu(MENU_ALM_UP);
                puts("alarm ring\n");
                send_key_voice(500);
            } else {
                if (timeSetting.switchDdisplayFlag){
                    timeSetting.displayCount++;
                    if (timeSetting.displayCount > SETTING_TIME_MAX_COUNT){
                        timeSetting.displayCount = 0;
                        timeSetting.timeDisplayPosition = TIME_TIME_DISPLAY;
                        timeSetting.switchDdisplayFlag = 0;
                    }
                }
            	read_rtc_time(&rtcTime);

            	read_alarm_time(&alarmTime);
//                rtc_module_read_rtc_datetime(&current_time);
				/*	Display the RTC time 	*/
				SET_UI_MAIN(MENU_RTC_MAIN);
				UI_menu(MENU_RTC_MAIN);
//                UI_menu(MENU_HALF_SEC_REFRESH);
                rtc_printf("rtc time,%d-%d-%d, %d:%d:%d, alarm time %d:%d\n",rtcTime.year + 2000,rtcTime.month,rtcTime.day,\
                       rtcTime.hour,rtcTime.min,rtcTime.second, alarmTime.hour, alarmTime.min);
            }

            break;

        default:
            break;
        }
    }
}
/*----------------------------------------------------------------------------*/
/**@brief  RTC 任务创建
   @param  priv：任务间参数传递指针
   @return 无
   @note   static void rtc_task_init(void *priv)
*/
/*----------------------------------------------------------------------------*/
static void keyi_rtc_task_init(void *priv)
{
    u32 err;

    err = os_task_create(keyi_rtc_task, 0, TaskRTCPrio, 50, RTC_TASK_NAME);
    if(OS_NO_ERR == err)
    {
        key_msg_register(RTC_TASK_NAME, rtc_ad_table, rtc_io_table, rtc_ir_table, NULL);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  RTC 任务删除退出
   @param  无
   @return 无
   @note   static void rtc_task_exit(void)
*/
/*----------------------------------------------------------------------------*/
static void keyi_rtc_task_exit(void)
{
    if (os_task_del_req(RTC_TASK_NAME) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(RTC_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        }
        while(os_task_del_req(RTC_TASK_NAME) != OS_TASK_NOT_EXIST);
        rtc_puts("del_rtc_task: succ\n");
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  RTC 任务信息
   @note   const struct task_info linein_task_info
*/
/*----------------------------------------------------------------------------*/
TASK_REGISTER(keyi_rtc_task_info) =
{
    .name = RTC_TASK_NAME,
    .init = keyi_rtc_task_init,
    .exit = keyi_rtc_task_exit,
};

#endif
