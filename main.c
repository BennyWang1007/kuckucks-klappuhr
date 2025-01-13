#include <xc.h>
#include <pic18f4520.h>
// #include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"

#include "constants.c"
#include "I2C/I2C.h"
#include "step_motor/step_motor.h"
#include "UART/uart.h"

#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit
#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON    // Brown-out Reset Enable bit
#pragma config PBADEN = OFF  // Watchdog Timer Enable bit
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)



void __interrupt(high_priority) Hi_ISR(void) {
    if (PIR1bits.SSPIF) {
        I2C_int();
    }
}

void SYSTEM_Initialize(void);


void main(void) {

    SYSTEM_Initialize();
   
    step_motor_test();
    
    while(1);

}


void SYSTEM_Initialize(void) {
    UART_Initialize();
    I2C_init();
    step_motor_init();

}

