/********************************************************************
	created:	2009/01/04
	created:	4:1:2009
	filename: 	SendDataRoute.h
	author:		zencg
	
	purpose:	
*********************************************************************/

#ifndef __H_FILE_SendDataRoute_st__
#define __H_FILE_SendDataRoute_st__

/*******************************************************************/

typedef struct _sd_queue_node_ 
{
	struct _sd_queue_node_ *Next;
	int nPortType;
	void *pData;
	int nLen;
	int nCharEncoding;
}SD_QUEUE_NODE;


/*******************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************/

/*******************************************************************/
#ifdef __cplusplus
	};
#endif
/*******************************************************************/

#endif		//#ifndef __H_FILE_SendDataRoute_st__

/************************  END OF FILE  ****************************/

