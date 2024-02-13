
#define F_CPU 20000000UL// Define CPU frequency here 20MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>


#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 32
#define SSD1306_BUFFERSIZE (SSD1306_WIDTH*SSD1306_HEIGHT)/8

#define SSD1306_DEFAULT_ADDRESS 0x78
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_NOP 0xE3
#define OLED_Write_Address 0x3C

void OLED_Clear();



 void OLED_Invert(int Mode){
	 if( Mode==1){
	 I2C_Write(SSD1306_INVERTDISPLAY);}
	 else if(Mode==0){
	 I2C_Write(SSD1306_NORMALDISPLAY);}
 }

 void OLED_Contrast(uint8_t data){
	 I2C_Write(SSD1306_SETCONTRAST);
	 I2C_Write(data);
 }




void OLED_Init(short Invert, uint8_t contrast){
DDRD|=0xF0;
PORTD|=0xF0;
_delay_ms(100);
	
I2C_Start_Wait(SSD1306_DEFAULT_ADDRESS);
I2C_Write(SSD1306_DISPLAYOFF);
I2C_Write(SSD1306_SETDISPLAYCLOCKDIV);
I2C_Write(0xF0);
I2C_Write(SSD1306_SETMULTIPLEX);
I2C_Write(0x3F);
I2C_Write(SSD1306_SETDISPLAYOFFSET);
I2C_Write(0x00);
I2C_Write(SSD1306_SETSTARTLINE | 0x00);
I2C_Write(SSD1306_CHARGEPUMP);
I2C_Write(0x14);    
// Horizontal memory mode
I2C_Write(SSD1306_MEMORYMODE);
I2C_Write(0x00);
I2C_Write(SSD1306_SEGREMAP | 0x1);
I2C_Write(SSD1306_COMSCANDEC);
I2C_Write(SSD1306_SETCOMPINS);
I2C_Write(0x12);
OLED_Contrast(contrast);
I2C_Write(SSD1306_SETPRECHARGE);
I2C_Write(0xF1);
I2C_Write(SSD1306_SETVCOMDETECT); 
I2C_Write(0x40);	  
// We use internal charge pump
I2C_Write(SSD1306_CHARGEPUMP);
I2C_Write(0x14);	
I2C_Write(SSD1306_DISPLAYALLON_RESUME);
OLED_Invert(Invert);
// Turn display back on
I2C_Write(SSD1306_DISPLAYON);
//I2C_Stop();
OLED_Clear();
} 

uint8_t     OLEDlineNumber = 0, OLEDcursorPos = 0;
void OLED_SetCursor(uint8_t myLineNumber,uint8_t myColumnPosition) {
	if( (myLineNumber <= 0x07) && (myColumnPosition <= 127) ) {
		OLEDlineNumber = myLineNumber;     // global var: current line number
		OLEDcursorPos =  myColumnPosition; // global var: current cursor position

		I2C_Write(SSD1306_COLUMNADDR);
		I2C_Write(myColumnPosition);
		I2C_Write(127);
		
		I2C_Write(SSD1306_PAGEADDR);
		I2C_Write(myLineNumber);
		I2C_Write(7);
		// I2C_Start_NORETURN(0x78);
		// I2C_Write(0x40);
	}
}


 void OLED_Clear() {	


I2C_Start_NORETURN(0x78);
OLED_SetCursor(0,0);

	// TWI NOT using Wire.h   128 bytes (8 bits vertical) per row * 8 char rows = 1024
	// Send 64 TWI messages { START:SLAw:0x40:16 bytes of 0x00:STOP }  1024 data bytes total
	for (uint16_t scrClearIndex=0; scrClearIndex<512; scrClearIndex++) {  // (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8)
		I2C_Start_NORETURN(0x78);
	   I2C_Write(0x40);
		for (uint8_t x=0; x<16; x++) {
			I2C_Write(0x00);
			TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));
			scrClearIndex++;  // 16 bytes per I2C transmission__64 transmissions per screen
		}
		scrClearIndex--;
		I2C_Stop();
	}
	}

void OLED_Fill() {
	I2C_Start_NORETURN(0x78);
	OLED_SetCursor(0,0);

	// TWI NOT using Wire.h   128 bytes (8 bits vertical) per row * 8 char rows = 1024
	// Send 64 TWI messages { START:SLAw:0x40:16 bytes of 0x00:STOP }  1024 data bytes total
	for (uint16_t scrClearIndex=0; scrClearIndex<512; scrClearIndex++) {  // (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8)
		I2C_Start_NORETURN(0x78);
		I2C_Write(0x40);
		for (uint8_t x=0; x<16; x++) {
			I2C_Write(0xFF);
			TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));
			scrClearIndex++;  // 16 bytes per I2C transmission__64 transmissions per screen
		}
		scrClearIndex--;
		I2C_Stop();
	}
}




