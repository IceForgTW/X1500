#ifndef __UARTMC_H__
#define __UARTMC_H__
void uart_get_parameter(void *param);
void uart_set_parameter(void *param);
int comm_read(void *buf, unsigned int len, unsigned int timeout);
int comm_write(void * buff, unsigned int size);
void UartInit(void);


#endif
