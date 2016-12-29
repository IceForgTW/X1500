
/******************************************************************************************
*	include files 
*******************************************************************************************/

#include "SysInfoEdit.h"
#include "../../X_layer/X_deco/uImgProcCtrl.h"


#include <string.h>
#include <stdio.h>
/************************************************************************/
/*                                                                      */
/************************************************************************/

//添加前缀，并处理前缀顺序
BOOL AddPrefix();
//添加AIM前缀
BOOL AddAIM();
//添加CodeID前缀
BOOL AddCodeID();
//添加自定义前缀
BOOL AddSelfPrefix();

//添加自定义后缀
BOOL AddSelfSuffix();
//添加Terminal结束符后缀,至多只允许2个字符
BOOL AddTerminal();

BOOL DataCut();
BOOL OneCut(unsigned char *MesData, unsigned char *p, int Flag, unsigned int Begin, unsigned int End, int pLength, int *nLength);
/************************************************************************/
/*                                                                      */
/************************************************************************/

const unsigned char AIMTab1D[32 * 2] = {
	'X','0','X','0','C','0','C','0','E','0','E','0','E','0','E','0',
	'I','0','I','0','I','0','X','0','X','0','A','0','A','0','F','0',
    'X','0','G','0','X','0','X','0','X','0','X','0','X','0','X','0',
	'X','0','X','0','X','0','X','0','X','0','X','0','X','0','X','0'};
const unsigned char AIMTab2D[32 * 2] = {
    'L','0','Q','0','z','0','d','0','U','0','V','0','X','0','X','0',
	'X','0','X','0','X','0','X','0','X','0','X','0','X','0','X','0',
    'X','0','X','0','X','0','X','0','X','0','X','0','X','0','X','0',
	'X','0','X','0','X','0','X','0','X','0','X','0','X','0','X','0'};
const unsigned char AIMOCR[32 * 2] = {
    'X','0','X','0','X','0','X','0','X','0','X','0','X','0','X','0',
	'X','0','X','0','X','0','X','0','X','0','X','0','X','0','X','0',
    'X','0','X','0','X','0','X','0','X','0','X','0','X','0','X','0',
	'X','0','X','0','X','0','X','0','X','0','X','0','X','0','X','0'};

//信息编辑的最大前/后 扩展数据长度
#define __INFEDIT_EXTLEN_MAX__		128
BYTE g_InfEdPrefix[__INFEDIT_EXTLEN_MAX__];
BYTE g_InfEdSuffix[__INFEDIT_EXTLEN_MAX__];
int	 g_nPrefixExtLen = 0;	//前扩展长度
int  g_nSuffixExtLen = 0;	//后扩展长度
WInfoParam g_infoParam;		//参数
/************************************************************************/
/*                                                                      */
/************************************************************************/

//每次信息编辑前,都要重新初始化
void	W_infoInitEveryTime(void)
{
	g_nPrefixExtLen = 0;
	g_nSuffixExtLen = 0;
	g_InfEdPrefix[0] = '\0';
	g_InfEdSuffix[0] = '\0';
	g_infoParam.nMsgLen = 0;
	g_infoParam.pbyMsgBuff = NULL;
}//End of function;


