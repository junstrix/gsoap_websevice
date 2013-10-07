/*****************************************************************************
 *   w25x10.c: 	W25x10 C file for Winbond SPI Flash W25XX0 Series
 *	 			W25X10.C �ǻ���SPI�ӿڴ洢����ض�д�����ļ�
 *   
 *   History
 *   2010.01.21	I2C�ӿ�����, Magicoe
 *
******************************************************************************/
#include "LPC11xx.h"                        						// LPC11xx definitions 
#include "type.h"
#include "ssp.h"
#include "gpio.h"
#include "uart.h"
#include "w25x10.h"	

uint8_t w25srcaddr [SSP_BUFSIZE];									// ��������ݻ����� 
uint8_t w25destaddr[SSP_BUFSIZE];									// д������ݻ�����

/*****************************************************************************
** Function name:		W25X10_Init
** Descriptions:		W25X10��ʼ��
** parameters:			��
** Returned value:		1, ��ʼ���ɹ�; 0, ��ʼ��ʧ��
** History:				2010.01.22  Magicoe  ��ɻ�������
*****************************************************************************/
uint8_t W25X10_Init( void)
{
 	LPC_IOCON->PIO2_0 &= ~0x07;										// P0.2 -- GPIO, SSP Master Select

	GPIOSetDir  ( PORT2, 0, 1 );							  		// P0.2 -- GPIO, output
	GPIOSetValue( PORT2, 0, 1 );									// P0.2 -- 1, output

	SSP_IOConfig(1);												// ��ʼ��SSP0���IO��,SCK,MISO,MOSI
	SSP_Init(1);													// ��ʼ��SSP0
	return 1;														// ��ʼ���ɹ�
}

/*****************************************************************************
** Function name:		W25X10_RcvData
** Descriptions:		W25X10��������
** parameters:			��
** Returned value:		��
** History:				2010.01.22  Magicoe  ��ɻ�������
*****************************************************************************/
void W25X10_RcvData(uint8_t *buf, uint32_t length)
{
	//LPC_IOCON->JTAG_TCK_PIO0_10 &= ~0x07;							// P0.10 -- SSP CLK 
	//LPC_IOCON->JTAG_TCK_PIO0_10 |= 0x02;				
	SSP_Send( 1, buf, length ); 									// SSP ��������
	//LPC_IOCON->JTAG_TCK_PIO0_10 &= ~0x07;							// P0.10 -- JTAG TCK
}

/*****************************************************************************
** Function name:		W25X10_SendData
** Descriptions:		W25X10��������
** parameters:			��
** Returned value:		��
** History:				2010.01.22  Magicoe  ��ɻ�������
*****************************************************************************/
void W25X10_SendData(uint8_t *buf, uint32_t length)
{
	//LPC_IOCON->JTAG_TCK_PIO0_10 &= ~0x07;							// P0.10 -- SSP CLK 
	//LPC_IOCON->JTAG_TCK_PIO0_10 |= 0x02;
	SSP_Receive( 1, buf, length ); 									// SSP ��������
	//LPC_IOCON->JTAG_TCK_PIO0_10 &= ~0x07;							// P0.10 -- JTAG TCK
}

/*****************************************************************************
** Function name:		W25X10_CS0
** Descriptions:		W25X10 ����CS Ϊ0, ѡ��W25X10
** parameters:			��
** Returned value:		��
** History:				2010.01.22  Magicoe  ��ɻ�������
*****************************************************************************/
void W25X10_CS0( void)
{
	GPIOSetValue( PORT2, 0, 0 ); 									// P0.2 -- 0, CS = 0 ѡ��SPI Flash
}

/*****************************************************************************
** Function name:		W25X10_CS1
** Descriptions:		W25X10 ����CS Ϊ1, ��ѡ��W25X10
** parameters:			��
** Returned value:		��
** History:				2010.01.22  Magicoe  ��ɻ�������
*****************************************************************************/
void W25X10_CS1( void)
{
	GPIOSetValue( PORT2, 0, 1 );									// P0.2 -- 1, CS = 1 �ͷ�SPI Flash
}														 

/*****************************************************************************
** Function name:		W25X10_WEnable
** Descriptions:		W25X10 дʹ��
** parameters:			��
** Returned value:		��
** History:				2010.01.22  Magicoe  ��ɻ�������
*****************************************************************************/
void W25X10_WEnable( void)
{
	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 ѡ��SPI Flash
	w25srcaddr[0] = 0x06;											// Write Enable ����
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 1);					// Send Command
	W25X10_CS1();													// P0.2 -- 1, CS = 1 �ͷ�SPI Flash
}

