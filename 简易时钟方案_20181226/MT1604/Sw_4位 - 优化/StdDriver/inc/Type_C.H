
// #define     TYPE_C_DFP  
#define     TYPE_C_UFP

#define  DufaultPowerMin  13
#define  DufaultPowerMax  31
#define  Power1_5AMin  36
#define  Power1_5AMax  59
#define  Power3_0AMin  67
#define  Power3_0AMax  104 

#ifdef TYPE_C_DFP
#define DFP_DisableRpUCC1()     {USB_C_CTRL = ~(bUCC1_PU1_EN | bUCC1_PU0_EN);}
#define DFP_DefaultPowerUCC1()  {USB_C_CTRL = USB_C_CTRL | bUCC1_PU1_EN & ~bUCC1_PU0_EN;}
#define DFP_1_5APowerUCC1()     {USB_C_CTRL = USB_C_CTRL & ~bUCC1_PU1_EN | bUCC1_PU0_EN;}
#define DFP_3_0APowerUCC1()     {USB_C_CTRL |= bUCC1_PU1_EN | bUCC1_PU0_EN;}

#define DFP_DisableRpUCC2()     {USB_C_CTRL = ~(bUCC2_PU1_EN | bUCC2_PU0_EN);}
#define DFP_DefaultPowerUCC2()  {USB_C_CTRL = USB_C_CTRL | bUCC2_PU1_EN & ~bUCC2_PU0_EN;}
#define DFP_1_5APowerUCC2()     {USB_C_CTRL = USB_C_CTRL & ~bUCC2_PU1_EN | bUCC2_PU0_EN;}
#define DFP_3_0APowerUCC2()     {USB_C_CTRL |= bUCC2_PU1_EN | bUCC2_PU0_EN;}

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
UINT8 TypeC_DFP_Init(UINT8 Power);
#endif

#ifdef TYPE_C_UFP
//FD814 UCC1&2 Rd 开启(SS=1)/禁止(SS=0)
#define UPF_DisableRd(SS)  (USB_C_CTRL = SS ? (USB_C_CTRL|bUCC1_PD_EN|bUCC2_PD_EN) : 0)

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
UINT8 TypeC_UPF_PDCheck();
#endif
