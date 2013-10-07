/*****************************************************************************
 *   w25x10.c: 	W25x10 C file for Winbond SPI Flash W25XX0 Series
 *	 			W25X10.C 是华邦SPI接口存储器相关读写函数文件
 *   
 *   History
 *   2010.01.21	I2C接口例程, Magicoe
 *
******************************************************************************/
#include "LPC11xx.h"                        						// LPC11xx definitions 
#include "type.h"
#include "ssp.h"
#include "gpio.h"
#include "uart.h"
#include "w25x10.h"	

uint8_t w25srcaddr [SSP_BUFSIZE];									// 读命令及数据缓冲区 
uint8_t w25destaddr[SSP_BUFSIZE];									// 写命令及数据缓冲区

/*****************************************************************************
** Function name:		W25X10_Init
** Descriptions:		W25X10初始化
** parameters:			无
** Returned value:		1, 初始化成功; 0, 初始化失败
** History:				2010.01.22  Magicoe  完成基本功能
*****************************************************************************/
uint8_t W25X10_Init( void)
{
 	LPC_IOCON->PIO2_0 &= ~0x07;										// P0.2 -- GPIO, SSP Master Select

	GPIOSetDir  ( PORT2, 0, 1 );							  		// P0.2 -- GPIO, output
	GPIOSetValue( PORT2, 0, 1 );									// P0.2 -- 1, output

	SSP_IOConfig(1);												// 初始化SSP0相关IO口,SCK,MISO,MOSI
	SSP_Init(1);													// 初始化SSP0
	return 1;														// 初始化成功
}

/*****************************************************************************
** Function name:		W25X10_RcvData
** Descriptions:		W25X10接收数据
** parameters:			无
** Returned value:		无
** History:				2010.01.22  Magicoe  完成基本功能
*****************************************************************************/
void W25X10_RcvData(uint8_t *buf, uint32_t length)
{
	//LPC_IOCON->JTAG_TCK_PIO0_10 &= ~0x07;							// P0.10 -- SSP CLK 
	//LPC_IOCON->JTAG_TCK_PIO0_10 |= 0x02;				
	SSP_Send( 1, buf, length ); 									// SSP 发送数据
	//LPC_IOCON->JTAG_TCK_PIO0_10 &= ~0x07;							// P0.10 -- JTAG TCK
}

/*****************************************************************************
** Function name:		W25X10_SendData
** Descriptions:		W25X10发送数据
** parameters:			无
** Returned value:		无
** History:				2010.01.22  Magicoe  完成基本功能
*****************************************************************************/
void W25X10_SendData(uint8_t *buf, uint32_t length)
{
	//LPC_IOCON->JTAG_TCK_PIO0_10 &= ~0x07;							// P0.10 -- SSP CLK 
	//LPC_IOCON->JTAG_TCK_PIO0_10 |= 0x02;
	SSP_Receive( 1, buf, length ); 									// SSP 发送数据
	//LPC_IOCON->JTAG_TCK_PIO0_10 &= ~0x07;							// P0.10 -- JTAG TCK
}

/*****************************************************************************
** Function name:		W25X10_CS0
** Descriptions:		W25X10 设置CS 为0, 选中W25X10
** parameters:			无
** Returned value:		无
** History:				2010.01.22  Magicoe  完成基本功能
*****************************************************************************/
void W25X10_CS0( void)
{
	GPIOSetValue( PORT2, 0, 0 ); 									// P0.2 -- 0, CS = 0 选中SPI Flash
}

/*****************************************************************************
** Function name:		W25X10_CS1
** Descriptions:		W25X10 设置CS 为1, 不选中W25X10
** parameters:			无
** Returned value:		无
** History:				2010.01.22  Magicoe  完成基本功能
*****************************************************************************/
void W25X10_CS1( void)
{
	GPIOSetValue( PORT2, 0, 1 );									// P0.2 -- 1, CS = 1 释放SPI Flash
}														 

/*****************************************************************************
** Function name:		W25X10_WEnable
** Descriptions:		W25X10 写使能
** parameters:			无
** Returned value:		无
** History:				2010.01.22  Magicoe  完成基本功能
*****************************************************************************/
void W25X10_WEnable( void)
{
	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 选中SPI Flash
	w25srcaddr[0] = 0x06;											// Write Enable 命令
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 1);					// Send Command
	W25X10_CS1();													// P0.2 -- 1, CS = 1 释放SPI Flash
}

