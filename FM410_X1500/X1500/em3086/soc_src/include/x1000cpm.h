/*
 * jz4775cpm.h
 * X1000 CPM register definition
 * Copyright (C) 2010 Ingenic Semiconductor Co., Ltd.
 *
 * Author: whxu@ingenic.cn
 */

#ifndef __X1000CPM_H__
#define __X1000CPM_H__

#ifndef JZ_EXTAL
#define JZ_EXTAL		24000000
#endif
#ifndef JZ_EXTAL2
#define JZ_EXTAL2		32768     /* 32.768 KHz */
#endif

/*
 * Clock reset and power controller module(CPM) address definition
 */
#define	CPM_BASE	0xb0000000

/*
 * CPM registers offset address definition
 */
#define CPM_CPCCR_OFFSET	(0x00)  /* rw, 32, 0x95000000 */
#define CPM_CPCSR_OFFSET	(0xd4)  /* rw, 32, 0x00000000 */
#define CPM_DDCDR_OFFSET	(0x2c)  /* rw, 32, 0x40000000 */
#define CPM_MACCDR_OFFSET	(0x54)  /* rw  32, 0x00000000 */
#define CPM_I2SCDR_OFFSET	(0x60)  /* rw, 32, 0x00000000 */
#define CPM_I2SCDR1_OFFSET	(0x70)  /* rw, 32, 0x00000000 */
#define CPM_LPCDR_OFFSET	(0x64)  /* rw, 32, 0x00000000 */
#define CPM_MSC0CDR_OFFSET	(0x68)  /* rw, 32, 0x40000000 */
#define CPM_MSC1CDR_OFFSET	(0xa4)  /* rw, 32, 0x00000000 */
#define CPM_USBCDR_OFFSET	(0x50)  /* rw, 32, 0x00000000 */
#define CPM_SSICDR_OFFSET	(0x74)  /* rw, 32, 0x00000000 */
#define CPM_CIMCDR_OFFSET	(0x7c)  /* rw, 32, 0x00000000 */
#define CPM_PCMCDR_OFFSET	(0x84)  /* rw, 32, 0x00000000 */
#define CPM_PCMCDR1_OFFSET	(0xe0)	/* rw, 32, 0x00000000 */
#define CPM_MPHYC_OFFSET	(0xe8)  /* rw, 32, 0x40000000 */
#define CPM_INTR_OFFSET		(0xb0)  /* rw, 32, 0x00000000 */
#define CPM_INTRE_OFFSET	(0xb4)  /* rw, 32, 0x00000000 */
#define CPM_DRCG_OFFSET		(0xd0)
#define CPM_CPSPR_OFFSET	(0x34)  /* rw, 32, 0x???????? */
#define CPM_CPSPPR_OFFSET	(0x38)  /* rw, 32, 0x0000a5a5 */
#define CPM_USBPCR_OFFSET	(0x3c)  /* rw, 32, 0x429919b8 */
#define CPM_USBRDT_OFFSET	(0x40)  /* rw, 32, 0x02000096 */
#define CPM_USBVBFIL_OFFSET	(0x44)  /* rw, 32, 0x00ff0080 */
#define CPM_USBPCR1_OFFSET	(0x48)  /* rw, 32, 0x00000004 */
#define CPM_CPAPCR_OFFSET	(0x10)  /* rw, 32, 0x00000000 */
#define CPM_CPMPCR_OFFSET	(0x14)  /* rw, 32, 0x00000000 */

#define CPM_LCR_OFFSET		(0x04)  /* rw, 32, 0x000000f8 */
#define CPM_PSWC0ST_OFFSET	(0x90)  /* rw, 32, 0x00000000 */
#define CPM_PSWC1ST_OFFSET	(0x94)  /* rw, 32, 0x00000000 */
#define CPM_PSWC2ST_OFFSET	(0x98)  /* rw, 32, 0x00000000 */
#define CPM_PSWC3ST_OFFSET	(0x9c)  /* rw, 32, 0x00000000 */
#define CPM_CLKGR0_OFFSET	(0x20)  /* rw, 32, 0xffffffe0 */
#define CPM_SRBC_OFFSET		(0xc4)  /* rw, 32, 0x00000000 */
#define CPM_SLBC_OFFSET		(0xc8)  /* rw, 32, 0x00000000 */
#define CPM_SLPC_OFFSET		(0xcc)  /* rw, 32, 0x???????? */
#define CPM_OPCR_OFFSET		(0x24)  /* rw, 32, 0x000015c0 */
#define CPM_MESTSEL_OFFSET	(0xec)

/*
 * CPM registers address definition
 */
