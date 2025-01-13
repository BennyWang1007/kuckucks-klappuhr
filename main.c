#include <xc.h>
#include <pic18f4520.h>
// #include "setting_hardaware/setting.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.c"
#include "step_motor/step_motor.h"
#include "UART/uart.h"
#include "DS1302/ds1302.h"
#include "utils.h"

#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit
#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON    // Brown-out Reset Enable bit
#pragma config PBADEN = OFF  // Watchdog Timer Enable bit
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)


void __interrupt(high_priority) Hi_ISR(void) {
    if (PIR1bits.SSPIF) {
//        I2C_int();
    }
}

void SYSTEM_Initialize(void);


void main(void) {

    SYSTEM_Initialize();
    
    if(!DS1302_GetIsRunning())
    {
        DS1302_SetIsRunning(1);
    }
    
    DS1302_DateTime now;
    
    
    char buffer[20];
    while(1)
    {
        now = DS1302_GetDateTime();
        itoa(now.yearFrom2000, buffer, 20);
        UART_Write_Text(buffer);
        UART_Write('/');
        itoa(now.month, buffer, 20);
        UART_Write_Text(buffer);
        UART_Write('/');
        itoa(now.dayOfMonth, buffer, 20);
        UART_Write_Text(buffer);
        UART_Write(' ');
        itoa(now.hour, buffer, 20);
        UART_Write_Text(buffer);
        UART_Write(':');
        itoa(now.minute, buffer, 20);
        UART_Write_Text(buffer);
        UART_Write(':');
        itoa(now.second, buffer, 20);
        UART_Write_Text(buffer);
        UART_Write_Text("\r\n");
        __delay_ms(1000);
    }
//    step_motor_test();
    
    while(1);

}


void SYSTEM_Initialize(void) {
    UART_Initialize();
    step_motor_init();
    DS1302_Begin();
}

