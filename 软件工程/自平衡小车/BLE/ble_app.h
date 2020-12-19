#ifndef _BLE_APP_H_
#define _BLE_APP_H_

#include "sys.h"
#include "ble_cfg.h"

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//ATK-BLE01ģ�鹦������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2020/1/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2020-2030
//All rights reserved
//********************************************************************************
//��

//#define BLE_STA   PAin(4)   
//#define BLE_WKUP  PAout(15)  

void Ble_Test(void);
u8 BLE_ReceData(char* Data,u32* Len);
void BLE_SendData(char* Data);
void Ble_SetInit(void);
void Ble_IoInit(void);
void Ble_Test(void);
void Ble_Process(void);

#endif

