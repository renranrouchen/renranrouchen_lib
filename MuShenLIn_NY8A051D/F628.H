#ifndef __FD668_H__
#define __FD668_H__


typedef  unsigned char  boolean;       /* ²¼¶û±äÁ¿²¼¶û±äÁ¿*/
typedef unsigned char uint8 ;
typedef unsigned int  uint16 ;
typedef unsigned long uint32 ;



extern boolean ChooseFlg;
#define		FD668_STB_SET		{ FD668_CHOOSE_STB_SET( ); }		/* 将STB设置为高电平 */
#define		FD668_STB_CLR		{	FD668_CHOOSE_STB_CLR( ); }		/* 将STB设置为低电平 */
#define		FD668_STB_D_OUT		{}				/* 设置STB为输出方向 */

#define		FD668_CLK_SET		{BS(PORTB,1);}		/* 将CLK设置为高电平 */
#define		FD668_CLK_CLR		{BC(PORTB,1);}		/* 将CLK设置为低电平 */
#define		FD668_CLK_D_OUT		{}				/* 设置CLK为输出方向 */

#define		FD668_DIO_SET		{BS(PORTB,0);}		/* 将DIO设置为高电平 */
#define		FD668_DIO_CLR		{BC(PORTB,0);}		/* 将DIO设置为低电平 */
#define		FD668_DIO_IN		PORTB & 0X01 		/* 当DIO设为输入方向时，读取的电平高低 */
#define		FD668_DIO_D_OUT  	{BC(IOSTB,0);}				/* 设置DIO为输出方向 */
#define		FD668_DIO_D_IN   	//BS(IOSTB,0);				/* 设置DIO为输入方向 */

//数码管码字定义，可根据相应硬件连接对应
#define DISP_BIT_A  (1<<0)    //<对应A段，根据硬件的连接配置
#define DISP_BIT_B  (1<<1)    //<对应B段，根据硬件的连接配置
#define DISP_BIT_C  (1<<2)    //<对应C段，根据硬件的连接配置
#define DISP_BIT_D  (1<<3)    //<对应D段，根据硬件的连接配置
#define DISP_BIT_E  (1<<4)    //<对应E段，根据硬件的连接配置
#define DISP_BIT_F  (1<<5)    //<对应F段，根据硬件的连接配置
#define DISP_BIT_G  (1<<6)    //<对应G段，根据硬件的连接配置
#define DISP_BIT_DP (1<<7)    //<对应dp段，根据硬件的连接配置

extern void Delay10us();
#define DELAY  Delay10us();		//@11.0592MHz		///<延时时序宏，控制SCL以及SDA的速率，建议在5uS以上
	
#define FD668_DELAY DELAY

#define FD668_DISP_NONE_DATA 0X00
#define FD668_DISP_0_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD668_DISP_1_DATA  DISP_BIT_B|DISP_BIT_C
#define FD668_DISP_2_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD668_DISP_3_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_G
#define FD668_DISP_4_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_5_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_6_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_7_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C
#define FD668_DISP_8_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_9_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_A_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_a_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G 
#define FD668_DISP_B_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_b_DATA  DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_C_DATA  DISP_BIT_A|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD668_DISP_c_DATA  DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD668_DISP_d_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD668_DISP_D_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD668_DISP_E_DATA  DISP_BIT_A|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_e_DATA  DISP_BIT_A|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_F_DATA  DISP_BIT_A|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G 
#define FD668_DISP_f_DATA  FD668_DISP_F_DATA 
#define FD668_DISP_G_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G 
#define FD668_DISP_g_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_H_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G 
#define FD668_DISP_h_DATA  DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_I_DATA  DISP_BIT_B|DISP_BIT_C
#define FD668_DISP_i_DATA  DISP_BIT_C
#define FD668_DISP_J_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E 
#define FD668_DISP_j_DATA  FD668_DISP_J_DATA
#define FD668_DISP_L_DATA  DISP_BIT_D|DISP_BIT_E|DISP_BIT_F 
#define FD668_DISP_l_DATA  FD668_DISP_J_DATA
#define FD668_DISP_N_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F
#define FD668_DISP_n_DATA  DISP_BIT_C|DISP_BIT_E|DISP_BIT_G
#define FD668_DISP_O_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD668_DISP_o_DATA  DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD668_DISP_P_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_p_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_Q_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_q_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_R_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_C|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_r_DATA  DISP_BIT_B|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_S_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_s_DATA  DISP_BIT_A|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_T_DATA  DISP_BIT_A|DISP_BIT_E|DISP_BIT_F
#define FD668_DISP_t_DATA  DISP_BIT_D|DISP_BIT_E|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_U_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_E|DISP_BIT_F
#define FD668_DISP_u_DATA  DISP_BIT_C|DISP_BIT_D|DISP_BIT_E
#define FD668_DISP_Y_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_y_DATA  DISP_BIT_B|DISP_BIT_C|DISP_BIT_D|DISP_BIT_F|DISP_BIT_G
#define FD668_DISP_Z_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G
#define FD668_DISP_z_DATA  DISP_BIT_A|DISP_BIT_B|DISP_BIT_D|DISP_BIT_E|DISP_BIT_G


