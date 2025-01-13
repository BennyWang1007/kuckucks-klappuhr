#include "step_motor.h"

#include <xc.h>
#include <pic18f4520.h>


void step_motor_init() {
    TRISC = 0x00;
    StepMotorbits.DATA = 0x00;
    step_motor_phase = 0;
}

void step_motor_step(int16_t degree) {
    accumulated_degree += degree;
    target_step = degree * MOTOR_STEP / 360;
    int16_t step = target_step - accumulated_step;
    if (step < 0) step += MOTOR_STEP;
    for (int i = 0; i < step; i++) {
        if (LATC == 0x08) LATC = 0x01;
        else LATC <<= 1;
        __delay_ms(MOTOR_DELAY);
    }
    accumulated_step = target_step % MOTOR_STEP;
}

void step_motor_test() {
    int16_t degree;
    int16_t counter = 0;
    while (1) {
        switch (step_motor_phase) {
        case 0:
            StepMotorbits.DATA = 0x08;
            step_motor_phase += 1;
            break;
        case 1:
            StepMotorbits.DATA = 0x04;
            step_motor_phase += 1;
            break;
        case 2:
            StepMotorbits.DATA = 0x02;
            step_motor_phase += 1;
            break;
        case 3:
            StepMotorbits.DATA = 0x01;
            step_motor_phase = 0;
            break;
        default:
            step_motor_phase = 0;
            break;
        }
        counter ++;
        // if (counter == MOTOR_STEP) break;
        __delay_ms(MOTOR_DELAY);
    }
}

