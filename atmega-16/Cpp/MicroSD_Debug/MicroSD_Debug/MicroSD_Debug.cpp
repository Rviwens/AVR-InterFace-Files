#include "MicroSD_Debug.h"
/***************************************************/
MicroSD_Debug::MicroSD_Debug(){};
void MicroSD_Debug::InitRead(){
	powerUpSeq();
	uint8_t res[5];
	usart.Send("Sending CMD0: \r\n");
	res[0] = goIdleState();
	usart.Send("Response: \r\n");
	printR1(res[0]);
	usart.Send("res7: \r\n");
	sendIfCond(res);
	usart.Send("Sending CMD8: \r\n");
	printR7(res);
	usart.Send("Sending CMD58: \r\n");
	readOCR(res);
	usart.Send("Response: \r\n");
	printR3(res);
	do
	{
		usart.Send("Sending CMD55: \r\n");
		res[0]=sendApp();
		usart.Send("Response: \r\n");
		printR1(res[0]);
		usart.Send("Sending ACMD41: \r\n");
		res[0]=sendOpCond();
		usart.Send("Response: \r\n");
		printR1(res[0]);

		_delay_ms(100);
	}
	while(res[0]!=0x0);
	usart.Send("Sending CMD58: \r\n");
	readOCR(res);
	usart.Send("Response: \r\n");
	printR3(res);
	
}

/***************************************************/
void MicroSD_Debug::printR1(uint8_t res)
{
	//USART_Send("\r\nRes1 =");
	if(res & 0b10000000)
	{usart.Send("\tError: MSB = 1\r\n"); return; }
	if(res == 0)
	{usart.Send("\tCard Ready\r\n"); return; }
	if(PARAM_ERROR(res))
	usart.Send("\tParameter Error\r\n");
	if(ADDR_ERROR(res))
	usart.Send("\tAddress Error\r\n");
	if(ERASE_SEQ_ERROR(res))
	usart.Send("\tErase Sequence Error\r\n");
	if(CRC_ERROR(res))
	usart.Send("\tCRC Error\r\n");
	if(ILLEGAL_CMD(res))
	usart.Send("\tIllegal Command\r\n");
	if(ERASE_RESET(res))
	usart.Send("\tErase Reset Error\r\n");
	if(IN_IDLE(res))
	usart.Send("\tIn Idle State\r\n");
}
/***************************************************/
void MicroSD_Debug::printR7(uint8_t *res)
{
	printR1(res[0]);

	if(res[0] > 1) return;

	usart.Send("\r\n\t Voltage : ");
	if(VOL_ACC(res[3]) == VOLTAGE_ACC_27_33)
	usart.Send("2.7-3.6V\r\n");
	else if(VOL_ACC(res[3]) == VOLTAGE_ACC_LOW)
	usart.Send("LOW VOLTAGE\r\n");
	else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES1)
	usart.Send("RESERVED\r\n");
	else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES2)
	usart.Send("RESERVED\r\n");
	else
	usart.Send("NOT DEFINED\r\n");
	usart.Send("\tCommand Version: ");
	usart.Int_Str(CMD_VER(res[1]),0);

	usart.Send("\r\n\tEcho: ");
	usart.Int_StrHEX(res[4],0);
	usart.Send("\r\n");
}
/***************************************************/
void MicroSD_Debug::printR3(uint8_t *res)
{
	printR1(res[0]);

	if(res[0] > 1) return;

	usart.Send("\tCard Power Up Status: ");
	if(POWER_UP_STATUS(res[1]))
	{
		usart.Send("READY\r\n");
		usart.Send("\tCCS Status: ");
		if(CCS_VAL(res[1])){usart.Send("1\r\n"); }
		else usart.Send("0\r\n");
	}
	else
	{
		usart.Send("BUSY\r\n");
	}

	usart.Send("\tVDD Window: ");
	if(VDD_2728(res[3])) usart.Send("2.7-2.8, ");
	if(VDD_2829(res[2])) usart.Send("2.8-2.9, ");
	if(VDD_2930(res[2])) usart.Send("2.9-3.0, ");
	if(VDD_3031(res[2])) usart.Send("3.0-3.1, ");
	if(VDD_3132(res[2])) usart.Send("3.1-3.2, ");
	if(VDD_3233(res[2])) usart.Send("3.2-3.3, ");
	if(VDD_3334(res[2])) usart.Send("3.3-3.4, ");
	if(VDD_3435(res[2])) usart.Send("3.4-3.5, ");
	if(VDD_3536(res[2])) usart.Send("3.5-3.6");
	usart.Send("\r\n");
}
