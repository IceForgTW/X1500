#include <x1000.h>
#include <list.h>
#include "../i2c/i2c.h"
#include "cmos.h"
#include "ov9282.h"

#include <gpio.h>
#include <time-tcu.h>
#include <stdio.h>

// pclk 104MHz

#define OV9282_DBG_ENABLE   0
#define OV9282_NO_GAIN		0

/* Maximum exposure time is frame length - 12 row periods,where frame length is set by registers {0x380E,0x380F}( 0x482-0xc) 
  * Minimum exposure time is 1 row period.
  */
#define OV9282_MAXEXPOSUE_A		0x1820           // 4ms
#define OV9282_MAXEXPOSUE_B		0x2820           // 8ms
#define OV9282_MAXEXPOSUE_C		0x4820           // 16ms 
#define OV9282_MINEXPOSUE         0x490            // 1ms

/* */
#define OV9282_MAXGAIN				0x40
#define OV9282_MINGAIN			    0x10
#define OV9282_INITAG				0xF6
#define OV9282_VB_MIN				45


#define OV9292_EXP_REG_H          0x3501
#define OV9292_EXP_REG_L          0x3502

#define OV9292_GAIN_REG           0x3509
static int LightCount = 0;


static unsigned char OV9282_slave_addr = 0x20;
static int nOV9282CaptureMode = 0;

static unsigned int nOv9282_Exp_A = OV9282_MAXEXPOSUE_A;
static unsigned int nOv9282_Gain_A = OV9282_MAXGAIN;

static unsigned int nOv9282_Exp_B = OV9282_MAXEXPOSUE_B;
static unsigned int nOv9282_Gain_B = OV9282_MAXGAIN;

static unsigned int nOv9282_Exp_C = OV9282_MAXEXPOSUE_C;
static unsigned int nOv9282_Gain_C = OV9282_MAXGAIN;

static unsigned int nOv9282_VBlanking = OV9282_VB_MIN;
static unsigned int nOv9282_Exp = OV9282_MAXEXPOSUE_B;
static unsigned int nOv9282_Gain = OV9282_MINGAIN;


extern volatile unsigned char nLightCtrlMode;;
extern volatile unsigned char nFocCtrlMode;
extern volatile unsigned char nExtLightCtrlMode;

int OV9282_reg_read (unsigned int addr, unsigned int *buf)
{
    int ret;
    //int t1,t2;
     
    //t1=Get_PerformanceCounter()*10/15;
	printf("OV9282_reg_read:%d %x %x\n",i2c_1,OV9282_slave_addr, addr);
    ret = i2c_read16(i2c_1, OV9282_slave_addr >> 1, (unsigned char *)buf, addr, 1);

    //t2=Get_PerformanceCounter()*10/15;
    //printf("rt1=%d, rt2=%d\n",t1,t2);
#if OV9282_DBG_ENABLE
	if (ret != 0)
		printf("i2c read error: %d\r\n", ret);
#endif
    

	return ret;
}

int OV9282_reg_write(unsigned int addr, unsigned int data)
{
    int ret;
    //int t1,t2;
    //t1=Get_PerformanceCounter()*10/15;
	
    ret = i2c_write16(i2c_1, OV9282_slave_addr >> 1, (unsigned char *)&data, addr, 1);
    //t2=Get_PerformanceCounter()*10/15;
    //printf("w=%d\n",t2-t1);
#if OV9282_DBG_ENABLE
	if (ret !=0)
		printf("i2c write error: %d\r\n", ret);
#endif

// 	printf("i2c write: %02x, %d\r\n", addr, data);

    return ret;
}

void _is_Decoding(int nFlag)
{
    return;
}

static void OV9282_set_exp(unsigned short val)
{
   OV9282_reg_write(OV9292_EXP_REG_H, val>>8);
   OV9282_reg_write(OV9292_EXP_REG_L, val);
}

static void OV9282_set_gain(unsigned char val)
{
   OV9282_reg_write(OV9292_GAIN_REG, val);
}

