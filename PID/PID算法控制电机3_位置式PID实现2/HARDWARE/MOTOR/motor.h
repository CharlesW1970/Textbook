#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"

void Set_Pwm(int moto1);
void Xianfu_Pwm(void);
int myabs(int a);
void TIM2_Init(u16 arr, u16 psc);

#endif