//添加前缀，并处理前缀顺序
BOOL AddPrefix()
{
	switch(X_paramGetGlobalAllParam()->pCode->PrefixOrder){
	case 0: //CodeID+AIM+自定义前缀
		if(!AddSelfPrefix()) return FALSE;//添加自定义前缀
	    if(!AddAIM()) return FALSE;//添加AIM前缀
        if(!AddCodeID()) return FALSE;//添加CodeID前缀
		break;
	case 1: //CodeID+自定义前缀+AIM
		if(!AddAIM()) return FALSE;//添加AIM前缀
		if(!AddSelfPrefix()) return FALSE;//添加自定义前缀
        if(!AddCodeID()) return FALSE;//添加CodeID前缀
		break;
	case 2: //AIM+CodeID+自定义前缀
		if(!AddSelfPrefix()) return FALSE;//添加自定义前缀 
        if(!AddCodeID()) return FALSE;//添加CodeID前缀
		if(!AddAIM()) return FALSE;//添加AIM前缀
		break;
	case 3: //AIM+自定义前缀+CodeID
		if(!AddCodeID()) return FALSE;//添加CodeID前缀
		if(!AddSelfPrefix()) return FALSE;//添加自定义前缀
		if(!AddAIM()) return FALSE;//添加AIM前缀
		break;
	case 4: //自定义前缀+CodeID+AIM
		if(!AddAIM()) return FALSE;//添加AIM前缀
        if(!AddCodeID()) return FALSE;//添加CodeID前缀
		if(!AddSelfPrefix()) return FALSE;//添加自定义前缀
		break;
	case 5: //自定义前缀+AIM+CodeID
		if(!AddCodeID()) return FALSE;//添加CodeID前缀
		if(!AddAIM()) return FALSE;//添加AIM前缀
		if(!AddSelfPrefix()) return FALSE;//添加自定义前缀
		break;
	default : return FALSE;
	}
	
	return TRUE;
}//End of function;
//添加AIM前缀
BOOL AddAIM()
{
	int i;
	
	if (g_infoParam.pbyMsgBuff == NULL || g_infoParam.nMsgLen <= 0)
	{ return FALSE; }//End of if;

	if(!(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] & 0x01)) return TRUE; //不添加AIMID前缀
	
	//添加3个AIM前缀
	for (i = g_nPrefixExtLen - 1; i >= 0; i--)
	{ g_InfEdPrefix[i + 3] = g_InfEdPrefix[i]; }
	X_decoGetAIMID((char*)g_InfEdPrefix);
	g_nPrefixExtLen += 3;

	return TRUE;
}//End of function;

//添加AIM前缀
BOOL AddAIM_Old()
{
	int i;
	int AIMMod;
	int AIMPos;
	
	if (g_infoParam.pbyMsgBuff == NULL || g_infoParam.nMsgLen <= 0)
	{ return FALSE; }//End of if;

	if(!(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] & 0x01)) return TRUE; //不添加AIMID前缀
	
	AIMMod = (int)((X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] & 0x06) >> 1);
	AIMPos = (g_infoParam.nCodePos << 1);
	
	switch(AIMMod) {
	case 0: //添加一个AIM前缀
		switch(g_infoParam.nCodeCategory){
		case UT_TYPE_1D:
			for (i = g_nPrefixExtLen - 1; i >= 0; i --)
			{ g_InfEdPrefix[i + 1] = g_InfEdPrefix[i]; }
			g_InfEdPrefix[0] = AIMTab1D[AIMPos];
			g_nPrefixExtLen ++;
			break;
		case UT_TYPE_2D:
			for (i = g_nPrefixExtLen - 1; i >= 0; i --)
			{ g_InfEdPrefix[i + 1] = g_InfEdPrefix[i]; }
			g_InfEdPrefix[0] = AIMTab2D[AIMPos];
			g_nPrefixExtLen ++;
			break;
		case UT_TYPE_OCR:
			for (i = g_nPrefixExtLen - 1; i >= 0; i --)
			{ g_InfEdPrefix[i + 1] = g_InfEdPrefix[i]; }
			g_InfEdPrefix[0] = AIMOCR[AIMPos];
			g_nPrefixExtLen ++;
			break;
		default:
			return FALSE;
			break;
		}
		break;
	case 1: //添加2个AIM前缀
		switch(g_infoParam.nCodeCategory){
		case UT_TYPE_1D:
			for (i = g_nPrefixExtLen - 1; i >= 0; i--)
			{ g_InfEdPrefix[i + 2] = g_InfEdPrefix[i]; }
			g_InfEdPrefix[0] = AIMTab1D[AIMPos];
			g_InfEdPrefix[1] = AIMTab1D[AIMPos + 1];
			g_nPrefixExtLen += 2;
			break;
		case UT_TYPE_2D:
			for (i = g_nPrefixExtLen - 1; i >= 0; i--)
			{ g_InfEdPrefix[i + 2] = g_InfEdPrefix[i]; }
			g_InfEdPrefix[0] = AIMTab2D[AIMPos];
			g_InfEdPrefix[1] = AIMTab2D[AIMPos + 1];
			g_nPrefixExtLen += 2;
			break;
		case UT_TYPE_OCR:
			for (i = g_nPrefixExtLen - 1; i >= 0; i--)
			{ g_InfEdPrefix[i + 2] = g_InfEdPrefix[i]; }
			g_InfEdPrefix[0] = AIMOCR[AIMPos];
			g_InfEdPrefix[1] = AIMOCR[AIMPos + 1];
			g_nPrefixExtLen += 2;
			break;
		default:
			return FALSE;
		}
		break;

	case 2: //添加3个AIM前缀
		switch(g_infoParam.nCodeCategory){
		case UT_TYPE_1D:
			for (i = g_nPrefixExtLen - 1; i >= 0; i--)
			{ g_InfEdPrefix[i + 3] = g_InfEdPrefix[i]; }
			g_InfEdPrefix[0] = ']';
			g_InfEdPrefix[1] = AIMTab1D[AIMPos];
			g_InfEdPrefix[2] = AIMTab1D[AIMPos + 1];
			g_nPrefixExtLen += 3;
			break;
		case UT_TYPE_2D:
			for (i = g_nPrefixExtLen - 1; i >= 0; i--)
			{ g_InfEdPrefix[i + 3] = g_InfEdPrefix[i]; }
			g_InfEdPrefix[0] = ']';
			g_InfEdPrefix[1] = AIMTab2D[AIMPos];
			g_InfEdPrefix[2] = AIMTab2D[AIMPos + 1];
			g_nPrefixExtLen += 3;
			break;
		case UT_TYPE_OCR:
			for (i = g_nPrefixExtLen - 1; i >= 0; i--)
			{ g_InfEdPrefix[i + 3] = g_InfEdPrefix[i]; }
			g_InfEdPrefix[0] = ']';
			g_InfEdPrefix[1] = AIMOCR[AIMPos];
			g_InfEdPrefix[2] = AIMOCR[AIMPos + 1];
			g_nPrefixExtLen += 3;
			break;
		default:
			return FALSE;
		}
		break;
	default: return FALSE;
	}

	return TRUE;
}//End of function;

