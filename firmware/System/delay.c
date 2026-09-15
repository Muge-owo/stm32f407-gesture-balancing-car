#include "STM32F4XX.H"
#include "delay.h"

void delay_init(void)
{
	// 设置系统定时器时钟源为HCLK（168MHz）
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}

void delay_us(int nus)
{
	SysTick->CTRL = 0; // Disable SysTick 关闭系统定时器
	SysTick->LOAD = 168*nus-1; // 装置值：从这个值递减0
	SysTick->VAL = 0; // Clear current value as well as count flag
	SysTick->CTRL = 5; // Enable SysTick timer with processor clock
	while ((SysTick->CTRL & 0x00010000)==0);// Wait until count flag is set
	SysTick->CTRL = 0; // Disable SysTick
}

void delay_ms(int nms)
{
	while(nms--)
	{
		delay_us(1000); // 当前配置下最大定时时间是99ms
	}
}
