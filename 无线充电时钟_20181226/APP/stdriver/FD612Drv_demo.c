#include "ca51f3sfr.h"
#include "FD612Drv_demo.h"
#include <intrins.h>
#include "system.h"
#include "gpiodef_f3.h"
#include "ca51f3xsfr.h"
 t_fd612_ops fd612_top_ops;
 FD612_Struct_DispBuff FD612_top_DispBuff;
 t_fd612_ops fd612_base_ops;
 FD612_Struct_DispBuff FD612_base_DispBuff;

#if 0
void FD612_TOP_SCL_D_OUT()
{
	GPIO_Init(P16F,OUTPUT|PU_EN);
}
void FD612_TOP_SDA_D_OUT()
{
	GPIO_Init(P17F,OUTPUT|PU_EN);
}
void FD612_TOP_SDA_D_IN()
{
	GPIO_Init(P17F,INPUT	);
}

int FD612_TOP_SDA_IN()
{
	return FD612_DIO_A;
}

void FD612_TOP_SCL_SET()
{
	FD612_CLK_A = 1	;
}

void FD612_TOP_SCL_CLR()
{
	FD612_CLK_A = 0	;
}

void FD612_TOP_SDA_SET()
{
	FD612_DIO_A	= 1;
}

void FD612_TOP_SDA_CLR()
{
	FD612_DIO_A	= 0;
}
#endif
void FD612_BASE_SCL_D_OUT()
{
	GPIO_Init(P15F,OUTPUT|PU_EN);
}
void FD612_BASE_SDA_D_OUT()
{
	GPIO_Init(P14F,OUTPUT|PU_EN);
}
void FD612_BASE_SDA_D_IN()
{
	GPIO_Init(P14F,INPUT);
}

int FD612_BASE_SDA_IN()
{
	return FD612_DIO_B;
}

void FD612_BASE_SCL_SET()
{
	FD612_CLK_B = 1	;
}

void FD612_BASE_SCL_CLR()
{
	FD612_CLK_B = 0	;
}

void FD612_BASE_SDA_SET()
{
	FD612_DIO_B = 1	;	
}

void FD612_BASE_SDA_CLR()
{
	FD612_DIO_B = 0	;	
}


void FD612_app_init()
{
	unsigned char loop;
		
//	fd612_top_ops.FD612_SCL_CLR = FD612_TOP_SCL_CLR;
//	fd612_top_ops.FD612_SCL_D_OUT = FD612_TOP_SCL_D_OUT;
//	fd612_top_ops.FD612_SCL_SET = FD612_TOP_SCL_SET;
//	fd612_top_ops.FD612_SDA_CLR = FD612_TOP_SDA_CLR;
//	fd612_top_ops.FD612_SDA_D_IN = FD612_TOP_SDA_D_IN;
//	fd612_top_ops.FD612_SDA_D_OUT =FD612_TOP_SDA_D_OUT;
//	fd612_top_ops.FD612_SDA_IN = FD612_TOP_SDA_IN;
//	fd612_top_ops.FD612_SDA_SET = FD612_TOP_SDA_SET;

	fd612_base_ops.FD612_SCL_CLR = FD612_BASE_SCL_CLR;
	fd612_base_ops.FD612_SCL_D_OUT = FD612_BASE_SCL_D_OUT;
	fd612_base_ops.FD612_SCL_SET = FD612_BASE_SCL_SET;
	fd612_base_ops.FD612_SDA_CLR = FD612_BASE_SDA_CLR;
	fd612_base_ops.FD612_SDA_D_IN = FD612_BASE_SDA_D_IN;
	fd612_base_ops.FD612_SDA_D_OUT =FD612_BASE_SDA_D_OUT;
	fd612_base_ops.FD612_SDA_IN = FD612_BASE_SDA_IN;
	fd612_base_ops.FD612_SDA_SET = FD612_BASE_SDA_SET;

	FD612_Init(&fd612_base_ops,&FD612_base_DispBuff);
//	FD612_Init(&fd612_top_ops,&FD612_top_DispBuff);
#if 0	
	for(loop = 0;loop < 12;loop++){
		FD612_top_DispBuff.DispData[loop] = 0xff;
	}
#endif	
//	FD612_Top_Refresh(&FD612_top_DispBuff,&fd612_top_ops);
#if 1

	for(loop = 0;loop < 12;loop++){
		FD612_base_DispBuff.DispData[loop] = 0X00;
	}
#endif	
//	FD612_Top_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
	FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
	
//						FD612_base_DispBuff.DispState=FD612_DISP_ON|FD612_INTENS5;
//					FD612_Command(&fd612_base_ops,FD612_base_DispBuff.DispState);
}

