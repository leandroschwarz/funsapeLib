
#ifndef __ILI9325D_HPP
#define __ILI9325D_HPP

#include "stm32f1xx.h"
#include "main.h"
#include "spi.h"
#include "gpio.h"

#define LCD_CS_PIN						LCD_CS_Pin
#define LCD_CS_PORT						LCD_CS_GPIO_Port
#define LCD_RS_PIN						LCD_RS_Pin
#define LCD_RS_PORT						LCD_RS_GPIO_Port
#define LCD_RST_PIN						LCD_RST_Pin
#define LCD_RST_PORT					LCD_RST_GPIO_Port
#define LCD_WR_PIN						LCD_WR_Pin
#define LCD_WR_PORT						LCD_WR_GPIO_Port
#define LCD_DATA_PORT					LCD_DATA_0_GPIO_Port

#define PORTRAIT						0
#define LANDSCAPE						1

#define HSPI_INSTANCE					&hspi1

#define VGA_BLACK						0x0000
#define VGA_WHITE						0xFFFF
#define VGA_RED							0xF800
#define VGA_GREEN						0x0400
#define VGA_BLUE						0x001F
#define VGA_SILVER						0xC618
#define VGA_GRAY						0x8410
#define VGA_MAROON						0x8000
#define VGA_YELLOW						0xFFE0
#define VGA_OLIVE						0x8400
#define VGA_LIME						0x07E0
#define VGA_AQUA						0x07FF
#define VGA_TEAL						0x0410
#define VGA_NAVY						0x0010
#define VGA_FUCHSIA						0xF81F
#define VGA_PURPLE						0x8010

#define setBit(reg, bit)				((reg) |= (1 << bit))
#define clrBit(reg, bit)				((reg) &= ~(1 << bit))
#define pulseLow(reg, bit)				do{				\
		setBit(reg->ODR, bit);							\
		clrBit(reg->ODR, bit);							\
		setBit(reg->ODR, bit);							\
	} while(0)
#define pulseHigh(reg, bit)				do{				\
		clrBit(reg->ODR, bit);							\
		setBit(reg->ODR, bit);							\
		clrBit(reg->ODR, bit);							\
	} while(0)
#define ioPortWrite(reg, value)			((reg->ODR) = (value))
#define ioPortRead(reg)					(reg->ODR)
#define ioPortMask(reg, value, mask, offset)	do {	\
		uint32_t aux32 = ioPortRead(reg);				\
		aux32 &= ~(mask << offset);						\
		aux32 |= ((value &mask) << offset);				\
		ioPortWrite(reg, aux32);						\
	} while(0)
#define swap(x, y)						do {			\
		x ^= y;											\
		y ^= x;											\
		x ^= y;											\
	} while(0)

#define ILI9341_SCREEN_HEIGHT			240
#define ILI9341_SCREEN_WIDTH			320

#define SCREEN_VERTICAL_1				0
#define SCREEN_HORIZONTAL_1				1
#define SCREEN_VERTICAL_2				2
#define SCREEN_HORIZONTAL_2				3

#define BURST_MAX_SIZE					500

void LCD_Writ_Bus(uint8_t VH, uint8_t VL);
void LCD_Write_COM(char com1);
void LCD_Write_DATA(char VH, char VL);
void LCD_Write_COM_DATA(char com1, int dat1);
void setColor(uint16_t r, uint16_t g, uint16_t b);
void setBackColor(uint16_t r, uint16_t g, uint16_t b);
void drawPixel(uint16_t x, uint16_t y);
void setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void clrXY(void);
void clrScr(void);
void _fast_fill_16(uint16_t ch, uint16_t cl, uint32_t pix);
void _fast_fill_8(uint16_t ch, uint32_t pix);
void init(void);
void setPixel(uint16_t color);

#endif
