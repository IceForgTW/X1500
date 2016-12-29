 /*******************************************************************************************
 *                                                                                         *
 *                                                                                         *
 *       Copy Right 2001-2002,LandStanding Co.,Ltd.NewLand Group. All Right Reserved.      *
 *                                                                                         *
 *                                                                                         *
 *******************************************************************************************/
/*******************************************************************************************
 *
 *	FILE:
 *		interface.c
 *
 *	INTRODUCTION:
 *		
 *
 *	Logs:
 *
 *	BUGREPORT:
 *
 ******************************************************************************************/
 
/******************************************************************************************
*	include files 
*******************************************************************************************/
#include <list.h>
#include "download/sys_dnld.h"
#include <generate.h>
#include <usb.h>
#include <compat.h>
#include <cmos.h>
#include <os_api.h>
#include "hid_sendkey.h"
#include <usb.h>
#include <time-tcu.h> 
#include <watchdog.h>
#include <uartmc.h>
#include <string.h>
#include <nor_flash.h>
#include "SendDataRoute.h"

#undef printf
#include <stdio.h>
//////////////////////////////////////////////////////////////////////////

//#define DEBUG_INTF

#ifdef DEBUG_INTF
#define entry_fun()		UartSendStr(__func__ "()\r\n")
#define level_fun()		UartSendStr(__func__ "() level\r\n")
#else
#define entry_fun()		
#define level_fun()		
#endif
extern void Idle(int mode);
extern void Standby(void);
extern void DeepSleep(void);
extern void USBSuspend(void);
extern void _is_Set_KBW_restore(int nformat);
extern void Set_KBW_Character_Encoding(int nMode);
extern void _is_Decoding(int nFlag);
extern unsigned short GetDevMagic(void);
extern int efuse_read_chipID(unsigned int *buf);
//static UTKeyBoard kbwpram;
static int commtype = COMMT_RS232;
int *pIdleTick;
/********************************************************************************************
*	local referenced extern function and variables 
********************************************************************************************/


/*********************************************************************************************
*	Implementation functions 
*********************************************************************************************/

//1.����
//����: ��ָ����Ƶ��/����ʱ��/���������, ��λ��(Hz,ms,��),��ȼ�Ϊ (1,10);
static void _s_beep(int freq, int mdelay, int volume)
{
	entry_fun();
	beep_frq(freq,mdelay,volume);
	level_fun();
}


static void _s_playmusic(char * pcMusic)
{
	entry_fun();
	playmusic(pcMusic);
	level_fun();
}

static void _s_playmusic_Ex(char * pcMusic, int volume)
{
	entry_fun();
	playmusic_ex(pcMusic, volume);
	level_fun();
}

//2.ʱ��
static void _s_udelay(int us)
{
	entry_fun();
	udelay(us);
	level_fun();
}

static void _s_mdelay(int ms)
{
    unsigned int i;
	entry_fun();
	
    for(i=0; i<ms; i++)
        _s_udelay(1000);

	level_fun();
}

static int _s_gettick(void)
{
	entry_fun();
	level_fun();
	return (int)os_TimeGet();
}

static unsigned int _s_getMstick(void)
{
	entry_fun();

	level_fun();
	return (unsigned int)(Get_PerformanceCounter()/1500);
}

//3.����
static void  _s_shake(int ms)
{
	entry_fun();
	level_fun();
	return;
}

//5.����
//��ȡ����״̬,1��ʾ����״̬,��0��ʾ����״̬
static int _s_KbdGetKey(void)
{
	entry_fun();
	level_fun();
	return get_key_state();
	//return KbdGetKey(TIMEOUT_NOWAIT);
}

//6.LED����
static void _s_ledon(int clr)  //����ָ��LED����ɫ
{
	entry_fun();
	if(clr == LED_RED)			led_on(2);
	else if(clr == LED_GREEN)	led_on(1);
	else if(clr == LED_BLUE)	led_on(4);
	level_fun();
}

static void _s_ledoff(int clr)
{
	entry_fun();
	if(clr == LED_RED)			led_off(2);
	else if(clr == LED_GREEN)	led_off(1);
	else if(clr == LED_BLUE)	led_off(4);
	level_fun();
}

//7.��ͨ������
static void _s_lighton(void)
{
	entry_fun();
	level_fun();
}

static void _s_lightoff(void)
{
	entry_fun();
	level_fun();
}

//8.����������
// typedef void (*UIRLIGHTON)(void);
// typedef void (*UIRLIGHTOFF)(void);

//9.�Խ���
static void _s_focuson(void)
{
	entry_fun();
	level_fun();
}

static void _s_focusoff(void)
{
	entry_fun();
	level_fun();
}

//10.CMOS����
static int _s_cmos_gettype(void)
{
	int lvi = 0;
//	char lvstr[64];
	entry_fun();
 	lvi = Get_Cmos_type();
	level_fun();
	return lvi;
}

static int _s_cmos_geti2c(int reg)
{
	int lvi;
	entry_fun();
	lvi = CMosREGRead(reg);
	level_fun();
	return lvi;
}

static int _s_cmos_seti2c(int reg, int val)
{
	entry_fun();
	CMosREGSet(reg, val);
	level_fun();
	return 1;
}

static int* _s_cmos_read(int mode)
{
	entry_fun();
	level_fun();
	return (int*)ReadCmosBuf(CMOSGetNewPic, 150);
}

// static 
int* _s_getimgbuf(void)
{
	entry_fun();
	level_fun();
	return (int*)ReadCmosBuf(CMOSGetNewPic, 150);
}




//11.����������洢
//���ܲ�����ȡ ,ÿ1��SectOffs���4096�ֽ�. �ܹ�ֻ��12��Sector������ƫ���� 0 ~ 11
//����ֵ: ʵ�ʴ�ȡ�ĳ���

#define PARAM_ADDR	0x00040000
/*
sfcboot: 0x0000 0000        
loader:  0x0004 0000 
param:	 0x000a 0000
kernel:  0x0010 0000         
*/
int _s_param_load(unsigned char * buff, int nSectOffs, unsigned int size)
{
//	int n;
    if((nSectOffs<0) || (nSectOffs>11))
		return -1;
	
    if(size>4096)
		size = 4096;
	
	jz_nor_read(PARAM_ADDR+nSectOffs*4096, size, (unsigned int)buff);

// 	{
// 		int i;
// 		diag_printf("\nread: %d, %d\n", nSectOffs, size);
// 		for(i=0; i<size; i++)
// 		{
// 			diag_printf("%02x ", buff[i]);
// 		}
// 		diag_printf("\n");
// 	}
	return size;
}

int _s_param_save(unsigned char * buff, int nSectOffs, unsigned int size)
{
//    int stat = 0;
    unsigned char read_buff[4096];
    
    if((nSectOffs<0) || (nSectOffs>11))
        return -1;
    if(size>4096)
        size = 4096;
	
// 	{
// 		int i;
// 		diag_printf("\nwrite: %d, %d\n", nSectOffs, size);
// 		for(i=0; i<size; i++)
// 		{
// 			diag_printf("%02x ", buff[i]);
// 		}
// 		diag_printf("\n");
// 	}

    jz_nor_write(PARAM_ADDR+nSectOffs*4096, size, (unsigned int)buff);
	jz_nor_read(PARAM_ADDR+nSectOffs*4096, size, (unsigned int)read_buff);
		
    if (0 != memcmp(buff,read_buff,size))
		return -1;
  
    return size;
}


//12.ͨѶ�ڲ���

//����: ��ȡͨѶ������    ( Ĭ �� �� �� )
//����ֵ: ���궨��
static int _s_comm_gettype(void)
{
	entry_fun();
	level_fun();
	return commtype;
}

//����: ��ͨѶ���ж���ָ����С���ڵ�����
//����ֵ: ʵ�ʶ������ֽ���
// static 
int _s_comm_read(unsigned char * buff, unsigned int size)
{
	int count = 0;
	int timeout = TIMEOUT_NOWAIT;

	entry_fun();
	if (!commtype){
		return 0;
	}

	switch(commtype)
	{
		case COMMT_RS232:		//RS232�Ĵ���
			count = comm_read(buff,size,timeout);
			break;
		case COMMT_USB_Data:		//USB������ͨ���ӿ�
		case COMMT_USB_RS232:
			count = udp_read((char*)buff,size,timeout);	
			//if(count) printf("len: %d\n%s\n", count, buff);
			break;
 		case COMMT_KBW:		//�������
 			break;
 		case COMMT_HID_POS:		//usb hid
 			count = pos_read((char*)buff, size, timeout);
 			break;
 		case COMMT_IBM_SurePOS_TT:
 		case COMMT_IBM_SurePOS_HH:
 			count = surepos_read((char*)buff, size, timeout);
 			break;
 		case COMMT_USB_CDC:
 			count = cdc_read((char*)buff,size,timeout);
 			break;
		default:
			break;
	}
	level_fun();

	return count;
}

int AddDataToQueue(void *pData, int nLen, int Reserve);

//����: ͨ��ͨѶ�ڷ�������
//����ֵ: ʵ�ʷ��͵�������
static int _s_comm_write(unsigned char * buff, unsigned int size)
{
	int count = 0;

	entry_fun();
	if (!commtype)
		return 0;
	switch(commtype)
	{
		case COMMT_RS232:		//RS232�Ĵ���
			count = comm_write(buff,size);
			break;
		case COMMT_USB_Data:		//USB������ͨ���ӿ�
		case COMMT_USB_RS232:
			count = udp_write(buff,size);
			break;
 		case COMMT_KBW:		//�������
 			AddDataToQueue(buff, size, SDR_PORT_USB_KBW);
 			count = size;
 			/*if(!send_key_str((char*)buff, size))
 				count = size;
 			else 
                count = size;*/
 			break;
 		case COMMT_HID_POS:
 			count = pos_write(buff, size);
 			break;
 		case COMMT_IBM_SurePOS_TT:
 		case COMMT_IBM_SurePOS_HH:
  			count = surepos_write(buff, size);
 			break;
 		case COMMT_USB_CDC:
 			count = cdc_write(buff, size);
 			break;
            case COMMT_PS2:
			//count = PS2_Port_SendData((char*)buff, size);
			AddDataToQueue(buff, size, SDR_PORT_PS2_KBW);
			count = size;
			break;

		default:
			break;
	}
	level_fun();

	//diag_printf("write ok: %d\n",count);

	return count;
}

static int _s_usb_fun = 0;

