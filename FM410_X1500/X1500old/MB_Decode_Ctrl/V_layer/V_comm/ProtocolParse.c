
/******************************************************************************************
*	include files 
*******************************************************************************************/
#include "ProtocolParse.h"
//#include <stdio.h>
//#include <string.h>
#include "../../NLS_SCProtocol/NLSComm_Proc.h"

#include <string.h>
#include <stdio.h>
/********************************************************************************************
*	local referenced extern function and variables 
********************************************************************************************/
#define  COMMREAD_TIMEOUT		400	//读取字符的超时限制,单位ms

#define  SENSITIVE_CALC(x)   (2 + (4 - (x)) * 3)  //感应级别和最终值得关系
/*********************************************************************************************
*	global variables
*********************************************************************************************/

int m_nSend = 0;		//发送的报文长度
int m_nReceive = 0;		//收到的报文长度
char m_chSendBytes[512];
char m_chReceiveBytes[512];

int nfixCnt,___nfixSumLen,nFixType;//前后缀设置长度计数,记录上一次长度,设置类型
unsigned char g_pbyFix[10]; //备份前后缀信息


const unsigned char CodeIDTab1D[32] = {
	'X','X','j','j',//ZASETUP,SETUP128,CODE128,UCCEAN128
	'd','d','c','c',//EAN8,EAN13,UPCE,UPCA
	'e','e','e','v',//INTER2OF5,ITF14,ITF6,MATRIX25
	'X','b','b','a',//MODULE25,REGULAR39,FULLASCII39,CODABAR
	'X','i','X','X',//2OUTOF5,CODE93,CODEZ,
	'X','X','X','g',//, , ,ISSN,
	'B','I','f','n',//ISBN,INDUSTRIAL25,STANDARD25,PLESSEY
	'H','m','y','R'};//CODE11,MSI_PLESSEY,COMPOSITE,RSS


const unsigned char CodeIDTab2D[32] = {
    'r','s','z','u',//PDF417,QR,Aztec,DataMatrix
	'x','V','X','h',//MaxiCode,VeriCode,UN_VeriCode,HAN_SENSIBLE
	'x','X','R','X',//GM,User,MicroPDF417
	'X','X','X','X',
    'X','X','X','X',
	'X','X','X','X',
	'X','X','X','X',
	'X','X','X','X'};

const unsigned char CodeIDPostal[32] = {
	'P','M','x','L',//POSTNET,Intelligent Mail,Royal Mail,Planet
	'K','A','X','X',//KixCode,AusCode
	'X','X','X','X',
	'X','X','X','X',
    'X','X','X','X',
	'X','X','X','X',
	'X','X','X','X',
	'X','X','X','X'};

const unsigned char CodeIDTab1D_2[32] = {
	0,0,0,0,//ZASETUP,SETUP128,CODE128
	0,0,0,0,//UCCEAN128,EAN8,EAN13,UPCE,UPCA
	0,0,0,0,//INTER2OF5,ITF14,ITF6,MATRIX25
	0,0,0,0,//MODULE25,REGULAR39,FULLASCII39,CODABAR
	0,0,0,0,//2OUTOF5,CODE93,CODEZ,
	0,0,0,0,//
	0,0,0,0,//
	0,0,0,0};//CODE11,MSI_PLESSEY,COMPOSITE,RSS
		
const unsigned char CodeIDTab2D_2[32] = {
	0,0,0,0,//PDF417,QR,Aztec,DataMatrix
	0,0,0,0,//MaxiCode,VeriCode,UN_VeriCode,HAN_SENSIBLE	
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0};

const unsigned char CodeIDPostal_2[32] = {
	0,0,0,0,//POSTNET,Intelligent Mail,Royal Mail,Planet
	0,0,0,0,//KixCode,AusCode
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0};

// const unsigned int  conBaudRate[9] = {1200,2400,4800,9600,14400,19200,38400,57600,115200};

//图形设置临时参数
static int g_ImgGetMode = 0;	//图像获取方式: 0:直接获取缓存图像数据	
static int g_ImgType = 0;		//图像样式: 0:原始图像
static int g_ImgColor = 0;		//图像颜色: 0:灰度图
static int g_ImgReduceRow = 0;	//图像水平压缩: 0: "1:1"
static int g_ImgReduceColumn = 0;	//图像垂直压缩: 0: "1:1"
static int g_ImgCutStartRow = 0;		//裁剪图像起始行
static int g_ImgCutStartColumn = 0;		//裁剪图像起始列
static int g_ImgCutEndRow = 0;		//裁剪图像结束行
static int g_ImgCutEndColumn = 0;		//裁剪图像结束列


/************************************************************************/
/* 嵌入的功能函数                                                        */
/************************************************************************/
FNC_commFrameBeforeTranslateMes	V_commFrameBFTranslateMes = NULL;
FNC_commProcManualFrame			V_commProcManualFrame = NULL;
FNC_commAddVerionMsg			V_commAddVerMsg = NULL;
/********************************************************************************************
*	local referenced macro defines & type defines
********************************************************************************************/

BOOL __IsDigit(char byChar)
{
	if (byChar < '0' || byChar > '9') return FALSE;
	return TRUE;
}

BOOL V_RunQuickSetCmd(char *buff)
{
	char lvbuf[512];
	char *lvCmd = buff;
	int lvi;

	if(!buff)
		return FALSE;

	lvi = strlen(buff);
	if(lvi < 7)
		return FALSE;

	if(buff[lvi - 1] != ';')		//为数据补足';'符号
	{
		strncpy(lvbuf, buff, 508);
		lvCmd = lvbuf;
		lvi = strlen(lvbuf);
		lvbuf[lvi++] = ';';
		lvbuf[lvi] = 0;
	}

	if(!W_setcParseBatch((unsigned char*)lvCmd, lvi))		//设置或命令运行失败
	{
	//	Dbg(0, "***Run cmd[%s] error\r\n", lvCmd);
		return FALSE;
	}

	if (W_setcToNoSaveParam())
	{
		W_setcSetSaveFlag(FALSE);		
		W_setcSetNoSaveFlag(FALSE);		
	} 
	else if (W_setcToSaveParam())				//判断是否保存
	{
		X_paramSaveAsSysDefaultParam();		//保存设置
		W_setcSetSaveFlag(FALSE);
	}
	
	//Dbg(0, "...Run cmd[%s] Succeed.\r\n", lvCmd);

	return TRUE;
}

void vQuickSetCode(char* buff)
{
	char cmd[2] = {0x06,0x00};
	int i;
	int n;

	if (Y_commBakRead((BYTE*)buff, 7, COMMREAD_TIMEOUT) != 7) return ;

	i = 0;
	while (i < 7) {
		if (!__IsDigit(buff[i])) return;
		i ++;
	}
	n = 7;
	buff[7] = 0;
	n += Y_commBakRead((BYTE*)buff + 7, 100, COMMREAD_TIMEOUT);
	buff[n++] = ';';
	buff[n] = 0;

	if(!V_RunQuickSetCmd(buff))
		cmd[0] = 0x15;				//命令或参数出错

	Y_commWrite((BYTE*)cmd, 1);
}


void AddLRC(char str[], int *len)
{
	BYTE LRCByte;
	int nOffset, i;
	
	nOffset = *len;
	LRCByte = 0xFF;
	for(i=2; i<nOffset; i++){
		LRCByte ^= str[i];
	}
	str[(*len)++] = LRCByte;
	str[*len] = 0;
}



BOOL CheckMessage(void)
{
	int i;
	char c;
	
	if (m_nReceive <= 0)	return FALSE;
	if (m_chReceiveBytes[0] != 0x02)	return FALSE;

	c = m_chReceiveBytes[m_nReceive-1];
	i = m_nReceive-1;
	AddLRC((char*)m_chReceiveBytes, &i);
	if (c != m_chReceiveBytes[m_nReceive-1]){
		m_chReceiveBytes[m_nReceive-1] = c;
		return FALSE;
	}
	
	return TRUE;
}

void RebootDevice(void)
{
	Z_DogReset(-1);
	Z_sysReset();
}

void PackAndSendString(char *pcStr, int nLen)
{
	unsigned char LRC;
	int i;
	char lvb[256];

	LRC = 0xff;
	LRC ^= ((nLen&0xff00)>>8);
	LRC ^= ((nLen&0xff));
	for (i = 0; i < nLen; i++)
	{ LRC ^= pcStr[i]; }

// 	Y_commSendChar(STX);
// 	Y_commSendChar(ATTR);
// 	Y_commSendChar((char)((nLen&0xff00)>>8));
// 	Y_commSendChar(((char)nLen&0xff));
// 	Y_commWrite((unsigned char*)pcStr,(unsigned int)nLen);
// 	Y_commSendChar((char)LRC);
	lvb[0] = STX;
	lvb[1] = ATTR;
	lvb[2] = (char)((nLen&0xff00)>>8);
	lvb[3] = ((char)nLen&0xff);
	memcpy(&lvb[4], pcStr, nLen);
	lvb[4+nLen] = LRC;
	Y_commWrite((unsigned char*)lvb,(unsigned int)(nLen+5));

}//End of function;

BOOL ChkStringParam(void)
{
	char cmd;

	unsigned char pValue[128]={0};
	static int FFP_enable = FALSE; //出厂序列号更改使能
	//BOOL bTemp;
	cmd = 0x06;

	memset(pValue,0,128);

	if(strncmp((char *)(m_chReceiveBytes + 4), "~IDLE~", 6) == 0)
	{
		Y_commWrite((BYTE*)&cmd,1);
		U_appSetIdleMode();
		return TRUE;
	}
	if(strncmp((char *)(m_chReceiveBytes + 4), "~STANDBY~", 9) == 0)
	{
		Y_commWrite((BYTE*)&cmd,1);
		U_appSetStandbyMode();
		return TRUE;
	}
	//看门狗测试，如果看门狗启动了，则会产生系统复位重启，如果看门狗未启动，则系统陷入死循环。
	if(strncmp((char *)(m_chReceiveBytes + 4), "~WDTEST~", 8) == 0)
	{
		cmd = 0x6;
		Y_commWrite((BYTE*)&cmd, 1);
		while(1);
	}

	//如果系统发送 "~upDate~" 加上前后的校验码等信息，采用死循环方式促使系统重新启动
	//从而进行系统更新
	if(strncmp((char *)(m_chReceiveBytes + 4), "~upDate~", 8) == 0)
	{
		Y_commWrite((BYTE*)&cmd,1);
		RebootDevice();
		return TRUE;
	}

	if(strncmp((char *)(m_chReceiveBytes + 4), "~upGrade~", 9) == 0)
	{
		Y_commWrite((BYTE*)&cmd,1);
		
// 		Z_FocusOff();	//对焦灯关闭
// 		Z_LightOff();	//照明灯关闭
// 		Y_cmosCapture_Ex(CAPTURE_STOP, 1);		//20FPS模式下退出读码模式时候，设置停止拍图
		Z_SetCmosLightMode(0x020202);

		switch(Y_commGetCurType()) {
		case COMMT_USB_RS232:
			Z_FirmwareUpgrade(2);
			break;
		case COMMT_USB_Data:
			Z_FirmwareUpgrade(1);
			break;
		case COMMT_RS232:
			Z_FirmwareUpgrade(0);
			break;
		case COMMT_USB_CDC:
			Z_FirmwareUpgrade(3);
			break;

		}

		RebootDevice();
		return TRUE;
	}
	//可以设定出厂参数的序列号和日期选项
	if (strncmp((char*)(m_chReceiveBytes + 4),"~YesFFP~",8) == 0)
	{
		FFP_enable = TRUE;
		Y_commWrite((BYTE*)&cmd,1);
		return TRUE;
	}
	//关闭对出厂参数的序列号和日期选项设定
	if (strncmp((char*)(m_chReceiveBytes + 4),"~NoFFP~",7) == 0)
	{
		FFP_enable = FALSE;
		Y_commWrite((BYTE*)&cmd,1);
		return TRUE;
	}
	if (strncmp((char*)(m_chReceiveBytes + 4), "~SLEEP~",7) == 0)
	{
		Z_DeepSleep(); //深度休眠
	}
	if (strncmp((char*)(m_chReceiveBytes + 4), "~PowerOff~",10) == 0)
	{
		Z_PowerOff();
	}//End of if;
	if (strncmp((char*)(m_chReceiveBytes + 4), "ESN=", 4) == 0){
		memcpy(pValue, m_chReceiveBytes + 8, m_nReceive - 9);
		if (X_paramPlaceKeyword((char*)"ESN",(char*)pValue,TRUE) == FALSE) cmd = 0x15;
		else
		{ X_paramSaveDeviceInfo(); }
		Y_commWrite((BYTE*) &cmd, 1);
		return TRUE;
	}

	if (strncmp((char*)(m_chReceiveBytes + 4), "S/N=", 4) == 0){
		memcpy(pValue,m_chReceiveBytes + 8, m_nReceive - 9);
		if (X_paramPlaceKeyword((char*)"S/N",(char*)pValue,FFP_enable) == FALSE)
		{ cmd = 0x15; }
		else
		{ X_paramSaveDeviceInfo(); }
		FFP_enable = FALSE;
		Y_commWrite((BYTE*)&cmd, 1);

		return TRUE;
	}

	if (strncmp((char*)(m_chReceiveBytes + 4), "DATE=", 5) == 0){
		memcpy(pValue, m_chReceiveBytes + 9, m_nReceive - 10);
		if (X_paramPlaceKeyword((char*)"DATE",(char*)pValue,FFP_enable) == FALSE)
		{ cmd = 0x15; }
		else
		{ X_paramSaveDeviceInfo(); }
		FFP_enable = FALSE;
		Y_commWrite((BYTE*)&cmd, 1);
		return TRUE;
	}

	if (strncmp((char*)(m_chReceiveBytes + 4), "ManufactureID=", 14) == 0){
		memcpy(pValue, m_chReceiveBytes + 18, m_nReceive - 19);
		if (X_paramPlaceKeyword((char*)"ManufactureID",(char*)pValue,FFP_enable) == FALSE)
		{ cmd = 0x15; }
		else
		{ X_paramSaveDeviceInfo(); }
		FFP_enable = FALSE;
		Y_commWrite((BYTE*)&cmd, 1);
		return TRUE;
	}

	if (strncmp((char*)(m_chReceiveBytes + 4), "ManufactureName=", 16) == 0){
		memcpy(pValue, m_chReceiveBytes + 20, m_nReceive - 21);
		if (X_paramPlaceKeyword((char*)"ManufactureName",(char*)pValue,FFP_enable) == FALSE)
		{ cmd = 0x15; }
		else
		{ X_paramSaveDeviceInfo(); }
		FFP_enable = FALSE;
		Y_commWrite((BYTE*)&cmd, 1);
		return TRUE;
	}

	if (strncmp((char*)(m_chReceiveBytes + 4), "IsDownload=", 11) == 0){
		memcpy(pValue, m_chReceiveBytes + 15, m_nReceive - 16);
		if (X_paramPlaceKeyword((char*)"IsDownload",(char*)pValue,FFP_enable) == FALSE)
		{ cmd = 0x15; }
		else
		{ X_paramSaveDeviceInfo(); }
		FFP_enable = FALSE;
		Y_commWrite((BYTE*)&cmd, 1);
		return TRUE;
	}

	if (strncmp((char*)(m_chReceiveBytes+4),"sucBEEP",7) == 0) //声音种类 
	{
		if (m_chReceiveBytes[11] >= '0' && m_chReceiveBytes[11] <= '9'){
			X_paramBakGetGlobalAllParam()->pHard->nBeepType &= 0x0f;
			X_paramBakGetGlobalAllParam()->pHard->nBeepType |= (m_chReceiveBytes[11] - '0');
                   X_paramGetGlobalAllParam()->pHard->nBeepType &= 0x0f;
			X_paramGetGlobalAllParam()->pHard->nBeepType |= (m_chReceiveBytes[11] - '0');
		}
		else {
			cmd = 0x15;
		}
		
		Y_commWrite((BYTE*)&cmd,1);
		return X_paramSaveAsSysDefaultParam();
	}

	//设置镜像
	if (strncmp((char*)(m_chReceiveBytes + 4), "Mirr=", 5) == 0){
		memcpy(pValue, m_chReceiveBytes + 9, m_nReceive - 10);
		if (X_paramPlaceKeyword((char*)"Mirr",(char*)pValue,FFP_enable) == FALSE)
		{ cmd = 0x15; }
		else
		{ X_paramSaveDeviceInfo(); uJ_HardTypeParam(); }
		FFP_enable = FALSE;
		Y_commWrite((BYTE*)&cmd, 1);
		return TRUE;
	}

	if (strncmp((char*)(m_chReceiveBytes + 4), "DofType=", 8) == 0){
		memcpy(pValue, m_chReceiveBytes + 12, m_nReceive - 13);		
		if (X_paramPlaceKeyword((char*)"DofType",(char*)pValue,FFP_enable) == FALSE)
		{ cmd = 0x15; }
		else
		{ X_paramSaveDeviceInfo(); uJ_HardTypeParam(); }
		FFP_enable = FALSE;
		Y_commWrite((BYTE*)&cmd, 1);
		return TRUE;
	}

	//查询关键字
	if (strncmp((char*)(m_chReceiveBytes + 4), "ASKEY=", 6) == 0){
		memcpy(pValue, m_chReceiveBytes + 10, m_nReceive - 11);
		if (X_paramQueryKeyword((char*)pValue,(char*)pValue) == FALSE)
		{
			cmd = 0x15;
			Y_commWrite((BYTE*)&cmd, 1);
		}
		else
		{
			PackAndSendString((char*)pValue,strlen((char*)pValue));
		}
		return TRUE;
	}
	
	if (strncmp((char*)(m_chReceiveBytes + 4), "CMOSTYPE?", 9) == 0){
		
		switch(Z_cmos_GetType())
		{
		case 0:		//T_CMOS_UNKNOW
			sprintf((char*)pValue, "Unknown CMOS");
		    break;
		case 1:		//T_CMOS_OV7141
			sprintf((char*)pValue, "OV7141");
		    break;
		case 2:		//T_CMOS_OV9650
			sprintf((char*)pValue, "OV9650");
			break;
		case 3:		//T_CMOS_MT9V022
			sprintf((char*)pValue, "MT9V022");
			break;
		case 4:		//T_CMOS_MT9M001
			sprintf((char*)pValue, "MT9M001");
		    break;
		case 5:		//T_CMOS_OV7221
			sprintf((char*)pValue, "OV7221");
		    break;
		case 6:		//T_CMOS_OV9210
			sprintf((char*)pValue, "OV9210");
			break;
		case 7:
			sprintf((char*)pValue, "AR0134");
			break;
		case 8:
			sprintf((char*)pValue, "BF3005");
			break;
		case 9:
			sprintf((char*)pValue, "GC0308");
			break;
		default:
			sprintf((char*)pValue, "Unknown CMOS");
		    break;
		}

		PackAndSendString((char*)pValue,strlen((char*)pValue));
		return TRUE;
	}

	if (strncmp((char*)(m_chReceiveBytes + 4), "CMOSREG", 7) == 0){
		int i;
		char buf[100];
		
		switch(Z_cmos_GetType())
		{
		case 1:		//T_CMOS_OV7141
			sprintf((char*)pValue, "OV7141:\r\n\r\n");
			break;
		case 2:		//T_CMOS_OV9650
			sprintf((char*)pValue, "OV9650:\r\n\r\n");
			break;
		case 3:		//T_CMOS_MT9V022
			sprintf((char*)pValue, "MT9V022:\r\n\r\n");
			break;
		case 4:		//T_CMOS_MT9M001
			sprintf((char*)pValue, "MT9M001:\r\n\r\n");
			break;
		case 5:		//T_CMOS_OV7221
			sprintf((char*)pValue, "OV7221:\r\n\r\n");
			break;
		case 6:		//T_CMOS_OV9210
			sprintf((char*)pValue, "OV9210:\r\n\r\n");
			break;
		case 7:
			sprintf((char*)pValue, "AR0134:\r\n\r\n");
			break;
		case 8:
			sprintf((char*)pValue, "BF3005:\r\n\r\n");
			break;
		case 9:
			sprintf((char*)pValue, "GC0308:\r\n\r\n");
			break;

		case 0:		//T_CMOS_UNKNOW
		default:
			sprintf((char*)pValue, "Unknown CMOS\r\n");
			Y_commSendString((char*)pValue);
			return TRUE;
			break;
		}

		Y_commSendString((char*)pValue);
		for (i=0; i<256; i++)
		{
			sprintf(buf, "reg: %02x =  %x \r\n", i, Z_cmos_GetI2C(i));
			Y_commSendString(buf);
		}

		
		return TRUE;
	}

	return FALSE;
}

int NewWidth = 0;			//记录裁减后的图像宽度
int NewHeight = 0;			//记录裁减后的图像高度
unsigned char *Img_buffer;
unsigned char Img_Newbuf[IMG_SIZE_MAX];		//全局的存放压缩或者裁减后的图像buffer指针
unsigned char Img_Sendbuf[IMG_SIZE_MAX];	//全局的存放完整图像格式的buffer
unsigned char Img_GoodDecode[IMG_SIZE_MAX];	//全局的存放成功解码的图像.
#define MaxPicBufSize	IMG_SIZE_MAX
unsigned char picbuf[MaxPicBufSize];

void SaveGoodDecodeImg(BYTE * pImg, int nSize)
{
	if (pImg && (nSize < IMG_SIZE_MAX))
		memcpy(Img_GoodDecode, pImg, nSize);
}

#define IMG_GREY	0	
#define IMG_BMP		1	
#define IMG_TIFF	2
#define IMG_JPG		3
static char bmp_info_8BIT[1078] =
{
	0x42, 0x4D, 0x56, 0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x04, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0xBC, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x20, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x07, 0x02, 0x02, 
	0x02, 0x00, 0x03, 0x03, 0x03, 0x00, 0x04, 0x04, 0x04, 0x00, 0x05, 0x05, 0x05, 0x01, 0x06, 0x06, 
	0x06, 0x00, 0x07, 0x07, 0x07, 0x00, 0x08, 0x08, 0x08, 0x4C, 0x09, 0x09, 0x09, 0x00, 0x0A, 0x0A, 
	0x0A, 0x00, 0x0B, 0x0B, 0x0B, 0x8F, 0x0C, 0x0C, 0x0C, 0x00, 0x0D, 0x0D, 0x0D, 0x00, 0x0E, 0x0E, 
	0x0E, 0x04, 0x0F, 0x0F, 0x0F, 0x00, 0x10, 0x10, 0x10, 0x00, 0x11, 0x11, 0x11, 0x85, 0x12, 0x12, 
	0x12, 0x00, 0x13, 0x13, 0x13, 0x00, 0x14, 0x14, 0x14, 0x00, 0x15, 0x15, 0x15, 0x01, 0x16, 0x16, 
	0x16, 0x00, 0x17, 0x17, 0x17, 0x00, 0x18, 0x18, 0x18, 0xD8, 0x19, 0x19, 0x19, 0x00, 0x1A, 0x1A, 
	0x1A, 0x00, 0x1B, 0x1B, 0x1B, 0x02, 0x1C, 0x1C, 0x1C, 0x00, 0x1D, 0x1D, 0x1D, 0x00, 0x1E, 0x1E, 
	0x1E, 0x9A, 0x1F, 0x1F, 0x1F, 0x00, 0x20, 0x20, 0x20, 0x00, 0x21, 0x21, 0x21, 0xF6, 0x22, 0x22, 
	0x22, 0x00, 0x23, 0x23, 0x23, 0x00, 0x24, 0x24, 0x24, 0x00, 0x25, 0x25, 0x25, 0x01, 0x26, 0x26, 
	0x26, 0x00, 0x27, 0x27, 0x27, 0x00, 0x28, 0x28, 0x28, 0x0D, 0x29, 0x29, 0x29, 0x00, 0x2A, 0x2A, 
	0x2A, 0x00, 0x2B, 0x2B, 0x2B, 0x68, 0x2C, 0x2C, 0x2C, 0x00, 0x2D, 0x2D, 0x2D, 0x00, 0x2E, 0x2E, 
	0x2E, 0x63, 0x2F, 0x2F, 0x2F, 0x00, 0x30, 0x30, 0x30, 0x00, 0x31, 0x31, 0x31, 0xED, 0x32, 0x32, 
	0x32, 0x00, 0x33, 0x33, 0x33, 0x00, 0x34, 0x34, 0x34, 0x00, 0x35, 0x35, 0x35, 0x01, 0x36, 0x36, 
	0x36, 0x00, 0x37, 0x37, 0x37, 0x00, 0x38, 0x38, 0x38, 0xE6, 0x39, 0x39, 0x39, 0x00, 0x3A, 0x3A, 
	0x3A, 0x00, 0x3B, 0x3B, 0x3B, 0xC6, 0x3C, 0x3C, 0x3C, 0x00, 0x3D, 0x3D, 0x3D, 0x00, 0x3E, 0x3E, 
	0x3E, 0x8B, 0x3F, 0x3F, 0x3F, 0x00, 0x40, 0x40, 0x40, 0x00, 0x41, 0x41, 0x41, 0x4D, 0x42, 0x42, 
	0x42, 0x00, 0x43, 0x43, 0x43, 0x00, 0x44, 0x44, 0x44, 0x00, 0x45, 0x45, 0x45, 0x01, 0x46, 0x46, 
	0x46, 0x00, 0x47, 0x47, 0x47, 0x00, 0x48, 0x48, 0x48, 0x0E, 0x49, 0x49, 0x49, 0x00, 0x4A, 0x4A, 
	0x4A, 0x00, 0x4B, 0x4B, 0x4B, 0x91, 0x4C, 0x4C, 0x4C, 0x00, 0x4D, 0x4D, 0x4D, 0x00, 0x4E, 0x4E, 
	0x4E, 0xAB, 0x4F, 0x4F, 0x4F, 0x00, 0x50, 0x50, 0x50, 0x00, 0x51, 0x51, 0x51, 0xAF, 0x52, 0x52, 
	0x52, 0x00, 0x53, 0x53, 0x53, 0x00, 0x54, 0x54, 0x54, 0x00, 0x55, 0x55, 0x55, 0x01, 0x56, 0x56, 
	0x56, 0x00, 0x57, 0x57, 0x57, 0x00, 0x58, 0x58, 0x58, 0xA7, 0x59, 0x59, 0x59, 0x00, 0x5A, 0x5A, 
	0x5A, 0x00, 0x5B, 0x5B, 0x5B, 0x87, 0x5C, 0x5C, 0x5C, 0x00, 0x5D, 0x5D, 0x5D, 0x00, 0x5E, 0x5E, 
	0x5E, 0xE4, 0x5F, 0x5F, 0x5F, 0x00, 0x60, 0x60, 0x60, 0x00, 0x61, 0x61, 0x61, 0x8A, 0x62, 0x62, 
	0x62, 0x00, 0x63, 0x63, 0x63, 0x00, 0x64, 0x64, 0x64, 0x00, 0x65, 0x65, 0x65, 0x01, 0x66, 0x66, 
	0x66, 0x00, 0x67, 0x67, 0x67, 0x00, 0x68, 0x68, 0x68, 0x69, 0x69, 0x69, 0x69, 0x00, 0x6A, 0x6A, 
	0x6A, 0x00, 0x6B, 0x6B, 0x6B, 0x56, 0x6C, 0x6C, 0x6C, 0x00, 0x6D, 0x6D, 0x6D, 0x00, 0x6E, 0x6E, 
	0x6E, 0x56, 0x6F, 0x6F, 0x6F, 0x00, 0x70, 0x70, 0x70, 0x00, 0x71, 0x71, 0x71, 0xAB, 0x72, 0x72, 
	0x72, 0x00, 0x73, 0x73, 0x73, 0x00, 0x74, 0x74, 0x74, 0x00, 0x75, 0x75, 0x75, 0x01, 0x76, 0x76, 
	0x76, 0x00, 0x77, 0x77, 0x77, 0x00, 0x78, 0x78, 0x78, 0xC2, 0x79, 0x79, 0x79, 0x00, 0x7A, 0x7A, 
	0x7A, 0x00, 0x7B, 0x7B, 0x7B, 0x88, 0x7C, 0x7C, 0x7C, 0x00, 0x7D, 0x7D, 0x7D, 0x00, 0x7E, 0x7E, 
	0x7E, 0x81, 0x7F, 0x7F, 0x7F, 0x00, 0x80, 0x80, 0x80, 0x00, 0x81, 0x81, 0x81, 0x51, 0x82, 0x82, 
	0x82, 0x00, 0x83, 0x83, 0x83, 0x00, 0x84, 0x84, 0x84, 0x00, 0x85, 0x85, 0x85, 0x01, 0x86, 0x86, 
	0x86, 0x00, 0x87, 0x87, 0x87, 0x00, 0x88, 0x88, 0x88, 0x94, 0x89, 0x89, 0x89, 0x00, 0x8A, 0x8A, 
	0x8A, 0x00, 0x8B, 0x8B, 0x8B, 0xBC, 0x8C, 0x8C, 0x8C, 0x00, 0x8D, 0x8D, 0x8D, 0x00, 0x8E, 0x8E, 
	0x8E, 0x6D, 0x8F, 0x8F, 0x8F, 0x00, 0x90, 0x90, 0x90, 0x00, 0x91, 0x91, 0x91, 0x37, 0x92, 0x92, 
	0x92, 0x00, 0x93, 0x93, 0x93, 0x00, 0x94, 0x94, 0x94, 0x00, 0x95, 0x95, 0x95, 0x01, 0x96, 0x96, 
	0x96, 0x00, 0x97, 0x97, 0x97, 0x00, 0x98, 0x98, 0x98, 0xAB, 0x99, 0x99, 0x99, 0x00, 0x9A, 0x9A, 
	0x9A, 0x00, 0x9B, 0x9B, 0x9B, 0x93, 0x9C, 0x9C, 0x9C, 0x00, 0x9D, 0x9D, 0x9D, 0x00, 0x9E, 0x9E, 
	0x9E, 0xC9, 0x9F, 0x9F, 0x9F, 0x00, 0xA0, 0xA0, 0xA0, 0x00, 0xA1, 0xA1, 0xA1, 0xF8, 0xA2, 0xA2, 
	0xA2, 0x00, 0xA3, 0xA3, 0xA3, 0x00, 0xA4, 0xA4, 0xA4, 0x00, 0xA5, 0xA5, 0xA5, 0x01, 0xA6, 0xA6, 
	0xA6, 0x00, 0xA7, 0xA7, 0xA7, 0x00, 0xA8, 0xA8, 0xA8, 0x37, 0xA9, 0xA9, 0xA9, 0x00, 0xAA, 0xAA, 
	0xAA, 0x00, 0xAB, 0xAB, 0xAB, 0x32, 0xAC, 0xAC, 0xAC, 0x00, 0xAD, 0xAD, 0xAD, 0x00, 0xAE, 0xAE, 
	0xAE, 0xE6, 0xAF, 0xAF, 0xAF, 0x00, 0xB0, 0xB0, 0xB0, 0x00, 0xB1, 0xB1, 0xB1, 0xD3, 0xB2, 0xB2, 
	0xB2, 0x00, 0xB3, 0xB3, 0xB3, 0x00, 0xB4, 0xB4, 0xB4, 0x00, 0xB5, 0xB5, 0xB5, 0x01, 0xB6, 0xB6, 
	0xB6, 0x00, 0xB7, 0xB7, 0xB7, 0x00, 0xB8, 0xB8, 0xB8, 0x5D, 0xB9, 0xB9, 0xB9, 0x00, 0xBA, 0xBA, 
	0xBA, 0x00, 0xBB, 0xBB, 0xBB, 0x4C, 0xBC, 0xBC, 0xBC, 0x00, 0xBD, 0xBD, 0xBD, 0x00, 0xBE, 0xBE, 
	0xBE, 0x84, 0xBF, 0xBF, 0xBF, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0xC1, 0xC1, 0xC1, 0x1B, 0xC2, 0xC2, 
	0xC2, 0x00, 0xC3, 0xC3, 0xC3, 0x00, 0xC4, 0xC4, 0xC4, 0x00, 0xC5, 0xC5, 0xC5, 0x01, 0xC6, 0xC6, 
	0xC6, 0x00, 0xC7, 0xC7, 0xC7, 0x00, 0xC8, 0xC8, 0xC8, 0x61, 0xC9, 0xC9, 0xC9, 0x00, 0xCA, 0xCA, 
	0xCA, 0x00, 0xCB, 0xCB, 0xCB, 0xD2, 0xCC, 0xCC, 0xCC, 0x00, 0xCD, 0xCD, 0xCD, 0x00, 0xCE, 0xCE, 
	0xCE, 0xC6, 0xCF, 0xCF, 0xCF, 0x00, 0xD0, 0xD0, 0xD0, 0x00, 0xD1, 0xD1, 0xD1, 0x3A, 0xD2, 0xD2, 
	0xD2, 0x00, 0xD3, 0xD3, 0xD3, 0x00, 0xD4, 0xD4, 0xD4, 0x00, 0xD5, 0xD5, 0xD5, 0x01, 0xD6, 0xD6, 
	0xD6, 0x00, 0xD7, 0xD7, 0xD7, 0x00, 0xD8, 0xD8, 0xD8, 0x67, 0xD9, 0xD9, 0xD9, 0x00, 0xDA, 0xDA, 
	0xDA, 0x00, 0xDB, 0xDB, 0xDB, 0x0C, 0xDC, 0xDC, 0xDC, 0x00, 0xDD, 0xDD, 0xDD, 0x00, 0xDE, 0xDE, 
	0xDE, 0xFA, 0xDF, 0xDF, 0xDF, 0x00, 0xE0, 0xE0, 0xE0, 0x00, 0xE1, 0xE1, 0xE1, 0x6B, 0xE2, 0xE2, 
	0xE2, 0x00, 0xE3, 0xE3, 0xE3, 0x00, 0xE4, 0xE4, 0xE4, 0x00, 0xE5, 0xE5, 0xE5, 0x01, 0xE6, 0xE6, 
	0xE6, 0x00, 0xE7, 0xE7, 0xE7, 0x00, 0xE8, 0xE8, 0xE8, 0x3F, 0xE9, 0xE9, 0xE9, 0x00, 0xEA, 0xEA, 
	0xEA, 0x00, 0xEB, 0xEB, 0xEB, 0x4F, 0xEC, 0xEC, 0xEC, 0x00, 0xED, 0xED, 0xED, 0x00, 0xEE, 0xEE, 
	0xEE, 0xD2, 0xEF, 0xEF, 0xEF, 0x00, 0xF0, 0xF0, 0xF0, 0x00, 0xF1, 0xF1, 0xF1, 0xC0, 0xF2, 0xF2, 
	0xF2, 0x00, 0xF3, 0xF3, 0xF3, 0x00, 0xF4, 0xF4, 0xF4, 0x00, 0xF5, 0xF5, 0xF5, 0x01, 0xF6, 0xF6, 
	0xF6, 0x00, 0xF7, 0xF7, 0xF7, 0x00, 0xF8, 0xF8, 0xF8, 0xFC, 0xF9, 0xF9, 0xF9, 0x00, 0xFA, 0xFA, 
	0xFA, 0x00, 0xFB, 0xFB, 0xFB, 0x6F, 0xFC, 0xFC, 0xFC, 0x00, 0xFD, 0xFD, 0xFD, 0x00, 0xFE, 0xFE, 
	0xFE, 0x06, 0xFF, 0xFF, 0xFF, 0x00
};
int BMP_Edit(BYTE *pSrcIMG, BYTE *pTagImg, int nWidth, int nHeight)
{
	int n = nWidth;
	int i = nHeight;
	int len = n*i + 1078;
	unsigned char* p = pTagImg;

	bmp_info_8BIT[2] = len &0xff;
	bmp_info_8BIT[3] = (len>>8) &0xff;
	bmp_info_8BIT[4] = (len>>16) &0xff;
	bmp_info_8BIT[5] = (len>>24) &0xff;
	
	bmp_info_8BIT[18] = n &0xff;
	bmp_info_8BIT[19] = (n>>8) &0xff;
	bmp_info_8BIT[20] = (n>>16) &0xff;
	bmp_info_8BIT[21] = (n>>24) &0xff;
	
	bmp_info_8BIT[22] = i &0xff;
	bmp_info_8BIT[23] = (i>>8) &0xff;
	bmp_info_8BIT[24] = (i>>16) &0xff;
	bmp_info_8BIT[25] = (i>>24) &0xff;
	
	i = len - 1078;
	bmp_info_8BIT[0x22] = i &0xff;
	bmp_info_8BIT[0x23] = (i>>8) &0xff;
	bmp_info_8BIT[0x24] = (i>>16) &0xff;
	bmp_info_8BIT[0x25] = (i>>24) &0xff;

	memcpy(p, bmp_info_8BIT, 1078);
	p += 1078;
	memcpy(p, pSrcIMG, nWidth*nHeight);

	return len;
}
/* 该接口将完整的图像进行压缩或者裁减,保存到Img_buffer */
void IMG_Edit(int mode, int start_x, int start_y, int end_x, int end_y, int nScale_x, int nScale_y, void *img)
{
	int i, j, nW; 
	int tmpW = 0;
	int tmpH = 0;
	unsigned long nTmpLen = 0;
	unsigned char *p = (unsigned char*)img;
    int Flag = 0;
	
	NewWidth = Y_cmosGetWidth();
	NewHeight = Y_cmosGetHeight();
	/* 没有任何裁减或者压缩的需求 */
	if(start_x==0 && start_y==0 && end_x==NewWidth && end_y==NewHeight && nScale_x==1 && nScale_y==1 && mode!=IMG_BMP)
	{
		Img_buffer = (unsigned char*)img;	//正常图像不用申请新的空间,直接将指针指向原来的img buf就OK.		
		return;
	}
	/* 有裁减或压缩需求则重新提取像素 */
	else
	{
		nW = NewWidth;
		
		if (mode == IMG_GREY)
		{//灰度图
			for(j = start_y, p += j*nW; j < end_y; j += nScale_y, p += nW*nScale_y)
			{
				for(i = start_x; i < end_x; i += nScale_x)
				{
					Img_Newbuf[nTmpLen++] = p[i];	
				}

				if(Flag == 0)	//只在第一行记录一次新的图像宽度.
					tmpW = nTmpLen;
				tmpH ++;		//记录新的图像的高度.
				Flag = 1;		//标记下个循环不再记录图像宽度.
			}
		}
		else if(mode == IMG_BMP)
		{//bmp图
			for(j=end_y-1, p=((BYTE*)img+j*nW); j>=start_y; j-=nScale_y, p=(BYTE*)img+j*nW)
			{
				for(i = start_x; i < end_x; i += nScale_x)
				{
					Img_Newbuf[nTmpLen++] = p[i];	
				}
				
				if(Flag == 0)	//只在第一行记录一次新的图像宽度.
					tmpW = nTmpLen;
				tmpH ++;		//记录新的图像的高度.
				Flag = 1;		//标记下个循环不再记录图像宽度.
			}
		}
		else
			return;		
		/* 只在函数的末尾给全局变量赋值一次 */
		NewWidth = tmpW;	
		NewHeight = tmpH;
		Img_buffer = Img_Newbuf;
	}	
}
extern int String2Hex(unsigned char* pSrc, unsigned short nSrcLen, unsigned char* pBuff, unsigned short nMaxSize);

