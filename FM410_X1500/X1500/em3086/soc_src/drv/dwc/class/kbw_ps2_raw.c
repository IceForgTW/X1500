
#include "kbw_ps2.h"
#include <x1000.h>
#include <watchdog.h>
#include <time-tcu.h> 
//////////////////////////////////////////////////////////////////////////

#define PS2_CLK          (32*3+0)
#define PS2_DAT          (32*3+1)
#define PS2_EN            (32*3+4)
#define PC_CLKIN         (32*3+5)

//////////////////////////////////////////////////////////////////////////

typedef struct {
	int dat_out_high;
	int dat_out_low;
	int clk_out_high;
	int clk_out_low;
	int switch_ext_kbw;
	int switch_int_kbw;
	int dat_in_pin;
	int clk_in_pin;
	int cpu_pc_clk;
}PS2_PIN_STRUCT;

static int ps2_init_flag = 0;

static PS2_PIN_STRUCT _s_pin;

static int ClkDelay = 50;		//单位:us



//////////////////////////////////////////////////////////////////////////
static void SetClkPinIn(void)
{
    __gpio_as_input(PS2_CLK);	
}

static void SetClkPinOut(void)
{
    __gpio_as_output(PS2_CLK);	
}

static void SetDatPinIn(void)
{
    __gpio_as_input(PS2_DAT);	
}

static void SetDatPinOut(void)
{
    __gpio_as_output(PS2_DAT);	
}

static void SetEnPinOut(void)
{
    __gpio_as_output(PS2_EN);	
}

static void SetEnPinHigh(void)
{
	__gpio_set_pin(PS2_EN);
}

static void SetEnPinLow(void)
{
	__gpio_clear_pin(PS2_EN);
}

static void SetClkPinHigh(void)
{
	__gpio_set_pin(PS2_CLK);
}

static void SetClkPinLow(void)
{
	__gpio_clear_pin(PS2_CLK);
}

static void SetDatPinHigh(void)
{
	__gpio_set_pin(PS2_DAT);
}

static void SetDatPinLow(void)
{
	__gpio_clear_pin(PS2_DAT);
}


//if clk pin high level, return 1, low level return 0
static int GetClkPinLevel(void)
{
	return __gpio_get_pin(GPIO_GROUP_D+0);
}

//if dat pin high level, return 1, low level return 0
static int GetDatPinLevel(void)
{
	return __gpio_get_pin(GPIO_GROUP_D+1);
}


static int GetCpuDatPinLevel(void)//目前无clk检测线
{
    return  __gpio_get_pin(GPIO_GROUP_D+5);//暂定PD5
}


static void SetOutPinToInStat(void)
{
    SetClkPinIn();
    SetDatPinIn();   
}

//切换并接通外接PS2键盘口
static void SwitchToExtPS2Dev(void)
{
    SetOutPinToInStat();
    //SetEnPinOut(PS2_EN);
    SetEnPinLow();	
}

//关断外接PS2键盘口，以防干扰
static void SwitchToIntPS2Dev(void)
{
    //SetEnPinOut(PS2_EN);
    SetEnPinHigh();
}

//////////////////////////////////////////////////////////////////////////

int init_ps2_kbw_io(void)
{

	SetEnPinOut();

	SwitchToIntPS2Dev();
    
	ps2_raw_send_char(0xaa);
	ps2_raw_send_char(0xaa);
	ps2_raw_send_char(0xaa);
	ps2_raw_send_char(0xaa);
	ps2_raw_send_char(0xaa);
	ps2_raw_send_char(0xaa);
	ps2_raw_send_char(0xaa);
	ps2_raw_send_char(0xfa);
	SwitchToExtPS2Dev();
//	SetClkPinLow();
//	mdelay(20);
//	SetOutPinToInStat();
	return 0;
}


int StartPS2KbwSend(void)
{
    unsigned int lvcnt = 0;
    
    ResetWatchDog();

    while(lvcnt < 3000)//wait clk pin keep high level 3ms
    {			
        if(GetCpuDatPinLevel())
        {
            lvcnt ++;
            udelay(1);
        } 
        else
        {
            ResetWatchDog();
            lvcnt = 0;
        }
    }

    SwitchToIntPS2Dev();
    lvcnt = 0;
    while(lvcnt < 1000)
    {			//wait clk pin keep high level 1ms
        if(GetCpuDatPinLevel()){
            lvcnt ++;
            udelay(1);
        } 
        else 
        {
            ResetWatchDog();
            lvcnt = 0;
        }
    }

    return 0;
    
}

int StopPS2KbwSend(void)
{
	SwitchToExtPS2Dev();
	return 0;
}

