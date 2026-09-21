// main.c
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// ****** System ****** //
#include "delay.h"		// SysTick
#include "timer.h"		// TIM6
#include "i2c.h"		// I2C1		PB8(SCL)\PB9(SDA)
#include "pwm.h"		// TIM5		PA0(Ch1)\PA1(Ch2)
#include "uart.h"		// USART1	PA9(TX)\PA10(RX)


// ****** Hardware ****** //
#include "led.h"		// PA6\PA7
#include "key.h"		// PE3\PE4
#include "encoder.h"	// TIM4		PB6\PB7		; TIM1		PE9\PE11
#include "motor.h"		// [TIM5(PA0\PA1)]		PD8\PD9		PD10\PD11
#include "mpu6050.h"	// [I2C1(PB8\PB9)]
#include "oled.h"		// [I2C1(PB8\PB9)]


/** 蓝牙数据包接收 测试程序 **/
int main(void)
{
	usart1_Init(9600);
	
	while(1)
	{
		if(usart1_GetFlag() == 1)
		{
			usart1_printf("rxdata: %s\r\n", usart1_rxdata);
			char *Tag = strtok(usart1_rxdata, ",");
			if(strcmp(Tag, "key") == 0)
			{
				char *Name = strtok(NULL, ",");
				char *Action = strtok(NULL, ",");
				
			}
			else if(strcmp(Tag, "slider") == 0)
			{
				char *Name = strtok(NULL, ",");
				char *Value = strtok(NULL, ",");
			
			}
			else if(strcmp(Tag, "joystick") == 0)
			{
				int8_t LH = atoi(strtok(NULL, ","));
				int8_t LV = atoi(strtok(NULL, ","));
				int8_t RH = atoi(strtok(NULL, ","));
				int8_t RV = atoi(strtok(NULL, ","));
				usart1_printf("joystick:%d, %d, %d, %d\r\n", LH, LV, RH, RV);
			}
		}
	}
}


/** MPU6050互补滤波 测试程序 **/
//volatile uint16_t uart1_SendCnt = 0;	// uart1发送计数器(每隔50ms发送一次数据)
//int16_t AX, AY, AZ, GX, GY, GZ;					// MPU6050采集的加速度、陀螺仪数据
//float Angle = 0, AngleAcc = 0 ,AngleGyro = 0;	// 互补角度、 加速度角度、 陀螺仪角度
//float Alpha = 0.01;								// 互补系数

//int main(void)
//{
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	delay_init();
//	I2C1_Init();
//	MPU6050_Init();
//	usart1_Init(9600);
//	
//	TIM6_Init();
//	
//	while(1)
//	{
//		if(uart1_SendCnt >= 50)
//		{
//			uart1_SendCnt = 0;

//			usart1_printf("Acc:[%d, %d, %d]\r\n", AX, AY, AZ);
//			usart1_printf("Gyro:[%d, %d, %d]\r\n", GX, GY, GZ);
//			usart1_printf("[plot,%f,%f,%f]\r\n", AngleAcc, AngleGyro, Angle);
//		}
//	}
//}
//void TIM6_DAC_IRQHandler(void)
//{
//	static uint16_t count_mpu = 0;	// MPU采集计时器

//	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
//		
//		uart1_SendCnt ++;

//		count_mpu++;
//		if(count_mpu >= 10)		// 10ms采集一次
//		{
//			count_mpu = 0;
//			MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
//			GY -= 12;			// 零点漂移, 以模块具体输出为准
//			AngleAcc = -atan2(AX, AZ) / 3.14159 * 180;			// arctan(x/z) 计算弧度, *(360°/2PI)算出角度
//			AngleGyro = Angle + GY / 32768.0 * 2000 * 0.01;		// 角速度积分得到角度(累加)
//			Angle = Alpha * AngleAcc + (1 - Alpha) * AngleGyro;	// 以陀螺仪角度为主、加速度角度为辅(陀螺仪系数 >> 加速度)
//		}
//	}
//}


/** USART通信 测试程序 **/
//int main(void)
//{
//	usart1_Init(9600);
//	delay_init();
//
//	uint8_t str[100] = "Hello World\r\n";
//
//
//	usart1_SendByte('a');
//	usart1_SendArray(str, strlen((char *)str));
//	usart1_printf("KoToShi :%4d \r\n", 2026);
//	while(1)
//	{
//		if(usart1_GetFlag() == 1)
//		{
//			usart1_printf("RxData: %s\r\n", usart1_rxdata);
//		}
//	}
//}


/** Encoder编码器与LED引脚冲突问题修复 测试程序 **/
//volatile uint16_t OLED_Cnt = 0;
//int main(void)
//{
//	delay_init();
//	TIM6_Init();
//	
//	Encoder_Init();
//	LED_Init();
//	I2C1_Init();
//	
//	delay_ms(100);
//	OLED_Init();

