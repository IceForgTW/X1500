#include <x1000.h>
#include <list.h>
#include "../i2c/i2c.h"
#include "cmos.h"
#include "gc0308.h"


static unsigned int i2c_addr = 0x42;	//GC0308

static int nGC0308CaptureMode = 0;

static int LightCount = 0;


#define GC0308_NO_GAIN		0
#define GC0308_USE_AEC		1

#define GC0308_MAXEXPOSUE_A		480
#define GC0308_MAXEXPOSUE_B		960
#define GC0308_MAXGAIN			0x30
#define GC0308_INITEXP			(480)
#define GC0308_INITGAIN			0x10
#define GC0308_INITAG			0xF6
#define GC0308_VB_MIN			45

static int nGC0308_Exp_A = GC0308_INITEXP;
static int nGC0308_Gain_A = GC0308_INITGAIN;
static int nGC0308_Exp_B = GC0308_MAXEXPOSUE_A;
static int nGC0308_Gain_B = GC0308_INITGAIN;

static int nGC0308_VBlanking = GC0308_VB_MIN;
static int nGC0308_Exp = GC0308_MAXEXPOSUE_A;
static int nGC0308_Gain = GC0308_INITGAIN;

extern volatile unsigned char nLightCtrlMode;;
extern volatile unsigned char nFocCtrlMode;
extern volatile unsigned char nExtLightCtrlMode;
static int bDecoding = 0;

int GC0308_reg_read (unsigned int addr, unsigned int *buf)
{
     int ret;
    //int t1,t2;
    //t1=Get_PerformanceCounter()*10/15;
	 printf("GC0308\n");
	 printf("addr:0x%02x\n", addr);
    ret = i2c_read(i2c_1, i2c_addr >> 1, (unsigned char*)buf, addr, 1);//一次读取两个字节数据

    //t2=Get_PerformanceCounter()*10/15;
    //printf("rt1=%d, rt2=%d\n",t1,t2);
    
    if (ret != 0)
	{
		i2c_close(i2c_1);
		i2c_open(i2c_1, CMOS_I2C_CLK_HIGH);
		printf("i2c read error: %d\r\n", ret);
	}

	return ret;
}

int GC0308_reg_write(unsigned int addr, unsigned int data)
{
    int ret;
    //int t1,t2;
    
	
    //t1=Get_PerformanceCounter()*10/15;
	
    ret = i2c_write(i2c_1, i2c_addr >> 1, (unsigned char*)&data, addr, 1);
    //t2=Get_PerformanceCounter()*10/15;
    //printf("w=%d\n",t2-t1);
	if (ret !=0)
	{
		i2c_close(i2c_1);
		i2c_open(i2c_1, CMOS_I2C_CLK_HIGH);
		printf("i2c write error: %d\r\n", ret);
	}

// 	printf("i2c write: %02x, %d\r\n", addr, data);

    return ret;
}

void _is_Decoding(int nFlag)
{
    bDecoding = nFlag;
}

static void LightCtrlmode1(int n)
{

// light_on(TRUE);
// return;

    if (n == 1)
	{
		foc_on(FALSE);
//foc_on_delay(1500);
		switch(nLightCtrlMode)
		{
		case 2:
			light_on(FALSE);
			break;
		default:
			light_on(TRUE);
            
			break;
		}

		LightCount++;
	}
	else if (n == 0)
	{
	    if(bDecoding)
          {
                if(nLightCtrlMode!=2)
                    light_on(TRUE);
                else
                    light_on(FALSE);
	    }
          else
		  light_on(FALSE);		
		
		switch(nFocCtrlMode)
		{
		case 2: //always off
			foc_on(FALSE);
			break;
		default:
// 			foc_on(TRUE);
			foc_on_delay(2500);
			break;
		}	

// 		if (LightCount>=2)
		{
// 			printf("L: %d  ", LightCount);
			GC0308_Exposure_update();
		}	
	}
}


//mode1 屏幕识读模式
static void LightCtrlmode2(int n)
{
	
	if (n == 1)
	{
		foc_on(FALSE);
		
		switch(nLightCtrlMode)
		{
		case 2:
			light_on(FALSE);
			break;
		default:
			if (LightCount % 3 == 0)
			{
				light_on(FALSE);
			}
			else if (LightCount % 3 == 1)
			{
				light_on(FALSE);
			}
			else
			{
 				light_on(TRUE);
			}
			break;
		}

		LightCount++;
	}
	else if (n == 0)
	{
		light_on(FALSE);		
		
		switch(nFocCtrlMode)
		{
		case 2: //always off
			foc_on(FALSE);
			break;
		default:
			//foc_on(TRUE);	
			foc_on_delay(2500);
			break;
		}

		if (LightCount>=2)
		{
			GC0308_Exposure_update();
		}
		
	}
	
}

//mode3 侦测识读模式
static void LightCtrlmode3(int n)
{
	if (n == 1)
	{
		if(nLightCtrlMode == 1)
		{
			foc_on(FALSE);
			light_on(TRUE);
		}
		else if (nFocCtrlMode == 1)
		{
			light_on(FALSE);
//			foc_on(TRUE);
			foc_on_delay(2500);
			
		}
		
	}
	else if (n == 0)
	{
		light_on(FALSE);		
		
		if ((nLightCtrlMode == 1) && (nFocCtrlMode == 1))
		{
			//foc_on(TRUE);
			foc_on_delay(2500);
		}
		else
			foc_on(FALSE);
		
	}
	
}

void * GC0308_LightHandle(int mode)
{
	void *p = NULL;
	if (mode == CMOSCAPTURE_PRINT)	
	{
		p = LightCtrlmode1;
	}
	else if (mode == CMOSCAPTURE_MOBILE)
	{
		p = LightCtrlmode2;
	}
	else if (mode == CMOSCAPTURE_DETECT)
	{
		p = LightCtrlmode3;
	}
	else if (mode == CMOSCAPTURE_STOP)
	{
	}
	return p;
}