#if 0
void FD612_TEST(void)
{
//		FD612_base_DispBuff.DispData[FD612_DIG5_ADDR]=FD612_DISP_5; 
//		FD612_base_DispBuff.DispData[FD612_DIG6_ADDR]=FD612_DISP_6;
//		FD612_base_DispBuff.DispData[FD612_DIG7_ADDR]=FD612_DISP_7; 
//		FD612_base_DispBuff.DispData[FD612_DIG8_ADDR]=FD612_DISP_8; 
//		FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
//		FD612_PotiveTNage(FD612_DIG1_ADDR,0xff, &FD612_base_DispBuff);
//		FD612_PotiveTNage(FD612_DIG2_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG3_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG4_ADDR,0xff, &FD612_base_DispBuff);
//		FD612_PotiveTNage(FD612_DIG5_ADDR,0xff, &FD612_base_DispBuff);
//		FD612_PotiveTNage(FD612_DIG6_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG7_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG8_ADDR,0xff, &FD612_base_DispBuff);
//		FD612_PotiveTNage(FD612_DIG9_ADDR,0xff, &FD612_base_DispBuff);
//		FD612_PotiveTNage(FD612_DIG10_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG11_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG12_ADDR,0xff, &FD612_base_DispBuff);		
//		FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
//		FD612_PotiveTNage(FD612_DIG1_ADDR,FD612_DISP_1, &FD612_top_DispBuff);
//		FD612_PotiveTNage(FD612_DIG2_ADDR,FD612_DISP_2, &FD612_top_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG3_ADDR,FD612_DISP_3, &FD612_top_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG4_ADDR,FD612_DISP_4, &FD612_top_DispBuff);
//		FD612_PotiveTNage(FD612_DIG5_ADDR,FD612_DISP_5, &FD612_top_DispBuff);
//		FD612_PotiveTNage(FD612_DIG6_ADDR,FD612_DISP_6, &FD612_top_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG7_ADDR,FD612_DISP_7, &FD612_top_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG8_ADDR,FD612_DISP_8, &FD612_top_DispBuff);
//		FD612_PotiveTNage(FD612_DIG9_ADDR,FD612_DISP_0, &FD612_top_DispBuff);
//		FD612_PotiveTNage(FD612_DIG10_ADDR,FD612_DISP_1, &FD612_top_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG11_ADDR,FD612_DISP_2, &FD612_top_DispBuff);	
		//FD612_PotiveTNage(FD612_DIG12_ADDR,FD612_DISP_3, &FD612_base_DispBuff);	

//	FD612_base_DispBuff.DispData[FD612_DIG1_ADDR]|=0Xff;
//	FD612_base_DispBuff.DispData[FD612_DIG6_ADDR]|=0X01;
//	FD612_base_DispBuff.DispData[FD612_DIG7_ADDR]|=0X01;
//	FD612_base_DispBuff.DispData[FD612_DIG8_ADDR]|=0X01;
//	FD612_base_DispBuff.DispData[FD612_DIG9_ADDR]|=0X01;
//	FD612_base_DispBuff.DispData[FD612_DIG10_ADDR]|=0X01;
//	FD612_base_DispBuff.DispData[FD612_DIG11_ADDR]|=0X01;
//	FD612_base_DispBuff.DispData[FD612_DIG12_ADDR]|=0X01;
//		FD612_base_DispBuff.DispData[FD612_DIG5_ADDR]|=0X01;
//		FD612_Base_Refresh(&FD612_top_DispBuff,&fd612_top_ops);

//		FD612_PotiveTNage(FD612_DIG1_ADDR,0xff, &FD612_base_DispBuff);
//		FD612_PotiveTNage(FD612_DIG2_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG3_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG4_ADDR,0xff, &FD612_base_DispBuff);
//		FD612_PotiveTNage(FD612_DIG5_ADDR,0xff, &FD612_base_DispBuff);
//		FD612_PotiveTNage(FD612_DIG6_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG7_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG8_ADDR,0xff, &FD612_base_DispBuff);
//		FD612_PotiveTNage(FD612_DIG9_ADDR,0xff, &FD612_base_DispBuff);
//		FD612_PotiveTNage(FD612_DIG10_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG11_ADDR,0xff, &FD612_base_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG12_ADDR,0xff, &FD612_base_DispBuff);		
//		FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
//		
//		
//		FD612_PotiveTNage(FD612_DIG1_ADDR,0xff, &FD612_top_DispBuff);
//		FD612_PotiveTNage(FD612_DIG2_ADDR,0xff, &FD612_top_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG3_ADDR,0xff, &FD612_top_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG4_ADDR,0xff, &FD612_top_DispBuff);
//		FD612_PotiveTNage(FD612_DIG5_ADDR,0xff, &FD612_top_DispBuff);
//		FD612_PotiveTNage(FD612_DIG6_ADDR,0xff, &FD612_top_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG7_ADDR,0xff, &FD612_top_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG8_ADDR,0xff, &FD612_top_DispBuff);
//		FD612_PotiveTNage(FD612_DIG9_ADDR,0xff, &FD612_top_DispBuff);
//		FD612_PotiveTNage(FD612_DIG10_ADDR,0xff, &FD612_top_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG11_ADDR,0xff, &FD612_top_DispBuff);	
//		FD612_PotiveTNage(FD612_DIG12_ADDR,0xff, &FD612_top_DispBuff);
//		FD612_top_DispBuff.DispData[FD612_DIG1_ADDR]|=0X01;
//		FD612_top_DispBuff.DispData[FD612_DIG2_ADDR]|=0X01;
//		FD612_top_DispBuff.DispData[FD612_DIG3_ADDR]|=0X01;
//		FD612_top_DispBuff.DispData[FD612_DIG4_ADDR]|=0X01;
//		FD612_top_DispBuff.DispData[FD612_DIG9_ADDR]|=0X10;
//		FD612_top_DispBuff.DispData[FD612_DIG10_ADDR]|=0X10;
//		FD612_top_DispBuff.DispData[FD612_DIG11_ADDR]|=0X10;
//		FD612_top_DispBuff.DispData[FD612_DIG12_ADDR]|=0X10;
////		FD612_top_DispBuff.DispData[FD612_DIG5_ADDR]=0Xff;


//		FD612_top_DispBuff.DispData[FD612_DIG5_ADDR]|=0X01;
//		FD612_top_DispBuff.DispData[FD612_DIG6_ADDR]|=0X01;
//		FD612_top_DispBuff.DispData[FD612_DIG7_ADDR]|=0X01;
//		FD612_top_DispBuff.DispData[FD612_DIG8_ADDR]|=0X01;
//		FD612_top_DispBuff.DispData[FD612_DIG9_ADDR]|=0X01;
//		FD612_top_DispBuff.DispData[FD612_DIG10_ADDR]|=0X01;
//		FD612_top_DispBuff.DispData[FD612_DIG11_ADDR]|=0X01;
//		FD612_top_DispBuff.DispData[FD612_DIG12_ADDR]|=0X01;
//		FD612_Base_Refresh(&FD612_top_DispBuff,&fd612_top_ops);
	FD612_PotiveTNage(FD612_DIG1_ADDR,FD612_DISP_1, &FD612_base_DispBuff);
	FD612_PotiveTNage(FD612_DIG2_ADDR,FD612_DISP_2, &FD612_base_DispBuff);	
	FD612_PotiveTNage(FD612_DIG3_ADDR,FD612_DISP_3, &FD612_base_DispBuff);	
	FD612_PotiveTNage(FD612_DIG4_ADDR,FD612_DISP_4, &FD612_base_DispBuff);
	FD612_PotiveTNage(FD612_DIG5_ADDR,FD612_DISP_5, &FD612_base_DispBuff);
	FD612_PotiveTNage(FD612_DIG6_ADDR,FD612_DISP_6, &FD612_base_DispBuff);	
	FD612_PotiveTNage(FD612_DIG7_ADDR,FD612_DISP_7, &FD612_base_DispBuff);	
	FD612_PotiveTNage(FD612_DIG8_ADDR,FD612_DISP_8, &FD612_base_DispBuff);
	FD612_PotiveTNage(FD612_DIG9_ADDR,FD612_DISP_9, &FD612_base_DispBuff);
	FD612_PotiveTNage(FD612_DIG10_ADDR,FD612_DISP_A, &FD612_base_DispBuff);	
	FD612_PotiveTNage(FD612_DIG11_ADDR,FD612_DISP_C, &FD612_base_DispBuff);	
//	FD612_PotiveTNage(FD612_DIG12_ADDR,FD612_DISP_4, &FD612_base_DispBuff);
	FD612_Base_Refresh(&FD612_base_DispBuff,&fd612_base_ops);
}
#endif