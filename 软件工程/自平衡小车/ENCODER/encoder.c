#include "encoder.h"
#include "exit.h"

u16 Encode10_cnt = 0;  // 电机一的编码器圈数计数器
u16 Encode11_cnt = 0;  // 电机一的编码器圈数计数器
u16 Encode10_time,Encode11_time; // 记录电机转一圈的时间
float car_directon = -1;   // 标记电机的方向

u16 Encode20_cnt = 0;  // 电机一的编码器圈数计数器
u16 Encode21_cnt = 0;  // 电机一的编码器圈数计数器
u16 Encode20_time,Encode21_time; // 记录电机转一圈的时间
float car_directon1 = -1;   // 标记电机的方向

//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	if(Encode20==1)	 	
	{		
		// 此处判断电机的转向：（考虑两个霍尔器件相邻小于半圆的原因）
		// 正转 ：电机2的霍尔1比霍尔0先触发
		// 反转 ：电机2的霍尔1比霍尔0后触发
		if(Encode20_time > Encode21_time / 2)
			car_directon1 = -1;
		else
			car_directon1 = 1;
		Encode21_time = 1;
		Encode21_cnt  += 1;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

//外部中断3服务程序 
void EXTI2_IRQHandler(void)
{
	if(Encode21==1)	 	
	{		
		Encode20_time = 1;
		Encode20_cnt  += 1;
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}


//外部中断3服务程序 
void EXTI4_IRQHandler(void)
{
	if(Encode11==1)	 	
	{		
		// 此处判断电机的转向：
		// 正转 ：电机1的霍尔1比霍尔0先触发
		// 反转 ：电机1的霍尔1比霍尔0后触发
		if(Encode10_time > Encode11_time / 2) 
			car_directon = 1;
		else
			car_directon = -1;
		Encode11_time = 1;
		Encode11_cnt  += 1;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

//外部中断5服务程序 
void EXTI9_5_IRQHandler(void)
{
	if(Encode10==1)	 	 
	{				 
		Encode10_time = 1;
		Encode10_cnt  += 1;
	}	
	EXTI_ClearITPendingBit(EXTI_Line5); 
}

/*
作者：楠风
时间：2020.12.05
功能：编码器初始化
*/
void Encoder_Init(void)
{
	EXTIX_Init();
}

/*
作者：楠风
时间：2020.12.05
功能：读取编码器的数据得到电机的转速
*/
void Encode_Read(ENCODER* Encoder)
{
	Encoder->Speed_Left = (float)Encode11_cnt * car_directon;
	Encoder->Speed_Right = (float)Encode21_cnt * car_directon1;
	Encode11_cnt = 0;
	Encode10_cnt = 0;
	Encode21_cnt = 0;
	Encode20_cnt = 0;
	
}
