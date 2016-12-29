
/*
 *
 * dwc_otg_core_init(unsigned char dma_enable)
 * 选1是dma mode
 * 选0是slave_mode
 * 注意：建议不要用slave_mode,该模式下代码还有待完善。。。
 *
 *
 * */


#include <x1000.h>
#include <mipsregs.h>
#include <usb.h>
#include <os_api.h>
#include <udcbus.h>
#include "dwc.h"
#include "compat.h"
#include <em3000_cfg.h>

#include <MemMgr.h>
#include <cache.h>
#include <time-tcu.h>
#undef printf
#include <stdio.h>
#include <string.h>
#undef memalign
#undef free
#define memalign(x,y) alignAlloc(x,y)
#define free(x) deAlloc(x)

#ifdef TRACE_DWC
#define TRACE(fmt, args...)	do { printf(fmt ,##args) }while(0)
#else
#define TRACE(fmt, args...)	do {}while(0)
#endif

#define UdcID (('U' << 24) | ('D' << 16) | ('C' << 16) | (':' << 16))
#define IS_SLAVE_MODE 	0
#define IS_INTERN_DMA	2
#define IS_EXTERN_DMA	1

static unsigned char usbTest = 0;

/*  */
static unsigned int Dwc_Task_Stack[TASK_DWC_STK_SIZE];

/* */
extern void enable_device(unsigned int handle);
extern void disable_device(unsigned int handle);
extern void init_endpoint_suppost(unsigned int handle,unsigned char *ep, USB_ENDPOINT_TYPE ep_type,
		unsigned short *ep_max_pkg, unsigned char *buf);
extern void set_address(unsigned int handle,unsigned short value);
extern void set_config(unsigned int handle);
extern void start_transfer(unsigned int handle,unsigned char ep,unsigned char *buf,unsigned int len);
extern void jz47xx_usb_set_device_only_mode(void);
extern void jz47xx_usb_phy_init(void);
extern void jz47xx_usb_phy_switch(unsigned char is_on);
extern void usb_cpm_init(void);


/*  */
static void dwc_otg_device_init(void);
static void dwc_otg_core_init(unsigned char dma_enable);

static dwc_ep ep[] = {
		{0, EP_SETUP, 0, 0, DWC_OTG_EP_TYPE_CONTROL, 64, 0, NULL, 0, 0, 0, 0}, //control
		{0x81, EP_IDLE, 0, 0, DWC_OTG_EP_TYPE_BULK, 64, 0, NULL, 0, 0, 0, 0}, //in
		{0x02, EP_IDLE, 1, 0, DWC_OTG_EP_TYPE_BULK, 64, 0, NULL, 0, 0, 0, 0},	//out
		{0x83, EP_IDLE, 0, 0, DWC_OTG_EP_TYPE_INTR, 64, 0, NULL, 0, 0, 0, 0} //in
};
static int g_ep_num;

static dwc_cfg_if dwc;

static unsigned int setup_packet[5] = {0, 0, 0, 0, 0};
// static unsigned int * Bulk_Buf;
static unsigned char udc_Buf[512*8] __attribute__ ((aligned (32)));

//volatile int req_num = 0;
extern int req_num;
#define MAX_PKT_CNT	1023

#ifdef TRACE_DWC
void dump_global_dwcreg(void)
{
	unsigned int reg;
	printf("Global CSR > \n");
	printf("GOTGCTL: %x\n", REG_GOTG_CTL);
	printf("GAHBCFG: %x\n", REG_GAHB_CFG);
	printf("GUSBCFG: %x\n", REG_GUSB_CFG);
	printf("GRSTCTL: %x\n", REG_GRST_CTL);
	printf("GINTSTS: %x\n", REG_GINT_STS);
	printf("GINTMSK: %x\n", REG_GINT_MASK);
	printf("GHWCFG1: %x\n", REG_GHW_CFG1);
	printf("GHWCFG2: %x\n", REG_GHW_CFG2);
	printf("GHWCFG3: %x\n", REG_GHW_CFG3);
	printf("GHWCFG4: %x\n", REG_GHW_CFG4);

	printf("Device CSR > \n");
	printf("DCFG: %x\n", REG_OTG_DCFG);
	printf("DCTL: %x\n", REG_OTG_DCTL);
	printf("DSTS: %x\n", REG_OTG_DSTS);
	printf("DAINTMSK: %x\n", REG_DAINT_MASK);
	printf("DOEPMSK: %x\n", REG_DOEP_MASK);
	printf("DIEPMSK: %x\n", REG_DOEP_MASK);
}
#endif


void *GetEp(int x)
{
	return (&ep[x]);
}

void SetEp(PIPE *pep, int num)
{
    int i;

    for(i=0;i<num;i++, pep++)
    {
        ep[i].num = pep->ep;
        ep[i].type = pep->ep_type;
        ep[i].maxpacket = pep->max_pkg;
    }
    g_ep_num = num;
}

static inline int DEP_EP_MAXPKT(int num)
{
    return ep[num].maxpacket;
}


static int dwc_get_utmi_width(void)
{
	return (REG_GHW_CFG4 >> 14) & 0x3;
}

static void dwc_otg_select_phy_width(void)
{
	// fixme full speed use 16bit phy can use trdtime_6?
	if (((REG_GHW_CFG2 >> 6) & 0x3) == 1) {
		/* UTMI+ WIDTH choose diff trdtiming */
		if (dwc_get_utmi_width() == 0) {
			TRACE("8BIT UTMI+.\n");
			REG_GUSB_CFG &= ~(1 << 3);
			REG_GUSB_CFG |= USBCFG_TRDTIME_9;
			REG_CPM_USBPCR1 &= ~(3 << 18);
		} else if (dwc_get_utmi_width() == 1) {
			TRACE("16BIT UTMI+.\n");
			REG_GUSB_CFG |= (1 << 3);
			REG_GUSB_CFG |= USBCFG_TRDTIME_6;
			REG_CPM_USBPCR1 |= (3 << 18);
		} else if (dwc_get_utmi_width() == 2) {
			TRACE("8BIT or 16BIT UTMI+.\n");
			REG_GUSB_CFG &= ~USBCFG_TRDTIME_MASK;

			if (UTMI_PHY_WIDTH == 8) {
				/* 8bit */
				REG_GUSB_CFG &= ~(1 << 3);
				REG_GUSB_CFG |= USBCFG_TRDTIME_9;
				REG_CPM_USBPCR1 &= ~(3 << 18);
			} else  {
				/* 16bit */
				REG_GUSB_CFG |= (1 << 3);
				REG_GUSB_CFG |= USBCFG_TRDTIME_6;
				REG_CPM_USBPCR1 |= (3 << 18);
			}		}
	}  else
		printf("Unkonwn PHY TYPE?.\n");
}


static void dwc_otg_write_packet( int epnum)
{
	int i;
	unsigned int dwords;
	unsigned int num_in_ep = 0;
	unsigned int byte_count;
	dwc_ep *pep = dwc.dep[num_in_ep + epnum];
	byte_count = pep->xfer_len - pep->xfer_count;

	if (byte_count > DEP_EP_MAXPKT(epnum))
		byte_count = DEP_EP_MAXPKT(epnum);

	dwords = (byte_count + 3) / 4;

	for (i = 0; i < dwords; i++) {
		REG_EP_FIFO(epnum) = REG32((unsigned int *)(pep->xfer_buff)+i);
	}

	pep->xfer_count += byte_count;
	pep->xfer_buff += byte_count;
}

void dwc_read_ep_packet(unsigned int epnum, u32 count)
{
	int i, num_out_ep = 16;
	int dwords = (count + 3) / 4;
	dwc_ep *pep = dwc.dep[num_out_ep + epnum];
	for (i = 0; i < dwords; i++)
		REG32((unsigned int *)(pep->xfer_buff + pep->xfer_count / 4) + i) = REG_EP_FIFO(epnum);
	pep->xfer_count += count;
}


