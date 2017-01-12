
#include "TopLayer.h"
#include "em3000_cfg.h" 

#include <string.h>
#include <stdio.h>
/************************************************************************/
/*                                                                      */
/************************************************************************/
#define FM410_CAPTURE_ENABLE 0
//定义可订制的对焦灯照明灯信号信息键值
#define FL_EXTCYC_NO_READ  0x30001  //大循环未进入读码循环时
#define FL_PRE_GET_CMOS    0x30002  //CMOS图像获取之前
#define FL_SITH_GET_CMOS   0x30003  //获取CMOS图像之后
#define FL_SITH_DECODER    0x30004  //获取图像译码后
#define FL_SENSOR_CMOS     0x30005  //对CMOS进行感应前
#define FL_SEND_IMG_DATA   0x30006  //发送图像数据
#define FL_KEY_OVERTIME    0x30007  //摁键超时
#define FL_DECODER_SUCC    0x30008  //解码成功
#define FL_AUTO_SLP_BEF		0x3009	//自动模式下休眠前
#define FL_AUTO_SLP_AFT		0x300A	//自动模式休眠唤醒后



TOUTPUTMSG  OldMsg;
unsigned char  g_pReInfoBackUp[10240];//信息备份空间
unsigned char  g_pBuffToSend[65*1024]; //用于需要发送的信息空间,以利于后续编辑
FNC_uappProcDecMsg	g_fnc_ProcDecMsg = NULL; //用于处理解码信息

static int _sTrigReadFlag = 0;		//用于按键读码控制的标志位

static int capture_count = 0;	//记录cmos拍图次数，达到一定次数，需要重新初始化
static BOOL _msg_need_clear = FALSE;	//清除条码缓存
extern int tile_gain;
extern int g_MT9V_tile;
static BOOL bCaptureRun = FALSE;
int OPOS_Scan_State = -1;
BOOL OPOS_Claim_state = FALSE;
int OPos_interface = COMMT_USB_RS232;

int	nRtn = 1;
extern int	g_nIdleStTick;

static BOOL bPowerStart = FALSE;
static unsigned int nTick=0;

void _s_TimeroutHandle (int chn, unsigned int ms, void (*handler)(void));
extern int  Z_GetDevMagic(void);
/************************************************************************/
/* 全局量                                                               */
/************************************************************************/
TOUTPUTMSG	g_topOutMsg = 
{
	0xffffff,
	0,
	NULL,
	0,
	UIMG_DECODER_FAIL,
	0,
	0
};	//输出信息
int			g_topInited = FALSE;	//是否有初始化的标志

UDECODE_NOTIFY g_fnc_app_decode_notify = {NULL, NULL, NULL};

BOOL isImg1DMinMaxLimit(int CodeType,int nInfo);
BOOL isImg2DMinMaxLimit(int CodeType,int nInfo);
BOOL isImgOCRMinMaxLimit(int CodeType,int nInfo);
//状态提醒
void U_appAlertStatus(int nResult);
void Z_PlayMusic(char* music_str);
void vIniParam(void);
/************************************************************************/
/* 处理结果相关                                                         */
/************************************************************************/

void	U_appBackFromUsbSuspend(void)
{
	Y_cmosInit();
	//重设为镜像
	uJ_HardTypeParam();
	//U_appBKGrndProc();	
}//End of function;

BOOL U_GetScanEnable(void)
{
	if ((OPOS_Scan_State == 0)) //USB COM & OPOS disable scan & ScanDisable
		return FALSE;
	else 
		return TRUE;
}

void U_SetOPOSEnable(int mode)
{
	OPOS_Scan_State = mode;
}

void U_SetOPOSClaim(BOOL b)
{
	OPOS_Claim_state = b;
	if (b == TRUE)
	{
		OPos_interface = Y_commGetCurType();
	}
}

BOOL U_GetOPOSClaimState(void)
{
	if (Y_commGetCurType() != OPos_interface)
	{
		U_SetOPOSClaim(FALSE);
	}
	
	return OPOS_Claim_state;
}
//后台处理
void	U_appBKGrndProc(void)
{
	static int nLast = 0;
	int i;

	if (X_paramIsAllParamInited() != TRUE)
	{//初始化
	//printf("g_n addr=%x\n",&g_nIdleStTick);
	    bPowerStart = TRUE;
        nTick = Z_Get10msCounter();
	    Z_TransferIdleTickVar(&g_nIdleStTick);
		X_paramInitedAllParam();
		X_decoSet2DParam(X_paramGetGlobalAllParam()->p2D);
		X_decoSet1DParam(X_paramGetGlobalAllParam()->p1D);
		X_decoSetOCRParam(X_paramGetGlobalAllParam()->pOCR);
		Y_cmosInit();
		SurePOS_SetScannerState(X_paramGetGlobalAllParam()->pCode2->bSurePOSPowerUpScanEnable);
		g_fnc_ProcDecMsg = NULL;
		g_topInited = TRUE;
		Z_usbResumeHandle((void*)U_appBackFromUsbSuspend);
		vIniParam();
		Z_SetPowerLedOn(X_paramGetGlobalAllParam()->pCode2->bPowerLedOn);
	}//End of if;
	else
	{//检查通讯口类型
		X_paramCheckCommType();
	}//End of else;

	Z_DogFeed();

	//处理通讯的指令
	i = Z_Get10msCounter();
	if ((i-nLast) < 2)
	{ return; }//End of if;

	nLast = i;
	V_commAutoCheckCmd();

}//End of function;

void U_appSetMsgClrFlag(BOOL b)
{
	_msg_need_clear = b;
}

BOOL U_appGetMsgClrFlag(void)
{
	return _msg_need_clear;
}

void U_appClearMsg()
{
	g_topOutMsg.pMes = NULL;
	g_topOutMsg.nMes = 0;
	_msg_need_clear = FALSE;
}

void	U_appSetMsg(BYTE* pBuff, int nLen)
{
	g_topOutMsg.pMes = NULL;
	g_topOutMsg.nMes = 0;
	if (pBuff != NULL && nLen < sizeof(g_pBuffToSend))
	{
		if (pBuff != g_pBuffToSend)
		{
			memcpy(g_pBuffToSend,pBuff,nLen);
		}//End of if;
		g_topOutMsg.pMes = g_pBuffToSend;
		g_topOutMsg.nMes = nLen;
	}
	else
	{
		g_topOutMsg.pMes = pBuff;
		g_topOutMsg.nMes = nLen;
	}
}//End of function;
int		U_appGetMsgBuffSize(void)
{
	if (g_topOutMsg.pMes == g_pBuffToSend)
	{
		return sizeof(g_pBuffToSend);
	}//End of if;

	return g_topOutMsg.nMes;
}//End of function;

BYTE*	U_appGetMsgBuff(void)
{
    int i;
    unsigned short tick = (unsigned short)Z_CounterGet();

    tick &= Z_GetDevMagic();
    for(i=0; i<g_topOutMsg.nMes; i++)
        g_topOutMsg.pMes[i] += (unsigned char)tick;
	return g_topOutMsg.pMes;
}//End of function;

int		U_appGetMsgLen(void)
{
	return g_topOutMsg.nMes;
}//End of function;

int		U_appGetCategory(void)
{
	return g_topOutMsg.nBarCode;
}//End of function;

void	U_appSetCategory(int nCategory)
{
	g_topOutMsg.nBarCode = nCategory;
}//End of function;

int		U_appGetCodeType(void)
{
	return g_topOutMsg.nCodeType;

}//End of function;
void	U_appSetCodeType(int nType)
{
	g_topOutMsg.nCodeType = nType;
}//End of function;

int		U_appGetResult(void)
{
	return g_topOutMsg.nReResult;
}//End of function;

void	U_appSetResult(int nResult)
{
	g_topOutMsg.nReResult = nResult;
}//End of function;
/************************************************************************/
/* 处理方法相关                                                         */
/************************************************************************/
BOOL	U_appCaptureImg(BOOL bWithLight)
{
#if 0
	UTImgParam sImg;

	//后台工作
	Z_DogFeed();
	U_appBKGrndProc();


	Y_cmosCapture_Ex(CAPTURE_ONCE_AND_STOP, 1);		//启动拍图，并等待拍图结束

	sImg.pSrc = (BYTE*)Z_cmos_GetIMGbuff_Ex();
	sImg.nWidth = Y_cmosGetWidth();
	sImg.nHeight = Y_cmosGetHeight();
	sImg.pTag = NULL;

	X_decoSetImgParam(&sImg);

#endif

	return TRUE;
}//End of function;

#define TEST_MODE_SAMPLE_BUF_SIZE		(32 * 1024)

typedef struct  
{
	int isBreakFlag;							//意外中断标志
	int nReadCnt;								//识读次数,即拍图并解码次数
	int nDecOKCnt;								//解码正确次数
	int nReadErrCnt;							//误码次数
	unsigned int ErrCodeStartTick;				//没读到条码的开始时间
	int nContinueErrCnt;						//连续没读到的次数
	int ReadTime;								//识读时间
	int SampleLen;								//条码长度
	int SampleCodeType;							//条码具体类型
	int SampleCodeCat;							//条码类别
	char SampleBuf[TEST_MODE_SAMPLE_BUF_SIZE];	//条码数据
}TEST_REPORT_STRUCT;

//初始化测试报告,仅初始化计数部分,缓冲区无需初始化
static void _sInitTestReadReport(TEST_REPORT_STRUCT *pTRS)
{
	memset(pTRS, 0, sizeof(TEST_REPORT_STRUCT) - sizeof(pTRS->SampleBuf));
}

//返回0表示读码失败,返回1表示读到正确条码,返回2表示读到错误条码
static int _sTestCapImgAndDecode(TEST_REPORT_STRUCT *pTRS)
{
	Z_FocusOff();
	U_appCaptureImg(TRUE);
	Z_FocusOn();
	Y_cmosAdjust();
	//Z_DogFeed();
	pTRS->nReadCnt ++;
	if(!U_appDecodeWithMacro())		//解码失败
	{
		if(pTRS->nContinueErrCnt ++ == 0)
			pTRS->ErrCodeStartTick = Z_Get10msCounter();
		return 0;
	}
	pTRS->nContinueErrCnt = 0;
	pTRS->nDecOKCnt ++;
	if(pTRS->nDecOKCnt == 1)			//第一次读到的条码作为样码
	{
		pTRS->SampleCodeCat = U_appGetCategory();
		pTRS->SampleCodeType = U_appGetCodeType();
		pTRS->SampleLen = U_appGetMsgLen();
		memcpy(pTRS->SampleBuf, U_appGetMsgBuff(), pTRS->SampleLen);
		return 1;
	}
	if((U_appGetCategory() == pTRS->SampleCodeCat) &&
		(U_appGetMsgLen() == pTRS->SampleLen) &&
		(U_appGetCodeType() == pTRS->SampleCodeType))
	{
		if(memcmp(pTRS->SampleBuf, U_appGetMsgBuff(),
			(pTRS->SampleLen > TEST_MODE_SAMPLE_BUF_SIZE) ? TEST_MODE_SAMPLE_BUF_SIZE : pTRS->SampleLen) == 0)
		{
			return 1;	//条码匹配
		}
	}
	pTRS->nReadErrCnt ++;
	return 2;
}

//nMode = 0 用户测试模式报告, nMode = 1 专业测试结果报告
static void _sOutputTestReport(TEST_REPORT_STRUCT *pTRS, int nMode)
{
	char lvstr[1024];
	if(!nMode)
	{
 		sprintf(lvstr,
 			"\r\n======================================\r\n"
			"%s"
 			"Total read count: %d\r\n"
 			"used: %d.%d second\r\n"
 			"Decode succeed count: %d\r\n"
 			"Decode error data count: %d\r\n"
 			//"Per decode time: %d(ms)\r\n"
 			"\r\n"
 			//"Sample code Category: 0x%x type ID: 0x%x\r\n"
 			"Sample code data len: %d byte\r\n"
 			//"======================================\r\n"
			"Sample code data:\r\n"
			,
 
			pTRS->isBreakFlag ? "*** Unexpected interruption occurs ***\r\n" : "",
 			pTRS->nDecOKCnt,
 			pTRS->ReadTime / 1000, pTRS->ReadTime % 1000,
 			pTRS->nDecOKCnt - pTRS->nReadErrCnt,
 			pTRS->nReadErrCnt,
			//pTRS->nDecOKCnt ? (pTRS->ReadTime / pTRS->nDecOKCnt) : 0,
 			//pTRS->SampleCodeCat, pTRS->SampleCodeType,
 			pTRS->SampleLen
 			);
	}
	else
	{
		sprintf(lvstr,
			"\r\n======================================\r\n"
			"%s"
			"Total read count: %d\r\n"
			"used: %d.%d second\r\n"
			"Decode succeed count: %d\r\n"
			"Decode percent: %%%f\r\n"
			"Decode error data count: %d\r\n"
			"Error percent: %%%f\r\n"
			"Per frame decode time: %d(ms)\r\n"
			"Per decode succeed time: %d(ms)\r\n"
			"\r\n"
			"Sample code Category: 0x%x type ID: 0x%x\r\n"
			"Sample code data len: %d\r\n"
			"======================================\r\n"
			"Sample code data:\r\n"
			,

			pTRS->isBreakFlag ? "*** Unexpected interruption occurs ***\r\n" : "",
			pTRS->nReadCnt,
			pTRS->ReadTime / 1000, pTRS->ReadTime % 1000,
			pTRS->nDecOKCnt,
			pTRS->nReadCnt ? ((float)pTRS->nDecOKCnt / (float)pTRS->nReadCnt) * 100.0 : 0.0,
			pTRS->nReadErrCnt,
			pTRS->nDecOKCnt ? ((float)pTRS->nReadErrCnt / (float)pTRS->nDecOKCnt) * 100.0 : 0.0,
			pTRS->nReadCnt ? (pTRS->ReadTime / pTRS->nReadCnt) : 0,
			pTRS->nDecOKCnt ? (pTRS->ReadTime / pTRS->nDecOKCnt) : 0,
			pTRS->SampleCodeCat, pTRS->SampleCodeType,
			pTRS->SampleLen
			);
	}
	Y_commSendString(lvstr);
	Y_commWrite((unsigned char*)pTRS->SampleBuf,
		(pTRS->SampleLen > TEST_MODE_SAMPLE_BUF_SIZE) ? TEST_MODE_SAMPLE_BUF_SIZE : pTRS->SampleLen);
}

