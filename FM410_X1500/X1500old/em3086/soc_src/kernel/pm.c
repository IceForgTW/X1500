/********************** BEGIN LICENSE BLOCK ************************************
 *
 * JZ4755  mobile_tv  Project  V1.0.0
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
 *  Author:  <dsqiu@ingenic.cn>  <zzhang@ingenic.cn>
 *
 *  Create:   2008-10, by zhang zhi
 *
 *  Maintain: 2008-11, by zhang zhi
 *
 *
 *******************************************************************************
 */

#include <bsp.h>
#include <x1000.h>
#include <os_api.h>
#include <mipsregs.h>

//--------------------------------------------------------------------
#include <dm.h>
#include <pm.h>
#include <cache.h>

#include <string.h>
#undef printf
#include <stdio.h>

extern void jz_cpu_sleep(void);
extern void jz_cpu_resume(void);
#define mb()	asm volatile ("": : :"memory")

//--------------------------------------------------------------------

#undef KSEG0
#define KSEG0	0x80000000
#define K0BASE  KSEG0

extern long long save_goto(unsigned int);
extern int restore_goto(void);

#define GSS_IGNORE		1
#define GSS_INPUT_NOPULL	2
#define GSS_OUTPUT_LOW		3
#define GSS_OUTPUT_HIGH		4
#define GSS_INPUT_PULL		5
#define GSS_TABLET_END		0x999

int gpio_sleep_state_table[][2] = {
    /*======== PORT A ===================================*/        
    {32*0+0,    GSS_OUTPUT_LOW  },  /* CIF_SCL */
    {32*0+1,    GSS_OUTPUT_LOW  },  /* CIF_SDA */
    {32*0+2,    GSS_INPUT_PULL  },  /* SLCD_D2 */
    {32*0+3,    GSS_OUTPUT_LOW  },  /* CIF_STANDBY */
    {32*0+4,    GSS_OUTPUT_LOW  },  /* CIF_EXPOUSE */
    {32*0+5,    GSS_OUTPUT_LOW  },  /* CIF_FOC */
    {32*0+6,    GSS_OUTPUT_LOW  },  /* CIF_RESET */
    {32*0+7,    GSS_OUTPUT_LOW  },  /* CIF_LED_OUT */
    {32*0+8,    GSS_OUTPUT_LOW  },  /* CIF_PCLK */
    {32*0+9,    GSS_OUTPUT_LOW  },  /* CIF_HREF */
    {32*0+10,   GSS_OUTPUT_LOW  },  /* CIF_VSYNC */
    {32*0+11,   GSS_OUTPUT_LOW  },  /* CIF_MCLK */
    {32*0+12,   GSS_OUTPUT_LOW  },  /* CIF_Y7 */
    {32*0+13,   GSS_OUTPUT_LOW  },  /* CIF_Y6 */
    {32*0+14,   GSS_OUTPUT_LOW  },  /* CIF_Y5 */
    {32*0+15,   GSS_OUTPUT_LOW  },  /* CIF_Y4 */
    {32*0+16,   GSS_OUTPUT_LOW  },  /* CIF_Y3 */
    {32*0+17,   GSS_OUTPUT_LOW  },  /* CIF_Y2 */
    {32*0+18,   GSS_OUTPUT_LOW  },  /* CIF_Y1 */
    {32*0+19,   GSS_OUTPUT_LOW  },  /* CIF_Y0 */
    {32*0+20,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*0+21,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*0+22,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*0+23,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*0+24,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*0+25,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*0+26,   GSS_INPUT_PULL  },  /* SFC_CLK */
    {32*0+27,   GSS_OUTPUT_HIGH },  /* SFC_CE */
    {32*0+28,   GSS_INPUT_PULL  },  /* SFC_DR */
    {32*0+29,   GSS_INPUT_PULL  },  /* SFC_DT */
    {32*0+30,   GSS_INPUT_PULL  },  /* SFC_WP */
    {32*0+31,   GSS_INPUT_PULL  },  /* SFC_HOL */
        
    /********** PORT B **************************/        
    {32*1+0,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+1,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+2,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+3,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+4,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+5,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+6,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+7,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+8,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+9,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+10,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+11,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+12,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+13,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+14,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+15,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+16,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+17,   GSS_INPUT_PULL  },  /* CIF_VSYNC */
    {32*1+18,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+19,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+20,   GSS_OUTPUT_HIGH },  /* PWR */
    {32*1+21,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+22,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+23,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+24,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+25,   GSS_INPUT_PULL  },  /* DRVVBUS */
    {32*1+26,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+27,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*1+28,   GSS_INPUT_PULL  },  /* BOOT_SEL0 */
    {32*1+29,   GSS_INPUT_PULL  },  /* BOOT_SEL1 */
    {32*1+30,   GSS_INPUT_PULL  },  /* BOOT_SEL2 */
    {32*1+31,   GSS_IGNORE  },  /* WAKEUP */
        
    /*********** port C  *************************/
    {32*2+0,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+1,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+2,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+3,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+4,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+5,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+6,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+7,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+8,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+9,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+10,   GSS_IGNORE      },  /* UART0_RXD */
    {32*2+11,   GSS_IGNORE      },  /* UART0_TXD */
    {32*2+12,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+13,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+16,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+17,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+18,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+19,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+20,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+21,   GSS_OUTPUT_LOW  },  /* LED */
    {32*2+22,   GSS_IGNORE  },  /* USB_DETECT */
    {32*2+23,   GSS_IGNORE  },  /* CPU_TRIG */
    {32*2+24,   GSS_OUTPUT_LOW  },  /* Light_PWM */
    {32*2+25,   GSS_OUTPUT_LOW  },  /* BUZ_PWM */
    {32*2+26,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+27,   GSS_INPUT_PULL  },  /* NOT USE */
    {32*2+31,   GSS_INPUT_PULL  },  /* NOT USE   */
        
    /*********** port D ***************************/    
    {32*3+0,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*3+1,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*3+2,    GSS_IGNORE  },  /* UART1_TXD */
    {32*3+3,    GSS_IGNORE  },  /* UART1_RXD */
    {32*3+4,    GSS_INPUT_PULL  },  /* NOT USE */
    {32*3+5,    GSS_INPUT_PULL  },  /* NOT USE */
    {GSS_TABLET_END,GSS_TABLET_END  }   /* GPIO Group Set End */
};

