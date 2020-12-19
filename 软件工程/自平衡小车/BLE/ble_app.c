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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//ATK-BLE01ģ�鹦������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2020/6/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2020-2030
//All rights reserved
//********************************************************************************
//��

//BLEģ��IO��ʼ��
void Ble_IoInit(void)
{

//    GPIO_InitTypeDef  GPIO_InitStructure;

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); /*ʹ��PA�˿�ʱ��*/
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); /*ʹ�ܸ��ù���ʱ��*/

//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); /*��ֹJTAG,�Ӷ�PA15��������ͨIOʹ��,����PA15��������ͨIO!!!*/

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;          /*BLE_WKUP*/
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /*�������*/
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /*IO���ٶ�Ϊ50MHz*/
//    GPIO_Init(GPIOA, &GPIO_InitStructure);              /*������� ��IO���ٶ�Ϊ50MHz*/

//    BLE_WKUP = 1;
//    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;            /*BLE_STA*/
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;        /*��������*/
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    /*IO���ٶ�Ϊ50MHz*/
//    GPIO_Init(GPIOA, &GPIO_InitStructure);               /*�����趨������ʼ��GPIOA.4*/
    
    usart3_init(115200);/*��������*/

}

u8 sendbuf[50];
u8 recivebuf[50];

//BLEģ���������
void Ble_SetInit(void)
{

    LED0 = 0; /*LED0��*/

    /*****��������ģʽ*****/
    while (ble_send_cmd("+++a", "a+ok", 5))
    {
        if (!ble_send_cmd("+++a", "ERROR", 10)) break; /**�п���ģ����������ģʽ,��ʱ����+++a,�᷵��error*/

        LED0 = ~LED0;
        delay_ms(100);
    }

    /*****AT����*****/
    if (!ble_send_cmd("AT", "OK", 5))
    {
        printf("AT OK\r\n");
    }

    /*****������������*****/
    sprintf((char *)sendbuf, "AT+NAME=%s", BLE_NAME);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****������ӭ������*****/
    sprintf((char *)sendbuf, "AT+HELLO=%s", BLE_HELLO);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****���书������*****/
    sprintf((char *)sendbuf, "AT+TPL=%d", BLE_TP_P2dBm);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }
    else{
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****��������*****/
    sprintf((char *)sendbuf, "AT+UART=%d,%d,%d,%d", BLE_UARTBPS, BLE_UARTDATE, BLE_UARTPR, BLE_UARTSTOP);

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****�㲥�ٶ�����*****/
    sprintf((char *)sendbuf, "AT+ADPTIM=%d", BLE_ADPTIM );

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    /*****��·ƥ����������*****/
#if BLE_LINKPASSEN

    if (!ble_send_cmd("AT+LINKPASSEN=ON", "OK", 10)) /*ʹ��*/
    {
        printf("AT+LINKPASSEN=ON  OK\r\n");
    }else
    {
        printf("AT+LINKPASSEN=ON  ERROR\r\n");
    }

#else
    
    if (!ble_send_cmd("AT+LINKPASSEN=OFF", "OK", 10)) /*�ر�*/
    {
        printf("AT+LINKPASSEN=OFF  OK\r\n");
    }else
    {
        printf("AT+LINKPASSEN=OFF  ERROR\r\n");
    }
    
#endif    
    
    /*****����LED����*****/
#if BLE_LEDEN

    if (!ble_send_cmd("AT+LEDEN=ON", "OK", 10)) /*ʹ��*/
    {
        printf("AT+LEDEN=ON  OK\r\n");
    }else
    {
        printf("AT+LEDEN=ON  ERROR\r\n");
    }

#else

    if (!ble_send_cmd("AT+LEDEN=OFF", "OK", 50)) /*�ر�*/
    {
        printf("AT+LEDEN=OFF  OK\r\n");
    }else
    {
        printf("AT+LEDEN=OFF  ERROR\r\n");
    }

#endif

    /*****���豸����˯������*****/
#if BLE_SLAVESLEEPEN

    if (!ble_send_cmd("AT+SLAVESLEEPEN=ON", "OK", 10)) /*ʹ��*/
    {
        printf("AT+SLAVESLEEPEN=ON  OK\r\n");
    }else
    {
        printf("AT+SLAVESLEEPEN=ON  ERROR\r\n");
    }

#else

    if (!ble_send_cmd("AT+SLAVESLEEPEN=OFF", "OK", 10)) /*�ر�*/
    {
        printf("AT+SLAVESLEEPEN=OFF  OK\r\n");
    }else
    {
        printf("AT+SLAVESLEEPEN=OFF  ERROR\r\n");
    }

#endif

    /*****��������������*****/
#if BLE_MAXPUT

    if (!ble_send_cmd("AT+MAXPUT=ON", "OK", 10)) /*ʹ��*/
    {
        printf("AT+MAXPUT=ON  OK\r\n");
    }else
    {
        printf("AT+MAXPUT=ON  ERROR\r\n");
    }

#else

    if (!ble_send_cmd("AT+MAXPUT=OFF", "OK", 10)) /*�ر�*/
    {
        printf("AT+MAXPUT=OFF  OK\r\n");
    }else
    {
        printf("AT+MAXPUT=OFF  ERROR\r\n");
    }

#endif

    /*****����ģʽ����(ģʽ���ú�,ģ��������λ����͸��ģʽ)*****/
    sprintf((char *)sendbuf, "AT+MODE=%c", BLE_MODE_S );

    if (!ble_send_cmd(sendbuf, "OK", 10))
    {
        printf("%s  OK\r\n", sendbuf);
    }else
    {
        printf("%s  ERROR\r\n", sendbuf);
    }

    LED0 = 1;/*LED0��*/
}

