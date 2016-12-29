
#ifndef __GENERATE_HEAD__

#define __GENERATE_HEAD__


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
#ifndef BOOL
#define BOOL int
#endif


#ifndef  max
#define max(x,y) ((x)>(y)?(x):(y))
#endif  //#ifndef  max

#ifndef  min
#define min(x,y) ((x)<(y)?(x):(y))
#endif  //#ifndef  max

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef NULL
#define NULL	0
#endif

#define RED_LED		2
#define GREEN_LED	1
#define ALL_LED		(RED_LED | GREEN_LED)


//#define RAM_RESEVER_PIC	0x1ff00000



#define BSP_UNKNOW		0x00
#define BSP_EM3086_V1		0x01
#define BSP_HR21_V1              0x02

#define TIMEOUT_NOWAIT		0xffffffff
#define TIMEOUT_INFINITE	0

//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////


void led_on(int nled);

void led_off(int nled);

int get_key_state(void);

void Init_CRC32_Table(void);
unsigned int Get_CRC32(unsigned char *pdata, int len); 

char *get_bsp_string(void);

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

#endif			//#ifndef __GENERATE_HEAD__