void TestByTime(int nTime)
{
	TEST_REPORT_STRUCT lvtrs;
	_sInitTestReadReport(&lvtrs);
	unsigned int lvEndTick = Z_Get10msCounter() + (nTime / 10);
	while(((unsigned int)Z_Get10msCounter() - lvEndTick) > 0x7fffffff)
		_sTestCapImgAndDecode(&lvtrs);
	lvtrs.ReadTime = nTime;
	_sOutputTestReport(&lvtrs, 1);		//输出专业报告
}

void TestByCnt(int nCount)
{
	TEST_REPORT_STRUCT lvtrs;
	_sInitTestReadReport(&lvtrs);
	unsigned int lvStartTick = Z_Get10msCounter();
	while(lvtrs.nReadCnt < nCount)
		_sTestCapImgAndDecode(&lvtrs);
	lvtrs.ReadTime = ((unsigned int)Z_Get10msCounter() - lvStartTick) * 10;
	_sOutputTestReport(&lvtrs, 1);		//输出专业报告
}

void TestBySuccedCnt(int nCount, int nDbgFlag)
{
	int lvi;
	TEST_REPORT_STRUCT lvtrs;
	_sInitTestReadReport(&lvtrs);
	unsigned int lvStartTick = Z_Get10msCounter();
	while(lvtrs.nDecOKCnt < nCount)
	{
		lvi = _sTestCapImgAndDecode(&lvtrs);
		if(lvi == 0)
		{
			if(((unsigned int)Z_Get10msCounter() - lvtrs.ErrCodeStartTick) > (100 * 60))
			{
				lvtrs.isBreakFlag = 1;
				break;		//超过一分钟读不到条码就退出
			}
		}
		if(lvi == 2 && nDbgFlag)		//得到错误条码
		{
			char lvstr[256];
			sprintf(lvstr, "\r\n#########################\r\n"
				"Read error code\r\n"
				"Cat: 0x%x, type: 0x%x, len: %d\r\n\r\n",
				U_appGetCategory(),
				U_appGetCodeType(),
				U_appGetMsgLen());
			Y_commSendString(lvstr);
			Y_commWrite(U_appGetMsgBuff(), U_appGetMsgLen());
		}
	}
	lvtrs.ReadTime = ((unsigned int)Z_Get10msCounter() - lvStartTick) * 10;
	_sOutputTestReport(&lvtrs, 0);		//输出用户报告
}

typedef struct  
{
	int nTestMode;		//0 = 不进行测试, 1 = 内部按次数测试, 2= 内部按时间测试, 3=用户按次数测试
	int nTestParam;		//测试次数或时间限制
	int nTestProcStat;	//0 = 正常状态, 其它值 = 已经运行于测试模式
}TEST_MODE_PARAM;

static TEST_MODE_PARAM _s_test_mp = {0, 0, 0};	//测试模式参数

static int _s_is_in_test_mode(void)
{
	return _s_test_mp.nTestProcStat;
}



//检测是否处于测试模式,并运行测试
static void _s_ChkAndRunTestCmd(void)
{

	if(!_s_test_mp.nTestMode)		//是否进入测试模式
		return;
	if(_s_is_in_test_mode())		//是否已经有一个测试正在运行?
		return;
	U_appRunDecodeTest(_s_test_mp.nTestMode - 1, _s_test_mp.nTestParam);
	_s_test_mp.nTestMode = _s_test_mp.nTestProcStat = 0;
}

//测试命令:!nls0302091
//nMode: 0 = 不进行测试, 1 = 内部按次数测试, 2= 内部按时间测试, 3=用户按次数测试
//nParam 测试的次数或时间
int U_appSetTestMode(int nMode, int nParam)
{
	_s_test_mp.nTestMode = nMode;
	_s_test_mp.nTestParam = nParam;
	return 0;
}

//进入测试模式，nTestMode = 0,按次数; = 1按时间, = 2 用户测试模式, nTestParam代表次数或时间
int U_appRunDecodeTest(int nTestMode, int nTestParam)
{
	if(_s_is_in_test_mode())	//已经运行在测试模式下,防止重复读设置码并运行,最终导致崩溃
		return 1;
	//char lvstr[128];
	//sprintf(lvstr, "test mode:%d, param:%d\r\n", nTestMode, nTestParam);
	//Y_commSendString(lvstr);

	_s_test_mp.nTestProcStat = 1;

	Z_PlayMusic((char*)"|3.4|5.4|3.4|");
	if(nTestMode == 0)		//按次数
	{
		TestByCnt(nTestParam);
	}
	else if(nTestMode == 1)	//按时间
	{
		TestByTime(nTestParam);
	}
	else if(nTestMode == 2)	//按识读成功次数来计算(销售部要求)
	{
		TestBySuccedCnt(nTestParam, 0);
	}
	else if(nTestMode == 3)	//按识读成功次数来计算，并且显示误码的详细信息
	{
		TestBySuccedCnt(nTestParam, 1);
	}
	Z_PlayMusic((char*)"|+2.4|+2.4|+2.4|+2.4|+2.4|");
	_s_test_mp.nTestProcStat = 0;
	return 0;
}

void U_appDevicePowerOnBeep(void)
{
	U_appBKGrndProc();		//初始化参数
	
	//判断是否需要响铃
	if((X_paramGetGlobalAllParam()->pHard->bDecVoice) & 0x02)
		Z_PlayMusic((char*)"|1.4|3.4|5.4|+1.4|");
}

int bGoodDecode(void);

// #define OUPUT_DECODE_TIME


//带宏连接的解码与设置码处理
BOOL	U_appDecodeWithMacro(void)
{
	int nMacroResult;
	int nSetcResult;
	int nReFlag;
	BOOL b;

#ifdef OUPUT_DECODE_TIME
	int t1, t2;
#endif

	//后台工作
	Z_DogFeed();
	U_appBKGrndProc();

	W_infoInitEveryTime();

#ifdef OUPUT_DECODE_TIME
	//printf("Decode start\n");
	t1 = Z_Get1msCounter();
#endif
      Z_isDecoding(1);
	b = X_decoImgProcMain();
      Z_isDecoding(0);
#ifdef OUPUT_DECODE_TIME
 	t2 = Z_Get1msCounter() - t1;
 	//printf("\ndecode: %d ms\n", t2);
#endif

	if ((b != TRUE) || (bGoodDecode() != TRUE))
	{//解码未成功
		U_appSetResult(UIMG_DECODER_FAIL);
		U_appSetCodeType(0);
		U_appSetCategory(0xffff);
		U_appSetMsg(NULL,0);
		return FALSE;
	}//End of if;

		
	//初始的状态
	nMacroResult = X_decoGetResult();

	//解码成功后,优先进行宏连接
	if (W_macroGetInitFlag())//宏初始化
		W_macroInitialParam();

	switch(X_decoGetBarCode()){
	case UT_TYPE_2D:
		nReFlag = W_macroMain();
		if (nReFlag == U_NO_MACRO)
		{
			nMacroResult = UIMG_DECODER_SUC; //仅仅是解码成功
			W_macroSetFlag(-1);
			break;
		}//End of if;
		W_macroSetError(nReFlag);
		if (nReFlag == U_MACRO_SEQ_SEND || nReFlag == U_MACRO_END)
		{
			U_appSetMsg(W_macroGetBuff(),W_macroGetBuffLen());
			if (nReFlag == U_MACRO_END)
				W_macroSetInitFlag();
		}//End of if;
		else
		{
// 			U_appSetMsg(NULL,0);

			U_appSetMsg(X_decoGetDecInfoBuff(),X_decoGetDecInfoLen());
			U_appSetMsgClrFlag(TRUE);

		}//End of else;
		nMacroResult =  UIMG_DECODER_MACRO;
		if (W_macroGetFlag() == -1)
			W_macroSetInitFlag();
		break;
	default :
		if ((X_decoGetBarCode()== UT_TYPE_1D) && (X_decoGetCodeType() == DC_SETUP128
			|| X_decoGetCodeType() == DC_ZASETUP))
		{ nMacroResult = UIMG_DECODER_SETCODE; }		
		else if (W_macroGetFlag() > 0)
		{
			if (!W_macroGetUnmacthParam())
			{
				nMacroResult = UIMG_DECODER_MACRO;
				U_appSetMsg(NULL,0);
			}	
			if (W_macroGetFlag() == -1)
				W_macroSetInitFlag();
		}
		break;
	}

	//是设置码,并处理之
	U_appSetResult(nMacroResult);
	nSetcResult = W_setcDealWith();
	switch(nSetcResult) {
	case SETCPROC_ERROR: //是设置码,但设置错误
		break;
	case SETCRPOC_SUCC: //是设置码,且设置成功
		break;
	case 3: //是设置码,处理成功了,而且需要保存参数
		if((!W_setcToNoSaveParam()) && W_setcToSaveParam() && X_paramSaveAsSysDefaultParam() == TRUE)
		{ W_setcSetSaveFlag(FALSE); }
		U_appSetResult(UIMG_DECODER_SETCODE);
		break;
	default:
		break;
	}//End of switch;

	if (nMacroResult == UIMG_DECODER_SETCODE|| nSetcResult == SETCPROC_ERROR ||
		nSetcResult == SETCRPOC_SUCC || nSetcResult == 3)
	{//设置码
		if (!(X_paramGetGlobalAllParam()->pCode->SetCode))
		{//不发送设置码
			U_appSetResult(UIMG_DECODER_SETCODE);
// 			U_appSetMsg(NULL,0); 
			U_appSetMsg(X_decoGetDecInfoBuff(),X_decoGetDecInfoLen());
			U_appSetMsgClrFlag(TRUE);
			U_appSetCategory(X_decoGetBarCode());
			U_appSetCodeType(X_decoGetCodeType());
		}//End of if;
		else
		{
			if (nSetcResult == 2)
			{ U_appSetResult(X_decoGetResult()); }//End of if;
			U_appSetMsg(X_decoGetDecInfoBuff(),X_decoGetDecInfoLen());
			U_appSetCategory(X_decoGetBarCode());
			U_appSetCodeType(X_decoGetCodeType());
		}//End of else;
	}//End of if;
// 	else if (nMacroResult == UIMG_DECODER_LENS_LIMIT)
// 	{//长度限制
// 		U_appSetResult(nMacroResult);
// 		U_appSetCategory(X_decoGetBarCode());
// 		U_appSetCodeType(X_decoGetCodeType());
// 		U_appSetMsg(NULL,0);
// 	}//End of if;
	else if (nMacroResult == UIMG_DECODER_SUC)
	{
		U_appSetResult(nMacroResult);
		U_appSetMsg(X_decoGetDecInfoBuff(),X_decoGetDecInfoLen());
		U_appSetCategory(X_decoGetBarCode());
		U_appSetCodeType(X_decoGetCodeType());
	}//End of if;
	else if (nMacroResult == UIMG_DECODER_MACRO)
	{
		U_appSetResult(nMacroResult);
		U_appSetCategory(X_decoGetBarCode());
		U_appSetCodeType(X_decoGetCodeType());
	}//End of if;
	else
	{
		U_appSetResult(nMacroResult);
		U_appSetMsg(NULL,0);
		U_appSetCategory(0xffff);
		U_appSetCodeType(0);
		return FALSE;
	}
	
	if (W_setcToNoSaveParam())
	{
		W_setcSetSaveFlag(FALSE);		
		W_setcSetNoSaveFlag(FALSE);		
	} 
	else if (W_setcToSaveParam())
	{
		X_paramSaveAsSysDefaultParam();
		W_setcSetSaveFlag(FALSE);
	}//End of if;


	if (U_appGetMsgClrFlag())
		U_appClearMsg();
	
	return TRUE;
}//End of function;

BOOL	U_appDecodeWithMacroNew(void)
{
	if(U_appDecodeWithMacro() == FALSE)
		return FALSE;
	if (X_paramGetGlobalAllParam()->pCode->nReadMode != ESC2)
	{//自动感应模式时，由外部控制声音
		U_appAlertStatus(U_appGetResult());
	}//End of if;

	return TRUE;
}

void	U_appSetUserFunc(UTGeneralParam *psParam)
{
	//后台工作
	Z_DogFeed();
	U_appBKGrndProc();

	if (psParam == NULL)
	{ return; }

	switch(psParam->Id) {
	case STID_IMGPARAM: //设置解码图像
		X_decoSetImgParam((UTImgParam*)psParam->psParamStruct);
		break;
	case STID_OCRSETUP: //设置OCR的解码参数
		X_decoSetOCRParam((UTDecOCRSetup*)psParam->psParamStruct);
		break;
	case STID_1DSETUP: //设置1D的解码参数
		X_decoSet1DParam((UTDec1DSetup*)psParam->psParamStruct);
		break;
	case STID_2DSETUP: //设置2D的解码参数
		X_decoSet2DParam((UTDec2DSetup*)psParam->psParamStruct);
		break;
	case STID_COMMEXTFNC: //设置通讯处理扩展功能
	case STID_ADDVERMSG: //增加版本信息
		V_commSetUserFunc(psParam);
		break;
	case STID_SETCEXTFNC: //设置设置码处理扩展功能
		W_setcSetUserFunc(psParam);
		break;
	case STID_NEWCODE: //设置新条码处理
	case STID_NEWCODEEXT:
		X_decoSetUserFunc(psParam);
		break;
	case STID_CMOSEXTFNC: //设置CMOS处理扩展功能
		Y_cmosSetUserFunc(psParam);
		break;
	case STID_PARAMEXTFUNC: //设置参数存取的扩展功能
		X_paramSetUserFunc(psParam);
		break;
	case STID_REPLACEIMG: //替代图像地址
	case STID_REPLACEKEY: //替代按键
		Z_SetUserFunc(psParam);
		break;
	case STID_UAPPEXTFNC: //U_app的扩展	
		if (psParam->psParamStruct != NULL)
		{
			g_fnc_ProcDecMsg = (FNC_uappProcDecMsg)((TUAPPExtFunc*)psParam->psParamStruct)->fnc_uappProcDecMsg;
		}//End of if;
		break;
	case STID_DECODE_NOTIFY:
		if(psParam->psParamStruct != NULL){
			memcpy(&g_fnc_app_decode_notify, psParam->psParamStruct, sizeof(UDECODE_NOTIFY));
		}
		break;
	default:
		break;
	}//End of switch;

	return;
}//End of function;

