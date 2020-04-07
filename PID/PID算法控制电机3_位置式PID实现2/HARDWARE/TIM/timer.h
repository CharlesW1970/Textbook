#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

#define AIN2   PBout(4)
#define AIN1   PBout(5)
#define PWMA   TIM3->CCR1
#define PWMB   TIM3->CCR2

void TIM3_PWM_Init(u16 arr,u16 psc);

#endif