/* **************************************用户不需要修改*********************************************** */
/* **************写入FD668延时部分（具体含义看Datasheet）********************** */
#define 	FD668_DELAY_LOW		     	    FD668_DELAY                     		        /* 时钟低电平时间 >500ns*/
#define		FD668_DELAY_HIGH     	 	    FD668_DELAY 	   										 				/* 时钟高电平时间 >500ns*/
#define  	FD668_DELAY_BUF		 		 			FD668_DELAY	                     				  	/* 通信结束到下一次通信开始的间隔 >1us*/
#define  	FD668_DELAY_STB							FD668_DELAY


/* ***********************写入FD668操作命令***************************** */
#define FD668_KEY_RDCMD        					0x42        //按键读取命令
#define FD668_4DIG_CMD        					0x00		/*设置FD668工作在4位模式的命令*/
#define FD668_5DIG_CMD        					0x01		/*设置FD668工作在5位模式的命令*/
#define FD668_6DIG_CMD         					0x02	 	/*设置FD668工作在6位模式的命令*/
#define FD668_7DIG_CMD         					0x03	 	/*设置FD668工作在7位模式的命令*/
#define FD668_DIGADDR_WRCMD  						0xC0		//显示地址写入命令
#define FD668_ADDR_INC_DIGWR_CMD       	0x40		//地址递增方式显示数据写入
#define FD668_ADDR_STATIC_DIGWR_CMD    	0x44		//地址不递增方式显示数据写入	
#define FD668_DISP_STATUE_WRCMD        	0x80		//显示亮度写入命令
///* ********************************************************************* */

///* **************按键名和对应码值定义********************** */
//#define FD668_KEY10 	0x00000200
//#define FD668_KEY9 		0x00000100
//#define FD668_KEY8 		0x00000080
//#define FD668_KEY7 		0x00000040
//#define FD668_KEY6  	0x00000020
//#define FD668_KEY5 		0x00000010
//#define FD668_KEY4  	0x00000008
//#define FD668_KEY3  	0x00000004
//#define FD668_KEY2  	0x00000002
//#define FD668_KEY1  	0x00000001
//#define FD668_KEY_NONE_CODE 0x00

////#define FD668_DELAY_KEY_SCAN             //延时10ms
#define FD668_DISP_NORMAL	 (FD668_DISP_ON|FD668_Brightness_8 )
/* *************************用户不需要修改部分************************** */
/* ************** 控制FD668的宏 ********************** */
#define FD668_4DIG_MODE 					FD668_Command(FD668_4DIG_CMD)						/*设置FD668工作在4位模式*/
#define FD668_5DIG_MODE 					FD668_Command(FD668_5DIG_CMD)						/*设置FD668工作在5位模式*/
#define FD668_6DIG_MODE 					FD668_Command(FD668_6DIG_CMD)						/*设置FD668工作在6位模式*/
#define FD668_7DIG_MODE 					FD668_Command(FD668_7DIG_CMD)						/*设置FD668工作在7位模式*/
#define FD668_Disp_Brightness_SET(Status)	FD668_Command(FD668_DISP_STATUE_WRCMD |(Status&0x0f))   	/*设置FD668的显示方式（亮度和显示开关）*/ 
/* *************************************************************************************************************************************** *
*	Status说明	| bit7	| bit6	| bit5	| bit4	| bit3			| bit2	| bit1	| bit0	| 		 Display_EN：显示使能位，1：打开显示；0：关闭显示
*				| 0		| 0		| 0		| 0		| Display_EN	|	brightness[3:0]		|		 brightness：显示亮度控制位，000～111 分别代表着1（min）～8（max）级亮度
* ************************************************************************************************************************************* */
/* ************** Status可以使用下面的宏 （之间用或的关系） ************ */
#define FD668_DISP_ON        					0x08		/*打开FD668显示*/
#define FD668_DISP_OFF        				0x00		/*关闭FD668显示*/

