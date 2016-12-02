#ifndef __UIMG_DEVELOP_INTERFACE_COMMON_H__
#define __UIMG_DEVELOP_INTERFACE_COMMON_H__

#include <math.h>
//#include <string.h>
//#include <stdio.h>
//#include "X_layer/X_deco/uIMGSrc/1D2006/Define1D.h"
//#include "X_layer/X_deco/uIMGSrc/OCR_BSrc/OCR_Config.h"
/************************************************************************/
/*                                                                      */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////

//#ifndef BYTE
#undef BYTE
	#define BYTE unsigned char
//#endif
#ifndef BOOL
	#define BOOL int
#endif
	
#define ____SQRT(x)  sqrt((double)(x))
#define ____POW(x,y)  pow((double)(x),(int)(y))

#ifndef  max
#define max(x,y) ((x)>(y)?(x):(y))
#endif  //#ifndef  max

#ifndef  min
#define min(x,y) ((x)<(y)?(x):(y))
#endif  //#ifndef  max

#ifndef TRUE
	#define TRUE	1
#endif

#ifndef FALSE
	#define FALSE	0
#endif

#ifndef NULL
	#define NULL	0
#endif

/************************************************************************/
/*                                                                      */
/************************************************************************/
#define IMG_WIDTH_MAX	1280
#define IMG_HEIGHT_MAX	970
#define IMG_SIZE_MAX	(IMG_WIDTH_MAX*IMG_HEIGHT_MAX + 2048)
                 	 

#define LIGHT_MODE1					0				//normal
#define LIGHT_MODE2					1				//always on
#define LIGHT_MODE3					2				//always off
#define LIGHT_MODE4					3				//
#define LIGHT_MODE5					4				//user control
                                	            	
#define FOCUS_MODE1					0				//normal
#define FOCUS_MODE2					1				//always on
#define FOCUS_MODE3					2				//always off
#define FOCUS_MODE4					3				//
#define FOCUS_MODE5					4				//user control
              	                                     	
                   
//���ֱ�׼ǰ��׺ʹ�ܴ��λ�ú궨��              	
#define POS_IsPrefix				0				//�Զ���ǰ׺ʹ�ܻ��ֹ����
#define POS_IsSuffix				1				//�Զ����׺ʹ�ܻ��ֹ����
#define POS_IsCodeID				2				//CodeIDǰ׺ʹ�ܻ��ֹ����
#define POS_IsAIM					3				//AIMǰ׺ʹ�ܻ��ֹ����
#define POS_IsTerminal				4				//��������׺ʹ�ܻ��ֹ����
#define POS_IsCut					5				//��ȡʹ�ܻ��ֹ����
            
#define  ESC0						0           	
#define  ESC1						1           	
#define  ESC2						2           	
#define  ESC3						3				//�������ƶ���: �趨��TRUE����������ܶ��룬ֱ����һ�������FALSE
#define  ESC7                       7               //����USB��������ͼ�����

#define  ESC_TEST_READ_MODE			0x1000			//���ڲ��Զ���ģʽ

#define  PAPER						0x01        	
#define  CELL						0x02        	
#define  ALL						0x03  


#define  MACRO_DIRECT				0x01			//��ȡ��ֱ�ӷ��ͣ��������κδ���
#define  MACRO_SEQ_BUFF				0x02			//����˳���ͣ����Զ��64k�Ļ��棬�������棬ϵͳ�������Զ���գ���Ҫ��ȡ�������ȡ�����buff
#define  MACRO_LINK_16				0x03			//��ȡ��ϣ�����˳�����Ӻ��ͣ��������Ӷ��16������С��64k��Ϣ�ķ���
          
#define  U_PRO_IS_Serial        0
#define  U_PRO_IS_DataPipe		1 
#define  U_PRO_IS_HID_KBW       2
#define	 U_PRO_IS_BLUE_TOOTH	4
#define  U_PRO_IS_Storage       5
#define	 U_PRO_IS_USBRS232		6
#define	 U_PRO_IS_PS2           7
#define	 U_PRO_IS_HID_POS       8
#define	 U_PRO_IS_SurePOS_TT    9
#define	 U_PRO_IS_SurePOS_HH    10
#define	 U_PRO_IS_CDC		    11


#define NOPARITY            0
#define ODDPARITY           1
#define EVENPARITY          2
#define MARKPARITY          3
#define SPACEPARITY         4

#ifndef ONESTOPBIT
#define ONESTOPBIT          1
#define ONE5STOPBITS        1
#define TWOSTOPBITS         2
#endif

#define CMOS3000_TYPE				0x00000000 	
#define CMOS3020_TYPE				0x00000001 
#define CMOSOV9650_TYPE             0x00000002 
#define CMOS_MT9V022_TYPE           0x00000003   
                             	            	
//Ӳ����Ʒ����                  	            	
#define  PRODUCTION_DEF				0				//����������
#define  PRODUCTION_FM102			1				// FM102 ��Ʒ
#define  PRODUCTION_FM102L			2				// FM102L ��Ʒ����
#define  PRODUCTION_MIRROR			3				// ͼ�����Ʒ����
#define  PRODUCTION_DOF				4				// ��������
#define  PRODUCTION_SCANSTATUE		5				// IBM SurePOS ����ʱ����Ƿ�ʹ��ʶ������

#define  PRODUCTION_DOF_NORMAL		0				// ��׼��������
#define  PRODUCTION_DOF_NEAR		1				// ����������
#define  PRODUCTION_DOF_MIDDLE		2				// �о�������
#define  PRODUCTION_DOF_FAR			3				// Զ��������

                                       
//ͨ�Žṹ                      	
#ifndef STX                     	
#define STX							0x02			//1Byte��������ʼ��־
#endif                          	            	
#define ATTR						0x00			//1Byte����������
	                            	            	
