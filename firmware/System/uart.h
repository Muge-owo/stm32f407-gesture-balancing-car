// uart.h
#ifndef UART_H
#define UART_H

#include "stm32f4xx.h"

#define USART1_RXBUF_SIZE 128
extern char usart1_rxdata[];

void usart1_Init(uint32_t baud);
void usart1_SendByte(uint8_t data);
void usart1_SendArray(uint8_t *data, uint32_t length);
void usart1_SendString(uint8_t *data);
void usart1_printf(char *format, ...);
uint8_t usart1_GetFlag(void);

#endif
