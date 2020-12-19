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

// �ǶȻ�
float Kpb = 110.0;//110.0
float Kib = 10.9;//10.9
float Kdb = 1.05;

float Kid_value = 0;               // PID����������
float roll_orignal = -2.2;         // С��ƽ���µĳ�ʼ�Ƕ�
float gyx_off = -2.0;              // �����ǵ���Ư
float Erroragl_sum = 0.0;          // PID����

/*******************�ǶȻ�***********************/
//���ܣ�ͨ��PID����С����ƽ��
//���룺Delta  -> PID_Angle����������ʱ����
//�����PWM_Angle -> ƽ��PWMֵ

float PID_Angle(float Delta)
{
	float PWM_Angle;
	float Error_angle;
	
	// ���ݶ�ȡ
	MPU6050_Data_Read(Delta, gyx_off, gyx_off, gyx_off, &MPU_data);
	// ����PID���ֵ�������
	Kid_value = 2.0*900.0 / Kib;
	// �Ƕ�PID
	Error_angle = MPU_data.ROLL - roll_orignal;
	Erroragl_sum   += Error_angle;
	if(Erroragl_sum > Kid_value)
		Erroragl_sum = Kid_value;
	else if(Erroragl_sum < -Kid_value)
		Erroragl_sum = -Kid_value;
	PWM_Angle = Kpb * Error_angle + Kib * Erroragl_sum + Kdb * MPU_data.GX;
	return PWM_Angle;
}

// �ٶȻ�
float Kps = 100.0;
float Kis = 15.5;

float Kis_value = 0.0;            // �ٶ�PID�Ļ���������
float Car_speedL, Car_speedR;     // ���ҵ����ת��
float last_cntlspd = 0.0;         // ���ڸ�������ٶ�ֵ��LPF
float Errorspd_sum = 0.0;         // �ٶ�PID�Ļ���

/*******************�ٶȻ�***********************/
//���ܣ�ͨ��PID����С�����ٶ�
//���룺Cntl_speed -> ң��С���Ը��ٶ���ʻ
//�����PWM_Speed -> ����С�����ٶ�PWMֵ

float PID_Speed(float Cntl_speed)
{
	float Error_speed, PWM_Speed;
	// ��ȡ��������
	Encode_Read(&Encoder);
	// �򵥵ĵ�ͨ�˲�LPF
	Car_speedL = Encoder.Speed_Left * 0.3 + Car_speedL * 0.7;
	Car_speedR = Encoder.Speed_Right * 0.3 + Car_speedR * 0.7;
	Cntl_speed = Cntl_speed * 0.05 + last_cntlspd * 0.95;
	last_cntlspd = Cntl_speed;

	Kis_value = 2.0*900.0 / Kis;
	// �ٶ�PID
	Error_speed = -((Car_speedR + Car_speedL) * 0.5 - Cntl_speed);
	Errorspd_sum += Error_speed;
	if(Errorspd_sum > Kis_value)
		Errorspd_sum = Kis_value;
	else if(Errorspd_sum < -Kis_value)
		Errorspd_sum = -Kis_value;
	PWM_Speed   = Kps * Error_speed + Kis * Errorspd_sum;
	return PWM_Speed;
}

// ����
float Kpt = 10.0;
float Kdt = 0.5;

float last_cntltrn = 0.0;    // ��������ķ�����LPF

/*******************����***********************/
//���ܣ�ͨ��PID����С���ķ���
//���룺Cntl_turn -> ң��С���Ը÷�����ʻ
//�����PWM_Turn -> ����С�������PWMֵ

float PID_Turn(float Cntl_turn)
{
	float PWM_Turn;
	float Error_turn;
	// �򵥵ĵ�ͨ�˲�
	Cntl_turn = Cntl_turn * 0.1 + last_cntltrn * 0.9;
	last_cntltrn = Cntl_turn;
	// ����PID
	Error_turn = MPU_data.YAW - Cntl_turn;
	PWM_Turn = Kpt * Error_turn + Kdt * MPU_data.GZ;
	return PWM_Turn;
}

float PWM_left,PWM_right;

/*****************ƽ��С���ǶȻ����ٶȻ�������***********************/
//���ܣ�ͨ�������ٶȡ�ת�򡢾������С��
//���룺Cntl_speed��Cntl_turn��Cntl_distance��Delta
//�������

void Balance_Car_PID(float Cntl_speed,float Cntl_turn,float Cntl_distance,float Delta)
{
	float PWM_Angle, PWM_Speed, PWM_Turn;
	// �ǶȻ�
	PWM_Angle = PID_Angle(Delta);
	// �ٶȻ�
	PWM_Speed = PID_Speed(Cntl_speed);
	// ����
	PWM_Turn = PID_Turn(Cntl_turn);
	// �ж�С����б�Ƕȣ�����20�㼴�жϵ��£�ֹͣ���
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
	// ���PWMֵ���Ƶ��
	PWM_Map(PWM_left,PWM_right);
}

