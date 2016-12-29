#include <os_api.h>
#include "x1000.h"
#include "cmos.h"
//#include "mt9v022.h"
#include <em3000_cfg.h>
#include "gc0308.h"
#include "uartmc.h"
#include <string.h>
// #define CIM_DEBUG_TIME_ID

//////////////tannyhjl////////////
#undef printf
#include <stdio.h>
#include "uartmc.h"
///////////////////////////////

#define LIGHTFOCUS 0

#define FOC_ON_TIME		1200
#define	CIM_PORT	0

#define DEFAULT_MCLK    24000000

static int CmosSensorType = T_CMOS_UNKNOW;

#define TASK_CIM_STK_SIZE 1024*2

#define CLKGR     *(volatile unsigned int *)(0xB0000020)
//#define PAPATOC     *(volatile unsigned int *)(0xB0010048)

static OS_TASK_STACK cim_task_stack[TASK_CIM_STK_SIZE];


#define RAM_RESEVER_PIC			0x81FFFF00
#define RAM_RESEVER_IMG_FLAG	0x81FFFF04

#define RAM_IMG_ADDR	0x81600000//0x81c00000

static unsigned int nImg_want_lux = 0x74;

static int nCsiCapture = -1;


#define CMOSBUFMAX 8

static int CmosSensor = 0;
static volatile int nCaptureMode = 0;
volatile unsigned char nLightCtrlMode = 0;
volatile unsigned char nFocCtrlMode = 0;
volatile unsigned char nExtLightCtrlMode = 0;
struct cim_desc {
	u32 nextdesc;   /* Physical address of next desc */
	u32 frameid;    /* Frame ID */
	u32 framebuf;   /* Physical address of frame buffer */
	u32 dmacmd;     /* DMA command */
};

static CSINF cmosInfo;

static OS_EVENT *CIMWaitSem;
static OS_EVENT *CmosReadSem;
static OS_EVENT *CmosStopSem;

typedef struct _buf_node  
{  
    unsigned int buffer;  
    //unsigned int phyAddr;  
    unsigned int descAddr;
    struct _buf_node* pNext;  
}CmosBufNode;

static int bufindex=0;

typedef void (*LightCtrl)(int n);
static LightCtrl _light_ctrl = NULL;

CmosBufNode* pFree = NULL; 
CmosBufNode* pComplete = NULL; 
CmosBufNode* pRead = NULL; 
CmosBufNode* pDma = NULL; 
CmosBufNode* pDmatemp = NULL; 

static CmosBufNode _cmosBuf[CMOSBUFMAX];
struct cim_desc frame_desc[CMOSBUFMAX] __attribute__((aligned (32)));	


static volatile int img_count = 0;
static volatile unsigned int nCapture =  CMOSCAPTURE_STOP;


TYCMOSOperate	TSImgTypeOperate[] = {
	{
		T_CMOS_GC0308,
		GC0308_Info,
		GC0308_Init,
		GC0308_Width,
		GC0308_Height,
		GC0308_Adjust,
		GC0308_GetLux,
		GC0308_Mirror,
		GC0308_reg_read,
		GC0308_reg_write,
		GC0308_Width_For_CSI,
		GC0308_Height_For_CSI,
		GC0308_ImgData,
		GC0308_Capture_Mode,
		GC0308_Standby,
		GC0308_Exposure_update,
		GC0308_LightHandle
	},
	{
		T_CMOS_UNKNOW,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	}
};


#undef CIM_UNCACHED
#define CIM_UNCACHED(addr)	(((unsigned int)(addr) & 0x0fffffff) | 0xa0000000)


static void CmosBufInit(unsigned int w, unsigned int h)
{
    int i;
    int nSize;
	unsigned char *p;

    pRead = NULL;
    pComplete = NULL;
    pDma = NULL;
    pFree = &_cmosBuf[0];

    nSize = w*h;


    for (i=0; i<CMOSBUFMAX; i++)
    {
		//p = (unsigned char *)alignAlloc(128, nSize);	//BIT[6:0]必须为0
		p = (unsigned char*)((RAM_IMG_ADDR+nSize*i + 255)&0xffffff00);
        _cmosBuf[i].buffer = (unsigned int)p;
        _cmosBuf[i].descAddr= PHYS((unsigned int)(&frame_desc[i]));
        _cmosBuf[i].pNext = ((i+1)== CMOSBUFMAX)? NULL : &_cmosBuf[i+1];

		frame_desc[i].framebuf = PHYS(p);
		frame_desc[i].frameid = i;	
#ifdef CIM_DEBUG_TIME_ID
		frame_desc[i].dmacmd = CIM_CMD_EOFINT|CIM_CMD_STOP|(nSize>>2) | CIM_CMD_SOFINT; // stop after capturing a frame
#else
		frame_desc[i].dmacmd = CIM_CMD_EOFINT|CIM_CMD_STOP|(nSize>>2); // stop after capturing a frame
#endif
		frame_desc[i].nextdesc = 0;// ((i+1)== CMOSBUFMAX)? 0 :PHYS(&frame_desc[i]);

// 		printf("buf %d: %08x,  desc: %08x, %08x,  buf: %08x, cmd: %x\n", i, p, _cmosBuf[i].descAddr, &frame_desc[i], frame_desc[i].framebuf, frame_desc[i].dmacmd);
    }

	__dcache_writeback_all();

// 	{
// 		struct cim_desc *p1;
// 
// 		for (i=0; i<CMOSBUFMAX; i++)
// 		{
// 			p1 = (struct cim_desc *)(0x80000000 | _cmosBuf[i].descAddr);
// 			printf("desc: %08x,  buf: %08x, cmd: %x, id: %d\n", p1->nextdesc, p1->framebuf, p1->dmacmd, p1->frameid);
// 			
// 		}
// 	}
}



