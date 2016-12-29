/********************************************************************
	created:	2007/02/09
	created:	9:2:2007   10:14
	filename: 	Comm_Method.cpp
	file path:	
	file base:	Comm_Method
	file ext:	cpp
	author:		LieMa
	
	purpose:	ͨѶ���Ƶķ�װ
*********************************************************************/

#include "Y_paramStorage.h"

/************************************************************************/
/* ���Ͷ���                                                             */
/************************************************************************/

/************************************************************************/
/* ȫ�ֱ�־��                                                           */
/************************************************************************/

/************************************************************************/
/* �궨��                                                               */
/************************************************************************/
//ÿ���洢��Ĵ�С����4096�ֽ�
#define FLASH_BLOCK_SIZE		4096

//uIMG�����洢��
#define PRI_SYSDEF_PARAM_BLOCK			0
//uIMG�������ݿ�
#define BAK_SYSDEF_PARAM_BLOCK			1
//uIMG����//�û��Զ���Ĭ�ϴ洢��
#define PRI_USRDEF_PARAM_BLOCK			2 
//uIMG����//�û��Զ���Ĭ�ϴ洢��(����)
#define BAK_USRDEF_PARAM_BLOCK			3 
// OEM Firmware Param    //�ṩ��OEM���̵Ĳ����洢λ�ã���ʼ�ڴ�飬�洢����
#define OEM_PARAM_BLOCK					4
//uIMG����//�û���������Ĭ�ϴ洢��
#define PRI_Factory_PARAM_BLOCK			5
//uIMG����//�û���������Ĭ�ϴ洢��(����)
#define BAK_Factory_PARAM_BLOCK			6

//APP ��ʹ�õĴ洢�ռ�
#define APP_PARAM_BLK_1		7
#define APP_PARAM_BLK_2		8
#define APP_PARAM_BLK_3		9
#define APP_PARAM_BLK_4		10
#define APP_PARAM_BLK_5     11

/************************************************************************/
/* ����ʵ��                                                             */
/************************************************************************/
//����ϵͳĬ�ϲ���
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

//�����û��Զ���Ĭ�ϲ���
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

//�����û���������
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

//����OEM����
BOOL Y_paramLoadOEM(unsigned char *pBuff, int nSize)
{
	if (pBuff == NULL || nSize <= 0)
	{ return FALSE; }//End of if;
	if(Z_param_Load(pBuff,OEM_PARAM_BLOCK,nSize) != nSize)
	{ return FALSE; }

	return TRUE;
}//End of function;

//��ȡApp�Ĳ�����,���Ϊ0~4,ÿ���СΪ4096�ֽ�
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

//�洢ϵͳĬ�ϲ���
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

//�洢�û��Զ���Ĭ�ϲ���
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

//�洢�û���������
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

//�洢OEM����
BOOL Y_paramSaveOEM(unsigned char *pBuff, int nSize)
{
	if (pBuff == NULL || nSize <= 0 || nSize > FLASH_BLOCK_SIZE)
	{ return FALSE; }//End of if;
	if(Z_param_Save(pBuff,OEM_PARAM_BLOCK,nSize) != nSize)
	{ return FALSE; }

	return TRUE;
}//End of function;

//�洢App�Ĳ�����,���Ϊ0~4,ÿ���СΪ4096�ֽ�
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
