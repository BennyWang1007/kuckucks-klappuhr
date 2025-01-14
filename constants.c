#ifndef _GLOBAL_VAR_H
#define _GLOBAL_VAR_H

#include <stdint.h>

#define _XTAL_FREQ 1000000
#define RB0_PRESSED (PORTBbits.RB0 == 0)

#define USE_TIMER1_INTERRUPT

#endif  /* _GLOBAL_VAR_H */
