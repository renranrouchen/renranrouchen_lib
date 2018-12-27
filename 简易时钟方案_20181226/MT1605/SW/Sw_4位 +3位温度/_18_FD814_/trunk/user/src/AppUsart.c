#include "AppUsart.h"
#include "stdlib.h"
#include "mydelay.h"

sbit   FD650_INT = P3^4;

u8 ReceiveCnt = 0x00;	//�������ݻ������:������дָ��
u8 ReadCnt = 0x00;		//��Ƭ�����ݶ�:�����ڶ�ָ��
u8 ReceiveStop = 0x00;	//0: û�н����� , 1:������
u8 ReceiveStart = 0x00;	//0:ֹͣ���մ���,1:��ʼ���մ���
u8 ReceiveDataBufeOld = 0x00;//��һ�������ļĴ���
u16 RegCnt = 0x00;		//��ֹ�����ݵ�ʱ��,�������,���Զ���Ϊ16λ
u8 RegType = 0x00; 		//�Ĵ�������: 0:��ͨ�Ĵ��� , 1:PageA, 2:PageB
u8 RegAddr = 0x00;		//�Ĵ�����ַ
u8 WriteNum = 0x00;		//д�����ݵĸ���
u8 WriteDataCnt = 0x00;	//��ȡ�������ݸ�����������
u8 FD_ChipCmd = 0x00;	//��λ������������
u8 FD_ChipMode = 0x00;	//ģʽѡ��:Ĭ�ϲ�������������
u8 FD_PrintCmdEna = 0x00;	//�Ƿ��ӡ��λ�����͵�����,Ĭ�ϲ���ӡ
u16 I2cDelayCnt = 0x00;	//i2c��ʱ��Ҫ�õ���
u8 FD_PrintfSysTimeEna = 0x00;	//�Ƿ��ӡʱ��Ĵ���
u8 FD_I2cContinuousEna = 0x00;	//i2c�Ƿ�����Ĭ�ϲ�����
u8 FD_WaveViewEna = 0x00;		//������ʾʹ��
u8 FD_I2cAddrType = I2C_ADDER_TYPE0;	//�Ĵ�����ַĬ����:I2C_ADDER_TYPE0

u8 ChipType = CMD_FDXXX_CHIP_TYPE_FD650;
u8 FD6XX_STATE_ADDR = 0x48;
u8 FD6XX_KEY_ADDR = 0x4F;
u8 FD6XX_DIG0_ADDR = 0x68;			//<DIG0��ַ
u8 FD6XX_DIG1_ADDR = 0x6a;			//<DIG1��ַ
u8 FD6XX_DIG2_ADDR = 0x6c;			//<DIG2��ַ
u8 FD6XX_DIG3_ADDR = 0x6e;			//<DIG3��ַ
u8 FD6XX_DIG4_ADDR = 0x66;			//<DIG4��ַ
u8 FD6XX_DIG5_ADDR = 0x70;			//��Ч
u8 FD6XX_DIG6_ADDR = 0x72;			//��Ч
u8 FD6XX_DIG7_ADDR = 0x74;			//��Ч
u8 FD5XX_CNTR1_ADDR = 0x09;
u8 FD5XX_Cntr1RegBuf = 0x78;		//Cntr1�ϵ�Ĭ��״̬

typedef struct 
{
	u8 Cntr;
	u8 Key;
	u8 Dis[DIS_LENGTH];
} FD6XX_Reg; 

FD6XX_Reg FD6XX_RegBuf;

ReceiveData ReceiveDataBufe[RECEIVE_LENGTH];
u8 WriteDataBufe[WRITE_DATA_LENGTH];

//�������ݸ���λ��
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

//��ӡϵͳʱ��
void AppUsart_PrintfSysTime()
{
	AppUsart_UsartSendData(TYPE_FDXXX_FUN, CMD_FDXXX_FUN_CMD_PRINTF_TIME_EN, "", 0);
}

//������ȡ
void AppUsart_ReadKey(u8 mode)	// 0: ��ӡ�仯 , 1:ֱ�Ӵ�ӡ����ֵ
{
	u8 KeyNum = 0;
	KeyNum = I2C_RdAddrData(FD6XX_KEY_ADDR);
	if((KeyNum != FD6XX_RegBuf.Key) | mode)	//�а������»��߰���ֵ��һ��
	{
		if(FD_PrintfSysTimeEna)
		{
			AppUsart_PrintfSysTime();
		}
		FD650_INT = 1;
		printf("KEY = %02X\n", (u16)KeyNum);
		FD650_INT = 0;
		FD6XX_RegBuf.Key = KeyNum;	//���¼Ĵ�����ֵ //��¼��һ������ֵ
		AppUsart_UsartSendData(TYPE_FDXX_KEY, CMD_KEY_REG | 0x80,&FD6XX_RegBuf.Key, 1);
	}
}


