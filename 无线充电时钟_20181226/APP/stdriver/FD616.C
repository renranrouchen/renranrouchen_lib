/** 
* @file 		  FD616.C   	数码管显示驱动/按键扫描芯片 客户代码
* @brief       FD616 读写的实现函数、芯片基本操作函数
* @author        陈启俊
* @version        A1 
* @date 			2017/10/29    更新说明 :  编写注释以及新API by 陈启俊
* @copyright Copyright (c) 2012 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.              
*/

#include "FD616.H"

//定义结构体
FD616_STRUCT_DISPBUFF FD616_DispBuff;


#define FD616_DELAY_N_US   10
 //@22.1184MHZ 
void DELAY10us(void)   //误差 -0.008318865741us
{
 #if FD616_DELAY_N_US == 10
//    unsigned char a;
//    for(a=1;a>0;a--);
//   _nop_();  //if Keil,require use intrins.h
	
		unsigned char i;

	_nop_();
	i = 25;
	while (--i);
 #endif
  
 #if FD616_DELAY_N_US == 5
     //5uS
  unsigned char a,b;
    for(b=7;b>0;b--)
        for(a=2;a>0;a--);
 #endif
  
 #if FD616_DELAY_N_US == 1
  //1uS
    unsigned char a,b;
  for(b=1;b>0;b--)
      for(a=1;a>0;a--);
#endif    
}

/** 
 * @ingroup FD616_Scl_Sda
 * @brief   启动FD616
 * @param   无
 * @return  无
 * @note   在SCL高电平期间捕获到SDA的下降沿，使FD616开始工作     
 */ 
static void FD616_Start( void )		 	 // 操作起始
{
	FD616_SDA_D_OUT;				 //设置SDA为输出方向 
	FD616_SCL_D_OUT;				 //设置SCL为输出方向 	
	FD616_SDA_SET;  				 //发送起始条件的数据信号
	FD616_SCL_SET;
	FD616_DELAY;
	FD616_SDA_CLR;					 //发送起始信号
	FD616_DELAY;      
	FD616_SCL_CLR;					 //钳住I2C总线，准备发送或接收数据
}					     

/** 
 * @ingroup FD616_Scl_Sda
 * @brief   停止FD616
 * @param   无
 * @return  无
 * @note   在SCL高电平期间捕获到SDA的上升沿，使FD616停止工作     
 */ 

static void FD616_Stop( void )				     // 操作结束
{
	FD616_SDA_D_OUT;					 //设置SDA为输出方向 
	FD616_SCL_D_OUT;				 	 //设置SCL为输出方向
	FD616_SDA_CLR;
	FD616_DELAY;
	FD616_SCL_SET;
	FD616_DELAY;
	FD616_SDA_SET;						 //发送I2C总线结束信号
	FD616_DELAY;
}

/**
 * @ingroup FD616_Scl_Sda
 * @brief   发送一个字节（8位）数据给FD616
 * @param   dat 无符号8位数据
 * @return  无
 * @note   在SCL上升沿写入FD616 ,已包括第9个字节读入应答信号	    
 */ 
static void FD616_WriteByte( uint8 dat)		 
{
	uint8 i;
	FD616_SDA_D_OUT;
	for( i = 0; i != 8; i++ )
	{
		if( dat & 0x80 ) 
		{
		    FD616_SDA_SET;
		}
		else 
		{
		    FD616_SDA_CLR;
		}
		FD616_DELAY;
		FD616_SCL_SET;
		dat <<= 1;
		FD616_DELAY;  // 可选延时
		FD616_SCL_CLR;
	}
	
	FD616_SDA_SET;			//释放总线
	FD616_SDA_D_IN;		 //读入应答信号
	FD616_DELAY;
	FD616_SCL_SET;
	FD616_DELAY; 
	FD616_SCL_CLR;
}

/* @} FD616_Scl_Sda */


/** @addtogroup  FD616_API_FUNTION  应用函数
  @{
*/

/**
 *  @brief  向FD616发送控制显示命令
 *  @param  dispstatetemp 向FD616发送控制显示开关以及亮度调节命令
 *  @return void
 *  @note   FD616_DISP_CMD_ADDR 是控制命令中的宏0X48
 *  @par Example 
 *  @code
 *  FD616_Command( INTENS5 );
 *  @endcode 
 */
void FD616_Command( uint8 dispstatetemp )		  		
{									
	FD616_DispBuff.DispState=dispstatetemp;
	FD616_Start();
  FD616_WriteByte( FD616_DISP_CMD_ADDR );
	FD616_WriteByte( FD616_DispBuff.DispState );
	FD616_Stop();	
}


#if(FD616_DECODE_TAB_EN==1)

