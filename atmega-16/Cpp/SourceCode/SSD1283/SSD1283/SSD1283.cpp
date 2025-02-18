/*
 * SSD12832.cpp
 *
 * Created: 2/14/2025 5:08:00 PM
 * Author : Operator
 */ 

#include "SSD1283.h"
/***********************************************************************/
SSD1283::SSD1283(){
	screen.setFname((char*)"scrn");
	screen.setType((char*)"txt");

usart.Send("\r\n Status = ");
int status = card.Init();
usart.Int_Str(status,0);

	//GenerateFile();
};
	
SSD1283::SSD1283(uint8_t cs, uint8_t cd,uint8_t rot)
{
	screen.setFname((char*)"scrn");
	screen.setType((char*)"txt");
	
	this->_cs = cs;
	this->_cd = cd;
	this->_rot=rot;
	DDRB|=(1<<cd)|(1<<cs)|(1<<5)|(1<<7);
	PORTB|=(1<<cd)|(1<<cs);

usart.Send("\r\n Status = ");
int status = card.Init();
usart.Int_Str(status,0);

	//GenerateFile();
}

void SSD1283::SetPins(uint8_t cs, uint8_t cd){
	this->_cs = cs;
	this->_cd = cd;
	DDRB|=(1<<cd)|(1<<cs)|(1<<5)|(1<<7);
	PORTB|=(1<<cd)|(1<<cs);	
}

void SSD1283::GenerateFile(){
	if (screen.Open()==false)
	{
		screen.Create(0);
	for(char clust =0; clust<=5;clust++)
		screen.Append();
	}
}

/***********************************************************************/
void SSD1283::startTransfer(){  PORTB &=~(1<<_cs);  spi.MAST_Init(0,4,0,0,0);}
void SSD1283::endTransfer(){ PORTB|=(1<<_cs); spi.End();}

void SSD1283::CD_EN(){PORTB |=(1<<_cd);}
void SSD1283::CD_DE(){PORTB &=~(1<<_cd);}

void SSD1283::set_cs(uint8_t pin){
	_cs=pin;
}
void SSD1283::set_cd(uint8_t pin){
	_cd=pin;
}


