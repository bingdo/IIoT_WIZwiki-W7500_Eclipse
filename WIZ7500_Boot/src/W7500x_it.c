/**
  ******************************************************************************
  * @file    WZTOE/Loopback/W7500x_it.c
  * @author  IOP Team
  * @version V1.0.0
  * @date    26-AUG-2015
  * @brief   This file contains the functions of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WIZnet SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 WIZnet Co.,Ltd.</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "W7500x.h"
#include "common.h"
#include "timerHandler.h"
#include "uartHandler.h"
#include "extiHandler.h"
#include "wztoeHandler.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void TimingDelay_Decrement(void);
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}


/******************************************************************************/
/*                 W7500x Peripherals Interrupt Handlers                      */
/*    Add here the Interrupt Handler for the used peripherals                 */
/******************************************************************************/

/**
  * @brief  This function handles SSP0 Handler.
  * @param  None
  * @retval None
  */
void SSP0_Handler(void)
{}

/**
  * @brief  This function handles SSP1 Handler.
  * @param  None
  * @retval None
  */
void SSP1_Handler(void)
{}


/**
  * @brief  This function handles UART0 Handler.
  * @param  None
  * @retval None
  */
void UART0_Handler(void)
{
	S2E_UART_IRQ_Handler(UART0);
}


/**
  * @brief  This function handles UART1 Handler.
  * @param  None
  * @retval None
  */
void UART1_Handler(void)
{
	S2E_UART_IRQ_Handler(UART1);
}


/**
  * @brief  This function handles UART2 Handler.
  * @param  None
  * @retval None
  */
void UART2_Handler(void)
{
	S2E_UART_IRQ_Handler(UART2);
}


/**
  * @brief  This function handles I2C0 Handler.
  * @param  None
  * @retval None
  */
void I2C0_Handler(void)
{}


/**
  * @brief  This function handles I2C1 Handler.
  * @param  None
  * @retval None
  */
void I2C1_Handler(void)
{}


/**
  * @brief  This function handles PORT0 Handler.
  * @param  None
  * @retval None
  */
void PORT0_Handler(void)
{}


/**
  * @brief  This function handles PORT1 Handler.
  * @param  None
  * @retval None
  */
void PORT1_Handler(void)
{}


/**
  * @brief  This function handles PORT2 Handler.
  * @param  None
  * @retval None
  */
void PORT2_Handler(void)
{}


/**
  * @brief  This function handles PORT3 Handler.
  * @param  None
  * @retval None
  */
void PORT3_Handler(void)
{}


/**
  * @brief  This function handles DMA Handler.
  * @param  None
  * @retval None
  */
void DMA_Handler(void)
{}


/**
  * @brief  This function handles DUALTIMER0 Handler.
  * @param  None
  * @retval None
  */
void DUALTIMER0_Handler(void)
{
	Timer_IRQ_Handler();
}


/**
  * @brief  This function handles DUALTIMER1 Handler.
  * @param  None
  * @retval None
  */
void DUALTIMER1_Handler(void)
{}


/**
  * @brief  This function handles PWM0 Handler.
  * @param  None
  * @retval None
  */
void PWM0_Handler(void)
{}

/**
  * @brief  This function handles PWM1 Handler.
  * @param  None
  * @retval None
  */
void PWM1_Handler(void)
{}

/**
  * @brief  This function handles PWM2 Handler.
  * @param  None
  * @retval None
  */
void PWM2_Handler(void)
{}

/**
  * @brief  This function handles PWM3 Handler.
  * @param  None
  * @retval None
  */
void PWM3_Handler(void)
{}

/**
  * @brief  This function handles PWM4 Handler.
  * @param  None
  * @retval None
  */
void PWM4_Handler(void)
{}

/**
  * @brief  This function handles PWM5 Handler.
  * @param  None
  * @retval None
  */
void PWM5_Handler(void)
{}

/**
  * @brief  This function handles PWM6 Handler.
  * @param  None
  * @retval None
  */
void PWM6_Handler(void)
{}

/**
  * @brief  This function handles PWM7 Handler.
  * @param  None
  * @retval None
  */
void PWM7_Handler(void)
{}

/**
  * @brief  This function handles RTC Handler.
  * @param  None
  * @retval None
  */
void RTC_Handler(void)
{
	//RTC_IRQ_Handler();
}

/**
  * @brief  This function handles ADC Handler.
  * @param  None
  * @retval None
  */
void ADC_Handler(void)
{}

/**
  * @brief  This function handles WATOE Handler.
  * @param  None
  * @retval None
  */
void WZTOE_Handler(void)
{
	WZTOE_IRQ_Handler();
}

/**
  * @brief  This function handles EXTI Handler.
  * @param  None
  * @retval None
  */
void EXTI_Handler(void)
{
	EXTI_IRQ_Handler();
}



/******************* (C) COPYRIGHT 2015 WIZnet Co.,Ltd *****END OF FILE****/
