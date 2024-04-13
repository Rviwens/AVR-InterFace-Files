

#define F_CPU 20000000UL
#define  Trigger_pin	PA2	/* Trigger pin */

volatile int TimerOverflow = 0;



ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	/* Increment Timer Overflow count */
}



void Error(uint8_t ErrorCode){
	switch (ErrorCode)
	{
		case 0: USART_Send("\r\n Error At -> \"Waiting for Raising Edge Error 0\" \r\n"); break;
		case 1: USART_Send("\r\nError At -> \"Waiting for Falling Edge Error 1\"\r\n ");break;
		case 2: USART_Send("\r\nError At -> \"UnKnown Error \" ");break;
	}

}

short ErrorFlag =0;
long count;
long double distance;
long long ErrorCount = 0;



double HC_SR04_Distance(void)
{
	DDRA |= (1<<Trigger_pin);/* Make trigger pin as output */
	PORTD |= (1<<PD6);

	TIMSK |= (1 << TOIE1);
	TCCR1A = 0;
	
	
ErrorCount=0;
count=0;
distance=0;
ErrorFlag=0; 
	/* Give 10us trigger pulse on trig. pin to HC-SR04 */
	PORTA |= (1 << Trigger_pin);
	_delay_us(15);
	PORTA &= (~(1 << Trigger_pin));
	
	TIMSK = (1 << TOIE1);
	TCCR1A = 0;
	TCNT1 = 0;	/* Clear Timer counter */
	TCCR1B |= 0x41;	/* Capture on rising edge, No prescaler*/
	TCCR1B |=(1<<CS10)|(1<<CS11);
	
	TIFR |= 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
	TIFR |= 1<<TOV1;	/* Clear Timer Overflow flag */

	/*Calculate width of Echo by Input Capture (ICP) */
	
	
	while ((TIFR & (1 << ICF1)) == 0)
	{
		ErrorCount++;
		if(ErrorCount>100000){
			Error(0);
			ErrorFlag=1;
			break;
		}
	}
	if(ErrorFlag==1)return 0;
	
	
TCNT1 = 0;	/* Clear Timer counter */
TCCR1B = 0x01;	/* Capture on falling edge, No prescaler */
TCCR1B |=(1<<CS10)|(1<<CS11);
	
TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */

TimerOverflow = 0;/* Clear Timer overflow count */


while ((TIFR & (1 << ICF1)) == 0)
{
	ErrorCount++;
	if(ErrorCount>100000){
		Error(1);
		ErrorFlag=1;
		break;
	}
}



if(ErrorFlag==1)return 0;
count = ICR1 + (65535 * TimerOverflow);	/* Take count */
/* 8MHz Timer freq, sound speed =343 m/s */
distance = (double)(17150*(count)*0.0000032);
TIMSK&=~(1 << TOIE1);
return distance;
}