void dwc_write_ep_packet(int epnum)
{
	unsigned int num_in_ep = 0;
	unsigned int xfersize, finish, insize;
	unsigned int dwords;
	unsigned int txstatus = REG_DIEP_TXFSTS(epnum);
	dwc_ep *pep = dwc.dep[num_in_ep + epnum];

	insize = pep->xfer_len;
	if (pep->xfer_len > DEP_EP_MAXPKT(epnum))
		xfersize = DEP_EP_MAXPKT(epnum);
	else
		xfersize = pep->xfer_len;

	dwords = (xfersize + 3) / 4;
	//	printf("txstatus (%x) dwords (%x) length (%x) xfer_count (%x) \n", txstatus, dwords,  pep->xfer_len, pep->xfer_count);
	while ((txstatus > dwords) && (pep->xfer_len > 0) && (pep->xfer_count < pep->xfer_len) )
	{
		dwc_otg_write_packet(epnum);
		xfersize = pep->xfer_len - pep->xfer_count;
		if (xfersize > DEP_EP_MAXPKT(epnum))
			xfersize = DEP_EP_MAXPKT(epnum);
		dwords = (xfersize + 3) / 4;
		txstatus = REG_DIEP_TXFSTS(epnum);
	}
	finish = pep->xfer_count;

	if (insize > finish) {
		u32 intr = REG_DIEP_INT(epnum);
		while(!(intr & DEP_TXFIFO_EMPTY)) {
			intr = REG_DIEP_INT(epnum);
		}
		HW_SendPKT(epnum, pep->xfer_buff, insize - finish);
	}
	return;
}


void dwc_handle_ep_data_in_phase(dwc_cfg_if *pdwc, int epnum)
{
	unsigned int pktcnt, xfersize, num_in_ep = 0;
	unsigned int dma_addr, dma_len;

	dwc_ep *pep = pdwc->dep[num_in_ep + epnum];
	xfersize = pep->xfer_len;
	pktcnt = (xfersize + DEP_EP_MAXPKT(epnum) - 1) / DEP_EP_MAXPKT(epnum);
	if (pktcnt > 1023) {
		printf("WARNING...\n");
		while(1);
	}
// printf("pkt: %d\n", pktcnt);

	if (epnum == 0)
	{
		REG_DIEP_SIZE(epnum) &= ~(0x1fffff);
		REG_DIEP_SIZE(epnum) |= (pktcnt << 19) | xfersize;
	}
	else
	{
		REG_DIEP_SIZE(epnum) &= ~(0x1fffffff);
		REG_DIEP_SIZE(epnum) |= (pktcnt << 19) | xfersize;
	}

	if (dwc.is_dma != IS_SLAVE_MODE)
	{
		dma_addr = (u32)(pep->xfer_buff);
		dma_len = (((pep->xfer_len + 7) >> 3) << 3);
		dma_cache_wback_inv(dma_addr, dma_len);

		REG_DIEP_DMA(epnum)	= PHYS(pep->xfer_buff);
		REG_DIEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
	}
	else
	{
		REG_DIEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
		REG_DIEP_EMPMSK |= (1 << epnum);
	}

	return ;
}


void dwc_handle_ep_status_in_phase(dwc_cfg_if *pdwc, int epnum)
{
// 	pep->xfer_len = 0;
// 	pep->xfer_count = 0;
	if (epnum == 0)
	{
		REG_DIEP_SIZE(epnum) &= ~(0x1fffff);
		REG_DIEP_SIZE(epnum) |= DOEPSIZE0_PKTCNT_BIT | (0); // pktcnt->1 xfersize->0
	}
	else
	{
		REG_DIEP_SIZE(epnum) &= ~(0x1FFFFFFF);
		REG_DIEP_SIZE(epnum) |= DOEPSIZE0_PKTCNT_BIT | (0); // pktcnt->1 xfersize->0
	}

	if (dwc.is_dma == IS_INTERN_DMA)
	{
// 		pep->xfer_buff = (void *)0xFFFFFFFF;
		REG_DIEP_DMA(epnum)	= PHYS(0xFFFFFFFF);
		REG_DIEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
	}
	else
	{
		REG_DIEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
	}
	return ;
}



void dwc_handle_ep_data_out_phase(unsigned int epnum)
{
	unsigned int dma_addr, dma_len;
	unsigned int num_out_ep = 16;
	unsigned int pktcnt;
	dwc_ep *pep = dwc.dep[num_out_ep + epnum];

	if (epnum == 0)
	{
		if (dwc.is_dma == IS_INTERN_DMA)
		{
			dma_len = 16;
			dma_addr = (unsigned int)(pep->ctrl_req_addr);
			dma_cache_wback_inv(dma_addr, dma_len);
			REG_DOEP_DMA(epnum) = PHYS(pep->ctrl_req_addr);
		}
		REG_DOEP_SIZE(epnum) = DOEPSIZE0_SUPCNT_3 | DOEPSIZE0_PKTCNT_BIT | (24);
		REG_DOEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
	}
	else
	{
		if (pep->xfer_len > 0)
		{
			if (pep->xfer_len > MAX_PKT_CNT * DEP_EP_MAXPKT(epnum))
				pep->xfer_len = MAX_PKT_CNT * DEP_EP_MAXPKT(epnum);

			pktcnt = (pep->xfer_len + DEP_EP_MAXPKT(epnum) - 1) / DEP_EP_MAXPKT(epnum);
			if (pktcnt > 1023) {
				printf("WARNING...\n");
				while(1);
			}
			REG_DOEP_SIZE(epnum) &= ~(0x1fffffff);
			REG_DOEP_SIZE(epnum) |= (pktcnt << 19) | (pep->xfer_len);
		}

		if (dwc.is_dma == IS_INTERN_DMA)
		{
			dma_len = (((pep->xfer_len + 7) >> 3) << 3);
			dma_addr = (unsigned int)(pep->xfer_buff);
			dma_cache_wback_inv(dma_addr, dma_len);
			REG_DOEP_DMA(epnum) = PHYS(pep->xfer_buff);
		}
		REG_DOEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
	}
}


static void QueueBuffer(DWC_Send_Buf *pBuffer, unsigned int epNum)
{
	DWC_Send_Buf *tmpBuffer;	
	unsigned int sr;
	//printf("QueueBuffer\n");

	sr = spin_lock_irqsave();
	
    if(dwc.tx_list[epNum] == NULL)
        dwc.tx_list[epNum] = pBuffer;
    else
    {
        tmpBuffer = dwc.tx_list[epNum];
		
        while(tmpBuffer->pNext != NULL)
            tmpBuffer = tmpBuffer->pNext;
        tmpBuffer->pNext = pBuffer;
    }
    //printf("A");
    req_num++;
	spin_unlock_irqrestore(sr);

}

static DWC_Send_Buf * GetXmtBuffer(unsigned int epNum)
{
    DWC_Send_Buf *pBuffer = dwc.tx_list[epNum];
	if(pBuffer != NULL)
	{
		dwc.tx_list[epNum] = pBuffer->pNext;
		pBuffer->pNext = NULL;
	}
 /*req_num--;  	
    if(req_num < 1) 
		req_num = 0;
printf("s");*/
    return pBuffer;
}

static void FreeAllBuff(void)
{
	int i;
	DWC_Send_Buf *pBuffer;
	dwc_ep *pep;


	for (i=1; i<16; i++)
	{
		pep = dwc.dep[i];
		//printf("\nin ep: %d,  pep: %x\n", i, pep);
	
		if (pep!= NULL)
		{
			//printf("ep: %d, xfer: %x, call: %x\n", i, pep->xfer_buff, pep->CallBackParameter);

			if(pep->CallBackParameter != NULL)
			{
				
				printf("ep: %d, CallBackParameter: %x, buf: %x\n", i, pep->CallBackParameter, pep->CallBackBuffer);

				if (pep->CallBackBuffer)
					deAlloc((MEM_ULONG)pep->CallBackBuffer);
				deAlloc((MEM_ULONG)pep->CallBackParameter);
				pep->CallBackParameter = NULL;
			}
		}

		//printf("\nep: %d, list: %x\n", i,dwc.tx_list[i]);
		if(dwc.tx_list[i] != NULL)
		{
			pBuffer = GetXmtBuffer(i);
			
			while(pBuffer)
			{
				printf("ep: %d, GetXmtBuffer: %x\n", i, pBuffer);
				deAlloc((MEM_ULONG)pBuffer->buf);
				deAlloc((MEM_ULONG)pBuffer);
				pBuffer = GetXmtBuffer(i);
			}
		}
		
	}

}


void udc_Tx(unsigned int epNum)
{
	DWC_Send_Buf *pbuf;
	dwc_ep *pep = dwc.dep[0 + epNum];

	pbuf = GetXmtBuffer(epNum);
	if (pbuf == NULL)
		return;

	pep->CallBackParameter = pbuf;
	pep->CallBackBuffer = pbuf->buf;

	HW_SendPKT(epNum, pbuf->buf, pbuf->nSize);
}

