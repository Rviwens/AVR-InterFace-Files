
#ifndef TimerFunc
#define TimerFunc

#define F_CPU 20000000UL// Define CPU frequency here 20MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>


#define Rising  11
#define Falling 12
#define TOG 3
#define OFF 0
#define SET 1
#define INV 2
#define Timer




// 	Atmega 16 Timer Registers
// TCCR0(|FOC0|WGM00|COM01|COM00|WGM01|CS02|CS01|CS00|)
//TIMSK(OCIE2|TOIE2|TICIE1|OCIE1A|OCIE1B|TOIE1|OCIE0|TOIE0|)
//TCCR1A(COM1A1|COM1A0| COM1B1|COM1B0 FOC1A|FOC1B|WGM11|WGM10|)
//TCCR1B(|ICNC1|ICES1|–|WGM13|WGM12|CS12|CS11|CS10|)



void Timer0(int Mode, int Prescaler, int Out){
if(Mode==0){TCCR0&=~(1<<WGM00);TCCR0&=~(1<<WGM01);} // normal
if(Mode==1){TCCR0|=(1<<WGM00)|(0<<WGM01);}  //phase
if(Mode==2){ TCCR0 |=(1<<WGM00)|(1<<WGM01);}  // fast
if (Mode==3){ TIMSK |= (1<<OCIE0);TCCR0|=(1<<WGM01);} //CTC
//
switch(Out){
case OFF:TCCR0 |=(0<<COM01)|(0<<COM00);break;
case TOG:TCCR0 |=(0<<COM01)|(1<<COM00);break;
case SET:TCCR0 |=(1<<COM01)|(0<<COM00);break;
case INV:TCCR0 |=(1<<COM01)|(1<<COM00);break;
}
switch(Prescaler){
case 0:TCCR0|=(0<<CS02)|(0<<CS01)|(1<<CS00);break;
case 8:TCCR0|=(0<<CS02)|(1<<CS01)|(0<<CS00);break;
case 64:TCCR0|=(0<<CS02)|(1<<CS01)|(1<<CS00);break;
case 256:TCCR0|=(1<<CS02)|(0<<CS01)|(0<<CS00);break;
case 1024:TCCR0|=(1<<CS02)|(0<<CS01)|(1<<CS00);break;
case Falling:TCCR0|=(1<<CS02)|(1<<CS01)|(0<<CS00);break;
case Rising:TCCR0|=(1<<CS02)|(1<<CS01)|(1<<CS00);break;
}
}
void Timer0_Compare(int Ticks){OCR0=Ticks;}








void  PWM0(double Vin, double VOUT,short CurrentD,short INPUT,short reg){
/*if(Mode==Fast){*/
//Math
Vin+=0.25;
char duty=0;
if(INPUT==0){
if(CurrentD==1)
{
double Vout = Vin-VOUT; //Find Out Voltage;
duty=100/(Vin/Vout);  //Fine the On percentage
}
else if(CurrentD==0)
{
duty=100/(Vin/VOUT); //Fine the On percentage
}
duty=(duty*255)/100;    //Find the Porpoteinal vaule of DUTY/100 & OCR0V/255;
OCR0=duty;
} else if(INPUT == 1){OCR0=VOUT;
}
}



void Timer2(int Mode, int Prescaler, int Out){
	if(Mode==1){TCCR2|=(1<<WGM20)|(0<<WGM21);}//Phase
	if (Mode==3){TIMSK = (1<<OCIE2);TCCR2=(1<<WGM21);} // CTC
	if(Mode==2){ TCCR2 |=(1<<WGM20)|(1<<WGM21);} //Fast
	switch(Out){
		case OFF:TCCR2|=(0<<COM21)|(0<<COM20);break;
		case TOG:TCCR2|=(0<<COM21)|(1<<COM20);break;
		case SET:TCCR2|=(1<<COM21)|(0<<COM20);break;
		case INV:TCCR2|=(1<<COM21)|(1<<COM20);break;
	}
	switch(Prescaler){
		case 0:TCCR2|=(0<<CS22)|(0<<CS21)|(1<<CS20);break;
		case 8:TCCR2|=(0<<CS22)|(1<<CS21)|(0<<CS20);break;
		case 64:TCCR2|=(0<<CS22)|(1<<CS21)|(1<<CS20);break;
		case 256:TCCR2|=(1<<CS22)|(0<<CS21)|(0<<CS20);break;
		case 1024:TCCR2|=(1<<CS22)|(0<<CS21)|(1<<CS20);break;
		case Falling:TCCR2|=(1<<CS22)|(1<<CS21)|(0<<CS20);break;
		case Rising:TCCR2|=(1<<CS22)|(1<<CS21)|(1<<CS20);break;
	}
}

void Timer2_Compare(int Ticks){OCR2=Ticks;}