//	int16_t encoder1_cnt = 0;
//	int16_t encoder2_cnt = 0;
//	while(1)
//	{
//		encoder1_cnt = Encoder_GetCNT(0);
//		encoder2_cnt = Encoder_GetCNT(1);
//	
//		if(encoder1_cnt > 0)
//		{
//			LED1_SetState(1);
//			LED2_SetState(1);
//		}
//		else
//		{
//			LED1_SetState(0);
//			LED2_SetState(0);
//		}
//		
//		if(OLED_Cnt >= 200)
//		{
//			OLED_Cnt = 0;
//			
//			OLED_Printf(0, 0, OLED_6X8, "encoder1:%+06d", encoder1_cnt);
//			OLED_Printf(0, 8, OLED_6X8, "encoder2:%+06d", encoder2_cnt);
//			OLED_Update();
//		}
//	}
//}
//void TIM6_DAC_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
//		OLED_Cnt++;
//	}
//}


/** OLED 测试程序 **/
//int main(void)
//{
//	I2C1_Init();
//	delay_ms(100);
//	
//	OLED_Init();
//	
//	
//	/*在(0, 0)位置显示字符'A'，字体大小为8*16点阵*/
//	OLED_ShowChar(0, 0, 'A', OLED_8X16);
//	
//	/*在(16, 0)位置显示字符串"Hello World!"，字体大小为8*16点阵*/
//	OLED_ShowString(16, 0, "Hello World!", OLED_8X16);
//	
//	/*在(0, 18)位置显示字符'A'，字体大小为6*8点阵*/
//	OLED_ShowChar(0, 18, 'A', OLED_6X8);
//	
//	/*在(16, 18)位置显示字符串"Hello World!"，字体大小为6*8点阵*/
//	OLED_ShowString(16, 18, "Hello World!", OLED_6X8);
//	
//	/*在(0, 28)位置显示数字12345，长度为5，字体大小为6*8点阵*/
//	OLED_ShowNum(0, 28, 12345, 5, OLED_6X8);
//	
//	/*在(40, 28)位置显示有符号数字-66，长度为2，字体大小为6*8点阵*/
//	OLED_ShowSignedNum(40, 28, -66, 2, OLED_6X8);
//	
//	/*在(70, 28)位置显示十六进制数字0xA5A5，长度为4，字体大小为6*8点阵*/
//	OLED_ShowHexNum(70, 28, 0xA5A5, 4, OLED_6X8);
//	
//	/*在(0, 38)位置显示二进制数字0xA5，长度为8，字体大小为6*8点阵*/
//	OLED_ShowBinNum(0, 38, 0xA5, 8, OLED_6X8);
//	
//	/*在(60, 38)位置显示浮点数字123.45，整数部分长度为3，小数部分长度为2，字体大小为6*8点阵*/
//	OLED_ShowFloatNum(60, 38, 123.45, 3, 2, OLED_6X8);
//	
//	/*在(0, 48)位置显示英文和汉字串"Hello,世界。"，支持中英文混写*/
//	OLED_ShowString(0, 48, "Hello,世界。", OLED_8X16);
//	
//	/*在(96, 48)位置显示图像，宽16像素，高16像素，图像数据为Diode数组*/
//	OLED_ShowImage(96, 48, 16, 16, Diode);
//	
//	/*在(96, 18)位置打印格式化字符串，字体大小为6*8点阵，格式化字符串为"[%02d]"*/
//	OLED_Printf(96, 18, OLED_6X8, "[%02d]", 6);
//	
//	/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
//	OLED_Update();
//	
//	
//	/*延时3000ms，观察现象*/
//	delay_ms(3000);
//	
//	/*清空OLED显存数组*/
//	OLED_Clear();
//	
//	/*在(5, 8)位置画点*/
//	OLED_DrawPoint(5, 8);
//	
//	/*获取(5, 8)位置的点*/
//	if (OLED_GetPoint(5, 8))
//	{
//		/*如果指定点点亮，则在(10, 4)位置显示字符串"YES"，字体大小为6*8点阵*/
//		OLED_ShowString(10, 4, "YES", OLED_6X8);
//	}
//	else
//	{
//		/*如果指定点未点亮，则在(10, 4)位置显示字符串"NO "，字体大小为6*8点阵*/
//		OLED_ShowString(10, 4, "NO ", OLED_6X8);
//	}
//	
//	/*在(40, 0)和(127, 15)位置之间画直线*/
//	OLED_DrawLine(40, 0, 127, 15);
//	
//	/*在(40, 15)和(127, 0)位置之间画直线*/
//	OLED_DrawLine(40, 15, 127, 0);
//	
//	/*在(0, 20)位置画矩形，宽12像素，高15像素，未填充*/
//	OLED_DrawRectangle(0, 20, 12, 15, OLED_UNFILLED);
//	
//	/*在(0, 40)位置画矩形，宽12像素，高15像素，填充*/
//	OLED_DrawRectangle(0, 40, 12, 15, OLED_FILLED);
//	
//	/*在(20, 20)、(40, 25)和(30, 35)位置之间画三角形，未填充*/
//	OLED_DrawTriangle(20, 20, 40, 25, 30, 35, OLED_UNFILLED);
//	
//	/*在(20, 40)、(40, 45)和(30, 55)位置之间画三角形，填充*/
//	OLED_DrawTriangle(20, 40, 40, 45, 30, 55, OLED_FILLED);
//	
//	/*在(55, 27)位置画圆，半径8像素，未填充*/
//	OLED_DrawCircle(55, 27, 8, OLED_UNFILLED);
//	
//	/*在(55, 47)位置画圆，半径8像素，填充*/
//	OLED_DrawCircle(55, 47, 8, OLED_FILLED);
//	
//	/*在(82, 27)位置画椭圆，横向半轴12像素，纵向半轴8像素，未填充*/
//	OLED_DrawEllipse(82, 27, 12, 8, OLED_UNFILLED);
//	
//	/*在(82, 47)位置画椭圆，横向半轴12像素，纵向半轴8像素，填充*/
//	OLED_DrawEllipse(82, 47, 12, 8, OLED_FILLED);
//	
//	/*在(110, 18)位置画圆弧，半径15像素，起始角度25度，终止角度125度，未填充*/
//	OLED_DrawArc(110, 18, 15, 25, 125, OLED_UNFILLED);
//	
//	/*在(110, 38)位置画圆弧，半径15像素，起始角度25度，终止角度125度，填充*/
//	OLED_DrawArc(110, 38, 15, 25, 125, OLED_FILLED);
//	
//	/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
//	OLED_Update();
//	
//	/*延时3000ms，观察现象*/
//	delay_ms(3000);
//	while(1)
//	{
//		
//		for (uint8_t i = 0; i < 4; i ++)
//		{
//			/*将OLED显存数组部分数据取反，从(0, i * 16)位置开始，宽128像素，高16像素*/
//			OLED_ReverseArea(0, i * 16, 128, 16);
//			
//			/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
//			OLED_Update();
//			
//			/*延时1000ms，观察现象*/
//			delay_ms(1000);
//			
//			/*把取反的内容翻转回来*/
//			OLED_ReverseArea(0, i * 16, 128, 16);
//		}
//		
//		/*将OLED显存数组全部数据取反*/
//		OLED_Reverse();
//		
//		/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
//		OLED_Update();
//		
//		/*延时1000ms，观察现象*/
//		delay_ms(1000);
//	}
//}


