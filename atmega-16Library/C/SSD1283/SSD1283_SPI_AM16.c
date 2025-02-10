
#include "SSD1283_SPI_AM16.h"
    
void SSD1283_Select(void){
	CS_SSD1283_EN;
}

void SSD1283_Deselect(void){
		CS_SSD1283_DE;
}
void SSD1283_Command(uint8_t cmd){
	PORTB &= ~(1<<RSpin);
	SPI_transfer(cmd&0xFF);
}
        
void SSD1283_Data(uint16_t data){
	PORTB |= (1<<RSpin);
	SPI_transfer(data>>8);
	SPI_transfer(data&0xFF);
} 

void SSD1283_Cmd_Data(uint8_t cmd,uint16_t data){
	PORTB &= ~(1<<RSpin);
	SPI_transfer(cmd&0xFF);
	PORTB |= (1<<RSpin);
	SPI_transfer(data>>8);
	SPI_transfer(data&0xFF);
}

void SSD1283_OSC_Start(void){
SSD1283_Cmd_Data(0x00,1);
}

void SSD1283_Driver_Output_Control(bool rev,bool CAD, bool BGR, bool SM, bool TB, bool RL){
SSD1283_Cmd_Data(0x01, (rev<<13)+(CAD<<12)+(BGR<<11)+(SM<<10)+(TB<<9)+(RL<<8)+SSD1283_R00H_MUX);
}

void SSD1283_Waveform_Control(bool FLD,bool BC, bool EOR,,uint8_t nlines){
SSD1283_Cmd_Data(0x02,(FLD<<11)+(BC<<9)+(EOR<<8)+nlines);
}

void SSD1283_Entry_Mode(bool VSMode,bool DFM0, bool TRANS, bool OEDef, bool Dmode1,bool ID1, bool ID0, bool AM){
SSD1283_Cmd_Data(0x02,(VSMode<<15)+(1<<14)+(DFM0<<13)+(TRANS<<12)+(OEDef<<11)+(Dmode1<<9)+(ID1<<5)+(ID0<<4)+(AM<<3));
}

void SSD1283_Frame_Cycle_Control(bool EQ1,bool EQ0,bool SDIV, bool SRTN ){
SSD1283_Cmd_Data(0x02,((EQ1<<11)+(EQ0<<10)+(SSD1283_R0BH_DIV<<8)+(SDIV<<5)+(SRTN<<4)+SSD1283_R0BH_RTN);
}






void SSD1283_setWindowAddress(int16_t x1, int16_t y1, int16_t x2, int16_t y2,uint8_t rot)
{
	switch (rot)
	{
		case 0:
		SSD1283_Command(0x44);
		SSD1283_Data(x2 + 2);
		SSD1283_Data(x1 + 2);
		SSD1283_Command(0x45);
		SSD1283_Data(y2 + 2);
		SSD1283_Data(y1 + 2);
		SSD1283_Command(0x21);
		SSD1283_Data(y1 + 2);
		SSD1283_Data(x1 + 2);
		break;
		case 1:
		SSD1283_Command(0x44); //Horizontal RAM address position 
		SSD1283_Data(SSD1283_HEIGHT - y1 + 1);
		SSD1283_Data(SSD1283_HEIGHT - y2 + 1);
		SSD1283_Command(0x45); //Vertical RAM address position 
		SSD1283_Data(SSD1283_WIDTH - x1 - 1);
		SSD1283_Data(SSD1283_WIDTH - x2 - 1);
		SSD1283_Command(0x21); //RAM address set
		SSD1283_Data(SSD1283_WIDTH - x1 - 1);
		SSD1283_Data(SSD1283_HEIGHT - y1 + 1);
		break;
		case 2:
		SSD1283_Command(0x44);
		SSD1283_Data(SSD1283_WIDTH - x1 + 1);
		SSD1283_Data(SSD1283_WIDTH - x2 + 1);
		SSD1283_Command(0x45);
		SSD1283_Data(SSD1283_HEIGHT - y1 + 1);
		SSD1283_Data(SSD1283_HEIGHT - y2 + 1);
		SSD1283_Command(0x21);
		SSD1283_Data(SSD1283_HEIGHT - y1 + 1);
		SSD1283_Data(SSD1283_WIDTH - x1 + 1);
		break;
		case 3:
		SSD1283_Command(0x44);
		SSD1283_Data(y2 + 2);
		SSD1283_Data(y1 + 2);
		SSD1283_Command(0x45);
		SSD1283_Data(x2);
		SSD1283_Data(x1);
		SSD1283_Command(0x21);
		SSD1283_Data(x1);
		SSD1283_Data(y1 + 2);
		break;
	}
}