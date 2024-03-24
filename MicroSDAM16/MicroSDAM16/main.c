#define F_CPU 20000000UL
#include "Link.c"



#include <AM16_Timer_Functions.c>
#include <USART_AM16.c>
#include <SPI_ATM16.c>
#include <I2C_AM16.c>
#include <AM16_RTC_.c>
#include <MicroSD_AM16.c>
#include <AM16_FAT32.c>
#include <FAT32_SSD1360_Driver.c>
#include <AM16_SSD1306_OLED.c>







int main(void)
{

	//CS pin for micro SD card
	DDRA=0x1;

	//Create register to store time & date
	char CurrentTimeRegister[7];


	//Start Communications
	SPI_MAST_Init(0,128,0,0,0);
	USART_init(0,8,57600);
	I2C_Init();
    SSD1306_Init(0,255);

	

	//Initiate SD
	char SDIS =SD_Init();


	// Send Status of SD boot up
	USART_Send("\r\nHello Void. I have awoken\r\n Response | ");
	USART_Int_Str(SDIS,0);
	USART_Send("\r\n");





if(SDIS==5){
		// Find Meta information about the FAT32

FAT32_Init(0);

SSD1306_Clear_Screen_Sector();
// 
while(1){
 SSD1306_Display_Time(0,0);
_delay_ms(30000);

}
}

	//Ending Block



	//Get time to update the CurrentTimeRegister
	RTC_Clock_Read(CurrentTimeRegister);
	RTC_Calendar_Read(CurrentTimeRegister);
	USART_Send("\r\n Completed At: ");


	//Send ending execution time

	USART_Send("\r\n Day =");
	USART_Int_Str(CurrentTimeRegister[4],0);
	USART_Send("\r\n Date =");
	USART_Int_Str(CurrentTimeRegister[6],0);
	USART_Send("/");
	USART_Int_Str(CurrentTimeRegister[5],0);
	USART_Send("/");
	USART_Int_Str(CurrentTimeRegister[7],0);
	USART_Send("\r\n Time =");
	USART_Int_Str(CurrentTimeRegister[2],0);
	USART_Send(":");
	USART_Int_Str(CurrentTimeRegister[1],0);
	USART_Send(":");
	USART_Int_Str(CurrentTimeRegister[0],0);
	USART_Send("\r\n\r\n\r\n");

}