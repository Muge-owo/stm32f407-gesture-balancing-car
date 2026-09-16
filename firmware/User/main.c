#include "stm32f4xx.h"


// ****** System ****** //
#include "delay.h"
#include "timer.h"
#include "i2c.h"


// ****** Hardware ****** //
#include "led.h"
#include "key.h"

int main(void)
{
	
	while(1)
	{
		
	}
}


/** 非阻塞按键 KEY 测试程序 **/
//uint16_t tim6_cnt = 0;
//int main(void)
//{
//	TIM6_Init();
//	LED_Init();
//	Key_Init();
//	
//	LED1_SetState(0);
//	LED2_SetState(0);
//	while(1)
//	{
//		if(Key_GetNum() == 2)
//		{
//			LED2_Toggle();
//		}
//		if(Key_GetNum() == 1)
//		{
//			LED2_Toggle();
//		}
//		if(tim6_cnt >= 1000)
//		{
//			tim6_cnt = 0;
//			LED1_Toggle();
//		}
//	}
//}
//void TIM6_DAC_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
//		Key_Tick();
//		tim6_cnt++;
//	}
//}


/** TIM6测试程序(用于程序非阻塞) **/
//volatile static uint16_t tim6_cnt = 0;
//int main(void)
//{
//	delay_init();
//	TIM6_Init();
//	LED_Init();
//	
//	while(1)
//	{
//		if(tim6_cnt >= 1000)
//		{
//			tim6_cnt = 0;
//			LED1_Toggle();
//		}
//		
//	}
//}
//void TIM6_DAC_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
//		tim6_cnt++;
//	}
//}



/** LED测试程序 **/
//int main(void)
//{
//	delay_init();
//	LED_Init();
//	
//	LED1_SetState(1);
//	LED2_SetState(0);
//	while(1)
//	{
//		LED1_Toggle();
//		delay_ms(500);
//		LED2_Toggle();
//		delay_ms(500);
//	}
//}
