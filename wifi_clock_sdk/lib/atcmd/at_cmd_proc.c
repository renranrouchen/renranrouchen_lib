#include <lynx_dev.h>
#include <event.h>
#include <common.h>
#include <serial.h>
#include <stdarg.h>
#include "at_cmd.h"
#include "at_cmd_func.h"
#if defined(CONFIG_FREERTOS)
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
SemaphoreHandle_t uart1_rx = NULL;
#endif

#include <lwip/netif.h>
#include <lwip/sockets.h>
#include <net_api.h>
#include <socket_api.h>

#define UART1 0
#define AT_CMD_NUM 40
#define AT_CMD_RX_BUFSIZE 2048
#define AT_CMD_TRANS_PKG_SIZE 1024

char trans_mode_timeout_stat = 0;
char uart1_rx_full = 0;
struct serial_buffer* ur1_rxbuf = NULL;
void (*enhance_scan_func)(void *list) = NULL;

extern u8	ur_irq[];
extern u32 urbase[];

extern char trans_mode;
extern int g_linkIdx;
extern int app_uart_isr(int chan);
extern unsigned int uart_read_timeout;//ms

char *at_err_str[] = {
    "operator",
    "parameter",
    "fail",
};

extern const char erasetoeol[];     //<ESC>[K<NUL>
extern const char eraseback[];      //erase backspace

atcmdt at_tb[AT_CMD_NUM]=
{
	{"at", at_test, "test command"},
	//Basic command 6
	{"ver" , at_show_version, "Show version"},
	{"rdef", at_return_def  , "Return to default setting"},
	{"rst" , at_rst_module  , "Reset module"},
#ifndef CONFIG_USB_CDC_ACM_DEV
	{"ur"  , at_ur_config   , "Set or query uart config,<baudrate id>[0-12]<parity>[0-2]<stopbit>[1-2]"},
#endif
	{"sflash", at_params_save , "Save at params to flash,<enable>"},
	{"debug", at_debug_print , "Debug log(UART2) <enable>"},

#ifdef CONFIG_WLA
	//wifi interface 7
	{"wmode"    , at_wifi_mode     , "Set or query work mode, <mode>[0-1]"},
	{"wscan"    , at_wifi_scan     , "Scan ap, returns a list of found aps"},
	{"wphy"     , at_wifi_phy      , "Set or query phy mode, <mode>[1-7]"},
	{"wtxpwr"   , at_wifi_txpwr    , "Set or query tx power, <level>[0-12]"},
	{"wstaup"   , at_wifi_staup    , "Start station, <ssid><password>"},
	{"wstadn"   , at_wifi_stadn    , "Stop station"},
	{"wautoconn", at_wifi_autoconn , "Set or query auto connect, <en>"},
	{"wreconn"  , at_wifi_reconn   , "Reconnect to target ap, <en>[0-1]"},
	{"womcfgon" , at_wifi_omcfgon  , "Start omniconfig, [<sec>]"},
	{"womcfgoff", at_wifi_omcfgoff , "Stop omniconfig"},
	{"wapup"    , at_wifi_apup     , "Start softap, <ssid><pwd><ch>[<max conn>][<ssid hidden>]"},
	{"wapdn"    , at_wifi_apdn     , "Stop softap"},
	{"wlink"    , at_wifi_link     , "Link status, <idx>[0-1]"},
	{"wsta"     , at_wifi_sta      , "Show sta info"},
#if 0
	{"wps"      , at_wifi_wps      , "Set wps mode,<enable>"},
	{"mdns"     , at_wifi_mdns     , "Start or stop the mdns,<enable><hostname><server_name><port>"},
#endif
	{"wsleep"   , at_wifi_sleep    , "Set sleep mode,<mode>"},
#endif

#ifdef CONFIG_LWIP
	//Network interface
	{"dhcp"     , at_net_dhcp      , "Set or query dhcp mode, <mode>[0-3]"},
	{"dhcps"    , at_net_dhcps     , "Set or query the IP range of dhcps, <enable><lease time><start ip><end ip>"},
	{"ip"       , at_net_ip        , "Static ip, <idx>[,<ip>,<netmask>,<gw>]"},
	{"mac"      , at_net_mac       , "Set or query mac address, <idx><mac>"},
	{"ping"     , at_net_ping      , "Ping ip, <ipaddr>[,<len>,<count>,<timeout>]"},
	{"dns"      , at_net_dns       , "Set or query dns server, <svr idx><ip>"},
	{"host"     , at_net_host      , "Get host by name, <hostname>"},
	//Socket
	{"sinfo"    , at_socket_info   , "Conn info"},
	{"sadd"     , at_socket_add    , "Add conn, <conn idx>,<TCP/UDP>,<ip>,<dport>,[<TCP:keeplive/UDP:lport>]"},
	{"sdel"     , at_socket_del    , "Delete conn, <conn idx>"},
	{"ssend"    , at_socket_send   , "Send data, [<conn idx>,<length>]"},
	{"tmode"    , at_trans_mode    , "Enable trans mode,<enable>"},
	{"tcpserver", at_socket_TCPServer,"Start tcp server,<mode><port>"},
	{"savelink" , at_trans_savelink,"Save trans params,<mode><ip><port>[<type>][<time>]"},
	{"update"   , at_update        , "Update, <url>,<port>"},	
#endif
};

