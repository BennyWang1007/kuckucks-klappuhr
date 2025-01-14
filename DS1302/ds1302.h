/* 
 * File:   ds1302.h
 * Author: laptop
 *
 * Created on 2025?1?13?, ?? 4:01
 */

#ifndef DS1302_H
#define	DS1302_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct
{
    uint8_t yearFrom2000;
    uint8_t month;
    uint8_t dayOfMonth;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t dayOfWeek;
} DS1302_DateTime_t;


void DS1302_Begin(void);
char DS1302_GetIsWriteProtected(void);
void DS1302_SetIsWriteProtected(char);
char DS1302_GetIsRunning(void);
void DS1302_SetIsRunning(char);
uint8_t DS1302_GetTrickleChargeSettings(void);
void DS1302_SetDateTime(const DS1302_DateTime_t*);
DS1302_DateTime_t DS1302_GetDateTime(void);
void DS1302_SetMemory(uint8_t memoryAddress, uint8_t value);
uint8_t DS1302_GetReg(uint8_t);
void DS1302_SetReg(uint8_t, uint8_t);


#ifdef	__cplusplus
}
#endif

#endif	/* DS1302_H */

