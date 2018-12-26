#ifndef _FD612DRV_DEMO_H_
#define	_FD612DRV_DEMO_H_

#include "FD612Drv_double.H"

void FD612_TOP_SCL_D_OUT();
void FD612_TOP_SDA_D_OUT();
void FD612_TOP_SDA_D_IN();
int FD612_TOP_SDA_IN();
void FD612_TOP_SCL_SET();
void FD612_TOP_SCL_CLR();
void FD612_TOP_SDA_SET();
void FD612_TOP_SDA_CLR();

void FD612_BASE_SCL_D_OUT();
void FD612_BASE_SDA_D_OUT();
void FD612_BASE_SDA_D_IN();
int FD612_BASE_SDA_IN();
void FD612_BASE_SCL_SET();
void FD612_BASE_SCL_CLR();
void FD612_BASE_SDA_SET();
void FD612_BASE_SDA_CLR();

void FD612_app_init();


void FD612_TEST(void);
#endif