//添加CodeID前缀
BOOL AddCodeID()
{
	int i;

	if (g_infoParam.pbyMsgBuff == NULL || g_infoParam.nMsgLen <= 0)
	{ return FALSE; }//End of if;

	if(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] != 1)
	{ return TRUE; }

	for (i = g_nPrefixExtLen;i > 0 ;i --)
	{ g_InfEdPrefix[i] = g_InfEdPrefix[i - 1]; }

	switch(g_infoParam.nCodeCategory){
	case UT_TYPE_1D:
		g_InfEdPrefix[0] = X_paramGetGlobalAllParam()->pCode->by1DCodeID[g_infoParam.nCodePos];
		g_nPrefixExtLen++;
		if (X_paramGetGlobalAllParam()->pCode2->by1DCodeID_2[g_infoParam.nCodePos] != 0)
		{
			for (i = g_nPrefixExtLen;i > 1 ;i --)
			{ g_InfEdPrefix[i] = g_InfEdPrefix[i - 1]; }
			g_InfEdPrefix[1] = X_paramGetGlobalAllParam()->pCode2->by1DCodeID_2[g_infoParam.nCodePos];
			g_nPrefixExtLen++;
		}
		break;
	case UT_TYPE_2D:
		g_InfEdPrefix[0] = X_paramGetGlobalAllParam()->pCode->by2DCodeID[g_infoParam.nCodePos];
		g_nPrefixExtLen++;
		if (X_paramGetGlobalAllParam()->pCode2->by2DCodeID_2[g_infoParam.nCodePos] != 0)
		{
			for (i = g_nPrefixExtLen;i > 1 ;i --)
			{ g_InfEdPrefix[i] = g_InfEdPrefix[i - 1]; }
			g_InfEdPrefix[1] = X_paramGetGlobalAllParam()->pCode2->by2DCodeID_2[g_infoParam.nCodePos];
			g_nPrefixExtLen++;
		}
		break;
	case UT_TYPE_POSTAL:
		g_InfEdPrefix[0] = X_paramGetGlobalAllParam()->pCode2->byPostalCodeID[g_infoParam.nCodePos];
		g_nPrefixExtLen++;
		if(X_paramGetGlobalAllParam()->pCode2->byPostalCodeID_2[g_infoParam.nCodePos] != 0)
		{
			for(i = g_nPrefixExtLen;i > 1 ;i --)
			{ g_InfEdPrefix[i] = g_InfEdPrefix[i - 1]; }
			g_InfEdPrefix[1] = X_paramGetGlobalAllParam()->pCode2->byPostalCodeID_2[g_infoParam.nCodePos];
			g_nPrefixExtLen++;
		}
		break;
	case UT_TYPE_OCR:
		if (g_infoParam.nCodePos == POS_PASSPORT_VISAS)
			g_InfEdPrefix[0] = 'O';
		else
			g_InfEdPrefix[0] = 'S';
		g_nPrefixExtLen++;
		break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
}//End of function;

