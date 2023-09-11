#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>
#include "AVR_ATmega16_TimerFunctions.h"
#define Buffer_Size 128
#define FequncyPin PIND7







void IR_Init(void);
void USART_init(int parity, int bitsize, long baud);
void T(void);
void USART_TxChar(char ch);
void USART_Send(char str[]);
void USART_Send_ESS(char str[],int ES);
void USART_Int_Str(int I, int ES);
void USART_Int_StrBIT(int I, int ES);
void USART_Int_StrHEX(int I, int ES);
void USART_Long_Str(long I, int ES);
void CheckRx(void);
void USART_END(void);
void USART_SSE(int baudRate, char *data,short Ebit);
void USART_SSIE(int baudRate, int data,short Ebit);