void GC0308_Info(CSINF *p)
{
	unsigned int data = 0;

	if (!p) return;

	printf("GC0308_InfoEnter\n");
	GC0308_reg_read(0, &data);
	if (data == 0x9b)
	{
		p->bValid = TRUE;
		p->nImgFormat = IMG_FORMAT_MONOCHROME_8BIT;
		p->nShutterType = CMOS_SHUTTER_TYPE_ROLLING;
		p->nInputClk = 24000000;
		p->nI2cClk = CMOS_I2C_CLK_HIGH;
	}
	else
		p->bValid = FALSE;
}



void GC0308_Standby(BOOL b)
{
    
    if(b == FALSE)
    {
		gpio_clear_pin(CIM_STANDBY);

    }
    else
    {
		gpio_set_pin(CIM_STANDBY);
		mdelay(17);
    }
	//return;
}


//0 - 停止拍图； 1 - 启动拍图-纸质；2 - 启动拍图-纸质与屏幕；3 - 侦测状态；

void GC0308_Capture_Mode(int mode)
{
// printf("Capture mode: %d, old: %d\n", mode, nGC0308CaptureMode);

	if (nGC0308CaptureMode == mode)
		return;

 	switch(mode)
 	{
 	case 0: //停止拍图
 
 		if (nGC0308CaptureMode == 1)
 		{
                #if GC0308_USE_AEC
                    GC0308_reg_write(0xD2, 0x00);  
                    
// 			GC0308_reg_read(0xbb, &nGC0308_Exp_A);
// 			nGC0308_Gain_A = GC0308_INITGAIN;
// #else
// 			nGC0308_Exp_A = nGC0308_Exp;
// 			nGC0308_Gain_A = nGC0308_Gain;
// 
// // 			printf("stop e: %x, g:%x, v:%d---\r\n", nGC0308_Exp, nGC0308_Gain, nGC0308_VBlanking);
                #endif
// 
 		}
// 		else if (nGC0308CaptureMode == 2)
// 		{
// 			GC0308_reg_write(0x0B, 480); 	
// 			GC0308_reg_write(0x35, GC0308_INITGAIN); 	
// 			GC0308_reg_write(0x80, 0xf6); 	
// 			nGC0308_Exp_B = nGC0308_Exp_C = 480;
// 			nGC0308_Gain_B = nGC0308_Gain_C = GC0308_INITGAIN;
// 		}
// 		else if (nGC0308CaptureMode == 3)
// 		{
// 
// 		}
// 		GC0308_reg_write(0xaf, 0); 
// 
// 		nGC0308_VBlanking = GC0308_VB_MIN;
// 		GC0308_reg_write(0x6, nGC0308_VBlanking);
// 
 		break;
// 
 	case 1: //启动拍图-纸质
        #if GC0308_USE_AEC
            GC0308_reg_write(0xD2, 0x90); 
            
// 		GC0308_reg_write(0xBD, GC0308_MAXEXPOSUE_A); 	
// 		GC0308_reg_write(0x35, GC0308_INITGAIN); 
// 		GC0308_reg_write(0x80, GC0308_INITAG); 
// 		GC0308_reg_write(0xaf, 1); 
// 		
// #else
// 		GC0308_reg_write(0xaf, 0); 
// 
// 		nGC0308_Exp_B = (nGC0308_Exp_A/2)<40? 40 : (nGC0308_Exp_A/2);
// 		nGC0308_Gain_B = GC0308_INITGAIN;//nGC0308_Gain_A;	
// 		nGC0308_Exp_C = (nGC0308_Exp_A/4)<40? 40 : (nGC0308_Exp_A/4);
// 		nGC0308_Gain_C = GC0308_INITGAIN;//nGC0308_Gain_A;	
// 
// 		nGC0308_Exp = nGC0308_Exp_B;
// 		nGC0308_Gain = nGC0308_Gain_B;
// 		nGC0308_VBlanking = GC0308_VB_MIN;
// 
// 		GC0308_reg_write(0x0B, nGC0308_Exp); 	
// 		GC0308_reg_write(0x35, nGC0308_Gain); 	
// 
// // 		printf("B0  e: %d, g:%x, v:%d ===\r\n", nGC0308_Exp, nGC0308_Gain, nGC0308_VBlanking);
// 
        #endif
// 		nGC0308_VBlanking = GC0308_VB_MIN;
// 		GC0308_reg_write(0x6, nGC0308_VBlanking);
// 
 		break;
// 
// 	case 2:	//启动拍图-纸质与屏幕
// 		if (nGC0308CaptureMode == 1)
// 		{
// #if GC0308_USE_AEC
// 			GC0308_reg_read(0xbb, &nGC0308_Exp_A);
// 			nGC0308_Gain_A = GC0308_INITGAIN;
// #else
// 			nGC0308_Exp_A = nGC0308_Exp;
// 			nGC0308_Gain_A = nGC0308_Gain;
// #endif	
// 			nGC0308_Exp_B = nGC0308_Exp_C = 480;
// 			nGC0308_Gain_B = nGC0308_Gain_C = GC0308_INITGAIN;
// 		}
// 
// 
// 
// 		GC0308_reg_write(0x0B, nGC0308_Exp_A); 	
// 		GC0308_reg_write(0x35, nGC0308_Gain_A); 	
// 		GC0308_reg_write(0x80, 0xf6); 	
// 		GC0308_reg_write(0xaf, 0); 
// // 		foc_on_delay(1000);
// 		
// 		nGC0308_Exp = nGC0308_Exp_A;
// 		nGC0308_Gain = nGC0308_Gain_A;
// 		nGC0308_VBlanking = GC0308_VB_MIN;
// 
// 		break;
// 
// 	case 3: //侦测状态
// 		if (nGC0308CaptureMode == 1)
// 		{
// #if GC0308_USE_AEC
// 			GC0308_reg_read(0xbb, &nGC0308_Exp_A);
// 			nGC0308_Gain_A = GC0308_INITGAIN;
// #else
// 			nGC0308_Exp_A = nGC0308_Exp;
// 			nGC0308_Gain_A = nGC0308_Gain;		
// #endif
// 		}
// 
// 		GC0308_reg_write(0xBD, GC0308_MAXEXPOSUE_A); 	
// 		GC0308_reg_write(0x35, 0x30); 
// 		GC0308_reg_write(0x80, GC0308_INITAG); 
// 		GC0308_reg_write(0xaf, 1); 	
// 
// 		break;
 	default:
 		return;
 	}

	nGC0308CaptureMode = mode;
	LightCount = 0;

}

