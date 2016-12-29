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
 *  Author:
 *
 *  Create:
 *
 *  Maintain:
 *
 *
 *******************************************************************************
 */


#include <x1000.h>
#include "os_api.h"
#include "../../../../MB_Decode_Ctrl/Common.h"
#include "../misc/generate.h"


#include <MemMgr.h>
#include <watchdog.h>
//------------------------------------------------------------------------------
//	Global defines
//------------------------------------------------------------------------------
#undef printf
#include <stdio.h>
#include <string.h>
#undef UART_BASE
#define UART_BASE 0xb0030000


#define UART_REC_BUF_LEN	(1024 * 16)

#define UART_FLAG_READ				1
#define UART_FLAG_WRITE_HALF		2
#define UART_FLAG_WRITE_COMPLETE	4


typedef struct _uart_info_struct_{
	TComParam param;
	OS_FLAG_GROUP *uartFlag;
	OS_EVENT  WriteMutex;
	unsigned char *recbuf;	//[UART_REC_BUF_LEN];
	unsigned int dat_trail;
	unsigned int dat_head;	
}UART_INFO;

UART_INFO *uartinfo;

static volatile BOOL bWaitEmt = FALSE;
static int nUARTPort = 0;


static int uart_gpio_init (unsigned int port)
{
    switch(port)
    {
    case 1:
    	__gpio_as_uart1_pa();
    	__cpm_start_uart1();
        break;
    case 0:      
    	__gpio_as_uart0();
    	__cpm_start_uart0();
        break;
   case 2:
    	__gpio_as_uart2_pa();
	__cpm_start_uart2();
    default:
        //printf("Error uart port! Must be port 0 or port 1.\n");
        break;
    }
    return 0;
}



static void receive_chars(unsigned int port)
{
	unsigned char err = 0;
	unsigned int lvi = 0;
	unsigned char c;
	unsigned int lsr = INREG8(UART_BASE + 0x1000 * port + OFF_LSR);

	lvi = UART_REC_BUF_LEN - 1 - (uartinfo->dat_head - uartinfo->dat_trail);
	if(lvi >= UART_REC_BUF_LEN)
	{	
		lvi -= UART_REC_BUF_LEN;
	}

	while(lsr & UARTLSR_DR) {
		c = INREG8(UART_BASE + 0x1000 * port + OFF_RDR);
		if(lvi > 0)
		{
			uartinfo->recbuf[uartinfo->dat_head++] = c;
			if(uartinfo->dat_head >= UART_REC_BUF_LEN) 
			{
				uartinfo->dat_head = 0;
			}
			lvi --;
		}
		lsr = INREG8(UART_BASE + 0x1000 * port + OFF_LSR);
	}
 //   printf("%s\n",uartinfo->recbuf);
//printf("rh=%d, rt=%d\n", uartinfo->dat_head, uartinfo->dat_trail);	
	os_FlagPost(uartinfo->uartFlag, UART_FLAG_READ, 1, &err);
}


static void uart_interrupt_handler(unsigned int port)
{
	unsigned int isr = INREG8(UART_BASE + 0x1000 * port + OFF_ISR);
	unsigned int lsr = INREG8(UART_BASE + 0x1000 * port + OFF_LSR);
	unsigned int ier = INREG8(UART_BASE + 0x1000 * port + OFF_IER);
	unsigned char err = 0;

	if(isr & UARTISR_IP)
		return;

	disable_irq(IRQ_UART0-port);

// printf("lsr:%x, isr: %x, ier: %x, utcr: %x\n", lsr, isr, ier,INREG8(UART_BASE + 0x1000 * port + OFF_UTCR));

	if(lsr & UART_LSR_DR)
		receive_chars(port);

	if((lsr & UART_LSR_TDRQ) && (ier & UARTIER_TIE))
	{
		
		if (!bWaitEmt)
		{
			os_FlagPost(uartinfo->uartFlag, UART_FLAG_WRITE_HALF, 1, &err);
			CLRREG8((UART_BASE + 0x1000 * port + OFF_IER), UARTIER_TIE);
		}
// 		printf("h:%d, %d\n", os_TimeGet(), bWaitEmt);
	}
	
	if ((lsr & UARTLSR_TEMT) && bWaitEmt)
	{
		bWaitEmt = FALSE;
		os_FlagPost(uartinfo->uartFlag, UART_FLAG_WRITE_COMPLETE, 1, &err);
		CLRREG8((UART_BASE + 0x1000 * port + OFF_IER), UARTIER_TIE);
//  		printf("f:%d\n", os_TimeGet());
	}

	enable_irq(IRQ_UART0-port);
}



