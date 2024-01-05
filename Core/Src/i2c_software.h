#include "main.h"



typedef struct
{
	GPIO_TypeDef			*SCL_GPIO_Port;

	GPIO_TypeDef			*SDA_GPIO_Port;

	uint32_t				SCL_GPIO_Pin;

	uint32_t				SDA_GPIO_Pin;

	TIM_HandleTypeDef		*TIMref;

	uint32_t				TIM_Clk;

	uint32_t				I2C_MaxSpeed;

	uint8_t 				DataSize;

	uint8_t					AddressSize;

} I2C_SoftwareTypeDef;