//��ӡ��Ϣ
void AppUsart_PrintfInfo()
{
	printf("��ǰоƬ��Ϣ:\n");
	AppUsart_PrintfSysTime();
	switch(ChipType)
	{
		case CMD_FDXXX_CHIP_TYPE_FD650 : printf("оƬģʽ: FD650\n"); break;
		case CMD_FDXXX_CHIP_TYPE_FD651 : printf("оƬģʽ: FD651\n"); break;
		case CMD_FDXXX_CHIP_TYPE_FD638 : printf("оƬģʽ: FD638\n"); break;
		case CMD_FDXXX_CHIP_TYPE_FD632 : printf("оƬģʽ: FD632\n"); break;
		case CMD_FDXXX_CHIP_TYPE_FD5012 : printf("оƬģʽ: FD5012\n"); break;
		default : printf("оƬģʽ: FD650\n"); break;
	}
	switch(FD_ChipMode)
	{
		case 0 : printf("����ģʽ: ��ʾģʽ0\n"); break;
		case 1 : printf("����ģʽ: ��ʾģʽ1\n"); break;
		case 2 : printf("����ģʽ: ����ģʽ0\n"); break;
		case 3 : printf("����ģʽ: ����ģʽ1\n"); break;
		case 4 : printf("����ģʽ: ����ģʽ\n"); break;
		case 5 : printf("����ģʽ: ��ʾʱ��\n"); break;
		case 6 : printf("����ģʽ: �ļ�����ѭ��\n"); break;
		default : printf("����ģʽ: ��ʾģʽ0\n"); break;
	}
	printf("CNTR = %02X\n", (u16)FD6XX_RegBuf.Cntr);	//��ӡ�Ĵ���ֵ
	AppUsart_ReadKey(1);
	printf("DIS0 = %02X; DIS1 = %02X; DIS2 = %02X; DIS3 = %02X; \nDIS4 = %02X; DIS5 = %02X; DIS6 = %02X; DIS7 = %02X; \n", \
	(u16)FD6XX_RegBuf.Dis[0], (u16)FD6XX_RegBuf.Dis[1], (u16)FD6XX_RegBuf.Dis[2], (u16)FD6XX_RegBuf.Dis[3], \
	(u16)FD6XX_RegBuf.Dis[4], (u16)FD6XX_RegBuf.Dis[5], (u16)FD6XX_RegBuf.Dis[6], (u16)FD6XX_RegBuf.Dis[7] );	//��ӡ��ʾֵ 

	printf("DIS0~DIS7 = %02X %02X %02X %02X %02X %02X %02X %02X \n", \
	(u16)FD6XX_RegBuf.Dis[0], (u16)FD6XX_RegBuf.Dis[1], (u16)FD6XX_RegBuf.Dis[2], (u16)FD6XX_RegBuf.Dis[3], \
	(u16)FD6XX_RegBuf.Dis[4], (u16)FD6XX_RegBuf.Dis[5], (u16)FD6XX_RegBuf.Dis[6], (u16)FD6XX_RegBuf.Dis[7] );	//��ӡ��ʾֵ 

	printf("\n");
}

