
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
//主属性宏定义                  	            	
#define MainSet_DataAndTotal    	0				//数据码与综合设置
#define MainSet_Comm            	1				//通讯设置
#define MainSet_Hard            	2				//硬件设置
#define MainSet_DecodeMode      	3				//解码模式符设置
#define MainSet_Code1D          	4				//一维条码设置
#define MainSet_Code2D          	5				//二维条码设置
#define MainSet_OCR             	6				//OCR设置
#define MainSet_UserType        	7				//用户类型设置
#define MainSet_2DNonStandar    	8				//二维非标参数设置

#define MainSet_NewComm            11
#define MainSet_NewDecMode         12 

#define DecMode_NewOscillation     16  //振子设置
                                	            	
//从属性宏定义     

	//data format 相关设置
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

#define SubSet_Data             	0				//数据码
#define SubSet_Total            	1				//综合设置
#define SubSet_SetCodeSend      	2				//设置码信息发送设置
#define SubSet_SetSysMesSend    	3				//设置码信息发送设置
#define SubSet_Set1DCodeID      	4				//自定义一维条码CodeID前缀设置
#define SubSet_Set2DCodeID      	5				//自定义二维条码CodeID前缀设置
#define SubSet_IsOpenSetCode    	6				//设置码功能启动与关闭设置
#define SubSet_IsPowerOn        	7				//电源开启时信息显示设置
#define SubSet_CloseDevice          8               //关闭设备设置
                                	            	
                                	            	
#define MulrInfoSet_LED				2				//LED灯设置
                                	            	
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

                                                	
//综合设置宏定义                                	
#define TotalSet_All_Default     	0           	
#define TotalSet_All_Off         	1           	
#define TotalSet_All_On          	2           	
#define TotalSet_All_1D_Off      	3           	
#define TotalSet_All_1D_On       	4           	
#define TotalSet_All_2D_Off      	5           	
#define TotalSet_All_2D_On       	6           	
#define TotalSet_All_1D_OnlyOne  	7				//一维条码只读单码
#define TotalSet_All_1D_OneOrTwo 	8				//可读双一维码（同一类型），但也可以读单一维码
#define TotalSet_All_1D_OnlyTwo  	9				//可读双一维码（同一类型），且只读双一维码
#define TotalSet_UserDef_On      	10				//启动用户自定义整体默认
#define TotalSet_UserDefPart_On  	11				//启动用户自定义部分默认
#define TotalSet_SN_On           	12				//启动产品SN设置
#define TotalSet_ESN_On          	13				//启动产品ESN设置
#define TotalSet_Date_On         	14				//启动产品Date设置
#define TotalSet_SaveToUserDef   	15				//将当前设置存为用户默认设置
#define TotalSet_LoadUserDef     	16				//载入用户默认设置
#define TotalSet_LIC_On             17              //设置产品Licence
#define TotalSet_ManufID_On         18              //设置产品ManufactureID
#define TotalSet_IsDownload         19              //设置产品IsDownload
#define TotalSet_SaveToFactory		20				//将当前设置存为出厂设置
#define TotalSet_Load_General		21				//载入通用出厂设置                                                	
#define TotalSet_ManufName_On       22              //设置产品ManufactureName
#define TotalSet_All_Postal_Off		23
#define TotalSet_All_Postal_On		24
#define TotalSet_Driver_License		25
                                                	
//数据码宏定义                                  	
#define POS_Save                 	16				//保存
#define POS_Discard1Data         	17				//取消前一次读的一位数据
#define POS_DiscardAllData       	18				//取消前面读的一串数据
#define POS_DiscardSet           	19				//取消当前最大最小读码或图象亮度设置
                                 	            	
//数据码类型宏定义               	            	
#define DataSetType_No           	0				//无类型
#define DataSetType_1DMin        	1				//一维码最小长度设置
#define DataSetType_1DMax        	2				//一维码最大长度设置
#define DataSetType_2DMin        	3				//二维码最小长度设置
#define DataSetType_2DMax        	4				//二维码最大长度设置
#define DataSetType_OnceDelay    	5				//一次读码延迟时间
#define DataSetType_InfoDelay    	6				//ESC +2，连续读码状态中断的时间间隔
#define DataSetType_Prefix       	7				//自定义前缀设置
#define DataSetType_Suffix       	8				//自定义后缀设置
#define	DataSetType_Terminal     	9				//自定义结束符设置
#define DataSetType_FixedLen     	10				//添加某种条码只读某些长度值
#define DataSetType_OCRMin       	11				//OCR最小长度设置
#define DataSetType_OCRMax      	12				//OCR最大长度设置
#define DataSetType_1DCodeID    	13				//自定义一维条码的CodeID设置
#define DataSetType_2DCodeID    	14				//自定义二维条码的CodeID设置
#define DataSetType_Cut         	15				//截取设置
#define DataSetType_Fixed_Dis   	16				//某种条码取消只读某些长度值
#define DataSetType_Sensibility 	17				//灵敏度值设置
#define DataSetType_ScanTime    	18				//读码次数限定设置
#define DataSetType_NonStandar  	19				//非标参数值设置
#define DataSetType_ErrorMsg       	20				//一定时间内没读到码返回信息设置

#define DataSetType_PostalCodeID    22				//自定义邮政码的CodeID设置


////
#define DataSetType_Oscillation   0x1a   //振子设置

#define DataSetType_KBW           0x30   //KBW设置
#define DataSetType_NetCard       0x31   //网卡设置
#define DataSetType_BlueT         0x32   //蓝牙设置
#define DataSetType_PS2           0x33   //PS2设置

#define DataSetType_DecodeArea    0x40				//读码区域设置

#define DataSetType_ReadBeepFreq	0x41				//解码成功蜂鸣器响声频率
#define DataSetType_ReadBeepDura	0x42				//解码成功蜂鸣器单次持续时间
#define DataSetType_ReadLEDDura		0x43				//解码成功LED持续时间

#define DataSetType_DataFormatting	0x50
#define DataSetKBW_Nationality    0  //键盘国家语言设置
#define DataSetKBW_CharDelay      1  //字符延迟
#define DataSetKBW_CodeDelay      2  //条码延迟
#define DataSetKBW_CodePage       3  //code page

#define DataSetBlueT_Name         0  //蓝牙名称设置
#define DataSetBlueT_Password     1  //蓝牙密码字符设置

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
                                	            	
#define Max_nPreSubfix          	10				//前后缀最大信息长度
#define Max_FixedLen            	2				//读码固定长度的参数个数
#define Max_DataCutLen             	20				//信息截取命令最大长度
#define Max_Delay					3600000			//最大时间延迟
#define Max_Sensibility             20              //感光灵敏度最大值
                                                	
//通讯设置宏定义                                	
#define SubSet_BoundRate			0				//波特率设置
#define SubSet_Parity           	1				//校验设置
#define SubSet_WordLenStop      	2				//字符长度停止位设置
#define SubSet_WordLenDataBits		3				//字符长度数据位设置
#define SubSet_HardwareFlowCtrl		4				//硬件流控设置
                                	            	
//解码模式符设置宏定义          	            	
#define DecModeSet_Prefix			0				//自定义前缀设置
#define DecModeSet_Suffix			1				//自定义后缀设置
#define DecModeSet_Repeat			2				//重复读码设置
#define DecModeSet_Spring			3				//接受串口控制设置
#define DecModeSet_Protect			4				//图象滤波设置
#define DecModeSet_IsPrefix			5				//自定义前缀使能或禁止设置
#define DecModeSet_IsSuffix			6				//自定义后缀使能或禁止设置
#define DecModeSet_IsCodeID			7				//CodeID前缀使能或禁止设置
#define DecModeSet_IsAIM			8				//AIM前缀使能或禁止设置
#define DecModeSet_IsTerminal		9				//结束符后缀使能或禁止设置
#define DecModeSet_Terminal			10				//结束符后缀设置
#define DecModeSet_AllPreSuf		11				//所有前后缀打开或屏蔽设置
#define DecModeSet_Sensibility		12				//灵敏度设置
#define DecModeSet_Delay			13				//延迟设置
#define DecModeSet_DataPacked		14				//数据打包设置
#define DecModeSet_IsCut			15				//截取使能或禁止设置
#define DecModeSet_Cut				16				//截取位置设置
#define DecModeSet_PrefixOrder		17				//前缀顺序设置
#define DecModeSet_ScanTime			18				//读码次数限定设置
#define DecModeSet_Medium			19				//读码介质设置
#define DecModeSet_ErrorMsg         20              //一定时间内没读到码返回信息设置
#define DecModeSet_CMOSAdjustMode   21              //
#define DecModeSet_DecodeArea		22              //
#define DecModeSet_DataFormat	    23                               	            	
                                	            	
#define POS_Macro					0   
   	
//出产用户配置和硬件设置        	            	
#define POS_UserType_Group1     	0				//用户设置第一组
                                	            	
                                	            	
//条码属性设置宏定义            	            	
#define Set_Default					0				//默认设置
#define Set_Off						1				//禁止设置
#define Set_On						2				//使能设置
#define Set_MinLen					3				//最小读码长度设置
#define Set_MaxLen					4				//最大读码长度设置
#define MicroQR_On					10              //使能识读MicroQR 
#define MicroQR_Off					11              //禁止识读MicroQR 
#define QR_ECI						15              //禁止ECI输出      
#define QR_CharEncoding				16
                              	            	
// 非标准前后缀使能设置宏定义    	            	
// #define POS_IsBlue2D_LenSend    	0				//Blue2D信息长度发送设置
// #define POS_IsBlue2D_TypeSend   	1				//Blue2D条码类型发送设置
// #define POS_IsBlue2D_Sensitive  	2				//Blue2D的灵敏度使能设置
                                	
//各参数在配置表中的起始位置    	
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

//各种一维条码设置的宏定义
//Code128码设置宏定义
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
#define Code128_CHECK_ON	  		15   // 校验检查开启 // add by Gavin 20080313
#define Code128_CHECK_OFF  		    16   // 检验交叉关闭 // add by Gavin 20080313
                               	
//EAN-UPC码设置宏定义          	
#define EANUPC_SendCheck_Off    	3				//不传送校验符
#define EANUPC_SendCheck_On     	4				//传送校验符
#define EANUPC_2Expand_Off      	5				//不识读两位扩展码
#define EANUPC_2Expand_On       	6				//识读两位扩展码
#define EANUPC_5Expand_Off      	7				//不识读五位扩展码
#define EANUPC_5Expand_On       	8				//识读五位扩展码
#define EANUPC_EAN8To13_Off     	9				//EAN8不把结果扩展成EAN-13
#define EANUPC_EAN8To13_On     		10				//EAN8把结果扩展成EAN-13
//#define EANUPC_UPC_Pre0_Off     	9				//UPC不传送前导字符'0'
//#define EANUPC_UPC_Pre0_On     		10				//UPC传送前导字符'0'
//#define EANUPC_UPCETo13_Off    		11				//UPCE不把结果扩展成EAN-13
//#define EANUPC_UPCETo13_On     		12				//UPCE把结果扩展成EAN-13
#define UPCE_Preamble_Off     				9				//UPCE不传送country code'0'与system character '0'
#define UPCE_Preamble_SystemChar_On     	10				//UPCE传送system character '0'
#define UPCE_ToUPCA_Off    					11				//UPCE不把转成UPC-A
#define UPCE_DataTo13_On     				12				//UPCE把结果扩展成UPC-A
#define UPCE_ToUPCA_On     					13				//UPC-E 转换成UPC-A
#define UPCE_Preamble_ALL_On     			14				//UPCE传送country code'0' + system character '0'

#define UPCA_Preamble_SystemChar_On 9				//UPCA传送system character
#define UPCA_Preamble_ALL_On     	10				//UPCA传送country code + system character 
#define UPCA_Coupon_Off  			13				//禁止UPCA+Coupon扩展
#define UPCA_Coupon_Allow			14				//允许UPCA+Coupon扩展
#define UPCA_Coupon_Require			15				//必须UPCA+Coupon扩展
#define UPCA_Coupon_RSS_Output_Off 	16				//输出所有coupon信息
#define UPCA_Coupon_RSS_Output_On 	17				//只输出RSS的信息
#define UPCA_Preamble_Off     		18				//UPCA不传送country code + system character 


#define EAN13_Coupon_Off  			11				//禁止EAN13+Coupon扩展
#define EAN13_Coupon_Allow			12				//允许EAN13+Coupon扩展
#define EAN13_Coupon_Require		13				//必须EAN13+Coupon扩展
                                                	
//INTER2OF5码设置宏定义                         	
#define Inter2Of5_Check_Off       	5				//不处理校验符
#define Inter2Of5_Check_Send_Off  	6				//处理校验符，但不传送校验符
#define Inter2Of5_Check_Send_On   	7				//处理校验符，且传送校验符
#define Inter2Of5_ITF14_Off       	8				//不处理ITF14条码
#define Inter2Of5_ITF14_Check_Off 	9				//处理ITF14条码，但不传送校验符
#define Inter2Of5_ITF14_Check_On 	10				//处理ITF14条码且传送校验符
#define Inter2Of5_ITF6_Off       	11			 	//不处理ITF6条码
#define Inter2Of5_ITF6_Check_Off 	12 				//处理ITF6条码，但不传送校验符
#define Inter2Of5_ITF6_Check_On  	13 				//处理ITF6条码且传送校验符
#define Inter2Of5_FixedLen_Off   	14 				//设置交叉25码只识读定长关闭
#define Inter2Of5_FixedLen_On    	15 				//设置交叉25码只识读定长打开
#define Inter2Of5_FixedLen_Ena   	16 				//设置交叉25码识读定长的长度
#define Inter2Of5_FixedLen_Dis   	17 				//取消交叉25码识读定长的长度
                                       				
//Matrix25码设置宏定义                 				
#define Matrix25_Check_Off        	5  				//不处理校验符
#define Matrix25_Check_Send_Off   	6  				//处理校验符，但不传送校验符
#define Matrix25_Check_Send_On    	7  				//处理校验符，且传送校验符
                                  	   				
//Module25码设置宏定义            	   				
#define Module25_Check_Off        	5  				//不处理校验符
#define Module25_Check_Send_Off   	6  				//处理校验符，但不传送校验符
#define Module25_Check_Send_On    	7  				//处理校验符，且传送校验符
                                       				
//CODE39码设置宏定义                   				
#define Code39_Check_Off       		5  				//不处理校验符
#define Code39_Check_Send_Off  		6  				//处理校验符，但不传送校验符
#define Code39_Check_Send_On   		7  				//处理校验符，且传送校验符
#define Code39_StartStop_Off   		8  				//不传送起始符与终止符
#define Code39_StartStop_On    		9  				//传送起始符与终止符
#define Code39_FullASCII_Off  		10 				  //不识别全ASCII码
#define Code39_FullASCII_On   		11 				  //识别全ASCII码
#define Code39_Code32_Off   		12 				  //不转换为Code32
#define Code39_Code32_On   			13 				  //转换为Code32  
#define Code39_Code32_Star_Off   	14 				  //Code32不添加起始符
#define Code39_Code32_Star_On		15 				  //Code32  添加起始符  

                                     				
//CODABAR码设置宏定义                  				
#define Codabar_Check_Off       	5  				//不处理校验符
#define Codabar_Check_Send_Off  	6  				//处理校验符，但不传送校验符
#define Codabar_Check_Send_On   	7  				//处理校验符，且传送校验符
#define Codabar_StartStop_Off   	8  				//不传送起始符与终止符
#define Codabar_StartStop_On    	9  				//传送起始符与终止符
#define Codabar_ABCDTNXE_Off		10				//起始符与终止符为普通ABCD格式
#define Codabar_ABCDTNXE_On			11				//起始符与终止符为ABCD/TN*E格式
#define Codabar_LOWERCASE_Off		12				//起始符与终止符为大写
#define Codabar_LOWERCASE_On		13				//起始符与终止符为小写

//CODE 93码设置宏定义   // add by Gavin 20080313  
#define Code93_Check_Off       	5  				//不处理校验符   // add by Gavin 20080313
#define Code93_Check_Send_Off  	6  				//处理校验符，但不传送校验符   // add by Gavin 20080313
#define Code93_Check_Send_On   	7  				//处理校验符，且传送校验符   // add by Gavin 20080313
                                	   				
//CODEZ码设置宏定义             	   				
#define CodeZ_Error_Send_Off    	5  				//只发送数据信息
#define CodeZ_Error_Send_On     	6  				//发送数据信息和纠错信息

//EAN·UCC复合码(COMPOSITE)设置宏定义
#define COMPOSITE_UCCEAN_Off		3
#define COMPOSITE_UCCEAN_On			4

//RSS设置宏定义
#define RSS_AI_Send_Off			5
#define RSS_AI_Send_On			6

//CODE11设置宏定义
#define CODE11_No_Check						5	//不校验
#define CODE11_C11Chk1						6	//一位校验，MOD11
#define CODE11_C11M11M11Chk					7	//两位校验MOD11/MOD11
#define CODE11_C11M11M9Chk					8	//两位校验MOD11/MOD9
#define CODE11_10char_C11M11M11Chk			9	//MOD11单校验(Len<=10),MOD11/MOD11双校验(Len>10)
#define CODE11_10char_C11M11M9Chk			10	//MOD11单校验(Len<=10),MOD11/MOD9双校验(Len>1)
#define CODE11_No_Send_Chk					11	//不传送校验符
#define CODE11_Send_Chk						12	//传送校验符

