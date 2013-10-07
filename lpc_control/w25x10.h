/*****************************************************************************
 *   w25x10.h:  Header file for Winbond W25x10 SPI Flash 
 *
 *   Copyright(C) 2010, Linpo Shanghai, RD Department
 *   All rights reserved.
 *
 *   History
 *   2010.01.22  ver 1.00    发行参考版本，Magicoe.Niu
 *
******************************************************************************/
#ifndef __W25X10_H__
#define __W25X10_H__

#define W25X10DEBUG     0																			// 是否需要串口打印出温度数据
																	
extern uint8_t 		W25X10_Init					(void);												// 初始化W25x10相关接口 
extern uint16_t 	W25X10_ReadID				(void);												// 读取ID号函数
extern uint8_t 		W25X10_ReadStatus			(void);												// 读取状态寄存器号
extern void 		W25Q64_SetStatus			(uint16_t SetReg);									// 设置状态寄存器
extern uint8_t 		W25X10_RData				(uint32_t RAddr, uint8_t *buf, uint32_t RLength);	// W25X10读数据函数
extern uint8_t		W25X10_WData				(uint32_t WAddr, uint8_t *buf, uint32_t WLength);	// W25X10写数据函数
extern uint8_t 		W25X10_Erase				(void);												// W25X10整片擦除函数
#endif  /* __SSP_H__ */
/*****************************************************************************
**                            End Of File
******************************************************************************/

