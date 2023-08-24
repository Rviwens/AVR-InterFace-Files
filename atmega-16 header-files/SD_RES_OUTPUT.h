#define PARAM_ERROR(X)      X & 0b01000000
#define ADDR_ERROR(X)       X & 0b00100000
#define ERASE_SEQ_ERROR(X)  X & 0b00010000
#define CRC_ERROR(X)        X & 0b00001000
#define ILLEGAL_CMD(X)      X & 0b00000100
#define ERASE_RESET(X)      X & 0b00000010
#define IN_IDLE(X)          X & 0b00000001

void SD_printR1(uint8_t res)
{
	//USART_Send("\r\nRes1 =");
	if(res & 0b10000000)
	{USART_Send("\tError: MSB = 1\r\n"); return; }
	if(res == 0)
	{ USART_Send("\tCard Ready\r\n"); return; }
	if(PARAM_ERROR(res))
	USART_Send("\tParameter Error\r\n");
	if(ADDR_ERROR(res))
	USART_Send("\tAddress Error\r\n");
	if(ERASE_SEQ_ERROR(res))
	USART_Send("\tErase Sequence Error\r\n");
	if(CRC_ERROR(res))
	USART_Send("\tCRC Error\r\n");
	if(ILLEGAL_CMD(res))
	USART_Send("\tIllegal Command\r\n");
	if(ERASE_RESET(res))
	USART_Send("\tErase Reset Error\r\n");
	if(IN_IDLE(res))
	USART_Send("\tIn Idle State\r\n");
}



#define CMD_VER(X)          ((X >> 4) & 0xF0)
#define VOL_ACC(X)          (X & 0x1F)

#define VOLTAGE_ACC_27_33   0b00000001
#define VOLTAGE_ACC_LOW     0b00000010
#define VOLTAGE_ACC_RES1    0b00000100
#define VOLTAGE_ACC_RES2    0b00001000

void SD_printR7(uint8_t *res)
{
	SD_printR1(res[0]);

	if(res[0] > 1) return;

   //  USART_Send("Hello");

	USART_Send("\r\n\t Voltage : ");
	if(VOL_ACC(res[3]) == VOLTAGE_ACC_27_33)
	USART_Send("2.7-3.6V\r\n");
	else if(VOL_ACC(res[3]) == VOLTAGE_ACC_LOW)
	USART_Send("LOW VOLTAGE\r\n");
	else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES1)
	USART_Send("RESERVED\r\n");
	else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES2)
	USART_Send("RESERVED\r\n");
	else
	USART_Send("NOT DEFINED\r\n");
	USART_Send("\tCommand Version: ");
	USART_Int_Str(CMD_VER(res[1]),0);

	USART_Send("\r\n\tEcho: ");
	USART_Int_StrHEX(res[4],0);
	USART_Send("\r\n");
}


#define POWER_UP_STATUS(X)  X & 0x40
#define CCS_VAL(X)          X & 0x40
#define VDD_2728(X)         X & 0b10000000
#define VDD_2829(X)         X & 0b00000001
#define VDD_2930(X)         X & 0b00000010
#define VDD_3031(X)         X & 0b00000100
#define VDD_3132(X)         X & 0b00001000
#define VDD_3233(X)         X & 0b00010000
#define VDD_3334(X)         X & 0b00100000
#define VDD_3435(X)         X & 0b01000000
#define VDD_3536(X)         X & 0b10000000

void SD_printR3(uint8_t *res)
{
	SD_printR1(res[0]);

	if(res[0] > 1) return;

	USART_Send("\tCard Power Up Status: ");
	if(POWER_UP_STATUS(res[1]))
	{
		USART_Send("READY\r\n");
		USART_Send("\tCCS Status: ");
		if(CCS_VAL(res[1])){ USART_Send("1\r\n"); }
		else USART_Send("0\r\n");
	}
	else
	{
		USART_Send("BUSY\r\n");
	}

	USART_Send("\tVDD Window: ");
	if(VDD_2728(res[3])) USART_Send("2.7-2.8, ");
	if(VDD_2829(res[2])) USART_Send("2.8-2.9, ");
	if(VDD_2930(res[2])) USART_Send("2.9-3.0, ");
	if(VDD_3031(res[2])) USART_Send("3.0-3.1, ");
	if(VDD_3132(res[2])) USART_Send("3.1-3.2, ");
	if(VDD_3233(res[2])) USART_Send("3.2-3.3, ");
	if(VDD_3334(res[2])) USART_Send("3.3-3.4, ");
	if(VDD_3435(res[2])) USART_Send("3.4-3.5, ");
	if(VDD_3536(res[2])) USART_Send("3.5-3.6");
	USART_Send("\r\n");
	
	
}

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