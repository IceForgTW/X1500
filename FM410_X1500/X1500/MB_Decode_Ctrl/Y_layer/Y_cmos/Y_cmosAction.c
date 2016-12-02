/********************************************************************
	created:	2007/02/06
	created:	6:2:2007   14:27
	filename: 	Img_Capture.cpp
	file path:	
	file base:	Img_Capture
	file ext:	cpp
	author:		LieMa
	
	purpose:	ͼ��ɼ�����
*********************************************************************/

#include "Y_cmosAction.h"

//#include <stdio.h>
//#include <string.h>
#include "../../list.h"
/************************************************************************/
/* ���Ͷ���                                                             */
/************************************************************************/


/************************************************************************/
/* ȫ�ֱ�־��                                                           */
/************************************************************************/
int _Img_Capture_Inited = 0; //��ʼ�����
int _Img_WantLux = 0x74;//0x98;	//ͼ���������������


/************************************************************************/
/* �궨��                                                               */
/************************************************************************/

/************************************************************************/
/*  ����ʵ��                                                            */
/************************************************************************/
//1.��ʼ��CMOS
void Y_cmosInit(void)
{
	//��ȡCMOS����
	_Img_Capture_Inited = Z_cmos_GetType();

}//End of function;


//static int	imgNum = 0;

// 	#define READ_SCANNING			0
// 	#define READ_DETECETING			1
/*��ȡͼ��
	Xmode: 
		0 -- ������ͼ��
		1 -- �ȴ�һ��ͼ��
		2 -- ֹͣ��ͼ��
	Ymode: 
		0 -- ʶ��״̬��
		1 -- ���״̬��
*/
void Y_cmosCapture_Ex(int Xmode, int Ymode)
{
	static BOOL cap_stop = TRUE;
	//int tmp;
//	int t1;
	if (_Img_Capture_Inited == 0)
	{ Y_cmosInit(); }
	
	if (_Img_Capture_Inited != 0)
	{
		if (Xmode == CAPTURE_START)
		{//������ͼ�����ȴ���ͼ����		
			
			if (cap_stop == FALSE)	//�ɼ�����ǰ�����ٴ������ɼ�
			{
// 				Y_commSendString("capture start err: start\r\n");
				return;
			}

			Z_CmosCapture(TRUE, Ymode);

			cap_stop = FALSE;
		}
		else if (Xmode == CAPTURE_WAIT)
		{//��������ͼ�����ȴ���ͼ����
			if (cap_stop == TRUE)	
				return;

			Z_GetCmosBuf(CMOSGetNewPic, 500);
		}
		else if (Xmode == CAPTURE_STOP)
		{//20FPSģʽ���˳�����ģʽʱ������ֹͣ��ͼ
			if (cap_stop == TRUE)	
				return;

			Z_CmosCapture(FALSE, Ymode);

			cap_stop = TRUE;

// 			Y_commSendString("set over\r\n");
		}
		else if (Xmode == CAPTURE_ONCE_AND_STOP)
		{
			if (cap_stop == FALSE)	//�ɼ�����ǰ�����ٴ������ɼ�
			{
// 				Y_commSendString("capture start err: once\r\n");
				return;
			}
// 			Y_commSendString("capture once\r\n");
			Z_CmosCapture(TRUE, Ymode);

			Z_GetCmosBuf(CMOSGetNewPic, 500);

			Z_CmosCapture(FALSE, Ymode);

// 			Y_commSendString("capture once over\r\n");

		}
		
	}//End of if;
	
}//End of function;


// static BOOL bCmosAdjed = FALSE;

//2.���ͼ��
void Y_cmosCapture(void)
{
	Y_cmosCapture_Ex(CAPTURE_ONCE_AND_STOP, READ_DETECETING);		//������ͼ�����ȴ���ͼ����

}//End of function;




//3.����Ԥ������
void Y_cmosAdjust(void)
{
//	int nLux;

	if (_Img_Capture_Inited == 0)
	{ Y_cmosInit(); }

	if (_Img_Capture_Inited != 0)
	{
// 		nLux = Y_cmosCalcLux();
// 		if (lux_multiple>1)
// 		{
// 			nLux *= lux_multiple;
// 		}
// 		if(TSImgTypeOperate[_Img_CurOperateOrder].fnc_Adjust != NULL)
// 		{
//  			TSImgTypeOperate[_Img_CurOperateOrder].fnc_Adjust(nLux,_Img_WantLux);
// //			_Img_WhenLastAdjust = Z_Get10msCounter();
// 		}
	}//End of if;

}//End of function;
//4.��ȡͼ����������ֵ
int Y_cmosCalcLux(void)
{
//	BYTE *pImg;

	if (_Img_Capture_Inited == 0)
	{ Y_cmosInit(); }

	if (_Img_Capture_Inited != 0)
	{
		
	}//End of if;

	return 0;
}//End of function;
//5.��ȡͼ����
int Y_cmosGetWidth(void)
{
	if (_Img_Capture_Inited == 0)
	{ Y_cmosInit(); }

	if (_Img_Capture_Inited != 0)
	{
		return Z_GetCmosWidth();
	}//End of if;

	return 0;
}//End of function;
//6.��ȡͼ��߶�
int Y_cmosGetHeight(void)
{
	if (_Img_Capture_Inited == 0)
	{ Y_cmosInit(); }

	if (_Img_Capture_Inited != 0)
	{
		return Z_GetCmosHeight();
	}//End of if;

	return 0;
}//End of function;
//7.��ȡͼ���ַ
char * Y_cmosGetDataAddr(void)
{
	char * pcAddr = NULL;

	if (_Img_Capture_Inited == 0)
	{ Y_cmosInit(); }

	if (_Img_Capture_Inited != 0)
	{
		pcAddr = (char*)Z_GetCmosBuf(CMOSGetUsedPic, 0);
	}//End of if;

	return pcAddr;
}//End of function;
//8.����ͼ���Ԥ������
void Y_cmosSetWantLux(int nWantLux)
{
// 	if (_Img_Capture_Inited == 0)
// 	{ Y_cmosInit(); }
// 
// 	if (nWantLux < 0)
// 	{ nWantLux = 0; }
// 	if (nWantLux != _Img_WantLux)
// 	{
// 		_Img_WantLux = nWantLux;
// 		Y_cmosAdjust();
// 	}//End of if;
}//End of function;

//10.����CMOS�ɼ�Ϊ����, nMode: 0 - Normal; 1 - horizontal mirror; 2 - vertical flip; 3 - horizontal mirror & vertical flip.
void Y_cmosMirror(int nMode)
{
	if (_Img_Capture_Inited == 0)
	{ Y_cmosInit(); }

	if (_Img_Capture_Inited != 0)
	{
		Z_SetCmosMirror(nMode);
	}//End of if;
}//End of function;



void Y_cmosSetUserFunc(UTGeneralParam *psParam)
{
//	int nCnt;

	if (psParam == NULL || psParam->Id != STID_CMOSEXTFNC)
	{ return; }//End of if;

// 	nCnt = sizeof(TSImgTypeOperate)/sizeof(TYCMOSOperate) - 1;
// 	if (nCnt <= 0)
// 	{
// 		return;
// 	}//End of if;
// 	
// 	if (psParam->psParamStruct == NULL)
// 	{
// 		memset(&TSImgTypeOperate[nCnt],0,sizeof(TYCMOSOperate));
// 		return;
// 	}//End of if;
// 	else
// 	{
// 		memcpy(&TSImgTypeOperate[nCnt],psParam->psParamStruct,sizeof(TYCMOSOperate));
// 	}

	return;
}//End of function;


