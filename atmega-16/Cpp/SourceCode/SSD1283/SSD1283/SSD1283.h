
#ifndef SSD1283_H_
#define SSD1283_H_

#include <math.h>
#include <SPI.h>
#include <USART.h>
#include <MicroSD.h>
#include <FAT32.h>
#include <SourceCode/Shape/Shape/Shape.h>
#define SSD1283_HEIGHT 132
#define SSD1283_WIDTH 132

#ifndef BufferSize
#define BufferSize 512
#endif

#define _R01H_MUX 131
#define _R0BH_DIV 0
#define _R0BH_RTN 12
#define _R17H_VBP 2
#define _R17H_VFP 4
#define Frame_Fq  (520000/(2*pow(2,_R0BH_DIV+1)*(_R0BH_RTN+16)*(_R01H_MUX+_R17H_VBP+_R17H_VFP+3)))

//POR values for each register
#define _R00H 0x1
#define _R01H 0x2183
#define _R02H 0x0300
#define _R03H 0x6830
#define _R07H 0x0021
#define _R0BH 0x5c0c
#define _R10H 0x2F84
#define _R11H 0x001c
#define _R12H 0x0609
#define _R13H 0x3100
#define _R16H 0x8302
#define _R17H 0x0402
#define _R1EH 0x0036
#define _R1FH 0x0036
#define _R42H 0x8300
#define _R43H 0x8300



class SSD1283 {
public:

SSD1283(void);
SSD1283(uint8_t cs, uint8_t cd,uint8_t rot);

void SetPins(uint8_t cs, uint8_t cd);

void Command(uint8_t cmd);
void Data(uint16_t data);
void Data(uint16_t data, uint16_t n);
void Cmd_Data(uint8_t cmd,uint16_t data);

void set_cs(uint8_t pin);
void set_cd(uint8_t pin);
void setRotation(uint8_t rot);

uint16_t Driver_Output_Control(bool rev,bool CAD, bool BGR, bool SM, bool TB, bool RL);
uint16_t Driver_Output_Control(uint16_t cmd);

uint16_t Waveform_Control(bool FLD,bool BC, bool EOR,uint8_t nlines);
uint16_t Waveform_Control(uint16_t cmd);

uint16_t Entry_Mode(bool VSMode,bool DFM0, bool TRANS, bool OEDef, bool Dmode1,bool ID1, bool ID0, bool AM);
uint16_t Entry_Mode(uint16_t data);

uint16_t Display_Control(bool VLE2,bool VLE1,bool GON, bool DTE,bool D1,bool D0);
uint16_t Display_Control(uint16_t data);

uint16_t Frame_Cycle_Control(bool EQ1,bool EQ0,bool SDIV, bool SRTN );
uint16_t Frame_Cycle_Control(uint16_t data);

uint16_t HorizontalPorch(uint8_t XL,uint8_t HBP);
uint16_t HorizontalPorch(uint16_t data);

uint16_t VerticalPorch();
uint16_t VerticalPorch(uint16_t data);

uint16_t PowerControl_1(bool DOT,uint8_t DCY,bool DC1, bool DC0,bool SLP);
uint16_t PowerControl_1(uint16_t cmd);

uint16_t PowerControl_2(bool PU1, bool PU0);
uint16_t PowerControl_2(uint16_t cmd);

uint16_t PowerControl_3(bool SX263B, bool V63SH,uint8_t VRH);
uint16_t PowerControl_3(uint16_t cmd);

uint16_t PowerControl_4(bool VCOMG, uint8_t VDV);
uint16_t PowerControl_4(uint16_t cmd);

uint16_t PowerControl_5(bool nOTP,uint8_t VCM);
uint16_t PowerControl_5(uint16_t cmd);

uint16_t PowerControl_6(uint16_t cmd);

uint16_t GateScan_Pos(uint8_t cmd);

uint16_t Set_RAM_Address(uint16_t data);

uint16_t Vertical_Scroll(uint16_t data);

uint16_t Screen_Driving_Pos1(uint8_t SSpos, uint8_t SEpos);
uint16_t Screen_Driving_Pos1(uint16_t data);

uint16_t Screen_Driving_Pos2(uint8_t SSpos, uint8_t SEpos);
uint16_t Screen_Driving_Pos2(uint16_t data);

void setWindowAddress(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint8_t rot);

void Color_Pixel(uint8_t x, uint8_t y, uint16_t color);

void Init();

void Fill_Screen(uint16_t color);
bool Draw_Circle(uint8_t x,uint8_t y,uint8_t radius,uint16_t color, bool fill);
bool Draw_Rect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color,bool fill);

void Start();void Clr_Screen();uint16_t color_8bit(uint8_t r, uint8_t g, uint8_t b);
bool Draw(Shape shp,bool fill);

void Display();

FAT32_File screen;

protected:
SPI spi;
USART usart;

private:	
bool fillRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void startTransfer();
void endTransfer();
void CD_EN();
void CD_DE();
void GenerateFile();
uint8_t _cd,_cs,_rot;
MicroSD_Card card;

};

#endif /* SSD1283_H7_ */