#define FD668_Brightness_1        				0x00		/*设置FD668显示亮度等级为1*/
#define FD668_Brightness_2        				0x01		/*设置FD668显示亮度等级为2*/
#define FD668_Brightness_3        				0x02		/*设置FD668显示亮度等级为3*/
#define FD668_Brightness_4        				0x03		/*设置FD668显示亮度等级为4*/
#define FD668_Brightness_5        				0x04		/*设置FD668显示亮度等级为5*/
#define FD668_Brightness_6        				0x05		/*设置FD668显示亮度等级为6*/
#define FD668_Brightness_7        				0x06		/*设置FD668显示亮度等级为7*/
#define FD668_Brightness_8        				0x07		/*设置FD668显示亮度等级为8*/

//#define	FD668_WAIT_KEY_FREE		 		{while(FD668_GetKey()!=FD668_KEY_NONE_CODE);}		//等待按键释放
//#define	FD668_WAIT_KEY_PRESS			{while(FD668_GetKey()==FD668_KEY_NONE_CODE);}		//等待按键按下	 														 									//按键扫描时间 20ms
/* ****************** 函数 ************************** */ 
/****************************************************************
 *	函数的名称:					    FD668_Command
 *	描述:							发送控制命令
 *	参数:		             		uint8 控制命令
 *	返回值:				    	    void
****************************************************************/
extern		void FD668_Command(uint8);
/***************************************************************
 *	函数的名称:					    FD668_GetKey
 *	描述:										读按键码值
 *	参数:			             	void
 *	返回值:					        uint8 返回按键值 
 **************************************************************************************************************************************
返回的按键值编码  
			| 0			| 0			| 0			| 0			| 0			| 0			| KS10	| KS9		| KS8		| KS7		| KS6		| KS5		| KS4		| KS3		| KS2		| KS1		|
KEY1 	| bit15	| bit14	| bit13	| bit12	| bit11	| bit10	| bit9	| bit8	| bit7	| bit6	| bit5	| bit4	| bit3	| bit2	| bit1	| bit0	| 
KEY2 	| bit31	| bit30	| bit29	| bit28	| bit27	| bit26	| bit25	| bit24	| bit23	| bit22	| bit21	| bit20	| bit19	| bit18	| bit17	| bit16	|
***************************************************************************************************************************************/

/* /// extern	uint32 FD668_GetKey(); */

/****************************************************************
 *	函数的名称:					    FD668_WrDisp_AddrINC
 *	描述:										以地址递增模式发送显示内容
 *	参数:		         				uint8 Addr发送显示内容的起始地址；具体地址和显示对应的表格见datasheet
 *													uint8 DataLen 发送显示内容的位数
 *	返回值:				        	BOOLEAN；如果地址超出将返回1；如果执行成功返回0。
 *  使用方法：						先将数据写入FD668_DispData[]的相应位置，再调用FD668_WrDisp_AddrINC（）函数。
****************************************************************/
extern		char FD668_WrDisp_AddrINC(uint8 Addr );
/****************************************************************
 *	函数的名称:				FD668_WrDisp_AddrStatic
 *	描述:							以地址固定模式发送显示内容 ;地址表看datasheet
 *	参数:		          uint8 Addr发送显示内容的地址；
 *										uint8 DIGData 写入显示内容
 *	返回值:				    BOOLEAN；如果地址超出将返回1；如果执行成功返回0。
****************************************************************/
extern		boolean FD668_WrDisp_AddrStatic(uint8,uint8 );
/****************************************************************
 *	函数的名称:				FD668_Init
 *	描述:							FD668初始化，用户可以根据需要修改显示
 *	参数:		          void
 *	返回值:				    void
****************************************************************/ 
extern 		void    FD668_Init(void);
extern		uint8	FD668_DispData[7]; /* 显示数据寄存器,调用FD668_WrDisp_AddrINC（）前，先将数据写入FD668_DispData[]的相应位置。*/  
void delay(int count) ;
 void FD668_WrByte( uint8 dat );
/* ****************************************************************************************************** */
#endif