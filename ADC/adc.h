#ifndef _ADC_H
#define _ADC_H

#include <xc.h>
#include <pic18f4520.h>
#include "../constants.c"

inline void ADC_init(void);
inline void ADC_start(void);
int8_t ADC_read_int8(void);
int16_t ADC_read_int16(void);

#endif  /* _ADC_H */