/*
 * Jz4780 ddr routines
 *
 *  Copyright (c) 2006
 *  Ingenic Semiconductor, <cwjia@ingenic.cn>
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
#include "../../include/common/ddr_timing.h"
#include "../../include/common/time-tcu.h"
#undef DEBUG
//#define DEBUG

#ifdef DEBUG
#define dprintf(fmt,args...)	printf(fmt, ##args)
#else
#define dprintf(fmt,args...)	{}
#endif

#define DDR_DMA_BASE  (0xa0000000)		/*un-cached*/

void sdram_init(void);
void remap_swap(int, int);

extern void ddr_cfg_init(void);
extern void ddr_phy_init(void);
extern void serial_put_hex(unsigned int  d);
extern void serial_puts (const char *s);

#ifdef DEBUG
static void dump_jz_dma_channel(unsigned int dmanr)
{
	serial_puts("====================================================================\n");
	dprintf("DMA%d Registers:\n", dmanr);
	dprintf("  DMACR  = 0x%08x\n", REG_DMAC_DMACR(dmanr));
	dprintf("  DMACPR = 0x%08x\n", REG_DMAC_DMACPR(0));
	dprintf("  DSAR   = 0x%08x\n", REG_DMAC_DSAR(dmanr));
	dprintf("  DTAR   = 0x%08x\n", REG_DMAC_DTAR(dmanr));
	dprintf("  DTCR   = 0x%08x\n", REG_DMAC_DTCR(dmanr));
	dprintf("  DRSR   = 0x%08x\n", REG_DMAC_DRSR(dmanr));
	dprintf("  DCCSR  = 0x%08x\n", REG_DMAC_DCCSR(dmanr));
	dprintf("  DCMD  = 0x%08x\n", REG_DMAC_DCMD(dmanr));
	dprintf("  DDA  = 0x%08x\n", REG_DMAC_DDA(dmanr));
	dprintf("  DMADBR = 0x%08x\n", REG_DMAC_DMADBR(dmanr));
	serial_puts("====================================================================\n");
}

static void ddr_control_regs_print(void)
{
	serial_puts("====================================================================\n");
	dprintf("DDRC REGS:\n");
	dprintf("REG_DDRC_ST \t\t= 0x%08x\n", REG_DDRC_ST);
	dprintf("REG_DDRC_CFG \t\t= 0x%08x\n", REG_DDRC_CFG);
	dprintf("REG_DDRC_CTRL \t\t= 0x%08x\n", REG_DDRC_CTRL);
	dprintf("REG_DDRC_LMR \t\t= 0x%08x\n", REG_DDRC_LMR);
	dprintf("REG_DDRC_TIMING1 \t= 0x%08x\n", REG_DDRC_TIMING(1));
	dprintf("REG_DDRC_TIMING2 \t= 0x%08x\n", REG_DDRC_TIMING(2));
	dprintf("REG_DDRC_TIMING3 \t= 0x%08x\n", REG_DDRC_TIMING(3));
	dprintf("REG_DDRC_TIMING4 \t= 0x%08x\n", REG_DDRC_TIMING(4));
	dprintf("REG_DDRC_TIMING5 \t= 0x%08x\n", REG_DDRC_TIMING(5));
	dprintf("REG_DDRC_TIMING6 \t= 0x%08x\n", REG_DDRC_TIMING(6));
	dprintf("REG_DDRC_MMAP0 \t\t= 0x%08x\n", REG_DDRC_MMAP0);
	dprintf("REG_DDRC_MMAP1 \t\t= 0x%08x\n", REG_DDRC_MMAP1);
	dprintf("REG_DDRC_REFCNT \t= 0x%08x\n", REG_DDRC_REFCNT);
	dprintf("REG_DDRC_REMAP1 \t= 0x%08x\n", REG_DDRC_REMAP(1));
	dprintf("REG_DDRC_REMAP2 \t= 0x%08x\n", REG_DDRC_REMAP(2));
	dprintf("REG_DDRC_REMAP3 \t= 0x%08x\n", REG_DDRC_REMAP(3));
	dprintf("REG_DDRC_REMAP4 \t= 0x%08x\n", REG_DDRC_REMAP(4));
	dprintf("REG_DDRC_REMAP5 \t= 0x%08x\n", REG_DDRC_REMAP(5));
	serial_puts("====================================================================\n");
}

