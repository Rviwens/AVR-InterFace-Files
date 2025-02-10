/********************************/
#ifndef SD_RES_PRINT
#define SD_RES_PRINT
/********************************/
#include <USART.h>
#include "SPI.h"
#include "MicroSD.h"

#define PARAM_ERROR(X)      X & 0b01000000
#define ADDR_ERROR(X)       X & 0b00100000
#define ERASE_SEQ_ERROR(X)  X & 0b00010000
#define CRC_ERROR(X)        X & 0b00001000
#define ILLEGAL_CMD(X)      X & 0b00000100
#define ERASE_RESET(X)      X & 0b00000010
#define IN_IDLE(X)          X & 0b00000001
/********************************/
#define CMD_VER(X)          ((X >> 4) & 0xF0)
#define VOL_ACC(X)          (X & 0x1F)
/********************************/
#define VOLTAGE_ACC_27_33   0b00000001
#define VOLTAGE_ACC_LOW     0b00000010
#define VOLTAGE_ACC_RES1    0b00000100
#define VOLTAGE_ACC_RES2    0b00001000
/********************************/
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
/********************************/
class MicroSD_Debug : public MicroSD_Card{
public:
MicroSD_Debug();
/* On SD initialization will output informational and status though USART
*/ 
void InitRead();
/*
* Starts a instance of USART communication
*/
void usart_Start(long buad);
/*
* Will output though USART a SD response(1)'s information
* @param partionNum - The int var that holds a SD response code (1)
*/
void printR1(uint8_t res);
/*
* Will output though USART a SD response(3)'s information
* @param partionNum - The int array that holds a SD response code (1)
*/
void printR3(uint8_t *res);
/*
* Will output though USART a SD response(7)'s information
* @param partionNum - The int array that holds a SD response code (1)
*/
void printR7(uint8_t*res);
/********************************/
private:
USART usart;
};
#endif



