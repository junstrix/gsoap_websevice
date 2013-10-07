/****************************************Copyright (c)*****************************************************
**--------------文件信息--------------------------------------------------------------------------------
** 文   件   名: stepmotor.c
** 描        述: 步进机驱动程序.
**
**------------------------------------------------------------------------------------------------------
** 创建人: 陈俊彦
** 版  本: 0.0.1
** 日　期: 2011.04.31
********************************************************************************************************/

#include "LPC11xx.h"
#include "stepmotor.h"
#include "gpio.h"
#include "timer16.h"

/*****************************************************************************
**     定义变量
******************************************************************************/
uint32_t step1_ccw[8] = {0x02,0x06,0x04,0x0c,0x08,0x48,0x40,0x42};
uint32_t step1_cw[8] = {0x42,0x40,0x48,0x08,0x0c,0x04,0x06,0x02};
uint32_t step2_ccw[8] = {0x80,0x180,0x100,0x300,0x200,0xa00,0x800,0x880};
uint32_t step2_cw[8] = {0x880,0x800,0xa00,0x200,0x300,0x100,0x180,0x80};
uint32_t step3_ccw[8] = {0x10,0x30,0x20,0x60,0x40,0xc0,0x80,0x90};
uint32_t step3_cw[8] = {0x90,0x80,0xc0,0x40,0x60,0x20,0x30,0x10};
/******************************************************************************
**   初始化步进机引脚
******************************************************************************/
void InitStep()
{
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
  LPC_IOCON->JTAG_TDI_PIO0_11 &= ~(0x07);           //设置P0.11为GPIO
  LPC_IOCON->JTAG_TDI_PIO0_11 |= 0x01;

  LPC_GPIO0->DIR &= ~(0xbce);                       //设置GPIO为输出
  LPC_GPIO2->DIR &= ~(0xf0);
  LPC_GPIO0->DIR |= 0xbce;
  LPC_GPIO2->DIR |= 0xf0;

  init_timer16( 1, 0 );                             //初始化16位定时器1
}
/*****************************************************************************
**  步进机转动一个齿距
**  stepNum为步进机编号
**  stepDir为步进机转动方向
*****************************************************************************/
void StepmotorMovePitch(uint8_t stepNum, uint8_t StepDir)     //步进机转动一个齿距
{
  uint8_t i, j;
  
  for( j=0; j<8; j++ )
  {
    if( StepDir == MOTOR_CCW )                              //逆时针转动
    {
      if( stepNum == StepMotor_1 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO0->DATA &= STEPMOTOR1_PIN;
          LPC_GPIO0->DATA |= step1_ccw[i];
          delayMs(1,1);
        }
      }
      else if( stepNum == StepMotor_2 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO0->DATA &= STEPMOTOR2_PIN;
          LPC_GPIO0->DATA |= step2_ccw[i];
          delayMs(1,1);
        }    
      }
      else if( stepNum == StepMotor_3 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO2->DATA &= STEPMOTOR3_PIN;
          LPC_GPIO2->DATA |= step3_ccw[i];
          delayMs(1,1);
        }    
      }
    }
    else if( StepDir == MOTOR_CW )                           //顺时针转动
    {
      if( stepNum == StepMotor_1 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO0->DATA &= STEPMOTOR1_PIN;
          LPC_GPIO0->DATA |= step1_cw[i];
        delayMs(1,1);
        }
      }
      else if( stepNum == StepMotor_2 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO0->DATA &= STEPMOTOR2_PIN;
          LPC_GPIO0->DATA |= step2_cw[i];
            delayMs(1,1);
        }    
      }
      else if( stepNum == StepMotor_3 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO2->DATA &= STEPMOTOR3_PIN;
          LPC_GPIO2->DATA |= step3_cw[i];
           delayMs(1,1);
        }    
      }
    }
  }    
  return;
}
