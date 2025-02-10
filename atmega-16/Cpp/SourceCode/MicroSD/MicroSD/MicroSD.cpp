/***************************************************/
#include "MicroSD.h"
/***************************************************/
MicroSD_Card::MicroSD_Card(void){};
	
	
/***************************************************/
void MicroSD_Card::Command(uint8_t cmd, uint32_t arg, uint8_t crc)
{

	// transmit command to sd card
	spi.transfer(cmd|0x40);
	// transmit argument
	spi.transfer((uint8_t)(arg >> 24));
	spi.transfer((uint8_t)(arg >> 16));
	spi.transfer((uint8_t)(arg >> 8));
	spi.transfer((uint8_t)(arg));
	// transmit crc
	spi.transfer(crc|0x01);
}
/***************************************************/
void MicroSD_Card::powerUpSeq()
{
	// make sure card is deselected
	CS_SD_DE;
	// give SD card time to power up
	_delay_ms(1);
	// send 80 clock cycles to synchronize
	for(uint8_t i = 0; i < 80; i++){
		CS_SD_EN;
	spi.transfer(0xFF);}
	// deselect SD card
	CS_SD_DE;
	spi.transfer(0xFF);
}
/***************************************************/
uint8_t MicroSD_Card::readRes1()
{
	uint8_t i = 0, res1;
	// keep polling until actual data received
	while((res1 = spi.transfer(0xFF)) == 0xFF) // while SPDR is 0xFF until Received data & update res1 with SPDR
	{
		i++;
		// if no data received for 8 bytes, break
		if(i > 8){res1= 128;break;}
	}
	//USART_Int_StrBIT(res1,0);
	return res1;
}
/***************************************************/
uint8_t MicroSD_Card::goIdleState()
{
	// assert chip select
	spi.transfer(0xFF);
	CS_SD_EN;
	spi.transfer(0xFF);
	// send CMD0
	Command(0, 0, 0x94);
	// read response
	uint8_t res1 = readRes1();
	// deassert chip select
	spi.transfer(0xFF);
	CS_SD_DE;
	spi.transfer(0xFF);
	//USART_Int_StrHEX(res1,0);
	return res1;
}
/***************************************************/
void MicroSD_Card::readRes7(uint8_t *res)
{
	// read response 1 in R7
	res[0] = readRes1();

	// if error reading R1, return
	if(res[0] > 1) return;

	// read remaining bytes
	res[1] = spi.transfer(0xFF);
	res[2] = spi.transfer(0xFF);
	res[3] = spi.transfer(0xFF);
	res[4] = spi.transfer(0xFF);
	//USART_Int_StrBIT(res,0);
}
/***************************************************/
void MicroSD_Card::sendIfCond(uint8_t *res)//send interface condition;
{
	// assert chip select
	spi.transfer(0xFF);
	CS_SD_EN;
	spi.transfer(0xFF);

	// send CMD8
	Command(8, 0x0000001AA, 0x86);

	// read response
	readRes7(res);
	//USART_Int_StrBIT(res,0);
	// deassert chip select
	spi.transfer(0xFF);
	CS_SD_DE;
	spi.transfer(0xFF);
}
/***************************************************/
void MicroSD_Card::readOCR(uint8_t *res)
{
	// assert chip select
	spi.transfer(0xFF);
	CS_SD_EN;
	spi.transfer(0xFF);

	// send CMD58
	Command(58,0, 0);

	// read response
	readRes7(res);

	// deassert chip select
	spi.transfer(0xFF);
	CS_SD_DE;
	spi.transfer(0xFF);
}
/***************************************************/
uint8_t MicroSD_Card::sendApp()
{
	// assert chip select
	spi.transfer(0xFF);
	CS_SD_EN;
	spi.transfer(0xFF);

	// send CMD0
	Command(55, 0, 0);

	// read response
	uint8_t res1 = readRes1();

	// deassert chip select
	spi.transfer(0xFF);
	CS_SD_DE;
	spi.transfer(0xFF);

	return res1;
}
/***************************************************/
uint8_t MicroSD_Card::sendOpCond()
{
	// assert chip select
	spi.transfer(0xFF);
	CS_SD_EN;
	spi.transfer(0xFF);

	// send CMD0
	Command(41, 0x40000000, 0);

	// read response
	uint8_t res1 = readRes1();

	// deassert chip select
	spi.transfer(0xFF);
	CS_SD_DE;
	spi.transfer(0xFF);

	return res1;
}
/***************************************************/
uint8_t MicroSD_Card::Init(){
	uint8_t res[5], cmdAttempts = 0;

	powerUpSeq();

	// command card to idle
	while((res[0] = goIdleState()) != 0x01)
	{
		cmdAttempts++;
		
		if(cmdAttempts > 10)
		return 0;
	}

	// send interface conditions
	sendIfCond(res);

	if(res[0] != 0x01){
		return 1;
	}

	// check echo pattern
	if(res[4] != 0xAA){
		return 2;
	}

	// attempt to initialize card
	cmdAttempts = 0;
	do
	{
		if(cmdAttempts > 100) return 3;

		// send app cmd
		res[0] = sendApp();

		// if no error in response
		if(res[0] < 2)
		{
			res[0] = sendOpCond();
		}

		// wait
		_delay_ms(10);

		cmdAttempts++;
	}
	while(res[0] != 0x0);

	// read OCR
	readOCR(res);

	// check card is ready
	if(!(res[1] & 0x80)) return 4;

	return 5;
}
/*********************************************************/
uint8_t MicroSD_Card::RSB( uint8_t*buf,uint32_t addr)
{
	_delay_ms(3);
	memset(buf,0,DataBuffSize);
	uint8_t res1, read;
	uint16_t readAttempts;
	read =0;
	// set token to none

	// assert chip select
	spi.transfer(0xFF);
	CS_SD_EN;
	spi.transfer(0xFF);

	// send CMD17
	Command(17, addr, 0x00);

	// read R1
	res1 = readRes1();

	// if response received from card
	if(res1 != 0xFF)
	{
		// wait for a response token (timeout = 100ms)
		readAttempts = 0;
		while(++readAttempts != SD_MAX_READ_ATTEMPTS)
		if((read = spi.transfer(0xFF)) != 0xFF) break;

		// if response token is 0xFE
		if(read == 0xFE)
		{
			// read 512 byte block
			for(uint16_t i = 0; i < 512; i++) *buf++ = spi.transfer(0xFF);

			// read 16-bit CRC
			spi.transfer(0xFF);
			spi.transfer(0xFF);
		}

		// set token to card response
	}

	// deassert chip select
	spi.transfer(0xFF);
	CS_SD_DE;
	spi.transfer(0xFF);


	return res1;
}
uint8_t MicroSD_Card::RSB(uint8_t *buf,int addr){
	return RSB(buf,addr);
}

