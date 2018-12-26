/** 
* @file 		 FD616.H   	数码管显示 FD616 头文件
* @brief         定义了硬件相关IO 操作，以及芯片相关配置参数 
* @author        陈启俊
* @version        A1 
* @date 			2017/10/29    更新说明 :  编写注释以及新API by 陈启俊
* @copyright Copyright (c) 2012 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.              
*/
#include "ca51f3sfr.h"
#include "ca51f3xsfr.h"
#include "gpiodef_f3.h"
#include <intrins.h>

#ifndef _FD616_H_
#define	_FD616_H_



/** @addtogroup FD616_DRIVER FD616 驱动
  * @{
*/
/** @addtogroup  FD616_REMOVE  移植需知
  * @{
*/
/** @addtogroup  FD616_REMOVE_CHANGE  需要修改
  * 根据平台和软件需要进行相应的修改
  * @{
*/

					/*硬件相关*/
typedef unsigned char uint8 ;
typedef unsigned int  uint16 ;
typedef unsigned long uint32 ;



#define FD616_DECODE_TAB_EN   0  		///< 是否开启FD616_DecodeTab[]功能 0为关闭,关闭该功能可以省CODE
#define FD616_COMMON_ANODE    1			///< 1 为共阳数码管接法，0为共阴数码管接法
#define FD616_RICETYPE_DIGITAL_TUBE 1    ///< 1为米字型数码管，0为普通数码管 


#if 0
typedef    bit			  boolean	 			 
typedef    unsigned char  uint8
typedef    unsigned int   uint16	 
typedef    unsigned long  uint32  
#endif 



#define HARDWARE     0    // 0为 COB , 1为 ATB
				
/********FD616.C的内部宏定义 *********/

/*串行IO口设置，根据平台IO口配置*/

#if HARDWARE > 0
	sbit 	  FD616_SCL			=P1^7	;	  //FD6X通信时钟 IO输出 ATB
	sbit 	  FD616_SDA			=P1^7	;
#else
	sbit FD616_SCL = P1^3;//fd632 cob
	sbit FD616_SDA = P1^2;
#endif

extern void DELAY10us( )	;										///< FD616_DELAY宏所需用到的函数，in FD616.C
#define		FD616_DELAY  		DELAY10us()	   						///< 延时根据晶振时间可选,该处建议>=10uS	
#define		FD616_SCL_D_OUT		GPIO_Init(P13F,OUTPUT|PU_EN)									///< 设置SCL为输出方向,对于双向I/O需切换为输出
#define		FD616_SDA_D_OUT		GPIO_Init(P12F,OUTPUT|PU_EN)								    ///< 设置SDA为输出方向,对于双向I/O需切换为输出，若设置为推挽输出时，应答位可能产生半高（阶梯状）波形，不影响整个时序
#define		FD616_SDA_D_IN		GPIO_Init(P12F,INPUT)							///< 设置SDA为输入方向,对于双向I/O需切换为输入模式，(准双向口时需置1) 切记，该处不能设置为推挽输出。					
#define		FD616_SDA_IN		FD616_SDA	   						///< 当SDA设为输入方向时，读取到的电平值
#define		FD616_SCL_SET		FD616_SCL=1   						///< 将SCL设置为高电平
#define		FD616_SCL_CLR		FD616_SCL=0   						///< 将SCL设置为低电平
#define		FD616_SDA_SET		FD616_SDA=1   						///< 将SDA设置为高电平
#define		FD616_SDA_CLR		FD616_SDA=0   						///< 将SDA设置为低电平

/* @} FD616_REMOVE_CHANGE */
/**
 * @addtogroup  FD612_REMOVE_CHANGEABLE   可以修改
 * 根据应用需要进行相应的修改 
 * 可以修改的函数 FD612_Init();FD612_Refresh();
 * @{
 */
/* @} FD616_REMOVE_CHANGEABLE */
/* @} FD616_REMOVE */
/** @addtogroup  FD612_APP 应用相关
  * @{
*/
/** @addtogroup  FD612_APP_MACRO   宏定义
  * @{
*/


#if FD616_RICETYPE_DIGITAL_TUBE ==0
/* *******************************************???******************************************************************************** 
 *            a
 *         -------
 *        |       |
 *      f |       | b
 *         ---g---		
 *        |       |	c
 *      e |       |	
 *         ---d---   dp
 * *************************************************************************************************************************************/
