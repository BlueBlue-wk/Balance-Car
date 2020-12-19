#include "ble_app.h"
#include "ble_uart.h"
#include "ble_cfg.h"
#include "usart3.h"
#include "usart.h"
#include "string.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "stdio.h"


//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//ATK-BLE01模块功能驱动
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2020/6/5
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2020-2030
//All rights reserved
//********************************************************************************
//无

//BLE模块IO初始化
void Ble_IoInit(void)
{

//    GPIO_InitTypeDef  GPIO_InitStructure;

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); /*使能PA端口时钟*/
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); /*使能复用功能时钟*/

//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); /*禁止JTAG,从而PA15可以做普通IO使用,否则PA15不能做普通IO!!!*/

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;          /*BLE_WKUP*/
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /*推挽输出*/
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /*IO口速度为50MHz*/
//    GPIO_Init(GPIOA, &GPIO_InitStructure);              /*推挽输出 ，IO口速度为50MHz*/

//    BLE_WKUP = 1;
//    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;            /*BLE_STA*/
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;        /*下拉输入*/
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    /*IO口速度为50MHz*/
//    GPIO_Init(GPIOA, &GPIO_InitStructure);               /*根据设定参数初始化GPIOA.4*/
    
    usart3_init(115200);/*串口配置*/

}

u8 sendbuf[50];
u8 recivebuf[50];

//BLE模块参数配置
void Ble_SetInit(void)
{

    LED0 = 0; /*LED0亮*/

    /*****进入配置模式*****/
    while (ble_send_cmd("+++a", "a+ok", 5))
    {
        if (!ble_send_cmd("+++a", "ERROR", 10)) break; /**有可能模块已在配置模式,这时发送+++a,会返回error*/

        LED0 = ~LED0;
        delay_ms(100);
    }

    /*****AT测试*****/
    if (!ble_send_cmd("AT", "OK", 5))
    {
        printf("AT OK\r\n");
    }

    /*****蓝牙名称设置*****/
    sprintf((char *)sendbuf, "AT+NAME=%s", BLE_NAME);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****开机欢迎语设置*****/
    sprintf((char *)sendbuf, "AT+HELLO=%s", BLE_HELLO);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****发射功率设置*****/
    sprintf((char *)sendbuf, "AT+TPL=%d", BLE_TP_P2dBm);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }
    else{
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****串口设置*****/
    sprintf((char *)sendbuf, "AT+UART=%d,%d,%d,%d", BLE_UARTBPS, BLE_UARTDATE, BLE_UARTPR, BLE_UARTSTOP);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****广播速度设置*****/
    sprintf((char *)sendbuf, "AT+ADPTIM=%d", BLE_ADPTIM );

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****链路匹配连接设置*****/
#if BLE_LINKPASSEN

    if (!ble_send_cmd("AT+LINKPASSEN=ON", "OK", 10)) /*使能*/
    {
        printf("AT+LINKPASSEN=ON  OK\r\n");
    }else
    {
        printf("AT+LINKPASSEN=ON  ERROR\r\n");
    }

#else
    
    if (!ble_send_cmd("AT+LINKPASSEN=OFF", "OK", 10)) /*关闭*/
    {
        printf("AT+LINKPASSEN=OFF  OK\r\n");
    }else
    {
        printf("AT+LINKPASSEN=OFF  ERROR\r\n");
    }
    
#endif    
    
    /*****板载LED设置*****/
#if BLE_LEDEN

    if (!ble_send_cmd("AT+LEDEN=ON", "OK", 10)) /*使能*/
    {
        printf("AT+LEDEN=ON  OK\r\n");
    }else
    {
        printf("AT+LEDEN=ON  ERROR\r\n");
    }

#else

    if (!ble_send_cmd("AT+LEDEN=OFF", "OK", 50)) /*关闭*/
    {
        printf("AT+LEDEN=OFF  OK\r\n");
    }else
    {
        printf("AT+LEDEN=OFF  ERROR\r\n");
    }

#endif

    /*****从设备断连睡眠设置*****/
#if BLE_SLAVESLEEPEN

    if (!ble_send_cmd("AT+SLAVESLEEPEN=ON", "OK", 10)) /*使能*/
    {
        printf("AT+SLAVESLEEPEN=ON  OK\r\n");
    }else
    {
        printf("AT+SLAVESLEEPEN=ON  ERROR\r\n");
    }

#else

    if (!ble_send_cmd("AT+SLAVESLEEPEN=OFF", "OK", 10)) /*关闭*/
    {
        printf("AT+SLAVESLEEPEN=OFF  OK\r\n");
    }else
    {
        printf("AT+SLAVESLEEPEN=OFF  ERROR\r\n");
    }

#endif

    /*****蓝牙最大输出设置*****/
#if BLE_MAXPUT

    if (!ble_send_cmd("AT+MAXPUT=ON", "OK", 10)) /*使能*/
    {
        printf("AT+MAXPUT=ON  OK\r\n");
    }else
    {
        printf("AT+MAXPUT=ON  ERROR\r\n");
    }

#else

    if (!ble_send_cmd("AT+MAXPUT=OFF", "OK", 10)) /*关闭*/
    {
        printf("AT+MAXPUT=OFF  OK\r\n");
    }else
    {
        printf("AT+MAXPUT=OFF  ERROR\r\n");
    }

#endif

    /*****工作模式设置(模式设置后,模块立即复位进入透传模式)*****/
    sprintf((char *)sendbuf, "AT+MODE=%c", BLE_MODE_S );

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    LED0 = 1;/*LED0灭*/
}