//����: �ı�ͨѶ�ڵ�����(����,��ʽ��)
//����ֵ: 1��ʾ�ɹ�,0��ʾʧ��,-1��ʾ�޴�ͨ�ſ�
static int _s_comm_config(void * lpsParam)
{
	SCOMMCFG *param;
	int lvi = 0;
	TComParam *scom;
	entry_fun();
	param = (SCOMMCFG*)lpsParam;

 	//printf("comm: %d %d\r\n", param->nCommType, commtype);

	switch(param->nCommType)
	{	
		case COMMT_RS232:		//RS232�Ĵ���
			scom = (TComParam*)(param->lpsDetail);
			if (param->nToModify & COMM_MDF_DISABLE)
			{

			}
			else 
			{
				if(!scom){
					lvi = 1;
					break;
				}
				if (!param->lpsDetail)
					return 0;

				if (param->nToModify & COMM_MDF_ENABLE)
				{
//					UartSendStr("open rs232\r\n");
					commtype = COMMT_RS232;
				}
				if (param->nToModify & COMM_MDF_CHGPARAM)
				{
					uart_set_parameter(scom);
				}
			}
			lvi = 1;
			break;

		case COMMT_USB_Data:		//USB������ͨ���ӿ�
			if (param->nToModify & COMM_MDF_DISABLE)
			{
				//printf("close udp\r\n");
				udp_close();
				os_TimeDelay(10);
				//commtype = 0;
			}
			else if (param->nToModify & COMM_MDF_ENABLE)
			{
				if(param->nCommType != _s_usb_fun){
					if(_s_usb_fun == COMMT_USB_RS232)
						udp_close();
 					else if(_s_usb_fun == COMMT_KBW)
 						kbw_close();
 					else if(_s_usb_fun == COMMT_HID_POS)
 						pos_close();
 					else if((_s_usb_fun == COMMT_IBM_SurePOS_HH) || (_s_usb_fun == COMMT_IBM_SurePOS_TT))
 						surepos_close();
 					else if(_s_usb_fun == COMMT_USB_CDC)
						cdc_close();

					if(_s_usb_fun)	os_TimeDelay(5);
					_s_usb_fun = param->nCommType;
					//printf("change udp\r\n");
				}
				//printf("try open udp\r\n");
				if(udp_open(UDP_OPEN_MODE_NOR) < 0){
					//printf("open udp ser error!\r\n");
					return 0;
				}
				//USleep(1500);
				if(!udp_active()){
					//printf("udp unlink\r\n");
					return 0;
				}
				commtype = COMMT_USB_Data;
				//printf("udp ok\r\n");
			}
			lvi = 1;
			
			break;
		case COMMT_USB_RS232:
			if (param->nToModify & COMM_MDF_DISABLE)
			{
				//printf("close udp to ser\r\n");
				udp_close();
				os_TimeDelay(10);
				//commtype = 0;
			}
			else if (param->nToModify & COMM_MDF_ENABLE)
			{
				if(param->nCommType != _s_usb_fun){
					if(_s_usb_fun == COMMT_USB_Data)
						udp_close();
 					else if(_s_usb_fun == COMMT_KBW)
 						kbw_close();
 					else if(_s_usb_fun == COMMT_HID_POS)
 						pos_close();
 					else if((_s_usb_fun == COMMT_IBM_SurePOS_HH) || (_s_usb_fun == COMMT_IBM_SurePOS_TT))
 						surepos_close();
 					else if(_s_usb_fun == COMMT_USB_CDC)
 						cdc_close();

					if(_s_usb_fun)	os_TimeDelay(5);
					_s_usb_fun = param->nCommType;
					//printf("change to udp ser\r\n");
				}
				//printf("try open udp ser\r\n");
				if(udp_open(UDP_OPEN_MODE_SER) < 0){
					//printf("open udp ser error!\r\n");
					return 0;
				}
				//USleep(1500);
				if(!udp_active()){
					//printf("usbser unlink\r\n");
					return 0;
				}
				commtype = COMMT_USB_RS232;
 				//printf("usbser ok\r\n");
			}
			lvi = 1;
			break;

 		case COMMT_KBW:		//�������
 			if(param->nToModify & COMM_MDF_DISABLE){
 				kbw_close();
 				os_TimeDelay(10);
 				//commtype = 0;
 				lvi = 1;
 				break;
 			}
 			if(param->nToModify & COMM_MDF_ENABLE){
 				if(param->nCommType != _s_usb_fun){
 					if(_s_usb_fun == COMMT_USB_RS232 || _s_usb_fun == COMMT_USB_Data)
 						udp_close();
 					else if(_s_usb_fun == COMMT_HID_POS)
 						pos_close();
 					else if((_s_usb_fun == COMMT_IBM_SurePOS_HH) || (_s_usb_fun == COMMT_IBM_SurePOS_TT))
 						surepos_close();
 					else if(_s_usb_fun == COMMT_USB_CDC)
 						cdc_close();
 					if(_s_usb_fun)	os_TimeDelay(5);
 					_s_usb_fun = param->nCommType;
 					//printf("Change to kbw\r\n");
 				}
 				if(kbw_open() < 0){
                    //printf("open kbw error!\r\n");
                    return 0;
 				}

 			}
 			if(param->nToModify & COMM_MDF_CHGPARAM){
 				UTHIDKBW *kbwpram = (UTHIDKBW*)(param->lpsDetail);
 				keyboard_country_set(kbwpram->nLanguage);
 				keyboard_style_set(_CAPS_LOCK, kbwpram->isCapsLock);
 				keyboard_style_set(_NUMERIC_KEYPAD_MODE, kbwpram->isNumericMode);
 				keyboard_style_set(_CONTROL_ASCII_MODE, kbwpram->isControlASCIIMode);
 				keyboard_style_set(_KEYSTROKE_DELAY, kbwpram->isKeysrockeDelay);
 				keyboard_style_set(_CONVERT_CASE, kbwpram->isConvertCase);
 				keyboard_style_set(_EMULATE_KEYPAD, kbwpram->isEmulateKeypad);
 				keyboard_style_set(_UNKNOWN_CHARACTERS_BEEP, kbwpram->isUnknownCharactersBeep);
 				keyboard_style_set(_KEYPAD_STATE, kbwpram->isKeyPad);
 				keyboard_style_set(_FAST_MODE, kbwpram->isFasttransferMode);
 				keyboard_style_set(_Poll_Rate, kbwpram->nPollRate);
 				keyboard_style_set(_Code_Page, kbwpram->nCodePage);
 			}
 //			printf("try kbd ");
 			if(!kbw_active()){
 			//	printf("kbd unlink\r\n");
 				return 0;
 			}
 			commtype = COMMT_KBW;
 			//printf("Kbd OK\r\n");
 			lvi = 1;
 			break;

            case COMMT_PS2:		//ps2
			if (param->nToModify & COMM_MDF_ENABLE)
			{
				commtype = COMMT_PS2;
				lvi = 1;
			}
			break;
 
 		case COMMT_HID_POS:
 			if (param->nToModify & COMM_MDF_DISABLE)
 			{
 				pos_close();
 				os_TimeDelay(10);
 				//commtype = 0;
 			}
 			else if (param->nToModify & COMM_MDF_ENABLE)
 			{
 				if(param->nCommType != _s_usb_fun){
 					if(_s_usb_fun == COMMT_USB_RS232 || _s_usb_fun == COMMT_USB_Data)
 						udp_close();
 					else if(_s_usb_fun == COMMT_KBW)
 						kbw_close();
 					else if((_s_usb_fun == COMMT_IBM_SurePOS_HH) || (_s_usb_fun == COMMT_IBM_SurePOS_TT))
 						surepos_close();
 					else if(_s_usb_fun == COMMT_USB_CDC)
 						cdc_close();
 					if(_s_usb_fun)	
                        os_TimeDelay(5);
 					_s_usb_fun = param->nCommType;
 				}
 				if(pos_open() < 0){
                    return 0;
 				}
 			}
// 			if (param->lpsDetail != NULL)
// 			{
//				int mode = *(int*)param->lpsDetail; 
// 				SetHidPosMode(mode);
// 			}
// 			else
// 				SetHidPosMode(0);
 			lvi = 1;
 			if(!pos_active()){
 				//UartSendStr("HIDPOS unlink\r\n");
 				return 0;
 			}
 
 			commtype = COMMT_HID_POS;
 
 			break;
 
 		case COMMT_IBM_SurePOS_TT:
 			if (param->nToModify & COMM_MDF_DISABLE)
 			{
 				surepos_close();
 				os_TimeDelay(10);
 				//commtype = 0;
 			}
 			else if (param->nToModify & COMM_MDF_ENABLE)
 			{
 				if(param->nCommType != _s_usb_fun){
 					if(_s_usb_fun == COMMT_USB_RS232 || _s_usb_fun == COMMT_USB_Data)
 						udp_close();
 					else if(_s_usb_fun == COMMT_KBW)
 						kbw_close();
 					else if(_s_usb_fun == COMMT_HID_POS)
 						pos_close();
 					else if(_s_usb_fun == COMMT_IBM_SurePOS_HH)
 						surepos_close();
 					else if(_s_usb_fun == COMMT_USB_CDC)
 						cdc_close();
 
 					if(_s_usb_fun)	
                        os_TimeDelay(5);
 					_s_usb_fun = param->nCommType;
 				}
 				if(surepos_open(IBM_SurePOS_TYPE_TableTop) < 0){
                    return 0;
 				}
 			}
 
 			lvi = 1;
 
 			if(!surepos_active()){
 				//UartSendStr("usbser unlink\r\n");
 				return 0;
 			}
 			
 			commtype = COMMT_IBM_SurePOS_TT;
 			
 			break;
 
 		case COMMT_IBM_SurePOS_HH:
 			if (param->nToModify & COMM_MDF_DISABLE)
 			{
 				surepos_close();
 				os_TimeDelay(10);
 				//commtype = 0;
 			}
 			else if (param->nToModify & COMM_MDF_ENABLE)
 			{
 				if(param->nCommType != _s_usb_fun){
 					if(_s_usb_fun == COMMT_USB_RS232 || _s_usb_fun == COMMT_USB_Data)
 						udp_close();
 					else if(_s_usb_fun == COMMT_KBW)
 						kbw_close();
 					else if(_s_usb_fun == COMMT_HID_POS)
 						pos_close();
 					else if(_s_usb_fun == COMMT_IBM_SurePOS_TT)
 						surepos_close();
 					else if(_s_usb_fun == COMMT_USB_CDC)
 						cdc_close();
 					
 					if(_s_usb_fun)	os_TimeDelay(5);
 					_s_usb_fun = param->nCommType;
 				}
 				if(surepos_open(IBM_SurePOS_TYPE_HandHeld) < 0){
                    return 0;
 				}
 			}
 			
 			lvi = 1;
 			
 			if(!surepos_active()){
 				//UartSendStr("usbser unlink\r\n");
 				return 0;
 			}
 			
 			commtype = COMMT_IBM_SurePOS_HH;
 			
 			break;
 
 		case COMMT_USB_CDC:
 			if (param->nToModify & COMM_MDF_DISABLE)
 			{
 				//printf("close cdc\r\n");
 				cdc_close();
				os_TimeDelay(10);
 				//commtype = 0;
 			}
 			else if (param->nToModify & COMM_MDF_ENABLE)
 			{
 				if(param->nCommType != _s_usb_fun){
 					if(_s_usb_fun == COMMT_USB_RS232 || _s_usb_fun == COMMT_USB_Data)
 						udp_close();
 					else if(_s_usb_fun == COMMT_KBW)
						kbw_close();
// 					else if((_s_usb_fun == COMMT_IBM_SurePOS_HH) || (_s_usb_fun == COMMT_IBM_SurePOS_TT))
// 						close_surePos();
 					else if(_s_usb_fun == COMMT_HID_POS)
 						pos_close();
 
					if(_s_usb_fun)	os_TimeDelay(5);
 					_s_usb_fun = param->nCommType;
 					//printf("change cdc\r\n");
 				}
 				
 				lvi = cdc_open();
 				if(lvi < 0){
 					//printf("cdc_open error: %d\r\n", lvi);
 					return 0;
 				}
 				
 				//USleep(1500);
 				if(!cdc_active()){
 					//printf("cdc unlink\r\n");
 					return 0;
 				}
 				commtype = COMMT_USB_CDC;
 				//printf("cdc ok\r\n");
 			}
 			lvi = 1;
 			
 			break;

		default:
			break;
	}
	level_fun();
	return lvi;
}

