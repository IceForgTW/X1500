/*
 * linux/include/asm-mips/mach-jz4775/jz4775intc.h
 *
 * X1000 INTC register definition.
 *
 * Copyright (C) 2010 Ingenic Semiconductor Co., Ltd.
 */

#ifndef __X1000INTC_H__
#define __X1000INTC_H__


//#define	INTC_BASE	0xB0010000
#define	INTC_BASE	0xB0001000


/*************************************************************************
 * INTC (Interrupt Controller)
 *************************************************************************/
/* n = 0 ~ 1 */
#define INTC_ISR(n)	(INTC_BASE + 0x00 + (n) * 0x20)
#define INTC_ICMR(n)	(INTC_BASE + 0x04 + (n) * 0x20)
#define INTC_ICMSR(n)	(INTC_BASE + 0x08 + (n) * 0x20)
#define INTC_ICMCR(n)	(INTC_BASE + 0x0c + (n) * 0x20)
#define INTC_IPR(n)	(INTC_BASE + 0x10 + (n) * 0x20)

#define INTC_DSR0       (INTC_BASE + 0x34)
#define INTC_DMR0       (INTC_BASE + 0x38)
#define INTC_DPR0       (INTC_BASE + 0x3c)
#define INTC_DSR1       (INTC_BASE + 0x40)
#define INTC_DMR1       (INTC_BASE + 0x44)
#define INTC_DPR1       (INTC_BASE + 0x48)

#define REG_INTC_ISR(n)		REG32(INTC_ISR(n))
#define REG_INTC_IMR(n)		REG32(INTC_ICMR(n))
#define REG_INTC_IMSR(n)	REG32(INTC_ICMSR(n))
#define REG_INTC_IMCR(n)	REG32(INTC_ICMCR(n))
#define REG_INTC_IPR(n)		REG32(INTC_IPR(n))

#define REG_INTC_DSR0           REG32(INTC_DSR0)
#define REG_INTC_DMR0           REG32(INTC_DMR0)
#define REG_INTC_DPR0           REG32(INTC_DPR0)
#define REG_INTC_DSR1           REG32(INTC_DSR1)
#define REG_INTC_DMR1           REG32(INTC_DMR1)
#define REG_INTC_DPR1           REG32(INTC_DPR1)

// 1st-level interrupts
#define IRQ_DMIC	0
#define IRQ_AIC		1
/*
 Reserved
*/
#define IRQ_SFC		7
#define IRQ_SSI0   	8
#define IRQ_PDMA	10
#define IRQ_PDMAD	11
/*
 * Reserved
*/
#define IRQ_GPIO3	14
#define IRQ_GPIO2	15
#define IRQ_GPIO1	16
#define IRQ_GPIO0	17
/*
 Reserved
*/
#define IRQ_OTG		21
/*
  Reserved
*/
#define IRQ_TCU2	25
#define IRQ_TCU1	26
#define IRQ_TCU0	27
#define IRQ_CIM		30
#define IRQ_LCD		31

#define IRQ_RTC		32
/*
 *Reserved
 */
#define IRQ_MSC1	36
#define IRQ_MSC0	37
#define IRQ_SCC		38	
/*
  Reserved
*/
#define IRQ_PCM0	40
/*
 * Reserved
 */
#define IRQ_HARB2	44

#define IRQ_HARB0	46
#define IRQ_CPM         47

#define IRQ_UART2       49
#define IRQ_UART1       50
#define IRQ_UART0       51
#define IRQ_UART(n)	IRQ_UART##n
#define IRQ_DDR         52
#define IRQ_EFUSE	54
#define IRQ_MAC         55
/*
 * Reserved
 */
#define IRQ_SMB2        58
#define IRQ_SMB1        59
#define IRQ_SMB0        60
#define IRQ_PDMAM       61
#define IRQ_JPEG         62

#define IRQ_I2C2        IRQ_SMB2
#define IRQ_I2C1        IRQ_SMB1
#define IRQ_I2C0        IRQ_SMB0

// Extended IRQ Boundary define

