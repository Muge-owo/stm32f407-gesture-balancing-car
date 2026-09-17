// mpu6050.c
#include "mpu6050.h"
#include "system.h"

uint8_t MPU6050_Init(void)
{
	uint8_t mpu_addr = I2C1_ReadByte(MPU6050_ADDR, 0x75);
	if(mpu_addr != 0x68 && mpu_addr != 0x72)
	{
		return 1;
	}

	// 唤醒 mpu6050: 向Power Management 1寄存器(0x6B RM手册P44: 4.36)写入 0x01
	I2C1_SendByte(MPU6050_ADDR, 0x6B, 0x01);
	delay_us(50);

	uint8_t pwr_val = I2C1_ReadByte(MPU6050_ADDR, 0x6B);
	// 检查 SLEEP位是否清零 并检查陀螺仪内部时钟选择是否为0x01
	if((pwr_val & 0x40) != 0 || (pwr_val & 0x07) != 0x01)
	{
		return 2;
	}
	
	// 六个轴均不待机: 向Power Management 2寄存器(0x6C RM手册P46: 4.37)写入 0x00
	I2C1_SendByte(MPU6050_ADDR, 0x6C, 0x00);
	
	// 0x19: SMPLRT_DIV: 写入0x09 表示 10分频
	// 0x1A: CONFIG: 写入0x06 表示滤波参数最大(可选0~6)
	// 0x1B: GYRO_CONFIG: 写入0x18 , AFS_SEL[1:0]位设置为 11, 表示最大量程 (± 2000 °/s)
	// 0x1C: ACCEL_CONFIG: 写入0x18 , AFS_SEL[1:0]位设置为 11, 表示最大量程 (±16g(1g = 9.8m/s²))
	uint8_t mpu_configs[4] = {0x09, 0x06, 0x18, 0x18};
	I2C1_SendBytes(MPU6050_ADDR, 0x19, mpu_configs, 4);
	
	return 0;
}

void MPU6050_GetByte(uint8_t regAddr, uint8_t *data)
{
	*data = I2C1_ReadByte(MPU6050_ADDR, regAddr);
}

void MPU6050_GetBytes(uint8_t regAddr, uint8_t *data, uint32_t len)
{
	I2C1_ReadBytes(MPU6050_ADDR, regAddr, data, len);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
	int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t data[14] = {0};
	I2C1_ReadBytes(MPU6050_ADDR, 0x3B, data, 14);
	*AccX = data[0] << 8 | data[1];
	*AccY = data[2] << 8 | data[3];
	*AccZ = data[4] << 8 | data[5];
	*GyroX = data[8] << 8 | data[9];
	*GyroY = data[10] << 8 | data[11];
	*GyroZ = data[12] << 8 | data[13];
}
