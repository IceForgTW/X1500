
#include "SetCodeParse.h"

//#include "stdio.h"
#include "../../NLS_SCProtocol/SC_Function.h"
#include "../../NLS_SCProtocol/NLSComm_Proc.h"
#include <string.h>
#include <stdio.h>
/********************************************************************************************
*	local referenced extern function and variables 
********************************************************************************************/
FNC_setcDealUserFunc	W_setcDealUserFunc = NULL;
/*********************************************************************************************
*	global variables
*********************************************************************************************/
extern void paramSaveType(unsigned char saveFlag);
extern unsigned int g_commChange;
//�����Ժ궨��                  	            	
#define MainSet_DataAndTotal    	0				//���������ۺ�����
#define MainSet_Comm            	1				//ͨѶ����
#define MainSet_Hard            	2				//Ӳ������
#define MainSet_DecodeMode      	3				//����ģʽ������
#define MainSet_Code1D          	4				//һά��������
#define MainSet_Code2D          	5				//��ά��������
#define MainSet_OCR             	6				//OCR����
#define MainSet_UserType        	7				//�û���������
#define MainSet_2DNonStandar    	8				//��ά�Ǳ��������

#define MainSet_NewComm            11
#define MainSet_NewDecMode         12 

#define DecMode_NewOscillation     16  //��������
                                	            	
//�����Ժ궨��     

	//data format �������
#define DataFormat_Enter			2
#define DataFormat_Save				1
#define DataFormat_Set				0
#define DataFormat_UsedID			3
#define DataFormat_OutFlag			4
#define DataFormat_SingleFormatID	5
#define DataFormat_isSingle			6
#define DataFormat_ErrorTone		7
#define DataFormat_Discard			8
#define DataFormat_Clear			9
#define DataFormat_default			10

#define SubSet_Data             	0				//������
#define SubSet_Total            	1				//�ۺ�����
#define SubSet_SetCodeSend      	2				//��������Ϣ��������
#define SubSet_SetSysMesSend    	3				//��������Ϣ��������
#define SubSet_Set1DCodeID      	4				//�Զ���һά����CodeIDǰ׺����
#define SubSet_Set2DCodeID      	5				//�Զ����ά����CodeIDǰ׺����
#define SubSet_IsOpenSetCode    	6				//�����빦��������ر�����
#define SubSet_IsPowerOn        	7				//��Դ����ʱ��Ϣ��ʾ����
#define SubSet_CloseDevice          8               //�ر��豸����
                                	            	
                                	            	
#define MulrInfoSet_LED				2				//LED������
                                	            	
#define Set_Code1D_Code128      	0           	
#define Set_Code1D_EAN8         	1           	
#define Set_Code1D_EAN13        	2           	
#define Set_Code1D_UPCE         	3           	
#define Set_Code1D_UPCA         	4           	
#define Set_Code1D_Inter2Of5    	5           	
#define Set_Code1D_Matrix25     	6           	
#define Set_Code1D_Module25     	7           	
#define Set_Code1D_Regular39    	8           	
#define Set_Code1D_Codabar      	9           	
#define Set_Code1D_Code93       	10          	
#define Set_Code1D_CodeZ        	11          	
#define Set_Code1D_UCCEAN128		12
#define Set_Code1D_RSS				13
#define Set_Code1D_COMPOSITE		14
#define Set_Code1D_CODE11			15
#define Set_Code1D_ISBN				16
#define Set_Code1D_INDUSTRIAL25		17
#define Set_Code1D_STANDARD25		18
#define Set_Code1D_PLESSEY			19
#define Set_Code1D_MSI_PLESSEY		20
#define Set_Code1D_ISSN				21
#define Set_Code1D_ChnPost25			22

                                                	
//�ۺ����ú궨��                                	
#define TotalSet_All_Default     	0           	
#define TotalSet_All_Off         	1           	
#define TotalSet_All_On          	2           	
#define TotalSet_All_1D_Off      	3           	
#define TotalSet_All_1D_On       	4           	
#define TotalSet_All_2D_Off      	5           	
#define TotalSet_All_2D_On       	6           	
#define TotalSet_All_1D_OnlyOne  	7				//һά����ֻ������
#define TotalSet_All_1D_OneOrTwo 	8				//�ɶ�˫һά�루ͬһ���ͣ�����Ҳ���Զ���һά��
#define TotalSet_All_1D_OnlyTwo  	9				//�ɶ�˫һά�루ͬһ���ͣ�����ֻ��˫һά��
#define TotalSet_UserDef_On      	10				//�����û��Զ�������Ĭ��
#define TotalSet_UserDefPart_On  	11				//�����û��Զ��岿��Ĭ��
#define TotalSet_SN_On           	12				//������ƷSN����
#define TotalSet_ESN_On          	13				//������ƷESN����
#define TotalSet_Date_On         	14				//������ƷDate����
#define TotalSet_SaveToUserDef   	15				//����ǰ���ô�Ϊ�û�Ĭ������
#define TotalSet_LoadUserDef     	16				//�����û�Ĭ������
#define TotalSet_LIC_On             17              //���ò�ƷLicence
#define TotalSet_ManufID_On         18              //���ò�ƷManufactureID
#define TotalSet_IsDownload         19              //���ò�ƷIsDownload
#define TotalSet_SaveToFactory		20				//����ǰ���ô�Ϊ��������
#define TotalSet_Load_General		21				//����ͨ�ó�������                                                	
#define TotalSet_ManufName_On       22              //���ò�ƷManufactureName
#define TotalSet_All_Postal_Off		23
#define TotalSet_All_Postal_On		24
#define TotalSet_Driver_License		25
                                                	
//������궨��                                  	
#define POS_Save                 	16				//����
#define POS_Discard1Data         	17				//ȡ��ǰһ�ζ���һλ����
#define POS_DiscardAllData       	18				//ȡ��ǰ�����һ������
#define POS_DiscardSet           	19				//ȡ����ǰ�����С�����ͼ����������
                                 	            	
//���������ͺ궨��               	            	
#define DataSetType_No           	0				//������
#define DataSetType_1DMin        	1				//һά����С��������
#define DataSetType_1DMax        	2				//һά����󳤶�����
#define DataSetType_2DMin        	3				//��ά����С��������
#define DataSetType_2DMax        	4				//��ά����󳤶�����
#define DataSetType_OnceDelay    	5				//һ�ζ����ӳ�ʱ��
#define DataSetType_InfoDelay    	6				//ESC +2����������״̬�жϵ�ʱ����
#define DataSetType_Prefix       	7				//�Զ���ǰ׺����
#define DataSetType_Suffix       	8				//�Զ����׺����
#define	DataSetType_Terminal     	9				//�Զ������������
#define DataSetType_FixedLen     	10				//���ĳ������ֻ��ĳЩ����ֵ
#define DataSetType_OCRMin       	11				//OCR��С��������
#define DataSetType_OCRMax      	12				//OCR��󳤶�����
#define DataSetType_1DCodeID    	13				//�Զ���һά�����CodeID����
#define DataSetType_2DCodeID    	14				//�Զ����ά�����CodeID����
#define DataSetType_Cut         	15				//��ȡ����
#define DataSetType_Fixed_Dis   	16				//ĳ������ȡ��ֻ��ĳЩ����ֵ
#define DataSetType_Sensibility 	17				//������ֵ����
#define DataSetType_ScanTime    	18				//��������޶�����
#define DataSetType_NonStandar  	19				//�Ǳ����ֵ����
#define DataSetType_ErrorMsg       	20				//һ��ʱ����û�����뷵����Ϣ����

#define DataSetType_PostalCodeID    22				//�Զ����������CodeID����


////
#define DataSetType_Oscillation   0x1a   //��������

#define DataSetType_KBW           0x30   //KBW����
#define DataSetType_NetCard       0x31   //��������
#define DataSetType_BlueT         0x32   //��������
#define DataSetType_PS2           0x33   //PS2����

#define DataSetType_DecodeArea    0x40				//������������

#define DataSetType_ReadBeepFreq	0x41				//����ɹ�����������Ƶ��
#define DataSetType_ReadBeepDura	0x42				//����ɹ����������γ���ʱ��
#define DataSetType_ReadLEDDura		0x43				//����ɹ�LED����ʱ��

#define DataSetType_DataFormatting	0x50
#define DataSetKBW_Nationality    0  //���̹�����������
#define DataSetKBW_CharDelay      1  //�ַ��ӳ�
#define DataSetKBW_CodeDelay      2  //�����ӳ�
#define DataSetKBW_CodePage       3  //code page

#define DataSetBlueT_Name         0  //������������
#define DataSetBlueT_Password     1  //���������ַ�����

#define KBW_Nationality           0
#define KBW_CapsLock_Off          1
#define KBW_CapsLock_On           2
//#define KBW_NumLock_Off         3
//#define KBW_NumLock_On          4
#define KBW_PromptUnknowChar      3
#define KBW_CovertCase            4
#define KBW_KeystrokeDelay        5
//#define KBW_CodeDelay           6
#define KBW_KeypadEmulation       6
#define KBW_ShiftLock_Off         7
#define KBW_ShiftLock_On          8
#define KBW_AutoCapsLock_Off      9
#define KBW_AutoCapsLock_On      10
#define KBW_AutoNumLock_Off      11
#define KBW_AutoNumLock_On       12
#define KBW_CtrlASCIIMode_Off    13
#define KBW_CtrlASCIIMode_On     14
#define KBW_KEYPAD				 15
#define KBW_FastMode			 16
#define KBW_PollRate			 17
#define KBW_CodePage			 18


#define BlueT_Reset          0
#define BlueT_Name           1
#define BlueT_PassWord       2

#define PS2_CapsLockDef           0

////
                                	            	
#define Max_nPreSubfix          	10				//ǰ��׺�����Ϣ����
#define Max_FixedLen            	2				//����̶����ȵĲ�������
#define Max_DataCutLen             	20				//��Ϣ��ȡ������󳤶�
#define Max_Delay					3600000			//���ʱ���ӳ�
#define Max_Sensibility             20              //�й����������ֵ
                                                	
//ͨѶ���ú궨��                                	
#define SubSet_BoundRate			0				//����������
#define SubSet_Parity           	1				//У������
#define SubSet_WordLenStop      	2				//�ַ�����ֹͣλ����
#define SubSet_WordLenDataBits		3				//�ַ���������λ����
#define SubSet_HardwareFlowCtrl		4				//Ӳ����������
                                	            	
//����ģʽ�����ú궨��          	            	
#define DecModeSet_Prefix			0				//�Զ���ǰ׺����
#define DecModeSet_Suffix			1				//�Զ����׺����
#define DecModeSet_Repeat			2				//�ظ���������
#define DecModeSet_Spring			3				//���ܴ��ڿ�������
#define DecModeSet_Protect			4				//ͼ���˲�����
#define DecModeSet_IsPrefix			5				//�Զ���ǰ׺ʹ�ܻ��ֹ����
#define DecModeSet_IsSuffix			6				//�Զ����׺ʹ�ܻ��ֹ����
#define DecModeSet_IsCodeID			7				//CodeIDǰ׺ʹ�ܻ��ֹ����
#define DecModeSet_IsAIM			8				//AIMǰ׺ʹ�ܻ��ֹ����
#define DecModeSet_IsTerminal		9				//��������׺ʹ�ܻ��ֹ����
#define DecModeSet_Terminal			10				//��������׺����
#define DecModeSet_AllPreSuf		11				//����ǰ��׺�򿪻���������
#define DecModeSet_Sensibility		12				//����������
#define DecModeSet_Delay			13				//�ӳ�����
#define DecModeSet_DataPacked		14				//���ݴ������
#define DecModeSet_IsCut			15				//��ȡʹ�ܻ��ֹ����
#define DecModeSet_Cut				16				//��ȡλ������
#define DecModeSet_PrefixOrder		17				//ǰ׺˳������
#define DecModeSet_ScanTime			18				//��������޶�����
#define DecModeSet_Medium			19				//�����������
#define DecModeSet_ErrorMsg         20              //һ��ʱ����û�����뷵����Ϣ����
#define DecModeSet_CMOSAdjustMode   21              //
#define DecModeSet_DecodeArea		22              //
#define DecModeSet_DataFormat	    23                               	            	
                                	            	
#define POS_Macro					0   
   	
//�����û����ú�Ӳ������        	            	
#define POS_UserType_Group1     	0				//�û����õ�һ��
                                	            	
                                	            	
//�����������ú궨��            	            	
#define Set_Default					0				//Ĭ������
#define Set_Off						1				//��ֹ����
#define Set_On						2				//ʹ������
#define Set_MinLen					3				//��С���볤������
#define Set_MaxLen					4				//�����볤������
#define MicroQR_On					10              //ʹ��ʶ��MicroQR 
#define MicroQR_Off					11              //��ֹʶ��MicroQR 
#define QR_ECI						15              //��ֹECI���      
#define QR_CharEncoding				16
                              	            	
// �Ǳ�׼ǰ��׺ʹ�����ú궨��    	            	
// #define POS_IsBlue2D_LenSend    	0				//Blue2D��Ϣ���ȷ�������
// #define POS_IsBlue2D_TypeSend   	1				//Blue2D�������ͷ�������
// #define POS_IsBlue2D_Sensitive  	2				//Blue2D��������ʹ������
                                	
//�����������ñ��е���ʼλ��    	
#define POS_COM1_Param				8
#define POS_Light_Param        		12
#define POS_Code1D_Param       		16
#define POS_Code2D_Param       		20
#define POS_ModeSign_Param     		24
#define POS_Delay_Param        		26
#define POS_UpcEan_Param       		28
#define POS_Code128_Param      		32
#define POS_Codabar_Param      		36
#define POS_Code39_Param       		40
#define POS_Inter2Of5_Param    		44
#define POS_Matrix_Param       		48
#define POS_Module_Param       		52
#define POS_MacroSet_Param     		56
#define POS_PDF417_Param       		60
#define POS_QR_Param           		64
#define POS_Datamatrix_Param   		68

//����һά�������õĺ궨��
//Code128�����ú궨��
#define Code128_UCCEAN_Off      	5
#define Code128_UCCEAN_FNC1_1   	6
#define Code128_UCCEAN_FNC1_2   	7
#define Code128_FNC2_Off        	8
#define Code128_FNC2_1          	9
#define Code128_FNC2_2         		10
#define Code128_UCCEAN_FNC3_1  		11
#define Code128_UCCEAN_FNC3_2  		12
#define Code128_UCCEAN_FNC4_1  		13
#define Code128_UCCEAN_FNC4_2  		14
#define Code128_CHECK_ON	  		15   // У���鿪�� // add by Gavin 20080313
#define Code128_CHECK_OFF  		    16   // ���齻��ر� // add by Gavin 20080313
                               	
//EAN-UPC�����ú궨��          	
#define EANUPC_SendCheck_Off    	3				//������У���
#define EANUPC_SendCheck_On     	4				//����У���
#define EANUPC_2Expand_Off      	5				//��ʶ����λ��չ��
#define EANUPC_2Expand_On       	6				//ʶ����λ��չ��
#define EANUPC_5Expand_Off      	7				//��ʶ����λ��չ��
#define EANUPC_5Expand_On       	8				//ʶ����λ��չ��
#define EANUPC_EAN8To13_Off     	9				//EAN8���ѽ����չ��EAN-13
#define EANUPC_EAN8To13_On     		10				//EAN8�ѽ����չ��EAN-13
//#define EANUPC_UPC_Pre0_Off     	9				//UPC������ǰ���ַ�'0'
//#define EANUPC_UPC_Pre0_On     		10				//UPC����ǰ���ַ�'0'
//#define EANUPC_UPCETo13_Off    		11				//UPCE���ѽ����չ��EAN-13
//#define EANUPC_UPCETo13_On     		12				//UPCE�ѽ����չ��EAN-13
#define UPCE_Preamble_Off     				9				//UPCE������country code'0'��system character '0'
#define UPCE_Preamble_SystemChar_On     	10				//UPCE����system character '0'
#define UPCE_ToUPCA_Off    					11				//UPCE����ת��UPC-A
#define UPCE_DataTo13_On     				12				//UPCE�ѽ����չ��UPC-A
#define UPCE_ToUPCA_On     					13				//UPC-E ת����UPC-A
#define UPCE_Preamble_ALL_On     			14				//UPCE����country code'0' + system character '0'

#define UPCA_Preamble_SystemChar_On 9				//UPCA����system character
#define UPCA_Preamble_ALL_On     	10				//UPCA����country code + system character 
#define UPCA_Coupon_Off  			13				//��ֹUPCA+Coupon��չ
#define UPCA_Coupon_Allow			14				//����UPCA+Coupon��չ
#define UPCA_Coupon_Require			15				//����UPCA+Coupon��չ
#define UPCA_Coupon_RSS_Output_Off 	16				//�������coupon��Ϣ
#define UPCA_Coupon_RSS_Output_On 	17				//ֻ���RSS����Ϣ
#define UPCA_Preamble_Off     		18				//UPCA������country code + system character 


#define EAN13_Coupon_Off  			11				//��ֹEAN13+Coupon��չ
#define EAN13_Coupon_Allow			12				//����EAN13+Coupon��չ
#define EAN13_Coupon_Require		13				//����EAN13+Coupon��չ
                                                	
//INTER2OF5�����ú궨��                         	
#define Inter2Of5_Check_Off       	5				//������У���
#define Inter2Of5_Check_Send_Off  	6				//����У�������������У���
#define Inter2Of5_Check_Send_On   	7				//����У������Ҵ���У���
#define Inter2Of5_ITF14_Off       	8				//������ITF14����
#define Inter2Of5_ITF14_Check_Off 	9				//����ITF14���룬��������У���
#define Inter2Of5_ITF14_Check_On 	10				//����ITF14�����Ҵ���У���
#define Inter2Of5_ITF6_Off       	11			 	//������ITF6����
#define Inter2Of5_ITF6_Check_Off 	12 				//����ITF6���룬��������У���
#define Inter2Of5_ITF6_Check_On  	13 				//����ITF6�����Ҵ���У���
#define Inter2Of5_FixedLen_Off   	14 				//���ý���25��ֻʶ�������ر�
#define Inter2Of5_FixedLen_On    	15 				//���ý���25��ֻʶ��������
#define Inter2Of5_FixedLen_Ena   	16 				//���ý���25��ʶ�������ĳ���
#define Inter2Of5_FixedLen_Dis   	17 				//ȡ������25��ʶ�������ĳ���
                                       				
//Matrix25�����ú궨��                 				
#define Matrix25_Check_Off        	5  				//������У���
#define Matrix25_Check_Send_Off   	6  				//����У�������������У���
#define Matrix25_Check_Send_On    	7  				//����У������Ҵ���У���
                                  	   				
//Module25�����ú궨��            	   				
#define Module25_Check_Off        	5  				//������У���
#define Module25_Check_Send_Off   	6  				//����У�������������У���
#define Module25_Check_Send_On    	7  				//����У������Ҵ���У���
                                       				
//CODE39�����ú궨��                   				
#define Code39_Check_Off       		5  				//������У���
#define Code39_Check_Send_Off  		6  				//����У�������������У���
#define Code39_Check_Send_On   		7  				//����У������Ҵ���У���
#define Code39_StartStop_Off   		8  				//��������ʼ������ֹ��
#define Code39_StartStop_On    		9  				//������ʼ������ֹ��
#define Code39_FullASCII_Off  		10 				  //��ʶ��ȫASCII��
#define Code39_FullASCII_On   		11 				  //ʶ��ȫASCII��
#define Code39_Code32_Off   		12 				  //��ת��ΪCode32
#define Code39_Code32_On   			13 				  //ת��ΪCode32  
#define Code39_Code32_Star_Off   	14 				  //Code32�������ʼ��
#define Code39_Code32_Star_On		15 				  //Code32  �����ʼ��  

                                     				
//CODABAR�����ú궨��                  				
#define Codabar_Check_Off       	5  				//������У���
#define Codabar_Check_Send_Off  	6  				//����У�������������У���
#define Codabar_Check_Send_On   	7  				//����У������Ҵ���У���
#define Codabar_StartStop_Off   	8  				//��������ʼ������ֹ��
#define Codabar_StartStop_On    	9  				//������ʼ������ֹ��
#define Codabar_ABCDTNXE_Off		10				//��ʼ������ֹ��Ϊ��ͨABCD��ʽ
#define Codabar_ABCDTNXE_On			11				//��ʼ������ֹ��ΪABCD/TN*E��ʽ
#define Codabar_LOWERCASE_Off		12				//��ʼ������ֹ��Ϊ��д
#define Codabar_LOWERCASE_On		13				//��ʼ������ֹ��ΪСд

//CODE 93�����ú궨��   // add by Gavin 20080313  
#define Code93_Check_Off       	5  				//������У���   // add by Gavin 20080313
#define Code93_Check_Send_Off  	6  				//����У�������������У���   // add by Gavin 20080313
#define Code93_Check_Send_On   	7  				//����У������Ҵ���У���   // add by Gavin 20080313
                                	   				
//CODEZ�����ú궨��             	   				
#define CodeZ_Error_Send_Off    	5  				//ֻ����������Ϣ
#define CodeZ_Error_Send_On     	6  				//����������Ϣ�;�����Ϣ

//EAN��UCC������(COMPOSITE)���ú궨��
#define COMPOSITE_UCCEAN_Off		3
#define COMPOSITE_UCCEAN_On			4

//RSS���ú궨��
#define RSS_AI_Send_Off			5
#define RSS_AI_Send_On			6

//CODE11���ú궨��
#define CODE11_No_Check						5	//��У��
#define CODE11_C11Chk1						6	//һλУ�飬MOD11
#define CODE11_C11M11M11Chk					7	//��λУ��MOD11/MOD11
#define CODE11_C11M11M9Chk					8	//��λУ��MOD11/MOD9
#define CODE11_10char_C11M11M11Chk			9	//MOD11��У��(Len<=10),MOD11/MOD11˫У��(Len>10)
#define CODE11_10char_C11M11M9Chk			10	//MOD11��У��(Len<=10),MOD11/MOD9˫У��(Len>1)
#define CODE11_No_Send_Chk					11	//������У���
#define CODE11_Send_Chk						12	//����У���

//MSIPLESSEY���ú궨��
#define MSI_No_CHK			5		//��У��
#define MSI_CHK_M10			6		//��MOD10У��
#define MSI_CHK_M10M10		7		//˫MOD10У��
#define MSI_CHK_M10M11		8		//MOD10/MOD11˫У��
#define MSI_Check_Send_Off	9		//������У��λ
#define MSI_Check_Send_On	10		//����У��λ


//PLESSEY���ú궨��
#define PLESSEY_No_CHK			5	//��У��
#define PLESSEY_CHK_NO_Send		6	//У�鲻����
#define PLESSEY_CHK_Send		7	//У�鲢����

//STANDARD25���ú궨��
#define STANDARD25_No_CHK			5	//��У��
#define STANDARD25_CHK_NO_Send		6	//У�鲻����
#define STANDARD25_CHK_Send			7	//У�鲢����

//INDUSTRIAL25���ú궨��
#define INDUSTRIAL25_No_CHK				5	//��У��
#define INDUSTRIAL25_CHK_NO_Send		6	//У�鲻����
#define INDUSTRIAL25_CHK_Send			7	//У�鲢����

//ISBN���ú궨��
#define ISBN_Send_13		3	//����13������
#define ISBN_Send_10		4	//����10������

//China Post 25�����ú궨��                 				
#define ChnPost25_Check_Off        	5  				//������У���
#define ChnPost25_Check_Send_Off   	6  				//����У�������������У���
#define ChnPost25_Check_Send_On    	7  				//����У������Ҵ���У���


//PDF417�����ú궨��
#define PDF417_Double_OnlyOne       7
#define PDF417_Double_OnlyTwo       8
#define PDF417_Double_OneOrTwo      9
#define PDF417_Reverse             32
#define PDF417_Mirror              33
#define PDF417_ECI					34
#define PDF417_CharEnCoding			35

//QR�����ú궨��
#define QR_Double_OnlyOne           7
#define QR_Double_OnlyTwo           8
#define QR_Double_OneOrTwo          9

//DataMatrix�����ú궨��
#define DataMatrix_129_Off          5                                	   				
#define DataMatrix_129_On           6
#define DataMatrix_Double_OnlyOne   7
#define DataMatrix_Double_OnlyTwo   8
#define DataMatrix_Double_OneOrTwo  9
#define DataMatrix_Rectangular_Off  10
#define DataMatrix_Rectangular_On   11
#define DataMatrix_AddPADCode_Off   12
#define DataMatrix_AddPADCode_On    13
#define DataMatrix_Reverse          32
#define DataMatrix_Mirror           33
#define DataMatrix_ECI				34
#define DataMatrix_CharEnCoding		35

//Aztec�����ú궨��
#define Aztec_Multi_Num             6 //����ͬͼ��������
#define Aztec_Multi_None            7 //��ֹ����ͬͼ
#define Aztec_Multi_Only            8 //ֻ���̶���������������
#define Aztec_Multi_Mixed           9 //���̶����������ģ����ȣ���ֻ��һ������
#define Aztec_ECI		           10
#define Aztec_CharEnCoding         11

#define VeriCode_UnZip_ModeUp       5
#define VeriCode_UnZip_ModeDown     6

                                	  
//���������ú궨��
#define HAN_SENSIBLE_Double_OnlyOne   5
#define HAN_SENSIBLE_Double_OnlyTwo   6
#define HAN_SENSIBLE_Double_OneOrTwo  7
#define HAN_SENSIBLE_Reverse          8
 				
