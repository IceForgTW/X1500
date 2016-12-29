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

//ͼ��������ʱ����
//static int gImgGetMode = 0;	//ͼ���ȡ��ʽ: 0:ֱ�ӻ�ȡ����ͼ������	
static int gImgType = 0;		//ͼ����ʽ: 0:ԭʼͼ��
static int gImgColor = 0;		//ͼ����ɫ: 0:�Ҷ�ͼ
static int gImgReduceRow = 0;	//ͼ��ˮƽѹ��: 0: "1:1"
static int gImgReduceColumn = 0;	//ͼ��ֱѹ��: 0: "1:1"
//static int gImgCutStartRow = 0;		//�ü�ͼ����ʼ��
//static int gImgCutStartColumn = 0;		//�ü�ͼ����ʼ��
//static int gImgCutEndRow = 0;		//�ü�ͼ�������
//static int gImgCutEndColumn = 0;		//�ü�ͼ�������
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
#define CUR_KEY_MSG_CNT				10				//ESN / S/N DATE ��

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
// OEM Firmware Param    //�ṩ��OEM���̵Ĳ����洢λ�ã���ʼ�ڴ�飬�洢����
#define OEM_BLOCK_VALID_SIZE	    1024
//У������pCode2�е�ƫ��
#define	  CRC_OFFSET_V1		 72 //4 + 32 + 32 + 4

#define  PARAMVERSION	0x0001

#endif

#define DECODESUCCVOICE 1
#define POWNONVOICE 2

#define DECODESUCCVOL 0xF0
#define DECODESUCCLAST 0xF00
#define DECODESUCCFREQ 0xF000

//////////////////////////////////////////////
#define Max_nPreSubfix          	10				//ǰ��׺�����Ϣ����
#define Max_FixedLen            	2				//����̶����ȵĲ�������
#define Max_DataCutLen             	20				//��Ϣ��ȡ������󳤶�
#define Max_Delay					3600000			//���ʱ���ӳ�
#define Max_Sensibility             20              //�й����������ֵ
#define Max_nTerminal				2				//����������󳤶�ֵ

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
*@��ƷĬ������
****************************************************************************/
//���س���Ĭ������	Tag:FACDEF
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
     
    //���
    if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
    { 
        return SC_RESULT_FAIL; 
    }
    
    //������
    if (Y_paramLoadFactory(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE) 
    { 
        return SC_RESULT_FAIL; 
    }
    
    //��������
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
//�����û�Ĭ������	Tag:@CUSDEF/#CUSDEF
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

    //���
    if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
    { 
        return SC_RESULT_FAIL; 
    }

    //������
    if (Y_paramLoadUsrDefault(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE) 
    { 
        return SC_RESULT_FAIL; 
    }

    //��������
    if (X_paramGetParamFromBuff(byBuff,FLASH_BLOCK_VALID_SIZE) != TRUE)
    { 
        return SC_RESULT_FAIL; 
    }

    //�����豸����
    X_paramUpdateToDevice();

    //���ö��ο�����"�����û�Ĭ��"
    if (X_param2DevLoadUserDefault != NULL)
    { 
        X_param2DevLoadUserDefault(); 
    }

     if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
     else
        X_SysParamCpy();
     

    //��ȡ�������������ɹ�
    return nResult;

}

//�����û�Ĭ������	Tag:@CUSSAV/#CUSSAV
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
    
    //���
    if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
    { 
        return SC_RESULT_FAIL; 
    }

    //�����豸����
    X_paramUpdateToDevice();

    //������д��������
    nSize = X_paramPushParamToBuff(byBuff,FLASH_BLOCK_SIZE);
    if (nSize <= 0)
    { 
        return SC_RESULT_FAIL; 
    }
	
    //�洢����
    if (Y_paramSaveUsrDefault(byBuff,nSize) != TRUE) 
    { 
        return SC_RESULT_FAIL; 
    }

    //���ο����ġ��洢�û�Ĭ�ϡ�
    if (X_param2DevSaveUserDefault != NULL)
    { 
        X_param2DevSaveUserDefault(); 
    }

    //if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    //else
        //X_SysParamCpy();
     
    //�洢�ɹ�
    return nResult;
}
//ɾ���û�Ĭ������
int DeleteUserDefaultCFG_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//��ѯ�û�Ĭ������

int SaveUserDefaultCFG_Get(sc_param_st *pParam){ return SC_RESULT_SUCC; }
int DeleteUserDefaultCFG_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }


//�������Ĭ������	Tag:SAVEFY
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
    //���
    if (FLASH_BLOCK_SIZE < FLASH_BLOCK_VALID_SIZE)
    { 
        return SC_RESULT_FAIL; 
    }
	
    //�����豸����
    X_paramUpdateToDevice();
	
    //������д��������
    nSize = X_paramPushParamToBuff(byBuff,FLASH_BLOCK_SIZE);
    if (nSize <= 0)
    { 
        return SC_RESULT_FAIL; 
    }
	
    //�洢����
    if (Y_paramSaveFactory(byBuff,nSize) != TRUE) 
    { 
        return SC_RESULT_FAIL; 
    }

    if(X_paramGetFactoryParam(FacParamBuf, FLASH_BLOCK_SIZE) == TRUE)//���³�ʼ������Ĭ�ϲ�������
            g_FacParamGet = TRUE;
    else
        g_FacParamGet = FALSE;
     
    //if(!(pParam->bSaveFlag))
        nResult = SC_RESULT_SuccUnsave;
    //else
        //X_SysParamCpy();
    //�洢�ɹ�
    return nResult;
}
int SaveFactoryDefaultCFG_Get(sc_param_st *pParam){ return SC_RESULT_SUCC; }

//����ͨ��Ĭ������	Tag:GENDEF
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
//���ڸ�ͨѶ�ӿ����͵��л�����	Tag:INTERF(0~10)
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
//���ò����� Tag:232BAD const unsigned int
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
      g_commChange = 1;//Ӧ����ٸ����豸����
	//X_paramUpdateToDevice();

	if (!(pParam->bSaveFlag))//������
	{
		nResult = SC_RESULT_SuccUnsave;
	}
	else //����
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

//����RS232У������ Tag:232PAR
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

	if (!(pParam->bSaveFlag))//������
	{
		nResult = SC_RESULT_UnsaveAndUpdateAll;
	}
	else//����
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

//����RS232ֹͣλ Tag��232STP
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

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_UnsaveAndUpdateAll;
    else//����
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

//RS232��������λ�� Tag:232DAT
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

    if(!(pParam->bSaveFlag)) //������
        nResult = SC_RESULT_UnsaveAndUpdateAll;
    else//����
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

//����RS232Ӳ���Զ����ص�ʹ�ܺͽ�ֹ	Tag:232AFL
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

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_UnsaveAndUpdateAll;
    else//����
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

//����Keyboard Country Layout���������ԣ�������	Tag:KBWCTY
int KBWCountryLayout_Set(sc_param_st *pParam)
{
        int  nVal, nResult = SC_RESULT_Save;

        if( !pParam )
        { return SC_RESULT_FAIL; }

        //if((!pParam->bCompleteFlag) && (!pParam -> nDataLen))
        if(!pParam -> nDataLen)
        {//��Ҫ������ 
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

        if(!(pParam->bSaveFlag))//������
            nResult = SC_RESULT_SuccUnsave;
        else//����
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

//���ڴ���δ֪�ַ�����ʾ��ʹ�ܡ���ֹ	Tag:KBWBUC
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
	if (nVal < 0 || nVal > 1)		//0:��ֹ��1��ʹ��
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

//���������������1�Ľ�ֹ����������	Tag:KBWALT
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
	if (nVal < 0 || nVal > 1)		//0:��ֹ��1��ʹ��
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

//���������������2��ʹ�ܡ���ֹ	Tag��KBWALK

int KBWEmulateAltKeypad2_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int KBWEmulateAltKeypad2_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//���ڹ��ܰ������Ctrl+ASCII��ʹ�ܺͽ�ֹ	Tag:KBWFKM
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
	if (nVal < 0 || nVal > 1)		//0:��ֹ��1��ʹ��
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

//�����ڲ����������ʱ������	Tag:KBWDLY
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

	if (pParam->nDataLen > 3 )		//ʱ�䳤�����100��3�ַ�
	{
		return SC_RESULT_FAIL;
	}

	nVal = Dec2Int(pParam->pData, pParam->nDataLen);
	if (nVal < 0 || nVal > 100)		//ʱ�䳤�����100��3�ַ�
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

//���ڼ���Caps Lock����ʹ�ܡ���ֹ	Tag:KBWCAP
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
	if (nVal < 0 || nVal > 1)		//orderֵ <= 6
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

//���ڼ��̵Ĵ�Сдת�����ܵ�ʹ�ܡ���ֹ	Tag��KBWCAS
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

//���������������ּ��̵��������	Tag:KBWNUM
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

//���������������ּ��̵������"+,-,*"�Ƿ���ò���Numeric Keypad ��ֵ 	Tag:KBWNCH
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

//�������ÿ���ģʽ�ǿ������߹ر�
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

//����KBW��ѯ�ٶȵ����úͲ�ѯ Tag:KBWPOR
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

//����KBW CODE PAGE���úͲ�ѯ  Tag:KBWCPG
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
*@����ǹ���������
****************************************************************************/
//���ڻָ������������� Tag:BASFDF
int LoadFactoryCFGOfPedestal_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//���ڵ�����������Ϣ Tag:BASCLP
int ClearConfigInfoOfPedestal_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//���ڲ�ѯ����ɨ�������ȫ����Ϣ	Tag WLSQAL
int AllInfoInWirelessScanner_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//���ڲ�ѯ����ϵͳ��Ϣ	Tag BASQSY
int SysInfoOfPedestal_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//���ڲ�ѯ�����̼��汾��	Tag BASQFW
int FirmwareVerOfPedestal_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//���ڲ�ѯ������Ʒ�ͺ�	Tag BASQHN
int ProductTypeOfPedestal_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//���ڲ�ѯ������Ʒ���к�	Tag BASQSN
int SerialNumOfPedestal_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//���ڲ�ѯ������Ʒ��������	Tag BASQDA
int DateOfPedestal_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����Batch Mode������	Tag��WLSBAT
int BatchModesOfWirelessScanner_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BatchModesOfWirelessScanner_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//���ڲ�ѯ�û�FLASH����������	Tag:WLSQFC
int AmountOfCodesInUserFlash_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//��������û�FLASH������		Tag:WLSCLF
int ClearCodesInUserFlash_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//���ڿ������رպ˶Թ���		Tag��WLSCHK
int CheckingFunctionEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int CheckingFunctionEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//�������ú˶Գ�ʱʱ��			Tag��WLSCHT
int TimeoutOfCheckingFunction_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TimeoutOfCheckingFunction_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//�������ô���ʱ���Խ����Զ��ػ� Tag��WLSAPO
int IdleTimeForAutoPowerdown_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int IdleTimeForAutoPowerdown_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//��������Zigbee�����ŵ�	Tag��WLSZCH
int SelectChannelOfZigbee_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int SelectChannelOfZigbee_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//���ڵ�������				Tag��BASRES
int ResetPedestal_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//�����������ߴ���Ӧ��ʱʱ�� 	Tag��WLSTTM
int TimeoutForACKInWirelessTrmt_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TimeoutForACKInWirelessTrmt_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@�����ơ���׼���������
****************************************************************************/
//���������ƵĿ�����	Tag��ILLSCN

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

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//���������Ƶ���ɫ��� 	Tag��ILLCLR
int ColorTypeOfIllumination_Set(sc_param_st *pParam)
{ 
        return SC_RESULT_UnSurpport; 
}
int ColorTypeOfIllumination_Get(sc_param_st *pParam)
{ 
        return SC_RESULT_UnSurpport; 
}

//���������Ƶ�λ��		Tag��ILLPST
int PositionOfIllumination_Set(sc_param_st *pParam)
{ 
        return SC_RESULT_UnSurpport; 
}
int PositionOfIllumination_Get(sc_param_st *pParam)
{ 
        return SC_RESULT_UnSurpport; 
}

//������׼�ƵĿ�����	Tag��AMLENA
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

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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
*@���ڴ���ָ��
****************************************************************************/
//�������ô��ڴ���ָ��	Tag:SCNTRG
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

    if (nVal == 1)	//����
    {
        if(X_paramGetGlobalAllParam()->pCode->nReadMode==ESC0)
        {
            X_paramGetGlobalAllParam()->pCode->nReadMode = ESC1;
       
            X_paramGetGlobalAllParam()->pMode->bContinue = TRUE;
            X_paramGetGlobalAllParam()->pMode->nReadDelay = Z_Get10msCounter();
        }
    }
    else if (nVal == 0)	//�ر�
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
//����ѡ��Sense Mode�¶���ɹ���Ĺ���״̬��After A Good Read��	Tag:SENAGR
//const unsigned short c_nSenseFullTmTab[2] = {0, eRMPE_AUTO_FULLTIME};
int StatusAfterGoodReadInSenseMode_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}
int StatusAfterGoodReadInSenseMode_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}

//����ѡ��Sense Mode�����״̬ʱ�����ƵĿ���	Tag:SENILL
int IlluStatusInSensingForSenseMode_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int IlluStatusInSensingForSenseMode_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@Exposure Imaging
****************************************************************************/
//���þ�ͷ���ع�����		Tag:EXPLVL
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

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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
//���ý�������	Tag��CADENA
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
    {//ȫ����ȡ
        X_paramGetGlobalAllParam()->pCode->decodeArea &= ~0xf;
        X_paramGetGlobalAllParam()->pAreaParam->nType = 0;
    }
    else if(nVal == 1)
    { //���������ȡ
        X_paramGetGlobalAllParam()->pCode->decodeArea |= 1;//�ɰ汾���������������
        X_paramGetGlobalAllParam()->pAreaParam->nType = 1;//�°汾���������������
    }

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//������������Ķ��˷�Χ	Tag��CADTOP, �����Bottom��ֵС
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


    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//������������ĵ׶˷�Χ	Tag��CADBOT,�����top��
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


    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//���������������˷�Χ	Tag��CADLEF,�����right��ֵС
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


    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//��������������Ҷ˷�Χ	Tag��CADRIG
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


    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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
*@��������ѡ��1D��Ʒ��
****************************************************************************/
//����ѡ�� �����м�����������һ����������������м������һ����������� 
//���ߡ��̶�ʶ��2��3��4�����롱
int DecoAreaLevelsFor1D_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int DecoAreaLevelsFor1D_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@���������ã�1D��Ʒ��
****************************************************************************/
//����ѡ��ÿ�����������̶����ʱ��	Tag��CDAINV
int IntervalTimeOfOutputingFor1D_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int IntervalTimeOfOutputingFor1D_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@Image Mirror(��ģ�����Ʒ���ṩ)							Tag:MIRROR
****************************************************************************/
//����ѡ��ÿ�����������̶����ʱ��	Tag��CDAINV
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
		
    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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
