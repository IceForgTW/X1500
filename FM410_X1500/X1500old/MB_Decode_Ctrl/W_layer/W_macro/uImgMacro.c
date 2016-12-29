
/******************************************************************************************
*	include files 
*******************************************************************************************/

#include "uImgMacro.h"
#include <string.h>
/********************************************************************************************
*	local referenced extern function and variables 
********************************************************************************************/
//Macro ��������
#define  MAX_MACRO_BUFF   (64*1024+1024) // Macro Mem
#define  MACRO_MAX_LINK   16    // ������ӷ�����

typedef struct tagINFODATAINDEX
{
	int nBlock;
	int nLens;
	int nOffset;
} InfoDataDex;

typedef  struct  tagMarcoParam
{
	InfoDataDex		InfoDx[MACRO_MAX_LINK]; //����Ϣ����������ȡ��Ϣ�ڻ����е�λ�ã���������
	int				nInfoDx;

	unsigned char   MInfBkUp[MAX_MACRO_BUFF]; // �ֳ�16��,ÿ���ܹ������Ϣ 4096�ֽ�

	unsigned char	MInfBuff[MAX_MACRO_BUFF]; // ��Ϣ�����ڴ�
	int             nMInfo; //��Ϣ���ͳ���

	int				nBCType; // ���������ͱ���,��ֹ���ֶ�ȡQR������ȡPDF���������û�м�ʱ���д���
	int				nCurSeq; // Ŀǰ�Ѿ����͵�����λ��

	int             nError;      //��������
	int             nStatus;     //״̬
	int             nUnMaching;  //��ƥ��״̬ͳ��
}UTMacroCtrl;
/*********************************************************************************************
*	global variables
*********************************************************************************************/
// ����ȷ���Ĳ���
typedef struct tagMACROID
{
	int				nTotal;
	int				nOrder;
	int				nIDLen;
	int				nFileID[32];
}TMacroID;

TMacroID	  g_MID,g_MIDTmp;

UTMacroCtrl   g_MCtrl;

int           g_MacroSet;

BOOL		  g_bMacroInited = FALSE;	//�괦���Ƿ��о�����ʼ��

BOOL		  g_bMacroNeedInited = FALSE;

/*********************************************************************************************
*	global variables
*********************************************************************************************/
#define _MACRO_INIT_() if(g_bMacroInited != TRUE){W_macroInitialParam();}
/********************************************************************************************
*	local referenced macro defines & type defines
********************************************************************************************/

/*********************************************************************************************
*	local function prototype
*********************************************************************************************/

BOOL isCheckMacro(void);

int  iDealMacro(void);
//�ͻ����Ӻõ���Ϣ
void SendSequenceInfo(void);

int  iMacroSeqBuff(int nOrder,int nTotal);

void SetMacroStatus(int nStatus);

void vMacroSetUnMachCnt(void);


/*********************************************************************************************
*	Implementation functions 
*********************************************************************************************/

void W_macroSetParam(int nMacro)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	if (nMacro >= 0 && nMacro <= MACRO_LINK_16)
		g_MacroSet = nMacro;
	else 
		g_MacroSet = MACRO_DIRECT;
}

int W_macroGetParam(void)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	return g_MacroSet;
}

void vMacroSetUnMachCnt(void)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	if ((g_MCtrl.nUnMaching ++) > 3){//	//4�����㣬��5�β��ܳɹ�������ȷ����
		W_macroSetFlag(-1);
	}
	SetMacroStatus(U_uIMG_STATUS_MACRO_ERROR);
}

