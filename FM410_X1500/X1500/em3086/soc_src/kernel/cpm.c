/*
 * linux/arch/mips/jz4775/cpm.c
 *
 * jz4775 on-chip modules.
 *
 * Copyright (C) 2010 Ingenic Semiconductor Co., Ltd.
 *
 * Author: <whxu@ingenic.cn>
 */

#include <x1000.h>
#include <minios.h>
#ifndef JZ_EXTAL
#define JZ_EXTAL        (24 * 1000000)  /* 24MHz */
#endif

/*
 * Get the xPLL clock
 */
unsigned int cpm_get_xpllout(src_clk sclk_name)
{
	unsigned int cpxpcr = 0, pllout =  __cpm_get_extalclk();
	unsigned int m, n, no;

	switch (sclk_name) {
	case SCLK_APLL:
		cpxpcr = INREG32(CPM_CPAPCR);
		if ((cpxpcr & CPAPCR_EN) && (!(cpxpcr & CPAPCR_BP))) {
			m = ((cpxpcr & CPAPCR_M_MASK) >> CPAPCR_M_LSB) + 1;
			n = ((cpxpcr & CPAPCR_N_MASK) >> CPAPCR_N_LSB) + 1;
			no = 1 << ((cpxpcr & CPAPCR_OD_MASK) >> CPAPCR_OD_LSB);
			pllout = ((JZ_EXTAL) * m / (n * no));
		}
		break;
	case SCLK_MPLL:
		cpxpcr = INREG32(CPM_CPMPCR);
		if ((cpxpcr & CPM_CPMPCR_EN) && (!(cpxpcr & CPMPCR_BP))) {
			m = ((cpxpcr & CPMPCR_M_MASK) >> CPMPCR_M_LSB) + 1;
			n = ((cpxpcr & CPMPCR_N_MASK) >> CPMPCR_N_LSB) + 1;
			no = 1 << ((cpxpcr & CPMPCR_OD_MASK) >> CPMPCR_OD_LSB);
			pllout = ((JZ_EXTAL) * m / (n * no));
		}
		break;
	default:
		printf("WARNING: can NOT get the %dpll's clock\n", sclk_name);
		break;
	}

	return pllout;
}

/*
 * Start the module clock
 */
void cpm_start_clock(clock_gate_module module_name)
{
	if (module_name == CGM_ALL_MODULE) {
		OUTREG32(CPM_CLKGR0, 0x0);
		return;
	}

	if (module_name & 0x1f) {
		CLRREG32(CPM_CLKGR0, 1 << module_name);
		switch(module_name) {
		case 3:
		case 11:
		case 12:
		     //SETREG32(CPM_MSC0CDR, MSCCDR_MPCS_APLL);
		     SETREG32(CPM_MSC0CDR, MSCCDR_MPCS_MPLL);
			break;
		default:
			;
		}
	} else {
		printf("WARNING: can NOT start the %d's clock\n", module_name);
	}
}

/*
 * Stop the module clock
 */
void cpm_stop_clock(clock_gate_module module_name)
{
	if (module_name == CGM_ALL_MODULE) {
		OUTREG32(CPM_CLKGR0, 0xffffffff);
		return;
	}

	if (module_name & 0x1f) {
		SETREG32(CPM_CLKGR0, 1 << module_name);
	} else {
		printf("WARNING: can NOT start the %d's clock\n", module_name);
	}
}

/*
 * Get the clock, assigned by the clock_name, and the return value unit is Hz
 */
