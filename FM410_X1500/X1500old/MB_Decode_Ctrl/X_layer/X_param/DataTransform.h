#ifndef ___UIMG_DEVELOP_INTERFACE_DATATRANSFORM_H___
#define ___UIMG_DEVELOP_INTERFACE_DATATRANSFORM_H___

#include "../../list.h"

typedef void	(*FNC_paramOriginalUserDefault)(UALLPARAM* psParam);
typedef void	(*FNC_param2DevLoadUserDefault)(void);
typedef void	(*FNC_param2DevSaveUserDefault)(void);
typedef struct zspXPARAMEXTFUNC
{
	FNC_paramOriginalUserDefault	fnc_OriginalUserDefault;
	FNC_param2DevLoadUserDefault	fnc_2DevLoadUserDefault;
	FNC_param2DevSaveUserDefault	fnc_2DevSaveUserDefault;
	void* Reserve[12];
}TXPARAMExtFunc;

#ifdef __cplusplus
extern "C"{
#endif
//判断是否全局参数已经初始化
BOOL	X_paramIsAllParamInited(void);
//全局参数初始化
void	X_paramInitedAllParam(void);
//获取全局参数的结构地址以使用具体参数
UALLPARAM * X_paramGetGlobalAllParam(void);
UALLPARAM * X_paramBakGetGlobalAllParam(void);
//获取蓝牙参数的结构地址
UTBlueTooth * X_paramGetBlueT(void);

//导入系统默认参数
BOOL X_paramLoadSysDefaultParam(void);
//导入用户默认参数
BOOL X_paramLoadUsrDefaultParam(void);
//导入出厂配置
BOOL X_paramLoadFactoryParam(void);
//保存为系统默认参数
BOOL X_paramSaveAsSysDefaultParam(void); 
//保存为用户默认参数
BOOL X_paramSaveAsUsrDefaultParam(void);
//保存为出厂配置
BOOL X_paramSaveAsFactoryParam(void);
//导入设备信息
BOOL X_paramLoadDeviceInfo(void); 
//保存设备信息
BOOL X_paramSaveDeviceInfo(void);
//出厂默认初始化全局参数
BOOL X_paramOriginalInitAllParam(void);

BOOL X_paramInitAllParamForNCR(void);

//替换关键字
BOOL X_paramPlaceKeyword(char* pKeyWord,char* pValue, BOOL bReplace);
//查询关键字是否存在
BOOL X_paramQueryKeyword(char* pKeyWord,char* pValue);
//解析出关键字
int X_paramAnalysisKeyword(unsigned char* pParam,int nParam,unsigned char* pWord,unsigned char * pValue);
//获取关键字结构
int X_paramGetKeywordsStruct(unsigned char *pBuff,int nBuff,TKEYSEG* tKS);
//检查指定地址空间中是否为正确的全局参数
BOOL X_paramIsParamsRight(BYTE *pBuff, int nSize);
//从指定地址空间中获取全局参数
BOOL X_paramGetParamFromBuff(BYTE *pBuff, int nSize);
//将全局参数保存到指定地址空间中
int X_paramPushParamToBuff(BYTE *pBuff, int nBuffSize);
//往通讯口发送参数信息
void X_paramSendParamInfo(void);
//将所有的参数改变，更新成设备的当前配置，包括通讯参数等
void X_paramUpdateToDevice(void);
//判断哪种通讯口类型可用
int  X_paramSelectUsefulCommType(void);
//检查通讯类型是否是指定的通讯类型,若否则试图进行切换
void X_paramCheckCommType(void);
//设置扩展功能
void X_paramSetUserFunc(UTGeneralParam *psParam);

//新增参数初始化（从旧设备flash中读出的参数跟新增的参数不一致，就执行）
void x_paramNewParamInit(void);

void X_SysParamCpy(void);
BOOL X_paramGetFactoryParam(unsigned char *buff, unsigned int nSize);
//主要用于更新镜像
BOOL uJ_HardTypeParam(void);

void paramSaveType(unsigned char saveFlag);

unsigned char getParamSaveType(void);

#ifdef __cplusplus
}
#endif
#endif//___UIMG_DEVELOP_INTERFACE_DATATRANSFORM_H___