#define CPM_CPCCR	(CPM_BASE + CPM_CPCCR_OFFSET)
#define CPM_CPCSR	(CPM_BASE + CPM_CPCSR_OFFSET)
#define CPM_DDCDR	(CPM_BASE + CPM_DDCDR_OFFSET)
#define CPM_MACCDR	(CPM_BASE + CPM_MACCDR_OFFSET)
#define CPM_I2SCDR	(CPM_BASE + CPM_I2SCDR_OFFSET)
#define CPM_I2SCDR1	(CPM_BASE + CPM_I2SCDR1_OFFSET)
#define CPM_USBCDR	(CPM_BASE + CPM_USBCDR_OFFSET)
#define CPM_LPCDR	(CPM_BASE + CPM_LPCDR_OFFSET)
#define CPM_MSC0CDR	(CPM_BASE + CPM_MSC0CDR_OFFSET)
#define CPM_MSC1CDR	(CPM_BASE + CPM_MSC1CDR_OFFSET)
#define CPM_USBCDR	(CPM_BASE + CPM_USBCDR_OFFSET)
#define CPM_SSICDR	(CPM_BASE + CPM_SSICDR_OFFSET)
#define CPM_CIMCDR	(CPM_BASE + CPM_CIMCDR_OFFSET)
#define CPM_PCMCDR	(CPM_BASE + CPM_PCMCDR_OFFSET)
#define CPM_PCMCDR1	(CPM_BASE + CPM_PCMCDR1_OFFSET)
#define CPM_MPHYC	(CPM_BASE + CPM_MPHYC_OFFSET)
#define CPM_INTRCDR	(CPM_BASE + CPM_INTR_OFFSET)
#define CPM_INTRECDR	(CPM_BASE + CPM_INTRE_OFFSET)
#define CPM_DRCG	(CPM_BASE + CPM_DRCG_OFFSET)
#define CPM_CPSPR	(CPM_BASE + CPM_CPSPR_OFFSET)
#define CPM_CPSPPR	(CPM_BASE + CPM_CPSPPR_OFFSET)
#define CPM_USBPCR	(CPM_BASE + CPM_USBPCR_OFFSET)
#define CPM_USBRDT	(CPM_BASE + CPM_USBRDT_OFFSET)
#define CPM_USBVBFIL	(CPM_BASE + CPM_USBVBFIL_OFFSET)
#define CPM_USBPCR1	(CPM_BASE + CPM_USBPCR1_OFFSET)
#define CPM_CPAPCR	(CPM_BASE + CPM_CPAPCR_OFFSET)
#define CPM_CPMPCR	(CPM_BASE + CPM_CPMPCR_OFFSET)

#define CPM_LCR		(CPM_BASE + CPM_LCR_OFFSET)
#define CPM_PSWC0ST	(CPM_BASE + CPM_PSWC0ST_OFFSET)
#define CPM_PSWC1ST	(CPM_BASE + CPM_PSWC1ST_OFFSET)
#define CPM_PSWC2ST	(CPM_BASE + CPM_PSWC2ST_OFFSET)
#define CPM_PSWC3ST	(CPM_BASE + CPM_PSWC3ST_OFFSET)
#define CPM_CLKGR0	(CPM_BASE + CPM_CLKGR0_OFFSET)
#define CPM_SRBC	(CPM_BASE + CPM_SRBC_OFFSET)
#define CPM_SLBC	(CPM_BASE + CPM_SLBC_OFFSET)
#define CPM_SLPC	(CPM_BASE + CPM_SLPC_OFFSET)
#define CPM_OPCR	(CPM_BASE + CPM_OPCR_OFFSET)
#define CPM_MESTSEL	(CPM_BASE + CPM_MESTSEL_OFFSET)

/*
 * CPM registers common define
 */

/* Clock control register(CPCCR) */
#define CPCCR_SEL_SRC_LSB	30
#define CPCCR_SEL_SRC_MASK	BITS_H2L(31, CPCCR_SEL_SRC_LSB)

#define CPCCR_SEL_CPLL_LSB	28
#define CPCCR_SEL_CPLL_MASK	BITS_H2L(29, CPCCR_SEL_CPLL_LSB)

#define CPCCR_SEL_H0PLL_LSB	26
#define CPCCR_SEL_H0PLL_MASK	BITS_H2L(27, CPCCR_SEL_H0PLL_LSB)

#define CPCCR_SEL_H2PLL_LSB	24
#define CPCCR_SEL_H2PLL_MASK	BITS_H2L(25, CPCCR_SEL_H2PLL_LSB)

#define CPCCR_CE_CPU		BIT22
#define CPCCR_CE_AHB0		BIT21
#define CPCCR_CE_AHB2		BIT20
#define CPCCR_CE	(CPCCR_CE_CPU | CPCCR_CE_AHB0 | CPCCR_CE_AHB2)

#define CPCCR_PDIV_LSB		16
#define CPCCR_PDIV_MASK		BITS_H2L(19, CPCCR_PDIV_LSB)

#define CPCCR_H2DIV_LSB		12
#define CPCCR_H2DIV_MASK	BITS_H2L(15, CPCCR_H2DIV_LSB)

#define CPCCR_H0DIV_LSB		8
#define CPCCR_H0DIV_MASK	BITS_H2L(11, CPCCR_H0DIV_LSB)

#define CPCCR_L2DIV_LSB		4
#define CPCCR_L2DIV_MASK	BITS_H2L(7,  CPCCR_L2DIV_LSB)

#define CPCCR_CDIV_LSB		0
#define CPCCR_CDIV_MASK		BITS_H2L(3,  CPCCR_CDIV_LSB)

#define CPM_SRC_SEL_APLL  1   
#define CPM_PLL_SEL_SRC   1   
#define CPM_PLL_SEL_MPLL  2

/* Clock Status register(CPCSR) */
#define CPCSR_SRC_MUX		BIT31
#define CPCSR_CPU_MUX		BIT30
#define CPCSR_AHB0_MUX		BIT29
#define CPCSR_AHB2_MUX		BIT28
#define CPCSR_DDR_MUX		BIT27
#define CPCSR_H2DIV_BUSY    	BIT2
#define CPCSR_H0DIV_BUSY       	BIT1
#define CPCSR_CDIV_BUSY   	BIT0
#define CPCSR_DIV_BUSY	(CPCSR_H2DIV_BUSY | CPCSR_H0DIV_BUSY | CPCSR_CDIV_BUSY)

/* DDR clock divider register(DDCDR) */
#define DDCDR_DCS_LSB		30
#define DDCDR_DCS_MASK	      	BITS_H2L(31, DDCDR_DCS_LSB)
#define DDCDR_DCS_STOP		(0 << DDCDR_DCS_LSB)
#define DDCDR_DCS_APLL		(1 << DDCDR_DCS_LSB)
#define DDCDR_DCS_MPLL		(2 << DDCDR_DCS_LSB)
#define DDCDR_CE_DDR	       	BIT29
#define DDCDR_DDR_BUSY	       	BIT28
#define DDCDR_DDR_STOP	       	BIT27
#define DDCDR_GATE_EN		BIT26
#define DDCDR_DDR_CHANGE_EN	BIT25
#define DDCDR_DDR		BIT24
#define DDCDR_DDRDIV_LSB       	0
#define DDCDR_DDRDIV_MASK      	BITS_H2L(3, DDCDR_DDRDIV_LSB)

