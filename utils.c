/*
 * File:   utils.c
 * Author: laptop
 *
 * Created on 2025?1?2?, ?? 7:40
 */


#include <xc.h>

#include <stdlib.h>
#include "DS1302/ds1302.h"
#include "step_motor/step_motor.h"
#include "UART/uart.h"
#include "utils.h"

void reverse(char* str, int length)
{
    int start = 0;
    int end = length - 1;
    char temp;
    while (start < end)
    {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char itoa(unsigned int num, char* str, int len)
{
    unsigned int sum = num;
    unsigned char i = 0;
    char digit;

    if (len == 0)
        return (char)(-1);

    do
    {
        digit = sum % 10;
        str[i++] = '0' + digit;
        sum /= 10;
    } while (sum && (i < (len - 1)));

    if (i == (len - 1) && sum)
        return (char)(-1);

    str[i] = '\0';

    // Reverse the string
    reverse((char*)str, i);

    return 0;
}

uint16_t min2step(uint32_t min)   // min in 0 ~ 1440(1 day)
{
    uint16_t step = (uint16_t)(min * MOTOR_STEP / 1440);
    return step;
}

uint16_t timediff_in_min(DS1302_DateTime_t from, DS1302_DateTime_t to)
{
    uint16_t diff = 0;
    uint16_t t1 = from.hour * 60 + from.minute;
    uint16_t t2 = to.hour * 60 + to.minute;

    if (t2 >= t1)   // new time after old time in same day
        diff = t2 - t1;
    else    // new time in next day
        diff = t2 + 1440 - t1;

    return diff;
}

void print_time(DS1302_DateTime_t time) {
    SendNumberUInt8(time.yearFrom2000);
    UART_Write('/');
    SendNumberUInt8(time.month);
    UART_Write('/');
    SendNumberUInt8(time.dayOfMonth);
    UART_Write(' ');
    SendNumberUInt8(time.hour);
    UART_Write(':');
    SendNumberUInt8(time.minute);
    UART_Write(':');
    SendNumberUInt8(time.second);
    UART_Write_Text("\r\n");
}