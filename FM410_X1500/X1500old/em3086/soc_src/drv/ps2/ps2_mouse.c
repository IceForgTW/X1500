/********************** BEGIN LICENSE BLOCK ************************************
 *
 * JZ4755  mobile_tv  Project  V1.0.0
 * INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Copyright Ingenic Semiconductor Co. Ltd 2008. All rights reserved.
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
 *  Author:  <yyuan@ingenic.cn>
 *
 *  Create: 2009-12-29, by yyuan
 *
 *******************************************************************************
 */ 
#if PS2TYPE == 1

#include <jz4770.h> 
#include <bsp.h>
#include <os_api.h>
#include <function.h>
#include <threadprio.h>

  
#ifdef USE_MIDWARE
#include "midware.h"
#endif
#include "ps2_mouse.h"

  
#undef printf

//#define delay(x)                   os_TimeDelay(x*OS_TICKS_PER_SEC/1000)
#define MOUSETIMEID	97
//=========================================================================
#define		BUFNUMMAX	4
#define		MOUSEMSGMAX	4
//=====================================================
//for DataFlag
#define LeftBtn			0x01
#define RightBtn			0x02
#define MiddleBtn		0x04
#define XSignBit			0x10
#define YSignBit			0x20
#define XOverFlow		0x40
#define YOverFlow		0x80

//for IrqFlag
#define ReceiveFlag 	0x01
#define SendFlag		0x02
#define SendOver    	0x04
#define ReceiveOver    	0x08

//for mouse ACT
#define LEFTACT				0X09000000
#define MIDCT				0X0C000000
#define RIGHTACT			0X0A000000

#define UPMOVE				0X0800ff00	
#define DOWNMOVE			0X2800ff00	
#define LEFTMOVE			0x18ff0000
#define RIGHTMOVE			0x08ff0000
#define ROLLMOVE			0x080000ff
//移动最大，最小位移
#define UPMOVEMIX				0X08000100	
#define DOWNMOVEMIX			0X28008000	
#define LEFTMOVEMIX			0x18800000
#define RIGHTMOVEMIX			0x08010000
#define ROLLFRONTMIX			0x080000f1
#define ROLLBACKMIX			0x08000000

#define UPMOVEMAX				0X08007f00	
#define DOWNMOVEMAX			0X2800ff00	
#define LEFTMOVEMAX			0x18ff0000
#define RIGHTMOVEMAX			0x087f0000
#define ROLLFRONTMAX			0x080000ff
#define ROLLBACKMAX			0x0800000f

#define XMOVE				0x00ff0000
#define YMOVE				0X0000ff00
#define ROLLACTMOVE		0x000000ff

#define ROLLACT			0x080000FF
#define KEYACT4				0x08000010
#define KEYACT5				0x08000020	

//
#define MOUSEDATA		1				//接受到的是标鼠数据
#define ACKDATA		2					//是ACK应答信号

#define MOUSECHECKTIMER 0	 //检测数据是否正确timer号

int mousetimenum;
int mousecheckflag;

int mousesystemflag;
unsigned int iqrcount = 0;
int mousereceivetype = 1;

#define MOUSE_TASK_STK_SIZE   1024 * 10//4
// Main Task
#define PRIO_MOUSE_TASK		26

OS_TASK_STACK   TaskMouseStk[MOUSE_TASK_STK_SIZE];
OS_EVENT *MouseEvent = NULL;


int IrqFlag;
int DataReg;
int BitCount;
int WriteCount;
int DataBufNum = 0;
unsigned int ReceiveBuffer[BUFNUMMAX];
unsigned int MouseMsgBuffer[MOUSEMSGMAX];
int MouseMsgNum;
int RestartFlag;
int ReadCount;
int ErrDataFlag;
int Mousejishu;
int Mousetype;
static unsigned int deviceconet = 1;					//判断有无鼠标连上?
static unsigned int sendtimeoutflag = 0;			//发送数据超时
static unsigned int datarease = 0;

int irq = 0;
//-----------------------------------------
const int BitTab[]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,
                     0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};


