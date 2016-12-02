#ifndef __NOR_FLASH_H__
#define __NOR_FLASH_H__

void jz_nor_init(void);
int  jz_nor_read(unsigned int src_addr, unsigned int count,unsigned int dst_addr);
int jz_nor_write(unsigned int src_addr, unsigned int count,unsigned int dst_addr);
int jz_nor_erase(unsigned int src_addr, unsigned int count);


#endif