//����ṹ                      	            	
#define AL_SET						0x31			//1Byte��������������
#define AL_SET_ECHO					0x32			//1Byte���������û�Ӧ
#define AL_SET_Q					0x33			//1Byte��������ѯ����
#define AL_SET_Q_ECHO				0x34			//1Byte��������ѯ���
#define AL_IMG_REQ					0x35			//1Byte������ͼ����������
#define AL_GET_MSG					0x36			//��������Ϣ�ϴ�����
#define AL_IMG_REQ_ECHO	            0x37	        //1Byte����Ӧͼ����������
#define AL_SET_STATE                0x38            //1Byte���豸״̬����

#define AL_SET_DATAFORMAT			0x3a			//1Byte������Data format 
                                	            	
#define ETX							0x03        	
#define ACK							0x06        	
#define NAK							0x15        	
#define ESC							0x1B        	
#define DLE							0x10      
  	                        	            	
//���ݴ������                  	            	
#define  FACTORY_PACK				1				//��ͨ�������
#define  ECS_BANK_PACK				2				//ECS �������
#define  BLUE2D_PACK				3				//BLUE 2D �������
#define  ECS_PACK_ADDLEN			4				//ECS  STX+DATALENS(4Digits)+DATA+ETX+BCC
                               
#define CAN_DEC_1D_NUM      		32  				//Ŀǰ�ɽ��һά����������
#define CAN_DEC_2D_NUM       		32  				//Ŀǰ�ɽ�Ķ�ά����������
#define CAN_DEC_OCR_NUM      		1  					//Ŀǰ�ɽ��OCR������
#define MAX_1D_NUM          		32  				//һά������������
#define MAX_2D_NUM          		32 	 				//��ά������������
#define NONCODELIST_NUM     		10  				//�޷�����ʶ�𳤶ȷ�Χ��������
#define QUERY_1DPARAM_NUM    		9  					//�ɹ���ѯ������һά����������
                 	            	
//�Ǳ�׼ǰ��׺ʹ�����ú궨��    	            	
#define POS_IsBlue2D_LenSend    	0				//Blue2D��Ϣ���ȷ�������
#define POS_IsBlue2D_TypeSend   	1				//Blue2D�������ͷ�������
#define POS_IsBlue2D_Sensitive  	2				//Blue2D��������ʹ������

#define CharacterEncoding_PDF417		0
#define CharacterEncoding_QR			1
#define CharacterEncoding_Aztec			2
#define CharacterEncoding_DataMatrix	3
#define CharacterEncoding_Code128		4
#define CharacterEncoding_Code39		5
#define CharacterEncoding_Code93		6                               	
/************************************************************************/
/*                                                                      */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
// ���뷵��ֵ

#define   UIMG_DECODER_FAIL        -1  // ����ʧ��

#define   UIMG_DECODER_SUC			0	// ��ͨ���뷵��
#define   UIMG_DECODER_MACRO		1	// ����쿴�귵��ֵ
#define   UIMG_DECODER_SETCODE		2	// ����쿴�����뷵��ֵ

#define   UIMG_DECODER_SPEC_LIMIT	10  // �ܵ���������趨����
#define   UIMG_DECODER_LENS_LIMIT	11  // �ܵ����Ȳ�������
#define   UIMG_DECODER_EN_LIMIT		12  // �ܵ�ʹ�ܲ�������

// ���뷵��ֵ
//////////////////////////////////////////////////////////////////////////
//�����봦����ֵ
#define U_uIMG_STATUS_SETCODE_START     0x000100   //���ÿ�ʼ
#define U_uIMG_STATUS_SETCODE_END       0x000101   //���ý���
#define U_uIMG_STATUS_SETCODE_ERROR     0x000102   //���ô���
#define U_uIMG_STATUS_SETCODE_SUC       0x000103   //������ȷ
#define U_uIMG_STATUS_SETCODE_UNKNOW    0x000104   //����״̬δ֪



#define MACRO_START					"|+1.8|+5.8|-6.8|+7.8|1.8|"//��ʼ��ȡ��
#define MACRO_OK					"|+1.8|-2.8|+7.8|"//���ȡ������
#define MACRO_END					"|+1.8|-6.8|+2.8|-6.8|+6.8|+6.8|+6.8|"//���ȡ����
#define MACRO_ERROR					"|+7.8|+4.8|+1.8|6.8|1.8|-7.8|-1.8|+1.8|+5.8|" //��ȡ����������ظ����߻������
                                	
#define SETCODE_START				"|-4.3|1.8|2.8|5.8|+1.8|+5.8|"//����������״̬
#define SETCODE_OK					"|+4.8|+6.8|-6.8|+5.8|"//������������е���
#define SETCODE_END					"|+2.4|+6.8|+6.8|6.3|-6.5|+6.2|+6.8|+5.8|"//�����봦����ϻ���һ�����óɹ�����
#define SETCODE_ERROR				"|+7.8|-6.8|+2.8|+7.8|-6.8|+2.8|1.4|+2.6|+7.8|+3.8|"//���������
                 
/************************************************************************/
/*                                                                      */
/************************************************************************/

#define Command_Num_MAX		516
#define Command_Num			512
#define  HEADLEN 12

// data formatting 
typedef struct tagFormat
{
	int FormatID;
	int FormatType;
// 	char TerID[3+1];
// 	char CodeID[2+1];
// 	char ComLen[4+1];
	int		TerID;
	int		SymbolID;
	int		MsgLen;
	char Cmd[Command_Num_MAX]; 
}Format;
typedef struct tagDataFormat
{
	int crc;
	BOOL DFEnable;
	int usedID;//  ʹ�õ�Format id��Ĭ��ֵ��0��
	int outFlag; //�����־��Formatִ�г���ʱ��ԭ��������Ǳ���Ҫ��Ҫ���ǰ��׺�ȡ���Ĭ�ϣ�����Ҫ��ǰ��׺��
	int singleFormatID;// 
	int iSsingle;//�Ƿ��е���ִ�е����
	int ErrorTone;// ����������
	Format format[4];
}DataFormat;