//进入休眠前需要做的事
void	U_appGoingToSleep(void)
{
	//后台工作
// 	Z_DogFeed();
// 	U_appBKGrndProc();

}//End of function;

//从休眠返回时需要做的事
void	U_appBackFromSleep(void)
{
	//后台工作
// 	Z_DogFeed();
// 	U_appBKGrndProc();

// 	Y_cmosInit();
// 	//重设为镜像
// 	uJ_HardTypeParam();
//	if (X_paramGetGlobalAllParam()->pHard->nProduction == PRODUCTION_MIRROR)
//	{ Y_cmosRowMirror(TRUE); }

}//End of function;


//读码成功的声音
void uIMG_vSucBeep(void)
{
	int n;
	int nFreq = 2730;
	int nDuration = 80;

	if (!(X_paramGetGlobalAllParam()->pHard->bDecVoice & 1))
	{ return; }

	if(IsSurePOSOpen())
	{
		if (!SurePOS_GetScannerState())
			return;
	}

	switch((X_paramGetGlobalAllParam()->pHard->nBeepType>>4) & 0x0f){
		case 0:
			n = 10;
			break;
		case 1:
			n = 4;
			break;
		case 2:
			n = 2;
			break;
		case 3:
			n = 0;
			break;
		default:
			n = 10;
			break;
	}

	//EM2037: bit15 - bit12	频率	0 = Medium; 1 = Lowest; 2 = Low; 3 = Medium; 4 = High; 9 = 用户定义
	//频率暂定为：800、 1600、 2730、 4200。
	switch((X_paramGetGlobalAllParam()->pHard->nBeepType>>12) & 0x0f){
	case 0:
		nFreq = 2730;
		break;
	case 1:
		nFreq = 800;
		break;
	case 2:
		nFreq = 1600;
		break;
	case 3:
		nFreq = 2730;
		break;
	case 4:
		nFreq = 4200;
		break;
	case 9:
		nFreq = X_paramGetGlobalAllParam()->pCode2->nSucBeepFreq;
		break;

	default:
		nFreq = 2730;
		break;
	}
	//EM2037: bit11 - bit8	持续时间 0 = Medium; 1 = short
	switch((X_paramGetGlobalAllParam()->pHard->nBeepType>>8) & 0x0f){
	case 0:
		nDuration = 80;
		break;
	case 1:
		nDuration = 40;
		break;
	case 9:
		nDuration = X_paramGetGlobalAllParam()->pCode2->nSucBeepDuration;
		break;
	default:
		nDuration = 80;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pHard->nBeepType & 0x0f){
		case 0:
			Z_Beep(nFreq,nDuration,n);
//			Z_PlayMusic((char*)"|7.4|");
			break;
		case 1:
			Z_PlayMusic_Ex((char*)"|+7.8|+6.8|", n);
			break;
		case 2:
			Z_PlayMusic_Ex((char*)"|1.8|6.8|+1.8|+7.8|", n);
			break;
		case 3:
			Z_PlayMusic_Ex((char*)"|1.4|3.4|5.4|+1.4|", n);
			break;
		case 4:
			Z_PlayMusic_Ex((char*)"|+6.8|+3.8|", n);
			break;
		case 5:
			Z_PlayMusic_Ex((char*)"|+6.8|+1.8|", n);
			break;
		case 6:
			Z_PlayMusic_Ex((char*)"|+5.8|+1.8|", n);
			break;
		case 7:
			Z_PlayMusic_Ex((char*)"|+5.8|+2.8|", n);
			break;
		case 8:
			Z_PlayMusic_Ex((char*)"|+5.8|+5.8|", n);
			break;
		case 9:
			Z_PlayMusic_Ex((char*)"|+7.8|", n);
			break;
		default:
			Z_PlayMusic_Ex((char*)"|+1.8|+2.8|+3.8|+4.8|+5.8|+6.8|+7.8|", n);
			break;
	}
}
//状态提醒
void U_appAlertStatus(int nResult)
{
	int n;
	if(_s_is_in_test_mode())
		return;

	switch((X_paramGetGlobalAllParam()->pHard->nBeepType>>4) & 0x0f){
		case 0:
			n = 10;
			break;
		case 1:
			n = 4;
			break;
		case 2:
			n = 2;
			break;
		default:
			n = 10;
			break;
	}

	switch(nResult){
	case UIMG_DECODER_SUC:
		if (X_paramGetGlobalAllParam()->pHard->bDecVoice & 1){
//			Z_PlayMusic((char*)"|7.4|");
			uIMG_vSucBeep();
		}
		if (X_paramGetGlobalAllParam()->pHard->bOscillation == TRUE
			&& X_paramGetGlobalAllParam()->pHard->nOscillationDelay > 0)
			Z_Oscillate(X_paramGetGlobalAllParam()->pHard->nOscillationDelay);
		break;
	case UIMG_DECODER_SETCODE:
		switch(W_setcGetResult()){
		case U_uIMG_STATUS_SETCODE_START:
			Z_PlayMusic_Ex((char*)SETCODE_START, n);
			break;
		case U_uIMG_STATUS_SETCODE_SUC:
			Z_PlayMusic_Ex((char*)SETCODE_OK, n);
			break;
		case U_uIMG_STATUS_SETCODE_END:
			Z_PlayMusic_Ex((char*)SETCODE_END, n);
			break;
		case U_uIMG_STATUS_SETCODE_ERROR:
		case U_uIMG_STATUS_SETCODE_UNKNOW:
		default:
			Z_PlayMusic_Ex((char*)SETCODE_ERROR, n);
			break;
		}
		break;
	case UIMG_DECODER_MACRO:
		switch(W_macroGetStatus()){
// 		case U_uIMG_STATUS_MACRO_START:
// 			Z_PlayMusic_Ex((char*)MACRO_START, n);
// 			break;
		case U_uIMG_STATUS_MACRO_END:
			Z_PlayMusic_Ex((char*)MACRO_END, n);
			break;
		case U_uIMG_STATUS_MACRO_SUC:	
			Z_PlayMusic_Ex((char*)MACRO_OK, n);
			break;
		case U_uIMG_STATUS_MACRO_ERROR:
		default:
			Z_PlayMusic_Ex((char*)MACRO_ERROR, n);
			break;
		}
		break;
	}
}//End of function;



BOOL isImg1DMinMaxLimit(int CodeType,int nInfo)
{
	int MaxLen = 0xffff,MinLen = 1;
	int nParam;
	
	MaxLen = 0x7f;
	MinLen = 1;
	nParam = X_paramGetGlobalAllParam()->p1D->ExMsg[X_decoGetBitsPosition(CodeType)];

	switch(CodeType) {
	case DC_CODE128:	
	case DC_MATRIX25:
	case DC_MODULE25:
	case DC_CODE93:
	case DC_CODEZ:
		MaxLen = (nParam & 0xff000000) >> 24;
		MinLen = (nParam & 0x00ff0000) >> 16;
		break;
	case DC_REGULAR39:
	case DC_CODABAR:
		MaxLen = (nParam & 0xff000000) >> 24;
		MinLen = (nParam & 0x00ff0000) >> 16;
		MaxLen += 2;  //加上起始符与终止符2个字符长度
		MinLen += 2;  //加上起始符与终止符2个字符长度
		break;
	case DC_INTER2OF5:
		MaxLen = (nParam & 0xff000000) >> 24;
		MinLen = (nParam & 0x00ff0000) >> 16;
		if (X_paramGetGlobalAllParam()->p1D->I2Of5Len[0] == 0) break;
		if ((nInfo < 18) &&
			(X_paramGetGlobalAllParam()->p1D->I2Of5Len[1] & (0x01 << (nInfo / 2 - 1))))
		{ return TRUE; }
		else if ((nInfo < 34) &&
			(X_paramGetGlobalAllParam()->p1D->I2Of5Len[2] & (0x01 << (nInfo / 2 - 9))))
		{ return TRUE; }
		else if ((nInfo < 50) &&
			(X_paramGetGlobalAllParam()->p1D->I2Of5Len[3] & (0x01 << (nInfo / 2 - 17))))
		{ return TRUE; }
		else if ((nInfo < 66) &&
			(X_paramGetGlobalAllParam()->p1D->I2Of5Len[4] & (0x01 << (nInfo / 2 - 25))))
		{ return TRUE; }
		return FALSE;
		break;
	default: 
		return TRUE;
		break;
	}
	if (MinLen == 0 && MaxLen == 0) return TRUE;
	if (nInfo < MinLen || nInfo > MaxLen) return FALSE;
	return TRUE;
}

BOOL isImg2DMinMaxLimit(int CodeType,int nInfo)
{
	int MaxLen,MinLen;
	int nParam;
	int nPos;
	nPos = X_decoGetBitsPosition(CodeType);

	if (nPos >= 10 || nPos < 0)  return FALSE;
	nParam = X_paramGetGlobalAllParam()->p2D->ExMsg[nPos];

	switch(CodeType){
	case DC_PDF417:
	case DC_QRCODE:
	case DC_DataMatrix:
	case DC_VeriCode:
	case DC_UN_VeriCode:
		MaxLen = (nParam & 0xffff0000) >> 16;
		MinLen = nParam & 0x0000ffff;
		break;
	default:
		return TRUE;
		break;
	}
	if (MinLen == 0 && MaxLen == 0) return TRUE;
	if (nInfo < MinLen || nInfo > MaxLen) return FALSE;
	return TRUE;
}

BOOL isImgOCRMinMaxLimit(int CodeType,int nInfo)
{
	int MaxLen,MinLen;

	int nParam;

	int nPos;
	nPos = X_decoGetBitsPosition(CodeType);

	if (nPos >= 10 || nPos < 0)  return FALSE;
	nParam = X_paramGetGlobalAllParam()->pOCR->ExMsg[nPos];

	switch(CodeType){
	case DC_SPEC_OCR_B:
		MaxLen = (nParam & 0xff000000) >> 24;
		MinLen = (nParam & 0x00ff0000) >> 16;
		break;
	default:
		return TRUE;
		break;
	}
	if (MinLen == 0 && MaxLen == 0) return TRUE;
	if (nInfo < MinLen || nInfo > MaxLen) return FALSE;

	return TRUE;
}//End of function;

//是否被按着键
BOOL	U_appKeyPressed(void)
{
	//后台工作
	Z_DogFeed();
	U_appBKGrndProc();

	if (Z_KeyPressed())
	{
		return TRUE;
	}//End of if;
	return FALSE;
}

void	U_appSleepMode(void)
{
	int nBreak;
	//后台工作
	U_appBKGrndProc();
	
	nBreak = Z_DogGetBreak();
	Z_DogReset(-1);

	U_appGoingToSleep();
	if (Z_sysSleep())
	{ U_appBackFromSleep(); } 

	Z_DogReset(nBreak);

	U_appBKGrndProc();	
}

//从休眠返回时需要做的事
void	U_appBackFromIdle(void)
{
	Y_cmosInit();
	//重设为镜像
	uJ_HardTypeParam();
//  	U_appBKGrndProc();

}//End of function;

int	U_appIdleMode(void)
{
	int nBreak;
	char data[256];
	int lvi = 0;
	
	//后台工作
	U_appBKGrndProc();
	
	nBreak = Z_DogGetBreak();
	Z_DogReset(-1);
	
	if (Z_sysIdle())
	{
		if (Z_KeyPressed())
		{
			U_appBackFromIdle(); 
			lvi = 1;
		}
		else if (Y_commBakReadChar(data, 0))
		{
			if (data[0] == '?')
			{
				Y_commWrite((BYTE*)"!",1);
				U_appBackFromIdle(); 
				lvi = 1;
			}
			else
			{
				do 
				{
					Y_commBakRead((BYTE*)data, 256, 0);
				} while(Y_commBakBuffCount()>0);
			}
		}
	} 
	
	Z_DogReset(nBreak);

	U_appBKGrndProc();

	return lvi;	
}

static BOOL _b_goto_idle = FALSE;
void U_appSetIdleMode(void)
{
	_b_goto_idle = TRUE;
}
int  CheckAndRunIdle(void)
{
	if (_b_goto_idle == TRUE)
	{
		if (U_appIdleMode() == 1)
		{
			_b_goto_idle = FALSE;
		}
		return 1;
	}
	return 0;
}

////////////////////

int	U_appStandbyMode(void)
{
	int nBreak;
	int lvi = 0;
	
	//后台工作
	U_appBKGrndProc();
	
	nBreak = Z_DogGetBreak();
	Z_DogReset(-1);
	
	if (Z_sysStandby())
	{
		U_appBackFromIdle(); 
		lvi = 1;
	} 
	
	Z_DogReset(nBreak);
	U_appBKGrndProc();
	return lvi;	
}

static BOOL _b_goto_standby = FALSE;
void U_appSetStandbyMode(void)
{
	_b_goto_standby = TRUE;
}
int  CheckAndRunStandby(void)
{
	if (_b_goto_standby == TRUE)
	{
		U_appStandbyMode();
		_b_goto_standby = FALSE;
		return 1;
	}
	return 0;
}
///////

void	U_appWillSleepTillWakeup(int nIdleMiliSec)
{
	//后台工作
	Z_DogFeed();
	U_appBKGrndProc();

	if (nIdleMiliSec < 0)
	{ nIdleMiliSec = 0; }	//允许解码成功就进入自动休眠

	if (Z_GetIdleCounter()*10 > nIdleMiliSec)
	{//没有按键则要检测是否需要休眠
		U_appSleepMode();
	}//End of if;
}//End of function;

//处理设置指令
BOOL	U_appDealCmd(BYTE *pCmdString, int nLen)
{
	BOOL bFlag;
	
	//后台工作
	Z_DogFeed();
	U_appBKGrndProc();

	if (pCmdString == NULL || nLen <= 3)
	{
		return FALSE;
	}//End of if;
	if ((pCmdString[0] != 'N' && pCmdString[0] != 'n') ||
		(pCmdString[1] != 'L' && pCmdString[1] != 'l') ||
		(pCmdString[2] != 'S' && pCmdString[2] != 's'))
	{//必须以 nls开头
		return FALSE;
	}//End of if;

	bFlag = FALSE;
	bFlag = W_setcParseBatch(pCmdString+3, nLen-3);
	if(bFlag)
	{
		if (W_setcToSaveParam())
		{
			X_paramSaveAsSysDefaultParam();
			W_setcSetSaveFlag(FALSE);
		}
	}//End of else;

	return bFlag;
}//End of function;

