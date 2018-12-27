#ifndef _CONFIG_H
#define _CONFIG_H

typedef  unsigned char uchar;
#define		BS(SFR,NUM)		(SFR|=(1<<NUM))           
#define		BC(SFR,NUM)		(SFR&=~(1<<NUM))




#define  	isr_init 0
void gpio_config(void);
void led_init(void);
void protect_data_enble(void);
#if isr_init
void isr_init(void);
#endif
#endif