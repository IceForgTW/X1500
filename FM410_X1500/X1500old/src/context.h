/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : CONTEXT.h                                               *
*                                                                         *
*   AUTHOR      : Michael Anburaj                                         *
*                 URL  : http://geocities.com/michaelanburaj/             *
*                 EMAIL: michaelanburaj@hotmail.com                       *
*                                                                         *
*   PROCESSOR   : MIPS 4Kc (32 bit RISC) - ATLAS board                    *
*                                                                         *
*   TOOL-CHAIN  : SDE & Cygnus                                            *
*                                                                         *
*   DESCRIPTION :                                                         *
*   Context switch macros.                                                *
*                                                                         *
**************************************************************************/


#ifndef __CONTEXT_H__
#define __CONTEXT_H__


/* ********************************************************************* */
/* Module configuration */


/* ********************************************************************* */
/* Interface macro & data definition */

#ifdef _ASSEMBLER_

#ifdef ENABLE_FPU
// push context: at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define STORE_REG_RET(Retaddr) \
		.set noat; \
    .set noreorder; \
        subu sp,320; \
        sw ra,0(sp); \
        sw fp,4(sp); \
        sw gp,8(sp); \
        sw t9,12(sp); \
        sw t8,16(sp); \
        sw s7,20(sp); \
        sw s6,24(sp); \
        sw s5,28(sp); \
        sw s4,32(sp); \
        sw s3,36(sp); \
        sw s2,40(sp); \
        sw s1,44(sp); \
        sw s0,48(sp); \
        sw t7,52(sp); \
        sw t6,56(sp); \
        sw t5,60(sp); \
        sw t4,64(sp); \
        sw t3,68(sp); \
        sw t2,72(sp); \
        sw t1,76(sp); \
        sw t0,80(sp); \
        sw a3,84(sp); \
        sw a2,88(sp); \
        sw a1,92(sp); \
        sw a0,96(sp); \
        sw v1,100(sp); \
        sw v0,104(sp); \
        sw AT,108(sp); \
        MFC0(k1,C0_STATUS); \
        sw k1,112(sp); \
        sw Retaddr,116(sp); \
        MFLo k1;       \
        sw k1,120(sp); \
        MFHi k1;       \
        sw k1,124(sp); \
.word	0b01110011101000001000000001010001;	/* #S32STD XR1,$29,128  */ \
.word	0b01110011101000001000010010010001;	/* #S32STD XR2,$29,132  */ \
.word	0b01110011101000001000100011010001;	/* #S32STD XR3,$29,136  */ \
.word	0b01110011101000001000110100010001;	/* #S32STD XR4,$29,140  */ \
.word	0b01110011101000001001000101010001;	/* #S32STD XR5,$29,144  */ \
.word	0b01110011101000001001010110010001;	/* #S32STD XR6,$29,148  */ \
.word	0b01110011101000001001100111010001;	/* #S32STD XR7,$29,152  */ \
.word	0b01110011101000001001111000010001;	/* #S32STD XR8,$29,156  */ \
.word	0b01110011101000001010001001010001;	/* #S32STD XR9,$29,160  */ \
.word	0b01110011101000001010011010010001;	/* #S32STD XR10,$29,164 */ \
.word	0b01110011101000001010101011010001;	/* #S32STD XR11,$29,168 */ \
.word	0b01110011101000001010111100010001;	/* #S32STD XR12,$29,172 */ \
.word	0b01110011101000001011001101010001;	/* #S32STD XR13,$29,176 */ \
.word	0b01110011101000001011011110010001;	/* #S32STD XR14,$29,180 */ \
.word	0b01110011101000001011101111010001;	/* #S32STD XR15,$29,184 */ \
	cfc1 k1, $31; \
	swc1 $f0,188(sp); \
	swc1 $f1,192(sp); \
	swc1 $f2,196(sp); \
	swc1 $f3,200(sp); \
	swc1 $f4,204(sp); \
	swc1 $f5,208(sp); \
	swc1 $f6,212(sp); \
	swc1 $f7,216(sp); \
	swc1 $f8,220(sp); \
	swc1 $f9,224(sp); \
	swc1 $f10,228(sp); \
	swc1 $f11,232(sp); \
	swc1 $f12,236(sp); \
	swc1 $f13,240(sp); \
	swc1 $f14,244(sp); \
	swc1 $f15,248(sp); \
	swc1 $f16,252(sp); \
	swc1 $f17,256(sp); \
	swc1 $f18,260(sp); \
	swc1 $f19,264(sp); \
	swc1 $f20,268(sp); \
	swc1 $f21,272(sp); \
	swc1 $f22,276(sp); \
	swc1 $f23,280(sp); \
	swc1 $f24,284(sp); \
	swc1 $f25,288(sp); \
	swc1 $f26,292(sp); \
	swc1 $f27,296(sp); \
	swc1 $f28,300(sp); \
	swc1 $f29,304(sp); \
	swc1 $f30,308(sp); \
	swc1 $f31,312(sp); \
	sw k1, 316(sp); \