/***********************************************************************/
void SSD1283::setRotation(uint8_t rot){
	this->_rot=_rot;
}
/***********************************************************************/
void SSD1283::Command(uint8_t cmd){
	startTransfer();
	CD_DE();
	spi.transfer(cmd&0xFF);
	endTransfer();
}
/***********************************************************************/
void SSD1283::Data(uint16_t data){
	startTransfer();
	CD_EN();
	spi.transfer(data>>8);
	spi.transfer(data);
	endTransfer();
}
void SSD1283::Data(uint16_t data, uint16_t n)
{
	startTransfer();
	CD_EN();
	while (n-- > 0)
	{
		spi.transfer(data >> 8);
		spi.transfer(data);
	}
	endTransfer();
}
/***********************************************************************/
void SSD1283::Cmd_Data(uint8_t cmd,uint16_t data){
	startTransfer();
	CD_DE();
	spi.transfer(cmd);
	CD_EN();
	spi.transfer(data>>8);
	spi.transfer(data);
	endTransfer();
}
/***********************************************************************/
uint16_t SSD1283::Driver_Output_Control(bool rev,bool CAD, bool BGR, bool SM, bool TB, bool RL){
	uint16_t value =  (rev<<13)+(CAD<<12)+(BGR<<11)+(SM<<10)+(TB<<9)+(RL<<8)+_R01H_MUX;
	Cmd_Data(0x01, value);
	return value;
}
uint16_t SSD1283::Driver_Output_Control(uint16_t cmd){
	Cmd_Data(0x01, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::Waveform_Control(bool FLD,bool BC, bool EOR,uint8_t nlines){
	uint16_t value =(FLD<<11)+(BC<<9)+(EOR<<8)+nlines;
	Cmd_Data(0x02,value);
	return value;
}
uint16_t SSD1283::Waveform_Control(uint16_t cmd){
	Cmd_Data(0x02, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::Entry_Mode(bool VSMode,bool DFM0, bool TRANS, bool OEDef, bool Dmode1,bool ID1, bool ID0, bool AM){
	uint16_t value = (VSMode<<15)+(1<<14)+(DFM0<<13)+(TRANS<<12)+(OEDef<<11)+(Dmode1<<9)+(ID1<<5)+(ID0<<4)+(AM<<3);
	Cmd_Data(0x03,value);
	return value;
}
uint16_t  SSD1283::Entry_Mode(uint16_t cmd){
	Cmd_Data(0x03, cmd);
	return cmd;
}
/*********************************************************************/
uint16_t SSD1283::Display_Control(bool VLE2,bool VLE1,bool GON, bool DTE,bool D1,bool D0){
	uint16_t value = ((VLE2<<10)+(VLE1<<9)+(GON<<5)+(DTE<<4)+(D1<<1)+(D0));
	Cmd_Data(0x07,value);
	return value;
}
uint16_t SSD1283::Display_Control(uint16_t cmd){
	Cmd_Data(0x07, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::Frame_Cycle_Control(bool EQ1,bool EQ0,bool SDIV, bool SRTN ){
	uint16_t value= ((EQ1<<11)+(EQ0<<10)+(_R0BH_DIV<<8)+(SDIV<<5)+(SRTN<<4)+_R0BH_RTN);
	Cmd_Data(0x0B, value);
	return value;
}
uint16_t SSD1283::Frame_Cycle_Control(uint16_t cmd){
	Cmd_Data(0x0B, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::HorizontalPorch(uint8_t XL,uint8_t HBP){
	uint16_t value= (XL<<8)+(HBP);
	Cmd_Data(0x16, value);
	return value;
}

uint16_t SSD1283::HorizontalPorch(uint16_t cmd){
	Cmd_Data(0x16, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::VerticalPorch(){
	uint16_t value = (_R17H_VFP<<8)+(_R17H_VBP);
	Cmd_Data(0x17, value);
	return value;
}
uint16_t SSD1283::VerticalPorch(uint16_t cmd){
	Cmd_Data(0x17, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::PowerControl_1(bool DOT,uint8_t DCY,bool DC1, bool DC0,bool SLP){
	uint16_t value = (DOT<<15)+(DCY<<12)+(1<<8)+(1<<7)+(1<<6)+(DC1<<5)+(DC0<<4)+(1<<2)+(SLP);
	Cmd_Data(0x10, value);
	return value;
}
uint16_t SSD1283::PowerControl_1(uint16_t cmd){
	cmd|= (1<<8)+(1<<7)+(1<<6);
	Cmd_Data(0x10, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::PowerControl_2(bool PU1, bool PU0){
	uint16_t value = (PU1<<4)+(PU0<<3)+(1<<2);
	Cmd_Data(0x11, value);
	return value;
}
uint16_t SSD1283::PowerControl_2(uint16_t cmd){
	cmd |= (1<<2);
	Cmd_Data(0x11, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::PowerControl_3(bool SX263B, bool V63SH,uint8_t VRH){
	uint16_t value = (1<<10)+(1<<9)+(SX263B<<8)+(V63SH<<7)+(VRH);
	Cmd_Data(0x12, value);
	return value;
}
uint16_t SSD1283::PowerControl_3(uint16_t cmd){
	cmd|=(1<<10)+(1<<9);
	Cmd_Data(0x12, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::PowerControl_4(bool VCOMG, uint8_t VDV){
	uint16_t value = (VCOMG<<13)+(VDV<<8);
	Cmd_Data(0x13, value);
	return value;
}
uint16_t SSD1283::PowerControl_4(uint16_t cmd){
	Cmd_Data(0x13, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::PowerControl_5(bool nOTP,uint8_t VCM){
	uint16_t value = (nOTP<<7)+(VCM);
	Cmd_Data(0x1E, value);
	return value;
}
uint16_t SSD1283::PowerControl_5(uint16_t cmd){
	Cmd_Data(0x1E, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::PowerControl_6(uint16_t cmd){
	Cmd_Data(0x1F, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::GateScan_Pos(uint8_t cmd){
	Cmd_Data(0x40, cmd);
	return cmd;
}
/***********************************************************************/
uint16_t SSD1283::Set_RAM_Address(uint16_t data){
	Cmd_Data(0x21, data);
	return data;
}
/***********************************************************************/
void SSD1283::setWindowAddress(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2,uint8_t rot)
{
	x1+=2;
	y1+=2;
	switch (rot)
	{
		case 0:
		Command(0x44);
		Data(((x2)<<8)+(x1));
		Command(0x45);
		Data( ((y2)<<8)+(y1));
		Command(0x21);
		Data(((y1)<<8)+(x1));
		break;
		case 1:
		Command(0x44); //Horizontal RAM address position
		Data(((SSD1283_HEIGHT - y1)<<8)+((SSD1283_HEIGHT - y2)));
		Command(0x45); //Vertical RAM address position
		Data(((SSD1283_WIDTH - x1)<<8)+((SSD1283_WIDTH - x2)));
		Command(0x21); //RAM address set
		Data(((SSD1283_WIDTH - x1)<<8)+(SSD1283_HEIGHT - y1));
		break;
		case 2:
		Command(0x44);
		Data(((SSD1283_WIDTH - x1)<<8)+((SSD1283_WIDTH - x2)));
		Command(0x45);
		Data(((SSD1283_HEIGHT - y1)<<8)+((SSD1283_HEIGHT - y2)));
		Command(0x21);
		Data(((SSD1283_WIDTH - y1)<<8)+(SSD1283_HEIGHT - x1));
		break;
		case 3:
		Command(0x44);
		Data(((y2)<<8)+(y1));
		Command(0x45);
		Data((x2<<8)+x1);
		Command(0x21);
		Data((x1<<8)+(y1));
		break;
	}
}
/***********************************************************************/
uint16_t SSD1283::Vertical_Scroll(uint16_t data){
	Cmd_Data(0x41, data);
	return data;
}
/***********************************************************************/
uint16_t SSD1283::Screen_Driving_Pos1(uint8_t SSpos, uint8_t SEpos){
	uint16_t value = (SEpos<<8)+(SSpos);
	Cmd_Data(0x42, value);
	return value;
}
uint16_t SSD1283::Screen_Driving_Pos1(uint16_t data){
	Cmd_Data(0x42, data);
	return data;
}
/***********************************************************************/
uint16_t SSD1283::Screen_Driving_Pos2(uint8_t SSpos, uint8_t SEpos){
	uint16_t value = (SEpos<<8)+(SSpos);
	Cmd_Data(0x43, value);
	return value;
}
uint16_t SSD1283::Screen_Driving_Pos2(uint16_t data){
	Cmd_Data(0x43, data);
	return data;
}
/***********************************************************************/
void SSD1283::Init(){
	DDRB|=(1<<_cd)|(1<<_cs)|(1<<5)|(1<<7);
	PORTB|=(1<<_cd)|(1<<_cs);
	_delay_ms(200);
	Cmd_Data(0x10,_R10H);
	Cmd_Data(0x11, _R11H);
	Cmd_Data(0x07, _R07H);

	Cmd_Data(0x28, 0x0006);
	Cmd_Data(0x28, 0x0005);
	Cmd_Data(0x27, 0x057F);
	Cmd_Data(0x29, 0x89A1);

	Cmd_Data(0x00, _R00H);

	_delay_ms(100);
	Cmd_Data(0x29, 0x80B0);

	_delay_ms(30);
	Cmd_Data(0x29, 0xFFFE);
	//_R07H + vertical Scroll
	Cmd_Data(0x07,  0x233);

	_delay_ms(30);
	Cmd_Data(0x07,  0x233);

	Cmd_Data(0x01, _R01H);
	Cmd_Data(0x03, _R03H);

	Cmd_Data(0x2F, 0xFFFF);
	Cmd_Data(0x2C, 0x8000);
	Cmd_Data(0x27, 0x0578);

	Cmd_Data(0x02, _R02H);
	Cmd_Data(0x0B,  _R0BH);
	Cmd_Data(0x12, _R12H);
	Cmd_Data(0x13, _R13H);
}
/***********************************************************************/
bool SSD1283::fillRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{

	if((x1<0)||(x2<0)||(y1<0)||(y2<0)){
		return false;
	}
	if((x1>SSD1283_WIDTH)||(x2>SSD1283_WIDTH)||(y1>SSD1283_HEIGHT)||(y2>SSD1283_WIDTH)){
		return false;
	}
	
	startTransfer();
	setWindowAddress(x1, y1,x2, y2,this->_rot);
	Command(0x22);
	Data(color, (y2-y1)*(x2-x1)-1);
	endTransfer();
	return	true;
}


void SSD1283::Fill_Screen(uint16_t color){
	setWindowAddress(0,0,SSD1283_WIDTH,SSD1283_HEIGHT,this->_rot);
	Command(0x22);
	Data(color,SSD1283_HEIGHT*SSD1283_WIDTH+2);
}

void SSD1283::Clr_Screen(){
	Fill_Screen(0x0);
}

uint16_t SSD1283::color_8bit(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
/***********************************************************************/
void SSD1283::Color_Pixel(uint8_t x, uint8_t y, uint16_t color){
	setWindowAddress(x,y,x,y,_rot);
	Command(0x22);
	Data(color);
}

/************************************************************************/
void SSD1283::Display(){
uint16_t color;

for(char clus =0; clus < 5;clus++)
	for(char sec =0; sec < 33;sec++){
		screen.RSIC(clus,sec);
	}

	
}



/********************************************************/
bool SSD1283::Draw_Circle(uint8_t x,uint8_t y,uint8_t radius,uint16_t color, bool fill){
	if((x<(0-radius))||(y<(0-radius))||(x>SSD1283_HEIGHT)||(y>SSD1283_HEIGHT)) return false;
	
	for (int l = y-radius; l<=y + radius;l++)
		for (int i = x-radius; i<=x + radius;i++){
			 double d = sqrt(pow(abs(i-x),2)+pow(abs(l-y),2));
			if(fill==0){
				if( (radius - 0.5) <= d && d <= (radius+0.5))
					Color_Pixel(i,l,color);
			}else if(fill==1)
				if(d < radius+0.5)
					Color_Pixel(i,l,color);
		}
	return true;
}

bool SSD1283::Draw_Rect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color,bool fill){
if(x1>x2||y1>y2)return false;

if(fill==1)
fillRect(x1,y1,x2,y2,color);
else if(fill==0){
	for (uint8_t i = x1; i<=x2;i++){
		Color_Pixel(i,y1,color);
		Color_Pixel(i,y2,color);
	}
	for (uint8_t i = y1; i<=y2;i++){
		Color_Pixel(x1,i,color);
		Color_Pixel(x2,i,color);
	}
}
return true;
}


bool SSD1283::Draw(Shape shp,bool fill){

Point temp = shp.getLoc1();

if(shp.getLoc1().x < 0)
	temp.x = SSD1283_WIDTH * ( 1 + (int)abs(shp.getLoc1().x/SSD1283_WIDTH))+shp.getLoc1().x;
if(shp.getLoc1().y < 0)
	temp.y = SSD1283_HEIGHT * ( 1 + (int)abs(shp.getLoc1().y/SSD1283_HEIGHT))+shp.getLoc1().y;

if(shp.getLoc1().x>SSD1283_WIDTH)
	temp.x = shp.getLoc1().x- SSD1283_WIDTH*((int)abs(shp.getLoc1().x/SSD1283_WIDTH));
if(shp.getLoc1().y>SSD1283_HEIGHT)
	temp.y = shp.getLoc1().y- SSD1283_HEIGHT*((int)abs(shp.getLoc1().y/SSD1283_HEIGHT));

 
shp.setLoc1(temp);
  
if(shp.getLoc2().x<0)
	temp.x = SSD1283_WIDTH * ( 1 + (int)abs(shp.getLoc2().x/SSD1283_WIDTH))+shp.getLoc2().x;
if(shp.getLoc2().y<0)
	temp.y = SSD1283_HEIGHT * ( 1 + (int)abs(shp.getLoc2().y/SSD1283_HEIGHT))+shp.getLoc2().y;

if(shp.getLoc2().x>SSD1283_WIDTH)
	temp.x = shp.getLoc2().x- SSD1283_WIDTH*((int)abs(shp.getLoc2().x/SSD1283_WIDTH));
if(shp.getLoc2().y>SSD1283_HEIGHT)
	temp.y = shp.getLoc2().y- SSD1283_HEIGHT*((int)abs(shp.getLoc2().y/SSD1283_HEIGHT));

shp.setLoc2(temp);

	if(shp.getType() == CIRCLE){
		Draw_Circle(shp.getLoc1().x,shp.getLoc1().y,shp.getRadius(),shp.getColor(),fill);
	}
	if(shp.getType() == RECT){
		Draw_Rect(shp.getLoc1().x,shp.getLoc1().y,shp.getLoc2().x,shp.getLoc2().x,shp.getColor(),fill);
	}
	return true;
}











