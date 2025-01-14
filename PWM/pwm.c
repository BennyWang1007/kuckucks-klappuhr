#include "pwm.h"
#include "../UART/uart.h"


void PWM_init(void) {
    TRISCbits.TRISC2 = 0;       // RC2 pin is output.
    CCP1CONbits.CCP1M = 0b1100; // PWM mode
    T2CONbits.T2CKPS = 0b11;    // Prescaler: 16
    PIR1bits.CCP1IF = 0;        // Clear the interrupt flag
    IPR1bits.CCP1IP = 1;        // High priority
}


void PWM_set_duty_cycle(uint16_t mus) {
    uint16_t cycle = ((uint32_t)mus * 1000 / (_XTAL_FREQ / 1000)) >> 4;
    CCPR1L = (uint8_t)(cycle >> 2);
    CCP1CONbits.DC1B = (cycle & 0b11);
}


void PWM_set_period(uint16_t mus) {
    // (PR2 + 1) * (_XTAL_FREQ / 1000) * 4 * 16 = ms;
    uint16_t cycle = (((uint32_t)mus * 1000 / (_XTAL_FREQ / 1000)) >> 6) - 1;
    PR2 = cycle;
}


void PWM_start(void) {
    T2CONbits.TMR2ON = 1;
}

void PWM_set_degree(int16_t degree) {
    // 500 - 2400 -> -90 - 90
    uint16_t ms = (uint16_t)190 * (uint16_t)(degree + 90) / 18 + 500;
    PWM_set_duty_cycle(ms);
}