static void ddr_phy_regs_print(void)
{
	serial_puts("====================================================================\n");
	dprintf("DDR PHY REGS:\n");
	dprintf("REG_DDRP_PGCR \t\t= 0x%08x\n", REG_DDRP_PGCR);
	dprintf("REG_DDRP_PGSR \t\t= 0x%08x\n", REG_DDRP_PGSR);
	dprintf("REG_DDRP_DCR \t\t= 0x%08x\n", REG_DDRP_DCR);
	dprintf("REG_DDRP_PTR0 \t\t= 0x%08x\n", REG_DDRP_PTR0);
	dprintf("REG_DDRP_PTR1 \t\t= 0x%08x\n", REG_DDRP_PTR1);
	dprintf("REG_DDRP_PTR2 \t\t= 0x%08x\n", REG_DDRP_PTR2);
	dprintf("REG_DDRP_DTPR0 \t\t= 0x%08x\n", REG_DDRP_DTPR0);
	dprintf("REG_DDRP_DTPR1 \t\t= 0x%08x\n", REG_DDRP_DTPR1);
	dprintf("REG_DDRP_DTPR2 \t\t= 0x%08x\n", REG_DDRP_DTPR2);
	dprintf("REG_DDRP_MR0 \t\t= 0x%08x\n", REG_DDRP_MR0);
	dprintf("REG_DDRP_MR1 \t\t= 0x%08x\n", REG_DDRP_MR1);
	dprintf("REG_DDRP_MR2 \t\t= 0x%08x\n", REG_DDRP_MR2);
	dprintf("REG_DDRP_MR3 \t\t= 0x%08x\n", REG_DDRP_MR3);
	dprintf("REG_DDRP_DTAR \t\t= 0x%08x\n", REG_DDRP_DTAR);
	dprintf("REG_DDRP_ACIOCR \t= 0x%08x\n", REG_DDRP_ACIOCR);
	dprintf("REG_DDRP_DXCCR \t\t= 0x%08x\n", REG_DDRP_DXCCR);
	dprintf("REG_DDRP_DSGCR \t\t= 0x%08x\n", REG_DDRP_DSGCR);
	dprintf("REG_DDRP_DXDQSTR(0) \t= 0x%08x\n", REG_DDRP_DXDQSTR(0));
	dprintf("REG_DDRP_DXGCR(0) \t= 0x%08x\n", REG_DDRP_DXDQSTR(0));
	dprintf("REG_DDRP_DXDQSTR(1) \t= 0x%08x\n", REG_DDRP_DXDQSTR(1));
	dprintf("REG_DDRP_DXGCR(1) \t= 0x%08x\n", REG_DDRP_DXDQSTR(1));
	serial_puts("====================================================================\n");
}
#endif /* DEBUG */

long int initdram(int board_type)
{
	u32 ddr_cfg;
	u32 rows, cols, dw, banks, cs0, cs1;
	unsigned long size = 0;

	ddr_cfg = REG_DDRC_CFG;
	rows = 12 + ((ddr_cfg & DDRC_CFG_ROW_MASK) >> DDRC_CFG_ROW_BIT);
	cols = 8 + ((ddr_cfg & DDRC_CFG_COL_MASK) >> DDRC_CFG_COL_BIT);

	dw = (ddr_cfg & DDRC_CFG_DW) ? 4 : 2;
	banks = (ddr_cfg & DDRC_CFG_BA) ? 8 : 4;
	cs0 = (ddr_cfg & DDRC_CFG_CS0EN) ? 1 : 0;
	cs1 = (ddr_cfg & DDRC_CFG_CS1EN) ? 1 : 0;

//	dprintf("rows=%d, cols=%d, dw=%d, banks=%d, cs0=%d, cs1=%d\n", rows, cols, dw, banks, cs0, cs1);

	size = (1 << (rows + cols)) * dw * banks;
	size *= (cs0 + cs1);

	return size;
}

#define REG_REMAP(bit) REG_DDRC_REMAP(bit / 4 + 1)
#define BIT(bit) ((bit % 4) * 8)
#define MASK(bit) (0x1f << BIT(bit))

void remap_swap(int a, int b)
{
	int tmp1 = 0, tmp2 = 0;

	tmp1 = (REG_REMAP(a) & MASK(a)) >> BIT(a);
	tmp2 = (REG_REMAP(b) & MASK(b)) >> BIT(b);

	REG_REMAP(a) &= ~MASK(a);
	REG_REMAP(b) &= ~MASK(b);

	REG_REMAP(a) |= tmp2 << BIT(a);
	REG_REMAP(b) |= tmp1 << BIT(b);

	dprintf("%d <==> %d\n", a, b);
	dprintf("REG_DDRC_REMAP(%d) = 0x%08x\n", a / 4 + 1, REG_REMAP(a));
	dprintf("REG_DDRC_REMAP(%d) = 0x%08x\n", b / 4 + 1, REG_REMAP(b));
}

