// mpu6050.h
#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f4xx.h"

#define MPU6050_ADDR 0xD0


uint8_t MPU6050_Init(void);

void MPU6050_GetByte(uint8_t regAddr, uint8_t *data);
void MPU6050_GetBytes(uint8_t regAddr, uint8_t *data, uint32_t len);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
	int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);


#endif
