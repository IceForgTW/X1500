#ifndef __NLSCOMM_PROC_H__
#define __NLSCOMM_PROC_H__

#ifdef __cplusplus
extern"C" {
#endif
/************************************************************************/
#define NLSCOMM_OUTBUF_SIZE    4096

#define NLSCOMM_BEEP_SUCC      0  
#define NLSCOMM_BEEP_FAIL      1
#define NLSCOMM_BEEP_NEXT      2
	
enum nlscomm_format_type
{
	    NLSCOMM_FORMAT_SOH  = 0x01,
	    NLSCOMM_FORMAT_STX  = 0x02,
	    NLSCOMM_FORMAT_ETX  = 0x03,
};

int NLSComm_ProcSettingCode(unsigned char *pcDecMsg, unsigned int nLen);
int  NLSComm_ProcNormalCode(unsigned char *pcDecMsg, unsigned int nLen);
int  NLSComm_ProcMain(unsigned char *pCOMMBuf, unsigned int nBufLen);
/************************************************************************/
#ifdef __cplusplus
}
#endif
#endif //__NLSCOMM_PROC_H__