void Timer1( int Prescaler,int Register, int Out, int wgm13, int wgm12, int wgm11, int wgm10){
	TCCR1A |=(wgm10<<WGM10)|(wgm11<<WGM11);
	TCCR1B |=(wgm12<<WGM12)|(wgm13<<WGM13);

	if (wgm13==0 && wgm12==0 && wgm11==0 && wgm10 ==0)
	{
		TIMSK = (1 << TOIE1);
	}

	if(Register==0){ //Register A
		switch(Out){
			case OFF:TCCR1A |=(0<<COM1A1)|(0<<COM1A0);break;
			case TOG:TCCR1A |=(0<<COM1A1)|(1<<COM1A0);break;
			case SET:TCCR1A |=(1<<COM1A1)|(0<<COM1A0);break;
			case INV:TCCR1A |=(1<<COM1A1)|(1<<COM1A0);break;
		}
	}
	if(Register==1){ //Register B
		switch(Out){
			case OFF:TCCR1A |=(0<<COM1B1)|(0<<COM1B0);break;
			case TOG:TCCR1A |=(0<<COM1B1)|(1<<COM1B0);break;
			case SET:TCCR1A |=(1<<COM1B1)|(0<<COM1B0);break;
			case INV:TCCR1A |=(1<<COM1B1)|(1<<COM1B0);break;
		}
	}
	switch(Prescaler){
		case 0:TCCR1B|=(0<<CS12)|(0<<CS11)|(1<<CS10);break;
		case 8:TCCR1B|=(0<<CS12)|(1<<CS11)|(0<<CS10);break;
		case 64:TCCR1B|=(0<<CS12)|(1<<CS11)|(1<<CS10);break;
		case 256:TCCR1B|=(1<<CS12)|(0<<CS11)|(0<<CS10);break;
		case 1024:TCCR1B|=(1<<CS12)|(0<<CS11)|(1<<CS10);break;
		case Falling:TCCR1B|=(1<<CS12)|(1<<CS11)|(0<<CS10);break;
		case Rising:TCCR1B|=(1<<CS12)|(1<<CS11)|(1<<CS10);break;
	}
}

void Timer1_Compare(int timer_ticks,int prescaler, int Vaule,int compare){
	OCR1A = timer_ticks;
	OCR1B = timer_ticks;
	TCCR1B=(1<<WGM12);
	if(compare ==1){TIMSK|=(1<<OCIE1A);}// Timer A
	if(compare ==2){TIMSK|=(1<<OCIE1B);} //Timer B
	TCNT1= Vaule;
	sei();
	switch(prescaler){
		case 0:TCCR1B=0b00001001;break;
		case 8:TCCR1B=0b00001010;break;
		case 64:TCCR1B=0b00001011;break;
		case 256:TCCR1B=0b00001100;break;
		case 1024:TCCR1B=0b00001101;break;
		case Falling:TCCR1B=0b00001110;break;
		case Rising:TCCR1B=0b00001111;break;
	}

}






	void PWM1(double Vin, double VOUT, int CurrentD,int Register,int INPUT, short bitsize){
		long duty=0;
		// Fast PWM
		if(INPUT==0){ // If volts

			if(CurrentD==1){double Vout = Vin-VOUT; duty=100/(Vin/Vout);  }//Positive =1
			else if(CurrentD==0){duty=100/(Vin/VOUT);}
			
			duty=(duty*(pow(2,bitsize)))/100;

			if(Register==0)//Timer A
			{
				OCR1A=duty;
				}else if(Register==1){
			OCR1B=duty;}
		}

		if(INPUT==1){if(Register==0){
			OCR1A=VOUT;
			}else if(Register==1){
		OCR1B=VOUT;}
	}

}





void PWM2(double Vin, double VOUT, int CurrentD,int INPUT){
	//Math
	char duty=0;
	if(INPUT ==0){
		if(CurrentD==1)
		{
			double Vout = Vin-VOUT; //Find Out Voltage;
			duty=100/(Vin/Vout);  //Fine the On percentage
		}
		else if(CurrentD==0)
		{duty=100/(Vin/VOUT); //Fine the On percentage
		}
		duty=(duty*255)/100;               //Find the Porpoteinal vaule of DUTY/100 & OCR0V/255;
	OCR2=duty;}
	if(INPUT==1){OCR2=VOUT;}
}


void  Input_capInit(int Clk){
	TCCR1B|=(1<<ICNC1);
	TCCR1B|=(1<<ICES1);
	TIMSK|=(1<<TICIE1);
	if(Clk==Falling){TCCR1B=(1<<CS12)|(1<<CS11)|(0<<CS10);TCCR1B&=~(1<<ICES1);}
	if(Clk==Rising){TCCR1B=(1<<CS12)|(1<<CS11)|(1<<CS10);TCCR1B|=(1<<ICES1);}
}





void ADC_Init(void){
	SFIOR &=~(1<<ACME);
	ADMUX|=(1<<REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
}

uint16_t ADC_Read(char channel){
	uint16_t Ain,AinLow;
	ADMUX=ADMUX|(channel & 0x0f);	/* Set input channel to read */

	
	
	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	_delay_us(10);
	AinLow = (int)ADCL;		/* Read lower byte*/
	Ain = (int)ADCH*256;		/* Read higher 2 bits and
	Multiply with weight */
	Ain = Ain + AinLow;
 	//ADMUX=0x00;
	return(Ain);			/* Return digital value*/
}










void AnalogCOMP(int Out, int INPUTcapt, int Mode, int IE){
	ADCSRA &= (1<<ADEN);
	ADMUX=0x0;
	SFIOR |=(1<<ACME);
	ACSR= 0x00;
	ACSR |= (Out<<ACO)| (IE<<ACIE) | (INPUTcapt<<ACIC);

	switch (Mode){
		case 1: ACSR|= (1<<ACIS1);
		break;
		case 2 : ACSR |= (1<ACIS1)| (1<<ACIS0);
		break;
	}



}



long Map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#endif