// typedef struct tagDFormat
// {
// 	Format format[4];
// }DFormat;

typedef int (*DF_ProcMain)(unsigned char *pCmd, int nCmdLen, unsigned char *pOriMsg, int nOriLen, unsigned char *pOutput);


///Data Format ����
typedef	struct	__DF_Param__
{
	int				nCodeBigType;				///��������, 1D, 2D
	int				nCodeType;					///��������, ����:Code128, PDF417
	
	int				nDecMsgLen;					///������Ϣ����
	unsigned char	*pDecMsg;					///������Ϣ
	
	unsigned char	*pDFCmd;					///Data Format����,���ͬʱ�а���,��ʱָ��;����ָ����ǰ,��ʱָ���ں�
	int				nDFMsgLen;					///Data Format�����
	
	///�ر�ע��:����������Ϣ��1���ֽ�ת����2��16�����ַ���ʾ
	///(���㴦����,��ʱ)
	unsigned char   *pDFOutputMsg;				///ע���ַ����Ѿ�������ת��
	int				nDFOutputLen;				///����DF����������ʱ�ȴ�������Ϣ����
	int				nMaxOutLen;					///����ռ��С
	
	int				nDFSelType;					///ѡ������,�뿴 Data Formatter�����궨��
	
	int				nRequired;					/// 1: required, 0: no required
	
	DF_ProcMain     func_DataFormat;			///����Data Format����
	
	int				nConvertToHexChar;			///ÿ���ֽ�ת��������16�����ַ��ı�ʶ, 0:û��ת��, 1:�Ѿ�ת��
}DF_Param;
///////////////////////////////////


typedef struct tagUTIMGPARAM
{
	BYTE* pSrc;
	BYTE* pTag;
	int   nWidth;
	int   nHeight;
}UTImgParam;

//���������������
typedef struct tagCODEPARAM{
	unsigned int  nPrefix;		 //	�Զ���ǰ׺��Ϣ����
	unsigned char byPrefix[11];  // �Զ���ǰ׺��Ϣ
	unsigned int  nSuffix;		 //	�Զ����׺��Ϣ����
	unsigned char bySuffix[11];  // �Զ����׺��Ϣ
	unsigned char by1DCodeID[32]; // �Զ���1D��CodeID��Ϣ(��һλ)
	unsigned char by2DCodeID[32]; // �Զ���2D ��CodeID��Ϣ
	unsigned int  nTerminal;     //	�Զ����������Ϣ����
	unsigned char byTerminal[3]; // �Զ����������Ϣ
	unsigned char IsPreSufOn[8]; // ����ǰ��׺������������
	unsigned char PrefixOrder;  //CodeID(C)��AIM(A)���Զ���(S)ǰ׺��˳��
	                            //0:C+A+S   1:C+S+A    2:A+C+S
	                            //3:A+S+C   4:S+C+A    6:S+A+C
	unsigned int  IsOtherPreSufOn; // ���˱�׼ǰ��׺��������û�ǰ��׺���ã�
	                               //��0λ:Blue2D ����Ϣ���ȷ�������
	                               //��1λ:Blue2D ���������ͷ�������
	unsigned char byCut[20 * 3]; //��ȡ����
	unsigned int  nMedium;		 // �������
	BOOL		  Protect;		 //	ͼ���Ƿ񾭹��ǲ�
	unsigned int  nSensitive;    // �ƹ������ȼ���  add by Tiercel 2004.07.06
	BOOL		  bIR;           // ��ǰ�Ƿ����˿ڷ��������������� add by Tiercel 2004.07.06
								 // IR ����״̬��

	unsigned int  nOnceDelay;	 //	һ�ζ����ӳ�ʱ��
	unsigned int  nInfoDelay;    // ESC +2,��ͬ��Ϣ�ӳ�ʱ�� 
	BOOL		  IsCompDelay;   // ��ȫ�ӳ�(��ͬ��Ϣ��ʱ��TRUE--��ȫ�ȴ���ʱ��FALSE-�м����һ���޷�ʶ����ʱȡ��
	unsigned int  nSuccDelay;    // ����ɹ��ӳ�
	int			  nReadMode;	//�������ģʽ��ESC0,ESC1,ESC2

	unsigned int nScanTimes;	//�޶����������0��ʾ���޶�

	BOOL		  SetCode;       // �����뷢�Ͳ���	

	unsigned int  nPack;         // ������� add by Tiercel 2004.07.06
	unsigned int  nUserDef;      // �������ͣ��û����������
	BOOL		  IsGotoConsole; // ��Դ����ʱ�Ƿ�������̨����
	BOOL		  IsPowerOnDisp; // ��Դ����ʱ�Ƿ���ʾϵͳ��Ϣ����
	BOOL		  bErrMsgEn;     //ʹ��
	unsigned int  nErrMsg;       //û������󷵻ص���Ϣ����
	unsigned char byErrMsg[11];  //��һ��ʱ����û�������Ƿ��ص���Ϣ

	unsigned int  decodeArea;	//��������ģʽ, ��4λֵΪ1��ʾ��������������룬0��ʾ�رգ�bit[12:4]��ʾ��������ռ��Ⱥ͸߶ȵİٷֱ�����ȡֵ��0 - 100

	BOOL		  IsSettingBarcodeOff;    //Reserved 
}CodeParam;

