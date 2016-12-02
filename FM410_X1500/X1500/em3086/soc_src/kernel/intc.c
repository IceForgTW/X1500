/********************** BEGIN LICENSE BLOCK **********************************
 *
 * JZ4755  mobile_tv  Project  V1.0.0
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
 *  Author:  <dsqiu@ingenic.cn>  <jgao@ingenic.cn>
 *
 *  Create:   2008-06-26, by dsqiu
 *
 *  Maintain: 2008-09-26, by zzhang, migrate from JZ4740
 *
 *  Maintain: 2008-10-09, by whxu, clean up the code
 *
 *  Maintain: 2008-10-10, by whxu, Remove ack_irq(), fix TCU2 bug
 *
 *****************************************************************************
 */
#include <stdio.h>
#include <mipsregs.h>
#include <x1000.h>
#include <os_api.h>
#undef printf

static unsigned intc_flag = 0;
static struct {
	void (*handler)(unsigned int);
	unsigned int arg;
} irq_table[IRQ_MAX];

//----------------------------------------------------------------------------


static void default_handler(unsigned int arg)
{
#if (RELEASE == 0)
	printf("11default_handler====\n");

	register unsigned int	group, bit;
	if (arg < IRQ_INTC_MAX)
		printf("Get the irq%d of INTC without irq handler, STOP here for ever\n", arg);
	else if (arg < EIRQ_TCU2_MAX)
		printf("Get the irq%d of TCU2 without irq handler, STOP here for ever\n", (arg - EIRQ_TCU2_BASE));
	else if (arg < EIRQ_PDMA_MAX)
		printf("Get the irq%d of DMA without irq handler, STOP here for ever\n", (arg - EIRQ_DMA_BASE));
	else if (arg < EIRQ_GPIO_MAX)
	{
		group = (arg - EIRQ_GPIO_BASE) / 32;
		bit   = (arg - EIRQ_GPIO_BASE) % 32;
		printf("Get the irq%d of GPIO group%d without irq handler, STOP here for ever\n", bit,group);
	}
	printf("ICMR= 0x%08x \n", INREG32(0xb0001004));
	printf("ICPR= 0x%08x \n", INREG32(0xb0001010));
	while(1);
#endif
}

//----------------------------------------------------------------------------

void enable_irq(unsigned int irq)
{
//	register unsigned int	group, bit;

#ifdef OPT_GPIO_VIRTUAL_INTR//robsky(2009.11.26)added for virtual gpio	
	if( irq >= IRQ_MAX && irq < (IRQ_MAX + GPIO_VIRTUAL_NUM) )
	{
		virtual_gpio_enable_irq(irq-IRQ_MAX);
		return;
	}
#endif
	
	if (irq < IRQ_INTC0_MAX)
		SETREG32(INTC_ICMCR(0), 1 << irq);
	else if (irq < IRQ_INTC_MAX)
		SETREG32(INTC_ICMCR(1), 1 << (irq - IRQ_INTC0_MAX));
	else if (irq < EIRQ_TCU2_MAX)
	{
		SETREG32(INTC_ICMCR(0), EIRQ_TCU2_BIT);
		SETREG32(TCU_TMCR, TMCR_HMASK(irq - EIRQ_TCU2_BASE) | TMCR_FMASK(irq - EIRQ_TCU2_BASE));
	}
	else if (irq < EIRQ_PDMA_MAX)
	{
		if (!cpm_pdma_started())
			__cpm_start_pdma();
		SETREG32(INTC_ICMCR(0), EIRQ_PDMA_BIT);
		SETREG32(DMAC_DCMD(irq - EIRQ_DMA_BASE), DMAC_DCMD_TIE);
	}
	else if (irq < EIRQ_GPIO_MAX)
	{
		if (irq == EIRQ_GPIO_BASE + GPA(31))
			return;
		SETREG32(INTC_ICMCR(0), 1 << (IRQ_GPIO0 - ((irq - EIRQ_GPIO_BASE) / 32)));
		__gpio_unmask_irq(irq - EIRQ_GPIO_BASE);
	}
}

//----------------------------------------------------------------------------

