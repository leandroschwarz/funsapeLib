/*
 * 8bit TFT Library for STM32F103
 * based on MCUFRIEND_kbv.cpp by David Prentice
 * https://github.com/prenticedavid/MCUFRIEND_kbv

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.	If not, see <http://www.gnu.org/licenses/>.
 */

#include <STM32_TFT_8bit.h>
//#include <avr/pgmspace.h>
//#include <limits.h>
//#include <libmaple/dma.h>

#define MIPI_DCS_REV1					(1<<0)
#define AUTO_READINC					(1<<1)
#define READ_BGR						(1<<2)
#define READ_LOWHIGH					(1<<3)
#define READ_24BITS						(1<<4)
#define XSA_XEA_16BIT					(1<<5)
#define READ_NODUMMY					(1<<6)
#define INVERT_GS						(1<<8)
#define INVERT_SS						(1<<9)
#define MV_AXIS							(1<<10)
#define INVERT_RGB						(1<<11)
#define REV_SCREEN						(1<<12)
#define FLIP_VERT						(1<<13)
#define FLIP_HORIZ						(1<<14)

static uint8_t is8347 = 0;

// Constructor when using 8080 mode of control.
STM32_TFT_8bit::STM32_TFT_8bit(void) : Adafruit_GFX(TFTWIDTH, TFTHEIGHT)
{
	//Set PB3 - PB7 as output
	//Note: CRH and CRL are both 32 bits wide
	//Each pin is represented by 5 bits 0x3 (hex) sets that pin to O/P
	TFT_CNTRL->CRL = (TFT_CNTRL->CRL & 0x00000FFF) | 0x33333000;
	CS_IDLE; // Disable CS
//	CD_DATA; // Enable Command
	WR_IDLE; // Disable WR
	RD_IDLE; // Disable RD

	// toggle RST low to reset
	RST_IDLE;	 // Set Reset HIGH
	delay(5);
	RST_ACTIVE; // Set Reset LOW
	delay(20);
	RST_IDLE;	 // Set Reset HIGH
	delay(150);
}

void STM32_TFT_8bit::setWriteDataBus(void)
{
	// set the pins to output mode
	// not required to mask and assign, because all pins of bus are set together
	TFT_DATA->CRL = 0x33333333;
	//each pin is configured by four bits, and 0b0011 or 0x3 means output mode (same as pinmode())
}

void STM32_TFT_8bit::setReadDataBus(void)
{
	//set the pins to input mode
	// not required to mask and assign, because all pins of bus are set together
	TFT_DATA->CRL = 0x88888888;
	//8 in hex is 0b1000, which means input, same as pinmode()
	// for (uint8_t i = 0; i <= 7; i++){
	//	 pinMode(DPINS[i], INPUT);
	// }
}

void STM32_TFT_8bit::write8(uint8_t c)
{
	//BRR or BSRR avoid read, mask write cycle time
	//BSRR is 32 bits wide. 1's in the most significant 16 bits signify pins to reset (clear)
	// 1's in least significant 16 bits signify pins to set high. 0's mean 'do nothing'
	TFT_DATA->BSRR = ((~c) << 16) | (c); //Set pins to the 8 bit number
	//TFT_DATA->ODR = ((TFT_DATA->ODR & 0xFF00) | ((c) & 0x00FF));//FF00 is Binary 1111111100000000
	WR_STROBE;
	//delayMicroseconds(50); //used to observe patterns
}

void STM32_TFT_8bit::writeCmdByte(uint8_t c)
{
	CD_COMMAND;
	CS_ACTIVE;
	write8(c);
	CS_IDLE;
}

void STM32_TFT_8bit::writeCmdWord(uint16_t c)
{
	CD_COMMAND;
	CS_ACTIVE;
	write8(c >> 8);
	write8(c & 0xff);
	CS_IDLE;
}

void STM32_TFT_8bit::WriteCmd(uint16_t c)
{
//	if (_lcd_cmdWidth == 8) writeCmdByte(c & 0xff);
//	if (_lcd_cmdWidth == 16) writeCmdWord(c);
	if(_lcd_capable & MIPI_DCS_REV1) {
		writeCmdByte(c & 0xff);
	} else {
		writeCmdWord(c);
	}
}

void STM32_TFT_8bit::writeDataByte(uint8_t c)
{
	CD_DATA;
	CS_ACTIVE;
	write8(c);
	CS_IDLE;
}

