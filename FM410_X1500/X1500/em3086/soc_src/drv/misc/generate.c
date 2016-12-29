
#include "generate.h"
#include "x1000.h"


//////////////////////////////////////////////////////////////////////////


int get_bsp_ver(void)
{
	static int _sver = BSP_UNKNOW;
	if(_sver != BSP_UNKNOW) return _sver;
	{
		int lvi = 0;

            __gpio_as_input(32*0 + 9);
            __gpio_as_input(32*0 + 10);
            __gpio_as_input(32*1 + 0);

            lvi = __gpio_get_pin(GPIO_GROUP_A+10);
// 		pxa_gpio_mode(GPIO_IN | 80);
// 		pxa_gpio_mode(GPIO_IN | 109);
// 		pxa_gpio_mode(GPIO_IN | 110);
// 		pxa_gpio_mode(GPIO_IN | 113);
// 		
 		lvi = ( __gpio_get_pin(GPIO_GROUP_A+10)) << 2;
 		lvi |= ( __gpio_get_pin(GPIO_GROUP_A+9)) << 1;
 		lvi |= ( __gpio_get_pin(GPIO_GROUP_B+0));

		switch (lvi)
		{
		case 0x03:
 			_sver = BSP_HR21_V1;
            printf("t1\n");
			break;
 		case 0x07:
 			_sver =  BSP_EM3086_V1;
			printf("t2\n");
 			break;
        case 0x05:
			printf("t3\n");
			break;
// 		case 0x08:
// 			_sver = BSP_MB2033_V1;
// 			break;
// 		case 0x0c:
// 			_sver = BSP_MB2037_V1;
// 			break;
// 		case 0x0d:
// 			_sver = BSP_MB2037_V2;
// 			break;
		default:
			//_sver = BSP_MB_V1;
			break;
		}
	}
	return _sver;
}

char *get_bsp_string(void)
{
	int nver = get_bsp_ver();
	switch (nver)
	{
	    case BSP_EM3086_V1:	return "EM3086";
        case BSP_HR21_V1:   return "HR21";
	    case BSP_UNKNOW:	return "Unknow bsp ver";
	}
	return "Unknow bsp ver";
}

static void trigger_handle(unsigned int arg)
{
	__gpio_ack_irq(GPIO_GROUP_A+22);

	if(!__gpio_get_pin(GPIO_GROUP_A+22))			//下降沿
	{
		__gpio_as_irq_rise_edge(GPIO_GROUP_A+22);	//改成上升沿中断
	}
	else
	{
		__gpio_as_irq_fall_edge(GPIO_GROUP_A+22);	//改成下降沿中断
	}
}

static void irtrigger_handle(unsigned int arg)
{
	__gpio_ack_irq(GPIO_GROUP_B + 1);

	if (!__gpio_get_pin(GPIO_GROUP_B + 1))			//下降沿
	{
		__gpio_as_irq_rise_edge(GPIO_GROUP_B + 1);	//改成上升沿中断
	}
	else
	{
		__gpio_as_irq_fall_edge(GPIO_GROUP_B + 1);	//改成下降沿中断
	}
}

static void wakeupKey_handle(unsigned int arg)
{
	__gpio_ack_irq((GPIO_GROUP_B+31));
}

void init_gpio(void)
{
	//LED
	__gpio_as_output(GPIO_GROUP_B+22);
// 	__gpio_disable_pull(GPIO_GROUP_C+21);
	__gpio_set_pin(GPIO_GROUP_B+22);
// 	__gpio_set_pin(GPIO_GROUP_C+21);

	//KEY
	__gpio_as_input(GPIO_GROUP_A+22);
	request_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+22), trigger_handle, 0);  
 	//disable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+22));
	__gpio_as_irq_fall_edge(GPIO_GROUP_A+22);//改成下降沿中断

	//wake up key 
	__gpio_as_irq_fall_edge(GPIO_GROUP_B+31);
	request_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_B+31), wakeupKey_handle, 0);

	//IR Trigger
	__gpio_as_input(GPIO_GROUP_B + 1);
	request_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_B + 1), irtrigger_handle, 0);
	//disable_irq(EIRQ_GPIO_BASE + (GPIO_GROUP_A+22));
	__gpio_as_irq_fall_edge(GPIO_GROUP_B + 1);//改成下降沿中断
}

void led_on(int nled)
{
//	if(nled & RED_LED) __gpio_clear_pin(GPIO_GROUP_B+22);
  	if(nled & GREEN_LED) 
         __gpio_clear_pin(GPIO_GROUP_B+22);

}

void led_off(int nled)
{
//	if(nled & RED_LED)  __gpio_set_pin(GPIO_GROUP_B+22);
  	if(nled & GREEN_LED) 
          __gpio_set_pin(GPIO_GROUP_B+22);

}

int get_key_state(void)
{  
//	return __gpio_get_pin(GPIO_GROUP_A + 22);

	if ((__gpio_get_pin(GPIO_GROUP_A + 22) == 0) || (__gpio_get_pin(GPIO_GROUP_B + 1) == 0) )
	{
		return 0;
	} 
	else
	{
		return 1;
	}

}