Ps2Mouse MouseData;
//============================================================================
void MouseDump()
{
	printf("MouseData.L_key =  %x \n", MouseData.L_key);
	printf("MouseData.R_key =  %x \n", MouseData.R_key);
	printf("MouseData.M_key =  %x \n", MouseData.M_key);
	printf("MouseData.up =  %x \n", MouseData.up);
	printf("MouseData.down =  %x \n", MouseData.down);
	printf("MouseData.left =  %x \n", MouseData.left);
	printf("MouseData.right =  %x \n", MouseData.right);
	printf("MouseData.rollfront =  %x \n",MouseData.rollfront );
	printf("MouseData.rollback =  %x \n", MouseData.rollback);
	printf("MouseData.key4 =  %x \n", MouseData.key4);
	printf("MouseData.key5 =  %x \n",MouseData.key5 );
	printf("MouseData.oldup =  %x \n", MouseData.oldup);
	printf("MouseData.olddown =  %x \n", MouseData.olddown);
	printf("MouseData.oldleft =  %x \n", MouseData.oldleft);
	printf("MouseData.oldright =  %x \n", MouseData.oldright);
	printf("MouseData.KeyActFlag =  %x \n", MouseData.KeyActFlag);
	printf("MouseData.MoveActFlag =  %x \n\n", MouseData.MoveActFlag);
	return;
}
//============================================================================
Ps2Mouse GetMouseData()
{
	return MouseData;
}
//============================================================================
void MouseStart()
{
	iqrcount = 0;
	MouseClk_OutputHigh;		//Set clock send low
	MouseData_OutputHigh;
	udelay(15);
	MouseClk_OutputLow;
	udelay(300);
	MouseData_OutputLow;
	udelay(50);
	
	printf("MouseStart----\n");

	MouseClk_OutputHigh;
	MouseClk_Input_No;
	MouseData_Input_No;

	MouseClk_SetFall_Int;		//收数据是设置成 下降沿触发响应	
	//MouseClk_SetLow_Int;
	MouseClk_Enable_Int;
	MouseClk_Clear_Flag;
	
	//MouseData_OutputPxpe;
	//MouseData_Input_No;
	//MouseClk_Input_No;
	//udelay(15);

}
//============================================================================
void MouseTimeEvent()
{
	
	//MouseStart;
	//MouseClk_Enable_Int;
}
//============================================================================
void MouseTimeOpenInt()
{
	//MouseClk_Dienable_Int;
	//SetTimer(MOUSETIMEID, 1000 , MouseStart);
	//MouseClk_Enable_Int;
}
//============================================================================
int MouseGetMessenger(void)
{

}

