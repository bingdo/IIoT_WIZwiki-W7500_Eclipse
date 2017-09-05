
#include "wztoeHandler.h"
#include "socket.h"


void WZTOE_IRQ_Handler(void)
{
	NVIC_ClearPendingIRQ(WZTOE_IRQn);
	printf("WZTOE_IRQ_Handler\r\n");
	setSn_ICR(0, (Sn_IR_CON|Sn_IR_DISCON|Sn_IR_RECV|Sn_IR_TIMEOUT|Sn_IR_SENDOK));
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

	setSIMR(0x01);
}
