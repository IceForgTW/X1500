/********************** BEGIN LICENSE BLOCK **********************************
 *
 * JZ4755  mobile_tv  Project  V1.0.0
 * INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Copyright Ingenic Semiconductor Co. Ltd 2008. All rights reserved.
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
 *  Author:  <whxu@ingenic.cn>
 *
 *  Create:   2009-3-4, by whxu
 *
 *****************************************************************************
 */

#include <bsp.h>
#include <x1000.h>

//----------------------------------------------------------------------------

unsigned char codec_reg_read(unsigned char addr)
{
	volatile int reg;
	AIC_RW_CODEC_START();
	OUTREG16(ICDC_RGADW, addr << ICDC_RGADW_RGADDR_LSB);
	
	reg = INREG8(ICDC_RGDATA);
	reg = INREG8(ICDC_RGDATA);
	reg = INREG8(ICDC_RGDATA);
	reg = INREG8(ICDC_RGDATA);
	reg = INREG8(ICDC_RGDATA);
	reg = INREG8(ICDC_RGDATA);
	return (INREG8(ICDC_RGDATA));
}

//----------------------------------------------------------------------------

void codec_reg_write(unsigned char addr, unsigned char data)
{
	unsigned int	temp;
	volatile int reg;
	
	temp = (addr << ICDC_RGADW_RGADDR_LSB) | (data << ICDC_RGADW_RGDIN_LSB);

	AIC_RW_CODEC_START();
	OUTREG32(ICDC_RGADW, temp);
	OUTREG32(ICDC_RGADW, temp | ICDC_RGADW_RGWR);
	
	reg = INREG32(ICDC_RGADW);
	reg = INREG32(ICDC_RGADW);
	reg = INREG32(ICDC_RGADW);
	reg = INREG32(ICDC_RGADW);
	reg = INREG32(ICDC_RGADW);
	reg = INREG32(ICDC_RGADW);
	AIC_RW_CODEC_STOP();
}

//----------------------------------------------------------------------------

void codec_reg_set(unsigned char addr, unsigned char data)
{
	volatile int reg;
	reg = codec_reg_read(addr);
	
	codec_reg_write(addr, reg | data);
}

//----------------------------------------------------------------------------

void codec_reg_clear(unsigned char addr, unsigned char data)
{
	volatile int reg;
	reg = codec_reg_read(addr);
	
	codec_reg_write(addr, reg & (~data));
}

//----------------------------------------------------------------------------
