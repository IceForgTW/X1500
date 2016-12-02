/*
 *  Auto Calculate DDR Controller and Synopsys Phy Timing Head File
 *  Support chip: Jz4780, Jz4775
 */
#define MAX(a,b) (((a)>(b))?(a):(b))

#define DDRP_PTR0_tDLLSRST  	50 * 1000		// 50ns
#define DDRP_PTR0_tDLLLOCK	5120 * 1000		// 5.12us
#define DDRP_PTR0_ITMSRST_8 	8			// 8tck
#define DDRP_PTR1_DINIT0_DDR3	500 * 1000 * 1000 	// 500us
#define DDRP_PTR1_DINIT1_DDR3   DDR_tRFC_ps + 10000
#define DDRP_PTR2_DINIT2_DDR3 	200 * 1000 * 1000	// 200us
#define DDRP_PTR2_DINIT3_DDR3	512 			// 512 tck

struct timing {
	char *name;
	unsigned int min;
	unsigned int max;
	unsigned long para_ps;
	unsigned long para_tck;
};

static struct timing timing[] = {
	/* controller timing */
	{"tRTP",    1,  6,    DDR_tRTP_ps,   DDR_tRTP_tck},
	{"tWTR",    1,	6,    DDR_tWTR_ps,   DDR_tWTR_tck},
#if defined(CONFIG_SDRAM_DDR3)
	{"tWR",	    5,  12,    DDR_tWR_ps,    DDR_tWR_tck},
#else
	{"tWR",     2,	6,     DDR_tWR_ps,    DDR_tWR_tck},
#endif
	{"tWL",	    1,  63,    DDR_tWL_ps,    DDR_tWL_tck},
	{"tCCD",    1,  63,   DDR_tCCD_ps,   DDR_tCCD_tck},
	{"tRAS",    1,  31,   DDR_tRAS_ps,   DDR_tRAS_tck},
	{"tRCD",    1,  11,   DDR_tRCD_ps,   DDR_tRCD_tck},
	{"tRL",	    1,  63,    DDR_tRL_ps,    DDR_tRL_tck},
	{"tONUM",   1,  15,		0,		4},
	/* Delay time is tCKSRE*8 (tCK), 7 * 8 = 56 */
	{"tCKSRE",  8,  56, DDR_tCKSRE_ps, DDR_tCKSRE_tck},
	{"tRP",	    1,  11,    DDR_tRP_ps,    DDR_tRP_tck},
	{"tRRD",    1,	8,    DDR_tRRD_ps,   DDR_tRRD_tck},
	{"tRC",	    1,  42,    DDR_tRC_ps,    DDR_tRC_tck},
	/* 63 * 2 + 1 = 127 */
	{"tRFC",    3, 127,   DDR_tRFC_ps,   DDR_tRFC_tck},
	{"tEXTRW",  1,	7,		0,		3},
	{"tRWCOV",  1,	3,		0,  		3},
	{"tCKE",    1,	7,    DDR_tCKE_ps,   DDR_tCKE_tck},
	/* 16 * 8 + 1 = 129 */
	{"tMINSR",  9, 129, DDR_tMINSR_ps, DDR_tMINSR_tck},
	{"tXP",	    1,	7,     DDR_tXP_ps,    DDR_tXP_tck},
	{"tMRD",    1,	4,    DDR_tMRD_ps,   DDR_tMRD_tck},
	{"tCTLUPD", 0,  0,              0,              0},
	{"tRTW",    1,  63,   DDR_tRTW_ps,   DDR_tRTW_tck},
	{"tRDLAT",  1,  63, DDR_tRDLAT_ps, DDR_tRDLAT_tck},
	{"tWDLAT",  1,  63, DDR_tWDLAT_ps, DDR_tWDLAT_tck},
	/* Delay time is tXSRD*4 (tCK), 63 * 4 = 252 */
	{"tXSRD",   4, 252,  DDR_tXSRD_ps,  DDR_tXSRD_tck},
	{"tFAW",    1,  31,   DDR_tFAW_ps,   DDR_tFAW_tck},
	{"tCFGW",   1,  63,		0,  		2},
	{"tCFGR",   1,  63,		0,		2},
	/* unit: tREFCT.para_ps is us */
	{"tREFCT", 16, 255 * 1024, DDR_tREFI * 1000,    0},