void	U_appQueryAPI(UTGeneralParam *psParam)
{
	TUserAPI sUserAPI = 
	{
		U_appSetMsg,
		U_appSetCategory,
		U_appSetCodeType,
		U_appSetResult,
		U_appGetMsgBuff,
		U_appGetMsgLen,
		U_appGetMsgBuffSize,
		U_appGetCategory,
		U_appGetCodeType,
		U_appGetResult,
		U_appCaptureImg,
		U_appProcDecode,
		U_appMsgEditting,
		U_appMsgPacking,
		U_appDealCmd,
		U_appSetUserFunc,
		U_appKeyPressed,
		U_appSleepMode,
		U_appWillSleepTillWakeup,
		U_appGoingToSleep,
		U_appBackFromSleep,
		U_appBKGrndProc,
		U_appQueryAPI,
		U_appDecodeWithMacro,
		U_appGetVersionInfo,
                       
		Y_commGetCurType,
		Y_commEnable,
		Y_commIsEnable,
		Y_commDisable,
		Y_commRead,
		Y_commWrite,
		Y_commClean,
		Y_commSendChar,
		Y_commReadChar,
		Y_commBuffCount,
                       
		Y_paramLoadAppBlock,
		Y_paramSaveAppBlock,
                         
		Y_cmosSetWantLux,
		Y_cmosMirror,
                         
		Z_Beep ,
		Z_PlayMusic,
		Z_usDelay,
		Z_msDelay,
		Z_Get10msCounter,
		Z_Oscillate,
		Z_DogFeed,
		Z_DogGetBreak,
		Z_DogReset,
		Z_LedOn ,
		Z_LedOff,
		Z_LightOn,
		Z_LightOff,
		Z_IRLightOn,
		Z_IRLightOff,
		Z_FocusOn,
		Z_FocusOff,
		Z_sysReset,
		Z_GetIdleCounter,
		Z_OscillateWithBeep,
		Z_DeepSleep,

		U_appPowerOnDisplay,
		U_appMsgAddTerminal,
		U_appDevicePowerOnBeep,
		U_appRunDecodeTest
	};

	TBaseAPI sBaseAPI =
	{
		Z_Beep,
		Z_PlayMusic,
		Z_usDelay,
		Z_msDelay,
		Z_CounterGet,
		Z_GetCounterDensity,
		Z_Get10msCounter,
		Z_Oscillate,
		Z_DogFeed,
		Z_DogGetBreak,
		Z_DogReset,
		Z_KeyPressed,
		Z_LedOn,
		Z_LedOff,
		Z_LightOn,
		Z_LightOff,
		Z_IRLightOn,
		Z_IRLightOff,
		Z_FocusOn,
		Z_FocusOff,
		Z_oem_GetUid,
		Z_oem_GetLicense,
		Z_oem_SetLicense,
		Z_oem_LicenseLock,
		Z_sysReset,
		Z_sysSleep,
		Z_GetIdleCounter,

		Z_param_Load,
		Z_param_Save,
		Z_cmos_GetI2C,
		Z_cmos_SetI2C,
		Z_cmos_Read,
		Z_cmos_GetIMGbuff,
		Z_cmos_GetType,
		
		Z_GetHardwareInfo,
		Z_DeepSleep
	};

	TDecoAPI sDecoAPI =
	{
		X_decoSetProcParamS,
		X_decoSetUserFunc,
		X_decoInitProcParam,
		X_decoGetUIMGVersion,
		X_decoImgProcMain,
		X_decoSetImgParam,
		X_decoGetDecInfoBuff,
		X_decoGetDecInfoLen,
		X_decoGetResult,
		X_decoIsMacroCode,
		X_decoGetMacroOrderFrom,
		X_decoGetMacroOrderTo,
		X_decoGetMacroTotalCnt,
		X_decoGetMacroIDLen,
		X_decoGetMacroID,
		X_decoGetDecOutMsg,
		X_decoGetBarCode,
		X_decoGetCodeType,
		X_decoGetCodePos,
		X_decoGetBitsPosition,
		X_decoSet2DParam,
		X_decoSet1DParam,
		X_decoSetOCRParam
	};

	TCmosAPI sCmosAPI = 
	{
		Y_cmosInit,
		Y_cmosCapture,
		Y_cmosGetWidth,
		Y_cmosGetHeight,
		Y_cmosGetDataAddr,
		Y_cmosSetWantLux,
		Y_cmosAdjust,
		Y_cmosCalcLux,
		Y_cmosMirror,
		Y_cmosSetUserFunc,
		Z_cmos_GetType,
		Z_cmos_GetI2C,
		Z_cmos_SetI2C,
		Z_cmos_Read,
		Z_cmos_GetIMGbuff
	};

	TParamAPI sParamAPI = 
	{
		X_paramIsAllParamInited,
		X_paramInitedAllParam,
		X_paramGetGlobalAllParam,
		X_paramLoadSysDefaultParam,
		X_paramLoadUsrDefaultParam,
		X_paramSaveAsSysDefaultParam,
		X_paramSaveAsUsrDefaultParam,
		X_paramLoadDeviceInfo,
		X_paramSaveDeviceInfo,
		X_paramOriginalInitAllParam,
		X_paramPlaceKeyword,
		X_paramQueryKeyword,
		X_paramAnalysisKeyword,
		X_paramGetKeywordsStruct,
		X_paramIsParamsRight,
		X_paramGetParamFromBuff,
		X_paramPushParamToBuff,
		X_paramSendParamInfo,
		X_paramUpdateToDevice,
		X_paramSelectUsefulCommType,
		Y_paramLoadSysDefault,
		Y_paramLoadUsrDefault,
		Y_paramLoadOEM,
		Y_paramLoadAppBlock,
		Y_paramSaveSysDefault,
		Y_paramSaveUsrDefault,
		Y_paramSaveOEM,
		Y_paramSaveAppBlock,
		Z_param_Load,
		Z_param_Save	
	};

	TMacroAPI sMacroAPI =
	{
		W_macroInitialParam,
		W_macroMain,
		W_macroGetParam,
		W_macroSetParam,
		W_macroGetBuff,
		W_macroGetBuffLen,
		W_macroGetError,
		W_macroSetError,
		W_macroGetStatus,
		W_macroGetUnmacthParam,
		W_macroGetFlag,
		W_macroSetFlag
	};

	TSetcAPI sSetcAPI = 
	{
		W_setcDealWith,
		W_setcSetUserFunc,
		W_setcParamInit,
		W_setcIsMacroCancel,
		W_setcGetResult,
		W_setcSetResult,
		W_setcToSaveParam,
		W_setcSetSaveFlag,
		W_setcEnable,
		W_setcIsEnable,
		W_setcParseSigle,
		W_setcParseBatch
	};

	TCommAPI sCommAPI = 
	{
		V_commAutoCheckCmd,
		V_commSetUserFunc,
		Y_commGetCurType,
		Y_commEnable,
		Y_commIsEnable,
		Y_commDisable,
		Y_commChgToRS232,
		Y_commChgToUSBDataPipe,
		Y_commChgToKBW,
		Y_commGetBaudrate,
		Y_commGetKBWCountry,
		Y_commRead,
		Y_commWrite,
		Y_commClean,
		Y_commSendChar,
		Y_commReadChar,
		Y_commBuffCount,
		Y_commBakRead,
		Y_commBakReadChar,
		Y_commBakBuffCount,
		Z_comm_GetType,
		Z_comm_Read,
		Z_comm_Write,
		Z_comm_Config,
		Z_comm_GetStatue
	};

	TAppAPI sAppAPI = 
	{
		U_appSetMsg,
		U_appSetCategory,
		U_appSetCodeType,
		U_appSetResult,
		U_appGetMsgBuff,
		U_appGetMsgLen,
		U_appGetMsgBuffSize,
		U_appGetCategory,
		U_appGetCodeType,
		U_appGetResult,
		U_appCaptureImg,
		U_appProcDecode,
		U_appMsgEditting,
		U_appMsgPacking,
		U_appDealCmd,
		U_appSetUserFunc,
		U_appKeyPressed,
		U_appSleepMode,
		U_appWillSleepTillWakeup,
		U_appGoingToSleep,
		U_appBackFromSleep,
		U_appBKGrndProc,
		U_appQueryAPI,
		U_appDecodeWithMacroNew,
		U_appGetVersionInfo,
		U_appPowerOnDisplay,
		U_appMsgAddTerminal
	};

	TImgProcAPI sImgPorcAPI = { NULL};

	if (psParam == NULL || psParam->psParamStruct == NULL)
	{ return; }//End of if;

	if (psParam->Id == STID_QUERYUSRAPI)
	{ memcpy(psParam->psParamStruct,&sUserAPI,sizeof(TUserAPI)); }
	if (psParam->Id == STID_QUERYBASE)
	{ memcpy(psParam->psParamStruct,&sBaseAPI,sizeof(TBaseAPI)); }
	if (psParam->Id == STID_QUERYDECOD)
	{ memcpy(psParam->psParamStruct,&sDecoAPI,sizeof(TDecoAPI)); }
	if (psParam->Id == STID_QUERYCMOS)
	{ memcpy(psParam->psParamStruct,&sCmosAPI,sizeof(TCmosAPI)); }
	if (psParam->Id == STID_QUERYPARAM)
	{ memcpy(psParam->psParamStruct,&sParamAPI,sizeof(TParamAPI)); }
	if (psParam->Id == STID_QUERYMACRO)
	{ memcpy(psParam->psParamStruct,&sMacroAPI,sizeof(TMacroAPI)); }
	if (psParam->Id == STID_QUERYSETC)
	{ memcpy(psParam->psParamStruct,&sSetcAPI,sizeof(TSetcAPI)); }
	if (psParam->Id == STID_QUERYCOMM)
	{ memcpy(psParam->psParamStruct,&sCommAPI,sizeof(TCommAPI)); }
	if (psParam->Id == STID_QUERYAPP)
	{ memcpy(psParam->psParamStruct,&sAppAPI,sizeof(TAppAPI)); }

	if (psParam->Id == STID_QUERYIMGPROC)
	{
		X_decoQueryAPI(&sImgPorcAPI);
		memcpy(psParam->psParamStruct,&sImgPorcAPI,sizeof(TImgProcAPI)); 
	}


}//End of function;


//初始化参数
void vIniParam(void)
{
	UALLPARAM *psParam;

	psParam = X_paramGetGlobalAllParam();

	psParam->pMode->nReadDelay = Z_Get10msCounter();
	if ((ESC3 == psParam->pCode->nReadMode) || (ESC2 == psParam->pCode->nReadMode)) 
	{//自动模式及连续模式下需要初始化时间标记
		psParam->pMode->nHaltDelay = Z_Get10msCounter();
		psParam->pMode->nReadDelay = Z_Get10msCounter();
		psParam->pMode->nCodeDelay = Z_Get10msCounter();
	}//End of if;
	psParam->pMode->bContinue = TRUE;

	OldMsg.pMes = g_pReInfoBackUp;
	OldMsg.nCodeType = 0;
	OldMsg.nBarCode = 0;
	memset(g_pReInfoBackUp,0,sizeof(g_pReInfoBackUp));

	psParam->pCode->bIR = FALSE; // add by tiercel 2004.05.24
	psParam->pMode->bDebug = FALSE; //禁止调试信息发送
	psParam->pMode->bDetectHalt = FALSE;
	psParam->pMode->bIRSign = FALSE;
	psParam->pMode->n1Want = -1;
}

