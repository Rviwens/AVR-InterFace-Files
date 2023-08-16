 #define F_CPU 16000000UL
 #define EN PIND2
 #define COMMANDPORT PORTD
 
 void HC_05_Info(void){
	_delay_ms(1000);
	USART_Send("AT\r\n",0);
	_delay_ms(1000);
	USART_Send("AT+VERSION?\r\n",0);
	_delay_ms(1000);
	USART_Send("AT+ADDR?\r\n",0); 
	_delay_ms(1000);
	USART_Send("AT+NAME?\r\n",0);
	_delay_ms(1000);
	USART_Send("AT+PSWD?\r\n",0);
	_delay_ms(1000);
	USART_Send("AT+UART?\r\n",0);
	_delay_ms(1000);
	USART_Send("AT+STATE?\r\n",0);
	_delay_ms(1000);
	USART_Send("AT+ROLE?\r\n",0);
	_delay_ms(1000);	 
	USART_Send("AT+POLAR?\r\n",0);
 }
 
