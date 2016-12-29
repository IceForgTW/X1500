/*
 * linux/include/asm-mips/mach-jz4775/jz4775lcdc.h
 *
 * JZ4775 LCDC register definition.
 *
 * Copyright (C) 2010 Ingenic Semiconductor Co., Ltd.
 */

#ifndef __JZ4775LCDC_H__
#define __JZ4775LCDC_H__

#define	LCD0_BASE	0xB3050000
#define	SLCD_BASE	0xB3050000


/*************************************************************************
 * SLCD (Smart LCD Controller)
 *************************************************************************/

#define SLCD_CFG	(SLCD_BASE + 0xA0)  /* SLCD Configure Register */
#define SLCD_CTRL	(SLCD_BASE + 0xA4)  /* SLCD Control Register */
#define SLCD_STATE	(SLCD_BASE + 0xA8)  /* SLCD Status Register */
#define SLCD_DATA	(SLCD_BASE + 0xAC)  /* SLCD Data Register */

#define REG_SLCD_CFG	REG32(SLCD_CFG)
#define REG_SLCD_CTRL	REG32(SLCD_CTRL)
#define REG_SLCD_STATE	REG32(SLCD_STATE)
#define REG_SLCD_DATA	REG32(SLCD_DATA)

/* SLCD Configure Register */
#define SLCD_CFG_DWIDTH_BIT	10
#define SLCD_CFG_DWIDTH_MASK	(0x7 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_18BIT	(0 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_16BIT	(1 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_8BIT_x3	(2 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_8BIT_x2	(3 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_8BIT_x1	(4 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_24BIT	(5 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_9BIT_x2	(7 << SLCD_CFG_DWIDTH_BIT)
#define SLCD_CFG_CWIDTH_BIT	(8)
#define SLCD_CFG_CWIDTH_MASK	(0x7 << SLCD_CFG_CWIDTH_BIT)
#define SLCD_CFG_CWIDTH_16BIT	(0 << SLCD_CFG_CWIDTH_BIT)
#define SLCD_CFG_CWIDTH_8BIT	(1 << SLCD_CFG_CWIDTH_BIT)
#define SLCD_CFG_CWIDTH_18BIT	(2 << SLCD_CFG_CWIDTH_BIT)
#define SLCD_CFG_CWIDTH_24BIT	(3 << SLCD_CFG_CWIDTH_BIT)
#define SLCD_CFG_CS_ACTIVE_LOW	(0 << 4)
#define SLCD_CFG_CS_ACTIVE_HIGH	(1 << 4)
#define SLCD_CFG_RS_CMD_LOW	(0 << 3)
#define SLCD_CFG_RS_CMD_HIGH	(1 << 3)
#define SLCD_CFG_CLK_ACTIVE_FALLING	(0 << 1)
#define SLCD_CFG_CLK_ACTIVE_RISING	(1 << 1)
#define SLCD_CFG_TYPE_PARALLEL	(0 << 0)
#define SLCD_CFG_TYPE_SERIAL	(1 << 0)

/* SLCD Control Register */
#define SLCD_CTRL_DMA_MODE	(1 << 2)
#define SLCD_CTRL_DMA_START	(1 << 1)

/* SLCD Status Register */
#define SLCD_STATE_BUSY		(1 << 0)

/* SLCD Data Register */
#define SLCD_DATA_RS_DATA	(0 << 31)
#define SLCD_DATA_RS_COMMAND	(1 << 31)

/*************************************************************************
 * LCD (LCD Controller)
 *************************************************************************/
#define LCD_CFG(n)	(LCD0_BASE + (n)*0x50000 + 0x00) /* LCD Configure Register */
#define LCD_CTRL(n)	(LCD0_BASE + (n)*0x50000 + 0x30) /* LCD Control Register */
#define LCD_STATE(n)	(LCD0_BASE + (n)*0x50000 + 0x34) /* LCD Status Register */

#define LCD_OSDC(n)	(LCD0_BASE + (n)*0x50000 + 0x100) /* LCD OSD Configure Register */
#define LCD_OSDCTRL(n)	(LCD0_BASE + (n)*0x50000 + 0x104) /* LCD OSD Control Register */
#define LCD_OSDS(n)	(LCD0_BASE + (n)*0x50000 + 0x108) /* LCD OSD Status Register */
#define LCD_BGC0(n)	(LCD0_BASE + (n)*0x50000 + 0x10C) /* LCD fg0 Background Color Register */
#define LCD_BGC1(n)	(LCD0_BASE + (n)*0x50000 + 0x2C4) /* LCD fg1 Background Color Register */
#define LCD_KEY0(n)	(LCD0_BASE + (n)*0x50000 + 0x110) /* LCD Foreground Color Key Register 0 */
#define LCD_KEY1(n)	(LCD0_BASE + (n)*0x50000 + 0x114) /* LCD Foreground Color Key Register 1 */
#define LCD_ALPHA(n)	(LCD0_BASE + (n)*0x50000 + 0x118) /* LCD ALPHA Register */
//#define LCD_IPUR(n)	(LCD0_BASE + (n)*0x50000 + 0x11C) /* LCD IPU Restart Register */

#define LCD_VAT(n)		(LCD0_BASE + (n)*0x50000 + 0x0c) /* Virtual Area Setting Register */
#define LCD_DAH(n)		(LCD0_BASE + (n)*0x50000 + 0x10) /* Display Area Horizontal Start/End Point */
#define LCD_DAV(n)		(LCD0_BASE + (n)*0x50000 + 0x14) /* Display Area Vertical Start/End Point */
#define LCD_XYP0(n)	(LCD0_BASE + (n)*0x50000 + 0x120) /* Foreground 0 XY Position Register */
#define LCD_XYP1(n)	(LCD0_BASE + (n)*0x50000 + 0x124) /* Foreground 1 XY Position Register */
#define LCD_SIZE0(n)	(LCD0_BASE + (n)*0x50000 + 0x128) /* Foreground 0 Size Register */
#define LCD_SIZE1(n)	(LCD0_BASE + (n)*0x50000 + 0x12C) /* Foreground 1 Size Register */
#define LCD_RGBC(n)	(LCD0_BASE + (n)*0x50000 + 0x90) /* RGB Controll Register */

#define LCD_VSYNC(n)	(LCD0_BASE + (n)*0x50000 + 0x04) /* Vertical Synchronize Register */
#define LCD_HSYNC(n)	(LCD0_BASE + (n)*0x50000 + 0x08) /* Horizontal Synchronize Register */
#define LCD_PS(n)		(LCD0_BASE + (n)*0x50000 + 0x18) /* PS Signal Setting */
#define LCD_CLS(n)		(LCD0_BASE + (n)*0x50000 + 0x1c) /* CLS Signal Setting */
#define LCD_SPL(n)		(LCD0_BASE + (n)*0x50000 + 0x20) /* SPL Signal Setting */
#define LCD_REV(n)		(LCD0_BASE + (n)*0x50000 + 0x24) /* REV Signal Setting */
#define LCD_IID(n)		(LCD0_BASE + (n)*0x50000 + 0x38) /* Interrupt ID Register */
#define LCD_DA0(n)		(LCD0_BASE + (n)*0x50000 + 0x40) /* Descriptor Address Register 0 */
#define LCD_SA0(n)		(LCD0_BASE + (n)*0x50000 + 0x44) /* Source Address Register 0 */
#define LCD_FID0(n)	(LCD0_BASE + (n)*0x50000 + 0x48) /* Frame ID Register 0 */
#define LCD_CMD0(n)	(LCD0_BASE + (n)*0x50000 + 0x4c) /* DMA Command Register 0 */
#define LCD_OFFS0(n)	(LCD0_BASE + (n)*0x50000 + 0x60) /* DMA Offsize Register 0 */
#define LCD_PW0(n)		(LCD0_BASE + (n)*0x50000 + 0x64) /* DMA Page Width Register 0 */
#define LCD_CNUM0(n)	(LCD0_BASE + (n)*0x50000 + 0x68) /* DMA Command Counter Register 0 */
#define LCD_POS0(n)	(LCD0_BASE + (n)*0x50000 + 0x68) /* DMA Command Counter Register 0 */
#define LCD_DESSIZE0(n)	(LCD0_BASE + (n)*0x50000 + 0x6C) /* Foreground Size in Descriptor 0 Register*/

#define LCD_DA1(n)		(LCD0_BASE + (n)*0x50000 + 0x50) /* Descriptor Address Register 1 */
#define LCD_SA1(n)		(LCD0_BASE + (n)*0x50000 + 0x54) /* Source Address Register 1 */
#define LCD_FID1(n)	(LCD0_BASE + (n)*0x50000 + 0x58) /* Frame ID Register 1 */
#define LCD_CMD1(n)	(LCD0_BASE + (n)*0x50000 + 0x5c) /* DMA Command Register 1 */
#define LCD_OFFS1(n)	(LCD0_BASE + (n)*0x50000 + 0x70) /* DMA Offsize Register 1 */
#define LCD_PW1(n)		(LCD0_BASE + (n)*0x50000 + 0x74) /* DMA Page Width Register 1 */
#define LCD_CNUM1(n)	(LCD0_BASE + (n)*0x50000 + 0x78) /* DMA Command Counter Register 1 */
#define LCD_POS1(n)	(LCD0_BASE + (n)*0x50000 + 0x78) /* DMA Command Counter Register 1 */
#define LCD_DESSIZE1(n)	(LCD0_BASE + (n)*0x50000 + 0x7C) /* Foreground Size in Descriptor 1 Register*/
#define LCD_PCFG(n)        (LCD0_BASE + (n)*0x50000 + 0x2C0)


#define REG_LCD_CFG(n)	REG32(LCD_CFG(n))
#define REG_LCD_CTRL(n)	REG32(LCD_CTRL(n))
#define REG_LCD_STATE(n)	REG32(LCD_STATE(n))

#define REG_LCD_OSDC(n)	REG32(LCD_OSDC(n))
#define REG_LCD_OSDCTRL(n)	REG32(LCD_OSDCTRL(n))
#define REG_LCD_OSDS(n)	REG32(LCD_OSDS(n))
#define REG_LCD_BGC0(n)	REG32(LCD_BGC0(n))
#define REG_LCD_BGC1(n)	REG32(LCD_BGC1(n))
#define REG_LCD_KEY0(n)	REG32(LCD_KEY0(n))
#define REG_LCD_KEY1(n)	REG32(LCD_KEY1(n))
#define REG_LCD_ALPHA(n)	REG32(LCD_ALPHA(n))
//#define REG_LCD_IPUR(n)	REG32(LCD_IPUR(n))

