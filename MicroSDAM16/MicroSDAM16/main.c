#include "Link.c"
#include <AM16_Timer_Functions.c>
#include <USART_AM16.c>
#include <SPI_ATM16.c>
#include <MicroSD_AM16.c>

int main(void)
{

USART_init(0,8,9600);
SPI_MAST_Init(0,128,0,0,0);


char SDIS = SD_Init();
USART_Send("\r\nHello Void. I have awoken\r\n Response | ");
USART_Int_Str(SDIS,0);


if(SDIS==5){
FAT32_Init();



// SD_RSB(StatusBuff,0,&token);
// USART_Send("\r\n");
// 
// for (int u =0; u<512; u++)
// {
// 		 USART_Send("\r\n |");
// 	 	 USART_Send("Index =");
// 	 	 USART_Int_Str(u,0);
// 	 	 USART_Send("| ");	
// USART_Int_Str(StatusBuff[u],0);
// 
// }


USART_Send("\r\n");
FAT32_Open_File("HELLO");



// for (long gh =RDS; gh<=(0xFF+RDS); gh++)
// {
// SD_RSB(StatusBuff,gh,&token);
// USART_Send("\r\n");	
// for (int u =0; u<512; u++)
// {
// USART_Int_Str(StatusBuff[u],0);	
// }
// }
// 




}
}