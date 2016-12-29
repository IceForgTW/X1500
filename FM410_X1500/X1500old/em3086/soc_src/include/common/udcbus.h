/********************** BEGIN LICENSE BLOCK ************************************
 *
 * JZ4740  mobile_tv  Project  V1.0.0
 * INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Copyright (c) Ingenic Semiconductor Co. Ltd 2005. All rights reserved.
 * 
 * This file, and the files included with this file, is distributed and made 
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS 
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * 
 * http://www.ingenic.cn 
 *
 ********************** END LICENSE BLOCK **************************************
 *
 *  Author:  <dsqiu@ingenic.cn>  <jgao@ingenic.cn> 
 *
 *  Create:   2008-06-26, by dsqiu
 *            
 *  Maintain: 2008-06-26, by jgao
 *            
 *
 *******************************************************************************
 */


#ifndef __UDCBUS_H__
#define __UDCBUS_H__

#include "usb.h"
#include <os_api.h>
#define UDC_IDLE    0
#define UDC_JUDGE   0x100
#define UDC_REMOVE  0x101

#define UDC_RESET     0x200
#define UDC_SUSPEND   0x201

#define UDC_FULLSPEED 0x300
#define UDC_HIGHSPEED 0x301

#define UDC_SETUP_PKG_FINISH 0x400

#define UDC_PROTAL_SEND_FINISH 0x500
#define UDC_PROTAL_RECEIVE_FINISH 0x501


#define USB_DIRECT_OUT       0x80
#define USB_DIRECT_IN        0

typedef struct
{
	unsigned char ep;
	USB_ENDPOINT_TYPE ep_type;
	unsigned short max_pkg;
}PIPE,*PPIPE;

typedef unsigned int (*PFN_BusNotify)(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len);

typedef struct
{
	void (*StartTransfer)(unsigned int handle,unsigned char ep, unsigned char *buf,unsigned int len);
	void (*InitEndpointSuppost)(unsigned int handle,unsigned char *ep,USB_ENDPOINT_TYPE ep_type,unsigned short *ep_max_pkg, unsigned char *buf);
	PFN_BusNotify BusNotify;
	void (*EnableDevice)(unsigned int handle);
	void (*DisableDevice)(unsigned int handle);
	void (*SetAddress)(unsigned int handle,unsigned short value);
	void (*SetConfig)(unsigned int handle);
	void (*EndPointStall)(unsigned int handle,unsigned char ep,unsigned char isStall);
	
	unsigned int DeviceName;
	OS_EVENT * bus_event;
}UDC_BUS,*PUDC_BUS;
PUDC_BUS CreateDevice(PFN_BusNotify busNotify);
void CloseDevice(void);
unsigned int BusNotify(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len);
void InitUDC(PUDC_BUS pBus);
void DeInitUDC(void);
#endif //__UDCBUS_H__
