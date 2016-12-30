/********************** BEGIN LICENSE BLOCK **********************************
 *
 * INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Copyright Ingenic Semiconductor Co. Ltd 2009. All rights reserved.
 * 
 * This file, and the files included with this file, is distributed and made 
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS 
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * 
 * http://www.ingenic.cn 
 *
 ********************** END LICENSE BLOCK ***********************************/

//#include <mobile_tv.h>
#include <os_api.h>
#include "math.h"

#include <mipsregs.h>
#include <x1000.h>

#include <function.h>
#include <excpt.h>
#include <em3000_cfg.h>
#include <list.h>


#include <cmos.h>
#include <usb.h>
#include <gpio.h>
#include <i2c.h>
#include <nor_flash.h>
#include <time-tcu.h>
#include <uartmc.h>
#include <misc.h>
#include <watchdog.h>
#include <string.h>
#include "SendDataRoute.h"
#undef printf
#include <stdio.h>
#include "uartmc.h"

#define Wacthdog_Timeout 6000


extern OSBTaskBSPHook x1000BspHook;

unsigned char os_init_completed = 0;
unsigned char os_malloc_completed = 0;
unsigned short dev_magic;

static unsigned int		g_save_register[2];
static OS_TASK_STACK	TaskStartStk[TASK_MAIN_STK_SIZE]; //size is DWORD
static unsigned char rsa_cpuid[CPUID_LEN] ;
static unsigned char efuse_cpuid[CPUID_LEN] __attribute__((aligned (4)));
//unsigned char CPUID[16];
static unsigned short rsa_cpuid_crc16;
static unsigned short efuse_cpuid_crc16;

extern int efuse_read_chipID(unsigned int *buf);
extern void rsa_get_cpuid(char *plaintext_buf);
extern unsigned int crc_cal_by_bit(unsigned char* ptr, unsigned int len);
extern int InitExcept(unsigned int x);
extern void os_SetBspInterface(POSTaskBSPHook pBSP);
extern osINTFACE* GetDecodeOSIntf(void);
extern int get_bsp_ver(void);

int abs(int n)
{
	return n >= 0? n: -n;
}

#if 0
static void datapipe_test(void)
{
	char *tbuf;
	register unsigned int t, i, k;
	tbuf = (unsigned char *)alignAlloc(4, 10240);
	//USB初始化
	udp_open(1);
	printf("open datapipe to serial\n");
	mdelay(1000);
	
	sprintf(tbuf, "\nudp open datapipe to serial ========\n");
	printf("send datapipe to serial\n");
	udp_write(tbuf, strlen(tbuf));
	udp_write(tbuf, strlen(tbuf));
	udp_write(tbuf, strlen(tbuf));
	udp_write(tbuf, strlen(tbuf));
	udp_write(tbuf, strlen(tbuf));
	// 	mdelay(1000);
	
	printf("close datapipe to serial\n\n\n");
	udp_close();
	
	mdelay(1000);
	
    udp_open(0);
	printf("test =================\n");
	
	memset(tbuf, 0, sizeof(tbuf));
	
	while(1)
	{
		i = udp_read(tbuf, 10240, 10);
		if (i>0)
		{
			t = 0;
			// 			while(i>6400)
			// 			{
			// 				udp_write(tbuf+t, 3200);
			// 				t += 3200;
			// 				i -= 3200;
			// 			}
			k = udp_write(tbuf+t, i);
			t += i;
			// 			printf("read: %d, write: %d\n", t, k);
			// 			printf("%s\n", tbuf);
		}
	}
}

static void uart_test(void)
{
	unsigned char *tbuf;
	register unsigned int t, i, k;

	tbuf = (unsigned char *)alignAlloc(4, 10240);
	//USB初始化
	memset(tbuf, 0, sizeof(tbuf));
	while(1)
	{
		i = comm_read(tbuf,10240, 30);
		if (i>0)
		{
			t = 0;
			// 			while(i>6400)
			// 			{
			// 				udp_write(tbuf+t, 3200);
			// 				t += 3200;
			// 				i -= 3200;
			// 			}
			k = comm_write(tbuf+t, i);
			t += i;
// 			printf("read: %d, write: %d\n", t, k);
// 			tbuf[i] = 0;
// 			printf("%s\n", tbuf);
		}
	}
}

static void HIDPOS_test(void)
{
	unsigned char *tbuf;
	register unsigned int t, i, k;
	tbuf = (unsigned char *)alignAlloc(4, 10240);
	
	
    pos_open();
mdelay(1000);
	printf("test =================\n");
	
	memset(tbuf, 0, sizeof(tbuf));
	
	while(1)
	{
		i = pos_read(tbuf, 10240, 30);
		if (i>0)
		{
			t = 0;
			// 			while(i>6400)
			// 			{
			// 				udp_write(tbuf+t, 3200);
			// 				t += 3200;
			// 				i -= 3200;
			// 			}
			k = pos_write(tbuf+t, i);
			t += i;
			// 			printf("read: %d, write: %d\n", t, k);
			// 			printf("%s\n", tbuf);
		}
	}
}

static void key_test(void)
{
	while(1)
	{
		printf("key: %d\n", get_key_state());

	}
}

