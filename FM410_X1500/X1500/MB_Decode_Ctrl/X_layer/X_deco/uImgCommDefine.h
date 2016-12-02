#ifndef __UIMG_COMM_DEFINE_H__
#define __UIMG_COMM_DEFINE_H__


//////////////////////////////////////////////////////////////////////////
//	大类型
#define UT_TYPE_1D                  0xC1
#define UT_TYPE_2D                  0xC2
#define UT_TYPE_OCR                 0xC3
#define UT_TYPE_POSTAL				0xC4

//	大类型
//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////
/////邮政码

#define DC_POSTNET			0x00000001		/*USPS PostNet ZIP with check digit computation*/
#define DC_ONECODE			0x00000002		/*USPS OneCode(4-CB)alias USPS 4-State Customer Barcode*/
#define DC_RM4SCC			0x00000004		/*Royal Mail 4 State Barcode*/
#define DC_PLANET			0x00000008		/*Postal Alpha Numeric Encoding Technique*/
#define DC_KIXCODE			0x00000010		/*Dutch Postal Barcode*/
#define DC_AUSPOST			0x00000020		///Australian Postal Barcode


#define POS_POSTNET			0			/*USPS PostNet ZIP with check digit computation*/
#define POS_ONECODE			1			/*USPS OneCode(4-CB)alias USPS 4-State Customer Barcode*/
#define POS_RM4SCC			2			/*Royal Mail 4 State Barcode*/
#define POS_PLANET			3			/*Postal Alpha Numeric Encoding Technique*/
#define POS_KIXCODE			4			/*Dutch Postal Barcode*/
#define POS_AUSPOST			5			///Australian Postal Barcode


///Postal扩展参数
#define BIT_PLANET_ENCHK	 0x00000001		///使能校验
#define BIT_PLANET_TRMTCHK	 0x00000002		///传输校验

#define BIT_POSTNET_ENCHK	 0x00000001		///使能校验
#define BIT_POSTNET_TRMTCHK  0x00000002		///传输校验

#define  Init_PLANET		0x00000003	//使能并传送校验			
#define  Init_POSTNET 		0x00000003	//使能并传送校验	
	
#define  Init_ONECODE		0				
#define  Init_RM4SCC		0				
#define  Init_KIXCODE		0				
#define  Init_AUSPOST		0				

#define Init_PostalDecParam		0x00000000  

#define Init_EnablePostal	0


//////////////////////////////////////////////////////////////////////////
//	OCR	下的设置值
#define POS_SPEC_OCR_B				0				//OCR-B
#define POS_PASSPORT_VISAS			2

#define DC_SPEC_OCR_B				(1 << POS_SPEC_OCR_B) 
#define DC_PASSPORT_VISAS			(1 << POS_PASSPORT_VISAS)

#define Init_SPEC_OCR_B				0x09090000  	//SPEC_OCR_B初始化设置
#define Init_PASSPORT_VISAS			0x00000000  	//PASSPORT_VISAS初始化设置
#define Init_EnableOCR				0x00000000  	//二维条码使能
//	OCR	下的设置值
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//	二维码的设置值
//PDF417还从0开始是为了与使能位置的保持一致
#define POS_PDF417					0
#define POS_QR						1
#define POS_Aztec					2
#define POS_DataMatrix				3
#define POS_MaxiCode				4
#define POS_VeriCode				5
#define POS_UN_VeriCode             6
#define POS_HAN_SENSIBLE            7
#define POS_GM_CODE		            8

#define POS_USER_2D_CODE            9
#define POS_MicroPDF417            10

#define DC_PDF417					(1 << POS_PDF417)
#define DC_QRCODE  					(1 << POS_QR)
#define DC_AZTEC  					(1 << POS_Aztec)
#define DC_MAXICODE					(1 << POS_MaxiCode)
#define DC_DataMatrix 				(1 << POS_DataMatrix)
#define DC_VeriCode  				(1 << POS_VeriCode)  
#define DC_UN_VeriCode  			(1 << POS_UN_VeriCode)  
#define DC_USER_2D_CODE				(1 << POS_USER_2D_CODE)  
#define DC_HAN_SENSIBLE             (1 << POS_HAN_SENSIBLE)
#define DC_HAN_GM_CODE		        (1 << POS_GM_CODE)
#define DC_HAN_MicroPDF417          (1 << POS_MicroPDF417)

