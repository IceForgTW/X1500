/********************************************************************
	created:	2008/03/17
	filename: 	kbw_ps2.h
	file path:	G:\Working\2027_Old\bsp\MB2027\src\devs\kbw_ps2
	author:		Zencg
	
	purpose:	
*********************************************************************/

#ifndef _HEAD_FILE_kbw_ps2_
#define _HEAD_FILE_kbw_ps2_

#define PS2_PORT_DEV_CNTL_CMD_WRITE			1
#define PS2_PORT_DEV_CNTL_CMD_RAW_WRITE		2
#define PS2_PORT_DEV_CNTL_CMD_SEND_STR		3

#ifdef __cplusplus
extern "C"{
#endif
////////////////////////////////////////////////////////

int ps2_raw_send_char(const char chr);
int init_ps2_kbw_io(void);
int StartPS2KbwSend(void);
int StopPS2KbwSend(void);

////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#endif		//#ifndef _HEAD_FILE_kbw_ps2_
/*********************************************************************
			End of file
*********************************************************************/
