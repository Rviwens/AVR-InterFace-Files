
#include "USART.h"

USART::USART(void){ RXSBWP = 0;}
	
USART::USART(int parity, int bitsize, long baud){
  RXSBWP = 0;	
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



void USART::TxChar(char ch){
	while(!(UCSRA & (1<<UDRE)));	/* Wait for empty transmit buffer*/
	UDR=ch;
}
 
void USART::Send(char str[]){
	for(int i=0;str[i]!='\0'; i++)
		TxChar(str[i]);

}
void USART::Send(const char str[]){
	for(int i=0;str[i]!='\0'; i++)
		TxChar(str[i]);

}

void USART::Send( uint8_t str[]){
	for(int i=0;str[i]!='\0'; i++)
	TxChar(str[i]);

}

	void USART::print_P(PGM_P p)
	{
		char c;
		while ((c = pgm_read_byte(p++)) != 0)
		{
			TxChar(c);      // or whatever your transmit function is called
		}
	}



void USART::Send_ESS(char *str,short ES){
	for(uint8_t i=0;i<strlen(str); i++)
		TxChar(str[i]);


	if (ES==1){
		TxChar(' ');
		TxChar('|');
		TxChar('E');
	}
}
void USART::Send_ESS(const char *str,short ES){
const char *out =str;
	Send_ESS((char*)out,ES);
}



void USART::Int_Str(int I,short ES){
	static char NUM_Hold[8];
	memset(NUM_Hold,0,8);
	itoa(I,NUM_Hold,10);
	Send_ESS(NUM_Hold,ES);
}
void USART::Int_Str(uint8_t I, short ES){
	Int_Str((int)I, ES);
}
void USART::Int_StrBIN(int I, short ES){
	static char NUM_Hold[8];
	memset(NUM_Hold,0,8);
	itoa(I,NUM_Hold,2);
	Send_ESS(NUM_Hold,ES);
}
void USART::Int_StrHEX(int I, short ES){
	static char NUM_Hold[8];
	memset(NUM_Hold,0,8);
	Send("0x");
	itoa(I,NUM_Hold,16);
	Send_ESS(NUM_Hold,ES);
}

void USART::Int_StrHEXRAW(int I, short ES){
	static char NUM_Hold[8];
	memset(NUM_Hold,0,8);
	itoa(I,NUM_Hold,16);
	if (I ==0 || I<=0xF) Send_ESS("0",0);
	Send_ESS(NUM_Hold,ES);
}

void USART::Long_Str(uint32_t I, short ES){
	static char NUM_Hold[16];
	memset(NUM_Hold,0,16);
	ltoa(I,NUM_Hold,10);
	Send_ESS(NUM_Hold,ES);
}
void USART::Long_StrHEX(uint32_t  I, short ES){
	static char NUM_Hold[11];
	memset(NUM_Hold,0,11);
	ltoa(I,NUM_Hold,16);
	Send_ESS(NUM_Hold,ES);
}
void USART::Long_StrBIN(uint32_t I , short ES){
	static char NUM_Hold[11];
	memset(NUM_Hold,0,8);
	ltoa(I,NUM_Hold,2);
	Send_ESS(NUM_Hold,ES);
}

void USART::Dtoa(double d, short ES){
	static char buff[11];
	String s1;
	s1.dtoa(d,buff);
	Send(buff);
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


USART::~USART()
{
	END();
}