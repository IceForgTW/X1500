



#define NAND_DATAPORT	0xb8000000
#define NAND_ADDRPORT	0xb8010000
#define NAND_COMMPORT	0xb8008000

#define INREG8(x)           ( (unsigned char)(*(volatile unsigned char * const)(x)) )
#define OUTREG8(x, y)       *(volatile unsigned char * const)(x) = (y)
#define SETREG8(x, y)       OUTREG8(x, INREG8(x)|(y))
#define CLRREG8(x, y)       OUTREG8(x, INREG8(x)&~(y))


#define __nand_cmd(n)		OUTREG8(NAND_COMMPORT, (n))
#define __nand_addr(n)		OUTREG8(NAND_ADDRPORT, (n))
#define __nand_data8()		INREG8(NAND_DATAPORT)
#define __nand_data16()		INREG16(NAND_DATAPORT)

#define NAND_CMD_READ1_00                 0x00	//  Read0
#define NAND_CMD_READ1_01                 0x01	//  Read1
#define NAND_CMD_READ2                    0x50	//  Read2
#define NAND_CMD_READ_ID1                 0x90	//  ReadID1
#define NAND_CMD_READ_ID2                 0x91	//	ReadID2
#define NAND_CMD_RESET                    0xFF	//  Reset

#define NAND_CMD_PAGE_PROGRAM_START       0x80	//  Write phase 1
#define P2_1_CMD_PAGE_PROGRAM_STOP        0x11

#define P2_2_CMD_PAGE_PROGRAM_START	      0x81
#define NAND_CMD_PAGE_PROGRAM_STOP        0x10

#define NAND_CMD_PAGE_RANDOM_PROGRAM_START 0x85
#define	NAND_CMD_RANDOM_READ				0x05
#define	NAND_CMD_RANDOM_READ_CONFIRM		0xE0

#define NAND_CMD_BLOCK_ERASE_START        0x60	//	Erase block
#define NAND_CMD_BLOCK_ERASE_CONFIRM      0xD0	//	Erase block confirm
#define NAND_CMD_READ_STATUS              0x70	//  Read Status
#define NAND_CMD_READ_STATUS_MULTI		  0x71	//	Read multi status

/* Extended commands for large page devices */
#define NAND_CMD_READSTART				0x30
#define NAND_CMD_CACHEDPROG				0x15

static inline void __nand_dev_ready(void)
{
	unsigned int timeout = 100;
	while ((INREG32(A_GPIO_PXPIN(2)) & 0x8000000) && timeout--);
	while (!(INREG32(A_GPIO_PXPIN(2)) & 0x8000000));
}

static inline void nand_read_buf8(void *buf, int count)
{
	int i;
	unsigned char *p = (unsigned char *)buf;
	for (i = 0; i < count; i++)
		*p++ =  __nand_data8();
}

static inline void nand_write_buf8(void *buf, int count)
{
	int i;
	unsigned char *p = (unsigned char *)buf;
	for (i = 0; i < count; i++)
		*p++ =  __nand_data8();
}