/*****************************************************************************
** Function name:		W25X10_WDisable
** Descriptions:		W25X10 д����
** parameters:			��
** Returned value:		��
** History:				2010.01.22  Magicoe  ��ɻ�������
*****************************************************************************/
void W25X10_WDisable( void)
{
	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 ѡ��SPI Flash
	w25srcaddr[0] = 0x04;											// Write Enable ����
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 1);					// Send Command
	W25X10_CS1();													// P0.2 -- 1, CS = 1 �ͷ�SPI Flash
}

/*****************************************************************************
** Function name:		W25X10_ReadID
** Descriptions:		W25X10��ȡID�ź���
** parameters:			��
** Returned value:		W25X10 ID��
** History:				2010.01.22  Magicoe  ��ɻ�������
*****************************************************************************/
uint16_t W25X10_ReadID(void)
{
	uint16_t IDcode;

	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 ѡ��SPI Flash
	w25srcaddr[0] = 0x90;											// ����������     0x90
	w25srcaddr[1] = 0x00;											// ������ַ�ĸ�λ 0x00
	w25srcaddr[2] = 0x00;											// ������ַ�ĵ�λ 0x00 
	w25srcaddr[3] = 0x00;
	W25X10_RcvData ( (uint8_t *)&w25srcaddr[0],  4 ); 				// SSP ��������
	W25X10_SendData( (uint8_t *)&w25destaddr[0], 2 );				// SSP ��������
	W25X10_CS1();													// P0.2 -- 1, CS = 1 �ͷ�SPI Flash

	IDcode = (w25destaddr[0] << 8)|(w25destaddr[1]);				// �õ�IDcode

#if W25X10DEBUG
	printf("ID ManID %0x, DevID %0x\r\n",w25destaddr[0],w25destaddr[1]);
#endif
	return IDcode; 												    // ����ID��
}

/*****************************************************************************
** Function name:		W25X10_ReadStatus1
** Descriptions:		��ȡ״̬�Ĵ�����		
** parameters:			��
** Returned value:		״̬�Ĵ�����ֵ
** History:				2009.10.12  Magicoe  ��ɻ�������
**						2010.01.22  Magicoe  Porting to Linpo-PS-LPC1114
*****************************************************************************/
uint8_t W25X10_ReadStatus( void )
{
	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 ѡ��SPI Flash
	w25srcaddr[0] = 0x05;											// Read Reg 1 Command
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 1 );					// Send Command
	W25X10_SendData( (uint8_t *)&w25destaddr[0], 2 );				// Receive Reg 1's content
	W25X10_CS1();													// P0.2 -- 1, CS = 1 �ͷ�SPI Flash
	return w25destaddr[0];											// Return Reg 1's Content
}

/*****************************************************************************
** Function name:		W25X10_SetStatus
** Descriptions:		����״̬�Ĵ���		
** parameters:			״̬�Ĵ�����Ҫ���õ�ֵ
** Returned value:		����״̬�Ĵ���
** History:				2009.10.12  Magicoe  ��ɻ�������
**						2010.01.22  Magicoe  Porting to Linpo-PS-LPC1114
*****************************************************************************/
void W25X10_SetStatus( uint16_t SetReg )
{
	//int i;
	W25X10_WEnable();												// дʹ��
	//for(i=0; i<100; i++);											// ��ʱ

	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 ѡ��SPI Flash
	w25srcaddr[0] = 0x01;											// Write Status Register Command
	w25srcaddr[1] = SetReg;											// Status Register In 1st
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 2 );					// Send Command
	W25X10_CS1();													// P0.2 -- 1, CS = 1 �ͷ�SPI Flash
}

/*****************************************************************************
** Function name:		W25X10_RData
** Descriptions:		W25X10�����ݺ���
** parameters:			����ַRaddr����ȡ���ݳ���RLength
** Returned value:		0����ʧ�ܣ�1�����ɹ�
** History:				2009.10.12  Magicoe  ��ɻ�������
**						2010.01.22  Magicoe  Porting to Linpo-PS-LPC1114
*****************************************************************************/	
uint8_t W25X10_RData(uint32_t RAddr, uint8_t *buf, uint32_t RLength)
{
	uint8_t Temp, i;
	while(1)				// ���9��״̬�Ĵ���
	{														
		Temp = W25X10_ReadStatus( );	// ��ȥ״̬�Ĵ���1
		Temp &= 0x01;			// ���Busyλ
		if(Temp == 0x00)		// �����æ
			break;						
		for(i=0; i<10; i++);		
	}

	W25X10_CS0();				// P0.2 -- 0, CS = 0 ѡ��SPI Flash
	w25srcaddr[0] = 0x03;											// ������
	w25srcaddr[1] = (RAddr & 0xFF0000) >> 16;	// ��ַ24bit
	w25srcaddr[2] = (RAddr & 0x00FF00) >> 8;
	w25srcaddr[3] = (RAddr & 0x0000FF); 
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 4 );	// �������� 
	W25X10_SendData( (uint8_t *)&buf[0], RLength );	// ���ܶ���������
	W25X10_CS1();													// P0.2 -- 1, CS = 1 �ͷ�SPI Flash

	return 1;
}

