
#include "ds1302.H"
#include <intrins.h>
#include "delay.h"
void ResetDS1302()
{
		SCLK = 0;
		RSTB = 0;
		RSTB = 1;
}
#if 0
UINT8 Read_addr_byte_ds1302(UINT8 addr)
{
	ResetDS1302();
	WriteByteDS1302(addr);
	return ReadByteDS1302() ;
}

void Write_addr_byte_ds1302(UINT8 addr,UINT8 DATA)
{
		ResetDS1302();
		WriteByteDS1302(addr);
		WriteByteDS1302(DATA);
}
#endif
UINT8 ReadByteDS1302()                            //字节读取
{
		UINT8 i;
		UINT8 RByte;
		UINT8 TempByte;
		P14F=0x00;
		RByte = 0x00;
		I_O = 1;
			GPIO_Init(P36F,INPUT|PD_EN);	
				for(i=0; i<8;++i)
				{
						SCLK = 1;

					

						SCLK = 0;
					GPIO_Init(P36F,INPUT|PD_EN);
						TempByte = (UINT8)I_O;
						TempByte <<= 7;
					  RByte >>= 1;
						RByte |= TempByte;
				}
				return RByte ;
}

void WriteByteDS1302(UINT8 W_Byte)               //字节写入
{
		  UINT8 i ;
			P14F=0x00;
			GPIO_Init(P36F,OUTPUT|PD_EN);
			for (i=0;i<8;i++)
			{		SCLK =0;
					I_O =0;
					if(W_Byte &0x01) I_O =1;					
					SCLK =1;
					W_Byte >>=1;
				//Delay_50us(1);
//					nop_();
//					_nop_();
//					_nop_();
			}
}


void ds1302_w_on(void)
{
			ResetDS1302();
			WriteByteDS1302(0x8e);		                      //写保护控制寄存器
			WriteByteDS1302(0x0);														//允许写入
}

void ds1302_w_off(void)
{
			ResetDS1302();
			WriteByteDS1302(0x8e);		                      //写保护控制寄存器
			WriteByteDS1302(0x80);														//允许写入
}

UINT8 Read_addr_byte_ds1302(UINT8 addr)
{
	ResetDS1302();
	WriteByteDS1302(addr);
	return ReadByteDS1302() ;
}

void Write_addr_byte_ds1302(UINT8 addr,UINT8 DATA)
{
	ds1302_w_on();
		ResetDS1302();
		WriteByteDS1302(addr);
		WriteByteDS1302(DATA);
	ds1302_w_off();
}

#if 0
void WriteClkByte()                                                 
{
	UINT8 Address ,Data;
				
			printf("\nWrite Clock Function:");
			printf("\nWrite Clock Address:");
			scanf("%bu",&Address);
			printf("\nWrite Clock Data:");
			scanf("%bx",&Data);
			
			Address = ((Address *2) | 0x80 );									//时钟数据写入命令
			ResetDS1302();
			WriteByteDS1302(Address);
			WriteByteDS1302(Data);
			ResetDS1302();
}
#endif
#if 0
void WriteRamByte()
{
	UINT8 Address ,Data;
			
			printf("\nWrite 	RAM Function:");
			printf("\nWrite RAM Address:");
			scanf("%bx",&Address);
			printf("\nWrite RA, Data:");
			scanf("%bx",&Data);
			
			Address = ((Address *2) | 0xC0 );                  //RAM数据写入命令
			ResetDS1302();
			WriteByteDS1302(Address);
			WriteByteDS1302(Data);
			ResetDS1302();
}

#endif

#if 0
void BurstReadRAM()
{
	 UINT8 DS1302RAM[31];                 //RAM数组
	 UINT8 i;
		
		ResetDS1302();
		WriteByteDS1302(0xff);           //多字节方式读取RAM 
		for(i=0;i<31;++i)
			{
				DS1302RAM[i] =	ReadByteDS1302(); //保存数据到RAM数组
				
			}
			ResetDS1302();
			
			printf("\nDS1302 ram data:");
			
			printf("\n %2.bX 2.bX 2.bX 2.bX 2.bX 2.bX 2.bX 2.bX ",
							DS1302RAM[0],DS1302RAM[1],DS1302RAM[2],DS1302RAM[3],DS1302RAM[4],DS1302RAM[5],DS1302RAM[6],DS1302RAM[7]);
			printf("\n %2.bX 2.bX 2.bX 2.bX 2.bX 2.bX 2.bX 2.bX ",
							DS1302RAM[8],DS1302RAM[9],DS1302RAM[10],DS1302RAM[11],DS1302RAM[12],DS1302RAM[13],DS1302RAM[14],DS1302RAM[15]);
			printf("\n %2.bX 2.bX 2.bX 2.bX 2.bX 2.bX 2.bX 2.bX ",
							DS1302RAM[16],DS1302RAM[17],DS1302RAM[18],DS1302RAM[19],DS1302RAM[20],DS1302RAM[21],DS1302RAM[22],DS1302RAM[23]);
			printf("\n %2.bX 2.bX 2.bX 2.bX 2.bX 2.bX 2.bX ",
							DS1302RAM[24],DS1302RAM[25],DS1302RAM[26],DS1302RAM[27],DS1302RAM[28],DS1302RAM[29],DS1302RAM[30]);
			
			printf("\n end ram data;");
}
#endif

#if 0
void BurstWriteRAM()
{
		UINT8 Data;               
	  UINT8 i;
		
			printf("\n write ram data (hex):");
			scanf("%bx",&Data);
			
			ResetDS1302();
	    WriteByteDS1302(0xfe);           //多字节方式写入RAM 
			for(i=0;i<31;++i)
			{
					WriteByteDS1302(Data);
			}
			ResetDS1302();
}
#endif