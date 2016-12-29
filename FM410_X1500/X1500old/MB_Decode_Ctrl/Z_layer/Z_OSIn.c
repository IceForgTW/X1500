/********************************************************************
	created:	2007/02/08
	created:	8:2:2007   16:32
	filename: 	OS_In.cpp
	file path:	
	file base:	OS_In
	file ext:	cpp
	author:		LieMa
	
	purpose:	OS提供的输出接口,及空接口提供
*********************************************************************/

#include "Z_OSIn.h"
#include <string.h>
/************************************************************************/
/* 类型定义                                                             */
/************************************************************************/
typedef int (*FNC_ReplaceKey)(void);
/************************************************************************/
/* 全局标志量                                                           */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
//硬件输入接口
osINTFACE	g_sHardIntf;
//硬件输入接口
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* 宏定义                                                               */
/************************************************************************/
int	g_nIdleStTick = 500;		//空闲起始时间	//5秒开机预留时间，避免开机未初始化完就进入休眠
int g_nInHignCoreFreq = 1;	//是否是在高核心频率状态下工作
unsigned int *g_pReplaceIMG = NULL; //替代图像
FNC_ReplaceKey	g_fncReplaceKEY = NULL;		//替代按键

BOOL g_bFocusIsOn = FALSE;	//Focus 是否是开着
BOOL g_bLightIsOn = FALSE;	//Light 是否是开着
BOOL g_bIRLightIsOn = FALSE; //IR Light 是否是开着
static int nPM = -1;
/************************************************************************/
/*  函数实现                                                            */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
// 虚接口
void GD_000001( int a, int b, int c )
{
	a = 0;
	b = 0;
	c = 0;
	return;
}//End of function;
void GD_000002(char * pstr)
{
//	char *ptmp = pstr;
	pstr = pstr;
	return;
}//End of function;
void GD_000003(int a)
{
	a = 0;
	return;
}//End of function;
void GD_000004(void)
{
	return;
}//End of function;
int  GD_000005(void)
{
	return 0;
}//End of function;
int  GD_000006(int a)
{
	a = 0;
	return 0;
}//End of function;
int  GD_000007(int a, int b)
{
	a = 0;
	b = 0;
	return 0;
}//End of function;
int*  GD_000008(int a)
{
	a = 0;
	return NULL;
}//End of function;
int* GD_000009(void)
{
	return NULL;
}//End of function;
int	 GD_000010(unsigned char * buff, unsigned int size)
{
	//unsigned char *p = buff;
	buff = buff;
	size = 0;
	return 0;
}//End of function;
int  GD_000011(void * lpsParam)
{
	//void *p = lpsParam;
	lpsParam = lpsParam;
	return 0;
}//End of function;