//����KBW��ѯ�ٶȵ����úͲ�ѯ
/*int KBWPollRate_Set(sc_param_st *pParam)
{}
int KBWPollRate_Get(sc_param_st *pParam)
{}

//����KBW CODE PAGE���úͲ�ѯ
int KBWCodePage_Set(sc_param_st *pParam)
{}
int KBWCodePage_Get(sc_param_st *pParam)
{}*/

//��������PS2����PCʱ�������Ƿ����Ӽ���
//const unsigned short c_nPS2KBConnectTab[2] = {eCMMT_PS2_INTERNAL, 0};
int PS2KeyboardConect_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int PS2KeyboardConect_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//��������PS2��CLKƵ��
int PS2KeyboardClock_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int PS2KeyboardClock_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//�������˳����������ù���
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
    
    if(nVal == 0)//�˳�����
    {
        if (X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff)
            return SC_RESULT_FAIL;//�����빦��δ����,��ʾ��
        W_setcEnable(FALSE);
        W_setcSetResult(U_uIMG_STATUS_SETCODE_END);
        //printf("T1");
    }
    else//��������
    {
        if (X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff)
            return SC_RESULT_FAIL;
        W_setcEnable(TRUE);//W_setcIsEnableʹ��������ʱ��Ҫ�Ըñ��������ж�
        W_setcSetResult(U_uIMG_STATUS_SETCODE_START);//W_setcGetResult,������ʾ����Ҫ�Ըñ��������ж�
        //printf("T0");
    }
    
    return nResult;
}
int StartExitSetup_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//�����͹ر����������ù��ܣ������ȼ���
//const unsigned short c_nSetupCodeEnLoTab[2] = {0, eEDIT_EnableSettingCode};
int OpenCloseSetup_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int OpenCloseSetup_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//������ʶ�����ܣ������ȼ���
//const unsigned short c_nSetupCodeEnHiTab[2] = {0, eMORERVD_EnableSettingCode};
//����ʶ��������TAG:FOBSET
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
//���������ݷ��͹���,TAG:SETUP

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
//VoiceType:1,����ɹ��������أ�2,������������
    pParam->pOutBuf[0] = (((nVal & nVoiceType)>>(nVoiceType-1))?0x31:0x30);
    *(pParam->pOutLen) = 1;

    return nResult;
}
 
//������������
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

//�ɹ�ʶ����ʾ��ʹ������
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

//�ɹ�ʶ����ʾ����������
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
//0:��,1:��,2:��
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
    //����:0:�ߣ�1:��,2:��
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

//�ɹ�ʶ����ʾ����������
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
        X_paramGetGlobalAllParam()->pHard->nBeepType |= (9 << 8);   //������ʱ����beeptype�е�mask��Ϊ9,����ʶ�����Զ������
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
            
            case 0x900://�Զ���
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



//�ɹ�ʶ����ʾ��Ƶ������
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
        X_paramGetGlobalAllParam()->pHard->nBeepType |= (9 << 12);   //������ʱ����beeptype�е�mask��Ϊ9,����ʶ�����Զ������
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
            
            case 8://�Զ���
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
//�ɹ���ʾ���Ĵ�������
int GoodReadBeepNumber_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GoodReadBeepNumber_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//�ɹ���ʾ���ļ��ʱ������
int GoodReadBeepIntval_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GoodReadBeepIntval_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//ʧ����ʾ����ʹ������
int BadReadBeepEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//ʧ��ʶ����ʾ����������
int BadReadBeepVolume_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepVolume_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//ʧ��ʶ����ʾ����������
int BadReadBeepDuration_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepDuration_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//ʧ��ʶ����ʾ��Ƶ������
int BadReadBeepFreq_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepFreq_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//ʧ����ʾ���Ĵ�������
int BadReadBeepNumber_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepNumber_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//ʧ����ʾ���ļ��ʱ������
int BadReadBeepIntval_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BadReadBeepIntval_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//�������Ź���ʹ������
//const unsigned char c_nVoiceEnableTab[2] = {eVOICE_DISABLE, 0};
int VoiceEnable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int VoiceEnable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//�������ż��ʱ������
int VoiceIntvalTime_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int VoiceIntvalTime_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//������������   CMD:VOICET
int VoiceTypeSelect_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int VoiceTypeSelect_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//����ģʽ����   CMD:VOICEM
int VoiceModeSelect_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int VoiceModeSelect_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//������������   CMD:VOICEV
int VoiceVolume_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int VoiceVolume_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//�ɹ�ʶ����ʹ������
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

    if (nVal == 1)	//����
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLED &= ~ 1;
    }
    else if (nVal == 0)	//�ر�
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLED |= 1;
    }

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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
//����:0:�ߣ�1:��,2:��
    nVal &= 0x1;
    if(nVal == 1)
        nVal = 0x30;
    else
        nVal = 0x31;
    pParam->pOutBuf[0] = nVal;
    *(pParam->pOutLen) = 1;

    return nResult;
}

//�ɹ�ʶ���Ƴ���ʱ������
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


    if (nVal == 20)//��
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 0;
    }
    else if (nVal == 120)//��
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 1;				
    }
    else if (nVal == 220)//��
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 2;
    }
    else if (nVal == 320)//�
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 3;
    }
    else 
    {
        X_paramGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 4;
        temp = 4;
        X_paramGetGlobalAllParam()->pCode2->nSucLedDuration = nVal;
    }

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//�ɹ�ʶ����ʹ������
int GoodReadVibrateEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GoodReadVibrateEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//�ɹ�ʶ���𶯳���ʱ������
int GoodReadVibrateDuration_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GoodReadVibrateDuration_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//������ʾ��ʹ������:POLENA
//const unsigned short c_nPowerOnLedTab[2] = {0, ePROMPT_LED_POWER};
int PowerOnLedEnable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int PowerOnLedEnable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//����ģʽѡ������
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
        case 0://�ֶ�
            X_paramGetGlobalAllParam()->pCode->nReadMode = ESC0;
            X_paramGetGlobalAllParam()->pMode->bContinue = TRUE;
            break;
        case 2://��Ӧ
            X_paramGetGlobalAllParam()->pCode->nReadMode = ESC2;
            X_paramGetGlobalAllParam()->pMode->bContinue = TRUE;
            X_paramGetGlobalAllParam()->pMode->nReadDelay = Z_Get10msCounter();
            X_paramGetGlobalAllParam()->pMode->nHaltDelay = Z_Get10msCounter();
            X_paramGetGlobalAllParam()->pMode->nCodeDelay = Z_Get10msCounter();
            break;
        case 3://����
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

//���ģ�ⴥ������
int PureSoftTrig_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int PureSoftTrig_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//�����Ъʱ������
int RestTime_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int RestTime_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//��Ӧģʽ�£��ȴ�ͼ���ȶ���ʱ������
//const short c_nImgStableTmParam[3] = {1, 100, 100};
int ImgStabTimeout_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}
int ImgStabTimeout_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}

//����ʱ������
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


//��ͬ������ʱ��������
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

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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
//����:0:�ߣ�1:��,2:��
    pParam->pOutBuf[0] = nVal+0x30;
    *(pParam->pOutLen) = 1;

    return nResult;
}


//��ͬ������ʱʱ������
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

//��Ъģʽ�µĶ���ʱ������
int FlashOnTime_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int FlashOnTime_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//��Ъģʽ�µļ�Ъʱ������
int FlashOffTime_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int FlashOffTime_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//��Ӧ����������
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

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
        X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode = 0;
  
    return SC_RESULT_UnSurpport;

}
int Sensitivity_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//Not Good Readʹ������
//const unsigned short c_nNotGoodReadEnTab[2] = {0, eEDIT_EnableNG};
int NotGoodReadEnable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int NotGoodReadEnable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//Not Good Read ��Ϣ����
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

//�ػ�ģʽ
//const unsigned int c_nModePowerOffTab[1] = {eAPP_MODE_SHUTDOWN};
int ModePowerOff_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int ModePowerOff_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
//Idleģʽ
//const unsigned int c_nModeIdleTab[1] = {eAPP_MODE_IDLE};
int ModeIdle_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int ModeIdle_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
//Sleepģʽ
//const unsigned int c_nModeSleepTab[1] = {eAPP_MODE_SLEEP};
int ModeSleep_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int ModeSleep_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
//testģʽ
int ModeTest_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ModeTest_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//Rebootģʽ
int ModeReboot_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int ModeReboot_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//�Զ����߹���ѡ��
//const unsigned short c_nAutoSleepEnTab[2] = {0, eRMP_AUTOSLEEP};
int AutoSleepEnable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AutoSleepEnable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//�Զ����߿���ʱ������
int AutoSleepIdleTime_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AutoSleepIdleTime_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//��ȫ��������
int SafeLv_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int SafeLv_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/*********************************************************************************************/
//////////////////////////////////////////////////////////////////////////
//��ѯϵͳ��ϢQRYSYS
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
    sprintf(buff+strlen(buff),"Hardware Version: 1.0\r\n");//////////�޿ɲ�ѯ��Ӳ���汾

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
//��ѯ�̼��汾��
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
//��ѯ����汾��
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
//��ѯӲ����Ϣ
int HardwareVer_Set(sc_param_st *pParam)
{
    int nResult = SC_RESULT_SUCC;
    char buff[64]={0};
    int nSize;

    if(pParam->nDataLen)
    { return SC_RESULT_FAIL; }

    nSize = sprintf(buff,"Hardware Version: 1.0\r\n");//////////�޿ɲ�ѯ��Ӳ���汾

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
//��ѯ��Ʒ����
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
//��ѯ��Ʒ���к�
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
//��ѯ��Ʒ��������
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

//��ѯ��Ʒ��������
int ProduceDate_Get(sc_param_st *pParam)
{
    return SC_RESULT_SUCC;
}

//��ѯ��ƷOEM�ͺ�
int ProductOEMType_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int ProductOEMType_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//��ѯ��ƷOEM���кţ�ESN��
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

//��ѯ��ƷOEM��������
int ProduceOEMDate_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ProduceOEMDate_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//��������ʹ������
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

    if (nVal == 1)	//ʹ����������
    {
        X_paramGetGlobalAllParam()->p1D->unDecTypes = 0xffffffff;
        X_paramGetGlobalAllParam()->p2D->unDecTypes = 0xffffffff;
        X_paramGetGlobalAllParam()->pPostal->unDecTypes = 0xffffffff;
        X_paramGetGlobalAllParam()->pOCR->unDecTypes = 0xffffffff;
    }
    else //��ֹ��������
    {
        X_paramGetGlobalAllParam()->p1D->unDecTypes = 0x03; //ֻʶ��������
        X_paramGetGlobalAllParam()->p2D->unDecTypes = 0;
        X_paramGetGlobalAllParam()->pPostal->unDecTypes = 0;
        X_paramGetGlobalAllParam()->pOCR->unDecTypes = 0;
    }

    X_paramUpdateToDevice();
    if(!(pParam->bSaveFlag))//������
    {
        nResult = SC_RESULT_SuccUnsave;
    }
    else//����
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

// ʹ��/��ֹ����һά����
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
        
    if (nVal == 1)	//ʹ������һά����
    {
        X_paramGetGlobalAllParam()->p1D->unDecTypes = 0xffffffff;
    }
    else//��ֹ����һά����
    {
        X_paramGetGlobalAllParam()->p1D->unDecTypes = 0x03; //ֻʶ��������:�а���setup128
    }

    X_paramUpdateToDevice();
    
    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//���ж�ά����ʹ������	Tag:ALL2DC
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
        
    if (nVal == 1)	//ʹ������һά����
    {
        X_paramGetGlobalAllParam()->p2D->unDecTypes = 0xffffffff;
    }
    else//��ֹ���ж�ά����
    {
        X_paramGetGlobalAllParam()->p2D->unDecTypes = 0;
    }

    X_paramUpdateToDevice();

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//������������ʹ������	Tag:ALLPST
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
        
    if (nVal == 1)	//ʹ������һά����
    {
        X_paramGetGlobalAllParam()->pPostal->unDecTypes = 0xffffffff;
    }
    else//��ֹ���ж�ά����
    {
        X_paramGetGlobalAllParam()->pPostal->unDecTypes = 0;
    }

    X_paramUpdateToDevice();
    
    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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
        case 0://������һά��
            X_paramGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
            break;
        case 1://�ɶ���һά�룬Ҳ�ɶ�˫һά��
            X_paramGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
            X_paramGetGlobalAllParam()->p1D->unDecParam |= 0x02; 
            break;
        case 2://����˫һά��
            X_paramGetGlobalAllParam()->p1D->unDecParam |= 0x03;
            break;

        default:
            nResult = SC_RESULT_UnSurpport;
            break;
    }
    X_paramUpdateToDevice();
    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//ʶ����������ʹ������	Tag:ALLINV
int InverterOfCodesEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int InverterOfCodesEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }


//����EANUPC����������
int AllEanUpcAddon_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int AllEanUpcAddon_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }




/****************************************************************************
*@��ͨ����
****************************************************************************/
//��������Ĭ������
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

//�������á���ȡ
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

	if (nVal == 0)//��ֹ
	{
		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << nCodeType));
	}
	else//ʹ��
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
	      if (nVal == 0)//��ֹ
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

//ȡֵ���á���ȡ __BarcodeParamValue_Set
int __1DCodeParamValue_Set(sc_param_st *pParam,
							unsigned short nCodeType,
							unsigned int nDataMin,			//Э��{Data}����Сֵ
							unsigned int nDataMax,			//Э��{Data}�����ֵ
							unsigned int nSetBitMsk,		//��Ӧ���ܵ�����λ��ʶ
							void		*pSetTal			//����ָ��˳�����еĹ�������ֵ
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
							unsigned int nDataMin,			//Э��{Data}����Сֵ
							unsigned int nDataMax,			//Э��{Data}�����ֵ
							unsigned int nSetBitMsk,		//��Ӧ���ܵ�����λ��ʶ
							void		*pSetTal			//����ָ��˳�����еĹ�������ֵ
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

//�����С�������á���ȡ
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

//Code ID���á���ȡ
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
*@��ͨCode 128
****************************************************************************/
//����Code 128 Ĭ������	Tag��128DEF
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

//Code 128��ʹ�ܡ���ֹ	Tag��128ENA
int Code128Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_CODE128);
}

int Code128Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_CODE128);
}

//����Code 128��С���ȵ�����	Tag��128MIN
int Code128MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_CODE128);
}

int Code128MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_CODE128);
}

//����Code 128����󳤶�	Tag��128MAX
int Code128MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_CODE128);
}

int Code128MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_CODE128);
}

//����Code 128У��λ���͵�ʹ�ܡ���ֹ	Tag��128CHK
//const unsigned int c_nCode128TransChkTab[2] = {0, eB1D_CODE128_TRSMTCHK};
int Code128ChkTrsmt_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int Code128ChkTrsmt_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����Code 128��FNC1�����ʹ�ܡ���ֹ	Tag��128FNC
//const unsigned int c_nCode128FNC1Tab[2] = {0, eB1D_CODE128_FNC1};
int Code128FNC1_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int Code128FNC1_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����Code 128 CODEID Tag��CID002
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

