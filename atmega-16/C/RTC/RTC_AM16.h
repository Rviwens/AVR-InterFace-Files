#ifndef RTC_GUARD
#define RTC_GUARD

#ifndef F_PU
#define F_CPU 20000000UL	// Define CPU frequency here 20MHZ
#endif

#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <I2C/I2C_AM16.h>

#define Device_Write_address	0xD0	/* Define RTC DS1307 slave write address */
#define Device_Read_address	0xD1	/* Make LSB bit high of slave address for read */
#define TimeFormat12		0x40	/* Define 12 hour format */

uint8_t second,minute,hour,day,date,month,year;
/*
* Writes the current time to the RTC
* @param _hour- The current Hour
* @param _minute-The current minute
* @param _second- The current second
* @param AMPM - 0 if AM, 1 if PM
*/
void RTC_Clock_Write(char _hour, char _minute, char _second, char AMPM);
/*
* Reads the current time from the RTC
* @param str - Pointer to an array[7] that will store the time and date
*/
void RTC_Clock_Read(uint8_t *str);
/*
* Writes the current time to the RTC
* @param _day- The current day (0->6)
* @param _date-The current date
* @param _month- The current month
* @param _year - the current year's last two digits
*/
void RTC_Calendar_Write(char _day, char _date, char _month, char _year);
/*
* Reads the current date from the RTC
* @param str - Pointer to an array[7] that will store the time and date
*/
void RTC_Calendar_Read(uint8_t *str);
#endif