#ifdef IMG_FOR_VISION			//是否编给维深的库

//二维条码使能
#define Init_Enable2D		DC_HAN_SENSIBLE

#else

//二维条码使能
#define Init_Enable2D		(DC_PDF417|DC_QRCODE|DC_DataMatrix)
							
#endif		//#ifdef IMG_FOR_VISION			//是否编给维深的库

#define  Init_2D_PDF417_MIN_MAX_LEN			0x0A960001  // 2710,1
#define  Init_2D_QRCODE_MIN_MAX_LEN			0x1BB10001  // 7089,1
#define  Init_2D_AZTEC_MIN_MAX_LEN			0x0EF80001	// 3832,1
#define  Init_2D_DataMatrix_MIN_MAX_LEN		0x0C2C0001  // 3116,1
#define  Init_2D_MaxiCode_MIN_MAX_LEN		0x00960001  // 150,1
#define  Init_2D_VeriCode_MIN_MAX_LEN		0x05BC0001  // 1500,1
#define  Init_2D_UnVeriCode_MIN_MAX_LEN		0x00200001  // 32,1
#define  Init_2D_HANSENSIBLE_MIN_MAX_LEN	0x1E930001  // 7827,1
#define  Init_2D_GMCODE_MIN_MAX_LEN			0x0ABF0001	///*2751,1*/
#define  Init_2D_MicroPDF417_MIN_MAX_LEN	0x016e0001	///*366,1*/


#define  Init_PDF417				0	//只识别正向条码,仅读单个PDF417码,镜像识别禁止			
#define  Init_QR					0	//仅读单个QR码
#define  Init_Aztec					0	//不进行多码同图处理	
#define  Init_DataMatrix			0	//关闭ECS129,仅读单个DM码,不识别矩形码,解码结果后不添加信息码词,只识别正向条码,镜像识别禁止
#define  Init_VeriCode				0	//解压缩模式0
#define  Init_CSC					0	//仅读单个汉信码,只识别正向条码
#define  Init_GM					0	//
#define  Init_MicroPDF417			0	//


//细部参数打开和关闭
#define  POS_DM_129_ENABLE  0   // 129 关闭打开设定
#define  POS_DM_MOBILE      1   // 是否针对 Mobile 
#define  POS_DM_LOTTERY     2   // USA  Lottery

#define  DC_DM_129_ENABLE (1<<POS_DM_129_ENABLE)
#define  DC_DM_MOBILE     (1<<POS_DM_MOBILE)
#define  DC_DM_LOTTERY    (1<<POS_DM_LOTTERY)
//	二维码的设置值
//////////////////////////////////////////////////////////////////////////



/*********************************************************************************************
*	global referenced variables
*********************************************************************************************/
//////////////////////////////////////////////////////////////////////////
#define Init_1DDecParam				0x00000000  	//一维条码的整体参数初始化，如是否读双码等

#define Init_CODE128				0x30010100  	//校验
#define Init_UCCEAN128				0x30010000
#define Init_EAN8					0x00000001  	
#define Init_EAN13					0x00000001  	
#define Init_UPCE					0x00000009  	//传送系统字符，不传送国家编码
#define Init_UPCA					0x00000201  	
#define Init_INTER25				0x50060000  
	
#define Init_MATRIX25				0x50040000  	
#define Init_MODULE25				0x50040000  	
#define Init_REGULAR39				0x30010000//不传送起始终止符  	
#define Init_FULLASCII39			0x30010000  	
#define Init_CODABAR				0x3c020000 //0x3c020004 默认不传送校验位
 	
#define Init_CODE93					0x30010001	//校验不发送校验符  	
#define Init_CODEZ					0x0E070001 

