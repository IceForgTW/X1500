/*
 * linux/include/asm-mips/mach-jz4770/jz4770cim.h
 *
 * JZ4770 CIM register definition.
 *
 * Copyright (C) 2010 Ingenic Semiconductor Co., Ltd.
 */

#ifndef __JZ4775CIM_H__
#define __JZ4775CIM_H__


#define	CIM_BASE0	0xB3060000
#define	CIM_BASE1	0xB30E0000

/*************************************************************************
 * CIM
 *************************************************************************/
#define	CIM_CFG(n)		(CIM_BASE##n + 0x0000)
#define	CIM_CTRL(n)		(CIM_BASE##n + 0x0004)
#define	CIM_STATE(n)		(CIM_BASE##n + 0x0008)
#define	CIM_IID(n)		(CIM_BASE##n + 0x000C)
//#define	CIM_RXFIFO(n)		(CIM_BASE + 0x0010)
#define	CIM_DA(n)		(CIM_BASE##n + 0x0020)
#define	CIM_FA(n)		(CIM_BASE##n + 0x0024)
#define	CIM_FID(n)		(CIM_BASE##n + 0x0028)
#define	CIM_CMD(n)		(CIM_BASE##n + 0x002C)
#define	CIM_SIZE(n)		(CIM_BASE##n + 0x0030)
#define	CIM_OFFSET(n)		(CIM_BASE##n + 0x0034)
#define CIM_YFA(n)		(CIM_BASE##n + 0x0038)
#define CIM_YCMD(n)		(CIM_BASE##n + 0x003C)
#define CIM_CBFA(n)		(CIM_BASE##n + 0x0040)
#define CIM_CBCMD(n)		(CIM_BASE##n + 0x0044)
#define CIM_CRFA(n)		(CIM_BASE##n + 0x0048)
#define CIM_CRCMD(n)		(CIM_BASE##n + 0x004C)
#define CIM_CTRL2(n)		(CIM_BASE##n + 0x0050)
#define CIM_FS(n) 		(CIM_BASE##n + 0X0054)
#define CIM_IMR(n) 		(CIM_BASE##n + 0X0058)
#define CIM_TC(n) 		(CIM_BASE##n + 0X005C)
#define CIM_TINX(n) 		(CIM_BASE##n + 0X0060)
#define CIM_TCNT(n) 		(CIM_BASE##n + 0X0064)

//#define	CIM_RAM_ADDR		(CIM_BASE + 0x1000)

#define	REG_CIM_CFG(n)		REG32(CIM_CFG(n))
#define	REG_CIM_CTRL(n)		REG32(CIM_CTRL(n))
#define REG_CIM_CTRL2(n)	REG32(CIM_CTRL2(n))
#define	REG_CIM_STATE(n)	REG32(CIM_STATE(n))
#define	REG_CIM_IID(n)		REG32(CIM_IID(n))
//#define	REG_CIM_RXFIFO		REG32(CIM_RXFIFO)
#define	REG_CIM_DA(n)		REG32(CIM_DA(n))
#define	REG_CIM_FA(n)		REG32(CIM_FA(n))
#define	REG_CIM_FID(n)		REG32(CIM_FID(n))
#define	REG_CIM_CMD(n)		REG32(CIM_CMD(n))
#define	REG_CIM_SIZE(n)		REG32(CIM_SIZE(n))
#define	REG_CIM_OFFSET(n)	REG32(CIM_OFFSET(n))
#define REG_CIM_YFA(n)		REG32(CIM_YFA(n))
#define REG_CIM_YCMD(n)		REG32(CIM_YCMD(n))
#define REG_CIM_CBFA(n)		REG32(CIM_CBFA(n))
#define REG_CIM_CBCMD(n)	REG32(CIM_CBCMD(n))
#define REG_CIM_CRFA(n)		REG32(CIM_CRFA(n))
#define REG_CIM_CRCMD(n)	REG32(CIM_CRCMD(n))
#define REG_CIM_FS(n)		REG32(CIM_FS(n))
#define REG_CIM_IMR(n)		REG32(CIM_IMR(n))
#define REG_CIM_TC(n)		REG32(CIM_TC(n))
#define REG_CIM_TINX(n)		REG32(CIM_TINX(n))
#define REG_CIM_TCNT(n)		REG32(CIM_TCNT(n))

