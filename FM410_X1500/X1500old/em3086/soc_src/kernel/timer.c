/********************** BEGIN LICENSE BLOCK **********************************
 *
 * INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Copyright (c) Ingenic Semiconductor Co., Ltd. 2008. All rights reserved.
 *
 * This file, and the files included with this file, is distributed and made
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 *
 * http://www.ingenic.cn
 *
 ********************** END LICENSE BLOCK ************************************
 *
 *  Author:  <dsqiu@ingenic.cn>  <zzhang@ingenic.cn>
 *
 *  Create:   2008-10-11, by zhang zhi
 *
 *  Maintain: 2008-10-11, by whxu
 *
 *  Maintain: 2008-11-29, by whxu, change the pwm function
 *
 *  Maintain: 2008-12-04, by whxu, change the JZ_StartTimer nad JZ_StopTimer
 *
 ****************************************************************************/

#include <bsp.h>
#include <x1000.h>

#include <em3000_cfg.h>
#include <minios.h>
#include <MemMgr.h>
#include <gpio.h>

#undef printf
#include <stdio.h>

#define OST_TICKS_CHN 	0 
#define OST_TIME_CHN 	1

#define OST_TICKS_DIV   16
#define OST_TIME_DIV    16

#define MUSIC_DELAY_PARAM 50

#define PWM_MSG_NUM 1

typedef struct
{
    const char *music;
    int vol;
}stPWMMsg,*pPwmMsg;

static void pwm_task(void *arg);

static void *msg_pwm[PWM_MSG_NUM];
OS_EVENT *g_pwm_q;
static OS_STK	pwm_task_stk[TASK_PWM_STK_SIZE];             // Define the Iotc0370 stack 

static int _wakeupTime = 0;
//static volatile int _b_set_wakeup_timer = 0;
//----------------------------------------------------------------------------
 int _b_set_wakeup_timer = 0;

int _b_set_LedDur = 3;

void hw_reset(void)
{
//        printf("Restarting after 4 ms\n");
//        REG_WDT_TCSR = WDT_TCSR_PRESCALE16 | WDT_TCSR_RTC_EN;
//        REG_WDT_TCNT = 0;
//        REG_WDT_TDR = 5*(RTCCLK/16);//RTCCLK/1000;   /* reset after 4ms */
      //  REG_TCU_TSCR = TSCR_WDT ; /* enable wdt clock */
        //REG_WDT_TCER = WDT_TCER_TCEN;  /* wdt start */
       // while (1);
}

void set_scrpad_data(unsigned int data)
{
	cpm_set_scrpad(data);
}

unsigned int get_scrpad_data(void)
{
	return cpm_get_scrpad();	
}

//----------------------------------------------------------------------------

void JZ_StopTicker(void)
{
	printf("WARMING: System Timer will STOP!!!\n");
	OUTREG32(OST_OSTECR, (1 << OST_TICKS_CHN));
}
int get_ost_div(int div)
{
	int ret;
	switch(div)
	{
		case 1:
		       ret = 0;
		       break;
		case 4:
			ret = 1;
			break;
		case 16:
			ret = 2;
			break;
		default:
			ret = 0;
	}
	return ret;
}
//----------------------------------------------------------------------------

void JZ_StartTicker(unsigned int TicksPerSec)
{
	unsigned int match_counter;
// 	serial_puts("JZ_StartTicker1111111111111\n\n");
// 1. Start OST Clock
	//CLRREG32(A_CPM_CLKGR, CLKGR_STOP_TCU);
	__cpm_start_ost();
	// 2. Disable TCU.
	OUTREG32(OST_OSTMR,1);
	OUTREG32(OST_OSTCR,1);
	OUTREG32(OST_OSTECR, (1 << OST_TICKS_CHN));
	OUTREG32(OST_OSTFR,0);
	OUTREG32(OST_OST1DFR,0);



	OUTREG32(OST_OSTCCR,get_ost_div(OST_TICKS_DIV));
	
	match_counter = OSC_CLOCK / OST_TICKS_DIV / TicksPerSec;
	OUTREG32(OST_OST1DFR,match_counter);

	OUTREG32(OST_OSTMR,0);
	OUTREG32(OST_OSTESR,1);
	//for ost interrupt test
	while(0){
		printf("REG32(OST_OSTFR = 0x%x\n",REG32(OST_OSTFR));
		printf("REG32(OST_OST1CNT = 0x%x\n",REG32(OST_OST1CNT));
		printf("REG32(OST_OST1DFR = 0x%x\n",REG32(OST_OST1DFR));
		if(REG32(OST_OSTFR))
			REG32(OST_OSTFR) = 0;

		
	}
}

