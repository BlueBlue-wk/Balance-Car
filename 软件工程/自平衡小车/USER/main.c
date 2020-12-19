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
 ALIENTEK STM32F103������ ��չʵ��23
 ATK-BLE01��������ģ���ֻ�ͨ��ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/





float Car_spd=0.0,Car_turn,Car_distance,Delta_turn = 0.0;


u16 BalanceCar_Cnt;
u16 Display_Cnt;

//������
int main(void)
{
	  delay_init();                         // ��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);                    // ���ڳ�ʼ��Ϊ115200
    LED_Init();                           // LED��ʼ��
	  OLED_Init();			                    // ��ʼ��OLED  
		OLED_Clear();    
	  Car_spd   = 0.0;
	  Car_turn     = 0.0;
	  Car_distance = 0.0;
		int Period = 20;                 // ����PID���Ƶ�����
	  
	  while(MPU_Init())	                    // ��ʼ��MPU6050
		{
			OLED_ShowString(0,2,"MPU Init......");
			delay_ms(1000);
			LED0 = !LED0;
		}
		OLED_ShowString(0,2,"MPU Init...OK!");
		TIM4_Int_Init(9, 719);
	  Ble_IoInit();                         // BLE���ų�ʼ��
    Ble_SetInit();                        // BLE��������
		u3_printf("\r\nATK-BLE01ģ�� �ֻ�ͨ��ʵ��\r\n\r\n");
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
			 // ����С��
			if(BalanceCar_Cnt >= Period * 10)
			{
				Car_turn += (Delta_turn / 2);
				Balance_Car_PID(Car_spd,Car_turn,Car_distance,(float)BalanceCar_Cnt / 10000.0);
				BalanceCar_Cnt = 1;
			}
			// ������ʾ
			if(Display_Cnt >= 2999)
			{
				Display_Data();
				Display_Cnt = 1;
			}
	  }

}