typedef struct tagCODEPARAM2{
	unsigned int	ID;		 //	�汾id=0xCCCCCXXX
	unsigned int	nsize;				//�ṹ���ֽ���
	unsigned char	by1DCodeID_2[32]; // �Զ���1D��CodeID��Ϣ(�ڶ�λ)
	unsigned char	by2DCodeID_2[32]; // �Զ���2D ��CodeID��Ϣ	
	unsigned int	cmosAdjustMode;	//cmos�������ģʽ, 0 - ��ͨģʽ��ֽ���룩��1 - �������ƣ�2 - mobileģʽ�ع��ʱ���е�֡�����������е�֡��������
									//	3 - ֽ���루motion�ã������Ѷ���Ļ��ʾ�룩��4 - mobileģʽ ÿ֡�ع������ƶ�����
	//	//0xCCCCC001  add 
	BOOL			bSurePOSPowerUpScanEnable;	//IBM SurePOS: when power up Imager System Status is enable scan or disable scan
	unsigned char  byPostalCodeID[32];		//�Զ���������ǰ׺
	unsigned char  byPostalCodeID_2[32];	//�Զ���������ǰ׺(�ڶ�λ)

	unsigned int	nSucBeepDuration;	//����ɹ�������ʾ����ʱ��
	unsigned int	nSucBeepFreq;		//����ɹ�������ʾƵ��
	unsigned int	nSucLedDuration;	//����ɹ�ָʾ�Ƴ���ʱ��
	BOOL			bPowerLedOn;	//��Դָʾ�ƣ� 0 - �رգ� 1 - ����
	unsigned char	nCharEncoding[8];	//�ַ�����ѡ��0-Ĭ��,1-UTF-8
	unsigned int	Reserved[29];
	//	//0xCCCCC001  add
	//int	crc32;
}CodeParam2;


typedef struct tagHARDPARAM{
//Ӳ������
	unsigned int   nLight; //����ģʽ
	unsigned int   nFocus; //�Խ�ģʽ����
	unsigned int    nProduction;//��Ʒ����
    unsigned int    bDecVoice; //Bit0:��������ʹ��,Bit1:��������ʹ��
	unsigned int    bOscillation; //����ʹ��
	unsigned int    nOscillationDelay; //�����ӳ�
	int				InterType;  // Serial | KBW / HID
	int             nBeepType;  //���4λ������ʾ�����������ͣ��ε�4λ��ʾ����������������
	unsigned char   nGoodreadLEDDuration;	//����ɹ�ָʾ�Ƴ���ʱ��
	unsigned char	nExLight;	//�ⲿ�����ƿ���
	unsigned char	nDofType;	//��������
	unsigned char	nGoodreadLED;		//����ɹ�ָʾ��ʹ��	//bit[0] 0 - ʹ�ܣ�1 - ��ֹ��bit[7] 0 - ���У� 1 - ����
}HardParam;

typedef struct tagKEYSEG{  //�������ֹؼ��ֺ�ֵ ���ɵĽṹ
	unsigned char pKeyWord[16];
	unsigned char pValue[512];
}TKEYSEG;

typedef struct tagParamInfo
{
	unsigned int paramLen;//����������
	unsigned int code2StartOffset;
	int	paramVersion;		//�����ṹ�汾
	unsigned int paramCRC;
}TParamInfo;

typedef struct tagCOMPORTPARAM
{
	long  lRate;		//������
	long  lDataBits;	//����
	long  lStopBits;     
	long  lParity;
	int   nPort; 
	int	  nflowCtrl;	//����
	int  Reserved;
}TComParam;

typedef struct  tagREADMODE{
	int nReadDelay; //���ֶ���״̬ʱ��
	int nCodeDelay; //�����ϴζ���ļ��ʱ��
	int nHaltDelay; //û�ж����κ���Ϣ��ʱ��
	BOOL bContinue;  //�������ƶ���: �趨��TRUE����������ܶ��룬ֱ����һ�������FALSE
	BOOL bIRSign;   //��ʱ���ô˽ṹ���в�����ȡ-���������(FALSE)/����(TRUE)
	unsigned int    CMOSID; // CMOS1020֮ǰ�İ汾û��ID��CM3020��product ID 0x0A = 0x76, 0x0B = 0x48
	int n1Want;		// ͼ��ɼ��ɹ�֮��ĵ�Ԫֵ
	int  bDebug;
	int  bDetectHalt; //̽���ж�
}TREADMODE;

typedef struct tagKeyBoardPARAM
{
	int  nLanguage;
	BOOL isCapsLock;
//	BOOL isNumLock;
	BOOL isUnknownCharactersBeep; //����δ֪�ַ��Ƿ�������ʾ��ȡֵ:0,1 Ĭ��:0
	BOOL isShiftLock;
//	BOOL isAutoCaps;
	int isConvertCase;	//ǿ��ת��������ĸ����Ϊ��д����Сд��ȡֵ:0,1,2 Ĭ��:0
	BOOL isNumericMode;
	int isKeysrockeDelay;	//����ÿ���ַ�֮�����ʱ���á�ȡֵ:0,1,2 Ĭ��:0
//	int  nCharDelay;    // �ַ�֮���ӳ�:0���ӳ٣�1�ӳ�20ms��2�ӳ�40ms
//	int  nCodeDelay;    // ����֮���ӳ�
	int isEmulateKeypad; //����ALT+ Keypad����ASCIIֵ �������ַ���ȡֵ:0,1 Ĭ��:0
	BOOL isCtrlASCIIMode; //
	//char isPS2CapsLockDef;
	char nCodePage;	//USB KBW alt+keypadģʽ��ѡ���codepage���
	char isKeyPad;	//С���̴򿪱�־��0���رգ�1���򿪡�Ĭ�ϣ�0���ر�
	char isFasttransferMode;
	char nPollRate;	//��ѯ����
}UTKeyBoard;

typedef struct tagBLUETOOTHPARAM
{
	unsigned char Name[20];
	BOOL isPSWCheck;
	unsigned char PSW[20];
}UTBlueTooth;

typedef struct tagDeviceINFO
{
	unsigned char pSN[21];
	unsigned char pESN[21];
	unsigned char pDate[21];
	unsigned char pManufID[21]; //ManufactureID
	unsigned char pLIC[21];
	unsigned char pUID[20];
	unsigned char IsDownload;
	unsigned char pManufName[21]; //ManufactureName 
	unsigned char pDofType[21]; //Dof type 
}UTDeviceINFO;

