#ifndef __JZ4775_MENSA_CFG_H__
#define __JZ4775_MENSA_CFG_H__

#define OSC_CLOCK			(24 * 1000000)	
#define CFG_EXTAL			(24 * 1000000)		        /* EXTAL freq: 12MHz */
#define CFG_CPU_SPEED			(1008 * 1000000)		/* CPU clock */
#define CFG_DIV				5
#define CFG_MPLL_SPEED			CFG_CPU_SPEED
#define	CFG_HZ				(CFG_EXTAL/256) 	        /* Incrementer freq */
#define CFG_PLL1_FRQ        		(432 * 1000000)         	/* PLL1 clock */
#define DEVICE_CLOCK  			CFG_EXTAL

#define DEBUG_UART_BAUDRATE		57600

#define	MSC_BOOT_CHANNEL		0

#include "jz4775_common.h"


#define CPCCR_CFG	( (0 << CPCCR_CDIV_LSB)		\
					| (1 << CPCCR_HDIV_LSB)		\
					| (3 << CPCCR_H2DIV_LSB)	\
					| (3 << CPCCR_PDIV_LSB)		\
					| (3 << CPCCR_MDIV_LSB) 		\
					| CPCCR_MEM				\
					| CPCCR_CE					\
					| CPCCR_PCS)

#define	CPPCR_CFG	( CPCCR_M_N_OD						\
					| (0x20 << CPPCR0_PLLST_LSB)		\
					| CPPCR0_PLLEN)




//#define CPPCR_CFG   (CPCCR_M_N_OD|(0x20 << CPM_CPPCR_PLLST_BIT)| CPM_CPPCR_PLLEN)   
//#define	LCD_POWER_PIN		(GPIO_GROUP_A+ 26)
//#define   LCD_POWER_PIN     (UNUSED_GPIO_PIN)
#define   LCD_STANDBY_PIN  (32*1+30)
#define   LCD_STANDBY_LEVEL  (GPIO_HIGH_LEVEL)
//#define   LCD_RESET_PIN       (UNUSED_GPIO_PIN)

//#define	LCD_MODE_PIN		(UNUSED_GPIO_PIN)
//#define	LCD_DITHB_PIN	    	(UNUSED_GPIO_PIN)
//#define	LCD_POWER_ON_LEVEL	(GPIO_HIGH_LEVEL)
//#define LCD_ADD_PIN         	(GPIO_GROUP_F + 20)
#define LCD_PWM_PIN 		(GPIO_GROUP_E + 1) /*open backlight 100%*/

#if 0
#define LCD_LR_PIN	    	(GPIO_GROUP_F + 16)
#define	LCD_LR_LEVEL 		(GPIO_HIGH_LEVEL)
#define LCD_UD_PIN	    	(UNUSED_GPIO_PIN)
#define LCD_UD_LEVEL		(GPIO_HIGH_LEVEL)
#define	LCD_RESET_PIN		(UNUSED_GPIO_PIN)
#define	LCD_RESET_LEVEL		(GPIO_HIGH_LEVEL)
#endif


#define	LCD_BACKLIGHT_PWM_CHN	(1)

#define MAX_BRIGHTNESS_STEP	16				/* RT9365 supports 16 brightness step */
#define CONVERT_FACTOR		(256/MAX_BRIGHTNESS_STEP)	/* System support 256 brightness step */

#define __send_low_pulse(n)					\
do {								\
	unsigned int i;						\
	for (i = n; i > 0; i--)	{				\
		__gpio_clear_pin(LCD_BACKLIGHT_PIN);			\
		udelay(1);					\
		__gpio_set_pin(LCD_BACKLIGHT_PIN);			\
		udelay(3);					\
	}							\
} while (0)

#define __lcd_init_backlight(n)					\
do {								\
	unsigned int tmp = (n)/CONVERT_FACTOR + 1;		\
	__gpio_as_output(LCD_BACKLIGHT_PIN);				\
	__gpio_set_pin(LCD_BACKLIGHT_PIN);				\
	udelay(30);						\
	__send_low_pulse(MAX_BRIGHTNESS_STEP-tmp);		\
} while (0)


//----------------------------------------------------------------------------
//	Debug Configuration
//----------------------------------------------------------------------------
#define	DEBUG_UART_BASE			UART2_BASE
#define DEBUG_UART_BAUDRATE		57600

//#define	LCD_BACKLIGHT_PWM_CHN	(1)
/*DDR2 chips */
#include "./jz_mem_nand_configs/DDR3_H5TQ2G63BFR.h" /* 512M */
//#include "./jz_mem_nand_configs/MDDR_H5MS5122DFR-J3M.h" /* 512M */

