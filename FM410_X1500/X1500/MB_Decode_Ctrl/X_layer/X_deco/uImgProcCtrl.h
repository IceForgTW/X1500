
#ifndef __UIMG_DEVELOP_INTERFACE_IMGPROCMAIN_H__
#define __UIMG_DEVELOP_INTERFACE_IMGPROCMAIN_H__

#include "../../Common.h"

#ifdef __cplusplus
extern"C"{
#endif
	
#define UIMAGE_PRI_VERSION	X_decoGetPriVer()

#if defined(_WIN32_WCE)
	#define UIMG_DLL_API		 _declspec(dllexport) 
#else
	#define UIMG_DLL_API
#endif 


typedef struct zspTIMGPROCAPI
{
	void (*LP_GetBinBitsFrom4Pt)(int p4Point[4][2],unsigned char* pBits,int* nWBits,int* nHBits); //�����ĵ��ö�ֵ����
	void* Reserve[127];
}TImgProcAPI;


//��ȡ���汾��Ϣ
UIMG_DLL_API char *  X_decoGetPriVer(void);

//���ô������
UIMG_DLL_API BOOL	X_decoSetProcParamS(UTGeneralParam * psParam);
//�����û�����Ĺ��ܻص�����
UIMG_DLL_API BOOL	X_decoSetUserFunc(UTGeneralParam *psParam);
//��ʼ���������
UIMG_DLL_API void	X_decoInitProcParam(void);
//��ȡuIMG�汾��Ϣ(������ϸ��Ϣ)
UIMG_DLL_API int	X_decoGetUIMGVersion(char *pBuff, int nBuffSize);
//ͼ��������
UIMG_DLL_API BOOL	X_decoImgProcMain(void);
//����ͼ�����Բ���
UIMG_DLL_API void	X_decoSetImgParam(UTImgParam *psParam);
//��ȡ������Ϣ��ַ
UIMG_DLL_API BYTE*	X_decoGetDecInfoBuff(void);
//��ȡ������Ϣ����
UIMG_DLL_API int	X_decoGetDecInfoLen(void);
//��ȡ���봦��ṹ����ֵ
UIMG_DLL_API int	X_decoGetResult(void);
//�ж��Ƿ��Ǻ�����
UIMG_DLL_API BOOL	X_decoIsMacroCode(void);
//��ȡ��������������ʼ���
UIMG_DLL_API int	X_decoGetMacroOrderFrom(void);
//��ȡ��������������ֹ���
UIMG_DLL_API int	X_decoGetMacroOrderTo(void);
//��ȡ������Ŀ�����
UIMG_DLL_API int	X_decoGetMacroTotalCnt(void);
//��ȡ�������־ID�ĳ���
UIMG_DLL_API int	X_decoGetMacroIDLen(void);
//��ȡ�������־ID��ָ��λ��ֵ
UIMG_DLL_API int	X_decoGetMacroID(int nPos);
//��ȡ������������Ľṹ��ַ
UIMG_DLL_API TOUTPUTMSG* X_decoGetDecOutMsg(void);
//��ȡ�����������������1D��2D��OCR��
UIMG_DLL_API int	X_decoGetBarCode(void);
//��ȡ����ԭ��������
UIMG_DLL_API int	X_decoGetCodeType(void);
//��ȡ��������ƵĲ����洢λ��
UIMG_DLL_API int	X_decoGetCodePos(void);
//��ȡ������������͵ı�־λλ��
UIMG_DLL_API int	X_decoGetBitsPosition(int nInt);
//����2D���봦�����
UIMG_DLL_API void	X_decoSet2DParam(UTDec2DSetup *psParam);
//����1D���봦�����
UIMG_DLL_API void	X_decoSet1DParam(UTDec1DSetup *psParam);
//����OCR���봦�����
UIMG_DLL_API void	X_decoSetOCRParam(UTDecOCRSetup *psParam);
//����Postal���봦�����
UIMG_DLL_API void	X_decoSetPostalParam(UTDecPostalSetup *psParam);
//////////////////////////////////////////////////////////////////////////
UIMG_DLL_API void X_decoSetImgForSenseMode(UTImgParam * psParam);
UIMG_DLL_API BOOL X_decoEjectFromSenseMode(UTImgParam * psParam, int nLevel);
//��ѯ������ʹ�õ�API
UIMG_DLL_API void X_decoQueryAPI(TImgProcAPI *psParam);

//��ȡ����� AIM ID
UIMG_DLL_API void X_decoGetAIMID(char* pAIMID);

//�����ӿ�
///��������������빦��: 0:�ر������������,  1:���������������
UIMG_DLL_API BOOL X_decoSetCenterDecMode(unsigned char s8CenterMode);

///�����������ʱ,���ʼ����������
///��������ͼ��߶�,��Ȳ���
///0:ʹ��Ĭ������,��������(20%W, 20%H)  (��ʱ fRatio����������)
///1:ʹ���Զ�������, nRatio ���������߷ֱ�ռ��ͼ��ߵı���
UIMG_DLL_API BOOL	X_decoInitCenterArea(int nType, int nRatio);
/* ���ý�������� */
UIMG_DLL_API void	X_decoIniDecArea(UIMGAreaParam *pDecArea);

unsigned int GXIMG_BMPTOJPEGSelQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight, BYTE bQulity);
unsigned int GXIMG_BMPTOTIFF(BYTE *pSrcIMG, BYTE *pTagImg, int nWidth, int nHeight, BYTE *pTIFFHead, BYTE *pTIFFTail);
unsigned int GXIMG_BMPTOTIFFEXT(BYTE *pSrcIMG, BYTE *pTagImg, int nWidth, int nHeight);
///�����Ǻ������͵�RSS(RSS-14, RSS-Limited, RSS-Expanded)
// 	0, 1����RSS 14
// 	2  RSS limited
// 	3  RSS Expanded
UIMG_DLL_API	BOOL	X_decoGetRSSType(int *pType);


