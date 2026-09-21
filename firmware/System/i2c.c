// i2c.h
#include "i2c.h"

/**
* @brief 初始化 I2C1
*/
void I2C1_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_StructInit(&I2C_InitStructure);
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C1, &I2C_InitStructure);
	
	I2C_Cmd(I2C1, ENABLE);
}

uint8_t I2C_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout = 10000;
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout--;
		if(Timeout == 0)
		{
			return 1;
		}
	}
	return 0;
}


/**
* @brief 给一个从设备的一个寄存器中写入一个字节
* @param (in)addr: 从设备地址
* @param (in)regAddr: 要写入的寄存器地址
* @param (in)data: 要写入的数据
*/
void I2C1_SendByte(uint8_t addr, uint8_t regAddr, uint8_t data)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);

	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, regAddr);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C1, data);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/**
* @brief 给一个从设备的一个寄存器中写入多个字节
* @param (in)addr: 从设备地址
* @param (in)regAddr: 要写入的寄存器地址
* @param (in)data: 要写入的数据(数组)
* @param (in)data_len: 要写入的数据长度
*/
void I2C1_SendBytes(uint8_t addr, uint8_t regAddr, const unsigned char *data, uint32_t data_len)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);

	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, regAddr);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	for(int i = 0; i < data_len; i++)
	{
		I2C_SendData(I2C1, data[i]);
		if(i < data_len - 1)
		{
			I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
		}
		else
		{
			I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
		}
	}
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/**
* @brief 读取从设备的一个寄存器中的单个字节
* @param (in)addr: 从设备地址
* @param (in)regAddr: 要读取的寄存器地址
*/
uint8_t I2C1_ReadByte(uint8_t addr, uint8_t regAddr)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C1, ENABLE);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, regAddr);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C1, ENABLE);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_GenerateSTOP(I2C1, ENABLE);

	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Data = I2C_ReceiveData(I2C1);
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	return Data;
}

/**
* @brief 读取从设备的, 从指定寄存器地址开始的多个字节
* @param (in)addr: 从设备地址
* @param (in)regAddr: 开始读取的寄存器地址
* @param (out)data: 存放数据的数组
* @param (in)data_len: 要写入的数据长度
*/
void I2C1_ReadBytes(uint8_t addr, uint8_t regAddr, unsigned char *data, uint32_t data_len)
{
	if(data_len == 0)
	{
		return ;
	}
	else if(data_len == 1)
	{
		data[0] = I2C1_ReadByte(addr, regAddr);
		return ;
	}
	
	I2C_GenerateSTART(I2C1, ENABLE);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, regAddr);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C1, ENABLE);
	I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);
	
	if(data_len <= 2)
	{
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET);
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Next);

//		I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
		(void)I2C1->SR1;
		(void)I2C1->SR2;
		
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET);
		I2C_GenerateSTOP(I2C1, ENABLE);
		
		data[0] = I2C_ReceiveData(I2C1);
		data[1] = I2C_ReceiveData(I2C1);
		
		I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);
	}
	else
	{
		I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);

		for(int i = 0; i < data_len - 3; i++)
		{
			I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);
			data[i] = I2C_ReceiveData(I2C1);
		}
		
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET);
		
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		
		data[data_len - 3] = I2C_ReceiveData(I2C1);
		
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET);
		
		I2C_GenerateSTOP(I2C1, ENABLE);
		
		data[data_len - 2] = I2C_ReceiveData(I2C1);
		data[data_len - 1] = I2C_ReceiveData(I2C1);
	}

	I2C_AcknowledgeConfig(I2C1, ENABLE);
}
