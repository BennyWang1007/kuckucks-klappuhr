#ifndef I2C_H
#define I2C_H

#include "../constants.c"


typedef union {
    struct {
        unsigned UNUSED                 :3;
        unsigned SCK                    :1;
        unsigned SDA                    :1;
        unsigned RESET                  :1;
    };
} Clockbits_t;
// extern volatile Clockbits_t Clockbits __at(LATC);
extern volatile Clockbits_t Clockbits __at(0xF8B);

void I2C_init(void);
void I2C_read(void);
void I2C_write(uint8 data);
void I2C_start(void);
void I2C_stop(void);
void I2C_ack(void);
void I2C_int(void);

#endif