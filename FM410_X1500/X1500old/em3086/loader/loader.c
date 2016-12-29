/*
 *  XBurst  mobile_tv  Project  V1.0.0
 *  Copyright (C) 2006 - 2007 Ingenic Semiconductor Inc.
 *  Author: <xujian@nlscan.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  2016-4-10, xujian
 *  Created.
 */

#include <bsp.h>
#include <os_api.h>
#include "function.h"
#include <x1000.h>
#include <nor_flash.h>
#include <generate.h>
#include <cache.h>
#include <time-tcu.h>
#undef printf
#define	LOADER_TASK_PRI		10
#define	TASK_STK_SIZE		1024 * 4

#define SFC_APP_ADDR    0x80000
#define SFC_APP_SIZE    0x300000
#define APP_ENTREY      0x80004000
static	OS_TASK_STACK		LoaderTaskStk[TASK_STK_SIZE];

unsigned char os_malloc_completed = 0;
unsigned char os_init_completed = 0;

extern int check_download(int port);
extern int download_uart(int port);

// 应用程序入口函数
static void (*minios_entry)();

#define UNCACHE_ADDR(addr)   (((unsigned int)(addr)) | 0xa0000000)

void LoaderTask(void *data)
{
    unsigned int addr;
    int lvi;
    
//    printf("loader.c:LoaderTask...\n");

    
    JZ_StartTicker(OS_TICKS_PER_SEC);  
    
    Init_CRC32_Table();
    jz_nor_init();

//    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n");
// printf("+++8k+++\r\n");
    while(check_download(0))
    {
        lvi = download_uart(0);
        if(lvi > 0)
        {
            mdelay(1000);		//wait next download
		}
		if(lvi < 0)	mdelay(1000);		//download have error, try again
		else if(lvi == 0){
			//send_err_msg("download finish\r\n");
			break;						//download finish.
		}
	}
 //printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n");  
        // 将应用程序读入内存
    __dcache_writeback_all();

    addr = UNCACHE_ADDR(APP_ENTREY);
	jz_nor_read(SFC_APP_ADDR,  SFC_APP_SIZE,  addr);

	// 跳转至应用程序入口
    minios_entry = (void *)APP_ENTREY;
	cli();
	__dcache_writeback_all();
	__icache_invalidate_all();

	minios_entry();
}

void StartLoaderTask(void)
{
//	printf("loader.c:StartLoaderTask...\n");
    unsigned int  size = TASK_STK_SIZE;
    OS_TASK_STACK *ptos = &LoaderTaskStk[TASK_STK_SIZE - 1];
    OS_TASK_STACK *pbos = &LoaderTaskStk[0];

    os_TaskCreateEx(LoaderTask,
                    (void *)0,
                    ptos,
                    LOADER_TASK_PRI,
                    LOADER_TASK_PRI,
                    pbos,
                    size,
                    (void *)0,
                    OS_TASK_CREATE_EX_STK_CHK | OS_TASK_CREATE_EX_STK_CLR);
}

// 主程序
void APP_vMain(void)
{
// 	printf("loader.c:APP_vMain\n");
// 	printf("StartLoaderTask...\n");
	StartLoaderTask();
//	printf("os_SystemStart...\n");
	os_SystemStart();                              /* Start multitasking */

	while(1);
}
// END
