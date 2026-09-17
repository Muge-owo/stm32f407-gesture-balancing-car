// i2c.h
#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"

void I2C1_Init(void);

void I2C1_SendByte(uint8_t addr, uint8_t regAddr, uint8_t data);
void I2C1_SendBytes(uint8_t addr, uint8_t regAddr, const unsigned char *data, uint32_t data_len);
uint8_t I2C1_ReadByte(uint8_t addr, uint8_t regAddr);
void I2C1_ReadBytes(uint8_t addr, uint8_t regAddr, unsigned char *data, uint32_t data_len);

#endif
