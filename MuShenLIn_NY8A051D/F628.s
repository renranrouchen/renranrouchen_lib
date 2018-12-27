;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.7.1 #0 (MSVC)
;--------------------------------------------------------
; NY8A port
;--------------------------------------------------------
	.file	"F628.c"
	list	p=ny8A051D,c=on
	#include "ny8A051D.inc"
	.debuginfo language C89
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_FD668_WrDisp_AddrStatic
	extern	_FD668_Init
	extern	_gpio_config
	extern	_led_init
	extern	_protect_data_enble
	extern	_ChooseFlg
	extern	_PORTBbits
	extern	_PCONbits
	extern	_BWUCONbits
	extern	_BPLCONbits
	extern	_BPHCONbits
	extern	_INTEbits
	extern	_INTFbits
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
	extern	__gptrget1

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
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	extern	_FD668_WrDisp_AddrINC
	extern	_FD668_Command
	extern	_FD668_WrByte
	extern	_Delay10us
	extern	_delay
	extern	_delay_us
	extern	_FD668_CHOOSE_STB_SET
	extern	_FD668_CHOOSE_STB_CLR
	extern	_FD668_DispData

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
r0x1014:
	.res	1
.segment "uninit"
r0x1013:
	.res	1
.segment "uninit"
r0x1015:
	.res	1
.segment "uninit"
r0x1016:
	.res	1
.segment "uninit"
r0x1017:
	.res	1
.segment "uninit"
r0x1009:
	.res	1
.segment "uninit"
r0x1008:
	.res	1
.segment "uninit"
r0x100A:
	.res	1
.segment "uninit"
r0x100B:
	.res	1
.segment "uninit"
r0x100C:
	.res	1
.segment "uninit"
r0x100D:
	.res	1
.segment "uninit"
r0x100E:
	.res	1
.segment "uninit"
r0x100F:
	.res	1
.segment "uninit"
r0x1010:
	.res	1
.segment "uninit"
r0x1011:
	.res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------

.segment "idata"
_FD668_DispData:
	dw	0x00
	dw	0x00
	dw	0x00
	dw	0x00
	dw	0x00
	dw	0x00
	dw	0x00
	.debuginfo gvariable name=_FD668_DispData,7byte,array=1,entsize=1,ext=1

;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; code
;--------------------------------------------------------
;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _FD668_Command
;   _FD668_Start
;   _FD668_WrByte
;   __gptrget1
;   _FD668_WrByte
;   _FD668_WrByte
;   _FD668_Stop
;   _FD668_Command
;   _FD668_Start
;   _FD668_WrByte
;   __gptrget1
;   _FD668_WrByte
;   _FD668_WrByte
;   _FD668_Stop
;6 compiler assigned registers:
;   r0x100F
;   r0x1010
;   r0x1011
;   STK01
;   STK00
;   r0x1012
;; Starting pCode block
.segment "code"; module=F628, function=_FD668_WrDisp_AddrINC
	.debuginfo subprogram _FD668_WrDisp_AddrINC
;local variable name mapping:
	.debuginfo variable _Addr=r0x100F
	.debuginfo variable _i=r0x100F
_FD668_WrDisp_AddrINC:
; 2 exit points
	.line	136, "F628.c"; 	char FD668_WrDisp_AddrINC(uint8 Addr )
	MOVAR	r0x100F
;;swapping arguments (AOP_TYPEs 1/2)
;;unsigned compare: left >= lit (0xF=15), size=1
	.line	139, "F628.c"; 	if(Addr>14) return(1);
	MOVIA	0x0f
	SUBAR	r0x100F,W
	BTRSS	STATUS,0
	LGOTO	_00179_DS_
;;genSkipc:3392: created from rifx:025A6160
	MOVIA	0x01
	LGOTO	_00184_DS_
_00179_DS_:
	.line	140, "F628.c"; 	FD668_Command(FD668_ADDR_INC_DIGWR_CMD);
	MOVIA	0x40
	PAGESEL	_FD668_Command
	LCALL	_FD668_Command
	PAGESEL	$
	.line	141, "F628.c"; 	FD668_Start();
	PAGESEL	_FD668_Start
	LCALL	_FD668_Start
	PAGESEL	$
	.line	142, "F628.c"; 	FD668_WrByte(FD668_DIGADDR_WRCMD|Addr);
	MOVIA	0xc0
	IORAR	r0x100F,W
	MOVAR	r0x1010
	PAGESEL	_FD668_WrByte
	LCALL	_FD668_WrByte
	PAGESEL	$
