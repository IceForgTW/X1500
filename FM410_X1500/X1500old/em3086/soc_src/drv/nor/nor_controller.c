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
 *  Maintain: 2010-5-20,  by chyuan           
 *
 *******************************************************************************
 */
#include <stdio.h>

#define writel(b, a) (*((volatile unsigned long *)(a)) = (unsigned long)b)
#define readl(x)   ((unsigned long)(*(volatile unsigned long *)(x)))
static int mode = 0;
static unsigned int g_sfc_type=0;
static int g_flash_size=0;

static void sfc_send_cmd(unsigned char *cmd,unsigned int len,unsigned int addr ,unsigned addr_len,unsigned dummy_byte,unsigned int daten,unsigned char dir)
{
	unsigned int tmp;
	//1. sfc_set_transfer	
//	sfc_set_transfer(&sfc,dir);
	if(dir == 0) {
		tmp = readl(0xb3440000);
		tmp &= ~(1 << 13);
		writel(tmp, 0xb3440000);
	} else {
		tmp = readl(0xb3440000);
		tmp |= (1 << 13);
		writel(tmp, 0xb3440000);
	}

//	sfc_set_length(hw->len);
	writel(len, 0xb344002c);
//	sfc_set_addr_length(0, hw->addr_len);
	tmp = readl(0xb3440014);
	tmp &= ~(0x7 << 26);
	tmp |= (addr_len << 26);
	writel(tmp, 0xb3440014);
//	sfc_cmd_en(0, 0x1);
	tmp = readl(0xb3440014);
	tmp |= (1 << 24);
	writel(tmp, 0xb3440014);
//	sfc_data_en(0, hw->daten);
	if(daten == 1) {
		tmp = readl(0xb3440014);
		tmp |= (1 << 16);
		writel(tmp, 0xb3440014);
	} else {
		tmp = readl(0xb3440014);
		tmp &= ~(1 << 16);
		writel(tmp, 0xb3440014);
	}
//	sfc_write_cmd(0, hw->cmd);
	tmp = readl(0xb3440014);
	tmp &= ~(0xffff << 0);
	tmp |= *cmd;
	writel(tmp, 0xb3440014);

//	sfc_dev_addr(0, hw->addr);
	writel(addr, 0xb3440030);
//	sfc_dev_addr_plus(0, hw->addr_plus);
	writel(0, 0xb3440048);
//	sfc_dev_addr_dummy_bytes(0,hw->dummy_byte);
	tmp = readl(0xb3440014);
	tmp &= ~(0x3f << 17);
	tmp |= (dummy_byte << 17);
	writel(tmp, 0xb3440014);
//	sfc_set_mode(0,hw->sfc_mode);
	tmp = readl(0xb3440014);
	tmp &= ~(0x7 << 29);
	if((daten == 1)&&(addr_len != 0))
		tmp |= (mode << 29);
	else
		tmp |= (0 << 29);
	writel(tmp, 0xb3440014);

	//2. SFC_TRIG
	writel(1 << 2, 0xb3440064);
	writel((1 << 0), 0xb3440064);
	
	/*this must judge the end status*/
	if((daten == 0)){
		tmp = readl(0xb3440068);
		while (!(tmp & (1 << 4))){
			tmp = readl(0xb3440068);
		}

		if ((readl(0xb3440068)) & (1 << 4))
			writel((1 << 4), 0xb344006c);
	}
}

static int sfc_read_data(unsigned int *data, unsigned int length)
{
	unsigned int tmp_len = 0;
	unsigned int fifo_num = 0;
	unsigned int i;
	unsigned int reg_tmp = 0;
	unsigned int  len = (length + 3) / 4 ;
//	unsigned int time_out = 1000;

	while(1){
		reg_tmp = readl(0xb3440068);
		if (reg_tmp & (1 << 2)) {
			writel((1 << 2), 0xb344006c);
			if ((len - tmp_len) > 31)
				fifo_num = 31;
			else {
				fifo_num = len - tmp_len;
			}
			for (i = 0; i < fifo_num; i++) {
				*data = readl(0xb3441000);
				data++;
				tmp_len++;
			}
		}
		if (tmp_len == len)
			break;
	}

	reg_tmp = readl(0xb3440068);
	while (!(reg_tmp & (1 << 4))){
		reg_tmp = readl(0xb3440068);
	}

	if ((readl(0xb3440068)) & (1 << 4))
		writel((1 << 4), 0xb344006c);
	
	return 0;
}

