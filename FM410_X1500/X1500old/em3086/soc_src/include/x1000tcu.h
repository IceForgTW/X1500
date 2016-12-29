/*
 * linux/include/asm-mips/mach-jz4775/jz4775tcu.h
 *
 * X1000 tcu register definition.
 *
 * Copyright (C) 2010 Ingenic Semiconductor Co., Ltd.
 */

#ifndef __X1000TCU_H__
#define __X1000TCU_H__


#define	TCU_BASE	0xB0002000

/* TCU total channel number */
#define TCU_CHANNEL_NUM		8
#define NUM_TIMER_TCU2		7


/*************************************************************************
 * TCU (Timer Counter Unit)
 *************************************************************************/
#define TCU_TSTR	(TCU_BASE + 0xF0) /* Timer Status Register,Only Used In Tcu2 Mode */
#define TCU_TSTSR	(TCU_BASE + 0xF4) /* Timer Status Set Register */
#define TCU_TSTCR	(TCU_BASE + 0xF8) /* Timer Status Clear Register */
#define TCU_TSR		(TCU_BASE + 0x1C) /* Timer Stop Register */
#define TCU_TSSR	(TCU_BASE + 0x2C) /* Timer Stop Set Register */
#define TCU_TSCR	(TCU_BASE + 0x3C) /* Timer Stop Clear Register */
#define TCU_TER		(TCU_BASE + 0x10) /* Timer Counter Enable Register */
#define TCU_TESR	(TCU_BASE + 0x14) /* Timer Counter Enable Set Register */
#define TCU_TECR	(TCU_BASE + 0x18) /* Timer Counter Enable Clear Register */
#define TCU_TFR		(TCU_BASE + 0x20) /* Timer Flag Register */
#define TCU_TFSR	(TCU_BASE + 0x24) /* Timer Flag Set Register */
#define TCU_TFCR	(TCU_BASE + 0x28) /* Timer Flag Clear Register */
#define TCU_TMR		(TCU_BASE + 0x30) /* Timer Mask Register */
#define TCU_TMSR	(TCU_BASE + 0x34) /* Timer Mask Set Register */
#define TCU_TMCR	(TCU_BASE + 0x38) /* Timer Mask Clear Register */

#define TCU_TDFR0	(TCU_BASE + 0x40) /* Timer Data Full Register */
#define TCU_TDHR0	(TCU_BASE + 0x44) /* Timer Data Half Register */
#define TCU_TCNT0	(TCU_BASE + 0x48) /* Timer Counter Register */
#define TCU_TCSR0	(TCU_BASE + 0x4C) /* Timer Control Register */
#define TCU_TDFR1	(TCU_BASE + 0x50)
#define TCU_TDHR1	(TCU_BASE + 0x54)
#define TCU_TCNT1	(TCU_BASE + 0x58)
#define TCU_TCSR1	(TCU_BASE + 0x5C)
#define TCU_TDFR2	(TCU_BASE + 0x60)
#define TCU_TDHR2	(TCU_BASE + 0x64)
#define TCU_TCNT2	(TCU_BASE + 0x68)
#define TCU_TCSR2	(TCU_BASE + 0x6C)
#define TCU_TDFR3	(TCU_BASE + 0x70)
#define TCU_TDHR3	(TCU_BASE + 0x74)
#define TCU_TCNT3	(TCU_BASE + 0x78)
#define TCU_TCSR3	(TCU_BASE + 0x7C)
#define TCU_TDFR4	(TCU_BASE + 0x80)
#define TCU_TDHR4	(TCU_BASE + 0x84)
#define TCU_TCNT4	(TCU_BASE + 0x88)
#define TCU_TCSR4	(TCU_BASE + 0x8C)
#define TCU_TDFR5	(TCU_BASE + 0x90)
#define TCU_TDHR5	(TCU_BASE + 0x94)
#define TCU_TCNT5	(TCU_BASE + 0x98)
#define TCU_TCSR5	(TCU_BASE + 0x9C)
#define TCU_TDFR6	(TCU_BASE + 0xA0)
#define TCU_TDHR6	(TCU_BASE + 0xA4)
#define TCU_TCNT6	(TCU_BASE + 0xA8)
#define TCU_TCSR6	(TCU_BASE + 0xAC)
#define TCU_TDFR7	(TCU_BASE + 0xB0)
#define TCU_TDHR7	(TCU_BASE + 0xB4)
#define TCU_TCNT7	(TCU_BASE + 0xB8)
#define TCU_TCSR7	(TCU_BASE + 0xBC)
///////////////////////////////////////////new register for jz4775

