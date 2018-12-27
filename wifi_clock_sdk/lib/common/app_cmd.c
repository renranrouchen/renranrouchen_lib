/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file   cmd.c
*   \brief  Command  handler
*   \author Montage
*/

#include <lynx_dev.h>
#include <common.h>
#include <event.h>
#include <gpio.h>

int apps_printf = 0;
char apps_buf[4096];
int apps_buf_len = 0;

#ifdef CONFIG_CMD
#if ((CONFIG_ROM_VER > 1)||defined(CONFIG_FLASH_BOOT))
int sp_cmd(int argc, char *argv[])
{
	unsigned int val;

	if (argc <= 0)
		goto help;

	val = atoi(argv[0]);
	if(!val)
		goto help;

	ldev->deep_sleep_func(val);

	return ERR_OK;
help:
	return ERR_HELP;
}

cmdt cmdt_sp __attribute__ ((section("cmdt"))) =
{
"sp", sp_cmd, "sp <msec> ;Enter sleep mode\n"};
#endif

void app_cmd_init(void)
{
}

int flash_to_dma(int argc, char *argv[])
{
    unsigned int src, dst, len;
    int rc;
    int op;
    int opt = argv[-1][1];
    dst = 0x0;
    len = ldev->cmd.byte_count;
    src = 0x0;

	if (!argc)
		goto help;
    if (argc > 0 && sscanf(argv[0], "%x", &dst) != 1)
        goto err1;
    if (argc > 1 && sscanf(argv[1], "%x", &len) != 1)
        goto err1;
    if (argc > 2 && sscanf(argv[2], "%x", &src) != 1)
        goto err1;
	SFREG(SF_CR) = 0x00ff1900;

	int ch = 0;
	DMAREG(DMA_BOOST) = SRC_FROM_FLASH;
	DMAREG(DMA_BOOST) |= ch<<7;
	serial_printf("DAMREG %08x\n", DMAREG(DMA_BOOST));
	dma_copy((void *)src, (void *)dst, len, ch);
	DMAREG(DMA_BOOST) = 0;
	serial_printf("DMA Read flash dst=%x len=%x src=%x\n", dst, len, src);

    return ERR_OK;

  err1:
    return ERR_PARM;

  help:
    return ERR_HELP;
}

cmdt cmdt_fld __attribute__ ((section("cmdt"))) =
{"fdma", flash_to_dma, "fdma <faddr> <len> <buf> ; flash to dma cmd"};


int gpio_test(int argc, char *argv[])
{
    int mode, pin, value;

	if (!argc)
		goto help;
    if (argc > 0 && sscanf(argv[0], "%d", &mode) != 1)
        goto err1;
    if (argc > 1 && sscanf(argv[1], "%d", &pin) != 1)
        goto err1;
    if (argc > 2 && sscanf(argv[2], "%d", &value) != 1)
        goto err1;

	pin_mode(pin, mode);
	if(mode == 1)
	{
		digital_write(pin, value);
		serial_printf("GPIO %d out value %d\n", pin, value);
	}
	else
	{
		value = digital_read(pin);
		serial_printf("GPIO %d in  value %d\n", pin, value);
	}

    return ERR_OK;

  err1:
    return ERR_PARM;

  help:
    return ERR_HELP;
}

cmdt cmdt_io __attribute__ ((section("cmdt"))) =
{"gpio", gpio_test, "gpio mode pin value"};

int gpio_intr_test(int argc, char *argv[])
{
    int mode, pin, en;

	if (!argc)
		goto help;
    if (argc > 0 && sscanf(argv[0], "%d", &en) != 1)
        goto err1;
    if (argc > 1 && sscanf(argv[1], "%d", &mode) != 1)
        goto err1;
    if (argc > 2 && sscanf(argv[2], "%d", &pin) != 1)
        goto err1;

	if(en == 1)
	{
		pin_en_intr(pin, mode, NULL,NULL);
		serial_printf("GPIO %d en  intr mode %d\n", pin, mode);
	}
	else
	{
		pin_dis_intr(pin);
//		serial_printf("GPIO %d dis intr mode %d\n", pin, mode);
	}

    return ERR_OK;

  err1:
    return ERR_PARM;

  help:
    return ERR_HELP;
}

cmdt cmdt_gintr __attribute__ ((section("cmdt"))) =
{"gintr", gpio_intr_test, "gpio interrupt en/dis mode pin"};

#if 0
#include <lynx_dev.h>
#include <i2c.h>
int pwm_cmd(int argc, char *argv[])
{
	int duty, ch;
	int option = argv[-1][3];
	if(argc > 0 && sscanf(argv[0], "%d", &ch) != 1)
		return ERR_PARM;
	if(argc > 1 && sscanf(argv[1], "%d", &duty) != 1)
		return ERR_PARM;

	pwm_set_enable(ch, 1);
	pwm_set_polarity(ch, 1);
	if(option == 'd')
		pwm_set_duty(ch, duty);
	else if(option == 'f')
		pwm_set_freq(ch, duty);
	else
		return ERR_HELP;

	return ERR_OK;
}
cmdt cmdt_pwm __attribute__ ((section("cmdt"))) =
{"pwm", pwm_cmd,
"pwmd <channel> <duty>; pwm set duty\n\
pwmf <channel> <freq_id>; pwm set frquency"};

int i2c_read(int argc, char *argv[])
{
	int addr, num, c;
	unsigned char t[64]="";

	if(argc > 0 && !hextoul(argv[0], &addr))
		return ERR_HELP;
	if(argc > 1 && !hextoul(argv[1], &num))
		return ERR_HELP;
	if(num > 64) num = 64;
#if 0
	serial_printf("%02x\n", i2c_read_byte(addr));
#else
	i2c_read_data(addr, t, num);

	serial_printf("i2c read data %s\n", t);
	for(c = 0; num > 0; num--, c++)
		serial_printf("%02x ", t[c]);
	serial_printf("\n");
#endif

	return ERR_OK;
}

int i2c_send(int argc, char *argv[])
{
	int addr, data;
	unsigned char t[]="4321ABCDEEE\r\n";

	if(argc > 0 && !hextoul(argv[0], &addr))
		return ERR_HELP;
	if(argc > 1 && !hextoul(argv[1], &data))
		return ERR_HELP;

	//i2c_send_byte(addr, data);
	//i2c_send_str(addr, t);
	i2c_send_data(addr, t, data);

	return ERR_OK;
}

cmdt cmdt_i2c[] __attribute__ ((section("cmdt"))) =
{
	{"ir", i2c_read, "ir <address> <num>; i2c read data"},
	{"is", i2c_send, "is <address> <data> ; i2c send data"},
};

#endif
#if 0
extern int config_cmd(int argc, char *argv[]);
cmdt cmdt_config __attribute__ ((section("cmdt"))) =
{
".cfg", config_cmd,
".cfg<l,d,e,s,r,w> <id> <len> <data>; Config command\n\
.cfgl; Config load data\n\
.cfgd; Config memory dump\n\
.cfge; Config end\n\
.cfgs 10000;              Config data set!!\n\
.cfgr <id>;              Config read data\n\
.cfgw <id> <len> <data>; Config write data"};
#endif
#endif //CONFIG_CMD
