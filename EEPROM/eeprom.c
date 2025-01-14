#include "eeprom.h"

void EEPROM_init() {
    PIE2bits.EEIE = 0;
}

void EEPROM_write(uint8_t address, uint8_t data) {
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0;   // access data EEPROM memory
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = 1;
    while(EECON1bits.WR == 1);
    PIR2bits.EEIF = 0;
    EECON1bits.WREN = 0;
}

uint8_t EEPROM_read(uint8_t address) {
    EEADR = address;
    EECON1bits.WREN = 0;	/* WREN bit is clear for Read operation*/  
    EECON1bits.EEPGD = 0;	/* Access data EEPROM memory*/
    EECON1bits.RD = 1;
    PIR2bits.EEIF = 0;
    return EEDATA;
}