
#include "ili9325d.hpp"

#define INTERFACE_8_BITS				0
#define INTERFACE_16_BITS				1
#define INTERFACE_16_BITS_LATCHED		2

#define clrCsPin()						HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define setCsPin()						HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define clrRdPin()						HAL_GPIO_WritePin(LCD_RD_PORT, LCD_RD_PIN, GPIO_PIN_RESET)
#define setRdPin()						HAL_GPIO_WritePin(LCD_RD_PORT, LCD_RD_PIN, GPIO_PIN_SET)
#define clrRsPin()						HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET)
#define setRsPin()						HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET)
#define clrRstPin()						HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)
#define setRstPin()						HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)
#define clrWrPin()						HAL_GPIO_WritePin(LCD_WR_PORT, LCD_WR_PIN, GPIO_PIN_RESET)
#define setWrPin()						HAL_GPIO_WritePin(LCD_WR_PORT, LCD_WR_PIN, GPIO_PIN_SET)
#define clrAlePin()						HAL_GPIO_WritePin(LCD_ALE_PORT, LCD_ALE_PIN, GPIO_PIN_RESET)
#define setAlePin()						HAL_GPIO_WritePin(LCD_ALE_PORT, LCD_ALE_PIN, GPIO_PIN_SET)
#define pulseReset()					do {	\
		HAL_Delay(200);							\
		setRstPin();							\
		HAL_Delay(200);							\
		clrRstPin();							\
		HAL_Delay(200);							\
		setRstPin();							\
		HAL_Delay(200);							\
	} while(0)
#define pulseWrHigh()					do {	\
		clrWrPin();								\
		setWrPin();								\
		clrWrPin();								\
	} while(0)
#define pulseWrLow()					do {	\
		setRstPin();							\
		clrRstPin();							\
		setRstPin();							\
	} while(0)
#define pulseAleHigh()					do {	\
		clrAlePin();							\
		setAlePin();							\
		clrAlePin();							\
	} while(0)
#define pulseAleLow()					do {	\
		setAlePin();							\
		clrAlePin();							\
		setAlePin();							\
	} while(0)
#define writeDataByteLow(data)			do {	\
		uint32_t aux32 = LCD_DATA_PORT->ODR;	\
		aux32 &= 0xFF00;						\
		aux32 |= ((data) & 0xFF);				\
		LCD_DATA_PORT->ODR = aux32;				\
	} while(0)
#define writeDataByteHigh(data)			do {	\
		uint32_t aux32 = LCD_DATA_PORT->ODR;	\
		aux32 &= 0x00FF;						\
		aux32 |= (((data) & 0xFF) >> 8);		\
		LCD_DATA_PORT->ODR = aux32;				\
	} while(0)
#define writeDataWord(data)				LCD_DATA_PORT->ODR = (data)

volatile uint16_t LCD_HEIGHT = ILI9341_SCREEN_HEIGHT;
volatile uint16_t LCD_WIDTH	 = ILI9341_SCREEN_WIDTH;
uint16_t fch;
uint16_t fcl;
uint16_t bch;
uint16_t bcl;
bool orient = LANDSCAPE;
const uint16_t disp_x_size = 239;
const uint16_t disp_y_size = 319;
uint8_t interface = INTERFACE_8_BITS;

void LCD_Writ_Bus(uint8_t VH, uint8_t VL)
{
	uint16_t aux16 = 0;

	switch(interface) {
	case INTERFACE_8_BITS:				// 8 bits
		writeDataByteLow(VH);
		pulseWrLow();
		writeDataByteLow(VL);
		pulseWrLow();
		break;
	case INTERFACE_16_BITS:				// 16 bits
		aux16 = VH;
		aux16 <<= 8;
		aux16 |= VL;
		pulseWrLow();
		break;
	case INTERFACE_16_BITS_LATCHED:		// 16 bits Latched
		writeDataByteLow(VH);
		pulseAleHigh();
		clrCsPin();
		writeDataByteLow(VL);
		pulseWrLow();
		setCsPin();
		break;
	}

	return;
}

void LCD_Write_COM(char com1)
{
	clrRsPin();
	LCD_Writ_Bus(0x00, com1);

	return;
}

