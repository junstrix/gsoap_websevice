/*******************************Copyright (c)**********************************
**--------------文件信息-------------------------------------------------------
** 文   件   名: key.c
** 描        述: 矩阵键盘驱动程序.
**
-------------------------------------------------------------------------------
** 制作人: 张全君
** 修改人: chen
** 版  本: 0.0.2
** 日　期: 2012.07.01
******************************************************************************/
#include "LPC11xx.h"                                            // LPC11xx definitions 
#include "type.h"
#include "uart.h"
#include "GPIO.h"
#include "key.h"

volatile unsigned char  KeyBufIndex = 0;
volatile unsigned char  KeyBuf[KeyBufSize];                            //串口数据缓存     
volatile unsigned char  KeyCnt;
static unsigned char  table[9]={0X31,0X32,0X33,0X34,0X35,0X36,0X37,0X38,0X39};  //按键返回值

/* 取按键返回值 */
unsigned char display(int i)  
{
  return table[i];
}

/* 推送数据入缓存 */
void KeyValuePush(unsigned char keyValue)
{
  if(KeyBufIndex < KeyBufSize )
    KeyBuf[KeyBufIndex++] = keyValue;	
}

/*  按键初始化 */
void KeyInit(void)
{
        LPC_IOCON->JTAG_TMS_PIO1_0 &= ~0x07;									// P1.0  GPIO 		keycol1
        LPC_IOCON->JTAG_TMS_PIO1_0 |= 0x01;
	LPC_IOCON->JTAG_TDO_PIO1_1 &= ~0x07;									// P1.1  GPIO		keycol2
        LPC_IOCON->JTAG_TDO_PIO1_1 |= 0x01;
	LPC_IOCON->JTAG_nTRST_PIO1_2 &= ~0x07;									// P1.2  GPIO		keycol3
        LPC_IOCON->JTAG_nTRST_PIO1_2 |= 0x01;
	LPC_IOCON->PIO3_0 &= ~0x07;									// P3.0  GPIO		keyrow1
	LPC_IOCON->PIO3_1 &= ~0x07;									// P3.1  GPIO		keyrow2
	LPC_IOCON->PIO3_2 &= ~0x07;									// P3.2  GPIO		keyrow3
}

/*  读按键值 */
unsigned char ReadKeyValue(void)
{
  char temp1;
 /*设置行、列数据方向，并设置列线初始值*/
  LPC_GPIO1->DIR &= 0xFFFFFFF8; 
  LPC_GPIO3->DIR |= 0x00000007; 
  /* 行置零 */
  LPC_GPIO3->DATA &= 0xFFFFFFF8;
  
 if(GPIOReadValue(1,0) == 0)
   temp1 = 1;
 if(GPIOReadValue(1,1) == 0)
   temp1 = 2;
 if(GPIOReadValue(1,2) == 0)
   temp1 = 4;
 
  /*设置行、列数据方向，并设置列线初始值*/
  LPC_GPIO1->DIR |= 0x00000007;
  LPC_GPIO3->DIR &= 0xFFFFFFF8;
  /* 列置零 */ 
  LPC_GPIO1->DATA &= 0xFFFFFFF8;
  
  /* 以下为判断按键位置 */
 if(GPIOReadValue(3,0) == 0)
  {
    switch( temp1 )
    {
    case 1:  
      return display(8);  //未加break;是因为执行不到
  
    case 2:  
      return display(7);
 
    case 4:  
      return display(6);
    
    default: 
    break;
    }
  }
  if(GPIOReadValue(3,1) == 0)
  {
    switch( temp1 )
    {
    case 1:  
      return display(5);
    
    case 2:  
      return display(4);
    
    case 4:  
      return display(3);
    
    default: 
    break;
    }
  }
  if(GPIOReadValue(3,2) == 0)
  {
    switch( temp1 )
    {
    case 1:  
      return display(2);
   
    case 2:  
      return display(1);
    
    case 4:  
      return display(0);
   
    default: 
    break;
    }
  }
   else
    return KEY_NONE;
}

/* 按键扫描 */
void KeyScan(void)
{
	static unsigned char KeyState = KEY_PROCESS_PRESS_DETECT;  //初始按键状态
	static unsigned char KeyCur;
	switch(KeyState)
	{
	case KEY_PROCESS_PRESS_DETECT:
		KeyCur = ReadKeyValue();        //第一次读按键值
		if(KeyCur != KEY_NONE)
		{
			 KeyState = KEY_PROCESS_PRESS_CONFIRM;
			 KeyCnt = 0;		
		}
                else
                {
                  KeyState = KEY_PROCESS_PRESS_DETECT;
                }
	break;

	case KEY_PROCESS_PRESS_CONFIRM:
		if(ReadKeyValue() == KeyCur)   //第二次读按键值并与第一次对比
		{
			KeyCnt ++;
			if(KeyCnt == 2) KeyState = KEY_PROCESS_PRESS_SURE;
		}
		else
		{
			KeyState = KEY_PROCESS_PRESS_DETECT;
		}
	break;

	case KEY_PROCESS_PRESS_SURE:       //按键确认             
		 KeyState = KEY_PROCESS_RELEASE_CONFIRM;
	break;

	case KEY_PROCESS_RELEASE_CONFIRM:  //按键释放
		 if(ReadKeyValue() == KeyCur)
		 {
                  KeyState = KEY_PROCESS_PRESS_SURE;     
		 }
		 else
		 {
		 	KeyState = KEY_PROCESS_RELEASE_SURE;
			KeyCnt = 0;
		 }
	break;

	case KEY_PROCESS_RELEASE_SURE:   //按键释放确认
		if(ReadKeyValue() == KeyCur)
		{
			KeyState = KEY_PROCESS_RELEASE_CONFIRM;
		}
		else
		{
			KeyCnt ++;
			if(KeyCnt == 2)
			{                        
                          KeyValuePush(KeyCur);  //保存按键值
  			  KeyState = KEY_PROCESS_PRESS_DETECT;
			}
		}
	break;
	
	default:
	break;
	}
}

/* 从缓冲区弹出数据 */
unsigned char KeyValuePop(void)  
{
	unsigned char keyValue,i;
	if(KeyBufIndex == 0) return KEY_NONE;
	keyValue = KeyBuf[0];
	for(i = 0;i < KeyBufIndex - 1; i++)
	KeyBuf[i] = KeyBuf[i+1];
	KeyBufIndex --;
	return keyValue;
}


/* 文件结束  ------------------------------------------------------------------	*/