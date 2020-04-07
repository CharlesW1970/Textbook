/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
#include "pid.h"
#include "encoder.h"
#include "timer.h"
#include "motor.h"

int Position_PID (int Encoder,int Target)
{ 	
    float Position_KP=80,Position_KI=0.1,Position_KD=50;
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=Encoder-Target;                                  //计算偏差
	Integral_bias+=Bias;	                                 //求出偏差的积分
	Pwm=Position_KP*Bias + Position_KI*Integral_bias + Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	Last_Bias=Bias;                                       //保存上一次偏差 
	return Pwm;                                           //增量输出
}