void Img_Send(int nType, int nQuality, BOOL bInstr)
{
//	int i = 0;
//	int cyctime,rest = 0;
	unsigned long nLength = 0;
      unsigned char *p;
      unsigned char chBuf[16];
      int len;
      
      if(bInstr == 0)
        p = Img_Sendbuf+6;
      else
        p = Img_Sendbuf;

	if(nQuality < 0 || nQuality > 3)
		return;
	if(nType == IMG_BMP){//发送BMP图像
		nLength = BMP_Edit(Img_buffer, p, NewWidth, NewHeight);	
	}
	else if(nType == IMG_TIFF){//发送TIFF图像
		nLength = GXIMG_BMPTOTIFFEXT(Img_buffer, p, NewWidth, NewHeight);
	}
	else if(nType == IMG_JPG){//发送JPEG图像
		nLength = GXIMG_BMPTOJPEGSelQulity(Img_buffer, p, NewWidth, NewHeight, nQuality);	
	}

       if(bInstr == 0)
	{
	    Img_Sendbuf[0] = 0x2;
	    Img_Sendbuf[1] = 0;
	    memcpy(&Img_Sendbuf[2], &nLength, 4);
      }

      if(bInstr == 0)
        Y_commWrite(Img_Sendbuf, nLength+6);
      else
      {   
        len = (unsigned int)nLength;
        String2Hex((unsigned char *)&len, 4, chBuf, 16);
        Y_commWrite(chBuf, 8);
        Y_commWrite(Img_Sendbuf, nLength);
      }

}

void IMG_Edit_Send(int mode, int start_x, int start_y, int end_x, int end_y, int nScale_x, int nScale_y, int nWidth, void *img)
{
	int i, j, nW;
	unsigned char data, value;
 	unsigned char byTmpData[13*1024];
// 	unsigned char byTmpData[1*1024];
	unsigned int nTmpLen = 0;
	unsigned char *p = (unsigned char*)img;
	nW = nWidth;

	if (mode == 0)
	{//灰度图
		for(j=start_y, p +=j*nW; j<end_y; j+=nScale_y,p += nW*nScale_y)
		{
			for(i=start_x; i<end_x; i+=nScale_x)
			{
				byTmpData[nTmpLen++] = p[i];
				if (nTmpLen >= sizeof(byTmpData))
				{
					Y_commWrite(byTmpData,nTmpLen);
					nTmpLen = 0;
					//从PC发来信息
					if(Y_commBakReadChar((char *)&value,0) > 0)
					{
						return;
					}
				}//End of if;
			}//End of for;
		}

	}
	else if (mode == 1)
	{//二值图
		for(j=start_y; j<end_y; j+=nScale_y)
		{
			for(i=start_x; i<end_x; i+=nScale_x)
			{
				data = (p[j*nW + i] > 128)?0xFF:0x00;
				if(data) value |= (1 << ((i/nScale_x)%8));
				if((((i/nScale_x)+1)%8) == 0)
				{
					byTmpData[nTmpLen++] = value;
					if (nTmpLen >= sizeof(byTmpData))
					{
						Y_commWrite(byTmpData,nTmpLen);
						nTmpLen = 0;
						//从PC发来信息
						if(Y_commBakReadChar((char *)&value,0) > 0)
						{
							return;
						}
					}//End of if;
					value = 0;
				}
			}
		}
	}
	else
		return;

	if (nTmpLen > 0)
	{
		Y_commWrite(byTmpData,nTmpLen);
		nTmpLen = 0;
	}//End of if;

}

BOOL Build_AL_IMG_REQ(void)
{
	int nScale_x, nScale_y;
	unsigned char *p;
	int nW,nH,sW,sH;
	int nBreak;
	int tmp;
	int jpgquelity = 0;
	int imgflag = 0;
	int editimgflag =0;
	BOOL bTestBuf = FALSE;
	int nTestBufNum = 0;

	nBreak = Z_DogGetBreak(); //记录原先看门狗的复位时间
	Z_DogReset(-1);//禁用看门狗

	if ((m_nReceive == 8) || (m_nReceive == 9))	//老的指令
	{

		if (m_nReceive == 9)
		{
			bTestBuf = TRUE;
			nTestBufNum = m_chReceiveBytes[7];
		}
		else
			bTestBuf = FALSE;

		//图像获取方式
		g_ImgGetMode = 0;
		//图像颜色设置
		g_ImgColor = m_chReceiveBytes[6];
		//图像样式设置
		if (m_chReceiveBytes[5] != 0)
		{
			if(m_chReceiveBytes[5] >= 1 && m_chReceiveBytes[5] <= 2)
			{
				g_ImgType = 1;
				g_ImgReduceRow = m_chReceiveBytes[5];		//记录行压缩参数,1为2倍,2为4倍
				g_ImgReduceColumn = m_chReceiveBytes[5];	//记录列压缩参数
			}
			else
				g_ImgType = (int)m_chReceiveBytes[5];
		}			
 		else
 			g_ImgType = 0;
		//压缩图像参数设置
	//	g_ImgReduceRow = m_chReceiveBytes[5];
	//	g_ImgReduceColumn = m_chReceiveBytes[5];
	}
	/* 
	为EM2028HP 通过通讯接口获取不同格式的图片提供接口:
	1. 可以获取
	2. 可以提供1:1,1:4,1:16压缩的不同格式图像.
	3. 可以选择获取cmos刚刚获取的一帧图像,或者最近一帧成功解码的图像
	4. jpg图像支持不同的图像质量,图像质量从0-3递增.
	图像获取接口的协议:
	byte0-1：包头（0x7e  0x00）
	byte2-3：数据信息长度MSB(byte2)、LSB(byte3)，不包括包头（2byte）、数据信息长度（2byte）和校验位（1byte）
	byte4: 图像获取数据包标志位（0x35） 
	byte5：图像压缩比例。
	0：原图；1：1/4压缩; 2: 1/16 压缩
	byte6: 选择获取图像的类型.
	4: 获取bmp图像 5: 获取tiff图像 6: 获取jpg图像
	byte7: 如果获取的是jpg图像,还提供图像质量选项
	0: 最差 1: 稍好 2: 更好 3: 最好
	byte8: 选择获取最近一帧拍摄的图像还是解码成功的图像
	0: 获取最近一帧拍摄的图像
	1: 获取最近一帧解码成功的图像
	byte9: 校验位

*/
/*---------------------------------------------------------------*/
	else if (m_nReceive == 10)
	{
		/* 记录压缩参数 */
		if(m_chReceiveBytes[5] >= 0 && m_chReceiveBytes[5] <= 2)	
		{												
			g_ImgReduceRow = m_chReceiveBytes[5];		//记录行压缩参数,0为正常,1为2倍,2为4倍
			g_ImgReduceColumn = m_chReceiveBytes[5];	//记录列压缩参数			
		}else{
			g_ImgReduceRow = 0;			//如果压缩参数超出范围则默认为不压缩
			g_ImgReduceColumn = 0;
		}
		/* 记录获取的图像类型 */
		if(m_chReceiveBytes[6] >= 4 && m_chReceiveBytes[6] <= 6)
			g_ImgType = (int)m_chReceiveBytes[6];
		else
			return FALSE;				//该接口下图像格式不为bmp,tiff,jpg的话直接退出失败
		/* 记录JPG图像的获取质量 */
		if(m_chReceiveBytes[7] >= 0 && m_chReceiveBytes[7] <= 3)
			jpgquelity = (int)m_chReceiveBytes[7];
		else
			jpgquelity = 0;				//如果图像质量范围超出0-3则默认为0
		/* 是否要发送解码成功的图像 */
		if(m_chReceiveBytes[8] == 1)
			imgflag = 1;
		
	}
/*
27字节图片获取指令提供接口:

	1. 可以提供1:1,1:4,1:16压缩的不同格式图像.
	2. 可以选择获取cmos刚刚获取的一帧图像,或者最近一帧成功解码的图像，立刻拍摄一张
	3. jpg图像支持不同的图像质量,图像质量从0-3递增.
	4.可以自由裁减图像

图像获取接口的协议:
	byte0-1：包头（0x7e 0x00）
	byte2-3：数据信息长度MSB(byte2)、LSB(byte3)，不包括包头（2byte）、数据信息长度（2byte）和校验位（1byte）
	byte4: 图像获取数据包标志位（0x35） 
	byte5：选择获取图像方式
	0: 获取最近一帧拍摄的图像
	1: 获取最近一帧解码成功的图像
	byte6: 值位0
	byte7: 如果获取的是jpg图像,还提供图像质量选项
	0: 最差 1: 稍好 2: 更好 3: 最好
	byte8：横向压缩比例1/2n ，n是byte8的值
	byte9:  纵向压缩比例1/2n ，n是byte9的值
	byte10-25: 裁剪图像的坐标
		byte10-13：start_x裁剪开始点横坐标
			start_x=byte10*1000+byte11*100+byte12*10+byte13;
		byte14-17：start_y裁剪开始点纵坐标
			start_y=byte14*1000+byte15*100+byte16*10+byte17;
		byte18-21：end_x裁剪结束点横坐标
			end_x=byte18*1000+byte19*100+byte12*20+byte21;
		byte22-25：end_y裁剪结束点纵坐标
			end_y =byte22*1000+byte23*100+byte24*10+byte25;
	byte26: 校验位

	*/
/*---------------------------------------------------------------*/
	else 
	{
	     editimgflag = 1;
		//图像获取方式
		g_ImgGetMode = m_chReceiveBytes[5];
		if (g_ImgGetMode == 1)
		{
				imgflag = 1;
		}
		//图像颜色设置
		g_ImgColor = m_chReceiveBytes[6];
		//图像样式设置
		g_ImgType = m_chReceiveBytes[7];
		//压缩图像参数设置
		g_ImgReduceRow = m_chReceiveBytes[8];
		g_ImgReduceColumn = m_chReceiveBytes[9];
		//裁剪图像参数设置

			tmp = m_chReceiveBytes[10]*1000 + m_chReceiveBytes[11]*100
					+ m_chReceiveBytes[12]*10 + m_chReceiveBytes[13];
			if ((tmp<0) || (tmp > Y_cmosGetWidth()))
				tmp = Y_cmosGetWidth();
			g_ImgCutStartRow = tmp;
			
			tmp = m_chReceiveBytes[14]*1000 + m_chReceiveBytes[15]*100
					+ m_chReceiveBytes[16]*10 + m_chReceiveBytes[17];
			if ((tmp<0) || (tmp > Y_cmosGetHeight()))
				tmp = Y_cmosGetHeight();
			g_ImgCutStartColumn = tmp;
			
			tmp = m_chReceiveBytes[18]*1000 + m_chReceiveBytes[19]*100
					+ m_chReceiveBytes[20]*10 + m_chReceiveBytes[21];
			if ((tmp<0) || (tmp > Y_cmosGetWidth()) || (tmp < g_ImgCutStartRow))
				tmp = g_ImgCutStartRow;
			g_ImgCutEndRow = tmp;
			
			tmp = m_chReceiveBytes[22]*1000	+ m_chReceiveBytes[23]*100
					+ m_chReceiveBytes[24]*10 + m_chReceiveBytes[25];
			if ((tmp<0) || (tmp > Y_cmosGetHeight()) || (tmp < g_ImgCutStartColumn))
				tmp = g_ImgCutStartColumn;
			g_ImgCutEndColumn = tmp;
		
	}

/*	if (g_ImgGetMode == 2 && X_paramGetGlobalAllParam()->pCode->nReadMode != ESC2)
	{
		U_appCaptureImg(TRUE);
	}*/

	if (bTestBuf)
	{
		p = Z_GetCmosBufTest(nTestBufNum);
	}
	else
		p = (BYTE*)Y_cmosGetDataAddr();
	
    if (editimgflag == 1)
    {
		sW=g_ImgCutStartRow;
		sH=g_ImgCutStartColumn;
		nW = g_ImgCutEndRow ;
		nH = g_ImgCutEndColumn;
    }
	else
	{
		sW=0;
		sH=0;
		nW = Y_cmosGetWidth();
		nH = Y_cmosGetHeight();
	}

	if(g_ImgType == 0)
	{//原始图
		nScale_x = 1;			//行压缩
		nScale_y = 1;			//列压缩
		if ((Y_commGetCurType() == COMMT_USB_Data) && (g_ImgColor == 0))
		{
			Y_commWrite(p,nW*nH);
			Z_DogReset(nBreak); //恢复看门狗
			return TRUE;
		}//End of if;
		IMG_Edit_Send(g_ImgColor, 0, 0, nW, nH, nScale_x, nScale_y, nW, p);
	}
	else if (g_ImgType == 1)
	{//压缩图像
	
		nScale_x = 1<<g_ImgReduceRow;			//行压缩
		nScale_y = 1<<g_ImgReduceColumn;		//列压缩
		IMG_Edit_Send(g_ImgColor, 0, 0, nW, nH, nScale_x, nScale_y, nW, p);
	}
	else if (g_ImgType == 2)
	{//剪裁图像
	
		nScale_x = 1;			//行压缩
		nScale_y = 1;			//列压缩
		IMG_Edit_Send(g_ImgColor, g_ImgCutStartRow, g_ImgCutStartColumn, 
			g_ImgCutEndRow, g_ImgCutEndColumn, nScale_x, nScale_y, nW, p);	
	}
	else if (g_ImgType == 3)
	{//剪裁并且压缩图像
	
		nScale_x = 1<<g_ImgReduceRow;			//行压缩
		nScale_y = 1<<g_ImgReduceColumn;		//列压缩
		IMG_Edit_Send(g_ImgColor, g_ImgCutStartRow, g_ImgCutStartColumn, 
			g_ImgCutEndRow, g_ImgCutEndColumn, nScale_x, nScale_y, nW, p);	
	}
/*---------------------------------------------------------------*/	
/* 这里新增2028HP项目获取bmp,tiff,jpg图像的图像处理与发送部分 */
	else if(g_ImgType >= 4 && g_ImgType <= 6)
	{
		/* 不管处理何种格式的图像.压缩参数的处理都相同 */
		nScale_x = 1<<g_ImgReduceRow;		
		nScale_y = 1<<g_ImgReduceColumn;
		/* 查询是否要发送解码成功的图像 */
		if(imgflag == 1 )	
		{
	    	p = Img_GoodDecode;			//要的话将临时图像指针指向解码成功的图像			
		}

		/* 处理并发送bmp格式图像 */
		if(g_ImgType == 4){	
			IMG_Edit(IMG_BMP, sW , sH, nW, nH, nScale_x, nScale_y, p);	//图像预处理:处理压缩操作并修改bmp的行颠倒排列
			Img_Send(IMG_BMP, 0, 0);									//将图像转成bmp格式并发送图像
		}
		/* 处理并发送tiff格式图像 */
		else if(g_ImgType == 5){
			IMG_Edit(IMG_GREY, sW , sH, nW, nH, nScale_x, nScale_y, p); //如果要压缩则要预处理灰度图像
			Img_Send(IMG_TIFF, 0, 0);									 //将图像转成tiff格式并发送图像
		}
		/* 处理并发送jpg格式图像 */
		else if(g_ImgType == 6){ 
			IMG_Edit(IMG_GREY, sW , sH, nW, nH, nScale_x, nScale_y, p); //如果要压缩则要预处理灰度图像
			Img_Send(IMG_JPG, jpgquelity, 0);							 //将图像按质量转成jpg格式并发送图像	
		}
	}
/*---------------------------------------------------------------*/
	Z_DogReset(nBreak); //恢复看门狗
	return TRUE;
}



extern int W_setFormat(int formatID,int setflag,char *cmd,int len);

BOOL Build_AL_SET_DATAFORMAT(void)
{


	int format_id = -1;
	int n =0;
	//char cmd[256];

	format_id = (int)m_chReceiveBytes[5];
	n = (int)m_chReceiveBytes[6];

	switch (n)
	{
	case 0x00: //整组dataformat
		if (!W_setFormat(format_id,1,m_chReceiveBytes,m_nReceive))
			return FALSE;
		break;
	case 0x01: //修改Terminal ID
		if (!W_setFormat(format_id,2,m_chReceiveBytes,m_nReceive))
			return FALSE;
		break;
	case 0x02: //修改Code ID
			//Y_commWrite((BYTE*)m_chReceiveBytes,10);
		if (!W_setFormat(format_id,3,m_chReceiveBytes,m_nReceive))
			return FALSE;
		break;
	case 0x03: //适用长度
		if (!W_setFormat(format_id,4,m_chReceiveBytes,m_nReceive))
			return FALSE;
		break;
	case 0x04: //修改命令字符串，包括命令类型 ID
		if (!W_setFormat(format_id,5,m_chReceiveBytes,m_nReceive))
			return FALSE;
		break;
	default:
		return FALSE;
		break;
	}
  	return TRUE;
}

static int GetHex(char c)
{
	if (c >= '0' && c <= '9') return (c-'0');
	else if (c >= 'a' && c <= 'f') return (c-'a') + 10;
	else if (c >= 'A' && c <= 'F') return (c-'A') + 10;
	else return -1;
}

//一维条码参数查询
BOOL b1DParamQuery(void)
{
	unsigned int nParam;
//	BOOL IsFinish = FALSE;
//	unsigned char *p;
//	int i;
	unsigned int Param;

    nParam = (int)(m_chReceiveBytes[6] - 0x30) * 10; //获得条码类型
	nParam += (int)(m_chReceiveBytes[7] - 0x30);
	m_chSendBytes[3] = 0x05;
	m_chSendBytes[5] = m_chReceiveBytes[6];
	m_chSendBytes[6] = m_chReceiveBytes[7];
	m_chSendBytes[7] = 0;
    m_chSendBytes[8] = 0;

	switch(nParam) {
	case POS_ZASETUP:
		if(X_paramGetGlobalAllParam()->pCode->SetCode) m_chSendBytes[10] |= 0x01;
		else m_chSendBytes[10] &= ~0x01;
		///////////
		if(W_setcIsEnable()) m_chSendBytes[10] |= 0x02;
		else m_chSendBytes[10] &= ~0x02;
		m_nSend = 11;
		break;
	case POS_CODE128:
	case POS_CODABAR:
	case POS_MATRIX25:
	case POS_REGULAR39:
	case POS_MODULE25:
	case POS_CODEZ:
//		sprintf(m_chSendBytes+7,"%04d",X_paramGetGlobalAllParam()->p1D->ExMsg[nParam]&0x0000ffff);
		memcpy(&Param, &(X_paramGetGlobalAllParam()->p1D->ExMsg[nParam]), 4);
		m_chSendBytes[7] = ((X_paramGetGlobalAllParam()->p1D->ExMsg[nParam] >> 12) & 0x0f) | 0x30;
		m_chSendBytes[8] = ((X_paramGetGlobalAllParam()->p1D->ExMsg[nParam] >> 8) & 0x0f) | 0x30;
		m_chSendBytes[9] = ((X_paramGetGlobalAllParam()->p1D->ExMsg[nParam] >> 4) & 0x0f) | 0x30;
		m_chSendBytes[10] = (X_paramGetGlobalAllParam()->p1D->ExMsg[nParam] & 0x0f) | 0x30;
		m_nSend = 11;
		break;
	case POS_EAN8:
	case POS_EAN13:
	case POS_UPCE:
	case POS_UPCA:
//		sprintf(m_chSendBytes+7,"%04d",X_paramGetGlobalAllParam()->p1D->ExMsg[nParam]&0x000000ff);
		memcpy(&Param, &(X_paramGetGlobalAllParam()->p1D->ExMsg[nParam]), 4);
		m_chSendBytes[7] = 0x30;
		m_chSendBytes[8] = ((X_paramGetGlobalAllParam()->p1D->ExMsg[nParam] >> 8) & 0x0f) | 0x30;
		m_chSendBytes[9] = ((X_paramGetGlobalAllParam()->p1D->ExMsg[nParam] >> 4) & 0x0f) | 0x30;
		m_chSendBytes[10] = (X_paramGetGlobalAllParam()->p1D->ExMsg[nParam] & 0x0f) | 0x30;
		m_nSend = 11;
		break;
	case POS_INTER2OF5:
//		sprintf(m_chSendBytes+7,"%04d",X_paramGetGlobalAllParam()->p1D->ExMsg[nParam]&0x0000ffff);
		memcpy(&Param, &(X_paramGetGlobalAllParam()->p1D->ExMsg[nParam]), 4);
		m_chSendBytes[7] = ((X_paramGetGlobalAllParam()->p1D->ExMsg[nParam] >> 12) & 0x0f) | 0x30;
		m_chSendBytes[8] = ((X_paramGetGlobalAllParam()->p1D->ExMsg[nParam] >> 8) & 0x0f) | 0x30;
		m_chSendBytes[9] = ((X_paramGetGlobalAllParam()->p1D->ExMsg[nParam] >> 4) & 0x0f) | 0x30;
		m_chSendBytes[10] = (X_paramGetGlobalAllParam()->p1D->ExMsg[nParam] & 0x0f) | 0x30;
		memcpy(m_chSendBytes+12, (char*)X_paramGetGlobalAllParam()->p1D->I2Of5Len+1, 4);
		m_chSendBytes[11] = 0x30;
		if(X_paramGetGlobalAllParam()->p1D->I2Of5Len[0]) m_chSendBytes[9] |= 0x01;
		else m_chSendBytes[9] &= (~0x01);
		m_nSend = 16;
		break;
	case POS_CODE93:
	case POS_ISBN:
	case POS_INDUSTRIAL25:
	case POS_STANDARD25:
	case POS_PLESSEY:
	case POS_CODE11:
	case POS_MSI_PLESSEY:
	case POS_COMPOSITE:
	case POS_RSS:
	case POS_CHNPOST:
		memcpy(&Param, &(X_paramGetGlobalAllParam()->p1D->ExMsg[nParam]), 4);
		m_chSendBytes[7] = ((Param >> 12) & 0x0f) | 0x30;
		m_chSendBytes[8] = ((Param >> 8) & 0x0f) | 0x30;
		m_chSendBytes[9] = ((Param >> 4) & 0x0f) | 0x30;
		m_chSendBytes[10] = (Param & 0x0f) | 0x30;
		m_nSend = 11;
		break;
	default : 
		return FALSE;
	}
	return TRUE;
}

//二维条码参数查询
BOOL b2DParamQuery(void)
{
	unsigned int nParam;
	int nTemp;
	unsigned int n;

    nParam = (int)(m_chReceiveBytes[6] - 0x30) * 10; //获得条码类型
	nParam += (int)(m_chReceiveBytes[7] - 0x30);
	n = (int)(m_chReceiveBytes[8] - 0x30);
	m_chSendBytes[3] = 0x05;
	m_chSendBytes[5] = m_chReceiveBytes[6];
	m_chSendBytes[6] = m_chReceiveBytes[7];

	switch(nParam - 0x20) {
	case POS_PDF417:
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] & DC_PDF417_DOUBLE;
		nTemp >>= POS_PDF417_DOUBLE0;
		if(nTemp >= 0 && nTemp < 3) m_chSendBytes[7] = nTemp + 0x30;//单双码
		else return FALSE;
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] & DC_PDF417_Reverse;
		nTemp >>= POS_PDF417_Reverse0;
		if(nTemp >= 0 && nTemp < 3) m_chSendBytes[8] = nTemp + 0x30;	//正反向：0--只读正向, 1--只读反向,2--- 正反向都读
		else return FALSE;
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] & DC_PDF417_Mirror;
		nTemp >>= POS_PDF417_Mirror;
		if(nTemp >= 0 && nTemp < 2) m_chSendBytes[9] = nTemp + 0x30;	//镜像识别使能
		else return FALSE;
		m_nSend = 10;
		break;

	case POS_QR:
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_QR] & DC_QR_DOUBLE;
		nTemp >>= POS_QR_DOUBLE0;
		if(nTemp >= 0 && nTemp < 3) m_chSendBytes[7] = nTemp + 0x30;
		else return FALSE;
		m_nSend = 8;
		break;

	case POS_Aztec:
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] & DC_AZTEC_MULTIEN;
		nTemp >>= POS_AZTEC_MULTIEN0;
		if(nTemp >= 0 && nTemp < 3) m_chSendBytes[7] = nTemp + 0x30;
		else return FALSE;
		
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] & DC_AZTEC_MULTINUM;
		m_chSendBytes[8] = nTemp + 0x30;
		m_nSend = 9;
		break;

	case POS_DataMatrix:
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] & DC_DM_DOUBLE;
		nTemp >>= POS_DM_DOUBLE0;
		if(nTemp >= 0 && nTemp < 3) m_chSendBytes[7] = nTemp + 0x30;//单双码
		else return FALSE;
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] & DC_DM_Reverse;
		nTemp >>= POS_DM_Reverse0;
		if(nTemp >= 0 && nTemp < 3) m_chSendBytes[8] = nTemp + 0x30;//正反向：0--只读正向, 1--只读反向,2--- 正反向都读
		else return FALSE;
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] & DC_DM_Mirror;
		nTemp >>= POS_DM_Mirror;
		if(nTemp >= 0 && nTemp < 2) m_chSendBytes[9] = nTemp + 0x30;	//镜像识别使能
		else return FALSE;
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] & DC_DM_129_ENABLE;
		nTemp >>= POS_DM_129_ENABLE;
		if(nTemp >= 0 && nTemp < 2) m_chSendBytes[10] = nTemp + 0x30;	//ECS129
		else return FALSE;
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] & DC_DM_DISABLE_RECT;
		nTemp >>= POS_DM_DISABLE_RECT;
		if(nTemp >= 0 && nTemp < 2) m_chSendBytes[11] = nTemp + 0x30;	//矩形码
		else return FALSE;
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] & DC_DM_ADD_PAD_CODE;
		nTemp >>= POS_DM_ADD_PAD_CODE;
		if(nTemp >= 0 && nTemp < 2) m_chSendBytes[12] = nTemp + 0x30;	//PAD
		m_nSend = 13;
		break;

	case POS_HAN_SENSIBLE:
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] & DC_CSC_DOUBLE;
		nTemp >>= POS_CSC_DOUBLE0;
		if(nTemp >= 0 && nTemp < 3) m_chSendBytes[7] = nTemp + 0x30;//双码设定 0--single, 1--trueDouble,2---both
		else return FALSE;
		
		nTemp = X_paramGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] & DC_CSC_Reverse;
		nTemp >>= POS_CSC_Reverse0;
		if(nTemp >= 0 && nTemp < 3) m_chSendBytes[8] = nTemp + 0x30;//正反相码设定: 0--正相, 1:反相, 2:both
		else return FALSE;
		m_nSend = 9;
		break;

	default : 
		return FALSE;
	}
	
	return TRUE;
}


//邮政码参数查询
BOOL bPostalParamQuery(void)
{
	unsigned int nParam;
	//	BOOL IsFinish = FALSE;
	//	unsigned char *p;
	//	int i;
	unsigned int Param;
	
    nParam = (int)(m_chReceiveBytes[6] - 0x30) * 10; //获得条码类型
	nParam += (int)(m_chReceiveBytes[7] - 0x30);
	m_chSendBytes[3] = 0x05;
	m_chSendBytes[5] = m_chReceiveBytes[6];
	m_chSendBytes[6] = m_chReceiveBytes[7];
	m_chSendBytes[7] = 0;
    m_chSendBytes[8] = 0;
	
	nParam -= 0x60;
	switch(nParam) {
	case POS_POSTNET:
	case POS_PLANET:
		memcpy(&Param, &(X_paramGetGlobalAllParam()->pPostal->ExMsg[nParam]), 4);
		m_chSendBytes[7] = 0x30;
		m_chSendBytes[8] = 0x30;
		m_chSendBytes[9] = 0x30;
		m_chSendBytes[10] = (X_paramGetGlobalAllParam()->pPostal->ExMsg[nParam] & 0x0f) | 0x30;
		m_nSend = 11;
		break;
	default : 
		return FALSE;
	}
	return TRUE;
}

