
/*
 * Original code came from uClibc's malloc routines.
 * 
 *  Copyright (C) 2002  NEC Corporation
 *  Copyright (C) 2002  Miles Bader <miles@gnu.org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License.  See the file COPYING.LIB in the main
 * directory of this archive for more details.
 * 
 * Written by Miles Bader <miles@gnu.org>
 *
 * Simplified by p.w. pwang@cognio.com
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
//#include <pthread.h>
#ifdef WIN32
#include <windows.h>
#else
#include <os_api.h>
#include <x1000intc.h>
#endif //WIN32
//#include "heap_malloc.h"

// �ڴ�������
#define HEAP_LOCK_TYPE_NONE    0 // ����
#define HEAP_LOCK_TYPE_THREAD  1 // �̻߳�����
#define HEAP_LOCK_TYPE_SCHED   2 // ���������
#define HEAP_LOCK_TYPE_HW      3 // Ӳ���������ж�

#undef printf
#define printf1(a,b...)

/* The heap allocates in multiples of, and aligned to, MALLOC_ALIGN_SIZE.
   MALLOC_ALIGN_SIZE must be a power of 2. */
#define MALLOC_ALIGN_SIZE (4)
#define MALLOC_ALIGN_MASK (MALLOC_ALIGN_SIZE - 1)
#define MALLOC_ALIGN(x)   ((((unsigned int)(x)) + MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK)
/* The minimum size of a free area; if allocating memory from a free-area
   would make the free-area smaller than this, the allocation is simply
   given the whole free-area instead.  It must include at least enough room
   to hold a struct heap_free_area, plus some extra to avoid excessive heap
   fragmentation (thus increasing speed).  This is only a heuristic -- it's
   possible for smaller free-areas than this to exist (say, by realloc
   returning the tail-end of a previous allocation), but __heap_alloc will
   try to get rid of them when possible.  */
#define MALLOC_MIN_FREE_AREA_SIZE (MALLOC_ALIGN(sizeof(chunk_mgt_t)) + 32)

/*
  �����magic��Ϊ����ڴ�Խ���õģ����������ֱ���MEM_MAGIC_HEAD��MEM_MAGIC_TAIL��
  MEM_MAGIC_HEAD�ŵ��ڴ�ͷ����������ƻ�˵��ǰһ���ڴ�Խ���ƻ��˱����ڴ档
  MEM_MAGIC_TAIL�ŵ��ڴ�β����������ƻ�˵�������ڴ�Խ���ˡ�
  magic���ռ��һЩ�ڴ棬debug�ڼ���Դ�magic�������ڴ��飬
  ��ʽ����Ӧ�ùص�magic����ʡһЩ�ڴ档     */

//#define MEM_MAGIC_HEAD   0xabababab
//#define MEM_MAGIC_TAIL   0xabababab

typedef unsigned int    mem_magic_t;

typedef unsigned int    mem_align_t;

typedef struct {
#ifdef MEM_MAGIC_HEAD
	mem_magic_t magic;
#endif //MEM_MAGIC_HEAD
	unsigned int size;
	mem_align_t align;
} mem_header_t;


/* typedefs */
typedef struct chunk_mgt_s
{
	mem_header_t         head; /* size of the free block, including this overhead */
	struct chunk_mgt_s* next;
} chunk_mgt_t;

#ifdef WIN32

typedef HANDLE heap_lock_obj_t;
#else
typedef OS_EVENT * heap_lock_obj_t;
#endif

typedef struct heap_lock_t
{
	heap_lock_obj_t lock_obj;
	unsigned int lock_type;
	unsigned int lock_data;
} heap_lock_t;

typedef struct heap_mgt_s
{
	heap_lock_t    lock;
	chunk_mgt_t*   pFreeMgt;
	unsigned int   total_size;
	unsigned int   alloc_size;
} heap_mgt_t;

#define HEAP_FREE_AREA_END(pChunk) ((((char *)pChunk) + (unsigned int)(pChunk->head.size)))
#define HEAP_FREE_AREA_START(pChunk) ((void *)(pChunk))

#undef MALLOC_DEBUG
#ifdef MALLOC_DEBUG
#define DEBUGP(x...)    (printf(x))
#else
#define DEBUGP(x...)
#endif

#define BEST    (0)
#define FIRST   (1)
#define FIT     (BEST)

/* typedef */