static void mem_remap(void)
{
	u32 ddr_cfg, start = 0;
	u32 rows, cols, dw, banks, cs0, cs1;
	int num = 0;

	ddr_cfg = REG_DDRC_CFG;
	rows = 12 + ((ddr_cfg & DDRC_CFG_ROW_MASK) >> DDRC_CFG_ROW_BIT);
	cols = 8 + ((ddr_cfg & DDRC_CFG_COL_MASK) >> DDRC_CFG_COL_BIT);

	dw = (ddr_cfg & DDRC_CFG_DW) ? 4 : 2;
	banks = (ddr_cfg & DDRC_CFG_BA) ? 8 : 4;
	cs0 = (ddr_cfg & DDRC_CFG_CS0EN) ? 1 : 0;
	cs1 = (ddr_cfg & DDRC_CFG_CS1EN) ? 1 : 0;

	start += rows + cols + dw / 2;
	start -= 12;
	if (banks == 8)
		num += 3;
	else
		num += 2;
	if (cs0 && cs1)
		num++;
	dprintf("start = %d, num = %d\n", start, num);

	for (; num > 0; num--) {
		if ((start + num - 1) == 17)
			continue;
		remap_swap(0 + num - 1, start + num - 1);
	}
}

static void ddr_dll_reset(void)
{
	* (volatile unsigned *) 0xb00000d0 = 0x73 | (1 << 6);//0x3;
	mdelay(5/*400*/);
	/* serial_puts("ddr dll reset over\n"); */
	* (volatile unsigned *) 0xb00000d0 = 0x71 | (1 << 6);//0x1;
	mdelay(5/*400*/);
}

