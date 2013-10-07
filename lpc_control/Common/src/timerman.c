/****************************************Copyright (c)*****************************************************
**--------------文件信息--------------------------------------------------------------------------------
** 文   件   名: timeman.c
** 描        述: 软定时器程序.
**
**------------------------------------------------------------------------------------------------------
** 创建人: 
** 版  本:
** 日　期: 
** 描　述: 
********************************************************************************************************/
#include "LPC11xx.h"
#include "timerman.h"
#include "type.h"

/* timer structure definition */
typedef struct 
{
    uint8_t      ready;
    uint32_t     tag;
    uint32_t     time;
    uint32_t     interval;
    void       (*tmrproc)(uint32_t);
} TIMER_T;

/* Private variables ---------------------------------------------------------*/

extern volatile uint32_t           systicks;
static   uint32_t                  preticks;
static   TIMER_T                   timer[MAX_TMR];


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
uint32_t CreateTimer(uint32_t tag, void (*tmrproc)(uint32_t))
{
  uint32_t tmrid;
	
  for (tmrid =0; tmrid<MAX_TMR; tmrid++) 
  {
    if (timer[tmrid].tmrproc == NULL) break;         
  }
	  
  timer[tmrid].ready     = FALSE;
  timer[tmrid].time      = 0;
  timer[tmrid].interval  = 0;
  timer[tmrid].tag       = tag;
  timer[tmrid].tmrproc   = tmrproc;
	  	  
  return tmrid;
}

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
void RemoveTimer(uint32_t tmrid)
{ 
  timer[tmrid].ready     = FALSE;
  timer[tmrid].tmrproc   = NULL;
}

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
void StartTimer(uint32_t tmrid, uint32_t attrib,uint32_t interval)
{
  timer[tmrid].time     = attrib * interval;
  timer[tmrid].interval = attrib * interval;
}

/*
*******************************************************************************
* Function Name  : StopTimer
* Description    : This function is to stop a soft timer .
* Arguments      : [in] timer id 
* Return         : none
*******************************************************************************
*/
void StopTimer(uint32_t tmrid)
{	 
  timer[tmrid].ready     = FALSE;
  timer[tmrid].interval  = 0;
  timer[tmrid].time      = 0;
}

/*
*******************************************************************************
* Function Name  : LeftTimer
* Description    : This function is to left time of a soft timer .
* Arguments      : [in] timer id 
* Return         : left time
*******************************************************************************
*/
uint32_t LeftTimer(uint32_t tmrid)
{   
  return timer[tmrid].interval;
}

/*
*******************************************************************************
* Function Name  : TimerIsRun
* Description    : This function is to test timer is run.
* Arguments      : [in] timer id 
* Return         : TRUE or FALSE
*******************************************************************************
*/
uint8_t TimerIsRun(uint32_t tmrid)
{  
  if (timer[tmrid].tmrproc == NULL) 
  {
    return FALSE;
  } 
  else 
  {
    if ((timer[tmrid].interval > 0) || (timer[tmrid].ready == TRUE)) 
    {
      return TRUE;
    } 
    else 
    {
      return FALSE;	
    }
  }
}

/*
*******************************************************************************
* Function Name  : ExeOverTimeProc
* Description    : This function is to exe the overtime proc.
* Arguments      : [in] queue pointer
* Return         : none
*******************************************************************************
*/
void ExeOverTimeProc(void)
{
  uint32_t i;  

  for (i=0; i<MAX_TMR; i++) 
  {
    if ((TRUE == timer[i].ready) && (timer[i].tmrproc != NULL)) 
    {
      timer[i].ready = FALSE;
      timer[i].tmrproc(timer[i].tag);
    }
  }
}

/*
*******************************************************************************
* Function Name  : InitTimerMan
* Description    : This function is to initialize the timer management module.
* Arguments      : None
* Return         : None
*******************************************************************************
*/
void InitTimerMan(void)
{
  uint32_t i;
 	  
  for (i=0; i<MAX_TMR; i++) 
  {
      timer[i].ready     = FALSE;
      timer[i].interval  = 0;
      timer[i].tmrproc   = NULL;
  }
 	  
  preticks  = 0;
  systicks  = 0;
}
 
 /*
*******************************************************************************
* Function Name  : TimerEntry
* Description    : This function is the entry of timer management module.
* Arguments      : None
* Return         : None
*******************************************************************************
*/
void TimerEntry(void)
{
  uint32_t     i;
  uint32_t     time;
  uint32_t     curticks;
	  
  curticks = systicks;
	  
  if (curticks == preticks) 
  {
    return;
  } 
  else if (curticks > preticks ) 
  {
    time = curticks - preticks;
  } 
  else 
  {
    time = (0xffffffff - preticks) + curticks;
  }
	  
  preticks  = curticks;
	  
  for (i=0; i<MAX_TMR; i++) 
  {
    if ((timer[i].tmrproc != NULL) && (timer[i].interval > 0)) 
    {
      if (timer[i].interval >= time) 
      {
	timer[i].interval -= time;
      } 
      else 
      {
        timer[i].interval  = 0;
      }

      if (timer[i].interval == 0) 
      {
	 timer[i].ready  = TRUE;
	 ExeOverTimeProc();
	 timer[i].interval = timer[i].time;
      }
    }
  }	  
}

/****************************  END OF FILE  **********************************/
