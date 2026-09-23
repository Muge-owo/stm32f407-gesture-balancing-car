#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"

void LED_Init(void);
void LED_SetState(uint8_t state);
void LED_Toggle(void);
uint8_t LED_GetState(void);

#endif
