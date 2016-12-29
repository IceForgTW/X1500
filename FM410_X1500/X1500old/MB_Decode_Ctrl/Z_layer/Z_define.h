
#ifndef __UIMG_MODULE_OSIN_DEFINE_H__
#define __UIMG_MODULE_OSIN_DEFINE_H__


#include "../list.h"


//�ӿ����Ͷ��弰˵��

//����: ���迴�Ź���λʱ��,�����¿�ʼ��ʱ
//����ֵ: 1��ʾ�ɹ�,0��ʾʧ��

//1����������洢
//���ܲ�����ȡ ,ÿ1��SectOffs���4096�ֽ�. �ܹ�ֻ��12��Sector������ƫ���� 0 ~ 11
//����ֵ: ʵ�ʴ�ȡ�ĳ���

//////////////////////////////////////////////////////////////////////////
//  OS �ṩ�Ľӿڹ��� ��Ͻṹ��
typedef struct tagINTFACE{
	void	(*beep)( int freq, int mdelay, int volume );//����: ��ָ����Ƶ��/����ʱ��/���������, ��λ��(Hz,ms,��),��ȼ�Ϊ (1,10);
	void	(*playmusic)(char * pcMusic);//�����ַ��������׽�����������
	void	(*usleep)(int us);//΢�뼶�ȴ�
	void	(*delay)(int ms);//���뼶�ȴ�
	int		(*counterget)(void); //��ȡϵͳ����
	int		(*counterdensity)(void); //��ȡ���ľ��ȣ���λ����
	void	(*oscillate)(int ms); //��
	void	(*dogfeed)(void);  //���Ź���λ
	int		(*doggetbreak)(void); //��ȡ���Ź���λʱ��
	int		(*dogreset)(int ms); //���Ź�����,����ΪС�ڵ���0��ֵ,���ʾ�رտ��Ź�

	int		(*keypressed)(void); //��ⱻ���µļ�ֵ,0Ϊû�а���
	void	(*ledon)(int clr); //����ָ����ɫ��Led
	void	(*ledoff)(int clr); //�ر�ָ����ɫ��Led
	void	(*lighton)(void); //����������
	void	(*lightoff)(void); //�ر�������
	void	(*irlighton)(void); //��������������
	void	(*irlightoff)(void); //�رպ���������
	void	(*focuson)(void); //��ʼ�Խ���
	void	(*focusoff)(void); //�رնԽ���
	int		(*cmos_gettype)(void); //��ȡCMOS����

	int		(*cmos_geti2c)(int reg); //��ȡCMOS �Ĵ���ֵ
	int		(*cmos_seti2c)(int reg, int value); //����CMOS�Ĵ���ֵ
	int*	(*cmos_read)(int mode); //����һ��ͼ��
	int*	(*cmos_getimgbuff)(void); //��ȡͼ���ַ
	int		(*param_load)(unsigned char * buff, int nSectOffs, unsigned int size); //��ָ����Ϣ�鵽����
	int		(*param_save)(unsigned char * buff, int nSectOffs, unsigned int size); //����Ϣд��ָ����Ϣ��
	int		(*comm_gettype)(void); //��ȡͨѶ������
	int		(*comm_read)(unsigned char * buff, unsigned int size);//����: ��ͨѶ���ж���ָ����С���ڵ����� ���ض������ֽ���
	int		(*comm_write)(unsigned char * buff, unsigned int size);//����: ͨ��ͨѶ�ڷ������� ����ʵ�ʷ��͵�������
	int		(*comm_config)(void * lpsParam);//����: �ı�ͨѶ�ڵ�����(����,��ʽ��) 1��ʾ�ɹ�,0��ʾʧ��

	int		(*comm_getstatue)(void * lpsParam);//����: ��ȡͨѶ�ڵ����ò��� ���� 1Ϊ�ɹ�,0Ϊʧ��
	int		(*oem_getuid)(unsigned char * buff, unsigned int maxsize); //��ȡUID
	int		(*oem_getlicense)(unsigned char * buff, unsigned int maxsize); //��ȡ���֤
	int		(*oem_setlicense)(unsigned char * buff, unsigned int size);//�������֤ ���� 1Ϊ�ɹ�,0 Ϊʧ��
	int		(*oem_licenselock)(void);//�������֤,���ⱻ���� ���� 1Ϊ�ɹ�,0 Ϊʧ��.
	int		(*syssleep)(void);//��������,���Զ����� ���� 1Ϊ�ɹ�, 0Ϊʧ��
	void	(*reset)(void);
	void	(*OscillateWithBeep)(int freq, int mdelay, int volume); //����,ͬʱ��
	int		(*GetHardwareInfo)(char *pBuff, int nBuffSize); //��ȡӲ���汾��Ϣ
	void	(*DeepSleep)(void);	//�������

	void	(*PowerOff)(void);	//�ػ�
	void	(*slvlighton)(void);//���������ƿ��������Զ�ģʽ
	void	(*slvlightoff)(void);//���������ƹأ������Զ�ģʽ
	void	(*wakeupTimer)(int ms); //���ڻ��ѵĵ���ʱ��
	int     (*SetDeviceSerial)(char *serialNum, int len); //��Ʒ���к�

	void	(*setCMOSCollect)(int mode);
	int		(*getCMOSCollect)(void);

	int		(*getCMOSDMANum)(void);	//��ȡ����ɲɼ���DMA�������
	void	(*setCMOSDMAInuseNum)(int num);//����Ҫ������DMA�������
	int*	(*cmos_getimgbuff_Ex)(void);//30FPSģʽ�»�ȡ�����������ڽ����ͼ���ַ

	int		(*sysIdle)(void);//��������,���Զ����� ���� 1Ϊ�ɹ�, 0Ϊʧ��
	int		(*sysStandby)(void);//����standby,���Զ����� ���� 1Ϊ�ɹ�, 0Ϊʧ��

	char*	(*ISRAMAddr)(void);//�����ڲ�SRAM��ַ��ʹ�ÿռ䲻����128kBytes
	void	(*playmusic_ex)(char * pcMusic, int volume);//�����ַ��������׽�����������,��ȼ�Ϊ (1,10)

	void	(*extLightCtrl)(BOOL bOn);//�ⲿ�����ƿ���
	int		(*SetDeviceManufacturer)(char *str, int len);	//��Ʒ������
	
	void	(*SetCmosLightMode)(int nMode);
	int		(*GetCmosLightMode)(void);
	
	int		(*GetHardwareInfo_NCR)(char *pBuff, int nBuffSize); //��ȡӲ���汾��ϢNCR��ʽ

	void	(*SetPowerFrequency)(int nFreqMode, BOOL bVoltage);	//����Ƶ��

	void	(*SetLightOnDuration)(int nLine);	//����ÿ���ع���������������ʱ�䣬��λ����

	void	(*SetLightMode)(int mode);	//�����ع�ģʽΪphone����normal

	unsigned int		(*MScounterget)(void); //��ȡms����
	void	(*UsbResumeHandle)(void *p);//usb resume�� ��������ָ�봫��
	int 	(*getKernelInfo)(int type, char *pBuff, int nBuffSize); //��ȡ�ں���Ϣ: type = 0 Ϊ�̼��汾�ţ� 1 Ϊ�豸�� 
	void	(*firmwareUpgrade)(int Port); //�̼����½ӿڣ�port = 0ΪRS232, 1 Ϊdatapipe�� 2ΪUSB COM

	void	(*setPowerLedOn)(BOOL b); //power led on ���ã� ����HR32����
	
	void	(*CmosCapture)(BOOL b, int mode);	// ����/ֹͣ�ɼ�ͼ��
	unsigned char* (*GetCmosBuf)(unsigned int mode, unsigned int timeout);//��ȡͼ��mode�� 0 Ϊ��ͼ�� 1 Ϊ��ͼ�� timeout���ȴ���ʱ

	int		(*GetCmosWidth)(void);	//
	int		(*GetCmosHeight)(void);	//
	void	(*SetCmosMirror)(int nMode);	//nMode: 0 - Normal; 1 - horizontal mirror; 2 - vertical flip
	unsigned int (*GetCRC32)(unsigned char *pdata, int len);
	unsigned char* (*GetCmosBufTest)(int n);//��ȡָ�������ͼ������
    void (*TimeoutHandle)(int chn, unsigned int us, void (*handler)(void));
    unsigned short (*GetDevMagic)(void);
    void (*TransferIdleTickVar)(int *pTick); 
    void (*GetCPUID)(unsigned char *cpuID);
    unsigned int (*EncryptionWrtie)(unsigned char *buff, unsigned int nsize, unsigned int flag);
     unsigned int (*EncryptionRead)(unsigned char *buff, unsigned int nsize, unsigned int flag);
    void	(*SetKBWCharacterEncoding)(int nMode);	//����usb kbw���ַ�����Ľ�����ʽ
	void    (* isSetKBWrestore)(int nFormat);//����Ƿ񾭹�dataformat�����
	void (*isDecoding)(int nFlag);
	void*	reserve[96];
   
}osINTFACE;

#endif //__UIMG_MODULE_OSIN_DEFINE_H__