void STM32_TFT_8bit::writeDataWord(uint16_t c)
{
	CD_DATA;
	CS_ACTIVE;
	write8(c >> 8);
	write8(c & 0xff);
	CS_IDLE;
}

void STM32_TFT_8bit::WriteCmdData(uint16_t cmd, uint16_t dat)
{
	WriteCmd(cmd);
	writeDataWord(dat);
}

void STM32_TFT_8bit::WriteCmdParamN(uint8_t cmd, int8_t N, uint8_t *block)
{
	writeCmdByte(cmd);
	while(N-- > 0) {
		uint8_t u8 = *block++;
		writeDataByte(u8);
	}
}

void STM32_TFT_8bit::WriteCmdParam4(uint8_t cmd, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4)
{
	uint8_t d[4];
	d[0] = d1, d[1] = d2, d[2] = d3, d[3] = d4;
	WriteCmdParamN(cmd, 4, d);
}

#define TFTLCD_DELAY					0xFFFF
#define TFTLCD_DELAY8					0x7F

void STM32_TFT_8bit::init_table(const void *table, int16_t size)
{
	uint8_t *p = (uint8_t *) table, dat[24];						//R61526 has GAMMA[22]
	while(size > 0) {
		uint8_t cmd = pgm_read_byte(p++);
		uint8_t len = pgm_read_byte(p++);
		if(cmd == TFTLCD_DELAY8) {
			delay(len);
			len = 0;
		} else {
			for(uint8_t i = 0; i < len; i++) {
				dat[i] = pgm_read_byte(p++);
			}
			WriteCmdParamN(cmd, len, dat);
		}
		size -= len + 2;
	}
}

void STM32_TFT_8bit::init_table16(const void *table, int16_t size)
{
	uint16_t *p = (uint16_t *) table;
	while(size > 0) {
		uint16_t cmd = pgm_read_word(p++);
		uint16_t d = pgm_read_word(p++);
		if(cmd == TFTLCD_DELAY) {
			delay(d);
		} else {
			writeCmdWord(cmd);
			writeDataWord(d);
		}
		size -= 2 * sizeof(int16_t);
	}
}

void STM32_TFT_8bit::setResolution(int16_t width, int16_t height)
{
	_lcd_width = width;
	_lcd_height = height;
}

void STM32_TFT_8bit::setOffset(int16_t offset)
{
	_lcd_offset = offset;
}

// Rather than a bazillion writeCmdByte() and writeDataByte() calls, screen
// initialization commands and arguments are organized in these tables
// stored in PROGMEM.	The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.	Companion function follows.

