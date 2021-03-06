/******************************************************************************
 * @file     FD612DRV.H   
 * @brief    FD612驱动的头文件
 * @version  1B3
 * @author   ywaby    
 * @date     2014-03-31
 * @note
 * @copyright  Copyright (c)2013 by 福大海矽微电子有限公司（FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD）	
 ******************************************************************************/
#ifndef _FD612DRV_H_
#define	_FD612DRV_H_
  #ifdef  FD612DRV_GROBLE
  #define FD612_EXT 
  #else
  #define FD612_EXT  extern
  #endif

	#include "FD814.H"
/** @addtogroup FD612_DRIVER FD612 驱动
  * @{
*/
/** @addtogroup  FD612_REMOVE  移植需知
  * @{
*/
/** @addtogroup  FD612_REMOVE_CHANGE  需要修改
  * 根据平台和软件需要进行相应的修改
  * @{
*/
#define FD628_NEGA_DISP       0///<1为共阴数码管，0为共阳数码管
#define	FD612_DECODE_TAB_EN   0///<是否开启FD612_DecodeTab[]功能 0为关闭,关闭该功能可以省CODE
#define	FD612_COMBINA_SW_EN     0 ///<是否开启组合键功能 0为关闭,关闭该功能可以省RAM


sbit FD612_CLK = P3^1 ;
sbit FD612_DIO = P3^0 ;

typedef  BOOLEAN ;            ///<定义布尔类型
typedef unsigned char INT8U ;  ///<定义无符号8位数类型
typedef  INT32U ;  ///<定义无符号32位数类型
#define CONST         ///<定义常量
/********FD612.C的内部宏定义 *********/
/*串行IO口设置，根据平台IO口配置*/
#define		FD612_SCL_D_OUT										///< 设置SCL接口为输出方向（最好设置为开漏输出，不可以设置成为推挽输出）		
#define		FD612_SDA_D_OUT                     ///< 设置SDA接口为输出方向（最好设置为开漏输出，不可以设置成为推挽输出）		
#define		FD612_SDA_D_IN								      ///< 设置SDA接口为输入方向（最好设置为高阻输入，不可以设置成为推挽输出）
#define		FD612_SDA_IN		   	FD612_DIO						   	///< 当SDA设为输入方向时，读取到的电平值
#define		FD612_SCL_SET		  	FD612_CLK = 1								///< 将SCL设置为高电平
#define		FD612_SCL_CLR	   		FD612_CLK = 0								///< 将SCL设置为低电平
#define		FD612_SDA_SET		   	FD612_DIO	= 1	 						///< 将SDA设置为高电平
#define		FD612_SDA_CLR		  	FD612_DIO	= 0								///< 将SDA设置为低电平
#define  FD612_DELAY_1US  		FD628_Delay10us()	; 
/* @} FD612_REMOVE_CHANGE */
/**
 * @addtogroup  FD612_REMOVE_CHANGEABLE   可以修改
 * 根据应用需要进行相应的修改 
 * 可以修改的函数 FD612_Init();FD612_Refresh();
 * @{
 */
/* @} FD612_REMOVE_CHANGEABLE */
/* @} FD612_REMOVE */
/** @addtogroup  FD612_APP 应用相关
  * @{
*/
/** @addtogroup  FD612_APP_MACRO   宏定义
  * @{
*/
/* *******************************************数码管******************************************************************************** 
 *            a
 *         -------
 *        |       |
 *      f |       | b
 *         ---g---		
 *        |       |	c
 *      e |       |	
 *         ---d---   dp
 * *************************************************************************************************************************************** *
 *码字		| 0		| 1		| 2		| 3		| 4		| 5		| 6		| 7		| 8		| 9		| A		| b		| C 	| d		| E		| F		|
 *编码		|0x3F	|0x06	|0x5B	|0x4F	|0x66	|0x6D	|0x7D	|0x07	|0x7F	|0x6F	|0x77	|0x7c	|0x39	|0x5E	|0x79	|0x71	|
 ************************************************************************************************************************************* */
 /* ********数码管显示字符宏定义 ******** */
