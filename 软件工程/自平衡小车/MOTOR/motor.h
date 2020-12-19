#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define PA1 PAout(0)
#define PA2 PAout(1)
#define PB1 PAout(2)
#define PB2 PAout(3)

void TIM3_PWM_Init(u16 arr,u16 psc);
void CntlIO_Init(void);
void Motor_Init(void);
void PWM_Map(float pwm_left,float pwm_right);

#endif
