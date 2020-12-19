#include "pid.h"

// 角度环
float Kpb = 110.0;
float Kib = 10.9;
float Kdb = 1.05;

float Kid_value = 0;
float roll_orignal = -2.2;         //小车平衡下的初始角度
float gyx_off = -2.0;
float last_roll = 0.0;
float Error_sum = 0.0;

// 速度环
float Kps = 100.0;
float Kis = 15.5;

float Kis_value = 0.0;
extern float Car_speed;
float Last_speed = 0.0;
float last_cntlspd = 0.0;
float Errorspd_sum = 0.0;

float PWM_left,PWM_right;


void Balance_Car_PID(float Cntl_speech,float Car_turn,float Car_distance,float Delta)
{
	float PWM_Balance, PWM_Speed;
	float Error_angle, Error_speed;
	
	
		
		Car_speed = Car_speed * 0.3 + Last_speed * 0.7;
		Cntl_speech = Cntl_speech * 0.1 + last_cntlspd * 0.9;
		last_cntlspd = Cntl_speech;
		Last_speed = Car_speed;
//		roll        = Kalman_Filter(0,Roll,0,gyx,0,0,Delta);
//		Pitch       = Kalman_Filter1(Pitch,0,0,0,gyy,0,Delta);
//		Yaw         = Kalman_Filter2(0,0,Yaw,0,0,gyz,Delta);
		Kid_value = 2.0*900.0 / Kib;
		Kis_value = 2.0*900.0 / Kis;
		if(fabs(roll) < 20.0)
		{
			// 角度PID
			Error_value = roll - roll_orignal;
			Error_sum   += Error_value;
			if(Error_sum > Kid_value)
				Error_sum = Kid_value;
			else if(Error_sum < -Kid_value)
				Error_sum = -Kid_value;
			PWM_Balance = Kpb * Error_value + Kib * Error_sum + Kdb * gyx;
			
			// 速度PID
			Error_speed = -(Car_speed - Cntl_speech);
			Errorspd_sum += Error_speed;
			if(Errorspd_sum > Kis_value)
				Errorspd_sum = Kis_value;
			else if(Errorspd_sum < -Kis_value)
				Errorspd_sum = -Kis_value;
			PWM_Speed   = Kps * Error_speed + Kis * Errorspd_sum;
			
			
			PWM_left    = PWM_Balance + PWM_Speed;
			PWM_right   = PWM_Balance + PWM_Speed;
		}
		else
		{
			PWM_left    = 0.0;
			PWM_right   = 0.0;
		}
		
		PWM_Map(PWM_left,PWM_right);
		last_roll   = roll;
//		usart1_report_imu(ax,ay,az,gx,gy,gz,(int)(roll*100),(int)(Pitch*100),(int)(Yaw*10));
	
	
		//usart1_report_imu(ax,ay,az,gx,gy,gz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
//	}
}