/*BLEģ�����ݷ��ʹ���*/
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
/*BLE����ָ�������� ������Ӧ�ڽ�β�����ϡ�0/'*/
void BLE_SendData(char* Data)
{
	u3_printf("%s\n",Data);
}

//������1��û���յ�����
//������0���յ����ݣ������ݴ���Data�У�����ΪLen
u8 BLE_ReceData(char* Data,u32* Len)
{
	if (USART3_RX_STA & 0x8000)
    {
        *Len = USART3_RX_STA & 0X7FFF;
        USART3_RX_BUF[*Len] = 0; //��ӽ�����
        USART3_RX_STA = 0;
        strcpy(Data,(const char *)USART3_RX_BUF);
			return 0;
    }
		else
			return 1;
}

/*BLEģ��������ݴ���*/
void Ble_ReceData(void)
{
    u16 i = 0;
    u16 len = 0;

    //����������
    if (USART3_RX_STA & 0x8000)
    {
        len = USART3_RX_STA & 0X7FFF;
        USART3_RX_BUF[len] = 0; //��ӽ�����
        USART3_RX_STA = 0;

        printf("R:");

        for (i = 0; i < len; i++)
        {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //ѭ������,ֱ���������
            USART_SendData(USART1, USART3_RX_BUF[i]);
        }

        if (strstr((const char *)USART3_RX_BUF, LED0_ON) && (len == (sizeof(LED0_ON) - 1)))        LED0 = 0; /*LED0����*/
        else if (strstr((const char *)USART3_RX_BUF, LED0_OFF) && (len == (sizeof(LED0_OFF) - 1))) LED0 = 1; /*LED0�ر�*/

        memset((char *)USART3_RX_BUF, 0x00, len); //���ڽ��ջ�������0
        printf("\r\n");
    }

}

/*BLE���̴���*/
void Ble_Process(void)
{
//    u8 key = 0;
//    u8 t = 0;
//    u8 BLE_LINK = 0; //1:�������� 0:δ����
//    static u8 flag1, flag2 = 0;

//    printf("\r\nKEY1:��������\r\n");

//    while (1)
//    {

//        key = KEY_Scan(0);

//        if(key == KEY0_PRES) //��������
//        {
//            if (BLE_LINK) /*����������*/
//            {
//                Ble_SendData();/*���ݷ��ʹ���*/
//            }
//        }

//        /*���ݽ��մ���*/
//        Ble_ReceData();

//        /*BLE���Ӵ���*/
//        if (BLE_STA)
//        {
//            BLE_LINK = 1; //��������

//            if (!flag1)
//            {
//                flag1 = 1;
//                flag2 = 0;
//                printf("\r\nble connect!\r\n");
//            }
//        }
//        else
//        {
//            BLE_LINK = 0; //�����Ͽ�

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

//�����Ժ���
void Ble_Test(void)
{

    printf("\r\nATK-BLE01ģ�� �ֻ�ͨ��ʵ��\r\n\r\n");
    Ble_IoInit(); //BLE���ų�ʼ��
    Ble_SetInit();//BLE��������
    Ble_Process();//���ݲ���

}
