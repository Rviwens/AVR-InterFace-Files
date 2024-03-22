#define F_CPU 20000000UL
#include "Link.c"

#include <AM16_Timer_Functions.c>
#include <USART_AM16.c>
#include <SPI_ATM16.c>
#include <I2C_AM16.c>
#include <AM16_RTC_.c>
#include <MicroSD_AM16.c>
#include <AM16_FAT32.c>
#include <AM16_SSD1306_OLED.c>
#include <FAT32_SSD1360_Driver.c>






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
OLED_Init(0,255);
OLED_Byte(0,5,DataBuff,0x36);
OLED_Byte(1,5,DataBuff,0x49);
OLED_Byte(2,5,DataBuff,0x49);
OLED_Byte(3,5,DataBuff,0x49);
OLED_Byte(4,5,DataBuff,0x36);
OLED_Display(DataBuff);



//Initiate SD
char SDIS =SD_Init();


// Send Status of SD boot up
USART_Send("\r\nHello Void. I have awoken\r\n Response | ");
USART_Int_Str(SDIS,0);
USART_Send("\r\n");

if(SDIS==5){
// Find Meta information about the FAT32
FAT32_Init(0);

DisplayChrFile();

}


// char flagA= FAT32_FILE_Write_Sector_In_Cluster("1306CHR",0,0);
// USART_Send("\r\n FlagA =");
// USART_Int_Str(flagA,0);











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








/*
memset(DataBuff,0,512);
DataBuff[0]=0x57;
DataBuff[1]=0x4F;
DataBuff[2]=0x52;
DataBuff[3]=0x4C;
DataBuff[4]=0x44;
DataBuff[5]=0x53;
DataBuff[6]=0x54;
DataBuff[7]=0x41;
DataBuff[8]=0x52;

char flagA= FAT32_FILE_Write_Sector_In_Cluster("DARKEN",2,4);
char flagB= FAT32_FILE_Read_Sector_In_Cluster("DARKEN",2,4);

USART_Send("\r\n FlagA =");
USART_Int_Str(flagA,0);
USART_Send("\r\n FlagB =");
USART_Int_Str(flagB,0);

if (flagA==0)
if (flagB==0){


	USART_Send("\r\n");
	for(short i =0; i<512;i++)
	USART_Int_StrHEXRAW(DataBuff[i],0);


}



USART_Send("\r\n Fat: \r\n");
SD_RSB(DataBuff,FATStartSec);
for(short i =0; i<512;i++)
USART_Int_StrHEXRAW(DataBuff[i],0);

}
*/
// uint16_t tempC=0;
// 
// 
// for(int i=0; i<512;i++){
// 
// if((DataBuff[tempC]=='|')&(DataBuff[tempC+2]=='|')){
// tempC+=3;
// DataBuff[tempC]=0x0;
// DataBuff[tempC+1]=0x00;
// DataBuff[tempC+2]=0x0;
// DataBuff[tempC+3]=0x0;
// DataBuff[tempC+4]=0x0;
// DataBuff[tempC+5]=0x0;
// DataBuff[tempC+6]=0x0;
// DataBuff[tempC+7]=0x0;
// DataBuff[tempC+8]=0x0;
// DataBuff[tempC+9]=0x0;
// }
// 
// tempC++;
// }
// 
// USART_Send("\r\n");
// for(short i =0; i<512;i++)
// USART_Int_StrHEXRAW(DataBuff[i],0);