#define FD616_DECODE_TAB_NUM 16*4

typedef struct 
{
	uint8 Character; ///<??
	#if FD616_RICETYPE_DIGITAL_TUBE !=0
	uint16 Bitmap;
	#else
	uint8 Bitmap;     ///<???????
	#endif
} Struct_LED_Bitmap; ///<???????????????,????

/** Character conversion of digital tube display code array*/
static code Struct_LED_Bitmap FD616_DecodeTab[FD616_DECODE_TAB_NUM] = 
{	

//	{'0', 0x3F}, {'1', 0x06}, {'2', 0x5B}, {'3', 0x4F},
//	{'4', 0x66}, {'5', 0x6D}, {'6', 0x7D}, {'7', 0x07},
//	{'8', 0x7F}, {'9', 0x6F}, {'a', 0x77}, {'A', 0x77},
//	{'b', 0x7C}, {'B', 0x7C}, {'c', 0x58}, {'C', 0x39},
//	{'d', 0x5E}, {'D', 0x5E}, {'e', 0x79}, {'E', 0x79},
//	{'f', 0x71}, {'F', 0x71}, {'I', 0X60}, {'i', 0x60},
//	{'L', 0x38}, {'l', 0x38}, {'r', 0x38}, {'R', 0x38},
//	{'n', 0x54}, {'N', 0x37}, {'O', 0x3F}, {'o', 0x3f},
//	{'p', 0xf3}, {'P', 0x38}, {'S', 0x6D}, {'s', 0x6d},
//	{'y', 0x6e}, {'Y', 0x6E}, {'_', 0x08}, {0,   0x3F}, 
//	{1,   0x06}, {2,   0x5B}, {3,   0x4F}, {4,   0x66}, 
//	{5,   0x6D}, {6,   0x7D}, {7,   0x07}, {8,   0x7F}, 
//	{9,   0x6F}, {'!', 0X01}, {'@', 0X02}, {'#', 0X04},
//	{'$', 0X08}, {':', 0X10}, {'^', 0X20}, {'&', 0X40},
//	{0xC5,0X00}, {0x3b,0x18}, {0xc4,0x08}, {0x3c,0x14},
//	{0xc3,0x04}, {0x3d,0x1c}, {0xc2,0x0c},

	
	//#endif 	
	 
	{'0', FD616_DISP_0_DATA}, {'1', FD616_DISP_1_DATA}, {'2', FD616_DISP_2_DATA},{'3', FD616_DISP_3_DATA},
	{'4', FD616_DISP_4_DATA}, {'5', FD616_DISP_5_DATA}, {'6', FD616_DISP_6_DATA}, {'7', FD616_DISP_7_DATA},
	{'8', FD616_DISP_8_DATA}, {'9', FD616_DISP_9_DATA}, {'a', FD616_DISP_a_DATA}, {'A', FD616_DISP_A_DATA},
	{'b', FD616_DISP_b_DATA}, {'B', FD616_DISP_B_DATA}, {'c', FD616_DISP_c_DATA}, {'C', FD616_DISP_C_DATA},
	{'d', FD616_DISP_d_DATA}, {'D', FD616_DISP_D_DATA}, {'e', FD616_DISP_e_DATA}, {'E', FD616_DISP_E_DATA},
	{'f', FD616_DISP_f_DATA}, {'F', FD616_DISP_F_DATA}, {'g', FD616_DISP_g_DATA}, {'G', FD616_DISP_G_DATA},
	{'h', FD616_DISP_h_DATA}, {'i', FD616_DISP_i_DATA}, {'j', FD616_DISP_j_DATA}, {'k', FD616_DISP_k_DATA},
	{'l', FD616_DISP_l_DATA}, {'m', FD616_DISP_m_DATA}, {'n', FD616_DISP_n_DATA}, {'o', FD616_DISP_o_DATA},
	{'p', FD616_DISP_p_DATA}, {'q', FD616_DISP_q_DATA}, {'r', FD616_DISP_r_DATA}, {'s', FD616_DISP_s_DATA},
	{'t', FD616_DISP_t_DATA}, {'u', FD616_DISP_u_DATA}, {'v', FD616_DISP_v_DATA}, {'w', FD616_DISP_w_DATA},
	{'x', FD616_DISP_x_DATA}, {'y', FD616_DISP_y_DATA}, {'z', FD616_DISP_z_DATA},
	{'H', FD616_DISP_H_DATA}, {'I', FD616_DISP_I_DATA}, {'J', FD616_DISP_J_DATA}, {'K', FD616_DISP_K_DATA},
	{'L', FD616_DISP_L_DATA}, {'M', FD616_DISP_M_DATA}, {'N', FD616_DISP_N_DATA}, {'O', FD616_DISP_O_DATA},
	{'P', FD616_DISP_P_DATA}, {'Q', FD616_DISP_Q_DATA}, {'R', FD616_DISP_R_DATA}, {'S', FD616_DISP_S_DATA},
	{'T', FD616_DISP_T_DATA}, {'U', FD616_DISP_U_DATA}, {'V', FD616_DISP_V_DATA}, {'W', FD616_DISP_W_DATA},
	{'X', FD616_DISP_X_DATA}, {'Y', FD616_DISP_Y_DATA}, {'Z', FD616_DISP_Z_DATA},
};//BCD码字映射