//计算节点个数
static int GetNodeCount(CmosBufNode* pNode)  
{  
	CmosBufNode* p = pNode;  
	CmosBufNode* tmp;  
	int n = 1;
	
	if (NULL == pNode)
		return 0;
	
	tmp = p->pNext;
	while(NULL != tmp)  
	{  
		p = tmp;  
		tmp = p->pNext;
		n++;
	}  
	
	return n;
}  



/*	
	从队列中取出第一个节点，pHead指向第二个节点
*/
 static CmosBufNode * GetCmosbuf(CmosBufNode **pHead)
{
    unsigned int sr;
    CmosBufNode *p;

    if (*pHead == NULL)
    {
        return NULL;
    }

//    sr = spin_lock_irqsave();

    p = *pHead;
    *pHead = p->pNext;
    p->pNext = NULL;

//    spin_unlock_irqrestore(sr);

// 	diag_printf("old: %x, new: %x, next: %x, free: %x, complete: %x\n", p->buffer, 
// 		(*pHead)?(*pHead)->buffer:(*pHead), (*pHead)?(*pHead)->pNext:(*pHead),
// 		pFree, pComplete);

    return p;
}



/*
	将pNode加入到pHead的末尾
*/
static void AddCmosBuf(CmosBufNode** pHead, CmosBufNode* pNode,  bool bAddSingleNode)  
{  
      unsigned int sr;
	CmosBufNode* p = *pHead;  
	CmosBufNode* pTem;
	int n=0;

// 	diag_printf("add: h: %x, %x; node: %x, next: %x\n", pHead, *pHead,  pNode, pNode?pNode->pNext:NULL);

	if (NULL == pNode)
		return;

 //	sr = spin_lock_irqsave();
//	cyg_interrupt_mask(IMX_INT_CSI);

	if(NULL == *pHead)  
	{  
		*pHead = pNode;
	}  
	else
	{
		pTem = p->pNext;
		
		while(NULL != pTem)  
		{  
#if 0
			if (n>CMOSBUFMAX)
			{
				p = *pHead;
				for (n=0; n<CMOSBUFMAX; n++)
				{
					pTem = p->pNext;
					p = pTem;
					diag_printf("next: %x\n", p->pNext);

				}
				while(1);
			}
#endif

			p = pTem;  
			pTem = p->pNext;  
			n++;
		}  

		p->pNext = pNode;  

	}
	if (bAddSingleNode)
		pNode->pNext = NULL;

 //	spin_unlock_irqrestore(sr);
//	cyg_interrupt_unmask(IMX_INT_CSI);

}  

static void ReleaseCmosBuf(void)
{
	//释放已采集完成的缓存节点到Free链表
	AddCmosBuf(&pFree, pComplete, FALSE);	
	pComplete = NULL;

}

