/********************************************************************
	created:	2007/02/09
	created:	9:2:2007   10:14
	filename: 	Comm_Method.cpp
	file path:	
	file base:	Comm_Method
	file ext:	cpp
	author:		LieMa
	
	purpose:	通讯控制的封装
*********************************************************************/

#include "Y_paramStorage.h"

/************************************************************************/
/* 类型定义                                                             */
/************************************************************************/

/************************************************************************/
/* 全局标志量                                                           */
/************************************************************************/

/************************************************************************/
/* 宏定义                                                               */
/************************************************************************/
//每个存储块的大小都是4096字节
#define FLASH_BLOCK_SIZE		4096

//uIMG参数存储块
#define PRI_SYSDEF_PARAM_BLOCK			0
//uIMG参数备份块
#define BAK_SYSDEF_PARAM_BLOCK			1
//uIMG参数//用户自定义默认存储块
#define PRI_USRDEF_PARAM_BLOCK			2 
//uIMG参数//用户自定义默认存储块(备份)
#define BAK_USRDEF_PARAM_BLOCK			3 
// OEM Firmware Param    //提供给OEM厂商的参数存储位置，起始内存块，存储长度
#define OEM_PARAM_BLOCK					4
//uIMG参数//用户出厂参数默认存储块
#define PRI_Factory_PARAM_BLOCK			5
//uIMG参数//用户出厂参数默认存储块(备份)
#define BAK_Factory_PARAM_BLOCK			6

//APP 可使用的存储空间
#define APP_PARAM_BLK_1		7
#define APP_PARAM_BLK_2		8
#define APP_PARAM_BLK_3		9
#define APP_PARAM_BLK_4		10
#define APP_PARAM_BLK_5     11

/************************************************************************/
/* 功能实现                                                             */
/************************************************************************/
//导入系统默认参数
BOOL Y_paramLoadSysDefault(unsigned char *pBuff, int nSize)
{
	if (pBuff == NULL || nSize <= 0)
	{ return FALSE; }//End of if;
	if (Z_param_Load(pBuff,PRI_SYSDEF_PARAM_BLOCK,nSize) != nSize)
	{
		if(Z_param_Load(pBuff,BAK_SYSDEF_PARAM_BLOCK,nSize) != nSize)
		{ return FALSE; }
	}//End of if;

	return TRUE;
}//End of function;

//导入用户自定义默认参数
BOOL Y_paramLoadUsrDefault(unsigned char *pBuff, int nSize)
{
	if (pBuff == NULL || nSize <= 0)
	{ return FALSE; }//End of if;
	if (Z_param_Load(pBuff,PRI_USRDEF_PARAM_BLOCK,nSize) != nSize)
	{
		if(Z_param_Load(pBuff,BAK_USRDEF_PARAM_BLOCK,nSize) != nSize)
		{ return FALSE; }
	}//End of if;

	return TRUE;
}//End of function;

//导入用户出厂参数
BOOL Y_paramLoadFactory(unsigned char *pBuff, int nSize)
{
	if (pBuff == NULL || nSize <= 0)
	{ return FALSE; }//End of if;
	if (Z_param_Load(pBuff,PRI_Factory_PARAM_BLOCK,nSize) != nSize)
	{
		if(Z_param_Load(pBuff,BAK_Factory_PARAM_BLOCK,nSize) != nSize)
		{ return FALSE; }
	}//End of if;
	
	return TRUE;
}//End of function;

//导入OEM参数
BOOL Y_paramLoadOEM(unsigned char *pBuff, int nSize)
{
	if (pBuff == NULL || nSize <= 0)
	{ return FALSE; }//End of if;
	if(Z_param_Load(pBuff,OEM_PARAM_BLOCK,nSize) != nSize)
	{ return FALSE; }

	return TRUE;
}//End of function;

