#define F_CPU 20000000UL// Define CPU frequency here 20MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#define FNAME "1306CHR"

void SSD1306_Display_Char(uint16_t chr,uint8_t x, uint8_t y){
	
	
uint8_t tempA[4];
chr = (chr-0x20)*5;
x*=5;
FAT32_FILE_Read_Sector_In_Cluster(FNAME,0,0);
tempA[0]=DataBuff[chr];
tempA[1]=DataBuff[chr+1];
tempA[2]=DataBuff[chr+2];
tempA[3]=DataBuff[chr+3];
tempA[4]=DataBuff[chr+4];
FAT32_FILE_Read_Sector_In_Cluster(FNAME,0,1);

SSD1306_Byte(x,y,DataBuff,tempA[0]);
SSD1306_Byte(x+1,y,DataBuff,tempA[1]);
SSD1306_Byte(x+2,y,DataBuff,tempA[2]);
SSD1306_Byte(x+3,y,DataBuff,tempA[3]);
SSD1306_Byte(x+4,y,DataBuff,tempA[4]);
FAT32_FILE_Write_Sector_In_Cluster(FNAME,0,1);
}

void SSD1306_Clear_Screen_Sector(){
	memset(DataBuff,0,512);
	FAT32_FILE_Write_Sector_In_Cluster(FNAME,0,1);
	FAT32_FILE_Read_Sector_In_Cluster(FNAME,0,1);
	SSD1306_Display(DataBuff);
}


void SSD1306_Display_String(uint8_t Data[], uint8_t x, uint8_t y){
for (uint8_t i =0; i<strlen(Data);i++)
	SSD1306_Display_Char(Data[i],x+i,y);
SSD1306_Display(DataBuff);	
}

void SSD1306_ltoa(uint32_t num,uint8_t x,uint8_t y, uint8_t base){
uint8_t temp[10];
uint8_t flag=0;
ltoa(num,temp,base);

switch(base){
	case 2: if(num<2){SSD1306_Display_String("0",x,y); flag=1;}
	break;
	case 10: if(num<10){SSD1306_Display_String("0",x,y);flag=1;}
	break;
	case 16: if(num<16){SSD1306_Display_String("0",x,y);flag=1;}
	break;
} 

if(flag==1){
SSD1306_Display_String(temp,x+1,y);
}else
SSD1306_Display_String(temp,x,y);


}

#if defined(RTC)
void SSD1306_Display_Time(uint8_t Offsetx, uint8_t Offsety){
char timeReg[7];
RTC_Clock_Read(timeReg);
RTC_Calendar_Read(timeReg);	
	
SSD1306_ltoa(timeReg[2],Offsetx,Offsety,10);	
SSD1306_Display_String(":",Offsetx+2,Offsety);
SSD1306_ltoa(timeReg[1],Offsetx+3,Offsety,10);

//Seconds are commented out here. 	
// SSD1306_Display_String(":",Offsetx+5,Offsety);
// SSD1306_ltoa(timeReg[0],Offsetx+6,Offsety,10);
Offsety+=8;	
SSD1306_ltoa(timeReg[6],Offsetx,Offsety,10);
SSD1306_Display_String("/",Offsetx+2,Offsety);
SSD1306_ltoa(timeReg[5],Offsetx+3,Offsety,10);		
SSD1306_Display_String("/",Offsetx+5,Offsety);		
SSD1306_ltoa(timeReg[7],Offsetx+6,Offsety,10);		
}

#endif