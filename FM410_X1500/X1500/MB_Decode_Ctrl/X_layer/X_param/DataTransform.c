#include "DataTransform.h"
//#include <stdio.h>
//#include <string.h>
#include "../../Z_layer/Z_define.h"
#include "../../NLS_SCProtocol/SC_MapTable.h"

#include <string.h>
#include <stdio.h>
//////////////////////////////////////////////////////////////////////////

#define CUR_KEY_MSG_CNT				10				//ESN / S/N DATE 等

#define   MEMSize_COM		(sizeof(TComParam) + 10)
#define   Start_MEMSize_COM   32

#define   MEMSize_UTK		(sizeof(UTKeyBoard) + 10)
#define   Start_MEMSize_UTK   (Start_MEMSize_COM + MEMSize_COM)

#define   MEMSize_HARD		(sizeof(HardParam) + 10)
#define   Start_MEMSize_HARD (Start_MEMSize_UTK + MEMSize_UTK)

#define   MEMSize_CODE		(sizeof(CodeParam) + 10)
#define   Start_MEMSize_CODE (Start_MEMSize_HARD + MEMSize_HARD)

#define   MEMSize_1DParam	(sizeof(UTDec1DSetup) + 10)
#define   Start_MEMSize_1D (Start_MEMSize_CODE + MEMSize_CODE)

#define   MEMSize_2DParam	(sizeof(UTDec2DSetup) + 10)
#define   Start_MEMSize_2D (Start_MEMSize_1D + MEMSize_1DParam)

#define   MEMSize_OCRParam	(sizeof(UTDecOCRSetup) + 10)
#define   Start_MEMSize_OCR (Start_MEMSize_2D + MEMSize_2DParam)

#define   MEMSize_PostalParam	(sizeof(UTDecPostalSetup) + 10)
#define   Start_MEMSize_Postal  (Start_MEMSize_OCR + MEMSize_OCRParam)


#define   MEMSize_CODE2		(sizeof(CodeParam2) + 10)
#define   Start_MEMSize_CODE2 (Start_MEMSize_Postal + MEMSize_PostalParam)

#define   MEMSize_DATAFORMAT		(sizeof(DataFormat) + 10)
#define   Start_MEMSize_DATAFORMAT (Start_MEMSize_CODE2 + MEMSize_CODE2)	

#define   MEMSize_AREAPARAM		(sizeof(UIMGAreaParam) + 10)
#define   Start_MEMSize_AREAPARAM (Start_MEMSize_DATAFORMAT + MEMSize_DATAFORMAT)	

#define   Total_MEMSize     (Start_MEMSize_AREAPARAM + MEMSize_AREAPARAM)
//#define   Total_MEMSize     (Start_MEMSize_DATAFORMAT + MEMSize_DATAFORMAT)

#define FLASH_BLOCK_SIZE			4096// SST block
#define FLASH_BLOCK_VALID_SIZE		Total_MEMSize	//1024
// OEM Firmware Param    //提供给OEM厂商的参数存储位置，起始内存块，存储长度
#define OEM_BLOCK_VALID_SIZE	    1024
//校验码在pCode2中的偏移
#define	  CRC_OFFSET_V1		 72 //4 + 32 + 32 + 4

#define  PARAMVERSION	0x0001

unsigned char cParamSaveFlag=0;

unsigned int g_commChange=0;


//////////////////////////////////////////////////////////////////////////

BOOL uJ_1DParam(void);
BOOL uJ_2DParam(void);
BOOL uJ_OCRParam(void);
BOOL uJ_ComParam(void);
BOOL uJ_InterFaceParam(void);
BOOL uJ_SysCtrlInfoParam(void);
BOOL uJ_ReadModeParam(void);
BOOL uJ_DataPackParam(void);
BOOL uJ_UserTypeParam(void);
BOOL uJ_InfoEditParam(void);
BOOL uJ_LightFocusParam(void);
int uMinMaxGetResult(int Value,int Final);
int uMinMaxGetResult_High(int Value,int Final);
//////////////////////////////////////////////////////////////////////////
BYTE FacParamBuf[FLASH_BLOCK_SIZE];
BOOL g_FacParamGet = FALSE;

BOOL			g_bAllParamInited = FALSE;	//是否经过初始化的标志

TComParam		g_Com;    //Com1设置
HardParam		g_Hard;   //硬件参数设置
CodeParam		g_Code;   //解码参数整体设置
CodeParam2		g_Code2;   //解码参数整体设置2
UTKeyBoard		g_KBW;    //KBW 模式
UTBlueTooth     g_BlueT;  //蓝牙
TREADMODE		g_Mode;   //读码模式
UTDeviceINFO	g_DevInfo;
UTDec1DSetup		g_1D;
UTDec2DSetup		g_2D;
UTDecOCRSetup		g_OCR;
UTDecPostalSetup	g_Postal;
//char *			g_IMGVersion = (char*)UIMAGE_PRI_VERSION;
TKEYSEG			g_sKeySeg[CUR_KEY_MSG_CNT];	//关键字
int				g_nKeyCnt = 0;	//关键字数
TParamInfo		g_ParamInfo;
DataFormat		g_dataformat;
UIMGAreaParam     g_areaParam;

UALLPARAM		__g_All_Param = 
						{
							//g_IMGVersion,
							&g_ParamInfo,
							&g_Com,
							&g_KBW,
							&g_Hard,
							&g_Code,
							&g_1D,
							&g_2D,
							&g_OCR,
							&g_Postal,
							&g_DevInfo,
							&g_Mode,
							g_sKeySeg,
							&g_Code2,
							&g_dataformat,
							&g_areaParam
						};

TComParam		g_bak_Com;    //Com1设置
HardParam		g_bak_Hard;   //硬件参数设置
CodeParam		g_bak_Code;   //解码参数整体设置
CodeParam2		g_bak_Code2;   //解码参数整体设置2
UTKeyBoard		g_bak_KBW;    //KBW 模式
UTBlueTooth         g_bak_BlueT;  //蓝牙
TREADMODE		g_bak_Mode;   //读码模式
UTDeviceINFO	       g_bak_DevInfo;
UTDec1DSetup		g_bak_1D;
UTDec2DSetup		g_bak_2D;
UTDecOCRSetup		g_bak_OCR;
UTDecPostalSetup	g_bak_Postal;
TKEYSEG			g_bak_sKeySeg[CUR_KEY_MSG_CNT];	//关键字
int				g_bak_nKeyCnt = 0;	//关键字数
TParamInfo		g_bak_ParamInfo;
DataFormat		g_bak_dataformat;
UIMGAreaParam     g_bak_areaParam;

UALLPARAM		__g_bak_All_Param = 
						{
							&g_bak_ParamInfo,
							&g_bak_Com,
							&g_bak_KBW,
							&g_bak_Hard,
							&g_bak_Code,
							&g_bak_1D,
							&g_bak_2D,
							&g_bak_OCR,
							&g_bak_Postal,
							&g_bak_DevInfo,
							&g_bak_Mode,
							g_bak_sKeySeg,
							&g_bak_Code2,
							&g_bak_dataformat,
							&g_bak_areaParam
						};


FNC_paramOriginalUserDefault	X_paramOriginalUserDefault = NULL;
FNC_param2DevLoadUserDefault	X_param2DevLoadUserDefault = NULL;
FNC_param2DevSaveUserDefault	X_param2DevSaveUserDefault = NULL;
//////////////////////////////////////////////////////////////////////////
#define __PARAM_INIT_() \
	if (X_paramIsAllParamInited() != TRUE) \
	{ X_paramInitedAllParam(); }//End of if;


BOOL	X_paramIsAllParamInited(void)
{
	return g_bAllParamInited;
}//End of function;

BOOL X_FacParamIsGet(void)
{
    return g_FacParamGet;
}

void	X_paramInitedAllParam(void)
{
	static int bHadIn = FALSE;
// BYTE pBuff[4096];

	if (bHadIn != FALSE) //防止递归
	{ return; }

	//若已初始化,则不再重新初始化
	if (X_paramIsAllParamInited() == TRUE) 
	{ return; }//End of if;

	bHadIn = TRUE;
	//导入设备信息
	X_paramLoadDeviceInfo();

	//导入参数信息
	if (X_paramLoadSysDefaultParam() != TRUE)
	{
		if (X_paramLoadUsrDefaultParam() != TRUE)
		{//导入用户默认参数

			if (X_paramLoadFactoryParam() != TRUE)
			{//导入用户出厂配置

				//出厂默认参数
				X_paramOriginalInitAllParam();
				X_paramSaveAsFactoryParam(); //存为出厂配置
			}
			else
			{
				//存为用户默认配置
				X_paramSaveAsUsrDefaultParam();
			}
		}//End of if;
		else
		{
			//存储为系统默认参数
			X_paramSaveAsSysDefaultParam(); //复制为系统默认
		}
	}//End of if;
// Y_paramLoadOEM(pBuff,OEM_BLOCK_VALID_SIZE);
// Y_commWrite(pBuff,OEM_BLOCK_VALID_SIZE);
      if(X_paramGetFactoryParam(FacParamBuf, FLASH_BLOCK_SIZE) == TRUE)
            g_FacParamGet = TRUE;

	g_bAllParamInited = TRUE;
	bHadIn = FALSE;
}//End of function;

UALLPARAM * X_paramGetGlobalAllParam(void)
{
	return (UALLPARAM * )(&__g_All_Param);
}//End of function;

UALLPARAM * X_paramBakGetGlobalAllParam(void)
{
	return (UALLPARAM * )(&__g_bak_All_Param);
}

UTBlueTooth * X_paramGetBlueT(void)
{
	return (UTBlueTooth * )(&g_BlueT);
}//End of function;

//判断输入的缓存中的系统参数是否合法，或版本是否可转换等
BOOL X_paramIsParamsRight(BYTE *pBuff, int nSize)
{
	//比对版本号
	char * pVer = (char*)UIMAGE_PRI_VERSION;

	if (nSize < 4)
	{ return FALSE; }//End of if;
	//字符串比较，若值为0,则表示字符串相同
	return (strncmp((const char *)pBuff,pVer,4) == 0);
}//End of function;

BOOL X_paramCRC32Check(BYTE *pBuff)
{
	unsigned int crc32;
	unsigned int crc32_2;
	//CodeParam2 pCode2;
	
	memcpy(&crc32, pBuff+12, sizeof(unsigned int));

	crc32_2 = Z_GetCRC32((pBuff+32),  Total_MEMSize-32);

	if (crc32_2 != crc32)
	{return FALSE;}

	return TRUE;
}

BOOL X_paramVersionCheck(BYTE *pBuff)
{
	int version=0;

	version = *(int*)(pBuff+8);

	if(version == 0)//	0.9以下版本
		return FALSE;
	else//高版本
		return TRUE;
}




//从输入缓存中读取系统各项参数，若有版本差异，最好也能处理
BOOL X_paramGetBakParamFromBuff(BYTE *pBuff, int nSize)
{
      UALLPARAM* psParam_bak = NULL;
//	BOOL bFlag;


	psParam_bak = X_paramBakGetGlobalAllParam();
      
	if (psParam_bak == NULL)
	{ return FALSE; }

	if (nSize < Total_MEMSize)
	{ return FALSE; }


//参数区里是新结构体，直接拷贝
	if (X_paramCRC32Check(pBuff) != TRUE)
	{
	    return FALSE;
	}
	
       //flash内容在RAM的另一份备份
      if (psParam_bak->pParamInfo != NULL)
	{
		memcpy(psParam_bak->pParamInfo,pBuff,sizeof(TParamInfo));
	}
	if (psParam_bak->pCom != NULL)
	{ memcpy(psParam_bak->pCom,	pBuff+Start_MEMSize_COM,sizeof(TComParam)); }
	if (psParam_bak->pKBW != NULL)
	{ memcpy(psParam_bak->pKBW,	pBuff+Start_MEMSize_UTK,sizeof(UTKeyBoard)); }
	if (psParam_bak->pHard != NULL)
	{ memcpy(psParam_bak->pHard,	pBuff+Start_MEMSize_HARD,sizeof(HardParam)); }
	if (psParam_bak->pCode != NULL)
	{ memcpy(psParam_bak->pCode,	pBuff+Start_MEMSize_CODE,sizeof(CodeParam)); }
	if (psParam_bak->p1D != NULL)
	{ memcpy(psParam_bak->p1D,		pBuff+Start_MEMSize_1D,sizeof(UTDec1DSetup)); }

	if (psParam_bak->p2D != NULL)
	{ memcpy(psParam_bak->p2D,		pBuff+Start_MEMSize_2D,sizeof(UTDec2DSetup)); }
	if (psParam_bak->pOCR != NULL)
	{ memcpy(psParam_bak->pOCR,	pBuff+Start_MEMSize_OCR,sizeof(UTDecOCRSetup)); }
	if (psParam_bak->pPostal != NULL)
	{ memcpy(psParam_bak->pPostal,	pBuff+Start_MEMSize_Postal,sizeof(UTDecPostalSetup)); }

	if (psParam_bak->pCode2 != NULL)
	{ 
		memcpy(psParam_bak->pCode2,	pBuff+Start_MEMSize_CODE2,sizeof(CodeParam2)); 
	}
	if (psParam_bak->pDFormat != NULL)  //zhongds
	{ 
		memcpy(psParam_bak->pDFormat,pBuff+Start_MEMSize_DATAFORMAT,sizeof(DataFormat)); 
	}
      if (psParam_bak->pAreaParam!= NULL)  
	{ 
		memcpy(psParam_bak->pAreaParam,pBuff+Start_MEMSize_AREAPARAM,sizeof(UIMGAreaParam)); 
	}

	return TRUE;
}//End of function;



//从输入缓存中读取系统各项参数，若有版本差异，最好也能处理
BOOL X_paramGetParamFromBuff(BYTE *pBuff, int nSize)
{
	UALLPARAM* psParam = NULL;
	//BOOL bFlag;
//	int len;

	psParam = X_paramGetGlobalAllParam();
	if (psParam == NULL)
	{ 	return FALSE; }
	 
	if (nSize < Total_MEMSize)
	{ return FALSE; }


// 	bFlag = X_paramVersionCheck(pBuff);
// 
// 	if (!bFlag)//参数区里是旧结构体需解析
// 	{
// 		x_paramNewParamInit();
// 		return TRUE;
// 	}	

//参数区里是新结构体，直接拷贝
	if (X_paramCRC32Check(pBuff) != TRUE)
	{
		return FALSE;
	}
//Dbg(0,"new param\r\n");

	if (psParam->pParamInfo != NULL)
	{
		memcpy(psParam->pParamInfo,pBuff,sizeof(TParamInfo));
	}
	if (psParam->pCom != NULL)
	{ memcpy(psParam->pCom,	pBuff+Start_MEMSize_COM,sizeof(TComParam)); }
	if (psParam->pKBW != NULL)
	{ memcpy(psParam->pKBW,	pBuff+Start_MEMSize_UTK,sizeof(UTKeyBoard)); }
	if (psParam->pHard != NULL)
	{ memcpy(psParam->pHard,	pBuff+Start_MEMSize_HARD,sizeof(HardParam)); }
	if (psParam->pCode != NULL)
	{ memcpy(psParam->pCode,	pBuff+Start_MEMSize_CODE,sizeof(CodeParam)); }
	if (psParam->p1D != NULL)
	{ memcpy(psParam->p1D,		pBuff+Start_MEMSize_1D,sizeof(UTDec1DSetup)); }

	if (psParam->p2D != NULL)
	{ memcpy(psParam->p2D,		pBuff+Start_MEMSize_2D,sizeof(UTDec2DSetup)); }
	if (psParam->pOCR != NULL)
	{ memcpy(psParam->pOCR,	pBuff+Start_MEMSize_OCR,sizeof(UTDecOCRSetup)); }
	if (psParam->pPostal != NULL)
	{ memcpy(psParam->pPostal,	pBuff+Start_MEMSize_Postal,sizeof(UTDecPostalSetup)); }

	if (psParam->pCode2 != NULL)
	{ 
		memcpy(psParam->pCode2,	pBuff+Start_MEMSize_CODE2,sizeof(CodeParam2)); 
	}
	if (psParam->pDFormat != NULL)  //zhongds
	{ 
		memcpy(psParam->pDFormat,pBuff+Start_MEMSize_DATAFORMAT,sizeof(DataFormat)); 
	}
      if(psParam->pAreaParam != NULL)
      { 
		memcpy(psParam->pAreaParam,pBuff+Start_MEMSize_AREAPARAM,sizeof(UIMGAreaParam)); 
	}
	x_paramNewParamInit();

	return TRUE;
}//End of function;


