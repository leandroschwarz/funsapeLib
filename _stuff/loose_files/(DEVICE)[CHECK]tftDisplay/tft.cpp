/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	tft.cpp
 * Module:		TFT Display Interface Module for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 * Build:		1002
 * Date:		July 20, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "tft.hpp"
#if __TFT_HPP != 1002
#	error [tft.cpp] Error 102 - Build mismatch on header and source code files.
#endif

#include "system.hpp"

// =============================================================================
// File exclusive - Constants
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - New data types
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// Global variables
// =============================================================================

/* NONE */

// =============================================================================
// Class constructors
// =============================================================================

Tft::Tft(tftDisplayModel model)
{
	// Reset data members - Error handling
	this->initialized_ = false;
	this->lastError_ = ERROR_NONE;
	// Reset data members - Port interface
	this->gpioRs_ = GPIO_PIN_UNDEFINED;
	this->gpioWr_ = GPIO_PIN_UNDEFINED;
	this->gpioRd_ = GPIO_PIN_UNDEFINED;
	this->gpioCs_ = GPIO_PIN_UNDEFINED;
	this->gpioReset_ = GPIO_PIN_UNDEFINED;
	this->gpioDataLow_ = GPIO_PIN_UNDEFINED;
	this->gpioDataHigh_ = GPIO_PIN_UNDEFINED;

	// Update data members - Display configuration
	this->displayModel_ = model;
	switch (model) {
	case TFT_DISPLAY_PCF8833:
		this->displaySizeX_ = 127;
		this->displaySizeY_ = 127;
		this->displayInterface_ = INTERFACE_SERIAL_5_WIRES;
		break;
	case TFT_DISPLAY_HX8353C:
	case TFT_DISPLAY_ST7735:
	case TFT_DISPLAY_ST7735_ALT:
	case TFT_DISPLAY_ST7735S:
		this->displaySizeX_ = 127;
		this->displaySizeY_ = 159;
		this->displayInterface_ = INTERFACE_SERIAL_5_WIRES;
		break;
	case TFT_DISPLAY_HX8340B_8:
	case TFT_DISPLAY_S6D0164:
		this->displaySizeX_ = 175;
		this->displaySizeY_ = 219;
		this->displayInterface_ = INTERFACE_PARALLEL_8_WIRES;
		break;
	case TFT_DISPLAY_HX8340B_S:
		this->displaySizeX_ = 175;
		this->displaySizeY_ = 219;
		this->displayInterface_ = INTERFACE_SERIAL_4_WIRES;
		break;
	case TFT_DISPLAY_HX8347A:
	case TFT_DISPLAY_ILI9325D_16:
	case TFT_DISPLAY_ILI9325D_16ALT:
	case TFT_DISPLAY_ILI9341_16:
	case TFT_DISPLAY_S1D19122:
	case TFT_DISPLAY_S6D1121_16:
	case TFT_DISPLAY_SSD1289:
		this->displaySizeX_ = 239;
		this->displaySizeY_ = 319;
		this->displayInterface_ = INTERFACE_PARALLEL_16_WIRES;
		break;
	case TFT_DISPLAY_SSD1289LATCHED:
		this->displaySizeX_ = 239;
		this->displaySizeY_ = 319;
		this->displayInterface_ = INTERFACE_PARALLEL_16_WIRES_LATCHED;
		break;
	case TFT_DISPLAY_ILI9325C:
	case TFT_DISPLAY_ILI9325D_8:
	case TFT_DISPLAY_S6D1121_8:
	case TFT_DISPLAY_SSD1289_8:
		this->displaySizeX_ = 239;
		this->displaySizeY_ = 319;
		this->displayInterface_ = INTERFACE_PARALLEL_8_WIRES;
		break;
	case TFT_DISPLAY_ILI9341_S4P:
		this->displaySizeX_ = 239;
		this->displaySizeY_ = 319;
		this->displayInterface_ = INTERFACE_SERIAL_4_WIRES;
		break;
	case TFT_DISPLAY_ILI9341_S5P:
		this->displaySizeX_ = 239;
		this->displaySizeY_ = 319;
		this->displayInterface_ = INTERFACE_SERIAL_5_WIRES;
		break;
	case TFT_DISPLAY_HX8352A:
	case TFT_DISPLAY_ILI9327:
		this->displaySizeX_ = 239;
		this->displaySizeY_ = 399;
		this->displayInterface_ = INTERFACE_PARALLEL_16_WIRES;
		break;
	case TFT_DISPLAY_SSD1963_480:
		this->displaySizeX_ = 271;
		this->displaySizeY_ = 479;
		this->displayInterface_ = INTERFACE_PARALLEL_16_WIRES;
		break;
	case TFT_DISPLAY_ILI9481:
	case TFT_DISPLAY_ILI9486:
	case TFT_DISPLAY_R61581:
		this->displaySizeX_ = 319;
		this->displaySizeY_ = 479;
		this->displayInterface_ = INTERFACE_PARALLEL_16_WIRES;
		break;
	case TFT_DISPLAY_SSD1963_800:
	case TFT_DISPLAY_SSD1963_800ALT:
		this->displaySizeX_ = 479;
		this->displaySizeY_ = 799;
		this->displayInterface_ = INTERFACE_PARALLEL_16_WIRES;
		break;
	case TFT_DISPLAY_CPLD:
		this->displaySizeX_ = 799;
		this->displaySizeY_ = 479;
		this->displayInterface_ = INTERFACE_PARALLEL_16_WIRES;
		break;
	}

	// Return successfully
	return;
}

