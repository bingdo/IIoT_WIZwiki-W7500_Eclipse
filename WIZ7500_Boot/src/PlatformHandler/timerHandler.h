
#ifndef __TIMERHANDLER_H__
#define __TIMERHANDLER_H__

void Timer0_Configuration(void);
void delay_cnt(uint32_t count);
void delay_ms(uint32_t ms);

void Timer_IRQ_Handler(void);

#endif
