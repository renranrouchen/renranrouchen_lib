/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/**
 * @file serial.c
 * @brief Serial port APIs
 *
 */

#include <lynx_dev.h>
#include <event.h>
#include <serial.h>
#include <gpio.h>

extern u8	ur_irq[];
extern u32 urbase[];
extern unsigned int millis(void);
int (*usb_write)(int chan, char* pdata, int datalen);
struct serial_buffer* ur1_txbuf = NULL;//chan 0   UART1
struct serial_buffer* ur2_txbuf = NULL;//chan 1,2 UART2
unsigned int br_tb[UART_BR_TB] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200,\
								  230400, 460800, 500000, 576000, 921600, 1000000, 1152000, 1500000};
unsigned int uart_read_timeout = 0;//ms
#ifdef CONFIG_UR_FLOW_CONTROL
unsigned int  uart_rx_delay = 0;
unsigned int  uart_cts_stat = 0;
#endif

/**
 * Uart configuration
 * @param br_id baudrate table index 0 ~ 12\n
 *  0: 1200\t \t \t \t \t 1: 2400\t \t \t \t \t \t \t 2: 4800\n 3: 9600\t \t \t \t \t 4: 19200\t \t \t \t \t 5: 38400\n 6: 57600\t \t \t
 *  7: 115200\t \t \t 8: 230400\n 9: 460800\t 10: 500000\t 11: 576000\n 12: 921600\t 13: 1000000\t 14: 1152000\n 15: 1500000
 * @param parity 0: none\n 1: odd\n 2:even
 * @param stopbits 1, 2 bit
 * @param chan uart chan 0 ~ 2
 * @return None
 */
void serial_conf(int br_id, int parity, int stopbits, int chan, int threshold)
{
	int URCS_RX_TX_INT_SAT = REG(urbase[chan], URCS) & (URCS_RIE | URCS_TIE);
	int baud_rate_cnt = (CONFIG_CRYSTAL_CLK / br_tb[br_id]);

	if((br_id >= UART_BR_TB)||(br_id < 0))
		return;
	if((parity > 2)||(parity < 0))
		return;
	if((stopbits > 2)||(stopbits < 1))
		return;
	if((chan > 2)||(chan < 0))
		return;
	if((threshold > 7)||(threshold < 0))
		return;

	if(!chan)//chan 0 set GPIO0,1 = UART
		PMUREG(PKG_MODE_CTRL) &= ~HOST_IF;
	if(parity) //parity: 1 odd, 2 even
		REG(urbase[chan], URCS) = ((baud_rate_cnt - 1) << 16)
											| URCS_PE
											| ((parity - 1) << 12)
											| ((stopbits - 1) << 11)
											| URCS_RXEN
											| URCS_RX_TX_INT_SAT
											| (threshold << 4);
	else //parity: 0 none
		REG(urbase[chan], URCS) = ((baud_rate_cnt - 1) << 16)
											| ((stopbits - 1) << 11)
											| URCS_RXEN
											| URCS_RX_TX_INT_SAT
											| (threshold << 4);
#ifdef CONFIG_UR_FLOW_CONTROL
	uart_rx_delay = (3750000 * 2 * 105) / br_tb[br_id];//delay 2 frame: 2*10(bit/frame), * 2 * 10
	//serial_printf("%d rx delay %d, %dus\n", br_tb[br_id], uart_rx_delay, uart_rx_delay*10/375);//150/4 = 37.5 MHz
#endif
}

/**
 * @private
 * Get data from tx buffer and insert to uart fifo
 *
 * If fifo full, enable tx interrupt
 * @param ev_num event number
 * @return None
 */
void uart_tx_insert(int ev_num)
{
	int chan;
	struct serial_buffer* ptxbuf;

	if(ev_num == EVT_UART_CH0_TX_NUM)
	{
		chan = 0;
		ptxbuf = ur1_txbuf;
	}
	else
	{
		if(ev_num == EVT_UART_CH1_TX_NUM)
			chan = 1;
		else
		if(ev_num == EVT_UART_CH2_TX_NUM)
			chan = 2;
		ptxbuf = ur2_txbuf;
	}

#if 0 //wait tx fifo empty
	while(1)
	{
		if(ptxbuf->space == UART_TX_BUF_SIZE && ptxbuf->pout == ptxbuf->pin)
			return; //tx buffer empty

		uart_putc(chan, *ptxbuf->pout);
		ptxbuf->pout++;
		if(ptxbuf->pout == (ptxbuf->purbuff + ptxbuf->buffsize))
			ptxbuf->pout = ptxbuf->purbuff;

		ptxbuf->pout = (ptxbuf->purbuff + (ptxbuf->pout - ptxbuf->purbuff) % ptxbuf->buffsize);
		ptxbuf->space += 1;
	}
#else //no wait
	while(1)
	{
		if(ptxbuf->space >= UART_TX_BUF_SIZE && ptxbuf->pout == ptxbuf->pin)
			return; //tx buffer empty

		if(uart_no_wait_putc(chan, *ptxbuf->pout))
			break; //tx fifo full

		ptxbuf->pout++;
		if(ptxbuf->pout == (ptxbuf->purbuff + ptxbuf->buffsize))
			ptxbuf->pout = ptxbuf->purbuff;

		ptxbuf->pout = (ptxbuf->purbuff + (ptxbuf->pout - ptxbuf->purbuff) % ptxbuf->buffsize);
		ptxbuf->space += 1;
		//if(ptxbuf->space == 2048)//empty
		//	uart_putc(chan,45);
	}
	//tx fifo full
	REG(urbase[chan], URCS) |= URCS_TIE;//enable uart tx interrupt
#endif
}