int W_macroMain(void)
{
	static  int  nstaErrMacro = 0;
	int          nResult = U_NO_MACRO;
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	memcpy(g_MCtrl.MInfBuff,X_decoGetDecInfoBuff(),X_decoGetDecInfoLen());
	g_MCtrl.nMInfo = X_decoGetDecInfoLen();

//��ͨ��ά���룬������һ�ζ�ȡ�������Ǻ�����
	if (isCheckMacro()) //�Ǻ�
	{
		if (W_macroGetFlag() == -1){//����ʹ�ú꣬���г�ʼ��
			W_macroSetFlag (X_decoGetCodeType());
		}
		else if (W_macroGetFlag() != X_decoGetCodeType()){ //���Ͳ�ƥ�䲻����
			vMacroSetUnMachCnt();
			return U_MACRO_ID_UNCONFORMITY;
		}
		nstaErrMacro = 0;
		//��������ֱ�Ӵ�����Ϣ
		if (W_macroGetParam() == MACRO_DIRECT)
		{ return U_NO_MACRO; }//ֱ�ӷ��ͣ�������ͨ������д���

		switch(W_macroGetFlag()){
		case DC_PDF417:
		case DC_QRCODE:
		case DC_DataMatrix:
			nResult = iDealMacro();
			break;
		default:
			return U_NO_MACRO;
			break;
		}		
		//����nResult�Ĳ�ͬ����ֵ���зֱ���:�趨��ǰ�ķ���ֵ�ʹ���״̬
		return nResult;
	}
	else// if(!isCheckMacro())//��ǰ����--���Ǻ�
	{
		if (W_macroGetFlag() != -1) //֮ǰ�Ǻ�
		{
			vMacroSetUnMachCnt();
			return U_MACRO_TYPE_UNCONFORMITY;
		}
	}
	return U_NO_MACRO; //���Ǻ����룬û������
}

//�Ƿ���Ϻ�����
// TRUE ---  ��������
// FALSE --- ����ֳ�ͻ
BOOL W_macroGetUnmacthParam(void)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	// ����������Ǻ�/֮ǰ�Ǻ꣬
	if (X_decoGetBarCode() == UT_TYPE_2D         //������ƥ��
		&& W_macroGetFlag() == X_decoGetCodeType()  // ��������ƥ��
		&& isCheckMacro() )  // ȷ���Ǻ�
	{
		return TRUE; //����ƥ�䣬����
	}
	//���Ͳ�ƥ�䲻����
	vMacroSetUnMachCnt();
	return FALSE;
	
}

void  W_macroSetError(int nError)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	switch(nError){
	case U_MACRO_ID_UNCONFORMITY:
	case U_MACRO_TYPE_UNCONFORMITY:	
	case U_MACRO_SUPERPOSITION:
	case U_MACRO_OVERFLOW:	
		SetMacroStatus(U_uIMG_STATUS_MACRO_ERROR);
		break;
	case U_MACRO_END:	
		SetMacroStatus(U_uIMG_STATUS_MACRO_END);
		break;
	case U_MACRO_SEQ_SEND:
	case U_MACRO_BUFF:
		SetMacroStatus(U_uIMG_STATUS_MACRO_SUC);
		break;
	}

	g_MCtrl.nError = nError;
}

int W_macroGetBuffLen(void)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	return g_MCtrl.nMInfo;
}

unsigned char* W_macroGetBuff(void)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	return g_MCtrl.MInfBuff;
}

void SetMacroStatus(int nStatus)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	g_MCtrl.nStatus = nStatus;
}

int W_macroGetStatus(void)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	return g_MCtrl.nStatus;
}

int W_macroGetError(void)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	return g_MCtrl.nError;
}


void W_macroSetFlag(int nFlag)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	if (nFlag == -1)
		g_MCtrl.nUnMaching = 0;
	g_MCtrl.nBCType = nFlag;
}

int W_macroGetFlag(void)
{
	_MACRO_INIT_() //�жϺ��Ƿ�����Ҫ�Զ���֮��ʼ��
	return g_MCtrl.nBCType;
}

void W_macroSetInitFlag(void)
{
	g_bMacroNeedInited = TRUE;
}

BOOL W_macroGetInitFlag(void)
{
	return g_bMacroNeedInited;
}

//��ʼ������
void W_macroInitialParam(void)
{
	memset(&g_MID,0,sizeof(TMacroID));
	memset(&g_MIDTmp,0,sizeof(TMacroID));
	memset(&g_MCtrl,0,sizeof(UTMacroCtrl));
	g_MCtrl.nBCType = -1;

	g_bMacroInited = TRUE;

	g_bMacroNeedInited = FALSE;
}