static int heap_lock_init(heap_lock_t * lock, unsigned int lock_type)
{
	lock->lock_type = lock_type;
	
	if (lock->lock_type == HEAP_LOCK_TYPE_NONE)
	{
		
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_THREAD)
	{
		//if (*mutex == NULL)
		{
#ifdef WIN32
			lock->lock_obj = (heap_lock_obj_t)CreateSemaphore(NULL, 1, 1, NULL);
#else
			lock->lock_obj = (heap_lock_obj_t)os_SemaphoreCreate(1);
#endif
		}
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_SCHED)
	{
		
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_HW)
	{
		
	}
	else
	{
		
	}

	return 0;
}

//================================================================
// 
//   
//----------------------------------------------------------------
// Parameters:
//   
// Return value:
//   
//================================================================
static int heap_lock_deinit(heap_lock_t * lock)
{
	if (lock->lock_type == HEAP_LOCK_TYPE_NONE)
	{
		
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_THREAD)
	{
		if (lock->lock_obj != NULL)
		{
#ifdef WIN32
			CloseHandle((HANDLE)(lock->lock_obj));
#else
			unsigned char err;
			os_SemaphoreDelete((OS_EVENT*)(lock->lock_obj), OS_DELETE_ALWAYS, &err);
#endif
			lock->lock_obj = NULL;
		}
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_SCHED)
	{
		
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_HW)
	{
		
	}
	else
	{
		
	}
	
	return 0;
}

//================================================================
// 
//   
//----------------------------------------------------------------
// Parameters:
//   
// Return value:
//   
//================================================================
static int heap_lock(heap_lock_t * lock)
{
	if (lock->lock_type == HEAP_LOCK_TYPE_NONE)
	{
		
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_THREAD)
	{
#ifdef WIN32
		WaitForSingleObject((HANDLE)(lock->lock_obj), INFINITE);
#else
		unsigned char err;
		os_SemaphorePend((OS_EVENT*)(lock->lock_obj), 0, &err);
		if (err != OS_NO_ERROR)
		{
//			printf("ERROR: heap_malloc lock fail, err = %d\n", (unsigned int)err);
			return -1;
		}
#endif
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_SCHED)
	{
		os_ScheduleLock();
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_HW)
	{
		lock->lock_data = spin_lock_irqsave();
	}
	else
	{
		
	}

	return 0;
}

//================================================================
// 
//   
//----------------------------------------------------------------
// Parameters:
//   
// Return value:
//   
//================================================================
static int heap_unlock(heap_lock_t * lock)
{

	if (lock->lock_type == HEAP_LOCK_TYPE_NONE)
	{
		
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_THREAD)
	{
#ifdef WIN32
		ReleaseSemaphore((HANDLE)(lock->lock_obj), 1, NULL);
#else
		os_SemaphorePost((OS_EVENT*)(lock->lock_obj));
#endif
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_SCHED)
	{
		os_ScheduleUnlock();
	}
	else if (lock->lock_type == HEAP_LOCK_TYPE_HW)
	{
		spin_unlock_irqrestore(lock->lock_data);
	}
	else
	{
		
	}

	return 0;
}

//////////////////////////////////////////////////////////////////
// heap_print_free_list
//   ��ӡ������ڴ��free�ڴ������
//----------------------------------------------------------------
// Parameters:
//   
// Return value:
//   
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void heap_print_free_list(heap_mgt_t* heap)
{
	chunk_mgt_t* pCurr;

	pCurr = heap->pFreeMgt;
	if (pCurr == NULL)
	{
		printf("NULL-------------------------------------\n");
		return;
	}

	printf("-----------------------------------------\n");
	printf("Addr        End        Size       Next\n");
	while (pCurr != NULL)
	{
		//DEBUGP("heapFreeList:pCurr(0x%x)\n", (unsigned int)pCurr);
		printf("0x%08x 0x%08x %8d 0x%08x\n", (unsigned int)pCurr,
			(unsigned int)((unsigned int)pCurr + pCurr->head.size - 1),
			(unsigned int)pCurr->head.size, (unsigned int)(pCurr->next));
		pCurr = pCurr->next;
	}
	printf("-----------------------------------------\n");
	printf("alloc_size=%d, free_size=%d\n", heap->alloc_size, heap->total_size - heap->alloc_size);
	printf("-----------------------------------------\n");
}

