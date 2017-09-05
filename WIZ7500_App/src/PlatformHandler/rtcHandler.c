
#include "rtcHandler.h"

#define RTC_TEST		RTC

void RTC_IRQ_Handler(void)
{
    NVIC_ClearPendingIRQ(RTC_IRQn); // Pending bit Clear

    if((RTC_GetStatusPendingBit(RTC_TEST) & RTC_INTP_ALF) != 0)
    {
	    RTC_ClearAlarmPendingBit(RTC_TEST);
	    //TODO
	    printf("RTC Alarm TODO\r\n");
    }
    else if ((RTC_GetStatusPendingBit(RTC_TEST) & RTC_INTP_CIF) != 0)
    {
    	RTC_ClearINTPendingBit(RTC_TEST);
    	RTC_Test();
    }
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{
	RTC_TIME_Type tmp_t;

    RTC_Init(RTC_TEST);

    /* Test Case1 : Check RTC Time */
    tmp_t.SEC = 0x00;
    tmp_t.MIN = 0x00;
    tmp_t.HOUR = 0x00;
    tmp_t.DOW = 0x03;
    tmp_t.DOM = 0x01;
    tmp_t.MONTH = 0x01;
    tmp_t.YEAR = 0x1970;
    RTC_SetFullTime(RTC_TEST, &tmp_t);

    /* Test Case2 : Check RTC Interrupt Handler */
    NVIC_ClearPendingIRQ(RTC_IRQn); // Pending bit Clear
    NVIC_EnableIRQ(RTC_IRQn);       // RTC Interrupt Enable
    RTC_EnableINT(RTC_TEST);
    RTC_SetINTMask(RTC_TEST, RTC_INTMASK_SECOND);

    /* Test Case3 : Check RTC Alarm Interrupt Handler */
    tmp_t.SEC = 0x10;
    tmp_t.MIN = 0x00;
    tmp_t.HOUR = 0x00;
    tmp_t.DOW = 0x03;
    tmp_t.DOM = 0x01;
    tmp_t.MONTH = 0x01;
    tmp_t.YEAR = 0x1970;
    RTC_SetFullAlarmTime(RTC_TEST, &tmp_t);
    RTC_SetAlarmMask(RTC_TEST, RTC_INTMASK_ALARM);
}

void RTC_Test(void)
{
	char date_t[8];
	char current_t[6];
	RTC_TIME_Type cur_t;

	//sprintf(date_t, "%04x%02x%02x", ((*(volatile uint32_t *)(0x4000E04c))&0xFFFF0000)>>16, ((*(volatile uint32_t *)(0x4000E04c))&0x1F00)>>8, ((*(volatile uint32_t *)(0x4000E04c))&0x3F));
	//sprintf(current_t, "%02x%02x%02x", ((*(volatile uint32_t *)(0x4000E048))&0x3F0000)>>16, ((*(volatile uint32_t *)(0x4000E048))&0x7F00)>>8, ((*(volatile uint32_t *)(0x4000E048))&0x7F));
    //printf("REG DATE: %s [%01x] TIME : %s \r\n", date_t, ((*(volatile uint32_t *)(0x4000E048))&0xF000000)>>24, current_t);

    strncpy(date_t, 0x00, 8);
    strncpy(current_t, 0x00, 6);

    RTC_GetFullTime(RTC_TEST, &cur_t);
	sprintf(date_t, "%04x%02x%02x", cur_t.YEAR, cur_t.MONTH, cur_t.DOM);
	sprintf(current_t, "%02x%02x%02x", cur_t.HOUR, cur_t.MIN, cur_t.SEC);
    printf("RTC DATE: %s [%01x] TIME : %s \r\n", date_t, cur_t.DOW, current_t);
}