#define CIM_CFG_EXPAND_MODE	BIT30
#define CIM_CFG_BW_LSB		22
#define CIM_CFG_BW_MASK		BITS_H2L(23, 22)
#define __cim_set_bus_width(n,bw)						\
	do {								\
		unsigned int ___old_cfg;				\
									\
		BUG_ON((bw) > 12);					\
									\
		___old_cfg = REG_CIM_CFG(n);				\
									\
		if ((bw) > 8) {						\
			___old_cfg |= CIM_CFG_EXPAND_MODE;		\
			___old_cfg &= ~CIM_CFG_BW_MASK;			\
			___old_cfg |= (((bw) - 9) << CIM_CFG_BW_LSB);	\
		} else {						\
			___old_cfg &= ~CIM_CFG_EXPAND_MODE;		\
		}							\
									\
		REG_CIM_CFG(n) = ___old_cfg;				\
	} while(0)
/*cim configuration register (CIMCFG)*/
#define CIM_CFG_EEOFEN		(1 << 31)
#define CIM_CFG_EXP		(1 << 30)
#define CIM_CFG_SEP		(1 << 20)
#define	CIM_CFG_ORDER_BIT	18
#define	CIM_CFG_ORDER_MASK	(0x3 << CIM_CFG_ORDER_BIT)
  #define CIM_CFG_ORDER_0	  (0x0 << CIM_CFG_ORDER_BIT) 	/* Y0CbY1Cr; YCbCr */
  #define CIM_CFG_ORDER_1	  (0x1 << CIM_CFG_ORDER_BIT)	/* Y0CrY1Cb; YCrCb */
  #define CIM_CFG_ORDER_2	  (0x2 << CIM_CFG_ORDER_BIT)	/* CbY0CrY1; CbCrY */
  #define CIM_CFG_ORDER_3	  (0x3 << CIM_CFG_ORDER_BIT)	/* CrY0CbY1; CrCbY */
#define	CIM_CFG_DF_BIT		16
#define	CIM_CFG_DF_MASK		  (0x3 << CIM_CFG_DF_BIT)
  #define CIM_CFG_DF_YUV444	  (0x1 << CIM_CFG_DF_BIT) 	/* YCbCr444 */
  #define CIM_CFG_DF_YUV422	  (0x2 << CIM_CFG_DF_BIT)	/* YCbCr422 */
  #define CIM_CFG_DF_ITU656	  (0x3 << CIM_CFG_DF_BIT)	/* ITU656 YCbCr422 */
#define	CIM_CFG_INV_DAT		(1 << 15)
#define	CIM_CFG_VSP		(1 << 14) /* VSYNC Polarity:0-rising edge active,1-falling edge active */
#define	CIM_CFG_HSP		(1 << 13) /* HSYNC Polarity:0-rising edge active,1-falling edge active */
#define	CIM_CFG_PCP		(1 << 12) /* PCLK working edge: 0-rising, 1-falling */
#define	CIM_CFG_DMA_BURST_TYPE_BIT	10
#define	CIM_CFG_DMA_BURST_TYPE_MASK	(0x3 << CIM_CFG_DMA_BURST_TYPE_BIT)
  #define	CIM_CFG_DMA_BURST_INCR4		(0 << CIM_CFG_DMA_BURST_TYPE_BIT)
  #define	CIM_CFG_DMA_BURST_INCR8		(1 << CIM_CFG_DMA_BURST_TYPE_BIT)	/* Suggested */
  #define	CIM_CFG_DMA_BURST_INCR16	(2 << CIM_CFG_DMA_BURST_TYPE_BIT)	/* Suggested High speed AHB*/
  #define	CIM_CFG_DMA_BURST_INCR32	(3 << CIM_CFG_DMA_BURST_TYPE_BIT)	/* Suggested High speed AHB*/
