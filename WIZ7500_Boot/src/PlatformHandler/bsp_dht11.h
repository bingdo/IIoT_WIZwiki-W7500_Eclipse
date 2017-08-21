#ifndef __DHT11_H
#define __DHT11_H

#include "W7500x_gpio.h"

typedef struct
{
	uint8_t temp_int;
	uint8_t temp_deci;
	uint8_t humi_int;
	uint8_t humi_deci;
	uint8_t check_sum;
} DHT11_DATA_TypeDef;

#define			GPIO_DHT11_PORT			GPIOA
#define			GPIO_DHT11_PIN			GPIO_Pin_2
#define 		GPIO_DHT11_PAD			PAD_PA
#define 		GPIO_DHT11_AF			PAD_AF1

#define			DHT11_DOUT_0			GPIO_ResetBits(GPIO_DHT11_PORT, GPIO_DHT11_PIN)
#define 		DHT11_DOUT_1			GPIO_SetBits(GPIO_DHT11_PORT, GPIO_DHT11_PIN)
#define 		DHT11_DATA_IN()			GPIO_ReadInputDataBit(GPIO_DHT11_PORT, GPIO_DHT11_PIN)

void DHT11_Init(void);
uint8_t Read_TempAndHumidity (DHT11_DATA_TypeDef *DHT11_DATA);

#endif