int  GD_000012(unsigned char * buff, int nSectOffs, unsigned int size)
{
//	unsigned char *p = buff;
	buff = buff;
	nSectOffs = 0;
	size = 0;

	return 0;
}//End of function;
int GD_000013(char *pBuff, int nBuffSize)
{
	nBuffSize = nBuffSize;
	pBuff = pBuff;
	return 0;
}
void InitDefaultOsIntface(void)
{
	memset((char*)&g_sHardIntf, 0, sizeof(osINTFACE));

}//End of function;
//虚接口
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 底层接口
void Z_InitHardIntface(osINTFACE * psParam)
{
	//默认初始化
	InitDefaultOsIntface();
	//用传入的参数初始化
	if (psParam->beep != NULL)
	{ g_sHardIntf.beep = psParam->beep; }
	if (psParam->playmusic != NULL)
	{ g_sHardIntf.playmusic = psParam->playmusic; }
	if (psParam->usleep != NULL)
	{ g_sHardIntf.usleep = psParam->usleep; }
	if (psParam->delay != NULL)
	{ g_sHardIntf.delay = psParam->delay; }
	if (psParam->counterget != NULL)
	{ g_sHardIntf.counterget = psParam->counterget; }
	if (psParam->counterdensity != NULL)
	{ g_sHardIntf.counterdensity = psParam->counterdensity; }
	if (psParam->oscillate != NULL)
	{ g_sHardIntf.oscillate = psParam->oscillate; }
	if (psParam->dogfeed != NULL)
	{ g_sHardIntf.dogfeed = psParam->dogfeed; }
	if (psParam->doggetbreak != NULL)
	{ g_sHardIntf.doggetbreak = psParam->doggetbreak; }
	if (psParam->dogreset != NULL)
	{ g_sHardIntf.dogreset = psParam->dogreset; }
	if (psParam->keypressed != NULL)
	{ g_sHardIntf.keypressed = psParam->keypressed; }
	if (psParam->ledon != NULL)
	{ g_sHardIntf.ledon = psParam->ledon; }
	if (psParam->ledoff != NULL)
	{ g_sHardIntf.ledoff = psParam->ledoff; }
	if (psParam->lighton != NULL)
	{ g_sHardIntf.lighton = psParam->lighton; }
	if (psParam->lightoff != NULL)
	{ g_sHardIntf.lightoff = psParam->lightoff; }
	if (psParam->irlighton != NULL)
	{ g_sHardIntf.irlighton = psParam->irlighton; }
	if (psParam->irlightoff != NULL)
	{ g_sHardIntf.irlightoff = psParam->irlightoff; }
	if (psParam->focuson != NULL)
	{ g_sHardIntf.focuson = psParam->focuson; }
	if (psParam->focusoff != NULL)
	{ g_sHardIntf.focusoff = psParam->focusoff; }
	if (psParam->cmos_gettype != NULL)
	{ g_sHardIntf.cmos_gettype = psParam->cmos_gettype; }
	if (psParam->cmos_geti2c != NULL)
	{ g_sHardIntf.cmos_geti2c = psParam->cmos_geti2c; }
	if (psParam->cmos_seti2c != NULL)
	{ g_sHardIntf.cmos_seti2c = psParam->cmos_seti2c; }
	if (psParam->cmos_read != NULL)
	{ g_sHardIntf.cmos_read = psParam->cmos_read; }
	if (psParam->cmos_getimgbuff != NULL)
	{ g_sHardIntf.cmos_getimgbuff = psParam->cmos_getimgbuff; }
	if (psParam->param_load != NULL)
	{ g_sHardIntf.param_load = psParam->param_load; }
	if (psParam->param_save != NULL)
	{ g_sHardIntf.param_save = psParam->param_save; }
	if (psParam->comm_gettype != NULL)
	{ g_sHardIntf.comm_gettype = psParam->comm_gettype; }
	if (psParam->comm_read != NULL)
	{ g_sHardIntf.comm_read = psParam->comm_read; }
	if (psParam->comm_write != NULL)
	{ g_sHardIntf.comm_write = psParam->comm_write; }
	if (psParam->comm_config != NULL)
	{ g_sHardIntf.comm_config = psParam->comm_config; }
	if (psParam->comm_getstatue != NULL)
	{ g_sHardIntf.comm_getstatue = psParam->comm_getstatue; }
	if (psParam->oem_getuid != NULL)
	{ g_sHardIntf.oem_getuid = psParam->oem_getuid; }
	if (psParam->oem_getlicense != NULL)
	{ g_sHardIntf.oem_getlicense = psParam->oem_getlicense; }
	if (psParam->oem_setlicense != NULL)
	{ g_sHardIntf.oem_setlicense = psParam->oem_setlicense; }
	if (psParam->oem_licenselock != NULL)
	{ g_sHardIntf.oem_licenselock = psParam->oem_licenselock;}
	if (psParam->syssleep != NULL)
	{ g_sHardIntf.syssleep = psParam->syssleep; }
	if (psParam->reset != NULL)
	{ g_sHardIntf.reset = psParam->reset; }
	if (psParam->GetHardwareInfo != NULL)
	{ g_sHardIntf.GetHardwareInfo = psParam->GetHardwareInfo; }
	if (psParam->DeepSleep != NULL)
	{ g_sHardIntf.DeepSleep = psParam->DeepSleep; }
	if (psParam->PowerOff != NULL)
	{ g_sHardIntf.PowerOff = psParam->PowerOff; }
	if (psParam->slvlighton != NULL)
	{ g_sHardIntf.slvlighton = psParam->slvlighton; }
	if (psParam->slvlightoff != NULL)
	{ g_sHardIntf.slvlightoff = psParam->slvlightoff; }
 	if (psParam->wakeupTimer != NULL)
 	{ g_sHardIntf.wakeupTimer = psParam->wakeupTimer; }
//	if (psParam->lowCoreFreq != NULL)
//	{ g_sHardIntf.lowCoreFreq = psParam->lowCoreFreq; }
//	if (psParam->highCoreFreq != NULL)
//	{ g_sHardIntf.highCoreFreq = psParam->highCoreFreq; }
	if (psParam->SetDeviceSerial != NULL)
 	{ g_sHardIntf.SetDeviceSerial = psParam->SetDeviceSerial; }

	if (psParam->setCMOSCollect != NULL)
	{ g_sHardIntf.setCMOSCollect = psParam->setCMOSCollect; }
	if (psParam->getCMOSCollect != NULL)
	{ g_sHardIntf.getCMOSCollect = psParam->getCMOSCollect; }

	if (psParam->getCMOSDMANum != NULL)
	{ g_sHardIntf.getCMOSDMANum = psParam->getCMOSDMANum; }
	if (psParam->setCMOSDMAInuseNum != NULL)
	{ g_sHardIntf.setCMOSDMAInuseNum = psParam->setCMOSDMAInuseNum; }
	if (psParam->cmos_getimgbuff_Ex != NULL)
	{ g_sHardIntf.cmos_getimgbuff_Ex = psParam->cmos_getimgbuff_Ex; }
	if (psParam->sysIdle != NULL)
	{ g_sHardIntf.sysIdle = psParam->sysIdle; }
	if (psParam->sysStandby != NULL)
	{ g_sHardIntf.sysStandby = psParam->sysStandby; }
	if (psParam->ISRAMAddr != NULL)
	{ g_sHardIntf.ISRAMAddr = psParam->ISRAMAddr; }
	if (psParam->playmusic_ex != NULL)
	{ g_sHardIntf.playmusic_ex = psParam->playmusic_ex; }
	if (psParam->extLightCtrl != NULL)
	{ g_sHardIntf.extLightCtrl = psParam->extLightCtrl; }
	if (psParam->SetDeviceManufacturer != NULL)
	{ g_sHardIntf.SetDeviceManufacturer = psParam->SetDeviceManufacturer; }

	if (psParam->SetCmosLightMode != NULL)
	{ g_sHardIntf.SetCmosLightMode = psParam->SetCmosLightMode; }
	
	if (psParam->GetCmosLightMode != NULL)
	{ g_sHardIntf.GetCmosLightMode = psParam->GetCmosLightMode; }
	
	if (psParam->GetHardwareInfo_NCR != NULL)
	{ g_sHardIntf.GetHardwareInfo_NCR = psParam->GetHardwareInfo_NCR; }

	if (psParam->SetPowerFrequency != NULL)
	{ g_sHardIntf.SetPowerFrequency = psParam->SetPowerFrequency; }

	if (psParam->SetLightOnDuration != NULL)
	{ g_sHardIntf.SetLightOnDuration = psParam->SetLightOnDuration; }

	if (psParam->SetLightMode != NULL)
	{ g_sHardIntf.SetLightMode = psParam->SetLightMode; }

	if (psParam->MScounterget != NULL)
	{ g_sHardIntf.MScounterget = psParam->MScounterget; }

	if (psParam->UsbResumeHandle != NULL)
	{ g_sHardIntf.UsbResumeHandle = psParam->UsbResumeHandle; }

	if (psParam->firmwareUpgrade != NULL)
	{ g_sHardIntf.firmwareUpgrade = psParam->firmwareUpgrade; }

	if (psParam->getKernelInfo != NULL)
	{ g_sHardIntf.getKernelInfo = psParam->getKernelInfo; }
	
	if (psParam->setPowerLedOn != NULL)
	{ g_sHardIntf.setPowerLedOn = psParam->setPowerLedOn; }

	if (psParam->CmosCapture != NULL)
	{ g_sHardIntf.CmosCapture = psParam->CmosCapture; }

	if (psParam->GetCmosBuf != NULL)
	{ g_sHardIntf.GetCmosBuf = psParam->GetCmosBuf; }
	
	if (psParam->GetCmosHeight != NULL)
	{ g_sHardIntf.GetCmosHeight = psParam->GetCmosHeight; }

	if (psParam->GetCmosWidth != NULL)
	{ g_sHardIntf.GetCmosWidth = psParam->GetCmosWidth; }

	if (psParam->SetCmosMirror != NULL)
	{ g_sHardIntf.SetCmosMirror = psParam->SetCmosMirror; }


	if (psParam->GetCRC32 != NULL)
	{ g_sHardIntf.GetCRC32 = psParam->GetCRC32; }

	if (psParam->GetCmosBufTest != NULL)
	{ g_sHardIntf.GetCmosBufTest = psParam->GetCmosBufTest; }
	
      if (psParam->TimeoutHandle!= NULL)
	{ g_sHardIntf.TimeoutHandle = psParam->TimeoutHandle; }

	if (psParam->GetDevMagic != NULL)
	{ g_sHardIntf.GetDevMagic = psParam->GetDevMagic; }

      if (psParam->TransferIdleTickVar!= NULL)
	{ g_sHardIntf.TransferIdleTickVar = psParam->TransferIdleTickVar; }	

       if (psParam->GetCPUID!= NULL)
	{ g_sHardIntf.GetCPUID= psParam->GetCPUID; }	

       if (psParam->EncryptionWrtie!= NULL)
	{ g_sHardIntf.EncryptionWrtie= psParam->EncryptionWrtie; }	

       if (psParam->EncryptionRead!= NULL)
	{ g_sHardIntf.EncryptionRead= psParam->EncryptionRead; }	

       if (psParam->SetKBWCharacterEncoding != NULL)
	{ g_sHardIntf.SetKBWCharacterEncoding = psParam->SetKBWCharacterEncoding; }

	if (psParam->isSetKBWrestore != NULL)
	{
		g_sHardIntf.isSetKBWrestore = psParam->isSetKBWrestore;
	}
      if (psParam->isDecoding!= NULL)
	{
		g_sHardIntf.isDecoding= psParam->isDecoding;
	}

}//End of function;

