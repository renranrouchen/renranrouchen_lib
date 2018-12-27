#include "I2C.h"
/** 
 * @brief   启动I2C
 * @param   无
 * @return  无
 * @note   在SCL高电平期间捕获到SDA的下降沿，使I2C开始工作     
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
 * @brief   停止I2C
 * @param   无
 * @return  无
 * @note   在SCL高电平期间捕获到SDA的上升沿，使I2C停止工作     
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
 * @brief   发送一个字节（8位）数据给I2C
 * @param   dat 无符号8位数据
 * @return  无
 * @note   在SCL上升沿写入I2C ，包括第9个字节读入应答信号	    
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
		I2C_DELAY;  // 可选延时
		SCL_CLR;
	}
	SDA_IN_EN;
	SDA_SET;
	I2C_DELAY;
	SCL_SET;
	I2C_DELAY;
	assert_param(!SDA_READ);	//读应答
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
				Temp = Temp + 2;				//这边需要查看i2c地址是加1还是加二
			}
		}
	}
}

/** 
 * @brief   读取I2C的按键码值
 * @param   无
 * @return  返回无符号8位数据
 * @note   在SCL下降沿读取I2C ，包括第9个字节发送无效应答    
 */ 
u8  I2C_RdByte( u8 ack )
{
	u8 dat,i;
	SDA_SET;
	SDA_IN_EN;
	dat = 0;
	for( i = 0; i != 8; i++ )
	{
		I2C_DELAY;  // 可选延时
		SCL_SET;
		I2C_DELAY;  // 可选延时
		dat <<= 1;
		if( SDA_READ )
		{
			I2C_DELAY;
			if( SDA_READ )	//毛刺处理
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