#define	CIM_CFG_DUMMY_ZERO	(1 << 9)
#define	CIM_CFG_EXT_VSYNC	(1 << 8)	/* Only for ITU656 Progressive mode */
#define CIM_CFG_LM		(1 << 7) /*Line mode for ITU656*/
#define	CIM_CFG_PACK_BIT	4
#define	CIM_CFG_PACK_MASK	(0x7 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_0	  (0 << CIM_CFG_PACK_BIT) /* 11 22 33 44 0xY0CbY1Cr */
  #define CIM_CFG_PACK_1	  (1 << CIM_CFG_PACK_BIT) /* 22 33 44 11 0xCbY1CrY0 */
  #define CIM_CFG_PACK_2	  (2 << CIM_CFG_PACK_BIT) /* 33 44 11 22 0xY1CrY0Cb */
  #define CIM_CFG_PACK_3	  (3 << CIM_CFG_PACK_BIT) /* 44 11 22 33 0xCrY0CbY1 */
  #define CIM_CFG_PACK_4	  (4 << CIM_CFG_PACK_BIT) /* 44 33 22 11 0xCrY1CbY0 */
  #define CIM_CFG_PACK_5	  (5 << CIM_CFG_PACK_BIT) /* 33 22 11 44 0xY1CbY0Cr */
  #define CIM_CFG_PACK_6	  (6 << CIM_CFG_PACK_BIT) /* 22 11 44 33 0xCbY0CrY1 */
  #define CIM_CFG_PACK_7	  (7 << CIM_CFG_PACK_BIT) /* 11 44 33 22 0xY0CrY1Cb */
#define CIM_CFG_FP		(1 << 3)
#define	CIM_CFG_BYPASS_BIT	2
#define	CIM_CFG_BYPASS_MASK	(1 << CIM_CFG_BYPASS_BIT)
  #define CIM_CFG_BYPASS	  (1 << CIM_CFG_BYPASS_BIT)
#define	CIM_CFG_DSM_BIT		0
#define	CIM_CFG_DSM_MASK	(0x3 << CIM_CFG_DSM_BIT)
  #define CIM_CFG_DSM_CPM	  (0 << CIM_CFG_DSM_BIT) /* CCIR656 Progressive Mode */
  #define CIM_CFG_DSM_CIM	  (1 << CIM_CFG_DSM_BIT) /* CCIR656 Interlace Mode */
  #define CIM_CFG_DSM_GCM	  (2 << CIM_CFG_DSM_BIT) /* Gated Clock Mode */

/* CIM Control Register  (CIM_CTRL) */
#define	CIM_CTRL_EEOF_LINE_BIT	20
#define	CIM_CTRL_EEOF_LINE_MASK	(0xfff << CIM_CTRL_EEOF_LINE_BIT)
#define	CIM_CTRL_FRC_BIT	16
#define	CIM_CTRL_FRC_MASK	(0xf << CIM_CTRL_FRC_BIT)
  #define CIM_CTRL_FRC_1	  (0x0 << CIM_CTRL_FRC_BIT) /* Sample every frame */
  #define CIM_CTRL_FRC_2	  (0x1 << CIM_CTRL_FRC_BIT) /* Sample 1/2 frame */
  #define CIM_CTRL_FRC_3	  (0x2 << CIM_CTRL_FRC_BIT) /* Sample 1/3 frame */
  #define CIM_CTRL_FRC_4	  (0x3 << CIM_CTRL_FRC_BIT) /* Sample 1/4 frame */
  #define CIM_CTRL_FRC_5	  (0x4 << CIM_CTRL_FRC_BIT) /* Sample 1/5 frame */
  #define CIM_CTRL_FRC_6	  (0x5 << CIM_CTRL_FRC_BIT) /* Sample 1/6 frame */
  #define CIM_CTRL_FRC_7	  (0x6 << CIM_CTRL_FRC_BIT) /* Sample 1/7 frame */
  #define CIM_CTRL_FRC_8	  (0x7 << CIM_CTRL_FRC_BIT) /* Sample 1/8 frame */
  #define CIM_CTRL_FRC_9	  (0x8 << CIM_CTRL_FRC_BIT) /* Sample 1/9 frame */
  #define CIM_CTRL_FRC_10	  (0x9 << CIM_CTRL_FRC_BIT) /* Sample 1/10 frame */
  #define CIM_CTRL_FRC_11	  (0xA << CIM_CTRL_FRC_BIT) /* Sample 1/11 frame */
  #define CIM_CTRL_FRC_12	  (0xB << CIM_CTRL_FRC_BIT) /* Sample 1/12 frame */
  #define CIM_CTRL_FRC_13	  (0xC << CIM_CTRL_FRC_BIT) /* Sample 1/13 frame */
  #define CIM_CTRL_FRC_14	  (0xD << CIM_CTRL_FRC_BIT) /* Sample 1/14 frame */
  #define CIM_CTRL_FRC_15	  (0xE << CIM_CTRL_FRC_BIT) /* Sample 1/15 frame */
  #define CIM_CTRL_FRC_16	  (0xF << CIM_CTRL_FRC_BIT) /* Sample 1/16 frame */