void udc_Send(unsigned int epNum,  const unsigned char *buf, int nSize)
{
	unsigned int num_in_ep = 0;
	dwc_ep *pep = dwc.dep[num_in_ep + epNum];
	DWC_Send_Buf *pBuf;

	if (pep->type == DWC_OTG_EP_TYPE_CONTROL)
		return;

	pBuf = (DWC_Send_Buf *)alignAlloc(4, sizeof(DWC_Send_Buf));
	if (pBuf == NULL)
	{
		printf("usb write malloc buffer error\n");
		return;
	}
	pBuf->nSize = nSize;
	pBuf->pNext = NULL;

	if (nSize>0)
	{
		pBuf->buf = (unsigned char *)alignAlloc(4, nSize);
		
		if (pBuf->buf == NULL)
		{
			printf("usb write malloc buffer error\n");
			deAlloc((MEM_ULONG)pBuf);
			return;
		}
		
// 		printf("alloc: %08x\n", pBuf->buf);
		memcpy(pBuf->buf, buf, nSize);
	}
	else
		pBuf->buf = NULL;
// 	printf("send 3: %x, %x\n", dwc.tx_list[epNum],pep->CallBackParameter);

	if ((dwc.tx_list[epNum] == NULL) && (pep->CallBackParameter == NULL))
	{
// 		printf("send 4\n");
		QueueBuffer(pBuf, epNum);
		udc_Tx(epNum);
	}
	else
		QueueBuffer(pBuf, epNum);

}


void HW_SendPKT(unsigned int epnum,  const u8 *buf, int size)
{
	unsigned int num_in_ep = 0;
	dwc_ep *pep = dwc.dep[num_in_ep + epnum];

	pep->xfer_len = size; 			/* number of bytes to transfer */
	pep->xfer_count = 0;  			/* number of bytes transfered */
	pep->xfer_buff = (u8 *)buf; 	/* pointer to transfer buffer */
	pep->EpNumTotalBytes = size;
	pep->EpBytesProcessed = 0;

	if (pep->xfer_len > MAX_PKT_CNT * DEP_EP_MAXPKT(epnum))
		pep->xfer_len = MAX_PKT_CNT * DEP_EP_MAXPKT(epnum);

	pep->xfer_count = 0;

    //printf("HW_SendPKT:%d, size: %d, tpye: %d, state: %d\n", epnum, size, pep->type, pep->ep_state);

 	switch (pep->type)
	{
	case DWC_OTG_EP_TYPE_CONTROL:

		if (pep->xfer_len > 0)
			pep->ep_state = EP_DATA;
		else
			pep->ep_state = EP_STATUS;

		/* 2 Stage */
		if (pep->ep_state == EP_STATUS && pep->xfer_len == 0) /*EP_SETUP 0   EP_DATA 1  EP_STATUS  	2*/
		{
			/* prepare for next out status packet */
			dwc_handle_ep_data_out_phase(epnum);

			/* in status packet */
			dwc_handle_ep_status_in_phase(&dwc, epnum);

			return ;
		}
		/* 3 Stage */
		if (pep->ep_state == EP_DATA)
		{
			/* enable in data phase */
			dwc_handle_ep_data_in_phase(&dwc, epnum);
		}

		break;

	case DWC_OTG_EP_TYPE_BULK:
    case DWC_OTG_EP_TYPE_INTR:    
		if (pep->ep_state == EP_IDLE || pep->ep_state == EP_TRANSFERED)
		{
			pep->ep_state = EP_TRANSFERING;
			if (pep->xfer_len == 0)
			{
				dwc_handle_ep_status_in_phase(&dwc, epnum);
				return ;
			}
			dwc_handle_ep_data_in_phase(&dwc, epnum);
		}
		break;
	}

}



void HW_GetPKT(unsigned int epnum,  const unsigned char *buf, int size)
{
	unsigned int num_out_ep = 16;
	dwc_ep *pep = dwc.dep[num_out_ep + epnum];
    
//     printf("HW_GetPKT:%d, %d, size: %d\n", epnum, dwc.is_dma, size);
    
	if (dwc.is_dma == IS_INTERN_DMA)
	{
// 		printf("HW_GetPKT:%x %x \n", pep->xfer_buff, UNCACHED(pep->xfer_buff));
        
		unsigned char *ptr = (unsigned char *)UNCACHED(pep->xfer_buff);

		memcpy((unsigned char*)buf, (unsigned char *)ptr, size);
	}
	else
	{
		memcpy((unsigned char*)buf, (unsigned char *)pep->xfer_buff, size);
	}
}



static void dwc_otg_flush_rx_fifo(void)
{
	;
}

static void dwc_otg_flush_tx_fifo(unsigned char txf_num)
{
	unsigned int gintsts;
	unsigned int grstctl;

	gintsts = REG_GINT_STS;
	/* Step1: Check that GINTSTS.GinNakEff=0 if this
	 * bit is cleared then set Dctl.SGNPInNak = 1.
	 * Nak effective interrupt = H indicating the core
	 * is not reading from fifo*/
	if ((gintsts & GINTSTS_GINNAK_EFF))
	{
		REG_OTG_DCTL |= DCTL_SGNPINNAK;

		/* Step2: wait for GINTSTS.GINNakEff=1,which indicates
		 * the NAK setting has taken effect to all IN endpoints */
		while(!(REG_GINT_STS & GINTSTS_GINNAK_EFF))
			udelay(1);
	}

	/* Step3: wait for ahb master idle state */
	while (!(REG_GRST_CTL & RSTCTL_AHB_IDLE))
		udelay(1);

	/* Step4: Check that GrstCtl.TxFFlsh=0, if it is 0, then write
	 * the TxFIFO number you want to flush to GrstCTL.TxFNum*/
	grstctl = REG_GRST_CTL;
	if (!(grstctl & RSTCTL_TXFIFO_FLUSH))
	{
		REG_GRST_CTL |= (txf_num << 6);
	}

	/* Step5: Set GRSTCTL.TxFFlsh=1 and wait for it to clear */
	REG_GRST_CTL |= RSTCTL_TXFIFO_FLUSH;

	while (REG_GRST_CTL & RSTCTL_TXFIFO_FLUSH)
	{
		udelay(1);
	}

	/* Step6: Set the DCTL.GCNPinNak bit */
	REG_OTG_DCTL |= DCTL_CLR_GNPINNAK;
}

int dwc_set_address(unsigned arg)
{
	REG_OTG_DCFG &= ~DCFG_DEV_ADDR_MASK;
	REG_OTG_DCFG |= arg << DCFG_DEV_ADDR_BIT;

	HW_SendPKT(0, NULL, 0);
	return 0;
}

int dwc_configure_epn(unsigned int type)
{
	unsigned int xfersize;
	unsigned int dma_addr, dma_len, pktcnt;
	unsigned int num_out_ep = 16;
	dwc_ep *pep;
    int epnum;

    for(epnum=1; epnum<g_ep_num; epnum++)
    {
        if((ep[epnum].num & 0x80) == 0)  // out end-point
        {
            xfersize = DEP_EP_MAXPKT(epnum);
            pktcnt = xfersize/DEP_EP_MAXPKT(epnum);
            pep = dwc.dep[num_out_ep + epnum];

            REG_DOEP_CTL(epnum) &= ~DEP_PKTSIZE_MASK;
        	REG_DOEP_CTL(epnum) &= ~DEP_TYPE_MASK;
            
            REG_DOEP_CTL(epnum) |= ep[epnum].maxpacket;
        	REG_DOEP_CTL(epnum) |= (USB_ACTIVE_EP | (ep[epnum].type << 18));
            REG_DOEP_CTL(epnum) |= (1 << 28);

            REG_DAINT_MASK |= (1 << (epnum + num_out_ep));
        	pep->xfer_len = xfersize;
        	pep->xfer_count = 0;
        	pep->xfer_buff = &udc_Buf[0];
        	pep->CallBackParameter = NULL;
            
        	/* Program the DOEPSIZn register for the transfer size and
        	 * corresponding packet count */

        	REG_DOEP_SIZE(epnum) &= ~(0x1fffffff);
        	REG_DOEP_SIZE(epnum) = (pktcnt << 19) | xfersize;
        	if (dwc.is_dma == IS_INTERN_DMA)
        	{
        		dma_addr = (unsigned long)(pep->xfer_buff);
        		dma_len = (((xfersize + 7) >> 3) << 3);//pep->xfer_len;
        		dma_cache_wback_inv(dma_addr, dma_len);
        		/* Additionally, in DMA mode, program the DOEPDMAn register */
        		REG_DOEP_DMA(epnum) = PHYS(pep->xfer_buff);
        	}
        	/* Program the DOEPCTLn Register with endpoint charateristics,
        	 * and set the Endpoint Enable and Clear NAK bit */
        	REG_DOEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
        }
        else                         // in end-point
        {
        	REG_DIEP_CTL(epnum) &= ~DEP_PKTSIZE_MASK;
            REG_DIEP_CTL(epnum) &= ~DEP_TYPE_MASK;

            REG_DAINT_MASK |= (1 << epnum);
            REG_DIEP_CTL(epnum) |= ep[epnum].maxpacket;
            REG_DIEP_CTL(epnum) |= (USB_ACTIVE_EP | (ep[epnum].type << 18));
            REG_DIEP_CTL(epnum) |= (1 << 28);
            REG_DIEP_CTL(epnum) |= (1 << 22);
        }
    }
	//printf("%s %d xfer_buff: %x, %x, dma: %d, DAINT=%x\n", __FUNCTION__, __LINE__, pep->xfer_buff, PHYS(pep->xfer_buff), dwc.is_dma, REG_DAINT_MASK);

	//	dwc2_dump_ep_regs(1, __FUNCTION__, __LINE__);
	HW_SendPKT(0, NULL, 0);
	return 0;
}



