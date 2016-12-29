
#ifndef __UIMG_DEVELOP_INTERFACE_MACROLINK_H__ 
#define __UIMG_DEVELOP_INTERFACE_MACROLINK_H__

/*********************************************************************************************
*	include files 
*********************************************************************************************/
#include "../../list.h"

/*********************************************************************************************
*	global referenced macro defines & type defines
*********************************************************************************************/

#define  MACRO_DEAL_VERSION  "2.01"// Macro �괦����Ч


//�ͼ�������������й�ϵ
#define U_MACRO_ID_UNCONFORMITY		-10		// ��Ч����ԭ���ĺ�ID����: ͬһ������ MACRO_ERROR
#define U_MACRO_SUPERPOSITION		-11		// ���ظ���ȡ // MACRO_ERROR
#define U_MACRO_TYPE_UNCONFORMITY	-12		// ��ԭ���ĺ����Ͳ��� : ��ͬ�������� // MACRO_ERROR
#define U_MACRO_OVERFLOW			-13		// ˳����/����: ��������������(MACRO_MAX_LINK == 16)�򻺴泬��64k����// MACRO_ERROR

// ����Ϣ���ͣ������й�
#define U_NO_MACRO					 0		// ��ͨ����Ϣ // Nothing
#define U_MACRO_END					 1		// ˳�򻺴�������������� //MACRO_END
												// ** ˳���ͽ�����Ҳ�Ǵ˷��ز���
#define U_MACRO_SEQ_SEND	         2		// ˳���� / �ɷ������Ӻõ��ַ� //MACRO_OK
#define U_MACRO_BUFF		         10		// ˳����/������: ֻ�治��// MACRO_OK

#define U_uIMG_STATUS_MACRO_START		0x000200   // ����뿪ʼ
#define U_uIMG_STATUS_MACRO_END			0x000201   // ��������
#define U_uIMG_STATUS_MACRO_ERROR		0x000202   // ��������
#define U_uIMG_STATUS_MACRO_SUC			0x000203   // �������ȷ


// �����ö���
#define  MACRO_DIRECT				0x01	//��ȡ��ֱ�ӷ��ͣ��������κδ���
#define  MACRO_SEQ_BUFF				0x02	//����˳���ͣ����Զ��64k�Ļ��棬�������棬ϵͳ�������Զ���գ���Ҫ��ȡ�������ȡ�����buff
#define  MACRO_LINK_16				0x03	//��ȡ��ϣ�����˳�����Ӻ��ͣ��������Ӷ��16������С��64k��Ϣ�ķ���


/*********************************************************************************************
*	global referenced variables
*********************************************************************************************/

/*********************************************************************************************
*	global function prototype
*********************************************************************************************/


#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************
����ֵ----
	#define U_RE_MACRO_ERROR            -1      
	#define U_RE_MACRO_NO_END_SEND      0		// �Ǻ�����/�����/����Ϣ����
	#define U_RE_MACRO_BUFF_DELAY       2       // �����ӳ�,��������Ϣ������ɹ�
************************************************************************/
//��ʼ������
void W_macroInitialParam(void);
//��ȡ�����ú괦��취
// MACRO_DIRECT �� MACRO_SEQ_BUFF �� MACRO_LINK_16
int W_macroGetParam(void);
void W_macroSetParam(int nMacro);

//��������ӵ�
int W_macroMain(void);

//��ȡ��Ϣ,���䳤��
//  �������ݿ��ܳ���ԭ����������ݿռ�Ĵ�С�����ԣ��������⿪�ٿռ�
unsigned char* W_macroGetBuff(void); 
int W_macroGetBuffLen(void);


//��ȡ�괦�������Ϣ״ֵ̬
int W_macroGetError(void);
void  W_macroSetError(int nError);

// ����Ϣ������״̬
int W_macroGetStatus(void);

//�Ƿ���Ϻ�����
// TRUE ---  ��������
// FALSE --- ����ֳ�ͻ
BOOL W_macroGetUnmacthParam(void);
//��־���Ƿ�ǰһ�������Ǻ����� <0 �Ͳ��Ǻ�
int W_macroGetFlag(void);
void W_macroSetFlag(int nFlag);

//��Ҫ��ʼ����ı�־
BOOL W_macroGetInitFlag(void);
void W_macroSetInitFlag(void);

#ifdef __cplusplus
}
#endif

#endif //#ifndef __UIMG_DEVELOP_INTERFACE_MACROLINK_H__ 

