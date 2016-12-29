
#define __MIPS_ASSEMBLER

#include <x1000.h>
#include <mips.h>
#include <mipsregs.h> 
#include <archdefs.h>
#include <cacheops.h>

			
#define RVECENT(f,n) \
   b f; nop
#define XVECENT(f,bev) \
   b f     ;           \
   li k0,bev

#define RESERVED_FOR_SC(x)	.space 1536, x

	.set noreorder

	.globl _start
	.text
_start:
     .word	0x03040506
     .word	0x55aa5502
     .word	0xffff00aa
     .word	0x00000000
	 .space (512-16),0
     RESERVED_FOR_SC(0)

#if 0
loop:
	la      v0, 0xb0010000
	li      t0, 0x100
	sw      t0, 0x74(v0)
	sw      t0, 0x18(v0)
	sw      t0, 0x24(v0)
	sw      t0, 0x38(v0)
	sw      t0, 0x48(v0)
	li      t1, 0x7fffff
loop2:
	subu    t1, t1, 1
	nop
	bgtz    t1, loop2
	nop
	sw      t0, 0x44(v0)
	li      t1, 0x7fffff
loop3:
	subu    t1, t1, 1
	nop
	bgtz    t1, loop3
	nop
	b       loop
	nop
#endif

	/* Invalidate BTB */
	mfc0	v0, CP0_CONFIG, 7
	nop
	ori	v0, v0, 2
	mtc0	v0, CP0_CONFIG, 7
	nop

	/*
	 * CU0=UM=EXL=IE=0, BEV=ERL=1, IP2~7=1
	 */
	li	t0, 0x0040FC04
	mtc0	t0, CP0_STATUS

	/* CAUSE register */
	/* IV=1, use the specical interrupt vector (0x200) */
	li	t1, 0x00800000
	mtc0	t1, CP0_CAUSE

	/* enable bridge radical mode */
	la	t0, CPM_BASE
	lw	t1, 0x24(t0)
	ori	t1, t1, 0x22
	sw	t1, 0x24(t0)

	.set	mips32
init_caches:
	li	t0, CONF_CM_CACHABLE_NONCOHERENT
	mtc0	t0, CP0_CONFIG
	nop

	/* enable idx-store-data cache insn */
	li      t0, 0x20000000
	mtc0    t0, CP0_ECC

	li	t1, 0x80000000		/* Start address */
#define CACHE_ALLOC_END (16384 - 32)

	ori     t2, t1, CACHE_ALLOC_END	/* End address */
	mtc0	zero, CP0_TAGLO, 0
	mtc0	zero, CP0_TAGLO, 1
cache_clear_a_line:
	cache   0x08, 0(t1)
	cache   0x09, 0(t1)
	addiu   t1, t1, 32
	bne     t1, t2, cache_clear_a_line
	nop

	li	t1, 0x80000000		/* Start address */
	ori     t2, t1, CACHE_ALLOC_END	/* End address */
	

	j	spl_main
	nop