//��ά��������ú궨��          	   				
#define Macro_Cancel            	0  				//ȡ����������Ϣ
#define Macro_DirectSend        	1  				//ÿ�����ݿ����Ϣ��ȡ��ֱ�Ӵ���
#define Macro_SendInTurn        	2  				//��˳���ͣ����治����64kbyte����
		                        	   				//�����ȵ�ǰ���ݿ�����Ӻ�С����
		                        	   				//�ݿ鶼��ʶ��ʱ������ЩĿǰ�Ѿ�
                                	   				//���Ӻõ����ݣ�������ǰ�飩����
                                	   				
#define Macro_LinkSendAfterRead 	3  				//����ȫ�������ݿ�������ӷ���,
		                           					//����������������64kʱ����ģʽ2��
                                   					//��ʽ��������

//ͨѶ��������
#define Comm_Interface          0
#define Comm_RS232              1
#define Comm_USB                2
#define Comm_KBW                3
#define Comm_NetCard            4
#define Comm_BlueTooth          5
#define Comm_PS2                6
#define Comm_HID_POS            7


//���������͵����������õ���������
#define Replace_No          		0
#define Replace_UserDef     		1
#define Replace_UserPart    		2
#define Replace_SN          		3
#define Replace_ESN         		4
#define Replace_Date        		5
#define Replace_LIC                 6
#define Replace_ManufID             7
#define Replace_ManufName           8
#define Replace_NewInstruction  9
                            		

#define  SENSITIVE_CALC(x)   (2 + (4 - (x)) * 3)  //��Ӧ���������ֵ�ù�ϵ
/********************************************************************************************
*	local referenced macro defines & type defines
********************************************************************************************/
//���������С��Ϣ���ȿ�������
unsigned int g_DataSet,g_PosMaxMin;
int g_DataType;
BOOL g_IsSetCodeStart; //��ʼҪ��������
BOOL g_DataSet_Odd; //��ȡ�����������ż����

//���������С��Ϣ���ȿ�������
unsigned char g_PreSuffix[20];
int g_nPreSuffix;

int g_nFixedLen;  //�������ö���̶�����

//ͨ���������������ã����������͵����������õ�����
int g_ReplaceType  = Replace_No;

//���ú��Ƿ������������û�����Ĭ������ʱ�õ�

//����Blue2D���������
BOOL g_Isdiagnostics = FALSE;
unsigned char Blue2D_I2Of5Len[5];
unsigned int Blue2D_n1DSet = 0;
unsigned int Blue2D_n2DSet = 0;

const unsigned int  conBaudRate[9] = {1200,2400,4800,9600,14400,19200,38400,57600,115200};
//dataformatting����������
unsigned char g_DataFormatbuf[512];
int g_nDataFormatbuf;

struct _D_F //���������Ҫ
{
	int FormatType;
	int Ter_Type;
	int Code_ID;
	int Command_Len;
	char Command[Command_Num+1];
}DF;
int istage = 0;
int enter_set_flag = 0;  //����dataformat���ñ�־
int clear_flag = 0;
/*********************************************************************************************
*	local function prototype
*********************************************************************************************/

void SetCode_vITF25Param2Buff_Mini(int v1,int v2, char* buff);

BOOL Parse_SetCode(unsigned char *SetCode,int nType);  //����������ܺ���
BOOL Set_DataAndTotal(int Rec34, int Rec56, int Rec7);  //���������ۺ�����
BOOL Set_Comm(int Rec34, int Rec56);  //ͨѶ����
BOOL Set_Hard(int Rec34, int Rec56, int Rec7);  //Ӳ������
//BOOL Set_DecodeMode(int Rec34, int Rec56);  //����ģʽ������
BOOL Set_Code1D(int Rec34, int Rec56);  //һά��������
BOOL Set_Code2D(int Rec34, int Rec56, int Rec7);  //��ά��������
BOOL Set_OCR(int Rec34,int Rec56);  //OCR����
BOOL Set_Postal(int Rec34, int Rec56); //����������
BOOL Set_UserType(int Rec34, int Rec56);//�����û����ú�Ӳ������
BOOL SetCode_Data_PreSuffix(int Rec56);  //ǰ��׺��������
BOOL SetCode_Data_FixedLen(int Rec56);  //����̶���������
BOOL SetCode_Discard(int Rec56);  //ȡ�����ã�����ȡ��һ���ַ���һ���ַ���һ������
BOOL SetCode_Save(void);  //��������
BOOL SetCode_Save_1DMin(void);  //����һά������С��������
BOOL SetCode_Save_1DMax(void);  //����һά��������������
BOOL SetCode_Save_2DMin(void);  //�����ά������С��������
BOOL SetCode_Save_2DMax(void);  //�����ά��������������
BOOL SetCode_Save_OCRMin(void);  //����OCR��С��������
BOOL SetCode_Save_OCRMax(void);  //����OCR��󳤶�����
BOOL SetCode_Save_NonStandar(void); //�����ά��Ǳ����
BOOL SetCode_Total(int Rec56, int Rec7);  //�����ۺ�����
BOOL SetCode1D_Code128(int Rec56);  //128������
BOOL SetCode1D_EAN8(int Rec56); //EAN8����
BOOL SetCode1D_EAN13(int Rec56); //EAN13����
BOOL SetCode1D_UPCE(int Rec56); //UPCE����
BOOL SetCode1D_UPCA(int Rec56); //UPCA����
BOOL SetCode1D_Inter2of5(int Rec56);  //INTER25����
BOOL SetCode1D_Matrix25(int Rec56);  //Matrix25����
BOOL SetCode1D_Module25(int Rec56);  //Module25����
BOOL SetCode1D_Regular39(int Rec56);  //CODE39����
BOOL SetCode1D_Codabar(int Rec56);  //Codabar����
BOOL SetCode1D_Code93(int Rec56);  //Code93����
BOOL SetCode1D_CodeZ(int Rec56);//CodeZ������
BOOL SetCode1D_RSS(int Rec56);//RSS������
BOOL SetCode1D_COMPOSITE(int Rec56);//COMPOSITE��(UCC EAN������)����
BOOL SetCode1D_CODE11(int Rec56);//Code11������
BOOL SetCode1D_ISBN(int Rec56);//ISBN������
BOOL SetCode1D_INDUSTRIAL25(int Rec56);//INDUSTRIAL25������
BOOL SetCode1D_STANDARD25(int Rec56);//STANDARD25������
BOOL SetCode1D_PLESSEY(int Rec56);//PLESSEY������
BOOL SetCode1D_MSIPLESSEY(int Rec56);//MSI_PLESSEY������

BOOL SetCode1D_ChnPost25(int Rec56);//china post 25

BOOL SetCode2D_Macro(int Rec56);  //��ά���������
BOOL SetCode2D_PDF417(int Rec56, int Rec7);  //PDF417������
BOOL SetCode2D_QR(int Rec56, int Rec7);  //QR������
BOOL SetCode2D_Aztec(int Rec56, int Rec7);  //Aztec������
BOOL SetCode2D_DataMatrix(int Rec56, int Rec7);  //DataMatrix������
BOOL SetCode2D_MaxiCode(int Rec56);  //MaxiCode������
BOOL SetCode2D_VeriCode(int Rec56, int Rec7);  //VeriCode����
BOOL SetCode2D_Han(int Rec56, int Rec7);//����������
BOOL SetCode2D_UserDef(int Rec56, int Rec7); //�û��Զ����ά������
BOOL SetCode2D_GM(int Rec56); //GM������
BOOL SetCode2D_MicroPDF417(int Rec56); //MicroPDF417������
BOOL SetOCR_SPEC_OCR_B(int Rec56);  //SPEC_OCR_B����
BOOL Set_2DNonStandar(int Rec34, int Rec56); //��ά����Ǳ��������
BOOL Set_QRNonStandar(int Rec56); //QR����Ǳ��������
BOOL SetCode1D_ISSN(int Rec56);
BOOL SetCodePos_Postnet(int Rec56);
BOOL SetCodePos_Onecode(int Rec56);
BOOL SetCodePos_RM4CC(int Rec56);
BOOL SetCodePos_Planet(int Rec56);
BOOL SetCodePos_Kixcode(int Rec56);
BOOL SetCodePos_Auspost(int Rec56);

BOOL Set_NewComm(int Rec34, int Rec56, int Rec7);//��ͨѶ��������

BOOL Set_NewCommKBW(int Rec56, int Rec7);//KBW��������
BOOL Set_NewCommBlueT(int Rec56, int Rec7);//������������
BOOL SetCode_Save_KBW(void);//KBW����
BOOL SetCode_Save_BlueT(void); //��������
BOOL Set_NewCommPS2(int Rec56, int Rec7);//PS2��������
BOOL Set_NewDecMode(int Rec34, int Rec56, int Rec7);//�½���ģʽ����


BOOL Parse_UserPart(unsigned char *pMes, int nlen);//�û�����Ĭ��������
BOOL bUseDataCode(unsigned char *pMes);
BOOL GetKeyWord(unsigned char * pKeyWord, unsigned char *pMes, int nlen, int *nPlace);
BOOL bGetData(unsigned char *pValue, int nsize, unsigned char *pMes, int nlen, int *nPlace);
BOOL Set_DataFormat(int Rec56, int Rec7);

int GetHex(char c);
int StringToInt(unsigned char *pMes); //�������ַ����л�ȡ����


/*********************************************************************************************
*	Implementation functions 
*********************************************************************************************/

char int_to_char(int intparam)
{
	char rec;

	if (intparam >= 0 && intparam <= 9)
	{
		rec = intparam + 0x30;
	} 
	else if (intparam >9 && intparam <16)
	{
		rec = intparam - 10 + 'A'; 
	}
	else
		rec = 0x00;
	return rec;
	
}

/***********
*setfalg :0 ͨ������������
*        :>=1 ͨ�����ʽ����
*							1: ȫ���޸�
*							2: ter id
*							3: code id
*							4: ���ó���
*							5: �����ַ���
*/
int W_setFormat(int formatID,int setflag,char *cmd,int len)
{
	//��һ��������ID����Format
	UALLPARAM *psParam;
	char *p = NULL;
//	char *p1 = NULL;
	int i=0;
	DF_Param	dfparam;
	int flag;
	char temp;

	psParam = X_paramGetGlobalAllParam();
	if (setflag == 0) // ֻ��������������ʱ��Ҫ�жϷ���
	{
		if (enter_set_flag == 0 && clear_flag == 0)
			return 0;
	}
	if (clear_flag == 2) //ȫ��ɾ��
	{
		memset(psParam->pDFormat->format,0,sizeof(psParam->pDFormat->format));
		clear_flag = 0; 
		return 1;
	}
	if (setflag == 0)//����Ҫ
	{	psParam->pDFormat->format[formatID].FormatID = formatID;
	//		psParam->pDFormat->format[formatID].FormatType = DF.FormatType;
	psParam->pDFormat->format[formatID].FormatType = 6;
	/*		psParam->pDFormat->format[formatID].TerID = DF.Ter_Type;*/ //���ڻ�û�ж�ͨ�Žӿڵ����ù��ܣ��豸û�ж����ṩͨ�Žӿڵ�Id��
	psParam->pDFormat->format[formatID].TerID = 999;

	psParam->pDFormat->format[formatID].SymbolID = DF.Code_ID;
	psParam->pDFormat->format[formatID].MsgLen = DF.Command_Len;
	//	memcpy(psParam->pFromat->format[formatID].TerID,DF.Ter_Type,Ter_Type_Num);
	//	memcpy(psParam->pFromat->format[formatID].CodeID,DF.Code_ID,Code_ID_Num);
	//	memcpy(psParam->pFromat->format[formatID].ComLen,DF.Command_Len,Command_Len_Num);
	memset(psParam->pDFormat->format[formatID].Cmd,0,Command_Num+1);
	memcpy(psParam->pDFormat->format[formatID].Cmd,DF.Command,Command_Num);

	clear_flag = 0; //�����ɾ��format��ִ��Save����Ҫ���clear_flag��־��
	}
	else if (setflag == 1)
	{//����

		dfparam.nDFSelType = (int)cmd[7];

		if (dfparam.nDFSelType == 3)
		{
			p = cmd+17;
			for (i=0; i<len-17; i++)
			{
				temp = int_to_char((int)p[i]);
				p[i] = temp;
			}
			dfparam.nDFMsgLen = len - 18;
		} 
		else
		{
			p = cmd+18;
			for (i=0; i<len-18; i++)
			{
				temp = int_to_char((int)p[i]);
				p[i] = temp;
			}
			dfparam.nDFMsgLen = len - 19;
		}
		//Y_commSendString(p);
		dfparam.pDFCmd = (unsigned char *)p;
		dfparam.nDFSelType = 3;//������ֻ������������λ1��2��3		

		flag = GXDataFormat_IsRightCmd(&dfparam);

		if (flag < 0)
		{

			return 0;
		}
		memset(psParam->pDFormat->format[formatID].Cmd,0,len);

		psParam->pDFormat->format[formatID].FormatID = formatID;
		psParam->pDFormat->format[formatID].FormatType = (int)cmd[7];
		psParam->pDFormat->format[formatID].TerID = (int)cmd[8]*100+(int)cmd[9]*10+(int)cmd[10];
		if (psParam->pDFormat->format[formatID].FormatType == 3)
		{//Y_commSendString("type3?\n");
			psParam->pDFormat->format[formatID].SymbolID = (int)cmd[11]*10+(int)cmd[12];
			psParam->pDFormat->format[formatID].MsgLen = (int)cmd[13]*1000+(int)cmd[14]*100+(int)cmd[15]*10+(int)cmd[16];
			p = cmd+17;
			for(i = 0; i< len-17 ;i++)
				psParam->pDFormat->format[formatID].Cmd[i] = p[i];
		} 
		else
		{//Y_commSendString("type6?\n");
			psParam->pDFormat->format[formatID].SymbolID = (int)cmd[11]*100+(int)cmd[12]*10+(int)cmd[13];
			psParam->pDFormat->format[formatID].MsgLen = (int)cmd[14]*1000+(int)cmd[15]*100+(int)cmd[16]*10+(int)cmd[17];
			p = cmd+18;
			for(i = 0; i< len-18 ;i++)
				psParam->pDFormat->format[formatID].Cmd[i] = p[i];	
		}

	}
	else if (setflag == 2)  
	{//TerID
		//psParam->pDFormat->format[formatID].TerID = (int)cmd[7]*100+(int)cmd[8]*10+(int)cmd[9];
		psParam->pDFormat->format[formatID].TerID = 999;//�ݶ�
	}
	else if (setflag == 3)
	{//CodeID
		/* Y_commWrite((BYTE*)cmd[0],1);*/
		if ((int)cmd[7] == 3)
		{
			psParam->pDFormat->format[formatID].SymbolID = (int)cmd[7]*10+(int)cmd[8];
		} 
		else
		{
			psParam->pDFormat->format[formatID].SymbolID = (int)cmd[7]*100+(int)cmd[8]*10+(int)cmd[9];
		}


	}
	else if (setflag == 4)
	{//ComLen
		psParam->pDFormat->format[formatID].MsgLen = (int)cmd[7]*1000+(int)cmd[8]*100+(int)cmd[9]*10+(int)cmd[10];
	}
	else if (setflag == 5)
	{//�������ͺ���������
		memset(psParam->pDFormat->format[formatID].Cmd, 0, len);
		//psParam->pDFormat->format[formatID].FormatType = (int)cmd[7];
		p = cmd+7;
		for(i = 0; i< len-8 ;i++)
			psParam->pDFormat->format[formatID].Cmd[i] = int_to_char((int)p[i]);
		psParam->pDFormat->format[formatID].Cmd[i] = '\0';
	}


	if (setflag == 0) // ���������
	{
		formatID = -1;
		DF.FormatType = 0;
		DF.Ter_Type = 0;
		DF.Code_ID = 0;
		DF.Command_Len = 0;
		//	memset(DF.Ter_Type,0,Ter_Type_Num+1);
		//	memset(DF.Code_ID,0,Code_ID_Num+1);
		//	memset(DF.Command_Len,0,Command_Len_Num+1);
		memset(DF.Command,0,Command_Num+1);

	}
	return 1;
}


//add by HUNTON,2004.03.06
//��������Ϣ�����ܺ���
//nType: ����
//    0���а����������
//    1��Blue2D����Ľ�����������

BOOL	W_setcParseSigle(BYTE* pbyMsg, int nMsgLen, BOOL bToSave)
{
	int Rec12,Rec34,Rec56, Rec7;
	int RecParam = -1;

	istage = 0;

	if (nMsgLen < 7)
	{ return FALSE; }//End of if;

	W_setcSetSaveFlag(TRUE); //�������������ú��Ƿ񱣴�
	W_setcSetResult(U_uIMG_STATUS_SETCODE_SUC);
	
	Rec12 = (int)(pbyMsg[0] - '0') * 10 + (int)(pbyMsg[1] - '0');
	Rec34 = (int)(pbyMsg[2] - '0') * 10 + (int)(pbyMsg[3] - '0');
	Rec56 = (int)(pbyMsg[4] - '0') * 10 + (int)(pbyMsg[5] - '0');
	Rec7 = (int)(pbyMsg[6] - '0');

	if(nMsgLen > 7 && pbyMsg[7] == '=')
	{
		int lvi = 0;
		char *lvp = (char*)(pbyMsg + 8);
		while(*lvp)
		{
			if(*lvp >= '0' && *lvp <= '9')
			{
				lvi = lvi * 10 + (*lvp - '0');
			}
			else
				break;
			lvp ++;
		}
		RecParam = lvi;
	}

	switch(Rec12)
	{
	case MainSet_DataAndTotal://���������ۺ�����
		if(!Set_DataAndTotal(Rec34, Rec56, Rec7))
		{ return FALSE; }
		break;

	case MainSet_Comm://ͨѶ����
		W_setcParamInit();
		if(pbyMsg[6] == '0')
		{//��COM1����
		    if(!Set_Comm(Rec34, Rec56))
			{ return FALSE; }
		}
		else
		{ return FALSE; }
		break;

	case MainSet_Hard://Ӳ������
		W_setcParamInit();
		if(!Set_Hard(Rec34, Rec56, Rec7))
		{ return FALSE; }
		break;

	case MainSet_DecodeMode://����ģʽ����
		W_setcParamInit();
		if(!Set_DecodeMode(Rec34, Rec56, Rec7, RecParam))
		{ return FALSE; }
		break;

	case MainSet_Code1D://һά������
		W_setcParamInit();
		if(!Set_Code1D(Rec34, Rec56))
		{ return FALSE; }
		break;
		
	case MainSet_Code2D://��ά������
		W_setcParamInit();
		if(!Set_Code2D(Rec34, Rec56, Rec7))
		{ return FALSE; }
		break;
	
	case MainSet_OCR://OCR������������
		W_setcParamInit();
		if(Rec34 >= 40){	//������
			if(!Set_Postal(Rec34, Rec56))
			{ return FALSE; }
		}
		else {		//OCR
			if(!Set_OCR(Rec34, Rec56))
			{ return FALSE; }
		}
		
		break;

	case MainSet_UserType://�û���������
		W_setcParamInit();
		if(!Set_UserType(Rec34, Rec56))
		{ return FALSE; }
		break;
	case MainSet_2DNonStandar://��ά�Ǳ��������
		W_setcParamInit();
		if(!Set_2DNonStandar(Rec34, Rec56))
		{ return FALSE; }
		break;
	case MainSet_NewComm://ͨѶ����
		W_setcParamInit();
		if(!Set_NewComm(Rec34, Rec56, Rec7))
		{ return FALSE; }
		break;
	case MainSet_NewDecMode://�½���ģʽ����
		W_setcParamInit();
		if(!Set_NewDecMode(Rec34, Rec56, Rec7))
		{ return FALSE; }
		break;
	default:
		return FALSE;
    }//End of switch;

    if (istage == 0) // ���������data Format����ϣ���˴�����ʧЧ,��format�ĸ����ù���������istage=0��ȷ��format���õ�������
    {
		enter_set_flag = 0;
		clear_flag = 0;
    }


	if(bToSave == TRUE)
	{//�а�����������
		W_setcSetSaveFlag(TRUE);
	}
	else if(bToSave != TRUE)
	{ //Blue2D�������ã�������
		W_setcSetSaveFlag(FALSE);
		return TRUE;
	}//End of elseif;

	W_setcSetSaveFlag(TRUE);
	if(g_DataType != DataSetType_No) 
	{
		if(g_IsSetCodeStart)
		{
			W_setcSetResult(U_uIMG_STATUS_SETCODE_SUC);
			g_IsSetCodeStart = FALSE;
		}
		else
		{ W_setcSetResult(U_uIMG_STATUS_SETCODE_END); }
	}
	else 	
	{ W_setcSetResult(U_uIMG_STATUS_SETCODE_END); }

	return TRUE;
}

//�û�����Ĭ��������
BOOL	W_setcParseBatch(BYTE* pbyMsg, int nMsgLen)
{
	int i;
	int j;
	unsigned char SetCode[9] = "00001600";
	unsigned char *p;
	TKEYSEG SetMes;
	int nPlace = 0;

	i = 0;
	j = 0;
	p = pbyMsg;
     //   printf("%s\r\n",pbyMsg);
	while(i+3 < nMsgLen)
	{ //����Ҫ��3���ַ��Ĺؼ���
		memset(SetMes.pKeyWord, 0, sizeof(SetMes.pKeyWord));
    	memset(SetMes.pValue, 0, sizeof(SetMes.pValue));
		
		//���ҵ���ǰλ�ú���ĵ�һ������
		for(j = i; j < nMsgLen; j++)
		{
			if(pbyMsg[j] <= '9' && pbyMsg[j] >= '0')
			{ break; }
		}//End of for;
		if(j >= nMsgLen)
		{ return FALSE; } //����ȷ��ʽ

		i = j;
		p = pbyMsg + j;

		nPlace = 0;
	    if(!GetKeyWord(SetMes.pKeyWord, p, nMsgLen-j, &nPlace))
		{ return FALSE; }
		
		if(i + 7 >= nMsgLen)
		{ break; }
		//���������7λ��ʮ��������
		if(strlen((char *)(SetMes.pKeyWord)) < 7)
		{ return FALSE; }
		for(j = 0; j < 7; j++)
		{
			if(SetMes.pKeyWord[j] < '0' || SetMes.pKeyWord[j] > '9')
			{ return FALSE; }
		}//End of for;

		if(!W_setcParseSigle(SetMes.pKeyWord, 7, FALSE))
		{ return FALSE; }//1������ʱ������
		
		p = p + nPlace;
		i += nPlace;

		if(g_DataType != DataSetType_No) 
		{
			nPlace = 0;
			if (bGetData(SetMes.pValue, sizeof(SetMes.pValue), p, nMsgLen - i, &nPlace))
			{
				if(!bUseDataCode(SetMes.pValue))
				{ return FALSE; }
				g_ReplaceType = Replace_UserPart;
				if(!SetCode_Save())
				{ return FALSE; }
				i += nPlace;
			}
			else
			{ return FALSE; }
			g_DataType = DataSetType_No;
		}//End of if;
		else 
		{ //��Ҫ���������룬ֱ�����õ�
			for(j = i; j < nMsgLen; j++)
			{ //Ѱ�ҷֺ�
				if(pbyMsg[i] == ';')
				{ break;  }
			}//End of for;
			i = j;
			if(i < nMsgLen)
			{ i++; }
			else
			{ return FALSE; }
		}//End of else;
		g_ReplaceType = Replace_UserPart;
	}//End of while;
	
	if(!W_setcParseSigle(SetCode, 8,TRUE))
	{ return FALSE; }//����

	W_setcParamInit();

	return TRUE;
}//End of function;

//��ȡ�ؼ���
BOOL GetKeyWord(unsigned char * pKeyWord, unsigned char *pMes, int nlen, int *nPlace)
{
	int i;
	int j = 0;

	//�ؼ���ǰ��Ŀո�
	for (i = 0; i < nlen; i++)
	{
		if(pMes[i] != ' ')
		{ break; }
	}//End of for;

	if(i >= nlen)
	{ return FALSE; }
	//��ȡ�ؼ��֣������ո��Զ�����,���ǷֺŻ�Ⱥ�,������
	for( ; i < nlen; i++)
	{
		if(pMes[i] == ' ')
		{ continue; }
		if((pMes[i] == ';') || (pMes[i] == '='))
		{ break; }
		pKeyWord[j++] = pMes[i];
		if (j>7)	//ָ��̶�Ϊ7������
			return FALSE;
	}//End of for;

	*nPlace = i;
	return TRUE;
}

