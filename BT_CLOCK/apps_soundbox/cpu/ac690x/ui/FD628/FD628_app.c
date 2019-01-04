/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf.cw@gmail.com
*@Brief
*@FileName		FD628_app.c
*@Data			2017/10/25 16:32:42
-------------------------------------------------------------------------*/
#include <sdk_cfg.h>
#include "ui/ui_api.h"
#if UI_ENABLE

#include "ui/ui_common.h"
#include "fm_radio.h"
#include "dec/decoder_phy.h"
#include "music_ui.h"
#include "music.h"
#include "timer.h"
#include "key_drv/key.h"
#include "file_operate/file_op.h"
//#include "play_file.h"
#include "dac/dac.h"
#include "rtc_setting.h"

#if REC_EN
    #include "encode/encode.h"
#endif
#if FD628_EN
#include <FD628_app.h>
#include "FD628_Drive.h"

t_FD628_param FD628Param;
#define HALF_SEC_FLASH_BUFF		FD628Param.buff[1]
#define COLON_FLASH_BIT			0x80

const u8 playmodestr[][5] =
{
    " ALL",
    "Fold",
    " ONE",
    " rAn",
};

const u8 menu_string[][5] =
{
    " HI ",
    "Lod ",
    "bLUE",
    " PC ",
    " UP ",
    " dN ",
    " AUX",
    "-AL-"
};
const u8 other_string[][5]=
{
    "Eq ",
    "V ",
    "P ",
    " NOP",
    " rec",
};

/*   S E T _ B U F F _ P O S I T I O N   */
/*-------------------------------------------------------------------------
*@param
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief			reset buff position
-------------------------------------------------------------------------*/
void set_buff_position(u8 pos)
{
	FD628Param.buffIndex = pos;
}

/*   S H O W _ S T R I N G   */
/*-------------------------------------------------------------------------
*@param
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief
-------------------------------------------------------------------------*/
static void inline show_string(u8* str)
{
	u8 tmp = 0;
	/*check out of range */
	while(*str != '\0') {
		if (FD628Param.buffIndex > 7) {
			FD628Param.buffIndex = 0;
		}
//		printf("%s ",*str);
		FD628Param.buff[FD628Param.buffIndex++] = DispGetCode(*str++);
	}
	/* Debug msg */
	for (tmp = 0;tmp < FD628Param.buffIndex;tmp++) {
  		printf("buff[%d]= 0x%x!!!\n",tmp,FD628Param.buff[tmp]);
	}
}
static char show_char(u8 data)
{
	if (FD628Param.buffIndex > 6) {
		FD628Param.buffIndex = 0;
		return -1;
	}
	FD628Param.buff[FD628Param.buffIndex++] = data;
	return 1;
}

void FD628_show_string_menu(u8 menu)
{
    if(menu >= (sizeof(menu_string)/sizeof(u8) * 5))
    {
        printf("*strid(%d) is over!\n",menu);
    }
    else
    {
        show_string((u8 *)menu_string[menu]);
    }
}

void FD628_show_volume(s32 vol)
{
	u8 tmp_vol;

	tmp_vol = (u8)vol;
	show_string((u8 *)other_string[1]);
	itoa2(tmp_vol);
	show_string((u8 *)bcd_number);
}

void FD628_clear_icon(void)
{
	FD628Param.flashBit = 0;
}
static inline void FD628_show_dev(UI_DEVICE dev)
{
	;
}
void FD628_show_music_main(void)
{
    //led7_show_string((u8*)"MUSI");
    u32 play_time;
    MUSIC_DIS_VAR *music_var;

    music_var = (MUSIC_DIS_VAR*)UI_var.ui_buf_adr;

    if(music_var)
    {
        /*Music Play time info*/
        play_time = music_var->play_time;

        itoa2(play_time/60);
        show_string((u8 *)bcd_number);

        itoa2(play_time%60);
        show_string((u8 *)bcd_number);

        /* led7_show_dev(); */
		FD628_show_dev(music_var->ui_curr_device);
		/*TODO other data */
    }
}

