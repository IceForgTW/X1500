//#include <string.h>
#include "SC_Function.h"

sc_setmsg_st  SC_stSetMsgParam ;

//����ִ�лص�����
void SC_SetMsgParam(sc_exefunc_t fnSetCB, 
                    unsigned int nMaxCnt, 
                    unsigned int bSaveFlag, 
                    int nState)
{
	    SC_stSetMsgParam.fnSetCB    = fnSetCB;
        SC_stSetMsgParam.nState     = nState;
        SC_stSetMsgParam.bSaveFlag  = bSaveFlag;
        
        SC_stSetMsgParam.nMaxCnt    = SC_MSGBUF_SIZE;
        if(nMaxCnt > 0 && nMaxCnt < SC_MSGBUF_SIZE)
        { SC_stSetMsgParam.nMaxCnt = nMaxCnt; }
}

//����ִ�лص�����
void SC_ResetMsgParam(void)
{
	    SC_stSetMsgParam.fnSetCB    = 0;
        SC_stSetMsgParam.nMsgCnt    = 0;
        SC_stSetMsgParam.bSaveFlag  = 0;
        SC_stSetMsgParam.nState     = SC_NEXT_DIGIT;
        SC_stSetMsgParam.nMaxCnt    = SC_MSGBUF_SIZE;
}

//û�����������޺���������
int SC_CheckSetMode(sc_param_st *pScParam)
{
	    int  nResult = SC_RESULT_FAIL;
	
	    if( !pScParam ) { return SC_RESULT_FAIL; }
		
		if(pScParam -> nDataLen == 0) 
		{//����
				nResult = SC_RESULT_SUCC;
		} 
        else
        {//��ѯ 
                if(pScParam -> pData && pScParam -> nDataLen == 1) 
        		{
        			    if(*pScParam -> pData == '*')
        				{//��ѯ��ǰ
				
                                nResult = SC_RESULT_QUERY;
        				}
        				else if(*pScParam -> pData == '&')
        				{//��ѯĬ��
        					    nResult = SC_RESULT_QUERY;
        				}
        				else if(*pScParam -> pData == '^')
        				{//��ѯȡֵ��Χ
        					    nResult = SC_RESULT_QUERY;
        				}
        		} 
        }
		
		return nResult;
}

//������
int  SC_Digital(sc_param_st *pScParam, unsigned char digit)
{
        int          nResult;
        unsigned int bSaveFlag;
        //static unsigned int bDigSave = 0;

        nResult = SC_CheckSetMode(pScParam);
        if(nResult == SC_RESULT_QUERY) 
        { return SC_RESULT_SUCC; }

        if(nResult != SC_RESULT_SUCC)
        { return SC_RESULT_FAIL; }

        if( !SC_stSetMsgParam.fnSetCB )
		{ return SC_RESULT_FAIL; }

        if(SC_stSetMsgParam.nState != SC_NEXT_DIGIT)
        { return SC_RESULT_FAIL; }
            
        do
        {
		    //�������
            if(SC_DIGIT_SAVE == digit)
            { 
                //bDigSave = 1;
                break; 
            }
    				
            //�ж������Ƿ�Ϸ�
            do
            {
                if(digit >= '0' && digit <= '9')
                { break; }
                if( !1 ) //��֧��A~F
                { return SC_RESULT_FAIL; }
                if(digit >= 'A' && digit <= 'F')
                { break; }
                return SC_RESULT_FAIL;
            } while(0);
    			    			
            if((SC_stSetMsgParam.nMsgCnt > SC_stSetMsgParam.nMaxCnt)||(SC_stSetMsgParam.nMsgCnt == SC_stSetMsgParam.nMaxCnt))
            { 
                SC_stSetMsgParam.nMsgCnt ++;   
                return SC_RESULT_NEXT;
            }
                   
            SC_stSetMsgParam.chMsgBuf[SC_stSetMsgParam.nMsgCnt] = digit;	 
            SC_stSetMsgParam.nMsgCnt ++;
            return SC_RESULT_NEXT;
        } while(0);
  
        pScParam -> pData    = SC_stSetMsgParam.chMsgBuf;
        pScParam -> nDataLen = SC_stSetMsgParam.nMsgCnt;

        bSaveFlag = pScParam -> bSaveFlag;
        pScParam -> bSaveFlag = SC_stSetMsgParam.bSaveFlag;
        nResult = SC_stSetMsgParam.fnSetCB(pScParam); 
        pScParam -> bSaveFlag = bSaveFlag;

        pScParam -> nDataLen = 0;
           
        SC_ResetMsgParam();
       
    	 return nResult;
}

//������
int  SC_Digit_0(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '0');
}

//������
int  SC_Digit_1(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '1');
}

//������
int  SC_Digit_2(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '2');
}

//������
int  SC_Digit_3(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '3');
}

//������
int  SC_Digit_4(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '4');
}

//������
int  SC_Digit_5(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '5');
}

//������
int  SC_Digit_6(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '6');
}

//������
int  SC_Digit_7(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '7');
}

//������
int  SC_Digit_8(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '8');
}

//������
int  SC_Digit_9(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '9');
}

//������
int  SC_Digit_A(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'A');
}

//������
int  SC_Digit_B(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'B');
}

//������
int  SC_Digit_C(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'C');
}

//������
int  SC_Digit_D(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'D');
}

//������
int  SC_Digit_E(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'E');
}

//������
int  SC_Digit_F(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'F');
}

//������
int  SC_Digit_Save(sc_param_st *pScParam)
{
        int  nResult;

        if(pScParam)
		{ pScParam -> bCompleteFlag = 1; }
        
	  nResult = SC_Digital(pScParam, SC_DIGIT_SAVE);
        if(nResult == SC_RESULT_NEXT)
        { nResult = SC_RESULT_FAIL; }

        return nResult;
}

