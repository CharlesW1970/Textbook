#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "encoder.h"
#include "pid.h"
#include "motor.h"

int main(void)
{
    HAL_Init();                     //��ʼ��HAL��    
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);
    uart_init(115200);
    LED_Init();
    TIM2_Init(100-1,9000-1);//10MS��һ���жϷ��������жϷ�������motor.c
    TIM3_PWM_Init(9000-1,0);//���10kHz��PWMƵ��
    TIM4_Encoder_Init(65535-1,0);
    	
	while(1)
	{	
        Read_Encoder();
        delay_ms(100);
    }    
}
