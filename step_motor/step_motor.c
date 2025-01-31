#include "step_motor.h"

#include <xc.h>
#include <pic18f4520.h>


void step_motor_init() {
    TRISD &= 0x0F;  // RD4-7 output
    StepMotorbits.DATA = 0x00;
    step_motor_phase = 0;
}

void step_motor_step() {
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
}

void step_motor_forward(int16_t degree) {
    if (degree == 0) return;
    accumulated_degree += degree;
    target_step = (uint16_t)((uint32_t)(accumulated_degree) * MOTOR_STEP / 360);
    // SendString("target_step = ");
    // SendNumberUInt16(target_step);
    // SendString("\r\n");
    uint16_t step;
    if (accumulated_step < target_step) step = target_step - accumulated_step;
    else step = MOTOR_STEP - accumulated_step + target_step;
    // SendString("step = ");
    // SendNumberUInt16(step);
    // SendString("\r\n");
    for (int i = 0; i < step; i++) {
        step_motor_step();
        __delay_ms(MOTOR_DELAY);
    }
    accumulated_step = target_step % MOTOR_STEP;
    accumulated_degree %= 360;
}

void step_motor_test() {
    while (1) {
        step_motor_step();
        __delay_ms(MOTOR_DELAY);
    }
}