static CmosBufNode * GetCmosDmaBuf(void)
{
	CmosBufNode *p;

// 	diag_printf("free: %x, %x, complete: %x, %x\n", pFree, pFree->buffer, pComplete, pComplete?pComplete->buffer:0);

	p = GetCmosbuf(&pFree);
	if (p == NULL)
	{

// 		diag_printf("free empty, get complete =====\n");
// 		diag_printf("GetCmosDmaBuf 1: %x\n", pComplete);
		p = GetCmosbuf(&pComplete);
// 		diag_printf("GetCmosDmaBuf 2: %x\n", pComplete);

		if (p == NULL)
		{
			int i;
			printf("\n\nGet img buffer error!!!! =====\n\n");
			printf("\r\nfree: %x, %x, complete: %x, %x\n", pFree, pFree?pFree->buffer:0, pComplete, pComplete?pComplete->buffer:0);
			for(i=0; i<CMOSBUFMAX; i++)
				printf("buf %d, %x\n", i, &_cmosBuf[i]);
			return NULL;	//图像缓存至少为5可以保证不会运行到此处
		}
	}

	return p;
}


 void cim_config(int w, int h)
{
    REG_CIM_FS(CIM_PORT) = ((h-1)<< 16) | (w-1);

    //gate clock mode
    //fifo order:0x11 0x22 0x33 0x44
    //dummy zero function disable
    //DMA burst type:INCR64
    //data is samppled by PCLK rising edge
    //HSYNC signal active high, HSYNC signal leading edge is rising edge
    //VSYNC signal active high, VSYNC signal leading edge is rising edge
    //not inverse
    //memory order:0x44 0x33 0x22 0x11,数据输出时从第四字节存储地址到第一字节存储地址的顺序输出
    //EEOF mode is disabel
    REG_CIM_CFG(CIM_PORT) = 0x001B0C02;	//refer to Linux-3.0.8

    //CIM is enable
    //stop resetting RXFIFO
    //enable DMA
    //software reset disable
    //not requred to stop
    //When a new descriptor-DMA transfer starts with writing CIMDA, a frame synchronization will be done, and the data in RXFIFO will be ignored
    REG_CIM_CTRL(CIM_PORT) = 0x00000085;

    //Auto priority mode enable. CIM can use the priority according the fifo status
    REG_CIM_CTRL2(CIM_PORT) = (CIM_CTRL2_APM | CIM_CTRL2_EME | CIM_CTRL2_OPE |(1<<CIM_CTRL2_OPG_BIT) | CIM_CTRL2_CSC_BYPASS| CIM_CTRL2_ARIF );
    REG_CIM_STATE(CIM_PORT) = 0;

    __cim_clear_state(CIM_PORT);	// clear state register

    __cim_reset_rxfifo(CIM_PORT);	// resetting rxfifo
        
    //enable DMA EOF interrupt
    __cim_enable_eof_intr(CIM_PORT);

    //enable  RXFIFO overflow interrupt
    __cim_enable_rxfifo_overflow_intr(CIM_PORT);

	__cim_enable_sof_intr(CIM_PORT);
    
///////////////////////////////////////////////////////////

}

 


void set_cmos_light_mode(int nMode)
{
	nLightCtrlMode = nMode&0xff;
	nFocCtrlMode = (nMode>>8)&0xff;
	nExtLightCtrlMode = (nMode>>16)&0xff;

// 	diag_printf("light: %d, foc: %d, ext: %d\n", nLightCtrlMode, nFocCtrlMode, nExtLightCtrlMode);
}

int get_cmos_light_mode(void)
{
	int mode = (nExtLightCtrlMode<<16)|(nFocCtrlMode<<8)|nLightCtrlMode;
	return mode;
}

void foc_on(BOOL b)
{
      #ifndef LIGHTFOCUS
	if (b)
	{
		
		__gpio_set_pin(CIM_FOC);	//foc high
	}
	else
	{
		 __gpio_clear_pin(CIM_FOC);//foc low
	}
      #else
      if (b)
	{		
		__gpio_clear_pin(CIM_FOC);//foc low
	}
	else
	{
		 __gpio_set_pin(CIM_FOC);	//foc high
	}
    #endif
}

void foc_timer_handle(void)
{
	foc_on(FALSE);
 //   printf("off\n");
}

void focoff_timer_handle(void)
{
	foc_on(TRUE);
    timer_delay(3, 1500, foc_timer_handle);
 //   printf("off\n");
}

void foc_on_delay(int us)
{
    unsigned int ndata[2];
    unsigned int exp_time;
      //timer_delay(3,30, focoff_timer_handle);
      //udelay(500);
	foc_on(TRUE);	
      timer_delay(3,us, foc_timer_handle);

/*      GC0308_reg_read(0x03, &ndata[0]);
      GC0308_reg_read(0x04, &ndata[1]);
      exp_time = ndata[0]&0xf<<8 | ndata[1];
      if(exp_time > 536)
        foc_on(TRUE);*/
   // printf("on\n");
	
}


void light_on(BOOL b)
{
    #ifndef LIGHTFOCUS
    if (b)
    {		
        __gpio_set_pin(CIM_LIGHT);
    }
    else
    {
        __gpio_clear_pin(CIM_LIGHT);
    }
    #else
    if (b)
    {		
        __gpio_clear_pin(CIM_LIGHT);
    }
    else
    {
        __gpio_set_pin(CIM_LIGHT);
    }
    #endif
}

void light_timer_handle(void)
{
	__gpio_clear_pin(CIM_LIGHT);
}

void light_on_delay(int us)
{
	__gpio_set_pin(CIM_LIGHT);
	timer_delay(3, us, light_timer_handle);
}

void Light_Foc_Init(void)
{
    __gpio_as_output(CIM_LIGHT);
    __gpio_as_output(CIM_FOC);
    light_on(FALSE);
    foc_on(FALSE);

}

void ledout_isr(void)
{
 //printf("ledoutISR\r\n");
	__gpio_ack_irq((GPIO_GROUP_B+21));//清中断标志
//     disable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+7));

	if(!__gpio_get_pin(GPIO_GROUP_B+21))//下降沿
	{
//         led_off(1);
		if (_light_ctrl)
			_light_ctrl(FALSE);//非曝光
		//printf("fall edge interrupt\r\n");


		__gpio_as_irq_rise_edge(GPIO_GROUP_B+21);//改成上升沿中断
		//         enable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+7));  
     }
     else
     {
// 		led_on(1);
		if (_light_ctrl)
			_light_ctrl(TRUE);//曝光
		//printf("rise edge interrupt\r\n");


		__gpio_as_irq_fall_edge(GPIO_GROUP_B+21);//改成下降沿中断
		//          enable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+7));  
     }
}



