#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"


void LED_Init(void);
void LED1_SetState(uint8_t state);
void LED1_Toggle(void);
void LED2_SetState(uint8_t state);
void LED2_Toggle(void);

#endif