/*
static void ExecDbgCmd(char *strCmd)
{
//	UartSendStr(strCmd);
	if(!strcmp(strCmd, "dbg_level=10"))
	{
		Dbg(0, "Set Dbg level to 10\r\n");
		SetDbgLevel(10);
	}
	else if(!strcmp(strCmd, "commtype"))
	{
		Dbg(0, "commtype:%d\r\n", commtype);
	}
	else if(!strcmp(strCmd, "reset usb"))
	{
		DEV *lvdev = GetDeviceByName(DEVNAME_UDC);
		lvdev->dev_ops->dev_cntl(0, 0, 0);
		Dbg(0, "Reset system usb bus.\r\n");
	}
	else
	{
		Dbg(0, "Unknow command:[%s]\r\n", strCmd);
	}

}

static void DbgCtrlTermTask(void *pParam)
{
	int lvLedFlag = 0;
	int lvCnt = 0;
	int lvi = 0;
	char lvbuf[128];
	while(1)
	{
		if(lvLedFlag)			//flash led
		{
			led_off(3);
			lvLedFlag = 0;
		}
		else
		{
			led_on(3);
			lvLedFlag = 1;
		}
		if(commtype == COMMT_RS232)
		{
			USleep(100);
			continue;
		}
		while(1)
		{
			lvi = UartRead(COM1, lvbuf + lvCnt, 1, 100);
			if(lvi != 1)
				break;
			if(lvbuf[lvCnt] == '\n')
			{
				lvbuf[lvCnt] = 0;
				ExecDbgCmd(lvbuf);
				lvCnt = 0;
				break;
			}
			else if(lvbuf[lvCnt] < ' ')
			{
				continue;
			}
			else if(lvCnt < 120)
			{
				lvCnt ++;
			}
		}
	}
}

void StartDbtCtrlTermTask(void)
{
	ST_TASKCREATE lvsc;
	lvsc.pFunc = DbgCtrlTermTask;
	lvsc.pParam = NULL;
	lvsc.nStackSize = 1024 * 8;
	lvsc.nPrio = SYS_PRIO_TERMINAL;
	UCreateTask(&lvsc);
}
*/

