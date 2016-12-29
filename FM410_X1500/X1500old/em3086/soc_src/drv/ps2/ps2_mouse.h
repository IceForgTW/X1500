#ifndef DRV_MOUSE_H_H_asjifijrf435345
#define DRV_MOUSE_H_H_asjifijrf435345


#define Mouse_Port_Data		(GPIO_GROUP_D + 26)
#define Mouse_Port_Clk		(GPIO_GROUP_D + 27)

typedef struct
{
	unsigned int L_key;				//��Ӧ�������(0,1)
	unsigned int R_key;				//��Ӧ�Ҽ�����
	unsigned int M_key;				//��Ӧ�м������
	unsigned int up;					//�����ƶ�(-8~7)
	unsigned int down;				//�����ƶ�
	unsigned int left;					//�����ƶ�
	unsigned int right;				//�����ƶ�
	unsigned int rollfront;			//�����ƶ���ǰ(-8~7)
	unsigned int rollback;				//�����ƶ����()
	unsigned int key4;
	unsigned int key5;

	unsigned int oldup;					//�����ƶ�(-8~7)
	unsigned int olddown;				//�����ƶ�
	unsigned int oldleft;					//�����ƶ�
	unsigned int oldright;				//�����ƶ�
	
	unsigned int KeyActFlag;			//�ǰ���������־
	unsigned int MoveActFlag;			//���ƶ�������־
}Ps2Mouse;

#define MouseClk_Clear_Flag		OUTREG32(A_GPIO_PXFLGC(Mouse_Port_Clk/32), 1 << (Mouse_Port_Clk%32));


#define MouseClk_Enable_Int			enable_irq(EIRQ_GPIO_BASE+Mouse_Port_Clk)
#define MouseClk_Dienable_Int		disable_irq(EIRQ_GPIO_BASE+Mouse_Port_Clk)

#define MouseClk_SetFall_Int 			__gpio_as_irq_fall_edge(Mouse_Port_Clk);			//�ʹ�����Ӧ
#define MouseClk_SetRise_Int			__gpio_as_irq_rise_edge(Mouse_Port_Clk);			//�ߴ�����Ӧ

#define MouseClk_SetHigh_Int 			__gpio_as_irq_high_level(Mouse_Port_Clk);			//�ߵ�ƽ��Ӧ
#define MouseClk_SetLow_Int			__gpio_as_irq_low_level(Mouse_Port_Clk);			//�͵�ƽ��Ӧ
/*
#define MouseClk_Input_No			gpio_set_dir(Mouse_Port_Clk,0,0);		//Data, input gpio mode ����
#define MouseClk_Input_Up			gpio_set_dir(Mouse_Port_Clk,0,0);		//Data, input gpio mode ����
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