//MSIPLESSEY设置宏定义
#define MSI_No_CHK			5		//不校验
#define MSI_CHK_M10			6		//单MOD10校验
#define MSI_CHK_M10M10		7		//双MOD10校验
#define MSI_CHK_M10M11		8		//MOD10/MOD11双校验
#define MSI_Check_Send_Off	9		//不发送校验位
#define MSI_Check_Send_On	10		//发送校验位


//PLESSEY设置宏定义
#define PLESSEY_No_CHK			5	//不校验
#define PLESSEY_CHK_NO_Send		6	//校验不发送
#define PLESSEY_CHK_Send		7	//校验并发送

//STANDARD25设置宏定义
#define STANDARD25_No_CHK			5	//不校验
#define STANDARD25_CHK_NO_Send		6	//校验不发送
#define STANDARD25_CHK_Send			7	//校验并发送

//INDUSTRIAL25设置宏定义
#define INDUSTRIAL25_No_CHK				5	//不校验
#define INDUSTRIAL25_CHK_NO_Send		6	//校验不发送
#define INDUSTRIAL25_CHK_Send			7	//校验并发送

//ISBN设置宏定义
#define ISBN_Send_13		3	//发送13个数字
#define ISBN_Send_10		4	//发送10个数字

//China Post 25码设置宏定义                 				
#define ChnPost25_Check_Off        	5  				//不处理校验符
#define ChnPost25_Check_Send_Off   	6  				//处理校验符，但不传送校验符
#define ChnPost25_Check_Send_On    	7  				//处理校验符，且传送校验符


//PDF417码设置宏定义
#define PDF417_Double_OnlyOne       7
#define PDF417_Double_OnlyTwo       8
#define PDF417_Double_OneOrTwo      9
#define PDF417_Reverse             32
#define PDF417_Mirror              33
#define PDF417_ECI					34
#define PDF417_CharEnCoding			35

//QR码设置宏定义
#define QR_Double_OnlyOne           7
#define QR_Double_OnlyTwo           8
#define QR_Double_OneOrTwo          9

//DataMatrix码设置宏定义
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

//Aztec码设置宏定义
#define Aztec_Multi_Num             6 //多码同图条码数量
#define Aztec_Multi_None            7 //禁止多码同图
#define Aztec_Multi_Only            8 //只读固定条码数量的条码
#define Aztec_Multi_Mixed           9 //读固定条码数量的（优先）或只读一个条码
#define Aztec_ECI		           10
#define Aztec_CharEnCoding         11

#define VeriCode_UnZip_ModeUp       5
#define VeriCode_UnZip_ModeDown     6

                                	  
//汉信码设置宏定义
#define HAN_SENSIBLE_Double_OnlyOne   5
#define HAN_SENSIBLE_Double_OnlyTwo   6
#define HAN_SENSIBLE_Double_OneOrTwo  7
#define HAN_SENSIBLE_Reverse          8
 				
//二维条码宏设置宏定义          	   				
#define Macro_Cancel            	0  				//取消宏数据信息
#define Macro_DirectSend        	1  				//每个数据块的信息读取后直接传送
#define Macro_SendInTurn        	2  				//按顺序发送（缓存不超过64kbyte），
		                        	   				//即当比当前数据块的连接号小的数
		                        	   				//据块都已识读时，把这些目前已经
                                	   				//连接好的数据（包括当前块）传送
                                	   				
#define Macro_LinkSendAfterRead 	3  				//读完全部的数据块后再连接发送,
		                           					//当缓存数据量超过64k时，以模式2的
                                   					//方式传送数据

//通讯参数设置
#define Comm_Interface          0
#define Comm_RS232              1
#define Comm_USB                2
#define Comm_KBW                3
#define Comm_NetCard            4
#define Comm_BlueTooth          5
#define Comm_PS2                6
#define Comm_HID_POS            7


//用其它类型的条码来设置的设置种类
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
                            		

#define  SENSITIVE_CALC(x)   (2 + (4 - (x)) * 3)  //感应级别和最终值得关系
/********************************************************************************************
*	local referenced macro defines & type defines
********************************************************************************************/
//解码最大最小信息长度开关设置
unsigned int g_DataSet,g_PosMaxMin;
int g_DataType;
BOOL g_IsSetCodeStart; //开始要用数据码
BOOL g_DataSet_Odd; //读取的数据码的奇偶次数

//解码最大最小信息长度开关设置
unsigned char g_PreSuffix[20];
int g_nPreSuffix;

int g_nFixedLen;  //用于设置读码固定长度

//通过设置码启动设置，用其它类型的条码来设置的种类
int g_ReplaceType  = Replace_No;

//设置后是否有声音，在用户部分默认设置时用到

//用于Blue2D的诊断设置
BOOL g_Isdiagnostics = FALSE;
unsigned char Blue2D_I2Of5Len[5];
unsigned int Blue2D_n1DSet = 0;
unsigned int Blue2D_n2DSet = 0;

const unsigned int  conBaudRate[9] = {1200,2400,4800,9600,14400,19200,38400,57600,115200};
//dataformatting批处理命令
unsigned char g_DataFormatbuf[512];
int g_nDataFormatbuf;

struct _D_F //参数设计需要
{
	int FormatType;
	int Ter_Type;
	int Code_ID;
	int Command_Len;
	char Command[Command_Num+1];
}DF;
int istage = 0;
int enter_set_flag = 0;  //进入dataformat设置标志
int clear_flag = 0;
/*********************************************************************************************
*	local function prototype
*********************************************************************************************/

void SetCode_vITF25Param2Buff_Mini(int v1,int v2, char* buff);

BOOL Parse_SetCode(unsigned char *SetCode,int nType);  //设置码解析总函数
BOOL Set_DataAndTotal(int Rec34, int Rec56, int Rec7);  //数据码与综合设置
BOOL Set_Comm(int Rec34, int Rec56);  //通讯设置
BOOL Set_Hard(int Rec34, int Rec56, int Rec7);  //硬件设置
//BOOL Set_DecodeMode(int Rec34, int Rec56);  //解码模式符设置
BOOL Set_Code1D(int Rec34, int Rec56);  //一维条码设置
BOOL Set_Code2D(int Rec34, int Rec56, int Rec7);  //二维条码设置
BOOL Set_OCR(int Rec34,int Rec56);  //OCR设置
BOOL Set_Postal(int Rec34, int Rec56); //邮政码设置
BOOL Set_UserType(int Rec34, int Rec56);//出产用户配置和硬件设置
BOOL SetCode_Data_PreSuffix(int Rec56);  //前后缀数据设置
BOOL SetCode_Data_FixedLen(int Rec56);  //读码固定长度设置
BOOL SetCode_Discard(int Rec56);  //取消设置，包括取消一个字符，一串字符和一个操作
BOOL SetCode_Save(void);  //保存设置
BOOL SetCode_Save_1DMin(void);  //保存一维条码最小读码设置
BOOL SetCode_Save_1DMax(void);  //保存一维条码最大读码设置
BOOL SetCode_Save_2DMin(void);  //保存二维条码最小读码设置
BOOL SetCode_Save_2DMax(void);  //保存二维条码最大读码设置
BOOL SetCode_Save_OCRMin(void);  //保存OCR最小长度设置
BOOL SetCode_Save_OCRMax(void);  //保存OCR最大长度设置
BOOL SetCode_Save_NonStandar(void); //保存二维码非标参数
BOOL SetCode_Total(int Rec56, int Rec7);  //条码综合设置
BOOL SetCode1D_Code128(int Rec56);  //128码设置
BOOL SetCode1D_EAN8(int Rec56); //EAN8设置
BOOL SetCode1D_EAN13(int Rec56); //EAN13设置
BOOL SetCode1D_UPCE(int Rec56); //UPCE设置
BOOL SetCode1D_UPCA(int Rec56); //UPCA设置
BOOL SetCode1D_Inter2of5(int Rec56);  //INTER25设置
BOOL SetCode1D_Matrix25(int Rec56);  //Matrix25设置
BOOL SetCode1D_Module25(int Rec56);  //Module25设置
BOOL SetCode1D_Regular39(int Rec56);  //CODE39设置
BOOL SetCode1D_Codabar(int Rec56);  //Codabar设置
BOOL SetCode1D_Code93(int Rec56);  //Code93设置
BOOL SetCode1D_CodeZ(int Rec56);//CodeZ码设置
BOOL SetCode1D_RSS(int Rec56);//RSS码设置
BOOL SetCode1D_COMPOSITE(int Rec56);//COMPOSITE码(UCC EAN复合码)设置
BOOL SetCode1D_CODE11(int Rec56);//Code11码设置
BOOL SetCode1D_ISBN(int Rec56);//ISBN码设置
BOOL SetCode1D_INDUSTRIAL25(int Rec56);//INDUSTRIAL25码设置
BOOL SetCode1D_STANDARD25(int Rec56);//STANDARD25码设置
BOOL SetCode1D_PLESSEY(int Rec56);//PLESSEY码设置
BOOL SetCode1D_MSIPLESSEY(int Rec56);//MSI_PLESSEY码设置

BOOL SetCode1D_ChnPost25(int Rec56);//china post 25

BOOL SetCode2D_Macro(int Rec56);  //二维条码宏设置
BOOL SetCode2D_PDF417(int Rec56, int Rec7);  //PDF417码设置
BOOL SetCode2D_QR(int Rec56, int Rec7);  //QR码设置
BOOL SetCode2D_Aztec(int Rec56, int Rec7);  //Aztec码设置
BOOL SetCode2D_DataMatrix(int Rec56, int Rec7);  //DataMatrix码设置
BOOL SetCode2D_MaxiCode(int Rec56);  //MaxiCode码设置
BOOL SetCode2D_VeriCode(int Rec56, int Rec7);  //VeriCode设置
BOOL SetCode2D_Han(int Rec56, int Rec7);//汉信码设置
BOOL SetCode2D_UserDef(int Rec56, int Rec7); //用户自定义二维码设置
BOOL SetCode2D_GM(int Rec56); //GM码设置
BOOL SetCode2D_MicroPDF417(int Rec56); //MicroPDF417码设置
BOOL SetOCR_SPEC_OCR_B(int Rec56);  //SPEC_OCR_B设置
BOOL Set_2DNonStandar(int Rec34, int Rec56); //二维条码非标参数设置
BOOL Set_QRNonStandar(int Rec56); //QR条码非标参数设置
BOOL SetCode1D_ISSN(int Rec56);
BOOL SetCodePos_Postnet(int Rec56);
BOOL SetCodePos_Onecode(int Rec56);
BOOL SetCodePos_RM4CC(int Rec56);
BOOL SetCodePos_Planet(int Rec56);
BOOL SetCodePos_Kixcode(int Rec56);
BOOL SetCodePos_Auspost(int Rec56);

BOOL Set_NewComm(int Rec34, int Rec56, int Rec7);//新通讯参数设置

BOOL Set_NewCommKBW(int Rec56, int Rec7);//KBW参数设置
BOOL Set_NewCommBlueT(int Rec56, int Rec7);//蓝牙参数设置
BOOL SetCode_Save_KBW(void);//KBW设置
BOOL SetCode_Save_BlueT(void); //蓝牙设置
BOOL Set_NewCommPS2(int Rec56, int Rec7);//PS2参数设置
BOOL Set_NewDecMode(int Rec34, int Rec56, int Rec7);//新解码模式设置


BOOL Parse_UserPart(unsigned char *pMes, int nlen);//用户部分默认设置码
BOOL bUseDataCode(unsigned char *pMes);
BOOL GetKeyWord(unsigned char * pKeyWord, unsigned char *pMes, int nlen, int *nPlace);
BOOL bGetData(unsigned char *pValue, int nsize, unsigned char *pMes, int nlen, int *nPlace);
BOOL Set_DataFormat(int Rec56, int Rec7);