//ɾ����,ȡ��ǰһ�ζ���һλ����
int  SC_Digit_Delete(sc_param_st *pScParam)
{
        int  nResult;

        nResult = SC_CheckSetMode(pScParam);
        if(nResult == SC_RESULT_QUERY) 
        { return SC_RESULT_SUCC; }

        if(nResult != SC_RESULT_SUCC)
        { return SC_RESULT_FAIL; }

        if( !SC_stSetMsgParam.fnSetCB )
		{ return SC_RESULT_FAIL; }

        if(SC_stSetMsgParam.nState != SC_NEXT_DIGIT)
        { return SC_RESULT_FAIL; }
        
        if( !SC_stSetMsgParam.nMsgCnt )
    	{ return SC_RESULT_FAIL; }
    	 
    	SC_stSetMsgParam.nMsgCnt --;
			 
	   return SC_RESULT_NEXT;
}

//ȫ��ɾ��,ȡ��ǰ�����һ������
int  SC_Digit_DelAll(sc_param_st *pScParam)
{
        int  nResult;

        nResult = SC_CheckSetMode(pScParam);
        if(nResult == SC_RESULT_QUERY) 
        { return SC_RESULT_SUCC; }

        if(nResult != SC_RESULT_SUCC)
        { return SC_RESULT_FAIL; }

        if( !SC_stSetMsgParam.fnSetCB )
		{ return SC_RESULT_FAIL; }

        if(SC_stSetMsgParam.nState != SC_NEXT_DIGIT)
        { return SC_RESULT_FAIL; }
       
	    SC_stSetMsgParam.nMsgCnt = 0;
			 
	    return SC_RESULT_NEXT;
}

//ȡ��
int  SC_Digit_Cancel(sc_param_st *pScParam)
{
       int  nResult;
       
	   SC_ResetMsgParam();

       nResult = SC_CheckSetMode(pScParam);
       if(nResult == SC_RESULT_QUERY) 
       { return SC_RESULT_SUCC; }

       if(nResult != SC_RESULT_SUCC)
       { return SC_RESULT_FAIL; }

       if(SC_stSetMsgParam.nState != SC_NEXT_DIGIT)
       { return SC_RESULT_FAIL; }
			 
	   return SC_RESULT_FAIL;
}

//����������
int  SC_Batch_Set(sc_param_st *pScParam)
{
        if( !pScParam ) { return SC_RESULT_FAIL; }

        if( !pScParam -> nDataLen ) 
        { return SC_RESULT_BATCH; }
        
	    return SC_RESULT_FAIL;
}

//����������
int  SC_Batch_Get(sc_param_st *pScParam)
{
	    return SC_RESULT_SUCC;
}

//��ͨ���봦��
int SC_NormalSet(unsigned char *pCodeBuf, unsigned int nCodeLen)
{
        if(SC_stSetMsgParam.nState == SC_NEXT_STRING && 
           SC_stSetMsgParam.fnSetCB )
        { 
                sc_param_st   stParam;
                int           nResult;
                
                stParam.pData    = pCodeBuf;
            	stParam.nDataLen = nCodeLen;

                nResult = SC_stSetMsgParam.fnSetCB(&stParam); 

            	SC_ResetMsgParam();

                return nResult;
        }

        if(SC_stSetMsgParam.nState == SC_NEXT_BATCH)
        {
                SC_ResetMsgParam();
                return SC_RESULT_BATCH;
        }

        SC_ResetMsgParam();
        return SC_RESULT_NORMAL;
}

//�ղ���
int SC_NullStrSet(sc_param_st *pScParam)
{
#ifndef SC_DEBUG
	    return SC_RESULT_UnSurpport;
#else
        if( !pScParam ) { return SC_RESULT_FAIL; }

        if( !pScParam -> nDataLen ) 
        { return SC_RESULT_NSTR; }
        
	    return SC_RESULT_SUCC;
#endif
}

//�ղ���
int SC_NullSet(sc_param_st *pScParam)
{
#ifndef SC_DEBUG
	    return SC_RESULT_UnSurpport;
#else
        if( !pScParam ) { return SC_RESULT_FAIL; }

        if( !pScParam -> nDataLen ) 
        { 
                pScParam -> nDigitMaxCnt = 3;
                return SC_RESULT_NEXT; 
        }
        
	    return SC_RESULT_SUCC;
#endif
}

//�ղ���
int SC_NullGet(sc_param_st *pScParam)
{
#ifndef SC_DEBUG
	    return SC_RESULT_UnSurpport;
#else
        char strData[8] = "1";
        int  nLen;
        
        if( !pScParam ) { return SC_RESULT_FAIL; }

        if(pScParam -> nQueryType)
        {
                memcpy(strData, "1|3-5", strlen("1|3-5"));
        }

        nLen = strlen(strData);
        if(pScParam -> pOutBuf && pScParam -> nOutBufSize > nLen)
        {
                memcpy(pScParam -> pOutBuf, strData, nLen);
                if(pScParam -> pOutLen)
                { *pScParam -> pOutLen = nLen; }
        }
        
	    return SC_RESULT_SUCC;
#endif
}

//��ѯ����
int  SC_QueryAll(sc_param_st *pScParam)
{
	    if( !pScParam ) { return SC_RESULT_FAIL; }
			
        if(pScParam -> pData && pScParam -> nDataLen == 1) 
		{
			    if(*pScParam -> pData == '*')
				{ return SC_RESULT_QUERY; }

                if(*pScParam -> pData == '&')
				{ return SC_RESULT_QUERY; }

                if(*pScParam -> pData == '^')
				{ return SC_RESULT_QUERY; }
		} 
			
	    return SC_RESULT_FAIL;
}