static int sfc_write_data(unsigned int *data, unsigned int length)
{
	unsigned int tmp_len = 0;
	unsigned int fifo_num = 0;
	unsigned int i;
	unsigned int reg_tmp = 0;
	unsigned int  len = (length + 3) / 4 ;
//	unsigned int time_out = 10000;

	while(1){
		reg_tmp = readl(0xb3440068);
		if (reg_tmp & (1 << 3)) {
			writel((1 << 3), 0xb344006c);
			if ((len - tmp_len) > 31)
				fifo_num = 31;
			else {
				fifo_num = len - tmp_len;
			}

			for (i = 0; i < fifo_num; i++) {
				writel(*data, 0xb3441000);
				data++;
				tmp_len++;
			}
		}
		if (tmp_len == len)
			break;
	}

	reg_tmp = readl(0xb3440068);
	while (!(reg_tmp & (1 << 4))){
		reg_tmp = readl(0xb3440068);
	}

	if ((readl(0xb3440068)) & (1 << 4))
		writel((1 << 4), 0xb344006c);

	return 0;
}

static int jz_sfc_erase(unsigned int offset, unsigned int len)
{
	unsigned long erase_size;
	unsigned char cmd[7];
	unsigned int  buf = 0, i;

	if((len >= 0x10000)&&((offset % 0x10000) == 0)){
		erase_size = 0x10000;
	}
    else if((len >= 0x8000)&&((offset % 0x8000) == 0)){
		erase_size = 0x8000;
	}
    else{
		erase_size = 0x1000;
	}

	if(len % erase_size != 0){
		len = len - (len % erase_size) + erase_size;
	}

	cmd[0] = 0x06;//CMD_WREN;

	switch(erase_size) {
	case 0x1000 :
		cmd[1] = 0x20;//CMD_ERASE_4K;
		break;
	case 0x8000 :
		cmd[1] = 0x52;//CMD_ERASE_32K;
		break;
	case 0x10000 :
		cmd[1] = 0xd8;//CMD_ERASE_64K;
		break;
	default:
		printf("unknown erase size !\n");
		return -1;
	}

	cmd[3/*flash->addr_size*/ + 2] = 0x05;//CMD_RDSR;

	while(len > 0) {
		for(i = 0; i < 3/*flash->addr_size*/; i++){
			cmd[i+2] = offset >> (3/*flash->addr_size*/ - i - 1) * 8;
		}

		//	printf("erase %x %x %x %x %x %x %x \n", cmd[0], cmd[1], cmd[2], cmd[3], cmd[4], cmd[5], offset);

		/* the paraterms is
		 * cmd , len, addr,addr_len
		 * dummy_byte, daten
		 * dir
		 *
		 * */
		sfc_send_cmd(&cmd[0],0,0,0,0,0,1);

		sfc_send_cmd(&cmd[1],0,offset,3/*flash->addr_size*/,0,0,1);

		sfc_send_cmd(&cmd[3/*flash->addr_size*/ + 2], 1,0,0,0,1,0);
		sfc_read_data(&buf, 1);
		while(buf & (1 << 0)/*CMD_SR_WIP*/) {
			sfc_send_cmd(&cmd[3/*flash->addr_size*/ + 2], 1,0,0,0,1,0);
			sfc_read_data(&buf, 1);
		}

		offset += erase_size;
		len -= erase_size;
	}

	return 0;

}