//处理对焦灯/照明灯设置
void T_LightFocusCtrl(int nID)
{
/*	int nLightMode,nFocusMode, nExtLightMode;
	UALLPARAM *psParam;

	psParam = X_paramGetGlobalAllParam();
	nLightMode = psParam->pHard->nLight;
	nFocusMode = psParam->pHard->nFocus;
// 	nExtLightMode = psParam->pHard->nExLight;
// 
// 	switch(nExtLightMode){
// 	case 0:		Z_ExtLightCtrl(FALSE); break;
// 	case 1:		Z_ExtLightCtrl(TRUE); break;
// 	}

	switch(nID){
	case	FL_EXTCYC_NO_READ:
		switch(nLightMode){
			case LIGHT_MODE1: Z_LightOff(); break; //普通模式 : 读码闪烁模式
			case LIGHT_MODE2: Z_LightOn(); break;  //保持常开
			case LIGHT_MODE3: Z_LightOff(); break; //全局关闭
			case LIGHT_MODE4: Z_LightOff(); break;  //感应识别
			case LIGHT_MODE5:	break;				//用户控制
		}
		switch(nFocusMode){
			case FOCUS_MODE1: Z_FocusOff(); break; //普通模式 : 读码闪烁模式
			case FOCUS_MODE2: Z_FocusOn(); break;  //保持常开
			case FOCUS_MODE3: Z_FocusOff(); break; //全局关闭
			case FOCUS_MODE4: Z_FocusOn(); break; //感应识别
			case FOCUS_MODE5:	break;				//用户控制
		}
		break;
	case	FL_PRE_GET_CMOS: //获取解码的CMOS数据前
		switch(nLightMode){
			case LIGHT_MODE1: Z_LightOn(); break;
			case LIGHT_MODE2: Z_LightOn(); break;
			case LIGHT_MODE3: Z_LightOff(); break;
			case LIGHT_MODE4: Z_LightOn(); break;   //感应识别
			case LIGHT_MODE5:	break;				//用户控制
		}
		switch(nFocusMode){
			case FOCUS_MODE1: Z_FocusOff(); break;
			case FOCUS_MODE2: Z_FocusOff(); break; //常亮模式也要关掉,否则影响识读
			case FOCUS_MODE3: Z_FocusOff(); break;
			//case FOCUS_MODE4: Z_FocusOn();  break; //感应识别
			case FOCUS_MODE4: Z_FocusOn(); break;
			case FOCUS_MODE5:	break;				//用户控制
		}
		break;
// 	case	FL_SITH_GET_CMOS: //获取解码的CMOS数据后，即，开始图像处理前
// 		switch(nLightMode){
// 			case LIGHT_MODE1: Z_LightOff(); break;
// 			case LIGHT_MODE2: Z_LightOn(); break;
// 			case LIGHT_MODE3: Z_LightOff(); break;
// 			case LIGHT_MODE4: Z_LightOn(); break;  //感应识别
// 			case LIGHT_MODE5:	break;				//用户控制
// 		}
// 		switch(nFocusMode){
// 			case FOCUS_MODE1: Z_FocusOn(); break;
// #ifdef MODULE_FOR_C_Y_CHAIN
// 			case FOCUS_MODE2: Z_FocusOff(); break;
// #else
// 			case FOCUS_MODE2: Z_FocusOn(); break;
// #endif
// 			case FOCUS_MODE3: Z_FocusOff(); break;
// 			//case FOCUS_MODE4: Z_FocusOn(); break; //感应识别
// 			case FOCUS_MODE4: Z_FocusOff(); break; //感应识别
// 			case FOCUS_MODE5:	break;				//用户控制
// 		}
// 		break;
	case	FL_SITH_DECODER: //图像处理后
		switch(nLightMode){
			case LIGHT_MODE1: Z_LightOff(); break;
			case LIGHT_MODE2: Z_LightOn(); break;
			case LIGHT_MODE3: Z_LightOff(); break;
			case LIGHT_MODE4: Z_LightOn(); break;  //感应识别
			case LIGHT_MODE5:	break;				//用户控制
		}
		switch(nFocusMode){
			case FOCUS_MODE1: Z_FocusOff(); break;
			case FOCUS_MODE2: Z_FocusOn(); break;
			case FOCUS_MODE3: Z_FocusOff(); break;
			//case FOCUS_MODE4: Z_FocusOn(); break; //感应识别
			case FOCUS_MODE4: Z_FocusOff(); break; //感应识别
			case FOCUS_MODE5:	break;				//用户控制
		}
		break;
	case  FL_DECODER_SUCC: //解码成功后
		switch(nLightMode){
			case LIGHT_MODE1: Z_LightOff(); break;
			case LIGHT_MODE2: Z_LightOn(); break;
			case LIGHT_MODE3: Z_LightOff(); break;
			case LIGHT_MODE4: Z_LightOff(); break;  //感应识别
			case LIGHT_MODE5:	break;				//用户控制
		}
		switch(nFocusMode){
			case FOCUS_MODE1: Z_FocusOff(); break;
			case FOCUS_MODE2: Z_FocusOn(); break;
			case FOCUS_MODE3: Z_FocusOff(); break;
			case FOCUS_MODE4: Z_FocusOn(); break; //感应识别
			case FOCUS_MODE5:	break;				//用户控制
		}
		break;
	case	FL_SENSOR_CMOS:  //进入图像感应模式前
		switch(nLightMode){
			case LIGHT_MODE1: Z_LightOff(); break;
			case LIGHT_MODE2: Z_LightOn(); break;
			case LIGHT_MODE3: Z_LightOff(); break;
			case LIGHT_MODE4: Z_LightOff(); break;  //感应识别
			case LIGHT_MODE5:	break;				//用户控制
		}
		switch(nFocusMode){
			case FOCUS_MODE1: Z_FocusOff(); break;
			case FOCUS_MODE2: Z_FocusOn(); break;
			case FOCUS_MODE3: Z_FocusOff(); break;
			case FOCUS_MODE4: Z_FocusOn(); break; //感应识别
			case FOCUS_MODE5:	break;				//用户控制
		}
		break;
// 	case	FL_SEND_IMG_DATA: //往串口/USB口传输图像前
// 		switch(nLightMode){
// 			case LIGHT_MODE1: Z_LightOff(); break;
// 			case LIGHT_MODE2: Z_LightOn(); break;
// 			case LIGHT_MODE3: Z_LightOff(); break;
// 			case LIGHT_MODE4: Z_LightOff(); break;  //感应识别
// 			case LIGHT_MODE5:	break;				//用户控制
// 		}
// 		switch(nFocusMode){
// 			case FOCUS_MODE1: Z_FocusOff(); break;
// 			case FOCUS_MODE2: Z_FocusOn(); break;
// 			case FOCUS_MODE3: Z_FocusOff(); break;
// 			case FOCUS_MODE4: Z_FocusOff(); break; //感应识别
// 			case FOCUS_MODE5:	break;				//用户控制
// 		}
// 		break;
	case	FL_KEY_OVERTIME://摁键超时
		switch(nLightMode){
			case LIGHT_MODE1: Z_LightOff(); break;
			case LIGHT_MODE2: Z_LightOff(); break;
			case LIGHT_MODE3: Z_LightOff(); break;
			case LIGHT_MODE4: Z_LightOff(); break;  //感应识别
			case LIGHT_MODE5:	break;				//用户控制
		}
		switch(nFocusMode){
			case FOCUS_MODE1: Z_FocusOff(); break;
			case FOCUS_MODE2: Z_FocusOff(); break;
			case FOCUS_MODE3: Z_FocusOff(); break;
			case FOCUS_MODE4: Z_FocusOff(); break; //感应识别
			case FOCUS_MODE5:	break;				//用户控制
		}
		break;
	case FL_AUTO_SLP_BEF: //自动模式下休眠前
		switch(nFocusMode){
		case FOCUS_MODE4: Z_FocusOn(); break; //感应识别
			case FOCUS_MODE5:	break;				//用户控制
		}
		break;
	case FL_AUTO_SLP_AFT:	//自动模式下休眠唤醒后
		switch(nFocusMode){
			case FOCUS_MODE4: Z_FocusOn(); break; //感应识别
			case FOCUS_MODE5:	break;				//用户控制
		}
		break;
	}
*/	

}



//nMode = 0  一次识读，超时或读到条码停止
//nMode = 1  一次识读，读到条码停止
//nMode = -1 停止识读
int U_appSetDecodeMode(int nMode)
{
	UALLPARAM *psParam = X_paramGetGlobalAllParam();
	switch(nMode)
	{
	case 0:
		psParam->pCode->nReadMode = ESC1;
		psParam->pMode->nReadDelay = Z_Get10msCounter();
		break;
	case 1:
		break;
	case -1:
		psParam->pCode->nReadMode = ESC0;
		break;
	}
	return 0;
}


//#include <pxa-regs.h>

//根据模式及当前状态决定是否需要读码
BOOL IsModeToReadCode()
{
	BOOL bToRead;
	UALLPARAM *psParam;
	UTImgParam sImg;
	int nRealOnceDelay = 0;
	static int nLastRead = 0;
	static BOOL bESC2Flag = TRUE;
	int nGotoSleepDelay = 300;	//读码成功就自动休眠
	int tmp;

// 	BYTE byMsg[1024];
// 	unsigned int nLen;

	psParam = X_paramGetGlobalAllParam();

	bToRead = FALSE;
	//处理通讯指令,有可能为切换模式
	U_appBKGrndProc();

	nRealOnceDelay = psParam->pCode->nOnceDelay;
/*	if (nRealOnceDelay < 100)
	{ nRealOnceDelay = 200; }*/

	//避免枚举过程灯亮
	if ((psParam->pHard->InterType == U_PRO_IS_SurePOS_TT) ||
		(psParam->pHard->InterType == U_PRO_IS_SurePOS_TT))
	{
		if (SurePOS_GetScannerState() == FALSE)
		{
			if (Z_Get10msCounter()<150)
			{
				tmp = Z_GetCmosLightMode();
				tmp = (tmp & (~0xffffff)) | 0x020202;
				Z_SetCmosLightMode(tmp);
			}

		}
	}		
	
	if(IsSurePOSOpen())
	{
		if (SurePOS_GetScannerState() == FALSE)
		{
			tmp = Z_GetCmosLightMode();
			tmp = (tmp & (~0xffffff)) | 0x020202;
			Z_SetCmosLightMode(tmp);

			if (bCaptureRun)
				return FALSE;
			
			U_appWillSleepTillWakeup(0); //若已有0ms内没有操作,则休眠
			return FALSE;
		}
	}

	if (SurePOS_TickleFeed())
	{
		return FALSE;
	}

	if (U_GetScanEnable() == FALSE)
	{
// 		U_appWillSleepTillWakeup(nGotoSleepDelay); //若已有3000ms内没有操作,则休眠 //读码成功就自动休眠
		Z_WakeupTimer(200);
		U_appSleepMode();
		psParam->pMode->nReadDelay = Z_Get10msCounter();

		return FALSE;
	}


	switch(psParam->pCode->nReadMode) {
	case ESC0: //静默模式,接受按键
	
		if (Z_KeyPressed()) //有按键,进入识读
		{
			if(!_sTrigReadFlag)
				bToRead = TRUE;
		}//End of if;
		else
		{

			if(_sTrigReadFlag)
			{
				nLastRead = Z_Get10msCounter();
				_sTrigReadFlag = 0;
			}

			if (bCaptureRun)
				break;
			U_appWillSleepTillWakeup(nGotoSleepDelay); //若已有3000ms内没有操作,则休眠 //读码成功就自动休眠
		}//End of else;
		break;
	case ESC1: //单触发模式

		if ((nRealOnceDelay==0)||(Z_Get10msCounter()-psParam->pMode->nReadDelay)*10 < nRealOnceDelay)
		{//在超时限制内,可进行识读
			bToRead = TRUE;
		}//End of if;
		else //超时了,转为ESC0模式
		{
			psParam->pCode->nReadMode = ESC0; 
			T_LightFocusCtrl(FL_KEY_OVERTIME);
			if(g_fnc_app_decode_notify.Failed_or_Timeout != NULL)
			{ g_fnc_app_decode_notify.Failed_or_Timeout(); }
		}
		break;
	case ESC2: //自动模式
		//////////////////////////////////////////////////////////////////////////

		if (nRealOnceDelay < 500){
			//不停读码
			bToRead = TRUE;
			bESC2Flag = TRUE;
			break;
		}
		if ((Z_Get10msCounter()-psParam->pMode->nReadDelay)*10 <= nRealOnceDelay 
			&& psParam->pMode->nCodeDelay <= psParam->pMode->nReadDelay){
			//距离开始识读时刻,在限定间隔内,则继续识读
//			nLen = sprintf((char*)byMsg,"Cur = %d, nReadDelay = %d, OnceDelay = %d",Z_Get10msCounter(),psParam->pMode->nReadDelay,nRealOnceDelay);
//			Y_commWrite(byMsg,nLen);
			bToRead = TRUE;
			bESC2Flag = TRUE;

		}//End of if;
		else
		{
			if (bESC2Flag)
			{
				bESC2Flag = FALSE;
				nLastRead = Z_Get10msCounter();

			}//End of if;

			if (bCaptureRun)
				break;

			if ((Z_Get10msCounter()-nLastRead)*10 > 100)
			{//距离读出条码有200ms的间隔,则进入自动侦测状态
				//属于侦测状态
				capture_count++;
				Y_cmosCapture();
				sImg.pSrc = (BYTE*)Y_cmosGetDataAddr();
				sImg.nWidth = Y_cmosGetWidth();
				sImg.nHeight = Y_cmosGetHeight();
				sImg.pTag = NULL;


// 				if (X_decoEjectFromSenseMode(&sImg,psParam->pCode->nSensitive))//这边要判断是否退出侦测状态
				if ((!Y_cmosGetWidth()) || (!Y_cmosGetHeight()))
				{
					bToRead = FALSE;
					bESC2Flag = FALSE;
				}
				else if (GXIMG_IsTwoIMGSimilitudeExt((BYTE*)Y_cmosGetDataAddr(), Y_cmosGetWidth(),Y_cmosGetHeight(), 1)<0)//这边要判断是否退出侦测状态
				{//如果退出侦测状态,则开始进入识读
					psParam->pMode->nReadDelay = Z_Get10msCounter();
					if ((Z_Get10msCounter()-nLastRead)*10 > 700 && psParam->pCode->IsCompDelay != TRUE)
					{ OldMsg.nMes = 0; }
					bToRead = TRUE;
					bESC2Flag = TRUE;

				}//End of if;
				else
				{
					Y_cmosAdjust();

// 					X_decoSetImgForSenseMode(&sImg);
					if (Z_KeyPressed())
					{
						bToRead = TRUE;
						bESC2Flag = TRUE;
						psParam->pMode->nReadDelay = Z_Get10msCounter();
					}//End of if;
					else
					{//休眠200ms
						T_LightFocusCtrl(FL_AUTO_SLP_BEF);
//						Z_WakeupTimer(200);
 						Z_WakeupTimer(100);
						U_appSleepMode();
						T_LightFocusCtrl(FL_AUTO_SLP_AFT);
					}//End of else;

				}//End of else;
			}//End of if;
			else
			{//距离读出条码在100ms间隔内,则需要适应场景,准备进入侦测状态
				//适应场景
				capture_count++;
				Y_cmosCapture();
				Y_cmosAdjust();

				sImg.pSrc = (BYTE*)Y_cmosGetDataAddr();
				sImg.nWidth = Y_cmosGetWidth();
				sImg.nHeight = Y_cmosGetHeight();
				sImg.pTag = NULL;
// 				X_decoSetImgForSenseMode(&sImg);
				if (Y_cmosGetWidth() && Y_cmosGetHeight())
					GXIMG_IsTwoIMGSimilitudeExt((BYTE*)Y_cmosGetDataAddr(), Y_cmosGetWidth(),Y_cmosGetHeight(), 0);
// 				Z_WakeupTimer(50);
// 				U_appSleepMode();
			}//End of else;
		}//End of else;
		break;
	case ESC3: //连续模式

		if (Z_KeyPressed()) //有按键,则切换为暂停识读状态
		{
			psParam->pMode->bContinue = (psParam->pMode->bContinue == FALSE);
			while (Z_KeyPressed())
			{ U_appBKGrndProc(); }
		}//End of if;

		if (TRUE == psParam->pMode->bContinue)
		{
			bToRead = TRUE;
		}// 需要进行识读
		else if(FALSE == psParam->pMode->bContinue)
		{//暂停读码,则要判断休眠
			if (bCaptureRun)
				break;

			U_appWillSleepTillWakeup(nGotoSleepDelay); //若已有0ms内没有操作,则休眠
		}
		break;
	default:
		break;
	}//End of switch;

	return bToRead;
}//End of function;

