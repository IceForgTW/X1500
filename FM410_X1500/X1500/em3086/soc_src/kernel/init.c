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

#include <archdefs.h>
#include <mipsregs.h>
#include <bsp.h>
#include <x1000.h>
#include <os_api.h>
#include <MemMgr.h>
#include <uart.h>

#include <string.h>
unsigned int g_stack[8192];

extern void except_common_entry(void);
extern void __dcache_writeback_all(void);
extern void __icache_invalidate_all(void);
extern void Em3000_Main(void);

void consol_send_ch(unsigned char ch)
{
	serial_putc(ch);
}

void consol_get_char(unsigned char *ch)
{
	int r;
	r = serial_getc();
	if (r > 0)
		*ch = (unsigned char)r;
	else
		*ch = 0;
}
//----------------------------------------------------------------------------------
//cchappy tlb  0x30000000 to 0xC0000000
//----------------------------------------------------------------------------------
void setup_tlb(void)
{
	unsigned int pagemask = 0x007fe000;//0x01ffe000; /* 4MB */
/* cached D:allow-W V:valid G */
	unsigned int entrylo0 = (0x30000000 >> 6) | ((3 << 3) + (1 << 2) + (1 << 1) + 1);
	unsigned int entrylo1 = (0x30400000 >> 6) | ((3 << 3) + (1 << 2) + (1 << 1) + 1);
	unsigned int entryhi = 0xc0000000; /* kseg2 base */
	int i;
	__write_32bit_c0_register($5, 4, 0xa9000000);
	write_c0_pagemask(pagemask);
	write_c0_wired(0);
/* indexed write 32 tlb entry */
	for(i = 0; i < 32; i++)
	{
		asm (
		".macro _ssnop; sll $0, $0, 1; .endm\n\t"
		".macro _ehb; sll $0, $0, 3; .endm\n\t"
		".macro mtc0_tlbw_hazard; _ssnop; _ssnop; _ehb; .endm\n\t"
		".macro tlbw_use_hazard; _ssnop; _ssnop; _ssnop; _ehb; .endm\n\t"
		"\n\t"
		"mtc0 %0, $0\n\t" /* write Index */
		"tlbw_use_hazard\n\t"
		"mtc0 %1, $5\n\t" /* write PageMask */
		"mtc0 %2, $10\n\t" /* write EntryHi */
		"mtc0 %3, $2\n\t" /* write EntryLo0 */
		"mtc0 %4, $3\n\t" /* write EntryLo1 */
		"mtc0_tlbw_hazard\n\t"
		"tlbwi \n\t" /* TLB indexed write */
		"tlbw_use_hazard\n\t"
		: : "Jr" (i), "r" (pagemask), "r" (entryhi),
		"r" (entrylo0), "r" (entrylo1)
		);
		entryhi += 0x0800000; /* 32MB */
		entrylo0 += (0x0800000 >> 6);
		entrylo1 += (0x0800000 >> 6);
	}
}
void mmu_dump_tlb(void)
{
	int i;
	serial_puts("Dump MMU tlb entries:\n");
	serial_puts("index  mask    entryhi  entrylo0 entrylo1\n");
	for (i = 0; i < 32; i++) {
		write_c0_index(i);
		__asm__ __volatile__("nop;nop;nop;nop;");
		__asm__ __volatile__("tlbr");
		__asm__ __volatile__("nop;nop;nop;nop;");
		serial_puts("		pagemask:0x");
		serial_put_hex(read_c0_pagemask());
		serial_puts("	 entryhi:0x");
		serial_put_hex(read_c0_entryhi());
		serial_puts("	entrylo0:0x");
		serial_put_hex(read_c0_entrylo0());
		serial_puts("	entrylo1:0x");
		serial_put_hex(read_c0_entrylo1());		
	}
}

typedef void (*pfunc)(void);

void c_main(void)
{
/*	*(volatile unsigned int *)0xb0010018 = (1 << 8);
	*(volatile unsigned int *)0xb0010024 = (1 << 8);
	*(volatile unsigned int *)0xb0010038 = (1 << 8);
	while(1) {
		*(volatile unsigned int *)0xb0010018 = (1 << 8);
		mdelay(1000);
		*(volatile unsigned int *)0xb0010014 = (1 << 8);
		mdelay(1000);
	}
*/

#ifdef ENABLE_FPU
// 	serial_puts("====>ENABLE_FPU\n");
	write_c0_status(0x30000c00);
#else
// 	serial_puts("=====>NOT ENABLE FPU\n");
	write_c0_status(0x10000c00);
#endif

	memcpy((void *)A_K0BASE, except_common_entry, 0x20);
	memcpy((void *)(A_K0BASE + 0x180), except_common_entry, 0x20);
	memcpy((void *)(A_K0BASE + 0x200), except_common_entry, 0x20);

	__dcache_writeback_all();
	__icache_invalidate_all();

	intc_init();
	
//	serial_init();
//	serial_puts("jwang test...c_main...\n");
	os_Init();

	setup_tlb();  //TLB management	
//	mmu_dump_tlb();

	heapInit();
 
#ifdef EM3000_V5
	Em3000_Main();
#else
	APP_vMain();
#endif
	while(1);
}

