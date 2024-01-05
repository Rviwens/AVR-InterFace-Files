 #define F_CPU 16000000UL	// Define CPU frequency here 20MHZ
 #include <avr/io.h>
 #include <util/delay.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <avr/interrupt.h>
 #include <math.h>
 # define Rising  10
 #define Falling 100
 #define TOG 102
 #define OFF 0
 #define SET 19
 #define INV 21

 
 // 	Atmega 16 Timer Registers
 // TCCR0(|FOC0|WGM00|COM01|COM00|WGM01|CS02|CS01|CS00|)
 //TIMSK(OCIE2|TOIE2|TICIE1|OCIE1A|OCIE1B|TOIE1|OCIE0|TOIE0|)
 //TCCR1A(COM1A1|COM1A0| COM1B1|COM1B0 FOC1A|FOC1B|WGM11|WGM10|)
 //TCCR1B(|ICNC1|ICES1|–|WGM13|WGM12|CS12|CS11|CS10|)
 
 
void Timer0( int Prescaler,int Register, int Out, int wgm3, int wgm2, int wgm1){
TCCR0A |=(wgm1<<WGM00)|(wgm2<<WGM01);
TCCR0B |=(wgm3<<WGM02);
 if(Register==0){
		  if(wgm1==0&&wgm2==1&&wgm3 ==0){TIMSK0|=(1<<OCIE0A);}
		 switch(Out){
			 case OFF:TCCR0A |=(0<<COM0A1)|(0<<COM0A0);break;
			 case TOG:TCCR0A |=(0<<COM0A1)|(1<<COM0A0);break;
			 case SET:TCCR0A |=(1<<COM0A1)|(0<<COM0A0);break;
			 case INV:TCCR0A |=(1<<COM0A1)|(1<<COM0A0);break;
		 }
	 }
	 if(Register==1){
		 	 if(wgm1==0&&wgm2==1&&wgm3 ==0){TIMSK0|=(1<<OCIE0B);}
		 switch(Out){
			 case OFF:TCCR0A |=(0<<COM0B1)|(0<<COM0B0);break;
			 case TOG:TCCR0A |=(0<<COM0B1)|(1<<COM0B0);break;
			 case SET:TCCR0A |=(1<<COM0B1)|(0<<COM0B0);break;
			 case INV:TCCR0A |=(1<<COM0B1)|(1<<COM0B0);break;
		 }
	 }
	 switch(Prescaler){
		 case 0:TCCR0B|=(0<<CS02)|(0<<CS01)|(1<<CS00);break;
		 case 8:TCCR0B|=(0<<CS02)|(1<<CS01)|(0<<CS00);break;
		 case 64:TCCR0B|=(0<<CS02)|(1<<CS01)|(1<<CS00);break;
		 case 256:TCCR0B|=(1<<CS02)|(0<<CS01)|(0<<CS00);break;
		 case 1024:TCCR0B|=(1<<CS02)|(0<<CS01)|(1<<CS00);break;
		 case Falling:TCCR0B|=(1<<CS02)|(1<<CS01)|(0<<CS00);break;
		 case Rising:TCCR0B|=(1<<CS02)|(1<<CS01)|(1<<CS00);break;
	 }
 }
 
void T0CompA(int ocr0){
OCR0A=ocr0;
}


void T0CompB(int ocr0AasTOP, int ocr0BasComp){
		OCR0A=ocr0AasTOP;
		OCR0B=ocr0BasComp;
}