void FD628_show_eq(s32 arg)
{
    u8 eq_cnt;
    eq_cnt = (u8)arg;
    show_string((u8 *)other_string[0]);
    show_char(eq_cnt % 10 + '0');
}

void FD628_show_filenumber(void)
{
    MUSIC_DIS_VAR *music_var;

    music_var = (MUSIC_DIS_VAR*)UI_var.ui_buf_adr;

    if(music_var)
    {
        /*Music File Number info*/
        //printf("ui_music.ui_play_file_num = %u----\n",ui_music.ui_play_file_num);
        itoa4(music_var->ui_curr_file);
        show_string((u8 *)bcd_number);
    }
}
void FD628_show_playmode(s32 arg)
{
    u8 pm_cnt;
    pm_cnt = (u8)arg;
    show_string((u8 *)&playmodestr[pm_cnt-FOP_MAX-1][0]);
}

void FD628_show_fm_main(void)
{
    /*FM - Frequency*/
 #if 1
    FM_MODE_VAR * fm_var;

    if(!UI_var.ui_buf_adr)
        return;

    fm_var = *(FM_MODE_VAR **)UI_var.ui_buf_adr;

    if(fm_var)
    {
        itoa4(fm_var->wFreq);

        if (fm_var->wFreq <= 999)
            bcd_number[0] = ' ';

        show_string((u8 *)bcd_number);

#if REC_EN
        RECORD_OP_API * rec_var_p;
        REC_CTL * rec_ctl_var;

        if((fm_var->fm_rec_op)&&(*(RECORD_OP_API **)(fm_var->fm_rec_op)))
        {
            rec_var_p = *(RECORD_OP_API **)fm_var->fm_rec_op;
            rec_ctl_var = rec_var_p->rec_ctl;

            if((rec_ctl_var)&&(ENC_STOP != rec_ctl_var->enable))
            {
				LED_STATUS &= ~(LED_PLAY | LED_PAUSE);
				LED_STATUS &= ~(LED_SD|LED_USB);

                /* led7_show_dev(); */
				FD628_show_dev(rec_ctl_var->curr_device);
                if (ENC_STAR == rec_ctl_var->enable)
                    LED_STATUS |= LED_PLAY;
                else if(ENC_PAUS == rec_ctl_var->enable)
                    LED_STATUS |= LED_PAUSE;
            }

        }
#endif
    }
#endif // 0
}

void FD628_show_fm_station(void)
{
//    /*FM - Station*/
#if 1
    FM_MODE_VAR * fm_var;

    if(!UI_var.ui_buf_adr)
        return;

    fm_var = *(FM_MODE_VAR **)UI_var.ui_buf_adr;

    if(fm_var)
    {
        show_string((u8 *)other_string[2]);
        itoa2(fm_var->wFreChannel);
        show_string((u8 *)bcd_number);
    }
#endif
}

