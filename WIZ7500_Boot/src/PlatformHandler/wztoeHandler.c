
#include "wztoeHandler.h"

void WZTOE_IRQ_Handler(void)
{
	//printf("I\n");
	NVIC_ClearPendingIRQ(WZTOE_IRQn);
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void WZTOE_Configuration(void)
{
    NVIC_ClearPendingIRQ(WZTOE_IRQn); // Pending bit Clear
    NVIC_EnableIRQ(WZTOE_IRQn);       // WZTOE Interrupt Enable
}