static void dwc_calculate_fifo_size(void)
{
	/*
	 * TODO: we are use "Dedicated FIFO Mode with No Thresholding"
	 *  if need thresholding, the calculation algorithm may need change
	 */

	/**
	 * 3.2.1.1 FIFO SPRAM(Single-Port RAM) mapping:
	 *
	 * 1. One common RxFIFO, used in Host and Device modes
	 * 2. One common Periodic TxFIFO, used in Host mode
	 * 3. Separate IN endpoint transmit FIFO for each Device mode IN endpoints in Dedicated Transmit FIFO
	 *    operation (OTG_EN_DED_TX_FIFO = 1)
	 * 4. The FIFO SPRAM is also used for storing some register values to save gates. In Scatter/Gather DMA
	 *    mode, four SPRAM locations (four 35-bit words) are reserved for this. In DMA and Slave modes
	 *    (non-Scatter/Gather mode), one SPRAM location (one 35-bit word) is used for storing the DMA address.
	 *
	 * NOTE: when the device is operating in Scatter/Gather mode, then the last
	 *       locations of the SPRAM store the Base Descriptor address, Current
	 *       Descriptor address, Current Buffer address and status quadlet
	 *       information for each endpoint direction (4 locations per Endpoint).
	 *       If an endpoint is bidirectional, then 4 locations will be used for IN,
	 *       and another 4 for OUT
	 * 3.2.4.4 Endpoint Information Controller
	 *       The last locations in the SPRAM are used to hold register values.
	 *    Device Buffer DMA Mode:
	 *       one location per endpoint direction is used in SPRAM to store the
	 *       DIEPDMA and DOEPDMA value. The application writes data and then reads
	 *       it from the same location
	 *       For example, if there are ten bidirectional endpoints, then the last
	 *       20 SPRAM locations are reserved for storing the DMA address for IN
	 *       and OUT endpoints
	 *   Scatter/Gather DMA Mode:
	 *       Four locations per endpoint direction are used in SPRAM to store the
	 *       Base Descriptor address, Current Descriptor address, Current Buffer
	 *       Pointer and the Status Quadlet.
	 *       The application writes data to the base descriptor address.
	 *       When the application reads the location where it wrote the base
	 *       descriptor address, it receives the current descriptor address.
	 *       For example, if there are ten bidirectional endpoints, then the last 80
	 *      locations are reserved for storing these values.
	 *
	 * Figure 3-13
	 *  ________________________
	 *  |                       |
	 *  | DI/OEPDMAn Register   | Depends on the value of OTG_NUM_EPS
	 *  | and Descriptor Status | and OTG_EP_DIRn, see not above
	 *  |      values           |
	 *  -------------------------
	 *  |   TxFIFO #n Packets   |  DIEPTXFn
	 *  -------------------------
	 *  |                       |
	 *  |   ................    |
	 *  |                       |
	 *  -------------------------
	 *  |  TxFIFO #1 Packets    | DIEPTXF1
	 *  -------------------------
	 *  |  TxFIFO #0 Packets    |
	 *  |( up to3 SETUP Packets)| GNPTXFSIZ
	 *  ------------------------
	 *  |                       |
	 *  |     Rx Packets        |  GRXFSIZ
	 *  |                       |
	 *  -------------------------  Address = 0, Rx starting address fixed to 0
	 *
	 */

	/**
	 * Rx FIFO Allocation (rx_fifo_size)
	 *
	 * RAM for SETUP Packets: 4 * n + 6 locations must be Reserved in the receive FIFO to receive up to
	 * n SETUP packets on control endpoints, where n is the number of control endpoints the device
	 * core supports.
	 *
	 * One location for Global OUT NAK
	 *
	 * Status information is written to the FIFO along with each received packet. Therefore, a minimum
	 * space of (Largest Packet Size / 4) + 1 must be allotted to receive packets. If a high-bandwidth
	 * endpoint is enabled, or multiple isochronous endpoints are enabled, then at least two (Largest
	 * Packet Size / 4) + 1 spaces must be allotted to receive back-to-back packets. Typically, two
	 * (Largest Packet Size / 4) + 1 spaces are recommended so that when the previous packet is being
	 * transferred to AHB, the USB can receive the subsequent packet. If AHB latency is high, you must
	 * allocate enough space to receive multiple packets. This is critical to prevent dropping of any
	 * isochronous packets.
	 *
	 * Typically, one location for each OUT endpoint is recommended.
	 *
	 * one location for eatch endpoint for EPDisable is required
	 */

	/**
	 * Tx FIFO Allocation (tx_fifo_size[n])
	 *
	 * The minimum RAM space required for each IN Endpoint Transmit FIFO is the maximum packet size
	 * for that particular IN endpoint.
	 *
	 * More space allocated in the transmit IN Endpoint FIFO results in a better performance on the USB
	 *and can hide latencies on the AHB.
	 */

	unsigned int rx_fifo_size, i;
	unsigned int np_txfifo_size = 0;
	unsigned int tx_fifo_size=0;

	/* Step1: Recevice FIFO Size Register (GRXFSIZ) */
	rx_fifo_size = (4 * 1 + 6) + (2) * (1024 / 4 + 1) +
			(2 * dwc.hwcfg2.b.num_dev_ep) + 1;

	REG_GRXFIFO_SIZE = rx_fifo_size;

	/* Step2: Program device in ep transmit fifo0 size register (GNPTXFSIZ) */
	np_txfifo_size |= ((1 + 1) * (64 / 4) << 16); //depth
	np_txfifo_size |= rx_fifo_size;	//startaddr
	REG_GNPTXFIFO_SIZE = np_txfifo_size;

#define DWC_TX_FIFO_SIZE ((1 + 1) * (512 / 4))
	unsigned short startaddr;
	startaddr = ((1 + 1) * (64 / 4) << 16) + rx_fifo_size;
	for (i=1; i<dwc.hwcfg4.b.num_in_eps; i++)
	{
		tx_fifo_size |= (DWC_TX_FIFO_SIZE << 16) | startaddr;
		REG_GDIEP_TXF(i) = tx_fifo_size;
		startaddr += DWC_TX_FIFO_SIZE;
	}

	/* Configure fifo start addr and depth for endpoint information controller *
	 */
	unsigned short fifocfg;
	REG_GDFIFO_CFG |= startaddr << 16;
	fifocfg = REG_GHW_CFG3;
	fifocfg = (fifocfg >> 16);
	REG_GDFIFO_CFG |= fifocfg;
	/* flush tx and rx fifo */
	dwc_otg_flush_rx_fifo();

	dwc_otg_flush_tx_fifo(0x10);

}


static void dwc_otg_core_reset(void)
{
	u32 greset = 0;
	u32 cnt = 0;

	REG_GRST_CTL |= RSTCTL_CORE_RST;
	do {
		greset = REG_GRST_CTL;
		if (cnt++ > 100000  ) {
			TRACE("GRESET wait reset timeout.\n");
			return;
		}
		udelay(10);
	} while (greset & RSTCTL_CORE_RST);

	cnt = 0;
    
	do {
		udelay(10);
		greset = REG_GRST_CTL;
		if (cnt++ > 100000) {
			TRACE("GRESET wait IDLE timeout.\n");
			return;
		}
	} while ((greset & RSTCTL_AHB_IDLE) == 0);

	/* wait for 3 phy clocks */
	udelay(100);
}

