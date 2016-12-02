/********************************************************************
	created:	2009/01/04
	created:	4:1:2009
	filename: 	SendDataRoute.h
	author:		zencg
	
	purpose:	
*********************************************************************/

#ifndef __H_FILE_SendDataRoute__
#define __H_FILE_SendDataRoute__

/*******************************************************************/

#define SDR_PORT_USB_KBW	0
#define SDR_PORT_PS2_KBW	1

/*******************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************/

int AddDataToQueue(void *pData, int nLen, int nPortType);
int IsKBDDataSendcomplete(void);
int InitSendDataRouteTask(void);

/*******************************************************************/
#ifdef __cplusplus
	};
#endif
/*******************************************************************/

#endif		//#ifndef __H_FILE_SendDataRoute__

/************************  END OF FILE  ****************************/
