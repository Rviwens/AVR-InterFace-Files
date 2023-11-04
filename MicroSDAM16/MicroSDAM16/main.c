#include "Link.c"
#include <AM16_Timer_Functions.c>
#include <USART_AM16.c>
#include <SPI_ATM16.c>
#include <MicroSD_AM16.c>
#include <AM16_FAT32.c>

//#define SDcsp PINA0

int main(void)
{

USART_init(0,8,9600);
SPI_MAST_Init(0,128,0,0,0);


char SDIS = SD_Init();
USART_Send("\r\nHello Void. I have awoken\r\n Response | ");
USART_Int_Str(SDIS,0);


if(SDIS!=5){
	
	
SD_Reset();
}


if(SDIS==5){
FAT32_Init();
USART_Send("\r\n");

FAT32_Read_File("HELLO");
}
}