unsigned int cpm_get_clock(cgu_clock clock_name)
{
	unsigned int exclk = __cpm_get_extalclk();
	unsigned int apllclk = cpm_get_xpllout(SCLK_APLL);
	unsigned int mpllclk = cpm_get_xpllout(SCLK_MPLL);
	unsigned int clock_ctrl = INREG32(CPM_CPCCR);
	unsigned int clock_hz = 0;
	unsigned int val, tmp, div,clksrc;

	switch (clock_name) {
		case CGU_CCLK:
			val = get_bf_value(clock_ctrl, CPCCR_CDIV_LSB, CPCCR_CDIV_MASK);
			div = val + 1;
			clock_hz = apllclk / div;
			printf("apllclk is %d\n",clock_hz,div);
			break;

		case CGU_L2CLK:
			val = get_bf_value(clock_ctrl, CPCCR_L2DIV_LSB, CPCCR_L2DIV_MASK);
			div = val + 1;
			clock_hz = apllclk / div;
			printf("l2clk is %d,div is %d\n",clock_hz,div);
			break;

		case CGU_HCLK:
			val = get_bf_value(clock_ctrl, CPCCR_H0DIV_LSB, CPCCR_H0DIV_MASK);
			div = val + 1;
			clock_hz = apllclk / div;
			printf("hclk is %d,div is %d\n",clock_hz,div);
			break;

		case CGU_H2CLK:
			val = get_bf_value(clock_ctrl, CPCCR_H2DIV_LSB, CPCCR_H2DIV_MASK);
			div = val + 1;
			clock_hz = mpllclk / div;
			printf("h2clk is %d,div is %d\n",clock_hz,div);
			break;

		case CGU_PCLK:
			val = get_bf_value(clock_ctrl, CPCCR_PDIV_LSB, CPCCR_PDIV_MASK);
			div = val + 1;
			clock_hz = apllclk / div;
			//printf("h2clk is %d,div is %d\n",clock_hz,div);
			break;

		case CGU_MCLK:
			tmp = INREG32(CPM_DDCDR);
			val = get_bf_value(tmp, DDCDR_DDRDIV_LSB, DDCDR_DDRDIV_MASK);
			div = val + 1;
			clock_hz = mpllclk / div;
			printf("mpllclk is %d,div is %d\n",mpllclk,div);
			break;

		case CGU_MSC0CLK:
			tmp = INREG32(CPM_MSC0CDR);
			clksrc = INREG32(CPM_MSC0CDR) & BIT31;
			val = get_bf_value(tmp, MSCCDR_MSCDIV_LSB, MSCCDR_MSCDIV_MASK);
			div = (val + 1) * 2;
			if (tmp & MSCCDR_MSC_STOP) {
				clock_hz = 0;
			} else {
				if (clksrc == MSCCDR_MPCS_APLL)
					clock_hz = apllclk / div;
				else if (clksrc == MSCCDR_MPCS_MPLL)
					clock_hz = mpllclk / div;
				else
					clock_hz = 0;
			}

			break;

		case CGU_MSC1CLK:
			tmp = INREG32(CPM_MSC1CDR);
			clksrc = INREG32(CPM_MSC0CDR) & BIT31;
			val = get_bf_value(tmp, MSCCDR_MSCDIV_LSB, MSCCDR_MSCDIV_MASK);
			div =  (val + 1) * 2;
			if (tmp & MSCCDR_MSC_STOP) {
				clock_hz = 0;
			} else {
				if (clksrc == MSCCDR_MPCS_APLL)
					clock_hz = apllclk / div;
				else if (clksrc == MSCCDR_MPCS_MPLL)
					clock_hz = mpllclk / div;
				else
					clock_hz = 0;
			}

			break;
		case CGU_CIMCLK:
			tmp = INREG32(CPM_CIMCDR);
			val = get_bf_value(tmp, CIMCDR_CIMDIV_LSB, CIMCDR_CIMDIV_MASK);
			div = val + 1;
			if (tmp & CIMCDR_CIM_STOP) {
				clock_hz = 0;
			} else {
				if (tmp & CIMCDR_CIMPCS_MPLL)
					clock_hz = mpllclk / div;
				else
					clock_hz = apllclk / div;
			}

			break;			
		case CGU_SSICLK:
			tmp = INREG32(CPM_SSICDR);
			val = get_bf_value(tmp, SSICDR_SSIDIV_LSB, SSICDR_SSIDIV_MASK);
			div = val + 1;
			if (tmp & SSICDR_SSI_STOP) {
				clock_hz = 0;
			} else {
				if (tmp & SSICDR_SCS) {
					if (tmp & SSICDR_SPCS)
						clock_hz = mpllclk / div;
					else
						clock_hz = apllclk / div;
				} else {
					clock_hz = exclk;
				}
			}

			break;

		case CGU_OTGCLK:
			tmp = INREG32(CPM_USBCDR);
			val = get_bf_value(tmp, USBCDR_OTGDIV_LSB, USBCDR_OTGDIV_MASK);
			div = val + 1;
			if (tmp & USBCDR_USB_STOP) {
				clock_hz = 0;
			} else {
				if (tmp & USBCDR_UCS) {
					if (tmp & USBCDR_UPCS)
						clock_hz = mpllclk / div;
					else
						clock_hz = apllclk / div;
				} else {
					clock_hz = exclk;
				}
			}

			break;

		case CGU_LPCLK:
			tmp = INREG32(CPM_LPCDR);
			clksrc = tmp & BIT31;
			val = get_bf_value(tmp, LPCDR_PIXDIV_LSB, LPCDR_PIXDIV_MASK);
			div = val + 1;
			if (tmp & LPCDR_LCD_STOP) {
				clock_hz = 0;
			} else {
				if (clksrc == LPCDR_LPCS_APLL)
					clock_hz = apllclk / div;
				else if (clksrc == LPCDR_LPCS_MPLL)
					clock_hz = mpllclk / div;
				else
					clock_hz = 0;
			}

			break;
		case CGU_I2SCLK:
			break;

		case CGU_PCMCLK:
			break;
#if 0
		case CGU_UHCCLK:
			tmp = INREG32(CPM_UHCCDR);
			clksrc = tmp & UHCCDR_UHCS_MASK;
			val = get_bf_value(tmp, UHCCDR_UHCDIV_LSB, UHCCDR_UHCDIV_MASK);
			div = val + 1;
			if (tmp & UHCCDR_UHC_STOP) {
				clock_hz = 0;
			} else {
				if (clksrc == UHCCDR_UHCS_APLL)
					clock_hz = apllclk / div;
				else if (clksrc == UHCCDR_UHCS_MPLL)
					clock_hz = mpllclk / div;
				else if (clksrc == UHCCDR_UHCS_OTG)
					clock_hz = cpm_get_clock(CGU_OTGCLK);
			}

			break;
#endif
		case CGU_UARTCLK:
		case CGU_SADCCLK:
		case CGU_TCUCLK:
			clock_hz = exclk;

			break;
		default:
			printf("WARNING: can NOT get clock %d!\n", clock_name);
			clock_hz = exclk;
			break;
	}

	return clock_hz;
}

