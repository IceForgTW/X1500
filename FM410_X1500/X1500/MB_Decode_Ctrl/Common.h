#ifndef __UIMG_DEVELOP_INTERFACE_COMMON_H__
#define __UIMG_DEVELOP_INTERFACE_COMMON_H__

#include <math.h>
//#include <string.h>
//#include <stdio.h>
//#include "X_layer/X_deco/uIMGSrc/1D2006/Define1D.h"
//#include "X_layer/X_deco/uIMGSrc/OCR_BSrc/OCR_Config.h"
/************************************************************************/
/*                                                                      */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////

//#ifndef BYTE
#undef BYTE
	#define BYTE unsigned char
//#endif
#ifndef BOOL
	#define BOOL int
#endif
	
#define ____SQRT(x)  sqrt((double)(x))
#define ____POW(x,y)  pow((double)(x),(int)(y))

#ifndef  max
#define max(x,y) ((x)>(y)?(x):(y))
#endif  //#ifndef  max

#ifndef  min
#define min(x,y) ((x)<(y)?(x):(y))
#endif  //#ifndef  max

#ifndef TRUE
	#define TRUE	1
#endif

#ifndef FALSE
	#define FALSE	0
#endif

#ifndef NULL
	#define NULL	0
#endif

/************************************************************************/
/*                                                                      */
/************************************************************************/
#define IMG_WIDTH_MAX	1280
#define IMG_HEIGHT_MAX	970
#define IMG_SIZE_MAX	(IMG_WIDTH_MAX*IMG_HEIGHT_MAX + 2048)
                 	 

#define LIGHT_MODE1					0				//normal
#define LIGHT_MODE2					1				//always on
#define LIGHT_MODE3					2				//always off
#define LIGHT_MODE4					3				//
#define LIGHT_MODE5					4				//user control
                                	            	
#define FOCUS_MODE1					0				//normal
#define FOCUS_MODE2					1				//always on
#define FOCUS_MODE3					2				//always off
#define FOCUS_MODE4					3				//
#define FOCUS_MODE5					4				//user control
              	                                     	
                   
//各种标准前后缀使能存放位置宏定义              	
#define POS_IsPrefix				0				//自定义前缀使能或禁止设置
#define POS_IsSuffix				1				//自定义后缀使能或禁止设置
#define POS_IsCodeID				2				//CodeID前缀使能或禁止设置
#define POS_IsAIM					3				//AIM前缀使能或禁止设置
#define POS_IsTerminal				4				//结束符后缀使能或禁止设置
#define POS_IsCut					5				//截取使能或禁止设置
            
#define  ESC0						0           	
#define  ESC1						1           	
#define  ESC2						2           	
#define  ESC3						3				//连续控制读码: 设定后TRUE，摁扳机才能读码，直到下一次摁扳机FALSE
#define  ESC7                       7               //用于USB连续传输图像控制

#define  ESC_TEST_READ_MODE			0x1000			//用于测试读码模式

#define  PAPER						0x01        	
#define  CELL						0x02        	
#define  ALL						0x03  


#define  MACRO_DIRECT				0x01			//读取后直接发送，不进行任何处理
#define  MACRO_SEQ_BUFF				0x02			//按照顺序发送，可以多达64k的缓存，超出缓存，系统不进行自动清空，需要读取设置码的取消清空buff
#define  MACRO_LINK_16				0x03			//读取完毕，按照顺序链接后发送，可以链接多达16个或者小于64k信息的符号
          
#define  U_PRO_IS_Serial        0
#define  U_PRO_IS_DataPipe		1 
#define  U_PRO_IS_HID_KBW       2
#define	 U_PRO_IS_BLUE_TOOTH	4
#define  U_PRO_IS_Storage       5
#define	 U_PRO_IS_USBRS232		6
#define	 U_PRO_IS_PS2           7
#define	 U_PRO_IS_HID_POS       8
#define	 U_PRO_IS_SurePOS_TT    9
#define	 U_PRO_IS_SurePOS_HH    10
#define	 U_PRO_IS_CDC		    11


#define NOPARITY            0
#define ODDPARITY           1
#define EVENPARITY          2
#define MARKPARITY          3
#define SPACEPARITY         4

#ifndef ONESTOPBIT
#define ONESTOPBIT          1
#define ONE5STOPBITS        1
#define TWOSTOPBITS         2
#endif

#define CMOS3000_TYPE				0x00000000 	
#define CMOS3020_TYPE				0x00000001 
#define CMOSOV9650_TYPE             0x00000002 
#define CMOS_MT9V022_TYPE           0x00000003   
                             	            	
//硬件产品类型                  	            	
#define  PRODUCTION_DEF				0				//无特殊类型
#define  PRODUCTION_FM102			1				// FM102 产品
#define  PRODUCTION_FM102L			2				// FM102L 产品类型
#define  PRODUCTION_MIRROR			3				// 图象镜像产品类型
#define  PRODUCTION_DOF				4				// 景深类型
#define  PRODUCTION_SCANSTATUE		5				// IBM SurePOS 开机时候的是否使能识读条码

#define  PRODUCTION_DOF_NORMAL		0				// 标准景深类型
#define  PRODUCTION_DOF_NEAR		1				// 近景深类型
#define  PRODUCTION_DOF_MIDDLE		2				// 中景深类型
#define  PRODUCTION_DOF_FAR			3				// 远景深类型

                                       
//通信结构                      	
#ifndef STX                     	
#define STX							0x02			//1Byte，报文起始标志
#endif                          	            	
#define ATTR						0x00			//1Byte，报文属性
	                            	            	
