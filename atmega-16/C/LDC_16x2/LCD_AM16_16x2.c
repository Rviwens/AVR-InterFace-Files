#include "LCD_AM16_16x2.h"
 

void E_Pulse(void){COMMAND_PORT|=(1<<E);_delay_us(20);COMMAND_PORT&=~(1<<E);_delay_us(20);}


void LCD_Command (uint8_t command){
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
 
void LCD_character(uint8_t character){
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
 
void LCD_Init(char mode){
Mode=mode;
if(Mode==8){
LCD_Command(0x0C);
_delay_ms(500);		
LCD_Command (0x38); 	
LCD_Command (0x0E);		/* Display ON Cursor OFF */
LCD_Command (0x06);		/* Auto Increment cursor */
LCD_Command (0x01);		/* Clear display */
LCD_Command (0x80);
}if(Mode==4){
LCD_Command(0x33);
LCD_Command(0x32);	/* Send for 4 bit initialization of LCD  */
LCD_Command(0x28);	/* 2 line, 5*7 matrix in 4-bit mode */
LCD_Command(0x0E);/* Display on cursor on */
LCD_Command(0x10);	//* Increment cursor (shift cursor to right) */
LCD_Command(0x01);
}
}


void LCD_Clear(){LCD_Command (0x01);}

void LCD_Home(){LCD_Command(0x02);}


void LCD_Set_Cursor(int row,int pos){
if (row == 0 && pos<16)
LCD_Command((pos & 0x0F)|0x80);	
else if (row == 1 && pos<16)
LCD_Command((pos & 0x0F)|0xC0);
}


void LCD_ShiftCursor(int Direction){
if (Direction==0){LCD_Command(0x04);}
if (Direction==1){	LCD_Command(0x06);}
}


void LCD_ShiftDisplay(int Direction){
if (Direction==0){LCD_Command(0x05);}
if (Direction==1){LCD_Command(0x07);}
}


void LCD_String (char* str){int i;for(i=0;str[i]!=0;i++){LCD_character(str[i]);}}


void LCD_String_xy (char row, char pos, char *str){
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	
	else if (row == 1 && pos<16){
	LCD_Command((pos & 0x0F)|0xC0);	
	LCD_String(str);		
}
}


void LCD_Long_Str(unsigned long Vaule, int Number_System){
long GH =10000000;
char HOLD[GH];
ltoa(Vaule,HOLD,Number_System);
LCD_String(HOLD);

}


void LCD_Int_Str(unsigned int INT, int Number_System){
    char Holder[100];
	itoa(INT,Holder,Number_System);
	LCD_String(Holder);
}
void LCD_dtoa(double doub){
    char Holder[100];
    dtoa(doub,Holder);
    LCD_String(Holder);
}

void LCD_Custom (unsigned char loc,int R1, int R2,int R3,int R4, int R5, int R6, int R7, int R8)
{
	unsigned char msg[8];
	
	
if(loc==1){unsigned char Character1[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; UpdateString(Character1,msg);}else
if(loc==2){unsigned char Character2[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; UpdateString(Character2,msg);}else
if(loc==3){unsigned char Character3[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; UpdateString(Character3,msg);}else
if(loc==4){unsigned char Character4[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; UpdateString(Character4,msg);}else
if(loc==5){unsigned char Character5[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; UpdateString(Character5,msg);}else
if(loc==6){unsigned char Character6[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; UpdateString(Character6,msg);}else
if(loc==7){unsigned char Character7[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; UpdateString(Character7,msg);}else
if(loc==8){unsigned char Character8[8] = {R1,R2,R3,R4,R5,R6,R7,R8}; UpdateString(Character8,msg);}

    unsigned char i;
    if(loc<8)
    {
     LCD_Command (0x40 + (loc*8));  /* Command 0x40 and onwards forces 
                                       the device to point CGRAM address */
       for(i=0;i<8;i++)  /* Write 8 byte for generation of 1 character */
           LCD_character(msg[i]);      
    }   
}