int GetHex(char c);
int StringToInt(unsigned char *pMes); //从数字字符串中获取数字


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
*setfalg :0 通过设置码设置
*        :>=1 通过命令方式设置
*							1: 全部修改
*							2: ter id
*							3: code id
*							4: 适用长度
*							5: 命令字符串
*/
int W_setFormat(int formatID,int setflag,char *cmd,int len)
{
	//第一步：依据ID设置Format
	UALLPARAM *psParam;
	char *p = NULL;
//	char *p1 = NULL;
	int i=0;
	DF_Param	dfparam;
	int flag;
	char temp;

	psParam = X_paramGetGlobalAllParam();
	if (setflag == 0) // 只有用设置码设置时才要判断返回
	{
		if (enter_set_flag == 0 && clear_flag == 0)
			return 0;
	}
	if (clear_flag == 2) //全部删除
	{
		memset(psParam->pDFormat->format,0,sizeof(psParam->pDFormat->format));
		clear_flag = 0; 
		return 1;
	}
	if (setflag == 0)//不需要
	{	psParam->pDFormat->format[formatID].FormatID = formatID;
	//		psParam->pDFormat->format[formatID].FormatType = DF.FormatType;
	psParam->pDFormat->format[formatID].FormatType = 6;
	/*		psParam->pDFormat->format[formatID].TerID = DF.Ter_Type;*/ //现在还没有对通信接口的设置功能（设备没有对外提供通信接口的Id）
	psParam->pDFormat->format[formatID].TerID = 999;

	psParam->pDFormat->format[formatID].SymbolID = DF.Code_ID;
	psParam->pDFormat->format[formatID].MsgLen = DF.Command_Len;
	//	memcpy(psParam->pFromat->format[formatID].TerID,DF.Ter_Type,Ter_Type_Num);
	//	memcpy(psParam->pFromat->format[formatID].CodeID,DF.Code_ID,Code_ID_Num);
	//	memcpy(psParam->pFromat->format[formatID].ComLen,DF.Command_Len,Command_Len_Num);
	memset(psParam->pDFormat->format[formatID].Cmd,0,Command_Num+1);
	memcpy(psParam->pDFormat->format[formatID].Cmd,DF.Command,Command_Num);

	clear_flag = 0; //如果是删除format而执行Save，则要清除clear_flag标志。
	}
	else if (setflag == 1)
	{//整体

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
		dfparam.nDFSelType = 3;//库里面只接受命令类型位1、2或3		

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
		psParam->pDFormat->format[formatID].TerID = 999;//暂定
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
	{//命令类型和命令内容
		memset(psParam->pDFormat->format[formatID].Cmd, 0, len);
		//psParam->pDFormat->format[formatID].FormatType = (int)cmd[7];
		p = cmd+7;
		for(i = 0; i< len-8 ;i++)
			psParam->pDFormat->format[formatID].Cmd[i] = int_to_char((int)p[i]);
		psParam->pDFormat->format[formatID].Cmd[i] = '\0';
	}


	if (setflag == 0) // 清除缓存区
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
//设置码信息解析总函数
//nType: 类型
//    0：中安设置码解析
//    1：Blue2D命令的解析，不保存

BOOL	W_setcParseSigle(BYTE* pbyMsg, int nMsgLen, BOOL bToSave)
{
	int Rec12,Rec34,Rec56, Rec7;
	int RecParam = -1;

	istage = 0;

	if (nMsgLen < 7)
	{ return FALSE; }//End of if;

	W_setcSetSaveFlag(TRUE); //设置码命令设置后是否保存
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
	case MainSet_DataAndTotal://数据码与综合设置
		if(!Set_DataAndTotal(Rec34, Rec56, Rec7))
		{ return FALSE; }
		break;

	case MainSet_Comm://通讯设置
		W_setcParamInit();
		if(pbyMsg[6] == '0')
		{//对COM1设置
		    if(!Set_Comm(Rec34, Rec56))
			{ return FALSE; }
		}
		else
		{ return FALSE; }
		break;

	case MainSet_Hard://硬件设置
		W_setcParamInit();
		if(!Set_Hard(Rec34, Rec56, Rec7))
		{ return FALSE; }
		break;

	case MainSet_DecodeMode://解码模式设置
		W_setcParamInit();
		if(!Set_DecodeMode(Rec34, Rec56, Rec7, RecParam))
		{ return FALSE; }
		break;

	case MainSet_Code1D://一维码设置
		W_setcParamInit();
		if(!Set_Code1D(Rec34, Rec56))
		{ return FALSE; }
		break;
		
	case MainSet_Code2D://二维码设置
		W_setcParamInit();
		if(!Set_Code2D(Rec34, Rec56, Rec7))
		{ return FALSE; }
		break;
	
	case MainSet_OCR://OCR与邮政码设置
		W_setcParamInit();
		if(Rec34 >= 40){	//邮政码
			if(!Set_Postal(Rec34, Rec56))
			{ return FALSE; }
		}
		else {		//OCR
			if(!Set_OCR(Rec34, Rec56))
			{ return FALSE; }
		}
		
		break;

	case MainSet_UserType://用户类型设置
		W_setcParamInit();
		if(!Set_UserType(Rec34, Rec56))
		{ return FALSE; }
		break;
	case MainSet_2DNonStandar://二维非标参数设置
		W_setcParamInit();
		if(!Set_2DNonStandar(Rec34, Rec56))
		{ return FALSE; }
		break;
	case MainSet_NewComm://通讯设置
		W_setcParamInit();
		if(!Set_NewComm(Rec34, Rec56, Rec7))
		{ return FALSE; }
		break;
	case MainSet_NewDecMode://新解码模式设置
		W_setcParamInit();
		if(!Set_NewDecMode(Rec34, Rec56, Rec7))
		{ return FALSE; }
		break;
	default:
		return FALSE;
    }//End of switch;

    if (istage == 0) // 如果在设置data Format被打断，则此次设置失效,在format的个设置过程中设置istage=0，确保format设置的连续性
    {
		enter_set_flag = 0;
		clear_flag = 0;
    }


	if(bToSave == TRUE)
	{//中安设置码设置
		W_setcSetSaveFlag(TRUE);
	}
	else if(bToSave != TRUE)
	{ //Blue2D命令设置，不保存
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

//用户部分默认设置码
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
	{ //至少要有3个字符的关键字
		memset(SetMes.pKeyWord, 0, sizeof(SetMes.pKeyWord));
    	memset(SetMes.pValue, 0, sizeof(SetMes.pValue));
		
		//先找到当前位置后面的第一是数字
		for(j = i; j < nMsgLen; j++)
		{
			if(pbyMsg[j] <= '9' && pbyMsg[j] >= '0')
			{ break; }
		}//End of for;
		if(j >= nMsgLen)
		{ return FALSE; } //非正确格式

		i = j;
		p = pbyMsg + j;

		nPlace = 0;
	    if(!GetKeyWord(SetMes.pKeyWord, p, nMsgLen-j, &nPlace))
		{ return FALSE; }
		
		if(i + 7 >= nMsgLen)
		{ break; }
		//命令必须是7位的十进制数字
		if(strlen((char *)(SetMes.pKeyWord)) < 7)
		{ return FALSE; }
		for(j = 0; j < 7; j++)
		{
			if(SetMes.pKeyWord[j] < '0' || SetMes.pKeyWord[j] > '9')
			{ return FALSE; }
		}//End of for;

		if(!W_setcParseSigle(SetMes.pKeyWord, 7, FALSE))
		{ return FALSE; }//1代表暂时不保存
		
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
		{ //不要借助数据码，直接设置的
			for(j = i; j < nMsgLen; j++)
			{ //寻找分号
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
	{ return FALSE; }//保存

	W_setcParamInit();

	return TRUE;
}//End of function;

//获取关键字
BOOL GetKeyWord(unsigned char * pKeyWord, unsigned char *pMes, int nlen, int *nPlace)
{
	int i;
	int j = 0;

	//关键字前面的空格
	for (i = 0; i < nlen; i++)
	{
		if(pMes[i] != ' ')
		{ break; }
	}//End of for;

	if(i >= nlen)
	{ return FALSE; }
	//获取关键字，碰到空格自动跳过,若是分号或等号,则跳出
	for( ; i < nlen; i++)
	{
		if(pMes[i] == ' ')
		{ continue; }
		if((pMes[i] == ';') || (pMes[i] == '='))
		{ break; }
		pKeyWord[j++] = pMes[i];
		if (j>7)	//指令固定为7个数字
			return FALSE;
	}//End of for;

	*nPlace = i;
	return TRUE;
}

//从设置串中获取要设置的数据值
//char * pValue    信息
//int nsize		   pValue 空间大小
//char * pMes      数据来源
//int nlen         pMes的最大偏移量
//int *nPlace      获取信息后的偏移量
BOOL bGetData(unsigned char *pValue, int nsize, unsigned char *pMes, int nlen, int *nPlace)
{
	int i,j,k,m,n;
	int nMesLen = 0;
	
	//找到命令后的第1个'='
	for(i = 0; i < nlen; i++)
		if(pMes[i] == '=') break;
	if(i == nlen) return FALSE;

	//等号后面的空格
	i++;
	while(i < nlen){
		if(pMes[i] != ' ') break;
		else i++;
	}
	if(i == nlen) return FALSE;

	if(pMes[i] == '\"'){//用引号的为字符串直接赋值
		//先找到字符串的结尾: 引号 + 分号
		if(i + 2 >= nlen) return FALSE;
		for(j = i + 1; j < nlen - 1; j++){
			if(pMes[j] == '\"' && pMes[j + 1] == ';') break;
		}
		if(j == (nlen - 1)) return FALSE; //没找到结尾
		if ((j - i - 1)> nsize) return FALSE; //超出pValue空间长度

		memcpy(pValue, pMes + i + 1, j - i - 1); //提取引号里面的数据
		*nPlace = j + 2;
	}
	else {
		//先找结尾：分号
		for(j = i; j < nlen; j++){
			if(pMes[j] == ';') break;
		}
		if(j == nlen) return FALSE; //没找到

		nMesLen = j - i; //要分析的信息长度
		if (nMesLen > nsize) return FALSE; //超出pValue空间长度

		if((i + 2 < nlen) && (pMes[i] == '0') && 
		    ((pMes[i + 1] == 'x') || (pMes[i + 1] == 'X')))
		{   //十六进制值字符处理
			if((nMesLen & 0x01) != 0) return FALSE; //必须是偶数个
			for(k = 0; k < nMesLen - 2; k += 2){
				m = GetHex(pMes[i + k + 2]);
				n = GetHex(pMes[i + k + 3]);
				if((m == -1) || (n == -1)) return FALSE;
				else pValue[k / 2] = ((m << 4) + n);
			}
			*nPlace = j + 1; //下一个命令的偏移位置,到分号后面
		}
		else{//必须是一串数字
			for(k = 0; k < nMesLen; k++){
				if((pMes[i + k] >= 0x30) && (pMes[i + k] <= 0x39)){
					pValue[k] = pMes[i + k];
				}
				else return FALSE;
			}
		    *nPlace = j + 1; //下一个命令的偏移位置,到分号后面
		}
	}

	return TRUE;
}

//用户设置码需要借助数据码的设置转换
BOOL bUseDataCode(unsigned char *pMes)
{
	int i;
	int nValue = 0;
	
	switch(g_DataType){
	case DataSetType_1DMin://一维码读码最小长度设置
    case DataSetType_1DMax://一维码读码最大长度设置
	case DataSetType_OCRMin://OCR最小长度设置
	case DataSetType_OCRMax://OCR最大长度设置
		nValue = StringToInt(pMes);
		if(nValue == -1 || nValue > 127) return FALSE;
		g_DataSet = (unsigned int)nValue;
   	    break;
	
    case DataSetType_2DMin://二维码读码最小长度设置
    case DataSetType_2DMax://二维码读码最大长度设置
		nValue = StringToInt(pMes);
		if(nValue == -1 || nValue > 65535) return FALSE;
		g_DataSet = (unsigned int)nValue;
		break;

	case DataSetType_Prefix://前缀信息设置
	case DataSetType_Suffix://后缀信息设置
	case DataSetType_ErrorMsg: //一定时间内没读到码返回信息设置
		if(strlen((char *)pMes) > 10) return FALSE;
		g_nPreSuffix = strlen((char *)pMes);
		memcpy(g_PreSuffix, pMes, g_nPreSuffix);
		g_DataSet_Odd = TRUE;
		break;

	case DataSetType_Terminal://结束符后缀设置
		if(strlen((char *)pMes) > 2) return FALSE;
		g_nPreSuffix = strlen((char *)pMes);
		memcpy(g_PreSuffix, pMes, g_nPreSuffix);
		g_DataSet_Odd = TRUE;
		break;

	case DataSetType_FixedLen://条码识读固定长度设置
		if(strlen((char *)pMes) == 1){ //设置值
			if(pMes[0] > 64) return FALSE;
			g_PreSuffix[0] = pMes[0];
			g_nFixedLen = 3;
	        g_nPreSuffix = 1;
		}
		else { //设置范围
			if(pMes[0] > 64 || pMes[1] > 64) return FALSE;
			g_PreSuffix[0] = pMes[0];
			g_PreSuffix[1] = pMes[1];
			g_nFixedLen = 6;
	        g_nPreSuffix = 2;
		}
		break;

	case DataSetType_1DCodeID://一维条码CodeID前缀设置
	case DataSetType_2DCodeID://二维条码CodeID前缀设置
	case DataSetType_PostalCodeID://邮政码CodeID前缀设置
		i = strlen((char *)pMes);
		if(i > 2) return FALSE;
		memcpy(g_PreSuffix, pMes, i);
		g_nPreSuffix = i;
		g_DataSet_Odd = TRUE;
		break;

	case DataSetType_Cut: //截取设置
		if(g_PosMaxMin == 0){	//截取方式设置
			if(pMes[0] < 0x20 || pMes[0] == 0x40 || (pMes[0] >= 96 && pMes[0] <= 127)){ //一维条码或OCR_B，或邮政码
				for(i = 1; i < 16; i += 3){		//一维条码截取最多5段数据，每段3位，加上条码类型一位，共16位
					if(pMes[i] > 0x01) break;	//判断截取方向位，非0即1
					if(pMes[i + 1] == 0x00 || pMes[i + 2] == 0x00) break;	//起始位和终止位不能为0
				}
				nValue = i;
			}
			else if(pMes[0] > 0x1f && pMes[0] < 0x40){ //二维条码
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

	case DataSetType_Fixed_Dis://I2odf5条码禁止识读固定长度设置
		if(strlen((char *)pMes) == 1){ //设置值
			if(pMes[0] > 64) return FALSE;
			g_PreSuffix[0] = pMes[0];
			g_nFixedLen = 3;
	        g_nPreSuffix = 1;
		}
		else { //设置范围
			if(pMes[0] > 64 || pMes[1] > 64) return FALSE;
			g_PreSuffix[0] = pMes[0];
			g_PreSuffix[1] = pMes[1];
			g_nFixedLen = 6;
	        g_nPreSuffix = 2;
		}
		break;

	case DataSetType_OnceDelay://一次读码延迟设置
	case DataSetType_InfoDelay://两次相同信息读码的时间延迟
	case DataSetType_Sensibility: //灵敏度设置
	case DataSetType_ScanTime: //读码次数限定设置
	case DataSetType_NonStandar: //非标参数设置
	case DataSetType_Oscillation: //振子时间设置
	case DataSetType_ReadBeepFreq: //解码成功蜂鸣频率
	case DataSetType_ReadBeepDura: //解码成功蜂鸣单次持续时间
	case DataSetType_ReadLEDDura:	//解码成功LED持续时间
		nValue = StringToInt(pMes);
		if(nValue == -1) return FALSE;
		g_DataSet = (unsigned int)nValue;
		break;

	case DataSetType_KBW: //键盘参数设置
		if(g_PosMaxMin == DataSetKBW_Nationality || g_PosMaxMin == DataSetKBW_CharDelay
		  || g_PosMaxMin == DataSetKBW_CodeDelay || g_PosMaxMin == DataSetKBW_CodePage){
			nValue = StringToInt(pMes);
			if(nValue == -1) return FALSE;
			g_DataSet = (unsigned int)nValue;
		}
		else return FALSE;
		break;
	case DataSetType_BlueT: //蓝牙参数设置
		if(g_PosMaxMin == DataSetBlueT_Name){
			if(strlen((char *)pMes) > 0x10) return FALSE; //名称长度不超过16字节
			g_nPreSuffix = strlen((char *)pMes);
			memcpy(g_PreSuffix, pMes, g_nPreSuffix);
			g_DataSet_Odd = TRUE;
		}
		else if(g_PosMaxMin == DataSetBlueT_Password){
			if(strlen((char *)pMes) > 6) return FALSE; //密码长度6个字符
			g_nPreSuffix = strlen((char *)pMes);
			memcpy(g_PreSuffix, pMes, g_nPreSuffix);
			g_DataSet_Odd = TRUE;
		}
		break;

	case DataSetType_DecodeArea://读码区域设置
		nValue = StringToInt(pMes);
		if(nValue == -1 || nValue > 100) return FALSE;
		g_DataSet = (unsigned int)nValue;
   	    break;
	
	case DataSetType_DataFormatting:
		//命令长度不可超过512字节
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
//从数字字符串中获取数字
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

//设置码公共参数设置初始化
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

//取消设置，包括取消一个字符，一串字符和一个操作
BOOL SetCode_Discard(int Rec56)
{
	if(Rec56 == POS_Discard1Data){//取消前一次读的一位数据
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
		{   //读码固定长度设置或信息截取设置
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
	else if(Rec56 == POS_DiscardAllData){//取消前面读的一串数据
		g_DataSet = 0;
		g_DataSet_Odd = TRUE;
		g_nPreSuffix = 0;
		g_nFixedLen = 0;
		g_nDataFormatbuf = 0;
		memset(g_PreSuffix, 0, 20);
		memset(g_DataFormatbuf, 0, 512);
	}
	else if(Rec56 == POS_DiscardSet){
		//取消当前最大最小读码或图象亮度或前后缀设置
		W_setcParamInit();
	}
	else return FALSE;

	return TRUE;
}

//前后缀数据设置
BOOL SetCode_Data_PreSuffix(int Rec56)
{
	if(g_nPreSuffix >= Max_nPreSubfix && g_DataType != DataSetType_BlueT)
		return FALSE; //前后缀信息至多10个字节
	if(g_nPreSuffix > 1 && (g_DataType == DataSetType_1DCodeID 
		|| g_DataType == DataSetType_2DCodeID || g_DataType == DataSetType_PostalCodeID)) 
		return FALSE; //CodeID前缀的长度至多2个字节
	
	if((g_nPreSuffix > 1) && (g_DataType == DataSetType_Terminal)) 
		return FALSE; //终止符长度至多2个字节

	if(g_DataSet_Odd) {
		g_DataSet = Rec56;
		g_DataSet_Odd = FALSE;
	}
	else{
		g_DataSet = (g_DataSet << 4) + Rec56; //前后缀信息数据采用16进制
		g_PreSuffix[g_nPreSuffix] = g_DataSet;
		g_nPreSuffix++;
		g_DataSet_Odd = TRUE;
	}
	return TRUE;
}

//读码固定长度设置
BOOL SetCode_Data_FixedLen(int Rec56)
{
	//三个数据构成一个固定长度
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
		if ((g_DataSet > 0x7f) && (g_DataType != DataSetType_Cut)) return FALSE; //一维条码的最大读码信息为127
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
		len = HEADLEN;//format头设置完，len恢复初值，保证下一次设置dataformat时len还是12
		c = int_to_char(Rec56);
		if (c == 0) return FALSE;
		g_DataFormatbuf[g_nDataFormatbuf] = c;//int_to_char(Rec56);
		g_nDataFormatbuf++;
	}
	return TRUE;
}
//保存一维条码最小读码设置
BOOL SetCode_Save_1DMin()
{
// 	int nMaxlen;
	if((g_DataSet > 0x7f)||(g_DataSet < 1)) return FALSE; //一维条码长度不超过127,不小于1

// 	nMaxlen = (X_paramGetGlobalAllParam()->p1D->ExMsg[g_PosMaxMin] >> 24) & 0xff;
// 	if (g_DataSet > nMaxlen) return FALSE;	//最小值>最大值

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

//保存一维条码最大读码设置
BOOL SetCode_Save_1DMax()
{
// 	int nMinlen;
	if((g_DataSet > 0x7f)||(g_DataSet < 1)) return FALSE; //一维条码长度不超过127
	
// 	nMinlen = (X_paramGetGlobalAllParam()->p1D->ExMsg[g_PosMaxMin] >> 16) & 0xff;
// 	if (g_DataSet < nMinlen) return FALSE;	//最大值<最小值

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

//保存二维条码最小读码设置
BOOL SetCode_Save_2DMin()
{
	int nMaxlen;
	if((g_DataSet > 0xffff)||(g_DataSet < 1)) return FALSE; //二维条码长度不超过65535

	nMaxlen = (X_paramGetGlobalAllParam()->p2D->ExMsg[g_PosMaxMin] >> 16) & 0xffff;
	if (g_DataSet > nMaxlen) return FALSE;	//最小值>最大值

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
	case POS_HAN_SENSIBLE: //汉信码
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

//保存二维条码最大读码设置
BOOL SetCode_Save_2DMax()
{
	int nMinlen;
	if((g_DataSet > 0xffff)||(g_DataSet < 1)) return FALSE; //二维条码长度不超过65535

	nMinlen = X_paramGetGlobalAllParam()->p2D->ExMsg[g_PosMaxMin] & 0xffff;
	if (g_DataSet < nMinlen) return FALSE;	//最大值<最小值

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
	case POS_HAN_SENSIBLE: //汉信码
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

//保存OCR最小长度设置
BOOL SetCode_Save_OCRMin()
{
	int nMaxlen;
	if(g_DataSet < 1) return FALSE; 
	nMaxlen = (X_paramGetGlobalAllParam()->pOCR->ExMsg[g_PosMaxMin] >> 24) & 0xff;
	if (g_DataSet > nMaxlen) return FALSE;	//最小值>最大值

	switch(g_PosMaxMin){
	case POS_SPEC_OCR_B: //SPEC_OCR_B
	    X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] & 0xff00ffff) | (g_DataSet<<16);
		break;	
	default: return FALSE;
	}
	return TRUE;
}

//保存OCR最大长度设置
BOOL SetCode_Save_OCRMax()
{
	int nMinlen;
	if(g_DataSet < 1) return FALSE; 
	nMinlen = (X_paramGetGlobalAllParam()->pOCR->ExMsg[g_PosMaxMin] >> 16) & 0xff;
	if (g_DataSet < nMinlen) return FALSE;	//最大值<最小值

	switch(g_PosMaxMin){
	case POS_SPEC_OCR_B: //SPEC_OCR_B
	    X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = (X_paramGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] & 0x00ffffff) | (g_DataSet<<24);
		break;	
	default: return FALSE;
	}
	return TRUE;
}

//保存设置
BOOL SetCode_Save()
{
	int i;
	unsigned char *pbyCut;

      DF_Param	dfparam;
      int flag;

	switch(g_DataType)
	{
	case DataSetType_No: //用户设置码的最后保存
		if(g_ReplaceType == Replace_UserPart)
		{
			W_setcSetSaveFlag(TRUE);
			return TRUE;
		}//End of if;
		break;
	case DataSetType_1DMin://一维码读码最小长度设置
		if(!SetCode_Save_1DMin()) return FALSE;
	    break;
    case DataSetType_1DMax://一维码读码最大长度设置
		if(!SetCode_Save_1DMax()) return FALSE;
   	    break;
    case DataSetType_2DMin://二维码读码最小长度设置
		if(!SetCode_Save_2DMin()) return FALSE;
	    break;
    case DataSetType_2DMax://二维码读码最大长度设置
	    if(!SetCode_Save_2DMax()) return FALSE;
		break;
	case DataSetType_OnceDelay://一次读码延迟设置
		if(g_DataSet > Max_Delay || g_DataSet < 500) return FALSE;
		X_paramBakGetGlobalAllParam()->pCode->nOnceDelay = g_DataSet;
		break;
	case DataSetType_InfoDelay://两次相同信息读码的时间延迟
		if(g_DataSet > Max_Delay) return FALSE;
		X_paramBakGetGlobalAllParam()->pCode->nInfoDelay = g_DataSet;
		break;

	case DataSetType_Prefix://前缀信息设置
		if(!g_DataSet_Odd)
		{ return FALSE; }
		memcpy(X_paramBakGetGlobalAllParam()->pCode->byPrefix, g_PreSuffix, 10);
		memset(g_PreSuffix, 0, 20);
		X_paramBakGetGlobalAllParam()->pCode->nPrefix = g_nPreSuffix;
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		break;

	case DataSetType_Suffix://后缀信息设置
		if(!g_DataSet_Odd)
		{ return FALSE; }
		memcpy(X_paramBakGetGlobalAllParam()->pCode->bySuffix, g_PreSuffix, 10);
		memset(g_PreSuffix, 0, 20);
		X_paramBakGetGlobalAllParam()->pCode->nSuffix = g_nPreSuffix;
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		break;

	case DataSetType_Terminal://结束符后缀设置
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

	case DataSetType_FixedLen://条码识读固定长度设置
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
			{//只添加一个固定值
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
			{ //添加一段固定值
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
	case DataSetType_OCRMin://OCR最小长度设置
	    if(!SetCode_Save_OCRMin()) 
		{ return FALSE; }
		break;
	case DataSetType_OCRMax://OCR最大长度设置
	    if(!SetCode_Save_OCRMax())
		{ return FALSE; }
		break;
	case DataSetType_1DCodeID://一维条码CodeID前缀设置
		if(!g_DataSet_Odd)
		{ return FALSE; }
		//检测设置字符是否为字母
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
	case DataSetType_2DCodeID://二维条码CodeID前缀设置
		if(!g_DataSet_Odd)
		{ return FALSE; }
		//检测设置字符是否为字母
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

	case DataSetType_PostalCodeID://邮政码CodeID前缀设置
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

	case DataSetType_Cut: //截取设置
		pbyCut = X_paramBakGetGlobalAllParam()->pCode->byCut;
		if(g_PosMaxMin == 1){ //取消某种条码的截取方式
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
		//0~31:一维条码，32~63:二维条码，64:OCR_B
		if(g_PreSuffix[0] <= 0x7f){
			if(g_PreSuffix[0] < 0x20 || (g_PreSuffix[0] >= 64 && g_PreSuffix[0] <= 127)){//一维码 OCR 邮政码
				if((g_nPreSuffix - 1) / 3 * 3 != (g_nPreSuffix - 1))
				{ return FALSE; }
			 	//方向只有0正向和1反向两种
			    if(g_PreSuffix[1] > 1 || g_PreSuffix[4] > 1 || g_PreSuffix[7] > 1
			    || g_PreSuffix[10] > 1 || g_PreSuffix[13] > 1 || g_PreSuffix[16] > 1)
				{ return FALSE; }

				//位置必须是正数, 且小于128
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
			    
				//方向只有0正向和1反向两种
			    if(g_PreSuffix[1] > 1 || g_PreSuffix[6] > 1 || g_PreSuffix[11] > 1)
				{ return FALSE; }
				
				//位置必须是正数, 且小于10000
				for(i = 2; i < g_nPreSuffix; i += 5)
				{
					if((g_PreSuffix[i] * 10 + g_PreSuffix[i + 1] < 1) 
						|| (g_PreSuffix[i] * 10 + g_PreSuffix[i + 1] > 9999) 
						|| (g_PreSuffix[i + 2] * 10 + g_PreSuffix[i + 3] < 1)
						|| (g_PreSuffix[i + 2] * 10 + g_PreSuffix[i + 3] > 9999)) 
					{ return FALSE; }
				}//End of for;
			}//End of if;

		    if(pbyCut[(Max_DataCutLen << 1)] == g_PreSuffix[0]){//最新的一组是当前的类型
			}
			else if(pbyCut[Max_DataCutLen] == g_PreSuffix[0]){//第2组是当前的类型
				memcpy(pbyCut + Max_DataCutLen, pbyCut + (Max_DataCutLen << 1), Max_DataCutLen);
			}
			else{
				//倒数第三次设置一组数据将被删除
				memcpy(pbyCut, pbyCut + Max_DataCutLen, Max_DataCutLen);
				memcpy(pbyCut + Max_DataCutLen, pbyCut + (Max_DataCutLen << 1), Max_DataCutLen);
			}
			memcpy(pbyCut + (Max_DataCutLen << 1) + 2, g_PreSuffix + 1, Max_DataCutLen - 2);
			
			pbyCut[Max_DataCutLen << 1] = g_PreSuffix[0]; //存储设置的类型
			if(g_PreSuffix[0] < 0x20 || (g_PreSuffix[0] >= 96 && g_PreSuffix[0] <= 127))
				pbyCut[(Max_DataCutLen << 1) + 1] = (g_nPreSuffix - 1) / 3; //存储截取的段数
			else if(g_PreSuffix[0] < 0x40)
				pbyCut[(Max_DataCutLen << 1)+ 1] = (g_nPreSuffix - 1) / 5; //存储截取的段数

			memset(g_PreSuffix, 0, 20);
			g_nPreSuffix = 0;
			g_nFixedLen = 0;
		}//End of if;
		else
		{ return FALSE; }
		break;

	case DataSetType_Fixed_Dis://I2odf5条码禁止识读固定长度设置
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
			{//只禁止一个固定值
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
			{ //禁止一段固定值
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
	case DataSetType_Sensibility: //灵敏度值设置
		if(g_DataSet > 20 || g_DataSet < 1)
		{ return FALSE; }//灵敏度最大50
		X_paramBakGetGlobalAllParam()->pCode->nSensitive = g_DataSet;
		break;
	case DataSetType_ScanTime: //读码次数限定设置
		if(g_DataSet > 65535)
		{ return FALSE; }
		X_paramBakGetGlobalAllParam()->pCode->nScanTimes = g_DataSet;
		break;
	case DataSetType_NonStandar: //非标参数值设置
		if(!SetCode_Save_NonStandar())
		{ return FALSE; }
		break;
	case DataSetType_ErrorMsg: //一定时间内没读到码返回信息设置
		if(!g_DataSet_Odd)
		{ return FALSE; }
		memcpy(X_paramBakGetGlobalAllParam()->pCode->byErrMsg, g_PreSuffix, 10);
		memset(g_PreSuffix, 0, 20);
		X_paramBakGetGlobalAllParam()->pCode->nErrMsg = g_nPreSuffix;
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		break;
	case DataSetType_Oscillation: //振子设置
		if(g_DataSet > 20000)
		{ return FALSE; }//振子时间最大20000
		X_paramBakGetGlobalAllParam()->pHard->nOscillationDelay = g_DataSet;
		break;
	case DataSetType_KBW: //KBW参数设置
		if(!SetCode_Save_KBW())
		{ return FALSE; }
		break;
	case DataSetType_BlueT: //蓝牙参数设置
		//蓝牙参数设置
		if(!SetCode_Save_BlueT()) 
		{ return FALSE; }
		break;

	case DataSetType_DecodeArea: //读码区域设置
		if(g_DataSet > 100 || g_DataSet < 1)
		{ return FALSE; }
		X_paramBakGetGlobalAllParam()->pCode->decodeArea = (g_DataSet << 4) + (X_paramGetGlobalAllParam()->pCode->decodeArea&0xf);
		break;

	case DataSetType_ReadBeepFreq://自定义解码成功蜂鸣频率
		if(g_DataSet < 20 || g_DataSet > 20000) return FALSE;
		X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0xf000;
		X_paramBakGetGlobalAllParam()->pHard->nBeepType |= (9 << 12);	//将频率在beeptype中的mask设为9,来标识启用自定义变量
		X_paramBakGetGlobalAllParam()->pCode2->nSucBeepFreq = g_DataSet;
		break;
	case DataSetType_ReadBeepDura://自定义解码成功单次蜂鸣持续时间
		if(g_DataSet < 20 || g_DataSet > 300) return FALSE;
		X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0xf00;
		X_paramBakGetGlobalAllParam()->pHard->nBeepType |= (9 << 8);   //将持续时间在beeptype中的mask设为9,来标识启用自定义变量
		X_paramBakGetGlobalAllParam()->pCode2->nSucBeepDuration = g_DataSet;
		break;
		
	case DataSetType_ReadLEDDura://自定义解码成功LED持续时间
		if(g_DataSet < 1 || g_DataSet > 10000) return FALSE;
		X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 4;
		X_paramBakGetGlobalAllParam()->pCode2->nSucLedDuration = g_DataSet;
		break;

	case DataSetType_DataFormatting:
		i = g_DataFormatbuf[0]-0x30;

		g_nDataFormatbuf = 0;
		if (g_PosMaxMin == 1)//清除某组dataformat
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
			
			dfparam.nDFSelType = 3;//库里面只接受命令类型位1、2或3			
			
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
		if (g_DataFormatbuf[0] == 0)//取消前面读取的一串数据，此处不能return false，否则，保存报错，且下一次串口发送命令总是返回0x15
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

//非标参数值设置
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

//条码综合设置
BOOL SetCode_Total(int Rec56, int Rec7)
{
	W_setcParamInit();
	switch(Rec56){
	case TotalSet_All_Default://所有属性设置出厂默认值
		if(!X_paramLoadFactoryParam()) {
			//无出厂配置的情况 
			if(!X_paramOriginalInitAllParam()) return FALSE;
		}
	//	if(!X_paramSaveAsSysDefaultParam()) return FALSE;
		break;
	case TotalSet_All_Off://禁止读所有条码
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = 0x01; //只识别设置码
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = 0;
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = 0;
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes = 0;
		break;
	case TotalSet_All_On://所有条码可读
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = 0xffffffff;
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = 0xffffffff;
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = 0xffffffff;
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes = 0xffffffff;
		break;
	case TotalSet_All_1D_Off://禁止读所有一维条码
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = 0x01; //只识别设置码
		break;
	case TotalSet_All_1D_On://所有一维条码可读
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes = 0xffffffff;
		break;
	case TotalSet_All_2D_Off://禁止读所有二维条码
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = 0;
		break;
	case TotalSet_All_2D_On://所有二维条码可读
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes = 0xffffffff;
		break;
	case TotalSet_All_Postal_Off: //禁止读所有邮政码
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = 0;
		break;
	case TotalSet_All_Postal_On: //所有邮政码可读
		X_paramBakGetGlobalAllParam()->pPostal->unDecTypes = 0xffffffff;
		break;
	case TotalSet_All_1D_OnlyOne: //一维条码只读单码
		X_paramBakGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
		break;
	case TotalSet_All_1D_OneOrTwo: //可读双一维码（同一类型），但也可以读单一维码
		X_paramBakGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->unDecParam |= 0x02; 
		break;
	case TotalSet_All_1D_OnlyTwo: //可读双一维码（同一类型），且只读双一维码
		X_paramBakGetGlobalAllParam()->p1D->unDecParam |= 0x03;
		break;
	case TotalSet_UserDef_On: //启动用户自定义默认设置
		g_ReplaceType = Replace_UserDef;
		break;
	case TotalSet_UserDefPart_On: //启动用户自定义默认设置
		g_ReplaceType = Replace_UserPart;
		break;
	case TotalSet_SN_On: //启动产品SN设置
		if(Rec7 == 0)
			g_ReplaceType = Replace_SN;
		else if(Rec7 == 1){
			X_paramPlaceKeyword((char*)"S/N", (char*)"", 1);
			X_paramSaveDeviceInfo();
		}		
		break;
	case TotalSet_ESN_On: //启动产品ESN设置
		if(Rec7 == 0)
			g_ReplaceType = Replace_ESN;
		else if(Rec7 == 1){
			X_paramPlaceKeyword((char*)"ESN", (char*)"", 1);
			X_paramSaveDeviceInfo();
		}
		break;
	case TotalSet_Date_On: //启动产品Date设置
		if(Rec7 == 0)
			g_ReplaceType = Replace_Date;
		else if(Rec7 == 1){
			X_paramPlaceKeyword((char*)"DATE", (char*)"", 1);
			X_paramSaveDeviceInfo();
		}
		break;
	case TotalSet_SaveToUserDef: //将当前设置存为用户默认设置
		if(!X_paramSaveAsUsrDefaultParam()) return FALSE;
		break;
	case TotalSet_LoadUserDef: //载入用户默认设置
		if(!X_paramLoadUsrDefaultParam()) return FALSE;
		break;
	case TotalSet_LIC_On: //设置产品Licence
		g_ReplaceType = Replace_LIC;
		break;
	case TotalSet_ManufID_On: //设置产品ManufactureID
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
	case TotalSet_SaveToFactory: //将当前设置存为出厂设置
		if(!X_paramSaveAsFactoryParam()) return FALSE;
		break;
	case TotalSet_Load_General://载入通用设置
		if(!X_paramOriginalInitAllParam()) return FALSE;
	//	if(!X_paramSaveAsSysDefaultParam()) return FALSE;
		break;
	case TotalSet_ManufName_On: //设置产品ManufactureName
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

//数据码与综合设置
BOOL Set_DataAndTotal(int Rec34, int Rec56, int Rec7)
{
	switch(Rec34){
	case SubSet_Data://数据码
		if(SetCode_Discard(Rec56)) {return TRUE;}
		else if(Rec56 < 0x10 && Rec56 >= 0 &&
			(g_DataType == DataSetType_Prefix || g_DataType == DataSetType_Suffix
			|| g_DataType == DataSetType_Terminal || g_DataType == DataSetType_1DCodeID
			|| g_DataType == DataSetType_2DCodeID || g_DataType == DataSetType_BlueT)){
				//设置前后缀
				if(!SetCode_Data_PreSuffix(Rec56)) return FALSE;
		}
		else if(Rec56 < 0x10 && Rec56 >= 0 && (g_DataType == DataSetType_DataFormatting)){	
				if (!SetCode_Data_DataFormat(Rec56)) return FALSE;	
		}
		else if(Rec56 < 0x0a && Rec56 >= 0 && (g_DataType == DataSetType_FixedLen
			|| g_DataType == DataSetType_Cut || g_DataType == DataSetType_Fixed_Dis)){
		    //设置固定读码长度
			if(!SetCode_Data_FixedLen(Rec56)) return FALSE;
		}
		else if(Rec56 < 10 && Rec56 >= 0){//其它数据设置
			g_DataSet = g_DataSet * 10 + Rec56;

			//一维码读码最大最小长度设置越界判断
			if(((g_DataType == DataSetType_1DMax) || (g_DataType == DataSetType_1DMin)) && (g_DataSet > 0x7f)) 
				return FALSE;

			//OCR最大最小长度设置越界判断
			if(((g_DataType == DataSetType_OCRMax) || (g_DataType == DataSetType_OCRMin)) && (g_DataSet > 0x7f)) 
				return FALSE;

			//二维码读码最大最小长度设置越界判断
			if(((g_DataType == DataSetType_2DMax) || (g_DataType == DataSetType_2DMin)) && (g_DataSet > 0xffff)) 
				return FALSE;

			//延时设置越界判断
			if((g_DataType == DataSetType_OnceDelay || g_DataType == DataSetType_InfoDelay) && g_DataSet > Max_Delay)
				return FALSE;
		}
		else if(Rec56 == POS_Save){ //保存
			if(!SetCode_Save()) return FALSE;
		}
		else return FALSE;
		break;

	case SubSet_Total: //条码综合设置
		if(!SetCode_Total(Rec56, Rec7)) return FALSE;
		break;

	case SubSet_SetCodeSend: //设置码信息发送设置
		if(Rec56 == 0 || Rec56 == 1)
			X_paramBakGetGlobalAllParam()->pCode->SetCode = (X_paramGetGlobalAllParam()->pCode->SetCode & (~0x01)) | Rec56;
		else return FALSE;
		break;

	case SubSet_SetSysMesSend: //系统相关信息发送设置
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
		
		W_setcSetNoSaveFlag(TRUE);//不保存

		break;
	case SubSet_Set1DCodeID: //自定义一维条码的CodeID前缀设置
		if(Rec56 > 0x1f) return FALSE; //至多32种一维条码
		g_DataType = DataSetType_1DCodeID;
		memset(g_PreSuffix, 0, 20);
		g_PosMaxMin = Rec56;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		break;
	case SubSet_Set2DCodeID: //自定义二维条码的CodeID前缀设置
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
		else if((Rec56 >= 40) && (Rec56 <= 72))	//将邮政码的CodeID前缀也暂时放在这里
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
	case SubSet_IsOpenSetCode: //设置码功能启动与关闭设置
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
	case SubSet_IsPowerOn: //电源开启时是否显示系统信息设置
		if(Rec56 == 0 || Rec56 == 1)
		    X_paramBakGetGlobalAllParam()->pCode->IsPowerOnDisp = Rec56;
		else if(Rec56 == 2 || Rec56 == 3)
			X_paramBakGetGlobalAllParam()->pCode->IsGotoConsole = Rec56 - 2;
		else return FALSE;
		break;
	case SubSet_CloseDevice: //关机设置
		break;
	default: return FALSE;
	}
	return TRUE;
}

//通讯设置
BOOL Set_Comm(int Rec34, int Rec56)
{
#if defined(MB1126)
	return TRUE;
#endif 
	switch(Rec34){
    case SubSet_BoundRate://波特率设置
		switch(Rec56) {
		case 0:	case 1:	case 2:	case 3:	case 4:
		case 5:	case 6:	case 7:	case 8:
			X_paramBakGetGlobalAllParam()->pCom->lRate = conBaudRate[Rec56];
			break;
		default: return FALSE;
		}    
		break;
	case SubSet_Parity://校验设置
		if(Rec56 == NOPARITY) X_paramBakGetGlobalAllParam()->pCom->lParity = NOPARITY;
		else if(Rec56 == EVENPARITY) X_paramBakGetGlobalAllParam()->pCom->lParity = EVENPARITY;
		else if(Rec56 == ODDPARITY) X_paramBakGetGlobalAllParam()->pCom->lParity = ODDPARITY;
		else return FALSE;
	    break;
    case SubSet_WordLenStop://字符长度停止位设置
		if(Rec56 == 0 || Rec56 == 1){
		    X_paramBakGetGlobalAllParam()->pCom->lStopBits = Rec56 + 1;
		}
		else return FALSE;
	    break;
	case SubSet_WordLenDataBits://字符长度数据位设置
// 		if(Rec56 >= 0 && Rec56 < 4){
		if(Rec56 >= 2 && Rec56 < 4){
		    X_paramBakGetGlobalAllParam()->pCom->lDataBits = Rec56 + 5;
		}
		else return FALSE;
		break;
	case SubSet_HardwareFlowCtrl://硬件流控设置
		if(Rec56 == 0 || Rec56 == 1){
			X_paramBakGetGlobalAllParam()->pCom->nflowCtrl = Rec56;
		}
		else return FALSE;
		break;
	default:return FALSE;
	}
 
	return TRUE;
}

//硬件设置
BOOL Set_Hard(int Rec34, int Rec56, int Rec7)
{
	char b[10];
	switch(Rec34){
	case 0://照明灯设置
		if(Rec56 > 3 || Rec56 < 0) return FALSE;
// 		if (Rec56 == 1) return FALSE; //不能常亮

		X_paramBakGetGlobalAllParam()->pHard->nLight = Rec56;
		break;

	case 1://对焦灯设置
// 		if(Rec56 > 2 || Rec56 < 0) return FALSE;
		if(Rec56 > 3 || Rec56 < 0) return FALSE;
// 		if (Rec56 == 1) return FALSE; //不能常亮
		X_paramBakGetGlobalAllParam()->pHard->nFocus = Rec56;
//		if(X_paramGetGlobalAllParam()->pHard->nFocus == 3)
//		{ X_paramGetGlobalAllParam()->pCode->nReadMode = ESC2; }
// 		if(X_paramGetGlobalAllParam()->pHard->nFocus == 3)
// 		{ X_paramGetGlobalAllParam()->pHard->nFocus = 0; }
		break;
	case 2: //硬件类型设置
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
// 		else if (Rec56 == PRODUCTION_DOF)	//景深类型
// 		{
// 			if (Rec7 == 0)			//近景深
// 			{
// 				X_paramGetGlobalAllParam()->pHard->nDofType = PRODUCTION_DOF_NEAR;
// 				X_paramPlaceKeyword((char*)"DofType", (char*)"HD", 1);	//High Density
// 
// 			}
// 			else if (Rec7 == 1)		//中景深
// 			{
// 				X_paramGetGlobalAllParam()->pHard->nDofType = PRODUCTION_DOF_MIDDLE;
// 				X_paramPlaceKeyword((char*)"DofType", (char*)"SR", 1);	//Standard Range
// 			}
// 			else if (Rec7 == 2)		//远景深
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
    case 3: //读码声音参数设置
		if(Rec56 < 2){ //读码声音开打关闭设置
			X_paramBakGetGlobalAllParam()->pHard->bDecVoice &= ~0x01;
			X_paramBakGetGlobalAllParam()->pHard->bDecVoice |= Rec56;
		}
		else if(2 == Rec56){ //设置解码声音类型
			if(Rec7 < 3){ //目前只提供三种声音类型
				X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0x0f;
				X_paramBakGetGlobalAllParam()->pHard->nBeepType |= Rec7;
			}
			else return FALSE;
		}
		else if(3 == Rec56){ //设置解码声音的音量
			if(Rec7 < 3){ //目前只提供高中低三种音量
				X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0xf0;
				X_paramBakGetGlobalAllParam()->pHard->nBeepType |= (Rec7 << 4);
			}
			else return FALSE;
		}
		else if(5 == Rec56){ //设置解码声音的持续时间duration  020305x
			if(Rec7 < 2){
				X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0xf00;
				X_paramBakGetGlobalAllParam()->pHard->nBeepType |= (Rec7 << 8);
			}
			else if(Rec7 == 2){ //设置解码0203052
				g_DataType = DataSetType_ReadBeepDura;
				g_DataSet = 0;
				g_IsSetCodeStart = TRUE;
			}
			else return FALSE;
		}
		else if(6 == Rec56){ //设置解码声音的频率freq 020306x
			if(Rec7 < 4){
				//bit15 - bit12	频率	0 = Medium; 1 = Lowest; 2 = Low; 3 = Medium; 4 = High; 9 = 用户定义
				//对应设置码的Rec7： 0 = Lowest; 1 = Low; 2 = Medium; 3 = High; 4 = 用户定义
				//频率暂定为：800、 1600、 2730、 4200。		
				X_paramBakGetGlobalAllParam()->pHard->nBeepType &= ~0xf000;
				X_paramBakGetGlobalAllParam()->pHard->nBeepType |= ((Rec7 + 1)<< 12);
			}
			else if(Rec7 == 4){ //用户自定义解码声音频率 0203064
				g_DataType = DataSetType_ReadBeepFreq;
				g_DataSet = 0;
				g_IsSetCodeStart = TRUE;
			}
			else return FALSE;
		}
		else return FALSE;
		break;
	case 4: //开机声音设置
		if(!Rec56){
			if(Rec7 < 2){ //读码声音开打关闭设置
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
	case 5://外部照明灯设置
		if(Rec56 > 1 || Rec56 < 0) return FALSE;	
		X_paramBakGetGlobalAllParam()->pHard->nExLight = Rec56;
		break;

	case 6://读码成功指示灯设置
		if(Rec56 == 1)//读码成功指示灯设置
		{
			if (Rec7 == 1)	//开启
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLED &= ~ 1;
			}
			else if (Rec7 == 0)	//关闭
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLED |= 1;
			}
			else if (Rec7 == 2)//串行
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLED &= ~0x80;				
			}
			else if (Rec7 == 3)//并行
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLED |= 0x80;				
			}
			else
				return FALSE;

		}
		else if (Rec56 == 2)//读码成功指示灯持续时间
		{
			if (Rec7 == 0)//短
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 0;
			}
			else if (Rec7 == 1)//中
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 1;				
			}
			else if (Rec7 == 2)//长
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 2;
			}
			else if (Rec7 == 3)//最长
			{
				X_paramBakGetGlobalAllParam()->pHard->nGoodreadLEDDuration = 3;
			}
			else if (Rec7 == 4)//自定义
			{
				g_DataType = DataSetType_ReadLEDDura;
				g_DataSet = 0;
				g_IsSetCodeStart = TRUE;
			}
			else
				return FALSE;

		}
#ifdef BUILT_HR32
		else if (Rec56 == 0)//电源指示灯设置
		{
			if (Rec7 == 0)//关
				X_paramBakGetGlobalAllParam()->pCode2->bPowerLedOn = FALSE;
			else if (Rec7 == 1)//开
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

//解码模式符设置
BOOL Set_DecodeMode(int Rec34, int Rec56, int Rec7, int RecParam)
{
    char buff[1152];
    unsigned int i;
    
	W_setcSetSaveFlag(TRUE);
	switch(Rec34){
	case DecModeSet_Prefix://前缀设置
	    if(!Rec56){
		    g_DataType = DataSetType_Prefix;
		    memset(g_PreSuffix, 0, 20);
		    g_nPreSuffix = 0;
		    g_DataSet_Odd = TRUE;
		    g_IsSetCodeStart = TRUE;
		}
		else return FALSE;
		break;
    case DecModeSet_Suffix://自定义后缀设置
		if(!Rec56) {
			g_DataType = DataSetType_Suffix;
		    memset(g_PreSuffix, 0, 20);
		    g_nPreSuffix = 0;
		    g_DataSet_Odd = TRUE;
		    g_IsSetCodeStart = TRUE;
		}
		else return FALSE;
	    break;
    case DecModeSet_Repeat://读码扫描方式设置
		switch(Rec56) {
		case 0://手动读码
			X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC0;
		    X_paramBakGetGlobalAllParam()->pMode->bContinue = TRUE;
			break;
		case 1://自动读码
			X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC2;
		    X_paramBakGetGlobalAllParam()->pMode->bContinue = TRUE;
	    	X_paramBakGetGlobalAllParam()->pMode->nReadDelay = Z_Get10msCounter();
		    X_paramBakGetGlobalAllParam()->pMode->nHaltDelay = Z_Get10msCounter();
		    X_paramBakGetGlobalAllParam()->pMode->nCodeDelay = Z_Get10msCounter();
			break;
		case 2://连续读码
			X_paramBakGetGlobalAllParam()->pMode->bContinue = FALSE;
		    X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC3;
			break;
		case 3://
			X_paramBakGetGlobalAllParam()->pCode->nReadMode = ESC1;
		    X_paramBakGetGlobalAllParam()->pMode->bContinue = TRUE;
			W_setcSetNoSaveFlag(TRUE);//不保存
		    X_paramBakGetGlobalAllParam()->pMode->nReadDelay = Z_Get10msCounter();
			break;
		case 9:
			if(Rec7 == 0)
			{
				if(RecParam <= 0)
					U_appSetTestMode(1, 1000); //按次数测试
				else
					U_appSetTestMode(1, RecParam);
			}
			else if(Rec7 == 1)
			{
				if(RecParam <= 0)
					U_appSetTestMode(2, 60*1000);//按时间测试
				else
					U_appSetTestMode(2, RecParam * 1000);
			}
			else if(Rec7 == 2)
			{
				if(RecParam <= 0)
					U_appSetTestMode(3, 1000);	//按成功次数测试
				else
					U_appSetTestMode(3, RecParam);
			}
			else if(Rec7 == 3)
			{
				if(RecParam <= 0)
					U_appSetTestMode(4, 10000);		//误码率测试：10000次
				else
					U_appSetTestMode(4, RecParam);	

			}
			//其它跟测试模式有关的设置码暂时忽略
			break;
		default: return FALSE;
		}
		break;
	case DecModeSet_Protect://图象滤波设置
		if(Rec56 < 2) X_paramBakGetGlobalAllParam()->pCode->Protect = Rec56;
		else return FALSE;
	    break;
	case DecModeSet_IsPrefix://自定义前缀使能或禁止设置
		if(Rec56 < 2) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsPrefix] = Rec56;
		else return FALSE;
		break;
	case DecModeSet_IsSuffix://自定义后缀使能或禁止设置
		if(Rec56 < 2) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsSuffix] = Rec56;
		else return FALSE;
		break;
	case DecModeSet_IsCodeID://CodeID前缀使能或禁止设置
		if(Rec56 < 2) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCodeID] = Rec56;
		else if(Rec56 == 2){ //所有条码采用默认CodeID
            memcpy(X_paramBakGetGlobalAllParam()->pCode->by1DCodeID, CodeIDTab1D, 32);
			memcpy(X_paramBakGetGlobalAllParam()->pCode->by2DCodeID, CodeIDTab2D, 32);
			memcpy(X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID, CodeIDPostal, 32);
            memcpy(X_paramBakGetGlobalAllParam()->pCode2->by1DCodeID_2, CodeIDTab1D_2, 32);
			memcpy(X_paramBakGetGlobalAllParam()->pCode2->by2DCodeID_2, CodeIDTab2D_2, 32);
			memcpy(X_paramBakGetGlobalAllParam()->pCode2->byPostalCodeID_2, CodeIDPostal_2, 32);
		}
		else return FALSE;
		break;
	case DecModeSet_IsAIM://AIM前缀禁止与使能模式设置
// 		if(Rec56 < 4) X_paramGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] = Rec56;
// 		else return FALSE;
		
		if(Rec56 == 0){ //使能设置
			if(Rec7) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] |= 0x01;
			else X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] &= (~0x01);
		}
		else if(Rec56 < 4){
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] &= (~0x06);
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsAIM] |= (((unsigned char)(Rec56 - 1) << 1) | 0x1);
		}
		else return FALSE;
		break;
	case DecModeSet_IsTerminal://结束符后缀使能或禁止设置
		if(Rec56 < 2) X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsTerminal] = Rec56;
		else return FALSE;
		break;
	case DecModeSet_Terminal://自定义结束符后缀设置
		if(Rec56 == 0){
			g_DataType = DataSetType_Terminal;
		    memset(g_PreSuffix, 0, 20);
		    g_nPreSuffix = 0;
		    g_DataSet_Odd = TRUE;
		    g_IsSetCodeStart = TRUE;
		}
		else if (Rec56 == 1)		//添加0x0d;
		{
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[0] = 0x0d;
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[1] = 0;
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[2] = 0;
			X_paramBakGetGlobalAllParam()->pCode->nTerminal = 1;
		}
		else if (Rec56 == 2)		//添加0x0d 0x0a;
		{
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[0] = 0x0d;
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[1] = 0x0a;
			X_paramBakGetGlobalAllParam()->pCode->byTerminal[2] = 0;
			X_paramBakGetGlobalAllParam()->pCode->nTerminal = 2;
		}
		else return FALSE;
		break;
	case DecModeSet_AllPreSuf://所有前后缀使能或禁止设置
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
		
	case DecModeSet_Sensibility:  //灵敏度设置
		if(Rec56 >= 0 && Rec56 < 4) //设置级别
		//	X_paramGetGlobalAllParam()->pCode->nSensitive = 3 - Rec56; //0最高，3最低
		    X_paramBakGetGlobalAllParam()->pCode->nSensitive = SENSITIVE_CALC(Rec56); // 5,10,15,20
		else if(Rec56 == 4) {//设置值
			g_DataType = DataSetType_Sensibility;
			g_DataSet = 0;
			g_IsSetCodeStart = TRUE;
		}
		else return FALSE;
		break;
    
	case DecModeSet_Delay:  //延迟设置
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
	
	case DecModeSet_DataPacked:  //数据打包设置
#if !defined(MB1126) //如果是1126的主板,采用默认包，其他包设置一概不接受
		if(Rec56 >= 0 && Rec56 < 5)
			X_paramBakGetGlobalAllParam()->pCode->nPack = Rec56;
		else return FALSE;
#endif
// 		if(X_paramGetGlobalAllParam()->pCode->nPack == BLUE2D_PACK){
// 			//发送长度信息
// 			X_paramGetGlobalAllParam()->pCode->IsOtherPreSufOn |= (0x01 << POS_IsBlue2D_LenSend);
// 			//不添加条码类型信息
//         	X_paramGetGlobalAllParam()->pCode->IsOtherPreSufOn &= ~(0x01 << POS_IsBlue2D_TypeSend);
// 		}
		break;
	
	case DecModeSet_IsCut: //截取使能设置
		if(Rec56 >= 0 && Rec56 < 2)
			X_paramBakGetGlobalAllParam()->pCode->IsPreSufOn[POS_IsCut] = Rec56;
		break;

	case DecModeSet_Cut: //截取方式设置
		if(!Rec56){
		    g_DataType = DataSetType_Cut;
	        g_PosMaxMin = 0;
	        g_DataSet = 0;
	        g_nPreSuffix = 0;
	        g_nFixedLen = 0;
	        g_IsSetCodeStart = TRUE;
	        memset(g_PreSuffix, 0, 20);
		}
		else if(Rec56 == 1){ //清空某种条码的截取方式
			g_DataType = DataSetType_Cut;
	        g_PosMaxMin = 1;
			g_DataSet = 0;
	        g_IsSetCodeStart = TRUE;
		}
		else if(Rec56 == 2){ //清空最新一组截取方式
			memset(X_paramBakGetGlobalAllParam()->pCode->byCut + (Max_DataCutLen << 1), 0, Max_DataCutLen);
		}
		else if(Rec56 == 3){ //清空全部截取方式
			memset(X_paramBakGetGlobalAllParam()->pCode->byCut, 0, (Max_DataCutLen << 1) + Max_DataCutLen);
		}
		else return FALSE;
		break;
			
	case DecModeSet_PrefixOrder: //前缀顺序设置
// 	    if(X_paramGetGlobalAllParam()->pCode->nUserDef == UserType_BLUE_2D && Rec56 >1)
// 			return FALSE; //BLUE_2D状态下CodeID必须在最前面
		if(Rec56 < 6)
			X_paramBakGetGlobalAllParam()->pCode->PrefixOrder = Rec56;
		break;

	case DecModeSet_ScanTime: //读码次数限定设置
	    g_DataType = DataSetType_ScanTime;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	
	case DecModeSet_Medium: //读码介质设置
		if(Rec56 > 2) return FALSE;
		X_paramBakGetGlobalAllParam()->pCode->nMedium = Rec56+1;
		break;

	case DecModeSet_ErrorMsg: //一定时间内没读到码返回信息设置
		if(Rec56 == 0){//设置禁止
			X_paramBakGetGlobalAllParam()->pCode->bErrMsgEn = FALSE;
		}
		else if(Rec56 == 1){ //设置使能
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
	case DecModeSet_CMOSAdjustMode:	//CMOS曝光模式设置
		if(Rec56 == 0){//普通模式 --纸质码
// 			if (Rec7 == 1)//纸质码 mode 2, 照顾纸质码，景深好，能读手机码
// 			{
// 				X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = 3;
// 			}
// 			else 
			if (Rec7 == 0)	//纸质码mode
			{
				X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode = 0;
			}

		}
// 		else if(Rec56 == 1){ //降低反光模式
// 			X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = 1;
// 		}
		else if (Rec56 == 2) //phone mode
		{
// 			if (Rec7 == 0)	//phone mode1
// 			{
// 				X_paramGetGlobalAllParam()->pCode2->cmosAdjustMode = 4;
// 			}
// 			else
			if (Rec7 == 1)//phone mode2(照明灯亮灭交替)
			{
				X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode = 2;
			}
		}
		else return FALSE;

		Z_SetLightMode(X_paramBakGetGlobalAllParam()->pCode2->cmosAdjustMode);

		break;

	case DecModeSet_DecodeArea:	//读码区域控制
		if(Rec56 == 0){//全区读取
			X_paramBakGetGlobalAllParam()->pCode->decodeArea &= ~0xf;
		}
		else if(Rec56 == 1){ //中心区域读取
			X_paramBakGetGlobalAllParam()->pCode->decodeArea |= 1;
		}
		else if(Rec56 == 2){ //中心区域设置
			g_DataType = DataSetType_DecodeArea;
			g_DataSet = 0;
			g_IsSetCodeStart = TRUE;
		}
		else return FALSE;
		break;
	case DecModeSet_DataFormat:
		if (Rec56 == 0)//数据格式编辑
		{
			g_DataType = DataSetType_DataFormatting;
			g_DataSet = 0;
			g_IsSetCodeStart = TRUE;
		}
		else if (Rec56 == 1)//清空某组dataformatting
		{
			if (Rec7 ==0)//清除某一组format设置
			{
				g_DataType = DataSetType_DataFormatting;
				g_PosMaxMin = 1;//借用改变量，表示要进行某一组dataformat清除的设置
				g_DataSet = 0;
				g_IsSetCodeStart = TRUE;
			} 
			else//清除所有format设置
			{
				memset(X_paramBakGetGlobalAllParam()->pDFormat->format, 0, sizeof(X_paramGetGlobalAllParam()->pDFormat->format));
			}
			
		}
		else if (Rec56 == 2)//禁用
		{			
			X_paramBakGetGlobalAllParam()->pDFormat->DFEnable = Rec7;	
		}
		else if (Rec56 == 3)//format组选择
		{
			X_paramBakGetGlobalAllParam()->pDFormat->usedID = Rec7;
		}
		else if (Rec56 == 4)
		{//Rec7=0:按dataformatting格式输出，所有符合格式的条码+前后缀输出(如果有前后缀)，所有不符合格式的条码不输出，且发出或不发出错误提示音
		 //Rec7=1:按dataformatting格式输出，所有符合格式的条码不+前后缀输出，所有不符合格式的条码不输出，且发出或不发出错误提示音
		 //Rec7=2:符合dataformatting格式的条码按dataformatting格式+前后缀输出(如果有前后缀)，所有不符合格式的条码按原文+前后缀输出(如果有前后缀)
		 //Rec7=3:符合dataformatting格式的条码按dataformatting格式不+前后缀输出，所有不符合格式的条码按原文+前后缀输出(如果有前后缀)
			X_paramBakGetGlobalAllParam()->pDFormat->DFEnable = 1;//使能 data formatting
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
		//清除所有格式并恢复出厂默认设置
		{
			X_paramBakGetGlobalAllParam()->pDFormat->DFEnable = FALSE;//disable
			X_paramBakGetGlobalAllParam()->pDFormat->ErrorTone = 0;//开启提示音
			X_paramBakGetGlobalAllParam()->pDFormat->outFlag = 0;//数据格式不匹配时的输出要求
			X_paramBakGetGlobalAllParam()->pDFormat->iSsingle = 0;//单次format关闭
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

//128码设置
BOOL SetCode1D_Code128(int Rec56)
{
	switch(Rec56){
	case Set_Default://128码的所有参数设置为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = Init_CODE128;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODE128);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UCCEAN128));
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_CODE128));
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UCCEAN128));
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODE128);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CODE128;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
	    g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_CODE128;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
// 	case Code128_UCCEAN_Off://不识别UCC-EAN128条码
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] &= 0xffffff0c;
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UCCEAN128));
// 		break;
// 	case Code128_UCCEAN_FNC1_1://识别UCC-EAN，且FNC1在Start起始的第一个位置
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xfffffffc) | 0x01;
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_UCCEAN_FNC1_2://识别UCC-EAN，且FNC1在Start起始的第二个位置
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xfffffffc) | 0x03;
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_FNC2_Off://不识别带有FNC2的条码
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] &= 0xfffffff3;
// 		break;
// 	case Code128_FNC2_1://识别在起始Start字符第一个字符后出现FNC2的条码
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xfffffff3) | (0x01 << 2);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_FNC2_2://识别在起始Start字符第二个字符后出现FNC2的条码
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xfffffff3) | (0x03 << 2);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_UCCEAN_FNC3_1://识别UCC-EAN，且FNC3在Start起始的第一个位置
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xffffffcf) | (0x01 << 4);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_UCCEAN_FNC3_2://识别UCC-EAN，且FNC3在Start起始的第二个位置
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xffffffcf) | (0x03 << 4);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_UCCEAN_FNC4_1://识别UCC-EAN，且FNC4在Start起始的第一个位置
// 		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_CODE128] & 0xffffff3f) | (0x01 << 6);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
// 		break;
// 	case Code128_UCCEAN_FNC4_2://识别UCC-EAN，且FNC4在Start起始的第二个位置
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

//EAN-8码设置
BOOL SetCode1D_EAN8(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] = Init_EAN8;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_EAN8);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_EAN8));
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_EAN8);
		break;
	case EANUPC_SendCheck_Off://不传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] &= (~0x01);
		break;
	case EANUPC_SendCheck_On://传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= 0x01;
		break;
	case EANUPC_2Expand_Off://不识读两位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] &= (~0x02);
		break;
	case EANUPC_2Expand_On://识读两位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= 0x02;
		break;
	case EANUPC_5Expand_Off://不识读五位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] &= (~0x04);
		break;
	case EANUPC_5Expand_On://识读五位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= 0x04;
		break;
	case EANUPC_EAN8To13_Off://不把结果扩展成EAN-13
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] &= (~0x10);
		break;
	case EANUPC_EAN8To13_On://把结果扩展成EAN-13
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN8] |= 0x10;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//EAN-13码设置
BOOL SetCode1D_EAN13(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] = Init_EAN13;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_EAN13);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_EAN13));
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_EAN13);
		break;
	case EANUPC_SendCheck_Off://不传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] &= (~0x01);
		break;
	case EANUPC_SendCheck_On://传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] |= 0x01;
		break;
	case EANUPC_2Expand_Off://不识读两位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] &= (~0x02);
		break;
	case EANUPC_2Expand_On://识读两位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] |= 0x02;
		break;
	case EANUPC_5Expand_Off://不识读五位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] &= (~0x04);
		break;
	case EANUPC_5Expand_On://识读五位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_EAN13] |= 0x04;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//UPC-E码设置
