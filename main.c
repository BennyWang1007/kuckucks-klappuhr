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
#include "timer/timer.h"
#include "utils.h"

#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit
#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON    // Brown-out Reset Enable bit
#pragma config PBADEN = OFF  // Watchdog Timer Enable bit
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)

void SYSTEM_Initialize(void);
void get_time_and_print(void);


void __interrupt(high_priority) Hi_ISR(void) {
    Timer1_ISR(get_time_and_print);
}


void main(void) {

    SYSTEM_Initialize();
    
    Timer1_set_ms(1000);
    Timer1_start();
    
    if(!DS1302_GetIsRunning())
    {
        DS1302_SetIsRunning(1);
    }
    
//    step_motor_test();
    
    while(1);
    return;
}

void SYSTEM_Initialize(void) {
    UART_Initialize();
    step_motor_init();
    DS1302_Begin();
    Timer1_init();
}


DS1302_DateTime now;
char buffer[20];
void get_time_and_print() {
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
}