.set at

// pop context: at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define RESTORE_REG_ERET() \
		.set noat; \
		.set noreorder; \
        lw ra,0(sp); \
        lw fp,4(sp); \
        lw gp,8(sp); \
        lw t9,12(sp); \
        lw t8,16(sp); \
        lw s7,20(sp); \
        lw s6,24(sp); \
        lw s5,28(sp); \
        lw s4,32(sp); \
        lw s3,36(sp); \
        lw s2,40(sp); \
        lw s1,44(sp); \
        lw s0,48(sp); \
        lw t7,52(sp); \
        lw t6,56(sp); \
        lw t5,60(sp); \
        lw t4,64(sp); \
        lw t3,68(sp); \
        lw t2,72(sp); \
        lw t1,76(sp); \
        lw t0,80(sp); \
        lw a3,84(sp); \
        lw a2,88(sp); \
        lw a1,92(sp); \
        lw a0,96(sp); \
        lw v1,100(sp); \
        lw v0,104(sp); \
        lw AT,108(sp); \
        lw k0,112(sp); \
        MFC0(k1,C0_STATUS); \
        srl k1,  k1, 1; \
        sll k1,  k1, 1; \
        andi k0, k0, 1;  \
        or k0, k1, k0;  \
        MTC0(k0,C0_STATUS); \
        lw k0,116(sp); \
        MTC0(k0,C0_EPC); \
        lw k0,120(sp); \
        MTLo k0;       \
        lw k0,124(sp); \
        MTHi  k0;      \
.word	0b01110011101000001000000001010000;	/*#S32LDD XR1,$29,128  */ \
.word	0b01110011101000001000010010010000;	/*#S32LDD XR2,$29,132  */ \
.word	0b01110011101000001000100011010000;	/*#S32LDD XR3,$29,136  */ \
.word	0b01110011101000001000110100010000;	/*#S32LDD XR4,$29,140  */ \
.word	0b01110011101000001001000101010000;	/*#S32LDD XR5,$29,144  */ \
.word	0b01110011101000001001010110010000;	/*#S32LDD XR6,$29,148  */ \
.word	0b01110011101000001001100111010000;	/*#S32LDD XR7,$29,152  */ \
.word	0b01110011101000001001111000010000;	/*#S32LDD XR8,$29,156  */ \
.word	0b01110011101000001010001001010000;	/*#S32LDD XR9,$29,160  */ \
.word	0b01110011101000001010011010010000;	/*#S32LDD XR10,$29,164 */ \
.word	0b01110011101000001010101011010000;	/*#S32LDD XR11,$29,168 */ \
.word	0b01110011101000001010111100010000;	/*#S32LDD XR12,$29,172 */ \
.word	0b01110011101000001011001101010000;	/*#S32LDD XR13,$29,176 */ \
.word	0b01110011101000001011011110010000;	/*#S32LDD XR14,$29,180 */ \
.word	0b01110011101000001011101111010000;	/*#S32LDD XR15,$29,184 */ \
		lw k1, 316(sp); \
		lwc1 $f0,188(sp); \
		lwc1 $f1,192(sp); \
		lwc1 $f2,196(sp); \
		lwc1 $f3,200(sp); \
		lwc1 $f4,204(sp); \
		lwc1 $f5,208(sp); \
		lwc1 $f6,212(sp); \
		lwc1 $f7,216(sp); \
		lwc1 $f8,220(sp); \
		lwc1 $f9,224(sp); \
		lwc1 $f10,228(sp); \
		lwc1 $f11,232(sp); \
		lwc1 $f12,236(sp); \
		lwc1 $f13,240(sp); \
		lwc1 $f14,244(sp); \
		lwc1 $f15,248(sp); \
		lwc1 $f16,252(sp); \
		lwc1 $f17,256(sp); \
		lwc1 $f18,260(sp); \
		lwc1 $f19,264(sp); \
		lwc1 $f20,268(sp); \
		lwc1 $f21,272(sp); \
		lwc1 $f22,276(sp); \
		lwc1 $f23,280(sp); \
		lwc1 $f24,284(sp); \
		lwc1 $f25,288(sp); \
		lwc1 $f26,292(sp); \
		lwc1 $f27,296(sp); \
		lwc1 $f28,300(sp); \
		lwc1 $f29,304(sp); \
		lwc1 $f30,308(sp); \
		lwc1 $f31,312(sp); \
		ctc1 k1, $31; \
		addu 	sp,320; \
        eret; \
        nop; \
