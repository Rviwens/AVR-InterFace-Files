
#include "MicroSD_AM16.h"
// #define SD_SUCCESS  0
// #define SD_ERROR    1



void SD_Command(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	// transmit command to sd card
	SPI_transfer(cmd|0x40);
	// transmit argument
	SPI_transfer((uint8_t)(arg >> 24));
	SPI_transfer((uint8_t)(arg >> 16));
	SPI_transfer((uint8_t)(arg >> 8));
	SPI_transfer((uint8_t)(arg));
	// transmit crc
	SPI_transfer(crc|0x01);
}


void SD_powerUpSeq()
{
	// make sure card is deselected
	CS_SD_DE;
	// give SD card time to power up
	_delay_ms(1);
	// send 80 clock cycles to synchronize
	for(uint8_t i = 0; i < 80; i++){
		CS_SD_EN;
	SPI_transfer(0xFF);}
	// deselect SD card
	CS_SD_DE;
	SPI_transfer(0xFF);
}


uint8_t SD_readRes1()
{
	uint8_t i = 0, res1;
	// keep polling until actual data received
	while((res1 = SPI_transfer(0xFF)) == 0xFF) // while SPDR is 0xFF until Received data & update res1 with SPDR
	{
		i++;
		// if no data received for 8 bytes, break
		if(i > 8){res1= 128;break;}
	}
	//USART_Int_StrBIT(res1,0);
	return res1;
}

uint8_t SD_goIdleState()
{
	// assert chip select
	SPI_transfer(0xFF);
	CS_SD_EN;
	SPI_transfer(0xFF);
	// send CMD0
	SD_Command(0, 0, 0x94);
	// read response
	uint8_t res1 = SD_readRes1();
	// deassert chip select
	SPI_transfer(0xFF);
	CS_SD_DE;
	SPI_transfer(0xFF);
	//USART_Int_StrHEX(res1,0);
	return res1;
}


void SD_readRes7(uint8_t *res)
{
	// read response 1 in R7
	res[0] = SD_readRes1();

	// if error reading R1, return
	if(res[0] > 1) return;

	// read remaining bytes
	res[1] = SPI_transfer(0xFF);
	res[2] = SPI_transfer(0xFF);
	res[3] = SPI_transfer(0xFF);
	res[4] = SPI_transfer(0xFF);
	//USART_Int_StrBIT(res,0);
}

void SD_sendIfCond(uint8_t *res)//send interface condition;
{
	// assert chip select
	SPI_transfer(0xFF);
	CS_SD_EN;
	SPI_transfer(0xFF);

	// send CMD8
	SD_Command(8, 0x0000001AA, 0x86);

	// read response
	SD_readRes7(res);
	//USART_Int_StrBIT(res,0);
	// deassert chip select
	SPI_transfer(0xFF);
	CS_SD_DE;
	SPI_transfer(0xFF);
}


void SD_readOCR(uint8_t *res)
{
	// assert chip select
	SPI_transfer(0xFF);
	CS_SD_EN;
	SPI_transfer(0xFF);

	// send CMD58
	SD_Command(58,0, 0);

	// read response
	SD_readRes7(res);

	// deassert chip select
	SPI_transfer(0xFF);
	CS_SD_DE;
	SPI_transfer(0xFF);
}


uint8_t SD_sendApp()
{
	// assert chip select
	SPI_transfer(0xFF);
	CS_SD_EN;
	SPI_transfer(0xFF);

	// send CMD0
	SD_Command(55, 0, 0);

	// read response
	uint8_t res1 = SD_readRes1();

	// deassert chip select
	SPI_transfer(0xFF);
	CS_SD_DE;
	SPI_transfer(0xFF);

	return res1;
}

uint8_t SD_sendOpCond()
{
	// assert chip select
	SPI_transfer(0xFF);
	CS_SD_EN;
	SPI_transfer(0xFF);

	// send CMD0
	SD_Command(41, 0x40000000, 0);

	// read response
	uint8_t res1 = SD_readRes1();

	// deassert chip select
	SPI_transfer(0xFF);
	CS_SD_DE;
	SPI_transfer(0xFF);

	return res1;
}