#endif

/**
 *  @brief  数码管的查表函数
 *  @param  cTemp 查找的字符
 *  @retval uint8  
 *  @return 字符对应的数码管码值
 *  @note   使用该函数需要打开 FD616_DECODE_TAB_EN 
 *  @par Example
 *  @code
 *  char CTmp;
 *  CTmp=FD616_DispGetCode('S');
 *  @endcode 
 */ 
#if 0
uint16 FD616_DispGetCode(char cTemp)
{
	uint8 i;
	uint16 Bitmap=0x00;
	for(i=0; i<FD616_DECODE_TAB_NUM; i++)
	{
		if(FD616_DecodeTab[i].Character == cTemp)
		{
			Bitmap = FD616_DecodeTab[i].Bitmap;
			break;
		}
	}
	//printf("Bitmap=0x%x\n",Bitmap);
	return Bitmap;
}
#endif

  /**
 *  @brief FD616 从某个显示地址开始显示相应的字符串
 *  @param  addr 字符串显示的起始地址
 *  @param  PStr 指向相应的字符串
 *  @retval uint8  
 *  @return 返回函数执行结果,0为执行成功，1为起始地址超出最大地址
 *  @note   使用该函数需要打开 FD616_DECODE_TAB_EN 
 *  @par Example
 *  @code
 *  FD612_DispString(FD612_DIG3_ADDR,"FD612"); //从DIG3开始显示后面5位FD612
 *  @endcode 
 */ 
#if 0
uint8 FD616_StringDisp( uint8 addr ,char *PStr)
{
	uint8 i; 

	if (addr>FD616_DISP_MAX_ADDR)return 1;
	FD616_Start();	
	FD616_WriteByte(FD616_DISP_MIN_ADDR_CMD | (addr<<1)) ; 
	for(i=0;i+addr<=FD616_DISP_MAX_ADDR;i++)
	{
		if(PStr[i]=='\0')break;
		FD616_DispBuff.DispData[i+addr]=FD616_DispGetCode(PStr[i]);
		FD616_WriteByte( FD616_DispBuff.DispData[i+addr] ) ; 
	}
	FD616_Stop();
	FD616_Command( FD616_DispBuff.DispState);
	return 0;
}
#endif
/**
 *  @brief  向共阴数码管某个显示地址写入一字节的显示数据
 *  @param  Addr      写入的显示地址
 *  @param  DispData  写入的显示数据
 *  @return void
 *  @note 地址固定方式写入显示数据
 *  @par Example
 *  @code
 *  FD612_SingleDisp(FD612_DIG3_ADDR,FD616_DISP_F_DATA);
 *  @endcode
 */
#if 0
void FD616_SingleDisp( uint8 addr, uint8 dat)
{
	FD616_Start();		
	FD616_WriteByte( FD616_DISP_MIN_ADDR_CMD| (addr<<1) ) ;
	FD616_WriteByte( dat ) ;
	FD616_Stop();
  FD616_Command( FD616_DispBuff.DispState);
}
#endif

#if FD616_COMMON_ANODE !=0

/**
 *  @brief  向共阳数码管写入数据
 *  @param  dat      写入的显示数据 16位
 *  @param  dig  			写入的某位，或者显示哪位或上哪位
 *  @return void
 *  @note 地址自动加1写入
 *  @par Example 
 *  @code
 *  FD616_AnodeDisp（FD616_DIG1_ADDR, FD616_DISP_1_DATA ）;//在第一位显示数据1
 *  @endcode
 */