//�����ô��л�ȡҪ���õ�����ֵ
//char * pValue    ��Ϣ
//int nsize		   pValue �ռ��С
//char * pMes      ������Դ
//int nlen         pMes�����ƫ����
//int *nPlace      ��ȡ��Ϣ���ƫ����
BOOL bGetData(unsigned char *pValue, int nsize, unsigned char *pMes, int nlen, int *nPlace)
{
	int i,j,k,m,n;
	int nMesLen = 0;
	
	//�ҵ������ĵ�1��'='
	for(i = 0; i < nlen; i++)
		if(pMes[i] == '=') break;
	if(i == nlen) return FALSE;

	//�Ⱥź���Ŀո�
	i++;
	while(i < nlen){
		if(pMes[i] != ' ') break;
		else i++;
	}
	if(i == nlen) return FALSE;

	if(pMes[i] == '\"'){//�����ŵ�Ϊ�ַ���ֱ�Ӹ�ֵ
		//���ҵ��ַ����Ľ�β: ���� + �ֺ�
		if(i + 2 >= nlen) return FALSE;
		for(j = i + 1; j < nlen - 1; j++){
			if(pMes[j] == '\"' && pMes[j + 1] == ';') break;
		}
		if(j == (nlen - 1)) return FALSE; //û�ҵ���β
		if ((j - i - 1)> nsize) return FALSE; //����pValue�ռ䳤��

		memcpy(pValue, pMes + i + 1, j - i - 1); //��ȡ�������������
		*nPlace = j + 2;
	}
	else {
		//���ҽ�β���ֺ�
		for(j = i; j < nlen; j++){
			if(pMes[j] == ';') break;
		}
		if(j == nlen) return FALSE; //û�ҵ�

		nMesLen = j - i; //Ҫ��������Ϣ����
		if (nMesLen > nsize) return FALSE; //����pValue�ռ䳤��

		if((i + 2 < nlen) && (pMes[i] == '0') && 
		    ((pMes[i + 1] == 'x') || (pMes[i + 1] == 'X')))
		{   //ʮ������ֵ�ַ�����
			if((nMesLen & 0x01) != 0) return FALSE; //������ż����
			for(k = 0; k < nMesLen - 2; k += 2){
				m = GetHex(pMes[i + k + 2]);
				n = GetHex(pMes[i + k + 3]);
				if((m == -1) || (n == -1)) return FALSE;
				else pValue[k / 2] = ((m << 4) + n);
			}
			*nPlace = j + 1; //��һ�������ƫ��λ��,���ֺź���
		}
		else{//������һ������
			for(k = 0; k < nMesLen; k++){
				if((pMes[i + k] >= 0x30) && (pMes[i + k] <= 0x39)){
					pValue[k] = pMes[i + k];
				}
				else return FALSE;
			}
		    *nPlace = j + 1; //��һ�������ƫ��λ��,���ֺź���
		}
	}

	return TRUE;
}

//�û���������Ҫ���������������ת��
BOOL bUseDataCode(unsigned char *pMes)
{
	int i;
	int nValue = 0;
	
	switch(g_DataType){
	case DataSetType_1DMin://һά�������С��������
    case DataSetType_1DMax://һά�������󳤶�����
	case DataSetType_OCRMin://OCR��С��������
	case DataSetType_OCRMax://OCR��󳤶�����
		nValue = StringToInt(pMes);
		if(nValue == -1 || nValue > 127) return FALSE;
		g_DataSet = (unsigned int)nValue;
   	    break;
	
    case DataSetType_2DMin://��ά�������С��������
    case DataSetType_2DMax://��ά�������󳤶�����
		nValue = StringToInt(pMes);
		if(nValue == -1 || nValue > 65535) return FALSE;
		g_DataSet = (unsigned int)nValue;
		break;

	case DataSetType_Prefix://ǰ׺��Ϣ����
	case DataSetType_Suffix://��׺��Ϣ����
	case DataSetType_ErrorMsg: //һ��ʱ����û�����뷵����Ϣ����
		if(strlen((char *)pMes) > 10) return FALSE;
		g_nPreSuffix = strlen((char *)pMes);
		memcpy(g_PreSuffix, pMes, g_nPreSuffix);
		g_DataSet_Odd = TRUE;
		break;

	case DataSetType_Terminal://��������׺����
		if(strlen((char *)pMes) > 2) return FALSE;
		g_nPreSuffix = strlen((char *)pMes);
		memcpy(g_PreSuffix, pMes, g_nPreSuffix);
		g_DataSet_Odd = TRUE;
		break;

	case DataSetType_FixedLen://����ʶ���̶���������
		if(strlen((char *)pMes) == 1){ //����ֵ
			if(pMes[0] > 64) return FALSE;
			g_PreSuffix[0] = pMes[0];
			g_nFixedLen = 3;
	        g_nPreSuffix = 1;
		}
		else { //���÷�Χ
			if(pMes[0] > 64 || pMes[1] > 64) return FALSE;
			g_PreSuffix[0] = pMes[0];
			g_PreSuffix[1] = pMes[1];
			g_nFixedLen = 6;
	        g_nPreSuffix = 2;
		}
		break;

	case DataSetType_1DCodeID://һά����CodeIDǰ׺����
	case DataSetType_2DCodeID://��ά����CodeIDǰ׺����
	case DataSetType_PostalCodeID://������CodeIDǰ׺����
		i = strlen((char *)pMes);
		if(i > 2) return FALSE;
		memcpy(g_PreSuffix, pMes, i);
		g_nPreSuffix = i;
		g_DataSet_Odd = TRUE;
		break;

	case DataSetType_Cut: //��ȡ����
		if(g_PosMaxMin == 0){	//��ȡ��ʽ����
			if(pMes[0] < 0x20 || pMes[0] == 0x40 || (pMes[0] >= 96 && pMes[0] <= 127)){ //һά�����OCR_B����������
				for(i = 1; i < 16; i += 3){		//һά�����ȡ���5�����ݣ�ÿ��3λ��������������һλ����16λ
					if(pMes[i] > 0x01) break;	//�жϽ�ȡ����λ����0��1
					if(pMes[i + 1] == 0x00 || pMes[i + 2] == 0x00) break;	//��ʼλ����ֹλ����Ϊ0
				}
				nValue = i;
			}
			else if(pMes[0] > 0x1f && pMes[0] < 0x40){ //��ά����
				for(i = 1; i < 16; i += 5){
					if(pMes[i] > 0x01) break;
					if((pMes[i + 1] == 0x00 && pMes[i + 2] == 0x00)
						|| (pMes[i + 3] == 0x00 && pMes[i + 4] == 0x00)) break;
				}
				nValue = i;
			}
			else return FALSE;

			if(nValue > 16 || nValue == 1) return FALSE;
			
			memcpy(g_PreSuffix, pMes, nValue);
			g_nPreSuffix = nValue;
			g_nFixedLen = g_nPreSuffix * 3;
		}
		else if(g_PosMaxMin == 1){
			nValue = StringToInt(pMes);
			if(nValue == -1 || nValue > 0x7f) return FALSE;
			g_DataSet = (unsigned int)nValue;
		}
		break;

	case DataSetType_Fixed_Dis://I2odf5�����ֹʶ���̶���������
		if(strlen((char *)pMes) == 1){ //����ֵ
			if(pMes[0] > 64) return FALSE;
			g_PreSuffix[0] = pMes[0];
			g_nFixedLen = 3;
	        g_nPreSuffix = 1;
		}
		else { //���÷�Χ
			if(pMes[0] > 64 || pMes[1] > 64) return FALSE;
			g_PreSuffix[0] = pMes[0];
			g_PreSuffix[1] = pMes[1];
			g_nFixedLen = 6;
	        g_nPreSuffix = 2;
		}
		break;

	case DataSetType_OnceDelay://һ�ζ����ӳ�����
	case DataSetType_InfoDelay://������ͬ��Ϣ�����ʱ���ӳ�
	case DataSetType_Sensibility: //����������
	case DataSetType_ScanTime: //��������޶�����
	case DataSetType_NonStandar: //�Ǳ��������
	case DataSetType_Oscillation: //����ʱ������
	case DataSetType_ReadBeepFreq: //����ɹ�����Ƶ��
	case DataSetType_ReadBeepDura: //����ɹ��������γ���ʱ��
	case DataSetType_ReadLEDDura:	//����ɹ�LED����ʱ��
		nValue = StringToInt(pMes);
		if(nValue == -1) return FALSE;
		g_DataSet = (unsigned int)nValue;
		break;

	case DataSetType_KBW: //���̲�������
		if(g_PosMaxMin == DataSetKBW_Nationality || g_PosMaxMin == DataSetKBW_CharDelay
		  || g_PosMaxMin == DataSetKBW_CodeDelay || g_PosMaxMin == DataSetKBW_CodePage){
			nValue = StringToInt(pMes);
			if(nValue == -1) return FALSE;
			g_DataSet = (unsigned int)nValue;
		}
		else return FALSE;
		break;
	case DataSetType_BlueT: //������������
		if(g_PosMaxMin == DataSetBlueT_Name){
			if(strlen((char *)pMes) > 0x10) return FALSE; //���Ƴ��Ȳ�����16�ֽ�
			g_nPreSuffix = strlen((char *)pMes);
			memcpy(g_PreSuffix, pMes, g_nPreSuffix);
			g_DataSet_Odd = TRUE;
		}
		else if(g_PosMaxMin == DataSetBlueT_Password){
			if(strlen((char *)pMes) > 6) return FALSE; //���볤��6���ַ�
			g_nPreSuffix = strlen((char *)pMes);
			memcpy(g_PreSuffix, pMes, g_nPreSuffix);
			g_DataSet_Odd = TRUE;
		}
		break;

	case DataSetType_DecodeArea://������������
		nValue = StringToInt(pMes);
		if(nValue == -1 || nValue > 100) return FALSE;
		g_DataSet = (unsigned int)nValue;
   	    break;
	
	case DataSetType_DataFormatting:
		//����Ȳ��ɳ���512�ֽ�
		g_nDataFormatbuf = strlen((char *)pMes);
		if(g_nDataFormatbuf > 512) return FALSE;
		memcpy(g_DataFormatbuf, pMes, g_nDataFormatbuf);
		//g_DataSet_Odd = TRUE;
		break;
	default: return FALSE;
	}

	return TRUE;
}

int GetHex(char c)
{
	if (c >= '0' && c <= '9') return (c-'0');
	else if (c >= 'a' && c <= 'f') return (c-'a') + 10;
	else if (c >= 'A' && c <= 'F') return (c-'A') + 10;
	else return -1;
}
//�������ַ����л�ȡ����
int StringToInt(unsigned char *pMes)
{
	int i;
	int value = 0;
	
	i = 0;
	for(i = 0; i < (int)strlen((char *)pMes); i++){
		if((pMes[i] > 0x39) || (pMes[i] < 0x30)) return -1;
		value = value * 10 + (int)(pMes[i] - 0x30);
	}
	
	return value;
}

//�����빫���������ó�ʼ��
void W_setcParamInit()
{
//	uImg_vSetCodeReResult(U_uIMG_STATUS_SETCODE_END);
	g_DataSet = 0;
	g_DataType = DataSetType_No;
	g_PosMaxMin = 0;
	g_DataSet_Odd = TRUE;
	g_nPreSuffix = 0;
	g_nFixedLen = 0;
	g_IsSetCodeStart = FALSE;

	g_ReplaceType = Replace_No;
	memset(g_PreSuffix, 0, 20);
}

//ȡ�����ã�����ȡ��һ���ַ���һ���ַ���һ������
BOOL SetCode_Discard(int Rec56)
{
	if(Rec56 == POS_Discard1Data){//ȡ��ǰһ�ζ���һλ����
		if(g_DataType == DataSetType_Prefix || g_DataType == DataSetType_Suffix
			|| g_DataType == DataSetType_1DCodeID || g_DataType == DataSetType_2DCodeID || g_DataType == DataSetType_PostalCodeID || g_DataType == DataSetType_DataFormatting)
		{
			if(g_DataSet_Odd && (g_nPreSuffix == 0) && (g_nDataFormatbuf == 0))return TRUE;
			if(g_DataSet_Odd) {
				g_DataSet_Odd = FALSE;
				g_nPreSuffix--;
				g_DataSet = g_PreSuffix[g_nPreSuffix] >> 4;
				g_nDataFormatbuf--;
				

			}
			else {
				g_DataSet_Odd = TRUE;
				g_DataSet = g_DataSet >> 4;
			}
		}
		else if(g_DataType == DataSetType_FixedLen || g_DataType == DataSetType_Cut)
		{   //����̶��������û���Ϣ��ȡ����
			if(g_nFixedLen == 0) return TRUE;
			if(g_nFixedLen % 3 == 0) {
				g_nPreSuffix--;
				g_DataSet = g_PreSuffix[g_nPreSuffix] / 10;
			}
			else {
				g_DataSet /= 10;
			}
			if(g_nFixedLen > 0) g_nFixedLen--;
		}
		else g_DataSet /= 10;
	}
	else if(Rec56 == POS_DiscardAllData){//ȡ��ǰ�����һ������
		g_DataSet = 0;
		g_DataSet_Odd = TRUE;
		g_nPreSuffix = 0;
		g_nFixedLen = 0;
		g_nDataFormatbuf = 0;
		memset(g_PreSuffix, 0, 20);
		memset(g_DataFormatbuf, 0, 512);
	}
	else if(Rec56 == POS_DiscardSet){
		//ȡ����ǰ�����С�����ͼ�����Ȼ�ǰ��׺����
		W_setcParamInit();
	}
	else return FALSE;

	return TRUE;
}

//ǰ��׺��������
BOOL SetCode_Data_PreSuffix(int Rec56)
{
	if(g_nPreSuffix >= Max_nPreSubfix && g_DataType != DataSetType_BlueT)
		return FALSE; //ǰ��׺��Ϣ����10���ֽ�
	if(g_nPreSuffix > 1 && (g_DataType == DataSetType_1DCodeID 
		|| g_DataType == DataSetType_2DCodeID || g_DataType == DataSetType_PostalCodeID)) 
		return FALSE; //CodeIDǰ׺�ĳ�������2���ֽ�
	
	if((g_nPreSuffix > 1) && (g_DataType == DataSetType_Terminal)) 
		return FALSE; //��ֹ����������2���ֽ�

	if(g_DataSet_Odd) {
		g_DataSet = Rec56;
		g_DataSet_Odd = FALSE;
	}
	else{
		g_DataSet = (g_DataSet << 4) + Rec56; //ǰ��׺��Ϣ���ݲ���16����
		g_PreSuffix[g_nPreSuffix] = g_DataSet;
		g_nPreSuffix++;
		g_DataSet_Odd = TRUE;
	}
	return TRUE;
}

//����̶���������
BOOL SetCode_Data_FixedLen(int Rec56)
{
	//�������ݹ���һ���̶�����
	if(g_nPreSuffix > Max_FixedLen && (g_DataType == DataSetType_FixedLen 
		|| g_DataType == DataSetType_Fixed_Dis))	return FALSE;
	if(g_nPreSuffix > Max_DataCutLen && g_DataType == DataSetType_Cut) return FALSE;
	if((g_nFixedLen / 3 * 3) == g_nFixedLen) {
		g_DataSet = Rec56;
		g_nFixedLen++;
	}
	else if((g_nFixedLen / 3 * 3) + 1 == g_nFixedLen){
		g_DataSet = g_DataSet * 10 + Rec56;
		g_nFixedLen++;
	}
	else{
		g_DataSet = g_DataSet * 10 + Rec56;
		if ((g_DataSet > 0x7f) && (g_DataType != DataSetType_Cut)) return FALSE; //һά�������������ϢΪ127
		g_PreSuffix[g_nPreSuffix] = g_DataSet;
		g_nPreSuffix++;
		g_nFixedLen++;
	}
	return TRUE;
}
BOOL SetCode_Data_DataFormat(int Rec56)
{	
	char c;
	static int len = HEADLEN;

//	char buf[16];
//	int length;

	if (g_nDataFormatbuf < 2)
	{
		g_DataSet = (0x30 + Rec56);
		g_DataFormatbuf[g_nDataFormatbuf] = g_DataSet;
		g_nDataFormatbuf++;

		if ((g_nDataFormatbuf == 2) && (g_DataFormatbuf[1] == 0x33))
		{
			len -= 1;
		}
	}
	else if (g_nDataFormatbuf<len)//dataforamt head
	{
		g_DataSet = (0x30 + Rec56);
		g_DataFormatbuf[g_nDataFormatbuf] = g_DataSet;
		g_nDataFormatbuf++;
	}
	else
	{
		len = HEADLEN;//formatͷ�����꣬len�ָ���ֵ����֤��һ������dataformatʱlen����12
		c = int_to_char(Rec56);
		if (c == 0) return FALSE;
		g_DataFormatbuf[g_nDataFormatbuf] = c;//int_to_char(Rec56);
		g_nDataFormatbuf++;
	}
	return TRUE;
}
//����һά������С��������
BOOL SetCode_Save_1DMin()
{
// 	int nMaxlen;
	if((g_DataSet > 0x7f)||(g_DataSet < 1)) return FALSE; //һά���볤�Ȳ�����127,��С��1

// 	nMaxlen = (X_paramGetGlobalAllParam()->p1D->ExMsg[g_PosMaxMin] >> 24) & 0xff;
// 	if (g_DataSet > nMaxlen) return FALSE;	//��Сֵ>���ֵ

	switch(g_PosMaxMin){
	case POS_CODE128:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_UCCEAN128:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_REGULAR39:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] & 0xff00ffff) | (g_DataSet<<16);
	    break;
	case POS_INTER2OF5:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_MATRIX25:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_MODULE25:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_CODABAR:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_CODE93:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_CODEZ:
        X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] & 0xff00ffff) | (g_DataSet<<16);
	    break;
	case POS_CODE11:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_INDUSTRIAL25:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_STANDARD25:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_PLESSEY:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_MSI_PLESSEY:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] & 0xff00ffff) | (g_DataSet<<16);
		break;
	case POS_CHNPOST:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] & 0xff00ffff) | (g_DataSet<<16);
		break;

	default:
		return FALSE;
		break;
	}
	return TRUE;
}

//����һά��������������
BOOL SetCode_Save_1DMax()
{
// 	int nMinlen;
	if((g_DataSet > 0x7f)||(g_DataSet < 1)) return FALSE; //һά���볤�Ȳ�����127
	
// 	nMinlen = (X_paramGetGlobalAllParam()->p1D->ExMsg[g_PosMaxMin] >> 16) & 0xff;
// 	if (g_DataSet < nMinlen) return FALSE;	//���ֵ<��Сֵ

	switch(g_PosMaxMin){
	case POS_CODE128:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_UCCEAN128:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_REGULAR39:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_INTER2OF5:
	    X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_MATRIX25:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_MODULE25:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_CODABAR:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_CODE93:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_CODEZ:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_CODE11:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_INDUSTRIAL25:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_STANDARD25:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_PLESSEY:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_MSI_PLESSEY:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] & 0x00ffffff) | (g_DataSet<<24);
		break;
	case POS_CHNPOST:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] & 0x00ffffff) | (g_DataSet<<24);
		break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
}

//�����ά������С��������
BOOL SetCode_Save_2DMin()
{
	int nMaxlen;
	if((g_DataSet > 0xffff)||(g_DataSet < 1)) return FALSE; //��ά���볤�Ȳ�����65535

	nMaxlen = (X_paramGetGlobalAllParam()->p2D->ExMsg[g_PosMaxMin] >> 16) & 0xffff;
	if (g_DataSet > nMaxlen) return FALSE;	//��Сֵ>���ֵ

	switch(g_PosMaxMin){
	case POS_PDF417: //PDF417
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] & 0xffff0000) | g_DataSet;	
		break;
	case POS_QR://QR
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_QR] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] & 0xffff0000) | g_DataSet;		
		break;
	case POS_Aztec: //Aztec
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] & 0xffff0000) | g_DataSet;
		break;
	case POS_DataMatrix: //DataMatrix
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] & 0xffff0000) | g_DataSet;
		break;
	case POS_MaxiCode: //MaxiCode
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] & 0xffff0000) | g_DataSet;
		break;
	case POS_VeriCode: //VeriCode
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] & 0xffff0000) | g_DataSet;
		break;
	case POS_HAN_SENSIBLE: //������
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] & 0xffff0000) | g_DataSet;
		break;
	case POS_GM_CODE:
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_GM_CODE] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_GM_CODE] & 0xffff0000) | g_DataSet;
		break;
	case POS_MicroPDF417:
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_MicroPDF417] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MicroPDF417] & 0xffff0000) | g_DataSet;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//�����ά��������������
BOOL SetCode_Save_2DMax()
{
	int nMinlen;
	if((g_DataSet > 0xffff)||(g_DataSet < 1)) return FALSE; //��ά���볤�Ȳ�����65535

	nMinlen = X_paramGetGlobalAllParam()->p2D->ExMsg[g_PosMaxMin] & 0xffff;
	if (g_DataSet < nMinlen) return FALSE;	//���ֵ<��Сֵ

	switch(g_PosMaxMin){
	case POS_PDF417: //PDF417
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] & 0x0000ffff) | (g_DataSet << 16);
		break;
	case POS_QR: //QR
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_QR] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_QR] & 0x0000ffff) | (g_DataSet << 16);
		break;
	case POS_Aztec: //Aztec
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] & 0x0000ffff) | (g_DataSet << 16);
		break;
	case POS_DataMatrix://DataMatrix
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] & 0x0000ffff) | (g_DataSet << 16);
		break;
	case POS_MaxiCode: //MaxiCode
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] & 0x0000ffff) | (g_DataSet << 16);
		break;
	case POS_VeriCode: //VeriCode
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] & 0x0000ffff) | (g_DataSet << 16);
		break;
	case POS_HAN_SENSIBLE: //������
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] & 0x0000ffff) | (g_DataSet << 16);
		break;
	case POS_GM_CODE:
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_GM_CODE] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_GM_CODE] & 0x0000ffff) | (g_DataSet << 16);
		break;
	case POS_MicroPDF417:
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_MicroPDF417] = (X_paramGetGlobalAllParam()->p2D->ExMsg[POS_MicroPDF417] & 0x0000ffff) | (g_DataSet << 16);
		break;
	default: return FALSE;
	}
	return TRUE;
}

//����OCR��С��������
BOOL SetCode_Save_OCRMin()
{
	int nMaxlen;
	if(g_DataSet < 1) return FALSE; 
	nMaxlen = (X_paramGetGlobalAllParam()->pOCR->ExMsg[g_PosMaxMin] >> 24) & 0xff;
	if (g_DataSet > nMaxlen) return FALSE;	//��Сֵ>���ֵ

	switch(g_PosMaxMin){
	case POS_SPEC_OCR_B: //SPEC_OCR_B
	    X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] & 0xff00ffff) | (g_DataSet<<16);
		break;	
	default: return FALSE;
	}
	return TRUE;
}

//����OCR��󳤶�����
BOOL SetCode_Save_OCRMax()
{
	int nMinlen;
	if(g_DataSet < 1) return FALSE; 
	nMinlen = (X_paramGetGlobalAllParam()->pOCR->ExMsg[g_PosMaxMin] >> 16) & 0xff;
	if (g_DataSet < nMinlen) return FALSE;	//���ֵ<��Сֵ

	switch(g_PosMaxMin){
	case POS_SPEC_OCR_B: //SPEC_OCR_B
	    X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] & 0x00ffffff) | (g_DataSet<<24);
		break;	
	default: return FALSE;
	}
	return TRUE;
}