void Z_KBWSetCharEncoding(int nMode)
{
	if (g_sHardIntf.SetKBWCharacterEncoding != NULL)
	{ g_sHardIntf.SetKBWCharacterEncoding(nMode); }
}

unsigned int Z_GetCRC32(unsigned char *pdata, int len)
{
	if (g_sHardIntf.GetCRC32 != NULL)
	{ return g_sHardIntf.GetCRC32(pdata, len); }
	return 0;
}

int Z_GetCmosWidth(void)
{
	if (g_sHardIntf.GetCmosWidth != NULL)
	{ return g_sHardIntf.GetCmosWidth(); }
	return 0;
}

int Z_GetCmosHeight(void)
{
	if (g_sHardIntf.GetCmosHeight != NULL)
	{ return g_sHardIntf.GetCmosHeight(); }
	return 0;
}

//nMode: 0 - Normal; 1 - horizontal mirror; 2 - vertical flip; 3 - horizontal mirror & vertical flip.
void Z_SetCmosMirror(int nMode)
{
	if (g_sHardIntf.SetCmosMirror != NULL)
	{ g_sHardIntf.SetCmosMirror(nMode); }
}



BYTE* Z_GetCmosBuf(unsigned int mode, unsigned int timeout)
{
	if (g_sHardIntf.GetCmosBuf != NULL)
	{ return g_sHardIntf.GetCmosBuf(mode, timeout); }
	return NULL;
}

