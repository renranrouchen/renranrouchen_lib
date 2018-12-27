;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.7.1 #0 (MSVC)
;--------------------------------------------------------
; NY8A port
;--------------------------------------------------------
	.file	"config.c"
	list	p=ny8A051D,c=on
	#include "ny8A051D.inc"
	.debuginfo language C89
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_led_init
	extern	_protect_data_enble
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
	extern	_gpio_config

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
r0x1000:
	.res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------
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
;1 compiler assigned register :
;   r0x1000
;; Starting pCode block
.segment "code"; module=config, function=_gpio_config
	.debuginfo subprogram _gpio_config
_gpio_config:
; 2 exit points
	.line	24, "config.c"; 	BC(BPHCON,7);	
	BCR	_BPHCON,7
	.line	27, "config.c"; 	BC(IOSTB,0);BS(PORTB,0); //SDA_PB0  输出高电平
	IOSTR	_IOSTB
	MOVAR	r0x1000
	BCR	r0x1000,0
	MOVR	r0x1000,W
	IOST	_IOSTB
	BSR	_PORTB,0
	.line	28, "config.c"; 	BC(IOSTB,1);BS(PORTB,1); //SCL_PB1  输出高电平
	IOSTR	_IOSTB
	MOVAR	r0x1000
	BCR	r0x1000,1
	MOVR	r0x1000,W
	IOST	_IOSTB
	BSR	_PORTB,1
	.line	30, "config.c"; 	BC(IOSTB,2);BS(PORTB,2); //ST1_PB2  输出高电平
	IOSTR	_IOSTB
	MOVAR	r0x1000
	BCR	r0x1000,2
	MOVR	r0x1000,W
	IOST	_IOSTB
	BSR	_PORTB,2
	.line	31, "config.c"; 	BC(IOSTB,4);BS(PORTB,4); //ST2_PB4  输出高电平
	IOSTR	_IOSTB
	MOVAR	r0x1000
	BCR	r0x1000,4
	MOVR	r0x1000,W
	IOST	_IOSTB
	BSR	_PORTB,4
	.line	33, "config.c"; 	BS(IOSTB,5);
	IOSTR	_IOSTB
	MOVAR	r0x1000
	BSR	r0x1000,5
	MOVR	r0x1000,W
	IOST	_IOSTB
	.line	34, "config.c"; 	BS(IOSTB,3);//key输入
	IOSTR	_IOSTB
	MOVAR	r0x1000
	BSR	r0x1000,3
	MOVR	r0x1000,W
	IOST	_IOSTB
	.line	39, "config.c"; 	}
	RETURN	
; exit point of _gpio_config


;	code size estimation:
;	   36+    0 =    36 instructions (   72 byte)

	end
