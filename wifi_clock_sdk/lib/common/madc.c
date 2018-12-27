/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/**
 * @file madc.c
 * @brief MADC APIs
 *
 */
#include <arch/ba22/chip.h>
#include <madc.h>


/**
 * MADC read digital data
 * @param pin select madc chan\n 0: CH I\n 1: CH Q
 * @return digital data(0 ~ 4095)
 */

#define MADC_CHI_OFF	(0x01 << 0)
#define MADC_CHQ_OFF	(0x01 << 1)
int analog_read(int pin)
{
	int data = 0;
	PMUREG(STDBY_PD_CTRL) &= ~STDBY_MADC_PD;//MADC power ON
	if(pin == 0) {
		MADCREG(0x00) &= ~(MADC_CHI_OFF);
	} else {
		MADCREG(0x00) &= ~(MADC_CHQ_OFF);
	}
	udelay(100);
	MADCREG(0x00) |= (1 << 3);
	while(MADCREG(0x00) & (1 << 3));
	udelay(100);
	if (pin == 0)//CH I
		data = (MADCREG(0x08) & 0x0fff);//J13
	else if(pin == 1)//CH Q
		data = (MADCREG(0x08) & 0x0fff0000) >> 16;//J14

	PMUREG(STDBY_PD_CTRL) |= STDBY_MADC_PD;//MADC power OFF
	if(pin == 0){
		MADCREG(0x00) |= (MADC_CHI_OFF);
	}else{
		MADCREG(0x00) |= (MADC_CHQ_OFF);
	}
	return (data);
}

