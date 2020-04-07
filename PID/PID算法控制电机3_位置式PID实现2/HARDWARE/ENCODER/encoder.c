/*********************************************************************************************************
函数功能：定时器4编码器模式检测电机转速

测速GPIO： A相 --> PD12(TIM4_CH1)   B相 --> PD13(TIM4_CH2)

电机脉冲： 编码器AB相输出：每转每相各输出360个脉冲
*********************************************************************************************************/
#include "encoder.h"
#include "timer.h"
#include "led.h"

TIM_HandleTypeDef TIM4_Handler;//定时器4句柄
TIM_Encoder_InitTypeDef TIM4_Encoder_Handler;//定时器4编码器模式句柄

//编码器模式初始化函数
void TIM4_Encoder_Init(u16 arr, u16 psc)
{
    TIM4_Handler.Instance=TIM4;//选择定时器4
    TIM4_Handler.Init.Prescaler=psc;//时钟预分频系数
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数
    TIM4_Handler.Init.Period=arr;//自动装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    //HAL_TIM_Base_Init(&TIM4_Handler);
    
    TIM4_Encoder_Handler.EncoderMode=TIM_ENCODERMODE_TI12;
    TIM4_Encoder_Handler.IC1Filter=0;
    TIM4_Encoder_Handler.IC1Polarity=TIM_ICPOLARITY_RISING;
    TIM4_Encoder_Handler.IC1Prescaler=TIM_ICPSC_DIV1;
    TIM4_Encoder_Handler.IC1Selection=TIM_ICSELECTION_DIRECTTI;
    
    TIM4_Encoder_Handler.IC2Filter=0;
    TIM4_Encoder_Handler.IC2Polarity=TIM_ICPOLARITY_RISING;
    TIM4_Encoder_Handler.IC2Prescaler=TIM_ICPSC_DIV1;
    TIM4_Encoder_Handler.IC2Selection=TIM_ICSELECTION_DIRECTTI;
    HAL_TIM_Encoder_Init(&TIM4_Handler, &TIM4_Encoder_Handler);
    
    HAL_TIM_Encoder_Start(&TIM4_Handler, TIM_CHANNEL_ALL);//开启编码器中断
    HAL_TIM_Encoder_Start_IT(&TIM4_Handler, TIM_CHANNEL_ALL);//开启更新中断
    TIM4->CNT=10000;
    
    __HAL_TIM_ENABLE(&TIM4_Handler);//使能中断
    __HAL_TIM_ENABLE_IT(&TIM4_Handler, TIM_IT_UPDATE);//使能更新中断
}

//编码器模式回调函数
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIOD_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();
    
    GPIOD_Initure.Pin=GPIO_PIN_12 | GPIO_PIN_13;
    GPIOD_Initure.Mode=GPIO_MODE_AF_PP;
    GPIOD_Initure.Pull=GPIO_PULLUP;
    GPIOD_Initure.Speed=GPIO_SPEED_HIGH;
    GPIOD_Initure.Alternate=GPIO_AF2_TIM4;//PD复用为TIM4模式
    HAL_GPIO_Init(GPIOD, &GPIOD_Initure);
    
    HAL_NVIC_SetPriority(TIM4_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
    HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM4中断   
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器4和定时器4更新中断：TIM_IT_UPDATE   
}

void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Handler);
}

/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Encoder()
{
    int Encoder_TIM;  
    Encoder_TIM = TIM4 -> CNT;
    printf("the Encoder capture is %d\r\n", Encoder_TIM);
	return Encoder_TIM;   
}
