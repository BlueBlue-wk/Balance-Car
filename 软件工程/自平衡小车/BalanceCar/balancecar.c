#include "balancecar.h"
#include "ble_app.h"
#include "mpu6050.h"
#include "encoder.h"
#include "usart3.h"
#include "math.h"
#include "led.h"
#include "motor.h"

MPU_DATA MPU_data;
ENCODER Encoder;

// 角度环
float Kpb = 110.0;//110.0
float Kib = 10.9;//10.9
float Kdb = 1.05;

float Kid_value = 0;               // PID积分上下限
float roll_orignal = -2.2;         // 小车平衡下的初始角度
float gyx_off = -2.0;              // 陀螺仪的零漂
float Erroragl_sum = 0.0;          // PID积分

/*******************角度环***********************/
//功能：通过PID控制小车的平衡
//输入：Delta  -> PID_Angle函数的运行时间间隔
//输出：PWM_Angle -> 平衡PWM值

float PID_Angle(float Delta)
{
	float PWM_Angle;
	float Error_angle;
	
	// 数据读取
	MPU6050_Data_Read(Delta, gyx_off, gyx_off, gyx_off, &MPU_data);
	// 设置PID积分的上下限
	Kid_value = 2.0*900.0 / Kib;
	// 角度PID
	Error_angle = MPU_data.ROLL - roll_orignal;
	Erroragl_sum   += Error_angle;
	if(Erroragl_sum > Kid_value)
		Erroragl_sum = Kid_value;
	else if(Erroragl_sum < -Kid_value)
		Erroragl_sum = -Kid_value;
	PWM_Angle = Kpb * Error_angle + Kib * Erroragl_sum + Kdb * MPU_data.GX;
	return PWM_Angle;
}

// 速度环
float Kps = 100.0;
float Kis = 15.5;

float Kis_value = 0.0;            // 速度PID的积分上下限
float Car_speedL, Car_speedR;     // 左右电机的转速
float last_cntlspd = 0.0;         // 用于给输入的速度值做LPF
float Errorspd_sum = 0.0;         // 速度PID的积分

/*******************速度环***********************/
//功能：通过PID控制小车的速度
//输入：Cntl_speed -> 遥控小车以该速度行驶
//输出：PWM_Speed -> 控制小车的速度PWM值

float PID_Speed(float Cntl_speed)
{
	float Error_speed, PWM_Speed;
	// 读取编码速率
	Encode_Read(&Encoder);
	// 简单的低通滤波LPF
	Car_speedL = Encoder.Speed_Left * 0.3 + Car_speedL * 0.7;
	Car_speedR = Encoder.Speed_Right * 0.3 + Car_speedR * 0.7;
	Cntl_speed = Cntl_speed * 0.05 + last_cntlspd * 0.95;
	last_cntlspd = Cntl_speed;

	Kis_value = 2.0*900.0 / Kis;
	// 速度PID
	Error_speed = -((Car_speedR + Car_speedL) * 0.5 - Cntl_speed);
	Errorspd_sum += Error_speed;
	if(Errorspd_sum > Kis_value)
		Errorspd_sum = Kis_value;
	else if(Errorspd_sum < -Kis_value)
		Errorspd_sum = -Kis_value;
	PWM_Speed   = Kps * Error_speed + Kis * Errorspd_sum;
	return PWM_Speed;
}

// 方向环
float Kpt = 10.0;
float Kdt = 0.5;

float last_cntltrn = 0.0;    // 用于输入的方向做LPF

/*******************方向环***********************/
//功能：通过PID控制小车的方向
//输入：Cntl_turn -> 遥控小车以该方向行驶
//输出：PWM_Turn -> 控制小车方向的PWM值

float PID_Turn(float Cntl_turn)
{
	float PWM_Turn;
	float Error_turn;
	// 简单的低通滤波
	Cntl_turn = Cntl_turn * 0.1 + last_cntltrn * 0.9;
	last_cntltrn = Cntl_turn;
	// 方向PID
	Error_turn = MPU_data.YAW - Cntl_turn;
	PWM_Turn = Kpt * Error_turn + Kdt * MPU_data.GZ;
	return PWM_Turn;
}

float PWM_left,PWM_right;

/*****************平衡小车角度环、速度环、方向环***********************/
//功能：通过输入速度、转向、距离控制小车
//输入：Cntl_speed、Cntl_turn、Cntl_distance、Delta
//输出：空

void Balance_Car_PID(float Cntl_speed,float Cntl_turn,float Cntl_distance,float Delta)
{
	float PWM_Angle, PWM_Speed, PWM_Turn;
	// 角度环
	PWM_Angle = PID_Angle(Delta);
	// 速度环
	PWM_Speed = PID_Speed(Cntl_speed);
	// 方向环
	PWM_Turn = PID_Turn(Cntl_turn);
	// 判断小车倾斜角度，大于20°即判断倒下，停止电机
	if(fabs(MPU_data.ROLL) < 20.0)
	{
		PWM_left    = PWM_Angle + PWM_Speed + PWM_Turn;
		PWM_right   = PWM_Angle + PWM_Speed - PWM_Turn;
	}
	else
	{
		PWM_left    = 0.0;
		PWM_right   = 0.0;
	}
	// 输出PWM值控制电机
	PWM_Map(PWM_left,PWM_right);
}