#define Init_ISSN					0
#define Init_ISBN					1//发送10个数字
#define Init_Industrial25			0x30060000
#define Init_Standard25				0x30060000
#define Init_Plessey				0x30040000	/*不校验,不发送校验字符*/
#define Init_CODE11					0x30040009	/*单校验MOD11,发送校验字符*/
#define Init_MSIPlessey				0x30040005	/*单校验MOD10,发送校验字符*/
#define Init_COMPOSITE				0x00000000
#define Init_RSS					0x00000001
#define Init_CHNPOST25				0x30010000  	

//各类型条码对应的位位置
#define POS_ZASETUP		0
#define POS_SETUP128	1
#define	POS_CODE128		2
#define	POS_UCCEAN128	3
#define	POS_EAN8		4
#define	POS_EAN13		5
#define	POS_UPCE		6
#define	POS_UPCA		7
#define	POS_INTER2OF5	8
#define	POS_ITF14		9
#define	POS_ITF6		10
#define	POS_MATRIX25	11
#define	POS_MODULE25	12
#define	POS_REGULAR39	13
#define	POS_FULLASCII39	14
#define	POS_CODABAR		15
#define	POS_2OUTOF5		16
#define POS_CODE93		17
#define POS_CODEZ       18
#define POS_CHNPOST		19

#define POS_ISSN		23	//

#define POS_ISBN		24	//2009.11.30
#define POS_INDUSTRIAL25 25	//2009.11.30
#define POS_STANDARD25	26	//2009.11.30
#define POS_PLESSEY		27	//2009.11.30

#define POS_CODE11		28
#define POS_MSI_PLESSEY	29
#define POS_COMPOSITE	30
#define POS_RSS			31

//条码设置禁止使能类型参数
#define	DC_ZASETUP		0x00000001		//中安102设置码
#define	DC_SETUP128		0x00000002
#define	DC_CODE128		0x00000004
#define	DC_UCCEAN128	0x00000008
#define	DC_EAN8			0x00000010
#define	DC_EAN13		0x00000020
#define	DC_UPCE			0x00000040
#define	DC_UPCA			0x00000080
#define	DC_INTER2OF5	0x00000100
#define	DC_ITF14		0x00000200
#define	DC_ITF6			0x00000400
#define	DC_MATRIX25		0x00000800
#define	DC_MODULE25		0x00001000
#define	DC_REGULAR39	0x00002000
#define	DC_FULLASCII39	0x00004000
#define	DC_CODABAR		0x00008000
#define	DC_2OUTOF5		0x00010000
#define DC_CODE93		0x00020000
#define DC_CODEZ		0x00040000
#define DC_CHNPOST		0x00080000

#define DC_ISSN			0x00800000		//
#define DC_ISBN			0x01000000		//
#define DC_INDUSTRIAL25	0x02000000		//
#define DC_STANDARD25	0x04000000
#define DC_PLESSEY		0x08000000

#define DC_CODE11		0x10000000		//
#define DC_MSI_PLESSEY	0x20000000		//
#define DC_COMPOSITE	0x40000000
#define DC_RSS			0x80000000

//////////////////////////////////////////////////////////////////////////
//	1D 条码的设置值
//#define Init_Enable1D				0x0000a1fd		//一维条码使能 2004.7.1改成UCC/EAN128为默认状态
//#define Init_Enable1D				0x0000ffff		//一维条码使能 2004.7.1改成UCC/EAN128为默认状态
#define Init_Enable1D		( \
							DC_ZASETUP | \
							DC_SETUP128 | \
							DC_CODE128 | \
							DC_EAN8 | \
							DC_EAN13 | \
							DC_UPCE | \
							DC_UPCA | \
							DC_INTER2OF5 | \
							DC_REGULAR39 | \
							DC_CODABAR | \
							DC_UCCEAN128 | \
							DC_RSS |\
							DC_MATRIX25 \
							)


