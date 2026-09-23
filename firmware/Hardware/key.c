#include "key.h"

volatile uint8_t Key_Num = 0;

void Key_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}	

uint8_t	__Key_GetState(void)
{
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) == 0)
	{
		return 1;
	}
	return 0;
}

void Key_Tick(void)
{
	static uint8_t Count = 0;
	static uint8_t CurrentState = 0, PreviousState = 0;

	Count++;
	if(Count >= 15)
	{
		Count = 0;
	
		PreviousState = CurrentState;
		CurrentState = __Key_GetState();
		
		if(CurrentState != 0 && PreviousState == 0)
		{
			Key_Num = CurrentState;
		}
	}
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp = 0;
	
	__disable_irq();
	Temp = Key_Num;
	Key_Num = 0;
    __enable_irq();
	
	return Temp;
}
