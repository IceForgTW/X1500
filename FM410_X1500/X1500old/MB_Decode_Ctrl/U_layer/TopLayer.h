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
	FNC_uappProcDecMsg fnc_uappProcDecMsg;	//���ڴ���(����)��������Ϣ
	void * Reserve[15];
}TUAPPExtFunc;
/************************************************************************/
/* ���������                                                         */
/************************************************************************/
//���ý�����Ϣ�洢��ַ����Ϣ����
void	U_appSetMsg(BYTE* pBuff, int nLen);
//��ȡ������Ϣ�洢�Ŀռ��С
int		U_appGetMsgBuffSize(void);
//��ȡ������Ϣ�洢��ַ
BYTE*	U_appGetMsgBuff(void);
//��ȡ������Ϣ����
int		U_appGetMsgLen(void);
//��ȡ������Ϣ�ķ��࣬1D��2D��OCR
int		U_appGetCategory(void);
//���ý�����Ϣ����
void	U_appSetCategory(int nCategory);
//��ȡ������Ϣ�ľ�����������
int		U_appGetCodeType(void);
//���ý�����Ϣ�ľ�����������
void	U_appSetCodeType(int nType);
//��ȡ���봦����ֵ
int		U_appGetResult(void);
//���ý��봦����ֵ
void	U_appSetResult(int nResult);

//����������ʾ
void U_appDevicePowerOnBeep(void);

//�ж��Ƿ�ɽ��ж���
BOOL IsModeToReadCode(void);

//�ж��Ƿ�ɽ��ж���
int U_appGetSenseImgChkEx(int SetReadOkFlag);
int U_appGetSenseImgChk(void);

//nMode = 0  һ��ʶ������ʱ���������ֹͣ
//nMode = 1  һ��ʶ������������ֹͣ
//nMode = -1 ֹͣʶ��
int U_appSetDecodeMode(int nMode);

void NCR_Get_New_Img(void);
// void SetExprosureMode(void);
void KbwSetCharEncoding(void);
/************************************************************************/
/* ���������                                                         */
/************************************************************************/
//����һ����ͼ��
BOOL	U_appCaptureImg(BOOL bWithLight);

UTImgParam * U_appChkToReadAndCapture(BOOL FirstFlag, int nDevMode);

/*
	nFlag
		= 0 ����ʧ�ܺ�Ĵ���
		= ��������ֵ������ɹ���Ĵ���
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

//����
BOOL	U_appProcDecode(void);
//���Զ������ӵ�ͼ��������
BOOL	U_appDecodeWithMacro(void);
BOOL	U_appDecodeWithMacroNew(void);

//�������ģʽ��nTestMode = 0,������; = 1��ʱ��, nTestParam���������ʱ��
int U_appRunDecodeTest(int nTestMode, int nTestParam);

//nMode: 0 = �����в���, 1 = �ڲ�����������, 2= �ڲ���ʱ�����, 3=�û�����������
//nParam ���ԵĴ�����ʱ��
int U_appSetTestMode(int nMode, int nParam);

//�����û����ֹ��ܵĻص�����
void	U_appSetUserFunc(UTGeneralParam *psParam);
//�ж��Ƿ񰴼�����
BOOL	U_appKeyPressed(void);

//����Idleģʽ
void	U_appSetIdleMode(void);

//����Standbyģʽ
void	U_appSetStandbyMode(void);

//��������ģʽ
void	U_appSleepMode(void);
//�ж��Ƿ��Զ���������ģʽ����������ָ������ʱ�䳤����û�а���
void	U_appWillSleepTillWakeup(int nIdleMiliSec);
//��������ָ��
BOOL	U_appDealCmd(BYTE *pCmdString, int nLen);
//��Ϣ�༭
BOOL	U_appMsgEditting(void);
//��Ϣ���
BOOL	U_appMsgPacking(void);
//�����ֹ�ַ�
BOOL	U_appMsgAddTerminal(void);
/************************************************************************/
/* �ṩ��ϵͳ����                                                       */
/************************************************************************/
//��ͨѶ�ڷ��Ϳ�����Ϣ
void	U_appPowerOnDisplay(void);
//������������ʱ��׼������
void	U_appGoingToSleep(void);
//�˳�����״̬ʱ�Ļָ�����
void	U_appBackFromSleep(void);
//�Զ���̨��������ͨѶ��������ȡ����ʼ����
void	U_appBKGrndProc(void);
//��ѯ�����ӿ�
void	U_appQueryAPI(UTGeneralParam *psParam);
//��ѯuIMG�汾��Ϣ
int		U_appGetVersionInfo(char *pBuff, int nBuffSize);
/************************************************************************/

//��ȡͼ��������ֵ
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