//解码模式查询
BOOL bDecodeModeQuery(void)
{
    int temp;
	unsigned int value;
//	int i;
	
	m_chSendBytes[3] = 0x05;
    m_chSendBytes[5] = m_chReceiveBytes[6];
	m_chSendBytes[6] = m_chReceiveBytes[7];
	temp = (m_chSendBytes[5] - '0') * 10 + (m_chSendBytes[6] - '0');
	switch(temp) {
	case 0: //
		if(X_paramGetGlobalAllParam()->pCode->nReadMode == ESC0)
		    m_chSendBytes[7] = 0x30;
		else if(X_paramGetGlobalAllParam()->pCode->nReadMode == ESC2)
		    m_chSendBytes[7] = 0x31;
		else if(X_paramGetGlobalAllParam()->pCode->nReadMode == ESC3)
		    m_chSendBytes[7] = 0x32;
		else return FALSE;
		m_nSend = 8;
		break;
	case 1: //图象滤波查询
		if(X_paramGetGlobalAllParam()->pCode->Protect == 1) m_chSendBytes[7] = 0x31;
		else m_chSendBytes[7] = 0x30;
		m_nSend = 8;
		break;
	case 2: //灵敏度查询
		m_chSendBytes[7] = 0x31;
		sprintf(m_chSendBytes+8,"%02d",X_paramGetGlobalAllParam()->pCode->nSensitive);//该处不需要置换：uImg_Int2CharReverse()...
		m_nSend = 10;
		break;
	case 3: //延迟查询
		if(m_chReceiveBytes[8] == '0'){//一次读码延迟查询
			m_chSendBytes[7] = 0x30;
			m_chSendBytes[8] = 0x30;
			value = X_paramGetGlobalAllParam()->pCode->nOnceDelay;
		}
		else if(m_chReceiveBytes[8] == '1'){
			//两次读码类型信息相同延迟查询
		    m_chSendBytes[7] = 0x31;
			if(!X_paramGetGlobalAllParam()->pCode->IsCompDelay) m_chSendBytes[8] = 0x30;
		    else m_chSendBytes[8] = 0x31;
			value = X_paramGetGlobalAllParam()->pCode->nInfoDelay;
		}
		else return FALSE;

		sprintf(m_chSendBytes+9,"%07d",value);//该处不需要置换：uImg_Int2CharReverse()...
		m_nSend = 16;
		break;
	case 4: //解码次数限定查询
		sprintf(m_chSendBytes+7,"%05d",X_paramGetGlobalAllParam()->pCode->nScanTimes);
		m_nSend = 12;
		break;

	case 5: //一定时间内没读到码返回信息查询
		m_chSendBytes[7] = X_paramGetGlobalAllParam()->pCode->bErrMsgEn + '0'; //使能
		sprintf(m_chSendBytes+7,"%01d%02d%s",(int)X_paramGetGlobalAllParam()->pCode->bErrMsgEn,X_paramGetGlobalAllParam()->pCode->nErrMsg,X_paramGetGlobalAllParam()->pCode->byErrMsg);
		m_nSend = 10 + X_paramGetGlobalAllParam()->pCode->nErrMsg;
		break;
	case 6:	//CMOS曝光模式
		m_chSendBytes[7] = X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode + '0';
		m_nSend = 8;
		break;

	case 7:	//读码区域控制
		m_chSendBytes[7] = (X_paramGetGlobalAllParam()->pCode->decodeArea & 0x1) + '0';
		temp = (X_paramGetGlobalAllParam()->pCode->decodeArea >>4) & 0xff;
		if (temp == 0)
			temp = 20;
		m_chSendBytes[8] = (temp/100) + '0';
		m_chSendBytes[9] = ((temp%100)/10) + '0';
		m_chSendBytes[10] = (temp%10) + '0';
		m_nSend = 11;
		break;

	default: return FALSE;
	}
	m_chSendBytes[m_nSend] = 0;
	return TRUE;
}

//除了前后缀外的其它数据格式查询
BOOL bOtherFormatQuery(void)
{
	int temp;
//	unsigned int value;

	m_chSendBytes[3] = 0x05;
    m_chSendBytes[5] = m_chReceiveBytes[6];
	m_chSendBytes[6] = m_chReceiveBytes[7];
	temp = (m_chSendBytes[5] - '0') * 10 + (m_chSendBytes[6] - '0');

	if(temp == 0){//数据打包查询
		sprintf(m_chSendBytes+7,"%02d",X_paramGetGlobalAllParam()->pCode->nPack);
		m_nSend = 9;
	}
	m_chSendBytes[m_nSend] = 0;
	return TRUE;
}

//产品相关参数综合查询
BOOL bOverAllQuery(void)
{
	char buff[100];
	int temp;
	unsigned int value;

	memset(buff,0,100);
	m_chSendBytes[3] = 0x05;
    m_chSendBytes[5] = m_chReceiveBytes[6];
	m_chSendBytes[6] = m_chReceiveBytes[7];
	temp = (m_chSendBytes[5] - '0') * 10 + (m_chSendBytes[6] - '0');

	switch(temp) {
	case 0: //电源开启查询
		//是否显示信息
		if(X_paramGetGlobalAllParam()->pCode->IsPowerOnDisp == 1) m_chSendBytes[7] = 0x31;
		else m_chSendBytes[7] = 0x30;
		//开机进入控制台查询
		if(X_paramGetGlobalAllParam()->pCode->IsGotoConsole == 1) m_chSendBytes[8] = 0x31;
		else m_chSendBytes[8] = 0x30;
		m_nSend = 9;
		break;
	case 1: //一维条码双码解码查询
		value = X_paramGetGlobalAllParam()->p1D->unDecParam & 0x03;
		if(value == 0 || value == 1)
		    m_chSendBytes[7] = 0x30;
		else
			m_chSendBytes[7] = (value - 1) + 0x30;
		m_nSend = 8;
		break;
	case 2: //产品ESN号查询
		value = strlen((char*)(X_paramGetGlobalAllParam()->pDevInfo->pESN));
		if (value >= 40) value = 0;
		sprintf(m_chSendBytes+7,"%02d",value);
		memcpy(m_chSendBytes + 9, (char*)X_paramGetGlobalAllParam()->pDevInfo->pESN, value);
		m_nSend = 9 + value;
		break;
	case 3: //产品S/N号查询
		value = strlen((char*)(X_paramGetGlobalAllParam()->pDevInfo->pSN));
		if (value >= 40) value = 0;
		sprintf(m_chSendBytes+7,"%02d",value);
		memcpy(m_chSendBytes + 9, (char*)X_paramGetGlobalAllParam()->pDevInfo->pSN, value);
		m_nSend = 9 + value;
		break;
	case 4: //产品出厂日期查询
		value = strlen((char*)(X_paramGetGlobalAllParam()->pDevInfo->pDate));
		if (value >= 20) value = 0;
		sprintf(m_chSendBytes+7,"%02d",value);
		memcpy(m_chSendBytes + 9, (char*)X_paramGetGlobalAllParam()->pDevInfo->pDate, value);
		m_nSend = 9 + value;
		break;
	case 5: //UID查询
        //查询函数
		value = strlen((char*)(X_paramGetGlobalAllParam()->pDevInfo->pUID));
		if (value >= 20) value = 0;
		sprintf(m_chSendBytes+7,"%02d",value);
		memcpy(m_chSendBytes + 9, (char*)X_paramGetGlobalAllParam()->pDevInfo->pUID, value);
		m_nSend = 9 + value;
		break;
	
	case 6: //licence查询
		value = strlen((char*)(X_paramGetGlobalAllParam()->pDevInfo->pLIC));
		if (value >= 20) value = 0;
		sprintf(m_chSendBytes+7,"%02d",value);
		memcpy(m_chSendBytes + 9, (char*)X_paramGetGlobalAllParam()->pDevInfo->pLIC, value);
		m_nSend = 9 + value;
		break;
	case 7: //ManufactureID查询
		value = strlen((char*)(X_paramGetGlobalAllParam()->pDevInfo->pManufID));
		if (value >= 20) value = 0;
		sprintf(m_chSendBytes+7,"%02d",value);
		memcpy(m_chSendBytes + 9, (char*)X_paramGetGlobalAllParam()->pDevInfo->pManufID, value);
		m_nSend = 9 + value;
		break;
	case 8: //IsDownload查询
		value = (unsigned int)(X_paramGetGlobalAllParam()->pDevInfo->IsDownload);
		if (value > 1) value = 0;
		m_chSendBytes[7] = 0x00;
		m_chSendBytes[8] = 0x01;
		m_chSendBytes[9] = (char)value;
		m_nSend = 10;
		break;
	default: 
		return FALSE;
	}

	m_chSendBytes[m_nSend] = 0;
	return TRUE;
}

void vCode128ParamSet(void)
{
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] &= 0xffff0000;
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] |= GetHex(m_chReceiveBytes[12]);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] |= (GetHex(m_chReceiveBytes[11]) << 4);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] |= (GetHex(m_chReceiveBytes[10]) << 8);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] |= (GetHex(m_chReceiveBytes[9]) << 12);

      X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] &= 0xffff0000;
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] |= GetHex(m_chReceiveBytes[12]);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] |= (GetHex(m_chReceiveBytes[11]) << 4);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] |= (GetHex(m_chReceiveBytes[10]) << 8);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] |= (GetHex(m_chReceiveBytes[9]) << 12);
}

void vEAN8ParamSet(void)
{
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] = 0;
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= GetHex(m_chReceiveBytes[12]);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= (GetHex(m_chReceiveBytes[11]) << 4);

      X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] = 0;
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= GetHex(m_chReceiveBytes[12]);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= (GetHex(m_chReceiveBytes[11]) << 4);
}

void vEAN13ParamSet(void)
{
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] = 0;
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] |= GetHex(m_chReceiveBytes[12]);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] |= (GetHex(m_chReceiveBytes[11]) << 4);

      X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] = 0;
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] |= GetHex(m_chReceiveBytes[12]);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] |= (GetHex(m_chReceiveBytes[11]) << 4);
}

void vUPCEParamSet(void)
{
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] = 0;
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= GetHex(m_chReceiveBytes[12]);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= (GetHex(m_chReceiveBytes[11]) << 4);

      X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] = 0;
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= GetHex(m_chReceiveBytes[12]);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= (GetHex(m_chReceiveBytes[11]) << 4);
}

void vUPCAParamSet(void)
{
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] = 0;
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= GetHex(m_chReceiveBytes[12]);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= (GetHex(m_chReceiveBytes[11]) << 4);

      X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] = 0;
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= GetHex(m_chReceiveBytes[12]);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= (GetHex(m_chReceiveBytes[11]) << 4);
}

BOOL bI2of5ParamSet(void)
{
	int Begin,End;
	int i;
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= 0xffff0000;
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= GetHex(m_chReceiveBytes[12]);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= (GetHex(m_chReceiveBytes[11]) << 4);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= (GetHex(m_chReceiveBytes[10]) << 8);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= (GetHex(m_chReceiveBytes[9]) << 12);

      X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= 0xffff0000;
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= GetHex(m_chReceiveBytes[12]);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= (GetHex(m_chReceiveBytes[11]) << 4);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= (GetHex(m_chReceiveBytes[10]) << 8);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= (GetHex(m_chReceiveBytes[9]) << 12);
	
	//打开或关闭固定长度
	if(m_chReceiveBytes[11] & 0x01)
      {
        X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[0] = 4;
        X_paramGetGlobalAllParam()->p1D->I2Of5Len[0] = 4;
      }
	else
      {
        X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[0] = 0;
        X_paramGetGlobalAllParam()->p1D->I2Of5Len[0] = 0;
      }
	Begin = (GetHex(m_chReceiveBytes[14]) << 4) + GetHex(m_chReceiveBytes[15]);
	End = (GetHex(m_chReceiveBytes[16]) << 4) + GetHex(m_chReceiveBytes[17]);
	
	if(Begin == 0 && End == 0) return TRUE;
	if((Begin > 64) || (End > 64) || (Begin > End)) return FALSE;
	if(End & 0x01) End--;

	if(Begin < 2) Begin = 2;
	
	//添加或删减固定长度值
	if(m_chReceiveBytes[13] & 0x01)
      {
		for(i = Begin; i <= End; i++)
            {
		    if(i < 18) 
                 {
                    X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[1] |= (0x01 << (i / 2 - 1));
                    X_paramGetGlobalAllParam()->p1D->I2Of5Len[1] |= (0x01 << (i / 2 - 1));
                 }
		    else if(i < 34) 
                {
                    X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[2] |= (0x01 << (i / 2 - 9));
                    X_paramGetGlobalAllParam()->p1D->I2Of5Len[2] |= (0x01 << (i / 2 - 9));
                }
		   else if(i < 50)
                { 
                    X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[3] |= (0x01 << (i / 2 - 17));
                    X_paramGetGlobalAllParam()->p1D->I2Of5Len[3] |= (0x01 << (i / 2 - 17));
                }
		   else 
                {
                    X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[4] |= (0x01 << (i / 2 - 25));
                    X_paramGetGlobalAllParam()->p1D->I2Of5Len[4] |= (0x01 << (i / 2 - 25));
                }
		}
	}
	else
      {
		for(i = Begin; i <= End; i++)
            {
		    if(i < 18) 
                 {
                    X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[1] &= ~(0x01 << (i / 2 - 1));
                    X_paramGetGlobalAllParam()->p1D->I2Of5Len[1] &= ~(0x01 << (i / 2 - 1));
                  }
		    else if(i < 34)
                 { 
                    X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[2] &= ~(0x01 << (i / 2 - 9));
                    X_paramGetGlobalAllParam()->p1D->I2Of5Len[2] &= ~(0x01 << (i / 2 - 9));
                 }
		    else if(i < 50) 
                {
                    X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[3] &= ~(0x01 << (i / 2 - 17));
                    X_paramGetGlobalAllParam()->p1D->I2Of5Len[3] &= ~(0x01 << (i / 2 - 17));
                }
		   else
               { 
                    X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[4] &= ~(0x01 << (i / 2 - 25));
                    X_paramGetGlobalAllParam()->p1D->I2Of5Len[4] &= ~(0x01 << (i / 2 - 25));
               }
		}
	}
	return TRUE;
}

void vCode39ParamSet(void)
{
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= 0xffff0000;
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= GetHex(m_chReceiveBytes[12]);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= (GetHex(m_chReceiveBytes[11]) << 4);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= (GetHex(m_chReceiveBytes[10]) << 8);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= (GetHex(m_chReceiveBytes[9]) << 12);

      X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= 0xffff0000;
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= GetHex(m_chReceiveBytes[12]);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= (GetHex(m_chReceiveBytes[11]) << 4);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= (GetHex(m_chReceiveBytes[10]) << 8);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= (GetHex(m_chReceiveBytes[9]) << 12);
}

void vCodabarParamSet(void)
{
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= 0xffff0000;
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= GetHex(m_chReceiveBytes[12]);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= (GetHex(m_chReceiveBytes[11]) << 4);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= (GetHex(m_chReceiveBytes[10]) << 8);
	X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= (GetHex(m_chReceiveBytes[9]) << 12);

      X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= 0xffff0000;
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= GetHex(m_chReceiveBytes[12]);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= (GetHex(m_chReceiveBytes[11]) << 4);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= (GetHex(m_chReceiveBytes[10]) << 8);
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= (GetHex(m_chReceiveBytes[9]) << 12);
}

void vPDF417ParamSet(void)
{
	int Temp = 0;

	Temp = m_chReceiveBytes[9] - 0x30;
	if(Temp == 0)
      {
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_DOUBLE);
             X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_DOUBLE);
	}
	else if(Temp == 1)
      {
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (1<<POS_PDF417_DOUBLE0);
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~(1<<POS_PDF417_DOUBLE1));
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_DOUBLE);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (1<<POS_PDF417_DOUBLE0);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (1<<POS_PDF417_DOUBLE1);

             X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_DOUBLE);
		X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (1<<POS_PDF417_DOUBLE0);
		X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (1<<POS_PDF417_DOUBLE1);
	}
	else if(Temp == 2){
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= DC_PDF417_DOUBLE;
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~(1<<POS_PDF417_DOUBLE0));
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_DOUBLE);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (1<<POS_PDF417_DOUBLE1);

             X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_DOUBLE);
		X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (1<<POS_PDF417_DOUBLE1);
	}
}

void vQRParamSet(void)
{
	int Temp = 0;

	Temp = m_chReceiveBytes[9] - 0x30;
	if(Temp == 0){
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~DC_QR_DOUBLE);
            X_paramGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~DC_QR_DOUBLE);
	}
	else if(Temp == 1){
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] |= (1<<POS_QR_DOUBLE0);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~(1<<POS_QR_DOUBLE1));

            X_paramGetGlobalAllParam()->p2D->ExParam[POS_QR] |= (1<<POS_QR_DOUBLE0);
		X_paramGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~(1<<POS_QR_DOUBLE1));
	}
	else if(Temp == 2){
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] |= DC_QR_DOUBLE;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~(1<<POS_QR_DOUBLE0));

            X_paramGetGlobalAllParam()->p2D->ExParam[POS_QR] |= DC_QR_DOUBLE;
		X_paramGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~(1<<POS_QR_DOUBLE0));
	}
}

void vAztecParamSet(void)
{
	int Temp = 0;

	Temp = m_chReceiveBytes[9] - 0x30;
	if(Temp == 0){
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
            X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
	}
	else if(Temp == 1){
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= (1<<POS_AZTEC_MULTIEN0);

            X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
		X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= (1<<POS_AZTEC_MULTIEN0);
	}
	else if(Temp == 2){
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= (1<<POS_AZTEC_MULTIEN1);

            X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
		X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= (1<<POS_AZTEC_MULTIEN1);

	}
	else return;

	Temp = m_chReceiveBytes[10] - 0x30;
	if(Temp >= 0 && Temp < 8){
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTINUM);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= Temp;

            X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTINUM);
		X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= Temp;
	}
}

void vDataMatrixParamSet(void)
{
	int Temp = 0;

	Temp = m_chReceiveBytes[9] - 0x30;
	if(Temp == 0){
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_DOUBLE);
             X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_DOUBLE);
	}
	else if(Temp == 1){
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= (1<<POS_DM_DOUBLE0);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~(1<<POS_DM_DOUBLE1));

            X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= (1<<POS_DM_DOUBLE0);
		X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~(1<<POS_DM_DOUBLE1));
	}
	else if(Temp == 2){
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= DC_DM_DOUBLE;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~(1<<POS_DM_DOUBLE0));

            X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= DC_DM_DOUBLE;
		X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~(1<<POS_DM_DOUBLE0));
	}
}

//解码模式设置
BOOL bDecModeParamSet(void)
{
	int nType;
	int temp,Value;
	int i;

	nType = (m_chReceiveBytes[7] - '0') * 10 + (m_chReceiveBytes[8] - '0');
	if((nType != 3) && (m_nReceive != 16)) return FALSE;
	if((nType == 3) && (m_nReceive != 19)) return FALSE;
	
	switch(nType) {
	case 0: //扫描模式设置
		Value = m_chReceiveBytes[9] - '0';
		if(Value == 0){ //手动读码
			X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC0;
		    X_paramBakGetGlobalAllParam()->pMode->bContinue = TRUE;

                X_paramGetGlobalAllParam()->pCode->nReadMode = ESC0;
		    X_paramGetGlobalAllParam()->pMode->bContinue = TRUE;
		}
		else if(Value == 1){ //自动读码
			X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC2;
		    X_paramBakGetGlobalAllParam()->pMode->bContinue = TRUE;
	    	X_paramBakGetGlobalAllParam()->pMode->nReadDelay = Z_Get10msCounter();
		    X_paramBakGetGlobalAllParam()->pMode->nHaltDelay = Z_Get10msCounter();
		    X_paramBakGetGlobalAllParam()->pMode->nCodeDelay = Z_Get10msCounter();

                X_paramGetGlobalAllParam()->pCode->nReadMode = ESC2;
		    X_paramGetGlobalAllParam()->pMode->bContinue = TRUE;
	    	X_paramGetGlobalAllParam()->pMode->nReadDelay = Z_Get10msCounter();
		    X_paramGetGlobalAllParam()->pMode->nHaltDelay = Z_Get10msCounter();
		    X_paramGetGlobalAllParam()->pMode->nCodeDelay = Z_Get10msCounter();
		}
		else if(Value == 2){ //连续读码
			if(X_paramBakGetGlobalAllParam()->pCode->nReadMode == ESC3)
			{
				X_paramBakGetGlobalAllParam()->pMode->bContinue = !X_paramBakGetGlobalAllParam()->pMode->bContinue;
			}
			else
			{ X_paramBakGetGlobalAllParam()->pMode->bContinue = TRUE; }
		    X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC3;

                if(X_paramGetGlobalAllParam()->pCode->nReadMode == ESC3)
			{
				X_paramGetGlobalAllParam()->pMode->bContinue = !X_paramGetGlobalAllParam()->pMode->bContinue;
			}
			else
			{ X_paramGetGlobalAllParam()->pMode->bContinue = TRUE; }
		    X_paramGetGlobalAllParam()->pCode->nReadMode = ESC3;
		}
		else if(Value == 9){ //测试
			temp = m_chReceiveBytes[10] - '0';
			i = (m_chReceiveBytes[11]-'0')*1000 +  (m_chReceiveBytes[12]-'0')*100 
				+ (m_chReceiveBytes[13]-'0')*10 + m_chReceiveBytes[14]-'0';
			if(temp == 0)
			{
				if(i <= 0)
					U_appSetTestMode(1, 1000);//按次数测试
				else
					U_appSetTestMode(1, i*10);
			}
			else if(temp == 1)
			{
				if(i <= 0)
					U_appSetTestMode(2, 60*1000);//按时间测试
				else
					U_appSetTestMode(2, i*1000);
			}
			else if(temp == 2)
			{
				if(i <= 0)
					U_appRunDecodeTest(3, 1000);		//按成功次数测试
				else
					U_appRunDecodeTest(3, i*10);
			}
			else if(temp == 3)
			{
				if(i <= 0)
					U_appSetTestMode(4, 10000);		//误码率测试：10000次
				else
					U_appSetTestMode(4, i*10);	
			}
// 			else if (temp == 9)
// 			{
// 				if (i == 0)
// 					U_appGetDefinition(TRUE);		//测试镜头采集的图像的清晰度并发送
// 				else
// 					U_appGetDefinition(FALSE);		//测试镜头采集的图像的清晰度并发送
// 			}
		}
		else return FALSE;
		break;
	case 1: //图象滤波设置
		Value = m_chReceiveBytes[9] - '0';
		if(Value == 0 || Value == 1)
		{    
		    X_paramBakGetGlobalAllParam()->pCode->Protect = Value;
                 X_paramGetGlobalAllParam()->pCode->Protect = Value;
             }
		else return FALSE;
		break;
	case 2: //灵敏度设置
		if(m_nReceive != 16) return FALSE;
		if(m_chReceiveBytes[9] == '0'){ //采用级别设置
			Value = m_chReceiveBytes[10] - '0';
			if(Value < 4 && Value >= 0) //目前只提供4种级别
			{	
			    X_paramBakGetGlobalAllParam()->pCode->nSensitive = SENSITIVE_CALC(Value);
                       X_paramGetGlobalAllParam()->pCode->nSensitive = SENSITIVE_CALC(Value);
                   }
			else return FALSE;
		}
		else{ //直接设置值
			Value = (m_chReceiveBytes[10] - '0') * 10 + (m_chReceiveBytes[11] - '0');
			if(Value > 20 || Value < 0) return FALSE;
			X_paramBakGetGlobalAllParam()->pCode->nSensitive = Value;
                   X_paramGetGlobalAllParam()->pCode->nSensitive = Value;
		}
		break;
	case 3: //延迟设置
		Value = m_chReceiveBytes[11] - '0';
		Value = Value * 10 + (m_chReceiveBytes[12] - '0');
		Value = Value * 10 + (m_chReceiveBytes[13] - '0');
		Value = Value * 10 + (m_chReceiveBytes[14] - '0');
		Value = Value * 10 + (m_chReceiveBytes[15] - '0');
		Value = Value * 10 + (m_chReceiveBytes[16] - '0');
		Value = Value * 10 + (m_chReceiveBytes[17] - '0');
		
		if(Value > 3600000) return FALSE;
		temp = m_chReceiveBytes[9] - '0';
		if(temp == 0)//一次读码延迟设置
		{	
		    X_paramBakGetGlobalAllParam()->pCode->nOnceDelay = Value;
                 X_paramGetGlobalAllParam()->pCode->nOnceDelay = Value;
            }
		else if(temp == 1){ //两次读码类型信息相同延迟设置
			X_paramBakGetGlobalAllParam()->pCode->nInfoDelay = Value;
			if((m_chReceiveBytes[10] == '0') || (m_chReceiveBytes[10] == '1'))
			    X_paramBakGetGlobalAllParam()->pCode->IsCompDelay = m_chReceiveBytes[10] - '0';
			else return FALSE;
		}
		else return FALSE;
		break;
	case 4: //限定读码的次数设置
		Value = m_chReceiveBytes[9] - '0';
		Value = Value * 10 + (m_chReceiveBytes[10] - '0');
		Value = Value * 10 + (m_chReceiveBytes[11] - '0');
		Value = Value * 10 + (m_chReceiveBytes[12] - '0');
		Value = Value * 10 + (m_chReceiveBytes[13] - '0');
		
		if(Value > 65535) return FALSE;
		X_paramBakGetGlobalAllParam()->pCode->nScanTimes = Value;
		break;
	case 5: //一定时间内没读到条码信息设置
		Value = m_chReceiveBytes[9] - '0';
		if(Value == 0){//设置使能
			temp = m_chReceiveBytes[10] - '0';
			if(temp != 0 && temp != 1) return FALSE;
			X_paramBakGetGlobalAllParam()->pCode->bErrMsgEn = temp;
		}
		else {//设置信息
			memset(X_paramBakGetGlobalAllParam()->pCode->byErrMsg, 0, 10);
			temp = (int)(m_chReceiveBytes[10] - '0') * 10 + (int)(m_chReceiveBytes[11] - '0'); //长度
			if(temp > 10) return FALSE;
			for(i = 0; i < temp; i++){
				X_paramBakGetGlobalAllParam()->pCode->byErrMsg[i] = (unsigned char)m_chReceiveBytes[12 + i];
			}
			X_paramBakGetGlobalAllParam()->pCode->nErrMsg = temp;
		}
		break;
	case 6:
		Value = m_chReceiveBytes[9] - '0';
		if (Value == 0)
		{
			X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode = 0;
		}
// 		else if (Value == 1)
// 		{
// 			X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = 1;
// 		}
// 		else if (Value == 2)
// 		{
// 			X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = 4;
// 		}
// 		else if (Value == 3)
// 		{
// 			X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = 3;
// 		}
		else if (Value == 4)
		{
			X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode = 2;
		}
		else return FALSE;

		Z_SetLightMode(X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode);
		break;

	default: return FALSE;
	}

	return TRUE;
}

