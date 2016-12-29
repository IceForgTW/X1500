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

#include "Y_commMethod.h"
#include <string.h>

/************************************************************************/
/* �궨��                                                               */
/************************************************************************/
//�������������Ķ���
#define BT_OP_RESET		0xC1	//��λ�豸
#define BT_OP_UNCHK		0xC2	//��Ϊ��У������
#define BT_OP_CHKPSW	0xC3	//��ΪУ������
#define BT_OP_DEFID		0xC4	//�����豸ID
#define BT_OP_DEFPSW	0xC5	//��������
/************************************************************************/
/* ���Ͷ���                                                             */
/************************************************************************/
typedef struct zspBTOPParam
{
	unsigned int unOperation;
	unsigned int wParam;
	int			 iParam;
}BTOPParam; //���������Ľṹ��

typedef struct tagCOMMFIFO{
	BYTE *m_pBuffHead;			//����ռ�ͷ
	BYTE *m_pBuffTail;			//����ռ�β
	unsigned int m_unBuffCnt;	//�����С
	BYTE *m_pReadPoint;			//��λ�õ�ָ��
	BYTE *m_pWritePoint;		//дλ�õ�ָ��
	unsigned int m_unDataCnt;	//�����е�������
}FIFOParam; //Fifo�Ĳ���


/************************************************************************/
/* ȫ�ֱ�־��                                                           */
/************************************************************************/
#define FIFOSIZE	(4*1024)
BYTE	g_fifoCOMM[FIFOSIZE];	//����ϵͳʹ��
BYTE	g_fifoBAK[FIFOSIZE];	//����ָ����ʹ��
FIFOParam g_sCOMMParam = {
	g_fifoCOMM,
	g_fifoCOMM+FIFOSIZE,
	FIFOSIZE,
	g_fifoCOMM,
	g_fifoCOMM,
	0
};
FIFOParam g_sBAKParam = {
	g_fifoBAK,
	g_fifoBAK+FIFOSIZE,
	FIFOSIZE,
	g_fifoBAK,
	g_fifoBAK,
	0
};
int g_commCurType		= 0;	//��ǰ�򿪵�����
BOOL g_commEnable		= FALSE;	//�Ƿ��ܽ���ͨѶ
int g_commRate			= 9600;	//���ڲ�����
int g_commCountry		= 0;	//KBW���Թ���
BOOL g_commRS232Opened	= FALSE; //RS232 �Ƿ��Ѿ���
BOOL g_commUSBOpened	= FALSE; //USB �Ƿ��Ѿ���
BOOL g_commBTOpened		= FALSE; //BlueTooth �Ƿ��Ѿ���
BOOL g_commPS2Opened	= FALSE; //PS2 �Ƿ��Ѿ���
BOOL g_commHIDPOSOpened	= FALSE; //HID-POS�Ƿ��Ѿ���
BOOL g_commSurePOSTTOpened	= FALSE; //IBM SurePOS�Ƿ��Ѿ���
BOOL g_commSurePOSHHOpened	= FALSE; //IBM SurePOS�Ƿ��Ѿ���
/************************************************************************/
/* ��������                                                             */
/************************************************************************/
unsigned int Y_FIFOWrite(FIFOParam *psParam, BYTE*pMsg, unsigned int unLen);
unsigned int Y_FIFORead(FIFOParam *psParam, BYTE *pMsg, unsigned int unLen);
unsigned int Y_FIFODataCnt(FIFOParam *psParam);	//FIFO�е�������
/************************************************************************/
/* �궨��                                                               */
/************************************************************************/

/************************************************************************/
/*  ����ʵ��                                                            */
/************************************************************************/
int	 Y_commGetCurType(void)
{
	return g_commCurType;
}//End of function;
BOOL Y_commIsEnable(void)
{
	return g_commEnable;
}
BOOL Y_commEnable()
{
	if (g_commEnable == TRUE)
	{ return g_commEnable; }
	if (g_commCurType != 0)
	{
		g_commEnable = TRUE;
		return g_commEnable;
	}//End of if;
	return g_commEnable;
}//End of function;
BOOL Y_commDisable()
{
	g_commEnable = FALSE;
	return TRUE;
}//End of function;

