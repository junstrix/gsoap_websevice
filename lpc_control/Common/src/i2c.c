/*****************************************************************************
 *   i2c.c:  I2C C file for NXP LPC11xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.19  ver 1.00    Preliminary version, first Release
 *
*****************************************************************************/
#include "LPC11xx.h"			/* LPC11xx Peripheral Registers */
#include "type.h"
#include "i2c.h"

volatile uint32_t I2CMasterState = I2C_IDLE;
volatile uint32_t I2CSlaveState = I2C_IDLE;

volatile uint32_t I2CMode;

volatile uint8_t I2CMasterBuffer[I2C_BUFSIZE];
volatile uint8_t I2CSlaveBuffer[I2C_BUFSIZE];
volatile uint32_t I2CCount = 0;
volatile uint32_t I2CReadLength;
volatile uint32_t I2CWriteLength;

volatile uint32_t RdIndex = 0;
volatile uint32_t WrIndex = 0;

/* 
From device to device, the I2C communication protocol may vary, 
in the example below, the protocol uses repeated start to read data from or 
write to the device:
For master read: the sequence is: STA,Addr(W),offset,RE-STA,Addr(r),data...STO 
for master write: the sequence is: STA,Addr(W),offset,RE-STA,Addr(w),data...STO
Thus, in state 8, the address is always WRITE. in state 10, the address could 
be READ or WRITE depending on the I2C command.
*/   

/*****************************************************************************
** Function name:		I2C0_IRQHandler
**
** Descriptions:		I2C interrupt handler, deal with master mode only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
#pragma diag_suppress=Pa082
void I2C0_IRQHandler(void) 
{
  uint8_t StatValue;

  /* this handler deals with master read and master write only */
  StatValue = LPC_I2C->STAT;
  switch ( StatValue )
  {
	case 0x08:			/* A Start condition is issued. */
	WrIndex = 0;
	LPC_I2C->DAT = I2CMasterBuffer[WrIndex++];
	LPC_I2C->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	I2CMasterState = I2C_STARTED;
	break;
	
	case 0x10:			/* A repeated started is issued */
	RdIndex = 0;
	/* Send SLA with R bit set, */
	LPC_I2C->DAT = I2CMasterBuffer[WrIndex++];
	LPC_I2C->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	I2CMasterState = I2C_RESTARTED;
	break;
	
	case 0x18:			/* Regardless, it's a ACK */
	if ( I2CMasterState == I2C_STARTED )
	{
	  LPC_I2C->DAT = I2CMasterBuffer[WrIndex++];
	  I2CMasterState = DATA_ACK;
	}
	LPC_I2C->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
	case 0x30:
	if ( WrIndex < I2CWriteLength )
	{   
	  LPC_I2C->DAT = I2CMasterBuffer[WrIndex++]; /* this should be the last one */
	  I2CMasterState = DATA_ACK;
	}
	else
	{
	  if ( I2CReadLength != 0 )
	  {
		LPC_I2C->CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
		I2CMasterState = I2C_REPEATED_START;
	  }
	  else
	  {
		I2CMasterState = DATA_NACK;
		LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */
	  }
	}
	LPC_I2C->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x40:	/* Master Receive, SLA_R has been sent */
	if ( I2CReadLength == 1 )
	{
	  /* Will go to State 0x58 */
	  LPC_I2C->CONCLR = I2CONCLR_AAC;	/* assert NACK after data is received */
	}
	else
	{
	  /* Will go to State 0x50 */
	  LPC_I2C->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	}
	LPC_I2C->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
	I2CSlaveBuffer[RdIndex++] = LPC_I2C->DAT;
	if ( RdIndex < I2CReadLength )
	{   
	  I2CMasterState = DATA_ACK;
	  LPC_I2C->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	}
	else
	{
	  I2CMasterState = DATA_NACK;
	  LPC_I2C->CONCLR = I2CONCLR_AAC;	/* assert NACK on last byte */
	}
	LPC_I2C->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x58:
	I2CSlaveBuffer[RdIndex++] = LPC_I2C->DAT;
	I2CMasterState = DATA_NACK;
	LPC_I2C->CONSET = I2CONSET_STO;	/* Set Stop flag */ 
	LPC_I2C->CONCLR = I2CONCLR_SIC;	/* Clear SI flag */
	break;

	case 0x20:		/* regardless, it's a NACK */
	case 0x48:
	LPC_I2C->CONCLR = I2CONCLR_SIC;
	I2CMasterState = DATA_NACK;
	break;
	
	case 0x38:		/* Arbitration lost, in this example, we don't
					deal with multiple master situation */
	default:
	LPC_I2C->CONCLR = I2CONCLR_SIC;	
	break;
  }
  return;
}
#pragma diag_default=Pa082
/*****************************************************************************
** Function name:		I2CStart
**
** Descriptions:		Create I2C start condition, a timeout
**				value is set if the I2C never gets started,
**				and timed out. It's a fatal error. 
**
** parameters:			None
** Returned value:		true or false, return false if timed out
** 
*****************************************************************************/
uint32_t I2CStart( void )
{
  uint32_t timeout = 0;
  uint32_t retVal = FALSE;
 
  /*--- Issue a start condition ---*/
  LPC_I2C->CONSET = I2CONSET_STA;	/* Set Start flag */
    
  /*--- Wait until START transmitted ---*/
  while( 1 )
  {
	if ( I2CMasterState == I2C_STARTED )
	{
	  retVal = TRUE;
	  break;	
	}
	if ( timeout >= MAX_TIMEOUT )
	{
	  retVal = FALSE;
	  break;
	}
	timeout++;
  }
  return( retVal );
}

