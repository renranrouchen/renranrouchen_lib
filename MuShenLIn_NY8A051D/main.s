;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.7.1 #0 (MSVC)
;--------------------------------------------------------
; NY8A port
;--------------------------------------------------------
	.file	"main.c"
	list	p=ny8A051D,c=on
	#include "ny8A051D.inc"
	.debuginfo language C89
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_gpio_config
	extern	_led_init
	extern	_protect_data_enble
	extern	_Delay10us
	extern	_FD668_Command
	extern	_FD668_WrDisp_AddrINC
	extern	_FD668_WrDisp_AddrStatic
	extern	_FD668_Init
	extern	_delay
	extern	_FD668_WrByte
	extern	_PORTBbits
	extern	_PCONbits
	extern	_BWUCONbits
	extern	_BPLCONbits
	extern	_BPHCONbits
	extern	_INTEbits
	extern	_INTFbits
	extern	_FD668_DispData
	extern	_INDF
	extern	_TMR0
	extern	_PCL
	extern	_STATUS
	extern	_FSR
	extern	_PORTB
	extern	_PCON
	extern	_BWUCON
	extern	_PCHBUF
	extern	_BPLCON
	extern	_BPHCON
	extern	_INTE
	extern	_INTF
	extern	_TMR1
	extern	_T1CR1
	extern	_T1CR2
	extern	_PWM1DUTY
	extern	_PS1CV
	extern	_BZ1CR
	extern	_IRCR
	extern	_TBHP
	extern	_TBHD
	extern	_OSCCR
	extern	_IOSTB
	extern	_PS0CV
	extern	_BODCON
	extern	_PCON1
	extern	_T0MD
	extern	__gptrput1
	extern	__sdcc_gsinit_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	extern	_FD668_init
	extern	_FD668_DispTest
	extern	_main
	extern	_KeyNum
	extern	_ChooseFlg
	extern	_britness

	extern PSAVE
	extern SSAVE
	extern WSAVE
	extern STK12
	extern STK11
	extern STK10
	extern STK09
	extern STK08
	extern STK07
	extern STK06
	extern STK05
	extern STK04
	extern STK03
	extern STK02
	extern STK01
	extern STK00

.segment "uninit", 0x20000010
PSAVE:
	.res 1
SSAVE:
	.res 1
WSAVE:
	.res 1
STK12:
	.res 1
STK11:
	.res 1
STK10:
	.res 1
STK09:
	.res 1
STK08:
	.res 1
STK07:
	.res 1
STK06:
	.res 1
STK05:
	.res 1
STK04:
	.res 1
STK03:
	.res 1
STK02:
	.res 1
STK01:
	.res 1
STK00:
	.res 1

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
.segment "uninit"
r0x100A:
	.res	1
.segment "uninit"
r0x100B:
	.res	1
.segment "uninit"
r0x100C:
	.res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------

.segment "idata"
_KeyNum:
	dw	0x00
	.debuginfo gvariable name=_KeyNum,1byte,array=0,entsize=1,ext=1


.segment "idata"
_ChooseFlg:
	dw	0x00
	.debuginfo gvariable name=_ChooseFlg,1byte,array=0,entsize=1,ext=1


.segment "idata"
_britness:
	dw	0x07
	.debuginfo gvariable name=_britness,1byte,array=0,entsize=1,ext=1

;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; reset vector 
;--------------------------------------------------------
ORG	0x0000
	goto	__sdcc_gsinit_startup
;--------------------------------------------------------
; code
;--------------------------------------------------------
;***
;  pBlock Stats: dbName = M
;***
;has an exit
;functions called:
;   _gpio_config
;   _FD668_init
;   _delay
;   _FD668_DispTest
;   _delay
;   _FD668_DispTest
;   _gpio_config
;   _FD668_init
;   _delay
;   _FD668_DispTest
;   _delay
;   _FD668_DispTest
;1 compiler assigned register :
;   STK00
;; Starting pCode block
.segment "code"; module=main, function=_main
	.debuginfo subprogram _main
_main:
; 2 exit points
	.line	126, "main.c"; 	gpio_config();
	PAGESEL	_gpio_config
	LCALL	_gpio_config
	PAGESEL	$
	.line	127, "main.c"; 	FD668_init();
	PAGESEL	_FD668_init
	LCALL	_FD668_init
	PAGESEL	$
