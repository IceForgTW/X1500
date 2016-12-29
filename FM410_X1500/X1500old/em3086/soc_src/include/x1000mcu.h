/*
 * linux/include/asm-mips/mach-jz4775/jz4775mcu.h
 *
 * JZ4775 PDMA MCU register definition.
 *
 * Copyright (C) 2010 Ingenic Semiconductor Co., Ltd.
 */

#ifndef __JZ4775MCU_H__
#define __JZ4775MCU_H__

#define	MCU_BASE	0xB3421000

/*************************************************************************
 * MCU
 *************************************************************************/
#define MCU_DMCSR	(MCU_BASE + 0x30) /* MCU Control & Statue Register */
#define MCU_DMNMB	(MCU_BASE + 0x34) /* MCU Normal MailBox */
#define MCU_DMSMB	(MCU_BASE + 0x38) /* MCU Security MailBox */
#define MCU_DMINT	(MCU_BASE + 0x3c) /* MCU Interrupt Register */

#define REG_MCU_DMCSR	REG32(MCU_DMCSR)
#define REG_MCU_DMNMB	REG32(MCU_DMNMB)
#define REG_MCU_DMSMB	REG32(MCU_DMSMB)
#define REG_MCU_DMINT	REG32(MCU_DMINT)

#define MCU_DMINT_SIP_BIT	17	/* Security MailBox IRQ pending */
#define MCU_DMINT_NIP_BIT	16	/* Normal MailBox IRQ pending */
#define MCU_DMINT_SIMSK_BIT	1	/* Security MailBox IRQ mask */
#define MCU_DMINT_NIMSK_BIT	0	/* Normal MailBox IRQ mask */

#define NUM_MCU			2	/* Num of IRQ source */

#define	__mcu_ack_irq(irq)	{}	/* A dummy ack. */

#define __mcu_mask_irq(irq)		\
do {					\
	REG_MCU_DMINT |= (1 << (irq));	\
} while (0)

#define __mcu_unmask_irq(irq)		\
do {					\
	REG_MCU_DMINT &= ~(1 << (irq));	\
} while (0)

#endif /* __JZ4775MCU_H__ */

