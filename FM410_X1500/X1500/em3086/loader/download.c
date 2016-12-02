#include <time-tcu.h>
#include <nor_flash.h>
#include <generate.h>

#include <string.h>
#include <stdio.h>
static int DL_PORT = 1;

extern void init_uart(int port, int baudrate);
extern void uart_write_char(int port, const char chr);
extern int uart_read_data(int port, void *buf, int len, unsigned int timeout);
extern void uart_setbrg (int port, int baudrate);
extern char uart_wait_char(int port);
extern int uart_send_data(int port, void *data, int len);

//////////////////////////////////////////////////////////////////////////
//if return not zero, the entry download function
//if return zero, the boot kernel and application
int check_download(int port)
{
    int lvi;
    int lvcnt = 0;
    int lvflag = 0;
    char lvc; 

    init_uart(0,9600);
    //DisableWatchDog();
    while(lvcnt < 10){
		//watch_dog_reset();
		//printf("checkdown\r\n");
		lvi = uart_read_data(port, &lvc, 1, 300);     
		if(lvi <= 0){
			return 0;
		}
		if((lvc != '?') && !lvflag){
			lvcnt ++;
			continue;		//系统启动时可能会接收到一些干扰字符
		}
		if(lvc != '?'){
			return 0;					//收到错误字符,说明不是下载程序
		}
		if(++lvflag >= 3){
			uart_write_char(port, '<');     
                return 1;					//连续收到三个字符才进入下载
		}                  
	}
    //printf("error\r\n");
	return 0;
}

//int flash_write(unsigned int dest, u8 *mem, u32 len);
//uint get_flash_blk_size(void);

#define QS_DL_STX			0x02	//报文标志
#define QS_DL_ATTR			0x05	//报文属性

//命令结构
/*
@  ---   状态切换
!  ---   参数设定
?  ---   查询信息
0/1  ---   Yes/No答复信息
*/
#define QS_DL_SET			'!'		//1Byte，参数设置命令
#define QS_DL_SET_Q			'?'		//1Byte，参数查询命令
#define QS_DL_Comm_Switch   '@'    	//1Byte，功能步骤切换
#define QS_DL_SET_SPEC		'#'    	//1Byte，特殊设置
#define QS_DL_FirmWare_Sign '>'     //1Byte, 下载数据标志
#define QS_DL_ECHO_SUCCESS  '0'     //1Byte, 设定成功回复
#define QS_DL_ECHO_FAIL		'1'     //1Byte, 设定失败回复


// Function setting 
#define  QS3_EXIT_Upgrade  "@Exit"    //退出当前更新 

#define  QS3_NEXT_Upgrade	"@NextDown"		//等待下一个更新

#define  QS3_Upgrade_FirmWare_Step ">Start"  //开始数据下载

#define  QS3_Setting_Comm      "#COMM"   //设定COMM

// Param Setting
#define  QS3_Setting_DataLens "!DataLens"   //设定数据总长度
#define  QS3_Setting_Frames   "!Frames"     //设定总帧数
#define  QS3_Setting_Framesize "!FrameSize" //设定帧大小
#define  QS3_Setting_Address  "!Address"    //设定下载地址
#define  QS3_Setting_FileName  "!FileName"  //设定文件名字
#define  QS3_Setting_FileType  "!FileType"  //设定文件类型
#define  QS3_Setting_Specific	"!BootDownload"


// Param Query
#define  QS3_Query_BootVer   "?BootVer"     //查询Boot版本
#define  QS3_Query_HardVer  "?HardVer"      //查询硬件类型及其版本
#define  QS3_Query_OSVer     "?OSVer"       //查询OS版本

#define  QS3_Query_NULL      "NULL"         //没有查询到任何数据时的回应数据

#define SendCmdFrame(a)		_s_send_cmd_frame((a), sizeof(a))
	

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


