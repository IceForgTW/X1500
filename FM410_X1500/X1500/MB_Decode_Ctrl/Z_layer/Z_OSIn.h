/********************************************************************
	created:	2007/02/08
	created:	8:2:2007   16:35
	filename: 	Interface.h
	file path:	
	file base:	Interface
	file ext:	h
	author:		LieMa
	
	purpose:	����ӿ�
*********************************************************************/

#ifndef __UIMG_DEVELOP_INTERFACE_OSIN_H__
#define __UIMG_DEVELOP_INTERFACE_OSIN_H__

#include "../list.h"
#include "Z_define.h"

/************************************************************************/
/* ���Ͷ���                                                             */
/************************************************************************/

// typedef struct zspHIDKBW
// {
// 	int  nLanguage;		//����
// 	int isAutoCaps;	//�Զ��л���Сд
// 	int isNumericMode;	//����ģʽ
// 	int isControlASCIIMode;	//Control + ASCII ģʽ
// 	int  Reserved[2];	//����
// }UTHIDKBW;

typedef struct zspHIDKBW
{
	int  nLanguage;		//����
	int isCapsLock;	//�Զ��л���Сд
	int isNumericMode;	//����ģʽ
	int isControlASCIIMode;	//Control + ASCII ģʽ

	int isKeysrockeDelay;	//����ÿ���ַ�֮�����ʱ���á�ȡֵ��0��1��2 Ĭ�ϣ�0��
	int isConvertCase;		//ǿ��ת��������ĸ����Ϊ��д����Сд��ȡֵ��0��1��2 Ĭ�ϣ�0��
	int isEmulateKeypad;	//����ALT+ Keypad����ASCIIֵ �������ַ���ȡֵ��0��1 Ĭ�ϣ�0�� 
	int isUnknownCharactersBeep;	//����δ֪�ַ��Ƿ�������ʾ��ȡֵ��0��1 Ĭ�ϣ�0��
	int isKeyPad;	//С���̴򿪱�־��0���رգ�1���򿪡�Ĭ�ϣ�0���ر�
	int isFasttransferMode;
	int nPollRate;	//��ѯ����
	int nCodePage;
}UTHIDKBW;


//ͨѶ�ڲ������ýṹ
typedef struct zspCOMMCFG{
	unsigned int nCommType;		//����
	unsigned int nToModify;		//ʹ��COMM_MDF_XXXXX �Ķ���ֵ,��ʾ��ͬ�Ķ���
	void * lpsDetail;	//��������Ľṹ��
}SCOMMCFG;

typedef struct _NCR_TRICKLE{
	int nCount;			//�м������������
	int nTargetAddr[10];	//ÿ���������ʼ��ַ
	int nSrcAddr[10];	//ÿ����������Դ��ַ
	int nSrcLength[10];	//ÿ���������ݳ���
	int crc32;
	int nOffset;		//host�˷��͹̼����ݵ���ʼƫ����
	int length;			//�̼����ݵĳ���
	BYTE *pFWbuff;
	int flashState;		//flash��״̬��0 = δд�룬 1 = ����д�룬 2 = д�����
}NCRTrickleST;

/************************************************************************/
/* ȫ�ֱ�־��                                                           */
/************************************************************************/

/************************************************************************/
/* �궨��                                                               */
/************************************************************************/
//�й�LED��ɫ�Ķ���
#define LED_RED		0xff0000	//��ɫLED
#define LED_GREEN	0x00ff00	//��ɫLED
#define LED_BLUE	0x0000ff	//��ɫLED
//CMOS����ֵ
#define T_CMOS_UNKNOW		0x00		//Unknow cmos type
#define T_CMOS_OV7141		0x01
#define T_CMOS_OV9650		0x02		
#define T_CMOS_MT9V022		0x03
#define T_CMOS_OV7221		0x05		// 640 * 480
#define T_CMOS_OV9210		0x06
#define T_CMOS_AR0134		0x07
#define T_CMOS_BF3005		0x08
#define T_CMOS_GC0308		0x09
#define T_CMOS_OV9282       0x0A


//�й�ͨѶ�ӿڵ����͵Ķ���
#define COMMT_RS232			1		//RS232�Ĵ���
#define COMMT_USB_Data		2		//USB������ͨ���ӿ�
#define COMMT_KBW			3		//HID-KBWģ��������
#define COMMT_USB_RS232		4		//USBģ��RS232�ӿ�
#define COMMT_BLUE_TOOTH	5		//BlueTooth�ӿ�
#define COMMT_PS2	        6		//PS2�ӿ�
#define COMMT_HID_POS		7		//HID-POS
#define COMMT_IBM_SurePOS_TT	8		//IBM SurePOS(Table-Top) 
#define COMMT_IBM_SurePOS_HH	9		//IBM SurePOS(Hand-Held)
#define COMMT_USB_CDC		10		//USB CDC�ӿ�


//�й�ͨѶ�ӿ����õĵ�������
#define COMM_MDF_ENABLE		0x01	//ʹ��
#define COMM_MDF_DISABLE	0x02	//�ر�
#define COMM_MDF_CHGPARAM	0x04	//�ı����

#define	CMOSGetUsedPic	0
#define CMOSGetNewPic	1
/************************************************************************/
/*  ����ʵ��                                                            */
/************************************************************************/

#ifdef __cplusplus
extern"C"{
#endif

//////////////////////////////////////////////////////////////////////////
//����/��ʼ��uIMG��Ӳ�����ܽӿ�
void Z_InitHardIntface(osINTFACE * psParam);
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//ϵͳӲ�����ܽӿ�
//ϵͳ��������
void Z_sysReset(void);

//ϵͳ���ù���ֱ���˳��� ���ط�0��ʾ���óɹ������˳�����״̬
//ϵͳ����
int Z_sysIdle(void);

int Z_sysStandby(void);

//0.ϵͳ���߹���ֱ���˳��� ���ط�0��ʾ���߳ɹ������˳�����״̬
//ϵͳ����
int Z_sysSleep(void);
//����������
void Z_Beep( int freq, int mdelay, int volume );
//����������
void Z_PlayMusic(char * pcMusic);
void Z_PlayMusic_Ex(char * pcMusic, int volume);
//΢�뼶�ӳ�
void Z_usDelay(int us);
//���뼶�ӳ�
void Z_msDelay(int ms);
//��ȡϵͳ������
int  Z_CounterGet(void); 
//��ȡϵͳ���ľ���
int  Z_GetCounterDensity(void); 
//��ȡ10ms��λ��ϵͳ������
int  Z_Get10msCounter(void);	

unsigned int  Z_Get1msCounter(void);

//������
void  Z_Oscillate(int ms);
//ι���Ź�
void Z_DogFeed(void);
//��ȡ���Ź��ĸ�λʱ��
int  Z_DogGetBreak(void); 
//���ÿ��Ź��ĸ�λʱ��
int  Z_DogReset(int ms); 
//��ȡ��ǰ���µļ�ֵ��0��ʾû�а���
int  Z_KeyPressed(void);
//����ָ����ɫ��LED
void Z_LedOn(int clr);
//�ر�ָ����ɫ��LED
void Z_LedOff(int clr);
//�����ƿ�
void Z_LightOn(void);
//�����ƹ�
void Z_LightOff(void);
//���������ƿ�
void Z_IRLightOn(void);
//���������ƹ�
void Z_IRLightOff(void);
//�Խ��ƿ�
void Z_FocusOn(void);
//�Խ��ƹ�
void Z_FocusOff(void);
//�Ƿ�Խ��ƴ��ڿ���״̬
BOOL Z_IsFocusOn(void);
//�Ƿ������ƴ��ڿ���״̬
BOOL Z_IsLightOn(void);
//�Ƿ���������ƴ��ڿ���״̬
BOOL Z_IsIRLightOn(void);

void Z_SetLightOnDuration(int nLine);

void Z_ExtLightCtrl(BOOL bOn);

/*
	void Z_SetCmosLightMode(int mode);
	nMode:	bit[7:0]	illumination mode �ڲ�������ģʽ
						0 = normal
						1 = always on
						2 = always off
			bit[15:8]	aim mode �Խ���ģʽ
						0 = normal
						1 = always on
						2 = always off
						3 = �Զ�����ģʽ�£��Խ�������Ϊsensor mode��ʱ����������Ʋ�Ϊalways onģʽ���Խ��ƿ�����Ϊ�������ĸ���������
			bit[23:16]	external illumination mode �ⲿ������ģʽ
						0 = ����ģʽ(���ڲ�������ͬʱ������)
						1 = always on
						2 = always off
						3 = ����ģʽ(�������ڲ������Ƶ�������������õ�ģʽ��������)
 */
void Z_SetCmosLightMode(int mode);
int Z_GetCmosLightMode(void);
void Z_SetLightMode(int mode);


//10.CMOS����
//��ȡCMOS����
int  Z_cmos_GetType(void); 
//��ȡCMOS�Ĵ�����ֵ
int  Z_cmos_GetI2C(int reg);	
//����CMOS�Ĵ���Ϊָ��ֵ
int  Z_cmos_SetI2C(int reg, int value);
//�ɼ�һ��ͼ��,��ַΪ4�ֽڶ���
int*  Z_cmos_Read(int mode); 
//��ȡͼ���ַ
int*  Z_cmos_GetIMGbuff(void);

//11.����������洢
 //���ܲ�����ȡ ,ÿ1��SectOffs���4096�ֽ�. �ܹ�ֻ��12��Sector������ƫ���� 0 ~ 11
//��ȡָ������Ϣ
int  Z_param_Load(unsigned char * buff, int nSectOffs, unsigned int size);
//�洢��Ϣ��ָ����
int  Z_param_Save(unsigned char * buff, int nSectOffs, unsigned int size);

//12.ͨѶ�ڲ���
//��ȡ��ǰͨѶ�ӿ�����
int  Z_comm_GetType(void); 
//��ͨѶ���ж�����
int  Z_comm_Read(unsigned char * buff, unsigned int size);
//��ͨѶ����д����
int  Z_comm_Write(unsigned char * buff, unsigned int size);
//�ı�ͨѶ�ӿڵ����ã��������ͻ�������
int  Z_comm_Config(void * lpsParam); 
//��ȡͨѶ�ӿڵ����ò���
int  Z_comm_GetStatue(void * lpsParam);

//13.��ȡ������֮��
 //��ȡUID
int  Z_oem_GetUid(unsigned char * buff, unsigned int maxsize);
 //��ȡ���֤
int  Z_oem_GetLicense(unsigned char * buff, unsigned int maxsize);
 //�������֤
int  Z_oem_SetLicense(unsigned char * buff, unsigned int size);
 //�������֤
int  Z_oem_LicenseLock(void);

//14.��ȡ����ʱ�������
int	 Z_GetIdleCounter(void);
//����,ͬʱ��
void Z_OscillateWithBeep( int freq, int mdelay, int volume );
//��ȡӲ���汾��Ϣ
int	Z_GetHardwareInfo(char *pBuff, int nBuffSize);

int	Z_GetHardwareInfo_NCR(char *pBuff, int nBuffSize);

//ϵͳ�������
void Z_DeepSleep(void);
//ϵͳ�ػ�
void Z_PowerOff(void);
//���������ƿ�
void Z_SlvLightOn(void);
//���������ƹ�
void Z_SlvLightOff(void);
//���û��ѵ���ʱ��,����ɼ�ʱʱ,���������߻���;��δ��������״̬,�򲻲�������Ч��.
void Z_WakeupTimer(unsigned int ms);
int Z_SetDeviceSerial(char *serialNum, int len);  //��Ʒ���к�����
BOOL Z_SetDeviceManufacturer(char *str, int len); //��Ʒ����������

void Z_SetCMOSCollect(int mode);	//cmos �����ɼ�
int Z_GetCMOSCollect(void);		//��ѯcmos�ɼ��Ƿ����


int Z_GetCMOSDMANum(void);//��ȡ����ɲɼ���DMA�������
void Z_SetCMOSDMAInuseNum(int num);//����Ҫ������DMA�������
int* Z_cmos_GetIMGbuff_Ex(void);//30FPSģʽ�»�ȡ�����������ڽ����ͼ���ַ

char* Z_Get_ISRAM_Addr(void);//��ȡCPU�ڲ�SRAM��ַ���洢�ռ��С128kBytes
void Z_usbResumeHandle(void *p);

void Z_SetPowerLedOn(BOOL b);
void Z_CmosCapture(BOOL b, int mode);
BYTE* Z_GetCmosBuf(unsigned int mode, unsigned int timeout);
BYTE* Z_GetCmosBufTest(int n);

int Z_GetCmosWidth(void);
int Z_GetCmosHeight(void);

void Z_SetCmosMirror(int nMode);	//nMode: 0 - Normal; 1 - horizontal mirror; 2 - vertical flip; 3 - horizontal mirror & vertical flip.

unsigned int Z_GetCRC32(unsigned char *pdata, int len);

/**************************
�̼�����
	port:   0 = RS232
			1 = Datapipe
			2 = USB COM
 **************************/
void Z_FirmwareUpgrade(int port);


/*****************************************
��ȡ�ں���Ϣ
	type:	0  = Kernel version
			1  = Device name
*****************************************/
int	Z_GetKernelInfo(int type, char *pBuff, int nBuffSize);


#define FREQ_RUN_HIGH		0
#define FREQ_RUN_LOW		1


//����Ƶ�ʣ��Ƿ�ʹ�ܱ����ѹ
void Z_SetPM(int nFreqMode, BOOL bVoltage);
void Z_SetPM_EX(int nFreqMode);
int Z_GetPM(void);
void Z_TransferIdleTickVar(int *pTick);
//���óɵͺ���Ƶ������
//void Z_LowCoreFreq(void);
//���óɸߺ���Ƶ������
//void Z_HighCoreFreq(void);
void Z_GetCPUID(unsigned char *pcpuID);

unsigned int Z_EncryptionWrite(unsigned char *buff, unsigned int nsize, unsigned int flag);
unsigned int Z_EncryptionRead(unsigned char *buff, unsigned int nsize, unsigned int flag);

void Z_KBWSetCharEncoding(int nMode);
void Z_isSetKBWrestore(int nFormat);
void Z_isDecoding(int nFlag);
//////////////////////////////////////////////////////////////////////////
//�����û��Ļ��������������
void Z_SetUserFunc(UTGeneralParam *psParam);

//////////////////////////////////////////////////////////////////////////
//
#ifdef __cplusplus
}
#endif

#endif //__UIMG_DEVELOP_INTERFACE_OSIN_H__

