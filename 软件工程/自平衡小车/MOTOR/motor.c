#include "motor.h"

//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
//	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.6.7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	TIM_SetCompare1(TIM3,899);
	TIM_SetCompare2(TIM3,899);
}

/************控制电机的IO的初始化***************/
void CntlIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PB,PE端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //根据设定参数初始化GPIOB.5
}

/************控制电机的初始化***************/

void Motor_Init(void)
{
	CntlIO_Init();
	TIM3_PWM_Init(899,0);
}

/*****************PWM控制电机***************/

void PWM_Map(float pwm_left,float pwm_right)
{
	if(pwm_left >= 0)
		{
			// 控制左轮后转
	    PA1 = 0;
	    PA2 = 1;
			if(pwm_left > 899.0)
				TIM_SetCompare1(TIM3,0);
			else
				TIM_SetCompare1(TIM3,899 - (u16)pwm_left);
		}
		else
		{
			// 控制左轮前转
			PA1 = 1;
	    PA2 = 0;
			pwm_left = -pwm_left;
			if(pwm_left > 899.0)
				TIM_SetCompare1(TIM3,0);
			else
				TIM_SetCompare1(TIM3,899 - (u16)pwm_left);
		}
		if(pwm_right >= 0)
		{
			// 控制右轮后转
			PB1 = 0;
	    PB2 = 1;
			if(pwm_right > 899.0)
				TIM_SetCompare2(TIM3,0);
			else
				TIM_SetCompare2(TIM3,899 - (u16)pwm_right);
		}
		else
		{
			// 控制右轮前转
			PB1 = 1;
	    PB2 = 0;
			pwm_right = -pwm_right;
			if(pwm_right > 899.0)
				TIM_SetCompare2(TIM3,0);
			else
				TIM_SetCompare2(TIM3,899 - (u16)pwm_right);
		}
}	