#if 0
void GC0308_Init(void)
{
	//初始化数组
	int nInit[][2] = {
		{0xFE, 0x90},	
		{0xFF, 0x01},//	未知寄存器	
		{0x0F, 0x00},		
 		{0x01, 0x6A},//Horizontal blanking		
		{0x02, 0x30},//Vertical blanking, 0x30 = 2.8ms
		{0xE2, 0x00},//flicker step MSB
		{0xE3, 0x96},//flicker step LSB

		//aec max level
		{0xE4, 0x02},
		{0xE5, 0x58},		
		{0xE6, 0x06},		
		{0xE7, 0x05},		
		{0xE8, 0x02},//最大曝光值不可超过0x2ff
		{0xE9, 0xff},
		{0xEA, 0x01},
		{0xEB, 0xe0},	
		{0xEC, 0x20},

		{0xe0, 0x09},
		{0xed, 0x04},//aec min exp		
		{0xee, 0xa0},//Digital post gain limit
		{0xef, 0x40},//
		
		{0x05, 0x00},
		{0x06, 0x00},	  
		{0x07, 0x00},
		{0x08, 0x00},
		{0x09, 0x01},
		{0x0A, 0xE8},//480
		{0x0B, 0x02},
        {0x0C, 0x88}, //640
		{0x0D, 0x02},
		{0x0E, 0x02},

		{0x10, 0x26},
		{0x11, 0x0D},
		{0x12, 0x2A},
		{0x13, 0x00},
		{0x14, 0x10},
		{0x15, 0x0A},
		{0x16, 0x05},
        {0x17, 0x01},	
        {0x18, 0x44},	
		{0x19, 0x44},
		
		{0x1A, 0x2A},
		{0x1B, 0x00},
        {0x1C, 0x49},	   
		//{0x19, 0x9A},///////////////////////
		{0x1E, 0x61},
		{0x1F, 0x23},

		{0x20, 0x08},//Interpolation enable
		{0x21, 0x02},/////////////////////////////////////{0x21, 0x00},//开启Y GAMMA校正，关闭其它图像处理功能
		{0x22, 0x00},//awb

		{0x24, 0xb1}, //only Y
		{0x25, 0x0F},
		{0x26, 0x03},//{0x26, 0x07},//
		{0x2F, 0x01},
		{0x30, 0xF7},
		{0x31, 0x50},
		{0x32, 0x00},

        {0x39, 0x04},	   
        {0x3A, 0x20},
        {0x3B, 0x20},
		{0x3C, 0x00},
		{0x3D, 0x00},
        {0x3E, 0x00},
        {0x3F, 0x00},


		{0x53, 0x80},
        {0x54, 0x80},
        {0x55, 0x80},
		{0x56, 0x80},
		{0x8B, 0x20},
		{0x8c, 0x20},/////////////////////////////////////
        {0x8d, 0x20},/////////////////////////////////////////
		{0x8E, 0x14},
		{0x8F, 0x10},

        {0x90, 0x14},
		{0x91, 0x3C},
		
		{0x92, 0x50},  
        {0x5D, 0x12},  
		{0x5E, 0x1A},
		{0x5F, 0x24},
        {0x60, 0x07},
		{0x61, 0x15},
		{0x62, 0x08},
		{0x64, 0x05},///////////////////////////////////{0x64, 0x08},
		{0x66, 0xE8},
		{0x67, 0x86},
		{0x68, 0xA2},
		{0x69, 0x18},
		{0x6A, 0x0F},
        {0x6B, 0x00},
        {0x6C, 0x5F},
		{0x6D, 0x8F},

        {0x6E, 0x55},
		{0x6F, 0x38},
		{0x70, 0x15},
		{0x71, 0x33},
        {0x72, 0xDC},
		{0x73, 0x80},
		{0x74, 0x02},
		{0x75, 0x3F},
		{0x76, 0x02},
		{0x77, 0x36},

		{0x78, 0x88},
		{0x79, 0x81},
		{0x7A, 0x81},
		{0x7B, 0x22},
		{0x7C, 0xFF},

        {0x93, 0x48},
		{0x94, 0x02},
        {0x95, 0x07},
        {0x96, 0xE0},
		{0x97, 0x40},

        {0x98, 0xF0},
		{0xB1, 0x40},
		{0xB2, 0x40},
		{0xB3, 0x40},
        {0xB6, 0xE0},
		{0xBD, 0x38},
		{0xBE, 0x36},
		{0xD0, 0xCB},
		{0xD1, 0x00},//AEC take action every N frame
		{0x50, 0x12}, //Global_gain
#if GC0308_USE_AEC
		{0xD2, 0x90},//AEC_en
#else
		{0xD2, 0x00},//AEC_en off
		{0x04, (GC0308_INITEXP&0xff)},//
		{0x03, ((GC0308_INITEXP>>8)&0x0f)},//

		{0x51, 0x40},
		{0x52, 0x40},

#endif

        {0xD3, 0x60},/////////////////////////////////{0xD3, 0x74},//AEC_target_Y
		{0xD5, 0xF2},
		{0xD6, 0x16},
		{0xDB, 0x92},
		{0xDC, 0xA5},

		{0xdf, 0x23},
		{0xd9, 0x00},
		{0xda, 0x00},
		{0x80, 0x03},
		{0x80, 0x03},
		{0x9f, 0x10},
		{0xa0, 0x20},
		{0xa1, 0x38},
		{0xa2, 0x4e},
		{0xa3, 0x63},
		{0xa4, 0x76},
		{0xa5, 0x87},
		{0xa6, 0xa2},
		{0xa7, 0xb8},
		{0xa8, 0xca},
		{0xa9, 0xd8},
		{0xaa, 0xe3},
		{0xab, 0xeb},
		{0xac, 0xf0},
		{0xad, 0xf8},
		{0xae, 0xfd},
		{0xaf, 0xff},
		{0xc0, 0x00},
		{0xc1, 0x10},
		{0xc2, 0x1c},
		{0xc3, 0x30},
		{0xc4, 0x43},
		{0xc5, 0x54},
		{0xc6, 0x65},
		{0xc7, 0x75},
		{0xc8, 0x93},
		{0xc9, 0xb0},
		{0xca, 0xcb},
		{0xcb, 0xe6},
		{0xcc, 0xff},
		{0xf0, 0x02},
		{0xf1, 0x01},
		{0xf2, 0x02},
		{0xf3, 0x30},
		{0xf7, 0x12},
		{0xf8, 0x0a},
		{0xf9, 0x9f},
		{0xfa, 0x78},

		//reg 1
		{0xfe, 0x01},
		{0x00, 0xf5},
		{0x02, 0x20},
		{0x04, 0x10},
		{0x05, 0x08},
		{0x06, 0x20},
		{0x08, 0x0a},
		{0x0a, 0xa0},
		{0x0b, 0x60},
		{0x0c, 0x08},
		{0x0e, 0x44},
		{0x0f, 0x32},
		{0x10, 0x41},
		{0x11, 0x37},
		{0x12, 0x22},
		{0x13, 0x19},
		{0x14, 0x44},
		{0x15, 0x44},
		{0x16, 0xc2},
		{0x17, 0xa8},
		{0x18, 0x18},
		{0x19, 0x50},
		{0x1a, 0xd8},
		{0x1b, 0xf5},
		{0x70, 0x40},
		{0x71, 0x58},
		{0x72, 0x30},
		{0x73, 0x48},
		{0x74, 0x20},
		{0x75, 0x60},
		{0x77, 0x20},
		{0x78, 0x32},
		{0x30, 0x03},
		{0x31, 0x40},
		{0x32, 0x10},
		{0x33, 0xe0},
		{0x34, 0xe0},
		{0x35, 0x00},
		{0x36, 0x80},
		{0x37, 0x00},
		{0x38, 0x04},
		{0x39, 0x09},
		{0x3a, 0x12},
		{0x3b, 0x1c},
		{0x3c, 0x28},
		{0x3d, 0x31},
		{0x3e, 0x44},
		{0x3f, 0x57},
		{0x40, 0x6c},
		{0x41, 0x81},
		{0x42, 0x94},
		{0x43, 0xa7},
		{0x44, 0xb8},
		{0x45, 0xd6},
		{0x46, 0xee},
		{0x47, 0x0d},
		{0xff, 0x01},
		{0xfe, 0x00},
		{0x04, (240&0xff)},//
		{0x03, ((240>>8)&0x0f)},//

	};
	int nCnt = sizeof(nInit)/(sizeof(int)*2);
	int i;

	for (i = 0; i < nCnt; i++)
	{//初始化关键寄存器
        GC0308_reg_write(nInit[i][0], nInit[i][1]);       
	}

// 	nGC0308_Exp_A = GC0308_MAXEXPOSUE_A;
// 	nGC0308_Gain_A = GC0308_INITGAIN;
// 	nGC0308_Exp_B = GC0308_MAXEXPOSUE_A;
// 	nGC0308_Gain_B = GC0308_INITGAIN;	
// 	nGC0308_VBlanking = GC0308_VB_MIN;
// 	nGC0308_Exp = GC0308_MAXEXPOSUE_A;
// 	nGC0308_Gain = GC0308_INITGAIN;
// 	GC0308_reg_write(0x06, GC0308_MAXEXPOSUE_B+90-480);  
	{
		unsigned int data[3] = {0,};
	
		GC0308_reg_read(0x51, &data[0]);
		GC0308_reg_read(0x52, &data[1]);
		GC0308_reg_read(0xd4, &data[2]);
		printf("0x51: %x,  52: %x, y: %x\r\n", data[0], data[1], data[2]);
		
	}

    mdelay(17);//必须满足一帧时间

	return;
}//End of function;
#else
void GC0308_Init(void)
{
	//初始化数组
	int nInit[][2] = {
		{0xFE, 0x90},	
		{0xFF, 0x01},//	未知寄存器	
		{0x0F, 0x00},		
 		{0x01, 0x6A},//Horizontal blanking		
		//{0x02, 0x70},//{0x02, 0x30},//Vertical blanking, 0x30 = 2.8ms/////////////////////////////////////////		
		{0x05, 0x00},
		{0x06, 0x00},	  
		{0x07, 0x00},
		{0x08, 0x00},
		{0x09, 0x01},
		{0x0A, 0xE8},//480
		{0x0B, 0x02},
             {0x0C, 0x88}, //640
		{0x0D, 0x02},
		{0x0E, 0x02},
		{0x10, 0x26},
		{0x11, 0x0D},
		{0x12, 0x2A},
		{0x13, 0x00},
		{0x14, 0x10},
		{0x15, 0x0A},
		{0x16, 0x05},
             {0x17, 0x01},	
             {0x18, 0x44},	
		{0x19, 0x44},	
		{0x1A, 0x2A},
		{0x1B, 0x00},
             {0x1C, 0x49},
             {0x1D, 0x9A},
             {0x1E, 0x61},		
		{0x1F, 0x23},

		{0x20, 0x08},//Interpolation enable
		{0x21, 0x02},
		{0x22, 0x00},//awb

		{0x24, 0xb1}, //only Y
		{0x25, 0x0F},
		{0x26, 0x03},//{0x26, 0x07},//
		{0x2F, 0x01},
		{0x30, 0xF7},
		{0x31, 0x50},
		{0x32, 0x00},

             {0x39, 0x04},	   
             {0x3A, 0x20},
             {0x3B, 0x20},
		{0x3C, 0x00},
		{0x3D, 0x00},
             {0x3E, 0x00},
             {0x3F, 0x00},
             
             {0x50, 0x20},//{0x50, 0x14},

		{0x53, 0x80},
             {0x54, 0x80},
             {0x55, 0x80},
		{0x56, 0x80},
		{0x8B, 0x20},
		{0x8c, 0x20},
		{0x8d, 0x20},
		{0x8E, 0x14},
		{0x8F, 0x10},
             {0x90, 0x14},
		{0x91, 0x3C},		
		{0x92, 0x50},  
             {0x5D, 0x12},  
		{0x5E, 0x1A},
		{0x5F, 0x24},
		
             {0x60, 0x07},
		{0x61, 0x15},
		{0x62, 0x08},
		{0x64, 0x03},//{0x64, 0x08},
		{0x66, 0xE8},
		{0x67, 0x86},
		{0x68, 0xA2},
		{0x69, 0x18},
		{0x6A, 0x0F},
             {0x6B, 0x00},
             {0x6C, 0x5F},
		{0x6D, 0x8F},

             {0x6E, 0x55},
		{0x6F, 0x38},
		{0x70, 0x15},
		{0x71, 0x33},
             {0x72, 0xDC},
		{0x73, 0x80},
		{0x74, 0x02},
		{0x75, 0x3F},
		{0x76, 0x02},
		{0x77, 0x36},

		{0x78, 0x88},
		{0x79, 0x81},
		{0x7A, 0x81},
		{0x7B, 0x22},
		{0x7C, 0xFF},

             {0x93, 0x48},
		{0x94, 0x02},
             {0x95, 0x07},
             {0x96, 0xE0},
		{0x97, 0x40},
             {0x98, 0xF0},
        
		{0xB1, 0x40},
		{0xB2, 0x40},
		{0xB3, 0x40},
             {0xB6, 0xE0},
		{0xBD, 0x38},
		{0xBE, 0x36},
		{0xc0, 0x00},
		{0xc1, 0x10},
		{0xc2, 0x1c},
		{0xc3, 0x30},
		{0xc4, 0x43},
		{0xc5, 0x54},
		{0xc6, 0x65},
		{0xc7, 0x75},
		{0xc8, 0x93},
		{0xc9, 0xb0},
		{0xca, 0xcb},
		{0xcb, 0xe6},
		{0xcc, 0xff},
		{0xD0, 0xCB},
		{0xD1, 0x10},//{0xD1, 0x00},//AEC take action every N frame
		//{0x50, 0x12}, //Global_gain
#if GC0308_USE_AEC
		{0xD2, 0x90},//AEC_en
#else
		{0xD2, 0x00},//AEC_en off
		//{0x04, (GC0308_INITEXP&0xff)},//
		//{0x03, ((GC0308_INITEXP>>8)&0x3f)},//

		//{0x51, 0x40},
		//{0x52, 0x40},

#endif

        {0xD3, 0x38},////////////////////////{0xD3, 0x74},//AEC_target_Y
		{0xD5, 0xF2},
		{0xD6, 0x16},
		{0xDB, 0x92},
		{0xDC, 0xA5},

		{0xdf, 0x23},
		{0xd9, 0x00},
		{0xda, 0x00},
		{0xe0, 0x09},
		{0xE2, 0x00},//flicker step MSB
		{0xE3, 0x40},//////////{0xE3, 0x96},//flicker step LSB
		//aec max level
		{0xE4, 0x02},
		{0xE5, 0x58},		
		{0xE6, 0x03},		
		{0xE7, 0x84},		
		{0xE8, 0x07},
		{0xE9, 0x08},
		{0xEA, 0x0d},//{0xEA, 0x01},
		{0xEB, 0x7a},//{0xEB, 0xe0},	
		{0xEC, 0x20},//{0xEC, 0x30},		
		{0xed, 0x04},
		{0xee, 0xa0},
		{0xef, 0x40},
		
		{0x80, 0x03},
		{0x80, 0x03},
		{0x9f, 0x10},
		{0xa0, 0x20},
		{0xa1, 0x38},
		{0xa2, 0x4e},
		{0xa3, 0x63},
		{0xa4, 0x76},
		{0xa5, 0x87},
		{0xa6, 0xa2},
		{0xa7, 0xb8},
		{0xa8, 0xca},
		{0xa9, 0xd8},
		{0xaa, 0xe3},
		{0xab, 0xeb},
		{0xac, 0xf0},
		{0xad, 0xf8},
		{0xae, 0xfd},
		{0xaf, 0xff},
	
		{0xf0, 0x02},
		{0xf1, 0x01},
		{0xf2, 0x02},
		{0xf3, 0x30},
		{0xf7, 0x12},
		{0xf8, 0x0a},
		{0xf9, 0x9f},
		{0xfa, 0x78},


		

		

		//{0xe0, 0x09},
		//{0xed, 0x04},//aec min exp		
		//{0xee, 0xa0},//Digital post gain limit
		//{0xef, 0x40},//

		//reg 1
	/*	{0xfe, 0x01},
		{0x00, 0xf5},
		{0x02, 0x20},
		{0x04, 0x10},
		{0x05, 0x08},
		{0x06, 0x20},
		{0x08, 0x0a},
		{0x0a, 0xa0},
		{0x0b, 0x60},
		{0x0c, 0x08},
		{0x0e, 0x44},
		{0x0f, 0x32},
		{0x10, 0x41},
		{0x11, 0x37},
		{0x12, 0x22},
		{0x13, 0x19},
		{0x14, 0x44},
		{0x15, 0x44},
		{0x16, 0xc2},
		{0x17, 0xa8},
		{0x18, 0x18},
		{0x19, 0x50},
		{0x1a, 0xd8},
		{0x1b, 0xf5},
		{0x70, 0x40},
		{0x71, 0x58},
		{0x72, 0x30},
		{0x73, 0x48},
		{0x74, 0x20},
		{0x75, 0x60},
		{0x77, 0x20},
		{0x78, 0x32},
		{0x30, 0x03},
		{0x31, 0x40},
		{0x32, 0x10},
		{0x33, 0xe0},
		{0x34, 0xe0},
		{0x35, 0x00},
		{0x36, 0x80},
		{0x37, 0x00},
		{0x38, 0x04},
		{0x39, 0x09},
		{0x3a, 0x12},
		{0x3b, 0x1c},
		{0x3c, 0x28},
		{0x3d, 0x31},
		{0x3e, 0x44},
		{0x3f, 0x57},
		{0x40, 0x6c},
		{0x41, 0x81},
		{0x42, 0x94},
		{0x43, 0xa7},
		{0x44, 0xb8},
		{0x45, 0xd6},
		{0x46, 0xee},
		{0x47, 0x0d},
		{0xff, 0x01},
		{0xfe, 0x00},*/

	};
	int nCnt = sizeof(nInit)/(sizeof(int)*2);
	int i;
	printf("GC0308 Initial\n");
	for (i = 0; i < nCnt; i++)
	{//初始化关键寄存器
        GC0308_reg_write(nInit[i][0], nInit[i][1]);       
	}

// 	nGC0308_Exp_A = GC0308_MAXEXPOSUE_A;
// 	nGC0308_Gain_A = GC0308_INITGAIN;
// 	nGC0308_Exp_B = GC0308_MAXEXPOSUE_A;
// 	nGC0308_Gain_B = GC0308_INITGAIN;	
// 	nGC0308_VBlanking = GC0308_VB_MIN;
// 	nGC0308_Exp = GC0308_MAXEXPOSUE_A;
// 	nGC0308_Gain = GC0308_INITGAIN;
// 	GC0308_reg_write(0x06, GC0308_MAXEXPOSUE_B+90-480);  
	{
		unsigned int data[3] = {0,};
	
		GC0308_reg_read(0x51, &data[0]);
		GC0308_reg_read(0x52, &data[1]);
		GC0308_reg_read(0xd4, &data[2]);
		printf("0x51: %x,  52: %x, y: %x\r\n", data[0], data[1], data[2]);
		
	}

    mdelay(17);//必须满足一帧时间

	return;
}//End of function;
#endif

