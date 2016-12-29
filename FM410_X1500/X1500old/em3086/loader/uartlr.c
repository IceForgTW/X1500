//#include <generate.h>
#include <bsp.h>
#include <x1000.h>
#include <os_api.h>
#include <mipsregs.h>

#include <time-tcu.h>

#undef UART0_BASE
#define UART0_BASE 0xb0030000

#define TIMEOUT_NOWAIT		0xffffffff
#define TIMEOUT_INFINITE	0

static int uart_gpio_init (unsigned int port)
{
    switch(port)
    {
    case 1:
    	__gpio_as_uart1_pd();
    	__cpm_start_uart1();
        break;
    case 0:
    	__gpio_as_uart0();
    	__cpm_start_uart0();
        break;
    default:
        //printf("Error uart port! Must be port 0 or port 1.\n");
        break;
    }
    return 0;
}

#if 0
void init_uart(int port, int baudrate)
{
	unsigned int	baudrate_div = 24000000 / 16 / baudrate;
    
      uart_gpio_init(port);
      __cpm_start_uart1();
      
	CLRREG8((UART_BASE + 0x1000 * port + OFF_FCR), UARTFCR_UUE);
	SETREG8((UART_BASE + 0x1000 * port + OFF_FCR), UARTFCR_TFLS|UARTFCR_RFLS);
	
	OUTREG8((UART_BASE + 0x1000 * port + OFF_SIRCR), ~(SIRCR_RSIRE|SIRCR_TSIRE));
	
	// Set Baudrate
	SETREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_DLAB);
	OUTREG8((UART_BASE + 0x1000 * port + OFF_DLHR), (baudrate_div >> 8) & 0xFF);
	OUTREG8((UART_BASE + 0x1000 * port + OFF_DLLR), baudrate_div & 0xFF);
	CLRREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_DLAB);
	
    /* Set parity */
      CLRREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_PE);
    
    
    /* Set stop bit */
      CLRREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_STOP);
   
	/* Set data size */
      SETREG8((UART_BASE + 0x1000 * port + OFF_LCR), UARTLCR_WLEN_8);
	
	
    
    /* Configure the flow control */
                    /* flowcontrol == FLOWCTRL_OFF */
      OUTREG8((UART_BASE + 0x1000 * port + OFF_MCR), 0);   //set Modem Control bit
    
	
	OUTREG8((UART_BASE + 0x1000 * port + OFF_FCR), 0);
	SETREG8((UART_BASE + 0x1000 * port + OFF_FCR), UARTFCR_FE|UARTFCR_UUE|UARTFCR_RTRG_1);
	
}

#else

void init_uart(int port, int baudrate)
{
	unsigned int	baudrate_div = 24000000 / 16 / baudrate;
    
      uart_gpio_init(port);
      __cpm_start_uart1();

      OUTREG8(UART0_BASE + 0x1000 * port + OFF_IER, 0);    //disable interupt
	OUTREG8(UART0_BASE + 0x1000 * port + OFF_FCR, ~UARTFCR_UUE);   //disable uart
	OUTREG8(UART0_BASE + 0x1000 * port + OFF_SIRCR, ~(SIRCR_TSIRE | SIRCR_RSIRE));   //config to uart mode
	
	//OUTREG8(UART_BASE + 0x1000 * port + OFF_LCR, UARTLCR_WLEN_8 | UARTLCR_STOP1);

	// Set Baudrate
	SETREG8(UART0_BASE + 0x1000 * port + OFF_LCR, UARTLCR_DLAB);
	OUTREG8(UART0_BASE + 0x1000 * port + OFF_DLHR, (baudrate_div >> 8) & 0xFF);
	OUTREG8(UART0_BASE + 0x1000 * port + OFF_DLLR, baudrate_div & 0xFF);
	CLRREG8(UART0_BASE + 0x1000 * port + OFF_LCR, UARTLCR_DLAB);

        //no parity
       CLRREG8((UART0_BASE + 0x1000 * port + OFF_LCR), UARTLCR_PE);

        //1stopbit
        CLRREG8((UART0_BASE + 0x1000 * port + OFF_LCR), UARTLCR_STOP);
        // 8bitdata
        SETREG8((UART0_BASE + 0x1000 * port + OFF_LCR), UARTLCR_WLEN_8);
        //flowctrl off
        OUTREG8((UART0_BASE + 0x1000 * port + OFF_MCR), 0); 
	// Enable UART
	SETREG8(UART0_BASE + 0x1000 * port + OFF_FCR, UARTFCR_UUE | UARTFCR_FE | UARTFCR_RFLS | UARTFCR_TFLS);
	
}

#endif

void uart_setbrg (int port, int baudrate)
{	
	unsigned short UART_DIVISOR_LATCH = DEVICE_CLOCK / 16 / baudrate;
      SETREG8(UART0_BASE + 0x1000 * port + OFF_LCR, UARTLCR_DLAB);
	OUTREG8(UART0_BASE + 0x1000 * port + OFF_DLHR, (UART_DIVISOR_LATCH >> 8) & 0xFF);
	OUTREG8(UART0_BASE + 0x1000 * port + OFF_DLLR, UART_DIVISOR_LATCH & 0xFF);
	CLRREG8(UART0_BASE + 0x1000 * port + OFF_LCR, UARTLCR_DLAB);
}

int uart_read_char(int port, char *chr)
{
    unsigned long int t1,t2;
    char c;

    t1 = os_TimeGet();
    while (!(INREG8((UART0_BASE + 0x1000 * port + OFF_LSR)) & UARTLSR_DR ))
    {
        t2 = os_TimeGet();

        if((t2-t1)>50)//500ms
            return -2;
    }
	
    c = INREG8(UART0_BASE + 0x1000 * port + OFF_RDR);

    *chr = c;

    return  0;
}


char uart_wait_char(int port)
{
	char lvchr;
	while (uart_read_char(port, &lvchr));
	return lvchr;
}


int uart_read_data(int port, void *buf, int len, unsigned int timeout)
{
    unsigned char *lvp = (unsigned char*)buf;
    int lvlen = 0;
    unsigned int t1,t2;

    t1 = os_TimeGet();
    
    while(lvlen < len)
    {
        if(uart_read_char(port, (char*)lvp))
        {
            t2 = os_TimeGet();
            if(timeout == TIMEOUT_NOWAIT) 
                break;
            else if(timeout == TIMEOUT_INFINITE) 
                continue;
            else if((t2-t1)*10 < timeout) 
                continue;
            else 
                break;
        }
        lvlen ++;
        lvp ++;
    }
    return lvlen;
    
 
    //return 0;
}

void uart_write_char(int port, const char chr)
{  
	while ( !(INREG8((UART0_BASE + 0x1000 * port + OFF_LSR))&UARTLSR_TDRQ ) );
    
	OUTREG8((UART0_BASE + 0x1000 * port + OFF_TDR), chr); 	
	mdelay(10);
}

int uart_send_data(int port, void *data, int len)
{
	char *lvp = (char*)data;
	int lvi = len;
//lvi=20;
	while (lvi > 0)
	{
		uart_write_char(port, *lvp ++);
		lvi --;
        //uart_write_char(1,'A');uart_write_char(1,'B');
        //uart_write_char(1,'C');
        //mdelay(1);
        //serial_putc('A');
	}
	return len;
}

