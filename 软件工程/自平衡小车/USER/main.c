#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "ble_app.h"
#include "mpu6050.h"
#include "usart3.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "timer.h"
#include "oled.h"
#include "bmp.h"
#include "balancecar.h"
#include "motor.h"
#include "encoder.h"

/************************************************
 ALIENTEK STM32F103开发板 扩展实验23
 ATK-BLE01蓝牙串口模块手机通信实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/





float Car_spd=0.0,Car_turn,Car_distance,Delta_turn = 0.0;


u16 BalanceCar_Cnt;
u16 Display_Cnt;

//主函数
int main(void)
{
	  delay_init();                         // 延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);                    // 串口初始化为115200
    LED_Init();                           // LED初始化
	  OLED_Init();			                    // 初始化OLED  
		OLED_Clear();    
	  Car_spd   = 0.0;
	  Car_turn     = 0.0;
	  Car_distance = 0.0;
		int Period = 20;                 // 定义PID控制的周期
	  
	  while(MPU_Init())	                    // 初始化MPU6050
		{
			OLED_ShowString(0,2,"MPU Init......");
			delay_ms(1000);
			LED0 = !LED0;
		}
		OLED_ShowString(0,2,"MPU Init...OK!");
		TIM4_Int_Init(9, 719);
	  Ble_IoInit();                         // BLE引脚初始化
    Ble_SetInit();                        // BLE参数配置
		u3_printf("\r\nATK-BLE01模块 手机通信实验\r\n\r\n");
		OLED_ShowString(0,0,"BLE OK!");
		OLED_Clear();
		OLED_ShowString(0,6,"Roll=");
		OLED_ShowString(0,2,"GYx =");
    Motor_Init();
	  Encoder_Init();
		Display_Cnt = 1;
		BalanceCar_Cnt = 1;
	  while(1)
		{	
			 // 控制小车
			if(BalanceCar_Cnt >= Period * 10)
			{
				Car_turn += (Delta_turn / 2);
				Balance_Car_PID(Car_spd,Car_turn,Car_distance,(float)BalanceCar_Cnt / 10000.0);
				BalanceCar_Cnt = 1;
			}
			// 控制显示
			if(Display_Cnt >= 2999)
			{
				Display_Data();
				Display_Cnt = 1;
			}
	  }

}


