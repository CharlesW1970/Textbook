#include "motor.h"
#include "timer.h"
#include "encoder.h"
#include "pid.h"
#include "led.h"
#include "delay.h"

int Moto1;
int Encoder;               //���������������
int Target_position=11000;    //��ʼֵ��10000��Ŀ��ֵ��11000

TIM_HandleTypeDef TIM2_Handler;

void TIM2_Init(u16 arr,u16 psc)
{
    TIM2_Handler.Instance=TIM2;
    TIM2_Handler.Init.Prescaler=psc;//ʱ��Ԥ��Ƶϵ��
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���
    TIM2_Handler.Init.Period=arr;//�Զ�װ��ֵ
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM2_Handler);
       
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����жϣ�TIM_IT_UPDATE  
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM2ʱ��
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM2�ж�   
	}
}

void TIM2_IRQHandler(void)
{     
    if(TIM2->SR & 0x0001)//10ms��ʱ���ж�
    {       
        TIM2->SR&=~(1<<0);                            //�����ʱ��2�жϱ�־λ		 
        Encoder=Read_Encoder();                       //��ȡ��������λ������ ��ʼֵ��10000�����encoder.c ��encoder.h
        LED0=!LED0;                                   //LED��˸;ָʾ��Ƭ����������	
        delay_ms(100);
        Moto1=Position_PID(Encoder,Target_position);  //λ��PID������
        Xianfu_Pwm();                                 //PWM�޷�
        Set_Pwm(Moto1);                               //��ֵ��PWM�Ĵ���  
    }  
}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int moto1)
{      
	PWMA=myabs(moto1);
    PWMB=myabs(moto1);
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{	
	int Amplitude=8000;    //===PWM������9000 ������8000
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
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
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