static void ledout_init(void)
{
    
    //printf("ledoutinit\r\n");
    request_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_B+21), ledout_isr, 0);  
	disable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_B+21));
}



static void vsync_handle(unsigned int arg)
{
    int ntime;
    unsigned char bri;
    __gpio_ack_irq((GPIO_GROUP_A+21));


    if(!__gpio_get_pin(GPIO_GROUP_A+21))
    {
        if (_light_ctrl)
            _light_ctrl(FALSE);
		//printf("fall edge interrupt\r\n");
        __gpio_as_irq_rise_edge(GPIO_GROUP_A+21);
    }
    else
    {
        if (_light_ctrl)
            _light_ctrl(TRUE);
		//printf("rise edge interrupt\r\n");
        __gpio_as_irq_fall_edge(GPIO_GROUP_A+21);
    }
    
    //ntime = nBF3005_Exp* 640/27;
    //timer_delay(3, ntime, light_timer_handle);
    //printf("exptime=%x\r\n",nBF3005_Exp);
	
	//printf("vsync interrupt\r\n");
}

void vsync_init(void)
{	//vsync
	//__gpio_as_irq_rise_edge(GPIO_GROUP_B+17);
	request_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+21), vsync_handle, 0);  
	disable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+21));
}


void illumination_interrupt_init(void)
{
	if (cmosInfo.nShutterType == CMOS_SHUTTER_TYPE_GLOBAL)
	{
		ledout_init();
	}
	else if(cmosInfo.nShutterType == CMOS_SHUTTER_TYPE_ROLLING)
	{
		vsync_init();
	}
	
}

void illumination_interrupt_reset(void)
{
	if (cmosInfo.nShutterType == CMOS_SHUTTER_TYPE_GLOBAL)
	{
		__gpio_ack_irq((GPIO_GROUP_B+21));//清中断标志
		__gpio_as_irq_rise_edge(GPIO_GROUP_B+21);//上升沿中断
		enable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_B+21));
	}
	else if(cmosInfo.nShutterType == CMOS_SHUTTER_TYPE_ROLLING)
	{
		__gpio_ack_irq(GPIO_GROUP_A+21);
		__gpio_as_irq_rise_edge(GPIO_GROUP_A+21);
		enable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+21));
	}
}

void illumination_interrupt_disable(void)
{
	if (cmosInfo.nShutterType == CMOS_SHUTTER_TYPE_GLOBAL)
	{
		disable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_B+21));
	}
	else if(cmosInfo.nShutterType == CMOS_SHUTTER_TYPE_ROLLING)
	{
		disable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+21));
	}	
}


unsigned char *ReadCmosBuf(unsigned int mode, unsigned int timeout)
{
	bool b;
	unsigned int t1, t2;	
	unsigned char *p;
	CmosBufNode * pTemp;
	unsigned char err;

		
	if (CMOSGetUsedPic == mode)
	{
		if (NULL != pRead)
			return TSImgTypeOperate[CmosSensor].fnc_ImgData((unsigned char*)pRead->buffer, (unsigned int*)RAM_RESEVER_IMG_FLAG);

		if (-1 == nCsiCapture)			
		{
			p = (unsigned char*)(*(unsigned int*)RAM_RESEVER_PIC);
			printf("resever: %08x\n", p);

			if (((unsigned int)p >= _cmosBuf[0].buffer) && 
				((unsigned int)p <= (unsigned int)TSImgTypeOperate[CmosSensor].fnc_ImgData((unsigned char*)_cmosBuf[CMOSBUFMAX].buffer, (unsigned int*)RAM_RESEVER_IMG_FLAG)))
				return p;
			else
				return TSImgTypeOperate[CmosSensor].fnc_ImgData((unsigned char*)_cmosBuf[0].buffer, (unsigned int*)RAM_RESEVER_IMG_FLAG);
		}
	}	
	else if (CMOSGetNewPic == mode)
	{

		if (NULL != pRead)
		{
            AddCmosBuf(&pFree, pRead, 1);
		}
		if (NULL == pComplete)
		{
			if (timeout == 0)
				return NULL;

// 			printf(" == sem: %d, t: %d, pComplete: %d\r\n", CmosReadSem->wEventCount, os_TimeGet(), GetNodeCount(pComplete));
			os_SemaphoreSet(CmosReadSem, 0, &err);

			t1 = Get_PerformanceCounter();
			os_SemaphorePend(CmosReadSem, (timeout+9)/10, &err);
			t2 = Get_PerformanceCounter();
 			//printf("read: %d, timeout: %d, t: %d\n", err, timeout, (t2 -t1)/1500);
			if (err != 0)	//err ==10 timeout
			{
				printf("wait cmos error: %d, time: %d\n", err, (t2 -t1)/1500);
				return NULL;	
			}
			
		}

		pTemp = GetCmosbuf(&pComplete);

		if (NULL == pTemp)
		{
			printf("ReadCmosBuf after wait, receive null\n");
			return NULL;
		}
		else
		{
			pRead = pTemp;
		}		

		nCsiCapture = 1;

		p = TSImgTypeOperate[CmosSensor].fnc_ImgData((unsigned char*)pRead->buffer, (unsigned int*)RAM_RESEVER_IMG_FLAG);
		*(unsigned int*)RAM_RESEVER_PIC = (unsigned int) p;

// 		printf("buf: %x\n", p);

		return p;
	}

// 	diag_printf("ReadCmosBuf return null, mode: %d, n: %d, resever: %x\nread: %x, %x, complete: %x, %x\n", 
// 		mode, nCsiCapture, (*(unsigned int*)RAM_RESEVER_PIC),pRead, pRead?pRead->buffer:0, pComplete, pComplete?pComplete->buffer:0);
	return NULL;
}