BOOL SetCode1D_UPCE(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] = Init_UPCE; 
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UPCE);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UPCE));
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UPCE);
		break;
	case EANUPC_SendCheck_Off://不传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x01);
		break;
	case EANUPC_SendCheck_On://传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x01;
		break;
	case EANUPC_2Expand_Off://不识读两位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x02);
		break;
	case EANUPC_2Expand_On://识读两位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x02;
		break;
	case EANUPC_5Expand_Off://不识读五位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x04);
		break;
	case EANUPC_5Expand_On://识读五位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x04;
		break;
/*	case EANUPC_UPC_Pre0_Off://不传送前导字符'0'
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x08);
		break;
	case EANUPC_UPC_Pre0_On://传送前导字符'0'
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x08;
		break;
	case EANUPC_UPCETo13_Off://不把结果扩展成UPC-A
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x10);
		break;
	case EANUPC_UPCETo13_On://把结果扩展成UPC-A
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x10;
		break;*/
	case UPCE_Preamble_Off://不传送Preamble
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x08);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~BIT_UPCE_USCode);
		break;
	case UPCE_Preamble_SystemChar_On://传送system character
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x08;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~BIT_UPCE_USCode);
		break;
	case UPCE_Preamble_ALL_On://UPCE传送country code '0' + system character'0'
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= BIT_UPCE_USCode;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x08;
		break;

	case UPCE_ToUPCA_Off://不扩展成UPC-A
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x10);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~BIT_UPCE_TDoUPCA);
		break;
	case UPCE_DataTo13_On://把data扩展成13位
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= 0x10;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~BIT_UPCE_TDoUPCA);
		break;
	case UPCE_ToUPCA_On://UPC-E转换成UPC-A
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] |= BIT_UPCE_TDoUPCA;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCE] &= (~0x10);
		break;
	default: return FALSE;
	}
	return TRUE;
}

