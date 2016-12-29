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
mdelay(5000);
 * 
 * http://www.ingenic.cn 
 *
 ********************** END LICENSE BLOCK **************************************
 *
 *  Author:  <dsqiu@ingenic.cn>  <zzhang@ingenic.cn> 
 *
 *  Create:   2008-11-17, by zzhagn
 *            
 *  Maintain: 
 *            
 *
 *******************************************************************************
 */

#include <x1000.h>
#include <os_api.h>
#include "i2c.h"
#include <mipsregs.h>
#include <time-tcu.h>
#undef printf
#include <stdio.h>
#include <string.h>

//#define I2C_DMA
//#define I2C_READ	1
//#define I2C_WRITE	0

//#define I2C_M_RD	1
//#define I2C_M_WR	2

#define TIMEOUT         100000
#define SCL             (32*0+0)//PA0
#define SDA             (32*0+1)//PA1
#define PAINTC        *(volatile unsigned int *)0xb0010018
#define PAMSKC       *(volatile unsigned int *)0xb0010028
#define PAPAT1S      *(volatile unsigned int *)0xb0010034
#define PAPAT0C      *(volatile unsigned int *)0xb0010048
#define CLKGR          *(volatile unsigned int *)0xb0000020
#define TIMEOUT         100000
//#define i2c_cur_dev		0

//static int i2c_cur_dev = -1;
//static int i2c_clock[3];
static int rd_command = 0;
#define __reg_printf()							\
do {								        \
	printf("  REG_I2C_CTRL(%d)=0x%x\n",i2c_cur_dev,REG_I2C_CTRL(i2c_cur_dev));		\
	printf("  REG_I2C_STA(%d)=0x%x\n",i2c_cur_dev,REG_I2C_STA(i2c_cur_dev));		\
	printf("  REG_I2C_TXTL(%d)=0x%x\n",i2c_cur_dev,REG_I2C_TXTL(i2c_cur_dev));		\
	printf("  REG_I2C_RXTL(%d)=0x%x\n",i2c_cur_dev,REG_I2C_RXTL(i2c_cur_dev));		\
	printf("  REG_I2C_INTM(%d)=0x%x\n",i2c_cur_dev,REG_I2C_INTM(i2c_cur_dev));	\
	printf("  REG_I2C_INTST(%d)=0x%x\n",i2c_cur_dev,REG_I2C_INTST(i2c_cur_dev));	\
	printf("  REG_I2C_TXABRT(%d)=0x%x\n",i2c_cur_dev,REG_I2C_TXABRT(i2c_cur_dev));	\
	printf("  REG_I2C_ENB(%d)=0x%x\n",i2c_cur_dev,REG_I2C_ENB(i2c_cur_dev));	\
	printf("  REG_I2C_TAR(%d)=0x%x\n",i2c_cur_dev,REG_I2C_TAR(i2c_cur_dev));	\
	printf("------------------%s:%d\n",__FUNCTION__,__LINE__);				\
} while(0)


static void i2c_set_target(int num, unsigned char address)
{
	while (!__i2c_txfifo_is_empty(num) || __i2c_master_active(num));
	REG_I2C_TAR(num) = address;  /* slave id needed write only once */
}
static int i2c_set_clk(int num, int i2c_clk)
{
	int cnt_high = 0;	/* HIGH period count of the SCL clock */
	int cnt_low = 0;	/* LOW period count of the SCL clock */
	int cnt_period = 0;	/* period count of the SCL clock */
	int setup_time = 0;
	int hold_time = 0;
	int dev_clk_hz = cpm_get_clock(CGU_PCLK);

	if (i2c_clk <= 0 || i2c_clk > 400000)
		goto set_clk_err;

	/* 1 I2C cycle equals to cnt_period PCLK(i2c_clk) */
	cnt_period = dev_clk_hz / i2c_clk;

	if (i2c_clk <= 100000) {
		/* i2c standard mode, the min LOW and HIGH period are 4700 ns and 4000 ns */
		cnt_high = (cnt_period * 4000) / (4700 + 4000);
	} else {
		/* i2c fast mode, the min LOW and HIGH period are 1300 ns and 600 ns */
		cnt_high = (cnt_period * 600) / (1300 + 600);
	}

	cnt_low = cnt_period - cnt_high;
	if (i2c_clk <= 100000) {
		REG_I2C_CTRL(num) = 0x43;	  /* standard speed mode*/
		REG_I2C_SHCNT(num) = I2CSHCNT_ADJUST(cnt_high);
		REG_I2C_SLCNT(num) = I2CSLCNT_ADJUST(cnt_low);
	} else {
		REG_I2C_CTRL(num) = 0x45;	   /* high speed mode*/
		REG_I2C_FHCNT(num) = I2CFLCNT_ADJUST(cnt_high);
		REG_I2C_FLCNT(num) = I2CFLCNT_ADJUST(cnt_low);
	}

	/*
	 * a i2c device must internally provide a hold time at least 300ns
	 * tHD:DAT
	 *	Standard Mode: min=300ns, max=3450ns
	 *	Fast Mode: min=0ns, max=900ns
	 * tSU:DAT
	 *	Standard Mode: min=250ns, max=infinite
	 *	Fast Mode: min=100(250ns is recommanded), max=infinite
	 *
	 * 1i2c_clk = 10^6 / dev_clk_hz
	 * on FPGA, dev_clk_hz = 12000, so 1i2c_clk = 1000/12 = 83ns
	 * on Pisces(1008M), dev_clk_hz=126000, so 1i2c_clk = 1000 / 126 = 8ns
	 *
	 * The actual hold time is (SDAHD + 1) * (i2c_clk period).
	 *
	 * The length of setup time is calculated using (SDASU - 1) * (ic_clk_period)
	 *
	 */

	if (i2c_clk <= 100000) { /* standard mode */
		setup_time = 300;
		hold_time = 400;
	} else {
		setup_time = 300;
		hold_time = 100;
	}

	hold_time = (hold_time / (1000000 / (dev_clk_hz / 1000)) - 1);
	setup_time = (setup_time / (1000000 / (dev_clk_hz / 1000)) + 1);

	if (setup_time > 255)
		setup_time = 255;
	if (setup_time <= 0)
		setup_time = 1;

//	if (hold_time > 255)
//		hold_time = 255;
	if(hold_time <= 0)
		hold_time = 1;

	__i2c_set_hold_time(num, hold_time);
	__i2c_set_setup_time(num, setup_time);
	return 0;

set_clk_err:
	printf("i2c set sclk faild,i2c_clk=%d Hz,dev_clk=%d Hz.\n", i2c_clk, dev_clk_hz);
	return -1;
}