void OLED_Display(uint8_t *ScreenBuffer){
	OLED_Clear();

	I2C_Start_NORETURN(0x78);
	OLED_SetCursor(0,0);
	I2C_Stop();
	for (uint16_t ScrIndex=0; ScrIndex<1024; ScrIndex+=0) {  // (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8)
		I2C_Start_NORETURN(0x78);
		I2C_Write(0x40);

		// for (int i=0; i<16; i++)
		// {
		// I2C_Write(ScreenBuffer[ScrIndex]);
		// ScrIndex++;
		// }

		for (int i=0; i<16; i++)
		{
			if(128>ScrIndex&& ScrIndex>=0){I2C_Write(ScreenBuffer[ScrIndex]);} //0-127
			if(256>ScrIndex&& 128<=ScrIndex){I2C_Write(ScreenBuffer[ScrIndex-128]>>1);}//0-127
			if(384>ScrIndex&& 256<=ScrIndex){I2C_Write(ScreenBuffer[ScrIndex-128]);} //128-255
			if(512>ScrIndex&& 384<=ScrIndex){I2C_Write(ScreenBuffer[ScrIndex-256]>>1);}//128-255
			if(640>ScrIndex&& 512<=ScrIndex){I2C_Write(ScreenBuffer[ScrIndex-256]);}//256-383
			if(768>ScrIndex&&640<=ScrIndex){I2C_Write(ScreenBuffer[ScrIndex-384]>>1);}//256-383
			if(896>ScrIndex&& 768<=ScrIndex){I2C_Write(ScreenBuffer[ScrIndex-384]);}//384-511
			if(1024>ScrIndex&& 896<=ScrIndex){I2C_Write(ScreenBuffer[ScrIndex-512]>>1);}//384-511
			ScrIndex++;
		}
		I2C_Stop();
	}
}



void OLED_XY_pixel(uint8_t x, uint8_t y, uint8_t *ScreenBuffer, uint8_t PixleStatus){
	if( (y  <32) && (x <128)){
		
		uint8_t Page=0;
		uint8_t HalfPage=0;
		uint8_t Shift=0;

		if(y>=0 && y<4){Page=0; Shift=0;HalfPage=0;}else if(y>=4&&y<8){Page=0;Shift=1;HalfPage=1;}else if(y>=8&&y<12){Page=1;Shift=0;HalfPage=2;}
		else if(y>=12&&y<16){Page=1;Shift=1;HalfPage=3;}else if(y>=16&&y<20){Page=2;Shift=0;HalfPage=4;}else if(y>=20&&y<24){Page=2;Shift=1;HalfPage=5;}
		else if(y>=24&&y<28){Page=3;Shift=0;HalfPage=6;}else if(y>=28&&y<32){Page=3;Shift=1;HalfPage=7;}
		uint8_t temp=0;
		y-=(4*(HalfPage));
		temp=(y*64)/32;
		temp+=Shift;
		int INDEX =((128*Page)+x);

		
		if(PixleStatus==1||PixleStatus==2||PixleStatus==4||PixleStatus==8||PixleStatus==16||PixleStatus==32||PixleStatus==64||PixleStatus==128){
			if(temp==0){ScreenBuffer[INDEX]|=1;}else{
		ScreenBuffer[INDEX]|=(1<<temp);}

		}else if( PixleStatus==0){
		if(temp==0){ScreenBuffer[INDEX]&=~1;}else{
		ScreenBuffer[INDEX]&=~(1<<temp);
	}
}
//USART_Send("\r\n ScreenBuffer= ");
// uint8_t OUT =ScreenBuffer[((128*Page)+x)];
// USART_Int_StrBIT(OUT,0);
// USART_Send("\r\n Address= ");
// USART_Int_Str(INDEX,0);
// USART_Send("\r\n temp= ");
// USART_Int_Str(temp,0);
// USART_Send("\r\n y= ");
// USART_Int_Str(y,0);
// USART_Send("\r\n x= ");
// USART_Int_Str(x,0);
// USART_Send("\r\n Shift= ");
// USART_Int_Str(Shift,0);
// USART_Send("\r\n HalfPage= ");
// USART_Int_Str(HalfPage,0);
// USART_Send("\r\n Page= ");
// USART_Int_Str(Page,0);
// USART_Send("\r\n ShiftData= ");
// USART_Int_StrBIT(ScreenBuffer[((128*Page)+x)]>>1,0);
}else return;
}


