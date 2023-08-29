#define F_CPU 20000000UL	// Define CPU frequency here 20MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#define input "input"
#define All "all"
#define OutPut "output"
#define High "High"
#define Low "Low"
#define B "B"
#define A "A"
#define In "in"
#define D "D"
#define C "C"
#define TOG "TOG"
//Write
 static int  Read;
 
void Port (int port, int State, int Num){
int Pin=Num-1;
if( State == High){
if(port==A){PORTA |=(1<<Pin);}
if(port==B){PORTB |=(1<<Pin);}
if(port==C){PORTC |=(1<<Pin);}
if(port==D){PORTD |=(1<<Pin);}
}
if(State ==Low){
if(port==A){PORTA  &=~(1<<Pin);}
if(port==B){PORTB &=~(1<<Pin);}
if(port==C){PORTC &=~(1<<Pin);}
if(port==D){PORTD &=~(1<<Pin);}
}
if(Num ==All&&State==High){
if(port==A){PORTA=0xFF;}
if(port==B){PORTB=0xFF;}
if(port==C){PORTC=0xFF;}
if(port==D){PORTD=0xFF;}
}
if(Num ==All&&State==Low){
if(port==A){PORTA=0x0;}
if(port==B){PORTB=0x0;}
if(port==C){PORTC=0x0;}
if(port==D){PORTD=0x0;}
}
if(State == TOG){
if(port==A){PORTA ^=(1<<Pin);}
if(port==B){PORTB ^=(1<<Pin);}
if(port==C){PORTC ^=(1<<Pin);}
if(port==D){PORTD ^=(1<<Pin);}
}}


//
void DDR(int port, char State, int Pin_Num){
int Pin =Pin_Num-1;
if(State ==input){
if(port==A){DDRA  &=~(1<<Pin);}
if(port==B){DDRB &=~(1<<Pin);}
if(port==C){DDRC &=(1<<Pin);}
if(port==D){DDRD &=~(1<<Pin);}
}
if( State ==OutPut){
if(port==A){DDRA |=(1<<Pin);}
if(port==B){DDRB |=(1<<Pin);}
if(port==C){DDRC |=(1<<Pin);}
if(port==D){DDRD |=(1<<Pin);}				
}
if(Pin_Num ==All&&State==OutPut){
if(port==A){DDRA=0xFF;}
if(port==B){DDRB=0xFF;}
if(port==C){DDRC=0xFF;}
if(port==D){DDRD=0xFF;}
}
if(Pin_Num ==All&&State==input){
if(port==A){DDRA=0x00;}
if(port==B){DDRB=0x00;}
if(port==C){DDRC=0x00;}
if(port==D){DDRD=0x00;}
}
if( State ==TOG){
if(port==A){DDRA ^=(1<<Pin);}
if(port==B){DDRB ^=(1<<Pin);}
if(port==C){DDRC ^=(1<<Pin);}
if(port==D){DDRD ^=(1<<Pin);}
}
}
			
			




		
	//End of Fast PWM









// uint32_t MAP(uint32_t IN, uint32_t INmin, uint32_t INmax, uint32_t OUTmin, uint32_t OUTmax)
// {
// 	return ((((IN - INmin)*(OUTmax - OUTmin))/(INmax - INmin)) + OUTmin);
// }

// double MAP(double IN, double INmin, double INmax, double OUTmin, double OUTmax)
// {
// 	return ((((IN - INmin)*(OUTmax - OUTmin))/(INmax - INmin)) + OUTmin);
// }