void LCD_Write_DATA(char VH, char VL)
{
	setRsPin();
	LCD_Writ_Bus(VH, VL);

	return;
}

void LCD_Write_COM_DATA(char com1, int dat1)
{
	clrRsPin();
	LCD_Writ_Bus(0x00, com1);
	setRsPin();
	LCD_Writ_Bus(dat1 >> 8, dat1);

	return;
}

void setColor(uint16_t r, uint16_t g, uint16_t b)
{
	fch = ((r & 248) | g >> 5);
	fcl = ((g & 28) << 3 | b >> 3);

	return;
}

void setBackColor(uint16_t r, uint16_t g, uint16_t b)
{
	bch = ((r & 248) | g >> 5);
	bcl = ((g & 28) << 3 | b >> 3);

	return;
}

void drawPixel(uint16_t x, uint16_t y)
{
	clrCsPin();
	setXY(x, y, x, y);
	setPixel((fch << 8) | fcl);
	setCsPin();
	clrXY();

	return;
}

void setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if(orient == LANDSCAPE) {
		swap(x1, y1);
		swap(x2, y2);
		y1 = disp_y_size - y1;
		y2 = disp_y_size - y2;
		swap(y1, y2);
	}
	LCD_Write_COM_DATA(0x20, x1);
	LCD_Write_COM_DATA(0x21, y1);
	LCD_Write_COM_DATA(0x50, x1);
	LCD_Write_COM_DATA(0x52, y1);
	LCD_Write_COM_DATA(0x51, x2);
	LCD_Write_COM_DATA(0x53, y2);
	LCD_Write_COM(0x22);

	return;
}

void clrXY(void)
{
	if(orient == PORTRAIT) {
		setXY(0, 0, disp_x_size, disp_y_size);
	} else {
		setXY(0, 0, disp_y_size, disp_x_size);
	}

	return;
}

void clrScr(void)
{
	clrCsPin();
	clrXY();
	setRsPin();
	if(interface == INTERFACE_16_BITS) {
		_fast_fill_16(0, 0, ((disp_x_size + 1) * (disp_y_size + 1)));
	} else if(interface == INTERFACE_8_BITS) {
		_fast_fill_8(0, ((disp_x_size + 1) * (disp_y_size + 1)));
	}
	setCsPin();

	return;
}

void _fast_fill_16(uint16_t ch, uint16_t cl, uint32_t pix)
{
	uint32_t blocks;
	uint16_t aux16 = 0;

	aux16 = ch;
	aux16 <<= 8;
	aux16 |= cl;
	writeDataWord(aux16);

	blocks = pix / 16;
	for(uint16_t i = 0; i < blocks; i++) {
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
		pulseWrLow();
	}
	if((pix % 16) != 0) {
		for(uint16_t i = 0; i < (pix % 16) + 1; i++) {
			pulseWrLow();
		}
	}

	return;
}

void _fast_fill_8(uint16_t ch, uint32_t pix)
{
	uint32_t blocks;

	writeDataByteLow(ch);

	blocks = pix / 16;
	for(uint16_t i = 0; i < blocks; i++) {
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
		pulseWrLow(); pulseWrLow();
	}
	if((pix % 16) != 0)
		for(uint16_t i = 0; i < (pix % 16) + 1; i++) {
			pulseWrLow(); pulseWrLow();
		}
}