//����: ��ȡͨѶ�ڵ����ò���
//����ֵ: 1��ʾ�ɹ�,0��ʾʧ��,-1��ʾ�޴�ͨ�ſ�
static int _s_comm_getstatue(void * lpsParam)
{
	SCOMMCFG *param;
	TComParam *scom;

	int lvi = 0;

	entry_fun();
	param = (SCOMMCFG*)lpsParam;
	
	switch(param->nCommType)
	{	
		case COMMT_RS232:		//RS232�Ĵ���
			if (!param->lpsDetail)
				return 0;
			scom = (TComParam*)(param->lpsDetail);
			uart_get_parameter(scom);	
			lvi = 1;
			break;
		case COMMT_USB_Data:		//USB������ͨ���ӿ�
			lvi = 0;
			break;
		case COMMT_USB_RS232:
			lvi = 0;
			break;
// //		case COMMT_USB_KBW:		//USBģ��������
// // 			memcpy((UTKeyBoard*)(param->lpsDetail),&kbwpram,sizeof(UTKeyBoard))
// // 			lvi = 1;
// //			lvi = -1;
// //			break;
// //		case COMMT_USB_RS232:		//USBģ�⴮��
// //			lvi = -1;
// //			break;
// 		case COMMT_KBW:		//�������
// 			lvi = 0;
// 			break;
// 		case COMMT_HID_POS:		//usb hid
// 			lvi = 0;
// 			break;
// 		case COMMT_IBM_SurePOS_TT:		
// 		case COMMT_IBM_SurePOS_HH:		
// 			lvi = 0;
// 			break;
 		case COMMT_USB_CDC:
 			lvi = 0;
 			break;
		default:
			break;
	}
	level_fun();
	return lvi;

}



//13.��ȡ������֮��

//����: ��ȡID
//����ֵ: ʵ�ʻ�ȡ�ĳ���
//max:8byte
static int _s_oem_getid(unsigned char * buff, unsigned int maxsize)
{
// 	unsigned char dat[8];
// 	entry_fun();
// 	if(maxsize>8)
// 		maxsize = 8;
// 	flash_get_id(dat);
// 	memcpy(buff,dat,maxsize);
// 	level_fun();
	return maxsize;
}


//����: ��ȡ���֤
//����ֵ: ʵ�ʻ�ȡ�ĳ���
//max:16byte
static int _s_oem_getlicense(unsigned char * buff, unsigned int maxsize)
{
//  	unsigned char data[16];
// 
// 	entry_fun();
// 	if(maxsize>16)
// 		maxsize = 16;
// 
// 	flash_get_uid(data);
// 
// 	memcpy(buff,data,maxsize);
// 	level_fun();
	return maxsize;
}


//����: �������֤
//����ֵ: 1Ϊ�ɹ�,0 Ϊʧ��
static int _s_oem_setlicense(unsigned char * buff, unsigned int size)
{
//	unsigned char data[16];
// 	unsigned char dat[16];
// 	int i;
// 
// 	entry_fun();
// 	if(size>16)
// 		size = 16;
// 	for (i=0; i<16; i++)
// 	{
// 		data[i] = 0xff;
// 	}
// 	memcpy(data,buff,size);
// 	flash_set_uid(data);
// 	flash_get_uid(dat);
// 	i = memcmp(buff,dat,size);
// 	level_fun();
// 	if(i != 0)
// 		return 0;

	return 1;
}



//����: �������֤,���ⱻ����
//����ֵ: 1Ϊ�ɹ�,0 Ϊʧ��.	
static int _s_oem_licenselock(void)
{
	entry_fun();
// 	flash_lock_uid();
	level_fun();
	return 1;
}

//��ȡ���ľ��ȣ���λ����
static int _s_gettick_pertime(void)
{
	entry_fun();
	level_fun();
	return 10;
}

static int _s_set_wd_time(int ms)
{
	entry_fun();
	if(ms <= 0){
		DisableWatchDog();
		return 0;
	}
	SetWatchDogResetTime(ms);
	EnableWatchDog();
	level_fun();
	return 0;
}

static void _s_reset_wd(void)
{
	entry_fun();
	ResetWatchDog();
	level_fun();
}