//����EAN-8��ʹ�ܡ���ֹ	Tag:EA8ENA
int EAN8Enable_Set(sc_param_st * pParam)
{
	return __1DCodeEnable_Set(pParam, POS_EAN8);
}

int EAN8Enable_Get(sc_param_st * pParam)
{
	return __1DCodeEnable_Get(pParam, POS_EAN8);
}

//����EAN-8У��λ��ʹ�ܡ���ֹ	Tag��EA8CHK
const unsigned int nEAN8TrsmtTab[3] = {0,0x00,0x01};
int EAN8ChkTrsmt_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN8,
									1,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN8TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int EAN8ChkTrsmt_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN8,
									1,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN8TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����EAN-8 2λ��չ���ʹ�ܡ���ֹ	Tag:EA8AD2
const unsigned int nEAN8AddOn2Tab[2] = {0, 0x02};
int EAN8AddCode2Enable_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN8,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x02,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN8AddOn2Tab	//����ָ��˳�����еĹ�������ֵ
									);

}

int EAN8AddCode2Enable_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN8,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x02,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN8AddOn2Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����EAN-8 5λ��չ���ʹ�ܡ���ֹ	Tag:EA8AD5
const unsigned int nEAN8AddOn5Tab[2] = {0, 0x04};
int EAN8AddCode5Enable_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN8,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN8AddOn5Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

int EAN8AddCode5Enable_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN8,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN8AddOn5Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����EAN-8��չ���ʹ�ܡ���ֹ	Tag:EA8REQ
//const unsigned int c_nEAN8AddOnReqTab[2] = {0, eB1D_EAN8_ADDONREQ};
int EAN8AddedCodeEnable_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN8AddedCodeEnable_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//����EAN-8��չ���Ƿ���ӷָ���	Tag:EA8SEP
int EAN8SEPRInAddedCodeEnable_Set(sc_param_st * pParam)
{
        return SC_RESULT_UnSurpport;
}

int EAN8SEPRInAddedCodeEnable_Get(sc_param_st * pParam)
{
        return SC_RESULT_UnSurpport;
}

//����EAN-8�Ƿ�ת��ΪEAN13��������չΪ13λ	Tag��EA8EXP
const unsigned int nEAN8Exp2EAN13Tab[2] = {0,0x10};
int EAN8ExpandedType_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN8,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x10,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN8Exp2EAN13Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

int EAN8ExpandedType_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN8,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x10,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN8AddOn5Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����EAN-8 CODEID�����ʹ�ܡ���ֹ	Tag��CID004
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
//����EAN13��Ĭ��ͨ������	Tag��E13DEF
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

//����EAN13��ʹ�ܡ���ֹ	Tag��E13ENA
int EAN13Enable_Set(sc_param_st * pParam)
{
	return __1DCodeEnable_Set(pParam, POS_EAN13);
}

int EAN13Enable_Get(sc_param_st * pParam)
{
	return __1DCodeEnable_Get(pParam, POS_EAN13);
}

//����EAN13У��λ��ʹ�ܡ���ֹ	Tag��E13CHK
const unsigned int nEAN13TrsmtTab[3] = { 0,0x00,0x01 };
int EAN13ChkTrsmt_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN13,
									1,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN13TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int EAN13ChkTrsmt_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN13,
									1,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN13TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����EAN13 2λ ��չ���ʹ�ܡ���ֹ  Tag:E13AD2
const unsigned int nEAN13AddOn2Tab[2] = { 0, 0x02 };
int EAN13AddCode2Enable_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN13,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x02,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN13AddOn2Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

int EAN13AddCode2Enable_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN13,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x02,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN13AddOn2Tab	//����ָ��˳�����еĹ�������ֵ
									);
}


//����EAN13 5λ ��չ���ʹ�ܡ���ֹ  Tag:E13AD5
const unsigned int nEAN13Addon5Tab[2] = {0, 0x04};
int EAN13AddCode5Enable_Set(sc_param_st * pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_EAN13,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN13Addon5Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

int EAN13AddCode5Enable_Get(sc_param_st * pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_EAN13,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nEAN13Addon5Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����EAN13��չ���ʹ�ܡ���ֹ	Tag:E13REQ
int EAN13AddedCodeEnable_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13AddedCodeEnable_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//����EAN13��չ���Ƿ���ӷָ���	Tag:E13SEP
int EAN13SEPRInAddedCodeEnable_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13SEPRInAddedCodeEnable_Get(sc_param_st * pParam)
{
        return SC_RESULT_UnSurpport;
}

//����EAN13 Beginning with 290 Addenda Tag��E132900�� E132901
//const unsigned int c_nEAN13290AddonReqTab[2] = {0, eB1D_EAN13_290AddOnReq};
int EAN13Beggin290Addenda_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin290Addenda_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//����EAN13 Beginning with 378_379 Addenda Tag��E132780�� E132781
//const unsigned int c_nEAN13378AddonReqTab[2] = {0, eB1D_EAN13_378AddOnReq};
int EAN13Beggin378_379Addenda_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin378_379Addenda_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//����EAN13 Beginning with 414_419 Addenda Tag��E134140�� E134141
//const unsigned int c_nEAN13414AddonReqTab[2] = {0, eB1D_EAN13_414AddOnReq};
int EAN13Beggin414_419Addenda_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin414_419Addenda_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//����EAN13 Beginning with 434_439 Addenda Tag��E134340�� E134341
//const unsigned int c_nEAN13434AddonReqTab[2] = {0, eB1D_EAN13_434AddOnReq};
int EAN13Beggin434_439Addenda_Set(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin434_439Addenda_Get(sc_param_st * pParam)
{
	return SC_RESULT_UnSurpport;
}

//����EAN13 Beginning with 977 Addenda Tag��E139770�� E139771
//const unsigned int c_nEAN13977AddonReqTab[2] = {0, eB1D_EAN13_977AddOnReq};
int EAN13Beggin977Addenda_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin977Addenda_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����EAN13 Beginning with 978 Addenda Tag��E139780�� E139781
//const unsigned int c_nEAN13978AddonReqTab[2] = {0, eB1D_EAN13_978AddOnReq};
int EAN13Beggin978Addenda_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin978Addenda_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����EAN13 Beginning with 979 Addenda Tag��E139790�� E139791
//const unsigned int c_nEAN13979AddonReqTab[2] = {0, eB1D_EAN13_979AddOnReq};
int EAN13Beggin979Addenda_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int EAN13Beggin979Addenda_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����EAN-13 CODEID�����ʹ�ܡ���ֹ	Tag��CID005
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
//����UPC-E��Ĭ������	Tag:UPEDEF
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

//����UPC-E��ʹ�ܡ���ֹ	Tag��UPEENA
int UPCEEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_UPCE);
}

int UPCEEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_UPCE);
}


//����UPC-E��UPC-E0ʹ�ܡ���ֹ	Tag��UPEEN0
int UPCE0Enable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
int UPCE0Enable_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_UnSurpport;
}

//����UPC-E��UPC-E1ʹ�ܡ���ֹ	Tag��UPEEN1
int UPCE1Enable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int UPCE1Enable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//����UPC-EУ��λ��ʹ�ܡ���ֹ	Tag��UPECHK
const unsigned int nUPCETrsmtTab[3] = {0,0,0x01};
int UPCEChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCE,
									1,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCETrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int UPCEChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCE,
									1,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCETrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����UPC-E 2λ��չ���ʹ�ܡ���ֹ Tag:UPEAD2
const unsigned int nUPCEAddOn2Tab[2] = { 0, 0x02 };
int UPCEAddCode2Enable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCE,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x02,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCEAddOn2Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

int UPCEAddCode2Enable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCE,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x02,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCEAddOn2Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����UPC-E 5λ��չ���ʹ�ܡ���ֹ Tag:UPEAD5
const unsigned int nUPCEAddOn5Tab[2] = { 0, 0x04 };
int UPCEAddCode5Enable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCE,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCEAddOn5Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

int UPCEAddCode5Enable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCE,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCEAddOn5Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����UPC-E��չ���ʹ�ܡ���ֹ	Tag:UPEREQ
//const unsigned int c_nUPCEAddonReqTab[2] = { 0, eB1D_UPCE_ADDONREQ };
int UPCEAddedCodeEnable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int UPCEAddedCodeEnable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����UPC-E��չ���Ƿ���ӷָ���	Tag:UPESEP
int UPCESEPRInAddedCodeEnable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int UPCESEPRInAddedCodeEnable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//����UPC-E ����ǰ�ĵ�����	Tag��UPEPRE
const unsigned int nUPCEPreTab[3] = { 0,0x08,0x108 };
int UPCETrsmtSysDigitType_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCE,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x108,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCEPreTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int UPCETrsmtSysDigitType_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCE,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x108,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCEPreTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����UPC-Eʹ�ܡ���ֹת��ΪUPC-A Tag��UPEEXP
const unsigned int nUPCEExpTab[3] = { 0, 0x80,0x10};
int UPCEExpandedType_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCE,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x90,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCEExpTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int UPCEExpandedType_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCE,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x90,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCEExpTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����UPC-E CODEID�����ʹ�ܡ���ֹ	Tag��CID006
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
//����UPC-A��Ĭ������	Tag:UPADEF
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

//����UPC-A��ʹ�ܡ���ֹ	Tag��UPAENA
int UPCAEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_UPCA);
}

int UPCAEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_UPCA);
}

//����UPC-AУ��λ��ʹ�ܡ���ֹ	Tag��UPACHK
const unsigned int nUPCATrsmtTab[3] = { 0, 0,0x01 };
int UPCAChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									1,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCATrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int UPCAChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									1,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCATrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����UPC-A 2λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:UPAAD2
const unsigned int nUPCAAddOn2Tab[2] = { 0, 0x02 };
int UPCAAddCode2Enable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x02,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCAAddOn2Tab	//����ָ��˳�����еĹ�������ֵ
									);
}
int UPCAAddCode2Enable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x02,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCAAddOn2Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����UPC-A 5λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:UPAAD5
const unsigned int nUPCAAddOn5Tab[2] = { 0, 0x04 };
int UPCAAddCode5Enable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCAAddOn5Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

int UPCAAddCode5Enable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCAAddOn5Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����UPC-E��չ���ʹ�ܡ���ֹ	Tag:UPAREQ
//const unsigned int c_nUPCAAddonReqTab[2] = { 0, eB1D_UPCA_ADDONREQ };
int UPCAAddedCodeEnable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
int UPCAAddedCodeEnable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����UPC-A��չ���Ƿ���ӷָ���	Tag:UPASEP
int UPCASEPRInAddedCodeEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int UPCASEPRInAddedCodeEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����UPC-A ����ǰ�ĵ�����	Tag��UPAPRE		
const unsigned int nUPCAPreTab[3] = {0x200,0x0, 0x08};
int UPCATrsmtSysDigitType_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x208,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCAPreTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int UPCATrsmtSysDigitType_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x208,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCAPreTab	//����ָ��˳�����еĹ�������ֵ
									);
}


//����UPC-A CODEID�����ʹ�ܡ���ֹ	Tag��CID007
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
//����UPCA/EAN13���Extended Coupon Code��״̬��off,allow,require���֣�Tag:CPNENA
const unsigned int nUPCA2CouponTab[3] = {0xc0,0x40,0x80};
int UPCA_EAN13WithExtendedCoupon_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0xc0,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCA2CouponTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int UPCA_EAN13WithExtendedCoupon_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0xc0,			//��Ӧ���ܵ�����λ��ʶ
									&nUPCA2CouponTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//Coupon GS1 DataBar Output Tag:CPNGS1
const unsigned int nCouponGS1Tab[2] = {0,0x100};
int CouponGS1DataBarOutput_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_UPCA,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x100,			//��Ӧ���ܵ�����λ��ʶ
									&nCouponGS1Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

int CouponGS1DataBarOutput_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_UPCA,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x100,			//��Ӧ���ܵ�����λ��ʶ
									&nCouponGS1Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

/****************************************************************************
*@Interleaved 2 of 5
****************************************************************************/
//����Inter2of5��Ĭ������	Tag:I25DEF
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

//����Inter2of5��ʹ�ܡ���ֹ	Tag��I25ENA
int Inter2of5Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_INTER2OF5);
}
int Inter2of5Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_INTER2OF5);
}

//����Inter2of5��ʶ����С����	Tag��I25MIN
int Inter2of5MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_INTER2OF5);
}

int Inter2of5MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_INTER2OF5);
}

//����Inter2of5��ʶ����󳤶�	Tag��I25MAX
int Inter2of5MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_INTER2OF5);
}

int Inter2of5MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_INTER2OF5);
}

//����Inter2of5У��λ��ʹ�ܡ���ֹ	Tag��I25CHK
const unsigned int nInter25TrsmtTab[3] = { 0, 0x01,0x03 };
int Inter2of5ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_INTER2OF5,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nInter25TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int Inter2of5ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_INTER2OF5,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nInter25TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����ʶ���̶��������빦�ܵ�ʹ�ܡ���ֹ	Tag:I25LEN
int Inter2of5DecoFixedLenEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int Inter2of5DecoFixedLenEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����Inter2of5��ӹ̶�����ֵ	Tag:I25LSE
int Inter2of5AddFixedLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int Inter2of5AddFixedLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����Inter2of5ȡ���̶�����ֵ	Tag:I25LCA
int Inter2of5CancelFixedLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int Inter2of5CancelFixedLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����Inter2of5 ��Febraban ʹ��̬��������Ϣ��չ	Tag:I25FBB
//const unsigned int c_nInter25FBBTab[3] = { 0, eB1D_ITF_SPECEDIT, eB1D_ITF_SPECEDIT | eB1D_ITF_TOFULL };
int Inter2of5FebrabanEnable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int Inter2of5FebrabanEnable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����Inter2of5 CODEID�����ʹ�ܡ���ֹ	Tag��CID008
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
//����ITF-14 ��Ĭ������	Tag:I14DEF
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

//����ITF-14 ��ʹ�ܡ���ֹ��У��λ�Ĵ���	Tag��I14ENA
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

	if (nVal == 0)//��ֹ
	{
		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_ITF14));
	}
	else if(nVal == 1)//ʹ��
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

//����ITF-14 CODEID�����ʹ�ܡ���ֹ	Tag��CID009
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
//����ITF-6 ��Ĭ������	Tag:IT6DEF
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

//����ITF-6 ��ʹ�ܡ���ֹ��У��λ�Ĵ���	Tag��IT6ENA
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

	if (nVal == 0)//��ֹ
	{
		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_ITF6));
	}
	else if (nVal == 1)//ʹ��
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

//����ITF-6 CODEID�����ʹ�ܡ���ֹ	Tag��CID010
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
//����Matrix 2 of 5 ��Ĭ������	Tag:M25DEF
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

//����Matrix 2 of 5 ��ʹ�ܡ���ֹ	Tag��M25ENA
int Matrix2of5Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_MATRIX25);
}

int Matrix2of5Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_MATRIX25);
}