/*MACPHY clock divider Register (MACCDR)*/
#define MACCDR_MACPCS		BIT31
#define MACCDR_CE_MAC		BIT29
#define	MACCDR_MAC_BUSY		BIT28
#define	MACCDR_MAC_STOP		BIT27
#define	MACCDR_MACCDR_LSB	BIT0
#define	MACCDR_MACCDR_MASK	BITS_H2L(7,MACCDR_MACCDR_LSB)

/* I2S device clock divider register(I2SCDR) */
#define I2SCDR_I2PCS            BIT31
#define I2SCDR_I2CS             BIT30

#define I2SCDR_I2SDIV_M_LSB	13
#define I2SCDR_I2SDIV_M_MASK	BITS_H2L(21,I2SCDR_I2SDIV_M_LSB)
#define I2SCDR_I2SDIV_N_LSB       0       /* I2SCDR bit */
#define I2SCDR_I2SDIV_N_MASK      BITS_H2L(7, I2SCDR_I2SDIV_N_LSB)


/* I2S device clock divider register(I2SCDR1) */
#define I2SCDR1_NEN 		BIT31
#define I2SCDR1_DEN		BIT30
#define I2SCDR1_I2SDIV_D_LSB	0
#define I2SCDR1_I2SDIV_D_MASK	BITS_H2L(12,I2SCDR1_I2SDIV_D_LSB)

/* LCD pix clock divider register(LPCDR) */
#define LPCDR_LPCS_LSB		31
#define LPCDR_LPCS_APLL		(0 << LPCDR_LPCS_LSB)
#define LPCDR_LPCS_MPLL		(1 << LPCDR_LPCS_LSB)
#define LPCDR_CE_LCD            BIT28
#define LPCDR_LCD_BUSY          BIT27
#define LPCDR_LCD_STOP          BIT26

#define LPCDR_PIXDIV_LSB        0       /* LPCDR bit */
#define LPCDR_PIXDIV_MASK       BITS_H2L(7, LPCDR_PIXDIV_LSB)

/* MSC clock divider register(MSCCDR) */
#define MSCCDR_MPCS_LSB		31       /* MPCS bit */
#define MSCCDR_MPCS_APLL	(0 << MSCCDR_MPCS_LSB)
#define MSCCDR_MPCS_MPLL	(1 << MSCCDR_MPCS_LSB)

#define MSCCDR_CE_MSC           BIT29
#define MSCCDR_MSC_BUSY         BIT28
#define MSCCDR_MSC_STOP		BIT27
#define MSCCDR_S_CLK0_SEL	BIT15

#define MSCCDR_MSCDIV_LSB       0       /* MSCCDR bit */
#define MSCCDR_MSCDIV_MASK      BITS_H2L(7, MSCCDR_MSCDIV_LSB)


/* OTG PHY clock divider register(USBCDR) */
#define USBCDR_UCS              BIT31
#define USBCDR_UPCS             BIT30
#define USBCDR_CE_USB           BIT29
#define USBCDR_USB_BUSY         BIT28
#define USBCDR_USB_STOP         BIT27

#define USBCDR_OTGDIV_LSB       0       /* USBCDR bit */
#define USBCDR_OTGDIV_MASK      BITS_H2L(7, USBCDR_OTGDIV_LSB)

/* SSI clock divider register(SSICDR) */
#define SSICDR_SPCS             BIT31
#define SSICDR_SCS              BIT30
#define SSICDR_CE_SSI		BIT29	
#define	SSICDR_SSI_BUSY		BIT28
#define SSICDR_SSI_STOP		BIT27
#define SSICDR_SSIDIV_LSB       0       /* SSICDR bit */
#define SSICDR_SSIDIV_MASK      BITS_H2L(7, SSICDR_SSIDIV_LSB)

/* CIM mclk clock divider register(CIMCDR) */
#define CIMCDR_CIMPCS_APLL	(0 << 31)
#define CIMCDR_CIMPCS_MPLL	BIT31
#define CIMCDR_CE_CIM		BIT29
#define CIMCDR_CIM_BUSY		BIT28
#define CIMCDR_CIM_STOP		BIT27

#define CIMCDR_CIMDIV_LSB       0       /* CIMCDR bit */
#define CIMCDR_CIMDIV_MASK      BITS_H2L(7, CIMCDR_CIMDIV_LSB)


/* PCM device clock divider register(PCMCDR) */
#define PCMCDR_PCMPCS_LSB	30
#define PCMCDR_PCMPCS_MASK	BITS_H2L(31,PCMCDR_PCMPCS_LSB)
#define PCMCDR_PCMPCS_SCLK_A	0 << PCMCDR_PCMPCS_LSB
#define PCMCDR_PCMPCS_EXTCLK	1 << PCMCDR_PCMPCS_LSB
#define PCMCDR_PCMPCS_MPLL	2 << PCMCDR_PCMPCS_LSB
#define PCMCDR_CE_PCM		BIT29
#define PCMCDR_PCMDIV_M_LSB	13
#define PCMCDR_PCMDIV_M_MASK	BITS_H2L(21,PCMCDR_PCMDIV_M_LSB)
#define PCMCDR_PCMDIV_N_LSB	0
#define PCMCDR_PCMDIV_N_MASK	BITS_H2L(12,PCMCDR_PCMDIV_N_LSB)

/* PCM device clock divider register(PCMCDR1) */

