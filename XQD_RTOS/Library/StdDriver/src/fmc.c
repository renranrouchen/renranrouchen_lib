/**************************************************************************//**
 * @file     fmc.c
 * @version  V3.00
 * $Revision: 5 $
 * $Date: 14/08/29 11:01a $
 * @brief    M451 series FMC driver source file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

//* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "M451Series.h"
#include "DMX512.h"
#include "config.h"
/** @addtogroup M451_Device_Driver M451 Device Driver
  @{
*/

/** @addtogroup M451_FMC_Driver FMC Driver
  @{
*/


/** @addtogroup M451_FMC_EXPORTED_FUNCTIONS FMC Exported Functions
  @{
*/


/**
  * @brief      Set boot source from LDROM or APROM after next software reset
  *
  * @param[in]  i32BootSrc
  *                         1: Boot from LDROM,
  *                         0: Boot from APROM
  *
  * @return   None
  *
  * @details  This function is used to switch APROM boot or LDROM boot. User need to call
  *           FMC_SetBootSource to select boot source first, then use CPU reset or
  *           System Reset Request to reset system.
  *
  */
void FMC_SetBootSource(int32_t i32BootSrc)
{
    if(i32BootSrc)
        FMC->ISPCTL |= FMC_ISPCTL_BS_Msk; /* Boot from LDROM */
    else
        FMC->ISPCTL &= ~FMC_ISPCTL_BS_Msk;/* Boot from APROM */
}


/**
  * @brief    Disable ISP Functions
  *
  * @param    None
  *
  * @return   None
  *
  * @details  This function will clear ISPEN bit of ISPCTL to disable ISP function
  *
  */
void FMC_Close(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_ISPEN_Msk;
}


/**
  * @brief    Disable APROM update function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  Disable APROM update function will forbid APROM programming when boot form APROM.
  *           APROM update is default to be disable.
  *
  */
void FMC_DisableAPUpdate(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_APUEN_Msk;
}


/**
  * @brief    Disable User Configuration update function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  Disable User Configuration update function will forbid User Configuration programming.
  *           User Configuration update is default to be disable.
  */
void FMC_DisableConfigUpdate(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_CFGUEN_Msk;
}


/**
  * @brief    Disable LDROM update function
  *
  * @param    None
  *
  * @return   None

  * @details  Disable LDROM update function will forbid LDROM programming.
  *           LDROM update is default to be disable.
  */
void FMC_DisableLDUpdate(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_LDUEN_Msk;
}


/**
  * @brief    Enable APROM update function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  Enable APROM to be able to program when boot from APROM.
  *
  */
void FMC_EnableAPUpdate(void)
{
    FMC->ISPCTL |= FMC_ISPCTL_APUEN_Msk;
}


/**
  * @brief    Enable User Configuration update function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  Enable User Configuration to be able to program.
  *
  */
void FMC_EnableConfigUpdate(void)
{
    FMC->ISPCTL |= FMC_ISPCTL_CFGUEN_Msk;
}


/**
  * @brief    Enable LDROM update function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  Enable LDROM to be able to program.
  *
  */
void FMC_EnableLDUpdate(void)
{
    FMC->ISPCTL |= FMC_ISPCTL_LDUEN_Msk;
}


/**
  * @brief    Get the current boot source
  *
  * @param    None
  *
  * @retval   0 This chip is currently booting from APROM
  * @retval   1 This chip is currently booting from LDROM
  *
  * @note     This function only show the boot source.
  *           User need to read ISPSTA register to know if IAP mode supported or not in relative boot.
  */
int32_t FMC_GetBootSource(void)
{
    if(FMC->ISPCTL & FMC_ISPCTL_BS_Msk)
        return 1;
    else
        return 0;
}


/**
  * @brief    Enable FMC ISP function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  ISPEN bit of ISPCTL must be set before we can use ISP commands.
  *           Therefore, To use all FMC function APIs, user needs to call FMC_Open() first to enable ISP functions.
  *
  * @note     ISP functions are write-protected. user also needs to unlock it by calling SYS_UnlockReg() before using all ISP functions.
  *
  */
