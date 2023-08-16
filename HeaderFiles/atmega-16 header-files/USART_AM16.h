
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#define Buffer_Size 128
#define FequncyPin PIND7





void IR_Init(){
	DDRD|=(1<<FequncyPin);
	Timer2(3,8,OFF);
	Timer2_Compare(80);
}
ISR(TIMER2_COMP_vect){PORTD^=(1<<FequncyPin); }

static long BAUD;
void USART_init(int parity, int bitsize, long baud)
{
 BAUD=baud;
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

void T(){if(BAUD<=9000){_delay_ms(1);}if(BAUD<=5000){_delay_ms(9);}if(BAUD<=1200){_delay_ms(50);}}


void USART_TxChar(char ch){
while(!(UCSRA & (1<<UDRE)));	/* Wait for empty transmit buffer*/
UDR=ch;
}

void USART_Send(char str[]){	
for(int i=0;str[i]!='\0'; i++){
USART_TxChar(str[i]);	
T();
}
}

void USART_Send_ESS(char str[],int ES){
	for(int i=0;i<strlen(str); i++){
		USART_TxChar(str[i]);
		T();
	}
	if (ES==1){
		USART_TxChar(' ');T();
		USART_TxChar('|');T();
		USART_TxChar('E');T();
	}
}


char NUM_Hold[10];
void USART_Int_Str(int I, int ES){
itoa(I,NUM_Hold,10);
USART_Send_ESS(NUM_Hold,ES);
}
void USART_Int_StrBIT(int I, int ES){
	itoa(I,NUM_Hold,2);
	USART_Send_ESS(NUM_Hold,ES);
}
void USART_Int_StrHEX(int I, int ES){
	USART_Send("0x");
	itoa(I,NUM_Hold,16);
	USART_Send_ESS(NUM_Hold,ES);
	
}
void USART_Long_Str(long I, int ES){
	ltoa(I,NUM_Hold,10);
	USART_Send_ESS(NUM_Hold,ES);
}

unsigned char RxSerialBuffer[Buffer_Size];
int RXSBWP; 
unsigned char RXSB[Buffer_Size];

void CheckRx(){
	if(RxSerialBuffer[RXSBWP-1]=='|' &&RxSerialBuffer[RXSBWP]=='E'){
		memset(RXSB,0,Buffer_Size);
		for(int i=1; i<=RXSBWP-3; i++){RXSB[i-1]=RxSerialBuffer[i];}
		PORTD^=(1<<PIND3);
		RXSBWP=0;
		memset(RxSerialBuffer,0,Buffer_Size);
	}}

ISR(USART_RXC_vect){ 	
RXSBWP++;
RxSerialBuffer[RXSBWP]=UDR; 
CheckRx();
if(RXSBWP>=Buffer_Size){RXSBWP=0;}}



void USART_END(){
_delay_ms(10);
UCSRB=0x00;
UBRRL=0x00;
UBRRH=0x00;
}

void USART_SSE(int baudRate, char *data,short Ebit){
USART_init(0,8,baudRate);
//USART_Send_ESS(data,Ebit);
USART_END();	
USART_Send_ESS(data,Ebit);
}

void USART_SSIE(int baudRate, int data,short Ebit){
	USART_init(0,8,baudRate);
	USART_Int_Str(data,Ebit);
	USART_END();
}