BOOL Y_commChgToRS232(TComParam *psParam)
{
	SCOMMCFG sParam;
	static TComParam sCom;

	if (g_commCurType == COMMT_RS232)
	{
		if (psParam != NULL && psParam->lRate == sCom.lRate
			&& psParam->lDataBits == sCom.lDataBits
			&& psParam->lParity == sCom.lParity
			&& psParam->lStopBits == sCom.lStopBits
			&& psParam->nPort == sCom.nPort
			&& psParam->nflowCtrl == sCom.nflowCtrl)
		{ return TRUE; }
	}//End of if;

	if (psParam != NULL)
	{ sCom = *psParam; }
	else
	{
		sCom.lDataBits = 8;
		sCom.lParity = NOPARITY;
		sCom.lRate = g_commRate;
		sCom.lStopBits = ONESTOPBIT;
		sCom.nPort = 0;
		sCom.nflowCtrl = 0;
		sCom.Reserved = 0;
	}//End of else;
	sParam.lpsDetail = &sCom;
	sParam.nCommType = COMMT_RS232;
	sParam.nToModify = COMM_MDF_ENABLE|COMM_MDF_CHGPARAM;
	if (Z_comm_Config(&sParam) == TRUE)
	{
// 		if (g_commCurType != 0 && g_commCurType != COMMT_RS232)
// 		{//�ر�ԭ��ʹ�õĽӿ�
// 			sParam.nCommType = g_commCurType;
// 			sParam.nToModify = COMM_MDF_DISABLE;
// 			sParam.lpsDetail = NULL;
// 			Z_comm_Config(&sParam);
// 		}//End of if;
		g_commRS232Opened = TRUE;
		g_commCurType = COMMT_RS232;
		g_commRate = sCom.lRate;
		return TRUE; 
	}
	return FALSE;
}//End of function;

BOOL Y_commChgToUSBRS232()
{
	SCOMMCFG sParam;

// 	if (g_commCurType == COMMT_USB_RS232)
// 	{
// 		return TRUE;
// 	}//End of if;

	sParam.lpsDetail = NULL;
	sParam.nCommType = COMMT_USB_RS232;
	sParam.nToModify = COMM_MDF_ENABLE;
	if (Z_comm_Config(&sParam) == TRUE)
	{
// 		if (g_commCurType != 0 && g_commCurType != COMMT_USB_RS232)
// 		{//�ر�ԭ��ʹ�õĽӿ�
// 			sParam.nCommType = g_commCurType;
// 			sParam.nToModify = COMM_MDF_DISABLE;
// 			sParam.lpsDetail = NULL;
// 			Z_comm_Config(&sParam);
// 		}//End of if;
		g_commUSBOpened = TRUE;
		g_commCurType = COMMT_USB_RS232;
		return TRUE; 
	}
	
	return FALSE;
}//End of function;


BOOL Y_commChgToUSBCDC()
{
	SCOMMCFG sParam;

	sParam.lpsDetail = NULL;
	sParam.nCommType = COMMT_USB_CDC;
	sParam.nToModify = COMM_MDF_ENABLE;
	if (Z_comm_Config(&sParam) == TRUE)
	{
		g_commUSBOpened = TRUE;
		g_commCurType = COMMT_USB_CDC;
		return TRUE; 
	}
	
	return FALSE;
}//End of function;


BOOL Y_commChgToUSBDataPipe(void)
{
	SCOMMCFG sParam;

// 	if (g_commCurType == COMMT_USB_Data)
// 	{ return TRUE; }


	sParam.nCommType = COMMT_USB_Data;
	sParam.nToModify = COMM_MDF_ENABLE;
	sParam.lpsDetail = NULL;
	if (Z_comm_Config(&sParam) == TRUE)
	{
// 		if (g_commCurType != 0 && g_commCurType != COMMT_USB_Data)
// 		{
// 			sParam.nCommType = g_commCurType;
// 			sParam.nToModify = COMM_MDF_DISABLE;
// 			sParam.lpsDetail = NULL;
// 			Z_comm_Config(&sParam);
// 		}//End of if;
		g_commUSBOpened = TRUE;
		g_commCurType = COMMT_USB_Data;
		return TRUE;
	}

	return FALSE;
}//End of function;