//////////////////////////////////////////////////////////////////
// heap_print_space
//   ��ӡ������ڴ�ʹ�����
//----------------------------------------------------------------
// Parameters:
//   
// Return value:
//   
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void heap_print_space(heap_mgt_t* heap)
{
	printf("heap=%x, alloc_size=%d, free_size=%d\n", heap, heap->alloc_size, heap->total_size - heap->alloc_size);
}

//////////////////////////////////////////////////////////////////
// heap_malloc_init
//   ���ڴ��ʼ��
//----------------------------------------------------------------
// Parameters:
//   pMem ���ڴ��ַ
//   size ���ڴ��С 
//   lock_type ������
// Return value:
//   �ѵĲ������
// Remarks:
//   
//////////////////////////////////////////////////////////////////
heap_mgt_t* heap_malloc_init1(unsigned char * pMem, size_t size, int lock_type)
{
	heap_mgt_t*    pHeap;
	chunk_mgt_t*   pFreeMgt;
	size_t        newSize;

	/* check to see pMem is aligned already and have enough space for
	* the management overheads */
	DEBUGP("heap_malloc_init:pMem(0x%x)size(%d)\n", (unsigned int)pMem, size);
	DEBUGP("heap_malloc_init:sizeofChunkMgt(%d)sizeofHeapMgt(%d)MALLOC_ALIGN_SIZE(%d)\n",	sizeof(chunk_mgt_t), MALLOC_ALIGN(sizeof(heap_mgt_t)), MALLOC_ALIGN_SIZE);
	/* calculate the residual from an alignment, and remove it from our thingie */
	DEBUGP("heap_malloc_init:size lost due to addr(%d)\n", ((((unsigned int)pMem + MALLOC_ALIGN_MASK) & MALLOC_ALIGN_MASK) ^ MALLOC_ALIGN_MASK));
	newSize = size - ((((unsigned int)pMem + MALLOC_ALIGN_MASK) & MALLOC_ALIGN_MASK) ^ MALLOC_ALIGN_MASK);
	DEBUGP("heap_malloc_init:size lost due to size(%d)\n", ((unsigned int)newSize & MALLOC_ALIGN_MASK));
	/* now make size a nice multiple of alignment size */
	newSize = newSize - ((unsigned int)newSize & MALLOC_ALIGN_MASK);
	if ((newSize <= (MALLOC_ALIGN(sizeof(chunk_mgt_t)) + MALLOC_ALIGN(sizeof(heap_mgt_t)))))
	{
		return(NULL);
	}

/*
	  _____ ��ͷ ____   ___________________________ ������ ________________________
	 /               \ /                                                           \
	+-----------------+-------------------------------------------------------------+
	|   heap_mgt_t    |                            pFreeMgt                         |
	+-----------------+-------------------------------------------------------------+
	                   \________________________ pFreeMgt->size ___________________/
*/
	/* put the heap management at the beginning chunk of the memory */
	pHeap = (heap_mgt_t *)MALLOC_ALIGN(pMem);
	DEBUGP("heap_malloc_init:pHeap(0x%x)size(%d)\n", (unsigned int)pHeap, (unsigned int)newSize);
	
	/* init mutex */
	if (heap_lock_init(&(pHeap->lock), lock_type) != 0)
	{
		return(NULL);
	}

	/* initialize the first free block'o memory */
	pFreeMgt = (chunk_mgt_t *)((unsigned char *)pHeap + MALLOC_ALIGN(sizeof(heap_mgt_t)));
	DEBUGP("heap_malloc_init:pFreeMgt(0x%x)\n", (unsigned int)pFreeMgt);
	pHeap->pFreeMgt = pFreeMgt;

	pHeap->total_size = newSize - MALLOC_ALIGN(sizeof(heap_mgt_t));
	pHeap->alloc_size = 0;
	
	/* initialize the free list */
	pFreeMgt->head.size = newSize - MALLOC_ALIGN(sizeof(heap_mgt_t));
	pFreeMgt->next = NULL;

	return(pHeap);
}

//////////////////////////////////////////////////////////////////
// heap_malloc_init
//   ���ڴ��ʼ��
//----------------------------------------------------------------
// Parameters:
//   pMem ���ڴ��ַ
//   size ���ڴ��С 
// Return value:
//   �ѵĲ������
// Remarks:
//   
//////////////////////////////////////////////////////////////////
heap_mgt_t* heap_malloc_init( unsigned char * pMem, size_t size)
{
	return heap_malloc_init1(pMem, size, HEAP_LOCK_TYPE_THREAD);
}

