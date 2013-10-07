/*****************************************************************************
 *   can.h:  Header file for NXP LPC11xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __CAN_H__
#define __CAN_H__

#include "type.h"
// control bits for CAN_MSG_OBJ.mode_id
#define   CAN_MSGOBJ_STD   0x00000000UL // CAN 2.0a 11-bit ID
#define   CAN_MSGOBJ_EXT   0x20000000UL // CAN 2.0b 29-bit ID
#define   CAN_MSGOBJ_DAT   0x00000000UL // data frame
#define   CAN_MSGOBJ_RTR   0x40000000UL // rtr frame

/* CAN CTRL register */
#define CTRL_INIT		(1 << 0)
#define CTRL_IE			(1 << 1) 
#define CTRL_SIE		(1 << 2)
#define CTRL_EIE		(1 << 3) 
#define CTRL_DAR		(1 << 5)
#define CTRL_CCE		(1 << 6) 
#define CTRL_TEST		(1 << 7)
	
/* Only below flag is set, it uses the default setting 
in the CAN bittiming configuration value. In this case, 
CAN clock should be 8MHz CAN clock bitrate should be 500kbps. */
#define USE_DEFAULT_BIT_TIMING    0

#define MSG_OBJ_MAX			      0x20
#define DLC_MAX				      8

/* BRP+1 = Fpclk/(CANBitRate * QUANTAValue)
   QUANTAValue = 1 + (Tseg1+1) + (Tseg2+1)
   QUANTA value varies based on the Fpclk and sample point
   e.g. (1) sample point is 87.5%, Fpclk is 48Mhz
   the QUANTA should be 16
        (2) sample point is 90%, Fpclk is 12.5Mhz
   the QUANTA should be 10 
   Fpclk = Fclk /APBDIV
   or
   BitRate = Fcclk/(APBDIV * (BRP+1) * ((Tseg1+1)+(Tseg2+1)+1))
*/ 	
/*
 * Bit Timing Values for 24MHz clk frequency
 */
/*    LPC_CAN->BT = 0x0101;	*/	/* 1Mbps with 8M sysclk */
/*    LPC_CAN->BT = 0x4501;	*/	/* 1Mbps with 24M sysclk */
/*    LPC_CAN->BT = 0x5801;	*/	/* 500kbps with 16M sysclk */
/*    LPC_CAN->BT = 0x5803;	*/	/* 250kbps with 16M sysclk */

//#define BITRATE20K8MHZ            0x00001431UL
//#define BITRATE20K8MHZ			  0x00001627UL			//80%
#define BITRATE20K8MHZ			  0x00000727UL			   //90%
#define BITRATE25K8MHZ            0x0000023fUL
#define BITRATE50K8MHZ            0x00000127UL
#define BITRATE100K8MHZ           0x00000113UL
#define BITRATE125K8MHZ           0x0000010FUL
#define BITRATE250K8MHZ           0x00000107UL
#define BITRATE500K8MHZ           0x00000103UL
#define BITRATE1000K8MHZ          0x00000101UL

#define BITRATE20K16MHZ			  0x0000074fUL			   //90%
#define BITRATE100K16MHZ          0x00005809UL
#define BITRATE125K16MHZ          0x00005807UL
#define BITRATE250K16MHZ          0x00005803UL
#define BITRATE500K16MHZ          0x00005801UL

#define BITRATE100K24MHZ          0x00007E09UL
#define BITRATE125K24MHZ          0x0000450FUL
#define BITRATE250K24MHZ          0x00004507UL
#define BITRATE500K24MHZ          0x00004503UL
#define BITRATE1000K24MHZ         0x00004501UL

#define BITRATE20K12MHZ			  0x00000931UL

// error status bits
#define CAN_ERROR_NONE            0x00000000UL
#define CAN_ERROR_PASS            0x00000001UL
#define CAN_ERROR_WARN            0x00000002UL
#define CAN_ERROR_BOFF            0x00000004UL
#define CAN_ERROR_STUF            0x00000008UL
#define CAN_ERROR_FORM            0x00000010UL
#define CAN_ERROR_ACK             0x00000020UL
#define CAN_ERROR_BIT1            0x00000040UL
#define CAN_ERROR_BIT0            0x00000080UL
#define CAN_ERROR_CRC             0x00000100UL

