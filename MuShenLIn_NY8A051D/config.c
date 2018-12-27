/*******************************
*文件名：config.c
*创建者：hwl
*创建时间：2018-8-1
*文件说明：初始化配置
********************************/



#include <config.h>
#include <NY8A051D.h>




/*****************************
*函数名：gpio_config
*参数：void
*返回值：void
*函数功能：IO配置
****************************/
void gpio_config(void)
{
	 BC(BPHCON,7);	
		
		
	 BC(IOSTB,0);BS(PORTB,0); //SDA_PB0  输出高电平
	 BC(IOSTB,1);BS(PORTB,1); //SCL_PB1  输出高电平
	 
	 BC(IOSTB,2);BS(PORTB,2); //ST1_PB2  输出高电平
	 BC(IOSTB,4);BS(PORTB,4); //ST2_PB4  输出高电平
	 //BC(IOSTB,5);BS(PORTB,5); //ST3_PB5   输出高电平
	 BS(IOSTB,5);
	 BS(IOSTB,3);//key输入
	// BC(IOSTB,RELAY_IO);BS(PORTB,RELAY_IO); //继电器PB0 输出高电平
	// BC(IOSTB,LED_IO);//LED PB1 输出
	// BS(IOSTB,KEY_IO);//key输入
	 //BS(IOSTB,DECLAY_IO);//检测输入
}