//命令结构                      	            	
#define AL_SET						0x31			//1Byte，参数设置命令
#define AL_SET_ECHO					0x32			//1Byte，参数设置回应
#define AL_SET_Q					0x33			//1Byte，参数查询命令
#define AL_SET_Q_ECHO				0x34			//1Byte，参数查询结果
#define AL_IMG_REQ					0x35			//1Byte，上载图像数据命令
#define AL_GET_MSG					0x36			//解码后的信息上传命令
#define AL_IMG_REQ_ECHO	            0x37	        //1Byte，回应图像数据命令
#define AL_SET_STATE                0x38            //1Byte，设备状态设置

#define AL_SET_DATAFORMAT			0x3a			//1Byte，设置Data format 
                                	            	
#define ETX							0x03        	
#define ACK							0x06        	
#define NAK							0x15        	
#define ESC							0x1B        	
#define DLE							0x10      
  	                        	            	
//数据打包类型                  	            	
#define  FACTORY_PACK				1				//普通打包类型
#define  ECS_BANK_PACK				2				//ECS 打包类型
#define  BLUE2D_PACK				3				//BLUE 2D 打包类型
#define  ECS_PACK_ADDLEN			4				//ECS  STX+DATALENS(4Digits)+DATA+ETX+BCC
                               
#define CAN_DEC_1D_NUM      		32  				//目前可解的一维条码种类数
#define CAN_DEC_2D_NUM       		32  				//目前可解的二维条码种类数
#define CAN_DEC_OCR_NUM      		1  					//目前可解的OCR种类数
#define MAX_1D_NUM          		32  				//一维条码的最多种类
#define MAX_2D_NUM          		32 	 				//二维条码的最多种类
#define NONCODELIST_NUM     		10  				//无法设置识别长度范围的种类数
#define QUERY_1DPARAM_NUM    		9  					//可供查询参数的一维条码种类数
                 	            	
//非标准前后缀使能设置宏定义    	            	
#define POS_IsBlue2D_LenSend    	0				//Blue2D信息长度发送设置
#define POS_IsBlue2D_TypeSend   	1				//Blue2D条码类型发送设置
#define POS_IsBlue2D_Sensitive  	2				//Blue2D的灵敏度使能设置

#define CharacterEncoding_PDF417		0
#define CharacterEncoding_QR			1
#define CharacterEncoding_Aztec			2
#define CharacterEncoding_DataMatrix	3
#define CharacterEncoding_Code128		4
#define CharacterEncoding_Code39		5
#define CharacterEncoding_Code93		6                               	
/************************************************************************/
/*                                                                      */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
// 解码返回值

#define   UIMG_DECODER_FAIL        -1  // 解码失败

#define   UIMG_DECODER_SUC			0	// 普通条码返回
#define   UIMG_DECODER_MACRO		1	// 具体察看宏返回值
#define   UIMG_DECODER_SETCODE		2	// 具体察看设置码返回值

#define   UIMG_DECODER_SPEC_LIMIT	10  // 受到特殊参数设定限制
#define   UIMG_DECODER_LENS_LIMIT	11  // 受到长度参数限制
#define   UIMG_DECODER_EN_LIMIT		12  // 受到使能参数限制

// 解码返回值
//////////////////////////////////////////////////////////////////////////
//设置码处理返回值
#define U_uIMG_STATUS_SETCODE_START     0x000100   //设置开始
#define U_uIMG_STATUS_SETCODE_END       0x000101   //设置结束
#define U_uIMG_STATUS_SETCODE_ERROR     0x000102   //设置错误
#define U_uIMG_STATUS_SETCODE_SUC       0x000103   //设置正确
#define U_uIMG_STATUS_SETCODE_UNKNOW    0x000104   //设置状态未知



#define MACRO_START					"|+1.8|+5.8|-6.8|+7.8|1.8|"//开始读取宏
#define MACRO_OK					"|+1.8|-2.8|+7.8|"//宏读取进行中
#define MACRO_END					"|+1.8|-6.8|+2.8|-6.8|+6.8|+6.8|+6.8|"//宏读取结束
#define MACRO_ERROR					"|+7.8|+4.8|+1.8|6.8|1.8|-7.8|-1.8|+1.8|+5.8|" //读取宏条码错误，重复或者缓存溢出
                                	
#define SETCODE_START				"|-4.3|1.8|2.8|5.8|+1.8|+5.8|"//进入设置码状态
#define SETCODE_OK					"|+4.8|+6.8|-6.8|+5.8|"//正在设置码进行当中
#define SETCODE_END					"|+2.4|+6.8|+6.8|6.3|-6.5|+6.2|+6.8|+5.8|"//设置码处理完毕或者一次设置成功结束
#define SETCODE_ERROR				"|+7.8|-6.8|+2.8|+7.8|-6.8|+2.8|1.4|+2.6|+7.8|+3.8|"//设置码错误
                 
/************************************************************************/
/*                                                                      */
/************************************************************************/

#define Command_Num_MAX		516
#define Command_Num			512
#define  HEADLEN 12

// data formatting 
typedef struct tagFormat
{
	int FormatID;
	int FormatType;
// 	char TerID[3+1];
// 	char CodeID[2+1];
// 	char ComLen[4+1];
	int		TerID;
	int		SymbolID;
	int		MsgLen;
	char Cmd[Command_Num_MAX]; 
}Format;
typedef struct tagDataFormat
{
	int crc;
	BOOL DFEnable;
	int usedID;//  使用的Format id（默认值：0）
	int outFlag; //输出标志，Format执行出错时是原样输出还是报错，要不要添加前后缀等。（默认：报错、要加前后缀）
	int singleFormatID;// 
	int iSsingle;//是否有单次执行的情况
	int ErrorTone;// 报警声开关
	Format format[4];
}DataFormat;



