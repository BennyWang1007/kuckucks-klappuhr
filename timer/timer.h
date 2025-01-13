#ifndef _TIMER_H
#define _TIMER_H

#include <xc.h>
#include <pic18f4520.h>
#include "../constants.c"

uint16_t timer1_postscaler_count = 0;
uint16_t timer1_postscaler = 1;
uint16_t timer1_last_cycle = 0;

void Timer1_init(void);
void Timer1_start(void);
void Timer1_stop(void);
void Timer1_reset(void);
void Timer1_set(uint16_t value);
void Timer1_set_ms(uint16_t ms);
void Timer1_ISR(void (*callback)());

#endif  /* _TIMER_H */