//将参数转换成缓冲区中的串行数据,并返回大小
int X_paramPushParamToBuff(BYTE *pBuff, int nBuffSize)
{
	UALLPARAM* psParam = NULL;

	//int tempCrc;
//	int len;
	psParam = X_paramBakGetGlobalAllParam();
	if (psParam == NULL)
	{ return 0; }
	if (Total_MEMSize > nBuffSize)
	{ return 0; }

	memset(pBuff,0,nBuffSize);


	if (psParam->pCom != NULL)
	{ memcpy(pBuff+Start_MEMSize_COM,psParam->pCom,sizeof(TComParam)); }
	if (psParam->pKBW != NULL)
	{ memcpy(pBuff+Start_MEMSize_UTK,psParam->pKBW,sizeof(UTKeyBoard)); }
	if (psParam->pHard != NULL)
	{ memcpy(pBuff+Start_MEMSize_HARD,psParam->pHard,sizeof(HardParam)); }
	if (psParam->pCode != NULL)
	{ memcpy(pBuff+Start_MEMSize_CODE,psParam->pCode,sizeof(CodeParam)); }
	if (psParam->p1D != NULL)
	{ memcpy(pBuff+Start_MEMSize_1D,psParam->p1D,sizeof(UTDec1DSetup)); }
	if (psParam->p2D != NULL)
	{ memcpy(pBuff+Start_MEMSize_2D,psParam->p2D,sizeof(UTDec2DSetup)); }
	if (psParam->pOCR != NULL)
	{ memcpy(pBuff+Start_MEMSize_OCR,psParam->pOCR,sizeof(UTDecOCRSetup)); }

	if (psParam->pPostal != NULL)
	{ memcpy(pBuff+Start_MEMSize_Postal,psParam->pPostal,sizeof(UTDecPostalSetup)); }

	if (psParam->pCode2 != NULL)
	{ 
		psParam->pCode2->nsize = sizeof(CodeParam2);
		memcpy(pBuff+Start_MEMSize_CODE2, psParam->pCode2, sizeof(CodeParam2)); 
		//psParam->pCode2->crc32 = Get_CRC32((char*)pBuff,Start_MEMSize_CODE2+sizeof(CodeParam2)-sizeof(int));
		//memcpy(pBuff+Start_MEMSize_CODE2, psParam->pCode2, sizeof(CodeParam2)); 
	}

	
	if (psParam->pDFormat != NULL)   //zhongds
	{
/*		psParam->pDFormat->crc = Get_CRC32((char*)pBuff+Start_MEMSize_DATAFORMAT,Start_MEMSize_DATAFORMAT+sizeof(DataFormat)-sizeof(int));*/
/*		psParam->pDFormat->crc = Get_CRC32((char*)pBuff+Start_MEMSize_DATAFORMAT+4,21);*/
	/*	diag_printf("start = %d\r\n",Start_MEMSize_DATAFORMAT);
		diag_printf("crc = %d\r\n", psParam->pDFormat->crc);
		diag_printf("DFEnable = %d\r\n", psParam->pDFormat->DFEnable);
		diag_printf("usedID = %d\r\n", psParam->pDFormat->usedID);
		diag_printf("outFlag = %d\r\n", psParam->pDFormat->outFlag);
		diag_printf("singleFormatID = %d\r\n", psParam->pDFormat->singleFormatID);
		diag_printf("iSsingle = %d\r\n", psParam->pDFormat->iSsingle);
		diag_printf("format = %s\r\n", psParam->pDFormat->format[0]);*/
		memcpy(pBuff+Start_MEMSize_DATAFORMAT,psParam->pDFormat,sizeof(DataFormat)); 
	}

       if(psParam->pAreaParam != NULL)
       {
            memcpy(pBuff+Start_MEMSize_AREAPARAM, psParam->pAreaParam, sizeof(UIMGAreaParam));
     /*       printf("bottom=%d,top=%d,right=%d,left=%d\n",psParam->pAreaParam->bottom,psParam->pAreaParam->top,psParam->pAreaParam->right,psParam->pAreaParam->left);
     for(i=0; i<sizeof(UIMGAreaParam); i++)
	    printf("%x ",pBuff[Start_MEMSize_AREAPARAM+i]);
    printf("\n");*/
       }

	if ((Start_MEMSize_AREAPARAM+sizeof(UIMGAreaParam)) >= FLASH_BLOCK_SIZE) //zhongds
	{ 
		return 0; 
	}
// 	if ((Start_MEMSize_CODE2+sizeof(CodeParam2)) >= FLASH_BLOCK_SIZE)
// 	{ return 0; }
//	return (Start_MEMSize_CODE2+sizeof(CodeParam2));//该长度比总长度小于10，若返回这个长度，参数区最后10个字节将直接填充255.

	if (psParam->pParamInfo != NULL)
	{
		psParam->pParamInfo->paramCRC = Z_GetCRC32((pBuff+32), Total_MEMSize-32);
		memcpy(pBuff,psParam->pParamInfo,sizeof(TParamInfo)); 
	}

	return Total_MEMSize;  // zhongds
}//End of function;

