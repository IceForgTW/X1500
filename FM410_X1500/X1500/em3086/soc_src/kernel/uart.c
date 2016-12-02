/********************** BEGIN LICENSE BLOCK ************************************
 *
 * JZ4755  Driver  V1.0.0
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
 *  Create:   2008-09-24, by George Zhang
 *            
 *  Maintain: 2008-11-08, by  George Zhang
 *            
 *
 *******************************************************************************
 */
 
#include <bsp.h>
#include <x1000.h>
#include <os_api.h>
#include <mipsregs.h>

#undef CFG_UART_BASE
#define CFG_UART_BASE UART2_BASE

void serial_setgpio(void)
{
#if (CFG_UART_BASE == UART0_BASE)
	__gpio_as_uart0();
	__cpm_start_uart0();
#elif (CFG_UART_BASE == UART2_BASE)
	__gpio_as_uart2_pa();
	__cpm_start_uart2();
#elif (CFG_UART_BASE == UART1_BASE)
	__gpio_as_uart1_pa();
	__cpm_start_uart1();
#else
	#error There are only two serial port!!!
#endif

}


void serial_setbrg (void)
{	
	unsigned short UART_DIVISOR_LATCH = DEVICE_CLOCK / 16 / DEBUG_UART_BAUDRATE;
	SETREG8( (CFG_UART_BASE + OFF_LCR), UARTLCR_DLAB );	
	OUTREG8( (CFG_UART_BASE + OFF_DLLR), UART_DIVISOR_LATCH & 0xFF );
	OUTREG8( (CFG_UART_BASE + OFF_DLHR), (UART_DIVISOR_LATCH >> 8) & 0xFF );
	CLRREG8( (CFG_UART_BASE + OFF_LCR), UARTLCR_DLAB );
}

void serial_putc (const char c)
{
	if (c == '\n') serial_putc ('\r');
	/* Wait for fifo to shift out some bytes */
	while ( !(INREG8((CFG_UART_BASE + OFF_LSR))&UARTLSR_TDRQ ) );
	OUTREG8((CFG_UART_BASE + OFF_TDR), c); 	
}

void serial_waitfinish()
{
	while(!(INREG8((CFG_UART_BASE + OFF_LSR)) & UARTLSR_TEMT ) );
}


void serial_puts (const char *s)
{
	while (*s)
		serial_putc (*s++);
}

/* run else step */

int serial_init (void)
{
	serial_setgpio();

	unsigned int	baudrate_div = 24000000 / 16 / 115200;

//	__cpm_start_uart1();
	*(volatile unsigned int *)0xb0000020 &= ~(1 << 16);

	OUTREG8(CFG_UART_BASE + OFF_IER, 0);    //disable interupt
	OUTREG8(CFG_UART_BASE + OFF_FCR, ~UARTFCR_UUE);   //disable uart
	OUTREG8(CFG_UART_BASE + OFF_SIRCR, ~(SIRCR_TSIRE | SIRCR_RSIRE));   //config to uart mode
	OUTREG8(CFG_UART_BASE + OFF_LCR, UARTLCR_WLEN_8 | UARTLCR_STOP1);

	// Set Baudrate
	SETREG8(CFG_UART_BASE + OFF_LCR, UARTLCR_DLAB);
	OUTREG8(CFG_UART_BASE + OFF_DLHR, (baudrate_div >> 8) & 0xFF);
	OUTREG8(CFG_UART_BASE + OFF_DLLR, baudrate_div & 0xFF);
	CLRREG8(CFG_UART_BASE + OFF_LCR, UARTLCR_DLAB);

	// Enable UART
	OUTREG8(CFG_UART_BASE + OFF_FCR, UARTFCR_UUE | UARTFCR_FE | UARTFCR_RFLS | UARTFCR_TFLS);

/*
	OUTREG8( (CFG_UART_BASE + OFF_IER), 0 );
	CLRREG8( (CFG_UART_BASE + OFF_FCR), UARTFCR_UUE );	
	CLRREG8( (CFG_UART_BASE + OFF_SIRCR), (SIRCR_RSIRE|SIRCR_TSIRE) );
	SETREG8( (CFG_UART_BASE + OFF_LCR), (UARTLCR_WLEN_8|UARTLCR_STOP) );
	serial_setbrg();
	SETREG8( (CFG_UART_BASE + OFF_FCR), (UARTFCR_FE | UARTFCR_RFLS | UARTFCR_TFLS | UARTFCR_UUE | UARTFCR_RTRG_15) );
*/
	serial_puts("UART : query mode \n");
	return 0;
}



int serial_tstc (void)
{
	/* Data in rfifo */
	return  (INREG8((CFG_UART_BASE + OFF_LSR)) & UARTLSR_DR );
}


static  OS_EVENT  *jz_std_sem = NULL;
#define OP_STD_LOCK() do{ \
		unsigned char  err; \
		os_SemaphorePend(jz_std_sem, 0, &err); \
		}while(0)
#define OP_STD_UNLOCK() do{ \
		os_SemaphorePost(jz_std_sem); \
		}while(0)

#if DRIVER_ONLY
unsigned char os_init_completed=1;
#else
extern unsigned char os_init_completed;
#endif

int jz_std_puts(const unsigned char *s,unsigned int count)
{
  unsigned int i = 0;
  if (os_init_completed)
  {
    if(jz_std_sem == NULL) jz_std_sem = os_SemaphoreCreate(1);
    OP_STD_LOCK();	
  }
  while(i++ < count)
   	serial_putc(*s++);
  if (os_init_completed)
    OP_STD_UNLOCK();
  return (count);
}

char serial_getc (void)
{
	while (!serial_tstc());               
	return INREG8((CFG_UART_BASE + OFF_RDR));
}

int jz_std_gets(unsigned char *s,unsigned int count)
{
  unsigned int i = 0;
  if(jz_std_sem == NULL) jz_std_sem = os_SemaphoreCreate(1);
  OP_STD_LOCK();	
  while(i++ < count)
   	*s++ = serial_getc();
  OP_STD_UNLOCK();
  return (count);
}

void serial_put_hex(unsigned int d)
{
	unsigned char c[12];
	unsigned int i;
	for(i = 0; i < 8;i++)
	{
		c[i] = (unsigned char)((d >> ((7 - i) * 4)) & 0xf);
		if(c[i] < 10)
			c[i] += 0x30;
		else
			c[i] += (0x41 - 10);
	}
	c[8] = '\n';
	c[9] = 0;
	serial_puts((const char *)c);
}