_00169_DS_:
	.line	132, "main.c"; 	if((PORTB & 0X08) == 0)
	BTRSC	_PORTB,3
	LGOTO	_00158_DS_
	.line	134, "main.c"; 	delay(100);
	MOVIA	0x64
	MOVAR	STK00
	MOVIA	0x00
	PAGESEL	_delay
	LCALL	_delay
	PAGESEL	$
	.line	135, "main.c"; 	if((PORTB & 0X08) == 0)
	BTRSC	_PORTB,3
	LGOTO	_00154_DS_
	.line	137, "main.c"; 	KeyNum++;
	INCR	_KeyNum,F
	.line	138, "main.c"; 	if(KeyNum==3) KeyNum=0;
	MOVR	_KeyNum,W
	XORIA	0x03
	BTRSC	STATUS,2
	CLRR	_KeyNum
	.line	139, "main.c"; 	FD668_DispTest();
	PAGESEL	_FD668_DispTest
	LCALL	_FD668_DispTest
	PAGESEL	$
_00154_DS_:
	.line	141, "main.c"; 	while((PORTB & 0X08) == 0);
	BTRSS	_PORTB,3
	LGOTO	_00154_DS_
_00158_DS_:
	.line	144, "main.c"; 	if((PORTB & 0X20) == 0)
	BTRSC	_PORTB,5
	LGOTO	_00169_DS_
	.line	146, "main.c"; 	delay(100);
	MOVIA	0x64
	MOVAR	STK00
	MOVIA	0x00
	PAGESEL	_delay
	LCALL	_delay
	PAGESEL	$
	.line	147, "main.c"; 	if((PORTB & 0X20) == 0)
	BTRSC	_PORTB,5
	LGOTO	_00163_DS_
	.line	149, "main.c"; 	britness++;
	INCR	_britness,F
;;unsigned compare: left < lit (0x8=8), size=1
	.line	150, "main.c"; 	if(britness>=8) britness=0;
	MOVIA	0x08
	SUBAR	_britness,W
	BTRSC	STATUS,0
	CLRR	_britness
	.line	151, "main.c"; 	FD668_DispTest();
	PAGESEL	_FD668_DispTest
	LCALL	_FD668_DispTest
	PAGESEL	$
_00163_DS_:
	.line	153, "main.c"; 	while((PORTB & 0X20) == 0);
	BTRSS	_PORTB,5
	LGOTO	_00163_DS_
	LGOTO	_00169_DS_
	.line	157, "main.c"; 	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _FD668_Command
;   _FD668_Command
;   __gptrput1
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   __gptrput1
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   __gptrput1
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   __gptrput1
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   _FD668_WrDisp_AddrINC
;6 compiler assigned registers:
;   r0x100A
;   r0x100B
;   r0x100C
;   STK02
;   STK01
;   STK00
;; Starting pCode block
.segment "code"; module=main, function=_FD668_DispTest
	.debuginfo subprogram _FD668_DispTest
;local variable name mapping:
	.debuginfo variable _state=r0x100A
	.debuginfo variable _i=r0x100A
_FD668_DispTest:
; 2 exit points
	.line	38, "main.c"; 	state = KeyNum ;
	MOVR	_KeyNum,W
	MOVAR	r0x100A
;;swapping arguments (AOP_TYPEs 1/2)
;;unsigned compare: left >= lit (0x3=3), size=1
	.line	39, "main.c"; 	switch (state)
	MOVIA	0x03
	SUBAR	r0x100A,W
	BTRSC	STATUS,0
	LGOTO	_00125_DS_
;;genSkipc:3392: created from rifx:025A6160
	MOVIA	((_00145_DS_ >> 8) & 0xff)
	MOVAR	PCHBUF
	MOVR	r0x100A,W
	ADDIA	_00145_DS_
	BTRSC	STATUS,0
	INCR	PCHBUF,F
	MOVAR	PCL
_00145_DS_:
	LGOTO	_00114_DS_
	LGOTO	_00117_DS_
	LGOTO	_00118_DS_
_00114_DS_:
	.line	43, "main.c"; 	CHOOSEONE;
	CLRR	_ChooseFlg
	.line	44, "main.c"; 	FD668_7DIG_MODE;
	MOVIA	0x03
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	45, "main.c"; 	FD668_Disp_Brightness_SET(0x08 |britness);
	MOVIA	0x0f
	ANDAR	_britness,W
	MOVAR	r0x100A
	MOVIA	0x88
	IORAR	r0x100A,F
	MOVR	r0x100A,W
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	46, "main.c"; 	for(i=0;i<7;i++)
	CLRR	r0x100A