static int jz_sfc_write(unsigned int offset, unsigned int length, const void *buf)
{
	unsigned char cmd[7];
	unsigned tmp = 0;
	int i;
	unsigned int *send_buf = (unsigned int *)buf;
	unsigned int pagelen = 0,len = 0,retlen = 0;

	if (offset + length > g_flash_size/*flash->size*/) {
		printf("Data write overflow this chip\n");
		return -1;
	}

	cmd[0] = 0x06;//CMD_WREN;

	cmd[1]  = 0x32;//CMD_QPP;
	mode = 0x5;//TRAN_SPI_QUAD;

	cmd[3/*flash->addr_size*/ + 2] = 0x05;//CMD_RDSR;

	while (length) {
		if (length >= 256/*flash->page_size*/)
			pagelen = 0;
		else
			pagelen = length % 256/*flash->page_size*/;

		/* the paraterms is
		 * cmd , len, addr,addr_len
		 * dummy_byte, daten
		 * dir
		 *
		 * */
		sfc_send_cmd(&cmd[0],0,0,0,0,0,1);

		if (!pagelen || pagelen > 256/*flash->page_size*/)
			len = 256/*flash->page_size*/;
		else
			len = pagelen;

		if (offset % 256/*flash->page_size*/ + len > 256/*flash->page_size*/)
			len -= offset % 256/*flash->page_size*/ + len - 256/*flash->page_size*/;

		for(i = 0; i < 3/*flash->addr_size*/; i++){
			cmd[i+2] = offset >> (3/*flash->addr_size*/ - i - 1) * 8;
		}

		sfc_send_cmd(&cmd[1], len,offset,3/*flash->addr_size*/,0,1,1);

		sfc_write_data(send_buf ,len);

		retlen = len;

		/*polling*/
		sfc_send_cmd(&cmd[3/*flash->addr_size*/ + 2],1,0,0,0,1,0);
		sfc_read_data(&tmp, 1);
		while(tmp & (1 << 0)/*CMD_SR_WIP*/) {
			sfc_send_cmd(&cmd[3/*flash->addr_size*/ + 2],1,0,0,0,1,0);
			sfc_read_data(&tmp, 1);
		}

		if (!retlen) {
			printf("spi nor write failed\n");
			return -1;
		}

		offset += retlen;
		send_buf += ((retlen + 3) / 4);
		length -= retlen;
	}

	return 0;
}

static int jz_sfc_read(unsigned int offset, unsigned int len, void *data)
{
	unsigned char cmd[5];
	unsigned long read_len;
	unsigned int i;

	cmd[0]  = 0x6b;//quad_mode->cmd_read;
	mode = 0x5;//quad_mode->sfc_mode;

	for(i = 0; i < 3/*flash->addr_size*/; i++){
		cmd[i + 1] = offset >> (3/*flash->addr_size*/ - i - 1) * 8;
	}

	read_len = g_flash_size/*flash->size*/ - offset;

	if(len < read_len)
		read_len = len;
	/* the paraterms is
		 * cmd , len, addr,addr_len
		 * dummy_byte, daten
		 * dir
		 *
		 * */
	sfc_send_cmd(&cmd[0],read_len,offset,3/*flash->addr_size*/,8/*quad_mode->dummy_byte*/,1,0);

	sfc_read_data(data, len);

	return 0;
}

static void sfc_set_quad_mode()
{
	/* the paraterms is
	 * cmd , len, addr,addr_len
	 * dummy_byte, daten
	 * dir
	 *
	 * */
	unsigned char cmd[5];
	unsigned int buf = 0;
	unsigned int tmp = 0;
	int i = 10;
	unsigned int wrsr_date = 0x2, rdsr_date = 0x2;

    switch(g_sfc_type)
    {
    case 0x1840C8:   //GD25Q128C
        wrsr_date = 0x2;
        rdsr_date = 0x2;
    	cmd[0] = 0x06;//CMD_WREN;
    	cmd[1] = 0x31;//quad_mode->WRSR_CMD;
    	cmd[2] = 0x35;//quad_mode->RDSR_CMD;
    	cmd[3] = 0x05;//CMD_RDSR;
    	g_flash_size = 16*1024*1024;
    	break;
	case 0x17609D:  //IS25LP064A/B
		wrsr_date = 0x40;
        rdsr_date = 0x40;
    	cmd[0] = 0x06;//CMD_WREN;
    	cmd[1] = 0x01;//quad_mode->WRSR_CMD;
    	cmd[2] = 0x05;//quad_mode->RDSR_CMD;
    	cmd[3] = 0x05;//CMD_RDSR;
    	g_flash_size = 8*1024*1024;
    	break;
    default:
        printf("error flash type\n");
        return;
    }
	sfc_send_cmd(&cmd[0],0,0,0,0,0,1);

	sfc_send_cmd(&cmd[1],1/*quad_mode->WD_DATE_SIZE*/,0,0,0,1,1);
	sfc_write_data(&wrsr_date/*&quad_mode->WRSR_DATE*/,1);

	sfc_send_cmd(&cmd[3],1,0,0,0,1,0);
	sfc_read_data(&tmp, 1);

	while(tmp & (1 << 0)) {
		sfc_send_cmd(&cmd[3],1,0,0,0,1,0);
		sfc_read_data(&tmp, 1);
	}

	sfc_send_cmd(&cmd[2], 1/*quad_mode->RD_DATE_SIZE*/,0,0,0,1,0);
	sfc_read_data(&buf, 1);
	while(!(buf & rdsr_date/*quad_mode->RDSR_DATE*/)&&((i--) > 0)) {
		sfc_send_cmd(&cmd[2], 1/*quad_mode->RD_DATE_SIZE*/,0,0,0,1,0);
		sfc_read_data(&buf, 1);
	}
}