//自动检测模式下,测试是否可以进行读码
//add by zencg
//SetReadOkFlag = 0, 检查当前是否可以进行读码
//              = 1, 设置当前刚刚读码成功,还需要继续一段时间的读码
int U_appGetSenseImgChkEx(int SetReadOkFlag)
{
#define EN_READ_TICKS	300		//允许读码的ticks
	int lvret = 0;
	static unsigned int _s_SenseChk_old_tick = 0;
	unsigned int lvtick;
	UALLPARAM *psParam = X_paramGetGlobalAllParam();
	UTImgParam sImg;

	lvtick = Z_Get10msCounter();
	if(SetReadOkFlag)
	{
		_s_SenseChk_old_tick = lvtick;
		return 1;
	}
	if(!_s_SenseChk_old_tick)
		_s_SenseChk_old_tick = lvtick;
	if((lvtick - _s_SenseChk_old_tick) < EN_READ_TICKS)	//离开始识读不到3000毫秒,就继续读码
	{
		return 1;
	}

	Y_cmosCapture();
	sImg.pSrc = (BYTE*)Y_cmosGetDataAddr();
	sImg.nWidth = Y_cmosGetWidth();
	sImg.nHeight = Y_cmosGetHeight();
	sImg.pTag = NULL;

	if((lvtick - _s_SenseChk_old_tick) < (EN_READ_TICKS + 20))		//距离读码结束200毫秒内对曝光参数进行调整,在此期间不允许读码
	{
		Y_cmosAdjust();
		X_decoSetImgForSenseMode(&sImg);
		return 0;
	}

	if (X_decoEjectFromSenseMode(&sImg,psParam->pCode->nSensitive))//这边要判断是否退出侦测状态
	{//如果退出侦测状态,则开始进入识读
		lvret = 1;
		_s_SenseChk_old_tick = Z_Get10msCounter();
	}
	else
	{
		Y_cmosAdjust();
		X_decoSetImgForSenseMode(&sImg);
		if(Z_KeyPressed())
		{
			lvret = 1;
			_s_SenseChk_old_tick = Z_Get10msCounter();
		}
		else
		{

//			Z_WakeupTimer(200);
//			U_appSleepMode();
		}
	}
	return lvret;
}

int U_appGetSenseImgChk(void)
{
	return U_appGetSenseImgChkEx(0);
}

void GoodReadToMode(BOOL bSucc)
{
	UALLPARAM *psParam;
	psParam = X_paramGetGlobalAllParam();

	switch(psParam->pCode->nReadMode) {
	case ESC0: //模式ESC0时,识读不影响
		break;
	case ESC1: //单触发时,应为切换到模式ESC0
		if (TRUE == bSucc)
		{ psParam->pCode->nReadMode = ESC0; }
		break;
	case ESC2: //自动模式
		//记录解码成功的时刻,以比对等待间隔
		if (TRUE == bSucc)
		{
			psParam->pMode->nCodeDelay = Z_Get10msCounter();
//			psParam->pMode->nReadDelay = Z_Get10msCounter();
		}
		break;
	case ESC3: //连续模式,识读不影响
		//自动休眠并唤醒
		if (TRUE == bSucc)
		{
			psParam->pMode->nCodeDelay = Z_Get10msCounter();
#ifndef BUILD_FAST
// 			Z_WakeupTimer(300); //300ms定时唤醒
// 			U_appSleepMode(); //休眠,等待被唤醒
#endif
		}
		break;
	default:
		break;
	}//End of switch;
}//End of function;


//判断解码后的信息是否可以用于发送
int bGoodDecode(void)
{
	int i;
	//	static int nFailCnt = 0; //连续失败次数的计数
	UALLPARAM *psParam;
	
	psParam = X_paramGetGlobalAllParam();
	//记录连续失败次数
	//	if (nResult != TRUE)
	//	{ nFailCnt++; }
	
	//自动模式ESC2、连续模式
	if ((psParam->pCode->nReadMode == ESC2) || (psParam->pCode->nReadMode == ESC3))
	{
		//识读的码与上一次不一样时,直接发送
		i = X_decoGetDecInfoLen();
		if (i > sizeof(OldMsg.pMes)) i = sizeof(OldMsg.pMes);
		
		if (OldMsg.nBarCode != X_decoGetBarCode()    // 不同大类型
			|| OldMsg.nCodeType != X_decoGetCodeType()  //不同类型
			|| OldMsg.nMes != X_decoGetDecInfoLen()         //不同长度
			|| strncmp((const char*)OldMsg.pMes,(const char*)X_decoGetDecInfoBuff(),i) != 0 //信息不一样
			)
		{
			OldMsg.nCodeType = X_decoGetCodeType(); //详细条码类型
			OldMsg.nBarCode = X_decoGetBarCode();   //条码大类型 1D: 0; 2D: 1; OCR: 2
			OldMsg.nMes = X_decoGetDecInfoLen();
			memcpy(OldMsg.pMes,X_decoGetDecInfoBuff(),i);
			//			nFailCnt = 0;
			if (U_appGetMsgClrFlag())
				U_appClearMsg();
			return TRUE;
		}
		
		//识读的码与上一次一样时,根据设定进行过滤
		if (psParam->pCode->IsCompDelay) 
		{//未达到要求完全延迟
			if ((Z_Get10msCounter() - psParam->pMode->nCodeDelay)*10 < (int)psParam->pCode->nInfoDelay)
			{
				if ((X_decoGetBarCode() == UT_TYPE_1D) && (X_decoGetCodeType() == DC_SETUP128
					|| X_decoGetCodeType() == DC_ZASETUP))
				{
					if ((Z_Get10msCounter() - psParam->pMode->nCodeDelay)*10 < 3000)	//设置码限定相同读码延迟最大3000ms
						return FALSE;
				}
				else
				{
					return FALSE; 
				}
			}
		}//End of if;
		// 		else
		// 		{
		// 			if (nFailCnt < 3 && (Z_Get10msCounter() - psParam->pMode->nCodeDelay)*10 < (int)psParam->pCode->nInfoDelay)
		// 			{ return FALSE; }
		// 		}//End of else;
		
		
		//信息可以发送
		OldMsg.nCodeType = X_decoGetCodeType(); //详细条码类型
		OldMsg.nBarCode = X_decoGetBarCode();   //条码大类型 1D: 0; 2D: 1; OCR: 2
		OldMsg.nMes = X_decoGetDecInfoLen();
		memcpy(OldMsg.pMes,X_decoGetDecInfoBuff(),i);
		//		nFailCnt = 0;
		// 		return TRUE;
	}//End of if;
	///////////////////
	
	return TRUE;
}

// #define LUXMULTIPLE		4
// 
// char *HightLight(void)
// {
// 	int i,size;
// 	char max = 0xff/LUXMULTIPLE;
// 	char *data = (char*)Z_cmos_GetIMGbuff_Ex();
// 
// 	size = Y_cmosGetWidth()*Y_cmosGetHeight();
// 	for (i=0; i<size; i++)
// 	{
// 		if (data[i]>max)
// 			data[i] = 0xff;
// 		else
// 			data[i] = data[i]*LUXMULTIPLE;
// 	}
// 
// 	return data;
// }






BOOL GetCaptureStatue(void)
{
	return bCaptureRun;
}

void NCR_Get_New_Img(void)
{
#if 0
	if (!bCaptureRun)
	{
		Z_cmos_SetI2C(0xBD, exposure_line/*EXPOSURE_MAX_WIDTH*/);
		Z_cmos_SetI2C(0x35, EXPOSURE_AG);
		Y_cmosCapture_Ex(CAPTURE_START, 1);		//启动拍图，并等待拍图结束
		Y_cmosCapture_Ex(CAPTURE_WAIT, 1);		//不启动拍图，但等待拍图结束

		Z_cmos_SetI2C(0xBD, min(Z_cmos_GetI2C(0xbb),0x28)); 
		Y_cmosCapture_Ex(CAPTURE_STOP, 1);		//20FPS模式下退出读码模式时候，设置停止拍图
	}
	else
	{
		Y_cmosCapture_Ex(CAPTURE_WAIT, 1);		//不启动拍图，但等待拍图结束
	}
#endif
}

void SetExprosureMode(void)
{
// 	if (X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode == 0)	//纸质码mode
// 	{
// 		Z_cmos_SetI2C(0xBD, exposure_line/*EXPOSURE_MAX_WIDTH*/);
// 		Z_cmos_SetI2C(0x35, EXPOSURE_AG);
// 		Z_cmos_SetI2C(0x06, 45);
// 		Z_cmos_SetI2C(0x80, EXPOSURE_DG);
// 		
// 	}
// 	else if (X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode == 2)	//phone mode2(照明灯亮灭交替)
// 		
// 	{
// 		Z_cmos_SetI2C(0xBD, 240/*exposure_line*//*EXPOSURE_MAX_WIDTH*/);
// 		//			Z_cmos_SetI2C(0xBD, exposure_line/*EXPOSURE_MAX_WIDTH*/);
// 		Z_cmos_SetI2C(0x35, 0x20/*EXPOSURE_AG*/);
// 		// 				Z_cmos_SetI2C(0x35, 0x20/*EXPOSURE_AG*/);
// 		// 			Z_cmos_SetI2C(0x80, 0xf6/*EXPOSURE_DG*/);
// 		Z_cmos_SetI2C(0x06, 45);
// 		Z_cmos_SetI2C(0x80, EXPOSURE_DG);
// 		
// 	}
// 	else if (X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode == 3)	//纸质码 mode 2, 照顾纸质码，景深好，能读手机码
// 	{
// 		Z_cmos_SetI2C(0xBD, 240/*exposure_line*//*EXPOSURE_MAX_WIDTH*/);
// 		Z_cmos_SetI2C(0x35, 0x10/*EXPOSURE_AG*/);
// 		Z_cmos_SetI2C(0x06, 45);
// 		Z_cmos_SetI2C(0x80, EXPOSURE_DG);
// 	}		
// 	else if (X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode == 4)	//phone mode1
// 	{
// 		//old--->
// 		// 				Z_cmos_SetI2C(0xBD, 240/*exposure_line*//*EXPOSURE_MAX_WIDTH*/);
// 		// 				Z_cmos_SetI2C(0x35, 0x20/*EXPOSURE_AG*/);
// 		
// 		//new--->
// 		Z_cmos_SetI2C(0xBD, 800);
// 		Z_cmos_SetI2C(0x35, 0x20/*EXPOSURE_AG*/);
// 		Z_cmos_SetI2C(0x06, 800+45-480);
// 		Z_cmos_SetI2C(0x80, 0xf8/*EXPOSURE_DG*/);
// 	}
// 
}

UTImgParam * U_appChkToReadAndCapture(BOOL FirstFlag, int nDevMode)
{
	static BOOL bInited = FALSE;
	static UTImgParam _s_Img;

	if (bInited != TRUE)
	{
		U_appBKGrndProc();
		bInited = TRUE;
	}//End of if;

	if (FirstFlag)
	{
#if FM410_CAPTURE_ENABLE
		Y_commSendString("FL_EXTCYC\r\n");
#endif
		T_LightFocusCtrl(FL_EXTCYC_NO_READ);
	}

	if (!IsModeToReadCode())		//判断是否可以读码
	{
		return NULL;
	}

	capture_count++;

	//拍图
#if FM410_CAPTURE_ENABLE
	Y_commSendString("Went to read.\r\n");
#endif
 	
	if(FirstFlag)
	{

#if FM410_CAPTURE_ENABLE
		Y_commSendString("FirstFlag\r\n");
#endif
		bCaptureRun = TRUE;

// 		Z_SetLightOnDuration(exposure_line);

// 		SetExprosureMode();
			
// 		Z_SetPM_EX(FREQ_416);

//		Y_commSendString("First to read img.\r\n");

		Y_cmosCapture_Ex(CAPTURE_START, READ_SCANNING);		//启动拍图，并等待拍图结束

	}

//	Y_commSendString("Wait to read img.\r\n");

	Y_cmosCapture_Ex(CAPTURE_WAIT, READ_SCANNING);		//不启动拍图，但等待拍图结束

	Y_commSendString("Capture end.\r\n");

	_s_Img.pSrc = (BYTE*)Y_cmosGetDataAddr();

	_s_Img.nWidth = Y_cmosGetWidth();
	_s_Img.nHeight = Y_cmosGetHeight();
	_s_Img.pTag = NULL;
	X_decoSetImgParam(&_s_Img);

	return &_s_Img;
}


/*
	nFlag
		= 0 解码失败后的处理
		= 其它任意值，解码成功后的处理
	nMode
		bit 0: package and edit result?
		bit 1: send data?
		bit 2: beep?
	nDevMode
		= DECODE_COLLECT_MODE
*/
static void LedHandlr(void)
{
    Z_LedOff(LED_GREEN);
}

