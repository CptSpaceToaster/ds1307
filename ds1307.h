/*
 * ds1307 lib 0x01
 * 
 * copyright (c) Davide Gironi, 2013
 * modified by CptSpaceToaster 09/16/2014
 * 
 * Released under GPLv3.
 * Please refer to LICENSE file for licensing information.
 * 
 * References: parts of the code taken from https://github.com/adafruit/RTClib
 */

#ifndef DS1307_H
#define DS1307_H

#include <stdbool.h>

//structures
typedef struct clock {
	uint8_t month;
	uint8_t day;
	uint8_t year;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} time_t;

//definitions
#define DS1307_ADDR (0x68<<1) //device address shifted over one

//functions
uint8_t ds1307_setdate(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
uint8_t ds1307_setdate_s(time_t time);
uint8_t ds1307_getdayofweek(uint8_t y, uint8_t m, uint8_t d);
bool ds1307_isleapyear(uint8_t year);
void ds1307_getdate(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second);
void ds1307_getdate_s(time_t *time);

#endif

