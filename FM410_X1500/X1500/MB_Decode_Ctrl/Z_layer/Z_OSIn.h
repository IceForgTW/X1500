/********************************************************************
	created:	2007/02/08
	created:	8:2:2007   16:35
	filename: 	Interface.h
	file path:	
	file base:	Interface
	file ext:	h
	author:		LieMa
	
	purpose:	对外接口
*********************************************************************/

#ifndef __UIMG_DEVELOP_INTERFACE_OSIN_H__
#define __UIMG_DEVELOP_INTERFACE_OSIN_H__

#include "../list.h"
#include "Z_define.h"

/************************************************************************/
/* 类型定义                                                             */
/************************************************************************/

// typedef struct zspHIDKBW
// {
// 	int  nLanguage;		//语言
// 	int isAutoCaps;	//自动切换大小写
// 	int isNumericMode;	//数字模式
// 	int isControlASCIIMode;	//Control + ASCII 模式
// 	int  Reserved[2];	//保留
// }UTHIDKBW;

typedef struct zspHIDKBW
{
	int  nLanguage;		//语言
	int isCapsLock;	//自动切换大小写
	int isNumericMode;	//数字模式
	int isControlASCIIMode;	//Control + ASCII 模式

	int isKeysrockeDelay;	//发送每个字符之间的延时设置。取值：0，1，2 默认：0。
	int isConvertCase;		//强制转换所有字母都成为大写或者小写。取值：0，1，2 默认：0。
	int isEmulateKeypad;	//采用ALT+ Keypad输入ASCII值 来产生字符。取值：0，1 默认：0。 
	int isUnknownCharactersBeep;	//遇到未知字符是否声音提示。取值：0，1 默认：0。
	int isKeyPad;	//小键盘打开标志，0：关闭，1：打开。默认：0，关闭
	int isFasttransferMode;
	int nPollRate;	//轮询速率
	int nCodePage;
}UTHIDKBW;


//通讯口参数配置结构
typedef struct zspCOMMCFG{
	unsigned int nCommType;		//类型
	unsigned int nToModify;		//使用COMM_MDF_XXXXX 的定义值,表示不同的动作
	void * lpsDetail;	//具体参数的结构体
}SCOMMCFG;

typedef struct _NCR_TRICKLE{
	int nCount;			//有几个区域的数据
	int nTargetAddr[10];	//每个区域的起始地址
	int nSrcAddr[10];	//每个区域数据源地址
	int nSrcLength[10];	//每个区域数据长度
	int crc32;
	int nOffset;		//host端发送固件数据的起始偏移量
	int length;			//固件数据的长度
	BYTE *pFWbuff;
	int flashState;		//flash的状态：0 = 未写入， 1 = 正在写入， 2 = 写入完成
}NCRTrickleST;

/************************************************************************/
/* 全局标志量                                                           */
/************************************************************************/

/************************************************************************/
/* 宏定义                                                               */
/************************************************************************/
//有关LED颜色的定义
#define LED_RED		0xff0000	//红色LED
#define LED_GREEN	0x00ff00	//绿色LED
#define LED_BLUE	0x0000ff	//蓝色LED
//CMOS类型值
#define T_CMOS_UNKNOW		0x00		//Unknow cmos type
#define T_CMOS_OV7141		0x01
#define T_CMOS_OV9650		0x02		
#define T_CMOS_MT9V022		0x03
#define T_CMOS_OV7221		0x05		// 640 * 480
#define T_CMOS_OV9210		0x06
#define T_CMOS_AR0134		0x07
#define T_CMOS_BF3005		0x08
#define T_CMOS_GC0308		0x09
#define T_CMOS_OV9282       0x0A


//有关通讯接口的类型的定义
#define COMMT_RS232			1		//RS232的串口
#define COMMT_USB_Data		2		//USB的数据通道接口
#define COMMT_KBW			3		//HID-KBW模拟键盘输出
#define COMMT_USB_RS232		4		//USB模拟RS232接口
#define COMMT_BLUE_TOOTH	5		//BlueTooth接口
#define COMMT_PS2	        6		//PS2接口
#define COMMT_HID_POS		7		//HID-POS
#define COMMT_IBM_SurePOS_TT	8		//IBM SurePOS(Table-Top) 
#define COMMT_IBM_SurePOS_HH	9		//IBM SurePOS(Hand-Held)
#define COMMT_USB_CDC		10		//USB CDC接口