// typedef struct tagDFormat
// {
// 	Format format[4];
// }DFormat;

typedef int (*DF_ProcMain)(unsigned char *pCmd, int nCmdLen, unsigned char *pOriMsg, int nOriLen, unsigned char *pOutput);


///Data Format 参数
typedef	struct	__DF_Param__
{
	int				nCodeBigType;				///条码类型, 1D, 2D
	int				nCodeType;					///条码类型, 例如:Code128, PDF417
	
	int				nDecMsgLen;					///解码信息长度
	unsigned char	*pDecMsg;					///解码信息
	
	unsigned char	*pDFCmd;					///Data Format命令,如果同时有按键,延时指令;按键指令在前,延时指令在后
	int				nDFMsgLen;					///Data Format命令长度
	
	///特别注意:这里的输出信息是1个字节转换成2个16进制字符表示
	///(方便处理按键,延时)
	unsigned char   *pDFOutputMsg;				///注意字符串已经经过了转换
	int				nDFOutputLen;				///经过DF、按键、延时等处理后的信息长度
	int				nMaxOutLen;					///缓存空间大小
	
	int				nDFSelType;					///选择处理方法,请看 Data Formatter方法宏定义
	
	int				nRequired;					/// 1: required, 0: no required
	
	DF_ProcMain     func_DataFormat;			///调用Data Format函数
	
	int				nConvertToHexChar;			///每个字节转换成两个16进制字符的标识, 0:没有转换, 1:已经转换
}DF_Param;
///////////////////////////////////


typedef struct tagUTIMGPARAM
{
	BYTE* pSrc;
	BYTE* pTag;
	int   nWidth;
	int   nHeight;
}UTImgParam;

//条码参数整体设置
typedef struct tagCODEPARAM{
	unsigned int  nPrefix;		 //	自定义前缀信息长度
	unsigned char byPrefix[11];  // 自定义前缀信息
	unsigned int  nSuffix;		 //	自定义后缀信息长度
	unsigned char bySuffix[11];  // 自定义后缀信息
	unsigned char by1DCodeID[32]; // 自定义1D的CodeID信息(第一位)
	unsigned char by2DCodeID[32]; // 自定义2D 的CodeID信息
	unsigned int  nTerminal;     //	自定义结束符信息长度
	unsigned char byTerminal[3]; // 自定义结束符信息
	unsigned char IsPreSufOn[8]; // 所有前后缀打开与屏蔽设置
	unsigned char PrefixOrder;  //CodeID(C)、AIM(A)和自定义(S)前缀的顺序
	                            //0:C+A+S   1:C+S+A    2:A+C+S
	                            //3:A+S+C   4:S+C+A    6:S+A+C
	unsigned int  IsOtherPreSufOn; // 除了标准前后缀外的其它用户前后缀设置，
	                               //第0位:Blue2D 的信息长度发送设置
	                               //第1位:Blue2D 的条码类型发送设置
	unsigned char byCut[20 * 3]; //截取设置
	unsigned int  nMedium;		 // 读码介质
	BOOL		  Protect;		 //	图像是否经过虑波
	unsigned int  nSensitive;    // 灯光灵敏度级别  add by Tiercel 2004.07.06
	BOOL		  bIR;           // 当前是否往端口发送外层解码后的数据 add by Tiercel 2004.07.06
								 // IR 读码状态下

	unsigned int  nOnceDelay;	 //	一次读码延迟时间
	unsigned int  nInfoDelay;    // ESC +2,相同信息延迟时间 
	BOOL		  IsCompDelay;   // 完全延迟(相同信息延时，TRUE--完全等待延时，FALSE-中间出现一次无法识别，延时取消
	unsigned int  nSuccDelay;    // 解码成功延迟
	int			  nReadMode;	//读码控制模式，ESC0,ESC1,ESC2

	unsigned int nScanTimes;	//限定读码次数，0表示不限定

	BOOL		  SetCode;       // 设置码发送参数	

	unsigned int  nPack;         // 打包类型 add by Tiercel 2004.07.06
	unsigned int  nUserDef;      // 控制类型，用户定义和设置
	BOOL		  IsGotoConsole; // 电源开启时是否进入控制台设置
	BOOL		  IsPowerOnDisp; // 电源开启时是否显示系统信息设置
	BOOL		  bErrMsgEn;     //使能
	unsigned int  nErrMsg;       //没读到码后返回的信息长度
	unsigned char byErrMsg[11];  //在一定时间内没读到码是返回的信息

	unsigned int  decodeArea;	//读码区域模式, 低4位值为1表示开启中心区域解码，0表示关闭；bit[12:4]表示中心区域占宽度和高度的百分比例，取值在0 - 100

	BOOL		  IsSettingBarcodeOff;    //Reserved 
}CodeParam;