/**
 * @private
 * Tx buffer initial
 * @param size buffer size
 * @return buffer struct
 */
struct serial_buffer* serial_buf_init(unsigned int size)
{
	struct serial_buffer* pbuf = (struct serial_buffer* )malloc(sizeof(struct serial_buffer));
	pbuf = (struct serial_buffer* )malloc(sizeof(struct serial_buffer));
	pbuf->buffsize = size;
	pbuf->space = pbuf->buffsize;
	pbuf->purbuff = (u8*)malloc(pbuf->buffsize);
	pbuf->pin = pbuf->purbuff;
	pbuf->pout = pbuf->purbuff;
	//pbuf->next = NULL;
	return pbuf;
}

/**
 * Serial initial
 *
 * chan 0(UART1), chan 1-2(UART2), initial tx buffer for transparent mode.
 * @param chan: uart channel 0 ~ 2
 * @return 1: success\n 0: error
 */
int serial_init(int chan)
{
	if(chan == 0)
	{
		if(ur1_txbuf == NULL)
			ur1_txbuf = serial_buf_init(UART_TX_BUF_SIZE);
		if(ur1_txbuf == NULL)
		{
			serial_printf("uart buffer init error!\n");
			return 0;
		}
		EVENT_REGISTER((evt_func)uart_tx_insert, EVT_UART_CH0_TX_NUM);
		return 1;
	}

	if(chan == 1 | chan == 2)
	{
		if(ur2_txbuf == NULL)
			ur2_txbuf = serial_buf_init(UART_TX_BUF_SIZE);
		if(ur2_txbuf == NULL)
		{
			serial_printf("uart buffer init error!\n");
			return 0;
		}
		EVENT_REGISTER((evt_func)uart_tx_insert, EVT_UART_CH1_TX_NUM);
		EVENT_REGISTER((evt_func)uart_tx_insert, EVT_UART_CH2_TX_NUM);
		return 1;
	}
	//serial_printf("purBuff %x In %x Out %x space %d\n", ptxbuf->purbuff, ptxbuf->pin , ptxbuf->pout , ptxbuf->space);
}

/**
 * Uart tx put character no wait
 * @param chan uart channal 0 ~ 2
 * @param    c character data
 * @return 0: tx fifo not full\n -1: tx fifo full
 */
int uart_no_wait_putc(int chan, int c)
{
#ifdef CONFIG_UR_FLOW_CONTROL
	if(uart_cts_stat != 0)
		return -1;
#endif
	if((REG(urbase[chan], URCS) & URCS_TF)) //1: tx full
		return -1;

	REG(urbase[chan], URBR) = c << 24;
    return 0;
}

/**
 * Uart get character, wait until time out
 * @param chan uart channel 0 ~ 2
 * @return character, -1 means no data available
 */
int uart_timeout_getc(int chan)
{
	unsigned int c;
	unsigned int start_millis;
	if(uart_read_timeout)
		start_millis = millis();

	while (!((c = REG(urbase[chan], URBR)) & (1 << 23)))
	{
		if(uart_read_timeout)
		{
			if(millis() - start_millis >= uart_read_timeout)
				goto timeout;
		}
		else
			goto timeout;
	}

	c >>= 24;
	return c;
timeout:
	return -1;
}

/**
 * Set the maximum milliseconds to wait for serial data
 * @param set_timeout time(ms)
 * @return None
 */
void uart_set_timeout(unsigned int set_timeout)
{
	uart_read_timeout = set_timeout;
}

int serial_buffer_enqueue(struct serial_buffer *pbuf, char *pdata, int datalen)
{
	int stat = 0;
	u16 tail_len = pbuf->purbuff + pbuf->buffsize - pbuf->pin;
	if(datalen == 0)
		return stat;

	if(datalen <= pbuf->space)
	{
		if(tail_len >= datalen)//do not need to loop back the queue
		{
			memcpy(pbuf->pin, pdata, datalen);
			pbuf->pin += datalen;
			pbuf->pin = (pbuf->purbuff + (pbuf->pin - pbuf->purbuff) % pbuf->buffsize);
			pbuf->space -= datalen;
		}
		else
		{
			memcpy(pbuf->pin, pdata, tail_len);
			pbuf->pin += tail_len;
			pbuf->pin = (pbuf->purbuff + (pbuf->pin - pbuf->purbuff) % pbuf->buffsize);
			pbuf->space -= tail_len;
			memcpy(pbuf->pin, pdata + tail_len, datalen - tail_len);
			pbuf->pin += (datalen - tail_len);
			pbuf->pin = (pbuf->purbuff + (pbuf->pin - pbuf->purbuff) % pbuf->buffsize);
			pbuf->space -= (datalen - tail_len);
		}
	}
	else
		stat = 1;//serial_printf("serial buffer full\n");

	return stat;
}