//从Flash中导出系统默认参数
BOOL X_paramLoadSysDefaultParam(void)
{
	BYTE byBuff[FLASH_BLOCK_SIZE];

	//检查
	if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
	{ return FALSE; }
	//读参数

	if (Y_paramLoadSysDefault(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE) 
	{ return FALSE; }//End of if;
	//解析参数

	if (X_paramGetParamFromBuff(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE)
	{ return FALSE; }
	//更新设备参数

      if(X_paramGetBakParamFromBuff(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE)//加载系统配置时，需要双份，备份块在查询时使用
      {return FALSE;}
      
	X_paramUpdateToDevice();

	//读取参数，并解析成功
	return TRUE;
}//End of function;

//从Flash中导出用户自定义的默认参数
BOOL X_paramLoadUsrDefaultParam(void)
{
	BYTE byBuff[FLASH_BLOCK_SIZE];

	//检查
	if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
	{ return FALSE; }

	//读参数
	if (Y_paramLoadUsrDefault(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE) 
	{ return FALSE; }//End of if; 

	//解析参数
	if (X_paramGetParamFromBuff(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE)
	{ return FALSE; }

      if(X_paramGetBakParamFromBuff(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE)//加载系统配置时，需要双份，备份块在查询时使用
      {return FALSE;}

	//更新设备参数
	X_paramUpdateToDevice();

	//调用二次开发的"导入用户默认"
	if (X_param2DevLoadUserDefault != NULL)
	{ X_param2DevLoadUserDefault(); }

	//读取参数，并解析成功
	return TRUE;
}//End of function;

//从Flash中到处用户出厂配置
BOOL X_paramLoadFactoryParam(void)
{
	BYTE byBuff[FLASH_BLOCK_SIZE];

	//检查
	if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
	{ return FALSE; }
	
	//读参数
	if (Y_paramLoadFactory(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE) 
	{ return FALSE; }//End of if; 
	
	//解析参数
	if (X_paramGetParamFromBuff(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE)
	{ return FALSE; }

       if(X_paramGetBakParamFromBuff(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE)//加载系统配置时，需要双份，备份块在查询时使用
      {return FALSE;}
	/* printf("read factory param\n");
     for(i=0; i<FLASH_BLOCK_VALID_SIZE; i++)
        printf("%x ", byBuff[i]);
     printf("\n");*/
	//更新设备参数
	X_paramUpdateToDevice();
	
	//读取参数，并解析成功
	return TRUE;
}//End of function;


//从Flash中到处用户出厂配置
BOOL X_paramGetFactoryParam(unsigned char *buff, unsigned int nSize)
{
	//BYTE byBuff[FLASH_BLOCK_SIZE];

	//检查
	if (nSize< FLASH_BLOCK_VALID_SIZE)
	{ return FALSE; }
	
	//读参数
	if (Y_paramLoadFactory(buff,FLASH_BLOCK_VALID_SIZE) != TRUE) 
	{ return FALSE; }

      //if(X_paramCRC32Check(buff) != TRUE)
      //{ return FALSE; }  
	
	//解析参数
	//if (X_paramGetParamFromBuff(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE)
	//{ return FALSE; }
	
	//更新设备参数
	//X_paramUpdateToDevice();
	
	//读取参数，并解析成功
	return TRUE;
}//End of function;

void X_SysParamCpy(void)
{
    memcpy(__g_bak_All_Param.p1D, __g_All_Param.p1D, sizeof(UTDec1DSetup));
    memcpy(__g_bak_All_Param.p2D, __g_All_Param.p2D, sizeof(UTDec2DSetup));
    memcpy(__g_bak_All_Param.pCode, __g_All_Param.pCode, sizeof(CodeParam));
    memcpy(__g_bak_All_Param.pCode2, __g_All_Param.pCode2, sizeof(CodeParam2));
    memcpy(__g_bak_All_Param.pCom, __g_All_Param.pCom, sizeof(TComParam));
    memcpy(__g_bak_All_Param.pDevInfo, __g_All_Param.pDevInfo, sizeof(UTDeviceINFO));
    memcpy(__g_bak_All_Param.pDFormat, __g_All_Param.pDFormat, sizeof(DataFormat));
    memcpy(__g_bak_All_Param.pHard, __g_All_Param.pHard, sizeof(HardParam));
    memcpy(__g_bak_All_Param.pKBW, __g_All_Param.pKBW, sizeof(UTKeyBoard));
    memcpy(__g_bak_All_Param.pKeyS, __g_All_Param.pKeyS, sizeof(TKEYSEG));
    memcpy(__g_bak_All_Param.pMode, __g_All_Param.pMode, sizeof(TREADMODE));
    memcpy(__g_bak_All_Param.pOCR, __g_All_Param.pOCR, sizeof(UTDecOCRSetup));
    memcpy(__g_bak_All_Param.pParamInfo, __g_All_Param.pParamInfo, sizeof(TParamInfo));
    memcpy(__g_bak_All_Param.pPostal, __g_All_Param.pPostal, sizeof(UTDecPostalSetup));
    memcpy(__g_bak_All_Param.pAreaParam, __g_All_Param.pAreaParam, sizeof(UIMGAreaParam));
}

//将当前系统参数存储为系统默认参数
BOOL X_paramSaveAsSysDefaultParam(void)
{
	BYTE byBuff[FLASH_BLOCK_SIZE];
	int nSize;

	nSize = 0;
	//检查
	if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
	{ return FALSE; }

	//更新设备的参数
	X_paramUpdateToDevice();
	X_paramSaveDeviceInfo();
	//将参数写到缓存中
	nSize = X_paramPushParamToBuff(byBuff,FLASH_BLOCK_SIZE);
	if (nSize <= 0)
	{ return FALSE; }

	//存储参数
	if (Y_paramSaveSysDefault(byBuff,nSize) != TRUE) 
	{ return FALSE; }//End of if;

      if (X_paramIsAllParamInited() == FALSE) 
        X_SysParamCpy();
	//存储成功
	return TRUE;
}//End of function;

//将当前系统参数存储为用户自定义默认参数，同时也成为系统默认参数
BOOL X_paramSaveAsUsrDefaultParam(void)
{
	BYTE byBuff[FLASH_BLOCK_SIZE];
	int nSize;

	nSize = 0;
	//检查
	if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
	{ return FALSE; }

	//更新设备参数
	X_paramUpdateToDevice();

	//将参数写到缓存中
	nSize = X_paramPushParamToBuff(byBuff,FLASH_BLOCK_SIZE);
	if (nSize <= 0)
	{ return FALSE; }
	
	//存储参数
//	if (Y_paramSaveSysDefault(byBuff,nSize) != TRUE) 
//	{ return FALSE; }//End of if;
	if (Y_paramSaveUsrDefault(byBuff,nSize) != TRUE) 
	{ return FALSE; }//End of if;

	//二次开发的“存储用户默认”
	if (X_param2DevSaveUserDefault != NULL)
	{ X_param2DevSaveUserDefault(); }

	//存储成功
	return TRUE;
}//End of function;

//将系统参数保存为出厂配置
BOOL X_paramSaveAsFactoryParam(void)
{
	BYTE byBuff[FLASH_BLOCK_SIZE];
	int nSize;

	nSize = 0;
	//检查
	if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
	{ return FALSE; }
	
	//更新设备参数
	X_paramUpdateToDevice();
	
	//将参数写到缓存中
	nSize = X_paramPushParamToBuff(byBuff,FLASH_BLOCK_SIZE);
	if (nSize <= 0)
	{ return FALSE; }
	
	//存储参数
	if (Y_paramSaveFactory(byBuff,nSize) != TRUE) 
	{ return FALSE; }//End of if;
	
	//存储成功
	return TRUE;
}//End of function;

//分析字符串，从中提取出关键字及其值
int X_paramAnalysisKeyword(unsigned char* pParam,int nParam,unsigned char* pWord,unsigned char * pValue)
{
	int i = 0, j = 0;

	if (pParam[i] == ';') i ++;
	while (i < nParam)
	{
		if (pParam[i] == 0x5c){  // '\'
			i ++;
			pWord[j++] = pParam[i++];
		}
		else if (pParam[i] == 0x3d) break; // '='
		else pWord[j++] = pParam[i++];

		if (j >= 16) return 0;
	}

	if (j == 0) return 0;
	if (pParam[i] != '=') return 0;
	i ++;

	j = 0;
	while (i < nParam)
	{
		if (pParam[i] == 0x5c){  // '\'
			i ++;
			pValue[j++] = pParam[i++];
		}
		else if (pParam[i] == 0x3b) break; // ';'
		else pValue[j++] = pParam[i++];

		if (j >= 39) return 0;
	}
/*	if (j == 0) {Testf = 1; return 0; }*/  //注释掉该处,防止Value="空"的属性直接中止该循环.使所有属性能正常载入
	if (pParam[i] == ';') i ++;

	return i; //返回偏移量
}

int X_paramGetKeywordsStruct(unsigned char *pBuff,int nBuff,TKEYSEG* tKS)
{
	int nKs;
	int i,j;

	i = 0;
	nKs = 0;
	while(pBuff[i] != 0 && pBuff[i] != 0xff)
	{
		j = X_paramAnalysisKeyword(pBuff+i,nBuff-i,tKS[nKs].pKeyWord,tKS[nKs].pValue);
		if (j == 0) break;
		i += j;
		nKs ++;
		if (nKs >= CUR_KEY_MSG_CNT) break;
	}//End of while;

	return nKs;
}

BOOL X_paramLoadDeviceInfo(void)
{
	UALLPARAM *psParam;
	int n;
	unsigned char pBuff[OEM_BLOCK_VALID_SIZE];

	psParam = X_paramGetGlobalAllParam();

	if (psParam == NULL || psParam->pKeyS == NULL || psParam->pDevInfo == NULL)
	{ return FALSE; }//End of if;


	memset(psParam->pDevInfo,0,sizeof(UTDeviceINFO));

	g_nKeyCnt = 0;
	n = 0;
	//1.读取OEM信息
	memset(pBuff,0,sizeof(pBuff));
	if (Y_paramLoadOEM(pBuff,OEM_BLOCK_VALID_SIZE) == TRUE)
	{
		memset(psParam->pKeyS,0,sizeof(TKEYSEG)*CUR_KEY_MSG_CNT);
		g_nKeyCnt = X_paramGetKeywordsStruct(pBuff,strlen((char*)pBuff),psParam->pKeyS); //把配置串信息格化
		n = 0;
		while (n < g_nKeyCnt)
		{
			if (psParam->pKeyS[n].pValue[0] == 0x00)
			{
				n++;
				continue;
			}//End of if;
			if (strncmp((char*)psParam->pKeyS[n].pKeyWord, "ESN",3) == 0)
			{ sprintf((char*)psParam->pDevInfo->pESN,"%s",(char*)psParam->pKeyS[n].pValue); }//End of if;
			if (strncmp((char*)psParam->pKeyS[n].pKeyWord, "S/N",3) == 0)
			{ 
				sprintf((char*)psParam->pDevInfo->pSN,"%s",(char*)psParam->pKeyS[n].pValue);				
			}//End of if;
			if (strncmp((char*)psParam->pKeyS[n].pKeyWord, "DATE",4) == 0)
			{ sprintf((char*)psParam->pDevInfo->pDate,"%s",(char*)psParam->pKeyS[n].pValue); }//End of if;
			if (strncmp((char*)psParam->pKeyS[n].pKeyWord, "ManufactureID",13) == 0)
			{ sprintf((char*)psParam->pDevInfo->pManufID,"%s",(char*)psParam->pKeyS[n].pValue); }//End of if;
			if (strncmp((char*)psParam->pKeyS[n].pKeyWord, "IsDownload",10) == 0)
			{ psParam->pDevInfo->IsDownload = psParam->pKeyS[n].pValue[0]; }//End of if;
			if (strncmp((char*)psParam->pKeyS[n].pKeyWord, "ManufactureName",15) == 0)
			{ sprintf((char*)psParam->pDevInfo->pManufName,"%s",(char*)psParam->pKeyS[n].pValue); }//End of if;
			if (strncmp((char*)psParam->pKeyS[n].pKeyWord, "DofType",7) == 0)
			{ sprintf((char*)psParam->pDevInfo->pDofType,"%s",(char*)psParam->pKeyS[n].pValue); }//End of if;

			n ++;
		}//End of while;
	}//End of if;

	//返回
	if (0 == g_nKeyCnt)
	{
		return FALSE;
	}

	Z_SetDeviceSerial((char *)(X_paramGetGlobalAllParam()->pDevInfo->pSN), strlen((char *)(X_paramGetGlobalAllParam()->pDevInfo->pSN)));
	Z_SetDeviceManufacturer((char*)(X_paramGetGlobalAllParam()->pDevInfo->pManufName), strlen((char *)(X_paramGetGlobalAllParam()->pDevInfo->pManufName)));

	return TRUE;
}//End of function;

void paramSaveType(unsigned char saveFlag)
{
    cParamSaveFlag = saveFlag;
}

unsigned char getParamSaveType(void)
{
    return cParamSaveFlag;
}

//功能：查询产品关键字信息
//char* pKeyWord ：关键字
//char* pValue   ：读取的信息
//int nBlock     ：读取的块
//int nValid     ：块长度
//BOOL t         ：格式长度
BOOL X_paramQueryKeyword(char* pKeyWord,char* pValue)
{
	int n;
	UALLPARAM *psParam;

      if(getParamSaveType() == 0)
	    psParam = X_paramGetGlobalAllParam();
      else 
          psParam = X_paramBakGetGlobalAllParam();

	if (psParam == NULL || g_nKeyCnt <= 0 || strlen(pKeyWord) == 0)
	{ return FALSE; }

	n = 0;
	while (n < g_nKeyCnt)
	{
		if (strncmp((char*)psParam->pKeyS[n].pKeyWord, pKeyWord, \
			max(strlen((char*)psParam->pKeyS[n].pKeyWord),strlen(pKeyWord))) == 0)
		{
			if(psParam->pKeyS[n].pValue[0] == 0)
			{ return FALSE; }
     	    sprintf(pValue,"%s",(char*)psParam->pKeyS[n].pValue);
			break;
		}//End of if;
		n ++;
	}//End of while;
	if (n == g_nKeyCnt)
	{ return FALSE; }
	return TRUE;
}//End of function;


BOOL X_paramPlaceKeyword(char* pKeyWord,char* pValue, BOOL bReplace)
{
	int n,nLen,i;
	UALLPARAM *psParam;

	if(getParamSaveType() == 0)
	    psParam = X_paramGetGlobalAllParam();
      else 
          psParam = X_paramBakGetGlobalAllParam();
      
	if (psParam == NULL || psParam->pKeyS == NULL)
	{ return FALSE; }

	n = 0;
	while (n < g_nKeyCnt)
	{
		if (strncmp((char*)psParam->pKeyS[n].pKeyWord, pKeyWord, \
			max(strlen((char*)psParam->pKeyS[n].pKeyWord),strlen(pKeyWord))) == 0)
		{ break; }//End of if;
		n ++;
	}//End of while;

	/* 处理输入的值为空的情况 */
	if(strlen(pValue) == 0)
	{
		/* 内存中原先没有这个属性,pValue=0.不对其进行保存 */
		if (n == g_nKeyCnt && g_nKeyCnt < CUR_KEY_MSG_CNT)
		{
			return FALSE;	
		}
		/* 如果该属性原本就存储在Flash中,pValue=0.在内存中清除该属性,并写回Flash */
		if (n < g_nKeyCnt && bReplace == TRUE)
		{
			i = n;
			while(i < g_nKeyCnt)
			{
				memset(psParam->pKeyS[i].pKeyWord,0, strlen((char*)psParam->pKeyS[i].pKeyWord));
				memset(psParam->pKeyS[i].pValue, 0, strlen((char*)psParam->pKeyS[i].pValue));	

				memcpy(psParam->pKeyS[i].pKeyWord, psParam->pKeyS[i+1].pKeyWord, \
					max(strlen((char*)psParam->pKeyS[i].pKeyWord),strlen((char*)psParam->pKeyS[i+1].pKeyWord)));
				memcpy(psParam->pKeyS[i].pValue, psParam->pKeyS[i+1].pValue, \
					max(strlen((char*)psParam->pKeyS[i].pValue),strlen((char*)psParam->pKeyS[i+1].pValue)));
				i ++;
			}
			g_nKeyCnt --;	//减少了一种属性
		}	
	}

	else{
		if (n < g_nKeyCnt && bReplace != TRUE) //有相同关键字表达式,但不允许替换
		{ return FALSE; }
		
		if (n < g_nKeyCnt && bReplace == TRUE)
		{//替换
			nLen = strlen(pValue);
			if (nLen > sizeof(psParam->pKeyS[n].pValue)-1)
			{ return FALSE; } //关键字值字串太长
			memcpy(psParam->pKeyS[n].pValue,pValue,nLen);
			if (nLen < sizeof(psParam->pKeyS[n].pValue))
			{ psParam->pKeyS[n].pValue[nLen] = '\0'; }
		}//End of if;
		
		if (n == g_nKeyCnt && g_nKeyCnt < CUR_KEY_MSG_CNT)
		{//增加
			nLen = strlen(pKeyWord);
			if (nLen > sizeof(psParam->pKeyS[n].pKeyWord)-1)
			{ return FALSE; }
			memcpy(psParam->pKeyS[n].pKeyWord,pKeyWord,nLen);
			if (nLen < sizeof(psParam->pKeyS[n].pKeyWord))
			{ psParam->pKeyS[n].pKeyWord[nLen] = '\0'; }
			nLen = strlen(pValue);
			if (nLen > sizeof(psParam->pKeyS[n].pValue)-1)
			{ return FALSE; } //关键字值字串太长
			memcpy(psParam->pKeyS[n].pValue,pValue,nLen);
			if (nLen < sizeof(psParam->pKeyS[n].pValue))
			{ psParam->pKeyS[n].pValue[nLen] = '\0'; }
			g_nKeyCnt++;
		}//End of if;
	}

// 	if (strncmp((char*)pKeyWord, "DofType",7) == 0){
// 		sprintf((char*)(psParam->pDevInfo->pDofType),"%s",(char*)pValue);
// 		if ((strncmp("HD",pValue,max(2,strlen(pValue))) == 0))
// 			n = PRODUCTION_DOF_NEAR;
// 		else if ((strncmp("SR",pValue,max(2,strlen(pValue))) == 0))
// 			n = PRODUCTION_DOF_MIDDLE;
// 		else if ((strncmp("ER",pValue,max(2,strlen(pValue))) == 0))
// 			n = PRODUCTION_DOF_FAR;
// 		else if (strlen(pValue) == 0)
// 			n = PRODUCTION_DOF_NORMAL;
// 		else
// 			return FALSE;	
// 		if (n != psParam->pHard->nDofType)
// 		{
// 			psParam->pHard->nDofType = n;
// 			
// 			X_paramSaveAsSysDefaultParam();
// 
// 			if (Z_cmos_GetType() == T_CMOS_MT9V022)
// 			{
// 				MT9V022_SetExp();
// 			}
// 
// 		}
// 
// 	}

	if (strncmp((char*)pKeyWord, "ManufactureName",15) == 0){
		sprintf((char*)(psParam->pDevInfo->pManufName),"%s",(char*)pValue);
	}
	
	if (strncmp((char*)pKeyWord, "ESN",3) == 0){
     	sprintf((char*)(psParam->pDevInfo->pESN),"%s",(char*)pValue);
	}
	if (strncmp((char*)pKeyWord, "S/N",3) == 0){
     	sprintf((char*)(psParam->pDevInfo->pSN),"%s",(char*)pValue);
	}
	if (strncmp((char*)pKeyWord, "DATE",4) == 0){
     	sprintf((char*)(psParam->pDevInfo->pDate),"%s",(char*)pValue);
	}
	if (strncmp((char*)pKeyWord, "ManufactureID",13) == 0){
     	sprintf((char*)(psParam->pDevInfo->pManufID),"%s",(char*)pValue);
	}
	if (strncmp((char*)pKeyWord, "IsDownload",10) == 0){
// 		if (strlen(pValue) == 0){
// 			switch(psParam->pHard->InterType) {
// 			case U_PRO_IS_USBRS232:
// 			case U_PRO_IS_DataPipe:
// 				break;
// 
// 			default:
// 				psParam->pDevInfo->IsDownload = 0;
// 				break;
// 			}
// 	
// 		}else
			psParam->pDevInfo->IsDownload = (unsigned char)pValue[0];
	}
	if (strncmp((char*)pKeyWord, "Mirr",4) == 0){
		if (strlen(pValue) == 0){
			psParam->pHard->nProduction = 0;
		}

		for (n = strlen(pValue)-1; n >= 0; n--)
		{//小写化
			if (pValue[n] >= 'A' && pValue[n] <= 'Z')
			{ pValue[n] = 'a'+(pValue[n]-'A'); }
		}//End of for;
		if ((strncmp("on",pValue,max(2,strlen(pValue))) == 0))
			psParam->pHard->nProduction = 3;
		else 
			psParam->pHard->nProduction = 0;
	}

	return TRUE;
}//End of function;

BOOL X_paramSaveDeviceInfo(void)
{
	int i,nLen;
	unsigned char pBuff[OEM_BLOCK_VALID_SIZE+128];
	unsigned char pTmpKey[48],pTmpVal[48];
	UALLPARAM *psParam;

	//psParam = X_paramGetGlobalAllParam();
	if(getParamSaveType() == 0)
	    psParam = X_paramGetGlobalAllParam();
      else 
          psParam = X_paramBakGetGlobalAllParam();
	
	if (psParam == NULL || psParam->pKeyS == NULL)
	{ return FALSE; }
	memset(pBuff,0,OEM_BLOCK_VALID_SIZE);

//	X_paramPlaceKeyword("ESN",(char*)psParam->pDevInfo->pESN,TRUE);
//	X_paramPlaceKeyword("S/N",(char*)psParam->pDevInfo->pSN,TRUE);
//	X_paramPlaceKeyword("DATE",(char*)psParam->pDevInfo->pDate,TRUE);

	nLen = 0;

	for (i = 0; i < g_nKeyCnt;i ++)
	{
		memset(pTmpKey,0,sizeof(pTmpKey));
		memset(pTmpVal,0,sizeof(pTmpVal));
		memcpy(pTmpKey,psParam->pKeyS[i].pKeyWord,sizeof(psParam->pKeyS[i].pKeyWord));
		memcpy(pTmpVal,psParam->pKeyS[i].pValue,min(sizeof(psParam->pKeyS[i].pValue), sizeof(pTmpVal)));
		nLen += sprintf((char*)(pBuff+nLen),"%s=%s;",pTmpKey,pTmpVal);
		if (nLen >= OEM_BLOCK_VALID_SIZE)
		{ break; }
	}//End of for;

	Z_SetDeviceManufacturer((char*)(X_paramGetGlobalAllParam()->pDevInfo->pManufName), strlen((char *)(X_paramGetGlobalAllParam()->pDevInfo->pManufName)));

	if(Y_paramSaveOEM(pBuff,OEM_BLOCK_VALID_SIZE) == TRUE)
	{
		return(Z_SetDeviceSerial((char *)(X_paramGetGlobalAllParam()->pDevInfo->pSN),
			strlen((char *)(X_paramGetGlobalAllParam()->pDevInfo->pSN))));
	}
	return FALSE;
}//End of function;

//新增参数初始化（从旧设备flash中读出的参数跟新增的参数不一致，就执行） 
void x_paramNewParamInit(void)
{
	UALLPARAM *psParam,*psParambak;
	//const unsigned char *ucCodeIDTab1D_2 = CodeIDTab1D_2;
	//const unsigned char *ucCodeIDTab2D_2 = CodeIDTab2D_2;
//	int i;

	psParam = X_paramGetGlobalAllParam();
    psParambak = X_paramBakGetGlobalAllParam();
	switch(psParam->pCode2->ID)
	{
		case 0xCCCCC004:

           //psParam->p1D->unDecTypes |= 0x2;
           //  psParam->pAreaParam->bottom = 60*IMG_HEIGHT_MAX/100;
           // psParam->pAreaParam->top = 40*IMG_HEIGHT_MAX/100;
            //psParam->pAreaParam->right = 60*IMG_WIDTH_MAX/100;
           // psParam->pAreaParam->left = 40*IMG_WIDTH_MAX/100;
           // psParam->pAreaParam->nType = 0;

           // psParambak->pAreaParam->bottom = 60*IMG_HEIGHT_MAX/100;
           // psParambak->pAreaParam->top = 40*IMG_HEIGHT_MAX/100;
           // psParambak->pAreaParam->right = 60*IMG_WIDTH_MAX/100;
           // psParambak->pAreaParam->left = 40*IMG_WIDTH_MAX/100;
           // psParambak->pAreaParam->nType = 0;
			break;		

		
		default:

	//	psParam->p1D->unDecTypes |= 0x2;
     //    psParam->pAreaParam->bottom = 60*IMG_HEIGHT_MAX/100;
     //       psParam->pAreaParam->top = 40*IMG_HEIGHT_MAX/100;
       //     psParam->pAreaParam->right = 60*IMG_WIDTH_MAX/100;
         //   psParam->pAreaParam->left = 40*IMG_WIDTH_MAX/100;
           // psParam->pAreaParam->nType = 0;

          //  psParambak->pAreaParam->bottom = 60*IMG_HEIGHT_MAX/100;
          //  psParambak->pAreaParam->top = 40*IMG_HEIGHT_MAX/100;
          //  psParambak->pAreaParam->right = 60*IMG_WIDTH_MAX/100;
           // psParambak->pAreaParam->left = 40*IMG_WIDTH_MAX/100;
          //  psParambak->pAreaParam->nType = 0;
			break;
	}

}

BOOL X_paramOriginalInitAllParam(void)
{
	UALLPARAM *psParam;

	psParam = X_paramGetGlobalAllParam();
	if (psParam == NULL)
	{ return FALSE; }//End of if;

	//psParam->pstrVersion = g_IMGVersion;

	if (psParam->pCom != NULL)
	{
		memset(psParam->pCom,0,sizeof(TComParam));
		psParam->pCom->lDataBits = 8;
		psParam->pCom->lStopBits = 1;
		psParam->pCom->lParity = NOPARITY;
		psParam->pCom->lRate = 9600;
		psParam->pCom->nflowCtrl = 0;
	}//End of if;
	

	if (psParam->pHard != NULL)
	{
		memset(psParam->pHard,0,sizeof(HardParam));
		//照明灯与对焦灯
		psParam->pHard->nLight = 0;
		psParam->pHard->nFocus = 0;
		psParam->pHard->bOscillation = FALSE;
		psParam->pHard->nOscillationDelay = 100;
		psParam->pHard->nProduction = 0;
		psParam->pHard->bDecVoice = 3;
		psParam->pHard->nBeepType = 0x3000;//每4bit分别表示:解码成功频率,持续时间,音量,类型
		psParam->pHard->InterType = X_paramSelectUsefulCommType();
		psParam->pHard->nDofType = PRODUCTION_DOF_NORMAL;
    	psParam->pHard->nGoodreadLED = 0;//打开
        psParam->pHard->nGoodreadLEDDuration = 0;//短

	}//End of if;


	if (psParam->pCode != NULL)
	{
		memset(psParam->pCode,0,sizeof(CodeParam));
		//前后缀
		psParam->pCode->nPrefix = 0;
		psParam->pCode->nSuffix = 0;
		memset(psParam->pCode->byPrefix, 0, sizeof(psParam->pCode->byPrefix));
		memset(psParam->pCode->bySuffix, 0, sizeof(psParam->pCode->bySuffix));
		//终止字符
		psParam->pCode->nTerminal = 1; 
		psParam->pCode->byTerminal[0] = 0x0D;
		//psParam->pCode->byTerminal[1] = 0x0A;
		//自定义前缀
		psParam->pCode->IsPreSufOn[POS_IsPrefix] = 0;    //不发送自定义前缀
		psParam->pCode->IsPreSufOn[POS_IsSuffix] = 0;    //不发送自定义后缀
		psParam->pCode->IsPreSufOn[POS_IsCodeID] = 0;    //不发送CodeID前缀
		psParam->pCode->IsPreSufOn[POS_IsAIM] = 0;       //不发送AIM前缀
		psParam->pCode->IsPreSufOn[POS_IsTerminal] = 0;  //不发送结束符后缀
		psParam->pCode->IsPreSufOn[POS_IsCut] = 0;      //信息不截取
		psParam->pCode->PrefixOrder = 4; //前缀顺序默认：自定义前缀+CodeID+AIM		-- 2016.09.19
		psParam->pCode->IsOtherPreSufOn = 0; //不发送除标准前后缀外的其它用户前后缀
		//CodeID
		memcpy(psParam->pCode->by1DCodeID, CodeIDTab1D,sizeof(CodeIDTab1D));
		memcpy(psParam->pCode->by2DCodeID, CodeIDTab2D,sizeof(CodeIDTab2D));

		//新增参数，CodeID
		//psParam->pCode2->ID = 0xCCCCC000;
		//psParam->pCode2->ID = 0xCCCCC001;
		psParam->pCode2->ID = 0xCCCCC004;
		psParam->pCode2->nsize = sizeof(CodeParam2);
		psParam->pCode2->cmosAdjustMode = 0;
		memcpy(psParam->pCode2->by1DCodeID_2, CodeIDTab1D_2,sizeof(CodeIDTab1D_2));
		memcpy(psParam->pCode2->by2DCodeID_2, CodeIDTab2D_2,sizeof(CodeIDTab2D_2));
             memset(psParam->pCode2->nCharEncoding, 0, sizeof(psParam->pCode2->nCharEncoding));
		memset(psParam->pCode2->Reserved, 0, sizeof(psParam->pCode2->Reserved));	//0xCCCCC001  add
		psParam->pCode2->bSurePOSPowerUpScanEnable = TRUE;	//0xCCCCC001  add
		psParam->pCode2->nSucBeepDuration = 80;	//解码成功蜂鸣提示持续时间
		psParam->pCode2->nSucBeepFreq = 2730;		//解码成功蜂鸣提示频率
		psParam->pCode2->nSucLedDuration = 20;	//解码成功指示灯持续时间
             memcpy(psParam->pCode2->byPostalCodeID, CodeIDPostal,sizeof(CodeIDPostal));
		memcpy(psParam->pCode2->byPostalCodeID_2, CodeIDPostal_2,sizeof(CodeIDPostal_2));

		//信息编辑
		memset(psParam->pCode->byCut, 0, sizeof(psParam->pCode->byCut));
		//识读模式
		psParam->pCode->nReadMode = ESC0;

		//延迟
		psParam->pCode->nOnceDelay = 3000;//默认:3000ms  Korean: 120000ms   USA: 2000ms
		psParam->pCode->nSuccDelay = 1000;//成功解码延迟 
		psParam->pCode->nInfoDelay = 1500; //延迟1500ms
		//->->
		psParam->pCode->Protect = TRUE; //保护模式
		psParam->pCode->nMedium = ALL;
		psParam->pCode->bIR = FALSE; //
		psParam->pCode->nSensitive = 11; //ESC2 情况下，光线敏感度
		//信息打包
		psParam->pCode->nPack = 0; //不需要任何打包
		//->->->
		psParam->pCode->SetCode = 0;  //默认不发送设置码解码信息
		psParam->pCode->IsGotoConsole = 0; //电源开启时不进入控制台
		psParam->pCode->IsPowerOnDisp = 0; //电源开启时不显示系统信息
		psParam->pCode->nScanTimes = 0; //默认不限定读码次数
		psParam->pCode->IsCompDelay = FALSE;//相同信息,不完全延时
		psParam->pCode->nUserDef = 0;   //普通硬件类型
		//错误信息
		psParam->pCode->bErrMsgEn = 0;
		psParam->pCode->nErrMsg = 1;
		memset(psParam->pCode->byErrMsg, 0, sizeof(psParam->pCode->byErrMsg));
		psParam->pCode->byErrMsg[0] = '?';
	}//End of if;

	if (psParam->pKBW != NULL)
	{
		memset(psParam->pKBW,0,sizeof(UTKeyBoard));
	}
       if (psParam->pKBW != NULL)
	{
		memset(psParam->pKBW,0,sizeof(UTKeyBoard));
		psParam->pKBW->nPollRate = 4;
	}

	if (psParam->p2D != NULL)
	{
		memset(psParam->p2D,0,sizeof(UTDec2DSetup));
		psParam->p2D->unDecTypes = Init_Enable2D;
		psParam->p2D->ExMsg[POS_PDF417] = Init_2D_PDF417_MIN_MAX_LEN;
		psParam->p2D->ExMsg[POS_QR] = Init_2D_QRCODE_MIN_MAX_LEN;
		psParam->p2D->ExMsg[POS_DataMatrix] = Init_2D_DataMatrix_MIN_MAX_LEN;
		psParam->p2D->ExMsg[POS_VeriCode] = Init_2D_VeriCode_MIN_MAX_LEN;
		psParam->p2D->ExMsg[POS_UN_VeriCode] = Init_2D_UnVeriCode_MIN_MAX_LEN;
		psParam->p2D->ExMsg[POS_Aztec] = Init_2D_AZTEC_MIN_MAX_LEN;
		psParam->p2D->ExMsg[POS_HAN_SENSIBLE] = Init_2D_HANSENSIBLE_MIN_MAX_LEN;
		psParam->p2D->ExMsg[POS_MaxiCode] = Init_2D_MaxiCode_MIN_MAX_LEN;

		psParam->p2D->ExMsg[POS_GM_CODE] = Init_2D_GMCODE_MIN_MAX_LEN;					// new
		psParam->p2D->ExMsg[POS_MicroPDF417] = Init_2D_MicroPDF417_MIN_MAX_LEN;			// new

		psParam->p2D->ExParam[POS_PDF417] = Init_PDF417;//DC_PDF417_ECICLOSE
		psParam->p2D->ExParam[POS_QR] = Init_QR;
		psParam->p2D->ExParam[POS_DataMatrix] = Init_DataMatrix;
		psParam->p2D->ExParam[POS_VeriCode] = Init_VeriCode;
		psParam->p2D->ExParam[POS_Aztec] = Init_Aztec;
		psParam->p2D->ExParam[POS_HAN_SENSIBLE] = Init_CSC;

		psParam->p2D->ExParam[POS_GM_CODE] = Init_GM;						// new
		psParam->p2D->ExParam[POS_MicroPDF417] = Init_MicroPDF417;			// new

		psParam->p2D->nMacro = MACRO_DIRECT;
		psParam->p2D->nQRParam = 0;
	}//End of if;

	if (psParam->p1D != NULL)
	{
		memset(psParam->p1D,0,sizeof(UTDec1DSetup));
		//modify by HUNTON,2003.12.26
		psParam->p1D->ExMsg[POS_CODE128] = Init_CODE128;
		psParam->p1D->ExMsg[POS_UCCEAN128] = Init_UCCEAN128;
		psParam->p1D->ExMsg[POS_EAN8] = Init_EAN8;
		psParam->p1D->ExMsg[POS_EAN13] = Init_EAN13;
		psParam->p1D->ExMsg[POS_UPCE] = Init_UPCE;
		psParam->p1D->ExMsg[POS_UPCA] = Init_UPCA;
		psParam->p1D->ExMsg[POS_INTER2OF5] = Init_INTER25;
		psParam->p1D->ExMsg[POS_MATRIX25] = Init_MATRIX25;
		psParam->p1D->ExMsg[POS_MODULE25] = Init_MODULE25;
		psParam->p1D->ExMsg[POS_REGULAR39] = Init_REGULAR39;
		psParam->p1D->ExMsg[POS_FULLASCII39] = Init_FULLASCII39;
		psParam->p1D->ExMsg[POS_CODABAR] = Init_CODABAR;
		psParam->p1D->ExMsg[POS_CODE93] = Init_CODE93;
		psParam->p1D->ExMsg[POS_CODEZ] = Init_CODEZ;

		psParam->p1D->ExMsg[POS_ISBN] = Init_ISBN;
		psParam->p1D->ExMsg[POS_INDUSTRIAL25] = Init_Industrial25;
		psParam->p1D->ExMsg[POS_STANDARD25] = Init_Standard25;
		psParam->p1D->ExMsg[POS_PLESSEY] = Init_Plessey;

		psParam->p1D->ExMsg[POS_CODE11] = Init_CODE11;
		psParam->p1D->ExMsg[POS_MSI_PLESSEY] = Init_MSIPlessey;
		psParam->p1D->ExMsg[POS_COMPOSITE] = Init_COMPOSITE;
		psParam->p1D->ExMsg[POS_RSS] = Init_RSS;
		psParam->p1D->ExMsg[POS_CHNPOST] = Init_CHNPOST25;
		
		psParam->p1D->unDecParam = Init_1DDecParam; //一维条码的整体参数初始化，如是否读双码
		psParam->p1D->unDecTypes = Init_Enable1D; 

	}//End of if;

	if (psParam->pOCR != NULL)
	{
		memset(psParam->pOCR,0,sizeof(UTDecOCRSetup));
		psParam->pOCR->unDecTypes = Init_EnableOCR;
		psParam->pOCR->ExMsg[POS_SPEC_OCR_B] = Init_SPEC_OCR_B;
	}//End of if;

	if(psParam->pDevInfo->IsDownload == '1')
	{
		if(psParam->pHard->InterType == U_PRO_IS_DataPipe || psParam->pHard->InterType == U_PRO_IS_USBRS232 \
			|| psParam->pHard->InterType == U_PRO_IS_Serial)
			X_paramPlaceKeyword((char*)"IsDownload",(char*)"1",TRUE);
		else
			X_paramPlaceKeyword((char*)"IsDownload",(char*)"0",TRUE);
	}		
	else
		X_paramPlaceKeyword((char*)"IsDownload",(char*)"0",TRUE);


    if (psParam->pDFormat !=NULL )
    {
		memset(psParam->pDFormat,0,sizeof(DataFormat));
		psParam->pDFormat->iSsingle = 0;
		psParam->pDFormat->DFEnable = 0;
		psParam->pDFormat->ErrorTone = 1;
		psParam->pDFormat->outFlag = 0;
		psParam->pDFormat->usedID = 0;
		psParam->pDFormat->singleFormatID = -1;
		// DF1要初始化  指令为按原样输出
    }

    if(psParam->pAreaParam != NULL)
    {   
            psParam->p1D->unDecTypes |= 0x2;
            psParam->pAreaParam->bottom = 60*Z_GetCmosHeight()/100;
            psParam->pAreaParam->top = 40*Z_GetCmosHeight()/100;
            psParam->pAreaParam->right = 60*Z_GetCmosWidth()/100;
            psParam->pAreaParam->left = 40*Z_GetCmosWidth()/100;
            psParam->pAreaParam->nType = 0;
    }
// 	if (psParam->pHard != NULL)
// 	{
// 		psParam->pHard->eMode = eCMD_MODE_NORMAL;
// 	}

	if (X_paramOriginalUserDefault != NULL)
	{
		X_paramOriginalUserDefault(psParam);
	}//End of if;

    if (psParam->pParamInfo !=NULL )
	{
		psParam->pParamInfo->paramLen = Total_MEMSize;
		psParam->pParamInfo->code2StartOffset = Start_MEMSize_CODE2;
		psParam->pParamInfo->paramVersion = PARAMVERSION;

	}

	//更新设备参数
	X_paramUpdateToDevice();
    
    if (X_paramIsAllParamInited() == FALSE) //仅在上电初始化时拷贝参数到备份区，用设置码设置时不用拷贝
       X_SysParamCpy();
      
	return TRUE;
}//End of function;


/************************************************************************/
/* 解释配置信息                                                         */
/************************************************************************/

extern FNC_commAddVerionMsg			V_commAddVerMsg;

void X_paramSendParamInfo(void)
{
	char buff[1024*2] = {0};
	int n, i;
	int bufflentemp = 0;
	UALLPARAM *psParam;
      int usedNo;
      //printf("sendinfo\n");
	__PARAM_INIT_();
	psParam = X_paramGetGlobalAllParam();
	if (psParam == NULL)
	{ return; }

	//获取设备的Firmware版本
	i = Z_GetHardwareInfo(buff, 1024);
	buff[i] = 0;

	if (V_commAddVerMsg != NULL)
	{ i += V_commAddVerMsg(buff+i,sizeof(buff)-i); }

	//版本信息
	sprintf(buff + strlen(buff),
		"\r\nSupport BarCode %sread.\r\n%s\r\n",
		(psParam->pOCR->unDecTypes & DC_SPEC_OCR_B) ? "& OCR-B " : "",
		UIMAGE_PRI_VERSION
		);
//	if (psParam->pOCR->unDecTypes & DC_SPEC_OCR_B)
//		sprintf(buff+strlen(buff),"\r\nDev: BarCode & OCR-B Reader\r\nVer: %s\r\n",UIMAGE_PRI_VERSION);
//	else 
//		sprintf(buff+strlen(buff),"\r\nDev: BarCode Reader\r\nVer: %s\r\n",UIMAGE_PRI_VERSION);

	/************************************************************************/
	/* 读取出出厂参数信息                                                   */
	/************************************************************************/
	n = strlen((char*)psParam->pDevInfo->pDate);
	if (n > 0)
		sprintf(buff+strlen(buff),"Date: %s\r\n",psParam->pDevInfo->pDate);
	else 
		sprintf(buff+strlen(buff),"Date: NULL\r\n");
	n = strlen((char*)psParam->pDevInfo->pSN);
	if (n > 0)
		sprintf(buff+strlen(buff),"S/N: %s\r\n",psParam->pDevInfo->pSN);
	else 
		sprintf(buff+strlen(buff),"S/N: NULL\r\n");

	/************************************************************************/
	/* 读取出OEM 参数信息                                                  */
	/************************************************************************/
	n = strlen((char*)psParam->pDevInfo->pESN);
	if (n > 0)
		sprintf(buff+strlen(buff),"ESN: %s\r\n",psParam->pDevInfo->pESN);
	else 
		sprintf(buff+strlen(buff),"ESN: Null\r\n");

	n = strlen((char*)psParam->pDevInfo->pManufID);
	if (n > 0)
		sprintf(buff+strlen(buff),"ManufactureID: %s\r\n",psParam->pDevInfo->pManufID);
	else 
		sprintf(buff+strlen(buff),"ManufactureID: Null\r\n");

	n = strlen((char*)psParam->pDevInfo->pManufName);
	if (n > 0)
		sprintf(buff+strlen(buff),"ManufactureName: %s\r\n",psParam->pDevInfo->pManufName);
	else 
		sprintf(buff+strlen(buff),"ManufactureName: Null\r\n");
	/************************************************************************/
	/*   系统通讯                                                           */
	/************************************************************************/
	sprintf(buff+strlen(buff),"Interface: \r\n\tDefault:  ");
	
	switch(psParam->pHard->InterType) {
	case U_PRO_IS_HID_KBW:
		sprintf(buff+strlen(buff),"USB(HID-KBW)\r\n");
		break;
	case U_PRO_IS_HID_POS:
		sprintf(buff+strlen(buff),"USB(HID-POS)\r\n");
		break;
	case U_PRO_IS_SurePOS_TT:
		sprintf(buff+strlen(buff),"USB(IBM SurePOS Table-Top)\r\n");
		break;
	case U_PRO_IS_SurePOS_HH:
		sprintf(buff+strlen(buff),"USB(IBM SurePOS Hand-Held)\r\n");
		break;
	case U_PRO_IS_USBRS232:
		sprintf(buff+strlen(buff),"USB(Virtual RS232)\r\n");
		break;
	case U_PRO_IS_DataPipe:
		sprintf(buff+strlen(buff),"USB(DataPipe)\r\n");
		break;
	case U_PRO_IS_BLUE_TOOTH:
		sprintf(buff+strlen(buff),"Blue Tooth\r\n");
		break;
	case U_PRO_IS_PS2:
		sprintf(buff+strlen(buff),"PS/2\r\n");
		break;
	case U_PRO_IS_CDC:
		sprintf(buff+strlen(buff),"USB CDC\r\n");
		break;
	default:
		sprintf(buff+strlen(buff),"RS232(%ldbps,%ld,%ld,%ld)\r\n",
			psParam->pCom->lRate,psParam->pCom->lDataBits, \
			psParam->pCom->lParity,psParam->pCom->lStopBits);
	}
	
	sprintf(buff+strlen(buff),"\tCurrent:  ");

	switch(Y_commGetCurType()) {
	case COMMT_KBW:
		sprintf(buff+strlen(buff),"USB(HID-KBW)\r\n");
		break;
	case COMMT_HID_POS:
		sprintf(buff+strlen(buff),"USB(HID-POS)\r\n");
		break;
	case COMMT_IBM_SurePOS_TT:
		sprintf(buff+strlen(buff),"USB(IBM SurePOS Table-Top)\r\n");
		break;
	case COMMT_IBM_SurePOS_HH:
		sprintf(buff+strlen(buff),"USB(IBM SurePOS Hand-Held)\r\n");
		break;
	case COMMT_USB_RS232:
		sprintf(buff+strlen(buff),"USB(Virtual RS232)\r\n");
		break;
	case COMMT_USB_Data:
		sprintf(buff+strlen(buff),"USB(DataPipe)\r\n");
		break;
	case COMMT_BLUE_TOOTH:
		sprintf(buff+strlen(buff),"Blue Tooth\r\n");
		break;
	case COMMT_PS2:
		sprintf(buff+strlen(buff),"PS/2\r\n");
		break;
	case COMMT_USB_CDC:
		sprintf(buff+strlen(buff),"USB CDC\r\n");
		break;	
	default:
		sprintf(buff+strlen(buff),"RS232(%ldbps,%ld,%ld,%ld)\r\n",
			psParam->pCom->lRate,psParam->pCom->lDataBits, \
			psParam->pCom->lParity,psParam->pCom->lStopBits);
	}
	

	/************************************************************************/
	/*  识别的种类及其相关参数                                              */
	/************************************************************************/
	sprintf(buff+strlen(buff),"1D:\r\n");
	bufflentemp = strlen(buff);
      if((psParam->p1D->unDecTypes & 0xf0) != 0) sprintf(buff+strlen(buff),"\t");
	if((psParam->p1D->unDecTypes & 0xf0) != 0) sprintf(buff+strlen(buff),"\t");

	for (n = 1;n < 5; n ++){
		if ((psParam->p1D->unDecTypes & (1<<(n + 3))) == 0) continue;
		if (n == 1) sprintf(buff+strlen(buff),"EAN-8  ");
		if (n == 2) sprintf(buff+strlen(buff),"EAN-13  ");
		if (n == 3) sprintf(buff+strlen(buff),"UPC-E  ");
		if (n == 4) sprintf(buff+strlen(buff),"UPC-A");
	}
	if((psParam->p1D->unDecTypes & 0xf0) != 0) sprintf(buff+strlen(buff),"\r\n");

	// Code 128
	if (psParam->p1D->unDecTypes & DC_CODE128)
	{	
		sprintf(buff+strlen(buff),"\tCode128(");

// 		if (psParam->p1D->ExMsg[POS_CODE128]&(0x01<<8)){
// 			sprintf(buff+strlen(buff),"No Check Digit,");
// 		}
// 		else {
// 			sprintf(buff+strlen(buff),"Check Digit,");
// 		}
		sprintf(buff+strlen(buff),"%d->%d)\r\n",
			(psParam->p1D->ExMsg[POS_CODE128]&0x00ff0000)>>16,
			(psParam->p1D->ExMsg[POS_CODE128]&0xff000000)>>24);
	}
	
	//UCCEAN128
	if (psParam->p1D->unDecTypes & DC_UCCEAN128)
	{	
		sprintf(buff+strlen(buff),"\tUCC/EAN128(%d->%d)\r\n",
			(psParam->p1D->ExMsg[POS_UCCEAN128]&0x00ff0000)>>16,
			(psParam->p1D->ExMsg[POS_UCCEAN128]&0xff000000)>>24);
	}

	// I2Of5
	if (psParam->p1D->unDecTypes & DC_INTER2OF5)
	{
		sprintf(buff+strlen(buff),"\tI2Of5(");
		if (psParam->p1D->ExMsg[POS_INTER2OF5] & 0x03){
			sprintf(buff+strlen(buff),"Check Digit,");
		}
		else {
			sprintf(buff+strlen(buff),"No Check Digit,");
		}
		if (psParam->p1D->I2Of5Len[0] == 0){//固定范围
			sprintf(buff+strlen(buff),"%d->%d",
				(psParam->p1D->ExMsg[POS_INTER2OF5]&0x00ff0000)>>16,
				(psParam->p1D->ExMsg[POS_INTER2OF5]&0xff000000)>>24);
		}
		else{  //特定几个识别长度
			sprintf(buff+strlen(buff),"Fixed Length:");
			for(i = 0; i < 4; i++){
				for(n = 0; n < 8; n++){
					if(psParam->p1D->I2Of5Len[i + 1] & (0x01 << n))
						sprintf(buff+strlen(buff),"%d ", (i << 4) + ((n + 1) << 1));
				}
			}
			if(psParam->p1D->I2Of5Len[1] == 0 && psParam->p1D->I2Of5Len[2] == 0
			&& psParam->p1D->I2Of5Len[3] == 0 && psParam->p1D->I2Of5Len[4] == 0){
				sprintf(buff+strlen(buff),"Null");
			}
		}
		
		sprintf(buff+strlen(buff),")\r\n");
	}
	//ITF14
	if (psParam->p1D->unDecTypes & DC_ITF14)
	{
		sprintf(buff+strlen(buff),"\tITF14(");
		if (psParam->p1D->ExMsg[POS_INTER2OF5] & 0x04){
			sprintf(buff+strlen(buff),"Check Digit)\r\n");
		}
		else {
			sprintf(buff+strlen(buff),"No Check Digit)\r\n");
		}
	}
	//ITF6
	if (psParam->p1D->unDecTypes & DC_ITF6)
	{
		sprintf(buff+strlen(buff),"\tITF6(");
		if (psParam->p1D->ExMsg[POS_INTER2OF5] & 0x08){
			sprintf(buff+strlen(buff),"Check Digit)\r\n");
		}
		else {
			sprintf(buff+strlen(buff),"No Check Digit)\r\n");
		}
	}	
	//MATRIX25
	if (psParam->p1D->unDecTypes & DC_MATRIX25)
	{
		sprintf(buff+strlen(buff),"\tMatrix 25(");
		if (psParam->p1D->ExMsg[POS_MATRIX25] & 0x03){
			sprintf(buff+strlen(buff),"Check Digit,");
		}
		else {
			sprintf(buff+strlen(buff),"No Check Digit,");
		}
		sprintf(buff+strlen(buff),"%d->%d)\r\n",
			(psParam->p1D->ExMsg[POS_MATRIX25]&0x00ff0000)>>16,
			(psParam->p1D->ExMsg[POS_MATRIX25]&0xff000000)>>24);
	}
	// CodaBar
	if (psParam->p1D->unDecTypes & DC_CODABAR)
	{
		sprintf(buff+strlen(buff),"\tCodaBar(");
		if (psParam->p1D->ExMsg[POS_CODABAR] &0x03){
			sprintf(buff+strlen(buff),"Check Digit,");
		}
		else{ 
			sprintf(buff+strlen(buff),"No Check Digit,");
		}
		sprintf(buff+strlen(buff),"%d->%d)\r\n",(psParam->p1D->ExMsg[POS_CODABAR]&0x00ff0000)>>16,(psParam->p1D->ExMsg[POS_CODABAR]&0xff000000)>>24);
	}

	//Code 39 Regular & Ascii
	if (psParam->p1D->unDecTypes & DC_REGULAR39)
	{
		sprintf(buff+strlen(buff),"\tCode39(");
		if (psParam->p1D->ExMsg[POS_REGULAR39] & 0x08){
			sprintf(buff+strlen(buff),"+FULL ASCII,");
		}
		if (psParam->p1D->ExMsg[POS_REGULAR39] &0x03){
			sprintf(buff+strlen(buff),"Check Digit,");
		}
		else{ 
			sprintf(buff+strlen(buff),"No Check Digit,");
		}
		if (psParam->p1D->ExMsg[POS_REGULAR39] & BIT_Code32SpecEdit)
		{
			sprintf(buff+strlen(buff),"Code 32 on,");			
		}
		else
			sprintf(buff+strlen(buff),"Code 32 off,");			


		sprintf(buff+strlen(buff),"%d->%d)\r\n",(psParam->p1D->ExMsg[POS_REGULAR39]&0x00ff0000)>>16,(psParam->p1D->ExMsg[POS_REGULAR39]&0xff000000)>>24);
	}

	//Code 93
	if (psParam->p1D->unDecTypes & DC_CODE93)
	{
		sprintf(buff+strlen(buff),"\tCode93(");
		if (psParam->p1D->ExMsg[POS_CODE93] & 0x03){
			sprintf(buff+strlen(buff),"Check Digit,");
		}
		else {
			sprintf(buff+strlen(buff),"No Check Digit,");
		}
		sprintf(buff+strlen(buff),"%d->%d)\r\n",
			(psParam->p1D->ExMsg[POS_CODE93]&0x00ff0000)>>16,
			(psParam->p1D->ExMsg[POS_CODE93]&0xff000000)>>24);
	}

	//ISSN
	if (psParam->p1D->unDecTypes & DC_ISSN)
		sprintf(buff+strlen(buff),"\tISSN\r\n");

	//ISBN
	if (psParam->p1D->unDecTypes & DC_ISBN)
		sprintf(buff+strlen(buff),"\tISBN\r\n");

	//INDUSTRIAL25
	if (psParam->p1D->unDecTypes & DC_INDUSTRIAL25)
	{
		sprintf(buff+strlen(buff),"\tIndustrial25(");
		if (psParam->p1D->ExMsg[POS_INDUSTRIAL25] & 0x01){
			sprintf(buff+strlen(buff),"Check Digit,");
		}
		else {
			sprintf(buff+strlen(buff),"No Check Digit,");
		}
		sprintf(buff+strlen(buff),"%d->%d)\r\n",
			(psParam->p1D->ExMsg[POS_INDUSTRIAL25]&0x00ff0000)>>16,
			(psParam->p1D->ExMsg[POS_INDUSTRIAL25]&0xff000000)>>24);
	}

	//STANDARD25
	if (psParam->p1D->unDecTypes & DC_STANDARD25)
	{
		sprintf(buff+strlen(buff),"\tStandard25(");
		if (psParam->p1D->ExMsg[POS_STANDARD25] & 0x01){
			sprintf(buff+strlen(buff),"Check Digit,");
		}
		else {
			sprintf(buff+strlen(buff),"No Check Digit,");
		}
		sprintf(buff+strlen(buff),"%d->%d)\r\n",
			(psParam->p1D->ExMsg[POS_STANDARD25]&0x00ff0000)>>16,
			(psParam->p1D->ExMsg[POS_STANDARD25]&0xff000000)>>24);
	}

	//PLESSEY
	if (psParam->p1D->unDecTypes & DC_PLESSEY)
	{
		sprintf(buff+strlen(buff),"\tPlessey(");
		if (psParam->p1D->ExMsg[POS_PLESSEY] & 0x01){
			sprintf(buff+strlen(buff),"Check Digit,");
		}
		else {
			sprintf(buff+strlen(buff),"No Check Digit,");
		}
		sprintf(buff+strlen(buff),"%d->%d)\r\n",
			(psParam->p1D->ExMsg[POS_PLESSEY]&0x00ff0000)>>16,
			(psParam->p1D->ExMsg[POS_PLESSEY]&0xff000000)>>24);
	}

	//CODE11
	if (psParam->p1D->unDecTypes & DC_CODE11)
	{
		sprintf(buff+strlen(buff),"\tCode11(");
		if (psParam->p1D->ExMsg[POS_CODE11] & 0x07){
			sprintf(buff+strlen(buff),"Check Digit,");
		}
		else {
			sprintf(buff+strlen(buff),"No Check Digit,");
		}
		sprintf(buff+strlen(buff),"%d->%d)\r\n",
			(psParam->p1D->ExMsg[POS_CODE11]&0x00ff0000)>>16,
			(psParam->p1D->ExMsg[POS_CODE11]&0xff000000)>>24);
	}
	//MSI PLESSEY
	if (psParam->p1D->unDecTypes & DC_MSI_PLESSEY)
	{
		sprintf(buff+strlen(buff),"\tMSI Plessey(");
		if (psParam->p1D->ExMsg[POS_MSI_PLESSEY] & 0x03){
			sprintf(buff+strlen(buff),"Check Digit,");
		}
		else {
			sprintf(buff+strlen(buff),"No Check Digit,");
		}
		sprintf(buff+strlen(buff),"%d->%d)\r\n",
			(psParam->p1D->ExMsg[POS_MSI_PLESSEY]&0x00ff0000)>>16,
			(psParam->p1D->ExMsg[POS_MSI_PLESSEY]&0xff000000)>>24);
	}

	//COMPOSITE
	if (psParam->p1D->unDecTypes & DC_COMPOSITE)
		sprintf(buff+strlen(buff),"\tUCC/EAN Composite\r\n");

	//RSS
	if (psParam->p1D->unDecTypes & DC_RSS)
		sprintf(buff+strlen(buff),"\tGS1 Databar\r\n");

	//MATRIX25
	if (psParam->p1D->unDecTypes & DC_CHNPOST)
	{
		sprintf(buff+strlen(buff),"\tChina Post 25(");
		if (psParam->p1D->ExMsg[POS_CHNPOST] & 0x03){
			sprintf(buff+strlen(buff),"Check Digit,");
		}
		else {
			sprintf(buff+strlen(buff),"No Check Digit,");
		}
		sprintf(buff+strlen(buff),"%d->%d)\r\n",
			(psParam->p1D->ExMsg[POS_CHNPOST]&0x00ff0000)>>16,
			(psParam->p1D->ExMsg[POS_CHNPOST]&0xff000000)>>24);
	}

	if(bufflentemp == (int)strlen(buff)) sprintf(buff+strlen(buff),"\tAll Disable\r\n");
	
	sprintf(buff+strlen(buff),"2D:\r\n");
	bufflentemp = strlen(buff);
	if (psParam->p2D->unDecTypes & DC_PDF417)
	{
		sprintf(buff+strlen(buff),"\tPDF417(%d->%d)\r\n",
			(psParam->p2D->ExMsg[POS_PDF417]&0x0000ffff),
			psParam->p2D->ExMsg[POS_PDF417] >> 16);
	}
	if (psParam->p2D->unDecTypes & DC_QRCODE)
	{
		sprintf(buff+strlen(buff),"\tQR Code(%d->%d), Micro QR ",
			(psParam->p2D->ExMsg[POS_QR]&0x0000ffff),
			psParam->p2D->ExMsg[POS_QR] >> 16);
		if (psParam->p2D->ExParam[POS_QR] & (1<<POS_QR_MicroQR_ON))
		{
			sprintf(buff+strlen(buff),"Off\r\n");
		}
		else if (!(psParam->p2D->ExParam[POS_QR] & (1<<POS_QR_MicroQR_ON)))
		{
			sprintf(buff+strlen(buff),"On\r\n");
		}

	}
	if (psParam->p2D->unDecTypes & DC_DataMatrix)
	{
		sprintf(buff+strlen(buff),"\tDataMatrix(%d->%d)\r\n",
			(psParam->p2D->ExMsg[POS_DataMatrix]&0x0000ffff),
			psParam->p2D->ExMsg[POS_DataMatrix] >> 16);
	}
	if (psParam->p2D->unDecTypes & DC_AZTEC)
	{
		sprintf(buff+strlen(buff),"\tAztec(%d->%d)\r\n",
			(psParam->p2D->ExMsg[POS_Aztec]&0x0000ffff),
			psParam->p2D->ExMsg[POS_Aztec] >> 16);
	}
	if (psParam->p2D->unDecTypes & DC_MAXICODE)
	{
		sprintf(buff+strlen(buff),"\tMaxiCode(%d->%d)\r\n",
			(psParam->p2D->ExMsg[POS_MaxiCode]&0x0000ffff),
			psParam->p2D->ExMsg[POS_MaxiCode] >> 16);
	}

	if (psParam->p2D->unDecTypes & DC_HAN_SENSIBLE)
	{
		sprintf(buff+strlen(buff),"\tChinese Sensible Code(%d->%d)\r\n",
			(psParam->p2D->ExMsg[POS_HAN_SENSIBLE]&0x0000ffff),
			psParam->p2D->ExMsg[POS_HAN_SENSIBLE] >> 16);
	}

/*	
	if(psParam->p2D->unDecTypes & DC_HAN_GM_CODE)
	{
		sprintf(buff+strlen(buff),"\tGM Code(%d->%d)\r\n",
			(psParam->p2D->ExMsg[POS_GM_CODE]&0x0ffff),
			psParam->p2D->ExMsg[POS_GM_CODE] >> 16);
	}
*/
	
	if(psParam->p2D->unDecTypes & DC_HAN_MicroPDF417)
	{
		sprintf(buff+strlen(buff),"\tMicroPDF417 Code(%d->%d)\r\n",
			(psParam->p2D->ExMsg[POS_MicroPDF417]&0x0ffff),
			psParam->p2D->ExMsg[POS_MicroPDF417] >> 16);
	}

	if(bufflentemp == (int)strlen(buff)) sprintf(buff+strlen(buff),"\tAll Disable\r\n");
	sprintf(buff+strlen(buff),"Postal:\r\n");
	bufflentemp = strlen(buff);

	if(psParam->pPostal->unDecTypes & DC_POSTNET)
		sprintf(buff+strlen(buff),"\tUSPS PostNet\r\n");
	
	if(psParam->pPostal->unDecTypes & DC_ONECODE)
		sprintf(buff+strlen(buff),"\tUSPS Intelligent Mail\r\n");

	if(psParam->pPostal->unDecTypes & DC_RM4SCC)
		sprintf(buff+strlen(buff),"\tRoyal Mail\r\n");

	if(psParam->pPostal->unDecTypes & DC_PLANET)
		sprintf(buff+strlen(buff),"\tUSPS Planet\r\n");

	if(psParam->pPostal->unDecTypes & DC_KIXCODE)
		sprintf(buff+strlen(buff),"\tKIX Post\r\n");

	if(psParam->pPostal->unDecTypes & DC_AUSPOST)
		sprintf(buff+strlen(buff),"\tAustralian Postal \r\n");

	if(bufflentemp == (int)strlen(buff)) sprintf(buff+strlen(buff),"\tAll Disable\r\n");

	sprintf(buff+strlen(buff),"OCR:\r\n");

	bufflentemp = strlen(buff);

	if (psParam->pOCR->unDecTypes & DC_SPEC_OCR_B)
	{
// 		sprintf(buff+strlen(buff),"OCR:\r\n");
		sprintf(buff+strlen(buff),"\tSpecifically OCR-B(%d->%d)\r\n",
			(psParam->pOCR->ExMsg[POS_SPEC_OCR_B]&0x00ff0000)>>16,
			(psParam->pOCR->ExMsg[POS_SPEC_OCR_B]&0xff000000)>>24);
	}

	if (psParam->pOCR->unDecTypes & DC_PASSPORT_VISAS)
		sprintf(buff+strlen(buff),"\tPassport\r\n");

	if(bufflentemp == (int)strlen(buff)) sprintf(buff+strlen(buff),"\tAll Disable\r\n");
	bufflentemp = strlen(buff);

	
	/************************************************************************/
	/*  识别模式及其相关参数                                                */
	/************************************************************************/

	sprintf(buff+strlen(buff),"Scan Mode: ");
	if (g_Code.nReadMode == ESC2){
		sprintf(buff+strlen(buff),"Auto Scan\r\n");
//		sprintf(buff+strlen(buff),"\tOnce Delay: %dms\r\n",g_Code.nOnceDelay);
//		sprintf(buff+strlen(buff),"\tSuccess Read Delay: %dms\r\n",g_Code.nSuccDelay);
//		sprintf(buff+strlen(buff),"\tSame Info Delay: %dms\r\n",g_Code.nInfoDelay);
//		sprintf(buff+strlen(buff),"\tLight Sensitivity: %d\r\n",g_Code.nSensitive);
	}
	else if (g_Code.nReadMode == ESC3)
		sprintf(buff+strlen(buff),"Continue Scan\r\n");
	else
		sprintf(buff+strlen(buff),"Manual Scan\r\n");

	switch(psParam->pCode2->cmosAdjustMode)
	{
	case 0:
		sprintf(buff+strlen(buff),"Printed Bar Codes Read Mode\r\n");
		break;
	case 2:
		sprintf(buff+strlen(buff),"Mobile Phone Read Mode 2\r\n");
		break;
	case 3:
		sprintf(buff+strlen(buff),"Printed Bar Codes Read Mode 2\r\n");
		break;
	case 4:
		sprintf(buff+strlen(buff),"Mobile Phone Read Mode\r\n");
		break;
	}

	switch(psParam->pHard->InterType) {
	case U_PRO_IS_SurePOS_TT:
	case U_PRO_IS_SurePOS_HH:
		if (psParam->pCode2->bSurePOSPowerUpScanEnable)
			sprintf(buff+strlen(buff),"IBM SurePOS Power Up Statue: Enable Scan\r\n");
		else
			sprintf(buff+strlen(buff),"IBM SurePOS Power Up Statue: Disable Scan\r\n");
		break;
	}

	sprintf(buff+strlen(buff),"One Reading Timeout: %d ms\r\n", psParam->pCode->nOnceDelay);


	if (psParam->pCode->IsCompDelay)
		sprintf(buff+strlen(buff),"Reread Delay Enable: %d ms\r\n", psParam->pCode->nInfoDelay);
	else
		sprintf(buff+strlen(buff),"Reread Delay Disable: %d ms\r\n", psParam->pCode->nInfoDelay);

	switch(psParam->pHard->nLight)
	{
	case 0:
		sprintf(buff+strlen(buff),"Light Normal,  ");
		break;
	case 1:
		sprintf(buff+strlen(buff),"Light Always On,  ");
		break;
	case 2:
		sprintf(buff+strlen(buff),"Light Always Off,  ");
		break;
	case 3:
		sprintf(buff+strlen(buff),"Light On When Reading,  ");
		break;
	}

	switch(psParam->pHard->nFocus)
	{
	case 0:
		sprintf(buff+strlen(buff),"AIM Normal\r\n");
		break;
	case 1:
		sprintf(buff+strlen(buff),"AIM Always On\r\n");
		break;
	case 2:
		sprintf(buff+strlen(buff),"AIM Always Off\r\n");
		break;
	case 3:
		sprintf(buff+strlen(buff),"AIM Sensor Mode\r\n");
		break;
	}

// 	switch(psParam->pHard->nExLight)
// 	{
// 	case 0:
// 		sprintf(buff+strlen(buff),"External Light Off\r\n");
// 		break;
// 	case 1:
// 		sprintf(buff+strlen(buff),"External Light On\r\n");
// 		break;
// 	}

	sprintf(buff+strlen(buff),"Good Read Indicators: \r\n");

	if (!(X_paramGetGlobalAllParam()->pHard->bDecVoice & 1))
		sprintf(buff+strlen(buff),"\tBeep Good Read Off, ");
	else
		sprintf(buff+strlen(buff),"\tBeep Good Read On, ");

	switch(X_paramGetGlobalAllParam()->pHard->nBeepType & 0x0f){
	case 0:
		sprintf(buff+strlen(buff),"Type: 1, ");
		break;
	case 1:
		sprintf(buff+strlen(buff),"Type: 2, ");
		break;
	case 2:
		sprintf(buff+strlen(buff),"Type: 3, ");
		break;
	}

	switch((X_paramGetGlobalAllParam()->pHard->nBeepType>>4) & 0x0f){
	case 0:
		sprintf(buff+strlen(buff),"Volume: High, ");
		break;
	case 1:
		sprintf(buff+strlen(buff),"Volume: Medium, ");
		break;
	case 2:
		sprintf(buff+strlen(buff),"Volume: Low, ");
		break;
	case 3:
		sprintf(buff+strlen(buff),"Volume: Off, ");
		break;
	}
	
	//EM2037: bit15 - bit12	频率	0 = Medium; 1 = Lowest; 2 = Low; 3 = Medium; 4 = High; 9 = 用户定义
	//频率暂定为：800、 1600、 2730、 4200。
	switch((X_paramGetGlobalAllParam()->pHard->nBeepType>>12) & 0x0f){
	case 0:
		sprintf(buff+strlen(buff),"Pitch: Medium, ");
		break;
	case 1:
		sprintf(buff+strlen(buff),"Pitch: Lowest, ");
		break;
	case 2:
		sprintf(buff+strlen(buff),"Pitch: Low, ");
		break;
	case 3:
		sprintf(buff+strlen(buff),"Pitch: Medium, ");
		break;
	case 4:
		sprintf(buff+strlen(buff),"Pitch: High, ");
		break;
	case 9:
		sprintf(buff+strlen(buff),"Pitch: %dHz, ", X_paramGetGlobalAllParam()->pCode2->nSucBeepFreq);
		break;
	}

	//EM2037: bit11 - bit8	持续时间 0 = Medium; 1 = short
	switch((X_paramGetGlobalAllParam()->pHard->nBeepType>>8) & 0x0f){
	case 0:
		sprintf(buff+strlen(buff),"Duration: Medium\r\n");
		break;
	case 1:
		sprintf(buff+strlen(buff),"Duration: Short\r\n");
		break;
	case 9:
		sprintf(buff+strlen(buff),"Duration: %dms\r\n", X_paramGetGlobalAllParam()->pCode2->nSucBeepDuration);
		break;
	}
	
	if (X_paramGetGlobalAllParam()->pHard->nGoodreadLED & 1)
		sprintf(buff+strlen(buff),"\tLED Indicator Good Read Off, ");
	else
		sprintf(buff+strlen(buff),"\tLED Indicator Good Read On, ");


	switch(X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration)
	{
	case 0:
		sprintf(buff+strlen(buff),"Duration: Short\r\n");
		break;
	case 1:
		sprintf(buff+strlen(buff),"Duration: Medium\r\n");
		break;
	case 2:
		sprintf(buff+strlen(buff),"Duration: Long\r\n");
		break;
	case 3:
		sprintf(buff+strlen(buff),"Duration: Longest\r\n");
		break;
	case 4:
		sprintf(buff+strlen(buff),"Duration: %dms\r\n", X_paramGetGlobalAllParam()->pCode2->nSucLedDuration);
		break;
	}

	sprintf(buff+strlen(buff),"USB HID-KBW:\r\n\t ");

	switch(X_paramGetGlobalAllParam()->pKBW->nLanguage) {
	case 0: 	sprintf(buff+strlen(buff),"US, ");						break;
	case 1: 	sprintf(buff+strlen(buff),"Belgium, ");					break;
	case 2: 	sprintf(buff+strlen(buff),"Brazil, ");					break;
	case 3: 	sprintf(buff+strlen(buff),"Canada (French), ");			break;
	case 4: 	sprintf(buff+strlen(buff),"Czechoslovakia, ");			break;
	case 5: 	sprintf(buff+strlen(buff),"Denmark, ");					break;
	case 6: 	sprintf(buff+strlen(buff),"Finland (Swedish), ");		break;
	case 7: 	sprintf(buff+strlen(buff),"France, ");					break;
	case 8: 	sprintf(buff+strlen(buff),"Germany/Austria, ");			break;
	case 9: 	sprintf(buff+strlen(buff),"Greece, ");					break;
	case 10: 	sprintf(buff+strlen(buff),"Hungary, ");					break;
	case 11: 	sprintf(buff+strlen(buff),"Israel (Hebrew), ");			break;
	case 12: 	sprintf(buff+strlen(buff),"Italy, ");					break;
	case 13: 	sprintf(buff+strlen(buff),"Latin-American, ");			break;
	case 14: 	sprintf(buff+strlen(buff),"Netherlands (Dutch), ");		break;
	case 15: 	sprintf(buff+strlen(buff),"Norway, ");					break;
	case 16: 	sprintf(buff+strlen(buff),"Poland, ");					break;
	case 17: 	sprintf(buff+strlen(buff),"Portugal, ");				break;
	case 18: 	sprintf(buff+strlen(buff),"Romania, ");					break;
	case 19: 	sprintf(buff+strlen(buff),"Russia, ");					break;
	case 21: 	sprintf(buff+strlen(buff),"Slovakia, ");				break;
	case 22: 	sprintf(buff+strlen(buff),"Spain, ");					break;
	case 23: 	sprintf(buff+strlen(buff),"Sweden, ");					break;
	case 24: 	sprintf(buff+strlen(buff),"Switzerland (German), ");	break;
	case 25: 	sprintf(buff+strlen(buff),"Turkey F, ");				break;
	case 26: 	sprintf(buff+strlen(buff),"Turkey Q, ");				break;
	case 27: 	sprintf(buff+strlen(buff),"UK, ");						break;
	case 28: 	sprintf(buff+strlen(buff),"Japan, ");					break;
	}

	if (X_paramGetGlobalAllParam()->pKBW->isUnknownCharactersBeep)
	{
		sprintf(buff+strlen(buff), "Beep on Unknown Character On, ");
	}
// 	else
// 		sprintf(buff+strlen(buff), "Beep on Unknown Character Off, ");

	if (X_paramGetGlobalAllParam()->pKBW->isEmulateKeypad)
	{
		sprintf(buff+strlen(buff), "Emulate ALT+Keypad On, ");
		switch(X_paramGetGlobalAllParam()->pKBW->nCodePage)
		{
		case 0:
			sprintf(buff+strlen(buff), "Code page: Windows 1252, ");
			break;
		case 1:
			sprintf(buff+strlen(buff), "Code page: Windows 1251, ");
			break;
		case 2:
			sprintf(buff+strlen(buff), "Code page: Windows 1250, ");
			break;
		case 3:
			sprintf(buff+strlen(buff), "Code page: Windows 1253, ");
			break;
		case 4:
			sprintf(buff+strlen(buff), "Code page: Windows 1254, ");
			break;
		case 5:
			sprintf(buff+strlen(buff), "Code page: Windows 1255, ");
			break;
		case 6:
			sprintf(buff+strlen(buff), "Code page: Windows 1256, ");
			break;
		case 7:
			sprintf(buff+strlen(buff), "Code page: Windows 1257, ");
			break;
		case 8:
			sprintf(buff+strlen(buff), "Code page: Windows 1258, ");
			break;
		case 9:
			sprintf(buff+strlen(buff), "Code page: CP437, ");
			break;
// 		case 10:
// 			sprintf(buff+strlen(buff), "Code page: CP932, ");
// 			break;
		}
	}
// 	else
// 		sprintf(buff+strlen(buff), "Emulate ALT+Keypad Off, ");
		
	if (X_paramGetGlobalAllParam()->pKBW->isCtrlASCIIMode)
	{
		sprintf(buff+strlen(buff), "Function Key Mapping On, ");
	}
// 	else
// 		sprintf(buff+strlen(buff), "Function Key Mapping Off, ");


	switch(X_paramGetGlobalAllParam()->pKBW->isKeysrockeDelay) {
	case 0:
// 		sprintf(buff+strlen(buff),"No Inter-Keystroke Delay, ");
		break;
	case 1:
		sprintf(buff+strlen(buff),"Short Inter-Keystroke Delay, ");
		break;
	case 2:
		sprintf(buff+strlen(buff),"Long Inter-Keystroke Delay, ");
		break;
	}

	if (X_paramGetGlobalAllParam()->pKBW->isCapsLock)
	{
		sprintf(buff+strlen(buff), "Caps Lock On, ");
	}
// 	else
// 		sprintf(buff+strlen(buff), "Caps Lock Off, ");

	switch(X_paramGetGlobalAllParam()->pKBW->isConvertCase) {
	case 0:
// 		sprintf(buff+strlen(buff),"No Case Conversion, ");
		break;
	case 1:
		sprintf(buff+strlen(buff),"Convert All to Upper Case, ");
		break;
	case 2:
		sprintf(buff+strlen(buff),"Convert All to Lower Case, ");
		break;
	}

	if (X_paramGetGlobalAllParam()->pKBW->isNumericMode)
	{
		sprintf(buff+strlen(buff), "Emulate Numeric Keypad On, ");
	}
// 	else
// 		sprintf(buff+strlen(buff), "Emulate Numeric Keypad Off, ");


	if (X_paramGetGlobalAllParam()->pKBW->isFasttransferMode)
		sprintf(buff+strlen(buff),"Fast Mode On, ");
// 	else
// 		sprintf(buff+strlen(buff),"Fast Mode Off, ");

	sprintf(buff+strlen(buff),"Poll Rate %d ms\r\n", X_paramGetGlobalAllParam()->pKBW->nPollRate);

#ifdef BUILT_HR32
	if (X_paramGetGlobalAllParam()->pCode2->bPowerLedOn)
		sprintf(buff+strlen(buff),"Power LED On\r\n");
	else
		sprintf(buff+strlen(buff),"Power LED Off\r\n");
#endif

	//dataformat
		
	sprintf(buff+strlen(buff),"\r\nData Formatting:\r\n");

	if (X_paramGetGlobalAllParam()->pDFormat->DFEnable)
	{
			sprintf(buff+strlen(buff),"\tData Formatter On\r\n");
// 			sprintf(buff+strlen(buff),"\tData Format No.%d\r\n",X_paramGetGlobalAllParam()->pDFormat->usedID);
// 			if (X_paramGetGlobalAllParam()->pDFormat->format[usedID].TerID > 0)
// 			{
// 				sprintf(buff+strlen(buff),"\t\tTerminal Type:all terminal types\r\n\t\tSymbology I.D.:%d\r\n\t\tMessage Length:%d\r\n\t\tCommand:%s\r\n",
// 					X_paramGetGlobalAllParam()->pDFormat->format[usedID].SymbolID,
// 					X_paramGetGlobalAllParam()->pDFormat->format[usedID].MsgLen,
// 					X_paramGetGlobalAllParam()->pDFormat->format[usedID].Cmd
// 					);
// 			}
			usedNo = X_paramGetGlobalAllParam()->pDFormat->usedID;
			if (X_paramGetGlobalAllParam()->pDFormat->format[usedNo].TerID > 0)
			{
				sprintf(buff + strlen(buff),"\tData Format%d: %d",usedNo,X_paramGetGlobalAllParam()->pDFormat->format[usedNo].FormatID);
				sprintf(buff + strlen(buff),"%d",X_paramGetGlobalAllParam()->pDFormat->format[usedNo].FormatType);
				sprintf(buff + strlen(buff),"%d",X_paramGetGlobalAllParam()->pDFormat->format[usedNo].TerID);
				if (X_paramGetGlobalAllParam()->pDFormat->format[usedNo].FormatType == 3)
				{
					sprintf(buff + strlen(buff),"%02d",X_paramGetGlobalAllParam()->pDFormat->format[usedNo].SymbolID);
				} 
				else
				{
					sprintf(buff + strlen(buff),"%03d",X_paramGetGlobalAllParam()->pDFormat->format[usedNo].SymbolID);
				}
				
				sprintf(buff + strlen(buff),"%04d",X_paramGetGlobalAllParam()->pDFormat->format[usedNo].MsgLen);
				sprintf(buff + strlen(buff),"%s",X_paramGetGlobalAllParam()->pDFormat->format[usedNo].Cmd);
				sprintf(buff + strlen(buff),";\r\n");	
			}
			else if (X_paramGetGlobalAllParam()->pDFormat->format[usedNo].TerID == 0)
			{
				sprintf(buff + strlen(buff),"\tData Format%d: NULL\r\n",usedNo);
			}
			switch (X_paramGetGlobalAllParam()->pDFormat->outFlag)
			{
			case 0:
				sprintf(buff+strlen(buff),"\tData Formatter : Data Format Required , Keep prefix/suffix\r\n");
				break;	
			case 1:
				sprintf(buff+strlen(buff),"\tData Formatter :Data Format Required , Drop prefix/suffix\r\n");
				break;	
			case 2:
				sprintf(buff+strlen(buff),"\tData Formatter :Data Formatter On, Not Required, Keep prefix/suffix\r\n");
				break;	
			case 3:
				sprintf(buff+strlen(buff),"\tData Formatter :Data Formatter On, Not Required, Drop prefix/suffix\r\n");
				break;	
			}
			if (X_paramGetGlobalAllParam()->pDFormat->ErrorTone)
			{
				sprintf(buff+strlen(buff),"\tData Format Non-Match Error Tone Off\r\n");
			} 
			else
			{
				sprintf(buff+strlen(buff),"\tData Format Non-Match Error Tone On\r\n");
			}

	} 
	else
	{
			sprintf(buff+strlen(buff),"\tData Formatter Off \r\n");
	}

//sprintf(buff+strlen(buff),"SC_MainTagMapTab[1].nMainTagId=%x\r\n",SC_MainTagMapTab[1].nMainTagId);
	n = Y_commGetCurType();
	if (n==COMMT_IBM_SurePOS_TT || n == COMMT_IBM_SurePOS_HH)
		Send_DirectIO_Return_Data((BYTE*)buff, strlen(buff));
	else
		Y_commWrite((unsigned char*)buff,strlen(buff));



}//End of function;

//////////////////////////////////////////////////////////////////////////
// 参数纠正

BOOL uJ_ComParam(void)
{
	BOOL bResult = TRUE;
	
	X_paramGetGlobalAllParam()->pCom->nPort = 0;

	switch(X_paramGetGlobalAllParam()->pCom->lRate) {
	case 1200:	case 2400:	case 4800:	case 9600:
	case 19200:	case 38400:	case 57600:	case 115200:
		break;
	case 14400:	
		break;
	default:
		X_paramGetGlobalAllParam()->pCom->lRate = 9600;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pCom->lDataBits) {
	//case 5: case 6:	
	case 7:	case 8:
		break;
	default:
		X_paramGetGlobalAllParam()->pCom->lDataBits = 8;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pCom->lStopBits) {
	case 1:	case 2:
		break;
	default:
		X_paramGetGlobalAllParam()->pCom->lStopBits = 1;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pCom->lParity)
	{
	case NOPARITY:	case EVENPARITY:	case ODDPARITY:
		break;
	default:
		X_paramGetGlobalAllParam()->pCom->lParity = NOPARITY;
		bResult = FALSE;
		break;
	}

	return bResult;
}

BOOL uJ_LightFocusParam(void)
{
	BOOL bResult = TRUE;
	int mode = 0;

	switch(X_paramGetGlobalAllParam()->pHard->nLight){
	case 0: case 1: case 2:	case 3:
		break;
	default:
		X_paramGetGlobalAllParam()->pHard->nLight = 0;
		bResult = FALSE;
		break;
	}
	switch(X_paramGetGlobalAllParam()->pHard->nFocus){
	case 0:	case 1:	case 2:	case 3:
		break;
	default:
		X_paramGetGlobalAllParam()->pHard->nFocus = 0;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pHard->nExLight){
	case 0:	case 1:	
		break;
	default:
		X_paramGetGlobalAllParam()->pHard->nExLight = 0;
		bResult = FALSE;
		break;
	}

	mode |= (X_paramGetGlobalAllParam()->pHard->nLight <= 2)? X_paramGetGlobalAllParam()->pHard->nLight: 0;

	mode |= X_paramGetGlobalAllParam()->pHard->nFocus << 8;

	mode |= X_paramGetGlobalAllParam()->pHard->nExLight <<16;

	Z_SetCmosLightMode(mode);

	return bResult;
}

BOOL uJ_InfoEditParam(void)
{
	BOOL bResult = TRUE;
	//自定义前缀设置
	switch(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix]) {
	case 0:  case 1:
		break; 
	default: 
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] = 0;
		bResult = FALSE;
		break;
	}

	//自定义后缀设置
	switch(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix]) {
	case 0:  case 1:
		break;
	default: 
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] = 0;
		bResult = FALSE;
		break;
	}
	//CodeID前缀设置
	switch(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID]) {
	case 0: case 1:
		break;
	default: 
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] = 0;
		bResult = FALSE;
		break;
	}

	//AIM前缀设置
	switch(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM]) {
	case 0: case 1: case 2:
	case 3:	case 4: case 5:
		break;
	default:
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] = 0;
		bResult = FALSE;
		break;
	}
	
	//结束符后缀设置
	switch(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal]) {
	case 0:	case 1:
		break;
	default:
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] = 0;
		bResult = FALSE;
		break;
	}
	
	//截取使能设置
	switch(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCut]) {
	case 0: case 1:
		break;
	default:
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCut] = 0;
		bResult = FALSE;
		break;
	}
	//除标准外的其它用户自定义的前后缀使能
	switch(X_paramGetGlobalAllParam()->pCode->IsOtherPreSufOn){
	case 0: case 1: case 2: case 3:
		break;
	default:
		X_paramGetGlobalAllParam()->pCode->IsOtherPreSufOn = 0;
		bResult = FALSE;
		break;
	}

	//CodeID、AIM和自定义前缀的顺序
	switch(X_paramGetGlobalAllParam()->pCode->PrefixOrder){
	case 0: case 1: case 2:
	case 3: case 4: case 5:
		break;
	default:
		X_paramGetGlobalAllParam()->pCode->PrefixOrder = 0;
		bResult = FALSE;
		break;
	}

	if (X_paramGetGlobalAllParam()->pCode->nPrefix > 10){
		bResult = FALSE;
		X_paramGetGlobalAllParam()->pCode->nPrefix = 0;
	}
	if (X_paramGetGlobalAllParam()->pCode->nSuffix > 10){
		bResult = FALSE;
		X_paramGetGlobalAllParam()->pCode->nSuffix = 0;
	}
	if (X_paramGetGlobalAllParam()->pCode->nTerminal > 3){
		bResult = FALSE;
		X_paramGetGlobalAllParam()->pCode->nTerminal = 0;
	}
	return bResult;
}

