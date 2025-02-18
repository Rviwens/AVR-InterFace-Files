#include "SSD1306_Driver.h"

SSD1306::SSD1306(void){}

SSD1306::SSD1306(uint8_t Invert, uint8_t contrast){
	Init(Invert,contrast);
}


void SSD1306::Invert(uint8_t Mode){
	if( Mode==1){
	i2c.Write(SSD1306_INVERTDISPLAY);}
	else if(Mode==0){
	i2c.Write(SSD1306_NORMALDISPLAY);}
}

void SSD1306::Contrast(uint8_t data){
	i2c.Write(SSD1306_SETCONTRAST);
	i2c.Write(data);
}


void SSD1306::Init(uint8_t invert, uint8_t contrast){
	DDRD|=0xF0;
	PORTD|=0xF0;
	_delay_ms(100);
	
	i2c.Start_Wait(SSD1306_DEFAULT_ADDRESS);
	i2c.Write(SSD1306_DISPLAYOFF);
	i2c.Write(SSD1306_SETDISPLAYCLOCKDIV);
	i2c.Write(0xF0);
	i2c.Write(SSD1306_SETMULTIPLEX);
	i2c.Write(0x3F);
	i2c.Write(SSD1306_SETDISPLAYOFFSET);
	i2c.Write(0x00);
	i2c.Write(SSD1306_SETSTARTLINE | 0x00);
	i2c.Write(SSD1306_CHARGEPUMP);
	i2c.Write(0x14);
	// Horizontal memory mode
	i2c.Write(SSD1306_MEMORYMODE);
	i2c.Write(0x00);
	i2c.Write(SSD1306_SEGREMAP | 0x1);
	i2c.Write(SSD1306_COMSCANDEC);
	i2c.Write(SSD1306_SETCOMPINS);
	i2c.Write(0x12);
	Contrast(contrast);
	i2c.Write(SSD1306_SETPRECHARGE);
	i2c.Write(0xF1);
	i2c.Write(SSD1306_SETVCOMDETECT);
	i2c.Write(0x40);
	// We use internal charge pump
	i2c.Write(SSD1306_CHARGEPUMP);
	i2c.Write(0x14);
	i2c.Write(SSD1306_DISPLAYALLON_RESUME);
	Invert(invert);
	// Turn display back on
	i2c.Write(SSD1306_DISPLAYON);
	//I2C_Stop();
	Clear();
}

void SSD1306::SetCursor(uint8_t myLineNumber,uint8_t myColumnPosition) {
	if( (myLineNumber <= 0x07) && (myColumnPosition <= 127) ) {
		lineNumber = myLineNumber;     // global var: current line number
		cursorPos =  myColumnPosition; // global var: current cursor position

		i2c.Write(SSD1306_COLUMNADDR);
		i2c.Write(myColumnPosition);
		i2c.Write(127);
		
		i2c.Write(SSD1306_PAGEADDR);
		i2c.Write(myLineNumber);
		i2c.Write(7);
		// I2C_Start_NORETURN(0x78);
		// I2C_Write(0x40);
	}
}


void SSD1306::Clear() {


	i2c.Start_NORETURN(0x78);
	SetCursor(0,0);

	// TWI NOT using Wire.h   128 bytes (8 bits vertical) per row * 8 char rows = 1024
	// Send 64 TWI messages { START:SLAw:0x40:16 bytes of 0x00:STOP }  1024 data bytes total
	for (uint16_t scrClearIndex=0; scrClearIndex<512; scrClearIndex++) {  // (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8)
		i2c.Start_NORETURN(0x78);
		i2c.Write(0x40);
		for (uint8_t x=0; x<16; x++) {
			i2c.Write(0x00);
			TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));
			scrClearIndex++;  // 16 bytes per I2C transmission__64 transmissions per screen
		}
		scrClearIndex--;
		i2c.Stop();
	}
}

void SSD1306::Fill() {
	i2c.Start_NORETURN(0x78);
	SetCursor(0,0);

	// TWI NOT using Wire.h   128 bytes (8 bits vertical) per row * 8 char rows = 1024
	// Send 64 TWI messages { START:SLAw:0x40:16 bytes of 0x00:STOP }  1024 data bytes total
	for (uint16_t scrClearIndex=0; scrClearIndex<512; scrClearIndex++) {  // (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8)
		i2c.Start_NORETURN(0x78);
		i2c.Write(0x40);
		for (uint8_t x=0; x<16; x++) {
			i2c.Write(0xFF);
			TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));
			scrClearIndex++;  // 16 bytes per I2C transmission__64 transmissions per screen
		}
		scrClearIndex--;
		i2c.Stop();
	}
}




