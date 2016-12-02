
#ifndef __UIMG_DEVELOP_INTERFACE_YCOMM_H__
#define __UIMG_DEVELOP_INTERFACE_YCOMM_H__

#include "../../list.h"
/************************************************************************/
/* 类型定义                                                             */
/************************************************************************/
//串口参数结构
/************************************************************************/
/* 全局标志量                                                           */
/************************************************************************/

/************************************************************************/
/*  函数实现                                                            */
/************************************************************************/

#ifdef __cplusplus
extern"C"{
#endif

//////////////////////////////////////////////////////////////////////////
// 通讯接口封装

BOOL Y_commIsBTOpened(void);
BOOL Y_commIsRS232Opened(void);
BOOL Y_commIsUSBOpened(void);
BOOL Y_commIsPS2Opened(void);
BOOL Y_commIsSurePOSTTOpened(void);
BOOL Y_commIsSurePOSHHOpened(void);
void Y_commCloseBT(void);
void Y_commCloseUSB(void);
void Y_commCloseRS232(void);
void Y_commClosePS2(void);
void Y_commCloseHIDPOS(void);
void Y_commCloseSurePOS_TT(void);
void Y_commCloseSurePOS_HH(void);


//获取当前的通讯接口类型
int	 Y_commGetCurType(void);
//使能通讯接口，使之可以收发
BOOL Y_commEnable(void);
//判断通讯接口是否使能
BOOL Y_commIsEnable(void);
//关闭通讯接口，使之不可收发
BOOL Y_commDisable(void);
//将通讯接口切换到RS232方式
BOOL Y_commChgToRS232(TComParam *psParam);
//将通讯接口切换到USB 的 DataPipe方式
BOOL Y_commChgToUSBDataPipe(void);
//将通讯接口切换到USB的RS232方式
BOOL Y_commChgToUSBRS232();


BOOL Y_commChgToUSBCDC();

//将通讯接口切换到 KBW 方式
BOOL Y_commChgToKBW(UTKeyBoard *psParam);
//将通讯接口切换到BlueTooth方式
BOOL Y_commChgToBT(void);
//将通讯接口切换到PS2方式
BOOL Y_commChgToPS2(UTKeyBoard *psParam);
//将通讯接口切换到HID-POS方式
BOOL Y_commChgToHIDPOS(void);
//将通讯接口切换到IBM SurePOS方式
BOOL Y_commChgToSurePOS_TT(void);
BOOL Y_commChgToSurePOS_HH(void);
// 获取波特率
int Y_commGetBaudrate(void);
//获取KBW的键盘分布国家代号
int Y_commGetKBWCountry(void);
//从通讯口，在指定超时时间内读出信息
int Y_commRead(unsigned char * buff, unsigned int size, int msTimeOut);
//往通讯中发送信息
int Y_commWrite(unsigned char *buff, unsigned int size);
//发送字符串信息
int Y_commSendString(const char *str);

//清空接收缓冲区中的数据
BOOL Y_commClean(void);
//发送一个字符
int Y_commSendChar(char cChar);
//读入一个字符
int Y_commReadChar(char *pcChar,int msTimeOut);
//获取接收缓冲区中的数据量
int Y_commBuffCount(void);
//用于后台指令处理的读缓冲区数据
int Y_commBakRead(unsigned char * buff, unsigned int size, int msTimeOut);
//用于后台指令处理的读缓冲区单字符
int Y_commBakReadChar(char *pcChar,int msTimeOut);
//用于后台指令处理的缓冲区数据量检查
int Y_commBakBuffCount(void);
// 通讯接口封装
//////////////////////////////////////////////////////////////////////////
BOOL BlueT_set_slave_pin(char *pin, int nlen);//设置slave模式下的匹配密码
void BlueT_set_auth_status(BOOL nstat);	//设置是否进行密码匹配
void BlueT_reset_cfg(void);							//复位所有的配置
BOOL BlueT_set_dev_name(char *devname, int nlen);	//设置设备名称 
//////////////////////////////////////////////////////////////////////////
//
#ifdef __cplusplus
}
#endif

#endif //__UIMG_DEVELOP_INTERFACE_YCOMM_H__