#define DISP_BIT_A 	(1<<0)    //<对应的数码管a段，根据具体电路进行修改
#define DISP_BIT_B 	(1<<1)   	//<对应的数码管b段，根据具体电路进行修改
#define DISP_BIT_C 	(1<<2)   	//<对应的数码管c段，根据具体电路进行修改
#define DISP_BIT_D 	(1<<3)   	//<对应的数码管d段，根据具体电路进行修改
#define DISP_BIT_E 	(1<<4)   	//<对应的数码管e段，根据具体电路进行修改
#define DISP_BIT_F 	(1<<5)   	//<对应的数码管f段，根据具体电路进行修改
#define DISP_BIT_G 	(1<<6)   	//<对应的数码管g段，根据具体电路进行修改
#define DISP_BIT_DP (1<<7)  	//<对应的数码管dp段，根据具体电路进行修改

#define	FD616_DISP_NONE_DATA 0X00
#define FD616_DISP_0_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD616_DISP_1_DATA  DISP_BIT_B|DISP_BIT_C
#define FD616_DISP_2_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD616_DISP_3_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_G
#define FD616_DISP_4_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_5_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_6_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_7_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C
#define FD616_DISP_8_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_9_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_A_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_a_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G 
#define FD616_DISP_B_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_b_DATA  DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_C_DATA  DISP_BIT_A|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD616_DISP_c_DATA  DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD616_DISP_d_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD616_DISP_D_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD616_DISP_E_DATA  DISP_BIT_A|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_e_DATA  DISP_BIT_A|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_F_DATA  DISP_BIT_A|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G 
#define FD616_DISP_f_DATA  FD616_DISP_F_DATA 
#define FD616_DISP_G_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G 
#define FD616_DISP_g_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_H_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G 
#define FD616_DISP_h_DATA  DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_I_DATA  DISP_BIT_B|DISP_BIT_C
#define FD616_DISP_i_DATA  DISP_BIT_C
#define FD616_DISP_J_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E 
#define FD616_DISP_j_DATA  FD616_DISP_J_DATA
#define FD616_DISP_L_DATA  DISP_BIT_D|DISP_BIT_E|DISP_BIT_F 
#define FD616_DISP_l_DATA  FD616_DISP_J_DATA
#define FD616_DISP_N_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F
#define FD616_DISP_n_DATA  DISP_BIT_C|DISP_BIT_E|DISP_BIT_G
#define FD616_DISP_O_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD616_DISP_o_DATA  DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD616_DISP_P_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_p_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_Q_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_q_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_R_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_r_DATA  DISP_BIT_B|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_S_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_s_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_T_DATA  DISP_BIT_A|DISP_BIT_E|DISP_BIT_F
#define FD616_DISP_t_DATA  DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_U_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD616_DISP_u_DATA  DISP_BIT_C|DISP_BIT_D|DISP_BIT_E
#define FD616_DISP_Y_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_y_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_Z_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD616_DISP_z_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G

#else
//14段米字型数码管
#define DISP_BIT_A 	(1<<0)     //<对应的数码管a段，根据具体电路进行修改
#define DISP_BIT_B 	(1<<1)   	 //<对应的数码管b段，根据具体电路进行修改
#define DISP_BIT_C 	(1<<2)   	 //<对应的数码管c段，根据具体电路进行修改
#define DISP_BIT_D 	(1<<3)   	 //<对应的数码管d段，根据具体电路进行修改
#define DISP_BIT_E 	(1<<4)   	 //<对应的数码管e段，根据具体电路进行修改
#define DISP_BIT_F 	(1<<5)   	 //<对应的数码管f段，根据具体电路进行修改
#define DISP_BIT_G1 (1<<6) 	 //<对应的数码管g段，根据具体电路进行修改
#define DISP_BIT_G2 (1<<7)
#define DISP_BIT_H  (1<<8)
#define DISP_BIT_J  (1<<9)
#define DISP_BIT_K  (1<<10)
#define DISP_BIT_L  (1<<11)
#define DISP_BIT_M  (1<<12)
#define DISP_BIT_N	(1<<13)

#define DISP_BIT_G   DISP_BIT_G1 |DISP_BIT_G2


