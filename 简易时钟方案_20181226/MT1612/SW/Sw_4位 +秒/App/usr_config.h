#ifndef	__USR_CONFIG_H__
#define __USR_CONFIG_H__

#include "usr.h"
#include "FD814.H" 


void read_single_ds1302(UINT8 addr,UINT8	time);
void  write_single_ds1302(UINT8 addr,UINT8	time);

#endif