void FMC_Open(void)
{
    FMC->ISPCTL |=  FMC_ISPCTL_ISPEN_Msk;
}

/**
  * @brief    Get the base address of Data Flash if enabled.
  *
  * @param    None
  *
  * @return   The base address of Data Flash
  *
  * @details  This function is used to return the base address of Data Flash.
  *
  */
uint32_t FMC_ReadDataFlashBaseAddr(void)
{
    return FMC->DFBA;
	
}


/**
  * @brief       Read the User Configuration words.
  *
  * @param[out]  u32Config  The word buffer to store the User Configuration data.
  * @param[in]   u32Count   The word count to be read.
  *
  * @retval       0 Success
  * @retval      -1 Failed
  *
  * @details     This function is used to read the settings of user configuration.
  *              if u32Count = 1, Only CONFIG0 will be returned to the buffer specified by u32Config.
  *              if u32Count = 2, Both CONFIG0 and CONFIG1 will be returned.
  */
int32_t FMC_ReadConfig(uint32_t *u32Config, uint32_t u32Count)
{
    int32_t i;

    for(i = 0; i < u32Count; i++)
        u32Config[i] = FMC_Read(FMC_CONFIG_BASE + i * 4);

    return 0;
}


/**
  * @brief      Write User Configuration
  *
  * @param[in]  u32Config The word buffer to store the User Configuration data.
  * @param[in]  u32Count The word count to program to User Configuration.
  *
  * @retval     0 Success
  * @retval    -1 Failed
  *
  * @details    User must enable User Configuration update before writing it.
  *             User must erase User Configuration before writing it.
  *             User Configuration is also be page erase. User needs to backup necessary data
  *             before erase User Configuration.
  */
int32_t FMC_WriteConfig(uint32_t *u32Config, uint32_t u32Count)
{
    int32_t i;

    for(i = 0; i < u32Count; i++)
    {
        FMC_Write(FMC_CONFIG_BASE + i * 4, u32Config[i]);
        if(FMC_Read(FMC_CONFIG_BASE + i * 4) != u32Config[i])
            return -1;
    }

    return 0;
}

/**
 * @brief      Enable Flash Access Frequency  Optimization Mode
 *
 * @param[in]  u32Mode   Optimize flash access cycle mode
 *             - \ref FMC_FTCTL_OPTIMIZE_DISABLE
 *             - \ref FMC_FTCTL_OPTIMIZE_12MHZ
 *             - \ref FMC_FTCTL_OPTIMIZE_36MHZ
 *             - \ref FMC_FTCTL_OPTIMIZE_60MHZ
 *             - \ref FMC_FTCTL_OPTIMIZE_72MHZ
 *
 * @return     None
 *
 * @details    This function will set FOM bit fields of FTCTL register to set flash access frequency optimization mode.
 *
 * @note       The flash optimization mode (FOM) bits are write protect.
 *
 */
void FMC_EnableFreqOptimizeMode(uint32_t u32Mode)
{
    FMC->FTCTL &= ~FMC_FTCTL_FOM_Msk;
    FMC->FTCTL |= (u32Mode << FMC_FTCTL_FOM_Pos);
}

/**
 * @brief      Disable Flash Access Frequency  Optimization Mode
 *
 * @param      None
 *
 * @return     None
 *
 * @details    This function will clear FOM bit fields of FTCTL register to disable flash access frequency optimization mode.
 *
 * @note       The flash optimization mode (FOM) bits are write protect.
 *
 */
void FMC_DisableFreqOptimizeMode(void)
{
    FMC->FTCTL &= ~FMC_FTCTL_FOM_Msk;
}

/*@}*/ /* end of group M451_FMC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M451_FMC_Driver */

/*@}*/ /* end of group M451_Device_Driver */

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/

extern DMX512_arg_t SM1751_arg ;
extern chip_param_t chip_param_sm17512;