static void dwc_otg_ep0_out_start(void)
{
	dwc_ep *pep = &ep[0];
	USB_DeviceRequest *device_req;
	if (dwc.is_dma == IS_INTERN_DMA)
	{
		pep->maxpacket = 64;
		pep->ctrl_req_addr = (unsigned int)(&setup_packet[0]);
		device_req = (USB_DeviceRequest *)&setup_packet[0];

		REG_DOEP_SIZE(0) = DOEPSIZE0_SUPCNT_3 | DOEPSIZE0_PKTCNT_BIT | (24);
		REG_DOEP_DMA(0) = PHYS(pep->ctrl_req_addr);
	}
	else
	{
		/* pio slave mode */
		pep->ctrl_req_addr = (unsigned int)(&setup_packet[0]);
		pep->maxpacket = 64;
		REG_DOEP_SIZE(0) = DOEPSIZE0_SUPCNT_3 | DOEPSIZE0_PKTCNT_BIT | (24);
	}
}

static void dwc_handle_early_suspend_intr(void)
{
// 	TRACE("Handle early suspend intr.\n");

	REG_GINT_STS = GINTSTS_USB_EARLYSUSPEND;

	return;
}

static void dwc_handle_suspend_intr(void)
{
// 	TRACE("Handle  suspend intr.\n");

	REG_GINT_STS = GINTSTS_USB_SUSPEND;

	return;

}

static void dwc_handle_start_frame_intr(void)
{
	//	TRACE("Handle start frame intr.\n");

	REG_GINT_STS = GINTSTS_START_FRAM;
	return;
}

static void dwc_handle_reset_intr(unsigned arg)
{
	int i;
	/* Step1: NAK OUT ep */
	for (i=0; i<dwc.hwcfg2.b.num_dev_ep; i++)
	{
		REG_DOEP_CTL(i) |= DEP_SET_NAK;
	}

	/* Step2: unmask the following interrupt bits */
	REG_DAINT_MASK = 0;
	REG_DOEP_MASK = 0;
	REG_DIEP_MASK = 0;

	REG_DAINT_MASK |=  (1 << 0) | (1 << 16);	//inep0 outep0
	REG_DOEP_MASK |= DEP_XFER_COMP | DEP_SETUP_PHASE_DONE | DEP_AHB_ERR;	// xfercompl setupdone
	REG_DIEP_MASK |= DEP_XFER_COMP | DEP_TIME_OUT	| DEP_AHB_ERR;	// xfercompl ahberr timeout

	/* Step3: Device initalization */
	dwc_otg_device_init();

	/* Step4: Set up the data fifo ram for each of the fifo */
	//dwc_calculate_fifo_size();

	/* Step5: Reset Device Address */
	REG_OTG_DCFG &= ~DCFG_DEV_ADDR_MASK;

	/* Step6: setup EP0 to receive SETUP packets */

	dwc_otg_ep0_out_start();

	REG_GINT_STS = GINTSTS_USB_RESET;

	//	dump_global_dwcreg();
	//	dwc2_ep0_dump_regs();
	return;
}


static void dwc_handle_enum_done_intr(dwc_cfg_if *pdwc)
{
	dwc_ep *pep = &ep[0];

	/* Step1: Read the DSTS register to determine the enumeration speed */
	u32 dsts = REG_OTG_DSTS;
	u32 diep0ctl = REG_DIEP_CTL(0);
	diep0ctl &= ~(0x3);


	switch(dsts & DSTS_ENUM_SPEED_MASK) {
	case DSTS_ENUM_SPEED_HIGH:
		TRACE("High speed.\n");
		pdwc->speed = USB_SPEED_HIGH;
		pep->maxpacket = 64;
		diep0ctl |= DEP_EP0_MPS_64;
		break;
	case DSTS_ENUM_SPEED_FULL_30OR60:
	case DSTS_ENUM_SPEED_FULL_48:
		TRACE("Full speed.\n");
		pdwc->speed = USB_SPEED_FULL;
		pep->maxpacket = 64;
		diep0ctl |= DEP_EP0_MPS_64;
		break;
	case DSTS_ENUM_SPEED_LOW:
		pdwc->speed = USB_SPEED_LOW;
		pep->maxpacket = 8;
		diep0ctl |= DEP_EP0_MPS_8;
		break;
	default:
		printf("Fault speed enumration\n");
		break;
	}

	/* Step2: Program the DIEPCTL0.MPS to set the maximum packet size */
	REG_DIEP_CTL(0) = diep0ctl;

	/* Step3: In Dma mode program the DOEPCTL0 register
	 * to enable control ouctrl_req_addrt endpoint0 to receive setup
	 * packet .*/
	dwc_otg_ep0_out_start();
	if (dwc.is_dma == IS_INTERN_DMA)
	{
		__dcache_writeback_all();
		TRACE("0 doepsize %x  ctl %x\n", REG_DOEP_SIZE(0), REG_DOEP_CTL(0));
		REG_DOEP_CTL(0) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
	}
	else
	{
		REG_DOEP_CTL(0) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
	}

	/* Step4: unmask the SOF interrupt */
// 	REG_GINT_MASK |= GINTMSK_START_FRAM;

	REG_GINT_STS = GINTSTS_ENUM_DONE;

	//	dump_global_dwcreg();
	return;
}

/*
 * ep0 control transfer:
 * 			3 Stage:
 * 			SetupPhase-------->IN DataPhase ---------> OUT StatusPhase
 * 		Or	2 Stage:
 * 			SetupPhase-------->IN StatusPhase
 * */

int dwc_ep0_out_intr(unsigned arg, unsigned int epnum)
{
	unsigned int intr, doep0size;
	unsigned char rem_supcnt, xfersize;
	unsigned int num_out_ep = 16;
	USB_DeviceRequest *device_req, *ptr;
	dwc_ep *pep = dwc.dep[num_out_ep + epnum];
    unsigned char *data_buf;
    
	intr = REG_DOEP_INT(epnum);
	//printf("out %x %d\n", intr, pep->ep_state);

	/* comp intrerrupt indeicates completion of the status out phase */
	if (intr & DEP_XFER_COMP)
	{
		REG_DOEP_INT(epnum) = DEP_XFER_COMP;
		if (pep->ep_state == EP_STATUS)
		{
			pep->ep_state = EP_SETUP;
			dwc_handle_ep_data_out_phase(epnum);
		}
	}

	if (intr & DEP_AHB_ERR)
	{
		printf("AHB ERR\n");
		REG_DOEP_INT(0) = DEP_AHB_ERR;
	}


	if ((intr & DEP_SETUP_PHASE_DONE) || (intr & DEP_STATUS_PHASE_RECV)) {
		/* read the DOEPTSIZn to determine the number of setup packets
		 * recevied and process the last recevied setup packet */
        if(intr & DEP_STATUS_PHASE_RECV)
            REG_DOEP_INT(epnum) = DEP_STATUS_PHASE_RECV;
        else
      		REG_DOEP_INT(epnum) = DEP_SETUP_PHASE_DONE;

        //接收 数据阶段：OUT + DATA1/0     ACK
        if(pep->ep_state == EP_DATA)
        {

            //printf("recv data\n");
            data_buf = (U8 *)&dwc.ctrl_request[2];
    		ptr = (USB_DeviceRequest *)(pep->ctrl_req_addr);//UNCACHED(&setup_packet[0]);//
    		__dcache_hitinv((unsigned int)ptr, sizeof(USB_DeviceRequest));
    		memcpy(data_buf, (u8 *)ptr, sizeof(USB_DeviceRequest));
            device_req = (USB_DeviceRequest *)(&dwc.ctrl_request[0]);
            
            BusNotify((unsigned int)arg, UDC_SETUP_PKG_FINISH, (unsigned char *)device_req, 8);
            return 0;
        }

        
		doep0size = REG_DOEP_SIZE(epnum);
		xfersize = doep0size & 0x7ffff;
		rem_supcnt = (doep0size & (0x3 << 29)) >> 29 ;
		//	printf("p:%d s:%d\n",rem_supcnt, xfersize);

		if (intr & DEP_B2B_SETUP_RECV)
		{
			printf("back to back setup recevie\n");
		}
		else
		{
			/* Read out the last packet from the rxfifo */

			ptr = (USB_DeviceRequest *)(pep->ctrl_req_addr);//UNCACHED(&setup_packet[0]);//
			__dcache_hitinv((unsigned int)ptr, sizeof(USB_DeviceRequest));
			memcpy(&dwc.ctrl_request[0], ptr, sizeof(USB_DeviceRequest));
			device_req = (USB_DeviceRequest *)(&dwc.ctrl_request[0]);
#if 0
			printf("\n\n-------------\n");
			printf("bRequest: %x\n", device_req->bRequest);
			printf("bRequestType: %x\n", device_req->bmRequestType);
			printf("wIndex: %x\n", device_req->wIndex);
			printf("wLength: %x\n", device_req->wLength);
			printf("wValue: %x\n", device_req->wValue);
			printf("-------------\n");
			//	dwc2_ep0_dump_regs();
#endif
            // 令牌阶段：SETUP +DATA0      ACK
			if(((device_req->bmRequestType & 0x80) == 0) && (device_req->wLength > 0))
			{
                //printf("enter recv data\n");
                dwc_handle_ep_data_out_phase(0);
                pep->ep_state = EP_DATA;
            }

			/* At the end of the Setup stage, the appliaction must reporgram the
			 * DOEPTSIZn.SUPCnt field to 3 receive the next SETUP packet */
			if (pep->ep_state == EP_SETUP)
			{
				if (dwc.is_dma == IS_INTERN_DMA)
				{
					//			printf("1 doepsize %x  ctl %x\n", REG_DOEP_SIZE(0), REG_DOEP_CTL(0));
					REG_DOEP_SIZE(epnum) = DOEPSIZE0_SUPCNT_3 | DOEPSIZE0_PKTCNT_BIT | (24);
					REG_DOEP_DMA(epnum) = PHYS(pep->ctrl_req_addr);
				}
				else
				{
					REG_DOEP_SIZE(epnum) = DOEPSIZE0_SUPCNT_3 | DOEPSIZE0_PKTCNT_BIT | (24);
				}
			}


			if ((device_req->bmRequestType == 0) && (device_req->bRequest == SET_FEATURE)
				 && (device_req->wValue == 2) && (device_req->wLength == 0))
			{
				if((device_req->wIndex>>8) <= 5)
				{
					usbTest = device_req->wIndex>>8;
					HW_SendPKT(0, NULL, 0);
					return 0;
				}
			}
			/* Find Setup cmd type */
            // 对处于有数据阶段的包等到接收到数据后再通知CLASS层处理
            if(pep->ep_state == EP_SETUP)
                BusNotify((unsigned int)arg, UDC_SETUP_PKG_FINISH, (unsigned char *)device_req, 8);
		}
	}
	return 0;
}

