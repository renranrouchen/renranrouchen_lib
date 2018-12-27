/* 调试 */
/* 提供printf子程序和延时函数 */

#ifndef	__DEBUG_H__
#define __DEBUG_H__

//定义函数返回值
#ifndef  SUCCESS
#define  SUCCESS  0
#endif
#ifndef  FAIL
#define  FAIL    0xFF
#endif

//定义定时器起始
#ifndef  START
#define  START  1
#endif
#ifndef  STOP
#define  STOP    0
#endif

#ifndef  DE_PRINTF
#define  DE_PRINTF     1
#endif
#define	 FREQ_SYS	     12000000	         //系统主频12MHz
#ifndef  UART0_BUAD
#define  UART0_BUAD    57600
#define  UART1_BUAD    57600
#endif

void	CfgFsys( );                        //FD814时钟选择和配置
void	mDelayuS( UINT16 n );              // 以uS为单位延时
void	mDelaymS( UINT16 n );              // 以mS为单位延时
void  FD814UART0Alter();                 //FD814串口0引脚映射到P0.2/P0.3
void	mInitSTDIO( );                      //T1作为波特率发生器
UINT8 FD814UART0RcvByte( );              //FD814 UART0查询方式接收一个字节
void  FD814UART0SendByte(UINT8 SendDat); //FD814UART0发送一个字节

void	UART1Setup( );                     //
UINT8 FD814UART1RcvByte( );              //FD814 UART1查询方式接收一个字节
void  FD814UART1SendByte(UINT8 SendDat); //FD814UART1发送一个字节

void FD814WDTModeSelect(UINT8 mode);     //FD814看门狗模式设置 
         //FD814看门狗喂狗
#endif