Tft::~Tft()
{
	// Return successfully
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

bool Tft::init(	gpioPin_e rs, gpioPin_e rd, gpioPin_e wr, gpioPin_e reset,
				gpioPin_e cs, gpioPin_e dataHigh, gpioPin_e dataLow)
{
	// Check for errors
	if (	(rs == GPIO_PIN_UNDEFINED) || (rd == GPIO_PIN_UNDEFINED) ||
			(wr == GPIO_PIN_UNDEFINED) || (reset == GPIO_PIN_UNDEFINED) ||
			(cs == GPIO_PIN_UNDEFINED) || (dataHigh == GPIO_PIN_UNDEFINED)) {
		this->lastError_ = ERROR_ARGUMENTS_MISMATCH;
		this->initialized_ = false;
		return false;
	}

	// Update data members
	this->gpioRs_ = rs;
	this->gpioRd_ = rd;
	this->gpioWr_ = wr;
	this->gpioReset_ = reset;
	this->gpioCs_ = cs;
	this->gpioDataHigh_ = dataHigh;
	this->gpioDataLow_ = dataLow;
	/*
		// Port configuration
		switch (this->displayInterface_) {
		case INTERFACE_PARALLEL_8_WIRES:
			configPortMode(this->gpioDataLow_, PORT_OUTPUT_LOW);
		case INTERFACE_PARALLEL_16_WIRES:
		case INTERFACE_PARALLEL_16_WIRES_LATCHED:




	#if defined(USE_UNO_SHIELD_ON_MEGA)
			DDRH = 0x18;
			DDRG = 0x20;
			DDRE = 0x3B;
	#else
			if (mode != LATCHED_16) {
				DDRA = 0xFF;
				if (mode == 16) {
					DDRC = 0xFF;
				}
			} else {
				DDRH = 0x18;
				DDRG = 0x20;
				DDRE = 0x3B;
			}
	#endif







			_set_direction_registers(display_transfer_mode);
			P_RS	= portOutputRegister(digitalPinToPort(RS));
			B_RS	= digitalPinToBitMask(RS);
			P_WR	= portOutputRegister(digitalPinToPort(WR));
			B_WR	= digitalPinToBitMask(WR);
			P_CS	= portOutputRegister(digitalPinToPort(CS));
			B_CS	= digitalPinToBitMask(CS);
			P_RST	= portOutputRegister(digitalPinToPort(RST));
			B_RST	= digitalPinToBitMask(RST);
			if (display_transfer_mode == LATCHED_16) {
				P_ALE	= portOutputRegister(digitalPinToPort(SER));
				B_ALE	= digitalPinToBitMask(SER);
				cbi(P_ALE, B_ALE);
				pinMode(8, OUTPUT);
				digitalWrite(8, LOW);
			}
			break;
		case INTERFACE_SERIAL_4_WIRES:
		case INTERFACE_SERIAL_5_WIRES:
			P_SDA	= portOutputRegister(digitalPinToPort(RS));
			B_SDA	= digitalPinToBitMask(RS);
			P_SCL	= portOutputRegister(digitalPinToPort(WR));
			B_SCL	= digitalPinToBitMask(WR);
			P_CS	= portOutputRegister(digitalPinToPort(CS));
			B_CS	= digitalPinToBitMask(CS);
			if (RST != NOTINUSE) {
				P_RST	= portOutputRegister(digitalPinToPort(RST));
				B_RST	= digitalPinToBitMask(RST);
			}
			if (display_serial_mode != SERIAL_4PIN) {
				P_RS	= portOutputRegister(digitalPinToPort(SER));
				B_RS	= digitalPinToBitMask(SER);
			}
			break;
		default:
			this->lastError_ = ERROR_UNKNOWN;
			this->initialized_ = false;
			return false;
		}

		if (display_transfer_mode != 1) {

		} else {

		}












	*/

	// Return successfully
	this->initialized_ = true;
	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Tft::getLastError(void)
{
	return this->lastError_;
}

// =============================================================================
// Class private methods
// =============================================================================

bool Tft::checkInitialize_(void)
{
	if (!this->initialized_) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	return true;
}

// =============================================================================
// Class protected methods
// =============================================================================

/* NONE */

// =============================================================================
// General public functions definitions
// =============================================================================

/* NONE */

// =============================================================================
// Public functions definitions
// =============================================================================

/* NONE */

// =============================================================================
// Interruption handlers
// =============================================================================

/* NONE */










/*
void UTFT::_convert_float(char *buf, double num, int width, byte prec)
{
	dtostrf(num, width, prec, buf);
}

void UTFT::_hw_special_init()
{
}

void UTFT::LCD_Writ_Bus(char VH, char VL, byte mode)
{
	switch (mode) {
	case 1:
		if (display_serial_mode == SERIAL_4PIN) {
			if (VH == 1) {
				sbi(P_SDA, B_SDA);
			} else {
				cbi(P_SDA, B_SDA);
			}
			pulse_low(P_SCL, B_SCL);
		} else {
			if (VH == 1) {
				sbi(P_RS, B_RS);
			} else {
				cbi(P_RS, B_RS);
			}
		}

		if (VL & 0x80) {
			sbi(P_SDA, B_SDA);
		} else {
			cbi(P_SDA, B_SDA);
		}
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x40) {
			sbi(P_SDA, B_SDA);
		} else {
			cbi(P_SDA, B_SDA);
		}
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x20) {
			sbi(P_SDA, B_SDA);
		} else {
			cbi(P_SDA, B_SDA);
		}
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x10) {
			sbi(P_SDA, B_SDA);
		} else {
			cbi(P_SDA, B_SDA);
		}
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x08) {
			sbi(P_SDA, B_SDA);
		} else {
			cbi(P_SDA, B_SDA);
		}
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x04) {
			sbi(P_SDA, B_SDA);
		} else {
			cbi(P_SDA, B_SDA);
		}
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x02) {
			sbi(P_SDA, B_SDA);
		} else {
			cbi(P_SDA, B_SDA);
		}
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x01) {
			sbi(P_SDA, B_SDA);
		} else {
			cbi(P_SDA, B_SDA);
		}
		pulse_low(P_SCL, B_SCL);
		break;
	case 8:
#if defined(USE_UNO_SHIELD_ON_MEGA)
		PORTG &= ~0x20;
		PORTG |= (VH & 0x10) << 1;
		PORTH &= ~0x18;
		PORTH |= (VH & 0xC0) >> 3;
		PORTE &= ~0x3B;
		PORTE |= (VH & 0x03) + ((VH & 0x0C) << 2) + ((VH & 0x20) >> 2);
		pulse_low(P_WR, B_WR);
		PORTG &= ~0x20;
		PORTG |= (VL & 0x10) << 1;
		PORTH &= ~0x18;
		PORTH |= (VL & 0xC0) >> 3;
		PORTE &= ~0x3B;
		PORTE |= (VL & 0x03) + ((VL & 0x0C) << 2) + ((VL & 0x20) >> 2);
		pulse_low(P_WR, B_WR);
#else
		PORTA = VH;
		pulse_low(P_WR, B_WR);
		PORTA = VL;
		pulse_low(P_WR, B_WR);
#endif
		break;
	case 16:
		PORTA = VH;
		PORTC = VL;
		pulse_low(P_WR, B_WR);
		break;
	case LATCHED_16:
		PORTG &= ~0x20;
		PORTG |= (VH & 0x10) << 1;
		PORTH &= ~0x18;
		PORTH |= (VH & 0xC0) >> 3;
		PORTE &= ~0x3B;
		PORTE |= (VH & 0x03) + ((VH & 0x0C) << 2) + ((VH & 0x20) >> 2);
		cbi(P_ALE, B_ALE);
		pulse_high(P_ALE, B_ALE);
		cbi(P_CS, B_CS);
		PORTG &= ~0x20;
		PORTG |= (VL & 0x10) << 1;
		PORTH &= ~0x18;
		PORTH |= (VL & 0xC0) >> 3;
		PORTE &= ~0x3B;
		PORTE |= (VL & 0x03) + ((VL & 0x0C) << 2) + ((VL & 0x20) >> 2);
		pulse_low(P_WR, B_WR);
		sbi(P_CS, B_CS);
		break;
	}
}

void UTFT::_set_direction_registers(byte mode)
{
#if defined(USE_UNO_SHIELD_ON_MEGA)
	DDRH = 0x18;
	DDRG = 0x20;
	DDRE = 0x3B;
#else
	if (mode != LATCHED_16) {
		DDRA = 0xFF;
		if (mode == 16) {
			DDRC = 0xFF;
		}
	} else {
		DDRH = 0x18;
		DDRG = 0x20;
		DDRE = 0x3B;
	}
#endif
}

void UTFT::_fast_fill_16(int ch, int cl, long pix)
{
#if defined(USE_UNO_SHIELD_ON_MEGA)
	if (ch == cl) {
		_fast_fill_8(ch, pix);
	} else {
		for (int i = 0; i < pix; i++) {
			PORTG &= ~0x20;
			PORTG |= (ch & 0x10) << 1;
			PORTH &= ~0x18;
			PORTH |= (ch & 0xC0) >> 3;
			PORTE &= ~0x3B;
			PORTE |= (ch & 0x03) + ((ch & 0x0C) << 2) + ((ch & 0x20) >> 2);
			pulse_low(P_WR, B_WR);
			PORTG &= ~0x20;
			PORTG |= (cl & 0x10) << 1;
			PORTH &= ~0x18;
			PORTH |= (cl & 0xC0) >> 3;
			PORTE &= ~0x3B;
			PORTE |= (cl & 0x03) + ((cl & 0x0C) << 2) + ((cl & 0x20) >> 2);
			pulse_low(P_WR, B_WR);
		}
	}
#else
	long blocks;

	PORTA = ch;
	PORTC = cl;

	blocks = pix / 16;
	for (int i = 0; i < blocks; i++) {
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i = 0; i < (pix % 16) + 1; i++) {
			pulse_low(P_WR, B_WR);
		}
#endif
}

void UTFT::_fast_fill_8(int ch, long pix)
{
	long blocks;

#if defined(USE_UNO_SHIELD_ON_MEGA)
	PORTG &= ~0x20;
	PORTG |= (ch & 0x10) << 1;
	PORTH &= ~0x18;
	PORTH |= (ch & 0xC0) >> 3;
	PORTE &= ~0x3B;
	PORTE |= (ch & 0x03) + ((ch & 0x0C) << 2) + ((ch & 0x20) >> 2);
#else
	PORTA = ch;
#endif

	blocks = pix / 16;
	for (int i = 0; i < blocks; i++) {
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i = 0; i < (pix % 16) + 1; i++) {
			pulse_low(P_WR, B_WR); pulse_low(P_WR, B_WR);
		}
}

//#define DISABLE_CPLD		 		1	// CTE50CPLD / CTE70CPLD / EHOUSE50CPLD
//#define DISABLE_HX8340B_8 		1	// ITDB22 8bit mode
//#define DISABLE_HX8352A			1	// ITDB32WD / TFT01_32WD / CTE32W
//#define DISABLE_HX8353C           1	// DMTFT18101
//#define DISABLE_ILI9325D  		1	// ITDB24D / ITDB24DWOT / ITDB28 / TFT01_24_8 / TFT01_24_16 / DMTFT24104 / DMTFT28103	- This single define will disable both 8bit and 16bit mode for this controller
//#define DISABLE_ILI9325D_ALT 		1	// CTE28
//#define DISABLE_ILI9341_S4P		1	// MI0283QT9
//#define DISABLE_ILI9341_S5P		1	// TFT01_22SP / TFT01_24SP / TFT22SHLD / DMTFT28105
//#define DISABLE_ILI9341_16		1	// CTE32_R2
//#define DISABLE_ILI9481			1	// CTE32HR
//#define DISABLE_ILI9486			1	// CTE40
//#define DISABLE_PCF8833			1	// LPH9135
//#define DISABLE_R61581			1	// CTE35IPS
//#define DISABLE_S6D0164			1	// CTE22 / DMTFT22102
//#define DISABLE_S6D1121			1	// ITDB24E	- This single define will disable both 8bit and 16bit mode for this controller
//#define DISABLE_SSD1289			1	// ITDB32S / TFT01_32 / ELEE32_REVA / ELEE32_REVB / CTE32	- This single define will disable both 8bit, 16bit and latched mode for this controller
//#define DISABLE_SSD1963_480		1	// ITDB43 / TFT01_43
//#define DISABLE_SSD1963_800		1	// ITDB50 / TFT01_50 / CTE50 / EHOUSE50
//#define DISABLE_SSD1963_800_ALT	1	// TFT01_70 / CTE70 / EHOUSE70
//#define DISABLE_ST7735			1	// ITDB18SP
//#define DISABLE_ST7735_ALT		1	// TFT01_18SHLD
//#define DISABLE_ST7735S			1	// TFT01_18SP

UTFT::UTFT(byte model, int RS, int WR, int CS, int RST, int SER)
{
	word	dsx[] = {239, 239, 239, 239, 239, 239, 175, 175, 239, 127,		// 00-09
					 127, 239, 271, 479, 239, 239, 239, 239, 0, 239,			// 10-19
					 479, 319, 239, 175, 127, 239, 239, 319, 319, 799,		// 20-29
					 127, 127
				 };												// 30-
	word	dsy[] = {319, 399, 319, 319, 319, 319, 219, 219, 399, 159,		// 00-09
					 127, 319, 479, 799, 319, 319, 319, 319, 0, 319,			// 10-19
					 799, 479, 319, 219, 159, 319, 319, 479, 479, 479,		// 20-29
					 159, 159
				 };												// 30-
	byte	dtm[] = {16, 16, 16, 8, 8, 16, 8, SERIAL_4PIN, 16, SERIAL_5PIN,					// 00-09
					 SERIAL_5PIN, 16, 16, 16, 8, 16, LATCHED_16, 16, 0, 8,					// 10-19
					 16, 16, 16, 8, SERIAL_5PIN, SERIAL_5PIN, SERIAL_4PIN, 16, 16, 16,		// 20-29
					 SERIAL_5PIN, SERIAL_5PIN
				 };												// 30-

	disp_x_size =			dsx[model];
	disp_y_size =			dsy[model];
	display_transfer_mode =	dtm[model];
	display_model =			model;

	__p1 = RS;
	__p2 = WR;
	__p3 = CS;
	__p4 = RST;
	__p5 = SER;

	if (display_transfer_mode == SERIAL_4PIN) {
		display_transfer_mode = 1;
		display_serial_mode = SERIAL_4PIN;
	}
	if (display_transfer_mode == SERIAL_5PIN) {
		display_transfer_mode = 1;
		display_serial_mode = SERIAL_5PIN;
	}

	if (display_transfer_mode != 1) {
		_set_direction_registers(display_transfer_mode);
		P_RS	= portOutputRegister(digitalPinToPort(RS));
		B_RS	= digitalPinToBitMask(RS);
		P_WR	= portOutputRegister(digitalPinToPort(WR));
		B_WR	= digitalPinToBitMask(WR);
		P_CS	= portOutputRegister(digitalPinToPort(CS));
		B_CS	= digitalPinToBitMask(CS);
		P_RST	= portOutputRegister(digitalPinToPort(RST));
		B_RST	= digitalPinToBitMask(RST);
		if (display_transfer_mode == LATCHED_16) {
			P_ALE	= portOutputRegister(digitalPinToPort(SER));
			B_ALE	= digitalPinToBitMask(SER);
			cbi(P_ALE, B_ALE);
			pinMode(8, OUTPUT);
			digitalWrite(8, LOW);
		}
	} else {
		P_SDA	= portOutputRegister(digitalPinToPort(RS));
		B_SDA	= digitalPinToBitMask(RS);
		P_SCL	= portOutputRegister(digitalPinToPort(WR));
		B_SCL	= digitalPinToBitMask(WR);
		P_CS	= portOutputRegister(digitalPinToPort(CS));
		B_CS	= digitalPinToBitMask(CS);
		if (RST != NOTINUSE) {
			P_RST	= portOutputRegister(digitalPinToPort(RST));
			B_RST	= digitalPinToBitMask(RST);
		}
		if (display_serial_mode != SERIAL_4PIN) {
			P_RS	= portOutputRegister(digitalPinToPort(SER));
			B_RS	= digitalPinToBitMask(SER);
		}
	}
}

void UTFT::LCD_Write_COM(char VL)
{
	if (display_transfer_mode != 1) {
		cbi(P_RS, B_RS);
		LCD_Writ_Bus(0x00, VL, display_transfer_mode);
	} else {
		LCD_Writ_Bus(0x00, VL, display_transfer_mode);
	}
}

void UTFT::LCD_Write_DATA(char VH, char VL)
{
	if (display_transfer_mode != 1) {
		sbi(P_RS, B_RS);
		LCD_Writ_Bus(VH, VL, display_transfer_mode);
	} else {
		LCD_Writ_Bus(0x01, VH, display_transfer_mode);
		LCD_Writ_Bus(0x01, VL, display_transfer_mode);
	}
}

void UTFT::LCD_Write_DATA(char VL)
{
	if (display_transfer_mode != 1) {
		sbi(P_RS, B_RS);
		LCD_Writ_Bus(0x00, VL, display_transfer_mode);
	} else {
		LCD_Writ_Bus(0x01, VL, display_transfer_mode);
	}
}

void UTFT::LCD_Write_COM_DATA(char com1, int dat1)
{
	LCD_Write_COM(com1);
	LCD_Write_DATA(dat1 >> 8, dat1);
}

void UTFT::InitLCD(byte orientation)
{
	orient = orientation;
	_hw_special_init();

	pinMode(__p1, OUTPUT);
	pinMode(__p2, OUTPUT);
	pinMode(__p3, OUTPUT);
	if (__p4 != NOTINUSE) {
		pinMode(__p4, OUTPUT);
	}
	if ((display_transfer_mode == LATCHED_16) or ((display_transfer_mode == 1) and (display_serial_mode == SERIAL_5PIN))) {
		pinMode(__p5, OUTPUT);
	}
	if (display_transfer_mode != 1) {
		_set_direction_registers(display_transfer_mode);
	}

	sbi(P_RST, B_RST);
	delay(5);
	cbi(P_RST, B_RST);
	delay(15);
	sbi(P_RST, B_RST);
	delay(15);

	cbi(P_CS, B_CS);

	switch (display_model) {
	case HX8347A:
		LCD_Write_COM_DATA(0x46, 0x00A4);
		LCD_Write_COM_DATA(0x47, 0x0053);
		LCD_Write_COM_DATA(0x48, 0x0000);
		LCD_Write_COM_DATA(0x49, 0x0044);
		LCD_Write_COM_DATA(0x4a, 0x0004);
		LCD_Write_COM_DATA(0x4b, 0x0067);
		LCD_Write_COM_DATA(0x4c, 0x0033);
		LCD_Write_COM_DATA(0x4d, 0x0077);
		LCD_Write_COM_DATA(0x4e, 0x0012);
		LCD_Write_COM_DATA(0x4f, 0x004C);
		LCD_Write_COM_DATA(0x50, 0x0046);
		LCD_Write_COM_DATA(0x51, 0x0044);

//240x320 window setting
		LCD_Write_COM_DATA(0x02, 0x0000); // Column address start2
		LCD_Write_COM_DATA(0x03, 0x0000); // Column address start1
		LCD_Write_COM_DATA(0x04, 0x0000); // Column address end2
		LCD_Write_COM_DATA(0x05, 0x00ef); // Column address end1
		LCD_Write_COM_DATA(0x06, 0x0000); // Row address start2
		LCD_Write_COM_DATA(0x07, 0x0000); // Row address start1
		LCD_Write_COM_DATA(0x08, 0x0001); // Row address end2
		LCD_Write_COM_DATA(0x09, 0x003f); // Row address end1

// Display Setting
		LCD_Write_COM_DATA(0x01, 0x0006); // IDMON=0, INVON=1, NORON=1, PTLON=0
		LCD_Write_COM_DATA(0x16, 0x00C8); // MY=0, MX=0, MV=0, ML=1, BGR=0, TEON=0   0048
		LCD_Write_COM_DATA(0x23, 0x0095); // N_DC=1001 0101
		LCD_Write_COM_DATA(0x24, 0x0095); // PI_DC=1001 0101
		LCD_Write_COM_DATA(0x25, 0x00FF); // I_DC=1111 1111

		LCD_Write_COM_DATA(0x27, 0x0002); // N_BP=0000 0010
		LCD_Write_COM_DATA(0x28, 0x0002); // N_FP=0000 0010
		LCD_Write_COM_DATA(0x29, 0x0002); // PI_BP=0000 0010
		LCD_Write_COM_DATA(0x2a, 0x0002); // PI_FP=0000 0010
		LCD_Write_COM_DATA(0x2C, 0x0002); // I_BP=0000 0010
		LCD_Write_COM_DATA(0x2d, 0x0002); // I_FP=0000 0010

		LCD_Write_COM_DATA(0x3a, 0x0001); // N_RTN=0000, N_NW=001    0001
		LCD_Write_COM_DATA(0x3b, 0x0000); // P_RTN=0000, P_NW=001
		LCD_Write_COM_DATA(0x3c, 0x00f0); // I_RTN=1111, I_NW=000
		LCD_Write_COM_DATA(0x3d, 0x0000); // DIV=00
		delay(1);
		LCD_Write_COM_DATA(0x35, 0x0038); // EQS=38h
		LCD_Write_COM_DATA(0x36, 0x0078); // EQP=78h
		LCD_Write_COM_DATA(0x3E, 0x0038); // SON=38h
		LCD_Write_COM_DATA(0x40, 0x000F); // GDON=0Fh
		LCD_Write_COM_DATA(0x41, 0x00F0); // GDOFF

// Power Supply Setting
		LCD_Write_COM_DATA(0x19, 0x0049); // CADJ=0100, CUADJ=100, OSD_EN=1 ,60Hz
		LCD_Write_COM_DATA(0x93, 0x000F); // RADJ=1111, 100%
		delay(1);
		LCD_Write_COM_DATA(0x20, 0x0040); // BT=0100
		LCD_Write_COM_DATA(0x1D, 0x0007); // VC1=111   0007
		LCD_Write_COM_DATA(0x1E, 0x0000); // VC3=000
		LCD_Write_COM_DATA(0x1F, 0x0004); // VRH=0011

//VCOM SETTING
		LCD_Write_COM_DATA(0x44, 0x004D); // VCM=101 0000  4D
		LCD_Write_COM_DATA(0x45, 0x000E); // VDV=1 0001   0011
		delay(1);
		LCD_Write_COM_DATA(0x1C, 0x0004); // AP=100
		delay(2);

		LCD_Write_COM_DATA(0x1B, 0x0018); // GASENB=0, PON=0, DK=1, XDK=0, VLCD_TRI=0, STB=0
		delay(1);
		LCD_Write_COM_DATA(0x1B, 0x0010); // GASENB=0, PON=1, DK=0, XDK=0, VLCD_TRI=0, STB=0
		delay(1);
		LCD_Write_COM_DATA(0x43, 0x0080); //set VCOMG=1
		delay(2);

// Display ON Setting
		LCD_Write_COM_DATA(0x90, 0x007F); // SAP=0111 1111
		LCD_Write_COM_DATA(0x26, 0x0004); //GON=0, DTE=0, D=01
		delay(1);
		LCD_Write_COM_DATA(0x26, 0x0024); //GON=1, DTE=0, D=01
		LCD_Write_COM_DATA(0x26, 0x002C); //GON=1, DTE=0, D=11
		delay(1);
		LCD_Write_COM_DATA(0x26, 0x003C); //GON=1, DTE=1, D=11

// INTERNAL REGISTER SETTING
		LCD_Write_COM_DATA(0x57, 0x0002); // TEST_Mode=1;
		LCD_Write_COM_DATA(0x95, 0x0001); // SET DISPLAY CLOCK AND PUMPING CLOCK TO SYNCHRONIZE
		LCD_Write_COM_DATA(0x57, 0x0000); // TEST_Mode=0;
//LCD_Write_COM_DATA(0x21,0x0000);
		LCD_Write_COM(0x22);
		break;

	case ILI9327:
		LCD_Write_COM(0xE9);
		LCD_Write_DATA(0x00, 0x20);
		LCD_Write_COM(0x11); //Exit Sleep
		delay(100);
		LCD_Write_COM(0xD1);
		LCD_Write_DATA(0x00, 0x00);
		LCD_Write_DATA(0x00, 0x71);
		LCD_Write_DATA(0x00, 0x19);
		LCD_Write_COM(0xD0);
		LCD_Write_DATA(0x00, 0x07);
		LCD_Write_DATA(0x00, 0x01);
		LCD_Write_DATA(0x00, 0x08);
		LCD_Write_COM(0x36);
		LCD_Write_DATA(0x00, 0x48);
		LCD_Write_COM(0x3A);
		LCD_Write_DATA(0x00, 0x05);
		LCD_Write_COM(0xC1);
		LCD_Write_DATA(0x00, 0x10);
		LCD_Write_DATA(0x00, 0x10);
		LCD_Write_DATA(0x00, 0x02);
		LCD_Write_DATA(0x00, 0x02);
		LCD_Write_COM(0xC0); //Set Default Gamma
		LCD_Write_DATA(0x00, 0x00);
		LCD_Write_DATA(0x00, 0x35);
		LCD_Write_DATA(0x00, 0x00);
		LCD_Write_DATA(0x00, 0x00);
		LCD_Write_DATA(0x00, 0x01);
		LCD_Write_DATA(0x00, 0x02);
		LCD_Write_COM(0xC5); //Set frame rate
		LCD_Write_DATA(0x00, 0x04);
		LCD_Write_COM(0xD2); //power setting
		LCD_Write_DATA(0x00, 0x01);
		LCD_Write_DATA(0x00, 0x44);
		LCD_Write_COM(0xC8); //Set Gamma
		LCD_Write_DATA(0x00, 0x04);
		LCD_Write_DATA(0x00, 0x67);
		LCD_Write_DATA(0x00, 0x35);
		LCD_Write_DATA(0x00, 0x04);
		LCD_Write_DATA(0x00, 0x08);
		LCD_Write_DATA(0x00, 0x06);
		LCD_Write_DATA(0x00, 0x24);
		LCD_Write_DATA(0x00, 0x01);
		LCD_Write_DATA(0x00, 0x37);
		LCD_Write_DATA(0x00, 0x40);
		LCD_Write_DATA(0x00, 0x03);
		LCD_Write_DATA(0x00, 0x10);
		LCD_Write_DATA(0x00, 0x08);
		LCD_Write_DATA(0x00, 0x80);
		LCD_Write_DATA(0x00, 0x00);
		LCD_Write_COM(0x2A);
		LCD_Write_DATA(0x00, 0x00);
		LCD_Write_DATA(0x00, 0x00);
		LCD_Write_DATA(0x00, 0x00);
		LCD_Write_DATA(0x00, 0xeF);
		LCD_Write_COM(0x2B);
		LCD_Write_DATA(0x00, 0x00);
		LCD_Write_DATA(0x00, 0x00);
		LCD_Write_DATA(0x00, 0x01);
		LCD_Write_DATA(0x00, 0x8F);
		LCD_Write_COM(0x29); //display on
		LCD_Write_COM(0x2C); //display on
		break;

	case SSD1289:
	case SSD1289_8:
	case SSD1289LATCHED:
		LCD_Write_COM_DATA(0x00, 0x0001);
		LCD_Write_COM_DATA(0x03, 0xA8A4);
		LCD_Write_COM_DATA(0x0C, 0x0000);
		LCD_Write_COM_DATA(0x0D, 0x080C);
		LCD_Write_COM_DATA(0x0E, 0x2B00);
		LCD_Write_COM_DATA(0x1E, 0x00B7);
		LCD_Write_COM_DATA(0x01, 0x2B3F);
		LCD_Write_COM_DATA(0x02, 0x0600);
		LCD_Write_COM_DATA(0x10, 0x0000);
		LCD_Write_COM_DATA(0x11, 0x6070);
		LCD_Write_COM_DATA(0x05, 0x0000);
		LCD_Write_COM_DATA(0x06, 0x0000);
		LCD_Write_COM_DATA(0x16, 0xEF1C);
		LCD_Write_COM_DATA(0x17, 0x0003);
		LCD_Write_COM_DATA(0x07, 0x0233);
		LCD_Write_COM_DATA(0x0B, 0x0000);
		LCD_Write_COM_DATA(0x0F, 0x0000);
		LCD_Write_COM_DATA(0x41, 0x0000);
		LCD_Write_COM_DATA(0x42, 0x0000);
		LCD_Write_COM_DATA(0x48, 0x0000);
		LCD_Write_COM_DATA(0x49, 0x013F);
		LCD_Write_COM_DATA(0x4A, 0x0000);
		LCD_Write_COM_DATA(0x4B, 0x0000);
		LCD_Write_COM_DATA(0x44, 0xEF00);
		LCD_Write_COM_DATA(0x45, 0x0000);
		LCD_Write_COM_DATA(0x46, 0x013F);
		LCD_Write_COM_DATA(0x30, 0x0707);
		LCD_Write_COM_DATA(0x31, 0x0204);
		LCD_Write_COM_DATA(0x32, 0x0204);
		LCD_Write_COM_DATA(0x33, 0x0502);
		LCD_Write_COM_DATA(0x34, 0x0507);
		LCD_Write_COM_DATA(0x35, 0x0204);
		LCD_Write_COM_DATA(0x36, 0x0204);
		LCD_Write_COM_DATA(0x37, 0x0502);
		LCD_Write_COM_DATA(0x3A, 0x0302);
		LCD_Write_COM_DATA(0x3B, 0x0302);
		LCD_Write_COM_DATA(0x23, 0x0000);
		LCD_Write_COM_DATA(0x24, 0x0000);
		LCD_Write_COM_DATA(0x25, 0x8000);
		LCD_Write_COM_DATA(0x4f, 0x0000);
		LCD_Write_COM_DATA(0x4e, 0x0000);
		LCD_Write_COM(0x22);
		break;

	case ILI9325C:
		LCD_Write_COM_DATA(0xE5, 0x78F0); // set SRAM internal timing
		LCD_Write_COM_DATA(0x01, 0x0100); // set Driver Output Control
		LCD_Write_COM_DATA(0x02, 0x0700); // set 1 line inversion
		LCD_Write_COM_DATA(0x03, 0x1030); // set GRAM write direction and BGR=1.
		LCD_Write_COM_DATA(0x04, 0x0000); // Resize register
		LCD_Write_COM_DATA(0x08, 0x0207); // set the back porch and front porch
		LCD_Write_COM_DATA(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
		LCD_Write_COM_DATA(0x0A, 0x0000); // FMARK function
		LCD_Write_COM_DATA(0x0C, 0x0000); // RGB interface setting
		LCD_Write_COM_DATA(0x0D, 0x0000); // Frame marker Position
		LCD_Write_COM_DATA(0x0F, 0x0000); // RGB interface polarity
//								// Power On sequence
		LCD_Write_COM_DATA(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_Write_COM_DATA(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_Write_COM_DATA(0x12, 0x0000); // VREG1OUT voltage
		LCD_Write_COM_DATA(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
		LCD_Write_COM_DATA(0x07, 0x0001);
		delay(200); // Dis-charge capacitor power voltage
		LCD_Write_COM_DATA(0x10, 0x1090); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_Write_COM_DATA(0x11, 0x0227); // Set DC1[2:0], DC0[2:0], VC[2:0]
		delay(50); // Delay 50ms
		LCD_Write_COM_DATA(0x12, 0x001F); // 0012
		delay(50); // Delay 50ms
		LCD_Write_COM_DATA(0x13, 0x1500); // VDV[4:0] for VCOM amplitude
		LCD_Write_COM_DATA(0x29, 0x0027); // 04  VCM[5:0] for VCOMH
		LCD_Write_COM_DATA(0x2B, 0x000D); // Set Frame Rate
		delay(50); // Delay 50ms
		LCD_Write_COM_DATA(0x20, 0x0000); // GRAM horizontal Address
		LCD_Write_COM_DATA(0x21, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------//
		LCD_Write_COM_DATA(0x30, 0x0000);
		LCD_Write_COM_DATA(0x31, 0x0707);
		LCD_Write_COM_DATA(0x32, 0x0307);
		LCD_Write_COM_DATA(0x35, 0x0200);
		LCD_Write_COM_DATA(0x36, 0x0008);
		LCD_Write_COM_DATA(0x37, 0x0004);
		LCD_Write_COM_DATA(0x38, 0x0000);
		LCD_Write_COM_DATA(0x39, 0x0707);
		LCD_Write_COM_DATA(0x3C, 0x0002);
		LCD_Write_COM_DATA(0x3D, 0x1D04);
//------------------ Set GRAM area ---------------//
		LCD_Write_COM_DATA(0x50, 0x0000); // Horizontal GRAM Start Address
		LCD_Write_COM_DATA(0x51, 0x00EF); // Horizontal GRAM End Address
		LCD_Write_COM_DATA(0x52, 0x0000); // Vertical GRAM Start Address
		LCD_Write_COM_DATA(0x53, 0x013F); // Vertical GRAM Start Address
		LCD_Write_COM_DATA(0x60, 0xA700); // Gate Scan Line
		LCD_Write_COM_DATA(0x61, 0x0001); // NDL,VLE, REV
		LCD_Write_COM_DATA(0x6A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
		LCD_Write_COM_DATA(0x80, 0x0000);
		LCD_Write_COM_DATA(0x81, 0x0000);
		LCD_Write_COM_DATA(0x82, 0x0000);
		LCD_Write_COM_DATA(0x83, 0x0000);
		LCD_Write_COM_DATA(0x84, 0x0000);
		LCD_Write_COM_DATA(0x85, 0x0000);
//-------------- Panel Control -------------------//
		LCD_Write_COM_DATA(0x90, 0x0010);
		LCD_Write_COM_DATA(0x92, 0x0600);
		LCD_Write_COM_DATA(0x07, 0x0133); // 262K color and display ON
		break;

	case ILI9325D_8:
	case ILI9325D_16:
		LCD_Write_COM_DATA(0xE5, 0x78F0); // set SRAM internal timing
		LCD_Write_COM_DATA(0x01, 0x0100); // set Driver Output Control
		LCD_Write_COM_DATA(0x02, 0x0200); // set 1 line inversion
		LCD_Write_COM_DATA(0x03, 0x1030); // set GRAM write direction and BGR=1.
		LCD_Write_COM_DATA(0x04, 0x0000); // Resize register
		LCD_Write_COM_DATA(0x08, 0x0207); // set the back porch and front porch
		LCD_Write_COM_DATA(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
		LCD_Write_COM_DATA(0x0A, 0x0000); // FMARK function
		LCD_Write_COM_DATA(0x0C, 0x0000); // RGB interface setting
		LCD_Write_COM_DATA(0x0D, 0x0000); // Frame marker Position
		LCD_Write_COM_DATA(0x0F, 0x0000); // RGB interface polarity
// *************Power On sequence **************** //
		LCD_Write_COM_DATA(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_Write_COM_DATA(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_Write_COM_DATA(0x12, 0x0000); // VREG1OUT voltage
		LCD_Write_COM_DATA(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
		LCD_Write_COM_DATA(0x07, 0x0001);
		delay(200); // Dis-charge capacitor power voltage
		LCD_Write_COM_DATA(0x10, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_Write_COM_DATA(0x11, 0x0227); // Set DC1[2:0], DC0[2:0], VC[2:0]
		delay(50); // Delay 50ms
		LCD_Write_COM_DATA(0x12, 0x000D); // 0012
		delay(50); // Delay 50ms
		LCD_Write_COM_DATA(0x13, 0x1200); // VDV[4:0] for VCOM amplitude
		LCD_Write_COM_DATA(0x29, 0x000A); // 04  VCM[5:0] for VCOMH
		LCD_Write_COM_DATA(0x2B, 0x000D); // Set Frame Rate
		delay(50); // Delay 50ms
		LCD_Write_COM_DATA(0x20, 0x0000); // GRAM horizontal Address
		LCD_Write_COM_DATA(0x21, 0x0000); // GRAM Vertical Address
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
		LCD_Write_COM_DATA(0x50, 0x0000); // Horizontal GRAM Start Address
		LCD_Write_COM_DATA(0x51, 0x00EF); // Horizontal GRAM End Address
		LCD_Write_COM_DATA(0x52, 0x0000); // Vertical GRAM Start Address
		LCD_Write_COM_DATA(0x53, 0x013F); // Vertical GRAM Start Address
		LCD_Write_COM_DATA(0x60, 0xA700); // Gate Scan Line
		LCD_Write_COM_DATA(0x61, 0x0001); // NDL,VLE, REV
		LCD_Write_COM_DATA(0x6A, 0x0000); // set scrolling line
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
		break;

	case ILI9325D_16ALT:
		LCD_Write_COM_DATA(0xE5, 0x78F0); // set SRAM internal timing
		LCD_Write_COM_DATA(0x01, 0x0100); // set Driver Output Control
		LCD_Write_COM_DATA(0x02, 0x0700); // set 1 line inversion
		LCD_Write_COM_DATA(0x03, 0x1030); // set GRAM write direction and BGR=1.
		LCD_Write_COM_DATA(0x04, 0x0000); // Resize register
		LCD_Write_COM_DATA(0x08, 0x0207); // set the back porch and front porch
		LCD_Write_COM_DATA(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
		LCD_Write_COM_DATA(0x0A, 0x0000); // FMARK function
		LCD_Write_COM_DATA(0x0C, 0x0000); // RGB interface setting
		LCD_Write_COM_DATA(0x0D, 0x0000); // Frame marker Position
		LCD_Write_COM_DATA(0x0F, 0x0000); // RGB interface polarity
// *************Power On sequence **************** //
		LCD_Write_COM_DATA(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_Write_COM_DATA(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_Write_COM_DATA(0x12, 0x0000); // VREG1OUT voltage
		LCD_Write_COM_DATA(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
		LCD_Write_COM_DATA(0x07, 0x0001);
		delay(200); // Dis-charge capacitor power voltage
		LCD_Write_COM_DATA(0x10, 0x1590); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_Write_COM_DATA(0x11, 0x0227); // Set DC1[2:0], DC0[2:0], VC[2:0]
		delay(50); // Delay 50ms
		LCD_Write_COM_DATA(0x12, 0x009C); // 0012
		delay(50); // Delay 50ms
		LCD_Write_COM_DATA(0x13, 0x1900); // VDV[4:0] for VCOM amplitude
		LCD_Write_COM_DATA(0x29, 0x0023); // 04  VCM[5:0] for VCOMH
		LCD_Write_COM_DATA(0x2B, 0x000E); // Set Frame Rate
		delay(50); // Delay 50ms
		LCD_Write_COM_DATA(0x20, 0x0000); // GRAM horizontal Address
		LCD_Write_COM_DATA(0x21, 0x0000); // GRAM Vertical Address
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
		LCD_Write_COM_DATA(0x50, 0x0000); // Horizontal GRAM Start Address
		LCD_Write_COM_DATA(0x51, 0x00EF); // Horizontal GRAM End Address
		LCD_Write_COM_DATA(0x52, 0x0000); // Vertical GRAM Start Address
		LCD_Write_COM_DATA(0x53, 0x013F); // Vertical GRAM Start Address
		LCD_Write_COM_DATA(0x60, 0xA700); // Gate Scan Line
		LCD_Write_COM_DATA(0x61, 0x0001); // NDL,VLE, REV
		LCD_Write_COM_DATA(0x6A, 0x0000); // set scrolling line
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
		break;

	case HX8340B_8:
		LCD_Write_COM_DATA(0x26, 0x0084); //PT=10,GON=0, DTE=0, D=0100
		delay(40);
		LCD_Write_COM_DATA(0x26, 0x00B8); //PT=10,GON=1, DTE=1, D=1000
		delay(40);
		LCD_Write_COM_DATA(0x26, 0x00BC); //PT=10,GON=1, DTE=1, D=1100
		delay(20);
// LCD_Write_COM_DATA(0x0001,0x0000);     // PTL='1' Enter Partail mode

//Driving ability Setting
		LCD_Write_COM_DATA(0x60, 0x0000);
		LCD_Write_COM_DATA(0x61, 0x0006);
		LCD_Write_COM_DATA(0x62, 0x0000);
		LCD_Write_COM_DATA(0x63, 0x00C8);
		delay(20);

//Gamma Setting
		LCD_Write_COM_DATA(0x73, 0x0070);
		LCD_Write_COM_DATA(0x40, 0x0000);
		LCD_Write_COM_DATA(0x41, 0x0040);
		LCD_Write_COM_DATA(0x42, 0x0045);
		LCD_Write_COM_DATA(0x43, 0x0001);
		LCD_Write_COM_DATA(0x44, 0x0060);
		LCD_Write_COM_DATA(0x45, 0x0005);
		LCD_Write_COM_DATA(0x46, 0x000C);
		LCD_Write_COM_DATA(0x47, 0x00D1);
		LCD_Write_COM_DATA(0x48, 0x0005);

		LCD_Write_COM_DATA(0x50, 0x0075);
		LCD_Write_COM_DATA(0x51, 0x0001);
		LCD_Write_COM_DATA(0x52, 0x0067);
		LCD_Write_COM_DATA(0x53, 0x0014);
		LCD_Write_COM_DATA(0x54, 0x00F2);
		LCD_Write_COM_DATA(0x55, 0x0007);
		LCD_Write_COM_DATA(0x56, 0x0003);
		LCD_Write_COM_DATA(0x57, 0x0049);
		delay(20);

//Power Setting
		LCD_Write_COM_DATA(0x1F, 0x0003); //VRH=4.65V     VREG1?GAMMA? 00~1E  080421
		LCD_Write_COM_DATA(0x20, 0x0000); //BT (VGH~15V,VGL~-12V,DDVDH~5V)
		LCD_Write_COM_DATA(0x24, 0x0024); //VCOMH(VCOM High voltage3.2V)     0024/12    080421    11~40
		LCD_Write_COM_DATA(0x25, 0x0034); //VCOML(VCOM Low voltage -1.2V)    0034/4A    080421    29~3F
// ****VCOM offset** ///
		LCD_Write_COM_DATA(0x23, 0x002F); //VMF(no offset)
		delay(20);

// ##################################################################
// Power Supply Setting
		LCD_Write_COM_DATA(0x18, 0x0044); //I/P_RADJ,N/P_RADJ Noraml mode 60Hz
		LCD_Write_COM_DATA(0x21, 0x0001); //OSC_EN='1' start osc
		LCD_Write_COM_DATA(0x01, 0x0000); //SLP='0' out sleep
		LCD_Write_COM_DATA(0x1C, 0x0003); //AP=011
		LCD_Write_COM_DATA(0x19, 0x0006); // VOMG=1,PON=1, DK=0,
		delay(20);

// ##################################################################
// Display ON Setting
		LCD_Write_COM_DATA(0x26, 0x0084); //PT=10,GON=0, DTE=0, D=0100
		delay(40);
		LCD_Write_COM_DATA(0x26, 0x00B8); //PT=10,GON=1, DTE=1, D=1000
		delay(40);
		LCD_Write_COM_DATA(0x26, 0x00BC); //PT=10,GON=1, DTE=1, D=1100
		delay(20);

//SET GRAM AREA
		LCD_Write_COM_DATA(0x02, 0x0000);
		LCD_Write_COM_DATA(0x03, 0x0000);
		LCD_Write_COM_DATA(0x04, 0x0000);
		LCD_Write_COM_DATA(0x05, 0x00AF);
		LCD_Write_COM_DATA(0x06, 0x0000);
		LCD_Write_COM_DATA(0x07, 0x0000);
		LCD_Write_COM_DATA(0x08, 0x0000);
		LCD_Write_COM_DATA(0x09, 0x00DB);
		delay(20);
		LCD_Write_COM_DATA(0x16, 0x0008); //MV MX MY ML SET  0028
		LCD_Write_COM_DATA(0x17, 0x0005); //COLMOD Control Register (R17h)
		LCD_Write_COM(0x21);
		LCD_Write_COM(0x22);
		break;

	case HX8340B_S:
		LCD_Write_COM(0xC1);
		LCD_Write_DATA(0xFF);
		LCD_Write_DATA(0x83);
		LCD_Write_DATA(0x40);
		LCD_Write_COM(0x11);
		delay(100);
		LCD_Write_COM(0xCA);
		LCD_Write_DATA(0x70);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0xD9);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x11);

		LCD_Write_COM(0xC9);
		LCD_Write_DATA(0x90);
		LCD_Write_DATA(0x49);
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x28);
		LCD_Write_DATA(0x28);
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x06);
		delay(20);
		LCD_Write_COM(0xC2);
		LCD_Write_DATA(0x60);
		LCD_Write_DATA(0x71);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x0E);
		LCD_Write_DATA(0x05);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x09);
		LCD_Write_DATA(0x31);
		LCD_Write_DATA(0x0A);

		LCD_Write_COM(0xc3);
		LCD_Write_DATA(0x67);
		LCD_Write_DATA(0x30);
		LCD_Write_DATA(0x61);
		LCD_Write_DATA(0x17);
		LCD_Write_DATA(0x48);
		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x05);
		LCD_Write_DATA(0x33);
		delay(10);
		LCD_Write_COM(0xB5);
		LCD_Write_DATA(0x35);
		LCD_Write_DATA(0x20);
		LCD_Write_DATA(0x45);

		LCD_Write_COM(0xB4);
		LCD_Write_DATA(0x33);
		LCD_Write_DATA(0x25);
		LCD_Write_DATA(0x4c);
		delay(10);
		LCD_Write_COM(0x3a);
		LCD_Write_DATA(0x05);
		LCD_Write_COM(0x29);
		delay(10);
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0xaf);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0xdb);
		LCD_Write_COM(0x2c);
		break;

	case ST7735:
		LCD_Write_COM(0x11);//Sleep exit
		delay(12);

//ST7735R Frame Rate
		LCD_Write_COM(0xB1);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x2C);
		LCD_Write_DATA(0x2D);
		LCD_Write_COM(0xB2);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x2C);
		LCD_Write_DATA(0x2D);
		LCD_Write_COM(0xB3);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x2C);
		LCD_Write_DATA(0x2D);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x2C);
		LCD_Write_DATA(0x2D);

		LCD_Write_COM(0xB4); //Column inversion
		LCD_Write_DATA(0x07);

//ST7735R Power Sequence
		LCD_Write_COM(0xC0);
		LCD_Write_DATA(0xA2);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x84);
		LCD_Write_COM(0xC1);
		LCD_Write_DATA(0xC5);
		LCD_Write_COM(0xC2);
		LCD_Write_DATA(0x0A);
		LCD_Write_DATA(0x00);
		LCD_Write_COM(0xC3);
		LCD_Write_DATA(0x8A);
		LCD_Write_DATA(0x2A);
		LCD_Write_COM(0xC4);
		LCD_Write_DATA(0x8A);
		LCD_Write_DATA(0xEE);

		LCD_Write_COM(0xC5); //VCOM
		LCD_Write_DATA(0x0E);

		LCD_Write_COM(0x36); //MX, MY, RGB mode
		LCD_Write_DATA(0xC8);

//ST7735R Gamma Sequence
		LCD_Write_COM(0xe0);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x1a);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x18);
		LCD_Write_DATA(0x2f);
		LCD_Write_DATA(0x28);
		LCD_Write_DATA(0x20);
		LCD_Write_DATA(0x22);
		LCD_Write_DATA(0x1f);
		LCD_Write_DATA(0x1b);
		LCD_Write_DATA(0x23);
		LCD_Write_DATA(0x37);
		LCD_Write_DATA(0x00);

		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x10);
		LCD_Write_COM(0xe1);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x1b);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x17);
		LCD_Write_DATA(0x33);
		LCD_Write_DATA(0x2c);
		LCD_Write_DATA(0x29);
		LCD_Write_DATA(0x2e);
		LCD_Write_DATA(0x30);
		LCD_Write_DATA(0x30);
		LCD_Write_DATA(0x39);
		LCD_Write_DATA(0x3f);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x10);

		LCD_Write_COM(0x2a);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x7f);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x9f);

		LCD_Write_COM(0xF0); //Enable test command
		LCD_Write_DATA(0x01);
		LCD_Write_COM(0xF6); //Disable ram power save mode
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0x3A); //65k mode
		LCD_Write_DATA(0x05);
		LCD_Write_COM(0x29);//Display on
		break;

	case ST7735_ALT:
		LCD_Write_COM(0x11);//Sleep exit
		delay(12);

//ST7735R Frame Rate
		LCD_Write_COM(0xB1);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x2C);
		LCD_Write_DATA(0x2D);
		LCD_Write_COM(0xB2);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x2C);
		LCD_Write_DATA(0x2D);
		LCD_Write_COM(0xB3);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x2C);
		LCD_Write_DATA(0x2D);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x2C);
		LCD_Write_DATA(0x2D);

		LCD_Write_COM(0xB4); //Column inversion
		LCD_Write_DATA(0x07);

//ST7735R Power Sequence
		LCD_Write_COM(0xC0);
		LCD_Write_DATA(0xA2);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x84);
		LCD_Write_COM(0xC1);
		LCD_Write_DATA(0xC5);
		LCD_Write_COM(0xC2);
		LCD_Write_DATA(0x0A);
		LCD_Write_DATA(0x00);
		LCD_Write_COM(0xC3);
		LCD_Write_DATA(0x8A);
		LCD_Write_DATA(0x2A);
		LCD_Write_COM(0xC4);
		LCD_Write_DATA(0x8A);
		LCD_Write_DATA(0xEE);

		LCD_Write_COM(0xC5); //VCOM
		LCD_Write_DATA(0x0E);

		LCD_Write_COM(0x36); //MX, MY, RGB mode
		LCD_Write_DATA(0xC0);

//ST7735R Gamma Sequence
		LCD_Write_COM(0xe0);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x1a);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x18);
		LCD_Write_DATA(0x2f);
		LCD_Write_DATA(0x28);
		LCD_Write_DATA(0x20);
		LCD_Write_DATA(0x22);
		LCD_Write_DATA(0x1f);
		LCD_Write_DATA(0x1b);
		LCD_Write_DATA(0x23);
		LCD_Write_DATA(0x37);
		LCD_Write_DATA(0x00);

		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x10);
		LCD_Write_COM(0xe1);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x1b);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x17);
		LCD_Write_DATA(0x33);
		LCD_Write_DATA(0x2c);
		LCD_Write_DATA(0x29);
		LCD_Write_DATA(0x2e);
		LCD_Write_DATA(0x30);
		LCD_Write_DATA(0x30);
		LCD_Write_DATA(0x39);
		LCD_Write_DATA(0x3f);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x10);

		LCD_Write_COM(0x2a);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x7f);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x9f);

		LCD_Write_COM(0xF0); //Enable test command
		LCD_Write_DATA(0x01);
		LCD_Write_COM(0xF6); //Disable ram power save mode
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0x3A); //65k mode
		LCD_Write_DATA(0x05);
		LCD_Write_COM(0x29);//Display on
		break;

	case PCF8833:
		LCD_Write_COM(0x01);
		LCD_Write_COM(0x25);
		LCD_Write_DATA(0x40);
		LCD_Write_COM(0x11);
		delay(10);
		LCD_Write_COM(0x20);
		LCD_Write_COM(0x38);
		LCD_Write_COM(0x29);
		LCD_Write_COM(0x13);
		LCD_Write_COM(0x36);
		LCD_Write_DATA(0x60);
		LCD_Write_COM(0x3A);
		LCD_Write_DATA(0x05);
		LCD_Write_COM(0x2A);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x7F);
		LCD_Write_COM(0xB4);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0x0b);
		LCD_Write_DATA(0x0e);
		LCD_Write_COM(0xBA);
		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x0D);
		LCD_Write_COM(0x2B);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x7F);
		LCD_Write_COM(0x2C);
		break;

	case S1D19122:
// ************* Start Initial Sequence ********** //
		int i, R, G, B;
		LCD_Write_COM(0x11);
		LCD_Write_COM(0x13);
		LCD_Write_COM(0x29);

//--------------  Display Control ---------//
		LCD_Write_COM(0xB0);

		LCD_Write_DATA(0x05);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0xF0);
		LCD_Write_DATA(0x0A);
		LCD_Write_DATA(0x41);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x0A);
		LCD_Write_DATA(0x30);
		LCD_Write_DATA(0x31);
		LCD_Write_DATA(0x36);
		LCD_Write_DATA(0x37);
		LCD_Write_DATA(0x40);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x3F);
		LCD_Write_DATA(0x40);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x81);
		LCD_Write_DATA(0x04);
		LCD_Write_DATA(0x05);
		LCD_Write_DATA(0x64);

// ----------- Gamma  Curve  Set3 Postive----------//
		LCD_Write_COM(0xFC);

		LCD_Write_DATA(0x88);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x42);
		LCD_Write_DATA(0x42);
		LCD_Write_DATA(0x22);
		LCD_Write_DATA(0x11);
		LCD_Write_DATA(0x11);
		LCD_Write_DATA(0x22);
		LCD_Write_DATA(0x99);
		LCD_Write_DATA(0xAA);
		LCD_Write_DATA(0xAA);
		LCD_Write_DATA(0xAA);
		LCD_Write_DATA(0xBB);
		LCD_Write_DATA(0xBB);
		LCD_Write_DATA(0xAA);
		LCD_Write_DATA(0x33);
		LCD_Write_DATA(0x33);
		LCD_Write_DATA(0x11);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0xC0);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);

// ----------- Gamma  Curve  Set3 Negative----------//
		LCD_Write_COM(0xFD);

		LCD_Write_DATA(0x88);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x42);
		LCD_Write_DATA(0x42);
		LCD_Write_DATA(0x22);
		LCD_Write_DATA(0x11);
		LCD_Write_DATA(0x11);
		LCD_Write_DATA(0x22);
		LCD_Write_DATA(0x99);
		LCD_Write_DATA(0xAA);
		LCD_Write_DATA(0xAA);
		LCD_Write_DATA(0xAA);
		LCD_Write_DATA(0xBB);
		LCD_Write_DATA(0xBB);
		LCD_Write_DATA(0xAA);
		LCD_Write_DATA(0x33);
		LCD_Write_DATA(0x33);
		LCD_Write_DATA(0x11);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x03);

// ----------- EVRSER Regulator Voltage Setting---------//
		LCD_Write_COM(0xBE);

		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x15);
		LCD_Write_DATA(0x16);
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0x09);
		LCD_Write_DATA(0x15);
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);

// -----------Module Definiton Setting---------//
		LCD_Write_COM(0xC0);

		LCD_Write_DATA(0x0E);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);

// -----------PWRDEF Power Ability Ddfinition----------//
		LCD_Write_COM(0xC1);

		LCD_Write_DATA(0x2F);
		LCD_Write_DATA(0x23);
		LCD_Write_DATA(0xB4);
		LCD_Write_DATA(0xFF);
		LCD_Write_DATA(0x24);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x20);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x20);
		LCD_Write_DATA(0x20);
		LCD_Write_DATA(0x00);

// -----------Other Setting----------//
		LCD_Write_COM(0xC2);
		LCD_Write_DATA(0x03);
		LCD_Write_COM(0x26);
		LCD_Write_DATA(0x08);
		LCD_Write_COM(0x35);

		LCD_Write_COM(0x36);
		LCD_Write_DATA(0x64);
		LCD_Write_COM(0x3A);
		LCD_Write_DATA(0x05);
		LCD_Write_COM(0x2A);
		LCD_Write_DATA(0x01, 0x3f);
		LCD_Write_COM(0x2B);
		LCD_Write_DATA(0xEF);
		LCD_Write_COM(0x2c);

// -----------RGB Setting----------//
		LCD_Write_COM(0x2D);
		R = 0;
		G = 0;
		B = 0;

		for (i = 0; i < 32; i++) {
			LCD_Write_DATA(R);
			R = R + 2;
		}
		for (i = 0; i < 64; i++) {
			LCD_Write_DATA(G);
			G = G + 1;
		}
		for (i = 0; i < 32; i++) {
			LCD_Write_DATA(B);
			B = B + 2;
		}
		break;

	case HX8352A:
		LCD_Write_COM(0x83);
		LCD_Write_DATA(0x02);  //TESTM=1

		LCD_Write_COM(0x85);
		LCD_Write_DATA(0x03);  //VDC_SEL=011
		LCD_Write_COM(0x8B);
		LCD_Write_DATA(0x01);
		LCD_Write_COM(0x8C);
		LCD_Write_DATA(0x93); //STBA[7]=1,STBA[5:4]=01,STBA[1:0]=11

		LCD_Write_COM(0x91);
		LCD_Write_DATA(0x01); //DCDC_SYNC=1

		LCD_Write_COM(0x83);
		LCD_Write_DATA(0x00); //TESTM=0
//Gamma Setting

		LCD_Write_COM(0x3E);
		LCD_Write_DATA(0xB0);
		LCD_Write_COM(0x3F);
		LCD_Write_DATA(0x03);
		LCD_Write_COM(0x40);
		LCD_Write_DATA(0x10);
		LCD_Write_COM(0x41);
		LCD_Write_DATA(0x56);
		LCD_Write_COM(0x42);
		LCD_Write_DATA(0x13);
		LCD_Write_COM(0x43);
		LCD_Write_DATA(0x46);
		LCD_Write_COM(0x44);
		LCD_Write_DATA(0x23);
		LCD_Write_COM(0x45);
		LCD_Write_DATA(0x76);
		LCD_Write_COM(0x46);
		LCD_Write_DATA(0x00);
		LCD_Write_COM(0x47);
		LCD_Write_DATA(0x5E);
		LCD_Write_COM(0x48);
		LCD_Write_DATA(0x4F);
		LCD_Write_COM(0x49);
		LCD_Write_DATA(0x40);
// **********Power On sequence************

		LCD_Write_COM(0x17);
		LCD_Write_DATA(0x91);

		LCD_Write_COM(0x2B);
		LCD_Write_DATA(0xF9);
		delay(10);

		LCD_Write_COM(0x1B);
		LCD_Write_DATA(0x14);

		LCD_Write_COM(0x1A);
		LCD_Write_DATA(0x11);

		LCD_Write_COM(0x1C);
		LCD_Write_DATA(0x06);

		LCD_Write_COM(0x1F);
		LCD_Write_DATA(0x42);
		delay(20);

		LCD_Write_COM(0x19);
		LCD_Write_DATA(0x0A);

		LCD_Write_COM(0x19);
		LCD_Write_DATA(0x1A);
		delay(40);

		LCD_Write_COM(0x19);
		LCD_Write_DATA(0x12);
		delay(40);

		LCD_Write_COM(0x1E);
		LCD_Write_DATA(0x27);
		delay(100);

// **********DISPLAY ON SETTING***********

		LCD_Write_COM(0x24);
		LCD_Write_DATA(0x60);

		LCD_Write_COM(0x3D);
		LCD_Write_DATA(0x40);

		LCD_Write_COM(0x34);
		LCD_Write_DATA(0x38);

		LCD_Write_COM(0x35);
		LCD_Write_DATA(0x38);

		LCD_Write_COM(0x24);
		LCD_Write_DATA(0x38);
		delay(40);

		LCD_Write_COM(0x24);
		LCD_Write_DATA(0x3C);

		LCD_Write_COM(0x16);
		LCD_Write_DATA(0x1C);

		LCD_Write_COM(0x01);
		LCD_Write_DATA(0x06);

		LCD_Write_COM(0x55);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0x02);
		LCD_Write_DATA(0x00);
		LCD_Write_COM(0x03);
		LCD_Write_DATA(0x00);
		LCD_Write_COM(0x04);
		LCD_Write_DATA(0x00);
		LCD_Write_COM(0x05);
		LCD_Write_DATA(0xef);

		LCD_Write_COM(0x06);
		LCD_Write_DATA(0x00);
		LCD_Write_COM(0x07);
		LCD_Write_DATA(0x00);
		LCD_Write_COM(0x08);
		LCD_Write_DATA(0x01);
		LCD_Write_COM(0x09);
		LCD_Write_DATA(0x8f);

		LCD_Write_COM(0x22);
		break;

	case SSD1963_480:
		LCD_Write_COM(0xE2);		//PLL multiplier, set PLL clock to 120M
		LCD_Write_DATA(0x23);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x54);
		LCD_Write_COM(0xE0);		// PLL enable
		LCD_Write_DATA(0x01);
		delay(10);
		LCD_Write_COM(0xE0);
		LCD_Write_DATA(0x03);
		delay(10);
		LCD_Write_COM(0x01);		// software reset
		delay(100);
		LCD_Write_COM(0xE6);		//PLL setting for PCLK, depends on resolution
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x1F);
		LCD_Write_DATA(0xFF);

		LCD_Write_COM(0xB0);		//LCD SPECIFICATION
		LCD_Write_DATA(0x20);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);		//Set HDP	479
		LCD_Write_DATA(0xDF);
		LCD_Write_DATA(0x01);		//Set VDP	271
		LCD_Write_DATA(0x0F);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xB4);		//HSYNC
		LCD_Write_DATA(0x02);		//Set HT	531
		LCD_Write_DATA(0x13);
		LCD_Write_DATA(0x00);		//Set HPS	8
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0x2B);		//Set HPW	43
		LCD_Write_DATA(0x00);		//Set LPS	2
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xB6);		//VSYNC
		LCD_Write_DATA(0x01);		//Set VT	288
		LCD_Write_DATA(0x20);
		LCD_Write_DATA(0x00);		//Set VPS	4
		LCD_Write_DATA(0x04);
		LCD_Write_DATA(0x0c);		//Set VPW	12
		LCD_Write_DATA(0x00);		//Set FPS	2
		LCD_Write_DATA(0x02);

		LCD_Write_COM(0xBA);
		LCD_Write_DATA(0x0F);		//GPIO[3:0] out 1

		LCD_Write_COM(0xB8);
		LCD_Write_DATA(0x07);	    //GPIO3=input, GPIO[2:0]=output
		LCD_Write_DATA(0x01);		//GPIO0 normal

		LCD_Write_COM(0x36);		//rotation
		LCD_Write_DATA(0x22);

		LCD_Write_COM(0xF0);		//pixel data interface
		LCD_Write_DATA(0x03);

		delay(1);

		setXY(0, 0, 479, 271);
		LCD_Write_COM(0x29);		//display on

		LCD_Write_COM(0xBE);		//set PWM for B/L
		LCD_Write_DATA(0x06);
		LCD_Write_DATA(0xf0);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0xf0);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xd0);
		LCD_Write_DATA(0x0d);

		LCD_Write_COM(0x2C);
		break;

	case SSD1963_800:
		LCD_Write_COM(0xE2);		//PLL multiplier, set PLL clock to 120M
		LCD_Write_DATA(0x1E);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x54);
		LCD_Write_COM(0xE0);		// PLL enable
		LCD_Write_DATA(0x01);
		delay(10);
		LCD_Write_COM(0xE0);
		LCD_Write_DATA(0x03);
		delay(10);
		LCD_Write_COM(0x01);		// software reset
		delay(100);
		LCD_Write_COM(0xE6);		//PLL setting for PCLK, depends on resolution
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0xFF);
		LCD_Write_DATA(0xFF);

		LCD_Write_COM(0xB0);		//LCD SPECIFICATION
		LCD_Write_DATA(0x24);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x03);		//Set HDP	799
		LCD_Write_DATA(0x1F);
		LCD_Write_DATA(0x01);		//Set VDP	479
		LCD_Write_DATA(0xDF);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xB4);		//HSYNC
		LCD_Write_DATA(0x03);		//Set HT	928
		LCD_Write_DATA(0xA0);
		LCD_Write_DATA(0x00);		//Set HPS	46
		LCD_Write_DATA(0x2E);
		LCD_Write_DATA(0x30);		//Set HPW	48
		LCD_Write_DATA(0x00);		//Set LPS	15
		LCD_Write_DATA(0x0F);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xB6);		//VSYNC
		LCD_Write_DATA(0x02);		//Set VT	525
		LCD_Write_DATA(0x0D);
		LCD_Write_DATA(0x00);		//Set VPS	16
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x10);		//Set VPW	16
		LCD_Write_DATA(0x00);		//Set FPS	8
		LCD_Write_DATA(0x08);

		LCD_Write_COM(0xBA);
		LCD_Write_DATA(0x0F);		//GPIO[3:0] out 1

		LCD_Write_COM(0xB8);
		LCD_Write_DATA(0x07);	    //GPIO3=input, GPIO[2:0]=output
		LCD_Write_DATA(0x01);		//GPIO0 normal

		LCD_Write_COM(0x36);		//rotation
		LCD_Write_DATA(0x22);

		LCD_Write_COM(0xF0);		//pixel data interface
		LCD_Write_DATA(0x03);

		delay(1);

		setXY(0, 0, 799, 479);
		LCD_Write_COM(0x29);		//display on

		LCD_Write_COM(0xBE);		//set PWM for B/L
		LCD_Write_DATA(0x06);
		LCD_Write_DATA(0xf0);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0xf0);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xd0);
		LCD_Write_DATA(0x0d);

		LCD_Write_COM(0x2C);
		break;

	case SSD1963_800ALT:
		LCD_Write_COM(0xE2);		//PLL multiplier, set PLL clock to 120M
		LCD_Write_DATA(0x23);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x04);
		LCD_Write_COM(0xE0);		// PLL enable
		LCD_Write_DATA(0x01);
		delay(10);
		LCD_Write_COM(0xE0);
		LCD_Write_DATA(0x03);
		delay(10);
		LCD_Write_COM(0x01);		// software reset
		delay(100);
		LCD_Write_COM(0xE6);		//PLL setting for PCLK, depends on resolution
		LCD_Write_DATA(0x04);
		LCD_Write_DATA(0x93);
		LCD_Write_DATA(0xE0);

		LCD_Write_COM(0xB0);		//LCD SPECIFICATION
		LCD_Write_DATA(0x00);	// 0x24
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x03);		//Set HDP	799
		LCD_Write_DATA(0x1F);
		LCD_Write_DATA(0x01);		//Set VDP	479
		LCD_Write_DATA(0xDF);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xB4);		//HSYNC
		LCD_Write_DATA(0x03);		//Set HT	928
		LCD_Write_DATA(0xA0);
		LCD_Write_DATA(0x00);		//Set HPS	46
		LCD_Write_DATA(0x2E);
		LCD_Write_DATA(0x30);		//Set HPW	48
		LCD_Write_DATA(0x00);		//Set LPS	15
		LCD_Write_DATA(0x0F);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xB6);		//VSYNC
		LCD_Write_DATA(0x02);		//Set VT	525
		LCD_Write_DATA(0x0D);
		LCD_Write_DATA(0x00);		//Set VPS	16
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x10);		//Set VPW	16
		LCD_Write_DATA(0x00);		//Set FPS	8
		LCD_Write_DATA(0x08);

		LCD_Write_COM(0xBA);
		LCD_Write_DATA(0x05);		//GPIO[3:0] out 1

		LCD_Write_COM(0xB8);
		LCD_Write_DATA(0x07);	    //GPIO3=input, GPIO[2:0]=output
		LCD_Write_DATA(0x01);		//GPIO0 normal

		LCD_Write_COM(0x36);		//rotation
		LCD_Write_DATA(0x22);		// -- Set to 0x21 to rotate 180 degrees

		LCD_Write_COM(0xF0);		//pixel data interface
		LCD_Write_DATA(0x03);

		delay(10);

		setXY(0, 0, 799, 479);
		LCD_Write_COM(0x29);		//display on

		LCD_Write_COM(0xBE);		//set PWM for B/L
		LCD_Write_DATA(0x06);
		LCD_Write_DATA(0xF0);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0xF0);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xD0);
		LCD_Write_DATA(0x0D);

		LCD_Write_COM(0x2C);
		break;

	case S6D1121_8:
	case S6D1121_16:
		LCD_Write_COM_DATA(0x11, 0x2004);
		LCD_Write_COM_DATA(0x13, 0xCC00);
		LCD_Write_COM_DATA(0x15, 0x2600);
		LCD_Write_COM_DATA(0x14, 0x252A);
		LCD_Write_COM_DATA(0x12, 0x0033);
		LCD_Write_COM_DATA(0x13, 0xCC04);
		LCD_Write_COM_DATA(0x13, 0xCC06);
		LCD_Write_COM_DATA(0x13, 0xCC4F);
		LCD_Write_COM_DATA(0x13, 0x674F);
		LCD_Write_COM_DATA(0x11, 0x2003);
		LCD_Write_COM_DATA(0x30, 0x2609);
		LCD_Write_COM_DATA(0x31, 0x242C);
		LCD_Write_COM_DATA(0x32, 0x1F23);
		LCD_Write_COM_DATA(0x33, 0x2425);
		LCD_Write_COM_DATA(0x34, 0x2226);
		LCD_Write_COM_DATA(0x35, 0x2523);
		LCD_Write_COM_DATA(0x36, 0x1C1A);
		LCD_Write_COM_DATA(0x37, 0x131D);
		LCD_Write_COM_DATA(0x38, 0x0B11);
		LCD_Write_COM_DATA(0x39, 0x1210);
		LCD_Write_COM_DATA(0x3A, 0x1315);
		LCD_Write_COM_DATA(0x3B, 0x3619);
		LCD_Write_COM_DATA(0x3C, 0x0D00);
		LCD_Write_COM_DATA(0x3D, 0x000D);
		LCD_Write_COM_DATA(0x16, 0x0007);
		LCD_Write_COM_DATA(0x02, 0x0013);
		LCD_Write_COM_DATA(0x03, 0x0003);
		LCD_Write_COM_DATA(0x01, 0x0127);
		LCD_Write_COM_DATA(0x08, 0x0303);
		LCD_Write_COM_DATA(0x0A, 0x000B);
		LCD_Write_COM_DATA(0x0B, 0x0003);
		LCD_Write_COM_DATA(0x0C, 0x0000);
		LCD_Write_COM_DATA(0x41, 0x0000);
		LCD_Write_COM_DATA(0x50, 0x0000);
		LCD_Write_COM_DATA(0x60, 0x0005);
		LCD_Write_COM_DATA(0x70, 0x000B);
		LCD_Write_COM_DATA(0x71, 0x0000);
		LCD_Write_COM_DATA(0x78, 0x0000);
		LCD_Write_COM_DATA(0x7A, 0x0000);
		LCD_Write_COM_DATA(0x79, 0x0007);
		LCD_Write_COM_DATA(0x07, 0x0051);
		LCD_Write_COM_DATA(0x07, 0x0053);
		LCD_Write_COM_DATA(0x79, 0x0000);
		LCD_Write_COM(0x22);
		break;

	case ILI9481:
		LCD_Write_COM(0x11);
		delay(20);
		LCD_Write_COM(0xD0);
		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x42);
		LCD_Write_DATA(0x18);

		LCD_Write_COM(0xD1);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x10);

		LCD_Write_COM(0xD2);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x02);

		LCD_Write_COM(0xC0);
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x3B);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x11);

		LCD_Write_COM(0xC5);
		LCD_Write_DATA(0x03);

		LCD_Write_COM(0xC8);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x32);
		LCD_Write_DATA(0x36);
		LCD_Write_DATA(0x45);
		LCD_Write_DATA(0x06);
		LCD_Write_DATA(0x16);
		LCD_Write_DATA(0x37);
		LCD_Write_DATA(0x75);
		LCD_Write_DATA(0x77);
		LCD_Write_DATA(0x54);
		LCD_Write_DATA(0x0C);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0x36);
		LCD_Write_DATA(0x0A);


		LCD_Write_COM(0x3A);
		LCD_Write_DATA(0x55);

		LCD_Write_COM(0x2A);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x3F);

		LCD_Write_COM(0x2B);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0xE0);
		delay(120);
		LCD_Write_COM(0x29);
		break;

	case S6D0164:
		LCD_Write_COM_DATA(0x11, 0x001A);
		LCD_Write_COM_DATA(0x12, 0x3121);
		LCD_Write_COM_DATA(0x13, 0x006C);
		LCD_Write_COM_DATA(0x14, 0x4249);

		LCD_Write_COM_DATA(0x10, 0x0800);
		delay(10);
		LCD_Write_COM_DATA(0x11, 0x011A);
		delay(10);
		LCD_Write_COM_DATA(0x11, 0x031A);
		delay(10);
		LCD_Write_COM_DATA(0x11, 0x071A);
		delay(10);
		LCD_Write_COM_DATA(0x11, 0x0F1A);
		delay(10);
		LCD_Write_COM_DATA(0x11, 0x0F3A);
		delay(30);

		LCD_Write_COM_DATA(0x01, 0x011C);
		LCD_Write_COM_DATA(0x02, 0x0100);
		LCD_Write_COM_DATA(0x03, 0x1030);
		LCD_Write_COM_DATA(0x07, 0x0000);
		LCD_Write_COM_DATA(0x08, 0x0808);
		LCD_Write_COM_DATA(0x0B, 0x1100);
		LCD_Write_COM_DATA(0x0C, 0x0000);

		LCD_Write_COM_DATA(0x0F, 0x1401);
		LCD_Write_COM_DATA(0x15, 0x0000);
		LCD_Write_COM_DATA(0x20, 0x0000);
		LCD_Write_COM_DATA(0x21, 0x0000);

		LCD_Write_COM_DATA(0x36, 0x00AF);
		LCD_Write_COM_DATA(0x37, 0x0000);
		LCD_Write_COM_DATA(0x38, 0x00DB);
		LCD_Write_COM_DATA(0x39, 0x0000);

		LCD_Write_COM_DATA(0x0F, 0x0B01);
		LCD_Write_COM_DATA(0x07, 0x0016);
		LCD_Write_COM_DATA(0x07, 0x0017);

		LCD_Write_COM(0x22);
		break;

	case ST7735S:
		LCD_Write_COM(0x11);//Sleep exit
		delay(120);

//ST7735R Frame Rate
		LCD_Write_COM(0xB1);
		LCD_Write_DATA(0x05);
		LCD_Write_DATA(0x3C);
		LCD_Write_DATA(0x3C);
		LCD_Write_COM(0xB2);
		LCD_Write_DATA(0x05);
		LCD_Write_DATA(0x3C);
		LCD_Write_DATA(0x3C);
		LCD_Write_COM(0xB3);
		LCD_Write_DATA(0x05);
		LCD_Write_DATA(0x3C);
		LCD_Write_DATA(0x3C);
		LCD_Write_DATA(0x05);
		LCD_Write_DATA(0x3C);
		LCD_Write_DATA(0x3C);

		LCD_Write_COM(0xB4); //Column inversion
		LCD_Write_DATA(0x03);

//ST7735R Power Sequence
		LCD_Write_COM(0xC0);
		LCD_Write_DATA(0x28);
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0x04);
		LCD_Write_COM(0xC1);
		LCD_Write_DATA(0xC0);
		LCD_Write_COM(0xC2);
		LCD_Write_DATA(0x0D);
		LCD_Write_DATA(0x00);
		LCD_Write_COM(0xC3);
		LCD_Write_DATA(0x8D);
		LCD_Write_DATA(0x2A);
		LCD_Write_COM(0xC4);
		LCD_Write_DATA(0x8D);
		LCD_Write_DATA(0xEE);

		LCD_Write_COM(0xC5); //VCOM
		LCD_Write_DATA(0x1A);

		LCD_Write_COM(0x36); //MX, MY, RGB mode
		LCD_Write_DATA(0xC0);

//ST7735R Gamma Sequence
		LCD_Write_COM(0xE0);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x22);
		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x0A);
		LCD_Write_DATA(0x2E);
		LCD_Write_DATA(0x30);
		LCD_Write_DATA(0x25);
		LCD_Write_DATA(0x2A);
		LCD_Write_DATA(0x28);
		LCD_Write_DATA(0x26);
		LCD_Write_DATA(0x2E);
		LCD_Write_DATA(0x3A);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x13);
		LCD_Write_COM(0xE1);
		LCD_Write_DATA(0x04);
		LCD_Write_DATA(0x16);
		LCD_Write_DATA(0x06);
		LCD_Write_DATA(0x0D);
		LCD_Write_DATA(0x2D);
		LCD_Write_DATA(0x26);
		LCD_Write_DATA(0x23);
		LCD_Write_DATA(0x27);
		LCD_Write_DATA(0x27);
		LCD_Write_DATA(0x25);
		LCD_Write_DATA(0x2D);
		LCD_Write_DATA(0x3B);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x04);
		LCD_Write_DATA(0x13);

		LCD_Write_COM(0x3A); //65k mode
		LCD_Write_DATA(0x05);
		LCD_Write_COM(0x29);//Display on
		break;

	case ILI9341_16:
		LCD_Write_COM(0xCB);
		LCD_Write_DATA(0x39);
		LCD_Write_DATA(0x2C);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x34);
		LCD_Write_DATA(0x02);

		LCD_Write_COM(0xCF);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0XC1);
		LCD_Write_DATA(0X30);

		LCD_Write_COM(0xE8);
		LCD_Write_DATA(0x85);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x78);

		LCD_Write_COM(0xEA);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xED);
		LCD_Write_DATA(0x64);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0X12);
		LCD_Write_DATA(0X81);

		LCD_Write_COM(0xF7);
		LCD_Write_DATA(0x20);

		LCD_Write_COM(0xC0);    //Power control
		LCD_Write_DATA(0x23);   //VRH[5:0]

		LCD_Write_COM(0xC1);    //Power control
		LCD_Write_DATA(0x10);   //SAP[2:0];BT[3:0]

		LCD_Write_COM(0xC5);    //VCM control
		LCD_Write_DATA(0x3e);   //Contrast
		LCD_Write_DATA(0x28);

		LCD_Write_COM(0xC7);    //VCM control2
		LCD_Write_DATA(0x86);   //--

		LCD_Write_COM(0x36);    // Memory Access Control
		LCD_Write_DATA(0x48);

		LCD_Write_COM(0x3A);
		LCD_Write_DATA(0x55);

		LCD_Write_COM(0xB1);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x18);

		LCD_Write_COM(0xB6);    // Display Function Control
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0x82);
		LCD_Write_DATA(0x27);

		LCD_Write_COM(0x11);    //Exit Sleep
		delay(120);

		LCD_Write_COM(0x29);    //Display on
		LCD_Write_COM(0x2c);
		break;

	case ILI9341_S4P:
		LCD_Write_COM(0x11);//sleep out
		delay(20);
//LCD_Write_COM(0x01); //reset
//delay(15);
		LCD_Write_COM(0x28); //display off
		delay(5);
		LCD_Write_COM(0xCF); //power control b
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x83); //83 81 AA
		LCD_Write_DATA(0x30);
		LCD_Write_COM(0xED); //power on seq control
		LCD_Write_DATA(0x64); //64 67
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x12);
		LCD_Write_DATA(0x81);
		LCD_Write_COM(0xE8); //timing control a
		LCD_Write_DATA(0x85);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x79); //79 78
		LCD_Write_COM(0xCB); //power control a
		LCD_Write_DATA(0x39);
		LCD_Write_DATA(0X2C);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x34);
		LCD_Write_DATA(0x02);
		LCD_Write_COM(0xF7); //pump ratio control
		LCD_Write_DATA(0x20);
		LCD_Write_COM(0xEA); //timing control b
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_COM(0xC0); //power control 2
		LCD_Write_DATA(0x26); //26 25
		LCD_Write_COM(0xC1); //power control 2
		LCD_Write_DATA(0x11);
		LCD_Write_COM(0xC5); //vcom control 1
		LCD_Write_DATA(0x35);
		LCD_Write_DATA(0x3E);
		LCD_Write_COM(0xC7); //vcom control 2
		LCD_Write_DATA(0xBE); //BE 94
		LCD_Write_COM(0xB1); //frame control
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x1B); //1B 70
		LCD_Write_COM(0xB6); //display control
		LCD_Write_DATA(0x0A);
		LCD_Write_DATA(0x82);
		LCD_Write_DATA(0x27);
		LCD_Write_DATA(0x00);
		LCD_Write_COM(0xB7); //emtry mode
		LCD_Write_DATA(0x07);
		LCD_Write_COM(0x3A); //pixel format
		LCD_Write_DATA(0x55); //16bit
		LCD_Write_COM(0x36); //mem access
		LCD_Write_DATA((1 << 3) | (1 << 6));
//LCD_Write_DATA((1<<3)|(1<<7)); //rotate 180
		LCD_Write_COM(0x29); //display on
		delay(5);
		break;

	case ILI9341_S5P:
		LCD_Write_COM(0xCB);
		LCD_Write_DATA(0x39);
		LCD_Write_DATA(0x2C);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x34);
		LCD_Write_DATA(0x02);

		LCD_Write_COM(0xCF);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0XC1);
		LCD_Write_DATA(0X30);

		LCD_Write_COM(0xE8);
		LCD_Write_DATA(0x85);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x78);

		LCD_Write_COM(0xEA);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xED);
		LCD_Write_DATA(0x64);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0X12);
		LCD_Write_DATA(0X81);

		LCD_Write_COM(0xF7);
		LCD_Write_DATA(0x20);

		LCD_Write_COM(0xC0);    //Power control
		LCD_Write_DATA(0x23);   //VRH[5:0]

		LCD_Write_COM(0xC1);    //Power control
		LCD_Write_DATA(0x10);   //SAP[2:0];BT[3:0]

		LCD_Write_COM(0xC5);    //VCM control
		LCD_Write_DATA(0x3e);   //Contrast
		LCD_Write_DATA(0x28);

		LCD_Write_COM(0xC7);    //VCM control2
		LCD_Write_DATA(0x86);   //--

		LCD_Write_COM(0x36);    // Memory Access Control
		LCD_Write_DATA(0x48);

		LCD_Write_COM(0x3A);
		LCD_Write_DATA(0x55);

		LCD_Write_COM(0xB1);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x18);

		LCD_Write_COM(0xB6);    // Display Function Control
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0x82);
		LCD_Write_DATA(0x27);
		LCD_Write_COM(0x11);    //Exit Sleep
		delay(120);

		LCD_Write_COM(0x29);    //Display on
		LCD_Write_COM(0x2c);
		break;

	case R61581:
		LCD_Write_COM(0xB0);
		LCD_Write_DATA(0x1E);

		LCD_Write_COM(0xB0);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xB3);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x10);

		LCD_Write_COM(0xB4);
		LCD_Write_DATA(0x00);//0X10

		LCD_Write_COM(0xC0);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x3B);//
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x00);//NW
		LCD_Write_DATA(0x43);

		LCD_Write_COM(0xC1);
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0x15);//CLOCK
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0x08);

		LCD_Write_COM(0xC4);
		LCD_Write_DATA(0x15);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x01);

		LCD_Write_COM(0xC6);
		LCD_Write_DATA(0x02);

		LCD_Write_COM(0xC8);
		LCD_Write_DATA(0x0c);
		LCD_Write_DATA(0x05);
		LCD_Write_DATA(0x0A);//0X12
		LCD_Write_DATA(0x6B);//0x7D
		LCD_Write_DATA(0x04);
		LCD_Write_DATA(0x06);//0x08
		LCD_Write_DATA(0x15);//0x0A
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x60);//0x23

		LCD_Write_COM(0x36);
		LCD_Write_DATA(0x0A);

		LCD_Write_COM(0x0C);
		LCD_Write_DATA(0x55);

		LCD_Write_COM(0x3A);
		LCD_Write_DATA(0x55);

		LCD_Write_COM(0x38);

		LCD_Write_COM(0xD0);
		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x07);//VCI1
		LCD_Write_DATA(0x14);//VRH 0x1D
		LCD_Write_DATA(0xA2);//BT 0x06

		LCD_Write_COM(0xD1);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x5A);//VCM  0x5A
		LCD_Write_DATA(0x10);//VDV

		LCD_Write_COM(0xD2);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x04);//0x24
		LCD_Write_DATA(0x04);

		LCD_Write_COM(0x11);
		delay(150);

		LCD_Write_COM(0x2A);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0xDF);//320

		LCD_Write_COM(0x2B);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x3F);//480

		delay(100);

		LCD_Write_COM(0x29);
		delay(30);

		LCD_Write_COM(0x2C);
		delay(30);
		break;

	case ILI9486:
		LCD_Write_COM(0x11);		// Sleep OUT
		delay(50);

		LCD_Write_COM(0xF2);		// ?????
		LCD_Write_DATA(0x1C);
		LCD_Write_DATA(0xA3);
		LCD_Write_DATA(0x32);
		LCD_Write_DATA(0x02);
		LCD_Write_DATA(0xb2);
		LCD_Write_DATA(0x12);
		LCD_Write_DATA(0xFF);
		LCD_Write_DATA(0x12);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xF1);		// ?????
		LCD_Write_DATA(0x36);
		LCD_Write_DATA(0xA4);

		LCD_Write_COM(0xF8);		// ?????
		LCD_Write_DATA(0x21);
		LCD_Write_DATA(0x04);

		LCD_Write_COM(0xF9);		// ?????
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x08);

		LCD_Write_COM(0xC0);		// Power Control 1
		LCD_Write_DATA(0x0d);
		LCD_Write_DATA(0x0d);

		LCD_Write_COM(0xC1);		// Power Control 2
		LCD_Write_DATA(0x43);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xC2);		// Power Control 3
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xC5);		// VCOM Control
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x48);

		LCD_Write_COM(0xB6);		// Display Function Control
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x22);		// 0x42 = Rotate display 180 deg.
		LCD_Write_DATA(0x3B);

		LCD_Write_COM(0xE0);		// PGAMCTRL (Positive Gamma Control)
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x24);
		LCD_Write_DATA(0x1c);
		LCD_Write_DATA(0x0a);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0x43);
		LCD_Write_DATA(0x88);
		LCD_Write_DATA(0x32);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x06);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0xE1);		// NGAMCTRL (Negative Gamma Control)
		LCD_Write_DATA(0x0F);
		LCD_Write_DATA(0x38);
		LCD_Write_DATA(0x30);
		LCD_Write_DATA(0x09);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x0f);
		LCD_Write_DATA(0x4e);
		LCD_Write_DATA(0x77);
		LCD_Write_DATA(0x3c);
		LCD_Write_DATA(0x07);
		LCD_Write_DATA(0x10);
		LCD_Write_DATA(0x05);
		LCD_Write_DATA(0x23);
		LCD_Write_DATA(0x1b);
		LCD_Write_DATA(0x00);

		LCD_Write_COM(0x20);		// Display Inversion OFF
		LCD_Write_DATA(0x00);//C8

		LCD_Write_COM(0x36);		// Memory Access Control
		LCD_Write_DATA(0x0A);

		LCD_Write_COM(0x3A);		// Interface Pixel Format
		LCD_Write_DATA(0x55);

		LCD_Write_COM(0x2A);		// Column Addess Set
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0xDF);

		LCD_Write_COM(0x002B);		// Page Address Set
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x3f);
		delay(50);
		LCD_Write_COM(0x0029);		// Display ON
		LCD_Write_COM(0x002C);		// Memory Write
		break;

	case CPLD:
		if (orient == LANDSCAPE) {
			orient = PORTRAIT;
		} else {
			orient = LANDSCAPE;
		}

		LCD_Write_COM(0x0F);
		LCD_Write_COM_DATA(0x01, 0x0010);
		LCD_Write_COM(0x0F);
		break;

	case HX8353C:
		LCD_Write_COM(0xB9);//SETEXTC
		LCD_Write_DATA(0xFF);
		LCD_Write_DATA(0x83);
		LCD_Write_DATA(0x53);

		LCD_Write_COM(0xB0);//RADJ
		LCD_Write_DATA(0x3C);
		LCD_Write_DATA(0x01);

		LCD_Write_COM(0xB6);//VCOM
		LCD_Write_DATA(0x94);
		LCD_Write_DATA(0x6C);
		LCD_Write_DATA(0x50);

		LCD_Write_COM(0xB1);//PWR
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x1B);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0x77);
		LCD_Write_DATA(0x89);

		LCD_Write_COM(0xE0); //Gamma setting for tpo Panel
		LCD_Write_DATA(0x50);
		LCD_Write_DATA(0x77);
		LCD_Write_DATA(0x40);
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0xBF);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0x0F);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x01);
		LCD_Write_DATA(0x73);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x72);
		LCD_Write_DATA(0x03);
		LCD_Write_DATA(0xB0);
		LCD_Write_DATA(0x0F);
		LCD_Write_DATA(0x08);
		LCD_Write_DATA(0x00);
		LCD_Write_DATA(0x0F);

		LCD_Write_COM(0x3A);
		LCD_Write_DATA(0x05);  //05
		LCD_Write_COM(0x36);
		LCD_Write_DATA(0xC0); //83  //0B

		LCD_Write_COM(0x11); // SLPOUT
		delay(150);

		LCD_Write_COM(0x29);    // display on

		delay(150);
		LCD_Write_COM(0x2D);  //Look up table

		for (int j = 0; j < 32; j++)
		{ LCD_Write_DATA(2 * j); } //Red
		for (int j = 0; j < 64; j++)
		{ LCD_Write_DATA(1 * j); } //Green
		for (int j = 0; j < 32; j++)
		{ LCD_Write_DATA(2 * j); } //Blue

		LCD_Write_COM(0x2c);
		delay(150);
		break;
	}

	sbi (P_CS, B_CS);

	setColor(255, 255, 255);
	setBackColor(0, 0, 0);
	cfont.font = 0;
	_transparent = false;
}

void UTFT::setXY(word x1, word y1, word x2, word y2)
{
	if (orient == LANDSCAPE) {
		swap(word, x1, y1);
		swap(word, x2, y2)
		y1 = disp_y_size - y1;
		y2 = disp_y_size - y2;
		swap(word, y1, y2)
	}

	switch (display_model) {
	case ST7735_ALT:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case ILI9341_S5P:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case CPLD:
		LCD_Write_COM_DATA(0x02, y1);
		LCD_Write_COM_DATA(0x03, x1);
		LCD_Write_COM_DATA(0x06, y2);
		LCD_Write_COM_DATA(0x07, x2);
		LCD_Write_COM(0x0F);
		break;

	case HX8340B_8:
		LCD_Write_COM_DATA(0x02, 0x0000);
		LCD_Write_COM_DATA(0x03, x1);
		LCD_Write_COM_DATA(0x04, 0x0000);
		LCD_Write_COM_DATA(0x05, x2);
		LCD_Write_COM_DATA(0x06, 0x0000);
		LCD_Write_COM_DATA(0x07, y1);
		LCD_Write_COM_DATA(0x08, 0x0000);
		LCD_Write_COM_DATA(0x09, y2);
		LCD_Write_COM(0x22);
		break;

	case HX8340B_S:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case HX8347A:
		LCD_Write_COM_DATA(0x02, x1 >> 8);
		LCD_Write_COM_DATA(0x03, x1);
		LCD_Write_COM_DATA(0x04, x2 >> 8);
		LCD_Write_COM_DATA(0x05, x2);
		LCD_Write_COM_DATA(0x06, y1 >> 8);
		LCD_Write_COM_DATA(0x07, y1);
		LCD_Write_COM_DATA(0x08, y2 >> 8);
		LCD_Write_COM_DATA(0x09, y2);
		LCD_Write_COM(0x22);
		break;

	case HX8352A:
		LCD_Write_COM_DATA(0x02, x1 >> 8);
		LCD_Write_COM_DATA(0x03, x1);
		LCD_Write_COM_DATA(0x04, x2 >> 8);
		LCD_Write_COM_DATA(0x05, x2);
		LCD_Write_COM_DATA(0x06, y1 >> 8);
		LCD_Write_COM_DATA(0x07, y1);
		LCD_Write_COM_DATA(0x08, y2 >> 8);
		LCD_Write_COM_DATA(0x09, y2);
		LCD_Write_COM(0x22);
		break;

	case ILI9325C:
		LCD_Write_COM_DATA(0x20, x1);
		LCD_Write_COM_DATA(0x21, y1);
		LCD_Write_COM_DATA(0x50, x1);
		LCD_Write_COM_DATA(0x52, y1);
		LCD_Write_COM_DATA(0x51, x2);
		LCD_Write_COM_DATA(0x53, y2);
		LCD_Write_COM(0x22);
		break;

	case ILI9325D_16ALT:
		LCD_Write_COM_DATA(0x20, x1);
		LCD_Write_COM_DATA(0x21, y1);
		LCD_Write_COM_DATA(0x50, x1);
		LCD_Write_COM_DATA(0x52, y1);
		LCD_Write_COM_DATA(0x51, x2);
		LCD_Write_COM_DATA(0x53, y2);
		LCD_Write_COM(0x22);
		break;

	case ILI9325D_8:
	case ILI9325D_16:
		LCD_Write_COM_DATA(0x20, x1);
		LCD_Write_COM_DATA(0x21, y1);
		LCD_Write_COM_DATA(0x50, x1);
		LCD_Write_COM_DATA(0x52, y1);
		LCD_Write_COM_DATA(0x51, x2);
		LCD_Write_COM_DATA(0x53, y2);
		LCD_Write_COM(0x22);
		break;

	case ILI9327:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(0x00, x1 >> 8);
		LCD_Write_DATA(0x00, x1);
		LCD_Write_DATA(0x00, x2 >> 8);
		LCD_Write_DATA(0x00, x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(0x00, y1 >> 8);
		LCD_Write_DATA(0x00, y1);
		LCD_Write_DATA(0x00, y2 >> 8);
		LCD_Write_DATA(0x00, y2);
		LCD_Write_COM(0x2c);
		break;

	case ILI9341_S4P:
		LCD_Write_COM(0x2A); //column
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2B); //page
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2C); //write
		break;

	case ILI9481:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case PCF8833:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case S1D19122:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case S6D0164:
		LCD_Write_COM_DATA(0x36, x2);
		LCD_Write_COM_DATA(0x37, x1);
		LCD_Write_COM_DATA(0x38, y2);
		LCD_Write_COM_DATA(0x39, y1);
		LCD_Write_COM_DATA(0x20, x1);
		LCD_Write_COM_DATA(0x21, y1);
		LCD_Write_COM(0x22);
		break;

	case S6D1121_8:
	case S6D1121_16:
		LCD_Write_COM_DATA(0x46, (x2 << 8) | x1);
		LCD_Write_COM_DATA(0x47, y2);
		LCD_Write_COM_DATA(0x48, y1);
		LCD_Write_COM_DATA(0x20, x1);
		LCD_Write_COM_DATA(0x21, y1);
		LCD_Write_COM(0x22);
		break;

	case SSD1289:
	case SSD1289_8:
	case SSD1289LATCHED:
		LCD_Write_COM_DATA(0x44, (x2 << 8) + x1);
		LCD_Write_COM_DATA(0x45, y1);
		LCD_Write_COM_DATA(0x46, y2);
		LCD_Write_COM_DATA(0x4e, x1);
		LCD_Write_COM_DATA(0x4f, y1);
		LCD_Write_COM(0x22);
		break;

	case SSD1963_800:
		swap(word, x1, y1);
		swap(word, x2, y2);
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case SSD1963_480:
		swap(word, x1, y1);
		swap(word, x2, y2);
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case SSD1963_800ALT:
		swap(word, x1, y1);
		swap(word, x2, y2);
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case ST7735:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case ST7735S:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case ILI9486:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case R61581:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case HX8353C:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;

	case ILI9341_16:
		LCD_Write_COM(0x2a);
		LCD_Write_DATA(x1 >> 8);
		LCD_Write_DATA(x1);
		LCD_Write_DATA(x2 >> 8);
		LCD_Write_DATA(x2);
		LCD_Write_COM(0x2b);
		LCD_Write_DATA(y1 >> 8);
		LCD_Write_DATA(y1);
		LCD_Write_DATA(y2 >> 8);
		LCD_Write_DATA(y2);
		LCD_Write_COM(0x2c);
		break;
	}
}

void UTFT::clrXY()
{
	if (orient == PORTRAIT) {
		setXY(0, 0, disp_x_size, disp_y_size);
	} else {
		setXY(0, 0, disp_y_size, disp_x_size);
	}
}

void UTFT::drawRect(int x1, int y1, int x2, int y2)
{
	if (x1 > x2) {
		swap(int, x1, x2);
	}
	if (y1 > y2) {
		swap(int, y1, y2);
	}

	drawHLine(x1, y1, x2 - x1);
	drawHLine(x1, y2, x2 - x1);
	drawVLine(x1, y1, y2 - y1);
	drawVLine(x2, y1, y2 - y1);
}

void UTFT::drawRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1 > x2) {
		swap(int, x1, x2);
	}
	if (y1 > y2) {
		swap(int, y1, y2);
	}
	if ((x2 - x1) > 4 && (y2 - y1) > 4) {
		drawPixel(x1 + 1, y1 + 1);
		drawPixel(x2 - 1, y1 + 1);
		drawPixel(x1 + 1, y2 - 1);
		drawPixel(x2 - 1, y2 - 1);
		drawHLine(x1 + 2, y1, x2 - x1 - 4);
		drawHLine(x1 + 2, y2, x2 - x1 - 4);
		drawVLine(x1, y1 + 2, y2 - y1 - 4);
		drawVLine(x2, y1 + 2, y2 - y1 - 4);
	}
}

void UTFT::fillRect(int x1, int y1, int x2, int y2)
{
	if (x1 > x2) {
		swap(int, x1, x2);
	}
	if (y1 > y2) {
		swap(int, y1, y2);
	}
	if (display_transfer_mode == 16) {
		cbi(P_CS, B_CS);
		setXY(x1, y1, x2, y2);
		sbi(P_RS, B_RS);
		_fast_fill_16(fch, fcl, ((long(x2 - x1) + 1) * (long(y2 - y1) + 1)));
		sbi(P_CS, B_CS);
	} else if ((display_transfer_mode == 8) and (fch == fcl)) {
		cbi(P_CS, B_CS);
		setXY(x1, y1, x2, y2);
		sbi(P_RS, B_RS);
		_fast_fill_8(fch, ((long(x2 - x1) + 1) * (long(y2 - y1) + 1)));
		sbi(P_CS, B_CS);
	} else {
		if (orient == PORTRAIT) {
			for (int i = 0; i < ((y2 - y1) / 2) + 1; i++) {
				drawHLine(x1, y1 + i, x2 - x1);
				drawHLine(x1, y2 - i, x2 - x1);
			}
		} else {
			for (int i = 0; i < ((x2 - x1) / 2) + 1; i++) {
				drawVLine(x1 + i, y1, y2 - y1);
				drawVLine(x2 - i, y1, y2 - y1);
			}
		}
	}
}

void UTFT::fillRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1 > x2) {
		swap(int, x1, x2);
	}
	if (y1 > y2) {
		swap(int, y1, y2);
	}

	if ((x2 - x1) > 4 && (y2 - y1) > 4) {
		for (int i = 0; i < ((y2 - y1) / 2) + 1; i++) {
			switch (i) {
			case 0:
				drawHLine(x1 + 2, y1 + i, x2 - x1 - 4);
				drawHLine(x1 + 2, y2 - i, x2 - x1 - 4);
				break;
			case 1:
				drawHLine(x1 + 1, y1 + i, x2 - x1 - 2);
				drawHLine(x1 + 1, y2 - i, x2 - x1 - 2);
				break;
			default:
				drawHLine(x1, y1 + i, x2 - x1);
				drawHLine(x1, y2 - i, x2 - x1);
			}
		}
	}
}

void UTFT::drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;

	cbi(P_CS, B_CS);
	setXY(x, y + radius, x, y + radius);
	LCD_Write_DATA(fch, fcl);
	setXY(x, y - radius, x, y - radius);
	LCD_Write_DATA(fch, fcl);
	setXY(x + radius, y, x + radius, y);
	LCD_Write_DATA(fch, fcl);
	setXY(x - radius, y, x - radius, y);
	LCD_Write_DATA(fch, fcl);

	while (x1 < y1) {
		if (f >= 0) {
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;
		setXY(x + x1, y + y1, x + x1, y + y1);
		LCD_Write_DATA(fch, fcl);
		setXY(x - x1, y + y1, x - x1, y + y1);
		LCD_Write_DATA(fch, fcl);
		setXY(x + x1, y - y1, x + x1, y - y1);
		LCD_Write_DATA(fch, fcl);
		setXY(x - x1, y - y1, x - x1, y - y1);
		LCD_Write_DATA(fch, fcl);
		setXY(x + y1, y + x1, x + y1, y + x1);
		LCD_Write_DATA(fch, fcl);
		setXY(x - y1, y + x1, x - y1, y + x1);
		LCD_Write_DATA(fch, fcl);
		setXY(x + y1, y - x1, x + y1, y - x1);
		LCD_Write_DATA(fch, fcl);
		setXY(x - y1, y - x1, x - y1, y - x1);
		LCD_Write_DATA(fch, fcl);
	}
	sbi(P_CS, B_CS);
	clrXY();
}

void UTFT::fillCircle(int x, int y, int radius)
{
	for (int y1 = -radius; y1 <= 0; y1++) {
		for (int x1 = -radius; x1 <= 0; x1++) {
			if (x1 * x1 + y1 * y1 <= radius * radius) {
				drawHLine(x + x1, y + y1, 2 * (-x1));
				drawHLine(x + x1, y - y1, 2 * (-x1));
				break;
			}
		}
	}
}

void UTFT::clrScr()
{
	long i;

	cbi(P_CS, B_CS);
	clrXY();
	if (display_transfer_mode != 1) {
		sbi(P_RS, B_RS);
	}
	if (display_transfer_mode == 16) {
		_fast_fill_16(0, 0, ((disp_x_size + 1) * (disp_y_size + 1)));
	} else if (display_transfer_mode == 8) {
		_fast_fill_8(0, ((disp_x_size + 1) * (disp_y_size + 1)));
	} else {
		for (i = 0; i < ((disp_x_size + 1) * (disp_y_size + 1)); i++) {
			if (display_transfer_mode != 1) {
				LCD_Writ_Bus(0, 0, display_transfer_mode);
			} else {
				LCD_Writ_Bus(1, 0, display_transfer_mode);
				LCD_Writ_Bus(1, 0, display_transfer_mode);
			}
		}
	}
	sbi(P_CS, B_CS);
}

void UTFT::fillScr(byte r, byte g, byte b)
{
	word color = ((r & 248) << 8 | (g & 252) << 3 | (b & 248) >> 3);
	fillScr(color);
}

void UTFT::fillScr(word color)
{
	long i;
	char ch, cl;

	ch = byte(color >> 8);
	cl = byte(color & 0xFF);

	cbi(P_CS, B_CS);
	clrXY();
	if (display_transfer_mode != 1) {
		sbi(P_RS, B_RS);
	}
	if (display_transfer_mode == 16) {
		_fast_fill_16(ch, cl, ((disp_x_size + 1) * (disp_y_size + 1)));
	} else if ((display_transfer_mode == 8) and (ch == cl)) {
		_fast_fill_8(ch, ((disp_x_size + 1) * (disp_y_size + 1)));
	} else {
		for (i = 0; i < ((disp_x_size + 1) * (disp_y_size + 1)); i++) {
			if (display_transfer_mode != 1) {
				LCD_Writ_Bus(ch, cl, display_transfer_mode);
			} else {
				LCD_Writ_Bus(1, ch, display_transfer_mode);
				LCD_Writ_Bus(1, cl, display_transfer_mode);
			}
		}
	}
	sbi(P_CS, B_CS);
}

void UTFT::setColor(byte r, byte g, byte b)
{
	fch = ((r & 248) | g >> 5);
	fcl = ((g & 28) << 3 | b >> 3);
}

void UTFT::setColor(word color)
{
	fch = byte(color >> 8);
	fcl = byte(color & 0xFF);
}

word UTFT::getColor()
{
	return (fch << 8) | fcl;
}

void UTFT::setBackColor(byte r, byte g, byte b)
{
	bch = ((r & 248) | g >> 5);
	bcl = ((g & 28) << 3 | b >> 3);
	_transparent = false;
}

void UTFT::setBackColor(uint32_t color)
{
	if (color == VGA_TRANSPARENT) {
		_transparent = true;
	} else {
		bch = byte(color >> 8);
		bcl = byte(color & 0xFF);
		_transparent = false;
	}
}

word UTFT::getBackColor()
{
	return (bch << 8) | bcl;
}

void UTFT::setPixel(word color)
{
	LCD_Write_DATA((color >> 8), (color & 0xFF));	// rrrrrggggggbbbbb
}

void UTFT::drawPixel(int x, int y)
{
	cbi(P_CS, B_CS);
	setXY(x, y, x, y);
	setPixel((fch << 8) | fcl);
	sbi(P_CS, B_CS);
	clrXY();
}

void UTFT::drawLine(int x1, int y1, int x2, int y2)
{
	if (y1 == y2) {
		drawHLine(x1, y1, x2 - x1);
	} else if (x1 == x2) {
		drawVLine(x1, y1, y2 - y1);
	} else {
		unsigned int	dx = (x2 > x1 ? x2 - x1;
		short			xstep =  x2 > x1 ? 1;
		unsigned int	dy = (y2 > y1 ? y2 - y1;
		short			ystep =  y2 > y1 ? 1;
		int				col = x1, row = y1;

		cbi(P_CS, B_CS);
		if (dx < dy) {
			int t = - (dy >> 1);
			while (true) {
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);
				if (row == y2) {
					return;
				}
				row += ystep;
				t += dx;
				if (t >= 0) {
					col += xstep;
					t   -= dy;
				}
			}
		} else {
			int t = - (dx >> 1);
			while (true) {
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);
				if (col == x2) {
					return;
				}
				col += xstep;
				t += dy;
				if (t >= 0) {
					row += ystep;
					t   -= dx;
				}
			}
		}
		sbi(P_CS, B_CS);
	}
	clrXY();
}

void UTFT::drawHLine(int x, int y, int l)
{
	if (l < 0) {
		l = -l;
		x -= l;
	}
	cbi(P_CS, B_CS);
	setXY(x, y, x + l, y);
	if (display_transfer_mode == 16) {
		sbi(P_RS, B_RS);
		_fast_fill_16(fch, fcl, l);
	} else if ((display_transfer_mode == 8) and (fch == fcl)) {
		sbi(P_RS, B_RS);
		_fast_fill_8(fch, l);
	} else {
		for (int i = 0; i < l + 1; i++) {
			LCD_Write_DATA(fch, fcl);
		}
	}
	sbi(P_CS, B_CS);
	clrXY();
}

void UTFT::drawVLine(int x, int y, int l)
{
	if (l < 0) {
		l = -l;
		y -= l;
	}
	cbi(P_CS, B_CS);
	setXY(x, y, x, y + l);
	if (display_transfer_mode == 16) {
		sbi(P_RS, B_RS);
		_fast_fill_16(fch, fcl, l);
	} else if ((display_transfer_mode == 8) and (fch == fcl)) {
		sbi(P_RS, B_RS);
		_fast_fill_8(fch, l);
	} else {
		for (int i = 0; i < l + 1; i++) {
			LCD_Write_DATA(fch, fcl);
		}
	}
	sbi(P_CS, B_CS);
	clrXY();
}

void UTFT::printChar(byte c, int x, int y)
{
	byte i, ch;
	word j;
	word temp;

	cbi(P_CS, B_CS);

	if (!_transparent) {
		if (orient == PORTRAIT) {
			setXY(x, y, x + cfont.x_size - 1, y + cfont.y_size - 1);

			temp = ((c - cfont.offset) * ((cfont.x_size / 8) * cfont.y_size)) + 4;
			for (j = 0; j < ((cfont.x_size / 8)*cfont.y_size); j++) {
				ch = pgm_read_byte(&cfont.font[temp]);
				for (i = 0; i < 8; i++) {
					if ((ch & (1 << (7 - i))) != 0) {
						setPixel((fch << 8) | fcl);
					} else {
						setPixel((bch << 8) | bcl);
					}
				}
				temp++;
			}
		} else {
			temp = ((c - cfont.offset) * ((cfont.x_size / 8) * cfont.y_size)) + 4;

			for (j = 0; j < ((cfont.x_size / 8)*cfont.y_size); j += (cfont.x_size / 8)) {
				setXY(x, y + (j / (cfont.x_size / 8)), x + cfont.x_size - 1, y + (j / (cfont.x_size / 8)));
				for (int zz = (cfont.x_size / 8) - 1; zz >= 0; zz--) {
					ch = pgm_read_byte(&cfont.font[temp + zz]);
					for (i = 0; i < 8; i++) {
						if ((ch & (1 << i)) != 0) {
							setPixel((fch << 8) | fcl);
						} else {
							setPixel((bch << 8) | bcl);
						}
					}
				}
				temp += (cfont.x_size / 8);
			}
		}
	} else {
		temp = ((c - cfont.offset) * ((cfont.x_size / 8) * cfont.y_size)) + 4;
		for (j = 0; j < cfont.y_size; j++) {
			for (int zz = 0; zz < (cfont.x_size / 8); zz++) {
				ch = pgm_read_byte(&cfont.font[temp + zz]);
				for (i = 0; i < 8; i++) {

					if ((ch & (1 << (7 - i))) != 0) {
						setXY(x + i + (zz * 8), y + j, x + i + (zz * 8) + 1, y + j + 1);
						setPixel((fch << 8) | fcl);
					}
				}
			}
			temp += (cfont.x_size / 8);
		}
	}

	sbi(P_CS, B_CS);
	clrXY();
}

void UTFT::rotateChar(byte c, int x, int y, int pos, int deg)
{
	byte i, j, ch;
	word temp;
	int newx, newy;
	double radian;
	radian = deg * 0.0175;

	cbi(P_CS, B_CS);

	temp = ((c - cfont.offset) * ((cfont.x_size / 8) * cfont.y_size)) + 4;
	for (j = 0; j < cfont.y_size; j++) {
		for (int zz = 0; zz < (cfont.x_size / 8); zz++) {
			ch = pgm_read_byte(&cfont.font[temp + zz]);
			for (i = 0; i < 8; i++) {
				newx = x + (((i + (zz * 8) + (pos * cfont.x_size)) * cos(radian)) - ((j) * sin(radian)));
				newy = y + (((j) * cos(radian)) + ((i + (zz * 8) + (pos * cfont.x_size)) * sin(radian)));

				setXY(newx, newy, newx + 1, newy + 1);

				if ((ch & (1 << (7 - i))) != 0) {
					setPixel((fch << 8) | fcl);
				} else {
					if (!_transparent) {
						setPixel((bch << 8) | bcl);
					}
				}
			}
		}
		temp += (cfont.x_size / 8);
	}
	sbi(P_CS, B_CS);
	clrXY();
}

void UTFT::print(char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (orient == PORTRAIT) {
		if (x == RIGHT) {
			x = (disp_x_size + 1) - (stl * cfont.x_size);
		}
		if (x == CENTER) {
			x = ((disp_x_size + 1) - (stl * cfont.x_size)) / 2;
		}
	} else {
		if (x == RIGHT) {
			x = (disp_y_size + 1) - (stl * cfont.x_size);
		}
		if (x == CENTER) {
			x = ((disp_y_size + 1) - (stl * cfont.x_size)) / 2;
		}
	}

	for (i = 0; i < stl; i++)
		if (deg == 0) {
			printChar(*st++, x + (i * (cfont.x_size)), y);
		} else {
			rotateChar(*st++, x, y, i, deg);
		}
}

void UTFT::print(String st, int x, int y, int deg)
{
	char buf[st.length() + 1];

	st.toCharArray(buf, st.length() + 1);
	print(buf, x, y, deg);
}

void UTFT::printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	boolean neg = false;
	int c = 0, f = 0;

	if (num == 0) {
		if (length != 0) {
			for (c = 0; c < (length - 1); c++) {
				st[c] = filler;
			}
			st[c] = 48;
			st[c + 1] = 0;
		} else {
			st[0] = 48;
			st[1] = 0;
		}
	} else {
		if (num < 0) {
			neg = true;
			num = -num;
		}

		while (num > 0) {
			buf[c] = 48 + (num % 10);
			c++;
			num = (num - (num % 10)) / 10;
		}
		buf[c] = 0;

		if (neg) {
			st[0] = 45;
		}

		if (length > (c + neg)) {
			for (int i = 0; i < (length - c - neg); i++) {
				st[i + neg] = filler;
				f++;
			}
		}

		for (int i = 0; i < c; i++) {
			st[i + neg + f] = buf[c - i - 1];
		}
		st[c + neg + f] = 0;

	}

	print(st, x, y);
}

void UTFT::printNumF(double num, byte dec, int x, int y, char divider, int length, char filler)
{
	char st[27];
	boolean neg = false;

	if (dec < 1) {
		dec = 1;
	} else if (dec > 5) {
		dec = 5;
	}

	if (num < 0) {
		neg = true;
	}

	_convert_float(st, num, length, dec);

	if (divider != '.') {
		for (int i = 0; i < sizeof(st); i++)
			if (st[i] == '.') {
				st[i] = divider;
			}
	}

	if (filler != ' ') {
		if (neg) {
			st[0] = '-';
			for (int i = 1; i < sizeof(st); i++)
				if ((st[i] == ' ') || (st[i] == '-')) {
					st[i] = filler;
				}
		} else {
			for (int i = 0; i < sizeof(st); i++)
				if (st[i] == ' ') {
					st[i] = filler;
				}
		}
	}

	print(st, x, y);
}

void UTFT::setFont(uint8_t* font)
{
	cfont.font = font;
	cfont.x_size = fontbyte(0);
	cfont.y_size = fontbyte(1);
	cfont.offset = fontbyte(2);
	cfont.numchars = fontbyte(3);
}

uint8_t* UTFT::getFont()
{
	return cfont.font;
}

uint8_t UTFT::getFontXsize()
{
	return cfont.x_size;
}

uint8_t UTFT::getFontYsize()
{
	return cfont.y_size;
}

void UTFT::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;

	if (scale == 1) {
		if (orient == PORTRAIT) {
			cbi(P_CS, B_CS);
			setXY(x, y, x + sx - 1, y + sy - 1);
			for (tc = 0; tc < (sx * sy); tc++) {
				col = pgm_read_word(&data[tc]);
				LCD_Write_DATA(col >> 8, col & 0xff);
			}
			sbi(P_CS, B_CS);
		} else {
			cbi(P_CS, B_CS);
			for (ty = 0; ty < sy; ty++) {
				setXY(x, y + ty, x + sx - 1, y + ty);
				for (tx = sx - 1; tx >= 0; tx--) {
					col = pgm_read_word(&data[(ty * sx) + tx]);
					LCD_Write_DATA(col >> 8, col & 0xff);
				}
			}
			sbi(P_CS, B_CS);
		}
	} else {
		if (orient == PORTRAIT) {
			cbi(P_CS, B_CS);
			for (ty = 0; ty < sy; ty++) {
				setXY(x, y + (ty * scale), x + ((sx * scale) - 1), y + (ty * scale) + scale);
				for (tsy = 0; tsy < scale; tsy++)
					for (tx = 0; tx < sx; tx++) {
						col = pgm_read_word(&data[(ty * sx) + tx]);
						for (tsx = 0; tsx < scale; tsx++) {
							LCD_Write_DATA(col >> 8, col & 0xff);
						}
					}
			}
			sbi(P_CS, B_CS);
		} else {
			cbi(P_CS, B_CS);
			for (ty = 0; ty < sy; ty++) {
				for (tsy = 0; tsy < scale; tsy++) {
					setXY(x, y + (ty * scale) + tsy, x + ((sx * scale) - 1), y + (ty * scale) + tsy);
					for (tx = sx - 1; tx >= 0; tx--) {
						col = pgm_read_word(&data[(ty * sx) + tx]);
						for (tsx = 0; tsx < scale; tsx++) {
							LCD_Write_DATA(col >> 8, col & 0xff);
						}
					}
				}
			}
			sbi(P_CS, B_CS);
		}
	}
	clrXY();
}

void UTFT::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy)
{
	unsigned int col;
	int tx, ty, newx, newy;
	double radian;
	radian = deg * 0.0175;

	if (deg == 0) {
		drawBitmap(x, y, sx, sy, data);
	} else {
		cbi(P_CS, B_CS);
		for (ty = 0; ty < sy; ty++)
			for (tx = 0; tx < sx; tx++) {
				col = pgm_read_word(&data[(ty * sx) + tx]);

				newx = x + rox + (((tx - rox) * cos(radian)) - ((ty - roy) * sin(radian)));
				newy = y + roy + (((ty - roy) * cos(radian)) + ((tx - rox) * sin(radian)));

				setXY(newx, newy, newx, newy);
				LCD_Write_DATA(col >> 8, col & 0xff);
			}
		sbi(P_CS, B_CS);
	}
	clrXY();
}

void UTFT::lcdOff()
{
	cbi(P_CS, B_CS);
	switch (display_model) {
	case PCF8833:
		LCD_Write_COM(0x28);
		break;
	case CPLD:
		LCD_Write_COM_DATA(0x01, 0x0000);
		LCD_Write_COM(0x0F);
		break;
	}
	sbi(P_CS, B_CS);
}

void UTFT::lcdOn()
{
	cbi(P_CS, B_CS);
	switch (display_model) {
	case PCF8833:
		LCD_Write_COM(0x29);
		break;
	case CPLD:
		LCD_Write_COM_DATA(0x01, 0x0010);
		LCD_Write_COM(0x0F);
		break;
	}
	sbi(P_CS, B_CS);
}

void UTFT::setContrast(char c)
{
	cbi(P_CS, B_CS);
	switch (display_model) {
	case PCF8833:
		if (c > 64) { c = 64; }
		LCD_Write_COM(0x25);
		LCD_Write_DATA(c);
		break;
	}
	sbi(P_CS, B_CS);
}

int UTFT::getDisplayXSize()
{
	if (orient == PORTRAIT) {
		return disp_x_size + 1;
	} else {
		return disp_y_size + 1;
	}
}

int UTFT::getDisplayYSize()
{
	if (orient == PORTRAIT) {
		return disp_y_size + 1;
	} else {
		return disp_x_size + 1;
	}
}

void UTFT::setBrightness(byte br)
{
	cbi(P_CS, B_CS);
	switch (display_model) {
	case CPLD:
		if (br > 16) { br = 16; }
		LCD_Write_COM_DATA(0x01, br);
		LCD_Write_COM(0x0F);
		break;
	}
	sbi(P_CS, B_CS);
}

void UTFT::setDisplayPage(byte page)
{
	cbi(P_CS, B_CS);
	switch (display_model) {
	case CPLD:
		if (page > 7) { page = 7; }
		LCD_Write_COM_DATA(0x04, page);
		LCD_Write_COM(0x0F);
		break;
	}
	sbi(P_CS, B_CS);
}

void UTFT::setWritePage(byte page)
{
	cbi(P_CS, B_CS);
	switch (display_model) {
	case CPLD:
		if (page > 7) { page = 7; }
		LCD_Write_COM_DATA(0x05, page);
		LCD_Write_COM(0x0F);
		break;
	}
	sbi(P_CS, B_CS);
}
*/