#if 0
void FD616_AnodeDisp( uint8 addr , uint8 dat )
{
	uint8 i;
	//616_DispBuff.DispDataBuff |=addr ;
	FD616_Start();	
	FD616_WriteByte(FD616_DISP_MIN_ADDR_CMD ) ; 
	for(i=0;i<=FD616_DISP_MAX_ADDR;i++)
	{
		if( (dat & 0x00000001) ==1)
		{
			FD616_WriteByte(  addr );
		}
		else
		{
			FD616_WriteByte( 0x00 );
		}
		dat>>=1;
	}
	FD616_Stop();
	FD616_Command( FD616_DispBuff.DispState);
}
#endif
#if 0
void FD616_AnodeDecode( char *PStr1 )
{
	uint8 i;
	uint16 xdata disptemp[8]=0;
	//c51 不支持位变量,编译器若支持位变量定义用位变量定义即可
	uint8  xdata dispbuff1[16]=0,dispbuff2[16]=0,dispbuff3[16]=0,dispbuff4[16]=0;   
	uint8  xdata dispbuff5[16]=0,dispbuff6[16]=0,dispbuff7[16]=0,dispbuff8[16]=0;

	for(i=0;i<8;i++)
	{
		disptemp[i]=FD616_DispGetCode(PStr1[i]);
	//	printf("disptemp[%bu] = 0x%04x\n",i,disptemp[i]);
	}
  for(i=0;i<16;i++)
	{

		 dispbuff1[i]=disptemp[0]& 0x0001;disptemp[0]>>=1;
		 dispbuff2[i]=disptemp[1]& 0x0001;disptemp[1]>>=1;
		 dispbuff3[i]=disptemp[2]& 0x0001;disptemp[2]>>=1;
		 dispbuff4[i]=disptemp[3]& 0x0001;disptemp[3]>>=1;
		 dispbuff5[i]=disptemp[4]& 0x0001;disptemp[4]>>=1;
		 dispbuff6[i]=disptemp[5]& 0x0001;disptemp[5]>>=1;
		 dispbuff7[i]=disptemp[6]& 0x0001;disptemp[6]>>=1;
		 dispbuff8[i]=disptemp[7]& 0x0001;disptemp[7]>>=1;
//rintf("[%bu] 1=%bu,2=%bu,3=%bu,4=%bu,5=%bu,6=%bu,7=%bu,8=%bu\n",i,dispbuff1[i],dispbuff2[i],dispbuff3[i],dispbuff4[i],dispbuff5[i],dispbuff6[i],dispbuff7[i],dispbuff8[i]);
	}	
	
	 for(i=0;i<16;i++)
	{
		FD616_DispBuff.DispData[i]=\
	(dispbuff1[i]<<0)+(dispbuff2[i]<<1)+(dispbuff3[i]<<2)+(dispbuff4[i]<<3)+(dispbuff5[i]<<4)+(dispbuff6[i]<<5)+(dispbuff7[i]<<6)+(dispbuff8[i]<<7); 
		//	printf("dispdata[%bu] = %04bx\n",i,FD616_DispBuff.DispData[i]);
	}	

}
#endif
#if 0
void FD616_AnodeStringDisp( char *PStr)
{
	uint8 i; 
  FD616_AnodeDecode(  PStr );
	FD616_Start();	
	FD616_WriteByte(FD616_DISP_MIN_ADDR_CMD ) ; 
	for(i=0;i<=FD616_DISP_MAX_ADDR;i++)
	{
		FD616_WriteByte( FD616_DispBuff.DispData[i] ) ; 
	}
	FD616_Stop();
	FD616_Command( FD616_DispBuff.DispState);
}

#endif


#endif

//uint8 code disp1234[8]={0x0d,0x0f,0x0b,0x0d,0x05,0x01,0x0c,0x00};


void FD616_Init( void)
{
	uint8 i;
	FD616_DispBuff.DispState=FD616_INTENS8;
	
	
	for(i=0;i<16;i++)
	{
		FD616_DispBuff.DispData[i]=0X00;
	}
	FD616_Refresh();
	
	
}

/* @} FD616_Api */


void FD616_Refresh(void)
{
//	FD616_Command(FD612_DispBuff.DispSEG_MODE);
	FD616_DispDataRefresh();
	FD616_Command(FD616_DispBuff.DispState);
}


void FD616_DispDataRefresh(void)
{
	uint8 i;
	FD616_Start();
	FD616_WriteByte(FD616_DISP_MIN_ADDR_CMD);			
	for(i=FD616_DISP_MIN_ADDR;i<=FD616_DISP_MAX_ADDR;i++)
	FD616_WriteByte(FD616_DispBuff.DispData[i]) ;
	FD616_Stop();
	FD616_Command( FD616_DispBuff.DispState);
}

void anode_display(UINT8 addr,uint16 Data)
{
	UINT8 i ;
	for(i=0;i<=FD616_DISP_MAX_ADDR;i++)
	{
		if( (Data & 0x01) ==1)
		{
			FD616_DispBuff.DispData[i] |= 1<<addr;
		}
		else
		{
			FD616_DispBuff.DispData[i] &= ~(1<<addr);
		}
		Data>>=1;
	}
}