/*****************************************************************************
** Function name:		W25X10_WData
** Descriptions:		W25Q64д���ݺ���
** parameters:			д���ַWAddr, д������buf, д�����ݳ���WLength
** Returned value:		0��д��ʧ�ܣ�1��д��ɹ�
** History:				2009.10.12  Magicoe  ��ɻ�������
**						2010.01.22  Magicoe  Porting to Linpo-PS-LPC1114
*****************************************************************************/							
uint8_t W25X10_WData(uint32_t WAddr, uint8_t *buf, uint32_t WLength)
{
	uint8_t Temp;
	uint32_t i;												
	
	while(1)														// ���9��״̬�Ĵ���
	{														
		Temp = W25X10_ReadStatus( );	// ��ȡ״̬�Ĵ���1
		Temp &= 0x01;			// ���Busyλ
		if(Temp == 0x00)		// �����æ
			break;													// ����for���
		//for(i=0; i<10; i++);  
	}							
															
	W25X10_WEnable();			// дʹ��

	while(1)														// ���9��״̬�Ĵ���
	{														
		//for(i=0; i<10; i++);
		Temp = W25X10_ReadStatus( );	// ��ȥ״̬�Ĵ���1
		Temp &= 0x03;			// ���Busyλ
		if(Temp == 0x02)		// ���дʹ��WEL = 1
			break;	
	}

	W25X10_CS0();				// P0.2 -- 0, CS = 0 ѡ��SPI Flash
	w25srcaddr[0]  = 0x02;			// ������Flashд���ݵ�����
	w25srcaddr[1]  = (WAddr & 0xFF0000) >> 16;// ����д��ַ�ĸ�8λ
	w25srcaddr[2]  = (WAddr & 0x00FF00) >> 8;	// ����д��ַ����8λ
	w25srcaddr[3]  = (WAddr & 0x0000FF); 		// ����д��ַ�ĵ�8λ

	for(i=0; i<WLength; i++)
	{
		w25srcaddr[4+i]  = buf[i]; 		// ����д��ַ�ĵ�8λ
	}


	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 4+WLength );		// ������Ҫд�������
	W25X10_CS1();							// P0.2 -- 1, CS = 1 �ͷ�SPI Flash

	return 1;
}

/*****************************************************************************
** Function name:		W25X10_Erase
** Descriptions:		W25X10��Ƭ��������
** parameters:			��
** Returned value:		��
** History:				2009.10.12  Magicoe  ��ɻ�������
**						2010.01.22  Magicoe  Porting to Linpo-PS-LPC1114
*****************************************************************************/	
uint8_t W25X10_Erase( void )
{
	uint8_t Temp;
	uint32_t i,j;

	while(1)														// ���9��״̬�Ĵ���
	{														
		Temp = W25X10_ReadStatus( );								// ��ȥ״̬�Ĵ���1
		Temp &= 0x01;												// ���Busyλ
		if(Temp == 0x00)											// �����æ
			break;													// ����for���
		for(i=0; i<10; i++);  
	}							
															
	W25X10_WEnable();												// дʹ��

	while(1)														// ���9��״̬�Ĵ���
	{														
		for(i=0; i<10; i++);
		Temp = W25X10_ReadStatus( );								// ��ȥ״̬�Ĵ���1
		Temp &= 0x03;												// ���Busyλ
		if(Temp == 0x02)											// ���дʹ��WEL = 1
			break;	
	}
	
	W25X10_CS0();				 									// P0.2 -- 0, CS = 0 ѡ��SPI Flash
	w25srcaddr[0] = 0xC7;
	W25X10_RcvData( (uint8_t *)&w25srcaddr[0], 1 );
 	W25X10_CS1();													// P0.2 -- 1, CS = 1 �ͷ�SPI Flash

	for(i=0; i<65000; i++);										    // ��ʱ
		for(j=0; j<30; j++);

#if W25X10DEBUG
	printf("Erase OK\r\n");
#endif
	return 1;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
  