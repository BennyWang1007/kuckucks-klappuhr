#ifndef _TIMER_H
#define _TIMER_H

#include <xc.h>
#include <pic18f4520.h>
#include "../constants.c"

uint16_t timer0_postscaler_count = 0;
uint16_t timer0_postscaler = 1;
uint16_t timer0_last_cycle = 0;

uint16_t timer1_postscaler_count = 0;
uint16_t timer1_postscaler = 1;
uint16_t timer1_last_cycle = 0;

uint16_t timer3_postscaler_count = 0;
uint16_t timer3_postscaler = 1;
uint16_t timer3_last_cycle = 0;

inline void Timer0_init(void);
inline void Timer0_start(void);
inline void Timer0_stop(void);
inline void Timer0_reset(void);
inline void Timer0_set(uint16_t value);
inline void Timer0_set_ms(uint16_t ms);
inline void Timer0_ISR(void (*callback)(void));

inline void Timer1_init(void);
inline void Timer1_start(void);
inline void Timer1_stop(void);
inline void Timer1_reset(void);
inline void Timer1_set(uint16_t value);
inline void Timer1_set_ms(uint16_t ms);
inline void Timer1_ISR(void (*callback)(void));

inline void Timer3_init(void);
inline void Timer3_start(void);
inline void Timer3_stop(void);
inline void Timer3_reset(void);
inline void Timer3_set(uint16_t value);
inline void Timer3_set_ms(uint16_t ms);
inline void Timer3_ISR(void (*callback)(void));

#endif  /* _TIMER_H */