typedef struct tagCODEPARAM2{
	unsigned int	ID;		 //	版本id=0xCCCCCXXX
	unsigned int	nsize;				//结构体字节数
	unsigned char	by1DCodeID_2[32]; // 自定义1D的CodeID信息(第二位)
	unsigned char	by2DCodeID_2[32]; // 自定义2D 的CodeID信息	
	unsigned int	cmosAdjustMode;	//cmos调光控制模式, 0 - 普通模式（纸质码）；1 - 反光抑制；2 - mobile模式曝光的时候有的帧照明灯亮，有的帧照明灯灭；
									//	3 - 纸质码（motion好，但很难读屏幕显示码）；4 - mobile模式 每帧曝光照明灯都亮。
	//	//0xCCCCC001  add 
	BOOL			bSurePOSPowerUpScanEnable;	//IBM SurePOS: when power up Imager System Status is enable scan or disable scan
	unsigned char  byPostalCodeID[32];		//自定义邮政码前缀
	unsigned char  byPostalCodeID_2[32];	//自定义邮政码前缀(第二位)

	unsigned int	nSucBeepDuration;	//解码成功蜂鸣提示持续时间
	unsigned int	nSucBeepFreq;		//解码成功蜂鸣提示频率
	unsigned int	nSucLedDuration;	//解码成功指示灯持续时间
	BOOL			bPowerLedOn;	//电源指示灯， 0 - 关闭， 1 - 开启
	unsigned char	nCharEncoding[8];	//字符编码选择0-默认,1-UTF-8
	unsigned int	Reserved[29];
	//	//0xCCCCC001  add
	//int	crc32;
}CodeParam2;


typedef struct tagHARDPARAM{
//硬件设置
	unsigned int   nLight; //照明模式
	unsigned int   nFocus; //对焦模式设置
	unsigned int    nProduction;//产品类型
    unsigned int    bDecVoice; //Bit0:解码声音使能,Bit1:开机声音使能
	unsigned int    bOscillation; //振子使能
	unsigned int    nOscillationDelay; //振子延迟
	int				InterType;  // Serial | KBW / HID
	int             nBeepType;  //最低4位用来表示解码声音类型，次低4位表示解码声音音量类型
	unsigned char   nGoodreadLEDDuration;	//读码成功指示灯持续时间
	unsigned char	nExLight;	//外部照明灯控制
	unsigned char	nDofType;	//景深类型
	unsigned char	nGoodreadLED;		//读码成功指示灯使能	//bit[0] 0 - 使能，1 - 禁止；bit[7] 0 - 串行， 1 - 并行
}HardParam;

typedef struct tagKEYSEG{  //用于区分关键字和值 构成的结构
	unsigned char pKeyWord[16];
	unsigned char pValue[512];
}TKEYSEG;

typedef struct tagParamInfo
{
	unsigned int paramLen;//数据区长度
	unsigned int code2StartOffset;
	int	paramVersion;		//参数结构版本
	unsigned int paramCRC;
}TParamInfo;

typedef struct tagCOMPORTPARAM
{
	long  lRate;		//波特率
	long  lDataBits;	//数据
	long  lStopBits;     
	long  lParity;
	int   nPort; 
	int	  nflowCtrl;	//流控
	int  Reserved;
}TComParam;

typedef struct  tagREADMODE{
	int nReadDelay; //保持读码状态时间
	int nCodeDelay; //距离上次读码的间隔时间
	int nHaltDelay; //没有读到任何信息的时间
	BOOL bContinue;  //连续控制读码: 设定后TRUE，摁扳机才能读码，直到下一次摁扳机FALSE
	BOOL bIRSign;   //临时利用此结构进行参数获取-读码后：明码(FALSE)/暗码(TRUE)
	unsigned int    CMOSID; // CMOS1020之前的版本没有ID，CM3020有product ID 0x0A = 0x76, 0x0B = 0x48
	int n1Want;		// 图像采集成功之后的单元值
	int  bDebug;
	int  bDetectHalt; //探测中断
}TREADMODE;

typedef struct tagKeyBoardPARAM
{
	int  nLanguage;
	BOOL isCapsLock;
//	BOOL isNumLock;
	BOOL isUnknownCharactersBeep; //遇到未知字符是否声音提示。取值:0,1 默认:0
	BOOL isShiftLock;
//	BOOL isAutoCaps;
	int isConvertCase;	//强制转换所有字母都成为大写或者小写。取值:0,1,2 默认:0
	BOOL isNumericMode;
	int isKeysrockeDelay;	//发送每个字符之间的延时设置。取值:0,1,2 默认:0
//	int  nCharDelay;    // 字符之间延迟:0不延迟，1延迟20ms，2延迟40ms
//	int  nCodeDelay;    // 条码之间延迟
	int isEmulateKeypad; //采用ALT+ Keypad输入ASCII值 来产生字符。取值:0,1 默认:0
	BOOL isCtrlASCIIMode; //
	//char isPS2CapsLockDef;
	char nCodePage;	//USB KBW alt+keypad模式下选择的codepage序号
	char isKeyPad;	//小键盘打开标志，0：关闭，1：打开。默认：0，关闭
	char isFasttransferMode;
	char nPollRate;	//轮询速率
}UTKeyBoard;

typedef struct tagBLUETOOTHPARAM
{
	unsigned char Name[20];
	BOOL isPSWCheck;
	unsigned char PSW[20];
}UTBlueTooth;

typedef struct tagDeviceINFO
{
	unsigned char pSN[21];
	unsigned char pESN[21];
	unsigned char pDate[21];
	unsigned char pManufID[21]; //ManufactureID
	unsigned char pLIC[21];
	unsigned char pUID[20];
	unsigned char IsDownload;
	unsigned char pManufName[21]; //ManufactureName 
	unsigned char pDofType[21]; //Dof type 
}UTDeviceINFO;

typedef struct tagUTDEC1DSETUP
{
	unsigned char	I2Of5Len[5]; // 交叉2Of5 长度设置和使能,[0]:是否采用目前的四个长度设置:[1][2][3][4]
	unsigned int	unDecTypes; //可解码的参数类型
	unsigned int	unDecParam; //与解码流程相关设置
	unsigned int	ExMsg[32];  //最后16个位作为最大最小信息长度值参数
}UTDec1DSetup;

