                      
#ifndef ___UIMG_DEVELOP_INTERFACE_PROTOCOL_PRASE_H___
#define ___UIMG_DEVELOP_INTERFACE_PROTOCOL_PRASE_H___

#include "../../list.h"


#ifdef __cplusplus
extern "C"{
#endif 

typedef void	(*FNC_commFrameBeforeTranslateMes)(char FrameProper, char *pcFrameMsg, int nMsgLen);
typedef void	(*FNC_commProcManualFrame)(char FstChar);
typedef int		(*FNC_commAddVerionMsg)(char *pBuff, int nBuffSize);
//用于传递自动处理通讯消息的扩展功能函数
typedef struct zspVCOMMEXTFUN
{
	FNC_commFrameBeforeTranslateMes	fnc_FrameBFTranslateMes;
	FNC_commProcManualFrame			fnc_ProcManualFrame;
}TVCOMMExtFunc;
//自动检测通讯口中收到的信息, 并处理其指令
void V_commAutoCheckCmd(void);

//设置扩展功能函数
void V_commSetUserFunc(UTGeneralParam *psParam);

//获取版本信息
int V_commGetVersion(char *pBuff, int nMaxLen);

//分析并运行QuickSet 命令，不带nls头缀，但语法格式一样，每条命令后必须有';'符号
BOOL V_RunQuickSetCmd(char *buff);

BOOL SurePOS_GetScannerState(void);
void SurePOS_SetScannerState(BOOL bEnable);
int SurePOS_SendData(void);
BOOL IsSurePOSOpen(void);
BOOL SurePOS_TickleFeed(void);
int Send_DirectIO_Return_Data(BYTE *buff, int buffLen);
void SaveGoodDecodeImg(BYTE * pImg, int nSize);


#ifdef __cplusplus
}
#endif 

#endif//___UIMG_DEVELOP_INTERFACE_PROTOCOL_PRASE_H___
