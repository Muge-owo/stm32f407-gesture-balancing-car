#include "led.h"

void LED_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
}

void LED1_SetState(uint8_t state)
{
	if(state)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	}
	else
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_6);
	}
}

void LED1_Toggle(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_6))
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	}
	else
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_6);
	}
}

void LED2_SetState(uint8_t state)
{
	if(state)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
	}
	else
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
	}
}

void LED2_Toggle(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_7))
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
	}
	else
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
	}
}