/** Motor 测试程序 **/
//int main(void)
//{
//	TIM6_Init();
//	Key_Init();
//	Motor_Init();
//	
//	volatile uint8_t key_num;
//	volatile int16_t pwm1_ccr = 0;
//	while(1)
//	{
//		key_num = Key_GetNum();
//		if(key_num == 1)
//		{
//			pwm1_ccr += 200;
//			if(pwm1_ccr > 1000)
//				pwm1_ccr = 1000;
//		}
//		else if(key_num == 2)
//		{
//			pwm1_ccr -= 200;
//			if(pwm1_ccr < -1000)
//				pwm1_ccr = -1000;
//		}
//		
//		Motor_SetPWM(0, pwm1_ccr);
//	}
//}
//void TIM6_DAC_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
//		Key_Tick();
//	}
//}


/** PWM 测试程序 **/
//int main(void)
//{
//	TIM6_Init();
//	Key_Init();
//	PWM_Init();
//	
//	volatile uint8_t key_num;
//	volatile uint16_t pwm1_ccr = 0, pwm2_ccr = 0;
//	volatile uint16_t get_pwm1_ccr, get_pwm2_ccr, get_pwm_arr, get_pwm_psc;
//	while(1)
//	{
//		key_num = Key_GetNum();
//		if(key_num == 1)
//		{
//			pwm1_ccr += 100;
//			if(pwm1_ccr > 1000)
//				pwm1_ccr = 0;
//			PWM_SetCompare1(pwm1_ccr);
//		}
//		else if(key_num == 2)
//		{
//			pwm2_ccr += 100;
//			if(pwm2_ccr > 1000)
//				pwm2_ccr = 0;
//			PWM_SetCompare2(pwm2_ccr);
//		}
//		get_pwm1_ccr = PWM_GetCompare1();
//		get_pwm2_ccr = PWM_GetCompare2();
//		get_pwm_arr = PWM_GetPeriodCounts();
//		get_pwm_psc = PWM_GetPrescaler();
//	}
//}
//void TIM6_DAC_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
//		Key_Tick();
//	}
//}


/** Encoder编码器读取 测试程序 **/
//int main(void)
//{
//	Encoder_Init();
//	
//	volatile int16_t encoder1_cnt = 0;
//	volatile int16_t encoder2_cnt = 0;

//	while(1)
//	{
//		encoder1_cnt = Encoder_GetCNT(1);
//		encoder2_cnt = Encoder_GetCNT(0);
//	}
//}


/** 硬件 mpu6050 测试程序 **/
//int main(void)
//{
//	I2C1_Init();
//	uint8_t initState = MPU6050_Init();
//	delay_init();
//	
//	if(initState != 0)
//	{
//		while(1)
//		{
//			//初始化失败
//		}
//	}
//	
//	int16_t AX, AY, AZ;
//	int16_t GX, GY, GZ;
//	
//	while(1)
//	{
//		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
//		delay_ms(5);
//	}
//}


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
