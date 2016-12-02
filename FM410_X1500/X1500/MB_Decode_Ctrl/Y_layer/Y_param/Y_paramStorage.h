
#ifndef __UIMG_DEVELOP_INTERFACE_YSTORAGE_H__
#define __UIMG_DEVELOP_INTERFACE_YSTORAGE_H__


#include "../../list.h"
/************************************************************************/
/* 类型定义                                                             */
/************************************************************************/

/************************************************************************/
/* 全局标志量                                                           */
/************************************************************************/

/************************************************************************/
/* 宏定义                                                               */
/************************************************************************/

/************************************************************************/
/*  函数实现                                                            */
/************************************************************************/

#ifdef __cplusplus
extern"C"{
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//	存储接口封装
//读入系统默认存储区数据
BOOL Y_paramLoadSysDefault(unsigned char *pBuff, int nSize);
//读入用户默认存储区数据
BOOL Y_paramLoadUsrDefault(unsigned char *pBuff, int nSize);
//读入出厂参数存储区数据
BOOL Y_paramLoadFactory(unsigned char *pBuff, int nSize);
//读出OEM参数存储区数据
BOOL Y_paramLoadOEM(unsigned char *pBuff, int nSize);
//读出应用程序存储区数据
//读取App的参数块,序号为0~5,每块大小为4096字节
BOOL Y_paramLoadAppBlock(int nBlockOrder, unsigned char *pBuff, int nSize);
//将数据存储到系统默认区
BOOL Y_paramSaveSysDefault(unsigned char *pBuff, int nSize);
//将数据存储到用户默认区
BOOL Y_paramSaveUsrDefault(unsigned char *pBuff, int nSize);
//将数据存储到出厂参数区
BOOL Y_paramSaveFactory(unsigned char *pBuff, int nSize);
//将数据存储到OEM参数区
BOOL Y_paramSaveOEM(unsigned char *pBuff, int nSize);
//将数据存储到应用程序参数区
//存储App的参数块,序号为0~5,每块大小为4096字节
BOOL Y_paramSaveAppBlock(int nBlockOrder, unsigned char *pBuff, int nSize);
//	存储接口封装
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
#ifdef __cplusplus
}
#endif

#endif //__UIMG_DEVELOP_INTERFACE_YSTORAGE_H__

