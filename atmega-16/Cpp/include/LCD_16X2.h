#ifndef LCD16X2
#define  LCD16X2

#ifndef F_CPU
#define F_CPU 20000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <StringFunctions.h>

#define COMMAND_PORT PORTB
#define DATA_PORT PORTB
#define RS PINB0
#define E  PINB1
#define RW PINB2
#define LCD_Port PORTB


class LCD_16X2 {	
public:
LCD_16X2();

LCD_16X2(uint8_t mode);

void Init(uint8_t mode);

void Clear();

void Home();

void Set_Cursor(int row,int pos);

void ShiftCursor(int Direction);

void ShiftDisplay(int Direction);

void Str (char *str);
void Str (const char *str);

void String_xy (char row, char pos, char *str);

void Long_Str(unsigned long Vaule, int Number_System);

void Int_Str(unsigned int INT, int Number_System);

void dtoa(double doub);

void Custom (unsigned char loc,uint8_t R1, uint8_t R2,uint8_t R3,uint8_t R4, uint8_t R5, uint8_t R6, uint8_t R7, uint8_t R8);

private:
short Mode;
String string;

void E_Pulse(void);

void Command (uint8_t command);

void character(uint8_t character);
};

#endif