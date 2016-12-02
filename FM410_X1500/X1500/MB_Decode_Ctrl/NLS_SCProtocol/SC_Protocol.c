//#include <string.h>
#include "SC_Debug.h"
#include "SC_Protocol.h"
#include "SC_MapTable.h"

#include "../Y_layer/Y_comm/Y_commMethod.h"
#include "../MB_Decode_Ctrl/NLS_SCProtocol/SC_Debug.h"

#include <string.h>

//extern unsigned int g_commChange;
//判断数据码为设置还是查询
int SC_Data_Parse(sc_param_st *pScParam)
{
    int  nResult = SC_RESULT_SUCC;;
        
    if( !pScParam ) 
    { return SC_RESULT_FAIL; }

    if(pScParam -> pData && pScParam -> nDataLen == 1) 
    {
        if(*pScParam -> pData == '*' )
        {//查询当前
            if(pScParam -> bSaveFlag)
            { 
                pScParam -> nConfType = SC_CFG_SYS; 
            }
            else
            { 
                pScParam -> nConfType = SC_CFG_NSYS; 
            }
            pScParam -> nQueryType = 0;
            nResult = SC_RESULT_QUERY;
        }
        else if(*pScParam -> pData == '&')
        {//查询默认
            pScParam -> nConfType = SC_CFG_FACT;
            pScParam -> nQueryType = 0;
            nResult = SC_RESULT_QUERY;
        }
        else if(*pScParam -> pData == '^')
        {//查询取值范围
            pScParam -> nConfType = SC_CFG_NSYS;
            pScParam -> nQueryType = 1;
            nResult = SC_RESULT_QUERY;
        }
    } 
		
    return nResult;
}


//判断是否允许设置码
int SC_CMDEnable_Check(unsigned int nTag, unsigned int nSubTag)
{

#ifdef SC_DEBUG
        if(!EXTAPI_bEnableSetting_High())
        {//禁止设置码
               if(SC_TAG_INT("FOB") != nTag && 
                  SC_SUBTAG_INT("SET") != nSubTag)
               { return SC_RESULT_FAIL; }
        }

        if(!EXTAPI_bEnableSetting_Low())
        {//关闭设置码
               if((*(unsigned int *)SC_TAG_INT("FOB"))) == nTag && 
                  (*(unsigned int *)SC_TAG_INT("SET")) == nSubTag)
               { return SC_RESULT_SUCC; }

               if((*(unsigned int *)SC_TAG_INT("SET")) == nTag && 
                  (*(unsigned int *)SC_TAG_INT("UPE")) == nSubTag)
               { return SC_RESULT_SUCC; }

               if((*(unsigned int *)SC_TAG_INT("SET")) == nTag && 
                  (*(unsigned int *)SC_TAG_INT("UPN")) == nSubTag)
               { return SC_RESULT_SUCC; }

               if((*(unsigned int *)SC_TAG_INT("FAC")) == nTag && 
                  (*(unsigned int *)SC_TAG_INT("DEF")) == nSubTag)
               { return SC_RESULT_SUCC; }

               return SC_RESULT_FAIL;
        }
#endif
        return SC_RESULT_SUCC;
}