#define REG_LCD_VAT(n)	REG32(LCD_VAT(n))
#define REG_LCD_DAH(n)	REG32(LCD_DAH(n))
#define REG_LCD_DAV(n)	REG32(LCD_DAV(n))

#define REG_LCD_XYP0(n)		REG32(LCD_XYP0(n))
#define REG_LCD_XYP1(n)		REG32(LCD_XYP1(n))
#define REG_LCD_SIZE0(n)		REG32(LCD_SIZE0(n))
#define REG_LCD_SIZE1(n)	REG32(LCD_SIZE1(n))

#define REG_LCD_RGBC(n)	REG32(LCD_RGBC(n))

#define REG_LCD_VSYNC(n)	REG32(LCD_VSYNC(n))
#define REG_LCD_HSYNC(n)	REG32(LCD_HSYNC(n))
#define REG_LCD_PS(n)	REG32(LCD_PS(n))
#define REG_LCD_CLS(n)	REG32(LCD_CLS(n))
#define REG_LCD_SPL(n)	REG32(LCD_SPL(n))
#define REG_LCD_REV(n)	REG32(LCD_REV(n))
#define REG_LCD_IID(n)	REG32(LCD_IID(n))
#define REG_LCD_DA0(n)	REG32(LCD_DA0(n))
#define REG_LCD_SA0(n)	REG32(LCD_SA0(n))
#define REG_LCD_FID0(n)	REG32(LCD_FID0(n))
#define REG_LCD_CMD0(n)	REG32(LCD_CMD0(n))

#define REG_LCD_OFFS0(n)	REG32(LCD_OFFS0(n))
#define REG_LCD_PW0(n)	REG32(LCD_PW0(n))
#define REG_LCD_CNUM0(n)	REG32(LCD_CNUM0(n))
#define REG_LCD_POS0(n)	REG32(LCD_POS0(n))
#define REG_LCD_DESSIZE0(n)	REG32(LCD_DESSIZE0(n))

#define REG_LCD_DA1(n)	REG32(LCD_DA1(n))
#define REG_LCD_SA1(n)	REG32(LCD_SA1(n))
#define REG_LCD_FID1(n)	REG32(LCD_FID1(n))
#define REG_LCD_CMD1(n)	REG32(LCD_CMD1(n))
#define REG_LCD_OFFS1(n)	REG32(LCD_OFFS1(n))
#define REG_LCD_PW1(n)	REG32(LCD_PW1(n))
#define REG_LCD_CNUM1(n)	REG32(LCD_CNUM1(n))
#define REG_LCD_POS1(n)	REG32(LCD_POS1(n))
#define REG_LCD_DESSIZE1(n)	REG32(LCD_DESSIZE1(n))
#define REG_LCD_PCFG(n)            REG32(LCD_PCFG(n))

#if 0
#define REG_LCD_ENHCFG(n)      REG32(LCD_ENHCFG(n))
#define REG_LCD_CSCCFG(n)      REG32(LCD_CSCCFG(n))
#define REG_LCD_LUMMACFG(n)    REG32(LCD_LUMMACFG(n))
#define REG_LCD_CHROCFG0(n)    REG32(LCD_CHROCFG0(n))
#define REG_LCD_CHROCFG1(n)    REG32(LCD_CHROCFG1(n))
#define REG_LCD_DITHERCFG(n)   REG32(LCD_DITHERCFG(n))
#define REG_LCD_ENHSTATUS(n)   REG32(LCD_ENHSTATUS(n))
#endif

/*
#define REG_LCD_GAMMA(n)       REG32(LCD_GAMMA(n))
#define REG_LCD_VEE(n)      REG32(LCD_VEE(n))
*/

/* LCD Configure Register */
#define LCD_CFG_LCDPIN_BIT	31  /* LCD pins selection */
#define LCD_CFG_LCDPIN_MASK	(0x1 << LCD_CFG_LCDPIN_BIT)
  #define LCD_CFG_LCDPIN_LCD	(0x0 << LCD_CFG_LCDPIN_BIT)
  #define LCD_CFG_LCDPIN_SLCD	(0x1 << LCD_CFG_LCDPIN_BIT)
#define LCD_CFG_TVEPEH		(1 << 30) /* TVE PAL enable extra halfline signal */
#define LCD_CFG_NEWDES		(1 << 28) /* use new descripter. old: 4words, new:8words */
#define LCD_CFG_PALBP		(1 << 27) /* bypass data format and alpha blending */
#define LCD_CFG_TVEN		(1 << 26) /* indicate the terminal is lcd or tv */
#define LCD_CFG_RECOVER		(1 << 25) /* Auto recover when output fifo underrun */
#define LCD_CFG_PSM		(1 << 23) /* PS signal mode */
#define LCD_CFG_CLSM		(1 << 22) /* CLS signal mode */
#define LCD_CFG_SPLM		(1 << 21) /* SPL signal mode */
#define LCD_CFG_REVM		(1 << 20) /* REV signal mode */
#define LCD_CFG_HSYNM		(1 << 19) /* HSYNC signal mode */
#define LCD_CFG_PCLKM		(1 << 18) /* PCLK signal mode */
#define LCD_CFG_INVDAT		(1 << 17) /* Inverse output data */
#define LCD_CFG_SYNDIR_IN	(1 << 16) /* VSYNC&HSYNC direction */
#define LCD_CFG_PSP		(1 << 15) /* PS pin reset state */
#define LCD_CFG_CLSP		(1 << 14) /* CLS pin reset state */
#define LCD_CFG_SPLP		(1 << 13) /* SPL pin reset state */
#define LCD_CFG_REVP		(1 << 12) /* REV pin reset state */
#define LCD_CFG_HSP		(1 << 11) /* HSYNC polarity:0-active high,1-active low */
#define LCD_CFG_PCP		(1 << 10) /* PCLK polarity:0-rising,1-falling */
#define LCD_CFG_DEP		(1 << 9)  /* DE polarity:0-active high,1-active low */
#define LCD_CFG_VSP		(1 << 8)  /* VSYNC polarity:0-rising,1-falling */
#define LCD_CFG_MODE_TFT_18BIT 	(1 << 7)  /* 18bit TFT */
#define LCD_CFG_MODE_TFT_16BIT 	(0 << 7)  /* 16bit TFT */
#define LCD_CFG_MODE_TFT_24BIT 	(1 << 6)  /* 24bit TFT */
#define LCD_CFG_PDW_BIT		4  /* STN pins utilization */
#define LCD_CFG_PDW_MASK	(0x3 << LCD_DEV_PDW_BIT)
#define LCD_CFG_PDW_1		(0 << LCD_CFG_PDW_BIT) /* LCD_D[0] */
  #define LCD_CFG_PDW_2		(1 << LCD_CFG_PDW_BIT) /* LCD_D[0:1] */
  #define LCD_CFG_PDW_4		(2 << LCD_CFG_PDW_BIT) /* LCD_D[0:3]/LCD_D[8:11] */
  #define LCD_CFG_PDW_8		(3 << LCD_CFG_PDW_BIT) /* LCD_D[0:7]/LCD_D[8:15] */
#define LCD_CFG_MODE_BIT	0  /* Display Device Mode Select */
#define LCD_CFG_MODE_MASK	(0x0f << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_GENERIC_TFT	(0 << LCD_CFG_MODE_BIT) /* 16,18 bit TFT */
  #define LCD_CFG_MODE_SPECIAL_TFT_1	(1 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SPECIAL_TFT_2	(2 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SPECIAL_TFT_3	(3 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_NONINTER_CCIR656	(4 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_INTER_CCIR656	(6 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SINGLE_CSTN	(8 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SINGLE_MSTN	(9 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_DUAL_CSTN	(10 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_DUAL_MSTN	(11 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SERIAL_TFT	(12 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_LCM  		(13 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SLCD  		LCD_CFG_MODE_LCM

/* LCD Control Register */
#define LCD_CTRL_PINMD		(1 << 31) /* This register set Pin distribution in 16-bit parallel mode
					    0: 16-bit data correspond with LCD_D[15:0]
					    1: 16-bit data correspond with LCD_D[17:10], LCD_D[8:1] */
#define LCD_CTRL_BST_BIT	28  /* Burst Length Selection */
#define LCD_CTRL_BST_MASK	(0x07 << LCD_CTRL_BST_BIT)
#define LCD_CTRL_BST_4	        (0 << LCD_CTRL_BST_BIT) /* 4-word */
#define LCD_CTRL_BST_8	        (1 << LCD_CTRL_BST_BIT) /* 8-word */
#define LCD_CTRL_BST_16	        (2 << LCD_CTRL_BST_BIT) /* 16-word */
#define LCD_CTRL_BST_32	        (3 << LCD_CTRL_BST_BIT) /* 32-word */
#define LCD_CTRL_BST_64         (4 << LCD_CTRL_BST_BIT)
#define LCD_CTRL_RGB565		(0 << 27) /* RGB565 mode(foreground 0 in OSD mode) */
#define LCD_CTRL_RGB555		(1 << 27) /* RGB555 mode(foreground 0 in OSD mode) */
#define LCD_CTRL_OFUP		(1 << 26) /* Output FIFO underrun protection enable */
#define LCD_CTRL_PDD_BIT	16  /* Load Palette Delay Counter */
#define LCD_CTRL_PDD_MASK	(0xff << LCD_CTRL_PDD_BIT)
#define LCD_CTRL_DACTE		(1 << 14) /* DAC loop back test */
#define LCD_CTRL_EOFM		(1 << 13) /* EOF interrupt mask */
#define LCD_CTRL_SOFM		(1 << 12) /* SOF interrupt mask */
#define LCD_CTRL_OFUM		(1 << 11) /* Output FIFO underrun interrupt mask */
#define LCD_CTRL_IFUM0		(1 << 10) /* Input FIFO 0 underrun interrupt mask */
#define LCD_CTRL_IFUM1		(1 << 9)  /* Input FIFO 1 underrun interrupt mask */
#define LCD_CTRL_LDDM		(1 << 8)  /* LCD disable done interrupt mask */
#define LCD_CTRL_QDM		(1 << 7)  /* LCD quick disable done interrupt mask */
#define LCD_CTRL_BEDN		(1 << 6)  /* Endian selection */
#define LCD_CTRL_PEDN		(1 << 5)  /* Endian in byte:0-msb first, 1-lsb first */
#define LCD_CTRL_DIS		(1 << 4)  /* Disable indicate bit */
#define LCD_CTRL_ENA		(1 << 3)  /* LCD enable bit */
#define LCD_CTRL_BPP_BIT	0  /* Bits Per Pixel */
#define LCD_CTRL_BPP_MASK	(0x07 << LCD_CTRL_BPP_BIT)
  #define LCD_CTRL_BPP_1	(0 << LCD_CTRL_BPP_BIT) /* 1 bpp */
  #define LCD_CTRL_BPP_2	(1 << LCD_CTRL_BPP_BIT) /* 2 bpp */
  #define LCD_CTRL_BPP_4	(2 << LCD_CTRL_BPP_BIT) /* 4 bpp */
  #define LCD_CTRL_BPP_8	(3 << LCD_CTRL_BPP_BIT) /* 8 bpp */
  #define LCD_CTRL_BPP_16	(4 << LCD_CTRL_BPP_BIT) /* 15/16 bpp */
  #define LCD_CTRL_BPP_18_24	(5 << LCD_CTRL_BPP_BIT) /* 18/24/32 bpp */
  #define LCD_CTRL_BPP_CMPS_24	(6 << LCD_CTRL_BPP_BIT) /* 24 compress bpp */
  #define LCD_CTRL_BPP_30	(7 << LCD_CTRL_BPP_BIT) /* 30 bpp */