//void STM32_TFT_8bit::begin(void) {
void STM32_TFT_8bit::begin(uint16_t ID)
{
	int16_t *p16;							 //so we can "write" to a const protected variable.
	const uint8_t *table8_ads = NULL;
	int16_t table_size;

	//set up 8 bit parallel port to write mode.
	setWriteDataBus();

	_lcd_ID = ID;
	switch(_lcd_ID) {
	case 0x9325:								//ILI9325
		_lcd_capable = 0 | REV_SCREEN | INVERT_GS;
		static const uint16_t ILI9325_regValues[] = {
			0x00E5, 0x78F0,		 // set SRAM internal timing
			0x0001, 0x0100,		 // set Driver Output Control
			0x0002, 0x0200,		 // set 1 line inversion
			0x0003, 0x1030,		 // set GRAM write direction and BGR=1.
			0x0004, 0x0000,		 // Resize register
			0x0005, 0x0000,		 // .kbv 16bits Data Format Selection
			0x0008, 0x0207,		 // set the back porch and front porch
			0x0009, 0x0000,		 // set non-display area refresh cycle ISC[3:0]
			0x000A, 0x0000,		 // FMARK function
			0x000C, 0x0000,		 // RGB interface setting
			0x000D, 0x0000,		 // Frame marker Position
			0x000F, 0x0000,		 // RGB interface polarity
			// ----------- Power On sequence ----------- //
			0x0010, 0x0000,		 // SAP, BT[3:0], AP, DSTB, SLP, STB
			0x0011, 0x0007,		 // DC1[2:0], DC0[2:0], VC[2:0]
			0x0012, 0x0000,		 // VREG1OUT voltage
			0x0013, 0x0000,		 // VDV[4:0] for VCOM amplitude
			0x0007, 0x0001,
			TFTLCD_DELAY, 200,	// Dis-charge capacitor power voltage
			0x0010, 0x1690,		 // SAP=1, BT=6, APE=1, AP=1, DSTB=0, SLP=0, STB=0
			0x0011, 0x0227,		 // DC1=2, DC0=2, VC=7
			TFTLCD_DELAY, 50,	 // wait_ms 50ms
			0x0012, 0x000D,		 // VCIRE=1, PON=0, VRH=5
			TFTLCD_DELAY, 50,	 // wait_ms 50ms
			0x0013, 0x1200,		 // VDV=28 for VCOM amplitude
			0x0029, 0x000A,		 // VCM=10 for VCOMH
			0x002B, 0x000D,		 // Set Frame Rate
			TFTLCD_DELAY, 50,	 // wait_ms 50ms
			0x0020, 0x0000,		 // GRAM horizontal Address
			0x0021, 0x0000,		 // GRAM Vertical Address
			// ----------- Adjust the Gamma Curve ----------//
			0x0030, 0x0000,
			0x0031, 0x0404,
			0x0032, 0x0003,
			0x0035, 0x0405,
			0x0036, 0x0808,
			0x0037, 0x0407,
			0x0038, 0x0303,
			0x0039, 0x0707,
			0x003C, 0x0504,
			0x003D, 0x0808,
			//------------------ Set GRAM area ---------------//
			0x0060, 0x2700,		 // Gate Scan Line GS=0 [0xA700]
			0x0061, 0x0001,		 // NDL,VLE, REV .kbv
			0x006A, 0x0000,		 // set scrolling line
			//-------------- Partial Display Control ---------//
			0x0080, 0x0000,
			0x0081, 0x0000,
			0x0082, 0x0000,
			0x0083, 0x0000,
			0x0084, 0x0000,
			0x0085, 0x0000,
			//-------------- Panel Control -------------------//
			0x0090, 0x0010,
			0x0092, 0x0000,
			0x0007, 0x0133,		 // 262K color and display ON
		};
		init_table16(ILI9325_regValues, sizeof(ILI9325_regValues));
		break;
	}

	// Set screen resolution
	if(_lcd_width != 0 || _lcd_height != 0) {
		p16 = (int16_t *) & HEIGHT;
		*p16 = _lcd_height;
		p16 = (int16_t *) & WIDTH;
		*p16 = _lcd_width;
	}

	if(table8_ads != NULL) {
		static const uint8_t reset_off[] PROGMEM = {
			0x01, 0,						//Soft Reset
			TFTLCD_DELAY8, 150, // .kbv will power up with ONLY reset, sleep out, display on
			0x28, 0,						//Display Off
			0x3A, 1, 0x55,			//Pixel read=565, write=565.
		};
		static const uint8_t wake_on[] PROGMEM = {
			0x11, 0,						//Sleep Out
			TFTLCD_DELAY8, 150,
			0x29, 0,						//Display On
		};
		init_table(&reset_off, sizeof(reset_off));
		init_table(table8_ads, table_size);	 //can change PIXFMT
		init_table(&wake_on, sizeof(wake_on));
	}
	setRotation(0);						 //PORTRAIT
	invertDisplay(false);
}

void STM32_TFT_8bit::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	// MCUFRIEND just plots at edge if you try to write outside of the box:
	if(x < 0 || y < 0 || x >= width() || y >= height()) {
		return;
	}
	if(_lcd_offset) {
		if(rotation == 2) {
			y += _lcd_offset;
		}
		if(rotation == 3) {
			x += _lcd_offset;
		}
	}
	if(_lcd_capable & MIPI_DCS_REV1) {
		WriteCmdParam4(_MC, x >> 8, x, x >> 8, x);
		WriteCmdParam4(_MP, y >> 8, y, y >> 8, y);
	} else {
		WriteCmdData(_MC, x);
		WriteCmdData(_MP, y);
	}
	WriteCmdData(_MW, color);
}

void STM32_TFT_8bit::setAddrWindow(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1)
{
	if(_lcd_offset) {
		if(rotation == 2) {
			y += _lcd_offset;
			y1 += _lcd_offset;
		}
		if(rotation == 3) {
			x += _lcd_offset;
			x1 += _lcd_offset;
		}
	}

	if(_lcd_capable & MIPI_DCS_REV1) {
		WriteCmdParam4(_MC, x >> 8, x, x1 >> 8, x1);
		WriteCmdParam4(_MP, y >> 8, y, y1 >> 8, y1);
	} else {
		WriteCmdData(_MC, x);
		WriteCmdData(_MP, y);
		if(_lcd_capable & XSA_XEA_16BIT) {
			if(rotation & 1) {
				y1 = y = (y1 << 8) | y;
			} else {
				x1 = x = (x1 << 8) | x;
			}
		}
		WriteCmdData(_SC, x);
		WriteCmdData(_SP, y);
		WriteCmdData(_EC, x1);
		WriteCmdData(_EP, y1);
	}
//	WriteCmd(_MW); // write to RAM
}

