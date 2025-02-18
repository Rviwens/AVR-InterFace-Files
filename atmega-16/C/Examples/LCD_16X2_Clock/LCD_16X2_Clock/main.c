
#ifndef F_PU
#define F_CPU 20000000UL	// Define CPU frequency here 20MHZ
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>


uint8_t DataBuff[511];

#define USARTTX


#include <LDC_16x2/LCD_AM16_16x2.c>
#include <USART/USART_AM16.c>
#include <I2C/I2C_AM16.c>
#include <RTC/RTC_AM16.c>


//char day[6] =["Sun","Mon","Tue","Wen","Thu","Fri","Sat"];

int main(void)
{
	//Create register to store time & date
	uint8_t CurrentTimeRegister[8];
	
	DDRB =0xFF;
	//Start Communications
	USART_init(0,8,57600);
	I2C_Init();
	LCD_Init(4);

	USART_Send("\r\nHello Void. I have awoken\r\n Response | ");

	//RTC_Clock_Write(4,50,30,1);

	while (1)
	{

		RTC_Clock_Read(CurrentTimeRegister);
		RTC_Calendar_Read(CurrentTimeRegister);

		_delay_ms(1000);
		LCD_Clear();
		LCD_Set_Cursor(0,0);

		LCD_Int_Str(CurrentTimeRegister[2],10);
		LCD_String(":");
		LCD_Int_Str(CurrentTimeRegister[1],10);
		LCD_String(":");
		LCD_Int_Str(CurrentTimeRegister[0],10);

		if (CurrentTimeRegister[3]==0)
		LCD_String(" AM");
		else
		LCD_String(" PM");

		LCD_Set_Cursor(1,0);
		LCD_Int_Str(CurrentTimeRegister[4],10);
		LCD_String("  ");
		LCD_Int_Str(CurrentTimeRegister[6],10);
		LCD_String("/");
		LCD_Int_Str(CurrentTimeRegister[5],10);
		LCD_String("/");
		LCD_Int_Str(CurrentTimeRegister[7],10);

	}

}