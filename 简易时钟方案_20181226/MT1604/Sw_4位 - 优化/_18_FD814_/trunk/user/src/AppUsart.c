#include "AppUsart.h"
#include "stdlib.h"
#include "mydelay.h"

sbit   FD650_INT = P3^4;

u8 ReceiveCnt = 0x00;	//接收数据缓存计数:类似于写指针
u8 ReadCnt = 0x00;		//单片机数据读:类似于读指针
u8 ReceiveStop = 0x00;	//0: 没有接收完 , 1:接收完
u8 ReceiveStart = 0x00;	//0:停止接收处理,1:开始接收处理
u8 ReceiveDataBufeOld = 0x00;//上一个读到的寄存器
u16 RegCnt = 0x00;		//防止大数据的时候,数据溢出,所以定义为16位
u8 RegType = 0x00; 		//寄存器类型: 0:普通寄存器 , 1:PageA, 2:PageB
u8 RegAddr = 0x00;		//寄存器地址
u8 WriteNum = 0x00;		//写入数据的个数
u8 WriteDataCnt = 0x00;	//获取到的数据个数个数计数
u8 FD_ChipCmd = 0x00;	//上位机译码后的命令
u8 FD_ChipMode = 0x00;	//模式选择:默认不进行其他操作
u8 FD_PrintCmdEna = 0x00;	//是否打印上位机发送的命令,默认不打印
u16 I2cDelayCnt = 0x00;	//i2c延时需要用到的
u8 FD_PrintfSysTimeEna = 0x00;	//是否打印时间寄存器
u8 FD_I2cContinuousEna = 0x00;	//i2c是否连续默认不连续
u8 FD_WaveViewEna = 0x00;		//波形显示使能
u8 FD_I2cAddrType = I2C_ADDER_TYPE0;	//寄存器地址默认是:I2C_ADDER_TYPE0

u8 ChipType = CMD_FDXXX_CHIP_TYPE_FD650;
u8 FD6XX_STATE_ADDR = 0x48;
u8 FD6XX_KEY_ADDR = 0x4F;
u8 FD6XX_DIG0_ADDR = 0x68;			//<DIG0地址
u8 FD6XX_DIG1_ADDR = 0x6a;			//<DIG1地址
u8 FD6XX_DIG2_ADDR = 0x6c;			//<DIG2地址
u8 FD6XX_DIG3_ADDR = 0x6e;			//<DIG3地址
u8 FD6XX_DIG4_ADDR = 0x66;			//<DIG4地址
u8 FD6XX_DIG5_ADDR = 0x70;			//无效
u8 FD6XX_DIG6_ADDR = 0x72;			//无效
u8 FD6XX_DIG7_ADDR = 0x74;			//无效
u8 FD5XX_CNTR1_ADDR = 0x09;
u8 FD5XX_Cntr1RegBuf = 0x78;		//Cntr1上电默认状态

typedef struct 
{
	u8 Cntr;
	u8 Key;
	u8 Dis[DIS_LENGTH];
} FD6XX_Reg; 

FD6XX_Reg FD6XX_RegBuf;

ReceiveData ReceiveDataBufe[RECEIVE_LENGTH];
u8 WriteDataBufe[WRITE_DATA_LENGTH];

//发送数据给上位机
void AppUsart_UsartSendData(u8 Type, u8 Addr, u8 * Str, u8 length)
{
	u8 i;
	putchar(0x55);
	putchar(0xaa);
	putchar(Type);
	putchar(Addr);
	putchar(length);
	for(i = 0; i < length; i++)
	{
		putchar(Str[i]);
	}
	putchar(0xaa);
	putchar(0x55);	
}

//打印系统时间
void AppUsart_PrintfSysTime()
{
	AppUsart_UsartSendData(TYPE_FDXXX_FUN, CMD_FDXXX_FUN_CMD_PRINTF_TIME_EN, "", 0);
}

//按键读取
void AppUsart_ReadKey(u8 mode)	// 0: 打印变化 , 1:直接打印按键值
{
	u8 KeyNum = 0;
	KeyNum = I2C_RdAddrData(FD6XX_KEY_ADDR);
	if((KeyNum != FD6XX_RegBuf.Key) | mode)	//有按键按下或者按键值不一样
	{
		if(FD_PrintfSysTimeEna)
		{
			AppUsart_PrintfSysTime();
		}
		FD650_INT = 1;
		printf("KEY = %02X\n", (u16)KeyNum);
		FD650_INT = 0;
		FD6XX_RegBuf.Key = KeyNum;	//更新寄存器的值 //记录上一个按键值
		AppUsart_UsartSendData(TYPE_FDXX_KEY, CMD_KEY_REG | 0x80,&FD6XX_RegBuf.Key, 1);
	}
}