static int i2c_disable(int num)
{
	int timeout = TIMEOUT;

	__i2c_disable(num);
	while(__i2c_is_enable(num) && (timeout > 0)) {
		udelay(1);
		timeout--;
	}
	if(timeout)
		return 0;
	else
		return 1;
}

static void i2c_init_as_master(int num, int iicclk)
{
	if(i2c_disable(num))
		printf("i2c not disable\n");
	
	__i2c_7bit_addr(num);

	i2c_set_clk(num, iicclk);
	REG_I2C_INTM(num) = 0x0; /*mask all interrupt????????*/
	REG_I2C_TXTL(num) = 0x3f;
	REG_I2C_RXTL(num) = 0;
	REG_I2C_ENB(num) = 1;   /*enable i2c*/
}



int xfer_write_subaddr(int num, unsigned char *subaddr,int sublength, unsigned char device, unsigned char *buf, int length)
{
	
	int i = 0;
	int ret = 0;
	int timeout;
	//int i2c_id = i2c_cur_dev;
	
	i2c_set_target(num, device);
	
	for (i = 0; i < sublength; i++) {
		timeout = TIMEOUT;
		while(!__i2c_txfifo_not_full(num) && --timeout);
			__i2c_write(I2C_DC_WRITE | subaddr[i], num);
	}


	for (i = 0; i < length; i++) {
		timeout = TIMEOUT;
		while(!__i2c_txfifo_not_full(num) && --timeout);
		if (i == (length - 1))
			__i2c_write(I2C_DC_WRITE | buf[i] | I2C_DC_STOP, num);
		else
			__i2c_write(I2C_DC_WRITE | buf[i], num);
	}
	
	if(rd_command == 1)
	{
		return 1;
	}
	timeout = 100*1000*20;
	while((!(REG_I2C_STA(num) & I2C_STA_TFE)) && --timeout)
		udelay(10);
	
	if (!timeout){
		printf("xfer_write_subaddr i2c device 0x%2x failed: wait TF buff empty timeout.\n",device);
		ret = -1;
		goto out;
	}
	
	timeout = TIMEOUT;
	while (__i2c_master_active(num) && --timeout);
	if (!timeout){
		printf("xfer_write_subaddr i2c device 0x%2x failed: wait master inactive timeout.\n",device);
		ret = -2;
		goto out;
	}
	
	if ((REG_I2C_INTST(num) & I2C_INTST_TXABT) || REG_I2C_TXABRT(num)) {
		volatile int intr;
		printf("xfer_write_subaddr i2c device 0x%2x failed: device no ack or abort.\n",device);
		__i2c_clear_interrupts(intr,num);
		ret = -3;
		goto out;
	}
	
out:
	
	return ret;
	
}

