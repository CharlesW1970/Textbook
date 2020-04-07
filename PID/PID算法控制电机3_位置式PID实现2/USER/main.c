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
    HAL_Init();                     //初始化HAL库    
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);
    uart_init(115200);
    LED_Init();
    TIM2_Init(100-1,9000-1);//10MS进一次中断服务函数，中断服务函数在motor.c
    TIM3_PWM_Init(9000-1,0);//输出10kHz的PWM频率
    TIM4_Encoder_Init(65535-1,0);
    	
	while(1)
	{	
        Read_Encoder();
        delay_ms(100);
    }    
}
