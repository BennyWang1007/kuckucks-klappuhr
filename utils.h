/* 
 * File:   utils.h
 * Author: laptop
 *
 * Created on 2025?1?2?, ?? 7:43
 */

#ifndef UTILS_H
#define	UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif


char itoa(unsigned int, char*, int);
void print_time(DS1302_DateTime_t time);
uint16_t timediff_in_min(DS1302_DateTime_t from, DS1302_DateTime_t to);


#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

