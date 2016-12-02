#ifndef __UIMG_DEVELOP_INTERFACE_INFOEDIT_H__
#define __UIMG_DEVELOP_INTERFACE_INFOEDIT_H__

/*********************************************************************************************
*	include files 
*********************************************************************************************/
#include "../../list.h"

typedef struct zspWINFOPARAM
{
	BYTE * pbyMsgBuff;
	int nMsgLen;
	int nBuffSize;
	int nCodeCategory;
	int nCodeType;
	int nCodePos;
}WInfoParam;
/*********************************************************************************************
*	global referenced macro defines & type defines
*********************************************************************************************/

#ifdef __cplusplus
extern"C"{
#endif

void	W_infoInitEveryTime(void);
//�༭��Ϣ,�����ر༭��ĳ���
int		W_infoEditting(WInfoParam *psParam);
//����Ϣ���д��
int		W_infoPacking(WInfoParam *psParam);
//�����ֹ�ַ�
int		W_infoAddTerminal(WInfoParam *psParam);
#ifdef __cplusplus
}
#endif


#endif//__UIMG_DEVELOP_INTERFACE_INFOEDIT_H__

