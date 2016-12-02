
#ifndef __UIMG_DEVELOP_INTERFACE_MACROLINK_H__ 
#define __UIMG_DEVELOP_INTERFACE_MACROLINK_H__

/*********************************************************************************************
*	include files 
*********************************************************************************************/
#include "../../list.h"

/*********************************************************************************************
*	global referenced macro defines & type defines
*********************************************************************************************/

#define  MACRO_DEAL_VERSION  "2.01"// Macro 宏处理有效


//和继续处理和声音有关系
#define U_MACRO_ID_UNCONFORMITY		-10		// 无效，和原来的宏ID不符: 同一种条码 MACRO_ERROR
#define U_MACRO_SUPERPOSITION		-11		// 宏重复读取 // MACRO_ERROR
#define U_MACRO_TYPE_UNCONFORMITY	-12		// 和原来的宏类型不符 : 不同类型条码 // MACRO_ERROR
#define U_MACRO_OVERFLOW			-13		// 顺序发送/缓存: 超出可用链接数(MACRO_MAX_LINK == 16)或缓存超过64k限制// MACRO_ERROR

// 和信息发送，声音有关
#define U_NO_MACRO					 0		// 普通的信息 // Nothing
#define U_MACRO_END					 1		// 顺序缓存结束：宏读码结束 //MACRO_END
												// ** 顺序发送结束，也是此返回参数
#define U_MACRO_SEQ_SEND	         2		// 顺序发送 / 可发送连接好的字符 //MACRO_OK
#define U_MACRO_BUFF		         10		// 顺序发送/缓存中: 只存不送// MACRO_OK

#define U_uIMG_STATUS_MACRO_START		0x000200   // 宏读码开始
#define U_uIMG_STATUS_MACRO_END			0x000201   // 宏读码结束
#define U_uIMG_STATUS_MACRO_ERROR		0x000202   // 宏读码错误
#define U_uIMG_STATUS_MACRO_SUC			0x000203   // 宏读码正确


// 宏设置定义
#define  MACRO_DIRECT				0x01	//读取后直接发送，不进行任何处理
#define  MACRO_SEQ_BUFF				0x02	//按照顺序发送，可以多达64k的缓存，超出缓存，系统不进行自动清空，需要读取设置码的取消清空buff
#define  MACRO_LINK_16				0x03	//读取完毕，按照顺序链接后发送，可以链接多达16个或者小于64k信息的符号


/*********************************************************************************************
*	global referenced variables
*********************************************************************************************/

/*********************************************************************************************
*	global function prototype
*********************************************************************************************/


#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************
返回值----
	#define U_RE_MACRO_ERROR            -1      
	#define U_RE_MACRO_NO_END_SEND      0		// 非宏条码/宏结束/宏信息发送
	#define U_RE_MACRO_BUFF_DELAY       2       // 缓存延迟,不发送信息，解码成功
************************************************************************/
//初始化参数
void W_macroInitialParam(void);
//获取及设置宏处理办法
// MACRO_DIRECT 或 MACRO_SEQ_BUFF 或 MACRO_LINK_16
int W_macroGetParam(void);
void W_macroSetParam(int nMacro);

//处理宏连接等
int W_macroMain(void);

//获取信息,及其长度
//  由于数据可能超出原来传入的数据空间的大小，所以，必须另外开辟空间
unsigned char* W_macroGetBuff(void); 
int W_macroGetBuffLen(void);


//获取宏处理出错信息状态值
int W_macroGetError(void);
void  W_macroSetError(int nError);

// 宏信息处理后的状态
int W_macroGetStatus(void);

//是否符合宏类型
// TRUE ---  继续处理
// FALSE --- 宏出现冲突
BOOL W_macroGetUnmacthParam(void);
//标志：是否前一个条码是宏条码 <0 就不是宏
int W_macroGetFlag(void);
void W_macroSetFlag(int nFlag);

//需要初始化宏的标志
BOOL W_macroGetInitFlag(void);
void W_macroSetInitFlag(void);

#ifdef __cplusplus
}
#endif

#endif //#ifndef __UIMG_DEVELOP_INTERFACE_MACROLINK_H__ 

