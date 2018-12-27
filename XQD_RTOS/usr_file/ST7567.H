#ifndef _ST7567_H_
#define _ST7567_H_
#include "M451Series.h"
	/******************************
	LCD_CSB PE0 24
	LCD_RES PC4 23
	LCD_A0  PD7 14
	LCD_SCK PC1 20
	LCD_SDA PC0 19
	
	******************************/

#define cs1  PE0	//L  input pin
#define reset  PC4	
#define rs   PD7 //a0 L dispaly_data; H command
#define sclk  PC1	
#define sid  PC0	
#define key  PD3	



void transfer_command(int data1) ;
void transfer_data(int data1);
void delay(int i) ;
void waitkey(void);
void initial_lcd(void);
void lcd_address(char page,char column);
void clear_screen(void);
void full_display(char data_left,char data_right);
void display_graphic_32x32(char page,char column,char *dp);
void display_graphic_16x16(char page,char column,const char *dp);
void display_graphic_8x16(char page,char column,const char *dp);
void display_string_8x16( int  page, int  column,const char *text);
void display_string_5x8( int  page, int  column,char *text);
void disp_grap(char *dp);
void display_graphic_16xn(char page,char column,const char *dp,int n,int m);
void display_nx16x16(char page,char column,char n,const char (*dp)[32]);
void display_num_8x16(int  page,int column,unsigned char num);
void display_num_string_8x16(int  page,int column,unsigned int num);
void display_Chinese_list(char page,char column,const char (*dp)[64],color_type type);

#endif
