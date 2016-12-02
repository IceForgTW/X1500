/********************** BEGIN LICENSE BLOCK ************************************
 *
 * JZ4740  mobile_tv  Project  V1.0.0
 * INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Copyright (c) Ingenic Semiconductor Co. Ltd 2005. All rights reserved.
 *
 * This file, and the files included with this file, is distributed and made
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 *
 * http://www.ingenic.cn
 *
 ********************** END LICENSE BLOCK **************************************
 *
 *  Author:  <dsqiu@ingenic.cn>  <jgao@ingenic.cn>
 *
 *  Create:   2008-06-26, by dsqiu
 *
 *  Maintain: 2008-06-26, by jgao
 *
 *
 *******************************************************************************
 */

#include <mipsregs.h>
#include <excpt.h>
#include <os_api.h>
#include <x1000.h>

#include "MemMgr.h"
#undef printf
extern void init_command(void);
extern int request_command(void (*handler)(char argc,char **argv),char *name);
extern int printf (__const char *__restrict fmt, ...);
extern void *memset(void* m, int c, int n);


const static char *regstr[] = {
	"$0  zero", "$1  at", "$2  v0", "$3  v1", "$4  a0", "$5  a1", "$6  a2", "$7  a3",
	"$8  t0", "$9  t1", "$10 t2", "$11 t3", "$12 t4", "$13 t5", "$14 t6", "$15 t7",
	"$16 s0", "$17 s1", "$18 s2", "$19 s3", "$20 s4", "$21 s5", "$22 s6", "$23 s7",
	"$24 t8", "$25 t9", "$26 k0", "$27 k1", "$28 gp", "$29 sp", "$30 fp", "$31 ra"
};

static const char *cause_strings[32] =
{
  /*  0 */ "Int",
  /*  1 */ "TLB Mods",
  /*  2 */ "TLB Load",
  /*  3 */ "TLB Store",
  /*  4 */ "Address Load",
  /*  5 */ "Address Store",
  /*  6 */ "Instruction Bus Error",
  /*  7 */ "Data Bus Error",
  /*  8 */ "Syscall",
  /*  9 */ "Breakpoint",
  /* 10 */ "Reserved Instruction",
  /* 11 */ "Coprocessor Unuseable",
  /* 12 */ "Overflow",
  /* 13 */ "Trap",
  /* 14 */ "Instruction Virtual Coherency Error",
  /* 15 */ "FP Exception",
  /* 16 */ "Reserved 16",
  /* 17 */ "Reserved 17",
  /* 18 */ "Reserved 18",
  /* 19 */ "Reserved 19",
  /* 20 */ "Reserved 20",
  /* 21 */ "Reserved 21",
  /* 22 */ "Reserved 22",
  /* 23 */ "Watch",
  /* 24 */ "Reserved 24",
  /* 25 */ "Reserved 25",
  /* 26 */ "Reserved 26",
  /* 27 */ "Reserved 27",
  /* 28 */ "Reserved 28",
  /* 29 */ "Reserved 29",
  /* 30 */ "Reserved 30",
  /* 31 */ "Data Virtual Coherency Error"
};

typedef struct
{
	unsigned int sp;
	unsigned int ra;
 	PFun_Exception_Modal_Handler except_handle;
}Except_Struct,*PExcept_Struct;
static PFun_Exception_Modal_Handler main_except_handle = 0;
#define malloc(x) alloc(x)
#define free(x) deAlloc(x)
static Except_Struct g_pExcept[10];
static unsigned int g_MaxExceptNum = 10;
static unsigned int g_CurExcept = 0;
unsigned long strtoul(const char *S, char **endptr,int base);

void ReadWord(char argc,char **argv)
{
	unsigned int addr;

	addr = strtoul(argv[0],0,0);

	if(argc > 0)
	{
		printf("\n");
		printf("0x%x:0x%x",addr,*(unsigned int *)addr);
	}
}

void WriteWord(char argc,char **argv)
{
	unsigned int addr = 0x82000000;

	unsigned int val;
	if(argc > 0)
	{
		addr = strtoul(argv[0], 0, 0);
		val = strtoul(argv[1], 0, 0);
		printf("\n");
		printf("pre wirte 0x%x:0x%x",addr,*(unsigned int *)addr);
		*(unsigned int *)addr = val;
		printf("after wirte 0x%x:0x%x",addr,*(unsigned int *)addr);
	}
}


void setmainexcpt(void *p)
{
    main_except_handle = p;
}
extern void shelltask();


