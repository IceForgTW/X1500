#include <MemMgr.h>
#include <generate.h>

#include "SC_Function.h"
#include "SC_ExtFunc.h"
#include "../MB_Decode_Ctrl/list.h"

#include <string.h>
#include <stdio.h>
#define _ENABLE_EXTFUNC_DBG_ 0
#define KBWlLANGNUM 28
#define GRBEEPDURMIN 20
#define GRBEEPDURMAX 300
#define GRBEEPFREQMIN 20
#define GRBEEPFREQMAX 20000
#define GRLEDDURMIN 1
#define GRLEDDURMAX 2500
#define ONEREADTIMEOUTMIN 0
#define ONEREADTIMEOUTMAX 3600000
#define REREADDELAYTIMEMIX 1
#define REREADDELAYTIMEMAX 3600000
//#undef printf

//图形设置临时参数
//static int gImgGetMode = 0;	//图像获取方式: 0:直接获取缓存图像数据	
static int gImgType = 0;		//图像样式: 0:原始图像
static int gImgColor = 0;		//图像颜色: 0:灰度图
static int gImgReduceRow = 0;	//图像水平压缩: 0: "1:1"
static int gImgReduceColumn = 0;	//图像垂直压缩: 0: "1:1"
//static int gImgCutStartRow = 0;		//裁剪图像起始行
//static int gImgCutStartColumn = 0;		//裁剪图像起始列
//static int gImgCutEndRow = 0;		//裁剪图像结束行
//static int gImgCutEndColumn = 0;		//裁剪图像结束列
#define malloc(x) alloc(x)
#define free(x) deAlloc(x)


#ifndef NLSCOMM_OUTBUF_SIZE
#define NLSCOMM_OUTBUF_SIZE    4096
#endif

extern BYTE FacParamBuf[NLSCOMM_OUTBUF_SIZE];
extern BOOL g_FacParamGet;
extern int g_ReplaceType;
extern unsigned int g_commChange;
#define Replace_NewInstruction    		9
#ifndef PARAMLEN
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

//#define   Total_MEMSize     (Start_MEMSize_DATAFORMAT + MEMSize_DATAFORMAT)
#define   MEMSize_AREAPARAM		(sizeof(UIMGAreaParam) + 10)
#define   Start_MEMSize_AREAPARAM (Start_MEMSize_DATAFORMAT + MEMSize_DATAFORMAT)	

#define   Total_MEMSize     (Start_MEMSize_AREAPARAM + MEMSize_AREAPARAM)

#define FLASH_BLOCK_SIZE			4096// SST block
#define FLASH_BLOCK_VALID_SIZE		Total_MEMSize	//1024
// OEM Firmware Param    //提供给OEM厂商的参数存储位置，起始内存块，存储长度
#define OEM_BLOCK_VALID_SIZE	    1024
//校验码在pCode2中的偏移
#define	  CRC_OFFSET_V1		 72 //4 + 32 + 32 + 4

#define  PARAMVERSION	0x0001

#endif

#define DECODESUCCVOICE 1
#define POWNONVOICE 2

#define DECODESUCCVOL 0xF0
#define DECODESUCCLAST 0xF00
#define DECODESUCCFREQ 0xF000

//////////////////////////////////////////////
#define Max_nPreSubfix          	10				//前后缀最大信息长度
#define Max_FixedLen            	2				//读码固定长度的参数个数
#define Max_DataCutLen             	20				//信息截取命令最大长度
#define Max_Delay					3600000			//最大时间延迟
#define Max_Sensibility             20              //感光灵敏度最大值
#define Max_nTerminal				2				//结束符的最大长度值

////????????????????????
extern FNC_paramOriginalUserDefault	X_paramOriginalUserDefault;
extern FNC_param2DevLoadUserDefault	X_param2DevLoadUserDefault;
extern FNC_param2DevSaveUserDefault	X_param2DevSaveUserDefault ;
extern const unsigned char CodeIDTab1D[32];
const char chHEX[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

extern void IMG_Edit_Send(int mode, int start_x, int start_y, int end_x, int end_y, int nScale_x, int nScale_y, int nWidth, void *img);
extern void IMG_Edit(int mode, int start_x, int start_y, int end_x, int end_y, int nScale_x, int nScale_y, void *img);
extern void Img_Send(int nType, int nQuality, BOOL bInstr);
extern BOOL X_FacParamIsGet(void);

BOOL	IsDec(unsigned char *pucStr, unsigned int short nLen)
{
	unsigned int short i;
	for (i = 0; i < nLen; i++,pucStr++)
	{
		if (! ((*pucStr >= '0' && *pucStr <= '9')))
		{ return FALSE; }
	}
	return TRUE;
}


unsigned int		Dec2Int(unsigned char *pucStr, unsigned int short nLen)
{
	unsigned int nV;
	unsigned int short i;

	nV = 0;
	for (i = 0; i < nLen; i++,pucStr++)
	{
		if (! ((*pucStr >= '0' && *pucStr <= '9')))
		{ break; }
		nV = (nV*10)+((*pucStr)-'0');
		continue;
	}/*End of for*/

	return nV;
}

BOOL	IsHex(unsigned char *pucStr, unsigned int short nLen)
{
	unsigned int short i;
	
	for (i = 0; i < nLen; i++, pucStr++)
	{

		if (!((*pucStr >= '0' && *pucStr <= '9') ||
			  (*pucStr >= 'a' && *pucStr <= 'f') || 
			  (*pucStr >= 'A' && *pucStr <= 'F')
			 )
		   )
		{
			return FALSE;
		}
	}
	return TRUE;
}


BOOL String2Hex(unsigned char* pSrc, unsigned short nSrcLen, unsigned char* pBuff, unsigned short nMaxSize)
{
	unsigned short i,k;

	unsigned char pSrctemp;
      

	if (nMaxSize < nSrcLen*2)
	{ return FALSE; }

       for (i = 0; i < nSrcLen/2; i++)
        {
            pSrctemp = pSrc[i];
            pSrc[i] = pSrc[nSrcLen-i-1];
            pSrc[nSrcLen-i-1] = pSrctemp;
       }
	for (i = 0; i < nSrcLen; i++)
	{
		k = (unsigned short)(*pSrc);
		*pBuff = (unsigned char)chHEX[((k>>4)&0x0f)];
		pBuff ++;
		*pBuff = (unsigned char)chHEX[(k&0x0f)];
		pBuff ++;
		pSrc++;
	}

        
/*	for (i = nSrcLen; i > 0; i--)
	{
		k = (unsigned short)(*pSrc);
             pBuff = pBuff+(i*2-2);
		*pBuff = (unsigned char)chHEX[((k>>4)&0x0f)];
		pBuff ++;
		*pBuff = (unsigned char)chHEX[(k&0x0f)];
		//pBuff ++;
		pSrc++;
	}*/
	return TRUE;
}

unsigned int	Hex2Int(unsigned char *pucStr, unsigned int short nLen)
{
	unsigned int nV;
	unsigned int short i;

	nV = 0;
	for (i = 0; i < nLen; i++, pucStr++)
	{
		if (!((*pucStr >= '0' && *pucStr <= '9') ||
			(*pucStr >= 'a' && *pucStr <= 'f') ||
			(*pucStr >= 'A' && *pucStr <= 'F')
			)
			)
		{
			break;
		}

		if (*pucStr >= '0' && *pucStr <= '9')
		{
			nV = (nV << (4*i)) + ((*pucStr) - '0');
			continue;
		}

		if ((*pucStr >= 'a' && *pucStr <= 'f') ||
			(*pucStr >= 'A' && *pucStr <= 'F'))
		{
			switch (*pucStr)
			{
			case 'a':
			case 'A':
				nV = (nV << (4 * i)) + 0x0A;
				break;

			case 'b':
			case 'B':
				nV = (nV << (4 * i)) + 0x0B;
				break;

			case 'c':
			case 'C':
				nV = (nV << (4 * i)) + 0x0C;
				break;

			case 'd':
			case 'D':
				nV = (nV << (4 * i)) + 0x0D;
				break;
			case 'e':
			case 'E':
				nV = (nV << (4 * i)) + 0x0E;
				break;

			case 'f':
			case 'F':
				nV = (nV << (4 * i)) + 0x0F;
				break;		

			default:
				break;
			}
			continue;
		}
		
	}/*End of for*/

	return nV;
}

/****************************************************************************
*@产品默认设置
****************************************************************************/
//加载出厂默认设置	Tag:FACDEF
int LoadFactoryDefaultCFG_Set(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_Save;
    BYTE byBuff[FLASH_BLOCK_SIZE];

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam->nDataLen != 0)
    {
		return SC_RESULT_FAIL;
    }
     
    //检查
    if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
    { 
        return SC_RESULT_FAIL; 
    }
    
    //读参数
    if (Y_paramLoadFactory(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE) 
    { 
        return SC_RESULT_FAIL; 
    }
    
    //解析参数
    if (X_paramGetParamFromBuff(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE)
    { 
        return SC_RESULT_FAIL; 
    }
    
    X_paramUpdateToDevice();

    if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else
        X_SysParamCpy();
   
    return nResult;
}

int LoadFactoryDefaultCFG_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}

int LoadUserDefaultCFG_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}
//加载用户默认设置	Tag:@CUSDEF/#CUSDEF
int LoadUserDefaultCFG_Set(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_Save;
    BYTE byBuff[FLASH_BLOCK_SIZE];

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam->nDataLen != 0)
    {
		return SC_RESULT_FAIL;
    }

    //检查
    if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
    { 
        return SC_RESULT_FAIL; 
    }

    //读参数
    if (Y_paramLoadUsrDefault(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE) 
    { 
        return SC_RESULT_FAIL; 
    }

    //解析参数
    if (X_paramGetParamFromBuff(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE)
    { 
        return SC_RESULT_FAIL; 
    }

    //更新设备参数
    X_paramUpdateToDevice();

    //调用二次开发的"导入用户默认"
    if (X_param2DevLoadUserDefault != NULL)
    { 
        X_param2DevLoadUserDefault(); 
    }

     if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
     else
        X_SysParamCpy();
     

    //读取参数，并解析成功
    return nResult;

}

//保存用户默认设置	Tag:@CUSSAV/#CUSSAV
int SaveUserDefaultCFG_Set(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SaveAndUpdateAll;
    BYTE byBuff[FLASH_BLOCK_SIZE];
    int nSize;

    nSize = 0;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam->nDataLen != 0)
    {
		return SC_RESULT_FAIL;
    }
    
    //检查
    if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
    { 
        return SC_RESULT_FAIL; 
    }

    //更新设备参数
    X_paramUpdateToDevice();

    //将参数写到缓存中
    nSize = X_paramPushParamToBuff(byBuff,FLASH_BLOCK_SIZE);
    if (nSize <= 0)
    { 
        return SC_RESULT_FAIL; 
    }
	
    //存储参数
    if (Y_paramSaveUsrDefault(byBuff,nSize) != TRUE) 
    { 
        return SC_RESULT_FAIL; 
    }

    //二次开发的“存储用户默认”
    if (X_param2DevSaveUserDefault != NULL)
    { 
        X_param2DevSaveUserDefault(); 
    }

    //if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    //else
        //X_SysParamCpy();
     
    //存储成功
    return nResult;
}
//删除用户默认设置
int DeleteUserDefaultCFG_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//查询用户默认设置

int SaveUserDefaultCFG_Get(sc_param_st *pParam){ return SC_RESULT_SUCC; }
int DeleteUserDefaultCFG_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }


//保存出厂默认设置	Tag:SAVEFY
int SaveFactoryDefaultCFG_Set(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SUCC;
    BYTE byBuff[FLASH_BLOCK_SIZE];
    int nSize;

    nSize = 0;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam->nDataLen != 0)
    {
		return SC_RESULT_FAIL;
    }
    //检查
    if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
    { 
        return SC_RESULT_FAIL; 
    }
	
    //更新设备参数
    X_paramUpdateToDevice();
	
    //将参数写到缓存中
    nSize = X_paramPushParamToBuff(byBuff,FLASH_BLOCK_SIZE);
    if (nSize <= 0)
    { 
        return SC_RESULT_FAIL; 
    }
	
    //存储参数
    if (Y_paramSaveFactory(byBuff,nSize) != TRUE) 
    { 
        return SC_RESULT_FAIL; 
    }

    if(X_paramGetFactoryParam(FacParamBuf, FLASH_BLOCK_SIZE) == TRUE)//重新初始化出厂默认参数数组
            g_FacParamGet = TRUE;
    else
        g_FacParamGet = FALSE;
     
    //if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    //else
        //X_SysParamCpy();
    //存储成功
    return nResult;
}
int SaveFactoryDefaultCFG_Get(sc_param_st *pParam){ return SC_RESULT_SUCC; }

//加载通用默认设置	Tag:GENDEF
int LoadGeneralDefaultCFG_Set(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }
    if(pParam->nDataLen != 0)
    {
		return SC_RESULT_FAIL;
    }
    
    if(!X_paramOriginalInitAllParam())
        return SC_RESULT_FAIL;

    if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else
        X_SysParamCpy();

     return nResult;
}

int LoadGeneralDefaultCFG_Get(sc_param_st *pParam){ return SC_RESULT_SUCC; }
//用于各通讯接口类型的切换设置	Tag:INTERF(0~10)
const unsigned int nCommInterfaceTab[9] = {0x0,0x01,0x06,0x02,0x07,0x08,0x09,0x0a,0x0b};
int CommunicationInterface_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

      if(pParam->nDataLen == 0)
      {
		return SC_RESULT_FAIL;
	}
        

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 8)
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pHard->InterType = nCommInterfaceTab[nVal];
	//X_paramUpdateToDevice();
	g_commChange = 1;

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pHard->InterType = X_paramGetGlobalAllParam()->pHard->InterType;
	}

	return nResult;
}



const unsigned int nCurrentInterfaceTab[9] = {0x1,0x02,0x04,0x03,0x06,0x07,0x08,0x09,0xa};

int CommunicationInterface_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	int	 nIndex = 0;
	char cBuff[8];
	HardParam pHard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

      if(!(pParam->nQueryType))
      {
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS:	//RAM
        		//nVal = X_paramGetGlobalAllParam()->pHard->InterType & 0xFF;Y_commGetCurType();//
        		//nVal = Y_commGetCurType();
        		nVal = X_paramGetGlobalAllParam()->pHard->InterType & 0xFF;
        		break;

        	case SC_CFG_SYS:	//SYS
        		//nVal = Y_commGetCurType();
        		nVal = X_paramBakGetGlobalAllParam()->pHard->InterType & 0xFF;
        		break;

        	case SC_CFG_FACT:	//FAC
        		if (X_FacParamIsGet() != TRUE)
        		{
        			return SC_RESULT_FAIL;
        		}
        		memcpy(&pHard, FacParamBuf + Start_MEMSize_HARD, sizeof(HardParam));
        		nVal = pHard.InterType & 0xFF;
        		break;

        	default:
        		return SC_RESULT_UnSurpport;
        		break;
        	}

            //printf("nval=%d\r\n",nVal);

        	for (nIndex = 0; nIndex < 9; nIndex++)
        	{

        		if (nVal == ((unsigned int *)nCommInterfaceTab)[nIndex])
        		{
        			break;
        		}
        	}

        	if (nVal != ((unsigned int *)nCommInterfaceTab)[nIndex])
        	{
        		return SC_RESULT_FAIL;
        	}

        	nLen = sprintf(cBuff, "%d", nIndex);
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
      }
      else
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-3|5-8");
	return nResult;

}
//设置波特率 Tag:232BAD const unsigned int
const unsigned int nRS232BaudrateTab[9] = { 1200,2400,4800,9600,14400,19200,38400,57600,115200 };
int RS232Baudrate_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam->nDataLen == 0)
    {
        return SC_RESULT_FAIL;
    }

    if(!pParam -> pData || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 8)
    { 
        return SC_RESULT_FAIL; 
    }

	X_paramGetGlobalAllParam()->pCom->lRate = nRS232BaudrateTab[nVal];
      g_commChange = 1;//应答后再更新设备参数
	//X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))//不保存
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else //保存
	{
		X_paramBakGetGlobalAllParam()->pCom->lRate = X_paramGetGlobalAllParam()->pCom->lRate;
	}
      return nResult;

}

int RS232Baudrate_Get(sc_param_st *pParam)
{
    int  nVal,nLen, nResult = SC_RESULT_SUCC;
    int	 nIndex = 0;
    char cBuff[8];
    TComParam pComparam;

    if (!pParam->pOutBuf)
    {
        return SC_RESULT_FAIL;
    }
    
    if(!(pParam->nQueryType))
    {
        switch (pParam->nConfType)
        {
            case SC_CFG_NSYS://RAM
                nVal = X_paramGetGlobalAllParam()->pCom->lRate & 0xFFFFF;
            break;

            case SC_CFG_SYS://SYS
                nVal = X_paramBakGetGlobalAllParam()->pCom->lRate & 0xFFFFF;
            break;

            case SC_CFG_FACT://FAC
                if (X_FacParamIsGet() != TRUE)
                {
                    return SC_RESULT_FAIL;
                }
                memcpy(&pComparam, FacParamBuf + Start_MEMSize_COM, sizeof(TComParam));
                nVal = pComparam.lRate & 0xFFFFF;
            break;

            default:
                return SC_RESULT_UnSurpport;
            break;
        }

        for (nIndex = 0; nIndex < 9; nIndex++)
        {
            if (nVal == ((unsigned int *)nRS232BaudrateTab)[nIndex])
            {
                break;
            }
        }

        if( nVal != ((unsigned int *)nRS232BaudrateTab)[nIndex] )
        {
            return SC_RESULT_FAIL;
        }

        nLen = sprintf(cBuff, "%d", nIndex);
        if (pParam->nOutBufSize >= nLen)
        {
            memcpy(pParam->pOutBuf, cBuff, nLen);
            *pParam->pOutLen = nLen;
        }
        else
        {
            nResult = SC_RESULT_FAIL;
        }
    }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-8");
    }
	return nResult;

}

//设置RS232校验设置 Tag:232PAR
const unsigned int nRS232ParTab[3] = { NOPARITY , ODDPARITY, EVENPARITY};
int RS232Parity_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_SaveAndUpdateAll;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam->nDataLen == 0)
    {
        return SC_RESULT_FAIL;
    }

    if(!pParam -> pData || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 2)
    { 
        return SC_RESULT_FAIL; 
    }

    X_paramGetGlobalAllParam()->pCom->lParity = nRS232ParTab[nVal];
	//X_paramUpdateToDevice();
	g_commChange = 1;

	if (!(pParam->bSaveFlag))//不保存
	{
		nResult = SC_RESULT_UnsaveAndUpdateAll;
	}
	else//保存
	{
		X_paramBakGetGlobalAllParam()->pCom->lParity = X_paramGetGlobalAllParam()->pCom->lParity;
	}

    return nResult;

}

int RS232Parity_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	int	 nIndex = 0;
	char cBuff[8];
	TComParam pComparam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
    
      if(!(pParam->nQueryType))
      {
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS://RAM
        		nVal = X_paramGetGlobalAllParam()->pCom->lParity & 0xFF;
        		break;

        	case SC_CFG_SYS://SYS
        		nVal = X_paramBakGetGlobalAllParam()->pCom->lParity & 0xFF;
        		break;

        	case SC_CFG_FACT://FAC
        		if (X_FacParamIsGet() != TRUE)
        		{
        			return SC_RESULT_FAIL;
        		}
        		memcpy(&pComparam, FacParamBuf + Start_MEMSize_COM, sizeof(TComParam));
        		nVal = pComparam.lParity & 0xFF;
        		break;

        	default:
        		return SC_RESULT_UnSurpport;
        		break;
        	}

        	for (nIndex = 0; nIndex < 3; nIndex++)
        	{
        		if (nVal == ((unsigned int *)nRS232ParTab)[nIndex])
        		{
        			break;
        		}
        	}

        	if (nVal != ((unsigned int *)nRS232ParTab)[nIndex])
        	{
        		return SC_RESULT_FAIL;
        	}

        	nLen = sprintf(cBuff, "%d", nIndex);
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
      }
      else
      {  
            *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-2");
      }
	return nResult;
}

//设置RS232停止位 Tag：232STP
const unsigned int nRS232StopTab[2] = {0x1,0x2};
int RS232StopBits_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_SaveAndUpdateAll;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam->nDataLen == 0)
    {
        return SC_RESULT_FAIL;
    }

    if(!pParam -> pData || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    X_paramGetGlobalAllParam()->pCom->lStopBits = nRS232StopTab[nVal];
	//X_paramUpdateToDevice();
	g_commChange = 1;

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_UnsaveAndUpdateAll;
    else//保存
        X_paramBakGetGlobalAllParam()->pCom->lStopBits = X_paramGetGlobalAllParam()->pCom->lStopBits;

    return nResult;

}

int RS232StopBits_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	int	 nIndex = 0;
	char cBuff[8];
	TComParam pComparam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
    
      if(!(pParam->nQueryType))
      {
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS: //RAM
        		nVal = X_paramGetGlobalAllParam()->pCom->lStopBits & 0xFF;
        		break;

        	case SC_CFG_SYS: //SYS
        		nVal = X_paramBakGetGlobalAllParam()->pCom->lStopBits & 0xFF;
        		break;

        	case SC_CFG_FACT: //FAC
        		if (X_FacParamIsGet() != TRUE)
        		{
        			return SC_RESULT_FAIL;
        		}
        		memcpy(&pComparam, FacParamBuf + Start_MEMSize_COM, sizeof(TComParam));
        		nVal = pComparam.lStopBits & 0xFF;
        		break;

        	default:
        		return SC_RESULT_UnSurpport;
        		break;
        	}

        	for (nIndex = 0; nIndex < 2; nIndex++)
        	{
        		if (nVal == ((unsigned int *)nRS232StopTab)[nIndex])
        		{
        			break;
        		}
        	}

        	if (nVal != ((unsigned int *)nRS232StopTab)[nIndex])
        	{
        		return SC_RESULT_FAIL;
        	}

        	nLen = sprintf(cBuff, "%d", nIndex);
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
      }
      else
      {  
             *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-1");
      }
	return nResult;
}

//RS232设置数据位， Tag:232DAT
const unsigned int nRS232DatBitsTab[2] = {0x08,0x07};
int RS232DataBits_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_SaveAndUpdateAll;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam->nDataLen == 0)
    {
        return SC_RESULT_FAIL;
    }

    if(!pParam -> pData || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }

   
    X_paramGetGlobalAllParam()->pCom->lDataBits = nRS232DatBitsTab[nVal];
	//X_paramUpdateToDevice();
	g_commChange = 1;

    if(!(pParam->bSaveFlag)) //不保存
        nResult = SC_RESULT_UnsaveAndUpdateAll;
    else//保存
        X_paramBakGetGlobalAllParam()->pCom->lDataBits= X_paramGetGlobalAllParam()->pCom->lDataBits;

  
    return nResult;

}

int RS232DataBits_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	int	 nIndex = 0;
	char cBuff[8];
	TComParam pComparam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
    
      if(!(pParam->nQueryType))
	{
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS: //RAM
        		nVal = X_paramGetGlobalAllParam()->pCom->lDataBits & 0xFF;
        		break;

        	case SC_CFG_SYS: //SYS
        		nVal = X_paramBakGetGlobalAllParam()->pCom->lDataBits & 0xFF;
        		break;

        	case SC_CFG_FACT: //FAC
        		if (X_FacParamIsGet() != TRUE)
        		{
        			return SC_RESULT_FAIL;
        		}
        		memcpy(&pComparam, FacParamBuf + Start_MEMSize_COM, sizeof(TComParam));
        		nVal = pComparam.lDataBits & 0xFF;
        		break;

        	default:
        		return SC_RESULT_UnSurpport;
        		break;
        	}

        	for (nIndex = 0; nIndex < 2; nIndex++)
        	{
        		if (nVal == ((unsigned int *)nRS232DatBitsTab)[nIndex])
        		{
        			break;
        		}
        	}

        	if (nVal != ((unsigned int *)nRS232DatBitsTab)[nIndex])
        	{
        		return SC_RESULT_FAIL;
        	}

        	nLen = sprintf(cBuff, "%d", nIndex);
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
      }
      else
      {  
             *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-3");
      }
	return nResult;
}

//用于RS232硬件自动流控的使能和禁止	Tag:232AFL
const unsigned int nRS232AFLTab[2] = {0x00,0x01};
int RS232HardwereAutoFlowCTL_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;
    return SC_RESULT_UnSurpport;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> pData || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }

   
    X_paramGetGlobalAllParam()->pCom->nflowCtrl = nRS232AFLTab[nVal];
	X_paramUpdateToDevice();

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_UnsaveAndUpdateAll;
    else//保存
        X_paramBakGetGlobalAllParam()->pCom->nflowCtrl = X_paramGetGlobalAllParam()->pCom->nflowCtrl;

  
    return nResult;

}

int RS232HardwereAutoFlowCTL_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	int	 nIndex = 0;
	char cBuff[8];
	TComParam pComparam;
return SC_RESULT_UnSurpport;
	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS: //RAM
		nVal = X_paramGetGlobalAllParam()->pCom->nflowCtrl & 0xFF;
		break;

	case SC_CFG_SYS: //SYS
		nVal = X_paramBakGetGlobalAllParam()->pCom->nflowCtrl & 0xFF;
		break;

	case SC_CFG_FACT: //FAC
		if (X_FacParamIsGet() != TRUE)
		{
			return SC_RESULT_FAIL;
		}
		memcpy(&pComparam, FacParamBuf + Start_MEMSize_COM, sizeof(TComParam));
		nVal = pComparam.nflowCtrl & 0xFF;
		break;

	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	for (nIndex = 0; nIndex < 2; nIndex++)
	{
		if (nVal == ((unsigned int *)nRS232AFLTab)[nIndex])
		{
			break;
		}
	}

	if (nVal != ((unsigned int *)nRS232AFLTab)[nIndex])
	{
		return SC_RESULT_FAIL;
	}

	nLen = sprintf(cBuff, "%d", nIndex);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

int RS232NoAutoFlowCTL_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}
int RS232NoAutoFlowCTL_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}

//用于Keyboard Country Layout（键盘语言）的设置	Tag:KBWCTY
int KBWCountryLayout_Set(sc_param_st *pParam)
{
        int  nVal, nResult = SC_RESULT_Save;

        if( !pParam )
        { return SC_RESULT_FAIL; }

        //if((!pParam->bCompleteFlag) && (!pParam -> nDataLen))
        if(!pParam -> nDataLen)
        {//需要设置码 
            pParam -> nDigitMaxCnt = 2;
            return SC_RESULT_NEXT; 
        }
        
        if(!pParam -> pData || pParam -> nDataLen > KBWlLANGNUM-1)
        { return SC_RESULT_FAIL; }

        if(!IsDec(pParam -> pData, pParam -> nDataLen))
        { return SC_RESULT_FAIL; }

        nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
        if(nVal < 0 || nVal > KBWlLANGNUM)
        { return SC_RESULT_FAIL; }

		X_paramGetGlobalAllParam()->pKBW->nLanguage = nVal;
#if 0
		if ( nVal >=0 && nVal<= 19)
		{
			X_paramGetGlobalAllParam()->pKBW->nLanguage = nVal + 1;
		} 
		else if( nVal >= 21 && nVal <= 28)
		{
			X_paramGetGlobalAllParam()->pKBW->nLanguage = nVal;
		}
#endif
        
		X_paramUpdateToDevice();

        if(!(pParam->bSaveFlag))//不保存
            nResult = SC_RESULT_SuccUnsave;
        else//保存
            X_paramBakGetGlobalAllParam()->pKBW->nLanguage= X_paramGetGlobalAllParam()->pKBW->nLanguage;
        
        return nResult;
}

int KBWCountryLayout_Get(sc_param_st *pParam)
{
    int nVal, nResult = SC_RESULT_SUCC;
    int nLen;
    UTKeyBoard pKeyboard;
    char cBuff[8];

    if (!pParam->pOutBuf)
    {
        return SC_RESULT_FAIL;
    }
    
    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pKBW->nLanguage &0xFF;          
            break;

            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pKBW->nLanguage & 0xFF;        
            break;

            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;

                memcpy(&pKeyboard, FacParamBuf+Start_MEMSize_UTK, sizeof(UTKeyBoard));
                nVal =  pKeyboard.nLanguage & 0xFF;             
            break; 
            default:
                return SC_RESULT_UnSurpport;
            break;
        }

        if (nVal < 0 || nVal > KBWlLANGNUM)
        {
            return SC_RESULT_FAIL;
        }

        nLen = sprintf(cBuff, "%d", nVal);
        if (pParam->nOutBufSize >= nLen)
        {
            memcpy(pParam->pOutBuf, cBuff, nLen);
            *pParam->pOutLen = nLen;
        }
        else
        {
            nResult = SC_RESULT_FAIL;
        }
    }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-28");
    }
    return nResult;
}

//用于传输未知字符的提示音使能、禁止	Tag:KBWBUC
int KBWBeepOnUnknownChar_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->pData || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	} 
    
      if (pParam->nDataLen == 0)
	{
		return SC_RESULT_FAIL;
	} 
	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)		//0:禁止，1：使能
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pKBW->isUnknownCharactersBeep = nVal;
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pKBW->isUnknownCharactersBeep = X_paramGetGlobalAllParam()->pKBW->isUnknownCharactersBeep;
	}
	return nResult;
}
int KBWBeepOnUnknownChar_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTKeyBoard pKeyBoard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pKBW->isUnknownCharactersBeep & 0x01;
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pKBW->isUnknownCharactersBeep & 0x01;
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pKeyBoard, FacParamBuf + Start_MEMSize_UTK ,sizeof(UTKeyBoard));
		nVal = pKeyBoard.isUnknownCharactersBeep & 0x01;

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//用于虚拟键盘类型1的禁止、类型设置	Tag:KBWALT
int KBWEmulateAltKeypad1_Set(sc_param_st *pParam)
{ 
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->pData || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

      if (pParam->nDataLen == 0)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)		//0:禁止，1：使能
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pKBW->isEmulateKeypad = nVal;
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pKBW->isEmulateKeypad = X_paramGetGlobalAllParam()->pKBW->isEmulateKeypad;
	}
	return nResult;
}

int KBWEmulateAltKeypad1_Get(sc_param_st *pParam)
{ 
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTKeyBoard pKeyBoard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pKBW->isEmulateKeypad & 0x01;
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pKBW->isEmulateKeypad & 0x01;
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pKeyBoard, FacParamBuf + Start_MEMSize_UTK ,sizeof(UTKeyBoard));
		nVal = pKeyBoard.isEmulateKeypad & 0x01;

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//用于虚拟键盘类型2的使能、禁止	Tag：KBWALK

int KBWEmulateAltKeypad2_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int KBWEmulateAltKeypad2_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//用于功能按键组合Ctrl+ASCII的使能和禁止	Tag:KBWFKM
int KBWCtrlPlusStatus_Set(sc_param_st* pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->pData || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

      if (pParam->nDataLen == 0)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)		//0:禁止，1：使能
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pKBW->isCtrlASCIIMode = nVal;
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pKBW->isCtrlASCIIMode = X_paramGetGlobalAllParam()->pKBW->isCtrlASCIIMode;
	}
	return nResult;
}

int KBWCtrlPlusStatus_Get(sc_param_st* pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTKeyBoard pKeyBoard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

      if (pParam->nQueryType)
	{
		nLen = sprintf(cBuff, "%d-%d", 0,1);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
		return nResult;
	}
      else
      {

	    switch (pParam->nConfType)
	    {
	        case SC_CFG_NSYS:	//RAM
		    nVal = X_paramGetGlobalAllParam()->pKBW->isCtrlASCIIMode & 0x01;
		    break;

	        case SC_CFG_SYS:	//SYS
		    nVal = X_paramBakGetGlobalAllParam()->pKBW->isCtrlASCIIMode & 0x01;
		    break;

	        case SC_CFG_FACT:	//FAC
		    if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		    memcpy(&pKeyBoard, FacParamBuf + Start_MEMSize_UTK, sizeof(UTKeyBoard));
		    nVal = pKeyBoard.isCtrlASCIIMode & 0x01;

		    break;
	        default:
		    return SC_RESULT_UnSurpport;
		    break;
	    }

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
    }
}

//用于内部键盘输出延时的设置	Tag:KBWDLY
int KBWInterKeyStrokeDelay_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;
//	int nCnt;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		//pParam->nDigitMaxCnt = Max_nPreSubfix;
		return SC_RESULT_FAIL;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

	if (pParam->nDataLen > 3 )		//时间长度最多100，3字符
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 100)		//时间长度最多100，3字符
	{
		return SC_RESULT_FAIL;
	}
	
      switch(nVal)
      {
            case 0:
                X_paramGetGlobalAllParam()->pKBW->isKeysrockeDelay = 0;
                break;
            case 20:
                X_paramGetGlobalAllParam()->pKBW->isKeysrockeDelay = 1;
                break;
            case 40:
                X_paramGetGlobalAllParam()->pKBW->isKeysrockeDelay = 2;
                break;
            default:
                return SC_RESULT_FAIL;
                break;

      }
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pKBW->isKeysrockeDelay = X_paramGetGlobalAllParam()->pKBW->isKeysrockeDelay;
	}

	return nResult;
}

int KBWInterKeyStrokeDelay_Get(sc_param_st *pParam)
{
	int  nVal, nLen=0, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTKeyBoard pKeyBoard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
    
      if(!(pParam->nQueryType))
      {     
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS:	//RAM
        		nVal = X_paramGetGlobalAllParam()->pKBW->isKeysrockeDelay;
        		break;

        	case SC_CFG_SYS:	//SYS
        		nVal = X_paramBakGetGlobalAllParam()->pKBW->isKeysrockeDelay;
        		break;

        	case SC_CFG_FACT:	//FAC
        		if (X_FacParamIsGet() != TRUE)
        			return SC_RESULT_FAIL;

        		memcpy(&pKeyBoard, FacParamBuf + Start_MEMSize_UTK, sizeof(UTKeyBoard));
        		nVal = pKeyBoard.isKeysrockeDelay & 0x01;

        		break;
        	default:
        		return SC_RESULT_UnSurpport;
        		break;
        	}

        	if (nVal == 0)
        	{
        		nLen = sprintf(cBuff, "%d", 0);
        	} 
        	else if(nVal == 1)
        	{
        		nLen = sprintf(cBuff, "%d", 20);
        	}
        	else if(nVal == 2)
        	{
        		nLen = sprintf(cBuff, "%d", 40);
        	}
            
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
       }
      else
      {  
            *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0|Short:20|Long:40");
      }
	return nResult;
}

//用于键盘Caps Lock键的使能、禁止	Tag:KBWCAP
int KBWCapsLockStatus_Set(sc_param_st *pParam)
{ 
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen|| pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)		//order值 <= 6
	{
		return SC_RESULT_FAIL;
	}

	if (nVal == 1)
	{
		X_paramGetGlobalAllParam()->pKBW->isCapsLock = TRUE;
	} 
	else if (nVal == 0)
	{
		X_paramGetGlobalAllParam()->pKBW->isCapsLock = FALSE;
	}
	
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pKBW->isCapsLock = X_paramGetGlobalAllParam()->pKBW->isCapsLock;
	}
	return nResult;
}
int KBWCapsLockStatus_Get(sc_param_st *pParam)
{ 
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTKeyBoard pKeyBoard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pKBW->isCapsLock & 0x01;
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pKBW->isCapsLock & 0x01;
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pKeyBoard, FacParamBuf + Start_MEMSize_UTK, sizeof(UTKeyBoard));
		nVal = pKeyBoard.isCapsLock & 0x01;

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}


	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//用于键盘的大小写转换功能的使能、禁止	Tag：KBWCAS
int KBWConvertCaseStatus_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 2)		
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pKBW->isConvertCase = nVal;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pKBW->isConvertCase = X_paramGetGlobalAllParam()->pKBW->isConvertCase;
	}
	return nResult;
}

int KBWConvertCaseStatus_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTKeyBoard pKeyBoard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
    
      if(!(pParam->nQueryType))
      {
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS:	//RAM
        		nVal = X_paramGetGlobalAllParam()->pKBW->isConvertCase & 0x03;
        		break;

        	case SC_CFG_SYS:	//SYS
        		nVal = X_paramBakGetGlobalAllParam()->pKBW->isConvertCase & 0x03;
        		break;

        	case SC_CFG_FACT:	//FAC
        		if (X_FacParamIsGet() != TRUE)
        			return SC_RESULT_FAIL;

        		memcpy(&pKeyBoard, FacParamBuf + Start_MEMSize_UTK, sizeof(UTKeyBoard));
        		nVal = pKeyBoard.isConvertCase & 0x03;

        		break;
        	default:
        		return SC_RESULT_UnSurpport;
        		break;
        	}

        	nLen = sprintf(cBuff, "%d", nVal);
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
      }
      else
      {  
            *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-2");
      }
	return nResult;
}

//用于设置虚拟数字键盘的输出类型	Tag:KBWNUM
//const unsigned short c_nKBWNumPadEnTab[2] = {0, eVKBW_NUMPAD};
int KBWEmulateNUMPADType_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }

	X_paramGetGlobalAllParam()->pKBW->isNumericMode = nVal;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pKBW->isNumericMode = X_paramGetGlobalAllParam()->pKBW->isNumericMode;
	}
	return nResult;
}

int KBWEmulateNUMPADType_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTKeyBoard pKeyBoard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pKBW->isNumericMode & 0x01;
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pKBW->isNumericMode & 0x01;
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
		return SC_RESULT_FAIL;

		memcpy(&pKeyBoard, FacParamBuf + Start_MEMSize_UTK, sizeof(UTKeyBoard));
		nVal = pKeyBoard.isNumericMode & 0x01;

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//用于设置虚拟数字键盘的运算符"+,-,*"是否采用采用Numeric Keypad 键值 	Tag:KBWNCH
int KBWEmulateNUMPADOperatorStatus_Set(sc_param_st *pParam)
{ 
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }

	X_paramGetGlobalAllParam()->pKBW->isKeyPad = nVal;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pKBW->isKeyPad = X_paramGetGlobalAllParam()->pKBW->isKeyPad;
	}
	return nResult;
}
int KBWEmulateNUMPADOperatorStatus_Get(sc_param_st *pParam)
{ 
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTKeyBoard pKeyBoard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pKBW->isKeyPad & 0x01;
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pKBW->isKeyPad & 0x01;
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pKeyBoard, FacParamBuf + Start_MEMSize_UTK, sizeof(UTKeyBoard));
		nVal = pKeyBoard.isKeyPad & 0x01;

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//用于设置快速模式是开启或者关闭
int KBWFastMode_Set(sc_param_st *pParam)
{ 
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pKBW->isFasttransferMode = nVal;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pKBW->isFasttransferMode = X_paramGetGlobalAllParam()->pKBW->isFasttransferMode;
	}
	return nResult;
}

int KBWFastMode_Get(sc_param_st *pParam)
{ 
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTKeyBoard pKeyBoard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pKBW->isFasttransferMode & 0x01;
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pKBW->isFasttransferMode & 0x01;
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pKeyBoard, FacParamBuf + Start_MEMSize_UTK, sizeof(UTKeyBoard));
		nVal = pKeyBoard.isFasttransferMode & 0x01;

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//用于KBW轮询速度的设置和查询 Tag:KBWPOR
int KBWPollRate_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_SaveAndUpdateAll;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 9)
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pKBW->nPollRate = nVal+1;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_UnsaveAndUpdateAll;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pKBW->nPollRate = X_paramGetGlobalAllParam()->pKBW->nPollRate;
	}
	return nResult;
}

int KBWPollRate_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTKeyBoard pKeyBoard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
    
      if(!(pParam->nQueryType))
      {
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS:	//RAM
        		nVal = X_paramGetGlobalAllParam()->pKBW->nPollRate & 0x0F;
        		break;

        	case SC_CFG_SYS:	//SYS
        		nVal = X_paramBakGetGlobalAllParam()->pKBW->nPollRate & 0x0F;
        		break;

        	case SC_CFG_FACT:	//FAC
        		if (X_FacParamIsGet() != TRUE)
        			return SC_RESULT_FAIL;

        		memcpy(&pKeyBoard, FacParamBuf + Start_MEMSize_UTK, sizeof(UTKeyBoard));
        		nVal = pKeyBoard.nPollRate & 0x0F;

        		break;
        	default:
        		return SC_RESULT_UnSurpport;
        		break;
        	}

        	nLen = sprintf(cBuff, "%d", nVal-1);
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
      }
      else
      {  
            *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-9");
      }
	return nResult;
}

//用于KBW CODE PAGE设置和查询  Tag:KBWCPG
int KBWCodePage_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = 2;
		return SC_RESULT_NEXT;
	}
	if (!pParam->pData )	//|| pParam->nDataLen > 8
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 9)
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pKBW->nCodePage = nVal;

	

	if (!(pParam->bSaveFlag))
	{     
	      X_paramUpdateToDevice();
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pKBW->nCodePage = X_paramGetGlobalAllParam()->pKBW->nCodePage;
	}
	return nResult;
}

int KBWCodePage_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTKeyBoard pKeyBoard;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
    
      if(!(pParam->nQueryType))
     {
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS:	//RAM
        		nVal = X_paramGetGlobalAllParam()->pKBW->nCodePage & 0x0F;
        		break;

        	case SC_CFG_SYS:	//SYS
        		nVal = X_paramBakGetGlobalAllParam()->pKBW->nCodePage & 0x0F;
        		break;

        	case SC_CFG_FACT:	//FAC
        		if (X_FacParamIsGet() != TRUE)
        			return SC_RESULT_FAIL;

        		memcpy(&pKeyBoard, FacParamBuf + Start_MEMSize_UTK, sizeof(UTKeyBoard));
        		nVal = pKeyBoard.nCodePage & 0x0F;

        		break;
        	default:
        		return SC_RESULT_UnSurpport;
        		break;
        	}

        	nLen = sprintf(cBuff, "%d", nVal);
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
      }
      else
      {  
            *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-2");
      }
	return nResult;
}

/****************************************************************************
*@无线枪相关设置码
****************************************************************************/
//用于恢复底座出厂设置 Tag:BASFDF
int LoadFactoryCFGOfPedestal_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于底座清空配对信息 Tag:BASCLP
int ClearConfigInfoOfPedestal_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于查询无线扫描器相关全部信息	Tag WLSQAL
int AllInfoInWirelessScanner_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于查询底座系统信息	Tag BASQSY
int SysInfoOfPedestal_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于查询底座固件版本号	Tag BASQFW
int FirmwareVerOfPedestal_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于查询底座产品型号	Tag BASQHN
int ProductTypeOfPedestal_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于查询底座产品序列号	Tag BASQSN
int SerialNumOfPedestal_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于查询底座产品生产日期	Tag BASQDA
int DateOfPedestal_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于Batch Mode的设置	Tag：WLSBAT
int BatchModesOfWirelessScanner_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BatchModesOfWirelessScanner_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于查询用户FLASH内条码数量	Tag:WLSQFC
int AmountOfCodesInUserFlash_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于清除用户FLASH内条码		Tag:WLSCLF
int ClearCodesInUserFlash_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于开启、关闭核对功能		Tag：WLSCHK
int CheckingFunctionEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int CheckingFunctionEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于设置核对超时时间			Tag：WLSCHT
int TimeoutOfCheckingFunction_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TimeoutOfCheckingFunction_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于设置待机时间以进入自动关机 Tag：WLSAPO
int IdleTimeForAutoPowerdown_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int IdleTimeForAutoPowerdown_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于设置Zigbee无线信道	Tag：WLSZCH
int SelectChannelOfZigbee_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int SelectChannelOfZigbee_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于底座重启				Tag：BASRES
int ResetPedestal_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//用于设置无线传输应答超时时间 	Tag：WLSTTM
int TimeoutForACKInWirelessTrmt_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TimeoutForACKInWirelessTrmt_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@照明灯、瞄准灯相关设置
****************************************************************************/
//设置照明灯的开、关	Tag：ILLSCN

int IlluminationEnable_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 2)
    { 
        return SC_RESULT_FAIL; 
    }  

    if(nVal==0)
        nVal = 2;
    else if(nVal==1)
        nVal = 0;
    else
        nVal = 1;
    X_paramGetGlobalAllParam()->pHard->nLight = nVal;
    X_paramUpdateToDevice();

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pHard->nLight= X_paramGetGlobalAllParam()->pHard->nLight;

  
    return nResult;

}
int IlluminationEnable_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    HardParam hHard;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pHard->nLight;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pHard->nLight;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&hHard, FacParamBuf+Start_MEMSize_HARD, sizeof(HardParam));
                nVal = hHard.nLight;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        if(nVal == 2)
            nVal = 0;
        else if(nVal == 1)
            nVal = 2;
        else
            nVal = 1;
        pParam->pOutBuf[0] = nVal+0x30;
        *(pParam->pOutLen) = 1;
    }
    else
    {
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-3");
    }
    return nResult;
}

//设置照明灯的颜色类别 	Tag：ILLCLR
int ColorTypeOfIllumination_Set(sc_param_st *pParam)
{ 
        return SC_RESULT_UnSurpport; 
}
int ColorTypeOfIllumination_Get(sc_param_st *pParam)
{ 
        return SC_RESULT_UnSurpport; 
}

//设置照明灯的位置		Tag：ILLPST
int PositionOfIllumination_Set(sc_param_st *pParam)
{ 
        return SC_RESULT_UnSurpport; 
}
int PositionOfIllumination_Get(sc_param_st *pParam)
{ 
        return SC_RESULT_UnSurpport; 
}

//设置瞄准灯的开、关	Tag：AMLENA
//const unsigned short c_nAimerSetTab[2] = {eRMP_FOCUS_ALWAYS_OFF, eRMP_FOCUS_NORMAL};
int AimerEnable_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 2)
    { 
        return SC_RESULT_FAIL; 
    }  

    if(nVal==0)
        nVal = 2;
    else if(nVal==1)
        nVal = 0;
    else
        nVal = 1;
    X_paramGetGlobalAllParam()->pHard->nFocus= nVal;
    X_paramUpdateToDevice();

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pHard->nFocus= X_paramGetGlobalAllParam()->pHard->nFocus;

  
    return nResult;

}
int AimerEnable_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    HardParam hHard;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pHard->nFocus;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pHard->nFocus;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&hHard, FacParamBuf+Start_MEMSize_HARD, sizeof(HardParam));
                nVal = hHard.nFocus;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        if(nVal == 2)
            nVal = 0;
        else if(nVal == 1)
            nVal = 2;
        else
            nVal = 1;
        pParam->pOutBuf[0] = nVal+0x30;
        *(pParam->pOutLen) = 1;
    }
    else
    {
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-3");
    }
    return nResult;
}

/****************************************************************************
*@串口触发指令
****************************************************************************/
//用于设置串口触发指令	Tag:SCNTRG
int	Trigger2DecoBySerialCommand_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_SuccUnsave;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }  

    if (nVal == 1)	//开启
    {
        if(X_paramGetGlobalAllParam()->pCode->nReadMode==ESC0)
        {
            X_paramGetGlobalAllParam()->pCode->nReadMode = ESC1;
       
            X_paramGetGlobalAllParam()->pMode->bContinue = TRUE;
            X_paramGetGlobalAllParam()->pMode->nReadDelay = Z_Get10msCounter();
        }
    }
    else if (nVal == 0)	//关闭
    {
        X_paramGetGlobalAllParam()->pCode->nReadMode = ESC0;
        //X_paramGetGlobalAllParam()->pMode->bContinue = TRUE;
    }

    return nResult;

}
int	Trigger2DecoBySerialCommand_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@Setting in Sense Mode
****************************************************************************/
//用于选择Sense Mode下读码成功后的工作状态（After A Good Read）	Tag:SENAGR
//const unsigned short c_nSenseFullTmTab[2] = {0, eRMPE_AUTO_FULLTIME};
int StatusAfterGoodReadInSenseMode_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}
int StatusAfterGoodReadInSenseMode_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}

//用于选择Sense Mode下侦测状态时照明灯的开关	Tag:SENILL
int IlluStatusInSensingForSenseMode_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int IlluStatusInSensingForSenseMode_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@Exposure Imaging
****************************************************************************/
//设置镜头的曝光类型		Tag:EXPLVL
int ExposureTypes_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal == 0 || nVal == 2)
    { 
        X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = nVal;
    }  
    else
        return SC_RESULT_UnSurpport;

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode = nVal;
  
    return nResult;

}
int ExposureTypes_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    CodeParam2 cCode2;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }
    
    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&cCode2, FacParamBuf+Start_MEMSize_HARD, sizeof(CodeParam2));
                nVal = cCode2.cmosAdjustMode;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        pParam->pOutBuf[0] = nVal+0x30;
        *(pParam->pOutLen) = 1;
    }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0|2");
    }
    return nResult;
}

/****************************************************************************
*@Decode Area
****************************************************************************/
//设置解码区域	Tag：CADENA
//const unsigned char c_nAreaDecoEnTab[] = {0, eMORERVD_EnCentreArea};
int DecoArea_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }  

    if(nVal == 0)
    {//全区读取
        X_paramGetGlobalAllParam()->pCode->decodeArea &= ~0xf;
        X_paramGetGlobalAllParam()->pAreaParam->nType = 0;
    }
    else if(nVal == 1)
    { //中心区域读取
        X_paramGetGlobalAllParam()->pCode->decodeArea |= 1;//旧版本中心区域解码设置
        X_paramGetGlobalAllParam()->pAreaParam->nType = 1;//新版本中心区域解码设置
    }

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
    {
        X_paramBakGetGlobalAllParam()->pCode->decodeArea = X_paramGetGlobalAllParam()->pCode->decodeArea;
        X_paramBakGetGlobalAllParam()->pAreaParam->nType = X_paramGetGlobalAllParam()->pAreaParam->nType;
    }
        
    return nResult;

}
int DecoArea_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    //CodeParam cCode;
    UIMGAreaParam uArea;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                //nVal = X_paramGetGlobalAllParam()->pCode->decodeArea;
                nVal = X_paramGetGlobalAllParam()->pAreaParam->nType;
                break;
            case 1://SYS
                //nVal = X_paramBakGetGlobalAllParam()->pCode->decodeArea;
                nVal = X_paramBakGetGlobalAllParam()->pAreaParam->nType;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                //memcpy(&cCode, FacParamBuf+Start_MEMSize_CODE, sizeof(CodeParam));
                //nVal = cCode.decodeArea;   
                memcpy(&uArea, FacParamBuf+Start_MEMSize_AREAPARAM, sizeof(UIMGAreaParam));
                nVal = uArea.nType;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        pParam->pOutBuf[0] = nVal + 0x30;
        *(pParam->pOutLen) = 1;
    }
    else
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-1");

    return nResult;
    //return SC_RESULT_UnSurpport;
}

//设置中心区域的顶端范围	Tag：CADTOP, 必须比Bottom的值小
int TopRangeOfCentralArea_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;
    int bottom,top;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        pParam -> nDigitMaxCnt = 3;
        return SC_RESULT_NEXT;  
    }

    if(!pParam -> pData ||  pParam -> nDataLen >3)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 100)
    { return SC_RESULT_FAIL; }

    top = Z_GetCmosHeight() * nVal /100;
    bottom = X_paramGetGlobalAllParam()->pAreaParam->bottom;
    if((top > bottom)||(top == bottom))
        return SC_RESULT_FAIL;
     
    X_paramGetGlobalAllParam()->pAreaParam->top = top;
    X_decoIniDecArea(X_paramGetGlobalAllParam()->pAreaParam);


    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pAreaParam->top = top;

  
    return nResult;
    //return SC_RESULT_UnSurpport;
 
}
int TopRangeOfCentralArea_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    UIMGAreaParam uArea;
    char cBuff[16];
    unsigned int nLen;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pAreaParam->top;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pAreaParam->top;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&uArea, FacParamBuf+Start_MEMSize_AREAPARAM, sizeof(UIMGAreaParam));
                nVal = uArea.top;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        nVal *= 100;
        nVal /= Z_GetCmosHeight();

        nLen = sprintf(cBuff, "%d", nVal);

        memcpy(pParam->pOutBuf, cBuff, nLen);
        *(pParam->pOutLen) = nLen;
    }
    else
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-100");
    
    return nResult;
    //return SC_RESULT_UnSurpport;
    
}

//设置中心区域的底端范围	Tag：CADBOT,必须必top大
int BottomRangeOfCentralArea_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;
    int bottom,top;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        pParam -> nDigitMaxCnt = 3;
        return SC_RESULT_NEXT;  
    }

    if(!pParam -> pData ||  pParam -> nDataLen >3)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 100)
    { return SC_RESULT_FAIL; }

    bottom = Z_GetCmosHeight() * nVal /100;
    top = X_paramGetGlobalAllParam()->pAreaParam->top;
    if((top < bottom)||(top == bottom))
    {    return SC_RESULT_FAIL;}
     
    X_paramGetGlobalAllParam()->pAreaParam->bottom= bottom;
    X_decoIniDecArea(X_paramGetGlobalAllParam()->pAreaParam);


    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pAreaParam->bottom = bottom;

  
    return nResult;
    //return SC_RESULT_UnSurpport;
 
}
int BottomRangeOfCentralArea_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    UIMGAreaParam uArea;
    char cBuff[16];
    unsigned int nLen; 
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pAreaParam->bottom;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pAreaParam->bottom;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&uArea, FacParamBuf+Start_MEMSize_AREAPARAM, sizeof(UIMGAreaParam));
                nVal = uArea.bottom;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        nVal *= 100;
        nVal /= Z_GetCmosHeight();

        nLen = sprintf(cBuff, "%d", nVal);
        memcpy(pParam->pOutBuf, cBuff, nLen);
        *(pParam->pOutLen) = nLen;
        
    }
    else
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-100");

    return nResult;
    //return SC_RESULT_UnSurpport;
}

//设置中心区域的左端范围	Tag：CADLEF,必须必right的值小
int LeftRangeOfCentralArea_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;
    int right,left;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        pParam -> nDigitMaxCnt = 3;
        return SC_RESULT_NEXT;  
    }

    if(!pParam -> pData ||  pParam -> nDataLen >3)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 100)
    { return SC_RESULT_FAIL; }

    left = Z_GetCmosWidth()* nVal /100;
    right = X_paramGetGlobalAllParam()->pAreaParam->right;
    if((right< left)||(right == left))
        return SC_RESULT_FAIL;
     
    X_paramGetGlobalAllParam()->pAreaParam->left= left;
    X_decoIniDecArea(X_paramGetGlobalAllParam()->pAreaParam);


    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pAreaParam->left = left;

  
    return nResult;
    //return SC_RESULT_UnSurpport;
 
}
int LeftRangeOfCentralArea_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    UIMGAreaParam uArea;
    char cBuff[16];
    unsigned int nLen;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }
    
    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pAreaParam->left;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pAreaParam->left;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&uArea, FacParamBuf+Start_MEMSize_AREAPARAM, sizeof(UIMGAreaParam));
                nVal = uArea.left;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        nVal *= 100;
        nVal /= Z_GetCmosWidth();

        nLen = sprintf(cBuff, "%d", nVal);
        memcpy(pParam->pOutBuf, cBuff, nLen);
        *(pParam->pOutLen) = nLen;
    }
    else
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-100");

    return nResult;
    //return SC_RESULT_UnSurpport;
}

//设置中心区域的右端范围	Tag：CADRIG
int RightRangeOfCentralArea_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;
    int right,left;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        pParam -> nDigitMaxCnt = 3;
        return SC_RESULT_NEXT;  
    }

    if(!pParam -> pData ||  pParam -> nDataLen >3)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 100)
    { return SC_RESULT_FAIL; }

    right = Z_GetCmosWidth()* nVal /100;
    left = X_paramGetGlobalAllParam()->pAreaParam->left;
    if((right< left)||(right == left))
        return SC_RESULT_FAIL;
     
    X_paramGetGlobalAllParam()->pAreaParam->right= right;
    X_decoIniDecArea(X_paramGetGlobalAllParam()->pAreaParam);


    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pAreaParam->right = right;

  
    return nResult;
    //return SC_RESULT_UnSurpport;
 
}
int RightRangeOfCentralArea_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    UIMGAreaParam uArea;
    char cBuff[16];
    unsigned int nLen;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pAreaParam->right;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pAreaParam->right;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&uArea, FacParamBuf+Start_MEMSize_AREAPARAM, sizeof(UIMGAreaParam));
                nVal = uArea.right;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        nVal *= 100;
        nVal /= Z_GetCmosWidth();

        nLen = sprintf(cBuff, "%d", nVal);
        memcpy(pParam->pOutBuf, cBuff, nLen);
        *(pParam->pOutLen) = nLen;
    }
    else
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-100");
    
    return nResult;
    //eturn SC_RESULT_UnSurpport;
}

/****************************************************************************
*@解码区域选择（1D产品）
****************************************************************************/
//用于选择 “离中间区域的最近的一个条码输出”，“中间区域的一个条码输出” 
//或者“固定识读2、3、4个条码”
int DecoAreaLevelsFor1D_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int DecoAreaLevelsFor1D_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@输出间隔设置（1D产品）
****************************************************************************/
//用于选择每个条码的输出固定间隔时间	Tag：CDAINV
int IntervalTimeOfOutputingFor1D_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int IntervalTimeOfOutputingFor1D_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@Image Mirror(仅模组类产品才提供)							Tag:MIRROR
****************************************************************************/
//用于选择每个条码的输出固定间隔时间	Tag：CDAINV
//const unsigned short c_ImageMirrorTab[4] = {0, eRMP_CIS_ROW_FLIP, eRMP_CIS_COL_FLIP, 
  //                                         eRMP_CIS_ROW_FLIP | eRMP_CIS_COL_FLIP};
int ImageMirrorTypes_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;
    char b[10];
    
    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 3)
    { 
        return SC_RESULT_FAIL; 
    }  

    if(nVal >1)
        return SC_RESULT_UnSurpport;
    
    if(nVal == 0) 
    {
        X_paramGetGlobalAllParam()->pHard->nProduction = 0;
        if (X_paramQueryKeyword((char*)"Mirr", b) == TRUE)
        {
            X_paramPlaceKeyword((char*)"Mirr", (char*)"off", 1);
        }
    }
    else
    {
        if(nVal == 1)
        {
            X_paramGetGlobalAllParam()->pHard->nProduction = PRODUCTION_MIRROR;
            X_paramPlaceKeyword((char*)"Mirr", (char*)"on", 1);
        }
        else
            nResult = SC_RESULT_UnSurpport;
    }
		
    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
    {
        X_paramBakGetGlobalAllParam()->pHard->nProduction= X_paramGetGlobalAllParam()->pHard->nProduction;
        paramSaveType(1);
        if(nVal == 0)
        {
            if (X_paramQueryKeyword((char*)"Mirr", b) == TRUE)
            {
                X_paramPlaceKeyword((char*)"Mirr", (char*)"off", 1);
            }
        }
        else
        {   
            if(nVal == 1)
                X_paramPlaceKeyword((char*)"Mirr", (char*)"on", 1);
            else
                nResult = SC_RESULT_UnSurpport;
        }
        paramSaveType(0);        
    }

  
    return nResult;

}
int ImageMirrorTypes_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    HardParam hHard;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }
    
    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pHard->nProduction;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pHard->nProduction;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&hHard, FacParamBuf+Start_MEMSize_HARD, sizeof(HardParam));
                nVal = hHard.nProduction;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        if(nVal == 0)
            nVal = 0x30;
        else
            nVal = 0x31;

        pParam->pOutBuf[0] = nVal;
        *(pParam->pOutLen) = 1;
    }
    else      
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-1");
    return nResult;
}


/********************************************************************************************************/
//用于KBW轮询速度的设置和查询
/*int KBWPollRate_Set(sc_param_st *pParam)
{}
int KBWPollRate_Get(sc_param_st *pParam)
{}

//用于KBW CODE PAGE设置和查询
int KBWCodePage_Set(sc_param_st *pParam)
{}
int KBWCodePage_Get(sc_param_st *pParam)
{}*/

//用于设置PS2连接PC时，从线是否连接键盘
//const unsigned short c_nPS2KBConnectTab[2] = {eCMMT_PS2_INTERNAL, 0};
int PS2KeyboardConect_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int PS2KeyboardConect_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//用于设置PS2的CLK频率
int PS2KeyboardClock_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int PS2KeyboardClock_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//启动和退出设置码设置功能
int StartExitSetup_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_SuccUnsave;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!pParam -> pData || pParam -> nDataLen > 1)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { return SC_RESULT_FAIL; }

    W_setcSetSaveFlag(FALSE);
    
    if(nVal == 0)//退出设置
    {
        if (X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff)
            return SC_RESULT_FAIL;//设置码功能未开启,提示音
        W_setcEnable(FALSE);
        W_setcSetResult(U_uIMG_STATUS_SETCODE_END);
        //printf("T1");
    }
    else//启动设置
    {
        if (X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff)
            return SC_RESULT_FAIL;
        W_setcEnable(TRUE);//W_setcIsEnable使用设置码时需要对该变量进行判断
        W_setcSetResult(U_uIMG_STATUS_SETCODE_START);//W_setcGetResult,发出提示音需要对该变量进行判断
        //printf("T0");
    }
    
    return nResult;
}
int StartExitSetup_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//开启和关闭设置码设置功能（低优先级别）
//const unsigned short c_nSetupCodeEnLoTab[2] = {0, eEDIT_EnableSettingCode};
int OpenCloseSetup_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int OpenCloseSetup_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置码识读功能（高优先级别）
//const unsigned short c_nSetupCodeEnHiTab[2] = {0, eMORERVD_EnableSettingCode};
//允许识读设置码TAG:FOBSET
int EnableSettingCode_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_SuccUnsave;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { return SC_RESULT_FAIL; }
    
    if (nVal == 1)
    {
        X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff = TRUE;
        W_setcEnable(FALSE);
        W_setcSetResult(U_uIMG_STATUS_SETCODE_END);
    }
    else 
    {
        X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff = FALSE;
    }

    if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else 
        X_paramBakGetGlobalAllParam()->pCode->IsSettingBarcodeOff = X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff;
    
    return nResult;
}
int EnableSettingCode_Get(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_SUCC;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    nVal = X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff;
    
    pParam->pOutBuf[0] = nVal+0x30;
    *(pParam->pOutLen) = 1;
    
    return nResult;
}

int QuerySettingCode_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_SUCC;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }   

    nVal = X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff;
    
    pParam->pOutBuf[0] = nVal+0x30;
    *(pParam->pOutLen) = 1;
    
    return nResult;
}

int QuerySettingCode_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}
//设置码内容发送功能,TAG:SETUP

int TrsmtSettingCode_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { return SC_RESULT_FAIL; }

    X_paramGetGlobalAllParam()->pCode->SetCode = (X_paramGetGlobalAllParam()->pCode->SetCode & (~0x01)) | nVal;

    if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else
        X_paramBakGetGlobalAllParam()->pCode->SetCode = X_paramGetGlobalAllParam()->pCode->SetCode;

    return nResult; 
}

int TrsmtSettingCode_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    CodeParam pcode;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    switch(pParam->nConfType)
    {
        case 0://RAM
            nVal = X_paramGetGlobalAllParam()->pCode->SetCode;
            break;
        case 1://SYS
            nVal = X_paramBakGetGlobalAllParam()->pCode->SetCode;
            break;
        case 2://FAC
            if(X_FacParamIsGet() != TRUE)
                return SC_RESULT_FAIL;
         
            memcpy(&pcode, FacParamBuf+Start_MEMSize_CODE, sizeof(CodeParam));
            nVal = pcode.SetCode;   
            break; 
        default:
            return SC_RESULT_UnSurpport;
            break;
    }

    pParam->pOutBuf[0] = 0x30+ (unsigned char)nVal;
    *(pParam->pOutLen) = 1;

    return nResult;
}

int SetVoiceOnOff(sc_param_st *pParam, int nVoiceType)
{
     int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!pParam -> pData)// || pParam -> nDataLen > 1)
    { return SC_RESULT_FAIL; }

    if(pParam -> nDataLen != 1)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { return SC_RESULT_FAIL; }

    switch(nVoiceType)
    {
        case DECODESUCCVOICE:         
            X_paramGetGlobalAllParam()->pHard->bDecVoice &= ~0x01;
            X_paramGetGlobalAllParam()->pHard->bDecVoice |= nVal;
            break;
        case POWNONVOICE:
            X_paramGetGlobalAllParam()->pHard->bDecVoice &= ~0x02;
            X_paramGetGlobalAllParam()->pHard->bDecVoice |= (nVal << 1);
            break;
        default:
            return SC_RESULT_UnSurpport;
            break;
    }

   
    if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else
        X_paramBakGetGlobalAllParam()->pHard->bDecVoice = X_paramGetGlobalAllParam()->pHard->bDecVoice;

    return nResult; 
}

int GetVoiceOnOff(sc_param_st *pParam, int nVoiceType)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    HardParam hHard;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    switch(pParam->nConfType)
    {
        case 0://RAM
            nVal = X_paramGetGlobalAllParam()->pHard->bDecVoice;
            break;
        case 1://SYS
            nVal = X_paramBakGetGlobalAllParam()->pHard->bDecVoice;
            break;
        case 2://FAC
            if(X_FacParamIsGet() != TRUE)
                return SC_RESULT_FAIL;
         
            memcpy(&hHard, FacParamBuf+Start_MEMSize_HARD, sizeof(HardParam));
            nVal = hHard.bDecVoice;   
            break; 
        default:
            return SC_RESULT_UnSurpport;
            break;
    }
//VoiceType:1,解码成功声音开关；2,开机声音开关
    pParam->pOutBuf[0] = (((nVal & nVoiceType)>>(nVoiceType-1))?0x31:0x30);
    *(pParam->pOutLen) = 1;

    return nResult;
}
 
//开机声音设置
//const unsigned short c_nStartBeepEnTab[2] = {0, ePROMPT_BUZ_STARTUP};
int PowerOnVoice_Set(sc_param_st *pParam)
{
    int nResult;
    nResult = SetVoiceOnOff(pParam, POWNONVOICE);
    return nResult;
}
int PowerOnVoice_Get(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SUCC;
//    HardParam hHard;
    
    nResult = GetVoiceOnOff(pParam, POWNONVOICE);

    return nResult;
}

//成功识读提示音使能设置
//const unsigned short c_nDecodeBeepEnTab[2] = {0, ePROMPT_BUZ_DECODE};
int GoodReadBeep_Set(sc_param_st *pParam)
{
    int nResult;
    nResult = SetVoiceOnOff(pParam, DECODESUCCVOICE);
    return nResult;
}
int GoodReadBeep_Get(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SUCC;
//    HardParam hHard;
    
    nResult = GetVoiceOnOff(pParam, DECODESUCCVOICE);

    return nResult;
}

//成功识读提示音音量设置
int GoodReadBeepVolume_Set(sc_param_st *pParam)
{
    int nVal, nResult = SC_RESULT_Save;

     if( !pParam )
    { return SC_RESULT_FAIL; }  

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 2)
    { return SC_RESULT_FAIL; }
//0:高,1:中,2:低
    X_paramGetGlobalAllParam()->pHard->nBeepType &= ~0xf0;
    X_paramGetGlobalAllParam()->pHard->nBeepType |= (nVal << 4);  

    if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else
    {
        X_paramBakGetGlobalAllParam()->pHard->nBeepType = X_paramGetGlobalAllParam()->pHard->nBeepType;
    }

    return nResult;
}
int GoodReadBeepVolume_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    HardParam hHard;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }
    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pHard->nBeepType;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pHard->nBeepType;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&hHard, FacParamBuf+Start_MEMSize_HARD, sizeof(HardParam));
                nVal = hHard.nBeepType;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }
    //音量:0:高，1:中,2:低
        nVal &= 0xf0;
        nVal >>= 4;
        nVal += 0x30;
        pParam->pOutBuf[0] = nVal;
        *(pParam->pOutLen) = 1;
    }
    else
    {
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-2");
    }
    return nResult;
}

//成功识读提示音音长设置
//const short c_nDecoBeepDurateParam[3] = {GRBEEPDURMIDUM, GRBEEPDURSHORT, GRBEEPDULONG};
int GoodReadBeepDuration_Set(sc_param_st *pParam)
{
    int  nVal, flag=0,nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        pParam -> nDigitMaxCnt = 3;
        return SC_RESULT_NEXT;  
    }

    if(!pParam -> pData ||  pParam -> nDataLen >3)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < GRBEEPDURMIN || nVal > GRBEEPDURMAX)
    { return SC_RESULT_FAIL; }
    
    X_paramGetGlobalAllParam()->pHard->nBeepType &= ~0xf00;
    if((nVal==80)||(nVal==40)||(nVal==120))
    {
        if(nVal==80)
            nVal = 0;
        else if(nVal ==40)
            nVal= 1;
        else
            nVal = 2;
        X_paramGetGlobalAllParam()->pHard->nBeepType |= (nVal << 8);
    }
    else
    {
        X_paramGetGlobalAllParam()->pHard->nBeepType |= (9 << 8);   //将持续时间在beeptype中的mask设为9,来标识启用自定义变量
        X_paramGetGlobalAllParam()->pCode2->nSucBeepDuration = nVal;
        flag = 1;
    }


    if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else
    {
        X_paramBakGetGlobalAllParam()->pHard->nBeepType = X_paramGetGlobalAllParam()->pHard->nBeepType;
        if(flag ==1)
            X_paramBakGetGlobalAllParam()->pCode2->nSucBeepDuration = X_paramGetGlobalAllParam()->pCode2->nSucBeepDuration;
    }
    
    return nResult;
    
}
int GoodReadBeepDuration_Get(sc_param_st *pParam)
{
    int  nVal,temp,dur=0,nResult = SC_RESULT_SUCC;
    HardParam hHard;
    CodeParam2 cCode2;
    char cBuff[16];
    unsigned int nLen;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pHard->nBeepType;
                if((nVal&0xf00) ==  0x900)
                    dur = X_paramGetGlobalAllParam()->pCode2->nSucBeepDuration;        
                break;
            
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pHard->nBeepType;
               
                if((nVal&0xf00) ==  0x900)
                    dur = X_paramBakGetGlobalAllParam()->pCode2->nSucBeepDuration;     
                  
                break;
            
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
         
                memcpy(&hHard, FacParamBuf+Start_MEMSize_HARD, sizeof(HardParam));
                nVal = hHard.nBeepType;   
                if((nVal&0xf00) ==  0x900)
                {
                    memcpy(&cCode2, FacParamBuf+Start_MEMSize_CODE2, sizeof(CodeParam2));
                    dur = cCode2.nSucBeepDuration;   
                }
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }
  
        temp = nVal & 0xf00;
        switch(temp)
        {
            case 0x000://80
                nLen = sprintf(cBuff, "%d", 80);
            break;
            
            case 0x100://40
                nLen = sprintf(cBuff, "%d", 40);
                break;
            
            case 0x200://120
                nLen = sprintf(cBuff, "%d", 120);
                break;
            
            case 0x900://自定义
            //printf("%d",nVal);
                nLen = sprintf(cBuff, "%d", dur);
                break;
            
            default:
                return SC_RESULT_FAIL;            
        }
        
         memcpy(pParam->pOutBuf, cBuff, nLen);
        *(pParam->pOutLen) = nLen;
        
    }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "20-300_Short:40|Medium:80|Long:120");
    }
    return nResult;
}



//成功识读提示音频率设置
//const unsigned short c_nDecoBeepFreqTab[4] = { 930, 2700, 3940, 4800 };
int GoodReadBeepFreq_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        pParam -> nDigitMaxCnt = 5;
        return SC_RESULT_NEXT;  
    }

    if(!pParam -> pData ||  pParam -> nDataLen >5)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < GRBEEPFREQMIN || nVal > GRBEEPFREQMAX)
    { return SC_RESULT_FAIL; }
    
    X_paramGetGlobalAllParam()->pHard->nBeepType &= ~0xf000;
    if(nVal == 800)
        nVal = 0;
    else if(nVal == 1600)
        nVal = 1;
    else if(nVal == 2730)
        nVal = 2;
    else if(nVal == 4200 )
        nVal = 3;
    else
    {//printf("%d",nVal);
        X_paramGetGlobalAllParam()->pHard->nBeepType |= (9 << 12);   //将持续时间在beeptype中的mask设为9,来标识启用自定义变量
        X_paramGetGlobalAllParam()->pCode2->nSucBeepFreq = nVal;
        return nResult;
    }

    X_paramGetGlobalAllParam()->pHard->nBeepType |= ((nVal + 1)<< 12);
    return nResult;
    
}

int GoodReadBeepFreq_Get(sc_param_st *pParam)
{
    int  nVal,temp,freq=0,nResult = SC_RESULT_SUCC;
    HardParam hHard;
    CodeParam2 cCode2;
    char cBuff[16];
    unsigned int nLen;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {    
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pHard->nBeepType;
                if((nVal&0xf000) ==  0x9000)
                    freq = X_paramGetGlobalAllParam()->pCode2->nSucBeepFreq;        
                break;
            
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pHard->nBeepType;
                if((nVal&0xf000) ==  0x9000)
                    freq = X_paramBakGetGlobalAllParam()->pCode2->nSucBeepFreq;      
                break;
            
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
         
                memcpy(&hHard, FacParamBuf+Start_MEMSize_HARD, sizeof(HardParam));
                nVal = hHard.nBeepType;   
                if((nVal&0xf000) ==  0x9000)
                {
                    memcpy(&cCode2, FacParamBuf+Start_MEMSize_CODE2, sizeof(CodeParam2));
                    freq = cCode2.nSucBeepFreq;   
                }
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        temp = ((nVal & 0xf000)>>12)-1;
 //printf("\n%d,%d\n",nVal,temp);
        switch(temp)
        {
            case 0://800
                nLen = sprintf(cBuff, "%d", 800);
            break;
            
            case 1://1600
                nLen = sprintf(cBuff, "%d", 1600);
                break;
            
            case 2://2730
                nLen = sprintf(cBuff, "%d", 2730);
                break;
            
            case 3://4200
                nLen = sprintf(cBuff, "%d", 4200);
                break;
            
            case 8://自定义
                nLen = sprintf(cBuff, "%d", freq);
                break;
            
            default:

                return SC_RESULT_FAIL;
            
        }
        memcpy(pParam->pOutBuf, cBuff, nLen);
        *(pParam->pOutLen) = nLen;
    }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "20-20000_Lowest:800|Low:1600|Medium:2730|High:4200");
    }    

   return nResult;
}
//成功提示音的次数设置
int GoodReadBeepNumber_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GoodReadBeepNumber_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//成功提示音的间隔时间设置
int GoodReadBeepIntval_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GoodReadBeepIntval_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//失败提示音的使能设置
int BadReadBeepEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//失败识读提示音音量设置
int BadReadBeepVolume_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepVolume_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//失败识读提示音音长设置
int BadReadBeepDuration_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepDuration_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//失败识读提示音频率设置
int BadReadBeepFreq_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepFreq_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//失败提示音的次数设置
int BadReadBeepNumber_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepNumber_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//失败提示音的间隔时间设置
int BadReadBeepIntval_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepIntval_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//语音播放功能使能设置
//const unsigned char c_nVoiceEnableTab[2] = {eVOICE_DISABLE, 0};
int VoiceEnable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int VoiceEnable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//语音播放间隔时间设置
int VoiceIntvalTime_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int VoiceIntvalTime_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//语音类型设置   CMD:VOICET
int VoiceTypeSelect_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int VoiceTypeSelect_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//语音模式设置   CMD:VOICEM
int VoiceModeSelect_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int VoiceModeSelect_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//语音音量设置   CMD:VOICEV
int VoiceVolume_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int VoiceVolume_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//成功识读灯使能设置
//const unsigned char c_nDecodeLedEnTab[2] = {0, ePROMPT_LED_DECODE};
int GoodReadLedEnable_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }  

    if (nVal == 1)	//开启
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLED &= ~ 1;
    }
    else if (nVal == 0)	//关闭
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLED |= 1;
    }

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pHard->nGoodreadLED= X_paramGetGlobalAllParam()->pHard->nGoodreadLED;

  
    return nResult;

}
int GoodReadLedEnable_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    HardParam hHard;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    switch(pParam->nConfType)
    {
        case 0://RAM
            nVal = X_paramGetGlobalAllParam()->pHard->nGoodreadLED;
            break;
        case 1://SYS
            nVal = X_paramBakGetGlobalAllParam()->pHard->nGoodreadLED;
            break;
        case 2://FAC
            if(X_FacParamIsGet() != TRUE)
                return SC_RESULT_FAIL;
         
            memcpy(&hHard, FacParamBuf+Start_MEMSize_HARD, sizeof(HardParam));
            nVal = hHard.nGoodreadLED;   
            break; 
        default:
            return SC_RESULT_UnSurpport;
            break;
    }
//音量:0:高，1:中,2:低
    nVal &= 0x1;
    if(nVal == 1)
        nVal = 0x30;
    else
        nVal = 0x31;
    pParam->pOutBuf[0] = nVal;
    *(pParam->pOutLen) = 1;

    return nResult;
}

//成功识读灯持续时间设置
//const short c_nDecoLedDurateParam[3] = {1, 100, 100};
int GoodReadLedDuration_Set(sc_param_st *pParam)
{
    int  nVal, temp=0xff,nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

     if(!(pParam -> nDataLen))
    {
        pParam -> nDigitMaxCnt = 5;
        return SC_RESULT_NEXT;  
    }

    if(!pParam -> pData || pParam -> nDataLen > 5)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < GRLEDDURMIN|| nVal > GRLEDDURMAX)
    { 
        return SC_RESULT_FAIL; 
    }  


    if (nVal == 20)//短
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 0;
    }
    else if (nVal == 120)//中
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 1;				
    }
    else if (nVal == 220)//长
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 2;
    }
    else if (nVal == 320)//最长
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 3;
    }
    else 
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 4;
        temp = 4;
        X_paramGetGlobalAllParam()->pCode2->nSucLedDuration = nVal;
    }

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
    {    
        X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration = X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration;
        if(temp == 4)
            X_paramBakGetGlobalAllParam()->pCode2->nSucLedDuration = X_paramGetGlobalAllParam()->pCode2->nSucLedDuration;
    }
    
    return nResult;
}
int GoodReadLedDuration_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    HardParam hHard;
    CodeParam2 cCode2;
    int nLedDur=0;
    char cBuff[16];
    unsigned int nLen;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {    
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration;
                if(nVal == 4)
                {
                    nLedDur = X_paramGetGlobalAllParam()->pCode2->nSucLedDuration;   
                }
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration;
                if(nVal == 4)
                {
                    nLedDur = X_paramGetGlobalAllParam()->pCode2->nSucLedDuration;   
                }
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
         
                memcpy(&hHard, FacParamBuf+Start_MEMSize_HARD, sizeof(HardParam));
                nVal = hHard.nGoodreadLEDDuration;   
                if(nVal == 4)
                {
                    memcpy(&cCode2, FacParamBuf+Start_MEMSize_CODE2, sizeof(CodeParam2));
                    nLedDur = cCode2.nSucLedDuration;   
                }
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }
        if(nVal < 4)
            nLedDur = 20+nVal*100;

        nLen = sprintf(cBuff, "%d", nLedDur);
        memcpy(pParam->pOutBuf, cBuff, nLen);
        *(pParam->pOutLen) = nLen;
    
    }
    else
    {
        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "1-2500_Short:20|Medium:120|Long:220|Prolonged:320");        

    }

    return nResult;
}

//成功识读震动使能设置
int GoodReadVibrateEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GoodReadVibrateEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//成功识读震动持续时间设置
int GoodReadVibrateDuration_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GoodReadVibrateDuration_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//开机提示灯使能设置:POLENA
//const unsigned short c_nPowerOnLedTab[2] = {0, ePROMPT_LED_POWER};
int PowerOnLedEnable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int PowerOnLedEnable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//工作模式选择设置
//const unsigned char c_nWorkingModeTab[8] = {eMODE_TRIGGER, eMODE_TRIGGER, eMODE_AUTOSENS,
 //                                           eMODE_CONTINIOUS, eMODE_TRIGGER, eMODE_TRIGGER,
 //                                           eMODE_TRIGGER, eMODE_TRIG_BATCH};
int WorkMode_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 7)
    { return SC_RESULT_FAIL; }

    switch(nVal)
    {
        case 0://手动
            X_paramGetGlobalAllParam()->pCode->nReadMode = ESC0;
            X_paramGetGlobalAllParam()->pMode->bContinue = TRUE;
            break;
        case 2://感应
            X_paramGetGlobalAllParam()->pCode->nReadMode = ESC2;
            X_paramGetGlobalAllParam()->pMode->bContinue = TRUE;
            X_paramGetGlobalAllParam()->pMode->nReadDelay = Z_Get10msCounter();
            X_paramGetGlobalAllParam()->pMode->nHaltDelay = Z_Get10msCounter();
            X_paramGetGlobalAllParam()->pMode->nCodeDelay = Z_Get10msCounter();
            break;
        case 3://连续
            X_paramGetGlobalAllParam()->pMode->bContinue = FALSE;
            X_paramGetGlobalAllParam()->pCode->nReadMode = ESC3;
            break;

       default:
            nResult = SC_RESULT_UnSurpport;
          

    }
  
    if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else
    {
        X_paramBakGetGlobalAllParam()->pCode->nReadMode = X_paramGetGlobalAllParam()->pCode->nReadMode;
        X_paramBakGetGlobalAllParam()->pMode->bContinue = X_paramGetGlobalAllParam()->pMode->bContinue;
    }
    
    return nResult;
}
int WorkMode_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    CodeParam pcode;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pCode->nReadMode;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pCode->nReadMode;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&pcode, FacParamBuf+Start_MEMSize_CODE, sizeof(CodeParam));
                nVal = pcode.nReadMode;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }
        pParam->pOutBuf[0] = 0x30+(unsigned char)nVal;
        *(pParam->pOutLen) = 1;

    }
    else
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0|2-3");
    
    return nResult;
    
}

//软件模拟触发设置
int PureSoftTrig_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int PureSoftTrig_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//读码间歇时长设置
int RestTime_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int RestTime_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//感应模式下，等待图像稳定的时间设置
//const short c_nImgStableTmParam[3] = {1, 100, 100};
int ImgStabTimeout_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}
int ImgStabTimeout_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}

//读码时长设置
int ReadTime_Set(sc_param_st *pParam)
{    
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        pParam -> nDigitMaxCnt = 7;
        return SC_RESULT_NEXT;  
    }

    if(!pParam -> pData ||  pParam -> nDataLen >7)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < ONEREADTIMEOUTMIN|| nVal > ONEREADTIMEOUTMAX)
    { return SC_RESULT_FAIL; }
    
    X_paramGetGlobalAllParam()->pCode->nOnceDelay = nVal;

     if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else
    {
        X_paramBakGetGlobalAllParam()->pCode->nOnceDelay = X_paramGetGlobalAllParam()->pCode->nOnceDelay ;
    }
    
    return nResult;

}
int ReadTime_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    CodeParam cCode;
    char cBuff[16];
    unsigned int nLen;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pCode->nOnceDelay;   
                break;
            
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pCode->nOnceDelay; 
                break;
            
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
         
                memcpy(&cCode, FacParamBuf+Start_MEMSize_CODE, sizeof(CodeParam));
                nVal = cCode.nOnceDelay;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        nLen = sprintf(cBuff, "%d", nVal);
        memcpy(pParam->pOutBuf, cBuff, nLen);
        *(pParam->pOutLen) = nLen;
    }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "ORTSET0-3600000");
    }
    return nResult;
}


//相同条码延时开关设置
//const unsigned short c_nReReadDlyEnTab[2] = {0, eRMP_DELAY_COMPELETE};
int RereadDelayEnable_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> pData || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }  

    X_paramGetGlobalAllParam()->pCode->IsCompDelay = nVal;

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pCode->IsCompDelay = X_paramGetGlobalAllParam()->pCode->IsCompDelay;

    return nResult;

}
int RereadDelayEnable_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    CodeParam cCode;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    switch(pParam->nConfType)
    {
        case 0://RAM
            nVal = X_paramGetGlobalAllParam()->pCode->IsCompDelay;
            break;
        case 1://SYS
            nVal = X_paramBakGetGlobalAllParam()->pCode->IsCompDelay;
            break;
        case 2://FAC
            if(X_FacParamIsGet() != TRUE)
                return SC_RESULT_FAIL;
         
            memcpy(&cCode, FacParamBuf+Start_MEMSize_CODE, sizeof(CodeParam));
            nVal = cCode.IsCompDelay;   
            break; 
        default:
            return SC_RESULT_UnSurpport;
            break;
    }
//音量:0:高，1:中,2:低
    pParam->pOutBuf[0] = nVal+0x30;
    *(pParam->pOutLen) = 1;

    return nResult;
}


//相同条码延时时间设置
int RereadDelayTime_Set(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        pParam -> nDigitMaxCnt = 3;
        return SC_RESULT_NEXT;  
    }

    if(!pParam -> pData ||  pParam -> nDataLen >7)
    { return SC_RESULT_FAIL; }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { return SC_RESULT_FAIL; }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < REREADDELAYTIMEMIX || nVal > REREADDELAYTIMEMAX)
    { return SC_RESULT_FAIL; }

    X_paramGetGlobalAllParam()->pCode->nInfoDelay = nVal;
    
    if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else
    {
        X_paramBakGetGlobalAllParam()->pCode->nInfoDelay = X_paramGetGlobalAllParam()->pCode->nInfoDelay;
    }
    
    return nResult;
    
}
int RereadDelayTime_Get(sc_param_st *pParam)
{  
    int  nVal,nResult = SC_RESULT_SUCC;
    CodeParam cCode;
    char cBuff[16];
    unsigned int nLen;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pCode->nInfoDelay;   
                break;
            
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pCode->nInfoDelay;
                break;
            
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
         
                memcpy(&cCode, FacParamBuf+Start_MEMSize_CODE, sizeof(CodeParam));
                nVal = cCode.nInfoDelay;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }
        
        nLen = sprintf(cBuff, "%d", nVal);
        memcpy(pParam->pOutBuf, cBuff, nLen);
        *(pParam->pOutLen) = nLen;
       
    }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "1-3600000");
    }
    return nResult;}

//间歇模式下的读码时间设置
int FlashOnTime_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int FlashOnTime_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//间歇模式下的间歇时间设置
int FlashOffTime_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int FlashOffTime_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//感应灵敏度设置
//const short c_nSensitivityParam[3] = {1, 1, 1};
int Sensitivity_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> pData || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 2)
    { 
        return SC_RESULT_FAIL; 
    }  

    if(nVal > 0)
        return SC_RESULT_UnSurpport;

   else
        X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = 0;

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode = 0;
  
    return SC_RESULT_UnSurpport;

}
int Sensitivity_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//Not Good Read使能设置
//const unsigned short c_nNotGoodReadEnTab[2] = {0, eEDIT_EnableNG};
int NotGoodReadEnable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int NotGoodReadEnable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//Not Good Read 信息设置
int NotGoodReadMsg_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int NotGoodReadMsg_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int NotGoodReadLenth_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int NotGoodReadLenth_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//关机模式
//const unsigned int c_nModePowerOffTab[1] = {eAPP_MODE_SHUTDOWN};
int ModePowerOff_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int ModePowerOff_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
//Idle模式
//const unsigned int c_nModeIdleTab[1] = {eAPP_MODE_IDLE};
int ModeIdle_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int ModeIdle_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
//Sleep模式
//const unsigned int c_nModeSleepTab[1] = {eAPP_MODE_SLEEP};
int ModeSleep_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int ModeSleep_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
//test模式
int ModeTest_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ModeTest_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//Reboot模式
int ModeReboot_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int ModeReboot_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//自动休眠功能选择
//const unsigned short c_nAutoSleepEnTab[2] = {0, eRMP_AUTOSLEEP};
int AutoSleepEnable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AutoSleepEnable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//自动休眠空闲时间设置
int AutoSleepIdleTime_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AutoSleepIdleTime_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//安全级别设置
int SafeLv_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int SafeLv_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/*********************************************************************************************/
//////////////////////////////////////////////////////////////////////////
//查询系统信息QRYSYS
int SysInfo_Set(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SUCC;
    char buff[NLSCOMM_OUTBUF_SIZE]={0};
    int nSize;
    char lvmid[512];

    if( !pParam )
        { return SC_RESULT_FAIL; }

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }

    if(X_paramQueryKeyword("ManufactureID", lvmid))
    {
		//found manufacture ID
		sprintf(buff+strlen(buff),"Product Name: %s\r\n",lvmid);
    }
    else
        sprintf(buff+strlen(buff),"Product Name: %s\r\n",get_bsp_string());
    sprintf(buff+strlen(buff),"Firmware Version: %s\r\n",KERNEL_VER_STRING);
    sprintf(buff+strlen(buff),"Decoder Version: %s\r\n",UIMAGE_PRI_VERSION);
    sprintf(buff+strlen(buff),"Hardware Version: 1.0\r\n");//////////无可查询的硬件版本

    nSize= strlen((char*)X_paramGetGlobalAllParam()->pDevInfo->pSN);
    if (nSize > 0)
        sprintf(buff+strlen(buff),"Serial Number: %s\r\n",X_paramGetGlobalAllParam()->pDevInfo->pSN);
    else 
        sprintf(buff+strlen(buff),"Serial Number: \r\n");
    
    nSize = strlen((char*)X_paramGetGlobalAllParam()->pDevInfo->pESN);
    if (nSize > 0)
        sprintf(buff+strlen(buff),"OEM Serial Number: %s\r\n",X_paramGetGlobalAllParam()->pDevInfo->pESN);
    else 
        sprintf(buff+strlen(buff),"OEM Serial Number: \r\n");

    nSize = strlen((char*)X_paramGetGlobalAllParam()->pDevInfo->pDate);
    if (nSize > 0)
        sprintf(buff+strlen(buff),"Manufacturing Date: %s\r\n",X_paramGetGlobalAllParam()->pDevInfo->pDate);
    else 
        sprintf(buff+strlen(buff),"Manufacturing Date: \r\n");

    nSize = strlen(buff);
    if(nSize < pParam->nOutBufSize)
    {    
        memcpy(pParam->pOutBuf, buff, strlen(buff));
        *(pParam->pOutLen) = nSize;
    }
    else
        nResult = SC_RESULT_FAIL;

    //Y_commWrite(pParam->pOutBuf, strlen(buff));
     return nResult;
}

int SysInfo_Get(sc_param_st *pParam)
{
     return SC_RESULT_SUCC;
}
//查询固件版本号
int FirmwareVer_Set(sc_param_st *pParam)
{
    int nResult = SC_RESULT_SUCC;
    char buff[64]={0};
    int nSize;

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }
        
    nSize = sprintf(buff,"Product Name: %s\r\n",get_bsp_string());

    //nSize = strlen(buff);
    if(nSize < pParam->nOutBufSize)
    {
        memcpy(pParam->pOutBuf, buff, nSize);
        *(pParam->pOutLen) = nSize;
    }
    else
        nResult = SC_RESULT_FAIL;

    return nResult;
}

int FirmwareVer_Get(sc_param_st *pParam)
{    
     return SC_RESULT_SUCC;
}
//查询解码版本号
int DecoderVer_Set(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SUCC;
    char buff[NLSCOMM_OUTBUF_SIZE]={0};
    int nSize;
    
    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }

    nSize = sprintf(buff,"Decoder Version: %s\r\n",UIMAGE_PRI_VERSION);
    
   // nSize = strlen(buff);
    if(nSize < pParam->nOutBufSize)
    {
        memcpy(pParam->pOutBuf, buff, nSize);
        *(pParam->pOutLen) = nSize;
    }
    else
        nResult = SC_RESULT_FAIL;
    
     return nResult;
}


int DecoderVer_Get(sc_param_st *pParam)
{
     return SC_RESULT_SUCC;
}
//查询硬件信息
int HardwareVer_Set(sc_param_st *pParam)
{
    int nResult = SC_RESULT_SUCC;
    char buff[64]={0};
    int nSize;

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }

    nSize = sprintf(buff,"Hardware Version: 1.0\r\n");//////////无可查询的硬件版本

    //nSize = strlen(buff);
    if(nSize < pParam->nOutBufSize)
    {
        memcpy(pParam->pOutBuf, buff, nSize);
        *(pParam->pOutLen) = nSize;
    }
    else
        nResult = SC_RESULT_FAIL;

    return nResult;
}

int HardwareVer_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}
//查询产品名称
int ProductName_Set(sc_param_st *pParam)
{
    int nResult = SC_RESULT_SUCC;
    char buff[64]={0};
    int nSize;
    char lvmid[512];

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }

    if(X_paramQueryKeyword("ManufactureID", lvmid))
    {
		//found manufacture ID
		nSize = sprintf(buff,"Product Name: %s\r\n",lvmid);
    }
    else
        nSize = sprintf(buff,"Product Name: %s\r\n",get_bsp_string());

   // nSize = strlen(buff);
    if(nSize < pParam->nOutBufSize)
    {
        memcpy(pParam->pOutBuf, buff, nSize);
        *(pParam->pOutLen) = nSize;
    }
    else
        nResult = SC_RESULT_FAIL;

    return nResult;
}

int ProductName_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}
//查询产品序列号
int ProductPID_Set(sc_param_st *pParam)
{
    int nResult = SC_RESULT_UnsaveAndUpdateAll;
    char buff[64]={0};
    int nSize;

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }
    
    //nSize= strlen((char*)X_paramGetGlobalAllParam()->pDevInfo->pSN);
    //if (nSize > 0)
        nSize = sprintf(buff,"Serial Number: %s\r\n",X_paramGetGlobalAllParam()->pDevInfo->pSN);
    //else 
        //sprintf(buff+strlen(buff),"Serial Number:\r\n");

   // nSize = strlen(buff);
    if(nSize < pParam->nOutBufSize)
    {
        memcpy(pParam->pOutBuf, buff, nSize);
        *pParam->pOutLen = nSize;
    }
    else
        nResult = SC_RESULT_FAIL;

    return nResult;
}

int ProductPID_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}
//查询产品生产日期
int ProduceDate_Set(sc_param_st *pParam)
{
    int nResult = SC_RESULT_SUCC;
    char buff[64]={0};
    int nSize;

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }
    
    //nSize = strlen((char*)X_paramGetGlobalAllParam()->pDevInfo->pDate);
    //if (nSize > 0)
        nSize = sprintf(buff+strlen(buff),"Manufacturing Date: %s\r\n",X_paramGetGlobalAllParam()->pDevInfo->pDate);
    //else 
       //sprintf(buff+strlen(buff),"Manufacturing Date: Null\r\n");

    //nSize = strlen(buff);
    if(nSize < pParam->nOutBufSize)
    {     
        memcpy(pParam->pOutBuf, buff, nSize);
        *pParam->pOutLen = nSize;
    }
    else
        nResult = SC_RESULT_FAIL;

    return nResult;
}

//查询产品生产日期
int ProduceDate_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}

//查询产品OEM型号
int ProductOEMType_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int ProductOEMType_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//查询产品OEM序列号（ESN）
int ProductOEMESN_Set(sc_param_st *pParam)
{
    int nResult = SC_RESULT_UnsaveAndUpdateAll;
    char buff[64]={0};
    int nSize;

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }
    //nSize = strlen((char*)X_paramGetGlobalAllParam()->pDevInfo->pESN);
   // if (nSize > 0)
        nSize = sprintf(buff+strlen(buff),"OEM Serial Number: %s\r\n",X_paramGetGlobalAllParam()->pDevInfo->pESN);
    //else 
        //sprintf(buff+strlen(buff),"OEM Serial Number: Null\r\n");

    //nSize = strlen(buff);
    if(nSize < pParam->nOutBufSize)
    {
        memcpy(pParam->pOutBuf, buff, nSize);
        *pParam->pOutLen = nSize;
    }
    else
        nResult = SC_RESULT_FAIL;

    return nResult;
}

int ProductOEMESN_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}

//查询产品OEM生产日期
int ProduceOEMDate_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ProduceOEMDate_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//所有码制使能设置
int AllCodeTypeEnable_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }  

    if (nVal == 1)	//使能所有条码
    {
        X_paramGetGlobalAllParam()->p1D->unDecTypes = 0xffffffff;
        X_paramGetGlobalAllParam()->p2D->unDecTypes = 0xffffffff;
        X_paramGetGlobalAllParam()->pPostal->unDecTypes = 0xffffffff;
        X_paramGetGlobalAllParam()->pOCR->unDecTypes = 0xffffffff;
    }
    else //禁止所有条码
    {
        X_paramGetGlobalAllParam()->p1D->unDecTypes = 0x03; //只识别设置码
        X_paramGetGlobalAllParam()->p2D->unDecTypes = 0;
        X_paramGetGlobalAllParam()->pPostal->unDecTypes = 0;
        X_paramGetGlobalAllParam()->pOCR->unDecTypes = 0;
    }

    X_paramUpdateToDevice();
    if(!(pParam->bSaveFlag))//不保存
    {
        nResult = SC_RESULT_SuccUnsave;
    }
    else//保存
    {
        X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
        X_paramBakGetGlobalAllParam()->p2D->unDecTypes = X_paramGetGlobalAllParam()->p2D->unDecTypes;
        X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = X_paramGetGlobalAllParam()->pPostal->unDecTypes;
        X_paramBakGetGlobalAllParam()->pOCR->unDecTypes = X_paramGetGlobalAllParam()->pOCR->unDecTypes;
    }    
 
    return nResult;
}

int AllCodeTypeEnable_Get(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SUCC;
   
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nQueryType)
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-1");
    }

    return nResult;
}

// 使能/禁止所有一维条码
int CodeType1DEnable_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }  
        
    if (nVal == 1)	//使能所有一维条码
    {
        X_paramGetGlobalAllParam()->p1D->unDecTypes = 0xffffffff;
    }
    else//禁止所有一维条码
    {
        X_paramGetGlobalAllParam()->p1D->unDecTypes = 0x03; //只识别设置码:中安和setup128
    }

    X_paramUpdateToDevice();
    
    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;

  
    return nResult;


}
int CodeType1DEnable_Get(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SUCC;
   
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nQueryType)
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-1");
    }
   
    return nResult;
}

//所有二维条码使能设置	Tag:ALL2DC
int CodeType2DEnable_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }  
        
    if (nVal == 1)	//使能所有一维条码
    {
        X_paramGetGlobalAllParam()->p2D->unDecTypes = 0xffffffff;
    }
    else//禁止所有二维条码
    {
        X_paramGetGlobalAllParam()->p2D->unDecTypes = 0;
    }

    X_paramUpdateToDevice();

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->p2D->unDecTypes = X_paramGetGlobalAllParam()->p2D->unDecTypes;
  
    return nResult;
}
int CodeType2DEnable_Get(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SUCC;
   
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nQueryType)
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-1");
    }

    return nResult;
}

//所有邮政条码使能设置	Tag:ALLPST
int CodeTypePostCodeEnable_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }  
        
    if (nVal == 1)	//使能所有一维条码
    {
        X_paramGetGlobalAllParam()->pPostal->unDecTypes = 0xffffffff;
    }
    else//禁止所有二维条码
    {
        X_paramGetGlobalAllParam()->pPostal->unDecTypes = 0;
    }

    X_paramUpdateToDevice();
    
    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = X_paramGetGlobalAllParam()->pPostal->unDecTypes;
  
    return nResult;
}
int CodeTypePostCodeEnable_Get(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SUCC;
   
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nQueryType)
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-1");
    }

    return nResult;
}

int DualCodeTypesFor1D_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 2)
    { 
        return SC_RESULT_FAIL; 
    }  

    switch(nVal)
    {
        case 0://仅读单一维码
            X_paramGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
            break;
        case 1://可读单一维码，也可读双一维码
            X_paramGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
            X_paramGetGlobalAllParam()->p1D->unDecParam |= 0x02; 
            break;
        case 2://仅读双一维码
            X_paramGetGlobalAllParam()->p1D->unDecParam |= 0x03;
            break;

        default:
            nResult = SC_RESULT_UnSurpport;
            break;
    }
    X_paramUpdateToDevice();
    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->p1D->unDecParam = X_paramGetGlobalAllParam()->p1D->unDecParam;
  
    return nResult;
}
int DualCodeTypesFor1D_Get(sc_param_st *pParam)
{ 
    int  nVal,nResult = SC_RESULT_SUCC;
    UTDec1DSetup ut1dSetup;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->p1D->unDecParam;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->p1D->unDecParam;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&ut1dSetup, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
                nVal = ut1dSetup.unDecParam;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        nVal &= 0x3;
        if(nVal != 0)
            nVal -= 1;
        nVal += 0x30;
        pParam->pOutBuf[0] = nVal;
        *(pParam->pOutLen) = 1;
    }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-2");
    }
    return nResult;

}

//识读反相条码使能设置	Tag:ALLINV
int InverterOfCodesEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int InverterOfCodesEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }


//所有EANUPC附加码设置
int AllEanUpcAddon_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int AllEanUpcAddon_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }




/****************************************************************************
*@普通条码
****************************************************************************/
//加载条码默认设置
/*int __1DCodeLoadDefault_Set(sc_param_st *pParam, unsigned int nCodeType)
{
	int  nVal, nResult = SC_RESULT_SaveAndUpdateAll;
	int  nOrder;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
      if(pParam->nDataLen != 0)
      {
		return SC_RESULT_FAIL;
	}
//printf("l=%d\n",pParam->nDataLen);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = Init_CODE128;
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODE128);

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
	
}*/

//开关设置、读取
int __1DCodeEnable_Set(sc_param_st *pParam,unsigned short nCodeType)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->pData)// || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

       if(pParam->nDataLen != 1)
       {
		return SC_RESULT_FAIL;
	 }
	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (nVal == 0)//禁止
	{
		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << nCodeType));
	}
	else//使能
	{
		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << nCodeType);
	}
	
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult =SC_RESULT_SuccUnsave; //SC_RESULT_UnsaveAndUpdateAll;
	}
	else
	{
	      if (nVal == 0)//禁止
		    X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << nCodeType));
            else
                X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << nCodeType);
	}
   
    
	return nResult;
}

//extern BYTE FacParamBuf[FLASH_BLOCK_SIZE];
int __1DCodeEnable_Get(sc_param_st *pParam,unsigned short nCodeType)
{
	int  nLen, nResult = SC_RESULT_SUCC;
      unsigned int nVal;
	UTDec1DSetup p1d;
	char cBuff[8];

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	if (pParam->nQueryType)
	{
		nLen = sprintf(cBuff, "%d-%d", 0,1);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
		return nResult;
	}
      else
      {
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS:	//RAM
        		nVal = X_paramGetGlobalAllParam()->p1D->unDecTypes & (0x01 << nCodeType);
               	break;

        	case SC_CFG_SYS:	//SYS
        		nVal = X_paramBakGetGlobalAllParam()->p1D->unDecTypes & (0x01 << nCodeType);
                	break;

        	case SC_CFG_FACT:	//FAC
        		if (X_FacParamIsGet() != TRUE)
        		return SC_RESULT_FAIL;

        		memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
        		nVal =p1d.unDecTypes &  (0x01 << nCodeType);
        	
        		break;
        	default:
        		return SC_RESULT_UnSurpport;
        		break;
        	}
        	
        	nLen = sprintf(cBuff, "%d", nVal >> nCodeType);
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
       }
	
	return nResult;

}

//取值设置、读取 __BarcodeParamValue_Set
int __1DCodeParamValue_Set(sc_param_st *pParam,
							unsigned short nCodeType,
							unsigned int nDataMin,			//协议{Data}的最小值
							unsigned int nDataMax,			//协议{Data}的最大值
							unsigned int nSetBitMsk,		//对应功能的设置位标识
							void		*pSetTal			//依据指令顺序排列的功能设置值
							)
{
	int  nVal, nResult = SC_RESULT_Save;

#if _ENABLE_EXTFUNC_DBG_
	char Buff[16];
#endif
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < nDataMin || nVal > nDataMax)
	{
		return SC_RESULT_FAIL;
	}

#if _ENABLE_EXTFUNC_DBG_
	sprintf(Buff,"\r\nSv,ST:%x,%x\r\n", nVal, *((unsigned int *)pSetTal + nVal));
	Y_commSendString(Buff);
#endif

	X_paramGetGlobalAllParam()->p1D->ExMsg[nCodeType] &= (~nSetBitMsk);
	X_paramGetGlobalAllParam()->p1D->ExMsg[nCodeType] |= *((unsigned int *)pSetTal+ nVal);

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[nCodeType] = X_paramGetGlobalAllParam()->p1D->ExMsg[nCodeType];
	}

	return nResult;

}

int __1DCodeParamValue_Get(sc_param_st *pParam,
							unsigned short nCodeType,
							unsigned int nDataMin,			//协议{Data}的最小值
							unsigned int nDataMax,			//协议{Data}的最大值
							unsigned int nSetBitMsk,		//对应功能的设置位标识
							void		*pSetTal			//依据指令顺序排列的功能设置值
							)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	int	 nIndex = 0;
	char cBuff[8];
	UTDec1DSetup p1d;

#if _ENABLE_EXTFUNC_DBG_
	char Buff[16];
#endif 

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	if (pParam->nQueryType)
	{
		nLen = sprintf(cBuff, "%d-%d", nDataMin, nDataMax);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
		return nResult;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS://RAM
		nVal = X_paramGetGlobalAllParam()->p1D->ExMsg[nCodeType] & nSetBitMsk;
		break;

	case SC_CFG_SYS://SYS
		nVal = X_paramBakGetGlobalAllParam()->p1D->ExMsg[nCodeType] & nSetBitMsk;
		break;

	case SC_CFG_FACT://FAC
		if (X_FacParamIsGet() != TRUE)
		{
			return SC_RESULT_FAIL;
		}
		memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
		nVal = p1d.ExMsg[nCodeType] & nSetBitMsk;
		break;

	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	

	for (nIndex = nDataMin; nIndex < nDataMax; nIndex++)
	{
		if (nVal == ((unsigned int *)pSetTal)[nIndex])
		{
			break;
		}
	}

	if (nVal != ((unsigned int *)pSetTal)[nIndex])
	{
		return SC_RESULT_FAIL;
	}

	nLen = sprintf(cBuff, "%d", nIndex);

	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//最大最小长度设置、读取
int __1DCodeMinLen_Set(sc_param_st *pParam,
                      unsigned short nCodeType)
{
	int  nVal,nResult = SC_RESULT_SaveAndUpdateAll;
      unsigned int nMax;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = 2;
		return SC_RESULT_NEXT;
	}

	if (!pParam->pData )
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 127)
	{
		return SC_RESULT_FAIL;
	}
	switch (nCodeType) {
	case POS_CODE128:
            nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_UCCEAN128:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_REGULAR39:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_INTER2OF5:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_MATRIX25:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_MODULE25:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_CODABAR:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_CODE93:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_CODEZ:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_CODE11:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_INDUSTRIAL25:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_STANDARD25:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_PLESSEY:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_MSI_PLESSEY:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] & 0xff00ffff) | (nVal << 16);
		break;
	case POS_CHNPOST:
             nMax = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] & 0xff000000) >> 24;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] & 0xff00ffff) | (nVal << 16);
		break;
	default:
		return SC_RESULT_FAIL;
		break;
	}

	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[nCodeType] = X_paramGetGlobalAllParam()->p1D->ExMsg[nCodeType];
	}

	return nResult;
}

int __1DCodeMinLen_Get(sc_param_st *pParam,unsigned short nCodeType)
{
	int  nVal,nLen,nResult = SC_RESULT_SUCC;
	int  nLowThre = 0;
	int  nHigThre = 0;
	char cBuff[8];
	UTDec1DSetup p1d;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (nCodeType) {
	case POS_CODE128:
		if (pParam->nQueryType)
		{
			nLowThre = 1;
			nHigThre = 48;
			
		}
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_CODE128] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}	
		}
		break;

	case POS_UCCEAN128:
		if (pParam->nQueryType)
		{
			nLowThre = 1;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_UCCEAN128] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		break;

	case POS_REGULAR39:

		if (pParam->nQueryType)
		{
			nLowThre = 1;
			nHigThre = 48;
		}
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_REGULAR39] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		break;
	
	case POS_INTER2OF5:
		if (pParam->nQueryType)
		{
			nLowThre = 6;
			nHigThre = 80;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_INTER2OF5] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_MATRIX25:
		if (pParam->nQueryType)
		{
			nLowThre = 4;
			nHigThre = 80;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_MATRIX25] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_MODULE25:
		if (pParam->nConfType == SC_CFG_NSYS)
		{
			nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] & ~0xff00ffff) >> 16;
		}
		else if (pParam->nConfType == SC_CFG_SYS)
		{
			nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] & ~0xff00ffff) >> 16;
		}
		else if (pParam->nConfType == SC_CFG_FACT)
		{
			if (X_FacParamIsGet() != TRUE)
			{
				return SC_RESULT_FAIL;
			}
			memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
			nVal = (p1d.ExMsg[POS_MODULE25] & ~0xff00ffff) >> 16;

		}
		else
		{
			return SC_RESULT_FAIL;
		}
		break;

	case POS_CODABAR:
		if (pParam->nQueryType)
		{
			nLowThre = 2;
			nHigThre = 60;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_CODABAR] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_CODE93:
		if (pParam->nQueryType)
		{
			nLowThre = 1;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_CODE93] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_CODEZ:
		if (pParam->nConfType == SC_CFG_NSYS)
		{
			nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] & ~0xff00ffff) >> 16;
		}
		else if (pParam->nConfType == SC_CFG_SYS)
		{
			nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] & ~0xff00ffff) >> 16;
		}
		else if (pParam->nConfType == SC_CFG_FACT)
		{
			if (X_FacParamIsGet() != TRUE)
			{
				return SC_RESULT_FAIL;
			}
			memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
			nVal = (p1d.ExMsg[POS_CODEZ] & ~0xff00ffff) >> 16;

		}
		else
		{
			return SC_RESULT_FAIL;
		}
		break;

	case POS_CODE11:

		if (pParam->nQueryType)
		{
			nLowThre = 4;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_CODE11] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_INDUSTRIAL25:
		
		if (pParam->nQueryType)
		{
			nLowThre = 6;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_INDUSTRIAL25] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;
	case POS_STANDARD25:

		if (pParam->nQueryType)
		{
			nLowThre = 6;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_STANDARD25] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_PLESSEY:

		if (pParam->nQueryType)
		{
			nLowThre = 4;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_PLESSEY] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_MSI_PLESSEY:
		if (pParam->nQueryType)
		{
			nLowThre = 4;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_MSI_PLESSEY] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;
	case POS_CHNPOST:
		if (pParam->nQueryType)
		{
			nLowThre = 1;
			nHigThre = 48;
		}
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] & ~0xff00ffff) >> 16;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_CHNPOST] & ~0xff00ffff) >> 16;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		break;

	default:
		return SC_RESULT_FAIL;
		break;
	}

	if (pParam->nQueryType)
	{
	      nLowThre = 0;
            nHigThre = 127;
		nLen = sprintf(cBuff, "%d-%d", nLowThre, nHigThre);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
	} 
	else
	{
		nLen = sprintf(cBuff, "%d", nVal);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
	}

	return nResult;
}

int __1DCodeMaxLen_Set(sc_param_st *pParam,
                      unsigned short nCodeType)
{
	int  nVal, nResult = SC_RESULT_Save;
      unsigned int nMin;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = 2;
		return SC_RESULT_NEXT;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 127)
	{
		return SC_RESULT_FAIL;
	}

	switch (nCodeType) {
	case POS_CODE128:
             nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_UCCEAN128:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_REGULAR39:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_INTER2OF5:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_MATRIX25:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_MODULE25:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_CODABAR:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_CODE93:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_CODEZ:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_CODE11:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_INDUSTRIAL25:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_STANDARD25:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_PLESSEY:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_MSI_PLESSEY:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] & 0x00ffffff) | (nVal << 24);
		break;
	case POS_CHNPOST:
            nMin = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] & 0x00ff0000)>>16;
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] & 0x00ffffff) | (nVal << 24);
		break;
	default:
		return SC_RESULT_FAIL;
		break;
	}
	
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[nCodeType] = X_paramGetGlobalAllParam()->p1D->ExMsg[nCodeType];
	}
	
	return nResult;
}

int __1DCodeMaxLen_Get(sc_param_st *pParam,
                      unsigned short nCodeType)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	int nLowThre = 0;
	int nHigThre = 0;
	char cBuff[8];
	UTDec1DSetup p1d;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (nCodeType) {
	case POS_CODE128:
		if (pParam->nQueryType)
		{
			nLowThre = 1;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_CODE128] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}

		break;

	case POS_UCCEAN128:

		if (pParam->nQueryType)
		{
			nLowThre = 1;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_UCCEAN128] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_REGULAR39:
		if (pParam->nQueryType)
		{
			nLowThre = 1;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_REGULAR39] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_INTER2OF5:
		if (pParam->nQueryType)
		{
			nLowThre = 6;
			nHigThre = 80;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_INTER2OF5] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_MATRIX25:

		if (pParam->nQueryType)
		{
			nLowThre = 4;
			nHigThre = 80;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_MATRIX25] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_MODULE25:
		if (pParam->nConfType == SC_CFG_NSYS)
		{
			nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] & ~0x00ffffff) >> 24;
		}
		else if (pParam->nConfType == SC_CFG_SYS)
		{
			nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] & ~0x00ffffff) >> 24;
		}
		else if (pParam->nConfType == SC_CFG_FACT)
		{
			if (X_FacParamIsGet() != TRUE)
			{
				return SC_RESULT_FAIL;
			}
			memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
			nVal = (p1d.ExMsg[POS_MODULE25] & ~0x00ffffff) >> 24;

		}
		else
		{
			return SC_RESULT_FAIL;
		}
		break;

	case POS_CODABAR:
		if (pParam->nQueryType)
		{
			nLowThre = 2;
			nHigThre = 60;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_CODABAR] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_CODE93:
		if (pParam->nQueryType)
		{
			nLowThre = 1;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_CODE93] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_CODEZ:
		if (pParam->nConfType == SC_CFG_NSYS)
		{
			nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] & ~0x00ffffff) >> 24;
		}
		else if (pParam->nConfType == SC_CFG_SYS)
		{
			nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] & ~0x00ffffff) >> 24;
		}
		else if (pParam->nConfType == SC_CFG_FACT)
		{
			if (X_FacParamIsGet() != TRUE)
			{
				return SC_RESULT_FAIL;
			}
			memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
			nVal = (p1d.ExMsg[POS_CODEZ] & ~0x00ffffff) >> 24;

		}
		else
		{
			return SC_RESULT_FAIL;
		}
		break;

	case POS_CODE11:
		if (pParam->nQueryType)
		{
			nLowThre = 4;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_CODE11] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_INDUSTRIAL25:
		if (pParam->nQueryType)
		{
			nLowThre = 6;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_INDUSTRIAL25] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_STANDARD25:
		if (pParam->nQueryType)
		{
			nLowThre = 6;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_STANDARD25] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;
	case POS_PLESSEY:
		if (pParam->nQueryType)
		{
			nLowThre = 4;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_PLESSEY] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_MSI_PLESSEY:
		if (pParam->nQueryType)
		{
			nLowThre = 4;
			nHigThre = 48;
		} 
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_MSI_PLESSEY] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		
		break;

	case POS_CHNPOST:
		if (pParam->nQueryType)
		{
			nLowThre = 1;
			nHigThre = 48;
		}
		else
		{
			if (pParam->nConfType == SC_CFG_NSYS)
			{
				nVal = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_SYS)
			{
				nVal = (X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] & ~0x00ffffff) >> 24;
			}
			else if (pParam->nConfType == SC_CFG_FACT)
			{
				if (X_FacParamIsGet() != TRUE)
				{
					return SC_RESULT_FAIL;
				}
				memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
				nVal = (p1d.ExMsg[POS_CHNPOST] & ~0x00ffffff) >> 24;

			}
			else
			{
				return SC_RESULT_FAIL;
			}
		}
		break;

	default:
		return SC_RESULT_FAIL;
		break;
	}

	if (pParam->nQueryType)
	{
	      nLowThre = 0;
            nHigThre = 127;
		nLen = sprintf(cBuff, "%d-%d", nLowThre, nHigThre);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
		
	} 
	else
	{
		nLen = sprintf(cBuff, "%d", nVal);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
	}
	

	return nResult;
}

//Code ID设置、读取
int __1DCodeID_Set(sc_param_st *pParam,unsigned short nCodeType)
{
	int nResult = SC_RESULT_SaveAndUpdateAll; 
      unsigned char ucID0, ucID1, temp1=0, temp2=0;
//	char Buff[32];

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
    	if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = 4;
		return SC_RESULT_NEXT;
	}
        
	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

	if (pParam->nDataLen < 2 || pParam->nDataLen > 4)
	{
		return SC_RESULT_FAIL;
	}

      if(pParam->pData[0] <= 0x39)
            temp1 = (pParam->pData[0] - 0x30)*16;
      else if(pParam->pData[0] <= 0x5a && pParam->pData[0] >= 0x41) 
            temp1 = (pParam->pData[0] - 0x41 + 10)*16;			
      else if(pParam->pData[0] <= 0x7a && pParam->pData[0] >= 0x61)	
            temp1 = (pParam->pData[0] - 0x61 + 10)*16;

      if(pParam->pData[1] <= 0x39)
            temp2 = (pParam->pData[1] - 0x30);
      else if(pParam->pData[1] <= 0x5a && pParam->pData[1] >= 0x41) 
            temp2 = (pParam->pData[1] - 0x41 + 10);			
      else if(pParam->pData[1] <= 0x7a && pParam->pData[1] >= 0x61)	
            temp2 = (pParam->pData[1] - 0x61 + 10);					
     
    ucID0 = temp1 + temp2;
     
    //if ((ucID0 > 0x40 && ucID0 < 0x5b) || (ucID0 > 0x60 && ucID0 < 0x7b))
    {
        X_paramGetGlobalAllParam()->pCode->by1DCodeID[nCodeType] = ucID0;
    }
    /*else
    {
        return SC_RESULT_FAIL;
    }*/

    if (pParam->nDataLen == 4)
    {
        if(pParam->pData[2] <= 0x39)
            temp1 = (pParam->pData[2] - 0x30)*16;
        else if(pParam->pData[2] <= 0x5a && pParam->pData[2] >= 0x41) 
            temp1 = (pParam->pData[2] - 0x41 + 10)*16;			
        else if(pParam->pData[2] <= 0x7a && pParam->pData[2] >= 0x61)	
            temp1 = (pParam->pData[2] - 0x61 + 10)*16;

        if(pParam->pData[3] <= 0x39)
            temp2 = (pParam->pData[3] - 0x30);
        else if(pParam->pData[3] <= 0x5a && pParam->pData[3] >= 0x41) 
            temp2 = (pParam->pData[3] - 0x41 + 10);			
        else if(pParam->pData[3] <= 0x7a && pParam->pData[3] >= 0x61)	
            temp2 = (pParam->pData[3] - 0x61 + 10);
					   
        ucID1 = temp1 + temp2;
    
        //if ((ucID1 > 0x40 && ucID1 < 0x5b) || (ucID1 > 0x60 && ucID1 < 0x7b))
        { 
            X_paramGetGlobalAllParam()->pCode2->by1DCodeID_2[nCodeType] = ucID1;
        }
        /*else
        {
            return SC_RESULT_FAIL;
        }*/
            
    }
    else
    {
        X_paramGetGlobalAllParam()->pCode2->by1DCodeID_2[nCodeType] = 0;
    }

    X_paramUpdateToDevice();
	
    if (!(pParam->bSaveFlag))
    {
        nResult = SC_RESULT_SuccUnsave;
    }
    else
    {
        X_paramBakGetGlobalAllParam()->pCode->by1DCodeID[nCodeType] = X_paramGetGlobalAllParam()->pCode->by1DCodeID[nCodeType];
        X_paramBakGetGlobalAllParam()->pCode2->by1DCodeID_2[nCodeType] = X_paramGetGlobalAllParam()->pCode2->by1DCodeID_2[nCodeType];
    }

    return nResult;

}

int __1DCodeID_Get(sc_param_st *pParam,unsigned short nCodeType)
{
	unsigned char ucID0, ucID1;
	int  nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	CodeParam sCodeParam0;
	CodeParam2 sCodeParam1;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

      if(pParam->nQueryType)
      {  
             nLen = sprintf(cBuff, "01-FF");
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
             else
                return SC_RESULT_FAIL;
             return nResult;
      }
	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS://RAM # + *
		//Y_commSendString("\r\nQRAM\r\n");
		ucID0 = X_paramGetGlobalAllParam()->pCode->by1DCodeID[nCodeType] ;
		ucID1 = X_paramGetGlobalAllParam()->pCode2->by1DCodeID_2[nCodeType];
		break;

	case SC_CFG_SYS://SYS @ + * 
		//Y_commSendString("\r\nQSYS\r\n");
		ucID0 = X_paramBakGetGlobalAllParam()->pCode->by1DCodeID[nCodeType];
		ucID1 = X_paramBakGetGlobalAllParam()->pCode2->by1DCodeID_2[nCodeType];
		break;

	case SC_CFG_FACT://FAC  @ +#  & 
		//Y_commSendString("\r\nQFAC\r\n");
		if (X_FacParamIsGet() != TRUE)
		{
			return SC_RESULT_FAIL;
		}
		memcpy(&sCodeParam0, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));
		memcpy(&sCodeParam1, FacParamBuf + Start_MEMSize_CODE2, sizeof(CodeParam2));

		ucID0 = sCodeParam0.by1DCodeID[nCodeType];
		ucID1 = sCodeParam1.by1DCodeID_2[nCodeType];
		break;

	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	if (!ucID1)
	{
		nLen = sprintf(cBuff, "%X", ucID0);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
	}
	else
	{
		nLen = sprintf(cBuff, "%X%X", ucID0,ucID1);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
	}
	return nResult;
}
/****************************************************************************
*@普通Code 128
****************************************************************************/
//加载Code 128 默认设置	Tag：128DEF
int Code128LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SaveAndUpdateAll;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));

	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = u1d.ExMsg[POS_CODE128];//Init_CODE128;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_CODE128));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_CODE128));//|= (0x01 << POS_CODE128);

	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int Code128LoadDefault_Get(sc_param_st *pParam)
{
      return SC_RESULT_SUCC;
}

//Code 128的使能、禁止	Tag：128ENA
int Code128Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_CODE128);
}

int Code128Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_CODE128);
}

//设置Code 128最小长度的设置	Tag：128MIN
int Code128MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_CODE128);
}

int Code128MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_CODE128);
}

//设置Code 128的最大长度	Tag：128MAX
int Code128MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_CODE128);
}

int Code128MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_CODE128);
}

//设置Code 128校验位发送的使能、禁止	Tag：128CHK
//const unsigned int c_nCode128TransChkTab[2] = {0, eB1D_CODE128_TRSMTCHK};
int Code128ChkTrsmt_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int Code128ChkTrsmt_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置Code 128的FNC1输出的使能、禁止	Tag：128FNC
//const unsigned int c_nCode128FNC1Tab[2] = {0, eB1D_CODE128_FNC1};
int Code128FNC1_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int Code128FNC1_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置Code 128 CODEID Tag：CID002
int Code128CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_CODE128);
}

int Code128CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_CODE128);
}

/****************************************************************************
*@EAN-8
****************************************************************************/

int EAN8LoadDefault_Set(sc_param_st * pParam)
{
	int  nResult = SC_RESULT_SUCC;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(pParam->nDataLen != 0)
      {
		return SC_RESULT_FAIL;
	}

       if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));

	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] = u1d.ExMsg[POS_EAN8];//Init_EAN8;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_EAN8));
	X_paramGetGlobalAllParam()->p1D->unDecTypes  |= (u1d.unDecTypes & POS_EAN8);//|= (0x01 << POS_EAN8);
	
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN8];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int EAN8LoadDefault_Get(sc_param_st * pParam)
{
        return SC_RESULT_SUCC;
}

//设置EAN-8的使能、禁止	Tag:EA8ENA
int EAN8Enable_Set(sc_param_st * pParam)
{
	return __1DCodeEnable_Set(pParam, POS_EAN8);
}

int EAN8Enable_Get(sc_param_st * pParam)
{
	return __1DCodeEnable_Get(pParam, POS_EAN8);
}

//设置EAN-8校验位的使能、禁止	Tag：EA8CHK
const unsigned int nEAN8TrsmtTab[3] = {0,0x00,0x01};
int EAN8ChkTrsmt_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN8,
									1,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nEAN8TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

int EAN8ChkTrsmt_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN8,
									1,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nEAN8TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置EAN-8 2位扩展码的使能、禁止	Tag:EA8AD2
const unsigned int nEAN8AddOn2Tab[2] = {0, 0x02};
int EAN8AddCode2Enable_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN8,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x02,			//对应功能的设置位标识
									&nEAN8AddOn2Tab	//依据指令顺序排列的功能设置值
									);

}

int EAN8AddCode2Enable_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN8,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x02,			//对应功能的设置位标识
									&nEAN8AddOn2Tab	//依据指令顺序排列的功能设置值
									);
}

//设置EAN-8 5位扩展码的使能、禁止	Tag:EA8AD5
const unsigned int nEAN8AddOn5Tab[2] = {0, 0x04};
int EAN8AddCode5Enable_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN8,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nEAN8AddOn5Tab	//依据指令顺序排列的功能设置值
									);
}

int EAN8AddCode5Enable_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN8,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nEAN8AddOn5Tab	//依据指令顺序排列的功能设置值
									);
}

//设置EAN-8拓展码的使能、禁止	Tag:EA8REQ
//const unsigned int c_nEAN8AddOnReqTab[2] = {0, eB1D_EAN8_ADDONREQ};
int EAN8AddedCodeEnable_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN8AddedCodeEnable_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置EAN-8扩展码是否添加分隔符	Tag:EA8SEP
int EAN8SEPRInAddedCodeEnable_Set(sc_param_st * pParam)
{
        return SC_RESULT_UnSurpport;
}

int EAN8SEPRInAddedCodeEnable_Get(sc_param_st * pParam)
{
        return SC_RESULT_UnSurpport;
}

//设置EAN-8是否转换为EAN13或内容扩展为13位	Tag：EA8EXP
const unsigned int nEAN8Exp2EAN13Tab[2] = {0,0x10};
int EAN8ExpandedType_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN8,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x10,			//对应功能的设置位标识
									&nEAN8Exp2EAN13Tab	//依据指令顺序排列的功能设置值
									);
}

int EAN8ExpandedType_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN8,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x10,			//对应功能的设置位标识
									&nEAN8AddOn5Tab	//依据指令顺序排列的功能设置值
									);
}

//设置EAN-8 CODEID的输出使能、禁止	Tag：CID004
int EAN8CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_EAN8);
}

int EAN8CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_EAN8);
}

/****************************************************************************
*@EAN-13
****************************************************************************/
//加载EAN13的默认通用设置	Tag：E13DEF
int EAN13LoadDefault_Set(sc_param_st * pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
      
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
      
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] = u1d.ExMsg[POS_EAN13];//Init_EAN13;
	
	X_paramGetGlobalAllParam()->p1D->unDecTypes  |= (u1d.unDecTypes & (0x01 << POS_EAN13));//|= (0x01 << POS_EAN13);
	X_paramUpdateToDevice();
    
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN13];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int EAN13LoadDefault_Get(sc_param_st * pParam)
{
	return SC_RESULT_SUCC;
}

//设置EAN13的使能、禁止	Tag：E13ENA
int EAN13Enable_Set(sc_param_st * pParam)
{
	return __1DCodeEnable_Set(pParam, POS_EAN13);
}

int EAN13Enable_Get(sc_param_st * pParam)
{
	return __1DCodeEnable_Get(pParam, POS_EAN13);
}

//设置EAN13校验位的使能、禁止	Tag：E13CHK
const unsigned int nEAN13TrsmtTab[3] = { 0,0x00,0x01 };
int EAN13ChkTrsmt_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN13,
									1,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nEAN13TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

int EAN13ChkTrsmt_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN13,
									1,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nEAN13TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置EAN13 2位 扩展码的使能、禁止  Tag:E13AD2
const unsigned int nEAN13AddOn2Tab[2] = { 0, 0x02 };
int EAN13AddCode2Enable_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN13,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x02,			//对应功能的设置位标识
									&nEAN13AddOn2Tab	//依据指令顺序排列的功能设置值
									);
}

int EAN13AddCode2Enable_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN13,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x02,			//对应功能的设置位标识
									&nEAN13AddOn2Tab	//依据指令顺序排列的功能设置值
									);
}


//设置EAN13 5位 扩展码的使能、禁止  Tag:E13AD5
const unsigned int nEAN13Addon5Tab[2] = {0, 0x04};
int EAN13AddCode5Enable_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN13,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nEAN13Addon5Tab	//依据指令顺序排列的功能设置值
									);
}

int EAN13AddCode5Enable_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN13,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nEAN13Addon5Tab	//依据指令顺序排列的功能设置值
									);
}

//设置EAN13拓展码的使能、禁止	Tag:E13REQ
int EAN13AddedCodeEnable_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13AddedCodeEnable_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置EAN13扩展码是否添加分隔符	Tag:E13SEP
int EAN13SEPRInAddedCodeEnable_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13SEPRInAddedCodeEnable_Get(sc_param_st * pParam)
{
        return SC_RESULT_UnSurpport;
}

//设置EAN13 Beginning with 290 Addenda Tag：E132900、 E132901
//const unsigned int c_nEAN13290AddonReqTab[2] = {0, eB1D_EAN13_290AddOnReq};
int EAN13Beggin290Addenda_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin290Addenda_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置EAN13 Beginning with 378_379 Addenda Tag：E132780、 E132781
//const unsigned int c_nEAN13378AddonReqTab[2] = {0, eB1D_EAN13_378AddOnReq};
int EAN13Beggin378_379Addenda_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin378_379Addenda_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置EAN13 Beginning with 414_419 Addenda Tag：E134140、 E134141
//const unsigned int c_nEAN13414AddonReqTab[2] = {0, eB1D_EAN13_414AddOnReq};
int EAN13Beggin414_419Addenda_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin414_419Addenda_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置EAN13 Beginning with 434_439 Addenda Tag：E134340、 E134341
//const unsigned int c_nEAN13434AddonReqTab[2] = {0, eB1D_EAN13_434AddOnReq};
int EAN13Beggin434_439Addenda_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin434_439Addenda_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置EAN13 Beginning with 977 Addenda Tag：E139770、 E139771
//const unsigned int c_nEAN13977AddonReqTab[2] = {0, eB1D_EAN13_977AddOnReq};
int EAN13Beggin977Addenda_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin977Addenda_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置EAN13 Beginning with 978 Addenda Tag：E139780、 E139781
//const unsigned int c_nEAN13978AddonReqTab[2] = {0, eB1D_EAN13_978AddOnReq};
int EAN13Beggin978Addenda_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin978Addenda_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置EAN13 Beginning with 979 Addenda Tag：E139790、 E139791
//const unsigned int c_nEAN13979AddonReqTab[2] = {0, eB1D_EAN13_979AddOnReq};
int EAN13Beggin979Addenda_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin979Addenda_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置EAN-13 CODEID的输出使能、禁止	Tag：CID005
int EAN13CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_EAN13);
}

int EAN13CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_EAN13);
}
/****************************************************************************
*@UPC-E
****************************************************************************/
//加载UPC-E的默认设置	Tag:UPEDEF
int UPCELoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
      
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] = u1d.ExMsg[POS_UPCE];// = Init_UPCE;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UPCE));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_UPCE));// |= (0x01 << POS_UPCE);

	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}


int UPCELoadDefault_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_SUCC;
}

//设置UPC-E的使能、禁止	Tag：UPEENA
int UPCEEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_UPCE);
}

int UPCEEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_UPCE);
}


//设置UPC-E的UPC-E0使能、禁止	Tag：UPEEN0
int UPCE0Enable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
int UPCE0Enable_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_UnSurpport;
}

//设置UPC-E的UPC-E1使能、禁止	Tag：UPEEN1
int UPCE1Enable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int UPCE1Enable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//设置UPC-E校验位的使能、禁止	Tag：UPECHK
const unsigned int nUPCETrsmtTab[3] = {0,0,0x01};
int UPCEChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCE,
									1,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nUPCETrsmtTab	//依据指令顺序排列的功能设置值
									);
}
int UPCEChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCE,
									1,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nUPCETrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置UPC-E 2位扩展码的使能、禁止 Tag:UPEAD2
const unsigned int nUPCEAddOn2Tab[2] = { 0, 0x02 };
int UPCEAddCode2Enable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCE,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x02,			//对应功能的设置位标识
									&nUPCEAddOn2Tab	//依据指令顺序排列的功能设置值
									);
}

int UPCEAddCode2Enable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCE,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x02,			//对应功能的设置位标识
									&nUPCEAddOn2Tab	//依据指令顺序排列的功能设置值
									);
}

//设置UPC-E 5位扩展码的使能、禁止 Tag:UPEAD5
const unsigned int nUPCEAddOn5Tab[2] = { 0, 0x04 };
int UPCEAddCode5Enable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCE,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nUPCEAddOn5Tab	//依据指令顺序排列的功能设置值
									);
}

int UPCEAddCode5Enable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCE,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nUPCEAddOn5Tab	//依据指令顺序排列的功能设置值
									);
}

//设置UPC-E拓展码的使能、禁止	Tag:UPEREQ
//const unsigned int c_nUPCEAddonReqTab[2] = { 0, eB1D_UPCE_ADDONREQ };
int UPCEAddedCodeEnable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int UPCEAddedCodeEnable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置UPC-E扩展码是否添加分隔符	Tag:UPESEP
int UPCESEPRInAddedCodeEnable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int UPCESEPRInAddedCodeEnable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//设置UPC-E 发送前文的类型	Tag：UPEPRE
const unsigned int nUPCEPreTab[3] = { 0,0x08,0x108 };
int UPCETrsmtSysDigitType_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCE,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x108,			//对应功能的设置位标识
									&nUPCEPreTab	//依据指令顺序排列的功能设置值
									);
}

int UPCETrsmtSysDigitType_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCE,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x108,			//对应功能的设置位标识
									&nUPCEPreTab	//依据指令顺序排列的功能设置值
									);
}

//设置UPC-E使能、禁止转换为UPC-A Tag：UPEEXP
const unsigned int nUPCEExpTab[3] = { 0, 0x80,0x10};
int UPCEExpandedType_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCE,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x90,			//对应功能的设置位标识
									&nUPCEExpTab	//依据指令顺序排列的功能设置值
									);
}
int UPCEExpandedType_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCE,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x90,			//对应功能的设置位标识
									&nUPCEExpTab	//依据指令顺序排列的功能设置值
									);
}

//设置UPC-E CODEID的输出使能、禁止	Tag：CID006
int UPCECodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_UPCE);
}
int UPCECodeID_Get(sc_param_st *pParam)
{ 
	return __1DCodeID_Get(pParam, POS_UPCE);
}

/****************************************************************************
*@UPC-A
****************************************************************************/
//加载UPC-A的默认设置	Tag:UPADEF
int UPCALoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
      
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] = u1d.ExMsg[POS_UPCA];// = Init_UPCA;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UPCA));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_UPCA));// |= (0x01 << POS_UPCA);
	X_paramUpdateToDevice();
    
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCA];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int UPCALoadDefault_Get(sc_param_st *pParam)
{ return SC_RESULT_SUCC; }

//设置UPC-A的使能、禁止	Tag：UPAENA
int UPCAEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_UPCA);
}

int UPCAEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_UPCA);
}

//设置UPC-A校验位的使能、禁止	Tag：UPACHK
const unsigned int nUPCATrsmtTab[3] = { 0, 0,0x01 };
int UPCAChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									1,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nUPCATrsmtTab	//依据指令顺序排列的功能设置值
									);
}
int UPCAChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									1,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nUPCATrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置UPC-A 2位扩展码的识读使能、禁止	Tag:UPAAD2
const unsigned int nUPCAAddOn2Tab[2] = { 0, 0x02 };
int UPCAAddCode2Enable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x02,			//对应功能的设置位标识
									&nUPCAAddOn2Tab	//依据指令顺序排列的功能设置值
									);
}
int UPCAAddCode2Enable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x02,			//对应功能的设置位标识
									&nUPCAAddOn2Tab	//依据指令顺序排列的功能设置值
									);
}

//设置UPC-A 5位扩展码的识读使能、禁止	Tag:UPAAD5
const unsigned int nUPCAAddOn5Tab[2] = { 0, 0x04 };
int UPCAAddCode5Enable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nUPCAAddOn5Tab	//依据指令顺序排列的功能设置值
									);
}

int UPCAAddCode5Enable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nUPCAAddOn5Tab	//依据指令顺序排列的功能设置值
									);
}

//设置UPC-E拓展码的使能、禁止	Tag:UPAREQ
//const unsigned int c_nUPCAAddonReqTab[2] = { 0, eB1D_UPCA_ADDONREQ };
int UPCAAddedCodeEnable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
int UPCAAddedCodeEnable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置UPC-A扩展码是否添加分隔符	Tag:UPASEP
int UPCASEPRInAddedCodeEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int UPCASEPRInAddedCodeEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置UPC-A 发送前文的类型	Tag：UPAPRE		
const unsigned int nUPCAPreTab[3] = {0x200,0x0, 0x08};
int UPCATrsmtSysDigitType_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x208,			//对应功能的设置位标识
									&nUPCAPreTab	//依据指令顺序排列的功能设置值
									);
}

int UPCATrsmtSysDigitType_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x208,			//对应功能的设置位标识
									&nUPCAPreTab	//依据指令顺序排列的功能设置值
									);
}


//设置UPC-A CODEID的输出使能、禁止	Tag：CID007
int UPCACodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_UPCA);
}

int UPCACodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_UPCA);
}

/****************************************************************************
*@Coupon
****************************************************************************/
//设置UPCA/EAN13输出Extended Coupon Code的状态（off,allow,require三种）Tag:CPNENA
const unsigned int nUPCA2CouponTab[3] = {0xc0,0x40,0x80};
int UPCA_EAN13WithExtendedCoupon_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0xc0,			//对应功能的设置位标识
									&nUPCA2CouponTab	//依据指令顺序排列的功能设置值
									);
}

int UPCA_EAN13WithExtendedCoupon_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0xc0,			//对应功能的设置位标识
									&nUPCA2CouponTab	//依据指令顺序排列的功能设置值
									);
}

//Coupon GS1 DataBar Output Tag:CPNGS1
const unsigned int nCouponGS1Tab[2] = {0,0x100};
int CouponGS1DataBarOutput_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x100,			//对应功能的设置位标识
									&nCouponGS1Tab	//依据指令顺序排列的功能设置值
									);
}

int CouponGS1DataBarOutput_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x100,			//对应功能的设置位标识
									&nCouponGS1Tab	//依据指令顺序排列的功能设置值
									);
}

/****************************************************************************
*@Interleaved 2 of 5
****************************************************************************/
//加载Inter2of5的默认设置	Tag:I25DEF
int Inter2of5LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      int i;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));

	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = u1d.ExMsg[POS_INTER2OF5];// = Init_INTER25;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_INTER2OF5));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_INTER2OF5));// |= (0x01 << POS_INTER2OF5);
	for(i=0;i<5;i++)
            X_paramGetGlobalAllParam()->p1D->I2Of5Len[i] = u1d.I2Of5Len[i];
	
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int Inter2of5LoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//设置Inter2of5的使能、禁止	Tag：I25ENA
int Inter2of5Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_INTER2OF5);
}
int Inter2of5Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_INTER2OF5);
}

//设置Inter2of5的识读最小长度	Tag：I25MIN
int Inter2of5MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_INTER2OF5);
}

int Inter2of5MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_INTER2OF5);
}

//设置Inter2of5的识读最大长度	Tag：I25MAX
int Inter2of5MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_INTER2OF5);
}

int Inter2of5MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_INTER2OF5);
}

//设置Inter2of5校验位的使能、禁止	Tag：I25CHK
const unsigned int nInter25TrsmtTab[3] = { 0, 0x01,0x03 };
int Inter2of5ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_INTER2OF5,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nInter25TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

int Inter2of5ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_INTER2OF5,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nInter25TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置识读固定长度条码功能的使能、禁止	Tag:I25LEN
int Inter2of5DecoFixedLenEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int Inter2of5DecoFixedLenEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置Inter2of5添加固定长度值	Tag:I25LSE
int Inter2of5AddFixedLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int Inter2of5AddFixedLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置Inter2of5取消固定长度值	Tag:I25LCA
int Inter2of5CancelFixedLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int Inter2of5CancelFixedLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置Inter2of5 的Febraban 使能态，进行信息扩展	Tag:I25FBB
//const unsigned int c_nInter25FBBTab[3] = { 0, eB1D_ITF_SPECEDIT, eB1D_ITF_SPECEDIT | eB1D_ITF_TOFULL };
int Inter2of5FebrabanEnable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int Inter2of5FebrabanEnable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置Inter2of5 CODEID的输出使能、禁止	Tag：CID008
int Inter2of5CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_INTER2OF5);
}

int Inter2of5CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_INTER2OF5);
}

/****************************************************************************
*@ITF-14
****************************************************************************/
//加载ITF-14 的默认设置	Tag:I14DEF
int ITF14LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));

	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_ITF14] = u1d.ExMsg[POS_ITF14];//Init_CODE128;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_ITF14));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_ITF14));//|= (0x01 << POS_CODE128);

	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ITF14] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_ITF14];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int ITF14LoadDefault_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_SUCC;
}

//设置ITF-14 的使能、禁止及校验位的传送	Tag：I14ENA
int ITF14Enable_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 2)
	{
		return SC_RESULT_FAIL;
	}

	if (nVal == 0)//禁止
	{
		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_ITF14));
	}
	else if(nVal == 1)//使能
	{
		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF14);
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= (~0x04);
	}
	else if(nVal == 2)
	{
		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF14);
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= 0x04;
	}
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5];
	}
	return nResult;
}

int ITF14Enable_Get(sc_param_st *pParam)
{
	int  nDecType,nExMsg, nResult = SC_RESULT_SUCC;
	UTDec1DSetup p1d;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	if (pParam->nQueryType)
	{
		pParam->pOutBuf[0] = '0';
		pParam->pOutBuf[1] = '-';
		pParam->pOutBuf[2] = '2';
		*(pParam->pOutLen) = 3;

	} 
	else
	{
		switch (pParam->nConfType)
		{
		case SC_CFG_NSYS://RAM
			nDecType = X_paramGetGlobalAllParam()->p1D->unDecTypes &(0x01 << POS_ITF14);
			nExMsg = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & 0x04;
			break;

		case SC_CFG_SYS://SYS
			nDecType = X_paramBakGetGlobalAllParam()->p1D->unDecTypes &(0x01 << POS_ITF14);
			nExMsg = X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & 0x04;
			break;

		case SC_CFG_FACT://FAC
			if (X_FacParamIsGet() != TRUE)
			{
				return SC_RESULT_FAIL;
			}
			memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
			nDecType = p1d.unDecTypes & (0x01 << POS_ITF14);
			nExMsg = p1d.ExMsg[POS_INTER2OF5] & 0x04;
			break;

		default:
			return SC_RESULT_UnSurpport;
			break;
		}

		nDecType >>=  POS_ITF14;
		if (nDecType == 0)
		{
			pParam->pOutBuf[0] = '0';
		}
		else if (nDecType == 1 && nExMsg == 0)
		{
			pParam->pOutBuf[0] = '1';
		}
		else if (nDecType == 1 && nExMsg == 0x04)
		{
			pParam->pOutBuf[0] = '2';
		}
		else 
		{
			return SC_RESULT_FAIL;
		}

		*(pParam->pOutLen) = 1;
	}

	return nResult;
}

//设置ITF-14 CODEID的输出使能、禁止	Tag：CID009
int ITF14CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_ITF14);
}

int ITF14CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_ITF14);
}

/****************************************************************************
*@ITF-6
****************************************************************************/
//加载ITF-6 的默认设置	Tag:IT6DEF
int ITF6LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SaveAndUpdateAll;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));

	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_ITF6] = u1d.ExMsg[POS_ITF6];//Init_CODE128;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_ITF6));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_ITF6));//|= (0x01 << POS_CODE128);

	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ITF6] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_ITF6];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int ITF6LoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//设置ITF-6 的使能、禁止及校验位的传送	Tag：IT6ENA
int ITF6Enable_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 2)
	{
		return SC_RESULT_FAIL;
	}

	if (nVal == 0)//禁止
	{
		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_ITF6));
	}
	else if (nVal == 1)//使能
	{
		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF6);
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= (~0x08);

	}
	else if (nVal == 2)
	{
		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF6);
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= 0x08;
	}
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5];
	}
	return nResult;
}

int ITF6Enable_Get(sc_param_st *pParam)
{
	int  nDecType, nExMsg, nResult = SC_RESULT_SUCC;
	UTDec1DSetup p1d;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	if (pParam->nQueryType)
	{
		pParam->pOutBuf[0] = '0';
		pParam->pOutBuf[1] = '-';
		pParam->pOutBuf[2] = '2';
		*(pParam->pOutLen) = 3;
	} 
	else
	{
		switch (pParam->nConfType)
		{
		case SC_CFG_NSYS://RAM
			nDecType = X_paramGetGlobalAllParam()->p1D->unDecTypes &(0x01 << POS_ITF6);
			nExMsg = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & 0x08;
			break;

		case SC_CFG_SYS://SYS
			nDecType = X_paramBakGetGlobalAllParam()->p1D->unDecTypes &(0x01 << POS_ITF6);
			nExMsg = X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & 0x08;
			break;

		case SC_CFG_FACT://FAC
			if (X_FacParamIsGet() != TRUE)
			{
				return SC_RESULT_FAIL;
			}
			memcpy(&p1d, FacParamBuf + Start_MEMSize_1D, sizeof(UTDec1DSetup));
			nDecType = p1d.unDecTypes & (0x01 << POS_ITF6);
			nExMsg = p1d.ExMsg[POS_INTER2OF5] & 0x08;
			break;

		default:
			return SC_RESULT_UnSurpport;
			break;
		}

		nDecType >>= POS_ITF6;
		if (nDecType == 0)
		{
			pParam->pOutBuf[0] = '0';
		}
		else if (nDecType == 1 && nExMsg == 0)
		{
			pParam->pOutBuf[0] = '1';
		}
		else if (nDecType == 1 && nExMsg == 0x08)
		{
			pParam->pOutBuf[0] = '2';
		}
		else
		{
			return SC_RESULT_FAIL;
		}

		*(pParam->pOutLen) = 1;
	}
	
	return nResult;
}

//设置ITF-6 CODEID的输出使能、禁止	Tag：CID010
int ITF6CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_ITF6);
}

int ITF6CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_ITF6);
}
//5285
/****************************************************************************
*@Matrix 2 of 5
****************************************************************************/
//加载Matrix 2 of 5 的默认设置	Tag:M25DEF
int Matrix2of5LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
       
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25]  = u1d.ExMsg[POS_MATRIX25];//= Init_MATRIX25;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_MATRIX25));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_MATRIX25));// &= ~(0x01 << POS_MATRIX25);
	//printf("u=%x,%X\n",u1d.unDecTypes,X_paramGetGlobalAllParam()->p1D->unDecTypes);
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int Matrix2of5LoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//设置Matrix 2 of 5 的使能、禁止	Tag：M25ENA
int Matrix2of5Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_MATRIX25);
}

int Matrix2of5Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_MATRIX25);
}

//设置Matrix 2 of 5的识读最小长度	Tag：M25MIN
int Matrix2of5MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_MATRIX25);
}
int Matrix2of5MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_MATRIX25);
}

//设置Matrix 2 of 5的识读最大长度	Tag：M25MAX
int Matrix2of5MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_MATRIX25);
}

int Matrix2of5MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_MATRIX25);
}

//设置Matrix 2 of 5校验位的使能、禁止	Tag：M25CHK
const unsigned int nMatr25TrsmtTab[3] = { 0,0x02,0x03 };
int Matrix2of5ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_MATRIX25,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nMatr25TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

int Matrix2of5ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_MATRIX25,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nMatr25TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置Matrix 2 of 5 CODEID的输出使能、禁止	Tag：CID011
int Matrix2of5CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_MATRIX25);
}

int Matrix2of5CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_MATRIX25);
}

/****************************************************************************
*@Code39
****************************************************************************/
//加载Code39的默认设置	Tag:C39DEF
int Code39LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
     
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
     
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] = u1d.ExMsg[POS_REGULAR39];// = Init_REGULAR39;
	
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_REGULAR39));// |= (0x01 << POS_REGULAR39);
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int Code39LoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//设置Code39的使能、禁止	Tag：C39ENA
int Code39Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_REGULAR39);
}

int Code39Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_REGULAR39);
}

//设置Code39的识读最小长度	Tag：C39MIN
int Code39MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_REGULAR39);
}

int Code39MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_REGULAR39);
}

//设置Code39的识读最大长度	Tag：C39MAX
int Code39MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_REGULAR39);
}
int Code39MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_REGULAR39);
}

//设置Code39校验位的使能、禁止	Tag：C39CHK
const unsigned int nCode39TrsmtTab[3] = { 0,0x01,0x03 };
int Code39ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_REGULAR39,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nCode39TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

int Code39ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_REGULAR39,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nCode39TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置Code39的传送起始符与终止符的使能、禁止	Tag：C39TSC
const unsigned int nCode39TSCTab[2] = { 0,0x04 };
int Code39TrsmtSTXETXEnable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_REGULAR39,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nCode39TSCTab	//依据指令顺序排列的功能设置值
									);
}

int Code39TrsmtSTXETXEnable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_REGULAR39,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nCode39TSCTab	//依据指令顺序排列的功能设置值
									);
}

//设置Code39 Full ASCII的使能、禁止	Tag：C39ASC
const unsigned int nCode39ASCTab[2] = { 0,0x08 };
int Code39FullASCIIEnable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_REGULAR39,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x08,			//对应功能的设置位标识
									&nCode39ASCTab	//依据指令顺序排列的功能设置值
									);
}

int Code39FullASCIIEnable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_REGULAR39,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x08,			//对应功能的设置位标识
									&nCode39ASCTab	//依据指令顺序排列的功能设置值
									);
}

//设置Code39的Code 32 Pharmaceutical (PARAF)的使能、禁止	Tag：C39E32
const unsigned int nCode39E32Tab[2] = {0,0x20 };
int Code39Code32PARAFEnable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_REGULAR39,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x20,			//对应功能的设置位标识
									&nCode39E32Tab	//依据指令顺序排列的功能设置值
									);
}

int Code39Code32PARAFEnable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_REGULAR39,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x20,			//对应功能的设置位标识
									&nCode39E32Tab	//依据指令顺序排列的功能设置值
									);
}

//设置Code39 是否添加Code32起始符	Tag:C39S32
const unsigned int nCode39S32Tab[2] = { 0,0x10 };
int Code39AddSTXOfCode32Enable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_REGULAR39,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x10,			//对应功能的设置位标识
									&nCode39S32Tab	//依据指令顺序排列的功能设置值
									);
}

int Code39AddSTXOfCode32Enable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_REGULAR39,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x10,			//对应功能的设置位标识
									&nCode39S32Tab	//依据指令顺序排列的功能设置值
									);
}

//设置Code39 是否添加Code32起始符/终止符	Tag:C39T32
int Code32TrsmtSTXETXEnable_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}
int Code32TrsmtSTXETXEnable_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}    

//设置Code39 是否添加Code32校验符	Tag:C39C32
int Code32ChkTrsmt_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}
int Code32ChkTrsmt_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}

//设置Code39 CODEID的输出使能、禁止	Tag：CID013
int Code39CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_REGULAR39);
}

int Code39CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_REGULAR39);
}

/****************************************************************************
*@Codabar
****************************************************************************/
//加载Codabar的默认设置	Tag:CBADEF
int CodabarLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SaveAndUpdateAll;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));

	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] = u1d.ExMsg[POS_CODABAR];// = Init_CODABAR;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_CODABAR));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_CODABAR));// |= (0x01 << POS_CODABAR);
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int CodabarLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//设置Codabar的使能、禁止	Tag：CBAENA
int CodabarEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_CODABAR);
}

int CodabarEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_CODABAR);
}

//设置Codabar的识读最小长度	Tag：CBAMIN
int CodabarMinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_CODABAR);
}

int CodabarMinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_CODABAR);
}

//设置Codabar的识读最大长度	Tag：CBAMAX
int CodabarMaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_CODABAR);
}

int CodabarMaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_CODABAR);
}

//设置Codabar校验位的使能、禁止	Tag：CBACHK
const unsigned int nCodebarTrsmtTab[3] = { 0,0x01,0x03};
int CodabarChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODABAR,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nCodebarTrsmtTab	//依据指令顺序排列的功能设置值
									);
}

int CodabarChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODABAR,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nCodebarTrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置Codabar传送起始符与终止符的使能、禁止	Tag：CBATSC
const unsigned int nCodabarTSCTab[2] = { 0, 0x04 };
int CodabarTrsmtSTXETXEnable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODABAR,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nCodabarTSCTab	//依据指令顺序排列的功能设置值
									);
}

int CodabarTrsmtSTXETXEnable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODABAR,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nCodabarTSCTab	//依据指令顺序排列的功能设置值
									);
}

//设置Codabar起始符与终止符的格式	Tag：CBASCF
const unsigned int nCodabarSCFTab[4] = {0, BIT_CODA_ABCDTNXE ,BIT_CODA_LOWERCASE, BIT_CODA_ABCDTNXE | BIT_CODA_LOWERCASE };
int CodabarSTXETXFormats_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODABAR,
									0,				//协议{Data}的最小值
									3,				//协议{Data}的最大值
									BIT_CODA_ABCDTNXE | BIT_CODA_LOWERCASE,			//对应功能的设置位标识
									&nCodabarSCFTab	//依据指令顺序排列的功能设置值
									);
}

int CodabarSTXETXFormats_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODABAR,
									0,				//协议{Data}的最小值
									3,				//协议{Data}的最大值
									BIT_CODA_ABCDTNXE | BIT_CODA_LOWERCASE,			//对应功能的设置位标识
									&nCodabarSCFTab	//依据指令顺序排列的功能设置值
									);
}

//设置Codabar的CLSI编辑的使能、禁止	Tag:CBACLS
int CodabarCLSIEdit_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int CodabarCLSIEdit_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置Codabar CODEID的输出使能、禁止	Tag：CID015
int CodabarCodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_CODABAR);
}

int CodabarCodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_CODABAR);
}

/****************************************************************************
*@Code93
****************************************************************************/
//加载Code93的默认设置	Tag:C93DEF
int Code93LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
      
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] = u1d.ExMsg[POS_CODE93];// = Init_CODE93;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_CODE93));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_CODE93));
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int Code93LoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}


//设置Code93的使能、禁止	Tag：C93ENA
int Code93Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_CODE93);
}

int Code93Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_CODE93);
}

//设置Code93的识读最小长度	Tag：C93MIN
int Code93MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_CODE93);
}
int Code93MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_CODE93);
}

//设置Code93的识读最大长度	Tag：C93MAX
int Code93MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_CODE93);
}

int Code93MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_CODE93);
}

//设置Code93校验位的使能、禁止	Tag：C93CHK
const unsigned int nCode93TrsmtTab[3] = { 0, 0x01,0x03 };
int Code93ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODE93,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nCode93TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

int Code93ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODE93,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nCode93TrsmtTab	//依据指令顺序排列的功能设置值
									);
}


//设置Code93 CODEID的输出使能、禁止	Tag：CID017
int Code93CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_CODE93);
}

int Code93CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_CODE93);
}

/****************************************************************************
*@GS1-128(UCC/EAN-128)
****************************************************************************/
//加载GS1-128的默认设置	Tag:GS1DEF
int GS1_128LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SaveAndUpdateAll;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
      
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] = u1d.ExMsg[POS_UCCEAN128];// = Init_UCCEAN128;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UCCEAN128));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_UCCEAN128));// |= (0x01 << POS_UCCEAN128);
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int GS1_128LoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//设置GS1-128的使能、禁止	Tag：GS1ENA
int GS1_128Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_UCCEAN128);
}

int GS1_128Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_UCCEAN128);
}


//设置GS1-128的识读最小长度	Tag：GS1MIN
int GS1_128MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_UCCEAN128);
}

int GS1_128MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_UCCEAN128);
}

//设置GS1-128的识读最大长度	Tag：GS1MAX
int GS1_128MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_UCCEAN128);
}

int GS1_128MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_UCCEAN128);
}

//设置GS1-128校验位的使能、禁止	Tag：GS1CHK
//const unsigned int nGS1_128TrsmtTab[2] = { 0, eB1D_UCCEAN128_TRSMTCHK };
int GS1_128ChkTrsmt_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int GS1_128ChkTrsmt_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置GS1-128的FNC1输出的使能、禁止	Tag：GS1FNC
//const unsigned int c_nGS1_128FNC1Tab[2] = { 0, eB1D_UCCEAN128_FNC1 };
int GS1_128OutputFNC1_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int GS1_128OutputFNC1_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置GS1-128 CODEID的输出使能、禁止	Tag：CID003
int GS1_128CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_UCCEAN128);
}

int GS1_128CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_UCCEAN128);
}

/****************************************************************************
*@GS1 DataBar(RSS)
****************************************************************************/
//加载GS1 DataBar(RSS)的默认设置	Tag:RSSDEF
int RSSLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
      
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_RSS] = u1d.ExMsg[POS_RSS];// = Init_RSS;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_RSS));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_RSS));// |= (0x01 << POS_RSS);
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_RSS] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_RSS];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int RSSLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//设置GS1 DataBar(RSS)的使能、禁止	Tag:RSSENA
int RSSEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_RSS);
}

int RSSEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_RSS);
}


//设置GS1 DataBar(RSS)传送AI(01)字符的使能、禁止 Tag:RSSTAI
const unsigned int nRSSAITab[2] = { 0, 0x01 }; 
int RSSTrsmtAIEnable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_RSS,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nRSSAITab	//依据指令顺序排列的功能设置值
									);
}

int RSSTrsmtAIEnable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_RSS,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nRSSAITab	//依据指令顺序排列的功能设置值
									);
}

//设置GS1 DataBar(RSS) GS1 DataBar Omnidirectional字符的使能、禁止 Tag:RSSE14
int RSSConvert2RSS14_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int RSSConvert2RSS14_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置GS1 DataBar(RSS)的GS1 DataBar Limited使能、禁止 Tag:RSSENL
int RSSForGS1DatabarLimited_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int RSSForGS1DatabarLimited_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置GS1 DataBar(RSS)的GS1 DataBar Limited使能、禁止 Tag:RSSENE
int RSSForGS1DatabarExpand_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int RSSForGS1DatabarExpand_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置GS1 DataBar(RSS) CODEID的输出使能、禁止	Tag：CID031
int RSSCodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_RSS);
}

int RSSCodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_RSS);
}

/****************************************************************************
*@GS1 Composite
****************************************************************************/
//加载GS1 Composite的默认设置	Tag:CPTDEF
int GS1_CompositeLoadDefault_Set(sc_param_st *pParam)
{ 
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
      
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_COMPOSITE] = u1d.ExMsg[POS_COMPOSITE];// = Init_COMPOSITE;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_COMPOSITE));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_COMPOSITE));// &= ~(0x01 << POS_COMPOSITE);
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_COMPOSITE] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_COMPOSITE];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int GS1_CompositeLoadDefault_Get(sc_param_st *pParam){ return SC_RESULT_SUCC; }

//设置GS1 Composite的使能、禁止	Tag:CPTENA
int GS1_CompositeEnable_Set(sc_param_st *pParam)
{ 
	return __1DCodeEnable_Set(pParam, POS_COMPOSITE);
}

int GS1_CompositeEnable_Get(sc_param_st *pParam)
{ 
	return __1DCodeEnable_Get(pParam, POS_COMPOSITE);
}

//设置GS1 Composite的UPC/EAN版本的使能、禁止 Tag:CPTUPC
const unsigned int nComp4UPCEANTab[2] = {0,0x01};
int GS1_CompositeForUPC_EAN_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_COMPOSITE,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nComp4UPCEANTab	//依据指令顺序排列的功能设置值
									);
}
int GS1_CompositeForUPC_EAN_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_COMPOSITE,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nComp4UPCEANTab	//依据指令顺序排列的功能设置值
									);
}

//设置GS1 Composite CODEID的输出使能、禁止	Tag：CID030
int GS1_CompositeCodeID_Set(sc_param_st *pParam)
{ 
	return __1DCodeID_Set(pParam, POS_COMPOSITE);
}

int GS1_CompositeCodeID_Get(sc_param_st *pParam)
{ 
	return __1DCodeID_Get(pParam, POS_COMPOSITE);
}

/****************************************************************************
*@Code11 
****************************************************************************/
//加载Code11的默认设置		Tag:C11DEF
int Code11LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));

	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] = u1d.ExMsg[POS_CODE11];// = Init_CODE11;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_CODE11));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_CODE11));// &= ~(0x01 << POS_CODE11);
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int Code11LoadDefault_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_SUCC; 
}

//设置Code11的使能、禁止	Tag：C11ENA
int Code11Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_CODE11);
}

int Code11Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_CODE11);
}

//设置Code11的识读最小长度	Tag：C11MIN
int Code11MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_CODE11);
}

int Code11MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_CODE11);
}

//设置Code11的识读最大长度	Tag：C11MAX
int Code11MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_CODE11);
}
int Code11MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_CODE11);
}

//设置Code11校验位的使能、禁止及校验位的选择	Tag：C11CHK
const unsigned int nCode11TrsmtTab[6] = { 0x00,0x01,0x02,0x03,0x04,0x05 };					
int Code11ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODE11,
									0,				//协议{Data}的最小值
									5,				//协议{Data}的最大值
									0x07,			//对应功能的设置位标识
									&nCode11TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

int Code11ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODE11,
									0,				//协议{Data}的最小值
									5,				//协议{Data}的最大值
									0x07,			//对应功能的设置位标识
									&nCode11TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置Code11校验位是否传送	Tag：C11TCK		eB1D_CODE11_TRSMTCHK
const unsigned int nCode11TransACTTab[2] = {0x00, 0x08 };
int Code11ChkTrsmtAction_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODE11,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x08,			//对应功能的设置位标识
									&nCode11TransACTTab	//依据指令顺序排列的功能设置值
									);
}

int Code11ChkTrsmtAction_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODE11,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x08,			//对应功能的设置位标识
									&nCode11TransACTTab	//依据指令顺序排列的功能设置值
									);
}


//设置Code11 CODEID的输出使能、禁止	Tag：CID028
int Code11CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_CODE11);
}

int Code11CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_CODE11);
}

/****************************************************************************
*@ISBN
****************************************************************************/
//加载ISBN的默认设置	Tag:ISBDEF
int ISBNLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
      
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_ISBN] = u1d.ExMsg[POS_ISBN];// = Init_ISBN;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_ISBN));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_ISBN));// &= ~(0x01 << POS_ISBN);
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ISBN] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_ISBN];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int ISBNLoadDefault_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_SUCC;
}

//设置ISBN的使能、禁止	Tag：ISBENA
int ISBNEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_ISBN);
}

int ISBNEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_ISBN);
}

//设置ISBN的发送数字个数（10个或者13个）	Tag：ISBT10
const unsigned int nISBNLenTrmstTab[2] = {0x00,0x01};
int ISBNLenOfNUM_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_ISBN,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nCodebarTrsmtTab	//依据指令顺序排列的功能设置值
									);
}

int ISBNLenOfNUM_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_ISBN,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x01,			//对应功能的设置位标识
									&nCodebarTrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置ISBN 2位扩展码的识读使能、禁止	Tag:ISBAD2
//const unsigned int c_nISBNAddOn2Tab[2] = { 0, eB1D_ISBN_ADDON2 };
int ISBNAddCode2Enable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int ISBNAddCode2Enable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置ISBN 5位扩展码的识读使能、禁止	Tag:ISBAD5

int ISBNAddCode5Enable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int ISBNAddCode5Enable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置ISBN拓展码的使能、禁止	Tag:ISBNREQ

int ISBNAddedCodeEnable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
int ISBNAddedCodeEnable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置ISBN扩展码是否添加分隔符	Tag:ISBSEP
int ISBNSEPRInAddedCodeEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ISBNSEPRInAddedCodeEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置ISBN CODEID的输出使能、禁止	Tag：CID024
int ISBNCodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_ISBN);
}

int ISBNCodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_ISBN);
}

/****************************************************************************
*@Industrial 25
****************************************************************************/
//加载Industrial 25的默认设置		Tag:L25DEF
int IND25LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
      
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] = u1d.ExMsg[POS_INDUSTRIAL25];// = Init_Industrial25;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_INDUSTRIAL25));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_INDUSTRIAL25));// &= ~(0x01 << POS_INDUSTRIAL25);
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int IND25LoadDefault_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_SUCC;
}

//设置Industrial 25的使能、禁止	Tag：L25ENA
int IND25Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_INDUSTRIAL25);
}

int IND25Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_INDUSTRIAL25);
}

//设置Industrial 25的识读最小长度	Tag：L25MIN
int IND25MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_INDUSTRIAL25);
}

int IND25MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_INDUSTRIAL25);
}

//设置Industrial 25的识读最大长度	Tag：L25MAX
int IND25MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_INDUSTRIAL25);
}

int IND25MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_INDUSTRIAL25);
}

//设置Industrial 25校验位的使能、禁止及校验位的选择	Tag：L25CHK
const unsigned int nIND25TrsmtTab[3] = { 0, 0x01,0x03 };
int IND25ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_INDUSTRIAL25,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nIND25TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

int IND25ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_INDUSTRIAL25,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nIND25TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置Industrial 25 CODEID的输出使能、禁止	Tag：CID025
int IND25CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_INDUSTRIAL25);
}

int IND25CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_INDUSTRIAL25);
}

/****************************************************************************
*@Standard 25
****************************************************************************/
//加载Standard 25的默认设置		Tag:S25DEF
int STD25LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SaveAndUpdateAll;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
      
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] = u1d.ExMsg[POS_STANDARD25];// = Init_Standard25;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_STANDARD25));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_STANDARD25));// &= ~(0x01 << POS_STANDARD25);
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int STD25LoadDefault_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_SUCC;
}

//设置Standard 25的使能、禁止	Tag：S25ENA
int STD25Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_STANDARD25);
}

int STD25Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_STANDARD25);
}

//设置Standard 25的识读最小长度	Tag：S25MIN
int STD25MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_STANDARD25);
}

int STD25MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_STANDARD25);
}

//设置Standard 25的识读最大长度	Tag：S25MAX
int STD25MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_STANDARD25);
}
int STD25MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_STANDARD25);
}

//设置Standard 25校验位的使能、禁止及校验位的选择	Tag：S25CHK
const unsigned int nSTD25TransChkTab[3] = { 0, 0x01,0x03 };
int STD25ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_STANDARD25,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nSTD25TransChkTab	//依据指令顺序排列的功能设置值
									);
}

int STD25ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_STANDARD25,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nSTD25TransChkTab	//依据指令顺序排列的功能设置值
									);
}

//设置Standard 25 CODEID的输出使能、禁止	Tag：CID026
int STD25CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_STANDARD25);
}

int STD25CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_STANDARD25);
}

/****************************************************************************
*@Plessey
****************************************************************************/
//加载Plessey 的默认设置		Tag:PLYDEF
int PlesseyLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if(X_FacParamIsGet() != TRUE)
            return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] = u1d.ExMsg[POS_PLESSEY];// = Init_Plessey;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_PLESSEY));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_PLESSEY));// &= ~(0x01 << POS_PLESSEY);
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int PlesseyLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//设置Plessey 的使能、禁止	Tag：PLYENA
int PlesseyEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_PLESSEY);
}

int PlesseyEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_PLESSEY);
}

//设置Plessey 的识读最小长度	Tag：PLYMIN
int PlesseyMinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_PLESSEY);
}

int PlesseyMinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_PLESSEY);
}

//设置Plessey 的识读最大长度	Tag：PLYMAX
int PlesseyMaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_PLESSEY);
}

int PlesseyMaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_PLESSEY);
}

//设置Plessey 校验位的使能、禁止及校验位的传送	Tag：PLYCHK
const unsigned int nPlesseyTrsmtTab[3] = { 0,0x01,0x03 };
int PlesseyChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_PLESSEY,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nPlesseyTrsmtTab	//依据指令顺序排列的功能设置值
									);
}

int PlesseyChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_PLESSEY,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nPlesseyTrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置Plessey CODEID的输出使能、禁止	Tag：CID027
int PlesseyCodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_PLESSEY);
}

int PlesseyCodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_PLESSEY);
}

/****************************************************************************
*@MSI Plessey
****************************************************************************/
//加载MSI Plessey 的默认设置		Tag:MSIDEF
int MSIPlesseyLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SaveAndUpdateAll;

	 UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));


	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] = u1d.ExMsg[POS_MSI_PLESSEY];// = Init_MSIPlessey;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_MSI_PLESSEY));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_MSI_PLESSEY));// &= ~(0x01 << POS_MSI_PLESSEY);
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int MSIPlesseyLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//设置MSI Plessey 的使能、禁止	Tag：MSIENA
int MSIPlesseyEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_MSI_PLESSEY);
}
int MSIPlesseyEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_MSI_PLESSEY);
}

//设置MSI Plessey 的识读最小长度	Tag：MSIMIN
int MSIPlesseyMinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_MSI_PLESSEY);
}

int MSIPlesseyMinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_MSI_PLESSEY);
}

//设置MSI Plessey 的识读最大长度	Tag：MSIMAX
int MSIPlesseyMaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_MSI_PLESSEY);
}

int MSIPlesseyMaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_MSI_PLESSEY);
}

//设置MSI Plessey 校验位的使能、禁止及校验位的选择	Tag：MSICHK
const unsigned int nMSITrsmtTab[4] = { 0x00,0x01,0x02,0x03 };
int MSIPlesseyChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_MSI_PLESSEY,
									0,				//协议{Data}的最小值
									3,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nMSITrsmtTab	//依据指令顺序排列的功能设置值
									);
}
int MSIPlesseyChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_MSI_PLESSEY,
									0,				//协议{Data}的最小值
									3,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nMSITrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置MSI Plessey 校验位是否发送	Tag：MSITCK
const unsigned int nMSITrsmtACTTab[2] = { 0, 0x04 };
int MSIPlesseyChkTrsmtAction_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_MSI_PLESSEY,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nMSITrsmtACTTab	//依据指令顺序排列的功能设置值
									);
}
int MSIPlesseyChkTrsmtAction_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_MSI_PLESSEY,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									0x04,			//对应功能的设置位标识
									&nMSITrsmtACTTab	//依据指令顺序排列的功能设置值
									);
}

//设置MSI Plessey CODEID的输出使能、禁止	Tag：CID029
int MSIPlesseyCodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_MSI_PLESSEY);
}

int MSIPlesseyCodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_MSI_PLESSEY);
}


/****************************************************************************
*@ISSN
****************************************************************************/
//加载ISSN的默认设置	Tag:ISSDEF
int ISSNLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));

	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_ISSN] = u1d.ExMsg[POS_ISSN];// = Init_ISSN;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_ISSN));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_ISSN));// |= (0x01 << POS_ISSN);
	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ISSN] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_ISSN];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int ISSNLoadDefault_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_SUCC;
}

//设置ISSN的使能、禁止	Tag：ISSENA
int ISSNEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_ISSN);
}
int ISSNEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_ISSN);
}

//设置ISSN 2位扩展码的识读使能、禁止	Tag:ISSAD2
//const unsigned int c_nISSNAddOn2Tab[2] = { 0, eB1D_ISSN_ADDON2 };
int ISSNAddCode2Enable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int ISSNAddCode2Enable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置ISSN 5位扩展码的识读使能、禁止	Tag:ISSAD5
//const unsigned int c_nISSNAddOn5Tab[2] = { 0, eB1D_ISSN_ADDON5 };
int ISSNAddCode5Enable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
int ISSNAddCode5Enable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置ISSN拓展码的使能、禁止	Tag:ISSREQ
//const unsigned int c_nISSNREQAddTab[2] = { 0, eB1D_ISSN_REQ2ADDON | eB1D_ISSN_REQ5ADDON };
int ISSNAddedCodeEnable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int ISSNAddedCodeEnable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置ISSN扩展码是否添加分隔符	Tag:ISSSEP
int ISSNSEPRInAddedCodeEnable_Set(sc_param_st *pParam)
{ 
	return SC_RESULT_UnSurpport;
}

int ISSNSEPRInAddedCodeEnable_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_UnSurpport; 
}

//设置ISSN CODEID的输出使能、禁止	Tag：CID023
int ISSNCodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_ISSN);
}

int ISSNCodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_ISSN);
}

/****************************************************************************
*@China Post 25
****************************************************************************/
//加载China Post 25的默认设置	Tag:CHPDEF
int CP25LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;

	UTDec1DSetup u1d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;
         
      memcpy(&u1d, FacParamBuf+Start_MEMSize_1D, sizeof(UTDec1DSetup));

	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] = u1d.ExMsg[POS_CHNPOST];// = Init_CHNPOST25;
	X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_CHNPOST));
	X_paramGetGlobalAllParam()->p1D->unDecTypes |= (u1d.unDecTypes & (0x01 << POS_CHNPOST));// |= (0x01 << POS_CHNPOST);

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] = X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST];
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = X_paramGetGlobalAllParam()->p1D->unDecTypes;
	}

	return nResult;
}

int CP25LoadDefault_Get(sc_param_st *pParam)
{ return SC_RESULT_SUCC; }

//设置China Post 25的使能、禁止	Tag：CHPENA
int CP25Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_CHNPOST);
}
int CP25Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_CHNPOST);
}

//设置China Post 25 的识读最小长度	Tag：CHPMIN
int CP25MinLen_Set(sc_param_st *pParam)
{ 
	return __1DCodeMinLen_Set(pParam, POS_CHNPOST);
}

int CP25MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_CHNPOST);
}

//设置China Post 25 的识读最大长度	Tag：CHPMAX
int CP25MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_CHNPOST);
}
int CP25MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_CHNPOST);
}

//设置China Post 25 校验位的使能、禁止	Tag：CHPCHK
const int nCP25TrsmtTab[3] = {0x0,0x01,0x03};
int CP25ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CHNPOST,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nCP25TrsmtTab	//依据指令顺序排列的功能设置值
									);
}
int CP25ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CHNPOST,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									0x03,			//对应功能的设置位标识
									&nCP25TrsmtTab	//依据指令顺序排列的功能设置值
									);
}

//设置China Post 25 CODEID的输出使能、禁止	Tag：CID019
int CP25CodeID_Set(sc_param_st *pParam)
{
	return __1DCodeID_Set(pParam, POS_CHNPOST);
}
int CP25CodeID_Get(sc_param_st *pParam)
{
	return __1DCodeID_Get(pParam, POS_CHNPOST);
}

/****************************************************************************
*@AIM 128
****************************************************************************/
//加载AIM 128的默认设置	Tag:AIMDEF
int AIM128LoadDefault_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128LoadDefault_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置AIM 128的使能、禁止	Tag：AIMENA
int AIM128Enable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128Enable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置AIM 128的识读最小长度	Tag：AIMMIN
int AIM128MinLen_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128MinLen_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置AIM 128的识读最大长度	Tag：AIMMAX
int AIM128MaxLen_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128MaxLen_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置AIM 128校验位的发送使能、禁止	Tag：AIMCHK
//const unsigned int c_nAIM128TransChkTab[2] = { 0, eB1D_AIM128_TRSMTCHK };
int AIM128ChkTrsmt_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128ChkTrsmt_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置AIM 128 FNC1格式的使能、禁止	Tag：AIMFNC
//const unsigned int c_nAIM128FNC1Tab[2] = { 0, eB1D_AIM128_FNC1 };
int AIM128FNC1_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128FNC1_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置AIM 128 CODEID的输出使能、禁止	Tag：CID020
int AIM128CodeID_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128CodeID_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

/****************************************************************************
*@ISBT 128
****************************************************************************/
//加载ISBT 128的默认设置	Tag:IBTDEF
int ISBT128LoadDefault_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int ISBT128LoadDefault_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置ISBT 128的使能、禁止	Tag：IBTENA
int ISBT128Enable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int ISBT128Enable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置ISBT 128 CODEID的输出使能、禁止	Tag：CID021
int ISBT128CodeID_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int ISBT128CodeID_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

/****************************************************************************
*@COOP 25
****************************************************************************/
//加载COOP 25的默认设置	Tag:COPDEF
int COOP25LoadDefault_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int COOP25LoadDefault_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置COOP 25的使能、禁止	Tag：COPENA
int COOP25Enable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int COOP25Enable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置COOP 25的识读最小长度	Tag：COPMIN
int COOP25MinLen_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int COOP25MinLen_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置COOP 25的识读最大长度	Tag：COPMAX
int COOP25MaxLen_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int COOP25MaxLen_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置COOP 25校验位的发送使能、禁止	Tag：COPCHK
//const unsigned int c_nCOOP25TransChkTab[2] = { 0, eB1D_COOP25_TRSMTCHK };
int COOP25ChkTrsmt_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int COOP25ChkTrsmt_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置COOP 25 CODEID的输出使能、禁止	Tag：CID022
int COOP25CodeID_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int COOP25CodeID_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

/****************************************************************************
*@Deutsche 14
****************************************************************************/
//加载Deutsche 14的默认设置	Tag:D14DEF
int Deut14LoadDefault_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int Deut14LoadDefault_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置Deutsche 14的使能、禁止	Tag：D14ENA
int Deut14Enable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int Deut14Enable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置Deutsche 14校验位的发送使能、禁止	Tag：D14CHK
//const unsigned int c_nDP14TransChkTab[2] = { 0, eB1D_DP14_TRSMTCHK };
int Deut14ChkTrsmt_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int Deut14ChkTrsmt_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置Deutsche 14 CODEID的输出使能、禁止	Tag：CID128
int Deut14CodeID_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int Deut14CodeID_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

/****************************************************************************
*@Deutsche 12
****************************************************************************/
//加载Deutsche 12的默认设置	Tag:D12DEF
int Deut12LoadDefault_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int Deut12LoadDefault_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置Deutsche 12的使能、禁止	Tag：D12ENA
int Deut12Enable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int Deut12Enable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置Deutsche 12校验位的发送使能、禁止	Tag：D12CHK
//const unsigned int c_nDP12TransChkTab[2] = { 0, eB1D_DP12_TRSMTCHK };
int Deut12ChkTrsmt_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int Deut12ChkTrsmt_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置Deutsche 12 CODEID的输出使能、禁止	Tag：CID129
int Deut12CodeID_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int Deut12CodeID_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}


/****************************************************************************
*@Telepen
****************************************************************************/
//加载Telepen 的默认设置	Tag:TLPDEF
int TelepenLoadDefault_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenLoadDefault_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置Telepen 的使能、禁止	Tag：TLPENA
int TelepenEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置Telepen 的识读最小长度	Tag：TLPMIN
int TelepenMinLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenMinLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置Telepen 的识读最大长度	Tag：TLPMAX
int TelepenMaxLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenMaxLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置Telepen 校验位的发送使能、禁止	Tag：TLPCHK
int TelepenChkTrsmt_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenChkTrsmt_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置Telepen 的解译输出组合：Full ASCII与Numeric Only	Tag：TLPASC
int TelepenFullASCIIWithNumericOnly_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenFullASCIIWithNumericOnly_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置Telepen CODEID的输出使能、禁止	Tag：CID130
int TelepenCodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenCodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/***************************************************************************
*@宏条码
****************************************************************************/
//清空宏缓存数据
int MacroCacheCleared_Set(sc_param_st *pParam)
{ 
    int  nResult = SC_RESULT_Save;
    
    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam -> nDataLen > 0)
    { 
        return SC_RESULT_FAIL; 
    }

    W_macroInitialParam();

    return nResult;
}

int MacroCacheCleared_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}


//设置条码信息输出的发送方式
int MacroOutputMode1_Set(sc_param_st *pParam)
{

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam -> nDataLen != 0)
    { 
        return SC_RESULT_FAIL; 
    }
    X_paramGetGlobalAllParam()->p2D->nMacro = 1;
    return SC_RESULT_Save;
}
int MacroOutputMode1_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}
int MacroOutputMode2_Set(sc_param_st *pParam) 
{

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam -> nDataLen != 0)
    { 
        return SC_RESULT_FAIL; 
    }
    X_paramGetGlobalAllParam()->p2D->nMacro = 2;
    return SC_RESULT_Save;
}
int MacroOutputMode2_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}
int MacroOutputMode3_Set(sc_param_st *pParam) 
{

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam -> nDataLen != 0)
    { 
        return SC_RESULT_FAIL; 
    }
    X_paramGetGlobalAllParam()->p2D->nMacro = 3;
    return SC_RESULT_Save;
}
int MacroOutputMode3_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}
/****************************************************************************
*@2D条码设置函数                                                                      
****************************************************************************/
int __2DCodeParamValue_Set(sc_param_st *pParam,
							unsigned short nCodeType,
							unsigned int nDataMin,			//协议{Data}的最小值
							unsigned int nDataMax,			//协议{Data}的最大值
							unsigned int nSetBitMsk,		//对应功能的设置位标识
							void		*pSetTal			//依据指令顺序排列的功能设置值
							)
{
	int  nVal, nResult = SC_RESULT_Save;

#if _ENABLE_EXTFUNC_DBG_
	char Buff[16];
#endif
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

      if(!pParam->nDataLen)
      {
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < nDataMin || nVal > nDataMax)
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->p2D->ExParam[nCodeType] &= (~nSetBitMsk);

	X_paramGetGlobalAllParam()->p2D->ExParam[nCodeType] |= *((unsigned int *)pSetTal + nVal);
    
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p2D->ExParam[nCodeType] = X_paramGetGlobalAllParam()->p2D->ExParam[nCodeType];
	}

	return nResult;
}

int __2DCodeParamValue_Get(sc_param_st *pParam,
							unsigned short nCodeType,
							unsigned int nDataMin,			//协议{Data}的最小值
							unsigned int nDataMax,			//协议{Data}的最大值
							unsigned int nSetBitMsk,		//对应功能的设置位标识
							void		*pSetTal			//依据指令顺序排列的功能设置值
							)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	int	 nIndex = 0;
	char cBuff[8];
	UTDec2DSetup p2d;

#if _ENABLE_EXTFUNC_DBG_
	char Buff[16];
#endif 

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS://RAM
		nVal = X_paramGetGlobalAllParam()->p2D->ExParam[nCodeType] & nSetBitMsk;
		break;

	case SC_CFG_SYS://SYS
		nVal = X_paramBakGetGlobalAllParam()->p2D->ExParam[nCodeType] & nSetBitMsk;
		break;

	case SC_CFG_FACT://FAC
		if (X_FacParamIsGet() != TRUE)
		{
			return SC_RESULT_FAIL;
		}
		memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
		nVal = p2d.ExParam[nCodeType] & nSetBitMsk;
		break;

	default:
		return SC_RESULT_UnSurpport;
		break;
	}


	for (nIndex = nDataMin; nIndex < nDataMax; nIndex++)
	{
		if (nVal == ((unsigned int *)pSetTal)[nIndex])
		{
			break;
		}
	}

	if (nVal != ((unsigned int *)pSetTal)[nIndex])
	{
		return SC_RESULT_FAIL;
	}

	nLen = sprintf(cBuff, "%d", nIndex);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//开关设置、读取
int __2DCodeEnable_Set(sc_param_st *pParam, unsigned short nCodeType)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (nVal == 0)//禁止
	{
		X_paramGetGlobalAllParam()->p2D->unDecTypes &= (~(0x01 << nCodeType));
	}
	else//使能
	{
		X_paramGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << nCodeType);
	}

	//X_paramUpdateToDevice();
	X_decoSet2DParam(X_paramGetGlobalAllParam()->p2D);

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave; //SC_RESULT_UnsaveAndUpdateAll;
	}
	else
	{
	      if (nVal == 0)//禁止
		    X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= (~(0x01 << nCodeType));
            else
                X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << nCodeType);
	}
	return nResult;
}


int __2DCodeEnable_Get(sc_param_st *pParam, unsigned short nCodeType)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTDec2DSetup p2d;


	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

    if(!(pParam->nQueryType))
    {
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS:	//RAM
        		nVal = X_paramGetGlobalAllParam()->p2D->unDecTypes & (0x01 << nCodeType);
        		break;

        	case SC_CFG_SYS:	//SYS
        		nVal = X_paramBakGetGlobalAllParam()->p2D->unDecTypes & (0x01 << nCodeType);
        		break;

        	case SC_CFG_FACT:	//FAC
        		if (X_FacParamIsGet() != TRUE)
        		 return SC_RESULT_FAIL;

        		memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        		nVal = p2d.unDecTypes &  (0x01 << nCodeType);

        		break;
        	default:

        		return SC_RESULT_UnSurpport;
        		break;
        	}

        	nLen = sprintf(cBuff, "%d", nVal >> nCodeType);

        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
     }
     else
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-1");

	return nResult;

}

//最大最小长度设置、读取
int __2DCodeMinLen_Set(sc_param_st *pParam,unsigned short nCodeType)
{
	int  nVal, nResult = SC_RESULT_Save;
      unsigned int nMax;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = 4;
		return SC_RESULT_NEXT;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 9999)
	{
		return SC_RESULT_FAIL;
	}
	switch (nCodeType) {
	case POS_PDF417:
            nMax = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] & 0xffff0000) >> 16;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] & 0xffff0000) | (nVal);
		break;
	case POS_QR:
            nMax = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] & 0xffff0000) >> 16;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] & 0xffff0000) | (nVal);
		break;
	case POS_Aztec:
            nMax = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] & 0xffff0000) >> 16;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] & 0xffff0000) | (nVal);
		break;
	case POS_DataMatrix:
             nMax = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] & 0xffff0000) >> 16;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] & 0xffff0000) | (nVal);
		break;
	case POS_MaxiCode:
            nMax = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] & 0xffff0000) >> 16;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] & 0xffff0000) | (nVal);
		break;
	case POS_VeriCode:
            nMax = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] & 0xffff0000) >> 16;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] & 0xffff0000) | (nVal);
		break;
	case POS_HAN_SENSIBLE:
            nMax = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] & 0xffff0000) >> 16;
            if(nVal > nMax)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] & 0xff00ffff) | (nVal);
		break;
	default:
		return SC_RESULT_FAIL;
		break;
	}

	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[nCodeType] = X_paramGetGlobalAllParam()->p2D->ExMsg[nCodeType];
	}

	return nResult;
}

int __2DCodeMinLen_Get(sc_param_st *pParam, unsigned short nCodeType)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTDec2DSetup p2d;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

      if(!pParam->nQueryType)
      {
         switch (nCodeType) 
         {
        	case POS_PDF417:
        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_PDF417] & ~0xffff0000) & 0xfffff;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_QR:

        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_QR] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_QR] & ~0xffff0000) & 0xfffff;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_Aztec:

        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_Aztec] & ~0xffff0000) & 0xfffff;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_DataMatrix:

        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_DataMatrix] & ~0xffff0000) & 0xfffff;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_MaxiCode:
        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_MaxiCode] & ~0xffff0000) & 0xfffff;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_VeriCode:
        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_VeriCode] & ~0xffff0000) & 0xfffff;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_HAN_SENSIBLE:
        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] & ~0xffff0000) & 0xffff;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_HAN_SENSIBLE] & ~0xffff0000) & 0xfffff;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	default:
        		return SC_RESULT_FAIL;
        		break;
        	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}
      }
      else
      {	              
		nLen = sprintf(cBuff, "0-65535");
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}	
	}
	return nResult;
}

int __2DCodeMaxLen_Set(sc_param_st *pParam, unsigned short nCodeType)
{
	int  nVal, nResult = SC_RESULT_SaveAndUpdateAll;
      unsigned int nMin;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = 4;
		return SC_RESULT_NEXT;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 9999)
	{
		return SC_RESULT_FAIL;
	}
	switch (nCodeType) {
	case POS_PDF417:
            nMin = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] & 0x0000ffff);
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] & 0x0000ffff) | (nVal << 16);
		break;
	case POS_QR:
            nMin = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] & 0x0000ffff);
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] & 0x0000ffff) | (nVal << 16);
		break;
	case POS_Aztec:
            nMin = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] & 0x0000ffff);
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] & 0x0000ffff) | (nVal << 16);
		break;
	case POS_DataMatrix:
            nMin = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] & 0x0000ffff);
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] & 0x0000ffff) | (nVal << 16);
		break;
	case POS_MaxiCode:
            nMin = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] & 0x0000ffff);
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] & 0x0000ffff) | (nVal << 16);
		break;
	case POS_VeriCode:
            nMin = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] & 0x0000ffff);
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] & 0x0000ffff) | (nVal << 16);
		break;
	case POS_HAN_SENSIBLE:
            nMin = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] & 0x0000ffff);
            if(nVal < nMin)
                return SC_RESULT_FAIL;
		X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] & 0x0000ffff) | (nVal << 16);
		break;
	default:
		return SC_RESULT_FAIL;
		break;
	}

	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[nCodeType] = X_paramGetGlobalAllParam()->p2D->ExMsg[nCodeType];
	}

	return nResult;
}

int __2DCodeMaxLen_Get(sc_param_st *pParam, unsigned short nCodeType)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTDec2DSetup p2d;
#if _ENABLE_EXTFUNC_DBG_
	char Buff[32];
#endif

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
      if(!pParam->nQueryType)
      {
        switch (nCodeType) 
        {
        	case POS_PDF417:
        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_PDF417] & 0xffff0000) >> 16;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_QR:

        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_QR] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_QR] & 0xffff0000) >> 16;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_Aztec:

        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_Aztec] & 0xffff0000) >> 16;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_DataMatrix:

        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_DataMatrix] & 0xffff0000) >> 16;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_MaxiCode:
        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_MaxiCode] & 0xffff0000) >> 16;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_VeriCode:
        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_VeriCode] & 0xffff0000) >> 16;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	case POS_HAN_SENSIBLE:
        		if (pParam->nConfType == SC_CFG_NSYS)
        		{
        			nVal = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_SYS)
        		{
        			nVal = (X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] & 0xffff0000) >> 16;
        		}
        		else if (pParam->nConfType == SC_CFG_FACT)
        		{
        			if (X_FacParamIsGet() != TRUE)
        			{
        				return SC_RESULT_FAIL;
        			}
        			memcpy(&p2d, FacParamBuf + Start_MEMSize_2D, sizeof(UTDec2DSetup));
        			nVal = (p2d.ExMsg[POS_HAN_SENSIBLE] & 0xffff0000) >> 16;

        		}
        		else
        		{
        			return SC_RESULT_FAIL;
        		}
        		break;

        	default:
        		return SC_RESULT_FAIL;
        		break;
        	}

        	nLen = sprintf(cBuff, "%d", nVal);
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
              }
      else
      {

		nLen = sprintf(cBuff, "0-65535");
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}	
	}
	return nResult;
}

//Code ID设置、读取
int __2DCodeID_Set(sc_param_st *pParam, unsigned short nCodeType)
{
	int nResult = SC_RESULT_SaveAndUpdateAll;
      unsigned char ucID0, ucID1, temp1=0, temp2=0;
	//	char Buff[32];

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = 4;
		return SC_RESULT_NEXT;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

	if (pParam->nDataLen < 2 || pParam->nDataLen >4)
	{
		return SC_RESULT_FAIL;
	}

      if(pParam->pData[0] <= 0x39)
            temp1 = (pParam->pData[0] - 0x30)*16;
      else if(pParam->pData[0] <= 0x5a && pParam->pData[0] >= 0x41) 
            temp1 = (pParam->pData[0] - 0x41 + 10)*16;			
      else if(pParam->pData[0] <= 0x7a && pParam->pData[0] >= 0x61)	
            temp1 = (pParam->pData[0] - 0x61 + 10)*16;

      if(pParam->pData[1] <= 0x39)
            temp2 = (pParam->pData[1] - 0x30);
      else if(pParam->pData[1] <= 0x5a && pParam->pData[1] >= 0x41) 
            temp2 = (pParam->pData[1] - 0x41 + 10);			
      else if(pParam->pData[1] <= 0x7a && pParam->pData[1] >= 0x61)	
            temp2 = (pParam->pData[1] - 0x61 + 10);					
     
    ucID0 = temp1 + temp2;

	//if ((ucID0 > 0x40 && ucID0 < 0x5b) || (ucID0 > 0x60 && ucID0 < 0x7b))
	{
		X_paramGetGlobalAllParam()->pCode->by2DCodeID[nCodeType] = ucID0;
	}
      /*else
      {
		return SC_RESULT_FAIL;
	}*/

	if (pParam->nDataLen == 4)
	{
	      if(pParam->pData[2] <= 0x39)
            temp1 = (pParam->pData[2] - 0x30)*16;
            else if(pParam->pData[2] <= 0x5a && pParam->pData[2] >= 0x41) 
                temp1 = (pParam->pData[2] - 0x41 + 10)*16;			
            else if(pParam->pData[2] <= 0x7a && pParam->pData[2] >= 0x61)	
                temp1 = (pParam->pData[2] - 0x61 + 10)*16;

            if(pParam->pData[3] <= 0x39)
                temp2 = (pParam->pData[3] - 0x30);
            else if(pParam->pData[3] <= 0x5a && pParam->pData[3] >= 0x41) 
                temp2 = (pParam->pData[3] - 0x41 + 10);			
            else if(pParam->pData[3] <= 0x7a && pParam->pData[3] >= 0x61)	
                temp2 = (pParam->pData[3] - 0x61 + 10);
					   
            ucID1 = temp1 + temp2;
            //if ((ucID1 > 0x40 && ucID1 < 0x5b) || (ucID1 > 0x60 && ucID1 < 0x7b))
		    X_paramGetGlobalAllParam()->pCode2->by2DCodeID_2[nCodeType] = ucID1;
            //else
		//    return SC_RESULT_FAIL;
	}
	else
	{
		X_paramGetGlobalAllParam()->pCode2->by2DCodeID_2[nCodeType] = 0;
	}

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pCode->by2DCodeID[nCodeType] = X_paramGetGlobalAllParam()->pCode->by2DCodeID[nCodeType];
		X_paramBakGetGlobalAllParam()->pCode2->by2DCodeID_2[nCodeType] = X_paramGetGlobalAllParam()->pCode2->by2DCodeID_2[nCodeType];
	}

	return nResult;

}

int __2DCodeID_Get(sc_param_st *pParam,unsigned short nCodeType)
{
	unsigned char ucID0, ucID1;
	int  nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	CodeParam sCodeParam0;
	CodeParam2 sCodeParam1;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

      if(pParam->nQueryType)
      {  
             nLen = sprintf(cBuff, "01-FF");
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
             else
                return SC_RESULT_FAIL;
             return nResult;
      }

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS://RAM # + *
		ucID0 = X_paramGetGlobalAllParam()->pCode->by2DCodeID[nCodeType];
		ucID1 = X_paramGetGlobalAllParam()->pCode2->by2DCodeID_2[nCodeType];
		break;

	case SC_CFG_SYS://SYS @ + * 
		ucID0 = X_paramBakGetGlobalAllParam()->pCode->by2DCodeID[nCodeType];
		ucID1 = X_paramBakGetGlobalAllParam()->pCode2->by2DCodeID_2[nCodeType];
		break;

	case SC_CFG_FACT://FAC  @ +#  & 
		if (X_FacParamIsGet() != TRUE)
		{
			return SC_RESULT_FAIL;
		}
		memcpy(&sCodeParam0, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));
		memcpy(&sCodeParam1, FacParamBuf + Start_MEMSize_CODE2, sizeof(CodeParam2));

		ucID0 = sCodeParam0.by2DCodeID[nCodeType];
		ucID1 = sCodeParam1.by2DCodeID_2[nCodeType];
		break;

	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	if (!ucID1)
	{
		nLen = sprintf(cBuff, "%X", ucID0);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
	}
	else
	{
		nLen = sprintf(cBuff, "%X%X", ucID0, ucID1);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
	}
	return nResult;
}

/****************************************************************************
*@PDF417
****************************************************************************/
//加载PDF417 的默认设置	Tag:PDFDEF
int PDF417LoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SaveAndUpdateAll;

	UTDec2DSetup u2d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&u2d, FacParamBuf+Start_MEMSize_2D, sizeof(UTDec2DSetup));
      X_paramGetGlobalAllParam()->p2D->unDecTypes &= (~(0x01 << POS_PDF417));
	X_paramGetGlobalAllParam()->p2D->unDecTypes |= (u2d.unDecTypes & (0x01 << POS_PDF417));//  |= (0x01 << POS_PDF417);
	X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] = u2d.ExMsg[POS_PDF417];// = Init_2D_PDF417_MIN_MAX_LEN;
	X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] = u2d.ExParam[POS_PDF417];// = Init_PDF417;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = X_paramGetGlobalAllParam()->p2D->unDecTypes;
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] = X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417];
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] = X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417];
	}

	return nResult;
}

int PDF417LoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}
//设置PDF417 的使能、禁止	Tag：PDFENA
int PDF417Enable_Set(sc_param_st *pParam)
{ 
	return __2DCodeEnable_Set(pParam,POS_PDF417);
}

int PDF417Enable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_PDF417);
}

//设置PDF417 的识读最小长度	Tag：PDFMIN
int PDF417MinLen_Set(sc_param_st *pParam)
{ 
	return __2DCodeMinLen_Set(pParam, POS_PDF417);
}

int PDF417MinLen_Get(sc_param_st *pParam)
{ 
	return __2DCodeMinLen_Get(pParam, POS_PDF417);
}

//设置PDF417 的识读最大长度	Tag：PDFMAX
int PDF417MaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_PDF417);
}

int PDF417MaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_PDF417);
}

//设置PDF417 的单双码功能	Tag：PDFDOU
const int PDF417MulCodeTab[3] = {0x00,0x18,0x10};
int PDF417DecoMultiCode_Set(sc_param_st *pParam)
{ 
    
        return __2DCodeParamValue_Set(pParam,
									POS_PDF417,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_PDF417_DOUBLE,			//对应功能的设置位标识
									&PDF417MulCodeTab	//依据指令顺序排列的功能设置值
									);
      
}
int PDF417DecoMultiCode_Get(sc_param_st *pParam) 
{

    if(!pParam->nQueryType)
    {
	    return __2DCodeParamValue_Get(pParam,
									POS_PDF417,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_PDF417_DOUBLE,			//对应功能的设置位标识
									&PDF417MulCodeTab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }
}

//设置PDF417 的单双码输出方式	Tag：PDFDOF
int PDF417OrderOfMultiCode_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int PDF417OrderOfMultiCode_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_UnSurpport;
}

//设置PDF417 的正反相条码识读的使能、禁止	Tag：PDFINV
const int PDF417PhaseTab[3] = {0x0,0x10000,0x20000};
int PDF417PhaseOfCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_PDF417,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_PDF417_Reverse,			//对应功能的设置位标识
									&PDF417PhaseTab	//依据指令顺序排列的功能设置值
									);
}

int PDF417PhaseOfCode_Get(sc_param_st *pParam)
{ 
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_PDF417,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_PDF417_Reverse,			//对应功能的设置位标识
									&PDF417PhaseTab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }

}

//设置PDF417 的ECI输出使能、禁止	Tag：PDFECI
const int PDF417ECITab[2] = {DC_PDF417_ECICLOSE, 0x0};
int PDF417OutputECI_Set(sc_param_st *pParam) 
{
	return __2DCodeParamValue_Set(pParam,
									POS_PDF417,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_PDF417_ECICLOSE,			//对应功能的设置位标识
									&PDF417ECITab	//依据指令顺序排列的功能设置值
									);
	
}

int PDF417OutputECI_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_PDF417,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_PDF417_ECICLOSE,			//对应功能的设置位标识
									&PDF417ECITab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-1");        
        return SC_RESULT_SUCC;
    }
}

//设置PDF417 的字符编码方式	Tag：PDFENC
int PDF417EncodingOfChar_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int PDF417EncodingOfChar_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置PDF417 CODEID的输出使能、禁止	Tag：CID032
int PDF417CodeID_Set(sc_param_st *pParam)
{
	return __2DCodeID_Set(pParam, POS_PDF417);
}

int PDF417CodeID_Get(sc_param_st *pParam)
{
	return __2DCodeID_Get(pParam, POS_PDF417);
}


/****************************************************************************
*@QR
****************************************************************************/
//加载QR 的默认设置	Tag:QRCDEF
int QRLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SaveAndUpdateAll;

	 UTDec2DSetup u2d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;
         
      memcpy(&u2d, FacParamBuf+Start_MEMSize_2D, sizeof(UTDec2DSetup));
        X_paramGetGlobalAllParam()->p2D->unDecTypes &= (~(0x01 << POS_QR));
	X_paramGetGlobalAllParam()->p2D->unDecTypes |= (u2d.unDecTypes & (0x01 << POS_QR));// |= (0x01 << POS_QR);
	X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] = u2d.ExMsg[POS_QR];// = Init_2D_QRCODE_MIN_MAX_LEN;
	X_paramGetGlobalAllParam()->p2D->ExParam[POS_QR] = u2d.ExParam[POS_QR];// = Init_QR;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = X_paramGetGlobalAllParam()->p2D->unDecTypes;
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_QR] = X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR];
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] = X_paramGetGlobalAllParam()->p2D->ExParam[POS_QR];
	}

	return nResult;
}

int QRLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}
//设置QR 的使能、禁止	Tag：QRCENA
int QREnable_Set(sc_param_st *pParam)
{
	return __2DCodeEnable_Set(pParam, POS_QR);
}
int QREnable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_QR);
}

//设置QR 的识读最小长度	Tag：QRCMIN
int QRMinLen_Set(sc_param_st *pParam)
{
	return __2DCodeMinLen_Set(pParam, POS_QR);
}

int QRMinLen_Get(sc_param_st *pParam)
{
	return __2DCodeMinLen_Get(pParam, POS_QR);
}

//设置QR 的识读最大长度	Tag：QRCMAX
int QRMaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_QR);
}
int QRMaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_QR);
}

//设置QR 的单双码功能	Tag：QRCDOU
const int QRMulCodeTab[3] = { 0x00,0x08,0x10 };
int QRDecoMultiCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_QR,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_QR_DOUBLE,			//对应功能的设置位标识
									&QRMulCodeTab	//依据指令顺序排列的功能设置值
									);
}
int QRDecoMultiCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_QR,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_QR_DOUBLE,			//对应功能的设置位标识
									&QRMulCodeTab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }
}

//设置QR 的单双码输出方式	Tag：QRCDOF
int QROrderOfMultiCode_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport; 
}

int QROrderOfMultiCode_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_UnSurpport;
}

//设置QR 的Micro QR识读使能、禁止	Tag：QRCMCR
const int QRMicroTab[2] = {0x0,0x400000};	//0x400000 = 1 << POS_QR_MicroQR_ON
int QRDecoMicroQR_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
	return __2DCodeParamValue_Set(pParam,
									POS_QR,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									1 << POS_QR_MicroQR_ON,			//对应功能的设置位标识
									&QRMicroTab	//依据指令顺序排列的功能设置值
									);
}
int QRDecoMicroQR_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;
	return __2DCodeParamValue_Get(pParam,
									POS_QR,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									1 << POS_QR_MicroQR_ON,			//对应功能的设置位标识
									&QRMicroTab	//依据指令顺序排列的功能设置值
									);
}

//设置QR 的正反相条码识读的使能、禁止	Tag：QRCINV
int QRPhaseOfCode_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int QRPhaseOfCode_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//设置QR 的ECI输出使能、禁止	Tag：QRCECI
const int QRECITab[2] = { DC_QR_ECICLOSE,0x0 };
int QROutputECI_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_QR,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_QR_ECICLOSE,			//对应功能的设置位标识
									&PDF417ECITab	//依据指令顺序排列的功能设置值
									);
}
int QROutputECI_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_QR,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_QR_ECICLOSE,			//对应功能的设置位标识
									&PDF417ECITab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-1");        
        return SC_RESULT_SUCC;
    }
}


//设置QR 的字符编码方式	Tag：QRCENC
int QREncodingOfChar_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int QREncodingOfChar_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//设置QR CODEID的输出使能、禁止	Tag：CID033
int QRCodeID_Set(sc_param_st *pParam)
{
	return __2DCodeID_Set(pParam, POS_QR);
}

int QRCodeID_Get(sc_param_st *pParam)
{
	return __2DCodeID_Get(pParam, POS_QR);
}

/****************************************************************************
*@Aztec
****************************************************************************/
//加载Aztec 的默认设置	Tag:AZTDEF
int AztecLoadDefault_Set(sc_param_st *pParam)
{ 
	int  nResult = SC_RESULT_Save;

	UTDec2DSetup u2d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&u2d, FacParamBuf+Start_MEMSize_2D, sizeof(UTDec2DSetup));
    X_paramGetGlobalAllParam()->p2D->unDecTypes &= (~(0x01 << POS_Aztec));
	X_paramGetGlobalAllParam()->p2D->unDecTypes |= (u2d.unDecTypes & (0x01 << POS_Aztec));// |= (0x01 << POS_Aztec);
	X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] = u2d.ExMsg[POS_Aztec];// = Init_2D_AZTEC_MIN_MAX_LEN;
	X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] = u2d.ExParam[POS_Aztec];// = Init_Aztec;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = X_paramGetGlobalAllParam()->p2D->unDecTypes;
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] = X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec];
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] = X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec];
	}

	return nResult;
}

int AztecLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//设置Aztec 的使能、禁止	Tag：AZTENA
int AztecEnable_Set(sc_param_st *pParam)
{
	return __2DCodeEnable_Set(pParam, POS_Aztec);
}

int AztecEnable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_Aztec);
}

//设置Aztec 的识读最小长度	Tag：AZTMIN
int AztecMinLen_Set(sc_param_st *pParam)
{
	return __2DCodeMinLen_Set(pParam, POS_Aztec);
}
int AztecMinLen_Get(sc_param_st *pParam)
{
	return __2DCodeMinLen_Get(pParam, POS_Aztec);
}

//设置Aztec 的识读最大长度	Tag：AZTMAX
int AztecMaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_Aztec);
}
int AztecMaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_Aztec);
}

//设置Aztec 的多码同图的条码个数	Tag：AZTMUL
const int AZTECNumOfMultiTab[9] = {0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
int AztecNumOfMultiCode_Set(sc_param_st *pParam)
{
        return __2DCodeParamValue_Set(pParam,
									POS_Aztec,
									1,				//协议{Data}的最小值
									8,				//协议{Data}的最大值
									DC_AZTEC_MULTINUM,			//对应功能的设置位标识
									&AZTECNumOfMultiTab	//依据指令顺序排列的功能设置值
									);
 
    
}
int AztecNumOfMultiCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_Aztec,
									1,				//协议{Data}的最小值
									8,				//协议{Data}的最大值
									DC_AZTEC_MULTINUM,			//对应功能的设置位标识
									&AZTECNumOfMultiTab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "1-8");        
        return SC_RESULT_SUCC;
    }
    
}


//设置Aztec 的多码同图的处理方式Tag：AZTMOD
const int AZTECModeOfMultiTab[4] = { 0xFF,0x00,0x08,0x10 };
int AztecModeOfHandleMultiCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_Aztec,
									1,				//协议{Data}的最小值
									3,				//协议{Data}的最大值
									DC_AZTEC_MULTIEN,			//对应功能的设置位标识
									&AZTECModeOfMultiTab	//依据指令顺序排列的功能设置值
									);
}
int AztecModeOfHandleMultiCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_Aztec,
									1,				//协议{Data}的最小值
									3,				//协议{Data}的最大值
									DC_AZTEC_MULTIEN,			//对应功能的设置位标识
									&AZTECModeOfMultiTab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "1-3");        
        return SC_RESULT_SUCC;
    }
}

//设置Aztec 的ECI输出使能、禁止	Tag：AZTECI
const int AZTECECITab[2] = {DC_AZTEC_ECICLOSE,0x0};
int AztecOutputECI_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_Aztec,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_AZTEC_ECICLOSE,			//对应功能的设置位标识
									&AZTECECITab	//依据指令顺序排列的功能设置值
									);
}
int AztecOutputECI_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_Aztec,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_AZTEC_ECICLOSE,			//对应功能的设置位标识
									&AZTECECITab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-1");        
        return SC_RESULT_SUCC;
    }
}

//设置Aztec 的字符编码方式	Tag：AZTENC
int AztecEncodingOfChar_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int AztecEncodingOfChar_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//设置Aztec CODEID的输出使能、禁止	Tag：CID034
int AztecCodeID_Set(sc_param_st *pParam)
{
	return __2DCodeID_Set(pParam, POS_Aztec);
}

int AztecCodeID_Get(sc_param_st *pParam)
{
	return __2DCodeID_Get(pParam, POS_Aztec);
}

/****************************************************************************
*@Data Matrix
****************************************************************************/
//加载DM 的默认设置	Tag:DMCDEF
int DMLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;

	UTDec2DSetup u2d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&u2d, FacParamBuf+Start_MEMSize_2D, sizeof(UTDec2DSetup));

    X_paramGetGlobalAllParam()->p2D->unDecTypes &= (~(0x01 << POS_DataMatrix));
	X_paramGetGlobalAllParam()->p2D->unDecTypes |= (u2d.unDecTypes & (0x01 << POS_DataMatrix));//   |= (0x01 << POS_DataMatrix);
	X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] = u2d.ExMsg[POS_DataMatrix];// = Init_2D_DataMatrix_MIN_MAX_LEN;
	X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] = u2d.ExParam[POS_DataMatrix];//  = Init_DataMatrix;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = X_paramGetGlobalAllParam()->p2D->unDecTypes;
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] = X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix];
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] = X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix];
	}

	return nResult;
}

int DMLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}
//设置DM 的使能、禁止	Tag：DMCENA
int DMEnable_Set(sc_param_st *pParam)
{
	return __2DCodeEnable_Set(pParam, POS_DataMatrix);
}
int DMEnable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_DataMatrix);
}

//设置DM 的识读最小长度	Tag：DMCMIN
int DMMinLen_Set(sc_param_st *pParam)
{
	return __2DCodeMinLen_Set(pParam, POS_DataMatrix);
}
int DMMinLen_Get(sc_param_st *pParam)
{
	return __2DCodeMinLen_Get(pParam, POS_DataMatrix);
}

//设置DM 的识读最大长度	Tag：DMCMAX
int DMMaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_DataMatrix);
}

int DMMaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_DataMatrix);
} 

//设置DM 的单双码功能	Tag：DMCDOU
const int DMMulCodeTab[3] = { 0x00,0x08,0x10 };
int DMDecoMultiCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_DataMatrix,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_DM_DOUBLE,			//对应功能的设置位标识
									&DMMulCodeTab	//依据指令顺序排列的功能设置值
									);
}
int DMDecoMultiCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_DataMatrix,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_DM_DOUBLE,			//对应功能的设置位标识
									&DMMulCodeTab	//依据指令顺序排列的功能设置值
									);
    }    
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }
}


//设置DM 的识读矩形码功能	Tag：DMCREC
const int DMRectEnaTab[2] = {DC_DM_DISABLE_RECT,0x0};
int DMDecoRectCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_DataMatrix,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_DM_DISABLE_RECT,			//对应功能的设置位标识
									&DMRectEnaTab	//依据指令顺序排列的功能设置值
									);
}
int DMDecoRectCode_Get(sc_param_st *pParam)
{ 
    if(!pParam->nQueryType)
    {
	    return __2DCodeParamValue_Get(pParam,
									POS_DataMatrix,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_DM_DISABLE_RECT,			//对应功能的设置位标识
									&DMRectEnaTab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-1");        
        return SC_RESULT_SUCC;
    }

 
}

//设置DM 解码后是否添加信息码词	Tag：DMCADD
const int DMAddInfoTab[2] = { 0x0,DC_DM_ADD_PAD_CODE };
int DMAddInfoWordsToDecoResult_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_DataMatrix,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_DM_ADD_PAD_CODE,			//对应功能的设置位标识
									&DMAddInfoTab	//依据指令顺序排列的功能设置值
									);
}
int DMAddInfoWordsToDecoResult_Get(sc_param_st *pParam)
{
	return __2DCodeParamValue_Get(pParam,
									POS_DataMatrix,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_DM_ADD_PAD_CODE,			//对应功能的设置位标识
									&DMAddInfoTab	//依据指令顺序排列的功能设置值
									);
} 

//设置DM 的正反相条码识读的使能、禁止	Tag：DMCINV
const int DMPhaseTab[3] = { 0x0,0x10000,0x20000 };
int DMPhaseOfCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_DataMatrix,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_DM_Reverse,			//对应功能的设置位标识
									&DMPhaseTab	//依据指令顺序排列的功能设置值
									);
}
int DMPhaseOfCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_DataMatrix,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_DM_Reverse,			//对应功能的设置位标识
									&DMPhaseTab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }

  
}

//设置DM 的ECI输出使能、禁止	Tag：DMCECI
const int DMECITab[2] = {DC_DM_ECICLOSE ,0x0};
int DMOutputECI_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_DataMatrix,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_DM_ECICLOSE,			//对应功能的设置位标识
									&DMECITab	//依据指令顺序排列的功能设置值
									);
}
int DMOutputECI_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_DataMatrix,
									0,				//协议{Data}的最小值
									1,				//协议{Data}的最大值
									DC_DM_ECICLOSE,			//对应功能的设置位标识
									&DMECITab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-1");        
        return SC_RESULT_SUCC;
    }

  
}

//设置DM 的字符编码方式	Tag：DMCENC
int DMEncodingOfChar_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int DMEncodingOfChar_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置DM CODEID的输出使能、禁止	Tag：CID035
int DMCodeID_Set(sc_param_st *pParam)
{
	return __2DCodeID_Set(pParam, POS_DataMatrix);
}

int DMCodeID_Get(sc_param_st *pParam)
{
	return __2DCodeID_Get(pParam, POS_DataMatrix);
}

/****************************************************************************
*@Maxicode
****************************************************************************/
//加载Maxicode 的默认设置	Tag:MXCDEF
int MaxiLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;

	UTDec2DSetup u2d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&u2d, FacParamBuf+Start_MEMSize_2D, sizeof(UTDec2DSetup));
    X_paramGetGlobalAllParam()->p2D->unDecTypes &= (~(0x01 << POS_MaxiCode));
	X_paramGetGlobalAllParam()->p2D->unDecTypes |= (u2d.unDecTypes & (0x01 << POS_MaxiCode));//   |= (0x01 << POS_MaxiCode);
	X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] = u2d.ExMsg[POS_MaxiCode];// = Init_2D_MaxiCode_MIN_MAX_LEN;
      X_paramGetGlobalAllParam()->p2D->ExParam[POS_MaxiCode] = u2d.ExParam[POS_MaxiCode];
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = X_paramGetGlobalAllParam()->p2D->unDecTypes;
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] = X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode];
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_MaxiCode] = X_paramGetGlobalAllParam()->p2D->ExParam[POS_MaxiCode];
	}

	return nResult;
}

int MaxiLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
//设置Maxicode 的使能、禁止	Tag：MXCENA
int MaxiEnable_Set(sc_param_st *pParam)
{
	return __2DCodeEnable_Set(pParam, POS_MaxiCode);
}

int MaxiEnable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_MaxiCode);
}

//设置Maxicode 的识读最小长度	Tag：MXCMIN
int MaxiMinLen_Set(sc_param_st *pParam)
{
	return __2DCodeMinLen_Set(pParam, POS_MaxiCode);
}
int MaxiMinLen_Get(sc_param_st *pParam)
{
	return __2DCodeMinLen_Get(pParam, POS_MaxiCode);
}

//设置Maxicode 的识读最大长度	Tag：MXCMAX
int MaxiMaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_MaxiCode);
}
int MaxiMaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_MaxiCode);
}

//设置Maxicode CODEID的输出使能、禁止	Tag：CID036
int MaxiCodeID_Set(sc_param_st *pParam)
{
	return __2DCodeID_Set(pParam, POS_MaxiCode);
}

int MaxiCodeID_Get(sc_param_st *pParam)
{
	return __2DCodeID_Get(pParam, POS_MaxiCode);
}

/****************************************************************************
*@汉信码 CSC
****************************************************************************/
//加载CSC 的默认设置	Tag:CSCDEF
int CSCLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SaveAndUpdateAll;

	UTDec2DSetup u2d;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&u2d, FacParamBuf+Start_MEMSize_2D, sizeof(UTDec2DSetup));

      X_paramGetGlobalAllParam()->p2D->unDecTypes &= (~(0x01 << POS_HAN_SENSIBLE));
	X_paramGetGlobalAllParam()->p2D->unDecTypes |= (u2d.unDecTypes & (0x01 << POS_HAN_SENSIBLE));//   |= (0x01 << POS_HAN_SENSIBLE);

      X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] = u2d.ExMsg[POS_HAN_SENSIBLE];//  = Init_2D_HANSENSIBLE_MIN_MAX_LEN;
	X_paramGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] = u2d.ExParam[POS_HAN_SENSIBLE];// = Init_CSC;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = X_paramGetGlobalAllParam()->p2D->unDecTypes;
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] = X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE];
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] = X_paramGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE];
	}

	return nResult;
}

int CSCLoadDefault_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_UnSurpport;
}

//设置CSC 的使能、禁止	Tag：CSCENA
int CSCEnable_Set(sc_param_st *pParam)
{
	return __2DCodeEnable_Set(pParam, POS_HAN_SENSIBLE);
}

int CSCEnable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_HAN_SENSIBLE);
}

//设置CSC 的识读最小长度	Tag：CSCMIN
int CSCMinLen_Set(sc_param_st *pParam)
{
	return __2DCodeMinLen_Set(pParam, POS_HAN_SENSIBLE);
}

int CSCMinLen_Get(sc_param_st *pParam)
{
	return __2DCodeMinLen_Get(pParam, POS_HAN_SENSIBLE);
}

//设置CSC 的识读最大长度	Tag：CSCMAX
int CSCMaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_HAN_SENSIBLE);
}
int CSCMaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_HAN_SENSIBLE);
}

//设置CSC 的单双码功能	Tag：CSCDOU
const int CSCMulCodeTab[3] = { 0x00,0x08,0x10};
int CSCDecoMultiCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_HAN_SENSIBLE,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_CSC_DOUBLE,	//对应功能的设置位标识
									&CSCMulCodeTab	//依据指令顺序排列的功能设置值
									);
}
int CSCDecoMultiCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
	    return __2DCodeParamValue_Get(pParam,
									POS_HAN_SENSIBLE,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_CSC_DOUBLE,	//对应功能的设置位标识
									&CSCMulCodeTab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }

  
}

//设置CSC 的正反相条码识读的使能、禁止	Tag：CSCINV
const int CSCPhaseTab[3] = { 0x0,0x10000,0x20000 };
int CSCPhaseOfCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_HAN_SENSIBLE,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_DM_Reverse,			//对应功能的设置位标识
									&CSCPhaseTab	//依据指令顺序排列的功能设置值
									);
}
int CSCPhaseOfCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
	    return __2DCodeParamValue_Get(pParam,
									POS_HAN_SENSIBLE,
									0,				//协议{Data}的最小值
									2,				//协议{Data}的最大值
									DC_DM_Reverse,			//对应功能的设置位标识
									&CSCPhaseTab	//依据指令顺序排列的功能设置值
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }

  
}

//设置CSC CODEID的输出使能、禁止	Tag：CID039
int CSCCodeID_Set(sc_param_st *pParam)
{
	return __2DCodeID_Set(pParam, POS_HAN_SENSIBLE);
}

int CSCCodeID_Get(sc_param_st *pParam)
{
	return __2DCodeID_Get(pParam, POS_HAN_SENSIBLE);
}

/****************************************************************************
*@GM码
****************************************************************************/
//加载GM 的默认设置	Tag:GMCDEF
int GMLoadDefault_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GMLoadDefault_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//设置GM 的使能、禁止	Tag：GMCENA
int GMEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GMEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置GM 的识读最小长度	Tag：GMCMIN
int GMMinLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GMMinLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置GM 的识读最大长度	Tag：GMCMAX
int GMMaxLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GMMaxLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置GM CODEID的输出使能、禁止	Tag：CID041
int GMCodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GMCodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@MicroPDF417码
****************************************************************************/
//加载MicroPDF417 的默认设置	Tag:MPDDEF
int MicroPDF417LoadDefault_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置MicroPDF417 的使能、禁止	Tag：MPDENA
int MicroPDF417Enable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroPDF417Enable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置MicroPDF417 的识读最小长度	Tag：MPDMIN
int MicroPDF417MinLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroPDF417MinLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置MicroPDF417 的识读最大长度	Tag：MPDMAX
int MicroPDF417MaxLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroPDF417MaxLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置MicroPDF417 CODEID的输出使能、禁止	Tag：CID042
int MicroPDF417CodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroPDF417CodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroPDF417LoadDefault_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }


/****************************************************************************
*@MicroQR码
****************************************************************************/
//加载MicroQR 的默认设置	Tag:MQRDEF
int MicroQRLoadDefault_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroQRLoadDefault_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置MicroQR 的使能、禁止	Tag：MQRENA
int MicroQREnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroQREnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置MicroQR 的识读最小长度	Tag：MQRMIN
int MicroQRMinLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroQRMinLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置MicroQR 的识读最大长度	Tag：MQRMAX
int MicroQRMaxLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroQRMaxLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置MicroQR CODEID的输出使能、禁止	Tag：CID043
int MicroQRCodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroQRCodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@Specific OCR-B
****************************************************************************/
//加载Specific OCR-B 的默认设置	Tag:SOBDEF
int SpecOCRBLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;

	UTDecOCRSetup uOcr;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&uOcr, FacParamBuf+Start_MEMSize_2D, sizeof(UTDecOCRSetup));
      
/*#if defined(SAGEM)
	X_paramGetGlobalAllParam()->pOCR->unDecTypes |= (0x01 << POS_SPEC_OCR_B);
#else
	X_paramGetGlobalAllParam()->pOCR->unDecTypes &= ~(0x01 << POS_SPEC_OCR_B);
#endif*/
    X_paramGetGlobalAllParam()->pOCR->unDecTypes &= (~(0x01 << POS_SPEC_OCR_B));
      X_paramGetGlobalAllParam()->pOCR->unDecTypes |= (uOcr.unDecTypes & (0x01 << POS_SPEC_OCR_B));// &= ~(0x01 << POS_SPEC_OCR_B);
	X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = uOcr.ExMsg[POS_SPEC_OCR_B];//  = Init_SPEC_OCR_B;
	
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes = X_paramGetGlobalAllParam()->pOCR->unDecTypes;
		X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B];
	}
	return nResult;
}

int SpecOCRBLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
//设置Specific OCR-B 的使能、禁止	Tag：SOBENA
int SpecOCRBEnable_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->pData || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (nVal == 0)//禁止
	{
		X_paramGetGlobalAllParam()->pOCR->unDecTypes &= (~(0x01 << POS_SPEC_OCR_B));
	}
	else//使能
	{
		X_paramGetGlobalAllParam()->pOCR->unDecTypes |= (0x01 << POS_SPEC_OCR_B);
	}

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave; //SC_RESULT_UnsaveAndUpdateAll;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes = X_paramGetGlobalAllParam()->pOCR->unDecTypes;
	}
	return nResult;
}
int SpecOCRBEnable_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTDecOCRSetup pOcr;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pOCR->unDecTypes & (0x01 << POS_SPEC_OCR_B);
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pOCR->unDecTypes & (0x01 << POS_SPEC_OCR_B);
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pOcr, FacParamBuf + Start_MEMSize_OCR, sizeof(UTDecOCRSetup));
		nVal = pOcr.unDecTypes &  (0x01 << POS_SPEC_OCR_B);

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal >> POS_SPEC_OCR_B);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//设置Specific OCR-B 的识读最小长度	Tag：SOBMIN
int SpecOCRBMinLen_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_SaveAndUpdateAll;
	int  nTmp;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = 4;
		return SC_RESULT_NEXT;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 0x7f)
	{
		return SC_RESULT_FAIL;
	}

	nTmp = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] >> 24) & 0xff;
	if (nVal > nTmp)	//最小值 > 最大值
	{
		return SC_RESULT_FAIL;
	}
	switch (POS_SPEC_OCR_B)
	{
	case POS_SPEC_OCR_B:
		X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] & 0xff00ffff) | (nVal << 16);
		break;
	default:
		return SC_RESULT_FAIL;
		break;
	}

	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B];
	}

	return nResult;
}
int SpecOCRBMinLen_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTDecOCRSetup pOcr;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (POS_SPEC_OCR_B)
	{
	case POS_SPEC_OCR_B:
		if (pParam->nConfType == SC_CFG_NSYS)
		{
			nVal = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] & ~0xff00ffff) >> 16;
		}
		else if (pParam->nConfType == SC_CFG_SYS)
		{
			nVal = (X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] & ~0xff00ffff) >>16;
		}
		else if (pParam->nConfType == SC_CFG_FACT)
		{
			if (X_FacParamIsGet() != TRUE)
			{
				return SC_RESULT_FAIL;
			}
			memcpy(&pOcr, FacParamBuf + Start_MEMSize_OCR, sizeof(UTDecOCRSetup));
			nVal = (pOcr.ExMsg[POS_SPEC_OCR_B] & ~0xff00ffff) >> 16;

		}
		else
		{
			return SC_RESULT_FAIL;
		}
		break;

	default:
		return SC_RESULT_FAIL;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//设置Specific OCR-B 的识读最大长度	Tag：SOBMAX
int SpecOCRBMaxLen_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_SaveAndUpdateAll;
	int  nTmp;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = 4;
		return SC_RESULT_NEXT;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 0x7f)
	{
		return SC_RESULT_FAIL;
	}

	nTmp = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] >> 16) & 0xff;
	if (nVal < nTmp)	//最小值 > 最大值
	{
		return SC_RESULT_FAIL;
	}
	switch (POS_SPEC_OCR_B)
	{
	case POS_SPEC_OCR_B:
		X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] & 0x00ffffff) | (nVal << 24);
		break;
	default:
		return SC_RESULT_FAIL;
		break;
	}

	X_paramUpdateToDevice();
	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B];
	}

	return nResult;
}
int SpecOCRBMaxLen_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTDecOCRSetup pOcr;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (POS_SPEC_OCR_B)
	{
	case POS_SPEC_OCR_B:
		if (pParam->nConfType == SC_CFG_NSYS)
		{
			nVal = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] & ~0x00ffffff) >> 24;
		}
		else if (pParam->nConfType == SC_CFG_SYS)
		{
			nVal = (X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] & ~0x00ffffff) >> 24;
		}
		else if (pParam->nConfType == SC_CFG_FACT)
		{
			if (X_FacParamIsGet() != TRUE)
			{
				return SC_RESULT_FAIL;
			}
			memcpy(&pOcr, FacParamBuf + Start_MEMSize_OCR, sizeof(UTDecOCRSetup));
			nVal = (pOcr.ExMsg[POS_SPEC_OCR_B] & ~0x00ffffff) >> 24;

		}
		else
		{
			return SC_RESULT_FAIL;
		}
		break;

	default:
		return SC_RESULT_FAIL;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

int PassportOCRBLoadDefault_Get(sc_param_st *pParam) 
{
	return SC_RESULT_UnSurpport;
};


/*Code ID 固定 暂时不提供接口
//设置Specific OCR-B CODEID的输出使能、禁止	Tag：CID043
int SpecOCRBCodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int SpecOCRBCodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
*/

/****************************************************************************
*@Passport OCR
****************************************************************************/
//加载Passport OCR 的默认设置	Tag:PASDEF
int PassportOCRBLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SaveAndUpdateAll;

	UTDecOCRSetup uOcr;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&uOcr, FacParamBuf+Start_MEMSize_2D, sizeof(UTDecOCRSetup));
     X_paramGetGlobalAllParam()->pOCR->unDecTypes &= (~(0x01 << POS_PASSPORT_VISAS));
	X_paramGetGlobalAllParam()->pOCR->unDecTypes |= (uOcr.unDecTypes & (0x01 << POS_PASSPORT_VISAS));//  &= ~(0x01 << POS_PASSPORT_VISAS);

	X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_PASSPORT_VISAS] = uOcr.ExMsg[POS_PASSPORT_VISAS];// = Init_PASSPORT_VISAS;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes = X_paramGetGlobalAllParam()->pOCR->unDecTypes;
		X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_PASSPORT_VISAS] = X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_PASSPORT_VISAS];
	}
	return nResult;
}

//设置Passport OCR 的使能、禁止	Tag：PASENA
int PassportOCRBEnable_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (nVal == 0)//禁止
	{
		X_paramGetGlobalAllParam()->pOCR->unDecTypes &= (~(0x01 << POS_PASSPORT_VISAS));
	}
	else//使能
	{
		X_paramGetGlobalAllParam()->pOCR->unDecTypes |= (0x01 << POS_PASSPORT_VISAS);
	}

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave; //SC_RESULT_UnsaveAndUpdateAll;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes = X_paramGetGlobalAllParam()->pOCR->unDecTypes;
	}
	return nResult;
}
int PassportOCRBEnable_Get(sc_param_st *pParam)
{ 
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTDecOCRSetup pOcr;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pOCR->unDecTypes & (0x01 << POS_PASSPORT_VISAS);
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pOCR->unDecTypes & (0x01 << POS_PASSPORT_VISAS);
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pOcr, FacParamBuf + Start_MEMSize_OCR, sizeof(UTDecOCRSetup));
		nVal = pOcr.unDecTypes &  (0x01 << POS_PASSPORT_VISAS);

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal >> POS_PASSPORT_VISAS);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}


/*Code ID 固定 暂时不提供接口
//设置Passport OCR CODEID的输出使能、禁止	Tag：
int SpecOCRBCodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int SpecOCRBCodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
*/

/****************************************************************************
*@PostalCode系列条码设置函数
****************************************************************************/
int __PostalCodeParamValue_Set(sc_param_st *pParam,
								unsigned short nCodeType,
								unsigned int nDataMin,			//协议{Data}的最小值
								unsigned int nDataMax,			//协议{Data}的最大值
								unsigned int nSetBitMsk,		//对应功能的设置位标识
								void		*pSetTal			//依据指令顺序排列的功能设置值
								)
{
	int  nVal, nResult = SC_RESULT_Save;

#if _ENABLE_EXTFUNC_DBG_
	char Buff[16];
#endif
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < nDataMin || nVal > nDataMax)
	{
		return SC_RESULT_FAIL;
	}

#if _ENABLE_EXTFUNC_DBG_
	sprintf(Buff, "\r\nSv,ST:%x,%x\r\n", nVal, *((unsigned int *)pSetTal + nVal));
	Y_commSendString(Buff);
#endif

	X_paramGetGlobalAllParam()->pPostal->ExMsg[nCodeType] &= (~nSetBitMsk);
	X_paramGetGlobalAllParam()->pPostal->ExMsg[nCodeType] |= *((unsigned int *)pSetTal + nVal);

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[nCodeType] = X_paramGetGlobalAllParam()->pPostal->ExMsg[nCodeType];
	}

	return nResult;
}


int __PostalCodeParamValue_Get(sc_param_st *pParam,
								unsigned short nCodeType,
								unsigned int nDataMin,			//协议{Data}的最小值
								unsigned int nDataMax,			//协议{Data}的最大值
								unsigned int nSetBitMsk,		//对应功能的设置位标识
								void		*pSetTal			//依据指令顺序排列的功能设置值
								)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	int	 nIndex = 0;
	char cBuff[8];
	UTDecPostalSetup pPostal;

#if _ENABLE_EXTFUNC_DBG_
	char Buff[16];
#endif 

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
    
      if(!(pParam->nQueryType))
      {
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS://RAM
        		nVal = X_paramGetGlobalAllParam()->pPostal->ExMsg[nCodeType] & nSetBitMsk;
        		break;

        	case SC_CFG_SYS://SYS
        		nVal = X_paramBakGetGlobalAllParam()->pPostal->ExMsg[nCodeType] & nSetBitMsk;
        		break;

        	case SC_CFG_FACT://FAC
        		if (X_FacParamIsGet() != TRUE)
        		{
        			return SC_RESULT_FAIL;
        		}
        		memcpy(&pPostal, FacParamBuf + Start_MEMSize_Postal, sizeof(UTDecPostalSetup));
        		nVal = pPostal.ExMsg[nCodeType] & nSetBitMsk;
        		break;

        	default:
        		return SC_RESULT_UnSurpport;
        		break;
        	}


#if _ENABLE_EXTFUNC_DBG_	
        	sprintf(Buff, "\r\nBitVal:%x\r\n", nVal);
        	Y_commSendString(Buff);
#endif

        	for (nIndex = nDataMin; nIndex < nDataMax; nIndex++)
        	{
#if _ENABLE_EXTFUNC_DBG_
        		sprintf(Buff, "\r\nTableVal:%x\r\n", ((unsigned int *)pSetTal)[nIndex]);
        		Y_commSendString(Buff);
#endif
        		if (nVal == ((unsigned int *)pSetTal)[nIndex])
        		{
        			break;
        		}
        	}

        	if (nVal != ((unsigned int *)pSetTal)[nIndex])
        	{
        		return SC_RESULT_FAIL;
        	}

        	nLen = sprintf(cBuff, "%d", nIndex);
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
      }
      else
      {  
            *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-1");
      }
	return nResult;
}


int __PostalCodeEnable_Set(sc_param_st *pParam, unsigned short nCodeType)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (nVal == 0)//禁止
	{
		X_paramGetGlobalAllParam()->pPostal->unDecTypes &= (~(0x01 << nCodeType));
	}
	else//使能
	{
		X_paramGetGlobalAllParam()->pPostal->unDecTypes |= (0x01 << nCodeType);
	}

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave; //SC_RESULT_UnsaveAndUpdateAll;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = X_paramGetGlobalAllParam()->pPostal->unDecTypes;
	}
	return nResult;
}

int __PostalCodeEnable_Get(sc_param_st *pParam, unsigned short nCodeType)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	UTDecPostalSetup pPostal;

#if	_ENABLE_EXTFUNC_DBG_
	char Buff[16];
#endif

	if (!pParam->pOutBuf)
	{
		Y_commSendString("\r\nFalse1\r\n");
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pPostal->unDecTypes & (0x01 << nCodeType);
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pPostal->unDecTypes & (0x01 << nCodeType);
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pPostal, FacParamBuf + Start_MEMSize_Postal, sizeof(UTDecPostalSetup));
		nVal = pPostal.unDecTypes &  (0x01 << nCodeType);

		break;
	default:
#if _ENABLE_EXTFUNC_DBG_
		Y_commSendString("\r\nFalse2\r\n");
#endif
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal >> nCodeType);

#if _ENABLE_EXTFUNC_DBG_
	sprintf(Buff, "\r\nEnst:%x\r\n", nVal >> nCodeType);
	Y_commSendString(Buff);
#endif

	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
#if _ENABLE_EXTFUNC_DBG_
		Y_commSendString("EN1\r\n");
#endif
	}
	else
	{
		nResult = SC_RESULT_FAIL;
#if _ENABLE_EXTFUNC_DBG_
		Y_commSendString("EN2\r\n");
#endif
	}


	return nResult;

}


//Code ID设置、读取
int __PostalCodeID_Set(sc_param_st *pParam, unsigned short nCodeType)
{
	int nResult = SC_RESULT_SaveAndUpdateAll; 
      unsigned char ucID0, ucID1, temp1=0, temp2=0;
//	char Buff[32];

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
    	if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = 4;
		return SC_RESULT_NEXT;
	}
        
	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

	if (pParam->nDataLen < 2 || pParam->nDataLen > 4)
	{
		return SC_RESULT_FAIL;
	}

      if(pParam->pData[0] <= 0x39)
            temp1 = (pParam->pData[0] - 0x30)*16;
      else if(pParam->pData[0] <= 0x5a && pParam->pData[0] >= 0x41) 
            temp1 = (pParam->pData[0] - 0x41 + 10)*16;			
      else if(pParam->pData[0] <= 0x7a && pParam->pData[0] >= 0x61)	
            temp1 = (pParam->pData[0] - 0x61 + 10)*16;

      if(pParam->pData[1] <= 0x39)
            temp2 = (pParam->pData[1] - 0x30);
      else if(pParam->pData[1] <= 0x5a && pParam->pData[1] >= 0x41) 
            temp2 = (pParam->pData[1] - 0x41 + 10);			
      else if(pParam->pData[1] <= 0x7a && pParam->pData[1] >= 0x61)	
            temp2 = (pParam->pData[1] - 0x61 + 10);					
     
    ucID0 = temp1 + temp2;
     
    //if ((ucID0 > 0x40 && ucID0 < 0x5b) || (ucID0 > 0x60 && ucID0 < 0x7b))
    {
        X_paramGetGlobalAllParam()->pCode2->byPostalCodeID[nCodeType] = ucID0;
    }
    /*else
    {
        return SC_RESULT_FAIL;
    }*/

    if (pParam->nDataLen == 4)
    {
        if(pParam->pData[2] <= 0x39)
            temp1 = (pParam->pData[2] - 0x30)*16;
        else if(pParam->pData[2] <= 0x5a && pParam->pData[2] >= 0x41) 
            temp1 = (pParam->pData[2] - 0x41 + 10)*16;			
        else if(pParam->pData[2] <= 0x7a && pParam->pData[2] >= 0x61)	
            temp1 = (pParam->pData[2] - 0x61 + 10)*16;

        if(pParam->pData[3] <= 0x39)
            temp2 = (pParam->pData[3] - 0x30);
        else if(pParam->pData[3] <= 0x5a && pParam->pData[3] >= 0x41) 
            temp2 = (pParam->pData[3] - 0x41 + 10);			
        else if(pParam->pData[3] <= 0x7a && pParam->pData[3] >= 0x61)	
            temp2 = (pParam->pData[3] - 0x61 + 10);
					   
        ucID1 = temp1 + temp2;
    
        //if ((ucID1 > 0x40 && ucID1 < 0x5b) || (ucID1 > 0x60 && ucID1 < 0x7b))
        { 
            X_paramGetGlobalAllParam()->pCode2->byPostalCodeID_2[nCodeType] = ucID1;
        }
        /*else
        {
            return SC_RESULT_FAIL;
        }*/
            
    }
    else
    {
        X_paramGetGlobalAllParam()->pCode2->byPostalCodeID_2[nCodeType] = 0;
    }

    X_paramUpdateToDevice();
	
    if (!(pParam->bSaveFlag))
    {
        nResult = SC_RESULT_SuccUnsave;
    }
    else
    {
        X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID[nCodeType] = X_paramGetGlobalAllParam()->pCode2->byPostalCodeID[nCodeType];
        X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID_2[nCodeType] = X_paramGetGlobalAllParam()->pCode2->byPostalCodeID_2[nCodeType];
    }

    return nResult;

}


int __PostalCodeID_Get(sc_param_st *pParam, unsigned short nCodeType)
{
	unsigned char ucID0, ucID1;
	int  nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	CodeParam2 sCodeParam1;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

      if(pParam->nQueryType)
      {  
             nLen = sprintf(cBuff, "01-FF");
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
             else
                return SC_RESULT_FAIL;
             return nResult;
      }
	//ucID0 = '0';
	//ucID1 = '0';
	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS://RAM # + *

		ucID0 = X_paramGetGlobalAllParam()->pCode2->byPostalCodeID[nCodeType];
		ucID1 = X_paramGetGlobalAllParam()->pCode2->byPostalCodeID_2[nCodeType];
		break;

	case SC_CFG_SYS://SYS @ + * 
		ucID0 = X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID[nCodeType];
		ucID1 = X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID_2[nCodeType];
		break;

	case SC_CFG_FACT://FAC  @ +#  & 

		if (X_FacParamIsGet() != TRUE)
		{
			return SC_RESULT_FAIL;
		}

		memcpy(&sCodeParam1, FacParamBuf + Start_MEMSize_CODE2, sizeof(CodeParam2));

		ucID0 = sCodeParam1.byPostalCodeID[nCodeType];
		ucID1 = sCodeParam1.byPostalCodeID_2[nCodeType];
		break;

	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	if (!ucID1)
	{
		nLen = sprintf(cBuff, "%X", ucID0);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
	}
	else
	{
		nLen = sprintf(cBuff, "%X%X", ucID0, ucID1);
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
		else
		{
			nResult = SC_RESULT_FAIL;
		}
	}
	return nResult;
}
/****************************************************************************
*@USPS Postnet
****************************************************************************/
//加载USPS Postnet 的默认设置	Tag:PNTDEF
int PostnetLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDecPostalSetup uPostal;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&uPostal, FacParamBuf+Start_MEMSize_2D, sizeof(UTDecPostalSetup));
	
     X_paramGetGlobalAllParam()->pPostal->unDecTypes &= (~(0x01 << POS_POSTNET));
	X_paramGetGlobalAllParam()->pPostal->unDecTypes |= (uPostal.unDecTypes & (0x01 << POS_POSTNET));// &= ~(0x01 << POS_POSTNET);
	X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_POSTNET] = uPostal.ExMsg[POS_POSTNET];//= Init_POSTNET;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = X_paramGetGlobalAllParam()->pPostal->unDecTypes;
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_POSTNET] = X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_POSTNET];
	}

	return nResult;
}

int PostnetLoadDefault_Get(sc_param_st *pParam) 
{
	return SC_RESULT_UnSurpport;
}

//设置USPS Postnet 的使能、禁止	Tag：PNTENA
int PostnetEnable_Set(sc_param_st *pParam)
{ 
	return __PostalCodeEnable_Set(pParam, POS_POSTNET);
}

int PostnetEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_POSTNET);
}

//设置USPS Postnet 校验位的发送使能、禁止	Tag：PNTCHK
const int nPostnetTrsmtTab[3] = {0xFF,0x0,BIT_POSTNET_TRMTCHK };
int PostnetChkTrsmt_Set(sc_param_st *pParam)
{
	return __PostalCodeParamValue_Set(pParam,
										POS_POSTNET,
										1,				//协议{Data}的最小值
										2,				//协议{Data}的最大值
										BIT_POSTNET_TRMTCHK,			//对应功能的设置位标识
										&nPostnetTrsmtTab	//依据指令顺序排列的功能设置值
										);
}

int PostnetChkTrsmt_Get(sc_param_st *pParam)
{ 
	return __PostalCodeParamValue_Get(pParam,
										POS_POSTNET,
										1,				//协议{Data}的最小值
										2,				//协议{Data}的最大值
										BIT_POSTNET_TRMTCHK,			//对应功能的设置位标识
										&nPostnetTrsmtTab	//依据指令顺序排列的功能设置值
										);
}

//设置USPS Postnet CODEID的输出使能、禁止	Tag：CID096
int PostnetCodeID_Set(sc_param_st *pParam)
{ 
	return __PostalCodeID_Set(pParam, POS_POSTNET);
}

int PostnetCodeID_Get(sc_param_st *pParam)
{ 
	return __PostalCodeID_Get(pParam, POS_POSTNET);
}

/****************************************************************************
*@USPS Intelligent Mail
****************************************************************************/
//加载USPS Intelligent Mail 的默认设置	Tag:ILGDEF
int InteMailLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
      UTDecPostalSetup uPostal;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&uPostal, FacParamBuf+Start_MEMSize_2D, sizeof(UTDecPostalSetup));
    X_paramGetGlobalAllParam()->pPostal->unDecTypes &= (~(0x01 << POS_ONECODE));
	X_paramGetGlobalAllParam()->pPostal->unDecTypes |= (uPostal.unDecTypes & (0x01 << POS_ONECODE));
	X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_ONECODE] = uPostal.ExMsg[POS_ONECODE];//Init_ONECODE;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = X_paramGetGlobalAllParam()->pPostal->unDecTypes;
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_ONECODE] = X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_ONECODE];
	}

	return nResult;
}


int InteMailLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
//设置USPS Intelligent Mail 的使能、禁止	Tag：ILGENA
int InteMailEnable_Set(sc_param_st *pParam)
{
	return __PostalCodeEnable_Set(pParam, POS_ONECODE);
}

int InteMailEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_ONECODE);
}

//设置USPS Intelligent Mail CODEID的输出使能、禁止	Tag：CID097
int InteMailCodeID_Set(sc_param_st *pParam)
{ 
	return __PostalCodeID_Set(pParam, POS_ONECODE);
}

int InteMailCodeID_Get(sc_param_st *pParam)
{
	return __PostalCodeID_Get(pParam, POS_ONECODE);
}

/****************************************************************************
*@Royal Mail
****************************************************************************/
//加载Royal Mail 的默认设置	Tag:ROYDEF
int RoyalMailLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SaveAndUpdateAll;
      UTDecPostalSetup uPostal;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&uPostal, FacParamBuf+Start_MEMSize_2D, sizeof(UTDecPostalSetup));
     X_paramGetGlobalAllParam()->pPostal->unDecTypes &= (~(0x01 << POS_RM4SCC));
	X_paramGetGlobalAllParam()->pPostal->unDecTypes |= (uPostal.unDecTypes & (0x01 << POS_RM4SCC));
	X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_RM4SCC] = uPostal.ExMsg[POS_RM4SCC];//Init_RM4SCC;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = X_paramGetGlobalAllParam()->pPostal->unDecTypes;
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_RM4SCC] = X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_RM4SCC];
	}

	return nResult;
}

int RoyalMailLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
//设置Royal Mail 的使能、禁止	Tag：ROYENA
int RoyalMailEnable_Set(sc_param_st *pParam)
{ 
	return __PostalCodeEnable_Set(pParam, POS_RM4SCC);
}

int RoyalMailEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_RM4SCC);
}

//设置Royal Mail 的输出使能、禁止	Tag：CID098
int RoyalMailCodeID_Set(sc_param_st *pParam)
{ 
	return __PostalCodeID_Set(pParam, POS_RM4SCC);
}

int RoyalMailCodeID_Get(sc_param_st *pParam)
{
	return __PostalCodeID_Get(pParam, POS_RM4SCC);
}

/****************************************************************************
*@USPS Planet
****************************************************************************/
//加载USPS Planet 的默认设置	Tag:PLADEF
int PlanetLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;

	 UTDecPostalSetup uPostal;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&uPostal, FacParamBuf+Start_MEMSize_2D, sizeof(UTDecPostalSetup));
    X_paramGetGlobalAllParam()->pPostal->unDecTypes &= (~(0x01 << POS_PLANET));
	X_paramGetGlobalAllParam()->pPostal->unDecTypes |= (uPostal.unDecTypes & (0x01 << POS_PLANET));
	X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_PLANET] = uPostal.ExMsg[POS_PLANET];//Init_PLANET;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = X_paramGetGlobalAllParam()->pPostal->unDecTypes;
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_PLANET] = X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_PLANET];
	}

	return nResult;
}


int PlanetLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//设置USPS Planet 的使能、禁止	Tag：PLAENA
int PlanetEnable_Set(sc_param_st *pParam)
{
	return __PostalCodeEnable_Set(pParam, POS_PLANET);
}
int PlanetEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_PLANET);
}

//设置USPS Planet 校验位的发送使能、禁止	Tag：PLACHK
const int nPlanetTrsmtTab[3] = {0xFF,0x00,BIT_PLANET_TRMTCHK };
int PlanetChkTrsmt_Set(sc_param_st *pParam)
{
	return __PostalCodeParamValue_Set(pParam,
										POS_PLANET,
										1,				//协议{Data}的最小值
										2,				//协议{Data}的最大值
										BIT_PLANET_TRMTCHK,			//对应功能的设置位标识
										&nPlanetTrsmtTab	//依据指令顺序排列的功能设置值
										);
}

int PlanetChkTrsmt_Get(sc_param_st *pParam)
{
	return __PostalCodeParamValue_Get(pParam,
										POS_PLANET,
										1,				//协议{Data}的最小值
										2,				//协议{Data}的最大值
										BIT_PLANET_TRMTCHK,			//对应功能的设置位标识
										&nPlanetTrsmtTab	//依据指令顺序排列的功能设置值
										);
}

//设置USPS Planet CODEID的输出使能、禁止	Tag：CID099
int PlanetCodeID_Set(sc_param_st *pParam)
{
	return __PostalCodeID_Set(pParam, POS_PLANET);
}

int PlanetCodeID_Get(sc_param_st *pParam)
{
	return __PostalCodeID_Get(pParam, POS_PLANET);
}

/****************************************************************************
*@KIX Post
****************************************************************************/
//加载KIX Post 的默认设置	Tag:KIXDEF
int KIXPostLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;

	 UTDecPostalSetup uPostal;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&uPostal, FacParamBuf+Start_MEMSize_2D, sizeof(UTDecPostalSetup));
     X_paramGetGlobalAllParam()->pPostal->unDecTypes &= (~(0x01 << POS_KIXCODE));
	X_paramGetGlobalAllParam()->pPostal->unDecTypes |= (uPostal.unDecTypes & (0x01 << POS_KIXCODE));
	X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_KIXCODE] = uPostal.ExMsg[POS_KIXCODE];//Init_KIXCODE;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = X_paramGetGlobalAllParam()->pPostal->unDecTypes;
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_KIXCODE] = X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_KIXCODE];
	}

	return nResult;
}

int KIXPostLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
//设置KIX Post 的使能、禁止	Tag：KIXENA
int KIXPostEnable_Set(sc_param_st *pParam)
{ 
	return __PostalCodeEnable_Set(pParam,POS_KIXCODE);
}

int KIXPostEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_KIXCODE);
}

//设置KIX Post 的输出使能、禁止	Tag：CID100
int KIXPostCodeID_Set(sc_param_st *pParam)
{ 
	return __PostalCodeID_Set(pParam, POS_KIXCODE);
}

int KIXPostCodeID_Get(sc_param_st *pParam)
{ 
	return __PostalCodeID_Get(pParam, POS_KIXCODE);
}

/****************************************************************************
*@Australian Postal
****************************************************************************/
//加载Australian Postal 的默认设置	Tag:APLDEF
int AUSPostalLoadDefault_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;

	 UTDecPostalSetup uPostal;
      
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

      if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;

      memcpy(&uPostal, FacParamBuf+Start_MEMSize_2D, sizeof(UTDecPostalSetup));
      X_paramGetGlobalAllParam()->pPostal->unDecTypes &= (~(0x01 << POS_AUSPOST));
	X_paramGetGlobalAllParam()->pPostal->unDecTypes |= (uPostal.unDecTypes & (0x01 << POS_AUSPOST));
	X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_AUSPOST] = uPostal.ExMsg[POS_AUSPOST];//Init_AUSPOST;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = X_paramGetGlobalAllParam()->pPostal->unDecTypes;
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_AUSPOST] = X_paramGetGlobalAllParam()->pPostal->ExMsg[POS_AUSPOST];
	}

	return nResult;
}

int AUSPostalLoadDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
//设置Australian Postal 的使能、禁止	Tag：APLENA
int AUSPostalEnable_Set(sc_param_st *pParam)
{ 
	return __PostalCodeEnable_Set(pParam, POS_AUSPOST);
}
int AUSPostalEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_AUSPOST);
}

//设置Australian Postal 的输出使能、禁止	Tag：CID101
int AUSPostalCodeID_Set(sc_param_st *pParam)
{ 
	return __PostalCodeID_Set(pParam, POS_AUSPOST);
}

int AUSPostalCodeID_Get(sc_param_st *pParam)
{
	return __PostalCodeID_Get(pParam, POS_AUSPOST);
}

//前后缀等附加信息综合设置		Tag:APSENA
int AllFixMsg_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] = nVal;
	X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] = nVal;
	X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] = nVal;
	X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] = nVal;

	if (nVal == 0)//禁止
	{
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] &= (~0x01);
	}
	else//使能
	{
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] |= 0x01;
	}

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave; //SC_RESULT_UnsaveAndUpdateAll;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix];
		X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix];
		X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID];
		X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal];

		X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM];
	}
	return nResult;
}

int AllFixMsg_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] & 0x01;
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] & 0x01;
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pCodeParam, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));
		nVal = pCodeParam.IsPreSufOn[POS_IsAIM] &  0x01;

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//前缀顺序设置					Tag:PRESEQ
int PrefixOrder_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 6)		//order值 <= 6
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pCode->PrefixOrder = nVal;
	
	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave; 
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pCode->PrefixOrder = X_paramGetGlobalAllParam()->pCode->PrefixOrder;
	}
	return nResult;
}

int PrefixOrder_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
    if(!(pParam->nQueryType))
    {
      
        	switch (pParam->nConfType)
        	{
        	case SC_CFG_NSYS:	//RAM
        		nVal = X_paramGetGlobalAllParam()->pCode->PrefixOrder;
        		break;

        	case SC_CFG_SYS:	//SYS
        		nVal = X_paramBakGetGlobalAllParam()->pCode->PrefixOrder;
        		break;

        	case SC_CFG_FACT:	//FAC
        		if (X_FacParamIsGet() != TRUE)
        			return SC_RESULT_FAIL;

        		memcpy(&pCodeParam, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));
        		nVal = pCodeParam.PrefixOrder & 0x03;

        		break;
        	default:
        		return SC_RESULT_UnSurpport;
        		break;
        	}

            if(nVal != 0)
                nVal = 1;
        	nLen = sprintf(cBuff, "%d", nVal);
        	if (pParam->nOutBufSize >= nLen)
        	{
        		memcpy(pParam->pOutBuf, cBuff, nLen);
        		*pParam->pOutLen = nLen;
        	}
        	else
        	{
        		nResult = SC_RESULT_FAIL;
        	}
    }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-1");
    }
	return nResult;
}

//前缀使能设置				Tag:CPRENA
int PrefixEnable_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)		//0:禁止，1：使能
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] = nVal;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix];
	}
	return nResult;
}

int PrefixEnable_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] & 0x01;
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] & 0x01;
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pCodeParam, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));
		nVal = pCodeParam.IsPreSufOn[POS_IsPrefix] & 0x01;

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);

	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//前缀信息编辑			   Tag:CPRSET$
int PrefixEdit_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
//	int nCnt;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = Max_nPreSubfix;	//自定义前缀最大10个
		return SC_RESULT_NEXT;
	}

	if (pParam->nDataLen >10 || pParam->nDataLen < 1)		//自定义前缀最多10个字符
	{
		return SC_RESULT_FAIL;
	}
	
	memcpy(X_paramGetGlobalAllParam()->pCode->byPrefix, pParam->pData, pParam->nDataLen);
	X_paramGetGlobalAllParam()->pCode->nPrefix = pParam->nDataLen;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		memcpy(X_paramBakGetGlobalAllParam()->pCode->byPrefix, X_paramGetGlobalAllParam()->pCode->byPrefix, pParam->nDataLen);
		X_paramBakGetGlobalAllParam()->pCode->nPrefix = X_paramGetGlobalAllParam()->pCode->nPrefix;
	}

	return nResult;
}

int PrefixEdit_Get(sc_param_st *pParam)
{
	int  nPreLen, nResult = SC_RESULT_SUCC;
	int	 nIndex = 0;
	char cBuff[20];
	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS://RAM
		nPreLen = X_paramGetGlobalAllParam()->pCode->nPrefix;
		for (nIndex = 0; nIndex < nPreLen ;nIndex++)
		{
			cBuff[nIndex] = X_paramGetGlobalAllParam()->pCode->byPrefix[nIndex];
		}
		break;

	case SC_CFG_SYS://SYS
		nPreLen = X_paramBakGetGlobalAllParam()->pCode->nPrefix;
		for (nIndex = 0; nIndex < nPreLen; nIndex++)
		{
			cBuff[nIndex] = X_paramBakGetGlobalAllParam()->pCode->byPrefix[nIndex];
		}
		break;

	case SC_CFG_FACT://FAC
		if (X_FacParamIsGet() != TRUE)
		{
			return SC_RESULT_FAIL;
		}

		memcpy(&pCodeParam, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));

		nPreLen = pCodeParam.nPrefix;
		for (nIndex = 0; nIndex < nPreLen;nIndex++)
		{
			cBuff[nIndex] = pCodeParam.byPrefix[nIndex];
		}

		break;

	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	if (pParam->nOutBufSize >= nPreLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nPreLen);
		*pParam->pOutLen = nPreLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}


int PrefixLenth_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_SUCC;
	int	 nLen = 0;
	char cBuff[20];
//	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

      if(pParam->nDataLen)
      {
		return SC_RESULT_FAIL;
	}
      
	nVal = Max_nPreSubfix;		//自定义前缀最大长度为10
	nLen = sprintf(cBuff,"%d",nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}
//查询自定义前缀支持的最大长度  Tag：CPRQSZ
int PrefixLenth_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//AIMID使能设置 AIDENA
int AIMIDEnable_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal > 1 || nVal < 0)
	{
		return SC_RESULT_FAIL;
	}
	if (!nVal)
	{
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] &= (~0x01);
	} 
	else
	{
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] &= (~0x06);
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] |= 0x5;
	}

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM];
	}

	return nResult;
}

int AIMIDEnable_Get(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_SUCC;
	int	 nLen = 0;
	char cBuff[4];
	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS://RAM
		nVal = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] & 0x01;
		break;

	case SC_CFG_SYS://SYS
		nVal = X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] & 0x01;
		break;

	case SC_CFG_FACT://FAC
		if (X_FacParamIsGet() != TRUE)
		{
			return SC_RESULT_FAIL;
		}

		memcpy(&pCodeParam, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));

		nVal = pCodeParam.IsPreSufOn[POS_IsAIM] & 0x01;

		break;

	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//CodeID综合设置    Tag:CIDENA
int CodeIDEnable_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal > 1 || nVal < 0)
	{
		return SC_RESULT_FAIL;
	}
	if (!nVal)
	{
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] &= (~0x01);
	}
	else
	{
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] |= 0x1;
	}

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID];
	}

	return nResult;
}

int CodeIDEnable_Get(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_SUCC;
	int	 nLen = 0;
	char cBuff[4];
	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS://RAM
		nVal = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] & 0x01;
		break;

	case SC_CFG_SYS://SYS
		nVal = X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] & 0x01;
		break;

	case SC_CFG_FACT://FAC
		if (X_FacParamIsGet() != TRUE)
		{
			return SC_RESULT_FAIL;
		}

		memcpy(&pCodeParam, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));

		nVal = pCodeParam.IsPreSufOn[POS_IsCodeID] & 0x01;

		break;

	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//Tag:CIDDEF
int AllCodeIDDefault_Set(sc_param_st *pParam)
{
	int nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
	
	memcpy(X_paramGetGlobalAllParam()->pCode->by1DCodeID, CodeIDTab1D, 32);
	memcpy(X_paramGetGlobalAllParam()->pCode->by2DCodeID, CodeIDTab2D, 32);
	memcpy(X_paramGetGlobalAllParam()->pCode2->by1DCodeID_2, CodeIDTab1D_2, 32);
	memcpy(X_paramGetGlobalAllParam()->pCode2->by2DCodeID_2, CodeIDTab2D_2, 32);

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		memcpy(X_paramBakGetGlobalAllParam()->pCode->by1DCodeID, X_paramGetGlobalAllParam()->pCode->by1DCodeID,32);
		memcpy(X_paramBakGetGlobalAllParam()->pCode->by2DCodeID, X_paramGetGlobalAllParam()->pCode->by2DCodeID,32);
		memcpy(X_paramBakGetGlobalAllParam()->pCode2->by1DCodeID_2, X_paramGetGlobalAllParam()->pCode2->by1DCodeID_2,32);
		memcpy(X_paramBakGetGlobalAllParam()->pCode2->by2DCodeID_2, X_paramGetGlobalAllParam()->pCode2->by2DCodeID_2,32);
	}

	return nResult;
}

int AllCodeIDDefault_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC; 
}

//Tag:CIDQSZ
int CodeIDLenth_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_SUCC;
	int	 nLen = 0;
	char cBuff[4];
//	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
      if(pParam->nDataLen)
      {
		return SC_RESULT_FAIL;
	}
	nVal = 2;		//自定义前缀最大长度为10
	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

int CodeIDLenth_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}


//Tag:CIDQVL
int CodeIDValueRange_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_SUCC;
	int	 nLen = 0;
	char cBuff[4];
//	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
      if(pParam->nDataLen)
      {
		return SC_RESULT_FAIL;
	}
	nLen = sprintf(cBuff, "01-FF");
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

int CodeIDValueRange_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//后缀使能设置  CSUENA
int SufixEnable_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)		//0:禁止，1：使能
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] = nVal;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix];
	}
	return nResult;
}

int SufixEnable_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] & 0x01;
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] & 0x01;
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pCodeParam, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));
		nVal = pCodeParam.IsPreSufOn[POS_IsSuffix] & 0x01;

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);

	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//后缀信息编辑  CSUSET
int SufixEdit_Set(sc_param_st *pParam)
{
	int  nResult = SC_RESULT_Save;
	//int nCnt;
      int i,j=0;
      unsigned char cSuffix[11]={'\0'};
      char temp1=0,temp2=0;
       
	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	/*if (!pParam->pData)//后缀可以为空
	{
		return SC_RESULT_FAIL;
	}*/

      if(!pParam->bCompleteFlag)
      {
            if (!pParam->nDataLen)
	     {
		    pParam->nDigitMaxCnt = Max_nPreSubfix;
		    return SC_RESULT_NEXT;
	     }
      }
      else
      {
        	if (pParam->nDataLen > Max_nPreSubfix*2)// || pParam->nDataLen < 2)		//自定义前缀最多10个字符
        	{
        		return SC_RESULT_FAIL;
        	}
              if( pParam->nDataLen%2!=2 && pParam->nDataLen%2!=0)//长度必须偶数
              {
        		return SC_RESULT_FAIL;
        	}

              for(i=0; i<pParam->nDataLen; i++,j++)
              {  
                    if(pParam->pData[i] <= 0x39)
                        temp1 = (pParam->pData[0] - 0x30)*16;
                    else if(pParam->pData[i] <= 0x5a && pParam->pData[i] >= 0x41) 
                        temp1 = (pParam->pData[i] - 0x41 + 10)*16;			
                    else if(pParam->pData[i] <= 0x7a && pParam->pData[i] >= 0x61)	
                        temp1 = (pParam->pData[i] - 0x61 + 10)*16;

                    if(pParam->pData[i+1] <= 0x39)
                        temp2 = (pParam->pData[i+1] - 0x30);
                    else if(pParam->pData[i+1] <= 0x5a && pParam->pData[i+1] >= 0x41) 
                        temp2 = (pParam->pData[i+1] - 0x41 + 10);			
                    else if(pParam->pData[i+1] <= 0x7a && pParam->pData[i+1] >= 0x61)	
                        temp2 = (pParam->pData[i+1] - 0x61 + 10);					
     
                    cSuffix[j] = temp1 + temp2;
                    i++;
              }

        	memcpy(X_paramGetGlobalAllParam()->pCode->bySuffix, cSuffix, 10);
            
        	X_paramGetGlobalAllParam()->pCode->nSuffix = j;

        	X_paramUpdateToDevice();

        	if (!(pParam->bSaveFlag))
        	{
        		nResult = SC_RESULT_SuccUnsave;
        	}
        	else
        	{
        		memcpy(X_paramBakGetGlobalAllParam()->pCode->bySuffix, X_paramGetGlobalAllParam()->pCode->bySuffix, pParam->nDataLen);
        		X_paramBakGetGlobalAllParam()->pCode->nSuffix = X_paramGetGlobalAllParam()->pCode->nSuffix;
        	}
       }
	return nResult;
}
int SufixEdit_Get(sc_param_st *pParam)
{
	int  nSufLen, nLen,nResult = SC_RESULT_SUCC;
	int	 nIndex = 0;
	char cBuff[20]={0x00};
	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

      if(pParam->nQueryType)
      {  
             nLen = sprintf(cBuff, "00-FF");
		if (pParam->nOutBufSize >= nLen)
		{
			memcpy(pParam->pOutBuf, cBuff, nLen);
			*pParam->pOutLen = nLen;
		}
             else
                return SC_RESULT_FAIL;
             return nResult;
      }

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS://RAM
		nSufLen = X_paramGetGlobalAllParam()->pCode->nSuffix;
		for (nIndex = 0; nIndex < nSufLen; nIndex++)
		{
			//cBuff[nIndex] = X_paramGetGlobalAllParam()->pCode->bySuffix[nIndex];
			sprintf(cBuff+strlen(cBuff),"%x", X_paramGetGlobalAllParam()->pCode->bySuffix[nIndex]);
		}
		break;

	case SC_CFG_SYS://SYS
		nSufLen = X_paramBakGetGlobalAllParam()->pCode->nSuffix;
		for (nIndex = 0; nIndex < nSufLen; nIndex++)
		{
			//cBuff[nIndex] = X_paramBakGetGlobalAllParam()->pCode->bySuffix[nIndex];
			sprintf(cBuff+strlen(cBuff),"%x", X_paramGetGlobalAllParam()->pCode->bySuffix[nIndex]);
		}
		break;

	case SC_CFG_FACT://FAC
		if (X_FacParamIsGet() != TRUE)
		{
			return SC_RESULT_FAIL;
		}

		memcpy(&pCodeParam, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));

		nSufLen = pCodeParam.nSuffix;
		for (nIndex = 0; nIndex < nSufLen; nIndex++)
		{
			//cBuff[nIndex] = pCodeParam.bySuffix[nIndex];
			sprintf(cBuff+strlen(cBuff),"%X", pCodeParam.bySuffix[nIndex]);
		}

		break;

	default:
		return SC_RESULT_UnSurpport;
		break;
	}

      //for(nIndex = 0; nIndex < nSufLen; nIndex++)
        

	if (pParam->nOutBufSize >= nSufLen)
	{
		memcpy(pParam->pOutBuf, cBuff, strlen(cBuff));
		*pParam->pOutLen = strlen(cBuff);
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}


//Tag:CSUQSZ
int SufixLenth_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_SUCC;
	int	 nLen = 0;
	char cBuff[4];
//	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}
      if(pParam->nDataLen)
      {
		return SC_RESULT_FAIL;
	}
	nVal = Max_nPreSubfix;		//自定义后缀最大长度为10
	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, strlen(cBuff));
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

int SufixLenth_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//结束符使能设置
int TerminatEnable_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_Save;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}

	if (!pParam->nDataLen || pParam->nDataLen > 1)
	{
		return SC_RESULT_FAIL;
	}

	if (!IsDec(pParam->pData, pParam->nDataLen))
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 1)		//0:禁止，1：使能
	{
		return SC_RESULT_FAIL;
	}

	X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] = nVal;

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal];
	}
	return nResult;
}
int TerminatEnable_Get(sc_param_st *pParam)
{
	int  nVal, nLen, nResult = SC_RESULT_SUCC;
	char cBuff[8];
	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
		nVal = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] & 0x01;
		break;

	case SC_CFG_SYS:	//SYS
		nVal = X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] & 0x01;
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pCodeParam, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));
		nVal = pCodeParam.IsPreSufOn[POS_IsTerminal] & 0x01;

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}

	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}

//结束符信息编辑  TSUSET
int TerminatEdit_Set(sc_param_st *pParam)
{
	int  nLen, nResult = SC_RESULT_Save;
//	int nCnt;
	unsigned int nChar1, nChar2;

	if (!pParam)
	{
		return SC_RESULT_FAIL;
	}
    
      if (!pParam->nDataLen)
	{
		pParam->nDigitMaxCnt = Max_nTerminal;
		return SC_RESULT_NEXT;
	}

	if (!pParam->pData)
	{
		return SC_RESULT_FAIL;
	}
	
	nLen = pParam->nDataLen >> 1;				//（pParam->nDataLen >> 1):一个结束符(hex)占用2个{Data}位

	if ( nLen > Max_nTerminal || nLen< 1)		//自定义结束符最多2个字符  
	{
		
		return SC_RESULT_FAIL;
	}

	if (!IsHex(pParam->pData, pParam->nDataLen))
	{
		
		return SC_RESULT_FAIL;
	}
	nChar1 = 0;
	nChar2 = 0;
	if (nLen == 1)
	{
		nChar1 = Hex2Int(pParam->pData, 2);
		nChar2 = 0;
		X_paramGetGlobalAllParam()->pCode->byTerminal[0] = nChar1;
		X_paramGetGlobalAllParam()->pCode->byTerminal[1] = nChar2;
		X_paramGetGlobalAllParam()->pCode->byTerminal[2] = 0;
		X_paramGetGlobalAllParam()->pCode->nTerminal = 1;
	} 
	else if (nLen == 2)
	{
		nChar1 = Hex2Int(pParam->pData, 2);
		nChar2 = Hex2Int(pParam->pData+2, 2);
		X_paramGetGlobalAllParam()->pCode->byTerminal[0] = nChar1;
		X_paramGetGlobalAllParam()->pCode->byTerminal[1] = nChar2;
		X_paramGetGlobalAllParam()->pCode->byTerminal[2] = 0;
		X_paramGetGlobalAllParam()->pCode->nTerminal = 2;
	}
	else 
	{
		
		return SC_RESULT_FAIL;
	}

	X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else
	{
		X_paramBakGetGlobalAllParam()->pCode->byTerminal[0] = X_paramGetGlobalAllParam()->pCode->byTerminal[0];
		X_paramBakGetGlobalAllParam()->pCode->byTerminal[1] = X_paramGetGlobalAllParam()->pCode->byTerminal[1];
		X_paramBakGetGlobalAllParam()->pCode->byTerminal[2] = X_paramGetGlobalAllParam()->pCode->byTerminal[2];
		X_paramBakGetGlobalAllParam()->pCode->nTerminal = X_paramGetGlobalAllParam()->pCode->nTerminal;
	}

	return nResult;
}
int TerminatEdit_Get(sc_param_st *pParam)
{
	int  nLen, nResult = SC_RESULT_SUCC;
	unsigned int nChar1, nChar2;
	char cBuff[16];
	CodeParam pCodeParam;
      unsigned int nTlen;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

	switch (pParam->nConfType)
	{
	case SC_CFG_NSYS:	//RAM
	        nTlen = X_paramGetGlobalAllParam()->pCode->nTerminal; 
		nChar1 = X_paramGetGlobalAllParam()->pCode->byTerminal[0];
		nChar2 = X_paramGetGlobalAllParam()->pCode->byTerminal[1];
//		nChar3 = X_paramGetGlobalAllParam()->pCode->byTerminal[2];
		break;

	case SC_CFG_SYS:	//SYS
	        nTlen = X_paramBakGetGlobalAllParam()->pCode->nTerminal; 
		nChar1 = X_paramBakGetGlobalAllParam()->pCode->byTerminal[0];
		nChar2 = X_paramBakGetGlobalAllParam()->pCode->byTerminal[1];
//		nChar3 = X_paramBakGetGlobalAllParam()->pCode->byTerminal[2];
		break;

	case SC_CFG_FACT:	//FAC
		if (X_FacParamIsGet() != TRUE)
			return SC_RESULT_FAIL;

		memcpy(&pCodeParam, FacParamBuf + Start_MEMSize_CODE, sizeof(CodeParam));
            nTlen = pCodeParam.nTerminal; 
		nChar1 = pCodeParam.byTerminal[0];
		nChar2 = pCodeParam.byTerminal[1];
//		nChar3 = pCodeParam.byTerminal[2];

		break;
	default:
		return SC_RESULT_UnSurpport;
		break;
	}
    
      if(nTlen == 1)
        nLen = sprintf(cBuff, "%02X", nChar1);
      else
        nLen = sprintf(cBuff, "%02X%02X", nChar1, nChar2);
      
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}


int TerminatLenth_Set(sc_param_st *pParam)
{
	int  nVal, nResult = SC_RESULT_SUCC;
	int	 nLen = 0;
	char cBuff[20];
//	CodeParam pCodeParam;

	if (!pParam->pOutBuf)
	{
		return SC_RESULT_FAIL;
	}

       if(pParam->nDataLen)
       {
		return SC_RESULT_FAIL;
	}
       
	nVal = Max_nTerminal;		//自定义前缀最大长度为10
	nLen = sprintf(cBuff, "%d", nVal);
	if (pParam->nOutBufSize >= nLen)
	{
		memcpy(pParam->pOutBuf, cBuff, nLen);
		*pParam->pOutLen = nLen;
	}
	else
	{
		nResult = SC_RESULT_FAIL;
	}

	return nResult;
}
int TerminatLenth_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}


/****************************************************************************
*@Data Formatting 数据格式编辑
****************************************************************************/
//DataFormatter 的使能设置		Tag：DFMENA
int DataFormatterEnable_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 4)
    { 
        return SC_RESULT_FAIL; 
    }  

    if (nVal == 0)	//关闭
    {
        X_paramGetGlobalAllParam()->pDFormat->DFEnable = 0;
    }
    else 
    {
        X_paramGetGlobalAllParam()->pDFormat->DFEnable = 1;

        X_paramGetGlobalAllParam()->pDFormat->outFlag = nVal - 1;
    }

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
    {   
        X_paramBakGetGlobalAllParam()->pDFormat->DFEnable = X_paramGetGlobalAllParam()->pDFormat->DFEnable;
        if(nVal != 0)
            X_paramGetGlobalAllParam()->pDFormat->outFlag = X_paramGetGlobalAllParam()->pDFormat->outFlag;
    }
  
    return nResult;
 }
int DataFormatterEnable_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    DataFormat dFormat;
    if( !pParam )
    { return SC_RESULT_FAIL; }
    
    if(!(pParam->nQueryType))
    {
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pDFormat->DFEnable;
                if(nVal !=0)
                    nVal = X_paramGetGlobalAllParam()->pDFormat->outFlag + 1;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pDFormat->DFEnable;
                if(nVal !=0)
                    nVal = X_paramBakGetGlobalAllParam()->pDFormat->outFlag + 1;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&dFormat, FacParamBuf+Start_MEMSize_DATAFORMAT, sizeof(DataFormat));
                nVal = dFormat.DFEnable;   
                 if(nVal !=0)
                    nVal = dFormat.outFlag + 1;
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }
        pParam->pOutBuf[0] = nVal+0x30;
        *(pParam->pOutLen) = 1;
    }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-4");
    }
    return nResult;
}


//恢复DataFormatter的默认数据格式 		Tag：DFMDEF
int LoadDefaultDataFormatType_Set(sc_param_st *pParam)
{

    int  nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> pData)
    { 
        return SC_RESULT_FAIL; 
    }

    X_paramGetGlobalAllParam()->pDFormat->DFEnable = FALSE;//disable
    X_paramGetGlobalAllParam()->pDFormat->ErrorTone = 0;//开启提示音
    X_paramGetGlobalAllParam()->pDFormat->outFlag = 0;//数据格式不匹配时的输出要求
    X_paramGetGlobalAllParam()->pDFormat->iSsingle = 0;//单次format关闭
    X_paramGetGlobalAllParam()->pDFormat->usedID = 0;
    memset(X_paramGetGlobalAllParam()->pDFormat->format, 0, sizeof(X_paramGetGlobalAllParam()->pDFormat->format));
            
    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
    {    
        X_paramBakGetGlobalAllParam()->pDFormat->DFEnable = FALSE;//disable
        X_paramBakGetGlobalAllParam()->pDFormat->ErrorTone = 0;//开启提示音
        X_paramBakGetGlobalAllParam()->pDFormat->outFlag = 0;//数据格式不匹配时的输出要求
        X_paramBakGetGlobalAllParam()->pDFormat->iSsingle = 0;//单次format关闭
        X_paramBakGetGlobalAllParam()->pDFormat->usedID = 0;
        memset(X_paramBakGetGlobalAllParam()->pDFormat->format, 0, sizeof(X_paramBakGetGlobalAllParam()->pDFormat->format));
    }
    return nResult;
}
int LoadDefaultDataFormatType_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//进入DataFormatter的数据格式编辑 		Tag：DFMSET
int DataFormatEditorEnable_Set(sc_param_st *pParam)
{ 
    DF_Param	dfparam;
    int nResult = SC_RESULT_SUCC;
    int i;
    int flag=0;
  
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        return SC_RESULT_NEXT;  
    }
 
    i = pParam->pData[0] - 0x30;

    if ((i>=0) && (i<4))
    {
        dfparam.nDFSelType = pParam->pData[1];

        if (dfparam.nDFSelType == 0x33)
        {
            dfparam.pDFCmd = pParam->pData+HEADLEN-1;
            dfparam.nDFMsgLen = strlen((char*)(pParam->pData+HEADLEN-1))-2;//-2:校验和<ETX>的长度
        } 
        else
        {
            dfparam.pDFCmd = pParam->pData+HEADLEN;
            dfparam.nDFMsgLen = strlen((char*)(pParam->pData+HEADLEN))-2;
        }
    			
        dfparam.nDFSelType = 3;//库里面只接受命令类型位1、2或3			
//printf("%s",dfparam.pDFCmd);
        flag = GXDataFormat_IsRightCmd(&dfparam);
 
        if (flag < 0)
        {
            //memset(pParam->pData, 0, Command_Num);
            return SC_RESULT_FAIL;
        }
			
        X_paramGetGlobalAllParam()->pDFormat->format[i].FormatID = i;
        X_paramGetGlobalAllParam()->pDFormat->format[i].FormatType = pParam->pData[1] - 0x30;
        X_paramGetGlobalAllParam()->pDFormat->format[i].TerID = (pParam->pData[2] - 0x30)*100+(pParam->pData[3] - 0x30)*10+(pParam->pData[4] - 0x30);
        memset((X_paramGetGlobalAllParam()->pDFormat->format[i].Cmd),'\0',Command_Num+1);
        if (pParam->pData[1] == 0x33)
        {		
            X_paramGetGlobalAllParam()->pDFormat->format[i].SymbolID = (pParam->pData[5] - 0x30)*10+(pParam->pData[6] - 0x30);
            X_paramGetGlobalAllParam()->pDFormat->format[i].MsgLen = (pParam->pData[7] - 0x30)*1000+(pParam->pData[8] - 0x30)*100+(pParam->pData[9] - 0x30)*10+(pParam->pData[10] - 0x30);
            memcpy((X_paramGetGlobalAllParam()->pDFormat->format[i].Cmd), pParam->pData+HEADLEN-1, strlen((char*)(pParam->pData+HEADLEN-1))-2);
        } 
        else
        {
            X_paramGetGlobalAllParam()->pDFormat->format[i].SymbolID = (pParam->pData[5] - 0x30)*100+(pParam->pData[6] - 0x30)*10+(pParam->pData[7] - 0x30);
            X_paramGetGlobalAllParam()->pDFormat->format[i].MsgLen = (pParam->pData[8] - 0x30)*1000+(pParam->pData[9] - 0x30)*100+(pParam->pData[10] - 0x30)*10+(pParam->pData[11] - 0x30);
            memcpy((X_paramGetGlobalAllParam()->pDFormat->format[i].Cmd), pParam->pData+HEADLEN, strlen((char*)(pParam->pData+HEADLEN))-2);
        }
    		
        //memset(g_DataFormatbuf, 0, FORMATLEN);
  
    }
    else
        {return SC_RESULT_FAIL;}
    /*if (g_DataFormatbuf[0] == 0)//取消前面读取的一串数据，此处不能return false，否则，保存报错，且下一次串口发送命令总是返回0x15
    {
        break;
    }*/

    if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else
    {
        X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatID = i;
        X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatType = pParam->pData[1] - 0x30;
        X_paramBakGetGlobalAllParam()->pDFormat->format[i].TerID = (pParam->pData[2] - 0x30)*100+(pParam->pData[3] - 0x30)*10+(pParam->pData[4] - 0x30);
        memset((X_paramBakGetGlobalAllParam()->pDFormat->format[i].Cmd),'\0',Command_Num+1);
        if (pParam->pData[1] == 0x33)
        {		
            X_paramBakGetGlobalAllParam()->pDFormat->format[i].SymbolID = (pParam->pData[5] - 0x30)*10+(pParam->pData[6] - 0x30);
            X_paramBakGetGlobalAllParam()->pDFormat->format[i].MsgLen = (pParam->pData[7] - 0x30)*1000+(pParam->pData[8] - 0x30)*100+(pParam->pData[9] - 0x30)*10+(pParam->pData[10] - 0x30);
            memcpy((X_paramBakGetGlobalAllParam()->pDFormat->format[i].Cmd), pParam->pData+HEADLEN-1, strlen((char*)(pParam->pData+HEADLEN-1))-2);
        } 
        else
        {
            X_paramBakGetGlobalAllParam()->pDFormat->format[i].SymbolID = (pParam->pData[5] - 0x30)*100+(pParam->pData[6] - 0x30)*10+(pParam->pData[7] - 0x30);
            X_paramBakGetGlobalAllParam()->pDFormat->format[i].MsgLen = (pParam->pData[8] - 0x30)*1000+(pParam->pData[9] - 0x30)*100+(pParam->pData[10] - 0x30)*10+(pParam->pData[11] - 0x30);
            memcpy((X_paramBakGetGlobalAllParam()->pDFormat->format[i].Cmd), pParam->pData+HEADLEN, strlen((char*)(pParam->pData+HEADLEN))-2);
        }
    }
    
    return nResult;
    
}
int DataFormatEditorEnable_Get(sc_param_st *pParam){ return SC_RESULT_SUCC; }

//清除某个DataFormatter数据格式类型 	Tag：DFMCLR
int ClearDataFormatTypes_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> pData || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 3)
    { 
        return SC_RESULT_FAIL; 
    }  

    memset(X_paramGetGlobalAllParam()->pDFormat->format[nVal].Cmd,0,Command_Num);
    X_paramGetGlobalAllParam()->pDFormat->format[nVal].FormatID = 0;
    X_paramGetGlobalAllParam()->pDFormat->format[nVal].FormatType = 0;
    X_paramGetGlobalAllParam()->pDFormat->format[nVal].TerID = 0;
    X_paramGetGlobalAllParam()->pDFormat->format[nVal].SymbolID = 0;
    X_paramGetGlobalAllParam()->pDFormat->format[nVal].MsgLen = 0;

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
    {   
        memset(X_paramBakGetGlobalAllParam()->pDFormat->format[nVal].Cmd,0,Command_Num);
        X_paramBakGetGlobalAllParam()->pDFormat->format[nVal].FormatID = 0;
        X_paramBakGetGlobalAllParam()->pDFormat->format[nVal].FormatType = 0;
        X_paramBakGetGlobalAllParam()->pDFormat->format[nVal].TerID = 0;
        X_paramBakGetGlobalAllParam()->pDFormat->format[nVal].SymbolID = 0;
        X_paramBakGetGlobalAllParam()->pDFormat->format[nVal].MsgLen = 0;
    }
    return nResult;

}
int ClearDataFormatTypes_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//清除所有DataFormatter数据格式类型 	Tag：DFMCAL
int ClearAllDataFormatTypes_Set(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_Save;

    memset(X_paramGetGlobalAllParam()->pDFormat->format, 0, sizeof(X_paramGetGlobalAllParam()->pDFormat->format));

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
    {   
        memset(X_paramBakGetGlobalAllParam()->pDFormat->format, 0, sizeof(X_paramBakGetGlobalAllParam()->pDFormat->format));
    }
    return nResult;
}

int ClearAllDataFormatTypes_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//加载选中的DataFormatter数据格式 		Tag：DFMUSE
int LoadDataFormatTypes_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 3)
    { 
        return SC_RESULT_FAIL; 
    }  
   
    X_paramGetGlobalAllParam()->pDFormat->usedID = nVal;
    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pDFormat->usedID = nVal;
  
    return nResult;
}
int LoadDataFormatTypes_Get(sc_param_st *pParam)
{ 
    int  nVal,nResult = SC_RESULT_SUCC;
    DataFormat dFormat;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }
    
     if(!(pParam->nQueryType))
    {
            switch(pParam->nConfType)
            {
                case 0://RAM
                    nVal = X_paramGetGlobalAllParam()->pDFormat->usedID;
                    break;
                case 1://SYS
                    nVal = X_paramBakGetGlobalAllParam()->pDFormat->usedID;
                    break;
                case 2://FAC
                    if(X_FacParamIsGet() != TRUE)
                        return SC_RESULT_FAIL;
                 
                    memcpy(&dFormat, FacParamBuf+Start_MEMSize_DATAFORMAT, sizeof(DataFormat));
                    nVal = dFormat.usedID;   
                    break; 
                default:
                    return SC_RESULT_UnSurpport;
                    break;
            }

            nVal += 0x30;
            pParam->pOutBuf[0] = nVal;
            *(pParam->pOutLen) = 1;
     }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-3");
    }
    return nResult;
}

//加载选中的单次读码DataFormatter数据格式 		Tag：DFMSIN
int LoadDataFormatTypesForSingleDeco_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 3)
    { 
        return SC_RESULT_FAIL; 
    }  

    X_paramGetGlobalAllParam()->pDFormat->iSsingle = 1;
    X_paramGetGlobalAllParam()->pDFormat->singleFormatID = nVal;

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
    {
        X_paramBakGetGlobalAllParam()->pDFormat->iSsingle = 1;
        X_paramBakGetGlobalAllParam()->pDFormat->singleFormatID = nVal;
    }
  
    return nResult;

}
int LoadDataFormatTypesForSingleDeco_Get(sc_param_st *pParam)
{ 

    int  nVal,nResult = SC_RESULT_SUCC;
    DataFormat dFormat;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }
    
    if(!(pParam->nQueryType))
    { 
        switch(pParam->nConfType)
        {
            case 0://RAM
                nVal = X_paramGetGlobalAllParam()->pDFormat->singleFormatID;
                break;
            case 1://SYS
                nVal = X_paramBakGetGlobalAllParam()->pDFormat->singleFormatID;
                break;
            case 2://FAC
                if(X_FacParamIsGet() != TRUE)
                    return SC_RESULT_FAIL;
             
                memcpy(&dFormat, FacParamBuf+Start_MEMSize_DATAFORMAT, sizeof(DataFormat));
                nVal = dFormat.singleFormatID;   
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }

        nVal += 0x30;
        pParam->pOutBuf[0] = nVal;
        *(pParam->pOutLen) = 1;
    }
    else
    {  
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-3");
    }
    return nResult;
}

//数据格式不匹配的错误提示音		Tag：DFMTON
int BeepOnDataFormatNotMatchEnable_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }  

    if(nVal == 0)
        X_paramGetGlobalAllParam()->pDFormat->ErrorTone = 1;
    else
        X_paramGetGlobalAllParam()->pDFormat->ErrorTone = 0;
      

    if(!(pParam->bSaveFlag))//不保存
        nResult = SC_RESULT_SuccUnsave;
    else//保存
        X_paramBakGetGlobalAllParam()->pDFormat->ErrorTone = X_paramGetGlobalAllParam()->pDFormat->ErrorTone;

    return nResult;
  }

int BeepOnDataFormatNotMatchEnable_Get(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    DataFormat dForamt;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    switch(pParam->nConfType)
    {
        case 0://RAM
            nVal = X_paramGetGlobalAllParam()->pDFormat->ErrorTone;
            break;
        case 1://SYS
            nVal = X_paramBakGetGlobalAllParam()->pDFormat->ErrorTone;
            break;
        case 2://FAC
            if(X_FacParamIsGet() != TRUE)
                return SC_RESULT_FAIL;
         
            memcpy(&dForamt, FacParamBuf+Start_MEMSize_DATAFORMAT, sizeof(DataFormat));
            nVal = dForamt.ErrorTone;   
            break; 
        default:
            return SC_RESULT_UnSurpport;
            break;
    }

    nVal += 0x30;
    pParam->pOutBuf[0] = nVal;
    *(pParam->pOutLen) = 1;

    return nResult;
}
int CurrentDataFormatType_Set(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SUCC;
    int i;
    char buf[1024]={'\0'};
   
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nDataLen)
        return SC_RESULT_FAIL;

    switch(pParam->bSaveFlag)
    {
        case 0://RAM
*pParam->pOutLen = 0;
            for(i=0; i<4; i++)
            {
                if (X_paramGetGlobalAllParam()->pDFormat->format[i].TerID > 0)
                { 
                    sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf) ,"Data Format %d: %d", i,X_paramGetGlobalAllParam()->pDFormat->format[i].FormatID);
                    sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%d",X_paramGetGlobalAllParam()->pDFormat->format[i].FormatType);
			 sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%d",X_paramGetGlobalAllParam()->pDFormat->format[i].TerID);
			 if (X_paramGetGlobalAllParam()->pDFormat->format[i].FormatType == 3)
			 {
                        sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%02d",X_paramGetGlobalAllParam()->pDFormat->format[i].SymbolID);
                    } 
                    else
                    {
                        sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%03d",X_paramGetGlobalAllParam()->pDFormat->format[i].SymbolID);
                    }
					
                    sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%04d",X_paramGetGlobalAllParam()->pDFormat->format[i].MsgLen);
                    sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%s",X_paramGetGlobalAllParam()->pDFormat->format[i].Cmd);
                    sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),";\r\n");	
		       *pParam->pOutLen = strlen(pParam->pOutBuf);
                }
            }    
            break;
            
            case 1://SYS
        *pParam->pOutLen = 0;
                for(i=0; i<4; i++)
                {
                    if (X_paramBakGetGlobalAllParam()->pDFormat->format[i].TerID > 0)
                    {
                        sprintf(buf ,"Data Format%d: %d", i,X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatID);
                        sprintf(buf + strlen(buf),"%d",X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatType);
                        sprintf(buf + strlen(buf),"%d",X_paramBakGetGlobalAllParam()->pDFormat->format[i].TerID);
                        if (X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatType == 3)
                        {
                            sprintf(buf + strlen(buf),"%02d",X_paramBakGetGlobalAllParam()->pDFormat->format[i].SymbolID);
                        } 
                        else
                        {
                            sprintf(buf + strlen(buf),"%03d",X_paramBakGetGlobalAllParam()->pDFormat->format[i].SymbolID);
                        }
					
                        sprintf(buf + strlen(buf),"%04d",X_paramBakGetGlobalAllParam()->pDFormat->format[i].MsgLen);
                        sprintf(buf + strlen(buf),"%s",X_paramBakGetGlobalAllParam()->pDFormat->format[i].Cmd);
                        sprintf(buf + strlen(buf),";\r\n");	
		            
                        memcpy((pParam->pOutBuf) + (*pParam->pOutLen), buf, strlen(buf));
                        *pParam->pOutLen += strlen(buf);
                    }
                }    
            //printf("%d\r\n",*pParam->pOutLen);
                break;

            default:
                
                return SC_RESULT_UnSurpport;
                break;
        }
   
    return nResult;

 }
//查询当前的数据格式				Tag：DFMQCU
int CurrentDataFormatType_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}

int DefaultFactoryDataFormatType_Set(sc_param_st *pParam)
{ 
    DataFormat dFormat;
    int i;

    if(pParam->nDataLen)
        return SC_RESULT_FAIL;
    
    if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;
         
    memcpy(&dFormat, FacParamBuf+Start_MEMSize_DATAFORMAT, sizeof(DataFormat));
                
    for(i=0; i<4; i++)
    {
        if (dFormat.format[i].TerID > 0)
        {
            sprintf(pParam->pOutBuf ,"Data Format%d: %d", i,dFormat.format[i].FormatID);
            sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%d",dFormat.format[i].FormatType);
            sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%d",dFormat.format[i].TerID);
            if (dFormat.format[i].FormatType == 3)
            {
                sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%02d",dFormat.format[i].SymbolID);
            } 
            else
            {
                sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%03d",dFormat.format[i].SymbolID);
            }
					
            sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%04d",dFormat.format[i].MsgLen);
            sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%s",dFormat.format[i].Cmd);
            sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),";\r\n");	
            *pParam->pOutLen = strlen(pParam->pOutBuf);
        }
    }    
    return SC_RESULT_SUCC; 
}
//查询出厂默认的数据格式			Tag：DFMQFA
int DefaultFactoryDataFormatType_Get(sc_param_st *pParam)
{ 
 /*   DataFormat dFormat;
    int i;
    
    if(X_FacParamIsGet() != TRUE)
        return SC_RESULT_FAIL;
         
    memcpy(&dFormat, FacParamBuf+Start_MEMSize_DATAFORMAT, sizeof(DataFormat));
                
    for(i=0; i<4; i++)
    {
        if (dFormat.format[i].TerID > 0)
        {
            sprintf(pParam->pOutBuf ,"Data Format%d: %d", i,dFormat.format[i].FormatID);
            sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%d",dFormat.format[i].FormatType);
            sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%d",dFormat.format[i].TerID);
            if (dFormat.format[i].FormatType == 3)
            {
                sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%02d",dFormat.format[i].SymbolID);
            } 
            else
            {
                sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%03d",dFormat.format[i].SymbolID);
            }
					
            sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%04d",dFormat.format[i].MsgLen);
            sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),"%s",dFormat.format[i].Cmd);
            sprintf(pParam->pOutBuf + strlen(pParam->pOutBuf),";\r\n");	
            *pParam->pOutLen = strlen(pParam->pOutBuf);
        }
    }    */
    return SC_RESULT_SUCC; 
}
int MaxLenOfDataFormatTypes_Set(sc_param_st *pParam)
{ 
    int nVal;
    char cBuff[16];
    unsigned int nLen;

    if(pParam->nDataLen)
        return SC_RESULT_FAIL;
    nVal = sizeof(X_paramGetGlobalAllParam()->pDFormat->format->Cmd) - 16;
    nLen = sprintf(cBuff, "%d", nVal);
    memcpy(pParam->pOutBuf, cBuff, nLen);
    *(pParam->pOutLen) = nLen;
    return SC_RESULT_SUCC; 
}
//查询单条Data Format支持的最大长度	Tag：DFMQSZ
int MaxLenOfDataFormatTypes_Get(sc_param_st *pParam){ return SC_RESULT_SUCC; }

/****************************************************************************
*@驾照解析功能
****************************************************************************/
//启动驾照解析功能设置		Tag：DLPSET
int EnableAnalyzeDrivingLic_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int EnableAnalyzeDrivingLic_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//禁止驾照解析功能，并删除设置		Tag：DLPCLR
int DisableAnalyzeDrivingLic_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//查询驾照解析功能支持的指令最大长度		Tag：DLPQSZ
int MaxLenOfCommandInAnalDrivingLic_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@数据打包设置
****************************************************************************/
//设置数据打包	Tag：PACKAG
int PackageForDecoInfo_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int PackageForDecoInfo_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@信息截取设置
****************************************************************************/
//信息截取的使能设置	Tag：EXTRAC
int CuttingoutDecoInfoEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int CuttingoutDecoInfoEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//添加信息截取设置		Tag:EXTADD
int AddConfigForCuttingoutDecoInfo_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int AddConfigForCuttingoutDecoInfo_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//清除某种条码的截取设置		Tag:EXTCLR
int ClearCuttingoutForCodeTypes_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ClearCuttingoutForCodeTypes_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//清除最新一组的截取设置		Tag:EXTCLO
int ClearLatestCFGOfCuttingout_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ClearLatestCFGOfCuttingout_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//清除所有的截取设置		Tag:EXTCAL
int ClearAllCFGOfCuttingout_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ClearAllCFGOfCuttingout_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }


/****************************************************************************
*@批处理
****************************************************************************/
//批处理的使能设置
int BatchProcessEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BatchProcessEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@获取设备分辨率
****************************************************************************/
//设备分辨率的查询
int ResolutionRatio_Set(sc_param_st *pParam)
{ 
    int  nVal, temp,nResult = SC_RESULT_SUCC;
    int width,height;
    char cBuff[16];
//    unsigned int nLen;
    
    width = Z_GetCmosWidth();
    height = Z_GetCmosHeight();

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(pParam->nDataLen)
    { 
        return SC_RESULT_FAIL; 
    }
    
    nVal = sprintf((char*)(pParam->pOutBuf), "%d", width);
    temp = sprintf((pParam->pOutBuf)+nVal,"W");
    temp += nVal;
    nVal = sprintf((pParam->pOutBuf)+temp, "%d",height);
    nVal = sprintf(cBuff, "%d", width);
    temp += nVal;
    nVal = sprintf((pParam->pOutBuf)+temp,"H");
    
    *(pParam->pOutLen) = temp+nVal;

    return nResult; 
}
int ResolutionRatio_Get(sc_param_st *pParam){ return SC_RESULT_SUCC; }

/****************************************************************************
*@获取图像
****************************************************************************/
//获取图像
extern unsigned char Img_GoodDecode[];
extern int NewWidth;			//记录裁减后的图像宽度
extern int NewHeight;			//记录裁减后的图像高度
#define IMG_GREY	0	
#define IMG_BMP		1	
#define IMG_TIFF	2
#define IMG_JPG		3
int FetchingImage_Set(sc_param_st *pParam)
{ 
    int  nVal, nResult = SC_RESULT_Save;
    int jpgquelity = 0;
    int imgflag = 0;
    int nBreak;
    unsigned char *p;
    int nW,nH,sW,sH;
    int nScale_x, nScale_y;
    unsigned char chBuf[16];
    static BOOL    s_bImgGetChk = FALSE;
    int pm;
    
    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> pData || !pParam -> nDataLen)
    { 
        return SC_RESULT_FAIL; 
    }

   if((pParam->pData[0]>0x31)||(pParam->pData[2]>0x32)||(pParam->pData[4]>0x33))
    {  
        return SC_RESULT_FAIL; 
    }

    if(pParam -> nDataLen > 8)
    {  
        return SC_RESULT_FAIL; 
    }

    if(( pParam -> pData[4] > 0x34)||( pParam -> pData[4] == 0x34))
        return SC_RESULT_FAIL; 

    if(s_bImgGetChk == FALSE)
    {
        s_bImgGetChk = TRUE; 
        return SC_RESULT_IMGGET;
    }

    s_bImgGetChk = FALSE;
    
    pm = Z_GetPM();
    if (pm != FREQ_RUN_HIGH)
        Z_SetPM_EX(FREQ_RUN_HIGH);
    nBreak = Z_DogGetBreak(); //记录原先看门狗的复位时间
    Z_DogReset(-1);
/*
T，类型：0T - 实时图像（最后一次拍摄的图像），1T - 解码成功的图像
R, 图像比率：0R - 原始大小，1R - 1/4图像，2R - 1/16图像
F，图像格式：0F - 原始图像（Raw data), 1F - BMP格式， 2F - JPEG格式，3F - TIFF格式
Q，JPEG格式的图像质量：0Q - Low， 1Q - Middle， 2Q - High， 3Q - Highest
0T0R0F0Q
*/
    /* 记录压缩参数 */
    if(pParam -> pData[2] < 0x33)	
    {
        gImgReduceRow = pParam -> pData[2]-0x30;		//记录行压缩参数,0为正常,1为2倍,2为4倍
        gImgReduceColumn = pParam -> pData[2]-0x30;	//记录列压缩参数			
    }
    else
    {
        gImgReduceRow = 0;			//如果压缩参数超出范围则默认为不压缩
        gImgReduceColumn = 0;
    }
        
    /* 记录获取的图像类型 */
    gImgType = (int)(pParam -> pData[4]-0x30);


    if((pParam -> pData[4] == 0x32)&&(pParam -> nDataLen == 8))
    {
        /* 记录JPG图像的获取质量 */
        if( pParam -> pData[6] <0x34)
            jpgquelity = pParam -> pData[6]-0x30;
        else
            jpgquelity = 0;	//如果图像质量范围超出0-3则默认为0
    }			
    /* 是否要发送解码成功的图像 */
    if(pParam -> pData[0] == 0x31)
       imgflag = 1;
    	
    p = (unsigned char*)Y_cmosGetDataAddr();

    sW=0;
    sH=0;
    nW = Y_cmosGetWidth();
    nH = Y_cmosGetHeight();

    if(imgflag == 1 )	
    {
        p = Img_GoodDecode;			//要的话将临时图像指针指向解码成功的图像			
    }
    if(gImgType == 0)
    {//原始图
    
        nScale_x = 1<<gImgReduceRow;			//行压缩
        nScale_y = 1<<gImgReduceColumn;			//列压缩
       /* if ((Y_commGetCurType() == COMMT_USB_Data) && (gImgColor == 0))
        {
            Y_commWrite(p,nW*nH);
            Z_DogReset(nBreak); //恢复看门狗
            return TRUE;
        }*/
        //printf("t");
        //发送图像数据大小
        nVal = (Z_GetCmosWidth()* Z_GetCmosHeight()>> (pParam -> pData[2]-0x30)) >> (pParam -> pData[2]-0x30);
        String2Hex((unsigned char *)&nVal, 4, chBuf, 16);
        Y_commWrite(chBuf, 8);
        
        IMG_Edit_Send(gImgColor, 0, 0, nW, nH, nScale_x, nScale_y, nW, p);
    }
    if(gImgType >= 1 && gImgType <= 3)
    {
        /* 不管处理何种格式的图像.压缩参数的处理都相同 */
        nScale_x = 1<<gImgReduceRow;		
        nScale_y = 1<<gImgReduceColumn;
        /* 查询是否要发送解码成功的图像 */
       

        /* 处理并发送bmp格式图像 */
        if(gImgType == 1)
        {	
            IMG_Edit(IMG_BMP, sW , sH, nW, nH, nScale_x, nScale_y, p);	//图像预处理:处理压缩操作并修改bmp的行颠倒排列    
            Img_Send(IMG_BMP, 0, 1);									//将图像转成bmp格式并发送图像
        }
        /* 处理并发送tiff格式图像 */
        else if(gImgType == 3)
        {
            IMG_Edit(IMG_GREY, sW , sH, nW, nH, nScale_x, nScale_y, p); //如果要压缩则要预处理灰度图像
            Img_Send(IMG_TIFF, 0, 1);									 //将图像转成tiff格式并发送图像
        }
        /* 处理并发送jpg格式图像 */
        else if(gImgType == 2)
        { 
            IMG_Edit(IMG_GREY, sW , sH, nW, nH, nScale_x, nScale_y, p); //如果要压缩则要预处理灰度图像
            Img_Send(IMG_JPG, jpgquelity, 1);							 //将图像按质量转成jpg格式并发送图像	
        }
    }
    Z_DogReset(nBreak); //恢复看门狗
	
    if (pm != FREQ_RUN_HIGH)
        Z_SetPM_EX(pm);
    
    return nResult;

}

int FetchingImage_Get(sc_param_st *pParam)
{
    if( !pParam )
    { return SC_RESULT_FAIL; }


    if(pParam->nQueryType)
    {
        *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf), "0-1T0-2R0-3F0-3Q");
        //*(pParam->pOutLen) = sprintf(pParam->pOutBuf, "0-1T0-2R0-3F");
    }


    return SC_RESULT_SUCC;
    }

/****************************************************************************
*@产品信息
****************************************************************************/
//设置产品S/N号		Tag:PSNEDT
int SettingProductSN_Set(sc_param_st *pParam)
{
   int nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        g_ReplaceType = Replace_NewInstruction;
        pParam -> nDigitMaxCnt = 20;
        
        if(pParam -> bCompleteFlag == 0)
            return SC_RESULT_NSTR;  
    }

    if (pParam -> nDataLen < sizeof(X_paramGetGlobalAllParam()->pDevInfo->pSN))
    {
        memcpy(X_paramGetGlobalAllParam()->pDevInfo->pSN,pParam -> pData,pParam -> nDataLen);
        X_paramGetGlobalAllParam()->pDevInfo->pSN[pParam -> nDataLen] = '\0';
        X_paramPlaceKeyword((char*)"S/N",(char*)X_paramGetGlobalAllParam()->pDevInfo->pSN,TRUE);
    }
    X_paramSaveDeviceInfo();


    /*if(!(pParam->bSaveFlag))
    { nResult = SC_RESULT_SuccUnsave;}
    else*/
    {
        paramSaveType(1);
        if (pParam -> nDataLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pSN))
        {
            memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pSN,pParam -> pData,pParam -> nDataLen);
            X_paramGetGlobalAllParam()->pDevInfo->pSN[pParam -> nDataLen] = '\0';
            X_paramPlaceKeyword((char*)"S/N",(char*)X_paramGetGlobalAllParam()->pDevInfo->pSN,TRUE);
        }
        X_paramSaveDeviceInfo();
        paramSaveType(0);  
    }
    return nResult;

}

int SettingProductSN_Get(sc_param_st *pParam)
{ 
    int  nVal,nResult = SC_RESULT_SUCC;
//    UTDeviceINFO deviceInfo;
       
    if( !pParam )
    { return SC_RESULT_FAIL; }
   
    switch(pParam->nConfType)
    {
        case 0://RAM
            nVal = strlen((char*)X_paramGetGlobalAllParam()->pDevInfo->pSN);
            if (nVal > 0)
                *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf),"%s",X_paramGetGlobalAllParam()->pDevInfo->pSN);
            else
                *(pParam->pOutLen) = 0;
       
            break;
            
        case 1://SYS
            nVal = strlen((char*)X_paramBakGetGlobalAllParam()->pDevInfo->pSN);
            if (nVal > 0)
                *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf),"%s",X_paramBakGetGlobalAllParam()->pDevInfo->pSN);
            else
                *(pParam->pOutLen) = 0;
     
            break;
            
        case 2://FAC
         /*   if(X_FacParamIsGet() != TRUE)
                return SC_RESULT_FAIL;
         
            memcpy(&deviceInfo, FacParamBuf+Start_MEMSize_u, sizeof(HardParam));
                nVal = hHard.nBeepType;   
                if((nVal&0xf00) ==  0x900)
                {
                    memcpy(&cCode2, FacParamBuf+Start_MEMSize_CODE2, sizeof(CodeParam2));
                    dur = cCode2.nSucBeepDuration;   
                }*/
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }            
    return nResult;
}

//删除产品S/N号		Tag:PSNDEL
int DeleteProductSN_Set(sc_param_st *pParam)
{         
    int nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

 
    X_paramPlaceKeyword((char*)"S/N", (char*)"", 1);
    X_paramSaveDeviceInfo();

    /*if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else*/
    {
        paramSaveType(1);
        X_paramPlaceKeyword((char*)"S/N", (char*)"", 1);
        X_paramSaveDeviceInfo();
        paramSaveType(0);  
    }
    
    return nResult;
}

int DeleteProductSN_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}

int ProductSNLength_Set(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    char cBuff[16];
    unsigned int nLen;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }

    nVal = sizeof(X_paramGetGlobalAllParam()->pDevInfo->pSN) - 1;

    nLen = sprintf(cBuff, "%d", nVal);
    memcpy(pParam->pOutBuf, cBuff, nLen);
    *(pParam->pOutLen) = nLen;

    return nResult;

}
int ProductSNLength_Get(sc_param_st *pParam)
{return SC_RESULT_SUCC;}
//设置产品ESN号		Tag:ESNSET
int SettingProductESN_Set(sc_param_st *pParam)
{
   int nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        g_ReplaceType = Replace_NewInstruction;
        pParam -> nDigitMaxCnt = 20;

        if(pParam -> bCompleteFlag == 0)
            return SC_RESULT_NSTR;  
    }

    if (pParam -> nDataLen < sizeof(X_paramGetGlobalAllParam()->pDevInfo->pESN))
    {
        memcpy(X_paramGetGlobalAllParam()->pDevInfo->pESN,pParam -> pData,pParam -> nDataLen);
        X_paramGetGlobalAllParam()->pDevInfo->pESN[pParam -> nDataLen] = '\0';
        X_paramPlaceKeyword((char*)"ESN",(char*)X_paramGetGlobalAllParam()->pDevInfo->pESN,TRUE);
    }
    X_paramSaveDeviceInfo();


    /*if(!(pParam->bSaveFlag))
    { nResult = SC_RESULT_SuccUnsave;}
    else*/
    {
        paramSaveType(1);
        if (pParam -> nDataLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pESN))
        {
            memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pESN,pParam -> pData,pParam -> nDataLen);
            X_paramGetGlobalAllParam()->pDevInfo->pESN[pParam -> nDataLen] = '\0';
            X_paramPlaceKeyword((char*)"ESN",(char*)X_paramGetGlobalAllParam()->pDevInfo->pESN,TRUE);
        }
        X_paramSaveDeviceInfo();
        paramSaveType(0);  
    }
    return nResult;

}
int SettingProductESN_Get(sc_param_st *pParam)
{ 
    int  nVal,nResult = SC_RESULT_SUCC;
    //UTDeviceINFO deviceInfo;
       
    if( !pParam )
    { return SC_RESULT_FAIL; }
   
    switch(pParam->nConfType)
    {
        case 0://RAM
            nVal = strlen((char*)X_paramGetGlobalAllParam()->pDevInfo->pESN);
            if (nVal > 0)
                *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf),"%s",X_paramGetGlobalAllParam()->pDevInfo->pESN);
            else
                *(pParam->pOutLen) = 0;
       
            break;
            
        case 1://SYS
            nVal = strlen((char*)X_paramBakGetGlobalAllParam()->pDevInfo->pESN);
            if (nVal > 0)
                *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf),"%s",X_paramBakGetGlobalAllParam()->pDevInfo->pESN);
            else
                *(pParam->pOutLen) = 0;
     
            break;
            
        case 2://FAC
         /*   if(X_FacParamIsGet() != TRUE)
                return SC_RESULT_FAIL;
         
            memcpy(&deviceInfo, FacParamBuf+Start_MEMSize_u, sizeof(HardParam));
                nVal = hHard.nBeepType;   
                if((nVal&0xf00) ==  0x900)
                {
                    memcpy(&cCode2, FacParamBuf+Start_MEMSize_CODE2, sizeof(CodeParam2));
                    dur = cCode2.nSucBeepDuration;   
                }*/
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }            
    return nResult;
}    

//删除产品ESN号		Tag:ESNDEL
int DeleteProductESN_Set(sc_param_st *pParam)
{         
    int nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

 
    X_paramPlaceKeyword((char*)"ESN", (char*)"", 1);
    X_paramSaveDeviceInfo();

    /*if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else*/
    {
        paramSaveType(1);
        X_paramPlaceKeyword((char*)"ESN", (char*)"", 1);
        X_paramSaveDeviceInfo();
        paramSaveType(0);  
    }
    
    return nResult;
}
int DeleteProductESN_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}

int ProductESNLength_Set(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    char cBuff[16];
    unsigned int nLen;    
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }
    nVal = sizeof(X_paramGetGlobalAllParam()->pDevInfo->pESN) - 1;

    nLen = sprintf(cBuff, "%d", nVal);
    memcpy(pParam->pOutBuf, cBuff, nLen);
    *(pParam->pOutLen) = nLen;
    
    return nResult;

}

int ProductESNLength_Get(sc_param_st *pParam)
{return SC_RESULT_SUCC;}
//设置产品生产日期		Tag:DATEDT
int SettingProductDate_Set(sc_param_st *pParam)
{
   int nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        g_ReplaceType = Replace_NewInstruction;
        pParam -> nDigitMaxCnt = 20;

        if(pParam -> bCompleteFlag == 0)
            return SC_RESULT_NSTR;  
    }

    if (pParam -> nDataLen < sizeof(X_paramGetGlobalAllParam()->pDevInfo->pDate))
    {
        memcpy(X_paramGetGlobalAllParam()->pDevInfo->pDate,pParam -> pData,pParam -> nDataLen);
        X_paramGetGlobalAllParam()->pDevInfo->pDate[pParam -> nDataLen] = '\0';
        X_paramPlaceKeyword((char*)"DATE",(char*)X_paramGetGlobalAllParam()->pDevInfo->pDate,TRUE);
    }
    X_paramSaveDeviceInfo();


    /*if(!(pParam->bSaveFlag))
    { nResult = SC_RESULT_SuccUnsave;}
    else*/
    {
        paramSaveType(1);
        if (pParam -> nDataLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pDate))
        {
            memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pDate,pParam -> pData,pParam -> nDataLen);
            X_paramGetGlobalAllParam()->pDevInfo->pDate[pParam -> nDataLen] = '\0';
            X_paramPlaceKeyword((char*)"DATE",(char*)X_paramGetGlobalAllParam()->pDevInfo->pDate,TRUE);
        }
        X_paramSaveDeviceInfo();
        paramSaveType(0);  
    }
    return nResult;

}
int SettingProductDate_Get(sc_param_st *pParam)
{ 
    int  nVal,nResult = SC_RESULT_SUCC;
//    UTDeviceINFO deviceInfo;
       
    if( !pParam )
    { return SC_RESULT_FAIL; }
   
    switch(pParam->nConfType)
    {
        case 0://RAM
            nVal = strlen((char*)X_paramGetGlobalAllParam()->pDevInfo->pDate);
            if (nVal > 0)
                *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf),"%s",X_paramGetGlobalAllParam()->pDevInfo->pDate);
            else
                *(pParam->pOutLen) = 0;
       
            break;
            
        case 1://SYS
            nVal = strlen((char*)X_paramBakGetGlobalAllParam()->pDevInfo->pDate);
            if (nVal > 0)
                *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf),"%s",X_paramBakGetGlobalAllParam()->pDevInfo->pDate);
            else
                *(pParam->pOutLen) = 0;
     
            break;
            
        case 2://FAC
         /*   if(X_FacParamIsGet() != TRUE)
                return SC_RESULT_FAIL;
         
            memcpy(&deviceInfo, FacParamBuf+Start_MEMSize_u, sizeof(HardParam));
                nVal = hHard.nBeepType;   
                if((nVal&0xf00) ==  0x900)
                {
                    memcpy(&cCode2, FacParamBuf+Start_MEMSize_CODE2, sizeof(CodeParam2));
                    dur = cCode2.nSucBeepDuration;   
                }*/
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }            
    return nResult;
}    

//删除产品生产日期		Tag:DATDEL
int DeleteProductDate_Set(sc_param_st *pParam)
{         
    int nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

 
    X_paramPlaceKeyword((char*)"DATE", (char*)"", 1);
    X_paramSaveDeviceInfo();

    /*if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else*/
    {
        paramSaveType(1);
        X_paramPlaceKeyword((char*)"DATE", (char*)"", 1);
        X_paramSaveDeviceInfo();
        paramSaveType(0);  
    }
    
    return nResult;
}

int DeleteProductDate_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}
    
int ProductDateLength_Set(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_Save;
    char cBuff[16];
    unsigned int nLen;    
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }

    nVal = sizeof(X_paramGetGlobalAllParam()->pDevInfo->pDate) - 1;

    nLen = sprintf(cBuff, "%d", nVal);
    memcpy(pParam->pOutBuf, cBuff, nLen);
    *(pParam->pOutLen) = nLen;

    return nResult;

}

int ProductDateLength_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}
//设置产品名称		Tag:PDNEDT
int SettingProductName_Set(sc_param_st *pParam)
{
   int nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        g_ReplaceType = Replace_NewInstruction;
        pParam -> nDigitMaxCnt = 20;
        
        if(pParam -> bCompleteFlag == 0)
            return SC_RESULT_NSTR;  
    }

    if (pParam -> nDataLen < sizeof(X_paramGetGlobalAllParam()->pDevInfo->pManufID))
    {
        memcpy(X_paramGetGlobalAllParam()->pDevInfo->pManufID,pParam -> pData,pParam -> nDataLen);
        X_paramGetGlobalAllParam()->pDevInfo->pManufID[pParam -> nDataLen] = '\0';
        X_paramPlaceKeyword((char*)"ManufactureID",(char*)X_paramGetGlobalAllParam()->pDevInfo->pManufID,TRUE);
    }
    X_paramSaveDeviceInfo();


    /*if(!(pParam->bSaveFlag))
    { nResult = SC_RESULT_SuccUnsave;}
    else*/
    {
        paramSaveType(1);
        if (pParam -> nDataLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pManufID))
        {
            memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pManufID,pParam -> pData,pParam -> nDataLen);
            X_paramGetGlobalAllParam()->pDevInfo->pManufID[pParam -> nDataLen] = '\0';
            X_paramPlaceKeyword((char*)"ManufactureID",(char*)X_paramGetGlobalAllParam()->pDevInfo->pManufID,TRUE);
        }
        X_paramSaveDeviceInfo();
        paramSaveType(0);  
    }
    return nResult;

}
int SettingProductName_Get(sc_param_st *pParam)
{ 
    int  nVal,nResult = SC_RESULT_SUCC;
//    UTDeviceINFO deviceInfo;
       
    if( !pParam )
    { return SC_RESULT_FAIL; }
   
    switch(pParam->nConfType)
    {
        case 0://RAM
            nVal = strlen((char*)X_paramGetGlobalAllParam()->pDevInfo->pManufID);
            if (nVal > 0)
                *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf),"%s",X_paramGetGlobalAllParam()->pDevInfo->pManufID);
            else
                *(pParam->pOutLen) = 0;
       
            break;
            
        case 1://SYS
            nVal = strlen((char*)X_paramBakGetGlobalAllParam()->pDevInfo->pManufID);
            if (nVal > 0)
                *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf),"%s",X_paramBakGetGlobalAllParam()->pDevInfo->pManufID);
            else
                *(pParam->pOutLen) = 0;
     
            break;
            
        case 2://FAC
         /*   if(X_FacParamIsGet() != TRUE)
                return SC_RESULT_FAIL;
         
            memcpy(&deviceInfo, FacParamBuf+Start_MEMSize_u, sizeof(HardParam));
                nVal = hHard.nBeepType;   
                if((nVal&0xf00) ==  0x900)
                {
                    memcpy(&cCode2, FacParamBuf+Start_MEMSize_CODE2, sizeof(CodeParam2));
                    dur = cCode2.nSucBeepDuration;   
                }*/
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }            
    return nResult;
}

//删除产品名称		Tag:PDNDEL
int DeleteProductName_Set(sc_param_st *pParam)
{         
    int nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

 
    X_paramPlaceKeyword((char*)"ManufactureID", (char*)"", 1);
    X_paramSaveDeviceInfo();

    /*if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else*/
    {
        paramSaveType(1);
        X_paramPlaceKeyword((char*)"ManufactureID", (char*)"", 1);
        X_paramSaveDeviceInfo();
        paramSaveType(0);  
    }
    
    return nResult;
}

int DeleteProductName_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}

int ProductNameLength_Set(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    char cBuff[16];
    unsigned int nLen;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }

    nVal = sizeof(X_paramGetGlobalAllParam()->pDevInfo->pManufID) - 1;

    nLen = sprintf(cBuff, "%d", nVal);
    memcpy(pParam->pOutBuf, cBuff, nLen);
    *(pParam->pOutLen) = nLen;

    return nResult;

}

int ProductNameLength_Get(sc_param_st *pParam)
{return SC_RESULT_SUCC;}
//Is Download 使能设置
int IsDownloadEnable_Set(sc_param_st *pParam)
{
    int  nVal, nResult = SC_RESULT_Save;

    if( !pParam )
    { 
        return SC_RESULT_FAIL; 
    }

    if(!pParam -> nDataLen || pParam -> nDataLen > 1)
    { 
        return SC_RESULT_FAIL; 
    }

    if(!IsDec(pParam -> pData, pParam -> nDataLen))
    { 
        return SC_RESULT_FAIL; 
    }

    nVal = Dec2Int(pParam -> pData, pParam -> nDataLen);
    if(nVal < 0 || nVal > 1)
    { 
        return SC_RESULT_FAIL; 
    }  

    X_paramGetGlobalAllParam()->pDevInfo->IsDownload = 0x30 + nVal;
    X_paramPlaceKeyword((char*)"IsDownload",(char*)&(X_paramGetGlobalAllParam()->pDevInfo->IsDownload),TRUE);
    X_paramSaveDeviceInfo();


    {
        paramSaveType(1);
        
        X_paramBakGetGlobalAllParam()->pDevInfo->IsDownload = 0x30 + nVal;
        X_paramPlaceKeyword((char*)"IsDownload",(char*)&(X_paramBakGetGlobalAllParam()->pDevInfo->IsDownload),TRUE);
        X_paramSaveDeviceInfo();
        
        paramSaveType(0);  
    }

    return nResult;

}
int IsDownloadEnable_Get(sc_param_st *pParam)
{
    int  nVal=0,nResult = SC_RESULT_SUCC;
//    HardParam hHard;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    switch(pParam->nConfType)
    {
        case 0://RAM
            nVal = X_paramGetGlobalAllParam()->pDevInfo->IsDownload;
            break;
        case 1://SYS
            nVal = X_paramBakGetGlobalAllParam()->pDevInfo->IsDownload;
            break;
        case 2://FAC
   
            break; 
        default:
            return SC_RESULT_UnSurpport;
            break;
    }

    pParam->pOutBuf[0] = nVal;
    *(pParam->pOutLen) = 1;

    return nResult;
}

//删除IS Download设置
int DeleteCFGOfIsDownload_Set(sc_param_st *pParam)
{         
   /* int  nVal, flag=0,nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

 
    X_paramPlaceKeyword((char*)"IsDownload", (char*)"", 1);
    X_paramSaveDeviceInfo();

 
    {
        paramSaveType(1);
        X_paramPlaceKeyword((char*)"IsDownload", (char*)"", 1);
        X_paramSaveDeviceInfo();
        paramSaveType(0);  
    }
    */
    return SC_RESULT_UnSurpport;
}

int DeleteCFGOfIsDownload_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//设置制造商名称		Tag:MFNEDT
int SettingManufacturerName_Set(sc_param_st *pParam)
{
   int nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam -> nDataLen))
    {
        g_ReplaceType = Replace_NewInstruction;
        pParam -> nDigitMaxCnt = 20;

        if(pParam -> bCompleteFlag == 0)
            return SC_RESULT_NSTR;  
    }

    if (pParam -> nDataLen < sizeof(X_paramGetGlobalAllParam()->pDevInfo->pManufName))
    {
        memcpy(X_paramGetGlobalAllParam()->pDevInfo->pManufName,pParam -> pData,pParam -> nDataLen);
        X_paramGetGlobalAllParam()->pDevInfo->pManufName[pParam -> nDataLen] = '\0';
        X_paramPlaceKeyword((char*)"ManufactureName",(char*)X_paramGetGlobalAllParam()->pDevInfo->pManufName,TRUE);
    }
    X_paramSaveDeviceInfo();


    /*if(!(pParam->bSaveFlag))
    { nResult = SC_RESULT_SuccUnsave;}
    else*/
    {
        paramSaveType(1);
        if (pParam -> nDataLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pManufName))
        {
            memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pManufName,pParam -> pData,pParam -> nDataLen);
            X_paramGetGlobalAllParam()->pDevInfo->pManufName[pParam -> nDataLen] = '\0';
            X_paramPlaceKeyword((char*)"ManufactureName",(char*)X_paramGetGlobalAllParam()->pDevInfo->pManufName,TRUE);
        }
        X_paramSaveDeviceInfo();
        paramSaveType(0);  
    }
    return nResult;

}
int SettingManufacturerName_Get(sc_param_st *pParam)
{ 
    int  nVal,nResult = SC_RESULT_SUCC;
//    UTDeviceINFO deviceInfo;
       
    if( !pParam )
    { return SC_RESULT_FAIL; }
   
    switch(pParam->nConfType)
    {
        case 0://RAM
            nVal = strlen((char*)X_paramGetGlobalAllParam()->pDevInfo->pManufName);
            if (nVal > 0)
                *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf),"%s",X_paramGetGlobalAllParam()->pDevInfo->pManufName);
            else
                *(pParam->pOutLen) = 0;
       
            break;
            
        case 1://SYS
            nVal = strlen((char*)X_paramBakGetGlobalAllParam()->pDevInfo->pManufName);
            if (nVal > 0)
                *(pParam->pOutLen) = sprintf((char*)(pParam->pOutBuf),"%s",X_paramBakGetGlobalAllParam()->pDevInfo->pManufName);
            else
                *(pParam->pOutLen) = 0;
     
            break;
            
        case 2://FAC
         /*   if(X_FacParamIsGet() != TRUE)
                return SC_RESULT_FAIL;
         
            memcpy(&deviceInfo, FacParamBuf+Start_MEMSize_u, sizeof(HardParam));
                nVal = hHard.nBeepType;   
                if((nVal&0xf00) ==  0x900)
                {
                    memcpy(&cCode2, FacParamBuf+Start_MEMSize_CODE2, sizeof(CodeParam2));
                    dur = cCode2.nSucBeepDuration;   
                }*/
                break; 
            default:
                return SC_RESULT_UnSurpport;
                break;
        }            
    return nResult;
}

//删除制造商名称		Tag:MFNDEL
int DeleteManufacturerName_Set(sc_param_st *pParam)
{         
    int nResult = SC_RESULT_Save;

    if( !pParam )
    { return SC_RESULT_FAIL; }

 
    X_paramPlaceKeyword((char*)"ManufactureName", (char*)"", 1);
    X_paramSaveDeviceInfo();

    /*if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    else*/
    {
        paramSaveType(1);
        X_paramPlaceKeyword((char*)"ManufactureName", (char*)"", 1);
        X_paramSaveDeviceInfo();
        paramSaveType(0);  
    }
    
    return nResult;
}

int DeleteManufacturerName_Get(sc_param_st *pParam)
{return SC_RESULT_SUCC;}

int ManufacturerNameLength_Set(sc_param_st *pParam)
{
    int  nVal,nResult = SC_RESULT_SUCC;
    char cBuff[16];
    unsigned int nLen;

    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }
    nVal = sizeof(X_paramGetGlobalAllParam()->pDevInfo->pManufName) - 1;

    nLen = sprintf(cBuff, "%d", nVal);
    memcpy(pParam->pOutBuf, cBuff, nLen);
    *(pParam->pOutLen) = nLen;

    return nResult;

}

int ManufacturerNameLength_Get(sc_param_st *pParam)
{return SC_RESULT_SUCC; }
    
//设置产品硬件信息		Tag:HWVEDT
int SettingInfoOfHardware_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }
int SettingInfoOfHardware_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//删除产品硬件信息		Tag:HWVDEL
int DeleteInfoOfHardware_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }

int DeleteInfoOfHardware_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }

int InfoOfHardwareLength_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }

int InfoOfHardwareLength_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }

//设置产品真实名称		Tag:PRNEDT
int SettingRealNameOfProduct_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int SettingRealNameOfProduct_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
//删除产品真实名称		Tag:PRNDEL
int DeleteRealNameOfProduct_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int DeleteRealNameOfProduct_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int RealNameOfProductLength_Set(sc_param_st *pParam)
    { return SC_RESULT_UnSurpport; }
//查询产品真实名称支持的长度 Tag:PRNQSZ
int RealNameOfProductLength_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }
//查询产品真实名称		Tag:PRQRY
int QueryRealNameOfProduct_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int QueryRealNameOfProduct_Get(sc_param_st *pParam)
{
        return SC_RESULT_UnSurpport;
}
extern int efuse_read_chipID(unsigned int *buf);

unsigned int  HexToASCII(unsigned char *srcbuf, unsigned char *destbuf, unsigned int size)
{
    unsigned int i = 0, tens, unit;
    unsigned char cc, ss=0;
    unsigned char *ps, *pd;

    ps = srcbuf;
    pd = destbuf;
    
    while(i<size)
    {
        cc = *ps++;
        tens = cc/16;
        unit = cc%16;

        if((tens<=9) && (tens>=0))
            ss = tens + 0x30;
        else if((tens<=15) && (tens>=10))
            ss = tens + 0x37;
        
        *pd++ = ss;
        
        if((unit<=9) && (unit>=0))
            ss = unit + 0x30;
        else if((unit<=15) && (unit>=10))
            ss = unit + 0x37;

        *pd++ = ss;
        i++;       
    }
    
    return i*2;
}

//CPUID查询
int QueryCPUID_Set(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_SUCC;
    unsigned char chipIDbuf[16];
    unsigned char chipIDbufEx[32];
    //char *p;
 
    if( !pParam )
    { return SC_RESULT_FAIL; }
//p = pParam->pOutBuf;
    Z_GetCPUID(chipIDbuf);
    
    HexToASCII(chipIDbuf, chipIDbufEx, 16);
 /* printf("\n");
    for(i=0; i<16; i++)
        printf("id=%x ", chipIDbuf[i]);
    printf("\n");

    for(i=0; i<32; i++)
        printf("idex=%x ", chipIDbufEx[i]);
    printf("\n");*/
    
    memcpy(pParam->pOutBuf, chipIDbufEx, 32); 
     *(pParam->pOutLen) = 32;
     
 /*for(i=0; i<32; i++)
        printf("out=%x ", pParam->pOutBuf[i]);
    printf("\n");*/
    return nResult;
    
}

int QueryCPUID_Get(sc_param_st *pParam)
{
        return SC_RESULT_UnSurpport;
}

int Encryption_Set(sc_param_st *pParam)
{
    int nResult = SC_RESULT_SUCC;
    //unsigned char buf[4096];
    unsigned char *lvp, *p;
    unsigned int i;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(!(pParam->nDataLen))
    { return SC_RESULT_FAIL; }

    if(!pParam->pData)
    { return SC_RESULT_FAIL; }

    lvp = (unsigned char*)malloc(pParam->nDataLen);
    //lvp = buf;
    //printf("seclen=%d\n", pParam->nDataLen);
    memcpy(lvp, pParam->pData, pParam->nDataLen);
    p = lvp;
    for(i=0; i<pParam->nDataLen; i++)
    {
        if(*p == 'A')    
            *p = ' ';    
        if(*p == 'B')
        {
            Z_EncryptionWrite(lvp, i, 0);//0公钥，1密文
            break;
        }
        p++;
    }
    //printf("i=%d\n",i);
    Z_EncryptionWrite(lvp+i+1, (pParam->nDataLen)-i-1, 1);//写密文
    //printf("len=%d\n",(pParam->nDataLen)-i-1);
    
    /*p = lvp;
    for(i=0; i<(pParam->nDataLen); i++)
    printf("%x ", *p++);*/
    free((MEM_ULONG)lvp);
    return nResult;

    
}

int Encryption_Get(sc_param_st *pParam)
{
    int nResult = SC_RESULT_SUCC;
    unsigned char buf[4096];
    unsigned char *lvp, *pBuf;
    
    unsigned int i, pkeylen=0, encrylen=0 ;
    
    if( !pParam )
    { return SC_RESULT_FAIL; }

    Z_EncryptionRead(buf, 4096, 0);//读公钥

    for(i=0; i<4096; i++)
    {    
        if(buf[i] == 0xff)
        {
            pkeylen = i;
            break;
        }
    }

    for(i=0; i<pkeylen; i++)
    {
        if(buf[i] == 0x20)
        {
            buf[i] = 'A';
            break;
        }
    }
    buf[pkeylen] = 'B';

    lvp = (unsigned char*)malloc(4096);
    Z_EncryptionRead(lvp, 4096, 1);//读密文
    for(i=0; i<4096; i++)
    {    
        if(lvp[i] == 0xff)
        {
            encrylen = i;
            break;
        }
    }    
    
    pBuf= (unsigned char*)malloc(pkeylen + encrylen + 1);
    memcpy(pBuf, buf, pkeylen+1);
    memcpy(pBuf+pkeylen+1, lvp, encrylen);

    memcpy(pParam->pOutBuf, pBuf, pkeylen + encrylen + 1);
    *(pParam->pOutLen) = pkeylen + encrylen + 1;
//printf("len=%d\n", *(pParam->pOutLen));
    free((MEM_ULONG)lvp);
    free((MEM_ULONG)pBuf);
    return nResult;

}


/****************************************************************************
*@景深设置
****************************************************************************/
//近、中、远三种景深的设置			Tag:DOFLVL
int DepthOfFieldInProduct_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int DepthOfFieldInProduct_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//删除景深配置参数					Tag：DELDOF
int DelDepthOfFieldInProduct_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int DelDepthOfFieldInProduct_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
/****************************************************************************
*@MCU运行频率		
****************************************************************************/
//设置MCU的运行频率		Tag：HWRFRQ
//const unsigned short c_nMCUFreqSelTab[2] = {0, eMORERVD_FreqFlag};
int RunningFreqOfMCU_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int RunningFreqOfMCU_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//运行无线模式		Tag：RUNWEL
int RunningWirelessMode_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int RunningWirelessMode_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//设置USB枚举PID--SN时是否使用设备SN	Tag：PIDUSN
//const unsigned short c_nUSBEnumOnPSNEnTab[2] = {0, eCMMT_NO_SN};
int USBEnumratePIDSNWithSN_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int USBEnumratePIDSNWithSN_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//USB接口与串口的切换方式（按指定要求切换或自适应）	Tag：AUTOUR
//const unsigned short c_nUSBAutoAdaptEnTab[2] = {0, eCMMT_AUTOMODE};
int AutoAdaptBetweenSerialCommAndUSB_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int AutoAdaptBetweenSerialCommAndUSB_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//查询设备指令接受缓存大小		Tag：BUFQRY
int QueryBuffSizeOfInstructions_Set(sc_param_st *pParam)
{     
    int nResult = SC_RESULT_SUCC;
//    HardParam hHard;
    char cBuff[16];
    unsigned int nLen;
    if( !pParam )
    { return SC_RESULT_FAIL; }

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }
    //nVal = sizeof(pParam->pData);
    
    nLen = sprintf(cBuff, "%d", 512);
    memcpy(pParam->pOutBuf, cBuff, nLen);
    *(pParam->pOutLen) = nLen;

    return nResult; 
}


int QueryBuffSizeOfInstructions_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}


//设置Febraban字符间延时的使能、禁止	Tag：FEBSEN
int FebrabanInterCharDelayEnable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int FebrabanInterCharDelayEnable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//设置Febraban字符间延时时间	Tag:FEBSDT
int FebrabanInterCharDelay_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int FebrabanInterCharDelay_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//设置Febraban每12字符间延时的使能、禁止	Tag：FEBMEN
int FebrabanInterAreaDelayEnable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int FebrabanInterAreaDelayEnable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//设置Febraban每12字符间延时时间	Tag:FEBMDT
int FebrabanInterAreaDelay_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int FebrabanInterAreaDelay_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
