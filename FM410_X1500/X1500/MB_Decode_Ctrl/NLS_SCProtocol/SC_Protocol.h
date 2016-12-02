#ifndef __SC_PROTOCOL_H__
#define __SC_PROTOCOL_H__

#include "SC_Function.h"

#ifdef __cplusplus
extern"C" {
#endif
/************************************************************************/
#define SC_LIB_VER			"1.00.001"
#define SC_FRAGMENT

//执行操作
#define SC_ACT_FACTPSV        (1 << 0)  //保存到出厂默认
#define SC_ACT_USERPSV        (1 << 1)  //保存到用户默认
#define SC_ACT_SYSPSV         (1 << 2)  //保存系统配置
#define SC_ACT_COMMCHG        (1 << 3)  //切换接口
#define SC_ACT_SYSRST         (1 << 4)  //重启系统

#if 0
typedef struct
{
    union
    {
        unsigned int nSubTagId;
        unsigned char nSubTagbuf[4];
    }subtagid;
    sc_exefunc_t pQueryFunc;
    sc_exefunc_t pSetFunc;
} sc_subtagmap_st;

typedef struct
{
    union
    {
        unsigned int nMainTagId;
        unsigned char nTagbuf[4];
    }tagid;
    void *pSubTagMap;
}sc_maintagmap_st;
#else
typedef struct
{
    unsigned char nSubTagbuf[4];

    sc_exefunc_t pQueryFunc;
    sc_exefunc_t pSetFunc;
} sc_subtagmap_st;

typedef struct
{
    unsigned char nTagbuf[4];
    void *pSubTagMap;
}sc_maintagmap_st;

#endif

enum sc_cmd_len
{
	    SC_CMD_LEN_TOTAL     = 6,
	    SC_CMD_LEN_TAG       = 3,
        SC_CMD_LEN_SUBTAG    = 3
};

enum sc_ack_type
{
	    SC_COMM_ACK          = 0x06,
	    SC_COMM_NAK          = 0x15,
	    SC_COMM_ENQ          = 0x05,
};

enum sc_format_type
{
		SC_FORMAT_SOH  = 0x01,
		SC_FORMAT_STX  = 0x02,
		SC_FORMAT_ETX  = 0x03,
};

int SC_Single_Deal(unsigned int nTag, unsigned int nSubTag, sc_param_st *pScParam);
int SC_Deal(unsigned char *pCodeBuf, 
            unsigned int   nCodeLen, 
            unsigned int   bSettingCode, 
            unsigned char *pOutBuf, 
            unsigned int   nOutBufSize, 
            unsigned int  *pOutLen,
            int           *pAction);
/************************************************************************/
#ifdef __cplusplus
}
#endif

void EXTAPI_ParameterSave(int saveflash);

#endif //__SC_PROTOCOL_H__
