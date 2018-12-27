#ifndef	__DS1302_H__
#define __DS1302_H__


#include "ca51f3sfr.h"
#include "ca51f3xsfr.h"
#include "gpiodef_f3.h"                                                 


sbit SCLK = P3^7;
sbit I_O = P3^6;
sbit RSTB = P3^5;

void ResetDS1302();

void WriteClkByte();
UINT8 ReadByteDS1302();
void WriteByteDS1302(UINT8);
UINT8 Read_addr_byte_ds1302(UINT8 addr);
void Write_addr_byte_ds1302(UINT8 addr,UINT8 DATA);

void ds1302_w_on(void);
void ds1302_w_off(void);
UINT8 Read_addr_byte_ds1302(UINT8 addr);
void Write_addr_byte_ds1302(UINT8 addr,UINT8 DATA);

void WriteRamByte();
void BurstReadRAM();
void BurstWriteRAM();
#endif