#define PCMCDR1_PCM_NEN      	BIT31
#define PCMCDR1_PCM_DEN		BIT30
#define PCMCDR1_PCMDIV_D_LSB	0
#define PCMCDR1_PCMDIV_D_MASK	BITS_H2L(12,PCMCDR1_PCMDIV_D_LSB)

/* MAC PHY Control Register (MPHYC) */
#define MPHYC_MODE_SEL		BIT31  //useless now
#define MPHYC_MAC_SPEED_LSB	29
#define MPHYC_MAC_SPEED_MASK	BITS_H2L(30,MPHYC_MAC_SPEED_LSB)
#define MPHYC_SOFT_RST		BIT3
#define	MPHYC_PHY_INTF_LSB	0
#define MPHYC_PHY_INTF_MASK	BITS_H2L(2,MPHYC_PHY_INTF_MASK) //useless now

/* CPM Interrupt Register (CPM_INTR)*/
#define CPM_INTR_VBUS_INTR	BIT1
#define CPM_INTR_ADEV_INTR	BIT0

/* CPM Interrupt Enable Register (CPM_INTRE)*/
#define CPM_INTRE_VBUS_INTRE	BIT1
#define CPM_INTRE_ADEV_INTRE	BIT0

/* CPM scratch pad protected register(CPSPPR) */
#define CPSPPR_CPSPR_WRITABLE   (0x00005a5a)

/* OTG parameter control register(USBPCR) */
#define USBPCR_USB_MODE         BIT31
#define USBPCR_AVLD_REG         BIT30
#define USBPCR_INCRM            BIT27   /* INCR_MASK bit */
#define USBPCR_TXRISE_TUNE    	BIT26
#define USBPCR_COMMONONN        BIT25
#define USBPCR_VBUSVLDEXT       BIT24
#define USBPCR_VBUSVLDEXTSEL    BIT23
#define USBPCR_POR              BIT22
#define USBPCR_SIDDQ            BIT21
#define USBPCR_OTG_DISABLE      BIT20
#define USBPCR_TXPREEMPHTUNE    BIT6

#define USBPCR_IDPULLUP_LSB             28   /* IDPULLUP_MASK bit */
#define USBPCR_IDPULLUP_MASK            BITS_H2L(29, USBPCR_IDPULLUP_LSB)

#define USBPCR_COMPDISTUNE_LSB          17
#define USBPCR_COMPDISTUNE_MASK         BITS_H2L(19, USBPCR_COMPDISTUNE_LSB)

#define USBPCR_OTGTUNE_LSB              14
#define USBPCR_OTGTUNE_MASK             BITS_H2L(16, USBPCR_OTGTUNE_LSB)

#define USBPCR_SQRXTUNE_LSB             11
#define USBPCR_SQRXTUNE_MASK            BITS_H2L(13, USBPCR_SQRXTUNE_LSB)

#define USBPCR_TXFSLSTUNE_LSB           7
#define USBPCR_TXFSLSTUNE_MASK          BITS_H2L(10, USBPCR_TXFSLSTUNE_LSB)

#define USBPCR_TXRISETUNE_LSB           4
#define USBPCR_TXRISETUNE_MASK          BITS_H2L(5, USBPCR_TXRISETUNE_LSB)

#define USBPCR_TXVREFTUNE_LSB           0
#define USBPCR_TXVREFTUNE_MASK          BITS_H2L(3, USBPCR_TXVREFTUNE_LSB)

/* OTG reset detect timer register(USBRDT) */
#define	USBRDT_HB_MASK		BIT26
#define USBRDT_VBFIL_LD_EN      BIT25
#define USBRDT_IDDIG_EN         BIT24
#define USBRDT_IDDIG_REG        BIT23

#define USBRDT_USBRDT_LSB       0
#define USBRDT_USBRDT_MASK      BITS_H2L(22, USBRDT_USBRDT_LSB)

/* OTG parameter control register(USBPCR1) */
#define USBPCR1_REG			BIT31
#define USBPCR1_USB_SEL      		BIT28
#define USBPCR1_REFCLKSEL_LSB		26
#define USBPCR1_REFCLKSEL_MASK		BITS_H2L(27, USBPCR1_REFCLKSEL_LSB)

#define USBPCR1_REFCLKDIV_LSB		24
#define USBPCR1_REFCLKDIV_MASK		BITS_H2L(25, USBPCR1_REFCLKDIV_LSB)

#define USBPCR1_PORT_RST		BIT21

#define USBPCR1_WORD_IF0		BIT19
#define USBPCR1_WORD_IF1        	BIT18


/* APLL control register (CPXPCR) */
#define CPAPCR_BS		BIT31
#define CPAPCR_M_LSB	     	24
#define CPAPCR_M_MASK	     	BITS_H2L(30, CPAPCR_M_LSB)

#define CPAPCR_N_LSB	     	18
#define CPAPCR_N_MASK	     	BITS_H2L(22, CPAPCR_N_LSB)

#define CPAPCR_OD_LSB	     	16
#define CPAPCR_OD_MASK	     	BITS_H2L(17, CPAPCR_OD_LSB)

#define CPAPCR_LOCK		BIT15   /* LOCK bit */
#define CPAPCR_ON		BIT10
#define CPAPCR_BP		BIT9
#define CPAPCR_EN		BIT8
#define CPAPCR_PLLST_LSB	0
#define CPAPCR_PLLST_MASK	BITS_H2L(7,CPAPCR_PLLST_LSB)

#define CPM_CPAPCR_EN		CPAPCR_EN
#define CPM_CPAPCR_ON		CPAPCR_ON

/* MPLL control register (CPXPCR) */
#define CPMPCR_BS		BIT31
#define CPMPCR_M_LSB	     	24
#define CPMPCR_M_MASK	     	BITS_H2L(30, CPAPCR_M_LSB)

#define CPMPCR_N_LSB	     	18
#define CPMPCR_N_MASK	     	BITS_H2L(22, CPAPCR_N_LSB)