//单条设置码处理
int SC_Single_Deal(unsigned int nTag, unsigned int nSubTag, sc_param_st *pScParam)
{
    unsigned int     nIndex;
    int              nResult;
    sc_subtagmap_st *pScSubTagMap;
    sc_exefunc_t     pScExeFunc;

        //设置码需要判断是否开启
        if(pScParam && pScParam -> bSettingCode)
        {
                if(SC_CMDEnable_Check(nTag, nSubTag) != SC_RESULT_SUCC)
                { return SC_RESULT_FAIL; }
        }
        
	    pScSubTagMap = 0;
	
    //查找Tag
    for(nIndex = 0; ; nIndex ++)
    {
        if((*(unsigned int*)(SC_MainTagMapTab[nIndex].nTagbuf))== nTag)
        { 
            pScSubTagMap = SC_MainTagMapTab[nIndex].pSubTagMap;
            break; 
        }
				
        if((*(unsigned int*)SC_MainTagMapTab[nIndex].nTagbuf) == (*(unsigned int*)"1"))
        { break; }
    }
			
    //没找到
    if( !pScSubTagMap )
    { return SC_RESULT_Unvalid; }

    //printf("nTag Found!\r\n");

    pScExeFunc = 0;
			
			//查找SubTag

    for(nIndex = 0; ; nIndex ++)
    {
        if((*(unsigned int*)pScSubTagMap[nIndex].nSubTagbuf)== nSubTag)
        { 
            nResult = SC_Data_Parse(pScParam);
            switch(nResult)
            {
                case SC_RESULT_QUERY:
                    pScExeFunc = (sc_exefunc_t)pScSubTagMap[nIndex].pQueryFunc; 
                break;
                case SC_RESULT_SUCC:
                    pScExeFunc = (sc_exefunc_t)pScSubTagMap[nIndex].pSetFunc; 
                break;
                default:
                    
                break;
            }
            //printf("nSubTag Found!\r\n");
            break; 
        }
				
        if((*(unsigned int*)pScSubTagMap[nIndex].nSubTagbuf) == (*(unsigned int*)"1"))
        { break; }
    }
			
		//没找到
	if( !pScExeFunc && nSubTag )
	{ return SC_RESULT_Unvalid; }
	else if ( !pScExeFunc )
	{ return SC_RESULT_UnSurpport; }

    //printf("Command Surpport!\r\n");
		
    nResult = pScExeFunc(pScParam);
    switch(nResult)
    {
        case SC_RESULT_NEXT:
            if(!pScParam -> bSettingCode)//通讯指令，需要数据码，却无数据码，返回失败
		{ //通讯指令，不支持
                nResult = SC_RESULT_FAIL;
                break;
             }
            if((*(unsigned int*)SC_TAG_INT("DIG")) != nTag)
            SC_SetMsgParam(pScExeFunc, 
                        pScParam -> nDigitMaxCnt, 
                        pScParam -> bSaveFlag, 
                        SC_NEXT_DIGIT); 
        break;
        case SC_RESULT_NSTR:
            if(!pScParam -> bSettingCode)//通讯指令，需要数据码，却无数据码，返回失败
		{ //通讯指令，不支持
                nResult = SC_RESULT_FAIL;
                break;
             }
              SC_SetMsgParam(pScExeFunc, 
                        pScParam -> nDigitMaxCnt, 
                        pScParam -> bSaveFlag, 
                        SC_NEXT_STRING); 
              break;
        case SC_RESULT_BATCH:
            if(!pScParam -> bSettingCode)//通讯指令，需要数据码，却无数据码，返回失败
		{ //通讯指令，不支持
                nResult = SC_RESULT_FAIL;
                break;
             }
              SC_SetMsgParam(pScExeFunc, 
                        pScParam -> nDigitMaxCnt, 
                        pScParam -> bSaveFlag, 
                        SC_NEXT_BATCH); 
              break;
        default:
              SC_ResetMsgParam(); 
        break;
    }
        
    return nResult;
}

//转换成ACK
unsigned char SC_Result2Ack(int nResult)
{
	    unsigned char ack;
	    
	    switch(nResult)
	    {
		case SC_RESULT_FAIL:
		case SC_RESULT_UnSurpport:
    		  ack = SC_COMM_NAK;
    	      break;
			
		case SC_RESULT_Unvalid:
			  ack = SC_COMM_ENQ;
			  break;
			
		default:
		      ack = SC_COMM_ACK;
		      break;
	    }
			
	    return ack;
}

