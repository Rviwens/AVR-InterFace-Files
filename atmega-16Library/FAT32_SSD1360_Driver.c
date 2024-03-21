#define F_CPU 20000000UL// Define CPU frequency here 20MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#define FNAME "1360CHR"
#define StrLen 7

void FindChar(uint8_t chr){

uint16_t loc = (chr*7)+0x20;
char Overflow = (int)loc/512;
// Create A ConstMin for loc

FAT32_FILE_Read_Sector_In_Cluster("FNAME",0,Overflow);	
}

