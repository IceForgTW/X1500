
#include "../../../MB_Decode_Ctrl/Common.h"
#include "../../../MB_Decode_Ctrl/list.h"

//��ȡ���汾��Ϣ
char *  X_decoGetPriVer(void)
{
	return NULL;
}

//���ô������
BOOL X_decoSetProcParamS(UTGeneralParam * psParam)
{
	return FALSE;
}

//�����û�����Ĺ��ܻص�����
 BOOL	X_decoSetUserFunc(UTGeneralParam *psParam)
 {
	 return FALSE;
 }

//��ʼ���������
 void	X_decoInitProcParam(void)
 {
	 ;
 }
//��ȡuIMG�汾��Ϣ(������ϸ��Ϣ)
 int	X_decoGetUIMGVersion(char *pBuff, int nBuffSize)
 {
	 return 0;
 }
//ͼ��������
 BOOL	X_decoImgProcMain(void)
 {
	 return FALSE;
 }

//����ͼ�����Բ���
 void	X_decoSetImgParam(UTImgParam *psParam)
 {
	 ;
 }
//��ȡ������Ϣ��ַ
 BYTE*	X_decoGetDecInfoBuff(void)
 {
	 return NULL;
 }
//��ȡ������Ϣ����
 int	X_decoGetDecInfoLen(void)
 {
	 return 0;
 }
//��ȡ���봦��ṹ����ֵ
 int	X_decoGetResult(void)
 {
	 return 0;
 }
//�ж��Ƿ��Ǻ�����
 BOOL	X_decoIsMacroCode(void)
 {
	 return FALSE;
 }

//��ȡ��������������ʼ���
 int	X_decoGetMacroOrderFrom(void)
 {
	 return 0;
 }
//��ȡ��������������ֹ���
 int	X_decoGetMacroOrderTo(void)
 {
	 return 0;
 }
//��ȡ������Ŀ�����
 int	X_decoGetMacroTotalCnt(void)
 {
	 return 0;
 }
//��ȡ�������־ID�ĳ���
 int	X_decoGetMacroIDLen(void)
 {
	 return 0;
 }
//��ȡ�������־ID��ָ��λ��ֵ
 int	X_decoGetMacroID(int nPos)
 {
	 return 0;
 }
//��ȡ������������Ľṹ��ַ
 TOUTPUTMSG* X_decoGetDecOutMsg(void)
 {
	 return NULL;
 }
//��ȡ�����������������1D��2D��OCR��
 int	X_decoGetBarCode(void)
 {
	 return 0;
 }
//��ȡ����ԭ��������
 int	X_decoGetCodeType(void)
 {
	 return 0;
 }
//��ȡ��������ƵĲ����洢λ��
 int	X_decoGetCodePos(void)
 {
	 return 0;
 }
//��ȡ������������͵ı�־λλ��
 int	X_decoGetBitsPosition(int nInt)
 {
	 return 0;
 }

//����2D���봦�����
 void	X_decoSet2DParam(UTDec2DSetup *psParam)
 {
	 ;
 }
//����1D���봦�����
 void	X_decoSet1DParam(UTDec1DSetup *psParam)
 {
	 ;
 }
//����OCR���봦�����
 void	X_decoSetOCRParam(UTDecOCRSetup *psParam)
 {
	 ;
 }
//////////////////////////////////////////////////////////////////////////
 void X_decoSetImgForSenseMode(UTImgParam * psParam)
 {
	 ;
 }

 BOOL X_decoEjectFromSenseMode(UTImgParam * psParam, int nLevel)
 {
	 return FALSE;
 }

//��ѯ������ʹ�õ�API
 void X_decoQueryAPI(TImgProcAPI *psParam)
 {
	 ;
 }

//��ȡ����� AIM ID
 void X_decoGetAIMID(char* pAIMID)
 {
	 ;
 }
 
 //�����ӿ�
///��������������빦��: 0:�ر������������,  1:���������������
UIMG_DLL_API BOOL X_decoSetCenterDecMode(unsigned char s8CenterMode);

///�����������ʱ,���ʼ����������
///��������ͼ��߶�,��Ȳ���
///0:ʹ��Ĭ������,��������(20%W, 20%H)  (��ʱ fRatio����������)
///1:ʹ���Զ�������, nRatio ���������߷ֱ�ռ��ͼ��ߵı���
UIMG_DLL_API BOOL	X_decoInitCenterArea(int nType, int nRatio);

//�����ӿ�
///��������������빦��: 0:�ر������������,  1:���������������
UIMG_DLL_API BOOL X_decoSetCenterDecMode(unsigned char s8CenterMode)
{}