/* LCD Status Register */
#define LCD_STATE_QD		(1 << 7) /* Quick Disable Done */
#define LCD_STATE_EOF		(1 << 5) /* EOF Flag */
#define LCD_STATE_SOF		(1 << 4) /* SOF Flag */
#define LCD_STATE_OFU		(1 << 3) /* Output FIFO Underrun */
#define LCD_STATE_IFU0		(1 << 2) /* Input FIFO 0 Underrun */
#define LCD_STATE_IFU1		(1 << 1) /* Input FIFO 1 Underrun */
#define LCD_STATE_LDD		(1 << 0) /* LCD Disabled */

/* OSD Configure Register */
#define LCD_OSDC_PREMULT1       (1 << 23) /* Premulti enable of fg1 ,data should be premultied by lcd */
#define LCD_OSDC_COEF_SLE1_BIT (21) /* Select coefficient for foreground1 */        
#define LCD_OSDC_COEF_SLE1_MASK (3 << LCD_OSDC_COEF_SLE1_BIT) /* Select coefficient for foreground1 */        
#define LCD_OSDC_PREMULT0       (1 << 20) /* Premulti enable of fg0, data should be premultied by lcd*/ 
#define LCD_OSDC_COEF_SLE0_BIT (18) /* Select coefficient for foreground0 */
#define LCD_OSDC_COEF_SLE0_MASK (3 << LCD_OSDC_COEF_SLE0_BIT) /* Select coefficient for foreground0 */
#define LCD_OSDC_ALPHAMD1       (1 << 17) /* Alpha blendind mode for foreground1 */ 
#define LCD_OSDC_SOFM1		(1 << 15) /* Start of frame interrupt mask for foreground 1 */
#define LCD_OSDC_EOFM1		(1 << 14) /* End of frame interrupt mask for foreground 1 */
#define LCD_OSDC_SOFM0		(1 << 11) /* Start of frame interrupt mask for foreground 0 */
#define LCD_OSDC_EOFM0		(1 << 10) /* End of frame interrupt mask for foreground 0 */
#define LCD_OSDC_DENDM          (1 << 9)  /* Display end interrupt mask */
#define LCD_OSDC_F1EN		(1 << 4) /* enable foreground 1 */
#define LCD_OSDC_F0EN		(1 << 3) /* enable foreground 0 */
#define LCD_OSDC_ALPHAEN	(1 << 2) /* enable alpha blending */
#define LCD_OSDC_ALPHAMD0	(1 << 1) /* alpha blending mode */
#define LCD_OSDC_OSDEN		(1 << 0) /* OSD mode enable */

/* OSD Controll Register */
#define LCD_OSDCTRL_REG0_555    (1 << 5) /* Bpp 16 RGB mode for fg0 */
#define LCD_OSDCTRL_REG0_565    (0 << 5)
#define LCD_OSDCTRL_RGB1_565	(0 << 4) /* foreground 1, 16bpp, 0-RGB565, 1-RGB555 */
#define LCD_OSDCTRL_RGB1_555	(1 << 4) /* foreground 1, 16bpp, 0-RGB565, 1-RGB555 */
#define LCD_OSDCTRL_OSDBPP_BIT	0 	 /* Bits Per Pixel of OSD Channel 1 */
#define LCD_OSDCTRL_OSDBPP_MASK	(0x7<<LCD_OSDCTRL_OSDBPP_BIT) 	 /* Bits Per Pixel of OSD Channel 1's MASK */
  #define LCD_OSDCTRL_OSDBPP_16	(4 << LCD_OSDCTRL_OSDBPP_BIT) /* RGB 15,16 bit*/
  #define LCD_OSDCTRL_OSDBPP_15_16	(4 << LCD_OSDCTRL_OSDBPP_BIT) /* RGB 15,16 bit*/
  #define LCD_OSDCTRL_OSDBPP_18_24	(5 << LCD_OSDCTRL_OSDBPP_BIT) /* RGB 18,24 bit*/
  #define LCD_OSDCTRL_OSDBPP_CMPS_24	(6 << LCD_OSDCTRL_OSDBPP_BIT) /* RGB compress 24 bit*/
  #define LCD_OSDCTRL_OSDBPP_30		(7 << LCD_OSDCTRL_OSDBPP_BIT) /* RGB 30 bit*/

/* OSD State Register */
#define LCD_OSDS_SOF1		(1 << 15) /* Start of frame flag for foreground 1 */
#define LCD_OSDS_EOF1		(1 << 14) /* End of frame flag for foreground 1 */
#define LCD_OSDS_SOF0		(1 << 11) /* Start of frame flag for foreground 0 */
#define LCD_OSDS_EOF0		(1 << 10) /* End of frame flag for foreground 0 */
#define LCD_OSDS_DEND           (1 << 8)  /* Display end */

/* Background0,1 Color Register */
#define LCD_BGC_RED_OFFSET	(1 << 16)  /* Red color offset */
#define LCD_BGC_RED_MASK	(0xFF<<LCD_BGC_RED_OFFSET)
#define LCD_BGC_GREEN_OFFSET	(1 << 8)   /* Green color offset */
#define LCD_BGC_GREEN_MASK	(0xFF<<LCD_BGC_GREEN_OFFSET)
#define LCD_BGC_BLUE_OFFSET	(1 << 0)   /* Blue color offset */
#define LCD_BGC_BLUE_MASK	(0xFF<<LCD_BGC_BLUE_OFFSET)

/* Foreground Color Key Register 0,1(foreground 0, foreground 1) */
#define LCD_KEY_KEYEN		(1 << 31)   /* enable color key */
#define LCD_KEY_KEYMD		(1 << 30)   /* color key mode */
#define LCD_KEY_RED_OFFSET	16  /* Red color offset */
#define LCD_KEY_RED_MASK	(0xFF<<LCD_KEY_RED_OFFSET)
#define LCD_KEY_GREEN_OFFSET	8   /* Green color offset */
#define LCD_KEY_GREEN_MASK	(0xFF<<LCD_KEY_GREEN_OFFSET)
#define LCD_KEY_BLUE_OFFSET	0   /* Blue color offset */
#define LCD_KEY_BLUE_MASK	(0xFF<<LCD_KEY_BLUE_OFFSET)
#define LCD_KEY_MASK		(LCD_KEY_RED_MASK|LCD_KEY_GREEN_MASK|LCD_KEY_BLUE_MASK)

/* Alpha Register*/
#define LCD_ALPHA_FG0_OFFSET    (0) 
#define LCD_ALPHA_FG0_MASK      (0xFF << LCD_ALPHA_FG0_OFFSET)
#define LCD_ALPHA_FG1_OFFSET    (8) 
#define LCD_ALPHA_FG1_MASK      (0xFF << LCD_ALPHA_FG0_OFFSET)

/* IPU Restart Register */
#define LCD_IPUR_IPUREN		(1 << 31)   /* IPU restart function enable*/
#define LCD_IPUR_IPURMASK	(0xFFFFFF)   /* IPU restart value mask*/

/* RGB Control Register */
#define LCD_RGBC_RGBDM		(1 << 15)   /* enable RGB Dummy data */
#define LCD_RGBC_DMM		(1 << 14)   /* RGB Dummy mode */
#define LCD_RGBC_YCC		(1 << 8)    /* RGB to YCC */
#define LCD_REGFMT              (1 << 7)    /* RGB format enable */
#define LCD_RGBC_ODDRGB_BIT	4	/* odd line serial RGB data arrangement */
#define LCD_RGBC_ODDRGB_MASK	(0x7<<LCD_RGBC_ODDRGB_BIT)
  #define LCD_RGBC_ODD_RGB	0
  #define LCD_RGBC_ODD_RBG	1
  #define LCD_RGBC_ODD_GRB	2
  #define LCD_RGBC_ODD_GBR	3
  #define LCD_RGBC_ODD_BRG	4
  #define LCD_RGBC_ODD_BGR	5
#define LCD_RGBC_EVENRGB_BIT	0	/* even line serial RGB data arrangement */
#define LCD_RGBC_EVENRGB_MASK	(0x7<<LCD_RGBC_EVENRGB_BIT)
  #define LCD_RGBC_EVEN_RGB	0
  #define LCD_RGBC_EVEN_RBG	1
  #define LCD_RGBC_EVEN_GRB	2
  #define LCD_RGBC_EVEN_GBR	3
  #define LCD_RGBC_EVEN_BRG	4
  #define LCD_RGBC_EVEN_BGR	5

/* Vertical Synchronize Register */
#define LCD_VSYNC_VPS_BIT	16  /* VSYNC pulse start in line clock, fixed to 0 */
#define LCD_VSYNC_VPS_MASK	(0xffff << LCD_VSYNC_VPS_BIT)
#define LCD_VSYNC_VPE_BIT	0   /* VSYNC pulse end in line clock */
#define LCD_VSYNC_VPE_MASK	(0xffff << LCD_VSYNC_VPS_BIT)

