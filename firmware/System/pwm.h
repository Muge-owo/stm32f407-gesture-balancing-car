// pwm.h
#ifndef PWM_H
#define PWM_H

#include "stm32f4xx.h"

void PWM_Init(void);
void PWM_SetCompare1(uint16_t compare);
void PWM_SetCompare2(uint16_t compare);
uint16_t PWM_GetCompare1(void);
uint16_t PWM_GetCompare2(void);
uint16_t PWM_GetPeriodCounts(void);
uint16_t PWM_GetPrescaler(void);


#endif