//一维条码参数设置
BOOL b1DParamSet(void)
{

	int nType;
	
	nType = (m_chReceiveBytes[7] - '0') * 10 + (m_chReceiveBytes[8] - '0');
	if((nType != POS_INTER2OF5) && (m_nReceive != 16)) return FALSE;
	if((nType == POS_INTER2OF5) && (m_nReceive != 19)) return FALSE;
	
	switch(nType) {
	case POS_ZASETUP: //ZA设置码
		if(GetHex(m_chReceiveBytes[12]) & 0x01) X_paramBakGetGlobalAllParam()->pCode->SetCode = 1;
		else X_paramBakGetGlobalAllParam()->pCode->SetCode = 0;
		if(GetHex(m_chReceiveBytes[12]) & 0x02)
		{ W_setcEnable(TRUE); }
		else
		{ W_setcEnable(FALSE); }
		break;
	case POS_CODE128: //Code 128
		vCode128ParamSet();
		break;
	case POS_EAN8: //EAN-8
		vEAN8ParamSet();
		break;
	case POS_EAN13: //EAN-13
		vEAN13ParamSet();
		break;
	case POS_UPCE: //UPC-E
		vUPCEParamSet();
		break;
	case POS_UPCA: //UPC-A
		vUPCAParamSet();
		break;
	case POS_INTER2OF5: //I2of5
		if(!bI2of5ParamSet()) return FALSE;
		break;
	case POS_REGULAR39: //Code 39
		vCode39ParamSet();
		break;
	case POS_CODABAR: //Codabar
		vCodabarParamSet();
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

//二维条码参数设置
BOOL b2DParamSet(void)
{
	int nType;
	
	nType = (m_chReceiveBytes[7] - '0') * 10 + (m_chReceiveBytes[8] - '0');
	if(m_nReceive != 16) return FALSE;
	
	switch(nType - 0x20) {
	case POS_PDF417:
		vPDF417ParamSet();
		break;

	case POS_QR:
		vQRParamSet();
		break;

	case POS_Aztec:
		vAztecParamSet();
		break;

	case POS_DataMatrix:
		vDataMatrixParamSet();
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

//综合参数设置
BOOL bOverallParamSet(void)
{
	int nType,Value;
	unsigned char pBuff[128];

	memset(pBuff,0,128);
	nType = (m_chReceiveBytes[7] - '0') * 10 + (m_chReceiveBytes[8] - '0');

	switch(nType) {
	case 0: //电源开启时是否显示系统信息设置
		Value = m_chReceiveBytes[9] - '0';
		if(Value == 0 || Value == 1) X_paramBakGetGlobalAllParam()->pCode->IsPowerOnDisp = Value;
		else if(Value == 2 || Value == 3) X_paramBakGetGlobalAllParam()->pCode->IsGotoConsole = Value - 2;
		else return FALSE;
		break;
	case 1: //一维条码双码解码设置
		Value = m_chReceiveBytes[9] - '0';
		if(Value == 0){
			X_paramBakGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
		}
		else if(Value == 1){
			X_paramBakGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
	    	X_paramBakGetGlobalAllParam()->p1D->unDecParam |= 0x02; 
		}
		else if(Value == 2){
			X_paramBakGetGlobalAllParam()->p1D->unDecParam |= 0x03;
		}
		else
			return FALSE;
		break;
	case 2: //设置产品ESN号
		Value = (m_chReceiveBytes[9] - '0') * 10 + (m_chReceiveBytes[10] - '0');
		if(Value != (m_nReceive - 12)) return FALSE;
		if(Value > 20) return FALSE;//最多20字节
		memcpy((char*)pBuff, m_chReceiveBytes + 11, m_nReceive - 12);//用减12是因为后面有一个是校验
		X_paramPlaceKeyword((char*)"ESN",(char*)pBuff,TRUE);
		X_paramSaveDeviceInfo();
	    break;
	case 8: //设置产品IsDownload参数
		Value = (m_chReceiveBytes[9] - '0') * 10 + (m_chReceiveBytes[10] - '0');
		if(Value != 1) return FALSE;
		pBuff[0] = m_chReceiveBytes[11];
		X_paramPlaceKeyword((char*)"IsDownload", (char*)pBuff,TRUE);
		X_paramSaveDeviceInfo();
	    break;
	default: return FALSE;
	}
	
	return TRUE;
}

//除了前后缀的其它数据格式设置
BOOL bOtherDataFormatSet(void)
{
	int nType,nValue;

	if(m_nReceive != 16) return FALSE;
	nType = (m_chReceiveBytes[7] - '0') * 10 + (m_chReceiveBytes[8] - '0');
	if(nType == 0){ //数据打包设置
		nValue = (m_chReceiveBytes[9] - '0') * 10 + (m_chReceiveBytes[10] - '0');
		if(nValue >= 0 && nValue < 5) //目前只支持5种特殊打包
			X_paramBakGetGlobalAllParam()->pCode->nPack = nValue;
		else return FALSE;
	}
	else return FALSE;
	return TRUE;
}

//OCR使能与禁止设置
BOOL bOCREnableSet(void)
{
	int nTmp;
	int nParam;

	if (m_nReceive != 16) return FALSE;
	nTmp = (int)(m_chReceiveBytes[7] - '0');
	nParam  = (m_chReceiveBytes[10] - '0') * 10000;
	nParam += (m_chReceiveBytes[11] - '0') * 1000;
	nParam += (m_chReceiveBytes[12] - '0') * 100;
	nParam += (m_chReceiveBytes[13] - '0') * 10;
	nParam += (m_chReceiveBytes[14] - '0') * 1;

	if(nTmp == 0){ //禁止
		if(m_chReceiveBytes[8] == '0')			
			X_paramBakGetGlobalAllParam()->pOCR->unDecTypes &= (~nParam);
		else X_paramBakGetGlobalAllParam()->pOCR->unDecTypes &= (~(nParam << 16));
	}
	else if(nTmp == 1){ //使能
		if(m_chReceiveBytes[8] == '0')
			X_paramBakGetGlobalAllParam()->pOCR->unDecTypes |= nParam;
		else X_paramBakGetGlobalAllParam()->pOCR->unDecTypes |= (nParam << 16);
	}
	else return FALSE;

    return TRUE;
}

//延迟，重复读码/时间，数据是否打包设置等
void vContinueDelayAndSoOnSet(void)
{
	int nParam;
	
	nParam  = (m_chReceiveBytes[10] - '0') * 10000;
	nParam += (m_chReceiveBytes[11] - '0') * 1000;
	nParam += (m_chReceiveBytes[12] - '0') * 100;
	nParam += (m_chReceiveBytes[13] - '0') * 10;
	nParam += (m_chReceiveBytes[14] - '0') * 1;

	if (m_chReceiveBytes[8] == '1')	nParam <<= 16;

	switch(m_chReceiveBytes[9])
	{
	case '0'://重复读取: LED Always ON 模式
		if (nParam == 0) return;
		if (nParam & 0x04){
			if(m_chReceiveBytes[7] == '0'){
				X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC0;
			}
			else if(m_chReceiveBytes[7] == '1')	{
				X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC2;
			}
			else if(m_chReceiveBytes[7] == '2'){
				X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC3;
			}
			else return;
		}
		else return;

		break;
	case '1': //延迟
		X_paramBakGetGlobalAllParam()->pCode->nOnceDelay = nParam;
		break;
	case '2'://ESC2延迟
		if(nParam > 3600000)
			return;
		X_paramBakGetGlobalAllParam()->pCode->nInfoDelay = nParam;
		break;
	case '3':
		X_paramBakGetGlobalAllParam()->pCode->nSuccDelay = nParam; 
		break;
	case'4': //信息数据是否需要打包
		if (nParam != 0) return;

#if !defined(MB1126)
		if(m_chReceiveBytes[7] == '0') X_paramBakGetGlobalAllParam()->pCode->nPack = FALSE;
		else if(m_chReceiveBytes[7] == '1')	X_paramBakGetGlobalAllParam()->pCode->nPack = FACTORY_PACK;
		else if(m_chReceiveBytes[7] == '2')	X_paramBakGetGlobalAllParam()->pCode->nPack = ECS_BANK_PACK;
		else if(m_chReceiveBytes[7] == '3')	X_paramBakGetGlobalAllParam()->pCode->nPack = BLUE2D_PACK;
		else return;
#endif //#if !defined(MB1126)

		break;
	case '6':
		if(m_chReceiveBytes[7] == '0') X_paramBakGetGlobalAllParam()->pHard->bDecVoice &= ~1;
		else if(m_chReceiveBytes[7] == '1')	X_paramBakGetGlobalAllParam()->pHard->bDecVoice |= 1;
		else return ;
		break;
	default:
		break;
	}

	m_chSendBytes[5] = 0x30;

	return;
}
//读码信息长度范围设置
void vMsgLenSet(void)
{
	char *p;
	int nParam,MinLen,MaxLen;
	int v;

	p = m_chReceiveBytes + 7;
	nParam = (int)(p[0] - 0x30) * 10 + (int)(p[1] - 0x30);
	if((GetHex(p[2]) != -1) && (GetHex(p[3]) != -1) 
		&& (GetHex(p[4]) != -1) && (GetHex(p[5]) != -1)){
		MinLen = GetHex(p[2]) << 12;
		MinLen += (GetHex(p[3]) << 8);
		MinLen += (GetHex(p[4]) << 4);
		MinLen += GetHex(p[5]);
	}
	else return;
	
	if((GetHex(p[6]) != -1) && (GetHex(p[7]) != -1) 
		&& (GetHex(p[8]) != -1) && (GetHex(p[9]) != -1)){
		MaxLen = GetHex(p[6]) << 12;
		MaxLen += (GetHex(p[7]) << 8);
		MaxLen += (GetHex(p[8]) << 4);
		MaxLen += GetHex(p[9]);
	}
	else return;

	if (MinLen > MaxLen) return ;

	if(nParam < MAX_1D_NUM){
		if(MaxLen < 0x80 && MinLen > 0){ //一维条码的设置范围是1--127
			X_paramBakGetGlobalAllParam()->p1D->ExMsg[nParam] &= 0x0000ffff;
			X_paramBakGetGlobalAllParam()->p1D->ExMsg[nParam] |= MaxLen << 24;
			X_paramBakGetGlobalAllParam()->p1D->ExMsg[nParam] |= MinLen << 16;
		}
		else
			return;
	}
	else if (nParam >= MAX_1D_NUM + MAX_2D_NUM) // OCR
	{
		v = nParam-(MAX_1D_NUM + MAX_2D_NUM);
		if (v >= 10) return ;
		X_paramBakGetGlobalAllParam()->pOCR->ExMsg[v] = 0;
		X_paramBakGetGlobalAllParam()->pOCR->ExMsg[v] |= MaxLen << 24;
		X_paramBakGetGlobalAllParam()->pOCR->ExMsg[v] |= MinLen << 16;
	}
	else{ // 2D
		v = nParam-MAX_1D_NUM;
		if (v >= 10) return ;
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[v] = 0;
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[v] |= MaxLen << 16;
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[v] |= MinLen;
	}
	m_chSendBytes[5] = 0x30;
}

//初始化设置
BOOL bInitParam(void)
{
    int nParam = 0;

	if (m_nReceive != 16) return FALSE;

	nParam = (int)(m_chReceiveBytes[7] - 0x30) * 10 + (int)(m_chReceiveBytes[8] - 0x30);
	if(nParam == 0){//厂家默认设置
		//if(!WriteComm()) return FALSE;
// 		if(!X_paramOriginalInitAllParam())
// 			return FALSE;
		if(X_paramLoadFactoryParam() != TRUE)
		{
			if(!X_paramOriginalInitAllParam())
				return FALSE;
		}
	}
	else if(nParam == 1){//存储当前设置为用户默认设置
		if(!X_paramSaveAsUsrDefaultParam()) return FALSE;
	}
	else if(nParam == 2){//载出用户默认设置
		if(!X_paramLoadUsrDefaultParam()) return FALSE;
	}
	else return FALSE;

	m_chSendBytes[5] = 0x30;//0:表示成功; 1表示失败
	return TRUE;
}

BOOL bSetPre_Suf_fix()
{
	char *p;
	int temp;
	int TotalLen,NowPlace;
	char byNak = 0x15;

	p = m_chReceiveBytes+7;

	TotalLen = (p[1] - '0') * 10 + (p[2] - '0');

	if (TotalLen > 10) return FALSE; //目前只限制设10个前缀/后缀信息
    if ('2' == p[0] && TotalLen > 2) return FALSE;//至多2个结束符后缀信息
	NowPlace = (p[3] - '0') * 10 + (p[4] - '0');

	if (TotalLen != 0 && NowPlace + 1 > TotalLen) return FALSE;
	if (NowPlace >= 10) return FALSE;

	if (GetHex(p[6]) == -1) return FALSE;
	temp = GetHex(p[6]);

	if (GetHex(p[7]) == -1) return FALSE;
	temp *= 16;
	temp += GetHex(p[7]);
	if (temp > 255) return FALSE;

	if(p[0] == '0') {//前缀设置
		if (TotalLen == 0) {//取消前缀
			X_paramBakGetGlobalAllParam()->pCode->nPrefix = 0;
		}
	}
	else if(p[0] == '1') {//后缀设置
		if (TotalLen == 0) {//取消后缀
			X_paramBakGetGlobalAllParam()->pCode->nSuffix = 0;
		}
	}
	else if(p[0] == '2'){
		if (TotalLen == 0) {//取消结束符后缀
			X_paramBakGetGlobalAllParam()->pCode->nTerminal = 0;
		}
	}
	else return FALSE;
	
	if (TotalLen == 0)
	{
		m_chSendBytes[5] = 0x30;
		nfixCnt = 0;
		nFixType = -1;
		___nfixSumLen = 0;
		byNak = 0x06;
		return TRUE;
	}
	if (nfixCnt == 0) //第一次设置
	{
		___nfixSumLen = TotalLen;
		nfixCnt |= (1 << NowPlace); //用32位保存设置位置
		nFixType = (p[0] - '0');
		g_pbyFix[NowPlace] = (unsigned char) temp;
	}
	//两次设置的类型或者长度不一致，显示错误，取消所有操作
	else if (___nfixSumLen != TotalLen || nFixType != (p[0] - '0'))
	{
		nfixCnt = 0;
		nFixType = -1;
		___nfixSumLen = 0;
		return FALSE;
	}
	else 
	{	//两次设置同一个位,以后一次设置的为准
		g_pbyFix[NowPlace] = (unsigned char) temp;
		nfixCnt |= (1 << NowPlace); //用32位保存设置位置
	}
	m_chSendBytes[5] = 0x30;

	if ((nfixCnt+1) == (1<<TotalLen))
	{
		if (nFixType == 0){
			X_paramBakGetGlobalAllParam()->pCode->nPrefix = TotalLen;
			memcpy(X_paramBakGetGlobalAllParam()->pCode->byPrefix, g_pbyFix, X_paramBakGetGlobalAllParam()->pCode->nPrefix);
		}
		else if (nFixType == 1){
			X_paramBakGetGlobalAllParam()->pCode->nSuffix = TotalLen;
			memcpy(X_paramBakGetGlobalAllParam()->pCode->bySuffix, g_pbyFix, X_paramBakGetGlobalAllParam()->pCode->nSuffix);
		}
		else if(nFixType == 2){
			X_paramBakGetGlobalAllParam()->pCode->nTerminal = TotalLen;
			memcpy(X_paramBakGetGlobalAllParam()->pCode->byTerminal, g_pbyFix, TotalLen);
		}
		else return FALSE;
		nfixCnt = 0;
		nFixType = -1;
		___nfixSumLen = 0;

		return TRUE;
	}

	return TRUE;
}

BOOL bDataFormatSet()
{
	int i,nTmp,n;
	char *p;
	int PrefixLen,SuffixLen;

	p = m_chReceiveBytes + 7;
	nTmp = (int)(p[0] - '0');
	
	if(nTmp < 3 ){//0,1,单独设置,2，设置自定义终止符
		if (m_nReceive != 16) return FALSE;
		if(bSetPre_Suf_fix()) return TRUE;
		else return FALSE;
	}
	// Code ID 设置
	else if(nTmp == 3){ // 1:是否发送ID,2:采用默认还是自定义3,4:条码类型,5:Code ID
		if (p[1] == '0') X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] = 0;
		else if (p[1] == '1'){
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] = 1;
		}

		if (p[3] > '9' || p[3] < '0' || p[4] > '9' || p[4] < '0') return FALSE;//条码类型
		n = (int)((p[3]-'0') * 10 + p[4] - '0');
		if (p[2] == '0') //默认
		{
			if (n >= MAX_1D_NUM && n < MAX_1D_NUM + CAN_DEC_2D_NUM)
			{
				X_paramBakGetGlobalAllParam()->pCode->by2DCodeID[n - MAX_1D_NUM] = CodeIDTab2D[n - MAX_1D_NUM];
				X_paramBakGetGlobalAllParam()->pCode2->by2DCodeID_2[n - MAX_1D_NUM] = CodeIDTab2D_2[n - MAX_1D_NUM];
			}
			else if (n < CAN_DEC_1D_NUM && n >= 0)
			{
				X_paramBakGetGlobalAllParam()->pCode->by1DCodeID[n] = CodeIDTab1D[n];
				X_paramBakGetGlobalAllParam()->pCode2->by1DCodeID_2[n] = CodeIDTab1D_2[n];
			}
			else if(n >= (MAX_1D_NUM + MAX_2D_NUM) && n < (MAX_1D_NUM + MAX_2D_NUM + CAN_DEC_OCR_NUM))
				return TRUE;//目前不提供SPEC_OCR_B的CodeID设置
			else return FALSE;
		}
		else if (p[2] == '1') //自定义
		{
			if (n >= MAX_1D_NUM && n < MAX_1D_NUM + CAN_DEC_2D_NUM)
			{
				X_paramBakGetGlobalAllParam()->pCode->by2DCodeID[n - MAX_1D_NUM] = p[5];
				if (m_nReceive==14)
					X_paramBakGetGlobalAllParam()->pCode2->by2DCodeID_2[n - MAX_1D_NUM] = CodeIDTab2D_2[n - MAX_1D_NUM];
				else if (m_nReceive==15)
					X_paramBakGetGlobalAllParam()->pCode2->by2DCodeID_2[n - MAX_1D_NUM] = p[6];
			}
			else if (n < CAN_DEC_1D_NUM && n >= 0)
			{
				X_paramBakGetGlobalAllParam()->pCode->by1DCodeID[n] = p[5];
				if (m_nReceive==14)
					X_paramBakGetGlobalAllParam()->pCode2->by1DCodeID_2[n] = CodeIDTab1D_2[n];
				else if (m_nReceive==15)
					X_paramBakGetGlobalAllParam()->pCode2->by1DCodeID_2[n] = p[6];
			}
			else if(n >= (MAX_1D_NUM + MAX_2D_NUM) && n < (MAX_1D_NUM + MAX_2D_NUM + CAN_DEC_OCR_NUM))
				return TRUE;//目前不提供SPEC_OCR_B的CodeID设置
			else if(n >= 96 && n < 128)
			{
				X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID[n - 96] = p[5];
				if (m_nReceive==14)
					X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID_2[n - 96] = CodeIDPostal_2[n - 96];
				else if (m_nReceive==15)
					X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID_2[n - 96] = p[6];
			}
			else return FALSE;
		}
		else return FALSE;
	}
	else if(nTmp == 4){ //使能1+长度2+内容(长度,0~10) +使能1+长度2+内容(长度：0~10)
		//前缀设置
		if(p[1] == '0'){ //使能 
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] = 0;
			PrefixLen = 0;
		}
		else if(p[1] == '1')  //使能 
		{
			memset(X_paramBakGetGlobalAllParam()->pCode->byPrefix,0,10);
			PrefixLen = (int)(p[2] - '0') * 10 + (int)(p[3] - '0'); //长度
			if(PrefixLen > 10) return FALSE;
			for(i = 0; i < PrefixLen; i++){
				X_paramBakGetGlobalAllParam()->pCode->byPrefix[i] = (unsigned char)p[4 + i];
			}
			X_paramBakGetGlobalAllParam()->pCode->nPrefix = PrefixLen;
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] = 1;
		}
		else return FALSE;

		//后缀设置
		if(p[4 + PrefixLen] == '0'){//使能 
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] = 0;
			SuffixLen = 0;
		}
		else if(p[4 + PrefixLen] == '1') {
			memset(X_paramBakGetGlobalAllParam()->pCode->bySuffix,0,10);
			SuffixLen = (int)(p[5 + PrefixLen] - '0') * 10 + (int)(p[6 + PrefixLen] - '0'); //长度
			if(SuffixLen > 10) return FALSE;
			else{
				for(i = 0; i < SuffixLen; i++){
					X_paramBakGetGlobalAllParam()->pCode->bySuffix[i] = (unsigned char)p[7 + PrefixLen + i];
				}
				X_paramBakGetGlobalAllParam()->pCode->nSuffix = SuffixLen;
				X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] = 1;
			}
		}
		else return FALSE;
	}
	else if(nTmp == 5){
		//后缀终止符设置
		if(p[1] == '0')  X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] = 0;
		else if(p[1] == '1') {
			PrefixLen = (int)(p[2] - '0');
			if(PrefixLen > 2) return FALSE;
			for(i = 0; i < PrefixLen; i++){
				if((GetHex(p[3+i*2]) == -1) || (GetHex(p[3+i*2+1]) == -1)) return FALSE;
				X_paramBakGetGlobalAllParam()->pCode->byTerminal[i] = (unsigned char)(GetHex(p[3+i*2]) * 16 + GetHex(p[3+i*2+1]));
			}
			if (i != PrefixLen) return FALSE;
			X_paramBakGetGlobalAllParam()->pCode->nTerminal = PrefixLen;
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] = 1;
		}
		else return FALSE;
	}
	else if (nTmp == 6){
//		i = p[1] - '0';
//		if (i < 0 || i > 3) return FALSE;
//		X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] = i;

		i = p[1] - '0'; //使能
		if(i == 0) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] &= (~0x01);
		else if(i < 4){
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] = ((unsigned char)((i - 1) << 1)) | 0x01;
		}
		else return FALSE;
	}
	else if(nTmp == 7){ //前缀顺序设置
		i = p[1] - '0';
		if(i > 5) return FALSE; //最多6种顺序
		X_paramBakGetGlobalAllParam()->pCode->PrefixOrder = i;
	}
	else return FALSE;

	return TRUE;
}

BOOL WriteComm(void)
{
	Y_commWrite((BYTE*)m_chSendBytes, m_nSend);
	return TRUE;
}


//分析COM设置数据
BOOL IsComProtocol(void)
{
	int nParam;
	TComParam  ComParam;

	long  plRate[] = {
		1200,2400,4800,9600,14400,19200,38400,57600,115200
	};
	long  plParity[] = {
		NOPARITY,EVENPARITY,ODDPARITY
	};
	long plStopBits[] = {
		1,2
	};
	long plDataBits[] = {
		5,6,7,8
	};

	m_chSendBytes[5] = 0x30;//0:表示成功; 1表示失败	

	//判断COM1
	nParam = (int)m_chReceiveBytes[7]-'0';
	//check param is to COM1
	if(nParam!=0){
		goto U_ERROR_EXIT;
	}

	//判断波特率
	nParam = (int)(m_chReceiveBytes[8]-'0') * 10
			 + (int)(m_chReceiveBytes[9]-'0');
	// 0 <= nParam < 8
// 	if (nParam == 4) goto U_ERROR_EXIT; // Linux 对 14400 波特率不支持
	if (nParam >= 0 && nParam <= 8){
		ComParam.lRate = plRate[nParam];
	}
	else {
		goto U_ERROR_EXIT;
	}

	//校验方法
	nParam = (int)m_chReceiveBytes[10]-'0';
	// nParam  = 0/1/2
	if (nParam >= 0 && nParam <= 2){
		ComParam.lParity = plParity[nParam];
	}
	else {
		goto U_ERROR_EXIT;
	}

	//停止位
	nParam = (int)m_chReceiveBytes[11]-'0';
	// nParam = 1/2
	if (nParam >= 0 && nParam <= 1){
		ComParam.lStopBits = plStopBits[nParam];
	}
	else {
		goto U_ERROR_EXIT;
	}

	//数据位
	nParam = (int)m_chReceiveBytes[12]-'0';
	// nParam = 5/6/7/8
// 	if (nParam >= 0 && nParam <= 3){
	if (nParam >= 2 && nParam <= 3){
		ComParam.lDataBits = plDataBits[nParam];
	}
	else {
		goto U_ERROR_EXIT;
	}
	*(X_paramBakGetGlobalAllParam()->pCom) = ComParam;

	m_chSendBytes[6] = 0;
	m_nSend = 6;
	AddLRC((char*)m_chSendBytes, &m_nSend);
	WriteComm();

	return X_paramSaveAsSysDefaultParam();

U_ERROR_EXIT:
	m_chSendBytes[5] = 0x31;//0:表示成功; 1表示失败	
	return FALSE;
}

/*
//图形参数设置
BOOL ImgSetting(void)
{
	int tmp;
	int fun = (int)(m_chReceiveBytes[7] - '0');
	switch(fun)
	{
	case 0:			//拍图
		U_appCaptureImg(TRUE);
		break;
	case 1:			//图像获取方式设置
		g_ImgGetMode = (int)(m_chReceiveBytes[8] - '0');
		break;
	case 2:			//图像样式设置
		g_ImgType = (int)(m_chReceiveBytes[8] - '0');
		break;
	case 3:			//图像颜色设置
		g_ImgColor = (int)(m_chReceiveBytes[8] - '0');
		break;
	case 4:			//压缩图像参数设置
		g_ImgReduceRow = (int)(m_chReceiveBytes[8] - '0');
		g_ImgReduceColumn = (int)(m_chReceiveBytes[9] - '0');
		break;
	case 5:			//裁剪图像参数设置
		tmp = (int)(m_chReceiveBytes[8] - '0')*1000
			+ (int)(m_chReceiveBytes[9] - '0')*100
			+ (int)(m_chReceiveBytes[10] - '0')*10
			+ (int)(m_chReceiveBytes[11] - '0');
		if ((tmp<0) && (tmp > Y_cmosGetWidth()))
			return FALSE;
		g_ImgCutStartRow = tmp;

		tmp = (int)(m_chReceiveBytes[12] - '0')*1000
			+ (int)(m_chReceiveBytes[13] - '0')*100
			+ (int)(m_chReceiveBytes[14] - '0')*10
			+ (int)(m_chReceiveBytes[15] - '0');
		if ((tmp<0) && (tmp > Y_cmosGetHeight()))
			return FALSE;
		g_ImgCutStartColumn = tmp;

		tmp = (int)(m_chReceiveBytes[16] - '0')*1000
			+ (int)(m_chReceiveBytes[17] - '0')*100
			+ (int)(m_chReceiveBytes[18] - '0')*10
			+ (int)(m_chReceiveBytes[19] - '0');
		if ((tmp<0) && (tmp > Y_cmosGetWidth()) && (tmp < g_ImgCutStartRow))
			return FALSE;
		g_ImgCutEndRow = tmp;

		tmp = (int)(m_chReceiveBytes[20] - '0')*1000
			+ (int)(m_chReceiveBytes[21] - '0')*100
			+ (int)(m_chReceiveBytes[22] - '0')*10
			+ (int)(m_chReceiveBytes[23] - '0');
		if ((tmp<0) && (tmp > Y_cmosGetWidth()) && (tmp < g_ImgCutStartColumn))
			return FALSE;
		g_ImgCutEndColumn = tmp;

		break;

	default:
		return FALSE;
	}
		
	return TRUE;
}
*/

BOOL Build_AL_SET_ECHO(void)
{
	int fun;
	int nParam;
	int nTmp;
	unsigned char i2C, num;
	char *p;
	int i;
	int iSetCode = FALSE;

	//功能号
	fun = (int)(m_chReceiveBytes[5]-'0')*10 + (int)(m_chReceiveBytes[6]-'0');
	
	m_chSendBytes[0] = STX;
	m_chSendBytes[1] = ATTR;
	m_chSendBytes[2] = 0x00;	//length
	m_chSendBytes[3] = 0x02;
	m_chSendBytes[4] = AL_SET_ECHO;
	m_chSendBytes[5] = 0x31; // 1---- 失败, 0---- 成功
	Z_usDelay(1);

	switch (fun){
	case 0://uEM2k_GetPort() setup	05
		if (m_nReceive != 16) break;
		if(IsComProtocol()) return TRUE;
		break;
	case 1://reserved, CMOS param setup
		if (m_nReceive != 16) break;
 
		p = m_chReceiveBytes + 10;
		if (GetHex(p[0]) != -1 && GetHex(p[1]) != -1)
			i2C = ((GetHex(p[0])<<4)+GetHex(p[1])); 
		else break;
		if (GetHex(p[3]) != -1 && GetHex(p[4]) != -1)
			num = ((GetHex(p[3])<<4)+GetHex(p[4])); 
		else break;
		Z_cmos_SetI2C(i2C,num);

		m_chSendBytes[5] = 0x30;
		break;

	case 2://light param setup
		if (m_nReceive != 16) break;

		nParam = (int)(m_chReceiveBytes[8]-0x30);
		if(nParam > 3) break; //读码常亮模式
// 		if (nParam == 1) break; //不能常亮
		X_paramBakGetGlobalAllParam()->pHard->nLight = nParam;

		nParam = (int)(m_chReceiveBytes[9]-0x30);
// 		if (nParam > 2) break;
// 		if (nParam == 1) break; //不能常亮
		if (nParam > 3) break;
		X_paramBakGetGlobalAllParam()->pHard->nFocus = nParam;
//		if(X_paramGetGlobalAllParam()->pHard->nFocus == 3) X_paramGetGlobalAllParam()->pCode->nReadMode = ESC2;
//		if(X_paramGetGlobalAllParam()->pHard->nFocus == 3)
//		{ X_paramGetGlobalAllParam()->pHard->nFocus = 0; }
		m_chSendBytes[5] = 0x30;
		break;
	case 3://1D code param setup
		if (m_nReceive != 16) break;
		nTmp = (int)(m_chReceiveBytes[7] - '0');
		nParam = ((int)((m_chReceiveBytes[8]-'0')*10 + (m_chReceiveBytes[9]-'0'))); 

		if(nTmp == 0) X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(1<<nParam);
		else if(nTmp == 1) X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (1<<nParam);
		else
			break;

		m_chSendBytes[5] = 0x30;
		break;

	case 4://2D code param setup
		if (m_nReceive != 16) break;

		nTmp = (int)(m_chReceiveBytes[7] - '0');
		nParam = ((int)((m_chReceiveBytes[8]-'0')*10 + (m_chReceiveBytes[9]-'0'))); 

		if(nTmp == 0) X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(1<<nParam);
		else if (nTmp == 1) X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (1<<nParam);
		else break;

		m_chSendBytes[5] = 0x30;
		break;

	case 5://all decode param setup
		if (m_nReceive != 16) break;

		nParam = (int)(m_chReceiveBytes[7] - 0x30);
		if(nParam == 0){
			X_paramBakGetGlobalAllParam()->p1D->unDecTypes = 0x0;
			X_paramBakGetGlobalAllParam()->p2D->unDecTypes = 0x0;
		}
		else if (nParam == 1){
			X_paramBakGetGlobalAllParam()->p1D->unDecTypes = 0xffffffff;
			X_paramBakGetGlobalAllParam()->p2D->unDecTypes = 0xffffffff;
		}
		else
			break;
		m_chSendBytes[5] = 0x30;
		break;
	case 6://Macro 设置
		if (m_nReceive != 16) break;
		nParam = (int)(m_chReceiveBytes[7] - 0x30);
	
		if (nParam == 0)
		{
			nParam = (int)(m_chReceiveBytes[8] - 0x30)*10 + (int)(m_chReceiveBytes[9] - 0x30);
			if (nParam > 3) break;
			X_paramBakGetGlobalAllParam()->p2D->nMacro = nParam;
		}
		else break;
		m_chSendBytes[5] = 0x30;

		break;

	case 7://初始化设置
		if(bInitParam()) m_chSendBytes[5] = 0x30;
		break;

	case 8:  //二维条码使能设置
		if (m_nReceive != 16) break;

		nTmp = (int)(m_chReceiveBytes[7] - '0');
		nParam  = (m_chReceiveBytes[10] - '0') * 10000;
		nParam += (m_chReceiveBytes[11] - '0') * 1000;
		nParam += (m_chReceiveBytes[12] - '0') * 100;
		nParam += (m_chReceiveBytes[13] - '0') * 10;
		nParam += (m_chReceiveBytes[14] - '0') * 1;
		
		//modify by HUNTON,2003.12.31
		if(nTmp == 0){
			if(m_chReceiveBytes[8] == '0')
				X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= (~nParam);
			else 
				X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= (~(nParam << 16));
		}
		else if(nTmp == 1){
			if(m_chReceiveBytes[8] == '0')
				X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= nParam;
			else 
				X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (nParam << 16);
		}
		else break;
		//end modify by HUNTON,2003.12.31

		m_chSendBytes[5] = 0x30;
		break;

	case 9: //一维条码使能设置
		if (m_nReceive != 16)	break;
		nTmp = (int)(m_chReceiveBytes[7] - '0');
		nParam  = (m_chReceiveBytes[10] - '0') * 10000;
		nParam += (m_chReceiveBytes[11] - '0') * 1000;
		nParam += (m_chReceiveBytes[12] - '0') * 100;
		nParam += (m_chReceiveBytes[13] - '0') * 10;
		nParam += (m_chReceiveBytes[14] - '0') * 1;
		if(nTmp == 0){
			if(m_chReceiveBytes[8] == '0')
				X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~nParam);
			else X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(nParam << 16));
		}
		else if(nTmp == 1){
			if(m_chReceiveBytes[8] == '0')
				X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= nParam;
			else X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (nParam << 16);
		}
		else
			break;
		
		m_chSendBytes[5] = 0x30;
		break;

	case 10: //读取信息长度设置
		vMsgLenSet();
		break;
	case 11: //条码参数设置
		if((!b1DParamSet()) && (!b2DParamSet())) break;
		m_chSendBytes[5] = 0x30;
		break;
	case 12: //解码模式设置
		if(!bDecModeParamSet()) break;
		m_chSendBytes[5] = 0x30;
		break;
	case 13: //系统综合设置
		if(!bOverallParamSet()) break;
		m_chSendBytes[5] = 0x30;
		break;
	case 14:
		break;
	case 15: //除了前后缀的其它数据格式设置
		if(!bOtherDataFormatSet()) break;
		m_chSendBytes[5] = 0x30;
		break;
	case 16: //OCR使能与禁止设置
		if(!bOCREnableSet()) break;
		m_chSendBytes[5] = 0x30;
		break;
	case 17: //声音频率及延迟时间设置
		break;
	case 26://重复，延迟设置
		if (m_nReceive != 16) break;
		vContinueDelayAndSoOnSet();
		break;
	case 27: //设置前缀，后缀,Code ID,AIM ......
		if (!bDataFormatSet()) break;
		m_chSendBytes[5] = 0x30;
		break;

	case 28: //所有前后缀禁止使能设置
		nTmp = (int)(m_chReceiveBytes[7] - '0');

		nParam = (int)(m_chReceiveBytes[8] - '0') * 10;
		nParam += (int)(m_chReceiveBytes[9] - '0');

		if((nTmp > 1 && nParam != POS_IsAIM + 1) 
			|| (nTmp > 3 && nParam == POS_IsAIM + 1)) break;
		if(0 == nParam){//整体设置
			for(i = 0; i < 5; i++) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[i] = nTmp;
		}
		else if(nParam < 6)
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[nParam - 1] = nTmp;
		else break;

		m_chSendBytes[5] = 0x30;
		break;

// 	case 29: //图形参数设置				add 2008.12.24  lf
// 		if (ImgSetting() == FALSE) break;
// 		m_chSendBytes[5] = 0x30;
// 		break;
	
    case 30: //设置CMOS
		if(GetHex(m_chReceiveBytes[7]) == -1 || GetHex(m_chReceiveBytes[8]) == -1) break;
		if(GetHex(m_chReceiveBytes[9]) == -1 || GetHex(m_chReceiveBytes[10]) == -1) break;
		nTmp = (GetHex(m_chReceiveBytes[7]) << 4) + GetHex(m_chReceiveBytes[8]);
		if (m_nReceive == 13)
		{
			if(GetHex(m_chReceiveBytes[11]) == -1)
				break;
			nParam = (GetHex(m_chReceiveBytes[9]) << 8) + (GetHex(m_chReceiveBytes[10]) << 4)
						+ GetHex(m_chReceiveBytes[11]);
			Z_cmos_SetI2C((unsigned char)nTmp, nParam);
		}
		else if (m_nReceive == 14)
		{
			if(GetHex(m_chReceiveBytes[11]) == -1 || GetHex(m_chReceiveBytes[12]) == -1)
				break;
			nParam = (GetHex(m_chReceiveBytes[9]) << 12) + (GetHex(m_chReceiveBytes[10]) << 8)
					 + (GetHex(m_chReceiveBytes[11]) << 4) + GetHex(m_chReceiveBytes[12]);
			Z_cmos_SetI2C((unsigned char)nTmp, nParam);
		}
		else
		{
			nParam = (GetHex(m_chReceiveBytes[9]) << 4) + GetHex(m_chReceiveBytes[10]);
			Z_cmos_SetI2C((unsigned char)nTmp, (unsigned char)nParam);
		}
		m_chSendBytes[5] = 0x30;
		break;
	case 31:	//设置振子使能、延时
		if (m_nReceive != 16) break;
		
		nParam = (int)(m_chReceiveBytes[7]-0x30);
		//振子使能
		X_paramBakGetGlobalAllParam()->pHard->bOscillation = nParam;
		
		nParam = (m_chReceiveBytes[8] - '0') * 1000;
		nParam += (m_chReceiveBytes[9] - '0') * 100;
		nParam += (m_chReceiveBytes[10] - '0') * 10;
		nParam += (m_chReceiveBytes[11] - '0') * 1;
		if (nParam > 3000)
		{
			nParam = 3000;
		}
		//振子延迟
		X_paramBakGetGlobalAllParam()->pHard->nOscillationDelay = nParam;

		m_chSendBytes[5] = 0x30;
		break;
	case 32:	//设置OPOS enable/disable
		/*
			7e 00 00 04 31 33 32 31 fa 	-- scan enable
			7e 00 00 04 31 33 32 30 fb 	-- scan disable
			
			02 00 00 02 32 30 ff		-- 设置成功
			02 00 00 02 32 31 fe		-- 设置失败
			15				-- 设置指令错误
		*/
		if (m_nReceive != 9) break;
		if (m_chReceiveBytes[7] == 0x30)	//disable
			U_SetOPOSEnable(0);
		else if (m_chReceiveBytes[7] == 0x31)	//enable
			U_SetOPOSEnable(1);
		else
			break;

		iSetCode = TRUE;
		m_chSendBytes[5] = 0x30;
		W_setcSetSaveFlag(FALSE);
		break;

	case 33:	//设置OPOS claim/release
		/*
			7e 00 00 04 31 33 33 31 fb 	-- claim
			7e 00 00 04 31 33 33 30 fa 	-- release
			
			02 00 00 02 32 30 ff		-- 设置成功
			02 00 00 02 32 31 fe		-- 设置失败
			15				-- 设置指令错误
		*/
		if (m_nReceive != 9) break;
		if (m_chReceiveBytes[7] == 0x30)	//release
			U_SetOPOSClaim(FALSE);
		else if (m_chReceiveBytes[7] == 0x31)	//claim
			U_SetOPOSClaim(TRUE);
		else
			break;

		iSetCode = TRUE;
		m_chSendBytes[5] = 0x30;
		W_setcSetSaveFlag(FALSE);
		break;
	default:
		//非QS命令，可能为打包形式的设置码批处理命令(长度不超过250)
		if((m_chReceiveBytes[5] == 'N' || m_chReceiveBytes[5] == 'n')
  		&& (m_chReceiveBytes[6] == 'L' || m_chReceiveBytes[6] == 'l')
		&& (m_chReceiveBytes[7] == 'S' || m_chReceiveBytes[7] == 's'))
		{
			iSetCode = TRUE;
			if(W_setcParseBatch((unsigned char *)(m_chReceiveBytes + 8), m_nReceive - 9)){
				m_chSendBytes[5] = 0x30;
			}
		}
		break;
	}
	
	m_chSendBytes[6] = 0;
	m_nSend = 6;
	AddLRC((char*)m_chSendBytes, &m_nSend);
	WriteComm();
	if((m_chSendBytes[5] == 0x30) &&
		((iSetCode && W_setcToSaveParam()) || (!iSetCode)))
	{
		W_setcSetSaveFlag(FALSE);
		return X_paramSaveAsSysDefaultParam();
	}

	return FALSE;
}

