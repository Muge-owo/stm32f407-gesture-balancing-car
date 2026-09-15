#include "key.h"

uint8_t Key_Num = 0;

void Key_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}	

uint8_t	__Key_GetState(void)
{
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
	{
		return 1;
	}
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
	{
		return 2;
	}
	return 0;
}

void Key_Tick(void)
{
	static uint8_t Count = 0;
	static uint8_t CurrentState, PreviousState;

	Count++;
	if(Count >= 20)
	{
		Count = 0;
	
		PreviousState = CurrentState;
		CurrentState = __Key_GetState();
		
		if(CurrentState == 0 && PreviousState != 0)
		{
			Key_Num = PreviousState;
		}
	}
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp = Key_Num;
	Key_Num = 0;
	return Temp;
}
