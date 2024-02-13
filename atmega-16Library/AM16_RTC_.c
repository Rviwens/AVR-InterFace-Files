#define F_CPU 20000000UL	// Define CPU frequency here 20MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>  
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#define Device_Write_address	0xD0	/* Define RTC DS1307 slave write address */
#define Device_Read_address	0xD1	/* Make LSB bit high of slave address for read */
#define TimeFormat12		0x40	/* Define 12 hour format */



uint8_t second,minute,hour,day,date,month,year;

void RTC_Clock_Write(char _hour, char _minute, char _second, char AMPM)
{
		_second= ((int)((_second/10)<<4))|(_second-((int)(_second/10)*10));
		_minute= ((int)((_minute/10)<<4))|(_minute-((int)(_minute/10)*10));
		_hour= ((int)((_hour/10)<<4))|(_hour-((int)(_hour/10)*10));
		
	_hour|=(AMPM<<5);
	_hour|=(1<<6);
	I2C_Start(Device_Write_address);/* Start I2C communication with RTC */
I2C_Write(0);			/* Write 0 address for second */
	I2C_Write(_second);		/* Write second on 00 location */
	I2C_Write(_minute);		/* Write minute on 01(auto increment) location */
	I2C_Write(_hour);		/* Write hour on 02 location */
 	I2C_Stop();			/* Stop I2C communication */

	
}

void RTC_Read_Clock(char *str)
{

	I2C_Start(Device_Write_address);/* Start I2C communication with RTC */
	I2C_Write(0);	/* Write address to read */
	I2C_Repeated_Start(Device_Read_address);/* Repeated start with device read address */

	second = I2C_Read_Ack();	/* Read second */
	minute = I2C_Read_Ack();	/* Read minute */
	hour = I2C_Read_Nack();		/* Read hour with Nack */
	I2C_Stop();			/* Stop i2C communication */
	
	uint8_t AMPM =hour&0b00100000;
	
	second= (second&0b00001111)+(((second&0b01110000)>>4)*10);
	minute= (minute&0b00001111)+(((minute&0b01110000)>>4)*10);
    hour= (hour&0b00001111)+(((hour&0b00010000)>>4)*10);
	

	str[0]=second;
	str[1]=minute;
	str[2]=hour;
	str[3]=AMPM;
}

void RTC_Calendar_Write(char _day, char _date, char _month, char _year)
{
	_date= ((int)((_date/10)<<4))|(_date-((int)(_date/10)*10));
	_month= ((int)((_month/10)<<4))|(_month-((int)(_month/10)*10));
    _year= ((int)((_year/10)<<4))|(_year-((int)(_year/10)*10));
	I2C_Start(Device_Write_address);/* Start I2C communication with RTC */
	I2C_Write(3);			/* Write 3 address for day */
	I2C_Write(_day);		/* Write day on 03 location */
	I2C_Write(_date);		/* Write date on 04 location */
	I2C_Write(_month);		/* Write month on 05 location */
	I2C_Write(_year);		/* Write year on 06 location */
	I2C_Stop();			/* Stop I2C communication */
}




void RTC_Read_Calendar(char*str)
{
	I2C_Start(Device_Write_address);
	I2C_Write(3);
	I2C_Repeated_Start(Device_Read_address);

	day = I2C_Read_Ack();		/* Read day */
	date = I2C_Read_Ack();		/* Read date */
	month = I2C_Read_Ack();		/* Read month */
	year = I2C_Read_Nack();		/* Read the year with Nack */
	I2C_Stop();			/* Stop i2C communication */
	
	date= (date&0b00001111)+(((date&0b00110000)>>4)*10);
		month= (month&0b00001111)+(((month&0b00010000)>>4)*10);
		year= (year&0b00001111)+(((year&0b11110000)>>4)*10);
	str[4]=day;
	str[5]=date;
	str[6]=month;
	str[7]=year;
}

