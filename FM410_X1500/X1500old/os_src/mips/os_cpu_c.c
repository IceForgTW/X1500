//********************* BEGIN LICENSE BLOCK **********************************
//
// INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
// Copyright (c) Ingenic Semiconductor Co. Ltd 2009. All rights reserved.
// 
// This file, and the files included with this file, is distributed and made 
// available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
// EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
// INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
// 
// http://www.ingenic.cn 
//
//********************* END LICENSE BLOCK ************************************

#define  OS_CPU_GLOBALS
#include    "sysdefs.h"
#include    "os_cpu.h"
#include    "os_cfg.h"
#include    "minios.h"
//#include    <string.h>


INT32U        OSIntCtxSwFlag = 0;           /* Used to flag a context switch                           */
/*BSP Interface Function */
/*
	void (*Init)(); 
	void (*Deinit)();
	void (*Create)(OS_TCB *ptcb);
	void (*Close)(OS_TCB *ptcb);
	int (*TaskIdle)();
	int (*TaskStat)();	
	void (*TaskSw)();
	void (*TCBInit)(OS_TCB *ptcb);
	void (*TimeTick)();
*/
POSTaskBSPHook g_pBSP = 0;
void os_SetBspInterface(POSTaskBSPHook pBSP)
{
	g_pBSP = pBSP;
}
/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookBegin (void)
{
	BSPHook(Init);
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookEnd (void)
{
	BSPHook(Deinit);
}
#endif

/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning */
    BSPHookArg(Create,ptcb);
}
#endif


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskDelHook (OS_TCB *ptcb)
{
    //ptcb = ptcb;                       /* Prevent compiler warning */
    if(ptcb->OSTCBExtPtr)
    {
    	deAlloc(ptcb->OSTCBExtPtr);
    	ptcb->OSTCBExtPtr = 0;
    }
    BSPHookArg(Close,ptcb);  
}
#endif

/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
void  OSTaskIdleHook (void)
{
	;
}
#endif

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0 
void  OSTaskStatHook (void)
{
	;
}
#endif





/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then 
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see minios.h for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : Interrupts are enabled when your task starts executing. Also that the tasks run in SVC
*              mode.
*********************************************************************************************************
*/

register U32 $GP __asm__ ("$28");

OS_STK *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    OS_STK *stk;
    static INT32U wSR=0;
    static INT32U wGP;

    if ((unsigned int)ptos & 0x7)
      ptos -= 4;
    else
      ptos -= 3;
    
    if(wSR == 0)
    {
	    wSR = CP0_wGetSR();
    	 wSR &= 0xfffffffe;
    	 wSR |= 0x003;

       wGP = $GP;
    }

    opt    = opt;                           /* 'opt' is not used, prevent warning                      */
    stk    = ptos;                          /* Load stack pointer */
    *stk     =  (INT32U)0;                  /*  reserve */
#ifdef ENABLE_FPU
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
	*(--stk) = (INT32U)0;