BOOL isCheckMacro(void)
{
	int i;
	
	if(X_decoIsMacroCode())
	{
		g_MIDTmp.nTotal = X_decoGetMacroTotalCnt();
		g_MIDTmp.nOrder = X_decoGetMacroOrderFrom();
		g_MIDTmp.nIDLen = X_decoGetMacroIDLen();
		for (i = 0; i < g_MIDTmp.nIDLen; i++)
		{
			g_MIDTmp.nFileID[i] = X_decoGetMacroID(i);
		}//End of for;)

		return TRUE;
	}

	return FALSE;
}

int iDealMacro(void)
{
	int i;

	//������Ϣzyb���
/********************************************/
//	unsigned char buf[512];
//buf[0] = (g_MCtrl.nInfoDx>>8)&0xff;
//buf[1] = g_MCtrl.nInfoDx&0xff;
/*	buf[0] = (g_MIDTmp.nTotal >> 8) & 0xff;
	buf[1] = g_MIDTmp.nTotal & 0xff;
	buf[2] = (g_MIDTmp.nOrder>> 8) & 0xff;
	buf[3] = g_MIDTmp.nOrder& 0xff;
	buf[4] = (g_MIDTmp.nIDLen>> 8) & 0xff;
	buf[5] = g_MIDTmp.nIDLen& 0xff;*/
	//Y_commWrite(g_MCtrl.MInfBuff, g_MCtrl.nMInfo);	//������ϢZYB���

//Y_commWrite(buf, 2);
/***************************************************/

	if (g_MCtrl.nInfoDx == 0)
	{
		g_MID = g_MIDTmp;
	}
	else
	{ 
		//�ж��Ƿ�ͬһ��������ṹ
//������Ϣzyb���
///********************************************/
	//	buf[2] = (W_macroGetFlag()>>8)&0xff;
	//	buf[3] = W_macroGetFlag()&0xff;
	//	Y_commWrite(buf+2, 2);
/********************************************/
		if(W_macroGetFlag() == DC_PDF417)
		{
			if (g_MIDTmp.nTotal != 0)
			{
				g_MID.nTotal = g_MIDTmp.nTotal;
			}
		}
		else
		{
			if(g_MID.nTotal != g_MIDTmp.nTotal)
			{ return U_MACRO_ID_UNCONFORMITY; }
		}

		for (i = 0; i < g_MID.nIDLen; i++)
		{
			if(g_MID.nFileID[i] != g_MIDTmp.nFileID[i])
			{ return U_MACRO_ID_UNCONFORMITY; }
		}//End of for;

		for (i = 0;i < g_MCtrl.nInfoDx;i ++)
		{
			if (g_MCtrl.InfoDx[i].nBlock == g_MIDTmp.nOrder){
				return U_MACRO_SUPERPOSITION;
			}
		}//End of for;
	}//End of else;

///������Ϣzyb���
/************************************/
/*	buf[6] = (g_MID.nTotal >> 8) & 0xff;
	buf[7] = g_MID.nTotal & 0xff;
	buf[8] = (g_MID.nOrder>> 8) & 0xff;
	buf[9] = g_MID.nOrder& 0xff;
	buf[10] = (g_MID.nIDLen>> 8) & 0xff;
	buf[11] = g_MID.nIDLen& 0xff;
Y_commWrite(buf, 12);*/
/************************************/

	
	//�ܳ��ȳ�������ķ���������,�Զ�תΪ����˳����
	if (g_MIDTmp.nOrder > MACRO_MAX_LINK ||  W_macroGetParam() == MACRO_SEQ_BUFF)
	{
	      //Y_commWrite(buf, 1);
		return iMacroSeqBuff(g_MIDTmp.nOrder,g_MID.nTotal);
	}//End of if;
	//Y_commWrite(buf+1, 1);
	//��¼��ǰ��
	g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nBlock = g_MIDTmp.nOrder;
	if (g_MCtrl.nInfoDx != 0 && g_MCtrl.nInfoDx < MACRO_MAX_LINK)
	{
		g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nOffset = 
			g_MCtrl.InfoDx[g_MCtrl.nInfoDx-1].nOffset + g_MCtrl.InfoDx[g_MCtrl.nInfoDx-1].nLens;
		g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nLens = g_MCtrl.nMInfo;
	}//End of if;
	else 
	{
		g_MCtrl.InfoDx[0].nLens = g_MCtrl.nMInfo;
		g_MCtrl.InfoDx[0].nOffset = 0;
	}//End of else;
	//�ݴ�Ŀǰ��Ϣ
	memcpy((char*)(g_MCtrl.MInfBkUp+g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nOffset),  
		   (char*)g_MCtrl.MInfBuff, g_MCtrl.nMInfo);




	g_MCtrl.nInfoDx ++;
	//������Ϣzyb���
/*********************************************/
	//Y_commWrite(g_MCtrl.MInfBuff, g_MCtrl.nMInfo);
	//Y_commWrite(g_MCtrl.MInfBkUp+g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nOffset, g_MCtrl.nMInfo);
/*	memset(buf, 0, sizeof(buf));
	for (i=0; i<g_MCtrl.nInfoDx; ++i)
	{	sprintf((char*)buf+(strlen((char*)(buf))), "%d,",g_MCtrl.InfoDx[i].nBlock);	}	///���Ϳ���Ƿ��

	Y_commWrite((unsigned char*)buf, strlen((char*)buf));*/


/*********************************************/

	if (g_MCtrl.nInfoDx == g_MID.nTotal)	//��ȡ�����һ��������
	{
		SendSequenceInfo();
		return U_MACRO_END;
	}


	return U_MACRO_BUFF;
}