//////////////////////////////////////////////////////////////////
// heap_malloc_deinit
//   ���ڴ淴��ʼ��
//----------------------------------------------------------------
// Parameters:
//   heap �Ѳ������
// Return value:
//   
// Remarks:
//   
//////////////////////////////////////////////////////////////////
/* init routine */
void heap_malloc_deinit(heap_mgt_t* heap)
{
	heap_lock_deinit(&(heap->lock));
}

//================================================================
// _heap_malloc_align
//   �Ӷ�������СΪsize�ֽڵ��ڴ棬�ɷ����ַ������ڴ�
//----------------------------------------------------------------
// Parameters:
//   heap �Ѳ������
//   size �����ڴ��С
//   align �ڴ����ֵ������Ϊ0��0��ʾ����Ҫ����
// Return value:
//   ������ڴ��ַ��������䲻�ɹ�������NULL
//================================================================
static void* _heap_malloc_align( heap_mgt_t* heap, size_t size, size_t align)
{
	chunk_mgt_t*   pPrev;
	chunk_mgt_t*   pCurr;
	void*         pMem;
	char*         pMem1;
#if (FIT == BEST)
	unsigned int  bestSize;
	chunk_mgt_t*   pBestCurr;
	chunk_mgt_t*   pBestPrev;
#endif

/*
	  ___ ǰMAGIC ___   ___ �ڴ��ͷ ___   ______________ �û��ڴ��� ______________   ___ ��MAGIC ___
	 /               \ /                \ /                                        \ /               \
	+-----------------+------------------+------------------------------------------+-----------------+
	|  MEM_MAGIC_HEAD |   mem_header_t   |                 USER MEM                 |  MEM_MAGIC_TAIL |
	+-----------------+------------------+------------------------------------------+-----------------+
	MEM_MAGIC_HEAD �� MEM_MAGIC_TAIL ��������ڴ���Ƿ��ƻ���
	��� MEM_MAGIC_HEAD ֵ����ȷ����ʾ���ڴ��ǰһ���ڴ��Խ���ƻ��˱��ڴ�顣
	��� MEM_MAGIC_TAIL ֵ����ȷ����ʾ���ڴ�Խ�磬���ܻ��ƻ���һ���ڴ�顣
*/

	if (size == 0) size = 1; //��׼����sizeΪ0���ڴ�
	
	/* check the parameters a bit */
	if (size <= 0)
	{
		return(NULL);
	}

	align = MALLOC_ALIGN(align);

	/* take account into the per malloc overhead */
	size = MALLOC_ALIGN(size) + MALLOC_ALIGN(sizeof(mem_header_t)) + align;
#ifdef MEM_MAGIC_TAIL
	size += MALLOC_ALIGN(sizeof(mem_magic_t));
#endif

	/* make sure size is bigger than our possible overhead */
	if (size < (MALLOC_ALIGN(sizeof(chunk_mgt_t))))
	{
		size = MALLOC_ALIGN(sizeof(chunk_mgt_t));
	}

	DEBUGP("heap_malloc:size(%d)\n", size);

	/* look for a free area that can contain size */
	/* prime the search */
	pPrev = pCurr = heap->pFreeMgt;
#if (FIT == BEST)
	bestSize = ~0;
	pBestCurr = pBestPrev = NULL;
	/* start the search.  Exit condition also in the middle */
	while (pCurr != NULL)
	{
		DEBUGP("heap_malloc:.\n");
		/* check to see if this chunk is a perfect fit */
		if (pCurr->head.size == size)
		{
			pBestCurr = pCurr;
			pBestPrev = pPrev;
		}
		else if (pCurr->head.size > size)
		{
			/* check to see if it's better than the one we've got */
			if (bestSize > pCurr->head.size)
			{
				bestSize = pCurr->head.size;
				pBestCurr = pCurr;
				pBestPrev = pPrev;
			}
		}
		pPrev = pCurr;
		pCurr = pCurr->next;
	}
	pCurr = pBestCurr;
	pPrev = pBestPrev;
#else
	/* start the search.  Exit condition also in the middle */
	while ((pCurr != NULL) && (pCurr->head.size < size))
	{
		pPrev = pCurr;
		pCurr = pCurr->next;
	}
#endif

	DEBUGP("heap_malloc:pCurr(0x%x)pPrev(0x%x)\n", (unsigned int)pCurr, (unsigned int)pPrev);

	/* see if it didn't find one */
	if (pCurr == NULL)
	{
		return(NULL);
	}

	/* now check to see if we shouldn't fragment this free space */
	if (pCurr->head.size < (size + MALLOC_MIN_FREE_AREA_SIZE))
	{
		/* don't fragment - remove pCurr from free list*/
		/* check to see if we're removing from root */
		if (pPrev == pCurr)
		{
			/* remove root  */
			heap->pFreeMgt = NULL;
		}
		else /* non-root removal */
		{
			pPrev->next = pCurr->next;
		}

		/* find the beginning of the memory space */
		pMem = HEAP_FREE_AREA_START(pCurr);
	}
	else /* fragment */
	{
		/* find the beginning of the memory space */
		pMem = (void *)(HEAP_FREE_AREA_END(pCurr) - size);

		pCurr->head.size -= size;

		/* set up the memory header */
		((mem_header_t *)pMem)->size = size;
#ifdef MEM_MAGIC_HEAD
		((mem_header_t *)pMem)->magic = MEM_MAGIC_HEAD;
#endif //MEM_MAGIC_HEAD
	}

#ifdef MEM_MAGIC_TAIL
	mem_magic_t * magic_tail = (mem_magic_t*)((char *)pMem + ((mem_header_t *)pMem)->size - MALLOC_ALIGN(sizeof(mem_magic_t)));
	*magic_tail = MEM_MAGIC_TAIL;
#endif //MEM_MAGIC_TAIL

	/* calculate the new pointer to free space - skip over size */
	pMem = (char *)pMem + MALLOC_ALIGN(sizeof(mem_header_t));
	pMem1 = (char *)pMem + MALLOC_ALIGN(align);
	*(mem_align_t*)((char *)pMem1 - MALLOC_ALIGN(sizeof(mem_align_t))) = ((unsigned int)pMem1 - (unsigned int)pMem);

	heap->alloc_size += size;

	DEBUGP("heap_malloc:done(0x%x)\n", (unsigned int)pMem1);
	return((void *)pMem1);
}