void STM32_TFT_8bit::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	int16_t end;
	if(w < 0) {
		w = -w;
		x -= w;
	}													 //+ve w
	end = x + w;
	if(x < 0) {
		x = 0;
	}
	if(end > width()) {
		end = width();
	}
	w = end - x;
	if(h < 0) {
		h = -h;
		y -= h;
	}													 //+ve h
	end = y + h;
	if(y < 0) {
		y = 0;
	}
	if(end > height()) {
		end = height();
	}
	h = end - y;

	setAddrWindow(x, y, x + w - 1, y + h - 1);
	WriteCmd(_MW);
	if(h > w) {
		end = h;
		h = w;
		w = end;
	}
	while(h-- > 0) {
		end = w;
		do {
			writeDataWord(color);
		} while(--end != 0);
	}
	if(!(_lcd_capable & MIPI_DCS_REV1) || ((_lcd_ID == 0x1526) && (rotation & 1))) {
		setAddrWindow(0, 0, width() - 1, height() - 1);
	}
}

void STM32_TFT_8bit::pushColors8(uint8_t *block, int16_t n, bool first)
{
	uint16_t color;
	uint8_t h, l;

	if(first) {
		WriteCmd(_MW);
	}
	while(n-- > 0) {
		h = (*block++);
		l = (*block++);
		color = h << 8 | l;
		writeDataWord(color);
	}
}

void STM32_TFT_8bit::pushColors(uint16_t *block, int16_t n, bool first)
{
	uint16_t color;

	if(first) {
		WriteCmd(_MW);
	}
	while(n-- > 0) {
		color = (*block++);
		writeDataWord(color);
	}
}

void STM32_TFT_8bit::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	// Rudimentary clipping
	if((x >= _width) || (y >= _height || h < 1)) {
		return;
	}
	if((y + h - 1) >= _height) {
		h = _height - y;
	}
	fillRect(x, y, 1, h, color);
}

void STM32_TFT_8bit::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	// Rudimentary clipping
	if((x >= _width) || (y >= _height || w < 1)) {
		return;
	}
	if((x + w - 1) >= _width) {
		w = _width - x;
	}
	fillRect(x, y, w, 1, color);
}

void STM32_TFT_8bit::fillScreen(uint16_t color)
{
	fillRect(0, 0, _width, _height, color);
}