#endif
    *(--stk) = (INT32U)0;                   /* xr15                                                      */
    *(--stk) = (INT32U)0;                   /* xr14                                                      */
    *(--stk) = (INT32U)0;                   /* xr13                                                      */
    *(--stk) = (INT32U)0;                   /* xr12                                                     */
    *(--stk) = (INT32U)0;                   /* xr11                                                     */
    *(--stk) = (INT32U)0;                   /* xr10                                                      */
    *(--stk) = (INT32U)0;                   /* xr9                                                      */
    *(--stk) = (INT32U)0;                   /* xr8                                                      */
    *(--stk) = (INT32U)0;                   /* xr7                                                      */
    *(--stk) = (INT32U)0;                   /* xr6                                                      */
    *(--stk) = (INT32U)0;                   /* xr5                                                      */
    *(--stk) = (INT32U)0;                   /* xr4                                                      */
    *(--stk) = (INT32U)0;                   /* xr3                                                      */
    *(--stk) = (INT32U)0;                   /* xr2                                                      */
    *(--stk) = (INT32U)0;                   /* xr1                                                      */
    *(--stk) = (INT32U)0;                   /* save register Lo                                      */
    *(--stk) =  (INT32U)0;                  /*  save register Hi */
    *(--stk) = (OS_STK)task;                  /* pc: Entry Point                                         */
    *(--stk) = (INT32U)wSR;                 /* C0_SR: HW0 = En , IE = En                               */
    *(--stk) = (INT32U)0;                   /* at                                                      */
    *(--stk) = (INT32U)0;                   /* v0                                                      */
    *(--stk) = (INT32U)0;                   /* v1                                                      */
    *(--stk) = (INT32U)pdata;               /* a0: argument                                            */
    *(--stk) = (INT32U)0;                   /* a1                                                      */
    *(--stk) = (INT32U)0;                   /* a2                                                      */
    *(--stk) = (INT32U)0;                   /* a3                                                      */
    *(--stk) = (INT32U)0;                   /* t0                                                      */
    *(--stk) = (INT32U)0;                   /* t1                                                      */
    *(--stk) = (INT32U)0;                   /* t2                                                      */
    *(--stk) = (INT32U)0;                   /* t3                                                      */
    *(--stk) = (INT32U)0;                   /* t4                                                      */
    *(--stk) = (INT32U)0;                   /* t5                                                      */
    *(--stk) = (INT32U)0;                   /* t6                                                      */
    *(--stk) = (INT32U)0;                   /* t7                                                      */
    *(--stk) = (INT32U)0;                   /* s0                                                      */
    *(--stk) = (INT32U)0;                   /* s1                                                      */
    *(--stk) = (INT32U)0;                   /* s2                                                      */
    *(--stk) = (INT32U)0;                   /* s3                                                      */
    *(--stk) = (INT32U)0;                   /* s4                                                      */
    *(--stk) = (INT32U)0;                   /* s5                                                      */
    *(--stk) = (INT32U)0;                   /* s6                                                      */
    *(--stk) = (INT32U)0;                   /* s7                                                      */
    *(--stk) = (INT32U)0;                   /* t8                                                      */
    *(--stk) = (INT32U)0;                   /* t9                                                      */
    *(--stk) = (INT32U)wGP;                 /* gp                                                      */
    *(--stk) = (INT32U)0;                   /* fp                                                      */
    *(--stk) = (INT32U)0;                   /* ra                                                      */



    return (stk);
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskSwHook (void)
{
	;
}
#endif

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;     /* Prevent Compiler warning                 */
		BSPHookArg(TCBInit,ptcb);
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTimeTickHook (void)
{
	
}
#endif

register U32 $SP __asm__ ("$29");

//取栈的深度，即栈的使用量，单位byte
unsigned int OSGetTaskStackDepth()
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR    cpu_sr = 0;                                    /* Storage for CPU status register     */
#endif
	U32 depth = 0;
	U32 stk;
	U32 sp;
	
	OS_ENTER_CRITICAL();

	if (OSTCBCur)
	{
		sp = $SP;
		stk = (U32)OSTCBCur->OSTCBStkPtr;
		depth = (sp > stk) ? (sp - stk) : (stk - sp);
	}

	OS_EXIT_CRITICAL();

	return depth;
}




unsigned int CP0_wGet_PRId(void);

unsigned int os_GetProcessorID(void)
{
	unsigned int procid = 4755;
	unsigned int id = CP0_wGet_PRId();

	unsigned int pre_check_id = (unsigned int)(*((volatile unsigned int *)0xB00F0000));

	if (pre_check_id & 0x80000000)
		return (4725);

	switch (id)
	{
		case 0x0AD0024F:
			procid = 4740;
			break;

		case 0x1ED0024F:
			procid = 4750;
			break;

		case 0x2ED0024F:
			procid = 4755;
			break;
	}

	return (procid);
}