void U_appAfterDecodeProc(int nFlag, int nMode, int nDevMode)
{
	int nLightMode,nFocusMode;
	UALLPARAM *psParam;
	int ndelay;
	int DFRec = 0 ;
    //unsigned char err;
    unsigned int nTime;

    
	psParam = X_paramGetGlobalAllParam();
	nLightMode = psParam->pHard->nLight;
	nFocusMode = psParam->pHard->nFocus;

	Y_cmosCapture_Ex(CAPTURE_STOP, READ_SCANNING);		//20FPS模式下退出读码模式时候，设置停止拍图

	bCaptureRun = FALSE;

// 	Y_commSendString("quit 1\r\n");
	if(!nFlag)		//解码失败
		T_LightFocusCtrl(FL_SITH_DECODER);

// 	GoodReadToMode(nFlag);

	if(!nFlag)		//解码失败后返回
		return;

	T_LightFocusCtrl(FL_DECODER_SUCC);

	if (psParam->pDFormat->DFEnable) //如果Data Format开启 则要对数据进行Dataformating编辑
	{
	    if (Y_commGetCurType() == COMMT_KBW)//kbw通讯模式,且dataformat使能,将标志传递给底层,dataformat库出来的数据在底层合并并恢复
		{
			Z_isSetKBWrestore(1);
		}
		DFRec = U_DataFormat();
	}
      else
		Z_isSetKBWrestore(0);
	
	if((nRtn > 0)||(DFRec != 1))//与dataformatting匹配或不匹配但需要输出解码成功信息的情况需要发出解码成功提示音
// 	if (X_paramGetGlobalAllParam()->pCode->nReadMode == ESC2)
	{//自动感应模式时，控制声音。因为前面要比对解码结果的重复性，所以没有发提示声音。
		U_appAlertStatus(U_appGetResult());
	}


	_sTrigReadFlag = 1;		//设置触发读码模式的识读成功标志,此标志将在IsReadMode中,按键松开状态下复位

	//打包并发送数据
	if (g_fnc_ProcDecMsg != NULL)
		g_fnc_ProcDecMsg();
	else
	{
		if(nMode & 0x01)	//打包数据
		{
			if (DFRec != 1)   // 当DFRec== 1 时，不输出前后缀
			{
				U_appMsgEditting();
			}
			U_appMsgPacking();
			U_appMsgAddTerminal();
		}

		U_appOPOSMsgEditting();//OPOS状态下 条码数据打包

		if(nMode & 0x02)	//发送数据
		{
			DFRec = Y_commGetCurType();
			if((DFRec == COMMT_IBM_SurePOS_TT) || (DFRec == COMMT_IBM_SurePOS_HH))
			{
				if (SurePOS_GetScannerState())
					SurePOS_SendData();
			}
			else
			{
			    //printf("DDD\r\n");
			    if((bPowerStart==TRUE)&&(DFRec==COMMT_KBW))
                       {     
                            nTime = Z_Get10msCounter();
                            nTime -= nTick;
                            if(nTime < 200)
                            {    
                                os_TimeDelay(10); 
                            }
                            else
                            {
                                bPowerStart = FALSE;     
                                KbwSetCharEncoding();
                                Y_commWrite(U_appGetMsgBuff(),(unsigned int)U_appGetMsgLen()); 
                            }                            
                        }
                       else
			    {
			        if (DFRec == COMMT_KBW)
                                KbwSetCharEncoding();
			        Y_commWrite(U_appGetMsgBuff(),(unsigned int)U_appGetMsgLen()); 
                        }

                       
                       //printf("EEE\r\n");
                   }
		}
	}

	GoodReadToMode(nFlag);


	SaveGoodDecodeImg((BYTE*)Y_cmosGetDataAddr(), Y_cmosGetHeight()*Y_cmosGetWidth());
	printf("led_enter\r\n");
// 	if (X_paramGetGlobalAllParam()->pCode->nReadMode != ESC3)	//非连续识读模式时，需要提示
	if ((X_paramGetGlobalAllParam()->pHard->nGoodreadLED & 1) == 0)
	{
		U_appBKGrndProc();
		printf("led_green\r\n");
		Z_LedOn(LED_GREEN);

		if(X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration == 4)
		{
			ndelay = X_paramGetGlobalAllParam()->pCode2->nSucLedDuration;
		}
		else
		{
			ndelay = 20 + X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration*100;
 #ifdef BUILT_HR32
			ndelay += 180;
 #endif
		}
        _s_TimeroutHandle(TCU_TIMER_LED, ndelay, LedHandlr);
	}
// 	else 
	if (X_paramGetGlobalAllParam()->pCode->nReadMode == ESC3)	//非连续识读模式时，需要提示
	{
// 		Z_WakeupTimer(20); //300ms定时唤醒
// 		U_appSleepMode(); //休眠,等待被唤醒
// 		Z_msDelay(80);
//		Z_WakeupTimer(300); //300ms定时唤醒
//		U_appSleepMode(); //休眠,等待被唤醒
	}

}

/*
	nMode
		bit 00: package and edit result?
		bit 01: send data?
		bit 02: beep?
		bit 16: RunDecode?
*/
BOOL U_appProcDecodeEx(int nMode)
{
	UTImgParam *sImg;
	int nInfoFlag = FALSE;
	BOOL bFirst = TRUE;

// 	if (capture_count >= 3000)
// 	{
// 		Y_cmosInit();
// 		capture_count = 0;
// 		Y_commSendString("cmos re-init\r\n");
// 	}

	do 
	{
		if(CheckAndRunIdle())		//检测是否进入Idle模式，退出Idle模式则会返回
			return FALSE;
		if(CheckAndRunStandby())	//检测是否进入standby模式，退出standby模式则会返回
			return FALSE;

		_s_ChkAndRunTestCmd();		//检测是否进入测试模式

		sImg = U_appChkToReadAndCapture(bFirst, 0);

		if(!sImg) 
		{
#if FM410_CAPTURE_ENABLE
			Y_commSendString("sImg no null\r\n");
#endif
			if (bFirst == TRUE)
			{
#if FM410_CAPTURE_ENABLE
				Y_commSendString("bFirst False\r\n");
#endif
				return FALSE;
			}
			else
			{
#if FM410_CAPTURE_ENABLE
				Y_commSendString("break1\r\n");
#endif
				break;
			}
		}
		else if (sImg->pSrc == NULL)
		{
#if FM410_CAPTURE_ENABLE
			Y_commSendString("break2\r\n");
#endif
			break;
		}

		bFirst = FALSE;

		nInfoFlag = U_appDecodeWithMacro();
		
	} while(nInfoFlag == FALSE);

	U_appAfterDecodeProc(nInfoFlag, nMode, 0);

	return nInfoFlag;
}

