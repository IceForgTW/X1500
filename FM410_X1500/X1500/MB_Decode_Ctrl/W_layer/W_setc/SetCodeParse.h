#ifndef __UIMG_DEVELOP_INTERFACE_SETCODEPARSE_H__
#define __UIMG_DEVELOP_INTERFACE_SETCODEPARSE_H__

/*********************************************************************************************
*	include files 
*********************************************************************************************/
#include "../../list.h"

#ifdef __cplusplus
extern "C"{
#endif
/*********************************************************************************************
*	global referenced macro defines & type defines
*********************************************************************************************/
   

/*********************************************************************************************
*	global referenced variables
*********************************************************************************************/

extern const unsigned char CodeIDTab1D[32];
extern const unsigned char CodeIDTab2D[32];
extern const unsigned char CodeIDPostal[32];
extern const unsigned char CodeIDTab1D_2[32];
extern const unsigned char CodeIDTab2D_2[32];
extern const unsigned char CodeIDPostal_2[32];
/*********************************************************************************************
*	global function prototype
*********************************************************************************************/
typedef void	(*FNC_setcDealUserFunc)(BOOL bSetCode, int nCodeCategory, int nCodeType, BYTE* pCodeMsg, int nMsgLen);

typedef struct zspWSETCEXTFUNC
{
	FNC_setcDealUserFunc	fnc_DealUserSettingCode;
}TWSETCExtFunc;
/*********************************************************************************************
*	global referenced variables
*********************************************************************************************/
//�����봦�������ʼ��
void	W_setcParamInit(void); 
//�ж��Ƿ���"������ȡ��"�Ĺ���������
BOOL	W_setcIsMacroCancel(BYTE* pbyMsg, int nMsgLen);
//��ȡ�����봦����
int		W_setcGetResult(void);
//���������봦����
void	W_setcSetResult(int nResult);
//�ж��Ƿ�Ҫ���������봦����ȫ�ֲ���
BOOL	W_setcToSaveParam(void);
//���ô�����ȫ�ֲ��������־
void	W_setcSetSaveFlag(BOOL bToSave);

//���ô�����ȫ�ֲ����������־
void W_setcSetNoSaveFlag(BOOL bNoSave);
//�ж��Ƿ񲻱��������봦����ȫ�ֲ���
BOOL W_setcToNoSaveParam(void);

//������ʹ�ܻ�ر�
void	W_setcEnable(BOOL bEnable);
//�ж��������Ƿ��Ѿ�ʹ��
BOOL	W_setcIsEnable(void);
//�Ե���������Ϣ��ָ��Ĵ���ӿ�
BOOL	W_setcParseSigle(BYTE* pbyMsg, int nMsgLen, BOOL bToSave);
//������������Ϣ��ָ��Ĵ���ӿ�
BOOL	W_setcParseBatch(BYTE* pbyMsg, int nMsgLen);

BOOL Set_DecodeMode(int Rec34, int Rec56, int Rec7, int RecParam);  //����ģʽ������

//�Զ����������Ϣ�е�������
#define SETCPROC_ERROR	0
#define SETCRPOC_SUCC	1
#define SETCPROC_NOT	2
int		W_setcDealWith(void);

//������չ���ܺ���
void	W_setcSetUserFunc(UTGeneralParam *psParam);
/*********************************************************************************************
*	global function prototype
*********************************************************************************************/


#ifdef __cplusplus
}
#endif

#endif //#ifndef __UIMG_DEVELOP_INTERFACE_SETCODEPARSE_H__ 

