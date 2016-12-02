/* along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <bsp.h>
#include <x1000.h>
#include <mscboot.h>
#include "../../include/common/ddr_timing.h"

extern void serial_puts(const char *s);
extern void serial_put_hex(unsigned int d);

/* new add */
void ddr_cfg_init(void)
{
	register unsigned int ddrc_cfg_reg = 0, tmp;

	tmp = DDR_CL - 1;
	if (tmp < 0)
		tmp = 0;
	if (tmp > 4)
		tmp = 4;

	ddrc_cfg_reg = DDRC_CFG_TYPE_MDDR | DDRC_CFG_IMBA | DDR_DW32 | DDRC_CFG_MPRT | (tmp | 0x8) << 2
		| (DDR_ROW - 12) << 11  | (DDR_COL - 8) << 8   | DDR_CS0EN << 6 | DDR_BANK8 << 1
		| (DDR_ROW - 12) << 27 | (DDR_COL - 8) << 24 | DDR_CS1EN << 7 | DDR_BANK8 << 23 ;

	if (DDR_BL > 4)
		ddrc_cfg_reg |= 1 << 21;

	REG_DDRC_CFG = ddrc_cfg_reg;
}

#define DDRP_PTR0_tDLLSRST 	50	// 50ns
#define DDRP_tDLLLOCK 		5120 	// 5.12us
#define DDRP_PTR0_ITMSRST_8 	8	// 8tck
#define DDRP_PTR1_DINIT0_MDDR	200 * 1000	//200us
#define DDRP_PTR1_DINIT1_MDDR	100	// reverse
#define DDRP_PTR2_DINIT2_MDDR	100	// reverse
#define DDRP_PTR2_DINIT3_MDDR	100	// reverse

void ddr_phy_init(unsigned long ps, unsigned int dtpr0_reg)
{
	int i;
	unsigned int pir_val, val;
	unsigned int timeout = 10000;
	REG_DDRP_DCR = 0x00000000;//DDRP_DCR_TYPE_MDDR | (DDR_BANK8 << 3);
	REG_DDRP_MR0 = 0x00000042;//DDR_CL << 4 | 3;
	REG_DDRP_PTR0 = 0x0021000a;//Phy_tITMSRST << 18 | Phy_tDLLLOCK << 6 | Phy_tDLLSRST;
	REG_DDRP_PTR1 = 0x00a09c40;//(REG_DDRP_PTR1 & (0xff << 19)) | Phy_DINIT0;
	REG_DDRP_PTR2 = 0x00280014;
	REG_DDRP_DTPR0 = 0x1869442e;//tccd << 31 | tRC << 25 | tRRD << 21 | tRAS << 16 | tRCD << 12
//		| tRP << 8 | tWTR << 5 | tRTP << 2 | tMRD;
	REG_DDRP_DTPR1 = 0x000f0048;//tRFC << 16 | tFAW << 3;
	REG_DDRP_DTPR2 = 0x1ff80828;//Phy_tDLLK << 19 | tCKE << 15 | tXP << 10 | 2/*tXSR*/;

	for(i = 0; i < 4; i++){
		unsigned int tmp = REG_DDRP_DXGCR(i);
		tmp &= ~(3 << 9);
		if(i > 1)
			tmp &= ~(1 << 0);
		REG_DDRP_DXGCR(i) = tmp;
	}
	
	REG_DDRP_PGCR = 0x01042e03;//DDRP_PGCR_ITMDMD | DDRP_PGCR_DQSCFG | 7 << DDRP_PGCR_CKEN_BIT | 2 << DDRP_PGCR_CKDV_BIT | (DDR_CS0EN | DDR_CS1EN << 1) << DDRP_PGCR_RANKEN_BIT | DDRP_PGCR_PDDISDX;
	REG_DDRP_ACIOCR = 0x30c00813;
	REG_DDRP_DXCCR = 0x4910;//0x4802;

	while (!(REG_DDRP_PGSR == (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE)) 
			&& (REG_DDRP_PGSR != 0x1f)/* && (--timeout)*/)
		;
//	serial_puts("pgsr = ");serial_put_hex(REG_DDRP_PGSR);
	if(timeout == 0) {
		serial_puts("DDR PHY init timeout: PGSR=");
		serial_put_hex(REG_DDRP_PGSR);
	}

/*	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE)) {
		if (REG_DDRP_PGSR == 0x1f)
			break;
	}
*/
	pir_val = (1 << 0);
	pir_val |= (1 << 6);
	pir_val |= (1 << 17) | (1 << 29);
	pir_val &= ~(1 << 1);
	*(volatile unsigned long *)0xb3011014 = (1 << 31);
	val = *(volatile unsigned long *)0xb301102c;
	val &= ~(1 << 4);
	*(volatile unsigned long *)0xb301102c = val;
	val = *(volatile unsigned long *)0xb3011010;
	val |= (1 << 23);
	*(volatile unsigned long *)0xb3011010 = val;
	REG_DDRP_PIR = pir_val;//DDRP_PIR_INIT | DDRP_PIR_DRAMINT;

	timeout = 10000;
	while (!(REG_DDRP_PGSR == (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE)) 
			&& (REG_DDRP_PGSR != 0x1f)/* && (--timeout)*/)
		;
//	serial_puts("pgsr = ");serial_put_hex(REG_DDRP_PGSR);
	if(timeout == 0) {
		serial_puts("DDR init timeout: PGSR=");
		serial_put_hex(REG_DDRP_PGSR);
	}

/*	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE)) {
		if (REG_DDRP_PGSR == 0x1f)
			break;
	}
*/
	pir_val = (1 << 0);
	pir_val |= (1 << 7) | (1 << 2);
	pir_val |= (1 << 17) | (1 << 29);
	pir_val &= ~(1 << 2);
	REG_DDRP_PIR = pir_val;//DDRP_PIR_INIT | DDRP_PIR_DLLLOCK | DDRP_PIR_QSTRN;
	timeout = 500000;
	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE | DDRP_PGSR_DTDONE)
			&& (!REG_DDRP_PGSR & ((1 << 4) | (1 << 5) | (1 << 6)))/* && (--timeout)*/)
		;
