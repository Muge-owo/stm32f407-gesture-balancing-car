#ifndef KEY_H
#define KEY_H

#include "stm32f4xx.h"

void Key_Init(void);
void Key_Tick(void);
uint8_t Key_GetNum(void);


#endif