/*
 * Check div value whether valid, if invalid, return the max valid value
 */
static unsigned int __check_div(unsigned int div, unsigned int lsb, unsigned int mask)
{
	if ((div << lsb) > mask) {
		printf("WARNING: Invalid div %d larger than %d\n", div, mask >> lsb);
		return mask >> lsb;
	} else
		return div;
}

/*
 * Set the clock, assigned by the clock_name, and the return value unit is Hz,
 * which means the actual clock
 */
#define ceil(v,div) ({ unsigned int val = 0; if(v % div ) val = v /div + 1; else val = v /div; val;})
#define nearbyint(v,div) ({unsigned int val = 0; if((v % div) * 2 >= div)  val = v / div + 1;else val = v / div; val;})
unsigned int cpm_set_clock(cgu_clock clock_name, unsigned int clock_hz)
{
	unsigned int actual_clock = 0;
	unsigned int exclk = __cpm_get_extalclk();
//	unsigned int apllclk = cpm_get_xpllout(SCLK_APLL);
	unsigned int mpllclk = cpm_get_xpllout(SCLK_MPLL);
	unsigned int div;

	if (!clock_hz)
		return actual_clock;

	switch (clock_name) {
		case CGU_MSC0CLK:
			div = ceil(mpllclk , clock_hz/2) - 1;
			div = __check_div(div, MSCCDR_MSCDIV_LSB, MSCCDR_MSCDIV_MASK);
			/* Select mpll clock as input */
			OUTREG32(CPM_MSC0CDR, MSCCDR_MPCS_MPLL | MSCCDR_CE_MSC | div);
			while (INREG32(CPM_MSC0CDR) & MSCCDR_MSC_BUSY) ;

			break;

		case CGU_MSC1CLK:
			div = ceil(mpllclk , clock_hz/2) - 1;
			div = __check_div(div, MSCCDR_MSCDIV_LSB, MSCCDR_MSCDIV_MASK);
			OUTREG32(CPM_MSC1CDR, MSCCDR_MPCS_MPLL | MSCCDR_CE_MSC | div);
			while (INREG32(CPM_MSC1CDR) & MSCCDR_MSC_BUSY) ;

			break;

		case CGU_LPCLK:
			div = nearbyint(mpllclk , clock_hz) - 1;
			div = __check_div(div, LPCDR_PIXDIV_LSB, LPCDR_PIXDIV_MASK);
			OUTREG32(CPM_LPCDR, div | LPCDR_LPCS_MPLL | LPCDR_CE_LCD);
			while (INREG32(CPM_LPCDR) & LPCDR_LCD_BUSY) ;
			break;

		case CGU_I2SCLK:
		     break;

		case CGU_PCMCLK:
			break;

		case CGU_OTGCLK:
			if (clock_hz == exclk) {
			     //	OUTREG32(CPM_USBCDR, USBCDR_CE_USB);
			     OUTREG32(CPM_USBCDR, 0);  //fix me please, cpm spec said , when use extclk ,do not set ce = 1
			} else {
				div = nearbyint(mpllclk , clock_hz) - 1;
				div = __check_div(div, USBCDR_OTGDIV_LSB, USBCDR_OTGDIV_MASK);
				OUTREG32(CPM_USBCDR, USBCDR_UPCS | USBCDR_UCS | USBCDR_CE_USB | div);
			}
			while (INREG32(CPM_USBCDR) & USBCDR_USB_BUSY) ;

			break;

		case CGU_RTCCLK:
			SETREG32(CPM_OPCR, OPCR_ERCS);
			break;

		case CGU_CIMCLK:
			/* Select apll clock as input */
			//CMSREG32(CPM_CIMCDR, CIMCDR_CIMPCS_APLL | CIMCDR_CE_CIM, 0xffffff00);
			CMSREG32(CPM_CIMCDR, CIMCDR_CIMPCS_MPLL | CIMCDR_CE_CIM, 0xffffff00);
			while (INREG32(CPM_CIMCDR) & CIMCDR_CIM_BUSY) ;

			div = ceil(mpllclk , clock_hz) - 1;
			div = __check_div(div, CIMCDR_CIMDIV_LSB, CIMCDR_CIMDIV_MASK);
			OUTREG32(CPM_CIMCDR, CIMCDR_CIMPCS_MPLL | CIMCDR_CIM_BUSY | div);
			while (INREG32(CPM_CIMCDR) & CIMCDR_CIM_BUSY) ;

			printf("==>%s L%d: CIMCDR=0x%08x\n", __func__, __LINE__, REG_CPM_CIMCDR);
			break;
#if 0

		case CGU_UHCCLK:
			div = nearbyint(mpllclk , clock_hz) - 1;
			div = __check_div(div, UHCCDR_UHCDIV_LSB, UHCCDR_UHCDIV_MASK);
			OUTREG32(CPM_UHCCDR, div | UHCCDR_CE_UHC | UHCCDR_UHCS_MPLL);
			while (INREG32(CPM_UHCCDR) & UHCCDR_UHC_BUSY) ;

			break;
#endif
		default:
			printf("WARNING: can NOT set clock %d!\n", clock_name);
			break;
	}

	/* Get the actual clock */
	actual_clock = cpm_get_clock(clock_name);

	return actual_clock;
}

/*
 * Control UHC phy, if en is NON-ZERO, enable the UHC phy, otherwise disable
 * Because UHC clock from OTG PHY, so realize enable OTG PHY
 */
void cpm_uhc_phy(unsigned int en)
{
	if (en)
		SETREG32(CPM_USBPCR1, BIT17);
	else
		CLRREG32(CPM_USBPCR1, BIT17);
}