//添加自定义前缀
BOOL AddSelfPrefix()
{
	int i,k;
	
	if (g_infoParam.pbyMsgBuff == NULL || g_infoParam.nMsgLen <= 0)
	{ return FALSE; }//End of if;

	if(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] != 1)
	{ return TRUE; }

	k = X_paramGetGlobalAllParam()->pCode->nPrefix;
	
	if (k + g_nPrefixExtLen > __INFEDIT_EXTLEN_MAX__)
	{ return FALSE; }//End of if;

	for (i = g_nPrefixExtLen - 1; i >= 0 ; i--){
		g_InfEdPrefix[i + k] = g_InfEdPrefix[i];
	}
	memcpy(g_InfEdPrefix,X_paramGetGlobalAllParam()->pCode->byPrefix,k);
	g_nPrefixExtLen += k;

	return TRUE;
}//End of function;

//添加自定义后缀
BOOL AddSelfSuffix()
{
	int k;

	if (g_infoParam.pbyMsgBuff == NULL || g_infoParam.nMsgLen <= 0)
	{ return FALSE; }//End of if;

	if (X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] != 1)
	{ return TRUE; }

	k = X_paramGetGlobalAllParam()->pCode->nSuffix;
	if (k + g_nSuffixExtLen > __INFEDIT_EXTLEN_MAX__)
	{ return FALSE; }//End of if;

	memcpy(g_InfEdSuffix+g_nSuffixExtLen,X_paramGetGlobalAllParam()->pCode->bySuffix,k);
	g_nSuffixExtLen += k;

	return TRUE;
}//End of function;

//添加Terminal结束符后缀,至多只允许2个字符
BOOL AddTerminal()
{
	int k;

	if (g_infoParam.pbyMsgBuff == NULL || g_infoParam.nMsgLen <= 0)
	{ return FALSE; }//End of if;

	if (X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] != 1)
	{ return FALSE; }

	k = X_paramGetGlobalAllParam()->pCode->nTerminal;
	if (k > 2 || k <= 0)
	{ return FALSE; }

	if (g_infoParam.nBuffSize < g_infoParam.nMsgLen+k)
	{ return FALSE; }
	
	memcpy(g_infoParam.pbyMsgBuff+g_infoParam.nMsgLen,X_paramGetGlobalAllParam()->pCode->byTerminal,k);
	g_infoParam.nMsgLen += k;

	return TRUE;
}//End of function;