/* Data structure for CAN message */
typedef struct
{
    uint32_t	mode_id;
	uint32_t    mask; 
    uint8_t     data[8];
    uint8_t     dlc;
	uint8_t     msgobj;
} CAN_MSG_OBJ;

typedef struct {
    uint16_t  index;
    uint8_t   subindex;
    uint8_t   len;
    uint32_t  val;
} CAN_ODCONSTENTRY;

typedef struct {
    uint16_t  index;
    uint8_t   subindex;
    uint8_t   entrytype_len;
    uint8_t  *val;
} CAN_ODENTRY;

typedef struct {
    uint8_t       node_id;
    uint8_t       msgobj_rx;
    uint8_t       msgobj_tx;
    uint32_t      od_const_num;
    CAN_ODCONSTENTRY *od_const_table;
    uint32_t      od_num;
    CAN_ODENTRY  *od_table;
} CAN_CANOPENCFG;

typedef struct {
    void       (*CAN_rx)(uint8_t msg_obj);
    void       (*CAN_tx)(uint8_t msg_obj);
    void       (*CAN_error)(uint32_t error_info);
    uint32_t   (*CANOPEN_sdo_read)(uint16_t index, uint8_t subindex);
    uint32_t   (*CANOPEN_sdo_write)(uint16_t index, uint8_t subindex, uint8_t *dat_ptr);
    uint32_t   (*CANOPEN_sdo_seg_read)(uint16_t index, uint8_t subindex, uint8_t openclose, uint8_t *length, uint8_t *data, uint8_t *last);
    uint32_t   (*CANOPEN_sdo_seg_write)(uint16_t index, uint8_t subindex, uint8_t openclose, uint8_t length, uint8_t *data, uint8_t *fast_resp);
    uint8_t    (*CANOPEN_sdo_req)(uint8_t length_req, uint8_t *req_ptr, uint8_t *length_resp, uint8_t *resp_ptr);
} CAN_CALLBACKS;

typedef struct 
{
    void    (*init_can)(uint32_t *can_cfg);
	void    (*isr)(void);
	void    (*config_rxmsgobj)(CAN_MSG_OBJ *msg_obj);
	void    (*can_receive)(CAN_MSG_OBJ *msg_obj);
	void    (*can_transmit)(CAN_MSG_OBJ *msg_obj);
	void    (*config_canopen)(CAN_CANOPENCFG *canopen_cfg);
	void    (*canopen_handler)(void);
	void    (*config_calb)(CAN_CALLBACKS *callback_cfg);
} CAND;

typedef struct {
    void     *ptr;
	void     *ptr1;
    const    CAND *pCANAPI;
} CAN_API;
 
/* interfaces for can API */
void    CAN_Init(uint8_t clkdiv, uint32_t CANBitClk, bool_t enisr);
bool_t  CAN_ConfigureMessages(CAN_MSG_OBJ *msg_obj);
bool_t  CAN_Receive(CAN_MSG_OBJ *msg_obj);
bool_t  CAN_Send(CAN_MSG_OBJ *msg_obj);
bool_t  CAN_Config_calb(CAN_CALLBACKS *callback_cfg);

/* interfaces for can test */
uint8_t CAN_GetCanTxFinished(void);
void    CAN_SetCanTxFinished(void);

/* callback APIs */
void  CAN_recv(uint8_t msg_obj);
void  CAN_send(uint8_t msg_obj);
void  CAN_Error(uint32_t error_info);
uint32_t  CANOPEN_sdo_read(uint16_t index, uint8_t subindex);
uint32_t  CANOPEN_sdo_write(uint16_t index, uint8_t subindex, uint8_t *dat_ptr);
uint32_t  CANOPEN_sdo_seg_read(uint16_t index, uint8_t subindex, uint8_t openclose, uint8_t *length, uint8_t *data, uint8_t *last);
uint32_t  CANOPEN_sdo_seg_write(uint16_t index, uint8_t subindex, uint8_t openclose, uint8_t length, uint8_t *data, uint8_t *fast_resp);
uint8_t   CANOPEN_sdo_req(uint8_t length_req, uint8_t *req_ptr, uint8_t *length_resp, uint8_t *resp_ptr);
						
#endif  /* __CAN_H__ */
/*****************************************************************************
**                            End Of File
******************************************************************************/

