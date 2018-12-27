#ifndef	__DS1302_H__
#define __DS1302_H__


#include "FD814.H"                                                   


sbit SCLK = P1^4;
sbit I_O = P1^5;
sbit RSTB = P1^6;

void ResetDS1302();

void WriteClkByte();
UINT8 ReadByteDS1302();
void WriteByteDS1302(UINT8);

void WriteRamByte();
void BurstReadRAM();
void BurstWriteRAM();

void ds1302_w_on(void);
void ds1302_w_off(void);
UINT8 Read_addr_byte_ds1302(UINT8 addr);
void Write_addr_byte_ds1302(UINT8 addr,UINT8 DATA);
#endif