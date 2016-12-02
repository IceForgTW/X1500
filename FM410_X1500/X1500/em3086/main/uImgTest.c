
#include "../../../MB_Decode_Ctrl/Common.h"
#include "../../../MB_Decode_Ctrl/list.h"

//获取主版本信息
char *  X_decoGetPriVer(void)
{
	return NULL;
}

//设置处理参数
BOOL X_decoSetProcParamS(UTGeneralParam * psParam)
{
	return FALSE;
}

//设置用户定义的功能回调函数
 BOOL	X_decoSetUserFunc(UTGeneralParam *psParam)
 {
	 return FALSE;
 }

//初始化处理参数
 void	X_decoInitProcParam(void)
 {
	 ;
 }
//获取uIMG版本信息(包含详细信息)
 int	X_decoGetUIMGVersion(char *pBuff, int nBuffSize)
 {
	 return 0;
 }
//图像处理并解码
 BOOL	X_decoImgProcMain(void)
 {
	 return FALSE;
 }

//设置图像属性参数
 void	X_decoSetImgParam(UTImgParam *psParam)
 {
	 ;
 }
//获取解码信息地址
 BYTE*	X_decoGetDecInfoBuff(void)
 {
	 return NULL;
 }
//获取解码信息长度
 int	X_decoGetDecInfoLen(void)
 {
	 return 0;
 }
//获取解码处理结构返回值
 int	X_decoGetResult(void)
 {
	 return 0;
 }
//判断是否是宏条码
 BOOL	X_decoIsMacroCode(void)
 {
	 return FALSE;
 }

//获取宏条码所属的起始序号
 int	X_decoGetMacroOrderFrom(void)
 {
	 return 0;
 }
//获取宏条码所属的终止序号
 int	X_decoGetMacroOrderTo(void)
 {
	 return 0;
 }
//获取宏条码的块总数
 int	X_decoGetMacroTotalCnt(void)
 {
	 return 0;
 }
//获取宏条码标志ID的长度
 int	X_decoGetMacroIDLen(void)
 {
	 return 0;
 }
//获取宏条码标志ID的指定位的值
 int	X_decoGetMacroID(int nPos)
 {
	 return 0;
 }
//获取解码输出参数的结构地址
 TOUTPUTMSG* X_decoGetDecOutMsg(void)
 {
	 return NULL;
 }
//获取解码的码制所属大类1D或2D或OCR等
 int	X_decoGetBarCode(void)
 {
	 return 0;
 }
//获取解码原码制类型
 int	X_decoGetCodeType(void)
 {
	 return 0;
 }
//获取解码的码制的参数存储位置
 int	X_decoGetCodePos(void)
 {
	 return 0;
 }
//获取解码的码制类型的标志位位置
 int	X_decoGetBitsPosition(int nInt)
 {
	 return 0;
 }

//设置2D解码处理参数
 void	X_decoSet2DParam(UTDec2DSetup *psParam)
 {
	 ;
 }
//设置1D解码处理参数
 void	X_decoSet1DParam(UTDec1DSetup *psParam)
 {
	 ;
 }
//设置OCR解码处理参数
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

//查询可引出使用的API
 void X_decoQueryAPI(TImgProcAPI *psParam)
 {
	 ;
 }

//获取解码的 AIM ID
 void X_decoGetAIMID(char* pAIMID)
 {
	 ;
 }
 
 //新增接口
///配置中心区域解码功能: 0:关闭中心区域解码,  1:开启中心区域解码
UIMG_DLL_API BOOL X_decoSetCenterDecMode(unsigned char s8CenterMode);

///中心区域解码时,需初始化中心区域
///需先配置图像高度,宽度参数
///0:使用默认配置,中心区域(20%W, 20%H)  (这时 fRatio不参与运算)
///1:使用自定义配置, nRatio 中心区域宽高分别占码图宽高的比例
UIMG_DLL_API BOOL	X_decoInitCenterArea(int nType, int nRatio);

//新增接口
///配置中心区域解码功能: 0:关闭中心区域解码,  1:开启中心区域解码
UIMG_DLL_API BOOL X_decoSetCenterDecMode(unsigned char s8CenterMode)
{}