//----------------------------------------------------------------------------

void Stop_PerformanceCounter(void)
{
	printf("WARMING: Performance Counter will STOP!!!\n");
	OUTREG32(OST_OSTECR, (1 << OST_TIME_CHN));
}

//----------------------------------------------------------------------------

int Init_PerformanceCounter(void)
{
	unsigned int  ostccr;
	OUTREG32(OST_OSTECR, (1 << OST_TIME_CHN));
	ostccr = REG32(OST_OSTCCR);
	ostccr |= (get_ost_div(OST_TIME_DIV) << 2);
	OUTREG32(OST_OSTCCR,ostccr);
//     printf("ostccr=%x\n", ostccr);
	OUTREG32(OST_OSTESR, (1 << OST_TIME_CHN));

	return OSC_CLOCK / OST_TIME_DIV;

}

//----------------------------------------------------------------------------

unsigned long long   Get_PerformanceCounter()
{
	union clycle_type
	{
		unsigned long long  cycle64;
		unsigned int cycle32[2];
	} cycle;

	cycle.cycle32[0] = REG32(OST_OST2CNTL);
	cycle.cycle32[1] = REG32(OST_OST2CNTHBUF);
	return cycle.cycle64;
}

//----------------------------------------------------------------------------

void JZ_StopTimer(void)
{
	Stop_PerformanceCounter();
}


//----------------------------------------------------------------------------
void JZ_StartTimer(void)
{
	Init_PerformanceCounter();
}

//----------------------------------------------------------------------------

void JZ_InitEx(int chn, int frequence, void (*handler)())
{
	unsigned int match_counter;

	if ( (frequence<((OSC_CLOCK / 4) / 0xffff)+1) || (frequence>(OSC_CLOCK / 400)) )
	{
		printf("The frequence of timer shoud be in [%d,%d]\n",
		(((OSC_CLOCK / 4) / 0xffff)+1),((OSC_CLOCK / 400)));
		return ;
	}

	//CLRREG32(A_CPM_CLKGR, CLKGR_STOP_TCU);
	OUTREG16(TCU_TECR, TECR_TIMER(chn));

	OUTREG16(TCU_TCSR(chn), TCU_TCSR_EXT_EN | TCSR_PRESCALE4);
	match_counter = (OSC_CLOCK / 4) / frequence;
	OUTREG16(TCU_TDFR(chn), match_counter);
	OUTREG16(TCU_TDHR(chn), match_counter);
	OUTREG16(TCU_TCNT(chn), 0x0000);

	OUTREG32(TCU_TFCR, TFCR_HFLAG(chn) |TFCR_FFLAG(chn));
	OUTREG32(TCU_TMCR, TMCR_FMASK(chn));
	OUTREG32(TCU_TMSR, TMCR_HMASK(chn));
    
	request_irq(EIRQ_TCU2_BASE + chn, handler, 0);

}

void JZ_StartTimerEx(int chn)
{
	OUTREG16(TCU_TESR, TESR_TIMER(chn));
}

//----------------------------------------------------------------------------
void JZ_StopTimerEx(int chn)
{
    OUTREG32(TCU_TFCR, (1<<chn) | (1<<(chn+16)));
	OUTREG16(TCU_TECR, TECR_TIMER(chn));
}