unsigned int MouseClkLowToHightDelayFlag()
{
	int count = 500000;
	while(count --)
	{
		udelay(1);
		if(MouseClkLevel == 1)return 0;
	}
	sendtimeoutflag = 1;
	//deviceconet = 0;
	printf("ps2 clk time out 0--->1err!\n");
	return 1;
}
unsigned int MouseClkHightToLowDelayFlag()
{
	int count = 500000;
	while(count --)
	{
		udelay(1);
		if(MouseClkLevel == 0)return 0;
	}
	sendtimeoutflag = 1;
	//deviceconet = 0;
	printf("ps2 clk time out 1--->0err!\n");	
	return 1;
}
//============================================================================
unsigned char SendData(int tdata)
{
	int i, k = 0;		 
	int Pbit = 0;
	unsigned char ackstart = 0;
	unsigned char ackdata = 0;
	unsigned char ackp = 0;
	unsigned char ackend = 0;

	if((deviceconet ==  1)&&(sendtimeoutflag == 0))
	{
		MouseClk_Dienable_Int;			//关中断
		MouseClk_Clear_Flag;			//清标志
		DataReg = tdata;
		printf("\nSendData zfq Set DataReg %x\n",DataReg);
		MouseClk_OutputHigh;		//Set clock send low
		MouseData_OutputHigh;
		udelay(10);

		MouseClk_OutputLow;
		udelay(200);
		MouseData_OutputLow;
		udelay(20);
		MouseClk_OutputHigh;
		MouseClk_Input_No;

		ackstart = 0;
		
		//等待设备拉低时钟线	
		if(MouseClkLowToHightDelayFlag())return 0;
		udelay(15);
		//MouseClk_OutputLow;
		  
		for(i = 0; i < 8; i++)         //低位在前，一次发送8个数据位
		{	
			if(MouseClkHightToLowDelayFlag())return 0;				//等待设备释放时钟线
			//printf("00 %x , %x\n", MouseClkLevel, MouseDataLevel);
			udelay(10);
			if(DataReg&BitTab[i])
			{
				MouseData_OutputHigh;
				ackdata |= BitTab[i];
				Pbit ++;
			}
			else  
			{
				MouseData_OutputLow;
				ackdata &= (~BitTab[i]);
				//ackdata = ackdata&0x7f;
			}
			//printf("DataReg = %x, ackdata = %x\n", DataReg, ackdata);
			if(MouseClkLowToHightDelayFlag())return 0;
			udelay(10);
		}	 

		if(MouseClkHightToLowDelayFlag())return 0;		
		udelay(10);
		if(!(Pbit %2))                             //发送校验位，奇校验
		{
			MouseData_OutputHigh;
			Pbit ++;
			ackp = 1;
		}
		else  
		{	
			ackp = 0;
			MouseData_OutputLow;
		}
		if(MouseClkLowToHightDelayFlag())return 0;
		udelay(10);

		if(MouseClkHightToLowDelayFlag())return 0;		
		udelay(15);
		MouseData_OutputHigh;                                 //发送停止位
		udelay(10);
		ackend = 1;
		if(MouseClkLowToHightDelayFlag())return 0;
		udelay(10);
		
		if(MouseClkHightToLowDelayFlag())return 0;		
		udelay(10);
		if(MouseClkLowToHightDelayFlag())return 0;
		
		return ackdata;	
	}
}
//============================================================================
void HostSendData(int tdata) 
{
	unsigned char ret = 0;
	ret  = SendData(tdata);
	//printf("ret ack = %x \n", ret);
	if(tdata != ret) 
	{
		printf("send commond again:   %x", tdata);
		sendtimeoutflag = 0;
		ret  = SendData(tdata);
		if(tdata != ret)			//再次发送也没有发送成功，就当该设备无效
		{
			printf("\n***this ps2 device err!!!\n");
			deviceconet = 0;	
		}
	} 
	udelay(500);							//延时时间不能太短
	mousereceivetype = ACKDATA;	
/*	
	MouseData_Input_No;
	MouseClk_Input_No;
	MouseClk_SetFall_Int;
	MouseClk_Enable_Int;
	MouseClk_Clear_Flag; 
*/	
	return;
}
	   
void SetMouseTimer(void);

//===================================================================
void mousetime_isr()
{
	//MouseClk_OutputHigh;
	printf("mousetime_isr----\n");
	MouseClk_Input_No;
	MouseClk_SetFall_Int;		//收数据是设置成 下降沿触发响应	
	MouseClk_Enable_Int;	
}
//============================================================================
void SetMouseTimer()
{
	JZ_StartTimerEx(MOUSECHECKTIMER,100000,mousetime_isr);// 1ms
}
//============================================================================
void MouseTimerErrPro()
{
	MouseClk_Dienable_Int;		//关中断
	
	MouseClk_Clear_Flag;		//清标志
	MouseClk_SetFall_Int;		//设置下降触发

	BitCount = 0;
	WriteCount = 0;
	ReadCount = 0;
	DataBufNum = 0 ;
	mousetimenum = 0;
	ErrDataFlag = 1;	
}
//============================================================================
void MouseTimerReturnPro()
{
	BitCount = 0;
	WriteCount = 0;
	ReadCount = 0;
	DataBufNum = 0 ;
	mousetimenum = 0;
	ErrDataFlag = 0;
	mousecheckflag = 0;
	MouseClk_Clear_Flag;		//清标志
	MouseClk_SetFall_Int;		//设置下降触发
	MouseClk_Enable_Int;		//开中断
}
 //============================================================================