#define REG_TCU_TSTR	REG32(TCU_TSTR)
#define REG_TCU_TSTSR	REG32(TCU_TSTSR)
#define REG_TCU_TSTCR	REG32(TCU_TSTCR)

#define REG_TCU_TSR	REG32(TCU_TSR)
#define REG_TCU_TSSR	REG32(TCU_TSSR)
#define REG_TCU_TSCR	REG32(TCU_TSCR)

#define REG_TCU_TER	REG16(TCU_TER)
#define REG_TCU_TESR	REG32(TCU_TESR)
#define REG_TCU_TECR	REG32(TCU_TECR)

#define REG_TCU_TFR	REG32(TCU_TFR)
#define REG_TCU_TFSR	REG32(TCU_TFSR)
#define REG_TCU_TFCR	REG32(TCU_TFCR)

#define REG_TCU_TMR	REG32(TCU_TMR)
#define REG_TCU_TMSR	REG32(TCU_TMSR)
#define REG_TCU_TMCR	REG32(TCU_TMCR)

#define REG_TCU_TDFR0	REG16(TCU_TDFR0)
#define REG_TCU_TDHR0	REG16(TCU_TDHR0)
#define REG_TCU_TCNT0	REG16(TCU_TCNT0)
#define REG_TCU_TCSR0	REG16(TCU_TCSR0)

#define REG_TCU_TDFR1	REG16(TCU_TDFR1)
#define REG_TCU_TDHR1	REG16(TCU_TDHR1)
#define REG_TCU_TCNT1	REG16(TCU_TCNT1)
#define REG_TCU_TCSR1	REG16(TCU_TCSR1)

#define REG_TCU_TDFR2	REG16(TCU_TDFR2)
#define REG_TCU_TDHR2	REG16(TCU_TDHR2)
#define REG_TCU_TCNT2	REG16(TCU_TCNT2)
#define REG_TCU_TCSR2	REG16(TCU_TCSR2)

#define REG_TCU_TDFR3	REG16(TCU_TDFR3)
#define REG_TCU_TDHR3	REG16(TCU_TDHR3)
#define REG_TCU_TCNT3	REG16(TCU_TCNT3)
#define REG_TCU_TCSR3	REG16(TCU_TCSR3)

#define REG_TCU_TDFR4	REG16(TCU_TDFR4)
#define REG_TCU_TDHR4	REG16(TCU_TDHR4)
#define REG_TCU_TCNT4	REG16(TCU_TCNT4)
#define REG_TCU_TCSR4	REG16(TCU_TCSR4)

#define REG_TCU_TDFR5	REG16(TCU_TDFR5)
#define REG_TCU_TDHR5	REG16(TCU_TDHR5)
#define REG_TCU_TCNT5	REG16(TCU_TCNT5)
#define REG_TCU_TCSR5	REG16(TCU_TCSR5)

#define REG_TCU_TDFR6	REG16(TCU_TDFR6)
#define REG_TCU_TDHR6	REG16(TCU_TDHR6)
#define REG_TCU_TCNT6	REG16(TCU_TCNT6)
#define REG_TCU_TCSR6	REG16(TCU_TCSR6)

#define REG_TCU_TDFR7	REG16(TCU_TDFR7)
#define REG_TCU_TDHR7	REG16(TCU_TDHR7)
#define REG_TCU_TCNT7	REG16(TCU_TCNT7)
#define REG_TCU_TCSR7	REG16(TCU_TCSR7)

// n = 0,1,2,3,4,5,6,7
#define TCU_TDFR(n)	(TCU_BASE + (0x40 + (n)*0x10)) /* Timer Data Full Reg */
#define TCU_TDHR(n)	(TCU_BASE + (0x44 + (n)*0x10)) /* Timer Data Half Reg */
#define TCU_TCNT(n)	(TCU_BASE + (0x48 + (n)*0x10)) /* Timer Counter Reg */
#define TCU_TCSR(n)	(TCU_BASE + (0x4C + (n)*0x10)) /* Timer Control Reg */

