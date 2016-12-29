/********************** BEGIN LICENSE BLOCK ************************************
 *
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
 *  Author:  <xyzhang@ingenic.cn>
 *
 *  Create:   2008-10-09, by xyzhang
 *
 *            
 *
 *******************************************************************************
 */

#include <nor_flash.h>
int NOR_Init(void)
{
	jz_nor_init();
	return 0;
}

int NOR_Read(unsigned int sector, unsigned int count, unsigned char *pBuffer)
{
	jz_nor_read(4*1024*sector, 256*count,(unsigned int) pBuffer);
	return 0;
}

int NOR_Write(unsigned int sector, unsigned int count, unsigned char *pBuffer)
{
	jz_nor_write(4*1024*sector, 256*count,(unsigned int) pBuffer);
	return 0;
}

