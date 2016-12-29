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


#ifndef __EXCPT_H__
#define __EXCPT_H__
typedef void (*PFun_Exception_Modal_Handler)(void);


#ifdef ENABLE_FPU
#define SAVE_PROCESS_REGISTER(sr) \
    do{          				\
    	__asm__ __volatile__(			\
    	".set noat    \n\t"				\
    		"subu $29,0x100   \n\t"			\
    		"sw $1,0x00($29)\n\t"			\
			"sw $2,0x04($29)\n\t"			\
			"sw $3,0x08($29)\n\t"			\
			"sw $4,0x0C($29)\n\t"			\
			"sw $5,0x10($29)\n\t"			\
			"sw $6,0x14($29)\n\t"			\
			"sw $7,0x18($29)\n\t"			\
			"sw $8,0x1C($29)\n\t"			\
			"sw $9,0x20($29)\n\t"			\
			"sw $10,0x24($29)\n\t"			\
			"sw $11,0x28($29)\n\t"			\
			"sw $12,0x2C($29)\n\t"			\
			"sw $13,0x30($29)\n\t"			\
			"sw $14,0x34($29)\n\t"			\
			"sw $15,0x38($29)\n\t"			\
			"sw $16,0x3C($29)\n\t"			\
			"sw $17,0x40($29)\n\t"			\
			"sw $18,0x44($29)\n\t"			\
			"sw $19,0x48($29)\n\t"			\
			"sw $20,0x4C($29)\n\t"			\
			"sw $21,0x50($29)\n\t"			\
			"sw $22,0x54($29)\n\t"			\
			"sw $23,0x58($29)\n\t"			\
			"sw $24,0x5C($29)\n\t"			\
			"sw $25,0x60($29)\n\t"			\
			"sw $26,0x64($29)\n\t"			\
			"sw $27,0x68($29)\n\t"			\
			"sw $28,0x6C($29)\n\t"			\
			"sw $29,0x70($29)\n\t"			\
			"sw $30,0x74($29)\n\t"			\
			"sw $31,0x78($29)\n\t"			\
		"swc1 $f0,0x7c($29)\n\t" \
		"swc1 $f1,0x80($29)\n\t" \
		"swc1 $f2,0x84($29)\n\t" \
		"swc1 $f3,0x88($29)\n\t" \
		"swc1 $f4,0x8c($29)\n\t" \
		"swc1 $f5,0x90($29)\n\t" \
		"swc1 $f6,0x94($29)\n\t" \
		"swc1 $f7,0x98($29)\n\t" \
		"swc1 $f8,0x9c($29)\n\t" \
		"swc1 $f9,0xa0($29)\n\t" \
		"swc1 $f10,0xa4($29)\n\t" \
		"swc1 $f11,0xa8($29)\n\t" \
		"swc1 $f12,0xac($29)\n\t" \
		"swc1 $f13,0xb0($29)\n\t" \
		"swc1 $f14,0xb4($29)\n\t" \
		"swc1 $f15,0xb8($29)\n\t" \
		"swc1 $f16,0xbc($29)\n\t" \
		"swc1 $f17,0xc0($29)\n\t" \
		"swc1 $f18,0xc4($29)\n\t" \
		"swc1 $f19,0xc8($29)\n\t" \
		"swc1 $f20,0xcc($29)\n\t" \
		"swc1 $f21,0xd0($29)\n\t" \
		"swc1 $f22,0xd4($29)\n\t" \
		"swc1 $f23,0xd8($29)\n\t" \
		"swc1 $f24,0xdc($29)\n\t" \
		"swc1 $f25,0xe0($29)\n\t" \
		"swc1 $f26,0xe4($29)\n\t" \
		"swc1 $f27,0xe8($29)\n\t" \
		"swc1 $f28,0xec($29)\n\t" \
		"swc1 $f29,0xf0($29)\n\t" \
		"swc1 $f30,0xf4($29)\n\t" \
		"swc1 $f31,0xf8($29)\n\t" \
		"cfc1 $26, $31\n\t" \
		"sw $26, 0xfc($29)\n\t" \
		".set at \n\t"						\
			);    							\
			__asm__ __volatile__(			\
    	"lw $21,0x70($29)     \n\t"			\
		"lw $20,0x78($29)     \n\t"			\
	   	"sw $20,0x00(%0)      \n\t"         \
	   	"sw $21,0x04(%0)      \n\t"         \
			:: "r" (sr)); \
    }while(0)				
#else

#define SAVE_PROCESS_REGISTER(sr) \
    do{          				\
    	__asm__ __volatile__(			\
    	".set noat    \n\t"				\
    		"subu $29,128   \n\t"			\
    		"sw $1,0x00($29)\n\t"			\
			"sw $2,0x04($29)\n\t"			\
			"sw $3,0x08($29)\n\t"			\
			"sw $4,0x0C($29)\n\t"			\
			"sw $5,0x10($29)\n\t"			\
			"sw $6,0x14($29)\n\t"			\
			"sw $7,0x18($29)\n\t"			\
			"sw $8,0x1C($29)\n\t"			\
			"sw $9,0x20($29)\n\t"			\
			"sw $10,0x24($29)\n\t"			\
			"sw $11,0x28($29)\n\t"			\
			"sw $12,0x2C($29)\n\t"			\
			"sw $13,0x30($29)\n\t"			\
			"sw $14,0x34($29)\n\t"			\
			"sw $15,0x38($29)\n\t"			\
			"sw $16,0x3C($29)\n\t"			\
			"sw $17,0x40($29)\n\t"			\
			"sw $18,0x44($29)\n\t"			\
			"sw $19,0x48($29)\n\t"			\
			"sw $20,0x4C($29)\n\t"			\
			"sw $21,0x50($29)\n\t"			\
			"sw $22,0x54($29)\n\t"			\
			"sw $23,0x58($29)\n\t"			\
			"sw $24,0x5C($29)\n\t"			\
			"sw $25,0x60($29)\n\t"			\
			"sw $26,0x64($29)\n\t"			\
			"sw $27,0x68($29)\n\t"			\
			"sw $28,0x6C($29)\n\t"			\
			"sw $29,0x70($29)\n\t"			\
			"sw $30,0x74($29)\n\t"			\
			"sw $31,0x78($29)\n\t"			\
		".set at \n\t"						\
			);    							\
			__asm__ __volatile__(			\
    	"lw $21,0x70($29)     \n\t"			\
		"lw $20,0x78($29)     \n\t"			\
	   	"sw $20,0x00(%0)      \n\t"         \
	   	"sw $21,0x04(%0)      \n\t"         \
			:: "r" (sr)); \
		}while(0)				

#endif
void excpt_exit(unsigned int x,unsigned int *sr);
void DecExcept(int d,unsigned int *sr);
int AddExcept(PFun_Exception_Modal_Handler pFun,unsigned int *sr);


#define EXCEPTION_XIMAGE 1
#define EXCEPTION_MPLAYER 1

#endif