/*BLE模块数据发送处理*/
void Ble_SendData(void)
{
    static u8 num = 0;

    u3_printf("ATK-BLE01 TEST %d\r\n", num);
    printf("S: ATK-BLE01 TEST %d\r\n", num);

    num++;
    if (num == 255) num = 0;

}
//u8 sendbuf[50];
//u8 recivebuf[50];
/*BLE发送指定的数据 （数据应在结尾处加上‘0/'*/
void BLE_SendData(char* Data)
{
	u3_printf("%s\n",Data);
}

//若返回1：没有收到数据
//若返回0：收到数据，且数据存在Data中，长度为Len
u8 BLE_ReceData(char* Data,u32* Len)
{
	if (USART3_RX_STA & 0x8000)
    {
        *Len = USART3_RX_STA & 0X7FFF;
        USART3_RX_BUF[*Len] = 0; //添加结束符
        USART3_RX_STA = 0;
        strcpy(Data,(const char *)USART3_RX_BUF);
			return 0;
    }
		else
			return 1;
}

/*BLE模块接收数据处理*/
void Ble_ReceData(void)
{
    u16 i = 0;
    u16 len = 0;

    //有数据来了
    if (USART3_RX_STA & 0x8000)
    {
        len = USART3_RX_STA & 0X7FFF;
        USART3_RX_BUF[len] = 0; //添加结束符
        USART3_RX_STA = 0;

        printf("R:");

        for (i = 0; i < len; i++)
        {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //循环发送,直到发送完毕
            USART_SendData(USART1, USART3_RX_BUF[i]);
        }

        if (strstr((const char *)USART3_RX_BUF, LED0_ON) && (len == (sizeof(LED0_ON) - 1)))        LED0 = 0; /*LED0点亮*/
        else if (strstr((const char *)USART3_RX_BUF, LED0_OFF) && (len == (sizeof(LED0_OFF) - 1))) LED0 = 1; /*LED0关闭*/

        memset((char *)USART3_RX_BUF, 0x00, len); //串口接收缓冲区清0
        printf("\r\n");
    }

}

/*BLE进程处理*/
void Ble_Process(void)
{
//    u8 key = 0;
//    u8 t = 0;
//    u8 BLE_LINK = 0; //1:蓝牙连接 0:未连接
//    static u8 flag1, flag2 = 0;

//    printf("\r\nKEY1:发送数据\r\n");

//    while (1)
//    {

//        key = KEY_Scan(0);

//        if(key == KEY0_PRES) //发送数据
//        {
//            if (BLE_LINK) /*蓝牙已连接*/
//            {
//                Ble_SendData();/*数据发送处理*/
//            }
//        }

//        /*数据接收处理*/
//        Ble_ReceData();

//        /*BLE连接处理*/
//        if (BLE_STA)
//        {
//            BLE_LINK = 1; //蓝牙连接

//            if (!flag1)
//            {
//                flag1 = 1;
//                flag2 = 0;
//                printf("\r\nble connect!\r\n");
//            }
//        }
//        else
//        {
//            BLE_LINK = 0; //蓝牙断开

//            if (!flag2)
//            {

//                flag2 = 1;
//                flag1 = 0;
//                printf("\r\nble disconnect!\r\n");
//            }
//        }

//        t++;
//        if (t == 20)
//        {
//            t = 0;
//            LED0 = ~LED0;
//        }
//        delay_ms(10);

//    }

}

//主测试函数
void Ble_Test(void)
{

    printf("\r\nATK-BLE01模块 手机通信实验\r\n\r\n");
    Ble_IoInit(); //BLE引脚初始化
    Ble_SetInit();//BLE参数配置
    Ble_Process();//数据测试

}