//UPC-A码设置
BOOL SetCode1D_UPCA(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] = Init_UPCA;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UPCA);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_UPCA));
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UPCA);
		break;
	case EANUPC_SendCheck_Off://不传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x01);
		break;
	case EANUPC_SendCheck_On://传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= 0x01;
		break;
	case EANUPC_2Expand_Off://不识读两位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x02);
		break;
	case EANUPC_2Expand_On://识读两位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= 0x02;
		break;
	case EANUPC_5Expand_Off://不识读五位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x04);
		break;
	case EANUPC_5Expand_On://识读五位扩展码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= 0x04;
		break;
/*	case EANUPC_UPC_Pre0_Off://不传送前导字符'0'
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x08);
		break;
	case EANUPC_UPC_Pre0_On://传送前导字符'0'
		X_paramGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= 0x08;
		break;*/
	case UPCA_Preamble_Off://不传送Preamble
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x08);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= BIT_UPCA_CloseSysChar;
		break;
	case UPCA_Preamble_ALL_On://传送country code + system character
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= 0x08;
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~BIT_UPCA_CloseSysChar);
		break;

	case UPCA_Preamble_SystemChar_On://传送 system character
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~0x08);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~BIT_UPCA_CloseSysChar);
		break;

	case UPCA_Coupon_Off://禁止UPC-A/EAN-13+Coupon扩展
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~(BIT_UPCA_Coupon|BIT_UPCA_REQCoupon));
		break;

	case UPCA_Coupon_Allow://允许UPC-A/EAN-13+Coupon扩展
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~BIT_UPCA_REQCoupon);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= BIT_UPCA_Coupon;
		break;

	case UPCA_Coupon_Require://必须UPC-A/EAN-13+Coupon扩展
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~BIT_UPCA_Coupon);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= BIT_UPCA_REQCoupon;
		break;

	case UPCA_Coupon_RSS_Output_Off:	//输出所有coupon信息
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] &= (~BIT_GS1COUPON_ONLYGS1);
		break;

	case UPCA_Coupon_RSS_Output_On:	//只输出RSS的信息
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UPCA] |= BIT_GS1COUPON_ONLYGS1;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//INTER25码设置
BOOL SetCode1D_Inter2of5(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		//交叉25码默认最大读码长度80，最小读码长度4
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = Init_INTER25;		
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_INTER2OF5);  //INTER2OF5码使能
		memset(X_paramBakGetGlobalAllParam()->p1D->I2Of5Len, 0, 5);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ITF14);     //ITF14禁止
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ITF6);      //ITF6禁止
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= (~(0x01 << POS_INTER2OF5));
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_INTER2OF5);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_INTER2OF5;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_INTER2OF5;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Inter2Of5_Check_Off://不处理校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= (~0x03);
		break;
	case Inter2Of5_Check_Send_Off://处理校验符，但不传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] = (X_paramGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] & (~0x03)) | 0x01;
		break;
	case Inter2Of5_Check_Send_On://处理校验符，且传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= 0x03;
		break;
	case Inter2Of5_ITF14_Off://不处理ITF14条码
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ITF14);
		break;
	case Inter2Of5_ITF14_Check_Off://处理ITF14条码，但不传送校验符
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF14);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= (~0x04);
		break;
	case Inter2Of5_ITF14_Check_On://处理ITF14条码且传送校验符
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF14);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= 0x04;
		break;
	case Inter2Of5_ITF6_Off://不处理ITF6条码
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ITF6);
		break;
	case Inter2Of5_ITF6_Check_Off://处理ITF6条码，但不传送校验符
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF6);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= (~0x08);
		break;
	case Inter2Of5_ITF6_Check_On://处理ITF6条码且传送校验符
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ITF6);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= 0x08;
		break;
	case Inter2Of5_FixedLen_Off: //设置交叉25码只识读定长关闭
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] &= (~0x10);
		X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[0] = 0;
		break;
	case Inter2Of5_FixedLen_On:  //设置交叉25码只识读定长打开
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INTER2OF5] |= 0x10;
		X_paramBakGetGlobalAllParam()->p1D->I2Of5Len[0] = 4;
		break;
	case Inter2Of5_FixedLen_Ena: //设置交叉25码只识读定长的长度
		g_DataType = DataSetType_FixedLen;
		g_PosMaxMin = POS_INTER2OF5;
		g_DataSet = 0;
		g_nPreSuffix = 0;
		g_nFixedLen = 0;
		g_IsSetCodeStart = TRUE;
		memset(g_PreSuffix, 0, 20);
		break;
	case Inter2Of5_FixedLen_Dis: //取消交叉25码只识读定长的长度
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

