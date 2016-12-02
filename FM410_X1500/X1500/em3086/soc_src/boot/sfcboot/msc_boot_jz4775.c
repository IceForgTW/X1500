/*
 * Copyright (C) 2007 Ingenic Semiconductor Inc.
 * Author: Peter <jlwei@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <bsp.h>
#include <x1000.h>
#include <mscboot.h>

/*
 * External routines
 */
extern int serial_init(void);
extern void serial_puts(const char *s);
extern void serial_puts_hex(unsigned int d);
extern void flush_cache_all(void);
extern void sdram_init();

#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
int mode = 0;

static int g_sfc_type=0;
static int g_flash_size=0;

void apll_init(void)
{
        int div[5] = {8, 4, 3, 2, 1};
        register unsigned int clk_ctrl;
//        unsigned int temp;

	REG_CPM_CPAPCR = CPAPCR_M_N_OD | CPM_CPAPCR_EN | 0x20;
        while (!(REG_CPM_CPAPCR & CPM_CPAPCR_ON)) ;

        /* init CPU, L2CACHE, AHB0, AHB2, APB clock */
        clk_ctrl = REG_CPM_CPCCR & (0xff << 24);
        clk_ctrl |= CPCCR_CE;
        clk_ctrl |= ((div[0] - 1) << CPCCR_PDIV_LSB)  |
                   ((div[1] - 1) << CPCCR_H2DIV_LSB) |
                   ((div[2] - 1) << CPCCR_H0DIV_LSB) |
                   ((div[3] - 1) << CPCCR_L2DIV_LSB) |
                   ((div[4] - 1) << CPCCR_CDIV_LSB);


        REG_CPM_CPCCR = clk_ctrl;
        while (REG_CPM_CPCSR & CPCSR_DIV_BUSY) ;

	/*
	 * 1, must set cpu and ahb0 get the clock from the same pll source.
	 * 2, consider the need of dynamic change cpu freq , set cpu and ahb0 get clock from apll, ahb2, ddr, and other peripherals
	 * get clock from mpll.
	 */
        clk_ctrl = REG_CPM_CPCCR;
        clk_ctrl &= ~(0xff << 24);
        clk_ctrl |= (/*CPM_SRC_SEL_APLL*/ 2 << CPCCR_SEL_SRC_LSB) | (CPM_PLL_SEL_SRC << CPCCR_SEL_CPLL_LSB) |
                        (/*CPM_PLL_SEL_SRC*/CPM_PLL_SEL_MPLL << CPCCR_SEL_H0PLL_LSB) | (CPM_PLL_SEL_MPLL << CPCCR_SEL_H2PLL_LSB);
        REG_CPM_CPCCR = clk_ctrl;

        clk_ctrl = REG_CPM_CPCCR;
        clk_ctrl &= ~(0xff << 24);

//         serial_puts("REG_CPM_CPCCR = ");
//         serial_put_hex(REG_CPM_CPCCR);
//         serial_puts("REG_CPM_CPAPCR = ");
//         serial_put_hex(REG_CPM_CPAPCR);	
}

void mpll_init(void)
{
//        unsigned int temp;

        REG_CPM_CPMPCR = CPMPCR_M_N_OD | CPM_CPMPCR_EN;
        while (!(REG_CPM_CPMPCR & CPM_CPMPCR_ON)) ;

         /*
         * Init DDR clock
         */
        REG_CPM_DDCDR = DDCDR_DCS_MPLL | DDCDR_CE_DDR | (CFG_DIV - 1);
        while (REG_CPM_DDCDR & DDCDR_DDR_BUSY) ;

// 		serial_puts("REG_CPM_CPMPCR = ");
//         serial_put_hex(REG_CPM_CPMPCR);
//         serial_puts("===> REG_CPM_DDCDR = ");
//         serial_put_hex(REG_CPM_DDCDR);
//         serial_puts("mpll init over\n");
}
void pll_init()
{
	mpll_init();
	apll_init();
}

void udelay(unsigned int usec)
{
        unsigned int i = usec * (CFG_CPU_SPEED / 2000000);

	__asm__ __volatile__ (
			"\t.set noreorder\n"
			"1:\n\t"
			"bne\t%0, $0, 1b\n\t"
			"addi\t%0, %0, -1\n\t"
			".set reorder\n"
			: "=r" (i)
			: "0" (i)
			);
}

void mdelay(int sdelay)
{
	udelay(sdelay * 1000);	
}

#define writel(b, a) (*((volatile unsigned long *)(a)) = (unsigned long)b)
#define readl(x)   ((unsigned long)(*(volatile unsigned long *)(x)))


