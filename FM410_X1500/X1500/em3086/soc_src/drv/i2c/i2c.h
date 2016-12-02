/********************** BEGIN LICENSE BLOCK ************************************
 *
 * JZ4740  mobile_tv  Project  V1.0.0
 * INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Copyright (c) Ingenic Semiconductor Co. Ltd 2005. All rights reserved.
 * 
 * This file, and the files included with this file, is distributed and made 
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS 
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * 
 * http://www.ingenic.cn 
 *
 ********************** END LICENSE BLOCK **************************************
 *
 *  Author:  <dsqiu@ingenic.cn>  <jgao@ingenic.cn> 
 *
 *  Create:   2008-06-26, by dsqiu
 *            
 *  Maintain: 2008-06-26, by jgao
 *            
 *
 *******************************************************************************
 */


#ifndef __I2C_H__
#define __I2C_H__
#include <x1000.h>

#define i2c_1   1


#define DPHYSADDR(n)  ((n) & 0x1fffffff)
#define CPHYSADDR(n) (DPHYSADDR(n) | 0xa0000000)
#define I2C0_DMA_WRITE_CHANNEL 18
#define I2C0_DMA_READ_CHANNEL 19
#define I2C1_DMA_WRITE_CHANNEL 26
#define I2C1_DMA_READ_CHANNEL 27
#define I2C2_DMA_WRITE_CHANNEL 28
#define I2C2_DMA_READ_CHANNEL 29

#define I2C0_DMA_WRITE_TYPE 0x24
#define I2C0_DMA_READ_TYPE  0x25
#define I2C1_DMA_WRITE_TYPE 0x26
#define I2C1_DMA_READ_TYPE  0x27
#define I2C2_DMA_WRITE_TYPE 0x28
#define I2C2_DMA_READ_TYPE  0x29


#define I2C_READ_CMD (1<<8)
#define I2C_DC_STP 	(1<<9)

int i2c_read(int num, unsigned char device, unsigned char *buf,
	       unsigned char address, int count); 	/* Read register though I2C */
int i2c_write(int num, unsigned char device_addr, unsigned char *buf,
		unsigned char address, int count);		/* Write register though I2C */

 int i2c_read16(int num, unsigned char device, unsigned char *buf,
	       unsigned short address, int count); 	/* Read register though I2C */
 int i2c_write16(int num, unsigned char device, unsigned char *buf,
		unsigned short address, int count);		/* Write register though I2C */
 void i2c_open(int num, int iicclk);				/* Open and enable I2C */
 void i2c_close(int num);				/* Disable and close I2C */
 //void i2c_setclk(unsigned int i2cclk); 		/* Set I2C clock */

//void select_i2c(int num) ; /* select i2c control 0 or 1 */

#endif