BOOL Y_commChgToKBW(UTKeyBoard *psParam)
{
	SCOMMCFG sParam;
	static UTHIDKBW sKBW;

	sParam.nToModify = COMM_MDF_ENABLE|COMM_MDF_CHGPARAM;
 	if (g_commCurType == COMMT_KBW)
	{
		if (psParam != NULL 
			&& psParam->nLanguage == sKBW.nLanguage 
			&& psParam->isCapsLock == sKBW.isCapsLock
			&& psParam->isNumericMode == sKBW.isNumericMode
			&& psParam->isCtrlASCIIMode == sKBW.isControlASCIIMode
			&& psParam->isKeysrockeDelay == sKBW.isKeysrockeDelay
			&& psParam->isConvertCase == sKBW.isConvertCase
			&& psParam->isEmulateKeypad == sKBW.isEmulateKeypad
			&& psParam->isUnknownCharactersBeep == sKBW.isUnknownCharactersBeep
			&& psParam->isKeyPad == sKBW.isKeyPad
			&& psParam->isFasttransferMode == sKBW.isFasttransferMode
			&& psParam->nPollRate == sKBW.nPollRate
			&& psParam->nCodePage == sKBW.nCodePage)
		{ sParam.nToModify = COMM_MDF_ENABLE; }
	}

	if (psParam != NULL)
	{
		sKBW.nLanguage				= psParam->nLanguage;
		sKBW.isCapsLock				= psParam->isCapsLock;
		sKBW.isNumericMode			= psParam->isNumericMode;
		sKBW.isControlASCIIMode		= psParam->isCtrlASCIIMode;
		sKBW.isKeysrockeDelay			= psParam->isKeysrockeDelay;
		sKBW.isConvertCase				= psParam->isConvertCase;
		sKBW.isEmulateKeypad			= psParam->isEmulateKeypad;
		sKBW.isUnknownCharactersBeep	= psParam->isUnknownCharactersBeep;
		sKBW.isKeyPad					= psParam->isKeyPad;
		sKBW.isFasttransferMode			= psParam->isFasttransferMode;
		sKBW.nPollRate					= psParam->nPollRate;
		sKBW.nCodePage					= psParam->nCodePage;
	}
	else
	{
		sKBW.nLanguage					= g_commCountry;
		sKBW.isCapsLock					= 1;
		sKBW.isNumericMode				= 0;
		sKBW.isControlASCIIMode			= 0;
		sKBW.isKeysrockeDelay			= 0;
		sKBW.isConvertCase				= 0;
		sKBW.isEmulateKeypad			= 0;
		sKBW.isUnknownCharactersBeep	= 0;
		sKBW.isKeyPad					= 0;
		sKBW.isFasttransferMode			= 0;
		sKBW.nPollRate					= 0;
	}//End of else;

	sParam.nCommType = COMMT_KBW;
	sParam.lpsDetail = &sKBW;
	if (Z_comm_Config(&sParam) == TRUE)
	{
// 		if (g_commCurType != 0 && g_commCurType != COMMT_KBW)
// 		{
// 			sParam.nCommType = g_commCurType;
// 			sParam.nToModify = COMM_MDF_DISABLE;
// 			sParam.lpsDetail = NULL;
// 			Z_comm_Config(&sParam);
// 		}//End of if;
		g_commUSBOpened = TRUE;
		g_commCurType = COMMT_KBW;
		g_commCountry = sKBW.nLanguage;
		return TRUE;
	}

	return FALSE;
}//End of function;

BOOL Y_commChgToBT(void)
{
	SCOMMCFG sParam;

	sParam.nCommType = COMMT_BLUE_TOOTH;
	sParam.nToModify = COMM_MDF_ENABLE;
	sParam.lpsDetail = NULL;
	if (Z_comm_Config(&sParam) == TRUE)
	{
// 		if (g_commCurType != 0 && g_commCurType != COMMT_BLUE_TOOTH)
// 		{
// 			sParam.nCommType = g_commCurType;
// 			sParam.nToModify = COMM_MDF_DISABLE;
// 			sParam.lpsDetail = NULL;
// 			Z_comm_Config(&sParam);
// 		}//End of if;
		g_commBTOpened = TRUE;
		g_commCurType = COMMT_BLUE_TOOTH;
		return TRUE;
	}//End of if;

	return FALSE;
}//End of function;

BOOL Y_commChgToPS2(UTKeyBoard *psParam)
{
	SCOMMCFG sParam;

	sParam.nToModify = COMM_MDF_ENABLE|COMM_MDF_CHGPARAM;

	sParam.nCommType = COMMT_PS2;

	if (Z_comm_Config(&sParam) == TRUE)
	{
		g_commPS2Opened = TRUE;
		g_commCurType = COMMT_PS2;
		return TRUE;
	}

	return FALSE;

}//End of function;