int xfer_read_subaddr(int num, unsigned char *subaddr,int sublength, unsigned char device, unsigned char *buf, int length)
{
	int timeout;
	int i;
	int ret = 0;
	unsigned long start_time;

	if (length  > 64)	/* FIFO depth is 64 */
		return -1;

	i2c_set_target(num, device);

	for (i = 0; i < sublength; i++) {
		timeout = TIMEOUT;
		while((!__i2c_txfifo_not_full(num)) && --timeout);
		__i2c_write(I2C_DC_WRITE | subaddr[i], num);
	}

	
	for (i = 0; i < length; i++) {
		timeout = TIMEOUT;
		while((!__i2c_txfifo_not_full(num)) && --timeout);
		if(i == (length - 1))
			__i2c_write(I2C_DC_STOP | I2C_DC_READ, num);
		else
			__i2c_write(I2C_DC_READ, num);
	}

	udelay(100);
	timeout = TIMEOUT;
	while((!(REG_I2C_STA(num) & I2C_STA_TFE)) && --timeout){
		udelay(100);
	}
	if (!timeout){
		printf("%s:%d: wait tx fifi empty timedout! dev_addr = 0x%02x\n",
				__func__, __LINE__, device);
		ret = -2;
		goto out;
	}

	/* TXABRT 0 bit: master is not recive a ack */
	if ((REG_I2C_INTST(num) & I2C_INTST_TXABT) ||
			REG_I2C_TXABRT(num)) {
		int tmp;
		printf("%s:%d: TX abart, dev 0x%02x no ack!\n",
				__func__, __LINE__, device);
		printf("INTST = 0x%x, TXABRT = 0x%x\n", REG_I2C_INTST(num), REG_I2C_TXABRT(num));
		__i2c_clear_interrupts(tmp,num);
		ret = -3;
		while (1) ;
		goto out;
	}

	memset((void *)buf, 0, length * sizeof(unsigned char));
	for (i = 0; i < length; i++) {
		start_time = TIMEOUT;;
		while (!(REG_I2C_STA(num) & I2C_STA_RFNE)) {
			start_time--;
			if ((REG_I2C_INTST(num) & I2C_INTST_TXABT) ||
					REG_I2C_TXABRT(num)) {
				int tmp;
				__i2c_clear_interrupts(tmp,num);
				printf("%s:%d: i2c transfer aborted, dev_addr = 0x%02x, intr = 0x%08x.\n",
						__func__, __LINE__, device, tmp);
				ret = -4;
				goto out;

			}
			while (!start_time) {
				printf("WARNING: i2c%d: data not received after 1 second!\n", num);
				ret = -5;
				goto out;
			}
			udelay(10);
		}
		buf[i] = __i2c_read(num);
	}

	timeout = TIMEOUT;
	while ((REG_I2C_STA(num) & I2C_STA_MSTACT) && --timeout)
		udelay(10);
	if (!timeout){
		printf("%s:%d: waite master inactive timeout, dev_addr = 0x%02x\n",
				__func__, __LINE__, device);
		ret = -6;
		goto out;
	}
out:
	return ret;

}

//#define SCL             (32*0+0)
//#define SDA             (32*0+1)
void i2c_open(int num, int iicclk)
{
    if(num > 2)
        num = 1;
    
    PAINTC |= (0x3 << 0);
    PAMSKC |= (0x3 << 0);
    PAPAT1S |= (0x3 << 0);
    PAPAT0C |= (0x3 << 0);
    CLKGR &= ~(1 << 8);

    switch(num)
	{
	case 0:
		cpm_start_clock(CGM_I2C0);
		break;
	case 1:
		cpm_start_clock(CGM_I2C1);
		break;
	case 2:
		cpm_start_clock(CGM_I2C2);
		break;
	default:
		printf("i2c not support!\n");
		break;
	}
	i2c_init_as_master(num, iicclk);
    
}

void i2c_close(int num)
{
	switch(num)
	{
	case 0:
		cpm_stop_clock(CGM_I2C0);		
		break;
	case 1:
		cpm_stop_clock(CGM_I2C1);		
		break;
	case 2:
		cpm_stop_clock(CGM_I2C2);		
		break;
	default:
		printf("i2c close not support!\n");
		break;
	}

//	os_SemaphorePost(rw_sem);
}

/*void i2c_setclk(unsigned int i2cclk)
{
	i2c_clock[i2c_cur_dev] = i2cclk;
}*/

int i2c_read(int num, unsigned char device, unsigned char *buf,
	       unsigned char address, int count)
{
	printf("read no16\n");
	return xfer_read_subaddr(num, &address, 1, device, buf, count);
}

int i2c_write(int num, unsigned char device_addr, unsigned char *buf,
		unsigned char address, int count)
{
	return xfer_write_subaddr(num, &address, 1, device_addr, buf, count);
}

int i2c_read_16(int num, unsigned char device, unsigned char *buf,
	       unsigned short address, int count)
{
	unsigned char addr_buf[2];

	addr_buf[0] = (unsigned char)(address >> 8);
	addr_buf[1] = (unsigned char)(address & 0xff);
	return xfer_read_subaddr(num, addr_buf, 2, device, buf, count);
}
int i2c_write_16(int num, unsigned char device, unsigned char *buf,
		unsigned short address, int count)
{
	unsigned char addr_buf[2];

	addr_buf[0] = (unsigned char)(address >> 8);
	addr_buf[1] = (unsigned char)(address & 0xff);
	return xfer_write_subaddr(num, addr_buf, 2, device, buf, count);
}


/*void select_i2c(int num)
{
	unsigned char err, i;
	if(i == 3){
//		rw_sem = os_SemaphoreCreate(1);
	}
//	os_SemaphorePend(rw_sem, 0, &err);
	i2c_cur_dev = num ;
}*/

