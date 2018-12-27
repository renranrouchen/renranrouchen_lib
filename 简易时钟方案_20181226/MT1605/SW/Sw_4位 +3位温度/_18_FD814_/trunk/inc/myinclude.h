#ifndef _MY_INCLUDE_H_
#define _MY_INCLUDE_H_

#include <stdio.h>
#include <intrins.h>
#include <math.h>


//通用配置
#include "mytype.h"		//数据类型:不同的单片机,配置可能不一样
#include "mydelay.h"	//延时函数:不同单片机,延时不同
#include "myconf.h"		//配置文件都在这里面配置:比如是否打印函数,功能开关


/* 下面可配置的 */
//#include "STC12LE5A60S2.H"
#include "FD814.H"
#include "AppUsart.h"
#include "i2c.h"
#include "TouchKey.H"

	
#endif