int GC0308_Width(void)
{
    return  GC0308_IMAGE_W;
}

int GC0308_Height(void)
{
    return GC0308_IMAGE_H;
}

int GC0308_Width_For_CSI(void)
{
	return GC0308_IMAGE_W;
}

int GC0308_Height_For_CSI(void)
{
	return GC0308_IMAGE_H;
}



//获取CMOS提供的当前亮度
//pImgData 为采集的原始图像，需要转换为实际有效图像再计算
int GC0308_GetLux(unsigned char* pImgData)	
{
	unsigned char *pImg = pImgData;
	int i,j,nCount;
	unsigned int nSum;
// 	int bCount[256];
// 	int max1,max2;

	if (pImg == NULL)
	{ return 0; }

#if 0

// 中间1/4面积计算亮度
	pImg = pImgData;
	pImg += ((GC0308_IMAGE_H/4)*GC0308_IMAGE_W);
	nSum = 0;
	nCount = 0;


	//重点测中央的1/16面积
	for (j = (GC0308_IMAGE_H/4); j < (GC0308_IMAGE_H*3/4); j += 3)
	{
		for (i = (GC0308_IMAGE_W/4); i < (GC0308_IMAGE_W*3/8); i += 2)
		{
			nSum += pImg[i];
			nCount ++;
// 			bCount[pImg[i]]++;
		}//End of for;
		for ( ; i < (GC0308_IMAGE_W*5/8); i += 1)
		{
			nSum += pImg[i];
			nCount ++;
// 			bCount[pImg[i]]++;
		}//End of for;
		for ( ; i < (GC0308_IMAGE_W*3/4); i += 2)
		{
			nSum += pImg[i];
			nCount ++;
// 			bCount[pImg[i]]++;
		}//End of for;
		pImg += GC0308_IMAGE_W*3;
	}//End of for;

#else
	pImg = pImgData;
	nSum = 0;
	nCount = 0;
	i =  GC0308_IMAGE_W*GC0308_IMAGE_H;

	while(nCount<i)
	{
		nSum += *pImg++;
		nCount++;
	}

	

#endif

	return (int)(nSum / nCount);
}//End of function;




