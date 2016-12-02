#ifndef __MEM_MGR_H__
#define  __MEM_MGR_H__

#define MEM_U8 unsigned char 
#define MEM_ULONG unsigned int 

void heapInit(void);
MEM_ULONG alloc(MEM_ULONG nbytes);
MEM_ULONG alignAlloc(MEM_ULONG align,MEM_ULONG size);
void deAlloc(MEM_ULONG address);
MEM_ULONG Drv_realloc(MEM_ULONG address,MEM_ULONG nbytes);
MEM_ULONG Drv_calloc(MEM_ULONG size,MEM_ULONG n);


#endif
