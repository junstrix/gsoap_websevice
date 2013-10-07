/****************************************Copyright (c)*****************************************************
**--------------文件信息--------------------------------------------------------------------------------
** 文   件   名: stepmotor.h
** 描        述: 步进机驱动程序头文件.
**
**------------------------------------------------------------------------------------------------------
** 创建人: 陈俊彦
** 版  本: 0.0.1
** 日　期: 2011.04.31
********************************************************************************************************/
#define   StepMotor_1     1
#define   StepMotor_2     2
#define   StepMotor_3     3
#define   MOTOR_CW        0 
#define   MOTOR_CCW       1
#define   STEPMOTOR1_PIN  0xffffffb1
#define   STEPMOTOR2_PIN  0xfffff47f
#define   STEPMOTOR3_PIN  0xffffff0f
          
extern void StepmotorMovePitch(uint8_t, uint8_t);
extern void InitStep();