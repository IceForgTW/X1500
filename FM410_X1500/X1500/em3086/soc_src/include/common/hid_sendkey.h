/*******************************************************************************************
	created:	2006/12/19  10:00
	filename: 	uBase-II\Apps\Decoder\HR200\hid_sendkey.h
	file path:	uBase-II\Apps\Decoder\HR200
	author:		lf
	
	purpose:	
********************************************************************************************/
#ifndef __H_FILE_hid_sendkey__

#define __H_FILE_hid_sendkey__

#if USB_DEBUG
#define LOG(arg...) diag_printf(arg)
#else
#define LOG(arg...)
#endif

#define KBW_IN_EP	1

#define KBD_USAGE_PAGE_SIZE		8

//KEYBOARD	COUNTRY CODE:
#define HID_KBW_COUNTRY_US					0
#define HID_KBW_COUNTRY_BELGIUM				1
#define HID_KBW_COUNTRY_BRAZIL				2
#define HID_KBW_COUNTRY_CANADA				3	//(FRENCH)
#define HID_KBW_COUNTRY_CZECHOSLOVAKIA		4	
#define HID_KBW_COUNTRY_DENMARK				5
#define HID_KBW_COUNTRY_FINLAND				6	//(SWEDEN)
#define HID_KBW_COUNTRY_FRANCE				7
#define HID_KBW_COUNTRY_GERMANY_AUSTRIA		8	//	GERMANY/AUSTRIA
#define HID_KBW_COUNTRY_GREECE				9
#define HID_KBW_COUNTRY_HUNGARY				10	
#define HID_KBW_COUNTRY_ISRAEL				11	//(HEBREW)
#define HID_KBW_COUNTRY_ITALY				12
#define HID_KBW_COUNTRY_LATIN_AMERICA		13
#define HID_KBW_COUNTRY_NETHERLANDS			14	//(DUTCH)
#define HID_KBW_COUNTRY_NORWAY				15
#define HID_KBW_COUNTRY_POLAND				16
#define HID_KBW_COUNTRY_PORTUAGAL			17
#define HID_KBW_COUNTRY_ROMANIA				18
#define HID_KBW_COUNTRY_RUSSIA				19
#define HID_KBW_COUNTRY_SCS					20
#define HID_KBW_COUNTRY_SLOVAKIA			21
#define HID_KBW_COUNTRY_SPAIN				22
#define HID_KBW_COUNTRY_SWEDEN				23
#define HID_KBW_COUNTRY_SWITZERLAND			24	//(GERMAN)
#define HID_KBW_COUNTRY_TURKEY_F			25
#define HID_KBW_COUNTRY_TURKEY_Q			26
#define HID_KBW_COUNTRY_UK					27
#define HID_KBW_COUNTRY_JAPAN				28


//fun_key_status
#define HID_KBW_KEY_L_CTRL		0x01
#define HID_KBW_KEY_L_SHIFT		0x02
#define HID_KBW_KEY_L_ALT		0x04
#define HID_KBW_KEY_L_WIN		0x08
#define HID_KBW_KEY_R_CTRL		0x10
#define HID_KBW_KEY_R_SHIFT		0x20
#define HID_KBW_KEY_R_ALT		0x40
#define HID_KBW_KEY_R_WIN		0x80
#define HID_KBW_KEY_CAPS		0x0100		//可以配合字母 
#define HID_KBW_KEY_NUMLCK		0x0200
#define HID_KBW_KEY_CAPS_ON		0x0400
#define HID_KBW_KEY_CAPS_OFF	0x0000 
// 

#define HID_KBW_KBD_MASK	0xa55a0ff0

//flag
#define HID_KBW_KEY_MODE_NORMAL		0
#define HID_KBW_KEY_MODE_FUN		1
#define HID_KBW_KEY_MODE_DOWN		2
#define HID_KBW_KEY_MODE_UP			4
#define HID_KBW_KEY_MODE_KEY		8
#define HID_KBW_KEY_MODE_WAIT_SEND	0x80000000

#define KBD_STAT_NUM_LOCK		1
#define KBD_STAT_CAPS_LOCK		2
#define KBD_STAT_SCROLL_LOCK	4


//keyboard style
#define _CAPS_LOCK			0		//输出字符大写转小写，小写转大写
#define _SHIFT_LOCK			1		//
#define _NUM_LOCK			2
#define _CTRL				3
#define _ALT				4
#define _NUMERIC_KEYPAD_MODE		5		//	sends numeric characters as if 
											//	entered from a numeric keypad.Default = Off
#define _CONTROL_ASCII_MODE			6		// The imager sends key combinations for ASCII
											//control characters for values 00-1F.
#define _KEYBOARD_TEST_MODE			7		//测试模式，键值为0的转换为空格对应键值发送
#define _KEYSTROKE_DELAY			8		//发送每个字符之间的延时设置
#define _CONVERT_CASE				9		//强制转换所有字母都成为大写或者小写
#define _EMULATE_KEYPAD				10		//采用ALT+ Keypad输入ASCII值 来产生字符 
#define _UNKNOWN_CHARACTERS_BEEP	11		//遇到未知字符是否声音提示
#define _KEYPAD_STATE				12		//小键盘是否打开
#define _FAST_MODE                  13      //快速模式是否打开
#define _Poll_Rate                  14      //轮询速率
#define _Code_Page                  15      //Code page

// keyboard style control
#define KEY_OFF				0
#define KEY_ON				1

//Keystroke Delay control
#define	KEYSTROKE_NO_DELAY			0		//Default: No delay 
#define	KEYSTROKE_MEDIUM_DELAY		1		//Delay 20 msec
#define	KEYSTROKE_LONG_DELAY		2		//Delay 40 msec

//Convert case control
#define CONVERT_NONE					0		//Default: No Case Conversion
#define CONVERT_ALL_UPPER_CASE			1		//Convert All to Upper Case
#define CONVERT_ALL_LOWER_CASE			2		//Convert All to Lower Case

//Emulate keypad control
#define EMULATE_KEYPAD_OFF			0		//Default: OFF
#define EMULATE_KEYPAD_ON			1		//Enable Emulate Keypad mode

//UNKNOWN_CHARACTERS_BEEP
#define UNKNOWN_CHARACTERS_BEEP_OFF		0	//default: OFF
#define UNKNOWN_CHARACTERS_BEEP_ON		1	//Enable an error beep sounds

/*******************************************************************/
int keyboard_style_set(int style,int control);
int send_key_str(unsigned char *str,int length, int nCharEncoding);
int keyboard_country_set(int country);


#define IBM_SurePOS_TYPE_TableTop	1
#define IBM_SurePOS_TYPE_HandHeld	2
#endif		

/************************  END OF FILE  ****************************/

