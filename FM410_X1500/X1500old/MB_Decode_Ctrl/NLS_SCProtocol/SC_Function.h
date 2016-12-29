#ifndef __SC_FUNCTION_H__
#define __SC_FUNCTION_H__

#ifdef __cplusplus
extern"C" {
#endif
/************************************************************************/
#define  SC_MSGBUF_SIZE            128
#define  SC_DIGIT_SAVE             0xEE

#define  SC_CFG_NSYS               0  //当前运行
#define  SC_CFG_SYS                1  //系统配置
#define  SC_CFG_FACT               2  //出厂配置
#define  SC_CFG_USER               3  //用户配置

#define  SC_PARA_SYS               0  //系统配置
#define  SC_PARA_FACT              1  //出厂配置
#define  SC_PARA_USER              2  //用户配置
#define  SC_PARA_LICS              3 

#define  SC_NEXT_DIGIT             0 
#define  SC_NEXT_STRING            1 
#define  SC_NEXT_BATCH             2
	
typedef struct tagsc_setfunc_st
{
    unsigned int nSetFuncSn;
    void *pSetExecFunc;
}sc_setfunc_st;
	
typedef struct tagsc_param_st
{
    unsigned char *pData;         //数据内容
    unsigned int   nDataLen;      //数据内容长度
    unsigned char *pOutBuf;       //输出缓存
    unsigned int   nOutBufSize;   //输出缓存大小
    unsigned int  *pOutLen;       //输出长度
    unsigned int  nConfType;      //配置类型 0: 当前运行 1: 系统配置 2: 出厂配置 3: 用户配置
    unsigned int  nQueryType;     //查询类型 1: 取值范围查询 0:值查询
    unsigned int  bSettingCode;   //   设置码标志 1:代表设置码；0:代表命令。
    unsigned int  bSaveFlag;      //当前设置是否需要保存的标志
    unsigned int  nDigitMaxCnt;   //设置时最大允许的数据码位数
    unsigned int  bCompleteFlag;  //命令是否完整  1：完整，不管Data是否有内容，都执行保存或者提示出错 0：不完整，提示需要后续数据码等  
}sc_param_st;

typedef int (*sc_exefunc_t)(sc_param_st *pScParam);

typedef struct tagsc_setmsg_st
{
	    unsigned char  chMsgBuf[SC_MSGBUF_SIZE];  
        unsigned int   nMsgCnt;
        unsigned int   nMaxCnt;
        unsigned int   bSaveFlag;
        int            nState;
        sc_exefunc_t   fnSetCB;
}	sc_setmsg_st;

enum sc_result_type
{
    SC_RESULT_NULL     = 0x00,          //
    SC_RESULT_NORMAL,                   //普通条码
    SC_RESULT_NEXT,                     //后续需要数据码
    SC_RESULT_NSTR,                     //后续需要普通条码字符串
    SC_RESULT_FAIL,                     //失败
    SC_RESULT_SUCC,                     //成功
    	    
    SC_RESULT_Unvalid,					//没有找到相应的设置码
    SC_RESULT_UnSurpport,				//暂不支持该设置
	 
    SC_RESULT_Save,						//操作成功，设置信息需保存更新
    SC_RESULT_Save2Factory,				//操作成功，设置信息需保存更新为出厂默认
    SC_RESULT_Save2User,				//操作成功，设置信息需保存更新为用户默认
    SC_RESULT_SuccUnsave,				//操作成功，但不需要存储
    SC_RESULT_SaveAndUpdateAll,			//操作成功，设置信息需保存更新,并更新所有功能状态
    SC_RESULT_UnsaveAndUpdateAll,			//操作成功，设置信息需保存更新,并更新所有功能状态

	    SC_RESULT_SetComAndSave,			//通讯口设置并存储参数
	    SC_RESULT_SetComUnSave,				//通讯口设置但不存储参数
	    SC_RESULT_IMGGET,		            //获取图像
		SC_RESULT_SYSRST,		            //重启系统

    SC_RESULT_QUERY   = 0x00010000,     //查询所有
    SC_RESULT_BATCH,                    //批处理设置
};

extern void SC_SetMsgParam(sc_exefunc_t fnSetCB, 
                           unsigned int nMaxCnt, 
                           unsigned int bSaveFlag, 
                           int nState);
extern void SC_ResetMsgParam(void);
extern int  SC_NormalSet(unsigned char *pCodeBuf, unsigned int nCodeLen);

extern int  SC_Batch_Set(sc_param_st *pScParam);
extern int  SC_Batch_Get(sc_param_st *pScParam);
extern int  SC_NullStrSet(sc_param_st *pScParam);
extern int  SC_NullGet(sc_param_st *pScParam);
extern int  SC_NullSet(sc_param_st *pScParam);
extern int  SC_QueryAll(sc_param_st *pScParam);

//DIGIT
extern int  SC_Digit_Cancel(sc_param_st *pScParam);
extern int  SC_Digit_DelAll(sc_param_st *pScParam);
extern int  SC_Digit_Delete(sc_param_st *pScParam);
extern int  SC_Digit_Save(sc_param_st *pScParam);
extern int  SC_Digit_0(sc_param_st *pScParam);
extern int  SC_Digit_1(sc_param_st *pScParam);
extern int  SC_Digit_2(sc_param_st *pScParam);
extern int  SC_Digit_3(sc_param_st *pScParam);
extern int  SC_Digit_4(sc_param_st *pScParam);
extern int  SC_Digit_5(sc_param_st *pScParam);
extern int  SC_Digit_6(sc_param_st *pScParam);
extern int  SC_Digit_7(sc_param_st *pScParam);
extern int  SC_Digit_8(sc_param_st *pScParam);
extern int  SC_Digit_9(sc_param_st *pScParam);
extern int  SC_Digit_A(sc_param_st *pScParam);
extern int  SC_Digit_B(sc_param_st *pScParam);
extern int  SC_Digit_C(sc_param_st *pScParam);
extern int  SC_Digit_D(sc_param_st *pScParam);
extern int  SC_Digit_E(sc_param_st *pScParam);
extern int  SC_Digit_F(sc_param_st *pScParam);

/************************************************************************/
#ifdef __cplusplus
}
#endif
#endif //__SC_FUNCTION_H__