int at_printf(char *fmt, ...)
{
	char pf_buf[256];
	int count;
	char *p;
	va_list args;

	va_start(args, fmt);
	count = vsprintf(pf_buf, fmt, args);
	va_end(args);

	p = pf_buf;

	while(*p)
	{
		if(*p == '\n')
			uart_putc(0, '\r');
		uart_putc(0, *p++);
	}
	return count;
}

int at_cmd_loop(char *at_cmd_buf, char *at_cmd_len, char *at_cmd_state)
{
	char *argv[AT_MAX_ARGV], *op[5], *bp;
	int argc, i;
	unsigned char key;
	struct serial_buffer *pbuf = ur1_rxbuf;

	while(1)
	{
		if(pbuf->space >= pbuf->buffsize && pbuf->pout == pbuf->pin)
			return; //tx buffer empty

		key = *pbuf->pout;

		pbuf->pout++;
		if(pbuf->pout == (pbuf->purbuff + pbuf->buffsize))
			pbuf->pout = pbuf->purbuff;

		pbuf->pout = (pbuf->purbuff + (pbuf->pout - pbuf->purbuff) % pbuf->buffsize);
		pbuf->space += 1;

		bp = at_cmd_buf + *at_cmd_len;
		op[0] = NUL;
		argc = 0;
		do
		{
			if(!key)
			{
				*at_cmd_len = (int) (bp - at_cmd_buf);
				break;
			}
			if (ldev->wmac.ps_uart != 2)
				ldev->wmac.ps_uart = 1;
			switch (key)
			{
				case LF:
				case CR:
					putchar(LF);
					putchar(CR);
					*at_cmd_len = (int) (bp - at_cmd_buf);
					//double NUL avoid that a remnant to be see an argument
					*bp++ = NUL;
					*bp-- = NUL;
					goto out;
				case TAB:
					while(bp > at_cmd_buf)
					{
						bp--;
						putchar(BS);
					}
					serial_printf("%s",erasetoeol);
					key = 0;
					continue;
				case BS:
				case DEL:
					if(bp > at_cmd_buf)
					{
						bp--;
						serial_printf("%s",eraseback);
					}
					key = 0;
					continue;
				case ESC:
					*at_cmd_state = STATE_ESC_START;
					key = 0;
					continue;
				case '[':
					if(*at_cmd_state == STATE_ESC_START) //for '['
					{
						*at_cmd_state = STATE_ESC_CODE;
						key = 0;
						continue;
					}
				default:
					if(*at_cmd_state == STATE_ESC_START)
					{
						*at_cmd_state = STATE_ESC_NONE;
					}
					else if(*at_cmd_state == STATE_ESC_CODE)
					{
						if(isnum(key))
							*at_cmd_state = STATE_ESC_CODE;
						else
							*at_cmd_state = STATE_ESC_NONE;
					}
					else
					{
						*bp++ = key;
						putchar(key);
					}
					key = 0;
			}
		}while ((bp - at_cmd_buf) < (128));
out:
		if((*(bp - 1) != NUL) && (key != CR) && (key != LF) && ((bp - at_cmd_buf) < 128))
			continue;

		for(i = 0; i < *at_cmd_len; i++)
		{
			if(at_cmd_buf[i] != ' ')//ignore the space before the at+cmd
			{
				if((!strncmp(&at_cmd_buf[i],"at+", 3)) && (at_cmd_buf[i+3] != ' ') && (at_cmd_buf[i+3] != NUL))
				{
					if(0 < (argc = get_at_args(&at_cmd_buf[i+3], argv, op)))
						at_cmd_proc(argc, argv, op);
					else
					{
						at_printf("ERROR\n");
						serial_printf("ERROR\n");
					}
					break;
				}
				if((!strncmp(&at_cmd_buf[i], "at\0", 3)))
				{
					at_printf("OK\n");
					serial_printf("OK\n");
				}
				else
				{
					at_printf("ERROR\n");
					serial_printf("ERROR\n");
				}
				break;
			}
		}
#if 0
		serial_printf("argc  = %d, op = %s\n", argc, op);
		for( i = 0; i < argc;i++)
			serial_printf("argv%d = %s\n", i, argv[i]);
#endif
loop_end:
		*at_cmd_len = 0;
		if (ldev->wmac.ps_uart != 2)
			ldev->wmac.ps_uart = 0;
	}
}