/* Horizontal Synchronize Register */
#define LCD_HSYNC_HPS_BIT	16  /* HSYNC pulse start position in dot clock */
#define LCD_HSYNC_HPS_MASK	(0xffff << LCD_HSYNC_HPS_BIT)
#define LCD_HSYNC_HPE_BIT	0   /* HSYNC pulse end position in dot clock */
#define LCD_HSYNC_HPE_MASK	(0xffff << LCD_HSYNC_HPE_BIT)

/* Virtual Area Setting Register */
#define LCD_VAT_HT_BIT		16  /* Horizontal Total size in dot clock */
#define LCD_VAT_HT_MASK		(0xffff << LCD_VAT_HT_BIT)
#define LCD_VAT_VT_BIT		0   /* Vertical Total size in dot clock */
#define LCD_VAT_VT_MASK		(0xffff << LCD_VAT_VT_BIT)

/* Display Area Horizontal Start/End Point Register */
#define LCD_DAH_HDS_BIT		16  /* Horizontal display area start in dot clock */
#define LCD_DAH_HDS_MASK	(0xffff << LCD_DAH_HDS_BIT)
#define LCD_DAH_HDE_BIT		0   /* Horizontal display area end in dot clock */
#define LCD_DAH_HDE_MASK	(0xffff << LCD_DAH_HDE_BIT)

/* Display Area Vertical Start/End Point Register */
#define LCD_DAV_VDS_BIT		16  /* Vertical display area start in line clock */
#define LCD_DAV_VDS_MASK	(0xffff << LCD_DAV_VDS_BIT)
#define LCD_DAV_VDE_BIT		0   /* Vertical display area end in line clock */
#define LCD_DAV_VDE_MASK	(0xffff << LCD_DAV_VDE_BIT)

/* Foreground XY Position Register */
#define LCD_XYP_YPOS_BIT	16  /* Y position bit of foreground 0 or 1 */
#define LCD_XYP_YPOS_MASK	(0xffff << LCD_XYP_YPOS_BIT)
#define LCD_XYP_XPOS_BIT	0   /* X position bit of foreground 0 or 1 */
#define LCD_XYP_XPOS_MASK	(0xffff << LCD_XYP_XPOS_BIT)

/* PS Signal Setting */
#define LCD_PS_PSS_BIT		16  /* PS signal start position in dot clock */
#define LCD_PS_PSS_MASK		(0xffff << LCD_PS_PSS_BIT)
#define LCD_PS_PSE_BIT		0   /* PS signal end position in dot clock */
#define LCD_PS_PSE_MASK		(0xffff << LCD_PS_PSE_BIT)

/* CLS Signal Setting */
#define LCD_CLS_CLSS_BIT	16  /* CLS signal start position in dot clock */
#define LCD_CLS_CLSS_MASK	(0xffff << LCD_CLS_CLSS_BIT)
#define LCD_CLS_CLSE_BIT	0   /* CLS signal end position in dot clock */
#define LCD_CLS_CLSE_MASK	(0xffff << LCD_CLS_CLSE_BIT)

/* SPL Signal Setting */
#define LCD_SPL_SPLS_BIT	16  /* SPL signal start position in dot clock */
#define LCD_SPL_SPLS_MASK	(0xffff << LCD_SPL_SPLS_BIT)
#define LCD_SPL_SPLE_BIT	0   /* SPL signal end position in dot clock */
#define LCD_SPL_SPLE_MASK	(0xffff << LCD_SPL_SPLE_BIT)

/* REV Signal Setting */
#define LCD_REV_REVS_BIT	16  /* REV signal start position in dot clock */
#define LCD_REV_REVS_MASK	(0xffff << LCD_REV_REVS_BIT)

/* DMA Command Register */
#define LCD_CMD_SOFINT		(1 << 31)
#define LCD_CMD_EOFINT		(1 << 30)
#define LCD_CMD_CMD		(1 << 29) /* indicate command in slcd mode */
#define LCD_CMD_PAL		(1 << 28)
#define LCD_CMD_COMPRE_EN       (1 << 27) /* It indicate this frm is 16/24bpp compressed or not*/
#define LCD_CMD_FRM_EN          (1 << 26) /* Indicat frm is enable*/
#define LCD_CMD_FIELD_SEL_EVEN  (1 << 25) /* Select even filed for interface*/
#define LCD_CMD_FIELD_SEL_ODD   (0 << 25) /* Select odd filed for interface*/
#define LCD_CMD_16X16_BLOCK      (1 << 24) /* Fetch data by 16x16 block*/
#define LCD_CMD_LEN_BIT		0
#define LCD_CMD_LEN_MASK	(0xffffff << LCD_CMD_LEN_BIT)

/* DMA Offsize Register 0,1 */

/* DMA Page Width Register 0,1 */

/* DMA Command Counter Register 0,1 */
#define LCD_CPOS_ALPHAMD       (1 << 31)
#define LCD_CPOS_RGB555        (1 << 30)
#define LCD_CPOS_REG565        (0 << 30)  
#define LCD_CPOS_BPP_BIT	27 	 /* Bits Per Pixel of OSD Channel 1 */
#define LCD_CPOS_BPP_MASK	(0x7<<LCD_CPOS_BPP_BIT) 	 /* Bits Per Pixel of OSD Channel 1's MASK */
#define LCD_CPOS_BPP_1	        (0 << LCD_CPOS_BPP_BIT) /* RGB 15,16 bit*/
#define LCD_CPOS_BPP_2	        (1 << LCD_CPOS_BPP_BIT) /* RGB 15,16 bit*/
#define LCD_CPOS_BPP_4	        (2 << LCD_CPOS_BPP_BIT) /* RGB 15,16 bit*/
#define LCD_CPOS_BPP_8	        (3 << LCD_CPOS_BPP_BIT) /* RGB 15,16 bit*/
#define LCD_CPOS_BPP_15_16      (4 << LCD_CPOS_BPP_BIT) /* RGB 15,16 bit*/
#define LCD_CPOS_BPP_18_24	(5 << LCD_CPOS_BPP_BIT) /* RGB 18,24 bit*/
#define LCD_CPOS_BPP_CMPS_24	(6 << LCD_CPOS_BPP_BIT) /* RGB compress 24 bit*/
#define LCD_CPOS_BPP_30		(7 << LCD_CPOS_BPP_BIT) /* RGB 30 bit*/
#define LCD_CPOS_PREMULT        (1 << 26) 
#define LCD_CPOS_COEF_SLE_MASK  (3 << 24)        
#define LCD_CPOS_COEF_SLE_BIT   (24)        
#define LCD_CPOS_POS_MASK      (0xffffff << 0)
#define LCD_CPOS_YPOS_BIT      (12) 
#define LCD_CPOS_XPOS_BIT      (0)

/* Foreground Size in Descriptor */
#define LCD_DESSIZE_ALPHA_MASK  (0xff << 24)
#define LCD_DESSIZE_ALPHA_BIT  (24)
#define LCD_DESSIZE_HEIGHT_BIT (12)
#define LCD_DESSIZE_WIDTH_BIT  (0)
#define LCD_DESSIZE_SIZE_MASK  (0xffffff << 0)

/* Priority level threshold configure Register*/
#define LCD_PCFG_DYNAMIC_PRI    (0 << 31) 
#define LCD_PCFG_ARBITER_PRI    (1 << 31)
#define LCD_PCFG_HP_BST_MASK    (7 << 28) /*Highest priority Burst Length Selection*/
#define LCD_PCFG_HP_BST_4       (0 << 28)
#define LCD_PCFG_HP_BST_8       (1 << 28)
#define LCD_PCFG_HP_BST_16      (2 << 28)
#define LCD_PCFG_HP_BST_32      (3 << 28)
#define LCD_PCFG_HP_BST_64      (4 << 28)
#define LCD_PCFG_PCFG2_MASK     (0x1ff << 18)
#define LCD_PCFG_PCFG1_MASK     (0x1ff << 9)
#define LCD_PCFG_PCFG0_MASK     (0x1ff << 0)

/* Image Enhancement Configure Register */
#define LCD_ENHCFG_DITHER_EN    (1 << 9)
#define LCD_ENHCFG_YCC2RGB_EN  (1 << 8) /* YCbCr to RGB enable */
#define LCD_ENHCFG_SATURATION_EN (1 << 7) /* Saturation enable */
#define LCD_ENHCFG_VEE_EN       (1 << 6) /* Visibility enhance enable */
#define LCD_ENHCFG_HUE_EN              (1 << 5) /* Hue enable */
#define LCD_ENHCFG_BRIGHTNESS_EN       (1 << 4) /* Brightness enable */
#define LCD_ENHCFG_CONTRAST_EN         (1 << 3) /* Contrast enable */ 
#define LCD_ENHCFG_RGB2YCC_EN          (1 << 2) /* RGB to YCbCr enable */
#define LCD_ENHCFG_GAMMA_EN            (1 << 1) /* Gamma control enable */
#define LCD_ENHCFG_ENH_EN              (1 << 0) /* Enhancement enable */

/* Color Space Conversion Configure */
#define LCD_CSCCFG_YCC2RGB_BIT           (2)
#define LCD_CSCCFG_YCC2RGBMD           (3 << 2)
#define LCD_CSCCFG_YCC2RGB_601WIDE     (0 << 2)
#define LCD_CSCCFG_YCC2RGB_601NARROW   (1 << 2)
#define LCD_CSCCFG_YCC2RGB_709WIDE     (2 << 2)
#define LCD_CSCCFG_YCC2RGB_709NARROW   (3 << 2)
#define LCD_CSCCFG_RGB2YCC_BIT           (0)
#define LCD_CSCCFG_RGB2YCCMD           (3 << 0)
#define LCD_CSCCFG_RGB2YCC_601WIDE     (0 << 2)
#define LCD_CSCCFG_RGB2YCC_601NARROW   (1 << 2)
#define LCD_CSCCFG_RGB2YCC_709WIDE     (2 << 2)
#define LCD_CSCCFG_RGB2YCC_709NARROW   (3 << 2)

