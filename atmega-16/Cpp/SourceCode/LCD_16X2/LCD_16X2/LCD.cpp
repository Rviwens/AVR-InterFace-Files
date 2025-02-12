#include "LCD_16x2.h"

 LCD_16X2::LCD_16X2(){
Init(8); 
 }

 LCD_16X2::LCD_16X2(uint8_t mode){Init(mode); }

void LCD_16X2::E_Pulse(void){COMMAND_PORT|=(1<<E);_delay_us(20);COMMAND_PORT&=~(1<<E);_delay_us(20);}


void LCD_16X2::Command (uint8_t command){
if(Mode==8){
DATA_PORT=command;
COMMAND_PORT &=~(1<<RS);
 E_Pulse();
}
if(Mode==4){
	LCD_Port = (LCD_Port & 0x0F) | (command & 0xF0);/* Sending upper nibble */
	LCD_Port &= ~ (1<<RS);		/* RS=0, command reg. */
	LCD_Port |= (1<<E);		/* Enable pulse */
	_delay_us(1);
	LCD_Port &= ~ (1<<E);
	_delay_us(100);
	LCD_Port = (LCD_Port & 0x0F) | (command << 4);/* Sending lower nibble */
	LCD_Port |= (1<<E);
	_delay_us(1);
	LCD_Port &= ~ (1<<E);
	_delay_ms(2);
}
LCD_Port =0X00;
}
 
void LCD_16X2::character(uint8_t character){
if(Mode==8){
DATA_PORT=character;
COMMAND_PORT|=(1<<RS);
E_Pulse();
DATA_PORT =0x00;
}
if(Mode==4){
	LCD_Port = (LCD_Port & 0x0F) | (character & 0xF0);/* Sending upper nibble */
	LCD_Port|= (1<<RS);  /* RS=1, data reg. */
	LCD_Port|= (1<<E);
	_delay_us(1);
	LCD_Port &= ~ (1<<E);
	_delay_us(100);
	LCD_Port = (LCD_Port & 0x0F) | (character << 4);  /* Sending lower nibble */
	LCD_Port |= (1<<E);
	_delay_us(1);
	LCD_Port &= ~ (1<<E);
	_delay_ms(2);
}
LCD_Port =0X00;
}
 
void LCD_16X2::Init(uint8_t mode){
Mode=mode;
if(Mode==8){
Command(0x0C);
_delay_ms(500);		
Command (0x38); 	
Command (0x0E);		/* Display ON Cursor OFF */
Command (0x06);		/* Auto Increment cursor */
Command (0x01);		/* Clear display */
Command (0x80);
}if(Mode==4){
Command(0x33);
Command(0x32);	/* Send for 4 bit initialization of LCD  */
Command(0x28);	/* 2 line, 5*7 matrix in 4-bit mode */
Command(0x0E);/* Display on cursor on */
Command(0x10);	//* Increment cursor (shift cursor to right) */
Command(0x01);
}
}


void LCD_16X2::Clear(){Command (0x01);}

void LCD_16X2::Home(){Command(0x02);}


void LCD_16X2::Set_Cursor(int row,int pos){
if (row == 0 && pos<16)
Command((pos & 0x0F)|0x80);	
else if (row == 1 && pos<16)
Command((pos & 0x0F)|0xC0);
}


void LCD_16X2::ShiftCursor(int Direction){
if (Direction==0){Command(0x04);}
if (Direction==1){Command(0x06);}
}


void LCD_16X2::ShiftDisplay(int Direction){
if (Direction==0){Command(0x05);}
if (Direction==1){Command(0x07);}
}

void LCD_16X2::Str (char *str){int i;for(i=0;str[i]!=0;i++){character(str[i]);}}
void LCD_16X2::Str (const char *str){Str((char*)str);}
	
void LCD_16X2::String_xy (char row, char pos, char *str){
	if (row == 0 && pos<16)
	Command((pos & 0x0F)|0x80);	
	else if (row == 1 && pos<16){
	Command((pos & 0x0F)|0xC0);	
	Str(str);		
}
}


void LCD_16X2::Long_Str(unsigned long Vaule, int Number_System){
long GH =10000000;
char HOLD[GH];
ltoa(Vaule,HOLD,Number_System);
Str(HOLD);
}


void LCD_16X2::Int_Str(unsigned int INT, int Number_System){
    char Holder[100];
	itoa(INT,Holder,Number_System);
	Str(Holder);
}
void LCD_16X2::dtoa(double doub){
    char Holder[100];
	string.dtoa(doub,Holder);
    Str(Holder);
}

void LCD_16X2::Custom (unsigned char loc,uint8_t R1, uint8_t R2,uint8_t R3,uint8_t R4, uint8_t R5, uint8_t R6, uint8_t R7, uint8_t R8)
{
unsigned char msg[8];
	
if(loc==1){unsigned char Character1[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; string.UpdateString(Character1,msg);}else
if(loc==2){unsigned char Character2[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; string.UpdateString(Character2,msg);}else
if(loc==3){unsigned char Character3[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; string.UpdateString(Character3,msg);}else
if(loc==4){unsigned char Character4[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; string.UpdateString(Character4,msg);}else
if(loc==5){unsigned char Character5[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; string.UpdateString(Character5,msg);}else
if(loc==6){unsigned char Character6[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; string.UpdateString(Character6,msg);}else
if(loc==7){unsigned char Character7[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; string.UpdateString(Character7,msg);}else
if(loc==8){unsigned char Character8[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; string.UpdateString(Character8,msg);}

    unsigned char i;
    if(loc<8)
    {
     LCD_16X2::Command (0x40 + (loc*8));  /* Command 0x40 and onwards forces 
                                       the device to point CGRAM address */
       for(i=0;i<8;i++)  /* Write 8 byte for generation of 1 character */
          LCD_16X2::character(msg[i]);      
    }   
}