static void pwm_set_fre(unsigned int chn, unsigned int frequence, unsigned int mode)
{
	unsigned int full_counter;

	OUTREG16(TCU_TCSR(chn), TCSR_EXT_EN);
	full_counter = (OSC_CLOCK) / frequence;

	OUTREG16(TCU_TDFR(chn), full_counter);
	OUTREG16(TCU_TDHR(chn), 0x0000);
	OUTREG16(TCU_TCNT(chn), 0x0000);

	OUTREG32(TCU_TMSR, TMCR_HMASK(chn) |TMCR_FMASK(chn));
	SETREG32((TCU_TCSR(chn)), (mode & 0x3) << 8);
	SETREG32((TCU_TCSR(chn)), TCSR_PWM_EN);


}



typedef void (*TimerHandler)(void);
TimerHandler thandler[8]= {NULL,};

static void timer_handler(unsigned int chn)
{
	// half 中断即使屏蔽 也是会产生
_b_set_LedDur ++;
    
        OUTREG16(TCU_TECR, TECR_TIMER(chn));
	if (REG32(TCU_TFR) & TFR_FFLAG(chn))
	{
		if (thandler[chn])
				thandler[chn]();
	}
	OUTREG32(TCU_TFCR, (1<<chn) | (1<<(chn+16)));

}

