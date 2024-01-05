 #define F_CPU 16000000UL	
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>
#define Buffer_Size 128
#define FequncyPin PINB1

void IR_Init(){
DDRB|=(1<<FequncyPin);
Timer2(8,0,OFF,0,1,0,130);//ORC May need to be adjusted, should work between (80-150)
}
ISR(TIMER2_COMPA_vect){PORTB^=(1<<FequncyPin);}

static long BAUD;
void USART_init(int parity, int bitsize, long baud)
{
BAUD=baud;
#define BDC ((F_CPU/16/BAUD)-1)
	UBRR0H = (BDC >> 8);
	UBRR0L = BDC;

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0); /* Turn on transmission and reception */
	UCSR0B|=(1<<RXCIE0);
	switch(parity){
		case 0: UCSR0C&=~(1<<UPM00); UCSR0C&=~(1<<UPM01);break;
		case 1:  UCSR0C|=(1<<UPM00)|(1<<UPM01);break;
		case 2:	 UCSR0C|=(1<<UPM01); break;
	}
	switch(bitsize){
		case 5: UCSR0C&=~(1<<UCSZ00); UCSR0C&=~(1<<UCSZ01); UCSR0C&=~(1<<UCSZ02);break;
		case 6:  UCSR0C|=(1<<UCSZ00);break;
		case 7:	 UCSR0C|=(1<<UCSZ01); break;
		case 8: UCSR0C|=(1<<UCSZ01);UCSR0C|=(1<<UCSZ00); break;
		case 9:  UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01)|(1<<UCSZ02);break;
	}

}



void USART_TxChar(char ch){
	while(!(UCSR0A & (1<<UDRE0)));	/* Wait for empty transmit buffer*/
	UDR0=ch;
}


void T(){if(BAUD<=9000){_delay_ms(1);}if(BAUD<=5000){_delay_ms(9);}if(BAUD<=1200){_delay_ms(50);}}

void USART_Send(char *str,int ES){
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


void USART_Int_Str(int I,int ES){
	char out[10000];
	itoa(I,out,10);
	USART_Send(out,ES);
}

unsigned char RxSerialBuffer[Buffer_Size];
int RXSBWP;
unsigned char RXSB[Buffer_Size];

	void CheckRx(){
		if(RxSerialBuffer[RXSBWP-1]=='|' &&RxSerialBuffer[RXSBWP]=='E'){
			memset(RXSB,0,Buffer_Size);
			for(int i=1; i<=RXSBWP-3; i++){RXSB[i-1]=RxSerialBuffer[i];}
			RXSBWP=0;
			memset(RxSerialBuffer,0,Buffer_Size);
		}
	}




ISR(USART_RX_vect){
	
	RXSBWP++;
	RxSerialBuffer[RXSBWP]=UDR0;
	CheckRx();
	if(RXSBWP>=100){RXSBWP=0;}
	if(RXSBWP>=10){PORTD^=(1<<PIND4);}}
	

	
	
	
	
	
	
	void reverse(char* str)
	{
		int len=strlen(str);
		int i = 0, j = len - 1, temp;
		while (i < j) {
			temp = str[i];
			str[i] = str[j];
			str[j] = temp;
			i++;
			j--;
		}
	}
	int upperNum(double op){int Out=op;return Out;}

	// finds the number below the decimal point. Counts how many zeros are between decimal point and number. Returns Number as integer.
	long lowerNum( long double IN,int BACK){
		int COUNT2=0;
		COUNT2-=1;
		int Zero=0;
		IN-=upperNum(IN);
		long long R=IN*pow(10,5);
		for(int POW=0; POW<5;IN*=10,POW++){Zero=IN; if(Zero==0){COUNT2++;}}
		IN++;
		if(R!=IN){IN+=1;}
		if(BACK==1){return COUNT2;}
		if(BACK==0){return R;}
			return 0; 
	}

	// Clear Zero's  Ex--(900->9)
	void CLZer(long Num,char*str){ltoa(Num,str,10);reverse(str);long Nh=atol(str);ltoa(Nh,str,10);reverse(str);}
	
	/*finds the number above the decimal  point on  turns into a string*/

	
	/*Take in the double and move it around to the proper functions.*/
	// combines the strings with the right number of zeros and the decimals

	void AddDec(char*str1, char*str2, int ZeroNum){
		char DEC[]={"."};
		strcat(str1,DEC);
		char Zero[ZeroNum-1];
		int i=0;
		while(ZeroNum>i){Zero[i]='0';i++;}
		strcat(str1,Zero);
		strcat(str1,str2);
	}

	void USART_Dtoa(double Num,int ES){
		char str3[10];
		char Holder1[10];
		char Holder2[6];
		long Up=lowerNum(Num,0);
		CLZer(Up,Holder2);
		itoa(upperNum(Num),Holder1,10);
		AddDec(Holder1,Holder2,lowerNum(Num,1));
		strcpy(str3,Holder1);
		USART_Send(str3,ES);
}
	void dtoa(double Num,char*str3){
		char Holder1[10];
		char Holder2[6];
		long Up=lowerNum(Num,0);
		CLZer(Up,Holder2);
		itoa(upperNum(Num),Holder1,10);
		AddDec(Holder1,Holder2,lowerNum(Num,1));
		strcpy(str3,Holder1);
	}
	

	
	void USART_END(){
		_delay_ms(10);
		UCSR0B=0x00;
		UBRR0L=0x00;
		UBRR0H=0x00;
	}

	void USART_SSE(int baudRate, char data,short Ebit){
		USART_init(0,8,baudRate);
		USART_Send(data,Ebit);
		USART_END();
	}
	
	
	void USART_SSIE(int baudRate, int data,short Ebit){
		USART_init(0,8,baudRate);
		USART_Int_Str(data,Ebit);
		USART_END();
	}