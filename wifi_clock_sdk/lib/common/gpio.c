/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf@gmail.com
*@Brief			
*@FileName		gpio.c
*@Data			2017/7/13 10:40:07
-------------------------------------------------------------------------*/
/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/**
 * @file gpio.c
 * @brief GPIO APIs
 *
 */
#include <arch/chip.h>
#include <arch/trap.h>
#include <gpio.h>

gpio_irq_handler gpio_irq_tables[22];//GPIO 0-21


/*   G P I O _ P U L L U P   */
/*-------------------------------------------------------------------------
*@param			
*@Author			Zwxf
*@Email			wengzw@fdhisi.com
*@Brief			
-------------------------------------------------------------------------*/
void gpio_pullup(int pin,int mode)
{
	if(pin > 21 || pin < 0 || mode > 1 || mode < 0)
		return;	
	int bit = 1 << pin;	
	
	if(mode == 1)
		PMUREG(0x30) |=  bit;//1: disable pullup GPIO
	else
		PMUREG(0x30) &= ~bit;//0:  enable pullup GPIO

}
/**
 * Enable GPIO function.
 * @param pin GPIO number
 * @param mode 0: disable\n 1: enable
 * @return None
 */

void gpio_enable(int pin, int mode)
{
	if(pin > 21 || pin < 0 || mode > 1 || mode < 0)
		return;

	int bit = 1 << pin;

	if(mode == 1)
		PMUREG(0x04) |=  bit;//1: enable GPIO
	else
		PMUREG(0x04) &= ~bit;//0: disable GPIO
}

/**
 * Set GPIO pin mode, include gpio_enable.
 * @param pin GPIO number
 * @param mode 0: input\n 1: output
 * @return None
 */
void pin_mode(int pin, int mode)
{
	if(pin > 21 || pin < 0 || mode > GPIO_OUT || mode < GPIO_IN)
		return;

	int bit = 1 << pin;

	PMUREG(0x04) |= bit;
	if(mode == GPIO_OUT){//1: output
		PMUREG(0x10) &= ~bit;//0: GPIO is set on output mode
		PMUREG(0x20) &= ~bit;
	}else{
		PMUREG(0x10) |=  bit;//1: GPIO is set on input mode
		PMUREG(0x20) |=  bit;
	}	
}

/**
 * Set GPIO pin output data, call it after pin_mode.
 * @param pin GPIO number
 * @param val 0: low\n 1: high
 * @return None
 */
void digital_write(int pin, int val)
{
	if(pin > 21 || pin < 0 || val > HIGH)
		return;

	int bit = 1 << pin;

	if(val == HIGH)//1: high
		GPREG(0x18) |= bit;
	else
		GPREG(0x18) &= ~bit;
}

/**
 * Set two GPIO pin output data at the same time, call it after pin_mode.
 * @param pin GPIO number
 * @param val 0: low\n 1: high
 * @param pin2 GPIO number
 * @param val2 0: low\n 1: high
 * @return None
 */

void digital_write_two(int pin, int val, int pin2, int val2)
{
	if(pin > 21 || pin < 0 || val > HIGH)
		return;
	if(pin2 > 21 || pin2 < 0 || val2 > HIGH)
		return;

	int bit = 1 << pin;
	int bit2 = 1 << pin2;
	int old = GPREG(0x18);

	if(val == HIGH)//1: high
		old |= bit;
	else
		old &= ~bit;

	if(val2 == HIGH)//1: high
		old |= bit2;
	else
		old &= ~bit2;
	GPREG(0x18) = old;

}

/**
 * Read GPIO pin input data, call it after pin_mode.
 * @param pin GPIO number
 * @return 0: low\n 1: high
 */
int digital_read(int pin)
{
	if(pin > 21 || pin < 0)
		return -1;

	int bit = 1 << pin;

	if(GPREG(0x14) & bit)
		return HIGH;
	return LOW;
}

/**
 * Enable GPIO pin interrupt.
 * @param pin GPIO number
 * @param mode 0: rising\n 1: falling\n 2:high level\n 3:low level\n
 * @param handler : a function pointer to the interrupt handler
 * @param arg     : an argument that will be passed to the
 *                  interrupt handler
 * @return None
 */
void pin_en_intr(int pin, int mode, gpio_func_cb handler,void* arg)
{
	if(pin > 21 || pin < 0 || mode > 3 || mode < 0)
		return;

	int bit = 1 << pin;

	GPREG(0x90) &= ~bit;
	if(mode == 0)//rising
	{
		GPREG(0x80) |= bit;
	}
	else if(mode == 1)//falling
	{
		GPREG(0x84) |= bit;
	}
	else if(mode == 2)//high level
	{
		GPREG(0x88) |= bit;
	}
	else if(mode == 3)//low level
	{
		GPREG(0x8c) |= bit;
	}

	gpio_irq_tables[pin].hnd = handler;
	gpio_irq_tables[pin].arg = arg;
}

/**
 * Disable GPIO pin interrupt.
 * @param pin GPIO number
 * @return None
 */
void pin_dis_intr(int pin)
{
	if(pin > 21 || pin < 0)
		return;

	int bit = 1 << pin;

	//GPREG(0x90) |= bit;

	GPREG(0x80) &= ~bit;//rising

	GPREG(0x84) &= ~bit;//falling

	GPREG(0x88) &= ~bit;//high level

	GPREG(0x8c) &= ~bit;//ow level

}

/**
 * Trigger an output GPIO pin 
 * @param pin GPIO number
 * 
 * @return None
 */
void pin_trigger(int pin)
{
	if(pin > 21 || pin < 0)
		return;

	int tmp = (GPREG(0x18)>>pin)&0x1;
	
	pin_mode(pin,GPIO_OUT);
	if(tmp)
		digital_write(pin,LOW);
	else
		digital_write(pin,HIGH);
	
}


void gpio_handler(void)
{
	unsigned int sts = PMUREG(GPIO_INT_STS);
	int pin = 0;
	PMUREG(GPIO_INT_STS) = sts;

	for(pin = 0; pin < 22; pin++, sts >>= 1)
		if(sts & 0x1)
			break;
	if(gpio_irq_tables[pin].hnd)
	{
		gpio_irq_tables[pin].hnd(gpio_irq_tables[pin].arg);
	}
}

