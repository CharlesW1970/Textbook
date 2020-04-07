#include "key.h"
#include "led.h"
#include "delay.h"
#include "timer.h"

//������ʼ������
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�������GPIO_InitTypeDefΪ�ṹ������������stm32f4xx_hal_gpio.h
	//��ʼ��ʱ�ӣ����ݿ������ϰ���ԭ�����ͼ��������PA0��PC13��PH2��PH3������Ҫ��ʼ��GPIOA/GPIOC/GPIOHʱ�ӣ������stm32f4xx_hal_rcc.h
	__HAL_RCC_GPIOA_CLK_ENABLE();//����GPIOAʱ��
	__HAL_RCC_GPIOC_CLK_ENABLE();//����GPIOCʱ��
	__HAL_RCC_GPIOH_CLK_ENABLE();//����GPIOHʱ��
	
	GPIO_InitStructure.Pin=GPIO_PIN_0;//GPIOA_PIN0
	GPIO_InitStructure.Mode=GPIO_MODE_INPUT;//����ģʽ
	GPIO_InitStructure.Pull=GPIO_PULLDOWN;//��������
	GPIO_InitStructure.Speed=GPIO_SPEED_HIGH;//����
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin=GPIO_PIN_13;
	GPIO_InitStructure.Mode=GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull=GPIO_PULLUP;
	GPIO_InitStructure.Speed=GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin=GPIO_PIN_2 | GPIO_PIN_3;
	HAL_GPIO_Init(GPIOH,&GPIO_InitStructure);
}

u8 KEY_Scan(u8 mode)
{
	static u8 key_up=1; //�����ɿ���־
	if(mode==1)key_up=1; //֧������
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);
		key_up=0;
		if(KEY0==0) return KEY0_PRES;
		else if(KEY1==0) return KEY1_PRES;
		else if(KEY2==0) return KEY2_PRES;
		else if(WK_UP==1) return WK_UP_PRES;
	}
	else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1;
	return 0; //�ް�������
}		

void USE_KEY_Init(u8 key)
{
    key=KEY_Scan(0);
    switch(key)
	{
		case KEY0_PRES:
			 TIM_SetTIM3Compare1(100);
             TIM_SetTIM3Compare2(100);
			 break;
		case KEY1_PRES:
			 TIM_SetTIM3Compare1(200);
             TIM_SetTIM3Compare2(200);
			 break;
		case KEY2_PRES: 
			 TIM_SetTIM3Compare1(300);
             TIM_SetTIM3Compare2(300);
			 break;
		case WK_UP_PRES: 
			 TIM_SetTIM3Compare1(350);
             TIM_SetTIM3Compare2(350);
			 break;
	}
	delay_ms(10);
}