//MATRIX25码设置(China Post 25)
BOOL SetCode1D_Matrix25(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] =Init_MATRIX25;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MATRIX25);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MATRIX25);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_MATRIX25);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_MATRIX25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_MATRIX25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Matrix25_Check_Off://不处理校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] &= (~0x03);
		break;
	case Matrix25_Check_Send_Off://处理校验但不传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] |= 0x02;
		break;
	case Matrix25_Check_Send_On://处理校验同时回传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MATRIX25] |= 0x03;
		break;
	default: return FALSE;
	}
    return TRUE;
}

//MODULE25码设置
BOOL SetCode1D_Module25(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] = Init_MODULE25;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MODULE25);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MODULE25);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_MODULE25);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_MODULE25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_MODULE25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Module25_Check_Off://不处理校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] &= (~0x03);
		break;
	case Module25_Check_Send_Off://处理校验但不传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] |= 0x01;
		break;
	case Module25_Check_Send_On://处理校验同时回传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MODULE25] |= 0x03;
		break;
	default: return FALSE;
	}
    return TRUE;
}

//CODE39码设置
BOOL SetCode1D_Regular39(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] = Init_REGULAR39;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_REGULAR39);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_REGULAR39);
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_FULLASCII39);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_REGULAR39);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_REGULAR39;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_REGULAR39;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Code39_Check_Off://不处理校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= (~0x03);
		break;
	case Code39_Check_Send_Off://处理校验但不传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= 0x01;
		break;
	case Code39_Check_Send_On://处理校验同时回传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= 0x03;
		break;
	case Code39_StartStop_Off://不传送起始符与终止符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= (~0x04);
		break;
	case Code39_StartStop_On://传送起始符与终止符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= 0x04;
		break;
	case Code39_FullASCII_Off://不识别全ASCII39码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= (~0x08);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_FULLASCII39);
		break;
	case Code39_FullASCII_On://识别全ASCII39码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= 0x08;
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_REGULAR39);
// 		X_paramGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_FULLASCII39);
		break;

	case Code39_Code32_Off://不转换为Code32
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= ~BIT_Code32SpecEdit;
		break;
	case Code39_Code32_On://转换为Code32
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= BIT_Code32SpecEdit;
		break;
	case Code39_Code32_Star_Off://Code32不添加起始符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] &= ~BIT_Code32PreCh;
		break;
	case Code39_Code32_Star_On://Code32不添加起始符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_REGULAR39] |= BIT_Code32PreCh;
		break;

	default: return FALSE;
	}
	return TRUE;
}

//Codabar码设置
BOOL SetCode1D_Codabar(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] = Init_CODABAR;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODABAR);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODABAR);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODABAR);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CODABAR;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_CODABAR;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Codabar_Check_Off://不处理校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= (~0x03);
		break;
	case Codabar_Check_Send_Off://处理校验但不传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= 0x01;
		break;
	case Codabar_Check_Send_On://处理校验同时回传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= 0x03;
		break;
	case Codabar_StartStop_Off://不传送起始符与终止符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= (~0x04);
		break;
	case Codabar_StartStop_On://传送起始符与终止符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= 0x04;
		break;
	case Codabar_ABCDTNXE_Off://起始符与终止符为普通ABCD格式
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= ~BIT_CODA_ABCDTNXE;
		break;
	case Codabar_ABCDTNXE_On://起始符与终止符为ABCD/TN*E格式
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= BIT_CODA_ABCDTNXE;
		break;
	case Codabar_LOWERCASE_Off://起始符与终止符为大写
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] &= ~BIT_CODA_LOWERCASE;
		break;
	case Codabar_LOWERCASE_On://起始符与终止符为小写
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODABAR] |= BIT_CODA_LOWERCASE;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//Code93码设置
BOOL SetCode1D_Code93(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] = Init_CODE93;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODE93);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODE93);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODE93);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CODE93;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_CODE93;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Code93_Check_Off://不处理校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] &= (~0x03);
		break;
	case Code93_Check_Send_Off://处理校验但不传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] |= 0x01;
		break;
	case Code93_Check_Send_On://处理校验同时回传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE93] |= 0x03;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//CodeZ码设置
