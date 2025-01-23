#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>




#if defined(SPI_M)
static int Device_Mode;
/*
* Finds the first cluster and location in the root directory of a file
* @param dataOrder - Set as 0 for MSB or as 1 for LSB
* @param Prescaler - SPI CLK divider: 4, 16, 64, 128
* @param clockPolarity - When this bit is written to 0, SCK is rising on leading edge. When set to 1, SCK is rising on falling edge.
* @param Cphase- Determines if data is sampled on the leading (0) or trailing (1) edge of SCK
* @param Double Time - When this bit is written logic one the SPI speed (SCK Frequency) will be doubled when the SPI is in Master mode
*/
void SPI_MAST_Init(int dataOrder,int Prescaler,int clockPolarity, int Cphase, int DoubleTime);
/*
* Finds the first cluster and location in the root directory of a file
* @param data - A byte of data to be transfered to slave device
* @return SPDR - A byte of data received from slave device
*/
uint8_t SPI_transfer(uint8_t data);
#endif



#if defined(SPI_S)

/*
* Initiates device as slave 
*/
void SPI_SLAVE_Init();
#endif