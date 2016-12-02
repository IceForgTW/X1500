
#ifndef __UIMG_DEVELOP_INTERFACE_IMGCAPTURE_H__
#define __UIMG_DEVELOP_INTERFACE_IMGCAPTURE_H__

#include "../../list.h"
/************************************************************************/
/* 类型定义                                                             */
/************************************************************************/

/************************************************************************/
/* 全局标志量                                                           */
/************************************************************************/

/************************************************************************/
/* 宏定义                                                               */
/************************************************************************/
//CMOS类型值
/*#define T_CMOS_OV7141		0x01
#define T_CMOS_OV9650		0x02		
#define T_CMOS_MT9V022		0x03
#define T_CMOS_OV7221		0x05
#define T_CMOS_OV9210		0x06*/


#define EXPOSURE_MAX_WIDTH_NR	80		//近景深
#define EXPOSURE_MAX_WIDTH_MR	100		//200行
#define EXPOSURE_MAX_WIDTH_FR	130		//200行

#define EXPOSURE_AG_NR			0x18
#define EXPOSURE_AG_MR			0x18
#define EXPOSURE_AG_FR			0x16

#define EXPOSURE_DG_NR			0xfa//0xf8	
#define EXPOSURE_DG_MR			0xfa	
#define EXPOSURE_DG_FR			0xfb	

#define EXPOSURE_AG				EXPOSURE_AG_MR
#define EXPOSURE_DG				EXPOSURE_DG_MR
#define EXPOSURE_MAX_WIDTH		EXPOSURE_MAX_WIDTH_MR



#define CAPTURE_START			0
#define CAPTURE_WAIT			1
#define CAPTURE_STOP			2
#define CAPTURE_ONCE_AND_STOP	3

#define READ_SCANNING			0
#define READ_DETECETING			1

/************************************************************************/
/*  函数实现                                                            */
/************************************************************************/

#ifdef __cplusplus
extern"C"{
#endif

//////////////////////////////////////////////////////////////////////////
//CMOS采集接口
//初始化CMOS
void Y_cmosInit(void);
//获得图像
void Y_cmosCapture_Ex(int Xmode, int Ymode);
void Y_cmosCapture(void);
//获取图像宽度
int Y_cmosGetWidth(void);
//获取图像高度
int Y_cmosGetHeight(void);
//获取图像地址
char * Y_cmosGetDataAddr(void);
//设置图像的预期亮度 值域为 0x00 ~ 0xff;
void Y_cmosSetWantLux(int nWantLux);
//调整预期亮度
void Y_cmosAdjust(void);
//获取图像亮度特征值
int Y_cmosCalcLux(void); //输出为 0x00～0xff;
//设置CMOS采集为镜像, nMode: 0 - Normal; 1 - horizontal mirror; 2 - vertical flip
void Y_cmosMirror(int nMode);

//设置扩展的CMOS处理功能
void Y_cmosSetUserFunc(UTGeneralParam *psParam);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
#ifdef __cplusplus
}
#endif

#endif //__UIMG_DEVELOP_INTERFACE_IMGCAPTURE_H__

