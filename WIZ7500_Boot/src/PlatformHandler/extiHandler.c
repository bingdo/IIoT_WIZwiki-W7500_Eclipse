
#include "extiHandler.h"

extern uint8_t factory_flag;

//void EXTI_Handler(void)
void EXTI_IRQ_Handler(void)
{
	factory_flag = 1;

	NVIC_ClearPendingIRQ(EXTI_IRQn);
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void EXTI_Configuration(void)
{
    NVIC_ClearPendingIRQ(EXTI_IRQn); // Pending bit Clear
    NVIC_EnableIRQ(EXTI_IRQn);       // EXTI Interrupt Enable
}