.set at				
#else
// push context: at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define STORE_REG_RET(Retaddr) \
		.set noat; \
    .set noreorder; \
        subu sp,192; \
        sw ra,0(sp); \
        sw fp,4(sp); \
        sw gp,8(sp); \
        sw t9,12(sp); \
        sw t8,16(sp); \
        sw s7,20(sp); \
        sw s6,24(sp); \
        sw s5,28(sp); \
        sw s4,32(sp); \
        sw s3,36(sp); \
        sw s2,40(sp); \
        sw s1,44(sp); \
        sw s0,48(sp); \
        sw t7,52(sp); \
        sw t6,56(sp); \
        sw t5,60(sp); \
        sw t4,64(sp); \
        sw t3,68(sp); \
        sw t2,72(sp); \
        sw t1,76(sp); \
        sw t0,80(sp); \
        sw a3,84(sp); \
        sw a2,88(sp); \
        sw a1,92(sp); \
        sw a0,96(sp); \
        sw v1,100(sp); \
        sw v0,104(sp); \
        sw AT,108(sp); \
        MFC0(k1,C0_STATUS); \
        sw k1,112(sp); \
        sw Retaddr,116(sp); \
        MFLo k1;       \
        sw k1,120(sp); \
        MFHi k1;       \
        sw k1,124(sp); \
.word	0b01110011101000001000000001010001;	/* #S32STD XR1,$29,128  */ \
.word	0b01110011101000001000010010010001;	/* #S32STD XR2,$29,132  */ \
.word	0b01110011101000001000100011010001;	/* #S32STD XR3,$29,136  */ \
.word	0b01110011101000001000110100010001;	/* #S32STD XR4,$29,140  */ \
.word	0b01110011101000001001000101010001;	/* #S32STD XR5,$29,144  */ \
.word	0b01110011101000001001010110010001;	/* #S32STD XR6,$29,148  */ \
.word	0b01110011101000001001100111010001;	/* #S32STD XR7,$29,152  */ \
.word	0b01110011101000001001111000010001;	/* #S32STD XR8,$29,156  */ \
.word	0b01110011101000001010001001010001;	/* #S32STD XR9,$29,160  */ \
.word	0b01110011101000001010011010010001;	/* #S32STD XR10,$29,164 */ \
.word	0b01110011101000001010101011010001;	/* #S32STD XR11,$29,168 */ \
.word	0b01110011101000001010111100010001;	/* #S32STD XR12,$29,172 */ \
.word	0b01110011101000001011001101010001;	/* #S32STD XR13,$29,176 */ \
.word	0b01110011101000001011011110010001;	/* #S32STD XR14,$29,180 */ \
.word	0b01110011101000001011101111010001;	/* #S32STD XR15,$29,184 */ \
.set at
// pop context (normal execution): at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define RESTORE_REG_RET() \
		.set noat; \
    .set noreorder; \
        lw ra,0(sp); \
        lw fp,4(sp); \
        lw gp,8(sp); \
        lw t9,12(sp); \
        lw t8,16(sp); \
        lw s7,20(sp); \
        lw s6,24(sp); \
        lw s5,28(sp); \
        lw s4,32(sp); \
        lw s3,36(sp); \
        lw s2,40(sp); \
        lw s1,44(sp); \
        lw s0,48(sp); \
        lw t7,52(sp); \
        lw t6,56(sp); \
        lw t5,60(sp); \
        lw t4,64(sp); \
        lw t3,68(sp); \
        lw t2,72(sp); \
        lw t1,76(sp); \
        lw t0,80(sp); \
        lw a3,84(sp); \
        lw a2,88(sp); \
        lw a1,92(sp); \
        lw a0,96(sp); \
        lw v1,100(sp); \
        lw v0,104(sp); \
        lw AT,108(sp); \
        lw k0,112(sp); \
        MFC0(k1,C0_STATUS); \
        srl k1,  k1, 1; \
        sll k1,  k1, 1; \
        andi k0, k0, 1;  \
        or k0, k1, k0;  \
        MTC0(k0,C0_STATUS); \
        lw k0,120(sp); \
        MTLo k0;       \
        lw k0,124(sp); \
        MTHi k0      ; \
        lw k0,116(sp); \
				.word	0b01110011101000001000000001010000;	/*#S32LDD XR1,$29,128*/  \
				.word	0b01110011101000001000010010010000;	/*#S32LDD XR2,$29,132 */ \
				.word	0b01110011101000001000100011010000;	/*#S32LDD XR3,$29,136 */ \
				.word	0b01110011101000001000110100010000;	/*#S32LDD XR4,$29,140 */ \
				.word	0b01110011101000001001000101010000;	/*#S32LDD XR5,$29,144 */ \
				.word	0b01110011101000001001010110010000;	/*#S32LDD XR6,$29,148 */ \
				.word	0b01110011101000001001100111010000;	/*#S32LDD XR7,$29,152 */ \
				.word	0b01110011101000001001111000010000;	/*#S32LDD XR8,$29,156 */ \
				.word	0b01110011101000001010001001010000;	/*#S32LDD XR9,$29,160 */ \
				.word	0b01110011101000001010011010010000;	/*#S32LDD XR10,$29,164*/ \
				.word	0b01110011101000001010101011010000;	/*#S32LDD XR11,$29,168*/ \
				.word	0b01110011101000001010111100010000;	/*#S32LDD XR12,$29,172*/ \
				.word	0b01110011101000001011001101010000;	/*#S32LDD XR13,$29,176*/ \
				.word	0b01110011101000001011011110010000;	/*#S32LDD XR14,$29,180*/ \
				.word	0b01110011101000001011101111010000;	/*#S32LDD XR15,$29,184*/ \
				addu sp,192; \
        jr k0; \
        nop; \
				.set at
    
