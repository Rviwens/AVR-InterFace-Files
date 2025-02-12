
#include <LCD_16X2.h>
#include <USART.h>
#include <I2C.h>
#include <RTC.h>


//char day[6] =["Sun","Mon","Tue","Wen","Thu","Fri","Sat"];

int main(void)
{
	//Create register to store time & date
	uint8_t CurrentTimeRegister[8];
	// Classes
	USART usart(0,8,57600);
	I2C i2c;
	RTC rtc;
	LCD_16X2 lcd(4);
	
	//IO
	DDRB =0xFF;
	
	//Start Communications
	i2c.Init();
	lcd.Init(4);

	usart.Send("\r\nHello Void. I have awoken\r\n Response | ");

	rtc.Clock_Write(4,42,0,1);
	while (1)
	{

		rtc.Clock_Read(CurrentTimeRegister);
		rtc.Calendar_Read(CurrentTimeRegister);

		_delay_ms(1000);
		lcd.Clear();
		lcd.Set_Cursor(0,0);

		lcd.Int_Str(CurrentTimeRegister[2],10);
		lcd.Str(":");
		lcd.Int_Str(CurrentTimeRegister[1],10);
		lcd.Str(":");
		lcd.Int_Str(CurrentTimeRegister[0],10);

		if (CurrentTimeRegister[3]==0)
		lcd.Str(" AM");
		else
		lcd.Str(" PM");

		lcd.Set_Cursor(1,0);
		lcd.Int_Str(CurrentTimeRegister[4],10);
		lcd.Str("  ");
		lcd.Int_Str(CurrentTimeRegister[6],10);
		lcd.Str("/");
		lcd.Int_Str(CurrentTimeRegister[5],10);
		lcd.Str("/");
		lcd.Int_Str(CurrentTimeRegister[7],10);

	}
}