void AppUsart_Init()
{
	u16 i;
	ReceiveCnt = 0x00;	//�������ݻ������:������дָ��
	ReadCnt = 0x00;		//��Ƭ�����ݶ�:�����ڶ�ָ��
	ReceiveStop = 0x00;	//0: û�н����� , 1:������
	ReceiveStart = 0x00;	//0:ֹͣ���մ���,1:��ʼ���մ���
	ReceiveDataBufeOld = 0x00;//��һ�������ļĴ���
	RegCnt = 0x00;		//��ֹ�����ݵ�ʱ��,�������,���Զ���Ϊ16λ
	RegType = 0x00; 		//�Ĵ�������: 0:��ͨ�Ĵ��� , 1:PageA, 2:PageB
	RegAddr = 0x00;		//�Ĵ�����ַ
	WriteNum = 0x00;		//д�����ݵĸ���
	WriteDataCnt = 0x00;	//��ȡ�������ݸ�����������
	FD_ChipCmd = 0x00;	//��λ������������
	FD_ChipMode = 0x00;	//ģʽѡ��:Ĭ�ϲ�������������
	FD_PrintCmdEna = 0x00;	//�Ƿ��ӡ��λ�����͵�����,Ĭ�ϲ���ӡ
	I2cDelayCnt = 0x00;	//i2c��ʱ��Ҫ�õ���
	FD_PrintfSysTimeEna = 0x00;	//�Ƿ��ӡʱ��
	FD_I2cContinuousEna = 0x00;	//i2c�Ƿ�����Ĭ�ϲ�����
	FD_WaveViewEna = 0x00;		//������ʾʹ��
	FD_I2cAddrType = I2C_ADDER_TYPE0;	//�Ĵ�����ַĬ����:I2C_ADDER_TYPE0
	
	ChipType = CMD_FDXXX_CHIP_TYPE_FD650;
	FD6XX_STATE_ADDR = 0x48;
	FD6XX_KEY_ADDR = 0x4F;
	FD6XX_DIG0_ADDR = 0x68;			//<DIG0��ַ
	FD6XX_DIG1_ADDR = 0x6a;			//<DIG1��ַ
	FD6XX_DIG2_ADDR = 0x6c;			//<DIG2��ַ
	FD6XX_DIG3_ADDR = 0x6e;			//<DIG3��ַ
	FD6XX_DIG4_ADDR = 0x66;			//<DIG4��ַ
	FD6XX_DIG5_ADDR = 0x70;			//��Ч
	FD6XX_DIG6_ADDR = 0x72;			//��Ч
	FD6XX_DIG7_ADDR = 0x74;			//��Ч

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

	for(i = 0; i < RECEIVE_LENGTH; i++)	//ȫ�ֱ������ݳ�ʼ��,�����ڶ����ʱ���ʼ��
	{
		ReceiveDataBufe[i].Flag = 0;
		ReceiveDataBufe[i].Data = 0;
	}
	printf("FD оƬ������������ɹ�\n");
	AppUsart_PrintfInfo();

	FD650_INT = 0;
}

//���ڽ������ݴ���
void AppUsart_UsartDataDeal(void)
{
	 if(ReceiveDataBufe[ReadCnt].Flag)
	 {
		 ReceiveDataBufe[ReadCnt].Flag = 0;		//��������,��Ӧ�ı��λȥ��
		
		if((ReceiveDataBufeOld == 0x55) && (ReceiveDataBufe[ReadCnt].Data == 0xaa) && (RegCnt == 0)) //��ȥ�������ݵ�ʱ��,���� 55aa���
		 {
		 	 ReceiveStop = 0x00;
			 ReceiveStart = 0x01;
			 RegCnt = 0x00;
			 WriteDataCnt = 0x00;
		 }
		else if((ReceiveDataBufeOld == 0xaa) && (ReceiveDataBufe[ReadCnt].Data == 0x55) && (RegCnt >= (WriteNum + 5)))	//���� WriteNum ��Ϊ�˷�ֹ��������ݻ���
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
				case 1 : RegType = ReceiveDataBufe[ReadCnt].Data; break;	//��������
				case 2 : RegAddr = ReceiveDataBufe[ReadCnt].Data; break;	//�Ĵ�����ַ
				case 3 : WriteNum = ReceiveDataBufe[ReadCnt].Data; break;	//���ݸ���
				default : break;
			}	
			if((RegCnt >= 4) /* && ((RegType & 0x7f) == 0x07) */)		
			{
				WriteDataBufe[WriteDataCnt] = ReceiveDataBufe[ReadCnt].Data;	
				WriteDataCnt ++;	
				if(WriteDataCnt >= WRITE_DATA_LENGTH)
				{
					RegCnt = 0;				//����������,���㴦��!!!!!!!!!!!!!!!!!!!!!!
					WriteDataCnt = 0;
				}
			}
			RegCnt ++;								//��಻�ܳ���65535,��������
		}
		
		ReceiveDataBufeOld = ReceiveDataBufe[ReadCnt].Data;	//��¼��һ������

		 ReadCnt++;
		 if(ReadCnt >= RECEIVE_LENGTH)
		 {
			 ReadCnt = 0;
		 }
	 }
}

