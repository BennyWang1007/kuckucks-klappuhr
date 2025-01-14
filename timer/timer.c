
# include "timer.h"

void Timer1_init() {
    T1CONbits.RD16 = 1; // 16-bit read/write mode
    T1CONbits.T1CKPS = 0b11; // 1:8 prescaler
    T1CONbits.T1OSCEN = 0; // Oscillator off
    T1CONbits.TMR1CS = 0; // Internal clock
    T1CONbits.T1SYNC = 0; // Do not synchronize
    T1CONbits.TMR1ON = 0; // Timer off

    timer1_postscaler_count = 0;
    timer1_postscaler = 1;
    timer1_last_cycle = 0;
#ifdef USE_TIMER1_INTERRUPT
    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
#endif
}

void Timer1_start() {
    T1CONbits.TMR1ON = 1;
}

void Timer1_stop() {
    T1CONbits.TMR1ON = 0;
}

void Timer1_reset() {
    TMR1H = 0;
    TMR1L = 0;
    timer1_postscaler_count = 0;
    timer1_postscaler = 1;
}

void Timer1_set(uint16_t value) {
    TMR1H = value >> 8;
    TMR1L = value & 0xFF;
}
#include "../UART/uart.h"
void Timer1_set_ms(uint16_t ms) {
    uint32_t cycle = (uint32_t)ms * 1000 >> 5;
    timer1_postscaler = (uint16_t)(cycle >> 16);
    timer1_last_cycle = (uint16_t)(cycle & 0xFFFF);
    timer1_last_cycle = (uint16_t)65535 - timer1_last_cycle + 1;
    // SendNumberInt16(timer1_last_cycle);
    // SendNumberInt16(timer1_postscaler);
    if (timer1_postscaler_count == timer1_postscaler) {
        Timer1_set(timer1_last_cycle);
    } else {
        Timer1_set(0);
    }
}

void Timer1_ISR(void (*callback)()) {
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;
        if (timer1_postscaler_count == timer1_postscaler) {
            timer1_postscaler_count = 0;
            Timer1_set(timer1_last_cycle);
            callback();
        } else {
            timer1_postscaler_count++;
        }
    }
}