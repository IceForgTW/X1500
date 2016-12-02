/*!
 * @file GC0308.h
 */

#ifndef __GC0308_H__
#define __GC0308_H__

// #include "cmos.h"

#define GC0308_IMAGE_W 640
#define GC0308_IMAGE_H 480


#ifdef __cplusplus
extern "C" {
#endif

void GC0308_Info(CSINF *p);

void GC0308_Init(void);
int GC0308_reg_read (unsigned int addr, unsigned int *buf);
int GC0308_reg_write(unsigned int addr, unsigned int data);


int GC0308_GetLux(unsigned char *pImg);
void GC0308_Adjust(int nWantLux, unsigned char* pImgData, unsigned int nCount, unsigned int nLightOn, unsigned int nLightOff);
int GC0308_Width(void);
int GC0308_Height(void);
int GC0308_Width_For_CSI(void);
int GC0308_Height_For_CSI(void);

void GC0308_Mirror(int mode);

unsigned char * GC0308_ImgData(unsigned char *p, unsigned int *pFlag);	

void GC0308_Capture_Mode(int mode);
void GC0308_Standby(BOOL b);
void GC0308_Exposure_update(void);
void * GC0308_LightHandle(int mode);


#ifdef __cplusplus
}
#endif //__cplusplus


#endif

