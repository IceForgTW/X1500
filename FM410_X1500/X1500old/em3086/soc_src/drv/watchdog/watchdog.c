
/*!
 * @file watchdog.c
 * @brief  watch dog driver source file.
 *
 */


//#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "watchdog.h"
#include <x1000.h>

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
#define RTCCLK 46875
//32768
static int resettime=0;

int GetWatchDogResetTime(void)//ms
{
    unsigned char wTECR;

    wTECR = REG8(WDT_TCER);
    if(!(wTECR&WDT_TCER_TCEN))
        return 0;

    //count = REG16(WDT_TDR);
    //time = count * 16*1000/RTCCLK;
    //printf("1TDR=%X\n",REG16(WDT_TDR));   
    return resettime;
}

int SetWatchDogResetTime(int ms)
{
    int rsttime;
    
    if(ms < 1) return -1;	

    resettime = ms;
    rsttime = ms*RTCCLK/16/1000;

    //printf("rsttime=%d\r\n",rsttime);
    //temp = rsttime*31/100;
    //rsttime = rsttime+temp;
    REG_WDT_TDR = rsttime;
    //printf("2TDR=%X",REG16(WDT_TDR));   
    return 0;

}

int ResetWatchDog(void)//dogfeed
{
    if(!(REG_WDT_TCER & WDT_TCER_TCEN ))//watch dog is disable now
        return -1;
    REG_WDT_TCNT = 0;
    return 0;
}

int EnableWatchDog(void)
{
    REG_TCU_TSCR = TSCR_WDT ; /* enable wdt clock */
    REG_WDT_TCER = WDT_TCER_TCEN;  /* wdt start */
    return 0;
}

int DisableWatchDog(void)
{  
//     REG_WDT_TCER &= ~WDT_TCER_TCEN;  /* wdt stop */
	REG_TCU_TSSR = TSCR_WDT ; /* disable wdt clock */
   return 0;
}

void ResetSystemByWD(void)
{
    REG_WDT_TCSR = WDT_TCSR_PRESCALE16 | WDT_TCSR_RTC_EN;
    REG_WDT_TCNT = 0;
    REG_WDT_TDR = 10;//RTCCLK/1000;   /* reset after 4ms */
    REG_TCU_TSCR = TSCR_WDT ; /* enable wdt clock */
    REG_WDT_TCER = WDT_TCER_TCEN;  /* wdt start */
    while (1);
}


int Init_WatchDog(void)
{

	OUTREG32(RTC_RTCCR,  RTC_RTCCR_SELEXC | RTC_RTCCR_RTCE);
    REG_WDT_TCSR = WDT_TCSR_PRESCALE16 | WDT_TCSR_RTC_EN;
    REG_WDT_TCNT = 0;
    
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