//��ͨѶ�ӿ��л���HID-POS��ʽ
BOOL Y_commChgToHIDPOS(void)
{
	SCOMMCFG sParam;

	sParam.nToModify = COMM_MDF_ENABLE|COMM_MDF_CHGPARAM;
 	if (g_commCurType == COMMT_HID_POS)
	{ sParam.nToModify = COMM_MDF_ENABLE; }

	sParam.nCommType = COMMT_HID_POS;
	sParam.lpsDetail = NULL;
	if (Z_comm_Config(&sParam) == TRUE)
	{
		g_commUSBOpened = TRUE;
		g_commHIDPOSOpened = TRUE;
		g_commCurType = COMMT_HID_POS;
		return TRUE;
	}

	return FALSE;
}//End of function;

//��ͨѶ�ӿ��л���IBM SurePOS(Table-Top)��ʽ
BOOL Y_commChgToSurePOS_TT(void)
{
	SCOMMCFG sParam;
	
	sParam.nToModify = COMM_MDF_ENABLE|COMM_MDF_CHGPARAM;
	if (g_commCurType == COMMT_IBM_SurePOS_TT)
	{ sParam.nToModify = COMM_MDF_ENABLE; }
	
	sParam.nCommType = COMMT_IBM_SurePOS_TT;
	sParam.lpsDetail = NULL;
	if (Z_comm_Config(&sParam) == TRUE)
	{
		g_commUSBOpened = TRUE;
		g_commSurePOSTTOpened = TRUE;
		g_commCurType = COMMT_IBM_SurePOS_TT;
		return TRUE;
	}
	
	return FALSE;
}//End of function;

//��ͨѶ�ӿ��л���IBM SurePOS(Hand-Held)��ʽ
BOOL Y_commChgToSurePOS_HH(void)
{
	SCOMMCFG sParam;
	
	sParam.nToModify = COMM_MDF_ENABLE|COMM_MDF_CHGPARAM;
	if (g_commCurType == COMMT_IBM_SurePOS_HH)
	{ sParam.nToModify = COMM_MDF_ENABLE; }
	
	sParam.nCommType = COMMT_IBM_SurePOS_HH;
	sParam.lpsDetail = NULL;
	if (Z_comm_Config(&sParam) == TRUE)
	{
		g_commUSBOpened = TRUE;
		g_commSurePOSHHOpened = TRUE;
		g_commCurType = COMMT_IBM_SurePOS_HH;
		return TRUE;
	}
	
	return FALSE;
}//End of function;


//��ȡͨѶ�ӿڵĲ�����
int Y_commGetBaudrate(void)
{
	if( g_commCurType == COMMT_RS232)
	{
		return g_commRate;
	}//End of if;
	return 0;
}//End of function;

//��ȡ���̲��ֹ�������
int Y_commGetKBWCountry(void)
{
	if (g_commCurType == COMMT_KBW)
	{
		return g_commCountry;
	}
	return 0;
}//End of function;

//��ȡ����
int Y_commRead(unsigned char * buff, unsigned int size, int msTimeOut)
{
	unsigned int nCount;
	int nSt,nEn;
	BYTE byTmpMsg[1024];
	int nTmpLen;

	if (g_commEnable != TRUE)
	{ return 0; }//End of if;

	nCount = 0;
	nSt = Z_Get10msCounter();
	nEn = nSt;
	do {
		if (Y_FIFODataCnt(&g_sCOMMParam) == 0)
		{
			nTmpLen = Z_comm_Read(byTmpMsg,1024);
			Y_FIFOWrite(&g_sCOMMParam,byTmpMsg,(unsigned int)nTmpLen);
			Y_FIFOWrite(&g_sBAKParam,byTmpMsg,(unsigned int)nTmpLen);
		}//End of if;
		nCount += (int)Y_FIFORead(&g_sCOMMParam,buff+nCount,size-nCount);
		nEn = Z_Get10msCounter();
	} while(nCount < size && (nEn-nSt)*10 < msTimeOut);//End of do-while;

	return nCount;
}//End of function;

