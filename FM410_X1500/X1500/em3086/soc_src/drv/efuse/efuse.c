/********************** BEGIN LICENSE BLOCK ************************************
 *
 * x1000  em3086  Project  V1.0.0
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
 *  Author: xujian
 *
 *  Create: 2016/8/26
 *
 *  Maintain:
 *
 *
 *******************************************************************************
 */
#include <x1000.h>
#include "os_api.h"

//------------------------------------------------------------------------------
//	Global defines
//------------------------------------------------------------------------------
#undef printf
#include <stdio.h>
#define CUSTID_PRT (1 << 15)
#define CHIP_PRT (1 << 14)

#define CHIP_ID_ADDR    (0x200)
#define RN_ADDR         (0x210)
#define CUT_ID_ADDR     (0x220)
#define PTR_ADDR        (0x23e)
unsigned int chip_sn_buf[4];

int efuse_init(void)
{
	unsigned long rate;

	u32 val, ns;
	int i, rd_strobe, wr_strobe;
	u32 rd_adj, wr_adj;

    cpm_start_clock(CGM_EFUSE);

	rate = cpm_get_clock(CGU_H2CLK);
	ns = 1000000000 / rate;

    // 计算rd_adj = wr_adj
	for(i = 0; i < 0x4; i++)
	{
		if((( i + 1) * ns ) > 2)
		{
			break;
		}
	}
	if(i == 0x4) 
    {
		printf("get efuse cfg rd_adj fail!\n");
		return -1;
	}
	rd_adj = wr_adj = i;

    // 计算rd_strobe
	for(i = 0; i < 0x8; i++)
	{
		if(((rd_adj + i + 3) * ns ) > 15)
		{
			break;
		}
	}
	if(i == 0x8) 
    {
		printf( "get efuse cfg rd_strobe fail!\n");
		return -1;
	}
	rd_strobe = i;

    //计算wr_strobe 目前不支持写操作
	/*for(i = 0; i < 0x1f; i += 100) 
    {
		val = (wr_adj + i + 916) * ns;
		if( val > 4 * 1000 && val < 6 *1000)
			break;
	}
	if(i >= 0x1f) {
		printf("get efuse cfg wd_strobe fail!\n");
		return -1;
	}
	wr_strobe = i;*/
	wr_strobe = 0x1f;

	//printf("rd_adj = %d | rd_strobe = %d | wr_adj = %d | wr_strobe = %d\n", rd_adj, rd_strobe, wr_adj, wr_strobe);
	/*set configer register*/
	val = rd_adj << 20 | rd_strobe << 16 | wr_adj << 12 | wr_strobe;
	REG_EFUSE_CFG = val;

    return 0;

}

int efuse_read_chipID(unsigned int *buf)
{
	int bit_num, word_num;

	unsigned int val, addr = 0;
    unsigned int data_length=16;
	unsigned int offset;
	bit_num = data_length * 8;

    efuse_init();
    
	addr = CHIP_ID_ADDR;
	//spin_lock_irqsave(&efuse->lock, flags);
	
	/* set read Programming address and data length */
	data_length = data_length - 1;
	val =  addr << 21 | data_length << 16;
    REG_EFUSE_CTRL = val;
	/* enable read */
    val = REG_EFUSE_CTRL;
	val |= 1;
    REG_EFUSE_CTRL = val;    
	/* wait read done status */
	while(!(REG_EFUSE_STATE & 1));

	word_num = data_length / 4;
	word_num += data_length % 4 ? 1 : 0;

	if(word_num > 8) {
		printf("strongly recommend operate each segment separately\n");
	}
    else 
    {
		for(offset = 0; offset < word_num; offset ++) 
        {
            val = REG_EFUSE_DATA(offset);
			*(buf + offset) = val;
		}
	}
	/* clear read done staus */
    REG_EFUSE_STATE = 0;
	//spin_unlock_irqrestore(&efuse->lock, flags);
	return word_num;

}






