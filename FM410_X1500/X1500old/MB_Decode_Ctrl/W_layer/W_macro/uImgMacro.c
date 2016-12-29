
/******************************************************************************************
*	include files 
*******************************************************************************************/

#include "uImgMacro.h"
#include <string.h>
/********************************************************************************************
*	local referenced extern function and variables 
********************************************************************************************/
//Macro 参数定义
#define  MAX_MACRO_BUFF   (64*1024+1024) // Macro Mem
#define  MACRO_MAX_LINK   16    // 最大链接符号数

typedef struct tagINFODATAINDEX
{
	int nBlock;
	int nLens;
	int nOffset;
} InfoDataDex;

typedef  struct  tagMarcoParam
{
	InfoDataDex		InfoDx[MACRO_MAX_LINK]; //宏信息块索引：获取信息在缓存中的位置，方便链接
	int				nInfoDx;

	unsigned char   MInfBkUp[MAX_MACRO_BUFF]; // 分成16块,每块能够存放信息 4096字节

	unsigned char	MInfBuff[MAX_MACRO_BUFF]; // 信息发送内存
	int             nMInfo; //信息发送长度

	int				nBCType; // 宏条码类型备份,防止出现读取QR条码后读取PDF条码的现象没有及时进行处理
	int				nCurSeq; // 目前已经发送的数据位置

	int             nError;      //错误类型
	int             nStatus;     //状态
	int             nUnMaching;  //不匹配状态统计
}UTMacroCtrl;
/*********************************************************************************************
*	global variables
*********************************************************************************************/
// 重新确定的参数
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

BOOL		  g_bMacroInited = FALSE;	//宏处理是否有经过初始化

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
//送回链接好的信息
void SendSequenceInfo(void);

int  iMacroSeqBuff(int nOrder,int nTotal);

void SetMacroStatus(int nStatus);

void vMacroSetUnMachCnt(void);


/*********************************************************************************************
*	Implementation functions 
*********************************************************************************************/

void W_macroSetParam(int nMacro)
{
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
	if (nMacro >= 0 && nMacro <= MACRO_LINK_16)
		g_MacroSet = nMacro;
	else 
		g_MacroSet = MACRO_DIRECT;
}

int W_macroGetParam(void)
{
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
	return g_MacroSet;
}

void vMacroSetUnMachCnt(void)
{
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
	if ((g_MCtrl.nUnMaching ++) > 3){//	//4次清零，第5次才能成功发送正确条码
		W_macroSetFlag(-1);
	}
	SetMacroStatus(U_uIMG_STATUS_MACRO_ERROR);
}

int W_macroMain(void)
{
	static  int  nstaErrMacro = 0;
	int          nResult = U_NO_MACRO;
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
	memcpy(g_MCtrl.MInfBuff,X_decoGetDecInfoBuff(),X_decoGetDecInfoLen());
	g_MCtrl.nMInfo = X_decoGetDecInfoLen();

//普通二维条码，并且上一次读取的条码是宏条码
	if (isCheckMacro()) //是宏
	{
		if (W_macroGetFlag() == -1){//初次使用宏，进行初始化
			W_macroSetFlag (X_decoGetCodeType());
		}
		else if (W_macroGetFlag() != X_decoGetCodeType()){ //类型不匹配不符合
			vMacroSetUnMachCnt();
			return U_MACRO_ID_UNCONFORMITY;
		}
		nstaErrMacro = 0;
		//无需链接直接传送信息
		if (W_macroGetParam() == MACRO_DIRECT)
		{ return U_NO_MACRO; }//直接发送，按照普通条码进行处理

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
		//根据nResult的不同返回值进行分别处理:设定当前的返回值和错误状态
		return nResult;
	}
	else// if(!isCheckMacro())//当前条码--不是宏
	{
		if (W_macroGetFlag() != -1) //之前是宏
		{
			vMacroSetUnMachCnt();
			return U_MACRO_TYPE_UNCONFORMITY;
		}
	}
	return U_NO_MACRO; //不是宏条码，没有限制
}

