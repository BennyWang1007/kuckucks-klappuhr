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
#pragma config BOREN = OFF    // Brown-out Reset Enable bit
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
//    SendString("ADC: ");
//    SendNumberInt16(adc_value);
//    SendString("\r\n");
    if (adc_value < PHOTORESISTOR_THRESHOLD) {
        // decoration lightup
        LATAbits.LA2 = 1;
    } else {
        // decoration lightdown
        LATAbits.LA2 = 0;
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
    if (CuckooBits.flag == 0 && CuckooBits.phase == 0) return;
//    SendString("Cuckoo Move\r\n");
    // SendNumberUInt8(cuckoo_hour_add1);
    // SendString("\r\n");
    if (CuckooBits.phase == 0) {
        cuckoo_out;
        cuckoo_down;
        CuckooBits.up = 0;
        CuckooBits.phase++;
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
    }
}

void TMR3_ISR() {
    button_ISR();
    cuckoo_move();
}

int8_t hour_buf;

void save_current_time(void);

void process_time() {

    now = DS1302_GetDateTime();
    hour_buf = now.hour + 12;
    if (hour_buf > 12) hour_buf %= 12;

    print_time(now);
    // if time updated
    if (now.minute != prev_time.minute) {
        // save now to EEPROM
        save_current_time();
        // move to next minute
        int16_t diff = timediff_in_min(prev_time, now);
        for (int i = 0; i < diff; i++) {
            step_motor_forward(6);
        }
        // if hour updated
        if (now.hour != prev_time.hour) {
            // cuckoo count
            cuckoo_hour_add1 = hour_buf + 1;
            // cuckoo flag, if this flag -> cuckoo move
            CuckooBits.flag = 1;
            SendString("Cuckoo\r\n");
        }
    }
    // update time
    prev_time = now;
}

uint8_t chiming_melody_counter = 0;

void timer0_ISR() {
    ADC_ISR();
    if (CuckooBits.flag == 0 && chiming_melody_counter == 0) return;

    if (chiming_melody_counter == 0) {
        // start chiming melody
        LATAbits.LA1 = 1;
    }
    chiming_melody_counter++;
    // SendNumberUInt8(chiming_melody_counter);
    // SendString("\r\n");
    // if (chiming_melody_counter > 34) {
    if (chiming_melody_counter > CHIMING_MELODY_DURATION) {
        // stop chiming melody
        LATAbits.LA1 = 0;
        chiming_melody_counter = 0; 
    }
}


void __interrupt(high_priority) Hi_ISR(void) {
    Timer0_ISR(timer0_ISR);
    Timer1_ISR(process_time);
    Timer3_ISR(TMR3_ISR);
}


void read_prev_time() {
    uint8_t tmp;
    tmp = EEPROM_read(0x00);
    prev_time.yearFrom2000 = tmp;
    tmp = EEPROM_read(0x01);
    prev_time.month = tmp;
    tmp = EEPROM_read(0x02);
    prev_time.dayOfMonth = tmp;
    tmp = EEPROM_read(0x03);
    prev_time.hour = tmp;
    tmp = EEPROM_read(0x04);
    prev_time.minute = tmp;
    tmp = EEPROM_read(0x05);
    prev_time.second = tmp;
    tmp = EEPROM_read(0x06);
    prev_time.dayOfWeek = tmp;
    print_time(prev_time);
}

void save_current_time() {
    EEPROM_write(0x00, now.yearFrom2000);
    EEPROM_write(0x01, now.month);
    EEPROM_write(0x02, now.dayOfMonth);
    EEPROM_write(0x03, now.hour);
    EEPROM_write(0x04, now.minute);
    EEPROM_write(0x05, now.second);
    EEPROM_write(0x06, now.dayOfWeek);
}


void clock_correction() {
    // read saved time in EEPROM to prev_time
    read_prev_time();
    // get now from DS1302
    now = DS1302_GetDateTime();
    
    SendString("Saved time: ");
    print_time(prev_time);
    SendString("Current time: ");
    print_time(now);

    // calculate diff in minutes
    uint16_t time_diff = timediff_in_min(prev_time, now);
    SendString("Time diff: ");
    SendNumberUInt16(time_diff);
    SendString("\r\n");
    // move the step motor
    step_motor_forward(time_diff * 6);
    // save now to EEPROM
    save_current_time();
    prev_time = now;
}

void set_EEPROM_time(uint8_t hour, uint8_t minute) {
   DS1302_DateTime_t newtime = {
       .yearFrom2000 = 25,
       .month = 1,
       .dayOfMonth = 15,
       .hour = hour,
       .minute = minute,
       .second = 0,
       .dayOfWeek = 3
   };
   now = newtime;
   save_current_time();
   prev_time = now;
}


void set_clock_time(uint8_t hour, uint8_t minute, uint8_t second) {
    DS1302_DateTime_t new = {
        .yearFrom2000 = 25,
        .month = 1,
        .dayOfMonth = 15,
        .hour = hour,
        .minute = minute,
        .second = second,
        .dayOfWeek = 3
    };
    DS1302_SetDateTime(&new);
}


void main(void) {
    
    // set time of timer module
    // DS1302_Begin();
    // set_clock_time(17, 32, 20);

    // set time of EEPROM
    // EEPROM_init();
    // set_EEPROM_time(17, 55);
    // while (1);

    // initialize cuckoo electromagnet
    LATAbits.LA1 = 0;
    TRISDbits.TRISD3 = 0;   // output
    CuckooBits.phase = 0;

    SYSTEM_Initialize();

    // set servo PPM period to 20ms
    PWM_set_period(20000);
    PWM_start();

    // start timers
    Timer0_set_ms(1000);
    Timer0_start();     // melody duration
    Timer1_set_ms(1000);
    Timer1_start();     // update time
    Timer3_set_ms(500);
    Timer3_start();     // button, ADC, cuckoo move
    
    // start the clock module if it is not running
    if(!DS1302_GetIsRunning()) {
        DS1302_SetIsRunning(1);
    }

    // step_motor_test();
    SendString("Halt\r\n");
    while(1);

    return;
}


void SYSTEM_Initialize(void) {
    step_motor_init();

    // chiming melody duration
    Timer0_init();

    // update time from clock module
    Timer1_init();

    // check button, ADC, cuckoo move
    Timer3_init();

    // init CCP for servo
    PWM_init();

    // init ADC module
    ADC_init();

    // init UART module
    UART_Initialize();

    // init EEPROM module
    EEPROM_init();

    // correct physical clock to internal clock
    DS1302_Begin();
    SendString("\r\n\r\n");
    INTCONbits.GIE = 0;
    clock_correction();
    INTCONbits.GIE = 1;

    // INTCONbits.INT0IE = 1;
    // INTCON3bits.INT1IE = 1;
    // INTCON3bits.INT2IE = 1;

    // clear button interrupt flag
    INTCONbits.INT0IF = 0;
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT2IF = 0;
}