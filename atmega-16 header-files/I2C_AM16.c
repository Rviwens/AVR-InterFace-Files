#define F_CPU 20000000UL// Define CPU frequency here 20MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>
//#define I2CbufferSize 1024
/* Define bit rate */
#define SCL_CLK 400000
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))
// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
// 
// void I2C_Init();											/* I2C initialize function */
// uint8_t I2C_Start(char);						/* I2C start function */
// void I2C_Start_NORETURN(uint8_t);
// uint8_t I2C_Repeated_Start(char);				/* I2C repeated start function */
// void I2C_Stop();											/* I2C stop function */						/* Wait until stop condition execution */
// void I2C_Start_Wait(char );						/* I2C start wait function */
// uint8_t I2C_Write(char );								/* I2C write function */
// char I2C_Read_Ack();										/* I2C read ack function */
// char I2C_Read_Nack();										/* I2C read nack function */
// void I2C_Slave_Init(uint8_t );
// int8_t I2C_Slave_Listen();
// int8_t I2C_Slave_Transmit(char );
// char I2C_Slave_Receive();
// int I2C_Event(char*,char*);
// void I2C_SRS(int );
// void I2C_Read(int , char*);
// void I2C_Str(char*,int );
// void I2C_SI(int , int );



 // SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
void I2C_Init()												/* I2C initialize function */
{
	TWBR = BITRATE(TWSR = 0x00);							/* Get bit rate register value by formula */
}


uint8_t I2C_Start(char write_address)/* I2C start function */
{
	uint8_t status;		/* Declare variable */
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT); /* Enable TWI, generate START */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */
	if(status!=0x08)		/* Check weather START transmitted or not? */
	return 0;			/* Return 0 to indicate start condition fail */
	TWDR=write_address;		/* Write SLA+W in TWI data register */
	TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI & clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */
	if(status==0x18)		/* Check for SLA+W transmitted &ack received */
	return 1;			/* Return 1 to indicate ack received */
	if(status==0x20)		/* Check for SLA+W transmitted &nack received */
	return 2;			/* Return 2 to indicate nack received */
	else
	return 3;			/* Else return 3 to indicate SLA+W failed */
}
void I2C_Start_NORETURN(uint8_t write_address){
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);					/* Enable TWI, generate start condition and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));	
		TWDR = write_address;									/* If yes then write SLA+W in TWI data register */
		TWCR = (1<<TWEN)|(1<<TWINT);							/* Enable TWI and clear interrupt flag */
		while (!(TWCR & (1<<TWINT)));
}

uint8_t I2C_Repeated_Start(char read_address)				/* I2C repeated start function */
{
	uint8_t status;											/* Declare variable */
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);					/* Enable TWI, generate start condition and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (start condition) */
	status = TWSR & 0xF8;									/* Read TWI status register with masking lower three bits */
	if (status != 0x10)										/* Check weather repeated start condition transmitted successfully or not? */
	return 0;												/* If no then return 0 to indicate repeated start condition fail */
	TWDR = read_address;									/* If yes then write SLA+R in TWI data register */
	TWCR = (1<<TWEN)|(1<<TWINT);							/* Enable TWI and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (Write operation) */
	status = TWSR & 0xF8;									/* Read TWI status register with masking lower three bits */
	if (status == 0x40)										/* Check weather SLA+R transmitted & ack received or not? */
	return 1;												/* If yes then return 1 to indicate ack received */
	if (status == 0x20)										/* Check weather SLA+R transmitted & nack received or not? */
	return 2;												/* If yes then return 2 to indicate nack received i.e. device is busy */
	else
	return 3;												/* Else return 3 to indicate SLA+W failed */
}

void I2C_Stop()												/* I2C stop function */
{
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);					/* Enable TWI, generate stop condition and clear interrupt flag */
	while(TWCR & (1<<TWSTO));								/* Wait until stop condition execution */
}

void I2C_Start_Wait(char write_address)						/* I2C start wait function */
{
	uint8_t status;											/* Declare variable */
	while (1)
	{
		TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);				/* Enable TWI, generate start condition and clear interrupt flag */
		while (!(TWCR & (1<<TWINT)));						/* Wait until TWI finish its current job (start condition) */
		status = TWSR & 0xF8;								/* Read TWI status register with masking lower three bits */
		if (status != 0x08)									/* Check weather start condition transmitted successfully or not? */
		continue;											/* If no then continue with start loop again */
		TWDR = write_address;								/* If yes then write SLA+W in TWI data register */
		TWCR = (1<<TWEN)|(1<<TWINT);						/* Enable TWI and clear interrupt flag */
		while (!(TWCR & (1<<TWINT)));						/* Wait until TWI finish its current job (Write operation) */
		status = TWSR & 0xF8;								/* Read TWI status register with masking lower three bits */
		if (status != 0x18 )								/* Check weather SLA+W transmitted & ack received or not? */
		{
			I2C_Stop();										/* If not then generate stop condition */
			continue;										/* continue with start loop again */
		}
		break;												/* If yes then break loop */
	}
}

uint8_t I2C_Write(char data)								/* I2C write function */
{
	uint8_t status;											/* Declare variable */
	TWDR = data;											/* Copy data in TWI data register */
	TWCR = (1<<TWEN)|(1<<TWINT);							/* Enable TWI and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (Write operation) */
	status = TWSR & 0xF8;									/* Read TWI status register with masking lower three bits */
	if (status == 0x28)										/* Check weather data transmitted & ack received or not? */
	return 0;												/* If yes then return 0 to indicate ack received */
	if (status == 0x30)										/* Check weather data transmitted & nack received or not? */
	return 1;												/* If yes then return 1 to indicate nack received */
	else
	return 2;												/* Else return 2 to indicate data transmission failed */
}

