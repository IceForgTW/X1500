
#include <x1000.h>

//------------------------------------------------------------------------
#define DMA_STRIDE_CPY_CHANNEL YUV_COPY_CHANNEL


//------------------------------------------------------------------------
#define PHYSADDR(x) (((unsigned int)x) & 0x1fffffff)
#define UNCACHE(x) ((unsigned int)x | 0xa0000000)
//static unsigned int g_des_space[3 * 8] __attribute__( (aligned(128)));
//static unsigned int *g_desc ;
//ÒªÇó32Bit¶ÔÆë

#if 0  //4755 has no MDMA (yqdeng)
void yuv_copy_nowait(unsigned int *tar,unsigned int *src,int *t_stride,int *s_stride,int *size,int *line)
{
	int i;

	while (!(INREG32(DMAC_DCCSR(DMA_STRIDE_CPY_CHANNEL)) & DMAC_DCCSR_CT) ); //error
	for(i = 0;i < 3;i++)
	{
		g_desc[8*i+1] = PHYSADDR(src[i]);
		g_desc[8*i+2] = PHYSADDR(tar[i]);
		g_desc[8*i+3] = (((unsigned int)&g_desc[8 * (i + 1)] & 0xff0)<<20 ) | ((*(line + i))<<16) | (*(size + i) );
		g_desc[8*i+4] = ((*(t_stride + i)) << 16) | (*(s_stride + i));
	}	
	OUTREG32(DMAC_DDA(DMA_STRIDE_CPY_CHANNEL),PHYSADDR(g_desc));
	OUTREG32(DMAC_DRSR(DMA_STRIDE_CPY_CHANNEL), DMAC_DRSR_RS_AUTO);
	CLRREG32(DMAC_DCCSR(DMA_STRIDE_CPY_CHANNEL),(DMAC_DCCSR_TT | DMAC_DCCSR_CT));
	OUTREG32(DMAC_DMADBSR, (1 << DMA_STRIDE_CPY_CHANNEL));//add
	SETREG32(DMAC_DCCSR(DMA_STRIDE_CPY_CHANNEL), DMAC_DCCSR_EN);
}

void yuv_copy_nowait_init()
{
	int i;
	g_desc=(unsigned int*)(((unsigned int)(g_des_space)) | 0xa0000000);
	for(i = 0;i < 3;i++)
	{
		g_desc[i * 8 + 0] = (DES_DAI | DES_SAI | DES_SP_32BIT | DES_DP_32BIT | DES_TSZ_32BIT | DES_STRIDE_EN | DES_LINK_EN);
		g_desc[i * 8 + 5] = DMAC_DRSR_RS_AUTO;
	}
	g_desc[(--i) * 8 + 0] &= ~(DES_LINK_EN);
			
	//CLRREG32(A_CPM_CLKGR, ( 1 << 12 ));
	SETREG32(DMAC_DMACR, DMAC_DMACR_DMAE);
	
	CLRREG32(DMAC_DMACR, (DMAC_DMACR_HLT | DMAC_DMACR_AR));//Ensure DMAC.AR = 0,DMAC.HLT = 0
	CLRREG32(DMAC_DCCSR(DMA_STRIDE_CPY_CHANNEL), DMAC_DCCSR_AR | DMAC_DCCSR_HLT | DMAC_DCCSR_TT); // Ensure DCSn.AR = 0, DCSn.HLT = 0, DCSn.TT = 0, DCSn.INV = 0

	OUTREG32(DMAC_DTCR(DMA_STRIDE_CPY_CHANNEL), 0);//DTCn = 0
	CLRREG32(DMAC_DCCSR(DMA_STRIDE_CPY_CHANNEL), DMAC_DCCSR_EN);
	CLRREG32(DMAC_DCCSR(DMA_STRIDE_CPY_CHANNEL), DMAC_DCCSR_NDES);	
	SETREG32(DMAC_DCCSR(DMA_STRIDE_CPY_CHANNEL), DMAC_DCCSR_DES8);
}