/*
* Draw lines faster by calculating straight sections and drawing them with fastVline and fastHline.
*/
#if defined (__STM32F1__)
void STM32_TFT_8bit::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	int16_t i;
	int16_t dx, dy;
	int16_t sx, sy;
	int16_t E;

	/* distance between two points */
	dx = (x2 > x1) ? x2 - x1 : x1 - x2;
	dy = (y2 > y1) ? y2 - y1 : y1 - y2;

	/* direction of two point */
	sx = (x2 > x1) ? 1 : -1;
	sy = (y2 > y1) ? 1 : -1;

	if(y1 == y2) {
		if(x2 > x1) {
			drawFastHLine(x1, y1, x2 - x1 + 1, color);
		} else {
			drawFastHLine(x2, y1, x1 - x2 + 1, color);
		}
		return;
	} else if(x1 == x2) {
		if(y2 > y1) {
			drawFastVLine(x1, y1, y2 - y1 + 1, color);
		} else {
			drawFastVLine(x1, y2, y1 - y2 + 1, color);
		}
		return;
	}

	/* inclination < 1 */
	if(dx > dy) {
		E = -dx;
		for(i = 0; i <= dx; i++) {
			drawPixel(x1, y1, color);
			x1 += sx;
			E += 2 * dy;
			if(E >= 0) {
				y1 += sy;
				E -= 2 * dx;
			}
		}
		/* inclination >= 1 */
	} else {
		E = -dy;
		for(i = 0 ; i <= dy ; i++) {
			drawPixel(x1, y1, color);
			y1 += sy;
			E += 2 * dx;
			if(E >= 0) {
				x1 += sx;
				E -= 2 * dy;
			}
		}
	}
}
#endif

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t STM32_TFT_8bit::color565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// independent cursor and window registers.	 ILI9341/9342 auto increments.	ILI9320/9325 do not.
// VERY VERY SLOW !!
int16_t STM32_TFT_8bit::readGRAM(int16_t x, int16_t y, uint16_t *block, int16_t w, int16_t h)
{
	uint16_t ret, _MR = _MW;
	int16_t n = w * h, row = 0, col = 0;
	uint8_t r, g, b;
	if(!is8347 && _lcd_capable & MIPI_DCS_REV1) {		// HX8347 uses same register
		_MR = 0x2E;
	}
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	while(n > 0) {
		if(!(_lcd_capable & MIPI_DCS_REV1)) {
			WriteCmdData(_MC, x + col);
			WriteCmdData(_MP, y + row);
		}
		WriteCmd(_MR);
		setReadDataBus();
		CS_ACTIVE;
		CD_DATA;
		if(_lcd_capable & READ_NODUMMY) {
			;
		} else if((_lcd_capable & MIPI_DCS_REV1) || _lcd_ID == 0x1289) {
			r = read8();
		}
		if(_lcd_ID == 0x1511) {
			r = read8();	 //extra dummy for R61511
		}
		while(n) {
			if(_lcd_capable & READ_24BITS) {
				r = read8();
				g = read8();
				b = read8();
				if(_lcd_capable & READ_BGR) {
					ret = color565(b, g, r);
				} else {
					ret = color565(r, g, b);
				}
			} else {
				ret = read16bits();
				if(_lcd_capable & READ_LOWHIGH) {
					ret = (ret >> 8) | (ret << 8);
				}
				if(_lcd_capable & READ_BGR) {
					ret = (ret & 0x07E0) | (ret >> 11) | (ret << 11);
				}
			}
			*block++ = ret;
			n--;
			if(!(_lcd_capable & AUTO_READINC)) {
				break;
			}
		}
		if(++col >= w) {
			col = 0;
			if(++row >= h) {
				row = 0;
			}
		}
		RD_IDLE;
//		CS_IDLE;
		setWriteDataBus();
	}
	if(!(_lcd_capable & MIPI_DCS_REV1)) {
		setAddrWindow(0, 0, width() - 1, height() - 1);
	}
	return 0;
}

