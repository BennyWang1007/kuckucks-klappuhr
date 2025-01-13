#include <stdlib.h>
#include "stdio.h"
#include "string.h"

#include <xc.h>
#include <pic18f4520.h>
#include "I2C.h"


void I2C_init() {
    SSPCON1bits.SSPEN = 1;
    // SSPCON1bits.SSPM = 0b1000;
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;

    Clockbits.RESET = 0;
    Clockbits.SCK = 0;
    Clockbits.SDA = 0;
}

void I2C_test() {
    SSPCON1bits.SSPM = 0b1111;
    I2C_write(0X81);
    SSPCON1bits.SSPM = 0b1000;
    __delay_ms(100);
//    sendNumberInt8(SSPBUF);
}

void I2C_read() {
    SSPCON2bits.RCEN = 1;
}

void I2C_write(uint8_t data) {
    SSPBUF = data;
    SSPCON1bits.SSPM = 0b0010;
}

void I2C_start() {
    SDA = 0;
}

void I2C_stop() {
    Clockbits.SDA = 1;
}

// SDA LOW during the 9th clock pulse
void I2C_ack() {
    Clockbits.SDA = 0;
}

void I2C_int() {
//    sendNumberInt8(SSPBUF);
    PIR1bits.SSPIF = 0;
}