void Timer1( int Prescaler,int Register, int Out, int wgm13, int wgm12, int wgm11, int wgm10){
TCCR1A |=(wgm10<<WGM10)|(wgm11<<WGM11);
TCCR1B |=(wgm12<<WGM12)|(wgm13<<WGM13);
if(Register==0){	
	if(wgm10==0&&wgm11==0&&wgm12 ==1&& wgm11==0 ){TIMSK1|=(1<<OCIE1A);}
switch(Out){
case OFF:TCCR1A |=(0<<COM1A1)|(0<<COM1A0);break;
case TOG:TCCR1A |=(0<<COM1A1)|(1<<COM1A0);break;
case SET:TCCR1A |=(1<<COM1A1)|(0<<COM1A0);break;
case INV:TCCR1A |=(1<<COM1A1)|(1<<COM1A0);break;
}
}
if(Register==1){
	if(wgm10==0&&wgm11==0&&wgm12 ==1){TIMSK1|=(1<<OCIE1B);}
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


void T1CompA(int ocr0){
	OCR1A=ocr0;
}
void T1CompB(int ocr0AasTOP, int ocr0BasComp){
	OCR1A=ocr0AasTOP;
	OCR1B=ocr0BasComp;
}




 void PWM0(double Vin, double VOUT,int polarity,int INPUT,int Reg){
unsigned char duty;
unsigned int ocr=0;

if(INPUT==0){
if(polarity==1)
{
double Vout = Vin-VOUT; //Find Out Voltage;
duty=100/(Vin/Vout);  //Fine the On percentage
}
else if(polarity==0)
{
duty=100/(Vin/VOUT); //Fine the On percentage
}
duty=(duty*0xFF)/100;    //Find the Porpoteinal vaule of DUTY/100 & OCR0V/255;
ocr=duty;
} 
if(INPUT == 1){ocr=VOUT;}
	
if(Reg==0){
OCR0A=ocr;
}
if(Reg==1){
OCR0B=ocr;
} 
}



void PWM1(double Vin, double VOUT,int polarity,int INPUT,int Reg, short bitSize){
	unsigned int duty;
	unsigned int ocr=0;
	if(INPUT==0){
		if(polarity==1)
		{
			double Vout = Vin-VOUT; //Find Out Voltage;
			duty=100/(Vin/Vout);  //Fine the On percentage
		}
		else if(polarity==0)
		{
			duty=100/(Vin/VOUT); //Fine the On percentage
		}
		duty=(duty*(pow(2,bitSize)))/100;    //Find the Porpoteinal vaule of DUTY/100 & OCR0V/255;
		ocr=duty;
	}
	if(INPUT == 1){ocr=VOUT;}
	if(Reg==0){
		OCR1A=ocr;
	}
	if(Reg==1){
		OCR1B=ocr;
	}
}



void Timer2( int Prescaler,short Register, short Out, int wgm12, int wgm11, int wgm10, int ocr2 ){
	TCCR2A |=(wgm10<<WGM20)|(wgm11<<WGM21);
	TCCR2B |=(wgm12<<WGM22);

	if(Register==0){
		OCR2A=ocr2;
		if(wgm10==0&&wgm11==1&&wgm12 ==0){TIMSK2|=(1<<OCIE2A);}
		switch(Out){
			 case OFF:TCCR2A |=(0<<COM2A1)|(0<<COM2A0);break;
			 case TOG:TCCR2A |=(0<<COM2A1)|(1<<COM2A0);break;
			 case SET:TCCR2A |=(1<<COM2A1)|(0<<COM2A0);break;
			 case INV:TCCR2A |=(1<<COM2A1)|(1<<COM2A0);break;
		 }
	 }
	 if(Register==1){
		 OCR0B=ocr2;
		 if(wgm10==0&&wgm11==1&&wgm12 ==0){TIMSK2|=(1<<OCIE2B);}
		 switch(Out){
			 case OFF:TCCR2A |=(0<<COM2B1)|(0<<COM2B0);break;
			 case TOG:TCCR2A |=(0<<COM2B1)|(1<<COM2B0);break;
			 case SET:TCCR2A |=(1<<COM2B1)|(0<<COM2B0);break;
			 case INV:TCCR2A |=(1<<COM2B1)|(1<<COM2B0);break;
		 }
	 }
	 switch(Prescaler){
		 case 0:TCCR2B|=(0<<CS22)|(0<<CS21)|(1<<CS20);break;
		 case 8:TCCR2B|=(0<<CS22)|(1<<CS21)|(0<<CS20);break;
		 case 64:TCCR2B|=(0<<CS22)|(1<<CS21)|(1<<CS20);break;
		 case 256:TCCR2B|=(1<<CS22)|(0<<CS21)|(0<<CS20);break;
		 case 1024:TCCR2B|=(1<<CS22)|(0<<CS21)|(1<<CS20);break;
		 case Falling:TCCR2B|=(1<<CS22)|(1<<CS21)|(0<<CS20);break;
		 case Rising:TCCR2B|=(1<<CS22)|(1<<CS21)|(1<<CS20);break;
	 }
 } 
 
 
 
void  Input_capInit(int Clk){
	TCCR1B|=(1<<ICNC1);
	TCCR1B|=(1<<ICES1);
	TIMSK1|=(1<<ICIE1);
	if(Clk==Falling){TCCR1B=(1<<CS12)|(1<<CS11)|(0<<CS10);TCCR1B&=~(1<<ICES1);}
	if(Clk==Rising){TCCR1B=(1<<CS12)|(1<<CS11)|(1<<CS10);TCCR1B|=(1<<ICES1);}
}

void ADC_Init(void){
	ADMUX|=(1<<REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
}
int ADC_Read(char channel){
	ADC_Init();
	int Ain,AinLow;
	ADMUX=ADMUX|(channel & 0x0f);	/* Set input channel to read */
	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	_delay_us(10);
	AinLow = (int)ADCL;		/* Read lower byte*/
	Ain = (int)ADCH*256;		/* Read higher 2 bits and
	Multiply with weight */
	Ain = Ain + AinLow;
	ADMUX=0x00;
	return(Ain);			/* Return digital value*/
}

long MAP(long IN, long vaule1, long vaule2){
vaule2*=IN;
vaule2/=vaule1;
return vaule2;
}