//#define	CIM_CTRL_DMA_EEOFM	(1 << 15)	/* Enable EEOF interrupt */
#define	CIM_CTRL_WIN_EN		(1 << 14)
//#define	CIM_CTRL_VDDM		(1 << 13) /* VDD interrupt enable */
//#define	CIM_CTRL_DMA_SOFM	(1 << 12)
//#define	CIM_CTRL_DMA_EOFM	(1 << 11)
//#define	CIM_CTRL_DMA_STOPM	(1 << 10)
//#define	CIM_CTRL_RXF_TRIGM	(1 << 9)
//#define	CIM_CTRL_RXF_OFM	(1 << 8)
#define	CIM_CTRL_DMA_SYNC	(1 << 7)	/*when change DA, do frame sync */
#define CIM_CTRL_MBEN		(1 << 6)
#define CIM_CTRL_HSYNC		(1 << 5)
#define CIM_CTRL_SW_RST		(1 << 3)/*Software reset enable*/		
#define	CIM_CTRL_DMA_EN		(1 << 2) /* Enable DMA */
#define	CIM_CTRL_RXF_RST	(1 << 1) /* RxFIFO reset */
#define	CIM_CTRL_ENA		(1 << 0) /* Enable CIM */


/* cim control2 */
#define CIM_CTRL2_FRAGHE	(1 << 25)	
#define CIM_CTRL2_FRAGVE	(1 << 24)
#define CIM_CTRL2_FSC		(1 << 23)
#define CIM_CTRL2_ARIF		(1 << 22)	
#define CIM_CTRL2_HRS_BIT	20
#define CIM_CTRL2_VRS_BIT	18
#define CIM_CTRL2_CSC_BIT	16
#define CIM_CTRL2_CSC_BYPASS	(0 << CIM_CTRL2_CSC_BIT)
#define CIM_CTRL2_CSC_YUV422	(2 << CIM_CTRL2_CSC_BIT)
#define CIM_CTRL2_CSC_YUV420	(3 << CIM_CTRL2_CSC_BIT)
#define CIM_CTRL2_OPG_BIT	4
#define CIM_CTRL2_OPG_MASK	(0x3 << CIM_CTRL2_OPG_BIT)
#define CIM_CTRL2_OPE		(1 << 2)
#define CIM_CTRL2_EME		(1 << 1)
#define CIM_CTRL2_APM		(1 << 0)

/* CIM State Register  (CIM_STATE) */
#define CIM_STATE_CR_RF_OF	(1 << 21)
#define CIM_STATE_CR_RF_EMPTY	(1 << 20)

#define CIM_STATE_CB_RF_OF	(1 << 19)
#define CIM_STATE_CB_RF_EMPTY	(1 << 18)

#define CIM_STATE_Y_RFOF	(1 << 17)/*Y_RXFIFO over flow*/
#define CIM_STATE_Y_RFE		(1 << 16)/*Y_RXFIFO empty*/

#define	CIM_STATE_DMA_EEOF	(1 << 11) /* DMA Line EEOf irq */
#define CIM_STATE_DMA_STOP	(1 << 10)/*DMA stop irq*/
#define CIM_STATE_DMA_EOF	(1 << 9)/*DMA end irq*/