//有关通讯接口配置的调整动作
#define COMM_MDF_ENABLE		0x01	//使能
#define COMM_MDF_DISABLE	0x02	//关闭
#define COMM_MDF_CHGPARAM	0x04	//改变参数

#define	CMOSGetUsedPic	0
#define CMOSGetNewPic	1
/************************************************************************/
/*  函数实现                                                            */
/************************************************************************/

#ifdef __cplusplus
extern"C"{
#endif

//////////////////////////////////////////////////////////////////////////
//输入/初始化uIMG的硬件功能接口
void Z_InitHardIntface(osINTFACE * psParam);
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//系统硬件功能接口
//系统重新启动
void Z_sysReset(void);

//系统闲置挂起，直到退出。 返回非0表示闲置成功并已退出闲置状态
//系统闲置
int Z_sysIdle(void);

int Z_sysStandby(void);

//0.系统休眠挂起，直到退出。 返回非0表示休眠成功并已退出休眠状态
//系统休眠
int Z_sysSleep(void);
//蜂鸣器发声
void Z_Beep( int freq, int mdelay, int volume );
//蜂鸣器演奏
void Z_PlayMusic(char * pcMusic);
void Z_PlayMusic_Ex(char * pcMusic, int volume);
//微秒级延迟
void Z_usDelay(int us);
//毫秒级延迟
void Z_msDelay(int ms);
//获取系统节拍数
int  Z_CounterGet(void); 
//获取系统节拍精度
int  Z_GetCounterDensity(void); 
//获取10ms单位的系统节拍数
int  Z_Get10msCounter(void);	

unsigned int  Z_Get1msCounter(void);

//振子振动
void  Z_Oscillate(int ms);
//喂看门狗
void Z_DogFeed(void);
//获取看门狗的复位时间
int  Z_DogGetBreak(void); 
//设置看门狗的复位时间
int  Z_DogReset(int ms); 
//获取当前按下的键值，0表示没有按键
int  Z_KeyPressed(void);
//开启指定颜色的LED
void Z_LedOn(int clr);
//关闭指定颜色的LED
void Z_LedOff(int clr);
//照明灯开
void Z_LightOn(void);
//照明灯关
void Z_LightOff(void);
//红外照明灯开
void Z_IRLightOn(void);
//红外照明灯关
void Z_IRLightOff(void);
//对焦灯开
void Z_FocusOn(void);
//对焦灯关
void Z_FocusOff(void);
//是否对焦灯处于开启状态
BOOL Z_IsFocusOn(void);
//是否照明灯处于开启状态
BOOL Z_IsLightOn(void);
//是否红外照明灯处于开启状态
BOOL Z_IsIRLightOn(void);

void Z_SetLightOnDuration(int nLine);

void Z_ExtLightCtrl(BOOL bOn);

/*
	void Z_SetCmosLightMode(int mode);
	nMode:	bit[7:0]	illumination mode 内部照明灯模式
						0 = normal
						1 = always on
						2 = always off
			bit[15:8]	aim mode 对焦灯模式
						0 = normal
						1 = always on
						2 = always off
						3 = 自动读码模式下，对焦灯设置为sensor mode的时候，如果照明灯不为always on模式，对焦灯可以作为侦测条码的辅助照明灯
			bit[23:16]	external illumination mode 外部照明灯模式
						0 = 联合模式(与内部照明灯同时亮、灭)
						1 = always on
						2 = always off
						3 = 独立模式(仅根据内部照明灯的设置码参数设置的模式控制亮灭)
 */
void Z_SetCmosLightMode(int mode);
int Z_GetCmosLightMode(void);
void Z_SetLightMode(int mode);


//10.CMOS操作
//获取CMOS类型
int  Z_cmos_GetType(void); 
//获取CMOS寄存器的值
int  Z_cmos_GetI2C(int reg);	
//设置CMOS寄存器为指定值
int  Z_cmos_SetI2C(int reg, int value);
//采集一幅图像,地址为4字节对齐
int*  Z_cmos_Read(int mode); 
//获取图像地址
int*  Z_cmos_GetIMGbuff(void);

//11.参数导入与存储
 //功能参数存取 ,每1个SectOffs间隔4096字节. 总共只有12个Sector，允许偏移量 0 ~ 11
//读取指定块信息
int  Z_param_Load(unsigned char * buff, int nSectOffs, unsigned int size);
//存储信息到指定块
int  Z_param_Save(unsigned char * buff, int nSectOffs, unsigned int size);

//12.通讯口操作
//获取当前通讯接口类型
int  Z_comm_GetType(void); 
//从通讯口中读数据
int  Z_comm_Read(unsigned char * buff, unsigned int size);
//往通讯口中写数据
int  Z_comm_Write(unsigned char * buff, unsigned int size);
//改变通讯接口的配置，包括类型或具体参数
int  Z_comm_Config(void * lpsParam); 
//获取通讯接口的配置参数
int  Z_comm_GetStatue(void * lpsParam);

//13.获取机器码之类
 //获取UID
int  Z_oem_GetUid(unsigned char * buff, unsigned int maxsize);
 //获取许可证
int  Z_oem_GetLicense(unsigned char * buff, unsigned int maxsize);
 //设置许可证
int  Z_oem_SetLicense(unsigned char * buff, unsigned int size);
 //锁定许可证
int  Z_oem_LicenseLock(void);

//14.获取空闲时间节拍数
int	 Z_GetIdleCounter(void);
//发声,同时振动
void Z_OscillateWithBeep( int freq, int mdelay, int volume );
//获取硬件版本信息
int	Z_GetHardwareInfo(char *pBuff, int nBuffSize);

int	Z_GetHardwareInfo_NCR(char *pBuff, int nBuffSize);

//系统深度休眠
void Z_DeepSleep(void);
//系统关机
void Z_PowerOff(void);
//辅助照明灯开
void Z_SlvLightOn(void);
//辅助照明灯关
void Z_SlvLightOff(void);
//设置唤醒倒计时器,在完成计时时,将触发休眠唤醒;若未处理休眠状态,则不产生其它效果.
void Z_WakeupTimer(unsigned int ms);
int Z_SetDeviceSerial(char *serialNum, int len);  //产品序列号设置
BOOL Z_SetDeviceManufacturer(char *str, int len); //产品制造商名称

void Z_SetCMOSCollect(int mode);	//cmos 启动采集
int Z_GetCMOSCollect(void);		//查询cmos采集是否结束


int Z_GetCMOSDMANum(void);//获取已完成采集的DMA表的数字
void Z_SetCMOSDMAInuseNum(int num);//设置要锁定的DMA表的数字
int* Z_cmos_GetIMGbuff_Ex(void);//30FPS模式下获取被锁定的用于解码的图像地址

char* Z_Get_ISRAM_Addr(void);//获取CPU内部SRAM地址，存储空间大小128kBytes
void Z_usbResumeHandle(void *p);

void Z_SetPowerLedOn(BOOL b);
void Z_CmosCapture(BOOL b, int mode);
BYTE* Z_GetCmosBuf(unsigned int mode, unsigned int timeout);
BYTE* Z_GetCmosBufTest(int n);

int Z_GetCmosWidth(void);
int Z_GetCmosHeight(void);

void Z_SetCmosMirror(int nMode);	//nMode: 0 - Normal; 1 - horizontal mirror; 2 - vertical flip; 3 - horizontal mirror & vertical flip.

unsigned int Z_GetCRC32(unsigned char *pdata, int len);

/**************************
固件更新
	port:   0 = RS232
			1 = Datapipe
			2 = USB COM
 **************************/
void Z_FirmwareUpgrade(int port);


/*****************************************
获取内核信息
	type:	0  = Kernel version
			1  = Device name
*****************************************/
int	Z_GetKernelInfo(int type, char *pBuff, int nBuffSize);


#define FREQ_RUN_HIGH		0
#define FREQ_RUN_LOW		1


//设置频率，是否使能变更电压
void Z_SetPM(int nFreqMode, BOOL bVoltage);
void Z_SetPM_EX(int nFreqMode);
int Z_GetPM(void);
void Z_TransferIdleTickVar(int *pTick);
//设置成低核心频率运行
//void Z_LowCoreFreq(void);
//设置成高核心频率运行
//void Z_HighCoreFreq(void);
void Z_GetCPUID(unsigned char *pcpuID);

unsigned int Z_EncryptionWrite(unsigned char *buff, unsigned int nsize, unsigned int flag);
unsigned int Z_EncryptionRead(unsigned char *buff, unsigned int nsize, unsigned int flag);

void Z_KBWSetCharEncoding(int nMode);
void Z_isSetKBWrestore(int nFormat);
void Z_isDecoding(int nFlag);
//////////////////////////////////////////////////////////////////////////
//设置用户的基本功能替代函数
void Z_SetUserFunc(UTGeneralParam *psParam);

//////////////////////////////////////////////////////////////////////////
//
#ifdef __cplusplus
}
#endif

#endif //__UIMG_DEVELOP_INTERFACE_OSIN_H__