unsigned char *ReadCmosBufTest(int n)
{
	unsigned char *p;

	if (n<0 || n>CMOSBUFMAX)
		p =  (unsigned char*)RAM_RESEVER_PIC;
	else
		p = TSImgTypeOperate[CmosSensor].fnc_ImgData((unsigned char*)_cmosBuf[n].buffer, (unsigned int*)RAM_RESEVER_IMG_FLAG);

// 	printf("testbuf: %08x\n", p);
	return p;
}


void Set_CmosCaptureMode(int mode)
{
// 	printf("\r\nCmosCaptureMode: %d\n", mode);

	if (TSImgTypeOperate[CmosSensor].fnc_LightHandle != NULL)
	{ _light_ctrl = (LightCtrl)TSImgTypeOperate[CmosSensor].fnc_LightHandle(mode); }	


	if (TSImgTypeOperate[CmosSensor].fnc_CaptureMode != NULL)
	{TSImgTypeOperate[CmosSensor].fnc_CaptureMode(mode); }	
	
}

static int _MobileMode = 0;
/*
	0 - 普通模式（纸质码）；nls0321000
	1 - 反光抑制；
	2 - mobile模式曝光的时候有的帧照明灯亮，有的帧照明灯灭； nls0321021
	3 - 纸质码（motion好，但很难读屏幕显示码）；
	4 - mobile模式 每帧曝光照明灯都亮。

	仅支持 模式0与2
*/

void Set_Mobile_mode(int mode)
{
// 	printf("\r\nSet_Mobile_mode: %d\n", mode);
	
	if (mode == nCaptureMode)
		return;

	if (mode == 0)	
	{
		nCaptureMode = 0;
		if (nCapture == CMOSCAPTURE_MOBILE)
		{
			printf("Set_Mobile_mode  set 0===\r\n");
			csi_capture(FALSE, 0);
			csi_capture(TRUE, 0);
		}
	}
	else if (mode == 2)
	{
		nCaptureMode = 2;
		if (nCapture == CMOSCAPTURE_PRINT)
		{
			printf("Set_Mobile_mode  set 2===\r\n");
			csi_capture(FALSE, 0);
			csi_capture(TRUE, 0);
		}
	}
	
}


#ifdef CIM_DEBUG_TIME_ID
static unsigned int testT = 0;
#endif

/*获取图像
mode: 
		0 -- 识读状态。
		1 -- 侦测状态。
*/
void csi_capture(BOOL b, int mode)
{
	unsigned char err;


	if(b)//start capture
	{
	     /*#if GC0308_USE_AEC
            GC0308_reg_write(0xD2, 0x90);    
           #endif*/
           
		switch(mode)
		{

		case 1:	//侦测状态 用纸质码配置
			Set_CmosCaptureMode(CMOSCAPTURE_DETECT);	
			nCapture = CMOSCAPTURE_DETECT;
			break;

		default://shi du
			if (nCaptureMode == 0)	//仅纸质码模式
			{
				Set_CmosCaptureMode(CMOSCAPTURE_PRINT);
				nCapture = CMOSCAPTURE_PRINT;
                         //mdelay(50);
                         //GC0308_reg_write(0xd2, 0x90);
			}
			else if (nCaptureMode == 2)
			{
				Set_CmosCaptureMode(CMOSCAPTURE_MOBILE);	
				nCapture = CMOSCAPTURE_MOBILE;
			}	
			break;
		}

// 		printf("\n-------cap on --->>> \n");

		//释放缓存节点到Free链表
		ReleaseCmosBuf();
			
		img_count = 0;
// 		__gpio_as_irq_rise_edge(GPIO_GROUP_A+7);//上升沿中断
// 		enable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+7));

		if (NULL == pDma)
		{
// 			diag_printf("Dma1 Null\n");
			pDma = GetCmosDmaBuf();
			if (NULL == pDma)
			{
				printf("pDMA1 No buf\nCan not start capture\n");
				return;
			}
		}
        
		os_SemaphoreSet(CmosReadSem, 0, &err);
		os_SemaphoreSet(CmosStopSem, 0, &err);

        __cim_disable(CIM_PORT);
		__cim_set_da(CIM_PORT, pDma->descAddr);
		__cim_clear_state(CIM_PORT);	// clear state register
		__cim_reset_rxfifo(CIM_PORT);	// resetting rxfifo
		__cim_unreset_rxfifo(CIM_PORT);
		__cim_enable_dma(CIM_PORT);	// enable dma

		illumination_interrupt_reset();

		__cim_enable(CIM_PORT);  

	}
    else//stop capture
    {
		nCapture = CMOSCAPTURE_STOP;
		//GC0308_reg_write(0xD2, 0x00);  
		illumination_interrupt_disable();

		
            //GC0308_reg_write(0xd2, 0x00);
            //mdelay(50);
		Set_CmosCaptureMode(CMOSCAPTURE_STOP);
        
		light_on(FALSE);
		foc_on(FALSE);
        
        os_SemaphorePend(CmosStopSem, 10, &err);
// 		printf("\nstop 2\n");
// 		led_off(1);

   }
}