#define CIM_STATE_DSOF		(1 << 8)
#define	CIM_STATE_TLBE		(1 << 4) /* TLB error */
#define	CIM_STATE_FSE		(1 << 3) /* Frame size check error irq */
#define	CIM_STATE_RXF_OF	(1 << 2) /* RXFIFO over flow irq */
#define	CIM_STATE_RXF_EMPTY	(1 << 1) /* RXFIFO empty irq */
#define	CIM_STATE_VDD		(1 << 0) /* CIM disabled irq */

/* CIM DMA Command Register (CIM_CMD) */

#define	CIM_CMD_SOFINT		(1 << 31) /* enable DMA start irq */
#define	CIM_CMD_EOFINT		(1 << 30) /* enable DMA end irq */
#define	CIM_CMD_EEOFINT		(1 << 29) /* enable DMA EEOF irq */
#define	CIM_CMD_STOP		(1 << 28) /* enable DMA stop irq */
#define	CIM_CMD_OFRCV		(1 << 27) /* enable recovery when TXFiFo overflow */
#define	CIM_CMD_LEN_BIT		0
#define	CIM_CMD_LEN_MASK	(0xffffff << CIM_CMD_LEN_BIT)

/* CIM Window-Image Size Register  (CIM_SIZE) */
#define	CIM_SIZE_LPF_BIT	16 /* Lines per freame for csc output image */
#define	CIM_SIZE_LPF_MASK	(0x1fff << CIM_SIZE_LPF_BIT)
#define	CIM_SIZE_PPL_BIT	0 /* Pixels per line for csc output image, should be an even number */
#define	CIM_SIZE_PPL_MASK	(0x1fff << CIM_SIZE_PPL_BIT)

/* CIM Image Offset Register  (CIM_OFFSET) */
#define	CIM_OFFSET_V_BIT	16 /* Vertical offset */
#define	CIM_OFFSET_V_MASK	(0xfff << CIM_OFFSET_V_BIT)
#define	CIM_OFFSET_H_BIT	0 /* Horizontal offset, should be an enen number */
#define	CIM_OFFSET_H_MASK	(0xfff << CIM_OFFSET_H_BIT) /*OFFSET_H should be even number*/
/*CIM Frame Size Register	(CIM_FS)*/
#define CIM_VERTICAL_SIZE_BIT	16
#define CIM_VERTICAL_SIZE_MASK	(0x1fff << CIM_VERTICAL_SIZE_BIT)
#define CIM_FHS_BIT		0
#define CIM_FHS_MASK		(0x1fff << CIM_FHS_BIT)	
#define CIM_BPP			14
/*CIM TLB Control Register (CIMTC)*/
#define CIM_TC_TLB_ADDR		2
#define	CIM_TC_TLB_RESET	(1 << 1)
#define	CIM_TC_TLB_ENB		(1 << 0)
/*CIM interrupt Mask Register CIMIMR*/
#define	CIM_IMR_DMA_EEOFM	(1 << 11)	/* Enable EEOF interrupt */
#define	CIM_IMR_DMA_STOPM	(1 << 10)
#define	CIM_IMR_DMA_EOFM	(1 << 9)
#define	CIM_IMR_DMA_SOFM	(1 << 8)
#define	CIM_IMR_TLBEM		(1 << 4)
#define CIM_IMR_FSEM		(1 << 3)
#define	CIM_IMR_RFOFM		(1 << 2)
#define	CIM_IMR_VDDM		(1 << 0) /* VDD interrupt enable */

#ifndef __MIPS_ASSEMBLER	

/***************************************************************************
 * CIM
 ***************************************************************************/

#define __cim_enable(n)	( REG_CIM_CTRL(n) |= CIM_CTRL_ENA )
#define __cim_disable(n)	( REG_CIM_CTRL(n) &= ~CIM_CTRL_ENA )

#define __cim_enable_sep(n) (REG_CIM_CFG(n) |= CIM_CFG_SEP)
#define __cim_disable_sep(n) (REG_CIM_CFG(n) &= ~CIM_CFG_SEP)