// typedef struct tagUTDEC2DSETUP
// {
// 	unsigned int	nMacro;		 //	处理宏
// 	int				nQRParam;    // QR 非标码设定参数
// 	int             nReserve[10]; //
// 
// 	unsigned int	unDecTypes; // 可解码的参数类型
// 	unsigned int	ExMsg[10];  // 最大长度：高16bit， 最小长度：低16bit
// 	unsigned int    ExParam[10]; // 细部参数 	
// }UTDec2DSetup;        //前面10个
// 
typedef struct tagUTDEC2DSETUP
{
	unsigned int	nMacro;		 //	处理宏
	int				nQRParam;    // QR 非标码设定参数
	int             nReserve[10]; //
	
	unsigned int	unDecTypes; // 可解码的参数类型
	// 	unsigned int	ExMsg[10];  // 最大长度：高16bit， 最小长度：低16bit
	// 	unsigned int    ExParam[10]; // 细部参数 	
	unsigned int	ExMsg[32];  // 最大长度：高16bit， 最小长度：低16bit	//new
	unsigned int    ExParam[32]; // 细部参数 								//new				
	
}UTDec2DSetup;        //前面10个


// typedef struct tagUTDECOCRSETUP
// {
// 	unsigned int	unDecTypes; // 可解码的参数类型
//  	unsigned int	ExMsg[10];  //
// }UTDecOCRSetup;
// 
typedef struct tagUTDECOCRSETUP
{
	unsigned int	unDecTypes; // 可解码的参数类型
	// 	unsigned int	ExMsg[10];  //
	unsigned int	ExMsg[32];  //									//new
}UTDecOCRSetup;

typedef struct	tagUTDECPOSTALSETUP
{
	unsigned int	unDecTypes;		/*可解码的参数类型*/
	unsigned int	unDecParam;		/*与解码流程相关设置*/
	unsigned int    ExMsg[32];		/*最后16个位作为最大最小信息长度值参数*/
}UTDecPostalSetup;

/* 设置解码的区域 */
typedef struct  tagUIMGAreaParamtag
{
	int top;
	int left;
	int right;
	int bottom;
	int	nType;				// 0：使用默认区域, 1:使用设置的区域 

}UIMGAreaParam;

typedef struct tagDECOUTMSG
{
	int				nBarCode;	// UT_TYPE_1D | UT_TYPE_2D | UT_TYPE_OCR
	int				nCodeType;  // 1D 察看 define1d.h 中 DC_XXX 的定义
	                            // 2D / OCR 察看本文件  DC_XXXX 的定义
	unsigned char*	pMes;		// 解码后的信息
	int				nMes;		// 信息长度
	int             nReResult;  // 返回值
	int				nMacroErr;	// 宏错误代码
	int				nMacroStatus;	// 宏状态代码
}TOUTPUTMSG;

typedef struct  UApp_Decode_Notify
{
	void (*Start_Decode)(void);			//开始解码过程
	void (*Decode_Succeed)(void);		//解码成功
	void (*Failed_or_Timeout)(void);	//解码失败，停止或是超时
}UDECODE_NOTIFY;

//////////////////////////////////////////////////////////////////////////
//	参数结构体定义
#define STID_IMGPARAM		0xCC0001	//图像参数结构体ID
#define STID_OCRSETUP		0xCC0002	//OCR 解码设置结构体ID
#define STID_1DSETUP		0xCC0003	//1D 码解码设置结构体ID
#define STID_2DSETUP		0xCC0004	//2D 码解码设置结构体ID
#define STID_COMMEXTFNC		0xCC0005	//自动处理通讯消息中的指令的扩展功能函数输入结构体ID
#define STID_SETCEXTFNC		0xCC0006	//设置码处理中嵌入扩展的处理功能
#define STID_NEWCODE		0xCC0007	//加入一种新的解码处理
#define STID_CMOSEXTFNC		0xCC0008	//加入对CMOS的扩展功能
#define STID_PARAMEXTFUNC	0xCC0009	//加入参数处理的扩展功能
#define STID_NEWCODEEXT		0xCC000A	//加入一种新的解码处理，简易接口
#define STID_ADDVERMSG		0xCC000B	//加入版本信息查询应答内容
#define STID_UAPPEXTFNC		0xCC000C	//U_app的扩展功能
#define STID_DECODE_NOTIFY	0xCC000D	//应用程序关于开始解码与解码结束或超时的回调接口

#define STID_QUERYUSRAPI	0xCC8000	//查询二次开发使用的API
#define STID_QUERYAPP		0xCC8002
#define STID_QUERYCOMM		0xCC8003
#define STID_QUERYSETC		0xCC8004
#define STID_QUERYMACRO		0xCC8006
#define STID_QUERYPARAM		0xCC8007
#define STID_QUERYCMOS		0xCC8008
#define STID_QUERYDECOD		0xCC8009
#define STID_QUERYBASE		0xCC800A
#define STID_QUERYIMGPROC	0xCC800B	//查询图像处理的功能函数

#define STID_REPLACEIMG		0xCCE000	//替代图像地址
#define STID_REPLACEKEY		0xCCE001	//替代按键函数


typedef struct tagGENERALPARAM
{
	unsigned int Id;			//传递的参数结构体的识别ID
	void *	psParamStruct;		//传递的参数结构体指针
}UTGeneralParam;

typedef struct tagALLPARAM {
	//char *pstrVersion;	//版本信息
	TParamInfo *pParamInfo;
	TComParam	*pCom;
	UTKeyBoard	*pKBW;
	HardParam	*pHard;
	CodeParam	*pCode;
	UTDec1DSetup	*p1D;
	UTDec2DSetup	*p2D;
	UTDecOCRSetup	*pOCR;
	UTDecPostalSetup *pPostal;
	UTDeviceINFO *pDevInfo;
	TREADMODE	*pMode;
	TKEYSEG		*pKeyS;
	CodeParam2  *pCode2;
	DataFormat  *pDFormat;
      UIMGAreaParam *pAreaParam;
} UALLPARAM;