//����Matrix 2 of 5��ʶ����С����	Tag��M25MIN
int Matrix2of5MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_MATRIX25);
}
int Matrix2of5MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_MATRIX25);
}

//����Matrix 2 of 5��ʶ����󳤶�	Tag��M25MAX
int Matrix2of5MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_MATRIX25);
}

int Matrix2of5MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_MATRIX25);
}

//����Matrix 2 of 5У��λ��ʹ�ܡ���ֹ	Tag��M25CHK
const unsigned int nMatr25TrsmtTab[3] = { 0,0x02,0x03 };
int Matrix2of5ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_MATRIX25,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nMatr25TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int Matrix2of5ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_MATRIX25,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nMatr25TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Matrix 2 of 5 CODEID�����ʹ�ܡ���ֹ	Tag��CID011
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
//����Code39��Ĭ������	Tag:C39DEF
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

//����Code39��ʹ�ܡ���ֹ	Tag��C39ENA
int Code39Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_REGULAR39);
}

int Code39Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_REGULAR39);
}

//����Code39��ʶ����С����	Tag��C39MIN
int Code39MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_REGULAR39);
}

int Code39MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_REGULAR39);
}

//����Code39��ʶ����󳤶�	Tag��C39MAX
int Code39MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_REGULAR39);
}
int Code39MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_REGULAR39);
}

//����Code39У��λ��ʹ�ܡ���ֹ	Tag��C39CHK
const unsigned int nCode39TrsmtTab[3] = { 0,0x01,0x03 };
int Code39ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_REGULAR39,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nCode39TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int Code39ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_REGULAR39,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nCode39TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Code39�Ĵ�����ʼ������ֹ����ʹ�ܡ���ֹ	Tag��C39TSC
const unsigned int nCode39TSCTab[2] = { 0,0x04 };
int Code39TrsmtSTXETXEnable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_REGULAR39,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nCode39TSCTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int Code39TrsmtSTXETXEnable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_REGULAR39,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nCode39TSCTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Code39 Full ASCII��ʹ�ܡ���ֹ	Tag��C39ASC
const unsigned int nCode39ASCTab[2] = { 0,0x08 };
int Code39FullASCIIEnable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_REGULAR39,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x08,			//��Ӧ���ܵ�����λ��ʶ
									&nCode39ASCTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int Code39FullASCIIEnable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_REGULAR39,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x08,			//��Ӧ���ܵ�����λ��ʶ
									&nCode39ASCTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Code39��Code 32 Pharmaceutical (PARAF)��ʹ�ܡ���ֹ	Tag��C39E32
const unsigned int nCode39E32Tab[2] = {0,0x20 };
int Code39Code32PARAFEnable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_REGULAR39,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x20,			//��Ӧ���ܵ�����λ��ʶ
									&nCode39E32Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

int Code39Code32PARAFEnable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_REGULAR39,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x20,			//��Ӧ���ܵ�����λ��ʶ
									&nCode39E32Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Code39 �Ƿ����Code32��ʼ��	Tag:C39S32
const unsigned int nCode39S32Tab[2] = { 0,0x10 };
int Code39AddSTXOfCode32Enable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_REGULAR39,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x10,			//��Ӧ���ܵ�����λ��ʶ
									&nCode39S32Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

int Code39AddSTXOfCode32Enable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_REGULAR39,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x10,			//��Ӧ���ܵ�����λ��ʶ
									&nCode39S32Tab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Code39 �Ƿ����Code32��ʼ��/��ֹ��	Tag:C39T32
int Code32TrsmtSTXETXEnable_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}
int Code32TrsmtSTXETXEnable_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}    

//����Code39 �Ƿ����Code32У���	Tag:C39C32
int Code32ChkTrsmt_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}
int Code32ChkTrsmt_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;}

//����Code39 CODEID�����ʹ�ܡ���ֹ	Tag��CID013
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
//����Codabar��Ĭ������	Tag:CBADEF
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

//����Codabar��ʹ�ܡ���ֹ	Tag��CBAENA
int CodabarEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_CODABAR);
}

int CodabarEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_CODABAR);
}

//����Codabar��ʶ����С����	Tag��CBAMIN
int CodabarMinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_CODABAR);
}

int CodabarMinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_CODABAR);
}

//����Codabar��ʶ����󳤶�	Tag��CBAMAX
int CodabarMaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_CODABAR);
}

int CodabarMaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_CODABAR);
}

//����CodabarУ��λ��ʹ�ܡ���ֹ	Tag��CBACHK
const unsigned int nCodebarTrsmtTab[3] = { 0,0x01,0x03};
int CodabarChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODABAR,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nCodebarTrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int CodabarChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODABAR,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nCodebarTrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Codabar������ʼ������ֹ����ʹ�ܡ���ֹ	Tag��CBATSC
const unsigned int nCodabarTSCTab[2] = { 0, 0x04 };
int CodabarTrsmtSTXETXEnable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODABAR,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nCodabarTSCTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int CodabarTrsmtSTXETXEnable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODABAR,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nCodabarTSCTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Codabar��ʼ������ֹ���ĸ�ʽ	Tag��CBASCF
const unsigned int nCodabarSCFTab[4] = {0, BIT_CODA_ABCDTNXE ,BIT_CODA_LOWERCASE, BIT_CODA_ABCDTNXE | BIT_CODA_LOWERCASE };
int CodabarSTXETXFormats_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODABAR,
									0,				//Э��{Data}����Сֵ
									3,				//Э��{Data}�����ֵ
									BIT_CODA_ABCDTNXE | BIT_CODA_LOWERCASE,			//��Ӧ���ܵ�����λ��ʶ
									&nCodabarSCFTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int CodabarSTXETXFormats_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODABAR,
									0,				//Э��{Data}����Сֵ
									3,				//Э��{Data}�����ֵ
									BIT_CODA_ABCDTNXE | BIT_CODA_LOWERCASE,			//��Ӧ���ܵ�����λ��ʶ
									&nCodabarSCFTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Codabar��CLSI�༭��ʹ�ܡ���ֹ	Tag:CBACLS
int CodabarCLSIEdit_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int CodabarCLSIEdit_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����Codabar CODEID�����ʹ�ܡ���ֹ	Tag��CID015
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
//����Code93��Ĭ������	Tag:C93DEF
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


//����Code93��ʹ�ܡ���ֹ	Tag��C93ENA
int Code93Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_CODE93);
}

int Code93Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_CODE93);
}

//����Code93��ʶ����С����	Tag��C93MIN
int Code93MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_CODE93);
}
int Code93MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_CODE93);
}

//����Code93��ʶ����󳤶�	Tag��C93MAX
int Code93MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_CODE93);
}

int Code93MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_CODE93);
}

//����Code93У��λ��ʹ�ܡ���ֹ	Tag��C93CHK
const unsigned int nCode93TrsmtTab[3] = { 0, 0x01,0x03 };
int Code93ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODE93,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nCode93TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int Code93ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODE93,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nCode93TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}


//����Code93 CODEID�����ʹ�ܡ���ֹ	Tag��CID017
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
//����GS1-128��Ĭ������	Tag:GS1DEF
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

//����GS1-128��ʹ�ܡ���ֹ	Tag��GS1ENA
int GS1_128Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_UCCEAN128);
}

int GS1_128Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_UCCEAN128);
}


//����GS1-128��ʶ����С����	Tag��GS1MIN
int GS1_128MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_UCCEAN128);
}

int GS1_128MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_UCCEAN128);
}

//����GS1-128��ʶ����󳤶�	Tag��GS1MAX
int GS1_128MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_UCCEAN128);
}

int GS1_128MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_UCCEAN128);
}

//����GS1-128У��λ��ʹ�ܡ���ֹ	Tag��GS1CHK
//const unsigned int nGS1_128TrsmtTab[2] = { 0, eB1D_UCCEAN128_TRSMTCHK };
int GS1_128ChkTrsmt_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int GS1_128ChkTrsmt_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����GS1-128��FNC1�����ʹ�ܡ���ֹ	Tag��GS1FNC
//const unsigned int c_nGS1_128FNC1Tab[2] = { 0, eB1D_UCCEAN128_FNC1 };
int GS1_128OutputFNC1_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int GS1_128OutputFNC1_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����GS1-128 CODEID�����ʹ�ܡ���ֹ	Tag��CID003
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
//����GS1 DataBar(RSS)��Ĭ������	Tag:RSSDEF
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

//����GS1 DataBar(RSS)��ʹ�ܡ���ֹ	Tag:RSSENA
int RSSEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_RSS);
}

int RSSEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_RSS);
}


//����GS1 DataBar(RSS)����AI(01)�ַ���ʹ�ܡ���ֹ Tag:RSSTAI
const unsigned int nRSSAITab[2] = { 0, 0x01 }; 
int RSSTrsmtAIEnable_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_RSS,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nRSSAITab	//����ָ��˳�����еĹ�������ֵ
									);
}

int RSSTrsmtAIEnable_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_RSS,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nRSSAITab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����GS1 DataBar(RSS) GS1 DataBar Omnidirectional�ַ���ʹ�ܡ���ֹ Tag:RSSE14
int RSSConvert2RSS14_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int RSSConvert2RSS14_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����GS1 DataBar(RSS)��GS1 DataBar Limitedʹ�ܡ���ֹ Tag:RSSENL
int RSSForGS1DatabarLimited_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int RSSForGS1DatabarLimited_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����GS1 DataBar(RSS)��GS1 DataBar Limitedʹ�ܡ���ֹ Tag:RSSENE
int RSSForGS1DatabarExpand_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int RSSForGS1DatabarExpand_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����GS1 DataBar(RSS) CODEID�����ʹ�ܡ���ֹ	Tag��CID031
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
//����GS1 Composite��Ĭ������	Tag:CPTDEF
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

//����GS1 Composite��ʹ�ܡ���ֹ	Tag:CPTENA
int GS1_CompositeEnable_Set(sc_param_st *pParam)
{ 
	return __1DCodeEnable_Set(pParam, POS_COMPOSITE);
}

int GS1_CompositeEnable_Get(sc_param_st *pParam)
{ 
	return __1DCodeEnable_Get(pParam, POS_COMPOSITE);
}

//����GS1 Composite��UPC/EAN�汾��ʹ�ܡ���ֹ Tag:CPTUPC
const unsigned int nComp4UPCEANTab[2] = {0,0x01};
int GS1_CompositeForUPC_EAN_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_COMPOSITE,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nComp4UPCEANTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int GS1_CompositeForUPC_EAN_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_COMPOSITE,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nComp4UPCEANTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����GS1 Composite CODEID�����ʹ�ܡ���ֹ	Tag��CID030
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
//����Code11��Ĭ������		Tag:C11DEF
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

//����Code11��ʹ�ܡ���ֹ	Tag��C11ENA
int Code11Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_CODE11);
}

int Code11Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_CODE11);
}

//����Code11��ʶ����С����	Tag��C11MIN
int Code11MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_CODE11);
}

int Code11MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_CODE11);
}

//����Code11��ʶ����󳤶�	Tag��C11MAX
int Code11MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_CODE11);
}
int Code11MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_CODE11);
}

//����Code11У��λ��ʹ�ܡ���ֹ��У��λ��ѡ��	Tag��C11CHK
const unsigned int nCode11TrsmtTab[6] = { 0x00,0x01,0x02,0x03,0x04,0x05 };					
int Code11ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODE11,
									0,				//Э��{Data}����Сֵ
									5,				//Э��{Data}�����ֵ
									0x07,			//��Ӧ���ܵ�����λ��ʶ
									&nCode11TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int Code11ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODE11,
									0,				//Э��{Data}����Сֵ
									5,				//Э��{Data}�����ֵ
									0x07,			//��Ӧ���ܵ�����λ��ʶ
									&nCode11TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Code11У��λ�Ƿ���	Tag��C11TCK		eB1D_CODE11_TRSMTCHK
const unsigned int nCode11TransACTTab[2] = {0x00, 0x08 };
int Code11ChkTrsmtAction_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CODE11,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x08,			//��Ӧ���ܵ�����λ��ʶ
									&nCode11TransACTTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int Code11ChkTrsmtAction_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CODE11,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x08,			//��Ӧ���ܵ�����λ��ʶ
									&nCode11TransACTTab	//����ָ��˳�����еĹ�������ֵ
									);
}


//����Code11 CODEID�����ʹ�ܡ���ֹ	Tag��CID028
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
//����ISBN��Ĭ������	Tag:ISBDEF
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

//����ISBN��ʹ�ܡ���ֹ	Tag��ISBENA
int ISBNEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_ISBN);
}

int ISBNEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_ISBN);
}

//����ISBN�ķ������ָ�����10������13����	Tag��ISBT10
const unsigned int nISBNLenTrmstTab[2] = {0x00,0x01};
int ISBNLenOfNUM_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_ISBN,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nCodebarTrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int ISBNLenOfNUM_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_ISBN,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x01,			//��Ӧ���ܵ�����λ��ʶ
									&nCodebarTrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����ISBN 2λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:ISBAD2
//const unsigned int c_nISBNAddOn2Tab[2] = { 0, eB1D_ISBN_ADDON2 };
int ISBNAddCode2Enable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int ISBNAddCode2Enable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����ISBN 5λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:ISBAD5

int ISBNAddCode5Enable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int ISBNAddCode5Enable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����ISBN��չ���ʹ�ܡ���ֹ	Tag:ISBNREQ

int ISBNAddedCodeEnable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
int ISBNAddedCodeEnable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����ISBN��չ���Ƿ���ӷָ���	Tag:ISBSEP
int ISBNSEPRInAddedCodeEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ISBNSEPRInAddedCodeEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����ISBN CODEID�����ʹ�ܡ���ֹ	Tag��CID024
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
//����Industrial 25��Ĭ������		Tag:L25DEF
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

//����Industrial 25��ʹ�ܡ���ֹ	Tag��L25ENA
int IND25Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_INDUSTRIAL25);
}

int IND25Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_INDUSTRIAL25);
}

//����Industrial 25��ʶ����С����	Tag��L25MIN
int IND25MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_INDUSTRIAL25);
}

int IND25MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_INDUSTRIAL25);
}

//����Industrial 25��ʶ����󳤶�	Tag��L25MAX
int IND25MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_INDUSTRIAL25);
}

int IND25MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_INDUSTRIAL25);
}

//����Industrial 25У��λ��ʹ�ܡ���ֹ��У��λ��ѡ��	Tag��L25CHK
const unsigned int nIND25TrsmtTab[3] = { 0, 0x01,0x03 };
int IND25ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_INDUSTRIAL25,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nIND25TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int IND25ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_INDUSTRIAL25,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nIND25TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Industrial 25 CODEID�����ʹ�ܡ���ֹ	Tag��CID025
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
//����Standard 25��Ĭ������		Tag:S25DEF
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

//����Standard 25��ʹ�ܡ���ֹ	Tag��S25ENA
int STD25Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_STANDARD25);
}

int STD25Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_STANDARD25);
}

//����Standard 25��ʶ����С����	Tag��S25MIN
int STD25MinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_STANDARD25);
}