//ģʽ����
void AppUsart_ModeDeal()
{
	switch(FD_ChipMode)
	{
		case CMD_FDXXX_MODE_OTHER : break;						//����ģʽ��ʱ�򲻽����κβ���
		// case CMD_FDXXX_MODE_DIS0 : AppUsart_ReadKey(0); break;
		// case CMD_FDXXX_MODE_DIS1 : AppUsart_ReadKey(0);break;
		// case CMD_FDXXX_MODE_KEY0 : AppUsart_ReadKey(0); break;	//��λ�������в���,ֻ���ж���������
		// case CMD_FDXXX_MODE_KEY1 : AppUsart_ReadKey(0); break;
		default : AppUsart_ReadKey(0); break;
	}
}

//ˢ����ʾ
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

//i2c�ٶ���ʱ���� 
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
		// I2C_RdAddrStr(FD5012_ADDER_KPV0, Temp, WAVE_NUM);	//��ǰ����ֵ
		for(i = 0; i < 6; i ++)
		{
			Temp[i] = WaveBufe[i];
		}
		AppUsart_UsartSendData(TYPE_FDXXX_WAVE_VIEW, CMD_FDXXX_WAVE_ALL_EN | 0x80, Temp, WAVE_NUM);
		
		// I2C_RdAddrStr(FD5012_ADDER_KCV0, Temp, WAVE_NUM);	//�ο�����ֵ
		// AppUsart_UsartSendData(TYPE_FDXXX_WAVE_VIEW, CMD_FDXXX_WAVE_KCV0 | 0x80, Temp, WAVE_NUM);
	}
	cnt ++;
}

//оƬѡ��
void AppUsart_ChipSel()
{
	if((ChipType == CMD_FDXXX_CHIP_TYPE_FD650) || (ChipType == CMD_FDXXX_CHIP_TYPE_FD632))
	{
		FD6XX_STATE_ADDR = 0x48;
		FD6XX_KEY_ADDR = 0x4F;
		FD6XX_DIG0_ADDR = 0x68;			//<DIG0��ַ
		FD6XX_DIG1_ADDR = 0x6a;			//<DIG1��ַ
		FD6XX_DIG2_ADDR = 0x6c;			//<DIG2��ַ
		FD6XX_DIG3_ADDR = 0x6e;			//<DIG3��ַ
		FD6XX_DIG4_ADDR = 0x70;			//��Ч
		FD6XX_DIG5_ADDR = 0x72;			//��Ч
		FD6XX_DIG6_ADDR = 0x74;			//��Ч
		FD6XX_DIG7_ADDR = 0x76;			//��Ч		
		FD_I2cAddrType = I2C_ADDER_TYPE0;
	}
	else if((ChipType == CMD_FDXXX_CHIP_TYPE_FD638))
	{
		FD6XX_STATE_ADDR = 0x48;
		FD6XX_KEY_ADDR  = 0x4F;
		FD6XX_DIG0_ADDR = 0x60;			//<DIG0��ַ
		FD6XX_DIG1_ADDR = 0x62;			//<DIG1��ַ
		FD6XX_DIG2_ADDR = 0x64;			//<DIG2��ַ
		FD6XX_DIG3_ADDR = 0x66;			//<DIG3��ַ
		FD6XX_DIG4_ADDR = 0x68;			//<DIG4��ַ
		FD6XX_DIG5_ADDR = 0x6a;			//<DIG5��ַ
		FD6XX_DIG6_ADDR = 0x6c;			//<DIG6��ַ
		FD6XX_DIG7_ADDR = 0x6e;			//<DIG7��ַ		
		FD_I2cAddrType = I2C_ADDER_TYPE0;
	}
	else if((ChipType == CMD_FDXXX_CHIP_TYPE_FD651))
	{
		FD6XX_STATE_ADDR = 0x48;
		FD6XX_KEY_ADDR = 0x4F;
		FD6XX_DIG0_ADDR = 0x66;			//<DIG0��ַ
		FD6XX_DIG1_ADDR = 0x68;			//<DIG1��ַ
		FD6XX_DIG2_ADDR = 0x6a;			//<DIG2��ַ
		FD6XX_DIG3_ADDR = 0x6c;			//<DIG3��ַ
		FD6XX_DIG4_ADDR = 0x6e;			//<DIG4��ַ
		FD6XX_DIG5_ADDR = 0x70;			//��Ч
		FD6XX_DIG6_ADDR = 0x72;			//��Ч
		FD6XX_DIG7_ADDR = 0x74;			//��Ч	
		FD_I2cAddrType = I2C_ADDER_TYPE0;		
	}
	else if((ChipType == CMD_FDXXX_CHIP_TYPE_FD5012))
	{
		FD6XX_STATE_ADDR = 0x08;
		FD5XX_CNTR1_ADDR = 0x09;
		FD6XX_KEY_ADDR  =  FD5012_ADDR_KEYBUF;
		FD6XX_DIG0_ADDR = 0x00;			//<DIG0��ַ
		FD6XX_DIG1_ADDR = 0x01;			//<DIG1��ַ
		FD6XX_DIG2_ADDR = 0x02;			//<DIG2��ַ
		FD6XX_DIG3_ADDR = 0x03;			//<DIG3��ַ
		FD6XX_DIG4_ADDR = 0x04;			//<DIG4��ַ
		FD6XX_DIG5_ADDR = 0x05;			
		FD6XX_DIG6_ADDR = 0x06;			
		FD6XX_DIG7_ADDR = 0x07;	
		FD_I2cAddrType = I2C_ADDER_TYPE1;		
	}
}