extern chip_param_t chip_param_sm165XX;
extern SM165XX_arg_t SM165XX_arg_set;

extern DMX512_arg_t SM17500_arg;
extern chip_param_t chip_param_sm17500;

extern RZ_arg_t chip_param_sm16703P;
extern RZ_arg_t chip_param_sm16804P;

extern Protocol_output_parameter_t Equipment_set_param;
//extern chip_param_t chip_param_sm17501;
#if 0
/*起始地址为0x1f000~0x1fff,为4KB*/
/*页擦除为2K*/
void write_usr_data(void)
{
	uchar i =0;
	uint32_t *arg =(uint32_t *)&SM1751_arg;
	SYS_UnlockReg();
	FMC_Open();
  FMC_ENABLE_AP_UPDATE();
	/*擦除4KB数据FLASH*/
	for(i=0;i<4096/2048;i++)
	{
		FMC_Erase(0x0001F000+i*2048);
	}
	for(i=0;i<(sizeof(DMX512_arg_t));i=i+4)
	{
		FMC_Write(0x0001F000+i,*arg++);
	}
	// FMC_Write(0x0001F000,0x12345678);
	//FMC_Write(0x0001F000+4,0xff564888);
	printf("write_ok\r\n");
	 SYS_LockReg();
	FMC->ISPCTL &=  ~FMC_ISPCTL_ISPEN_Msk;

}

