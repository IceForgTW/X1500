
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
	void (*LP_GetBinBitsFrom4Pt)(int p4Point[4][2],unsigned char* pBits,int* nWBits,int* nHBits); //根据四点获得二值点阵
	void* Reserve[127];
}TImgProcAPI;


//获取主版本信息
UIMG_DLL_API char *  X_decoGetPriVer(void);

//设置处理参数
UIMG_DLL_API BOOL	X_decoSetProcParamS(UTGeneralParam * psParam);
//设置用户定义的功能回调函数
UIMG_DLL_API BOOL	X_decoSetUserFunc(UTGeneralParam *psParam);
//初始化处理参数
UIMG_DLL_API void	X_decoInitProcParam(void);
//获取uIMG版本信息(包含详细信息)
UIMG_DLL_API int	X_decoGetUIMGVersion(char *pBuff, int nBuffSize);
//图像处理并解码
UIMG_DLL_API BOOL	X_decoImgProcMain(void);
//设置图像属性参数
UIMG_DLL_API void	X_decoSetImgParam(UTImgParam *psParam);
//获取解码信息地址
UIMG_DLL_API BYTE*	X_decoGetDecInfoBuff(void);
//获取解码信息长度
UIMG_DLL_API int	X_decoGetDecInfoLen(void);
//获取解码处理结构返回值
UIMG_DLL_API int	X_decoGetResult(void);
//判断是否是宏条码
UIMG_DLL_API BOOL	X_decoIsMacroCode(void);
//获取宏条码所属的起始序号
UIMG_DLL_API int	X_decoGetMacroOrderFrom(void);
//获取宏条码所属的终止序号
UIMG_DLL_API int	X_decoGetMacroOrderTo(void);
//获取宏条码的块总数
UIMG_DLL_API int	X_decoGetMacroTotalCnt(void);
//获取宏条码标志ID的长度
UIMG_DLL_API int	X_decoGetMacroIDLen(void);
//获取宏条码标志ID的指定位的值
UIMG_DLL_API int	X_decoGetMacroID(int nPos);
//获取解码输出参数的结构地址
UIMG_DLL_API TOUTPUTMSG* X_decoGetDecOutMsg(void);
//获取解码的码制所属大类1D或2D或OCR等
UIMG_DLL_API int	X_decoGetBarCode(void);
//获取解码原码制类型
UIMG_DLL_API int	X_decoGetCodeType(void);
//获取解码的码制的参数存储位置
UIMG_DLL_API int	X_decoGetCodePos(void);
//获取解码的码制类型的标志位位置
UIMG_DLL_API int	X_decoGetBitsPosition(int nInt);
//设置2D解码处理参数
UIMG_DLL_API void	X_decoSet2DParam(UTDec2DSetup *psParam);
//设置1D解码处理参数
UIMG_DLL_API void	X_decoSet1DParam(UTDec1DSetup *psParam);
//设置OCR解码处理参数
UIMG_DLL_API void	X_decoSetOCRParam(UTDecOCRSetup *psParam);
//设置Postal解码处理参数
UIMG_DLL_API void	X_decoSetPostalParam(UTDecPostalSetup *psParam);
//////////////////////////////////////////////////////////////////////////
UIMG_DLL_API void X_decoSetImgForSenseMode(UTImgParam * psParam);
UIMG_DLL_API BOOL X_decoEjectFromSenseMode(UTImgParam * psParam, int nLevel);
//查询可引出使用的API
UIMG_DLL_API void X_decoQueryAPI(TImgProcAPI *psParam);

//获取解码的 AIM ID
UIMG_DLL_API void X_decoGetAIMID(char* pAIMID);

//新增接口
///配置中心区域解码功能: 0:关闭中心区域解码,  1:开启中心区域解码
UIMG_DLL_API BOOL X_decoSetCenterDecMode(unsigned char s8CenterMode);

///中心区域解码时,需初始化中心区域
///需先配置图像高度,宽度参数
///0:使用默认配置,中心区域(20%W, 20%H)  (这时 fRatio不参与运算)
///1:使用自定义配置, nRatio 中心区域宽高分别占码图宽高的比例
UIMG_DLL_API BOOL	X_decoInitCenterArea(int nType, int nRatio);
/* 设置解码的区域 */
UIMG_DLL_API void	X_decoIniDecArea(UIMGAreaParam *pDecArea);

unsigned int GXIMG_BMPTOJPEGSelQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight, BYTE bQulity);
unsigned int GXIMG_BMPTOTIFF(BYTE *pSrcIMG, BYTE *pTagImg, int nWidth, int nHeight, BYTE *pTIFFHead, BYTE *pTIFFTail);
unsigned int GXIMG_BMPTOTIFFEXT(BYTE *pSrcIMG, BYTE *pTagImg, int nWidth, int nHeight);
///返回是何种类型的RSS(RSS-14, RSS-Limited, RSS-Expanded)
// 	0, 1都是RSS 14
// 	2  RSS limited
// 	3  RSS Expanded
UIMG_DLL_API	BOOL	X_decoGetRSSType(int *pType);


/*
	比较两幅图相似性

	参数:	pIMG1  第一幅图图像指针
			pIMG2  第二幅图图像指针
			nWidth  图像宽度
			nHeight 图像高度
	返回值
			大于0: 相似
			小于0: 不相似
*/
int		GXIMG_IsTwoIMGSimilitude(BYTE *pIMG1, BYTE *pIMG2, int nWidth, int nHeight);

/*
	nType: 0:初始化,且对当前图采样  1:比较是否相同
*/
int		GXIMG_IsTwoIMGSimilitudeExt(BYTE *pIMG, int nWidth, int nHeight, int nType);


///pSrcIMG:输入图像
///pJPEGIMG:输出JPEG图像
///nWidth:图像宽度
///nHeight:图像高度
///bQulity:输出JPEG图像质量, 0:最差, 1:稍好, 2:更好, 3:最好
// 返回值：输出图像的长度
//int		GXIMG_BMPTOJPEGSelQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight, BYTE bQulity);

///固定质量的压缩
int		GXIMG_BMPTOJPEGFixedQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight);

// 调用例子:
// bMicroQRType = X_decoGetQRType(&nQRType);
// bMicroQRType :TRUE:获取成功  FALSE:获取失败
// nQRType: 10: 是MicroQR, 非10:不是MicroQR
UIMG_DLL_API	BOOL    X_decoGetQRType(int *pType);


//data Format
int		GXDataFormat_Invoke(DF_Param *pDF);
int		GXDataFormat_IsRightCmd(DF_Param *pDF);

// QR 的彩色屏幕码
//void	X_decoGuidenceProcParam(unsigned int nCode, BYTE *pProcImgAddr);

#ifdef __cplusplus
}
#endif

#endif //__UIMG_DEVELOP_INTERFACE_IMGPROCMAIN_H__
