/*****************************************************************************
 *   w25x10.h:  Header file for Winbond W25x10 SPI Flash 
 *
 *   Copyright(C) 2010, Linpo Shanghai, RD Department
 *   All rights reserved.
 *
 *   History
 *   2010.01.22  ver 1.00    ���вο��汾��Magicoe.Niu
 *
******************************************************************************/
#ifndef __W25X10_H__
#define __W25X10_H__

#define W25X10DEBUG     0																			// �Ƿ���Ҫ���ڴ�ӡ���¶�����
																	
extern uint8_t 		W25X10_Init					(void);												// ��ʼ��W25x10��ؽӿ� 
extern uint16_t 	W25X10_ReadID				(void);												// ��ȡID�ź���
extern uint8_t 		W25X10_ReadStatus			(void);												// ��ȡ״̬�Ĵ�����
extern void 		W25Q64_SetStatus			(uint16_t SetReg);									// ����״̬�Ĵ���
extern uint8_t 		W25X10_RData				(uint32_t RAddr, uint8_t *buf, uint32_t RLength);	// W25X10�����ݺ���
extern uint8_t		W25X10_WData				(uint32_t WAddr, uint8_t *buf, uint32_t WLength);	// W25X10д���ݺ���
extern uint8_t 		W25X10_Erase				(void);												// W25X10��Ƭ��������
#endif  /* __SSP_H__ */
/*****************************************************************************
**                            End Of File
******************************************************************************/