;;unsigned compare: left < lit (0x7=7), size=1
_00182_DS_:
	.line	143, "F628.c"; 	for(i=Addr;i < 7;i++)
	MOVIA	0x07
	SUBAR	r0x100F,W
	BTRSC	STATUS,0
	LGOTO	_00180_DS_
;;genSkipc:3392: created from rifx:025A6160
	.line	146, "F628.c"; 	FD668_WrByte(FD668_DispData[i]);
	MOVR	r0x100F,W
	ADDIA	(_FD668_DispData + 0)
	MOVAR	r0x1010
	MOVIA	((_FD668_DispData + 0) >> 8) & 0xff
	BTRSC	STATUS,0
	ADDIA	0x01
	MOVAR	r0x1011
	MOVR	r0x1010,W
	MOVAR	STK01
	MOVR	r0x1011,W
	MOVAR	STK00
	MOVIA	0x00
	PAGESEL	__gptrget1
	LCALL	__gptrget1
	PAGESEL	$
;;1	MOVAR	r0x1012
	PAGESEL	_FD668_WrByte
	LCALL	_FD668_WrByte
	PAGESEL	$
	.line	147, "F628.c"; 	FD668_WrByte(0x00);
	MOVIA	0x00
	PAGESEL	_FD668_WrByte
	LCALL	_FD668_WrByte
	PAGESEL	$
	.line	143, "F628.c"; 	for(i=Addr;i < 7;i++)
	INCR	r0x100F,F
	LGOTO	_00182_DS_
_00180_DS_:
	.line	149, "F628.c"; 	FD668_Stop();
	PAGESEL	_FD668_Stop
	LCALL	_FD668_Stop
	PAGESEL	$
	.line	150, "F628.c"; 	return(0);
	MOVIA	0x00
_00184_DS_:
	.line	151, "F628.c"; 	}
	RETURN	
; exit point of _FD668_WrDisp_AddrINC

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _FD668_Start
;   _FD668_WrByte
;   _FD668_Stop
;   _FD668_Start
;   _FD668_WrByte
;   _FD668_Stop
;1 compiler assigned register :
;   r0x100E
;; Starting pCode block
.segment "code"; module=F628, function=_FD668_Command
	.debuginfo subprogram _FD668_Command
;local variable name mapping:
	.debuginfo variable _CMD=r0x100E
_FD668_Command:
; 2 exit points
	.line	129, "F628.c"; 	void FD668_Command(uint8 CMD)
	MOVAR	r0x100E
	.line	131, "F628.c"; 	FD668_Start();
	PAGESEL	_FD668_Start
	LCALL	_FD668_Start
	PAGESEL	$
	.line	132, "F628.c"; 	FD668_WrByte(CMD);
	MOVR	r0x100E,W
	PAGESEL	_FD668_WrByte
	LCALL	_FD668_WrByte
	PAGESEL	$
	.line	133, "F628.c"; 	FD668_Stop();
	PAGESEL	_FD668_Stop
	LCALL	_FD668_Stop
	PAGESEL	$
	.line	134, "F628.c"; 	}
	RETURN	
; exit point of _FD668_Command

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _delay_us
;   _delay_us
;   _delay_us
;   _delay_us
;3 compiler assigned registers:
;   r0x100C
;   r0x100D
;   STK00
;; Starting pCode block
.segment "code"; module=F628, function=_FD668_WrByte
	.debuginfo subprogram _FD668_WrByte
;local variable name mapping:
	.debuginfo variable _dat=r0x100C
	.debuginfo variable _i=r0x100D
_FD668_WrByte:
; 2 exit points
	.line	68, "F628.c"; 	void FD668_WrByte( uint8 dat )
	MOVAR	r0x100C
	.line	72, "F628.c"; 	for( i = 0; i < 8; i++ )	   				/* 输出8 bit的数据 */        
	CLRR	r0x100D
_00169_DS_:
	.line	74, "F628.c"; 	FD668_CLK_CLR;					  		/* 设置CLK为低电平 */
	BCR	_PORTB,1
	.line	75, "F628.c"; 	if( dat & 0x01 ) 						/* 数据从低位到高位输出 */
	BTRSS	r0x100C,0
	LGOTO	_00166_DS_
	.line	77, "F628.c"; 	FD668_DIO_SET;		    			/* 设置DIO为高电平 */
	BSR	_PORTB,0
	LGOTO	_00167_DS_
_00166_DS_:
	.line	81, "F628.c"; 	FD668_DIO_CLR;						/* 设置DIO为低电平 */
	BCR	_PORTB,0