// static unsigned int tEOF = 0;

static void cim_irq_handler(unsigned int arg)
{
    unsigned char err;

    u32 state = REG_CIM_STATE(CIM_PORT);

          



// 	printf("\ncim: %x, id: %d, t:%ld\n", state, REG_CIM_IID(CIM_PORT), (Get_PerformanceCounter())/150-testT);

	if (state & CIM_STATE_DSOF)
	{
#ifdef CIM_DEBUG_TIME_ID
		testT = Get_PerformanceCounter()/150;
#endif
		//printf("sof: %d\n", testT);
	}

    if (state & CIM_STATE_DMA_EOF) 
    {
// 		tEOF = Get_PerformanceCounter();
		__cim_disable_dma(CIM_PORT);	// enable dma
		__cim_disable(CIM_PORT);
        os_SemaphorePost(CIMWaitSem);
        //printf("EOF: %d, id: %d\n\n", (Get_PerformanceCounter())/150-testT, REG_CIM_IID(CIM_PORT));
    }
    
    if (state & CIM_STATE_RXF_OF)
    { 
		printf("CIM RXFIFO overflow, addr: %08x, buf: %08x, cmd: %x, pdma: %08x\n\n", REG_CIM_DA(CIM_PORT), REG_CIM_FA(CIM_PORT), REG_CIM_CMD(CIM_PORT), pDma->descAddr);
// 		{
// 			struct cim_desc *p = (struct cim_desc *)(0x80000000 | pDma->descAddr);
// 			if (p)
// 				printf("buf: %08x, id: %d, cmd: %x\n",p->framebuf, p->frameid, p->nextdesc);
// 
// 		}

        __cim_disable(CIM_PORT);
        __cim_set_da(CIM_PORT, pDma->descAddr);
        __cim_reset_rxfifo(CIM_PORT);
        __cim_clear_rxfifo_overflow(CIM_PORT);
        __cim_unreset_rxfifo(CIM_PORT);
        __cim_enable(CIM_PORT);

    }	
// 	if (state & CIM_STATE_FSE) {
// 	}
// 	if (state & CIM_STATE_DMA_STOP) {
// 	}	

    REG_CIM_STATE(CIM_PORT) = 0;
}

