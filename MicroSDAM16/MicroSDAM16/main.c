#include "Link.c"
#include <AM16_Timer_Functions.c>
#include <USART_AM16.c>
#include <SPI_ATM16.c>
#include <MicroSD_AM16.c>

int main(void)
{

// memset(buff,59,512);

DDRA=0xF;
PORTA=0xF;

USART_init(0,8,9600);
SPI_MAST_Init(0,128,0,0,0);


char SDIS = SD_Init();
USART_Send("\r\nHello Void. I have awoken\r\n Response | ");
USART_Int_Str(SDIS,0);

 if(SDIS==5){
FAT32_Init();
FAT32_Open_File("VOID");
SD_RSB(StatusBuff,0x5700576,&token);	
return 0xFF; 	
}
}