BOOL IsOV9282(void)
{
	unsigned int data_low = 0, data_high=0, chip_id=0;
	
	OV9282_reg_read(OV9282_ID_REG, &data_low);
	OV9282_reg_read(OV9282_ID_REG+1, &data_high);

	chip_id = ((data_high<<8) | data_low);

#if OV9282_DBG_ENABLE
	printf("chip id=%x %x %x\n",chip_id, data_high, data_low);
#endif
	if(chip_id == 0x8192)
		return TRUE;
	return FALSE;
}

void OV9282_Info(CSINF *p)
{
	if (!p) return;

#if OV9282_DBG_ENABLE
	printf("OV9282_InfoEnter\n");
#endif
	

	if (IsOV9282())
	{
		p->bValid = TRUE;
		p->nImgFormat = IMG_FORMAT_MONOCHROME_8BIT;
		p->nShutterType = CMOS_SHUTTER_TYPE_GLOBAL;
		p->nInputClk = 24000000;
		p->nI2cClk = CMOS_I2C_CLK_HIGH;
	}
	else
		p->bValid = FALSE;
}

/* ov9282 没有standby 信号控制*/
void OV9282_Standby(BOOL b)
{
	return;
}

void OV9282_snapshot_init(void)
{
     __gpio_as_output(CIM_EXPOSURE);
     __gpio_clear_pin(CIM_EXPOSURE);
	return;
}

//0 - 停止拍图； 1 - 启动拍图-纸质；2 - 启动拍图-纸质与屏幕；3 - 侦测状态；
void OV9282_Capture_Mode(int mode)
{
#if OV9282_DBG_ENABLE
     printf("Capture mode: %d, old: %d\n", mode, nOV9282CaptureMode);
#endif
	if (nOV9282CaptureMode == mode)
		return;

	switch(mode)
	{
	case 0: //停止拍图

		if (nOV9282CaptureMode == 1)
		{
			nOv9282_Exp_B = nOv9282_Exp;
			nOv9282_Gain_B = nOv9282_Gain;

// 			printf("stop e: %x, g:%x, v:%d---\r\n", nOv9282_Exp, nOv9282_Gain, nOv9282_VBlanking);

		}
		else if (nOV9282CaptureMode == 2)
		{
			OV9282_set_exp(480);
			OV9282_set_gain(OV9282_MINGAIN);
			nOv9282_Exp_B = nOv9282_Exp_C = 480;
			nOv9282_Gain_B = nOv9282_Gain_C = OV9282_MINGAIN;
		}
		else if (nOV9282CaptureMode == 3)
		{

		}
		break;

	case 1: //启动拍图-纸质
		nOv9282_Exp = nOv9282_Exp_C;
		nOv9282_Gain = nOv9282_Gain_C;

		OV9282_set_exp(nOv9282_Exp); 	
		OV9282_set_gain(nOv9282_Gain); 	
#if OV9282_DBG_ENABLE
		printf("B0  e: %x, g:%x===\r\n", nOv9282_Exp, nOv9282_Gain);
#endif
 		
		break;

	case 2:	//启动拍图-纸质与屏幕
		if (nOV9282CaptureMode == 1)
		{
			nOv9282_Exp_A = nOv9282_Exp;
			nOv9282_Gain_A = nOv9282_Gain;
			nOv9282_Exp_B = nOv9282_Exp_C = 480;
			nOv9282_Gain_B = nOv9282_Gain_C = OV9282_MINGAIN;
		}

		OV9282_set_exp(nOv9282_Exp_A); 	
		OV9282_set_gain( nOv9282_Gain_A); 	
// 		foc_on_delay(1000);
		
		nOv9282_Exp = nOv9282_Exp_A;
		nOv9282_Gain = nOv9282_Gain_A;
		nOv9282_VBlanking = OV9282_VB_MIN;

		break;

	case 3: //侦测状态
		if (nOV9282CaptureMode == 1)
		{
			nOv9282_Exp_A = nOv9282_Exp;
			nOv9282_Gain_A = nOv9282_Gain;		
		}

		OV9282_set_exp(OV9282_MAXEXPOSUE_A);
		OV9282_set_gain(OV9282_INITAG); 

		break;
	default:
		return;
	}

	nOV9282CaptureMode = mode;

}