//��������
BOOL SetCode_Save()
{
	int i;
	unsigned char *pbyCut;

      DF_Param	dfparam;
      int flag;

	switch(g_DataType)
	{
	case DataSetType_No: //�û����������󱣴�
		if(g_ReplaceType == Replace_UserPart)
		{
			W_setcSetSaveFlag(TRUE);
			return TRUE;
		}//End of if;
		break;
	case DataSetType_1DMin://һά�������С��������
		if(!SetCode_Save_1DMin()) return FALSE;
	    break;
    case DataSetType_1DMax://һά�������󳤶�����
		if(!SetCode_Save_1DMax()) return FALSE;
   	    break;
    case DataSetType_2DMin://��ά�������С��������
		if(!SetCode_Save_2DMin()) return FALSE;
	    break;
    case DataSetType_2DMax://��ά�������󳤶�����
	    if(!SetCode_Save_2DMax()) return FALSE;
		break;
	case DataSetType_OnceDelay://һ�ζ����ӳ�����
		if(g_DataSet > Max_Delay || g_DataSet < 500) return FALSE;
		X_paramBakGetGlobalAllParam()->pCode->nOnceDelay = g_DataSet;
		break;
	case DataSetType_InfoDelay://������ͬ��Ϣ�����ʱ���ӳ�
		if(g_DataSet > Max_Delay) return FALSE;
		X_paramBakGetGlobalAllParam()->pCode->nInfoDelay = g_DataSet;
		break;

	case DataSetType_Prefix://ǰ׺��Ϣ����
		if(!g_DataSet_Odd)
		{ return FALSE; }
		memcpy(X_paramBakGetGlobalAllParam()->pCode->byPrefix, g_PreSuffix, 10);
		memset(g_PreSuffix, 0, 20);
		X_paramBakGetGlobalAllParam()->pCode->nPrefix = g_nPreSuffix;
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		break;

	case DataSetType_Suffix://��׺��Ϣ����
		if(!g_DataSet_Odd)
		{ return FALSE; }
		memcpy(X_paramBakGetGlobalAllParam()->pCode->bySuffix, g_PreSuffix, 10);
		memset(g_PreSuffix, 0, 20);
		X_paramBakGetGlobalAllParam()->pCode->nSuffix = g_nPreSuffix;
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		break;

	case DataSetType_Terminal://��������׺����
		if(!g_DataSet_Odd)
		{ return FALSE; }
		if (g_nPreSuffix > 2)
		{ return FALSE; }
		memcpy(X_paramBakGetGlobalAllParam()->pCode->byTerminal, g_PreSuffix, 2);
		memset(g_PreSuffix, 0, 20);
		X_paramBakGetGlobalAllParam()->pCode->nTerminal = g_nPreSuffix;
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		break;

	case DataSetType_FixedLen://����ʶ���̶���������
		if((g_nFixedLen / 3 * 3) != g_nFixedLen)
		{ return FALSE; }
		if(g_nPreSuffix == 0)
		{ return FALSE; }
		
		if(g_PosMaxMin == POS_INTER2OF5)
		{
			if(g_PreSuffix[0] > 64 || g_PreSuffix[0] == 0)
			{ return FALSE; }
			if(g_PreSuffix[0] & 0x01)
			{ g_PreSuffix[0]++; }
			if(g_nPreSuffix == 1)
			{//ֻ���һ���̶�ֵ
				if(g_PreSuffix[0] < 18)
				{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[1] |= (0x01 << (g_PreSuffix[0] / 2 - 1)); }
				else if(g_PreSuffix[0] < 34)
				{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[2] |= (0x01 << (g_PreSuffix[0] / 2 - 9)); }
				else if(g_PreSuffix[0] < 50)
				{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[3] |= (0x01 << (g_PreSuffix[0] / 2 - 17)); }
				else
				{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[4] |= (0x01 << (g_PreSuffix[0] / 2 - 25)); }
			}//End of if;
			else if(g_nPreSuffix == 2)
			{ //���һ�ι̶�ֵ
				if((g_PreSuffix[1] > 64) || (g_PreSuffix[0] > g_PreSuffix[1]) || (g_PreSuffix[1] == 0)) 
				{ return FALSE; }
				if(g_PreSuffix[1] & 0x01)
				{ g_PreSuffix[1]--; }

				for(i = g_PreSuffix[0]; i <= g_PreSuffix[1]; i+=2)
				{
					if(i < 18)
					{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[1] |= (0x01 << (i / 2 - 1)); }
				    else if(i < 34)
					{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[2] |= (0x01 << (i / 2 - 9)); }
				    else if(i < 50)
					{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[3] |= (0x01 << (i / 2 - 17)); }
				    else
					{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[4] |= (0x01 << (i / 2 - 25)); }
				}//End of for;
			}//End of else if;
			else
			{ return FALSE; }

			memset(g_PreSuffix, 0, 20);
			g_nPreSuffix = 0;
			g_nFixedLen = 0;
		}//End of if;
		else
		{ return FALSE; }
		break;
	case DataSetType_OCRMin://OCR��С��������
	    if(!SetCode_Save_OCRMin()) 
		{ return FALSE; }
		break;
	case DataSetType_OCRMax://OCR��󳤶�����
	    if(!SetCode_Save_OCRMax())
		{ return FALSE; }
		break;
	case DataSetType_1DCodeID://һά����CodeIDǰ׺����
		if(!g_DataSet_Odd)
		{ return FALSE; }
		//��������ַ��Ƿ�Ϊ��ĸ
		if((g_PreSuffix[0] > 0x40 && g_PreSuffix[0] < 0x5b) 
			|| (g_PreSuffix[0] > 0x60 && g_PreSuffix[0] < 0x7b))
		{ X_paramBakGetGlobalAllParam()->pCode->by1DCodeID[g_PosMaxMin] = g_PreSuffix[0]; }
		else
		{ return FALSE; }
		if (g_nPreSuffix == 2)
		{
			if((g_PreSuffix[1] > 0x40 && g_PreSuffix[1] < 0x5b) 
				|| (g_PreSuffix[1] > 0x60 && g_PreSuffix[1] < 0x7b))
			{ X_paramBakGetGlobalAllParam()->pCode2->by1DCodeID_2[g_PosMaxMin] = g_PreSuffix[1]; }
			else
			{ return FALSE; }
		}
		else
		{
			X_paramBakGetGlobalAllParam()->pCode2->by1DCodeID_2[g_PosMaxMin] = 0;
		}
		memset(g_PreSuffix, 0, 20);
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		g_PosMaxMin = 0;
		g_DataSet = 0;
		break;
	case DataSetType_2DCodeID://��ά����CodeIDǰ׺����
		if(!g_DataSet_Odd)
		{ return FALSE; }
		//��������ַ��Ƿ�Ϊ��ĸ
		if((g_PreSuffix[0] > 0x40 && g_PreSuffix[0] < 0x5b)
			|| (g_PreSuffix[0] > 0x60 && g_PreSuffix[0] < 0x7b))
		{ X_paramBakGetGlobalAllParam()->pCode->by2DCodeID[g_PosMaxMin] = g_PreSuffix[0]; }
		else
		{ return FALSE; }
		if (g_nPreSuffix == 2)
		{
			if((g_PreSuffix[1] > 0x40 && g_PreSuffix[1] < 0x5b) 
				|| (g_PreSuffix[1] > 0x60 && g_PreSuffix[1] < 0x7b))
			{ X_paramBakGetGlobalAllParam()->pCode2->by2DCodeID_2[g_PosMaxMin] = g_PreSuffix[1]; }
			else
			{ return FALSE; }
		}
		else
		{
			X_paramBakGetGlobalAllParam()->pCode2->by2DCodeID_2[g_PosMaxMin] = 0;
		}
		memset(g_PreSuffix, 0, 20);
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		g_PosMaxMin = 0;
		g_DataSet = 0;
		break;

	case DataSetType_PostalCodeID://������CodeIDǰ׺����
		if(!g_DataSet_Odd)
		{ return FALSE; }
// 		if((g_PreSuffix[0] > 0x40 && g_PreSuffix[0] < 0x5b)
// 			|| (g_PreSuffix[0] > 0x60 && g_PreSuffix[0] < 0x7b))
		if (g_PreSuffix[0] != 0)
		{ X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID[g_PosMaxMin] = g_PreSuffix[0];	}
		else
		{ return FALSE; }
		if (g_nPreSuffix == 2)
		{
// 			if((g_PreSuffix[1] > 0x40 && g_PreSuffix[1] < 0x5b) 
// 				|| (g_PreSuffix[1] > 0x60 && g_PreSuffix[1] < 0x7b))
			if (g_PreSuffix[1] != 0)
			{ X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID_2[g_PosMaxMin] = g_PreSuffix[1]; }
			else
			{ return FALSE; }
		}
		else
		{
			X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID_2[g_PosMaxMin] = 0;
		}
		memset(g_PreSuffix, 0, 20);
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		g_PosMaxMin = 0;
		g_DataSet = 0;
		break;

	case DataSetType_Cut: //��ȡ����
		pbyCut = X_paramBakGetGlobalAllParam()->pCode->byCut;
		if(g_PosMaxMin == 1){ //ȡ��ĳ������Ľ�ȡ��ʽ
			if(g_DataSet == (unsigned int)pbyCut[0]){
				memset(pbyCut, 0, Max_DataCutLen);
			}
			else if(g_DataSet == (unsigned int)pbyCut[Max_DataCutLen]){
				memcpy(pbyCut + Max_DataCutLen, pbyCut, Max_DataCutLen);
				memset(pbyCut, 0, Max_DataCutLen);
			}
			else if(g_DataSet == (unsigned int)pbyCut[(Max_DataCutLen << 1)]){
				memcpy(pbyCut + (Max_DataCutLen << 1), pbyCut + Max_DataCutLen, Max_DataCutLen);
				memcpy(pbyCut + Max_DataCutLen,	pbyCut, Max_DataCutLen);
				memset(pbyCut, 0, Max_DataCutLen);
			}
			break;
		}

		if(g_nPreSuffix > 16)
			return FALSE;

		if((g_nFixedLen / 3 * 3) != g_nFixedLen)
		{ return FALSE;}
		//0~31:һά���룬32~63:��ά���룬64:OCR_B
		if(g_PreSuffix[0] <= 0x7f){
			if(g_PreSuffix[0] < 0x20 || (g_PreSuffix[0] >= 64 && g_PreSuffix[0] <= 127)){//һά�� OCR ������
				if((g_nPreSuffix - 1) / 3 * 3 != (g_nPreSuffix - 1))
				{ return FALSE; }
			 	//����ֻ��0�����1��������
			    if(g_PreSuffix[1] > 1 || g_PreSuffix[4] > 1 || g_PreSuffix[7] > 1
			    || g_PreSuffix[10] > 1 || g_PreSuffix[13] > 1 || g_PreSuffix[16] > 1)
				{ return FALSE; }

				//λ�ñ���������, ��С��128
				for(i = 2; i < g_nPreSuffix; i += 3)
				{
					if((g_PreSuffix[i] < 1) || (g_PreSuffix[i] > 127) || (g_PreSuffix[i+1] < 1) || (g_PreSuffix[i+1] > 127))
					{ return FALSE; }
				}//End of for;
			}//End of if;

			if(g_PreSuffix[0] > 31 && g_PreSuffix[0] < 64)
			{
				if((g_nPreSuffix - 1) / 5 * 5 != (g_nPreSuffix - 1))
				{ return FALSE; }
			    
				//����ֻ��0�����1��������
			    if(g_PreSuffix[1] > 1 || g_PreSuffix[6] > 1 || g_PreSuffix[11] > 1)
				{ return FALSE; }
				
				//λ�ñ���������, ��С��10000
				for(i = 2; i < g_nPreSuffix; i += 5)
				{
					if((g_PreSuffix[i] * 10 + g_PreSuffix[i + 1] < 1) 
						|| (g_PreSuffix[i] * 10 + g_PreSuffix[i + 1] > 9999) 
						|| (g_PreSuffix[i + 2] * 10 + g_PreSuffix[i + 3] < 1)
						|| (g_PreSuffix[i + 2] * 10 + g_PreSuffix[i + 3] > 9999)) 
					{ return FALSE; }
				}//End of for;
			}//End of if;

		    if(pbyCut[(Max_DataCutLen << 1)] == g_PreSuffix[0]){//���µ�һ���ǵ�ǰ������
			}
			else if(pbyCut[Max_DataCutLen] == g_PreSuffix[0]){//��2���ǵ�ǰ������
				memcpy(pbyCut + Max_DataCutLen, pbyCut + (Max_DataCutLen << 1), Max_DataCutLen);
			}
			else{
				//��������������һ�����ݽ���ɾ��
				memcpy(pbyCut, pbyCut + Max_DataCutLen, Max_DataCutLen);
				memcpy(pbyCut + Max_DataCutLen, pbyCut + (Max_DataCutLen << 1), Max_DataCutLen);
			}
			memcpy(pbyCut + (Max_DataCutLen << 1) + 2, g_PreSuffix + 1, Max_DataCutLen - 2);
			
			pbyCut[Max_DataCutLen << 1] = g_PreSuffix[0]; //�洢���õ�����
			if(g_PreSuffix[0] < 0x20 || (g_PreSuffix[0] >= 96 && g_PreSuffix[0] <= 127))
				pbyCut[(Max_DataCutLen << 1) + 1] = (g_nPreSuffix - 1) / 3; //�洢��ȡ�Ķ���
			else if(g_PreSuffix[0] < 0x40)
				pbyCut[(Max_DataCutLen << 1)+ 1] = (g_nPreSuffix - 1) / 5; //�洢��ȡ�Ķ���

			memset(g_PreSuffix, 0, 20);
			g_nPreSuffix = 0;
			g_nFixedLen = 0;
		}//End of if;
		else
		{ return FALSE; }
		break;

	case DataSetType_Fixed_Dis://I2odf5�����ֹʶ���̶���������
		if((g_nFixedLen / 3 * 3) != g_nFixedLen)
		{ return FALSE; }
		if(g_nPreSuffix == 0)
		{ return FALSE; }
		
		if(g_PosMaxMin == POS_INTER2OF5)
		{
			if(g_PreSuffix[0] > 64 || g_PreSuffix[0] == 0)
			{ return FALSE; }
			if(g_PreSuffix[0] & 0x01)
			{ g_PreSuffix[0]++; }
			if(g_nPreSuffix == 1)
			{//ֻ��ֹһ���̶�ֵ
				if(g_PreSuffix[0] < 18)
				{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[1] &= ~(0x01 << (g_PreSuffix[0] / 2 - 1)); }
				else if(g_PreSuffix[0] < 34)
				{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[2] &= ~(0x01 << (g_PreSuffix[0] / 2 - 9)); }
				else if(g_PreSuffix[0] < 50)
				{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[3] &= ~(0x01 << (g_PreSuffix[0] / 2 - 17)); }
				else
				{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[4] &= ~(0x01 << (g_PreSuffix[0] / 2 - 25)); }
			}//End of if;
			else if(g_nPreSuffix == 2)
			{ //��ֹһ�ι̶�ֵ
				if((g_PreSuffix[1] > 64) || (g_PreSuffix[0] > g_PreSuffix[1]) || (g_PreSuffix[1] == 0)) 
				{ return FALSE; }
				if(g_PreSuffix[1] & 0x01)
				{ g_PreSuffix[1]--; }
				for(i = g_PreSuffix[0]; i <= g_PreSuffix[1]; i+=2)
				{
					if(i < 18)
					{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[1] &= ~(0x01 << (i / 2 - 1)); }
				    else if(i < 34)
					{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[2] &= ~(0x01 << (i / 2 - 9)); }
				    else if(i < 50)
					{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[3] &= ~(0x01 << (i / 2 - 17)); }
				    else
					{ X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[4] &= ~(0x01 << (i / 2 - 25)); }
				}//End of for;
			}//End of elseif;
			else
			{ return FALSE; }

			memset(g_PreSuffix, 0, 20);
			g_nPreSuffix = 0;
			g_nFixedLen = 0;
		}//End of if;
		else
		{ return FALSE; }
		break;
	case DataSetType_Sensibility: //������ֵ����
		if(g_DataSet > 20 || g_DataSet < 1)
		{ return FALSE; }//���������50
		X_paramBakGetGlobalAllParam()->pCode->nSensitive = g_DataSet;
		break;
	case DataSetType_ScanTime: //��������޶�����
		if(g_DataSet > 65535)
		{ return FALSE; }
		X_paramBakGetGlobalAllParam()->pCode->nScanTimes = g_DataSet;
		break;
	case DataSetType_NonStandar: //�Ǳ����ֵ����
		if(!SetCode_Save_NonStandar())
		{ return FALSE; }
		break;
	case DataSetType_ErrorMsg: //һ��ʱ����û�����뷵����Ϣ����
		if(!g_DataSet_Odd)
		{ return FALSE; }
		memcpy(X_paramBakGetGlobalAllParam()->pCode->byErrMsg, g_PreSuffix, 10);
		memset(g_PreSuffix, 0, 20);
		X_paramBakGetGlobalAllParam()->pCode->nErrMsg = g_nPreSuffix;
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		break;
	case DataSetType_Oscillation: //��������
		if(g_DataSet > 20000)
		{ return FALSE; }//����ʱ�����20000
		X_paramBakGetGlobalAllParam()->pHard->nOscillationDelay = g_DataSet;
		break;
	case DataSetType_KBW: //KBW��������
		if(!SetCode_Save_KBW())
		{ return FALSE; }
		break;
	case DataSetType_BlueT: //������������
		//������������
		if(!SetCode_Save_BlueT()) 
		{ return FALSE; }
		break;

	case DataSetType_DecodeArea: //������������
		if(g_DataSet > 100 || g_DataSet < 1)
		{ return FALSE; }
		X_paramBakGetGlobalAllParam()->pCode->decodeArea = (g_DataSet << 4) + (X_paramGetGlobalAllParam()->pCode->decodeArea&0xf);
		break;

	case DataSetType_ReadBeepFreq://�Զ������ɹ�����Ƶ��
		if(g_DataSet < 20 || g_DataSet > 20000) return FALSE;
		X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0xf000;
		X_paramBakGetGlobalAllParam()->pHard->nBeepType |= (9 << 12);	//��Ƶ����beeptype�е�mask��Ϊ9,����ʶ�����Զ������
		X_paramBakGetGlobalAllParam()->pCode2->nSucBeepFreq = g_DataSet;
		break;
	case DataSetType_ReadBeepDura://�Զ������ɹ����η�������ʱ��
		if(g_DataSet < 20 || g_DataSet > 300) return FALSE;
		X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0xf00;
		X_paramBakGetGlobalAllParam()->pHard->nBeepType |= (9 << 8);   //������ʱ����beeptype�е�mask��Ϊ9,����ʶ�����Զ������
		X_paramBakGetGlobalAllParam()->pCode2->nSucBeepDuration = g_DataSet;
		break;
		
	case DataSetType_ReadLEDDura://�Զ������ɹ�LED����ʱ��
		if(g_DataSet < 1 || g_DataSet > 10000) return FALSE;
		X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 4;
		X_paramBakGetGlobalAllParam()->pCode2->nSucLedDuration = g_DataSet;
		break;

	case DataSetType_DataFormatting:
		i = g_DataFormatbuf[0]-0x30;

		g_nDataFormatbuf = 0;
		if (g_PosMaxMin == 1)//���ĳ��dataformat
		{
			
			if ((i>=0) && (i<4))
			{
				memset(X_paramBakGetGlobalAllParam()->pDFormat->format[i].Cmd,0,Command_Num);
				X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatID = 0;
				X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatType = 0;
				X_paramBakGetGlobalAllParam()->pDFormat->format[i].TerID = 0;
				X_paramBakGetGlobalAllParam()->pDFormat->format[i].SymbolID = 0;
				X_paramBakGetGlobalAllParam()->pDFormat->format[i].MsgLen = 0;
				break;
			}
			return FALSE;
		}
		if ((i>=0) && (i<4))
		{

			dfparam.nDFSelType =g_DataFormatbuf[1];

			if (dfparam.nDFSelType == 0x33)
			{
				dfparam.pDFCmd = g_DataFormatbuf+HEADLEN-1;
				dfparam.nDFMsgLen = strlen((char*)(g_DataFormatbuf+HEADLEN-1));
			} 
			else
			{
				dfparam.pDFCmd = g_DataFormatbuf+HEADLEN;
				dfparam.nDFMsgLen = strlen((char*)(g_DataFormatbuf+HEADLEN));
			}
			
			dfparam.nDFSelType = 3;//������ֻ������������λ1��2��3			
			
			flag = GXDataFormat_IsRightCmd(&dfparam);

			if (flag < 0)
			{
				memset(g_DataFormatbuf, 0, Command_Num);
				return FALSE;
			}
			
			X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatID = i;
			X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatType = g_DataFormatbuf[1] - 0x30;
			X_paramBakGetGlobalAllParam()->pDFormat->format[i].TerID = (g_DataFormatbuf[2] - 0x30)*100+(g_DataFormatbuf[3] - 0x30)*10+(g_DataFormatbuf[4] - 0x30);
			memset((X_paramBakGetGlobalAllParam()->pDFormat->format[i].Cmd),'\0',Command_Num+1);
			if (g_DataFormatbuf[1] == 0x33)
			{		
				X_paramBakGetGlobalAllParam()->pDFormat->format[i].SymbolID = (g_DataFormatbuf[5] - 0x30)*10+(g_DataFormatbuf[6] - 0x30);
				X_paramBakGetGlobalAllParam()->pDFormat->format[i].MsgLen = (g_DataFormatbuf[7] - 0x30)*1000+(g_DataFormatbuf[8] - 0x30)*100+(g_DataFormatbuf[9] - 0x30)*10+(g_DataFormatbuf[10] - 0x30);
				memcpy((X_paramBakGetGlobalAllParam()->pDFormat->format[i].Cmd), g_DataFormatbuf+HEADLEN-1, strlen((char*)(g_DataFormatbuf+HEADLEN-1)));
			} 
			else
			{
				X_paramBakGetGlobalAllParam()->pDFormat->format[i].SymbolID = (g_DataFormatbuf[5] - 0x30)*100+(g_DataFormatbuf[6] - 0x30)*10+(g_DataFormatbuf[7] - 0x30);
				X_paramBakGetGlobalAllParam()->pDFormat->format[i].MsgLen = (g_DataFormatbuf[8] - 0x30)*1000+(g_DataFormatbuf[9] - 0x30)*100+(g_DataFormatbuf[10] - 0x30)*10+(g_DataFormatbuf[11] - 0x30);
				memcpy((X_paramBakGetGlobalAllParam()->pDFormat->format[i].Cmd), g_DataFormatbuf+HEADLEN, strlen((char*)(g_DataFormatbuf+HEADLEN)));
			}
		
			memset(g_DataFormatbuf, 0, Command_Num);
		break;
		}
		if (g_DataFormatbuf[0] == 0)//ȡ��ǰ���ȡ��һ�����ݣ��˴�����return false�����򣬱��汨������һ�δ��ڷ����������Ƿ���0x15
		{
			break;
		}
		return FALSE;
	default:
		return FALSE;
	}//End of switch;

	if(g_ReplaceType != Replace_UserPart)
	{ W_setcSetSaveFlag(TRUE); }

	g_DataSet = 0;
	g_DataType = DataSetType_No;
	g_PosMaxMin = 0;
	g_DataSet_Odd = TRUE;
	g_nPreSuffix = 0;
	g_nFixedLen = 0;
	return TRUE;
}//End of function;