//��ȡ����
int Y_commBakRead(unsigned char * buff, unsigned int size, int msTimeOut)
{
	unsigned int nCount;
	int nSt,nEn;
	BYTE byTmpMsg[1024];
	int nTmpLen;

//	if (g_commEnable != TRUE)
//	{ return 0; }//End of if;

	nCount = 0;
	nSt = Z_Get10msCounter();
	nEn = nSt;
	do {
		if (Y_FIFODataCnt(&g_sBAKParam) == 0)
		{
			nTmpLen = Z_comm_Read(byTmpMsg,1024);
			Y_FIFOWrite(&g_sCOMMParam,byTmpMsg,(unsigned int)nTmpLen);
			Y_FIFOWrite(&g_sBAKParam,byTmpMsg,(unsigned int)nTmpLen);
		}//End of if;
		nCount += (int)Y_FIFORead(&g_sBAKParam,buff+nCount,size-nCount);
		nEn = Z_Get10msCounter();
	} while(nCount < size && (nEn-nSt)*10 < msTimeOut);//End of do-while;

	return nCount;
}//End of function;
//��������
int Y_commWrite(unsigned char *buff, unsigned int size)
{
	if (g_commEnable != TRUE)
	{
		return 0; 
	}//End of if;

	return Z_comm_Write(buff,size);
}//End of function;

int Y_commSendString(const char *str)
{
	int lvlen = strlen(str);
	return Y_commWrite((unsigned char*)str, lvlen);
}


//��ս��ջ�����������,��0����ֵΪ������. 1�볬ʱ����
BOOL Y_commClean(void)
{
	unsigned char buff[256];
	int nCount,nLast;
	int nSt,nEn;

	if (g_commEnable != TRUE)
	{ return 0; }//End of if;

	nCount = 0;
	nSt = Z_Get10msCounter();
	nEn = nSt;
	do {
		nLast = Y_commRead(buff,256,0);
		nCount += nLast;
		nEn = Z_Get10msCounter();
	} while(nLast > 0 && (nEn-nSt)*10 < 1000);//End of do-while;

	return (nLast==0);
}//End of function;

int Y_commSendChar(char cChar)
{
	unsigned char buff[4];

	if (g_commEnable != TRUE)
	{ return 0; }//End of if;

	buff[0] = (unsigned char)cChar;
	buff[1] = '\0';

	return Z_comm_Write(buff,1);
}//End of function;
int Y_commReadChar(char *pcChar,int msTimeOut)
{
	unsigned char buff[4];
	int nCount;

	if (g_commEnable != TRUE)
	{ return 0; }//End of if;

	nCount = 0;
	nCount = Y_commRead(buff,1,msTimeOut);
	pcChar[0] = (char)buff[0];

	return nCount;
}//End of function;

int Y_commBakReadChar(char *pcChar,int msTimeOut)
{
	unsigned char buff[4];
	int nCount;

//	if (g_commEnable != TRUE)
//	{ return 0; }//End of if;

	nCount = 0;
	nCount = Y_commBakRead(buff,1,msTimeOut);
	pcChar[0] = (char)buff[0];

	return nCount;
}//End of function;

int Y_commBuffCount(void)
{
	BYTE buff[1024];
	int nLen;

	if (g_commEnable != TRUE)
	{ return 0; }//End of if;
	
	if (Y_FIFODataCnt(&g_sCOMMParam) == 0)
	{//��Щ������
		nLen = Z_comm_Read(buff,1024);
		Y_FIFOWrite(&g_sCOMMParam,buff,(unsigned int)nLen);
		Y_FIFOWrite(&g_sBAKParam,buff,(unsigned int)nLen);
	}//End of if;

	return (int)Y_FIFODataCnt(&g_sCOMMParam);
}//End of function;

int Y_commBakBuffCount(void)
{
	BYTE buff[1024];
	int nLen;

//	if (g_commEnable != TRUE)
//	{ return 0; }//End of if;
	
	if (Y_FIFODataCnt(&g_sBAKParam) == 0)
	{//��Щ������
		nLen = Z_comm_Read(buff,1024);
		Y_FIFOWrite(&g_sCOMMParam,buff,(unsigned int)nLen);
		Y_FIFOWrite(&g_sBAKParam,buff,(unsigned int)nLen);
	}//End of if;

	return (int)Y_FIFODataCnt(&g_sBAKParam);
}//End of function;