_00167_DS_:
	.line	83, "F628.c"; 	delay_us(1);
	MOVIA	0x01
	MOVAR	STK00
	MOVIA	0x00
	PAGESEL	_delay_us
	LCALL	_delay_us
	PAGESEL	$
	.line	85, "F628.c"; 	FD668_CLK_SET;							/* 设置SCL为高电平 上升沿写入*/
	BSR	_PORTB,1
;;shiftRight_Left2ResultLit:5458: shCount=1, size=1, sign=0, same=1, offr=0
	.line	86, "F628.c"; 	dat >>= 1;								/* 输出数据右移一位，数据从低到高的输出 */
	BCR	STATUS,0
	RRR	r0x100C,F
	.line	87, "F628.c"; 	delay_us(1);
	MOVIA	0x01
	MOVAR	STK00
	MOVIA	0x00
	PAGESEL	_delay_us
	LCALL	_delay_us
	PAGESEL	$
	.line	72, "F628.c"; 	for( i = 0; i < 8; i++ )	   				/* 输出8 bit的数据 */        
	INCR	r0x100D,F
;;unsigned compare: left < lit (0x8=8), size=1
	MOVIA	0x08
	SUBAR	r0x100D,W
	BTRSS	STATUS,0
	LGOTO	_00169_DS_
;;genSkipc:3392: created from rifx:025A6160
	.line	90, "F628.c"; 	}
	RETURN	
; exit point of _FD668_WrByte

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _Delay10us
;   _FD668_CHOOSE_STB_SET
;   _Delay10us
;   _Delay10us
;   _FD668_CHOOSE_STB_SET
;   _Delay10us
;; Starting pCode block
.segment "code"; module=F628, function=_FD668_Stop
	.debuginfo subprogram _FD668_Stop
_FD668_Stop:
; 2 exit points
	.line	53, "F628.c"; 	FD668_CLK_SET;						  /* 设置CLK为高电平 */
	BSR	_PORTB,1
	.line	54, "F628.c"; 	FD668_DELAY_STB;
	PAGESEL	_Delay10us
	LCALL	_Delay10us
	PAGESEL	$
	.line	55, "F628.c"; 	FD668_STB_SET;  					  /* 设置STB为高电平 */
	PAGESEL	_FD668_CHOOSE_STB_SET
	LCALL	_FD668_CHOOSE_STB_SET
	PAGESEL	$
	.line	56, "F628.c"; 	FD668_DIO_SET;						  /* 设置DIO为高电平 */
	BSR	_PORTB,0
	.line	58, "F628.c"; 	FD668_DELAY_BUF;					  /* 通信结束到下一次通信开始的间隔 */
	PAGESEL	_Delay10us
	LCALL	_Delay10us
	PAGESEL	$
	.line	59, "F628.c"; 	}
	RETURN	
; exit point of _FD668_Stop

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _FD668_CHOOSE_STB_CLR
;   _Delay10us
;   _FD668_CHOOSE_STB_CLR
;   _Delay10us
;; Starting pCode block
.segment "code"; module=F628, function=_FD668_Start
	.debuginfo subprogram _FD668_Start
_FD668_Start:
; 2 exit points
	.line	39, "F628.c"; 	FD668_STB_CLR;  				  /* 设置STB为低电平 */
	PAGESEL	_FD668_CHOOSE_STB_CLR
	LCALL	_FD668_CHOOSE_STB_CLR
	PAGESEL	$
	.line	42, "F628.c"; 	FD668_DELAY_STB;	
	PAGESEL	_Delay10us
	LCALL	_Delay10us
	PAGESEL	$
	.line	43, "F628.c"; 	}
	RETURN	
; exit point of _FD668_Start

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
.segment "code"; module=F628, function=_FD668_CHOOSE_STB_CLR
	.debuginfo subprogram _FD668_CHOOSE_STB_CLR
_FD668_CHOOSE_STB_CLR:
; 2 exit points
	.line	30, "F628.c"; 	if (ChooseFlg) BC(PORTB,4);
	MOVR	_ChooseFlg,W
	BTRSC	STATUS,2
	LGOTO	_00150_DS_
	BCR	_PORTB,4
	LGOTO	_00152_DS_
_00150_DS_:
	.line	31, "F628.c"; 	else 		 			 BC(PORTB,2);
	BCR	_PORTB,2
_00152_DS_:
	.line	32, "F628.c"; 	}
	RETURN	
