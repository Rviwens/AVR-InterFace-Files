#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

char StatusBuff[511];

#define USARTTX
#define USARTRX
#define SPITX
#define SPIMast
#define FAT32
#define SDRSB
#define SDWSB


char token;
long RDS;