#define		FD612_DISP_NONE 0X00
#define		FD612_DISP_0 0X3F
#define		FD612_DISP_1 0x06
#define		FD612_DISP_2 0x5B
#define		FD612_DISP_3 0x4F
#define		FD612_DISP_4 0x66
#define		FD612_DISP_5 0X6d
#define		FD612_DISP_6 0x7D
#define		FD612_DISP_7 0x07
#define		FD612_DISP_8 0x7f
#define		FD612_DISP_9 0x6F
#define		FD612_DISP_A 0X77 
#define		FD612_DISP_b 0x7c 
#define		FD612_DISP_C 0X39
#define		FD612_DISP_c 0X58
#define		FD612_DISP_d 0x5E
#define		FD612_DISP_E 0X79
#define		FD612_DISP_e 0X7b
#define		FD612_DISP_F 0x71
#define		FD612_DISP_I 0X60
#define		FD612_DISP_L 0X38
#define		FD612_DISP_r 0X72		
#define		FD612_DISP_n 0X54
#define		FD612_DISP_N 0X37
#define		FD612_DISP_O 0X3F
#define		FD612_DISP_P 0X73
#define		FD612_DISP_S 0X6d
#define		FD612_DISP_y 0X6e
#define		FD612_DISP__ 0x08
#define   FD612_DISP_H 0x76
#define   FD612_DISP_ 0x44
#define   FD612_DISP_cc 0x61
/********ACK位********/
#define FD612_NACK 1///<无应答信号
#define FD612_ACK  0///<有应答信号

#define	FD612_DISP_MAX_ADDR 11    ///<FD612显示地址的最大值
#define	FD612_DISP_MIN_ADDR 0     ///<FD612显示地址的最小值
/** @addtogroup  FD612_APP_MACRO_CMD    控制命令
  * @{
*/
//显示模式命令设置
#define FD612_8SEG_CMD	0x00		///<8段12位
#define FD612_7SEG_CMD	0x03		///<7段10位
//数据命令设置
#define FD612_ADDR_INC_DIGWR_CMD	      	0x40		    ///< 自动地址增加，写数据	
#define FD612_ADDR_STATIC_DIGWR_CMD			  0x4c		    ///< 固定地址模式。写显示
//地址命令设置
/**
 *  @brief 写入显示数据的地址的命令
 *  @note  使用方法：FD612_DIGADDR_WRCMD|相应的地址 
 *  @par Example
 *  @code
 *  FD612_Command(FD612_DIGADDR_WRCMD|FD612_DIG5_ADDR);
 *  @endcode 
 */
#define FD612_DIGADDR_WRCMD	0xc0       
//显示位相应的地址
#define FD612_DIG1_ADDR 0x00
#define FD612_DIG2_ADDR 0x01
#define FD612_DIG3_ADDR 0x02
#define FD612_DIG4_ADDR 0x03
#define FD612_DIG5_ADDR 0x04
#define FD612_DIG6_ADDR 0x05
#define FD612_DIG7_ADDR 0x06
#define FD612_DIG8_ADDR 0x07
#define FD612_DIG9_ADDR 0x08
#define FD612_DIG10_ADDR 0x09
#define FD612_DIG11_ADDR 0x0A
#define FD612_DIG12_ADDR 0x0B
//显示亮度和显示开关之间用或的关系
#define FD612_INTENS1		0x80		///< 1级亮度	
#define FD612_INTENS2		0x81		///< 2级亮度	
#define FD612_INTENS3		0x82		///< 3级亮度
#define FD612_INTENS4		0x83		///< 4级亮度	
#define FD612_INTENS5		0x84  	///< 5级亮度	
#define FD612_INTENS6		0x85		///< 6级亮度
#define FD612_INTENS7		0x86		///< 7级亮度	
#define FD612_INTENS8		0x87		///< 8级亮度