//User使用的API
typedef struct zspTUSERAPI
{
	void	(*U_appSetMsg)(BYTE* pBuff, int nLen);
	void	(*U_appSetCategory)(int nCategory);
	void	(*U_appSetCodeType)(int nType);
	void	(*U_appSetResult)(int nResult);
	BYTE*	(*U_appGetMsgBuff)(void);
	int		(*U_appGetMsgLen)(void);
	int		(*U_appGetMsgBuffSize)(void);
	int		(*U_appGetCategory)(void);
	int		(*U_appGetCodeType)(void);
	int		(*U_appGetResult)(void);
	BOOL	(*U_appCaptureImg)(BOOL bWithLight);
	BOOL	(*U_appProcDecode)(void);
	BOOL	(*U_appMsgEditting)(void);
	BOOL	(*U_appMsgPacking)(void);
	BOOL	(*U_appDealCmd)(BYTE *pCmdString, int nLen);
	void	(*U_appSetUserFunc)(UTGeneralParam *psParam);
	BOOL	(*U_appKeyPressed)(void);
	void	(*U_appSleepMode)(void);
	void	(*U_appWillSleepTillWakeup)(int nIdleMiliSec);
	void	(*U_appGoingToSleep)(void);
	void	(*U_appBackFromSleep)(void);
	void	(*U_appBKGrndProc)(void); 
	void	(*U_appQueryAPI)(UTGeneralParam *psParam);
	BOOL	(*U_appDecodeWithMacro)(void);
	int		(*U_appGetVersionInfo)(char *pBuff, int nBuffSize);
	
	int		(*Y_commGetCurType)(void);
	BOOL	(*Y_commEnable)(void);
	BOOL	(*Y_commIsEnable)(void);
	BOOL	(*Y_commDisable)(void);
	int 	(*Y_commRead)(unsigned char * buff, unsigned int size, int msTimeOut);
	int 	(*Y_commWrite)(unsigned char *buff, unsigned int size);
	BOOL	(*Y_commClean)(void);
	int 	(*Y_commSendChar)(char cChar);
	int 	(*Y_commReadChar)(char *pcChar,int msTimeOut);
	int 	(*Y_commBuffCount)(void);
	
	BOOL 	(*Y_paramLoadAppBlock)(int nBlockOrder, BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramSaveAppBlock)(int nBlockOrder, BYTE *pBuff, int nSize);

	
	void 	(*Y_cmosSetWantLux)(int nWantLux);
	void 	(*Y_cmosMirror)(int nMode);

	void	(*Z_Beep)( int freq, int mdelay, int volume );
	void	(*Z_PlayMusic)(char * pcMusic);
	void	(*Z_usDelay)(int us);
	void	(*Z_msDelay)(int ms);
	int 	(*Z_Get10msCounter)(void);	
	void	(*Z_Oscillate)(int ms);
	void	(*Z_DogFeed)(void);
	int 	(*Z_DogGetBreak)(void); 
	int 	(*Z_DogReset)(int ms); 
	void	(*Z_LedOn)(int clr);
	void	(*Z_LedOff)(int clr);
	void	(*Z_LightOn)(void);
	void	(*Z_LightOff)(void);
	void	(*Z_IRLightOn)(void);
	void	(*Z_IRLightOff)(void);
	void	(*Z_FocusOn)(void);
	void	(*Z_FocusOff)(void);
	void	(*Z_sysReset)(void);
	int		(*Z_GetIdleCounter)(void);
	void	(*Z_OscillateWithBeep)( int freq, int mdelay, int volume );
	void	(*Z_DeepSleep)(void);	//深度休眠
	//添加
	void	(*U_appPowerOnDisplay)(void);
	BOOL	(*U_appMsgAddTerminal)(void);
	void	(*U_appDevicePowerOnBeep)(void);
	int		(*U_appRunDecodeTest)(int nTestMode, int nTestParam);
	void*	Reserve[16];
}TUserAPI;

//App 的API
typedef struct zspTAPPAPI
{
	void	(*U_appSetMsg)(BYTE* pBuff, int nLen);
	void	(*U_appSetCategory)(int nCategory);
	void	(*U_appSetCodeType)(int nType);
	void	(*U_appSetResult)(int nResult);
	BYTE*	(*U_appGetMsgBuff)(void);
	int		(*U_appGetMsgLen)(void);
	int		(*U_appGetMsgBuffSize)(void);
	int		(*U_appGetCategory)(void);
	int		(*U_appGetCodeType)(void);
	int		(*U_appGetResult)(void);
	BOOL	(*U_appCaptureImg)(BOOL bWithLight);
	BOOL	(*U_appProcDecode)(void);
	BOOL	(*U_appMsgEditting)(void);
	BOOL	(*U_appMsgPacking)(void);
	BOOL	(*U_appDealCmd)(BYTE *pCmdString, int nLen);
	void	(*U_appSetUserFunc)(UTGeneralParam *psParam);
	BOOL	(*U_appKeyPressed)(void);
	void	(*U_appSleepMode)(void);
	void	(*U_appWillSleepTillWakeup)(int nIdleMiliSec);
	void	(*U_appGoingToSleep)(void);
	void	(*U_appBackFromSleep)(void);
	void	(*U_appBKGrndProc)(void); 
	void	(*U_appQueryAPI)(UTGeneralParam *psParam);
	BOOL	(*U_appDecodeWithMacro)(void);
	int		(*U_appGetVersionInfo)(char *pBuff, int nBuffSize);
	//添加
	void	(*U_appPowerOnDisplay)(void);
	BOOL	(*U_appMsgAddTerminal)(void);
	void*	Reserve[18];
}TAppAPI;

