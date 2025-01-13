#ifndef _GLOBAL_VAR_H
#define _GLOBAL_VAR_H

#define _XTAL_FREQ 125000
#define RB0_PRESSED (PORTBbits.RB0 == 0)
#define TIMER2_POSTSCALER 8

typedef unsigned char uint8;
typedef char int8;
typedef unsigned int uint16;
typedef int int16;

int cur_state = 0;
int acc_1A;

int timer2_postscaler_counter = 0;

#endif  /* _GLOBAL_VAR_H */