#define CPMPCR_OD_LSB	     	16
#define CPMPCR_OD_MASK	     	BITS_H2L(17, CPAPCR_OD_LSB)

#define CPMPCR_EN		BIT7
#define CPMPCR_BP		BIT6
#define CPMPCR_LOCK		BIT1	/* LOCK bit */
#define CPMPCR_ON		BIT0

#define CPM_CPMPCR_EN		CPMPCR_EN
#define CPM_CPMPCR_ON		CPMPCR_ON



/* Low power control register(LCR) */
#define LCR_PST_LSB		8
#define	LCD_PST_MASK		BITS_H2L(19,LCR_PST_LSB)
#define LCR_LPM_LSB		0
#define	LCR_LPM_MASK		BITS_H2L(1,LCR_LPM_LSB)

/* Clock gate register 0(CGR0) */
#define CLKGR0_DDR              BIT31
#define CLKGR0_CPU		BIT30
#define CLKGR0_AHB0		BIT29
#define CLKGR0_APB0		BIT28
#define CLKGR0_RTC	        BIT27
#define CLKGR0_PCM              BIT26
#define CLKGR0_MAC              BIT25
#define CLKGR0_AES              BIT24
#define CLKGR0_LCD              BIT23
#define CLKGR0_CIM              BIT22
#define CLKGR0_PDMA             BIT21
#define CLKGR0_OST              BIT20
#define CLKGR0_SSI              BIT19
#define CLKGR0_TCU              BIT18
#define CLKGR0_DMIC             BIT17
#define CLKGR0_UART2            BIT16
#define CLKGR0_UART1            BIT15
#define CLKGR0_UART0            BIT14
#define CLKGR0_SADC             BIT13
#define CLKGR0_JPEG             BIT12
#define CLKGR0_AIC              BIT11
#define CLKGR0_I2C3		BIT10
#define CLKGR0_I2C2             BIT9
#define CLKGR0_I2C1             BIT8
#define CLKGR0_I2C0		BIT7
#define CLKGR0_SCC		BIT6
#define CLKGR0_MSC1		BIT5
#define CLKGR0_MSC0             BIT4
#define CLKGR0_OTG              BIT3
#define CLKGR0_SFC              BIT2
#define CLKGR0_EFUSE            BIT1
#define CLKGR0_NEMC             BIT0

/* CPM MEST SEL Register */

#define MEST_SEL_TST8		BIT8
#define MEST_SEL_TST7		BIT7
#define MEST_SEL_TST4		BIT4
#define MEST_SEL_TST3		BIT3
#define MEST_SEL_TST1		BIT1
#define MEST_SEL_TST0		BIT0

/*Soft Reset and Bus Control Register (SRBC)*/

#define SRBC_JPEG_SR		BIT31
#define SRBC_JPEG_STP		BIT30
#define SRBC_JPEG_ACK		BIT29
#define	SRBC_LCD_SR		BIT25
#define	SRBC_LCD_STP		BIT24
#define	SRBC_LCD_ACK		BIT23
#define	SRBC_CIM_STP		BIT21
#define	SRBC_CIM_ACK		BIT20
#define	SRBC_CPU_STP		BIT15
#define	SRBC_CPU_ACK		BIT14
#define	SRBC_OTG_SR		BIT12
#define	SRBC_AHB2_STP		BIT8
#define	SRBC_AHB2_ACK		BIT7
#define	SRBC_DDR_STP		BIT6
#define	SRBC_DDR_ACK		BIT5


/* Oscillator and power control register(OPCR) */
#define OPCR_IDLE_DIS           BIT31
#define OPCR_MASK_INT	        BIT30
#define OPCR_MASK_VPU		BIT29  //ONLY FOR DEBUG
#define OPCR_GATE_SCLK_ABUS	BIT28
#define OPCR_L2C_PD		BIT25
#define OPCR_REQ_MODE		BIT24
#define OPCR_GATE_USBPHY_CLK	BIT23
#define OPCR_DIS_STOP_MUX	BIT22
#define OPCR_O1ST_LSB           8
#define OPCR_O1ST_MASK          BITS_H2L(19, OPCR_O1ST_LSB)
#define OPCR_OTGPHY0_ENABLE    BIT7    /* otg */
#define OPCR_OTGPHY1_ENABLE    BIT6    /* uhc */
#define OPCR_USBPHY_ENABLE     (OPCR_OTGPHY0_ENABLE | OPCR_OTGPHY1_ENABLE)
#define OPCR_O1SE               BIT4
#define OPCR_PD                 BIT3
#define OPCR_ERCS               BIT2
#define OPCR_BUSMODE		BIT1




/* Reset status register(RSR) */
#define RSR_HR			BIT3
#define RSR_P0R         	BIT2
#define RSR_WR          	BIT1
#define RSR_PR          	BIT0








#ifndef __MIPS_ASSEMBLER