//打印信息
void AppUsart_PrintfInfo()
{
	printf("当前芯片信息:\n");
	AppUsart_PrintfSysTime();
	switch(ChipType)
	{
		case CMD_FDXXX_CHIP_TYPE_FD650 : printf("芯片模式: FD650\n"); break;
		case CMD_FDXXX_CHIP_TYPE_FD651 : printf("芯片模式: FD651\n"); break;
		case CMD_FDXXX_CHIP_TYPE_FD638 : printf("芯片模式: FD638\n"); break;
		case CMD_FDXXX_CHIP_TYPE_FD632 : printf("芯片模式: FD632\n"); break;
		case CMD_FDXXX_CHIP_TYPE_FD5012 : printf("芯片模式: FD5012\n"); break;
		default : printf("芯片模式: FD650\n"); break;
	}
	switch(FD_ChipMode)
	{
		case 0 : printf("工作模式: 显示模式0\n"); break;
		case 1 : printf("工作模式: 显示模式1\n"); break;
		case 2 : printf("工作模式: 按键模式0\n"); break;
		case 3 : printf("工作模式: 按键模式1\n"); break;
		case 4 : printf("工作模式: 其他模式\n"); break;
		case 5 : printf("工作模式: 显示时间\n"); break;
		case 6 : printf("工作模式: 文件命令循环\n"); break;
		default : printf("工作模式: 显示模式0\n"); break;
	}
	printf("CNTR = %02X\n", (u16)FD6XX_RegBuf.Cntr);	//打印寄存器值
	AppUsart_ReadKey(1);
	printf("DIS0 = %02X; DIS1 = %02X; DIS2 = %02X; DIS3 = %02X; \nDIS4 = %02X; DIS5 = %02X; DIS6 = %02X; DIS7 = %02X; \n", \
	(u16)FD6XX_RegBuf.Dis[0], (u16)FD6XX_RegBuf.Dis[1], (u16)FD6XX_RegBuf.Dis[2], (u16)FD6XX_RegBuf.Dis[3], \
	(u16)FD6XX_RegBuf.Dis[4], (u16)FD6XX_RegBuf.Dis[5], (u16)FD6XX_RegBuf.Dis[6], (u16)FD6XX_RegBuf.Dis[7] );	//打印显示值 

	printf("DIS0~DIS7 = %02X %02X %02X %02X %02X %02X %02X %02X \n", \
	(u16)FD6XX_RegBuf.Dis[0], (u16)FD6XX_RegBuf.Dis[1], (u16)FD6XX_RegBuf.Dis[2], (u16)FD6XX_RegBuf.Dis[3], \
	(u16)FD6XX_RegBuf.Dis[4], (u16)FD6XX_RegBuf.Dis[5], (u16)FD6XX_RegBuf.Dis[6], (u16)FD6XX_RegBuf.Dis[7] );	//打印显示值 

	printf("\n");
}

void AppUsart_Init()
{
	u16 i;
	ReceiveCnt = 0x00;	//接收数据缓存计数:类似于写指针
	ReadCnt = 0x00;		//单片机数据读:类似于读指针
	ReceiveStop = 0x00;	//0: 没有接收完 , 1:接收完
	ReceiveStart = 0x00;	//0:停止接收处理,1:开始接收处理
	ReceiveDataBufeOld = 0x00;//上一个读到的寄存器
	RegCnt = 0x00;		//防止大数据的时候,数据溢出,所以定义为16位
	RegType = 0x00; 		//寄存器类型: 0:普通寄存器 , 1:PageA, 2:PageB
	RegAddr = 0x00;		//寄存器地址
	WriteNum = 0x00;		//写入数据的个数
	WriteDataCnt = 0x00;	//获取到的数据个数个数计数
	FD_ChipCmd = 0x00;	//上位机译码后的命令
	FD_ChipMode = 0x00;	//模式选择:默认不进行其他操作
	FD_PrintCmdEna = 0x00;	//是否打印上位机发送的命令,默认不打印
	I2cDelayCnt = 0x00;	//i2c延时需要用到的
	FD_PrintfSysTimeEna = 0x00;	//是否打印时间
	FD_I2cContinuousEna = 0x00;	//i2c是否连续默认不连续
	FD_WaveViewEna = 0x00;		//波形显示使能
	FD_I2cAddrType = I2C_ADDER_TYPE0;	//寄存器地址默认是:I2C_ADDER_TYPE0
	
	ChipType = CMD_FDXXX_CHIP_TYPE_FD650;
	FD6XX_STATE_ADDR = 0x48;
	FD6XX_KEY_ADDR = 0x4F;
	FD6XX_DIG0_ADDR = 0x68;			//<DIG0地址
	FD6XX_DIG1_ADDR = 0x6a;			//<DIG1地址
	FD6XX_DIG2_ADDR = 0x6c;			//<DIG2地址
	FD6XX_DIG3_ADDR = 0x6e;			//<DIG3地址
	FD6XX_DIG4_ADDR = 0x66;			//<DIG4地址
	FD6XX_DIG5_ADDR = 0x70;			//无效
	FD6XX_DIG6_ADDR = 0x72;			//无效
	FD6XX_DIG7_ADDR = 0x74;			//无效

	FD6XX_RegBuf.Cntr = 0x30;
	FD6XX_RegBuf.Dis[0] = 0x00;	
	FD6XX_RegBuf.Dis[1] = 0x00;	
	FD6XX_RegBuf.Dis[2] = 0x00;	
	FD6XX_RegBuf.Dis[3] = 0x00;	
	FD6XX_RegBuf.Dis[4] = 0x00;	
	FD6XX_RegBuf.Dis[5] = 0x00;	
	FD6XX_RegBuf.Dis[6] = 0x00;	
	FD6XX_RegBuf.Dis[7] = 0x00;	
	FD6XX_RegBuf.Key = 0x2e;

	for(i = 0; i < RECEIVE_LENGTH; i++)	//全局变量数据初始化,不能在定义的时候初始化
	{
		ReceiveDataBufe[i].Flag = 0;
		ReceiveDataBufe[i].Data = 0;
	}
	printf("FD 芯片测试软件启动成功\n");
	AppUsart_PrintfInfo();

	FD650_INT = 0;
}

