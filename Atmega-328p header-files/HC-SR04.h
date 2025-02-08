#define F_CPU 16000000UL
#define Trigger_pin PINB1
//Declare Variables
short ErrorFlag =0;
long count;
long double distance;
long long ErrorCount = 0;
int TimerOverflow = 0;

double HC_SR04_Distance(){
	DDRB|=(1<<PINB1);
	DDRB&=~(1<<PINB0);
	TIMSK1 |= (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;
	PORTB |= (1 << Trigger_pin);
	_delay_us(50);
	PORTB &= (~(1 << Trigger_pin));
	_delay_us(400);
	/* Wait for rising edge */
	while ((TIFR1 & (1 << ICF1)) == 0);

	//set Timer 1 to start counting the time for signal to be received
	TCNT1 = 0;	/* Clear Timer counter */
	TCCR1B = 0x01;	/* Capture on falling edge, No prescaler */
	TIFR1 = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
	TIFR1 = 1<<TOV1;	/* Clear Timer Overflow flag */
	TimerOverflow = 0;/* Clear Timer overflow count */
	ErrorCount=0;


	/* Wait for falling edge */

	while ((TIFR1 & (1 << ICF1)) == 0);
	//Caculate Distance
	count = ICR1 + (65535 * TimerOverflow);	/* Take count */
	/* 20MHz Timer freq, sound speed =343 m/s */
	distance = (double)count * 0.001071875;
	if (ErrorFlag!=1&& distance<=400){
			return distance;
		
	}
}


ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	/* Increment Timer Overflow count */
}




void Error(uint8_t ErrorCode){
	switch (ErrorCode)
	{
		case 0: USART_Send("\r\n Error At -> \"Waiting for Raising Edge \" \r\n",0); break;
		case 1: USART_Send("\r\nError At -> \"Waiting for Falling Edge \"\r\n ",0);break;
		case 2: USART_Send("\r\nError At -> \"UnKnown Error \" ",0);break;
	}
	
}