_00121_DS_:
	.line	48, "main.c"; 	FD668_DispData[i]=0xff ;
	MOVR	r0x100A,W
	ADDIA	(_FD668_DispData + 0)
	MOVAR	r0x100B
	MOVIA	((_FD668_DispData + 0) >> 8) & 0xff
	BTRSC	STATUS,0
	ADDIA	0x01
	MOVAR	r0x100C
	MOVIA	0xff
	MOVAR	STK02
	MOVR	r0x100B,W
	MOVAR	STK01
	MOVR	r0x100C,W
	MOVAR	STK00
	MOVIA	0x00
	PAGESEL	__gptrput1
	LCALL	__gptrput1
	PAGESEL	$
	.line	46, "main.c"; 	for(i=0;i<7;i++)
	INCR	r0x100A,F
;;unsigned compare: left < lit (0x7=7), size=1
	MOVIA	0x07
	SUBAR	r0x100A,W
	BTRSS	STATUS,0
	LGOTO	_00121_DS_
;;genSkipc:3392: created from rifx:025A6160
	.line	50, "main.c"; 	FD668_WrDisp_AddrINC(0x00);
	MOVIA	0x00
	PAGESEL	_FD668_WrDisp_AddrINC
	LCALL	_FD668_WrDisp_AddrINC
	PAGESEL	$
	.line	52, "main.c"; 	CHOOSETWO;
	MOVIA	0x01
	MOVAR	_ChooseFlg
	.line	53, "main.c"; 	FD668_7DIG_MODE;
	MOVIA	0x03
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	54, "main.c"; 	FD668_Disp_Brightness_SET(0x08 |britness);
	MOVIA	0x0f
	ANDAR	_britness,W
	MOVAR	r0x100A
	MOVIA	0x88
	IORAR	r0x100A,F
	MOVR	r0x100A,W
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	55, "main.c"; 	for(i=0;i<7;i++)
	CLRR	r0x100A
_00123_DS_:
	.line	57, "main.c"; 	FD668_DispData[i]=0xff ;
	MOVR	r0x100A,W
	ADDIA	(_FD668_DispData + 0)
	MOVAR	r0x100B
	MOVIA	((_FD668_DispData + 0) >> 8) & 0xff
	BTRSC	STATUS,0
	ADDIA	0x01
	MOVAR	r0x100C
	MOVIA	0xff
	MOVAR	STK02
	MOVR	r0x100B,W
	MOVAR	STK01
	MOVR	r0x100C,W
	MOVAR	STK00
	MOVIA	0x00
	PAGESEL	__gptrput1
	LCALL	__gptrput1
	PAGESEL	$
	.line	55, "main.c"; 	for(i=0;i<7;i++)
	INCR	r0x100A,F
;;unsigned compare: left < lit (0x7=7), size=1
	MOVIA	0x07
	SUBAR	r0x100A,W
	BTRSS	STATUS,0
	LGOTO	_00123_DS_
;;genSkipc:3392: created from rifx:025A6160
	.line	59, "main.c"; 	FD668_WrDisp_AddrINC(0x00);
	MOVIA	0x00
	PAGESEL	_FD668_WrDisp_AddrINC
	LCALL	_FD668_WrDisp_AddrINC
	PAGESEL	$
	.line	60, "main.c"; 	break;
	LGOTO	_00125_DS_