///中心区域解码时,需初始化中心区域
///需先配置图像高度,宽度参数
///0:使用默认配置,中心区域(20%W, 20%H)  (这时 fRatio不参与运算)
///1:使用自定义配置, nRatio 中心区域宽高分别占码图宽高的比例
UIMG_DLL_API BOOL	X_decoInitCenterArea(int nType, int nRatio)
{}


unsigned int GXIMG_BMPTOJPEGSelQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight, BYTE bQulity)
{}

unsigned int GXIMG_BMPTOTIFF(BYTE *pSrcIMG, BYTE *pTagImg, int nWidth, int nHeight, BYTE *pTIFFHead, BYTE *pTIFFTail){}

unsigned int GXIMG_BMPTOTIFFEXT(BYTE *pSrcIMG, BYTE *pTagImg, int nWidth, int nHeight){}

///返回是何种类型的RSS(RSS-14, RSS-Limited, RSS-Expanded)
// 	0, 1都是RSS 14
// 	2  RSS limited
// 	3  RSS Expanded
UIMG_DLL_API	BOOL	X_decoGetRSSType(int *pType){}


// 调用例子:
// bMicroQRType = X_decoGetQRType(&nQRType);
// bMicroQRType :TRUE:获取成功  FALSE:获取失败
// nQRType: 10: 是MicroQR, 非10:不是MicroQR
UIMG_DLL_API	BOOL    X_decoGetQRType(int *pType){}


// 
// #define		USADL_ERRCODE_NOPARSE		0		  ///不需要解析
// #define		USADL_ERRCODE_PARAM			-9		///参数不符合
// #define		USADL_ERRCODE_NO93			-10		///第0字节不是Dec(93)或者后面一个字节不是Hex(00)或Hex(01)
// #define		USADL_ERRCODE_NO94			-11		///第2字节不是Dec(94)或者后面一个字节不是Hex(00)或Hex(01)
// #define		USADL_ERRCODE_HEADER		-12		///解析Header失败
// #define		USADL_ERRCODE_SUB			-13		  ///解析Sub File失败
// #define		USADL_ERRCODE_SEG			-14		  ///解析Mandatory或Optional失败
// #define		USADL_ERRCODE_LEN			-15		  ///解析过程总长度判断失败
// #define		USADL_ERRCODE_COMBIN		-16		 ///组合解析数据失败
// 
// 1. 返回 0, 表明不需要解析
// 2. 返回 -12, -13, -14 , 可以认为不是驾照码
// 3. -15, -16,  解析过程失败 (可能驾照数据不符合标准)
// 4. -9, -10, -11, 检查输入参数

///命令格式:[93][0/1]  [94][0/1]  [1-89]  [90][分隔符0x00-0xFF]  [91][8个字节日期]  [92][长度][填充信息]
///pMsg:输入信息
///nMsgLen:信息长度
///pOutput:输出信息
///nMaxOutput:最大输出信息长度
///pCmd:命令
///nCmdLen:命令长度
///返回解析后的长度
///大于0:解析成功
///小于等于0:请查看错误代码
int		GXInfo_PDF417_USADL_Parse(BYTE *pMsg, int nLen, BYTE *pOutput, int nOutMaxLen, BYTE *pCmd, int nCmdLen){}


//设置Postal解码处理参数
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
	nType: 0:初始化,且对当前图采样  1:比较是否相同
*/
int		GXIMG_IsTwoIMGSimilitudeExt(BYTE *pIMG, int nWidth, int nHeight, int nType)
{
	return 0;
}


///pSrcIMG:输入图像
///pJPEGIMG:输出JPEG图像
///nWidth:图像宽度
///nHeight:图像高度
///bQulity:输出JPEG图像质量, 0:最差, 1:稍好, 2:更好, 3:最好
// 返回值：输出图像的长度
//int		GXIMG_BMPTOJPEGSelQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight, BYTE bQulity);

///固定质量的压缩
int		GXIMG_BMPTOJPEGFixedQulity(BYTE *pSrcIMG, BYTE *pJPEGIMG, int nWidth, int nHeight)
{
	return 0;
}