int dwc_epn_out_intr(unsigned arg, u32 epnum)
{
	unsigned int intr, updated_size;

	unsigned int num_out_ep = 16;
	unsigned char *p;

	dwc_ep *pep = dwc.dep[num_out_ep + epnum];
	intr = REG_DOEP_INT(epnum);
    
// 	printf("\n\n EP %d OUT- intr: %x\n", epnum, intr);
    
	if (intr & DEP_XFER_COMP) {
		REG_DOEP_INT(epnum) = DEP_XFER_COMP;
		updated_size = pep->xfer_len - (REG_DOEP_SIZE(epnum) & 0x7ffff);
        
		p = pep->xfer_buff;
//         printf("\n\n EP epnum%d OUT- intr: %x\n",epnum, intr);  


// 		printf("REG_DOEP_SIZE: %x \n", REG_DOEP_SIZE(epnum));
		pep->xfer_len = DEP_EP_MAXPKT(epnum); /* number of bytes to transfer */
		pep->xfer_count = 0;  /* number of bytes transfered */

        pep->xfer_buff = udc_Buf;
		pep->CallBackBuffer = pep->xfer_buff;
		dwc_handle_ep_data_out_phase(epnum);
        
		BusNotify((unsigned int)arg,UDC_PROTAL_RECEIVE_FINISH, p, updated_size);

	}

	if (intr & DEP_AHB_ERR)
	{
		printf("1 AHB ERR\n");
		REG_DOEP_INT(epnum) = DEP_AHB_ERR;
	}
	return 0;
}


int dwc_handle_outep_intr(unsigned arg)
{
	u32 ep_intr, epnum = 0;
	ep_intr = (REG_OTG_DAINT & 0xffff0000) >> 16;
	while (ep_intr)
	{
		if (ep_intr & 0x1)
		{
			if (epnum == 0)
    		{
				//printf("out ep0\n");
				dwc_ep0_out_intr(arg, 0);
			}
			else
			{
				//printf("out ep:%d\n", epnum);
				dwc_epn_out_intr(arg, epnum);
			}
		}
		epnum ++;
		ep_intr >>= 1;
	}
	return 0;
}



void dwc_ep0_in_intr(unsigned arg, unsigned int epnum)
{
	unsigned int updated_size;
	unsigned char *ptr;
	unsigned int num_in_ep = 0;
	unsigned int intr = REG_DIEP_INT(epnum);
	dwc_ep *pep = dwc.dep[num_in_ep + epnum];
	/* When the transfer size if 0 and the packet count is 0,
	 * the transfer complete interrupt for the endpoint is generated
	 * and the endpoint enable is cleared */

	//printf("in %x %d\n", intr, pep->ep_state);
	if (intr & DEP_XFER_COMP)
	{
		REG_DIEP_INT(epnum) = DEP_XFER_COMP;
		if (dwc.is_dma == IS_SLAVE_MODE)
			REG_DIEP_EMPMSK &= ~(1 << epnum);
		//	printf("i\n");
		updated_size = (REG_DIEP_SIZE(epnum) & 0x7f);
		pep->xfer_count = pep->xfer_len - updated_size; // number of bytes transfered


		if (pep->xfer_count != pep->xfer_len)
		{
			printf("in xfer_count:%d xfer_len:%d updated_size:%d\n", pep->xfer_count, pep->xfer_len, updated_size);
			pep->xfer_len -= pep->xfer_count;
			ptr = (u8 *)pep->xfer_buff;
			ptr = (u8 *)(ptr + pep->xfer_count);
			HW_SendPKT(0, ptr, pep->xfer_len);
			return ;
		}

		switch(pep->ep_state)
		{
		case EP_DATA:
			/* 3 Stage */
			pep->ep_state = EP_STATUS;

			/* enable next out status phase */
			dwc_handle_ep_data_out_phase(epnum);
			break;
		case EP_STATUS:
			/* 2 Stage */
			pep->ep_state = EP_SETUP;
			if (usbTest)
			{
				REG_OTG_DCTL &= ~(7<<4);
				REG_OTG_DCTL |= (usbTest<<4);
				//printf("test: %d\n", usbTest);
			}
			break;
		}


	}

	if (dwc.is_dma == IS_SLAVE_MODE)
	{
		if ((intr & DEP_TXFIFO_EMPTY) && (REG_DIEP_EMPMSK & (1 << epnum)))
		{
			if (pep->xfer_len)
			{
				dwc_write_ep_packet(epnum);
			}
			REG_DIEP_INT(epnum) = DEP_TXFIFO_EMPTY;
		}
	}

	if (intr & DEP_AHB_ERR)
	{
		printf("1 AHB ERR\n");
		REG_DIEP_INT(epnum) = DEP_AHB_ERR;
	}


	if (intr & DEP_TIME_OUT)
	{
		printf("IN TIME_OUT.\n");
		REG_DIEP_INT(epnum) = DEP_TIME_OUT;
	}
}