/*
	�Ƚ�����ͼ������

	����:	pIMG1  ��һ��ͼͼ��ָ��
			pIMG2  �ڶ���ͼͼ��ָ��
			nWidth  ͼ����
			nHeight ͼ��߶�
	����ֵ
			����0: ����
			С��0: ������
*/
int		GXIMG_IsTwoIMGSimilitude(BYTE *pIMG1, BYTE *pIMG2, int nWidth, int nHeight);

/*
	nType: 0:��ʼ��,�ҶԵ�ǰͼ����  1:�Ƚ��Ƿ���ͬ
*/
int		GXIMG_IsTwoIMGSimilitudeExt(BYTE *pIMG, int nWidth, int nHeight, int nType);


///pSrcIMG:����ͼ��
///pJPEGIMG:���JPEGͼ��
///nWidth:ͼ����
///nHeight:ͼ��߶�
///bQulity:���JPEGͼ������, 0:���, 1:�Ժ�, 2:����, 3:���
// ����ֵ�����ͼ��ĳ���
//int		GXIMG_BMPTOJPEGSelQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight, BYTE bQulity);

///�̶�������ѹ��
int		GXIMG_BMPTOJPEGFixedQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight);

// ��������:
// bMicroQRType = X_decoGetQRType(&nQRType);
// bMicroQRType :TRUE:��ȡ�ɹ�  FALSE:��ȡʧ��
// nQRType: 10: ��MicroQR, ��10:����MicroQR
UIMG_DLL_API	BOOL    X_decoGetQRType(int *pType);


//data Format
int		GXDataFormat_Invoke(DF_Param *pDF);
int		GXDataFormat_IsRightCmd(DF_Param *pDF);

// QR �Ĳ�ɫ��Ļ��
//void	X_decoGuidenceProcParam(unsigned int nCode, BYTE *pProcImgAddr);

#ifdef __cplusplus
}
#endif

#endif //__UIMG_DEVELOP_INTERFACE_IMGPROCMAIN_H__
