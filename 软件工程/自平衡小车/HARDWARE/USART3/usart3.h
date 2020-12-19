#ifndef __USART3_H
#define __USART3_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����3��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

#define USART3_MAX_RECV_LEN   1024//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN   600 //����ͻ����ֽ���
#define USART3_RX_EN  1 //0,������;1,����.

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; //���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; //���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern vu16 USART3_RX_STA; //��������״̬

void usart3_init(u32 bound);		//����2��ʼ��
void usart3_set(u8 bps, u8 parity);
void usart3_rx(u8 enable);
void u3_printf(char *fmt, ...);
#endif













