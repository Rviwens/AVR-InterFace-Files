#define F_CPU 20000000UL	// Define CPU frequency here 20MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>

#define COMMAND_PORT PORTB
#define DATA_PORT PORTB
#define RS PINB0
#define E  PINB1
#define RW PINB2
#define LCD_Port PORTB

static int Mode;


void E_Pulse(void);

void LCD_Command (uint8_t command);

void LCD_character(uint8_t character);

void LCD_Init(char mode);

void LCD_Clear();

void LCD_Home();

void LCD_Set_Cursor(int row,int pos);

void LCD_ShiftCursor(int Direction);

void LCD_ShiftDisplay(int Direction);

void LCD_String (char *str);

void LCD_String_xy (char row, char pos, char *str);

void LCD_Long_Str(unsigned long Vaule, int Number_System);

void LCD_int_Str(unsigned int INT, int Number_System);

void LCD_dtoa(double doub);

void LCD_Custom (unsigned char loc,int R1, int R2,int R3,int R4, int R5, int R6, int R7, int R8);