void SSD1306::Display(uint8_t *ScreenBuffer){
	Clear();

	i2c.Start_NORETURN(0x78);
	SetCursor(0,0);
	i2c.Stop();
	for (uint16_t ScrIndex=0; ScrIndex<1024; ScrIndex+=0) {  // (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8)
		i2c.Start_NORETURN(0x78);
		i2c.Write(0x40);

		// for (int i=0; i<16; i++)
		// {
		// I2C_Write(ScreenBuffer[ScrIndex]);
		// ScrIndex++;
		// }

		for (int i=0; i<16; i++)
		{
			if(128>ScrIndex&& ScrIndex>=0){i2c.Write(ScreenBuffer[ScrIndex]);} //0-127
			if(256>ScrIndex&& 128<=ScrIndex){i2c.Write(ScreenBuffer[ScrIndex-128]>>1);}//0-127
			if(384>ScrIndex&& 256<=ScrIndex){i2c.Write(ScreenBuffer[ScrIndex-128]);} //128-255
			if(512>ScrIndex&& 384<=ScrIndex){i2c.Write(ScreenBuffer[ScrIndex-256]>>1);}//128-255
			if(640>ScrIndex&& 512<=ScrIndex){i2c.Write(ScreenBuffer[ScrIndex-256]);}//256-383
			if(768>ScrIndex&&640<=ScrIndex){i2c.Write(ScreenBuffer[ScrIndex-384]>>1);}//256-383
			if(896>ScrIndex&& 768<=ScrIndex){i2c.Write(ScreenBuffer[ScrIndex-384]);}//384-511
			if(1024>ScrIndex&& 896<=ScrIndex){i2c.Write(ScreenBuffer[ScrIndex-512]>>1);}//384-511
			ScrIndex++;
		}
		i2c.Stop();
	}
}



void SSD1306::XY_pixel(uint8_t x, uint8_t y, uint8_t *ScreenBuffer, uint8_t PixleStatus){
	if( (y  <32) && (x <128)){
		
		uint8_t Page=0;
		uint8_t HalfPage=0;
		uint8_t Shift=0;



		Page=(int)(y/8);
		HalfPage=(int)(y/4);
		if((y>=4&&y<8)||(y>=12&&y<16)||(y>=20&&y<24)||(y>=28&&y<32))
		Shift=1;
		
		
		Page=(int)(y/8);
		HalfPage=(int)(y/4);

		uint8_t temp=0;
		y-=(4*(HalfPage));
		temp=(y*2);
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

}else return;
}


void SSD1306::Byte_READ(uint8_t x, uint8_t y, uint8_t *ScreenBuffer){
	if( (y  <32) && (x <128)){
		
		uint8_t Page=0;
		uint8_t HalfPage=0;
		uint8_t Shift=0;
		
		Page=(int)(y/8);
		HalfPage=(int)(y/4);
		if((y>=4&&y<8)||(y>=12&&y<16)||(y>=20&&y<24)||(y>=28&&y<32))
		Shift=1;

		uint8_t temp=0;
		y-=(4*(HalfPage));
		temp=(y*64)/32;
		temp+=Shift;
		//int INDEX =((128*Page)+x);

		
		uint8_t OUT =ScreenBuffer[((128*Page)+x)];
		// USART_Send("\r\n ScreenBuffer= ");
		// USART_Int_StrBIT(OUT,0);
		uint8_t dataRead=0;
		/*USART_Send("\r\n Data = ");*/
		if(OUT&0b1){dataRead+=1;}
		if(OUT&0b10){dataRead+=16;}
		if(OUT&0b100){dataRead+=2;}
		if(OUT&0b1000){dataRead+=32;}
		if(OUT&0b10000){dataRead+=4;}
		if(OUT&0b100000){dataRead+=64;}
		if(OUT&0b1000000){dataRead+=8;}
		if(OUT&0b10000000){dataRead+=128;}

		}else{return;}
	}


	void SSD1306::Byte( uint8_t x, uint8_t y,uint8_t *SB, uint8_t data){
		XY_pixel(x,y,SB,data&0b1);
		XY_pixel(x,y+1,SB,data&0b10);
		XY_pixel(x,y+2,SB,data&0b100);
		XY_pixel(x,y+3,SB,data&0b1000);
		XY_pixel(x,y+4,SB,data&0b10000);
		XY_pixel(x,y+5,SB,data&0b100000);
		XY_pixel(x,y+6,SB,data&0b1000000);
		XY_pixel(x,y+7,SB,data&0b10000000);
	}

	void SSD1306::VLine(uint8_t x, uint8_t y, uint8_t lenght, uint8_t*SB){
		for (int i=0; i<lenght && i<32;i++)
		{
			XY_pixel(x,y+i,SB,1);
		}
	}

	void SSD1306::HLine(uint8_t x, uint8_t y, uint8_t lenght, uint8_t*SB){
		for (int i=0; i<lenght && i<127;i++)
		{
			XY_pixel(x+i,y,SB,1);
		}
	}
	void SSD1306::Rect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t*SB){
		uint8_t length = x2 - x1 + 1;
		uint8_t height = y2 - y1;
		
		VLine(x1,y1,height,SB);
		VLine(x2,y1,height,SB);
		HLine(x1,y1,length,SB);
		HLine(x1,y2,length,SB);
	}
	void SSD1306::Bitmap(uint8_t*IA, uint8_t*SB){
		int page=0;
		int Xpos=0;
		for (int i=0; i<512; i++)

		{

			if(i==128||i==256||i==384){page++;Xpos=0;}

			Byte(Xpos,page,SB,IA[i]);

			Xpos++;

		}
	}