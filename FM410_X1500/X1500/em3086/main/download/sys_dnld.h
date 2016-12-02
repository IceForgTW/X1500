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

#define DN_ZONE_BOOTROM		"boot"	//BootRom,忽略附加参数
#define DN_ZONE_KERNEL		"kern"	//系统内核,忽略附加参数
#define DN_ZONE_APP			"appl"	//application,忽略附加参数
#define DN_ZONE_FLASH		"flah"	//写入系统flash,附加参数指定块号
#define DN_ZONE_FACTORYINFO	"faif"	//出厂信息,包括序列号与出厂日期等

//代码	说明									
//"boot"	BootRom,忽略附加参数				
//"kern"	系统内核,忽略附加参数				
//"deco"	Decode app,忽略附加参数 			
//"faif"	出厂信息,包括序列号与出厂日期等 	
//"font"	字库,附加参数指明字库类型			
//"nand"	写入nand flash,附加参数指明块号 	
//"shel"	系统shell,忽略附加参数
//"appl"	application,忽略附加参数{
//"file"	,忽略附加参数
//"para"	写入系统参数区,附加参数指定类型}
//"flah"	写入系统flash,附加参数指定块号
//"memy"	写入系统内存,附加参数指定地址

//////////////////////////////////////////////////////////////////////////
#define SYS_DN_PORT_RS232		0
#define SYS_DN_PORT_UDP			1
#define SYS_DN_PORT_CDC			2

#define SYS_DN_SAVE_SPACE_AUTO		0		//根据协议自动选择
#define SYS_DN_SAVE_SPACE_FLASH		1		//存于Flash
#define SYS_DN_SAVE_SPACE_FS		2		//存于文件系统

#define UDP_OPEN_MODE_SER			1
#define UDP_OPEN_MODE_NOR			0

//////////////////////////////////////////////////////////////////////////

typedef struct _kern_dn_struct_{
	int PortType;			//通讯口类型
	int PortIdx;			//通讯口索引号
	int SaveSpace;			//保存在Flash或是文件系统上
	int chk_open;			//open设备失败时是否退出  0 忽略, 其它值表示出错退出
	void *port_init_param;	//通讯口初始化参数
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