#define	FD616_DISP_NONE_DATA 0X00
#define FD616_DISP_0_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD616_DISP_1_DATA  DISP_BIT_B|DISP_BIT_C
#define FD616_DISP_2_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD616_DISP_3_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_G
#define FD616_DISP_4_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_5_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_6_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_7_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C
#define FD616_DISP_8_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_9_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_A_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_a_DATA  FD616_DISP_A_DATA 
#define FD616_DISP_B_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_b_DATA  DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G1|DISP_BIT_G2
#define FD616_DISP_C_DATA  DISP_BIT_A|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD616_DISP_c_DATA  DISP_BIT_D|DISP_BIT_E|DISP_BIT_G1|DISP_BIT_G2
#define FD616_DISP_d_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G1|DISP_BIT_G2
#define FD616_DISP_D_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_H|DISP_BIT_L
#define FD616_DISP_E_DATA  DISP_BIT_A|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G1|DISP_BIT_G2
#define FD616_DISP_e_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G1|DISP_BIT_G2  // gengxin
#define FD616_DISP_F_DATA  DISP_BIT_A|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G1|DISP_BIT_G2
#define FD616_DISP_f_DATA  FD616_DISP_F_DATA                                                                                                                                      
#define FD616_DISP_G_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G 
#define FD616_DISP_g_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G1|DISP_BIT_G2 
#define FD616_DISP_H_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G 
#define FD616_DISP_h_DATA  DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_I_DATA  DISP_BIT_A|DISP_BIT_D|DISP_BIT_H|DISP_BIT_L
#define FD616_DISP_i_DATA  DISP_BIT_J|DISP_BIT_M
#define FD616_DISP_J_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E 
#define FD616_DISP_j_DATA  FD616_DISP_J_DATA
#define FD616_DISP_K_DATA  DISP_BIT_E|DISP_BIT_F|DISP_BIT_G1|DISP_BIT_K|DISP_BIT_L
#define FD616_DISP_k_DATA  DISP_BIT_E|DISP_BIT_F|DISP_BIT_G1|DISP_BIT_K|DISP_BIT_L
#define FD616_DISP_L_DATA  DISP_BIT_D|DISP_BIT_E|DISP_BIT_F 
#define FD616_DISP_l_DATA  FD616_DISP_L_DATA
#define FD616_DISP_M_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_N|DISP_BIT_J|DISP_BIT_L
#define FD616_DISP_m_DATA  DISP_BIT_C|DISP_BIT_E|DISP_BIT_G1|DISP_BIT_G2|DISP_BIT_M
#define FD616_DISP_N_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_N|DISP_BIT_K
#define FD616_DISP_n_DATA  DISP_BIT_C|DISP_BIT_E|DISP_BIT_G
#define FD616_DISP_O_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD616_DISP_o_DATA  DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD616_DISP_P_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_p_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_Q_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_q_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_R_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G|DISP_BIT_K
#define FD616_DISP_r_DATA  FD616_DISP_R_DATA
#define FD616_DISP_S_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_s_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_T_DATA  DISP_BIT_A|DISP_BIT_H|DISP_BIT_L
#define FD616_DISP_t_DATA  DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G //YOU WENT
#define FD616_DISP_U_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD616_DISP_u_DATA  DISP_BIT_C|DISP_BIT_D|DISP_BIT_E
#define FD616_DISP_V_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_H|DISP_BIT_L
#define FD616_DISP_v_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_H|DISP_BIT_L
#define FD616_DISP_W_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_M|DISP_BIT_K|DISP_BIT_H
#define FD616_DISP_w_DATA  DISP_BIT_C|DISP_BIT_E|DISP_BIT_N|DISP_BIT_L
#define FD616_DISP_X_DATA  DISP_BIT_H|DISP_BIT_K|DISP_BIT_N|DISP_BIT_L
#define FD616_DISP_x_DATA  FD616_DISP_X_DATA
#define FD616_DISP_Y_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD616_DISP_y_DATA  DISP_BIT_H|DISP_BIT_K|DISP_BIT_M
#define FD616_DISP_Z_DATA  DISP_BIT_A|DISP_BIT_D|DISP_BIT_K|DISP_BIT_N
#define FD616_DISP_z_DATA  FD616_DISP_Z_DATA


#endif


				/* 硬件相关 */

//命令控制字地址
#define FD616_DISP_CMD_ADDR            0x48  //控制显示地址
#define FD616_DISP_MIN_ADDR_CMD        0x60  //最小显示地址
#define FD616_DISP_MAX_ADDR_CMD        0x7e  //最大显示地址

//命令数据
#define FD616_DISP_ON         0x01		/*打开FD616显示*/
#define FD616_DISP_OFF        0x00		/*关闭FD616显示*/
#define	FD616_DISP_MAX_ADDR    15    ///<FD616显示地址的最大值
#define	FD616_DISP_MIN_ADDR     0     ///<FD616显示地址的最小值

 
//地址命令设置