/* n = 0, 1, 2, 3 */
#define __cim_set_input_data_stream_order(m,n)				\
	do {								\
		REG_CIM_CFG(m) &= ~CIM_CFG_ORDER_MASK;			\
		REG_CIM_CFG(m) |= ((n)<<CIM_CFG_ORDER_BIT)&CIM_CFG_ORDER_MASK; \
	} while (0)

#define __cim_input_data_format_select_YUV444(n)		\
	do {						\
		REG_CIM_CFG(n) &= ~CIM_CFG_DF_MASK;		\
		REG_CIM_CFG(n) |= CIM_CFG_DF_YUV444;	\
	} while (0)

#define __cim_input_data_format_select_YUV422(n)		\
	do {						\
		REG_CIM_CFG(n) &= ~CIM_CFG_DF_MASK;		\
		REG_CIM_CFG(n) |= CIM_CFG_DF_YUV422;	\
	} while (0)

#define __cim_input_data_format_select_ITU656(n)		\
	do {						\
		REG_CIM_CFG(n) &= ~CIM_CFG_DF_MASK;		\
		REG_CIM_CFG(n) |= CIM_CFG_DF_ITU656;	\
	} while (0)

#define __cim_input_data_inverse(n)	( REG_CIM_CFG(n) |= CIM_CFG_INV_DAT )
#define __cim_input_data_normal(n)	( REG_CIM_CFG(n) &= ~CIM_CFG_INV_DAT )

#define __cim_vsync_active_low(n)	( REG_CIM_CFG(n) |= CIM_CFG_VSP )
#define __cim_vsync_active_high(n)	( REG_CIM_CFG(n) &= ~CIM_CFG_VSP )

#define __cim_hsync_active_low(n)	( REG_CIM_CFG(n) |= CIM_CFG_HSP )
#define __cim_hsync_active_high(n)	( REG_CIM_CFG(n) &= ~CIM_CFG_HSP )

#define __cim_sample_data_at_pclk_falling_edge(n) \
	( REG_CIM_CFG(n) |= CIM_CFG_PCP )
#define __cim_sample_data_at_pclk_rising_edge(n) \
	( REG_CIM_CFG(n) &= ~CIM_CFG_PCP )

#define __cim_enable_dummy_zero(n)	( REG_CIM_CFG(n) |= CIM_CFG_DUMMY_ZERO )
#define __cim_disable_dummy_zero(n)	( REG_CIM_CFG(n) &= ~CIM_CFG_DUMMY_ZERO )

#define __cim_select_external_vsync(n)	( REG_CIM_CFG(n) |= CIM_CFG_EXT_VSYNC )
#define __cim_select_internal_vsync(n)	( REG_CIM_CFG(n) &= ~CIM_CFG_EXT_VSYNC )