typedef struct _dn_info_struct_{
	int datalen;
	int frames;
	int framesize;
	int address;
	int filetype;
	int zone_is_set;
}DN_INFO;

static DN_INFO dninfo;

int string_to_int(char *instr)
{
	int lvret = 0;
	while((*instr >= '0') && (*instr <= '9')){
		lvret = lvret * 10 + *instr - '0';
		instr ++;
	}
	return lvret;
}

/*void int_to_str(char *buf, int nval)
{
	int lvi;
	if(nval < 0){
		nval = -nval;
		*buf++ = '-';
	}
	for(lvi = 28; lvi >= 0; lvi -= 4){
		*buf = (nval >> lvi) & 0xf;
		*buf += *buf > 9 ? ('a' - 10) : '0';
		buf ++;
	}
	*buf = 0;
}*/
unsigned int get_flash_blk_size(void)
{
	return 4 * 1024;
}

static int set_zone(char *zonestr, int len)
{
	char *lvp;
	if(dninfo.zone_is_set) return -1;
	lvp = strchr(zonestr, ':');
	if(lvp)	*lvp++ = 0;

	if(!strcmp(zonestr, DN_ZONE_KERNEL))
		dninfo.address = 0x80000;
	else if(!strcmp(zonestr, DN_ZONE_APP))
		//dninfo.address = 0x170000;
		dninfo.address = 0x80000;	// 0x1f4000 - 0x1a0000 = 344064 (336k)
	else if(!strcmp(zonestr, DN_ZONE_BOOTROM))
		dninfo.address = 0x0000;
	else if(!strcmp(zonestr, DN_ZONE_FACTORYINFO))
		dninfo.address = 0x1f8000;		//0x1ff000;		sector 504
	else if(!strcmp(zonestr, DN_ZONE_FLASH)){
		if(!lvp) return -1;
		dninfo.address = string_to_int(lvp) * get_flash_blk_size();
	} else return -1;		//unknow or not supporly zone

	dninfo.zone_is_set = 1;
	return 0;
}

static void _s_send_cmd_frame(const void *data, int len)
{
	unsigned char lvbuf[256];
	int lvcrc;
	lvbuf[0] = QS_DL_STX;
	lvbuf[1] = QS_DL_ATTR;
	lvbuf[2] = (len >> 8) & 0xff;
	lvbuf[3] = len & 0xff;
	memcpy(lvbuf + 4, data, len);
	lvcrc = Get_CRC32(lvbuf, len + 4);
	lvbuf[len + 4] = lvcrc >> 24;
	lvbuf[len + 5] = lvcrc >> 16;
	lvbuf[len + 6] = lvcrc >> 8;
	lvbuf[len + 7] = lvcrc & 0xff;
	uart_send_data(DL_PORT, lvbuf, len + 8);
    //printf("len=%d\r\n",len+8);
   
}

extern char HardBoardStr[];

