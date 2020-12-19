#ifndef __BALANCECAR_H
#define __BALANCECAR_H	

void Balance_Car_PID(float Cntl_speed,float Cntl_turn,float Cntl_distance,float Delta);
float PID_Angle(float Delta);
float PID_Speed(float Cntl_speed);
float PID_Turn(float Cntl_turn);

#endif