DMX512_arg_t data_arg;
void read_usr_data(void)
{
	uchar i=0;
//	uint32_t u32Data,u32Data2;
	uint32_t *arg =(uint32_t *)&data_arg;
	SYS_UnlockReg();
	FMC_Open();
  FMC_ENABLE_AP_UPDATE();
//	u32Data = FMC_Read(0x0001F000);
//	u32Data2 = FMC_Read(0x0001F000+4);
	for(i=0;i<(sizeof(DMX512_arg_t));i=i+4)
	{
		(*arg++)=FMC_Read(0x0001F000+i);
	}
	printf("u32Data____%d__%d__%d\r\n",data_arg.RGBW_R,data_arg.AUTO,data_arg.RGBW_IB);
//	printf("u32Data____%x__%x\r\n",u32Data,u32Data2);
	FMC->ISPCTL &=  ~FMC_ISPCTL_ISPEN_Msk;
	SYS_LockReg();

}
#endif
void data_init(void)
{
SM165XX_arg_set.Power_Up_Color=1;


	SM1751_arg.AUTO =0;
	SM1751_arg.REDIS = 0;
	SM1751_arg.CH1_CH0 =2;
	SM1751_arg.RGBW_R =255;
	SM1751_arg.RGBW_G =255;
	SM1751_arg.RGBW_B =255;
	SM1751_arg.RGBW_W =0;
	SM1751_arg.RGBW_IG = 1;
	SM1751_arg.RGBW_IR = 1;
	SM1751_arg.RGBW_IW = 1;
	SM1751_arg.RGBW_IB = 1;
  


	SM17500_arg.RGBW_R =0;
	SM17500_arg.RGBW_G =0;
	SM17500_arg.RGBW_B =255;
	SM17500_arg.RGBW_W =0;
	SM17500_arg.IC_select = 0;
	SM17500_arg.RTZ_select = 1;
	SM17500_arg.REDIS = 1;
	SM17500_arg.CH1_CH0 = 0;
	SM17500_arg.EN0_EN1 =1;
	SM17500_arg.RGBW_IG = 0;
	SM17500_arg.RGBW_IR = 0;
	SM17500_arg.RGBW_IW = 0;
	SM17500_arg.RGBW_IB = 0;	
	SM17500_arg.SELF_CHAN_LD = 3;



/*struct SM17512 init*/

	chip_param_sm17512.Run_manually_point=3;
	chip_param_sm17512.lamps_num = 0X01;
	chip_param_sm17512.Channel_num = 0X01;
	chip_param_sm17512.Channel_form = RGB;
	chip_param_sm17512.Rate_num = 0x00;
	chip_param_sm17512.coding_form_num = RGB;
	chip_param_sm17512.chip_chanle_num =512;
	chip_param_sm17512.coding_data_change_flag = lamps;
	chip_param_sm17512.red_data = 0;
	chip_param_sm17512.green_data =0;
	chip_param_sm17512.blue_data =0;


   chip_param_sm165XX.Run_manually_point=3;
	chip_param_sm165XX.lamps_num = 0X01;
	chip_param_sm165XX.coding_form_num = RGB;
	chip_param_sm165XX.chip_chanle_num =512;
	chip_param_sm165XX.red_data = 0;
	chip_param_sm165XX.green_data =0;
	chip_param_sm165XX.blue_data =0;


	chip_param_sm17500.Run_manually_point=1;
	chip_param_sm17500.Self_Channel_num =0X01;
	chip_param_sm17500.lamps_num = 0X01;
	chip_param_sm17500.Channel_num = 0X01;
	chip_param_sm17500.Channel_form = RGB;
	chip_param_sm17500.Rate_num = 0x00;
	chip_param_sm17500.coding_form_num = RGB;
	chip_param_sm17500.chip_chanle_num =512;
	//.coding_data_change_flag = lamps,
	chip_param_sm17500.red_data = 0;
	chip_param_sm17500.green_data =0;
	chip_param_sm17500.blue_data =0;




	chip_param_sm16804P.RGBW_R=255;
	chip_param_sm16804P.RGBW_G=255;
	chip_param_sm16804P.RGBW_B=0;
	chip_param_sm16804P.RGBW_W=0;
	chip_param_sm16804P.RGBW_IR=0;//0~15
	chip_param_sm16804P.RGBW_IG=0;
	chip_param_sm16804P.RGBW_IB=0;
	chip_param_sm16804P.RGBW_IW=0;
	chip_param_sm16804P.Chip_Num=6;



	chip_param_sm16703P.RGBW_R=255;
	chip_param_sm16703P.RGBW_G=255;
	chip_param_sm16703P.RGBW_B=0;
	chip_param_sm16703P.Chip_Num=6;
	
	Equipment_set_param.Color_num = loop_lamp;
	Equipment_set_param.On_load_channe_param=512;
	Equipment_set_param.Output_frequency_param=Freq_baud_250k;
	Equipment_set_param.Play_speed_param=1;
	Equipment_set_param.colour_num_param=3;
	Equipment_set_param.Protocol_type_param =DMX512;
	Equipment_set_param.RZ_chip_param =RZ_SM16804;
	Equipment_set_param.Red_data=255;
	Equipment_set_param.Green_data=255;
	Equipment_set_param.Blue_data=255;
	Equipment_set_param.Write_data=255;
	Equipment_set_param.Yellow_data=255;

}

#define addr_start 0x0001f000
void write_all_data(void)
{
	uint32_t i=0;
	uint32_t addr=0;
	uint32_t *arg =NULL;
	uint32_t addr_for=0;
	
	SYS_UnlockReg();
	FMC_Open();
	//页擦除
	for(i=0;i<4096/2048;i++)
  {
	 FMC_Erase(addr_start+i*2048);
	}
	//写入标志位
	FMC_Write(addr_start,0x505050);
	addr_for=4;
	
	/*struct Equipment_set_param*/
	arg = (uint32_t *)&Equipment_set_param;
	addr = addr_for;
	addr_for=addr_for+sizeof(Equipment_set_param);
	//4字节对齐
	for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
	for(i=addr;i<(addr_for);i=i+4)
	{
		FMC_Write(addr_start+i,*arg++);
	}	
	
	/*struct SM1751_arg*/
	arg = (uint32_t *)&SM1751_arg;
	addr = addr_for;
	addr_for=addr_for+sizeof(DMX512_arg_t);
	//4字节对齐
	for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
	for(i=addr;i<(addr_for);i=i+4)
	{
		FMC_Write(addr_start+i,*arg++);
	}

	/*struct chip_param_sm17512;*/
	arg = (uint32_t *)&chip_param_sm17512;
	addr = addr_for;
	addr_for=addr_for+sizeof(chip_param_t);
		for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
	for(i=addr;i<(addr_for);i=i+4)
	{
		FMC_Write(addr_start+i,*arg++);
	}

	/*struct SM165XX_arg_set*/
	
	arg = (uint32_t *)&SM165XX_arg_set;
	addr = addr_for;
	addr_for=addr_for+sizeof(SM165XX_arg_t);
		for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}	
	for(i=addr;i<(addr_for);i=i+4)
	{
		FMC_Write(addr_start+i,*arg++);
	}

	/*struct chip_param_sm165XX*/
	arg = (uint32_t *)&chip_param_sm165XX;
	addr = addr_for;
	addr_for=addr_for+sizeof(chip_param_t);
	for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}	
	for(i=addr;i<(addr_for);i=i+4)
	{
		FMC_Write(addr_start+i,*arg++);
	}	
