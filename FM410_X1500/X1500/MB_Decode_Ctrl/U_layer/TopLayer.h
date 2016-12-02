#ifndef __UIMG_DEVELOP_INTERFACE_TOPLAYER_H__
#define __UIMG_DEVELOP_INTERFACE_TOPLAYER_H__

#include "../Common.h"
#include "../list.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"{
#endif 

typedef void (*FNC_uappProcDecMsg)(void);
typedef struct zspUDEVEXTFNC{
	FNC_uappProcDecMsg fnc_uappProcDecMsg;	//用于处理(或发送)解码后的信息
	void * Reserve[15];
}TUAPPExtFunc;
/************************************************************************/
/* 处理结果相关                                                         */
/************************************************************************/
//设置解码信息存储地址和信息长度
void	U_appSetMsg(BYTE* pBuff, int nLen);
//获取解码信息存储的空间大小
int		U_appGetMsgBuffSize(void);
//获取解码信息存储地址
BYTE*	U_appGetMsgBuff(void);
//获取解码信息长度
int		U_appGetMsgLen(void);
//获取解码信息的分类，1D或2D或OCR
int		U_appGetCategory(void);
//设置解码信息分类
void	U_appSetCategory(int nCategory);
//获取解码信息的具体码制类型
int		U_appGetCodeType(void);
//设置解码信息的具体码制类型
void	U_appSetCodeType(int nType);
//获取解码处理返回值
int		U_appGetResult(void);
//设置解码处理返回值
void	U_appSetResult(int nResult);

//开机铃音提示
void U_appDevicePowerOnBeep(void);

//判断是否可进行读码
BOOL IsModeToReadCode(void);

//判断是否可进行读码
int U_appGetSenseImgChkEx(int SetReadOkFlag);
int U_appGetSenseImgChk(void);

//nMode = 0  一次识读，超时或读到条码停止
//nMode = 1  一次识读，读到条码停止
//nMode = -1 停止识读
int U_appSetDecodeMode(int nMode);

void NCR_Get_New_Img(void);
// void SetExprosureMode(void);
void KbwSetCharEncoding(void);
/************************************************************************/
/* 处理方法相关                                                         */
/************************************************************************/
//拍摄一幅新图像
BOOL	U_appCaptureImg(BOOL bWithLight);

UTImgParam * U_appChkToReadAndCapture(BOOL FirstFlag, int nDevMode);

/*
	nFlag
		= 0 解码失败后的处理
		= 其它任意值，解码成功后的处理
	nMode
		bit 0: package and edit result?
		bit 1: send data?
		bit 2: beep?
	nDevMode
		= DECODE_COLLECT_MODE
*/
void U_appAfterDecodeProc(int nFlag, int nMode, int nDevMode);

/*
	nMode
		bit 00: package and edit result?
		bit 01: send data?
		bit 02: beep?
		bit 16: RunDecode?
*/
BOOL U_appProcDecodeEx(int nMode);

//解码
BOOL	U_appProcDecode(void);
//带自动宏连接的图像处理及解码
BOOL	U_appDecodeWithMacro(void);
BOOL	U_appDecodeWithMacroNew(void);

//进入测试模式，nTestMode = 0,按次数; = 1按时间, nTestParam代表次数或时间
int U_appRunDecodeTest(int nTestMode, int nTestParam);

//nMode: 0 = 不进行测试, 1 = 内部按次数测试, 2= 内部按时间测试, 3=用户按次数测试
//nParam 测试的次数或时间
int U_appSetTestMode(int nMode, int nParam);

//设置用户各种功能的回调函数
void	U_appSetUserFunc(UTGeneralParam *psParam);
//判断是否按键按下
BOOL	U_appKeyPressed(void);

//进入Idle模式
void	U_appSetIdleMode(void);

//进入Standby模式
void	U_appSetStandbyMode(void);

//进入休眠模式
void	U_appSleepMode(void);
//判断是否自动进入休眠模式，依据是在指定空闲时间长度内没有按键
void	U_appWillSleepTillWakeup(int nIdleMiliSec);
//处理设置指令
BOOL	U_appDealCmd(BYTE *pCmdString, int nLen);
//信息编辑
BOOL	U_appMsgEditting(void);
//信息打包
BOOL	U_appMsgPacking(void);
//添加终止字符
BOOL	U_appMsgAddTerminal(void);
/************************************************************************/
/* 提供给系统调用                                                       */
/************************************************************************/
//往通讯口发送开机信息
void	U_appPowerOnDisplay(void);
//即将进入休眠时的准备动作
void	U_appGoingToSleep(void);
//退出休眠状态时的恢复动作
void	U_appBackFromSleep(void);
//自动后台处理，包括通讯及参数存取及初始化等
void	U_appBKGrndProc(void);
//查询函数接口
void	U_appQueryAPI(UTGeneralParam *psParam);
//查询uIMG版本信息
int		U_appGetVersionInfo(char *pBuff, int nBuffSize);
/************************************************************************/

//获取图像清晰度值
void U_appGetDefinition(BOOL run);

void U_SetOPOSEnable(int mode);
void U_SetOPOSClaim(BOOL b);
void U_appOPOSMsgEditting(void);
BOOL U_GetOPOSClaimState(void);


// data format
int U_DataFormat(void);
int U_Set_OutMsg(BYTE *param,BYTE *AOutput,int size);

#ifdef __cplusplus
}
#endif 

#endif//__UIMG_DEVELOP_INTERFACE_TOPLAYER_H__