BOOL SetCode1D_CodeZ(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODEZ] = Init_CODEZ;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODEZ);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODEZ);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODEZ);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CODEZ;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
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

//RSS码设置
BOOL SetCode1D_RSS(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_RSS] = Init_RSS;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (DC_RSS);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(DC_RSS);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (DC_RSS);
		break;
	case RSS_AI_Send_Off://AI字符不发送
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_RSS] &= ~(0x1);
		break;
	case RSS_AI_Send_On://AI字符发送
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_RSS] |= (0x1);
		break;
	default: return FALSE;
	}
	return TRUE;
}

//COMPOSITE码(UCC EAN复合码)设置
BOOL SetCode1D_COMPOSITE(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_COMPOSITE] = Init_COMPOSITE;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_COMPOSITE);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_COMPOSITE);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_COMPOSITE);
		break;
	case COMPOSITE_UCCEAN_Off://UPC/EAN版本禁止
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_COMPOSITE] &= (~0x01);
		break;
	case COMPOSITE_UCCEAN_On://UPC/EAN版本使能
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_COMPOSITE] |= 0x01;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//Code11码设置
BOOL SetCode1D_CODE11(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] = Init_CODE11;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODE11);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CODE11);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CODE11);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CODE11;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_CODE11;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case CODE11_No_Check://不校验
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		break;
	case CODE11_C11Chk1://一位校验，MOD11
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x01;
		break;
	case CODE11_C11M11M11Chk://两位校验MOD11/MOD11
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x02;
		break;
	case CODE11_C11M11M9Chk://两位校验MOD11/MOD9
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x03;
		break;
	case CODE11_10char_C11M11M11Chk://MOD11单校验(Len<=10),MOD11/MOD11双校验(Len>10)
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x04;
		break;
	case CODE11_10char_C11M11M9Chk://MOD11单校验(Len<=10),MOD11/MOD9双校验(Len>1)
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x07);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x05;
		break;
	case CODE11_No_Send_Chk://不传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] &= (~0x08);
		break;
	case CODE11_Send_Chk://传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CODE11] |= 0x08;
		break;
	default: return FALSE;
	}
	return TRUE;
}


//ISBN码设置
BOOL SetCode1D_ISBN(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ISBN] = Init_ISBN;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ISBN);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ISBN);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ISBN);
		break;
	case ISBN_Send_13://发送13字节
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ISBN] &= (~0x01);
		break;
	case ISBN_Send_10://发送10字节
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ISBN] |= 0x01;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//INDUSTRIAL25码设置
BOOL SetCode1D_INDUSTRIAL25(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] = Init_Industrial25;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_INDUSTRIAL25);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_INDUSTRIAL25);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_INDUSTRIAL25);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_INDUSTRIAL25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_INDUSTRIAL25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case INDUSTRIAL25_No_CHK://不校验
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] &= (~0x03);
		break;
	case INDUSTRIAL25_CHK_NO_Send://校验,不传送
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] |= 0x01;
		break;
	case INDUSTRIAL25_CHK_Send://校验并传送
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_INDUSTRIAL25] |= 0x03;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//STANDARD25码设置
BOOL SetCode1D_STANDARD25(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] = Init_Standard25;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_STANDARD25);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_STANDARD25);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_STANDARD25);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_STANDARD25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_STANDARD25;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case STANDARD25_No_CHK://不校验
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] &= (~0x03);
		break;
	case STANDARD25_CHK_NO_Send://校验,不传送
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] |= 0x01;
		break;
	case STANDARD25_CHK_Send://校验并传送
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_STANDARD25] |= 0x03;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//PLESSEY码设置
BOOL SetCode1D_PLESSEY(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] = Init_Plessey;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_PLESSEY);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_PLESSEY);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_PLESSEY);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_PLESSEY;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_PLESSEY;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case PLESSEY_No_CHK://不校验
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] &= (~0x03);
		break;
	case PLESSEY_CHK_NO_Send://校验,不传送
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] |= 0x01;
		break;
	case PLESSEY_CHK_Send://校验并传送
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_PLESSEY] |= 0x03;
		break;
	default: return FALSE;
	}
	return TRUE;
}

//MSI_PLESSEY码设置
BOOL SetCode1D_MSIPLESSEY(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] = Init_MSIPlessey;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MSI_PLESSEY);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_MSI_PLESSEY);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_MSI_PLESSEY);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_MSI_PLESSEY;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_MSI_PLESSEY;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case MSI_No_CHK://不校验
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] &= (~0x03);
		break;
	case MSI_CHK_M10://单MOD10校验
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] |= 0x01;
		break;
	case MSI_CHK_M10M10://双MOD10校验
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] |= 0x02;
		break;
	case MSI_CHK_M10M11://MOD10/MOD11双校验
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] |= 0x03;
		break;
	case MSI_Check_Send_Off://不传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] &= (~0x04);
		break;
	case MSI_Check_Send_On://传送校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_MSI_PLESSEY] |= 0x04;
		break;
	default: return FALSE;
	}
	return TRUE;
}

BOOL SetCode1D_ISSN(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_ISSN] = Init_ISSN;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ISSN);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_ISSN);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_ISSN);
		break;
	default: return FALSE;
	}
	return TRUE;
}



//China Post 25码设置
BOOL SetCode1D_ChnPost25(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] =Init_CHNPOST25;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CHNPOST);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_CHNPOST);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_CHNPOST);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_CHNPOST;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_1DMax;
		g_PosMaxMin = POS_CHNPOST;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case ChnPost25_Check_Off://不处理校验符
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] &= (~0x03);
		break;
	case ChnPost25_Check_Send_Off://处理校验但不传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] &= (~0x03);
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] |= 0x01;
		break;
	case ChnPost25_Check_Send_On://处理校验同时回传送校验码
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_CHNPOST] |= 0x03;
		break;
	default: return FALSE;
	}

    return TRUE;
}


//二维码宏设置
BOOL SetCode2D_Macro(int Rec56)
{
	switch(Rec56){
	case Macro_Cancel: //取消宏数据信息
		W_macroInitialParam();
		break;
	case Macro_DirectSend://每个数据块的信息读取后直接传送
		X_paramBakGetGlobalAllParam()->p2D->nMacro = Macro_DirectSend;
		break;
	case Macro_SendInTurn://按顺序发送（缓存不超过64kbyte），
		     //即当比当前数据块的连接号小的数据块都已识读时，
		     //把这些目前已经连接好的数据（包括当前块）传送
		X_paramBakGetGlobalAllParam()->p2D->nMacro = Macro_SendInTurn;
		break;
	case Macro_LinkSendAfterRead://读完全部的数据块后再连接发送。
		         //当缓存数据量超过64k时，以模式2的方式传送数据
		X_paramBakGetGlobalAllParam()->p2D->nMacro = Macro_LinkSendAfterRead;
		break;
	
	default: return FALSE;
	}
	return TRUE;
}

//PDF417码设置
BOOL SetCode2D_PDF417(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_PDF417);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_PDF417] = Init_2D_PDF417_MIN_MAX_LEN;  //PDF默认最大2710，最小1
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_PDF417] = Init_PDF417;
            X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_PDF417] = 0;
// #if defined(DC_PDF417_DOUBLE)
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_PDF417] &= (~DC_PDF417_DOUBLE);
// #endif
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_PDF417);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_PDF417);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_PDF417; //PDF417码的位置
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_PDF417; //PDF417码的位置
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
	case PDF417_Reverse: //正反向设置
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

//QR码设置
BOOL SetCode2D_QR(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_QR);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_QR] = Init_2D_QRCODE_MIN_MAX_LEN;  //QR默认最大7089，最小1
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_QR] = Init_QR;
// #if defined(DC_QR_DOUBLE)
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_QR] &= (~DC_QR_DOUBLE);
// #endif
             X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_QR] = 0;
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_QR);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_QR);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_QR; //QR码的位置
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_QR; //QR码的位置
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

//DataMatrix码设置
BOOL SetCode2D_DataMatrix(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_DataMatrix);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_DataMatrix] = Init_2D_DataMatrix_MIN_MAX_LEN;  //DataMatrix默认最大3116，最小1
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
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= (~(0x01 << POS_DataMatrix));
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_DataMatrix);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_DataMatrix; //Data Matrix码的位置
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_DataMatrix; //Data Matrix码的位置
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
#if defined(DC_DM_129_ENABLE)
	case DataMatrix_129_Off: // DM 129 FOR ECS 项目关闭
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_129_ENABLE);
		break;
	case DataMatrix_129_On: // DM 129 FOR ECS 项目开启
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

#if defined(DC_DM_DISABLE_RECT)	//默认开启，置位则关闭
	case DataMatrix_Rectangular_Off: //不识别矩形码
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= DC_DM_DISABLE_RECT;
		break;
	case DataMatrix_Rectangular_On:  //识别矩形码
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_DISABLE_RECT);
		break;
#endif

#if defined(DC_DM_ADD_PAD_CODE)
	case DataMatrix_AddPADCode_Off: //信息后不添加PAD后面的码词
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] &= (~DC_DM_ADD_PAD_CODE);
		break;
	case DataMatrix_AddPADCode_On:  //信息后添加PAD后面的码词
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_DataMatrix] |= DC_DM_ADD_PAD_CODE;
		break;
#endif

#if defined(DC_DM_Reverse)
	case DataMatrix_Reverse: //正反向设置
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

//Aztec码设置
BOOL SetCode2D_Aztec(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_Aztec);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_Aztec] = Init_2D_AZTEC_MIN_MAX_LEN;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] = Init_Aztec;
             X_paramBakGetGlobalAllParam()->pCode2->nCharEncoding[CharacterEncoding_Aztec] = 0;
// #if  defined(DC_AZTEC_MULTINUM)
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTINUM);
// #endif
	    break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_Aztec);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_Aztec);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_Aztec; //Aztec码的位置
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_Aztec; //Aztec码的位置
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
		
#if  defined(DC_AZTEC_MULTINUM)
	case Aztec_Multi_Num: //设置多码同图的条码数量
		if(Rec7 > 7) return FALSE;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTINUM);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= (Rec7);
		break;
	case Aztec_Multi_None: //设置多码同图禁止
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
		break;
	case Aztec_Multi_Only: //设置只解多码
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] &= (~DC_AZTEC_MULTIEN);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_Aztec] |= (1<<POS_AZTEC_MULTIEN0);
		break;
	case Aztec_Multi_Mixed: //设置解多码（优先处理）或单码
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

//MaxiCode码设置
BOOL SetCode2D_MaxiCode(int Rec56)
{
	switch(Rec56){
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_MaxiCode);
	    X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_MaxiCode] = Init_2D_MaxiCode_MIN_MAX_LEN;
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_MaxiCode);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_MaxiCode);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_MaxiCode; //MaxiCode码的位置
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_MaxiCode; //MaxiCode码的位置
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	default :return FALSE;
	}
	return TRUE;
}

//VeriCode码设置
BOOL SetCode2D_VeriCode(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_VeriCode);
	    X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_VeriCode] = Init_2D_VeriCode_MIN_MAX_LEN;  //VeriCode默认最大1500，最小1
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] = Init_VeriCode;
// 		X_paramGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] &= (~0x0f);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_VeriCode);
		break;
	case Set_On://使能
	    X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_VeriCode);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_VeriCode; //VeriCode码的位置
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_VeriCode; //VeriCode码的位置
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case VeriCode_UnZip_ModeUp: //解压缩模式上
		if(Rec7 > 7) return FALSE;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] &= (~0x0f);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] |= Rec7;
		break;
	case VeriCode_UnZip_ModeDown://解压缩模式下
		if(Rec7 > 7) return FALSE;
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] &= (~0x0f);
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_VeriCode] |= Rec7 + 8;
		break;
	default : return FALSE;
	}
	return TRUE;
}

//汉信码设置
BOOL SetCode2D_Han(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_HAN_SENSIBLE);
		X_paramBakGetGlobalAllParam()->p2D->ExMsg[POS_HAN_SENSIBLE] = Init_2D_HANSENSIBLE_MIN_MAX_LEN;  //汉信码默认最大7827，最小1
		X_paramBakGetGlobalAllParam()->p2D->ExParam[POS_HAN_SENSIBLE] = Init_CSC;
	    break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_HAN_SENSIBLE);
		break;
	case Set_On://使能
	    X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_HAN_SENSIBLE);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_2DMin;
		g_PosMaxMin = POS_HAN_SENSIBLE; //汉信码的位置
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
		g_DataType = DataSetType_2DMax;
		g_PosMaxMin = POS_HAN_SENSIBLE; //汉信码的位置
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
	case HAN_SENSIBLE_Reverse: //正反向设置
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

//用户自定义二维码设置
BOOL SetCode2D_UserDef(int Rec56, int Rec7)
{
	switch(Rec56){
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_USER_2D_CODE);
	    break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p2D->unDecTypes &= ~(0x01 << POS_USER_2D_CODE);
		break;
	case Set_On://使能
	    X_paramBakGetGlobalAllParam()->p2D->unDecTypes |= (0x01 << POS_USER_2D_CODE);
		break;
	default : return FALSE;
	}

	return TRUE;
	Rec7 = Rec7;
}


//SPEC_OCR_B的设置
BOOL SetOCR_SPEC_OCR_B(int Rec56)
{
	switch(Rec56){
	case Set_Default://设置该类的所有属性为默认值
#if defined(SAGEM)
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes |= (0x01 << POS_SPEC_OCR_B);
#else   
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes &= ~(0x01 << POS_SPEC_OCR_B);
#endif		
		
		X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_SPEC_OCR_B] = Init_SPEC_OCR_B ; //默认只读长度为9的 
	    break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes &= ~(0x01 << POS_SPEC_OCR_B);
		break;
	case Set_On://使能
	    X_paramBakGetGlobalAllParam()->pOCR->unDecTypes |= (0x01 << POS_SPEC_OCR_B);
		break;

	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_OCRMin;
		g_PosMaxMin = POS_SPEC_OCR_B;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
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
	case Set_Default://设置该类的所有属性为默认值
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes &= ~(0x01 << POS_PASSPORT_VISAS);
		X_paramBakGetGlobalAllParam()->pOCR->ExMsg[POS_PASSPORT_VISAS] = Init_PASSPORT_VISAS ;  
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes &= ~(0x01 << POS_PASSPORT_VISAS);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->pOCR->unDecTypes |= (0x01 << POS_PASSPORT_VISAS);
		break;

	default : return FALSE;
	}
	return TRUE;
}