//	
	/*struct SM17500_arg*/
	arg = (uint32_t *)&SM17500_arg;
	addr = addr_for;
	addr_for=addr_for+sizeof(DMX512_arg_t);
		for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
	for(i=addr;i<(addr_for);i=i+4)
	{
		FMC_Write(addr_start+i,*arg++);
	}	
	
	/*struct chip_param_sm17500*/
	arg = (uint32_t *)&chip_param_sm17500;
	addr = addr_for;
	addr_for=addr_for+sizeof(chip_param_t);
		for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
	for(i=addr;i<(addr_for);i=i+4)
	{
		FMC_Write(addr_start+i,*arg++);
	}	
	
	/*struct chip_param_sm16703P*/
	arg = (uint32_t *)&chip_param_sm16703P;
	
	addr = addr_for;
	addr_for=addr_for+sizeof(RZ_arg_t);
		for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
	for(i=addr;i<(addr_for);i=i+4)
	{
		FMC_Write(addr_start+i,*arg++);
	}	

	/*struct chip_param_sm16804P*/
	arg = (uint32_t *)&chip_param_sm16804P;
//	printf("arg_addr%x\r\n",arg);
	addr = addr_for;
	addr_for=addr_for+sizeof(chip_param_sm16804P);
	for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
//	FMC_Write(addr_start+addr,*arg);
//	arg++;
//	FMC_Write(addr_start+addr+4,*arg);
//	arg++;
//	FMC_Write(addr_start+addr+8,*arg);		
	for(i=addr;i<(addr_for);i=i+4)
	{

		FMC_Write(addr_start+i,*arg);
		arg++;
	}	

	FMC_Close();
	SYS_LockReg();
	printf("write_ok\r\n");
}
void delay_us(unsigned short nTimer);
void read_all_data(void)
{
	unsigned int i=0;
	unsigned int addr=0;
	uint32_t *arg =NULL;	
  uint32_t addr_for=0;

  SYS_UnlockReg();
	FMC_Open();	
	addr_for=4;
	
	/*struct Equipment_set_param*/
	arg = (uint32_t *)&Equipment_set_param;
	addr = addr_for;
	addr_for=addr_for+sizeof(Equipment_set_param);
	//4字节对齐
	for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
	for(i=addr;i<(addr_for);i=i+4)
	{
		(*arg++)=FMC_Read(addr_start+i);
	}		
	
	
	/*struct SM1751_arg*/
	arg = (uint32_t *)&SM1751_arg;
	addr = addr_for;
	addr_for=addr+sizeof(DMX512_arg_t);
	for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
	for(i=addr;i<(addr_for);i=i+4)
	{
		(*arg++)=FMC_Read(addr_start+i);
	}

	/*struct chip_param_sm17512;*/
	arg = (uint32_t *)&chip_param_sm17512;
	addr = addr_for;
	addr_for=addr_for+sizeof(chip_param_t);
	for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}	
	for(i=addr;i<(addr_for);i=i+4)
	{
		(*arg++)=FMC_Read(addr_start+i);
	}

	/*struct SM165XX_arg_set*/
	arg = (uint32_t *)&SM165XX_arg_set;
	addr = addr_for;
	addr_for=addr_for+sizeof(SM165XX_arg_t);
		for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}

	for(i=addr;i<(addr_for);i=i+4)
	{
		(*arg++)=FMC_Read(addr_start+i);
	}
	/*struct chip_param_sm165XX*/
	arg = (uint32_t *)&chip_param_sm165XX;
	addr = addr_for;
	addr_for=addr_for+sizeof(chip_param_t);
	for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}	
	for(i=addr;i<(addr_for);i=i+4)
	{
		(*arg++)=FMC_Read(addr_start+i);
	}	