void uartsetparam(TComParam * param)
{
	unsigned int	port = param->nPort;
	unsigned int	baudrate_div = 24000000 / 16 / param->lRate;

//	CLRREG8((UART_BASE + 0x1000 * port + OFF_FCR), UARTFCR_UUE);
//	SETREG8((UART_BASE + 0x1000 * port + OFF_FCR), UARTFCR_TFLS|UARTFCR_RFLS);
	
	OUTREG8((UART_BASE + 0x1000 * port + OFF_SIRCR), ~(SIRCR_RSIRE|SIRCR_TSIRE));
	
	// Set Baudrate
	SETREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_DLAB);
	OUTREG8((UART_BASE + 0x1000 * port + OFF_DLHR), (baudrate_div >> 8) & 0xFF);
	OUTREG8((UART_BASE + 0x1000 * port + OFF_DLLR), baudrate_div & 0xFF);
	CLRREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_DLAB);
	
    /* Set parity */
    if (param->lParity == NOPARITY)
		CLRREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_PE);
    else if (param->lParity == ODDPARITY)
	{
		SETREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_PROE);
		SETREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_PE);
	}
    else {                      /* parity == PARITY_EVEN */
        CLRREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_PROE);
		SETREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_PE);
    }
    
    /* Set stop bit */
    if (param->lStopBits == 1)
		CLRREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_STOP);
    else                        /* stopbits == STOPBITS_TWO */
		SETREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_STOP2);
	
	/* Set data size */
	if (param->lDataBits == 8)
		SETREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_WLEN_8);
	else if (param->lDataBits == 7)
	{
		CLRREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_WLEN);
		SETREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_WLEN_7);
	}
	else if (param->lDataBits == 6)
	{
		CLRREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_WLEN);
		SETREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_WLEN_6);
	}
	else if (param->lDataBits == 5)
	{
		CLRREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_WLEN);
		SETREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_WLEN_5);
	}
	
    
    /* Configure the flow control */
    if (param->nflowCtrl == 1) {
		OUTREG8((UART_BASE + 0x1000 * port + OFF_MCR), UARTMCR_MCE|UARTMCR_FCM);   //set Modem Control bit
		
    } else {                    /* flowcontrol == FLOWCTRL_OFF */
		OUTREG8((UART_BASE + 0x1000 * port + OFF_MCR), 0);   //set Modem Control bit
    }
	
//	OUTREG8((UART_BASE + 0x1000 * port + OFF_FCR), 0);
//	SETREG8((UART_BASE + 0x1000 * port + OFF_FCR), UARTFCR_FE|UARTFCR_UUE|UARTFCR_RTRG_1);
	
}




void UartInit(void)
{
	unsigned char err;

       nUARTPort = 0;
       __gpio_as_output(GPIO_GROUP_B+3);   
       __gpio_clear_pin(GPIO_GROUP_B+3);	
	uartinfo = (UART_INFO*)alloc(sizeof(UART_INFO));
	uartinfo->param.nPort = nUARTPort;
	uartinfo->param.lRate = 115200;
	uartinfo->param.lDataBits = 8;
	uartinfo->param.lParity = 0;
	uartinfo->param.lStopBits = 1;
	uartinfo->param.nflowCtrl = 0;

	uartinfo->recbuf = (unsigned char*)alignAlloc(4, UART_REC_BUF_LEN);
	uartinfo->dat_head = uartinfo->dat_trail = 0;

	uart_gpio_init(uartinfo->param.nPort);

  	CLRREG8((UART_BASE + 0x1000 * (uartinfo->param.nPort) + OFF_FCR), UARTFCR_UUE);
	SETREG8((UART_BASE + 0x1000 * (uartinfo->param.nPort) + OFF_FCR), UARTFCR_TFLS|UARTFCR_RFLS);
	uartsetparam(&uartinfo->param);

	OUTREG8((UART_BASE + 0x1000 * (uartinfo->param.nPort) + OFF_FCR), 0);
	SETREG8((UART_BASE + 0x1000 * (uartinfo->param.nPort) + OFF_FCR), UARTFCR_UUE|UARTFCR_FE|UARTFCR_RTRG_1);
	OUTREG8((UART_BASE + 0x1000 * (uartinfo->param.nPort) + OFF_IER), UARTIER_RIE);

	request_irq(IRQ_UART0-uartinfo->param.nPort, uart_interrupt_handler, uartinfo->param.nPort);
	
	uartinfo->uartFlag  = os_FlagCreate(0, &err);
//	uartinfo->WriteMutex = os_MutexCreate(5, &err);
	
}