//	serial_puts("pgsr = ");serial_put_hex(REG_DDRP_PGSR);
	if(timeout == 0) {
		serial_puts("DDR training timeout\n");
	}else if(REG_DDRP_PGSR & ((1 << 5) | (1 << 6))){
		serial_puts("DDR hardware training error\n");
	}

/*
	serial_puts("######DDRP registers######\n");
	serial_put_hex(*(volatile unsigned long *)0xb3011000);
	serial_put_hex(*(volatile unsigned long *)0xb3011004);
	serial_put_hex(*(volatile unsigned long *)0xb3011008);
	serial_put_hex(*(volatile unsigned long *)0xb301100c);
	serial_puts("\n");
	serial_put_hex(*(volatile unsigned long *)0xb3011010);
	serial_put_hex(*(volatile unsigned long *)0xb3011014);
	serial_put_hex(*(volatile unsigned long *)0xb3011018);
	serial_put_hex(*(volatile unsigned long *)0xb301101c);
	serial_puts("\n");
	serial_put_hex(*(volatile unsigned long *)0xb3011020);
	serial_put_hex(*(volatile unsigned long *)0xb3011024);
	serial_put_hex(*(volatile unsigned long *)0xb3011028);
	serial_put_hex(*(volatile unsigned long *)0xb301102c);
	serial_puts("\n");
	serial_put_hex(*(volatile unsigned long *)0xb3011030);
	serial_put_hex(*(volatile unsigned long *)0xb3011034);
	serial_put_hex(*(volatile unsigned long *)0xb3011038);
	serial_put_hex(*(volatile unsigned long *)0xb301103c);
	serial_puts("\n");
	serial_put_hex(*(volatile unsigned long *)0xb3011040);
	serial_put_hex(*(volatile unsigned long *)0xb3011044);
	serial_put_hex(*(volatile unsigned long *)0xb3011048);
	serial_put_hex(*(volatile unsigned long *)0xb301104c);
	serial_puts("\n");
	serial_put_hex(*(volatile unsigned long *)0xb3011050);
	serial_put_hex(*(volatile unsigned long *)0xb3011054);
	serial_put_hex(*(volatile unsigned long *)0xb3011058);
	serial_put_hex(*(volatile unsigned long *)0xb301105c);
	serial_puts("\n");
	serial_put_hex(*(volatile unsigned long *)0xb30110c0);
	serial_put_hex(*(volatile unsigned long *)0xb30110c4);
	serial_put_hex(*(volatile unsigned long *)0xb30110c8);
	serial_put_hex(*(volatile unsigned long *)0xb30110cc);
	serial_puts("\n");
	serial_put_hex(*(volatile unsigned long *)0xb30110d0);
	serial_put_hex(*(volatile unsigned long *)0xb30110d4);
	serial_put_hex(*(volatile unsigned long *)0xb30110d8);
	serial_put_hex(*(volatile unsigned long *)0xb30110dc);
	serial_puts("\n");
	for(i =0; i < 4; i++)
		serial_put_hex(*(volatile unsigned long *)(0xb3011000 + 0x1c0 + 0x40 * i));
	serial_puts("######DDRP registers######\n");
*/

