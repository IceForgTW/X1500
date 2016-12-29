
#include <MemMgr.h>
#include <time-tcu.h>

#include <math.h>
#include <stdlib.h>
#include "sys_dnld.h"
#include "../misc/generate.h"

#include "../watchdog/watchdog.h"
#include <list.h>

#include <os_api.h>
#include <string.h>
/*
typedef struct mem_addr {
  unsigned long addr;
} mem_addr_t ;*/
#define malloc(x) alloc(x)
#define free(x) deAlloc(x)

DN_OPS * sys_dn_get_port_ops(int port_type);
DOWNLOAD_PARAM *sys_dn_get_def_pdp(void);
int dnld_bsp_set_zone(char *zonestr, int len);
int dnld_bsp_update_flash(void *data, int len, unsigned int flash_addr);

#define QS_DL_STX			0x02	//���ı�־
#define QS_DL_ATTR			0x05	//��������
//����ṹ
/*
@  ---   ״̬�л�
!  ---   �����趨
?  ---   ��ѯ��Ϣ
0/1  ---   Yes/No����Ϣ
*/
#define QS_DL_SET			'!'		//1Byte��������������
#define QS_DL_SET_Q			'?'		//1Byte��������ѯ����
#define QS_DL_Comm_Switch   '@'    	//1Byte�����ܲ����л�
#define QS_DL_SET_SPEC		'#'    	//1Byte����������
#define QS_DL_FirmWare_Sign '>'     //1Byte, �������ݱ�־
#define QS_DL_ECHO_SUCCESS  '0'     //1Byte, �趨�ɹ��ظ�
#define QS_DL_ECHO_FAIL		'1'     //1Byte, �趨ʧ�ܻظ�


// Function setting 
#define  QS3_EXIT_Upgrade  "@Exit"    //�˳���ǰ���� 

#define  QS3_NEXT_Upgrade	"@NextDown"		//�ȴ���һ������

#define  QS3_Upgrade_FirmWare_Step ">Start"  //��ʼ��������

#define  QS3_Setting_Comm      "#COMM"   //�趨COMM

// Param Setting
#define  QS3_Setting_DataLens "!DataLens"   //�趨�����ܳ���
#define  QS3_Setting_Frames   "!Frames"     //�趨��֡��
#define  QS3_Setting_Framesize "!FrameSize" //�趨֡��С
#define  QS3_Setting_Address  "!Address"    //�趨���ص�ַ
#define  QS3_Setting_FileName  "!FileName"  //�趨�ļ�����
#define  QS3_Setting_FileType  "!FileType"  //�趨�ļ�����
#define  QS3_Setting_Specific	"!BootDownload"


// Param Query
#define  QS3_Query_BootVer   "?BootVer"     //��ѯBoot�汾
#define  QS3_Query_HardVer  "?HardVer"      //��ѯӲ�����ͼ���汾
#define  QS3_Query_OSVer     "?OSVer"       //��ѯOS�汾

#define  QS3_Query_NULL      "NULL"         //û�в�ѯ���κ�����ʱ�Ļ�Ӧ����

#define SendCmdFrame(a)		send_dnld_frame((a), sizeof(a))
	
typedef struct _dn_info_struct_{
	int datalen;
	int frames;
	int framesize;
	int address;
	int filetype;
	int zone_is_set;
}DN_INFO;

static DN_OPS *dnport = NULL;
static DN_INFO dninfo;

//////////////////////////////////////////////////////////////////////////

int str_to_int(char *instr)
{
	int lvret = 0;
	while((*instr >= '0') && (*instr <= '9')){
		lvret = lvret * 10 + *instr - '0';
		instr ++;
	}
	return lvret;
}

void int_to_str(char *buf, int nval)
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
}


int send_dnld_frame(const void *data, int len)
{
	unsigned char lvbuf[256];
	int lvcrc;
	if(len > 240 || len < 1) return -1;
	lvbuf[0] = QS_DL_STX;
	lvbuf[1] = QS_DL_ATTR;
	lvbuf[2] = (len >> 8) & 0xff;
	lvbuf[3] = len & 0xff;
	memcpy(lvbuf + 4, data, len);
	lvcrc = Get_CRC32(lvbuf, len + 4);
	lvbuf[len + 4] = lvcrc >> 24;
	lvbuf[len + 5] = lvcrc >> 16;
	lvbuf[len + 6] = lvcrc >> 8;
	lvbuf[len + 7] = (lvcrc & 0xff);
	dnport->write(lvbuf, len + 8, TIMEOUT_NOWAIT);

	return len;
}