#if defined(USARTTX) && defined (SDPrintRes)
void SD_InitRead(){
	SD_powerUpSeq();
	uint8_t res[5];
	USART_Send("Sending CMD0: \r\n");
	res[0] = SD_goIdleState();
	USART_Send("Response: \r\n");
	SD_printR1(res[0]);
	USART_Send("res7: \r\n");
	SD_sendIfCond(res);
	USART_Send("Sending CMD8: \r\n");
	SD_printR7(res);
	USART_Send("Sending CMD58: \r\n");
	SD_readOCR(res);
	USART_Send("Response: \r\n");
	SD_printR3(res);
	do
	{
		USART_Send("Sending CMD55: \r\n");
		res[0]=SD_sendApp();
		USART_Send("Response: \r\n");
		SD_printR1(res[0]);
		USART_Send("Sending ACMD41: \r\n");
		res[0]=SD_sendOpCond();
		USART_Send("Response: \r\n");
		SD_printR1(res[0]);

		_delay_ms(100);
	}
	while(res[0]!=0x0);
	USART_Send("Sending CMD58: \r\n");
	SD_readOCR(res);
	USART_Send("Response: \r\n");
	SD_printR3(res);
	
}
#endif



uint8_t SD_Init(){
	uint8_t res[5], cmdAttempts = 0;

	SD_powerUpSeq();

	// command card to idle
	while((res[0] = SD_goIdleState()) != 0x01)
	{
		cmdAttempts++;
		
		if(cmdAttempts > 10)
		return 0;
	}

	// send interface conditions
	SD_sendIfCond(res);

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
		res[0] = SD_sendApp();

		// if no error in response
		if(res[0] < 2)
		{
			res[0] = SD_sendOpCond();
		}

		// wait
		_delay_ms(10);

		cmdAttempts++;
	}
	while(res[0] != 0x0);

	// read OCR
	SD_readOCR(res);




	// check card is ready
	if(!(res[1] & 0x80)) return 4;

	return 5;
}




#if defined(SDRSB)

uint8_t SD_RSB( uint8_t*buf,uint32_t addr)
{
	_delay_ms(3);
	memset(buf,0,DataBuffSize);
	uint8_t res1, read;
	uint16_t readAttempts;
	read =0;
	// set token to none

	// assert chip select
	SPI_transfer(0xFF);
	CS_SD_EN;
	SPI_transfer(0xFF);

	// send CMD17
	SD_Command(17, addr, 0x00);

	// read R1
	res1 = SD_readRes1();

	// if response received from card
	if(res1 != 0xFF)
	{
		// wait for a response token (timeout = 100ms)
		readAttempts = 0;
		while(++readAttempts != SD_MAX_READ_ATTEMPTS)
		if((read = SPI_transfer(0xFF)) != 0xFF) break;

		// if response token is 0xFE
		if(read == 0xFE)
		{
			// read 512 byte block
			for(uint16_t i = 0; i < 512; i++) *buf++ = SPI_transfer(0xFF);

			// read 16-bit CRC
			SPI_transfer(0xFF);
			SPI_transfer(0xFF);
		}

		// set token to card response
	}

	// deassert chip select
	SPI_transfer(0xFF);
	CS_SD_DE;
	SPI_transfer(0xFF);


	return res1;
}



#endif







#if defined(SDWSB)
void SD_WSB(uint8_t *buf,uint32_t addr)
{
	_delay_ms(5);
	uint32_t readAttempts;
	uint8_t read;
	uint8_t res[5];

	// assert chip select
	SPI_transfer(0xFF);
	CS_SD_EN;
	SPI_transfer(0xFF);

	// send CMD24d s
	SD_Command(24, addr, 0x0);

	// read response
	
	res[0] = SD_readRes1();
	
	if(res[0] == SD_READY)
	{
		// send start token
		SPI_transfer(0xFE);

		// write buffer to card
		for(uint16_t i = 0; i <= DataBuffSize; i++) SPI_transfer(buf[i]);

		// wait for a response (timeout = 250ms)
		readAttempts=0;
		while(++readAttempts != SD_MAX_WRITE_ATTEMPTS)
		if((read = SPI_transfer(0xFF)) != 0xFF) { break;}
		
	}
	CS_SD_DE;
}
#endif