/*UART BASE*/
#define CFG_UART_BASE  		UART2_BASE		/* Base of the UART channel */
#define CFG_UART_BAUDRATE	57600
#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

#define CFG_DCACHE_SIZE		(16 * 1024)		// Unit: Byte
#define CFG_ICACHE_SIZE		(16 * 1024)		// Unit: Byte
#define CFG_CACHELINE_SIZE	(32) 


//----------------------------------------------------------------------------
//	GPIO Configurations
//----------------------------------------------------------------------------
#define GPIO_UDC_DETE_PIN		(GPIO_GROUP_A + 16)	//usb detect

#define MMC1_POWER_PIN			(GPIO_GROUP_B + 3)
#define MMC1_CD_PIN			(GPIO_GROUP_B + 2)	//low is enable (GPIO_GROUP_F + 14)
//#define MMC1_CD_PIN			(GPIO_GROUP_A + 25)
#define MMC1_SD_WP			(UNUSED_GPIO_PIN)
#define CARD_INSERT_VALUE1		GPIO_LOW_LEVEL

#if (MMCTYPE == 1)
	#define MMC_BIT_MODE  MMC_4BIT_MODE
#elif (MMCTYPE == 2)
	//#define MMC_BIT_MODE  MMC_1BIT_MODE
	#define MMC_BIT_MODE  MMC_4BIT_MODE
#endif

//keypad
#define AD_KEY_INT_PIN			(GPIO_GROUP_E + 10)	//pe10

#define	KEY_VOL_DECR_PIN		(GPIO_GROUP_C + 31)	//sw3
#define	KEY_VOL_INCR_PIN		(GPIO_GROUP_D + 18)	//sw8
	
#define	KEY_AUD_PREV_PIN		(GPIO_GROUP_G + 15)  	//sw2
#define	KEY_AUD_NEXT_PIN		(GPIO_GROUP_D + 19) 	//sw7
#define	KEY_MENU_PIN			(GPIO_GROUP_D + 17)	//sw9

#define	KEY_DOWN_VALUE			( GPIO_LOW_LEVEL )

// The Special pin Key Down Value is NOT the KEY_DOWN_VALUE
#define	KEY_SPECIAL_PIN			(KEY_AUD_NEXT_PIN)
#define	KEY_SPECIAL_EXTRA_PIN		(KEY_AUD_PREV_PIN)	//as extra key pin when need two Special key pin


#define	CHARG_CTR_PIN			(GPIO_GROUP_F + 0)
#define	CHARG_CTR_LEVEL	    		(GPIO_HIGH_LEVEL)

#define	CHARG_STAT_PIN			(GPIO_GROUP_D + 27)
#define	CHARG_STAT_LEVEL		(GPIO_LOW_LEVEL)

////////////////////////////////////////////////////////
// SLCD
//#define SLCD_CS_PIN			(GPIO_GROUP_D + 22)
//#define	SLCD_CS_LEVEL		(GPIO_LOW_LEVEL)

//#define SLCD_RESET_PIN 		(UNUSED_GPIO_PIN)
//#define	SLCD_RESET_LEVEL	(GPIO_LOW_LEVEL)

//#define SLCD_BACKLIGHT_PWM_CHN	(2)

//Camera
#define CAM_RESET_PIN		(GPIO_GROUP_A+27) 
#define CAM_STANDBY_PIN		(GPIO_GROUP_B+6)  

// TV
#define TV_OUT_MODE_CVBS		0x00aa
#define TV_OUT_MODE_SVIDEO		0x00bb
#define TV_OUT_MODE_3DIV		0x00cc
#define TV_VIDEO_OUT_MODE		TV_OUT_MODE_CVBS
//#define TV_VIDEO_OUT_MODE		TV_OUT_MODE_SVIDEO

// Battery Monitor
//#define	DC_DETE_PIN		(GPIO_GROUP_F + 11)
#define	DC_DETE_LEVEL	(GPIO_HIGH_LEVEL)

//MTV
#define MTV_POWER_PIN		( GPIO_GROUP_A + 16 )
#define MTV_RESET_PIN		( GPIO_GROUP_A + 17 )
#define MTV_IRQ_PIN			( GPIO_GROUP_E + 18 )
#define MTV_CS_PIN				( GPIO_GROUP_E + 19 )
#define SMDDATAIO      	( GPIO_GROUP_A + 22 )
#define SMDRESETIO    	( GPIO_GROUP_A + 23 )
#define PWM_SMD        	( 3 )

//////////////////////////////////////////////////////////////////

#endif 
