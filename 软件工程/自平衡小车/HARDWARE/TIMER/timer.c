#include "timer.h"
#include "balancecar.h"
#include "led.h"
#include "oled.h"
#include "usart3.h"
#include "encoder.h"

//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//定时器 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

extern vu16 USART3_RX_STA;
extern u16 USART3_Cont;
extern u16 BalanceCar_Cnt;
extern u16 Display_Cnt;

//通用定时器4中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
							 
}

extern u16 Encode10_time,Encode11_time;
extern u16 Encode20_time,Encode21_time;
//定时器3中断服务程序
void TIM4_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
    	//***********the counter of usart3****************//
				if(USART3_Cont > 0)
					USART3_Cont++;
				if(USART3_Cont == 100)
				{
					USART3_RX_STA |= 1 << 15; //标记接收完成
					USART3_Cont = 0;
				}     
		  //***********the counter of Balance Car************//
				if(BalanceCar_Cnt > 0)
						BalanceCar_Cnt++;
		  //***********the counter of OLED*******************//
				if(Display_Cnt > 0)
						Display_Cnt++;
		  //***********the counter of encoder************//
				if(Encode10_time > 0)
					Encode10_time++;
				if(Encode11_time > 0)
					Encode11_time++;
				if(Encode20_time > 0)
					Encode20_time++;
				if(Encode21_time > 0)
					Encode21_time++;
		}
}


