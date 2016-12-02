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
File        : mmc_api.h
Purpose     : Define global functions and types to be used by an
              application using the mmc/sd card.

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

#ifndef __VMNAND_API__
#define __VMNANDC_API__

#include "mmc_core.h"

typedef struct {
	unsigned int	dwBlockSize;
	unsigned int	dwSectorsPerBlock;
	unsigned int	dwSectorSize;
	unsigned int	dwFSTotalBlocks;
	unsigned int	dwFSTotalSectors;
} FS_MMC_info,*PFS_MMC_info;

typedef struct {
	unsigned char	*dev_name;		/*�豸����*/
	unsigned int 	boot_card;		/*�Ƿ�Ϊϵͳ��*/
	unsigned int  	hotplug_en;		/*�Ƿ������Ȳ��*/
	unsigned int  	bus_width;		/*�߿�����*/
	struct card_info mmc_info;		/*����Ϣ*/
} MMC_HOST,*P_MMC_HOST;

#endif /* __VMNAND_API__ */
