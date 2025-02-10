#ifndef I2C_COM
#define I2C_COM

#ifndef F_CPU
#define F_CPU 20000000UL// Define CPU frequency here 20MHZ
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <string.h>

//SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
//Define bit rate 
#define SCL_CLK 400000
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))

class I2C{
public:

I2C();
//Prototypes
 /*
 * initializes I2C 
 */
 void Init();
 /*
 * Sends A I2C start command to a slave device
 * @param write_address - The I2C write address of the slave device 
 * @return 0-   start condition fail
 * @return 1-  ack received 
 * @return 2-  nack received 
 * @return 3 - SLA+W failed
 */
 uint8_t Start(char write_address);
  /*
 * Sends A I2C start command to a slave device with no returns
 * @param write_address - The I2C write address of the slave device 
 */
 void Start_NORETURN(uint8_t write_address);
  /*
 * Sends A I2C repeated start command to a slave device
 * @param read_address - The I2C read address of the slave device 
 * @return 0-   start condition fail
 * @return 1-  ack received 
 * @return 2-  nack received 
 * @return 3 - SLA+W failed
 */
uint8_t Repeated_Start(uint8_t read_address);
   /*
 * Stops I2C 
 */
 void Stop();
  /*
 * Sends a I2C start command to a slave device and waits for a response 
 * @param write_address - The I2C write address of the slave device 
 */
 void Start_Wait(char write_address);
  /*
 * Writes a byte of data to the slave device
 * @param char - The byte of data to be sent
 * @return 0-  ack received
 * @return 1-  nack received 
 * @return 2-  data transmission failed
 */
 uint8_t Write(char data);
 /*
 * Initializes the device as a I2C slave
 * @param slave_address - The I2C address of this device
 */
 void Slave_Init(uint8_t slave_address);
  /*
 * Waits to be addressed by the Master
 * @return 0 - 
 * @return 1 - 
 * @return 2 - 
 */
 uint8_t Slave_Listen();
  /*
 * Transmits data from the slave to the master 
 * @param char - The byte of data to be sent
 * @return 0-  Data transmitted & ack received
 * @return 1-  STOP/REPEATED START received
 * @return 2-  Data transmitted & nack received
*  @return 3-  Last data byte transmitted with ack received TWEA = 0
*  @return 4-  Else return 4
 */ 
 uint8_t Slave_Transmit(char data);
  /*
 * Receives data from the master device
 * @return TWDR-  received data
 * @return 1-  STOP/REPEATED START
 * @return 2-  Else return 2
 */
 uint8_t Slave_Receive();
  /*
 * 
 * @param WriteBuffer -Pointer to a buffer that will store data to write to the master device
 * @param ReadBuffer - Pointer to a buffer that will store data has been received from the master device to be read
 * @return 0- 
 * @return 1-  
 * @return 2-  
 */
 uint8_t Event(char*WriteBuffer,char* ReadBuffer);
 /*
 * Sends a start command and then a repeated start command
 * @param address - address of slave device
 */
 void SRS(uint8_t address);
 /*
 * Sends a start and then a repeated start, then reads data from the slave device and sends an acknowledgment
 * @param address - address of slave device
 * @param str - Pointer to the buffer that will store the received data
 */
 void Read(uint8_t address, char *str );
 /*
 * Sends a start and then a repeated start, then reads data from the slave device and dose not send an acknowledgment
 * @param address - address of slave device
 * @param str - Pointer to the buffer that will store the received data
  *@param NumOfBytes - The number of bytes expected to be received 
 */
 void ReadWNES(uint8_t address, char *str, uint8_t NumOfBytes);
 /*
 * Sends a String to the slave device 
 * @param string - Pointer to the string being sent
 * @param slave_address - The I2C address of this device
 */
 void Str(char*string,int Address);
 /*
 * Sends a Interger to the slave device 
 * @param int - Integer being sent
 * @param slave_address - The I2C address of this device
 */
 void SI(int num, int address);
 private:
 /*
 * Reads a byte of data from the slave device and sends an acknowledgment
 * @return TWDR-  received data
 */
 uint8_t Read_Ack();
 /*
 * Reads a byte of data from the slave device and dose not send an acknowledgment
 * @return TWDR-  received data
 */
 uint8_t Read_Nack();
 };
 #endif