//Communication
typedef struct zspTCOMMAPI
{
	void	(*V_commAutoCheckCmd)(void); 
	void	(*V_commSetUserFunc)(UTGeneralParam *psParam);
	int		(*Y_commGetCurType)(void);
	BOOL	(*Y_commEnable)(void);
	BOOL	(*Y_commIsEnable)(void);
	BOOL	(*Y_commDisable)(void);
	BOOL	(*Y_commChgToRS232)(TComParam *psParam);
	BOOL	(*Y_commChgToUSBDataPipe)(void);
	BOOL	(*Y_commChgToKBW)(UTKeyBoard *psParam);
	int 	(*Y_commGetBaudrate)(void);
	int 	(*Y_commGetKBWCountry)(void);
	int 	(*Y_commRead)(unsigned char * buff, unsigned int size, int msTimeOut);
	int 	(*Y_commWrite)(unsigned char *buff, unsigned int size);
	BOOL	(*Y_commClean)(void);
	int 	(*Y_commSendChar)(char cChar);
	int 	(*Y_commReadChar)(char *pcChar,int msTimeOut);
	int 	(*Y_commBuffCount)(void);
	int 	(*Y_commBakRead)(unsigned char * buff, unsigned int size, int msTimeOut);
	int 	(*Y_commBakReadChar)(char *pcChar,int msTimeOut);
	int 	(*Y_commBakBuffCount)(void);
	int 	(*Z_comm_GetType)(void); 
	int 	(*Z_comm_Read)(unsigned char * buff, unsigned int size);
	int		(*Z_comm_Write)(unsigned char * buff, unsigned int size);
	int		(*Z_comm_Config)(void * lpsParam); 
	int		(*Z_comm_GetStatue)(void * lpsParam);
	//添加
	void*	Reserve[20];
}TCommAPI;

//Setting code
typedef struct zspTSETCAPI
{
	int		(*W_setcDealWith)(void); 
	void 	(*W_setcSetUserFunc)(UTGeneralParam *psParam);
	void	(*W_setcParamInit)(void); 
	BOOL	(*W_setcIsMacroCancel)(BYTE* pbyMsg, int nMsgLen);
	int		(*W_setcGetResult)(void);
	void	(*W_setcSetResult)(int nResult);
	BOOL	(*W_setcToSaveParam)(void);
	void	(*W_setcSetSaveFlag)(BOOL bToSave);
	void	(*W_setcEnable)(BOOL bEnable);
	BOOL	(*W_setcIsEnable)(void);
	BOOL	(*W_setcParseSigle)(BYTE* pbyMsg, int nMsgLen, BOOL bToSave);
	BOOL	(*W_setcParseBatch)(BYTE* pbyMsg, int nMsgLen);
	//添加
	void*	Reserve[20];
}TSetcAPI;

//Macro code
typedef struct zspTMACROAPI
{
	void 	(*W_macroInitialParam)(void);
	int 	(*W_macroMain)(void);
	int 	(*W_macroGetParam)(void);
	void 	(*W_macroSetParam)(int nMacro);
	BYTE* 	(*W_macroGetBuff)(void); 
	int 	(*W_macroGetBuffLen)(void);
	int 	(*W_macroGetError)(void);
	void 	(*W_macroSetError)(int nError);
	int 	(*W_macroGetStatus)(void);
	BOOL	(*W_macroGetUnmacthParam)(void);
	int 	(*W_macroGetFlag)(void);
	void 	(*W_macroSetFlag)(int nFlag);
	//添加
	void*	Reserve[20];
}TMacroAPI;

//Params storage
typedef struct zspTPARAMAPI
{
	BOOL	(*X_paramIsAllParamInited)(void);
	void	(*X_paramInitedAllParam)(void);
UALLPARAM* 	(*X_paramGetGlobalAllParam)(void);
	BOOL 	(*X_paramLoadSysDefaultParam)(void);
	BOOL 	(*X_paramLoadUsrDefaultParam)(void);
	BOOL 	(*X_paramSaveAsSysDefaultParam)(void); 
	BOOL 	(*X_paramSaveAsUsrDefaultParam)(void);
	BOOL 	(*X_paramLoadDeviceInfo)(void); 
	BOOL 	(*X_paramSaveDeviceInfo)(void);
	BOOL 	(*X_paramOriginalInitAllParam)(void);
	BOOL 	(*X_paramPlaceKeyword)(char* pKeyWord,char* pValue, BOOL bReplace);
	BOOL 	(*X_paramQueryKeyword)(char* pKeyWord,char* pValue);
	int 	(*X_paramAnalysisKeyword)(BYTE* pParam,int nParam,BYTE* pWord,BYTE * pValue);
	int 	(*X_paramGetKeywordsStruct)(BYTE *pBuff,int nBuff,TKEYSEG* tKS);
	BOOL 	(*X_paramIsParamsRight)(BYTE *pBuff, int nSize);
	BOOL 	(*X_paramGetParamFromBuff)(BYTE *pBuff, int nSize);
	int 	(*X_paramPushParamToBuff)(BYTE *pBuff, int nBuffSize);
	void 	(*X_paramSendParamInfo)(void);
	void 	(*X_paramUpdateToDevice)(void);
	int  	(*X_paramSelectUsefulCommType)(void);
	BOOL 	(*Y_paramLoadSysDefault)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramLoadUsrDefault)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramLoadOEM)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramLoadAppBlock)(int nBlockOrder, BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramSaveSysDefault)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramSaveUsrDefault)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramSaveOEM)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramSaveAppBlock)(int nBlockOrder, BYTE *pBuff, int nSize);
	int  	(*Z_param_Load)(BYTE * buff, int nSectOffs, unsigned int size);
	int  	(*Z_param_Save)(BYTE * buff, int nSectOffs, unsigned int size);
	//添加
	void*	Reserve[20];
}TParamAPI;

