#ifndef __MSCBOOT_H__
#define __MSCBOOT_H__

#define CFG_NAND_MINIOS_OFFS   	 8 * 1024 /*address offset*/
#define CFG_NAND_MINIOS_SIZE   	 256 * 1024 /*address size */
#define CFG_NAND_MINIOS_START	(0x80800000)
#define CFG_NAND_MINIOS_DST		(0x80800000)



#define  CONFIG_DDR2_DIFFERENTIAL  1
#define CONFIG_DDR2_DRV_CK_CS_FULL 1
#define CONFIG_SDRAM_DDR2 1

#endif 