#define DMA_CPY_CHANNEL CODEC_COPY_CHANNEL
void dma_nowait_cpyinit()
{
	//CLRREG32(A_CPM_CLKGR, ( 1 << 12 ));
	SETREG32(MDMAC_DMACR,DMAC_DMACR_DMAE);

	OUTREG32(MDMAC_DMACKE,(1 << DMA_CPY_CHANNEL));//Open channel clock
	CLRREG32(MDMAC_DMACR, (DMAC_DMACR_HLT | DMAC_DMACR_AR));//Ensure DMAC.AR = 0,DMAC.HLT = 0
	CLRREG32(MDMAC_DCCSR(DMA_CPY_CHANNEL), DMAC_DCCSR_AR | DMAC_DCCSR_HLT | DMAC_DCCSR_TT); // Ensure DCSn.AR = 0, DCSn.HLT = 0, DCSn.TT = 0, DCSn.INV = 0
           
	OUTREG32(MDMAC_DTCR(DMA_CPY_CHANNEL), 0);//DTCn = 0
	CLRREG32(MDMAC_DCCSR(DMA_CPY_CHANNEL), DMAC_DCCSR_EN);
	SETREG32(MDMAC_DCCSR(DMA_CPY_CHANNEL), DMAC_DCCSR_NDES);
}

void dma_copy_nowait(void *tar,void *src,int size)
{
	int timeout = 0x1000000;
	
	while ((!(INREG32(MDMAC_DCCSR(DMA_CPY_CHANNEL)) & DMAC_DCCSR_TT)) && (timeout--));
	CLRREG32(MDMAC_DCCSR(DMA_CPY_CHANNEL), DMAC_DCCSR_EN);
	OUTREG32(MDMAC_DSAR(DMA_CPY_CHANNEL), PHYSADDR((unsigned long)src));
	OUTREG32(MDMAC_DTAR(DMA_CPY_CHANNEL), PHYSADDR((unsigned long)tar));
	OUTREG32(MDMAC_DTCR(DMA_CPY_CHANNEL), size / 32);	            	    
	OUTREG32(MDMAC_DRSR(DMA_CPY_CHANNEL), DMAC_DRSR_RS_AUTO);
	OUTREG32(MDMAC_DCMD(DMA_CPY_CHANNEL), (DMAC_DCMD_SAI| DMAC_DCMD_DAI | DMAC_DCMD_SWDH_32 | DMAC_DCMD_DWDH_32 | DMAC_DCMD_DS_32BYTE));
	CLRREG32(MDMAC_DCCSR(DMA_CPY_CHANNEL),(DMAC_DCCSR_TT));
	SETREG32(MDMAC_DCCSR(DMA_CPY_CHANNEL), DMAC_DCCSR_EN | DMAC_DCCSR_NDES);
}

void dma_copy_wait(void *tar,void *src,int size)
{
	int timeout = 0x1000000;
		
	CLRREG32(MDMAC_DCCSR(DMA_CPY_CHANNEL), DMAC_DCCSR_EN);
	OUTREG32(MDMAC_DSAR(DMA_CPY_CHANNEL), PHYSADDR((unsigned long)src));
	OUTREG32(MDMAC_DTAR(DMA_CPY_CHANNEL), PHYSADDR((unsigned long)tar));
	OUTREG32(MDMAC_DTCR(DMA_CPY_CHANNEL), size / 32);	            	    
	OUTREG32(MDMAC_DRSR(DMA_CPY_CHANNEL), DMAC_DRSR_RS_AUTO);
	OUTREG32(MDMAC_DCMD(DMA_CPY_CHANNEL), (DMAC_DCMD_SAI| DMAC_DCMD_DAI | DMAC_DCMD_SWDH_32 | DMAC_DCMD_DWDH_32 | DMAC_DCMD_DS_32BYTE));
	CLRREG32(MDMAC_DCCSR(DMA_CPY_CHANNEL),(DMAC_DCCSR_TT));
	SETREG32(MDMAC_DCCSR(DMA_CPY_CHANNEL), DMAC_DCCSR_EN | DMAC_DCCSR_NDES);
	while ((!(INREG32(MDMAC_DCCSR(DMA_CPY_CHANNEL)) & DMAC_DCCSR_TT)) && (timeout--));
	
}

#define 	DMA_NAND_COPY_CHANNEL			SSFDC_DMA_CHANNEL

void dma_nand_cpyinit()
{
	//CLRREG32(A_CPM_CLKGR, ( 1 << 12 ));
	SETREG32(DMAC_DMACR, DMAC_DMACR_DMAE);

	CLRREG32(DMAC_DMACR, (DMAC_DMACR_HLT | DMAC_DMACR_AR));//Ensure DMAC.AR = 0,DMAC.HLT = 0
	CLRREG32(DMAC_DCCSR(DMA_NAND_COPY_CHANNEL), DMAC_DCCSR_AR | DMAC_DCCSR_HLT | DMAC_DCCSR_TT); // Ensure DCSn.AR = 0, DCSn.HLT = 0, DCSn.TT = 0, DCSn.INV = 0

	OUTREG32(DMAC_DTCR(DMA_NAND_COPY_CHANNEL), 0);//DTCn = 0
	CLRREG32(DMAC_DCCSR(DMA_NAND_COPY_CHANNEL), DMAC_DCCSR_EN);
	SETREG32(DMAC_DCCSR(DMA_NAND_COPY_CHANNEL), DMAC_DCCSR_NDES);
}

