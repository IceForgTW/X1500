/********************** BEGIN LICENSE BLOCK **********************************
 *
 * INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Copyright (c) Ingenic Semiconductor Co., Ltd. 2008. All rights reserved.
 *
 * This file, and the files included with this file, is distributed and made
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 *
 * http://www.ingenic.cn
 *
 ********************** END LICENSE BLOCK ************************************
 *
 *  Author:  <whxu@ingenic.cn>
 *
 *  Create:   2008-10-25, by whxu
 *
 ****************************************************************************/

#include <bsp.h>
#include <x1000.h>

#undef CFG_UART_BASE
#define CFG_UART_BASE UART2_BASE
//----------------------------------------------------------------------------
void serial_putc(char c);
void serial_puts(const char *s);
int serial_init(void)
{
	//unsigned int	baudrate_div = CFG_EXTAL / 16 / CFG_UART_BAUDRATE;
	unsigned int	baudrate_div = 24000000 / 16 / 115200;

#if (CFG_UART_BASE == UART0_BASE)
	__gpio_as_uart0();
	__cpm_start_uart0();
#elif (CFG_UART_BASE == UART2_BASE)
	__gpio_as_uart2_pa();
	__cpm_start_uart2();
#elif (CFG_UART_BASE == UART1_BASE)
	__gpio_as_uart1_pd();
	__cpm_start_uart1();
#else
	#error There are only two serial port!!!
#endif

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

	return 0;
}

//----------------------------------------------------------------------------

void serial_putc(char c)
{
	if (c == '\n') 
		serial_putc('\r');

	// Wait FIFO Empty
	while ( !(INREG8((CFG_UART_BASE + OFF_LSR)) & UART_LSR_TDRQ) );

	OUTREG8(CFG_UART_BASE + OFF_TDR, c);
}

//----------------------------------------------------------------------------

void serial_puts(const char *s)
{
	while (*s)
	{
		serial_putc(*s++);
	}
}

//----------------------------------------------------------------------------

int serial_getc(void)
{
	return (0);
}

//----------------------------------------------------------------------------

void serial_put_hex(unsigned int d)
{
	unsigned char c ;
	unsigned int i;
	for(i = 0; i < 8;i++)
	{
		c = (unsigned char)((d >> ((7 - i) * 4)) & 0xf);
		if(c < 10)
			c += 0x30;
		else
			c += (0x41 - 10);
		serial_putc(c);
	}
	serial_putc('\n');
}


//----------------------------------------------------------------------------
