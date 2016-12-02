//#include <string.h>
#include "SC_Function.h"

sc_setmsg_st  SC_stSetMsgParam ;

//设置执行回调函数
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

//设置执行回调函数
void SC_ResetMsgParam(void)
{
	    SC_stSetMsgParam.fnSetCB    = 0;
        SC_stSetMsgParam.nMsgCnt    = 0;
        SC_stSetMsgParam.bSaveFlag  = 0;
        SC_stSetMsgParam.nState     = SC_NEXT_DIGIT;
        SC_stSetMsgParam.nMaxCnt    = SC_MSGBUF_SIZE;
}

//没有数据码且无后续数据码
int SC_CheckSetMode(sc_param_st *pScParam)
{
	    int  nResult = SC_RESULT_FAIL;
	
	    if( !pScParam ) { return SC_RESULT_FAIL; }
		
		if(pScParam -> nDataLen == 0) 
		{//设置
				nResult = SC_RESULT_SUCC;
		} 
        else
        {//查询 
                if(pScParam -> pData && pScParam -> nDataLen == 1) 
        		{
        			    if(*pScParam -> pData == '*')
        				{//查询当前
				
                                nResult = SC_RESULT_QUERY;
        				}
        				else if(*pScParam -> pData == '&')
        				{//查询默认
        					    nResult = SC_RESULT_QUERY;
        				}
        				else if(*pScParam -> pData == '^')
        				{//查询取值范围
        					    nResult = SC_RESULT_QUERY;
        				}
        		} 
        }
		
		return nResult;
}

//数据码
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
		    //保存操作
            if(SC_DIGIT_SAVE == digit)
            { 
                //bDigSave = 1;
                break; 
            }
    				
            //判断数据是否合法
            do
            {
                if(digit >= '0' && digit <= '9')
                { break; }
                if( !1 ) //不支持A~F
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

//数据码
int  SC_Digit_0(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '0');
}

//数据码
int  SC_Digit_1(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '1');
}

//数据码
int  SC_Digit_2(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '2');
}

//数据码
int  SC_Digit_3(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '3');
}

//数据码
int  SC_Digit_4(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '4');
}

//数据码
int  SC_Digit_5(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '5');
}

//数据码
int  SC_Digit_6(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '6');
}

//数据码
int  SC_Digit_7(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '7');
}

//数据码
int  SC_Digit_8(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '8');
}

//数据码
int  SC_Digit_9(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, '9');
}

//数据码
int  SC_Digit_A(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'A');
}

//数据码
int  SC_Digit_B(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'B');
}

//数据码
int  SC_Digit_C(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'C');
}

//数据码
int  SC_Digit_D(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'D');
}

//数据码
int  SC_Digit_E(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'E');
}

//数据码
int  SC_Digit_F(sc_param_st *pScParam)
{
	    return SC_Digital(pScParam, 'F');
}

//保存码
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

//删除码,取消前一次读的一位数据
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

//全部删除,取消前面读的一串数据
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

//取消
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

//批处理设置
int  SC_Batch_Set(sc_param_st *pScParam)
{
        if( !pScParam ) { return SC_RESULT_FAIL; }

        if( !pScParam -> nDataLen ) 
        { return SC_RESULT_BATCH; }
        
	    return SC_RESULT_FAIL;
}

//批处理设置
int  SC_Batch_Get(sc_param_st *pScParam)
{
	    return SC_RESULT_SUCC;
}

//普通条码处理
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

//空操作
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

//空操作
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

//空操作
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

//查询所有
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