//串口接收数据处理
void AppUsart_UsartDataDeal(void)
{
	 if(ReceiveDataBufe[ReadCnt].Flag)
	 {
		 ReceiveDataBufe[ReadCnt].Flag = 0;		//读到数据,对应的标记位去除
		
		if((ReceiveDataBufeOld == 0x55) && (ReceiveDataBufe[ReadCnt].Data == 0xaa) && (RegCnt == 0)) //滤去接收数据的时候,出现 55aa情况
		 {
		 	 ReceiveStop = 0x00;
			 ReceiveStart = 0x01;
			 RegCnt = 0x00;
			 WriteDataCnt = 0x00;
		 }
		else if((ReceiveDataBufeOld == 0xaa) && (ReceiveDataBufe[ReadCnt].Data == 0x55) && (RegCnt >= (WriteNum + 5)))	//加入 WriteNum 是为了防止命令和数据混乱
		{
			ReceiveStop = 0x01;
			ReceiveStart = 0x00;
			RegCnt = 0x00;	
		}
		
		if(ReceiveStart)
		{
			
			switch(RegCnt)
			{
				case 0 : break;
				case 1 : RegType = ReceiveDataBufe[ReadCnt].Data; break;	//数据类型
				case 2 : RegAddr = ReceiveDataBufe[ReadCnt].Data; break;	//寄存器地址
				case 3 : WriteNum = ReceiveDataBufe[ReadCnt].Data; break;	//数据个数
				default : break;
			}	
			if((RegCnt >= 4) /* && ((RegType & 0x7f) == 0x07) */)		
			{
				WriteDataBufe[WriteDataCnt] = ReceiveDataBufe[ReadCnt].Data;	
				WriteDataCnt ++;	
				if(WriteDataCnt >= WRITE_DATA_LENGTH)
				{
					RegCnt = 0;				//超过长度了,清零处理!!!!!!!!!!!!!!!!!!!!!!
					WriteDataCnt = 0;
				}
			}
			RegCnt ++;								//最多不能超过65535,否则会溢出
		}
		
		ReceiveDataBufeOld = ReceiveDataBufe[ReadCnt].Data;	//记录上一个数据

		 ReadCnt++;
		 if(ReadCnt >= RECEIVE_LENGTH)
		 {
			 ReadCnt = 0;
		 }
	 }
}

//模式操作
void AppUsart_ModeDeal()
{
	switch(FD_ChipMode)
	{
		case CMD_FDXXX_MODE_OTHER : break;						//其他模式的时候不进行任何操作
		// case CMD_FDXXX_MODE_DIS0 : AppUsart_ReadKey(0); break;
		// case CMD_FDXXX_MODE_DIS1 : AppUsart_ReadKey(0);break;
		// case CMD_FDXXX_MODE_KEY0 : AppUsart_ReadKey(0); break;	//上位机不进行操作,只进行读按键操作
		// case CMD_FDXXX_MODE_KEY1 : AppUsart_ReadKey(0); break;
		default : AppUsart_ReadKey(0); break;
	}
}

//刷新显示
void AppUsart_Refresh()
{
	I2C_WrAddrData(FD6XX_STATE_ADDR, FD6XX_RegBuf.Cntr);
	I2C_WrAddrData(FD6XX_DIG0_ADDR, FD6XX_RegBuf.Dis[0]);
	I2C_WrAddrData(FD6XX_DIG1_ADDR, FD6XX_RegBuf.Dis[1]);
	I2C_WrAddrData(FD6XX_DIG2_ADDR, FD6XX_RegBuf.Dis[2]);
	I2C_WrAddrData(FD6XX_DIG3_ADDR, FD6XX_RegBuf.Dis[3]);
	I2C_WrAddrData(FD6XX_DIG4_ADDR, FD6XX_RegBuf.Dis[4]);
	I2C_WrAddrData(FD6XX_DIG5_ADDR, FD6XX_RegBuf.Dis[5]);
	I2C_WrAddrData(FD6XX_DIG6_ADDR, FD6XX_RegBuf.Dis[6]);
	I2C_WrAddrData(FD6XX_DIG7_ADDR, FD6XX_RegBuf.Dis[7]);
}