//�Ǳ����ֵ����
BOOL SetCode_Save_NonStandar(void)
{
	switch(g_PosMaxMin) {
	case POS_QR:
		if(g_DataSet > 65535) return FALSE;
		X_paramBakGetGlobalAllParam()->p2D->nQRParam = g_DataSet;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//�����ۺ�����
BOOL SetCode_Total(int Rec56, int Rec7)
{
	W_setcParamInit();
	switch(Rec56){
	case TotalSet_All_Default://�����������ó���Ĭ��ֵ
		if(!X_paramLoadFactoryParam()) {
			//�޳������õ���� 
			if(!X_paramOriginalInitAllParam()) return FALSE;
		}
	//	if(!X_paramSaveAsSysDefaultParam()) return FALSE;
		break;
	case TotalSet_All_Off://��ֹ����������
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = 0x01; //ֻʶ��������
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = 0;
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = 0;
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes = 0;
		break;
	case TotalSet_All_On://��������ɶ�
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = 0xffffffff;
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = 0xffffffff;
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = 0xffffffff;
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes = 0xffffffff;
		break;
	case TotalSet_All_1D_Off://��ֹ������һά����
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = 0x01; //ֻʶ��������
		break;
	case TotalSet_All_1D_On://����һά����ɶ�
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = 0xffffffff;
		break;
	case TotalSet_All_2D_Off://��ֹ�����ж�ά����
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = 0;
		break;
	case TotalSet_All_2D_On://���ж�ά����ɶ�
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = 0xffffffff;
		break;
	case TotalSet_All_Postal_Off: //��ֹ������������
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = 0;
		break;
	case TotalSet_All_Postal_On: //����������ɶ�
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = 0xffffffff;
		break;
	case TotalSet_All_1D_OnlyOne: //һά����ֻ������
		X_paramBakGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
		break;
	case TotalSet_All_1D_OneOrTwo: //�ɶ�˫һά�루ͬһ���ͣ�����Ҳ���Զ���һά��
		X_paramBakGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->unDecParam |= 0x02; 
		break;
	case TotalSet_All_1D_OnlyTwo: //�ɶ�˫һά�루ͬһ���ͣ�����ֻ��˫һά��
		X_paramBakGetGlobalAllParam()->p1D->unDecParam |= 0x03;
		break;
	case TotalSet_UserDef_On: //�����û��Զ���Ĭ������
		g_ReplaceType = Replace_UserDef;
		break;
	case TotalSet_UserDefPart_On: //�����û��Զ���Ĭ������
		g_ReplaceType = Replace_UserPart;
		break;
	case TotalSet_SN_On: //������ƷSN����
		if(Rec7 == 0)
			g_ReplaceType = Replace_SN;
		else if(Rec7 == 1){
			X_paramPlaceKeyword((char*)"S/N", (char*)"", 1);
			X_paramSaveDeviceInfo();
		}		
		break;
	case TotalSet_ESN_On: //������ƷESN����
		if(Rec7 == 0)
			g_ReplaceType = Replace_ESN;
		else if(Rec7 == 1){
			X_paramPlaceKeyword((char*)"ESN", (char*)"", 1);
			X_paramSaveDeviceInfo();
		}
		break;
	case TotalSet_Date_On: //������ƷDate����
		if(Rec7 == 0)
			g_ReplaceType = Replace_Date;
		else if(Rec7 == 1){
			X_paramPlaceKeyword((char*)"DATE", (char*)"", 1);
			X_paramSaveDeviceInfo();
		}
		break;
	case TotalSet_SaveToUserDef: //����ǰ���ô�Ϊ�û�Ĭ������
		if(!X_paramSaveAsUsrDefaultParam()) return FALSE;
		break;
	case TotalSet_LoadUserDef: //�����û�Ĭ������
		if(!X_paramLoadUsrDefaultParam()) return FALSE;
		break;
	case TotalSet_LIC_On: //���ò�ƷLicence
		g_ReplaceType = Replace_LIC;
		break;
	case TotalSet_ManufID_On: //���ò�ƷManufactureID
		if(Rec7 == 0)
			g_ReplaceType = Replace_ManufID;
		else if(Rec7 == 1){
			X_paramPlaceKeyword((char*)"ManufactureID", (char*)"", 1);
			X_paramSaveDeviceInfo();
		}
		break;
	case TotalSet_IsDownload:
		if(Rec7 > 1) return FALSE;
		else{
			X_paramGetGlobalAllParam()->pDevInfo->IsDownload = 0x30 + Rec7;
                    paramSaveType(1);
			X_paramPlaceKeyword((char*)"IsDownload",(char*)&(X_paramGetGlobalAllParam()->pDevInfo->IsDownload),TRUE);
			X_paramSaveDeviceInfo();
		}
		break;
	case TotalSet_SaveToFactory: //����ǰ���ô�Ϊ��������
		if(!X_paramSaveAsFactoryParam()) return FALSE;
		break;
	case TotalSet_Load_General://����ͨ������
		if(!X_paramOriginalInitAllParam()) return FALSE;
	//	if(!X_paramSaveAsSysDefaultParam()) return FALSE;
		break;
	case TotalSet_ManufName_On: //���ò�ƷManufactureName
		if(Rec7 == 0)
			g_ReplaceType = Replace_ManufName;
		else if(Rec7 == 1){
			X_paramPlaceKeyword((char*)"ManufactureName", (char*)"", 1);
			X_paramSaveDeviceInfo();
		}
		break;

	default : return FALSE;
	}
	return TRUE;
}

//���������ۺ�����
BOOL Set_DataAndTotal(int Rec34, int Rec56, int Rec7)
{
	switch(Rec34){
	case SubSet_Data://������
		if(SetCode_Discard(Rec56)) {return TRUE;}
		else if(Rec56 < 0x10 && Rec56 >= 0 &&
			(g_DataType == DataSetType_Prefix || g_DataType == DataSetType_Suffix
			|| g_DataType == DataSetType_Terminal || g_DataType == DataSetType_1DCodeID
			|| g_DataType == DataSetType_2DCodeID || g_DataType == DataSetType_BlueT)){
				//����ǰ��׺
				if(!SetCode_Data_PreSuffix(Rec56)) return FALSE;
		}
		else if(Rec56 < 0x10 && Rec56 >= 0 && (g_DataType == DataSetType_DataFormatting)){	
				if (!SetCode_Data_DataFormat(Rec56)) return FALSE;	
		}
		else if(Rec56 < 0x0a && Rec56 >= 0 && (g_DataType == DataSetType_FixedLen
			|| g_DataType == DataSetType_Cut || g_DataType == DataSetType_Fixed_Dis)){
		    //���ù̶����볤��
			if(!SetCode_Data_FixedLen(Rec56)) return FALSE;
		}
		else if(Rec56 < 10 && Rec56 >= 0){//������������
			g_DataSet = g_DataSet * 10 + Rec56;

			//һά����������С��������Խ���ж�
			if(((g_DataType == DataSetType_1DMax) || (g_DataType == DataSetType_1DMin)) && (g_DataSet > 0x7f)) 
				return FALSE;

			//OCR�����С��������Խ���ж�
			if(((g_DataType == DataSetType_OCRMax) || (g_DataType == DataSetType_OCRMin)) && (g_DataSet > 0x7f)) 
				return FALSE;

			//��ά����������С��������Խ���ж�
			if(((g_DataType == DataSetType_2DMax) || (g_DataType == DataSetType_2DMin)) && (g_DataSet > 0xffff)) 
				return FALSE;

			//��ʱ����Խ���ж�
			if((g_DataType == DataSetType_OnceDelay || g_DataType == DataSetType_InfoDelay) && g_DataSet > Max_Delay)
				return FALSE;
		}
		else if(Rec56 == POS_Save){ //����
			if(!SetCode_Save()) return FALSE;
		}
		else return FALSE;
		break;

	case SubSet_Total: //�����ۺ�����
		if(!SetCode_Total(Rec56, Rec7)) return FALSE;
		break;

	case SubSet_SetCodeSend: //��������Ϣ��������
		if(Rec56 == 0 || Rec56 == 1)
			X_paramBakGetGlobalAllParam()->pCode->SetCode = (X_paramGetGlobalAllParam()->pCode->SetCode & (~0x01)) | Rec56;
		else return FALSE;
		break;

	case SubSet_SetSysMesSend: //ϵͳ�����Ϣ��������
		if(Rec56 != 0) return FALSE;

		if (Rec7 == 0)
		{
			X_paramSendParamInfo();
		}
		else if (Rec7 == 1)
		{
			char buff[40];
			Z_GetKernelInfo(0, buff,sizeof(buff));
			Y_commWrite((unsigned char*)buff,strlen(buff));
		}
		
		W_setcSetNoSaveFlag(TRUE);//������

		break;
	case SubSet_Set1DCodeID: //�Զ���һά�����CodeIDǰ׺����
		if(Rec56 > 0x1f) return FALSE; //����32��һά����
		g_DataType = DataSetType_1DCodeID;
		memset(g_PreSuffix, 0, 20);
		g_PosMaxMin = Rec56;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		break;
	case SubSet_Set2DCodeID: //�Զ����ά�����CodeIDǰ׺����
		if(Rec56 < 0x20)
		{
			g_DataType = DataSetType_2DCodeID;
			memset(g_PreSuffix, 0, 20);
			g_PosMaxMin = Rec56;
			g_DataSet = 0;
			g_IsSetCodeStart = TRUE;
			g_nPreSuffix = 0;
			g_DataSet_Odd = TRUE;
		}
		else if((Rec56 >= 40) && (Rec56 <= 72))	//���������CodeIDǰ׺Ҳ��ʱ��������
		{
			g_DataType = DataSetType_PostalCodeID;
			memset(g_PreSuffix, 0, 20);
			g_PosMaxMin = Rec56-40;
			g_DataSet = 0;
			g_IsSetCodeStart = TRUE;
			g_nPreSuffix = 0;
			g_DataSet_Odd = TRUE;
		}else{
			return FALSE;
		}
		break;
	case SubSet_IsOpenSetCode: //�����빦��������ر�����
		W_setcSetSaveFlag(FALSE);
		if(Rec56 == 0){
			if (X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff)
				return FALSE;
		    W_setcEnable(FALSE);
			W_setcSetResult(U_uIMG_STATUS_SETCODE_END);
		}
		else if (Rec56 == 1){
			if (X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff)
				return FALSE;
			W_setcEnable(TRUE);
			W_setcSetResult(U_uIMG_STATUS_SETCODE_START);
		}
		else if (Rec56 == 3){
			if (Rec7 == 0)
			{
				X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff = TRUE;
				W_setcEnable(FALSE);
				W_setcSetResult(U_uIMG_STATUS_SETCODE_END);
			}
			else if (Rec7 == 1){
				X_paramGetGlobalAllParam()->pCode->IsSettingBarcodeOff = FALSE;
			}

		}
		else
			return FALSE;
		break;
	case SubSet_IsPowerOn: //��Դ����ʱ�Ƿ���ʾϵͳ��Ϣ����
		if(Rec56 == 0 || Rec56 == 1)
		    X_paramBakGetGlobalAllParam()->pCode->IsPowerOnDisp = Rec56;
		else if(Rec56 == 2 || Rec56 == 3)
			X_paramBakGetGlobalAllParam()->pCode->IsGotoConsole = Rec56 - 2;
		else return FALSE;
		break;
	case SubSet_CloseDevice: //�ػ�����
		break;
	default: return FALSE;
	}
	return TRUE;
}

//ͨѶ����
BOOL Set_Comm(int Rec34, int Rec56)
{
#if defined(MB1126)
	return TRUE;
#endif 
	switch(Rec34){
    case SubSet_BoundRate://����������
		switch(Rec56) {
		case 0:	case 1:	case 2:	case 3:	case 4:
		case 5:	case 6:	case 7:	case 8:
			X_paramBakGetGlobalAllParam()->pCom->lRate = conBaudRate[Rec56];
			break;
		default: return FALSE;
		}    
		break;
	case SubSet_Parity://У������
		if(Rec56 == NOPARITY) X_paramBakGetGlobalAllParam()->pCom->lParity = NOPARITY;
		else if(Rec56 == EVENPARITY) X_paramBakGetGlobalAllParam()->pCom->lParity = EVENPARITY;
		else if(Rec56 == ODDPARITY) X_paramBakGetGlobalAllParam()->pCom->lParity = ODDPARITY;
		else return FALSE;
	    break;
    case SubSet_WordLenStop://�ַ�����ֹͣλ����
		if(Rec56 == 0 || Rec56 == 1){
		    X_paramBakGetGlobalAllParam()->pCom->lStopBits = Rec56 + 1;
		}
		else return FALSE;
	    break;
	case SubSet_WordLenDataBits://�ַ���������λ����
// 		if(Rec56 >= 0 && Rec56 < 4){
		if(Rec56 >= 2 && Rec56 < 4){
		    X_paramBakGetGlobalAllParam()->pCom->lDataBits = Rec56 + 5;
		}
		else return FALSE;
		break;
	case SubSet_HardwareFlowCtrl://Ӳ����������
		if(Rec56 == 0 || Rec56 == 1){
			X_paramBakGetGlobalAllParam()->pCom->nflowCtrl = Rec56;
		}
		else return FALSE;
		break;
	default:return FALSE;
	}
 
	return TRUE;
}

//Ӳ������
BOOL Set_Hard(int Rec34, int Rec56, int Rec7)
{
	char b[10];
	switch(Rec34){
	case 0://����������
		if(Rec56 > 3 || Rec56 < 0) return FALSE;
// 		if (Rec56 == 1) return FALSE; //���ܳ���

		X_paramBakGetGlobalAllParam()->pHard->nLight = Rec56;
		break;

	case 1://�Խ�������
// 		if(Rec56 > 2 || Rec56 < 0) return FALSE;
		if(Rec56 > 3 || Rec56 < 0) return FALSE;
// 		if (Rec56 == 1) return FALSE; //���ܳ���
		X_paramBakGetGlobalAllParam()->pHard->nFocus = Rec56;
//		if(X_paramGetGlobalAllParam()->pHard->nFocus == 3)
//		{ X_paramGetGlobalAllParam()->pCode->nReadMode = ESC2; }
// 		if(X_paramGetGlobalAllParam()->pHard->nFocus == 3)
// 		{ X_paramGetGlobalAllParam()->pHard->nFocus = 0; }
		break;
	case 2: //Ӳ����������
		if(Rec56 == 0) 
		{
			X_paramBakGetGlobalAllParam()->pHard->nProduction = 0;
                   paramSaveType(1);
			if (X_paramQueryKeyword((char*)"Mirr", (char*)b) == TRUE)
			{
				X_paramPlaceKeyword((char*)"Mirr", (char*)"off", 1);
			}
		}
		else if(Rec56 == PRODUCTION_MIRROR) 
		{
			if(Rec7 ==0) 
			{
				X_paramBakGetGlobalAllParam()->pHard->nProduction = PRODUCTION_MIRROR;
                         paramSaveType(1);
				X_paramPlaceKeyword((char*)"Mirr", (char*)"on", 1);
			}
			else if(Rec7 == 7)
			{
			      paramSaveType(1);
				X_paramPlaceKeyword((char*)"Mirr", (char*)"", 1);
				X_paramBakGetGlobalAllParam()->pHard->nProduction = PRODUCTION_DEF;
			}
			else
				return FALSE;
		}
// 		else if (Rec56 == PRODUCTION_DOF)	//��������
// 		{
// 			if (Rec7 == 0)			//������
// 			{
// 				X_paramGetGlobalAllParam()->pHard->nDofType = PRODUCTION_DOF_NEAR;
// 				X_paramPlaceKeyword((char*)"DofType", (char*)"HD", 1);	//High Density
// 
// 			}
// 			else if (Rec7 == 1)		//�о���
// 			{
// 				X_paramGetGlobalAllParam()->pHard->nDofType = PRODUCTION_DOF_MIDDLE;
// 				X_paramPlaceKeyword((char*)"DofType", (char*)"SR", 1);	//Standard Range
// 			}
// 			else if (Rec7 == 2)		//Զ����
// 			{
// 				X_paramGetGlobalAllParam()->pHard->nDofType = PRODUCTION_DOF_FAR;
// 				X_paramPlaceKeyword((char*)"DofType", (char*)"ER", 1);	//Extended Range
// 			}
// 			else if (Rec7 == 3)
// 			{
// 				X_paramPlaceKeyword((char*)"DofType", (char*)"", 1);	
// 				X_paramSaveDeviceInfo();
// 				X_paramGetGlobalAllParam()->pHard->nDofType = PRODUCTION_DOF_NORMAL;
// 			}
// 			else
// 				return FALSE;
// 
// 			if (Z_cmos_GetType() == T_CMOS_MT9V022)
// 			{
// 				MT9V022_SetExp();
// 			}
// 
// 		}
		else if (Rec56 == PRODUCTION_SCANSTATUE)
		{
			if (Rec7 == 0)		
			{
				X_paramBakGetGlobalAllParam()->pCode2->bSurePOSPowerUpScanEnable = TRUE;
			}
			else if (Rec7 == 1)	
			{
				X_paramBakGetGlobalAllParam()->pCode2->bSurePOSPowerUpScanEnable = FALSE;
			}
			else 
				return FALSE;
		}
		else 
			return FALSE;

		break;
    case 3: //����������������
		if(Rec56 < 2){ //������������ر�����
			X_paramBakGetGlobalAllParam()->pHard->bDecVoice &= ~0x01;
			X_paramBakGetGlobalAllParam()->pHard->bDecVoice |= Rec56;
		}
		else if(2 == Rec56){ //���ý�����������
			if(Rec7 < 3){ //Ŀǰֻ�ṩ������������
				X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0x0f;
				X_paramBakGetGlobalAllParam()->pHard->nBeepType |= Rec7;
			}
			else return FALSE;
		}
		else if(3 == Rec56){ //���ý�������������
			if(Rec7 < 3){ //Ŀǰֻ�ṩ���е���������
				X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0xf0;
				X_paramBakGetGlobalAllParam()->pHard->nBeepType |= (Rec7 << 4);
			}
			else return FALSE;
		}
		else if(5 == Rec56){ //���ý��������ĳ���ʱ��duration  020305x
			if(Rec7 < 2){
				X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0xf00;
				X_paramBakGetGlobalAllParam()->pHard->nBeepType |= (Rec7 << 8);
			}
			else if(Rec7 == 2){ //���ý���0203052
				g_DataType = DataSetType_ReadBeepDura;
				g_DataSet = 0;
				g_IsSetCodeStart = TRUE;
			}
			else return FALSE;
		}
		else if(6 == Rec56){ //���ý���������Ƶ��freq 020306x
			if(Rec7 < 4){
				//bit15 - bit12	Ƶ��	0 = Medium; 1 = Lowest; 2 = Low; 3 = Medium; 4 = High; 9 = �û�����
				//��Ӧ�������Rec7�� 0 = Lowest; 1 = Low; 2 = Medium; 3 = High; 4 = �û�����
				//Ƶ���ݶ�Ϊ��800�� 1600�� 2730�� 4200��		
				X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0xf000;
				X_paramBakGetGlobalAllParam()->pHard->nBeepType |= ((Rec7 + 1)<< 12);
			}
			else if(Rec7 == 4){ //�û��Զ����������Ƶ�� 0203064
				g_DataType = DataSetType_ReadBeepFreq;
				g_DataSet = 0;
				g_IsSetCodeStart = TRUE;
			}
			else return FALSE;
		}
		else return FALSE;
		break;
	case 4: //������������
		if(!Rec56){
			if(Rec7 < 2){ //������������ر�����
				X_paramBakGetGlobalAllParam()->pHard->bDecVoice &= ~0x02;
				X_paramBakGetGlobalAllParam()->pHard->bDecVoice |= (Rec7 << 1);
				//U_appDevicePowerOnBeep();
				//if(X_paramGetGlobalAllParam()->pHard->bDecVoice & 0x02)
				//	Z_Beep(2500, 100, 10);
			}
			else return FALSE;
		}
		else return FALSE;
		break;
	case 5://�ⲿ����������
		if(Rec56 > 1 || Rec56 < 0) return FALSE;	
		X_paramBakGetGlobalAllParam()->pHard->nExLight = Rec56;
		break;

	case 6://����ɹ�ָʾ������
		if(Rec56 == 1)//����ɹ�ָʾ������
		{
			if (Rec7 == 1)	//����
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLED &= ~ 1;
			}
			else if (Rec7 == 0)	//�ر�
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLED |= 1;
			}
			else if (Rec7 == 2)//����
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLED &= ~0x80;				
			}
			else if (Rec7 == 3)//����
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLED |= 0x80;				
			}
			else
				return FALSE;

		}
		else if (Rec56 == 2)//����ɹ�ָʾ�Ƴ���ʱ��
		{
			if (Rec7 == 0)//��
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 0;
			}
			else if (Rec7 == 1)//��
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 1;				
			}
			else if (Rec7 == 2)//��
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 2;
			}
			else if (Rec7 == 3)//�
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 3;
			}
			else if (Rec7 == 4)//�Զ���
			{
				g_DataType = DataSetType_ReadLEDDura;
				g_DataSet = 0;
				g_IsSetCodeStart = TRUE;
			}
			else
				return FALSE;

		}
#ifdef BUILT_HR32
		else if (Rec56 == 0)//��Դָʾ������
		{
			if (Rec7 == 0)//��
				X_paramBakGetGlobalAllParam()->pCode2->bPowerLedOn = FALSE;
			else if (Rec7 == 1)//��
				X_paramBakGetGlobalAllParam()->pCode2->bPowerLedOn = TRUE;
			else
				return FALSE;

			Z_SetPowerLedOn(X_paramBakGetGlobalAllParam()->pCode2->bPowerLedOn);

		}
#endif
		else
			return FALSE;


		break;

	default:return FALSE;
	}
	return TRUE;
}

//����ģʽ������
BOOL Set_DecodeMode(int Rec34, int Rec56, int Rec7, int RecParam)
{
    char buff[1152];
    unsigned int i;
    
	W_setcSetSaveFlag(TRUE);
	switch(Rec34){
	case DecModeSet_Prefix://ǰ׺����
	    if(!Rec56){
		    g_DataType = DataSetType_Prefix;
		    memset(g_PreSuffix, 0, 20);
		    g_nPreSuffix = 0;
		    g_DataSet_Odd = TRUE;
		    g_IsSetCodeStart = TRUE;
		}
		else return FALSE;
		break;
    case DecModeSet_Suffix://�Զ����׺����
		if(!Rec56) {
			g_DataType = DataSetType_Suffix;
		    memset(g_PreSuffix, 0, 20);
		    g_nPreSuffix = 0;
		    g_DataSet_Odd = TRUE;
		    g_IsSetCodeStart = TRUE;
		}
		else return FALSE;
	    break;
    case DecModeSet_Repeat://����ɨ�跽ʽ����
		switch(Rec56) {
		case 0://�ֶ�����
			X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC0;
		    X_paramBakGetGlobalAllParam()->pMode->bContinue = TRUE;
			break;
		case 1://�Զ�����
			X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC2;
		    X_paramBakGetGlobalAllParam()->pMode->bContinue = TRUE;
	    	X_paramBakGetGlobalAllParam()->pMode->nReadDelay = Z_Get10msCounter();
		    X_paramBakGetGlobalAllParam()->pMode->nHaltDelay = Z_Get10msCounter();
		    X_paramBakGetGlobalAllParam()->pMode->nCodeDelay = Z_Get10msCounter();
			break;
		case 2://��������
			X_paramBakGetGlobalAllParam()->pMode->bContinue = FALSE;
		    X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC3;
			break;
		case 3://
			X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC1;
		    X_paramBakGetGlobalAllParam()->pMode->bContinue = TRUE;
			W_setcSetNoSaveFlag(TRUE);//������
		    X_paramBakGetGlobalAllParam()->pMode->nReadDelay = Z_Get10msCounter();
			break;
		case 9:
			if(Rec7 == 0)
			{
				if(RecParam <= 0)
					U_appSetTestMode(1, 1000); //����������
				else
					U_appSetTestMode(1, RecParam);
			}
			else if(Rec7 == 1)
			{
				if(RecParam <= 0)
					U_appSetTestMode(2, 60*1000);//��ʱ�����
				else
					U_appSetTestMode(2, RecParam * 1000);
			}
			else if(Rec7 == 2)
			{
				if(RecParam <= 0)
					U_appSetTestMode(3, 1000);	//���ɹ���������
				else
					U_appSetTestMode(3, RecParam);
			}
			else if(Rec7 == 3)
			{
				if(RecParam <= 0)
					U_appSetTestMode(4, 10000);		//�����ʲ��ԣ�10000��
				else
					U_appSetTestMode(4, RecParam);	

			}
			//����������ģʽ�йص���������ʱ����
			break;
		default: return FALSE;
		}
		break;
	case DecModeSet_Protect://ͼ���˲�����
		if(Rec56 < 2) X_paramBakGetGlobalAllParam()->pCode->Protect = Rec56;
		else return FALSE;
	    break;
	case DecModeSet_IsPrefix://�Զ���ǰ׺ʹ�ܻ��ֹ����
		if(Rec56 < 2) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] = Rec56;
		else return FALSE;
		break;
	case DecModeSet_IsSuffix://�Զ����׺ʹ�ܻ��ֹ����
		if(Rec56 < 2) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] = Rec56;
		else return FALSE;
		break;
	case DecModeSet_IsCodeID://CodeIDǰ׺ʹ�ܻ��ֹ����
		if(Rec56 < 2) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] = Rec56;
		else if(Rec56 == 2){ //�����������Ĭ��CodeID
            memcpy(X_paramBakGetGlobalAllParam()->pCode->by1DCodeID, CodeIDTab1D, 32);
			memcpy(X_paramBakGetGlobalAllParam()->pCode->by2DCodeID, CodeIDTab2D, 32);
			memcpy(X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID, CodeIDPostal, 32);
            memcpy(X_paramBakGetGlobalAllParam()->pCode2->by1DCodeID_2, CodeIDTab1D_2, 32);
			memcpy(X_paramBakGetGlobalAllParam()->pCode2->by2DCodeID_2, CodeIDTab2D_2, 32);
			memcpy(X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID_2, CodeIDPostal_2, 32);
		}
		else return FALSE;
		break;
	case DecModeSet_IsAIM://AIMǰ׺��ֹ��ʹ��ģʽ����
// 		if(Rec56 < 4) X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] = Rec56;
// 		else return FALSE;
		
		if(Rec56 == 0){ //ʹ������
			if(Rec7) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] |= 0x01;
			else X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] &= (~0x01);
		}
		else if(Rec56 < 4){
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] &= (~0x06);
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] |= (((unsigned char)(Rec56 - 1) << 1) | 0x1);
		}
		else return FALSE;
		break;
	case DecModeSet_IsTerminal://��������׺ʹ�ܻ��ֹ����
		if(Rec56 < 2) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] = Rec56;
		else return FALSE;
		break;
	case DecModeSet_Terminal://�Զ����������׺����
		if(Rec56 == 0){
			g_DataType = DataSetType_Terminal;
		    memset(g_PreSuffix, 0, 20);
		    g_nPreSuffix = 0;
		    g_DataSet_Odd = TRUE;
		    g_IsSetCodeStart = TRUE;
		}
		else if (Rec56 == 1)		//���0x0d;
		{
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[0] = 0x0d;
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[1] = 0;
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[2] = 0;
			X_paramBakGetGlobalAllParam()->pCode->nTerminal = 1;
		}
		else if (Rec56 == 2)		//���0x0d 0x0a;
		{
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[0] = 0x0d;
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[1] = 0x0a;
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[2] = 0;
			X_paramBakGetGlobalAllParam()->pCode->nTerminal = 2;
		}
		else return FALSE;
		break;
	case DecModeSet_AllPreSuf://����ǰ��׺ʹ�ܻ��ֹ����
		if(Rec56 < 2){
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] = Rec56;
		    X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] = Rec56;
		    X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] = Rec56;
		    X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] = Rec56;

			if(Rec56) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] |= 0x01;
			else X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] &= (~0x01);
		}
		else return FALSE;
		break;
		
	case DecModeSet_Sensibility:  //����������
		if(Rec56 >= 0 && Rec56 < 4) //���ü���
		//	X_paramGetGlobalAllParam()->pCode->nSensitive = 3 - Rec56; //0��ߣ�3���
		    X_paramBakGetGlobalAllParam()->pCode->nSensitive = SENSITIVE_CALC(Rec56); // 5,10,15,20
		else if(Rec56 == 4) {//����ֵ
			g_DataType = DataSetType_Sensibility;
			g_DataSet = 0;
			g_IsSetCodeStart = TRUE;
		}
		else return FALSE;
		break;
    
	case DecModeSet_Delay:  //�ӳ�����
		if(Rec56 == 0){
			g_DataType = DataSetType_OnceDelay;
			g_DataSet = 0;
			g_IsSetCodeStart = TRUE;
		}
		else if(Rec56 == 1){
			g_DataType = DataSetType_InfoDelay;
			g_DataSet = 0;
			g_IsSetCodeStart = TRUE;
		}
		else if(Rec56 == 2 || Rec56 == 3)
		    X_paramBakGetGlobalAllParam()->pCode->IsCompDelay = Rec56 - 2;
		else return FALSE;
		break;
	
	case DecModeSet_DataPacked:  //���ݴ������
#if !defined(MB1126) //�����1126������,����Ĭ�ϰ�������������һ�Ų�����
		if(Rec56 >= 0 && Rec56 < 5)
			X_paramBakGetGlobalAllParam()->pCode->nPack = Rec56;
		else return FALSE;
#endif
// 		if(X_paramGetGlobalAllParam()->pCode->nPack == BLUE2D_PACK){
// 			//���ͳ�����Ϣ
// 			X_paramGetGlobalAllParam()->pCode->IsOtherPreSufOn |= (0x01 << POS_IsBlue2D_LenSend);
// 			//���������������Ϣ
//         	X_paramGetGlobalAllParam()->pCode->IsOtherPreSufOn &= ~(0x01 << POS_IsBlue2D_TypeSend);
// 		}
		break;
	
	case DecModeSet_IsCut: //��ȡʹ������
		if(Rec56 >= 0 && Rec56 < 2)
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCut] = Rec56;
		break;

	case DecModeSet_Cut: //��ȡ��ʽ����
		if(!Rec56){
		    g_DataType = DataSetType_Cut;
	        g_PosMaxMin = 0;
	        g_DataSet = 0;
	        g_nPreSuffix = 0;
	        g_nFixedLen = 0;
	        g_IsSetCodeStart = TRUE;
	        memset(g_PreSuffix, 0, 20);
		}
		else if(Rec56 == 1){ //���ĳ������Ľ�ȡ��ʽ
			g_DataType = DataSetType_Cut;
	        g_PosMaxMin = 1;
			g_DataSet = 0;
	        g_IsSetCodeStart = TRUE;
		}
		else if(Rec56 == 2){ //�������һ���ȡ��ʽ
			memset(X_paramBakGetGlobalAllParam()->pCode->byCut + (Max_DataCutLen << 1), 0, Max_DataCutLen);
		}
		else if(Rec56 == 3){ //���ȫ����ȡ��ʽ
			memset(X_paramBakGetGlobalAllParam()->pCode->byCut, 0, (Max_DataCutLen << 1) + Max_DataCutLen);
		}
		else return FALSE;
		break;
			
	case DecModeSet_PrefixOrder: //ǰ׺˳������
// 	    if(X_paramGetGlobalAllParam()->pCode->nUserDef == UserType_BLUE_2D && Rec56 >1)
// 			return FALSE; //BLUE_2D״̬��CodeID��������ǰ��
		if(Rec56 < 6)
			X_paramBakGetGlobalAllParam()->pCode->PrefixOrder = Rec56;
		break;

	case DecModeSet_ScanTime: //��������޶�����
	    g_DataType = DataSetType_ScanTime;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	
	case DecModeSet_Medium: //�����������
		if(Rec56 > 2) return FALSE;
		X_paramBakGetGlobalAllParam()->pCode->nMedium = Rec56+1;
		break;

	case DecModeSet_ErrorMsg: //һ��ʱ����û�����뷵����Ϣ����
		if(Rec56 == 0){//���ý�ֹ
			X_paramBakGetGlobalAllParam()->pCode->bErrMsgEn = FALSE;
		}
		else if(Rec56 == 1){ //����ʹ��
			X_paramBakGetGlobalAllParam()->pCode->bErrMsgEn = TRUE;
		}
		else if(Rec56 == 2){
		    g_DataType = DataSetType_ErrorMsg;
		    memset(g_PreSuffix, 0, 20);
		    g_nPreSuffix = 0;
		    g_DataSet_Odd = TRUE;
		    g_IsSetCodeStart = TRUE;
		}
		else return FALSE;
		break;
	case DecModeSet_CMOSAdjustMode:	//CMOS�ع�ģʽ����
		if(Rec56 == 0){//��ͨģʽ --ֽ����
// 			if (Rec7 == 1)//ֽ���� mode 2, �չ�ֽ���룬����ã��ܶ��ֻ���
// 			{
// 				X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = 3;
// 			}
// 			else 
			if (Rec7 == 0)	//ֽ����mode
			{
				X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode = 0;
			}

		}
// 		else if(Rec56 == 1){ //���ͷ���ģʽ
// 			X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = 1;
// 		}
		else if (Rec56 == 2) //phone mode
		{
// 			if (Rec7 == 0)	//phone mode1
// 			{
// 				X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = 4;
// 			}
// 			else
			if (Rec7 == 1)//phone mode2(������������)
			{
				X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode = 2;
			}
		}
		else return FALSE;

		Z_SetLightMode(X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode);

		break;

	case DecModeSet_DecodeArea:	//�����������
		if(Rec56 == 0){//ȫ����ȡ
			X_paramBakGetGlobalAllParam()->pCode->decodeArea &= ~0xf;
		}
		else if(Rec56 == 1){ //���������ȡ
			X_paramBakGetGlobalAllParam()->pCode->decodeArea |= 1;
		}
		else if(Rec56 == 2){ //������������
			g_DataType = DataSetType_DecodeArea;
			g_DataSet = 0;
			g_IsSetCodeStart = TRUE;
		}
		else return FALSE;
		break;
	case DecModeSet_DataFormat:
		if (Rec56 == 0)//���ݸ�ʽ�༭
		{
			g_DataType = DataSetType_DataFormatting;
			g_DataSet = 0;
			g_IsSetCodeStart = TRUE;
		}
		else if (Rec56 == 1)//���ĳ��dataformatting
		{
			if (Rec7 ==0)//���ĳһ��format����
			{
				g_DataType = DataSetType_DataFormatting;
				g_PosMaxMin = 1;//���øı�������ʾҪ����ĳһ��dataformat���������
				g_DataSet = 0;
				g_IsSetCodeStart = TRUE;
			} 
			else//�������format����
			{
				memset(X_paramBakGetGlobalAllParam()->pDFormat->format, 0, sizeof(X_paramGetGlobalAllParam()->pDFormat->format));
			}
			
		}
		else if (Rec56 == 2)//����
		{			
			X_paramBakGetGlobalAllParam()->pDFormat->DFEnable = Rec7;	
		}
		else if (Rec56 == 3)//format��ѡ��
		{
			X_paramBakGetGlobalAllParam()->pDFormat->usedID = Rec7;
		}
		else if (Rec56 == 4)
		{//Rec7=0:��dataformatting��ʽ��������з��ϸ�ʽ������+ǰ��׺���(�����ǰ��׺)�����в����ϸ�ʽ�����벻������ҷ����򲻷���������ʾ��
		 //Rec7=1:��dataformatting��ʽ��������з��ϸ�ʽ�����벻+ǰ��׺��������в����ϸ�ʽ�����벻������ҷ����򲻷���������ʾ��
		 //Rec7=2:����dataformatting��ʽ�����밴dataformatting��ʽ+ǰ��׺���(�����ǰ��׺)�����в����ϸ�ʽ�����밴ԭ��+ǰ��׺���(�����ǰ��׺)
		 //Rec7=3:����dataformatting��ʽ�����밴dataformatting��ʽ��+ǰ��׺��������в����ϸ�ʽ�����밴ԭ��+ǰ��׺���(�����ǰ��׺)
			X_paramBakGetGlobalAllParam()->pDFormat->DFEnable = 1;//ʹ�� data formatting
			X_paramBakGetGlobalAllParam()->pDFormat->outFlag = Rec7;
		}
		else if (Rec56 == 5)
		{
			X_paramBakGetGlobalAllParam()->pDFormat->iSsingle = 1;
			X_paramBakGetGlobalAllParam()->pDFormat->singleFormatID = Rec7;
		}
		else if (Rec56 == 6)
		{
			X_paramBakGetGlobalAllParam()->pDFormat->ErrorTone = Rec7;
		}
		else if (Rec56 == 7)
		{
			//069999999999f141
			for(i=0; i<4; i++)
			{
				if (X_paramBakGetGlobalAllParam()->pDFormat->format[i].TerID > 0)
				{
					sprintf(buff,"Data Format %d: %d",i,X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatID);
					sprintf(buff + strlen(buff),"%d",X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatType);
					sprintf(buff + strlen(buff),"%d",X_paramBakGetGlobalAllParam()->pDFormat->format[i].TerID);
					if (X_paramBakGetGlobalAllParam()->pDFormat->format[i].FormatType == 3)
					{
						sprintf(buff + strlen(buff),"%02d",X_paramBakGetGlobalAllParam()->pDFormat->format[i].SymbolID);
					} 
					else
					{
						sprintf(buff + strlen(buff),"%03d",X_paramBakGetGlobalAllParam()->pDFormat->format[i].SymbolID);
					}
					
					sprintf(buff + strlen(buff),"%04d",X_paramBakGetGlobalAllParam()->pDFormat->format[i].MsgLen);
					sprintf(buff + strlen(buff),"%s",X_paramBakGetGlobalAllParam()->pDFormat->format[i].Cmd);
					sprintf(buff + strlen(buff),";\r\n");	
					Y_commWrite((unsigned char*)buff,strlen(buff));
				}
			}
		}
		else if (Rec56 == 10)
		//������и�ʽ���ָ�����Ĭ������
		{
			X_paramBakGetGlobalAllParam()->pDFormat->DFEnable = FALSE;//disable
			X_paramBakGetGlobalAllParam()->pDFormat->ErrorTone = 0;//������ʾ��
			X_paramBakGetGlobalAllParam()->pDFormat->outFlag = 0;//���ݸ�ʽ��ƥ��ʱ�����Ҫ��
			X_paramBakGetGlobalAllParam()->pDFormat->iSsingle = 0;//����format�ر�
			X_paramBakGetGlobalAllParam()->pDFormat->usedID = 0;
			memset(X_paramBakGetGlobalAllParam()->pDFormat->format, 0, sizeof(X_paramGetGlobalAllParam()->pDFormat->format));
		}
		else
			break;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//128������
BOOL SetCode1D_Code128(int Rec56)
{
	switch(Rec56){
	case Set_Default://128������в�������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = Init_CODE128;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODE128);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UCCEAN128));
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_CODE128));
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UCCEAN128));
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODE128);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CODE128;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
	    g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_CODE128;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
// 	case Code128_UCCEAN_Off://��ʶ��UCC-EAN128����
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] &= 0xffffff0c;
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UCCEAN128));
// 		break;
// 	case Code128_UCCEAN_FNC1_1://ʶ��UCC-EAN����FNC1��Start��ʼ�ĵ�һ��λ��
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xfffffffc) | 0x01;
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_UCCEAN_FNC1_2://ʶ��UCC-EAN����FNC1��Start��ʼ�ĵڶ���λ��
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xfffffffc) | 0x03;
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_FNC2_Off://��ʶ�����FNC2������
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] &= 0xfffffff3;
// 		break;
// 	case Code128_FNC2_1://ʶ������ʼStart�ַ���һ���ַ������FNC2������
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xfffffff3) | (0x01 << 2);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_FNC2_2://ʶ������ʼStart�ַ��ڶ����ַ������FNC2������
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xfffffff3) | (0x03 << 2);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_UCCEAN_FNC3_1://ʶ��UCC-EAN����FNC3��Start��ʼ�ĵ�һ��λ��
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xffffffcf) | (0x01 << 4);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_UCCEAN_FNC3_2://ʶ��UCC-EAN����FNC3��Start��ʼ�ĵڶ���λ��
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xffffffcf) | (0x03 << 4);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_UCCEAN_FNC4_1://ʶ��UCC-EAN����FNC4��Start��ʼ�ĵ�һ��λ��
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xffffff3f) | (0x01 << 6);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_UCCEAN_FNC4_2://ʶ��UCC-EAN����FNC4��Start��ʼ�ĵڶ���λ��
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xffffff3f) | (0x03 << 6);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_CHECK_ON:
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] &= 0xfffffeff;
// 		break;
// 	case Code128_CHECK_OFF:
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xfffffeff) | (0x01 << 8);
// 		break;
	default: return FALSE;
	}
	return TRUE;
}

//EAN-8������
BOOL SetCode1D_EAN8(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] = Init_EAN8;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_EAN8);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_EAN8));
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_EAN8);
		break;
	case EANUPC_SendCheck_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] &= (~0x01);
		break;
	case EANUPC_SendCheck_On://����У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= 0x01;
		break;
	case EANUPC_2Expand_Off://��ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] &= (~0x02);
		break;
	case EANUPC_2Expand_On://ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= 0x02;
		break;
	case EANUPC_5Expand_Off://��ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] &= (~0x04);
		break;
	case EANUPC_5Expand_On://ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= 0x04;
		break;
	case EANUPC_EAN8To13_Off://���ѽ����չ��EAN-13
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] &= (~0x10);
		break;
	case EANUPC_EAN8To13_On://�ѽ����չ��EAN-13
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= 0x10;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//EAN-13������
BOOL SetCode1D_EAN13(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] = Init_EAN13;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_EAN13);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_EAN13));
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_EAN13);
		break;
	case EANUPC_SendCheck_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] &= (~0x01);
		break;
	case EANUPC_SendCheck_On://����У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] |= 0x01;
		break;
	case EANUPC_2Expand_Off://��ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] &= (~0x02);
		break;
	case EANUPC_2Expand_On://ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] |= 0x02;
		break;
	case EANUPC_5Expand_Off://��ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] &= (~0x04);
		break;
	case EANUPC_5Expand_On://ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] |= 0x04;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//UPC-E������
BOOL SetCode1D_UPCE(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] = Init_UPCE; 
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UPCE);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UPCE));
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UPCE);
		break;
	case EANUPC_SendCheck_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x01);
		break;
	case EANUPC_SendCheck_On://����У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x01;
		break;
	case EANUPC_2Expand_Off://��ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x02);
		break;
	case EANUPC_2Expand_On://ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x02;
		break;
	case EANUPC_5Expand_Off://��ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x04);
		break;
	case EANUPC_5Expand_On://ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x04;
		break;
/*	case EANUPC_UPC_Pre0_Off://������ǰ���ַ�'0'
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x08);
		break;
	case EANUPC_UPC_Pre0_On://����ǰ���ַ�'0'
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x08;
		break;
	case EANUPC_UPCETo13_Off://���ѽ����չ��UPC-A
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x10);
		break;
	case EANUPC_UPCETo13_On://�ѽ����չ��UPC-A
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x10;
		break;*/
	case UPCE_Preamble_Off://������Preamble
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x08);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~BIT_UPCE_USCode);
		break;
	case UPCE_Preamble_SystemChar_On://����system character
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x08;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~BIT_UPCE_USCode);
		break;
	case UPCE_Preamble_ALL_On://UPCE����country code '0' + system character'0'
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= BIT_UPCE_USCode;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x08;
		break;

	case UPCE_ToUPCA_Off://����չ��UPC-A
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x10);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~BIT_UPCE_TDoUPCA);
		break;
	case UPCE_DataTo13_On://��data��չ��13λ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x10;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~BIT_UPCE_TDoUPCA);
		break;
	case UPCE_ToUPCA_On://UPC-Eת����UPC-A
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= BIT_UPCE_TDoUPCA;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x10);
		break;
	default: return FALSE;
	}
	return TRUE;
}

//UPC-A������
BOOL SetCode1D_UPCA(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] = Init_UPCA;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UPCA);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UPCA));
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UPCA);
		break;
	case EANUPC_SendCheck_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x01);
		break;
	case EANUPC_SendCheck_On://����У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= 0x01;
		break;
	case EANUPC_2Expand_Off://��ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x02);
		break;
	case EANUPC_2Expand_On://ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= 0x02;
		break;
	case EANUPC_5Expand_Off://��ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x04);
		break;
	case EANUPC_5Expand_On://ʶ����λ��չ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= 0x04;
		break;
/*	case EANUPC_UPC_Pre0_Off://������ǰ���ַ�'0'
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x08);
		break;
	case EANUPC_UPC_Pre0_On://����ǰ���ַ�'0'
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= 0x08;
		break;*/
	case UPCA_Preamble_Off://������Preamble
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x08);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= BIT_UPCA_CloseSysChar;
		break;
	case UPCA_Preamble_ALL_On://����country code + system character
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= 0x08;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~BIT_UPCA_CloseSysChar);
		break;

	case UPCA_Preamble_SystemChar_On://���� system character
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x08);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~BIT_UPCA_CloseSysChar);
		break;

	case UPCA_Coupon_Off://��ֹUPC-A/EAN-13+Coupon��չ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~(BIT_UPCA_Coupon|BIT_UPCA_REQCoupon));
		break;

	case UPCA_Coupon_Allow://����UPC-A/EAN-13+Coupon��չ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~BIT_UPCA_REQCoupon);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= BIT_UPCA_Coupon;
		break;

	case UPCA_Coupon_Require://����UPC-A/EAN-13+Coupon��չ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~BIT_UPCA_Coupon);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= BIT_UPCA_REQCoupon;
		break;

	case UPCA_Coupon_RSS_Output_Off:	//�������coupon��Ϣ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~BIT_GS1COUPON_ONLYGS1);
		break;

	case UPCA_Coupon_RSS_Output_On:	//ֻ���RSS����Ϣ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= BIT_GS1COUPON_ONLYGS1;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//INTER25������
BOOL SetCode1D_Inter2of5(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		//����25��Ĭ�������볤��80����С���볤��4
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = Init_INTER25;		
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_INTER2OF5);  //INTER2OF5��ʹ��
		memset(X_paramBakGetGlobalAllParam()->p1D->I2Of5Len, 0, 5);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ITF14);     //ITF14��ֹ
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ITF6);      //ITF6��ֹ
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_INTER2OF5));
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_INTER2OF5);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_INTER2OF5;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_INTER2OF5;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Inter2Of5_Check_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= (~0x03);
		break;
	case Inter2Of5_Check_Send_Off://����У�������������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & (~0x03)) | 0x01;
		break;
	case Inter2Of5_Check_Send_On://����У������Ҵ���У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= 0x03;
		break;
	case Inter2Of5_ITF14_Off://������ITF14����
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ITF14);
		break;
	case Inter2Of5_ITF14_Check_Off://����ITF14���룬��������У���
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF14);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= (~0x04);
		break;
	case Inter2Of5_ITF14_Check_On://����ITF14�����Ҵ���У���
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF14);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= 0x04;
		break;
	case Inter2Of5_ITF6_Off://������ITF6����
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ITF6);
		break;
	case Inter2Of5_ITF6_Check_Off://����ITF6���룬��������У���
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF6);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= (~0x08);
		break;
	case Inter2Of5_ITF6_Check_On://����ITF6�����Ҵ���У���
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF6);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= 0x08;
		break;
	case Inter2Of5_FixedLen_Off: //���ý���25��ֻʶ�������ر�
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= (~0x10);
		X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[0] = 0;
		break;
	case Inter2Of5_FixedLen_On:  //���ý���25��ֻʶ��������
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= 0x10;
		X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[0] = 4;
		break;
	case Inter2Of5_FixedLen_Ena: //���ý���25��ֻʶ�������ĳ���
		g_DataType = DataSetType_FixedLen;
		g_PosMaxMin = POS_INTER2OF5;
		g_DataSet = 0;
		g_nPreSuffix = 0;
		g_nFixedLen = 0;
		g_IsSetCodeStart = TRUE;
		memset(g_PreSuffix, 0, 20);
		break;
	case Inter2Of5_FixedLen_Dis: //ȡ������25��ֻʶ�������ĳ���
		g_DataType = DataSetType_Fixed_Dis;
		g_PosMaxMin = POS_INTER2OF5;
		g_DataSet = 0;
		g_nPreSuffix = 0;
		g_nFixedLen = 0;
		g_IsSetCodeStart = TRUE;
		memset(g_PreSuffix, 0, 20);
		break;
	default: return FALSE;
	}
	return TRUE;
}

//MATRIX25������(China Post 25)
BOOL SetCode1D_Matrix25(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] =Init_MATRIX25;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MATRIX25);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MATRIX25);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_MATRIX25);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_MATRIX25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_MATRIX25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Matrix25_Check_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] &= (~0x03);
		break;
	case Matrix25_Check_Send_Off://����У�鵫������У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] |= 0x02;
		break;
	case Matrix25_Check_Send_On://����У��ͬʱ�ش���У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] |= 0x03;
		break;
	default: return FALSE;
	}
    return TRUE;
}

//MODULE25������
BOOL SetCode1D_Module25(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] = Init_MODULE25;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MODULE25);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MODULE25);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_MODULE25);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_MODULE25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_MODULE25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Module25_Check_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] &= (~0x03);
		break;
	case Module25_Check_Send_Off://����У�鵫������У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] |= 0x01;
		break;
	case Module25_Check_Send_On://����У��ͬʱ�ش���У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] |= 0x03;
		break;
	default: return FALSE;
	}
    return TRUE;
}

//CODE39������
BOOL SetCode1D_Regular39(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] = Init_REGULAR39;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_REGULAR39);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_REGULAR39);
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_FULLASCII39);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_REGULAR39);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_REGULAR39;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_REGULAR39;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Code39_Check_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= (~0x03);
		break;
	case Code39_Check_Send_Off://����У�鵫������У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= 0x01;
		break;
	case Code39_Check_Send_On://����У��ͬʱ�ش���У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= 0x03;
		break;
	case Code39_StartStop_Off://��������ʼ������ֹ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= (~0x04);
		break;
	case Code39_StartStop_On://������ʼ������ֹ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= 0x04;
		break;
	case Code39_FullASCII_Off://��ʶ��ȫASCII39��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= (~0x08);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_FULLASCII39);
		break;
	case Code39_FullASCII_On://ʶ��ȫASCII39��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= 0x08;
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_REGULAR39);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_FULLASCII39);
		break;

	case Code39_Code32_Off://��ת��ΪCode32
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= ~BIT_Code32SpecEdit;
		break;
	case Code39_Code32_On://ת��ΪCode32
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= BIT_Code32SpecEdit;
		break;
	case Code39_Code32_Star_Off://Code32�������ʼ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= ~BIT_Code32PreCh;
		break;
	case Code39_Code32_Star_On://Code32�������ʼ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= BIT_Code32PreCh;
		break;

	default: return FALSE;
	}
	return TRUE;
}

//Codabar������
BOOL SetCode1D_Codabar(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] = Init_CODABAR;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODABAR);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODABAR);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODABAR);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CODABAR;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_CODABAR;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Codabar_Check_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= (~0x03);
		break;
	case Codabar_Check_Send_Off://����У�鵫������У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= 0x01;
		break;
	case Codabar_Check_Send_On://����У��ͬʱ�ش���У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= 0x03;
		break;
	case Codabar_StartStop_Off://��������ʼ������ֹ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= (~0x04);
		break;
	case Codabar_StartStop_On://������ʼ������ֹ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= 0x04;
		break;
	case Codabar_ABCDTNXE_Off://��ʼ������ֹ��Ϊ��ͨABCD��ʽ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= ~BIT_CODA_ABCDTNXE;
		break;
	case Codabar_ABCDTNXE_On://��ʼ������ֹ��ΪABCD/TN*E��ʽ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= BIT_CODA_ABCDTNXE;
		break;
	case Codabar_LOWERCASE_Off://��ʼ������ֹ��Ϊ��д
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= ~BIT_CODA_LOWERCASE;
		break;
	case Codabar_LOWERCASE_On://��ʼ������ֹ��ΪСд
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= BIT_CODA_LOWERCASE;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//Code93������
BOOL SetCode1D_Code93(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] = Init_CODE93;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODE93);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODE93);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODE93);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CODE93;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_CODE93;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Code93_Check_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] &= (~0x03);
		break;
	case Code93_Check_Send_Off://����У�鵫������У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] |= 0x01;
		break;
	case Code93_Check_Send_On://����У��ͬʱ�ش���У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] |= 0x03;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//CodeZ������
BOOL SetCode1D_CodeZ(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] = Init_CODEZ;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODEZ);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODEZ);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODEZ);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CODEZ;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_CODEZ;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case CodeZ_Error_Send_Off:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] &= (~0x01);
		break;
	case CodeZ_Error_Send_On:
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] |= 0x01;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//RSS������
BOOL SetCode1D_RSS(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_RSS] = Init_RSS;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (DC_RSS);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(DC_RSS);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (DC_RSS);
		break;
	case RSS_AI_Send_Off://AI�ַ�������
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_RSS] &= ~(0x1);
		break;
	case RSS_AI_Send_On://AI�ַ�����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_RSS] |= (0x1);
		break;
	default: return FALSE;
	}
	return TRUE;
}

//COMPOSITE��(UCC EAN������)����
BOOL SetCode1D_COMPOSITE(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_COMPOSITE] = Init_COMPOSITE;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_COMPOSITE);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_COMPOSITE);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_COMPOSITE);
		break;
	case COMPOSITE_UCCEAN_Off://UPC/EAN�汾��ֹ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_COMPOSITE] &= (~0x01);
		break;
	case COMPOSITE_UCCEAN_On://UPC/EAN�汾ʹ��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_COMPOSITE] |= 0x01;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//Code11������
BOOL SetCode1D_CODE11(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] = Init_CODE11;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODE11);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODE11);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODE11);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CODE11;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_CODE11;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case CODE11_No_Check://��У��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		break;
	case CODE11_C11Chk1://һλУ�飬MOD11
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x01;
		break;
	case CODE11_C11M11M11Chk://��λУ��MOD11/MOD11
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x02;
		break;
	case CODE11_C11M11M9Chk://��λУ��MOD11/MOD9
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x03;
		break;
	case CODE11_10char_C11M11M11Chk://MOD11��У��(Len<=10),MOD11/MOD11˫У��(Len>10)
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x04;
		break;
	case CODE11_10char_C11M11M9Chk://MOD11��У��(Len<=10),MOD11/MOD9˫У��(Len>1)
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x05;
		break;
	case CODE11_No_Send_Chk://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x08);
		break;
	case CODE11_Send_Chk://����У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x08;
		break;
	default: return FALSE;
	}
	return TRUE;
}


//ISBN������
BOOL SetCode1D_ISBN(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ISBN] = Init_ISBN;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ISBN);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ISBN);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ISBN);
		break;
	case ISBN_Send_13://����13�ֽ�
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ISBN] &= (~0x01);
		break;
	case ISBN_Send_10://����10�ֽ�
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ISBN] |= 0x01;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//INDUSTRIAL25������
BOOL SetCode1D_INDUSTRIAL25(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] = Init_Industrial25;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_INDUSTRIAL25);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_INDUSTRIAL25);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_INDUSTRIAL25);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_INDUSTRIAL25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_INDUSTRIAL25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case INDUSTRIAL25_No_CHK://��У��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] &= (~0x03);
		break;
	case INDUSTRIAL25_CHK_NO_Send://У��,������
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] |= 0x01;
		break;
	case INDUSTRIAL25_CHK_Send://У�鲢����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] |= 0x03;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//STANDARD25������
BOOL SetCode1D_STANDARD25(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] = Init_Standard25;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_STANDARD25);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_STANDARD25);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_STANDARD25);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_STANDARD25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_STANDARD25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case STANDARD25_No_CHK://��У��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] &= (~0x03);
		break;
	case STANDARD25_CHK_NO_Send://У��,������
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] |= 0x01;
		break;
	case STANDARD25_CHK_Send://У�鲢����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] |= 0x03;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//PLESSEY������
BOOL SetCode1D_PLESSEY(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] = Init_Plessey;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_PLESSEY);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_PLESSEY);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_PLESSEY);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_PLESSEY;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_PLESSEY;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case PLESSEY_No_CHK://��У��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] &= (~0x03);
		break;
	case PLESSEY_CHK_NO_Send://У��,������
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] |= 0x01;
		break;
	case PLESSEY_CHK_Send://У�鲢����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] |= 0x03;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//MSI_PLESSEY������
BOOL SetCode1D_MSIPLESSEY(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] = Init_MSIPlessey;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MSI_PLESSEY);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MSI_PLESSEY);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_MSI_PLESSEY);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_MSI_PLESSEY;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_MSI_PLESSEY;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case MSI_No_CHK://��У��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] &= (~0x03);
		break;
	case MSI_CHK_M10://��MOD10У��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] |= 0x01;
		break;
	case MSI_CHK_M10M10://˫MOD10У��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] |= 0x02;
		break;
	case MSI_CHK_M10M11://MOD10/MOD11˫У��
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] |= 0x03;
		break;
	case MSI_Check_Send_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] &= (~0x04);
		break;
	case MSI_Check_Send_On://����У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] |= 0x04;
		break;
	default: return FALSE;
	}
	return TRUE;
}

BOOL SetCode1D_ISSN(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ISSN] = Init_ISSN;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ISSN);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ISSN);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ISSN);
		break;
	default: return FALSE;
	}
	return TRUE;
}



//China Post 25������
BOOL SetCode1D_ChnPost25(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] =Init_CHNPOST25;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CHNPOST);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CHNPOST);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CHNPOST);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CHNPOST;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_CHNPOST;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case ChnPost25_Check_Off://������У���
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] &= (~0x03);
		break;
	case ChnPost25_Check_Send_Off://����У�鵫������У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] |= 0x01;
		break;
	case ChnPost25_Check_Send_On://����У��ͬʱ�ش���У����
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] |= 0x03;
		break;
	default: return FALSE;
	}

    return TRUE;
}


//��ά�������
BOOL SetCode2D_Macro(int Rec56)
{
	switch(Rec56){
	case Macro_Cancel: //ȡ����������Ϣ
		W_macroInitialParam();
		break;
	case Macro_DirectSend://ÿ�����ݿ����Ϣ��ȡ��ֱ�Ӵ���
		X_paramBakGetGlobalAllParam()->p2D->nMacro = Macro_DirectSend;
		break;
	case Macro_SendInTurn://��˳���ͣ����治����64kbyte����
		     //�����ȵ�ǰ���ݿ�����Ӻ�С�����ݿ鶼��ʶ��ʱ��
		     //����ЩĿǰ�Ѿ����Ӻõ����ݣ�������ǰ�飩����
		X_paramBakGetGlobalAllParam()->p2D->nMacro = Macro_SendInTurn;
		break;
	case Macro_LinkSendAfterRead://����ȫ�������ݿ�������ӷ��͡�
		         //����������������64kʱ����ģʽ2�ķ�ʽ��������
		X_paramBakGetGlobalAllParam()->p2D->nMacro = Macro_LinkSendAfterRead;
		break;
	
	default: return FALSE;
	}
	return TRUE;
}

//PDF417������
BOOL SetCode2D_PDF417(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_PDF417);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] = Init_2D_PDF417_MIN_MAX_LEN;  //PDFĬ�����2710����С1
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] = Init_PDF417;
            X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_PDF417] = 0;
// #if defined(DC_PDF417_DOUBLE)
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_DOUBLE);
// #endif
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_PDF417);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_PDF417);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_PDF417; //PDF417���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_PDF417; //PDF417���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
#if defined(DC_PDF417_DOUBLE)
	case PDF417_Double_OnlyOne:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_DOUBLE);
		break;
	case PDF417_Double_OnlyTwo:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_DOUBLE);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (1<<POS_PDF417_DOUBLE0);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (1<<POS_PDF417_DOUBLE1);
		break;
	case PDF417_Double_OneOrTwo:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_DOUBLE);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (1<<POS_PDF417_DOUBLE1);
		break;
#endif

#if defined(DC_PDF417_Reverse)
	case PDF417_Reverse: //����������
		if(!Rec7) X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_Reverse);
		else if(Rec7 == 1){
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_Reverse);
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (1 << POS_PDF417_Reverse0);
		}
		else if(Rec7 == 2){
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= DC_PDF417_Reverse;
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~(1<<POS_PDF417_Reverse0));
		}
		else return FALSE;
		break;
#endif

#if defined(DC_PDF417_Mirror)
	case PDF417_Mirror:
		if(!Rec7) X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_Mirror);
		else if(Rec7 == 1)
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (DC_PDF417_Mirror);
		else return FALSE;
		break;
#endif
      case PDF417_ECI:
		if(!Rec7) X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_ECICLOSE);	//off
		else if(Rec7 == 1)
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] |= (DC_PDF417_ECICLOSE);	//on
		else return FALSE;

		break;

      case PDF417_CharEnCoding:
		if(!Rec7) 		X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_PDF417] = 0;
		else if(Rec7 == 1)
			X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_PDF417] = 1;
		else return FALSE;

		break;
	default : return FALSE;
	}
	return TRUE;
}

//QR������
BOOL SetCode2D_QR(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_QR);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_QR] = Init_2D_QRCODE_MIN_MAX_LEN;  //QRĬ�����7089����С1
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] = Init_QR;
// #if defined(DC_QR_DOUBLE)
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~DC_QR_DOUBLE);
// #endif
             X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_QR] = 0;
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_QR);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_QR);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_QR; //QR���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_QR; //QR���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
#if defined(DC_QR_DOUBLE)
	case QR_Double_OnlyOne:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~DC_QR_DOUBLE);
		break;
	case QR_Double_OnlyTwo:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] |= (1<<POS_QR_DOUBLE0);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~(1<<POS_QR_DOUBLE1));
		break;
	case QR_Double_OneOrTwo:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] |= DC_QR_DOUBLE;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~(1<<POS_QR_DOUBLE0));
		break;
#endif
	case MicroQR_On:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~(1<<POS_QR_MicroQR_ON));
		break;
	case MicroQR_Off:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] |= (1<<POS_QR_MicroQR_ON);
		break;
      case QR_ECI:
		if(!Rec7) X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~DC_QR_ECICLOSE);	//off
		else if(Rec7 == 1)
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] |= (DC_QR_ECICLOSE);	//on
		else return FALSE;
        break;
        case QR_CharEncoding:
		if(!Rec7) 		X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_QR] = 0;
		else if(Rec7 == 1)
			X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_QR] = 1;
		else return FALSE;
		break;
	default :return FALSE;
	}
	return TRUE;
}

//DataMatrix������
BOOL SetCode2D_DataMatrix(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_DataMatrix);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] = Init_2D_DataMatrix_MIN_MAX_LEN;  //DataMatrixĬ�����3116����С1
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] = Init_DataMatrix;
             X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_DataMatrix] = 0;
// #if defined(DC_DM_129_ENABLE)
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_129_ENABLE);
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_DOUBLE);
// #endif
// 
// #if defined(DC_DM_DISABLE_RECT)
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_DISABLE_RECT);
// #endif
// 
// #if defined(DC_DM_ADD_PAD_CODE)
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_ADD_PAD_CODE);
// #endif
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= (~(0x01 << POS_DataMatrix));
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_DataMatrix);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_DataMatrix; //Data Matrix���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_DataMatrix; //Data Matrix���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
#if defined(DC_DM_129_ENABLE)
	case DataMatrix_129_Off: // DM 129 FOR ECS ��Ŀ�ر�
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_129_ENABLE);
		break;
	case DataMatrix_129_On: // DM 129 FOR ECS ��Ŀ����
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= DC_DM_129_ENABLE;
		break;
#endif

#if defined(DC_DM_DOUBLE)
	case DataMatrix_Double_OnlyOne:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_DOUBLE);
		break;
	case DataMatrix_Double_OnlyTwo:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= (1<<POS_DM_DOUBLE0);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~(1<<POS_DM_DOUBLE1));
		break;
	case DataMatrix_Double_OneOrTwo:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= DC_DM_DOUBLE;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~(1<<POS_DM_DOUBLE0));
		break;
#endif

#if defined(DC_DM_DISABLE_RECT)	//Ĭ�Ͽ�������λ��ر�
	case DataMatrix_Rectangular_Off: //��ʶ�������
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= DC_DM_DISABLE_RECT;
		break;
	case DataMatrix_Rectangular_On:  //ʶ�������
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_DISABLE_RECT);
		break;
#endif

#if defined(DC_DM_ADD_PAD_CODE)
	case DataMatrix_AddPADCode_Off: //��Ϣ�����PAD��������
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_ADD_PAD_CODE);
		break;
	case DataMatrix_AddPADCode_On:  //��Ϣ�����PAD��������
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= DC_DM_ADD_PAD_CODE;
		break;
#endif

#if defined(DC_DM_Reverse)
	case DataMatrix_Reverse: //����������
		if(!Rec7) X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_Reverse);
		else if(Rec7 == 1){
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_Reverse);
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= (1 << POS_DM_Reverse0);
		}
		else if(Rec7 == 2){
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= DC_DM_Reverse;
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~(1<<POS_DM_Reverse0));
		}
		break;
#endif

#if defined(DC_DM_Mirror)
	case DataMatrix_Mirror:
		if(!Rec7) X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_Mirror);
		else if(Rec7 == 1)
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= (DC_DM_Mirror);
		else return FALSE;
		break;
#endif

	case DataMatrix_ECI:
		if(!Rec7) X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_ECICLOSE);
		else if(Rec7 == 1)
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= (DC_DM_ECICLOSE);
		else return FALSE;
		break;
	case DataMatrix_CharEnCoding:
		if(!Rec7) 		X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_DataMatrix] = 0;
		else if(Rec7 == 1)
			X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_DataMatrix] = 1;
		else return FALSE;
		break;	
	default :return FALSE;
	}
	return TRUE;
}

//Aztec������
BOOL SetCode2D_Aztec(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_Aztec);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] = Init_2D_AZTEC_MIN_MAX_LEN;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] = Init_Aztec;
             X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_Aztec] = 0;
// #if  defined(DC_AZTEC_MULTINUM)
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTINUM);
// #endif
	    break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_Aztec);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_Aztec);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_Aztec; //Aztec���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_Aztec; //Aztec���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
		
#if  defined(DC_AZTEC_MULTINUM)
	case Aztec_Multi_Num: //���ö���ͬͼ����������
		if(Rec7 > 7) return FALSE;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTINUM);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= (Rec7);
		break;
	case Aztec_Multi_None: //���ö���ͬͼ��ֹ
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
		break;
	case Aztec_Multi_Only: //����ֻ�����
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= (1<<POS_AZTEC_MULTIEN0);
		break;
	case Aztec_Multi_Mixed: //���ý���루���ȴ�������
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= (1<<POS_AZTEC_MULTIEN1);
		break;
#endif

      case Aztec_ECI:
		if(!Rec7) X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_ECICLOSE);
		else if(Rec7 == 1)
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= (DC_AZTEC_ECICLOSE);
		else return FALSE;
		break;
      case Aztec_CharEnCoding:
		if(!Rec7) 		X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_Aztec] = 0;
		else if(Rec7 == 1)
			X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_Aztec] = 1;
		else return FALSE;
		break;

	default :return FALSE;
	}
	return TRUE;
}

//MaxiCode������
BOOL SetCode2D_MaxiCode(int Rec56)
{
	switch(Rec56){
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_MaxiCode);
	    X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] = Init_2D_MaxiCode_MIN_MAX_LEN;
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_MaxiCode);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_MaxiCode);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_MaxiCode; //MaxiCode���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_MaxiCode; //MaxiCode���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	default :return FALSE;
	}
	return TRUE;
}

//VeriCode������
BOOL SetCode2D_VeriCode(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_VeriCode);
	    X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] = Init_2D_VeriCode_MIN_MAX_LEN;  //VeriCodeĬ�����1500����С1
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] = Init_VeriCode;
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] &= (~0x0f);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_VeriCode);
		break;
	case Set_On://ʹ��
	    X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_VeriCode);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_VeriCode; //VeriCode���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_VeriCode; //VeriCode���λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case VeriCode_UnZip_ModeUp: //��ѹ��ģʽ��
		if(Rec7 > 7) return FALSE;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] &= (~0x0f);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] |= Rec7;
		break;
	case VeriCode_UnZip_ModeDown://��ѹ��ģʽ��
		if(Rec7 > 7) return FALSE;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] &= (~0x0f);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] |= Rec7 + 8;
		break;
	default : return FALSE;
	}
	return TRUE;
}

//����������
BOOL SetCode2D_Han(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_HAN_SENSIBLE);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] = Init_2D_HANSENSIBLE_MIN_MAX_LEN;  //������Ĭ�����7827����С1
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] = Init_CSC;
	    break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_HAN_SENSIBLE);
		break;
	case Set_On://ʹ��
	    X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_HAN_SENSIBLE);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_HAN_SENSIBLE; //�������λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_HAN_SENSIBLE; //�������λ��
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	
#if defined(DC_CSC_DOUBLE)
	case HAN_SENSIBLE_Double_OnlyOne:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] &= (~DC_CSC_DOUBLE);
		break;
	case HAN_SENSIBLE_Double_OnlyTwo:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] |= (1<<POS_CSC_DOUBLE0);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] &= (~(1<<POS_CSC_DOUBLE1));
		break;
	case HAN_SENSIBLE_Double_OneOrTwo:
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] |= DC_CSC_DOUBLE;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] &= (~(1<<POS_CSC_DOUBLE0));
		break;
#endif
		
		
#if defined(DC_CSC_Reverse)
	case HAN_SENSIBLE_Reverse: //����������
		if(!Rec7) X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] &= (~DC_DM_Reverse);
		else if(Rec7 == 1){
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] &= (~DC_DM_Reverse);
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] |= (1 << POS_DM_Reverse0);
		}
		else if(Rec7 == 2){
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] |= DC_DM_Reverse;
			X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] &= (~(1<<POS_DM_Reverse0));
		}
		break;
#endif


	default : return FALSE;
	}

	return TRUE;
	Rec7 = Rec7;
}

//�û��Զ����ά������
BOOL SetCode2D_UserDef(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_USER_2D_CODE);
	    break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_USER_2D_CODE);
		break;
	case Set_On://ʹ��
	    X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_USER_2D_CODE);
		break;
	default : return FALSE;
	}

	return TRUE;
	Rec7 = Rec7;
}


//SPEC_OCR_B������
BOOL SetOCR_SPEC_OCR_B(int Rec56)
{
	switch(Rec56){
	case Set_Default://���ø������������ΪĬ��ֵ
#if defined(SAGEM)
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes |= (0x01 << POS_SPEC_OCR_B);
#else   
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes &= ~(0x01 << POS_SPEC_OCR_B);
#endif		
		
		X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = Init_SPEC_OCR_B ; //Ĭ��ֻ������Ϊ9�� 
	    break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes &= ~(0x01 << POS_SPEC_OCR_B);
		break;
	case Set_On://ʹ��
	    X_paramBakGetGlobalAllParam()->pOCR->unDecTypes |= (0x01 << POS_SPEC_OCR_B);
		break;

	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_OCRMin;
		g_PosMaxMin = POS_SPEC_OCR_B;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_OCRMax;
		g_PosMaxMin = POS_SPEC_OCR_B;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	default : return FALSE;
	}
	return TRUE;
}


BOOL SetOCR_PASSPORT_VISAS(int Rec56)
{
	switch(Rec56){
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes &= ~(0x01 << POS_PASSPORT_VISAS);
		X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_PASSPORT_VISAS] = Init_PASSPORT_VISAS ;  
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes &= ~(0x01 << POS_PASSPORT_VISAS);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes |= (0x01 << POS_PASSPORT_VISAS);
		break;

	default : return FALSE;
	}
	return TRUE;
}


BOOL SetCode1D_UCCEAN128(int Rec56)
{
	switch(Rec56) {
	case Set_Default://���ø������������ΪĬ��ֵ
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] = Init_UCCEAN128;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
		break;
	case Set_Off://��ֹ
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_UCCEAN128);
		break;
	case Set_On://ʹ��
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
		break;
	case Set_MinLen://���ö�����С����
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_UCCEAN128;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://���ö�����󳤶�
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_UCCEAN128;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//GM������
BOOL SetCode2D_GM(int Rec56)
{
	switch(Rec56)
	{
	case Set_Default:
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_GM_CODE);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_GM_CODE] = Init_2D_GMCODE_MIN_MAX_LEN;  
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_GM_CODE] = Init_GM;
		break;
	case Set_Off:
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_GM_CODE);
		break;
	case Set_On:
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_GM_CODE);
		break;
	case Set_MinLen:
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_GM_CODE;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen:
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_GM_CODE; 
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	default : return FALSE;
	}
	return TRUE;
}

//MicroPDF417������
BOOL SetCode2D_MicroPDF417(int Rec56)
{
	switch(Rec56)
	{
	case Set_Default:
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_MicroPDF417);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_MicroPDF417] = Init_2D_MicroPDF417_MIN_MAX_LEN;  
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_MicroPDF417] = Init_MicroPDF417;
		break;
	case Set_Off:
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_MicroPDF417);
		break;
	case Set_On:
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_MicroPDF417);
		break;
	case Set_MinLen:
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_MicroPDF417;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen:
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_MicroPDF417; 
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	default : return FALSE;
	}
	return TRUE;
}

//һά��������
BOOL Set_Code1D(int Rec34, int Rec56)
{
	switch(Rec34){
	case Set_Code1D_Code128://CODE128������
		if(!SetCode1D_Code128(Rec56)) return FALSE;
		break;
	case Set_Code1D_EAN8://EAN-8������
		if(!SetCode1D_EAN8(Rec56)) return FALSE;
		break;
	case Set_Code1D_EAN13://EAN-13������
		if(!SetCode1D_EAN13(Rec56)) return FALSE;
  	    break;
	case Set_Code1D_UPCE://UPC-E������
		if(!SetCode1D_UPCE(Rec56)) return FALSE;
		break;
	case Set_Code1D_UPCA://UPC-A����
		if(!SetCode1D_UPCA(Rec56)) return FALSE;
		break;
	case Set_Code1D_Inter2Of5://INTER2OF5������,������ITF14��ITF6
		if(!SetCode1D_Inter2of5(Rec56)) return FALSE;
		break;
	case Set_Code1D_Matrix25://MATRIX25������
		if(!SetCode1D_Matrix25(Rec56)) return FALSE;
		break;
	case Set_Code1D_Module25://MODULE25������
	    if(!SetCode1D_Module25(Rec56)) return FALSE;
		break;
	case Set_Code1D_Regular39://REGULAR39������
		if(!SetCode1D_Regular39(Rec56)) return FALSE;
		break;
	case Set_Code1D_Codabar://CODABAR������
		if(!SetCode1D_Codabar(Rec56)) return FALSE;
		break;
	case Set_Code1D_Code93: //Code93������
		if(!SetCode1D_Code93(Rec56)) return FALSE;
		break;
	case Set_Code1D_CodeZ: //CodeZ������
		if(!SetCode1D_CodeZ(Rec56)) return FALSE;
		break;
	case Set_Code1D_UCCEAN128:	//UCCEAN128������
		if(!SetCode1D_UCCEAN128(Rec56)) return FALSE;
		break;
	case Set_Code1D_RSS:
		if(!SetCode1D_RSS(Rec56)) return FALSE;
		break;
	case Set_Code1D_COMPOSITE:
		if(!SetCode1D_COMPOSITE(Rec56)) return FALSE;
		break;
	case Set_Code1D_CODE11:
		if(!SetCode1D_CODE11(Rec56)) return FALSE;
		break;
	case Set_Code1D_ISBN:
		if(!SetCode1D_ISBN(Rec56)) return FALSE;
		break;
	case Set_Code1D_INDUSTRIAL25:
		if(!SetCode1D_INDUSTRIAL25(Rec56)) return FALSE;
		break;
	case Set_Code1D_STANDARD25:
		if(!SetCode1D_STANDARD25(Rec56)) return FALSE;
		break;
	case Set_Code1D_PLESSEY:
		if(!SetCode1D_PLESSEY(Rec56)) return FALSE;
		break;
	case Set_Code1D_MSI_PLESSEY:
		if(!SetCode1D_MSIPLESSEY(Rec56)) return FALSE;
		break;
	case Set_Code1D_ISSN:
		if(!SetCode1D_ISSN(Rec56)) return FALSE;
		break;

	case Set_Code1D_ChnPost25:
		if(!SetCode1D_ChnPost25(Rec56)) return FALSE;
		break;
		
	default: return FALSE;
	}

	return TRUE;
}

//��ά��������
BOOL Set_Code2D(int Rec34, int Rec56, int Rec7)
{
	switch(Rec34){
	case POS_Macro://������
		if(!SetCode2D_Macro(Rec56)) return FALSE;
		break;

	//����ǰ����˺����ã����Ը������λ��ƫ����һλ
	case POS_PDF417 + 1://PDF417������
		if(!SetCode2D_PDF417(Rec56, Rec7)) return FALSE;
		break;
	case POS_QR + 1://QR������
		if(!SetCode2D_QR(Rec56, Rec7)) return FALSE;
		break;
	case POS_Aztec + 1://Aztec������
		if(!SetCode2D_Aztec(Rec56, Rec7)) return FALSE;
		break;
	case POS_DataMatrix + 1://Data Matrix������
		if(!SetCode2D_DataMatrix(Rec56, Rec7)) return FALSE;
		break;
	case POS_MaxiCode + 1://MaxiCode������
		if(!SetCode2D_MaxiCode(Rec56)) return FALSE;
		break;
	case POS_VeriCode + 1 ://VeriCode������
		if(!SetCode2D_VeriCode(Rec56, Rec7)) return FALSE;
		break;
	case POS_HAN_SENSIBLE + 1: //����������
		if(!SetCode2D_Han(Rec56, Rec7)) return FALSE;
		break;
	case POS_GM_CODE + 1:
		if(!SetCode2D_GM(Rec56)) return FALSE;
		break;
	case POS_USER_2D_CODE + 1: //�û��Զ����ά������
		if(!SetCode2D_UserDef(Rec56, Rec7)) return FALSE;
		break;
	case POS_MicroPDF417 + 1:
		if(!SetCode2D_MicroPDF417(Rec56)) return FALSE;
		break;
	default: return FALSE;
	}
	return TRUE;
}

BOOL SetCodePos_Postnet(int Rec56)
{
	switch(Rec56)
	{
	case Set_Default:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_POSTNET);
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_POSTNET] = Init_POSTNET;
		break;
	case Set_Off:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_POSTNET);
		break;
	case Set_On:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes |= (0x01 << POS_POSTNET);
		break;

	case 3://������У���
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_POSTNET] &= (~BIT_POSTNET_TRMTCHK);
		break;
	case 4://����У���
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_POSTNET] |= (BIT_POSTNET_TRMTCHK);
		break;
	}	

	return TRUE;
}

BOOL SetCodePos_Onecode(int Rec56)
{
	switch(Rec56)
	{
	case Set_Default:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_ONECODE);
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_ONECODE] = Init_ONECODE;
		break;
	case Set_Off:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_ONECODE);
		break;
	case Set_On:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes |= (0x01 << POS_ONECODE);
		break;
	}
	return TRUE;
}

BOOL SetCodePos_RM4CC(int Rec56)
{
	switch(Rec56)
	{
	case Set_Default:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_RM4SCC);
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_RM4SCC] = Init_RM4SCC;
		break;
	case Set_Off:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_RM4SCC);
		break;
	case Set_On:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes |= (0x01 << POS_RM4SCC);
		break;
	}
	return TRUE;
}

BOOL SetCodePos_Planet(int Rec56)
{
	switch(Rec56)
	{
	case Set_Default:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_PLANET);
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_PLANET] = Init_PLANET;
		break;
	case Set_Off:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_PLANET);
		break;
	case Set_On:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes |= (0x01 << POS_PLANET);
		break;

	case 3://������У���
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_PLANET] &= (~BIT_PLANET_TRMTCHK);
		break;
	case 4://����У���
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_PLANET] |= (BIT_PLANET_TRMTCHK);
		break;

	}

	return TRUE;
}

BOOL SetCodePos_Kixcode(int Rec56)
{
	switch(Rec56)
	{
	case Set_Default:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_KIXCODE);
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_KIXCODE] = Init_KIXCODE;
		break;
	case Set_Off:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_KIXCODE);
		break;
	case Set_On:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes |= (0x01 << POS_KIXCODE);
		break;
	}
	return TRUE;
}

BOOL SetCodePos_Auspost(int Rec56)
{
	switch(Rec56)
	{
	case Set_Default:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_AUSPOST);
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_AUSPOST] = Init_AUSPOST;
		break;
	case Set_Off:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes &= ~(0x01 << POS_AUSPOST);
		break;
	case Set_On:
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes |= (0x01 << POS_AUSPOST);
		break;
	}
	return TRUE;
}

BOOL Set_Postal(int Rec34, int Rec56)
{
	Rec34 -= 40;
	switch(Rec34){
	case POS_POSTNET:           
		if(!SetCodePos_Postnet(Rec56)) return FALSE;
		break;
	case POS_ONECODE:
		if(!SetCodePos_Onecode(Rec56)) return FALSE;
		break;
	case POS_RM4SCC:
		if(!SetCodePos_RM4CC(Rec56)) return FALSE;
		break;
	case POS_PLANET:
		if(!SetCodePos_Planet(Rec56)) return FALSE;
		break;
	case POS_KIXCODE:
		if(!SetCodePos_Kixcode(Rec56)) return FALSE;
		break;
	case POS_AUSPOST:
		if(!SetCodePos_Auspost(Rec56)) return FALSE;
		break;
	}
	return TRUE;
}

