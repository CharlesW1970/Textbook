#ifndef _ENCODER_H
#define _ENCODER_H
#include "sys.h"

void TIM4_Encoder_Init(u16 arr, u16 psc);
int Read_Encoder(void);

#endif