int STD25MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_STANDARD25);
}

//����Standard 25��ʶ����󳤶�	Tag��S25MAX
int STD25MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_STANDARD25);
}
int STD25MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_STANDARD25);
}

//����Standard 25У��λ��ʹ�ܡ���ֹ��У��λ��ѡ��	Tag��S25CHK
const unsigned int nSTD25TransChkTab[3] = { 0, 0x01,0x03 };
int STD25ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_STANDARD25,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nSTD25TransChkTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int STD25ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_STANDARD25,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nSTD25TransChkTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Standard 25 CODEID�����ʹ�ܡ���ֹ	Tag��CID026
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
//����Plessey ��Ĭ������		Tag:PLYDEF
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

//����Plessey ��ʹ�ܡ���ֹ	Tag��PLYENA
int PlesseyEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_PLESSEY);
}

int PlesseyEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_PLESSEY);
}

//����Plessey ��ʶ����С����	Tag��PLYMIN
int PlesseyMinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_PLESSEY);
}

int PlesseyMinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_PLESSEY);
}

//����Plessey ��ʶ����󳤶�	Tag��PLYMAX
int PlesseyMaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_PLESSEY);
}

int PlesseyMaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_PLESSEY);
}

//����Plessey У��λ��ʹ�ܡ���ֹ��У��λ�Ĵ���	Tag��PLYCHK
const unsigned int nPlesseyTrsmtTab[3] = { 0,0x01,0x03 };
int PlesseyChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_PLESSEY,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nPlesseyTrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int PlesseyChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_PLESSEY,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nPlesseyTrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����Plessey CODEID�����ʹ�ܡ���ֹ	Tag��CID027
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
//����MSI Plessey ��Ĭ������		Tag:MSIDEF
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

//����MSI Plessey ��ʹ�ܡ���ֹ	Tag��MSIENA
int MSIPlesseyEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_MSI_PLESSEY);
}
int MSIPlesseyEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_MSI_PLESSEY);
}

//����MSI Plessey ��ʶ����С����	Tag��MSIMIN
int MSIPlesseyMinLen_Set(sc_param_st *pParam)
{
	return __1DCodeMinLen_Set(pParam, POS_MSI_PLESSEY);
}

int MSIPlesseyMinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_MSI_PLESSEY);
}

//����MSI Plessey ��ʶ����󳤶�	Tag��MSIMAX
int MSIPlesseyMaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_MSI_PLESSEY);
}

int MSIPlesseyMaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_MSI_PLESSEY);
}

//����MSI Plessey У��λ��ʹ�ܡ���ֹ��У��λ��ѡ��	Tag��MSICHK
const unsigned int nMSITrsmtTab[4] = { 0x00,0x01,0x02,0x03 };
int MSIPlesseyChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_MSI_PLESSEY,
									0,				//Э��{Data}����Сֵ
									3,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nMSITrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int MSIPlesseyChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_MSI_PLESSEY,
									0,				//Э��{Data}����Сֵ
									3,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nMSITrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����MSI Plessey У��λ�Ƿ���	Tag��MSITCK
const unsigned int nMSITrsmtACTTab[2] = { 0, 0x04 };
int MSIPlesseyChkTrsmtAction_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_MSI_PLESSEY,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nMSITrsmtACTTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int MSIPlesseyChkTrsmtAction_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_MSI_PLESSEY,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									0x04,			//��Ӧ���ܵ�����λ��ʶ
									&nMSITrsmtACTTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����MSI Plessey CODEID�����ʹ�ܡ���ֹ	Tag��CID029
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
//����ISSN��Ĭ������	Tag:ISSDEF
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

//����ISSN��ʹ�ܡ���ֹ	Tag��ISSENA
int ISSNEnable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_ISSN);
}
int ISSNEnable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_ISSN);
}

//����ISSN 2λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:ISSAD2
//const unsigned int c_nISSNAddOn2Tab[2] = { 0, eB1D_ISSN_ADDON2 };
int ISSNAddCode2Enable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int ISSNAddCode2Enable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����ISSN 5λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:ISSAD5
//const unsigned int c_nISSNAddOn5Tab[2] = { 0, eB1D_ISSN_ADDON5 };
int ISSNAddCode5Enable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}
int ISSNAddCode5Enable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����ISSN��չ���ʹ�ܡ���ֹ	Tag:ISSREQ
//const unsigned int c_nISSNREQAddTab[2] = { 0, eB1D_ISSN_REQ2ADDON | eB1D_ISSN_REQ5ADDON };
int ISSNAddedCodeEnable_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int ISSNAddedCodeEnable_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����ISSN��չ���Ƿ���ӷָ���	Tag:ISSSEP
int ISSNSEPRInAddedCodeEnable_Set(sc_param_st *pParam)
{ 
	return SC_RESULT_UnSurpport;
}

int ISSNSEPRInAddedCodeEnable_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_UnSurpport; 
}

//����ISSN CODEID�����ʹ�ܡ���ֹ	Tag��CID023
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
//����China Post 25��Ĭ������	Tag:CHPDEF
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

//����China Post 25��ʹ�ܡ���ֹ	Tag��CHPENA
int CP25Enable_Set(sc_param_st *pParam)
{
	return __1DCodeEnable_Set(pParam, POS_CHNPOST);
}
int CP25Enable_Get(sc_param_st *pParam)
{
	return __1DCodeEnable_Get(pParam, POS_CHNPOST);
}

//����China Post 25 ��ʶ����С����	Tag��CHPMIN
int CP25MinLen_Set(sc_param_st *pParam)
{ 
	return __1DCodeMinLen_Set(pParam, POS_CHNPOST);
}

int CP25MinLen_Get(sc_param_st *pParam)
{
	return __1DCodeMinLen_Get(pParam, POS_CHNPOST);
}

//����China Post 25 ��ʶ����󳤶�	Tag��CHPMAX
int CP25MaxLen_Set(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Set(pParam, POS_CHNPOST);
}
int CP25MaxLen_Get(sc_param_st *pParam)
{
	return __1DCodeMaxLen_Get(pParam, POS_CHNPOST);
}

//����China Post 25 У��λ��ʹ�ܡ���ֹ	Tag��CHPCHK
const int nCP25TrsmtTab[3] = {0x0,0x01,0x03};
int CP25ChkTrsmt_Set(sc_param_st *pParam)
{
	return __1DCodeParamValue_Set(pParam,
									POS_CHNPOST,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nCP25TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int CP25ChkTrsmt_Get(sc_param_st *pParam)
{
	return __1DCodeParamValue_Get(pParam,
									POS_CHNPOST,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									0x03,			//��Ӧ���ܵ�����λ��ʶ
									&nCP25TrsmtTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����China Post 25 CODEID�����ʹ�ܡ���ֹ	Tag��CID019
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
//����AIM 128��Ĭ������	Tag:AIMDEF
int AIM128LoadDefault_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128LoadDefault_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����AIM 128��ʹ�ܡ���ֹ	Tag��AIMENA
int AIM128Enable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128Enable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����AIM 128��ʶ����С����	Tag��AIMMIN
int AIM128MinLen_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128MinLen_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����AIM 128��ʶ����󳤶�	Tag��AIMMAX
int AIM128MaxLen_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128MaxLen_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����AIM 128У��λ�ķ���ʹ�ܡ���ֹ	Tag��AIMCHK
//const unsigned int c_nAIM128TransChkTab[2] = { 0, eB1D_AIM128_TRSMTCHK };
int AIM128ChkTrsmt_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128ChkTrsmt_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����AIM 128 FNC1��ʽ��ʹ�ܡ���ֹ	Tag��AIMFNC
//const unsigned int c_nAIM128FNC1Tab[2] = { 0, eB1D_AIM128_FNC1 };
int AIM128FNC1_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int AIM128FNC1_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����AIM 128 CODEID�����ʹ�ܡ���ֹ	Tag��CID020
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
//����ISBT 128��Ĭ������	Tag:IBTDEF
int ISBT128LoadDefault_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int ISBT128LoadDefault_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����ISBT 128��ʹ�ܡ���ֹ	Tag��IBTENA
int ISBT128Enable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int ISBT128Enable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����ISBT 128 CODEID�����ʹ�ܡ���ֹ	Tag��CID021
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
//����COOP 25��Ĭ������	Tag:COPDEF
int COOP25LoadDefault_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int COOP25LoadDefault_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����COOP 25��ʹ�ܡ���ֹ	Tag��COPENA
int COOP25Enable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int COOP25Enable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����COOP 25��ʶ����С����	Tag��COPMIN
int COOP25MinLen_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int COOP25MinLen_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����COOP 25��ʶ����󳤶�	Tag��COPMAX
int COOP25MaxLen_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int COOP25MaxLen_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����COOP 25У��λ�ķ���ʹ�ܡ���ֹ	Tag��COPCHK
//const unsigned int c_nCOOP25TransChkTab[2] = { 0, eB1D_COOP25_TRSMTCHK };
int COOP25ChkTrsmt_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int COOP25ChkTrsmt_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����COOP 25 CODEID�����ʹ�ܡ���ֹ	Tag��CID022
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
//����Deutsche 14��Ĭ������	Tag:D14DEF
int Deut14LoadDefault_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int Deut14LoadDefault_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����Deutsche 14��ʹ�ܡ���ֹ	Tag��D14ENA
int Deut14Enable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int Deut14Enable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����Deutsche 14У��λ�ķ���ʹ�ܡ���ֹ	Tag��D14CHK
//const unsigned int c_nDP14TransChkTab[2] = { 0, eB1D_DP14_TRSMTCHK };
int Deut14ChkTrsmt_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int Deut14ChkTrsmt_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����Deutsche 14 CODEID�����ʹ�ܡ���ֹ	Tag��CID128
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
//����Deutsche 12��Ĭ������	Tag:D12DEF
int Deut12LoadDefault_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int Deut12LoadDefault_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����Deutsche 12��ʹ�ܡ���ֹ	Tag��D12ENA
int Deut12Enable_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}
int Deut12Enable_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����Deutsche 12У��λ�ķ���ʹ�ܡ���ֹ	Tag��D12CHK
//const unsigned int c_nDP12TransChkTab[2] = { 0, eB1D_DP12_TRSMTCHK };
int Deut12ChkTrsmt_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

int Deut12ChkTrsmt_Get(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
}

//����Deutsche 12 CODEID�����ʹ�ܡ���ֹ	Tag��CID129
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
//����Telepen ��Ĭ������	Tag:TLPDEF
int TelepenLoadDefault_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenLoadDefault_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����Telepen ��ʹ�ܡ���ֹ	Tag��TLPENA
int TelepenEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����Telepen ��ʶ����С����	Tag��TLPMIN
int TelepenMinLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenMinLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����Telepen ��ʶ����󳤶�	Tag��TLPMAX
int TelepenMaxLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenMaxLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����Telepen У��λ�ķ���ʹ�ܡ���ֹ	Tag��TLPCHK
int TelepenChkTrsmt_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenChkTrsmt_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����Telepen �Ľ��������ϣ�Full ASCII��Numeric Only	Tag��TLPASC
int TelepenFullASCIIWithNumericOnly_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenFullASCIIWithNumericOnly_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����Telepen CODEID�����ʹ�ܡ���ֹ	Tag��CID130
int TelepenCodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int TelepenCodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/***************************************************************************
*@������
****************************************************************************/
//��պ껺������
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


//����������Ϣ����ķ��ͷ�ʽ
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
*@2D�������ú���                                                                      
****************************************************************************/
int __2DCodeParamValue_Set(sc_param_st *pParam,
							unsigned short nCodeType,
							unsigned int nDataMin,			//Э��{Data}����Сֵ
							unsigned int nDataMax,			//Э��{Data}�����ֵ
							unsigned int nSetBitMsk,		//��Ӧ���ܵ�����λ��ʶ
							void		*pSetTal			//����ָ��˳�����еĹ�������ֵ
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
							unsigned int nDataMin,			//Э��{Data}����Сֵ
							unsigned int nDataMax,			//Э��{Data}�����ֵ
							unsigned int nSetBitMsk,		//��Ӧ���ܵ�����λ��ʶ
							void		*pSetTal			//����ָ��˳�����еĹ�������ֵ
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

//�������á���ȡ
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

	if (nVal == 0)//��ֹ
	{
		X_paramGetGlobalAllParam()->p2D->unDecTypes &= (~(0x01 << nCodeType));
	}
	else//ʹ��
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
	      if (nVal == 0)//��ֹ
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

//�����С�������á���ȡ
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

//Code ID���á���ȡ
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
//����PDF417 ��Ĭ������	Tag:PDFDEF
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
//����PDF417 ��ʹ�ܡ���ֹ	Tag��PDFENA
int PDF417Enable_Set(sc_param_st *pParam)
{ 
	return __2DCodeEnable_Set(pParam,POS_PDF417);
}

int PDF417Enable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_PDF417);
}

//����PDF417 ��ʶ����С����	Tag��PDFMIN
int PDF417MinLen_Set(sc_param_st *pParam)
{ 
	return __2DCodeMinLen_Set(pParam, POS_PDF417);
}

int PDF417MinLen_Get(sc_param_st *pParam)
{ 
	return __2DCodeMinLen_Get(pParam, POS_PDF417);
}

//����PDF417 ��ʶ����󳤶�	Tag��PDFMAX
int PDF417MaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_PDF417);
}

int PDF417MaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_PDF417);
}

//����PDF417 �ĵ�˫�빦��	Tag��PDFDOU
const int PDF417MulCodeTab[3] = {0x00,0x18,0x10};
int PDF417DecoMultiCode_Set(sc_param_st *pParam)
{ 
    
        return __2DCodeParamValue_Set(pParam,
									POS_PDF417,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_PDF417_DOUBLE,			//��Ӧ���ܵ�����λ��ʶ
									&PDF417MulCodeTab	//����ָ��˳�����еĹ�������ֵ
									);
      
}
int PDF417DecoMultiCode_Get(sc_param_st *pParam) 
{

    if(!pParam->nQueryType)
    {
	    return __2DCodeParamValue_Get(pParam,
									POS_PDF417,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_PDF417_DOUBLE,			//��Ӧ���ܵ�����λ��ʶ
									&PDF417MulCodeTab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }
}

//����PDF417 �ĵ�˫�������ʽ	Tag��PDFDOF
int PDF417OrderOfMultiCode_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int PDF417OrderOfMultiCode_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_UnSurpport;
}

//����PDF417 ������������ʶ����ʹ�ܡ���ֹ	Tag��PDFINV
const int PDF417PhaseTab[3] = {0x0,0x10000,0x20000};
int PDF417PhaseOfCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_PDF417,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_PDF417_Reverse,			//��Ӧ���ܵ�����λ��ʶ
									&PDF417PhaseTab	//����ָ��˳�����еĹ�������ֵ
									);
}

int PDF417PhaseOfCode_Get(sc_param_st *pParam)
{ 
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_PDF417,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_PDF417_Reverse,			//��Ӧ���ܵ�����λ��ʶ
									&PDF417PhaseTab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }

}

