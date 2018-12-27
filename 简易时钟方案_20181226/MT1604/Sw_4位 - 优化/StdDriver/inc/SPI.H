
// #define SPI_INTERRUPT   1

#define  SPI_CK_SET( n ) (SPI0_CK_SE = n)                                     //SPI时钟设置函数 

/*******************************************************************************
* Function Name  : SPIMasterModeSet( UINT8 mode ) 
* Description    : SPI主机模式初始化
* Input          : UINT8 mode						 
* Output         : None
* Return         : None
*******************************************************************************/
void SPIMasterModeSet(UINT8 mode);

/*******************************************************************************
* Function Name  : FD814SPIInterruptInit()
* Description    : FD814SPI中断初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FD814SPIInterruptInit();

/*******************************************************************************
* Function Name  : FD814SPIMasterWrite(UINT8 dat)
* Description    : FD814硬件SPI写数据，主机模式
* Input          : UINT8 dat   数据
* Output         : None
* Return         : None
*******************************************************************************/
void FD814SPIMasterWrite(UINT8 dat);

/*******************************************************************************
* Function Name  : FD814SPIMasterRead( )
* Description    : FD814硬件SPI0读数据，主机模式
* Input          : None
* Output         : None
* Return         : UINT8 ret   
*******************************************************************************/
UINT8 FD814SPIMasterRead();

/*******************************************************************************
* Function Name  : SPISlvModeSet( ) 
* Description    : SPI从机模式初始化
* Input          : None						 
* Output         : None
* Return         : None
*******************************************************************************/
void SPISlvModeSet( );

/*******************************************************************************
* Function Name  : FD814SPISlvWrite(UINT8 dat)
* Description    : FD814硬件SPI写数据，从机模式
* Input          : UINT8 dat   数据
* Output         : None
* Return         : None
*******************************************************************************/
void FD814SPISlvWrite(UINT8 dat);

/*******************************************************************************
* Function Name  : FD814SPISlvRead( )
* Description    : FD814硬件SPI0读数据，从机模式
* Input          : None
* Output         : None
* Return         : UINT8 ret   
*******************************************************************************/
UINT8 FD814SPISlvRead();