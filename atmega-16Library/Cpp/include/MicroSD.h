/****************************************************************************/
#ifndef MICROSD
#define MICROSD

#ifndef F_CPU
#define F_CPU 20000000UL
#endif
/****************************************************************************/


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include <SPI.h>
/****************************************************************************/

/****************************************************************************/
#define CS_SD_DE PORTA |=(1<<0);
#define CS_SD_EN PORTA &=~(1<<0);
#define DataBuffSize 511
/****************************************************************************/
#define SD_MAX_READ_ATTEMPTS    62500
#define SD_MAX_WRITE_ATTEMPTS    62500
/****************************************************************************/
//Remember to set DDR for pins!!!!


/****************************************************************************/
class MicroSD_Card{
//Members
public:

MicroSD_Card();


/*
* Initializes the SD card
* @return 0 - Card failed to go idle
* @return 1 - Failed to read interface conditions
* @return 2 - Failed verify Echo
* @return 3 - Failed to initialize card
* @return 4 - Card is not ready, Unknown error
* @return 5 - No error, Card was initialized successfully
*/
uint8_t Init();
/*
* Read an address on the SD card
* @param buf - Pointer to the buffer storing the Data returned from the address
* @param addr- Address to read
* @return res1 - Returns an SD response code 1
*/
uint8_t RSB( uint8_t*buf,uint32_t addr);
/*
* Writes to an address on the SD card
* @param buf - Pointer to the buffer storing the Data returned from the address
* @param addr- Address to read
*/
void WSB(uint8_t *buf,uint32_t addr);
private:
SPI spi;
/*
* Sends a command to the SD card
* @param cmd - The SD command type
* @param arg - The arguments for the command
* @param arg - The crc code for the command
*/
void Command(uint8_t cmd, uint32_t arg, uint8_t crc);
protected:
/*
* Sends a command to power up the SD card
*/
void powerUpSeq();
/*
* Reads response code 1 from the SD card
* @return res1 - Returns the 8-bit response(1)
*/
uint8_t readRes1();
/*
* Resets the SD Memory Card
* @return res1 - Returns the 8-bit response(1)
*/
uint8_t goIdleState();
/*
* Reads response code 7 from the SD card
* @param res - A pointer to a array to store the 5 byte response
*/
void readRes7(uint8_t *res);
/*
* Sends SD Memory Card interface condition that includes Host Supply Voltage (VHS) information and asks the accessed card whether card can operate in supplied voltage range.
* @param res - The array that will store a response code (7)
*/
void sendIfCond(uint8_t *res);
/*
* Reads the SD card's ORC
* @param res - The array that will store a response code (7)
*/
void readOCR(uint8_t *res);
/*
* Defines to the card that the next command is an application specific command rather than a standard command.
* @return res1 - Returns the 8-bit response(3)
*/
uint8_t sendApp();
/*
* Sends host capacity support information and activates the card's initialization process.
* @return res1 - Returns the 8-bit response(1)
*/
uint8_t sendOpCond();
/****************************************************************************/
};
/****************************************************************************/
#endif