#define REG_TCU_TDFR(n)	REG16(TCU_TDFR((n)))
#define REG_TCU_TDHR(n)	REG16(TCU_TDHR((n)))
#define REG_TCU_TCNT(n)	REG16(TCU_TCNT((n)))
#define REG_TCU_TCSR(n)	REG16(TCU_TCSR((n)))

/*
 * TCU registers bit field common define
 */

/* When n is NOT less than TCU_CHANNEL_NUM, change to TCU_CHANNEL_NUM - 1 */
#define __TIMER(n)	(1 << ((n) < TCU_CHANNEL_NUM ? (n) : (TCU_CHANNEL_NUM - 1)))
#define TCU_TIMER(n)	__TIMER(n)

/* Timer counter enable register(TER) */
#define TER_OSTEN	BIT15
#define TER_TCEN(n)	__TIMER(n)

/* Timer counter enable set register(TESR) */
#define TESR_OST	BIT15
#define TESR_TIMER(n)	__TIMER(n)

/* Timer counter enable clear register(TECR) */
#define TECR_OST	BIT15
#define TECR_TIMER(n)	__TIMER(n)

/* Timer stop register(TSR) */
#define TSR_WDT_STOP		BIT16
#define TSR_OST_STOP		BIT15
#define TSR_TIMER_STOP(n)	__TIMER(n)

/* Timer stop set register(TSSR) */
#define TSSR_WDT		BIT16
#define TSSR_OST		BIT15
#define TSSR_TIMER(n)		__TIMER(n)

/* Timer stop clear register(TSCR) */
#define TSCR_WDT		BIT16
#define TSCR_OST		BIT15
#define TSSR_TIMER(n)		__TIMER(n)

/* Timer flag register(TFR) */
#define TFR_HFLAG(n)	(__TIMER(n) << 16)
#define TFR_OSTFLAG	BIT15
#define TFR_FFLAG(n)	__TIMER(n)

/* Timer flag set register(TFSR) */
#define TFSR_HFLAG(n)	(__TIMER(n) << 16)
#define TFSR_OSTFLAG	BIT15
#define TFSR_FFLAG(n)	__TIMER(n)

/* Timer flag clear register(TFCR) */
#define TFCR_HFLAG(n)	(__TIMER(n) << 16)
#define TFCR_OSTFLAG	BIT15
#define TFCR_FFLAG(n)	(__TIMER(n))
#define	TCU_INTR_FLAG(n)	( TFCR_HFLAG(n) | TFCR_FFLAG(n) )

/* Timer mast register(TMR) */
#define TMR_HMASK(n)	(__TIMER(n) << 16)
#define TMR_OSTMASK	BIT15
#define TMR_FMASK(n)	(__TIMER(n))

/* Timer mask set register(TMSR) */
#define TMSR_HMASK(n)	(__TIMER(n) << 16)
#define TMSR_OSTMASK	BIT15
#define TMSR_FMASK(n)	(__TIMER(n))

/* Timer mask clear register(TMCR) */
#define TMCR_HMASK(n)	(__TIMER(n) << 16)
#define TMCR_OSTMASK	BIT15
#define TMCR_FMASK(n)	(__TIMER(n))

// Register definitions
#define TCU_TSTR_REAL2		(1 << 18) /* only used in TCU2 mode */
#define TCU_TSTR_REAL1		(1 << 17) /* only used in TCU2 mode */
#define TCU_TSTR_BUSY2		(1 << 2)  /* only used in TCU2 mode */
#define TCU_TSTR_BUSY1		(1 << 1)  /* only used in TCU2 mode */

#define TCU_TSTSR_REAL2		(1 << 18)
#define TCU_TSTSR_REAL1		(1 << 17)
#define TCU_TSTSR_BUSY2		(1 << 2)
#define TCU_TSTSR_BUSY1		(1 << 1)