/**
 * Copy data to tx buffer and insert to fifo
 *
 * Need to initial txbuf, call serial_init(chan) first
 * @param    chan uart channel 0 ~ 2
 * @param   pdata rx buffer
 * @param datalen data length
 * @return stat\n
 *  0:\t \t done\n
 *  1:\t \t busy, tx buffer is full\n
 *  -1:\t fail, tx buffer is null
 */
int serial_write(int chan, char* pdata, int datalen)
{
	int stat = 0;
	struct serial_buffer* ptxbuf;

	if(chan == 0)
		ptxbuf = ur1_txbuf;
	else if(chan == 1 || chan == 2)
		ptxbuf = ur2_txbuf;
	else if(chan == 3)
	{
		if(usb_write)
			return stat = usb_write(chan, pdata, datalen);
		else
			return stat = -1;
	}

	if(ptxbuf == NULL)
		return stat = -1;

	stat = serial_buffer_enqueue(ptxbuf, pdata, datalen);

	//tx buffer insert to fifo, call serial_tx_insert
	if(chan == 0) //uart1
		ldev->raise_event(UART_CH0_TX_EVENT);//uart_tx_insert
	else
	if(chan == 1) //uart2 GPIO6-7
		ldev->raise_event(UART_CH1_TX_EVENT);
	else
	if(chan == 2) //uart2 GPIO8-9
		ldev->raise_event(UART_CH2_TX_EVENT);
	return stat;
}

/**
 * Read serial data
 * @param   mode
 *  0: read one byte\n
 *  1: read bytes\n
 *  2: read bytes until terminator character
 * @param   chan uart channel
 * @param   *buf read buffer pointer
 * @param   len data length
 * @param   end_c terminator character
 * @return
 *  mode 0: return the first byte of incoming serial data (-1 means no data available)\n
 *  mode 1, 2 :\t return data length (0 means no valid data was found)
 */
int serial_read_byte(int mode, int chan, char *buf, int len, char end_c)
{
	int rc, num = 0;
	char *rbuf = buf;
	u32 start_millis = millis();

	//serial_printf("\nmode %d start millis %d", mode, start_millis);
	if(mode != 0)
		goto read_loop;

read_one:
	rc = uart_timeout_getc(chan);
	//rc = uart_getc(chan);
	//uart_putc(chan,rc);
	return rc;

read_loop:
#ifdef CONFIG_UR_FLOW_CONTROL
	while((MFSPR(SPR_TTCR) & SPR_TTMR_PERIOD) < uart_rx_delay);
	MTSPR(SPR_SR, (MFSPR(SPR_SR) & ~SPR_SR_TEE));
#endif
	while(0 <= (rc = uart_timeout_getc(chan)))
	{
		//rc = uart_getc(chan);
		*rbuf++ = rc;
		num++;
		if((num >= len)  || (rc == end_c && mode == 2))
			goto loop_end;
	}

loop_end:
	//serial_printf("\n*buf %s len %d end_c %d\n", buf, len, end_c);
	//serial_printf("\nmode %d start millis %d\n", mode, start_millis = millis());
#ifdef CONFIG_UR_FLOW_CONTROL
	digital_write(CONFIG_UR_RTS, 0);//RTS active
#endif
	return num;
}

/*
 * Read serial string data
 *
 * Need to free buffer when the data is no longer in use
 * @param   mode serial read string mode\n
 *  0: keep reading until string end '\0'\n
 *  1: keep reading until terminator character
 * @param   chan uart channel 0 ~ 2
 * @param   end_c terminator character
 * @return  a string data buffer point\n NULL: don't have enough memory
 */
char *serial_read_string(int mode, int chan, char end_c)
{
	int rc, num = 0;
	char tmp[128];
	char *rbuf = tmp;
	//char *rbuf;

	while(0 <= (rc = uart_timeout_getc(chan)))
	{
		//rc = uart_getc(chan);
		*rbuf++ = rc;
		num++;
		if(num == 128 | (rc == end_c && mode == 1))
			break;
	}

	*rbuf = '\0';
	char *str = (char*)malloc(num * sizeof(char));
	if(str == NULL)
		return NULL;

	strcpy(str, rbuf-num);
	//serial_printf("\nstr %s slen %d num %d\n", str, strlen(str), num);
	return str;
}

#ifdef CONFIG_UR_FLOW_CONTROL
void uart_flow_cts_stat(void)
{
	uart_cts_stat = digital_read(CONFIG_UR_CTS);
	//serial_printf("CTS %d stat %d\n", CONFIG_UR_CTS, uart_cts_stat);
}
#endif