//CMOS
typedef struct zspTCMOSAPI
{
	void 	(*Y_cmosInit)(void);
	void 	(*Y_cmosCapture)(void);
	int 	(*Y_cmosGetWidth)(void);
	int 	(*Y_cmosGetHeight)(void);
	char* 	(*Y_cmosGetDataAddr)(void);
	void 	(*Y_cmosSetWantLux)(int nWantLux);
	void 	(*Y_cmosAdjust)(void);
	int 	(*Y_cmosCalcLux)(void);
	void 	(*Y_cmosMirror)(int nMode);
	void 	(*Y_cmosSetUserFunc)(UTGeneralParam *psParam);
	int  	(*Z_cmos_GetType)(void); 
	int  	(*Z_cmos_GetI2C)(int reg);	
	int  	(*Z_cmos_SetI2C)(int reg, int value);
	int*  	(*Z_cmos_Read)(int mode); 
	int*  	(*Z_cmos_GetIMGbuff)(void);
	//添加
	void*	Reserve[20];
}TCmosAPI;

//Decode
typedef struct zspTDECODEAPI
{
	BOOL  	(*X_decoSetProcParamS)(UTGeneralParam * psParam);
	BOOL  	(*X_decoSetUserFunc)(UTGeneralParam *psParam);
	void	(*X_decoInitProcParam)(void);
	int		(*X_decoGetUIMGVersion)(char *pBuff, int nBuffSize);
	BOOL	(*X_decoImgProcMain)(void);
	void	(*X_decoSetImgParam)(UTImgParam *psParam);
	BYTE*	(*X_decoGetDecInfoBuff)(void);
	int		(*X_decoGetDecInfoLen)(void);
	int		(*X_decoGetResult)(void);
	BOOL  	(*X_decoIsMacroCode)(void);
	int  	(*X_decoGetMacroOrderFrom)(void);
	int   	(*X_decoGetMacroOrderTo)(void);
	int   	(*X_decoGetMacroTotalCnt)(void);
	int   	(*X_decoGetMacroIDLen)(void);
	int	  	(*X_decoGetMacroID)(int nPos);
TOUTPUTMSG*	(*X_decoGetDecOutMsg)(void);
	int		(*X_decoGetBarCode)(void);
	int		(*X_decoGetCodeType)(void);
	int		(*X_decoGetCodePos)(void);
	int		(*X_decoGetBitsPosition)(int nInt);
	void	(*X_decoSet2DParam)(UTDec2DSetup *psParam);
	void	(*X_decoSet1DParam)(UTDec1DSetup *psParam);
	void	(*X_decoSetOCRParam)(UTDecOCRSetup *psParam);
	//添加
	void	(*X_decoSetPostalParam)(UTDecPostalSetup *psParam);
	void*	Reserve[19];
}TDecoAPI;

//Foundation api
typedef struct zspTBASEAPI
{
	void	(*Z_Beep)( int freq, int mdelay, int volume );
	void	(*Z_PlayMusic)(char * pcMusic);
	void	(*Z_usDelay)(int us);
	void	(*Z_msDelay)(int ms);
	int 	(*Z_CounterGet)(void); 
	int 	(*Z_GetCounterDensity)(void); 
	int 	(*Z_Get10msCounter)(void);	
	void	(*Z_Oscillate)(int ms);
	void	(*Z_DogFeed)(void);
	int 	(*Z_DogGetBreak)(void); 
	int 	(*Z_DogReset)(int ms); 
	int 	(*Z_KeyPressed)(void);
	void	(*Z_LedOn)(int clr);
	void	(*Z_LedOff)(int clr);
	void	(*Z_LightOn)(void);
	void	(*Z_LightOff)(void);
	void	(*Z_IRLightOn)(void);
	void	(*Z_IRLightOff)(void);
	void	(*Z_FocusOn)(void);
	void	(*Z_FocusOff)(void);
	int 	(*Z_oem_GetUid)(BYTE * buff, unsigned int maxsize);
	int 	(*Z_oem_GetLicense)(BYTE * buff, unsigned int maxsize);
	int 	(*Z_oem_SetLicense)(BYTE * buff, unsigned int size);
	int 	(*Z_oem_LicenseLock)(void); 
	void	(*Z_sysReset)(void);
	int 	(*Z_sysSleep)(void);
	int		(*Z_GetIdleCounter)(void);
	
	int		(*Z_param_Load)(unsigned char * buff, int nSectOffs, unsigned int size);
	int		(*Z_param_Save)(unsigned char * buff, int nSectOffs, unsigned int size);
	int		(*Z_cmos_GetI2C)(int reg);
	int		(*Z_cmos_SetI2C)(int reg, int value);
	int*	(*Z_cmos_Read)(int mode); 
	int*	(*Z_cmos_GetIMGbuff)(void);
	int		(*Z_cmos_GetType)(void);
	
	int		(*Z_GetHardwareInfo)(char *pBuff, int nBuffSize); //获取硬件版本信息
	void	(*Z_DeepSleep)(void);	//深度休眠
	//添加
	void*	Reserve[20];
}TBaseAPI;


#endif//__UIMG_DEVELOP_INTERFACE_COMMON_H__



