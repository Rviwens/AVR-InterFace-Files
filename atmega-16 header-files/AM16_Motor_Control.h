#define F_CPU 20000000UL	// Define CPU frequency here 20MHZ

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>


static int P;
void Servo_write(int port, int pin, int angle){
int P=port;
int POS=MAP(angle,0,180,125,755);
if(TCNT1<POS+20&&TCNT1>POS-20){
if(TCNT1>=POS&&port&port){
if(P&PORTA){PORTA&=~(1<<pin);}
if(P&PORTB){PORTB&=~(1<<pin);}
if(P&PORTC){PORTD&=~(1<<pin);}
if(P&PORTD){PORTC&=~(1<<pin);}}}  
}

void Servo_INIT(void){
DDRA=0xFF;
TIMSK|=(1<<OCIE1A);
TCNT1= 0;
TCCR1A=0b00000010;
TCCR1B=0b00011011;
ICR1=6249;
OCR0=0;
}


void Step(int in, int step ){if(step==1){
switch (in){
case 1: P = 0x09; break;
case 2: P = 0x0C;break;
case 3: P = 0x06;break;
case 4: P = 0x03;break;
}}
if(step==2){
switch(in){
case 1: P = 0x09;break;
case 2: P = 0x08;break;
case 3: P = 0x0C;break;
case 4: P = 0x04;break;
case 5: P = 0x06;break;
case 6: P = 0x02;break;
case 7: P = 0x03;break;
case 8: P = 0x01;break;
}}
PORTA=P;}