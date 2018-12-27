#include "string.h"
#include <lynx_debug.h>
#include <common.h>
#include <cfg_api.h>
#include <event.h>
#include <lynx_dev.h>
#include <wla_api.h>

extern char * strstr(const char *searchee, const char *lookfor);
extern sdk_param g_atParam;
extern sdk_param g_defparam;

int checkIpValuable(char * ip)
{
	if(ip ==NULL)
	{
		return 0;
	}

	char *p_start = NULL;
	char *p_end =NULL;
	int len = 0;

	int value = 0;
	int i = 0;
	p_start = ip;

	for(i = 0 ; i < 3; i++)
	{
		p_end = strstr(p_start,".");
		len +=p_end-p_start+1;
		p_start +=p_end-p_start+1;
	}

	if(!strncmp(g_atParam.ap_param.ip,ip,len))
	{
		value = atoi(&ip[len]);
		return value;
	}

	return -1;

}

unsigned long strtoul( char *nptr,char **endptr,int base)/*only support for 16*/
{
	int len = strlen(nptr),i = 0,j = 0;
	unsigned long uTotalValue = 0,uMidValue = 0;
	char * str = nptr;
	if(base !=16 || nptr == NULL)
	{
		serial_printf("the strtoul only support  16\n");
		return 0;
	}
	if(len > 2)
	{
		if(str[0] == '0' && str[1] == 'x')
		{
			str +=2;
			len -= 2;
		}
	}
	while(len)
	{
		if((str[i] >=  '0' &&  str[i] <= '9') || (str[i] >= 'A' && str[i] <= 'F') || (str[i] >= 'a' && str[i] <= 'f') )
		{
			if(str[i] > '9')
			{
				if(str[i] > 'Z')
				{
					if(len-1 == 0)
					{
						 uMidValue = (str[i] - 'a' +10);
					}
					else
					{
						uMidValue = (str[i] - 'a' +10);
						for(j = 0;j < len-1;j++)
						{
							uMidValue *= 16;
						}
					}
					uTotalValue +=uMidValue;
				}
				else
				{
					if(len-1 == 0)
					{
						 uMidValue = (str[i] - 'A' +10);
					}
					else
					{
						uMidValue = (str[i] - 'A' +10);
						for(j = 0;j < len-1;j++)
						{
							uMidValue *= 16;
						}
					}
					uTotalValue +=uMidValue;
				}
			}
			else
			{
				if(len-1 == 0)
				{
					uMidValue =(str[i] - '0');
				}
				else
				{
					uMidValue =(str[i] - '0');
					for(j = 0;j < len-1;j++)
					{
						uMidValue *= 16;
					}
				}
				uTotalValue +=uMidValue;
			}
			len--;
			i++;
		}
		else
		{
			if(!endptr)
			{
				*endptr = &str[i];	
			}
			serial_printf("### invild char --[%s]\n",nptr);
			return 0;
		}

	}

	return uTotalValue;
}

void macToArray(char *sMac,unsigned char mac[])
{
	char strMacByte[3] = {0};
	int i = 0;
	strMacByte[2] = 0;
	if(sMac == NULL)
	{
		return;
	}
	for(i=0;i<6;i++)
	{
		strMacByte[0] = sMac[i*2];
		strMacByte[1] = sMac[i*2 + 1];
		mac[i] = strtoul(strMacByte, NULL, 16);
	}
}

int parseAddress(char *scr,char * ip,char *mask,char *gw)
{
	char *p_start = NULL;
	char *p_end =NULL; 

	if(ip == NULL||mask ==NULL||gw == NULL)
	{
		return -1;
	}
	p_start = strstr( (const char*)scr,(const char*)"ip=");
	if(p_start==NULL)
		return -1;
	p_start = p_start + strlen("ip=");
	p_end = strstr( p_start,"&mask=");
	memcpy(ip,p_start,p_end-p_start);

	p_start = strstr( p_start,"mask=");
	if(p_start==NULL)
		return -1;
	p_start = p_start + strlen("mask=");
	p_end = strstr( p_start,"&gw=");
	memcpy(mask,p_start,p_end-p_start);

	p_start = strstr( p_start,"gw=");
	if(p_start==NULL)
		return -1;
	p_start = p_start + strlen("gw=");
	memcpy(gw,p_start,strlen(p_start));

	return 0;

}

void reset_dhcp_ip_range(void)
{
#ifdef CONFIG_DHCPS
	int min = 0,max = 0;

	min = checkIpValuable(g_defparam.dhs_param.startip);
	max = checkIpValuable(g_defparam.dhs_param.endip);
	serial_printf("<min:%d><max:%d>\n",min,max);
	if(min < 0 || max < 0)
	{
		return;
	}
	set_dhcp_ip_range((unsigned char)min,(unsigned char)max);
	set_dhcp_leavetime(g_defparam.dhs_param.leave_time);
	memcpy(g_atParam.dhs_param.startip,g_defparam.dhs_param.startip,16);
	memcpy(g_atParam.dhs_param.endip,g_defparam.dhs_param.endip,16);
	g_atParam.dhs_param.leave_time = g_defparam.dhs_param.leave_time;
	save_at_param();
#endif
}

int save_at_param(void)
{
	config_set(&g_atParam);
	if(g_atParam.flags & CFG_SAVE)
		config_submit();
}