void STM32_TFT_8bit::setRotation(uint8_t r)
{
	uint16_t GS, SS;
	uint8_t val, d[3];
	rotation = r & 3;					 // just perform the operation ourselves on the protected variables
	_width = (rotation & 1) ? HEIGHT : WIDTH;
	_height = (rotation & 1) ? WIDTH : HEIGHT;

	switch(rotation) {
	case 0:										//PORTRAIT:
		val = 0x48;							//MY=0, MX=1, MV=0, ML=0, BGR=1
		break;
	case 1:										//LANDSCAPE: 90 degrees
		val = 0x28;							//MY=0, MX=0, MV=1, ML=0, BGR=1
		break;
	case 2:										//PORTRAIT_REV: 180 degrees
		val = 0x98;							//MY=1, MX=0, MV=0, ML=1, BGR=1
		break;
	case 3:										//LANDSCAPE_REV: 270 degrees
		val = 0xF8;							//MY=1, MX=1, MV=1, ML=1, BGR=1
		break;
	}

	if(_lcd_capable & INVERT_GS) {
		val ^= 0x80;
	}
	if(_lcd_capable & INVERT_SS) {
		val ^= 0x40;
	}
	if(_lcd_capable & INVERT_RGB) {
		val ^= 0x08;
	}

	if(_lcd_capable & MIPI_DCS_REV1) {
		if(_lcd_ID == 0x6814) {	//.kbv my weird 0x9486 might be 68140
			GS = (val & 0x80) ? (1 << 6) : 0;	 //MY
			SS = (val & 0x40) ? (1 << 5) : 0;	 //MX
			val &= 0x28;				//keep MV, BGR, MY=0, MX=0, ML=0
			d[0] = 0;
			d[1] = GS | SS | 0x02;			//MY, MX
			d[2] = 0x3B;
			WriteCmdParamN(0xB6, 3, d);
			goto common_MC;
		} else if(_lcd_ID == 0x1963 || _lcd_ID == 0x9481 || _lcd_ID == 0x1511) {
			if(val & 0x80) {
				val |= 0x01;		//GS
			}
			if((val & 0x40)) {
				val |= 0x02;		//SS
			}
			if(_lcd_ID == 0x1963) {
				val &= ~0xC0;
			}
			if(_lcd_ID == 0x9481) {
				val &= ~0xD0;
			}
			if(_lcd_ID == 0x1511) {
				val &= ~0x10;	 //remove ML
				val |= 0xC0;		//force penguin 180 rotation
			}
//			val &= (_lcd_ID == 0x1963) ? ~0xC0 : ~0xD0; //MY=0, MX=0 with ML=0 for ILI9481
			goto common_MC;
		} else if(is8347) {
			_MC = 0x02, _MP = 0x06, _MW = 0x22, _SC = 0x02, _EC = 0x04, _SP = 0x06, _EP = 0x08;
			if(_lcd_ID == 0x5252) {
				val |= 0x02;	 //VERT_SCROLLON
				if(val & 0x10) {
					val |= 0x04;	 //if (ML) SS=1 kludge mirror in XXX_REV modes
				}
			}
			goto common_BGR;
		}
common_MC:
		_MC = 0x2A, _MP = 0x2B, _MW = 0x2C, _SC = 0x2A, _EC = 0x2A, _SP = 0x2B, _EP = 0x2B;
common_BGR:
		WriteCmdParamN(is8347 ? 0x16 : 0x36, 1, &val);
		_lcd_madctl = val;
	} else {
		uint16_t GS, SS, ORG;
		switch(_lcd_ID) {
		case 0x5420:
		case 0x7793:
		case 0x9326:
		case 0xB509:
			_MC = 0x200, _MP = 0x201, _MW = 0x202, _SC = 0x210, _EC = 0x211, _SP = 0x212, _EP = 0x213;
			GS = (val & 0x80) ? (1 << 15) : 0;
			uint16_t NL;
			NL = ((432 / 8) - 1) << 9;
			if(_lcd_ID == 0x9326 || _lcd_ID == 0x5420) {
				NL >>= 1;
			}
			WriteCmdData(0x400, GS | NL);
			goto common_SS;

		default:
			_MC = 0x20, _MP = 0x21, _MW = 0x22, _SC = 0x50, _EC = 0x51, _SP = 0x52, _EP = 0x53;
			GS = (val & 0x80) ? (1 << 15) : 0;
			WriteCmdData(0x60, GS | 0x2700);		// Gate Scan Line (0xA700)
common_SS:
			SS = (val & 0x40) ? (1 << 8) : 0;
			WriteCmdData(0x01, SS);		 // set Driver Output Control
			ORG = (val & 0x20) ? (1 << 3) : 0;
			if(val & 0x08) {
				ORG |= 0x1000;	//BGR
			}
			_lcd_madctl = ORG | 0x0030;
			WriteCmdData(0x03, _lcd_madctl);		// set GRAM write direction and BGR=1.
			break;
		}
	}

	if((rotation & 1) && ((_lcd_capable & MV_AXIS) == 0)) {
		uint16_t x;
		x = _MC, _MC = _MP, _MP = x;
		x = _SC, _SC = _SP, _SP = x;		//.kbv check 0139
		x = _EC, _EC = _EP, _EP = x;		//.kbv check 0139
	}

	setAddrWindow(0, 0, width() - 1, height() - 1);
	vertScroll(0, HEIGHT, 0);	 //reset scrolling after a rotation
}

