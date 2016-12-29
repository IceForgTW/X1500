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
//�ж��Ƿ�ȫ�ֲ����Ѿ���ʼ��
BOOL	X_paramIsAllParamInited(void);
//ȫ�ֲ�����ʼ��
void	X_paramInitedAllParam(void);
//��ȡȫ�ֲ����Ľṹ��ַ��ʹ�þ������
UALLPARAM * X_paramGetGlobalAllParam(void);
UALLPARAM * X_paramBakGetGlobalAllParam(void);
//��ȡ���������Ľṹ��ַ
UTBlueTooth * X_paramGetBlueT(void);

//����ϵͳĬ�ϲ���
BOOL X_paramLoadSysDefaultParam(void);
//�����û�Ĭ�ϲ���
BOOL X_paramLoadUsrDefaultParam(void);
//�����������
BOOL X_paramLoadFactoryParam(void);
//����ΪϵͳĬ�ϲ���
BOOL X_paramSaveAsSysDefaultParam(void); 
//����Ϊ�û�Ĭ�ϲ���
BOOL X_paramSaveAsUsrDefaultParam(void);
//����Ϊ��������
BOOL X_paramSaveAsFactoryParam(void);
//�����豸��Ϣ
BOOL X_paramLoadDeviceInfo(void); 
//�����豸��Ϣ
BOOL X_paramSaveDeviceInfo(void);
//����Ĭ�ϳ�ʼ��ȫ�ֲ���
BOOL X_paramOriginalInitAllParam(void);

BOOL X_paramInitAllParamForNCR(void);

//�滻�ؼ���
BOOL X_paramPlaceKeyword(char* pKeyWord,char* pValue, BOOL bReplace);
//��ѯ�ؼ����Ƿ����
BOOL X_paramQueryKeyword(char* pKeyWord,char* pValue);
//�������ؼ���
int X_paramAnalysisKeyword(unsigned char* pParam,int nParam,unsigned char* pWord,unsigned char * pValue);
//��ȡ�ؼ��ֽṹ
int X_paramGetKeywordsStruct(unsigned char *pBuff,int nBuff,TKEYSEG* tKS);
//���ָ����ַ�ռ����Ƿ�Ϊ��ȷ��ȫ�ֲ���
BOOL X_paramIsParamsRight(BYTE *pBuff, int nSize);
//��ָ����ַ�ռ��л�ȡȫ�ֲ���
BOOL X_paramGetParamFromBuff(BYTE *pBuff, int nSize);
//��ȫ�ֲ������浽ָ����ַ�ռ���
int X_paramPushParamToBuff(BYTE *pBuff, int nBuffSize);
//��ͨѶ�ڷ��Ͳ�����Ϣ
void X_paramSendParamInfo(void);
//�����еĲ����ı䣬���³��豸�ĵ�ǰ���ã�����ͨѶ������
void X_paramUpdateToDevice(void);
//�ж�����ͨѶ�����Ϳ���
int  X_paramSelectUsefulCommType(void);
//���ͨѶ�����Ƿ���ָ����ͨѶ����,��������ͼ�����л�
void X_paramCheckCommType(void);
//������չ����
void X_paramSetUserFunc(UTGeneralParam *psParam);

//����������ʼ�����Ӿ��豸flash�ж����Ĳ����������Ĳ�����һ�£���ִ�У�
void x_paramNewParamInit(void);

void X_SysParamCpy(void);
BOOL X_paramGetFactoryParam(unsigned char *buff, unsigned int nSize);
//��Ҫ���ڸ��¾���
BOOL uJ_HardTypeParam(void);

void paramSaveType(unsigned char saveFlag);

unsigned char getParamSaveType(void);

#ifdef __cplusplus
}
#endif
#endif//___UIMG_DEVELOP_INTERFACE_DATATRANSFORM_H___

