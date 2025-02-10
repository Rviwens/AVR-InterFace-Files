#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#define Buffer_Size 128
#define FequncyPin PIND7
static long BAUD;

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
void USART_init(int parity, int bitsize, long baud);
/*
* Sets a delay time a known baud rate
*/
void T();

#if defined(USARTTX)
/*
* Sends a byte of data through USART
* @param ch - Byte to be sent
*/
void USART_TxChar(char ch);
/*
* Sends a String of data(chars) through USART
* @param str - Pointer to string being sent
*/
void USART_Send(char *str);
/*
* Sends a String of data(chars) through USART with an optional ending tag
* @param str - Pointer to string being sent
* @param ES - if set, a ending tag will be sent (" |E")
*/
void USART_Send_ESS(char *str,int ES);
/*
* Sends a integer in decimal through USART
* @param I - integer being sent
* @param ES - if set, a ending tag will be sent (" |E")
*/
void USART_Int_Str(int I, int ES);
/*
* Sends a integer in binary through USART
* @param I - integer being sent
* @param ES - if set, a ending tag will be sent (" |E")
*/
void USART_Int_StrBIN(int I, int ES);
/*
* Sends a integer in hexadecimal through USART
* @param I - integer being sent
* @param ES - if set, a ending tag will be sent (" |E")
*/
void USART_Int_StrHEX(int I, int ES);
/*
* Sends a integer in hexadecimal through USART without "0x"
* @param I - integer being sent
* @param ES - if set, a ending tag will be sent (" |E")
*/
void USART_Int_StrHEXRAW(int I, int ES);
/*
* Sends a long integer in decimal through USART
* @param I - integer being sent
* @param ES - if set, a ending tag will be sent (" |E")
*/
void USART_Long_Str(long I, int ES);
/*
* Sends a long integer in hexadecimal through USART
* @param I - integer being sent
* @param ES - if set, a ending tag will be sent (" |E")
*/
void USART_Long_StrHEX(long I, int ES);
/*
* Sends a long integer in binary through USART
* @param I - integer being sent
* @param ES - if set, a ending tag will be sent (" |E")
*/
void USART_Long_StrBIN(long I, int ES);
#endif


#if defined(USARTRX)

unsigned char RxSerialBuffer[Buffer_Size];
int RXSBWP;
unsigned char RXSB[Buffer_Size];

/*
* Checks if RX buffer has data
*/
void CheckRx();
#endif	

#if defined(QuickUSART)
/*
* Ends USART 
*/
void USART_END();
/*
* Initializes USART communication and sends a string then ends USART
* @param baud - Sets the baud rate (transmission speed)
* @param data - Pointer to string being sent
* @param ES - if set, a ending tag will be sent (" |E")
*/
void USART_SSE(int baudRate, char *data,short Ebit);
/*
* Initializes USART communication and sends an integer then ends USART
* @param baud - Sets the baud rate (transmission speed)
* @param data - Integer being sent
* @param ES - if set, a ending tag will be sent (" |E")
*/
void USART_SSIE(int baudRate, int data,short Ebit);
#endif
