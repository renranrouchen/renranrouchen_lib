#include "I2C.h"
/** 
 * @brief   ����I2C
 * @param   ��
 * @return  ��
 * @note   ��SCL�ߵ�ƽ�ڼ䲶��SDA���½��أ�ʹI2C��ʼ����     
 */ 
void I2C_Start( void )
{
	SDA_SET;  
	SDA_OUT_EN;
	SCL_SET;
	SCL_OUT_EN;
	I2C_DELAY;
	SDA_CLR;
	I2C_DELAY;      
	SCL_CLR;
}

/** 
 * @brief   ֹͣI2C
 * @param   ��
 * @return  ��
 * @note   ��SCL�ߵ�ƽ�ڼ䲶��SDA�������أ�ʹI2Cֹͣ����     
 */ 
void I2C_Stop( void )
{
	SDA_CLR;
	SDA_OUT_EN;
	I2C_DELAY;
	SCL_SET;
	I2C_DELAY;
	SDA_SET;
	I2C_DELAY;
	SDA_IN_EN;
}

/** 
 * @brief   ����һ���ֽڣ�8λ�����ݸ�I2C
 * @param   dat �޷���8λ����
 * @return  ��
 * @note   ��SCL������д��I2C ��������9���ֽڶ���Ӧ���ź�	    
 */ 
void I2C_WrByte( u8 dat )
{
	u8 i;
	SDA_OUT_EN;
	for( i = 0; i != 8; i++ )
	{
		if( dat & 0x80 ) 
		{
		    SDA_SET;
		}
		else 
		{
		    SDA_CLR;
		}
		I2C_DELAY;
		SCL_SET;
		dat <<= 1;
		I2C_DELAY;  // ��ѡ��ʱ
		SCL_CLR;
	}
	SDA_IN_EN;
	SDA_SET;
	I2C_DELAY;
	SCL_SET;
	I2C_DELAY;
	assert_param(!SDA_READ);	//��Ӧ��
	SCL_CLR;
}

void I2C_WrAddrData(u8 Addr, u8 Data)
{
	I2C_Start();
	if(FD_I2cAddrType == I2C_ADDER_TYPE1)
	{
		I2C_WrByte(I2C_CHIP_ADDR);
	}
	I2C_WrByte(Addr /* & 0xfe */);
	I2C_WrByte(Data);
	I2C_Stop();
}


void I2C_WrAddrStr(u8 Addr, u8* Str, u8 Length)
{
	u8 i;
	u8 Temp;
	if(FD_I2cContinuousEna)
	{
		
		I2C_Start();
		if(FD_I2cAddrType == I2C_ADDER_TYPE1)
		{
			I2C_WrByte(I2C_CHIP_ADDR);
		}
		I2C_WrByte(Addr /* & 0xfe */);
		for(i = 0; i < Length; i = i + 1)
		{
			I2C_WrByte(Str[i]);
		}
		I2C_Stop();
	}
	else
	{
		Temp = Addr;
		for(i = 0; i < Length; i = i + 1)
		{
			I2C_WrAddrData(Temp, Str[i]);
			if(FD_I2cAddrType == I2C_ADDER_TYPE1)
			{
				Temp = Temp + 1;
			}
			else
			{
				Temp = Temp + 2;				//�����Ҫ�鿴i2c��ַ�Ǽ�1���ǼӶ�
			}
		}
	}
}

/** 
 * @brief   ��ȡI2C�İ�����ֵ
 * @param   ��
 * @return  �����޷���8λ����
 * @note   ��SCL�½��ض�ȡI2C ��������9���ֽڷ�����ЧӦ��    
 */ 
u8  I2C_RdByte( u8 ack )
{
	u8 dat,i;
	SDA_SET;
	SDA_IN_EN;
	dat = 0;
	for( i = 0; i != 8; i++ )
	{
		I2C_DELAY;  // ��ѡ��ʱ
		SCL_SET;
		I2C_DELAY;  // ��ѡ��ʱ
		dat <<= 1;
		if( SDA_READ )
		{
			I2C_DELAY;
			if( SDA_READ )	//ë�̴���
			{
				dat++;
			}
		}
		SCL_CLR;
	}
	if(ack)
	{
		SDA_SET;
	}
	else 
	{
		SDA_CLR;
	}
	I2C_DELAY;
	SCL_SET;
	I2C_DELAY;
	SCL_CLR;
	return dat;
}

u8 I2C_RdAddrData(u8 Addr)
{
	u8 Tmp = 0;
	I2C_Start();
	if(FD_I2cAddrType == I2C_ADDER_TYPE1)
	{
		I2C_WrByte(I2C_CHIP_ADDR | 0x01);
	}
	I2C_WrByte(Addr /* | 0x01 */);
	Tmp = I2C_RdByte(1);
	I2C_Stop();
	return (Tmp);
}

void I2C_RdAddrStr(u8 Addr, u8* Str, u8 Length)
{
	u8 i;
	I2C_Start();
	if(FD_I2cAddrType == I2C_ADDER_TYPE1)
	{
		I2C_WrByte(I2C_CHIP_ADDR | 0x01);
	}
	I2C_WrByte(Addr /* | 0x01 */);
	for(i = 0; i < Length - 1; i = i + 1)
	{
		Str[i] = I2C_RdByte(0);
	}
	Str[i] = I2C_RdByte(1);
	I2C_Stop();
}
