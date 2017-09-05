
#ifndef __RTCHANDLER_H__
#define __RTCHANDLER_H__

#include "W7500x_crg.h"
#include "W7500x_rtc.h"

void RTC_Configuration(void);
void RTC_IRQ_Handler(void);
void RTC_Test(void);

#endif