int InitExcept(unsigned int x)
{
	//g_pExcept = (PExcept_Struct)malloc(x * sizeof(Except_Struct));
	//if(g_pExcept == 0)
	//	return 0;
	memset(g_pExcept,0,10 * sizeof(Except_Struct));
	g_MaxExceptNum = 10;
  g_CurExcept = 0;
#if RELEASE==0
  init_command();
  request_command(ReadWord,"ReadWord");
  request_command(WriteWord,"WriteWord");
#endif
	return 1;
}
void DeinitExcept()
{
	//if(g_pExcept)
	//	free(g_pExcept);
	memset(g_pExcept,0,10 * sizeof(Except_Struct));
}
int AddExcept(PFun_Exception_Modal_Handler pFun,unsigned int *sr)
{
	unsigned int save;
	int ret;
	save = spin_lock_irqsave();
	g_CurExcept = 0;
	for(ret = g_MaxExceptNum - 1;ret >= 0;ret--)
	{
		//printf("g_pExcept[%d].sp = %x\n",ret,g_pExcept[ret].sp);
		if(g_pExcept[ret].sp != 0)
		{
			g_CurExcept = ret + 1;
			break;
		}
	}
	//printf("g_CurExcept = %d\n",g_CurExcept);
	if(g_CurExcept >= g_MaxExceptNum)
	{
		 //printf("AddExcept error = %d\n",g_CurExcept);
		 spin_unlock_irqrestore(save);
		 return -1;
	}
	g_pExcept[g_CurExcept].ra = sr[0];
	g_pExcept[g_CurExcept].sp = sr[1];
	g_pExcept[g_CurExcept].except_handle = pFun;
	g_CurExcept++;

	ret = g_CurExcept;
	spin_unlock_irqrestore(save);
	
	//printf("register return fun = %x %d sp = %x ra = %x\n",pFun,g_CurExcept,sr[1],sr[0]);
	
	return ret;
}
void DecExcept(int d,unsigned int *sr)
{
	unsigned int save;
	save = spin_lock_irqsave();

//	printf("%s %d\n",__FUNCTION__,d);

    if(g_CurExcept <= 0) {
    	spin_unlock_irqrestore(save);
    	return ;
		}
    if(d > g_CurExcept) {
    	spin_unlock_irqrestore(save);
    	return;
    }
    d--;
    if(d < 0){
    	 spin_unlock_irqrestore(save);
    	 return;
    }
    sr[0] = g_pExcept[d].ra;
  	sr[1] = g_pExcept[d].sp;


    //memcpy(&(g_pExcept[d]),&(g_pExcept[d+1]),sizeof(Except_Struct) * (g_CurExcept - d - 1));
    //g_CurExcept--;
    g_pExcept[d].sp            = 0;
    g_pExcept[d].ra            = 0;
    g_pExcept[d].except_handle = 0;
    spin_unlock_irqrestore(save);
}