/*****************************************************************************
** Function name:		W25X10_WDisable
** Descriptions:		W25X10 写禁用
** parameters:			无
** Returned value:		无
** History:				2010.01.22  Magicoe  完成基本功能
*****************************************************************************/
void W25X10_WDisable( void)
{
	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 选中SPI Flash
	w25srcaddr[0] = 0x04;											// Write Enable 命令
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 1);					// Send Command
	W25X10_CS1();													// P0.2 -- 1, CS = 1 释放SPI Flash
}

/*****************************************************************************
** Function name:		W25X10_ReadID
** Descriptions:		W25X10读取ID号函数
** parameters:			无
** Returned value:		W25X10 ID号
** History:				2010.01.22  Magicoe  完成基本功能
*****************************************************************************/
uint16_t W25X10_ReadID(void)
{
	uint16_t IDcode;

	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 选中SPI Flash
	w25srcaddr[0] = 0x90;											// 读器件命令     0x90
	w25srcaddr[1] = 0x00;											// 器件地址的高位 0x00
	w25srcaddr[2] = 0x00;											// 器件地址的低位 0x00 
	w25srcaddr[3] = 0x00;
	W25X10_RcvData ( (uint8_t *)&w25srcaddr[0],  4 ); 				// SSP 发送数据
	W25X10_SendData( (uint8_t *)&w25destaddr[0], 2 );				// SSP 接收数据
	W25X10_CS1();													// P0.2 -- 1, CS = 1 释放SPI Flash

	IDcode = (w25destaddr[0] << 8)|(w25destaddr[1]);				// 得到IDcode

#if W25X10DEBUG
	printf("ID ManID %0x, DevID %0x\r\n",w25destaddr[0],w25destaddr[1]);
#endif
	return IDcode; 												    // 返回ID号
}

/*****************************************************************************
** Function name:		W25X10_ReadStatus1
** Descriptions:		读取状态寄存器号		
** parameters:			无
** Returned value:		状态寄存器的值
** History:				2009.10.12  Magicoe  完成基本功能
**						2010.01.22  Magicoe  Porting to Linpo-PS-LPC1114
*****************************************************************************/
uint8_t W25X10_ReadStatus( void )
{
	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 选中SPI Flash
	w25srcaddr[0] = 0x05;											// Read Reg 1 Command
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 1 );					// Send Command
	W25X10_SendData( (uint8_t *)&w25destaddr[0], 2 );				// Receive Reg 1's content
	W25X10_CS1();													// P0.2 -- 1, CS = 1 释放SPI Flash
	return w25destaddr[0];											// Return Reg 1's Content
}

/*****************************************************************************
** Function name:		W25X10_SetStatus
** Descriptions:		设置状态寄存器		
** parameters:			状态寄存器需要设置的值
** Returned value:		设置状态寄存器
** History:				2009.10.12  Magicoe  完成基本功能
**						2010.01.22  Magicoe  Porting to Linpo-PS-LPC1114
*****************************************************************************/
void W25X10_SetStatus( uint16_t SetReg )
{
	//int i;
	W25X10_WEnable();												// 写使能
	//for(i=0; i<100; i++);											// 延时

	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 选中SPI Flash
	w25srcaddr[0] = 0x01;											// Write Status Register Command
	w25srcaddr[1] = SetReg;											// Status Register In 1st
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 2 );					// Send Command
	W25X10_CS1();													// P0.2 -- 1, CS = 1 释放SPI Flash
}

/*****************************************************************************
** Function name:		W25X10_RData
** Descriptions:		W25X10读数据函数
** parameters:			读地址Raddr，读取数据长度RLength
** Returned value:		0，读失败；1，读成功
** History:				2009.10.12  Magicoe  完成基本功能
**						2010.01.22  Magicoe  Porting to Linpo-PS-LPC1114
*****************************************************************************/	
uint8_t W25X10_RData(uint32_t RAddr, uint8_t *buf, uint32_t RLength)
{
	uint8_t Temp, i;
	while(1)				// 检查9次状态寄存器
	{														
		Temp = W25X10_ReadStatus( );	// 读去状态寄存器1
		Temp &= 0x01;			// 检查Busy位
		if(Temp == 0x00)		// 如果不忙
			break;						
		for(i=0; i<10; i++);		
	}

	W25X10_CS0();				// P0.2 -- 0, CS = 0 选中SPI Flash
	w25srcaddr[0] = 0x03;											// 读命令
	w25srcaddr[1] = (RAddr & 0xFF0000) >> 16;	// 地址24bit
	w25srcaddr[2] = (RAddr & 0x00FF00) >> 8;
	w25srcaddr[3] = (RAddr & 0x0000FF); 
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 4 );	// 发送命令 
	W25X10_SendData( (uint8_t *)&buf[0], RLength );	// 接受读到的数据
	W25X10_CS1();													// P0.2 -- 1, CS = 1 释放SPI Flash

	return 1;
}