#define TCU_TSTCR_REAL2		(1 << 18)
#define TCU_TSTCR_REAL1		(1 << 17)
#define TCU_TSTCR_BUSY2		(1 << 2)
#define TCU_TSTCR_BUSY1		(1 << 1)

#define TCU_TSR_WDTS		(1 << 16) /*the clock supplies to wdt is stopped */
#define TCU_TSR_OSTS		(1 << 15) /*the clock supplies to osts is stopped */
#define TCU_TSR_STOP7		(1 << 7)  /*the clock supplies to timer7 is stopped */
#define TCU_TSR_STOP6		(1 << 6)  /*the clock supplies to timer6 is stopped */
#define TCU_TSR_STOP5		(1 << 5)  /*the clock supplies to timer5 is stopped */
#define TCU_TSR_STOP4		(1 << 4)  /*the clock supplies to timer4 is stopped */
#define TCU_TSR_STOP3		(1 << 3)  /*the clock supplies to timer3 is stopped */
#define TCU_TSR_STOP2		(1 << 2)  /*the clock supplies to timer2 is stopped */
#define TCU_TSR_STOP1		(1 << 1)  /*the clock supplies to timer1 is stopped */
#define TCU_TSR_STOP0		(1 << 0)  /*the clock supplies to timer0 is stopped */

#define TCU_TSSR_WDTSS		(1 << 16)
#define TCU_TSSR_OSTSS		(1 << 15)
#define TCU_TSSR_STPS7		(1 << 7)
#define TCU_TSSR_STPS6		(1 << 6)
#define TCU_TSSR_STPS5		(1 << 5)
#define TCU_TSSR_STPS4		(1 << 4)
#define TCU_TSSR_STPS3		(1 << 3)
#define TCU_TSSR_STPS2		(1 << 2)
#define TCU_TSSR_STPS1		(1 << 1)
#define TCU_TSSR_STPS0		(1 << 0)

#define TCU_TSCR_WDTSC		(1 << 16)
#define TCU_TSCR_OSTSC		(1 << 15)
#define TCU_TSCR_STPC7		(1 << 7)
#define TCU_TSCR_STPC6		(1 << 6)
#define TCU_TSCR_STPC5		(1 << 5)
#define TCU_TSCR_STPC4		(1 << 4)
#define TCU_TSCR_STPC3		(1 << 3)
#define TCU_TSCR_STPC2		(1 << 2)
#define TCU_TSCR_STPC1		(1 << 1)
#define TCU_TSCR_STPC0		(1 << 0)

#define TCU_TER_OSTEN		(1 << 15) /* enable the counter in ost */
#define TCU_TER_TCEN7		(1 << 7)  /* enable the counter in timer5 */
#define TCU_TER_TCEN6		(1 << 6)
#define TCU_TER_TCEN5		(1 << 5)  /* enable the counter in timer5 */
#define TCU_TER_TCEN4		(1 << 4)
#define TCU_TER_TCEN3		(1 << 3)
#define TCU_TER_TCEN2		(1 << 2)
#define TCU_TER_TCEN1		(1 << 1)
#define TCU_TER_TCEN0		(1 << 0)

#define TCU_TESR_OSTST		(1 << 15)
#define TCU_TESR_TCST7		(1 << 7)
#define TCU_TESR_TCST6		(1 << 6)
#define TCU_TESR_TCST5		(1 << 5)
#define TCU_TESR_TCST4		(1 << 4)
#define TCU_TESR_TCST3		(1 << 3)
#define TCU_TESR_TCST2		(1 << 2)
#define TCU_TESR_TCST1		(1 << 1)
#define TCU_TESR_TCST0		(1 << 0)

#define TCU_TECR_OSTCL		(1 << 15)
#define TCU_TECR_TCCL7		(1 << 7)
#define TCU_TECR_TCCL6		(1 << 6)
#define TCU_TECR_TCCL5		(1 << 5)
#define TCU_TECR_TCCL4		(1 << 4)
#define TCU_TECR_TCCL3		(1 << 3)
#define TCU_TECR_TCCL2		(1 << 2)
#define TCU_TECR_TCCL1		(1 << 1)
#define TCU_TECR_TCCL0		(1 << 0)

