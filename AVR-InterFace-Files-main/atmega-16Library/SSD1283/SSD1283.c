
#define F_CPU 20000000UL// Define CPU frequency here 20MHZ


#define SSD1283_HEIGHT 132
#define SSD1306_BUFFERSIZE SSD1283_HEIGHT*SSD1283_HEIGHT*(18/8)
#define SSD1283_DEFAULT_ADDRESS 0x1283

#define CS_DE PORTB|=(0b10);
#define CS_EN PORTB&=~(0b10);
#define RSpin PB2



//Prototypes
void SSD1283_LCD_Init();
void SSD1283_Command(uint8_t Reg, uint16_t CMD);
void SSD1283_Data(uint8_t data);





void SSD1283_LCD_Init(){
	DDRB |= (1<<PB0)|(1>>PB1);
	//SSD1283_Command();

}          

void SSD1283_Command(uint8_t Reg, uint16_t CMD){
	CS_EN;
	PORTB &= ~(1<<RSpin);
	SPI_Write(Reg);
	SPI_Write(CMD>>8);
	SPI_Write(CMD);
	CS_DE;
}
        
void SSD1283_Data(uint8_t data){
	CS_EN;
	SPI_Write(data);
} 
