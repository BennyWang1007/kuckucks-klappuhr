#include "adc.h"

inline void ADC_init() {
    ADCON1 = 0x0E; // Select channel 0, enable ADC
    TRISAbits.RA0 = 1; // Set RA0/AN0 to input
    TRISAbits.RA1 = 0; // Set RA1 to output

    // ADCON2 = 0x92;

    TRISAbits.RA0 = 1;          // set RA0 as input port

    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.PCFG = 0b1110;   // set AN0 as analog input, others as digital
    ADCON0bits.CHS = 0b0000;    // set AN0 as channel
    // ADCON2bits.ADCS = 0b000;  // set to 000 (1Mhz < 2.86Mhz) by the sheet
    ADCON2bits.ACQT = 0b001;    // Tad = 2 us acquisition time?2Tad = 4 > 2.4
    ADCON0bits.ADON = 1;

#ifdef ADC8BIT
    ADCON2bits.ADFM = 0;        // left justified
#else
    ADCON2bits.ADFM = 1;        // right justified, for 10 bit ADC
#endif

    ADCON2bits.ADCS = 7;
    ADRESH = 0;                 // Flush ADC output
    ADRESL = 0;
}

inline void ADC_start() {
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO);
}

int8_t ADC_read_int8() {
    ADC_start();
    return (int8_t)ADRESL;
}

int16_t ADC_read_int16() {
    ADC_start();
#ifdef ADC8BIT
    return (int16_t)ADRESL;
#else
    return (int16_t)(ADRESH << 8) | ADRESL;
#endif
}