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
#include "PWM/pwm.h"
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


int8_t cuckoo_phase = 0;

#define cuckoo_high (LATDbits.LATD3 = 1)
#define cuckoo_low (LATDbits.LATD3 = 0)
#define cuckoo_out PWM_set_degree(90)
#define cuckoo_in PWM_set_degree(-90)

void cuckoo_test(void) {

    switch (cuckoo_phase++)
    {
    case 0:
        cuckoo_out;
        cuckoo_low;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        cuckoo_high;
        __delay_ms(500);
        cuckoo_low;
        break;
    case 5:
        cuckoo_in;
        break;
    case 6:
        break;
        
    default:
        cuckoo_phase = 0;
    }
}

void my_tests() {
    get_time_and_print();
    cuckoo_test();
}


void __interrupt(high_priority) Hi_ISR(void) {
    Timer1_ISR(my_tests);
}


void main(void) {

    SYSTEM_Initialize();
    
    Timer1_set_ms(1000);
    Timer1_start();
    PWM_set_period(20000);
    PWM_start();
    
    if(!DS1302_GetIsRunning())
    {
        DS1302_SetIsRunning(1);
    }

    TRISDbits.TRISD3 = 0;

//    step_motor_test();
    
    while(1);
    return;
}

void SYSTEM_Initialize(void) {
    UART_Initialize();
    step_motor_init();
    DS1302_Begin();
    Timer1_init();
    PWM_init();
}


DS1302_DateTime now;
void get_time_and_print() {
    now = DS1302_GetDateTime();
    SendNumberUInt8(now.yearFrom2000);
    UART_Write('/');
    SendNumberUInt8(now.month);
    UART_Write('/');
    SendNumberUInt8(now.dayOfMonth);
    UART_Write(' ');
    SendNumberUInt8(now.hour);
    UART_Write(':');
    SendNumberUInt8(now.minute);
    UART_Write(':');
    SendNumberUInt8(now.second);
    UART_Write_Text("\r\n");
}

