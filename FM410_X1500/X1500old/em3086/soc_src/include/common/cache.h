#ifndef __CACHE_H__
#define __CACHE_H__

void __flush_dcache_line(unsigned long addr);
void __icache_invalidate_all(void);
void __dcache_invalidate_all(void);
void __dcache_writeback_all(void);
void __dcache_writeback_all(void);
void __dcache_hitinv(unsigned int addr,unsigned int size);
void __dcache_hitwback(unsigned long addr, unsigned long size);
void dma_cache_wback_inv(unsigned long addr, unsigned long size);

#endif
