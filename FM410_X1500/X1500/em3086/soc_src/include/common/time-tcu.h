#ifndef __TIME_TCU_H__
#define __TIME_TCU_H__

void JZ_StartTicker(unsigned int TicksPerSec);
void Stop_PerformanceCounter(void);
int Init_PerformanceCounter(void);
unsigned long long   Get_PerformanceCounter(void);
void JZ_StopTimer(void);
void JZ_StartTimer(void);
void timer_delay(int chn, unsigned int us, void (*handler)(void));
void pwm_init(unsigned int chn);
void beep_frq(int freq, int msdelay, int volume);
void playmusic_ex(const char * music, int vol);
void playmusic(const char * music);
void Init_Wakeup_Timer(void);
void Set_Wakeup_timer(int ms);
int Get_Wakeup_timer_state(void);

void mdelay(unsigned int msec);
void udelay(unsigned int usec);

#endif
