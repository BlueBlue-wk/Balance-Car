#include "ble_uart.h"
#include "delay.h"
#include "string.h"
#include "usart3.h"

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//ATK-BLE01ģ��ײ㴦������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2020/1/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2020-2030
//All rights reserved
//********************************************************************************
//��

//BLE���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//����,�ڴ�Ӧ������λ��(str��λ��)
u8 *ble_check_cmd(u8 *str)
{
    char *strx = 0;

    if (USART3_RX_STA & 0X8000) //���յ�һ��������
    {
        USART3_RX_BUF[USART3_RX_STA & 0X7FFF] = 0; //��ӽ�����
        strx = strstr((const char *)USART3_RX_BUF, (const char *)str);
    }    
    return (u8 *)strx;
}
//BLE��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 ble_send_cmd(u8 *cmd, u8 *ack, u16 waittime)
{
    u8 res = 0;
    USART3_RX_STA = 0;

    if ((u32)cmd <= 0XFF)
    {
        while ((USART3->SR & 0X40) == 0); //�ȴ���һ�����ݷ������

        USART3->DR = (u32)cmd;
    }
    else u3_printf("%s\r\n", cmd);//��������

    if (ack && waittime) //��Ҫ�ȴ�Ӧ��
    {
        while (--waittime) //�ȴ�����ʱ
        {
            delay_ms(10);

            if (USART3_RX_STA & 0X8000) //���յ��ڴ���Ӧ����
            {
                if (ble_check_cmd(ack))
                {
                   USART3_RX_STA = 0;
                   break; //�õ���Ч����
                }

                USART3_RX_STA = 0;
            }
        }

        if (waittime == 0)res = 1;
    }

    return res;
}


