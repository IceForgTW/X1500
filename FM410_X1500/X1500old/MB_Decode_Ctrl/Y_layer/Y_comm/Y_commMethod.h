
#ifndef __UIMG_DEVELOP_INTERFACE_YCOMM_H__
#define __UIMG_DEVELOP_INTERFACE_YCOMM_H__

#include "../../list.h"
/************************************************************************/
/* ���Ͷ���                                                             */
/************************************************************************/
//���ڲ����ṹ
/************************************************************************/
/* ȫ�ֱ�־��                                                           */
/************************************************************************/

/************************************************************************/
/*  ����ʵ��                                                            */
/************************************************************************/

#ifdef __cplusplus
extern"C"{
#endif

//////////////////////////////////////////////////////////////////////////
// ͨѶ�ӿڷ�װ

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


//��ȡ��ǰ��ͨѶ�ӿ�����
int	 Y_commGetCurType(void);
//ʹ��ͨѶ�ӿڣ�ʹ֮�����շ�
BOOL Y_commEnable(void);
//�ж�ͨѶ�ӿ��Ƿ�ʹ��
BOOL Y_commIsEnable(void);
//�ر�ͨѶ�ӿڣ�ʹ֮�����շ�
BOOL Y_commDisable(void);
//��ͨѶ�ӿ��л���RS232��ʽ
BOOL Y_commChgToRS232(TComParam *psParam);
//��ͨѶ�ӿ��л���USB �� DataPipe��ʽ
BOOL Y_commChgToUSBDataPipe(void);
//��ͨѶ�ӿ��л���USB��RS232��ʽ
BOOL Y_commChgToUSBRS232();


BOOL Y_commChgToUSBCDC();

//��ͨѶ�ӿ��л��� KBW ��ʽ
BOOL Y_commChgToKBW(UTKeyBoard *psParam);
//��ͨѶ�ӿ��л���BlueTooth��ʽ
BOOL Y_commChgToBT(void);
//��ͨѶ�ӿ��л���PS2��ʽ
BOOL Y_commChgToPS2(UTKeyBoard *psParam);
//��ͨѶ�ӿ��л���HID-POS��ʽ
BOOL Y_commChgToHIDPOS(void);
//��ͨѶ�ӿ��л���IBM SurePOS��ʽ
BOOL Y_commChgToSurePOS_TT(void);
BOOL Y_commChgToSurePOS_HH(void);
// ��ȡ������
int Y_commGetBaudrate(void);
//��ȡKBW�ļ��̷ֲ����Ҵ���
int Y_commGetKBWCountry(void);
//��ͨѶ�ڣ���ָ����ʱʱ���ڶ�����Ϣ
int Y_commRead(unsigned char * buff, unsigned int size, int msTimeOut);
//��ͨѶ�з�����Ϣ
int Y_commWrite(unsigned char *buff, unsigned int size);
//�����ַ�����Ϣ
int Y_commSendString(const char *str);

//��ս��ջ������е�����
BOOL Y_commClean(void);
//����һ���ַ�
int Y_commSendChar(char cChar);
//����һ���ַ�
int Y_commReadChar(char *pcChar,int msTimeOut);
//��ȡ���ջ������е�������
int Y_commBuffCount(void);
//���ں�ָ̨���Ķ�����������
int Y_commBakRead(unsigned char * buff, unsigned int size, int msTimeOut);
//���ں�ָ̨���Ķ����������ַ�
int Y_commBakReadChar(char *pcChar,int msTimeOut);
//���ں�ָ̨���Ļ��������������
int Y_commBakBuffCount(void);
// ͨѶ�ӿڷ�װ
//////////////////////////////////////////////////////////////////////////
BOOL BlueT_set_slave_pin(char *pin, int nlen);//����slaveģʽ�µ�ƥ������
void BlueT_set_auth_status(BOOL nstat);	//�����Ƿ��������ƥ��
void BlueT_reset_cfg(void);							//��λ���е�����
BOOL BlueT_set_dev_name(char *devname, int nlen);	//�����豸���� 
//////////////////////////////////////////////////////////////////////////
//
#ifdef __cplusplus
}
#endif

#endif //__UIMG_DEVELOP_INTERFACE_YCOMM_H__