#define FD612_DISP_ON   0x88	  ///<打开FD612显示
#define FD612_DISP_OFF  0x80		///<关闭FD612显示
/* @} FD612_APP_MACRO_CMD */

#define READ_KEY			  0x42		    ///< 读按键命令

/** @brief  FD612 8段12位模式显示 */
#define FD612_8SEG_MODE   do{ \
                          FD612_DispBuff.DispSEG_MODE=FD612_8SEG_CMD;   \
                          FD612_Command(FD612_8SEG_CMD);  \
                          } while(0)
/** @brief  FD612 7段10位模式显示 */                      
#define FD612_7SEG_MODE do{  \
                          FD612_DispBuff.DispSEG_MODE=FD612_7SEG_CMD;   \               \
                          FD612_Command(FD612_7SEG_CMD);  \
                          }while(0)
/**
 *  @brief  FD612显示亮度和显示开关控制
 *  @note   显示亮度和显示开关之间使用或的关系
 *  @par Example
 *  @code
 *  FD612_DispStateWr(FD612_INTENS6|FD612_DISP_ON);
 *  @endcode 
 */                         
#define FD612_DispStateWr(DispStateTemp)  do{  \
                                        FD612_DispBuff.DispState=DispStateTemp; \
                                        FD612_Command(DispStateTemp); \
                                        }while(0)

/* @} FD612_APP_MACRO */
/** @addtogroup  FD612_APP_DATA   相关数据
  * @{
*/
FD612_EXT	CONST  INT8U DISP_TAB[0x10]; ///<数码管码值表
/* @} FD612_APP_DATA */
/** @addtogroup  FD612_APP_FUNC   相关函数
  * @{
*/
extern BOOLEAN FD612_WrByte( INT8U dat );
void FD628_Delay10us();
void FD612_loop(void);
void  FD612_Command(INT8U CMD ); //发送一字节的命令
void FD612_Init(void);  //初始化FD612芯片
#if FD612_DECODE_TAB_EN!=0
INT8U DispGetCode(char cTemp);  //数码管的查表函数
INT8U FD612_DispString( INT8U Addr,char *PStr); //某个显示地址开始显示相应的字符串
#endif
#if FD628_NEGA_DISP==1
void FD612_SingleDisp(INT8U addr,INT8U dat); //向共阴数码管某个显示地址写入一字节的显示数据
#else
void FD612_PotiveTNage(INT8U addr,INT8U dat); //向共阳数码管某个显示地址写入一字节的显示数据
#endif
void FD612_Refresh(void); //刷新FD612的相关数据
void  FD612_DispDataRefresh(void);  //刷新FD612的显示数据
/* @} FD612_APP_FUNC */
/**
 * @addtogroup  FD612_APP_DATA   相关数据
 * @{
 */ 
 static unsigned char FD612_ReadByte()	;
 unsigned long  FD612_ReadKey()  ;
 unsigned char Get_KeyCode(unsigned long dat);
typedef struct {
	INT8U DispData[FD612_DISP_MAX_ADDR-FD612_DISP_MIN_ADDR+1]; ///<12位显示数据缓存器
	INT8U DispState;      ///<显示亮度和开关状态缓存器
	INT8U DispSEG_MODE;   ///<显示位段模式缓存器
} FD612_Struct_DispBuff;

typedef struct {
	unsigned int	Key_Num;
	unsigned long Key_Code;
}Key_Code_T;
/**  
 *  @brief  显示数据和状态缓存器
 *  @note   可以通过宏FD612_DISP_BUFF_EN开启和关闭该功能
 *  @note   使用刷新功能必须先打开该功能
 */
FD612_EXT   FD612_Struct_DispBuff FD612_DispBuff;

/* @} FD612_APP_DATA */
/* @} FD612_APP */
/* @} FD612_DRIVER */
#endif
/******************* (C) COPYRIGHT 2013 FDHISI *****END OF FILE****/