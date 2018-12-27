/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/**
 * @file pwm.c
 * @brief PWM APIs
 *
 */
#include <arch/chip.h>
#include <pwm.h>

#if (CONFIG_ROM_VER > 1)
int prescaler_tb[PWM_FREQ_ID_NUM] = { 119, 59, 23,  0, 243, 194, 96, 48, 26, 15, 11, 239, 222, 194, 155, 77,\
									  51, 38, 30, 25, 21, 16, 14, 12, 11, 7, 5, 3, 2, 1, 0};
int cycletime_tb[PWM_FREQ_ID_NUM] = { 1, 1, 1, 1, 7, 7, 7, 7, 7, 7, 7};
#else
int prescaler_tb[PWM_FREQ_ID_NUM] = { 244,146,  6,  2,  1,  0,243,155, 77, 64, 45, 38, 25, 18, 12,  9,\
									  249,155,124, 99, 82, 61, 49, 39, 24, 19,  9,  4,  3,  1,  0};
int cycletime_tb[PWM_FREQ_ID_NUM] = {  5,  5,  6,  5,  2,  2,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7};
#endif

/**
 * Set PWM frequency
 *
 * PWM0 and PWM1 share the same frequency setting. PWM1's frequency will be changed, if user changes PWM0's frequency. (PWM2 and PWM3 as well)
 *
 * id: (CONFIG_ROM_VER > 1)
 *
 * 0 : 0.1   Hz, 1 : 0.2   Hz, 2 : 0.5   Hz, 3 : 12    Hz, 4 : 20    Hz, 5 : 25    Hz,
 *
 * 6 : 50.34 Hz, 7 : 99.65 Hz, 8 : 180.84Hz, 9 : 305.17Hz, 10: 407   Hz, 11: 651   Hz,
 *
 * 12: 701   Hz, 13: 801   Hz, 14: 1    kHz, 15: 2    kHz, 16: 3    kHz, 17: 4    kHz,
 *
 * 18: 5.04 kHz, 19: 6.01 kHz, 20: 7.1  kHz, 21: 9.19 kHz, 22: 10.41kHz, 23: 12.02kHz,
 *
 * 24: 13.02kHz, 25: 19.53kHz, 26: 26.04kHz, 27: 39.06kHz, 28: 52.08kHz, 29: 78.13kHz,
 *
 * 30: 156.25kHz
 *
 * id: (CONFIG_ROM_VER == 1)
 *
 * 0 : 0.3   Hz, 1 : 0.5   Hz, 2 : 10    Hz, 3 : 25    Hz, 4 : 45    Hz, 5 : 90    Hz,
 *
 * 6 : 160   Hz, 7 : 250.4 Hz, 8 : 500.8 Hz, 9 : 600.96Hz, 10: 849.18Hz, 11: 1    kHz,
 *
 * 12: 1.502kHz, 13: 2.056kHz, 14: 3    kHz, 15: 3.906kHz, 16: 5    kHz, 17: 8    kHz,
 *
 * 18: 10   kHz, 19: 12.5 kHz, 20: 15   kHz, 21: 20.16kHz, 22: 25   kHz, 23: 31.25kHz,
 *
 * 24: 50   kHz, 25: 62.5 kHz, 26: 125  kHz, 27: 250  kHz, 28: 312.5kHz, 29: 625  kHz,
 *
 * 30: 1250 kHz
 *
 * @param pwm_ch 0 ~ 3 (GPIO 6 ~ 9)
 * @param id 0 ~ 30
 * @return None
 */
void pwm_set_freq(int pwm_ch, int id)
{
	if(pwm_ch > 3 || pwm_ch < 0)
		return;
	if(id > PWM_FREQ_ID_NUM - 1 || id < 0)
		return;

	int tmp;
	//serial_printf("\nprescaler %d cycle time %d",prescaler_tb[id],cycletime_tb[id]);

	if(pwm_ch < 2)//PWM0,1
	{
		tmp = MIREG(0x10);
		tmp &= PRE_SCALER;
		tmp &= CHA_TIME;
		tmp &= CHB_TIME;
		tmp |= (prescaler_tb[id] << 22);
		tmp |= (cycletime_tb[id] << 14);
		tmp |= (cycletime_tb[id] << 17);
		MIREG(0x10) = tmp;
	}
	else//PWM2,3
	{
		tmp = MIREG(0x14);
		tmp &= PRE_SCALER;
		tmp &= CHA_TIME;
		tmp &= CHB_TIME;
		tmp |= (prescaler_tb[id] << 22);
		tmp |= (cycletime_tb[id] << 14);
		tmp |= (cycletime_tb[id] << 17);
		MIREG(0x14) = tmp;
	}
}

