
#include "SPI_AM16.h"

#if defined(SPI_M)

	void SPI_MAST_Init(int dataOrder,int Prescaler,int clockPolarity, int Cphase, int DoubleTime){
		Device_Mode=0;
		DDRB|=(1<<PB5)|(1<<PB4)|(1<<PB7)|(1<<PB6);
		SPSR=(DoubleTime<<SPI2X);
		SPCR=0x00;
		SPCR|=(dataOrder<<DORD);
		SPCR|=(1<<SPE)|(1<<SPIE)|(clockPolarity<<CPOL)|(Cphase<<CPHA)|(1<<MSTR);

		switch(Prescaler){
			case 4: SPCR&=~(1<<SPR1);SPCR&=~(1<<SPR0);
			break;
			case 16:SPCR&=~(1<<SPR1);SPCR|=(1<<SPR0);
			break;
			case 64:SPCR|=(1<<SPR1);SPCR&=~(1<<SPR0);
			break;
			case 128:SPCR|=(1<<SPR1);SPCR|=(1<<SPR0);
			break;
		}

}

uint8_t SPI_transfer(uint8_t data)
{
	// load data into register
	SPDR = data;
	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));
	// return SPDR
	return SPDR;
}
#endif







#if defined(SPI_S)
	 void SPI_SLAVE_Init()
	 {
		Device_Mode=1;
		DDRB &= ~(1<<PB5);  /* Make MOSI, SCK, SS as input pins */
		DDRB &= ~(1<<PB7);
		DDRB &= ~(1<<PB4);
		DDRB |= (1<<PB6);			/* Make MISO pin as 
							output pin */
		SPCR = (1<<SPE);			/* Enable SPI in slave mode */
	}
	char SPI_SR()			/* SPI Receive data function */
	{
		while(!(SPSR & (1<<SPIF)));	/* Wait till reception complete */
		return(SPDR);			/* Return received data */
	}
#endif











// void SPI_Str(char *str){
// //SPI_Write(" ");
// for (int i=0; str[i]!=0; i++){
// _delay_ms(1);
// SPI_Write(str[i]);
// //USART_TxChar(str[i]);
// }
// 
// SPI_Write('|');	
// }


// void SPI_Receive(char *buffer){
// 	memset(buffer,0,strlen(buffer));
// 	for (int i=0; buffer[i-1]!='|';i++)
// 	{
// 		buffer[i]=SPI_SR();
// 		//USART_TxChar(buffer[i]);
// 	}// }

// void SPI_Read(char *buffer){
// 	//PORTB&=~(1<<SS);
// 	memset(buffer,0,strlen(buffer));
// int count =0;	
// do{
// buffer[count]=SPI_MR();
// //USART_TxChar(buffer[count])		
// _delay_ms(50);
// count++;
// } while(buffer[count-1]!='|');
// }





// void SPI_USART_MREAD(){
// char temp[100]; 
//	USART_Send("\r\n Receiving Echo: ");
// 	 SPI_Read(temp);
// // USART_Send(temp);
// }
// 
// void SPI_USART_SREAD(){
// char temp[100];
// USART_Send("\r\n Receiving Echo: ");
/*	SPI_Receive(temp);*/
//USART_Send(temp);
// }
// 