/* LUMMA Configure */
#define LCD_LUMMACFG_BRIGHTNESS_BIT   (16) /* Brightness value */
#define LCD_LUMMACFG_CONTRAST_BIT     (0) /* Contrast value */
#define LCD_LUMMACFG_BRIGHTNESS_MASK   (0x7ff << 16) /* Brightness value */
#define LCD_LUMMACFG_CONTRAST_MASK     (0x7ff << 0) /* Contrast value */

/*Chroma0 Configure */
#define LCD_CHROCFG0_HUE_SIN_BIT      (16)
#define LCD_CHROCFG0_HUE_COS_BIT      (0)
#define LCD_CHROCFG0_HUE_SIN_MASK      (0xfff << 16)
#define LCD_CHROCFG0_HUE_COS_MASK      (0xfff << 0)

/*Chroma1 Configure */
#define LCD_CHROCFG1_SATURATION_MASK   (0x7ff << 0)

/*Dither Configure */
#define LCD_DITHERCFG_MD               (0x3f << 0)
#define LCD_DITHERCFG_MD_8             (0 << 0) 
#define LCD_DITHERCFG_MD_6             (1 << 0)  
#define LCD_DITHERCFG_MD_5             (2 << 0)  
#define LCD_DITHERCFG_MD_4             (3 << 0)        

/* Enhance Status */
#define LCD_ENHSTATUS_DITHER_DIS       (1 << 9)
#define LCD_ENHSTATUS_YCC2RGB_DIS      (1 << 8)
#define LCD_ENHSTATUS_SATURATION_DIS   (1 << 7)
#define LCD_ENHSTATUS_VEE_DIS          (1 << 6)
#define LCD_ENHSTATUS_HUE_DIS          (1 << 5)
#define LCD_ENHSTATUS_BRIGHTNESS_DIS   (1 << 4)
#define LCD_ENHSTATUS_CONTRAST_DIS     (1 << 3)
#define LCD_ENHSTATUS_REG2YCC_DIS      (1 << 2)
#define LCD_ENHSTATUS_GAMMA_DIS        (1 << 1)

/* GAMMA Configure */
#define LCD_GAMMACFG_DATA1_BIT        (16)  
#define LCD_GAMMACFG_DATA0_BIT        (0) 
#define LCD_GAMMACFG_DATA1_MASK        (0x2ff << 16)  
#define LCD_GAMMACFG_DATA0_MASK        (0x2ff << 0) 
#define LCD_GAMMACFG_LEN                  (0x7ff+1)

/* VEE Configure */
#define LCD_VEECFG_DATA1_BIT         16
#define LCD_VEECFG_DATA0_BIT         0
#define LCD_VEECFG_DATA1_MASK        (0x2ff << 16)  
#define LCD_VEECFG_DATA0_MASK        (0x2ff << 0) 
#define LCD_VEECFG_LEN                  (0x7ff+1) 

#ifndef __MIPS_ASSEMBLER

/*************************************************************************
 * SLCD (Smart LCD Controller)
 *************************************************************************/
#define __slcd_set_data_18bit() \
  ( REG_SLCD_CFG = (REG_SLCD_CFG & ~SLCD_CFG_DWIDTH_MASK) | SLCD_CFG_DWIDTH_18BIT )
#define __slcd_set_data_16bit() \
  ( REG_SLCD_CFG = (REG_SLCD_CFG & ~SLCD_CFG_DWIDTH_MASK) | SLCD_CFG_DWIDTH_16BIT )
#define __slcd_set_data_8bit_x3() \
  ( REG_SLCD_CFG = (REG_SLCD_CFG & ~SLCD_CFG_DWIDTH_MASK) | SLCD_CFG_DWIDTH_8BIT_x3 )
#define __slcd_set_data_8bit_x2() \
  ( REG_SLCD_CFG = (REG_SLCD_CFG & ~SLCD_CFG_DWIDTH_MASK) | SLCD_CFG_DWIDTH_8BIT_x2 )
#define __slcd_set_data_8bit_x1() \
  ( REG_SLCD_CFG = (REG_SLCD_CFG & ~SLCD_CFG_DWIDTH_MASK) | SLCD_CFG_DWIDTH_8BIT_x1 )
#define __slcd_set_data_24bit() \
  ( REG_SLCD_CFG = (REG_SLCD_CFG & ~SLCD_CFG_DWIDTH_MASK) | SLCD_CFG_DWIDTH_24BIT )
#define __slcd_set_data_9bit_x2() \
  ( REG_SLCD_CFG = (REG_SLCD_CFG & ~SLCD_CFG_DWIDTH_MASK) | SLCD_CFG_DWIDTH_9BIT_x2 )

#define __slcd_set_cmd_16bit() \
  ( REG_SLCD_CFG = (REG_SLCD_CFG & ~SLCD_CFG_CWIDTH_MASK) | SLCD_CFG_CWIDTH_16BIT )
#define __slcd_set_cmd_8bit() \
  ( REG_SLCD_CFG = (REG_SLCD_CFG & ~SLCD_CFG_CWIDTH_MASK) | SLCD_CFG_CWIDTH_8BIT )
#define __slcd_set_cmd_18bit() \
  ( REG_SLCD_CFG = (REG_SLCD_CFG & ~SLCD_CFG_CWIDTH_MASK) | SLCD_CFG_CWIDTH_18BIT )
#define __slcd_set_cmd_24bit() \
  ( REG_SLCD_CFG = (REG_SLCD_CFG & ~SLCD_CFG_CWIDTH_MASK) | SLCD_CFG_CWIDTH_24BIT )

#define __slcd_set_cs_high()        ( REG_SLCD_CFG |= SLCD_CFG_CS_ACTIVE_HIGH )
#define __slcd_set_cs_low()         ( REG_SLCD_CFG &= ~SLCD_CFG_CS_ACTIVE_HIGH )

#define __slcd_set_rs_high()        ( REG_SLCD_CFG |= SLCD_CFG_RS_CMD_HIGH )
#define __slcd_set_rs_low()         ( REG_SLCD_CFG &= ~SLCD_CFG_RS_CMD_HIGH )

#define __slcd_set_clk_falling()    ( REG_SLCD_CFG &= ~SLCD_CFG_CLK_ACTIVE_RISING )
#define __slcd_set_clk_rising()     ( REG_SLCD_CFG |= SLCD_CFG_CLK_ACTIVE_RISING )

#define __slcd_set_parallel_type()  ( REG_SLCD_CFG &= ~SLCD_CFG_TYPE_SERIAL )
#define __slcd_set_serial_type()    ( REG_SLCD_CFG |= SLCD_CFG_TYPE_SERIAL )

/* SLCD Status Register */
#define __slcd_is_busy()            ( REG_SLCD_STATE & SLCD_STATE_BUSY )

/* SLCD Data Register */
#define __slcd_set_cmd_rs()         ( REG_SLCD_DATA |= SLCD_DATA_RS_COMMAND)
#define __slcd_set_data_rs()        ( REG_SLCD_DATA &= ~SLCD_DATA_RS_COMMAND)


/***************************************************************************
 * LCD
 ***************************************************************************/

/***************************************************************************
 * LCD
 ***************************************************************************/
#define __lcd_as_smart_lcd(n) 		( REG_LCD_CFG(n) |= ( LCD_CFG_LCDPIN_SLCD | LCD_CFG_MODE_SLCD))
#define __lcd_as_general_lcd(n) 		( REG_LCD_CFG(n) &= ~( LCD_CFG_LCDPIN_SLCD | LCD_CFG_MODE_SLCD))

#define __lcd_enable_tvepeh(n) 		( REG_LCD_CFG(n) |= LCD_CFG_TVEPEH )
#define __lcd_disable_tvepeh(n) 		( REG_LCD_CFG(n) &= ~LCD_CFG_TVEPEH )

#define __lcd_enable_fuhold(n) 		( REG_LCD_CFG(n) |= LCD_CFG_FUHOLD )
#define __lcd_disable_fuhold(n) 		( REG_LCD_CFG(n) &= ~LCD_CFG_FUHOLD )

#define __lcd_des_8word(n) 		( REG_LCD_CFG(n) |= LCD_CFG_NEWDES )
#define __lcd_des_4word(n) 		( REG_LCD_CFG(n) &= ~LCD_CFG_NEWDES )

#define __lcd_enable_bypass_pal(n) 	( REG_LCD_CFG(n) |= LCD_CFG_PALBP )
#define __lcd_disable_bypass_pal(n) 	( REG_LCD_CFG(n) &= ~LCD_CFG_PALBP )

#define __lcd_set_lcdpnl_term(n)		( REG_LCD_CFG(n) |= LCD_CFG_TVEN )
#define __lcd_set_tv_term(n)		( REG_LCD_CFG(n) &= ~LCD_CFG_TVEN )

#define __lcd_enable_auto_recover(n) 	( REG_LCD_CFG(n) |= LCD_CFG_RECOVER )
#define __lcd_disable_auto_recover(n) 	( REG_LCD_CFG(n) &= ~LCD_CFG_RECOVER )

#define __lcd_enable_dither(n) 	        ( REG_LCD_CFG(n) |= LCD_CFG_DITHER )
#define __lcd_disable_dither(n) 	        ( REG_LCD_CFG(n) &= ~LCD_CFG_DITHER )

#define __lcd_disable_ps_mode(n)	        ( REG_LCD_CFG(n) |= LCD_CFG_PSM )
#define __lcd_enable_ps_mode(n)	        ( REG_LCD_CFG(n) &= ~LCD_CFG_PSM )

#define __lcd_disable_cls_mode(n) 	( REG_LCD_CFG(n) |= LCD_CFG_CLSM )
#define __lcd_enable_cls_mode(n)	        ( REG_LCD_CFG(n) &= ~LCD_CFG_CLSM )

#define __lcd_disable_spl_mode(n) 	( REG_LCD_CFG(n) |= LCD_CFG_SPLM )
#define __lcd_enable_spl_mode(n)	        ( REG_LCD_CFG(n) &= ~LCD_CFG_SPLM )

#define __lcd_disable_rev_mode(n) 	( REG_LCD_CFG(n) |= LCD_CFG_REVM )
#define __lcd_enable_rev_mode(n)	        ( REG_LCD_CFG(n) &= ~LCD_CFG_REVM )

#define __lcd_disable_hsync_mode(n) 	( REG_LCD_CFG(n) |= LCD_CFG_HSYNM )
#define __lcd_enable_hsync_mode(n)	( REG_LCD_CFG(n) &= ~LCD_CFG_HSYNM )