void GC0308_Adjust_n(int nCurLux, int nWantLux, int *pExp, int *pGain, int nMaxExp, int nMaxGain)
{
#define MT9_SCALE_G2E	9		//效用系数
	int nScale = 0;
	int nTmp;
	int nNewGain,nNewExp;
	int newLux;


	nTmp = (int)((*pGain-0x10) * MT9_SCALE_G2E);
	nScale = *pExp + nTmp; //常数项为增益值相对于曝光值的效用系数
	nScale = (nWantLux * nScale) / (max(1,nCurLux)); //计算新的值

#if GC0308_NO_GAIN

	nNewExp = nScale - nTmp;


	if (nNewExp> nMaxExp)
		nNewExp = nMaxExp;


	if (nNewExp<1)
	{
// 	printf("\r\nGain: %x, newE: %d\r\n", *pGain, nNewExp);
		nNewExp = 1;
	}

#else

	if (nScale > nMaxExp)
	{
		nTmp = nScale-nMaxExp;
		nScale = nMaxExp;
		nTmp = (int)(nTmp / MT9_SCALE_G2E) + 0x10;
		if (nTmp > nMaxGain)
		{ nTmp = nMaxGain; }
		
		nNewGain = nTmp;
		nNewExp = nMaxExp;
	}//End of if;
	else
	{
		//		nGain = 0x10;
		//		nExp = max(1,nScale);
		nNewGain = 0x10;
		nNewExp = max(1,nScale);
	}//End of else;
	
#endif

#if ! GC0308_NO_GAIN
	*pGain = nNewGain;
	nGC0308_Gain = nNewGain;
#endif

	*pExp = nNewExp;
	nGC0308_Exp = nNewExp;

	if (nGC0308_Exp>GC0308_MAXEXPOSUE_A)
		nGC0308_VBlanking = nGC0308_Exp+GC0308_VB_MIN-GC0308_MAXEXPOSUE_A;
	else
		nGC0308_VBlanking = GC0308_VB_MIN;
	
#undef MT9_SCALE_G2E
}//End of function;


