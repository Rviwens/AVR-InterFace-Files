#define F_CPU 20000000UL	// Define CPU frequency here 20MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>




int button(int pinport,int pin){
	int Vaule=0;
	if(pinport&(1<<pin)){Vaule=1;}else{Vaule=0;}
	return Vaule;
}