#define IRQ_INTC_BASE	0
#define IRQ_INTC0_MAX	32
#define IRQ_INTC0_BASE	IRQ_INTC0_MAX

#define IRQ_INTC_MAX    64 /**/	

#define EIRQ_TCU2_BASE 	IRQ_INTC_MAX
#define	EIRQ_TCU2_MAX	(EIRQ_TCU2_BASE + NUM_TIMER_TCU2)	/* 64 +  7 = 71 */	
				
#define	EIRQ_PDMA_BASE	(EIRQ_TCU2_MAX)		
#define	EIRQ_PDMA_MAX	(EIRQ_PDMA_BASE + NUM_DMA)	/* 71 +  32 = 103 */

#define	EIRQ_GPIO_BASE	(EIRQ_PDMA_MAX)	
#define	EIRQ_GPIO_MAX	(EIRQ_GPIO_BASE + MAX_GPIO_NUM) /* 103 +  224 = 327 */

#define IRQ_MAX		(EIRQ_GPIO_MAX)

#define	EIRQ_TCU2_BIT	(1 << IRQ_TCU2)
#define EIRQ_PDMA_BIT	(1 << IRQ_PDMA)
#define EIRQ_DMA_BASE	EIRQ_PDMA_BASE

// GPIO, from PORT-A to PORT-D
#define EIRQ_PORTD_BIT		(1 << IRQ_GPIO3)
#define EIRQ_PORTC_BIT		(1 << IRQ_GPIO2)
#define EIRQ_PORTB_BIT		(1 << IRQ_GPIO1)
#define EIRQ_PORTA_BIT		(1 << IRQ_GPIO0)
#define	EIRQ_PORT_A2D_BIT	( EIRQ_PORTA_BIT | EIRQ_PORTB_BIT	\
				| EIRQ_PORTC_BIT | EIRQ_PORTD_BIT	)

#define	EIRQ_ALL_BIT	(EIRQ_PORT_A2D_BIT | EIRQ_PDMA_BIT | EIRQ_TCU2_BIT) 	

// 2nd-level interrupts

#define IRQ_DMA_0	65    /* 64,65,66,67,68,69 */
#define IRQ_GPIO_0	(IRQ_DMA_0 + MAX_DMA_NUM)
#define IRQ_MCU_0	(IRQ_GPIO_0 + MAX_GPIO_NUM)
#define IRQ_SYSOST_0    (IRQ_MCU_0 + NUM_MCU) 

#define NUM_INTC	64
#define NUM_DMA         MAX_DMA_NUM	/* 32 */
#define NUM_GPIO        MAX_GPIO_NUM	/* GPIO NUM: 192, Jz4775 real num GPIO 178 */
#define NUM_SYSOST      32     /* new add on jz4775, MAX_OST_NUM: 32, jz4775 rea use num is 3. */


#ifndef __MIPS_ASSEMBLER

typedef struct CP0_tstREGS
{
	unsigned int dwStatus;
	unsigned int dwCause;
} CP0_tstREGS;


/***************************************************************************
 * INTC
 ***************************************************************************/
#define __intc_unmask_irq(n)	(REG_INTC_IMCR((n)/32) = (1 << ((n)%32)))
#define __intc_mask_irq(n)	(REG_INTC_IMSR((n)/32) = (1 << ((n)%32)))
#define __intc_ack_irq(n)	(REG_INTC_IPR((n)/32) = (1 << ((n)%32))) /* A dummy ack, as the Pending Register is Read Only. Should we remove __intc_ack_irq() */


/************ intc extern fun *******************/
void enable_irq(unsigned int irq);
void disable_irq(unsigned int irq);
void intc_init(void);
int request_irq(unsigned int irq, void (*handler)(unsigned int), unsigned arg);
void free_irq(unsigned int irq);
void C_vINTHandler(CP0_tstREGS *pstC0);
void cli(void);
void sti(void);
unsigned int spin_lock_irqsave(void);
void spin_unlock_irqrestore(unsigned int val);
void hw_reset(void);

#endif /* __MIPS_ASSEMBLER */

#endif /* __X1000INTC_H__ */