static int _s_cmd_ansyies(char *cmd, int len)
{
	int cmdlen;
	int vallen;
	char *lvp;
//	int lvi;

	lvp = strchr(cmd, ':');
	if(!lvp) cmdlen = len;
	else {
		cmdlen = (int)((unsigned int)lvp - (unsigned int)cmd);
		vallen = len - cmdlen - 1;
		lvp ++;
	}
	cmd[cmdlen] = 0;

	if(!strcmp(cmd, QS3_Setting_Comm)){
		int nbaudrate;
		int bits;
		int parity;
		int stopbits;
		SendCmdFrame("0");
		if(!lvp) return -1;
		nbaudrate = string_to_int(lvp);
		lvp = strchr(lvp, ',') + 1;
		bits = string_to_int(lvp);
		lvp = strchr(lvp, ',') + 1;
		parity = string_to_int(lvp);
		lvp = strchr(lvp, ',') + 1;
		stopbits = string_to_int(lvp);
		mdelay(2);
		uart_setbrg(DL_PORT, nbaudrate);
		*cmd = uart_wait_char(1);
        //printf("cmd=%x",*cmd);
		uart_write_char(DL_PORT, *cmd);
	} else if(!strcmp(cmd, QS3_Query_BootVer)){
		SendCmdFrame("V3.2 support watchdog");
	} else if(!strcmp(cmd, QS3_Query_HardVer)){
		//_s_send_cmd_frame(HardBoardStr, strlen(HardBoardStr));
		SendCmdFrame("EM3000-V5");
	} else if(!strcmp(cmd, QS3_Query_OSVer)){		//maybe not need
		SendCmdFrame("uBase-III");
	} else if(!strcmp(cmd, QS3_Setting_FileType)){
		if(set_zone(lvp, vallen)) SendCmdFrame("1");
		else SendCmdFrame("0");
	} else if(!strcmp(cmd, QS3_Setting_DataLens)){
		dninfo.datalen = string_to_int(lvp);
		SendCmdFrame("0");
	} else if(!strcmp(cmd, QS3_Setting_Frames)){		//maybe need modify
		dninfo.frames = string_to_int(lvp);
		SendCmdFrame("0");
	} else if(!strcmp(cmd, QS3_Setting_Framesize)){		//maybe need modify
		dninfo.framesize = string_to_int(lvp);
		SendCmdFrame("0");
	} else if(!strcmp(cmd, QS3_Setting_FileName)){		//maybe not need
		SendCmdFrame("0");
	} else if(!strcmp(cmd, QS3_Setting_Specific)){		//maybe not need
		SendCmdFrame("0");
	} else if(!strcmp(cmd, QS3_Setting_Address)){		//maybe not need
		if(dninfo.zone_is_set){							//zone is set
			SendCmdFrame("1");
		}else {
			dninfo.address = string_to_int(lvp);
			SendCmdFrame("0");
		}
	} else if(!strcmp(cmd, QS3_Upgrade_FirmWare_Step)){
		SendCmdFrame("0");
		return -11;
	} else if(!strcmp(cmd, QS3_EXIT_Upgrade)){
		SendCmdFrame("0");
		return -2;
	} else if(!strcmp(cmd, QS3_NEXT_Upgrade)){
		SendCmdFrame("0");
		return -12;
	} else {		//not support cmd
		return -1;
	}
	return 0;		
}


static int _s_frame_proc(void *buf, int proc_cmd, int buflen)
{
	unsigned char *lvbuf = (unsigned char*)buf;
	int lvlen;
	int lvi;
      int len;
      unsigned int crc;

	//watch_dog_reset();

	if(uart_read_data(DL_PORT, lvbuf, 1, 3000) <= 0) return -1;
	if(*lvbuf != QS_DL_STX){
    // printf("b[1]=%d\r\n",*lvbuf);
		return -3;
	}
	if(!proc_cmd){
		lvi = uart_read_data(DL_PORT, lvbuf + 1, buflen + 4, 1500);

		if(lvi > 4)
		{
			int lvcrc = (lvbuf[lvi + 1 - 4] << 24) | (lvbuf[lvi + 1 - 3] << 16) |
				(lvbuf[lvi + 1 - 2] << 8) | (lvbuf[lvi + 1 - 1]);

			if(lvcrc == Get_CRC32(lvbuf, lvi + 1 - 4))
				return lvi - 4;

			return -4;		//crc32 error

		}
/*		if(lvi == buflen + 4){
			lvi = (lvbuf[buflen + 1] << 24) | (lvbuf[buflen + 2] << 16) |
				(lvbuf[buflen + 3] << 8) | lvbuf[buflen + 4];
			if(lvi == Get_CRC32(lvbuf, buflen + 1)) return buflen;
			return -4;				//crc32 error
		}
		if(lvi > 8) return lvi - 8;
		return 0;
		*/
	}

    len = uart_read_data(DL_PORT, lvbuf + 1, 3, 200);
    //printf("len=%d\r\n",len);
	//if(uart_read_data(DL_PORT, lvbuf + 1, 3, 200) != 3)
	if(len != 3)
	{
		return -4;
	}
	if(lvbuf[1] != QS_DL_ATTR){
     //printf("b[1]=%d\r\n",lvbuf[1]);
		return -5;
	}
	lvlen = (lvbuf[2] << 8) + lvbuf[3];
	if(uart_read_data(DL_PORT, lvbuf + 4, lvlen, 500) != lvlen){
		return -6;
	}
	if(uart_read_data(DL_PORT, lvbuf + lvlen + 4, 4, 100) != 4){
		return -7;
	}
	//watch_dog_reset();
	crc = (lvbuf[lvlen + 4] << 24) | (lvbuf[lvlen + 5] << 16) | 
		(lvbuf[lvlen + 6] << 8) | (lvbuf[lvlen + 7]);
	if(crc != Get_CRC32(lvbuf, lvlen + 4)){
     
     //printf("crc = %x\r\n", Get_CRC32(lvbuf, lvlen + 4));
     //printf("crcget = %x\r\n", crc);
		return -4;
	}
	lvbuf[lvlen + 4] = 0;
	if(proc_cmd) return _s_cmd_ansyies((char *)(lvbuf + 4), lvlen);
	else return lvlen;
}

