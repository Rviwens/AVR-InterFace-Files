
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>


#define Rising  11
#define Falling 12
#define TOG 13
#define OFF 21
#define SET 22
#define INV 23
//#define F_CPU 20000000UL// Define CPU frequency here 20MHZ
#define Timer

	
void Timer0(int Mode, int Prescaler, int Out);
void Timer0_Compare(int Ticks);

void Timer2(int Mode, int Prescaler, int Out);
void Timer2_Compare(int Ticks);

void Timer1( int Prescaler,int Register, int Out, int wgm13, int wgm12, int wgm11, int wgm10);
void Timer1_Compare(int timer_ticks,int prescaler, int Vaule,int compare);

void PWM0(double Vin, double VOUT,short CurrentD,short INPUT,short reg);
void PWM1(double Vin, double VOUT, int CurrentD,int Register,int INPUT, short bitsize);
void PWM2(double Vin, double VOUT, int CurrentD,int INPUT);

void  Input_capInit(int Clk);

void ADC_Init(void);
void ADC_Read(char channel);
void AnalogCOMP(int Out, int INPUTcapt, int Mode, int IE);