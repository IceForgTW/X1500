/********************************************************************
	filename: 	dnld_bsp.c
*********************************************************************/
#include <uartmc.h>
#include <usb.h>
#include <nor_flash.h>

#include "sys_dnld.h"
#include <list.h>
#include <os_api.h>
#include "x1000.h"

#include <string.h>
//////////////////////////////////////////////////////////////////////////
#define USB_DEV_STATUS_LINK		1
#define USB_DEV_STATUS_UNLINK	0
#define USB_DEV_STATUS_ERROR	-1
extern int send_dnld_frame(const void *data, int len);
extern int str_to_int(char *instr);


int ser_read(void *buf, int len, int timeout)
{
	return comm_read(buf, len, timeout);
	
}

int ser_write(void *buf, int len, int timeout)
{
	return comm_write(buf, len);
}

int ser_open(int idx)
{
	return 1;
}

int ser_set_param(void *param)
{
	uart_set_parameter(param);

	return 1;
}

int ser_close(int idx)
{
	/*USleep(10);
	return UartClose(COM1);*/
	return 0;
}

#define  QS3_Setting_Comm      "#COMM"   //设定COMM

int ser_proc_not_comm_cmd(void *cmd, int cmdlen, void *param, int paramlen)
{
	char *lvcmd = (char*)cmd;
	char *lvp = (char*)param;

	if(!strcmp(lvcmd, QS3_Setting_Comm)){
		int nbaudrate;
		int bits;
		int parity;
		int stopbits;
		TComParam lvup;

		send_dnld_frame("0", 1);
		if(!lvp) return -1;
		nbaudrate = str_to_int(lvp);
		lvp = strchr(lvp, ',') + 1;
		bits = str_to_int(lvp);
		lvp = strchr(lvp, ',') + 1;
		parity = str_to_int(lvp);
		lvp = strchr(lvp, ',') + 1;
		stopbits = str_to_int(lvp);
		uart_get_parameter(&lvup);
		lvup.lRate = nbaudrate;


		uart_set_parameter(&lvup);
//		delay(2);

		comm_read(lvp, 1, 2000);
		comm_write(lvp, 1);
		return 0;
	}
	return -1;		//can't support command		
}

//////////////////////////////////////////////////////////////////////////

int dn_udp_read(void *buf, int len, int timeout)
{
	return udp_read(buf, len, timeout);
}

int dn_udp_write(void *buf, int len, int timeout)
{
	return udp_write(buf, len);
}
int dn_udp_set_param(void *param)
{
	return 0;
}

int dn_udp_open(int idx)
{
	int lvi, lvtick;	
	lvi = udp_open(idx);
// 	if(!lvi) return lvi;
// 	USleep(1500);

	if (lvi<0)	return lvi;
	lvtick = os_TimeGet();
	do 
	{
		if (udp_active())
			return lvi;
		os_TimeDelay(1);
	} while((os_TimeGet() - lvtick) < 150);
	return -10;
}

static int dn_udp_close(int idx)
{
	return udp_close();
}

static int dn_udp_proc_not_comm_cmd(void *cmd, int cmdlen, void *param, int paramlen)
{
	return -1;
}

//////////////////////////////////////////////////////////////////////////
static DN_OPS _s_dnport_ops[2] = {
	{
		ser_open,
		ser_close,
		ser_set_param,
		ser_proc_not_comm_cmd,
		ser_read,
		ser_write,
	},
	{
		dn_udp_open,
		dn_udp_close,
		dn_udp_set_param,
		dn_udp_proc_not_comm_cmd,
		dn_udp_read,
		dn_udp_write,
	}
};

DN_OPS * sys_dn_get_port_ops(int port_type)
{
	if(port_type < 0 || port_type >= (sizeof(_s_dnport_ops) / sizeof(DN_OPS)))
		return NULL;
	return &_s_dnport_ops[port_type];
}

//////////////////////////////////////////////////////////////////////////

TComParam _s_up = {
	9600,		//baudrate
	8,			//bits
	1,	//stop bits
	0,	//parity
	4,
	0,

};

DOWNLOAD_PARAM _s_def_dp = {
	SYS_DN_PORT_RS232,		//	int PortType;			//通讯口类型
	1,					//	int PortIdx;			//通讯口索引号
	SYS_DN_SAVE_SPACE_AUTO,	//	int SaveSpace;			//保存在Flash或是文件系统上
	0,						//	int chk_open;			//open设备失败时是否退出
	&_s_up					//	void *port_init_param;	//通讯口初始化参数
};

DOWNLOAD_PARAM *sys_dn_get_def_pdp(void)
{
	return &_s_def_dp;
}

int _s_dn_cntl(int id, int cmd, int ofs)
{
	return 0;
}

static int upgradeType = -1;

int dnld_bsp_get_type(void)
{
	return upgradeType;
}


int dnld_bsp_set_zone(char *zonestr, int len)
{
    int lvret = -1;
    char *lvp;
    
    lvp = strchr(zonestr, ':');
    if(lvp)	*lvp++ = 0;
	
    if(!strcmp(zonestr, DN_ZONE_KERNEL))
    {
        lvret = 0x80000;		//block:65
		upgradeType = 2;
    }
    else if(!strcmp(zonestr, DN_ZONE_APP))
    {
        lvret = 0x80000;		//block:65
    }
    else if(!strcmp(zonestr, DN_ZONE_BOOTROM))
    {
        lvret = 0;
		upgradeType = 1;
	}
    else if(!strcmp(zonestr, DN_ZONE_FACTORYINFO))
        lvret = 0x00D00000;		//block:104
    else if(!strcmp(zonestr, DN_ZONE_FLASH)){
		if(!lvp) return -1;
		lvret = str_to_int(lvp) * 128 *1024;}
	
    return lvret;
}



int dnld_bsp_update_flash(void *data, int len, unsigned int flash_addr)
{
    unsigned char *p;
    int datalen;
    unsigned int addr;

    datalen = len;

    p = (unsigned char*)data;
    addr = (unsigned int)p;
    
    jz_nor_write(flash_addr, datalen, addr);  
  
    return 0;
}

/*******************************************************************
			End of file
*********************************************************************/
