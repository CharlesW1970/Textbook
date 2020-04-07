/*****************************************************************************************************************
函数功能： 实现对定时器的初始化，所用到的定时器为定时器3，通道1和通道2，分别输出PWM波控制电机
          实现编码器对电机速度的获取，并打印到串口助手上

电机GPIO： TIM3_CH1Handler --> PB4    TIM3_CH2Handler --> PB5

占空比：   compare1：TIM3通道1的占空比，compare2：TIM3通道2的占空比

相关解析： 输出的方波周期就是自动重装载值的值（us为单位），占空比 = compare / arr

A4950接线：单片机     A4950模块     
           5V    -->   VCC      
           GND   -->   GND
           PB4   -->   AN1
           GND   -->   AN2
           PB5   -->   BN1
           GND   -->   BN2
                       AOUT1   -->  电机1+
                       AOUT2   -->  电机1-
                       BOUT1   -->  电机2+
                       BOUT2   -->  电机2-
                       VM----->>>>> 外接电源，7.6V---12V，需要共地
*****************************************************************************************************************/
#include "timer.h"
#include "led.h"
#include "encoder.h"
#include "pid.h"

TIM_HandleTypeDef  TIM3_PWM_Handler; //定时器3句柄
TIM_OC_InitTypeDef TIM3_CH1Handler; //定时器3通道1句柄
TIM_OC_InitTypeDef TIM3_CH2Handler; //定时器3通道2句柄

//PWM 输出初始化
//arr：自动重装值 psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{
    TIM3_PWM_Handler.Instance=TIM3;//定时器3
    TIM3_PWM_Handler.Init.Prescaler= psc;//时钟预分频系数
    TIM3_PWM_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM3_PWM_Handler.Init.Period=arr;//自动重装载值
    TIM3_PWM_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_PWM_Init(&TIM3_PWM_Handler);
    
    TIM3_CH1Handler.OCMode=TIM_OCMODE_PWM1;//PWM1模式
    TIM3_CH1Handler.Pulse=arr;//设置比较值,此值用来确定占空比
    TIM3_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW;//输出比较极性低
    HAL_TIM_PWM_ConfigChannel(&TIM3_PWM_Handler,&TIM3_CH1Handler,TIM_CHANNEL_1);//配置 TIM3通道1
    HAL_TIM_PWM_ConfigChannel(&TIM3_PWM_Handler,&TIM3_CH1Handler,TIM_CHANNEL_2);//配置 TIM3通道2
    HAL_TIM_PWM_Start(&TIM3_PWM_Handler,TIM_CHANNEL_1);//开启PWM通道1
    HAL_TIM_PWM_Start(&TIM3_PWM_Handler,TIM_CHANNEL_2);//开启PWM通道2
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被 HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM3_CLK_ENABLE();//开启TIM3时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();//开启GPIOB时钟
    
    GPIO_Initure.Pin=GPIO_PIN_4 | GPIO_PIN_5;//PB4和PB5
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;//复用为推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;//高速
    GPIO_Initure.Alternate=GPIO_AF2_TIM3; //PB 复用为 TIM3
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
} 
