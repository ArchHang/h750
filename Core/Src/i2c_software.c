#include "i2c_software.h"

I2C_SoftwareTypeDef i2c_sw;

void I2C_Software_Init()
{
	i2c_sw.SCL_GPIO_Port = GPIOI;
	i2c_sw.SCL_GPIO_Pin	= GPIO_PIN_8;
	i2c_sw.SDA_GPIO_Port = GPIOI;
	i2c_sw.SDA_GPIO_Pin	= GPIO_PIN_9;
	//i2c_sw.TIMref = &htim3;
	i2c_sw.TIM_Clk = 240000000;
	i2c_sw.I2C_MaxSpeed = 400000;
	i2c_sw.AddressSize = 16;
	i2c_sw.DataSize = 8;


}


void I2C_Delay_us(float us)
{
	i2c_sw.TIMref->Instance->CNT = 0;
	HAL_TIM_Base_Start(i2c_sw.TIMref);
	while((us * i2c_sw.TIM_Clk / 1000000) > i2c_sw.TIMref->Instance->CNT)
	{
	}
	HAL_TIM_Base_Stop(i2c_sw.TIMref);
}

void I2C_Delay_Cycle()
{
	I2C_Delay_us(1000000/i2c_sw.I2C_MaxSpeed);
}

void I2C_Delay_HalfCycle()
{
	I2C_Delay_us(500000/i2c_sw.I2C_MaxSpeed);
}

void I2C_SCL_Set()
{
	i2c_sw.SCL_GPIO_Port->BSRR = i2c_sw.SCL_GPIO_Pin;
}

void I2C_SCL_Reset()
{
	i2c_sw.SCL_GPIO_Port->BSRR = i2c_sw.SCL_GPIO_Pin << 16;
}


void I2C_SDA_SetBit(uint8_t bitstate)
{
	if(bitstate != 0x00)
		i2c_sw.SDA_GPIO_Port->BSRR = i2c_sw.SDA_GPIO_Pin;
	else
		i2c_sw.SDA_GPIO_Port->BSRR = i2c_sw.SDA_GPIO_Pin << 16;
}

void I2C_Software_Start()
{
	I2C_SCL_Set();
	I2C_SDA_SetBit(1);
	I2C_Delay_HalfCycle();

	I2C_SDA_SetBit(0);
	I2C_Delay_HalfCycle();
	I2C_SCL_Reset();
	I2C_Delay_HalfCycle();

}

void I2C_Software_Stop()
{
	I2C_SDA_SetBit(0);
	I2C_Delay_HalfCycle();
	I2C_SCL_Set();
	I2C_Delay_HalfCycle();
	I2C_SDA_SetBit(1);
	I2C_Delay_HalfCycle();
}

void I2C_TansmitBit(uint8_t bitstate)
{
	I2C_SDA_SetBit(bitstate);
	I2C_Delay_HalfCycle();
	I2C_SCL_Set();
	I2C_Delay_HalfCycle();
	I2C_SCL_Reset();
}

void I2C_SDA_Input()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = i2c_sw.SDA_GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(i2c_sw.SDA_GPIO_Port, &GPIO_InitStruct);
}

void I2C_SDA_Output()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = i2c_sw.SDA_GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(i2c_sw.SDA_GPIO_Port, &GPIO_InitStruct);
}



int I2C_RecieveBit()
{

	uint16_t gpio_state;
	I2C_SDA_Input();

	I2C_Delay_HalfCycle();
	I2C_SCL_Set();
	gpio_state = i2c_sw.SDA_GPIO_Port->IDR & i2c_sw.SDA_GPIO_Pin;
	I2C_SCL_Reset();
	I2C_SDA_Output();
	if(gpio_state)
		return 1;
	else
		return 0;
}

int I2C_Software_Tansmit(uint16_t addr, uint16_t reg, char *pdata)
{
	I2C_Software_Start();
	addr = (addr << 1) & 0xfffe;

	for(int i=0; i<i2c_sw.AddressSize/2; i++)
	{
		I2C_TansmitBit(addr & (0x0001 << i));
	}

	for(int i=i2c_sw.AddressSize/2; i<i2c_sw.AddressSize; i++)
	{
		I2C_TansmitBit(addr & (0x0001 << i));
	}



	I2C_Software_Stop();
	return 0;
}