//对各类型条码扩展参数的意义解释
//modify by HUNTON,2004.4.14
// DC_EAN8 定义
#define BIT_EAN8_TRMTCHK	0x00000001		//第0位，允许发送校验位
#define BIT_EAN8_2_ADDON	0x00000002		//第1位，允许2个附加码
#define BIT_EAN8_5_ADDON	0x00000004		//第2位，允许5个附加码
#define BIT_EAN8_TN2EAN13	0x00000010		//第4位，转化为EAN-13，前加0

// DC_EAN13 定义
#define BIT_EAN13_TRMTCHK	0x00000001		//第0位，允许发送校验位
#define BIT_EAN13_2_ADDON	0x00000002		//第1位，允许2个附加码
#define BIT_EAN13_5_ADDON	0x00000004		//第2位，允许5个附加码

// DC_UPCA 定义
#define BIT_UPCA_TRMTCHK	0x00000001		//第0位，允许发送校验位
#define BIT_UPCA_2_ADDON	0x00000002		//第1位，允许2个附加码
#define BIT_UPCA_5_ADDON	0x00000004		//第2位，允许5个附加码
#define BIT_UPCA_0_AHEAD	0x00000008		//第3位，允许以0作前导
#define BIT_UPCA_Coupon		0x00000040		///允许 UPCA+Coupon扩展
#define BIT_UPCA_REQCoupon  0x00000080		///必须 UPCA+Coupon扩展
#define BIT_GS1COUPON_ONLYGS1	0x00000100		///遇到UPC + GS1 的Coupon, 只输出GS1部分 (如果是0,则输出UPC+GS1的信息)
#define BIT_UPCA_CloseSysChar   0x00000200		///不发送系统字符   2014.05.22

// DC_UPCE 定义
#define BIT_UPCE_TRMTCHK	0x00000001		//第0位，允许发送校验位
#define BIT_UPCE_2_ADDON	0x00000002		//第1位，允许2个附加码
#define BIT_UPCE_5_ADDON	0x00000004		//第2位，允许5个附加码
#define BIT_UPCE_0_AHEAD	0x00000008		//第3位，允许以0作前导
#define BIT_UPCE_TN2UPCA	0x00000010		//第4位，转化为UPC-A，前加0
#define BIT_UPCE_ADDONREQ   0x00000020       /*第5位，要求有附加码*/
#define BIT_UPCE_TDoUPCA    0x00000080       ///Bit7, 按照UPCA的设置来处理   2014.05.22
#define BIT_UPCE_USCode     0x00000100		///Bit8,  要求发送国家码(USA)			2014.05.22

// DC_REGULAR39	定义
#define BIT_R39_ENBCHK		0x00000001		//第0位，允许校验
#define BIT_R39_TRMTCHK		0x00000002		//第1位，允许发送校验位
#define BIT_R39_RSVSTAR		0x00000004		//第2位，允许保留*号起始符终止符

#define BIT_Code32PreCh     0x00000010		//Code 32 允许有起始符
#define BIT_Code32SpecEdit	0x00000020		//转换为Code32

// DC_FULLASCII39 定义
#define	BIT_F39_ENBCHK		0x00000001		//第0位，允许校验
#define BIT_F39_TRMTCHK		0x00000002		//第1位，允许发送校验位
#define BIT_F39_RSVSTAR		0x00000004		//第2位，允许保留*号起始符终止符

// DC_INTER2OF5 定义
#define BIT_I25_ENBCHK		0x00000001		//第0位，允许校验
#define BIT_I25_TRMTCHK		0x00000002		//第1位，允许发送校验位

// DC_ITF14 定义
#define BIT_I14_TRMTCHK		0x00000004		//第2位，允许发送校验位

// DC_ITF6 定义
#define BIT_I6_TRMTCHK		0x00000008		//第3位，允许发送校验位

// DC_MATRIX25 定义
#define BIT_MA25_TRMTCHK    0x00000001		//第0位，允许发送校验位

// DC_MODULE25 定义
#define BIT_MO25_TRMTCHK    0x00000001		//第0位，允许发送校验位