//截取
BOOL DataCut()
{
	int j,k;
	int nCut;
	int Flag = 0; //0代表截取位置是从前向后数的，1则代表是倒数的
	int nLength = 0;
	unsigned int Begin,End;
	BYTE MesData[2000];
	BYTE* pMsg = g_infoParam.pbyMsgBuff;
	UALLPARAM *psParam;

	psParam = X_paramGetGlobalAllParam();
	
	if(psParam->pCode->IsPreSufOn[POS_IsCut]){
		switch(g_infoParam.nCodeCategory){
		case UT_TYPE_1D: //一维条码
			for(j = 2; j >= 0; j--){//至多也只能有三种条码做了截取设置
				if(g_infoParam.nCodePos != psParam->pCode->byCut[j * 20]) continue;
				nCut = psParam->pCode->byCut[j * 20 + 1];
				if(nCut > 5)
				{
					return FALSE; //最多截取5段
				}//End of if;
				for(k = 0; k < nCut; k++){
					Flag = psParam->pCode->byCut[j * 20 + k * 3 + 2];
					Begin = psParam->pCode->byCut[j * 20 + k * 3 + 3];
					End = psParam->pCode->byCut[j * 20 + k * 3 + 4];
					if(!OneCut(MesData, (BYTE*)pMsg, Flag, Begin, End, g_infoParam.nMsgLen, &nLength))
					{
						return FALSE;
					}//End of if;
				}//End of for;
			
				if(nLength != 0) {
					memcpy((BYTE*)pMsg, MesData, nLength);
					g_infoParam.nMsgLen = nLength;
					break;//截取以最新一次的设置优先
				}
			}
			break;
		case UT_TYPE_OCR://OCR			
			for(j = 2; j >= 0; j--){//至多也只能有三种条码做了截取设置
				if(g_infoParam.nCodePos != psParam->pCode->byCut[j * 20]-64) continue;
				nCut = psParam->pCode->byCut[j * 20 + 1];
				if(nCut > 5) return FALSE; //最多截取5段
				for(k = 0; k < nCut; k++){
					Flag = psParam->pCode->byCut[j * 20 + k * 3 + 2];
					Begin = psParam->pCode->byCut[j * 20 + k * 3 + 3];
					End = psParam->pCode->byCut[j * 20 + k * 3 + 4];
					if(!OneCut(MesData, (BYTE*)pMsg, Flag, Begin, End, g_infoParam.nMsgLen, &nLength))
						return FALSE;
				}
			
				if(nLength != 0) {
					memcpy((BYTE*)pMsg, MesData, nLength);
					g_infoParam.nMsgLen = nLength;
					break;//截取以最新一次的设置优先
				}
			}
			break;
		case UT_TYPE_2D:
			for(j = 2; j >= 0; j--){
				if(g_infoParam.nCodePos != psParam->pCode->byCut[j * 20]-32) continue;
				nCut = psParam->pCode->byCut[j * 20 + 1];
				if(nCut > 3)
				{
					return FALSE; //最多截取3段
				}//End of if;
				for(k = 0; k < nCut; k++){
					Flag = psParam->pCode->byCut[j * 20 + k * 5 + 2];
					Begin = psParam->pCode->byCut[j * 20 + k * 5 + 3] * 100 + psParam->pCode->byCut[j * 20 + k * 5 + 4];
					End = psParam->pCode->byCut[j * 20 + k * 5 + 5] * 100 + psParam->pCode->byCut[j * 20 + k * 5 + 6];
					if(!OneCut(MesData, (BYTE*)pMsg, Flag, Begin, End,  g_infoParam.nMsgLen, &nLength))
					{
						return FALSE;
					}//End of if;
				}//End of for;
				
				if(nLength != 0) {
					memcpy((BYTE*)pMsg, MesData, nLength);
					g_infoParam.nMsgLen = nLength;
					break;//截取以最新一次的设置优先
				}
			}//End of for;
			break;
		case UT_TYPE_POSTAL:
			for(j = 2; j >= 0; j--){
				if(g_infoParam.nCodePos != psParam->pCode->byCut[j * 20]-96) continue;
				nCut = psParam->pCode->byCut[j * 20 + 1];
		
				if(nCut > 5)
				{
					return FALSE; 
				}//End of if;
				for(k = 0; k < nCut; k++){
					Flag = psParam->pCode->byCut[j * 20 + k * 3 + 2];
					Begin = psParam->pCode->byCut[j * 20 + k * 3 + 3];
					End = psParam->pCode->byCut[j * 20 + k * 3 + 4];
					if(!OneCut(MesData, (BYTE*)pMsg, Flag, Begin, End, g_infoParam.nMsgLen, &nLength))
					{
						return FALSE;
					}//End of if;
				}//End of for;
				
				if(nLength != 0) {
					memcpy((BYTE*)pMsg, MesData, nLength);
					g_infoParam.nMsgLen = nLength;
					break;
				}
			}
			break;
		default:
			return FALSE;
		}
	}

	return TRUE;
}

//截取一段数据
BOOL OneCut(unsigned char *MesData, unsigned char *p, int Flag, unsigned int Begin, unsigned int End, int pLength, int *nLength)
{
	int i;
	unsigned int BeginTemp,EndTemp;	
	
	BeginTemp = Begin;
	EndTemp = End;
	if(BeginTemp < 1 || EndTemp < 1) return FALSE;
	if(Flag > 1) return FALSE;
	if(BeginTemp > pLength && EndTemp > pLength) return TRUE;
    if(BeginTemp > pLength) BeginTemp = pLength;
	if(EndTemp > pLength) EndTemp = pLength;
	
	if(Flag == 1) {//反向的
		BeginTemp = pLength + 1 - BeginTemp;
		EndTemp = pLength + 1 - EndTemp;
	}

	if(BeginTemp <= EndTemp){//起始位置在终止位置前
		memcpy(MesData + *nLength, p + BeginTemp - 1, EndTemp + 1 - BeginTemp);
		(*nLength) += (EndTemp + 1 - BeginTemp);
	}
	else {//起始位置在终止位置后
		for(i = BeginTemp; i >= EndTemp; i--){
			MesData[*nLength + BeginTemp - i] = p[i - 1];
		}
		(*nLength) += (BeginTemp + 1 - EndTemp);
	}

	return TRUE;
}

