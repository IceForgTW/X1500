#ifndef __DM_H
#define __DM_H
#if (DM==1)
#if 1
typedef struct __tag_pll_opt
{
	unsigned int cpuclock;   // cpu clock
	int cdiv;                // cpu spec cdiv
	int mdiv;                // mem div
	int h1div;
	int pdiv;
	int h0div;
	
}PLL_OPT,*PPLL_OPT;
typedef int (*PCONVERT)(unsigned int div);
typedef int (*PPERCONVERT)(PPLL_OPT pllopt);

struct dm_jz4740_t{
	unsigned char *name;
	int type;
	PCONVERT convert;
	PPERCONVERT preconvert;
	int result;
	struct dm_jz4740_t *next;
};
#endif
struct jz_pll_info {
     unsigned int pll_sel;
     unsigned int pll_m;
     unsigned int pll_n;
     unsigned int pll_od;	
     unsigned int pll_bs;
     unsigned int ahb0_div;
};

#endif
#endif	/* __CONFIG_H */
