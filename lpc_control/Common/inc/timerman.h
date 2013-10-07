/****************************************Copyright (c)*****************************************************
**--------------文件信息--------------------------------------------------------------------------------
** 文   件   名: timeman.h
** 描        述: 软定时器程序头文件.
**
**------------------------------------------------------------------------------------------------------
** 创建人: 
** 版  本:
** 日　期: 
** 描　述: 
********************************************************************************************************/

#define  MAX_TMR                  20
/* tick definition*/
#define   PERTICK                  1               /* 1 tick  = 10ms        */
#define   TICKS_SECOND             100*PERTICK  
#define   TICKS_MINUTE             60*TICKS_SECOND

/* attrib definition */
#define   _TICK                    1
#define   _SECOND                  TICKS_SECOND
#define   _MINUTE                  TICKS_MINUTE

/*
*******************************************************************************
* Function Name  : CreateTimer
* Description    : This function is to create a soft timer .
* Arguments      : [in] queue pointer
*                : [in] app tag
*                : [in] timer overtime hdl
* Return         : timer id
*******************************************************************************
*/
uint32_t CreateTimer(uint32_t tag, void (*tmrproc)(uint32_t));

/*
*******************************************************************************
* Function Name  : RemoveTimer
* Description    : This function is to remove a soft timer .
* Arguments      : [in] queue pointer
*                : [in] app tag
*                : [in] timer overtime hdl
* Return         : timer id
*******************************************************************************
*/
void RemoveTimer(uint32_t tmrid);

/*
*******************************************************************************
* Function Name  : StartTimer
* Description    : This function is to start a soft timer .
* Arguments      : [in] timer id 
*                : [in] attrib
*                : [in] interval
* Return         : none
*******************************************************************************
*/
void StartTimer(uint32_t tmrid,uint32_t attrib,uint32_t interval);

/*
*******************************************************************************
* Function Name  : StopTimer
* Description    : This function is to stop a soft timer .
* Arguments      : [in] timer id 
* Return         : none
*******************************************************************************
*/
void StopTimer(uint32_t tmrid);

/*
*******************************************************************************
* Function Name  : LeftTimer
* Description    : This function is to left time of a soft timer .
* Arguments      : [in] timer id 
* Return         : left time
*******************************************************************************
*/
uint32_t LeftTimer(uint32_t tmrid);

/*
*******************************************************************************
* Function Name  : TimerIsRun
* Description    : This function is to test timer is run.
* Arguments      : [in] timer id 
* Return         : TRUE or FALSE
*******************************************************************************
*/
uint8_t TimerIsRun(uint32_t tmrid);

/*
*******************************************************************************
* Function Name  : ExeOverTimeProc
* Description    : This function is to exe the overtime proc.
* Arguments      : [in] queue pointer
* Return         : none
*******************************************************************************
*/
void ExeOverTimeProc(void);

/*
*******************************************************************************
* Function Name  : InitTimerMan
* Description    : This function is to initialize the timer management module.
* Arguments      : None
* Return         : None
*******************************************************************************
*/
void InitTimerMan(void);
 
  /*
*******************************************************************************
* Function Name  : TimerEntry
* Description    : This function is the entry of timer management module.
* Arguments      : None
* Return         : None
*******************************************************************************
*/
void TimerEntry(void);

/*********END OF FILE******/