_00117_DS_:
	.line	64, "main.c"; 	CHOOSEONE;
	CLRR	_ChooseFlg
	.line	65, "main.c"; 	FD668_7DIG_MODE;
	MOVIA	0x03
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	66, "main.c"; 	FD668_Disp_Brightness_SET(0x08 |britness);
	MOVIA	0x0f
	ANDAR	_britness,W
	MOVAR	r0x100A
	MOVIA	0x88
	IORAR	r0x100A,F
	MOVR	r0x100A,W
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	67, "main.c"; 	FD668_DispData[0]=DISP_BIT_A | DISP_BIT_D| DISP_BIT_G;  
	MOVIA	0x49
	MOVAR	(_FD668_DispData + 0)
	.line	68, "main.c"; 	FD668_DispData[1]=DISP_BIT_A | DISP_BIT_D| DISP_BIT_G;
	MOVAR	(_FD668_DispData + 1)
	.line	69, "main.c"; 	FD668_DispData[2]=DISP_BIT_A | DISP_BIT_D| DISP_BIT_G;
	MOVAR	(_FD668_DispData + 2)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	70, "main.c"; 	FD668_DispData[3]=DISP_BIT_A | DISP_BIT_D| DISP_BIT_G;
	MOVIA	0x49
	MOVAR	(_FD668_DispData + 3)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	71, "main.c"; 	FD668_DispData[4]= DISP_BIT_F | DISP_BIT_C;  													  //W3 W6
	MOVIA	0x24
	MOVAR	(_FD668_DispData + 4)
	.line	72, "main.c"; 	FD668_DispData[5]=DISP_BIT_F | DISP_BIT_C;; 														//W11 W14
	MOVAR	(_FD668_DispData + 5)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	73, "main.c"; 	FD668_DispData[6]=DISP_BIT_A | DISP_BIT_B| DISP_BIT_G| DISP_BIT_DP ;	  //W17 W18 W23 W24
	MOVIA	0xc3
	MOVAR	(_FD668_DispData + 6)
	.line	74, "main.c"; 	FD668_WrDisp_AddrINC(0x00);
	MOVIA	0x00
	PAGESEL	_FD668_WrDisp_AddrINC
	LCALL	_FD668_WrDisp_AddrINC
	PAGESEL	$
	.line	76, "main.c"; 	CHOOSETWO;
	MOVIA	0x01
	MOVAR	_ChooseFlg
	.line	77, "main.c"; 	FD668_7DIG_MODE;
	MOVIA	0x03
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	78, "main.c"; 	FD668_Disp_Brightness_SET(0x08 |britness);
	MOVIA	0x0f
	ANDAR	_britness,W
	MOVAR	r0x100A
	MOVIA	0x88
	IORAR	r0x100A,F
	MOVR	r0x100A,W
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	79, "main.c"; 	FD668_DispData[0]=	DISP_BIT_A | DISP_BIT_D | DISP_BIT_G;
	MOVIA	0x49
	MOVAR	(_FD668_DispData + 0)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	80, "main.c"; 	FD668_DispData[1]=	DISP_BIT_A | DISP_BIT_D | DISP_BIT_G | DISP_BIT_DP ; //B2
	MOVIA	0xc9
	MOVAR	(_FD668_DispData + 1)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	81, "main.c"; 	FD668_DispData[2]=	DISP_BIT_A | DISP_BIT_D | DISP_BIT_G;
	MOVIA	0x49
	MOVAR	(_FD668_DispData + 2)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	82, "main.c"; 	FD668_DispData[3]=	DISP_BIT_A | DISP_BIT_D | DISP_BIT_G | DISP_BIT_DP;  //B4
	MOVIA	0xc9
	MOVAR	(_FD668_DispData + 3)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	83, "main.c"; 	FD668_DispData[4]=	DISP_BIT_B | DISP_BIT_D | DISP_BIT_E | DISP_BIT_DP;  // B6 B8 B9 B12
	MOVIA	0x9a
	MOVAR	(_FD668_DispData + 4)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	84, "main.c"; 	FD668_DispData[5]=	DISP_BIT_A | DISP_BIT_B | DISP_BIT_E | DISP_BIT_F; 	 // W25 W26 W29 W30
	MOVIA	0x33
	MOVAR	(_FD668_DispData + 5)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	85, "main.c"; 	FD668_DispData[6]=	DISP_BIT_A | DISP_BIT_B;														 // DP1 DP2	
	MOVIA	0x03
	MOVAR	(_FD668_DispData + 6)
	.line	86, "main.c"; 	FD668_WrDisp_AddrINC(0x00);
	MOVIA	0x00
	PAGESEL	_FD668_WrDisp_AddrINC
	LCALL	_FD668_WrDisp_AddrINC
	PAGESEL	$
	.line	87, "main.c"; 	break;
	LGOTO	_00125_DS_