typedef struct tagUTDEC1DSETUP
{
	unsigned char	I2Of5Len[5]; // ����2Of5 �������ú�ʹ��,[0]:�Ƿ����Ŀǰ���ĸ���������:[1][2][3][4]
	unsigned int	unDecTypes; //�ɽ���Ĳ�������
	unsigned int	unDecParam; //����������������
	unsigned int	ExMsg[32];  //���16��λ��Ϊ�����С��Ϣ����ֵ����
}UTDec1DSetup;

// typedef struct tagUTDEC2DSETUP
// {
// 	unsigned int	nMacro;		 //	�����
// 	int				nQRParam;    // QR �Ǳ����趨����
// 	int             nReserve[10]; //
// 
// 	unsigned int	unDecTypes; // �ɽ���Ĳ�������
// 	unsigned int	ExMsg[10];  // ��󳤶ȣ���16bit�� ��С���ȣ���16bit
// 	unsigned int    ExParam[10]; // ϸ������ 	
// }UTDec2DSetup;        //ǰ��10��
// 
typedef struct tagUTDEC2DSETUP
{
	unsigned int	nMacro;		 //	�����
	int				nQRParam;    // QR �Ǳ����趨����
	int             nReserve[10]; //
	
	unsigned int	unDecTypes; // �ɽ���Ĳ�������
	// 	unsigned int	ExMsg[10];  // ��󳤶ȣ���16bit�� ��С���ȣ���16bit
	// 	unsigned int    ExParam[10]; // ϸ������ 	
	unsigned int	ExMsg[32];  // ��󳤶ȣ���16bit�� ��С���ȣ���16bit	//new
	unsigned int    ExParam[32]; // ϸ������ 								//new				
	
}UTDec2DSetup;        //ǰ��10��


// typedef struct tagUTDECOCRSETUP
// {
// 	unsigned int	unDecTypes; // �ɽ���Ĳ�������
//  	unsigned int	ExMsg[10];  //
// }UTDecOCRSetup;
// 
typedef struct tagUTDECOCRSETUP
{
	unsigned int	unDecTypes; // �ɽ���Ĳ�������
	// 	unsigned int	ExMsg[10];  //
	unsigned int	ExMsg[32];  //									//new
}UTDecOCRSetup;

typedef struct	tagUTDECPOSTALSETUP
{
	unsigned int	unDecTypes;		/*�ɽ���Ĳ�������*/
	unsigned int	unDecParam;		/*����������������*/
	unsigned int    ExMsg[32];		/*���16��λ��Ϊ�����С��Ϣ����ֵ����*/
}UTDecPostalSetup;

/* ���ý�������� */
typedef struct  tagUIMGAreaParamtag
{
	int top;
	int left;
	int right;
	int bottom;
	int	nType;				// 0��ʹ��Ĭ������, 1:ʹ�����õ����� 

}UIMGAreaParam;

typedef struct tagDECOUTMSG
{
	int				nBarCode;	// UT_TYPE_1D | UT_TYPE_2D | UT_TYPE_OCR
	int				nCodeType;  // 1D �쿴 define1d.h �� DC_XXX �Ķ���
	                            // 2D / OCR �쿴���ļ�  DC_XXXX �Ķ���
	unsigned char*	pMes;		// ��������Ϣ
	int				nMes;		// ��Ϣ����
	int             nReResult;  // ����ֵ
	int				nMacroErr;	// ��������
	int				nMacroStatus;	// ��״̬����
}TOUTPUTMSG;

typedef struct  UApp_Decode_Notify
{
	void (*Start_Decode)(void);			//��ʼ�������
	void (*Decode_Succeed)(void);		//����ɹ�
	void (*Failed_or_Timeout)(void);	//����ʧ�ܣ�ֹͣ���ǳ�ʱ
}UDECODE_NOTIFY;

//////////////////////////////////////////////////////////////////////////
//	�����ṹ�嶨��
#define STID_IMGPARAM		0xCC0001	//ͼ������ṹ��ID
#define STID_OCRSETUP		0xCC0002	//OCR �������ýṹ��ID
#define STID_1DSETUP		0xCC0003	//1D ��������ýṹ��ID
#define STID_2DSETUP		0xCC0004	//2D ��������ýṹ��ID
#define STID_COMMEXTFNC		0xCC0005	//�Զ�����ͨѶ��Ϣ�е�ָ�����չ���ܺ�������ṹ��ID
#define STID_SETCEXTFNC		0xCC0006	//�����봦����Ƕ����չ�Ĵ�����
#define STID_NEWCODE		0xCC0007	//����һ���µĽ��봦��
#define STID_CMOSEXTFNC		0xCC0008	//�����CMOS����չ����
#define STID_PARAMEXTFUNC	0xCC0009	//��������������չ����
#define STID_NEWCODEEXT		0xCC000A	//����һ���µĽ��봦�����׽ӿ�
#define STID_ADDVERMSG		0xCC000B	//����汾��Ϣ��ѯӦ������
#define STID_UAPPEXTFNC		0xCC000C	//U_app����չ����
#define STID_DECODE_NOTIFY	0xCC000D	//Ӧ�ó�����ڿ�ʼ��������������ʱ�Ļص��ӿ�

#define STID_QUERYUSRAPI	0xCC8000	//��ѯ���ο���ʹ�õ�API
#define STID_QUERYAPP		0xCC8002
#define STID_QUERYCOMM		0xCC8003
#define STID_QUERYSETC		0xCC8004
#define STID_QUERYMACRO		0xCC8006
#define STID_QUERYPARAM		0xCC8007
#define STID_QUERYCMOS		0xCC8008
#define STID_QUERYDECOD		0xCC8009
#define STID_QUERYBASE		0xCC800A
#define STID_QUERYIMGPROC	0xCC800B	//��ѯͼ����Ĺ��ܺ���

#define STID_REPLACEIMG		0xCCE000	//���ͼ���ַ
#define STID_REPLACEKEY		0xCCE001	//�����������


typedef struct tagGENERALPARAM
{
	unsigned int Id;			//���ݵĲ����ṹ���ʶ��ID
	void *	psParamStruct;		//���ݵĲ����ṹ��ָ��
}UTGeneralParam;

typedef struct tagALLPARAM {
	//char *pstrVersion;	//�汾��Ϣ
	TParamInfo *pParamInfo;
	TComParam	*pCom;
	UTKeyBoard	*pKBW;
	HardParam	*pHard;
	CodeParam	*pCode;
	UTDec1DSetup	*p1D;
	UTDec2DSetup	*p2D;
	UTDecOCRSetup	*pOCR;
	UTDecPostalSetup *pPostal;
	UTDeviceINFO *pDevInfo;
	TREADMODE	*pMode;
	TKEYSEG		*pKeyS;
	CodeParam2  *pCode2;
	DataFormat  *pDFormat;
      UIMGAreaParam *pAreaParam;
} UALLPARAM;

//Userʹ�õ�API
typedef struct zspTUSERAPI
{
	void	(*U_appSetMsg)(BYTE* pBuff, int nLen);
	void	(*U_appSetCategory)(int nCategory);
	void	(*U_appSetCodeType)(int nType);
	void	(*U_appSetResult)(int nResult);
	BYTE*	(*U_appGetMsgBuff)(void);
	int		(*U_appGetMsgLen)(void);
	int		(*U_appGetMsgBuffSize)(void);
	int		(*U_appGetCategory)(void);
	int		(*U_appGetCodeType)(void);
	int		(*U_appGetResult)(void);
	BOOL	(*U_appCaptureImg)(BOOL bWithLight);
	BOOL	(*U_appProcDecode)(void);
	BOOL	(*U_appMsgEditting)(void);
	BOOL	(*U_appMsgPacking)(void);
	BOOL	(*U_appDealCmd)(BYTE *pCmdString, int nLen);
	void	(*U_appSetUserFunc)(UTGeneralParam *psParam);
	BOOL	(*U_appKeyPressed)(void);
	void	(*U_appSleepMode)(void);
	void	(*U_appWillSleepTillWakeup)(int nIdleMiliSec);
	void	(*U_appGoingToSleep)(void);
	void	(*U_appBackFromSleep)(void);
	void	(*U_appBKGrndProc)(void); 
	void	(*U_appQueryAPI)(UTGeneralParam *psParam);
	BOOL	(*U_appDecodeWithMacro)(void);
	int		(*U_appGetVersionInfo)(char *pBuff, int nBuffSize);
	
	int		(*Y_commGetCurType)(void);
	BOOL	(*Y_commEnable)(void);
	BOOL	(*Y_commIsEnable)(void);
	BOOL	(*Y_commDisable)(void);
	int 	(*Y_commRead)(unsigned char * buff, unsigned int size, int msTimeOut);
	int 	(*Y_commWrite)(unsigned char *buff, unsigned int size);
	BOOL	(*Y_commClean)(void);
	int 	(*Y_commSendChar)(char cChar);
	int 	(*Y_commReadChar)(char *pcChar,int msTimeOut);
	int 	(*Y_commBuffCount)(void);
	
	BOOL 	(*Y_paramLoadAppBlock)(int nBlockOrder, BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramSaveAppBlock)(int nBlockOrder, BYTE *pBuff, int nSize);

	
	void 	(*Y_cmosSetWantLux)(int nWantLux);
	void 	(*Y_cmosMirror)(int nMode);

	void	(*Z_Beep)( int freq, int mdelay, int volume );
	void	(*Z_PlayMusic)(char * pcMusic);
	void	(*Z_usDelay)(int us);
	void	(*Z_msDelay)(int ms);
	int 	(*Z_Get10msCounter)(void);	
	void	(*Z_Oscillate)(int ms);
	void	(*Z_DogFeed)(void);
	int 	(*Z_DogGetBreak)(void); 
	int 	(*Z_DogReset)(int ms); 
	void	(*Z_LedOn)(int clr);
	void	(*Z_LedOff)(int clr);
	void	(*Z_LightOn)(void);
	void	(*Z_LightOff)(void);
	void	(*Z_IRLightOn)(void);
	void	(*Z_IRLightOff)(void);
	void	(*Z_FocusOn)(void);
	void	(*Z_FocusOff)(void);
	void	(*Z_sysReset)(void);
	int		(*Z_GetIdleCounter)(void);
	void	(*Z_OscillateWithBeep)( int freq, int mdelay, int volume );
	void	(*Z_DeepSleep)(void);	//�������
	//���
	void	(*U_appPowerOnDisplay)(void);
	BOOL	(*U_appMsgAddTerminal)(void);
	void	(*U_appDevicePowerOnBeep)(void);
	int		(*U_appRunDecodeTest)(int nTestMode, int nTestParam);
	void*	Reserve[16];
}TUserAPI;

//App ��API
typedef struct zspTAPPAPI
{
	void	(*U_appSetMsg)(BYTE* pBuff, int nLen);
	void	(*U_appSetCategory)(int nCategory);
	void	(*U_appSetCodeType)(int nType);
	void	(*U_appSetResult)(int nResult);
	BYTE*	(*U_appGetMsgBuff)(void);
	int		(*U_appGetMsgLen)(void);
	int		(*U_appGetMsgBuffSize)(void);
	int		(*U_appGetCategory)(void);
	int		(*U_appGetCodeType)(void);
	int		(*U_appGetResult)(void);
	BOOL	(*U_appCaptureImg)(BOOL bWithLight);
	BOOL	(*U_appProcDecode)(void);
	BOOL	(*U_appMsgEditting)(void);
	BOOL	(*U_appMsgPacking)(void);
	BOOL	(*U_appDealCmd)(BYTE *pCmdString, int nLen);
	void	(*U_appSetUserFunc)(UTGeneralParam *psParam);
	BOOL	(*U_appKeyPressed)(void);
	void	(*U_appSleepMode)(void);
	void	(*U_appWillSleepTillWakeup)(int nIdleMiliSec);
	void	(*U_appGoingToSleep)(void);
	void	(*U_appBackFromSleep)(void);
	void	(*U_appBKGrndProc)(void); 
	void	(*U_appQueryAPI)(UTGeneralParam *psParam);
	BOOL	(*U_appDecodeWithMacro)(void);
	int		(*U_appGetVersionInfo)(char *pBuff, int nBuffSize);
	//���
	void	(*U_appPowerOnDisplay)(void);
	BOOL	(*U_appMsgAddTerminal)(void);
	void*	Reserve[18];
}TAppAPI;

