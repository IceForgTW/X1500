
#ifndef __UIMG_MODULE_OSIN_DEFINE_H__
#define __UIMG_MODULE_OSIN_DEFINE_H__


#include "../list.h"


//接口类型定义及说明

//功能: 重设看门狗复位时间,并重新开始计时
//返回值: 1表示成功,0表示失败

//1参数导入与存储
//功能参数存取 ,每1个SectOffs间隔4096字节. 总共只有12个Sector，允许偏移量 0 ~ 11
//返回值: 实际存取的长度

//////////////////////////////////////////////////////////////////////////
//  OS 提供的接口功能 组合结构体
typedef struct tagINTFACE{
	void	(*beep)( int freq, int mdelay, int volume );//功能: 按指定的频率/延续时间/响度来发声, 单位是(Hz,ms,无),响度级为 (1,10);
	void	(*playmusic)(char * pcMusic);//根据字符串的音谱进行演奏音乐
	void	(*usleep)(int us);//微秒级等待
	void	(*delay)(int ms);//毫秒级等待
	int		(*counterget)(void); //获取系统节拍
	int		(*counterdensity)(void); //获取节拍精度，单位毫秒
	void	(*oscillate)(int ms); //振动
	void	(*dogfeed)(void);  //看门狗复位
	int		(*doggetbreak)(void); //获取看门狗复位时间
	int		(*dogreset)(int ms); //看门狗重置,若设为小于等于0的值,则表示关闭看门狗

	int		(*keypressed)(void); //检测被按下的键值,0为没有按键
	void	(*ledon)(int clr); //开启指定颜色的Led
	void	(*ledoff)(int clr); //关闭指定颜色的Led
	void	(*lighton)(void); //开启照明灯
	void	(*lightoff)(void); //关闭照明灯
	void	(*irlighton)(void); //开启红外照明灯
	void	(*irlightoff)(void); //关闭红外照明灯
	void	(*focuson)(void); //开始对焦灯
	void	(*focusoff)(void); //关闭对焦灯
	int		(*cmos_gettype)(void); //获取CMOS类型

	int		(*cmos_geti2c)(int reg); //获取CMOS 寄存器值
	int		(*cmos_seti2c)(int reg, int value); //设置CMOS寄存器值
	int*	(*cmos_read)(int mode); //拍摄一幅图像
	int*	(*cmos_getimgbuff)(void); //获取图像地址
	int		(*param_load)(unsigned char * buff, int nSectOffs, unsigned int size); //读指定信息块到缓存
	int		(*param_save)(unsigned char * buff, int nSectOffs, unsigned int size); //将信息写入指定信息块
	int		(*comm_gettype)(void); //获取通讯口类型
	int		(*comm_read)(unsigned char * buff, unsigned int size);//功能: 从通讯口中读出指定大小以内的数据 返回读出的字节数
	int		(*comm_write)(unsigned char * buff, unsigned int size);//功能: 通过通讯口发送数据 返回实际发送的数据量
	int		(*comm_config)(void * lpsParam);//功能: 改变通讯口的配置(参数,或方式等) 1表示成功,0表示失败

	int		(*comm_getstatue)(void * lpsParam);//功能: 获取通讯口的配置参数 返回 1为成功,0为失败
	int		(*oem_getuid)(unsigned char * buff, unsigned int maxsize); //获取UID
	int		(*oem_getlicense)(unsigned char * buff, unsigned int maxsize); //获取许可证
	int		(*oem_setlicense)(unsigned char * buff, unsigned int size);//设置许可证 返回 1为成功,0 为失败
	int		(*oem_licenselock)(void);//锁定许可证,避免被设置 返回 1为成功,0 为失败.
	int		(*syssleep)(void);//进入休眠,能自动唤醒 返回 1为成功, 0为失败
	void	(*reset)(void);
	void	(*OscillateWithBeep)(int freq, int mdelay, int volume); //发声,同时振动
	int		(*GetHardwareInfo)(char *pBuff, int nBuffSize); //获取硬件版本信息
	void	(*DeepSleep)(void);	//深度休眠

	void	(*PowerOff)(void);	//关机
	void	(*slvlighton)(void);//辅助照明灯开，用于自动模式
	void	(*slvlightoff)(void);//辅助照明灯关，用于自动模式
	void	(*wakeupTimer)(int ms); //用于唤醒的倒计时器
	int     (*SetDeviceSerial)(char *serialNum, int len); //产品序列号

	void	(*setCMOSCollect)(int mode);
	int		(*getCMOSCollect)(void);

	int		(*getCMOSDMANum)(void);	//获取已完成采集的DMA表的数字
	void	(*setCMOSDMAInuseNum)(int num);//设置要锁定的DMA表的数字
	int*	(*cmos_getimgbuff_Ex)(void);//30FPS模式下获取被锁定的用于解码的图像地址

	int		(*sysIdle)(void);//进入休眠,能自动唤醒 返回 1为成功, 0为失败
	int		(*sysStandby)(void);//进入standby,能自动唤醒 返回 1为成功, 0为失败

	char*	(*ISRAMAddr)(void);//返回内部SRAM地址，使用空间不超过128kBytes
	void	(*playmusic_ex)(char * pcMusic, int volume);//根据字符串的音谱进行演奏音乐,响度级为 (1,10)

	void	(*extLightCtrl)(BOOL bOn);//外部照明灯控制
	int		(*SetDeviceManufacturer)(char *str, int len);	//产品制造商
	
	void	(*SetCmosLightMode)(int nMode);
	int		(*GetCmosLightMode)(void);
	
	int		(*GetHardwareInfo_NCR)(char *pBuff, int nBuffSize); //获取硬件版本信息NCR格式

	void	(*SetPowerFrequency)(int nFreqMode, BOOL bVoltage);	//设置频率

	void	(*SetLightOnDuration)(int nLine);	//设置每次曝光照明灯亮的上限时间，单位是行

	void	(*SetLightMode)(int mode);	//设置曝光模式为phone还是normal

	unsigned int		(*MScounterget)(void); //获取ms节拍
	void	(*UsbResumeHandle)(void *p);//usb resume后 处理函数的指针传递
	int 	(*getKernelInfo)(int type, char *pBuff, int nBuffSize); //获取内核信息: type = 0 为固件版本号， 1 为设备名 
	void	(*firmwareUpgrade)(int Port); //固件更新接口，port = 0为RS232, 1 为datapipe， 2为USB COM

	void	(*setPowerLedOn)(BOOL b); //power led on 设置， 仅对HR32有用
	
	void	(*CmosCapture)(BOOL b, int mode);	// 启动/停止采集图像
	unsigned char* (*GetCmosBuf)(unsigned int mode, unsigned int timeout);//获取图像，mode： 0 为旧图像， 1 为新图像； timeout：等待超时

	int		(*GetCmosWidth)(void);	//
	int		(*GetCmosHeight)(void);	//
	void	(*SetCmosMirror)(int nMode);	//nMode: 0 - Normal; 1 - horizontal mirror; 2 - vertical flip
	unsigned int (*GetCRC32)(unsigned char *pdata, int len);
	unsigned char* (*GetCmosBufTest)(int n);//获取指定缓存的图像数据
    void (*TimeoutHandle)(int chn, unsigned int us, void (*handler)(void));
    unsigned short (*GetDevMagic)(void);
    void (*TransferIdleTickVar)(int *pTick); 
    void (*GetCPUID)(unsigned char *cpuID);
    unsigned int (*EncryptionWrtie)(unsigned char *buff, unsigned int nsize, unsigned int flag);
     unsigned int (*EncryptionRead)(unsigned char *buff, unsigned int nsize, unsigned int flag);
    void	(*SetKBWCharacterEncoding)(int nMode);	//设置usb kbw对字符编码的解析方式
	void    (* isSetKBWrestore)(int nFormat);//码词是否经过dataformat处理的
	void (*isDecoding)(int nFlag);
	void*	reserve[96];
   
}osINTFACE;

#endif //__UIMG_MODULE_OSIN_DEFINE_H__