int		W_infoEditting(WInfoParam *psParam)
{
	int i,k;
	//初始化
	W_infoInitEveryTime();
	if (psParam == NULL || psParam->pbyMsgBuff == NULL || psParam->nMsgLen <= 0)
	{ return 0; }

	g_infoParam = *psParam;
    
      if (!(X_paramGetGlobalAllParam()->pDFormat->DFEnable)) //如果Data Format开启 不进行数据截取
      {	//正向截取
	    if(!DataCut())
	    { 
		return g_infoParam.nMsgLen;
	    }//End of if;
      }
	//添加前缀
	if(!AddPrefix())
	{ 
		return g_infoParam.nMsgLen;
	}//End of if;

	//添加自定义后缀
	if(!AddSelfSuffix())
	{ 
		return g_infoParam.nMsgLen;
	}//End of if;

// 	//添加Terminal结束符后缀,至多只允许2个字符
// 	if(!AddTerminal())
// 	{ 
// 		return g_infoParam.nMsgLen;
// 	}//End of if;
	//////////////////////////////////////////////////////////////////////////
	// 将编辑后的信息综合
	k = g_nPrefixExtLen + g_infoParam.nMsgLen + g_nSuffixExtLen; //总长度
	if (g_infoParam.nBuffSize < k)
	{//空间不够
		return g_infoParam.nMsgLen;
	}//End of if;
	//添加后缀信息
	k -= g_nSuffixExtLen;
	memcpy(g_infoParam.pbyMsgBuff+k,g_InfEdSuffix,g_nSuffixExtLen);
	//回写解码信息
	k -= g_infoParam.nMsgLen;
	for (i = g_infoParam.nMsgLen-1; i >= 0; i--)
	{
		g_infoParam.pbyMsgBuff[k+i] = g_infoParam.pbyMsgBuff[i];
	}
	//添加前缀信息
	memcpy(g_infoParam.pbyMsgBuff,g_InfEdPrefix,g_nPrefixExtLen);

	//编辑后的最后信息长度
	g_infoParam.nMsgLen += (g_nPrefixExtLen+g_nSuffixExtLen);

	psParam->nMsgLen = g_infoParam.nMsgLen;
	
	return g_infoParam.nMsgLen;
}//End of function;



