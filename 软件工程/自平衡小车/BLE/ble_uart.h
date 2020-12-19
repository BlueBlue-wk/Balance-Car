#ifndef __BLE_UART_H__
#define __BLE_UART_H__
#include "sys.h"
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

u8 *ble_check_cmd(u8 *str);
u8 ble_send_cmd(u8 *cmd, u8 *ack, u16 waittime);


#endif