/* DDR sdram init */
void sdram_init(void)
{
	//unsigned int ddrc_mmap0_reg, ddrc_mmap1_reg, mem_base0, mem_base1, mem_mask0, mem_mask1;
	//unsigned int memsize, memsize0, memsize1, init_ddrc_ctrl;
	//unsigned int tminsr = (0 == (tMINSR - 1) % 8) ? ((tMINSR - 1) / 8) : ((tMINSR - 1) / 8 + 1);
	//unsigned int trefct = tREFCT, tcksre = tCKSRE / 8, trfc = (tRFC - 1) / 2, tmrd = tMRD - 1;
	//unsigned int twtr = tWL + DDR_BL / 2 + tWTR, txp = 0, txsrd = 0;

//	serial_puts("CPM_DDCDR = ");serial_put_hex(*(volatile unsigned long *)0xb000002c);
	*(volatile unsigned long *)0xb0000020 &= ~(1 << 31);//clkgr:ddr
	ddr_dll_reset();

	//reset_controller
	REG_DDRC_CTRL = 0xf << 20;
mdelay(5);
//	init_ddrc_ctrl = 1 << 15 | 4 << 12 | 1 << 11 | 1 << 8 | 0 << 6 | 1 << 4 | 1 << 3 | 1 << 2 | 1 << 1;

	REG_DDRC_CTRL = 0x0;
mdelay(5);

	//force CKE1 high
REG_DDRC_CFG = 0x0946896c;
REG_DDRC_CTRL = (1 << 1);

	REG_DDRP_DTAR = 0x150000;
	ddr_phy_init();
	REG_DDRC_CTRL = 0x0;
	
	REG_DDRC_CTRL = DDRC_CTRL_CKE | DDRC_CTRL_ALH;
	REG_DDRC_CTRL = 0x0;

	//ddr_cfg_init();
	REG_DDRC_CFG = 0x0946896c;

/*	trefct = trefct / (16 * (1 << DDR_CLK_DIV)) - 2;
	txsrd = 2;//tXSR;
	txp = tXP;
	txsrd = (txsrd % 4) ? (txsrd / 4 + 1) : (txsrd / 4);

	if (tMRD > 4) {
		serial_puts("Too much tMRD -");
		serial_put_hex(tMRD);
		while (1);
	}
	if (txp > 7) {
		serial_puts("Too much tXP -");
		serial_put_hex(tXP);
		while (1);
	}
*/
	REG_DDRC_TIMING(1) = 0x03040501;//   tRTP << 24 |   twtr << 16 |     tWR << 8 | tWL;
	REG_DDRC_TIMING(2) = 0x02090404;//   tCCD << 24 |   tRAS << 16 |    tRCD << 8 | tRL;
	REG_DDRC_TIMING(3) = 0x2704030c;//  tONUM << 27 | tcksre << 24 |    tRP << 16 | tRRD << 8  | tRC;
	REG_DDRC_TIMING(4) = 0x07780021;//   trfc << 24 | tEXTRW << 21 | tRWCOV << 19 | tCKE << 16 | tminsr << 8 | txp << 4 | tmrd;
	REG_DDRC_TIMING(5) = 0xff070200;//tCTLUPD << 24 |   tRTW << 16 |  tRDLAT << 8 | tWDLAT;
	REG_DDRC_TIMING(6) = 0x00090202;//  txsrd << 24 |   tFAW << 16 |   tCFGW << 8 | tCFGR;
/*
	memsize = initdram(0);
	dprintf("total memsize: 0x%08x\n", memsize);

	memsize0 = memsize / (DDR_CS1EN + DDR_CS0EN);
	memsize1 = memsize - memsize0;
	dprintf("memsize0: 0x%08x\n", memsize0);
	dprintf("memsize1: 0x%08x\n", memsize1);
		
	if (memsize > 0x20000000) {
		if (memsize1 || memsize > 0x40000000) {
			mem_base0 = 0x0;
			mem_mask0 = (~((memsize0 >> 24) - 1) & ~(memsize >> 24)) & DDRC_MMAP_MASK_MASK;
			mem_base1 = (memsize1 >> 24) & 0xff;
			mem_mask1 = (~((memsize1 >> 24) - 1) & ~(memsize >> 24)) & DDRC_MMAP_MASK_MASK;
		} else {
			// 1 ddr chip, capacity 1G.
			// when only ddr chip select 0, mmap0 and mmap1 acting on both chip select 0 in jz4775.
			//
			mem_base0 = 0x20;
			mem_base1 = 0x40;
			mem_mask0 = mem_mask1 = 0xe0;
		}
	} else {
		mem_base0 = (DDR_MEM_PHY_BASE >> 24) & 0xff;
		mem_mask0 = ~((memsize0 >> 24) - 1) & DDRC_MMAP_MASK_MASK;
		mem_base1 = ((DDR_MEM_PHY_BASE + memsize0) >> 24) & 0xff;	
		mem_mask1 = ~((memsize1 >> 24) - 1) & DDRC_MMAP_MASK_MASK;
	}

	dprintf("mem_base0 = %x\n", mem_base0);
	dprintf("mem_base1 = %x\n", mem_base1);

	ddrc_mmap0_reg = mem_base0 << DDRC_MMAP_BASE_BIT | mem_mask0;
	ddrc_mmap1_reg = mem_base1 << DDRC_MMAP_BASE_BIT | mem_mask1;
*/
	REG_DDRC_MMAP0 = 0x000020fe;//ddrc_mmap0_reg;
	REG_DDRC_MMAP1 = 0x00002200;//ddrc_mmap1_reg;

//	dprintf("REG_DDRC_MMAP0 \t\t= 0x%08x\n", REG_DDRC_MMAP0);

	REG_DDRC_CTRL = DDRC_CTRL_CKE | DDRC_CTRL_ALH;

	REG_DDRC_REFCNT = 0x002f0003;//trefct << DDRC_REFCNT_CON_BIT | DDR_CLK_DIV << 1 | DDRC_REFCNT_REF_EN;

//	init_ddrc_ctrl = 1 << 15 | 4 << 12 | 1 << 11 | 1 << 8 | 0 << 6 | 1 << 4 | 1 << 3 | 1 << 2 | 1 << 1;
	REG_DDRC_CTRL = 0x0000c91e;//init_ddrc_ctrl;

	REG_DDRC_ST &= ~0x40;

	mem_remap();

/*	serial_puts("******DDR registers******\n");
	serial_put_hex(*(volatile unsigned long *)0xb34f0000);
	serial_put_hex(*(volatile unsigned long *)0xb34f0004);
	serial_put_hex(*(volatile unsigned long *)0xb34f0008);
	serial_put_hex(*(volatile unsigned long *)0xb34f000c);
	serial_puts("******DDR registers******\n");
	*/
// 	serial_puts("sdram init ok\n");
}