void init(void)
{
	// setAlePin();
	setCsPin();
	// setRdPin();
	setRsPin();
	setRstPin();
	setWrPin();

	pulseReset();
	clrCsPin();

	// LCD_Write_COM_DATA(0xE5, 0x78F0);	// set SRAM internal timing
	LCD_Write_COM_DATA(0x01, 0x0100);	// set Driver Output Control
	LCD_Write_COM_DATA(0x02, 0x0200);	// set 1 line inversion
	LCD_Write_COM_DATA(0x03, 0x1030);	// set GRAM write direction and BGR=1.
	// LCD_Write_COM_DATA(0x04, 0x0000);	// Resize register
	LCD_Write_COM_DATA(0x08, 0x0207);	// set the back porch and front porch
	LCD_Write_COM_DATA(0x09, 0x0000);	// set non-display area refresh cycle ISC[3:0]
	LCD_Write_COM_DATA(0x0A, 0x0000);	// FMARK function
	LCD_Write_COM_DATA(0x0C, 0x0000);	// RGB interface setting
	LCD_Write_COM_DATA(0x0D, 0x0000);	// Frame marker Position
	LCD_Write_COM_DATA(0x0F, 0x0000);	// RGB interface polarity
	//*************Power On sequence ****************//
	LCD_Write_COM_DATA(0x10, 0x0000);	// SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_Write_COM_DATA(0x11, 0x0007);	// DC1[2:0], DC0[2:0], VC[2:0]
	LCD_Write_COM_DATA(0x12, 0x0000);	// VREG1OUT voltage
	LCD_Write_COM_DATA(0x13, 0x0000);	// VDV[4:0] for VCOM amplitude
	LCD_Write_COM_DATA(0x07, 0x0001);
	HAL_Delay(200);						// Discharge capacitor power voltage
	LCD_Write_COM_DATA(0x10, 0x1690);	// SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_Write_COM_DATA(0x11, 0x0227);	// Set DC1[2:0], DC0[2:0], VC[2:0]
	HAL_Delay(50);						// Delay 50ms
	LCD_Write_COM_DATA(0x12, 0x000D); 	// 0012
	HAL_Delay(50);						// Delay 50ms
	LCD_Write_COM_DATA(0x13, 0x1200);	// VDV[4:0] for VCOM amplitude
	LCD_Write_COM_DATA(0x29, 0x000A);	// 04  VCM[5:0] for VCOMH
	LCD_Write_COM_DATA(0x2B, 0x000D);	// Set Frame Rate
	HAL_Delay(50);						// Delay 50ms
	LCD_Write_COM_DATA(0x20, 0x0000);	// GRAM horizontal Address
	LCD_Write_COM_DATA(0x21, 0x0000);	// GRAM Vertical Address
	// ----------- Adjust the Gamma Curve ----------//
	LCD_Write_COM_DATA(0x30, 0x0000);
	LCD_Write_COM_DATA(0x31, 0x0404);
	LCD_Write_COM_DATA(0x32, 0x0003);
	LCD_Write_COM_DATA(0x35, 0x0405);
	LCD_Write_COM_DATA(0x36, 0x0808);
	LCD_Write_COM_DATA(0x37, 0x0407);
	LCD_Write_COM_DATA(0x38, 0x0303);
	LCD_Write_COM_DATA(0x39, 0x0707);
	LCD_Write_COM_DATA(0x3C, 0x0504);
	LCD_Write_COM_DATA(0x3D, 0x0808);
	//------------------ Set GRAM area ---------------//
	LCD_Write_COM_DATA(0x50, 0x0000);	// Horizontal GRAM Start Address
	LCD_Write_COM_DATA(0x51, 0x00EF);	// Horizontal GRAM End Address
	LCD_Write_COM_DATA(0x52, 0x0000);	// Vertical GRAM Start Address
	LCD_Write_COM_DATA(0x53, 0x013F);	// Vertical GRAM Start Address
	LCD_Write_COM_DATA(0x60, 0xA700);	// Gate Scan Line
	LCD_Write_COM_DATA(0x61, 0x0001);	// NDL,VLE, REV
	LCD_Write_COM_DATA(0x6A, 0x0000);	// set scrolling line
	//-------------- Partial Display Control ---------//
	LCD_Write_COM_DATA(0x80, 0x0000);
	LCD_Write_COM_DATA(0x81, 0x0000);
	LCD_Write_COM_DATA(0x82, 0x0000);
	LCD_Write_COM_DATA(0x83, 0x0000);
	LCD_Write_COM_DATA(0x84, 0x0000);
	LCD_Write_COM_DATA(0x85, 0x0000);
	//-------------- Panel Control -------------------//
	LCD_Write_COM_DATA(0x90, 0x0010);
	LCD_Write_COM_DATA(0x92, 0x0000);
	LCD_Write_COM_DATA(0x07, 0x0133); // 262K color and display ON

	setColor(255, 255, 255);
	setBackColor(0, 0, 0);

	setCsPin();

	return;
}

void setPixel(uint16_t color)
{
	LCD_Write_DATA((color >> 8), (color & 0xFF));	// rrrrrggggggbbbbb

	return;
}