#define DN_FRAME_FIZE_MAX	8200
int _s_cmd_ansyies(char *cmd, int len)
{
    int cmdlen;
    int vallen = 0;
    char *lvp;


    lvp = strchr(cmd, ':');
    if(!lvp) cmdlen = len;
    else 
    {
        cmdlen = (int)((unsigned int)lvp - (unsigned int)cmd);
        vallen = len - cmdlen - 1;
        lvp ++;
    }
    
    cmd[cmdlen] = 0;

    
    if(!strcmp(cmd, QS3_Setting_Comm)){
		TComParam param;
		if(!lvp) return -1;

		param.lRate = str_to_int(lvp);
		lvp = strchr(lvp, ',') + 1;
		param.lDataBits = str_to_int(lvp);
		lvp = strchr(lvp, ',') + 1;
		param.lParity = str_to_int(lvp);
		lvp = strchr(lvp, ',') + 1;
		param.lStopBits = str_to_int(lvp);  
		param.nflowCtrl = 0;

		SendCmdFrame("0");

		dnport->set_port_param(&param);
		mdelay(2);

		vallen = dnport->read(cmd, 1, 10000);
		dnport->write(cmd, 1, TIMEOUT_NOWAIT);

	} else if(!strcmp(cmd, QS3_Query_BootVer)){
		SendCmdFrame("V3.1");
	} else if(!strcmp(cmd, QS3_Query_HardVer)){
		SendCmdFrame("EM300-V5");
	} else if(!strcmp(cmd, QS3_Query_OSVer)){		//maybe not need
            
        SendCmdFrame("uBase-III");
	} else if(!strcmp(cmd, QS3_Setting_FileType)){
		int lvi = dnld_bsp_set_zone(lvp, vallen);
		if( lvi == -1) SendCmdFrame("1");
		else {
			SendCmdFrame("0");
			dninfo.address = lvi;
		}
	} else if(!strcmp(cmd, QS3_Setting_DataLens)){
		dninfo.datalen = str_to_int(lvp);
		SendCmdFrame("0");
	} else if(!strcmp(cmd, QS3_Setting_Frames)){		//maybe need modify
		dninfo.frames = str_to_int(lvp);
		SendCmdFrame("0");
	} else if(!strcmp(cmd, QS3_Setting_Framesize)){		//maybe need modify
		dninfo.framesize = str_to_int(lvp);
		if(dninfo.framesize > DN_FRAME_FIZE_MAX || dninfo.framesize < 14){
			dninfo.framesize = 0;
			SendCmdFrame("1");
		} else SendCmdFrame("0");
	} else if(!strcmp(cmd, QS3_Setting_FileName)){		//maybe not need
		SendCmdFrame("0");
	} else if(!strcmp(cmd, QS3_Setting_Specific)){		//maybe not need
		SendCmdFrame("0");
	} else if(!strcmp(cmd, QS3_Setting_Address)){		//maybe not need
		if(dninfo.zone_is_set){							//zone is set
			SendCmdFrame("1");
		}else {
			dninfo.address = str_to_int(lvp);
			SendCmdFrame("0");
		}
	} else if(!strcmp(cmd, QS3_Upgrade_FirmWare_Step)){
		SendCmdFrame("0");
		return 3;
	} else if(!strcmp(cmd, QS3_EXIT_Upgrade)){
		SendCmdFrame("0");
		return 2;
	} else if(!strcmp(cmd, QS3_NEXT_Upgrade)){
		SendCmdFrame("0");
		return 1;
	} else {		//not support cmd
		return dnport->proc_not_comm_cmd(cmd, cmdlen, lvp, vallen);
		//return -1;
	}
	return 0;		
}

