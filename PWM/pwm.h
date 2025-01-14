#ifndef _PWM_H
#define _PWM_H

#include <xc.h>
#include <pic18f4520.h>
#include "../constants.c"


void PWM_init(void);
void PWM_set_duty_cycle(uint16_t ms);
void PWM_set_period(uint16_t ms);
void PWM_start(void);
void PWM_set_degree(int16_t degree);


#endif	/* _PWM_H */