// DC_CODEBAR定义
#define	BIT_CODA_ENBCHK		0x00000001		//第0位，允许校验
#define BIT_CODA_TRMTCHK	0x00000002		//第1位，允许发送校验位
#define BIT_CODA_STAREND	0x00000004		//第2位，允许保留起始符终止符
#define BIT_CODA_ABCDTNXE	0x00000010		/*第4位，起始符与终止符为ABCD/TN*E格式 2009.11.30*/
#define BIT_CODA_LOWERCASE	0x00000020		/*第5位,起始符与终止符为小写 2009.11.30*/

/*DC_MSIPLESSEY 2009.11.30*/
#define BIT_MSI_MASK_CHK	0x00000003		/* 0为无校验,1为单MOD10校验,2为双MOD10校验,3为MOD10/MOD11双校验*/
#define BIT_MSI_TRMTCHK		0x00000004		/*发送校验位*/

/*DC_Composite 2009.11.30*/
#define BIT_CC_EANUPC		0x00000001		/*EAN/UPC的复合码允许*/

/*DC_PLESSEY 2009.11.30*/
#define BIT_PLSY_CHK		0x00000001
#define BIT_PLSY_TRMTCHK	0x00000002

/*DC_STANDARD25  2009.11.30*/
#define BIT_STD25_CHK		0x00000001
#define BIT_STD25_TRMTCHK	0x00000002

/*DC_INDUSTRIAL25 2009.11.30*/
#define BIT_IND25_CHK		0x00000001
#define BIT_IND25_TRMTCHK	0x00000002

/*DC_ISBN 2009.11.30*/
#define BIT_ISBN_10DIGIT	0x00000001

//	1D 条码的设置值
//////////////////////////////////////////////////////////////////////////

//细部参数打开和关闭
#define  POS_DM_129_ENABLE  0   // 129 关闭打开设定
#define  POS_DM_MOBILE      1   // 是否针对 Mobile 
#define  POS_DM_LOTTERY     2   // USA  Lottery
#define  POS_DM_DOUBLE0     3   //双码设定 0--single, 1--trueDouble,2---both
#define  POS_DM_DOUBLE1     4
#define  POS_DM_DISABLE_RECT  7   //关闭Rectangular的识别
#define  POS_DM_ADD_PAD_CODE  8   // 信息后，是否添加PAD后面的码词

#define  DC_DM_129_ENABLE (1<<POS_DM_129_ENABLE)  // 129 使能
#define  DC_DM_MOBILE     (1<<POS_DM_MOBILE)	  // 
#define  DC_DM_LOTTERY    (1<<POS_DM_LOTTERY)    
#define  DC_DM_DOUBLE     ((1<<POS_DM_DOUBLE0)+(1<<POS_DM_DOUBLE1))
#define  DC_DM_DISABLE_RECT (1<<POS_DM_DISABLE_RECT)  
#define  DC_DM_ADD_PAD_CODE  (1<<POS_DM_ADD_PAD_CODE)   // 信息后，是否添加PAD后面的码词
//正反向参数位
#define  POS_DM_Reverse0     16 //正反向设定：0--只读正向, 1--只读反向,2--- 正反向都读
#define  POS_DM_Reverse1     17 
#define  DC_DM_Reverse    ((1<<POS_DM_Reverse0)+(1<<POS_DM_Reverse1))

//镜像参数位
#define  POS_DM_Mirror       18 //
#define  DC_DM_Mirror     (1<<POS_DM_Mirror)

//ECI开关
#define POS_DM_ECICLOSE		19	//0 -- 输出ECI， 1 -- 不输出ECI
#define DC_DM_ECICLOSE		(1<<POS_AZTEC_ECICLOSE)

//////////////////////////////////////////////////////////////////////////


#define  POS_AZTEC_MULTINUM0    0   //多码同图数量
#define  POS_AZTEC_MULTINUM1    1
#define  POS_AZTEC_MULTINUM2    2
#define  POS_AZTEC_MULTIEN0     3   //多码同图使能0--None, 1--Only, 2---Mixed
#define  POS_AZTEC_MULTIEN1     4

