/*--------------------------------------------------------------------------------
 * This file contains the memory configuration parameters for the cygnus board.
 * DDR3-1066 info
 */
#ifndef __DDR3_CONFIG_H
#define __DDR3_CONFIG_H

#define DLL_OFF	    0
#define CONFIG_SDRAM_DDR3

/* Chip Select */
#define DDR_CS1EN   0   /* CSEN : whether a ddr chip exists 0 - un-used, 1 - used */
#define DDR_CS0EN   1
#define DDR_DW32    1   /* 0 - 16-bit data width, 1 - 32-bit data width */

/* DDR3 paramters */
#define DDR_ROW     14  /* ROW : 12 to 18 row address    */
#define DDR_COL     10  /* COL :  8 to 14 column address */
#define DDR_BANK8   1 	/* Banks each chip: 0-4bank, 1-8bank */
#define DDR_BL	    8   /* DDR3 Burst length: 0 - 8 burst, 2 - 4 burst , 1 - 4 or 8(on the fly) */
#define DDR_AL      0   /* Additive Latency */
#if (DLL_OFF)
#define DDR_CL      6
#define DDR_CWL     6
#else
#define DDR_CL      7              /* CAS latency: 5 to 14 */
#define DDR_CWL     (DDR_CL - 1)   /* DDR3 only: CAS Write Latency, 5 to 8 */
#endif

/* DDR3: Write Latexncy = tAL + tCWL */
#define DDR_tWL_tck	(DDR_AL + DDR_CWL)
#define DDR_tWL_ps	0
/* DDR3: Read Latency  = tAL + tCL  */
#define DDR_tRL_tck	(DDR_AL + DDR_CL)
#define DDR_tRL_ps	0
/* RDLAT */
#define DDR_tRDLAT_tck	(DDR_tRL_tck - 2)
#define DDR_tRDLAT_ps	0
/* WDLAT */
#define DDR_tWDLAT_tck	(DDR_tWL_tck - 1)
#define DDR_tWDLAT_ps	0
/* CKE minimum pulse width, DDR3 spec no. */
#define DDR_tCKE_tck	3
#define DDR_tCKE_ps	5625
/* PRECHARGE command period to the same bank. */
#define DDR_tRP_tck	0
#define DDR_tRP_ps	14000
/* ACTIVE to ACTIVE command period to the same bank. */
#define DDR_tRC_tck	0
#define DDR_tRC_ps	51000
/* ACTIVE to PRECHARGE command period to the same bank. */
#define DDR_tRAS_tck	0
#define DDR_tRAS_ps	38000
/* ACTIVE to READ or WRITE command period to the same bank. */
#define DDR_tRCD_tck	0
#define DDR_tRCD_ps	14000
/* CAS# to CAS# command delay. */
#define DDR_tCCD_tck	4
#define DDR_tCCD_ps	0
/* READ to PRECHARGE command period. DDR3 spec no. */
#define DDR_tRTP_tck	4
#define DDR_tRTP_ps	7500
/* ACTIVE bank A to ACTIVE bank B command period.  */
#define DDR_tRRD_tck	4
#define DDR_tRRD_ps	7500
/* Four bank activate period.  */
#define DDR_tFAW_tck	0
#define DDR_tFAW_ps	50000
/* WRITE Recovery Time defined by register MR of DDR2 memory. */
#define DDR_tWR_tck	0
#define DDR_tWR_ps	15000
/* WRITE to READ command delay. DDR3 spec no.  */
#define DDR_tWTR_tck	4
#define DDR_tWTR_ps	7500
/* Read to Write delay */
#define DDR_tRTW_tck	(DDR_tRL_tck + DDR_tCCD_tck + 2 - DDR_tWL_tck)
#define DDR_tRTW_ps	0
/* Load-Mode-Register to next valid command period   */
#define DDR_tMRD_tck	4
#define DDR_tMRD_ps	0
/* DDR3 only: Mode Register Set Command update delay */
#define DDR_tMOD_tck	12
#define DDR_tMOD_ps	15000
/* AUTO-REFRESH command period. */
#define DDR_tRFC_tck	0
#define DDR_tRFC_ps	160000
/* Minimum Self-Refresh / Deep-Power-Down. */
#define DDR_tMINSR_tck	50
#define DDR_tMINSR_ps	0
/* Valid Clock Requirement after Self Refresh Entry or Power-Down Entry */
#define DDR_tCKSRE_tck	5
#define DDR_tCKSRE_ps	10000
/* DDR3 only: DLL LOCK */
#define DDR_tDLLLOCK_tck	512
#define DDR_tDLLLOCK_ps		0
/* DDR3 only: EXit self-refresh to command not requiring a locked DLL */
#define DDR_tXS_tck	5
#define DDR_tXS_ps	(DDR_tRFC_ps + 10000)
/* DDR3 only: EXit self-refresh to command     requiring a locked DLL */
#define DDR_tXSDLL_tck	DDR_tDLLLOCK_tck
#define DDR_tXSDLL_ps	DDR_tDLLLOCK_ps
/* DDR3 only: Exit active power down to any valid command */
#define DDR_tXP_tck	3
#define DDR_tXP_ps	7500
/* DDR3 only: Exit active power down to command requirint a locked DLL */
#define DDR_tXPDLL_tck	10
#define DDR_tXPDLL_ps	24000
/* DDR2 only: Exit self refresh to a read command.  */
#define DDR_tXSRD_tck	DDR_tXS_tck
#define DDR_tXSRD_ps	DDR_tXS_ps

/*
 * DDR3 controller refcnt register
 */
#define DDR_tREFI   7800	/* Refresh period: 7.8 us */
#define DDR_CLK_DIV 1	 	/* Clock Divider. auto refresh
				 * cnt_clk = memclk/(16*(2^DDR_CLK_DIV)) */

#endif /* __DDR3_CONFIG_H */