#define TCU_TFR_HFLAG7		(1 << 23) /* half comparison match flag */
#define TCU_TFR_HFLAG6		(1 << 22)
#define TCU_TFR_HFLAG5		(1 << 21) /* half comparison match flag */
#define TCU_TFR_HFLAG4		(1 << 20)
#define TCU_TFR_HFLAG3		(1 << 19)
#define TCU_TFR_HFLAG2		(1 << 18)
#define TCU_TFR_HFLAG1		(1 << 17)
#define TCU_TFR_HFLAG0		(1 << 16)
#define TCU_TFR_OSTFLAG		(1 << 15) /* ost comparison match flag */
#define TCU_TFR_FFLAG7		(1 << 7)  /* full comparison match flag */
#define TCU_TFR_FFLAG6		(1 << 6)  /* full comparison match flag */
#define TCU_TFR_FFLAG5		(1 << 5)
#define TCU_TFR_FFLAG4		(1 << 4)
#define TCU_TFR_FFLAG3		(1 << 3)
#define TCU_TFR_FFLAG2		(1 << 2)
#define TCU_TFR_FFLAG1		(1 << 1)
#define TCU_TFR_FFLAG0		(1 << 0)

#define TCU_TFSR_HFST7		(1 << 23)
#define TCU_TFSR_HFST6		(1 << 22)
#define TCU_TFSR_HFST5		(1 << 21)
#define TCU_TFSR_HFST4		(1 << 20)
#define TCU_TFSR_HFST3		(1 << 19)
#define TCU_TFSR_HFST2		(1 << 18)
#define TCU_TFSR_HFST1		(1 << 17)
#define TCU_TFSR_HFST0		(1 << 16)
#define TCU_TFSR_OSTFST		(1 << 15)
#define TCU_TFSR_FFST7		(1 << 7)
#define TCU_TFSR_FFST6		(1 << 6)
#define TCU_TFSR_FFST5		(1 << 5)
#define TCU_TFSR_FFST4		(1 << 4)
#define TCU_TFSR_FFST3		(1 << 3)
#define TCU_TFSR_FFST2		(1 << 2)
#define TCU_TFSR_FFST1		(1 << 1)
#define TCU_TFSR_FFST0		(1 << 0)

#define TCU_TFCR_HFCL7		(1 << 23)
#define TCU_TFCR_HFCL6		(1 << 22)
#define TCU_TFCR_HFCL5		(1 << 21)
#define TCU_TFCR_HFCL4		(1 << 20)
#define TCU_TFCR_HFCL3		(1 << 19)
#define TCU_TFCR_HFCL2		(1 << 18)
#define TCU_TFCR_HFCL1		(1 << 17)
#define TCU_TFCR_HFCL0		(1 << 16)
#define TCU_TFCR_OSTFCL		(1 << 15)
#define TCU_TFCR_FFCL7		(1 << 7)
#define TCU_TFCR_FFCL6		(1 << 6)
#define TCU_TFCR_FFCL5		(1 << 5)
#define TCU_TFCR_FFCL4		(1 << 4)
#define TCU_TFCR_FFCL3		(1 << 3)
#define TCU_TFCR_FFCL2		(1 << 2)
#define TCU_TFCR_FFCL1		(1 << 1)
#define TCU_TFCR_FFCL0		(1 << 0)

#define TCU_TMR_HMASK7		(1 << 23) /* half comparison match interrupt mask */
#define TCU_TMR_HMASK6		(1 << 22)
#define TCU_TMR_HMASK5		(1 << 21) /* half comparison match interrupt mask */
#define TCU_TMR_HMASK4		(1 << 20)
#define TCU_TMR_HMASK3		(1 << 19)
#define TCU_TMR_HMASK2		(1 << 18)
#define TCU_TMR_HMASK1		(1 << 17)
#define TCU_TMR_HMASK0		(1 << 16)
#define TCU_TMR_OSTMASK		(1 << 15) /* ost comparison match interrupt mask */
#define TCU_TMR_FMASK7		(1 << 7)  /* full comparison match interrupt mask */
#define TCU_TMR_FMASK6		(1 << 6)
#define TCU_TMR_FMASK5		(1 << 5)  /* full comparison match interrupt mask */
#define TCU_TMR_FMASK4		(1 << 4)
#define TCU_TMR_FMASK3		(1 << 3)
#define TCU_TMR_FMASK2		(1 << 2)
#define TCU_TMR_FMASK1		(1 << 1)
#define TCU_TMR_FMASK0		(1 << 0)

