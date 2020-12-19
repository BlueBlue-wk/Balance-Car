#include "ble_uart.h"
#include "delay.h"
#include "string.h"
#include "usart3.h"

//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//ATK-BLE01模块底层处理驱动
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2020/1/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2020-2030
//All rights reserved
//********************************************************************************
//无

//BLE发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//其他,期待应答结果的位置(str的位置)
u8 *ble_check_cmd(u8 *str)
{
    char *strx = 0;

    if (USART3_RX_STA & 0X8000) //接收到一次数据了
    {
        USART3_RX_BUF[USART3_RX_STA & 0X7FFF] = 0; //添加结束符
        strx = strstr((const char *)USART3_RX_BUF, (const char *)str);
    }    
    return (u8 *)strx;
}
//BLE发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 ble_send_cmd(u8 *cmd, u8 *ack, u16 waittime)
{
    u8 res = 0;
    USART3_RX_STA = 0;

    if ((u32)cmd <= 0XFF)
    {
        while ((USART3->SR & 0X40) == 0); //等待上一次数据发送完成

        USART3->DR = (u32)cmd;
    }
    else u3_printf("%s\r\n", cmd);//发送命令

    if (ack && waittime) //需要等待应答
    {
        while (--waittime) //等待倒计时
        {
            delay_ms(10);

            if (USART3_RX_STA & 0X8000) //接收到期待的应答结果
            {
                if (ble_check_cmd(ack))
                {
                   USART3_RX_STA = 0;
                   break; //得到有效数据
                }

                USART3_RX_STA = 0;
            }
        }

        if (waittime == 0)res = 1;
    }

    return res;
}