BYTE* Z_GetCmosBufTest(int n)
{
	if (g_sHardIntf.GetCmosBufTest != NULL)
	{ return g_sHardIntf.GetCmosBufTest(n); }
	return NULL;
}

void Z_CmosCapture(BOOL b, int mode)
{
	if (g_sHardIntf.CmosCapture != NULL)
	{ g_sHardIntf.CmosCapture(b, mode); }

}

void Z_SetPowerLedOn(BOOL b)
{
	if (g_sHardIntf.setPowerLedOn != NULL)
	{ g_sHardIntf.setPowerLedOn(b); }
}

//固件更新接口
void Z_FirmwareUpgrade(int port)
{
	if (g_sHardIntf.firmwareUpgrade != NULL)
	{ g_sHardIntf.firmwareUpgrade(port); }
}

void Z_Beep( int freq, int mdelay, int volume )
{
	if (g_sHardIntf.beep != NULL)
	{ g_sHardIntf.beep(freq,mdelay,volume); }
}

#define MUSIC_DELAY_PARAM 50
void GD_Play_1_1(int n, int t, int volume)
{
	unsigned short int frq;
	const int frqs[7] = {1760/*Hz*/, 2010/*Hz*/,
		2260/*Hz*/, 2410/*Hz*/,2710/*Hz*/, 
		3010/*Hz*/, 3360/*Hz*/};
	
	if(t <= 0) return;
	if(n == 36){
		Z_usDelay(MUSIC_DELAY_PARAM * t);
		return;
	}		
	if( n < 0 || n > 35) return;
	
	frq = (frqs[n%7] >> 2) << (n / 7);
	
	Z_Beep(frq, MUSIC_DELAY_PARAM * t,volume);
	Z_msDelay(MUSIC_DELAY_PARAM * t);
	
}

void GD_Play_1(const char * music, int volume)
{
	int s;
	char c=0;
	int t;
	
	if(music == NULL)
		return ;
	do{
		s = 14;
		while( *music != '.' && *music != 0){
			c = *music;
			if(c == '+') s += 7;
			if(c == '-') s -= 7;
			music++;
		}
		if(*music == 0)break;
		music++;
		t = 0;
		while( *music != '|' && *music != 0){
			if(*music > '0' && *music <= '8'){
				t += (8 / (*music - '0'));
			}
			music++;
		}
		if(*music == 0)break;
		
		if(c == '0')
			GD_Play_1_1(36,t,volume);
		else
			GD_Play_1_1(c - '1' + s,t,volume);
	}while(*music != 0);
	return;
}

void Z_PlayMusic(char * pcMusic)
{
	if (g_sHardIntf.playmusic != NULL)
	{ g_sHardIntf.playmusic(pcMusic); }
	else
	{
//		Z_Beep(3000,100,1);
		GD_Play_1(pcMusic,9);
	}
}

void Z_PlayMusic_Ex(char * pcMusic, int volume)
{
	if (g_sHardIntf.playmusic_ex != NULL)
	{ g_sHardIntf.playmusic_ex(pcMusic, volume); }
	else
	{
		GD_Play_1(pcMusic,volume);
	}

}

