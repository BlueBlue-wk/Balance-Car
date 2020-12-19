#include "exit.h"
#include "delay.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//外部中断0服务程序
void EXTIX_Init(void)
{
 
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		// 由于硬件原因，只采集了一个电机的编码器
		GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PORTB时钟
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB4、5

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

  //GPIOB.4 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


 //GPIOB.5	  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
		 //GPIOB.5	  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
		 //GPIOB.5	  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource2);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}
