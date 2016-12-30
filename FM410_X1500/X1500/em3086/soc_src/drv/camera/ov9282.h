/*!
 * @file ov9282.h
 */

#ifndef __OV9282_H__
#define __OV9282_H__

#define OV9282_IMAGE_W 752
#define OV9282_IMAGE_H 480


#ifdef __cplusplus
extern "C" {
#endif

#define OV9282_ID_REG		 				0x300a
#define OV9282_VERTICAL_FLIP_REG 		0x3820
#define OV9282_HORIZONAL_MIRROR_REG		0x3821


BOOL IsOV9282(void);

void OV9282_Init(void);
int OV9282_reg_read (unsigned int addr, unsigned int *buf);
int OV9282_reg_write(unsigned int addr, unsigned int data);


int OV9282_GetLux(unsigned char *pImg);
void OV9282_Adjust(int nWantLux, unsigned char* pImgData, unsigned int nCount, unsigned int nLightOn, unsigned int nLightOff);
int OV9282_Width(void);
int OV9282_Height(void);
int OV9282_Width_For_CSI(void);
int OV9282_Height_For_CSI(void);

void OV9282_Mirror(int mode);


int OV9282_ReservedDataCnt(void);

void OV9282_Capture_Mode(int mode);
void OV9282_Standby(BOOL b);
void OV9282_Exposure_update(void);
void OV9282_Info(CSINF *p);
void * OV9282_LightHandle(int mode);
unsigned char * OV9282_ImgData(unsigned char *p, unsigned int *pFlag);	//Í¼ÏñÎÞÐè×ª»»


#ifdef __cplusplus
}
#endif //__cplusplus


#endif