#define REG_CPM_CPCCR           REG32(CPM_CPCCR)
#define REG_CPM_CPCSR           REG32(CPM_CPCSR)
#define REG_CPM_DDCDR           REG32(CPM_DDCDR)
#define REG_CPM_MACCDR		REG32(CPM_MACCDR)
#define REG_CPM_I2SCDR          REG32(CPM_I2SCDR)
#define REG_CPM_I2SCDR1         REG32(CPM_I2SCDR1)
#define REG_CPM_LPCDR          	REG32(CPM_LPCDR)
#define REG_CPM_MSC0CDR         REG32(CPM_MSC0CDR)
#define REG_CPM_MSC1CDR         REG32(CPM_MSC1CDR)
#define REG_CPM_USBCDR          REG32(CPM_USBCDR)
#define REG_CPM_SSICDR          REG32(CPM_SSICDR)
#define REG_CPM_CIMCDR          REG32(CPM_CIMCDR)
#define REG_CPM_PCMCDR          REG32(CPM_PCMCDR)
#define REG_CPM_PCMCDR1         REG32(CPM_PCMCDR1)
#define REG_CPM_MPHYC         	REG32(CPM_MPHYC)
#define REG_CPM_INTRCDR         REG32(CPM_INTRCDR)
#define REG_CPM_INTRECDR        REG32(CPM_INTRECDR)
#define REG_CPM_CPSPR           REG32(CPM_CPSPR)
#define REG_CPM_CPSPPR          REG32(CPM_CPSPPR)
#define REG_CPM_USBPCR          REG32(CPM_USBPCR)
#define REG_CPM_USBRDT          REG32(CPM_USBRDT)
#define REG_CPM_USBVBFIL        REG32(CPM_USBVBFIL)
#define REG_CPM_USBPCR1         REG32(CPM_USBPCR1)
#define REG_CPM_CPAPCR          REG32(CPM_CPAPCR)
#define REG_CPM_CPMPCR          REG32(CPM_CPMPCR)

#define REG_CPM_LCR             REG32(CPM_LCR)
#define REG_CPM_PSWC0ST         REG32(CPM_PSWC0ST)
#define REG_CPM_PSWC1ST         REG32(CPM_PSWC1ST)
#define REG_CPM_PSWC2ST         REG32(CPM_PSWC2ST)
#define REG_CPM_PSWC3ST         REG32(CPM_PSWC3ST)
#define REG_CPM_CLKGR0          REG32(CPM_CLKGR0)
#define REG_CPM_SRBC            REG32(CPM_SRBC)
#define REG_CPM_SLBC            REG32(CPM_SLBC)
#define REG_CPM_SLPC            REG32(CPM_SLPC)
#define REG_CPM_OPCR            REG32(CPM_OPCR)
#define REG_CPM_RSR             REG32(CPM_RSR)

#define _REG_CPM_MSCCDR(n)	REG_CPM_MSC##n##CDR
#define REG_CPM_MSCCDR(n)	_REG_CPM_MSCCDR(n)

typedef enum {
	SCLK_APLL = 0,
	SCLK_MPLL,
} src_clk;

typedef enum {
	CGM_NEMC  	= 0,
	CGM_EFUSE   	= 1,
	CGM_SFC   	= 2,
	CGM_OTG  	= 3,
	CGM_MSC0  	= 4,
	CGM_MSC1 	= 5,
	CGM_SCC 	= 6,
	CGM_I2C0  	= 7,
	CGM_I2C1        = 8,
	CGM_I2C2        = 9,
	CGM_I2C3     	= 10,
	CGM_AIC	  	= 11,
	CGM_JPEG	= 12,
	CGM_SADC	= 13,
	CGM_UART0	= 14,
	CGM_UART1       = 15,
	CGM_UART2 	= 16,
	CGM_DMIC 	= 17,
	CGM_TCU 	= 18,
	CGM_SSI	 	= 19,
	CGM_OST		= 20,
	CGM_PDMA	= 21,
	CGM_CIM	   	= 22,
	CGM_LCD		= 23,
	CGM_AES		= 24,
	CGM_MAC	        = 25,
	CGM_PCM	        = 26,
	CGM_RTC	        = 27,
	CGM_APB0	= 28,
	CGM_AHB0	= 29,
	CGM_CPU		= 30,
	CGM_DDR		= 31,
	CGM_ALL_MODULE,
} clock_gate_module;

typedef enum __CLKDIV
{
	CPM_CDIV 	= CPCCR_CDIV_LSB,
	CPM_H0DIV 	= CPCCR_H0DIV_LSB,
	CPM_PDIV 	= CPCCR_PDIV_LSB,
	CPM_LC2DIV 	= CPCCR_L2DIV_LSB,
	CPM_H2DIV 	= CPCCR_H2DIV_LSB,
} CLKDIV, *PCLKDIV;

#define __CGU_CLOCK_BASE__      0x1000

typedef enum {
	/* Clock source is pll0 */
	CGU_CCLK = __CGU_CLOCK_BASE__ + 0,
	CGU_L2CLK,
	CGU_HCLK,
	CGU_H2CLK,
	CGU_PCLK,

	/* Clock source is apll or mpll */
	CGU_MCLK,
	CGU_MSC0CLK,
	CGU_MSC1CLK,
	CGU_CIMCLK,

	/* Clock source is apll, mpll or exclk */
	CGU_SSICLK,
	CGU_OTGCLK,

	/* Clock source is apll, mpll or vpll */
	CGU_LPCLK,

	/* Clock source is apll, mpll or epll */
	CGU_VPUCLK,
	CGU_GPUCLK,

	/* Clock source is apll, epll or exclk */
	CGU_I2SCLK,

	/* Clock source is apl, mpll, vpll, epll or exclk*/
	CGU_PCMCLK,

	/* Clock source is apll, mpll, epll or otg phy  */
	CGU_UHCCLK,

	/* Clock source is exclk */
	CGU_UARTCLK,
	CGU_SADCCLK,
	CGU_TCUCLK,

	/* Clock source is external rtc clock */
	CGU_RTCCLK,

	CGU_CLOCK_MAX,
} cgu_clock;

/*
 * X1000 clocks structure
 */