//Blue2D前缀处理
BOOL MsgPack_Blue2D(char *pMsg,int *nMsg)
{
	int i = 0;

	int nMsgLens = *nMsg;

	const char* TypeTable1D[] = {
		"ZASETUP","SETUP 128","Code 128","UCCEAN128","EAN-8","EAN-13","UPC-E",
		"UPC-A","I2Of5","ITF14","ITF6","China Post25","MODULE25","Code 39(Standard)",
		"Code 39(Full ASCII)","CodaBar","Standard 25","Code93","CodeZ"
	};
	UALLPARAM *psParam;
	psParam = X_paramGetGlobalAllParam();

	if (BLUE2D_PACK != psParam->pCode->nPack)
	{ return TRUE; }//BLUE2D打包类型

	if(nMsgLens > 9999)
	{ return FALSE; }

	if (psParam->pCode->IsPreSufOn[POS_IsPrefix]) nMsgLens -= psParam->pCode->nPrefix;
	if (psParam->pCode->IsPreSufOn[POS_IsSuffix]) nMsgLens -= psParam->pCode->nSuffix;
	if (psParam->pCode->IsPreSufOn[POS_IsAIM] & 0x01) nMsgLens -= (psParam->pCode->IsPreSufOn[POS_IsAIM]>>1)+1;

	if(psParam->pCode->IsPreSufOn[POS_IsCodeID])	{
		if((psParam->pCode->IsOtherPreSufOn >> POS_IsBlue2D_LenSend) & 0x01){ //有CodeID,且发送数据长度
			for (i = (*nMsg) - 1; i > 0; i--){
				pMsg[i + 6] = pMsg[i];
			}

			pMsg[5] = pMsg[0];
			pMsg[0] = ':';
			pMsg[6] = ':';
			
			pMsg[1] = nMsgLens / 1000 + '0';
			pMsg[2] = nMsgLens / 100 - nMsgLens / 1000 * 10 + '0';
			pMsg[3] = nMsgLens / 10 - nMsgLens / 100 * 10 + '0';
			pMsg[4] = nMsgLens - nMsgLens / 10 * 10 + '0';

			(*nMsg) += 6;
		}
		else { //有CodeID,但不发送数据长度
			for (i = (*nMsg) - 1; i > 0; i--){
				pMsg[i + 2] = pMsg[i];
			}
			
			pMsg[1] = pMsg[0];
			
			pMsg[0] = ':';
			pMsg[2] = ':';

			(*nMsg) += 2;
		}
	}
	else if((psParam->pCode->IsOtherPreSufOn >> POS_IsBlue2D_LenSend) & 0x01){
		//没有CodeID,但发送数据长度
		for (i = (*nMsg) - 1; i >= 0; i--){
			pMsg[i + 6] = pMsg[i];
		}

		pMsg[0] = ':';
		pMsg[1] = nMsgLens / 1000 + '0';
		pMsg[2] = nMsgLens / 100 - nMsgLens / 1000 * 10 + '0';
		pMsg[3] = nMsgLens / 10 - nMsgLens / 100 * 10 + '0';
		pMsg[4] = nMsgLens - nMsgLens / 10 * 10 + '0';
		pMsg[5] = ':';

		(*nMsg) += 6;
	}

	//添加条码类型信息
	if((psParam->pCode->IsOtherPreSufOn >> POS_IsBlue2D_TypeSend) & 0x01) {
		pMsg[(*nMsg)++] = '\r';
		pMsg[(*nMsg)++] = '\n';
		pMsg[(*nMsg)++] = 'T';
        pMsg[(*nMsg)++] = 'y';
        pMsg[(*nMsg)++] = 'p';
        pMsg[(*nMsg)++] = 'e';
		pMsg[(*nMsg)++] = ':';
		switch(g_infoParam.nCodeCategory){
		case UT_TYPE_1D:
			sprintf(pMsg + (*nMsg), " %s\r\n", TypeTable1D[g_infoParam.nCodePos]);
					(*nMsg) += strlen(TypeTable1D[g_infoParam.nCodePos]) + strlen(" \r\n");
			break;
		case UT_TYPE_2D:
			switch(g_infoParam.nCodeType){
			case DC_PDF417:
				sprintf(pMsg + (*nMsg)," PDF417\r\n");
				(*nMsg) += strlen(" PDF417\r\n");
				break;
			case DC_QRCODE:
				sprintf(pMsg + (*nMsg)," QRCode\r\n");
				(*nMsg) += strlen(" QRCode\r\n");
				break;
			case DC_DataMatrix:
				sprintf(pMsg + (*nMsg)," DMatrix\r\n");
				(*nMsg) += strlen(" DMatrix\r\n");
				break;
			case DC_VeriCode:
			case DC_UN_VeriCode:
			case DC_HAN_SENSIBLE:
			default:
				return FALSE;
				break;
			}
			break;
		case UT_TYPE_OCR:

#if defined(__OCR_B_VERSION)
			switch(g_infoParam.nCodeType){
			case DC_SPEC_OCR_B:
				sprintf(pMsg + (*nMsg)," SPEC_OCR_B\r\n");
				(*nMsg) += strlen(" SPEC_OCR_B\r\n");
				break;
			default:
				return FALSE;
				break;
			}
#endif 
			break;
		default:
			return FALSE;
			break;
		}
	}
	return TRUE;
}

static void AddLRC(char str[], int *len)
{
	BYTE LRCByte;
	int nOffset, i;
	
	nOffset = *len;
	LRCByte = 0xFF;
	for(i=2; i<nOffset; i++){
		LRCByte ^= str[i];
	}
	str[(*len)++] = LRCByte;
	str[*len] = 0;
}
void MsgPack_NLS(char* pMsg,int *nMsg)
{
	int i = 0;
	int nMsgLen = *nMsg;
	UALLPARAM *psParam;
	psParam = X_paramGetGlobalAllParam();

	if (FACTORY_PACK != psParam->pCode->nPack) return; //不需要打包


	//加上包头信息
	for(i = 0;i < nMsgLen; i ++)
		pMsg[nMsgLen+4-i] = pMsg[nMsgLen-i-1];
	//报文首部
	pMsg[0] = STX;
	pMsg[1] = ATTR;
	pMsg[2] = 0x00;	//length
	pMsg[3] = 0x00;
	pMsg[4] = AL_GET_MSG;
	//加上校验
	pMsg[2] = (nMsgLen&0x00ff00)>>8;
	pMsg[3] = (nMsgLen&0x0000ff);
	nMsgLen += 5;
#if defined(MB1126)
	if (UserType_PT852_P_H == psParam->pCode->nUserDef && g_Mode.bIRSign == TRUE) 
		pMsg[5] ^= 128;
#endif //#if defined(MB1126)
	AddLRC((char*)pMsg,&nMsgLen);
	(*nMsg) = nMsgLen;
}

