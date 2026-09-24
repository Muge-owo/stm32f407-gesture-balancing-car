// encoder.h
#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx.h"

void Encoder_Init(void);
int16_t Encoder_GetCNT(uint8_t n);


#endif