//20us - 500ms 定时器延时, chn: 2 - 7
void timer_delay(int chn, unsigned int us, void (*handler)(void))
{
	unsigned int match_counter;

	if (chn < 2)
	{
		return;
	}

	if ( (us > ((1000000/(OSC_CLOCK / 1024 ))*0xffff)) || (us < (1000000/(OSC_CLOCK / 1024)) ))
	{
		printf("The timer shoud be in [%d us, %d us] \n",
			(1000000/(OSC_CLOCK / 1024)), (1000000/(OSC_CLOCK / 1024 )*0xffff));
		return ;
	}

       _b_set_LedDur = 0;
	//CLRREG32(A_CPM_CLKGR, CLKGR_STOP_TCU);
	OUTREG16(TCU_TECR, TECR_TIMER(chn));
	
	OUTREG16(TCU_TCSR(chn), TCU_TCSR_EXT_EN | TCSR_PRESCALE1024);
	match_counter = ((OSC_CLOCK/1000000)*us)/1024;
	OUTREG16(TCU_TDFR(chn), match_counter);
	OUTREG16(TCU_TDHR(chn), 0);
	OUTREG16(TCU_TCNT(chn), 0x0000);
	
	OUTREG32(TCU_TFCR, TFCR_HFLAG(chn) |TFCR_FFLAG(chn));
	OUTREG32(TCU_TMCR, TMCR_FMASK(chn));
	OUTREG32(TCU_TMSR, TMCR_HMASK(chn));
    
	thandler[chn] = handler;

	free_irq(EIRQ_TCU2_BASE + chn);
	request_irq(EIRQ_TCU2_BASE + chn, timer_handler, chn);
	OUTREG16(TCU_TESR, TESR_TIMER(chn));

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Note:
//    	frequence=[(OSC_CLOCK / 4) / 0xffff+1,((OSC_CLOCK / 4) / 100)]
//    	IF OSC_CLOCK=12,000,000  THEN  frequence=[190,120000]
//      mode: 	bit0---InitLevel  0--LowLevel  1--HightLevel
//				bit1---ShutDown   0--Gracefull 1--Abrutp
void pwm_init(unsigned int chn)
{
	if(chn > 4){
		printf("the pwm chn must be set between 0 to 4\n");
		return;
	}
	switch(chn){
		case 0:
			__gpio_as_func0( GPIO_GROUP_C + 25 );
			break;	
		case 1:
		case 2:
			__gpio_as_func1( GPIO_GROUP_C + 25 + chn);
			break;
		case 3:
			__gpio_as_func2( GPIO_GROUP_B + 6);
			break;
		case 4:
			__gpio_as_func0( GPIO_GROUP_C + 24);
			break;
	}
 
    if(g_pwm_q == 0)
    {
    	g_pwm_q = os_QueueCreate(&msg_pwm[0],PWM_MSG_NUM);

    	os_TaskCreate(pwm_task, NULL, (void *)&pwm_task_stk[TASK_PWM_STK_SIZE-1], TASK_PWM_PRIO);
    }

}


//----------------------------------------------------------------------------

static void pwm_set_duty(unsigned int chn, unsigned int percent)
{

	unsigned int full_cycles;
	static unsigned int old_percent=1;
	//printf("===>%s,chn is %d,percent is %d\n",__func__,chn,percent);
	if (percent == 108)
	{
		OUTREG16(TCU_TESR, TESR_TIMER(chn));
		SETREG32((TCU_TCSR(chn)), TCSR_PWM_EN);
		OUTREG32(TCU_TMSR, TMCR_HMASK(chn) |TMCR_FMASK(chn));
		return;
	}

	if (percent > 100)
	{
		printf("The percent of PWM should be within 100\n");
		return;
	}

	if (percent == 0)
	{
		
		CLRREG32((TCU_TCSR(chn)), TCSR_PWM_EN);
		OUTREG32(TCU_TMSR, TMCR_HMASK(chn) |TMCR_FMASK(chn));
		OUTREG16(TCU_TECR, TECR_TIMER(chn));
		if(old_percent!=0)
		{
			switch(chn){
			case 0:
			case 1:
			case 2:
				gpio_as_output( GPIO_GROUP_C + 25 + chn );
				gpio_clear_pin( GPIO_GROUP_C + 25 + chn );
				break;
			case 3:
				gpio_as_output( GPIO_GROUP_B + 6 );
				gpio_clear_pin( GPIO_GROUP_B + 6 );
				break;
			case 4:
				gpio_as_output( GPIO_GROUP_C + 24 );
				gpio_clear_pin( GPIO_GROUP_C + 24);
				break;
			}
		}
		old_percent=0;
	}
	else
	{
		if(old_percent==0)
		{	
			switch(chn){
			case 0:
				__gpio_as_func0( GPIO_GROUP_C + 25 );
				break;	
			case 1:
			case 2:
				__gpio_as_func1( GPIO_GROUP_C + 25 + chn);
				break;
			case 3:
				__gpio_as_func2( GPIO_GROUP_B + 6);
				break;
			case 4:
				__gpio_as_func0( GPIO_GROUP_C + 24);
				break;
			}
		}
		
		full_cycles = INREG16(TCU_TDFR(chn));
		CLRREG32((TCU_TCSR(chn)), TCSR_PWM_EN);
		OUTREG16(TCU_TDHR(chn), full_cycles - (full_cycles * percent) / 110);
		OUTREG16(TCU_TCNT(chn), 0x0000);
		OUTREG16(TCU_TESR, TESR_TIMER(chn));
		SETREG32((TCU_TCSR(chn)), TCSR_PWM_EN);
		old_percent=percent;

	}

}

//----------------------------------------------------------------------------
void beep_frq(int freq, int msdelay, int volume)
{
    unsigned int percent;
    percent = volume*10/2;

    pwm_set_fre(TCU_TIMER_PWM0, freq, 1);
    pwm_set_duty(TCU_TIMER_PWM0, percent);
    os_TimeDelay((msdelay+9)/10);
    JZ_StopTimerEx(TCU_TIMER_PWM0);  
}


static void _play_my_music(int n, int t, int vol)
{
	unsigned short int frq;
	const int frqs[7] = {1760/*Hz*/, 2010/*Hz*/,
		2260/*Hz*/, 2410/*Hz*/,2710/*Hz*/, 
		3010/*Hz*/, 3360/*Hz*/};
	
	if(t <= 0) return;
	if(n == 36){
        os_TimeDelay((MUSIC_DELAY_PARAM * t+9)/10);
		return;
	}		
	if( n < 0 || n > 35) return;
	
	frq = (frqs[n%7] >> 2) << (n / 7);
	
	beep_frq(frq, MUSIC_DELAY_PARAM * t, vol);
	
}

void playmusic_ex(const char * music, int vol)
{
    pPwmMsg pMsg;

    pMsg = (pPwmMsg)alignAlloc(4, sizeof(stPWMMsg));
    if(pMsg == NULL)
        return;
    
    pMsg->music = music;
    pMsg->vol = vol;

    os_QueuePost(g_pwm_q, (void *)pMsg);          
	return;
}


void playmusic(const char * music)
{
	playmusic_ex(music, 10);
}

static void pwm_task(void *arg)
{
    unsigned char err;
	pPwmMsg pMsg;

    while(1)
    {
    	pMsg = (pPwmMsg)os_QueuePend(g_pwm_q, 0 ,&err);
		if((err == OS_NO_ERR) && (pMsg != NULL))
		{
        	int s=0;
        	char c=0;
        	int t=0;
            const char *music = pMsg->music;
            int vol = pMsg->vol;
            
        	if(music == NULL)
        		return ;
        	do{
        		s = 14;
        		while( *music != '.' && *music != 0){
        			c = *music;
        			if(c == '+') s += 7;
        			if(c == '-') s -= 7;
        			music++;
        		}
        		if(*music == 0)break;
        		music++;
        		t = 0;
        		while( *music != '|' && *music != 0){
        			if(*music > '0' && *music <= '8'){
        				t += (8 / (*music - '0'));
        			}
        			music++;
        		}
        		if(*music == 0)break;
        		
        		if(c == '0')
        			_play_my_music(36,t,vol);
        		else
        			_play_my_music(c - '1' + s,t,vol);
        	}while(*music != 0);
        }

        deAlloc((MEM_ULONG)pMsg);
    }
}


//----------------------------------------------------------------------------
void Wakeup_timer_handler(unsigned int param)
{
	
    JZ_StopTimerEx(TCU_TIMER_WAKEUP);
	
    _b_set_wakeup_timer = 0;
	
}

void Init_Wakeup_Timer(void)
{

	OUTREG16(TCU_TECR, TECR_TIMER(TCU_TIMER_WAKEUP));//stop counting up

	OUTREG16(TCU_TCSR(TCU_TIMER_WAKEUP), TCU_TCSR_EXT_EN | TCSR_PRESCALE1024);

	OUTREG32(TCU_TFCR, TFCR_HFLAG(TCU_TIMER_WAKEUP) |TFCR_FFLAG(TCU_TIMER_WAKEUP));
	OUTREG32(TCU_TMCR, TMCR_FMASK(TCU_TIMER_WAKEUP));
	OUTREG32(TCU_TMSR, TMCR_HMASK(TCU_TIMER_WAKEUP));
    OUTREG16(TCU_TCNT(TCU_TIMER_WAKEUP), 0x0000);
	request_irq(EIRQ_TCU2_BASE + TCU_TIMER_WAKEUP, Wakeup_timer_handler, 0);

}

void Set_Wakeup_timer(int ms)
{
    int freq;
    unsigned int match_counter;
    
    _wakeupTime = ms;
    _b_set_wakeup_timer = 1;

    freq = 1000/ms;
    if ( (freq<((OSC_CLOCK / 1024) / 0xffff)+1) || (freq>(OSC_CLOCK / 400)) )
	{
		printf("The frequence of timer shoud be in [%d,%d]\n",
		(((OSC_CLOCK / 1024) / 0xffff)+1),((OSC_CLOCK / 400)));
		return ;
	}

	
	match_counter = (OSC_CLOCK / 1024) / freq;
	OUTREG16(TCU_TDFR(TCU_TIMER_WAKEUP), match_counter);
	OUTREG16(TCU_TDHR(TCU_TIMER_WAKEUP), match_counter);
        OUTREG16(TCU_TCNT(TCU_TIMER_WAKEUP), 0x0000);
    
    //JZ_InitEx(CHN,freq ,timer_handler);
    JZ_StartTimerEx(TCU_TIMER_WAKEUP);

}

int Get_Wakeup_timer_state(void)
{
	return _b_set_wakeup_timer;
}