#define __lcd_disable_pclk_mode(n) 	( REG_LCD_CFG(n) |= LCD_CFG_PCLKM )
#define __lcd_enable_pclk_mode(n)	( REG_LCD_CFG(n) &= ~LCD_CFG_PCLKM )

#define __lcd_normal_outdata(n)          ( REG_LCD_CFG(n) &= ~LCD_CFG_INVDAT )
#define __lcd_inverse_outdata(n)         ( REG_LCD_CFG(n) |= LCD_CFG_INVDAT )

#define __lcd_sync_input(n)              ( REG_LCD_CFG(n) |= LCD_CFG_SYNDIR_IN )
#define __lcd_sync_output(n)             ( REG_LCD_CFG(n) &= ~LCD_CFG_SYNDIR_IN )

#define __lcd_hsync_active_high(n)       ( REG_LCD_CFG(n) &= ~LCD_CFG_HSP )
#define __lcd_hsync_active_low(n)        ( REG_LCD_CFG(n) |= LCD_CFG_HSP )

#define __lcd_pclk_rising(n)             ( REG_LCD_CFG(n) &= ~LCD_CFG_PCP )
#define __lcd_pclk_falling(n)            ( REG_LCD_CFG(n) |= LCD_CFG_PCP )

#define __lcd_de_active_high(n)          ( REG_LCD_CFG(n) &= ~LCD_CFG_DEP )
#define __lcd_de_active_low(n)           ( REG_LCD_CFG(n) |= LCD_CFG_DEP )

#define __lcd_vsync_rising(n)            ( REG_LCD_CFG(n) &= ~LCD_CFG_VSP )
#define __lcd_vsync_falling(n)           ( REG_LCD_CFG(n) |= LCD_CFG_VSP )

#define __lcd_set_16_tftpnl(n) \
  ( REG_LCD_CFG(n) = (REG_LCD_CFG(n) & ~LCD_CFG_MODE_TFT_MASK) | LCD_CFG_MODE_TFT_16BIT )

#define __lcd_set_18_tftpnl(n) \
  ( REG_LCD_CFG(n) = (REG_LCD_CFG(n) & ~LCD_CFG_MODE_TFT_MASK) | LCD_CFG_MODE_TFT_18BIT )

#define __lcd_set_24_tftpnl(n)		( REG_LCD_CFG(n) |= LCD_CFG_MODE_TFT_24BIT )

/* 
 * n=1,2,4,8 for single mono-STN 
 * n=4,8 for dual mono-STN
 */
#define __lcd_set_panel_datawidth(n, m)		\
do { 						\
	REG_LCD_CFG(n) &= ~LCD_CFG_PDW_MASK; 	\
	REG_LCD_CFG(n) |= LCD_CFG_PDW_m##;		\
} while (0)

/* m = LCD_CFG_MODE_GENERUIC_TFT_xxx */
#define __lcd_set_panel_mode(n, m) 		\
do {						\
	REG_LCD_CFG(n) &= ~LCD_CFG_MODE_MASK;	\
	REG_LCD_CFG(n) |= (m);			\
} while(0)

/* n=4,8,16 */
#define __lcd_set_burst_length(n, m) 		\
do {						\
	REG_LCD_CTRL(n) &= ~LCD_CTRL_BST_MASK;	\
	REG_LCD_CTRL(n) |= LCD_CTRL_BST_m##;	\
} while (0)

#define __lcd_select_rgb565(n)		( REG_LCD_CTRL(n) &= ~LCD_CTRL_RGB555 )
#define __lcd_select_rgb555(n)		( REG_LCD_CTRL(n) |= LCD_CTRL_RGB555 )

#define __lcd_set_ofup(n)		( REG_LCD_CTRL(n) |= LCD_CTRL_OFUP )
#define __lcd_clr_ofup(n)		( REG_LCD_CTRL(n) &= ~LCD_CTRL_OFUP )

/* n=2,4,16 */
#define __lcd_set_stn_frc(n, m) 			\
do {						\
	REG_LCD_CTRL(n) &= ~LCD_CTRL_FRC_MASK;	\
	REG_LCD_CTRL(n) |= LCD_CTRL_FRC_m##;	\
} while (0)

#define __lcd_enable_eof_intr(n)		( REG_LCD_CTRL(n) |= LCD_CTRL_EOFM )
#define __lcd_disable_eof_intr(n)	( REG_LCD_CTRL(n) &= ~LCD_CTRL_EOFM )

#define __lcd_enable_sof_intr(n)		( REG_LCD_CTRL(n) |= LCD_CTRL_SOFM )
#define __lcd_disable_sof_intr(n)	( REG_LCD_CTRL(n) &= ~LCD_CTRL_SOFM )

#define __lcd_enable_ofu_intr(n)		( REG_LCD_CTRL(n) |= LCD_CTRL_OFUM )
#define __lcd_disable_ofu_intr(n)	( REG_LCD_CTRL(n) &= ~LCD_CTRL_OFUM )

#define __lcd_enable_ifu0_intr(n)	( REG_LCD_CTRL(n) |= LCD_CTRL_IFUM0 )
#define __lcd_disable_ifu0_intr(n)	( REG_LCD_CTRL(n) &= ~LCD_CTRL_IFUM0 )

#define __lcd_enable_ifu1_intr(n)	( REG_LCD_CTRL(n) |= LCD_CTRL_IFUM1 )
#define __lcd_disable_ifu1_intr(n)	( REG_LCD_CTRL(n) &= ~LCD_CTRL_IFUM1 )

#define __lcd_enable_ldd_intr(n)		( REG_LCD_CTRL(n) |= LCD_CTRL_LDDM )
#define __lcd_disable_ldd_intr(n)	( REG_LCD_CTRL(n) &= ~LCD_CTRL_LDDM )

#define __lcd_enable_qd_intr(n)		( REG_LCD_CTRL(n) |= LCD_CTRL_QDM )
#define __lcd_disable_qd_intr(n)		( REG_LCD_CTRL(n) &= ~LCD_CTRL_QDM )

#define __lcd_reverse_byte_endian(n)	( REG_LCD_CTRL(n) |= LCD_CTRL_BEDN )
#define __lcd_normal_byte_endian(n)	( REG_LCD_CTRL(n) &= ~LCD_CTRL_BEDN )

#define __lcd_pixel_endian_little(n)	( REG_LCD_CTRL(n) |= LCD_CTRL_PEDN )
#define __lcd_pixel_endian_big(n)	( REG_LCD_CTRL(n) &= ~LCD_CTRL_PEDN )

#define __lcd_set_dis(n)			( REG_LCD_CTRL(n) |= LCD_CTRL_DIS )
#define __lcd_clr_dis(n)			( REG_LCD_CTRL(n) &= ~LCD_CTRL_DIS )

#define __lcd_set_ena(n)			( REG_LCD_CTRL(n) |= LCD_CTRL_ENA )
#define __lcd_clr_ena(n)			( REG_LCD_CTRL(n) &= ~LCD_CTRL_ENA )
#define __is_lcd_ena(n)                         ( REG_LCD_CTRL(n) & LCD_CTRL_ENA ) 

