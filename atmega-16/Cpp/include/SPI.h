/****************************************************************************/
#ifndef SPI_COM
#define SPI_COM
/****************************************************************************/
#ifndef F_CPU
#define F_CPU 20000000UL
#endif
/****************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>
/****************************************************************************/
class SPI{
/****************************************************************************/
public:

//Default Constructor 
SPI();
//SPI as master Constructor 
SPI(int dataOrder,int Prescaler,int clockPolarity, int Cphase, int DoubleTime);
/****************************************************************************/
/*
* initializes the device as a SPI master 
* @param dataOrder - Set as 0 for MSB or as 1 for LSB
* @param Prescaler - SPI CLK divider: 4, 16, 64, 128
* @param clockPolarity - When this bit is written to 0, SCK is rising on leading edge. When set to 1, SCK is rising on falling edge.
* @param Cphase- Determines if data is sampled on the leading (0) or trailing (1) edge of SCK
* @param Double Time - When this bit is written logic one the SPI speed (SCK Frequency) will be doubled when the SPI is in Master mode
*/
void MAST_Init(int dataOrder,int Prescaler,int clockPolarity, int Cphase, int DoubleTime);
/*
* transfers one byte of data
* @param data - A byte of data to be transfered to slave device
* @return SPDR - A byte of data received from slave device
*/
uint8_t transfer(uint8_t data);
/*
* transfers a string over SPI
* @param str -Pointer to the desired string
*/
void Str(char *str);
/*
* transfers a string over SPI
* @param str -Pointer to the desired constant string
*/
void Str(const char *str);
/*
* SPI Slave receive data 
*/
char SR();
/****************************************************************************/
/*
* Initiates device as SPI slave
*/
void SLAVE_Init();
/*
* Ends SPI and clears registers
*/
void End();
/****************************************************************************/
private:
int Device_Mode;
/****************************************************************************/
};
/****************************************************************************/
#endif