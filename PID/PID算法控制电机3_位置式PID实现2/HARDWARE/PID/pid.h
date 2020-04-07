#ifndef _PID_H
#define _PID_H
#include "sys.h"

#define PI 3.14159265
int TIM3_IRQHandler(void);  
int Position_PID (int Encoder,int Target);

#endif
