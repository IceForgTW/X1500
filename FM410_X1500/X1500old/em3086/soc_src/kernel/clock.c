/********************** BEGIN LICENSE BLOCK **********************************
 *
 * INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Copyright Ingenic Semiconductor Co., Ltd. 2008. All rights reserved.
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
 *  Author:  <zzhang@ingenic.cn>
 *
 *  Create:   2008-10-11, by zzhang
 *
 *  Maintain:   2008-10-13, by whxu
 *
 ****************************************************************************/
 
#include <bsp.h>
#include <x1000.h>

//----------------------------------------------------------------------------

int get_sdram_configure(unsigned int pllin, unsigned int *sdram_freq)
{
return 0;
#if 0
	register unsigned int ps, dmcr = 0, tmp;
 	#if 0
	ps = 1000000/(pllin / 1000000);
	tmp = (SDRAM_TRAS * 1000)/ps;
	if (tmp < 4) tmp = 4;
	if (tmp > 11) tmp = 11;
	dmcr |= ((tmp-4) << DMCR_TRAS_BIT);

	tmp = (SDRAM_RCD * 1000)/ps;
	if (tmp > 3) tmp = 3;
	dmcr |= (tmp << DMCR_RCD_BIT);

	tmp = (SDRAM_TPC * 1000)/ps;
	if (tmp > 7) tmp = 7;
	dmcr |= (tmp << DMCR_TPC_BIT);

	tmp = (SDRAM_TRWL * 1000)/ps;
	if (tmp > 3) tmp = 3;
	dmcr |= (tmp << DMCR_TRWL_BIT);

	tmp = ((SDRAM_TRAS + SDRAM_TPC) * 1000)/ps;
	if (tmp > 14) tmp = 14;
	dmcr |= (((tmp + 1) >> 1) << DMCR_TRC_BIT);

	/* Set refresh registers */
	tmp = (SDRAM_TREF * 1000)/ps;
	tmp = tmp/64 + 1;
	if (tmp > 0xff) tmp = 0xff;
        *sdram_freq = tmp; 
	#else
	*sdram_freq = __cpm_get_mclk();
	#endif
	
	return dmcr;
#endif
}

void sys_pll_init(unsigned int reserved)
{
#if 0
	unsigned int sdramclock = 0;

	OUTREG32(CPM_CPCCR, CPCCR_CFG);
	OUTREG32(CPM_CPPCR0, CPPCR_CFG);
	while (!((REG_CPM_CPPCR0 & CPPCR0_PLLS) ));


	get_sdram_configure(CFG_CPU_SPEED, &sdramclock);
	if (sdramclock > 0)
	{
		//OUTREG16(A_EMC_RTCOR, sdramclock);qhliu
		//OUTREG16(A_EMC_RTCNT, sdramclock);qhliu

  		// Config SDRAM to auto Power Down Mode
		//SETREG32(A_EMC_DMCR, DMCR_POWER_DOWN);	
	}
	else
	{
		while(1);
	}
#endif
}

//----------------------------------------------------------------------------
unsigned int GetCurrentPLLClock(int pll_sel)
{
     unsigned int pll_cfg, freq, pll_m, pll_n, pll_od;

     if(pll_sel)
	  pll_cfg = INREG32(CPM_CPMPCR);
     else
	  pll_cfg = INREG32(CPM_CPAPCR);
     
     pll_m = ((pll_cfg & CPAPCR_M_MASK )>> CPAPCR_M_LSB) + 1;
     pll_n = ((pll_cfg & CPAPCR_N_MASK) >> CPAPCR_N_LSB) + 1;
     pll_od = ((pll_cfg & CPAPCR_OD_MASK) >> CPAPCR_OD_LSB) + 1;
	

     freq = (CFG_EXTAL * pll_m) / pll_n / pll_od;
     
     return freq;

}
//----------------------------------------------------------------------------
static unsigned int div_matrix[] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 32};

unsigned int GetCommonClock(CLKDIV	clk)
{
	unsigned int div, clock;
	div = (INREG32(CPM_CPCCR) >> (unsigned int)clk) & 0x0000000F;
	clock = GetCurrentPLLClock(0) / div_matrix[div];

	return (clock);
}

//----------------------------------------------------------------------------
int find_cpudiv(int div)
{
	int i;
	for(i = 0;i < sizeof(div_matrix)/sizeof(div_matrix[0]);i++)
		if(div == div_matrix[i])
			return i;
	return -1;
}
//----------------------------------------------------------------------------

void udelay(unsigned int usec)
{
	unsigned int i = usec * (GetCommonClock(CPM_CDIV) / 2400000);

	__asm__ __volatile__ (
		"\t.set noreorder\n"
		"1:\n\t"
		"bne\t%0, $0, 1b\n\t"
		"addi\t%0, %0, -1\n\t"
		".set reorder\n"
		: "=r" (i)
		: "0" (i)
	);
}

//----------------------------------------------------------------------------

void mdelay(unsigned int msec)
{
	unsigned int i;

	for (i = 0; i < msec; i++)
		udelay(1000);
}
//----------------------------------------------------------------------------

