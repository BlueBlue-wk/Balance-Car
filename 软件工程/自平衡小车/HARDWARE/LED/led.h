#ifndef __LED_H
#define __LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#define LED0 PCout(13)// PB5
#define PA1 PAout(0)// PB5
#define PA2 PAout(1)// PB5
#define PB1 PAout(2)// PB5
#define PB2 PAout(3)// PB5

void LED_Init(void);//��ʼ��


#endif