BOOL uJ_UserTypeParam(void)
{
	BOOL bResult = TRUE;
	switch(X_paramGetGlobalAllParam()->pCode->nUserDef){
	case 0: case 1: case 2: case 3:
	case 4: case 5:
		break;
	default:
		X_paramGetGlobalAllParam()->pCode->nUserDef = 0;
		bResult = FALSE;
		break;
	}
	return bResult;
}


BOOL uJ_HardTypeParam(void)
{
	int nType;
	BOOL bResult = TRUE;
	char cValue[64];
	int i;

#ifdef MODULE_SDK_ROWMIRROR
//镜像
	X_paramGetGlobalAllParam()->pHard->nProduction = PRODUCTION_MIRROR;
#endif //MODULE_SDK_ROWMIRROR
	nType = X_paramGetGlobalAllParam()->pHard->nProduction;

	memset(cValue,0,sizeof(cValue));
	if (X_paramQueryKeyword((char*)"Mirr",cValue))
	{
		for (i = strlen(cValue)-1; i >= 0; i--)
		{//小写化
			if (cValue[i] >= 'A' && cValue[i] <= 'Z')
			{ cValue[i] = 'a'+(cValue[i]-'A'); }
		}//End of for;
		if ((strncmp("on",cValue,max(2,strlen(cValue))) == 0))
		{//固定为镜像形式
			nType = PRODUCTION_MIRROR;
			X_paramGetGlobalAllParam()->pHard->nProduction = PRODUCTION_MIRROR;
		}//End of if;
	}//End of if;

	switch(nType){
	case 0:
		Y_cmosMirror(0);
		break;
	case 1: case 2:
		break;
	case PRODUCTION_MIRROR:
		Y_cmosMirror(1);
		break;
	default:
		X_paramGetGlobalAllParam()->pHard->nProduction = 0;
		bResult = FALSE;
		break;
	}

	nType = X_paramGetGlobalAllParam()->pHard->nDofType;
	if (X_paramQueryKeyword((char*)"DofType",cValue))
	{
		if ((strncmp("HD",cValue,max(2,strlen(cValue))) == 0))
			i = PRODUCTION_DOF_NEAR;
		else if ((strncmp("SR",cValue,max(2,strlen(cValue))) == 0))
			i = PRODUCTION_DOF_MIDDLE;
		else if ((strncmp("ER",cValue,max(2,strlen(cValue))) == 0))
			i = PRODUCTION_DOF_FAR;
		else if (strlen(cValue) == 0)
			i = PRODUCTION_DOF_NORMAL;
		else 
			i = nType;

		if (i != nType)
		{
			X_paramGetGlobalAllParam()->pHard->nDofType = i;
			nType = i;
			
		}

	}

	switch(nType){
	case 0:	case 1:	case 2:	case 3:	
		break;
	default:
		X_paramGetGlobalAllParam()->pHard->nDofType = 0;
		bResult = FALSE;
		break;
	}

	return bResult;
}



 //数据打包类型
