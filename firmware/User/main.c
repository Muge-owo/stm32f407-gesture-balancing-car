// main.c
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// ****** System ****** //
#include "delay.h"		// SysTick
#include "timer.h"		// TIM6
#include "i2c.h"		// I2C1		PB8(SCL)\PB9(SDA)
#include "pwm.h"		// TIM5		PA0(Ch1)\PA1(Ch2)
#include "uart.h"		// USART1	PA9(TX)\PA10(RX)
						// USART2	PA2(TX)\PA3(RX)

// ****** Hardware ****** //
#include "led.h"		// PC0
#include "key.h"		// PC1
#include "encoder.h"	// TIM4		PB6\PB7		; TIM1		PE9\PE11
#include "motor.h"		// [TIM5(PA0\PA1)]		PD8\PD9		PD10\PD11
#include "mpu6050.h"	// [I2C1(PB8\PB9)]
#include "oled.h"		// [I2C1(PB8\PB9)]

int main(void)
{
	TIM6_Init();
	Key_Init();

	while(1)
	{
	
	}
}
void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{

		Key_Tick();
	}
}
