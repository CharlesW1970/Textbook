/*********************************************************************************************************
�������ܣ���ʱ��4������ģʽ�����ת��

����GPIO�� A�� --> PD12(TIM4_CH1)   B�� --> PD13(TIM4_CH2)

������壺 ������AB�������ÿתÿ������360������
*********************************************************************************************************/
#include "encoder.h"
#include "timer.h"
#include "led.h"

TIM_HandleTypeDef TIM4_Handler;//��ʱ��4���
TIM_Encoder_InitTypeDef TIM4_Encoder_Handler;//��ʱ��4������ģʽ���

//������ģʽ��ʼ������
void TIM4_Encoder_Init(u16 arr, u16 psc)
{
    TIM4_Handler.Instance=TIM4;//ѡ��ʱ��4
    TIM4_Handler.Init.Prescaler=psc;//ʱ��Ԥ��Ƶϵ��
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���
    TIM4_Handler.Init.Period=arr;//�Զ�װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
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
    
    HAL_TIM_Encoder_Start(&TIM4_Handler, TIM_CHANNEL_ALL);//�����������ж�
    HAL_TIM_Encoder_Start_IT(&TIM4_Handler, TIM_CHANNEL_ALL);//���������ж�
    TIM4->CNT=10000;
    
    __HAL_TIM_ENABLE(&TIM4_Handler);//ʹ���ж�
    __HAL_TIM_ENABLE_IT(&TIM4_Handler, TIM_IT_UPDATE);//ʹ�ܸ����ж�
}

//������ģʽ�ص�����
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIOD_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();
    
    GPIOD_Initure.Pin=GPIO_PIN_12 | GPIO_PIN_13;
    GPIOD_Initure.Mode=GPIO_MODE_AF_PP;
    GPIOD_Initure.Pull=GPIO_PULLUP;
    GPIOD_Initure.Speed=GPIO_SPEED_HIGH;
    GPIOD_Initure.Alternate=GPIO_AF2_TIM4;//PD����ΪTIM4ģʽ
    HAL_GPIO_Init(GPIOD, &GPIOD_Initure);
    
    HAL_NVIC_SetPriority(TIM4_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
    HAL_NVIC_EnableIRQ(TIM4_IRQn);          //����ITM4�ж�   
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��4�Ͷ�ʱ��4�����жϣ�TIM_IT_UPDATE   
}

void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Handler);
}

/**************************************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
����  ֵ���ٶ�ֵ
**************************************************************************/
int Read_Encoder()
{
    int Encoder_TIM;  
    Encoder_TIM = TIM4 -> CNT;
    printf("the Encoder capture is %d\r\n", Encoder_TIM);
	return Encoder_TIM;   
}
