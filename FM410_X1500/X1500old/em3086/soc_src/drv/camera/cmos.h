/*!
 * @file cmos.h
 */

#ifndef __CMOS_H__
#define __CMOS_H__

#include <list.h>
#include "i2c.h"

#define	CMOSGetUsedPic		0
#define CMOSGetNewPic		1

#define LIGHT_ON_DELAY		3000
#define LIGHT_ON_COUNT		1
#define LIGHT_OFF_COUNT		1

#define CMOS_I2C_CLK_DEFAULT	100000
#define CMOS_I2C_CLK_HIGH		400000


#define CIM_STANDBY	     (32*1+4)
#define CIM_RST		     (32*1+5)
#define CIM_EXPOSURE     (32*0+4)
#define CIM_LEDOUT       (32*1+21)
#define CIM_FOC          (32*0+5)
#define CIM_LIGHT        (32*2+24)
#define CIM_PWR          (32*0+20)
#define PAINTC          *(volatile unsigned int *)0xb0010018
#define PAMSKC         *(volatile unsigned int *)0xb0010028
#define PAPAT1S        *(volatile unsigned int *)0xb0010034
#define PAPAT0C        *(volatile unsigned int *)0xb0010048
#define CLKGR            *(volatile unsigned int *)(0xB0000020)
#define CIMCDR          *(volatile unsigned int *)(0xB000007C)
#define CIMCR            *(volatile unsigned int *)(0xB3060004)



#define IMG_FORMAT_MONOCHROME_8BIT		0
#define IMG_FORMAT_YUV422				1

#define CMOS_SHUTTER_TYPE_GLOBAL	0
#define CMOS_SHUTTER_TYPE_ROLLING	1

#define CMOS_FLAG_DATA_ORIGINAL		0
#define CMOS_FLAG_DATA_PROCESSED	1

typedef struct _cmosInfo
{
	BOOL bValid;
	int nImgFormat;
	int nShutterType;
	int nInputClk;
	int nI2cClk;
} CSINF;


typedef void	(*FNC_cmosInfo)(CSINF *p);
typedef void	(*FNC_cmosInit)(void);
typedef int		(*FNC_cmosGetWidth)(void);
typedef int		(*FNC_cmosGetHeight)(void);
typedef void	(*FNC_cmosAdjust)(int nWantLux, unsigned char* pImgData, unsigned int nCount, unsigned int nLightOn, unsigned int nLightOff);
typedef int		(*FNC_cmosCalcLux)(unsigned char* pImgData);
typedef void	(*FNC_cmosMirror)(int nMode);
typedef int		(*FNC_cmosRegRead)(unsigned int addr, unsigned int *buf);
typedef int		(*FNC_cmosRegWrite)(unsigned int addr, unsigned int data);
typedef int		(*FNC_cmosGetWidthForCSI)(void);
typedef int		(*FNC_cmosGetHeightForCSI)(void);
typedef unsigned char *	(*FNC_cmosImgData)(unsigned char *p, unsigned int *pFlag);
typedef void	(*FNC_cmosCaptureMode)(int mode);
typedef void	(*FNC_cmosStandby)(BOOL bstandby);
typedef void	(*FNC_cmosExposureUpdate)(void);
typedef void *	(*FNC_cmosLightHandle)(int mode);


#define CMOSCAPTURE_STOP	0
#define CMOSCAPTURE_PRINT	1
#define CMOSCAPTURE_MOBILE	2
#define CMOSCAPTURE_DETECT	3



typedef struct zspCMOSOPRATE
{
	int					nIDForCMOS;	//֧�ֵ�CMOS����ID
	FNC_cmosInfo		fnc_Info;	//cmos�Ƿ����
	FNC_cmosInit		fnc_Init;	//CMOS��ʼ��
	FNC_cmosGetWidth	fnc_GetWidth;	//ͼ����
	FNC_cmosGetHeight	fnc_GetHeight;	//ͼ��߶�
	FNC_cmosAdjust		fnc_Adjust;		//���ݴ�������Ⱥ��������ȵ����ع������
	FNC_cmosCalcLux		fnc_CalcLux;	//���㲶���ͼ�������
	FNC_cmosMirror		fnc_Mirror;	//���þ��� nMode: 0 - Normal; 1 - horizontal mirror; 2 - vertical flip; 3 - horizontal mirror & vertical flip.
	FNC_cmosRegRead		fnc_RegRead;	//�Ĵ���������
	FNC_cmosRegWrite	    fnc_RegWrite;	//�Ĵ���д����
	FNC_cmosGetWidthForCSI	fnc_GetWidth_ForCSI;	//��������cpu��CSI�ӿڵĿ��ֵ����csi�ӿڲ��졢cmos sensor���죬��������Ӧ�ӿڣ�
	FNC_cmosGetHeightForCSI fnc_GetHeight_ForCSI;	//��������cpu��CSI�ӿڵĸ߶�ֵ����csi�ӿڲ��졢cmos sensor���죬��������Ӧ�ӿڣ�
	FNC_cmosImgData			fnc_ImgData;		//�Խ��յ���ͼ�����ݴ�������ʵ��ͼ��ָ��
	FNC_cmosCaptureMode		fnc_CaptureMode;	//0 - ֹͣ��ͼ�� 1 - ������ͼ-ֽ�ʣ�2 - ������ͼ-ֽ������Ļ��3 - ���״̬��
	FNC_cmosStandby			fnc_Standby;	//CMOS Sensor Low Power
	FNC_cmosExposureUpdate	fnc_ExposureUpdate;	//�����ع������cmos sensor 
	FNC_cmosLightHandle		fnc_LightHandle;	//�ƹ���ƾ��

}TYCMOSOperate;

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

void CIM_init(void);

unsigned char *ReadCmosBuf(unsigned int mode, unsigned int timeout);

int Get_Cmos_type(void);


int CMosWidth(void);
int CMosHeight(void);
void CMosMirror(int nMode);
void csi_capture(BOOL b, int mode);

void set_cmos_light_mode(int nMode);
int get_cmos_light_mode(void);
void Set_Mobile_mode(int mode);

void CMosREGSet(int addr,int data);
int CMosREGRead(int addr);


int CMosWidth(void);
int CMosHeight(void);
void CMosMirror(int nMode);

unsigned char *ReadCmosBufTest(int n);


void foc_on(BOOL b);
void foc_on_delay(int us);
void light_on(BOOL b);
void light_on_delay(int us);


#if defined(__cplusplus)
}
#endif

//! @}

#endif //__CMOS_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