#define TCU_TMSR_HMST7		(1 << 23)
#define TCU_TMSR_HMST6		(1 << 23)
#define TCU_TMSR_HMST5		(1 << 21)
#define TCU_TMSR_HMST4		(1 << 20)
#define TCU_TMSR_HMST3		(1 << 19)
#define TCU_TMSR_HMST2		(1 << 18)
#define TCU_TMSR_HMST1		(1 << 17)
#define TCU_TMSR_HMST0		(1 << 16)
#define TCU_TMSR_OSTMST		(1 << 15)
#define TCU_TMSR_FMST7		(1 << 7)
#define TCU_TMSR_FMST6		(1 << 6)
#define TCU_TMSR_FMST5		(1 << 5)
#define TCU_TMSR_FMST4		(1 << 4)
#define TCU_TMSR_FMST3		(1 << 3)
#define TCU_TMSR_FMST2		(1 << 2)
#define TCU_TMSR_FMST1		(1 << 1)
#define TCU_TMSR_FMST0		(1 << 0)

#define TCU_TMCR_HMCL7		(1 << 23)
#define TCU_TMCR_HMCL6		(1 << 22)
#define TCU_TMCR_HMCL5		(1 << 21)
#define TCU_TMCR_HMCL4		(1 << 20)
#define TCU_TMCR_HMCL3		(1 << 19)
#define TCU_TMCR_HMCL2		(1 << 18)
#define TCU_TMCR_HMCL1		(1 << 17)
#define TCU_TMCR_HMCL0		(1 << 16)
#define TCU_TMCR_OSTMCL		(1 << 15)
#define TCU_TMCR_FMCL7		(1 << 7)
#define TCU_TMCR_FMCL6		(1 << 6)
#define TCU_TMCR_FMCL5		(1 << 5)
#define TCU_TMCR_FMCL4		(1 << 4)
#define TCU_TMCR_FMCL3		(1 << 3)
#define TCU_TMCR_FMCL2		(1 << 2)
#define TCU_TMCR_FMCL1		(1 << 1)
#define TCU_TMCR_FMCL0		(1 << 0)


#define TCU_TCSR_CNT_CLRZ	(1 << 10) /* clear counter to 0, only used in TCU2 mode */
#define TCU_TCSR_PWM_SD		(1 << 9)  /* shut down the pwm output only used in TCU1 mode */
#define TCU_TCSR_PWM_INITL_HIGH	(1 << 8)  /* selects an initial output level for pwm output */
#define TCU_TCSR_PWM_EN		(1 << 7)  /* pwm pin output enable */
#define TCU_TCSR_IN_EN		(1 << 6)  /* PWM input mode enable */
#define TCU_TCSR_PRESCALE_BIT	3         /* select the tcnt count clock frequency*/
#define TCU_TCSR_PRESCALE_MASK	(0x7 << TCU_TCSR_PRESCALE_BIT)
#define TCU_TCSR_PRESCALE1	(0x0 << TCU_TCSR_PRESCALE_BIT)
#define TCU_TCSR_PRESCALE4	(0x1 << TCU_TCSR_PRESCALE_BIT)
#define TCU_TCSR_PRESCALE16	(0x2 << TCU_TCSR_PRESCALE_BIT)
#define TCU_TCSR_PRESCALE64	(0x3 << TCU_TCSR_PRESCALE_BIT)
#define TCU_TCSR_PRESCALE256	(0x4 << TCU_TCSR_PRESCALE_BIT)
#define TCU_TCSR_PRESCALE1024	(0x5 << TCU_TCSR_PRESCALE_BIT)
#define TCU_TCSR_EXT_EN		(1 << 2)  /* select extal as the timer clock input */
#define TCU_TCSR_RTC_EN		(1 << 1)  /* select rtcclk as the timer clock input */
#define TCU_TCSR_PCK_EN		(1 << 0)  /* select pclk as the timer clock input */
#define TCSR_PWM_EN		TCU_TCSR_PWM_EN
#define TCSR_PRESCALE1		TCU_TCSR_PRESCALE1
#define TCSR_PRESCALE4		TCU_TCSR_PRESCALE4
#define TCSR_PRESCALE16		TCU_TCSR_PRESCALE16
#define TCSR_PRESCALE64		TCU_TCSR_PRESCALE64
#define TCSR_PRESCALE256	TCU_TCSR_PRESCALE256
#define TCSR_PRESCALE1024	TCU_TCSR_PRESCALE1024
#define TCSR_EXT_EN		TCU_TCSR_EXT_EN
#define TCSR_RTC_EN		TCU_TCSR_RTC_EN

