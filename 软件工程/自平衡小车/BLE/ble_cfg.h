#ifndef _BLE_CFG_H
#define _BLE_CFG_H

//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//ATK-BLE01模块参数定义
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2020/1/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2020-2030
//All rights reserved
//********************************************************************************
//无

/*******************蓝牙设置***********************/

/*蓝牙名称*/
#define BLE_NAME  "South"

/*开机欢迎语*/
#define BLE_HELLO "ATK-BLE01"

/*工作模式*/
#define BLE_MODE_M    'M'   /*主模式(可连接)*/
#define BLE_MODE_S    'S'   /*从模式(可连接)*/
#define BLE_MODE_O    'O'   /*观察模式(不可连接,扫描广播数据)*/
#define BLE_MODE_B    'B'   /*广播模式(不可连接,广播用户自定义数据)*/
#define BLE_MODE_I    'I'   /*iBeacon(不可连接,广播特定数据,室内定位)*/

/*发射功率*/
#define BLE_TP_N14dBm  1   /*-14dBm*/
#define BLE_TP_N11dBm  2   /*-11dBm*/
#define BLE_TP_N8dBm   3   /*-8dBm*/
#define BLE_TP_N5dBm   4   /*-5dBm*/
#define BLE_TP_N2dBm   5   /*-2dBm*/
#define BLE_TP_P2dBm   6   /*+2dBm*/
#define BLE_TP_P4dBm   7   /*+4dBm*/
#define BLE_TP_P8dBm   8   /*+8dBm*/


/*串口设置*/
#define BLE_UARTBPS   115200   /*波特率 范围:1200-921600
                                (1200,2400,4800,9600,14400,19200、38400、43000
                                57600、76800、115200、128000、230400、256000、460800
                                921600)
                                */
#define BLE_UARTDATE  8        /*数据位*/
#define BLE_UARTSTOP  1        /*停止位*/
#define BLE_UARTPR    0        /*无校验*/

/*广播速度*/
#define BLE_ADPTIM  5  /*单位:10ms */

/*链路匹配连接使能*/
#define BLE_LINKPASSEN 0

/*板载LED使能*/
#define BLE_LEDEN  1

/*从设备断连睡眠使能*/
#define BLE_SLAVESLEEPEN 0

/*蓝牙最大输出使能*/
#define BLE_MAXPUT  0

/************************************************/

/*LED控制命令*/

#define LED0_ON  "+LED0:ON"
#define LED0_OFF "+LED0:OFF"


#endif