typedef struct {
	unsigned int cclk;	/* CPU clock				*/
	unsigned int l2clk;	/* L2Cache clock			*/
	unsigned int hclk;	/* System bus clock: AHB0		*/
	unsigned int h2clk;	/* System bus clock: AHB2		*/
	unsigned int pclk;	/* Peripheral bus clock			*/
	unsigned int mclk;	/* DDR controller clock			*/
	unsigned int vpuclk;	/* VPU controller clock			*/
	unsigned int sclk;	/* NEMC controller clock		*/
	unsigned int cko;	/* SDRAM or DDR clock			*/
	unsigned int pixclk0;	/* LCD0 pixel clock			*/
	unsigned int pixclk1;	/* LCD1 pixel clock			*/
	unsigned int tveclk;	/* TV encoder 27M  clock		*/
	unsigned int cimmclk;	/* Clock output from CIM module		*/
	unsigned int cimpclk;	/* Clock input to CIM module		*/
	unsigned int gpuclk;	/* GPU clock				*/
	unsigned int gpsclk;	/* GPS clock				*/
	unsigned int i2sclk;	/* I2S codec clock			*/
	unsigned int bitclk;	/* AC97 bit clock			*/
	unsigned int pcmclk;	/* PCM clock				*/
	unsigned int msc0clk;	/* MSC0 clock				*/
	unsigned int msc1clk;	/* MSC1 clock				*/
	unsigned int msc2clk;	/* MSC2 clock				*/
	unsigned int ssiclk;	/* SSI clock				*/
	unsigned int tssiclk;	/* TSSI clock				*/
	unsigned int otgclk;	/* USB OTG clock			*/
	unsigned int uhcclk;	/* USB UHCI clock			*/
	unsigned int extalclk;	/* EXTAL clock for
				   UART,I2C,TCU,USB2.0-PHY,AUDIO CODEC	*/
	unsigned int rtcclk;	/* RTC clock for CPM,INTC,RTC,TCU,WDT	*/
} jz_clocks_t;

void cpm_start_clock(clock_gate_module module_name);
void cpm_stop_clock(clock_gate_module module_name);

unsigned int cpm_set_clock(cgu_clock clock_name, unsigned int clock_hz);
unsigned int cpm_get_clock(cgu_clock clock_name);
unsigned int cpm_get_xpllout(src_clk);

void cpm_uhc_phy(unsigned int en);

/**************remove me if android's kernel support these operations********start*********  */

#define cpm_get_pllout()	cpm_get_xpllout(SCLK_APLL)
#define cpm_get_pllout1()	cpm_get_xpllout(SCLK_MPLL)

static __inline__ unsigned int __cpm_get_pllout2(void)
{
#if defined(CONFIG_FPGA)
	return JZ_EXTAL;
#else
	return cpm_get_xpllout(SCLK_APLL);
#endif
}

/* EXTAL clock for UART,I2C,SSI,SADC,USB-PHY */
static __inline__ unsigned int __cpm_get_extalclk(void)
{
	return JZ_EXTAL;
}

/* RTC clock for CPM,INTC,RTC,TCU,WDT */
static __inline__ unsigned int __cpm_get_rtcclk(void)
{
	return JZ_EXTAL2;
}

extern jz_clocks_t jz_clocks;

#define __cpm_enable_pll_change()	(REG_CPM_CPCCR |= CPCCR_CE)

#define __cpm_suspend_otg_phy()		(REG_CPM_OPCR &= ~OPCR_OTGPHY0_ENABLE)
#define __cpm_enable_otg_phy()		(REG_CPM_OPCR |=  OPCR_OTGPHY0_ENABLE)

#define __cpm_suspend_uhc_phy()		(REG_CPM_USBPCR1 &= ~(1 << 17))
#define __cpm_enable_uhc_phy()		(REG_CPM_USBPCR1 |=  (1 << 17))

#define __cpm_disable_osc_in_sleep()	(REG_CPM_OPCR &= ~OPCR_O1SE)
#define __cpm_select_rtcclk_rtc()	(REG_CPM_OPCR |= OPCR_ERCS)

#define __cpm_get_cdiv() \
	((REG_CPM_CPCCR & CPCCR_CDIV_MASK) >> CPCCR_CDIV_LSB)
#define __cpm_get_hdiv() \
	((REG_CPM_CPCCR & CPCCR_H0DIV_MASK) >> CPCCR_H0DIV_LSB)
#define __cpm_get_h2div() \
	((REG_CPM_CPCCR & CPCCR_H2DIV_MASK) >> CPCCR_H2DIV_LSB)
#define __cpm_get_otgdiv() \
	((REG_CPM_USBCDR & USBCDR_OTGDIV_MASK) >> USBCDR_OTGDIV_LSB)
#define __cpm_get_pdiv() \
	((REG_CPM_CPCCR & CPCCR_PDIV_MASK) >> CPCCR_PDIV_LSB)

#define __cpm_get_mdiv() __cpm_get_hdiv()

#define __cpm_get_pixdiv() \
	((REG_CPM_LPCDR & LPCDR_PIXDIV_MASK) >> LPCDR_PIXDIV_LSB)
#define __cpm_get_ssidiv() \
	((REG_CPM_SSICDR & SSICDR_SSIDIV_MASK) >> SSICDR_SSIDIV_LSB)

#define __cpm_set_ssidiv(v) \
	(REG_CPM_SSICDR = (REG_CPM_SSICDR & ~SSICDR_SSIDIV_MASK) | ((v) << (SSICDR_SSIDIV_LSB)))

#define cpm_get_scrpad()	INREG32(CPM_CPSPR)
#define cpm_set_scrpad(data)				\
do {							\
	OUTREG32(CPM_CPSPPR, CPSPPR_CPSPR_WRITABLE);	\
	OUTREG32(CPM_CPSPR, data);			\
	OUTREG32(CPM_CPSPPR, ~CPSPPR_CPSPR_WRITABLE);	\
} while (0)

