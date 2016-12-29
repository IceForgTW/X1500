#ifndef __PM_H__
#define __PM_H__

void pm_init();
void PM_AddWakeUp(int id, void* preWakeUp, void* WakeUp);
void jz_pm_idle(void);
int jz_pm_sleep(void);
int jz_pm_hibernate(void);
int jz_pm_control(int level);
void PM_AddPowerCtrl(int id, void* ppowerdown, void* ppowerup);

#endif