static void sfc_nor_RDID(unsigned int *idcode)
{
	/* the paraterms is
	 * cmd , len, addr,addr_len
	 * dummy_byte, daten
	 * dir
	 *
	 * */
	unsigned char cmd[1];
	unsigned int chip_id = 0;
	cmd[0] = 0x9f;//CMD_RDID;
	sfc_send_cmd(&cmd[0],3,0,0,0,1,0);
	sfc_read_data(&chip_id, 1);
	*idcode = chip_id & 0x00ffffff;
}

static void sfc_init(void)
{
	unsigned int tmp;
	int i;
	//1. clk_set_rate
	*(volatile unsigned long *)0xb0000074 = 0xa0000009;
	*(volatile unsigned long *)0xb0000020 &= ~(1 << 2);
	//2. SFC_GLB
	tmp = readl(0xb3440000);
	tmp &= ~((1 << 13) | (1 << 6) );
	tmp |= (1 << 2);
	writel(tmp, 0xb3440000);
	//3. SFC_DEV_CONF
	tmp = readl(0xb3440004);
	tmp &= ~((1 << 15) | (1 << 4) | (1 << 3) | (0x3 << 16) |
			(0x3 << 11) | (0x3 << 9) | (0xf << 5));
	tmp |= ((1 << 2) | (1 << 1) | (1 << 0) | 1 << 16);
	writel(tmp,0xb3440004);
	//4. SFC_TRAN_CONF(i)
	for (i = 0; i < 6; i++) {
		tmp = readl(0xb3440014+4*i);
		tmp &= ~((0x7 << 29) | (1 << 23));
		writel(tmp, 0xb3440014+4*i);
	}
	//5. SFC_INTC
	writel(((1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)), 0xb3440070);
	//6. SFC_CGE
	writel(0, 0xb3440078);
	//7. SFC_GLB
	tmp = readl(0xb3440000);
	tmp &= ~(0x3f << 7);
	tmp |= (31 << 7);
	writel(tmp, 0xb3440000);

	sfc_nor_RDID(&g_sfc_type);
//     printf("sfc init id=%x\n", g_sfc_type);
}

static void sfc_gpio_init(void)
{
	*(volatile unsigned long *)0xb0010018 = (0x3f << 26);
	*(volatile unsigned long *)0xb0010028 = (0x3f << 26);
	*(volatile unsigned long *)0xb0010038 = (0x3f << 26);
	*(volatile unsigned long *)0xb0010044 = (0x3f << 26);
}

void jz_nor_init(void)
{
	sfc_gpio_init();
	sfc_init();
	sfc_set_quad_mode();
}

int  jz_nor_read(unsigned int src_addr, unsigned int count,unsigned int dst_addr)
{
	writel((1 << 2), 0xb3440064);
	return jz_sfc_read(src_addr, count, (void *)(dst_addr));
}

int jz_nor_write(unsigned int src_addr, unsigned int count,unsigned int dst_addr)
{
	if(jz_sfc_erase(src_addr, count))
        return -1;
    
	writel((1 << 2), 0xb3440064);
	return jz_sfc_write(src_addr, count, (void *)(dst_addr));
}

int jz_nor_erase(unsigned int src_addr, unsigned int count)
{
	writel((1 << 2), 0xb3440064);
	return jz_sfc_erase(src_addr, count);
}