//读取App的参数块,序号为0~4,每块大小为4096字节
BOOL Y_paramLoadAppBlock(int nBlockOrder, unsigned char *pBuff, int nSize)
{
	nBlockOrder += APP_PARAM_BLK_1;
	if (nBlockOrder < APP_PARAM_BLK_1 || nBlockOrder > APP_PARAM_BLK_5)
	{ return FALSE; }//End of if;
	if (pBuff == NULL || nSize <= 0)
	{ return FALSE; }//End of if;
	if(Z_param_Load(pBuff,nBlockOrder,nSize) != nSize)
	{ return FALSE; }

	return TRUE;
}//End of function;

//存储系统默认参数
BOOL Y_paramSaveSysDefault(unsigned char *pBuff, int nSize)
{
	if (pBuff == NULL || nSize <= 0 || nSize > FLASH_BLOCK_SIZE)
	{ return FALSE; }//End of if;
	if (Z_param_Save(pBuff,PRI_SYSDEF_PARAM_BLOCK,nSize) != nSize)
	{
		if(Z_param_Save(pBuff,BAK_SYSDEF_PARAM_BLOCK,nSize) != nSize)
		{ return FALSE; }
	}//End of if;
	else
	{
		Z_param_Save(pBuff,BAK_SYSDEF_PARAM_BLOCK,nSize);
	}//End of else;

	return TRUE;
}//End of function;

//存储用户自定义默认参数
BOOL Y_paramSaveUsrDefault(unsigned char *pBuff, int nSize)
{
	if (pBuff == NULL || nSize <= 0 || nSize > FLASH_BLOCK_SIZE)
	{ return FALSE; }//End of if;
	if (Z_param_Save(pBuff,PRI_USRDEF_PARAM_BLOCK,nSize) != nSize)
	{
		if(Z_param_Save(pBuff,BAK_USRDEF_PARAM_BLOCK,nSize) != nSize)
		{ return FALSE; }
	}//End of if;
	else
	{
		Z_param_Save(pBuff,BAK_USRDEF_PARAM_BLOCK,nSize);
	}//End of else;

	return TRUE;
}//End of function;

//存储用户出厂参数
BOOL Y_paramSaveFactory(unsigned char *pBuff, int nSize)
{
	if (pBuff == NULL || nSize <= 0 || nSize > FLASH_BLOCK_SIZE)
	{ return FALSE; }//End of if;
	if (Z_param_Save(pBuff,PRI_Factory_PARAM_BLOCK,nSize) != nSize)
	{
		if(Z_param_Save(pBuff,BAK_Factory_PARAM_BLOCK,nSize) != nSize)
		{ return FALSE; }
	}//End of if;
	else
	{
		Z_param_Save(pBuff,BAK_Factory_PARAM_BLOCK,nSize);
	}//End of else;
	
	return TRUE;
}//End of function;

//存储OEM参数
BOOL Y_paramSaveOEM(unsigned char *pBuff, int nSize)
{
	if (pBuff == NULL || nSize <= 0 || nSize > FLASH_BLOCK_SIZE)
	{ return FALSE; }//End of if;
	if(Z_param_Save(pBuff,OEM_PARAM_BLOCK,nSize) != nSize)
	{ return FALSE; }

	return TRUE;
}//End of function;

//存储App的参数块,序号为0~4,每块大小为4096字节
BOOL Y_paramSaveAppBlock(int nBlockOrder, unsigned char *pBuff, int nSize)
{
	nBlockOrder += APP_PARAM_BLK_1;
	if (nBlockOrder < APP_PARAM_BLK_1 || nBlockOrder > APP_PARAM_BLK_5)
	{ return FALSE; }//End of if;
	if (pBuff == NULL || nSize <= 0 || nSize > FLASH_BLOCK_SIZE)
	{ return FALSE; }//End of if;
	if(Z_param_Save(pBuff,nBlockOrder,nSize) != nSize)
	{ return FALSE; }

	return TRUE;
}//End of function;