//д��
unsigned int Y_FIFOWrite(FIFOParam *psParam, BYTE*pMsg, unsigned int unLen)
{
	unsigned int unCnt,unTmp;

	if (psParam == NULL || pMsg == NULL || unLen == 0)
	{ return 0; }//End of if;
	if (psParam->m_pBuffHead == NULL 
		|| psParam->m_pBuffTail <= psParam->m_pBuffHead
		|| psParam->m_pReadPoint < psParam->m_pBuffHead
		|| psParam->m_pWritePoint < psParam->m_pBuffHead
		|| psParam->m_pReadPoint > psParam->m_pBuffTail
		|| psParam->m_pWritePoint > psParam->m_pBuffTail)
	{ return 0; }//End of if;

	//�ռ䲻��
	if (psParam->m_unBuffCnt < unLen)
	{
		if (unLen >= FIFOSIZE)
		{//ֱ�Ӵ�ͷд��
			memcpy(psParam->m_pBuffHead,pMsg+(unLen-FIFOSIZE),FIFOSIZE);
			psParam->m_pReadPoint = psParam->m_pBuffHead;
			psParam->m_pWritePoint = psParam->m_pBuffTail;
			psParam->m_unDataCnt = FIFOSIZE;
			unCnt = FIFOSIZE;
			return FIFOSIZE;
		}//End of if;
		else
		{//�����Ƕ���Щ����,�򽫾�����������һ����
			unCnt = unLen-psParam->m_unBuffCnt; //��Ҫ������������

			unTmp = (psParam->m_pBuffTail-psParam->m_pReadPoint);
			if (unTmp < unCnt)
			{//����һֱ����β
				unCnt -= unTmp;
				psParam->m_unDataCnt -= unTmp;
				psParam->m_unBuffCnt += unTmp;
				psParam->m_pReadPoint = psParam->m_pBuffHead;//ת��ͷ����ȡ
			}//End of if;

			if (unCnt > 0)
			{//������ַС��д���ַ
				psParam->m_pReadPoint += unCnt;
				psParam->m_unBuffCnt += unCnt;
				psParam->m_unDataCnt -= unCnt;
				unCnt = 0;
			}
		}//End of else;
	}//End of if;
	
	if(psParam->m_unBuffCnt < unLen)
	{ return 0; }
	//ʣ��ռ��㹻
	//��ʼд
	unCnt = (psParam->m_pBuffTail-psParam->m_pWritePoint);
	if (unCnt < unLen)
	{//����һֱд��β
		memcpy(psParam->m_pWritePoint,pMsg,unCnt);
		psParam->m_pWritePoint += unCnt;
		psParam->m_unBuffCnt -= unCnt;
		psParam->m_unDataCnt += unCnt;

		//��������δд��,��ת��ͷ����ʼд
		psParam->m_pWritePoint = psParam->m_pBuffHead;
		memcpy(psParam->m_pWritePoint,pMsg+unCnt,unLen-unCnt);
		unCnt = unLen-unCnt;
		psParam->m_pWritePoint += unCnt;
		psParam->m_unBuffCnt -= unCnt;
		psParam->m_unDataCnt += unCnt;

		return unLen;
	}//End of if;
	else
	{//
		memcpy(psParam->m_pWritePoint,pMsg,unLen);
		psParam->m_pWritePoint += unLen;
		psParam->m_unBuffCnt -= unLen;
		psParam->m_unDataCnt += unLen;

		if (psParam->m_pWritePoint >= psParam->m_pBuffTail)
		{ psParam->m_pWritePoint = psParam->m_pBuffHead; }

		return unLen;
	}//End of else;

	return 0;
}//End of function;
//����
unsigned int Y_FIFORead(FIFOParam *psParam, BYTE *pMsg, unsigned int unLen)
{
	unsigned int unCnt;
	
	if (psParam == NULL || pMsg == NULL || unLen == 0)
	{ return 0; }//End of if;
	if (psParam->m_pBuffHead == NULL 
		|| psParam->m_pBuffTail <= psParam->m_pBuffHead
		|| psParam->m_pReadPoint < psParam->m_pBuffHead
		|| psParam->m_pWritePoint < psParam->m_pBuffHead
		|| psParam->m_pReadPoint > psParam->m_pBuffTail
		|| psParam->m_pWritePoint > psParam->m_pBuffTail
		)
	{ return 0; }//End of if;
	
	//û������
	if (psParam->m_unDataCnt == 0)
	{ return 0; }

	//��ʼ��
	if(psParam->m_unDataCnt < unLen)
	{ unLen = psParam->m_unDataCnt; }
	
	unCnt = (psParam->m_pBuffTail-psParam->m_pReadPoint);
	if (unCnt < unLen )
	{//����һֱ����β
		if (unCnt > 0)
		{
			memcpy(pMsg,psParam->m_pReadPoint,unCnt);
			psParam->m_pReadPoint += unCnt;
			psParam->m_unBuffCnt += unCnt;
			psParam->m_unDataCnt -= unCnt;
		}
		if(psParam->m_pReadPoint >= psParam->m_pBuffTail)
		{ psParam->m_pReadPoint = psParam->m_pBuffHead; } //����β������ת��ͷ��
		if (unLen > unCnt)
		{//��������δ����,��ת��ͷ����ʼ��
			psParam->m_pReadPoint = psParam->m_pBuffHead;
			memcpy(pMsg+unCnt,psParam->m_pReadPoint,unLen-unCnt);
			unCnt = unLen-unCnt;
			psParam->m_pReadPoint += unCnt;
			psParam->m_unBuffCnt += unCnt;
			psParam->m_unDataCnt -= unCnt;
		}//End of if;
		return unLen;
	}//End of if;
	else
	{
		memcpy(pMsg,psParam->m_pReadPoint,unLen);
		psParam->m_pReadPoint += unLen;
		psParam->m_unBuffCnt += unLen;
		psParam->m_unDataCnt -= unLen;
		if(psParam->m_pReadPoint >= psParam->m_pBuffTail)
		{ psParam->m_pReadPoint = psParam->m_pBuffHead; } //����β������ת��ͷ��
		if (psParam->m_pWritePoint >= psParam->m_pBuffTail)
		{ psParam->m_pWritePoint = psParam->m_pBuffHead; }
		return unLen;
	}

	return 0;
}//End of function;