/* Clock Gate Register */
#define CPM_CLKGR0_DDR		(1 << 31)
#define CPM_CLKGR0_CPU		(1 << 30)
#define CPM_CLKGR0_AHB0		(1 << 29)
#define CPM_CLKGRO_APB0		(1 << 28)
#define CPM_CLKGR0_RTC  	(1 << 27)
#define CPM_CLKGR0_PCM    	(1 << 26)
#define CPM_CLKGR0_MAC    	(1 << 25)
#define CPM_CLKGR0_AES    	(1 << 24)
#define CPM_CLKGR0_LCD    	(1 << 23)
#define CPM_CLKGR0_CIM    	(1 << 22)
#define CPM_CLKGR0_PDMA    	(1 << 21)
#define CPM_CLKGR0_OST    	(1 << 20)
#define CPM_CLKGR0_SSI    	(1 << 19)
#define CPM_CLKGR0_TCU    	(1 << 18)
#define CPM_CLKGR0_DIMC    	(1 << 17)
#define CPM_CLKGR0_UART2    	(1 << 16)
#define CPM_CLKGR0_UART1	(1 << 15)
#define CPM_CLKGR0_UART0	(1 << 14)
#define CPM_CLKGR0_SADC		(1 << 13)
#define CPM_CLKGR0_JPEG		(1 << 12)
#define CPM_CLKGR0_AIC		(1 << 11)
#define CPM_CLKGR0_I2C3		(1 << 10)
#define CPM_CLKGR0_I2C2		(1 << 9)
#define CPM_CLKGR0_I2C1		(1 << 8)
#define CPM_CLKGR0_I2C0		(1 << 7)
#define CPM_CLKGR0_SCC		(1 << 6)
#define CPM_CLKGR0_MSC1		(1 << 5)
#define CPM_CLKGR0_MSC0		(1 << 4)
#define CPM_CLKGR0_OTG		(1 << 3)
#define CPM_CLKGR0_SFC		(1 << 2)
#define CPM_CLKGR0_EFUSE	(1 << 1)
#define CPM_CLKGR0_NEMC		(1 << 0)

#define __cpm_stop_all() 	\
	do {\
		(REG_CPM_CLKGR0 = 0xffffffff);\
	}while(0)
#define __cpm_stop_ddr()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_DDR)
#define __cpm_stop_cpu()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_CPU)
#define __cpm_stop_ahb0()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_AHB0)
#define __cpm_stop_apb0()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_APB0)
#define __cpm_stop_rtc()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_RTC)
#define __cpm_stop_pcm()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_PCM)
#define __cpm_stop_mac()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_MAC)
#define __cpm_stop_aes()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_AES)
#define __cpm_stop_lcd()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_LCD)
#define __cpm_stop_cim()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_CIM)
#define __cpm_stop_pdma()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_PDMA)
#define __cpm_stop_ost()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_OST)
#define __cpm_stop_ssi()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_SSI)
#define __cpm_stop_tcu()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_TCU)
#define __cpm_stop_dmic()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_DMIC)
#define __cpm_stop_uart2()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_UART2)
#define __cpm_stop_uart1()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_UART1)
#define __cpm_stop_uart0()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_UART0)
#define __cpm_stop_sadc()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_SADC)
#define __cpm_stop_jpeg()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_JPEG)
#define __cpm_stop_aic()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_AIC)
#define __cpm_stop_i2c3()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_I2C3)
#define __cpm_stop_i2c2()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_I2C2)
#define __cpm_stop_i2c1()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_I2C1)
#define __cpm_stop_i2c0()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_I2C0)
#define __cpm_stop_scc()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_SCC)
#define __cpm_stop_msc1()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_MSC1)
#define __cpm_stop_msc0()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_MSC0)
#define __cpm_stop_otg()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_OTG)
#define __cpm_stop_sfc()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_SFC)
#define __cpm_stop_efuse()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_EFUSE)
#define __cpm_stop_nemc()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_NEMC)
#define __cpm_stop_dmac()	__cpm_stop_pdma()

#define __cpm_stop_uart(n)	__cpm_stop_uart##n()

#define __cpm_start_all() 	\
	do {\
		REG_CPM_CLKGR0 = 0x0;\
	} while(0)

#define __cpm_start_ddr()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_DDR)
#define __cpm_start_cpu()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_CPU)
#define __cpm_start_ahb0()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_AHB0)
#define __cpm_start_apb0()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_APB0)
#define __cpm_start_rtc()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_RTC)
#define __cpm_start_pcm()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_PCM)
#define __cpm_start_mac()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_MAC)
#define __cpm_start_aes()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_AES)
#define __cpm_start_lcd()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_LCD)
#define __cpm_start_cim()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_CIM)
#define __cpm_start_pdma()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_PDMA)
#define __cpm_start_ost()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_OST)
#define __cpm_start_ssi()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_SSI)
#define __cpm_start_tcu()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_TCU)
#define __cpm_start_dmic()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_DMIC)
#define __cpm_start_uart2()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_UART2)
#define __cpm_start_uart1()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_UART1)
#define __cpm_start_uart0()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_UART0)

#define __cpm_start_sadc()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_SADC)
#define __cpm_start_jpeg()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_JPEG)
#define __cpm_start_aic()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_AIC)
#define __cpm_start_i2c3()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_I2C3)
#define __cpm_start_i2c2()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_I2C2)
#define __cpm_start_i2c1()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_I2C1)
#define __cpm_start_i2c0()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_I2C0)
#define __cpm_start_scc()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_SCC)
#define __cpm_start_msc1()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_MSC1)
#define __cpm_start_msc0()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_MSC0)
#define __cpm_start_otg()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_OTG)
#define __cpm_start_sfc()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_SFC)
#define __cpm_start_efuse()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_EFUSE)
#define __cpm_start_nemc()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_NEMC)

#define cpm_pdma_started()	((REG_CPM_CLKGR0 & CPM_CLKGR0_PDMA) ? 0 : 1)

#define OPCR_OTGPHY_ENABLE      BIT7    /* SPENDN bit */
#define LPCDR_LPCS              BIT31

#define __cpm_start_uart(n)	__cpm_start_uart##n()

#endif /* __MIPS_ASSEMBLER */

#endif /* __X1000CPM_H__ */

