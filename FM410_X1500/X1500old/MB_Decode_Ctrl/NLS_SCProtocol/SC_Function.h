#ifndef __SC_FUNCTION_H__
#define __SC_FUNCTION_H__

#ifdef __cplusplus
extern"C" {
#endif
/************************************************************************/
#define  SC_MSGBUF_SIZE            128
#define  SC_DIGIT_SAVE             0xEE

#define  SC_CFG_NSYS               0  //��ǰ����
#define  SC_CFG_SYS                1  //ϵͳ����
#define  SC_CFG_FACT               2  //��������
#define  SC_CFG_USER               3  //�û�����

#define  SC_PARA_SYS               0  //ϵͳ����
#define  SC_PARA_FACT              1  //��������
#define  SC_PARA_USER              2  //�û�����
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
    unsigned char *pData;         //��������
    unsigned int   nDataLen;      //�������ݳ���
    unsigned char *pOutBuf;       //�������
    unsigned int   nOutBufSize;   //��������С
    unsigned int  *pOutLen;       //�������
    unsigned int  nConfType;      //�������� 0: ��ǰ���� 1: ϵͳ���� 2: �������� 3: �û�����
    unsigned int  nQueryType;     //��ѯ���� 1: ȡֵ��Χ��ѯ 0:ֵ��ѯ
    unsigned int  bSettingCode;   //   �������־ 1:���������룻0:�������
    unsigned int  bSaveFlag;      //��ǰ�����Ƿ���Ҫ����ı�־
    unsigned int  nDigitMaxCnt;   //����ʱ��������������λ��
    unsigned int  bCompleteFlag;  //�����Ƿ�����  1������������Data�Ƿ������ݣ���ִ�б��������ʾ���� 0������������ʾ��Ҫ�����������  
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
    SC_RESULT_NORMAL,                   //��ͨ����
    SC_RESULT_NEXT,                     //������Ҫ������
    SC_RESULT_NSTR,                     //������Ҫ��ͨ�����ַ���
    SC_RESULT_FAIL,                     //ʧ��
    SC_RESULT_SUCC,                     //�ɹ�
    	    
    SC_RESULT_Unvalid,					//û���ҵ���Ӧ��������
    SC_RESULT_UnSurpport,				//�ݲ�֧�ָ�����
	 
    SC_RESULT_Save,						//�����ɹ���������Ϣ�豣�����
    SC_RESULT_Save2Factory,				//�����ɹ���������Ϣ�豣�����Ϊ����Ĭ��
    SC_RESULT_Save2User,				//�����ɹ���������Ϣ�豣�����Ϊ�û�Ĭ��
    SC_RESULT_SuccUnsave,				//�����ɹ���������Ҫ�洢
    SC_RESULT_SaveAndUpdateAll,			//�����ɹ���������Ϣ�豣�����,���������й���״̬
    SC_RESULT_UnsaveAndUpdateAll,			//�����ɹ���������Ϣ�豣�����,���������й���״̬

	    SC_RESULT_SetComAndSave,			//ͨѶ�����ò��洢����
	    SC_RESULT_SetComUnSave,				//ͨѶ�����õ����洢����
	    SC_RESULT_IMGGET,		            //��ȡͼ��
		SC_RESULT_SYSRST,		            //����ϵͳ

    SC_RESULT_QUERY   = 0x00010000,     //��ѯ����
    SC_RESULT_BATCH,                    //����������
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
