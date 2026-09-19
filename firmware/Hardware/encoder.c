// encoder.c
#include "encoder.h"

void Encoder_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);		//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);		//
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;		//
	GPIO_Init(GPIOE, &GPIO_InitStructure);						//
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);		//
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);	//


	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);			//
	
	TIM_ICInitTypeDef TIM_ICInitStrcuture;
	TIM_ICStructInit(&TIM_ICInitStrcuture);
	TIM_ICInitStrcuture.TIM_ICFilter = 0xF;
	TIM_ICInitStrcuture.TIM_Channel = TIM_Channel_1;
	TIM_ICInit(TIM1, &TIM_ICInitStrcuture);						//
	TIM_ICInit(TIM4, &TIM_ICInitStrcuture);
	TIM_ICInitStrcuture.TIM_Channel = TIM_Channel_2;			//
	TIM_ICInit(TIM1, &TIM_ICInitStrcuture);						//
	TIM_ICInit(TIM4, &TIM_ICInitStrcuture);						//
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	//
	
	TIM_Cmd(TIM1, ENABLE);			//
	TIM_Cmd(TIM4, ENABLE);
}

int16_t Encoder_GetCNT(uint8_t id)
{
	if(id == 0)
	{
		return TIM_GetCounter(TIM1);
	}
	else if(id == 1)
	{
		return TIM_GetCounter(TIM4);
	}
	return 0;
}

