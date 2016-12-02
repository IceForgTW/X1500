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
//设置码处理参数初始化
void	W_setcParamInit(void); 
//判断是否是"宏连接取消"的功能设置码
BOOL	W_setcIsMacroCancel(BYTE* pbyMsg, int nMsgLen);
//获取设置码处理结果
int		W_setcGetResult(void);
//设置设置码处理结果
void	W_setcSetResult(int nResult);
//判断是否要保存设置码处理后的全局参数
BOOL	W_setcToSaveParam(void);
//设置处理后的全局参数保存标志
void	W_setcSetSaveFlag(BOOL bToSave);

//设置处理后的全局参数不保存标志
void W_setcSetNoSaveFlag(BOOL bNoSave);
//判断是否不保存设置码处理后的全局参数
BOOL W_setcToNoSaveParam(void);

//设置码使能或关闭
void	W_setcEnable(BOOL bEnable);
//判断设置码是否已经使能
BOOL	W_setcIsEnable(void);
//对单条设置信息或指令的处理接口
BOOL	W_setcParseSigle(BYTE* pbyMsg, int nMsgLen, BOOL bToSave);
//对批量设置信息或指令的处理接口
BOOL	W_setcParseBatch(BYTE* pbyMsg, int nMsgLen);

BOOL Set_DecodeMode(int Rec34, int Rec56, int Rec7, int RecParam);  //解码模式符设置

//自动处理解码信息中的设置码
#define SETCPROC_ERROR	0
#define SETCRPOC_SUCC	1
#define SETCPROC_NOT	2
int		W_setcDealWith(void);

//设置扩展功能函数
void	W_setcSetUserFunc(UTGeneralParam *psParam);
/*********************************************************************************************
*	global function prototype
*********************************************************************************************/


#ifdef __cplusplus
}
#endif

#endif //#ifndef __UIMG_DEVELOP_INTERFACE_SETCODEPARSE_H__ 

