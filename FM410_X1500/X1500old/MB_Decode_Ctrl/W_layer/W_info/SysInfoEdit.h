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
//编辑信息,并返回编辑后的长度
int		W_infoEditting(WInfoParam *psParam);
//对信息进行打包
int		W_infoPacking(WInfoParam *psParam);
//添加终止字符
int		W_infoAddTerminal(WInfoParam *psParam);
#ifdef __cplusplus
}
#endif


#endif//__UIMG_DEVELOP_INTERFACE_INFOEDIT_H__

