/*
**********************************************************************
*
*                            uC/MMC
*
*             (c) Copyright 2005 - 2007, Ingenic Semiconductor, Inc
*                      All rights reserved.
*
***********************************************************************

----------------------------------------------------------------------
File        : mmc_core.h 
Purpose     : MMC core definitions.

----------------------------------------------------------------------
Version-Date-----Author-Explanation
----------------------------------------------------------------------
1.00.00 20060831 WeiJianli     First release

----------------------------------------------------------------------
Known problems or limitations with current version
----------------------------------------------------------------------
(none)
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __MMC_CORE__
#define __MMC_CORE__

#include "mmc_protocol.h"

#define ID_TO_RCA(x) ((x)+1)

#define MMC_BLOCKSIZE 		512		/* MMC/SD Block Size */

/*  卡命令对应MSC_CMDAT寄存器相关设置 */
#define	 MSC_NOT_CARE 		(1<<0)		/* none msc func setting*/
#define	 MSC_RESET_INIT 	(1<<1)		/* add 80  setting cloks */
#define	 MSC_READ			(1<<2)		/* msc read setting  */
#define	 MSC_WRITE			(1<<3)		/* msc write setting */
#define	 MSC_BUS_WIDTH		(1<<4)		/* msc should set bus width  */

//--------------------------------------------------------------------------
// IRQ Number descriptions(can used for MSC_IMASK,MSC_IREG)
//--------------------------------------------------------------------------
#define MSC_DATA_TRAN_DONE		( 1 << 0 )
#define MSC_PRG_DONE			( 1 << 1 )
#define MSC_END_CMD_RES			( 1 << 2 )
#define MSC_RXFIFO_RD_REQ		( 1 << 5 )
#define MSC_TXFIFO_WR_REQ		( 1 << 6 )
#define MSC_SDIO				( 1 << 7 )
#define MSC_TIME_OUT_READ		( 1 << 8 )
#define MSC_TIME_OUT_RES		( 1 << 9 )
#define MSC_CRC_WRITE_ERR		( 1 << 10 )
#define MSC_CRC_READ_ERR		( 1 << 11 )
#define MSC_CRC_RES_ERR			( 1 << 12 )
#define MSC_DATA_FIFO_EMP		( 1 << 13 )
#define MSC_DATA_FIFO_FULL		( 1 << 14 )
#define MSC_AUTO_CMD_DONE		( 1 << 15 )

/* Error codes */
enum mmc_result_t {
	MMC_NO_RESPONSE        = -1,
	MMC_NO_ERROR           = 0,
	MMC_ERROR_OUT_OF_RANGE,
	MMC_ERROR_ADDRESS,
	MMC_ERROR_BLOCK_LEN,
	MMC_ERROR_ERASE_SEQ,
	MMC_ERROR_ERASE_PARAM,		//5
	MMC_ERROR_WP_VIOLATION,
	MMC_ERROR_CARD_IS_LOCKED,
	MMC_ERROR_LOCK_UNLOCK_FAILED,
	MMC_ERROR_COM_CRC,
	MMC_ERROR_ILLEGAL_COMMAND,	//10
	MMC_ERROR_CARD_ECC_FAILED,
	MMC_ERROR_CC,
	MMC_ERROR_GENERAL,
	MMC_ERROR_UNDERRUN,
	MMC_ERROR_OVERRUN,		//15
	MMC_ERROR_CID_CSD_OVERWRITE,
	MMC_ERROR_STATE_MISMATCH,
	MMC_ERROR_HEADER_MISMATCH,
	MMC_ERROR_TIMEOUT,
	MMC_ERROR_CRC,			//20
	MMC_ERROR_DRIVER_FAILURE,
	MMC_ERROR_CARD_OUT,
};


enum card_use_bus_width_t{
	MMC_8BIT_MODE = 0,	
	MMC_4BIT_MODE,
	MMC_1BIT_MODE
};

enum msc_channel_t {
	MSC_CHANNEL_0 = 0,  /*MSC0*/
	MSC_CHANNEL_1		/*MSC1*/
};

enum card_speed_mode_t {
	SLOW_SPEED_MODEL = 0,//init card period
	FAST_SPEED_MODEL,
	HIGH_SPEED_MODEL
};

enum card_type_t {
 	SD_CARD = 0,
 	MMC_CARD
};

enum card_status{
	CARD_OUT = 0, 
	CARD_IN

};

/* card operate to MSC  */
struct card_operate 
{
	unsigned int    cmd_index;      /* Command  sended */
	unsigned int	msc_ch;			/* 0-MSC0 1-MSC1 */
	unsigned int    bus_width; 		/* bus width used	*/
	unsigned int    use_dam;		  /* 1:使用DMA方式 0:使用CPU方式 */
	/* Results */
	unsigned char   resp[18]; 	/* Buffer to store response - CRC is optional */
	unsigned int 	err;		
};


/* the information structure of MMC/SD Card */
struct card_info
{
	unsigned int 	sd_20;	/* 0-stand 1-high */
	unsigned int    id;     /* Card index */
	unsigned int    type;   /* 0-MMC or 1-SD card */
	unsigned int   	rca;    /* RCA */
	unsigned int    scr;    /* SCR 63:32*/ 
	unsigned int    block_num;
	unsigned int    block_len;
	unsigned int    erase_unit;       
	unsigned int    flags;  /* Ejected, inserted */

	/* Card specific information */
	struct mmc_cid  cid;
	struct mmc_csd  csd;
	struct mmc_ext_csd ext_csd;
	struct card_operate operate;
	enum card_state state;  /* empty, ident, ready, whatever */
	enum mmc_result_t result;
};


#endif  /* __MMC_CORE__ */