//Communication
typedef struct zspTCOMMAPI
{
	void	(*V_commAutoCheckCmd)(void); 
	void	(*V_commSetUserFunc)(UTGeneralParam *psParam);
	int		(*Y_commGetCurType)(void);
	BOOL	(*Y_commEnable)(void);
	BOOL	(*Y_commIsEnable)(void);
	BOOL	(*Y_commDisable)(void);
	BOOL	(*Y_commChgToRS232)(TComParam *psParam);
	BOOL	(*Y_commChgToUSBDataPipe)(void);
	BOOL	(*Y_commChgToKBW)(UTKeyBoard *psParam);
	int 	(*Y_commGetBaudrate)(void);
	int 	(*Y_commGetKBWCountry)(void);
	int 	(*Y_commRead)(unsigned char * buff, unsigned int size, int msTimeOut);
	int 	(*Y_commWrite)(unsigned char *buff, unsigned int size);
	BOOL	(*Y_commClean)(void);
	int 	(*Y_commSendChar)(char cChar);
	int 	(*Y_commReadChar)(char *pcChar,int msTimeOut);
	int 	(*Y_commBuffCount)(void);
	int 	(*Y_commBakRead)(unsigned char * buff, unsigned int size, int msTimeOut);
	int 	(*Y_commBakReadChar)(char *pcChar,int msTimeOut);
	int 	(*Y_commBakBuffCount)(void);
	int 	(*Z_comm_GetType)(void); 
	int 	(*Z_comm_Read)(unsigned char * buff, unsigned int size);
	int		(*Z_comm_Write)(unsigned char * buff, unsigned int size);
	int		(*Z_comm_Config)(void * lpsParam); 
	int		(*Z_comm_GetStatue)(void * lpsParam);
	//���
	void*	Reserve[20];
}TCommAPI;

//Setting code
typedef struct zspTSETCAPI
{
	int		(*W_setcDealWith)(void); 
	void 	(*W_setcSetUserFunc)(UTGeneralParam *psParam);
	void	(*W_setcParamInit)(void); 
	BOOL	(*W_setcIsMacroCancel)(BYTE* pbyMsg, int nMsgLen);
	int		(*W_setcGetResult)(void);
	void	(*W_setcSetResult)(int nResult);
	BOOL	(*W_setcToSaveParam)(void);
	void	(*W_setcSetSaveFlag)(BOOL bToSave);
	void	(*W_setcEnable)(BOOL bEnable);
	BOOL	(*W_setcIsEnable)(void);
	BOOL	(*W_setcParseSigle)(BYTE* pbyMsg, int nMsgLen, BOOL bToSave);
	BOOL	(*W_setcParseBatch)(BYTE* pbyMsg, int nMsgLen);
	//���
	void*	Reserve[20];
}TSetcAPI;

//Macro code
typedef struct zspTMACROAPI
{
	void 	(*W_macroInitialParam)(void);
	int 	(*W_macroMain)(void);
	int 	(*W_macroGetParam)(void);
	void 	(*W_macroSetParam)(int nMacro);
	BYTE* 	(*W_macroGetBuff)(void); 
	int 	(*W_macroGetBuffLen)(void);
	int 	(*W_macroGetError)(void);
	void 	(*W_macroSetError)(int nError);
	int 	(*W_macroGetStatus)(void);
	BOOL	(*W_macroGetUnmacthParam)(void);
	int 	(*W_macroGetFlag)(void);
	void 	(*W_macroSetFlag)(int nFlag);
	//���
	void*	Reserve[20];
}TMacroAPI;

//Params storage
typedef struct zspTPARAMAPI
{
	BOOL	(*X_paramIsAllParamInited)(void);
	void	(*X_paramInitedAllParam)(void);
UALLPARAM* 	(*X_paramGetGlobalAllParam)(void);
	BOOL 	(*X_paramLoadSysDefaultParam)(void);
	BOOL 	(*X_paramLoadUsrDefaultParam)(void);
	BOOL 	(*X_paramSaveAsSysDefaultParam)(void); 
	BOOL 	(*X_paramSaveAsUsrDefaultParam)(void);
	BOOL 	(*X_paramLoadDeviceInfo)(void); 
	BOOL 	(*X_paramSaveDeviceInfo)(void);
	BOOL 	(*X_paramOriginalInitAllParam)(void);
	BOOL 	(*X_paramPlaceKeyword)(char* pKeyWord,char* pValue, BOOL bReplace);
	BOOL 	(*X_paramQueryKeyword)(char* pKeyWord,char* pValue);
	int 	(*X_paramAnalysisKeyword)(BYTE* pParam,int nParam,BYTE* pWord,BYTE * pValue);
	int 	(*X_paramGetKeywordsStruct)(BYTE *pBuff,int nBuff,TKEYSEG* tKS);
	BOOL 	(*X_paramIsParamsRight)(BYTE *pBuff, int nSize);
	BOOL 	(*X_paramGetParamFromBuff)(BYTE *pBuff, int nSize);
	int 	(*X_paramPushParamToBuff)(BYTE *pBuff, int nBuffSize);
	void 	(*X_paramSendParamInfo)(void);
	void 	(*X_paramUpdateToDevice)(void);
	int  	(*X_paramSelectUsefulCommType)(void);
	BOOL 	(*Y_paramLoadSysDefault)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramLoadUsrDefault)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramLoadOEM)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramLoadAppBlock)(int nBlockOrder, BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramSaveSysDefault)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramSaveUsrDefault)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramSaveOEM)(BYTE *pBuff, int nSize);
	BOOL 	(*Y_paramSaveAppBlock)(int nBlockOrder, BYTE *pBuff, int nSize);
	int  	(*Z_param_Load)(BYTE * buff, int nSectOffs, unsigned int size);
	int  	(*Z_param_Save)(BYTE * buff, int nSectOffs, unsigned int size);
	//���
	void*	Reserve[20];
}TParamAPI;