void OV9282_Init(void)
{
	//初始化数组
	int nInit[][2] = {
		{0x0103, 0x01},

		//26MHz MCLK 
		{0x0302, 0x30},
		{0x030D, 0x60},
		{0x030E, 0x06},
		
		{0x3001, 0x62},
		{0x3004, 0x01},
		{0x3005, 0xFF},
		{0x3006, 0xEA}, // 0xE2  //0xEA enable STROBE output
		{0x3011, 0x0A},
		{0x3013, 0x18},
		{0x3022, 0x07},
		{0x3030, 0x10},
		{0x3039, 0x2E},
		{0x303A, 0xF0},

		//AEC/AGC
		{0x3500, 0x00},
		{0x3501, 0x2a},   //F0
		{0x3502, 0x90},   //F4 
		{0x3503, 0x08},
		{0x3505, 0x8C},
		{0x3507, 0x03},
		{0x3508, 0x00},
		{0x3509, 0x40},  // gain

		//alalog control
		{0x3610, 0x80},
		{0x3611, 0xA0},
		{0x3620, 0x6F},
		{0x3632, 0x56},
		{0x3633, 0x78},
		{0x3662, 0x05},
		{0x3666, 0x5A},
		{0x366F, 0x7E},
		{0x3680, 0x84},

		//sensor control
		{0x3712, 0x80},
		{0x372D, 0x22},
		{0x3731, 0x80},
		{0x3732, 0x30},
		{0x3778, 0x00},
		{0x377D, 0x22},
		{0x3788, 0x02},
		{0x3789, 0xA4},
		{0x378A, 0x00},
		{0x378B, 0x4A},
		{0x3799, 0x20},

        //timing control 
		{0x3800, 0x00},
		{0x3801, 0x00},
		{0x3802, 0x00},
		{0x3803, 0x00},
		{0x3804, 0x05},
		{0x3805, 0x0F},
		{0x3806, 0x03},
		{0x3807, 0x2F},

		{0x3808, 0x02},  //
		{0x3809, 0xF0},  //80--640	  F0--752
		{0x380A, 0x01},  //
		{0x380B, 0xE0},  //90
		
         // 2 * HTS(0x380c,0x380d) * VTS(0x380e,0x380d) * FPS=PCLK. 
		{0x380C, 0x02},  //02 
		{0x380D, 0xd8},   //D8
		{0x380E, 0x04},  //02
		{0x380F, 0x8E},   //08

		{0x3810, 0x01}, //00
		{0x3811, 0x08},  //04
		{0x3812, 0x00},  //00
		{0x3813, 0x08},  //04

		{0x3814, 0x11},
		{0x3815, 0x11},
		{0x3820, 0x40},
		{0x3821, 0x00},
		{0x3881, 0x42},
		{0x38B1, 0x00},
		{0x3920, 0xFF},
        {0x3927, 0x04},

		{0x4003, 0x40},  //blc
		{0x4008, 0x04},  //02
		{0x4009, 0x0B},  //05
		{0x400C, 0x00},
		{0x400D, 0x07},   //03
		{0x4010, 0x40},
		{0x4043, 0x40},
		{0x4307, 0x30},
		{0x4317, 0x01},  //DVP Enable
		{0x4501, 0x00},  
		{0x4507, 0x00},
		{0x4509, 0x00},
		{0x450A, 0x08},
		{0x4601, 0x04},

		{0x470f, 0xE0},  //00E0
		{0x4f07, 0x00},
		{0x4800, 0x00},
		{0x5000, 0x9F},  //ISP功能
		{0x5001, 0x00},
		{0x5e00, 0x00},
		{0x5d00, 0x0B},
		{0x5d01, 0x02},
		{0x0100, 0x01}   //启动视频流
	};
	int nCnt = sizeof(nInit)/(sizeof(int)*2);
	int i;
	OV9282_reg_write(nInit[0][0], nInit[0][1]); 
	mdelay(2);

	for (i = 1; i < nCnt; i++)
	{//初始化关键寄存器
        OV9282_reg_write(nInit[i][0], nInit[i][1]);       
	}

	OV9282_snapshot_init();//初始化CMOS的曝光控制引脚,待用于SNAPSHOT模式
    mdelay(17);//必须满足一帧时间

	return;
}//End of function;

int OV9282_Width(void)
{
    return  OV9282_IMAGE_W;
}

int OV9282_Height(void)
{
    return OV9282_IMAGE_H;
}

int OV9282_Width_For_CSI(void)
{
	return OV9282_IMAGE_W;
}

