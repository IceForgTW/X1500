//#include <string.h>
#include "SC_Protocol.h"
#include "NLSComm_Proc.h"
#include "../Y_layer/Y_comm/Y_commMethod.h"
#include <string.h>

extern unsigned int g_commChange;
//�����봦��
int NLSComm_ProcSettingCode(unsigned char *pcDecMsg, unsigned int nLen)
{
    unsigned char chOutBuf[NLSCOMM_OUTBUF_SIZE];
    unsigned int  nOutLen = 0;
    int           nResult = SC_RESULT_FAIL;
    int           nAction = 0;

    do {
            //�жϳ���
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
    //��ʾ��
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
		
    //�����ѯ��Ϣ
    if( nOutLen )
    {
        //EXTAPI_SendComm(chOutBuf, nOutLen);
        Y_commWrite(chOutBuf, nOutLen);
    }

		if(nAction & SC_ACT_SYSRST)//ϵͳ��λ
		{
		    //EXTAPI_SystemReset();
        }

        if(nAction & SC_ACT_COMMCHG)
        { 
            //EXTAPI_UpdateParameter(); 
			X_paramSaveAsSysDefaultParam(); //����ΪϵͳĬ��
        }
#else			
    while(nResult);
#endif
    return nResult;
}

//��ͨ���봦��
int  NLSComm_ProcNormalCode(unsigned char *pcDecMsg, unsigned int nLen)
{
        unsigned char chOutBuf[NLSCOMM_OUTBUF_SIZE];
	    unsigned int  nOutLen = 0;
	    int           nResult = SC_RESULT_NORMAL;
        int           nAction = 0;

        nResult = SC_NormalSet(pcDecMsg, nLen);
        do {
                //����������
                if(nResult != SC_RESULT_BATCH)
                { break; }
                
                //�жϳ���
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
	    //��ʾ��
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
		
		//�����ѯ��Ϣ
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
            X_paramSaveAsSysDefaultParam(); //����ΪϵͳĬ��
        }
#else			
		while(nResult);
#endif
        return nResult;
}


//ָ���
int  NLSComm_ProcMain(unsigned char *pCOMMBuf, unsigned int nBufLen)
{
    unsigned char chOutBuf[NLSCOMM_OUTBUF_SIZE];
    unsigned int  nLen, nOutLen = 0;
    int           nResult, nAction = 0;
	
    if(!pCOMMBuf || nBufLen < 1)
    { return 0; }

    //֡ͷ���
    if(pCOMMBuf[0] != '~')
    { return 0; }

    //��鳤��
    if(nBufLen < 2)
    { return -1; }

    //֡ͷ���
    if(pCOMMBuf[1] != NLSCOMM_FORMAT_SOH)
    { return 0; }
		
    //֡β���
    for(nLen = 0; nLen < nBufLen; nLen ++)
    {
        if(pCOMMBuf[nLen] == NLSCOMM_FORMAT_ETX)
        { 
            break; 
        }
    }
	
    //���ݲ�ȫ
    if(nLen >= nBufLen)
    { return -1; }
		
    nLen ++;
		
    //�жϳ���
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
		//�����ѯ��Ϣ
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