; exit point of _FD668_CHOOSE_STB_CLR

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
.segment "code"; module=F628, function=_FD668_CHOOSE_STB_SET
	.debuginfo subprogram _FD668_CHOOSE_STB_SET
_FD668_CHOOSE_STB_SET:
; 2 exit points
	.line	24, "F628.c"; 	if (ChooseFlg) BS(PORTB,4); 
	MOVR	_ChooseFlg,W
	BTRSC	STATUS,2
	LGOTO	_00142_DS_
	BSR	_PORTB,4
	LGOTO	_00144_DS_
_00142_DS_:
	.line	25, "F628.c"; 	else 		 			 BS(PORTB,2);
	BSR	_PORTB,2
_00144_DS_:
	.line	26, "F628.c"; 	}
	RETURN	
; exit point of _FD668_CHOOSE_STB_SET

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _delay_us
;   _delay_us
;1 compiler assigned register :
;   STK00
;; Starting pCode block
.segment "code"; module=F628, function=_Delay10us
	.debuginfo subprogram _Delay10us
_Delay10us:
; 2 exit points
	.line	19, "F628.c"; 	delay_us(1);
	MOVIA	0x01
	MOVAR	STK00
	MOVIA	0x00
	PAGESEL	_delay_us
	LCALL	_delay_us
	PAGESEL	$
	.line	20, "F628.c"; 	}
	RETURN	
; exit point of _Delay10us

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;5 compiler assigned registers:
;   r0x1008
;   STK00
;   r0x1009
;   r0x100A
;   r0x100B
;; Starting pCode block
.segment "code"; module=F628, function=_delay_us
	.debuginfo subprogram _delay_us
;local variable name mapping:
	.debuginfo variable _num[0]=r0x1009
	.debuginfo variable _num[1]=r0x1008
	.debuginfo variable _kk[0]=r0x100A
	.debuginfo variable _kk[1]=r0x100B
_delay_us:
; 2 exit points
	.line	11, "F628.c"; 	void delay_us(unsigned int num)
	MOVAR	r0x1008
	MOVR	STK00,W
	MOVAR	r0x1009
	.line	14, "F628.c"; 	for(kk = 0;kk<=num;kk++);
	CLRR	r0x100A
	CLRR	r0x100B
_00125_DS_:
	INCR	r0x100A,F
	BTRSC	STATUS,2
	INCR	r0x100B,F
	MOVR	r0x100B,W
	SUBAR	r0x1008,W
	BTRSS	STATUS,2
	LGOTO	_00132_DS_
	MOVR	r0x100A,W
	SUBAR	r0x1009,W
_00132_DS_:
	BTRSC	STATUS,0
	LGOTO	_00125_DS_
;;genSkipc:3392: created from rifx:025A6160
	.line	15, "F628.c"; 	}
	RETURN	
; exit point of _delay_us

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;6 compiler assigned registers:
;   r0x1013
;   STK00
;   r0x1014
;   r0x1015
;   r0x1016
;   r0x1017
;; Starting pCode block
.segment "code"; module=F628, function=_delay
	.debuginfo subprogram _delay
;local variable name mapping:
	.debuginfo variable _count[0]=r0x1014
	.debuginfo variable _count[1]=r0x1013
	.debuginfo variable _i[0]=r0x1015
	.debuginfo variable _i[1]=r0x1016
_delay:
; 2 exit points
	.line	5, "F628.c"; 	void delay(int count) //延时21.6us
	MOVAR	r0x1013
	MOVR	STK00,W
	MOVAR	r0x1014
	.line	8, "F628.c"; 	for(i=1;i<=count;i++)
	MOVIA	0x01
	MOVAR	r0x1015
	CLRR	r0x1016
_00107_DS_:
	MOVR	r0x1013,W
	ADDIA	0x80
	MOVAR	r0x1017
	MOVR	r0x1016,W
	ADDIA	0x80
	SUBAR	r0x1017,W
	BTRSS	STATUS,2
	LGOTO	_00118_DS_
	MOVR	r0x1015,W
	SUBAR	r0x1014,W
_00118_DS_:
	BTRSS	STATUS,0
	LGOTO	_00109_DS_
;;genSkipc:3392: created from rifx:025A6160
	INCR	r0x1015,F
	BTRSC	STATUS,2
	INCR	r0x1016,F
	LGOTO	_00107_DS_
_00109_DS_:
	.line	10, "F628.c"; 	}
	RETURN	
; exit point of _delay


;	code size estimation:
;	  138+   36 =   174 instructions (  420 byte)

	end
