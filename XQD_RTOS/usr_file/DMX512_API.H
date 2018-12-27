#ifndef _DMX512_API_H_
#define _DMX512_API_H_

#include "Event_process.h"

#define struct_func 0

typedef void (*color_display_cb)(void);

#if struct_func 
typedef struct color_setup_function 
{
	color_display_cb DM512_RED_DISPLAY_CB;
	color_display_cb DM512_GREEN_DISPLAY_CB;
	color_display_cb DM512_BLUE_DISPLAY_CB;
	color_display_cb DM512_WHITE_DISPLAY_CB;
	color_display_cb DM512_CRAN_DISPLAY_CB;
	color_display_cb DM512_PUPRLE_DISPLAY_CB;
	color_display_cb DM512_YELLOW_DISPLAY_CB;
	color_display_cb DM512_gradual_change_DISPLAY_CB;
	color_display_cb DM512_RACE_LAMP_DISPLAY_CB;
	color_display_cb DM512_Gamma_LAMP_DISPLAY_CB;
}color_setup_function_t;
#endif

void gradual_change_display(void);
void RACE_LAMP_fuc(void);
void DMX_color_display(unsigned char (*arg)[3],color_type type);
void color_setup(color_type type);
void DMX_Init(void);

#endif