void uEcho_Com2Buff(void)
{
	BOOL bResult = TRUE;
	m_chSendBytes[3] = 0x05;
 	m_chSendBytes[5] = 0x01;//0x00;//表示COM1
 	m_chSendBytes[6] = 0x00;//0x00;//保留字段
	m_chSendBytes[7] = 0x00;//0x00;
 	m_chSendBytes[8] = 0x00;//0x00;
	switch(X_paramGetGlobalAllParam()->pCom->lRate) {
	case 1200:
		m_chSendBytes[8] |= 0x00;	break;
	case 2400:
		m_chSendBytes[8] |= 0x01;	break;
	case 4800:
		m_chSendBytes[8] |= 0x02;	break;
	case 9600:
		m_chSendBytes[8] |= 0x03;	break;
	case 14400:
		m_chSendBytes[8] |= 0x04;	break;
	case 19200:
		m_chSendBytes[8] |= 0x05;	break;
	case 38400:
		m_chSendBytes[8] |= 0x06;	break;
	case 57600:
		m_chSendBytes[8] |= 0x07;	break;
	case 115200:
		m_chSendBytes[8] |= 0x08;	break;
	default:
		bResult = FALSE; break;
	}
	switch(X_paramGetGlobalAllParam()->pCom->lDataBits) {
	case 5://all zero
		break;
	case 6:
		m_chSendBytes[8] |= 0x80;break;
	case 7:
		m_chSendBytes[7] |= 0x01;break;
	case 8:
		m_chSendBytes[7] |= 0x01;
		m_chSendBytes[8] |= 0x80;break;
	default:
		bResult = FALSE; break;
	}
	switch(X_paramGetGlobalAllParam()->pCom->lStopBits) {
	case 1://zero
		break;
	case 2:
		m_chSendBytes[8] |= 0x40;break;
	default:
		bResult = FALSE; break;
	}
	switch(X_paramGetGlobalAllParam()->pCom->lParity)
	{
	case NOPARITY://all zero
		break;
	case EVENPARITY:
		m_chSendBytes[8] |= 0x20;break;
	case ODDPARITY:
		m_chSendBytes[8] |= 0x10;break;
	default:
		bResult = FALSE; break;
	}
	m_chSendBytes[9] = 0;
	m_nSend = 9;
}

void uImg_Int2CharReverse(int nCrc,unsigned char* str)
{
	str[3] = (unsigned char)((nCrc & 0xff000000)>>24);
	str[2] = (unsigned char)((nCrc & 0x00ff0000)>>16);
	str[1] = (unsigned char)((nCrc & 0x00ff00)>>8);
	str[0] = (unsigned char)(nCrc & 0x00ff);
}

void nEcho_1DParam2Buff(void)
{
	m_chSendBytes[3] = 0x05;
	uImg_Int2CharReverse(X_paramGetGlobalAllParam()->p1D->unDecTypes,(unsigned char*)m_chSendBytes+5);
//	sprintf(m_chSendBytes+5,"%04d",X_paramGetGlobalAllParam()->p1D->unDecTypes);
// 	m_chSendBytes[5] = ___ParamBuf[16];//0x00;//保留字段
// 	m_chSendBytes[6] = ___ParamBuf[17];//0x00;//保留字段
// 	m_chSendBytes[7] = ___ParamBuf[18];//0x00;//保留字段
// 	m_chSendBytes[8] = ___ParamBuf[19];//0x07;//UPC, EAN, CODE128
	m_chSendBytes[9] = 0;
	m_nSend = 9;	
}

void nEcho_2DParam2Buff(void)
{
	m_chSendBytes[3] = 0x05;
	//sprintf(m_chSendBytes+5,"%04d",X_paramGetGlobalAllParam()->p2D->unDecTypes);
	uImg_Int2CharReverse(X_paramGetGlobalAllParam()->p2D->unDecTypes,(unsigned char*)m_chSendBytes+5);
// 	m_chSendBytes[5] = ___ParamBuf[20];//0x00;//保留字段
// 	m_chSendBytes[6] = ___ParamBuf[21];//0x00;//保留字段
// 	m_chSendBytes[7] = ___ParamBuf[22];//0x00;//保留字段
// 	m_chSendBytes[8] = ___ParamBuf[23];//0x07;//AZTEC, QR, PDF417
	m_chSendBytes[9] = 0;
	m_nSend = 9;	
}



void nEcho_PostalParam2Buff(void)
{
	m_chSendBytes[3] = 0x05;
	uImg_Int2CharReverse(X_paramGetGlobalAllParam()->pPostal->unDecTypes,(unsigned char*)m_chSendBytes+5);
	m_chSendBytes[9] = 0;
	m_nSend = 9;
}

void uEcho_LightParam2Buff(void)
{
	m_chSendBytes[3] = 0x05;

	m_chSendBytes[5] = 0x00;//0x00;//保留字段
	m_chSendBytes[6] = 0x00;//0x00;//保留字段
	m_chSendBytes[7] = 0x00;//0x00;//保留字段
	m_chSendBytes[8] = 0;
	switch(X_paramGetGlobalAllParam()->pHard->nLight){
	case 0://all zero
		break;
	case 1:
		m_chSendBytes[8] |= 0x04;break;
	case 2:
		m_chSendBytes[8] |= 0x08;break;
	case 3:
		m_chSendBytes[8] |= 0x0C;break;
	default:
		break;
	}
	switch(X_paramGetGlobalAllParam()->pHard->nFocus){
	case 0://all zero
		break;
	case 1:
		m_chSendBytes[8] |= 0x01;break;
	case 2:
		m_chSendBytes[8] |= 0x02;break;
	case 3:
		m_chSendBytes[8] |= 0x03;break;
	default:
		break;
	}
	if(X_paramGetGlobalAllParam()->pHard->nFocus & 0x10)
		m_chSendBytes[8] = (m_chSendBytes[8] & 0xf3) | 0x0c;
	m_chSendBytes[9] = 0;
	m_nSend = 9;
}

void uEcho_Macro2Buff(void)
{
	m_chSendBytes[3] = 0x05;
	uImg_Int2CharReverse(X_paramGetGlobalAllParam()->p2D->nMacro,(unsigned char*)m_chSendBytes+5);
// 	sprintf(m_chSendBytes+5,"%04d",X_paramGetGlobalAllParam()->p2D->nMacro);
// 	m_chSendBytes[5] = ___ParamBuf[56];//0x00;//保留字段
// 	m_chSendBytes[6] = ___ParamBuf[57];//0x00;//保留字段
// 	m_chSendBytes[7] = ___ParamBuf[58];//0x00;//保留字段
// 	m_chSendBytes[8] = ___ParamBuf[59]; //X_paramGetGlobalAllParam()->pCode->nMacro
	m_chSendBytes[9] = 0;
	m_nSend = 9;
}

void uEcho_PreSufix2Buff(void)
{
	m_chSendBytes[3] = 0x05;
	
	//前缀部分
	if(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] == 1)
		m_chSendBytes[5] = 0x01;
	m_chSendBytes[6] = (char)X_paramGetGlobalAllParam()->pCode->nPrefix;
	memcpy(m_chSendBytes+7,(char*)X_paramGetGlobalAllParam()->pCode->byPrefix,X_paramGetGlobalAllParam()->pCode->nPrefix);

	if(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] == 1) 
		m_chSendBytes[7+X_paramGetGlobalAllParam()->pCode->nPrefix] = 0x01;

	m_chSendBytes[8+X_paramGetGlobalAllParam()->pCode->nPrefix] = (char)X_paramGetGlobalAllParam()->pCode->nSuffix;
	memcpy(m_chSendBytes+9+X_paramGetGlobalAllParam()->pCode->nPrefix,(char*)X_paramGetGlobalAllParam()->pCode->bySuffix,X_paramGetGlobalAllParam()->pCode->nSuffix);
	m_nSend = 9+X_paramGetGlobalAllParam()->pCode->nPrefix+X_paramGetGlobalAllParam()->pCode->nSuffix;
	m_chSendBytes[m_nSend] = 0;
}

void uEcho_CodeID2Buff(void)
{
	int nParam;

	m_chSendBytes[3] = 0x05;

	if(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] == 1)
		m_chSendBytes[5] = 0x01;

	nParam = (int)(m_chReceiveBytes[6] - 0x30) * 10;
	nParam += (int)(m_chReceiveBytes[7] - 0x30);
	m_chSendBytes[6] = m_chReceiveBytes[6];
	m_chSendBytes[7] = m_chReceiveBytes[7];

	if(nParam < CAN_DEC_1D_NUM){
		m_chSendBytes[8] = X_paramGetGlobalAllParam()->pCode->by1DCodeID[nParam];
		if(X_paramGetGlobalAllParam()->pCode2->by1DCodeID_2[nParam] != 0)
		{
			m_chSendBytes[9] = X_paramGetGlobalAllParam()->pCode2->by1DCodeID_2[nParam];
			m_nSend = 10;
		}
		else
			m_nSend = 9;
	}
	else if(nParam >= MAX_1D_NUM && nParam < (MAX_1D_NUM + CAN_DEC_2D_NUM)){
		nParam -= MAX_1D_NUM;
		m_chSendBytes[8] = X_paramGetGlobalAllParam()->pCode->by2DCodeID[nParam];
		if(X_paramGetGlobalAllParam()->pCode2->by2DCodeID_2[nParam] != 0)
		{
			m_chSendBytes[9] = X_paramGetGlobalAllParam()->pCode2->by2DCodeID_2[nParam];
			m_nSend = 10;
		}
		else
			m_nSend = 9;
	}
	else if((nParam >= (MAX_1D_NUM + MAX_2D_NUM)) && (nParam < (MAX_1D_NUM + MAX_2D_NUM + CAN_DEC_OCR_NUM))){
		nParam -= (MAX_1D_NUM+MAX_2D_NUM);
		m_chSendBytes[8] = 'X';
		m_nSend = 9;
	}
	else if((nParam >= (MAX_1D_NUM + MAX_2D_NUM + 16)) && (nParam < 99)){
		nParam -= (MAX_1D_NUM + MAX_2D_NUM + 16);
		m_chSendBytes[8] = X_paramGetGlobalAllParam()->pCode2->byPostalCodeID[nParam];
		if(X_paramGetGlobalAllParam()->pCode2->byPostalCodeID_2[nParam] != 0)
		{
			m_chSendBytes[9] = X_paramGetGlobalAllParam()->pCode2->byPostalCodeID_2[nParam];
			m_nSend = 10;
		}
		else
			m_nSend = 9;
	}

	else return;
	m_chSendBytes[m_nSend] = 0;
}

void uEcho_AIMEnable2Buff(void)
{
	m_chSendBytes[3] = 0x05;
//	m_chSendBytes[5] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] + '0';
//	m_nSend = 6;

	if(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] & 0x01){
		m_chSendBytes[5] = ((X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] & 0x06) >> 1) + 0x31;
	}
	else m_chSendBytes[5] = 0x30;
	
	m_nSend = 6;
	m_chSendBytes[m_nSend] = 0;
}

void uEcho_EndSufix2Buff(void)
{
//	int nParam;
	int i;

	m_chSendBytes[3] = 0x05;

	if(X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] == 1) m_chSendBytes[5] = 0x01;

	m_chSendBytes[6] = X_paramGetGlobalAllParam()->pCode->nTerminal;
	for(i = 0; i < (int)(X_paramGetGlobalAllParam()->pCode->nTerminal); i++) //已经包含长度
		m_chSendBytes[7+i] = X_paramGetGlobalAllParam()->pCode->byTerminal[i];
	m_nSend = 7+i;
	m_chSendBytes[m_nSend] = 0;
}

////前后缀顺序查询
void uEcho_FmtOrder2Buff(void)
{
	m_chSendBytes[3] = 0x05;
	m_chSendBytes[5] = X_paramGetGlobalAllParam()->pCode->PrefixOrder + '0';
	m_nSend = 6;
	m_chSendBytes[m_nSend] = 0;
}

void uEcho_OCRParam2Buff(void)
{
	m_chSendBytes[3] = 0x05;
	uImg_Int2CharReverse(X_paramGetGlobalAllParam()->pOCR->unDecTypes,(unsigned char*)m_chSendBytes+5);
//	sprintf(m_chSendBytes+5,"%04d",X_paramGetGlobalAllParam()->pOCR->unDecTypes);
// 	m_chSendBytes[5] = ___ParamBuf[178];//0x00;//保留字段
// 	m_chSendBytes[6] = ___ParamBuf[179];//0x00;//保留字段
// 	m_chSendBytes[7] = ___ParamBuf[180];//0x00;//保留字段
// 	m_chSendBytes[8] = ___ParamBuf[181];//0x01;//SPEC_OCR_B
	m_chSendBytes[9] = 0;
	m_nSend = 9;
}


//查询设备详细的版本信息
void vQueryDeviceVersion(void)
{
	char buff[512];
	int nLen;
// 	char *p;

	m_chSendBytes[3] = 0x06;
	m_chSendBytes[5] = 'V';
	m_chSendBytes[6] = 'E';
	m_chSendBytes[7] = 'R';
	m_chSendBytes[8] = ':';
	m_chSendBytes[9] = 0x7F;
	m_chSendBytes[10] = 0x7F;
	m_chSendBytes[11] = 0x7F;
	m_chSendBytes[12] = 0x7F;
	m_chSendBytes[13] = '\r';
	m_chSendBytes[14] = '\n';
	m_nSend = 15;
// 
// 	nLen = 0;
// 	//uIMG信息
// 	nLen += X_decoGetUIMGVersion(buff+nLen,sizeof(buff)-nLen);
// 	//硬件信息
// 	nLen += Z_GetHardwareInfo(buff+nLen,sizeof(buff)-nLen);
// 
// 	if (sizeof(buff)-nLen > sizeof(MODULE_SDK_VERSION)+12)
// 	{
// 		nLen += sprintf(buff+nLen,"MSDKVer:%s\r\n",MODULE_SDK_VERSION);
// 	}//End of if;
	nLen = V_commGetVersion(buff,sizeof(buff));

	if (nLen > sizeof(m_chSendBytes)-m_nSend-1)
	{ nLen = sizeof(m_chSendBytes)-m_nSend-1; }
	memcpy(m_chSendBytes+m_nSend,buff,nLen);
	m_nSend += nLen;

	m_chSendBytes[m_nSend] = 0;
}

void vQueryFirmwareVersion(int type)
{
	char buff[512];
	int nLen;
	// 	char *p;
	
	m_chSendBytes[3] = 0x06;
	m_nSend = 5;
	nLen = Z_GetKernelInfo(type, buff,sizeof(buff));
	
	if (nLen > sizeof(m_chSendBytes)-m_nSend-1)
	{ nLen = sizeof(m_chSendBytes)-m_nSend-1; }
	memcpy(m_chSendBytes+m_nSend,buff,nLen);
	m_nSend += nLen;
	
	m_chSendBytes[m_nSend] = 0;
}

BOOL QueryMinMaxLength()
{
	unsigned int nParam,v;

	const char conCodeLen[] = {
	 '0','8','0','8', //ZA
	 '0','8','0','8', //SETUP 128
	 '0','1','3','0', //Code 128
	 '0','1','3','0', //UCCEAN128
	 '0','8','0','8', //EAN-8
     '0','D','0','D', //EAN-13
	 '0','8','0','8', //UPC-E
	 '0','C','0','C', //UPC-A
	 '0','6','5','0', //INTERLEAVE 2Of5
	 '0','E','0','E', //ITF14
	 '0','6','0','6', //ITF6
	 '0','4','5','0', //Matrix25
	 '0','4','5','0', //Module25
	 '0','1','3','0', //Code 39(Standard)
	 '0','1','3','0', //Code 39(Full ASCII)
	 '0','2','3','C', //CodaBar
	 '0','1','5','0', //2OUTOF5
	 '0','1','3','0', //Code 93
	};
	m_chSendBytes[3] = 0x05;

	nParam = (int)(m_chReceiveBytes[6] - 0x30) * 10; //获得条码类型
	nParam += (int)(m_chReceiveBytes[7] - 0x30);
	m_chSendBytes[5] = m_chReceiveBytes[6];
	m_chSendBytes[6] = m_chReceiveBytes[7];
	if(nParam < MAX_1D_NUM || nParam >= (MAX_1D_NUM+MAX_2D_NUM)){
		switch(nParam) {
		case POS_CODE128:
		case POS_UCCEAN128:
// 		case POS_EAN8:
// 		case POS_EAN13:
// 		case POS_UPCE:
// 		case POS_UPCA:
		case POS_INTER2OF5:
// 		case POS_ITF14:
// 		case POS_ITF6:
		case POS_MATRIX25:
		case POS_MODULE25:
		case POS_REGULAR39:
		case POS_FULLASCII39:
		case POS_CODABAR:
// 		case POS_2OUTOF5:
		case POS_CODE93:
		case POS_CODEZ:
		case POS_ISBN:
		case POS_INDUSTRIAL25:
		case POS_STANDARD25:
		case POS_PLESSEY:
		case POS_CODE11:
		case POS_MSI_PLESSEY:
		case POS_COMPOSITE:
		case POS_RSS:
		case POS_CHNPOST:
			v = (X_paramGetGlobalAllParam()->p1D->ExMsg[nParam]&0xff000000)>>24;
			sprintf(m_chSendBytes+7,"%04x",v);
			//uImg_Int2DigitChar(v,(unsigned char*)m_chSendBytes+7);
			v = (X_paramGetGlobalAllParam()->p1D->ExMsg[nParam]&0x00ff0000)>>16;
			sprintf(m_chSendBytes+11,"%04x",v);
			//uImg_Int2DigitChar(v,(unsigned char*)m_chSendBytes+11);
			break;

		case POS_SPEC_OCR_B + 64:
			v = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B]&0xff000000)>>24;
			//uImg_Int2DigitChar(v,(unsigned char*)m_chSendBytes+7);
			sprintf(m_chSendBytes+7,"%04x",v);
			v = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B]&0x00ff0000)>>16;
			sprintf(m_chSendBytes+11,"%04x",v);
			break;
		default : 
			if(nParam >= CAN_DEC_1D_NUM) break;
			m_chSendBytes[7] = 0x30;
		    m_chSendBytes[8] = 0x30;
			m_chSendBytes[9] = conCodeLen[nParam * 4 + 2];
			m_chSendBytes[10] = conCodeLen[nParam * 4 + 3];
			
			m_chSendBytes[11] = 0x30;
			m_chSendBytes[12] = 0x30;
			m_chSendBytes[13] = conCodeLen[nParam * 4];
			m_chSendBytes[14] = conCodeLen[nParam * 4 + 1];
			break;
		}
	}
	else{
		nParam -= MAX_1D_NUM;
		switch(nParam)
		{
		case POS_PDF417:
		case POS_QR:
		case POS_Aztec:
		case POS_DataMatrix:
		case POS_MaxiCode:
		case POS_VeriCode:
		case POS_UN_VeriCode:
		case POS_HAN_SENSIBLE:
			v = (X_paramGetGlobalAllParam()->p2D->ExMsg[nParam]&0xffff0000)>>16;
			//uImg_Int2DigitChar(v,(unsigned char*)m_chSendBytes+7);
			sprintf(m_chSendBytes+7,"%04x",v);
			v = (X_paramGetGlobalAllParam()->p2D->ExMsg[nParam]&0x0000ffff);
			//uImg_Int2DigitChar(v,(unsigned char*)m_chSendBytes+11);
			sprintf(m_chSendBytes+11,"%04x",v);
			break;
		default: 
			return FALSE;
		}
	}
	m_nSend = 15;
	m_chSendBytes[15] = 0;

	return TRUE;
}


BOOL Build_AL_SET_Q_ECHO(void)
{
//	int i,nParam,v,v1;
// 	unsigned char I2C;
	int i2c;
	int fun;

	memset(m_chSendBytes,0,256);

	m_chSendBytes[0] = STX;
	m_chSendBytes[1] = ATTR;
	m_chSendBytes[2] = 0x00;	//length
	m_chSendBytes[3] = 0x00;
	m_chSendBytes[4] = AL_SET_Q_ECHO;

	switch (m_chReceiveBytes[5])
	{
	case 0x30://uEM2k_GetPort() 参数返回		//查询通信口1的设置
		if (m_nReceive != 7) return FALSE;
		uEcho_Com2Buff();
		break;
	case 0x32://1D条码允许、禁止读取参数查询
		if (m_nReceive != 7) return FALSE;
		nEcho_1DParam2Buff();
		break;

	case 0x33://2D条码允许、禁止读取参数查询
		if (m_nReceive != 7) return FALSE;
		nEcho_2DParam2Buff();
		break;

	case 0x35://照明、对焦灯设置参数查询
		if (m_nReceive != 7) return FALSE;
		uEcho_LightParam2Buff();
		break;
	case 0x36:
		if (m_nReceive != 7) return FALSE;
		uEcho_Macro2Buff();
		break;

	case 0x37://前后缀查询
		if (m_nReceive != 7) return FALSE;
		uEcho_PreSufix2Buff();
		break;

	case 0x38: //CodeID查询
		if (m_nReceive != 9) return FALSE;
		uEcho_CodeID2Buff();
		break;

	case 0x39://AIM前缀使用情况查询
		if (m_nReceive != 7) return FALSE;
		uEcho_AIMEnable2Buff();
		break;
	case 0x40://结束符后缀查询
		if (m_nReceive != 7) return FALSE;
		uEcho_EndSufix2Buff();
		break;
	case 0x41: //查询条码的最大最小读码长度
		if (m_nReceive != 9) return FALSE;
		if (QueryMinMaxLength() == FALSE) return FALSE;
		break;
	case 0x42: //前后缀顺序查询
		if(m_nReceive != 7) return FALSE;
		uEcho_FmtOrder2Buff();
		break;
	case 0x43: //一维条码参数查询
		if((!b1DParamQuery()) && (!b2DParamQuery()) && (!bPostalParamQuery())) return FALSE;
		break;
	case 0x44: //解码模式查询
		//if(m_nReceive != 7) return FALSE;
		if(!bDecodeModeQuery()) return FALSE;
		break;
	case 0x46: //除了前后缀外的其它数据格式查询
		if(!bOtherFormatQuery()) return FALSE;
		break;
	case 0x47: //查询版本号
		if (m_nReceive == 7) 
			vQueryDeviceVersion();
		else if (m_nReceive == 8)
		{
			if (m_chReceiveBytes[6]<2)
			{
				vQueryFirmwareVersion(m_chReceiveBytes[6]);
			}
			else
				return FALSE;
		}
		else 
			return FALSE;
		break;
	case 0x48: //产品相关参数综合查询
		if(!bOverAllQuery()) return FALSE;
		break;
	case 0x49: //OCR允许、禁止读取参数查询
		if (m_nReceive != 7) return FALSE;
		uEcho_OCRParam2Buff();
		break;
	
	case 0x4b: //CMOS地址参数查询
		if(GetHex(m_chReceiveBytes[6]) == -1 || GetHex(m_chReceiveBytes[7]) == -1) return FALSE;
		i2c = (GetHex(m_chReceiveBytes[6]) << 4) + GetHex(m_chReceiveBytes[7]);
		
		i2c = Z_cmos_GetI2C(i2c);

		if (i2c&0xf000)
		{
			m_chSendBytes[3] = 0x05;
			if(((i2c&0xf000)>>12) > 0x09) m_chSendBytes[5] = ((i2c&0xf000)>>12) - 10 + 0x41;
			else m_chSendBytes[5] = ((i2c&0xf000)>>12) + 0x30;
			if(((i2c&0x0f00)>>8) > 0x09) m_chSendBytes[6] = ((i2c&0x0f00)>>8) - 10 + 0x41;
			else m_chSendBytes[6] = ((i2c&0x0f00)>>8) + 0x30;
			if(((i2c&0xf0) >> 4) > 0x09) m_chSendBytes[7] = ((i2c&0xf0) >> 4) - 10 + 0x41;
			else m_chSendBytes[7] = ((i2c&0xf0) >> 4) + 0x30;			
			if((i2c & 0x0f) > 0x09) m_chSendBytes[8] = (i2c & 0x0f) - 10 + 0x41;
			else m_chSendBytes[8] = (i2c & 0x0f) + 0x30;
			m_chSendBytes[9] = 0;
			m_nSend = 9;
		}
		else if (i2c&0x0f00)
		{
			m_chSendBytes[3] = 0x04;
			if(((i2c&0x0f00)>>8) > 0x09) m_chSendBytes[5] = ((i2c&0x0f00)>>8) - 10 + 0x41;
			else m_chSendBytes[5] = ((i2c&0x0f00)>>8) + 0x30;
			if(((i2c&0xf0) >> 4) > 0x09) m_chSendBytes[6] = ((i2c&0xf0) >> 4) - 10 + 0x41;
			else m_chSendBytes[6] = ((i2c&0xf0) >> 4) + 0x30;			
			if((i2c & 0x0f) > 0x09) m_chSendBytes[7] = (i2c & 0x0f) - 10 + 0x41;
			else m_chSendBytes[7] = (i2c & 0x0f) + 0x30;
			m_chSendBytes[8] = 0;
			m_nSend = 8;
		}
		else
		{
			m_chSendBytes[3] = 0x03;
			if(((i2c&0xf0) >> 4) > 0x09) m_chSendBytes[5] = ((i2c&0xf0) >> 4) - 10 + 0x41;
			else m_chSendBytes[5] = ((i2c&0xf0) >> 4) + 0x30;			
			if((i2c & 0x0f) > 0x09) m_chSendBytes[6] = (i2c & 0x0f) - 10 + 0x41;
			else m_chSendBytes[6] = (i2c & 0x0f) + 0x30;			
			m_chSendBytes[7] = 0;
			m_nSend = 7;
		}
		break;

	case 0x4c: //图像参数查询
		if (m_nReceive != 9) return FALSE;
		fun = (int)(m_chReceiveBytes[6] - '0')*10 
			+ (int)(m_chReceiveBytes[7] - '0');
		switch(fun){
		case 0:		//查询原始图像分辨率
			m_chSendBytes[5] = 0x30;
			m_chSendBytes[6] = 0x30;
			sprintf(m_chSendBytes+7, "%04d%04d", Y_cmosGetWidth(), Y_cmosGetHeight());
			m_nSend = 15;
			break;
/*
		case 1:			//查询图像获取方式
			m_chSendBytes[5] = 0x30;
			m_chSendBytes[6] = 0x31;
			m_chSendBytes[7] = g_ImgGetMode + '0';
			m_chSendBytes[8] = 0;
			m_nSend = 8;
			break;
		case 2:		//查询图像样式
			m_chSendBytes[5] = 0x30;
			m_chSendBytes[6] = 0x32;
			m_chSendBytes[7] = g_ImgType + '0';
			m_chSendBytes[8] = 0;
			m_nSend = 8;
			break;
		case 3:		//查询图像颜色
			m_chSendBytes[5] = 0x30;
			m_chSendBytes[6] = 0x33;
			m_chSendBytes[7] = g_ImgColor + '0';
			m_chSendBytes[8] = 0;
			m_nSend = 8;
			break;
		case 4:		//查询图像压缩参数
			m_chSendBytes[5] = 0x30;
			m_chSendBytes[6] = 0x34;
			m_chSendBytes[7] = g_ImgReduceRow + '0';
			m_chSendBytes[8] = g_ImgReduceColumn + '0';
			m_chSendBytes[9] = 0;
			m_nSend = 9;
			break;
		case 5:		//查询图像裁剪参数
			m_chSendBytes[5] = 0x30;
			m_chSendBytes[6] = 0x35;
			sprintf(m_chSendBytes+7, "%04d%04d%04d%04d", 
				g_ImgCutStartRow, g_ImgCutStartColumn,g_ImgCutEndRow,g_ImgCutEndColumn);
			m_nSend = 23;
			break;
*/
		case 6:		//查询CMOS类型
			m_chSendBytes[5] = 0x30;
			m_chSendBytes[6] = 0x36;
			sprintf(m_chSendBytes+7, "%02d", Z_cmos_GetType());
			m_nSend = 9;
			break;
		default:
			return FALSE;
		}
		break;

	case 0x4d: //振子查询
		if (m_nReceive != 7) return FALSE;		
		m_chSendBytes[5] = X_paramGetGlobalAllParam()->pHard->bOscillation + '0';	//使能
		sprintf(&m_chSendBytes[6], "%04d", X_paramGetGlobalAllParam()->pHard->nOscillationDelay);//延时
		m_chSendBytes[10] = 0;
		m_nSend = 10;
		break;

	case 0x4e: //镜像查询
		m_chSendBytes[5] = X_paramGetGlobalAllParam()->pHard->nProduction + '0';	
		m_chSendBytes[6] = 0;
		m_nSend = 6;
		break;

	case 0x4f: //解码声音查询
		m_chSendBytes[5] = X_paramGetGlobalAllParam()->pHard->bDecVoice + '0';		//使能
		m_chSendBytes[6] = (X_paramGetGlobalAllParam()->pHard->nBeepType & 0xf) + '0';		//解码声音类型
		m_chSendBytes[7] = ((X_paramGetGlobalAllParam()->pHard->nBeepType>>4) & 0xf) + '0';		//解码声音的音量
		m_chSendBytes[8] = 0;
		m_nSend = 8;
		break;

	case 0x50: //信息截取
		m_chSendBytes[5] = X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCut] + '0';		//使能
		memcpy(m_chSendBytes+6, X_paramGetGlobalAllParam()->pCode->byCut, 60);
		m_chSendBytes[66] = 0;
		m_nSend = 66;
		break;

	case 0x51: //HID-KBW参数
		m_chSendBytes[5] = X_paramGetGlobalAllParam()->pKBW->nLanguage;		//语言	
		m_chSendBytes[6] = X_paramGetGlobalAllParam()->pKBW->isUnknownCharactersBeep;//遇到未知字符是否声音提示。取值：0，1 默认：0。
		m_chSendBytes[7] = X_paramGetGlobalAllParam()->pKBW->isEmulateKeypad;//采用ALT+ Keypad输入ASCII值 来产生字符。取值：0，1 默认：0。		
		m_chSendBytes[8] = X_paramGetGlobalAllParam()->pKBW->isCtrlASCIIMode;	//Control + ASCII 模式
		m_chSendBytes[9] = X_paramGetGlobalAllParam()->pKBW->isKeysrockeDelay;	//发送每个字符之间的延时设置。取值：0，1，2 默认：0。	
		m_chSendBytes[10] = X_paramGetGlobalAllParam()->pKBW->isCapsLock;	//自动切换大小写		
		m_chSendBytes[11] = X_paramGetGlobalAllParam()->pKBW->isConvertCase;//强制转换所有字母都成为大写或者小写。取值：0，1，2 默认：0		
		m_chSendBytes[12] = X_paramGetGlobalAllParam()->pKBW->isNumericMode;	//数字模式	
		m_chSendBytes[13] = X_paramGetGlobalAllParam()->pKBW->isKeyPad;	//键盘模式
		m_chSendBytes[14] = X_paramGetGlobalAllParam()->pKBW->isFasttransferMode;	//传送速度
		m_chSendBytes[15] = X_paramGetGlobalAllParam()->pKBW->nPollRate;	//轮询速率
		m_chSendBytes[16] = X_paramGetGlobalAllParam()->pKBW->nCodePage;	//code page
		m_chSendBytes[17] = 0;
		m_nSend = 16;
// 		m_chSendBytes[14] = 0;
// 		m_nSend = 14;
		break;

	default:
		if (m_nReceive != 7) return FALSE;
		m_chSendBytes[3] = 0x06;
		m_chSendBytes[5] = 'N';
		m_chSendBytes[6] = 'o';
		m_chSendBytes[7] = ' ';
		m_chSendBytes[8] = 'U';
		m_chSendBytes[9] = 's';
		m_chSendBytes[10] = 'e';
		m_chSendBytes[11] = '!';
		m_chSendBytes[12] = 0;
		m_nSend = 12;
		break;
	}
	m_chSendBytes[2] = (char)((m_nSend-4)>>8);
	m_chSendBytes[3] = (char)((m_nSend-4) & 0xFF);
	AddLRC((char*)m_chSendBytes, &m_nSend);
	WriteComm();
	return TRUE;
}

