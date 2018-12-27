/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/**
 * @file i2c.c
 * @brief I2C APIs
 *
 */
#include <gpio.h>
#include <i2c.h>
#include <common.h>
#if 1

void i2c_start(void)
{
	digital_write(SDA, HIGH);
	digital_write(SCL, HIGH);
	pin_mode(SDA, GPIO_OUT);
	pin_mode(SCL, GPIO_OUT);
	udelay(delay_mid);
	digital_write(SDA, LOW);
	udelay(delay_mid);
	digital_write(SCL, LOW);
	udelay(delay_mid);
}


int send_byte(unsigned char byte)
{
	int s, c, ack;
	//serial_printf("byte %02x\n", byte);
	for(c = 7; c >= 0; c--)
	{
		s = (byte >> c) & 0x01;
		digital_write(SDA, s);
		udelay(delay_us);
		digital_write(SCL, HIGH);
		udelay(delay_mid);
		digital_write(SCL, LOW);
		udelay(delay_us);
	}
//after send one byte, receive ack
	pin_mode(SDA, GPIO_IN);
	digital_write(SDA, LOW);
	udelay(delay_us);
	digital_write(SCL, HIGH);
	udelay(delay_us);
	ack = digital_read(SDA);
	pin_mode(SDA, GPIO_OUT);
	//digital_write_two(SCL, LOW, SDA, HIGH);
	digital_write(SCL, LOW);
	udelay(delay_last);

	return ack;
}

void i2c_stop(void)
{
	digital_write(SDA, LOW);
	udelay(delay_us);
	digital_write(SCL, HIGH);
	udelay(delay_mid);
	digital_write(SDA, HIGH);
	udelay(delay_last);
}

/**
 * I2C master send data(1 byte).
 * @param slave_addr I2C slave address
 * @param byte data
 * @return 0: ack
 */
int i2c_send_byte(unsigned char slave_addr, unsigned char byte)
{
	int ack, i;

	slave_addr <<= 1;
	//serial_printf("slave addr %02x data %02x\n", slave_addr, byte);
	i2c_start();

	if(!(ack = send_byte(slave_addr)))
		ack = send_byte(byte);

	i2c_stop();

	//serial_printf("ack %d\n", ack);
	return ack;
}

/**
 * I2C master send string.
 * @param slave_addr I2C slave address
 * @param *str data pointer
 * @return 0: ack
 */
int i2c_send_str(unsigned char slave_addr, char *str)
{
	int ack, i;

	slave_addr <<= 1;
	i2c_start();

	if(!(ack = send_byte(slave_addr)))
	{
		for(i = 0; (*str != 0) && (i < 64); str++, i++)
			if(ack = send_byte(*str))
				break;
	}

	i2c_stop();

	return ack;
}


/**
 * I2C master send data.
 * @param slave_addr I2C slave address
 * @param *data data pointer
 * @param len data length
 * @return 0: ack
 */
int i2c_send_data(unsigned char slave_addr, char *data, int len)
{
	int ack;

	slave_addr <<= 1;
	i2c_start();

	if(!(ack = send_byte(slave_addr)))
	{
		for(; len > 0; len--, data++)
			if(ack = send_byte(*data))
				break;
	}

	i2c_stop();

	return ack;
}


/**
 * I2C master read data(1 byte).
 * @param slave_addr I2C slave address
 * @return data
 */
int i2c_read_byte(unsigned char slave_addr)
{
	unsigned char byte = 0;
	int c;
	slave_addr <<= 1;
	slave_addr += 1;
	i2c_start();
//read
	if(!(send_byte(slave_addr)))
	{
		udelay(10);
		digital_write(SDA, LOW);
		pin_mode(SDA, GPIO_IN);
		for(c = 7; c >= 0; c--)
		{
			digital_write(SCL, HIGH);
			udelay(delay_mid);
			byte |= digital_read(SDA) << c;
			if(c == 0)
				digital_write_two(SCL, LOW, SDA, HIGH);//last byte, master send NAK
			else
			{
				digital_write(SCL, LOW);
				udelay(delay_us);
			}
		}
		pin_mode(SDA, GPIO_OUT);
		udelay(delay_mid);
		digital_write(SCL, HIGH);
		udelay(delay_mid);
		digital_write(SCL, LOW);
		udelay(delay_mid);
	}

	i2c_stop();

	//serial_printf("read %02x\n", byte);
	return byte;
}


/**
 * I2C master read data.
 * @param slave_addr I2C slave address
 * @param *str read data pointer
 * @param len data length, master must know the correct length of data
 * @return None
 */
void i2c_read_data(unsigned char slave_addr, char *str, int len)
{
	unsigned char byte = 0;
	int c, dlen = len;
	slave_addr <<= 1;
	slave_addr += 1;
	i2c_start();
//read
	if(!(send_byte(slave_addr)))
	{
		digital_write(SDA, LOW);
		udelay(10);
		for(; len > 0; len--, str++)
		{
			udelay(10);
			pin_mode(SDA, GPIO_IN);
			for(c = 7, byte = 0; c >= 0; c--)
			{
				digital_write(SCL, HIGH);
				udelay(delay_mid);
				byte |= digital_read(SDA) << c;
				if(c == 0)
				{
					if(len == 1)
						digital_write_two(SCL, LOW, SDA, HIGH);//last byte, master send NAK
					else
						digital_write_two(SCL, LOW, SDA, LOW);//read next byte, master send ACK
				}
				else
				{
					digital_write(SCL, LOW);
					udelay(delay_us);
				}
			}
			pin_mode(SDA, GPIO_OUT);
			udelay(delay_mid);
			digital_write(SCL, HIGH);
			udelay(delay_mid);
			digital_write(SCL, LOW);
			udelay(delay_mid);
			*str = byte;
			//serial_printf("byte %02x\n", byte);
		}
	}
	i2c_stop();
}
#endif