void dma_nand_copy_wait(void *tar,void *src,int size)
{
	int timeout = 0x1000000;
	
	if(((unsigned int)src < 0xa0000000) && size)
		 dma_cache_wback_inv((unsigned long)src, size);
	
	if(((unsigned int)tar < 0xa0000000) && size)
		dma_cache_wback_inv((unsigned long)tar, size);
	
	CLRREG32(DMAC_DCCSR(DMA_NAND_COPY_CHANNEL), DMAC_DCCSR_EN);
	OUTREG32(DMAC_DSAR(DMA_NAND_COPY_CHANNEL), PHYSADDR((unsigned long)src));
	OUTREG32(DMAC_DTAR(DMA_NAND_COPY_CHANNEL), PHYSADDR((unsigned long)tar));
	OUTREG32(DMAC_DTCR(DMA_NAND_COPY_CHANNEL), size / 32);	            	    
	OUTREG32(DMAC_DRSR(DMA_NAND_COPY_CHANNEL), DMAC_DRSR_RS_AUTO);
	OUTREG32(DMAC_DCMD(DMA_NAND_COPY_CHANNEL), (DMAC_DCMD_SAI| DMAC_DCMD_DAI | DMAC_DCMD_SWDH_32 | DMAC_DCMD_DWDH_32 | DMAC_DCMD_DS_32BYTE));
	CLRREG32(DMAC_DCCSR(DMA_NAND_COPY_CHANNEL),(DMAC_DCCSR_TT));
	SETREG32(DMAC_DCCSR(DMA_NAND_COPY_CHANNEL), DMAC_DCCSR_EN | DMAC_DCCSR_NDES);
	while ((!(INREG32(DMAC_DCCSR(DMA_NAND_COPY_CHANNEL)) & DMAC_DCCSR_TT)) && (timeout--));
}

void dma_nand_set_wait(void *tar,unsigned char src,unsigned int size)
{
	unsigned int setdata[16];
	unsigned int *ptemp;
	ptemp = (unsigned int *)UNCACHE(((unsigned int)(&setdata)+ 31)& (~31));
	*ptemp = (unsigned int) ((src << 24) | (src << 16) | (src << 8) | src);
	
	if(((unsigned int)tar < 0xa0000000) && size)
		dma_cache_wback_inv((unsigned long)tar, size);
		
	CLRREG32(DMAC_DCCSR(DMA_NAND_COPY_CHANNEL), DMAC_DCCSR_EN);
	OUTREG32(DMAC_DSAR(DMA_NAND_COPY_CHANNEL), PHYSADDR((unsigned long)ptemp));
	OUTREG32(DMAC_DTAR(DMA_NAND_COPY_CHANNEL), PHYSADDR((unsigned long)tar));
	OUTREG32(DMAC_DTCR(DMA_NAND_COPY_CHANNEL), size / 32);	            	    
	OUTREG32(DMAC_DRSR(DMA_NAND_COPY_CHANNEL), DMAC_DRSR_RS_AUTO);
	OUTREG32(DMAC_DCMD(DMA_NAND_COPY_CHANNEL),(DMAC_DCMD_DAI | DMAC_DCMD_SWDH_32 | DMAC_DCMD_DWDH_32| DMAC_DCMD_DS_32BYTE));
	CLRREG32(DMAC_DCCSR(DMA_NAND_COPY_CHANNEL),(DMAC_DCCSR_TT));
	SETREG32(DMAC_DCCSR(DMA_NAND_COPY_CHANNEL), DMAC_DCCSR_EN | DMAC_DCCSR_NDES);
	while (!(INREG32(DMAC_DCCSR(DMA_NAND_COPY_CHANNEL)) & DMAC_DCCSR_TT));
}
#else
void yuv_copy_nowait(unsigned int *tar,unsigned int *src,int *t_stride,int *s_stride,int *size,int *line)
{
}
void yuv_copy_nowait_init()
{
}
void dma_nowait_cpyinit()
{
}
void dma_copy_nowait(void *tar,void *src,int size)
{
}
void dma_copy_wait(void *tar,void *src,int size)
{
}
void dma_nand_cpyinit()
{
}
void dma_nand_copy_wait(void *tar,void *src,int size)
{
}
void dma_nand_set_wait(void *tar,unsigned char src,unsigned int size)
{
}
#endif