void STM32_TFT_8bit::vertScroll(int16_t top, int16_t scrollines, int16_t offset)
{
	if(_lcd_offset) {
		if(rotation == 2 || rotation == 3) {
			top += _lcd_offset;
		}
	}
	int16_t bfa = HEIGHT - top - scrollines;	// bottom fixed area
	int16_t vsp;
	if(_lcd_ID == 0x9327) {
		bfa += 32;
	}
	if(offset <= -scrollines || offset >= scrollines) {
		offset = 0; //valid scroll
	}
	vsp = top + offset; // vertical start position
	if(offset < 0) {
		vsp += scrollines;					//keep in unsigned range
	}
	if(_lcd_capable & MIPI_DCS_REV1) {
		uint8_t d[6];					 // for multi-byte parameters
		d[0] = top >> 8;				//TFA
		d[1] = top;
		d[2] = scrollines >> 8;			//VSA
		d[3] = scrollines;
		d[4] = bfa >> 8;				//BFA
		d[5] = bfa;
		WriteCmdParamN(is8347 ? 0x0E : 0x33, 6, d);
		d[0] = vsp >> 8;				//VSP
		d[1] = vsp;
		WriteCmdParamN(is8347 ? 0x14 : 0x37, 2, d);
		if(is8347) {
			d[0] = (offset != 0) ? (_lcd_ID == 0x8347 ? 0x02 : 0x08) : 0;
			WriteCmdParamN(_lcd_ID == 0x8347 ? 0x18 : 0x01, 1, d);	//HX8347-D
		} else if(offset == 0 && (_lcd_capable & MIPI_DCS_REV1)) {
			WriteCmdParamN(0x13, 0, NULL);		//NORMAL i.e. disable scroll
		}
		return;
	} else {
		if(top != 0) {
			return;
		}
		// cope with 9320 style variants:
		switch(_lcd_ID) {
		case 0x7783:
			WriteCmdData(0x61, _lcd_rev);	 //!NDL, !VLE, REV
			WriteCmdData(0x6A, vsp);				//VL#
			break;
		case 0x5420:
		case 0x7793:
		case 0x9326:
		case 0xB509:
			WriteCmdData(0x401, (1 << 1) | _lcd_rev);			 //VLE, REV
			WriteCmdData(0x404, vsp);			 //VL#
			break;
		default:
			// 0x6809, 0x9320, 0x9325, 0x9335, 0xB505 can only scroll whole screen
			WriteCmdData(0x61, (1 << 1) | _lcd_rev);				//!NDL, VLE, REV
			WriteCmdData(0x6A, vsp);				//VL#
			break;
		}
	}
}

void STM32_TFT_8bit::invertDisplay(boolean i)
{
	uint8_t val;
	_lcd_rev = ((_lcd_capable & REV_SCREEN) != 0) ^ i;

	if(_lcd_capable & MIPI_DCS_REV1) {
		if(is8347) {
			// HX8347D: 0x36 Panel Characteristic. REV_Panel
			// HX8347A: 0x36 is Display Control 10
			if(_lcd_ID == 0x8347 || _lcd_ID == 0x5252) {  // HX8347-A, HX5352-A
				val = _lcd_rev ? 6 : 2;			 //INVON id bit#2,	NORON=bit#1
			} else {
				val = _lcd_rev ? 8 : 10;		 //HX8347-D, G, I: SCROLLON=bit3, INVON=bit1
			}
			// HX8347: 0x01 Display Mode has diff bit mapping for A, D
			WriteCmdParamN(0x01, 1, &val);
		} else {
			WriteCmdParamN(_lcd_rev ? 0x21 : 0x20, 0, NULL);
		}
		return;
	}
	// cope with 9320 style variants:
	switch(_lcd_ID) {
	case 0x9225:																				//REV is in reg(0x07) like Samsung
	case 0x9226:
	case 0x0154:
		WriteCmdData(0x07, 0x13 | (_lcd_rev << 2));		 //.kbv kludge
		break;
	case 0x5420:
	case 0x7793:
	case 0x9326:
	case 0xB509:
		WriteCmdData(0x401, (1 << 1) | _lcd_rev);			 //.kbv kludge VLE
		break;
	default:
		WriteCmdData(0x61, _lcd_rev);
		break;
	}
}

uint8_t STM32_TFT_8bit::read8(void)
{
	RD_ACTIVE;
	delay(5);
	uint8_t temp = (TFT_DATA->IDR & 0x00FF);
	RD_IDLE;
	delay(5);
	return temp;
}

uint16_t STM32_TFT_8bit::read16bits(void)
{
	uint8_t lo;
	uint8_t hi;
	CS_ACTIVE;
	CD_DATA;
	hi = read8();
	//all MIPI_DCS_REV1 style params are 8-bit
	lo = read8();
	CS_IDLE;
	return (hi << 8) | lo;
}

uint16_t STM32_TFT_8bit::readReg16(uint16_t reg)
{
	uint16_t ret;
	setWriteDataBus();
	writeCmdWord(reg);
	setReadDataBus();
	//		READ_16(ret);
	ret = read16bits();
	setWriteDataBus();
	return ret;
}

uint16_t STM32_TFT_8bit::readReg16Index(uint16_t reg, int8_t index)
{
	uint16_t ret;
	setWriteDataBus();
	writeCmdByte(reg);
	setReadDataBus();
	//		READ_16(ret);
	do {
		ret = read16bits();
	} while(--index >= 0);	//need to test with SSD1963
	setWriteDataBus();
	return ret;
}