BOOL uJ_DataPackParam(void)
{
	BOOL bResult = TRUE;
	switch(X_paramGetGlobalAllParam()->pCode->nPack){
	case 0: case FACTORY_PACK:case ECS_BANK_PACK: case BLUE2D_PACK:
	case ECS_PACK_ADDLEN:	
		break;
	default:
		X_paramGetGlobalAllParam()->pCode->nPack = 0;
		bResult = FALSE;
		break;
	}
	return bResult;
}

BOOL uJ_ReadModeParam(void)
{
	BOOL bResult = TRUE;
	//开发模式和用户模式设置
	switch(X_paramGetGlobalAllParam()->pCode->Protect) {
	case 0: case 1:
		break;
	default:
		X_paramGetGlobalAllParam()->pCode->Protect = FALSE;
		bResult = FALSE;
		break;
	}
	
	switch(X_paramGetGlobalAllParam()->pCode->bIR)
	{
	case 0: case 1:
		break;
	default: 
		X_paramGetGlobalAllParam()->pCode->bIR = 0;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pCode->SetCode)
	{
	case 0: case 1:
		break;
	default: 
		X_paramGetGlobalAllParam()->pCode->SetCode = 0;
		bResult = FALSE;
		break;
	}

	//灵敏度设置
	if (X_paramGetGlobalAllParam()->pCode->nSensitive > 20){
		X_paramGetGlobalAllParam()->pCode->nSensitive = 20;
		bResult = FALSE;
	}
	//读码模式设置
	switch(X_paramGetGlobalAllParam()->pCode->nReadMode){
	case ESC0: 
		X_paramGetGlobalAllParam()->pCode->nReadMode = ESC0;
		break;
	case ESC1: case ESC2: case ESC3:
		break;
	default:
		X_paramGetGlobalAllParam()->pCode->nReadMode = ESC0;
		bResult = FALSE;
		break;
	}	
	
	//完全延迟设置
	switch(X_paramGetGlobalAllParam()->pCode->IsCompDelay)
	{
	case 0: case 1:
		break;
	default: 
		X_paramGetGlobalAllParam()->pCode->IsCompDelay = 0;
		bResult = FALSE;
		break;
	}
	//错误信息
	switch(X_paramGetGlobalAllParam()->pCode->bErrMsgEn)
	{
	case 0: case 1:
		break;
	default: 
		X_paramGetGlobalAllParam()->pCode->bErrMsgEn = 0;
		bResult = FALSE;
		break;
	}
	if (X_paramGetGlobalAllParam()->pCode->nErrMsg > 10){
		X_paramGetGlobalAllParam()->pCode->nErrMsg = 0;
		bResult = FALSE;
	}

	switch(X_paramGetGlobalAllParam()->pHard->bDecVoice)
	{
	case 0: case 1: case 2: case 3:
		break;
	default: 
		X_paramGetGlobalAllParam()->pHard->bDecVoice = 3;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pHard->bOscillation)
	{
	case 0: case 1:
		break;
	default: 
		X_paramGetGlobalAllParam()->pHard->bOscillation = FALSE;
		bResult = FALSE;
		break;
	}
	if (X_paramGetGlobalAllParam()->pHard->nOscillationDelay > 3000){
		X_paramGetGlobalAllParam()->pHard->nOscillationDelay = 3000;
		bResult = FALSE;
	}

	if(X_paramGetGlobalAllParam()->pCode->decodeArea & 0xf)
	{
		int nratio;
		X_decoSetCenterDecMode(1);
		nratio = X_paramGetGlobalAllParam()->pCode->decodeArea>>4;
		if (nratio == 0)
			nratio = 20;
		else if (nratio > 100)
			nratio = 100;
		X_decoInitCenterArea(1, nratio);
	}
	else
	{
		X_decoSetCenterDecMode(0);
	}

	switch(X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode)
	{
	case 0: case 2: 
//	case 3: case 4:
		break;
	default: 
		X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = 0;
		bResult = FALSE;
		break;
	}

	Z_SetLightMode(X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode);

	return bResult;
}
//开机进入控制台设置
//电源开启时是否显示系统信息设置
BOOL uJ_SysCtrlInfoParam(void)
{
	BOOL bResult = FALSE;
	
	switch(X_paramGetGlobalAllParam()->pCode->IsPowerOnDisp){
	case 0: case 1:
		break;
	default:
		X_paramGetGlobalAllParam()->pCode->IsPowerOnDisp = FALSE;
		bResult = FALSE;
		break;
	}
	
	switch(X_paramGetGlobalAllParam()->pCode->IsGotoConsole){
	case 0: case 1:
		break;
	default:
		X_paramGetGlobalAllParam()->pCode->IsGotoConsole = FALSE;
		bResult = FALSE;
		break;
	}
	return bResult;

}
BOOL uJ_KBWParam(void)
{
	BOOL bResult = TRUE;

	if (X_paramGetGlobalAllParam()->pKBW->nLanguage < 0)
	{
		X_paramGetGlobalAllParam()->pKBW->nLanguage = 0;
		bResult = FALSE;
	}//End of if;

	switch(X_paramGetGlobalAllParam()->pKBW->isCapsLock){
	case 0: case 1:
		break;
	default:
		X_paramGetGlobalAllParam()->pKBW->isCapsLock = FALSE;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pKBW->isCtrlASCIIMode){
	case 0: case 1:
		break;
	default: 
		X_paramGetGlobalAllParam()->pKBW->isCtrlASCIIMode = FALSE;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pKBW->isEmulateKeypad){
	case 0: case 1:
		break;
	default:
		X_paramGetGlobalAllParam()->pKBW->isEmulateKeypad = FALSE;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pKBW->isNumericMode){
	case 0: case 1:
		break;
	default:
		X_paramGetGlobalAllParam()->pKBW->isNumericMode = FALSE;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pKBW->isUnknownCharactersBeep){
	case 0: case 1:
		break;
	default:
		X_paramGetGlobalAllParam()->pKBW->isUnknownCharactersBeep = FALSE;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pKBW->isShiftLock){
	case 0: case 1:
		break;
	default:
		X_paramGetGlobalAllParam()->pKBW->isShiftLock = FALSE;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pKBW->isConvertCase){
	case 0: case 1: case 2:
		break;
	default:
		X_paramGetGlobalAllParam()->pKBW->isConvertCase = FALSE;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pKBW->isKeysrockeDelay){
	case 0: case 1: case 2:
		break;
	default:
		X_paramGetGlobalAllParam()->pKBW->isKeysrockeDelay = 0;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pKBW->isKeyPad){
	case 0: case 1:
		break;
	default:
		X_paramGetGlobalAllParam()->pKBW->isKeyPad = 0;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pKBW->isFasttransferMode){
	case 0: case 1: 
		break;
	default:
		X_paramGetGlobalAllParam()->pKBW->isFasttransferMode = 0;
		bResult = FALSE;
		break;
	}

	switch(X_paramGetGlobalAllParam()->pKBW->nPollRate){
	case 0: 
		X_paramGetGlobalAllParam()->pKBW->nPollRate = 4;
		break;
	case 1: case 2: case 3: case 4: case 5: case 6: case 7:
		case 8: case 9: case 10:
		break;
	default:
		X_paramGetGlobalAllParam()->pKBW->nPollRate = 4;
		bResult = FALSE;
		break;
	}

	if (X_paramGetGlobalAllParam()->pKBW->nCodePage < 0)
	{
		X_paramGetGlobalAllParam()->pKBW->nCodePage = 0;
		bResult = FALSE;
	}//End of if;
	
	return bResult;
}
//接口参数
BOOL uJ_InterFaceParam(void)
{
	uJ_KBWParam();

	X_paramCheckCommType();
	
	return TRUE;
}

