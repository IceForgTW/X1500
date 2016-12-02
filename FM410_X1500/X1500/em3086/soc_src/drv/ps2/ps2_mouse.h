#ifndef DRV_MOUSE_H_H_asjifijrf435345
#define DRV_MOUSE_H_H_asjifijrf435345


#define Mouse_Port_Data		(GPIO_GROUP_D + 26)
#define Mouse_Port_Clk		(GPIO_GROUP_D + 27)

typedef struct
{
	unsigned int L_key;				//对应左键动作(0,1)
	unsigned int R_key;				//对应右键动作
	unsigned int M_key;				//对应中间键动作
	unsigned int up;					//向上移动(-8~7)
	unsigned int down;				//向下移动
	unsigned int left;					//向左移动
	unsigned int right;				//向右移动
	unsigned int rollfront;			//滚轮移动向前(-8~7)
	unsigned int rollback;				//滚轮移动向后()
	unsigned int key4;
	unsigned int key5;

	unsigned int oldup;					//向上移动(-8~7)
	unsigned int olddown;				//向下移动
	unsigned int oldleft;					//向左移动
	unsigned int oldright;				//向右移动
	
	unsigned int KeyActFlag;			//是按键动作标志
	unsigned int MoveActFlag;			//是移动动作标志
}Ps2Mouse;

#define MouseClk_Clear_Flag		OUTREG32(A_GPIO_PXFLGC(Mouse_Port_Clk/32), 1 << (Mouse_Port_Clk%32));


#define MouseClk_Enable_Int			enable_irq(EIRQ_GPIO_BASE+Mouse_Port_Clk)
#define MouseClk_Dienable_Int		disable_irq(EIRQ_GPIO_BASE+Mouse_Port_Clk)

#define MouseClk_SetFall_Int 			__gpio_as_irq_fall_edge(Mouse_Port_Clk);			//低触发响应
#define MouseClk_SetRise_Int			__gpio_as_irq_rise_edge(Mouse_Port_Clk);			//高触发响应

#define MouseClk_SetHigh_Int 			__gpio_as_irq_high_level(Mouse_Port_Clk);			//高电平响应
#define MouseClk_SetLow_Int			__gpio_as_irq_low_level(Mouse_Port_Clk);			//低电平响应
/*
#define MouseClk_Input_No			gpio_set_dir(Mouse_Port_Clk,0,0);		//Data, input gpio mode 悬浮
#define MouseClk_Input_Up			gpio_set_dir(Mouse_Port_Clk,0,0);		//Data, input gpio mode 上拉
#define MouseClk_OutputLow			gpio_set_dir(Mouse_Port_Clk,1,0);		//Data, output gpio mode
#define MouseClk_OutputHigh			gpio_set_dir(Mouse_Port_Clk,1,1);		//Data, output gpio mode
*/

									
#define MouseClk_OutputPxpe			OUTREG32(A_GPIO_PXPEC(Mouse_Port_Clk/32), 1 << (Mouse_Port_Clk%32));	
#define MouseData_OutputPxpe		OUTREG32(A_GPIO_PXPEC(Mouse_Port_Data/32), 1 << (Mouse_Port_Data%32));	

#define MouseClk_Input_No			__gpio_as_input(Mouse_Port_Clk);		
									//OUTREG32(A_GPIO_PXPEC(Mouse_Port_Clk/32), 1 << (Mouse_Port_Clk%32));
#define MouseClk_OutputHigh			__gpio_as_output(Mouse_Port_Clk);	\
									__gpio_set_pin(Mouse_Port_Clk);
#define MouseClk_OutputLow			__gpio_as_output(Mouse_Port_Clk);	\
									__gpio_clear_pin(Mouse_Port_Clk);

#define MouseData_Input_No			__gpio_as_input(Mouse_Port_Data);		
									//OUTREG32(A_GPIO_PXPEC(Mouse_Port_Data/32), 1 << (Mouse_Port_Data%32));
#define MouseData_OutputHigh		__gpio_as_output(Mouse_Port_Data);	\
									__gpio_set_pin(Mouse_Port_Data);
#define MouseData_OutputLow			__gpio_as_output(Mouse_Port_Data);	\
									__gpio_clear_pin(Mouse_Port_Data);

//#define MouseDataLevel 		(INREG32(A_GPIO_PXPIN(Mouse_Port_Data/32)) & (1 << (Mouse_Port_Data % 32))&0x00010000)>>16
//#define MouseClkLevel 		(INREG32(A_GPIO_PXPIN(Mouse_Port_Clk/32)) & (1 << (Mouse_Port_Clk % 32))&0x00020000)>>17

//#define MouseDataLevel 		(((INREG32(A_GPIO_PXPIN(Mouse_Port_Data/32)) & (1 << (Mouse_Port_Data % 32)))>>(Mouse_Port_Data%32))&0x1)
//#define MouseClkLevel 		(((INREG32(A_GPIO_PXPIN(Mouse_Port_Clk/32)) & (1 << (Mouse_Port_Clk % 32)))>>(Mouse_Port_Clk%32))&0x1)
#define MouseDataLevel 		(((INREG32(A_GPIO_PXPIN(Mouse_Port_Data/32))) >> (Mouse_Port_Data%32)) & 0x1)
#define MouseClkLevel 		(((INREG32(A_GPIO_PXPIN(Mouse_Port_Clk/32))) >> (Mouse_Port_Clk%32)) & 0x1) 

//-----------------------------------------


void Ps_init(void);
void HostSendData(int tdata);
Ps2Mouse GetMouseData();
void SetMouseTimer();
void MouseStart();

#endif
