// uart.h
#include "uart.h"
#include <stdio.h>
#include <stdarg.h>

char usart1_rxdata[USART1_RXBUF_SIZE] = {0};
volatile int usart1_rxcnt = 0;
volatile char usart1_rxflag = 0;

void usart1_Init(uint32_t baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void usart1_SendByte(uint8_t data)
{
	USART_SendData(USART1, data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void usart1_SendArray(uint8_t *data, uint32_t length)
{
	for(int i = 0; i < length; i++)
	{
		usart1_SendByte(data[i]);
	}
}

void usart1_SendString(uint8_t *data)
{
	for(int i = 0; data[i] != '\0'; i++)
	{
		usart1_SendByte(data[i]);
	}
}

void usart1_printf(char *format, ...)
{
	char str[128];
	
	va_list arg;
	va_start(arg, format);
	vsprintf(str, format, arg);
	va_end(arg);
	usart1_SendString((uint8_t *)str);
}

uint8_t usart1_GetFlag(void)
{
	if(usart1_rxflag == 1)
	{
		usart1_rxcnt = 0;
		usart1_rxflag = 0;
		return 1;
	}
	return 0;
}

void USART1_IRQHandler(void)
{
	static uint8_t usart1_rxstate = 0;
	
	uint8_t data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
		data = USART_ReceiveData(USART1);
		if(data == '[' && usart1_rxstate == 0)
		{
			usart1_rxstate = 1;
		}
		else if(usart1_rxstate == 1)
		{
			if(data != ']')
			{
				usart1_rxdata[usart1_rxcnt++] = data;
				if(usart1_rxcnt >= USART1_RXBUF_SIZE - 1)
				{
					usart1_rxstate = 0;
				}
			}
			else
			{
				usart1_rxdata[usart1_rxcnt] = '\0';
				usart1_rxstate = 0;
				usart1_rxflag = 1;
			}
		}
		else
		{
			usart1_rxstate = 0;
		}
	}
	
}
