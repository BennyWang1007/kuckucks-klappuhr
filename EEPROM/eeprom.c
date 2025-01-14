#include "eeprom.h"

void EEPROM_init() {
    EECON1 = 0x00;
    EECON1bits.WREN = 1;
}

void EEPROM_write(uint8_t address, uint8_t data) {
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0;   // access data EEPROM memory
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    PIR2.EEIE = 0;
    while (EECON1bits.WR);
    EECON1bits.WREN = 0;
}

uint8_t EEPROM_read(uint8_t address) {
    EEADR = address;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}