//2.时间
void Z_usDelay(int us)
{
	if (g_sHardIntf.usleep != NULL)
	{ g_sHardIntf.usleep(us); }
}
void Z_msDelay(int ms)
{
	if (g_sHardIntf.delay != NULL)
	{ g_sHardIntf.delay(ms); }
}
int  Z_CounterGet(void)
{
	if(g_sHardIntf.counterget != NULL)
	{ return g_sHardIntf.counterget();}
	return 0;
}
int  Z_GetCounterDensity(void)
{
	if(g_sHardIntf.counterdensity != NULL)
	{ return g_sHardIntf.counterdensity(); }
	return 0;
}
int  Z_Get10msCounter(void)
{
	return (Z_GetCounterDensity()*Z_CounterGet() / 10);
}
unsigned int  Z_Get1msCounter(void)
{
	if(g_sHardIntf.MScounterget != NULL)
	{ return g_sHardIntf.MScounterget();}
	else 
		return (Z_CounterGet()*Z_GetCounterDensity());
	return 0;
}
//3.振子
void  Z_Oscillate(int ms)
{
	if (g_sHardIntf.oscillate != NULL)
	{ g_sHardIntf.oscillate(ms); }
}
//4.看门狗
void Z_DogFeed(void)
{
	if (g_sHardIntf.dogfeed != NULL)
	{ g_sHardIntf.dogfeed(); }
}

int  Z_DogGetBreak(void) //获取看门狗复位时间
{
	if(g_sHardIntf.doggetbreak != NULL)
	{ return g_sHardIntf.doggetbreak(); }
	return 0;
}

int  Z_DogReset(int ms)	 //重设看门狗复位时间,并重新开始计时
{
	if (g_sHardIntf.dogreset != NULL)
	{ return g_sHardIntf.dogreset(ms); }
	return 0;
}

//5.按键
int  Z_KeyPressed(void)   //获取按键状态,0表示弹起状态,非0表示按下状态
{
	int key;
	if (g_fncReplaceKEY != NULL)
	{
		return (g_fncReplaceKEY());
	}//End of if;

	if (g_sHardIntf.keypressed != NULL)
	{
		key = g_sHardIntf.keypressed(); 
		if (key==0)
		{
//			Z_Beep(3000,200,1);
// 			Z_msDelay(10);
			Z_msDelay(3);
			key = g_sHardIntf.keypressed(); 
		}//End of if;
		if (key==0)
		{ 
		    g_nIdleStTick = Z_Get10msCounter(); 
            return 1;
        }
		else
       {
			return 0;
	   }
		
	}

	return 0;
}

//6.LED控制
void Z_LedOn(int clr)  //可以指定LED的颜色
{
	if (g_sHardIntf.ledon != NULL)
	{ g_sHardIntf.ledon(clr); }
}
void Z_LedOff(int clr)
{
	if (g_sHardIntf.ledoff != NULL)
	{ g_sHardIntf.ledoff(clr); }
}

//7.普通照明灯
void Z_LightOn(void)
{
	if (g_sHardIntf.lighton != NULL)
	{ g_sHardIntf.lighton(); }
	g_bLightIsOn = TRUE;
}
void Z_LightOff(void)
{
	if (g_sHardIntf.lightoff != NULL)
	{ g_sHardIntf.lightoff(); }
	g_bLightIsOn = FALSE;
}

void Z_ExtLightCtrl(BOOL bOn)
{
	if (g_sHardIntf.extLightCtrl != NULL)
	{ g_sHardIntf.extLightCtrl(bOn); }

}

void Z_SetCmosLightMode(int mode)
{
	if (g_sHardIntf.SetCmosLightMode != NULL)
	{ g_sHardIntf.SetCmosLightMode(mode); }
}

int Z_GetCmosLightMode(void)
{
	if (g_sHardIntf.GetCmosLightMode != NULL)
	{ return g_sHardIntf.GetCmosLightMode(); }
	return 0;
}

void Z_SetLightOnDuration(int nLine)
{
	if (g_sHardIntf.SetLightOnDuration != NULL)
	{ g_sHardIntf.SetLightOnDuration(nLine); }
}

//设置曝光模式：
// 0 - 普通模式（纸质码）；1 - 反光抑制；2 - mobile模式曝光的时候有的帧照明灯亮，有的帧照明灯灭；
//	3 - 纸质码（motion好，但很难读屏幕显示码）；4 - mobile模式 每帧曝光照明灯都亮。
void Z_SetLightMode(int mode)
{
	if (g_sHardIntf.SetLightMode != NULL)
	{
		g_sHardIntf.SetLightMode(mode);
// 		SetExprosureMode();
	}
}