BOOL TranslateMes(void)
{
	int nResult;
	char cmd = 0x06;
	int pm;

	if (!CheckMessage())
	{
		if (m_nReceive < 2) return FALSE;
		cmd = 0x15;
		Y_commWrite((BYTE*)&cmd,1);
		return FALSE;
	}

	//信息串检测
	if (ChkStringParam())  return FALSE;

	cmd = 0x06;
	switch (m_chReceiveBytes[4]){
	case AL_SET:
		nResult = Build_AL_SET_ECHO();
		break;
	case AL_SET_Q:
		nResult = Build_AL_SET_Q_ECHO();
		break;
	case AL_IMG_REQ:
		//GetCMOSFrame();
		pm = Z_GetPM();
		if (pm != FREQ_RUN_HIGH)
			Z_SetPM_EX(FREQ_RUN_HIGH);

		Build_AL_IMG_REQ();

		if (pm != FREQ_RUN_HIGH)
			Z_SetPM_EX(pm);

		break;
	case AL_SET_DATAFORMAT:
		if(Build_AL_SET_DATAFORMAT()) 
			Y_commWrite((BYTE*)&cmd,1);
             else
             {
			cmd = 0x15;
			Y_commWrite((BYTE*)&cmd,1);
		 }
		break;
	default:
		break;
	}
	return TRUE;
}

int ReadComm(char FrameProper)
{
    int nLenCmd = 0;
    int nEn ,nSt;

    memset(m_chReceiveBytes,0,NLSCOMM_OUTBUF_SIZE);
    m_nReceive = 0;
    
    if(FrameProper == 0)
    {
        m_chReceiveBytes[m_nReceive++] = 0x02;
        m_chReceiveBytes[m_nReceive++] = FrameProper;

	if (Y_commBakRead((BYTE*)(m_chReceiveBytes+m_nReceive),2,40) < 2) return m_nReceive;

	nLenCmd = (unsigned char)m_chReceiveBytes[m_nReceive++];
    nLenCmd = (nLenCmd << 8) | (unsigned char)m_chReceiveBytes[m_nReceive++];

	nLenCmd++;//add the checksum
	if (nLenCmd >= 200) return 0;

	if (Y_commBakRead((BYTE*)(m_chReceiveBytes+m_nReceive),nLenCmd,nLenCmd*10)!= nLenCmd)	return m_nReceive;

	m_nReceive += nLenCmd;

	if (!CheckMessage()) return 0;

        
    }
    else
   {
        m_chReceiveBytes[m_nReceive++] = '~';
        m_chReceiveBytes[m_nReceive++] = FrameProper;

        nSt = Z_Get10msCounter();
        while(1)
        {
            nLenCmd = Y_commBakRead((BYTE*)(m_chReceiveBytes+m_nReceive), 32, 5);//usb接口，或串口baud115200,32字节接收超时仅需5ms左右
            m_nReceive += nLenCmd;
            if(m_chReceiveBytes[m_nReceive-1] == 0x03)
                break;
            if(nLenCmd != 32)
            {
                while(1)
                {
                    nLenCmd = Y_commBakReadChar((BYTE*)(m_chReceiveBytes+m_nReceive), 20);//当超时时间内未收到数据，改成1次读1个字节并超时时间设为20ms，这样baud1200也不至于数据漏收
                    
                    if(!nLenCmd)
                        break;
                    
                    m_nReceive += nLenCmd;
                    if(m_chReceiveBytes[m_nReceive-1] == 0x03)
                        break;

                    nEn = Z_Get10msCounter();
                    if((nEn-nSt)>350)
                    {
                        Z_DogFeed();
                        break;
                    }
                 }
            }
            nEn = Z_Get10msCounter();
            if((nEn-nSt)>350)
            {
                Z_DogFeed();
                break;
            }
        }
     }
    //printf("%s",m_chReceiveBytes);
    return m_nReceive;
}



BOOL bPicDecodeSuc = FALSE;
int testPicWidth = IMG_WIDTH_MAX;
int testPicHigh = IMG_HEIGHT_MAX;
int testTime = 0;


//采用ESC控制分析和设置
BOOL IsEscCtrl()
{
//	int i;
	char buff = 0x06;
	unsigned char cmd;
	UALLPARAM *psParam;

	psParam = X_paramGetGlobalAllParam();

	if (Y_commBakReadChar((char *)&cmd, COMMREAD_TIMEOUT) == 0){
		buff = 0x15;
		Y_commWrite((BYTE*)&buff,1);
		return FALSE;
	}

	switch(cmd) {
	case 0x30: // ESC + 0 for  LED POWER OFF
		psParam->pCode->nReadMode = ESC0;
		psParam->pMode->bContinue = TRUE;
		X_paramSaveAsSysDefaultParam();
		break;

	case 0x31: /* ESC + 1  for ONE TIME LED POWER ON (READ ON) 
				.  REDING GOOD -> IMMEDIATELY LED OFF*/
		psParam->pCode->nReadMode = ESC1;
		psParam->pMode->bContinue = TRUE;
		psParam->pMode->nReadDelay = Z_Get10msCounter();
		break;

	case 0x32: // ESC + 2 for CONTINUOUS LED POWER ON
		psParam->pCode->nReadMode = ESC2;
		psParam->pMode->bContinue = TRUE;
		psParam->pMode->nReadDelay = Z_Get10msCounter();
		psParam->pMode->nHaltDelay = Z_Get10msCounter();
		psParam->pMode->nCodeDelay = Z_Get10msCounter();
		X_paramSaveAsSysDefaultParam();
		break;
		
	case 0x33: // ESC + 3 for CONTINUOUS READ
		psParam->pCode->nReadMode = ESC3;
		psParam->pMode->bContinue = TRUE;
		X_paramSaveAsSysDefaultParam();
		break;

	case 0x99:	//接收图像，进行解码测试。
		{
			BYTE rec[20];
			int i, len, nWidth, nHigh, err, k, wt;
			UTImgParam _s_Img;
			
			i = Y_commBakRead(rec, 1, 10);
			if (i<=0)
			{
				buff = 0x15; //其他表示错误
				break;
			}
			else 
			{
				if (rec[0] == 0)	//返回设备支持的图像大小
				{
// 					sprintf(b, "max pic size: %d\r\n", MaxPicBufSize);
// 					Y_commSendString(b);
					i = MaxPicBufSize;
					Y_commWrite((BYTE*)&i, 4);
					return FALSE;
				}
				else if (rec[0] == 1)	//接收图像数据
				{
					i = Y_commBakRead(rec, 8, 20);
					if (i!=8)
					{
						buff = 0x15; 
						Y_commWrite((BYTE*)&buff,1); //ACK = 0x06 NAK = 0x15
						return FALSE;
					}

					len = (rec[3]<<24) + (rec[2]<<16) + (rec[1]<<8) + rec[0];
					nWidth = (rec[5]<<8) + rec[4];
					nHigh = (rec[7]<<8) + rec[6];

					if ((len > MaxPicBufSize) || (nWidth*nHigh != len)) //接收的数据长度超过设备缓存的最大值
					{
						buff = 0x15; 
						Y_commWrite((BYTE*)&buff,1); //ACK = 0x06 NAK = 0x15
						return FALSE;
					}
					else
					{
						buff = 0x6; 
						Y_commWrite((BYTE*)&buff,1); //ACK = 0x06 NAK = 0x15
					}
	
					i = 0;
					err = 0;
					while(i<len)
					{
						k = Y_commBakRead((BYTE*)(picbuf+i), len-i, 500);
						if (k<=0)
						{
							printf("K: %d, i: %d\n", k, i);
							err++;
							if (err>4)
							{
								buff = 0x15; //其他表示错误
								Y_commWrite((BYTE*)&buff,1); //ACK = 0x06 NAK = 0x15
								return FALSE;
							}					
						}
						else
							i += k;
						Z_DogFeed();
					}

					Y_commWrite((BYTE*)&buff,1); //ACK = 0x06 
					testPicWidth = nWidth;
					testPicHigh = nHigh;
// 					printf("len:%d, w:%d, h:%d, tw:%d, th:%d\n", len, nWidth, nHigh, testPicWidth,testPicHigh);	
					return TRUE;

				}
				else if (rec[0] ==2)	//对图像解码
				{
					_s_Img.pSrc = (BYTE*)picbuf;					
					_s_Img.nWidth = testPicWidth;
					_s_Img.nHeight = testPicHigh;
					_s_Img.pTag = NULL;
					X_decoSetImgParam(&_s_Img);
					
					Z_SetPM_EX(FREQ_RUN_HIGH);

					printf("Start Decode....\r\n");
					i = Z_Get1msCounter();
					bPicDecodeSuc = X_decoImgProcMain();
					testTime = Z_Get1msCounter() - i;
					
					printf("decode time: %d\n", testTime);

					if (bPicDecodeSuc == TRUE)
					{
						Z_Beep(2730, 80, 10);

						U_appSetMsg(X_decoGetDecInfoBuff(),X_decoGetDecInfoLen());

						i = U_appGetMsgLen();
						Y_commWrite((BYTE*)&i, 4); 
						
						if(Y_commBakRead(rec, 1, 2000) != 1)
						{
							return TRUE;
						}

						if (rec[0] != 0x06)
						{
							buff = 0x15;
							Y_commWrite((BYTE*)&buff,1); 
							return TRUE;
						}
						
						Y_commWrite(U_appGetMsgBuff(),(unsigned int)U_appGetMsgLen()); 
						return TRUE;
						
					}
					else
					{
						i = 0;
						Y_commWrite((BYTE*)&i, 4); 
						return TRUE;
					}
					

				}
				else if (rec[0] == 3)	//发送接收到的数据
				{//原始图

					//应答图像的分辨率：宽2字节+高2字节，低字节在前，高字节在后
					i = ((testPicHigh<<16)&0xffff0000) | (testPicWidth&0xffff);
					Y_commWrite((BYTE*)&i, 4); 

					wt = Z_DogGetBreak(); //记录原先看门狗的复位时间
					Z_DogReset(-1);//禁用看门狗
					
					if (Y_commGetCurType() == COMMT_USB_Data)
					{
						Y_commWrite((BYTE*)picbuf, testPicWidth*testPicHigh);
					}
					else							
					{
						len = testPicWidth*testPicHigh/10240;
						k = testPicWidth*testPicHigh%10240;
						for (i=0; i<len; i++)
						{
							Y_commWrite((BYTE*)picbuf+i*10240, 10240);
						}
						if (k)
							Y_commWrite((BYTE*)picbuf+len-k, k);

					}

					Z_DogReset(wt); //恢复看门狗
					return TRUE;
				}
				else if (rec[0] == 4)	//发送解码耗时
				{
					Y_commWrite((BYTE*)&testTime, 4);
					return TRUE;
				}

				buff = 0x15; //其他表示错误
				break;
			}


		}


		break;
	default:
		buff = 0x15; //其他表示错误
		break;
	}

	Y_commWrite((BYTE*)&buff,1); //ACK = 0x06 NAK = 0x15
	return FALSE;
}

#define SurePOSReadPackSize		11
#define SurePOSSendPackSize		64

#define S0_FLASH_IN_PROGRESS		1
#define S0_CONFIG_DATA_RESPONSE		(1<<1)
#define S0_EANJAN12_2LABEL			(1<<2)
#define	S0_GOODREAD_BEEP			(1<<4)
#define	S0_HARDWARE_ERROR			(1<<5)
#define S0_DIRECT_IO_COMMAND		(1<<6)
#define S0_DEVICE_NO_READY			(1<<7)

#define S1_SCANNER_ALIVE					1
#define	S1_SCANNER_ENABLED					(1<<1)
#define S1_DATA_LENGTH_CHECKDIGIT_ERROR		(1<<2)
#define S1_UNDEFINED_COMMAND				(1<<7)

#define S2_CONFIG_SUCCESSFUL					1
#define S2_CONFIG_COERCED						(1<<1)
#define S2_EANJAN13_2LABEL_FLAG_CONFIG_SUCCESS	(1<<2)
#define S2_DIRECT_IO_COMMAND_ACCEPTED			(1<<3)
#define S2_DIRECT_IO_COMMAND_NOT_ALLOWED		(1<<4)
#define S2_DIRECT_IO_COMMAND_UNDEFINED			(1<<5)

#define BIT0	(1 << 0)
#define BIT1	(1 << 1)
#define BIT2	(1 << 2)
#define BIT3	(1 << 3)
#define BIT4	(1 << 4)
#define BIT5	(1 << 5)
#define BIT6	(1 << 6)
#define BIT7	(1 << 7)


typedef struct SurePosData 
{
	BYTE length;
	BYTE Status0;
	BYTE Status1;
	BYTE Status2;
	BYTE Data[60];
} SUREPOSDATA;

static BOOL bScannerEnable = FALSE;
static BOOL bCfg6Bit6 = FALSE;
static BOOL bCfg6Bit7 = FALSE;
static BOOL bSurePOSInit = FALSE;
static BOOL bSurePOSTickleFeedMode = FALSE;
//static BOOL bSurePOSTickleFeedStore = FALSE;

BOOL IsSurePOSOpen(void)
{
	int type;
	type = Y_commGetCurType();
	if((type == COMMT_IBM_SurePOS_TT) || (type == COMMT_IBM_SurePOS_HH))
		return TRUE;
	else
		return FALSE;
}

BOOL SurePOS_GetScannerState(void)
{
	return bScannerEnable;
}
void SurePOS_SetScannerState(BOOL bEnable)
{
	bScannerEnable = bEnable;
}

BOOL SurePOS_GetBeepState(void)
{
	if (X_paramGetGlobalAllParam()->pHard->bDecVoice & 1)
		return TRUE;
	else
		return FALSE;
}
void SurePOS_SetBeepState(BOOL bEnable)
{
	if(bEnable)
		X_paramGetGlobalAllParam()->pHard->bDecVoice |= 1;
	else
		X_paramGetGlobalAllParam()->pHard->bDecVoice &= ~1;
}

BOOL SurePOS_TickleFeed(void)
{
	return bSurePOSTickleFeedMode;
}

BOOL IsSurePOSInit(void)
{
	return bSurePOSInit;
}
void SurePOS_Init(void)
{
	//UPC-E传送前导字符'0'
	X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x08;
	bSurePOSInit = TRUE;

}

const unsigned short Label_Type_Id[66] = {
	//1D
	0x03FF,	0x03FF,	0x0318,	0x0325,//ZASETUP,SETUP128,CODE128,UCCEAN128
	0x010C,	0x0116,	0x010A,	0x010D,//EAN8,EAN13,UPCE,UPCA
	0x030D,	0x030D,	0x030D,	0x03FF,//INTER2OF5,ITF14,ITF6,MATRIX25
	0x03FF,	0x030A,	0x030A,	0x030E,//MODULE25,REGULAR39,FULLASCII39,CODABAR
	0x0000,	0x0319,	0x03FF,	0x0000,//2OUTOF5,CODE93,CODEZ,
	0x0000,	0x0000,	0x0000,	0x0000,//
	0x03FF,	0x03FF,	0x030C,	0x03FF,//ISBN,INDUSTRIAL25,STANDARD25,PLESSEY
	0x03FF,	0x03FF,	0x03FF,	0x032A,//CODE11,MSI_PLESSEY,COMPOSITE,RSS

	//2D
	0x032E,	0x0333,	0x0334,	0x0332,//PDF417,QR,Aztec,DataMatrix
	0x032F,	0x03FF,	0x03FF,	0x3FF,//MaxiCode,VeriCode,UN_VeriCode,HAN_SENSIBLE
	0,	0,	0,	0,
	0,	0,	0,	0,
	0,	0,	0,	0,
	0,	0,	0,	0,
	0,	0,	0,	0,
	0,	0,	0,	0,

	//OCR
	0x0331,	0
};

const unsigned short Label_Type_Id2[8] = {
	0x0317,	0x031D,	0x0313,	0x0315,//EAN8+2,EAN8+5,EAN13+2,EAN13+5
	0x0312,	0x0314,	0x0316,	0x0311,//UPCE+2,UPCE+5,UPCA+2,UPCA+5
};

#define BufMaxPackage	200	
int SurePOS_SendData(void)
{
	int nType;
	int nLabelIDLen;
	SUREPOSDATA packageData[BufMaxPackage];	//最大200个数据包 57*200 字节条码数据
	int nPackage = 0;
	int msglen = U_appGetMsgLen();
	BYTE *pMsgBuff = U_appGetMsgBuff();	
	int dataLen, readlen, len, sendlen;
	int status0, status1, status2;
	BYTE Label;
//	BYTE tmp[20];
	int lvi;
	UALLPARAM *pParam = X_paramGetGlobalAllParam();


	Z_DogFeed();

	switch(U_appGetCategory()){
	case UT_TYPE_1D:
		nType = X_decoGetCodePos();
		break;
	case UT_TYPE_2D:
		nType = 32 + X_decoGetCodePos();
		break;
	case UT_TYPE_OCR:
		nType = 64;
		break;
	default:
		return -1;
		break;
	}

// 	{
// 		char b[10000];
// 		sprintf(b, "Category: %#x,  CodeType: %d, Type: %d, len: %d\r\n", U_appGetCategory(), X_decoGetCodePos(), nType, X_decoGetDecInfoLen());
// 		Y_commSendString(b);
// 		sprintf(b, "%s\r\n", X_decoGetDecInfoBuff());
// 		Y_commSendString(b);
// 	}

	if (nType > 65)
		return -2;

	status0 = 0;
	status1 = S1_SCANNER_ALIVE|S1_SCANNER_ENABLED;
	status2 = 0;
	
	if (SurePOS_GetBeepState())
		status0 |= S0_GOODREAD_BEEP;

	nLabelIDLen = Label_Type_Id[nType] >> 8;
	Label = Label_Type_Id[nType] & 0xff;

	if (nType == 4)	//EAN8
	{
		if (msglen == 10) //EAN8+2
		{
			nLabelIDLen = Label_Type_Id2[0] >> 8;
			Label = Label_Type_Id2[0] & 0xff;
		}
		else if (msglen == 13) //EAN8+5
		{
			nLabelIDLen = Label_Type_Id2[1] >> 8;
			Label = Label_Type_Id2[1] & 0xff;
		}
		else if (msglen != 8)	//长度不符
		{
			nLabelIDLen = 0;
			Label = 0;
		}
	}
	else if (nType == 5)	//EAN13
	{
		if (msglen == 15) //EAN13+2
		{
			nLabelIDLen = Label_Type_Id2[2] >> 8;
			Label = Label_Type_Id2[2] & 0xff;
		}
		else if (msglen == 18) //EAN13+5
		{
			nLabelIDLen = Label_Type_Id2[3] >> 8;
			Label = Label_Type_Id2[3] & 0xff;
		}
		else if (msglen != 13)	//长度不符
		{
			nLabelIDLen = 0;
			Label = 0;
		}

	}
	else if (nType == 6)	//UPCE
	{


		if (!(pParam->p1D->ExMsg[POS_UPCE] & 0x01))	//校验符必须发送
		{
			nLabelIDLen = 0;
			Label = 0;
		}
		else if (msglen == 8)
		{
			nLabelIDLen = Label_Type_Id[nType] >> 8;
			Label = Label_Type_Id[nType] & 0xff;
		}
		else if (msglen == 10) //UPCE+2
		{
			nLabelIDLen = Label_Type_Id2[4] >> 8;
			Label = Label_Type_Id2[4] & 0xff;
		}
		else if (msglen == 13) //UPCE+5
		{
			nLabelIDLen = Label_Type_Id2[5] >> 8;
			Label = Label_Type_Id2[5] & 0xff;
		}
		else
		{
			nLabelIDLen = 0;
			Label = 0;
		}


	}
	else if (nType == 7)	//UPCA
	{

		if (!(pParam->p1D->ExMsg[POS_UPCA] & 0x01))	//校验符必须发送
		{
			nLabelIDLen = 0;
			Label = 0;
		}
		else if (msglen == 14) //UPA+2
		{
			nLabelIDLen = Label_Type_Id2[6] >> 8;
			Label = Label_Type_Id2[6] & 0xff;
		}
		else if (msglen == 17) //UPCA+5
		{
			nLabelIDLen = Label_Type_Id2[7] >> 8;
			Label = Label_Type_Id2[7] & 0xff;
		}
		else if (msglen != 12)	//长度不符
		{
			nLabelIDLen = 0;
			Label = 0;
		}

	}
	else if (nType == 31)	//RSS
	{
		if (X_decoGetRSSType(&lvi))
		{
			if (lvi == 3)	//RSS Expanded
			{
				Label = 0x2B;
			}
		}
	}

	if ((nLabelIDLen<0) && (nLabelIDLen>3))
		return -3;


	sendlen = 0;
	readlen = 0;
	dataLen = 60 - nLabelIDLen;

// 	{
// 		char b[100];
// 		sprintf(b, "Type: %d, len: %d,  LabelLen: %d, datalen: %d \r\n", nType, msglen, nLabelIDLen, dataLen);
// 		Y_commSendString(b);
// 	}


	while (msglen)
	{
		memset((char*)&packageData[nPackage], 0, sizeof(SUREPOSDATA));

		len = (msglen >= dataLen)?dataLen:msglen;

		packageData[nPackage].length = len + 4 + nLabelIDLen;
		packageData[nPackage].Status0 = status0;
		packageData[nPackage].Status1 = status1;
		packageData[nPackage].Status2 = status2;

		memcpy(packageData[nPackage].Data, pMsgBuff+readlen, len);
		readlen += len;

		//Label_Type_Id
		if (nLabelIDLen == 1)
		{
			packageData[nPackage].Data[len] = Label;
		}
		else if (nLabelIDLen == 2)
		{
			packageData[nPackage].Data[len] = (msglen>dataLen)?0x10:0;
			packageData[nPackage].Data[len+1] = Label;
		}
		else if (nLabelIDLen == 3)
		{
			packageData[nPackage].Data[len] = (msglen>dataLen)?0x10:0;
			packageData[nPackage].Data[len+1] = Label;
			packageData[nPackage].Data[len+2] = 0x0B;		//该字节固定为0Bh
		}

		msglen -= len;
		nPackage++;
		if (nPackage == BufMaxPackage)
		{
			sendlen = Y_commWrite((BYTE*)&packageData, nPackage*64);
			if (sendlen<(nPackage*64))
			{
				return sendlen;
			}
			nPackage = 0;
		}
		
	}

	if (nPackage)
	{
		sendlen += Y_commWrite((BYTE*)&packageData, nPackage*64);
	}

	return sendlen;
}

int Send_DirectIO_Return_Data(BYTE *buff, int buffLen)
{
	SUREPOSDATA packageData[BufMaxPackage];	//最大200个数据包 57*200 字节条码数据
	int nPackage = 0;
	int sendlen = 0;
	BYTE status0, status1, status2;
	int len;
	int readlen = 0;

	status0 = S0_DIRECT_IO_COMMAND;
	status1 = S1_SCANNER_ALIVE;
	status2 = S2_DIRECT_IO_COMMAND_ACCEPTED;
	
	if (SurePOS_GetBeepState())
		status0 |= S0_GOODREAD_BEEP;

	if(SurePOS_GetScannerState())
		status1 |= S1_SCANNER_ENABLED;

	while (buffLen)
	{
		memset((char*)&packageData[nPackage], 0, sizeof(SUREPOSDATA));
		
		if (buffLen>59)
		{
			packageData[nPackage].length = 64;
			packageData[nPackage].Data[0] = 0x10;
			len = 59;
		}
		else
		{
			packageData[nPackage].length = buffLen + 5;
			packageData[nPackage].Data[0] = 0;
			len = buffLen;
		}
		packageData[nPackage].Status0 = status0;
		packageData[nPackage].Status1 = status1;
		packageData[nPackage].Status2 = status2;
		
		memcpy(packageData[nPackage].Data+1, buff+readlen, len);

		readlen += len;		
		
		buffLen -= len;

		nPackage++;

		if (nPackage == BufMaxPackage)
		{
			Z_DogFeed();
			sendlen = Y_commWrite((BYTE*)&packageData, nPackage*64);
			Z_DogFeed();

			if (sendlen<(nPackage*64))
			{
				return sendlen;
			}
			nPackage = 0;
		}
		
	}
	
	if (nPackage)
	{
		sendlen += Y_commWrite((BYTE*)&packageData, nPackage*64);
	}
	
	return sendlen;
}


// int JpegCompress (BYTE* pSrcImg, BYTE* pJpegImg, unsigned short  nImgWidth, unsigned short  nImgHeight);

/*
	mode:		0	当前图像
				1	解码成功图像
				2	NCR -- 采集新的一幅图
	nformat:	0	原始图像
				1	jpeg - worst
				2	jpeg - better
				3	jpeg - even better
				4	jpeg - best
	nResolution:	0	VGA	（640*480）
					1	Native Resolution （原始分辨率）
	nInverted:	0	normal
				1	Inverted image
*/
/*
void NCR_IMG_Edit_Send(int mode, int nformat, int nResolution, int nInverted)
{
	int i;
	int pm;
	unsigned char byTmpData[480*752];
	unsigned char tmp[480*752];
	unsigned char tmp2[480*752];
	unsigned int nTmpLen = 0;
	int nWidth, nHigh;
	unsigned char *p = (unsigned char*)Z_cmos_GetIMGbuff_Ex();

	pm = Z_GetPM();
	if (pm>0)
	{
		Z_SetPM_EX(FREQ_416);
	}

	if (mode == 1)
	{
		p = (BYTE*)Img_GoodDecode;
	}
	else if (mode == 2)
	{

		NCR_Get_New_Img();

		p = (unsigned char*)Y_cmosGetDataAddr();

		if (nInverted == 1)
		{
			for (i=0; i<480; i++)
			{
				memcpy(tmp2+752*i, &p[752*(479-i)], 752);
			}
			p = tmp2;

		}


	}

	if (nResolution == 1)
	{
		nWidth = 752;
		nHigh = 480;
		nTmpLen = 752*480;
	}
	else
	{
		nWidth = 640;
		nHigh = 480;
		for (i=0; i<480; i++)
		{
			memcpy(byTmpData+nTmpLen, &p[752*i+56], 640);
			nTmpLen += 640;
		}
		p = byTmpData;
	}


	if (nformat > 0)
	{
// 		i = JpegCompress(byTmpData, tmp, 640, 480);
		i = GXIMG_BMPTOJPEGSelQulity(p, tmp, nWidth, nHigh, (nformat - 1));
		
		if (pm>0)
		{
			Z_SetPM_EX(pm);
		}

		Send_DirectIO_Return_Data(tmp, i);
		return;
	}
	

	if (pm>0)
	{
		Z_SetPM_EX(pm);
	}
	
	if (nTmpLen > 0)
	{
		Send_DirectIO_Return_Data(p, nTmpLen);
	}//End of if;

}
*/

/*
	void set_cmos_light_mode(int nMode);
	nMode:	bit[7:0]	illumination mode 内部照明灯模式
						0 = normal(按设置码设置的模式控制照明灯)
						1 = always on(忽略设置码参数)
						2 = always off(忽略设置码参数)
			bit[15:8]	aim mode 对焦灯模式
						0 = normal(按设置码设置的模式控制对焦灯)
						1 = always on(忽略设置码参数)
						2 = always off(忽略设置码参数)
						3 = 自动读码模式下，对焦灯设置为always on 或者 sensor mode的时候，如果照明灯不为always on模式，对焦灯可以作为侦测条码的辅助照明灯
			bit[23:16]	external illumination mode 外部照明灯模式
						0 = 联合模式(与内部照明灯同时亮、灭)
						1 = always on(忽略设置码参数)
						2 = always off(忽略设置码参数)
						3 = 独立模式(仅根据内部照明灯的设置码参数设置的模式控制亮灭)
 */
/*
static BOOL bGrab = FALSE;
static unsigned char Jpgbuf[752*480];
static int jpgLen = 0;
BOOL NCR_IMG_Grab(int light)
{
	int i;
	int pm;
	unsigned char byTmpData[480*752];
	unsigned int nTmpLen = 0;
	int nWidth, nHigh;
	unsigned char *p;
	int tmp;
	
	if (bGrab == TRUE)
	{
		return FALSE;
	}


	pm = Z_GetPM();
	if (pm>0)
	{
		Z_SetPM_EX(FREQ_416);
	}
	
	if (light&0x1)	//开照明灯
	{
		tmp = Z_GetCmosLightMode();
		tmp = (tmp & (~0xff00ff)) | 0x010001;
		Z_SetCmosLightMode(tmp);
		
	}
	else
	{
		tmp = Z_GetCmosLightMode();
		tmp = (tmp & (~0xffffff)) | 0x020202;
		Z_SetCmosLightMode(tmp);

	}
		
	
	NCR_Get_New_Img();
	
	p = (unsigned char*)Y_cmosGetDataAddr();
	
	nWidth = 640;
	nHigh = 480;
	for (i=0; i<480; i++)
	{
		memcpy(byTmpData+nTmpLen, &p[752*i+56], 640);
		nTmpLen += 640;
	}
	p = byTmpData;
	
	
	jpgLen = GXIMG_BMPTOJPEGSelQulity(p, Jpgbuf, nWidth, nHigh, 1);
		
	if (pm>0)
	{
		Z_SetPM_EX(pm);
	}
	
	bGrab = TRUE;

	//关灯，等待ship指令
	tmp = Z_GetCmosLightMode();
	tmp = (tmp & (~0xffffff)) | 0x020202;
	Z_SetCmosLightMode(tmp);
	return TRUE;
}

BOOL NCR_IMG_Ship(int light)
{
	int tmp;

	if (bGrab == FALSE)
	{
		return FALSE;
	}

	if (light&0x1)	//开照明灯
	{
		tmp = Z_GetCmosLightMode();
		tmp = (tmp & (~0xff00ff)) | 0x010001;
		Z_SetCmosLightMode(tmp);
		
	}
	else
	{
		tmp = Z_GetCmosLightMode();
		tmp = (tmp & (~0xffffff)) | 0x020202;
		Z_SetCmosLightMode(tmp);
		
	}

	Send_DirectIO_Return_Data(Jpgbuf, jpgLen);
	bGrab = FALSE;

	Z_DogFeed();
	Z_msDelay(700);
	//恢复原始状态
	tmp = Z_GetCmosLightMode();
	tmp &= (~0xffffff);
	Z_SetCmosLightMode(tmp);

	return TRUE;
}


NCRTrickleST ncrTrickle;

// #include <pxa-regs.h>
// int _time_start;

//mode:		0	固件保存
//			1	握手等
int NCR_Download(BYTE *buff, SUREPOSDATA *txbuf, int mode)	//
{
	int tmp;
	if (buff == NULL)
		return -1;

	if (txbuf == NULL)
		return -2;

	if ((mode == 1) && (buff[3] == 0x1))	//Enter Trickle Feed Mode
	{
		txbuf->length = 6;
		txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
		txbuf->Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;					
		if (bSurePOSTickleFeedMode)
		{
			txbuf->Data[0] = 0;
			txbuf->Data[1] = 0x6;
		}
		else
		{
			if ((bSurePOSTickleFeedStore == TRUE) && (ncrTrickle.pFWbuff != NULL))
			{
				bSurePOSTickleFeedStore = FALSE;
				bSurePOSTickleFeedMode = TRUE;

				//关灯
				tmp = Z_GetCmosLightMode();
				tmp = (tmp & (~0xffffff)) | 0x020202;
				Z_SetCmosLightMode(tmp);
			}
			else
			{
				tmp = Z_NCR_Trickle_Feed(&ncrTrickle, 0);
				if (tmp == 0)//初始化参数, 分配空间
				{
					bSurePOSTickleFeedStore = FALSE;
					bSurePOSTickleFeedMode = TRUE;

					//关灯
					tmp = Z_GetCmosLightMode();
					tmp = (tmp & (~0xffffff)) | 0x020202;
					Z_SetCmosLightMode(tmp);
				}
			}
// Dbg(1, "init:%d,  store:%d,  mode:%d \r\n", tmp, bSurePOSTickleFeedStore, bSurePOSTickleFeedMode);
			txbuf->Data[0] = 0;
			txbuf->Data[1] = 0x5;
		}
		return 0;
	}
		
	if (!bSurePOSTickleFeedMode)	//没进入Feed mode 返回
	{
		txbuf->length = 4;
		// 				txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
		txbuf->Status2 |= S2_CONFIG_COERCED;
		return 0;
	}

	if (ncrTrickle.pFWbuff == NULL)
		return -3;

	if (mode == 0)	//接收固件
	{
		txbuf->length = 4;
		//  				txbuf.Status0 |= S0_DIRECT_IO_COMMAND;
		txbuf->Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;	

		memcpy(ncrTrickle.pFWbuff+ncrTrickle.length,  buff+2, 9);
		ncrTrickle.length += 9;
		ncrTrickle.nOffset = ncrTrickle.length;

// 		Dbg(1, "FW len: %d, offset: %d \r\n", 
// 			ncrTrickle.length, ncrTrickle.nOffset);
// 
	}
	else if (mode == 1)
	{
		if (buff[3] == 0x2)	//download offset, host发送固件的起始偏移量
		{
			txbuf->length = 8;
			txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
			txbuf->Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;					
			txbuf->Data[0] = 0;

			txbuf->Data[1] = (ncrTrickle.nOffset>>16) && 0xff;
			txbuf->Data[2] = (ncrTrickle.nOffset>>8) && 0xff;
			txbuf->Data[3] = ncrTrickle.nOffset && 0xff;

// 			_time_start = OSCR;

// 			Dbg(1, "send len: %d, offset: %d \r\n", 
// 				ncrTrickle.length, ncrTrickle.nOffset);

		}
		else if (buff[3] == 0x4)	//传送固件结束，计算crc32
		{
// Dbg(1, "time: %d \r\n", (OSCR-_time_start)/3250);
			if(Z_NCR_Trickle_Feed(&ncrTrickle, 1) < 0)
			{
				//失败
				txbuf->length = 4;
// 				txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
				txbuf->Status2 |= S2_CONFIG_COERCED;
			}
			else
			{
				txbuf->length = 4;
// 				txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
				txbuf->Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;
			}
// 			Dbg(1, "cmp: len: %d, offset: %d, crc: %08x \r\n", 
// 				ncrTrickle.length, ncrTrickle.nOffset,  ncrTrickle.crc32);

		}
		else if (buff[3] == 0x5)	//暂停，计算crc32，保存已有数据
		{
// 			if(Z_NCR_Trickle_Feed(&ncrTrickle, 1) < 0)
// 			{
// 				//失败
// 				txbuf->length = 4;
// 				// 				txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
// 				txbuf->Status2 |= S2_CONFIG_COERCED;
// 			}
// 			else
			{
				txbuf->length = 4;
				// 				txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
				txbuf->Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;
			}
			bSurePOSTickleFeedStore = TRUE;		
			bSurePOSTickleFeedMode = FALSE;	
			
			//恢复原来灯光状态
			tmp = Z_GetCmosLightMode();
			tmp &= (~0xffffff);
			Z_SetCmosLightMode(tmp);

// 			Dbg(1, "pause: len: %d, offset: %d, crc: %08x \r\n", 
// 				ncrTrickle.length, ncrTrickle.nOffset,  ncrTrickle.crc32);
		}
		else if (buff[3] == 0x6)	//Terminate，退出，不保存任何参数
		{
			if(Z_NCR_Trickle_Feed(&ncrTrickle, 3) < 0)
			{
				//失败
				txbuf->length = 4;
				// 				txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
				txbuf->Status2 |= S2_CONFIG_COERCED;
			}
			else
			{
				txbuf->length = 4;
				// 				txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
				txbuf->Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;
				bSurePOSTickleFeedStore = TRUE;						//
			}
			bSurePOSTickleFeedStore = FALSE;
			bSurePOSTickleFeedMode = FALSE;

			//恢复原来灯光状态
			tmp = Z_GetCmosLightMode();
			tmp &= (~0xffffff);
			Z_SetCmosLightMode(tmp);

// 			Dbg(1, "Terminate: len: %d, offset: %d, crc: %08x \r\n", 
// 				ncrTrickle.length, ncrTrickle.nOffset,  ncrTrickle.crc32);
			
		}
		else if (buff[3] == 0x7)	//Activate
		{

// Dbg(1, "state: %d\r\n", ncrTrickle.flashState);
			if (ncrTrickle.flashState == 0)
			{
				txbuf->length = 6;
				txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
				txbuf->Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;					
				txbuf->Data[0] = 0;
				txbuf->Data[1] = 0x5;
				if(Z_NCR_Trickle_Feed(&ncrTrickle, 2) < 0)	// flash update
				{
					txbuf->Data[1] = 0x0;		//error
				}

			}
			else if (ncrTrickle.flashState == 1)
			{
				txbuf->length = 6;
				txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
				txbuf->Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;					
				txbuf->Data[0] = 0;
//				txbuf->Data[1] = 0x6;
 				txbuf->Data[1] = 0x5;
			}
			else if (ncrTrickle.flashState == 2)
			{
				txbuf->length = 6;
				txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
				txbuf->Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;					
				txbuf->Data[0] = 0;
				txbuf->Data[1] = 0x6;
				
				if(SurePOS_GetScannerState())
					txbuf->Status1 |= S1_SCANNER_ENABLED;
				
				if (SurePOS_GetBeepState())
					txbuf->Status0 |= S0_GOODREAD_BEEP;
				
				Y_commWrite((BYTE*)txbuf, txbuf->length);
				Z_NCR_Trickle_Feed(&ncrTrickle, 3);
				bSurePOSTickleFeedStore = FALSE;
				bSurePOSTickleFeedMode = FALSE;

				//恢复原来灯光状态
				tmp = Z_GetCmosLightMode();
				tmp &= (~0xffffff);
				Z_SetCmosLightMode(tmp);

				Z_msDelay(50);
				RebootDevice();

			}
			else
			{
				txbuf->length = 6;
				txbuf->Status0 |= S0_DIRECT_IO_COMMAND;
				txbuf->Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;					
				txbuf->Data[0] = 0;
				txbuf->Data[1] = 0;
			}

		}
	}

	return 0;
}
*/