void MicroSD_Card::WSB(uint8_t *buf,uint32_t addr)
{
	_delay_ms(5);
	uint32_t readAttempts;
	uint8_t read;
	uint8_t res[5];

	// assert chip select
	spi.transfer(0xFF);
	CS_SD_EN;
	spi.transfer(0xFF);

	// send CMD24d s
	Command(24, addr, 0x0);

	// read response
	
	res[0] = readRes1();
	
	if(res[0] == 0)
	{
		// send start token
		spi.transfer(0xFE);

		// write buffer to card
		for(uint16_t i = 0; i <= DataBuffSize; i++) spi.transfer(buf[i]);

		// wait for a response (timeout = 250ms)
		readAttempts=0;
		while(++readAttempts != SD_MAX_WRITE_ATTEMPTS)
		if((read = spi.transfer(0xFF)) != 0xFF) { break;}
		
	}
	CS_SD_DE;
}

void MicroSD_Card::WSB(uint8_t *buf,int addr){
	WSB(buf,addr);
}
/***************************************************/




// #define SD_TOKEN_OOR(X)     X & 0b00001000
// #define SD_TOKEN_CECC(X)    X & 0b00000100
// #define SD_TOKEN_CC(X)      X & 0b00000010
// #define SD_TOKEN_ERROR(X)   X & 0b00000001
//
// void SD_printDataErrToken(uint8_t token)
// {
// 	if(SD_TOKEN_OOR(token))
// 	USART_Send("\tData out of range\r\n");
// 	if(SD_TOKEN_CECC(token))
// 	USART_Send("\tCard ECC failed\r\n");
// 	if(SD_TOKEN_CC(token))
// 	USART_Send("\tCC Error\r\n");
// 	if(SD_TOKEN_ERROR(token))
// 	USART_Send("\tError\r\n");
// }