/**
 * Set PWM duty
 * @param pwm_ch 0 ~ 3 (GPIO 6 ~ 9)
 * @param duty 0 ~ 31; 16 = 50%, 31 = 100%
 * @return None
 */
void pwm_set_duty(int pwm_ch, int duty)
{
	if(pwm_ch > 3 || pwm_ch < 0)
		return;
#if (CONFIG_ROM_VER > 1)
	if(duty > 255 || duty < 0)
		return;
#else
	if(duty > 31 || duty < 0)
		return;
#endif

	int tmp, tmp2;

#if (CONFIG_ROM_VER > 1)
	if((duty & 0x1f) == 0)
		duty += 1;
	if((duty & 0x1f) == 0x1f)
		duty -= 1;
#endif
	//serial_printf("duty %d\n", duty);
	if(pwm_ch < 2)
		tmp = MIREG(0x10);
	else
		tmp = MIREG(0x14);
	tmp2 = RFREG(0x60);

	if(pwm_ch == 0)//cha
	{
		tmp &= CHA_DUTY;
		tmp |= ((duty & 0x1f) << 4);

		tmp2 &= PWM1_A;
		tmp2 |= ((duty >> 5) << 20);
	}
	else if(pwm_ch == 1)//chb
	{
		tmp &= CHB_DUTY;
		tmp |= ((duty & 0x1f) << 9);

		tmp2 &= PWM1_B;
		tmp2 |= ((duty >> 5) << 23);
	}
	else if(pwm_ch == 2)//cha
	{
		tmp &= CHA_DUTY;
		tmp |= ((duty & 0x1f) << 4);

		tmp2 &= PWM2_A;
		tmp2 |= ((duty >> 5) << 26);
	}
	else if(pwm_ch == 3)//chb
	{
		tmp &= CHB_DUTY;
		tmp |= ((duty & 0x1f) << 9);

		tmp2 &= PWM2_B;
		tmp2 |= ((duty >> 5) << 29);
	}

	if(pwm_ch < 2)
		MIREG(0x10) = tmp;
	else
		MIREG(0x14) = tmp;
#if (CONFIG_ROM_VER > 1)
	RFREG(0x60) = tmp2;
#endif
}

/**
 * Set PWM polarity
 * @param pwm_ch 0 ~ 3 (GPIO 6 ~ 9)
 * @param value 0: active low\n 1: active high
 * @return None
 */
void pwm_set_polarity(int pwm_ch, int value)
{
	if(pwm_ch > 3 || pwm_ch < 0)
		return;
	if(value > 1 || value < 0)
		return;

	int tmp;

	if(pwm_ch < 2)
		tmp = MIREG(0x10);
	else
		tmp = MIREG(0x14);

	if(pwm_ch == 0)//cha
	{
		tmp &= CHA_POLAR;
		tmp |= (value << 20);
	}
	else if(pwm_ch == 1)//chb
	{
		tmp &= CHB_POLAR;
		tmp |= (value << 21);
	}
	else if(pwm_ch == 2)//cha
	{
		tmp &= CHA_POLAR;
		tmp |= (value << 20);
	}
	else if(pwm_ch == 3)//chb
	{
		tmp &= CHB_POLAR;
		tmp |= (value << 21);
	}

	if(pwm_ch < 2)
		MIREG(0x10) = tmp;
	else
		MIREG(0x14) = tmp;
}

