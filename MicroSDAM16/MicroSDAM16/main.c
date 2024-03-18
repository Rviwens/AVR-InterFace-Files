#define F_CPU 20000000UL
#include "Link.c"



#include <AM16_Timer_Functions.c>
#include <USART_AM16.c>
#include <SPI_ATM16.c>
#include <I2C_AM16.c>


#include <AM16_RTC_.c>


#include <MicroSD_AM16.c>


#include <AM16_FAT32.c>








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


//Initiate SD
char SDIS =SD_Init();


// Send Status of SD boot up
USART_Send("\r\nHello Void. I have awoken\r\n Response | ");
USART_Int_Str(SDIS,0);
USART_Send("\r\n");





if(SDIS==5){
// Find Meta information about the FAT32

FAT32_Init(0);

// for(int i=0; i<200; i++)
// FAT32_AppendNewClust("FLOWER");

FAT32_LocationOfLastClustInFile("FLOWER");



// Fix Appending cluster to move into the next sector in the FAT.  






// memset(DataBuff,0,512);
// 	DataBuff[0]=0x57;
// 	DataBuff[1]=0x4F;
// 	DataBuff[2]=0x52;
// 	DataBuff[3]=0x4C;
// 	DataBuff[4]=0x44;
// 	DataBuff[5]=0x53; 
// 	DataBuff[6]=0x54;
// 	DataBuff[7]=0x41;
// 	DataBuff[8]=0x52;
// 
// 	
// char flagA= FAT32_FILE_Write_Sector_In_Cluster("FLOWER",0,0);
// char flagB= FAT32_FILE_Read_Sector_In_Cluster("FLOWER",0,0);
// 
// USART_Send("\r\n FlagA =");
// USART_Int_Str(flagA,0);
// USART_Send("\r\n FlagB =");
// USART_Int_Str(flagB,0);
// 
//  if (flagA==0)
//  if (flagB==0){
// 
// 	 
//  USART_Send("\r\n");
//  			for(short i =0; i<512;i++)
//  			USART_Int_StrHEXRAW(DataBuff[i],0);
//  	
//   
// }






//Delete File
//FAT32_FILE_Delete("FLOWER");


// Create File
// RTC_Read_Clock(CurrentTimeRegister);
// RTC_Read_Calendar(CurrentTimeRegister);
// FAT32_FILE_Create("DARKEN","txt",CurrentTimeRegister);


}

//Ending Block



//Get time to update the CurrentTimeRegister
RTC_Read_Clock(CurrentTimeRegister); 
RTC_Read_Calendar(CurrentTimeRegister);
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