#ifndef _I2C_H_
#define	_I2C_H_
#include "myinclude.h"

#define I2C_CHIP_ADDR 0x48	//芯片器件地址

sbit I2C_SCL = P3 ^ 5;
sbit I2C_SDA = P3 ^ 3;

//sbit I2C_SCL = P3^5;
//sbit I2C_SDA = P3^4;

#define SDA I2C_SDA
#define SCL I2C_SCL

#define nop() _nop_()	
//#define	I2C_DELAY  {if(I2cDelayCnt) AppUsart_I2cDelay();}
// #define	I2C_DELAY  {nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();}
#define	I2C_DELAY  
// #define	I2C_DELAY delayus(100)

#define SDA_OUT_EN {}
#define SDA_IN_EN {SDA = 1;}
#define SDA_SET {SDA = 1;}
#define SDA_CLR {SDA = 0;}
#define SDA_READ SDA

#define SCL_OUT_EN {}
#define SCL_SET {SCL = 1;}
#define SCL_CLR {SCL = 0;}

void I2C_WrAddrData(u8 Addr, u8 Data);
void I2C_WrAddrStr(u8 Addr, u8 * Str, u8 Length);
u8 I2C_RdAddrData(u8 Addr);
void I2C_RdAddrStr(u8 Addr, u8* Str, u8 Length);


#endif