int OV9282_Height_For_CSI(void)
{
	return OV9282_IMAGE_H;
}



//获取CMOS提供的当前亮度
int OV9282_GetLux(unsigned char* pImgData)
{
	unsigned char *pImg = pImgData;
	int i,j,nCount;
	unsigned int nSum;
// 	int bCount[256];
// 	int max1,max2;

	if (pImg == NULL)
	{ return 0; }


// 中间1/4面积计算亮度
	pImg = pImgData;
	pImg += ((480/4)*752);
	nSum = 0;
	nCount = 0;


	//重点测中央的1/16面积
	for (j = (480/4); j < (480*3/4); j += 3)
	{
		for (i = (752/4); i < (752*3/8); i += 2)
		{
			nSum += pImg[i];
			nCount ++;
// 			bCount[pImg[i]]++;
		}//End of for;
		for ( ; i < (752*5/8); i += 1)
		{
			nSum += pImg[i];
			nCount ++;
// 			bCount[pImg[i]]++;
		}//End of for;
		for ( ; i < (752*3/4); i += 2)
		{
			nSum += pImg[i];
			nCount ++;
// 			bCount[pImg[i]]++;
		}//End of for;
		pImg += 752*3;
	}//End of for;


	return (int)(nSum / nCount);
}//End of function;
#if 0
typedef struct ___RECT___{
	int top;
	int left;
	int right;
	int bottom;
} RECT;

int OV9282_GetLuxExp(unsigned char* pImgData, RECT *pRect, int nMax)
{
	unsigned char *pImg = pImgData;
	int i,j,k, nCount;
	unsigned int nSum;

	if (pImg == NULL)
	{ return 0; }

    //对于2个以上的区域直接采用中心区域的方式获取亮度均值。
    if(nMax > 2)
    {
        OV9282_GetLux(pImgData);
    }
    
	nSum = 0;
	nCount = 0;

    for(k=0; k<nMax; k++)
    {
    	pImg = pImgData;
    	pImg += (pRect->top * 752);
    	
        for(j=pRect->top; j<pRect->bottom; j++)
        {
            for(i=pRect->left; i<pRect->right; i++)
            {
                nSum += pImg[i];
                nCount ++;
            }
            pImg += 752;
        }
        pRect++;
    }

	return (int)(nSum / nCount);
}//End of function;
#endif

void OV9282_Adjust_n(unsigned int nCurLux, unsigned int nWantLux, unsigned int *pExp, unsigned int *pGain)
{
    float fScale;
    float calGain;
    
    //printf("OV9282_Adjust_n: pGain=%d \n",  *pGain);

    // 接近期望亮度不做处理
    if(abs(nWantLux - nCurLux) < 0x10)
        return;

    // 优先处理过亮或过暗的情况
    if(nCurLux > 0xD0)  // 过亮
    {
        *pExp = OV9282_MAXEXPOSUE_A;
        *pGain = max(*pGain/2, OV9282_MINGAIN);
    }
    else if(nCurLux < 0x20) // 过暗
    {
        *pExp = OV9282_MAXEXPOSUE_C;
        *pGain = min(*pGain*2, OV9282_MAXGAIN);
    }
    else
    {
        /* 先调整增益值，当增益已经达到上限或下限后开始调整曝光时间 */
        if((*pGain == OV9282_MAXGAIN) && ( nWantLux > nCurLux))
        {
            *pExp = min((*pExp+0x1000), OV9282_MAXEXPOSUE_C);
        }
        else if((*pGain == OV9282_MINGAIN) && (nWantLux < nCurLux))
        {
            *pExp = max((*pExp-0x1000), OV9282_MINEXPOSUE);
        }
        else
        {
            fScale = ((float)nWantLux/(float)nCurLux);
            calGain = ((*pGain) * fScale);

            if( nWantLux> nCurLux)
            {
                *pGain = min(calGain, OV9282_MAXGAIN);
            }
            else
            {
                *pGain = max(calGain, OV9282_MINGAIN);
            }
        }
        
    }
    //printf("4 %x %x\n", *pExp, *pGain);    
	nOv9282_Gain = *pGain;
	nOv9282_Exp = *pExp;
}//End of function;