void c_except_handler(unsigned int *sp)
{
	static int read_epc_count = 0;
	static int epc_save = 0;
	int i;
	unsigned int epc;
	
	//如果 read_epc_count>0 说明 c_except_handler 在读 epc 时重入了，即读 epc 导致了一个新的异常
	if (read_epc_count > 0)
	{
		printf("ERROR: read epc fail when except handle\n");
		epc = epc_save;
		read_epc_count = 0;
	}
	else
	{
		read_epc_count++;
		epc_save = 0;
		epc = read_c0_epc();
		epc_save = epc;
		
		if(epc != 0)
		{
			printf("-----------------------------------------------------\n");
			for(i = 0;i < 4;i++)
			{
				printf("%08x:\t%08x\n",(epc - 4*4 + i*4),*(unsigned int *)((epc - 4 * 4 + i * 4)| 0xa0000000));
		  	}
		  	for(i = 0;i < 4;i++)
			{
				printf("%08x:\t%08x\n",(epc + i*4),*(unsigned int *)((epc + i*4)| 0xa0000000));
		  	}
			printf("-----------------------------------------------------\n");
		}
			
		read_epc_count--;
	}
	
	unsigned int cause = read_c0_cause();
	unsigned int exc = (cause >> 2) & 0x1f;
	printf("CAUSE=%08x --> %s\n", cause, cause_strings[exc]);
	printf("EPC=%08x\n", epc);

	for (i=0;i<32;i++) {
		if (i % 4 == 0)
			printf("\n");
		printf("%8s %08x ", regstr[i], sp[i]);
	}
	printf("\n");
	
	//发生watch异常时，直接返回，系统可以继续运行。方便调试。
	if ((exc == 23) && (epc != 0))
	{
		__asm__ __volatile__("eret\n\t");
	}

#if RELEASE==0
	i = (unsigned int)shelltask;
	write_32bit_cp0_register(CP0_EPC,i);
	__asm__ __volatile__("eret\n\t");
#endif

	if(g_CurExcept > 0)
	{
		int sp1 = 0,sp2 = 0x7fffffff,save = 0;
		for(i = 0;i < g_CurExcept;i++)
		{
			if(g_pExcept[i].sp != 0)
			{
				sp1 = (int)(g_pExcept[i].sp - sp[29]);
				printf("g_pExcept[%d].sp = %x sp = %x\n",i,g_pExcept[i].sp,sp[29]);
				if((sp1 <= sp2)&& (sp1 > 0))
				{
					sp2 = sp1;
					save = i;
				}
			}
		}
		if(sp2 == 0x7fffffff)
			hw_reset();
		printf("save = %d\n",save);
		i = (unsigned int)(g_pExcept[save].except_handle);
		printf("except return %x %d %x %x\n",i,g_CurExcept,g_pExcept[g_CurExcept].sp,g_pExcept[g_CurExcept].ra);
		if(i)
		{
			g_pExcept[save].sp            = 0;
		  g_pExcept[save].ra            = 0;
		  g_pExcept[save].except_handle = 0;
		}

		//memcpy(&(g_pExcept[save]),&(g_pExcept[save+1]),sizeof(Except_Struct) * (g_CurExcept - save - 1));
		//g_CurExcept--;



    }else if(main_except_handle){
		i = (unsigned int)main_except_handle;
    }else{
        printf("c_except_handler: while(1)");
#if DRIVER_ONLY
		while(1);
#else
		hw_reset();
#endif
    }

	write_32bit_cp0_register(CP0_EPC,i);
	__asm__ __volatile__("eret\n\t");
}
#ifdef ENABLE_FPU
#define RESTORE_PROCESS_REGISTER(x,sr) \
    do{ 								\
    	__asm__ __volatile__(			\
    	"lw $31,0x00(%0)\n\t"			\
    	:: "r" (&sr));          \
    	__asm__ __volatile__(			\
    	"lw $3,(%0)\n\t"			\
    	:: "r" (&x));          \
    	__asm__ __volatile__(			\
    	".set noat    \n\t"		\
    		"lw $29,0x04(%0)\n\t"			\
    		"lw $1,0x00($29)\n\t"			\
			"lw $2,0x04($29)\n\t"			\
			"lw $4,0x0C($29)\n\t"			\
			"lw $5,0x10($29)\n\t"			\
			"lw $6,0x14($29)\n\t"			\
			"lw $7,0x18($29)\n\t"			\
			"lw $8,0x1C($29)\n\t"			\
			"lw $9,0x20($29)\n\t"			\
			"lw $10,0x24($29)\n\t"			\
			"lw $11,0x28($29)\n\t"			\
			"lw $12,0x2C($29)\n\t"			\
			"lw $13,0x30($29)\n\t"			\
			"lw $14,0x34($29)\n\t"			\
			"lw $15,0x38($29)\n\t"			\
			"lw $16,0x3C($29)\n\t"			\
			"lw $17,0x40($29)\n\t"			\
			"lw $18,0x44($29)\n\t"			\
			"lw $19,0x48($29)\n\t"			\
			"lw $20,0x4C($29)\n\t"			\
			"lw $21,0x50($29)\n\t"			\
			"lw $22,0x54($29)\n\t"			\
			"lw $23,0x58($29)\n\t"			\
			"lw $24,0x5C($29)\n\t"			\
			"lw $25,0x60($29)\n\t"			\
			"lw $27,0x68($29)\n\t"			\
			"lw $28,0x6C($29)\n\t"			\
			"lw $30,0x74($29)\n\t"			\
			"lw $31,0x78($29)\n\t"			\
		"lwc1 $f0,0x7c($29)\n\t" \
		"lwc1 $f1,0x80($29)\n\t" \
		"lwc1 $f2,0x84($29)\n\t" \
		"lwc1 $f3,0x88($29)\n\t" \
		"lwc1 $f4,0x8c($29)\n\t" \
		"lwc1 $f5,0x90($29)\n\t" \
		"lwc1 $f6,0x94($29)\n\t" \
		"lwc1 $f7,0x98($29)\n\t" \
		"lwc1 $f8,0x9c($29)\n\t" \
		"lwc1 $f9,0xa0($29)\n\t" \
		"lwc1 $f10,0xa4($29)\n\t" \
		"lwc1 $f11,0xa8($29)\n\t" \
		"lwc1 $f12,0xac($29)\n\t" \
		"lwc1 $f13,0xb0($29)\n\t" \
		"lwc1 $f14,0xb4($29)\n\t" \
		"lwc1 $f15,0xb8($29)\n\t" \
		"lwc1 $f16,0xbc($29)\n\t" \
		"lwc1 $f17,0xc0($29)\n\t" \
		"lwc1 $f18,0xc4($29)\n\t" \
		"lwc1 $f19,0xc8($29)\n\t" \
		"lwc1 $f20,0xcc($29)\n\t" \
		"lwc1 $f21,0xd0($29)\n\t" \
		"lwc1 $f22,0xd4($29)\n\t" \
		"lwc1 $f23,0xd8($29)\n\t" \
		"lwc1 $f24,0xdc($29)\n\t" \
		"lwc1 $f25,0xe0($29)\n\t" \
		"lwc1 $f26,0xe4($29)\n\t" \
		"lwc1 $f27,0xe8($29)\n\t" \
		"lwc1 $f28,0xec($29)\n\t" \
		"lwc1 $f29,0xf0($29)\n\t" \
		"lwc1 $f30,0xf4($29)\n\t" \
		"lwc1 $f31,0xf8($29)\n\t" \
		"lw $26, 0xfc($29)\n\t"  \
		"ctc1 $26, $31\n\t"  \
			"addu $29,0x100    \n\t"			\
			"move $2,$3 \n\t"  \
			"jr $31 \n\t"							\
			"nop\n\t"									\
			".set at    \n\t"		\
			:     										\
			: "r" (sr)); \
    }while(0)