#define TCU_TSTSR_REALS2	(1 << 18)
#define TCU_TSTSR_REALS1	(1 << 17)
#define TCU_TSTSR_BUSYS2	(1 << 2)
#define TCU_TSTSR_BUSYS1	(1 << 1)

#define TCU_TSTCR_REALC2	(1 << 18)
#define TCU_TSTCR_REALC1	(1 << 17)
#define TCU_TSTCR_BUSYC2	(1 << 2)
#define TCU_TSTCR_BUSYC1	(1 << 1)


#ifndef __MIPS_ASSEMBLER

/***************************************************************************
 * TCU
 ***************************************************************************/
// where 'n' is the TCU channel
#define __tcu_disable_all_clk(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~(TCU_TCSR_EXT_EN | TCU_TCSR_RTC_EN | TCU_TCSR_PCK_EN)))
#define __tcu_select_extalclk(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~(TCU_TCSR_EXT_EN | TCU_TCSR_RTC_EN | TCU_TCSR_PCK_EN)) | TCU_TCSR_EXT_EN)
#define __tcu_select_rtcclk(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~(TCU_TCSR_EXT_EN | TCU_TCSR_RTC_EN | TCU_TCSR_PCK_EN)) | TCU_TCSR_RTC_EN)
#define __tcu_select_pclk(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~(TCU_TCSR_EXT_EN | TCU_TCSR_RTC_EN | TCU_TCSR_PCK_EN)) | TCU_TCSR_PCK_EN)
#define __tcu_disable_pclk(n) \
	REG_TCU_TCSR(n) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PCK_EN);
#define __tcu_select_clk_div1(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE1)
#define __tcu_select_clk_div4(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE4)
#define __tcu_select_clk_div16(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE16)
#define __tcu_select_clk_div64(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE64)
#define __tcu_select_clk_div256(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE256)
#define __tcu_select_clk_div1024(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE1024)

#define __tcu_enable_pwm_output(n)	(REG_TCU_TCSR((n)) |= TCU_TCSR_PWM_EN)
#define __tcu_disable_pwm_output(n)	(REG_TCU_TCSR((n)) &= ~TCU_TCSR_PWM_EN)

#define __tcu_init_pwm_output_high(n)	(REG_TCU_TCSR((n)) |= TCU_TCSR_PWM_INITL_HIGH)
#define __tcu_init_pwm_output_low(n)	(REG_TCU_TCSR((n)) &= ~TCU_TCSR_PWM_INITL_HIGH)

#define __tcu_set_pwm_output_shutdown_graceful(n)	(REG_TCU_TCSR((n)) &= ~TCU_TCSR_PWM_SD)
#define __tcu_set_pwm_output_shutdown_abrupt(n)		(REG_TCU_TCSR((n)) |= TCU_TCSR_PWM_SD)

#define __tcu_clear_counter_to_zero(n)	(REG_TCU_TCSR((n)) |= TCU_TCSR_CNT_CLRZ)

#define __tcu_ost_enabled()		(REG_TCU_TER & TCU_TER_OSTEN)
#define __tcu_enable_ost()		(REG_TCU_TESR = TCU_TESR_OSTST)
#define __tcu_disable_ost()		(REG_TCU_TECR = TCU_TECR_OSTCL)

#define __tcu_counter_enabled(n)	(REG_TCU_TER & (1 << (n)))
#define __tcu_start_counter(n)		(REG_TCU_TESR |= (1 << (n)))
#define __tcu_stop_counter(n)		(REG_TCU_TECR |= (1 << (n)))