void OLED_Byte_READ(uint8_t x, uint8_t y, uint8_t *ScreenBuffer){
	if( (y  <32) && (x <128)){
		
		uint8_t Page=0;
		uint8_t HalfPage=0;
		uint8_t Shift=0;

		if(y>=0 && y<4){Page=0; Shift=0;HalfPage=0;}else if(y>=4&&y<8){Page=0;Shift=1;HalfPage=1;}else if(y>=8&&y<12){Page=1;Shift=0;HalfPage=2;}
		else if(y>=12&&y<16){Page=1;Shift=1;HalfPage=3;}else if(y>=16&&y<20){Page=2;Shift=0;HalfPage=4;}else if(y>=20&&y<24){Page=2;Shift=1;HalfPage=5;}
		else if(y>=24&&y<28){Page=3;Shift=0;HalfPage=6;}else if(y>=28&&y<32){Page=3;Shift=1;HalfPage=7;}
		uint8_t temp=0;
		y-=(4*(HalfPage));
		temp=(y*64)/32;
		temp+=Shift;
		int INDEX =((128*Page)+x);

		
uint8_t OUT =ScreenBuffer[((128*Page)+x)];
USART_Send("\r\n ScreenBuffer= ");
USART_Int_StrBIT(OUT,0);
uint8_t dataRead=0;
USART_Send("\r\n Data = ");
if(OUT&0b1){dataRead+=1;}
if(OUT&0b10){dataRead+=16;}
if(OUT&0b100){dataRead+=2;}
if(OUT&0b1000){dataRead+=32;}
if(OUT&0b10000){dataRead+=4;}
if(OUT&0b100000){dataRead+=64;}
if(OUT&0b1000000){dataRead+=8;}
if(OUT&0b10000000){dataRead+=128;}
USART_Int_StrBIT(dataRead,0);
USART_Send("\r\n Address= ");
USART_Int_Str(INDEX,0);
// USART_Send("\r\n temp= ");
// USART_Int_Str(temp,0);
// USART_Send("\r\n y= ");
// USART_Int_Str(y,0);
// USART_Send("\r\n x= ");
// USART_Int_Str(x,0);
// USART_Send("\r\n Shift= ");
// USART_Int_Str(Shift,0);
// USART_Send("\r\n HalfPage= ");
// USART_Int_Str(HalfPage,0);
// USART_Send("\r\n Page= ");
// USART_Int_Str(Page,0);
// USART_Send("\r\n ShiftData= ");
// USART_Int_StrBIT(ScreenBuffer[((128*Page)+x)]>>1,0);
}else{return;}
}


void OLED_Byte( uint8_t x, uint8_t y,uint8_t *SB, uint8_t data){
	OLED_XY_pixel(x,y,SB,data&0b1);
	OLED_XY_pixel(x,y+1,SB,data&0b10);
	OLED_XY_pixel(x,y+2,SB,data&0b100);
	OLED_XY_pixel(x,y+3,SB,data&0b1000);
	OLED_XY_pixel(x,y+4,SB,data&0b10000);
	OLED_XY_pixel(x,y+5,SB,data&0b100000);
	OLED_XY_pixel(x,y+6,SB,data&0b1000000);
	OLED_XY_pixel(x,y+7,SB,data&0b10000000);
}

void OLED_VLine(uint8_t x, uint8_t y, uint8_t lenght, uint8_t*SB){
	for (int i=0; i<lenght && i<32;i++)
	{
		OLED_XY_pixel(x,y+i,SB,1);
	}
}

void OLED_HLine(uint8_t x, uint8_t y, uint8_t lenght, uint8_t*SB){
	for (int i=0; i<lenght && i<127;i++)
	{
		OLED_XY_pixel(x+i,y,SB,1);
	}
}
void OLED_Rect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t*SB){
	uint8_t length = x2 - x1 + 1;
	uint8_t height = y2 - y1;
	
	OLED_VLine(x1,y1,height,SB);
	OLED_VLine(x2,y1,height,SB);
	OLED_HLine(x1,y1,length,SB);
	OLED_HLine(x1,y2,length,SB);
}
void OLED_Bitmap(uint8_t*IA, uint8_t*SB){
int page=0;
int Xpos=0;
for (int i=0; i<512; i++)
{
if(i==128||i==256||i==384){page++;Xpos=0;}
OLED_Byte(Xpos,page,SB,IA[i]);
Xpos++;
}
}