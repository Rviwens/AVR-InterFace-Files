
#include "USART.h"

USART::USART(void){}
USART::USART(int parity, int bitsize, long baud){
Init(parity,bitsize,baud);	
}	

	
void USART::IR_Init(){
	DDRD|=(1<<FequncyPin);
	TCCR2 |=(1<<WGM20)|(1<<WGM21);
	TCCR2|=(0<<CS22)|(1<<CS21)|(0<<CS20);
	OCR2=80;
}

#ifdef IR_USART
ISR(TIMER2_COMP_vect){PORTD^=(1<<FequncyPin); }
#endif

void USART::Init(int parity, int bitsize, long baud)
{
	this->BAUD=baud;
	
	#define BDC ((F_CPU/16/BAUD)-1)
	
	UCSRB |= (1 << RXEN) | (1 << TXEN)|(1<<RXCIE);/* Turn on transmission and reception */
	switch(parity){
		case 0: UCSRC&=~(1<<UPM0); UCSRC&=~(1<<UPM1);break;
		case 1:  UCSRC|=(1<<UPM0)|(1<<UPM1);break;
		case 2:	 UCSRC|=(1<<UPM1); break;
	}
	switch(bitsize){
		case 5: UCSRC&=~(1<<UCSZ0); UCSRC&=~(1<<UCSZ1); UCSRC&=~(1<<UCSZ2);break;
		case 6:  UCSRC|=(1<<UCSZ0);break;
		case 7:	 UCSRC|=(1<<UCSZ1); break;
		case 8: UCSRC|=(1<<UCSZ1);UCSRC|=(1<<UCSZ0); break;
		case 9:  UCSRC|=(1<<UCSZ0)|(1<<UCSZ1)|(1<<UCSZ2);break;
	}

	UBRRL = BDC;		/* Load lower 8-bits of the baud rate value */
	UBRRH = (BDC >> 8);	/* Load upper 8-bits*/
}

void USART::T(){if(BAUD<=9000){_delay_ms(1);}if(BAUD<=5000){_delay_ms(9);}if(BAUD<=1200){_delay_ms(50);}}


void USART::TxChar(char ch){
	while(!(UCSRA & (1<<UDRE)));	/* Wait for empty transmit buffer*/
	UDR=ch;
}

void USART::Send(char*str){
	for(int i=0;str[i]!='\0'; i++){
		TxChar(str[i]);
		T();
	}
}

void USART::Send_ESS(char *str,int ES){
	for(uint8_t i=0;i<strlen(str); i++){
		TxChar(str[i]);
		T();
	}
	if (ES==1){
		TxChar(' ');T();
		TxChar('|');T();
		TxChar('E');T();
	}
}



void USART::Int_Str(int I, int ES){
	itoa(I,NUM_Hold,10);
	Send_ESS(NUM_Hold,ES);
}
void USART::Int_StrBIN(int I, int ES){
	itoa(I,NUM_Hold,2);
	Send_ESS(NUM_Hold,ES);
}
void USART::Int_StrHEX(int I, int ES){
	Send("0x");
	itoa(I,NUM_Hold,16);
	Send_ESS(NUM_Hold,ES);
}

void USART::Int_StrHEXRAW(int I, int ES){
	itoa(I,NUM_Hold,16);
	if (I ==0 || I<=0xF) Send_ESS("0",0);
	Send_ESS(NUM_Hold,ES);

}

void USART::Long_Str(long I, int ES){
	ltoa(I,NUM_Hold,10);
	Send_ESS(NUM_Hold,ES);
}
void USART::Long_StrHEX(long I, int ES){
	ltoa(I,NUM_Hold,16);
	Send_ESS(NUM_Hold,ES);
}
void USART::Long_StrBIN(long I, int ES){
	ltoa(I,NUM_Hold,2);
	Send_ESS(NUM_Hold,ES);
}




void USART::CheckRx(){
	if(RxSerialBuffer[RXSBWP-1]=='|' &&RxSerialBuffer[RXSBWP]=='E'){
		memset(RXSB,0,Buffer_Size);
		for(int i=1; i<=RXSBWP-3; i++){RXSB[i-1]=RxSerialBuffer[i];}
		PORTD^=(1<<PIND3);
		RXSBWP=0;
		memset(RxSerialBuffer,0,Buffer_Size);
	}}

#ifdef USART_RX
	ISR(USART_RXC_vect){
		RXSBWP++;
		RxSerialBuffer[RXSBWP]=UDR;
		CheckRx();
	if(RXSBWP>=Buffer_Size){RXSBWP=0;}}
#endif

		void USART::END(){
			_delay_ms(10);
			UCSRB=0x00;
			UBRRL=0x00;
			UBRRH=0x00;
		}

