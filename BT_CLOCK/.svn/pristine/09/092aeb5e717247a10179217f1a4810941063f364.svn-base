/*-------------------------------------------------------------------------
*@Author			Jerry
*@Email			Zwxf.cw@gmail.com
*@Brief			
*@FileName		FD628_app.h
*@Data			2017/10/27 13:33:25
-------------------------------------------------------------------------*/
#ifndef FD628_APP_H
#define FD628_APP_H

#define MAX_BUFF_SIZE		7
typedef struct {
	unsigned char buffIndex;
	char buff[MAX_BUFF_SIZE];
	/* Left 7segs and each segs have two bits to operate;BIT0 BIT1 */
	char segBuff[MAX_BUFF_SIZE];
	unsigned char brightness;
	unsigned char flashBit;	
}t_FD628_param;

void FD628_init(void);
void FD628_scan(void *param);
void FD628_show_linin_main(u8 menu);
void FD628_show_rec_main(void);
void FD628_show_alarm(void);
void FD628_show_RTC_main(void);
void FD628_show_fm_station(void);
void FD628_show_fm_main(void);
void FD628_show_playmode(s32 arg);
void FD628_show_filenumber(void);
void FD628_show_eq(s32 arg);
void FD628_show_music_main(void);
void FD628_clear_icon(void);
void FD628_show_volume(s32 vol);
void FD628_show_string_menu(u8 menu);
void set_buff_position(u8 pos);
extern u8 timeFormat;
#define time_format_switch() 	(timeFormat = ~timeFormat)

#endif