//================================================================
// _heap_mem_size
//   ȡ�ڴ���С
//----------------------------------------------------------------
// Parameters:
//   heap �Ѳ������
//   pMem
// Return value:
//   
//================================================================
static unsigned int _heap_mem_size( heap_mgt_t* heap, void * pMem)
{
	void*       pOrigMem;
	chunk_mgt_t* pNode;

	/* find size */
	mem_align_t align = *((mem_align_t*) ((char *)pMem - MALLOC_ALIGN(sizeof(mem_align_t))));
	pOrigMem = (char *)pMem - MALLOC_ALIGN(sizeof(mem_header_t)) - align;
	pNode = (chunk_mgt_t *)pOrigMem;

	/* find the end of this block */
	return pNode->head.size;
}

//================================================================
// _heap_free
//   �Ӷ��ͷ��ڴ�
//----------------------------------------------------------------
// Parameters:
//   heap �Ѳ������
//   pMem Ҫ�ͷŵ��ڴ��ַ
// Return value:
//   
//================================================================
static void _heap_free( heap_mgt_t* heap, void* pMem)
{
	void*       pOrigMem;
	void*       pEnd;
	chunk_mgt_t* pCurr;
	chunk_mgt_t* pPrev;
	chunk_mgt_t* pNode;
	mem_align_t align;
#ifdef MEM_MAGIC_TAIL
	mem_magic_t* magic_tail
#endif //MEM_MAGIC_TAIL

	DEBUGP("heap_free:pMem(0x%x)\n", (unsigned int)pMem);

	/* sanity check the parameter a bit */
	if (pMem == NULL)
	{
		return;
	}

	/* find size */
	align = *((mem_align_t*) ((char *)pMem - MALLOC_ALIGN(sizeof(mem_align_t))));
	pOrigMem = (char *)pMem - MALLOC_ALIGN(sizeof(mem_header_t)) - align;
	pNode = (chunk_mgt_t *)pOrigMem;

#ifdef MEM_MAGIC_HEAD
	//check magic flag
	if (pNode->head.magic != MEM_MAGIC_HEAD)
	{
		printf("WARNING: _heap_free magic_head is damaged, magic_head=%x, mem_addr=%x\n", pNode->head.magic, pMem);
	}
#endif //MEM_MAGIC_HEAD

#ifdef MEM_MAGIC_TAIL
	magic_tail = (mem_magic_t*)((char *)pOrigMem + ((mem_header_t *)pOrigMem)->size - MALLOC_ALIGN(sizeof(mem_magic_t))) ;
	if ((*magic_tail) != MEM_MAGIC_TAIL)
	{
		printf("WARNING: _heap_free magic_tail is damaged, magic_tail=%x, mem_addr=%x\n", magic_tail, pMem);
	}
#endif //MEM_MAGIC_TAIL

	/* sanity check the size a little */
	if (pNode->head.size <= 0)
	{
		printf("ERROR: _heap_free size is invalid, size=%d\n", pNode->head.size);
		return;
	}

	/* find the end of this block */
	pEnd = (char *)pNode + pNode->head.size;
	
	heap->alloc_size -= pNode->head.size;

	/* linear search the free list to see where we can insert this */
	/* prime it */
	pPrev = pCurr = heap->pFreeMgt;
	while ((pCurr != NULL) && (pCurr < pNode))
	{
		pPrev = pCurr;
		pCurr = pCurr->next;
	}

	DEBUGP("heap_free:pPrev(0x%x)pCurr(0x%x)size(%d)\n",
		(unsigned int)pPrev, (unsigned int)pCurr, pNode->size);

	DEBUGP("heap_free:pEnd(0x%x)HEAP_FREE_AREA_START(pCurr)(0x%x)\n",
		(unsigned int)pEnd, (unsigned int)HEAP_FREE_AREA_START(pCurr));
	/* check to see if we can merge */
	if ((pCurr != NULL) && (pEnd == HEAP_FREE_AREA_START(pCurr)))
	{
		/* they're next to each other, we expand pNode */
		pNode->head.size += pCurr->head.size;
		pNode->next = pCurr->next;
	}
	else
	{
		/* update the pointers, get ready to insert into list */
		pNode->next = pCurr;
	}

	/* check to see if we're inserting at nil or root*/
	if (pPrev == pCurr)
	{
		heap->pFreeMgt = pNode;
	}
	else /* insert into body */
	{
		DEBUGP("heap_free:HEAP_FREE_AREA_END(pPrev)(0x%x)HEAP_FREE_AREA_START(pNode)(0x%x)\n",
			(unsigned int)HEAP_FREE_AREA_END(pPrev), (unsigned int)HEAP_FREE_AREA_START(pNode));
		/* check to see if we can further merge */
		if (HEAP_FREE_AREA_END(pPrev) == HEAP_FREE_AREA_START(pNode))
		{
			/* collect the two blocks */
			pPrev->head.size += pNode->head.size;
			pPrev->next = pNode->next;
		}
		else
		{
			pPrev->next = pNode;
		}
	}

	DEBUGP("heap_free:done\n");
}