//����PDF417 ��ECI���ʹ�ܡ���ֹ	Tag��PDFECI
const int PDF417ECITab[2] = {DC_PDF417_ECICLOSE, 0x0};
int PDF417OutputECI_Set(sc_param_st *pParam) 
{
	return __2DCodeParamValue_Set(pParam,
									POS_PDF417,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_PDF417_ECICLOSE,			//��Ӧ���ܵ�����λ��ʶ
									&PDF417ECITab	//����ָ��˳�����еĹ�������ֵ
									);
	
}

int PDF417OutputECI_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_PDF417,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_PDF417_ECICLOSE,			//��Ӧ���ܵ�����λ��ʶ
									&PDF417ECITab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-1");        
        return SC_RESULT_SUCC;
    }
}

//����PDF417 ���ַ����뷽ʽ	Tag��PDFENC
int PDF417EncodingOfChar_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

int PDF417EncodingOfChar_Get(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport;
}

//����PDF417 CODEID�����ʹ�ܡ���ֹ	Tag��CID032
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
//����QR ��Ĭ������	Tag:QRCDEF
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
//����QR ��ʹ�ܡ���ֹ	Tag��QRCENA
int QREnable_Set(sc_param_st *pParam)
{
	return __2DCodeEnable_Set(pParam, POS_QR);
}
int QREnable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_QR);
}

//����QR ��ʶ����С����	Tag��QRCMIN
int QRMinLen_Set(sc_param_st *pParam)
{
	return __2DCodeMinLen_Set(pParam, POS_QR);
}

int QRMinLen_Get(sc_param_st *pParam)
{
	return __2DCodeMinLen_Get(pParam, POS_QR);
}

//����QR ��ʶ����󳤶�	Tag��QRCMAX
int QRMaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_QR);
}
int QRMaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_QR);
}

//����QR �ĵ�˫�빦��	Tag��QRCDOU
const int QRMulCodeTab[3] = { 0x00,0x08,0x10 };
int QRDecoMultiCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_QR,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_QR_DOUBLE,			//��Ӧ���ܵ�����λ��ʶ
									&QRMulCodeTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int QRDecoMultiCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_QR,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_QR_DOUBLE,			//��Ӧ���ܵ�����λ��ʶ
									&QRMulCodeTab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }
}

//����QR �ĵ�˫�������ʽ	Tag��QRCDOF
int QROrderOfMultiCode_Set(sc_param_st *pParam)
{
	return SC_RESULT_UnSurpport; 
}

int QROrderOfMultiCode_Get(sc_param_st *pParam)
{ 
	return SC_RESULT_UnSurpport;
}

//����QR ��Micro QRʶ��ʹ�ܡ���ֹ	Tag��QRCMCR
const int QRMicroTab[2] = {0x0,0x400000};	//0x400000 = 1 << POS_QR_MicroQR_ON
int QRDecoMicroQR_Set(sc_param_st *pParam)
{
    return SC_RESULT_UnSurpport;
	return __2DCodeParamValue_Set(pParam,
									POS_QR,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									1 << POS_QR_MicroQR_ON,			//��Ӧ���ܵ�����λ��ʶ
									&QRMicroTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int QRDecoMicroQR_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport;
	return __2DCodeParamValue_Get(pParam,
									POS_QR,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									1 << POS_QR_MicroQR_ON,			//��Ӧ���ܵ�����λ��ʶ
									&QRMicroTab	//����ָ��˳�����еĹ�������ֵ
									);
}

//����QR ������������ʶ����ʹ�ܡ���ֹ	Tag��QRCINV
int QRPhaseOfCode_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int QRPhaseOfCode_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//����QR ��ECI���ʹ�ܡ���ֹ	Tag��QRCECI
const int QRECITab[2] = { DC_QR_ECICLOSE,0x0 };
int QROutputECI_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_QR,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_QR_ECICLOSE,			//��Ӧ���ܵ�����λ��ʶ
									&PDF417ECITab	//����ָ��˳�����еĹ�������ֵ
									);
}
int QROutputECI_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_QR,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_QR_ECICLOSE,			//��Ӧ���ܵ�����λ��ʶ
									&PDF417ECITab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-1");        
        return SC_RESULT_SUCC;
    }
}


//����QR ���ַ����뷽ʽ	Tag��QRCENC
int QREncodingOfChar_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int QREncodingOfChar_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//����QR CODEID�����ʹ�ܡ���ֹ	Tag��CID033
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
//����Aztec ��Ĭ������	Tag:AZTDEF
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

//����Aztec ��ʹ�ܡ���ֹ	Tag��AZTENA
int AztecEnable_Set(sc_param_st *pParam)
{
	return __2DCodeEnable_Set(pParam, POS_Aztec);
}

int AztecEnable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_Aztec);
}

//����Aztec ��ʶ����С����	Tag��AZTMIN
int AztecMinLen_Set(sc_param_st *pParam)
{
	return __2DCodeMinLen_Set(pParam, POS_Aztec);
}
int AztecMinLen_Get(sc_param_st *pParam)
{
	return __2DCodeMinLen_Get(pParam, POS_Aztec);
}

//����Aztec ��ʶ����󳤶�	Tag��AZTMAX
int AztecMaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_Aztec);
}
int AztecMaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_Aztec);
}

//����Aztec �Ķ���ͬͼ���������	Tag��AZTMUL
const int AZTECNumOfMultiTab[9] = {0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
int AztecNumOfMultiCode_Set(sc_param_st *pParam)
{
        return __2DCodeParamValue_Set(pParam,
									POS_Aztec,
									1,				//Э��{Data}����Сֵ
									8,				//Э��{Data}�����ֵ
									DC_AZTEC_MULTINUM,			//��Ӧ���ܵ�����λ��ʶ
									&AZTECNumOfMultiTab	//����ָ��˳�����еĹ�������ֵ
									);
 
    
}
int AztecNumOfMultiCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_Aztec,
									1,				//Э��{Data}����Сֵ
									8,				//Э��{Data}�����ֵ
									DC_AZTEC_MULTINUM,			//��Ӧ���ܵ�����λ��ʶ
									&AZTECNumOfMultiTab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "1-8");        
        return SC_RESULT_SUCC;
    }
    
}


//����Aztec �Ķ���ͬͼ�Ĵ���ʽTag��AZTMOD
const int AZTECModeOfMultiTab[4] = { 0xFF,0x00,0x08,0x10 };
int AztecModeOfHandleMultiCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_Aztec,
									1,				//Э��{Data}����Сֵ
									3,				//Э��{Data}�����ֵ
									DC_AZTEC_MULTIEN,			//��Ӧ���ܵ�����λ��ʶ
									&AZTECModeOfMultiTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int AztecModeOfHandleMultiCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_Aztec,
									1,				//Э��{Data}����Сֵ
									3,				//Э��{Data}�����ֵ
									DC_AZTEC_MULTIEN,			//��Ӧ���ܵ�����λ��ʶ
									&AZTECModeOfMultiTab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "1-3");        
        return SC_RESULT_SUCC;
    }
}

//����Aztec ��ECI���ʹ�ܡ���ֹ	Tag��AZTECI
const int AZTECECITab[2] = {DC_AZTEC_ECICLOSE,0x0};
int AztecOutputECI_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_Aztec,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_AZTEC_ECICLOSE,			//��Ӧ���ܵ�����λ��ʶ
									&AZTECECITab	//����ָ��˳�����еĹ�������ֵ
									);
}
int AztecOutputECI_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_Aztec,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_AZTEC_ECICLOSE,			//��Ӧ���ܵ�����λ��ʶ
									&AZTECECITab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-1");        
        return SC_RESULT_SUCC;
    }
}

//����Aztec ���ַ����뷽ʽ	Tag��AZTENC
int AztecEncodingOfChar_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int AztecEncodingOfChar_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//����Aztec CODEID�����ʹ�ܡ���ֹ	Tag��CID034
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
//����DM ��Ĭ������	Tag:DMCDEF
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
//����DM ��ʹ�ܡ���ֹ	Tag��DMCENA
int DMEnable_Set(sc_param_st *pParam)
{
	return __2DCodeEnable_Set(pParam, POS_DataMatrix);
}
int DMEnable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_DataMatrix);
}

//����DM ��ʶ����С����	Tag��DMCMIN
int DMMinLen_Set(sc_param_st *pParam)
{
	return __2DCodeMinLen_Set(pParam, POS_DataMatrix);
}
int DMMinLen_Get(sc_param_st *pParam)
{
	return __2DCodeMinLen_Get(pParam, POS_DataMatrix);
}

//����DM ��ʶ����󳤶�	Tag��DMCMAX
int DMMaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_DataMatrix);
}

int DMMaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_DataMatrix);
} 

//����DM �ĵ�˫�빦��	Tag��DMCDOU
const int DMMulCodeTab[3] = { 0x00,0x08,0x10 };
int DMDecoMultiCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_DataMatrix,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_DM_DOUBLE,			//��Ӧ���ܵ�����λ��ʶ
									&DMMulCodeTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int DMDecoMultiCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_DataMatrix,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_DM_DOUBLE,			//��Ӧ���ܵ�����λ��ʶ
									&DMMulCodeTab	//����ָ��˳�����еĹ�������ֵ
									);
    }    
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }
}


//����DM ��ʶ�������빦��	Tag��DMCREC
const int DMRectEnaTab[2] = {DC_DM_DISABLE_RECT,0x0};
int DMDecoRectCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_DataMatrix,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_DM_DISABLE_RECT,			//��Ӧ���ܵ�����λ��ʶ
									&DMRectEnaTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int DMDecoRectCode_Get(sc_param_st *pParam)
{ 
    if(!pParam->nQueryType)
    {
	    return __2DCodeParamValue_Get(pParam,
									POS_DataMatrix,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_DM_DISABLE_RECT,			//��Ӧ���ܵ�����λ��ʶ
									&DMRectEnaTab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-1");        
        return SC_RESULT_SUCC;
    }

 
}

//����DM ������Ƿ������Ϣ���	Tag��DMCADD
const int DMAddInfoTab[2] = { 0x0,DC_DM_ADD_PAD_CODE };
int DMAddInfoWordsToDecoResult_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_DataMatrix,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_DM_ADD_PAD_CODE,			//��Ӧ���ܵ�����λ��ʶ
									&DMAddInfoTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int DMAddInfoWordsToDecoResult_Get(sc_param_st *pParam)
{
	return __2DCodeParamValue_Get(pParam,
									POS_DataMatrix,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_DM_ADD_PAD_CODE,			//��Ӧ���ܵ�����λ��ʶ
									&DMAddInfoTab	//����ָ��˳�����еĹ�������ֵ
									);
} 

//����DM ������������ʶ����ʹ�ܡ���ֹ	Tag��DMCINV
const int DMPhaseTab[3] = { 0x0,0x10000,0x20000 };
int DMPhaseOfCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_DataMatrix,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_DM_Reverse,			//��Ӧ���ܵ�����λ��ʶ
									&DMPhaseTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int DMPhaseOfCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_DataMatrix,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_DM_Reverse,			//��Ӧ���ܵ�����λ��ʶ
									&DMPhaseTab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }

  
}

//����DM ��ECI���ʹ�ܡ���ֹ	Tag��DMCECI
const int DMECITab[2] = {DC_DM_ECICLOSE ,0x0};
int DMOutputECI_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_DataMatrix,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_DM_ECICLOSE,			//��Ӧ���ܵ�����λ��ʶ
									&DMECITab	//����ָ��˳�����еĹ�������ֵ
									);
}
int DMOutputECI_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
        return __2DCodeParamValue_Get(pParam,
									POS_DataMatrix,
									0,				//Э��{Data}����Сֵ
									1,				//Э��{Data}�����ֵ
									DC_DM_ECICLOSE,			//��Ӧ���ܵ�����λ��ʶ
									&DMECITab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-1");        
        return SC_RESULT_SUCC;
    }

  
}

//����DM ���ַ����뷽ʽ	Tag��DMCENC
int DMEncodingOfChar_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int DMEncodingOfChar_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����DM CODEID�����ʹ�ܡ���ֹ	Tag��CID035
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
//����Maxicode ��Ĭ������	Tag:MXCDEF
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
//����Maxicode ��ʹ�ܡ���ֹ	Tag��MXCENA
int MaxiEnable_Set(sc_param_st *pParam)
{
	return __2DCodeEnable_Set(pParam, POS_MaxiCode);
}

int MaxiEnable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_MaxiCode);
}

//����Maxicode ��ʶ����С����	Tag��MXCMIN
int MaxiMinLen_Set(sc_param_st *pParam)
{
	return __2DCodeMinLen_Set(pParam, POS_MaxiCode);
}
int MaxiMinLen_Get(sc_param_st *pParam)
{
	return __2DCodeMinLen_Get(pParam, POS_MaxiCode);
}

//����Maxicode ��ʶ����󳤶�	Tag��MXCMAX
int MaxiMaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_MaxiCode);
}
int MaxiMaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_MaxiCode);
}

//����Maxicode CODEID�����ʹ�ܡ���ֹ	Tag��CID036
int MaxiCodeID_Set(sc_param_st *pParam)
{
	return __2DCodeID_Set(pParam, POS_MaxiCode);
}

int MaxiCodeID_Get(sc_param_st *pParam)
{
	return __2DCodeID_Get(pParam, POS_MaxiCode);
}

/****************************************************************************
*@������ CSC
****************************************************************************/
//����CSC ��Ĭ������	Tag:CSCDEF
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

//����CSC ��ʹ�ܡ���ֹ	Tag��CSCENA
int CSCEnable_Set(sc_param_st *pParam)
{
	return __2DCodeEnable_Set(pParam, POS_HAN_SENSIBLE);
}

int CSCEnable_Get(sc_param_st *pParam)
{
	return __2DCodeEnable_Get(pParam, POS_HAN_SENSIBLE);
}

//����CSC ��ʶ����С����	Tag��CSCMIN
int CSCMinLen_Set(sc_param_st *pParam)
{
	return __2DCodeMinLen_Set(pParam, POS_HAN_SENSIBLE);
}

int CSCMinLen_Get(sc_param_st *pParam)
{
	return __2DCodeMinLen_Get(pParam, POS_HAN_SENSIBLE);
}

//����CSC ��ʶ����󳤶�	Tag��CSCMAX
int CSCMaxLen_Set(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Set(pParam, POS_HAN_SENSIBLE);
}
int CSCMaxLen_Get(sc_param_st *pParam)
{
	return __2DCodeMaxLen_Get(pParam, POS_HAN_SENSIBLE);
}

//����CSC �ĵ�˫�빦��	Tag��CSCDOU
const int CSCMulCodeTab[3] = { 0x00,0x08,0x10};
int CSCDecoMultiCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_HAN_SENSIBLE,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_CSC_DOUBLE,	//��Ӧ���ܵ�����λ��ʶ
									&CSCMulCodeTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int CSCDecoMultiCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
	    return __2DCodeParamValue_Get(pParam,
									POS_HAN_SENSIBLE,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_CSC_DOUBLE,	//��Ӧ���ܵ�����λ��ʶ
									&CSCMulCodeTab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }

  
}

