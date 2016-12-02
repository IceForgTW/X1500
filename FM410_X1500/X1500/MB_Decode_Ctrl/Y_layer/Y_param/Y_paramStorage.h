
#ifndef __UIMG_DEVELOP_INTERFACE_YSTORAGE_H__
#define __UIMG_DEVELOP_INTERFACE_YSTORAGE_H__


#include "../../list.h"
/************************************************************************/
/* ���Ͷ���                                                             */
/************************************************************************/

/************************************************************************/
/* ȫ�ֱ�־��                                                           */
/************************************************************************/

/************************************************************************/
/* �궨��                                                               */
/************************************************************************/

/************************************************************************/
/*  ����ʵ��                                                            */
/************************************************************************/

#ifdef __cplusplus
extern"C"{
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//	�洢�ӿڷ�װ
//����ϵͳĬ�ϴ洢������
BOOL Y_paramLoadSysDefault(unsigned char *pBuff, int nSize);
//�����û�Ĭ�ϴ洢������
BOOL Y_paramLoadUsrDefault(unsigned char *pBuff, int nSize);
//������������洢������
BOOL Y_paramLoadFactory(unsigned char *pBuff, int nSize);
//����OEM�����洢������
BOOL Y_paramLoadOEM(unsigned char *pBuff, int nSize);
//����Ӧ�ó���洢������
//��ȡApp�Ĳ�����,���Ϊ0~5,ÿ���СΪ4096�ֽ�
BOOL Y_paramLoadAppBlock(int nBlockOrder, unsigned char *pBuff, int nSize);
//�����ݴ洢��ϵͳĬ����
BOOL Y_paramSaveSysDefault(unsigned char *pBuff, int nSize);
//�����ݴ洢���û�Ĭ����
BOOL Y_paramSaveUsrDefault(unsigned char *pBuff, int nSize);
//�����ݴ洢������������
BOOL Y_paramSaveFactory(unsigned char *pBuff, int nSize);
//�����ݴ洢��OEM������
BOOL Y_paramSaveOEM(unsigned char *pBuff, int nSize);
//�����ݴ洢��Ӧ�ó��������
//�洢App�Ĳ�����,���Ϊ0~5,ÿ���СΪ4096�ֽ�
BOOL Y_paramSaveAppBlock(int nBlockOrder, unsigned char *pBuff, int nSize);
//	�洢�ӿڷ�װ
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
#ifdef __cplusplus
}
#endif

#endif //__UIMG_DEVELOP_INTERFACE_YSTORAGE_H__