//i2c速度延时函数 
void AppUsart_I2cDelay(void)
{
	int i = 0;
	int Temp = I2cDelayCnt;
	while(Temp --)
	{
		i = 2;
		while(i --);
	}
}

void AppUsart_WaveView(u8 Ena)
{
	u8 Temp[WAVE_NUM];
	int i = 0;
	static int cnt = 0;
	
	if(Ena && (cnt > 20))
	{
		cnt = 0;
		// I2C_RdAddrStr(FD5012_ADDER_KPV0, Temp, WAVE_NUM);	//当前计数值
		for(i = 0; i < 6; i ++)
		{
			Temp[i] = WaveBufe[i];
		}
		AppUsart_UsartSendData(TYPE_FDXXX_WAVE_VIEW, CMD_FDXXX_WAVE_ALL_EN | 0x80, Temp, WAVE_NUM);
		
		// I2C_RdAddrStr(FD5012_ADDER_KCV0, Temp, WAVE_NUM);	//参考计数值
		// AppUsart_UsartSendData(TYPE_FDXXX_WAVE_VIEW, CMD_FDXXX_WAVE_KCV0 | 0x80, Temp, WAVE_NUM);
	}
	cnt ++;
}

//芯片选择
void AppUsart_ChipSel()
{
	if((ChipType == CMD_FDXXX_CHIP_TYPE_FD650) || (ChipType == CMD_FDXXX_CHIP_TYPE_FD632))
	{
		FD6XX_STATE_ADDR = 0x48;
		FD6XX_KEY_ADDR = 0x4F;
		FD6XX_DIG0_ADDR = 0x68;			//<DIG0地址
		FD6XX_DIG1_ADDR = 0x6a;			//<DIG1地址
		FD6XX_DIG2_ADDR = 0x6c;			//<DIG2地址
		FD6XX_DIG3_ADDR = 0x6e;			//<DIG3地址
		FD6XX_DIG4_ADDR = 0x70;			//无效
		FD6XX_DIG5_ADDR = 0x72;			//无效
		FD6XX_DIG6_ADDR = 0x74;			//无效
		FD6XX_DIG7_ADDR = 0x76;			//无效		
		FD_I2cAddrType = I2C_ADDER_TYPE0;
	}
	else if((ChipType == CMD_FDXXX_CHIP_TYPE_FD638))
	{
		FD6XX_STATE_ADDR = 0x48;
		FD6XX_KEY_ADDR  = 0x4F;
		FD6XX_DIG0_ADDR = 0x60;			//<DIG0地址
		FD6XX_DIG1_ADDR = 0x62;			//<DIG1地址
		FD6XX_DIG2_ADDR = 0x64;			//<DIG2地址
		FD6XX_DIG3_ADDR = 0x66;			//<DIG3地址
		FD6XX_DIG4_ADDR = 0x68;			//<DIG4地址
		FD6XX_DIG5_ADDR = 0x6a;			//<DIG5地址
		FD6XX_DIG6_ADDR = 0x6c;			//<DIG6地址
		FD6XX_DIG7_ADDR = 0x6e;			//<DIG7地址		
		FD_I2cAddrType = I2C_ADDER_TYPE0;
	}
	else if((ChipType == CMD_FDXXX_CHIP_TYPE_FD651))
	{
		FD6XX_STATE_ADDR = 0x48;
		FD6XX_KEY_ADDR = 0x4F;
		FD6XX_DIG0_ADDR = 0x66;			//<DIG0地址
		FD6XX_DIG1_ADDR = 0x68;			//<DIG1地址
		FD6XX_DIG2_ADDR = 0x6a;			//<DIG2地址
		FD6XX_DIG3_ADDR = 0x6c;			//<DIG3地址
		FD6XX_DIG4_ADDR = 0x6e;			//<DIG4地址
		FD6XX_DIG5_ADDR = 0x70;			//无效
		FD6XX_DIG6_ADDR = 0x72;			//无效
		FD6XX_DIG7_ADDR = 0x74;			//无效	
		FD_I2cAddrType = I2C_ADDER_TYPE0;		
	}
	else if((ChipType == CMD_FDXXX_CHIP_TYPE_FD5012))
	{
		FD6XX_STATE_ADDR = 0x08;
		FD5XX_CNTR1_ADDR = 0x09;
		FD6XX_KEY_ADDR  =  FD5012_ADDR_KEYBUF;
		FD6XX_DIG0_ADDR = 0x00;			//<DIG0地址
		FD6XX_DIG1_ADDR = 0x01;			//<DIG1地址
		FD6XX_DIG2_ADDR = 0x02;			//<DIG2地址
		FD6XX_DIG3_ADDR = 0x03;			//<DIG3地址
		FD6XX_DIG4_ADDR = 0x04;			//<DIG4地址
		FD6XX_DIG5_ADDR = 0x05;			
		FD6XX_DIG6_ADDR = 0x06;			
		FD6XX_DIG7_ADDR = 0x07;	
		FD_I2cAddrType = I2C_ADDER_TYPE1;		
	}
}