#define  DC_AZTEC_MULTINUM ((1<<POS_AZTEC_MULTINUM0)|(1<<POS_AZTEC_MULTINUM1)|(1<<POS_AZTEC_MULTINUM2))
#define  DC_AZTEC_MULTIEN ((1<<POS_AZTEC_MULTIEN0)|(1<<POS_AZTEC_MULTIEN1))
//ECI开关
#define POS_AZTEC_ECICLOSE		19	//0 -- 输出ECI， 1 -- 不输出ECI
#define DC_AZTEC_ECICLOSE		(1<<POS_AZTEC_ECICLOSE)
//////////////////////////////////////////////////////////////////////////

#define  POS_QR_DOUBLE0     3   //双码设定 0--single, 1--trueDouble,2---both
#define  POS_QR_DOUBLE1     4
#define	 POS_QR_MicroQR_ON   22   
	
#define  DC_QR_DOUBLE     ((1<<POS_QR_DOUBLE0)+(1<<POS_QR_DOUBLE1))

//ECI开关
#define POS_QR_ECICLOSE		19	//0 -- 输出ECI， 1 -- 不输出ECI
#define DC_QR_ECICLOSE		(1<<POS_QR_ECICLOSE)

//////////////////////////////////////////////////////////////////////////

/*  5 4 3   ---  3表示开关，0 --- 都可以，1 --- 仅仅指定的多个码同时识别(最多四码同图识别)
0 0 b b b 0 0 0
| | |__0 ---- 两者,1  --- 仅仅指定的数据有效
|_|______00 -- 单码，01 -- 2码 ,10 -- 3码，11 -- 4码 */

#define  POS_PDF417_DOUBLE0     3   //双码设定 0--single, 3--trueDouble,2---both
#define  POS_PDF417_DOUBLE1     4
#define  POS_PDF417_DOUBLE2     5   //
	
#define  DC_PDF417_DOUBLE     ((1<<POS_PDF417_DOUBLE0)+(1<<POS_PDF417_DOUBLE1)+(1<<POS_PDF417_DOUBLE2))



#define POS_PDF417_TRUNCATE0    6   //截断和全码设定：0--both, 1--only full,2--- only truncate
#define POS_PDF417_TRUNCATE1    7
#define DC_PDF417_TRUNCATE     ((1<<POS_PDF417_TRUNCATE0)+(1<<POS_PDF417_TRUNCATE1))


//正反向参数位
#define  POS_PDF417_Reverse0     16 //正反向设定：0--只读正向, 1--只读反向,2--- 正反向都读
#define  POS_PDF417_Reverse1     17 
#define  DC_PDF417_Reverse    ((1<<POS_PDF417_Reverse0)+(1<<POS_PDF417_Reverse1))


//镜像参数位
#define  POS_PDF417_Mirror       18 //镜像
#define  DC_PDF417_Mirror     (1<<POS_PDF417_Mirror)

//ECI开关
#define POS_PDF417_ECICLOSE		19	//0 -- 输出ECI， 1 -- 不输出ECI
#define DC_PDF417_ECICLOSE		(1<<POS_PDF417_ECICLOSE)

//汉信码
#define  POS_CSC_DOUBLE0     3   //双码设定 0--single, 1--trueDouble,2---both
#define  POS_CSC_DOUBLE1     4
#define  POS_CSC_Reverse0    16 ///正反相码设定: 0--正相, 1:反相, 2:both
#define  POS_CSC_Reverse1    17

#define  DC_CSC_Reverse		((1<<POS_CSC_Reverse0)+(1<<POS_CSC_Reverse1))

#define  DC_CSC_DOUBLE		((1<<POS_CSC_DOUBLE0)+(1<<POS_CSC_DOUBLE1))

//////////////////////////////////////////////////////////////////////////
#endif //__UIMG_COMM_DEFINE_H__