void MsgPack_ECS_DATA(char* pMsg,int *nMsg)
{
	int i = 0;
	int nMsgLen = *nMsg;
	UALLPARAM *psParam;
	psParam = X_paramGetGlobalAllParam();

	if (ECS_BANK_PACK != psParam->pCode->nPack) return ;

	// 根据韩国ECS for bank设计的打包类型

	pMsg[nMsgLen+2] = 0;
	for(i = 0;i < nMsgLen;i ++)
	{
		pMsg[nMsgLen+2] ^= pMsg[nMsgLen-1-i];
		pMsg[nMsgLen-i] = pMsg[nMsgLen-1-i];
		//buff[i+1] = pMes[i];
	}
	pMsg[0] = STX;
	pMsg[nMsgLen+1] = ETX;//ETX = 0x03
	pMsg[nMsgLen+2] ^= pMsg[nMsgLen+1];
	pMsg[nMsgLen+3] = 0;
	nMsgLen += 3;
	(*nMsg) = nMsgLen;
}

void MsgPack_ECS_DATALEN(char* pMsg,int *nMsg)
{
	int i = 0;
	int nMsgLen = *nMsg;
	UALLPARAM *psParam;
	psParam = X_paramGetGlobalAllParam();

	if (ECS_PACK_ADDLEN != psParam->pCode->nPack) return ;
	// 根据韩国ECS for bank设计的打包类型

	pMsg[nMsgLen+6] = 0;//校验位置
	for(i = 0;i < nMsgLen;i ++)
	{
		pMsg[nMsgLen+6] ^= pMsg[nMsgLen-1-i];
		pMsg[nMsgLen-i+4] = pMsg[nMsgLen-1-i]; 
		//buff[i+1] = pMes[i];
	}
	pMsg[0] = STX;
	pMsg[1] = 0x30 + (nMsgLen/1000)%10;
	pMsg[2] = 0x30 + (nMsgLen/100)%10;
	pMsg[3] = 0x30 + (nMsgLen/10)%10;
	pMsg[4] = 0x30 + nMsgLen%10;

	pMsg[nMsgLen+6] ^= pMsg[1];
	pMsg[nMsgLen+6] ^= pMsg[2];
	pMsg[nMsgLen+6] ^= pMsg[3];
	pMsg[nMsgLen+6] ^= pMsg[4];

	pMsg[nMsgLen+5] = ETX;//ETX = 0x03
	pMsg[nMsgLen+6] ^= pMsg[nMsgLen+5];

	pMsg[nMsgLen+7] = 0;
	nMsgLen += 7;

	(*nMsg) = nMsgLen;
}


//将数据打包
int	W_infoPacking(WInfoParam *psParam)
{
	if (psParam == NULL || psParam->pbyMsgBuff == NULL || psParam->nMsgLen <= 0)
	{ return 0; }
	g_infoParam = *psParam;

	if (psParam->nBuffSize > psParam->nMsgLen+7)
	{
		MsgPack_NLS((char*)psParam->pbyMsgBuff,&psParam->nMsgLen);
	}//End of if;
	if (psParam->nBuffSize > psParam->nMsgLen + 3)
	{
		MsgPack_ECS_DATA((char*)psParam->pbyMsgBuff,&psParam->nMsgLen);
	}//End of if;
	if (psParam->nBuffSize > psParam->nMsgLen + 7)
	{
		MsgPack_ECS_DATALEN((char*)psParam->pbyMsgBuff,&psParam->nMsgLen);
	}//End of if;
	if (psParam->nBuffSize > psParam->nMsgLen+32)
	{
		if (!MsgPack_Blue2D((char*)psParam->pbyMsgBuff,&psParam->nMsgLen))
		{ return psParam->nMsgLen;}
	}//End of if;


	return psParam->nMsgLen;
}


//添加终止字符
int		W_infoAddTerminal(WInfoParam *psParam)
{
	if (psParam == NULL || psParam->pbyMsgBuff == NULL || psParam->nMsgLen <= 0)
	{ return 0; }

	g_infoParam = *psParam;
	
	//添加Terminal结束符后缀,至多只允许2个字符
	if(!AddTerminal())
	{ 
		return g_infoParam.nMsgLen;
	}//End of if;
	
	psParam->nMsgLen = g_infoParam.nMsgLen;

	return psParam->nMsgLen;
}
