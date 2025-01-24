
#include "SSD1283_SPI_AM16.h"

void SSD1283_LCD_Init(){


}          

void SSD1283_Command(uint8_t Reg, uint16_t CMD){
	CS_SSD1283_EN;
	PORTB &= ~(1<<RSpin);

	CS_SSD1283_DE;
}
        
void SSD1283_Data(uint8_t data){
	CS_SSD1283_EN;
	PORTB |= (1<<RSpin);
	
	CS_SSD1283_DE;
} 
