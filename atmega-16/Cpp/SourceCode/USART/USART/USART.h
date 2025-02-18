#ifndef USART_COM
#define USART_COM

#ifndef F_CPU
#define F_CPU 20000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>
#include <StringFunctions.h>


#define Buffer_Size 128
#define FequncyPin PIND7


class USART{
	public:
	//default constructor
	USART();
	USART(int parity, int bitsize, long baud);
	/*
	* Initializes pins for IR communication
	*/
	void IR_Init();
	/*
	* Initializes USART communication
	* @param parity - Sets parity. 0 for disabled, 1 for even parity, 2 for odd parity
	* @param bitsize - sets the number of data bit in a frame the RX and TX use.
	* @param baud - Sets the baud rate (transmission speed)
	*/
	void Init(int parity, int bitsize, long baud);
	/*
	* Sends a byte of data through USART
	* @param ch - Byte to be sent
	*/
	void TxChar(char str);
	/*
	* Sends a String of data(chars) through USART
	* @param str - Pointer to string being sent
	*/
	void Send(char*str);
	void Send(const char*str);
	/*
	* Sends a String of data(chars) through USART with an optional ending tag
	* @param str - Pointer to string being sent
	* @param ES - if set, a ending tag will be sent (" |E")
	*/
	void Send_ESS(char *str,short ES);
	void Send_ESS(const char *str,short ES);
	/*
	* Sends a integer in decimal through USART
	* @param I - integer being sent
	* @param ES - if set, a ending tag will be sent (" |E")
	*/
	void Int_Str(int I, short ES);
	void Int_Str(uint8_t I, short ES);
	/*
	* Sends a integer in binary through USART
	* @param I - integer being sent
	* @param ES - if set, a ending tag will be sent (" |E")
	*/
	void Int_StrBIN(int I, short ES);
	/*
	* Sends a integer in hexadecimal through USART
	* @param I - integer being sent
	* @param ES - if set, a ending tag will be sent (" |E")
	*/
	void Int_StrHEX(int I, short ES);
	/*
	* Sends a integer in hexadecimal through USART without "0x"
	* @param I - integer being sent
	* @param ES - if set, a ending tag will be sent (" |E")
	*/
	void Int_StrHEXRAW(int I, short ES);
	/*
	* Sends a long integer in decimal through USART
	* @param I - integer being sent
	* @param ES - if set, a ending tag will be sent (" |E")
	*/
	void Long_Str(long I, short ES);
	/*
	* Sends a long integer in hexadecimal through USART
	* @param I - integer being sent
	* @param ES - if set, a ending tag will be sent (" |E")
	*/
	void Long_StrHEX(long I,short ES);
	/*
	* Sends a long integer in binary through USART
	* @param I - integer being sent
	* @param ES - if set, a ending tag will be sent (" |E")
	*/
	void Long_StrBIN(long I, short ES);
	void Dtoa(double d, short ES);
	/*
	* Checks if RX buffer has data
	*/
	void CheckRx();
	/*
	* Ends USART
	*/
	void END();

	private:
	long BAUD;
	char NUM_Hold[20];
	unsigned char RxSerialBuffer[Buffer_Size];
	int RXSBWP;
	unsigned char RXSB[Buffer_Size];
	/*
	* Sets a delay time a known baud rate
	*/
	void T();

};

#endif