void sfc_send_cmd(unsigned char *cmd,unsigned int len,unsigned int addr ,unsigned addr_len,unsigned dummy_byte,unsigned int daten,unsigned char dir)
{
//	serial_puts("mode = ");serial_put_hex(mode);
//	serial_puts("cmd = ");serial_put_hex(*cmd);
//	serial_puts("len = ");serial_put_hex(len);
//	serial_puts("addr = ");serial_put_hex(addr);
//	serial_puts("addr_len = ");serial_put_hex(addr_len);
//	serial_puts("dummy_byte = ");serial_put_hex(dummy_byte);
//	serial_puts("daten = ");serial_put_hex(daten);
//	serial_puts("dir = ");serial_put_hex(dir);
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
//serial_puts("sfc_read_data\n");	
	while(1){
		reg_tmp = readl(0xb3440068);
//		serial_put_hex(reg_tmp);
		if (reg_tmp & (1 << 2)) {
			writel((1 << 2), 0xb344006c);
			if ((len - tmp_len) > 31)
				fifo_num = 31;
			else {
				fifo_num = len - tmp_len;
			}
//			serial_put_hex(fifo_num);
			for (i = 0; i < fifo_num; i++) {
				*data = readl(0xb3441000);
//				serial_puts("data = ");serial_put_hex(data);
//				serial_puts("*data = ");serial_put_hex(*data);
//serial_puts("dst = ");serial_put_hex(*(unsigned int *)0x80800000);
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

int jz_sfc_read(u32 offset, size_t len, void *data)
{
	unsigned char cmd[5];
	unsigned long read_len;
//	unsigned int words_of_len = 0;
	unsigned int i;
//serial_puts("offset = ");serial_put_hex(offset);
//serial_puts("len = ");serial_put_hex(len);
//serial_puts("data = ");serial_put_hex(data);
	cmd[0]  = 0x6b;//quad_mode->cmd_read;
	mode = 0x5;//quad_mode->sfc_mode;

	for(i = 0; i < 3; i++){
		cmd[i + 1] = offset >> (3 - i - 1) * 8;
	}

	read_len = g_flash_size - offset;

	if(len < read_len)
		read_len = len;
	/* the paraterms is
		 * cmd , len, addr,addr_len
		 * dummy_byte, daten
		 * dir
		 *
		 * */

	sfc_send_cmd(&cmd[0],read_len,offset,3,8,1,0);
	
	sfc_read_data(data, len);

	return 0;
}

void sfc_set_quad_mode()
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
    case 0x1740C8:   //GD25Q128C
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
        serial_puts("boot:error flash type\n");
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

// 	serial_puts("set quad mode is enable.the buf = ");
// 	serial_put_hex(buf);
}

void sfc_nor_RDID(unsigned int *idcode)
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
// 	serial_puts("idcode = ");serial_put_hex(*idcode);
}

void sfc_init(void)
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

	sfc_nor_RDID((unsigned int *)&g_sfc_type);
}

void sfc_gpio_init(void)
{
	*(volatile unsigned long *)0xb0010018 = (0x3f << 26);
	*(volatile unsigned long *)0xb0010028 = (0x3f << 26);
	*(volatile unsigned long *)0xb0010038 = (0x3f << 26);
	*(volatile unsigned long *)0xb0010044 = (0x3f << 26);
}

void sfc_nor_read(unsigned int src_addr, unsigned int count,unsigned int dst_addr)
{
	sfc_gpio_init();
	sfc_init();
	sfc_set_quad_mode();
	writel((1 << 2), 0xb3440064);
	jz_sfc_read(src_addr, count, (void *)(dst_addr));
}

void spl_main(void)
{
	void (*xboot)(void);

	//照明灯、对焦灯的控制管脚上电默认为高电平，灯亮，需初始化关灯
	__gpio_as_output(32*0+5);
	__gpio_set_pin(32*0+5);
	__gpio_as_output(32*2+24);
	__gpio_set_pin(32*2+24);
      __gpio_as_output(32*1+22);
	__gpio_set_pin(32*1+22);


	// This function can avoid UART floating, but should not call if UART will be in high frequency.
	serial_init();
// 	serial_puts("\nSerial is ok...\n");

	pll_init();
	sdram_init();

// 	serial_puts("Sdram initted ...\n");

	/*
	 * Load X-Boot image from NAND into RAM
	 */
	sfc_nor_read(8*1024, CFG_NAND_MINIOS_SIZE, CFG_NAND_MINIOS_DST);
	xboot = (void (*)(void))CFG_NAND_MINIOS_START;
	
// 	serial_puts("***********************\n");
//     serial_put_hex(*(unsigned int *)CFG_NAND_MINIOS_START);
// 	serial_puts("***********************\n");

// 	serial_puts("Starting X-Boot OK...\n\n");

	/*
	 * Flush caches
	 */
	flush_cache_all();

	/*
	 * Jump to x-boot image
	 */
	(*xboot)();
}