void cim_task_entry(void *arg)
{
    u8 err;
    CmosBufNode* temp=NULL;
	unsigned int n,ndata;
    unsigned char buf[128]={'\0'}; 
	printf("cim_task_entry\n");
    while(1)
    {       
        os_SemaphorePend(CIMWaitSem, 0, &err);//第2个参数是超时参数。0表示硬等，单位为10ms/tick

        temp = pDma;
		
		if (img_count > 0)
			pDma = GetCmosDmaBuf();
		
		if (nCapture)
		{
			__cim_set_da(CIM_PORT, (pDma->descAddr));
			__cim_reset_rxfifo(CIM_PORT);
			__cim_clear_rxfifo_overflow(CIM_PORT);
			__cim_unreset_rxfifo(CIM_PORT);
			__cim_enable(CIM_PORT);
			__cim_enable_dma(CIM_PORT);	// enable dma
			
#ifdef CIM_DEBUG_TIME_ID
			n = REG_CIM_IID(CIM_PORT);
			printf("\nid: %d, t:%ld\n", n, (Get_PerformanceCounter())/150-testT);
			
			testT = Get_PerformanceCounter()/150;
#endif
		}
		else
		{
			__cim_disable(CIM_PORT); 
// 			
// 			disable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+7));
// 			light_on(FALSE);
// 			foc_on(FALSE);
			os_SemaphorePost(CmosStopSem);
// 			printf("\nstop\n");
		}
		
		if (img_count > 0)
		{
			AddCmosBuf(&pComplete, temp, TRUE);	//插入到Complete队列
			
			
			os_SemaphorePost(CmosReadSem);
		}


		if (TSImgTypeOperate[CmosSensor].fnc_Adjust != NULL)
		{
			TSImgTypeOperate[CmosSensor].fnc_Adjust(nImg_want_lux, 
				(unsigned char*)(temp->buffer), 
				img_count, LIGHT_ON_COUNT, LIGHT_OFF_COUNT);
		}

		//printf( "i: %d, n: %d，complete: %d\r\n", img_count, REG_CIM_IID(CIM_PORT), GetNodeCount(pComplete));

		img_count ++;

	
      //GC0308_reg_read(0x03, buf+1);

      GC0308_reg_read(0x3, &ndata);
      sprintf(buf,"%02x",ndata);
     GC0308_reg_read(0x4, &ndata);
      sprintf(buf+strlen(buf),"%02x, ",ndata);
      GC0308_reg_read(0xd2, &ndata);
      sprintf(buf+strlen(buf),"%02x, ",ndata);
     GC0308_reg_read(0xe4, &ndata);
      sprintf(buf+strlen(buf),"%02x",ndata);
      GC0308_reg_read(0xe5, &ndata);
      sprintf(buf+strlen(buf),"%02x, ",ndata);
      GC0308_reg_read(0xe6, &ndata);
      sprintf(buf+strlen(buf),"%02x",ndata);
      GC0308_reg_read(0xe7, &ndata);
      sprintf(buf+strlen(buf),"%02x, ",ndata);
      GC0308_reg_read(0xe8, &ndata);
      sprintf(buf+strlen(buf),"%02x",ndata);
      GC0308_reg_read(0xe9, &ndata);
      sprintf(buf+strlen(buf),"%02x, ",ndata);
      GC0308_reg_read(0xea, &ndata);
      sprintf(buf+strlen(buf),"%02x",ndata);
      GC0308_reg_read(0xeb, &ndata);
      sprintf(buf+strlen(buf),"%02x",ndata);
      sprintf(buf+strlen(buf),"\r\n",ndata);
     comm_write(buf, strlen(buf));



      
     
   
// 		n = (Get_PerformanceCounter() - tEOF)*10/15;
// 		printf("\r\ntask: %d \r\n", n);
    }

}



int cmos_sensor_Hw_Reset(void)
{   
    __gpio_clear_pin(CIM_RST);
    mdelay(2);
    __gpio_set_pin(CIM_RST);
    mdelay(2);
	
    return 0;
}

void CIM_init(void)
{
    int nCnt, i;
    unsigned int dividor;
    
    CLKGR &= ~(1 << 23);//lcd clk gate，打开lcd时钟，cim时钟来自LCD
    CLKGR |= (1 << 22);//cim clk gate,关闭cim时钟
    CLKGR |= (1 << 21);//pdma clk gate，关闭dma时钟

	printf("CMI entry\r\n");

    gpio_as_output(CIM_PWR);
    gpio_set_pin(CIM_PWR);
    gpio_as_output(CIM_STANDBY);
    gpio_as_output(CIM_RST);
    gpio_clear_pin(CIM_STANDBY);
    Light_Foc_Init();
	
	cmos_sensor_Hw_Reset();

	printf("CMI 1\r\n");

    //cim接口脚复用功能配置
    PAINTC |= 0xFFF00;//PA脚不作为中断口使用
    PAMSKC  |= 0xFFF00;//pa8,pa9,pa[19:11],pa21相应脚被设置成功功能口使用
    PAPAT1S |=  0xFFF00;
    PAPAT0C |=  0xFFF00;

    CLKGR &= ~(1 << 22);//enable CIM LCD clk
    CIMCDR = 0xA0000000;
    //dividor = (cpm_get_xpllout(SCLK_MPLL)+DEFAULT_MCLK -1)/DEFAULT_MCLK - 1; 
    dividor = cpm_get_xpllout(SCLK_MPLL)/DEFAULT_MCLK - 1; 
    CIMCDR &=0xFFFFFF00;
    CIMCDR |= dividor;	//实际：26.5M，帧周期：16.8ms，帧有效期：15.4ms
    CLKGR &= ~(1 << 29);
    CIMCR = 1 << 0;//enable CIM

   	printf("mpll: %d, mclk: %d \n", cpm_get_xpllout(SCLK_MPLL), cpm_get_clock(CGU_CIMCLK));
 
    i2c_open(i2c_1, CMOS_I2C_CLK_DEFAULT);
	printf("CIM_CNT\n");
    nCnt = sizeof(TSImgTypeOperate)/sizeof(TYCMOSOperate);
    for (i = 0; i < nCnt; i++)
    {
		if (TSImgTypeOperate[i].fnc_Info == NULL)	//不存在cmos sensor
		{
			CmosSensorType = TSImgTypeOperate[i].nIDForCMOS;
			CmosSensor = i;
			break;
		}

		TSImgTypeOperate[i].fnc_Info(&cmosInfo);
		if (cmosInfo.bValid)
		{
			CmosSensorType = TSImgTypeOperate[i].nIDForCMOS;
			CmosSensor = i;
			break;
		}
    }

    printf("sensor: %d, type: %d, format: %d, shutter: %d, inputClk: %d, i2cClk: %d\n", 
		CmosSensor, CmosSensorType, cmosInfo.nImgFormat, cmosInfo.nShutterType, cmosInfo.nInputClk, cmosInfo.nI2cClk);

	if (CmosSensorType == T_CMOS_UNKNOW)
	{
		printf("\r\nCan not find CMOS Sensor!\r\n");
		return;
	}

	if (cmosInfo.nInputClk != DEFAULT_MCLK)
	{
		CIMCDR = 0xA0000000;
		dividor = (cpm_get_xpllout(SCLK_MPLL)+cmosInfo.nInputClk -1)/cmosInfo.nInputClk - 1; 
		CIMCDR |= dividor;	
// 		CLKGR &= ~(1 << 29);
		
   		printf("mclk: %d \n", cpm_get_clock(CGU_CIMCLK));
	}

	if (cmosInfo.nI2cClk != CMOS_I2C_CLK_DEFAULT)
	{
		i2c_close(i2c_1);
		i2c_open(i2c_1, cmosInfo.nI2cClk);
		printf("i2c clock: %d\r\n", cmosInfo.nI2cClk);
	}

    if (TSImgTypeOperate[CmosSensor].fnc_Init != NULL)
    {
        //printf("MT9V022 REG INIT!\r\n");
        TSImgTypeOperate[CmosSensor].fnc_Init(); 
    }
 
// 	printf("init 1: %x\n",REG_CIM_CTRL(CIM_PORT));

    cim_config(TSImgTypeOperate[CmosSensor].fnc_GetWidth_ForCSI(), TSImgTypeOperate[CmosSensor].fnc_GetHeight_ForCSI());
//    _light_ctrl = LightCtrlmode1;
    __cim_disable(CIM_PORT);

// 	printf("init 2: %x\n",REG_CIM_CTRL(CIM_PORT));

	illumination_interrupt_init();
    
	CmosBufInit(TSImgTypeOperate[CmosSensor].fnc_GetWidth_ForCSI(), TSImgTypeOperate[CmosSensor].fnc_GetHeight_ForCSI());
    CIMWaitSem = os_SemaphoreCreate(0);
    CmosReadSem = os_SemaphoreCreate(0);
    CmosStopSem = os_SemaphoreCreate(0);

    os_TaskCreate(cim_task_entry, 
		(void *)0, 
		(void*)&cim_task_stack[TASK_CIM_STK_SIZE -1], 
		TASK_CIM_PRIO);
	
    //if ((ret =  request_irq(IRQ_CIM, cim_irq_handler, (unsigned int)dev)) == -1) 
    if (( request_irq(IRQ_CIM, cim_irq_handler, (unsigned int)&CIMWaitSem)) == -1) 
    {
		printf("CIM IRQ request error\n");
    }
}