static int _s_get_wd_time(void)
{
	entry_fun();
	return GetWatchDogResetTime();
	return 0;
	level_fun();
}

static void _s_sys_reset(void)
{
	entry_fun();
	ResetSystemByWD();	
	level_fun();
}

static void	_s_OscillateWithBeep(int freq, int mdelay, int volume)
{
	entry_fun();
	level_fun();
	return;
}

//ϵͳ���ù���
//����: ��ϵͳ��������״̬,�����а����򴮿ڡ�usb��Ϣ���¼�ʱ�Զ�����
//����ֵ:  1  ��ʾ���óɹ�, 0 ��ʾʧ��
static int _s_sysIdle(void)
{
	entry_fun();
	Idle(0);
	level_fun();
	return 1;
}

static int _s_standby(void)
{
	entry_fun();
	Standby();
	level_fun();
	return 0;
}

//ϵͳ���ù���
//����: ��ϵͳ��������״̬,�����а����򴮿ڡ�usb��Ϣ���¼�ʱ���߶�ʱ���ѷ���ʱ�Զ�����
//����ֵ:  1  ��ʾ���óɹ�, 0 ��ʾʧ��
static int _s_syssleep(void)
{
	entry_fun();
	Idle(1);
	level_fun();
	return 1;
}

static void _s_wakeupTimer(int ms)
{
	entry_fun();
 	Set_Wakeup_timer(ms);
	level_fun();
}

static void _s_PowerOff(void)
{
	entry_fun();
	DeepSleep();
	level_fun();
}

void _s_TransferIdleTickVar(int *pTick)
{   
    entry_fun();
    pIdleTick = pTick;
    //printf("idleaddr=%x\n",pIdleTick);
    level_fun();
}
    

typedef void (*BackFromUsbResume)(void);
BackFromUsbResume usbResume = NULL;
static void UsbResumeHandle(void *p)
{
	if (p==NULL)
		return;
	usbResume = (BackFromUsbResume)p;
}

void UsbBackFromResume(void)
{
	if (usbResume == NULL)
		return;
	usbResume();
}


// extern const char __IMG_BUILD_TIME__[];
// extern const char __IMG_BUILD_DATE__[];

#define KERNEL_VER_STRING	"1.01.001TEST"


/*****************************************
 type:		0  = Kernel version
			1  = Device name
*****************************************/
static int _s_GetKernelInfo(int type, char *pBuff, int nBuffSize)
{
	char pb[200];
	int i = 0;
	if (type == 0)	// kernel version
	{
		i = sprintf(pb, "%s", KERNEL_VER_STRING);
		if (nBuffSize<=i)
		{
			i = nBuffSize;
			memcpy(pBuff, pb, i);
		}
		else
			strcpy(pBuff, pb);

		return i;
	}
	else if (type == 1) // device name
	{
		i = sprintf(pb, "%s", get_bsp_string());
		if (nBuffSize<=i)
		{
			i = nBuffSize;
			memcpy(pBuff, pb, i);
		}
		else
			strcpy(pBuff, pb);
		return i;
	}
	else
		return i;

}

static int	_s_GetHardwareInfo(char *pBuff, int nBuffSize)
{
	char *lvstr = get_bsp_string();
	char lvmid[512];
	static char lvver[50] = {0};
	char Doftype[50] = {0};

	if(X_paramQueryKeyword("ManufactureID", lvmid)){
		//found manufacture ID
		lvstr = NULL;
	}
	
// 	switch (X_paramGetGlobalAllParam()->pHard->nDofType)
// 	{
// 	case 1://PRODUCTION_DOF_NEAR:
// 		sprintf(Doftype, "  DOF Type:   Near (HD)\r\n");
// 		break;
// 	case 2://PRODUCTION_DOF_MIDDLE:
// 		sprintf(Doftype, "  DOF Type:   Middle (SR)\r\n");
// 		break;
// 	case 3://PRODUCTION_DOF_FAR:
// 		sprintf(Doftype, "  DOF Type:   Far (ER)\r\n");
// 		break;
// 	default:
// 		sprintf(Doftype, "");
// 		break;
// 	}

	sprintf(pBuff, 
		"Firmware Ver: %s %s\r\n"
// 		"Build time:   %s %s\r\n"
		"  Device ID:  %s\r\n"
		"%s",
		KERNEL_VER_STRING,
		lvver,
// 		__IMG_BUILD_DATE__,
// 		__IMG_BUILD_TIME__,
		lvstr ? lvstr : lvmid,
		Doftype);
	return strlen(pBuff);

//	char *pInfo = VER_HARDWARE;
	
//	if (nBuffSize < sizeof(VER_HARDWARE)-1)
//	{ return 0; }
	
//	memcpy(pBuff,pInfo,sizeof(VER_HARDWARE)-1);
	
//	return (sizeof(VER_HARDWARE)-1);
}

static int	_s_GetHardwareInfo_NCR(char *pBuff, int nBuffSize)
{
	char lvsn[50] = {0};
	char Doftype[10] = {0};
	char buff[1024];
	int i, k;
	char *p;

	char *lvstr = get_bsp_string();
	char lvmid[512];
	if(X_paramQueryKeyword("ManufactureID", lvmid)){
		//found manufacture ID
		lvstr = NULL;
	}

	if(!X_paramQueryKeyword("ESN", lvsn)){
		X_paramQueryKeyword("S/N", lvsn);
	}
	
// 	switch (X_paramGetGlobalAllParam()->pHard->nDofType)
// 	{
// 	case 1://PRODUCTION_DOF_NEAR:
// 		sprintf(Doftype, "-RN");
// 		break;
// 	case 2://PRODUCTION_DOF_MIDDLE:
// 		sprintf(Doftype, "-RM");
// 		break;
// 	case 3://PRODUCTION_DOF_FAR:
// 		sprintf(Doftype, "-RF");
// 		break;
// 	default:
// 		sprintf(Doftype, "");
// 		break;		
// 	}
	
// 	i = sprintf(buff, "MODEL=EM2037%s;SN=%s;FW=%s;",
// 		Doftype, lvsn, KERNEL_VER_STRING);

	p = strstr(pBuff, ":");
	if (p)
	{
		strcpy(buff, &p[9]);
		k = strlen(pBuff) - (&p[9]-pBuff);
		for (i=0; i<k; i++)
		{
			if (buff[i] != 0x20)
			{
				sprintf(pBuff, "uIMG_ver#:%s", &buff[i]);
				break;
			}
		}

	}

	i = sprintf(buff, "Firmware#:%s\n%s\nSerial#:%s\nModel#:%s%s",
		KERNEL_VER_STRING,pBuff, lvsn, lvstr ? lvstr : lvmid, Doftype);
// 	i = sprintf(buff, "Firmware#:%s\n%s",
// 		KERNEL_VER_STRING,pBuff);

	if (i > nBuffSize)
		i = nBuffSize;
	memcpy(pBuff, buff, i);
	return i;
}


