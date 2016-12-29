
#ifndef __HEAP_MALLOC_H__
#define __HEAP_MALLOC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct heap_mgt_s;
typedef struct heap_mgt_s heap_mgt_t;

// 内存锁类型
#define HEAP_LOCK_TYPE_NONE    0 // 无锁
#define HEAP_LOCK_TYPE_THREAD  1 // 线程互斥锁
#define HEAP_LOCK_TYPE_SCHED   2 // 任务调度锁
#define HEAP_LOCK_TYPE_HW      3 // 硬件锁，关中断

//////////////////////////////////////////////////////////////////
// heap_malloc_init
//   堆内存初始化
//----------------------------------------------------------------
// Parameters:
//   pMem 堆内存地址
//   size 堆内存大小 
// Return value:
//   堆的操作句柄
// Remarks:
//   
//////////////////////////////////////////////////////////////////
heap_mgt_t* heap_malloc_init(unsigned char * pMem, size_t size);


//////////////////////////////////////////////////////////////////
// heap_malloc_init
//   堆内存初始化
//----------------------------------------------------------------
// Parameters:
//   pMem 堆内存地址
//   size 堆内存大小 
//   lock_type 锁类型
// Return value:
//   堆的操作句柄
// Remarks:
//   
//////////////////////////////////////////////////////////////////
heap_mgt_t* heap_malloc_init1(unsigned char * pMem, size_t size, int lock_type);


//////////////////////////////////////////////////////////////////
// heap_malloc_deinit
//   堆内存反初始化
//----------------------------------------------------------------
// Parameters:
//   heap 堆操作句柄
// Return value:
//   
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void heap_malloc_deinit(heap_mgt_t* heap);


//////////////////////////////////////////////////////////////////
// heap_malloc
//   从堆里分配长度为size字节的内存
//----------------------------------------------------------------
// Parameters:
//   heap 堆操作句柄
//   size 申请内存大小
// Return value:
//   分配的内存地址，如果分配不成功，返回NULL
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void* heap_malloc(heap_mgt_t* heap, size_t size);

//////////////////////////////////////////////////////////////////
// heap_malloc_align
//   从堆里分配长度为size字节，地址按align对齐的内存
//----------------------------------------------------------------
// Parameters:
//   heap 堆操作句柄
//   size 申请内存大小
//   align 地址对齐的值
// Return value:
//   分配的内存地址，如果分配不成功，返回NULL
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void* heap_malloc_align(heap_mgt_t* heap, size_t align, size_t size);

//////////////////////////////////////////////////////////////////
// heap_calloc
//   在堆里分配elem_count个长度为elem_size的连续空间
//----------------------------------------------------------------
// Parameters:
//   heap 堆操作句柄
// Return value:
//   分配的内存地址，如果分配不成功，返回NULL
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void* heap_calloc(heap_mgt_t* heap, size_t elem_count, size_t elem_size);

//////////////////////////////////////////////////////////////////
// heap_realloc
//   先释放原来old_mem所指内存区域，并按照new_size指定的大小重新分配空间，
//   同时将原有数据从头到尾复制到新分配的内存区域，并返回该内存区域的首地址。
//----------------------------------------------------------------
// Parameters:
//   heap 堆操作句柄
//   old_mem 原内存地址
//   new_size 要重新分配的空间
// Return value:
//   分配的内存地址，如果分配不成功，返回NULL
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void* heap_realloc(heap_mgt_t* heap, void* old_mem, size_t size);

//////////////////////////////////////////////////////////////////
// heap_free
//   释放内存
//----------------------------------------------------------------
// Parameters:
//   heap 堆操作句柄
//   pMem 要释放的内存
// Return value:
//   
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void heap_free(heap_mgt_t* heap, void* pMem);

//////////////////////////////////////////////////////////////////
// heap_print_free_list
//   打印输出堆内存的free内存块链表
//----------------------------------------------------------------
// Parameters:
//   
// Return value:
//   
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void heap_print_free_list(heap_mgt_t* heap);

//////////////////////////////////////////////////////////////////
// heap_print_space
//   打印输出堆内存使用情况
//----------------------------------------------------------------
// Parameters:
//   
// Return value:
//   
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void heap_print_space(heap_mgt_t* heap);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__HEAP_MALLOC_H__

