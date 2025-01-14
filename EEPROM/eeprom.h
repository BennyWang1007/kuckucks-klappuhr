#ifndef _EEPROM_H
#define _EEPROM_H

#include <xc.h>
#include <pic18f4520.h>
#include "../constants.c"

void EEPROM_init(void);
void EEPROM_write(uint8_t address, uint8_t data);
uint8_t EEPROM_read(uint8_t address);


#endif  /* _EEPROM_H */