	/* phy timing */
	{"Phy_tDLLSRST", 1,   63, DDRP_PTR0_tDLLSRST,  	   		0},
	{"Phy_tDLLLOCK", 1, 4095, DDRP_PTR0_tDLLLOCK,  			0},
	{"Phy_tITMSRST", 1,   15,                  0, DDRP_PTR0_ITMSRST_8},
#if defined(CONFIG_SDRAM_DDR3)
	{"Phy_DINIT0",   1, 0x7ffff, DDRP_PTR1_DINIT0_DDR3, 0},
	{"Phy_DINIT1",   1,    0xff, DDRP_PTR1_DINIT1_DDR3, 0},
	{"Phy_DINIT2",   1, 0x1ffff, DDRP_PTR2_DINIT2_DDR3, 0},
	{"Phy_DINIT3",   1,   0x3ff, 0, DDRP_PTR2_DINIT3_DDR3},
#elif defined(CONFIG_SDRAM_MDDR)
	{"Phy_DINIT0",   1, 0x7ffff, DDRP_PTR1_DINIT0_MDDR, 0},
	{"Phy_DINIT1",   1,    0xff, DDRP_PTR1_DINIT1_MDDR, 0},
	{"Phy_DINIT2",   1, 0x1ffff, DDRP_PTR2_DINIT2_MDDR, 0},
	{"Phy_DINIT3",   1,   0x3ff, 0, DDRP_PTR2_DINIT3_MDDR},

#elif defined(CONFIG_SDRAM_LPDDR2)
#endif
	{"Phy_tMRD",     1,    4,     DDR_tMRD_ps,     DDR_tMRD_tck},
	{"Phy_tRTP",	 2,    6,     DDR_tRTP_ps,     DDR_tRTP_tck},
	{"Phy_tWTR",	 1,    6,     DDR_tWTR_ps,     DDR_tWTR_tck},
	{"Phy_tRP" ,	 2,   11,      DDR_tRP_ps,	DDR_tRP_tck},
	{"Phy_tRCD",	 2,   11,     DDR_tRCD_ps,     DDR_tRCD_tck},
	{"Phy_tRAS",	 2,   31,     DDR_tRAS_ps,     DDR_tRAS_tck},
	{"Phy_tRRD",	 1,    8,     DDR_tRRD_ps,     DDR_tRRD_tck},
	{"Phy_tRC" ,	 2,   42,      DDR_tRC_ps,	DDR_tRC_tck},
	{"Phy_tCCD",	 1,   63,     DDR_tCCD_ps,     DDR_tCCD_tck},
	{"Phy_tAOND",	 0,    3,		0,		  0},
	{"Phy_tRTW",	 0,    1,		0,		  0},
	{"Phy_tFAW",	 2,   31,     DDR_tFAW_ps,     DDR_tFAW_tck},
	{"Phy_tMOD",	12,   15,     DDR_tMOD_ps,     DDR_tMOD_tck},
	{"Phy_tRTODT",	 0,    1,		0,		  1},
	{"Phy_tRFC",	 3,  127,     DDR_tRFC_ps,     DDR_tRFC_tck},
#if defined(CONFIG_SDRAM_LPDDR2)
	{"Phy_tDQSCK"...},
	{"Phy_tDQSCKMAX"...},
#else
	{"Phy_tDQSCK",    0,   0,               0,                0},
	{"Phy_tDQSCKMAX", 0,   0,               0,                0},
#endif
	{"Phy_tXS",	 2, 1023,      DDR_tXS_ps,	DDR_tXS_tck},
	{"Phy_tXSDLL",	 2, 1023,   DDR_tXSDLL_ps,   DDR_tXSDLL_tck},
	{"Phy_tXP",	 2, 1023,      DDR_tXP_ps,	DDR_tXP_tck},
	{"Phy_tXPDLL",	 2, 1023,   DDR_tXPDLL_ps,   DDR_tXPDLL_tck},
	{"Phy_tCKE",	 1,    7,     DDR_tCKE_ps,     DDR_tCKE_tck},
	{"Phy_tDLLK",    2, 1023, DDR_tDLLLOCK_ps, DDR_tDLLLOCK_tck},

	{NULL, -1, -1, -1, -1}
};

/* Calculate Method */
static unsigned long gen_ddr_value(struct timing tim)
{
	unsigned long value = 0, tmp = 0;

	if (strcmp(tim.name, "Phy_DINIT1") == 0) {
		tim.para_ps = MAX(tim.para_ps, 5 * ps);
	}

	if ((tmp = tim.para_tck * ps) >= tim.para_ps) {
		value = tim.para_tck;
	} else {
		tmp = tim.para_ps;
		value = ((0 == tmp % ps)) ? (tmp / ps) : (tmp / ps + 1);
	}

	/* printf("%s: ps = %ld, tck = %ld, value = %ld, PS = %ld\n", tim.name, tim.para_ps, tim.para_tck, value, ps); */
	if (value < tim.min)
		value = tim.min;
	if (value > tim.max)
		value = tim.max;

	return value;
}
