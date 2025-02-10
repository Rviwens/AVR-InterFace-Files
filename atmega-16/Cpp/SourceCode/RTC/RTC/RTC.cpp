


#include "RTC.h"

#if defined (I2C)
void RTC_Clock_Write(char _hour, char _minute, char _second, char AMPM)
{
	_second= ((int)((_second/10)<<4))|(_second-((int)(_second/10)*10));
	_minute= ((int)((_minute/10)<<4))|(_minute-((int)(_minute/10)*10));
	_hour= ((int)((_hour/10)<<4))|(_hour-((int)(_hour/10)*10));
	
	_hour|=(AMPM<<5);
	_hour|=(1<<6);
	i2c.Start(Device_Write_address);/* Start I2C communication with RTC */
	i2c.Write(0);			/* Write 0 address for second */
	i2c.Write(_second);		/* Write second on 00 location */
	i2c.Write(_minute);		/* Write minute on 01(auto increment) location */
	i2c.Write(_hour);		/* Write hour on 02 location */
	i2c.Stop();			/* Stop I2C communication */

	
}

void RTC_Clock_Read(char *str)
{

	i2c.Start(Device_Write_address);/* Start I2C communication with RTC */
	i2c.Write(0);	/* Write address to read */
	i2c.Repeated_Start(Device_Read_address);/* Repeated start with device read address */

	second = i2c.Read_Ack();	/* Read second */
	minute = i2c.Read_Ack();	/* Read minute */
	hour = i2c.Read_Nack();		/* Read hour with Nack */
	i2c.Stop();			/* Stop i2C communication */
	
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
	i2c.Start(Device_Write_address);/* Start I2C communication with RTC */
	i2c.Write(3);			/* Write 3 address for day */
	i2c.Write(_day);		/* Write day on 03 location */
	i2c.Write(_date);		/* Write date on 04 location */
	i2c.Write(_month);		/* Write month on 05 location */
	i2c.Write(_year);		/* Write year on 06 location */
	i2c.Stop();			/* Stop I2C communication */
}




void RTC_Calendar_Read(char*str)
{
	i2c.Start(Device_Write_address);
	i2c.Write(3);
	i2c.Repeated_Start(Device_Read_address);

	day = i2c.Read_Ack();		/* Read day */
	date = i2c.Read_Ack();		/* Read date */
	month = i2c.Read_Ack();		/* Read month */
	year = i2c.Read_Nack();		/* Read the year with Nack */
	i2c.Stop();			/* Stop i2C communication */
	
	date= (date&0b00001111)+(((date&0b00110000)>>4)*10);
	month= (month&0b00001111)+(((month&0b00010000)>>4)*10);
	year= (year&0b00001111)+(((year&0b11110000)>>4)*10);
	str[4]=day;
	str[5]=date;
	str[6]=month;
	str[7]=year;
}
#endif
