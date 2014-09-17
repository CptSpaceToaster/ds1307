/*
 * ds1307 lib 0x01
 * 
 * copyright (c) Davide Gironi, 2013
 * modified by CptSpaceToaster 09/16/2014
 * 
 * Released under GPLv3.
 * Please refer to LICENSE file for licensing information.
 */


#include "avr/io.h"
#include "avr/pgmspace.h"
#include "util/delay.h"

#include "ds1307.h"
#include "i2cmaster.h"

/*
 * days per month
 */
const uint8_t ds1307_daysinmonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };

/*
 * transform decimal value to bcd
 */
uint8_t ds1307_dec2bcd(uint8_t val) {
	return val + 6 * (val / 10);
}

/*
 * transform bcd value to deciaml
 */
static uint8_t ds1307_bcd2dec(uint8_t val) {
	return val - 6 * (val >> 4);
}

/*
 * get number of days since 2000/01/01 (valid for 2001..2099)
 */
static uint16_t ds1307_date2days(uint8_t y, uint8_t m, uint8_t d) {
	uint16_t days = d;
	for (uint8_t i = 1; i < m; ++i)
		days += pgm_read_byte(ds1307_daysinmonth + i - 1);
	if (m > 2 && y % 4 == 0)
		++days;
	return days + 365 * y + (y + 3) / 4 - 1;
}

/*
 * get day of a week
 */
uint8_t ds1307_getdayofweek(uint8_t y, uint8_t m, uint8_t d) {
	uint16_t day = ds1307_date2days(y, m, d);
	return (day + 6) % 7;
}

/*
 * set date
 */
uint8_t ds1307_setdate(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
	//check bounds
	if (second < 0 || second > 59 ||
		minute < 0 || minute > 59 ||
		hour < 0   || hour > 23   ||
		day < 1    || day > 31    ||
		month < 1  || month > 12  ||
		year < 0   || year > 99)
		return 2;

	//sanitize day based on month
	if(day > pgm_read_byte(ds1307_daysinmonth + month - 1))
		return 1;

	//get day of week
	uint8_t dayofweek = ds1307_getdayofweek(year, month, day);

	//write date
	i2c_start_wait(DS1307_ADDR | I2C_WRITE);
	i2c_write(0x00); //start writing data at memory address 0x00
	i2c_write(ds1307_dec2bcd(second));
	i2c_write(ds1307_dec2bcd(minute));
	i2c_write(ds1307_dec2bcd(hour));
	i2c_write(ds1307_dec2bcd(dayofweek));
	i2c_write(ds1307_dec2bcd(day));
	i2c_write(ds1307_dec2bcd(month));
	i2c_write(ds1307_dec2bcd(year));
	i2c_write(0x00); //do nothing with the the external oscillator
	i2c_stop();

	return 0;
}

/*
 * set date
 */
uint8_t ds1307_setdate_s(time_t time) {
	//check bounds
	if (time.second < 0 || time.second > 59 ||
	    time.minute < 0 || time.minute > 59 ||
	    time.hour < 0   || time.hour > 23   ||
	    time.day < 1    || time.day > 31    ||
	    time.month < 1  || time.month > 12  ||
	    time.year < 0   || time.year > 99)
		return 2;

	//sanitize day based on month
	if(time.day > pgm_read_byte(ds1307_daysinmonth + time.month - 1))
		return 1;

	//get day of week
	uint8_t dayofweek = ds1307_getdayofweek(time.year, time.month, time.day);

	//write date
	i2c_start_wait(DS1307_ADDR | I2C_WRITE);
	i2c_write(0x00); //start writing data at memory address 0x00
	i2c_write(ds1307_dec2bcd(time.second));
	i2c_write(ds1307_dec2bcd(time.minute));
	i2c_write(ds1307_dec2bcd(time.hour));
	i2c_write(ds1307_dec2bcd(dayofweek));
	i2c_write(ds1307_dec2bcd(time.day));
	i2c_write(ds1307_dec2bcd(time.month));
	i2c_write(ds1307_dec2bcd(time.year));
	i2c_write(0x00); //do nothing with the the external oscillator
	i2c_stop();

	return 1;
}

/*
 * get date
 */
void ds1307_getdate(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second) {
	i2c_start_wait(DS1307_ADDR | I2C_WRITE);
	i2c_write(0x00); //start reading data at memory address 0x00???  This may not be necessary
	i2c_stop();

	i2c_rep_start(DS1307_ADDR | I2C_READ);
	*second = ds1307_bcd2dec(i2c_readAck() & 0x7F);
	*minute = ds1307_bcd2dec(i2c_readAck());
	*hour = ds1307_bcd2dec(i2c_readAck());
	i2c_readAck();
	*day = ds1307_bcd2dec(i2c_readAck());
	*month = ds1307_bcd2dec(i2c_readAck());
	*year = ds1307_bcd2dec(i2c_readNak());
	i2c_stop();
}

/*
 * get date
 */
void ds1307_getdate_s(time_t *time) {
	i2c_start_wait(DS1307_ADDR | I2C_WRITE);
	i2c_write(0x00); //start reading data at memory address 0x00???  This may not be necessary
	i2c_stop();

	i2c_rep_start(DS1307_ADDR | I2C_READ);
	time->second = ds1307_bcd2dec(i2c_readAck() & 0x7F);
	time->minute = ds1307_bcd2dec(i2c_readAck());
	time->hour = ds1307_bcd2dec(i2c_readAck());
	i2c_readAck();
	time->day = ds1307_bcd2dec(i2c_readAck());
	time->month = ds1307_bcd2dec(i2c_readAck());
	time->year = ds1307_bcd2dec(i2c_readNak());
	i2c_stop();
}