static int _s_receive_frame(void *buf, int len, int ansyies_flag, int timeout)
{
	unsigned char *lvbuf = (unsigned char*)buf;
	int lvlen;
	unsigned int lvi;

wait_dl_stx:

	*lvbuf = 0x00;
	while(1){
		ResetWatchDog();
		if(dnport->read(lvbuf, 1, 3000) <= 0){
//			dbg("frame receive timeout\r\n");
			return -1;		//timeout
		}
		if(*lvbuf == QS_DL_STX) break;
	}
	if(!ansyies_flag){
		ResetWatchDog();
		lvlen = dnport->read(lvbuf + 1, len + 4, len + 1000);

		if(lvlen > 4)
		{
			lvi = (lvbuf[lvlen + 1 - 4] << 24) | (lvbuf[lvlen + 1 - 3] << 16) |
				(lvbuf[lvlen + 1 - 2] << 8) | lvbuf[lvlen + 1 - 1];
			if(lvi == Get_CRC32(lvbuf, lvlen + 1 - 4))
				return lvlen - 4;						//return size not include stx char
			return -2;
		}
		/*
		if(lvlen == len + 4){
			lvi = (lvbuf[len + 1] << 24) | (lvbuf[len + 2] << 16) |
				(lvbuf[len + 3] << 8) | lvbuf[len + 4];
			if(lvi == Get_CRC32(lvbuf, len + 1)) return len;
//			sprintf(lvstr, "frame crc err, rec len:%d, crc:%.8x clac:%.8x\r\n",
//				lvlen + 1, lvi, Get_CRC32(lvbuf, len + 1));
//			dbg(lvstr);
			//UartWrite(COM2, lvbuf + 1, 16, TIMEOUT_NOWAIT);
//			UartWrite(COM2, lvbuf + len - 11, 16, TIMEOUT_NOWAIT);
			return -2;											//crc check error
		}
		if(lvlen > 4){
			lvi = (lvbuf[lvlen - 4] << 24) | (lvbuf[lvlen - 3] << 16) |
				(lvbuf[lvlen - 2] << 8) | lvbuf[lvlen - 1];
			if(lvi == Get_CRC32(lvbuf, lvlen + 1 - 4)){
				memcpy(lvbuf + 1, lvbuf + 4, lvlen - 7);
				lvbuf[lvlen - 6] = 0;
				return lvlen - 7;
			} else return -2;
		}
//		dbg("invalid data\r\n");
		*/
		goto wait_dl_stx;
	}

	if(dnport->read(lvbuf + 1, 3, 100) != 3) goto wait_dl_stx;
	if(lvbuf[1] != QS_DL_ATTR) goto wait_dl_stx;
	lvlen = ((lvbuf[2] << 8) | lvbuf[3]) + 4;

	ResetWatchDog();
	if(dnport->read(lvbuf + 4, lvlen, 500) != lvlen) goto wait_dl_stx;
    //printf("crc1=%x,crc2=%x,crc3=%x,crc4=%x\r\n",lvbuf[lvlen],lvbuf[lvlen + 1],lvbuf[lvlen + 2],lvbuf[lvlen + 3]);
	lvi = (lvbuf[lvlen] << 24) | (lvbuf[lvlen + 1] << 16) |
		(lvbuf[lvlen + 2] << 8) | (lvbuf[lvlen + 3]);
	if(lvi == Get_CRC32(lvbuf, lvlen)){
		lvbuf[lvlen] = 0;
		return lvlen - 4;
	}
	goto wait_dl_stx;
	return 0;
}

