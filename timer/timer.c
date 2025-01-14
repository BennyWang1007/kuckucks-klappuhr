
# include "timer.h"


inline void Timer0_init() {
    T0CONbits.TMR0ON = 0;       // Timer off
    T0CONbits.T08BIT = 0;       // 16-bit  mode
    T0CONbits.T0CS = 0;         // Internal clock
    T0CONbits.PSA = 0;  
    T0CONbits.T0PS = 0b111;     // 1:256 prescaler

    timer0_postscaler_count = 0;
    timer0_postscaler = 1;
    timer0_last_cycle = 0;
#ifdef USE_TIMER1_INTERRUPT
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
#endif
}

inline void Timer0_start() {
    T0CONbits.TMR0ON = 1;
}

inline void Timer0_stop() {
    T0CONbits.TMR0ON = 0;
}

inline void Timer0_reset() {
    TMR0H = 0;
    TMR0L = 0;
    timer0_postscaler_count = 0;
    timer0_postscaler = 1;
}

inline void Timer0_set(uint16_t value) {
    TMR0H = value >> 8;
    TMR0L = value & 0xFF;
}

inline void Timer0_set_ms(uint16_t ms) {
    uint32_t cycle = (uint32_t)ms * 1000 >> 10;
    timer0_postscaler = (uint16_t)(cycle >> 16);
    timer0_last_cycle = (uint16_t)(cycle & 0xFFFF);
    timer0_last_cycle = (uint16_t)65535 - timer0_last_cycle + 1;
    if (timer0_postscaler_count == timer0_postscaler) {
        Timer0_set(timer0_last_cycle);
    } else {
        Timer0_set(0);
    }
}

inline void Timer0_ISR(void (*callback)(void)) {
    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        if (timer0_postscaler_count == timer0_postscaler) {
            timer0_postscaler_count = 0;
            Timer0_set(timer0_last_cycle);
            callback();
        } else {
            timer0_postscaler_count++;
        }
    }
}

inline void Timer1_init() {
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
    PIR1bits.TMR1IF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
#endif
}

inline void Timer1_start() {
    T1CONbits.TMR1ON = 1;
}

inline void Timer1_stop() {
    T1CONbits.TMR1ON = 0;
}

inline void Timer1_reset() {
    TMR1H = 0;
    TMR1L = 0;
    timer1_postscaler_count = 0;
    timer1_postscaler = 1;
}

inline void Timer1_set(uint16_t value) {
    TMR1H = value >> 8;
    TMR1L = value & 0xFF;
}

inline void Timer1_set_ms(uint16_t ms) {
    uint32_t cycle = (uint32_t)ms * 1000 >> 5;
    timer1_postscaler = (uint16_t)(cycle >> 16);
    timer1_last_cycle = (uint16_t)(cycle & 0xFFFF);
    timer1_last_cycle = (uint16_t)65535 - timer1_last_cycle + 1;
    if (timer1_postscaler_count == timer1_postscaler) {
        Timer1_set(timer1_last_cycle);
    } else {
        Timer1_set(0);
    }
}

inline void Timer1_ISR(void (*callback)(void)) {
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


inline void Timer3_init() {
    T3CONbits.RD16 = 1; // 16-bit read/write mode
    T3CONbits.T3CKPS = 0b11; // 1:8 prescaler
    T3CONbits.T3SYNC = 0; // Do not synchronize
    T3CONbits.TMR3CS = 0; // Internal clock
    T3CONbits.TMR3ON = 0; // Timer off

    timer3_postscaler_count = 0;
    timer3_postscaler = 1;
    timer3_last_cycle = 0;
#ifdef USE_TIMER3_INTERRUPT
    PIE2bits.TMR3IE = 1;
    PIR2bits.TMR3IF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
#endif
}

inline void Timer3_start() {
    T3CONbits.TMR3ON = 1;
}

inline void Timer3_stop() {
    T3CONbits.TMR3ON = 0;
}

inline void Timer3_reset() {
    TMR3H = 0;
    TMR3L = 0;
    timer3_postscaler_count = 0;
    timer3_postscaler = 1;
}

inline void Timer3_set(uint16_t value) {
    TMR3H = value >> 8;
    TMR3L = value & 0xFF;
}

inline void Timer3_set_ms(uint16_t ms) {
    uint32_t cycle = (uint32_t)ms * 1000 >> 5;
    timer3_postscaler = (uint16_t)(cycle >> 16);
    timer3_last_cycle = (uint16_t)(cycle & 0xFFFF);
    timer3_last_cycle = (uint16_t)65535 - timer3_last_cycle + 1;
    if (timer3_postscaler_count == timer3_postscaler) {
        Timer3_set(timer3_last_cycle);
    } else {
        Timer3_set(0);
    }
}

inline void Timer3_ISR(void (*callback)(void)) {
    if (PIR2bits.TMR3IF) {
        PIR2bits.TMR3IF = 0;
        if (timer3_postscaler_count == timer3_postscaler) {
            timer3_postscaler_count = 0;
            Timer3_set(timer3_last_cycle);
            callback();
        } else {
            timer3_postscaler_count++;
        }
    }
}
