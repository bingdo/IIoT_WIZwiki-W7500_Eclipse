/**
	***************************************************
	*
	*@brief			DHT11
	*@data 			2017.03
	*@author		Jim
	*@file 			bsp_dht11.c
	*
	***************************************************
**/

#include "bsp_dht11.h"
#include "timerHandler.h"

static void GPIO_DHT11_OUT(void);
static void GPIO_DHT11_IN(void);
static uint8_t READ_DATA_8BIT(void);

void DHT11_Init(void)
{
	GPIO_DHT11_OUT();
	DHT11_DOUT_1;
	delay_ms(100);
}

static void GPIO_DHT11_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_DHT11_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	
	GPIO_Init(GPIO_DHT11_PORT, &GPIO_InitStruct);

    PAD_AFConfig(GPIO_DHT11_PAD, GPIO_DHT11_PIN, GPIO_DHT11_AF);
}

static void GPIO_DHT11_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_DHT11_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	
	GPIO_Init(GPIO_DHT11_PORT, &GPIO_InitStruct);

    PAD_AFConfig(GPIO_DHT11_PAD, GPIO_DHT11_PIN, GPIO_DHT11_AF);
}

static uint8_t READ_DATA_8BIT(void)
{
	uint8_t i, data=0;

	for(i=0;i<8;i++)
	{	
		while(GPIO_ReadInputDataBit(GPIO_DHT11_PORT, GPIO_DHT11_PIN) == Bit_RESET);
		delay_cnt(40);

		if(GPIO_ReadInputDataBit(GPIO_DHT11_PORT, GPIO_DHT11_PIN) == Bit_SET)
		{
			while(GPIO_ReadInputDataBit(GPIO_DHT11_PORT, GPIO_DHT11_PIN) == Bit_SET);
			data |= (uint8_t)(0x01 << (7-i));
		}
		else 
			data &= (uint8_t)~(0x01 << (7-i));
			//data &= (uint8_t)(0x00 << (7-i));
	}

	return data;
}

uint8_t Read_TempAndHumidity (DHT11_DATA_TypeDef *DHT11_DATA)
{
	GPIO_DHT11_OUT();
	DHT11_DOUT_0;
	delay_ms(20);
	DHT11_DOUT_1;
	delay_cnt(30);
	GPIO_DHT11_IN();
	if(GPIO_ReadInputDataBit(GPIO_DHT11_PORT, GPIO_DHT11_PIN) == Bit_RESET)
	{
		while(GPIO_ReadInputDataBit(GPIO_DHT11_PORT, GPIO_DHT11_PIN) == Bit_RESET);
		while(GPIO_ReadInputDataBit(GPIO_DHT11_PORT, GPIO_DHT11_PIN) == Bit_SET);
		DHT11_DATA->humi_int = READ_DATA_8BIT();
		DHT11_DATA->humi_deci = READ_DATA_8BIT();
		DHT11_DATA->temp_int = READ_DATA_8BIT();
		DHT11_DATA->temp_deci = READ_DATA_8BIT();
    	DHT11_DATA->check_sum = READ_DATA_8BIT();
		GPIO_DHT11_OUT();
		DHT11_DOUT_1;
		if(DHT11_DATA->check_sum == DHT11_DATA->humi_int + DHT11_DATA->humi_deci + DHT11_DATA->temp_int + DHT11_DATA->temp_deci)
			return SUCCESS;
		else
			return ERROR;
	}		
	else
	{
		return ERROR;
	}
}
