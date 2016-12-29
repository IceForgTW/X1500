                      
#ifndef ___UIMG_DEVELOP_INTERFACE_PROTOCOL_PRASE_H___
#define ___UIMG_DEVELOP_INTERFACE_PROTOCOL_PRASE_H___

#include "../../list.h"


#ifdef __cplusplus
extern "C"{
#endif 

typedef void	(*FNC_commFrameBeforeTranslateMes)(char FrameProper, char *pcFrameMsg, int nMsgLen);
typedef void	(*FNC_commProcManualFrame)(char FstChar);
typedef int		(*FNC_commAddVerionMsg)(char *pBuff, int nBuffSize);
//���ڴ����Զ�����ͨѶ��Ϣ����չ���ܺ���
typedef struct zspVCOMMEXTFUN
{
	FNC_commFrameBeforeTranslateMes	fnc_FrameBFTranslateMes;
	FNC_commProcManualFrame			fnc_ProcManualFrame;
}TVCOMMExtFunc;
//�Զ����ͨѶ�����յ�����Ϣ, ��������ָ��
void V_commAutoCheckCmd(void);

//������չ���ܺ���
void V_commSetUserFunc(UTGeneralParam *psParam);

//��ȡ�汾��Ϣ
int V_commGetVersion(char *pBuff, int nMaxLen);

//����������QuickSet �������nlsͷ׺�����﷨��ʽһ����ÿ������������';'����
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