void dwc_epn_in_intr(unsigned arg, u32 epnum)
{
	unsigned int intr = REG_DIEP_INT(epnum);
	unsigned int updated_size;
	unsigned int num_in_ep = 0;
	static int bZeroPacket = 0;

	/* When the transfer size if 0 and the packet count is 0,
	 * the transfer complete interrupt for the endpoint is generated
	 * and the endpoint enable is cleared */
	dwc_ep *pep = dwc.dep[num_in_ep + epnum];
	if (intr & DEP_XFER_COMP)
	{
		//printf("1 IN XFER_COMP. %x\n", REG_DIEP_SIZE(epnum));
		REG_DIEP_INT(epnum) = DEP_XFER_COMP;
		if (pep->ep_state == EP_TRANSFERING)
		{
			if (dwc.is_dma == IS_SLAVE_MODE)
				REG_DIEP_EMPMSK &= ~(1 << epnum);

			if (bZeroPacket == 0)
				pep->EpBytesProcessed += pep->xfer_len;
			updated_size = pep->EpNumTotalBytes - pep->EpBytesProcessed;
			if (updated_size>0)//未发完数据
			{
				pep->xfer_buff += pep->xfer_len; 	/* pointer to transfer buffer */
				pep->xfer_len = updated_size; 			/* number of bytes to transfer */
				pep->xfer_count = 0;  			/* number of bytes transfered */
				if (pep->xfer_len > MAX_PKT_CNT * DEP_EP_MAXPKT(epnum))
					pep->xfer_len = MAX_PKT_CNT * DEP_EP_MAXPKT(epnum);
				dwc_handle_ep_data_in_phase(&dwc, epnum);
			}
			else
			{
    			if ((pep->type == DWC_OTG_EP_TYPE_BULK) &&
    				((pep->xfer_len % DEP_EP_MAXPKT(epnum)) == 0) &&
    				(bZeroPacket ==	0))
    			{
    				bZeroPacket = 1;
    				dwc_handle_ep_status_in_phase(&dwc, epnum);
    			}
    			else
    			{
    				updated_size = (REG_DIEP_SIZE(epnum) & 0x7ffff);
    				bZeroPacket = 0;
					BusNotify(arg, UDC_PROTAL_SEND_FINISH, 0, pep->EpBytesProcessed);
    				pep->xfer_len = 0;
    				pep->xfer_count = 0;
    				pep->ep_state = EP_TRANSFERED;	
    				if (pep->CallBackParameter)
    				{
						if (pep->CallBackBuffer)
    					{
    	 					//printf("free: %08x\n", pep->xfer_buff);
							deAlloc((MEM_ULONG)pep->CallBackBuffer);					
    					}
    					deAlloc((MEM_ULONG)pep->CallBackParameter);					
    				}
    				pep->xfer_buff = NULL;
    				pep->CallBackParameter = NULL;

					pep->EpBytesProcessed = 0;
					pep->EpNumTotalBytes = 0;
    				if (dwc.tx_list[epnum] != NULL)
    				{
    					udc_Tx(epnum);
    				}
				}
			}
		}
	}
    
	if (dwc.is_dma == IS_SLAVE_MODE)
	{
		if ((intr & DEP_TXFIFO_EMPTY) && (REG_DIEP_EMPMSK & (1 << epnum)))
		{
			REG_DIEP_EMPMSK &= ~(1 << epnum);
			if (pep->xfer_len)
			{
				dwc_write_ep_packet(epnum);
			}
			REG_DIEP_INT(epnum) = DEP_TXFIFO_EMPTY;
		}
	}
	if (intr & DEP_AHB_ERR)
	{
		printf("1 AHB ERR\n");
		REG_DIEP_INT(epnum) = DEP_AHB_ERR;
	}

	if (intr & DEP_TIME_OUT)
	{
		printf("IN TIME_OUT.\n");
		REG_DIEP_INT(epnum) = DEP_TIME_OUT;
	}
}


void dwc_handle_inep_intr(unsigned arg)
{
	unsigned int ep_intr;
	int epnum = 0;

	ep_intr = (REG_OTG_DAINT & 0xffff);
	//printf("\n\nEp IN %x  - \n", ep_intr);
	while(ep_intr)
	{
		if (ep_intr & 0x1)
		{
			if (epnum == 0)
			{
				dwc_ep0_in_intr(arg, epnum);
			}
			else
			{
				dwc_epn_in_intr(arg, epnum);
			}
		}
		epnum++;
		ep_intr >>= 1;
	}
	return ;
}

void dwc_handle_rxfifo_nempty(dwc_cfg_if *dwc_cfg)
{
	unsigned int count;
	unsigned int rxsts_pop = REG_GRXSTS_POP;
    u32 epnum = (rxsts_pop & 0xf);
    
	switch(rxsts_pop & GRXSTSP_PKSTS_MASK) {
	case GRXSTSP_PKSTS_GOUT_NAK:
		//		DBG("GRXSTSP_PKSTS_GOUT_NAK.\n");
		break;
	case GRXSTSP_PKSTS_GOUT_RECV:
		//		DBG("GRXSTSP_PKSTS_GOUT_RECV. - ");
		/* FIXME */

		count = (rxsts_pop & GRXSTSP_BYTE_CNT_MASK) >> GRXSTSP_BYTE_CNT_BIT;
		if (count) {
			printf("count:%d\n", count);
			dwc_read_ep_packet(epnum, count);
		}

		break;
	case GRXSTSP_PKSTS_TX_COMP:
		//	printf("GRXSTSP_PKSTS_TX_COMP.\n");
		break;
	case GRXSTSP_PKSTS_SETUP_COMP:
		//	printf("GRXSTSP_PKSTS_SETUP_COMP.\n");
		break;
	case GRXSTSP_PKSTS_SETUP_RECV:
		//	printf("GRXSTSP_PKSTS_SETUP_RECV. - ");
		setup_packet[0] = REG_EP_FIFO(0);
		setup_packet[1] = REG_EP_FIFO(0);
		//	printf("%x %x\n", setup_packet[0], setup_packet[1]);
		break;
	default:
		break;
	}
	REG_GINT_STS |= GINTSTS_RXFIFO_NEMPTY;
	return;
}

void dwc_otg_intr(void)
{
	return ;
}

void dwc_common_intr(unsigned int intsts, unsigned arg)
{
	if (intsts & GINTSTS_USB_EARLYSUSPEND)
	{
// 		printf("\n\nEARLY SUSPEND\n");
		dwc_handle_early_suspend_intr();
	}

	if (intsts & GINTSTS_USB_SUSPEND)
	{
// 		printf("\n\nUSB SUSPEND\n");
        BusNotify((unsigned int)arg, UDC_SUSPEND, NULL, 0);
		dwc_handle_suspend_intr();
	}


	if (intsts & GINTSTS_USB_RESET)
	{
//  	printf("\n\nUSB RESET\n");
        BusNotify((unsigned int)arg, UDC_RESET, NULL, 0);
		dwc_handle_reset_intr(arg);
        FreeAllBuff();
	}

	if (intsts & GINTSTS_ENUM_DONE)
	{
// 		printf("\n\nENUM DONE \n");
		dwc_handle_enum_done_intr(&dwc);
	}

	if (intsts & GINTSTS_START_FRAM)
	{
		dwc_handle_start_frame_intr();
	}
	return ;
}

void dwc_irq(unsigned arg)
{
 	disable_irq(IRQ_OTG);
	//t1 = os_TimeGet();
	os_SemaphorePost(dwc.dwc_sem);
}

static void dwc_task(void *arg)
{
	unsigned char err;
	unsigned int intsts;

	while(1)
	{
		os_SemaphorePend(dwc.dwc_sem, 0, &err);
		if (err != 0)
		{
			printf("dwc sem pend err %d\n", err);
			while(1);
		}
		intsts = REG_GINT_STS;
			//printf("intsts: %x\n", intsts);

		if (intsts & GINTSTS_OTG_INTR)
		{
			dwc_otg_intr();
		}

		if ( (intsts & GINTSTS_USB_EARLYSUSPEND) ||
				(intsts & GINTSTS_USB_SUSPEND)   ||
				(intsts & GINTSTS_START_FRAM) 	 ||
				(intsts & GINTSTS_USB_RESET) 	 ||
				(intsts & GINTSTS_ENUM_DONE) )
		{
			dwc_common_intr(intsts, (unsigned)arg);
		}

		/* dwc in pio mode not dma mode */
		if (intsts & GINTSTS_RXFIFO_NEMPTY)
		{
			if (dwc.is_dma == IS_SLAVE_MODE)
				dwc_handle_rxfifo_nempty(&dwc);
		}

		if (intsts & GINTSTS_IEP_INTR)
		{
			dwc_handle_inep_intr((unsigned)arg);
		}

		if (intsts & GINTSTS_OEP_INTR)
		{
			dwc_handle_outep_intr((unsigned)arg);
		}
		enable_irq(IRQ_OTG);
	}
}

