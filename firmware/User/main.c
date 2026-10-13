// main.c
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "PID.h"

// ****** System ****** //
#include "delay.h"		// SysTick
#include "timer.h"		// TIM6
#include "i2c.h"		// I2C1		PB8(SCL)\PB9(SDA)
//#include "pwm.h"		// TIM5		PA0(Ch1)\PA1(Ch2)
#include "uart.h"		// USART1	PA9(TX)\PA10(RX)
						// USART2	PA2(TX)\PA3(RX)

// ****** Hardware ****** //
#include "led.h"		// PC0
#include "key.h"		// PC1
#include "encoder.h"	// TIM4		PB6\PB7		; TIM1		PE9\PE11
#include "motor.h"		// [TIM8(PA6\PA7)]		PE0\PE1		PE2		PE3\PE4
#include "mpu6050.h"	// [I2C1(PB8\PB9)]
#include "oled.h"		// [I2C1(PB8\PB9)]


#define MPU_READ_TIME 	10		// MPU角度读取及姿态解算间隔
#define ANGLE_PID_TIME	10

#define ENC_READ_TIME	20		// 编码器读取间隔

#define BLEINTERVAL		40		// 蓝牙发送间隔
#define LEDINTERVAL		50		// LED控制间隔

// ****** ++++++++ ****** //
volatile uint16_t ble_send_cnt;
volatile uint16_t led_ctrl_cnt;
volatile uint16_t LedState,LedCnt;		// 手动LED控制计时器

int16_t encoder1_cnt = 0, encoder2_cnt = 0;

int16_t AX, AY, AZ, GX, GY, GZ;
volatile float Angle, AngleAcc, AngleGyro;		// 互补角度、 加速度角度、 陀螺仪角度
float Alpha = 0.01;						// 互补系数

volatile uint8_t RunFlag;				// 平衡算法运行标志

int16_t LeftPWM, RightPWM;
int16_t AvePWM, DifPWM;

PID_t PID_Angle = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0,
	
	.OutMax = 100,
	.OutMin = -100
};


/**********************************/
void app_init(void);

void mpu6050_read(void);
void encoder_read(void);
void angle_pid_ctrl(void);

void bluetooth_send(void);
void bluetooth_read(void);
void led_ctrl(void);


int main(void)
{
	app_init();
	
	while(1)
	{
		bluetooth_send();
		bluetooth_read();
		led_ctrl();
		
		
		if(Angle > 50 || Angle < -50)
		{
			RunFlag = 0;
		}
	}
}

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		mpu6050_read();
		angle_pid_ctrl();
		encoder_read();
		
		ble_send_cnt++;
		led_ctrl_cnt++;
		Key_Tick();
	}
}


/**********************************/
void app_init(void)
{
	Key_Init();
	LED_Init();
	Encoder_Init();
	Motor_Init();
	
	usart1_Init(115200);
	usart2_Init(9600);
	I2C1_Init();
	delay_init();

	delay_ms(233);
	MPU6050_Init();

	TIM6_Init();
}

void mpu6050_read(void)
{
	static uint16_t mpu_read_cnt = 0;

	mpu_read_cnt++;
	if(mpu_status && mpu_read_cnt >= MPU_READ_TIME)
	{mpu_read_cnt = 0;
		
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		GY -= 12;												// 零点漂移, 以模块具体输出为准
		AngleAcc = atan2(AX, -AZ) / 3.14159 * 180;				// arctan(x/z) 计算弧度, *(360°/2PI)算出角度
		AngleGyro = Angle + GY / 32768.0 * 2000 * 0.01;			// 角速度积分得到角度(累加)
		Angle = Alpha * AngleAcc + (1 - Alpha) * AngleGyro;		// 以陀螺仪角度为主、加速度角度为辅(陀螺仪系数 >> 加速度)
	}
	else if(0==mpu_status && mpu_read_cnt>=500)
	{
		mpu_read_cnt = 0;
		MPU6050_Init();
	}
}