//����CSC ������������ʶ����ʹ�ܡ���ֹ	Tag��CSCINV
const int CSCPhaseTab[3] = { 0x0,0x10000,0x20000 };
int CSCPhaseOfCode_Set(sc_param_st *pParam)
{
	return __2DCodeParamValue_Set(pParam,
									POS_HAN_SENSIBLE,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_DM_Reverse,			//��Ӧ���ܵ�����λ��ʶ
									&CSCPhaseTab	//����ָ��˳�����еĹ�������ֵ
									);
}
int CSCPhaseOfCode_Get(sc_param_st *pParam)
{
    if(!pParam->nQueryType)
    {
	    return __2DCodeParamValue_Get(pParam,
									POS_HAN_SENSIBLE,
									0,				//Э��{Data}����Сֵ
									2,				//Э��{Data}�����ֵ
									DC_DM_Reverse,			//��Ӧ���ܵ�����λ��ʶ
									&CSCPhaseTab	//����ָ��˳�����еĹ�������ֵ
									);
    }
    else
    {

        *(pParam->pOutLen)  = sprintf((char*)(pParam->pOutBuf), "0-2");        
        return SC_RESULT_SUCC;
    }

  
}

//����CSC CODEID�����ʹ�ܡ���ֹ	Tag��CID039
int CSCCodeID_Set(sc_param_st *pParam)
{
	return __2DCodeID_Set(pParam, POS_HAN_SENSIBLE);
}

int CSCCodeID_Get(sc_param_st *pParam)
{
	return __2DCodeID_Get(pParam, POS_HAN_SENSIBLE);
}

/****************************************************************************
*@GM��
****************************************************************************/
//����GM ��Ĭ������	Tag:GMCDEF
int GMLoadDefault_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GMLoadDefault_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//����GM ��ʹ�ܡ���ֹ	Tag��GMCENA
int GMEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GMEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����GM ��ʶ����С����	Tag��GMCMIN
int GMMinLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GMMinLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����GM ��ʶ����󳤶�	Tag��GMCMAX
int GMMaxLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GMMaxLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����GM CODEID�����ʹ�ܡ���ֹ	Tag��CID041
int GMCodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int GMCodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@MicroPDF417��
****************************************************************************/
//����MicroPDF417 ��Ĭ������	Tag:MPDDEF
int MicroPDF417LoadDefault_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����MicroPDF417 ��ʹ�ܡ���ֹ	Tag��MPDENA
int MicroPDF417Enable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroPDF417Enable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����MicroPDF417 ��ʶ����С����	Tag��MPDMIN
int MicroPDF417MinLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroPDF417MinLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����MicroPDF417 ��ʶ����󳤶�	Tag��MPDMAX
int MicroPDF417MaxLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroPDF417MaxLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����MicroPDF417 CODEID�����ʹ�ܡ���ֹ	Tag��CID042
int MicroPDF417CodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroPDF417CodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroPDF417LoadDefault_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }


/****************************************************************************
*@MicroQR��
****************************************************************************/
//����MicroQR ��Ĭ������	Tag:MQRDEF
int MicroQRLoadDefault_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroQRLoadDefault_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����MicroQR ��ʹ�ܡ���ֹ	Tag��MQRENA
int MicroQREnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroQREnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����MicroQR ��ʶ����С����	Tag��MQRMIN
int MicroQRMinLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroQRMinLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����MicroQR ��ʶ����󳤶�	Tag��MQRMAX
int MicroQRMaxLen_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroQRMaxLen_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����MicroQR CODEID�����ʹ�ܡ���ֹ	Tag��CID043
int MicroQRCodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int MicroQRCodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@Specific OCR-B
****************************************************************************/
//����Specific OCR-B ��Ĭ������	Tag:SOBDEF
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
//����Specific OCR-B ��ʹ�ܡ���ֹ	Tag��SOBENA
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

	if (nVal == 0)//��ֹ
	{
		X_paramGetGlobalAllParam()->pOCR->unDecTypes &= (~(0x01 << POS_SPEC_OCR_B));
	}
	else//ʹ��
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

//����Specific OCR-B ��ʶ����С����	Tag��SOBMIN
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
	if (nVal > nTmp)	//��Сֵ > ���ֵ
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

//����Specific OCR-B ��ʶ����󳤶�	Tag��SOBMAX
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
	if (nVal < nTmp)	//��Сֵ > ���ֵ
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


/*Code ID �̶� ��ʱ���ṩ�ӿ�
//����Specific OCR-B CODEID�����ʹ�ܡ���ֹ	Tag��CID043
int SpecOCRBCodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int SpecOCRBCodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
*/

/****************************************************************************
*@Passport OCR
****************************************************************************/
//����Passport OCR ��Ĭ������	Tag:PASDEF
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

//����Passport OCR ��ʹ�ܡ���ֹ	Tag��PASENA
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

	if (nVal == 0)//��ֹ
	{
		X_paramGetGlobalAllParam()->pOCR->unDecTypes &= (~(0x01 << POS_PASSPORT_VISAS));
	}
	else//ʹ��
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


/*Code ID �̶� ��ʱ���ṩ�ӿ�
//����Passport OCR CODEID�����ʹ�ܡ���ֹ	Tag��
int SpecOCRBCodeID_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int SpecOCRBCodeID_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
*/

/****************************************************************************
*@PostalCodeϵ���������ú���
****************************************************************************/
int __PostalCodeParamValue_Set(sc_param_st *pParam,
								unsigned short nCodeType,
								unsigned int nDataMin,			//Э��{Data}����Сֵ
								unsigned int nDataMax,			//Э��{Data}�����ֵ
								unsigned int nSetBitMsk,		//��Ӧ���ܵ�����λ��ʶ
								void		*pSetTal			//����ָ��˳�����еĹ�������ֵ
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
								unsigned int nDataMin,			//Э��{Data}����Сֵ
								unsigned int nDataMax,			//Э��{Data}�����ֵ
								unsigned int nSetBitMsk,		//��Ӧ���ܵ�����λ��ʶ
								void		*pSetTal			//����ָ��˳�����еĹ�������ֵ
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

	if (nVal == 0)//��ֹ
	{
		X_paramGetGlobalAllParam()->pPostal->unDecTypes &= (~(0x01 << nCodeType));
	}
	else//ʹ��
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


//Code ID���á���ȡ
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
//����USPS Postnet ��Ĭ������	Tag:PNTDEF
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

//����USPS Postnet ��ʹ�ܡ���ֹ	Tag��PNTENA
int PostnetEnable_Set(sc_param_st *pParam)
{ 
	return __PostalCodeEnable_Set(pParam, POS_POSTNET);
}

int PostnetEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_POSTNET);
}

//����USPS Postnet У��λ�ķ���ʹ�ܡ���ֹ	Tag��PNTCHK
const int nPostnetTrsmtTab[3] = {0xFF,0x0,BIT_POSTNET_TRMTCHK };
int PostnetChkTrsmt_Set(sc_param_st *pParam)
{
	return __PostalCodeParamValue_Set(pParam,
										POS_POSTNET,
										1,				//Э��{Data}����Сֵ
										2,				//Э��{Data}�����ֵ
										BIT_POSTNET_TRMTCHK,			//��Ӧ���ܵ�����λ��ʶ
										&nPostnetTrsmtTab	//����ָ��˳�����еĹ�������ֵ
										);
}

int PostnetChkTrsmt_Get(sc_param_st *pParam)
{ 
	return __PostalCodeParamValue_Get(pParam,
										POS_POSTNET,
										1,				//Э��{Data}����Сֵ
										2,				//Э��{Data}�����ֵ
										BIT_POSTNET_TRMTCHK,			//��Ӧ���ܵ�����λ��ʶ
										&nPostnetTrsmtTab	//����ָ��˳�����еĹ�������ֵ
										);
}

//����USPS Postnet CODEID�����ʹ�ܡ���ֹ	Tag��CID096
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
//����USPS Intelligent Mail ��Ĭ������	Tag:ILGDEF
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
//����USPS Intelligent Mail ��ʹ�ܡ���ֹ	Tag��ILGENA
int InteMailEnable_Set(sc_param_st *pParam)
{
	return __PostalCodeEnable_Set(pParam, POS_ONECODE);
}

int InteMailEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_ONECODE);
}

//����USPS Intelligent Mail CODEID�����ʹ�ܡ���ֹ	Tag��CID097
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
//����Royal Mail ��Ĭ������	Tag:ROYDEF
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
//����Royal Mail ��ʹ�ܡ���ֹ	Tag��ROYENA
int RoyalMailEnable_Set(sc_param_st *pParam)
{ 
	return __PostalCodeEnable_Set(pParam, POS_RM4SCC);
}

int RoyalMailEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_RM4SCC);
}

//����Royal Mail �����ʹ�ܡ���ֹ	Tag��CID098
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
//����USPS Planet ��Ĭ������	Tag:PLADEF
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

//����USPS Planet ��ʹ�ܡ���ֹ	Tag��PLAENA
int PlanetEnable_Set(sc_param_st *pParam)
{
	return __PostalCodeEnable_Set(pParam, POS_PLANET);
}
int PlanetEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_PLANET);
}

//����USPS Planet У��λ�ķ���ʹ�ܡ���ֹ	Tag��PLACHK
const int nPlanetTrsmtTab[3] = {0xFF,0x00,BIT_PLANET_TRMTCHK };
int PlanetChkTrsmt_Set(sc_param_st *pParam)
{
	return __PostalCodeParamValue_Set(pParam,
										POS_PLANET,
										1,				//Э��{Data}����Сֵ
										2,				//Э��{Data}�����ֵ
										BIT_PLANET_TRMTCHK,			//��Ӧ���ܵ�����λ��ʶ
										&nPlanetTrsmtTab	//����ָ��˳�����еĹ�������ֵ
										);
}

int PlanetChkTrsmt_Get(sc_param_st *pParam)
{
	return __PostalCodeParamValue_Get(pParam,
										POS_PLANET,
										1,				//Э��{Data}����Сֵ
										2,				//Э��{Data}�����ֵ
										BIT_PLANET_TRMTCHK,			//��Ӧ���ܵ�����λ��ʶ
										&nPlanetTrsmtTab	//����ָ��˳�����еĹ�������ֵ
										);
}

//����USPS Planet CODEID�����ʹ�ܡ���ֹ	Tag��CID099
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
//����KIX Post ��Ĭ������	Tag:KIXDEF
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
//����KIX Post ��ʹ�ܡ���ֹ	Tag��KIXENA
int KIXPostEnable_Set(sc_param_st *pParam)
{ 
	return __PostalCodeEnable_Set(pParam,POS_KIXCODE);
}

int KIXPostEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_KIXCODE);
}

//����KIX Post �����ʹ�ܡ���ֹ	Tag��CID100
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
//����Australian Postal ��Ĭ������	Tag:APLDEF
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
//����Australian Postal ��ʹ�ܡ���ֹ	Tag��APLENA
int AUSPostalEnable_Set(sc_param_st *pParam)
{ 
	return __PostalCodeEnable_Set(pParam, POS_AUSPOST);
}
int AUSPostalEnable_Get(sc_param_st *pParam)
{
	return __PostalCodeEnable_Get(pParam, POS_AUSPOST);
}

//����Australian Postal �����ʹ�ܡ���ֹ	Tag��CID101
int AUSPostalCodeID_Set(sc_param_st *pParam)
{ 
	return __PostalCodeID_Set(pParam, POS_AUSPOST);
}

int AUSPostalCodeID_Get(sc_param_st *pParam)
{
	return __PostalCodeID_Get(pParam, POS_AUSPOST);
}

//ǰ��׺�ȸ�����Ϣ�ۺ�����		Tag:APSENA
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

	if (nVal == 0)//��ֹ
	{
		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] &= (~0x01);
	}
	else//ʹ��
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

//ǰ׺˳������					Tag:PRESEQ
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
	if (nVal < 0 || nVal > 6)		//orderֵ <= 6
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

//ǰ׺ʹ������				Tag:CPRENA
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
	if (nVal < 0 || nVal > 1)		//0:��ֹ��1��ʹ��
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

//ǰ׺��Ϣ�༭			   Tag:CPRSET$
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
		pParam->nDigitMaxCnt = Max_nPreSubfix;	//�Զ���ǰ׺���10��
		return SC_RESULT_NEXT;
	}

	if (pParam->nDataLen >10 || pParam->nDataLen < 1)		//�Զ���ǰ׺���10���ַ�
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
      
	nVal = Max_nPreSubfix;		//�Զ���ǰ׺��󳤶�Ϊ10
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
//��ѯ�Զ���ǰ׺֧�ֵ���󳤶�  Tag��CPRQSZ
int PrefixLenth_Get(sc_param_st *pParam)
{
	return SC_RESULT_SUCC;
}

//AIMIDʹ������ AIDENA
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

//CodeID�ۺ�����    Tag:CIDENA
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
	nVal = 2;		//�Զ���ǰ׺��󳤶�Ϊ10
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

//��׺ʹ������  CSUENA
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
	if (nVal < 0 || nVal > 1)		//0:��ֹ��1��ʹ��
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

//��׺��Ϣ�༭  CSUSET
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

	/*if (!pParam->pData)//��׺����Ϊ��
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
        	if (pParam->nDataLen > Max_nPreSubfix*2)// || pParam->nDataLen < 2)		//�Զ���ǰ׺���10���ַ�
        	{
        		return SC_RESULT_FAIL;
        	}
              if( pParam->nDataLen%2!=2 && pParam->nDataLen%2!=0)//���ȱ���ż��
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
	nVal = Max_nPreSubfix;		//�Զ����׺��󳤶�Ϊ10
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

//������ʹ������
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
	if (nVal < 0 || nVal > 1)		//0:��ֹ��1��ʹ��
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

//��������Ϣ�༭  TSUSET
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
	
	nLen = pParam->nDataLen >> 1;				//��pParam->nDataLen >> 1):һ��������(hex)ռ��2��{Data}λ

	if ( nLen > Max_nTerminal || nLen< 1)		//�Զ�����������2���ַ�  
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
       
	nVal = Max_nTerminal;		//�Զ���ǰ׺��󳤶�Ϊ10
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
*@Data Formatting ���ݸ�ʽ�༭
****************************************************************************/
//DataFormatter ��ʹ������		Tag��DFMENA
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

    if (nVal == 0)	//�ر�
    {
        X_paramGetGlobalAllParam()->pDFormat->DFEnable = 0;
    }
    else 
    {
        X_paramGetGlobalAllParam()->pDFormat->DFEnable = 1;

        X_paramGetGlobalAllParam()->pDFormat->outFlag = nVal - 1;
    }

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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