//	
	/*struct SM17500_arg*/
	arg = (uint32_t *)&SM17500_arg;
	addr = addr_for;
	addr_for=addr_for+sizeof(DMX512_arg_t);
		for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
	for(i=addr;i<(addr_for);i=i+4)
	{
		(*arg++)=FMC_Read(addr_start+i);
	}	
//	
	/*struct chip_param_sm17500*/
	arg = (uint32_t *)&chip_param_sm17500;
	addr = addr_for;
	addr_for=addr_for+sizeof(chip_param_t);
		for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
	for(i=addr;i<(addr_for);i=i+4)
	{
		(*arg++)=FMC_Read(addr_start+i);
	}	
//	
	/*struct chip_param_sm16703P*/
	arg = (uint32_t *)&chip_param_sm16703P;
	addr = addr_for;
	addr_for=addr_for+sizeof(RZ_arg_t);
		for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
	for(i=addr;i<(addr_for);i=i+4)
	{
		(*arg++)=FMC_Read(addr_start+i);
	}	
//	
	/*struct chip_param_sm16804P*/
	arg = (uint32_t *)&chip_param_sm16804P;
	addr = addr_for;
	addr_for=addr_for+sizeof(RZ_arg_t);
	for(i=0;i<4;i++)
	{
		if(addr_for%4 !=0)addr_for++;
		else break;
	}
//	for(i=addr;i<(addr_for);i=i+4)
//	{
//		*arg=FMC_Read(addr_start+i);
//		arg++;
//		
//	}		
//用FOR循环会使硬件跑飞
	*arg=FMC_Read(addr_start+addr);
	arg++;
	*arg=FMC_Read(addr_start+addr+4);
	arg++;
	*arg=FMC_Read(addr_start+addr+8);
	
//	for(i=addr;i<(addr_for+4);i=i+4)
//	{
//		printf("arg\r\n");
//		//(*arg++)=0x01;

//	}	

		/*struct chip_param_sm17501*/
//	arg = (uint32_t *)&chip_param_sm17501;
//	addr = addr_for;
//	addr_for=addr_for+sizeof(chip_param_t);
//		for(i=0;i<4;i++)
//	{
//		if(addr_for%4 !=0)addr_for++;
//		else break;
//	}
//	for(i=addr;i<(addr_for);i=i+4)
//	{
//		(*arg++)=FMC_Read(addr_start+i);
//	}	
	FMC_Close();
	SYS_LockReg();
	printf("read_ok\r\n");
}

void Data_init(void)
{
	//uchar i =0;
	SYS_UnlockReg();
	FMC_Open();
  FMC_ENABLE_AP_UPDATE();
	if(0x505050==FMC_Read(addr_start))
	{
		read_all_data();
//		SYS_UnlockReg();
//		FMC_Open();		
//		for(i=0;i<4096/2048;i++)
//		{
//			FMC_Erase(addr_start+i*2048);
//		}
//		FMC_Close();		
//		SYS_LockReg();		
		printf("read_fuc\r\n");
	}
	else
	{
		data_init();
		flash_data_addr_base( );
		write_all_data();	
		printf("write_fuc\r\n");
	}
	FMC_Close();
	SYS_LockReg();
}