int uMinMaxGetResult_High(int Value,int Final)
{
	int nMin,nMax;
	if (Value == 0)
		return Final;
	nMax = Value&0x7f000000;
	nMax >>=24;
	nMin = Value&0x00ff0000;
	nMin >>= 16;
	if (nMax < nMin) return Final;
	return Value;
}

BOOL uJ_1DParam(void)
{
//	if (X_paramGetGlobalAllParam()->p1D->unDecTypes == 0){
//		X_paramGetGlobalAllParam()->p1D->unDecTypes = Init_Enable1D;
//	}
//	if (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] == 0)
//		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] = Init_EAN8;
//	if (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] == 0)
//		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] = Init_EAN13;
//	if (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] == 0)
//		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] = Init_UPCE;
//	if (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] == 0)
//		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] = Init_UPCA;

//	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = 
//		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128],Init_CODE128);
//	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = 
//		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5],Init_INTER25);
//	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] = 
//		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128],Init_CODE128);
//	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] = 
//		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25],Init_MATRIX25);
//	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] = 
//		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25],Init_MODULE25);
//	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] = 
//		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39],Init_REGULAR39);
//	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_FULLASCII39] = 
//		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->p1D->ExMsg[POS_FULLASCII39],Init_FULLASCII39);
//	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] = 
//		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR],Init_CODABAR);
//	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_2OUTOF5] = 
//		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->p1D->ExMsg[POS_2OUTOF5],Init_INTER25);
//	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] = 
//		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93],Init_CODE93);
//	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] = 
//		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ],Init_CODEZ);
	return TRUE;
}
BOOL uJ_OCRParam(void)
{
	X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = 
		uMinMaxGetResult_High(X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B],Init_SPEC_OCR_B);
	return TRUE;
}