void DealSurePOSCommand(BYTE *buff)
{
	unsigned int tmp;
	UALLPARAM *pParam = X_paramGetGlobalAllParam();
	SUREPOSDATA txbuf;
	unsigned int itf_max, itf_min; 
	char lvbuf[300];

	txbuf.length = 4;
	txbuf.Status0 = 0;
	txbuf.Status1 = S1_SCANNER_ALIVE;
	txbuf.Status2 = 0;
	memset(txbuf.Data, 0, sizeof(txbuf.Data));

	switch (buff[0])
	{
	case 0:
		switch (buff[1])
		{
		case 0x10:		//Test

			break;

		case 0x20:		//Status request

			break;

		case 0x40:		//Reset
			RebootDevice();
			return;

		default:
			txbuf.Status1 |= S1_UNDEFINED_COMMAND;
			break;
		}

		break;

	case 0x11:		//Scanner enable
		SurePOS_SetScannerState(TRUE);
		tmp = Z_GetCmosLightMode();
		tmp &= (~0xffffff);
		Z_SetCmosLightMode(tmp);
		pParam->pMode->nReadDelay = Z_Get10msCounter();
		break;

	case 0x12:		//Scanner disable
		SurePOS_SetScannerState(FALSE);
		tmp = Z_GetCmosLightMode();
		tmp = (tmp & (~0xffffff)) | 0x020202;
		Z_SetCmosLightMode(tmp);
		break;

	case 0x14:		//Beeper enable
		SurePOS_SetBeepState(TRUE);
		break;

	case 0x18:		//Beeper disable
		SurePOS_SetBeepState(FALSE);
		break;

	case 0x20:		//Configure scanner
		txbuf.Status2 = 3;

		//cfg0
		//cfg0.bit0 Enable UPC-A/E, EAN 8/13
		if (buff[2] & BIT0)
			pParam->p1D->unDecTypes |= (DC_EAN13 | DC_EAN8 | DC_UPCA | DC_UPCE);
		else
			pParam->p1D->unDecTypes &= ~(DC_EAN13 | DC_EAN8 | DC_UPCA | DC_UPCE);
	
		//cfg0.bit1 UPC D1...D5 不支持

		//cfg0.bit2 Enable Code39
		if (buff[2] & BIT2)
			pParam->p1D->unDecTypes |= DC_REGULAR39;
		else
			pParam->p1D->unDecTypes &= ~ DC_REGULAR39;

		//cfg0.bit3 Enable Interleaved 25
		if (buff[2] & BIT3)
		{
			pParam->p1D->unDecTypes |= DC_INTER2OF5;
			pParam->p1D->unDecTypes &= ~ DC_ITF14;
			pParam->p1D->unDecTypes &= ~ DC_ITF6;
		}
		else
		{
			pParam->p1D->unDecTypes &= ~ DC_INTER2OF5;
			pParam->p1D->unDecTypes &= ~ DC_ITF14;
			pParam->p1D->unDecTypes &= ~ DC_ITF6;
		}

		//cfg0.bit4 Enable Codabar
		if (buff[2] & BIT4)
			pParam->p1D->unDecTypes |= DC_CODABAR;
		else
			pParam->p1D->unDecTypes &= ~ DC_CODABAR;

		//cfg0.bit5 Enable Code93
		if (buff[2] & BIT5)
			pParam->p1D->unDecTypes |= DC_CODE93;
		else
			pParam->p1D->unDecTypes &= ~ DC_CODE93;

		//cfg0.bit6 Enable Code 128
		if (buff[2] & BIT6)
			pParam->p1D->unDecTypes |= DC_CODE128;
		else
			pParam->p1D->unDecTypes &= ~ DC_CODE128;
		
		//cfg0.bit7 Enable UCC/EAN 128
		if (buff[2] & BIT7)
			pParam->p1D->unDecTypes |= DC_UCCEAN128;
		else
			pParam->p1D->unDecTypes &= ~ DC_UCCEAN128;
		
		/////////////////////////////////////////////////
		//cfg1
//		//cfg1.bit0 Enable 2-digit supplementals
		if (buff[3] & BIT0)
		{
			pParam->p1D->ExMsg[POS_EAN8] |= 0x02;
			pParam->p1D->ExMsg[POS_EAN13] |= 0x02;
			pParam->p1D->ExMsg[POS_UPCE] |= 0x02;
			pParam->p1D->ExMsg[POS_UPCA] |= 0x02;
		}
		else
		{
			pParam->p1D->ExMsg[POS_EAN8] &= (~0x02);
			pParam->p1D->ExMsg[POS_EAN13] &= (~0x02);
			pParam->p1D->ExMsg[POS_UPCE] &= (~0x02);
			pParam->p1D->ExMsg[POS_UPCA] &= (~0x02);
		}

//		//cfg1.bit1 Enable 5-digit supplementals
		if (buff[3] & BIT1)
		{
			pParam->p1D->ExMsg[POS_EAN8] |= 0x04;
			pParam->p1D->ExMsg[POS_EAN13] |= 0x04;
			pParam->p1D->ExMsg[POS_UPCE] |= 0x04;
			pParam->p1D->ExMsg[POS_UPCA] |= 0x04;
		}
		else
		{
			pParam->p1D->ExMsg[POS_EAN8] &= (~0x04);
			pParam->p1D->ExMsg[POS_EAN13] &= (~0x04);
			pParam->p1D->ExMsg[POS_UPCE] &= (~0x04);
			pParam->p1D->ExMsg[POS_UPCA] &= (~0x04);
		}

//		//cfg1.bit2 Enable Enable Code128 supplementals	 不支持	

		
		//cfg1.bit3 Enable UPC-A check Digit
		if (buff[3] & BIT3)	
			pParam->p1D->ExMsg[POS_UPCA] |= 0x01;	//传送校验符
		else
			pParam->p1D->ExMsg[POS_UPCA] &= ~0x01;
		
		
		//cfg1.bit4 Enable UPC-E check Digit
		if (buff[3] & BIT4)	
			pParam->p1D->ExMsg[POS_UPCE] |= 0x01;	//传送校验符
		else
			pParam->p1D->ExMsg[POS_UPCE] &= ~0x01;
		
		//cfg1.bit5 Enable Code39 check Digit
		if (buff[3] & BIT5)	
			pParam->p1D->ExMsg[POS_REGULAR39] |= 0x03;	//校验并发送校验符
		else
			pParam->p1D->ExMsg[POS_REGULAR39] &= ~0x03;		//不校验

		//cfg1.bit6 Enable ITF check Digit
		if (buff[3] & BIT6)	
		{
			pParam->p1D->ExMsg[POS_INTER2OF5] &= ~0x03;	
			pParam->p1D->ExMsg[POS_INTER2OF5] |= 0x1;	//校验不发送校验符
		}
		else
			pParam->p1D->ExMsg[POS_INTER2OF5] &= ~0x03;		//不校验
		
		//cfg1.bit7 Enable Standard 25
		if (buff[3] & BIT7)	
			pParam->p1D->unDecTypes |= DC_STANDARD25;	
		else
			pParam->p1D->unDecTypes &= ~ DC_STANDARD25;	


		////////////////////////////////////////////////
		//cfg2
		//cfg2.bit0 Enable EAN/JAN 2-Label decode	不支持
			
		//cfg2.bit1 Enable UPC-A to EAN13 expansion		不支持
		
		//cfg2.bit2 Enable UPC-E to EAN13 expansion		不支持	
		
		//cfg2.bit3 Enable UPC-E to UPC-A expansion
		if (buff[4] & BIT3)	
		{
			pParam->p1D->ExMsg[POS_UPCE] |= 0x80;
			pParam->p1D->ExMsg[POS_UPCE] &= ~0x10;	
		}
		else
		{
			pParam->p1D->ExMsg[POS_UPCE] &= ~0x80;			
			pParam->p1D->ExMsg[POS_UPCE] &= ~0x10;
		}

		//cfg2.bit4 Enable verification of UPC-A/EAN13 4-digit price check character	不支持

		//cfg2.bit5 Enable verification of UPC-A/EAN13 5-digit price check character	不支持
		
		//cfg2.bit6 Reserved
		
		//cfg2.bit7 Reserved 

		////////////////////////////////////////////////
		//EM2037: bit7 - bit4	音量 0 = high; 1 = Medium; 2 = low; 3 = off
		//EM2037: bit11 - bit8	持续时间 0 = Medium; 1 = short
		//EM2037: bit15 - bit12	频率	0 = Medium; 1 = Lowest; 2 = Low; 3 = Medium; 4 = High

		//cfg3
		//cfg3.bit0 Enable good-read beep	不支持
		
		//cfg3.bit2-1 Beeper volume: 0 = Lowest; 1 = Low; 2 = Medium; 3 = High.
			//EM2037: bit7 - bit4	音量 0 = high; 1 = Medium; 2 = low; 3 = off
		tmp = 3 - ((buff[5]>>1) & 0x3);
		pParam->pHard->nBeepType &= ~0xf0;
		pParam->pHard->nBeepType |= (tmp<<4);
		
		//cfg3.bit4-3 Beeper frequecy: 0 = Lowest; 1 = Low; 2 = Medium; 3 = High.
			//EM2037: bit15 - bit12	频率	0 = Medium; 1 = Lowest; 2 = Low; 3 = Medium; 4 = High
		tmp = ((buff[5]>>3) & 0x3) + 1;
		pParam->pHard->nBeepType &= ~0xf000;
		pParam->pHard->nBeepType |= (tmp<<12);

		
		//cfg3.bit6-5 Beeper Duration: 0 = short; 1 = Medium; 2 = Long; 3 = longest.
			//EM2037: bit11 - bit8	持续时间 0 = Medium; 1 = short
		tmp = (buff[5]>>5) & 0x3;
		pParam->pHard->nBeepType &= ~0xf00;
		if (tmp == 0)	
			pParam->pHard->nBeepType |= (1<<8);			
		
		//cfg3.bit7 Reserved 

		////////////////////////////////////////////////
		//cfg4
		//cfg4.bit2-0 Motor time-out 不支持
		//cfg4.bit4-3 Laser timer-out 不支持		
		//cfg4.bit6-5 Double-read time-out 不支持
		//cfg4.bit7 Reserved 

		////////////////////////////////////////////////
		//cfg5
		//cfg5.bit1-0 Security/Integrity level for in-store labels	 不支持			
		//cfg5.bit7-2 Reserved 

		////////////////////////////////////////////////
		//cfg6
		//cfg6.bit5-0 ITF Length 1			
		//cfg6.bit6 1 = Two ITF lengths specified; 0 = One ITF lengths specified;
		//cfg6.bit7 1 = ITF lengths represent a range; 0 = ITF lengths are discrete; 

		//cfg7
		//cfg7.bit5-0 ITF Length 2			
		//cfg7.bit7-6 Reserved
		pParam->p1D->I2Of5Len[0] = 0;
		tmp = buff[8] & 0x3f;
		itf_min = (pParam->p1D->ExMsg[POS_INTER2OF5]>>16) & 0x3f;
		itf_max = (pParam->p1D->ExMsg[POS_INTER2OF5]>>24) & 0x3f;
		if (buff[8] & BIT6)
		{
			bCfg6Bit6 = TRUE;
			bCfg6Bit7 = TRUE;
			buff[9] &= 0x3f;
// 			if ((tmp != 0) || (buff[9] != 0))
			{
				if (tmp == 0)
				{
					if (buff[9] >= itf_min)
					{
						itf_max = buff[9];
					}
				}
				else if (buff[9] == 0)
				{
					if (tmp >= itf_min)
					{
						itf_max = tmp;
					}
				}
				else if (tmp && buff[9])
				{
					if (tmp > buff[9])
					{
						itf_max = tmp;
						itf_min = buff[9];
					}
					else
					{
						itf_max = buff[9];
						itf_min = tmp;
					}

				}
					
				pParam->p1D->ExMsg[POS_INTER2OF5] &= 0x0000ffff;
				pParam->p1D->ExMsg[POS_INTER2OF5] |= ((itf_min<<16) | ((itf_max)<<24));
			}

		}
		else 
		{
			bCfg6Bit6 = FALSE;
			if (buff[8] & BIT7)
				bCfg6Bit7 = TRUE;
			else
				bCfg6Bit7 = FALSE;

			if (tmp)
			{
				pParam->p1D->ExMsg[POS_INTER2OF5] &= 0x0000ffff;
				if (tmp>itf_max)
					pParam->p1D->ExMsg[POS_INTER2OF5] |= ((itf_min<<16) | ((tmp)<<24));
				else
					pParam->p1D->ExMsg[POS_INTER2OF5] |= ((tmp<<16) | (tmp<<24));
			}
		}

		
		////////////////////////////////////////////////
		//cfg8
		//cfg8.bit1-0 LED good-read duration:  0 = short; 1 = Medium; 2 = Long; 3 = longest.
		tmp = buff[10] & 0x3;
		pParam->pHard->nGoodreadLEDDuration = tmp;			
				
		//cfg8.bit2 Enable progamming via bar codes	使能通过设置码设置 
		if (buff[10] & BIT2)
		{
			pParam->p1D->unDecTypes |= DC_ZASETUP;
		}
		else
			pParam->p1D->unDecTypes &= ~DC_ZASETUP;
		
		//cfg8.bit3 Enable Laser ON/OFF Swith 不支持
		
		//cfg8.bit4 Enable volume switch 不支持
		
		//cfg8.bit7-5 Reserved 

		break;

	case 0x21:		//Report scanner configuration
		txbuf.length = 13;
		txbuf.Status0 |= S0_CONFIG_DATA_RESPONSE;
		memset(txbuf.Data, 0, 10);

		//cfg0.bit0 Enable UPC-A/E, EAN 8/13
		tmp = pParam->p1D->unDecTypes & (DC_EAN13 | DC_EAN8 | DC_UPCA | DC_UPCE);
		if(tmp == (DC_EAN13 | DC_EAN8 | DC_UPCA | DC_UPCE))
			txbuf.Data[0] |= BIT0;

		//cfg0.bit1 UPC D1...D5 不支持
		
		//cfg0.bit2 Enable Code39
		if (pParam->p1D->unDecTypes & DC_REGULAR39)
			txbuf.Data[0] |= BIT2;

		//cfg0.bit3 Enable Interleaved 25
		if (pParam->p1D->unDecTypes & DC_INTER2OF5)
			txbuf.Data[0] |= BIT3;

		//cfg0.bit4 Enable Codabar
		if (pParam->p1D->unDecTypes & DC_CODABAR)
			txbuf.Data[0] |= BIT4;

		//cfg0.bit5 Enable Code93
		if (pParam->p1D->unDecTypes & DC_CODE93)
			txbuf.Data[0] |= BIT5;
				
		//cfg0.bit6 Enable Code 128
		if (pParam->p1D->unDecTypes & DC_CODE128)
			txbuf.Data[0] |= BIT6;

		//cfg0.bit7 Enable UCC/EAN 128
		if (pParam->p1D->unDecTypes & DC_UCCEAN128)
			txbuf.Data[0] |= BIT7;


				/////////////////////////////////////////////////
		//cfg1
//		//cfg1.bit0 Enable 2-digit supplementals

			if ((pParam->p1D->ExMsg[POS_EAN8] & 0x02) &&
			   (pParam->p1D->ExMsg[POS_EAN13] & 0x02) &&
			   (pParam->p1D->ExMsg[POS_UPCE] & 0x02) &&
			   (pParam->p1D->ExMsg[POS_UPCA] & 0x02))
				txbuf.Data[1] |= BIT0;
		
		//cfg1.bit1 Enable 5-digit supplementals
			if ((pParam->p1D->ExMsg[POS_EAN8] & 0x04) &&
				(pParam->p1D->ExMsg[POS_EAN13] & 0x04) &&
				(pParam->p1D->ExMsg[POS_UPCE] & 0x04) &&
				(pParam->p1D->ExMsg[POS_UPCA] & 0x04))
				txbuf.Data[1] |= BIT1;
		
//		//cfg1.bit2 Enable Enable Code128 supplementals		不支持

		
		//cfg1.bit3 Enable UPC-A check Digit
		if (pParam->p1D->ExMsg[POS_UPCA] & 0x01)
			txbuf.Data[1] |= BIT3;		
		
		//cfg1.bit4 Enable UPC-E check Digit
		if (pParam->p1D->ExMsg[POS_UPCE] & 0x01)
			txbuf.Data[1] |= BIT4;
		
		//cfg1.bit5 Enable Code39 check Digit
		if ((pParam->p1D->ExMsg[POS_REGULAR39] & 0x03) == 0x3)//校验并发送校验符
			txbuf.Data[1] |= BIT5;

		//cfg1.bit6 Enable ITF check Digit
		if ((pParam->p1D->ExMsg[POS_INTER2OF5] & 0x03) == 1)	//校验并发送校验符
			txbuf.Data[1] |= BIT6;

		//cfg1.bit7 Enable Standard 25
		if (pParam->p1D->unDecTypes & DC_STANDARD25)
			txbuf.Data[1] |= BIT7;


		////////////////////////////////////////////////
		//cfg2
		//cfg2.bit0 Enable EAN/JAN 2-Label decode	不支持
			
		//cfg2.bit1 Enable UPC-A to EAN13 expansion		不支持
		
		//cfg2.bit2 Enable UPC-E to EAN13 expansion		不支持	
		
		//cfg2.bit3 Enable UPC-E to UPC-A expansion
		if (pParam->p1D->ExMsg[POS_UPCE] & 0x80)
			txbuf.Data[2] |= BIT3;

		//cfg2.bit4 Enable verification of UPC-A/EAN13 4-digit price check character	不支持

		//cfg2.bit5 Enable verification of UPC-A/EAN13 5-digit price check character	不支持
		
		//cfg2.bit6 Reserved
		
		//cfg2.bit7 Reserved 

		////////////////////////////////////////////////
		//cfg3
		//cfg3.bit0 Enable good-read beep	不支持
		
		//cfg3.bit2-1 Beeper volume: 0 = Lowest; 1 = Low; 2 = Medium; 3 = High.
			//EM2037: bit7 - bit4	音量 0 = high; 1 = Medium; 2 = low; 3 = off
		tmp = 3 - ((pParam->pHard->nBeepType>>4) & 0x03);
		txbuf.Data[3] |= (tmp<<1);

		//cfg3.bit4-3 Beeper frequecy: 0 = Lowest; 1 = Low; 2 = Medium; 3 = High.
			//EM2037: bit15 - bit12	频率	0 = Medium; 1 = Lowest; 2 = Low; 3 = Medium; 4 = High
		tmp = (pParam->pHard->nBeepType>>12) & 0x0f;
		if (tmp == 0)
			tmp = 2;
		else
			tmp = (tmp - 1) & 0x3;
		txbuf.Data[3] |= (tmp<<3);
		
		//cfg3.bit6-5 Beeper Duration: 0 = short; 1 = Medium; 2 = Long; 3 = longest.
			//EM2037: bit11 - bit8	持续时间 0 = Medium; 1 = short
		tmp = (pParam->pHard->nBeepType>>8) & 0x01;
		if (tmp == 0)
			txbuf.Data[3] |= (1<<5);
		
		//cfg3.bit7 Reserved 

		////////////////////////////////////////////////
		//cfg4
		//cfg4.bit2-0 Motor time-out 不支持
		//cfg4.bit4-3 Laser timer-out 不支持		
		//cfg4.bit6-5 Double-read time-out 不支持
		//cfg4.bit7 Reserved 

		////////////////////////////////////////////////
		//cfg5
		//cfg5.bit1-0 Security/Integrity level for in-store labels	 不支持			
		//cfg5.bit7-2 Reserved 

		////////////////////////////////////////////////
		//cfg6
		//cfg6.bit5-0 ITF Length 1			
		//cfg6.bit6 1 = Two ITF lengths specified; 0 = One ITF lengths specified;
		//cfg6.bit7 1 = ITF lengths represent a range; 0 = ITF lengths are discrete; 

		//cfg7
		//cfg7.bit5-0 ITF Length 2			
		//cfg7.bit7-6 Reserved

		txbuf.Data[6] |= ((bCfg6Bit6<<6)|(bCfg6Bit7<<7));
		itf_min = (pParam->p1D->ExMsg[POS_INTER2OF5]>>16) & 0x3f;
		itf_max = (pParam->p1D->ExMsg[POS_INTER2OF5]>>24) & 0x3f;
		txbuf.Data[6] |= itf_min;
		txbuf.Data[7] = itf_max;

	
		////////////////////////////////////////////////
		//cfg8
		//cfg8.bit1-0 LED good-read duration:  0 = short; 1 = Medium; 2 = Long; 3 = longest.
		tmp = pParam->pHard->nGoodreadLEDDuration & 0x3;
		txbuf.Data[8] |= tmp;
				
		//cfg8.bit2 Enable progamming via bar codes	使能通过设置码设置 
		if (pParam->p1D->unDecTypes & DC_ZASETUP)
			txbuf.Data[8] |= BIT2;
		
		//cfg8.bit3 Enable Laser ON/OFF Swith 不支持
		
		//cfg8.bit4 Enable volume switch 不支持
		
		//cfg8.bit7-5 Reserved 

		break;

	case 0x23:		//Configuration EAN/JAN-13 Two-Label Flags
		txbuf.Status0 |= S0_EANJAN12_2LABEL;
		break;

	case 0x34:		//Report EAN/JAN-13 Two-Label Flag configuration
		txbuf.length = 12;
		txbuf.Status0 |= S0_EANJAN12_2LABEL;
		break;

	case 0x30:		//Direct I/O command
		if ((buff[1]==0xff))	//IBM
		{
			if (buff[2]==0xff)
			{
				txbuf.length = 4;

// 				txbuf.Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;

				//sym0.bit0 Enable RSS, RSS Stacked, and RSS Limited
				if (buff[3] & BIT0)
					pParam->p1D->unDecTypes |= (DC_RSS);
				else
					pParam->p1D->unDecTypes &= ~(DC_RSS);

				//sym0.bit1 Enable RSS Expanded

				//sym0.bit2 Enable Enable RSS Composites

				//sym0.bit3 Enable PDF417
				if (buff[3] & BIT3)
					pParam->p2D->unDecTypes |= (0x01 << POS_PDF417);
				else
					pParam->p2D->unDecTypes &= ~(0x01 << POS_PDF417);

				//sym0.bit4 Enable MaxiCode
				if (buff[3] & BIT4)
					pParam->p2D->unDecTypes |= (0x01 << POS_MaxiCode);
				else
					pParam->p2D->unDecTypes &= ~(0x01 << POS_MaxiCode);

				//sym0.bit5 Enable OCR
				if (buff[3] & BIT5)
					pParam->pOCR->unDecTypes |= (0x01 << POS_SPEC_OCR_B);
				else
					pParam->pOCR->unDecTypes &= ~(0x01 << POS_SPEC_OCR_B);

				//sym0.bit7-6 Reserved


				break;
			}
			else if (buff[2] == 0xfe)
			{
				txbuf.length = 7;
				txbuf.Status0 |= S0_DIRECT_IO_COMMAND;
				txbuf.Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;
				txbuf.Data[0] = 0;
				txbuf.Data[1] = 0x1e;
				txbuf.Data[2] = 0xab;
				break;
			}

		}
/*		else if ((buff[1]==0x01))	//NL
		{
			if (buff[2]==0x10)
			{
				if (buff[3] == 0x1)//always on
				{
					pParam->pHard->nLight = 1;
					txbuf.length = 4;
					txbuf.Status0 |= S0_DIRECT_IO_COMMAND;
					txbuf.Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;					
					break;
				}
				else if (buff[3] == 0x2)//image export
				{
					NCR_IMG_Edit_Send(buff[4], buff[5], buff[6], buff[7]);
// 					NCR_IMG_Edit_Send(1);
					return;
				}
				else if (buff[3] == 0x3)//版本信息
				{
					sprintf(lvbuf, "%s", UIMAGE_PRI_VERSION);
					tmp = Z_GetHardwareInfo_NCR(lvbuf, sizeof(lvbuf));
					Send_DirectIO_Return_Data((BYTE*)lvbuf, tmp);
					return; 
				}
				else if (buff[3] == 0x4)
				{
					X_paramSendParamInfo();
					return;
				}

			}
		}

		else if (buff[1] == 0x30)	//NCR Download
		{
			if ((buff[2] == 0x41) && (buff[3] == 0x30))
			{
				//Query asset info
				sprintf(lvbuf, "%s", UIMAGE_PRI_VERSION);
				tmp = Z_GetHardwareInfo_NCR(lvbuf, sizeof(lvbuf));
				Send_DirectIO_Return_Data((BYTE*)lvbuf, tmp);
				return;
			}
			else if (buff[2] == 0x74)
			{
				NCR_Download(buff, &txbuf, 1);
				break;
			}
// 			else if ((buff[2] == 0x48) && (buff[3] == 0x30))
// 			{
// 
// 			}
		}
		else if (buff[1] == 0x1B)	//FW data
		{
			if (bSurePOSTickleFeedMode)	//保存Fw数据	
			{
				//保存数据
				NCR_Download(buff, &txbuf, 0);	
				break;
			}	
			else
			{
				txbuf.length = 4;
				// 					txbuf.Status0 |= S0_DIRECT_IO_COMMAND;
				txbuf.Status2 |= S2_CONFIG_COERCED;
				break;
			}
			
		}
		else if (buff[1] == 0x49)	//NCR DirectIO command
		{
			if (buff[2] == 0x4E)	//Image Grab
			{
				if (NCR_IMG_Grab(buff[3]))
				{
					txbuf.length = 4;
					txbuf.Status2 |= S2_DIRECT_IO_COMMAND_ACCEPTED;					
				}
				else
				{
					txbuf.length = 4;
					txbuf.Status2 |= S2_DIRECT_IO_COMMAND_NOT_ALLOWED;
				}
				break;
			}
			else if (buff[2] == 0x48)	//Image Ship
			{
				if (NCR_IMG_Ship(buff[3]))
					return;
				txbuf.length = 4;
				txbuf.Status2 |= S2_DIRECT_IO_COMMAND_NOT_ALLOWED;
				break;
			}
			else if (buff[2] == 0x50)	//Simple Image Grab & Ship Command
			{
				NCR_IMG_Edit_Send(2, ((buff[3]>>2)&0x3)+1, buff[3]&0x1, (buff[3]>>4)&0x1);
				return;
			}
		}
*/

		else if ((buff[1] == 0) && (buff[2] == 0x20))
		{
			sprintf(lvbuf, "%s", UIMAGE_PRI_VERSION);
			tmp = Z_GetHardwareInfo_NCR(lvbuf, sizeof(lvbuf));
			Send_DirectIO_Return_Data((BYTE*)lvbuf, tmp);
			return; 
		}

		txbuf.length = 4;
		txbuf.Status2 |= S2_DIRECT_IO_COMMAND_UNDEFINED;

		break;

	default:
		txbuf.Status2 |= S2_CONFIG_SUCCESSFUL | S2_CONFIG_COERCED;
	}


	if(SurePOS_GetScannerState())
		txbuf.Status1 |= S1_SCANNER_ENABLED;

	if (SurePOS_GetBeepState())
		txbuf.Status0 |= S0_GOODREAD_BEEP;

	Y_commWrite((BYTE*)&txbuf, txbuf.length);
}


void V_commAutoCheckCmd(void)
{
	int nLength;
	char cmd;
	char buff_Out[1024];
	int type;

	type = Y_commGetCurType();

	if((type == COMMT_IBM_SurePOS_TT) || (type == COMMT_IBM_SurePOS_HH))
	{

// 		while (Y_commBakBuffCount()>=SurePOSReadPackSize)
// 		{
// 			Y_commBakRead((BYTE*)buff_Out, SurePOSReadPackSize, 0);
// 			DealSurePOSCommand((BYTE*)buff_Out);
// 		}

		do 
		{
			Z_DogFeed();

			while (Y_commBakReadChar(buff_Out,0))
			{
// Dbg(1, "Z:%d\r\n", OSCR/3250);
				nLength = Y_commBakRead((BYTE*)buff_Out+1, SurePOSReadPackSize-1, 10);
				if (nLength != (SurePOSReadPackSize-1))
					break;
	// Dbg(1, "rec: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \r\n", 
	// 	buff_Out[0],buff_Out[1],buff_Out[2],buff_Out[3],buff_Out[4],buff_Out[5],
	// 	buff_Out[6],buff_Out[7],buff_Out[8],buff_Out[9],buff_Out[10]);
				DealSurePOSCommand((BYTE*)buff_Out);
			}
		}while(SurePOS_TickleFeed());

		return;
	}

	while (Y_commBakReadChar(&cmd,0))
	{
		switch(cmd)
		{
		case '?'://先判断握手信号
			Y_commWrite((BYTE*)"!",1);
			break;
		case 0x7E: //命令报文
			Y_commBakReadChar(&cmd,COMMREAD_TIMEOUT); //确定报文属性
			nLength = ReadComm(cmd);
			if (V_commFrameBFTranslateMes != NULL)
			{ V_commFrameBFTranslateMes(cmd,m_chReceiveBytes,m_nReceive); }
            //printf("%s\r\n",m_chReceiveBytes);
			if(cmd == 0x00){ //QS2
				TranslateMes();
			}
			else if(cmd == 0x01){//新统一指令
			//printf("%s", m_chReceiveBytes);
			       NLSComm_ProcMain((unsigned char *) m_chReceiveBytes, m_nReceive);
			}
			break;
		case 0x1B: // ESC + 0/1/2/3/4/5   the Cmd of Function  is the Bank  of Korean
			IsEscCtrl();
			break;
		case 'N':
		case 'n': //设置码
			if (Y_commBakReadChar(&cmd, COMMREAD_TIMEOUT) == 0) return;
			if(cmd != 'L' && cmd != 'l') return;
			if (Y_commBakReadChar(&cmd, COMMREAD_TIMEOUT) == 0) return;
			if(cmd != 'S' && cmd != 's') return;
			memset(buff_Out, 0, 1024);
			vQuickSetCode(buff_Out);
			return ;
// 		case 'I':
// 			if (Y_commBakReadChar(&cmd, COMMREAD_TIMEOUT) == 0) return;
// 			if(cmd != 'D' && cmd != 'D') return;
// 			if (Y_commBakReadChar(&cmd, COMMREAD_TIMEOUT) == 0) return;
// 			if(cmd != 'L' && cmd != 'L') return;
// 			if (Y_commBakReadChar(&cmd, COMMREAD_TIMEOUT) == 0) return;
// 			if(cmd != 'E' && cmd != 'E') return;
// 			//Y_commWrite((BYTE*)"idle",4);
// 			U_appSetIdleMode();
// 			return;
		default:
			if (V_commProcManualFrame != NULL)
			{ V_commProcManualFrame(cmd); }
			break;
		}//End of switch;
	}//End of while;
}//End of function;

