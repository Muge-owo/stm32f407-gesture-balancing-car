// main.c
#include "stm32f4xx.h"


// ****** System ****** //
#include "delay.h"
#include "timer.h"
#include "i2c.h"


// ****** Hardware ****** //
#include "led.h"
#include "key.h"
#include "mpu6050.h"


/** 硬件 mpu6050 测试程序 **/
int main(void)
{
	I2C1_Init();
	uint8_t initState = MPU6050_Init();
	delay_init();
	
	if(initState != 0)
	{
		while(1)
		{
			//初始化失败
		}
	}
	
	int16_t AX, AY, AZ;
	int16_t GX, GY, GZ;
	
	while(1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		delay_ms(5);
	}
}


/** 硬件I2C收发功能 测试程序 **/
// 向 mpu6050 发送读命令, 根据返回值是否正确来判断 I2C 运行是否正常
//int main(void)
//{
//	LED_Init();
//	I2C1_Init();
//	delay_init();
//	
//	// 读取 mpu6050 的Who Am I寄存器(0x75 RM手册P49: 4.40), 查看寄存器的实际地址
//	uint8_t mpuAddr = I2C1_ReadByte(0xD0, 0x75);
//	
//	if(mpuAddr == 0x68 || mpuAddr == 0x72)
//	{
//		LED1_SetState(1);
//	}
//	
//	// 唤醒 mpu6050, 向Power Management 1寄存器(0x6B RM手册P44: 4.36)写入 0x01
//	I2C1_SendByte(0xD0, 0x6B, 0x01);
//	
//	uint8_t mpuData[14] = {0};
//	volatile int16_t ACCEL_XOUT, ACCEL_YOUT, ACCEL_ZOUT;
//	volatile int16_t TEMP_OUT;
//	volatile int16_t GYRO_XOUT, GYRO_YOUT, GYRO_ZOUT;
//	while(1)
//	{
//		// 暂时没有 OLED 显示, 可以在 Keil5 调试窗口通过 Watch 窗口实时查看接受的数据
//		I2C1_ReadBytes(0xD0, 0x3B, mpuData, 14);
//		ACCEL_XOUT = mpuData[0] << 8 | mpuData[1];
//		ACCEL_YOUT = mpuData[2] << 8 | mpuData[3];
//		ACCEL_ZOUT = mpuData[4] << 8 | mpuData[5];
//		TEMP_OUT = mpuData[6] << 8 | mpuData[7];
//		GYRO_XOUT = mpuData[8] << 8 | mpuData[9];
//		GYRO_YOUT = mpuData[10] << 8 | mpuData[11];
//		GYRO_ZOUT = mpuData[12] << 8 | mpuData[13];
//		
//		delay_ms(10);
//	}
//}


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