//查询处理
int SC_Query_Proc(unsigned int nTag, int bTagNeed, unsigned int *pInitLen, sc_param_st *pScParam)
{
    unsigned int     nIndex, nTagIndex, nOffSet;
    unsigned int     nOutTotalLen, nOutLen, *pOutLen;
    int              nResult, nRetStat = SC_RESULT_FAIL;
    sc_subtagmap_st *pScSubTagMap;
    sc_exefunc_t     pScExeFunc;

    if( !pScParam || !pInitLen )
    { return SC_RESULT_FAIL; }
	
    nOutTotalLen = *pInitLen;
    pOutLen      = pScParam -> pOutLen;
	
    //遍历Tag
    for(nTagIndex = 1; ; nTagIndex ++)
    {
        if(nTag != 0x30)
        {
            pScSubTagMap = 0;
				
            //查找Tag
            for(nIndex = 1; ; nIndex ++)
            {
                if((*(unsigned int*)SC_MainTagMapTab[nIndex].nTagbuf)== nTag)
                { 
                    pScSubTagMap = SC_MainTagMapTab[nIndex].pSubTagMap;
                    break; 
                }
								
                if((*(unsigned int*)SC_MainTagMapTab[nIndex].nTagbuf) == (*(unsigned int*)"1"))
                { break; }
            }		
        }
        else
        {
            pScSubTagMap = SC_MainTagMapTab[nTagIndex].pSubTagMap;
			if((*(unsigned int*)SC_MainTagMapTab[nTagIndex].nTagbuf) == (*(unsigned int*)"2"))
			{ break; }
            if((*(unsigned int*)SC_MainTagMapTab[nTagIndex].nTagbuf) == (*(unsigned int*)"1"))
            { break; }
            nIndex = nTagIndex;
        }	

        //没找到
        if( !pScSubTagMap )
        { continue; }						

        if(bTagNeed && pScParam -> nOutBufSize < 6)
        { 
            break; 
        }
        else if(pScParam -> nOutBufSize < 3)
        { 
            break; 
        }

        if( bTagNeed )
        {
            nOffSet = 6;
            //memcpy(pScParam -> pOutBuf, (char *)&SC_MainTagMapTab[nIndex].nMainTagId, 3);
            memcpy(pScParam -> pOutBuf, SC_MainTagMapTab[nIndex].nTagbuf, 3);
        }
        else
        {
            nOffSet = 3;  
        }
				
        pScExeFunc = 0;
				//printf("%s\r\n",SC_MainTagMapTab[nIndex].nTagbuf);
        //遍历SubTag
        for(nIndex = 1; ; nIndex ++)
        {
            if((*(unsigned int*)pScSubTagMap[nIndex].nSubTagbuf) == (*(unsigned int*)"1"))
            { break; }
			if((*(unsigned int*)pScSubTagMap[nIndex].nSubTagbuf) == (*(unsigned int*)"2"))
            { break; }

            if(SC_RESULT_QUERY != SC_Data_Parse(pScParam))
            { continue; }
                    
            pScExeFunc = (sc_exefunc_t)pScSubTagMap[nIndex].pQueryFunc;
            if( !pScExeFunc )
            { continue; }
    	  //printf("%d,%s\r\n",nIndex,pScSubTagMap[nIndex].nSubTagbuf);  				
            nOutLen = 0;
            pScParam -> pOutBuf     += nOffSet;
            pScParam -> nOutBufSize -= nOffSet;
            pScParam -> pOutLen      = &nOutLen;
            nResult = pScExeFunc(pScParam);
            if(nResult != SC_RESULT_SUCC)
            { 
                pScParam -> pOutBuf     -= nOffSet;
                pScParam -> nOutBufSize += nOffSet;
                continue; 
            }
    					
            pScParam -> pOutBuf     += nOutLen;
            pScParam -> nOutBufSize -= nOutLen;
            nOutTotalLen            += nOutLen;
            if(pScParam -> nOutBufSize >= 2)
            {//是查询码
                nOutTotalLen            += nOffSet;
                nOffSet = 3;
                //memcpy(pScParam -> pOutBuf - nOutLen - 3, (char *)&pScSubTagMap[nIndex].nSubTagId, 3);
                memcpy(pScParam -> pOutBuf - nOutLen - 3, pScSubTagMap[nIndex].nSubTagbuf, 3);
                if(!pScParam -> bSettingCode)
                { 
                    *pScParam -> pOutBuf ++ = SC_Result2Ack(nResult); 
                    pScParam -> nOutBufSize --;
                    nOutTotalLen ++;
                }
                *pScParam -> pOutBuf ++ = ',';
                pScParam -> nOutBufSize --;
                nOutTotalLen ++;
            }
            nRetStat = SC_RESULT_SUCC;
            if(pScParam -> nOutBufSize < nOffSet)
            { break; }
        }
				
        if(nOutTotalLen > (*pInitLen) && *(pScParam -> pOutBuf - 1) == ',')
        { *(pScParam -> pOutBuf - 1) = ';'; }
				
        if(nTag != 0x30 || pScParam -> nOutBufSize < nOffSet)
        { break; }

#ifdef SC_FRAGMENT
                pScParam -> pOutBuf     -= nOutTotalLen;
                pScParam -> nOutBufSize += nOutTotalLen;
        #ifndef SC_DEBUG
                if( nOutTotalLen )
                { 
                        //printf("One Tag Query!\r\n");
                        //EXTAPI_SendComm(pScParam -> pOutBuf, nOutTotalLen); 
                        Y_commWrite(pScParam -> pOutBuf, nOutTotalLen); 
						nOutTotalLen = 0;
						*pInitLen     = 0;
                }
        #endif

#endif
    	}			
    	
    	pScParam -> pOutLen = pOutLen;
    	if(pScParam -> pOutLen)
    	{ *pScParam -> pOutLen = nOutTotalLen - *pInitLen; }
			
		return nRetStat;
}

