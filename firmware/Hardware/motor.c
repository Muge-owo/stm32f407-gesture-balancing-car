// motor.c
#include "motor.h"
#include "pwm.h"

void Motor_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	PWM_Init();
	
    PWM_SetCompare1(0);
    PWM_SetCompare2(0);
}

void Motor_SetPWM(uint8_t id, int16_t pwm)
{
	if(pwm > 1000)
	{
		pwm = 1000;
	}
	else if(pwm < -1000)
	{
		pwm = -1000;
	}
	
	if(id == 0)
	{
		if(pwm >= 0)
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_8);
			GPIO_ResetBits(GPIOD, GPIO_Pin_9);
			PWM_SetCompare1(pwm);
		}
		else
		{
			GPIO_ResetBits(GPIOD, GPIO_Pin_8);
			GPIO_SetBits(GPIOD, GPIO_Pin_9);
			PWM_SetCompare1(-pwm);
		}
	}
	else
	{
		if(pwm >= 0)
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_10);
			GPIO_ResetBits(GPIOD, GPIO_Pin_11);
			PWM_SetCompare2(pwm);
		}
		else
		{
			GPIO_ResetBits(GPIOD, GPIO_Pin_10);
			GPIO_SetBits(GPIOD, GPIO_Pin_11);
			PWM_SetCompare2(-pwm);
		}
	}
}

