#ifndef EM3000_CFG_H
#define EM3000_CFG_H
/* 
********************************************************************************************************* 
*                                      TASK STACK SIZES 
********************************************************************************************************* 
*/
#define TASK_KEY_PRIO       3 //1
#define TASK_IDLE_PRI       80
#define TASK_DWC_PRIO       5
#define TASK_CIM_PRIO       10
#define TASK_PWM_PRIO       11
#define TASK_SENDDATAROUTE_PRIO       12
// #define OS_TASK_TMR_PRIO		27

#define TASK_MAIN_STK_SIZE      1024*128 
#define TASK_KEY_STK_SIZE       1024
#define TASK_DWC_STK_SIZE       1024 * 2   
#define TASK_PWM_STK_SIZE 	    0x1000

/* 设备使用的硬件定时器列表 */
#define TCU_TIMER_PWM0         0       // PWM使用
#define TCU_TIMER_WAKEUP	   1       // 定时唤醒使用
#define TCU_TIMER_2            2       // 未使用
#define TCU_TIMER_LIGHT        3       // 补光灯使用
#define TCU_TIMER_LED          4       // LED

#define ENCRTION_TEXT_ADD   0x20000
#define PUBLIC_KEYS_ADD     0x21000
#define CPUID_LEN 16
#endif