unsigned int Y_FIFODataCnt(FIFOParam *psParam)
{
	if (psParam == NULL)
	{ return 0; }//End of if;
	if (psParam->m_pBuffHead == NULL 
		|| psParam->m_pBuffTail <= psParam->m_pBuffHead
		|| psParam->m_pReadPoint < psParam->m_pBuffHead
		|| psParam->m_pWritePoint < psParam->m_pBuffHead
		|| psParam->m_pReadPoint >= psParam->m_pBuffTail
		|| psParam->m_pWritePoint >= psParam->m_pBuffTail
		)
	{ return 0; }//End of if;

	return psParam->m_unDataCnt;
}//End of function;

//����slaveģʽ�µ�ƥ������
BOOL BlueT_set_slave_pin(char *pin, int nlen)
{
	BTOPParam sParam;
	SCOMMCFG sComm;
// 	if (g_commCurType != COMMT_BLUE_TOOTH)
// 	{ return FALSE; }

	sParam.unOperation = BT_OP_DEFPSW;//��������
	sParam.wParam = (unsigned int)pin;
	sParam.iParam = nlen;
	sComm.nCommType = COMMT_BLUE_TOOTH;
	sComm.nToModify = COMM_MDF_CHGPARAM;
	sComm.lpsDetail = &sParam;
	if (Z_comm_Config(&sComm))
	{
//		Z_PlayMusic("|7.4|3.4|7.4|3.4|");
		return TRUE;
	}

//	Z_Beep(1000,1000,5);
	return FALSE;
}//End of function;

//�����Ƿ��������ƥ��
void BlueT_set_auth_status(BOOL nstat)
{
	BTOPParam sParam;
	SCOMMCFG sComm;
// 	if (g_commCurType != COMMT_BLUE_TOOTH)
// 	{ return FALSE; }
	if (nstat)
	{
		sParam.unOperation = BT_OP_CHKPSW;//����Ϊ��У������
	}//End of if;
	else
	{
		sParam.unOperation = BT_OP_UNCHK;//����Ϊ��У������
	}//End of else;
	sParam.wParam = 0;
	sParam.iParam = 0;
	sComm.nCommType = COMMT_BLUE_TOOTH;
	sComm.nToModify = COMM_MDF_CHGPARAM;
	sComm.lpsDetail = &sParam;
	if (Z_comm_Config(&sComm))
	{
//		Z_PlayMusic("|7.4|3.4|7.4|3.4|");
		return ;
	}

//	Z_Beep(1000,1000,5);
}//End of function;