//�ͻ����Ӻõ���Ϣ
void SendSequenceInfo(void) 
{
	int i,j;

	g_MCtrl.nMInfo = 0;

	//�ҵ�������С�ı�־(��Ҫ�Ƕ�Macro PDF417)
	for (i = 0;i < g_MCtrl.nInfoDx;i ++)
	{
		for (j = 0;j < g_MCtrl.nInfoDx;j ++)
		{
			if (i != g_MCtrl.InfoDx[j].nBlock) continue;
			memcpy((char*)(g_MCtrl.MInfBuff+g_MCtrl.nMInfo),
				   (char*)(g_MCtrl.MInfBkUp+g_MCtrl.InfoDx[j].nOffset),
				   g_MCtrl.InfoDx[j].nLens);
				//������Ϣzyb���
/*********************************************/
	//Y_commWrite(g_MCtrl.MInfBuff, g_MCtrl.nMInfo);
	//Y_commWrite(g_MCtrl.MInfBkUp+g_MCtrl.InfoDx[j].nOffset, g_MCtrl.InfoDx[j].nLens);

/*********************************************/
			g_MCtrl.nMInfo += g_MCtrl.InfoDx[j].nLens;
		}
	}
	//��Բ�ͬ������,������Ҫ����У����֤
	if (X_decoGetCodeType() == DC_PDF417){}
	else if (X_decoGetCodeType() == DC_QRCODE){}
	else if (X_decoGetCodeType() == DC_DataMatrix){}

	return ;
}