void _s_TimeroutHandle (int chn, unsigned int ms, void (*handler)(void))
{
	entry_fun();
	timer_delay(chn, ms*1000, handler);	
	level_fun();
}


unsigned short _s_GetDevMagic(void)
{
	entry_fun();
    level_fun();

    return GetDevMagic();
}

void _s_getCPUID(unsigned char *cID)
{
    unsigned char *p;
    unsigned char chipID[16];
    p = cID;
    efuse_read_chipID((unsigned int *)chipID);
    memcpy(p, chipID, 16);  
}

#define ENCRTION_TEXT_ADD   0x20000
#define PUBLIC_KEYS_ADD     0x21000
unsigned int  _s_EncryptionWrtie(unsigned char *buff, unsigned int nsize, unsigned int flag)//flag=0��ʾд��Կ��flag=1��ʾд����
{
    unsigned char read_buff[4096];
    
    if(nsize>4096)
        nsize = 4096;
        
    if(flag == 0)//д��Կ
    {
        jz_nor_write(PUBLIC_KEYS_ADD, nsize, (unsigned int)buff);
        jz_nor_read(PUBLIC_KEYS_ADD, nsize, (unsigned int)read_buff);
    }
    else
    {
        jz_nor_write(ENCRTION_TEXT_ADD, nsize, (unsigned int)buff);   
        jz_nor_read(ENCRTION_TEXT_ADD, nsize, (unsigned int)read_buff);
    }
    if(0 != memcmp(buff, read_buff, nsize))
        return -1;

    return nsize;
}    
	
int _s_EncryptionRead(unsigned char *buff, unsigned int nsize, unsigned int flag)
{
    if(nsize>4096)
        nsize = 4096;
      
    if(flag == 0)//����Կ
            jz_nor_read(PUBLIC_KEYS_ADD, nsize, (unsigned int)buff);
        else
            jz_nor_read(ENCRTION_TEXT_ADD, nsize, (unsigned int)buff);

    return nsize;       
}

void Firmware_download(int port)
{
	DOWNLOAD_PARAM lvdp;
 	if (port == 1)	//datapipe
 	{
 		lvdp.chk_open = 0;
 		lvdp.PortType = SYS_DN_PORT_UDP;
 		lvdp.SaveSpace = SYS_DN_SAVE_SPACE_AUTO;
 		lvdp.PortIdx = UDP_OPEN_MODE_NOR;
 		lvdp.port_init_param = NULL;
 	}
 	else if (port == 2)	//USB COM
 	{
 		lvdp.chk_open = 0;
 		lvdp.PortType = SYS_DN_PORT_UDP;
 		lvdp.SaveSpace = SYS_DN_SAVE_SPACE_AUTO;
 		lvdp.PortIdx = UDP_OPEN_MODE_SER;
 		lvdp.port_init_param = NULL;
 	}
 	else if (port == 3)	//USB CDC
 	{
 		lvdp.chk_open = 0;
 		lvdp.PortType = SYS_DN_PORT_CDC;
 		lvdp.SaveSpace = SYS_DN_SAVE_SPACE_AUTO;
 		lvdp.PortIdx = 0;
 		lvdp.port_init_param = NULL;
 	}
 	else			//RS232
	{
		TComParam param = {			
			9600,		//baudrate
			8,			//bits
			ONESTOPBIT,	//stop bits
			NOPARITY,	//parity
			1,
			0,
			0
		};

		lvdp.chk_open = 0;
		lvdp.PortType = SYS_DN_PORT_RS232;
		lvdp.SaveSpace = SYS_DN_SAVE_SPACE_AUTO;
		lvdp.PortIdx = 1;
		lvdp.port_init_param = &param;
	}
	ResetWatchDog();
    
 	kernel_download(&lvdp, 0, 0, 60000);
}


