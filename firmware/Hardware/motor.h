// motor.h
#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f4xx.h"

void Motor_Init(void);
void Motor_SetPWM(uint8_t id, int16_t pwm);


#endif
