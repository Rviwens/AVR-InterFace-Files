#define F_CPU 20000000UL
#define Trigger_pin PINA0


int TimerOverflow = 0;

void SendPulse(void){
	PORTA |= (1 << Trigger_pin);
	_delay_us(15);
	PORTA &= (~(1 << Trigger_pin));
}


void HC_SR04_Init(){
	// Set GPIO registers
	DDRA = 0x01;		/* Make trigger pin as output */
	PORTD = 0xFF;

	//Set Timer 1 to use Input Capture Pin and keep time
}

//Declare Variables
short ErrorFlag =0;
long count;
long double distance;
long long ErrorCount = 0;

double HC_SR04_Distance(){
	HC_SR04_Init();
	TIMSK |= (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;
	SendPulse();
	_delay_us(375);
	/* Wait for rising edge */
	while ((TIFR & (1 << ICF1)) == 0)
	{
		ErrorCount++;
		if(ErrorCount>1000000){
			Error(0);
			ErrorFlag=1;
			break;
		}
	};
	if(ErrorFlag==1){return 0.0000;}

	//set Timer 1 to start counting the time for signal to be received
	TCNT1 = 0;	/* Clear Timer counter */
	TCCR1B = 0x01;	/* Capture on falling edge, No prescaler */
	TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
	TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */
	TimerOverflow = 0;/* Clear Timer overflow count */
	ErrorCount=0;


	/* Wait for falling edge */

	while ((TIFR & (1 << ICF1)) == 0)
	{
		ErrorCount++;
		if(ErrorCount>1000000){
			Error(1);
			ErrorFlag=1;
			break;
		}
	}
	ErrorCount=0;
	if(ErrorFlag==1){return 0.0000;}
	//Caculate Distance

	count = ICR1 + (65535 * TimerOverflow);	/* Take count */
	/* 20MHz Timer freq, sound speed =343 m/s */
	distance = (double)count * 0.0008575;
	char testH[20];
	USART_Send("\r\n\nTesting Distance=||||| ");
	dtoa(distance,testH);
	USART_Send(testH);
	USART_Send("\r\n");
	if (ErrorFlag!=1&& distance<=400){
		return distance;

		} else{
		return 0.0000;
	}
}


ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	/* Increment Timer Overflow count */
}




void Error(uint8_t ErrorCode){
	switch (ErrorCode)
	{
		case 0: USART_Send("\r\n Error At -> \"Waiting for Raising Edge \" \r\n"); break;
		case 1: USART_Send("\r\nError At -> \"Waiting for Falling Edge \"\r\n ");break;
		case 2: USART_Send("\r\nError At -> \"UnKnown Error \" ");break;
	}
	
}