/*****************************************************************************
** Function name:		W25X10_WData
** Descriptions:		W25Q64写数据函数
** parameters:			写入地址WAddr, 写入数据buf, 写入数据长度WLength
** Returned value:		0，写入失败；1，写入成功
** History:				2009.10.12  Magicoe  完成基本功能
**						2010.01.22  Magicoe  Porting to Linpo-PS-LPC1114
*****************************************************************************/							
uint8_t W25X10_WData(uint32_t WAddr, uint8_t *buf, uint32_t WLength)
{
	uint8_t Temp;
	uint32_t i;												
	
	while(1)														// 检查9次状态寄存器
	{														
		Temp = W25X10_ReadStatus( );	// 读取状态寄存器1
		Temp &= 0x01;			// 检查Busy位
		if(Temp == 0x00)		// 如果不忙
			break;													// 跳出for语句
		//for(i=0; i<10; i++);  
	}							
															
	W25X10_WEnable();			// 写使能

	while(1)														// 检查9次状态寄存器
	{														
		//for(i=0; i<10; i++);
		Temp = W25X10_ReadStatus( );	// 读去状态寄存器1
		Temp &= 0x03;			// 检查Busy位
		if(Temp == 0x02)		// 如果写使能WEL = 1
			break;	
	}

	W25X10_CS0();				// P0.2 -- 0, CS = 0 选中SPI Flash
	w25srcaddr[0]  = 0x02;			// 发送向Flash写数据的命令
	w25srcaddr[1]  = (WAddr & 0xFF0000) >> 16;// 发送写地址的高8位
	w25srcaddr[2]  = (WAddr & 0x00FF00) >> 8;	// 发送写地址的中8位
	w25srcaddr[3]  = (WAddr & 0x0000FF); 		// 发送写地址的低8位

	for(i=0; i<WLength; i++)
	{
		w25srcaddr[4+i]  = buf[i]; 		// 发送写地址的低8位
	}


	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 4+WLength );		// 发送需要写入的数据
	W25X10_CS1();							// P0.2 -- 1, CS = 1 释放SPI Flash

	return 1;
}

/*****************************************************************************
** Function name:		W25X10_Erase
** Descriptions:		W25X10整片擦出命令
** parameters:			无
** Returned value:		无
** History:				2009.10.12  Magicoe  完成基本功能
**						2010.01.22  Magicoe  Porting to Linpo-PS-LPC1114
*****************************************************************************/	
uint8_t W25X10_Erase( void )
{
	uint8_t Temp;
	uint32_t i,j;

	while(1)														// 检查9次状态寄存器
	{														
		Temp = W25X10_ReadStatus( );								// 读去状态寄存器1
		Temp &= 0x01;												// 检查Busy位
		if(Temp == 0x00)											// 如果不忙
			break;													// 跳出for语句
		for(i=0; i<10; i++);  
	}							
															
	W25X10_WEnable();												// 写使能

	while(1)														// 检查9次状态寄存器
	{														
		for(i=0; i<10; i++);
		Temp = W25X10_ReadStatus( );								// 读去状态寄存器1
		Temp &= 0x03;												// 检查Busy位
		if(Temp == 0x02)											// 如果写使能WEL = 1
			break;	
	}
	
	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 选中SPI Flash
	w25srcaddr[0] = 0xC7;
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 1 );
 	W25X10_CS1();													// P0.2 -- 1, CS = 1 释放SPI Flash

	for(i=0; i<65000; i++);										    // 延时
		for(j=0; j<30; j++);

#if W25X10DEBUG
	printf("Erase OK\r\n");
#endif
	return 1;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
  