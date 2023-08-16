#define F_CPU 20000000UL	// Define CPU frequency here 20MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#define Red 1
#define Green 2
#define Blue 3
#define White 4
#define ColorPort PORTD
#define ColorDDR DDRD
#define S0  PD0
#define S1  PD1
#define S2  PD2
#define S3  PD3


 #define DHT11_PIN PIND6
  #define DHT11_PORT PORTD
 #define DHT11_DDR DDRD
 #define DHT11_pin PIND
 uint8_t dta,I_RH,D_RH,I_Temp,D_Temp,CheckSum;
static DHE=0;


int button(int pinport,int pin){
int Vaule=0;
if(pinport&(1<<pin)){Vaule=1;}else{Vaule=0;}
return Vaule;
}



void Request(){
DHT11_DDR |= (1<<DHT11_PIN);
DHT11_PORT &= ~(1<<DHT11_PIN);	/* set to low pin */
_delay_ms(20);			/* wait for 20ms */
DHT11_PORT |= (1<<DHT11_PIN);	/* set to high pin */
}

void Response(){	
long long i=0;	
int P1=0;
DHT11_DDR &= ~(1<<DHT11_PIN); // Set To input;
while(button(DHT11_pin,DHT11_PIN)){i++; if(i>200000){P1=2;break;}}// Watch for low pulse
i=0;
while(button(DHT11_pin,DHT11_PIN)==0){i++; if(i>200000){P1=2;break;}}//Watch for Pulse End
i=0;
while(button(DHT11_pin,DHT11_PIN)){i++; if(i>200000){P1=2;break;}} // Watch for Data Pulse
i=0;
if(P1==2){P1=0; DHE=1;}else{DHE=0;}
}

uint8_t Receive_data(){
if(DHE==0){
dta=0;
for (int q=0; q<8; q++){
while(button(DHT11_pin,DHT11_PIN) == 0);  // Wait For Next Bit of Data
_delay_us(35);
if(button(DHT11_pin,DHT11_PIN)==1){
dta=(dta<<1)|0x01;
}else{
dta=(dta<<1);
}
while(button(DHT11_pin,DHT11_PIN)==1);
}
return dta; 
}
}

void DHT11(){
Request();
Response();
if(DHE==0){
I_RH=Receive_data();	/* store first eight bit in I_RH */
D_RH=Receive_data();	/* store next eight bit in D_RH */
I_Temp=Receive_data();	/* store next eight bit in I_Temp */
D_Temp=Receive_data();	/* store next eight bit in D_Temp */
CheckSum=Receive_data(); /* store next eight bit in CheckSum */
}
}

void DHT11_Print_Data(){
LCD_Clear();	
if (DHE==0){	
LCD_CursorPostion(0,0);
LCD_String("Hum =");
LCD_CursorPostion(1,0);
LCD_String("Temp=");
LCD_CursorPostion(0,6);
LCD_int_Str(I_RH,10);
LCD_String(".");
LCD_int_Str(D_RH,10);
LCD_String("%");
LCD_CursorPostion(1,6);
LCD_int_Str(I_Temp,10);
LCD_String(".");
LCD_int_Str(D_Temp,10);
LCD_character(0xDF);
LCD_String("C");
LCD_CursorPostion(0,13);
LCD_int_Str(CheckSum,10);
}//else if(DHE==1){LCD_CursorPostion(0,0); LCD_String("Check Connection");}
}

void DHT11_USART_Data(int Ebit,int Vbit , int CFbit){
	if(Vbit==0){
	USART_Send("|HUM|",Ebit);
	USART_Int_Str(I_RH,Ebit);
	USART_Send("|HN|",Ebit);
	USART_Int_Str(D_RH,Ebit);
	USART_Send("|TEM|",Ebit);
	USART_Int_Str(I_Temp,Ebit);
	USART_Send("|TN|",Ebit);
	USART_Int_Str(D_Temp,Ebit);
	}else if(Vbit==1){
		
	double HUM= I_RH + (D_RH/100);
	double TEMP= I_Temp + (D_Temp/100);
	char HUMStorage[10];
	char TEMPStorage[10];
	if(CFbit==1){TEMP=(TEMP*1.8)+32;}
	int RH = TEMP*10;
	//if(((TEMP*10)/RH)!=1){TEMP+=0.01;}
		
	dtoa(HUM,HUMStorage);
	dtoa(TEMP,TEMPStorage);
	USART_Send("\r\n|HUM|",Ebit);
	USART_Send(HUMStorage,Ebit);
	USART_Send("\r\n|TEM|",Ebit);	
	USART_Send(TEMPStorage,Ebit);
	//if(DHE==1){USART_Send("Check Connection",0);}
	}
	
}




int ColorSensorSettings(int scaler, int color){
	
switch (scaler){
	case 0: ColorPort&=~(1<<S1); ColorPort&=~(1<<S0); break;
	case 2: ColorPort&=~(1<<S1); ColorPort|=(1<<S0); break;
 	case 1: ColorPort|=(1<<S1); ColorPort&=~(1<<S0); break;
	case 3: ColorPort|=(1<<S1)|(1<<S0);             break;
}

switch (color){
	case 1: ColorPort&=~(1<<S3); ColorPort&=~(1<<S2);break;
	case 2: ColorPort|=(1<<S3)|(1<<S2);break;
	case 4: ColorPort&=~(1<<S3); ColorPort|=(1<<S2);break;
	case 3: ColorPort|=(1<<S3); ColorPort&=~(1<<S2);break;
}
}




