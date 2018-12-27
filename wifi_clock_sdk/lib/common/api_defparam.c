#include <version.h>
#include <wla_api.h>
#include <cfg_api.h>

sdk_param g_defparam =
{
	CFG_MAGIC,		//HEAD
	CFG_CHKSUM_EN|CFG_RECONN|CFG_AUTO_CONN,	//flags
	0,				//check sum
	0,				//led value
	OPMODE_STA, 	//work mode
	3,				//dhcp mode
	7,				//phy mode
	12,				//txpower
	0,				//ps_mode
	TEMP_C,
	BRIGHTNESS5,
	/*
	{				//servo_moter_param
		1,
		0,
	},
	{				//dc_moter_param
		0,
		0,
	},
	{				//rgbled_param
		1,
		31,
		31,
		31,
	},
	*/
	{				//mac
	},
	{
		115200,		//baudrate;
		8,    		//databits;
		0,			//stopbits
		0,			//parity
		0,			//flowctrl
	},
	{
		"STA1",				//ssid
		"123456789", 			//key
		"192.168.0.105",		//ip
		"255.255.255.0",		//mask
		"192.168.0.1",			//gw

		"STA2",				//ssid
		"123456789", 			//key
		"192.168.0.105",		//ip
		"255.255.255.0",		//mask
		"192.168.0.1",			//gw

		"STA3",				//ssid
		"123456789", 			//key
		"192.168.0.105",		//ip
		"255.255.255.0",		//mask
		"192.168.0.1",			//gw
	},
	{
		"Demo_AP1",				//ap ssid
		"12345678",				//ap key
		11,						//channel
		SECURITY_WPA2_MIXED,	//enc
		4,
		0,
		"192.168.169.1",		//ip
		"255.255.255.0",		//mask
		"192.168.169.1",		//gw
	},
	{
		"192.168.169.2",
		"192.168.169.254",
		1440,
	},	
	/*
	{
		0,						//trans mode when start
		"192.168.169.1",
		8080,
		"TCP",
		0,
	},
	*/
	{							//webtime default setting
		1,
		8,	
		14400,
		32400,
	},							//default alarm time config
#if !VERSION_MUSHAN
	{
		1,
		0,
		32400,
		32400,
		0,
		300,
		
		1,
		0,	
		32400,
		32400,
		0,
		300,

		1,
		0,			
		32400,
		32400,
		0,
		300,

		2,
		0,	
		36000,
		34200,
		0,
		300,

		2,
		0,	
		37800,
		37800,
		0,
		300,

		2,
		0,	
		50400,
		50400,
		0,
		300,

		3,
		0,	
		32400,
		32400,
		0,
		300,

		3,
		0,	
		32400,
		32400,
		0,
		300,

		3,
		0,	
		32400,
		32400,
		0,
		300,

		4,
		0,	
		32400,
		32400,
		0,
		300,

		4,
		0,	
		32400,
		32400,
		0,
		300,

		4,
		0,	
		32400,
		32400,
		0,
		300,

		5,
		0,	
		32400,
		32400,
		0,
		300,

		5,
		0,	
		32400,
		32400,
		0,
		300,

		5,
		0,	
		32400,
		32400,
		0,
		300,

		6,
		0,	
		32400,
		32400,
		0,
		300,

		6,
		0,	
		32400,
		32400,
		0,
		300,
		
		6,
		0,	
		32400,
		32400,
		0,
		300,

		7,
		0,	
		32400,
		32400,
		0,
		300,

		7,
		0,	
		32400,
		32400,
		0,
		300,

		7,
		0,	
		32400,
		32400,
		0,
		300,
	},
#endif
#if VERSION_MUSHAN
	{
		1,
		11210505,
	},
	{			// ACID
		"123",
	},
	// Device ID
	{
		100,	
	},
	// Web register statue
	0,
#endif
	// config mode
	0,	
	{			//default weather param
		1,
		141,	
		3600,	
	},
	{			//nowtime param
		0,
	},
	{			//time format
		FORMAT_24,
	},
};