_00118_DS_:
	.line	91, "main.c"; 	CHOOSEONE;
	CLRR	_ChooseFlg
	.line	92, "main.c"; 	FD668_7DIG_MODE;
	MOVIA	0x03
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	93, "main.c"; 	FD668_Disp_Brightness_SET(0x08 |britness);
	MOVIA	0x0f
	ANDAR	_britness,W
	MOVAR	r0x100A
	MOVIA	0x88
	IORAR	r0x100A,F
	MOVR	r0x100A,W
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	94, "main.c"; 	FD668_DispData[0]=DISP_BIT_B | DISP_BIT_C| DISP_BIT_E | DISP_BIT_F;  
	MOVIA	0x36
	MOVAR	(_FD668_DispData + 0)
	.line	95, "main.c"; 	FD668_DispData[1]=DISP_BIT_B | DISP_BIT_C| DISP_BIT_E | DISP_BIT_F;
	MOVAR	(_FD668_DispData + 1)
	.line	96, "main.c"; 	FD668_DispData[2]=DISP_BIT_B | DISP_BIT_C| DISP_BIT_E | DISP_BIT_F;
	MOVAR	(_FD668_DispData + 2)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	97, "main.c"; 	FD668_DispData[3]=DISP_BIT_B | DISP_BIT_C| DISP_BIT_E | DISP_BIT_F;
	MOVIA	0x36
	MOVAR	(_FD668_DispData + 3)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	98, "main.c"; 	FD668_DispData[4]= DISP_BIT_A | DISP_BIT_D | DISP_BIT_G;  							//W1 W4 W7
	MOVIA	0x49
	MOVAR	(_FD668_DispData + 4)
	.line	99, "main.c"; 	FD668_DispData[5]=DISP_BIT_A | DISP_BIT_D | DISP_BIT_G;								//W9 W12 W15
	MOVAR	(_FD668_DispData + 5)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	100, "main.c"; 	FD668_DispData[6]=DISP_BIT_A | DISP_BIT_B | DISP_BIT_E| DISP_BIT_F ;	  //W17 W18 W21 W22
	MOVIA	0x33
	MOVAR	(_FD668_DispData + 6)
	.line	101, "main.c"; 	FD668_WrDisp_AddrINC(0x00);
	MOVIA	0x00
	PAGESEL	_FD668_WrDisp_AddrINC
	LCALL	_FD668_WrDisp_AddrINC
	PAGESEL	$
	.line	105, "main.c"; 	CHOOSETWO;
	MOVIA	0x01
	MOVAR	_ChooseFlg
	.line	106, "main.c"; 	FD668_7DIG_MODE;
	MOVIA	0x03
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	107, "main.c"; 	FD668_Disp_Brightness_SET(0x08 |britness);
	MOVIA	0x0f
	ANDAR	_britness,W
	MOVAR	r0x100A
	MOVIA	0x88
	IORAR	r0x100A,F
	MOVR	r0x100A,W
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	108, "main.c"; 	FD668_DispData[0]=	DISP_BIT_B | DISP_BIT_C	| DISP_BIT_E | DISP_BIT_F| DISP_BIT_DP ; //B1;
	MOVIA	0xb6
	MOVAR	(_FD668_DispData + 0)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	109, "main.c"; 	FD668_DispData[1]=	DISP_BIT_B | DISP_BIT_C	| DISP_BIT_E | DISP_BIT_F;
	MOVIA	0x36
	MOVAR	(_FD668_DispData + 1)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	110, "main.c"; 	FD668_DispData[2]=	DISP_BIT_B | DISP_BIT_C	| DISP_BIT_E | DISP_BIT_F| DISP_BIT_DP ; //B3;
	MOVIA	0xb6
	MOVAR	(_FD668_DispData + 2)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	111, "main.c"; 	FD668_DispData[3]=	DISP_BIT_B | DISP_BIT_C	| DISP_BIT_E | DISP_BIT_F;
	MOVIA	0x36
	MOVAR	(_FD668_DispData + 3)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	112, "main.c"; 	FD668_DispData[4]=	DISP_BIT_A | DISP_BIT_C | DISP_BIT_F | DISP_BIT_G;  						 // B5 B7 B10 B11
	MOVIA	0x65
	MOVAR	(_FD668_DispData + 4)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	113, "main.c"; 	FD668_DispData[5]=	DISP_BIT_C | DISP_BIT_D | DISP_BIT_G | DISP_BIT_DP;  						 // W27 W28 W31 W32
	MOVIA	0xcc
	MOVAR	(_FD668_DispData + 5)
;;gen.c:6748: size=0, offset=0, AOP_TYPE (res)=8
	.line	114, "main.c"; 	FD668_DispData[6]=	0x00;														 						 										 // DP1 DP2	
	CLRR	(_FD668_DispData + 6)
	.line	115, "main.c"; 	FD668_WrDisp_AddrINC(0x00);
	MOVIA	0x00
	PAGESEL	_FD668_WrDisp_AddrINC
	LCALL	_FD668_WrDisp_AddrINC
	PAGESEL	$
_00125_DS_:
	.line	121, "main.c"; 	}
	RETURN	
