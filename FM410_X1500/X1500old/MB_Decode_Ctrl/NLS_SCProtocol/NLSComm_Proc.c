//#include <string.h>
#include "SC_Protocol.h"
#include "NLSComm_Proc.h"
#include "../Y_layer/Y_comm/Y_commMethod.h"
#include <string.h>

extern unsigned int g_commChange;
//设置码处理
int NLSComm_ProcSettingCode(unsigned char *pcDecMsg, unsigned int nLen)
{
    unsigned char chOutBuf[NLSCOMM_OUTBUF_SIZE];
    unsigned int  nOutLen = 0;
    int           nResult = SC_RESULT_FAIL;
    int           nAction = 0;

    do {
            //判断长度
            if(nLen < 2)
            { break; }
                
            nResult = SC_Deal(pcDecMsg,                //pCodeBuf
                                  nLen,                    //nCodeLen
                                  1,                       //bSettingCode
                                  chOutBuf,                //pOutBuf
                                  NLSCOMM_OUTBUF_SIZE,     //nOutBufSize
                                  &nOutLen,                //nOutLen
                                  &nAction);               //pAction
         } while(0);
	
#ifndef SC_DEBUG
    //提示音
    switch(nResult)
    {
        case SC_RESULT_FAIL:
        case SC_RESULT_Unvalid:
        case SC_RESULT_UnSurpport:
            //EXTAPI_vBeep(NLSCOMM_BEEP_FAIL);
        break;
        case SC_RESULT_NEXT:
        case SC_RESULT_NSTR:
        case SC_RESULT_BATCH:
            //EXTAPI_vBeep(NLSCOMM_BEEP_NEXT);
        break;
        case SC_RESULT_NULL:
        case SC_RESULT_NORMAL:
        break;
        default:
            //EXTAPI_vBeep(NLSCOMM_BEEP_SUCC);
        break;
    }
		
    //输出查询信息
    if( nOutLen )
    {
        //EXTAPI_SendComm(chOutBuf, nOutLen);
        Y_commWrite(chOutBuf, nOutLen);
    }

		if(nAction & SC_ACT_SYSRST)//系统复位
		{
		    //EXTAPI_SystemReset();
        }

        if(nAction & SC_ACT_COMMCHG)
        { 
            //EXTAPI_UpdateParameter(); 
			X_paramSaveAsSysDefaultParam(); //复制为系统默认
        }
#else			
    while(nResult);
#endif
    return nResult;
}

//普通条码处理
int  NLSComm_ProcNormalCode(unsigned char *pcDecMsg, unsigned int nLen)
{
        unsigned char chOutBuf[NLSCOMM_OUTBUF_SIZE];
	    unsigned int  nOutLen = 0;
	    int           nResult = SC_RESULT_NORMAL;
        int           nAction = 0;

        nResult = SC_NormalSet(pcDecMsg, nLen);
        do {
                //批处理设置
                if(nResult != SC_RESULT_BATCH)
                { break; }
                
                //判断长度
        		if(nLen < 2)
        		{ nResult = SC_RESULT_FAIL; break; }
                
        	    nResult = SC_Deal(pcDecMsg,                //pCodeBuf
                                  nLen,                    //nCodeLen
                                  1,                       //bSettingCode
                                  chOutBuf,                //pOutBuf
                                  NLSCOMM_OUTBUF_SIZE,     //nOutBufSize
                                  &nOutLen,                //nOutLen
                                  &nAction);               //pAction
        } while(0);
	
#ifndef SC_DEBUG
	    //提示音
		switch(nResult)
		{
		case SC_RESULT_FAIL:
	    case SC_RESULT_Unvalid:
		case SC_RESULT_UnSurpport:
			    //EXTAPI_vBeep(NLSCOMM_BEEP_FAIL);
			    break;
		case SC_RESULT_NEXT:
        case SC_RESULT_NSTR:
        case SC_RESULT_BATCH:
			    //EXTAPI_vBeep(NLSCOMM_BEEP_NEXT);
		      break;
		case SC_RESULT_NULL:
        case SC_RESULT_NORMAL:
		      break;
	    default:
			    //EXTAPI_vBeep(NLSCOMM_BEEP_SUCC);
			    break;
		}
		
		//输出查询信息
		if( nOutLen )
		{
                //EXTAPI_SendComm(chOutBuf, nOutLen);
                Y_commWrite(chOutBuf, nOutLen);
		}

		if(nAction & SC_ACT_SYSRST)
		{ 
			//EXTAPI_SystemReset(); 
		}

        if(nAction & SC_ACT_COMMCHG)
        { 
            //EXTAPI_UpdateParameter(); 
            X_paramSaveAsSysDefaultParam(); //复制为系统默认
        }
#else			
		while(nResult);
#endif
        return nResult;
}


//指令处理
int  NLSComm_ProcMain(unsigned char *pCOMMBuf, unsigned int nBufLen)
{
    unsigned char chOutBuf[NLSCOMM_OUTBUF_SIZE];
    unsigned int  nLen, nOutLen = 0;
    int           nResult, nAction = 0;
	
    if(!pCOMMBuf || nBufLen < 1)
    { return 0; }

    //帧头检查
    if(pCOMMBuf[0] != '~')
    { return 0; }

    //检查长度
    if(nBufLen < 2)
    { return -1; }

    //帧头检查
    if(pCOMMBuf[1] != NLSCOMM_FORMAT_SOH)
    { return 0; }
		
    //帧尾检查
    for(nLen = 0; nLen < nBufLen; nLen ++)
    {
        if(pCOMMBuf[nLen] == NLSCOMM_FORMAT_ETX)
        { 
            break; 
        }
    }
	
    //数据不全
    if(nLen >= nBufLen)
    { return -1; }
		
    nLen ++;
		
    //判断长度
    if(nLen < 9)
    { return 0; }
		
    memset(chOutBuf, '0', NLSCOMM_OUTBUF_SIZE);
        
		nResult = SC_Deal(pCOMMBuf + 6,            //pCodeBuf
                          nLen - 7,                //nCodeLen
                          0,                       //bSettingCode
                          chOutBuf,				   //pOutBuf
                          NLSCOMM_OUTBUF_SIZE,     //nOutBufSize
                          &nOutLen,                //nOutLen
                          &nAction);               //pAction

    switch(nResult)
    {
        case SC_RESULT_FAIL:
        break;
    }

#ifndef SC_DEBUG
		//输出查询信息
		if( nOutLen )
		{
			    Y_commWrite(chOutBuf, nOutLen);
                 if(g_commChange==1)
                        {
                            Z_msDelay(10);
                            g_commChange = 0;
                         }   
		}

		if(nAction & SC_ACT_SYSRST)
		{ 
			//EXTAPI_SystemReset(); 
		}

        if(nAction & SC_ACT_COMMCHG)
        { 
                X_paramSaveAsSysDefaultParam();
        }
#else		
		while(nResult);
#endif
		
		return nLen;
}