int MouseTimerCheck(int *PacketReg)
{	
	int temph = PacketReg[0];
	int tempm = PacketReg[1];
	int tempe = PacketReg[2];
	int tempf = PacketReg[3];
//	if(DataBufNum>=Mousetype)
//	{ 
//		DataBufNum -= Mousetype;
//	}
//	else return 1;
	if((!(temph&0x010))|| ((temph&0x100)) || ((temph&0x80)))
	{											//Allways1,YOverFlow ,XOverFlow		
		return 1;
	}
	else  
	{
		PacketReg[0] = (temph>>1)&0xff ;
		PacketReg[1] = (tempm>>1)&0xff ;
		PacketReg[2] = (tempe>>1)&0xff ;
		if( Mousetype==4 ) PacketReg[3] = (tempf>>1)&0xff ;		
	} 
	return 0;	
}
//============================================================================
void MouseSetMsgBuffer(int *PacketReg)
{
	if(MouseMsgNum<=MOUSEMSGMAX-4)
	{
		MouseMsgBuffer[MouseMsgNum] = PacketReg[0];
		MouseMsgBuffer[MouseMsgNum+1] = PacketReg[1];
		MouseMsgBuffer[MouseMsgNum+2] = PacketReg[2];
		MouseMsgBuffer[MouseMsgNum+3] = PacketReg[3];
		MouseMsgNum = 4; 
	}
}
//============================================================================

void MouseIrqHandle(void)
{
	MouseClk_Dienable_Int;
	//MouseClk_OutputLow;
	//unsigned long flags = spin_lock_irqsave();
	if(MouseClkLevel == 0)
	{
		udelay(15);
		if(MouseDataLevel)
		{
			DataReg |= BitTab[iqrcount];
		}
		else
		{
			DataReg &= (~BitTab[iqrcount]);
		}
		iqrcount ++;
		irq ++;
		if(iqrcount >= 11)
		{
			MouseClk_OutputLow;
			os_SemaphorePost(MouseEvent);
			//printf("\nDataReg = %x\n", DataReg);
			iqrcount	= 0;
		}
		else
		{
			MouseClk_Enable_Int;
		}
	}
	else
	{	
		MouseClk_Enable_Int;
	}
	//spin_unlock_irqrestore(flags);
}

void MouseIRQAInit(void)
{
	MouseClk_SetFall_Int;//下降触发响应
	MouseClk_Clear_Flag;
	request_irq(EIRQ_GPIO_BASE + Mouse_Port_Clk, MouseIrqHandle, 0);
	MouseClk_Dienable_Int;
	MouseClk_Clear_Flag; 
} 
 
//-----------------------------------------------------------

int TestMouseData(int data)
{
	int temp ,t1,i,tt , par = 0;
	temp = data ;
	if(data < 0x400 || data >= 0x800) return 1;
	
	temp = temp & 0x000003fe;
	temp = temp >>1;
	for(i = 0; i <9; i++)
	{
		if(temp&BitTab[i])
		{
			par++;
		}	
	}
	if(par&0x01)
	{
		data = (data & 0x1fe)>>1;
		return 0;
	}
	return 1;
}