static void gpio_sleep_state(void)
{
	int i;

	for(i = 0; gpio_sleep_state_table[i][1] != GSS_TABLET_END; i++) 
    {
		switch(gpio_sleep_state_table[i][1]) 
        {
		case GSS_OUTPUT_LOW:
			__gpio_as_output(gpio_sleep_state_table[i][0]);
			__gpio_clear_pin(gpio_sleep_state_table[i][0]);
			break;
		case GSS_OUTPUT_HIGH:
			__gpio_as_output(gpio_sleep_state_table[i][0]);
			__gpio_set_pin(gpio_sleep_state_table[i][0]);
			break;
		case GSS_INPUT_NOPULL:
			__gpio_as_input(gpio_sleep_state_table[i][0]);
			__gpio_disable_pull(gpio_sleep_state_table[i][0]);
			break;
		case GSS_INPUT_PULL:
			__gpio_as_input(gpio_sleep_state_table[i][0]);
			__gpio_enable_pull(gpio_sleep_state_table[i][0]);
			break;
        default:
            break;
		}
	}
}

static void config_irq_wakeup(void)
{
 	 gpio_sleep_state();
// 	__intc_unmask_irq(IRQ_GPIO0);
	__intc_unmask_irq(IRQ_GPIO1);
	__intc_unmask_irq(IRQ_GPIO2);
// 	__intc_unmask_irq(IRQ_GPIO3);
	__intc_unmask_irq(IRQ_UART1);
	__intc_unmask_irq(IRQ_OTG);
	

}

static void jzsoc_do_sleep(unsigned long *ptr)
{
	unsigned char i;

	for(i = 0; i < GPIO_PORT_NUM; i++) {
		*ptr++ = REG_GPIO_PXINT(i);
		*ptr++ = REG_GPIO_PXMASK(i);
		*ptr++ = REG_GPIO_PXPAT1(i);
		*ptr++ = REG_GPIO_PXPAT0(i);
		*ptr++ = REG_GPIO_PXPEN(i);
	}
	config_irq_wakeup();
}

void save_gpio_regs2(void)
{
}

void restore_gpio_regs2(void)
{
}

static void jzsoc_do_resume(unsigned long *ptr)
{
	unsigned char i;
	for(i = 0; i < GPIO_PORT_NUM; i++) {
		REG_GPIO_PXINTS(i) = *ptr;
		REG_GPIO_PXINTC(i) = ~(*ptr++);
		REG_GPIO_PXMASKS(i) = *ptr;
		REG_GPIO_PXMASKC(i) = ~(*ptr++);
		REG_GPIO_PXPAT1S(i) = *ptr;
		REG_GPIO_PXPAT1C(i) = ~(*ptr++);
		REG_GPIO_PXPAT0S(i) = *ptr;
		REG_GPIO_PXPAT0C(i) = ~(*ptr++);
		REG_GPIO_PXPENS(i) = *ptr;
		REG_GPIO_PXPENC(i) = ~(*ptr++);
	}
}