//��λ���е�����
void BlueT_reset_cfg(void)
{
	BTOPParam sParam;
	SCOMMCFG sComm;
// 	if (g_commCurType != COMMT_BLUE_TOOTH)
// 	{ return FALSE; }

	sParam.unOperation = BT_OP_RESET;//����
	sParam.wParam = 0;
	sParam.iParam = 0;
	sComm.nCommType = COMMT_BLUE_TOOTH;
	sComm.nToModify = COMM_MDF_CHGPARAM;
	sComm.lpsDetail = &sParam;
	if (Z_comm_Config(&sComm))
	{
//		Z_PlayMusic("|7.4|3.4|7.4|3.4|");
		return ;
	}

//	Z_Beep(1000,1000,5);

}//End of function;

//�����豸���� 
BOOL BlueT_set_dev_name(char *devname, int nlen)
{
	BTOPParam sParam;
	SCOMMCFG sComm;
// 	if (g_commCurType != COMMT_BLUE_TOOTH)
// 	{ return FALSE; }

	sParam.unOperation = BT_OP_DEFID;//����ID
	sParam.wParam = (unsigned int)devname;
	sParam.iParam = nlen;
	sComm.nCommType = COMMT_BLUE_TOOTH;
	sComm.nToModify = COMM_MDF_CHGPARAM;
	sComm.lpsDetail = &sParam;
	if (Z_comm_Config(&sComm))
	{
//		Z_PlayMusic("|7.4|3.4|7.4|3.4|");
		return TRUE;
	}

//	Z_Beep(1000,1000,5);
	return FALSE;
}//End of function;

BOOL Y_commIsBTOpened(void)
{
	return g_commBTOpened;
}//End of function;

BOOL Y_commIsRS232Opened(void)
{
	return g_commRS232Opened;
}//End of function;

BOOL Y_commIsUSBOpened(void)
{
	return g_commUSBOpened;
}//End of function;

BOOL Y_commIsPS2Opened(void)
{
	return g_commPS2Opened;
}//End of function;

BOOL Y_commIsHIDPOSOpened(void)
{
	return g_commHIDPOSOpened;
}//End of function;

BOOL Y_commIsSurePOSTTOpened(void)
{
	return g_commSurePOSTTOpened;
}//End of function;

BOOL Y_commIsSurePOSHHOpened(void)
{
	return g_commSurePOSHHOpened;
}//End of function;

void Y_commCloseBT(void)
{
	SCOMMCFG sParam;
	
	sParam.nCommType = COMMT_BLUE_TOOTH;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);

	g_commBTOpened = FALSE;
}//End of function;

void Y_commCloseUSB(void)
{
	SCOMMCFG sParam;
	
	sParam.nCommType = COMMT_USB_Data;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);
	sParam.nCommType = COMMT_USB_RS232;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);
	sParam.nCommType = COMMT_KBW;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);
	sParam.nCommType = COMMT_HID_POS;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);
	sParam.nCommType = COMMT_IBM_SurePOS_TT;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);
	sParam.nCommType = COMMT_IBM_SurePOS_HH;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);
	
	sParam.nCommType = COMMT_USB_CDC;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);

	g_commUSBOpened = FALSE;
}//End of function;

void Y_commCloseRS232(void)
{
	SCOMMCFG sParam;
	
	sParam.nCommType = COMMT_RS232;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);

	g_commRS232Opened = FALSE;
}//End of function;

void Y_commClosePS2(void)
{
	SCOMMCFG sParam;
	
	sParam.nCommType = COMMT_PS2;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);

	g_commPS2Opened = FALSE;
}//End of function;

void Y_commCloseHIDPOS(void)
{
	SCOMMCFG sParam;
	
	sParam.nCommType = COMMT_HID_POS;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);

	g_commHIDPOSOpened = FALSE;
}//End of function;

void Y_commCloseSurePOS_TT(void)
{
	SCOMMCFG sParam;
	
	sParam.nCommType = COMMT_IBM_SurePOS_TT;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);
	
	g_commSurePOSTTOpened = FALSE;
}//End of function;

void Y_commCloseSurePOS_HH(void)
{
	SCOMMCFG sParam;
	
	sParam.nCommType = COMMT_IBM_SurePOS_HH;
	sParam.nToModify = COMM_MDF_DISABLE;
	sParam.lpsDetail = NULL;
	Z_comm_Config(&sParam);
	
	g_commSurePOSHHOpened = FALSE;
}//End of function;
