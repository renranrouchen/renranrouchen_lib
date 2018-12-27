#include <NY8A051D.h>
#include <config.h>
#include "F628.H"

uint8   KeyNum =  0;	
boolean ChooseFlg = 0;
uchar  britness = 7;

#define CHOOSEONE ChooseFlg=0;
#define CHOOSETWO ChooseFlg=1;

void FD668_init(void)
{
		unsigned char i = 0;
		CHOOSEONE;
		FD668_7DIG_MODE;
		FD668_Disp_Brightness_SET(0x08 |britness);
		for(i=0;i<7;i++)
		{
			FD668_DispData[i]=0xff ;
		}
		FD668_WrDisp_AddrINC(0x00);

		CHOOSETWO;
		FD668_7DIG_MODE;
		FD668_Disp_Brightness_SET(0x08 |britness);
		for(i=0;i<7;i++)
		{
			FD668_DispData[i]=0xff ;
		}
		FD668_WrDisp_AddrINC(0x00);	
}

void FD668_DispTest( )
{ 
		uint8 state ;
		uint8 i;
		state = KeyNum ;
		switch (state)
	  {
			case 0 : {
													//状态一
									CHOOSEONE;
									FD668_7DIG_MODE;
									FD668_Disp_Brightness_SET(0x08 |britness);
									for(i=0;i<7;i++)
									{
										FD668_DispData[i]=0xff ;
									}
									FD668_WrDisp_AddrINC(0x00);

									CHOOSETWO;
									FD668_7DIG_MODE;
									FD668_Disp_Brightness_SET(0x08 |britness);
									for(i=0;i<7;i++)
									{
										FD668_DispData[i]=0xff ;
									}
									FD668_WrDisp_AddrINC(0x00);
									break;
							  }
			case 1 :  {
												//状态二
									CHOOSEONE;
									FD668_7DIG_MODE;
									FD668_Disp_Brightness_SET(0x08 |britness);
									FD668_DispData[0]=DISP_BIT_A | DISP_BIT_D| DISP_BIT_G;  
									FD668_DispData[1]=DISP_BIT_A | DISP_BIT_D| DISP_BIT_G;
									FD668_DispData[2]=DISP_BIT_A | DISP_BIT_D| DISP_BIT_G;
									FD668_DispData[3]=DISP_BIT_A | DISP_BIT_D| DISP_BIT_G;
									FD668_DispData[4]= DISP_BIT_F | DISP_BIT_C;  													  //W3 W6
									FD668_DispData[5]=DISP_BIT_F | DISP_BIT_C;; 														//W11 W14
									FD668_DispData[6]=DISP_BIT_A | DISP_BIT_B| DISP_BIT_G| DISP_BIT_DP ;	  //W17 W18 W23 W24
									FD668_WrDisp_AddrINC(0x00);

									CHOOSETWO;
									FD668_7DIG_MODE;
									FD668_Disp_Brightness_SET(0x08 |britness);
									FD668_DispData[0]=	DISP_BIT_A | DISP_BIT_D | DISP_BIT_G;
									FD668_DispData[1]=	DISP_BIT_A | DISP_BIT_D | DISP_BIT_G | DISP_BIT_DP ; //B2
									FD668_DispData[2]=	DISP_BIT_A | DISP_BIT_D | DISP_BIT_G;
									FD668_DispData[3]=	DISP_BIT_A | DISP_BIT_D | DISP_BIT_G | DISP_BIT_DP;  //B4
									FD668_DispData[4]=	DISP_BIT_B | DISP_BIT_D | DISP_BIT_E | DISP_BIT_DP;  // B6 B8 B9 B12
									FD668_DispData[5]=	DISP_BIT_A | DISP_BIT_B | DISP_BIT_E | DISP_BIT_F; 	 // W25 W26 W29 W30
									FD668_DispData[6]=	DISP_BIT_A | DISP_BIT_B;														 // DP1 DP2	
									FD668_WrDisp_AddrINC(0x00);
									break;
								}
			case  2: {
											//状态三
									CHOOSEONE;
									FD668_7DIG_MODE;
									FD668_Disp_Brightness_SET(0x08 |britness);
									FD668_DispData[0]=DISP_BIT_B | DISP_BIT_C| DISP_BIT_E | DISP_BIT_F;  
									FD668_DispData[1]=DISP_BIT_B | DISP_BIT_C| DISP_BIT_E | DISP_BIT_F;
									FD668_DispData[2]=DISP_BIT_B | DISP_BIT_C| DISP_BIT_E | DISP_BIT_F;
									FD668_DispData[3]=DISP_BIT_B | DISP_BIT_C| DISP_BIT_E | DISP_BIT_F;
									FD668_DispData[4]= DISP_BIT_A | DISP_BIT_D | DISP_BIT_G;  							//W1 W4 W7
									FD668_DispData[5]=DISP_BIT_A | DISP_BIT_D | DISP_BIT_G;								//W9 W12 W15
									FD668_DispData[6]=DISP_BIT_A | DISP_BIT_B | DISP_BIT_E| DISP_BIT_F ;	  //W17 W18 W21 W22
									FD668_WrDisp_AddrINC(0x00);



									CHOOSETWO;
									FD668_7DIG_MODE;
									FD668_Disp_Brightness_SET(0x08 |britness);
									FD668_DispData[0]=	DISP_BIT_B | DISP_BIT_C	| DISP_BIT_E | DISP_BIT_F| DISP_BIT_DP ; //B1;
									FD668_DispData[1]=	DISP_BIT_B | DISP_BIT_C	| DISP_BIT_E | DISP_BIT_F;
									FD668_DispData[2]=	DISP_BIT_B | DISP_BIT_C	| DISP_BIT_E | DISP_BIT_F| DISP_BIT_DP ; //B3;
									FD668_DispData[3]=	DISP_BIT_B | DISP_BIT_C	| DISP_BIT_E | DISP_BIT_F;
									FD668_DispData[4]=	DISP_BIT_A | DISP_BIT_C | DISP_BIT_F | DISP_BIT_G;  						 // B5 B7 B10 B11
									FD668_DispData[5]=	DISP_BIT_C | DISP_BIT_D | DISP_BIT_G | DISP_BIT_DP;  						 // W27 W28 W31 W32
									FD668_DispData[6]=	0x00;														 						 										 // DP1 DP2	
									FD668_WrDisp_AddrINC(0x00);
									break;
								}
			default : break;
		}
		
}

void main(void)
{
	
	gpio_config();
	FD668_init();
	while(1)	
    {
      
      
	 if((PORTB & 0X08) == 0)
	 {
	 	  delay(100);
	 	  if((PORTB & 0X08) == 0)
	 	  {
	 	  	KeyNum++;
	    if(KeyNum==3) KeyNum=0;
	    FD668_DispTest();
	   }
	 	  while((PORTB & 0X08) == 0);
	 }
	
	 if((PORTB & 0X20) == 0)
	 {
	 	  delay(100);
	 	  if((PORTB & 0X20) == 0)
	 	  {
	 	  	britness++;
	    if(britness>=8) britness=0;
	    FD668_DispTest();
	   }
	 	  while((PORTB & 0X20) == 0);
	 }
    }
           
}