int uMinMaxGetResult(int Value,int Final)
{
	int nMin,nMax;
	if (Value == 0)
		return Final;
	nMin = Value&0x0000ffff;
	nMax = Value&0x7fff0000;
	nMax >>= 16;
	if (nMax < nMin) return Final;
	return Value;
}

BOOL uJ_2DParam(void)
{
//	if (X_paramGetGlobalAllParam()->p2D->unDecTypes > (unsigned int)(DC_USER_2D_CODE << 1))
//	{
//		X_paramGetGlobalAllParam()->p2D->unDecTypes = Init_Enable2D;
//	}
	
//	X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] = 
//		uMinMaxGetResult(X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417],Init_2D_PDF417_MIN_MAX_LEN);
//	X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] =
//		uMinMaxGetResult(X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR],Init_2D_QRCODE_MIN_MAX_LEN);
//	X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] = 
//		uMinMaxGetResult(X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix],Init_2D_DataMatrix_MIN_MAX_LEN);
//	X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] =
//		uMinMaxGetResult(X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode],Init_2D_VeriCode_MIN_MAX_LEN);
//	X_paramGetGlobalAllParam()->p2D->ExMsg[POS_UN_VeriCode] = 
//		uMinMaxGetResult(X_paramGetGlobalAllParam()->p2D->ExMsg[POS_UN_VeriCode],Init_2D_UnVeriCode_MIN_MAX_LEN);
//	X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] = 
//		uMinMaxGetResult(X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec],Init_2D_AZTEC_MIN_MAX_LEN);
//		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] = 
//		uMinMaxGetResult(X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE],Init_2D_HANSENSIBLE_MIN_MAX_LEN);

// 	if (X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] > 0x0FFFF) X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] = 0;
#if defined(NL_IT_IC) 
	X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= DC_DM_MOBILE;
#endif

#if defined(USA_LOTTERY)
	X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= DC_DM_LOTTERY;
#endif 

	if (X_paramGetGlobalAllParam()->p2D->nMacro < 1 || X_paramGetGlobalAllParam()->p2D->nMacro > 3){
		X_paramGetGlobalAllParam()->p2D->nMacro = MACRO_DIRECT;
	}
	W_macroSetParam(X_paramGetGlobalAllParam()->p2D->nMacro);
	if (X_paramGetGlobalAllParam()->p2D->nQRParam >= 255*255 || X_paramGetGlobalAllParam()->p2D->nQRParam < 0){
		X_paramGetGlobalAllParam()->p2D->nQRParam = 0;
	}
	return TRUE;
}

