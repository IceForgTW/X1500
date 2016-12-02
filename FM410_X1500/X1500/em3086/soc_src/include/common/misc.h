#ifndef __MISC_H__
#define __MISC_H__

void init_gpio(void);
void led_on(int nled);
void led_off(int nled);
int get_key_state(void);
void Init_CRC32_Table(void); 
unsigned int Get_CRC32(unsigned char *pdata, int len); 


#endif