void disable_irq(unsigned int irq)
{
//	register unsigned int	group, bit;

#ifdef OPT_GPIO_VIRTUAL_INTR//robsky(2009.11.26)added for virtual gpio	
	if( irq >= IRQ_MAX && irq < (IRQ_MAX + GPIO_VIRTUAL_NUM) )
	{
		virtual_gpio_disable_irq(irq-IRQ_MAX);
		return;
	}
#endif	
	if (irq < IRQ_INTC0_MAX)
		REG_INTC_IMSR(0) |= (1 << irq);
	else if (irq < IRQ_INTC_MAX)
		REG_INTC_IMSR(1) |= (1 << irq);
	else if (irq < EIRQ_TCU2_MAX)
		OUTREG32(TCU_TMSR, TMSR_HMASK(irq - EIRQ_TCU2_BASE) | TMSR_FMASK(irq - EIRQ_TCU2_BASE));
	else if (irq < EIRQ_PDMA_MAX) {
		if (!cpm_pdma_started())
			__cpm_start_pdma();
		CLRREG32(DMAC_DCMD(irq - EIRQ_DMA_BASE), DMAC_DCMD_TIE);
	}
	else if (irq < EIRQ_GPIO_MAX)
	{
		if (irq == EIRQ_GPIO_BASE + GPA(31))
			return;
		REG_INTC_IMSR(0) |= (1 << (IRQ_GPIO0 - ((irq - EIRQ_GPIO_BASE) / 32)));
		__gpio_mask_irq(irq - EIRQ_GPIO_BASE);
	}
}

/**
 * fls - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
 */

static unsigned  int fls(int x)
{

	unsigned int i;
	for (i=0; i<32; i++)
	{
		if (x & (1 << (31 - i)))
			return (32 - i);
	}
	return 0;
}


//----------------------------------------------------------------------------
static inline void intc_irq(void)
{
	register unsigned int irq, ipl, iplh, tmp;
	unsigned int i, group;
	ipl = INREG32(INTC_IPR(0));	
	iplh = INREG32(INTC_IPR(1));
	

	if (!(ipl || iplh))
		return;
	
	if (ipl)
	{
		irq = fls(ipl) - 1;
		if (ipl & EIRQ_ALL_BIT)
		{
			if (ipl & EIRQ_PDMA_BIT)
			{	
				tmp = INREG32(DMAC_DMAIPR);
				for (irq = 0; irq < MAX_DMA_NUM; irq++)
					if (tmp & (1 << irq))
						break;
		
				irq += EIRQ_DMA_BASE;
				irq_table[irq].handler(irq_table[irq].arg);
			}
			if (ipl & EIRQ_TCU2_BIT)
			{
				tmp = INREG32(TCU_TFR) & (~ INREG32(TCU_TMR));
				for (irq = 0; irq < NUM_TIMER_TCU2; irq++)
					if (tmp & TCU_INTR_FLAG(irq))
						break;
				irq += EIRQ_TCU2_BASE;
				irq_table[irq].handler(irq_table[irq].arg);
			}

			if (ipl & EIRQ_PORT_A2D_BIT)
			{
				if (irq <= IRQ_GPIO0)
				{
					group = IRQ_GPIO0 - irq;
					irq = EIRQ_GPIO_BASE + __gpio_group_irq(group) + group * 0x20;
					irq_table[irq].handler(irq_table[irq].arg);
				}
			}

			ipl &= ~EIRQ_ALL_BIT;

		}

		for (i=0; i<32; i++)
		{
			if (ipl & (1 << irq))
			{
				ipl &= ~(1 << irq);
				irq_table[irq].handler(irq_table[irq].arg);
			}
		}
	}
    
	if (iplh)
	{
		for (irq = 0; irq < 20; irq++)
		{
			if (iplh & (1 << irq))
			{
				iplh &= ~(1 << irq);
				irq += IRQ_INTC0_MAX;
				irq_table[irq].handler(irq_table[irq].arg);
			}
		}
	}
	
#if 0
	}
	else 
	{
		if (ipl & 2)
		{
			irq = 1;
			irq_table[irq].handler(irq_table[irq].arg);
		} else {
			irq = 0;
			irq_table[irq].handler(irq_table[irq].arg);
		}
	}
#endif
}

//----------------------------------------------------------------------------