//-------------------------
/*
extern int InitMouse(void);
void InitMouseFirst()
{
	int i;
	int err;
	for(i=0;i<5;i++)
	{
		err = InitMouse();
		if(err) break;
	}
	if(err)
	{
		Mousetype = 4;
		mousesystemflag = 0x8000;
		R_INT_KECON &= ~POL_EXTA_Trigger;		//设置成falling响应
		R_INT_KECON |= INT_EXTA_Flag ;			//清标志
		R_INT_KECON |= INT_Enable_EXTA ;
		SetMouseTimer();				
	}
}
*/
//============================================================================
int MouseDataCheck(int *data)
{
	int temp ,t1,i,tt , par = 0;
	temp = *data ;
	//if(data < 0x600 || data >= 0x800) return 1;
	
	temp = temp & 0x000003fe;
	temp = temp >>1;
	//printf("temp = %x\n", temp);
	for(i = 0; i < 9; i++)
	{
		if(temp&BitTab[i])
		{
			par++;
		}	
	}
	if(par&0x01)
	{
		*data = (*data & 0x1fe)>>1;
		//printf(" data = %x\n", *data);
		return 0;
	}
	printf("par err!\n\n");
	return 1;
}
//============================================================================
void MouseRest()
{
	printf("MouseRest----------------\n");
	DataBufNum = 0;
	//irq = 0;
	iqrcount = 0;
	DataReg = 0;
	mousereceivetype = MOUSEDATA;
	ReceiveBuffer[0] = 0xff;
	ReceiveBuffer[1] = 0xff;
	ReceiveBuffer[2] = 0xff;
	ReceiveBuffer[3] = 0xff;		
	//HostSendData(0xff);	
	//HostSendData(0xf4);
}
//============================================================================
void	 MouseDataDivision(int *pdata)
{
	int i;
	unsigned int data = 0;
	unsigned int data1 = 0; 
	data |= *pdata;
	data = data << 8;
	pdata++;	
	data |= *pdata;
	data = data << 8;
	pdata++;	
	data |= *pdata;
	data = data << 8;
	pdata++;	
	data |= *pdata;

	printf("32data = %08x\n", data);
	data1 = data;
	MouseData.oldup = MouseData.up;				
	MouseData.olddown = MouseData.down;
	MouseData.oldleft = MouseData.left;
	MouseData.oldright = MouseData.right;
	if(((data & LEFTACT) >>24) == 0x09)
	{
		MouseData.L_key  = 1;				//对应左键动作(0,1)
	}
	else
	{
		MouseData.L_key = 0;				
	}
	if(((data & RIGHTACT) >> 24) == 0x0a)
	{
		MouseData.R_key = 1;			//对应右键动作
	}
	else
	{
		MouseData.R_key = 0;			
	}

	if(((data & MIDCT) >> 24) == 0x0c)
	{
		MouseData.M_key = 1;				//对应中间键动作
	}
	else
	{
		MouseData.M_key = 0;				
	}
///////////////
	if((UPMOVEMIX <= (data & UPMOVE)) && ((data & UPMOVE) <= UPMOVEMAX))
	{
		MouseData.up = (data & YMOVE)  >> 8;			//向上移动
	}
	else
	{
		MouseData.up = 0;
	}
	if((DOWNMOVEMIX <= (data & DOWNMOVE)) && ((data & DOWNMOVE) <= DOWNMOVEMAX))
	{
		MouseData.down = (0x100 - ((data & YMOVE) >> 8));			//向下移动		
	}	
	else
	{
		MouseData.down = 0;
	}
	if((LEFTMOVEMIX <= (data & LEFTMOVE)) && ((data & LEFTMOVE) <= LEFTMOVEMAX))
	{
		MouseData.left  = (0x100 - ((data & XMOVE) >> 16));			//向左移动	
	}
	else
	{
		MouseData.left  = 0;
	}
	if((RIGHTMOVEMIX <= (data & LEFTMOVE)) && ((data & LEFTMOVE) <= RIGHTMOVEMAX))
	{
		MouseData.right  = (data & XMOVE) >> 16;			//向右移动(-8~7)		
	}	
	else
	{
		MouseData.right = 0;
	}
	///滚轮
	if((ROLLBACKMIX <= (data & ROLLMOVE)) && ((data & ROLLMOVE) <= ROLLFRONTMAX))
	{
		if((ROLLFRONTMIX <= (data & ROLLMOVE)) && ((data & ROLLMOVE) <= ROLLFRONTMAX))
		{
			MouseData.rollfront  = (0X100 - (data & ROLLACTMOVE));			//
			MouseData.rollback = 0;
		}
		
		if((ROLLBACKMIX <= (data & ROLLMOVE)) && ((data & ROLLMOVE) <= ROLLBACKMAX))
		{
			MouseData.rollfront  = 0;
			MouseData.rollback = (data & ROLLACTMOVE);
		}
	}
	else
	{
		MouseData.rollfront  = 0;
		MouseData.rollback = 0;
	}
	MouseData.key4 = ((data & KEYACT4) >> 4) & 0x01;
	MouseData.key5 = ((data & KEYACT5) >> 5) & 0x01;

	if((MouseData.L_key) || (MouseData.R_key) || (MouseData.M_key) || (MouseData.key4) || (MouseData.key5))
	{
		MouseData.KeyActFlag = 1;			//是按键动作标志
	}else
	{
		MouseData.KeyActFlag = 0;			//
	}
	if((MouseData.up) || (MouseData.down) || (MouseData.left) || (MouseData.right) ||(MouseData.rollfront) ||(MouseData.rollback))
	{
		MouseData.MoveActFlag = 1;			//是移动动作标志
	}else
	{
		MouseData.MoveActFlag = 0;			//
	}
	//MouseData.MoveActFlag = 1;			
	
	MouseDump();
}
//============================================================================
void MouseDataDeal( int *data)
{
	unsigned int temp[BUFNUMMAX];
	int kk = 10;
	int ret = MouseDataCheck(data);
	if((ret == 1)||(datarease > 0))
	{
		if(datarease > 0)datarease --;
		MouseRest();
		return;
	}
	ReceiveBuffer[DataBufNum] = *data;
	DataBufNum++;	
	//printf("                        data = %x\n", *data);
	//printf("00irq = %d, ReceiveBuffer[%d]= %x, %x, %x, %x\n",
	//	irq, DataBufNum, ReceiveBuffer[0], ReceiveBuffer[1], ReceiveBuffer[2], ReceiveBuffer[3]);
	if((ReceiveBuffer[0] == 0x00) && (ReceiveBuffer[1] == 0x08) && (ReceiveBuffer[2] == 0x00) && (ReceiveBuffer[3] == 0x00))
	{
		datarease = 1;
		return;
	}
	if((ReceiveBuffer[0] == 0x00) && (ReceiveBuffer[1] == 0x00) && (ReceiveBuffer[2] == 0x08) && (ReceiveBuffer[3] == 0x00))
	{
		datarease = 2;
		return;		
	}
	if((ReceiveBuffer[0] == 0x00) && (ReceiveBuffer[1] == 0x00) && (ReceiveBuffer[2] == 0x00) && (ReceiveBuffer[3] == 0x08))
	{
		datarease = 3;
		return;		
	}
	if(DataBufNum  > (BUFNUMMAX-1))
	{
		//printf("11irq = %d, ReceiveBuffer[%d]= %x, %x, %x, %x\n",
			//irq, DataBufNum, ReceiveBuffer[0], ReceiveBuffer[1], ReceiveBuffer[2], ReceiveBuffer[3]);
		DataBufNum = 0;		
		irq = 0;
		temp[0] = ReceiveBuffer[0];
		temp[1] = ReceiveBuffer[1];
		temp[2] = ReceiveBuffer[2];
		temp[3] = ReceiveBuffer[3];
		MouseDataDivision(temp);	
		ReceiveBuffer[0] = 0xff;
		ReceiveBuffer[1] = 0xff;
		ReceiveBuffer[2] = 0xff;
		ReceiveBuffer[3] = 0xff;	
	}
	
}
//============================================================================
unsigned int MouseQueryRead()
{
	int i = 0;
	while(MouseClkLevel==0);    //等待设备拉低时钟线
	while(MouseClkLevel==1);
}
//============================================================================
void MouseTask(void *data)
{
	unsigned char err;
	while(1)
	{
		os_SemaphorePend(MouseEvent, 0, &err);
		//printf("DataReg = %x\n", DataReg);
		unsigned int data = DataReg;
		MouseDataDeal(&data);
		DataReg = 0;
		udelay(50);
		if(mousereceivetype==ACKDATA)
		{
			printf("ACKDATA---=%x\n", data);
			MouseRest();
		}		
		
		MouseData_Input_No;
		MouseClk_Input_No;
		MouseClk_SetFall_Int;
		//MouseClk_SetLow_Int;
		MouseClk_Enable_Int;
		MouseClk_Clear_Flag;
		//if(MouseClkLevel == 0);
		//printf("leveldata--%d , iqrcount = %d\n", MouseDataLevel, iqrcount);
	}
 
}
//============================================================================
void mouse_sys_init()
{
	mousesystemflag = 0x8000;
}

 //============================================================================