int Get_Cmos_type(void)
{
	return CmosSensorType;
}


void CMosREGSet(int addr,int data)
{	
	if (TSImgTypeOperate[CmosSensor].fnc_RegWrite != NULL)
	{TSImgTypeOperate[CmosSensor].fnc_RegWrite(addr, data); }	
}

int CMosREGRead(int addr)
{
	unsigned int lvchr = 0;
	
	if (TSImgTypeOperate[CmosSensor].fnc_RegRead != NULL)
	{TSImgTypeOperate[CmosSensor].fnc_RegRead(addr, &lvchr); }	
	
	return lvchr;
}

int CMosWidth(void)
{
	if (TSImgTypeOperate[CmosSensor].fnc_GetWidth != NULL)
	{ return TSImgTypeOperate[CmosSensor].fnc_GetWidth(); }	
	return 0;
}

int CMosHeight(void)
{
	if (TSImgTypeOperate[CmosSensor].fnc_GetHeight != NULL)
	{ return TSImgTypeOperate[CmosSensor].fnc_GetHeight(); }	
	return 0;
}

//设置镜像 nMode: 0 - Normal; 1 - horizontal mirror; 2 - vertical flip; 3 - horizontal mirror & vertic
void CMosMirror(int nMode)
{
	if (TSImgTypeOperate[CmosSensor].fnc_Mirror != NULL)
	{ TSImgTypeOperate[CmosSensor].fnc_Mirror(nMode); }	
}


void CmosStandby(BOOL bStandby)
{
	if (bStandby)
	{
		//休眠
		if (TSImgTypeOperate[CmosSensor].fnc_Standby != NULL)
		{ 
			TSImgTypeOperate[CmosSensor].fnc_Standby(TRUE); 
			//关clk
		}	
		
	}
	else
	{

		if (TSImgTypeOperate[CmosSensor].fnc_Standby != NULL)
		{ 
			//开clk
			TSImgTypeOperate[CmosSensor].fnc_Standby(FALSE); 
		}	
	}
	
}

unsigned char* YUV422toMonochrome(unsigned char *pSrc, int nSize)
{
	int i, k;
	unsigned char *p = pSrc;

	if (pSrc == NULL)
		return NULL;

	k = 0;

	for (i=0; i<nSize; i+=2)
	{
		p[k++] = pSrc[i];
	}

	return p;
}

