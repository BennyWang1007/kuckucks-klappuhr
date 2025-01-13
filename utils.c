/*
 * File:   utils.c
 * Author: laptop
 *
 * Created on 2025?1?2?, ?? 7:40
 */


#include <xc.h>

#include <stdlib.h>

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

char itoa(unsigned int num, unsigned char* str, int len)
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