//CMOS
typedef struct zspTCMOSAPI
{
	void 	(*Y_cmosInit)(void);
	void 	(*Y_cmosCapture)(void);
	int 	(*Y_cmosGetWidth)(void);
	int 	(*Y_cmosGetHeight)(void);
	char* 	(*Y_cmosGetDataAddr)(void);
	void 	(*Y_cmosSetWantLux)(int nWantLux);
	void 	(*Y_cmosAdjust)(void);
	int 	(*Y_cmosCalcLux)(void);
	void 	(*Y_cmosMirror)(int nMode);
	void 	(*Y_cmosSetUserFunc)(UTGeneralParam *psParam);
	int  	(*Z_cmos_GetType)(void); 
	int  	(*Z_cmos_GetI2C)(int reg);	
	int  	(*Z_cmos_SetI2C)(int reg, int value);
	int*  	(*Z_cmos_Read)(int mode); 
	int*  	(*Z_cmos_GetIMGbuff)(void);
	//���
	void*	Reserve[20];
}TCmosAPI;

//Decode
typedef struct zspTDECODEAPI
{
	BOOL  	(*X_decoSetProcParamS)(UTGeneralParam * psParam);
	BOOL  	(*X_decoSetUserFunc)(UTGeneralParam *psParam);
	void	(*X_decoInitProcParam)(void);
	int		(*X_decoGetUIMGVersion)(char *pBuff, int nBuffSize);
	BOOL	(*X_decoImgProcMain)(void);
	void	(*X_decoSetImgParam)(UTImgParam *psParam);
	BYTE*	(*X_decoGetDecInfoBuff)(void);
	int		(*X_decoGetDecInfoLen)(void);
	int		(*X_decoGetResult)(void);
	BOOL  	(*X_decoIsMacroCode)(void);
	int  	(*X_decoGetMacroOrderFrom)(void);
	int   	(*X_decoGetMacroOrderTo)(void);
	int   	(*X_decoGetMacroTotalCnt)(void);
	int   	(*X_decoGetMacroIDLen)(void);
	int	  	(*X_decoGetMacroID)(int nPos);
TOUTPUTMSG*	(*X_decoGetDecOutMsg)(void);
	int		(*X_decoGetBarCode)(void);
	int		(*X_decoGetCodeType)(void);
	int		(*X_decoGetCodePos)(void);
	int		(*X_decoGetBitsPosition)(int nInt);
	void	(*X_decoSet2DParam)(UTDec2DSetup *psParam);
	void	(*X_decoSet1DParam)(UTDec1DSetup *psParam);
	void	(*X_decoSetOCRParam)(UTDecOCRSetup *psParam);
	//���
	void	(*X_decoSetPostalParam)(UTDecPostalSetup *psParam);
	void*	Reserve[19];
}TDecoAPI;

//Foundation api
typedef struct zspTBASEAPI
{
	void	(*Z_Beep)( int freq, int mdelay, int volume );
	void	(*Z_PlayMusic)(char * pcMusic);
	void	(*Z_usDelay)(int us);
	void	(*Z_msDelay)(int ms);
	int 	(*Z_CounterGet)(void); 
	int 	(*Z_GetCounterDensity)(void); 
	int 	(*Z_Get10msCounter)(void);	
	void	(*Z_Oscillate)(int ms);
	void	(*Z_DogFeed)(void);
	int 	(*Z_DogGetBreak)(void); 
	int 	(*Z_DogReset)(int ms); 
	int 	(*Z_KeyPressed)(void);
	void	(*Z_LedOn)(int clr);
	void	(*Z_LedOff)(int clr);
	void	(*Z_LightOn)(void);
	void	(*Z_LightOff)(void);
	void	(*Z_IRLightOn)(void);
	void	(*Z_IRLightOff)(void);
	void	(*Z_FocusOn)(void);
	void	(*Z_FocusOff)(void);
	int 	(*Z_oem_GetUid)(BYTE * buff, unsigned int maxsize);
	int 	(*Z_oem_GetLicense)(BYTE * buff, unsigned int maxsize);
	int 	(*Z_oem_SetLicense)(BYTE * buff, unsigned int size);
	int 	(*Z_oem_LicenseLock)(void); 
	void	(*Z_sysReset)(void);
	int 	(*Z_sysSleep)(void);
	int		(*Z_GetIdleCounter)(void);
	
	int		(*Z_param_Load)(unsigned char * buff, int nSectOffs, unsigned int size);
	int		(*Z_param_Save)(unsigned char * buff, int nSectOffs, unsigned int size);
	int		(*Z_cmos_GetI2C)(int reg);
	int		(*Z_cmos_SetI2C)(int reg, int value);
	int*	(*Z_cmos_Read)(int mode); 
	int*	(*Z_cmos_GetIMGbuff)(void);
	int		(*Z_cmos_GetType)(void);
	
	int		(*Z_GetHardwareInfo)(char *pBuff, int nBuffSize); //��ȡӲ���汾��Ϣ
	void	(*Z_DeepSleep)(void);	//�������
	//���
	void*	Reserve[20];
}TBaseAPI;


#endif//__UIMG_DEVELOP_INTERFACE_COMMON_H__