void AppUsart_CmdDeal()
{
	u8 i = 0;
	if(ReceiveStop)	//其他命令清零
	{
		ReceiveStop = 0x00;	//要清除,不然下次发送的时候,会多打印一次
		WriteDataCnt = 0x00;//清零
		if(RegType == TYPE_FDXX_DISPLAY)	//写显示
		{
			if(ChipType == CMD_FDXXX_CHIP_TYPE_FD5012)
			{
				switch(RegAddr)
				{
					case CMD_DIS0_REG : I2C_WrAddrStr(FD6XX_DIG0_ADDR, WriteDataBufe,WriteNum > 8 ? 8 : WriteNum); break;
					case CMD_DIS1_REG : I2C_WrAddrStr(FD6XX_DIG1_ADDR, WriteDataBufe,WriteNum > 7 ? 7 : WriteNum); break;
					case CMD_DIS2_REG : I2C_WrAddrStr(FD6XX_DIG2_ADDR, WriteDataBufe,WriteNum > 6 ? 6 : WriteNum); break;
					case CMD_DIS3_REG : I2C_WrAddrStr(FD6XX_DIG3_ADDR, WriteDataBufe,WriteNum > 5 ? 5 : WriteNum); break;
					case CMD_DIS4_REG : I2C_WrAddrStr(FD6XX_DIG4_ADDR, WriteDataBufe,WriteNum > 4 ? 4 : WriteNum); break;
					case CMD_DIS5_REG : I2C_WrAddrStr(FD6XX_DIG5_ADDR, WriteDataBufe,WriteNum > 3 ? 3 : WriteNum); break;
					case CMD_DIS6_REG : I2C_WrAddrStr(FD6XX_DIG6_ADDR, WriteDataBufe,WriteNum > 2 ? 2 : WriteNum); break;
					case CMD_DIS7_REG : I2C_WrAddrStr(FD6XX_DIG7_ADDR, WriteDataBufe,WriteNum > 1 ? 1 : WriteNum); break;
					default : break;
				}
			}
			else
			{
				switch(RegAddr)
				{
					case CMD_DIS0_REG : I2C_WrAddrStr(FD6XX_DIG0_ADDR, WriteDataBufe,WriteNum); break;
					case CMD_DIS1_REG : I2C_WrAddrStr(FD6XX_DIG1_ADDR, WriteDataBufe,WriteNum); break;
					case CMD_DIS2_REG : I2C_WrAddrStr(FD6XX_DIG2_ADDR, WriteDataBufe,WriteNum); break;
					case CMD_DIS3_REG : I2C_WrAddrStr(FD6XX_DIG3_ADDR, WriteDataBufe,WriteNum); break;
					case CMD_DIS4_REG : I2C_WrAddrStr(FD6XX_DIG4_ADDR, WriteDataBufe,WriteNum); break;
					case CMD_DIS5_REG : I2C_WrAddrStr(FD6XX_DIG5_ADDR, WriteDataBufe,WriteNum); break;
					case CMD_DIS6_REG : I2C_WrAddrStr(FD6XX_DIG6_ADDR, WriteDataBufe,WriteNum); break;
					case CMD_DIS7_REG : I2C_WrAddrStr(FD6XX_DIG7_ADDR, WriteDataBufe,WriteNum); break;
					default : break;
				}				
			}
			for(i = 0; (i < WriteNum) && (i < DIS_LENGTH); i++)
			{
				FD6XX_RegBuf.Dis[RegAddr + i] = WriteDataBufe[i];	//写寄存器写入缓存中
			}
		}
		else if(RegType == TYPE_FDXX_KEY) //读按键
		{
			if(RegAddr == CMD_KEY_REG)	//只读一次按键
			{
				AppUsart_ReadKey(1);			
			}
		}
		else if(RegType == TYPE_FDXX_REG)//寄存器配置
		{
			switch(RegAddr)
			{
				case CMD_CNTR_REG : I2C_WrAddrData(FD6XX_STATE_ADDR, WriteDataBufe[0]);FD6XX_RegBuf.Cntr = WriteDataBufe[0]; break;
				case CMD_CNTR_LIGHT_REG : FD6XX_RegBuf.Cntr = FD6XX_RegBuf.Cntr & 0x8f; FD6XX_RegBuf.Cntr =FD6XX_RegBuf.Cntr | (WriteDataBufe[0] << 4);I2C_WrAddrData(FD6XX_STATE_ADDR, FD6XX_RegBuf.Cntr); break;
				case CMD_CNTR1_REG : I2C_WrAddrData(FD5XX_CNTR1_ADDR, WriteDataBufe[0]);/* FD6XX_RegBuf.Cntr = WriteDataBufe[0]; */ break;
				case CMD_MSDIFF : I2C_WrAddrData(FD5012_ADDR_MSDIFF, WriteDataBufe[0]); break;
				case CMD_BDV : I2C_WrAddrData(FD5012_ADDR_BDV, WriteDataBufe[0]); break;
				
				case CMD_REG_KDV0  : I2C_WrAddrData(FD5012_ADDER_KDV0 , WriteDataBufe[0]); break;
				case CMD_REG_KDV1  : I2C_WrAddrData(FD5012_ADDER_KDV1 , WriteDataBufe[0]); break;
				case CMD_REG_KDV2  : I2C_WrAddrData(FD5012_ADDER_KDV2 , WriteDataBufe[0]); break;
				case CMD_REG_KDV3  : I2C_WrAddrData(FD5012_ADDER_KDV3 , WriteDataBufe[0]); break;
				case CMD_REG_KDV4  : I2C_WrAddrData(FD5012_ADDER_KDV4 , WriteDataBufe[0]); break;
				case CMD_REG_KDV5  : I2C_WrAddrData(FD5012_ADDER_KDV5 , WriteDataBufe[0]); break;
				case CMD_REG_KDV6  : I2C_WrAddrData(FD5012_ADDER_KDV6 , WriteDataBufe[0]); break;
				case CMD_REG_KDV7  : I2C_WrAddrData(FD5012_ADDER_KDV7 , WriteDataBufe[0]); break;
				case CMD_REG_KDV8  : I2C_WrAddrData(FD5012_ADDER_KDV8 , WriteDataBufe[0]); break;
				case CMD_REG_KDV9  : I2C_WrAddrData(FD5012_ADDER_KDV9 , WriteDataBufe[0]); break;
				case CMD_REG_KDV10 : I2C_WrAddrData(FD5012_ADDER_KDV10, WriteDataBufe[0]); break;
				case CMD_REG_KDV11 : I2C_WrAddrData(FD5012_ADDER_KDV11, WriteDataBufe[0]); break;
				
				case CMD_REG_KUV0  : I2C_WrAddrData(FD5012_ADDER_KUV0 , WriteDataBufe[0]); break;
				case CMD_REG_KUV1  : I2C_WrAddrData(FD5012_ADDER_KUV1 , WriteDataBufe[0]); break;
				case CMD_REG_KUV2  : I2C_WrAddrData(FD5012_ADDER_KUV2 , WriteDataBufe[0]); break;
				case CMD_REG_KUV3  : I2C_WrAddrData(FD5012_ADDER_KUV3 , WriteDataBufe[0]); break;
				case CMD_REG_KUV4  : I2C_WrAddrData(FD5012_ADDER_KUV4 , WriteDataBufe[0]); break;
				case CMD_REG_KUV5  : I2C_WrAddrData(FD5012_ADDER_KUV5 , WriteDataBufe[0]); break;
				case CMD_REG_KUV6  : I2C_WrAddrData(FD5012_ADDER_KUV6 , WriteDataBufe[0]); break;
				case CMD_REG_KUV7  : I2C_WrAddrData(FD5012_ADDER_KUV7 , WriteDataBufe[0]); break;
				case CMD_REG_KUV8  : I2C_WrAddrData(FD5012_ADDER_KUV8 , WriteDataBufe[0]); break;
				case CMD_REG_KUV9  : I2C_WrAddrData(FD5012_ADDER_KUV9 , WriteDataBufe[0]); break;
				case CMD_REG_KUV10 : I2C_WrAddrData(FD5012_ADDER_KUV10, WriteDataBufe[0]); break;
				case CMD_REG_KUV11 : I2C_WrAddrData(FD5012_ADDER_KUV11, WriteDataBufe[0]); break;

				case CMD_REG_KAV0  : I2C_WrAddrData(FD5012_ADDER_KAV0 , WriteDataBufe[0]); break;
				case CMD_REG_KAV1  : I2C_WrAddrData(FD5012_ADDER_KAV1 , WriteDataBufe[0]); break;
				case CMD_REG_KAV2  : I2C_WrAddrData(FD5012_ADDER_KAV2 , WriteDataBufe[0]); break;
				case CMD_REG_KAV3  : I2C_WrAddrData(FD5012_ADDER_KAV3 , WriteDataBufe[0]); break;
				case CMD_REG_KAV4  : I2C_WrAddrData(FD5012_ADDER_KAV4 , WriteDataBufe[0]); break;
				case CMD_REG_KAV5  : I2C_WrAddrData(FD5012_ADDER_KAV5 , WriteDataBufe[0]); break;
				case CMD_REG_KAV6  : I2C_WrAddrData(FD5012_ADDER_KAV6 , WriteDataBufe[0]); break;
				case CMD_REG_KAV7  : I2C_WrAddrData(FD5012_ADDER_KAV7 , WriteDataBufe[0]); break;
				case CMD_REG_KAV8  : I2C_WrAddrData(FD5012_ADDER_KAV8 , WriteDataBufe[0]); break;
				case CMD_REG_KAV9  : I2C_WrAddrData(FD5012_ADDER_KAV9 , WriteDataBufe[0]); break;
				case CMD_REG_KAV10 : I2C_WrAddrData(FD5012_ADDER_KAV10, WriteDataBufe[0]); break;
				case CMD_REG_KAV11 : I2C_WrAddrData(FD5012_ADDER_KAV11, WriteDataBufe[0]); break;
				
				default : break;
			}
			
		}
		else if(RegType == TYPE_FDXX_REG_BIT_SET)	//读写寄存器:按位操作置位
		{
			switch(RegAddr)
			{
				case CMD_CNTR_BIT_REG : FD6XX_RegBuf.Cntr = FD6XX_RegBuf.Cntr | WriteDataBufe[0]; I2C_WrAddrData(FD6XX_STATE_ADDR, FD6XX_RegBuf.Cntr); FD6XX_RegBuf.Cntr = FD6XX_RegBuf.Cntr & 0xFB; break;	
				case CMD_DIS0_BIT_REG : I2C_WrAddrData(FD6XX_DIG0_ADDR, FD6XX_RegBuf.Dis[0] | WriteDataBufe[0]); break;
				case CMD_DIS1_BIT_REG : I2C_WrAddrData(FD6XX_DIG1_ADDR, FD6XX_RegBuf.Dis[1] | WriteDataBufe[0]); break;
				case CMD_DIS2_BIT_REG : I2C_WrAddrData(FD6XX_DIG2_ADDR, FD6XX_RegBuf.Dis[2] | WriteDataBufe[0]); break;
				case CMD_DIS3_BIT_REG : I2C_WrAddrData(FD6XX_DIG3_ADDR, FD6XX_RegBuf.Dis[3] | WriteDataBufe[0]); break;
				case CMD_DIS4_BIT_REG : I2C_WrAddrData(FD6XX_DIG4_ADDR, FD6XX_RegBuf.Dis[4] | WriteDataBufe[0]); break;
				case CMD_DIS5_BIT_REG : I2C_WrAddrData(FD6XX_DIG5_ADDR, FD6XX_RegBuf.Dis[5] | WriteDataBufe[0]); break;
				case CMD_DIS6_BIT_REG : I2C_WrAddrData(FD6XX_DIG6_ADDR, FD6XX_RegBuf.Dis[6] | WriteDataBufe[0]); break;
				case CMD_DIS7_BIT_REG : I2C_WrAddrData(FD6XX_DIG7_ADDR, FD6XX_RegBuf.Dis[7] | WriteDataBufe[0]); break;
				case CMD_CNTR1_BIT_REG : FD5XX_Cntr1RegBuf = FD5XX_Cntr1RegBuf | WriteDataBufe[0]; I2C_WrAddrData(FD5XX_CNTR1_ADDR, FD5XX_Cntr1RegBuf); FD5XX_Cntr1RegBuf = FD5XX_Cntr1RegBuf & 0xEB; break;	
				default : break;
			}
		}
		else if(RegType == TYPE_FDXX_REG_BIT_CLR)	//读写寄存器:按位操作清零
		{
			switch(RegAddr)
			{
				case CMD_CNTR_BIT_REG : FD6XX_RegBuf.Cntr = FD6XX_RegBuf.Cntr  & (~WriteDataBufe[0]); I2C_WrAddrData(FD6XX_STATE_ADDR, FD6XX_RegBuf.Cntr); FD6XX_RegBuf.Cntr = FD6XX_RegBuf.Cntr & 0xFB; break;	
				case CMD_DIS0_BIT_REG : I2C_WrAddrData(FD6XX_DIG0_ADDR, FD6XX_RegBuf.Dis[0] & (~WriteDataBufe[0]));break;
				case CMD_DIS1_BIT_REG : I2C_WrAddrData(FD6XX_DIG1_ADDR, FD6XX_RegBuf.Dis[1] & (~WriteDataBufe[0]));break;
				case CMD_DIS2_BIT_REG : I2C_WrAddrData(FD6XX_DIG2_ADDR, FD6XX_RegBuf.Dis[2] & (~WriteDataBufe[0]));break;
				case CMD_DIS3_BIT_REG : I2C_WrAddrData(FD6XX_DIG3_ADDR, FD6XX_RegBuf.Dis[3] & (~WriteDataBufe[0]));break;
				case CMD_DIS4_BIT_REG : I2C_WrAddrData(FD6XX_DIG4_ADDR, FD6XX_RegBuf.Dis[4] & (~WriteDataBufe[0]));break;
				case CMD_DIS5_BIT_REG : I2C_WrAddrData(FD6XX_DIG5_ADDR, FD6XX_RegBuf.Dis[5] & (~WriteDataBufe[0]));break;
				case CMD_DIS6_BIT_REG : I2C_WrAddrData(FD6XX_DIG6_ADDR, FD6XX_RegBuf.Dis[6] & (~WriteDataBufe[0]));break;
				case CMD_DIS7_BIT_REG : I2C_WrAddrData(FD6XX_DIG7_ADDR, FD6XX_RegBuf.Dis[7] & (~WriteDataBufe[0]));break;
				case CMD_CNTR1_BIT_REG : FD5XX_Cntr1RegBuf = FD5XX_Cntr1RegBuf & (~WriteDataBufe[0]); I2C_WrAddrData(FD5XX_CNTR1_ADDR, FD5XX_Cntr1RegBuf); FD5XX_Cntr1RegBuf = FD5XX_Cntr1RegBuf & 0xEB; break;	
				default : break;
			}
		}
		else if(RegType == TYPE_FDXXX_MODE)	//模式选择
		{
			FD_ChipMode = RegAddr;	//获取模式的指令代码
		}
		else if(RegType == TYPE_MCU)		//CPU操作命令
		{
			#ifdef STC12LE5A60S2   	//单片机对应的复位操作
				 if(RegAddr == CMD_CPU_RST)
				 {
//				 	  IAP_CONTR |= 0x20;	//单片机复位
				 }
			#endif
		}
		else if(RegType == TYPE_FDXXX_FUN)	//其他功能模式
		{
			switch(RegAddr)
			{
				case CMD_FDXXX_FUN_REFRESH : AppUsart_Refresh(); break;	//刷新下显示
				case CMD_FDXXX_FUN_PRINTF_INFO : AppUsart_PrintfInfo(); break; 		//打印信息
				case CMD_FDXXX_FUN_PRINFT_CDM_EN : FD_PrintCmdEna = 0x01; break;	//打印命令
				case CMD_FDXXX_FUN_PRINFT_CMD_DISABLE: FD_PrintCmdEna = 0x00; break; //不打印命令
				case CMD_FDXXX_FUN_CMD_I2C_SPEED : I2cDelayCnt = WriteDataBufe[0]; break;
				case CMD_FDXXX_FUN_CMD_I2C_10K : I2cDelayCnt = 255; break;
				case CMD_FDXXX_FUN_CMD_I2C_20K : I2cDelayCnt = 150; break;
				case CMD_FDXXX_FUN_CMD_I2C_50K : I2cDelayCnt = 200; break;
				case CMD_FDXXX_FUN_CMD_I2C_100K: I2cDelayCnt = 150; break;
				case CMD_FDXXX_FUN_CMD_I2C_200K: I2cDelayCnt = 100; break;
				case CMD_FDXXX_FUN_CMD_I2C_500K: I2cDelayCnt = 50; break;
				case CMD_FDXXX_FUN_CMD_I2C_1M  : I2cDelayCnt = 20; break;
				case CMD_FDXXX_FUN_CMD_I2C_2M  : I2cDelayCnt = 10; break;
				case CMD_FDXXX_FUN_CMD_I2C_4M  : I2cDelayCnt = 0; break;
				case CMD_FDXXX_FUN_CMD_PRINTF_TIME_EN  : if(!FD_PrintCmdEna) {AppUsart_PrintfSysTime();} FD_PrintfSysTimeEna = 0x01; break; //打印系统时间
				case CMD_FDXXX_FUN_CMD_PRINTF_TIME_DISABLE : FD_PrintfSysTimeEna = 0x00; break;
				case CMD_FDXXX_FUN_CMD_I2C_CONTINUOUS_EN : FD_I2cContinuousEna = 0x01;break;
				case CMD_FDXXX_FUN_CMD_I2C_CONTINUOUS_DISABLE : FD_I2cContinuousEna = 0x00;break;
				default : break;
			}			
		}
		else if(RegType == TYPE_FDXXX_CHIP_TYPE)
		{
			ChipType = RegAddr;
			AppUsart_ChipSel();
		}
		else if(RegType == TYPE_MY_CMD)	//自定义命令
		{
			AppUsart_UsartSendData(RegType, RegAddr | 0x80,WriteDataBufe, WriteNum);	//返回发送的命令数据
		}
		else if(RegType == TYPE_FDXXX_WAVE_VIEW)	//波形显示
		{
			switch(RegAddr)
			{
				case CMD_FDXXX_WAVE_ALL_EN : if(ChipType == CMD_FDXXX_CHIP_TYPE_FD5012){FD_WaveViewEna = 0x01;}else{printf("当前芯片模式非FD5012,请重新选择FD5012芯片\n");} break; //打印波形使能
				case CMD_FDXXX_WAVE_ALL_DISABLE : FD_WaveViewEna = 0x00; break;	//不打印波形
				default : FD_WaveViewEna = 0x00;break;
			}
		}
		
		if(FD_PrintCmdEna)	//是否打印上位机发送的命令的命令,按键检测的时候,在上位机发送频繁的时候,打印按键值会有问题
		{
			AppUsart_UsartSendData(RegType, RegAddr,WriteDataBufe, WriteNum);	//返回发送的命令数据
		}
	}
}

void AppUsart_Run()
{
	AppUsart_UsartDataDeal();
	AppUsart_CmdDeal();
	AppUsart_ModeDeal();
	AppUsart_WaveView(FD_WaveViewEna);
}