// pop context: at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define RESTORE_REG_ERET() \
		.set noat; \
		.set noreorder; \
        lw ra,0(sp); \
        lw fp,4(sp); \
        lw gp,8(sp); \
        lw t9,12(sp); \
        lw t8,16(sp); \
        lw s7,20(sp); \
        lw s6,24(sp); \
        lw s5,28(sp); \
        lw s4,32(sp); \
        lw s3,36(sp); \
        lw s2,40(sp); \
        lw s1,44(sp); \
        lw s0,48(sp); \
        lw t7,52(sp); \
        lw t6,56(sp); \
        lw t5,60(sp); \
        lw t4,64(sp); \
        lw t3,68(sp); \
        lw t2,72(sp); \
        lw t1,76(sp); \
        lw t0,80(sp); \
        lw a3,84(sp); \
        lw a2,88(sp); \
        lw a1,92(sp); \
        lw a0,96(sp); \
        lw v1,100(sp); \
        lw v0,104(sp); \
        lw AT,108(sp); \
        lw k0,112(sp); \
        MFC0(k1,C0_STATUS); \
        srl k1,  k1, 1; \
        sll k1,  k1, 1; \
        andi k0, k0, 1;  \
        or k0, k1, k0;  \
        MTC0(k0,C0_STATUS); \
        lw k0,116(sp); \
        MTC0(k0,C0_EPC); \
        lw k0,120(sp); \
        MTLo k0;       \
        lw k0,124(sp); \
        MTHi  k0;      \
.word	0b01110011101000001000000001010000;	/*#S32LDD XR1,$29,128  */ \
.word	0b01110011101000001000010010010000;	/*#S32LDD XR2,$29,132  */ \
.word	0b01110011101000001000100011010000;	/*#S32LDD XR3,$29,136  */ \
.word	0b01110011101000001000110100010000;	/*#S32LDD XR4,$29,140  */ \
.word	0b01110011101000001001000101010000;	/*#S32LDD XR5,$29,144  */ \
.word	0b01110011101000001001010110010000;	/*#S32LDD XR6,$29,148  */ \
.word	0b01110011101000001001100111010000;	/*#S32LDD XR7,$29,152  */ \
.word	0b01110011101000001001111000010000;	/*#S32LDD XR8,$29,156  */ \
.word	0b01110011101000001010001001010000;	/*#S32LDD XR9,$29,160  */ \
.word	0b01110011101000001010011010010000;	/*#S32LDD XR10,$29,164 */ \
.word	0b01110011101000001010101011010000;	/*#S32LDD XR11,$29,168 */ \
.word	0b01110011101000001010111100010000;	/*#S32LDD XR12,$29,172 */ \
.word	0b01110011101000001011001101010000;	/*#S32LDD XR13,$29,176 */ \
.word	0b01110011101000001011011110010000;	/*#S32LDD XR14,$29,180 */ \
.word	0b01110011101000001011101111010000;	/*#S32LDD XR15,$29,184 */ \
       addu sp,192; \
        eret; \
        nop; \
.set at

#endif
    

#endif /* _ASSEMBLER_ */


/* ********************************************************************* */
/* Interface function definition */


/* ********************************************************************* */

#ifdef __cplusplus
}
#endif

#endif /*__CONTEXT_H__*/
