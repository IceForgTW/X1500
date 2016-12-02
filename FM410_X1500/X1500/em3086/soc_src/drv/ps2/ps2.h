#ifndef __PS2_H__
#define __PS2_H__

int PS2_Port_SendData(void *dat, int len);
int ps2_init(void);
int ps2_open(void);
int ps2_close(void);

#endif