osINTFACE img_os_mi ={
	_s_beep,		//void	(*beep)( int freq, int mdelay, int volume ); ����: ��ָ����Ƶ��/����ʱ��/���������, ��λ��(Hz,ms,��),��ȼ�Ϊ (1,10);
	_s_playmusic,	//void	(*playmusic)(char * pcMusic);//�����ַ��������׽�����������
	_s_udelay,		//void	(*usleep)(int us);//΢�뼶�ȴ�
	_s_mdelay,		//void	(*delay)(int ms);//���뼶�ȴ�
	_s_gettick,		//int	(*counterget)(void); //��ȡϵͳ����
	_s_gettick_pertime,	//int	(*counterdensity)(void); //��ȡ���ľ��ȣ���λ����
	_s_shake,		//void	(*oscillate)(int ms); //��
	_s_reset_wd,	//void	(*dogfeed)(void);  //���Ź���λ
	_s_get_wd_time,	//int	(*doggetbreak)(void); //��ȡ���Ź���λʱ��
	_s_set_wd_time,	//int	(*dogreset)(int ms); //���Ź�����,����ΪС�ڵ���0��ֵ,���ʾ�رտ��Ź�
	_s_KbdGetKey,	//int	(*keypressed)(void); //��ⱻ���µļ�ֵ,0Ϊû�а���
	_s_ledon,		//void	(*ledon)(int clr); //����ָ����ɫ��Led
	_s_ledoff,		//void	(*ledoff)(int clr); //�ر�ָ����ɫ��Led
	_s_lighton,		//void	(*lighton)(void); //����������
	_s_lightoff,	//void	(*lightoff)(void); //�ر�������
	NULL,			//void	(*irlighton)(void); //��������������
	NULL,			//void	(*irlightoff)(void); //�رպ���������
	_s_focuson,		//void	(*focuson)(void); //��ʼ�Խ���
	_s_focusoff,	//void	(*focusoff)(void); //�رնԽ���
	_s_cmos_gettype,//int	(*cmos_gettype)(void); //��ȡCMOS����
	_s_cmos_geti2c,	//int	(*cmos_geti2c)(int reg); //��ȡCMOS �Ĵ���ֵ
	_s_cmos_seti2c,	//int	(*cmos_seti2c)(int reg, int value); //����CMOS�Ĵ���ֵ
	_s_cmos_read,	//int*	(*cmos_read)(int mode); //����һ��ͼ��
	_s_getimgbuf,	//int*	(*cmos_getimgbuff)(void); //��ȡͼ���ַ
	_s_param_load,	//int	(*param_load)(unsigned char * buff, int nSectOffs, unsigned int size); //��ָ����Ϣ�鵽����
	_s_param_save,	//int	(*param_save)(unsigned char * buff, int nSectOffs, unsigned int size); //����Ϣд��ָ����Ϣ��
	_s_comm_gettype,//int	(*comm_gettype)(void); //��ȡͨѶ������
	_s_comm_read,	//int	(*comm_read)(unsigned char * buff, unsigned int size);//����: ��ͨѶ���ж���ָ����С���ڵ����� ���ض������ֽ���
	_s_comm_write,	//int	(*comm_write)(unsigned char * buff, unsigned int size);//����: ͨ��ͨѶ�ڷ������� ����ʵ�ʷ��͵�������
	_s_comm_config,	//int	(*comm_config)(void * lpsParam);//����: �ı�ͨѶ�ڵ�����(����,��ʽ��) 1��ʾ�ɹ�,0��ʾʧ��
	_s_comm_getstatue,	//int	(*comm_getstatue)(void * lpsParam);//����: ��ȡͨѶ�ڵ����ò��� ���� 1Ϊ�ɹ�,0Ϊʧ��
	_s_oem_getid,	//int	(*oem_getuid)(unsigned char * buff, unsigned int maxsize); //��ȡUID
	_s_oem_getlicense,	//int	(*oem_getlicense)(unsigned char * buff, unsigned int maxsize); //��ȡ���֤
	_s_oem_setlicense,	//int	(*oem_setlicense)(unsigned char * buff, unsigned int size);//�������֤ ���� 1Ϊ�ɹ�,0 Ϊʧ��
	_s_oem_licenselock,	//int	(*oem_licenselock)(void);//�������֤,���ⱻ���� ���� 1Ϊ�ɹ�,0 Ϊʧ��.
	_s_syssleep,	//int	(*syssleep)(void);//��������,���Զ����� ���� 1Ϊ�ɹ�, 0Ϊʧ��
	_s_sys_reset,	//void	(*reset)(void);
	_s_OscillateWithBeep,	//void OscillateWithBeep(int freq, int mdelay, int volume)
	_s_GetHardwareInfo,		//int (*GetHardwareInfo)(char *pBuff, int nBuffSize); //��ȡӲ���汾��Ϣ 
	_s_PowerOff,					//�������
	_s_PowerOff,					//�ػ�
	NULL,					//���������ƿ��������Զ�ģʽ
	NULL,					//���������ƹأ������Զ�ģʽ
	_s_wakeupTimer,					//���ڻ��ѵĵ���ʱ��
	USB_SetDeviceSerial,		//���ò�Ʒ���к�
	NULL,//setCMOSCollect,			//����CMOS��ʼ�ɼ�
	NULL,//getCMOSCollect,			//��ȡCMOS�ɼ��Ƿ����
	NULL,//getCMOSDMANum,			//��ȡ����ɲɼ���DMA�������
	NULL,//setCMOSDMAInuseNum,		//����Ҫ������DMA�������
	NULL,//_s_getimgbuf_Ex,		//20FPSģʽ�»�ȡ�����������ڽ����ͼ���ַ
	_s_sysIdle,
	_s_standby,
	NULL,
	_s_playmusic_Ex,	//void	(*playmusic_ex)(char * pcMusic, int volume));//�����ַ��������׽�����������,��ȼ�Ϊ (1,10)

	NULL,//ExtLedctrl,		//�ⲿ�����ƿ���
	USB_SetDeviceManufacturer, 		//���ò�Ʒ������

	set_cmos_light_mode,	
	get_cmos_light_mode,

	_s_GetHardwareInfo_NCR,
	NULL,//SetPowerFrequency,	//����Ƶ��
	NULL,//set_light_on,
	Set_Mobile_mode,//setLightMode,
	_s_getMstick,
	UsbResumeHandle,
	_s_GetKernelInfo,
	Firmware_download,
	NULL,//SetPowerLEDOn,
	csi_capture,
	ReadCmosBuf,

	CMosWidth,
	CMosHeight,
	CMosMirror,
	Get_CRC32,
	ReadCmosBufTest,
	_s_TimeroutHandle,
	_s_GetDevMagic,
	_s_TransferIdleTickVar,
	_s_getCPUID,
	_s_EncryptionWrtie,
	_s_EncryptionRead,
	Set_KBW_Character_Encoding,
	_is_Set_KBW_restore,
	_is_Decoding,
};

osINTFACE* GetDecodeOSIntf(void)
{
	//StartDbtCtrlTermTask();
	return &img_os_mi;
}