int at_cmd_proc(int argc, char **argv, char *op[])
{
    int i, match, rc;

    for(i = 0; i < AT_CMD_NUM; i++)
    {
        if(!strcmp(at_tb[i].atcmd, argv[0]))
        {
            match = i;
            goto match;
        }
    }
    if(!match)
	{
        at_printf("ERROR\n");
        serial_printf("ERROR: no command\n");
	}
    else
    {
match:
        rc = at_tb[match].func(argc - 1, argv + 1, op);
        if(rc == at_ERR_HELP)
		{
            at_printf("ERROR\n");
            serial_printf("%s\n", at_tb[match].msg);
		}
        else if(rc == at_ERR_OK)
		{
            at_printf("OK\n");
            serial_printf("OK\n");
		}
        else if(rc == at_ERR_TRANS)
		{
            at_printf("OK\n");
            serial_printf("OK\n>>\n");
		}
        else if(0 > rc)
        {
            i = 0 - rc - 1;
            at_printf("ERROR\n");
            serial_printf("ERROR: ");
            if (i < (0 - at_ERR_LAST))
                serial_printf("%s\n", at_err_str[i]);
        }
        return rc;
    }
    return -1;
}

int get_at_args(const char *string, char *argv[], char *op[])
{
    char *p, last_empty = 0;
    int n;

    n = 0;
    p = (char *)string;

	if(*p == '=' || *p == '?')
		goto err;

	argv[n] = p;

	while((*p != '=') && (*p != '?') && (*p != '\0'))//get cmd
		p++;

	if(*p == '?')        //get op
	{
		*p++ = '\0';     //*p = '\0'; p++;
		strcpy(op,"?");
	}
	else if(*p == '=')
	{
		*p++ = '\0';
		if(*p == '?')
		{
			*p++ = '\0';
			strcpy(op, "=?");
		}
		else
			strcpy(op, "=");
	}
	else //NUL
	{
		strcpy(op,NUL);
		n++;
		return n;
	}
	if(!(isnum(*p) || upcase(*p) || lowcase(*p) || *p == NUL || *p == ',' || *p == ' '))
		goto err;

    n++;
    while(*p)        //get parameters
    {
        last_empty = 0;
        argv[n] = p;
        while(*p != ',' && *p)
            p++;
        if(*p == ',')
            last_empty = 1;
        *p++ = '\0';
        n++;
        if(n == AT_MAX_ARGV)
            goto err;
    }
	if(last_empty == 1)
	{
		argv[n] = p;
		n++;
		//serial_printf("\r\nlast is empty!");
		if(n == AT_MAX_ARGV)
			goto err;
	}
	return n;
err:
	return 0;
}