void MouseThread()
{
	OS_TASK_STACK *ptos;
	OS_TASK_STACK *pbos;
	unsigned int  size;

	//os_malloc_completed = 1;
	//os_init_completed = 1;
	
   	MouseEvent = os_SemaphoreCreate(0);
	
	ptos        = &TaskMouseStk[MOUSE_TASK_STK_SIZE - 1];
	pbos        = &TaskMouseStk[0];
	size        = MOUSE_TASK_STK_SIZE;
	os_TaskCreateEx(MouseTask,
                   (void *)0,
                   ptos,
                   PRIO_MOUSE_TASK,
                   PRIO_MOUSE_TASK,
                   pbos,
                   size,
                   (void *)0,
                   OS_TASK_CREATE_EX_STK_CHK | OS_TASK_CREATE_EX_STK_CLR);
}
//============================================================================
void Ps2FunctionInitial()
{
	MouseIRQAInit();
//	MouseClk_SetFall_Int;					//设置成falling响应
//	MouseClk_Clear_Flag;					//清标志
		
	IrqFlag = ReceiveFlag;
	mousesystemflag = 0;						
	BitCount = 0;
	WriteCount=0;
	ReadCount = 0;
	RestartFlag = 0;
	ErrDataFlag = 0 ;

	MouseData.L_key = 0;
	MouseData.R_key = 0;
	MouseData.M_key = 0;
	MouseData.up = 0;
	MouseData.down = 0;		
	MouseData.left = 0;
	MouseData.right = 0;
	MouseData.rollfront = 0;
	MouseData.rollback = 0;
	MouseData.key4 = 0;
	MouseData.key5 = 0;
	MouseData.oldup = MouseData.up;
	MouseData.olddown = MouseData.down;
	MouseData.oldleft = MouseData.left;
	MouseData.oldright = MouseData.right;	
	MouseData.KeyActFlag = 0;
	MouseData.MoveActFlag = 0;

	sendtimeoutflag = 0;
	deviceconet = 1;
	MouseMsgNum = 0;
	Mousetype = 3;
}
//============================================================================

