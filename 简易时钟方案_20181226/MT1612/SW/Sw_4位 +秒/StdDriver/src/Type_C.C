
/********************************** (C) COPYRIGHT *******************************
* File Name          : Type_C.C
* Author             : FDHISI
* Version            : V1.0
* Date               : 2017/01/20
* Description        : FD814 Type-C使用
                       主模式 设备正反插检测、充电电流通知从设备
                       从模式 检测主机供电能力	 
*******************************************************************************/

#include "..\Public\FD814.H"                                                         
#include "..\Public\Debug.H"
#include "Type_C.H"
#include "stdio.h"

#pragma  NOAREGS
/********************************************************************************
DFP (Downstream Facing Port) Host端
UFP (Upstream Facing Port)   Dev端

在DFP的CC pin会有上拉电阻Rp,在UFP会有下拉电阻Rd
在DFP和UFP未连接前,DFP的VBUS是没有输出的

CC PIN是用来侦测正反插，从DPF来看，当CC1接到下拉就是正插;如果CC2接到下拉就是反插；
	 侦测完正反插之后，就会输出相应的USB信号
	 
DPF在不同的模式下，定义了在CC PIN要供多大的电流或是要用多大的Rp值
********************************************************************************/
#ifdef TYPE_C_DFP

/*******************************************************************************
* Function Name  : TypeC_DFP_Init(UINT8 Power)
* Description    : Type-C UPF检测初始化
* Input          : UINT8 Power
                   0 禁止UCC1&2上拉
                   1 默认电流
                   2 1.5A
                   3 3.0A									 
* Output         : None
* Return         : 1   UCC1
                   2   UCC2
*******************************************************************************/
UINT8 TypeC_DFP_Init(UINT8 Power)
{
   P1_MOD_OC &= ~(bUCC2|bUCC1);                                                   
   P1_DIR_PU &= ~(bUCC2|bUCC1);                                                   //UCC1 UCC2 设置浮空输入
   if(Power == 0){
     DFP_DisableRpUCC1();                                                         //UCC1禁止
     DFP_DisableRpUCC2();	                                                        //UCC2禁止	
   }
	 else if(Power == 1){
     DFP_DefaultPowerUCC1();                                                      //输出能力默认
     DFP_DefaultPowerUCC2();	                                                    	 
   }
   else if(Power == 2){
     DFP_1_5APowerUCC1();                                                         //输出能力1.5A
     DFP_1_5APowerUCC2();		 
   }
   else if(Power == 3){
     DFP_3_0APowerUCC1();                                                         //输出能力3.0A
     DFP_3_0APowerUCC2();		 
   }
   ADC_CFG |= bCMP_EN;                                                            //电平比较电源使能
   ADC_CHAN1 =1;ADC_CHAN0=0;CMP_CHAN =0;                                          //UCC1 反相输入，UCC2正相输入
                                                                                  //查询CMPO 如果CMPO为1，正插；如果CMPO为0反插
   if(CMPO) return 1;
   else return 2;
}
#endif

/********************************************************************************
UPF,需要检测CC管脚的电压值来获取DFP的电流输出能力
--------  CC电压Min   CC电压Max
默认电流   0.25V         0.61V
1.5A       0.70V         1.16V
3.0A       1.31V         2.04V
********************************************************************************/
#ifdef TYPE_C_UFP

/*******************************************************************************
* Function Name  : TypeC_UPF_PDCheck()
* Description    : Type-C UPF检测DPF供电能力
* Input          : None
* Output         : None
* Return         : UINT8 RE  
                   0  defaultPower
                   1  1.5A
                   2  3.0A	
                   0xff 悬空									 
*******************************************************************************/
UINT8 TypeC_UPF_PDCheck()
{
    UPF_DisableRd(1);                                                          //开启UCC下拉电阻
    ADC_CFG |= bADC_EN;                                                        //ADC电源使能 
    P1_DIR_PU &= ~bAIN1;ADC_CHAN1 =0;ADC_CHAN0=1;  
    mDelayuS(2);                                                               //等待上拉完全关闭和ADC电源稳定
    ADC_START = 1;                                                             //开始采样，采样完成进入中断	
    while(ADC_START);                                                          //ADC_START变为0时，表示采样完成
    if((ADC_DATA >= Power3_0AMin)&&(ADC_DATA <= Power3_0AMax)){
      return 2;                                                                //3.0A供电能力			
    }
    else if((ADC_DATA >= Power1_5AMin)&&(ADC_DATA <= Power1_5AMax)){
      return 1;                                                                //1.5A供电能力			
    }		
    else if((ADC_DATA >= DufaultPowerMin)&&(ADC_DATA <= DufaultPowerMax)){
      return 0;                                                                //默认供电能力			
    }		
		
    P1_DIR_PU &= ~bAIN2;ADC_CHAN1 =1;ADC_CHAN0=0;
    mDelayuS(2);                                                               //等待上拉完全关闭	
    ADC_START = 1;                                                             //开始采样，采样完成进入中断	
    while(ADC_START);                                                          //ADC_START变为0时，表示采样完成
    if((ADC_DATA >= Power3_0AMin)&&(ADC_DATA <= Power3_0AMax)){
      return 2;                                                                //3.0A供电能力			
    }
    else if((ADC_DATA >= Power1_5AMin)&&(ADC_DATA <= Power1_5AMax)){
      return 1;                                                                //1.5A供电能力			
    }		
    else if((ADC_DATA >= DufaultPowerMin)&&(ADC_DATA <= DufaultPowerMax)){
      return 0;                                                                //默认供电能力			
    }	
    return 0xFF;		
}
#endif