/**
 * Set PWM enable
 *
 * UART channel 1 use the same pin as PWM0,1 (GPIO6,7)
 *
 * UART channel 2 use the same pin as PWM2,3 (GPIO8,9)
 * @param pwm_ch 0 ~ 3 (GPIO 6 ~ 9)
 * @param value 0: disable\n 1: enable
 * @return None
*/
void pwm_set_enable(int pwm_ch, int value)
{
	if(pwm_ch > 3 || pwm_ch < 0)
		return;
	if(value > 1 || value < 0)
		return;

	int enable = 1;

	if(value)
	{
		if(pwm_ch < 2)
			PMUREG(PKG_MODE_CTRL) &= ~UART2_EN0;
		else
			PMUREG(PKG_MODE_CTRL) &= ~UART2_EN1;
	}

	if(pwm_ch == 0)//cha
	{
		if(value)
			MIREG(0x10) |= enable;
		else
			MIREG(0x10) &= ~enable;
	}
	else if(pwm_ch == 1)//chb
	{
		if(value)
			MIREG(0x10) |= (enable << 1);
		else
			MIREG(0x10) &= ~(enable << 1);
	}
	else if(pwm_ch == 2)//cha
	{
		if(value)
			MIREG(0x14) |= enable;
		else
			MIREG(0x14) &= ~enable;
	}
	else if(pwm_ch == 3)//chb
	{
		if(value)
			MIREG(0x14) |= (enable << 1);
		else
			MIREG(0x14) &= ~(enable << 1);
	}
}

/**
 * Get PWM frequency
 * @param pwm_ch 0 ~ 3 (GPIO 6 ~ 9)
 * @return frequency id, -1: can't find correct id
 */
int pwm_get_freq(int pwm_ch)
{
	if(pwm_ch > 3 || pwm_ch < 0)
		return;

	int tmp, prescaler, cycletime, id;

	if(pwm_ch == 0)//cha
	{
		tmp = MIREG(0x10);
		prescaler = (tmp & ~PRE_SCALER) >> 22;
		cycletime = (tmp & ~CHA_TIME) >> 14;
	}
	else if(pwm_ch == 1)//chb
	{
		tmp = MIREG(0x10);
		prescaler = (tmp & ~PRE_SCALER) >> 22;
		cycletime = (tmp & ~CHB_TIME) >> 17;
	}
	else if(pwm_ch == 2)//cha
	{
		tmp = MIREG(0x14);
		prescaler = (tmp & ~PRE_SCALER) >> 22;
		cycletime = (tmp & ~CHA_TIME) >> 14;
	}
	else if(pwm_ch == 3)//chb
	{
		tmp = MIREG(0x14);
		prescaler = (tmp & ~PRE_SCALER) >> 22;
		cycletime = (tmp & ~CHB_TIME) >> 17;
	}
	for(id = 0; id < PWM_FREQ_ID_NUM; id++)
	{
		if(prescaler == prescaler_tb[id])
			if(cycletime == cycletime_tb[id])
				return id;
	}
	return -1;
}

/**
 * Get PWM duty
 * @param pwm_ch 0 ~ 3(GPIO 6 ~ 9)
 * @return duty 0 ~ 31
 */
int pwm_get_duty(int pwm_ch)
{
	if(pwm_ch > 3 || pwm_ch < 0)
		return;

	int tmp;

	if(pwm_ch == 0)//cha
	{
		tmp = MIREG(0x10);
		tmp &= ~CHA_DUTY;
		tmp = (tmp >> 4);
	}
	else if(pwm_ch == 1)//chb
	{
		tmp = MIREG(0x10);
		tmp &= ~CHB_DUTY;
		tmp = (tmp >> 9);
	}
	else if(pwm_ch == 2)//cha
	{
		tmp = MIREG(0x14);
		tmp &= ~CHA_DUTY;
		tmp = (tmp >> 4);
	}
	else if(pwm_ch == 3)//chb
	{
		tmp = MIREG(0x14);
		tmp &= ~CHB_DUTY;
		tmp = (tmp >> 9);
	}
	return tmp;
}

/**
 * Get PWM polarity
 * @param pwm_ch 0 ~ 3 (GPIO 6 ~ 9)
 * @return 0: active low\n 1: active high
 */
int pwm_get_polarity(int pwm_ch)
{
	if(pwm_ch > 3 || pwm_ch < 0)
		return;

	int tmp;

	if(pwm_ch == 0)//cha
	{
		tmp = MIREG(0x10);
		tmp &= ~CHA_POLAR;
		tmp = (tmp >> 20);
	}
	else if(pwm_ch == 1)//chb
	{
		tmp = MIREG(0x10);
		tmp &= ~CHB_POLAR;
		tmp = (tmp >> 21);
	}
	else if(pwm_ch == 2)//cha
	{
		tmp = MIREG(0x14);
		tmp &= ~CHA_POLAR;
		tmp = (tmp >> 20);
	}
	else if(pwm_ch == 3)//chb
	{
		tmp = MIREG(0x14);
		tmp &= ~CHB_POLAR;
		tmp = (tmp >> 21);
	}
	return tmp;
}

