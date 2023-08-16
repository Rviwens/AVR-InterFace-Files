#define F_CPU 8000000UL	// Define CPU frequency here 8MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>

/* Define bit rate */
#define SCL_CLK 400000
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))
// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)


int USISIF_Check(){
	if((USISR&(1<<USISIF))){
	 return 1; 
	 USISR|=(USISIF<<1);
	}else
	return 0;
}
void I2C_Transfer()
{
	do
	{
		USICR|=(1<<USITC);   //Clock signal from SCL
		while((PINB&(1<<PB2))); //Waiting for SCL to go high
		_delay_ms(5);
		USICR|=(1<<USITC);  //Toggle SCL to go low
		_delay_ms(5);
	}while(!(USISR&(1<<USIOIF)));  //Repeat clock generation at SCL until the counter overflows and a byte is transferred
	_delay_ms(5);
	USISR|=(1<<USIOIF);      //Clear overflow flag
}

void I2C_ack()
{
	DDRB&=~(1<<PB0);  //Change direction of SDA to receive acknowledge bit
	USISR|=(1<<USICNT3)|(1<<USICNT2)|(1<<USICNT1); //Set counter to 1110 to force it to overflow when ACK bit is received
	I2C_Transfer();   //Byte transmission
}


void I2C_MAST_init()
{
DDRB|=(1<<PB0)|(1<<PB2); //SDA & SCL direction as output
PORTB|=(1<<PB0)|(1<<PB2); //SDA & SCL default state

 USIDR    =  0xFF;                      // Preload data register with "released level" data.
 
 USICR=0x00;
 USICR    |=  (1<<USISIE)|(0<<USIOIE);                            // Disable Interrupts.
 USICR    |= (1<<USIWM1)|(0<<USIWM0);                           // Set USI in Two-wire mode.
 USICR    |= (1<<USICS1)|(0<<USICS0)|(1<<USICLK);                // Software stobe as counter clock source
 USICR    |= (0<<USITC);
 
 USISR=0x00;
 USISR   |=   (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC);      // Clear flags,
 USISR  |=(0x0<<USICNT0);
}

I2C_Start(uint8_t Address)
{
	//Send Start Condition
	PORTB&=~(1<<PB0);  //Pulling SDA line low
	_delay_ms(5);
	PORTB&=~(1<<PB2);  //Pulling SCL line low
    _delay_ms(5);
	
	while(USISIF_Check()==0); //detection of start condition
	 //PORTB|=(1<<PB1);
	USIDR=Address;	
	I2C_Transfer();
	I2C_ack();
//if(USIDR&0x01)
// 	 return 0;
// 	else 
// 	 return 1; 
}

void I2C_Send(char tosend[])
{
	DDRB|=(1<<PB0);
	short int i=0;
	while(tosend[i]!='\0')  //Actual data string "test"
	{
		USIDR=tosend[i];     //Placing byte in Data register
		I2C_Transfer();      //Transfer the data placed in register
		i++;
	}
}
void I2C_Write(char i)
{
	DDRB|=(1<<PB0);
		USIDR=i;     //Placing byte in Data register
		I2C_Transfer();      //Transfer the data placed in register
}

void I2C_Stop()
{
	PORTB|=(1<<PB2);       //Pulling SDA high
	_delay_ms(5);
	PORTB|=(1<<PB0);       //Pulling SCL low
	_delay_ms(5);
}



short int flag;
short int i=0;

 //PB2 - SCL
 //PB0 - SDA

 void I2C_Slave_Init()
 {
	 USICR=(1<<USIWM1)|(1<<USICS1)|(1<<USICLK)|(1<<USISIE);  //TWI mode
	 DDRB=0x00; //SDA & SCL direction as input
	 PORTB=0x00; //SDA & SCL default state
	 USISR=0x00;       //Counter value
 }

 void i2c_address(uint8_t address)
 {
	 char usi_data;
	 while (USISIF_Check()==0);
	 {
	     //Wait till address bit is received
		 usi_data=USIDR;
		 if(usi_data==address)  //Verifying the address
		 {
			 i2c_ack();   //Send acknowledge bit
			 flag=1;
		 }
	 }
 }

 void i2c_ack()
 {
	 DDRB|=(1<<PB0);       //Set the direction to output
	 USISR|=(1<<USIOIF);   //clear overflow flags
	 PORTB|=(1<<PB0);
	 while((USISR&0x01)==0); //Wait until counter goes 1
	 PORTB&=~(1<<PB0);     //Ack bit end
	 USISR&=0xF0;          //Clear counter bits
 }

//  void receive_data()
//  {
// 	 if((flag==1)&&(USIOIF==1))    //Checked address and counter overflow bits
// 	 {
// 		 [i]=USIDR;   //Received data stored in string
// 		 i++;
// 		 USISR|=(1<<USIOIF);  //Clear overflow flag
// 	 }
//  }

 