//OCR����
BOOL Set_OCR(int Rec34, int Rec56)
{
	switch(Rec34){
	case POS_SPEC_OCR_B ://SPEC_OCR_B����
		if(!SetOCR_SPEC_OCR_B(Rec56)) return FALSE;
		break;

	case POS_PASSPORT_VISAS:
		if (!SetOCR_PASSPORT_VISAS(Rec56)) return FALSE;
		break;

	default: return FALSE;
	}
	return TRUE;
}

//�û���������
BOOL Set_UserType(int Rec34, int Rec56)
{
	switch(Rec34){
	case POS_UserType_Group1 ://��һ��
// 		if(Rec56 > UserType_ECS_SAMSUNG) return FALSE;
// 		else X_paramGetGlobalAllParam()->pCode->nUserDef = Rec56;
// 		
// 		//����Ϊ�������޷����õ�һЩ����������
// 		if(X_paramGetGlobalAllParam()->pCode->nUserDef == UserType_NL_COMP_CQ)
// 		{
// //			g_bEnPort = FALSE;
// 		}
// 		else if(X_paramGetGlobalAllParam()->pCode->nUserDef == UserType_USA_DOUBLE_1D){
// 			//I2of5ֻʶ��˫�룬������ֻʶ����
// 			X_paramGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
// 	        X_paramGetGlobalAllParam()->p1D->unDecParam |= 0x04; //�����ڸ�λ����
// 		}
		
		break;

	default:
		return FALSE;
	}
	return TRUE;
	Rec56 = Rec56;
}

//��ά����Ǳ��������
BOOL Set_2DNonStandar(int Rec34, int Rec56)
{

	switch(Rec34){
	case POS_PDF417:
		break;
	case POS_QR ://QR�Ǳ��������
		if(!Set_QRNonStandar(Rec56)) return FALSE;
		break;
	case POS_Aztec:
		break;
	case POS_DataMatrix:
		break;
	default: return FALSE;
	}
	return TRUE;
}

//QR��Ǳ��������
BOOL Set_QRNonStandar(int Rec56)
{
	switch(Rec56){
	case 0://ֵ����
		g_DataType = DataSetType_NonStandar;
		g_PosMaxMin = POS_QR;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case 1: //Ĭ��
		X_paramBakGetGlobalAllParam()->p2D->nQRParam = 0;
		break;
	case 2:
		X_paramBakGetGlobalAllParam()->p2D->nQRParam = 254+(251<<8);
		break;
	case 3:
		X_paramBakGetGlobalAllParam()->p2D->nQRParam = 254+(252<<8);
	    break;
	case 4:
		X_paramBakGetGlobalAllParam()->p2D->nQRParam = 255;
	    break;
	default : return FALSE;
	}
	
	return TRUE;
}
//end add by HUNTON,2004.04.07

void SetCode_vParam2Buff(char* buff,const char* pInfo,int nParam,int nType)
{
	switch(nType){
	case UT_TYPE_1D:
	case UT_TYPE_OCR:
		sprintf(buff,pInfo,(nParam&0x00ff0000)>>16,(nParam&0xff000000)>>24);
		break;
	case UT_TYPE_2D:
		sprintf(buff,pInfo,(nParam&0x0000ffff),nParam>>16);
		break;
	default:
		break;
	}
}


void SetCode_vITF25Param2Buff_Mini(int v1,int v2, char* buff)
{
	int i;
	for(i = v1; i < v2; i+=2){
		if(i < 18 && (X_paramGetGlobalAllParam()->p1D->I2Of5Len[1] & (0x01 << (i/2 - 1))))
	        sprintf(buff+strlen(buff),"%d  ",i);
		else if(i < 34 && (X_paramGetGlobalAllParam()->p1D->I2Of5Len[2] & (0x01 << (i/2 - 9))))
	        sprintf(buff+strlen(buff),"%d  ",i);
		else if(i < 50 && (X_paramGetGlobalAllParam()->p1D->I2Of5Len[3] & (0x01 << (i/2 - 17))))
	        sprintf(buff+strlen(buff),"%d  ",i);
		else if(i < 66 && (X_paramGetGlobalAllParam()->p1D->I2Of5Len[4] & (0x01 << (i/2 - 25))))
	        sprintf(buff+strlen(buff),"%d  ",i);
	}
}

void SetCode_vITF25Param2Buff(char* buff)
{
	if (X_paramGetGlobalAllParam()->p1D->I2Of5Len[0] == 1){
		SetCode_vITF25Param2Buff_Mini(2,66,buff);
		if(X_paramGetGlobalAllParam()->p1D->I2Of5Len[1] == 0 
			&& X_paramGetGlobalAllParam()->p1D->I2Of5Len[2] == 0 
			&& X_paramGetGlobalAllParam()->p1D->I2Of5Len[3] == 0 
			&& X_paramGetGlobalAllParam()->p1D->I2Of5Len[4] == 0)
			sprintf(buff+strlen(buff),"All Length Disable)\r\n");
		else sprintf(buff+strlen(buff),"digits)\r\n");
	}
	else {
		SetCode_vParam2Buff(buff,"%d -> %d digits\r\n",X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5],UT_TYPE_1D);
	}
}


//��ͨѶ��������
BOOL Set_NewComm(int Rec34, int Rec56, int Rec7)
{
	switch(Rec34) {
	case Comm_Interface: //�ӿ���������
		if (Rec56 == U_PRO_IS_Storage)
		{
		}
		else
		{
			if(Rec56 <= U_PRO_IS_CDC)
			{ X_paramBakGetGlobalAllParam()->pHard->InterType = Rec56; }
		}

		break;
	case Comm_RS232:
//		if(!Set_NewCommRS232(Rec56, Rec7)) return FALSE;
		break;
	case Comm_USB:
		//if(!Set_NewCommUSB(Rec56, Rec7)) return FALSE;
		break;
	case Comm_KBW:
		if(!Set_NewCommKBW(Rec56, Rec7)) return FALSE;
		break;
	case Comm_NetCard:
		//if(!Set_NewCommNetCard(Rec56, Rec7)) return FALSE;
		break;
	case Comm_BlueTooth:
		if(!Set_NewCommBlueT(Rec56, Rec7)) return FALSE;
		break;
	case Comm_PS2:
		if(!Set_NewCommPS2(Rec56, Rec7)) return FALSE;
		break;
	case Comm_HID_POS:
		break;
	default: return FALSE;
	}

	return TRUE;
}

//KBW�ڲ�������
BOOL Set_NewCommKBW(int Rec56, int Rec7)
{
	switch(Rec56) {
	case KBW_Nationality: //������������
		g_DataType = DataSetType_KBW;
		g_PosMaxMin = DataSetKBW_Nationality;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case KBW_CapsLock_Off: //�رմ�д��
		X_paramBakGetGlobalAllParam()->pKBW->isCapsLock = FALSE;
		break;
	case KBW_CapsLock_On: //�򿪴�д��
		X_paramBakGetGlobalAllParam()->pKBW->isCapsLock =TRUE;
		break;
	case KBW_PromptUnknowChar: //��Ч�ַ���ʾ����
		if(Rec7 < 2) X_paramBakGetGlobalAllParam()->pKBW->isUnknownCharactersBeep = Rec7;
		else return FALSE;
		break;
	case KBW_CovertCase: //��Сд�ַ�ת������
		if(Rec7 < 3) X_paramBakGetGlobalAllParam()->pKBW->isConvertCase = Rec7;
		break;
	case KBW_KeystrokeDelay: //�����ӳ�����
		if(Rec7 < 3){
			X_paramBakGetGlobalAllParam()->pKBW->isKeysrockeDelay = Rec7;
		}
		else return FALSE;
		break;
	case KBW_KeypadEmulation: //Alt+ASCIIģʽ����
		if(Rec7 < 2) X_paramBakGetGlobalAllParam()->pKBW->isEmulateKeypad = Rec7;
		else return FALSE;
		break;
	case KBW_ShiftLock_Off: //
		X_paramBakGetGlobalAllParam()->pKBW->isShiftLock = FALSE;
		break;
	case KBW_ShiftLock_On:
		X_paramBakGetGlobalAllParam()->pKBW->isShiftLock = TRUE;
		break;
//	case KBW_AutoCapsLock_Off:
//		X_paramGetGlobalAllParam()->pKBW->isAutoCaps = FALSE;
//		break;
//	case KBW_AutoCapsLock_On:
//		X_paramGetGlobalAllParam()->pKBW->isAutoCaps = TRUE;
//		break;
	case KBW_AutoNumLock_Off:
		X_paramBakGetGlobalAllParam()->pKBW->isNumericMode = FALSE;
		break;
	case KBW_AutoNumLock_On:
		X_paramBakGetGlobalAllParam()->pKBW->isNumericMode = TRUE;
		break;
	case KBW_CtrlASCIIMode_Off:
		X_paramBakGetGlobalAllParam()->pKBW->isCtrlASCIIMode = FALSE;
		break;
	case KBW_CtrlASCIIMode_On:
		X_paramBakGetGlobalAllParam()->pKBW->isCtrlASCIIMode = TRUE;
		break; 
	case KBW_KEYPAD:
		if((Rec7==0)||(Rec7==1))
			X_paramBakGetGlobalAllParam()->pKBW->isKeyPad = Rec7;//Rec7=0�ر�С���̣�Rec7=1��С����
		else
			return FALSE;
		break;
	case KBW_FastMode: 
		if((Rec7==0)||(Rec7==1))
			X_paramBakGetGlobalAllParam()->pKBW->isFasttransferMode = Rec7;   //Rec7=0�رտ���ģʽ��Rec7=1��������ģʽ
		else
			return FALSE;
		break;

	case KBW_PollRate: 
		if(Rec7 < 10)
			X_paramBakGetGlobalAllParam()->pKBW->nPollRate = Rec7+1;   //1 - 10ms
		else
			return FALSE;
		break;

	case KBW_CodePage: 
		g_DataType = DataSetType_KBW;
		g_PosMaxMin = DataSetKBW_CodePage;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
		

	default: return FALSE;
	}

	return TRUE;
	Rec7 = Rec7;
}

//������������
BOOL Set_NewCommBlueT(int Rec56, int Rec7)
{
	switch(Rec56) {
	case BlueT_Reset: //������λ����
		BlueT_reset_cfg();//��λ���е�����
		break;
	case BlueT_Name: //�����豸��������
		g_DataType = DataSetType_BlueT;
		g_PosMaxMin = DataSetBlueT_Name;
		memset(g_PreSuffix, 0, 20);
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		g_IsSetCodeStart = TRUE;
		break;
	case BlueT_PassWord: //����
		if(Rec7 == 0 || Rec7 == 1){ //����У��ʹ������
			X_paramGetBlueT()->isPSWCheck = Rec7;
			BlueT_set_auth_status(Rec7);//�����Ƿ��������ƥ��
		}
		else if(Rec7 == 2){
			g_DataType = DataSetType_BlueT;
			g_PosMaxMin = DataSetBlueT_Password;
			memset(g_PreSuffix, 0, 20);
			g_nPreSuffix = 0;
		    g_DataSet_Odd = TRUE;
		    g_IsSetCodeStart = TRUE;
		}
		break;
	default: return FALSE;
	}

	return TRUE;
}


//PS2��������
BOOL Set_NewCommPS2(int Rec56, int Rec7)
{
	switch(Rec56) {
//	case PS2_Nationality: //������������
//		g_DataType = DataSetType_PS2;
//		g_PosMaxMin = DataSetPS2_Nationality;
//		g_DataSet = 0;
//		g_IsSetCodeStart = TRUE;
//		break;
// 	case PS2_CapsLockDef: //��д��Ĭ������
// 		if(Rec7 == 0 || Rec7 == 1) 
// 			X_paramGetGlobalAllParam()->pKBW->isPS2CapsLockDef = Rec7;
// 		else return FALSE;
// 		break;
	default: return FALSE;
	}

	return TRUE;
}


//KBW����
BOOL SetCode_Save_KBW(void)
{
	switch(g_PosMaxMin) {
	case DataSetKBW_Nationality: //���̹�����������
		X_paramBakGetGlobalAllParam()->pKBW->nLanguage = g_DataSet;
		break;
	case DataSetKBW_CodePage: //code page
		X_paramBakGetGlobalAllParam()->pKBW->nCodePage = g_DataSet;
		break;
	default: return FALSE;
	}

	return TRUE;
}
	
//������������
BOOL SetCode_Save_BlueT(void)
{
	switch(g_PosMaxMin) {
	case DataSetBlueT_Name: //�豸��������
		if(!g_DataSet_Odd)	{ return FALSE; }
		if(g_nPreSuffix > 16) return FALSE;
		memset(X_paramGetBlueT()->Name, 0, sizeof(X_paramGetBlueT()->Name));
		memcpy(X_paramGetBlueT()->Name, g_PreSuffix, g_nPreSuffix);
		memset(g_PreSuffix, 0, 20);
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		if (!BlueT_set_dev_name((char*)X_paramGetBlueT()->Name,strlen((char*)X_paramGetBlueT()->Name)))
		{ return FALSE; }//�����豸���� 
		break;
	case DataSetBlueT_Password: //������������
		if(!g_DataSet_Odd)	{ return FALSE; }
		if(g_nPreSuffix > 6) return FALSE;
		memset(X_paramGetBlueT()->PSW, 0, sizeof(X_paramGetBlueT()->PSW));
		memcpy(X_paramGetBlueT()->PSW, g_PreSuffix, 6);
		memset(g_PreSuffix, 0, 20);
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		if (!BlueT_set_slave_pin((char*)X_paramGetBlueT()->PSW,strlen((char*)X_paramGetBlueT()->PSW)))
		{ return FALSE; }//����slaveģʽ�µ�ƥ������
		break;
	default: return FALSE;
	}

	return TRUE;
}

//�½���ģʽ����
BOOL Set_NewDecMode(int Rec34, int Rec56, int Rec7)
{
	switch(Rec34) {
	case DecMode_NewOscillation: //��������
		if(Rec56 == 0 || Rec56 == 1) //ʹ������
			X_paramBakGetGlobalAllParam()->pHard->bOscillation = Rec56;
		else if(Rec56 == 2){//ʱ������
			g_DataType = DataSetType_Oscillation;
			g_DataSet = 0;
			g_IsSetCodeStart = TRUE;
		}
		else return FALSE;
		break;
	default: return FALSE;
	}
	return TRUE;
	Rec7 = Rec7;
}
typedef struct tagSetCodeParam{
	int   nError; //����
	int   nStatus; //״̬
	BOOL  bSave2Flash;   // �洢
	BOOL  isOpenSet;
	BOOL  bNoSave;	//�������־
}UTSetCodeParam;

/********************************************************************************************
*	local referenced macro defines & type defines
********************************************************************************************/
UTSetCodeParam  g_SetParam;
/*********************************************************************************************
*	local function prototype
*********************************************************************************************/

/*********************************************************************************************
*	Implementation functions 
*********************************************************************************************/

void W_setcSetResult(int nResult)
{
	if (nResult >= U_uIMG_STATUS_SETCODE_START
		&& nResult <= U_uIMG_STATUS_SETCODE_SUC)
		g_SetParam.nError = nResult;
	else if(nResult >= U_uIMG_STATUS_SETCODE_END){
		g_SetParam.nError = U_uIMG_STATUS_SETCODE_END;
		W_setcEnable(FALSE);
	}
	else
		g_SetParam.nError = U_uIMG_STATUS_SETCODE_ERROR;
}

int  W_setcGetResult(void)
{
	return g_SetParam.nError;
}

void W_setcSetSaveFlag(BOOL bToSave)
{
	g_SetParam.bSave2Flash = bToSave;
}

BOOL W_setcToSaveParam(void)
{
	return g_SetParam.bSave2Flash;
}

void W_setcSetNoSaveFlag(BOOL bNoSave)
{
	g_SetParam.bNoSave = bNoSave;
}

BOOL W_setcToNoSaveParam(void)
{
	return g_SetParam.bNoSave;
}


void W_setcEnable(BOOL bEnable)
{
	g_SetParam.isOpenSet = bEnable;
}

BOOL W_setcIsEnable(void)
{
	return g_SetParam.isOpenSet;
}

BOOL	W_setcIsMacroCancel(BYTE* pbyMsg, int nMsgLen)
{
	//����SetCode ��ȡ����������ȡ����Ч����������Ч
	if (nMsgLen == 8 && strncmp((char*)pbyMsg,"05000000",nMsgLen) == 0){
		W_setcSetResult(U_uIMG_STATUS_SETCODE_SUC);
		W_setcEnable(FALSE);
		return TRUE;
	}//End of if;
	W_setcSetResult(U_uIMG_STATUS_SETCODE_UNKNOW);
	return FALSE;
}


//�����봦��
//���أ�0 ���� ����(FALSE)
//      1 ���� ������ȷ����(TRUE)
//      2 ���� ����������
int W_setcDealWith(void)
{
	int i;
	int nResult;
	BOOL IsSetCode;
	BYTE* pMsg={'\0'};
	int  nReFlag;
	int nMsgLen;

	nReFlag = SETCPROC_NOT; //��ͨ����
	
	pMsg = X_decoGetDecInfoBuff();
	nMsgLen = X_decoGetDecInfoLen();
	if (pMsg == NULL || nMsgLen <= 0)
	{ return nReFlag; }


	//�ж��ǲ���������

	IsSetCode = (X_decoGetBarCode() == UT_TYPE_1D && ((X_decoGetCodeType() == DC_ZASETUP) || (X_decoGetCodeType() == DC_SETUP128)));

	if (W_setcDealUserFunc != NULL)
	{//����Ƕ��������봦����
		W_setcDealUserFunc(IsSetCode,X_decoGetBarCode(),X_decoGetCodeType(),X_decoGetDecInfoBuff(),X_decoGetDecInfoLen());
	}

	//������������ù�����������������Ĳ��������룬��ر����������ù���
	if(W_setcIsEnable() && (!IsSetCode))
	{
		nReFlag = SETCPROC_NOT; //��ͨ����
		switch(g_ReplaceType) {
		case Replace_No: //���������룬�������û�д򿪣���ر�������
			W_setcEnable(FALSE);
			break;
		case Replace_UserPart: //��ָ���
			if(W_setcParseBatch(pMsg, nMsgLen))
			{ nReFlag = 3; }
			else
			{ nReFlag = SETCPROC_ERROR; } //�������
			break;
		case Replace_SN: //����������SN
			if (nMsgLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pSN))
			{
				memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pSN,pMsg,nMsgLen);
				X_paramBakGetGlobalAllParam()->pDevInfo->pSN[nMsgLen] = '\0';
                          paramSaveType(1);
				X_paramPlaceKeyword((char*)"S/N",(char*)X_paramBakGetGlobalAllParam()->pDevInfo->pSN,TRUE);
				nReFlag = 4;
			}//End of if;
			break;
		case Replace_ESN: //����������ESN
			if (nMsgLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pESN))
			{
				memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pESN,pMsg,nMsgLen);
				X_paramBakGetGlobalAllParam()->pDevInfo->pESN[nMsgLen] = '\0';
                         paramSaveType(1);
				X_paramPlaceKeyword((char*)"ESN",(char*)X_paramGetGlobalAllParam()->pDevInfo->pESN,TRUE);
				nReFlag = 4;
			}//End of if;
			break;
		case Replace_Date: //���������ó�������
			if (nMsgLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pDate))
			{
				memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pDate,pMsg,nMsgLen);
				X_paramBakGetGlobalAllParam()->pDevInfo->pDate[nMsgLen] = '\0';
                         paramSaveType(1);
				X_paramPlaceKeyword((char*)"DATE",(char*)X_paramBakGetGlobalAllParam()->pDevInfo->pDate,TRUE);
				nReFlag = 4;
			}//End of if;
			break;
		case Replace_ManufID: //����������ManufactureID
			if (nMsgLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pManufID))
			{
				memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pManufID,pMsg,nMsgLen);
				X_paramBakGetGlobalAllParam()->pDevInfo->pManufID[nMsgLen] = '\0';
				paramSaveType(1);
				X_paramPlaceKeyword((char*)"ManufactureID",
					(char*)X_paramBakGetGlobalAllParam()->pDevInfo->pManufID,TRUE);
				nReFlag = 4;
			}//End of if;
			break;
		case Replace_ManufName: //����������ManufactureName
			if (nMsgLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pManufName))
			{
				memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pManufName,pMsg,nMsgLen);
				X_paramBakGetGlobalAllParam()->pDevInfo->pManufName[nMsgLen] = '\0';
				paramSaveType(1);
				X_paramPlaceKeyword((char*)"ManufactureName",
					(char*)X_paramBakGetGlobalAllParam()->pDevInfo->pManufName,TRUE);
				nReFlag = 4;
			}//End of if;
			break;
            case Replace_NewInstruction:
                NLSComm_ProcNormalCode(pMsg, nMsgLen);
		default: 
                    
			break;
		}
		W_setcParamInit(); //�����빫��������ʼ��
	}//End of if;
	else if (IsSetCode && nMsgLen >= 7)
	{   //���Ŀǰ��ȡ����������
		//������ڶ�ȡ�����벻���Խ�������
		//������ֵ
            if(X_decoGetCodeType() == DC_ZASETUP)
            {	
                for(nResult = 0, i = 0; i < 7; i++)
                { 
                    nResult = nResult*10 + (pMsg[i] - '0'); 
                }
		
                if((!W_setcIsEnable()) && (nResult != 6000) && (nResult != 6010) && (nResult != 6030) && (nResult != 6031))
                { 
                    nReFlag = SETCPROC_ERROR; 
                }//���������ù���û��
                else if(nResult == 6000 || nResult == 6010 || nResult == 6030 || nResult == 6031)
                {
                    if (W_setcParseSigle((BYTE*)pMsg,nMsgLen, FALSE))
						{ nReFlag = SETCRPOC_SUCC; }//�����а�������,���Բ�����
					else
						{ nReFlag = SETCPROC_ERROR; }
                }
                else
                {
					if (W_setcParseSigle((BYTE*)pMsg,nMsgLen, TRUE))
						{nReFlag = SETCRPOC_SUCC; } //���а�������
					else
					{ nReFlag = SETCPROC_ERROR; }
                }
            }
            else//set128������
            {
                //���ж��ǲ��ǿ��� /�Ƿ������������???? 
                int r1,r2,r3,r4;
                //char buf[32];
                //int len;
                r1 = strncasecmp ((char *)(pMsg+1), "SETUPE1",7);     
                r2 = strncasecmp ((char *)(pMsg+1), "SETUPE0",7);//�ַ�����ͬ������0
                r3 = strncasecmp ((char *)(pMsg+1), "FOBSET1",7);     
                r4 = strncasecmp ((char *)(pMsg+1), "FOBSET0",7);//�ַ�����ͬ������0

                //Y_commSendString(pMsg);
                //Y_commSendString("\r\n");
                //len=sprintf(buf, "r1=%d,r2=%d\r\n", r1,r2);
                //Y_commWrite((unsigned char * )buf, len);
                
                if((!W_setcIsEnable()) && (r1!= 0) && (r2 != 0) && (r3!= 0) && (r4 != 0))
                { 
                    nReFlag = SETCPROC_ERROR; 
                }//���������ù���û��
                else if((r1==0)||(r2==0)||(r1==0)||(r2==0))//������������4����
                {
                    if(NLSComm_ProcSettingCode(pMsg, (unsigned int)nMsgLen) == SC_RESULT_SuccUnsave)
                    { 
                        nReFlag = SETCRPOC_SUCC; 
                    } 
                    else
			 { 
                        nReFlag = SETCPROC_ERROR; 
                    }
                }
                else//��������������
                {
                    if(NLSComm_ProcSettingCode(pMsg, (unsigned int)nMsgLen) != SC_RESULT_FAIL)
                    { 
                        nReFlag = SETCRPOC_SUCC; 
                    } 
                    else
			 { 
                        nReFlag = SETCPROC_ERROR; 
                    }
                     if(g_commChange==1)
                     {
                           g_commChange = 0;
                     }
                }
            }
        }
// 	else if(X_decoGetBarCode() == UT_TYPE_1D && X_decoGetCodeType() == DC_SETUP128)
// 	{//�������͵�������
//  		if (Parse_SetCode_Blue2D((BYTE*)pMsg,iGetuImgInfoLen()))
// 		{ nReFlag = 1; }
// 	}
	else
	{ nReFlag = 2; } //��ͨ����

	switch(nReFlag){
	case 4: //������Ϣ
		X_paramSaveDeviceInfo();
		W_setcSetSaveFlag(FALSE);
		nReFlag = SETCRPOC_SUCC;
		break;
	case 3: //������Ϣ
		W_setcSetSaveFlag(TRUE);
		X_paramSaveDeviceInfo();
		if(X_paramSaveAsSysDefaultParam() == TRUE)
		{
			W_setcSetSaveFlag(FALSE); 
			W_setcSetResult(U_uIMG_STATUS_SETCODE_SUC);
			nReFlag = SETCRPOC_SUCC;
		}//End of if;
		break;
	case 2: // ������������Ϣ
		break;
	case 1: // ��������ȷ
		W_setcSetResult(U_uIMG_STATUS_SETCODE_SUC);
		break;
	case 0: // ���������
		W_setcSetResult(U_uIMG_STATUS_SETCODE_ERROR);
		break;
	}

	return nReFlag;

}


//������չ���ܺ���
void W_setcSetUserFunc(UTGeneralParam *psParam)
{
	if (psParam != NULL && psParam->Id == STID_SETCEXTFNC && psParam->psParamStruct != NULL)
	{
		W_setcDealUserFunc = ((TWSETCExtFunc*)psParam->psParamStruct)->fnc_DealUserSettingCode;
	}//End of if;
}

