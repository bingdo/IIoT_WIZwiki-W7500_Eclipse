
#ifndef __GPIOHANDLER_H__
#define __GPIOHANDLER_H__

#include <stdint.h>
#include <stdbool.h>
#include "W7500x_gpio.h"
#include "common.h"

#define LEDn                             3

#define LED1_PIN						GPIO_Pin_8
#define LED1_GPIO_PORT					GPIOC
//#define LED1_GPIO_PAD					PAD_PC
//#define LED1_GPIO_AF					PAD_AF1

#define LED2_PIN						GPIO_Pin_9
#define LED2_GPIO_PORT					GPIOC
//#define LED2_GPIO_PAD					PAD_PC
//#define LED2_GPIO_AF					PAD_AF1

#define LED3_PIN						GPIO_Pin_5
#define LED3_GPIO_PORT					GPIOC
//#define LED3_GPIO_PAD					PAD_PC
//#define LED3_GPIO_AF					PAD_AF1

#define BOOT_PIN						GPIO_Pin_0
#define BOOT_GPIO_PORT					GPIOC
#define BOOT_GPIO_PAD					PAD_PC
#define BOOT_GPIO_AF					PAD_AF1

#define FACT_PIN						GPIO_Pin_1
#define FACT_GPIO_PORT					GPIOC
#define FACT_GPIO_PAD					PAD_PC
#define FACT_GPIO_AF					PAD_AF1

typedef enum
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2
} Led_TypeDef;

void LED_Init(Led_TypeDef Led);
void LED_On(Led_TypeDef Led);
void LED_Off(Led_TypeDef Led);
void LED_Toggle(Led_TypeDef Led);
uint8_t get_LED_Status(Led_TypeDef Led);
void BOOT_Pin_Init();
uint8_t get_bootpin_Status();
bool Board_factory_get(void);
void Board_factory_Init(void);
extern volatile int32_t g_int_cnt;
extern volatile int32_t g_int_rflag;
void RORT2_Configuration();
void PORT2_IRQ_Handler(GPIO_TypeDef* GPIOx, uint32_t port_num);

#endif