static unsigned long crc32_table[256];  // Lookup table array 
static BOOL bCrc32Init = FALSE;
static unsigned long 
Reflect(unsigned long ref, unsigned char ch) 
{
	// Used only by Init_CRC32_Table(). 
	
	unsigned long value = 0; 
	
	// Swap bit 0 for bit 7 
	// bit 1 for bit 6, etc. 
	unsigned int i;
	for(i = 1; i < (ch + 1); i++) 
	{ 
		if(ref & 1) 
			value |= 1 << (ch - i); 
		ref >>= 1; 
	} 
	return value; 
} 

void Init_CRC32_Table(void) 
{
	// Call this function only once to initialize the CRC table. 
	
	// This is the official polynomial used by CRC-32 
	// in PKZip, WinZip and Ethernet. 
	unsigned long ulPolynomial = 0x04c11db7; 
	
	// 256 values representing ASCII character codes. 
	int i, j;
	for(i = 0; i <= 0xFF; i++) 
	{ 
		crc32_table[i] = Reflect(i, 8) << 24; 
		for (j = 0; j < 8; j++) 
			crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1UL << 31) ? ulPolynomial : 0); 
		crc32_table[i] = Reflect(crc32_table[i], 32); 
	} 
	bCrc32Init = TRUE;
} 

unsigned int Get_CRC32(unsigned char *pdata, int len) 
{
	// Pass a text string to this function and it will return the CRC. 
	
	// Once the lookup table has been filled in by the two functions above, 
	// this function creates all CRCs using only the lookup table. 
	
	// Be sure to use unsigned variables, 
	// because negative values introduce high bits 
	// where zero bits are required. 
	
	// Start out with all bits set high. 
	unsigned long ulCRC = 0xffffffff; 

	if (bCrc32Init == FALSE)
		Init_CRC32_Table();
	
	// Perform the algorithm on each character 
	// in the string, using the lookup table values. 
	while(len--) 
		ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF) ^ (unsigned char)(*pdata++)]; 
	// Exclusive OR the result with the beginning value. 
	return ulCRC ^ 0xffffffff; 
} 



// 
// static BOOL bPowerLedOn = FALSE;
// void SetPowerLEDOn(BOOL b)
// {
// 	bPowerLedOn = b;
// 	
// 	
// 	PowerStatue_PowerOn();
// }
// 
// BOOL GetPowerLedSetting(void)
// {
// 	return bPowerLedOn;
// }


#if 0

#define FREQ_416		0
#define FREQ_312		1
#define FREQ_208		2
#define FREQ_104		3

extern void __ChangeFrequency(void);

void SetPowerFrequency(int nFreqMode, BOOL bVoltage)
{
	uchar volt; 
// 	int t1, t2;
// 	char b[100];

// 	t1 = OSCR;
/*
	CCCR_A				EQU		1 << 25
	CCCR_N2_520			EQU		5 << 7
	CCCR_N2_416			EQU		4 << 7
	CCCR_N2_312			EQU		3 << 7
	CCCR_N2_104			EQU		2 << 7
	CCCR_L				EQU		16
	CCCR_L_104			EQU		8
	
	  CCCR_520_BUS208_MEM104		EQU		(CCCR_N2_520 + CCCR_L)
	  CCCR_416_BUS208_MEM104		EQU		(CCCR_N2_416 + CCCR_L)
	  CCCR_312_BUS208_MEM104		EQU		(CCCR_N2_312 + CCCR_L)
	  CCCR_208_BUS208_MEM104		EQU		(CCCR_N2_104 + CCCR_L)
	  CCCR_104_BUS104_MEM104		EQU		(CCCR_N2_104 + CCCR_L_104 + CCCR_A)
	  //520M, Vcore = 1.40V,  0xd
	  //416M, Vcore = 1.35V,	0xc
	  //312M, Vcore = 1.25V,	0xa
	  //208M, Vcore = 1.20V,	0x9
	  //104M, Vcore = 0.90V,	0x3; 0.95v, 0x4; 1.00v , 0x5

  */
	switch(nFreqMode)
	{
	case FREQ_416:
		CCCR = (4 << 7) | 16 ;	//CCCR_416_BUS208_MEM104
		volt = 0xc;
		break;
	case FREQ_312:
		CCCR = (3 << 7) | 16 ;	//CCCR_312_BUS208_MEM104
 		volt = 0xa;
//		volt = 0x9;
		break;
	case FREQ_208:
		CCCR = (2 << 7) | 16 ;	//CCCR_208_BUS208_MEM104
 		volt = 0x9;
//		volt = 0x8;
		break;
	case FREQ_104:
		CCCR = (2 << 7) | 8 | (1 << 25);	//CCCR_104_BUS104_MEM104
 		volt = 0x5;
//		volt = 0x4;
// 		volt = 0x3;
		break;
	default:
		return;
	}


	if (bVoltage)
	{
		while(PVCR&PVCR_VCSA);
		PVCR = 0x34;
		PCMD(0) = 0x23 | PCMD_MBC;
		PCMD(1) = volt | PCMD_LC;
		PCFR = PCFR_FVC | PCFR_PI2CEN;
	}

	__ChangeFrequency();
	while((CCSR&0x39f) != (CCCR&0x39f));

// 	t2 = OSCR;
// 	sprintf(b, "t: %d \r\n", (t2-t1)*100/325);
// 	UartSendStr(b);

}

#endif




