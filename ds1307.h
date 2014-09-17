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

//structures
typedef struct time {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} time_t;

//definitions
#define DS1307_ADDR (0x68<<1) //device address

#define DS1307_I2CINIT 1 //init i2c

//functions
extern void ds1307_init();
extern uint8_t ds1307_setdate(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
extern uint8_t ds1307_setdate_s(time_t time);
extern void ds1307_getdate(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second);
extern void ds1307_getdate_s(time_t *time);

#endif