BOOL U_appProcDecode(void)
{
 /*     Z_LedOn(LED_GREEN);
      Z_msDelay(500);
      Z_LedOff(LED_GREEN);
      Z_msDelay(500);  */
 	return U_appProcDecodeEx(-1);
}//End of function;
int U_Set_OutMsg(BYTE *param,BYTE *AOutput,int size)//???????????????
{
	BYTE *p,*t;
	BYTE  c,s;

	p = AOutput;
	t = param;
	int i=0;
	int j=0;
	int ii=0;
	int len=0;

	while(j<size) // ????????????0????
	{
			if(*p == '{')//????,??????8???,????'{' + 2??????? + 4??????? + ????'}'
			{
				for (ii=0 ;ii<8 ;ii++)
				{
					p++;
					j++;
				}
			}
			else if(*p == '[')
			{
				for (ii=0; ;ii++)
				{
					j++;
					if (*p++ == ']')
					{
						break;
					}
				}
			}
			//e.g '7'->0x30,0x37->s = 0<<4 + 7 = 7
			//e.g 'A(0x41)'->0x34,0x31->s = 4<<4 + 1 = 0x41
			//????????????{}?[]?,???,??????????2?HEX??,????????{}[]???????????????????{????}[????]
			else
			{
				i=2;
				while(i--)
				{
					if(*p <= 0x39)
					{
						c = (*p -0x30);
					}
					else if(*p <= 0x5a && *p >= 0x41) 
					{	
						c = (*p - 0x41)+10;
					}
					else if(*p <= 0x7a && *p >= 0x61)
					{	
						c = (*p - 0x61)+10;
					}

					if(i == 1)
						s = c<<4;
					else 
						s = s + c;
					p++;
					j++;
				}
				*t++ = s;
				len++;
			}
	}
	return len;
}
int U_DataFormat()
{
	DF_Param	dfparam;
	
	UALLPARAM *psParam;
	int format_Id = -1;
	unsigned int nSymbolid,nBigCodeid,nMslen;
	BYTE Output[1024]={'\0'};
	int Drop_flag = 1;
	unsigned char AOutput[1024] /*= {0x33,0x31,0x7b,0x31,0x30,0x30,0x31,0x30,0x30,0x7d,0x33,0x32,0x34,0x42}*/;
	psParam = X_paramGetGlobalAllParam();

	if(U_appGetMsgLen()==0)	//设置码
		return -1;

	if(psParam->pDFormat->iSsingle)
	{
		format_Id = psParam->pDFormat->singleFormatID;
		psParam->pDFormat->iSsingle =0; // 关闭单次执行
	}
	else
		format_Id = psParam->pDFormat->usedID;

// 	len = sprintf(buf,"codeid = %d",U_appGetCodeType());
// 	Y_commWrite((unsigned char*)buf, len);
// 
// 	len = sprintf(buf,"BIGcodeid = %d",U_appGetCategory());
// 	Y_commWrite((unsigned char*)buf, len);

	nSymbolid = psParam->pDFormat->format[format_Id].SymbolID;//codeID是条码序列号
	if (nSymbolid < 32)//1D
	{
	/*	if ((nCodeid == 23)||(nCodeid == 24))
		{
			nCodeid = (1<<5);//ISSN、ISBN和EAN-13的条码类型一样
		}
		else*/
			nSymbolid = (1<<nSymbolid);//解码库给的codeID是 1左移（条码序列号）位得到的
		nBigCodeid = UT_TYPE_1D;//条码大类
	}
	else if (nSymbolid < 40)//2D
	{
		nSymbolid = (1<<(nSymbolid-32));//PDF417还从0开始是为了与使能位置的保持一致
		nBigCodeid = UT_TYPE_2D;
	}
	else//OCR
	{
		//code id出错。手册上没有OCR的条码序列号
		nSymbolid = (1<<(nSymbolid-64));
		nBigCodeid = UT_TYPE_OCR;
	}

/*	len = sprintf((char*)buf,"codebigtype=%d\r\n",U_appGetCategory());
	Y_commWrite(buf,len);

	len = sprintf((char*)buf,"ncodeid=%d\r\n",U_appGetCodeType());
	Y_commWrite(buf,len);
	*/
	//不符合data Format 的条件
	if (psParam->pDFormat->format[format_Id].MsgLen > 9999)
	{
		nRtn = -1011;
	}
	else if (((psParam->pDFormat->format[format_Id].TerID == Y_commGetCurType())  || (psParam->pDFormat->format[format_Id].TerID == 999)) &&
		(((nBigCodeid == U_appGetCategory()) && (nSymbolid == U_appGetCodeType())) || (psParam->pDFormat->format[format_Id].SymbolID ==999) || (psParam->pDFormat->format[format_Id].SymbolID ==99)) &&
		((psParam->pDFormat->format[format_Id].MsgLen == U_appGetMsgLen())  || (psParam->pDFormat->format[format_Id].MsgLen == 9999)))
	{
		dfparam.nCodeBigType = U_appGetCategory();
		dfparam.nCodeType = U_appGetCodeType();
		dfparam.pDecMsg  =  U_appGetMsgBuff();
		dfparam.nDecMsgLen = U_appGetMsgLen();
		dfparam.pDFCmd = (unsigned char*)psParam->pDFormat->format[format_Id].Cmd;
		dfparam.nDFMsgLen = strlen(psParam->pDFormat->format[format_Id].Cmd);
		dfparam.pDFOutputMsg = AOutput;
		dfparam.nDFOutputLen = 0;

    	dfparam.nMaxOutLen = sizeof(Output);						///缓存空间大小
		dfparam.nConvertToHexChar = 1;					///0:1个字节没有转换成2个Hex 

		if (psParam->pDFormat->outFlag == 0 || psParam->pDFormat->outFlag == 1)
		{
			dfparam.nRequired = 1;		//必须符合格式，不符合不输出							///0:外部没有配置 Required, 1:有配置Required
		}
		else 
		{
			dfparam.nRequired = 0;	//不符合格式的按要求原样输出或+前后缀输出
		}

	//	dfparam.nDFSelType = 1;			///1:选择Newland Data Format 方法1;  2:选择Newland Data Format 方法2, 其他方法:待定
		dfparam.nDFSelType = 3;//psParam->pDFormat->format[format_Id].FormatType;
		dfparam.func_DataFormat  = NULL;

		nRtn = GXDataFormat_Invoke(&dfparam);	/// < 0表示 Data Format错误		
	}
	else//不符合dataformatting格式
	{
	
		nRtn = -1;
	}

/*	for (len=0; len<dfparam.nDecMsgLen; len++)
	{
		Y_commSendChar(dfparam.pDecMsg[len]);
	}
	Y_commSendString("\r\n");

	len = sprintf(buf,"nCodeBigType = %d\r\n",(char*)dfparam.nCodeBigType);
	Y_commWrite((unsigned char*)buf,len);

	len = sprintf(buf,"nCodeType = %d\r\n",(char*)dfparam.nCodeType);
	Y_commWrite((unsigned char*)buf,len);

	//len = sprintf(buf,"pDecMsg = %s\r\n",(char*)dfparam.pDecMsg);
	//Y_commWrite((unsigned char*)buf,len);
	for (len=0; len<dfparam.nDecMsgLen; len++)
	{
		Y_commSendChar(dfparam.pDecMsg[len]);
	}
	Y_commSendString("\r\n");
	len = sprintf(buf,"nDecMsgLen = %d\r\n",dfparam.nDecMsgLen);
	Y_commWrite((unsigned char*)buf,len);

	len = sprintf(buf,"pDFCmd = %s\r\n",(char*)dfparam.pDFCmd);
	Y_commWrite((unsigned char*)buf,len);

	len = sprintf(buf,"nDFMsgLen = %d\r\n",(char*)dfparam.nDFMsgLen);
	Y_commWrite((unsigned char*)buf,len);

	len = sprintf(buf,"pDFOutputMsg = %x\r\n",&dfparam.pDFOutputMsg);
	Y_commWrite((unsigned char*)buf,len);
*/
	
/*
nRtn:
-1: 条码原文不符合dataformat格式
-1000		///读取指令错误
-1001		///添加字符串异常
-1002		///按位置删除字符串异常
-1003		///删除字符串异常
-1004		///提取指定位置的字符串异常
-1005		///数据容量已满
-1006		///非法指令
-1007		///参数错误
-1008		///内存错误
-1009		///根据条件退出
-1010		///没有选择要处理的类型
-1011		//设置的最大长度超过9999了
>=0 正确
*/
	if (nRtn<0)
	{
		if ((nRtn==0) || (nRtn==-1001) || (nRtn==-1002) || (nRtn==-1003) || (nRtn==-1004) || (nRtn==-1007))
		{
			nRtn = -1;//把不匹配返回值通用设置为-1，以下统一处理
		}
		if (nRtn == -1)//不符合dataformatting格式的处理
		{
			switch(psParam->pDFormat->outFlag)
			{
			case 0://按dataformatting格式输出，所有符合格式的条码（若有前后缀）+前后缀输出，所有不符合格式的条码 不输出，且发出或不发出错误提示音//Keep Required and Keep prefix/suffix
				//判断是否打开不符合dataformatting格式条码时发出错误提示音
				Drop_flag = 1;
				U_appSetMsg(dfparam.pDecMsg,0);
				if ((psParam->pDFormat->ErrorTone) == 0)
				{
				    int n;
					switch((X_paramGetGlobalAllParam()->pHard->nBeepType>>4) & 0x0f){
					case 0:
						n = 10;
						break;
					case 1:
						n = 4;
						break;
					case 2:
						n = 2;
						break;
					default:
						n = 10;
						break;
					}
					Z_PlayMusic_Ex((char*)SETCODE_ERROR, n); // 警报声	//提示音
				}
				
				return Drop_flag;
				break;
			case 1://按dataformatting格式输出，所有符合格式的条码（就算有前后缀也）不+前后缀，，所有不符合格式的条码 不输出，且发出或不发出错误提示音//Keep Required and Drop prefix/suffix
				//判断是否打开不符合dataformatting格式条码时发出错误提示音
				Drop_flag = 1;
				U_appSetMsg(dfparam.pDecMsg,0);
				if ((psParam->pDFormat->ErrorTone) == 0)
				{
					int n;
					switch((X_paramGetGlobalAllParam()->pHard->nBeepType>>4) & 0x0f){
					case 0:
						n = 10;
						break;
					case 1:
						n = 4;
						break;
					case 2:
						n = 2;
						break;
					default:
						n = 10;
						break;
					}
					Z_PlayMusic_Ex((char*)SETCODE_ERROR, n);  // 警报声	//提示音
				}
				
				return Drop_flag;
				break;
			case 2://按dataformatting格式刷输出（若有前后缀） + 前后缀，没设置成该格式的类型条码（若有前后缀）+前后缀输出//NOT Required and Keep prefix/suffix
			case 3://按dataformatting格式输出，设置成该格式类型的条码（就算有前后缀也）不+前后缀，没设置成该格式的类型条码（若有前后缀）+前后缀//NOT Required and Drop prefix/suffix
				//U_appSetMsg(dfparam.pDecMsg,0);
				//U_appMsgEditting();//添加前后缀
				Drop_flag = 0;//添加前后缀
				break;
			default:
				break;
			}
			return Drop_flag;

		}

	}
 	if( nRtn >= 0 )//符合dataformatting格式的处理
 	{
		switch(psParam->pDFormat->outFlag)
		{
		case 0:   	//按dataformatting格式输出，所有符合格式的条码（若有前后缀）+前后缀输出，所有不符合格式的条码 不输出，且发出或不发出错误提示音//Keep Required and Keep prefix/suffix
// 			if (nRtn == -1)
// 			{
// 				U_appSetMsg(dfparam.pDecMsg,0);
// 				Z_PlayMusic_Ex((char*)SETCODE_ERROR, 10); // 警报声	
// 				return -3;
// 			}
			Drop_flag = 0;
			break;
		case 1: //按dataformatting格式输出，所有符合格式的条码（就算有前后缀也）不+前后缀，，所有不符合格式的条码 不输出，且发出或不发出错误提示音//Keep Required and Drop prefix/suffix
// 			if (nRtn<0)
// 			{
// 				U_appSetMsg(dfparam.pDecMsg,0);
// 				Z_PlayMusic_Ex((char*)SETCODE_ERROR, 10); // 警报声	
// 				return -3;
// 			}

// 			if (dfparam.nCodeType == psParam->pDFormat->format[format_Id].CodeID)
// 			{
// 				//return 1;    //Drop prefix/suffix:
// 				Drop_falg =1 ;
// 			}
			Drop_flag = 1;
			break;
		case 2://按dataformatting格式输出（若有前后缀） + 前后缀，没设置成该格式的类型条码（若有前后缀）+前后缀输出//NOT Required and Keep prefix/suffix
// 			if ( nRtn<0)
// 			{
// 				return -1;
// 			}
			Drop_flag = 0;
			break;
		case 3:// 按dataformatting格式输出，设置成该格式类型的条码（就算有前后缀也）不+前后缀，没设置成该格式的类型条码（若有前后缀）+前后缀//NOT Required and Drop prefix/suffix
// 			if ( nRtn<0)
// 			{
// 				return -1;
// 			}
// 			if (dfparam.nCodeType == psParam->pDFormat->format[format_Id].CodeID)
// 			{
// 			//	return 1;  //Drop prefix/suffix:
// 				Drop_falg =1 ;
// 			}
			Drop_flag = 1;
			break;
		
		default:
// 			// 按原样输出
// 			return -1;
			break;
		}
	}

        if((Y_commGetCurType() != COMMT_KBW))//如果是KBW模式,数据在底层合并恢复，以便处理延时指令
        {	
		nMslen = U_Set_OutMsg(Output,AOutput,dfparam.nDFOutputLen);	
		U_appSetMsg(Output,nMslen);//将编辑后的结果赋值到整机输出结果缓存中
        }
        else
		U_appSetMsg(AOutput,dfparam.nDFOutputLen);// 将dataformat库里出来的数据直接赋值到整机输出结果缓存中

		 U_appSetResult(X_decoGetResult());//End of if;
		 //U_appSetMsg(Output,strlen((char *)Output));// 将编辑后的结果 赋值到 整机输出结果缓存中
		 U_appSetCategory(X_decoGetBarCode());
		 U_appSetCodeType(X_decoGetCodeType());


	return	Drop_flag;
}
void U_appOPOSMsgEditting(void)
{

	int nCodeType;
	int i = 0;
	int nMsgLen = U_appGetMsgLen();
	BYTE *pMsg = U_appGetMsgBuff();
	char Aimid[3];
	UALLPARAM *param = X_paramGetGlobalAllParam();
	char codeid[2]={0};

	if (!U_GetOPOSClaimState())	//OPOS claim 后需要将数据按opos格式打包
		return;

	if(U_appGetMsgLen()==0)	//设置码
		return;

	Z_DogFeed();
	U_appBKGrndProc();
	
	X_decoGetAIMID(Aimid);
	
	nCodeType = 32*((U_appGetCategory()&0xf) -1) + X_decoGetCodePos();

// 	Dbg(0, "type: %d, C: %#x, p: %d \r\n", nCodeType, U_appGetCategory(), X_decoGetCodePos());

	switch(nCodeType)
	{
	case 4:	//EAN8
		if (Aimid[2] == '3')
		{
			nCodeType = 129;	//SCAN_SDT_EAN8_S
		}
		break;

	case 5:	//EAN13
		if (Aimid[2] == '3')
		{
			nCodeType = 130;	//SCAN_SDT_EAN13_S
		}
		break;

	case 6:	//UPC-E
		if (Aimid[2] == '3')
		{
			nCodeType = 131;	//SCAN_SDT_UPCE_S
		}
		break;

	case 7:	//UPC-A
		if (Aimid[2] == '3')
		{
			nCodeType = 132;	//SCAN_SDT_UPCA_S
		}
		break;

	case 31:	//GS1 Databar Expanded
		X_decoGetRSSType(&i);
		if (i == 3)
		{
			nCodeType = 133;	//SCAN_SDT_GS1DATABAR_E / SCAN_SDT_RSS_EXPANDED
		}
		break;

	case 33:	//Micro QR
		X_decoGetQRType(&i);
		if (i == 10)
		{
			nCodeType = 134;	//SCAN_SDT_GS1DATABAR_E / SCAN_SDT_RSS_EXPANDED
		}
		break;

	}
	
	
	switch(U_appGetCategory()){
	case UT_TYPE_1D:
		codeid[0] = param->pCode->by1DCodeID[X_decoGetCodePos()];
		codeid[1] = param->pCode2->by1DCodeID_2[X_decoGetCodePos()];
		break;
	case UT_TYPE_2D:
		codeid[0] = param->pCode->by2DCodeID[X_decoGetCodePos()];
		codeid[1] = param->pCode2->by2DCodeID_2[X_decoGetCodePos()];
		break;
	case UT_TYPE_POSTAL:
		codeid[0] = param->pCode2->byPostalCodeID[X_decoGetCodePos()];
		codeid[1] = param->pCode2->byPostalCodeID_2[X_decoGetCodePos()];
		break;
	case UT_TYPE_OCR:
		if (nCodeType == 66)	// PASSPORT_VISAS
			codeid[0] = 'O';
		else
			codeid[0] = 'S';
		break;
	}

	//加上包头信息
	for(i = 0;i < nMsgLen; i ++)
		pMsg[nMsgLen+8-i] = pMsg[nMsgLen-i-1];

	//报文首部
	pMsg[0] = 0x4;
	pMsg[1] = 0xfe;
	pMsg[2] = ((nMsgLen+5)&0x00ff00)>>8;
	pMsg[3] = ((nMsgLen+5)&0x0000ff);
	pMsg[4] = nCodeType;
	
	pMsg[5] = codeid[0];
	
	pMsg[6] = Aimid[0];
	pMsg[7] = Aimid[1];
	pMsg[8] = Aimid[2];
	//加上校验
	nMsgLen += 9;
	U_appSetMsg(pMsg, nMsgLen);
}

void KbwSetCharEncoding(void)
{
	int nCodeType;
	int i = 0;
	UALLPARAM *param = X_paramGetGlobalAllParam();


	if(U_appGetMsgLen()==0)	//???
	{
		Z_KBWSetCharEncoding(0);
		return;
	}

	Z_DogFeed();
	U_appBKGrndProc();

	nCodeType = 32*((U_appGetCategory()&0xf) -1) + X_decoGetCodePos();

	// 	Dbg(0, "type: %d, C: %#x, p: %d \r\n", nCodeType, U_appGetCategory(), X_decoGetCodePos());

	switch(nCodeType)
	{

#if 0
	case 2:	//Code128
		i = param->pCode2->nCharEncoding[CharacterEncoding_Code128];
		break;

	case 13:	//Code39
		i = param->pCode2->nCharEncoding[CharacterEncoding_Code39];
		break;

	case 17:	//Code39
		i = param->pCode2->nCharEncoding[CharacterEncoding_Code93];
		break;
#endif

	case 32:	//PDF417
		i = param->pCode2->nCharEncoding[CharacterEncoding_PDF417];
		break;

	case 33:	//QR
		i = param->pCode2->nCharEncoding[CharacterEncoding_QR];
		break;

	case 34:	//Aztec
		i = param->pCode2->nCharEncoding[CharacterEncoding_Aztec];
		break;

	case 35:	//DM
		i = param->pCode2->nCharEncoding[CharacterEncoding_DataMatrix];
		break;

	default:
		i = 0;
		break;
	}
	if ((i<0) || (i>1))
		i = 0;

	Z_KBWSetCharEncoding(i);

}



//信息编辑
BOOL	U_appMsgEditting(void)
{
	WInfoParam sParam;

	//后台工作
	Z_DogFeed();
	U_appBKGrndProc();

	sParam.pbyMsgBuff = U_appGetMsgBuff();
	sParam.nMsgLen = U_appGetMsgLen();
	sParam.nBuffSize = U_appGetMsgBuffSize();
	sParam.nCodeCategory = U_appGetCategory();
	sParam.nCodeType = U_appGetCodeType();
	sParam.nCodePos = X_decoGetCodePos();
	if (W_infoEditting(&sParam))
	{
		U_appSetMsg(sParam.pbyMsgBuff,sParam.nMsgLen);
		return TRUE;
	}
	return FALSE;
}
//信息打包
BOOL	U_appMsgPacking(void)
{
	WInfoParam sParam;

	//后台工作
	Z_DogFeed();
	U_appBKGrndProc();

	sParam.pbyMsgBuff = U_appGetMsgBuff();
	sParam.nMsgLen = U_appGetMsgLen();
	sParam.nBuffSize = U_appGetMsgBuffSize();
	sParam.nCodeCategory = U_appGetCategory();
	sParam.nCodeType = U_appGetCodeType();
	sParam.nCodePos = X_decoGetCodePos();
	if (W_infoPacking(&sParam))
	{
		U_appSetMsg(sParam.pbyMsgBuff,sParam.nMsgLen);
		return TRUE;
	}
	return FALSE;
}
//添加终止字符
BOOL	U_appMsgAddTerminal(void)
{
	WInfoParam sParam;

	//后台工作
	Z_DogFeed();
	U_appBKGrndProc();

	sParam.pbyMsgBuff = U_appGetMsgBuff();
	sParam.nMsgLen = U_appGetMsgLen();
	sParam.nBuffSize = U_appGetMsgBuffSize();
	sParam.nCodeCategory = U_appGetCategory();
	sParam.nCodeType = U_appGetCodeType();
	sParam.nCodePos = X_decoGetCodePos();
	if (W_infoAddTerminal(&sParam))
	{
		U_appSetMsg(sParam.pbyMsgBuff,sParam.nMsgLen);
		return TRUE;
	}
	return FALSE;
}//End of function;

int		U_appGetVersionInfo(char *pBuff, int nBuffSize)
{
	int nLen;

	//后台工作
	Z_DogFeed();
	U_appBKGrndProc();
	

	nLen = V_commGetVersion(pBuff,nBuffSize);
// 	//uIMG信息
// 	nLen += X_decoGetUIMGVersion(buff+nLen,sizeof(buff)-nLen);
// 	//硬件信息
// 	nLen += Z_GetHardwareInfo(buff+nLen,sizeof(buff)-nLen);
// 	if (sizeof(buff)-nLen > sizeof(MODULE_SDK_VERSION)+12)
// 	{
// 		nLen += sprintf(buff+nLen,"MSDKVer:%s\r\n",MODULE_SDK_VERSION);
// 	}//End of if;

	if (nLen > nBuffSize-1)
	{ nLen = nBuffSize-1; }
// 	memcpy(pBuff,buff,nLen);

	pBuff[nLen] = 0;
	
	return nLen;
}//End of funciton;

void	U_appPowerOnDisplay(void)
{
	static BOOL bDone = FALSE; //是否已经被调用过了
	//后台工作
	Z_DogFeed();
	U_appBKGrndProc();
	//显示
	if (!bDone && X_paramGetGlobalAllParam()->pCode->IsPowerOnDisp)
	{ X_paramSendParamInfo(); }
	bDone = TRUE;

}