#if	RTC_CLK_EN
/*   R   T   C _ S H O W _ N O R M A L   */
/*-------------------------------------------------------------------------
*@param
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief
-------------------------------------------------------------------------*/
u8 timeFormat = TIME_FORMAT_24_EN;
static void inline RTC_show_normal(RTC_TIME* ui_time)
{
	unsigned char hour = ui_time->bHour;
#if TIME_FORMAT
	if (timeFormat != TIME_FORMAT_24_EN && ui_time->bHour > 12) {
		hour = hour - 12;
	}
#endif
	itoa2(hour);
	show_string((u8 *)bcd_number);
	itoa2(ui_time->bMin);
	show_string((u8 *)bcd_number);
//	printf("half_sec=%d \n",get_sys_halfsec());
	if (get_sys_halfsec()) {
		HALF_SEC_FLASH_BUFF |= COLON_FLASH_BIT;
	} else {
		HALF_SEC_FLASH_BUFF &= ~(COLON_FLASH_BIT);
	}

}
/*   R   T   C _ S H O W _ S E T T I N G   */
/*-------------------------------------------------------------------------
*@param
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief
-------------------------------------------------------------------------*/
static void inline RTC_show_setting(RTC_SETTING* rtc_var,RTC_TIME* ui_time)
{
	switch (rtc_var->calendar_set.coordinate) {
		case RTC_YEAR_SETTING:
			printf("rtc setting year process!!! \n");
			itoa4(ui_time->dYear);
			show_string((u8* )bcd_number);
			if (get_sys_halfsec()) {
				set_buff_position(0);
				itoa4(0);
				show_string((u8* )bcd_number);
			}
			break;
		case RTC_MONTH_SETTING:
			printf("month setting process!!! \n");
			itoa2(ui_time->bMonth);
			show_string((u8* )bcd_number);
			itoa2(ui_time->bDay);
			show_string((u8* )bcd_number);
			if (get_sys_halfsec()) {
				set_buff_position(0);
				itoa2(0);
				show_string((u8* )bcd_number);
			}
			break;
		case RTC_DAT_SETTING:
			printf("day setting process!!! \n");
			itoa2(ui_time->bMonth);
			show_string((u8* )bcd_number);
			itoa2(ui_time->bDay);
			show_string((u8* )bcd_number);
			if (get_sys_halfsec()) {
				set_buff_position(2);
				itoa2(0);
				show_string((u8* )bcd_number);
			}
			break;
		case RTC_HOUR_SETTING:
			printf("hour setting process!!! \n");
			itoa2(ui_time->bHour);
			show_string((u8* )bcd_number);
			itoa2(ui_time->bMin);
			show_string((u8* )bcd_number);
			if (get_sys_halfsec()) {
				set_buff_position(0);
				itoa2(0);
				show_string((u8* )bcd_number);
			}
			break;
		case RTC_MIN_SETTING:
			printf("min setting process!!! \n");
			itoa2(ui_time->bHour);
			show_string((u8* )bcd_number);
			itoa2(ui_time->bMin);
			show_string((u8* )bcd_number);
			if (get_sys_halfsec()) {
				set_buff_position(2);
				itoa2(0);
				show_string((u8* )bcd_number);
			}
			break;
	}
}

/*   F   D 6 2 8 _ S H O W _   R   T   C _ M A I N   */
/*-------------------------------------------------------------------------
*@param
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief
-------------------------------------------------------------------------*/
void FD628_show_RTC_main(void)
{
    RTC_SETTING * rtc_var;
    RTC_TIME *ui_time;

    rtc_var = (RTC_SETTING *)UI_var.ui_buf_adr;
    if(rtc_var)
    {
        ui_time = rtc_var->calendar_set.curr_rtc_time;
		if (rtc_var->rtc_set_mode == RTC_DISPLAY) {
			RTC_show_normal(ui_time);
		} else if (rtc_var->rtc_set_mode == RTC_SET_MODE) {
			RTC_show_setting(rtc_var,ui_time);
		}
	}else {
		printf("NULL POINTER!!! \n");
	}
}

/*----------------------------------------------------------------------------*/
/**@brief   Alarm 显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void led7_show_alarm(void)
*/
/*----------------------------------------------------------------------------*/
#if RTC_ALM_EN

static void inline alarm_setting_mode(RTC_TIME* ui_time,RTC_SETTING* rtc_var)
{
	switch (rtc_var->alarm_set[rtc_var->alarmIndex].coordinate){
		case ALARM_NUM_INDEX:
			printf("setting alarm num process!!! \n");
			break;
		case RTC_HOUR_SETTING:
			printf("setting hour process!!! \n");

			break;
		case RTC_MIN_SETTING:
			printf("setting min process!!! \n");
			break;
		default:
			break;
	}
}
void FD628_show_alarm(void)
{
    RTC_SETTING * rtc_var;
    RTC_TIME* ui_time;
	rtc_var = (RTC_SETTING *)UI_var.ui_buf_adr;
	if (rtc_var != NULL) {
		ui_time = rtc_var->alarm_set[rtc_var->alarmIndex].curr_alm_time;
#if ALARM_NUM_THREE
		alarm_setting_mode(ui_time,rtc_var);
#endif
	}
}
#endif
#endif
//do not open it current
#if REC_EN
#if 0

