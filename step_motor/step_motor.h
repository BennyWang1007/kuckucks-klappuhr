#ifndef _STEP_MOTOR_H
#define _STEP_MOTOR_H

#include "../constants.c"

#define MOTOR_STEP 2048
#define MOTOR_DELAY 5

int16 accumulated_degree = 0;
int16 accumulated_step = 0;
int16 target_step = 0;
int8 step_motor_phase;

typedef union {
    struct {
        unsigned UNUSED                 :4;
        unsigned DATA                   :4;
    };
} StepMotorbits_t;
// extern volatile StepMotorbits_t StepMotorbits __at(LATD);
extern volatile StepMotorbits_t StepMotorbits __at(0xF8C);

void step_motor_init(void);
void step_motor_step(int16 degree);
void step_motor_test(void);

#endif  /* _STEP_MOTOR_H */