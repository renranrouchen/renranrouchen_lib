#include <serial.h>
#include "at_cmd.h"
#include "at_cmd_func.h"
#include <event.h>
#include <lynx_dev.h>
#include <version.h>
#include <cfg_api.h>

extern u32 urbase[];
extern sdk_param g_atParam;
extern void uart_putchar(unsigned int cc);

int at_test(int argc, char *argv[], char *op[])			//at
{
	if(strcmp(op,NUL))
		return at_ERR_OP;
	at_printf("\ntest at command\n");
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: show_version
 *
 *  \brief:    Show version info
 *  \operator: NULL execute
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_show_version(int argc, char *argv[], char *op[])	//ver
{
	if(strcmp(op,NUL))
		return at_ERR_OP;
	if(argc != 0)
		return at_ERR_PARM;


	at_printf("\nSDK Version:%s \n",sw_build_sdk);
	at_printf("\nSDK Date:%s \n",sw_build_time);

	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: go_cli
 *
 *  \brief:    Go to CLI mode
 *  \operator: NULL execute
 *  \return:   check result
 +----------------------------------------------------------------------------*/
#if 0
extern int cmd_loop(void);
int at_go_cli(int argc, char *argv[], char *op[])			//cli
{
	if(strcmp(op,NUL))
		return at_ERR_OP;
	if(argc != 0)
		return at_ERR_PARM;

	/* continue to enter sleeping mode even go to CLI mode */
	ldev->wmac.ps_uart = 2;
	if(ldev->ur.chan == 3)//ldev->udev.cfg_len == 67
		EVENT_REGISTER((evt_func)cmd_loop, EVT_HOST_NUM);
	else
		EVENT_REGISTER((evt_func)cmd_loop, EVT_UART_RX_NUM);

	return at_ERR_CLI;
}
#endif

/*!-----------------------------------------------------------------------------
 * function: return_def
 *
 *  \brief:    Return to default settings
 *  \operator: NULL execute
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_return_def(int argc, char *argv[], char *op[])		//rdef
{
	if(strcmp(op,NUL))
		return at_ERR_OP;
	if(argc != 0)
		return at_ERR_PARM;
	config_erase();
	reboot(1);

	serial_printf("\nLoad default settings...\n");
	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: rst_module
 *
 *  \brief:    Reset
 *  \operator: NULL execute
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_rst_module(int argc, char *argv[], char *op[])		//rst
{
	if(strcmp(op,NUL))
		return at_ERR_OP;
	if(argc != 0)
		return at_ERR_PARM;

	serial_printf("\n Reset...\n");
	reboot(1);

	return at_ERR_OK;
}


/*!-----------------------------------------------------------------------------
 * function: ur_config
 *
 *  \brief:    Set or query UART baud rate, data bits, parity, stop bits, flow control
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  baudrate id 0~12
 *  \param 1:  parity   0: no, 1: even, 2: odd
 *  \param 2:  stopbits 1,2
 *  \return:   check result
 +----------------------------------------------------------------------------*/

extern unsigned int br_tb[];
int at_ur_config(int argc, char *argv[], char *op[])		//ur
{
	int p, check, br_id;
	int baudrate,  //0 ~ 12  br_tb
		parity,    //0, 1, 2 = none, odd, even
		stopbits;  //1, 2    = 1, 2 bit

	if(strcmp(op,"=") && strcmp(op,"=?"))
		return at_ERR_OP;
	if(!strcmp(op,"=?"))
		goto query;
	if(argc != 3)//op is "="
		return at_ERR_PARM;

//check if parameters are number
	for(p = 0; p < argc; p++)
	{
		for(check = 0; argv[p][check] != '\0'; check++)
			if(!isnum(argv[p][check]))//if(argv[p][check] < 48 || argv[p][check] >57)
				return at_ERR_PARM;
	}

	if(0 <= (br_id = atoi(argv[0])) && strcmp(argv[0],NUL))
	{
		if(br_id < 0 || br_id > 12)
			return at_ERR_PARM;
	}
	else //null, no change
	{
		baudrate = (CONFIG_CRYSTAL_CLK / (((REG(urbase[0], URCS) & ~URCS_RXEN) >> 16) +1));
		for(p = 0; p < UART_BR_TB; p++)
		{
			if(baudrate >= br_tb[p])
				br_id = p;
			else
				break;
		}
	}

	if(0 <= (parity = atoi(argv[1])) && strcmp(argv[1],NUL))
	{
		if(parity < 0 || parity > 2)
			return at_ERR_PARM;
	}
	else
	{
		if(REG(urbase[0], URCS) & URCS_PE)
			parity = ((REG(urbase[0], URCS) & URCS_EVEN) >> 12) + 1;
		else
			parity = 0;
	}
	
	if(0 <= (stopbits = atoi(argv[2])) && strcmp(argv[2],NUL))
	{
		if(stopbits < 1 || stopbits > 2)
			return at_ERR_PARM;
	}
	else
		stopbits = ((REG(urbase[0], URCS) & URCS_SP2) >> 11) + 1;

	serial_conf(br_id, parity, stopbits, 0, 0);
	return at_ERR_OK;

query:
	baudrate = (CONFIG_CRYSTAL_CLK / (((REG(urbase[0], URCS) & ~URCS_RXEN) >> 16) +1));
	for(p = 0; p < UART_BR_TB; p++)
	{
		if(baudrate >= br_tb[p])
			br_id = p;
		else
			break;
	}

	if(REG(urbase[0], URCS) & URCS_PE)
		parity = ((REG(urbase[0], URCS) & URCS_EVEN) >> 12) + 1;
	else
		parity = 0;
	stopbits = ((REG(urbase[0], URCS) & URCS_SP2) >> 11) + 1;

	at_printf("\nBaudrate = %d", br_tb[br_id]);
	at_printf("\nParity   = %d", parity);
	at_printf("\nStopbits = %d\n", stopbits);

	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_params_save
 *
 *  \brief:    Set or query setting of save to flash
 *  \operator: "="set, "=?"query the current setting
 *  \param 0:  1:save;0:not save
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_params_save(int argc, char *argv[], char *op[])
{
	if (!strcmp(op,"=")) {
		int val;
		if(argc != 1)
			return at_ERR_PARM;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;
		val = atoi(argv[0]);
		if ((val != 0) && (val != 1))
			return at_ERR_PARM;
		if (val)
			g_atParam.flags |= CFG_SAVE;
		else
			g_atParam.flags &= ~CFG_SAVE;
	} else if (!strcmp(op,"=?")) {
		if (argc != 0)
			return at_ERR_HELP;
		at_printf("\n<%d>\n", (g_atParam.flags & CFG_SAVE) ? 1 : 0);
	} else {
		return at_ERR_OP;
	}

	return at_ERR_OK;
}

/*!-----------------------------------------------------------------------------
 * function: at_debug_print
 *
 *  \brief:    Debug log(UART2) enable or disable
 *  \operator: "="set
 *  \param 0:  1: enable, 0: disable
 *  \return:   check result
 +----------------------------------------------------------------------------*/
int at_debug_print(int argc, char *argv[], char *op[])
{
	int enable;
	if (!strcmp(op,"="))
	{
		if(argc != 1)
			return at_ERR_PARM;
		if (strchr(argv[0], ' '))
			return at_ERR_PARM;

		enable = atoi(argv[0]);
		if(enable)
		{
			ldev->serial_io_char = uart_putchar;
			PMUREG(0x34) |= UART2_EN0;
		}
		else
		{
			ldev->serial_io_char = NULL;
			PMUREG(0x34) &= ~UART2_EN0;
		}
	}
	else
		return at_ERR_OP;

	return at_ERR_OK;
}