static int chk_dninfo(void)
{
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//if return not zero, the entry download function
//if return zero, the boot kernel and application
static int _s_check_download(void)
{
	int lvi;
	int lvcnt = 0;
	int lvflag = 0;
	char lvc;

//	uart_clear_buf(port);
	while(lvcnt < 10){
		ResetWatchDog();
		lvi = dnport->read(&lvc, 1, 500);
		if(lvi <= 0){
			return 0;
		}
		if((lvc != '?') && !lvflag){
			lvcnt ++;
			continue;		//ϵͳ����ʱ���ܻ���յ�һЩ�����ַ�
		}
		if(lvc != '?'){
			return 0;					//�յ������ַ�,˵���������س���
		}
		if(++lvflag >= 3){
			dnport->write("<", 1, TIMEOUT_NOWAIT);
			return 1;					//�����յ������ַ��Ž�������
		}
	}
	return 0;
}


int kernel_download(DOWNLOAD_PARAM *pdp, int steps, int protocol, int wait_timeout)
{
    int lvopen_flag = 0;
    int lvi;
    int lverr;

    char lvbuf[DN_FRAME_FIZE_MAX + 100];

    if(!pdp) pdp = sys_dn_get_def_pdp();
    dnport = sys_dn_get_port_ops(pdp->PortType);
    if(!dnport) return -1;							//not found port support function

    lvi = dnport->open(pdp->PortIdx);
    if(!lvi) lvopen_flag = 1;
    else if(pdp->chk_open) return -10;		
    
down1:    	
    
    dnport->set_port_param(pdp->port_init_param);
    
    if(steps == 0)
    {
        unsigned int lvtick = os_TimeGet();
        if(wait_timeout == TIMEOUT_INFINITE)
        {
            while(!_s_check_download());
        } 
        else 
        {
            do 
            {
                lvi = _s_check_download();//�յ�3���ʺţ�������ʼ���³��򣬻�дһ��С�ں�
                if(lvi || wait_timeout == TIMEOUT_NOWAIT) break;
            }while((os_TimeGet() - lvtick) < wait_timeout);

            if(!lvi) return -2;
        }						//not check download command
    }
      
    memset(&dninfo, 0, sizeof(dninfo));
    
down2:
    if(steps <= 1)
    {
        while(1)
        {
            lvi = _s_receive_frame(lvbuf, 256, 1, TIMEOUT_INFINITE);//������������:02 05 lenh lenl data crchh crch crcl crcll
            if(lvi == -1) return -3;				//timeout
            if(lvi < 1) continue;
            lvi = _s_cmd_ansyies(lvbuf + 4, lvi);//�������
            if(lvi == 1)	
                goto down1; //continue;				//next download
            if(lvi == 2)	
            {
				ResetSystemByWD(); 
                return -3;				//host break download
            }
            if(lvi == 3)
            {
               if(!chk_dninfo()) break;					//receive start download
               break;//����whileѭ������ʼ����
            }
        }
    }

    if(steps <= 2)
    {
        char *lvp;
        int  lvdatlen = 0;
        
        lvp = (char*)malloc(dninfo.framesize * (dninfo.frames + 1));
        if(!lvp) return -20;						//not enough memory
	  lverr = 0;
		
        while(lvdatlen < dninfo.datalen)
        {                  
            lvi = _s_receive_frame(lvbuf, dninfo.framesize, 0, 1500);
			
            if(lvi <= 0)
            {
                if(lvi != -1)
                    dnport->write("!", 1, TIMEOUT_NOWAIT);
                    //WriteChar('!');//���ճ���Ӧ��!
                lverr ++;
                if(lverr > 15)
                {
                    //diag_printf("too more error! exit kernel download function.\r\n");
                    free((MEM_ULONG)lvp);
                    return -100;
                }
                continue;
            }
            lverr = 0;
            if((lvi == sizeof(QS3_EXIT_Upgrade) || lvi == strlen(QS3_EXIT_Upgrade))&& _s_cmd_ansyies(lvbuf + 1, lvi) == 2)
            {
                free((MEM_ULONG)lvp);
                return -30;					//host abort
            }
            dnport->write("*", 1, TIMEOUT_NOWAIT);
            memcpy(lvp + lvdatlen, lvbuf + 1, lvi);                   
            lvdatlen += lvi; 
        }
		dnport->write("*", 1, TIMEOUT_NOWAIT);
        DisableWatchDog();
 		if(dnld_bsp_update_flash(lvp, dninfo.datalen, dninfo.address)){
			dnport->write("!", 1, TIMEOUT_NOWAIT);
		} else
		{
			dnport->write("^", 1, TIMEOUT_NOWAIT);
		}
    EnableWatchDog();
        free((MEM_ULONG)lvp);
	  goto down2;
	}
	
	ResetSystemByWD();

	return 0;
}


/*********************************************************************
			End of file
*********************************************************************/