//8.红外照明灯
void Z_IRLightOn(void)
{
	if (g_sHardIntf.irlighton != NULL)
	{ g_sHardIntf.irlighton(); }
	g_bIRLightIsOn = TRUE;
}
void Z_IRLightOff(void)
{
	if (g_sHardIntf.irlightoff != NULL)
	{ g_sHardIntf.irlightoff(); }
	g_bIRLightIsOn = FALSE;
}
//9.对焦灯
void Z_FocusOn(void)
{
	if (g_sHardIntf.focuson != NULL)
	{ g_sHardIntf.focuson(); }
	g_bFocusIsOn = TRUE;
}
void Z_FocusOff(void)
{
	if (g_sHardIntf.focusoff != NULL)
	{ g_sHardIntf.focusoff(); }
	g_bFocusIsOn = FALSE;
}
//10.CMOS操作
int  Z_cmos_GetType(void)	//获取CMOS类型
{
	if (g_sHardIntf.cmos_gettype != NULL)
	{ return g_sHardIntf.cmos_gettype(); }
	return 0;
}
int  Z_cmos_GetI2C(int reg)
{
	if (g_sHardIntf.cmos_geti2c != NULL)
	{ return g_sHardIntf.cmos_geti2c(reg); }
	return 0;
}
int  Z_cmos_SetI2C(int reg, int value)
{
	int n;
	if (g_sHardIntf.cmos_seti2c != NULL)
	{
		n = g_sHardIntf.cmos_seti2c(reg,value);
//if (Z_cmos_GetI2C(reg) != value)
//{
//	Z_Beep(3000,100,1);
//	Z_Delay(100);
//}
		return n;
	}
	return 0;
}
int*  Z_cmos_Read(int mode)   //拍摄并抓取一幅图像,存储地址为4字节对齐
{
	if (g_sHardIntf.cmos_read != NULL)
	{ return g_sHardIntf.cmos_read(mode); }
	return NULL;
}
int*  Z_cmos_GetIMGbuff(void) //获取CMOS图像存储地址
{
	if (g_pReplaceIMG != NULL)
	{ return (int*)g_pReplaceIMG; }
	if (g_sHardIntf.cmos_getimgbuff != NULL)
	{ return g_sHardIntf.cmos_getimgbuff(); }
	return NULL;
}
//11.参数导入与存储
 //功能参数存取 ,4096字节
int  Z_param_Load(unsigned char * buff, int nSectOffs, unsigned int size)
{
	if (g_sHardIntf.param_load != NULL)
	{ return g_sHardIntf.param_load(buff,nSectOffs,size); }
	return 0;
}
int  Z_param_Save(unsigned char * buff, int nSectOffs, unsigned int size)
{
	if (g_sHardIntf.param_save != NULL)
	{ return g_sHardIntf.param_save(buff,nSectOffs,size); }
	return 0;
}
//12.通讯口操作
int  Z_comm_GetType(void)	//获取通讯口类型
{
	if (g_sHardIntf.comm_gettype != NULL)
	{ return g_sHardIntf.comm_gettype(); }
	return 0;
}
int  Z_comm_Read(unsigned char * buff, unsigned int size)
{
	int nLen;
	if (g_sHardIntf.comm_read != NULL)
	{
		nLen = g_sHardIntf.comm_read(buff,size);
		if (nLen > 0)
		{ g_nIdleStTick = Z_Get10msCounter(); }
		return nLen;
	}
	return 0;
}
int  Z_comm_Write(unsigned char * buff, unsigned int size)
{
	g_nIdleStTick = Z_Get10msCounter();

	if (g_sHardIntf.comm_write != NULL)
	{ return g_sHardIntf.comm_write(buff,size); }
	return 0;
}
int  Z_comm_Config(void * lpsParam) //改变配置,结构体待定
{
	if (g_sHardIntf.comm_config != NULL)
	{ return g_sHardIntf.comm_config(lpsParam); }
	return 0;
}
int  Z_comm_GetStatue(void * lpsParam) //获取配置参数
{
	if (g_sHardIntf.comm_getstatue != NULL)
	{ return g_sHardIntf.comm_getstatue(lpsParam); }
	return 0;
}
//13.获取机器码之类
 //获取UID
int  Z_oem_GetUid(unsigned char * buff, unsigned int maxsize)
{
	if (g_sHardIntf.oem_getuid != NULL)
	{ return g_sHardIntf.oem_getuid(buff, maxsize); }
	return 0;
}
 //获取许可证
int  Z_oem_GetLicense(unsigned char * buff, unsigned int maxsize)
{
	if (g_sHardIntf.oem_getlicense != NULL)
	{ return g_sHardIntf.oem_getlicense(buff,maxsize); }
	return 0;
}
 //设置许可证
