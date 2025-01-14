/*
 * File:   utils.c
 * Author: laptop
 *
 * Created on 2025?1?2?, ?? 7:40
 */


#include <xc.h>

#include <stdlib.h>
#include "step_motor/step_motor.h"

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
    unsigned int digit;

    if (len == 0)
        return -1;

    do
    {
        digit = sum % 10;
        str[i++] = '0' + digit;
        sum /= 10;
    } while (sum && (i < (len - 1)));

    if (i == (len - 1) && sum)
        return -1;

    str[i] = '\0';

    // Reverse the string
    reverse((char*)str, i);

    return 0;
}

uint16_t min2step(uint32_t min)   // min in 0 ~ 1440(1 day)
{
    uint16_t step = min * MOTOR_STEP / 1440;
    return step;
}

uint16_t timediff_in_min(uint8_t h1, uint8_t m1, uint8_t h2, uint8_t m2)
{
    uint16_t diff = 0;
    uint16_t t1 = h1 * 60 + m1;
    uint16_t t2 = h2 * 60 + m2;

    if (t2 >= t1)   // new time after old time in same day
        diff = t2 - t1;
    else    // new time in next day
        diff = t1 + 1440 - t1;

    return diff;
}