void intc_init(void)
{
	unsigned int i;

	__intc_unmask_irq(IRQ_GPIO0);
	__intc_unmask_irq(IRQ_GPIO1);
	__intc_unmask_irq(IRQ_GPIO2);
	__intc_unmask_irq(IRQ_GPIO3);

	for (i = 0; i < IRQ_MAX; i++)
	{
		irq_table[i].handler = default_handler;
		irq_table[i].arg = i;
	}
}

//----------------------------------------------------------------------------

int request_irq(unsigned int irq, void (*handler)(unsigned int), unsigned arg)
{
	if (irq >= IRQ_MAX)
	{
#ifdef OPT_GPIO_VIRTUAL_INTR//robsky(2009.11.26)added for virtual gpio	
		if( irq < IRQ_MAX + GPIO_VIRTUAL_NUM )
		{
			return virtual_gpio_request_irq(irq-IRQ_MAX, handler, arg);
		}
#endif		
		printf("The IRQ[%d] requested is larger than IRQ_MAX[%d]\n",
					irq, IRQ_MAX);
		return (-1);
	}

	if (irq_table[irq].handler != default_handler)
	{
		printf("The IRQ[%d] has been requested already\n", irq);
		return (-1);
	}

	irq_table[irq].handler = handler;
	irq_table[irq].arg = arg;

	enable_irq(irq);
	return 0;
}

//----------------------------------------------------------------------------

void free_irq(unsigned int irq)
{
#ifdef OPT_GPIO_VIRTUAL_INTR//robsky(2009.11.26)added for virtual gpio	
	if( irq >= IRQ_MAX && irq < (IRQ_MAX + GPIO_VIRTUAL_NUM) )
	{
		virtual_gpio_free_irq(irq-IRQ_MAX);
		return;
	}
#endif	
	disable_irq(irq);
	irq_table[irq].handler = default_handler;
	irq_table[irq].arg = irq;
}

//----------------------------------------------------------------------------


void C_vINTHandler(CP0_tstREGS *pstC0)
{

	if (intc_flag > 0)
	{
		printf("###%d\n", intc_flag);
	}

	intc_flag ++;
	if(pstC0->dwCause & CAUSEF_IP3){
		OUTREG32(OST_OSTFR,0);
		os_SystemTick();
	}else{
		intc_irq();
	}
	
	intc_flag --;
}

//----------------------------------------------------------------------------

unsigned int mips_get_sr(void)
{
	unsigned int t = read_c0_status();

	return (t);
}

//----------------------------------------------------------------------------

void cli(void)
{
	register unsigned int t;

	t = read_c0_status();
	t &= ~1;
	write_c0_status(t);
}

//----------------------------------------------------------------------------

void sti(void)
{
	register unsigned int t;

	t = read_c0_status();
	t |= 1;
	t &= ~2;
	write_c0_status(t);
}
//----------------------------------------------------------------------------
unsigned int spin_lock_irqsave(void)
{
	register unsigned int t;
	t = read_c0_status();
	write_c0_status((t & (~1)));
	return (t);
}

//----------------------------------------------------------------------------

void spin_unlock_irqrestore(unsigned int val)
{
	write_c0_status(val);
}

//----------------------------------------------------------------------------
void watch_write_address(unsigned int id,unsigned int addr_from,unsigned int size)
{
	printf("%s-> id = %d ,0x%x\n",__FUNCTION__,id,addr_from);
	addr_from = (addr_from & (~7)) | 1;
	size = (size & (0xff <<3))|(1 << 30);

	switch(id){
	case 0:
		write_c0_watchlo0(addr_from);
		write_c0_watchhi0(size);
		break;
	case 1:
		write_c0_watchlo1(addr_from);
		write_c0_watchhi1(size);
		break;
	case 2:
		write_c0_watchlo2(addr_from);
		write_c0_watchhi2(size);
		break;
	case 3:
		write_c0_watchlo3(addr_from);
		write_c0_watchhi3(size);
		break;
	case 4:
		write_c0_watchlo4(addr_from);
		write_c0_watchhi4(size);
		break;
	case 5:
		write_c0_watchlo5(addr_from);
		write_c0_watchhi5(size);
		break;
	case 6:
		write_c0_watchlo6(addr_from);
		write_c0_watchhi6(size);
		break;
	case 7:
		write_c0_watchlo7(addr_from);
		write_c0_watchhi7(size);
		break;
	default:
		break;
	}
}