/* n=1,2,4,8,16 */
#define __lcd_set_bpp(n, m)						\
  ( REG_LCD_CTRL(n) = (REG_LCD_CTRL(n) & ~LCD_CTRL_BPP_MASK) | LCD_CTRL_BPP_##m )

/* LCD status register indication */

#define __lcd_quick_disable_done(n)	( REG_LCD_STATE(n) & LCD_STATE_QD )
#define __lcd_disable_done(n)		( REG_LCD_STATE(n) & LCD_STATE_LDD )
#define __lcd_infifo0_underrun(n)	( REG_LCD_STATE(n) & LCD_STATE_IFU0 )
#define __lcd_infifo1_underrun(n)	( REG_LCD_STATE(n) & LCD_STATE_IFU1 )
#define __lcd_outfifo_underrun(n)	( REG_LCD_STATE(n) & LCD_STATE_OFU )
#define __lcd_start_of_frame(n)		( REG_LCD_STATE(n) & LCD_STATE_SOF )
#define __lcd_end_of_frame(n)		( REG_LCD_STATE(n) & LCD_STATE_EOF )

#define __lcd_clr_outfifounderrun(n)	( REG_LCD_STATE(n) &= ~LCD_STATE_OFU )
#define __lcd_clr_infifo0underrun(n)	( REG_LCD_STATE(n) &= ~LCD_STATE_IFU0 )
#define __lcd_clr_infifo1underrun(n)	( REG_LCD_STATE(n) &= ~LCD_STATE_IFU1 )
#define __lcd_clr_sof(n)			( REG_LCD_STATE(n) &= ~LCD_STATE_SOF )
#define __lcd_clr_eof(n)			( REG_LCD_STATE(n) &= ~LCD_STATE_EOF )
#define __lcd_clr_ldd(n)			( REG_LCD_STATE(n) &= ~LCD_STATE_LDD )
#define __lcd_state_clr_all(n)                        ( REG_LCD_STATE(n) = 0 )
#define __lcd_get_state(n)                      ( REG_LCD_STATE(n) )

/* OSD functions */
#define __lcd_enable_osd(n) 	(REG_LCD_OSDC(n) |= LCD_OSDC_OSDEN)
#define __lcd_enable_f0(n) 	(REG_LCD_OSDC(n) |= LCD_OSDC_F0EN)
#define __lcd_enable_f1(n)	(REG_LCD_OSDC(n) |= LCD_OSDC_F1EN)
#define __lcd_enable_alpha(n) 	(REG_LCD_OSDC(n) |= LCD_OSDC_ALPHAEN)
#define __lcd_enable_alphamd(n)	(REG_LCD_OSDC(n) |= LCD_OSDC_ALPHAMD)

#define __lcd_disable_osd(n)	(REG_LCD_OSDC(n) &= ~LCD_OSDC_OSDEN)
#define __lcd_disable_f0(n) 	(REG_LCD_OSDC(n) &= ~LCD_OSDC_F0EN)
#define __lcd_disable_f1(n) 	(REG_LCD_OSDC(n) &= ~LCD_OSDC_F1EN)
#define __lcd_disable_alpha(n)	(REG_LCD_OSDC(n) &= ~LCD_OSDC_ALPHAEN)
#define __lcd_disable_alphamd(n)	(REG_LCD_OSDC(n) &= ~LCD_OSDC_ALPHAMD)

/* OSD Controll Register */
#define __lcd_fg1_use_ipu(n) 		(REG_LCD_OSDCTRL(n) |= LCD_OSDCTRL_IPU)
#define __lcd_fg1_use_dma_chan1(n) 	(REG_LCD_OSDCTRL(n) &= ~LCD_OSDCTRL_IPU)
#define __lcd_fg1_unuse_ipu(n) 		__lcd_fg1_use_dma_chan1()
#define __lcd_osd_rgb555_mode(n)         ( REG_LCD_OSDCTRL(n) |= LCD_OSDCTRL_RGB555 )
#define __lcd_osd_rgb565_mode(n)         ( REG_LCD_OSDCTRL(n) &= ~LCD_OSDCTRL_RGB555 )
#define __lcd_osd_change_size(n)         ( REG_LCD_OSDCTRL(n) |= LCD_OSDCTRL_CHANGES )
#define __lcd_osd_bpp_15_16(nn) \
  ( REG_LCD_OSDCTRL(n) = (REG_LCD_OSDCTRL(n) & ~LCD_OSDCTRL_OSDBPP_MASK) | LCD_OSDCTRL_OSDBPP_15_16 )
#define __lcd_osd_bpp_18_24() \
  ( REG_LCD_OSDCTRL(n) = (REG_LCD_OSDCTRL(n) & ~LCD_OSDCTRL_OSDBPP_MASK) | LCD_OSDCTRL_OSDBPP_18_24 )

/* OSD State Register */
#define __lcd_start_of_fg1(n)		( REG_LCD_OSDS(n) & LCD_OSDS_SOF1 )
#define __lcd_end_of_fg1(n)		( REG_LCD_OSDS(n) & LCD_OSDS_EOF1 )
#define __lcd_start_of_fg0(n)		( REG_LCD_OSDS(n) & LCD_OSDS_SOF0 )
#define __lcd_end_of_fg0(n)		( REG_LCD_OSDS(n) & LCD_OSDS_EOF0 )

#define __lcd_osds_clr_all(n)           ( REG_LCD_OSDS(n) = 0)

/* Foreground Color Key Register 0,1(foreground 0, foreground 1) */
#define __lcd_enable_colorkey0(n)	(REG_LCD_KEY0(n) |= LCD_KEY_KEYEN)
#define __lcd_enable_colorkey1(n)	(REG_LCD_KEY1(n) |= LCD_KEY_KEYEN)
#define __lcd_enable_colorkey0_md(n) 	(REG_LCD_KEY0(n) |= LCD_KEY_KEYMD)
#define __lcd_enable_colorkey1_md(n) 	(REG_LCD_KEY1(n) |= LCD_KEY_KEYMD)
#define __lcd_set_colorkey0(n,key) 	(REG_LCD_KEY0(n) = (REG_LCD_KEY0(n)&~0xFFFFFF)|(key))
#define __lcd_set_colorkey1(n,key) 	(REG_LCD_KEY1(n) = (REG_LCD_KEY1(n)&~0xFFFFFF)|(key))

#define __lcd_disable_colorkey0(n) 	(REG_LCD_KEY0(n) &= ~LCD_KEY_KEYEN)
#define __lcd_disable_colorkey1(n) 	(REG_LCD_KEY1(n) &= ~LCD_KEY_KEYEN)
#define __lcd_disable_colorkey0_md(n) 	(REG_LCD_KEY0(n) &= ~LCD_KEY_KEYMD)
#define __lcd_disable_colorkey1_md(n) 	(REG_LCD_KEY1(n) &= ~LCD_KEY_KEYMD)

/* IPU Restart Register */
#define __lcd_enable_ipu_restart(n) 	(REG_LCD_IPUR(n) |= LCD_IPUR_IPUREN)
#define __lcd_disable_ipu_restart(n) 	(REG_LCD_IPUR(n) &= ~LCD_IPUR_IPUREN)
#define __lcd_set_ipu_restart_triger(n,m)	(REG_LCD_IPUR(n) = (REG_LCD_IPUR(n)&(~0xFFFFFF))|(m))

/* RGB Control Register */
#define __lcd_enable_rgb_dummy(n) 	(REG_LCD_RGBC(n) |= LCD_RGBC_RGBDM)
#define __lcd_disable_rgb_dummy(n) 	(REG_LCD_RGBC(n) &= ~LCD_RGBC_RGBDM)

#define __lcd_dummy_rgb(n) 	(REG_LCD_RGBC(n) |= LCD_RGBC_DMM)
#define __lcd_rgb_dummy(n) 	(REG_LCD_RGBC(n) &= ~LCD_RGBC_DMM)

#define __lcd_rgb2ycc(n) 	(REG_LCD_RGBC(n) |= LCD_RGBC_YCC)
#define __lcd_notrgb2ycc(n) 	(REG_LCD_RGBC(n) &= ~LCD_RGBC_YCC)

#define __lcd_odd_mode_rgb(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_ODDRGB_MASK) | LCD_RGBC_ODD_RGB )
#define __lcd_odd_mode_rbg(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_ODDRGB_MASK) | LCD_RGBC_ODD_RBG )
#define __lcd_odd_mode_grb(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_ODDRGB_MASK) | LCD_RGBC_ODD_GRB)

#define __lcd_odd_mode_gbr(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_ODDRGB_MASK) | LCD_RGBC_ODD_GBR)
#define __lcd_odd_mode_brg(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_ODDRGB_MASK) | LCD_RGBC_ODD_BRG)
#define __lcd_odd_mode_bgr(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_ODDRGB_MASK) | LCD_RGBC_ODD_BGR)

#define __lcd_even_mode_rgb(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_EVENRGB_MASK) | LCD_RGBC_EVEN_RGB )
#define __lcd_even_mode_rbg(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_EVENRGB_MASK) | LCD_RGBC_EVEN_RBG )
#define __lcd_even_mode_grb(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_EVENRGB_MASK) | LCD_RGBC_EVEN_GRB)

#define __lcd_even_mode_gbr(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_EVENRGB_MASK) | LCD_RGBC_EVEN_GBR)
#define __lcd_even_mode_brg(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_EVENRGB_MASK) | LCD_RGBC_EVEN_BRG)
#define __lcd_even_mode_bgr(n) \
  ( REG_LCD_RGBC(n) = (REG_LCD_RGBC(n) & ~LCD_RGBC_EVENRGB_MASK) | LCD_RGBC_EVEN_BGR)

/* Vertical Synchronize Register */
#define __lcd_vsync_get_vps(n) \
  ( (REG_LCD_VSYNC(n) & LCD_VSYNC_VPS_MASK) >> LCD_VSYNC_VPS_BIT )

#define __lcd_vsync_get_vpe(n) \
  ( (REG_LCD_VSYNC(n) & LCD_VSYNC_VPE_MASK) >> LCD_VSYNC_VPE_BIT )
#define __lcd_vsync_set_vpe(n, m)				\
do {							\
	REG_LCD_VSYNC(n) &= ~LCD_VSYNC_VPE_MASK;		\
	REG_LCD_VSYNC(n) |= (m) << LCD_VSYNC_VPE_BIT;	\
} while (0)

#define __lcd_hsync_get_hps(n) \
  ( (REG_LCD_HSYNC(n) & LCD_HSYNC_HPS_MASK) >> LCD_HSYNC_HPS_BIT )
#define __lcd_hsync_set_hps(n,m)				\
do {							\
	REG_LCD_HSYNC(n) &= ~LCD_HSYNC_HPS_MASK;		\
	REG_LCD_HSYNC(n) |= (m) << LCD_HSYNC_HPS_BIT;	\
} while (0)

#define __lcd_hsync_get_hpe(n) \
  ( (REG_LCD_HSYNC(n) & LCD_HSYNC_HPE_MASK) >> LCD_VSYNC_HPE_BIT )
#define __lcd_hsync_set_hpe(n, m)				\
do {							\
	REG_LCD_HSYNC(n) &= ~LCD_HSYNC_HPE_MASK;		\
	REG_LCD_HSYNC(n) |= (m) << LCD_HSYNC_HPE_BIT;	\
} while (0)

#define __lcd_vat_get_ht(n) \
  ( (REG_LCD_VAT(n) & LCD_VAT_HT_MASK) >> LCD_VAT_HT_BIT )
#define __lcd_vat_set_ht(n, m) 				\
do {							\
	REG_LCD_VAT(n) &= ~LCD_VAT_HT_MASK;		\
	REG_LCD_VAT(n) |= (m) << LCD_VAT_HT_BIT;		\
} while (0)

#define __lcd_vat_get_vt(n) \
  ( (REG_LCD_VAT(n) & LCD_VAT_VT_MASK) >> LCD_VAT_VT_BIT )
#define __lcd_vat_set_vt(n, m) 				\
do {							\
	REG_LCD_VAT(n) &= ~LCD_VAT_VT_MASK;		\
	REG_LCD_VAT(n) |= (m) << LCD_VAT_VT_BIT;		\
} while (0)

#define __lcd_dah_get_hds(n) \
  ( (REG_LCD_DAH(n) & LCD_DAH_HDS_MASK) >> LCD_DAH_HDS_BIT )
#define __lcd_dah_set_hds(n, m) 				\
do {							\
	REG_LCD_DAH(n) &= ~LCD_DAH_HDS_MASK;		\
	REG_LCD_DAH(n) |= (m) << LCD_DAH_HDS_BIT;		\
} while (0)

#define __lcd_dah_get_hde(n) \
  ( (REG_LCD_DAH(n) & LCD_DAH_HDE_MASK) >> LCD_DAH_HDE_BIT )
#define __lcd_dah_set_hde(n,m) 				\
do {							\
	REG_LCD_DAH(n) &= ~LCD_DAH_HDE_MASK;		\
	REG_LCD_DAH(n) |= (m) << LCD_DAH_HDE_BIT;		\
} while (0)

#define __lcd_dav_get_vds(n) \
  ( (REG_LCD_DAV(n) & LCD_DAV_VDS_MASK) >> LCD_DAV_VDS_BIT )
#define __lcd_dav_set_vds(n,m) 				\
do {							\
	REG_LCD_DAV(n) &= ~LCD_DAV_VDS_MASK;		\
	REG_LCD_DAV(n) |= (m) << LCD_DAV_VDS_BIT;		\
} while (0)

#define __lcd_dav_get_vde(n) \
  ( (REG_LCD_DAV(n) & LCD_DAV_VDE_MASK) >> LCD_DAV_VDE_BIT )
#define __lcd_dav_set_vde(n,m) 				\
do {							\
	REG_LCD_DAV(n) &= ~LCD_DAV_VDE_MASK;		\
	REG_LCD_DAV(n) |= (m) << LCD_DAV_VDE_BIT;		\
} while (0)

/* DMA Command Register */
#define __lcd_cmd0_set_sofint(n)		( REG_LCD_CMD0(n) |= LCD_CMD_SOFINT )
#define __lcd_cmd0_clr_sofint(n)		( REG_LCD_CMD0(n) &= ~LCD_CMD_SOFINT )
#define __lcd_cmd1_set_sofint(n)		( REG_LCD_CMD1(n) |= LCD_CMD_SOFINT )
#define __lcd_cmd1_clr_sofint(n)		( REG_LCD_CMD1(n) &= ~LCD_CMD_SOFINT )

#define __lcd_cmd0_set_eofint(n)		( REG_LCD_CMD0(n) |= LCD_CMD_EOFINT )
#define __lcd_cmd0_clr_eofint(n)		( REG_LCD_CMD0(n) &= ~LCD_CMD_EOFINT )
#define __lcd_cmd1_set_eofint(n)		( REG_LCD_CMD1(n) |= LCD_CMD_EOFINT )
#define __lcd_cmd1_clr_eofint(n)		( REG_LCD_CMD1(n) &= ~LCD_CMD_EOFINT )

#define __lcd_cmd0_set_pal(n)		( REG_LCD_CMD0(n) |= LCD_CMD_PAL )
#define __lcd_cmd0_clr_pal(n)		( REG_LCD_CMD0(n) &= ~LCD_CMD_PAL )

#define __lcd_cmd0_get_len(n) \
  ( (REG_LCD_CMD0(n) & LCD_CMD_LEN_MASK) >> LCD_CMD_LEN_BIT )
#define __lcd_cmd1_get_len(n) \
  ( (REG_LCD_CMD1(n) & LCD_CMD_LEN_MASK) >> LCD_CMD_LEN_BIT )


/* Enhancement Configure */
#define __gamma_disable_done(n)           (REG_LCD_ENHSTATUS(n) |= LCD_ENHSTATUS_GAMMA_DIS)
#define __rgb2ycc_disable_done(n)         (REG_LCD_ENHSTATUS(n) |= LCD_ENHSTATUS_REG2YCC_DIS)
#define __contrast_disable_done(n)        (REG_LCD_ENHSTATUS(n) |= LCD_ENHSTATUS_CONTRAST_DIS)
#define __brightness_disable_done(n)      (REG_LCD_ENHSTATUS(n) |= LCD_ENHSTATUS_BRIGHTNESS_DIS)
#define __hue_disable_done(n)             (REG_LCD_ENHSTATUS(n) |= LCD_ENHSTATUS_HUE_DIS)
#define __vee_disable_done(n)             (REG_LCD_ENHSTATUS(n) |= LCD_ENHSTATUS_VEE_DIS)
#define __saturation_disable_done(n)      (REG_LCD_ENHSTATUS(n) |= LCD_ENHSTATUS_SATURATION_DIS)
#define __ycc2rgb_disable_done(n)         (REG_LCD_ENHSTATUS(n) |= LCD_ENHSTATUS_YCC2RGB_DIS)
#define __dither_disable_done(n)          (REG_LCD_ENHSTATUS(n) |= LCD_ENHSTATUS_DITHER_DIS)
#define __is_enh_enabled(n)               ((REG_LCD_ENHCFG(n) | LCD_ENHCFG_ENH_EN)) 

#define __gamma_enable(n)                 (REG_LCD_ENHCFG(n) |= LCD_ENHCFG_GAMMA_EN)
#define __rgb2ycc_enable(n)               (REG_LCD_ENHCFG(n) |= LCD_ENHCFG_RGB2YCC_EN)
#define __contrast_enable(n)              (REG_LCD_ENHCFG(n) |= LCD_ENHCFG_CONTRAST_EN)
#define __brightness_enable(n)            (REG_LCD_ENHCFG(n) |= LCD_ENHCFG_BRIGHTNESS_EN)
#define __hue_enable(n)                   (REG_LCD_ENHCFG(n) |= LCD_ENHCFG_HUE_EN)
#define __vee_enable(n)                   (REG_LCD_ENHCFG(n) |= LCD_ENHCFG_VEE_EN)
#define __saturation_enable(n)            (REG_LCD_ENHCFG(n) |= LCD_ENHCFG_SATURATION_EN)
#define __ycc2rgb_enable(n)               (REG_LCD_ENHCFG(n) |= LCD_ENHCFG_YCC2RGB_EN)
#define __dither_enable(n)                (REG_LCD_ENHCFG(n) |= LCD_ENHCFG_DITHER_EN)
#define __enh_enable(n)                   (REG_LCD_ENHCFG(n) |= LCD_ENHCFG_ENH_EN)

#define __gamma_disable(n)                (REG_LCD_ENHCFG(n) &= ~LCD_ENHCFG_GAMMA_EN)
#define __rgb2ycc_disable(n)              (REG_LCD_ENHCFG(n) &= ~LCD_ENHCFG_RGB2YCC_EN)
#define __contrast_disable(n)             (REG_LCD_ENHCFG(n) &= ~LCD_ENHCFG_CONTRAST_EN)
#define __brightness_disable(n)           (REG_LCD_ENHCFG(n) &= ~LCD_ENHCFG_BRIGHTNESS_EN)
#define __hue_disable(n)                  (REG_LCD_ENHCFG(n) &= ~LCD_ENHCFG_HUE_EN)
#define __vee_disable(n)                  (REG_LCD_ENHCFG(n) &= ~LCD_ENHCFG_VEE_EN)
#define __saturation_disable(n)           (REG_LCD_ENHCFG(n) &= ~LCD_ENHCFG_SATURATION_EN)
#define __ycc2rgb_disable(n)              (REG_LCD_ENHCFG(n) &= ~LCD_ENHCFG_YCC2RGB_EN)
#define __dither_disable(n)               (REG_LCD_ENHCFG(n) &= ~LCD_ENHCFG_DITHER_EN)
#define __enh_disable(n)                  (REG_LCD_ENHCFG(n) &= ~LCD_ENHCFG_ENH_EN)

#define __set_dither_value(n, m)            (REG_LCD_DITHERCFG(n) = m)
#define __set_ycc2rgb_value(n, m)           (REG_LCD_CSCCFG(n) = (REG_LCD_CSCCFG(n) & ~LCD_CSCCFG_YCC2RGBMD) | (m << LCD_CSCCFG_YCC2RGB_BIT))
#define __set_rgb2ycc_value(n, m)           (REG_LCD_CSCCFG(n) = (REG_LCD_CSCCFG(n) & ~LCD_CSCCFG_RGB2YCCMD) | (m << LCD_CSCCFG_RGB2YCC_BIT))
#define __set_saturatin_value(n, m)         (REG_LCD_CHROCFG1(n) = m)
#define __set_hue_value(n, x, y)            (REG_LCD_CHROCFG0(n) = (x<<LCD_CHROCFG0_HUE_SIN_BIT) | (y<<LCD_CHROCFG0_HUE_COS_BIT))

#define __set_brightness_value(n, m)        (REG_LCD_LUMMACFG(n) = (REG_LCD_LUMMACFG(n) & ~LCD_LUMMACFG_BRIGHTNESS_MASK)|(m<<LCD_LUMMACFG_BRIGHTNESS_BIT))
#define __set_contrast_value(n, m)          (REG_LCD_LUMMACFG(n) = (REG_LCD_LUMMACFG(n) & ~LCD_LUMMACFG_CONTRAST_MASK)|(m<<LCD_LUMMACFG_CONTRAST_BIT))
/*
#define __set_gamma_value(x, y,n)        (REG_LCD_GAMMA(n) = (x<<LCD_GAMMACFG_DATA1_BIT) | (y<<LCD_GAMMACFG_DATA0_BIT))
#define __set_vee_value(n, x, y)         (REG_LCD_VEE(n)   = (x<<LCD_VEECFG_DATA1_BIT) | (y<<LCD_VEECFG_DATA0_BIT))
*/

#define is_gamma_enabled(n)                 (REG_LCD_ENHCFG(n) & LCD_ENHCFG_GAMMA_EN)
#define is_rgb2ycc_enabled(n)               (REG_LCD_ENHCFG(n) & LCD_ENHCFG_RGB2YCC_EN)
#define is_contrast_enabled(n)              (REG_LCD_ENHCFG(n) & LCD_ENHCFG_CONTRAST_EN)
#define is_brightness_enabled(n)            (REG_LCD_ENHCFG(n) & LCD_ENHCFG_BRIGHTNESS_EN)
#define is_hue_enabled(n)                   (REG_LCD_ENHCFG(n) & LCD_ENHCFG_HUE_EN)
#define is_vee_enabled(n)                   (REG_LCD_ENHCFG(n) & LCD_ENHCFG_VEE_EN)
#define is_saturation_enabled(n)            (REG_LCD_ENHCFG(n) & LCD_ENHCFG_SATURATION_EN)
#define is_ycc2rgb_enabled(n)               (REG_LCD_ENHCFG(n) & LCD_ENHCFG_YCC2RGB_EN)
#define is_dither_enabled(n)                (REG_LCD_ENHCFG(n) & LCD_ENHCFG_DITHER_EN)
#define is_enh_enabled(n)                   (REG_LCD_ENHCFG(n) & LCD_ENHCFG_ENH_EN)

#endif /* is_MIPS_ASSEMBLER */


#define __jzlcd_enable_fg0(info) 	(info.osd_mode.osd_cfg |= LCD_OSDC_F0EN)
#define __jzlcd_enable_fg1(info) 	(info.osd_mode.osd_cfg |= LCD_OSDC_F1EN)
#define __jzlcd_disable_fg0(info) 	(info.osd_mode.osd_cfg &= ~LCD_OSDC_F0EN)
#define __jzlcd_disable_fg1(info) 	(info.osd_mode.osd_cfg &= ~LCD_OSDC_F1EN)

#endif /* is_JZ4775LCDC_His_ */