void OV9282_Exposure_update(void)
{
	OV9282_set_exp(nOv9282_Exp);
 	OV9282_set_gain(nOv9282_Gain);
    //printf("e: %x, g:%x\n", nOv9282_Exp, nOv9282_Gain);
}

void OV9282_Adjust(int nWantLux, unsigned char* pImgData, unsigned int nCount, unsigned int nLightOn, unsigned int nLightOff)
{
	int curLux;
	unsigned int *pExp=NULL, *pGain=NULL;

	curLux = OV9282_GetLux(pImgData);

#if OV9282_DBG_ENABLE
    printf("wantlux%d curlux:%d  nCount=%d\n ", nWantLux, curLux, nCount);
#endif

    //return;
	if (nOV9282CaptureMode == 1)
	{
		if ((nCount % 3) == 0)	//曝光期间照明灯
		{
            OV9282_Adjust_n(curLux, nWantLux,  &nOv9282_Exp_C, &nOv9282_Gain_C);
		}
	}
	else if (nOV9282CaptureMode == 2)
	{
		if ((nCount % 3) == 0)//曝光期间无照明灯
		{
			pExp = &nOv9282_Exp_B;
			pGain = &nOv9282_Gain_B;
		}
		else if (nCount % 3  == 1)	//曝光期间无照明灯
		{
			pExp = &nOv9282_Exp_C;
			pGain = &nOv9282_Gain_C;
		}
		else if (nCount % 3 == 2)	//曝光期间照明灯
		{
			pExp = &nOv9282_Exp_A;
			pGain = &nOv9282_Gain_A;
		}
        OV9282_Adjust_n(curLux, nWantLux,  &nOv9282_Exp_B, &nOv9282_Gain_B);		
	}
	else
	{
		return;
	}

}


////nMode: 0 - Normal; 1 - horizontal mirror; 2 - vertical flip; 3 - horizontal mirror & vertical flip.
void OV9282_Mirror(int mode)
{
	unsigned int flip_data;
	unsigned int mirror_data;
	
	//unsigned char val[2];
	if (OV9282_reg_read(OV9282_VERTICAL_FLIP_REG, &flip_data) != 0)
		return;
	if (OV9282_reg_read(OV9282_HORIZONAL_MIRROR_REG, &mirror_data) != 0)
		return;
      
	if (mode == 1)	//horizontal mirror 
	{
	    flip_data |= (1<<2);
	}
	else if (mode == 2) //vertical flip
	{
	    mirror_data |= (1<<2);
	}
	else if (mode == 3)
	{
	    flip_data |= (1<<2);
	    mirror_data |= (1<<2);	    
	}
	else if(mode == 0)
	{
        flip_data   &= ~(1<<2);
        mirror_data &= ~(1<<2);
	}

    OV9282_reg_write(OV9282_HORIZONAL_MIRROR_REG, mirror_data);
    OV9282_reg_write(OV9282_VERTICAL_FLIP_REG, flip_data);
}

int OV9282_ReservedDataCnt(void)
{
	return 0;	
}

void LightCtrlmode1(int n)
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
			light_on(TRUE);
			break;
		}

		LightCount++;
// 		printf("H\r\n");
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
			foc_on(TRUE);	
			break;
		}	
        OV9282_Exposure_update();
	}
}


//mode1 屏幕识读模式
void LightCtrlmode2(int n)
{
	
	if (n == 1)
	{
		foc_on(FALSE);
		
		switch(nLightCtrlMode)
		{
		case 2:
			light_on(FALSE);
			// 			diag_printf("L\n");
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
            OV9282_Exposure_update();
		}
		
	}
	
}

//mode3 侦测识读模式
void LightCtrlmode3(int n)
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
			foc_on(TRUE);
			
		}
		
	}
	else if (n == 0)
	{
		light_on(FALSE);		
		
		if ((nLightCtrlMode == 1) && (nFocCtrlMode == 1))
		{
			foc_on(TRUE);
		}
		else
			foc_on(FALSE);
		
	}
	
}


void * OV9282_LightHandle(int mode)
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


unsigned char * OV9282_ImgData(unsigned char *p, unsigned int *pFlag)	//图像无需转换
{
	if (pFlag) *pFlag = CMOS_FLAG_DATA_PROCESSED;
	return p;	
}



