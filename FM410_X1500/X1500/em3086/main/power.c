
/******************************************************************************************
*	include files 
*******************************************************************************************/
#include <watchdog.h>
#include <pm.h>
#include <time-tcu.h>
#include <generate.h>
#include <os_api.h>
#include "x1000.h"
#include "cmos.h"
#include "SendDataRoute.h"

/********************************************************************************************
*	local referenced extern function and variables 
*********************************************************************************************/
extern void CmosStandby(BOOL bStandby);

/*********************************************************************************************
*	local function prototype
*********************************************************************************************/

/*********************************************************************************************
*	Implementation functions 
*********************************************************************************************/
extern int *pIdleTick;
extern int _b_set_LedDur;
void DeepSleep(void)
{

}

void USBSuspend(void)
{

}



/***********************************
* mode: 0	=  不允许OST1定时唤醒
*		1   =  允许OST1 唤醒
***********************************/
void Idle(int mode)
{
	int dog_time;
	unsigned int save_ICMR0;
	unsigned int save_ICMR1;
//	int save_CKEN;
//	int save_CCCR;
//	int save_OSCR;
	int save_tick;
	unsigned int sr;


/////////////////
//  	printf(" into idle, mode:%d \r\n", mode);
///////////////
      if((_b_set_LedDur == 0)||(_b_set_LedDur == 1))
            return;
     // printf("test");
 	if(IsKBDDataSendcomplete() == FALSE)
 		return;

// 	printf("gint: %x\r\n", REG_GINT_MASK);

	if (mode == 0)
	{
		CmosStandby(TRUE);

	}
	else
	{
		CmosStandby(TRUE);
	}

	
// 	save_tick = GetTick();
	sr = spin_lock_irqsave();
// 	save_OSCR = OSCR;

	dog_time = GetWatchDogResetTime();
	DisableWatchDog();
	led_off(4);
	led_off(2);
// 	led_off(1);

	save_tick = Get_PerformanceCounter()/15000;
	save_ICMR0 = REG_INTC_IMR(0);
	save_ICMR1 = REG_INTC_IMR(1);

// printf("a icmr: %x, %x\r\n", save_ICMR0, save_ICMR1);
// 
	REG_INTC_IMR(0) = 0xffffffff;
	REG_INTC_IMR(1) = 0xffffffff;
	REG_INTC_IMCR(0) = ((1<<17) | (1<<21)|(1<<16));//port0 port1
	REG_INTC_IMCR(1) = (1<<19);//uart0


	if ((mode == 0x1) && Get_Wakeup_timer_state())
	{
		REG_INTC_IMCR(0) =  (1<<25);	//ost1 use tcu2	
	}


// 	printf("b icmr: %x, %x\r\n", REG_INTC_IMR(0), REG_INTC_IMR(1));

	jz_pm_idle();

	//exit idle mode	

// 	printf("wakeup icpr0: %x, icpr1: %x, isr: %x, %x\r\n", REG_INTC_IPR(0), REG_INTC_IPR(1),REG_INTC_ISR(0),REG_INTC_ISR(1));

	REG_INTC_IMR(0) = save_ICMR0;
	REG_INTC_IMR(1) = save_ICMR1;

	if(dog_time > 0)
	{
		SetWatchDogResetTime(dog_time);
		EnableWatchDog();
		ResetWatchDog();
	}


	if ((mode == 0x1) && Get_Wakeup_timer_state())
	{
		os_TimeSet(os_TimeGet() + Get_PerformanceCounter()/15000 - save_tick);
	}
	else
	{
		os_TimeSet(os_TimeGet() + 3);
	}

	spin_unlock_irqrestore(sr);	

	if (mode == 0)
	{

		CmosStandby(FALSE);
	}
	else
	{
		CmosStandby(FALSE);
	}
      //*pIdleTick = os_TimeGet();
      //printf("pa=%x\n",pIdleTick);
      *pIdleTick = os_TimeGet();
//printf("time2=%d\r\n",  *pIdleTick);
// 	mdelay(20);	//usb唤醒需要延时

/////////////////
//  	printf(" out idle\r\n");
///////////////
}



//////////////////////////////////////////////

void Standby(void)
{
	int dog_time;
//	unsigned int sr;


/////////////////
 	printf(" into standby\r\n");
///////////////

	
// 	sr = spin_lock_irqsave();

	dog_time = GetWatchDogResetTime();
	DisableWatchDog();


	CmosStandby(TRUE);

	led_off(1);
	led_off(2);
	led_off(4);


	jz_pm_sleep();
	//exit standby mode	

	
// 	if(dog_time > 0)
// 	{
// 		SetWatchDogResetTime(dog_time);
// 		EnableWatchDog();
// 	}

// 	spin_unlock_irqrestore(sr);	
	
	CmosStandby(FALSE);


/////////////////
 	printf("out standby\r\n");
///////////////
}