int flash_write(void *data, int len, unsigned int flash_addr)//(unsigned int dest, unsigned char *mem, unsigned int len)
{
    unsigned char *p;
    int  datalen;
    unsigned int addr;


    datalen = len;

    p = (unsigned char*)data;
    addr = (unsigned int)p;

 //   printf("flashaddr=%x\r\n",flash_addr);
//    printf("datalen=%x\r\n",datalen);
    jz_nor_write(flash_addr, datalen, addr);  
  
    return 0;
}


int download_uart(int port)
{
	int lvi;
	char lvbuf[4200];
	char lvDnData[1024 * 1024 * 3];
	int lvdatlen = 0;

	DL_PORT = port;
	dninfo.zone_is_set = 0;

	while(1)
	{
		lvi = _s_frame_proc(lvbuf, 1, 256);
             //printf("lvi1=%d\r\n", lvi);
		if(lvi == -1) return -1;			//receive data error
		else if(lvi == -2) return -2;		//receive cancel command
		else if(lvi == -11) break;			//receive Upgrade_Start cmd
	}

	while (lvdatlen < dninfo.datalen)
	{
		lvi = _s_frame_proc(lvbuf, 0, dninfo.framesize);
      //printf("lvi2=%d\r\n", lvi);  
		if(lvi == -1) return -1;			//receive timeout
		if(lvi <= 0){
			if(lvi == -4) uart_write_char(DL_PORT, '!');		//send err, retry send
			continue;
		}
		if(lvi == 5){
			if(_s_cmd_ansyies(lvbuf + 4, lvi) == -2) return -2;		//receive cancel command
		}
//		int_to_str(lvstr, lvi);
//		dbg_print(lvstr);
//		dbg_msg("");
//		uart_send_data(COM2, lvbuf + 1, lvi);
		uart_write_char(DL_PORT, '*');
		memcpy(lvDnData + lvdatlen, lvbuf + 1, lvi);
		lvdatlen += lvi;
	}
	uart_write_char(DL_PORT, '*');	 						//start config
	if(flash_write( (unsigned char*)lvDnData, dninfo.datalen,dninfo.address))
		uart_write_char(DL_PORT, '!');		//download file size out of flash range
	else
		uart_write_char(DL_PORT, '^');								//end config
	lvi = _s_frame_proc(lvbuf, 1, 256);
	if(lvi == -2) return 0;								//download finish
	if(lvi == -12) return 1;							//next download
	printf("Download finish");
	return 0;
}

    