void trans_mode_timeout(void *data)
{
	trans_mode_timeout_stat = 1;
}

static unsigned char s_pdata[AT_CMD_TRANS_PKG_SIZE] = "";
static int s_length = 0;
static void init_data_buffer(void)
{
	memset(s_pdata,0x0,AT_CMD_TRANS_PKG_SIZE);
	s_length = 0;
}

void trans_mode_loop(void)
{
#ifdef CONFIG_LWIP
	struct serial_buffer *pbuf = ur1_rxbuf;
	int len = 0, set_timer = 0;
	int i;

	trans_mode_timeout_stat = 0;

	while(1)
	{
		if((s_length >= AT_CMD_TRANS_PKG_SIZE) || (trans_mode_timeout_stat))
			break;

		if(pbuf->space >= pbuf->buffsize && pbuf->pout == pbuf->pin)
		{
			if(!set_timer)
				add_timeout((FUNCPTR)trans_mode_timeout, 0, 200);
			set_timer = 1;
			continue;//tx buffer empty
		}
		if(set_timer)
		{
			del_timeout((FUNCPTR)trans_mode_timeout, 0);
			set_timer = 0;
		}

		/* check */
		if(uart1_rx_full && pbuf->space >= 16)
		{
			uart1_rx_full = 0;
#ifdef CONFIG_UR_FLOW_CONTROL				
			digital_write(CONFIG_UR_RTS, 0);//RTS active
#endif			
			enable_irq(ur_irq[UART1]);
		}
		s_pdata[s_length++] = *pbuf->pout;

		vTaskEnterCritical();
		pbuf->pout++;
		if(pbuf->pout == (pbuf->purbuff + pbuf->buffsize))
			pbuf->pout = pbuf->purbuff;

		pbuf->space += 1;
		vTaskExitCritical();

		if(checkExitTransMode(s_length))
		{
			trans_mode = 0;
			del_timeout((FUNCPTR)trans_mode_timeout, 0);
			net_socket_tx(g_linkIdx, s_pdata,s_length);
			init_data_buffer();			
			serial_printf("\n%s %d\n",__FUNCTION__,__LINE__);
			return;
		}		
	}

	if(((s_length == 4) && (s_pdata[s_length - 2] == '+') && (!strncmp(&s_pdata[s_length - 4], "+++", 3))) ||
		((s_length == 3) && (s_pdata[s_length - 1] == '+') && (!strncmp(&s_pdata[s_length - 3], "+++", 3))))
	{
		trans_mode = 0;
		init_data_buffer();
		return;
	}	
	if(getSendLength() == -1)
	{
		net_socket_tx(g_linkIdx, s_pdata,s_length);
		init_data_buffer();
	}
	/* Send data */
#endif
}