int comm_read(void *buf, unsigned int len, unsigned int timeout)
{
	int lvret = 0;
	unsigned char err;

	char *lvp = (char*)buf;

	if(!buf) return -1;
	
	if(uartinfo->dat_trail == uartinfo->dat_head){
		if(timeout == TIMEOUT_NOWAIT) return 0;
		ResetWatchDog();
		if ( UART_FLAG_READ != os_FlagPend(uartinfo->uartFlag, UART_FLAG_READ, OS_FLAG_WAIT_SET_ALL | OS_FLAG_CONSUME, (timeout+9)/10, &err))
			return 0;
	}

	while(1){
		while((lvret < len) && (uartinfo->dat_head != uartinfo->dat_trail)){
			*lvp ++ = uartinfo->recbuf[uartinfo->dat_trail ++];
			if(uartinfo->dat_trail >= UART_REC_BUF_LEN) uartinfo->dat_trail = 0;
			lvret ++;
		}
		if((lvret >= len) || (timeout == TIMEOUT_NOWAIT)) break;
		ResetWatchDog();
		if ( UART_FLAG_READ != os_FlagPend(uartinfo->uartFlag, UART_FLAG_READ, OS_FLAG_WAIT_SET_ALL | OS_FLAG_CONSUME, (timeout+9)/10, &err))
			break;
	}
//printf("l:%d, h:%d, t:%d\n", lvret, uartinfo->dat_head, uartinfo->dat_trail);	
	return lvret;
}


int comm_write(void * buff, unsigned int size)
{
	int lvret = 0;
	char *lvp = (char*)buff;
    
	int timeout;
	int port = 1;
	unsigned short flag;
	unsigned char err;
	
	if(size < 0) return -1;
	
//	os_MutexPend(&uartinfo->WriteMutex, 0, &err);

	timeout = GetWatchDogResetTime();
// 	printf("write 1: %d\n", size);

////////

	while (lvret < size)
	{
		if ((INREG8(UART_BASE + 0x1000 * nUARTPort+ OFF_UTCR) & 0x7f) <64)
		{
			OUTREG8((UART_BASE + 0x1000 * nUARTPort + OFF_TDR), *lvp ++);
			lvret ++;
		}
		else
		{
			SETREG8((UART_BASE + 0x1000 * nUARTPort + OFF_IER), UARTIER_TIE);
			if (timeout)
			{
			      ResetWatchDog();
				while(UART_FLAG_WRITE_HALF != os_FlagPend(uartinfo->uartFlag, UART_FLAG_WRITE_HALF, 
					OS_FLAG_WAIT_SET_ALL | OS_FLAG_CONSUME, timeout/10 - 2, &err))
				{	
					ResetWatchDog();
				}
				
			}
			else
			{
				flag = os_FlagPend(uartinfo->uartFlag, UART_FLAG_WRITE_HALF, OS_FLAG_WAIT_SET_ALL | OS_FLAG_CONSUME, 0, &err);
				if (flag != UART_FLAG_WRITE_HALF)
				{
					printf("comm_wait FlagPend1: %x\n", flag);
				}
			}
		}
	}

	SETREG8((UART_BASE + 0x1000 * nUARTPort + OFF_IER), UARTIER_TIE);

	bWaitEmt = TRUE;
	SETREG8((UART_BASE + 0x1000 * nUARTPort + OFF_IER), UARTIER_TIE);

	if (timeout)
	{
	      ResetWatchDog();
		while(UART_FLAG_WRITE_COMPLETE != os_FlagPend(uartinfo->uartFlag, UART_FLAG_WRITE_COMPLETE, 
			OS_FLAG_WAIT_SET_ALL | OS_FLAG_CONSUME, timeout/10 - 2, &err))
		{	
			ResetWatchDog();
		}
	}
	else
	{
		flag = os_FlagPend(uartinfo->uartFlag, UART_FLAG_WRITE_COMPLETE, OS_FLAG_WAIT_SET_ALL | OS_FLAG_CONSUME, 0, &err);
		if (flag != UART_FLAG_WRITE_COMPLETE)
		{
			printf("comm_wait FlagPend2: %x, err: %d\n", flag, err);
		}
	}

// 	os_MutexPost(&uartinfo->WriteMutex);

	return lvret;
}

void uart_set_parameter(void *param)
{

	if (!param) return;

	memcpy(&uartinfo->param, param, sizeof(TComParam));
	uartinfo->param.nPort = nUARTPort;
	
// 	diag_printf("b: %d, s: %d, d: %d, p: %d, f: %d\n", 
// 		uartinfo->param.lRate, uartinfo->param.lStopBits, uartinfo->param.lDataBits, uartinfo->param.lParity, uartinfo->param.nflowCtrl);
 	uartsetparam(&uartinfo->param);
}

void uart_get_parameter(void *param)
{
	if (!param) return;

	memcpy(param, &uartinfo->param, sizeof(TComParam));
}