int  Z_oem_SetLicense(unsigned char * buff, unsigned int size)
{
	if (g_sHardIntf.oem_setlicense != NULL)
	{ g_sHardIntf.oem_setlicense(buff,size); }
	return 0;
}
 //锁定许可证
int  Z_oem_LicenseLock(void)
{
	if (g_sHardIntf.oem_licenselock != NULL)
	{ return g_sHardIntf.oem_licenselock(); }
	return 0;
}

int Z_sysIdle(void)
{
	if (g_sHardIntf.sysIdle != NULL)
	{ return g_sHardIntf.sysIdle(); }
	return 0;
}

int Z_sysStandby(void)
{
	if (g_sHardIntf.sysStandby != NULL)
	{ return g_sHardIntf.sysStandby(); }
	return 0;
}


int Z_sysSleep(void)
{
	if (g_sHardIntf.syssleep != NULL)
	{ return g_sHardIntf.syssleep(); }
	return 0;
}

void Z_sysReset(void)
{
	if (g_sHardIntf.reset != NULL)
	{ g_sHardIntf.reset(); }
}

void Z_usbResumeHandle(void *p)
{
	if (g_sHardIntf.UsbResumeHandle != NULL)
	{ g_sHardIntf.UsbResumeHandle(p); }
}

//14.获取空闲时间节拍数
int	 Z_GetIdleCounter(void)
{
	int nCurTick;

	nCurTick = Z_Get10msCounter();
	
	return (nCurTick-g_nIdleStTick);
}

void Z_OscillateWithBeep( int freq, int mdelay, int volume )
{
	if (g_sHardIntf.OscillateWithBeep != NULL)
	{ g_sHardIntf.OscillateWithBeep(freq,mdelay,volume); }
}

int	Z_GetHardwareInfo(char *pBuff, int nBuffSize)
{
	if (g_sHardIntf.GetHardwareInfo != NULL)
	{
		return g_sHardIntf.GetHardwareInfo(pBuff,nBuffSize);
	}//End of if;
	return 0;
}

int	Z_GetHardwareInfo_NCR(char *pBuff, int nBuffSize)
{
	if (g_sHardIntf.GetHardwareInfo_NCR != NULL)
	{
		return g_sHardIntf.GetHardwareInfo_NCR(pBuff,nBuffSize);
	}//End of if;
	return 0;
}

int	Z_GetKernelInfo(int type, char *pBuff, int nBuffSize)
{
	if (g_sHardIntf.getKernelInfo != NULL)
	{
		return g_sHardIntf.getKernelInfo(type, pBuff,nBuffSize);
	}//End of if;
	return 0;
}

void Z_DeepSleep(void)
{
	if (g_sHardIntf.DeepSleep != NULL)
	{
		g_sHardIntf.DeepSleep();
	}//End of if;
}
//系统关机
void Z_PowerOff(void)
{
	if (g_sHardIntf.PowerOff != NULL)
	{ g_sHardIntf.PowerOff(); }
}

void Z_SlvLightOn(void)
{
	if (g_sHardIntf.slvlighton != NULL)
	{ g_sHardIntf.slvlighton(); }
}

void Z_SlvLightOff(void)
{
	if (g_sHardIntf.slvlightoff != NULL)
	{ g_sHardIntf.slvlightoff(); }
}

void Z_WakeupTimer(unsigned int ms)
{
 	if (g_sHardIntf.wakeupTimer != NULL)
 	{ g_sHardIntf.wakeupTimer((int)ms); }

}//End of function;

BOOL Z_SetDeviceSerial(char *serialNum, int len)
{
 	if (g_sHardIntf.SetDeviceSerial != NULL)
 	{ return(g_sHardIntf.SetDeviceSerial(serialNum, len)); }

	return TRUE;
}//End of function;

BOOL Z_SetDeviceManufacturer(char *str, int len)
{
 	if (g_sHardIntf.SetDeviceManufacturer != NULL)
 	{ return(g_sHardIntf.SetDeviceManufacturer(str, len)); }

	return TRUE;
}//End of function;

void Z_SetCMOSCollect(int mode)
{
	if (g_sHardIntf.setCMOSCollect != NULL)
	{
		g_sHardIntf.setCMOSCollect(mode);
	}

}//End of function;

int Z_GetCMOSCollect(void)
{
	if (g_sHardIntf.getCMOSCollect != NULL)
	{
		return g_sHardIntf.getCMOSCollect();
	}

	return 0;
}//End of function;

//获取已完成采集的DMA表的数字
int Z_GetCMOSDMANum(void)
{
	if (g_sHardIntf.getCMOSDMANum != NULL)
	{
		return g_sHardIntf.getCMOSDMANum();
	}
	return -1;	
}//End of function;

//设置要锁定的DMA表的数字
void Z_SetCMOSDMAInuseNum(int num)
{
	if (g_sHardIntf.setCMOSDMAInuseNum != NULL)
	{
		g_sHardIntf.setCMOSDMAInuseNum(num);
	}
}//End of function;