/*----------------------------------------------------------------------------*/
/**@brief   REC 显示界面
   @param   void
   @return  void
   @note    void led7_show_rec_start(void)
*/
/*----------------------------------------------------------------------------*/
static void FD628_show_rec_start(REC_CTL * rec_ctl_var)
{
    u32 rec_time;

//    LED_STATUS &= ~(LED_PLAY | LED_PAUSE);
//    if(rec_ctl_var)
    {
        rec_time = rec_ctl_var->file_info.enc_time_cnt;

        itoa2(rec_time/60);
        show_string((u8 *)bcd_number);

        itoa2(rec_time%60);
        show_string((u8 *)bcd_number);

        /* led7_show_dev(); */
		FD628_show_dev(rec_ctl_var->curr_device);

        LED_STATUS |= LED_2POINT; //| LED_PLAY;

        if (ENC_STAR == rec_ctl_var->enable)
            LED_STATUS |= LED_PLAY;
        else if (ENC_PAUS == rec_ctl_var->enable)
            LED_STATUS |= LED_PAUSE;
    }
}

/*----------------------------------------------------------------------------*/
/**@brief   REC 显示界面
   @param   void
   @return  void
   @note    void led7_show_rec_main(void)
*/
/*----------------------------------------------------------------------------*/
void FD628_show_rec_main(void)
{
    RECORD_OP_API * rec_var_p;
    REC_CTL * rec_ctl_var;

    LED_STATUS &= ~(LED_PLAY | LED_PAUSE);
    LED_STATUS &= ~LED_2POINT; //| LED_PLAY;
    LED_STATUS &= ~(LED_SD|LED_USB);

    if(UI_var.ui_buf_adr)
    {
        rec_var_p = *(RECORD_OP_API **)UI_var.ui_buf_adr;
        if(rec_var_p)
        {
            rec_ctl_var = rec_var_p->rec_ctl;
            if((rec_ctl_var)&&(ENC_STOP != rec_ctl_var->enable))
            {
                FD628_show_rec_start(rec_ctl_var);
                return;
            }
        }
    }

    show_string((u8 *)other_string[4]);
}
#endif
#endif

void FD628_show_linin_main(u8 menu)
{
    FD628_show_string_menu(menu);

#if REC_EN

    RECORD_OP_API * rec_var_p;
    REC_CTL * rec_ctl_var;

    rec_var_p = *(RECORD_OP_API **)UI_var.ui_buf_adr;
    if((UI_var.ui_buf_adr)&&(rec_var_p))
    {
        rec_ctl_var = rec_var_p->rec_ctl;

        LED_STATUS &= ~(LED_PLAY | LED_PAUSE);
        LED_STATUS &= ~(LED_SD|LED_USB);

        if((menu == MENU_AUX_MAIN)||(menu == MENU_BT_MAIN))
        {
            if((rec_ctl_var)&&(ENC_STOP !=rec_ctl_var->enable))
            {
                if (ENC_STAR == rec_ctl_var->enable)
                    LED_STATUS |= LED_PLAY;
                else if(ENC_PAUS == rec_ctl_var->enable)
                    LED_STATUS |= LED_PAUSE;

				FD628_show_dev(rec_ctl_var->curr_device);
            }
        }
    }
#endif
}
static void inline FD628_clear()
{
	unsigned char buff[14] = {0,};
	FD628_write_string(buff);
}
void FD628_scan(void *param)
{

	unsigned char writeBuff[14] = {0,},buffIndex = 0;
	for (buffIndex = 0;buffIndex < 7;buffIndex++) {
		writeBuff[buffIndex * 2] = FD628Param.buff[buffIndex];
	}
	for (buffIndex = 1;buffIndex < 14;buffIndex++) {
		writeBuff[buffIndex] = FD628Param.segBuff[buffIndex / 2];
		buffIndex += 2;
	}
    FD628_clear();
//	FD628_write_string(writeBuff);
}

static void inline param_init()
{
	FD628Param.brightness = 5;
	FD628Param.buffIndex = 0;
	FD628Param.flashBit = 0;
}
void FD628_init(void)
{
	param_init();
    s32 ret;
	/* Register UI display callback function */
	ret = timer_reg_isr_fun(timer0_hl,1,FD628_scan,NULL);
	if(ret != TIMER_NO_ERR)
	{
		printf("FD628_scan err = %x\n",ret);
	}
}
#endif
#endif