#define __tcu_half_match_flag(n)	(REG_TCU_TFR & (1 << ((n) + 16)))
#define __tcu_full_match_flag(n)	(REG_TCU_TFR & (1 << (n)))
#define __tcu_set_half_match_flag(n)	(REG_TCU_TFSR = (1 << ((n) + 16)))
#define __tcu_set_full_match_flag(n)	(REG_TCU_TFSR = (1 << (n)))
#define __tcu_clear_half_match_flag(n)	(REG_TCU_TFCR = (1 << ((n) + 16)))
#define __tcu_clear_full_match_flag(n)	(REG_TCU_TFCR = (1 << (n)))
#define __tcu_mask_half_match_irq(n)	(REG_TCU_TMSR = (1 << ((n) + 16)))
#define __tcu_mask_full_match_irq(n)	(REG_TCU_TMSR = (1 << (n)))
#define __tcu_unmask_half_match_irq(n)	(REG_TCU_TMCR = (1 << ((n) + 16)))
#define __tcu_unmask_full_match_irq(n)	(REG_TCU_TMCR = (1 << (n)))

#define __tcu_ost_match_flag()		(REG_TCU_TFR & TCU_TFR_OSTFLAG)
#define __tcu_set_ost_match_flag()	(REG_TCU_TFSR = TCU_TFSR_OSTFST)
#define __tcu_clear_ost_match_flag()	(REG_TCU_TFCR = TCU_TFCR_OSTFCL)
#define __tcu_ost_match_irq_masked()	(REG_TCU_TMR & TCU_TMR_OSTMASK)
#define __tcu_mask_ost_match_irq()	(REG_TCU_TMSR = TCU_TMSR_OSTMST)
#define __tcu_unmask_ost_match_irq()	(REG_TCU_TMCR = TCU_TMCR_OSTMCL)

#define __tcu_wdt_clock_stopped()	(REG_TCU_TSR & TCU_TSSR_WDTSC)
#define __tcu_ost_clock_stopped()	(REG_TCU_TSR & TCU_TSR_OST)
#define __tcu_timer_clock_stopped(n)	(REG_TCU_TSR & (1 << (n)))

#define __tcu_start_wdt_clock()		(REG_TCU_TSCR = TCU_TSSR_WDTSC)
#define __tcu_start_ost_clock()		(REG_TCU_TSCR = TCU_TSCR_OSTSC)
#define __tcu_start_timer_clock(n)	(REG_TCU_TSCR = (1 << (n)))

#define __tcu_stop_wdt_clock()		(REG_TCU_TSSR = TCU_TSSR_WDTSC)
#define __tcu_stop_ost_clock()		(REG_TCU_TSSR = TCU_TSSR_OSTSS)
#define __tcu_stop_timer_clock(n)	(REG_TCU_TSSR = (1 << (n)))

#define __tcu_get_count(n)		(REG_TCU_TCNT((n)))
#define __tcu_set_count(n,v)		(REG_TCU_TCNT((n)) = (v))
#define __tcu_set_full_data(n,v)	(REG_TCU_TDFR((n)) = (v))
#define __tcu_set_half_data(n,v)	(REG_TCU_TDHR((n)) = (v))

/* TCU2, counter 1, 2*/
#define __tcu_read_real_value(n)	(REG_TCU_TSTR & (1 << ((n) + 16)))
#define __tcu_read_false_value(n)	(REG_TCU_TSTR & (1 << ((n) + 16)))
#define __tcu_counter_busy(n)		(REG_TCU_TSTR & (1 << (n)))
#define __tcu_counter_ready(n)		(REG_TCU_TSTR & (1 << (n)))

#define __tcu_set_read_real_value(n)	(REG_TCU_TSTSR = (1 << ((n) + 16)))
#define __tcu_set_read_false_value(n)	(REG_TCU_TSTCR = (1 << ((n) + 16)))
#define __tcu_set_counter_busy(n)	(REG_TCU_TSTSR = (1 << (n)))
#define __tcu_set_counter_ready(n)	(REG_TCU_TSTCR = (1 << (n)))

#endif /* __MIPS_ASSEMBLER */

#endif /* __X1000TCU_H__ */

