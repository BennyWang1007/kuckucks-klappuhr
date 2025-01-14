#include <xc.h>
#include <pic18f4520.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ADC/adc.h"
#include "constants.c"
#include "DS1302/ds1302.h"
#include "EEPROM/eeprom.h"
#include "PWM/pwm.h"
#include "step_motor/step_motor.h"
#include "timer/timer.h"
#include "UART/uart.h"
#include "utils.h"

#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit
#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON    // Brown-out Reset Enable bit
#pragma config PBADEN = OFF  // Watchdog Timer Enable bit
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)

#define cuckoo_up (LATDbits.LATD3 = 1)
#define cuckoo_down (LATDbits.LATD3 = 0)
#define cuckoo_out PWM_set_degree(90)
#define cuckoo_in PWM_set_degree(-90)

DS1302_DateTime_t now, prev_time;

void SYSTEM_Initialize(void);

typedef union {
    struct {
        unsigned flag                 :1;
        unsigned up                   :1;
        unsigned phase                :4;
        unsigned unused               :2;
    };
} Cuckoobits_t;

Cuckoobits_t CuckooBits;

uint8_t cuckoo_hour_add1 = 0;

void ADC_ISR() {
    int16_t adc_value = ADC_read_int16();
    SendNumberInt16(adc_value);
    SendString("\r\n");
    if (adc_value < 20) {
        // TODO: decoration lightup
    } else {
        // TODO: decoration lightdown
    }
}

void button_ISR() {
    if (INTCONbits.INT0IF) {
        SendString("INT0\r\n");
        step_motor_step();
        INTCONbits.INT0IF = 0;
    }
    if (INTCON3bits.INT1IF) {
        SendString("INT1\r\n");
        step_motor_forward(6);
        INTCON3bits.INT1IF = 0;
    }
    if (INTCON3bits.INT2IF) {
        SendString("INT2\r\n");
        step_motor_forward(360);
        INTCON3bits.INT2IF = 0;
    }
}

void cuckoo_move() {
    if (CuckooBits.flag == 0) return;
    SendString("TMR3INT\r\n");
    SendNumberUInt8(cuckoo_hour_add1);
    SendString("\r\n");
    if (CuckooBits.phase == 0) {
        cuckoo_out;
        cuckoo_down;
        CuckooBits.phase++;
        // LATAbits.LA1 = 1;
    } else if (CuckooBits.phase < cuckoo_hour_add1) {
        if (CuckooBits.up) {
            cuckoo_down;
            CuckooBits.up = 0;
            CuckooBits.phase++;
        } else {
            cuckoo_up;
            CuckooBits.up = 1;
        }
    } else { //if (CuckooBits.phase == cuckoo_hour_add1) {
        cuckoo_in;
        CuckooBits.flag = 0;
        CuckooBits.phase = 0;
        // LATAbits.LA1 = 0;
    }
}

void TMR3_ISR() {
    button_ISR();
    ADC_ISR();
    cuckoo_move();
}

int8_t test_minute = 0, test_prev_minute = 0;
int8_t test_hour = 0, test_prev_hour = 0;

void process_time() {
    // get_time_and_print();
    // cuckoo_test();
    // step_motor_forward(6);
    now = DS1302_GetDateTime();

#ifdef SPEEDUP_TEST // 1sec = 1min
    now.hour = now.minute;
    now.hour += 12;
    if (now.hour > 12) now.hour %= 12;
    now.minute = now.second;
    now.second = 0;
#endif

    print_time(now);
    if (now.minute != prev_time.minute) {
        step_motor_forward(6);
        if (now.hour != prev_time.hour) {
            cuckoo_hour_add1 = now.hour + 1;
            CuckooBits.flag = 1;
        }
    }
    prev_time = now;
}

uint8_t chiming_melody_counter = 0;

void timer0_ISR() {
    // 33.6/0.2
    if (CuckooBits.flag == 0 && chiming_melody_counter == 0) return;

    if (chiming_melody_counter == 0) {
        LATAbits.LA1 = 1;
    }
    chiming_melody_counter++;
    SendNumberUInt8(chiming_melody_counter);
    SendString("\r\n");
    if (chiming_melody_counter > 34) {
        LATAbits.LA1 = 0;
        // CuckooBits.flag = 0;
        chiming_melody_counter = 0; 
        // __delay_ms(2000);
    }
}


void __interrupt(high_priority) Hi_ISR(void) {
    // Timer0_ISR(timer0_ISR);
    // Timer1_ISR(process_time);
    // Timer3_ISR(TMR3_ISR);
}

void ee_test() {
//    EEPROM_write(0x23, 0x91);
    while (1) {
        SendNumberUInt8(EEPROM_read(0x23));
        SendString("\r\n");
        __delay_ms(1000);
    }
}


void main(void) {

    SYSTEM_Initialize();
    
    Timer0_set_ms(1000);
    Timer0_start();
    Timer1_set_ms(1000);
    Timer1_start();
    Timer3_set_ms(500);
    Timer3_start();
    PWM_set_period(20000);
    PWM_start();
    
    if(!DS1302_GetIsRunning())
    {
        DS1302_SetIsRunning(1);
    }

    TRISDbits.TRISD3 = 0;

//    step_motor_test();
    ee_test();

    while(1);
    return;
}

void SYSTEM_Initialize(void) {
    UART_Initialize();
    step_motor_init();
    DS1302_Begin();
    Timer0_init();
    Timer1_init();
    Timer3_init();
    PWM_init();
    ADC_init();
    // INTCONbits.INT0IE = 1;
    // INTCON3bits.INT1IE = 1;
    // INTCON3bits.INT2IE = 1;
    INTCONbits.INT0IF = 0;
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT2IF = 0;
}