unsigned int pm_firmware_new[] ={
#include "core_sleep.hex"
};


/* 将休眠、唤醒的执行代码加载到指定地址运行 */
void load_pm_firmware_new(unsigned int addr)
{
	void (*func)(unsigned int addr,unsigned int to);
	unsigned int firmware_size = sizeof(pm_firmware_new);

	if(firmware_size > TCSM_BANK_LEN * 1024)
		printf("WARN: firmware_size %d bigger than" \
		       "TCSM_BANK_LEN %d\n", firmware_size, TCSM_BANK_LEN * 1024);
	func = (void (*)(unsigned int,unsigned int))addr;
	memcpy((void *)addr,pm_firmware_new,firmware_size);
	func(addr,0);
}


struct sleep_param
{
	unsigned int  pm_core_enter;
	unsigned char pmu_i2c_scl;           //default 0xff
	unsigned char pmu_i2c_sda;           //default 0xff
	unsigned char pmu_addr;               //default 0xff
	unsigned char pmu_reg;                //default 0xff
	unsigned char pmu_register_val;

	unsigned char pmu_pin;               //default 0xff
	unsigned char pmu_pin_func;          //default 0xff
	unsigned char uart_id;          //default 0xff

	unsigned int  prev_resume_pc;  //ddr is self-reflash default 0xffffffff
	unsigned int  post_resume_pc;  //ddr is ok. default 0xffffffff
	unsigned int  prev_sleep_pc;   //after flush cache. default 0xffffffff
	unsigned int  post_sleep_pc;   //before wait. default 0xffffffff
};
struct sleep_save_register
{
	unsigned int lcr;
	unsigned int opcr;
	unsigned int sleep_voice_enable;
	unsigned int ddr_training_space[20];
};

static struct sleep_save_register s_reg;

struct sleep_param *sleep_param;

static int jz_pm_do_sleep(void)
{
    unsigned long clkgr0 =  INREG32(CPM_CLKGR0); 
	unsigned long icmr0 = INREG32(INTC_ICMR(0));
	unsigned long icmr1 = INREG32(INTC_ICMR(1));
	unsigned long sleep_gpio_save[5*(GPIO_PORT_NUM)];
	unsigned int val;
	unsigned int sr = spin_lock_irqsave();

	/* set SLEEP mode */
	OUTREG32(CPM_CLKGR0, 0);

    
	/* Mask all interrupts */
	OUTREG32(INTC_ICMSR(0), 0xffffffff);
	OUTREG32(INTC_ICMSR(1), 0xffffffff);


	REG_INTC_IMCR(0) = (1<<15) | (1<<21);
	REG_INTC_IMCR(1) = (1<<18);

	/* OUTREG32(INTC_ICMCR(0), 0xffffffff); */
	/* /\* unmask rtc interrupts *\/ */
	/* OUTREG32(INTC_ICMCR(1), 0x1); */
	/* Sleep on-board modules */
	jzsoc_do_sleep(sleep_gpio_save);

	/* 将休眠唤醒的执行代码放到 SLEEP_TCSM_SPACE的地址上执行，避免了在引导代码中做相应的处理 
	 * SLEEP_TCSM_SPACE是cpu的内部ram
     */
	load_pm_firmware_new(SLEEP_TCSM_SPACE);

	sleep_param = (struct sleep_param *)SLEEP_TCSM_SPACE;

	sleep_param->post_resume_pc = (unsigned int)restore_goto;
	sleep_param->uart_id = 0;

	memcpy(&s_reg.ddr_training_space,(void*)0x80000000,sizeof(s_reg.ddr_training_space));
	s_reg.opcr = INREG32(CPM_OPCR);
	s_reg.lcr = INREG32(CPM_LCR);

    /*
	 *   set OPCR.
	 */
    val = s_reg.opcr;
    val &= ~((1 << 25) | (1 << 22) | (0xfff << 8) | (1 << 7) | (1 << 6) | (1 << 4) | (1 << 3) | (1 << 2));
	val |= (1 << 31) | (1 << 30) | (1 << 25) | (1 << 23) | (0xfff << 8) | (0 << 4) | (1 << 3) | (1 << 2);
	OUTREG32(CPM_OPCR, val);

	val = s_reg.lcr;
	val &= ~3;
	val |= 1;
	val |= 0xff << 8;	/* power stable time */
	OUTREG32(CPM_LCR, val);

    /* 调用休眠代码cpu进入sleep状态 */
	mb();
	save_goto((unsigned int)sleep_param->pm_core_enter);
	mb();
    /* cpu从此唤醒 */
    
	memcpy((void*)0x80000000,&s_reg.ddr_training_space,sizeof(s_reg.ddr_training_space));
	dma_cache_wback_inv(0x80000000,sizeof(s_reg.ddr_training_space));

	OUTREG32(CPM_LCR, s_reg.lcr);
	OUTREG32(CPM_OPCR, s_reg.opcr);

	spin_unlock_irqrestore(sr);


	/* Restore interrupts */
	OUTREG32(INTC_ICMR(0), icmr0);
	OUTREG32(INTC_ICMR(1), icmr1);

	/* Resume on-board modules */
	jzsoc_do_resume(sleep_gpio_save);

    OUTREG32(CPM_CLKGR0, clkgr0);

	/* Restore Oscillator and Power Control Register */
	/* OUTREG32(CPM_OPCR, opcr); */
	return 0;
}