//////////////////////////////////////////////////////////////////
// heap_malloc
//   �Ӷ�����䳤��Ϊsize�ֽڵ��ڴ�
//----------------------------------------------------------------
// Parameters:
//   heap �Ѳ������
//   size �����ڴ��С
// Return value:
//   ������ڴ��ַ��������䲻�ɹ�������NULL
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void* heap_malloc( heap_mgt_t* heap, size_t size)
{
	void * addr;
	int lock_err = 0;
	unsigned int irq_save;
	
	/* mutex lock here */
	lock_err = heap_lock(&(heap->lock));
	if (lock_err != 0)
	{
		// XXX - we should fatal here 
//		return NULL;
		irq_save = spin_lock_irqsave();
	}

	addr = _heap_malloc_align( heap, size, 0 );

	if (lock_err == 0)
	{
		if (heap_unlock(&(heap->lock)) != 0)
		{
			/* XXX - we should fatal here */
		}
	}
	else
	{
		spin_unlock_irqrestore(irq_save);
	}
	return addr;
}

//////////////////////////////////////////////////////////////////
// heap_malloc_align
//   �Ӷ�����䳤��Ϊsize�ֽڣ���ַ��align������ڴ�
//----------------------------------------------------------------
// Parameters:
//   heap �Ѳ������
//   size �����ڴ��С
//   align ��ַ�����ֵ
// Return value:
//   ������ڴ��ַ��������䲻�ɹ�������NULL
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void* heap_malloc_align( heap_mgt_t* heap, size_t align, size_t size)
{
	void * addr;
	int lock_err = 0;
	unsigned int irq_save;
	
	/* mutex lock here */
	lock_err = heap_lock(&(heap->lock));
	
	if (lock_err != 0)
	{
		// XXX - we should fatal here 
		//return NULL;
		irq_save = spin_lock_irqsave();
	}

	addr = _heap_malloc_align( heap, size, align );

	if (lock_err == 0)
	{
		if (heap_unlock(&(heap->lock)) != 0)
		{
			/* XXX - we should fatal here */
		}
	}
	else
	{
		spin_unlock_irqrestore(irq_save);
	}
	
	return addr;
}

