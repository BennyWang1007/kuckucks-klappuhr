#include "eeprom.h"

void EEPROM_init() {
    PIE2bits.EEIE = 0;
}

void EEPROM_write(uint8_t address, uint8_t data) {
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0;   // access data EEPROM memory
    EECON1bits.CFGS = 0;    // select program memory/data EEPROM memory
    EECON1bits.WREN = 1;    // allow write operation
    INTCONbits.GIE = 0;     // disable global interrupt during write operation
    EECON2 = 0x55;  // write sequence must be followed, no interrupt between these instructions
    EECON2 = 0xAA;
    EECON1bits.WR = 1;  // start write operation
    INTCONbits.GIE = 1; // re-enable global interrupt
    while(EECON1bits.WR == 1);
    PIR2bits.EEIF = 0;  // clear EEIF flag
    EECON1bits.WREN = 0;    // disable write operation after write
}

uint8_t EEPROM_read(uint8_t address) {
    EEADR = address;
    EECON1bits.EEPGD = 0;   // access data EEPROM memory
    EECON1bits.CFGS = 0;    //  program memory/data EEPROM memory
    EECON1bits.RD = 1;  // start read operation
    PIR2bits.EEIF = 0;
    return EEDATA;
}