void AppUsart_CmdDeal()
{
	u8 i = 0;
	if(ReceiveStop)	//������������
	{
		ReceiveStop = 0x00;	//Ҫ���,��Ȼ�´η��͵�ʱ��,����ӡһ��
		WriteDataCnt = 0x00;//����
		if(RegType == TYPE_FDXX_DISPLAY)	//д��ʾ
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
				FD6XX_RegBuf.Dis[RegAddr + i] = WriteDataBufe[i];	//д�Ĵ���д�뻺����
			}
		}
		else if(RegType == TYPE_FDXX_KEY) //������
		{
			if(RegAddr == CMD_KEY_REG)	//ֻ��һ�ΰ���
			{
				AppUsart_ReadKey(1);			
			}
		}
		else if(RegType == TYPE_FDXX_REG)//�Ĵ�������
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
		else if(RegType == TYPE_FDXX_REG_BIT_SET)	//��д�Ĵ���:��λ������λ
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
		else if(RegType == TYPE_FDXX_REG_BIT_CLR)	//��д�Ĵ���:��λ��������
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
		else if(RegType == TYPE_FDXXX_MODE)	//ģʽѡ��
		{
			FD_ChipMode = RegAddr;	//��ȡģʽ��ָ�����
		}
		else if(RegType == TYPE_MCU)		//CPU��������
		{
			#ifdef STC12LE5A60S2   	//��Ƭ����Ӧ�ĸ�λ����
				 if(RegAddr == CMD_CPU_RST)
				 {
//				 	  IAP_CONTR |= 0x20;	//��Ƭ����λ
				 }
			#endif
		}
		else if(RegType == TYPE_FDXXX_FUN)	//��������ģʽ
		{
			switch(RegAddr)
			{
				case CMD_FDXXX_FUN_REFRESH : AppUsart_Refresh(); break;	//ˢ������ʾ
				case CMD_FDXXX_FUN_PRINTF_INFO : AppUsart_PrintfInfo(); break; 		//��ӡ��Ϣ
				case CMD_FDXXX_FUN_PRINFT_CDM_EN : FD_PrintCmdEna = 0x01; break;	//��ӡ����
				case CMD_FDXXX_FUN_PRINFT_CMD_DISABLE: FD_PrintCmdEna = 0x00; break; //����ӡ����
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
				case CMD_FDXXX_FUN_CMD_PRINTF_TIME_EN  : if(!FD_PrintCmdEna) {AppUsart_PrintfSysTime();} FD_PrintfSysTimeEna = 0x01; break; //��ӡϵͳʱ��
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
		else if(RegType == TYPE_MY_CMD)	//�Զ�������
		{
			AppUsart_UsartSendData(RegType, RegAddr | 0x80,WriteDataBufe, WriteNum);	//���ط��͵���������
		}
		else if(RegType == TYPE_FDXXX_WAVE_VIEW)	//������ʾ
		{
			switch(RegAddr)
			{
				case CMD_FDXXX_WAVE_ALL_EN : if(ChipType == CMD_FDXXX_CHIP_TYPE_FD5012){FD_WaveViewEna = 0x01;}else{printf("��ǰоƬģʽ��FD5012,������ѡ��FD5012оƬ\n");} break; //��ӡ����ʹ��
				case CMD_FDXXX_WAVE_ALL_DISABLE : FD_WaveViewEna = 0x00; break;	//����ӡ����
				default : FD_WaveViewEna = 0x00;break;
			}
		}
		
		if(FD_PrintCmdEna)	//�Ƿ��ӡ��λ�����͵����������,��������ʱ��,����λ������Ƶ����ʱ��,��ӡ����ֵ��������
		{
			AppUsart_UsartSendData(RegType, RegAddr,WriteDataBufe, WriteNum);	//���ط��͵���������
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