void jz_flush_cache_all(void)
{
	unsigned long addr;

	/* Clear CP0 TagLo */
	asm volatile ("mtc0 $0, $28\n\t"::);

	for (addr = K0BASE; addr < (K0BASE + 0x4000); addr += 32) {
		asm volatile (
				".set mips3\n\t"
				" cache %0, 0(%1)\n\t"
				".set mips0\n\t"
				:
				: "I" (Index_Writeback_Inv_D), "r"(addr));

		asm volatile (
				".set mips3\n\t"
				" cache %0, 0(%1)\n\t"
				".set mips0\n\t"
				:
				: "I" (Index_Store_Tag_I), "r"(addr));
	}

	asm volatile ("sync\n\t"::);

	asm volatile (
		".set	push\n\t"
		".set	noreorder\n\t"
		"lw	$0,%0\n\t"
		"nop\n\t"
		".set	pop"
		: /* no output */
		: "m" (*(int *)0xa0000000)
		: "memory");

	/* invalidate BTB */
	asm volatile (
		".set mips32\n\t"
		" mfc0 %0, $16, 7\n\t"
		" nop\n\t"
		" ori $0, 2\n\t"
		" mtc0 %0, $16, 7\n\t"
		" nop\n\t"
		".set mips0\n\t"
		:
		: "r"(addr));
}

//--------------------------------------------------------------------

/* Put CPU to IDLE mode */
void jz_pm_idle(void)
{
    unsigned int val;
    
	/* Mask all interrupts */
    OUTREG32(OST_OSTECR, 1);	
	
    val = INREG32(CPM_LCR);
    val &= ~3;	
    val |= 0xff << 8;	/* power stable time */	
    OUTREG32(CPM_LCR, val);
    
	asm volatile ("sync\n\t"::);
	
	asm volatile (
		".set	push\n\t"
		".set	noreorder\n\t"
		"lw	$0,%0\n\t"
		"nop\n\t"
		".set	pop"
		: /* no output */
	: "m" (*(int *)0xa0000000)
		: "memory");
	
	__asm__(
		".set\tmips3\n\t"
		"wait\n\t"
		"nop\n\t"
		"nop\n\t"
		".set\tmips0"
		);
    
    OUTREG32(OST_OSTESR,1);	
}

static int jz_pm_do_hibernate(void)
{
	printf("Put CPU into hibernate mode.\n");

	/* Mask all interrupts */
	OUTREG32(INTC_ICMSR(0), 0xffffffff);
	OUTREG32(INTC_ICMSR(1), 0xffffffff);

	/*
	 * RTC Wakeup or 1Hz interrupt can be enabled or disabled
	 * through  RTC driver's ioctl (linux/driver/char/rtc_jz.c).
	 */
	/* Set minimum wakeup_n pin low-level assertion time for wakeup: 100ms */
	rtc_write_reg(RTC_HWFCR, HWFCR_WAIT_TIME(100));

	/* Set reset pin low-level assertion time after wakeup: must  > 60ms */
	rtc_write_reg(RTC_HRCR, HRCR_WAIT_TIME(60));

	/* Scratch pad register to be reserved */
	rtc_write_reg(RTC_HSPR, HSPR_RTCV);

	/* clear wakeup status register */
	rtc_write_reg(RTC_HWRSR, 0x0);

	/* set wake up valid level as low  and disable rtc alarm wake up.*/
	//rtc_write_reg(RTC_HWCR,0x8);
	rtc_write_reg(RTC_HWCR,0x9);

	/* Put CPU to hibernate mode */
	rtc_write_reg(RTC_HCR, RTC_HCR_PD);

	//while (1)
	//	printf("We should NOT come here, please check the jz4775rtc.h!!!\n");

	return 0;
}