char I2C_Read_Ack()											/* I2C read ack function */
{
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);					/* Enable TWI, generation of ack and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (read operation) */
	return TWDR;											/* Return received data */
}

char I2C_Read_Nack()										/* I2C read nack function */
{
	TWCR=(1<<TWEN)|(1<<TWINT);								/* Enable TWI and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (read operation)*/
	return TWDR;		/* Return received data */
}



void I2C_Slave_Init(uint8_t slave_address)
{
	TWAR=slave_address;		/* Assign Address in TWI address register */
	TWCR=(1<<TWEN)|(1<<TWEA)|(1<<TWINT);/* Enable TWI, Enable ack generation */

}

int8_t I2C_Slave_Listen()
{
	while(1)
	{
		uint8_t status;			/* Declare variable */
		while(!(TWCR&(1<<TWINT)));	/* Wait to be addressed */
		status=TWSR&0xF8;		/* Read TWI status register */
		if(status==0x60||status==0x68)	/* Own SLA+W received &ack returned */
		return 0;			/* Return 0 to indicate ack returned */
		if(status==0xA8||status==0xB0)	/* Own SLA+R received &ack returned */
		return 1;			/* Return 0 to indicate ack returned */
		if(status==0x70||status==0x78)	/* General call received &ack returned */
		return 2;			/* Return 1 to indicate ack returned */
		else
		continue;			/* Else continue */
	}
}
int8_t I2C_Slave_Transmit(char data)
{
	uint8_t status;
	TWDR = data;								/* Write data to TWDR to be transmitted */
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWEA);		/* Enable TWI and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));				/* Wait until TWI finish its current job (Write operation) */
	status = TWSR & 0xF8;						/* Read TWI status register with masking lower three bits */
	if (status == 0xA0)							/* Check weather STOP/REPEATED START received */
	{
		TWCR |= (1<<TWINT);						/* If yes then clear interrupt flag & return -1 */
		return -1;
	}
	if (status == 0xB8)							/* Check weather data transmitted & ack received */
	return 0;									/* If yes then return 0 */
	if (status == 0xC0)							/* Check weather data transmitted & nack received */
	{
		TWCR |= (1<<TWINT);						/* If yes then clear interrupt flag & return -2 */
		return -2;
	}
	if (status == 0xC8)							/* If last data byte transmitted with ack received TWEA = 0 */
	return -3;									/* If yes then return -3 */
	else										/* else return -4 */
	return -4;
}

// uint8_t I2C_read_slave(void)
// {
// 	// Clear TWI interrupt flag,Get acknowlegement, Enable TWI
// 	TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
// 	while (!(TWCR & (1<<TWINT))); // Wait for TWINT flag
// 	while((TWSR & 0xF8)!=0x80); // Wait for acknowledgement
// 	return TWDR; // Get value from TWDR//
// }

char I2C_Slave_Receive()
{
	uint8_t status;		/* Declare variable */
	TWCR=(1<<TWEN)|(1<<TWEA)|(1<<TWINT);/* Enable TWI & generation of ack */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */
	if(status==0x80||status==0x90)/* Check for data received &ack returned */
	return TWDR;		/* If yes then return received data */

	/* Check for data received, nack returned & switched to not addressed slave mode */
	if(status==0x88||status==0x98)
	return TWDR;		/* If yes then return received data */
	if(status==0xA0)		/* Check wether STOP/REPEATED START */
	{
		TWCR|=(1<<TWINT);	/* Clear interrupt flag & return -1 */
		return '|';
	}
	else
	return -2;			/* Else return -2 */
}



int I2C_Event(char*WriteBuffer,char* ReadBuffer){
int8_t count = 0;
int x = 3;

x =I2C_Slave_Listen();
int State=0;
if(x==0){
memset(WriteBuffer,0,strlen(WriteBuffer));
//USART_Send("\r\n WRITE REQUEST: ",0);
count=-1;
do{
count++;	
WriteBuffer[count] = I2C_Slave_Receive();
//USART_Send(WriteBuffer[count]);
} 
while (WriteBuffer[count]!='|');
WriteBuffer[count]=0;


State= 1;
}else if(x==1){	
//USART_Send("\r\n TRANSMISSION REQUEST\r\n ",0);
for(int i=0; i<strlen(ReadBuffer);i++){
I2C_Slave_Transmit(ReadBuffer[i]);	
}
I2C_Slave_Transmit('|');
I2C_Slave_Transmit('|');
State=2;	
}
return State;
}

void I2C_SRS(int address){
	I2C_Start(address);
	I2C_Write(0x00);
	I2C_Repeated_Start(address+1);
}

void I2C_Read(int address, char *str ){
I2C_SRS(address);
int count =-1;
do {
count++;
str[count]= I2C_Read_Ack();
if(str[count]=='|'){str[count]=I2C_Read_Nack();}
//USART_TxChar(array2[count]);
}
while (str[count]!='|');
str[count]=0;
I2C_Stop();	
}

void I2C_ReadWNES(int address, char *str, uint8_t NumOfBytes){
	I2C_SRS(address);
	int count =0;
	do {
		
		str[count]= I2C_Read_Ack();
		count++;
	}
	while (count<=NumOfBytes);
    I2C_Read_Nack();
	I2C_Stop();
}


void I2C_Str(char*string,int Address){
USART_Int_Str(I2C_Start(Address),0);
for (uint8_t i = 0; string[i]!=0; i++){
 I2C_Write(string[i]);
}
I2C_Stop();
}


void I2C_SI(int num, int address){
char holder[10];
itoa(num,holder,10);
I2C_Str(holder,address);	
}

// void I2C_Double_Str(double din,int address){
// 	char holder[100];
// 	dtoa(din,holder);
// 	I2C_Str(holder,address);
// }