//////////////////////////////////////////////////////////////////
// heap_calloc
//   �ڶ������elem_count������Ϊelem_size�������ռ�
//----------------------------------------------------------------
// Parameters:
//   heap �Ѳ������
// Return value:
//   ������ڴ��ַ��������䲻�ɹ�������NULL
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void* heap_calloc( heap_mgt_t* heap, size_t elem_count, size_t elem_size)
{
	size_t size = elem_count * elem_size;
	void * addr;
	int lock_err = 0;
	unsigned int irq_save;
	
	/* mutex lock here */
	lock_err = heap_lock(&(heap->lock));
	
	if (lock_err != 0)
	{
		// XXX - we should fatal here 
		//return NULL;
		irq_save = spin_lock_irqsave();
	}

	addr = _heap_malloc_align( heap, size, 0 );

	if (lock_err == 0) 
	{
		if (heap_unlock(&(heap->lock)) != 0)
		{
			/* XXX - we should fatal here */
		}
	} else {
		spin_unlock_irqrestore(irq_save);
	}

	if (addr != NULL)
	{
		memset(addr, 0, size);
	}
	
	return addr;
}

//////////////////////////////////////////////////////////////////
// heap_realloc
//   ���ͷ�ԭ��old_mem��ָ�ڴ����򣬲�����new_sizeָ���Ĵ�С���·���ռ䣬
//   ͬʱ��ԭ�����ݴ�ͷ��β���Ƶ��·�����ڴ����򣬲����ظ��ڴ�������׵�ַ��
//----------------------------------------------------------------
// Parameters:
//   heap �Ѳ������
//   old_mem ԭ�ڴ��ַ
//   new_size Ҫ���·���Ŀռ�
// Return value:
//   ������ڴ��ַ��������䲻�ɹ�������NULL
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void* heap_realloc(heap_mgt_t* heap, void* old_mem, size_t new_size)
{
	void * addr;
	int lock_err = 0;
	unsigned int irq_save=0;
	
	/* mutex lock here */
	lock_err = heap_lock(&(heap->lock));
	
	if (lock_err != 0)
	{
		// XXX - we should fatal here 
		//return NULL;
		irq_save = spin_lock_irqsave();
	}

	addr = _heap_malloc_align( heap, new_size, 0 );
	if (old_mem != NULL)
	{
		if (addr) 
		{
			unsigned int old_size = _heap_mem_size(heap, old_mem);
			size_t s = (old_size > new_size) ? new_size : old_size;
			memcpy(addr, old_mem, s);
		}
		_heap_free( heap, old_mem );
	}

	if (lock_err == 0)
	{
		if (heap_unlock(&(heap->lock)) != 0)
		{
			/* XXX - we should fatal here */
		}
	}
	else
	{
		spin_unlock_irqrestore(irq_save);
	}
	return addr;
}

//////////////////////////////////////////////////////////////////
// heap_free
//   �ͷ��ڴ�
//----------------------------------------------------------------
// Parameters:
//   heap �Ѳ������
//   pMem Ҫ�ͷŵ��ڴ�
// Return value:
//   
// Remarks:
//   
//////////////////////////////////////////////////////////////////
void heap_free( heap_mgt_t* heap, void* pMem)
{
	int lock_err = 0;
	unsigned int irq_save;
	
	if (pMem == NULL) return;
	
	/* mutex lock here */
	lock_err = heap_lock(&(heap->lock));
	
	if (lock_err != 0)
	{
		// XXX - we should fatal here 
		//return NULL;
		irq_save = spin_lock_irqsave();
	}

	_heap_free( heap, pMem );

	if (lock_err == 0)
	{
		if (heap_unlock(&(heap->lock)) != 0)
		{
			/* XXX - we should fatal here */
		}
	}
	else
	{
		spin_unlock_irqrestore(irq_save);
	}
}

