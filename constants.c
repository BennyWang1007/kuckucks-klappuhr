#ifndef _GLOBAL_VAR_H
#define _GLOBAL_VAR_H

#include <stdint.h>

#define _XTAL_FREQ 1000000
#define PHOTORESISTOR_THRESHOLD 20
#define CHIMING_MELODY_DURATION 8
#define RB0_PRESSED (PORTBbits.RB0 == 0)

#define ADC8BIT

// #define SPEEDUP_TEST

#define USE_TIMER1_INTERRUPT
#define USE_TIMER3_INTERRUPT

#endif  /* _GLOBAL_VAR_H */
