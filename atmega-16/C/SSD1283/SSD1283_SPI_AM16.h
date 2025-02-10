#define F_CPU 20000000UL// Define CPU frequency here 20MHZ


#define SSD1283_HEIGHT 132
#define SSD1283_WIDTH 132
#define SSD1283_BUFFERSIZE SSD1283_HEIGHT*SSD1283_HEIGHT*(18/8)

#define CS_SSD1283_DE PORTB |=(1<<PB0)
#define CS_SSD1283_EN PORTB &=~(1<<PB0)
#define RSpin PB1

#define SSD1283_R00H_MUX 
#define SSD1283_R0BH_DIV 
#define SSD1283_R0BH_RTN 
#define SSD1283_R17H_VBP
#define SSD1283_R17H_VFP

#define Frame_Fq = (520,000/(2*math.pow(2,SSD1283_R0BH_DIV+1)*(SSD1283_R0BH_RTN+16)*(SSD1283_R00H_MUX+SSD1283_R17H_VBP+SSD1283_R17H_VFP+3)))


#include <stdbool.h>



//Remember to set DDR for pins!!!!


//Prototypes
