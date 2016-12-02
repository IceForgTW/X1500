/********************** BEGIN LICENSE BLOCK ************************************
 *
 * JZ4755  mobile_tv  Project  V1.0.0
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
 

#include <x1000.h>
#include <bsp.h>
#include <os_api.h>
#include <pm.h>
void  StatHookInit (void)
{

}
int count_sec=0;

int OSTaskStatHook_x1000 (void)
{
    return 0;

}

int OSTaskIdleHook_x1000(void)
{
	jz_pm_idle();
    return 0;
}

void OSTimeTick_x1000(void)
{
    os_TimerSignal();
}

OSBTaskBSPHook x1000BspHook = {
	0,
	0,
	0,
	0,
	0,//OSTaskIdleHook_x1000,
	0,//OSTaskStatHook_x1000,
	0,
	0,
	OSTimeTick_x1000,
	0
};

