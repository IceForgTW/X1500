/********************************************************************
	created:	2009/01/04
	created:	4:1:2009   10:19
	filename: 	D:\Build\HR200-uImage-3.05.013\bsp\MB2027\src\devs\kbw\SendDataRoute.c
	file path:	D:\Build\HR200-uImage-3.05.013\bsp\MB2027\src\devs\kbw
	file base:	SendDataRoute
	file ext:	c
	author:		zencg
	
	purpose:	
*********************************************************************/

#include <os_api.h>
#include "x1000.h"
#include <em3000_cfg.h>
#include <SendDataRoute.h>
#include "SendDataRoute_st.h"

//////////////////////////////////////////////////////////////////////////
#define malloc(x) alloc(x)
#define free(x) deAlloc(x)

static SD_QUEUE_NODE *_s_queue_head = NULL;
static SD_QUEUE_NODE *_s_queue_last = NULL;

static OS_EVENT *_s_queue_evt;

static int _s_send_complete = 1;
static int _s_buf_len = 0;
#define MAX_KBW_COUNT			(100*1024)

static int nCEncoding = 0;
#define TASK_SENDOUT_STK_SIZE 1024*16
static OS_TASK_STACK sendout_task_stack[TASK_SENDOUT_STK_SIZE];
//////////////////////////////////////////////////////////////////////////

//int PS2_Port_SendData(void *dat, int len);

//////////////////////////////////////////////////////////////////////////
int IsKBDDataSendcomplete(void)
{
	return _s_send_complete;
}

void Set_KBW_Character_Encoding(int nMode)
{
	nCEncoding = nMode;
}


static SD_QUEUE_NODE *GetDataFromQueue(void)
{
	SD_QUEUE_NODE *node = NULL;
	//IrqLock();
	if(_s_queue_head)
	{
		node = _s_queue_head;
		_s_queue_head = node->Next;
		if(!_s_queue_head)		//end of queue
		{
			_s_queue_last = NULL;
		}
	}
	//IrqUnLock();
	return node;
}

static void SendDataRouteTask(void *pParam)
{
	SD_QUEUE_NODE *node;
      unsigned char err;
      
	while(1)
	{
		os_SemaphorePend(_s_queue_evt, 0, &err);

		node = GetDataFromQueue();
		_s_send_complete = 0;
		while(node)
		{
// Dbg(0, "%d \r\n", _s_buf_len);

			switch (node->nPortType)
			{
			case SDR_PORT_USB_KBW:
                          //printf("BBB\r\n");
				send_key_str((char*)node->pData, node->nLen, node->nCharEncoding);
				//printf("CCC\r\n");
				break;
			case SDR_PORT_PS2_KBW:
				PS2_Port_SendData(node->pData, node->nLen);
				break;
			default:			//Unknow or unsupport port type
				break;
			}		
			_s_buf_len -= node->nLen;
                   free(node);
			node = GetDataFromQueue();
		}
		_s_send_complete = 1;
	}
}

int InitSendDataRouteTask(void)
{
	static int _s_init_flag = 0;

	if(_s_init_flag)
		return -1;			//already inited
		
	_s_queue_head = _s_queue_last = NULL;
      _s_queue_evt = os_SemaphoreCreate(0);
      
      os_TaskCreate(SendDataRouteTask, 
		(void *)0, 
		(void*)&sendout_task_stack[TASK_SENDOUT_STK_SIZE -1], 
		TASK_SENDDATAROUTE_PRIO);

	_s_init_flag = 1;
	return 0;
}

int AddDataToQueue(void *pData, int nLen, int nPortType)
{
	SD_QUEUE_NODE *node;
	
	if(!pData || nLen <= 0)
		return -1;

	if(_s_buf_len+nLen>MAX_KBW_COUNT)
	{
// 		Dbg(0, "max: %d > %d \r\n", (_s_buf_len+nLen), MAX_KBW_COUNT);
		return -2;
	}

	node = (SD_QUEUE_NODE*)malloc(sizeof(SD_QUEUE_NODE) + nLen + 4);
	if(!node)
		return -3;		//Not enough memory
	node->pData = (void*)(((char*)node) + sizeof(SD_QUEUE_NODE));
	node->nLen = nLen;
	node->nPortType = nPortType;
	node->Next = NULL;
	node->nCharEncoding = nCEncoding;
	nCEncoding = 0;

	memcpy(node->pData, pData, nLen);

	//IrqLock();
	if(_s_queue_last)
	{
		_s_queue_last->Next = node;
		_s_queue_last = node;
	}
	else
	{
		_s_queue_head = _s_queue_last = node;
	}
	_s_buf_len += nLen;
	//IrqUnLock();

	os_SemaphorePost(_s_queue_evt);
	return 0;
}