//#if (FD616_COMMON_ANODE !=0)

//		#define FD616_DIG1_ADDR			0x01		///< 数码管位1显示
//		#define FD616_DIG2_ADDR			0x02		///< 数码管位2显示
//		#define FD616_DIG3_ADDR			0x04		///< 数码管位3显示
//		#define FD616_DIG4_ADDR			0x08		///< 数码管位4显示
//		#define FD616_DIG5_ADDR			0x10		///< 数码管位5显示
//		#define FD616_DIG6_ADDR			0x20		///< 数码管位6显示
//		#define FD616_DIG7_ADDR			0x40		///< 数码管位7显示
//		#define FD616_DIG8_ADDR			0x80		///< 数码管位8显示
//		
//		#define FD616_ALL_ADDR  (FD616_DIG1_ADDR|FD616_DIG2_ADDR|FD616_DIG3_ADDR|FD616_DIG4_ADDR)
//#else

		#define FD616_DIG1_ADDR			0				///< 数码管位1显示
		#define FD616_DIG2_ADDR			1				///< 数码管位2显示
		#define FD616_DIG3_ADDR			2				///< 数码管位3显示
		#define FD616_DIG4_ADDR			3				///< 数码管位4显示
		#define FD616_DIG5_ADDR			4				///< 数码管位5显示
		#define FD616_DIG6_ADDR			5				///< 数码管位6显示
		#define FD616_DIG7_ADDR			6				///< 数码管位7显示
		#define FD616_DIG8_ADDR			7				///< 数码管位8显示
		#define FD616_DIG9_ADDR			8				///< 数码管位9显示
		#define FD616_DIG10_ADDR		9				///< 数码管位10显示
		#define FD616_DIG11_ADDR		10			///< 数码管位11显示
		#define FD616_DIG12_ADDR		11			///< 数码管位12显示
		#define FD616_DIG13_ADDR		12			///< 数码管位13显示
		#define FD616_DIG14_ADDR		13			///< 数码管位14显示
		#define FD616_DIG15_ADDR		14			///< 数码管位15显示
		#define FD616_DIG16_ADDR		15			///< 数码管位16显示

//#endif


//亮度等级设置含控制开关位已开
#define FD616_INTENS1 15
#define FD616_INTENS2 13
#define FD616_INTENS3 11
#define FD616_INTENS4 9
#define FD616_INTENS5 7
#define FD616_INTENS6 5
#define FD616_INTENS7 3
#define FD616_INTENS8 1

#define FD616_OFF 0
/**
 * @addtogroup  FD612_APP_DATA   相关数据
 * @{
 */ 
typedef struct  FD616_STRUCT_DISPBUFF
{
	//#if FD616_COMMON_ANODE!=0
	uint8 DispDataBuff;

	uint8 DispData[FD616_DISP_MAX_ADDR-FD616_DISP_MIN_ADDR+1]; ///<16位显示数据缓存器
	uint8 DispState;      ///<显示亮度和开关状态缓存器
} FD616_STRUCT_DISPBUFF;
/**  
 *  @brief  显示数据和状态缓存器
 *  @note   可以通过宏FD616_DISP_BUFF_EN开启和关闭该功能
 *  @note   使用刷新功能必须先打开该功能
 */
extern FD616_STRUCT_DISPBUFF FD616_DispBuff;




/* @} FD616_APP_DATA */

/** @addtogroup  FD612_APP_FUNC   相关函数
  * @{
*/

extern void FD616_Init(void);							//初始化FD616芯片

#if FD616_DECODE_TAB_EN!=0
extern uint16 FD616_DispGetCode(char cTemp);      		//数码管查表函数
extern uint8 FD616_StringDisp( uint8 addr ,char *PStr); // 某个显示地址开始显示相应的字符串
#endif

extern void FD616_SingleDisp( uint8 addr, uint8 dat); // 向数码管某个显示地址写入一字节的显示数据

extern void FD616_AnodeDisp( uint8 addr , uint8 dat );

extern void FD616_AnodeStringDisp(uint8 *Pstr);
/* @} FD616_APP_FUNC */

/* @} FD616_APP */
/* @} FD616_DRIVER */
void FD616_DispDataRefresh(void);
void FD616_Refresh(void);
void anode_display(UINT8 addr,uint16 Data);
void FD616_Command( uint8 dispstatetemp )	;
#endif