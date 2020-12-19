#ifndef _BLE_APP_H_
#define _BLE_APP_H_

#include "sys.h"
#include "ble_cfg.h"

//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//ATK-BLE01模块功能驱动
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2020/1/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2020-2030
//All rights reserved
//********************************************************************************
//无

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