/* n=0-7 */
#define __cim_set_data_packing_mode(m,n) 		\
do {						\
	REG_CIM_CFG(m) &= ~CIM_CFG_PACK_MASK;	\
	REG_CIM_CFG(m) |= (CIM_CFG_PACK_##n);	\
} while (0)

#define __cim_enable_bypass_func(n) 	(REG_CIM_CFG(n) |= CIM_CFG_BYPASS)
#define __cim_disable_bypass_func(n) 	(REG_CIM_CFG(n) &= ~CIM_CFG_BYPASS_MASK)

#define __cim_enable_ccir656_progressive_mode(n)	\
do {						\
	REG_CIM_CFG(n) &= ~CIM_CFG_DSM_MASK;	\
	REG_CIM_CFG(n) |= CIM_CFG_DSM_CPM;		\
} while (0)

#define __cim_enable_ccir656_interlace_mode(n)	\
do {						\
	REG_CIM_CFG(n) &= ~CIM_CFG_DSM_MASK;	\
	REG_CIM_CFG(n) |= CIM_CFG_DSM_CIM;		\
} while (0)

#define __cim_enable_gated_clock_mode(n)		\
do {						\
	REG_CIM_CFG(n) &= ~CIM_CFG_DSM_MASK;	\
	REG_CIM_CFG(n) |= CIM_CFG_DSM_GCM;		\
} while (0)

/* n=1-16 */
#define __cim_set_frame_rate(m,n) 		\
do {						\
	REG_CIM_CTRL(n) &= ~CIM_CTRL_FRC_MASK; 	\
	REG_CIM_CTRL(n) |= CIM_CTRL_FRC_##n;	\
} while (0)

#define __cim_enable_size_func(n) \
	( REG_CIM_CTRL(n) |= CIM_CTRL_WIN_EN)
#define __cim_disable_size_func(n) \
	( REG_CIM_CTRL(n) &= ~CIM_CTRL_WIN_EN )

#define __cim_disable_vdd_intr(n) \
	( REG_CIM_IMR(n) |= CIM_IMR_VDDM )
#define __cim_enable_vdd_intr(n) \
	( REG_CIM_IMR(n) &= ~CIM_IMR_VDDM )

#define __cim_disable_sof_intr(n) \
	( REG_CIM_IMR(n) |= CIM_IMR_DMA_SOFM )
#define __cim_enable_sof_intr(n) \
	( REG_CIM_IMR(n) &= ~CIM_IMR_DMA_SOFM )

#define __cim_disable_eof_intr(n) \
	( REG_CIM_IMR(n) |= CIM_IMR_DMA_EOFM )
#define __cim_enable_eof_intr(n) \
	( REG_CIM_IMR(n) &= ~CIM_IMR_DMA_EOFM )

#define __cim_disable_eeof_intr(n) \
	( REG_CIM_IMR(n) |= CIM_IMR_DMA_EEOFM )
#define __cim_enable_eeof_intr(n) \
	( REG_CIM_IMR(n) &= ~CIM_IMR_DMA_EEOFM )

#define __cim_disable_stop_intr(n) \
	( REG_CIM_IMR(n) |= CIM_IMR_DMA_STOPM )
#define __cim_enable_stop_intr(n) \
	( REG_CIM_IMR(n) &= ~CIM_IMR_DMA_STOPM )

#define __cim_disable_fsem_intr(n) \
	( REG_CIM_IMR(n) |= CIM_IMR_FSEM )
#define __cim_enable_fsem_intr(n) \
	( REG_CIM_IMR(n) &= ~CIM_IMR_FSEM )

#define __cim_disable_rxfifo_overflow_intr(n)	\
	( REG_CIM_IMR(n) |= CIM_IMR_RFOFM )
#define __cim_enable_rxfifo_overflow_intr(n)	\
	( REG_CIM_IMR(n) &= ~CIM_IMR_RFOFM )

#define __cim_disable_TLB_error_intr(n)	\
	( REG_CIM_IMR(n) |= CIM_IMR_TLBEM)
#define __cim_enable_TLB_eeror_intr(n)	\
	( REG_CIM_IMR(n) |= CIM_IMR_TLBEM)

#define __cim_set_eeof_line(m,n)						\
	do {								\
		REG_CIM_CTRL(m) &= ~CIM_CTRL_EEOF_LINE_MASK;		\
		REG_CIM_CTRL(m) |= ( ((n) << CIM_CTRL_EEOF_LINE_BIT) & CIM_CTRL_EEOF_LINE_MASK ); \
	} while (0)

#define __cim_enable_dma(n)   ( REG_CIM_CTRL(n) |= CIM_CTRL_DMA_EN )
#define __cim_disable_dma(n)  ( REG_CIM_CTRL(n) &= ~CIM_CTRL_DMA_EN )
#define __cim_reset_rxfifo(n) ( REG_CIM_CTRL(n) |= CIM_CTRL_RXF_RST )
#define __cim_unreset_rxfifo(n) ( REG_CIM_CTRL(n) &= ~CIM_CTRL_RXF_RST )

/* cim control2 */
#define __cim_enable_priority_control(n)		( REG_CIM_CTRL2(n) |= CIM_CTRL2_APM)
#define __cim_disable_priority_control(n)	( REG_CIM_CTRL2(n) &= ~CIM_CTRL2_APM)
#define __cim_enable_auto_priority(n)		( REG_CIM_CTRL2(n) |= CIM_CTRL2_OPE)
#define __cim_disable_auto_priority(n)		( REG_CIM_CTRL2(n) &= ~CIM_CTRL2_OPE)
#define __cim_enable_emergency(n)		( REG_CIM_CTRL2(n) |= CIM_CTRL2_EME)
#define __cim_disable_emergency(n)		( REG_CIM_CTRL2(n) &= ~CIM_CTRL2_EME);
/* 0, 1, 2, 3
 * 0: highest priority
 * 3: lowest priority
 * 1 maybe best for SEP=1
 * 3 maybe best for SEP=0
 */
#define __cim_set_opg(m,n)				\
	do {								\
		REG_CIM_CTRL2(m) &= ~CIM_CTRL2_OPG_MASK;			\
		REG_CIM_CTRL2(m) |= ((n) << CIM_CTRL2_OPG_BIT) & CIM_CTRL2_OPG_MASK; \
	} while (0)

#define __cim_clear_state(n)   	     ( REG_CIM_STATE(n) = 0 )

#define __cim_disable_done(n)   	     ( REG_CIM_STATE(n) & CIM_STATE_VDD )
#define __cim_rxfifo_empty(n)   	     ( REG_CIM_STATE(n) & CIM_STATE_RXF_EMPTY )
#define __cim_rxfifo_overflow(n)      ( REG_CIM_STATE(n) & CIM_STATE_RXF_OF )
#define __cim_clear_rxfifo_overflow(n) ( REG_CIM_STATE(n) &= ~CIM_STATE_RXF_OF )
#define __cim_dma_stop(n)   	     ( REG_CIM_STATE(n) & CIM_STATE_DMA_STOP )
#define __cim_dma_eof(n)   	     ( REG_CIM_STATE(n) & CIM_STATE_DMA_EOF )
#define __cim_dma_sof(n)   	     ( REG_CIM_STATE(n) & CIM_STATE_DSOF )

#define __cim_get_iid(n)   	     ( REG_CIM_IID(n) )
#define __cim_get_fid(n)   	     ( REG_CIM_FID(n) )
//#define __cim_get_image_data()       ( REG_CIM_RXFIFO )
#define __cim_get_dma_cmd(n)          ( REG_CIM_CMD(n) )

#define __cim_set_da(n,a)              ( REG_CIM_DA(n) = (a) )

#define __cim_set_line(n,a) 	( REG_CIM_SIZE(n) = (REG_CIM_SIZE(n)&(~CIM_SIZE_LPF_MASK))|((a)<<CIM_SIZE_LPF_BIT) )
#define __cim_set_pixel(n,a) 	( REG_CIM_SIZE(n) = (REG_CIM_SIZE(n)&(~CIM_SIZE_PPL_MASK))|((a)<<CIM_SIZE_PPL_BIT) )
#define __cim_get_line(n) 	((REG_CIM_SIZE(n)&CIM_SIZE_LPF_MASK)>>CIM_SIZE_LPF_BIT)
#define __cim_get_pixel(n) 	((REG_CIM_SIZE(n)&CIM_SIZE_PPL_MASK)>>CIM_SIZE_PPL_BIT)

#define __cim_set_v_offset(n,a) 	( REG_CIM_OFFSET(n) = (REG_CIM_OFFSET(n)&(~CIM_OFFSET_V_MASK)) | ((a)<<CIM_OFFSET_V_BIT) )
#define __cim_set_h_offset(n,a) 	( REG_CIM_OFFSET(n) = (REG_CIM_OFFSET(n)&(~CIM_OFFSET_H_MASK)) | ((a)<<CIM_OFFSET_H_BIT) )
#define __cim_get_v_offset(n) 	((REG_CIM_OFFSET(n)&CIM_OFFSET_V_MASK)>>CIM_OFFSET_V_BIT)
#define __cim_get_h_offset(n) 	((REG_CIM_OFFSET(n)&CIM_OFFSET_H_MASK)>>CIM_OFFSET_H_BIT)

#endif /* __MIPS_ASSEMBLER */

#endif /* __JZ4775CIM_H__ */