BOOL SetCode1D_UCCEAN128(int Rec56)
{
	switch(Rec56) {
	case Set_Default://设置该码的所有属性为默认值
		X_paramBakGetGlobalAllParam()->p1D->ExMsg[POS_UCCEAN128] = Init_UCCEAN128;
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
		break;
	case Set_Off://禁止
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes &= ~(0x01 << POS_UCCEAN128);
		break;
	case Set_On://使能
		X_paramBakGetGlobalAllParam()->p1D->unDecTypes |= (0x01 << POS_UCCEAN128);
		break;
	case Set_MinLen://设置读码最小长度
		g_DataType = DataSetType_1DMin;
		g_PosMaxMin = POS_UCCEAN128;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case Set_MaxLen://设置读码最大长度
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

//GM码设置
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

//MicroPDF417码设置
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

//一维条码设置
BOOL Set_Code1D(int Rec34, int Rec56)
{
	switch(Rec34){
	case Set_Code1D_Code128://CODE128码设置
		if(!SetCode1D_Code128(Rec56)) return FALSE;
		break;
	case Set_Code1D_EAN8://EAN-8码设置
		if(!SetCode1D_EAN8(Rec56)) return FALSE;
		break;
	case Set_Code1D_EAN13://EAN-13码设置
		if(!SetCode1D_EAN13(Rec56)) return FALSE;
  	    break;
	case Set_Code1D_UPCE://UPC-E码设置
		if(!SetCode1D_UPCE(Rec56)) return FALSE;
		break;
	case Set_Code1D_UPCA://UPC-A设置
		if(!SetCode1D_UPCA(Rec56)) return FALSE;
		break;
	case Set_Code1D_Inter2Of5://INTER2OF5码设置,包含了ITF14和ITF6
		if(!SetCode1D_Inter2of5(Rec56)) return FALSE;
		break;
	case Set_Code1D_Matrix25://MATRIX25码设置
		if(!SetCode1D_Matrix25(Rec56)) return FALSE;
		break;
	case Set_Code1D_Module25://MODULE25码设置
	    if(!SetCode1D_Module25(Rec56)) return FALSE;
		break;
	case Set_Code1D_Regular39://REGULAR39码设置
		if(!SetCode1D_Regular39(Rec56)) return FALSE;
		break;
	case Set_Code1D_Codabar://CODABAR码设置
		if(!SetCode1D_Codabar(Rec56)) return FALSE;
		break;
	case Set_Code1D_Code93: //Code93码设置
		if(!SetCode1D_Code93(Rec56)) return FALSE;
		break;
	case Set_Code1D_CodeZ: //CodeZ码设置
		if(!SetCode1D_CodeZ(Rec56)) return FALSE;
		break;
	case Set_Code1D_UCCEAN128:	//UCCEAN128码设置
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

//二维条码设置
BOOL Set_Code2D(int Rec34, int Rec56, int Rec7)
{
	switch(Rec34){
	case POS_Macro://宏设置
		if(!SetCode2D_Macro(Rec56)) return FALSE;
		break;

	//由于前面多了宏设置，所以各种码的位置偏移了一位
	case POS_PDF417 + 1://PDF417码设置
		if(!SetCode2D_PDF417(Rec56, Rec7)) return FALSE;
		break;
	case POS_QR + 1://QR码设置
		if(!SetCode2D_QR(Rec56, Rec7)) return FALSE;
		break;
	case POS_Aztec + 1://Aztec码设置
		if(!SetCode2D_Aztec(Rec56, Rec7)) return FALSE;
		break;
	case POS_DataMatrix + 1://Data Matrix码设置
		if(!SetCode2D_DataMatrix(Rec56, Rec7)) return FALSE;
		break;
	case POS_MaxiCode + 1://MaxiCode码设置
		if(!SetCode2D_MaxiCode(Rec56)) return FALSE;
		break;
	case POS_VeriCode + 1 ://VeriCode码设置
		if(!SetCode2D_VeriCode(Rec56, Rec7)) return FALSE;
		break;
	case POS_HAN_SENSIBLE + 1: //汉信码设置
		if(!SetCode2D_Han(Rec56, Rec7)) return FALSE;
		break;
	case POS_GM_CODE + 1:
		if(!SetCode2D_GM(Rec56)) return FALSE;
		break;
	case POS_USER_2D_CODE + 1: //用户自定义二维码设置
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

	case 3://不传送校验符
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_POSTNET] &= (~BIT_POSTNET_TRMTCHK);
		break;
	case 4://传送校验符
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

	case 3://不传送校验符
		X_paramBakGetGlobalAllParam()->pPostal->ExMsg[POS_PLANET] &= (~BIT_PLANET_TRMTCHK);
		break;
	case 4://传送校验符
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

//OCR设置
BOOL Set_OCR(int Rec34, int Rec56)
{
	switch(Rec34){
	case POS_SPEC_OCR_B ://SPEC_OCR_B设置
		if(!SetOCR_SPEC_OCR_B(Rec56)) return FALSE;
		break;

	case POS_PASSPORT_VISAS:
		if (!SetOCR_PASSPORT_VISAS(Rec56)) return FALSE;
		break;

	default: return FALSE;
	}
	return TRUE;
}

//用户类型设置
BOOL Set_UserType(int Rec34, int Rec56)
{
	switch(Rec34){
	case POS_UserType_Group1 ://第一组
// 		if(Rec56 > UserType_ECS_SAMSUNG) return FALSE;
// 		else X_paramGetGlobalAllParam()->pCode->nUserDef = Rec56;
// 		
// 		//下面为设置码无法设置的一些参数的设置
// 		if(X_paramGetGlobalAllParam()->pCode->nUserDef == UserType_NL_COMP_CQ)
// 		{
// //			g_bEnPort = FALSE;
// 		}
// 		else if(X_paramGetGlobalAllParam()->pCode->nUserDef == UserType_USA_DOUBLE_1D){
// 			//I2of5只识别双码，其它的只识别单码
// 			X_paramGetGlobalAllParam()->p1D->unDecParam &= (~0x03);
// 	        X_paramGetGlobalAllParam()->p1D->unDecParam |= 0x04; //功能在该位体现
// 		}
		
		break;

	default:
		return FALSE;
	}
	return TRUE;
	Rec56 = Rec56;
}

//二维条码非标参数设置
BOOL Set_2DNonStandar(int Rec34, int Rec56)
{

	switch(Rec34){
	case POS_PDF417:
		break;
	case POS_QR ://QR非标参数设置
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

//QR码非标参数设置
BOOL Set_QRNonStandar(int Rec56)
{
	switch(Rec56){
	case 0://值设置
		g_DataType = DataSetType_NonStandar;
		g_PosMaxMin = POS_QR;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case 1: //默认
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


//新通讯参数设置
BOOL Set_NewComm(int Rec34, int Rec56, int Rec7)
{
	switch(Rec34) {
	case Comm_Interface: //接口类型设置
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

//KBW口参数设置
BOOL Set_NewCommKBW(int Rec56, int Rec7)
{
	switch(Rec56) {
	case KBW_Nationality: //语言类型设置
		g_DataType = DataSetType_KBW;
		g_PosMaxMin = DataSetKBW_Nationality;
		g_DataSet = 0;
		g_IsSetCodeStart = TRUE;
		break;
	case KBW_CapsLock_Off: //关闭大写键
		X_paramBakGetGlobalAllParam()->pKBW->isCapsLock = FALSE;
		break;
	case KBW_CapsLock_On: //打开大写键
		X_paramBakGetGlobalAllParam()->pKBW->isCapsLock =TRUE;
		break;
	case KBW_PromptUnknowChar: //无效字符提示设置
		if(Rec7 < 2) X_paramBakGetGlobalAllParam()->pKBW->isUnknownCharactersBeep = Rec7;
		else return FALSE;
		break;
	case KBW_CovertCase: //大小写字符转换设置
		if(Rec7 < 3) X_paramBakGetGlobalAllParam()->pKBW->isConvertCase = Rec7;
		break;
	case KBW_KeystrokeDelay: //键盘延迟设置
		if(Rec7 < 3){
			X_paramBakGetGlobalAllParam()->pKBW->isKeysrockeDelay = Rec7;
		}
		else return FALSE;
		break;
	case KBW_KeypadEmulation: //Alt+ASCII模式设置
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
			X_paramBakGetGlobalAllParam()->pKBW->isKeyPad = Rec7;//Rec7=0关闭小键盘，Rec7=1打开小键盘
		else
			return FALSE;
		break;
	case KBW_FastMode: 
		if((Rec7==0)||(Rec7==1))
			X_paramBakGetGlobalAllParam()->pKBW->isFasttransferMode = Rec7;   //Rec7=0关闭快速模式，Rec7=1开启快速模式
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

//蓝牙参数设置
BOOL Set_NewCommBlueT(int Rec56, int Rec7)
{
	switch(Rec56) {
	case BlueT_Reset: //蓝牙复位设置
		BlueT_reset_cfg();//复位所有的配置
		break;
	case BlueT_Name: //蓝牙设备名称设置
		g_DataType = DataSetType_BlueT;
		g_PosMaxMin = DataSetBlueT_Name;
		memset(g_PreSuffix, 0, 20);
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		g_IsSetCodeStart = TRUE;
		break;
	case BlueT_PassWord: //密码
		if(Rec7 == 0 || Rec7 == 1){ //密码校验使能设置
			X_paramGetBlueT()->isPSWCheck = Rec7;
			BlueT_set_auth_status(Rec7);//设置是否进行密码匹配
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


//PS2参数设置
BOOL Set_NewCommPS2(int Rec56, int Rec7)
{
	switch(Rec56) {
//	case PS2_Nationality: //语言类型设置
//		g_DataType = DataSetType_PS2;
//		g_PosMaxMin = DataSetPS2_Nationality;
//		g_DataSet = 0;
//		g_IsSetCodeStart = TRUE;
//		break;
// 	case PS2_CapsLockDef: //大写键默认设置
// 		if(Rec7 == 0 || Rec7 == 1) 
// 			X_paramGetGlobalAllParam()->pKBW->isPS2CapsLockDef = Rec7;
// 		else return FALSE;
// 		break;
	default: return FALSE;
	}

	return TRUE;
}


//KBW设置
BOOL SetCode_Save_KBW(void)
{
	switch(g_PosMaxMin) {
	case DataSetKBW_Nationality: //键盘国家语言设置
		X_paramBakGetGlobalAllParam()->pKBW->nLanguage = g_DataSet;
		break;
	case DataSetKBW_CodePage: //code page
		X_paramBakGetGlobalAllParam()->pKBW->nCodePage = g_DataSet;
		break;
	default: return FALSE;
	}

	return TRUE;
}
	
//蓝牙参数设置
BOOL SetCode_Save_BlueT(void)
{
	switch(g_PosMaxMin) {
	case DataSetBlueT_Name: //设备名称设置
		if(!g_DataSet_Odd)	{ return FALSE; }
		if(g_nPreSuffix > 16) return FALSE;
		memset(X_paramGetBlueT()->Name, 0, sizeof(X_paramGetBlueT()->Name));
		memcpy(X_paramGetBlueT()->Name, g_PreSuffix, g_nPreSuffix);
		memset(g_PreSuffix, 0, 20);
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		if (!BlueT_set_dev_name((char*)X_paramGetBlueT()->Name,strlen((char*)X_paramGetBlueT()->Name)))
		{ return FALSE; }//设置设备名称 
		break;
	case DataSetBlueT_Password: //蓝牙密码设置
		if(!g_DataSet_Odd)	{ return FALSE; }
		if(g_nPreSuffix > 6) return FALSE;
		memset(X_paramGetBlueT()->PSW, 0, sizeof(X_paramGetBlueT()->PSW));
		memcpy(X_paramGetBlueT()->PSW, g_PreSuffix, 6);
		memset(g_PreSuffix, 0, 20);
		g_nPreSuffix = 0;
		g_DataSet_Odd = TRUE;
		if (!BlueT_set_slave_pin((char*)X_paramGetBlueT()->PSW,strlen((char*)X_paramGetBlueT()->PSW)))
		{ return FALSE; }//设置slave模式下的匹配密码
		break;
	default: return FALSE;
	}

	return TRUE;
}

//新解码模式设置
BOOL Set_NewDecMode(int Rec34, int Rec56, int Rec7)
{
	switch(Rec34) {
	case DecMode_NewOscillation: //振子设置
		if(Rec56 == 0 || Rec56 == 1) //使能设置
			X_paramBakGetGlobalAllParam()->pHard->bOscillation = Rec56;
		else if(Rec56 == 2){//时间设置
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
	int   nError; //错误
	int   nStatus; //状态
	BOOL  bSave2Flash;   // 存储
	BOOL  isOpenSet;
	BOOL  bNoSave;	//不保存标志
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
	//调用SetCode 宏取消函数：宏取消有效，其他都无效
	if (nMsgLen == 8 && strncmp((char*)pbyMsg,"05000000",nMsgLen) == 0){
		W_setcSetResult(U_uIMG_STATUS_SETCODE_SUC);
		W_setcEnable(FALSE);
		return TRUE;
	}//End of if;
	W_setcSetResult(U_uIMG_STATUS_SETCODE_UNKNOW);
	return FALSE;
}


//设置码处理
//返回：0 —— 错误(FALSE)
//      1 —— 设置正确返回(TRUE)
//      2 —— 不是设置码
int W_setcDealWith(void)
{
	int i;
	int nResult;
	BOOL IsSetCode;
	BYTE* pMsg={'\0'};
	int  nReFlag;
	int nMsgLen;

	nReFlag = SETCPROC_NOT; //普通条码
	
	pMsg = X_decoGetDecInfoBuff();
	nMsgLen = X_decoGetDecInfoLen();
	if (pMsg == NULL || nMsgLen <= 0)
	{ return nReFlag; }


	//判断是不是设置码

	IsSetCode = (X_decoGetBarCode() == UT_TYPE_1D && ((X_decoGetCodeType() == DC_ZASETUP) || (X_decoGetCodeType() == DC_SETUP128)));

	if (W_setcDealUserFunc != NULL)
	{//处理嵌入的设置码处理功能
		W_setcDealUserFunc(IsSetCode,X_decoGetBarCode(),X_decoGetCodeType(),X_decoGetDecInfoBuff(),X_decoGetDecInfoLen());
	}

	//如果设置码设置功能启动，但如果读的不是设置码，则关闭设置码设置功能
	if(W_setcIsEnable() && (!IsSetCode))
	{
		nReFlag = SETCPROC_NOT; //普通条码
		switch(g_ReplaceType) {
		case Replace_No: //不是设置码，且替代码没有打开，则关闭设置码
			W_setcEnable(FALSE);
			break;
		case Replace_UserPart: //批指令处理
			if(W_setcParseBatch(pMsg, nMsgLen))
			{ nReFlag = 3; }
			else
			{ nReFlag = SETCPROC_ERROR; } //处理错误
			break;
		case Replace_SN: //用条码设置SN
			if (nMsgLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pSN))
			{
				memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pSN,pMsg,nMsgLen);
				X_paramBakGetGlobalAllParam()->pDevInfo->pSN[nMsgLen] = '\0';
                          paramSaveType(1);
				X_paramPlaceKeyword((char*)"S/N",(char*)X_paramBakGetGlobalAllParam()->pDevInfo->pSN,TRUE);
				nReFlag = 4;
			}//End of if;
			break;
		case Replace_ESN: //用条码设置ESN
			if (nMsgLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pESN))
			{
				memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pESN,pMsg,nMsgLen);
				X_paramBakGetGlobalAllParam()->pDevInfo->pESN[nMsgLen] = '\0';
                         paramSaveType(1);
				X_paramPlaceKeyword((char*)"ESN",(char*)X_paramGetGlobalAllParam()->pDevInfo->pESN,TRUE);
				nReFlag = 4;
			}//End of if;
			break;
		case Replace_Date: //用条码设置出厂日期
			if (nMsgLen < sizeof(X_paramBakGetGlobalAllParam()->pDevInfo->pDate))
			{
				memcpy(X_paramBakGetGlobalAllParam()->pDevInfo->pDate,pMsg,nMsgLen);
				X_paramBakGetGlobalAllParam()->pDevInfo->pDate[nMsgLen] = '\0';
                         paramSaveType(1);
				X_paramPlaceKeyword((char*)"DATE",(char*)X_paramBakGetGlobalAllParam()->pDevInfo->pDate,TRUE);
				nReFlag = 4;
			}//End of if;
			break;
		case Replace_ManufID: //用条码设置ManufactureID
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
		case Replace_ManufName: //用条码设置ManufactureName
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
		W_setcParamInit(); //设置码公共参数初始化
	}//End of if;
	else if (IsSetCode && nMsgLen >= 7)
	{   //如果目前读取的是设置码
		//如果正在读取宏条码不予以进行设置
		//设置码值
            if(X_decoGetCodeType() == DC_ZASETUP)
            {	
                for(nResult = 0, i = 0; i < 7; i++)
                { 
                    nResult = nResult*10 + (pMsg[i] - '0'); 
                }
		
                if((!W_setcIsEnable()) && (nResult != 6000) && (nResult != 6010) && (nResult != 6030) && (nResult != 6031))
                { 
                    nReFlag = SETCPROC_ERROR; 
                }//设置码设置功能没打开
                else if(nResult == 6000 || nResult == 6010 || nResult == 6030 || nResult == 6031)
                {
                    if (W_setcParseSigle((BYTE*)pMsg,nMsgLen, FALSE))
						{ nReFlag = SETCRPOC_SUCC; }//不是中安设置码,所以不保存
					else
						{ nReFlag = SETCPROC_ERROR; }
                }
                else
                {
					if (W_setcParseSigle((BYTE*)pMsg,nMsgLen, TRUE))
						{nReFlag = SETCRPOC_SUCC; } //是中安设置码
					else
					{ nReFlag = SETCPROC_ERROR; }
                }
            }
            else//set128设置码
            {
                //先判断是不是开启 /是否允许读设置码???? 
                int r1,r2,r3,r4;
                //char buf[32];
                //int len;
                r1 = strncasecmp ((char *)(pMsg+1), "SETUPE1",7);     
                r2 = strncasecmp ((char *)(pMsg+1), "SETUPE0",7);//字符串相同，返回0
                r3 = strncasecmp ((char *)(pMsg+1), "FOBSET1",7);     
                r4 = strncasecmp ((char *)(pMsg+1), "FOBSET0",7);//字符串相同，返回0

                //Y_commSendString(pMsg);
                //Y_commSendString("\r\n");
                //len=sprintf(buf, "r1=%d,r2=%d\r\n", r1,r2);
                //Y_commWrite((unsigned char * )buf, len);
                
                if((!W_setcIsEnable()) && (r1!= 0) && (r2 != 0) && (r3!= 0) && (r4 != 0))
                { 
                    nReFlag = SETCPROC_ERROR; 
                }//设置码设置功能没打开
                else if((r1==0)||(r2==0)||(r1==0)||(r2==0))//读到的是以上4个码
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
                else//读到的是设置码
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
// 	{//其它类型的设置码
//  		if (Parse_SetCode_Blue2D((BYTE*)pMsg,iGetuImgInfoLen()))
// 		{ nReFlag = 1; }
// 	}
	else
	{ nReFlag = 2; } //普通条码

	switch(nReFlag){
	case 4: //设置信息
		X_paramSaveDeviceInfo();
		W_setcSetSaveFlag(FALSE);
		nReFlag = SETCRPOC_SUCC;
		break;
	case 3: //保存信息
		W_setcSetSaveFlag(TRUE);
		X_paramSaveDeviceInfo();
		if(X_paramSaveAsSysDefaultParam() == TRUE)
		{
			W_setcSetSaveFlag(FALSE); 
			W_setcSetResult(U_uIMG_STATUS_SETCODE_SUC);
			nReFlag = SETCRPOC_SUCC;
		}//End of if;
		break;
	case 2: // 不是设置码信息
		break;
	case 1: // 设置码正确
		W_setcSetResult(U_uIMG_STATUS_SETCODE_SUC);
		break;
	case 0: // 设置码错误
		W_setcSetResult(U_uIMG_STATUS_SETCODE_ERROR);
		break;
	}

	return nReFlag;

}


//设置扩展功能函数
void W_setcSetUserFunc(UTGeneralParam *psParam)
{
	if (psParam != NULL && psParam->Id == STID_SETCEXTFNC && psParam->psParamStruct != NULL)
	{
		W_setcDealUserFunc = ((TWSETCExtFunc*)psParam->psParamStruct)->fnc_DealUserSettingCode;
	}//End of if;
}