void GC0308_Exposure_update(void)
{
#if GC0308_USE_AEC
	if (nGC0308CaptureMode == 1)
		return;
#endif

	return;


	GC0308_reg_write(0x04, nGC0308_Exp&0xff);
	GC0308_reg_write(0x03, (nGC0308_Exp>>8)&0x3f);
#if ! GC0308_NO_GAIN
 	GC0308_reg_write(0x35, nGC0308_Gain);
#endif
	GC0308_reg_write(0x6, nGC0308_VBlanking);

//  	printf("e: %d, g:%x, v:%d\r\n", nGC0308_Exp, nGC0308_Gain, nGC0308_VBlanking);
// 	printf("e: %x, v:%d\r\n", nGC0308_Exp, nGC0308_VBlanking);
// 	printf("\r\n");

}

int aa1=0, aa2=0;

void GC0308_Adjust(int nWantLux, unsigned char* pImgData, unsigned int nCount, unsigned int nLightOn, unsigned int nLightOff)
{
	int n;
	int *pExp, *pGain, maxExp, maxG;
return;

	if (nGC0308CaptureMode == 1)
	{
#if GC0308_USE_AEC
	
// 		printf("lux: %02x\r\n", GC0308_GetLux(pImgData));

		return;
#else
// 		if (nCount == 0)
// 		{
// 			nGC0308_Exp = nGC0308_Exp_C;
// 			nGC0308_Gain = nGC0308_Gain_C;
// 			nGC0308_VBlanking = GC0308_VB_MIN;
// // 			printf("C0  ");
// 			return;
// 		}
// 		else if (nCount % 3 == 1)//曝光期间照明灯
// 		{
// 			pExp = &nGC0308_Exp_A;
// 			pGain = &nGC0308_Gain_A;
// 			maxExp = GC0308_MAXEXPOSUE_A;
// // 			printf("A  ");
// 		}
// 		else if (nCount % 3  == 2)	//曝光期间照明灯
// 		{
// 			pExp = &nGC0308_Exp_B;
// 			pGain = &nGC0308_Gain_B;
// 			maxExp = GC0308_MAXEXPOSUE_A;		
// // 			printf("B  ");
// 		}
// 		else if (nCount % 3 == 0)	//曝光期间照明灯
// 		{
// 			pExp = &nGC0308_Exp_C;
// 			pGain = &nGC0308_Gain_C;
// 			maxExp = GC0308_MAXEXPOSUE_A;
// // 			printf("C  ");
// 		}
// 		maxG = GC0308_MAXGAIN;
#endif
// 
// 	}
// 	else if (nGC0308CaptureMode == 2)
// 	{
// 		if (nCount % 2 == 0)//曝光期间无照明灯
// 		{
// 			pExp = &nGC0308_Exp_B;
// 			pGain = &nGC0308_Gain_B;
// 			maxExp = GC0308_MAXEXPOSUE_B;
// 		}
// 		else if (nCount % 2 == 1)	//曝光期间照明灯
// 		{
// 			pExp = &nGC0308_Exp_A;
// 			pGain = &nGC0308_Gain_A;
// 			maxExp = GC0308_MAXEXPOSUE_A;
// 		}
// 
// 		maxG = 0x20;
// 
// 	}
// 	else
// 	{
// 		return;
	}

#if 0
			pExp = &nGC0308_Exp_A;
			pGain = &nGC0308_Gain_A;
			maxExp = GC0308_MAXEXPOSUE_A;
			maxG = GC0308_MAXGAIN;
			
			if ((aa2 % 5) == 4)
			{
#if 0
				switch (aa1%10)
				{
				case 0:
					nGC0308_Exp_A = 80;
					GC0308_reg_write(0x4, nGC0308_Exp_A&0xff);
					GC0308_reg_write(0x3, (nGC0308_Exp_A>>8)&0xff);
					GC0308_reg_write(0x52, 0x40);
					break;
				case 1:
					nGC0308_Exp_A = 180;
					GC0308_reg_write(0x4, nGC0308_Exp_A&0xff);
					GC0308_reg_write(0x3, (nGC0308_Exp_A>>8)&0xff);
					GC0308_reg_write(0x52, 0x40);
					break;
				case 2:
					nGC0308_Exp_A = 280;
					GC0308_reg_write(0x4, nGC0308_Exp_A&0xff);
					GC0308_reg_write(0x3, (nGC0308_Exp_A>>8)&0xff);
					GC0308_reg_write(0x52, 0x40);
					break;
				case 3:
					nGC0308_Exp_A = 380;
					GC0308_reg_write(0x4, nGC0308_Exp_A&0xff);
					GC0308_reg_write(0x3, (nGC0308_Exp_A>>8)&0xff);
					GC0308_reg_write(0x52, 0x40);
					break;
				case 4:
					nGC0308_Exp_A = 480;
					GC0308_reg_write(0x4, nGC0308_Exp_A&0xff);
					GC0308_reg_write(0x3, (nGC0308_Exp_A>>8)&0xff);
					GC0308_reg_write(0x52, 0x40);
					break;
					
				case 5:
					nGC0308_Exp_A = 580;
					GC0308_reg_write(0x4, nGC0308_Exp_A&0xff);
					GC0308_reg_write(0x3, (nGC0308_Exp_A>>8)&0xff);
					GC0308_reg_write(0x52, 0x40);
					break;
					
				case 6:
					nGC0308_Exp_A = 680;
					GC0308_reg_write(0x4, nGC0308_Exp_A&0xff);
					GC0308_reg_write(0x3, (nGC0308_Exp_A>>8)&0xff);
					GC0308_reg_write(0x52, 0x40);
					break;
					
				case 7:
					nGC0308_Exp_A = 480;
					GC0308_reg_write(0x4, nGC0308_Exp_A&0xff);
					GC0308_reg_write(0x3, (nGC0308_Exp_A>>8)&0xff);
					GC0308_reg_write(0x52, 0x42);
					break;
					
				case 8:
					nGC0308_Exp_A = 480;
					GC0308_reg_write(0x4, nGC0308_Exp_A&0xff);
					GC0308_reg_write(0x3, (nGC0308_Exp_A>>8)&0xff);
					GC0308_reg_write(0x52, 0x43);
					break;
					
				case 9:
					nGC0308_Exp_A = 480;
					GC0308_reg_write(0x4, nGC0308_Exp_A&0xff);
					GC0308_reg_write(0x3, (nGC0308_Exp_A>>8)&0xff);
					GC0308_reg_write(0x52, 0x44);
					break;
					
				}
				printf("set exp: %d, aa1: %d\r\n", nGC0308_Exp_A, aa1);
				aa1++;
#endif

#if 0
{

	if ((aa1==0) || (aa1 >= 0x40))
	{
		aa1 = 0x10;
	}

		GC0308_reg_write(0x50, aa1);
			
		printf("set all gain: %x\r\n", aa1);
	aa1++;
}

#endif

#if 0
{
	
	if ((aa1==0) || (aa1 >= 0xC0))
	{
		aa1 = 0x40;
	}
	
	GC0308_reg_write(0x52, aa1);
	
	printf("set post gain: %x\r\n", aa1);
	aa1++;
}

#endif

#if 1
{
	
	if ((aa1==0) || (aa1 >= 0xC0))
	{
		aa1 = 0x40;
	}
	
	GC0308_reg_write(0x51, aa1);
	
	printf("set pre gain: %x\r\n", aa1);
	aa1++;
}

#endif
				
			}
			aa2 ++;


	n = GC0308_GetLux(pImgData);

// 	printf("g: %x, %x, %x\r\n", nGC0308_Gain_A, nGC0308_Gain_B, nGC0308_Gain_C);
// 	printf("lux: %02x\r\n", n);

	{
		unsigned int data[6] = {0,};
		
		GC0308_reg_read(0x51, &data[0]);
		GC0308_reg_read(0x52, &data[1]);
		GC0308_reg_read(0xd4, &data[2]);
		GC0308_reg_read(0x3, &data[3]);
		GC0308_reg_read(0x4, &data[4]);
		GC0308_reg_read(0x50, &data[5]);

		printf("lux: %02x y: %02x, preG: %x,  postG: %x, G: %x, expH: %x, expL: %x, exp: %x, %d\r\n", n, data[2], data[0], data[1], data[5], data[3], data[4], nGC0308_Exp_A,nGC0308_Exp_A);
		
	}


#endif

// 	GC0308_Adjust_n(n, nWantLux, pExp, pGain, maxExp, maxG);


// 	printf("n:%d, lux:%x, A e:%x, g:%x; B e:%x, g:%x\n", nCount, n, nGC0308_Exp_A, nGC0308_Gain_A, nGC0308_Exp_B,nGC0308_Gain_B);

}


////nMode: 0 - Normal; 1 - horizontal mirror; 2 - vertical flip; 3 - horizontal mirror & vertical flip.
void GC0308_Mirror(int mode)
{
    	int data = 0;
      //unsigned char val[2];
		printf("Mirror\n");
	  if (GC0308_reg_read(0x14, &data) != 0)
		  return;
      
	  data &= ~(3);

	if (mode == 1)	//horizontal mirror 
	{
		data |= 1;
	}
	else if (mode == 2) //vertical flip
	{
		data |= 1<<1;
	}
	else if (mode == 3)
	{
		data |= 3;
	}

    GC0308_reg_write(0x14, data);
    
}

unsigned char * GC0308_ImgData(unsigned char *p, unsigned int *pFlag)	//图像无需转换
{
	if (pFlag) *pFlag = CMOS_FLAG_DATA_PROCESSED;
	return p;	
}