void MouseInitCommond()
{
	HostSendData(0xff);	
	//udelay(400);
	HostSendData(0xff);	
	//udelay(400);
	HostSendData(0xff);	
	//udelay(400);
	
	HostSendData(0xf3);	
	//udelay(400);	
	HostSendData(0xc8);				//数据
	//udelay(400);

	HostSendData(0xf3);	
	//udelay(400);	
	HostSendData(0x64);				//数据
	//udelay(400);

	HostSendData(0xf3);	
	//udelay(400);	
	HostSendData(0x50);				//数据
	//udelay(400);
	

	HostSendData(0xf2);	
	//udelay(400);
	HostSendData(0xf3);	
	//udelay(400);
	HostSendData(0x0a);	
	//udelay(400);
	HostSendData(0xf2);	
	//udelay(400);	
	HostSendData(0xf2);	
	//udelay(400);	
	HostSendData(0xe8);	
	//udelay(400);	
	HostSendData(0x03);	
	//udelay(400);	
	HostSendData(0xe6);	
	//udelay(400);	
	HostSendData(0xf3);	
	//udelay(400);	
	HostSendData(0x28);	
	//udelay(400);	
	HostSendData(0xf4);	
	//udelay(400);	
	MouseData_Input_No;
	MouseClk_Input_No;
	MouseClk_SetFall_Int;
	MouseClk_Enable_Int;
	MouseClk_Clear_Flag; 	
}
//============================================================================
void Ps_init(void)
{   
	//mouse_sys_init();	
	MouseThread();	
	Ps2FunctionInitial();
	printf("Zfq_Mouse_Init \n");
	MouseClk_Dienable_Int;			//关中断
	MouseInitCommond();
	//console(10);
	//HostSendData(0xf4);	
	//udelay(400);
	//MouseStart();
	//MouseTimeOpenInt();
	//SetMouseTimer();
	//while(1);
	return ;
}

#endif
  