void angle_pid_ctrl(void)
{
	static uint16_t angle_pid_cnt = 0;
	
	angle_pid_cnt++;
	if(angle_pid_cnt >= ANGLE_PID_TIME)
	{angle_pid_cnt = 0;
		
		if(RunFlag)
		{
			PID_Angle.Actual = Angle;
			PID_Update(&PID_Angle);
			AvePWM = -PID_Angle.Out;
			
			LeftPWM = AvePWM + DifPWM/2;
			RightPWM = AvePWM - DifPWM/2;
			
			if(LeftPWM > 100) LeftPWM = 100;
			else if (LeftPWM < -100) LeftPWM = -100;
			
			if(RightPWM > 100) RightPWM = 100;
			else if (RightPWM < -100) RightPWM = -100;
			
			Motor_SetPWM(1, LeftPWM);
			Motor_SetPWM(2, RightPWM);
		}
		else
		{
			Motor_SetPWM(1, 0);
			Motor_SetPWM(2, 0);
		}
	}
}

void encoder_read(void)
{
	static uint16_t encoder_read_cnt = 0;
	
	encoder_read_cnt++;
	if(encoder_read_cnt >= ENC_READ_TIME)
	{encoder_read_cnt = 0;
		
		encoder1_cnt = Encoder_GetCNT(1);
		encoder2_cnt = Encoder_GetCNT(2);
	}
}


void bluetooth_send(void)
{
	if(ble_send_cnt >= BLEINTERVAL)
	{ble_send_cnt = 0;
		
		usart2_printf("[plot,%f]\r\n", Angle);
	}
}

void bluetooth_read(void)
{
	if(usart2_GetFlag() == 1)
	{
//		usart2_printf("rxdata: %s\r\n", usart2_rxdata);
		
		char *Tag = strtok(usart2_rxdata, ",");
		if(strcmp(Tag, "key") == 0)
		{
			char *Name = strtok(NULL, ",");
			char *Action = strtok(NULL, ",");
			if(strcmp(Name, "Led") == 0)
			{
				if(strcmp(Action, "up") == 0)
				{
					LED_SetState(0);
				}
				else if(strcmp(Action, "down") == 0)
				{
					LED_SetState(1);
				}
				LedCnt = 1000 / LEDINTERVAL;
				LedState = 1;
			}
			
			if(strcmp(Name, "motor") == 0)
			{
				if(strcmp(Action, "up") == 0)
				{
					Morot_SetState(0);
				}
				else if(strcmp(Action, "down") == 0)
				{
					Morot_SetState(1);
				}
			}
			
			if(strcmp(Name, "run") == 0)
			{
				if(strcmp(Action, "up") == 0)
				{
					RunFlag = 0;
				}
				else if(strcmp(Action, "down") == 0)
				{
					RunFlag = 1;
				}
			}
		}
		else if(strcmp(Tag, "slider") == 0)
		{
			char *Name = strtok(NULL, ",");
			char *Value = strtok(NULL, ",");
			
			if(strcmp(Name, "AngleKp") == 0)
			{
				PID_Angle.Kp = atof(Value);
			}
			else if(strcmp(Name, "AngleKi") == 0)
			{
				PID_Angle.Ki = atof(Value);
			}
			else if(strcmp(Name, "AngleKd") == 0)
			{
				PID_Angle.Kd = atof(Value);
			}
		}
		else if(strcmp(Tag, "joystick") == 0)
		{
			int8_t LH = atoi(strtok(NULL, ","));
			int8_t LV = atoi(strtok(NULL, ","));
			int8_t RH = atoi(strtok(NULL, ","));
			int8_t RV = atoi(strtok(NULL, ","));
			usart2_printf("joystick:%d, %d, %d, %d\r\n", LH, LV, RH, RV);
		}
	}
}

void led_ctrl(void)
{
	if(led_ctrl_cnt >= LEDINTERVAL)
	{led_ctrl_cnt = 0;
		if(!LedState)
		{
			if(RunFlag)
				LED_SetState(1);
			else
				LED_SetState(0);
		}
		else
		{
			if(0 == LedCnt--)
				LedState = 0;
		}
	}
}
