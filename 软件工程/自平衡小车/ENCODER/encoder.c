#include "encoder.h"
#include "exit.h"

u16 Encode10_cnt = 0;  // ���һ�ı�����Ȧ��������
u16 Encode11_cnt = 0;  // ���һ�ı�����Ȧ��������
u16 Encode10_time,Encode11_time; // ��¼���תһȦ��ʱ��
float car_directon = -1;   // ��ǵ���ķ���

u16 Encode20_cnt = 0;  // ���һ�ı�����Ȧ��������
u16 Encode21_cnt = 0;  // ���һ�ı�����Ȧ��������
u16 Encode20_time,Encode21_time; // ��¼���תһȦ��ʱ��
float car_directon1 = -1;   // ��ǵ���ķ���

//�ⲿ�ж�0������� 
void EXTI0_IRQHandler(void)
{
	if(Encode20==1)	 	
	{		
		// �˴��жϵ����ת�򣺣���������������������С�ڰ�Բ��ԭ��
		// ��ת �����2�Ļ���1�Ȼ���0�ȴ���
		// ��ת �����2�Ļ���1�Ȼ���0�󴥷�
		if(Encode20_time > Encode21_time / 2)
			car_directon1 = -1;
		else
			car_directon1 = 1;
		Encode21_time = 1;
		Encode21_cnt  += 1;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

//�ⲿ�ж�3������� 
void EXTI2_IRQHandler(void)
{
	if(Encode21==1)	 	
	{		
		Encode20_time = 1;
		Encode20_cnt  += 1;
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}


//�ⲿ�ж�3������� 
void EXTI4_IRQHandler(void)
{
	if(Encode11==1)	 	
	{		
		// �˴��жϵ����ת��
		// ��ת �����1�Ļ���1�Ȼ���0�ȴ���
		// ��ת �����1�Ļ���1�Ȼ���0�󴥷�
		if(Encode10_time > Encode11_time / 2) 
			car_directon = 1;
		else
			car_directon = -1;
		Encode11_time = 1;
		Encode11_cnt  += 1;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

//�ⲿ�ж�5������� 
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
���ߣ�骷�
ʱ�䣺2020.12.05
���ܣ���������ʼ��
*/
void Encoder_Init(void)
{
	EXTIX_Init();
}

/*
���ߣ�骷�
ʱ�䣺2020.12.05
���ܣ���ȡ�����������ݵõ������ת��
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
