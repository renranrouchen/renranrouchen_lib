#ifndef FD612_APP_H
#define FD612_APP_H
#define MAX_BUFF_SIZE		12
typedef struct {
	unsigned char buffIndex;
	char buff[MAX_BUFF_SIZE];
	unsigned char brightness;
	unsigned char flashBit;	
}t_FD612_param;

void FD612_init(void);
void FD612_scan(void *param);
void FD612_show_linin_main(u8 menu);
void FD612_show_rec_main(void);
void FD612_show_alarm(void);
void FD612_show_RTC_main(void);
void FD612_show_fm_station(void);
void FD612_show_fm_main(void);
void FD612_show_playmode(s32 arg);
void FD612_show_filenumber(void);
void FD612_show_eq(s32 arg);
void FD612_show_music_main(void);
void FD612_clear_icon(void);
void FD612_show_volume(s32 vol);
void FD612_show_string_menu(u8 menu);
void set_buff_position(u8 pos);
extern u8 timeFormat;
#define time_format_switch() 	(timeFormat = ~timeFormat)

#endif