//--------------------------------------------------------
static int curLevel = 0;
extern int jz_dm_init(void);

void pm_init()
{
#if (DM == 1)
	printf("Driver Manager \r\n");
	jz_dm_init();
#endif
}

#define DETECT_LCD_EOF 0
/*
 level:
 0,1,2,3,-1;
 -1 is GetLevel;
 */

#define OP_PM_LOCK()
#define OP_PM_UNLOCK()
int jz_pm_control(int level)
{
    printf("level=%d\n",level);
	OP_PM_LOCK();
	if (level != -1) 
    {
		if (curLevel != level) 
        {
			if (level < 0) 
            {
				printf("the level is out of limit\n");
				OP_PM_UNLOCK();
				return -1;
			}
            else if(level == 0) 
            {
                printf("sleep\n");
				OP_PM_UNLOCK();
				curLevel = level;
				jz_pm_sleep();
				return 0;
			}
			else 
            {
//				jz_pllconvert(opt_pll[level]);
				curLevel = level;
				OP_PM_UNLOCK();
				return 0;
			}
		}
		else
			return curLevel;
	}
	else 
    {
		OP_PM_UNLOCK();
		return curLevel;
	}
}

//--------------------------------------------------------------------

typedef struct __tag_wake_type__
{
	void (*WakeUp)();
	void (*PreWakeUp)();
	unsigned int id;
} WakeUpType;

#define MAX_WakeUp_SOURCE 5
static WakeUpType WakeUpsource[MAX_WakeUp_SOURCE];
static unsigned char wcount = 0;

void PM_AddWakeUp(int id, void* preWakeUp, void* WakeUp)
{
	if (wcount < MAX_WakeUp_SOURCE) {
		WakeUpsource[wcount].id = id;
		WakeUpsource[wcount].PreWakeUp = preWakeUp;
		WakeUpsource[wcount].WakeUp = WakeUp;
		wcount++;
	}
}

static void PM_SetWakeUp(void)
{
	int i;
	for (i = 0; i < wcount; i++) {
		if (WakeUpsource[i].PreWakeUp) {
			printf("%s->%x\n", __FUNCTION__, WakeUpsource[i].id);
			WakeUpsource[i].PreWakeUp();
		}
	}
}

static void PM_WakeUp(void)
{
	int i;
	for (i = 0; i < wcount; i++) {
		if (WakeUpsource[i].WakeUp) {
			printf("%s->%x\n", __FUNCTION__, WakeUpsource[i].id);
			WakeUpsource[i].WakeUp();
		}
	}
}

//--------------------------------------------------------------------
/* Put CPU to SLEEP mode */
int jz_pm_sleep(void)
{
	PM_SetWakeUp();
	jz_pm_do_sleep();
// 	printf("jz_pm_do_sleep\n");
	PM_WakeUp();
	return 1;
}

//---------------------------------------------------------------------
//--------------------------------------------------------------------
typedef struct __tag_power_type__
{
	void (*pPowerDown)();
	void (*pPowerUp)();
	unsigned int id;
} PowerType;

#define MAX_Power_SOURCE 10
static PowerType PowerSource[MAX_Power_SOURCE];
static unsigned char w_powercount = 0;

void PM_AddPowerCtrl(int id, void* ppowerdown, void* ppowerup)
{
	if (w_powercount < MAX_Power_SOURCE) {
		PowerSource[w_powercount].id = id;
		PowerSource[w_powercount].pPowerDown = ppowerdown;
		PowerSource[w_powercount].pPowerUp = ppowerup;
		w_powercount++;
	}
}

static void PM_PowerDown(void)
{
	int i;
	for (i = 0; i < w_powercount; i++) {
		if (PowerSource[i].pPowerDown) {
			printf("%s->%x\n", __FUNCTION__, PowerSource[i].id);
			PowerSource[i].pPowerDown();
		}
	}
}

static void PM_PowerUp(void)
{
	int i;
	for (i = 0; i < w_powercount; i++) {
		if (PowerSource[i].pPowerUp) {
			printf("%s->%x\n", __FUNCTION__, PowerSource[i].id);
			PowerSource[i].pPowerUp();
		}
	}
}

/* Put CPU to HIBERNATE mode */
int jz_pm_hibernate(void)
{
	PM_PowerDown();
	return jz_pm_do_hibernate();
}