#else

#define RESTORE_PROCESS_REGISTER(x,sr) \
    do{ 								\
    	__asm__ __volatile__(			\
    	"lw $31,0x00(%0)\n\t"			\
    	:: "r" (&sr));          \
    	__asm__ __volatile__(			\
    	"lw $3,(%0)\n\t"			\
    	:: "r" (&x));          \
    	__asm__ __volatile__(			\
    	".set noat    \n\t"		\
    		"lw $29,0x04(%0)\n\t"			\
    		"lw $1,0x00($29)\n\t"			\
			"lw $2,0x04($29)\n\t"			\
			"lw $4,0x0C($29)\n\t"			\
			"lw $5,0x10($29)\n\t"			\
			"lw $6,0x14($29)\n\t"			\
			"lw $7,0x18($29)\n\t"			\
			"lw $8,0x1C($29)\n\t"			\
			"lw $9,0x20($29)\n\t"			\
			"lw $10,0x24($29)\n\t"			\
			"lw $11,0x28($29)\n\t"			\
			"lw $12,0x2C($29)\n\t"			\
			"lw $13,0x30($29)\n\t"			\
			"lw $14,0x34($29)\n\t"			\
			"lw $15,0x38($29)\n\t"			\
			"lw $16,0x3C($29)\n\t"			\
			"lw $17,0x40($29)\n\t"			\
			"lw $18,0x44($29)\n\t"			\
			"lw $19,0x48($29)\n\t"			\
			"lw $20,0x4C($29)\n\t"			\
			"lw $21,0x50($29)\n\t"			\
			"lw $22,0x54($29)\n\t"			\
			"lw $23,0x58($29)\n\t"			\
			"lw $24,0x5C($29)\n\t"			\
			"lw $25,0x60($29)\n\t"			\
			"lw $27,0x68($29)\n\t"			\
			"lw $28,0x6C($29)\n\t"			\
			"lw $30,0x74($29)\n\t"			\
			"lw $31,0x78($29)\n\t"			\
			"addu $29,128    \n\t"			\
			"move $2,$3 \n\t"  \
			"jr $31 \n\t"							\
			"nop\n\t"									\
			".set at    \n\t"		\
			:     										\
			: "r" (sr)); \
    }while(0)

#endif

void excpt_exit(unsigned int x,unsigned int *sr)
{
		printf("%s ra = %x sp = %x\n", __FUNCTION__, sr[0],sr[1]);

		RESTORE_PROCESS_REGISTER(x,sr);
		//STORE_REG_RET();
	  //RESTORE_PROCESS_REGISTER(x);
}