///�����������ʱ,���ʼ����������
///��������ͼ��߶�,��Ȳ���
///0:ʹ��Ĭ������,��������(20%W, 20%H)  (��ʱ fRatio����������)
///1:ʹ���Զ�������, nRatio ���������߷ֱ�ռ��ͼ��ߵı���
UIMG_DLL_API BOOL	X_decoInitCenterArea(int nType, int nRatio)
{}


unsigned int GXIMG_BMPTOJPEGSelQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight, BYTE bQulity)
{}

unsigned int GXIMG_BMPTOTIFF(BYTE *pSrcIMG, BYTE *pTagImg, int nWidth, int nHeight, BYTE *pTIFFHead, BYTE *pTIFFTail){}

unsigned int GXIMG_BMPTOTIFFEXT(BYTE *pSrcIMG, BYTE *pTagImg, int nWidth, int nHeight){}

///�����Ǻ������͵�RSS(RSS-14, RSS-Limited, RSS-Expanded)
// 	0, 1����RSS 14
// 	2  RSS limited
// 	3  RSS Expanded
UIMG_DLL_API	BOOL	X_decoGetRSSType(int *pType){}


// ��������:
// bMicroQRType = X_decoGetQRType(&nQRType);
// bMicroQRType :TRUE:��ȡ�ɹ�  FALSE:��ȡʧ��
// nQRType: 10: ��MicroQR, ��10:����MicroQR
UIMG_DLL_API	BOOL    X_decoGetQRType(int *pType){}


// 
// #define		USADL_ERRCODE_NOPARSE		0		  ///����Ҫ����
// #define		USADL_ERRCODE_PARAM			-9		///����������
// #define		USADL_ERRCODE_NO93			-10		///��0�ֽڲ���Dec(93)���ߺ���һ���ֽڲ���Hex(00)��Hex(01)
// #define		USADL_ERRCODE_NO94			-11		///��2�ֽڲ���Dec(94)���ߺ���һ���ֽڲ���Hex(00)��Hex(01)
// #define		USADL_ERRCODE_HEADER		-12		///����Headerʧ��
// #define		USADL_ERRCODE_SUB			-13		  ///����Sub Fileʧ��
// #define		USADL_ERRCODE_SEG			-14		  ///����Mandatory��Optionalʧ��
// #define		USADL_ERRCODE_LEN			-15		  ///���������ܳ����ж�ʧ��
// #define		USADL_ERRCODE_COMBIN		-16		 ///��Ͻ�������ʧ��
// 
// 1. ���� 0, ��������Ҫ����
// 2. ���� -12, -13, -14 , ������Ϊ���Ǽ�����
// 3. -15, -16,  ��������ʧ�� (���ܼ������ݲ����ϱ�׼)
// 4. -9, -10, -11, ����������

///�����ʽ:[93][0/1]  [94][0/1]  [1-89]  [90][�ָ���0x00-0xFF]  [91][8���ֽ�����]  [92][����][�����Ϣ]
///pMsg:������Ϣ
///nMsgLen:��Ϣ����
///pOutput:�����Ϣ
///nMaxOutput:��������Ϣ����
///pCmd:����
///nCmdLen:�����
///���ؽ�����ĳ���
///����0:�����ɹ�
///С�ڵ���0:��鿴�������
int		GXInfo_PDF417_USADL_Parse(BYTE *pMsg, int nLen, BYTE *pOutput, int nOutMaxLen, BYTE *pCmd, int nCmdLen){}


//����Postal���봦�����
UIMG_DLL_API void	X_decoSetPostalParam(UTDecPostalSetup *psParam){}

//data Format
int		GXDataFormat_Invoke(DF_Param *pDF)
{
	return 0;
}

int		GXIMG_IsTwoIMGSimilitude(BYTE *pIMG1, BYTE *pIMG2, int nWidth, int nHeight)
{
	return 0;
}

/*
	nType: 0:��ʼ��,�ҶԵ�ǰͼ����  1:�Ƚ��Ƿ���ͬ
*/
int		GXIMG_IsTwoIMGSimilitudeExt(BYTE *pIMG, int nWidth, int nHeight, int nType)
{
	return 0;
}


///pSrcIMG:����ͼ��
///pJPEGIMG:���JPEGͼ��
///nWidth:ͼ����
///nHeight:ͼ��߶�
///bQulity:���JPEGͼ������, 0:���, 1:�Ժ�, 2:����, 3:���
// ����ֵ�����ͼ��ĳ���
//int		GXIMG_BMPTOJPEGSelQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight, BYTE bQulity);

///�̶�������ѹ��
int		GXIMG_BMPTOJPEGFixedQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight)
{
	return 0;
}