int uart_recv_sem_give(int ev_num)
{
#if defined(CONFIG_FREERTOS)
	struct serial_buffer *pbuf = ur1_rxbuf;
	unsigned short tail_len;
	int datalen, stat = 0;

	vTaskEnterCritical();
	tail_len = pbuf->purbuff + pbuf->buffsize - pbuf->pin;
	if(tail_len > pbuf->space)
		tail_len = pbuf->space;

	datalen = serial_read_byte(1, UART1, pbuf->pin, tail_len, 0);//tail_len >= datalen

	pbuf->pin += datalen;

	if(pbuf->pin == (pbuf->purbuff + pbuf->buffsize))
		pbuf->pin = pbuf->purbuff;

	pbuf->space -= datalen;

	if(pbuf->space <= 0)
	{
#ifdef CONFIG_UR_FLOW_CONTROL		
		digital_write(CONFIG_UR_RTS, 1);//RTS deactive
#endif		
		uart1_rx_full = 1;
		//serial_printf("uart_rx_full\n");
	}
	else
		enable_irq(ur_irq[UART1]);

	vTaskExitCritical();
	xSemaphoreGive(uart1_rx);
#endif	
	return 0;
}

int uart_recv_sem_wait(void)
{
#if defined(CONFIG_FREERTOS)
	if(uart1_rx != NULL)
	{
		if(xSemaphoreTake(uart1_rx, 1) == pdTRUE)
		{
			if(ur1_rxbuf->space != AT_CMD_RX_BUFSIZE)
				return 0;
			else
				return -1;//no rx data
		}
		return -2;//timeout err, not obtain the semaphore
	}
	return -3;//unexpected err
#endif	
}

extern char tcprx_sig;

void at_cmd_start(void)
{
	char clibuf[128] = "";
	char *at_cmd_buf = clibuf;
	int at_cmd_len = 0, at_cmd_state = 0;
	int i = 0;
	fd_set fdsr;   
    struct timeval tv; 
    int maxsock;  
	int ret = 0;	
	while(1)
	{
		if(!uart_recv_sem_wait())
		{
			//serial_printf("\n%s trans_mode:%d \n",__FUNCTION__,trans_mode);
			if(!trans_mode)
				at_cmd_loop(at_cmd_buf, (char *)(&at_cmd_len), (char *)(&at_cmd_state));
			else/* transparent mode */
				trans_mode_loop();
		}

		if(tcprx_sig == RECEIVE_ENABLE)
		{
			FD_ZERO(&fdsr); 
	        tv.tv_sec = 0;  
	        tv.tv_usec = 0; 
			/* is it a bug */
	        for (i = 0; i < MEMP_NUM_NETCONN; i++)  
	        {  
	            if (idev->s[i] == -1)  
	            {  
	                continue;
	            }  
				FD_SET(idev->s[i], &fdsr); 
				maxsock = idev->s[i]+1;
	        }
			ret = select(maxsock, &fdsr, NULL, NULL, &tv); 
	        if (ret < 0)  
	        {  
	            serial_printf("<%s><%d>ret:%d! \n",__FUNCTION__,__LINE__,ret);  
	            break;  
	        }  
	        else if (ret == 0)  
	        {    
	            continue;  
	        } 
	        for (i = 0; i < MEMP_NUM_NETCONN; i++) 
	        {
	            if (FD_ISSET(idev->s[i], &fdsr))  
	            {  
					net_socket_reveive(i,idev->s[i]); 
	            }  			
	        }	
		}
	}
}

void at_cmd_thread(void *arg)
{
	at_cmd_start();

	vTaskDelete(NULL);
}

void at_cmd_init(void)
{
	request_irq(ur_irq[UART1], &app_uart_isr, UART1);
	REG(urbase[UART1], URCS) |= URCS_RIE;
#if defined(CONFIG_FREERTOS)
	uart1_rx = xSemaphoreCreateBinary();
	xSemaphoreTake(uart1_rx, (TickType_t)10);

	if(ur1_rxbuf == NULL)
		ur1_rxbuf = serial_buf_init(AT_CMD_RX_BUFSIZE);
	if(ur1_rxbuf == NULL)
		serial_printf("ERR: UART1 Rx Buffer NULL\n");

	EVENT_REGISTER((evt_func)uart_recv_sem_give, EVT_UART1_RX_NUM);
	init_data_buffer();

	xTaskCreate(at_cmd_thread, "atcmd", 4096, 0, 4, NULL);
#endif	
}