//获取版本信息
int V_commGetVersion(char *pBuff, int nMaxLen)
{
	char buff[512];
	int nLen;

	nLen = 0;

	//硬件信息
	nLen += Z_GetHardwareInfo(buff+nLen,sizeof(buff)-nLen);
	//APP
	if (V_commAddVerMsg != NULL)
	{ nLen += V_commAddVerMsg(buff+nLen,sizeof(buff)-nLen); }//End of if;
	//MLib:
	if (sizeof(buff)-nLen > sizeof(MODULE_SDK_VERSION)+12)
	{
#ifdef MODULE_SDK_ROWMIRROR
		nLen += sprintf(buff+nLen,"\r\nMLIB Ver:     %s %s",MODULE_SDK_VERSION,MODULE_SDK_ROWMIRROR);
#else
		nLen += sprintf(buff+nLen,"\r\nMLIB Ver:     %s",MODULE_SDK_VERSION);
#endif //MODULE_SDK_ROWMIRROR
	}//End of if;
	//uIMG信息
	nLen += X_decoGetUIMGVersion(buff+nLen,sizeof(buff)-nLen);

	if (nLen > nMaxLen)
	{ nLen = nMaxLen; }
	memcpy(pBuff,buff,nLen);

	return nLen;
}//End of function;

//设置扩展功能函数
void V_commSetUserFunc(UTGeneralParam *psParam)
{
	if (psParam != NULL && psParam->Id == STID_COMMEXTFNC && psParam->psParamStruct != NULL)
	{
		V_commFrameBFTranslateMes = ((TVCOMMExtFunc*)psParam->psParamStruct)->fnc_FrameBFTranslateMes;
		V_commProcManualFrame = ((TVCOMMExtFunc*)psParam->psParamStruct)->fnc_ProcManualFrame;
	}//End of if;

	if (psParam != NULL && psParam->Id == STID_ADDVERMSG)
	{
		V_commAddVerMsg = (FNC_commAddVerionMsg)(psParam->psParamStruct);
	}
}//End of function;



////////////////////////////////////////////////////////jpg压缩
// 
// #define DCTBLOCKSIZE 64
// #define DCTSIZE 8
// 
// typedef struct tagHUFFCODE
// {
// 	unsigned short  code;  // huffman 码字
// 	BYTE length;  // 编码长度
// //	unsigned short  val;   // 码字对应的值
// 	BYTE val;   // 码字对应的值
// } HUFFCODE;
// 
// typedef struct tagACSYM
// {
// 	BYTE zeroLen;  //0行程
// 	BYTE codeLen;  //幅度编码长度
// 	short  amplitude;//振幅
// } ACSYM;
// 
// typedef struct tagSYM2
// {
// 	short  amplitude;//振幅
// 	BYTE codeLen;  //振幅长度(二进制形式的振幅数据的位数)
// } SYM2;
// 
// float JPEG_YQT_DCT[DCTBLOCKSIZE] =
// {
// 	0.02083333, 0.02253000, 0.02391771, 0.01771729, 0.01250000, 0.00994343, 0.01154849, 0.01887766,
// 	0.01802400, 0.01299458, 0.01149581, 0.00958009, 0.00901200, 0.00498700, 0.00693833, 0.01484731,
// 	0.01594514, 0.01379497, 0.01220388, 0.00813614, 0.00597943, 0.00529417, 0.00631345, 0.01576182,
// 	0.01771729, 0.01094868, 0.00904015, 0.00753366, 0.00531519, 0.00386569, 0.00613824, 0.01541196,
// 	0.01785714, 0.01001333, 0.00637806, 0.00483199, 0.00462963, 0.00361579, 0.00563341, 0.01461496,
// 	0.01590948, 0.00819293, 0.00553481, 0.00520381, 0.00497171, 0.00482117, 0.00653264, 0.01558489,
// 	0.01154849, 0.00640462, 0.00570247, 0.00561211, 0.00563341, 0.00612435, 0.00889118, 0.02092882,
// 	0.01562289, 0.00882813, 0.00912526, 0.00987946, 0.01006808, 0.01441602, 0.02041836, 0.04105335
// };
// 
// const static BYTE JPEG_FZBT[DCTBLOCKSIZE] =
// {
// 	0, 1, 5, 6, 14, 15, 27, 28,
// 	2, 4, 7, 13, 16, 26, 29, 42,
// 	3, 8, 12, 17, 25, 30, 41, 43,
// 	9, 11, 18, 24, 31, 40, 44, 53,
// 	10, 19, 23, 32, 39, 45, 52, 54,
// 	20, 22, 33, 38, 46, 51, 55, 60,
// 	21, 34, 37, 47, 50, 56, 59, 61,
// 	35, 36, 48, 49, 57, 58, 62, 63
// };
// 
// const static BYTE JPEG_std_Y_QT[DCTBLOCKSIZE] =
// {
// 	16, 11, 10, 16, 24, 40, 51, 61,
// 	12, 12, 14, 19, 26, 58, 60, 55,
// 	14, 13, 16, 24, 40, 57, 69, 56,
// 	14, 17, 22, 29, 51, 87, 80, 62,
// 	18, 22, 37, 56, 68, 109, 103, 77,
// 	24, 35, 55, 64, 81, 104, 113, 92,
// 	49, 64, 78, 87, 103, 121, 120, 101,
// 	72, 92, 95, 98, 112, 100, 103, 99
// };
// 
// static unsigned short JPEG_MASK[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
// 
// static HUFFCODE JPEG_DC[12] = {{0, 2, 0}, {2, 3, 1}, {3, 3, 2}, {4, 3, 3}, {5, 3, 4}, {6, 3, 5}, {14, 4, 6}, {30, 5, 7}, {62, 6, 8}, {126, 7, 9}, {254, 8, 10}, {510, 9, 11}};
// 
// static HUFFCODE JPEG_AC[256] =
// {
// 	{10, 4, 1}, {0, 2, 2}, {1, 2, 3}, {4, 3, 0}, {11, 4, 4}, {26, 5, 17}, {120, 7, 5}, {248, 8, 18}, {1014, 10, 33}, {65410, 16, 49},
// 	{65411, 16, 65}, {0, 0, 6}, {0, 0, 19}, {0, 0, 81}, {0, 0, 97}, {0, 0, 7}, {0, 0, 34}, {12, 4, 113}, {27, 5, 20}, {121, 7, 50},
// 	{502, 9, 129}, {2038, 11, 145}, {65412, 16, 161}, {65413, 16, 8}, {65414, 16, 35}, {65415, 16, 66}, {65416, 16, 177}, {0, 0, 193},
// 	{0, 0, 21}, {0, 0, 82}, {0, 0, 209}, {0, 0, 240}, {0, 0, 36}, {28, 5, 51}, {249, 8, 98}, {1015, 10, 114}, {4084, 12, 130},
// 	{65417, 16, 9}, {65418, 16, 10}, {65419, 16, 22}, {65420, 16, 23}, {65421, 16, 24}, {65422, 16, 25}, {0, 0, 26}, {0, 0, 37},
// 	{0, 0, 38}, {0, 0, 39}, {0, 0, 40}, {0, 0, 41}, {58, 6, 42}, {503, 9, 52}, {4085, 12, 53}, {65423, 16, 54}, {65424, 16, 55},
// 	{65425, 16, 56}, {65426, 16, 57}, {65427, 16, 58}, {65428, 16, 67}, {65429, 16, 68}, {0, 0, 69}, {0, 0, 70}, {0, 0, 71}, {0, 0, 72},
// 	{0, 0, 73}, {0, 0, 74}, {59, 6, 83}, {1016, 10, 84}, {65430, 16, 85}, {65431, 16, 86}, {65432, 16, 87}, {65433, 16, 88},
// 	{65434, 16, 89}, {65435, 16, 90}, {65436, 16, 99}, {65437, 16, 100}, {0, 0, 101}, {0, 0, 102}, {0, 0, 103}, {0, 0, 104},
// 	{0, 0, 105}, {0, 0, 106}, {122, 7, 115}, {2039, 11, 116}, {65438, 16, 117}, {65439, 16, 118}, {65440, 16, 119}, {65441, 16, 120},
// 	{65442, 16, 121}, {65443, 16, 122}, {65444, 16, 131}, {65445, 16, 132}, {0, 0, 133}, {0, 0, 134}, {0, 0, 135}, {0, 0, 136},
// 	{0, 0, 137}, {0, 0, 138}, {123, 7, 146}, {4086, 12, 147}, {65446, 16, 148}, {65447, 16, 149}, {65448, 16, 150}, {65449, 16, 151},
// 	{65450, 16, 152}, {65451, 16, 153}, {65452, 16, 154}, {65453, 16, 162}, {0, 0, 163}, {0, 0, 164}, {0, 0, 165}, {0, 0, 166},
// 	{0, 0, 167}, {0, 0, 168}, {250, 8, 169}, {4087, 12, 170}, {65454, 16, 178}, {65455, 16, 179}, {65456, 16, 180}, {65457, 16, 181},
// 	{65458, 16, 182}, {65459, 16, 183}, {65460, 16, 184}, {65461, 16, 185}, {0, 0, 186}, {0, 0, 194}, {0, 0, 195}, {0, 0, 196},
// 	{0, 0, 197}, {0, 0, 198}, {504, 9, 199}, {32704, 15, 200}, {65462, 16, 201}, {65463, 16, 202}, {65464, 16, 210}, {65465, 16, 211},
// 	{65466, 16, 212}, {65467, 16, 213}, {65468, 16, 214}, {65469, 16, 215}, {0, 0, 216}, {0, 0, 217}, {0, 0, 218}, {0, 0, 225},
// 	{0, 0, 226}, {0, 0, 227}, {505, 9, 228}, {65470, 16, 229}, {65471, 16, 230}, {65472, 16, 231}, {65473, 16, 232}, {65474, 16, 233},
// 	{65475, 16, 234}, {65476, 16, 241}, {65477, 16, 242}, {65478, 16, 243}, {0, 0, 244}, {0, 0, 245}, {0, 0, 246}, {0, 0, 247},
// 	{0, 0, 248}, {0, 0, 249}, {506, 9, 250}, {65479, 16, 0}, {65480, 16, 0}, {65481, 16, 0}, {65482, 16, 0}, {65483, 16, 0},
// 	{65484, 16, 0}, {65485, 16, 0}, {65486, 16, 0}, {65487, 16, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
// 	{1017, 10, 0}, {65488, 16, 0}, {65489, 16, 0}, {65490, 16, 0}, {65491, 16, 0}, {65492, 16, 0}, {65493, 16, 0}, {65494, 16, 0},
// 	{65495, 16, 0}, {65496, 16, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {1018, 10, 0}, {65497, 16, 0},
// 	{65498, 16, 0}, {65499, 16, 0}, {65500, 16, 0}, {65501, 16, 0}, {65502, 16, 0}, {65503, 16, 0}, {65504, 16, 0}, {65505, 16, 0},
// 	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {2040, 11, 0}, {65506, 16, 0}, {65507, 16, 0}, {65508, 16, 0},
// 	{65509, 16, 0}, {65510, 16, 0}, {65511, 16, 0}, {65512, 16, 0}, {65513, 16, 0}, {65514, 16, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
// 	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {65515, 16, 0}, {65516, 16, 0}, {65517, 16, 0}, {65518, 16, 0}, {65519, 16, 0}, {65520, 16, 0},
// 	{65521, 16, 0}, {65522, 16, 0}, {65523, 16, 0}, {65524, 16, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {2041, 11, 0},
// 	{65525, 16, 0}, {65526, 16, 0}, {65527, 16, 0}, {65528, 16, 0}, {65529, 16, 0}, {65530, 16, 0}, {65531, 16, 0}, {65532, 16, 0},
// 	{65533, 16, 0}, {65534, 16, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
// };
// 
// static const double JEPG_aanScaleFactor[8] = {1.0, 1.387039845, 1.306562965, 1.175875602, 1.0, 0.785694958, 0.541196100, 0.275899379};
// 
// BYTE*	g_pJpegFileBuff;
// static BYTE g_JPEG_bytenew = 0;
// static signed char g_JPEG_bytepos = 7;
// 
// short 	g_MaxVal = 0;
// 
// void JPEG_SetYQuantTable (BYTE* pQT, BYTE Q)
// {
// 	short  tmpVal = 0;                  //临时变量
// 	short  i = 0;
// 
// 	if (Q < 1) Q = 1;               //限制质量系数
// 
// 	if (Q > 100) Q = 100;
// 
// 	//非线性映射 1->5000, 10->500, 25->200, 50->100, 75->50, 100->0
// 	if (Q < 50)
// 	{
// 		Q = 5000 / Q;
// 	}
// 	else
// 	{
// 		Q = 200 - Q * 2;
// 	}
// 
// 	for (i = 0; i < DCTBLOCKSIZE; ++i)
// 	{
// 		tmpVal = (JPEG_std_Y_QT[i] * Q + 50L) / 100L;
// 
// 		if (tmpVal < 1)                 //数值范围限定
// 		{
// 			tmpVal = 1L;
// 		}
// 
// 		if (tmpVal > 255)
// 		{
// 			tmpVal = 255L;
// 		}
// 
// 		pQT[JPEG_FZBT[i]] = (BYTE) tmpVal;
// 	}
// }
// 
// void InitQTForAANDCT (BYTE* pQT)
// {
// 	short  i = 0;           //临时变量
// 	short  j = 0;
// 	short  k = 0;
// 
// 	for (i = 0; i < 8; i++)  //初始化亮度信号量化表
// 	{
// 		for (j = 0; j < 8; j++)
// 		{
// 			JPEG_YQT_DCT[k] = (float) (1.0 / ( (double) pQT[JPEG_FZBT[k]] * JEPG_aanScaleFactor[i] * JEPG_aanScaleFactor[j] * 8.0));
// 			++k;
// 		}
// 	}
// }
// 
// void Write2Buff (const void* pVal, unsigned short  Length)
// {
// 	memcpy (g_pJpegFileBuff, pVal, Length);
// 	g_pJpegFileBuff += Length;
// }
// 
// void WriteHead2Buff (unsigned short  nWidth, unsigned short  nHeight)
// {
// 	BYTE fileHead[328] =
// 	{
// 		255, 216, 255, 224, 0, 16, 74, 70, 73, 70, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 255, 219, 0,
// 		67, 0, 6, 4, 5, 6, 5, 4, 6, 6, 5, 6, 7, 7, 6, 8, 10, 16, 10, 10, 9, 9, 10, 20, 14, 15,
// 		12, 16, 23, 20, 24, 24, 23, 20, 22, 22, 26, 29, 37, 31, 26, 27, 35, 28, 22, 22, 32, 44,
// 		32, 35, 38, 39, 41, 42, 41, 25, 31, 45, 48, 45, 40, 48, 37, 40, 41, 40, 255, 192, 0, 11,
// 		8, 3, 32, 5, 0, 1, 1, 17, 0, 255, 196, 0, 31, 0, 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
// 		0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 255, 196, 0, 181, 16, 0, 2, 1, 3, 3, 2,
// 		4, 3, 5, 5, 4, 4, 0, 0, 1, 125, 1, 2, 3, 0, 4, 17, 5, 18, 33, 49, 65, 6, 19, 81, 97, 7,
// 		34, 113, 20, 50, 129, 145, 161, 8, 35, 66, 177, 193, 21, 82, 209, 240, 36, 51, 98, 114,
// 		130, 9, 10, 22, 23, 24, 25, 26, 37, 38, 39, 40, 41, 42, 52, 53, 54, 55, 56, 57, 58, 67,
// 		68, 69, 70, 71, 72, 73, 74, 83, 84, 85, 86, 87, 88, 89, 90, 99, 100, 101, 102, 103, 104,
// 		105, 106, 115, 116, 117, 118, 119, 120, 121, 122, 131, 132, 133, 134, 135, 136, 137, 138,
// 		146, 147, 148, 149, 150, 151, 152, 153, 154, 162, 163, 164, 165, 166, 167, 168, 169, 170,
// 		178, 179, 180, 181, 182, 183, 184, 185, 186, 194, 195, 196, 197, 198, 199, 200, 201, 202,
// 		210, 211, 212, 213, 214, 215, 216, 217, 218, 225, 226, 227, 228, 229, 230, 231, 232, 233,
// 		234, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 255, 218, 0, 8, 1, 1, 0, 0, 63, 0,
// 	};
// //	memcpy (fileHead + 25, pYQT, 64);
// 	fileHead[94] = nHeight >> 8;
// 	fileHead[95] = nHeight & 0x00FF;
// 	fileHead[96] = nWidth >> 8;
// 	fileHead[97] = nWidth & 0x00FF;
// 	Write2Buff (fileHead, 328);
// }
// 
// void WriteTail2Buff()
// {
// 	unsigned short  EOITAG = 0xD9FF;
// 	Write2Buff (&EOITAG, 2);
// }
// 
// 
// inline BYTE ComputeVLI (short  val)
// {
// 	if (val == 0)
// 	{
// 		return 0;
// 	}/*End of If*/
// 
// 	if (val < 0)
// 	{
// 		val = -val;
// 	}
// 
// 	g_MaxVal = max (g_MaxVal, val);
// 
// 	if (val < 2)
// 	{
// 		return 1;
// 	}/*End of If*/
// 
// 	if (val < 4)
// 	{
// 		return 2;
// 	}/*End of If*/
// 
// 	if (val < 8)
// 	{
// 		return 3;
// 	}/*End of If*/
// 
// 	if (val < 16)
// 	{
// 		return 4;
// 	}/*End of If*/
// 
// 	if (val < 32)
// 	{
// 		return 5;
// 	}/*End of If*/
// 
// 	if (val < 64)
// 	{
// 		return 6;
// 	}/*End of If*/
// 
// 	if (val < 128)
// 	{
// 		return 7;
// 	}/*End of If*/
// 
// 	if (val < 256)
// 	{
// 		return 8;
// 	}/*End of If*/
// 
// 	if (val < 512)
// 	{
// 		return 9;
// 	}/*End of If*/
// 
// 	if (val < 1024)
// 	{
// 		return 10;
// 	}/*End of If*/
// 
// 	if (val < 2048)
// 	{
// 		return 11;
// 	}/*End of If*/
// }
// 
// void FDCT (float* lpBuff)
// {
// 	float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
// 	float tmp10, tmp11, tmp12, tmp13;
// 	float z1, z2, z3, z4, z5, z11, z13;
// 	float* dataptr;
// 	int ctr;
// 	/* 第一部分，对行进行计算 */
// 	dataptr = lpBuff;
// 
// 	for (ctr = DCTSIZE - 1; ctr >= 0; ctr--)
// 	{
// 		tmp0 = dataptr[0] + dataptr[7];
// 		tmp1 = dataptr[1] + dataptr[6];
// 		tmp2 = dataptr[2] + dataptr[5];
// 		tmp3 = dataptr[3] + dataptr[4];
// 		tmp4 = dataptr[3] - dataptr[4];
// 		tmp5 = dataptr[2] - dataptr[5];
// 		tmp6 = dataptr[1] - dataptr[6];
// 		tmp7 = dataptr[0] - dataptr[7];
// 		/* 对偶数项进行运算 */
// 		tmp10 = tmp0 + tmp3; /* phase 2 */
// 		tmp11 = tmp1 + tmp2;
// 		tmp12 = tmp1 - tmp2;
// 		tmp13 = tmp0 - tmp3;
// 		dataptr[0] = tmp10 + tmp11; /* phase 3 */
// 		dataptr[4] = tmp10 - tmp11;
// 		z1 = (tmp12 + tmp13) * (0.707106781); /* c4 */
// 		dataptr[2] = tmp13 + z1; /* phase 5 */
// 		dataptr[6] = tmp13 - z1;
// 		/* 对奇数项进行计算 */
// 		tmp10 = tmp4 + tmp5; /* phase 2 */
// 		tmp11 = tmp5 + tmp6;
// 		tmp12 = tmp6 + tmp7;
// 		z5 = (tmp10 - tmp12) * (0.382683433);  /* c6 */
// 		z2 = (0.541196100) * tmp10 + z5; /* c2-c6 */
// 		z4 = (1.306562965) * tmp12 + z5; /* c2+c6 */
// 		z3 = tmp11 * (0.707106781); /* c4 */
// 		z11 = tmp7 + z3;  /* phase 5 */
// 		z13 = tmp7 - z3;
// 		dataptr[5] = z13 + z2; /* phase 6 */
// 		dataptr[3] = z13 - z2;
// 		dataptr[1] = z11 + z4;
// 		dataptr[7] = z11 - z4;
// 		dataptr += DCTSIZE; /* 将指针指向下一行 */
// 	}
// 
// 	/* 第二部分，对列进行计算 */
// 	dataptr = lpBuff;
// 
// 	for (ctr = DCTSIZE - 1; ctr >= 0; ctr--)
// 	{
// 		tmp0 = dataptr[DCTSIZE*0] + dataptr[DCTSIZE*7];
// 		tmp1 = dataptr[DCTSIZE*1] + dataptr[DCTSIZE*6];
// 		tmp2 = dataptr[DCTSIZE*2] + dataptr[DCTSIZE*5];
// 		tmp3 = dataptr[DCTSIZE*3] + dataptr[DCTSIZE*4];
// 		tmp4 = dataptr[DCTSIZE*3] - dataptr[DCTSIZE*4];
// 		tmp5 = dataptr[DCTSIZE*2] - dataptr[DCTSIZE*5];
// 		tmp6 = dataptr[DCTSIZE*1] - dataptr[DCTSIZE*6];
// 		tmp7 = dataptr[DCTSIZE*0] - dataptr[DCTSIZE*7];
// 		/* 对偶数项进行运算 */
// 		tmp10 = tmp0 + tmp3; /* phase 2 */
// 		tmp13 = tmp0 - tmp3;
// 		tmp11 = tmp1 + tmp2;
// 		tmp12 = tmp1 - tmp2;
// 		dataptr[DCTSIZE*0] = tmp10 + tmp11; /* phase 3 */
// 		dataptr[DCTSIZE*4] = tmp10 - tmp11;
// 		z1 = (tmp12 + tmp13) * (0.707106781); /* c4 */
// 		dataptr[DCTSIZE*2] = tmp13 + z1; /* phase 5 */
// 		dataptr[DCTSIZE*6] = tmp13 - z1;
// 		/* 对奇数项进行计算 */
// 		tmp10 = tmp4 + tmp5; /* phase 2 */
// 		tmp11 = tmp5 + tmp6;
// 		tmp12 = tmp6 + tmp7;
// 		z5 = (tmp10 - tmp12) * (0.382683433); /* c6 */
// 		z2 = (0.541196100) * tmp10 + z5; /* c2-c6 */
// 		z4 = (1.306562965) * tmp12 + z5; /* c2+c6 */
// 		z3 = tmp11 * (0.707106781); /* c4 */
// 		z11 = tmp7 + z3;  /* phase 5 */
// 		z13 = tmp7 - z3;
// 		dataptr[DCTSIZE*5] = z13 + z2; /* phase 6 */
// 		dataptr[DCTSIZE*3] = z13 - z2;
// 		dataptr[DCTSIZE*1] = z11 + z4;
// 		dataptr[DCTSIZE*7] = z11 - z4;
// 		++dataptr;   /* 将指针指向下一列 */
// 	}
// }
// 
// void WriteByte (BYTE val)
// {
// 	Write2Buff (&val, sizeof (val));
// }
// 
// void WriteBitsStream (unsigned short value, BYTE codeLen)
// {
// 	signed char posval;//bit position in the bitstring we read, should be<=15 and >=0
// 	posval = codeLen - 1;
// 
// 	while (posval >= 0)
// 	{
// 		if (value & JPEG_MASK[posval])
// 		{
// 			g_JPEG_bytenew |= JPEG_MASK[g_JPEG_bytepos];
// 		}
// 
// 		posval--;
// 		g_JPEG_bytepos--;
// 
// 		if (g_JPEG_bytepos < 0)
// 		{
// 			if (g_JPEG_bytenew == 0xFF)
// 			{
// 				WriteByte (0xFF);
// 				WriteByte (0);
// 			}
// 			else
// 			{
// 				WriteByte (g_JPEG_bytenew);
// 			}
// 
// 			g_JPEG_bytepos = 7;
// 			g_JPEG_bytenew = 0;
// 		}
// 	}
// }
// 
// void WriteBits_HUFF (HUFFCODE huffCode)
// {
// 	WriteBitsStream (huffCode.code, huffCode.length);
// }
// 
// void WriteBits_SYM2 (SYM2 sym)
// {
// 	WriteBitsStream (sym.amplitude, sym.codeLen);
// }
// 
// SYM2 BuildSym2 (short  value)
// {
// 	SYM2 Symbol;
// 	Symbol.codeLen = ComputeVLI (value);             //获取编码长度
// 
// 	if (value >= 0)
// 	{
// 		Symbol.amplitude = value;
// 	}
// 	else
// 	{
// 		Symbol.amplitude = short  ( (2 << (Symbol.codeLen - 1)) - 1) + value; //计算反码
// 	}
// 
// 	return Symbol;
// }
// 
// void RLEComp (short * lpbuf, ACSYM* lpOutBuf, BYTE& resultLen)
// {
// 	BYTE zeroNum     = 0;       //0行程计数器
// 	int EOBPos      = 0;       //EOB出现位置
// 	const BYTE MAXZEROLEN = 15;          //最大0行程
// 	int i        = 0;      //临时变量
// 	int j        = 0;
// 	EOBPos = DCTBLOCKSIZE - 1;          //设置起始位置，从最后一个信号开始
// 
// 	for (i = EOBPos; i > 0; i--)         //从最后的AC信号数0的个数
// 	{
// 		if (lpbuf[i] == 0)           //判断数据是否为0
// 		{
// 			--EOBPos;            //向前一位
// 		}
// 		else              //遇到非0，跳出
// 		{
// 			break;
// 		}
// 	}
// 
// 	for (i = 1; i <= EOBPos; i++)         //从第二个信号，即AC信号开始编码
// 	{
// 		if (lpbuf[i] == 0 && zeroNum < MAXZEROLEN)     //如果信号为0并连续长度小于15
// 		{
// 			++zeroNum;
// 		}
// 		else
// 		{
// 			lpOutBuf[j].zeroLen = zeroNum;       //0行程（连续长度）
// 			lpOutBuf[j].codeLen = ComputeVLI (lpbuf[i]);   //幅度编码长度
// 			lpOutBuf[j].amplitude = lpbuf[i];      //振幅
// 			zeroNum = 0;           //0计数器复位
// 			++resultLen;           //符号数量++
// 			++j;             //符号计数
// 		}
// 	}
// }
// 
// void ProcessDU (float* lpBuf, short * DC)
// {
// 	BYTE i    = 0;              //临时变量
// 	int j    = 0;
// 	short  diffVal = 0;                //DC差异值
// 	BYTE acLen  = 0;               //熵编码后AC中间符号的数量
// 	short  sigBuf[DCTBLOCKSIZE];              //量化后信号缓冲
// 	ACSYM acSym[DCTBLOCKSIZE];              //AC中间符号缓冲
// 
// 	FDCT (lpBuf);                //离散余弦变换
// 
// 	for (i = 0; i < DCTBLOCKSIZE; i++)            //量化操作
// 	{
// 		sigBuf[JPEG_FZBT[i]] = lpBuf[i] * JPEG_YQT_DCT[i] + 0.5;
// 	}
// 
// 	//-----------------------------------------------------
// 	//对DC信号编码，写入文件
// 	//DPCM编码
// 	diffVal = sigBuf[0] - *DC;
// 	*DC = sigBuf[0];
// 
// 	//搜索Huffman表，写入相应的码字
// 	if (diffVal == 0)
// 	{
// 		WriteBits_HUFF (JPEG_DC[0]);
// 	}
// 	else
// 	{
// //		WriteBits_HUFF (JPEG_DC[JPEG_pVLITAB[diffVal]]);
// 		WriteBits_HUFF (JPEG_DC[ComputeVLI (diffVal) ]);
// 		WriteBits_SYM2 (BuildSym2 (diffVal));
// 	}
// 
// 	//-------------------------------------------------------
// 	//对AC信号编码并写入文件
// 	for (i = 63; (i > 0) && (sigBuf[i] == 0); i--) //判断ac信号是否全为0
// 	{
// 		//注意，空循环
// 	}
// 
// 	if (i == 0)                //如果全为0
// 	{
// 		WriteBits_HUFF (JPEG_AC[0x00]);          //写入块结束标记
// 	}
// 	else
// 	{
// 		RLEComp (sigBuf, &acSym[0], acLen);      //对AC运行长度编码
// 
// 		for (j = 0; j < acLen; j++)           //依次对AC中间符号Huffman编码
// 		{
// 			if (acSym[j].codeLen == 0)          //是否有连续16个0
// 			{
// 				WriteBits_HUFF (JPEG_AC[0xF0]);        //写入(15,0)
// 			}
// 			else
// 			{
// 				WriteBits_HUFF (JPEG_AC[acSym[j].zeroLen * 16 + acSym[j].codeLen]); //
// 				WriteBits_SYM2 (BuildSym2 (acSym[j].amplitude));
// 			}
// 		}
// 
// 		if (i != 63)              //如果最后位以0结束就写入EOB
// 		{
// 			WriteBits_HUFF (JPEG_AC[0x00]);
// 		}
// 	}
// }
// 
// void ProcessData (BYTE* SrcImg, unsigned short  nWidth, unsigned short  nHeight)
// {
// 	short  i, j, x, y;
// 	float buff[DCTBLOCKSIZE];
// 	float* pfbuff;
// 	BYTE*	pProc;
// 	short 	yDC = 0;
// 
// 	for (y = 0; y < nHeight; y += 8)
// 	{
// 		for (x = 0; x < nWidth; x += 8)
// 		{
// 			pProc = SrcImg + y * nWidth + x;
// 			pfbuff = buff;
// 
// 			for (j = 0; j < 8; ++j)
// 			{
// 				for (i = 0; i < 8; ++i)
// 				{
// 					*pfbuff = (float) (pProc[i] - 128);
// 					++pfbuff;
// 				}/*End of For*/
// 
// 				pProc += nWidth;
// 			}/*End of For*/
// 
// 			ProcessDU (buff, &yDC);
// 		}/*End of For*/
// 	}/*End of For*/
// }
// 
// int JpegCompress (BYTE* pSrcImg, BYTE* pJpegImg, unsigned short  nImgWidth, unsigned short  nImgHeight)
// {
// //以8*8个像素单元进行压缩，宽度和高度必须是8的倍数。
// 	g_JPEG_bytenew = 0;
// 	g_JPEG_bytepos = 7;
// 	g_pJpegFileBuff = pJpegImg;
// #if 0
// 	BYTE* pYQT;
// 	pYQT = g_pJpegFileBuff + 25;
// 	WriteHead2Buff();
// 	JPEG_SetYQuantTable (pYQT, 80);
// 	InitQTForAANDCT (pYQT);
// //////////////////////////////////////////////////
// 	int i;
// 	char msg[20];
// 	OutputDebugString ("\n");
// 
// 	for (i = 0; i < 328; ++i)
// 	{
// 		sprintf_s (msg, "%d,", UI_TagIMG[i]);
// 		OutputDebugString (msg);
// 	}/*End of For*/
// 
// 	OutputDebugString ("\n");
// 
// 	for (i = 0; i < 64; ++i)
// 	{
// 		sprintf_s (msg, "%3.8f,", JPEG_YQT_DCT[i]);
// 		OutputDebugString (msg);
// 	}/*End of For*/
// 
// 	OutputDebugString ("\n");
// #else
// 	WriteHead2Buff (nImgWidth, nImgHeight);
// #endif
// //////////////////////////////////////////////////
// 	ProcessData (pSrcImg, nImgWidth, nImgHeight);
// 	WriteTail2Buff();
// 	return (g_pJpegFileBuff - pJpegImg);
// ///////////////////////////////////////////////
// }
// 
// 


