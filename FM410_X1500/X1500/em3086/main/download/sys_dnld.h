/********************************************************************
	created:	2007/06/26
	filename: 	sys_dnld.h
	file path:	G:\Working\2027_uBase\bsp\MB2027\src\Download
	author:		Zencg
	
	purpose:	
*********************************************************************/

#ifndef _HEAD_FILE_sys_dnld_
#define _HEAD_FILE_sys_dnld_

//////////////////////////////////////////////////////////////////////////

#define DN_ZONE_BOOTROM		"boot"	//BootRom,���Ը��Ӳ���
#define DN_ZONE_KERNEL		"kern"	//ϵͳ�ں�,���Ը��Ӳ���
#define DN_ZONE_APP			"appl"	//application,���Ը��Ӳ���
#define DN_ZONE_FLASH		"flah"	//д��ϵͳflash,���Ӳ���ָ�����
#define DN_ZONE_FACTORYINFO	"faif"	//������Ϣ,�������к���������ڵ�

//����	˵��									
//"boot"	BootRom,���Ը��Ӳ���				
//"kern"	ϵͳ�ں�,���Ը��Ӳ���				
//"deco"	Decode app,���Ը��Ӳ��� 			
//"faif"	������Ϣ,�������к���������ڵ� 	
//"font"	�ֿ�,���Ӳ���ָ���ֿ�����			
//"nand"	д��nand flash,���Ӳ���ָ����� 	
//"shel"	ϵͳshell,���Ը��Ӳ���
//"appl"	application,���Ը��Ӳ���{
//"file"	,���Ը��Ӳ���
//"para"	д��ϵͳ������,���Ӳ���ָ������}
//"flah"	д��ϵͳflash,���Ӳ���ָ�����
//"memy"	д��ϵͳ�ڴ�,���Ӳ���ָ����ַ

//////////////////////////////////////////////////////////////////////////
#define SYS_DN_PORT_RS232		0
#define SYS_DN_PORT_UDP			1
#define SYS_DN_PORT_CDC			2

#define SYS_DN_SAVE_SPACE_AUTO		0		//����Э���Զ�ѡ��
#define SYS_DN_SAVE_SPACE_FLASH		1		//����Flash
#define SYS_DN_SAVE_SPACE_FS		2		//�����ļ�ϵͳ

#define UDP_OPEN_MODE_SER			1
#define UDP_OPEN_MODE_NOR			0

//////////////////////////////////////////////////////////////////////////

typedef struct _kern_dn_struct_{
	int PortType;			//ͨѶ������
	int PortIdx;			//ͨѶ��������
	int SaveSpace;			//������Flash�����ļ�ϵͳ��
	int chk_open;			//open�豸ʧ��ʱ�Ƿ��˳�  0 ����, ����ֵ��ʾ�����˳�
	void *port_init_param;	//ͨѶ�ڳ�ʼ������
}DOWNLOAD_PARAM;

typedef struct _dn_ops_struct_{
	int (*open)(int idx);
	int (*close)(int idx);
	int (*set_port_param)(void *param);
	int (*proc_not_comm_cmd)(void *cmd, int cmdlen, void *param, int paramlen);
	int (*read)(void *buf, int len, int timeout);
	int (*write)(void *buf, int len, int timeout);
}DN_OPS;

#ifdef __cplusplus
extern "C"{
#endif
////////////////////////////////////////////////////////

int kernel_download(DOWNLOAD_PARAM *pdp, int steps, int protocol, int wait_timeout);



////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#endif		//#ifndef _HEAD_FILE_sys_dnld_
/*********************************************************************
			End of file
*********************************************************************/