int ps2_raw_send_char(const char chr)
{
    int lvi;
    int lvmb;		//mask
    int lvchk;		//check bit
    int lvCpuStat;
    int lverr = 0;
    int lvret = 0;
    int s=0;

    ResetWatchDog();
    
WaitBusFree_RawSend:
    SetOutPinToInStat();
    lvi = 0;

    //等待ps2总线空闲
    while (!GetCpuDatPinLevel())	
    {	
        if(lvi ++ < 1000)
            udelay(10);
        else
        {
            lvret = -1;
            goto exit_ps2_raw_send_char;
        }
    }

    if(lverr ++ > 20)				//错误太多，太久
    {
        lvret = -2;
        goto exit_ps2_raw_send_char;
    }

    lvi = 0;
    while (lvi < 100)
    {
        if (GetCpuDatPinLevel())
        {
            lvi ++;
            udelay(1);
        }
        else 
        {
            goto WaitBusFree_RawSend;
        }
    }
  
    SetDatPinOut();
    SetClkPinOut();

    SetClkPinHigh();
    SetDatPinHigh();
    udelay(6);

    //总线冲突
    if (!GetCpuDatPinLevel())
    {
        goto WaitBusFree_RawSend;
    }

    lvmb = 1;
    lvchk = 0;
    
    //发送起始位
    SetDatPinHigh();
    udelay(8);
    SetClkPinLow();//起始位0
    udelay(ClkDelay);

    while(lvmb <= 0x80)		//send data bit
    {
        SetClkPinHigh();//clk拉高
        udelay(ClkDelay - 8);
        if(chr & lvmb)//发送数据
        {
            SetDatPinHigh();
            lvchk ++;
        } 
        else
            SetDatPinLow();
        
        if (!GetCpuDatPinLevel())
        {
            goto WaitBusFree_RawSend;
        }
        
        lvmb = lvmb << 1;
        udelay(8);
        SetClkPinLow();
        udelay(ClkDelay);           
    }
    
    SetClkPinHigh();
    udelay(ClkDelay - 8);
    if(lvchk & 1)
        SetDatPinLow();
    else
        SetDatPinHigh();
    
    if (!GetCpuDatPinLevel())
    {
        goto WaitBusFree_RawSend;
    }

    udelay(8);
    SetClkPinLow();
    udelay(ClkDelay);

    SetClkPinHigh();			//send stop bit
    udelay(ClkDelay - 8);
    SetDatPinHigh();

    if (!GetCpuDatPinLevel())
    {
        goto WaitBusFree_RawSend;
    }

    udelay(8);
    SetClkPinLow();
    udelay(ClkDelay);

    SetOutPinToInStat();
	
    while (!GetCpuDatPinLevel());

exit_ps2_raw_send_char:
    SetOutPinToInStat();
    ResetWatchDog();
    return lvret;
    
}

#if 0
int ps2_raw_send_char(const char chr)
{
	int lvi;
	int lvmb;		//mask
	int lvchk;		//check bit
	int lvCpuStat;
	int lverr = 0;
	int lvret = 0;
	int s=0;

	ResetWatchDog();

	if(!ps2_init_flag) return 0;

	lvCpuStat = IrqDisable();

WaitBusFree_RawSend:
	SetOutPinToInStat();
	lvi = 0;
//	while((!GetClkPinLevel()) || (!GetDatPinLevel())){		//等待ps2总线空闲
	while (!GetCpuDatPinLevel())	{	
		if(lvi ++ < 1000)
			udelay(10);
		else
		{
			lvret = -1;
/*			UartSendStr("step1 timeout\r\n");*/
			goto exit_ps2_raw_send_char;
		}
	}

	if(lverr ++ > 20)				//错误太多，太久
	{
		lvret = -2;
		//IrqEnable(lvCpuStat);
		//ResetWatchDog();
/*		UartSendStr("step2 more err\r\n");*/
		goto exit_ps2_raw_send_char;
		//return -1;
	}

	lvi = 0;
	while (lvi < 100)
	{
	//if(GetClkPinLevel() && GetDatPinLevel()){
		if (GetCpuDatPinLevel()){
			lvi ++;
			udelay(1);
		}
		else {
/*			UartSendStr("step3 timeout\r\n");*/
			goto WaitBusFree_RawSend;
		}
	}

	SetClkPinHigh();
	SetDatPinHigh();
	udelay(6);
//	if(!GetClkPinLevel() || !GetDatPinLevel())			//总线冲突
	if (!GetCpuDatPinLevel())
	{
/*		UartSendStr("step4 timeout\r\n");*/
		goto WaitBusFree_RawSend;
	}
	lvmb = 1;
	lvchk = 0;
	
	SetDatPinLow();			//send start bit
	udelay(8);
	SetClkPinLow();
	udelay(ClkDelay);
	
	while(lvmb <= 0x80)		//send data bit
	{
		SetClkPinHigh();
		udelay(ClkDelay - 8);
		if(chr & lvmb)
		{
			SetDatPinHigh();
			lvchk ++;
		} else
			SetDatPinLow();
	//	if(!GetClkPinLevel()) {
	if (!GetCpuDatPinLevel()){
/*			UartSendStr("step5 host disable\r\n");*/
			goto WaitBusFree_RawSend;
		}
		lvmb = lvmb << 1;
		udelay(8);
		SetClkPinLow();
		udelay(ClkDelay);
	}

	SetClkPinHigh();			//send check bit
	udelay(ClkDelay - 8);
	if(lvchk & 1)
		SetDatPinLow();
	else
		SetDatPinHigh();
	//if(!GetClkPinLevel()){
	if (!GetCpuDatPinLevel()){
/*		UartSendStr("step6 host disable\r\n");*/
		goto WaitBusFree_RawSend;
	}
	udelay(8);
	SetClkPinLow();
	udelay(ClkDelay);

	SetClkPinHigh();			//send stop bit
	udelay(ClkDelay - 8);
	SetDatPinHigh();
//	if(!GetClkPinLevel() || !GetDatPinLevel()){
	if (!GetCpuDatPinLevel()){
/*		UartSendStr("step7 host dis\r\n");*/
		goto WaitBusFree_RawSend;
	}
	udelay(8);
	SetClkPinLow();
	udelay(ClkDelay);

	SetOutPinToInStat();
	//while(!GetClkPinLevel() || !GetDatPinLevel());
	while (!GetCpuDatPinLevel());
//	udelay(400);
//	SetClkPinHigh();			//Frame end and set system bus to free
exit_ps2_raw_send_char:
	SetOutPinToInStat();
	IrqEnable(lvCpuStat);
	ResetWatchDog();
	return lvret;
}
#endif


/*********************************************************************
			End of file
*********************************************************************/