// ����˳���ͽ�������Ϣ
// �������˳����ʱ���棬ֱ���ֵ�Ϊֹ
// ����Ŀǰ���泬��64k
int iMacroSeqBuff(int nOrder,int nTotal)
{
	int i,j,k,offsetLen;
//	unsigned char buf[10];
	if (nOrder == g_MCtrl.nCurSeq) {//�������˳��,Ŀǰ�÷��ʹ���Ϣ

//		buf[0] = (nOrder<<8)&0xff;
//		buf[1] = (nOrder&0XFF);
//		Y_commWrite( buf,2);
		
		g_MCtrl.nCurSeq ++;
		//�����ڵ�ǰ�Ļ�����Ѱ�ҿ������ӵ���Ϣ
		for (i = 0;i < g_MCtrl.nInfoDx;i ++)
		{
			if (g_MCtrl.InfoDx[i].nBlock != g_MCtrl.nCurSeq) continue;

			memcpy(g_MCtrl.MInfBuff+g_MCtrl.nMInfo,
				   g_MCtrl.MInfBkUp + g_MCtrl.InfoDx[i].nOffset,
				   g_MCtrl.InfoDx[i].nLens);
			/*memcpy(g_MCtrl.MInfBuff+g_MCtrl.nMInfo,
				   g_MCtrl.MInfBkUp,
				   g_MCtrl.InfoDx[i].nLens);*/

			g_MCtrl.nMInfo += g_MCtrl.InfoDx[i].nLens;

			g_MCtrl.nInfoDx --;  //��һ��������Ϣ

			g_MCtrl.nCurSeq ++;  //��ǰ��һ��


			offsetLen = g_MCtrl.InfoDx[i] .nLens;///////zyb���
			if(i != g_MCtrl.nInfoDx)	//�����ӵ��Ŀ��ڻ�������λ��ʱ���������ơ�zyb���
			{
				for (j = i;j < g_MCtrl.nInfoDx; j ++) 	//�����Ѿ����ӵ��Ŀ�ĳ���
				{
					for (k = 0; k < g_MCtrl.InfoDx[j+1].nLens; k ++)//for (k = 0; k < g_MCtrl.InfoDx[j].nLens; k ++)
						g_MCtrl.MInfBkUp[g_MCtrl.InfoDx[j].nOffset + k] = 
							g_MCtrl.MInfBkUp[g_MCtrl.InfoDx[j+1].nOffset+k];
					
					g_MCtrl.InfoDx[j] = g_MCtrl.InfoDx[j+1];	
				}

/**************************************************/
//zyb��ӣ������е�����������ʱ���������ڻ����е�ƫ����ҲҪ�޸�
				j--;
				for(;j>0;j--)
				{
					g_MCtrl.InfoDx[j].nOffset -= offsetLen; //����һ���ƫ����Ӧ�ü��������ӵ��ĳ���		
				}
				if(g_MCtrl.InfoDx[j].nOffset != 0)	//��һ��ûƫ��ʱ�����뽫ƫ������0
					g_MCtrl.InfoDx[j].nOffset -= offsetLen; //j=0;
/***************************************************/
			}
			
			i = -1;
		}

		if (g_MCtrl.nCurSeq == nTotal)	return U_MACRO_END;

		return U_MACRO_SEQ_SEND;
	}
	else if (nOrder > g_MCtrl.nCurSeq){ // ��ǰ��˳�򳬳��˸����ӵĲ���
		if (g_MCtrl.nInfoDx < MACRO_MAX_LINK)
		{
			if (g_MCtrl.nInfoDx != 0){
				g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nOffset = 
						g_MCtrl.InfoDx[g_MCtrl.nInfoDx-1].nOffset + g_MCtrl.InfoDx[g_MCtrl.nInfoDx-1].nLens;
				g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nLens = g_MCtrl.nMInfo;
			}
			else{
				g_MCtrl.InfoDx[0].nLens = g_MCtrl.nMInfo;
				g_MCtrl.InfoDx[0].nOffset = 0;
			}
			if (g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nLens + g_MCtrl.nMInfo < MAX_MACRO_BUFF)//�����㹻
			{ 
				memcpy((char*)(g_MCtrl.MInfBkUp+g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nOffset), 
						(char*)g_MCtrl.MInfBuff,g_MCtrl.nMInfo);
				g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nBlock = nOrder;
				g_MCtrl.nInfoDx ++;
				return U_MACRO_BUFF;
			}
			else
				return U_MACRO_OVERFLOW; // ���治��
		}
		else //if (g_MCtrl.nInfoDx >= MACRO_MAX_LINK) //
		{
			return U_MACRO_OVERFLOW;
		}
	}
	else{// if (nOrder < g_MCtrl.nCurSeq){ 
		return U_MACRO_SUPERPOSITION; //�Ѿ�������Ϣ��
	}
}