void X_paramUpdateToDevice(void)
{
	//参数校正
	uJ_1DParam();
	uJ_2DParam();
	uJ_OCRParam();
	uJ_InterFaceParam();
	uJ_SysCtrlInfoParam();
	uJ_ReadModeParam();
	uJ_DataPackParam();
	uJ_HardTypeParam();
	uJ_UserTypeParam();
	uJ_InfoEditParam();
	uJ_LightFocusParam();
	
	X_decoSet2DParam(X_paramGetGlobalAllParam()->p2D);
	X_decoSet1DParam(X_paramGetGlobalAllParam()->p1D);
	X_decoSetOCRParam(X_paramGetGlobalAllParam()->pOCR);
	X_decoSetPostalParam(X_paramGetGlobalAllParam()->pPostal);

      X_decoIniDecArea(X_paramGetGlobalAllParam()->pAreaParam);

	//!参数校正
}//End of function;


//判断哪种通讯口类型可用
int  X_paramSelectUsefulCommType(void)
{
	if (Y_commChgToUSBRS232() == TRUE)
	{ return U_PRO_IS_USBRS232; }
	else
	{
		if (Y_commChgToRS232(X_paramGetGlobalAllParam()->pCom) == TRUE)
		{ return U_PRO_IS_Serial; }
	}

	return U_PRO_IS_Serial;
}//End of function;

void X_paramCheckCommType(void)
{
	int lvflag =0;

	static int _s_port = U_PRO_IS_USBRS232;

	int lvPortType = X_paramGetGlobalAllParam()->pHard->InterType;
if(g_commChange!=0)
    return;
	switch(lvPortType){
	case U_PRO_IS_Serial:
		if (Y_commIsBTOpened())
		{ Y_commCloseBT(); }
		if (!Y_commChgToUSBRS232())
		{ Y_commChgToRS232(X_paramGetGlobalAllParam()->pCom); }
		break;
	case U_PRO_IS_HID_KBW:
		if (Y_commIsBTOpened())
		{ Y_commCloseBT(); }
		if (!Y_commChgToKBW(X_paramGetGlobalAllParam()->pKBW))
		{ Y_commChgToRS232(X_paramGetGlobalAllParam()->pCom); }
		break;
	case U_PRO_IS_DataPipe:
		if (Y_commIsBTOpened())
		{ Y_commCloseBT(); }
		if (!Y_commChgToUSBDataPipe())
		{ 
			Y_commChgToRS232(X_paramGetGlobalAllParam()->pCom);
		}
		lvflag = 1;
	
		break;
	case U_PRO_IS_USBRS232:
		if (Y_commIsBTOpened())
		{ Y_commCloseBT(); }
		if (!Y_commChgToUSBRS232())
		{ Y_commChgToRS232(X_paramGetGlobalAllParam()->pCom); }
		lvflag = 1;
		break;
	case U_PRO_IS_BLUE_TOOTH:
		if (!Y_commChgToBT())
		{
			if (!Y_commChgToUSBRS232())
			{ Y_commChgToRS232(X_paramGetGlobalAllParam()->pCom); }
		}
		break;
	case U_PRO_IS_PS2:
		if (!Y_commChgToUSBRS232())
			Y_commChgToPS2(X_paramGetGlobalAllParam()->pKBW);
		break;
	case U_PRO_IS_HID_POS:
		if (Y_commIsBTOpened())
		{ Y_commCloseBT(); }
		if(!Y_commChgToHIDPOS())
		{
			Y_commChgToRS232(X_paramGetGlobalAllParam()->pCom);
		}
		break;

	case U_PRO_IS_SurePOS_TT:
		if (Y_commIsBTOpened())
		{ Y_commCloseBT(); }
		if(!Y_commChgToSurePOS_TT())
		{
			Y_commChgToRS232(X_paramGetGlobalAllParam()->pCom);
		}
		break;

	case U_PRO_IS_SurePOS_HH:
		if (Y_commIsBTOpened())
		{ Y_commCloseBT(); }
		if(!Y_commChgToSurePOS_HH())
		{
			Y_commChgToRS232(X_paramGetGlobalAllParam()->pCom);
		}
		break;

	case U_PRO_IS_CDC:
		if (Y_commIsBTOpened())
		{ Y_commCloseBT(); }
		if (!Y_commChgToUSBCDC())
		{ Y_commChgToRS232(X_paramGetGlobalAllParam()->pCom); }
// 		lvflag = 1;
		break;
		

	default:
		break;
	}//End of switch;
	Y_commEnable();

	if(lvPortType != _s_port)
	{
		_s_port = lvPortType;
		
		if(lvflag)
		{
			X_paramGetGlobalAllParam()->pDevInfo->IsDownload = '1';
			X_paramPlaceKeyword((char*)"IsDownload",(char*)"1",TRUE);
		}
		else
		{
			X_paramGetGlobalAllParam()->pDevInfo->IsDownload = '0';
			X_paramPlaceKeyword((char*)"IsDownload",(char*)"0",TRUE);
		}
	}
}//End of function;

void X_paramSetUserFunc(UTGeneralParam *psParam)
{
	if (psParam == NULL || psParam->Id != STID_PARAMEXTFUNC || psParam->psParamStruct == NULL)
	{ return; }

	X_paramOriginalUserDefault = ((TXPARAMExtFunc*)psParam->psParamStruct)->fnc_OriginalUserDefault;
	X_param2DevLoadUserDefault = ((TXPARAMExtFunc*)psParam->psParamStruct)->fnc_2DevLoadUserDefault;
	X_param2DevSaveUserDefault = ((TXPARAMExtFunc*)psParam->psParamStruct)->fnc_2DevSaveUserDefault;

}//End of function;

