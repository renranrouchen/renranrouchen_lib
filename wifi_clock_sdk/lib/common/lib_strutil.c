/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage inc. all right reserved.                                           |
|                                                                              |
+=============================================================================*/
/*! 
*   \file  
*   \brief
*   \author Montage
*/

/*=============================================================================+
| Included Files							       |
+=============================================================================*/
#include <stdlib.h>
#if 0
#include <config.h>
#endif
#include <string.h>
#include <cmd.h>
#if 0
#include <str_util.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

int str2args (const char *str, char *argv[], char *delim, int max)
{
	char *p;
	int n;
	p = (char *) str;
	for (n=0; n < max; n++)
	{
		if (0==(argv[n]=strtok_r(p, delim, &p)))
			break;
	}
	return n;
}

#if 0
/*!-----------------------------------------------------------------------------
 * function: int str2argv(char *buf, int char **v_list, char c)
 *
 *      \brief 	convert string to argument array, ',' character was regarded as delimiter
 *		and '\n' character regarded as end of string. 
 *
 *	\param buf: input string
 *	\param list: output array pointer
 *	\param c: delimiter
 *
 *      \return number of arguments
 +----------------------------------------------------------------------------*/
int str2argv(char *buf, char **v_list, char c)
{
	int n;
	char del[4];

	del[0] = c;
	del[1] = 0;
	n=str2args(buf, v_list, del, MAX_ARGVS);
	v_list[n] = 0;
	return n;
}
#endif

char *str_arg(char **v_list, char *var_name)
{
	int i;
	for(i=0; i<MAX_ARGV; i++)
	{
		if(v_list[i] == 0)
			break;
		if(strncmp(v_list[i], var_name, strlen(var_name)) == 0)
			return (v_list[i]+strlen(var_name));
	}
	return NULL;
}

#if 0
/*!-----------------------------------------------------------------------------
 * function: int str2arglist(char *buf, int *list, char c, int max)
 *
 *      \brief 	convert string to argument array, ',' character was regarded as delimiter
 *							and '\n' character regarded as end of string. 
 *
 *			\param buf: input string
 *			\param list: output array pointer
 *			\param c: delimiter
 *			\param max: maximum number of sub-string
 *
 *      \return number of arguments
 +----------------------------------------------------------------------------*/
int str2arglist(char *buf, int *list, char c, int max)
{
	char *idx = buf;
	int j=0;
	
	list[j++] = (int)buf;
	while(*idx && j<max) {
		if(*idx == c || *idx == '\n') {
			*idx = 0;
			list[j++] = (int)(idx+1);
		}
		idx++;	
	}
	if(j==1 && !(*buf)) // No args
		j = 0;
		
	return j;
}

int hex2str(char *pszHex, int nHexLen, char *pszString)
{
        int i;
        char ch;
        for (i = 0; i < nHexLen; i++)
        {
                ch = (pszHex[i] & 0xF0) >> 4;
                pszString[i * 2] = (ch > 9) ? (ch + 0x61 - 0x0A) : (ch + 0x30);
                ch = pszHex[i] & 0x0F;
                pszString[i * 2 + 1] = (ch > 9) ? (ch + 0x61 - 0x0A) : (ch + 0x30);
        }
       pszString[i * 2] = 0;

        return 0;
}		



char *h323_instead_arg(char *rule, char *find_value, char *instead_value)
{
	char* result;
do {	
	result=strstr(rule,find_value);
	if(result != NULL)
		memcpy(result,instead_value,4);
} while(result!=NULL);
	return NULL;
}

char translate_iprange_from_str(char *ipstr, int ip_result[])
{
	struct in_addr addr;
	char *sec;

	if((sec = strchr(ipstr, '-')) != NULL)
	{
		*sec++ = '\0';
		if(inet_aton(ipstr, &addr))
			ip_result[0] = addr.s_addr;
		if(inet_aton(sec, &addr))
			ip_result[1] =  addr.s_addr;
	}
	else
	{
		if(inet_aton(ipstr, &addr))
			ip_result[0] = ip_result[1] = addr.s_addr;
	}

	/* illegal IP */
	if((ip_result[0] == 0) || (ip_result[1] == 0))
		return 0;
	return 1;
}

char translate_portrange_from_str(char *portstr, short port_result[])
{
	char *sec;

	if((sec = strchr(portstr, '-')) != NULL)
	{
		*sec++ = '\0';
		port_result[0] = atoi(portstr);
		port_result[1] = atoi(sec);
	}
	else
		port_result[0] = port_result[1] = atoi(portstr);
	/* illegal port */
	if((port_result[0] == 0) || (port_result[1] == 0))
		return 0;
	return 1;
}

char translate_sched_from_argv(char **argv, struct schedule *sched)
{
	char* tmpp;
	if(((tmpp = str_arg(argv, "schen=")) != NULL) && (atoi(tmpp) == 1))
	{
		char *etime;
		
		tmpp = str_arg(argv, "time=");
		etime = strchr(tmpp, '-');
		*etime++ = '\0';
		sched->time[0] = atoi(tmpp);
		sched->time[1] = atoi(etime);

		if((tmpp = str_arg(argv, "day=")) != NULL)
			sched->day = atoi(tmpp);
		else
			sched->day = 127;

		return 1;
	}
	return 0;
}

#endif