//�ָ�DataFormatter��Ĭ�����ݸ�ʽ 		Tag��DFMDEF
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
    X_paramGetGlobalAllParam()->pDFormat->ErrorTone = 0;//������ʾ��
    X_paramGetGlobalAllParam()->pDFormat->outFlag = 0;//���ݸ�ʽ��ƥ��ʱ�����Ҫ��
    X_paramGetGlobalAllParam()->pDFormat->iSsingle = 0;//����format�ر�
    X_paramGetGlobalAllParam()->pDFormat->usedID = 0;
    memset(X_paramGetGlobalAllParam()->pDFormat->format, 0, sizeof(X_paramGetGlobalAllParam()->pDFormat->format));
            
    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
    {    
        X_paramBakGetGlobalAllParam()->pDFormat->DFEnable = FALSE;//disable
        X_paramBakGetGlobalAllParam()->pDFormat->ErrorTone = 0;//������ʾ��
        X_paramBakGetGlobalAllParam()->pDFormat->outFlag = 0;//���ݸ�ʽ��ƥ��ʱ�����Ҫ��
        X_paramBakGetGlobalAllParam()->pDFormat->iSsingle = 0;//����format�ر�
        X_paramBakGetGlobalAllParam()->pDFormat->usedID = 0;
        memset(X_paramBakGetGlobalAllParam()->pDFormat->format, 0, sizeof(X_paramBakGetGlobalAllParam()->pDFormat->format));
    }
    return nResult;
}
int LoadDefaultDataFormatType_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����DataFormatter�����ݸ�ʽ�༭ 		Tag��DFMSET
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
            dfparam.nDFMsgLen = strlen((char*)(pParam->pData+HEADLEN-1))-2;//-2:У���<ETX>�ĳ���
        } 
        else
        {
            dfparam.pDFCmd = pParam->pData+HEADLEN;
            dfparam.nDFMsgLen = strlen((char*)(pParam->pData+HEADLEN))-2;
        }
    			
        dfparam.nDFSelType = 3;//������ֻ������������λ1��2��3			
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
    /*if (g_DataFormatbuf[0] == 0)//ȡ��ǰ���ȡ��һ�����ݣ��˴�����return false�����򣬱��汨������һ�δ��ڷ����������Ƿ���0x15
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

//���ĳ��DataFormatter���ݸ�ʽ���� 	Tag��DFMCLR
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

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//�������DataFormatter���ݸ�ʽ���� 	Tag��DFMCAL
int ClearAllDataFormatTypes_Set(sc_param_st *pParam)
{
    int  nResult = SC_RESULT_Save;

    memset(X_paramGetGlobalAllParam()->pDFormat->format, 0, sizeof(X_paramGetGlobalAllParam()->pDFormat->format));

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
    {   
        memset(X_paramBakGetGlobalAllParam()->pDFormat->format, 0, sizeof(X_paramBakGetGlobalAllParam()->pDFormat->format));
    }
    return nResult;
}

int ClearAllDataFormatTypes_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����ѡ�е�DataFormatter���ݸ�ʽ 		Tag��DFMUSE
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
    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//����ѡ�еĵ��ζ���DataFormatter���ݸ�ʽ 		Tag��DFMSIN
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

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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

//���ݸ�ʽ��ƥ��Ĵ�����ʾ��		Tag��DFMTON
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
      

    if(!(pParam->bSaveFlag))//������
        nResult = SC_RESULT_SuccUnsave;
    else//����
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
//��ѯ��ǰ�����ݸ�ʽ				Tag��DFMQCU
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
//��ѯ����Ĭ�ϵ����ݸ�ʽ			Tag��DFMQFA
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
//��ѯ����Data Format֧�ֵ���󳤶�	Tag��DFMQSZ
int MaxLenOfDataFormatTypes_Get(sc_param_st *pParam){ return SC_RESULT_SUCC; }

/****************************************************************************
*@���ս�������
****************************************************************************/
//�������ս�����������		Tag��DLPSET
int EnableAnalyzeDrivingLic_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int EnableAnalyzeDrivingLic_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//��ֹ���ս������ܣ���ɾ������		Tag��DLPCLR
int DisableAnalyzeDrivingLic_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//��ѯ���ս�������֧�ֵ�ָ����󳤶�		Tag��DLPQSZ
int MaxLenOfCommandInAnalDrivingLic_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@���ݴ������
****************************************************************************/
//�������ݴ��	Tag��PACKAG
int PackageForDecoInfo_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int PackageForDecoInfo_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@��Ϣ��ȡ����
****************************************************************************/
//��Ϣ��ȡ��ʹ������	Tag��EXTRAC
int CuttingoutDecoInfoEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int CuttingoutDecoInfoEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//�����Ϣ��ȡ����		Tag:EXTADD
int AddConfigForCuttingoutDecoInfo_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int AddConfigForCuttingoutDecoInfo_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//���ĳ������Ľ�ȡ����		Tag:EXTCLR
int ClearCuttingoutForCodeTypes_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ClearCuttingoutForCodeTypes_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//�������һ��Ľ�ȡ����		Tag:EXTCLO
int ClearLatestCFGOfCuttingout_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ClearLatestCFGOfCuttingout_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//������еĽ�ȡ����		Tag:EXTCAL
int ClearAllCFGOfCuttingout_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int ClearAllCFGOfCuttingout_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }


/****************************************************************************
*@������
****************************************************************************/
//�������ʹ������
int BatchProcessEnable_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int BatchProcessEnable_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

/****************************************************************************
*@��ȡ�豸�ֱ���
****************************************************************************/
//�豸�ֱ��ʵĲ�ѯ
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
*@��ȡͼ��
****************************************************************************/
//��ȡͼ��
extern unsigned char Img_GoodDecode[];
extern int NewWidth;			//��¼�ü����ͼ����
extern int NewHeight;			//��¼�ü����ͼ��߶�
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
    nBreak = Z_DogGetBreak(); //��¼ԭ�ȿ��Ź��ĸ�λʱ��
    Z_DogReset(-1);
/*
T�����ͣ�0T - ʵʱͼ�����һ�������ͼ�񣩣�1T - ����ɹ���ͼ��
R, ͼ����ʣ�0R - ԭʼ��С��1R - 1/4ͼ��2R - 1/16ͼ��
F��ͼ���ʽ��0F - ԭʼͼ��Raw data), 1F - BMP��ʽ�� 2F - JPEG��ʽ��3F - TIFF��ʽ
Q��JPEG��ʽ��ͼ��������0Q - Low�� 1Q - Middle�� 2Q - High�� 3Q - Highest
0T0R0F0Q
*/
    /* ��¼ѹ������ */
    if(pParam -> pData[2] < 0x33)	
    {
        gImgReduceRow = pParam -> pData[2]-0x30;		//��¼��ѹ������,0Ϊ����,1Ϊ2��,2Ϊ4��
        gImgReduceColumn = pParam -> pData[2]-0x30;	//��¼��ѹ������			
    }
    else
    {
        gImgReduceRow = 0;			//���ѹ������������Χ��Ĭ��Ϊ��ѹ��
        gImgReduceColumn = 0;
    }
        
    /* ��¼��ȡ��ͼ������ */
    gImgType = (int)(pParam -> pData[4]-0x30);


    if((pParam -> pData[4] == 0x32)&&(pParam -> nDataLen == 8))
    {
        /* ��¼JPGͼ��Ļ�ȡ���� */
        if( pParam -> pData[6] <0x34)
            jpgquelity = pParam -> pData[6]-0x30;
        else
            jpgquelity = 0;	//���ͼ��������Χ����0-3��Ĭ��Ϊ0
    }			
    /* �Ƿ�Ҫ���ͽ���ɹ���ͼ�� */
    if(pParam -> pData[0] == 0x31)
       imgflag = 1;
    	
    p = (unsigned char*)Y_cmosGetDataAddr();

    sW=0;
    sH=0;
    nW = Y_cmosGetWidth();
    nH = Y_cmosGetHeight();

    if(imgflag == 1 )	
    {
        p = Img_GoodDecode;			//Ҫ�Ļ�����ʱͼ��ָ��ָ�����ɹ���ͼ��			
    }
    if(gImgType == 0)
    {//ԭʼͼ
    
        nScale_x = 1<<gImgReduceRow;			//��ѹ��
        nScale_y = 1<<gImgReduceColumn;			//��ѹ��
       /* if ((Y_commGetCurType() == COMMT_USB_Data) && (gImgColor == 0))
        {
            Y_commWrite(p,nW*nH);
            Z_DogReset(nBreak); //�ָ����Ź�
            return TRUE;
        }*/
        //printf("t");
        //����ͼ�����ݴ�С
        nVal = (Z_GetCmosWidth()* Z_GetCmosHeight()>> (pParam -> pData[2]-0x30)) >> (pParam -> pData[2]-0x30);
        String2Hex((unsigned char *)&nVal, 4, chBuf, 16);
        Y_commWrite(chBuf, 8);
        
        IMG_Edit_Send(gImgColor, 0, 0, nW, nH, nScale_x, nScale_y, nW, p);
    }
    if(gImgType >= 1 && gImgType <= 3)
    {
        /* ���ܴ�����ָ�ʽ��ͼ��.ѹ�������Ĵ�����ͬ */
        nScale_x = 1<<gImgReduceRow;		
        nScale_y = 1<<gImgReduceColumn;
        /* ��ѯ�Ƿ�Ҫ���ͽ���ɹ���ͼ�� */
       

        /* ��������bmp��ʽͼ�� */
        if(gImgType == 1)
        {	
            IMG_Edit(IMG_BMP, sW , sH, nW, nH, nScale_x, nScale_y, p);	//ͼ��Ԥ����:����ѹ���������޸�bmp���еߵ�����    
            Img_Send(IMG_BMP, 0, 1);									//��ͼ��ת��bmp��ʽ������ͼ��
        }
        /* ��������tiff��ʽͼ�� */
        else if(gImgType == 3)
        {
            IMG_Edit(IMG_GREY, sW , sH, nW, nH, nScale_x, nScale_y, p); //���Ҫѹ����ҪԤ����Ҷ�ͼ��
            Img_Send(IMG_TIFF, 0, 1);									 //��ͼ��ת��tiff��ʽ������ͼ��
        }
        /* ��������jpg��ʽͼ�� */
        else if(gImgType == 2)
        { 
            IMG_Edit(IMG_GREY, sW , sH, nW, nH, nScale_x, nScale_y, p); //���Ҫѹ����ҪԤ����Ҷ�ͼ��
            Img_Send(IMG_JPG, jpgquelity, 1);							 //��ͼ������ת��jpg��ʽ������ͼ��	
        }
    }
    Z_DogReset(nBreak); //�ָ����Ź�
	
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
*@��Ʒ��Ϣ
****************************************************************************/
//���ò�ƷS/N��		Tag:PSNEDT
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

//ɾ����ƷS/N��		Tag:PSNDEL
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
//���ò�ƷESN��		Tag:ESNSET
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

//ɾ����ƷESN��		Tag:ESNDEL
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
//���ò�Ʒ��������		Tag:DATEDT
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

//ɾ����Ʒ��������		Tag:DATDEL
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
//���ò�Ʒ����		Tag:PDNEDT
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

//ɾ����Ʒ����		Tag:PDNDEL
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
//Is Download ʹ������
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

//ɾ��IS Download����
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

//��������������		Tag:MFNEDT
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

//ɾ������������		Tag:MFNDEL
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
    
//���ò�ƷӲ����Ϣ		Tag:HWVEDT
int SettingInfoOfHardware_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }
int SettingInfoOfHardware_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//ɾ����ƷӲ����Ϣ		Tag:HWVDEL
int DeleteInfoOfHardware_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }

int DeleteInfoOfHardware_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }

int InfoOfHardwareLength_Set(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }

int InfoOfHardwareLength_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }

//���ò�Ʒ��ʵ����		Tag:PRNEDT
int SettingRealNameOfProduct_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int SettingRealNameOfProduct_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
//ɾ����Ʒ��ʵ����		Tag:PRNDEL
int DeleteRealNameOfProduct_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int DeleteRealNameOfProduct_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int RealNameOfProductLength_Set(sc_param_st *pParam)
    { return SC_RESULT_UnSurpport; }
//��ѯ��Ʒ��ʵ����֧�ֵĳ��� Tag:PRNQSZ
int RealNameOfProductLength_Get(sc_param_st *pParam)
{return SC_RESULT_UnSurpport; }
//��ѯ��Ʒ��ʵ����		Tag:PRQRY
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

//CPUID��ѯ
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
            Z_EncryptionWrite(lvp, i, 0);//0��Կ��1����
            break;
        }
        p++;
    }
    //printf("i=%d\n",i);
    Z_EncryptionWrite(lvp+i+1, (pParam->nDataLen)-i-1, 1);//д����
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

    Z_EncryptionRead(buf, 4096, 0);//����Կ

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
    Z_EncryptionRead(lvp, 4096, 1);//������
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
*@��������
****************************************************************************/
//�����С�Զ���־��������			Tag:DOFLVL
int DepthOfFieldInProduct_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int DepthOfFieldInProduct_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
//ɾ���������ò���					Tag��DELDOF
int DelDepthOfFieldInProduct_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int DelDepthOfFieldInProduct_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
/****************************************************************************
*@MCU����Ƶ��		
****************************************************************************/
//����MCU������Ƶ��		Tag��HWRFRQ
//const unsigned short c_nMCUFreqSelTab[2] = {0, eMORERVD_FreqFlag};
int RunningFreqOfMCU_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int RunningFreqOfMCU_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//��������ģʽ		Tag��RUNWEL
int RunningWirelessMode_Set(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }
int RunningWirelessMode_Get(sc_param_st *pParam){ return SC_RESULT_UnSurpport; }

//����USBö��PID--SNʱ�Ƿ�ʹ���豸SN	Tag��PIDUSN
//const unsigned short c_nUSBEnumOnPSNEnTab[2] = {0, eCMMT_NO_SN};
int USBEnumratePIDSNWithSN_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int USBEnumratePIDSNWithSN_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//USB�ӿ��봮�ڵ��л���ʽ����ָ��Ҫ���л�������Ӧ��	Tag��AUTOUR
//const unsigned short c_nUSBAutoAdaptEnTab[2] = {0, eCMMT_AUTOMODE};
int AutoAdaptBetweenSerialCommAndUSB_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
int AutoAdaptBetweenSerialCommAndUSB_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//��ѯ�豸ָ����ܻ����С		Tag��BUFQRY
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


//����Febraban�ַ�����ʱ��ʹ�ܡ���ֹ	Tag��FEBSEN
int FebrabanInterCharDelayEnable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int FebrabanInterCharDelayEnable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//����Febraban�ַ�����ʱʱ��	Tag:FEBSDT
int FebrabanInterCharDelay_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int FebrabanInterCharDelay_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//����Febrabanÿ12�ַ�����ʱ��ʹ�ܡ���ֹ	Tag��FEBMEN
int FebrabanInterAreaDelayEnable_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int FebrabanInterAreaDelayEnable_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

//����Febrabanÿ12�ַ�����ʱʱ��	Tag:FEBMDT
int FebrabanInterAreaDelay_Set(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }

int FebrabanInterAreaDelay_Get(sc_param_st *pParam)
{ return SC_RESULT_UnSurpport; }