static void flash_test(void)
{
	unsigned char buf1[4096];
	unsigned char buf2[4096];
	int i;
	printf("test flash: %x, %x\n" ,  buf1, buf2);

	for (i=0; i<4096; i++)
	{
		buf1[i] = i;
	}

	jz_nor_write(0x200000, 4096, buf1);
	jz_nor_read(0x200000, 4096, buf2);    

	for (i=0; i<4096; i++)
	{
		if (buf1[i] != buf2[i])
			printf("%d, w: %02x, r: %02x\n", i, buf1[i], buf2[i]);
	}
	printf("test over\n");

}


//测试计算π的速度
void test_pi(void)
{
	long a=10000,b1=0,c=2800,d=0,e=0,f[2801],g=0, h=0; 
	int t1,t2;
	char bb[100];
	
	t1 = Get_PerformanceCounter()/1500;
	for(;b1-c;) 
		f[b1++]=a/5; 
	for(;d=0,g=c*2;c-=14,h=(e+d/a),/*printf("%.4d",h),*/e=d%a) 
		for(b1=c;d+=f[b1]*a,f[b1]=d%--g,d/=g--,--b1;d*=b1); 

	t2 = Get_PerformanceCounter()/1500;
	sprintf(bb, "%d", h);
	sprintf(bb, "shell *** PI test run time: %d ms ***\r\n\r\n", t2-t1);
	printf( bb);
}
#endif

BOOL X_paramLoadDeviceInfo(void);

int QueryDownFlag(void)
{
	char lvbuf[64];
	int lvret = 1;

	X_paramLoadDeviceInfo();


	if(X_paramQueryKeyword("IsDownload", lvbuf))
	{
		lvret = lvbuf[0] == '0' ? 0 : 1;
	}

	return lvret;
}


static void TaskStart(void *data)
{

    int i;
    int dnflag;
    //unsigned char buf[32];
    gpio_as_output(32*0+5);
    gpio_set_pin(32*0+5);
    gpio_as_output(32*2+24);
    gpio_set_pin(32*2+24);
    gpio_as_output(32*1+22);
    gpio_set_pin(32*1+22);

    /*for(i=0;i<7;i++)
        jz_nor_erase(0x40000+i*4096, 4096);*/
    // 	printf("\n\n newland MiniOS Kernel Start at %s\n", __TIME__);
    
    printf("TaskStart\n");
    //注册系统回调函数
    os_SetBspInterface(&x1000BspHook);
    
    // Save Informantion for debugging
    InitExcept(5);
    SAVE_PROCESS_REGISTER(g_save_register);

    // Start OS Ticker
    JZ_StartTicker(OS_TICKS_PER_SEC);
    JZ_StartTimer();
    get_bsp_ver();

    init_gpio();
    Init_CRC32_Table();
    jz_nor_init();
    i2c_open(1, 400000);
    UartInit();
    //dnflag = QueryDownFlag();
    printf("CIM_init\n");
    CIM_init();
    Init_WatchDog();
    printf("DataRoute\n");
    InitSendDataRouteTask();
    //HIDPOS_test();
    //uart_test();
    //datapipe_test();
    //key_test();
    //flash_test();

    //test_pi();
	printf("WAKE\n");
    Init_Wakeup_Timer();
    printf("1\n");
    //PWM初始化
    pwm_init(TCU_TIMER_PWM0);
    printf("2\n");
    Z_InitHardIntface((osINTFACE * )GetDecodeOSIntf());	
    printf("3\n");
    Z_DogReset(Wacthdog_Timeout);//+Wacthdog_Timeout*33/100);

    //解密
    rsa_get_cpuid((char *)rsa_cpuid);
    //rsa_cpuid[15] = 0x93;
    efuse_read_chipID((unsigned int *)efuse_cpuid);
    rsa_cpuid_crc16 = crc_cal_by_bit(rsa_cpuid, 16);
    efuse_cpuid_crc16 = crc_cal_by_bit(efuse_cpuid, 16);
    dev_magic = rsa_cpuid_crc16 ^ efuse_cpuid_crc16;
    printf("4\n");
    /*for(i=0;i<16;i++)
    {
        printf("%x %x\n", rsa_cpuid[i], efuse_cpuid[i]);
    }
    printf("magic=%x %x %x\n",rsa_cpuid_crc16, efuse_cpuid_crc16, dev_magic);
    // init_gpio();
    */

    U_appDevicePowerOnBeep();
    printf("5\n");
    U_appPowerOnDisplay(); 
//	Y_commSendString("Main_UART.\r\n");
	printf("6\n");
    
    while(1)
    {
        Z_DogFeed();
        //Z_LedOn(LED_GREEN);
        //Start decode
        if (TRUE == U_appProcDecode())
        {
			
        }
    }
}

unsigned short GetDevMagic(void)
{
    return dev_magic;
}


void Em3000_Main(void)
{
    printf("Em3000_Main...\n");	
	OS_TASK_STACK *ptos;
	OS_TASK_STACK *pbos;
	unsigned int  size;
	os_malloc_completed = 1;
	os_init_completed = 1;
   
	ptos        = &TaskStartStk[TASK_MAIN_STK_SIZE - 1];
	pbos        = &TaskStartStk[0];
	size        = TASK_MAIN_STK_SIZE;
	os_TaskCreateEx(TaskStart,
                   (void *)0,
                   ptos,
                   TASK_IDLE_PRI,
                   TASK_IDLE_PRI,
                   pbos,
                   size,
                   (void *)0,
                   OS_TASK_CREATE_EX_STK_CHK | OS_TASK_CREATE_EX_STK_CLR);

	os_SystemStart();                              /* Start multitasking */
	while(1);   
}