//是否符合宏类型
// TRUE ---  继续处理
// FALSE --- 宏出现冲突
BOOL W_macroGetUnmacthParam(void)
{
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
	// 如果是现在是宏/之前是宏，
	if (X_decoGetBarCode() == UT_TYPE_2D         //大类型匹配
		&& W_macroGetFlag() == X_decoGetCodeType()  // 具体类型匹配
		&& isCheckMacro() )  // 确认是宏
	{
		return TRUE; //类型匹配，继续
	}
	//类型不匹配不符合
	vMacroSetUnMachCnt();
	return FALSE;
	
}

void  W_macroSetError(int nError)
{
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
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
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
	return g_MCtrl.nMInfo;
}

unsigned char* W_macroGetBuff(void)
{
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
	return g_MCtrl.MInfBuff;
}

void SetMacroStatus(int nStatus)
{
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
	g_MCtrl.nStatus = nStatus;
}

int W_macroGetStatus(void)
{
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
	return g_MCtrl.nStatus;
}

int W_macroGetError(void)
{
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
	return g_MCtrl.nError;
}


void W_macroSetFlag(int nFlag)
{
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
	if (nFlag == -1)
		g_MCtrl.nUnMaching = 0;
	g_MCtrl.nBCType = nFlag;
}

int W_macroGetFlag(void)
{
	_MACRO_INIT_() //判断宏是否有需要自动将之初始化
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

//初始化参数
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

	//测试信息zyb添加
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
	//Y_commWrite(g_MCtrl.MInfBuff, g_MCtrl.nMInfo);	//测试信息ZYB添加

//Y_commWrite(buf, 2);
/***************************************************/

	if (g_MCtrl.nInfoDx == 0)
	{
		g_MID = g_MIDTmp;
	}
	else
	{ 
		//判断是否同一个宏条码结构
//测试信息zyb添加
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

///测试信息zyb添加
/************************************/
/*	buf[6] = (g_MID.nTotal >> 8) & 0xff;
	buf[7] = g_MID.nTotal & 0xff;
	buf[8] = (g_MID.nOrder>> 8) & 0xff;
	buf[9] = g_MID.nOrder& 0xff;
	buf[10] = (g_MID.nIDLen>> 8) & 0xff;
	buf[11] = g_MID.nIDLen& 0xff;
Y_commWrite(buf, 12);*/
/************************************/

	
	//总长度超过允许的符号连接数,自动转为按照顺序传送
	if (g_MIDTmp.nOrder > MACRO_MAX_LINK ||  W_macroGetParam() == MACRO_SEQ_BUFF)
	{
	      //Y_commWrite(buf, 1);
		return iMacroSeqBuff(g_MIDTmp.nOrder,g_MID.nTotal);
	}//End of if;
	//Y_commWrite(buf+1, 1);
	//记录当前块
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
	//暂存目前信息
	memcpy((char*)(g_MCtrl.MInfBkUp+g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nOffset),  
		   (char*)g_MCtrl.MInfBuff, g_MCtrl.nMInfo);




	g_MCtrl.nInfoDx ++;
	//调试信息zyb添加
/*********************************************/
	//Y_commWrite(g_MCtrl.MInfBuff, g_MCtrl.nMInfo);
	//Y_commWrite(g_MCtrl.MInfBkUp+g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nOffset, g_MCtrl.nMInfo);
/*	memset(buf, 0, sizeof(buf));
	for (i=0; i<g_MCtrl.nInfoDx; ++i)
	{	sprintf((char*)buf+(strlen((char*)(buf))), "%d,",g_MCtrl.InfoDx[i].nBlock);	}	///发送块号是否对

	Y_commWrite((unsigned char*)buf, strlen((char*)buf));*/


/*********************************************/

	if (g_MCtrl.nInfoDx == g_MID.nTotal)	//读取到最后一个宏条码
	{
		SendSequenceInfo();
		return U_MACRO_END;
	}


	return U_MACRO_BUFF;
}

//送回链接好的信息
void SendSequenceInfo(void) 
{
	int i,j;

	g_MCtrl.nMInfo = 0;

	//找到块中最小的标志(主要是对Macro PDF417)
	for (i = 0;i < g_MCtrl.nInfoDx;i ++)
	{
		for (j = 0;j < g_MCtrl.nInfoDx;j ++)
		{
			if (i != g_MCtrl.InfoDx[j].nBlock) continue;
			memcpy((char*)(g_MCtrl.MInfBuff+g_MCtrl.nMInfo),
				   (char*)(g_MCtrl.MInfBkUp+g_MCtrl.InfoDx[j].nOffset),
				   g_MCtrl.InfoDx[j].nLens);
				//调试信息zyb添加
/*********************************************/
	//Y_commWrite(g_MCtrl.MInfBuff, g_MCtrl.nMInfo);
	//Y_commWrite(g_MCtrl.MInfBkUp+g_MCtrl.InfoDx[j].nOffset, g_MCtrl.InfoDx[j].nLens);

/*********************************************/
			g_MCtrl.nMInfo += g_MCtrl.InfoDx[j].nLens;
		}
	}
	//针对不同的条码,可能需要进行校验验证
	if (X_decoGetCodeType() == DC_PDF417){}
	else if (X_decoGetCodeType() == DC_QRCODE){}
	else if (X_decoGetCodeType() == DC_DataMatrix){}

	return ;
}


// 按照顺序发送解码后的信息
// 如果超出顺序，暂时缓存，直到轮到为止
// 除非目前缓存超出64k
int iMacroSeqBuff(int nOrder,int nTotal)
{
	int i,j,k,offsetLen;
//	unsigned char buf[10];
	if (nOrder == g_MCtrl.nCurSeq) {//如果按照顺序,目前该发送此信息

//		buf[0] = (nOrder<<8)&0xff;
//		buf[1] = (nOrder&0XFF);
//		Y_commWrite( buf,2);
		
		g_MCtrl.nCurSeq ++;
		//继续在当前的缓存中寻找可以连接的信息
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

			g_MCtrl.nInfoDx --;  //少一个检索信息

			g_MCtrl.nCurSeq ++;  //往前行一步


			offsetLen = g_MCtrl.InfoDx[i] .nLens;///////zyb添加
			if(i != g_MCtrl.nInfoDx)	//当连接掉的块在缓存的最后位置时，不用左移。zyb添加
			{
				for (j = i;j < g_MCtrl.nInfoDx; j ++) 	//左移已经连接掉的块的长度
				{
					for (k = 0; k < g_MCtrl.InfoDx[j+1].nLens; k ++)//for (k = 0; k < g_MCtrl.InfoDx[j].nLens; k ++)
						g_MCtrl.MInfBkUp[g_MCtrl.InfoDx[j].nOffset + k] = 
							g_MCtrl.MInfBkUp[g_MCtrl.InfoDx[j+1].nOffset+k];
					
					g_MCtrl.InfoDx[j] = g_MCtrl.InfoDx[j+1];	
				}

/**************************************************/
//zyb添加，缓存中的子条码左移时，各条码在缓存中的偏移量也要修改
				j--;
				for(;j>0;j--)
				{
					g_MCtrl.InfoDx[j].nOffset -= offsetLen; //后面一块的偏移量应该减掉被连接掉的长度		
				}
				if(g_MCtrl.InfoDx[j].nOffset != 0)	//第一块没偏移时，无须将偏移量置0
					g_MCtrl.InfoDx[j].nOffset -= offsetLen; //j=0;
/***************************************************/
			}
			
			i = -1;
		}

		if (g_MCtrl.nCurSeq == nTotal)	return U_MACRO_END;

		return U_MACRO_SEQ_SEND;
	}
	else if (nOrder > g_MCtrl.nCurSeq){ // 当前的顺序超出了该连接的部分
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
			if (g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nLens + g_MCtrl.nMInfo < MAX_MACRO_BUFF)//缓存足够
			{ 
				memcpy((char*)(g_MCtrl.MInfBkUp+g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nOffset), 
						(char*)g_MCtrl.MInfBuff,g_MCtrl.nMInfo);
				g_MCtrl.InfoDx[g_MCtrl.nInfoDx].nBlock = nOrder;
				g_MCtrl.nInfoDx ++;
				return U_MACRO_BUFF;
			}
			else
				return U_MACRO_OVERFLOW; // 缓存不足
		}
		else //if (g_MCtrl.nInfoDx >= MACRO_MAX_LINK) //
		{
			return U_MACRO_OVERFLOW;
		}
	}
	else{// if (nOrder < g_MCtrl.nCurSeq){ 
		return U_MACRO_SUPERPOSITION; //已经发过信息了
	}
}

