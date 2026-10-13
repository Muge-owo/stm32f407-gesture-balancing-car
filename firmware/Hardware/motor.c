// motor.c
#include "motor.h"
#include "pwm.h"

void Motor_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	PWM_Init();

    PWM_SetCompare1(0);
    PWM_SetCompare2(0);
}

void Morot_SetState(uint8_t state)
{
	if(state)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_2);
	}
	else
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_2);
	}
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
	
	if(id == 1)
	{
		if(pwm >= 0)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
			GPIO_SetBits(GPIOE, GPIO_Pin_1);
			PWM_SetCompare1(pwm);
		}
		else
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_0);
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);
			PWM_SetCompare1(-pwm);
		}
	}
	else if(id == 2)
	{
		if(pwm >= 0)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_3);
			GPIO_SetBits(GPIOE, GPIO_Pin_4);
			PWM_SetCompare2(pwm);
		}
		else
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_3);
			GPIO_ResetBits(GPIOE, GPIO_Pin_4);
			PWM_SetCompare2(-pwm);
		}
	}
}