static void dwc_otg_device_init(void)
{
	u32 dcfg = 0;




// 	printf("Device Init...%d\n", dwc.is_dma);
	/* Restart the phy clock */
	if (REG_PCGC_CTL & 0x1)
	{
		printf("<<<<<< pcgcctl %x >>>>>\n", REG_PCGC_CTL);
		REG_PCGC_CTL &= ~(0x1 | (1<<2) | (1 << 3));
	}

	/* In dma mode GINTMSK_NPTXFIFO_EMPTY ,
	 * GINTMSK_RXFIFO_NEMPTY must be masked*/

	if (dwc.is_dma == IS_INTERN_DMA)
	{
		if (REG_GINT_MASK & (GINTMSK_NPTXFIFO_EMPTY | GINTMSK_RXFIFO_NEMPTY))
		{
			REG_GINT_MASK &= ~(GINTMSK_NPTXFIFO_EMPTY | GINTMSK_RXFIFO_NEMPTY);
		}
	}
	else
	{
		REG_GINT_MASK |= (GINTMSK_NPTXFIFO_EMPTY | GINTMSK_RXFIFO_NEMPTY);
	}

	/* Program the DCFG register */
	if (dwc.hwcfg4.b.desc_dma)
	{
		dcfg |= DCFG_DEV_DESC_DMA;
	}
    #if DWC_FORCE_SPEED_FULL
	REG_OTG_DCFG = 1; //dma buffer mode full speed
	#else
    REG_OTG_DCFG = 0; //dma buffer mode HIGH speed
    #endif

	/* Clear the DCTL.SftDiscon bit the core issues aconnect after ths bit is cleared */
	REG_OTG_DCTL &= ~DCTL_SOFT_DISCONN;

	REG_GINT_STS = 0xffffffff;
	/* Program the GINTMSK */
	REG_GINT_MASK |= GINTMSK_IEP_INTR | GINTMSK_OEP_INTR |GINTMSK_USB_RESET | GINTMSK_ENUM_DONE |GINTMSK_USB_EARLYSUSPEND |	GINTMSK_USB_SUSPEND;
}

static void dwc_otg_core_init(unsigned char dma_enable)
{
	//	dwc_cfg_if dwc;
	u32 ahbcfg = 0, gusbcfg = 0;
	u8 arch;

	//	serial_printf("Core Init...\n");
	/* Step1: Read the GHWCFG1,2,3,4 to find the configuration parameters selected for DWC_otg core */
	dwc.hwcfg1.d32 = REG_GHW_CFG1;
	dwc.hwcfg2.d32 = REG_GHW_CFG2;
	dwc.hwcfg3.d32 = REG_GHW_CFG3;
	dwc.hwcfg4.d32 = REG_GHW_CFG4;

	//	serial_printf("cfg1:%x 2:%x 3:%x 4:%x\n", dwc.hwcfg1, dwc.hwcfg2, dwc.hwcfg3, dwc.hwcfg4);
	//	serial_printf("cfg2->arch %x\n", dwc.hwcfg2.b.architecture);
	arch = dwc.hwcfg2.b.architecture;
	switch (arch)
	{
	case IS_SLAVE_MODE:
		dwc.is_dma = 0; 
        break;
        
	case IS_EXTERN_DMA:
		dwc.is_dma = 1; 
        break;
        
	case IS_INTERN_DMA:
		dwc.is_dma = 2; 
        break;
	}
	/* Step2: Program the GAHBCFG register */

	/* DMA Mode bit and Burst Length */
	if (dwc.is_dma == IS_EXTERN_DMA)
	{
		//		serial_printf("DWC IS_EXTERN_DMA\n");
		ahbcfg |= AHBCFG_DMA_ENA;
	}
	else if (dwc.is_dma == IS_INTERN_DMA)
	{
		if (dma_enable)
		{
			//		serial_printf("DWC IS_INTERN_DMA\n");
			ahbcfg |= AHBCFG_DMA_ENA | (DWC_GAHBCFG_INT_DMA_BURST_INCR16 << 1);
		}
		else
		{
			ahbcfg |= AHBCFG_TXFE_LVL;
			dwc.is_dma = 0;
		}
	}
	else
	{
		printf("DWC IS_SLAVE_MODE\n");
	}

	/* Step3: Program the GINTMSK register */
	REG_GINT_MASK = 0;

	/* Step4: Program the GUSBCFG register */
	gusbcfg = REG_GUSB_CFG;

	gusbcfg &= ~((1 << 4) | (1 << 6) | (1 << 8) | (1 << 9));
    gusbcfg |= (1<<30);
	REG_GUSB_CFG = gusbcfg;	// HNP SRP not support and select UTMI+
	dwc_otg_select_phy_width();

	dwc_otg_core_reset();

	/* Global Interrupt Mask bit = 1 */
	ahbcfg |= AHBCFG_GLOBLE_INTRMASK;
	REG_GAHB_CFG = ahbcfg;

	/* Step5: The software must unmask OTG Interrupt Mask bit ,
	 * MOde mismatch interrupt Mask bit in the GINTMSK */
	REG_GINT_MASK |= (GINTMSK_MODE_MISMATCH | GINTMSK_OTG_INTR);
	//	REG_GINT_MASK = 0;
}

void dwc_init_endpoint(dwc_cfg_if *dwc_cfg)
{
	unsigned int num_in_ep = 0;
	unsigned int num_out_ep = 16;
	unsigned int i;

	/* control ep */
	dwc_cfg->dep[num_in_ep] = &ep[0];
	dwc_cfg->dep[num_out_ep] = &ep[0];

    for(i=1; i< g_ep_num; i++)
    {
        if((ep[i].num & 0x80) == 0)  //out ep
        {
        	dwc_cfg->dep[num_out_ep + i] = &ep[i];
        }
        else                          //in ep
        {
        	dwc_cfg->dep[num_in_ep + i] = &ep[i];
        }
    }
    
	for (i=0; i<16; i++)
	{
		dwc_cfg->tx_list[i] = NULL;
	}

}


/* usb device init */
void dwc_gadget_init(unsigned arg)
{
	disable_irq(IRQ_OTG);

    ep[0].ep_state = EP_SETUP;
    ep[1].ep_state = EP_IDLE;
    ep[2].ep_state = EP_IDLE;

	usb_cpm_init();

	/* force usb device mode */
	jz47xx_usb_set_device_only_mode();


	jz47xx_usb_phy_init();

	jz47xx_usb_phy_switch(1);

	/* soft disconnect and soft reset */
	REG_OTG_DCTL |= DCTL_SOFT_DISCONN;
    udelay(3000);

	/* reset dwc register */
	dwc_otg_core_reset();

	/* DWC OTG Core init */
	dwc_otg_core_init(1);

	/* Read Gintsts confirm the device or host mode */
// 	curmod = REG_GINT_STS;
// 	if (curmod & 0x1)
// 	{
// 		printf("Curmod: Host Mode\n");
// 	}
// 	else
	{
		//printf("Curmod: Device Mode\n");

		/* DWC OTG Device init */
		dwc_otg_device_init();

		/* DWC OTG Fifo init */
		dwc_calculate_fifo_size();
	}

	dwc_init_endpoint(&dwc);

	if (request_irq(IRQ_OTG, dwc_irq, (unsigned) arg)) {
		printf("[DWC] DWC request irq failed\n");
	}
    else {
	
		//printf("[DWC] DWC request irq success %x\n", REG_GINT_MASK);
	}
}

void init_udc(PUDC_BUS pBus)
{
	unsigned char err;

    if( dwc.dwc_sem == 0)
    {
    	pBus->EnableDevice = enable_device;
    	pBus->SetAddress = set_address;
    	pBus->SetConfig = set_config;
    	pBus->StartTransfer = start_transfer;
    	pBus->InitEndpointSuppost = init_endpoint_suppost;
    	pBus->DisableDevice = disable_device;

    	dwc.dwc_sem = os_SemaphoreCreate(0);
    	if (!dwc.dwc_sem)
    	{
    		printf("%s %d sem create err\n", __FUNCTION__, __LINE__);
    		while (1);
    	}

    	err = os_TaskCreate(dwc_task, (void *)pBus, (void *)&Dwc_Task_Stack[TASK_DWC_STK_SIZE - 1], TASK_DWC_PRIO);
    	if (err != 0)
    	{
    		printf("%s %d task create err\n", __FUNCTION__, __LINE__);
    		while (1);
    	}

        //printf("disconnect usb cable 2\n");
    	dwc.status.b.state = USB_CABLE_DISCONNECT;
    	dwc.status.b.event = 0;
    }
	dwc_gadget_init((unsigned) pBus);
}


// void deinit_udc()
// {
// 	/* soft disconnect and soft reset */
// 	REG_OTG_DCTL |= DCTL_SOFT_DISCONN;
// 
//     free_irq(IRQ_OTG);
// }



void deinit_udc()
{
// 	disable_irq(IRQ_OTG);

	/* soft disconnect and soft reset */
	REG_OTG_DCTL |= DCTL_SOFT_DISCONN;

    free_irq(IRQ_OTG);

    FreeAllBuff();
}