; exit point of _FD668_DispTest

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _FD668_Command
;   _FD668_Command
;   __gptrput1
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   __gptrput1
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   __gptrput1
;   _FD668_WrDisp_AddrINC
;   _FD668_Command
;   _FD668_Command
;   __gptrput1
;   _FD668_WrDisp_AddrINC
;6 compiler assigned registers:
;   r0x100A
;   r0x100B
;   r0x100C
;   STK02
;   STK01
;   STK00
;; Starting pCode block
.segment "code"; module=main, function=_FD668_init
	.debuginfo subprogram _FD668_init
;local variable name mapping:
	.debuginfo variable _i=r0x100A
_FD668_init:
; 2 exit points
	.line	15, "main.c"; 	CHOOSEONE;
	CLRR	_ChooseFlg
	.line	16, "main.c"; 	FD668_7DIG_MODE;
	MOVIA	0x03
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	17, "main.c"; 	FD668_Disp_Brightness_SET(0x08 |britness);
	MOVIA	0x0f
	ANDAR	_britness,W
	MOVAR	r0x100A
	MOVIA	0x88
	IORAR	r0x100A,F
	MOVR	r0x100A,W
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	18, "main.c"; 	for(i=0;i<7;i++)
	CLRR	r0x100A
_00107_DS_:
	.line	20, "main.c"; 	FD668_DispData[i]=0xff ;
	MOVR	r0x100A,W
	ADDIA	(_FD668_DispData + 0)
	MOVAR	r0x100B
	MOVIA	((_FD668_DispData + 0) >> 8) & 0xff
	BTRSC	STATUS,0
	ADDIA	0x01
	MOVAR	r0x100C
	MOVIA	0xff
	MOVAR	STK02
	MOVR	r0x100B,W
	MOVAR	STK01
	MOVR	r0x100C,W
	MOVAR	STK00
	MOVIA	0x00
	PAGESEL	__gptrput1
	LCALL	__gptrput1
	PAGESEL	$
	.line	18, "main.c"; 	for(i=0;i<7;i++)
	INCR	r0x100A,F
;;unsigned compare: left < lit (0x7=7), size=1
	MOVIA	0x07
	SUBAR	r0x100A,W
	BTRSS	STATUS,0
	LGOTO	_00107_DS_
;;genSkipc:3392: created from rifx:025A6160
	.line	22, "main.c"; 	FD668_WrDisp_AddrINC(0x00);
	MOVIA	0x00
	PAGESEL	_FD668_WrDisp_AddrINC
	LCALL	_FD668_WrDisp_AddrINC
	PAGESEL	$
	.line	24, "main.c"; 	CHOOSETWO;
	MOVIA	0x01
	MOVAR	_ChooseFlg
	.line	25, "main.c"; 	FD668_7DIG_MODE;
	MOVIA	0x03
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	26, "main.c"; 	FD668_Disp_Brightness_SET(0x08 |britness);
	MOVIA	0x0f
	ANDAR	_britness,W
	MOVAR	r0x100A
	MOVIA	0x88
	IORAR	r0x100A,F
	MOVR	r0x100A,W
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	27, "main.c"; 	for(i=0;i<7;i++)
	CLRR	r0x100A
_00109_DS_:
	.line	29, "main.c"; 	FD668_DispData[i]=0xff ;
	MOVR	r0x100A,W
	ADDIA	(_FD668_DispData + 0)
	MOVAR	r0x100B
	MOVIA	((_FD668_DispData + 0) >> 8) & 0xff
	BTRSC	STATUS,0
	ADDIA	0x01
	MOVAR	r0x100C
	MOVIA	0xff
	MOVAR	STK02
	MOVR	r0x100B,W
	MOVAR	STK01
	MOVR	r0x100C,W
	MOVAR	STK00
	MOVIA	0x00
	PAGESEL	__gptrput1
	LCALL	__gptrput1
	PAGESEL	$
	.line	27, "main.c"; 	for(i=0;i<7;i++)
	INCR	r0x100A,F
;;unsigned compare: left < lit (0x7=7), size=1
	MOVIA	0x07
	SUBAR	r0x100A,W
	BTRSS	STATUS,0
	LGOTO	_00109_DS_
;;genSkipc:3392: created from rifx:025A6160
	.line	31, "main.c"; 	FD668_WrDisp_AddrINC(0x00);	
	MOVIA	0x00
	PAGESEL	_FD668_WrDisp_AddrINC
	LCALL	_FD668_WrDisp_AddrINC
	PAGESEL	$
	.line	32, "main.c"; 	}
	RETURN	
; exit point of _FD668_init


;	code size estimation:
;	  289+   68 =   357 instructions (  850 byte)

	end