uint32_t STM32_TFT_8bit::readReg32(uint16_t reg)
{
	uint16_t h = readReg16Index(reg, 0);
	uint16_t l = readReg16Index(reg, 1);
	return ((uint32_t) h << 16) | (l);
}

uint32_t STM32_TFT_8bit::readReg40(uint16_t reg)
{
	uint16_t h = readReg16Index(reg, 0);
	uint16_t m = readReg16Index(reg, 1);
	uint16_t l = readReg16Index(reg, 2);
	return ((uint32_t) h << 24) | (m << 8) | (l >> 8);
}

uint16_t STM32_TFT_8bit::readID(void)
{
	uint16_t ret;
	uint8_t msb;
	ret = readReg16(0);					 //forces a reset() if called before begin()
	if(ret == 0x5408) {					//the SPFD5408 fails the 0xD3D3 test.
		return 0x5408;
	}
	if(ret == 0x5420) {					//the SPFD5420 fails the 0xD3D3 test.
		return 0x5420;
	}
	if(ret == 0x8989) {					//SSD1289 is always 8989
		return 0x1289;
	}
	ret = readReg16(0x67);				//HX8347-A
	if(ret == 0x4747) {
		return 0x8347;
	}

	ret = readReg32(0xA1);			//SSD1963: [01 57 61 01]
	if(ret == 0x6101) {
		return 0x1963;
	}
	if(ret == 0xFFFF) {					//R61526: [xx FF FF FF]
		return 0x1526;					//subsequent begin() enables Command Access
	}
//	if (ret == 0xFF00) {					//R61520: [xx FF FF 00]
//		return 0x1520;					//subsequent begin() enables Command Access
//	}

	ret = readReg40(0xBF);
	if(ret == 0x8357) {					//HX8357B: [xx 01 62 83 57 FF]
		return 0x8357;
	}
	if(ret == 0x9481) {					//ILI9481: [xx 02 04 94 81 FF]
		return 0x9481;
	}
	if(ret == 0x1511) {					//?R61511: [xx 02 04 15 11] not tested yet
		return 0x1511;
	}
	if(ret == 0x1520) {					//?R61520: [xx 01 22 15 20]
		return 0x1520;
	}
	if(ret == 0x1526) {					//?R61526: [xx 01 22 15 26]
		return 0x1526;
	}
	if(ret == 0x1581) {					//R61581:	[xx 01 22 15 81]
		return 0x1581;
	}
	if(ret == 0x1400) {					//?RM68140:[xx FF 68 14 00] not tested yet
		return 0x6814;
	}
	ret = readReg32(0xD4);
	if(ret == 0x5310) {					//NT35310: [xx 01 53 10]
		return 0x5310;
	}
	ret = readReg40(0xEF);			//ILI9327: [xx 02 04 93 27 FF]
	if(ret == 0x9327) {
		return 0x9327;
	}
	ret = readReg32(0xFE) >> 8; //weird unknown from BangGood [04 20 53]
	if(ret == 0x2053) {
		return 0x2053;
	}
	uint32_t ret32 = readReg32(0x04);
	msb = ret32 >> 16;
	ret = ret32;
//		if (msb = 0x38 && ret == 0x8000) //unknown [xx 38 80 00] with D3 = 0x1602
//		if (msb == 0xFF && ret == 0xFFFF) //R61526 [xx FF FF FF]
//				return 0x1526;					//subsequent begin() enables Command Access
	if(ret == 0x1526) {					//R61526 [xx 06 15 26] if I have written NVM
		return 0x1526;					//subsequent begin() enables Command Access
	}
	if(ret == 0x8552) {					//ST7789V: [xx 85 85 52]
		return 0x7789;
	}
	if(ret == 0xAC11) {					//?unknown [xx 61 AC 11]
		return 0xAC11;
	}
//		if (msb == 0x54 && ret == 0x8066)	//RM68140:[xx 54 80 66] But Many different controllers will return same value.
//				return 0x6814;
	ret = readReg32(0xD3);			//for ILI9488, 9486, 9340, 9341
	msb = ret >> 8;
	if(msb == 0x93 || msb == 0x94 || msb == 0x98 || msb == 0x77 || msb == 0x16) {
		return ret;						 //0x9488, 9486, 9340, 9341, 7796
	}
	if(ret == 0x00D3 || ret == 0xD3D3) {
		return ret;						 //16-bit write-only bus
	}
	ret = readReg16(0);					//0154, 7783, 9320, 9325, 9335, B505, B509
	return ret;
}