/*****************************************************************************
** Function name:		I2CStop
**
** Descriptions:		Set the I2C stop condition, if the routine
**				never exit, it's a fatal bus error.
**
** parameters:			None
** Returned value:		true or never return
** 
*****************************************************************************/
uint32_t I2CStop( void )
{
  LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */ 
  LPC_I2C->CONCLR = I2CONCLR_SIC;  /* Clear SI flag */ 
            
  /*--- Wait for STOP detected ---*/
  while( LPC_I2C->CONSET & I2CONSET_STO );
  return TRUE;
}

/*****************************************************************************
** Function name:		I2CInit
**
** Descriptions:		Initialize I2C controller
**
** parameters:			I2c mode is either MASTER or SLAVE
** Returned value:		true or false, return false if the I2C
**				interrupt handler was not installed correctly
** 
*****************************************************************************/
uint32_t I2CInit( uint32_t I2cMode ) 
{
  /* It seems to be bit0 is for I2C, different from
  UM. To be retested along with SSP reset. SSP and I2C
  reset are overlapped, a known bug, for now, both SSP 
  and I2C use bit 0 for reset enable. Once the problem
  is fixed, change to "#if 1". */
#if 1
  LPC_SYSCON->PRESETCTRL |= (0x1<<1);
#else
  LPC_SYSCON->PRESETCTRL |= (0x1<<0);
#endif
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<5);
  LPC_IOCON->PIO0_4 &= ~0x3F;	/*  I2C I/O config */
  LPC_IOCON->PIO0_4 |= 0x01;		/* I2C SCL */
  LPC_IOCON->PIO0_5 &= ~0x3F;	
  LPC_IOCON->PIO0_5 |= 0x01;		/* I2C SDA */
  LPC_IOCON->PIO0_4 |= (0x1<<10);	/* open drain pins */
  LPC_IOCON->PIO0_5 |= (0x1<<10);	/* open drain pins */

  /*--- Clear flags ---*/
  LPC_I2C->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    

  /*--- Reset registers ---*/
#if FAST_MODE_PLUS
  LPC_IOCON->PIO0_4 |= (0x2<<8);
  LPC_IOCON->PIO0_5 |= (0x2<<8);
  LPC_I2C->SCLL   = I2SCLL_HS_SCLL;
  LPC_I2C->SCLH   = I2SCLH_HS_SCLH;
#else
  LPC_I2C->SCLL   = I2SCLL_SCLL;
  LPC_I2C->SCLH   = I2SCLH_SCLH;
#endif

  if ( I2cMode == I2CSLAVE )
  {
	LPC_I2C->ADR0 = PCF8594_ADDR;
  }    

  /* Enable the I2C Interrupt */
  NVIC_EnableIRQ(I2C_IRQn);

  LPC_I2C->CONSET = I2CONSET_I2EN;
  return( TRUE );
}

/*****************************************************************************
** Function name:		I2CEngine
**
** Descriptions:		The routine to complete a I2C transaction
**				from start to stop. All the intermitten
**				steps are handled in the interrupt handler.
**				Before this routine is called, the read
**				length, write length, I2C master buffer,
**				and I2C command fields need to be filled.
**				see i2cmst.c for more details. 
**
** parameters:			None
** Returned value:		true or false, return false only if the
**				start condition can never be generated and
**				timed out. 
** 
*****************************************************************************/
uint32_t I2CEngine( void )    //I2CEngine(void)是I2C的运行调度程序，在书中有介绍到。
{
  I2CMasterState = I2C_IDLE;  //I2CMasterState是I2C的状态，I2CMASTERSTATE=I2C_IDLE是指I2C处于空闲状态，从字面上就可以看出。
  RdIndex = 0;
  WrIndex = 0;
  if ( I2CStart() != TRUE )   //当I2CStart()返回非TRUE时，I2C处于空闲状态，否则为工作状态
  {
	I2CStop();          // I2C Stop
	return ( FALSE );  //直接返回FALSE，跳出I2CEngine()
  }

  while ( 1 )             //I2C处于工作状态
  {
	if ( I2CMasterState == DATA_NACK )//数据传输完成，停止I2C的运作
	{
	  I2CStop();     // I2C Stop
	  break;
	}
  }    
  return ( TRUE );      
}

/******************************************************************************
**                            End Of File
******************************************************************************/