//30FPS模式下获取被锁定的用于解码的图像地址
int* Z_cmos_GetIMGbuff_Ex(void)
{
	if (g_sHardIntf.cmos_getimgbuff_Ex != NULL)
	{ return g_sHardIntf.cmos_getimgbuff_Ex(); }
	return NULL;
}//End of function;


//获取CPU内部SRAM地址，存储空间大小128kBytes
char* Z_Get_ISRAM_Addr(void)
{
	if (g_sHardIntf.ISRAMAddr != NULL)
	{ return (char*)g_sHardIntf.ISRAMAddr(); }
	return NULL;
}


BOOL Z_IsFocusOn(void)
{
	return (g_bFocusIsOn == TRUE);
}

BOOL Z_IsLightOn(void)
{
	return (g_bLightIsOn == TRUE);
}

BOOL Z_IsIRLightOn(void)
{
	return (g_bIRLightIsOn == TRUE);
}
//设置成低核心频率运行
//void Z_LowCoreFreq(void)
//{
//	if(g_nInHignCoreFreq)
//	{
//		if (g_sHardIntf.lowCoreFreq != NULL)
//		{ g_sHardIntf.lowCoreFreq(); }
//		g_nInHignCoreFreq = 0;
//	}//End of if;
//}
//设置成高核心频率运行
//void Z_HighCoreFreq(void)
//{
//	if (g_nInHignCoreFreq)
//	{ return; }
//	if (g_sHardIntf.highCoreFreq != NULL)
//	{ g_sHardIntf.highCoreFreq(); }
//	g_nInHignCoreFreq = 1;
//}

// #define FREQ_416		0
// #define FREQ_312		1
// #define FREQ_208		2
// #define FREQ_104		3
// 
//设置频率，是否使能变更电压
void Z_SetPM(int nFreqMode, BOOL bVoltage)
{
	if (g_sHardIntf.SetPowerFrequency != NULL)
	{
		nPM = nFreqMode;
		g_sHardIntf.SetPowerFrequency(nFreqMode, bVoltage);
	}//End of if;
}

#define PM_VOLTAGE_CHANGE	0

void Z_SetPM_EX(int nFreqMode)
{
	if (g_sHardIntf.SetPowerFrequency != NULL)
	{
		nPM = nFreqMode;
		g_sHardIntf.SetPowerFrequency(nFreqMode, PM_VOLTAGE_CHANGE);
	}//End of if;
}

int Z_GetPM(void)
{
	return nPM;
}


int  Z_GetDevMagic(void)	//获取CMOS类型
{
	if (g_sHardIntf.GetDevMagic != NULL)
	{ 
        return g_sHardIntf.GetDevMagic(); 
    }
	return 0;
}

void Z_TransferIdleTickVar(int *pTick)
{
    if (g_sHardIntf.TransferIdleTickVar != NULL)
	{
		g_sHardIntf.TransferIdleTickVar(pTick);
	}
}

void Z_GetCPUID(unsigned char *pcpuID)
{
    if (g_sHardIntf.GetCPUID!= NULL)
	{
		g_sHardIntf.GetCPUID(pcpuID);
	}
}

unsigned int Z_EncryptionWrite(unsigned char *buff, unsigned int nsize, unsigned int flag)
{
    if (g_sHardIntf.EncryptionWrtie!= NULL)
	{
		return g_sHardIntf.EncryptionWrtie(buff, nsize, flag);
	}
}

unsigned int Z_EncryptionRead(unsigned char *buff, unsigned int nsize, unsigned int flag)
{
    if (g_sHardIntf.EncryptionRead!= NULL)
	{
		return g_sHardIntf.EncryptionRead(buff, nsize, flag);
	}
}

void Z_isSetKBWrestore(int nFormat)//码词是否是经过dataformat处理的
{
	if (g_sHardIntf.isSetKBWrestore != NULL)
	{
		g_sHardIntf.isSetKBWrestore(nFormat);
	}

}

void Z_isDecoding(int nFlag)
{
    if (g_sHardIntf.isDecoding != NULL)
	{
		g_sHardIntf.isDecoding(nFlag);
	}
}
// 底层接口
//////////////////////////////////////////////////////////////////////////

void Z_SetUserFunc(UTGeneralParam *psParam)
{
	if (psParam != NULL && psParam->Id == STID_REPLACEIMG)
	{
		g_pReplaceIMG = (unsigned int*)psParam->psParamStruct;
	}//End of if;
	if (psParam != NULL && psParam->Id == STID_REPLACEKEY)
	{
		g_fncReplaceKEY = (FNC_ReplaceKey)(psParam->psParamStruct);
	}//End of if;
}//End of function;