//命令串处理
int SC_Deal(unsigned char *pCodeBuf, 
            unsigned int   nCodeLen, 
            unsigned int   bSettingCode, //   设置码标志 1:代表设置码；0:代表命令。
            unsigned char *pOutBuf, 
            unsigned int   nOutBufSize, 
            unsigned int  *pOutLen,
            int           *pAction)
{
    unsigned int  nTag, nSubTag, nCmdLen, nSubCmdLen;
    unsigned int  nOutTotalLen, nOutLen, nTmpLen, nDataOffset;
    unsigned char *pTag, *pSubTag, i, bSendFlag, bSaveFlag;
    int           nResult = SC_RESULT_FAIL;
	int           nRetStat = SC_RESULT_SUCC;
    sc_param_st   stParam;
    
    if(!pCodeBuf || !pOutBuf || !pAction)
    { return SC_RESULT_FAIL; }

    if(nCodeLen < 1 || nOutBufSize < 1)
    { return SC_RESULT_FAIL; }

	memset((char *)&stParam, 0, sizeof(sc_param_st));

	nOutTotalLen = 0;
	if( !bSettingCode )
	{//通讯口命令
		pOutBuf[0] = SC_FORMAT_STX;
		pOutBuf[1] = SC_FORMAT_SOH;
		nOutTotalLen = 6;
		if(nOutBufSize < 8)
		{ return SC_RESULT_FAIL; }
		//预留1个给ETX
		nOutBufSize -= 1;
	}
        
    //判断存储区域标志
    if(pCodeBuf[0] == '@') //得用另一个变量来保存存储区域，如果是数据码，要把存储区域变量恢复成设置码的存储区域设置???
    { bSaveFlag = 1; }
    else if(pCodeBuf[0] == '#') 
        { bSaveFlag = 0; }
    else 
        { return SC_RESULT_FAIL; }

    memcpy(pOutBuf + nOutTotalLen, pCodeBuf, 1);

    pCodeBuf ++;   
    nCodeLen --;
    bSendFlag = 0; 
    nOutTotalLen += 1;

    while( nCodeLen )
    {
        nTag = nSubTag = 0x30;
        //查找";"
        pTag = memchr(pCodeBuf, ';', nCodeLen);
        if( pTag ) 
        { nCmdLen = pTag - pCodeBuf; }
        else 
        { nCmdLen = nCodeLen; }
				
        //查找","
        pSubTag = memchr(pCodeBuf, ',', nCmdLen);
        if( pSubTag ) 
        { nSubCmdLen = pSubTag - pCodeBuf; }
        else 
        { nSubCmdLen = nCmdLen; }
				
        //提取Tag
        if(nSubCmdLen >= SC_CMD_LEN_TAG)
        { 
            for(i = SC_CMD_LEN_TAG; i > 0; i --)
            { 
                nTag <<= 8; 
                nTag |= pCodeBuf[i - 1]; 
            }
            nTag &= SC_TAG_MSK; 
        }

        //printf("nTag=%x!\r\n", nTag);
		//printf("nTag=%x\r\n", nTag);			
        nTmpLen = nCmdLen;				
        while( nCmdLen )
        {
            //提取SubTag
            nSubTag = 0x30; 
            nOutLen = 0;
            
            if(nSubCmdLen >= SC_CMD_LEN_TOTAL && nTmpLen == nCmdLen)
            {
                for(i = SC_CMD_LEN_SUBTAG; i > 0; i --)
                { 
                    nSubTag <<= 8; 
                    nSubTag |= pCodeBuf[i + 2]; 
                }
                nSubTag &= SC_TAG_MSK;
								
                if(nOutTotalLen + SC_CMD_LEN_TOTAL + 2 > nOutBufSize)// 应答:SC_CMD_LEN_TOTAL + 2=1Storage + 3Tag + 3SubTag + 1ACK + 1; (+ 1ETX)
                { break; }
                nDataOffset = SC_CMD_LEN_TOTAL;
            }
            else if(nSubCmdLen >= SC_CMD_LEN_SUBTAG)//","后tag
            {
                if(nTmpLen != nCmdLen)
                { 
                    for(i = SC_CMD_LEN_SUBTAG; i > 0; i --)
                    { 
                        nSubTag <<= 8; 
                        nSubTag |= pCodeBuf[i - 1]; 
                    }
                    nSubTag &= SC_TAG_MSK;													
                }
								
                if(nOutTotalLen + SC_CMD_LEN_TAG + 2 > nOutBufSize)
                { break; }
                nDataOffset = SC_CMD_LEN_TAG;
            }
            else//??
            {
                nDataOffset   = 0;
            }

            //printf("nSubTag=%x!\r\n", nSubTag);
            //printf("nSubTag=%x\r\n", nSubTag);
            stParam.pData        = pCodeBuf + nDataOffset;//(TAG)SUBTAG之后的数据
            stParam.nDataLen     = nSubCmdLen - nDataOffset;//(TAG)SUBTAG和","或";"之间的数据长度
            stParam.pOutBuf      = pOutBuf + nOutTotalLen + nDataOffset;//+storage+(TAG)SUBTAG
            stParam.nOutBufSize  = nOutBufSize - nOutTotalLen - nDataOffset;//-存储方式-tag(subtag)
            stParam.pOutLen      = &nOutLen;
            stParam.bSettingCode = bSettingCode;//0:命令,1:设置码
            stParam.bSaveFlag    = bSaveFlag;//  @flash #ram
            stParam.bCompleteFlag = 0;

            if(!bSettingCode) { stParam.bCompleteFlag = 1; }//命令

            nResult = SC_Single_Deal(nTag, nSubTag, &stParam);
            do
            {
                if(SC_RESULT_QUERY == nResult)
                {
                    unsigned int nInitLen = nOutTotalLen;
                    stParam.pOutBuf      -= nDataOffset;
                    stParam.nOutBufSize  += nDataOffset;
                    nResult = SC_Query_Proc(nTag, (nTmpLen == nCmdLen), &nInitLen, &stParam); //仅有tag或无tag，subtag
                    //分段发送时用
                    if(nInitLen == 0) 
    				{ nOutTotalLen = 0; }
    				if(nResult == SC_RESULT_SUCC) 
    				{//查询成功 
    					bSendFlag = 1; 
    					break;
    				}
    				else if( !nTag )
    				{//失败，Tag为0	
    					break;
    				}
    				stParam.pOutBuf      += nDataOffset;
    				stParam.nOutBufSize  -= nDataOffset;
    				nOutLen = 0;
                }
                if(SC_RESULT_QUERY  == SC_Data_Parse(&stParam))//有tag和subtag??????此处还要判断是否设置码/查询码?????
                {//查询码
                    memcpy(pOutBuf + nOutTotalLen, pCodeBuf, nDataOffset);
                    if( !bSettingCode )
                    {//通讯指令
                        stParam.pOutBuf[nOutLen]     = SC_Result2Ack(nResult);
                        stParam.pOutBuf[nOutLen + 1] = ',';
                        nOutTotalLen += 2;
                        bSendFlag = 1;
                    }
                    else
                    {
                        stParam.pOutBuf[nOutLen] = ',';
                        nOutTotalLen += 1;
                        if(nResult == SC_RESULT_SUCC)
                        { bSendFlag = 1; }
                    }
        								
                    nOutTotalLen += nDataOffset;
                }
                else
                {//设置码
                    if(nOutTotalLen + nDataOffset + 2 + stParam.nDataLen + nOutLen > nOutBufSize)// 2:ACK+","或";"
                    { break; }
                    memcpy(pOutBuf + nOutTotalLen, pCodeBuf, nDataOffset);//拷贝命令(tag subtag)到输出缓存
                    memcpy(stParam.pOutBuf + nOutLen, stParam.pData, stParam.nDataLen);//拷贝命令后和","或";"之间的数据到输出缓存
                    if( !bSettingCode )
                    {//通讯指令
                        stParam.pOutBuf[stParam.nDataLen + nOutLen]     = SC_Result2Ack(nResult);
                        stParam.pOutBuf[stParam.nDataLen + nOutLen + 1] = ',';
                        nOutTotalLen += 2;
                        bSendFlag = 1;
                        //printf("%s\r\n",pOutBuf);
                    }
                    else//设置码
                    {
                        stParam.pOutBuf[stParam.nDataLen + nOutLen] = ',';
                        nOutTotalLen += 1;
                        if(nOutLen && nResult == SC_RESULT_SUCC)
                        { bSendFlag = 1; }
                    }
                    nOutTotalLen += (nDataOffset + stParam.nDataLen);
                }
            }while(0);
    					
            nOutTotalLen    += nOutLen;//当前tag已处理的长度,包括应答和","或";"
            nCmdLen         -= nSubCmdLen;//当前tag剩下命令的长度
            
			if(nResult == SC_RESULT_FAIL ||
                           nResult == SC_RESULT_UnSurpport ||
                           nResult == SC_RESULT_Unvalid)
                        { nRetStat = nResult; }
#ifndef SC_DEBUG
                        if(nResult == SC_RESULT_IMGGET)
                        {
                                unsigned char nTailLen = 2;

                                if( bSettingCode )
                                { nTailLen = 1; }
                                
                                if(nOutTotalLen > nTailLen)
                                {
                                        //发送Tag+SubTag
                                        Y_commWrite(pOutBuf, nOutTotalLen - nTailLen); 
                                        //发送图像数据
                                        nResult = SC_Single_Deal(nTag, nSubTag, &stParam);
                                        //发送<ACK>
                                        if(nTailLen == 2)
                                        { 
											*pOutBuf = SC_Result2Ack(nResult);
											Y_commWrite(pOutBuf, 1); 
										}
                                        //保留逗号
                                        *pOutBuf = ',';
                                        nOutTotalLen = 1;
                                }
                        }
                        
                        if(nResult == SC_RESULT_Save2Factory)
                        { 
							EXTAPI_ParameterSave(SC_PARA_FACT); 
						}

            if(nResult == SC_RESULT_Save2User)
            { EXTAPI_ParameterSave(SC_PARA_USER); }

            if(nResult == SC_RESULT_Save || nResult == SC_RESULT_SaveAndUpdateAll ||nResult == SC_RESULT_SetComAndSave)
            {*pAction |= SC_ACT_SYSPSV; }

            if(nResult == SC_RESULT_SaveAndUpdateAll ||nResult == SC_RESULT_UnsaveAndUpdateAll ||nResult == SC_RESULT_SetComUnSave)
            { *pAction |= SC_ACT_COMMCHG; }
			if(nResult == SC_RESULT_SYSRST)
			{ *pAction |= SC_ACT_SYSRST; }
#endif

            //移除','
            if( pSubTag )
            {
                pCodeBuf = pSubTag + 1;//下一个subtag
                if( nCmdLen ) 
                { 
                    nCmdLen -= 1; 
                }
            }
    					
            //查找下一个","
            pSubTag = memchr(pCodeBuf, ',', nCmdLen);
            if( pSubTag )
            { nSubCmdLen = pSubTag - pCodeBuf; }
            else
            { nSubCmdLen = nCmdLen; }
    					
            if(nOutTotalLen > 0 && pOutBuf[nOutTotalLen - 1] == ';')
            { pOutBuf[nOutTotalLen - 1] = ','; }
        }
				
        if(nOutTotalLen > 0 && pOutBuf[nOutTotalLen - 1] == ',')
        { pOutBuf[nOutTotalLen - 1] = ';'; }

#ifdef SC_FRAGMENT
        #ifndef SC_DEBUG
              if( nOutTotalLen && bSendFlag )
              { 
				if(pOutBuf[0] != SC_FORMAT_STX || pOutBuf[1] != SC_FORMAT_SOH || nOutTotalLen > 7)
				{
                    //printf("One Tag Send!\r\n");
                    //EXTAPI_SendComm(pOutBuf, nOutTotalLen); 
                    //for(i=0; i<nOutTotalLen; i++)
                    //printf("%x,",pOutBuf[i]);
                    if(Y_commWrite(pOutBuf, nOutTotalLen)==nOutTotalLen)
                    {    
                    /*    if(g_commChange==1)
                        {
                            Z_msDelay(10);
                            g_commChange = 0;
                         }  */                      
                    }
					nOutTotalLen = 0;
				}
              }
        #endif
              
#endif
				
        nCodeLen -= nTmpLen;
			
        //移除';'
        if( pTag )
        {
            pCodeBuf = pTag + 1;
            if( nCodeLen ) 
            { 
                nCodeLen -= 1;
            }
        }
    }
		
    if(pOutLen && bSendFlag) 
    {        
		if( !bSettingCode )
		{//通讯命令
			#ifdef SC_FRAGMENT
				if(nOutTotalLen == 0)
			#else
				if(nOutTotalLen > 7)
			#endif
			{//补上ETX
				pOutBuf[nOutTotalLen ++] = SC_FORMAT_ETX;
				*pOutLen = nOutTotalLen;
			}
		}
		else
		{//设置码
			*pOutLen = nOutTotalLen; 
		}
    }

#ifndef SC_DEBUG
      if(/*bSaveFlag && */((*pAction) & SC_ACT_SYSPSV))
      { 
      	EXTAPI_ParameterSave(SC_PARA_SYS); 
	  }
#endif

    *pAction &= ~SC_ACT_SYSPSV;
	
	if(nRetStat != SC_RESULT_SUCC)
      { nResult = nRetStat; }
    
    return nResult;
}

void EXTAPI_ParameterSave(int saveflash)
{BOOL bRES;
    switch(saveflash)
    {
        case SC_PARA_SYS:
            bRES = X_paramSaveAsSysDefaultParam();
            //printf("%d\r\n",bRES);
            break;
        case SC_PARA_FACT:
			X_paramSaveAsFactoryParam();
            break;
        case SC_PARA_USER:

            break;

    }
}


