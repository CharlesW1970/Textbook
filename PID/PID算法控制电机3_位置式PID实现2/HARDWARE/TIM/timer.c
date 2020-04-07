/*****************************************************************************************************************
�������ܣ� ʵ�ֶԶ�ʱ���ĳ�ʼ�������õ��Ķ�ʱ��Ϊ��ʱ��3��ͨ��1��ͨ��2���ֱ����PWM�����Ƶ��
          ʵ�ֱ������Ե���ٶȵĻ�ȡ������ӡ������������

���GPIO�� TIM3_CH1Handler --> PB4    TIM3_CH2Handler --> PB5

ռ�ձȣ�   compare1��TIM3ͨ��1��ռ�ձȣ�compare2��TIM3ͨ��2��ռ�ձ�

��ؽ����� ����ķ������ھ����Զ���װ��ֵ��ֵ��usΪ��λ����ռ�ձ� = compare / arr

A4950���ߣ���Ƭ��     A4950ģ��     
           5V    -->   VCC      
           GND   -->   GND
           PB4   -->   AN1
           GND   -->   AN2
           PB5   -->   BN1
           GND   -->   BN2
                       AOUT1   -->  ���1+
                       AOUT2   -->  ���1-
                       BOUT1   -->  ���2+
                       BOUT2   -->  ���2-
                       VM----->>>>> ��ӵ�Դ��7.6V---12V����Ҫ����
*****************************************************************************************************************/
#include "timer.h"
#include "led.h"
#include "encoder.h"
#include "pid.h"

TIM_HandleTypeDef  TIM3_PWM_Handler; //��ʱ��3���
TIM_OC_InitTypeDef TIM3_CH1Handler; //��ʱ��3ͨ��1���
TIM_OC_InitTypeDef TIM3_CH2Handler; //��ʱ��3ͨ��2���

//PWM �����ʼ��
//arr���Զ���װֵ psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{
    TIM3_PWM_Handler.Instance=TIM3;//��ʱ��3
    TIM3_PWM_Handler.Init.Prescaler= psc;//ʱ��Ԥ��Ƶϵ��
    TIM3_PWM_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM3_PWM_Handler.Init.Period=arr;//�Զ���װ��ֵ
    TIM3_PWM_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_PWM_Init(&TIM3_PWM_Handler);
    
    TIM3_CH1Handler.OCMode=TIM_OCMODE_PWM1;//PWM1ģʽ
    TIM3_CH1Handler.Pulse=arr;//���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ�
    TIM3_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW;//����Ƚϼ��Ե�
    HAL_TIM_PWM_ConfigChannel(&TIM3_PWM_Handler,&TIM3_CH1Handler,TIM_CHANNEL_1);//���� TIM3ͨ��1
    HAL_TIM_PWM_ConfigChannel(&TIM3_PWM_Handler,&TIM3_CH1Handler,TIM_CHANNEL_2);//���� TIM3ͨ��2
    HAL_TIM_PWM_Start(&TIM3_PWM_Handler,TIM_CHANNEL_1);//����PWMͨ��1
    HAL_TIM_PWM_Start(&TIM3_PWM_Handler,TIM_CHANNEL_2);//����PWMͨ��2
}

//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻ HAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM3_CLK_ENABLE();//����TIM3ʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();//����GPIOBʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_4 | GPIO_PIN_5;//PB4��PB5
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;//����Ϊ�������
    GPIO_Initure.Pull=GPIO_PULLUP;//����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;//����
    GPIO_Initure.Alternate=GPIO_AF2_TIM3; //PB ����Ϊ TIM3
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
} 
