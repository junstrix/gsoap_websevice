/*****************************************************************************
 *   can.c:  CAN C file for NXP LPC11xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.20  ver 1.00    Preliminary version, first Release
 *
*****************************************************************************/
#include "LPC11xx.h"			/* LPC11xx Peripheral Registers */
#include "type.h"
#include "gpio.h"
#include "can_api.h"

/*****************************************************************************
**      define variables 
*****************************************************************************/
static CAN_API **CanAPI = (CAN_API **)0x1fff1ff8UL;   // start address of can api rom.
static uint32_t s_CANBitClk[2] = {
                                0x05UL,
								0x00004c5dUL  
                                }; 
static volatile uint32_t ErrorInfo;				      // error information
static volatile uint8_t  CanTxFinished;               // can data send finished status
CAN_MSG_OBJ can_buff[MSG_OBJ_MAX];
uint32_t CANRxDone[MSG_OBJ_MAX]; 

 /*****************************************************************************
** Function name:		CAN_Init2
** Descriptions:		CAN clock, port initialization			
** parameters:			CANBitClk[in]: can baudrate
**                      enisr[in]: enable can interrupt
** Returned value:		None
*****************************************************************************/
void CAN_Init(uint8_t clkdiv, uint32_t CANBitClk, bool_t enisr)
{
	uint8_t i;
	static  CAN_MSG_OBJ    can_config;
    s_CANBitClk[0] = clkdiv;
	s_CANBitClk[1] = CANBitClk;
    (*CanAPI)->pCANAPI->init_can(s_CANBitClk);
	//	CAN_ConfigureMessages
	for (i = 0; i < MSG_OBJ_MAX; i++) 
	{
	    can_config.mode_id  = (CAN_MSGOBJ_EXT|0x2000);
	    can_config.mask     = 0x1FFFFFFF; // 
	    can_config.msgobj   = i;    
	    CAN_ConfigureMessages(&can_config);
	}
}

/*****************************************************************************
** Function name:		CAN_IRQHandler
** Descriptions:		Processing CAN interrupt
** parameters:			None
** Returned value:		None
*****************************************************************************/
void CAN_IRQHandler(void) 
{
    (*CanAPI)->pCANAPI->isr();
}

/*****************************************************************************
** Function name:		CAN_ConfigureMessages
** Descriptions:		Configure all the message buffers(32) that
**           			starting from message object one, the RX and TX 
**                      alternate. It's configured to support both standard
**						and extended frame type. 
** parameters:			msg_obj[in]: a pointer for can rx message object.
** Returned value:		TRUE/FALSE
*****************************************************************************/
bool_t CAN_ConfigureMessages(CAN_MSG_OBJ *msg_obj)
{
    if (msg_obj == NULL) return FALSE;
    (*CanAPI)->pCANAPI->config_rxmsgobj(msg_obj);
	return TRUE;
}

/*****************************************************************************
** Function name:		CAN_Receive
** Descriptions:        Reading messages that have been received by an 
**                      Rx message object. A pointer to a message object structure is 
**						passed to the receive function. 
**						Before calling, the number of the message object that is to be 
**						read has to be set in the structure.
** parameters:			[in]: msg_obj
** Returned value:		TRUE/FALSE
*****************************************************************************/
bool_t CAN_Receive(CAN_MSG_OBJ *msg_obj)
{
    if (msg_obj == NULL) return FALSE;
    (*CanAPI)->pCANAPI->can_receive(msg_obj);
	return TRUE;
}

/*****************************************************************************
** Function name:		CAN_Send
** Descriptions:		Send data to the CAN port
** parameters:			[in]: msg_obj
** Returned value:		TRUE/FALSE
*****************************************************************************/
bool_t CAN_Send(CAN_MSG_OBJ *msg_obj)
{
    if (msg_obj == NULL) return FALSE;
	(*CanAPI)->pCANAPI->can_transmit(msg_obj);
	return TRUE;
}

/*****************************************************************************
** Function name:		CAN_Config_calb
** Descriptions:		Config the CAN/CANOPEN callbacks
** parameters:			[in]: callback_cfg
** Returned value:		TRUE/FALSE
*****************************************************************************/
bool_t CAN_Config_calb(CAN_CALLBACKS *callback_cfg)
{
    if (callback_cfg == NULL) return FALSE;
	(*CanAPI)->pCANAPI->config_calb(callback_cfg);
	return TRUE;
}

/*****************************************************************************
** Function name:		CAN_rx
** Descriptions:		
** parameters:			[in]: msg_obj
** Returned value:		none
*****************************************************************************/
void CAN_recv(uint8_t msg_obj)
{
	can_buff[msg_obj].msgobj = msg_obj;
    CAN_Receive(&can_buff[msg_obj]);
	CANRxDone[msg_obj]= TRUE;
}

/*****************************************************************************
** Function name:		CAN_tx
** Descriptions:		
** parameters:			[in]: msg_obj
** Returned value:		none
*****************************************************************************/
void CAN_send(uint8_t msg_obj)
{
    CanTxFinished = msg_obj;
}

/*****************************************************************************
** Function name:		CAN_GetCanTxFinished
** Descriptions:		
** parameters:			none
** Returned value:		CanTxFinished
*****************************************************************************/
uint8_t CAN_GetCanTxFinished(void)
{
    return CanTxFinished;
}

/*****************************************************************************
** Function name:		CAN_SetCanTxFinished
** Descriptions:		
** parameters:			none
** Returned value:		none
*****************************************************************************/
void CAN_SetCanTxFinished(void)
{
    CanTxFinished = MSG_OBJ_MAX;
}

/*****************************************************************************
** Function name:		CAN_error
** Descriptions:		
** parameters:			[in]: error_info
** Returned value:		none
*****************************************************************************/
void CAN_Error(uint32_t error_info)
{
extern void InitCanBus(void);
    ErrorInfo = error_info;
	if (error_info != CAN_ERROR_NONE) {
	#ifdef DEBUG_UART
		NoteError(error_info);
	#endif
		InitCanBus();
	}
}

/******************************************************************************
**                            End Of File
******************************************************************************/

