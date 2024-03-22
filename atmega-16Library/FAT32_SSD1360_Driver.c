#define F_CPU 20000000UL// Define CPU frequency here 20MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#define FNAME "1306CHR"

void SendChar(uint8_t chr){
//	uint16_t loc = ((int)chr-0x20)*5;
	FAT32_FILE_Read_Sector_In_Cluster(FNAME,0,0);
// DataBuff[loc];
// DataBuff[loc+1];	
// DataBuff[loc+2];	
// DataBuff[loc+3];
// DataBuff[loc+4];	
}




void DisplayChrFile(){
	char flagB= FAT32_FILE_Read_Sector_In_Cluster(FNAME,0,0);
	USART_Send("\r\n");
	if (flagB==0){
		USART_Send("\r\n");
		for(short i =0; i<512;i++)
			USART_Int_StrHEXRAW(DataBuff[i],0);
	}else{
		USART_Send("\r\n FlagB =");
		USART_Int_Str(flagB,0);
	}
}