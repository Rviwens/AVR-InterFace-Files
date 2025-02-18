
#ifndef SSD1306_DRIVER
#define SSD1306_DRIVER

#ifndef F_CPU
#define F_CPU 20000000UL// Define CPU frequency here 20MHZ
#endif

#include <I2C.h>

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 32
#define SSD1306_BUFFERSIZE (SSD1306_WIDTH*SSD1306_HEIGHT)/8

#define SSD1306_DEFAULT_ADDRESS 0x78
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_NOP 0xE3
#define SSD1306_Write_Address 0x3C




class SSD1306 {
	public:

	SSD1306();
	SSD1306(uint8_t Invert, uint8_t contrast);

	void Invert(uint8_t Mode);
	void Contrast(uint8_t data);
	void Init(uint8_t Invert, uint8_t contrast);
	void SetCursor(uint8_t myLineNumber,uint8_t myColumnPosition);
	void Clear();
	void Fill();
	void Display(uint8_t *ScreenBuffer);
	void XY_pixel(uint8_t x, uint8_t y, uint8_t *ScreenBuffer, uint8_t PixleStatus);
	void VLine(uint8_t x, uint8_t y, uint8_t lenght, uint8_t*SB);
	void HLine(uint8_t x, uint8_t y, uint8_t lenght, uint8_t*SB);
	void Rect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t*SB);
	void Bitmap(uint8_t*IA, uint8_t*SB);

	private:
	I2C i2c;
	uint8_t lineNumber;
	uint8_t cursorPos;
	void Byte_READ(uint8_t x, uint8_t y, uint8_t *ScreenBuffer);
	void Byte( uint8_t x, uint8_t y,uint8_t *SB, uint8_t data);
};




#endif