#if 0
	unsigned int count = 0, i = 0, bl;
	unsigned int tccd = !!(tCCD > (DDR_BL / 2));

	REG_DDRP_DCR = DDRP_DCR_TYPE_MDDR | (DDR_BANK8 << 3);

	REG_DDRP_PTR0 = Phy_tITMSRST << 18 | Phy_tDLLLOCK << 6 | Phy_tDLLSRST;
	REG_DDRP_PTR1 = (REG_DDRP_PTR1 & (0xff << 19)) | Phy_DINIT0;

	/* brust length must be 8, when MDDR DQS training */
	REG_DDRP_MR0 = DDR_CL << 4 | 3;
	/* MR2 ??? */

	REG_DDRP_DTPR0 = tccd << 31 | tRC << 25 | tRRD << 21 | tRAS << 16 | tRCD << 12
		| tRP << 8 | tWTR << 5 | tRTP << 2 | tMRD;
	REG_DDRP_DTPR1 = tRFC << 16 | tFAW << 3;
	REG_DDRP_DTPR2 = Phy_tDLLK << 19 | tCKE << 15 | tXP << 10 | 2/*tXSR*/;

	REG_DDRP_PGCR = DDRP_PGCR_ITMDMD | DDRP_PGCR_DQSCFG | 7 << DDRP_PGCR_CKEN_BIT | 2 << DDRP_PGCR_CKDV_BIT | (DDR_CS0EN | DDR_CS1EN << 1) << DDRP_PGCR_RANKEN_BIT | DDRP_PGCR_PDDISDX;

	REG_DDRP_ACIOCR = 0x30c00813;
	REG_DDRP_DXCCR = 0x4802;

	/* set up ODT ? */

	/* bus width */
	if (DDR_DW32 == 0) {
		serial_puts("16 bit DDR3 SDRAM\n");
		REG_DDRP_DXGCR(2) = 0;
		REG_DDRP_DXGCR(3) = 0;
	} else {
		serial_puts("32 bit DDR3 SDRAM\n");
	}

	serial_puts("MDDR Init PHY\n");
	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE)) {
		if (REG_DDRP_PGSR == 0x1f)
			break;
	}

#if (DLL_OFF)
	serial_puts("MDDR Init(DLL Bypass)\n");
	REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_DRAMINT | DDRP_PIR_DLLBYP;
#else
	serial_puts("MDDR Init\n");
	REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_DRAMINT;
#endif
	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE)) {
		if (REG_DDRP_PGSR == 0x1f)
			break;
	}

	serial_puts("MDDR Hardware Train\n");
	/* DLLLOCK ? */
	REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_DLLLOCK | DDRP_PIR_QSTRN;
	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE | DDRP_PGSR_DTDONE)) {
		if ((count++ == CFG_CPU_SPEED) || (REG_DDRP_PGSR & (DDRP_PGSR_DTERR | DDRP_PGSR_DTIERR))) {
			serial_puts("Init PHY: DDR TRAIN DONE\n");
			serial_puts("REG_DDP_PGSR: ");
			serial_put_hex(REG_DDRP_PGSR);
			for (i = 0; i < 4; i++) {
				serial_puts("REG_DDP_GXnGSR: ");
				serial_put_hex(REG_DDRP_DXGSR0(i));
			}
			while (1);
		}
	}

	serial_puts("MDDR use apporpriate BL\n");
	bl = 1;
	while (DDR_BL >> bl)
		bl++;
	REG_DDRP_MR0 = DDR_CL << 4 | (bl - 1);
	REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_DRAMINT;
	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE | DDRP_PGSR_DTDONE))
		;
#endif
}
