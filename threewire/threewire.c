/*
 * File:   threewire.c
 * Author: laptop
 *
 * Created on 2025?1?13?, ?? 4:29
 */


#include <xc.h>
#include <stdint.h>
#include "threewire.h"

const uint8_t THREEWIRE_READFLAG = 0x01;

void ThreeWire_Init()
{
    // set pins in original code
    // hard coded in pic
}

void ThreeWire_Reset()
{
    // Set three pins to input
    IO_TRIS = 1;
    CE_TRIS = 1;
    CLK_TRIS = 1;
}

void ThreeWire_Begin()
{
    ThreeWire_Reset();
}

void ThreeWire_End()
{
    ThreeWire_Reset();
}


// void beginTransmission(uint8_t command) {
//     digitalWrite(_cePin, LOW); // default, not enabled
//     pinMode(_cePin, OUTPUT);

//     digitalWrite(_clkPin, LOW); // default, clock low
//     pinMode(_clkPin, OUTPUT);

//     pinMode(_ioPin, OUTPUT);

//     digitalWrite(_cePin, HIGH); // start the session
//     delayMicroseconds(4);           // tCC = 4us

//     write(command, (command & THREEWIRE_READFLAG) == THREEWIRE_READFLAG);
// }

void ThreeWire_BeginTransmission(uint8_t command)
{
    CE_PIN = 0; // default, not enabled
    CE_TRIS = OUTPUT;

    CLK_PIN = 0; // default, clock low
    CLK_TRIS = OUTPUT;

    IO_TRIS = OUTPUT;

    CE_PIN = 1; // start the session
    __delay_us(4);           // tCC = 4us

    ThreeWire_Write(command, (command & THREEWIRE_READFLAG) == THREEWIRE_READFLAG);
}

void ThreeWire_EndTransmission()
{
    CE_PIN = 0;
    __delay_us(4);
}


// void write(uint8_t value, bool isDataRequestCommand = false) {
//     for (uint8_t bit = 0; bit < 8; bit++) {
//         digitalWrite(_ioPin, value & 0x01);
//         delayMicroseconds(1);     // tDC = 200ns

//         // clock up, data is read by DS1302
//         digitalWrite(_clkPin, HIGH);
//         delayMicroseconds(1);     // tCH = 1000ns, tCDH = 800ns

//         // for the last bit before a read
//         // Set IO line for input before the clock down
//         if (bit == 7 && isDataRequestCommand) {
//             pinMode(_ioPin, INPUT);
//         }

//         digitalWrite(_clkPin, LOW);
//         delayMicroseconds(1);     // tCL=1000ns, tCDD=800ns

//         value >>= 1;
//     }
// }

void ThreeWire_Write(uint8_t value, uint8_t isDataRequestCommand)
{
    for (uint8_t bit = 0; bit < 8; bit++) {
        IO_PIN = value & 0x01;
        __delay_us(1);     // tDC = 200ns

        // clock up, data is read by DS1302
        CLK_PIN = 1;
        __delay_us(1);     // tCH = 1000ns, tCDH = 800ns

        // for the last bit before a read
        // Set IO line for input before the clock down
        if (bit == 7 && isDataRequestCommand) {
            IO_TRIS = INPUT;
        }

        CLK_PIN = 0;
        __delay_us(1);     // tCL=1000ns, tCDD=800ns

        value >>= 1;
    }
}


// uint8_t read() {
//     uint8_t value = 0;

//     for (uint8_t bit = 0; bit < 8; bit++) {
//         // first bit is present on io pin, so only clock the other
//         // bits
//         value |= (digitalRead(_ioPin) << bit);
    
//         // Clock up, prepare for next
//         digitalWrite(_clkPin, HIGH);
//         delayMicroseconds(1);

//         // Clock down, value is ready after some time.
//         digitalWrite(_clkPin, LOW);
//         delayMicroseconds(1);        // tCL=1000ns, tCDD=800ns
//     }

//     return value;
// }

uint8_t ThreeWire_Read()
{
    uint8_t value = 0;

    for (uint8_t bit = 0; bit < 8; bit++) {
        // first bit is present on io pin, so only clock the other
        // bits
        value |= (IO_PIN << bit);
    
        // Clock up, prepare for next
        CLK_PIN = 1;
        __delay_us(1);

        // Clock down, value is ready after some time.
        CLK_PIN = 0;
        __delay_us(1);        // tCL=1000ns, tCDD=800ns
    }

    return value;
}