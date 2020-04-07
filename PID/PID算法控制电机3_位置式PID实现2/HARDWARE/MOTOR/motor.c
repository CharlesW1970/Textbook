#include "motor.h"
#include "timer.h"
#include "encoder.h"
#include "pid.h"
#include "led.h"
#include "delay.h"

int Moto1;
int Encoder;               //编码器的脉冲计数
int Target_position=11000;    //初始值是10000，目标值是11000

TIM_HandleTypeDef TIM2_Handler;

void TIM2_Init(u16 arr,u16 psc)
{
    TIM2_Handler.Instance=TIM2;
    TIM2_Handler.Init.Prescaler=psc;//时钟预分频系数
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数
    TIM2_Handler.Init.Period=arr;//自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);
       
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器2和定时器2更新中断：TIM_IT_UPDATE  
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断   
	}
}

void TIM2_IRQHandler(void)
{     
    if(TIM2->SR & 0x0001)//10ms定时器中断
    {       
        TIM2->SR&=~(1<<0);                            //清除定时器2中断标志位		 
        Encoder=Read_Encoder();                       //读取编码器的位置数据 初始值是10000，详见encoder.c 和encoder.h
        LED0=!LED0;                                   //LED闪烁;指示单片机正常运行	
        delay_ms(100);
        Moto1=Position_PID(Encoder,Target_position);  //位置PID控制器
        Xianfu_Pwm();                                 //PWM限幅
        Set_Pwm(Moto1);                               //赋值给PWM寄存器  
    }  
}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto1)
{      
	PWMA=myabs(moto1);
    PWMB=myabs(moto1);
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{	
	int Amplitude=8000;    //===PWM满幅是9000 限制在8000
    if(Moto1<-Amplitude) 
    {
        Moto1=-Amplitude; 
    }	
	if(Moto1>Amplitude)
    {
        Moto1=Amplitude;
    }	
}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	int temp;
	if(a<0) 
    {
        temp=-a;  
    }        
	else 
    {
        temp=a;
    }
	return temp;
}
