/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Integrated Library
 * File:			globalDefines.h
 * Module:			Global definitions file for the GPDSE AVR8 Project
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * ---------------------------------------------------------------------------*/

#ifndef __GLOBAL_DEFINES_H
#define __GLOBAL_DEFINES_H 1

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "pinout.h"
#if __PINOUT_H != 1
	#error Error 100 - pinout.h - wrong build (pinout must be build 1).
#endif
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#define __DEBUG_MODE_ON 1	// Enables function for debug. If changed, the
// library must be recompiled before first use

// -----------------------------------------------------------------------------
// Bit handling macro functions ------------------------------------------------

#ifndef setBit
	#define setBit(reg, bit)					((reg) |= (1 << (bit)))
#endif
#ifndef clrBit
	#define clrBit(reg, bit)					((reg) &= ~(1 << (bit)))
#endif
#ifndef cplBit
	#define cplBit(reg, bit)					((reg) ^= (1 << (bit)))
#endif
#ifndef isBitSet
	#define isBitSet(reg, bit)					(((reg) >> (bit)) & 1)
#endif
#ifndef isBitClr
	#define isBitClr(reg, bit)					(!(((reg) >> (bit)) & 1))
#endif
#ifndef waitUntilBitIsSet
	#define waitUntilBitIsSet(reg, bit)			do{}while(isBitClr((reg), (bit)))
#endif
#ifndef waitUntilBitIsClear
	#define waitUntilBitIsClear(reg, bit)		do{}while(isBitSet((reg), (bit)))
#endif
#ifndef setMask
	#define setMask(reg, mask, offset)			((reg) |= ((mask) << (offset)))
#endif
#ifndef clrMask
	#define clrMask(reg, mask, offset)			((reg) &= ~((mask) << (offset)))
#endif
#ifndef cplMask
	#define cplMask(reg, mask, offset)			((reg) ^= ((mask) << (offset)))
#endif

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef volatile uint8_t	reg_t;
typedef volatile int8_t		vint8_t;
typedef volatile int16_t	vint16_t;
typedef volatile int32_t	vint32_t;
typedef volatile int64_t	vint64_t;
typedef volatile uint8_t	vuint8_t;
typedef volatile uint16_t	vuint16_t;
typedef volatile uint32_t	vuint32_t;
typedef volatile uint64_t	vuint64_t;

typedef enum bool_t {
	FALSE	= 0,
	TRUE	= 1
} bool_t;

typedef enum logic_t {
	LOW		= 0,
	HIGH	= 1,
	OFF		= 0,
	ON		= 1
} logic_t;

typedef enum direction_t {
	LEFT	= 0,
	RIGHT	= 1,
	UP		= 0,
	DOWN	= 1
} direction_t;

typedef enum rotacionalDirection_t {
	CLOCKWISE			= 0,
	COUNTERCLOCKWISE	= 1
} rotacionalDirection_t;

typedef enum edge_t {
	FALLING_EDGE	= 0,
	RISING_EDGE		= 1
} edge_t;

typedef enum displayType_t {
	DISPLAY_COMMON_ANODE	= 0,
	DISPLAY_COMMON_CATHODE	= 1
} displayType_t;

typedef enum weekDay_t {
	WEEKDAY_UNDEFINED	= 0,
	WEEKDAY_SUNDAY		= 1,
	WEEKDAY_MONDAY		= 2,
	WEEKDAY_TUESDAY		= 3,
	WEEKDAY_WEDNESDAY	= 4,
	WEEKDAY_THURSDAY	= 5,
	WEEKDAY_FRIDAY		= 6,
	WEEKDAY_SATURDAY	= 7
} weekDay_t;

typedef enum month_t {
	MONTH_UNDEFINED	= 0,
	MONTH_JANUARY	= 1,
	MONTH_FEBRUARY	= 2,
	MONTH_MARCH		= 3,
	MONTH_APRIL		= 4,
	MONTH_MAY		= 5,
	MONTH_JUNE		= 6,
	MONTH_JULY		= 7,
	MONTH_AUGUST	= 8,
	MONTH_SEPTEMBER	= 9,
	MONTH_OCTOBER	= 10,
	MONTH_NOVEMBER	= 11,
	MONTH_DECEMBER	= 12
} month_t;

typedef enum resultValue_t {
	RESULT_OK										= 0,
	RESULT_UNSUPPORTED_SYSTEM_PRESCALER_VALUE		,
	RESULT_UNSUPPORTED_ADC_REFERENCE				,
	RESULT_UNSUPPORTED_ADC_PRESCALER_VALUE			,
	RESULT_UNSUPPORTED_ADC_MODE						,
	RESULT_UNSUPPORTED_ADC_CHANNEL					,
	RESULT_UNSUPPORTED_ADC_ADJUST_RESULT			,
	RESULT_UNSUPPORTED_USART_STOP_BITS				,
	RESULT_UNSUPPORTED_USART_PARITY					,
	RESULT_UNSUPPORTED_USART_DATA_BITS				,
	RESULT_UNSUPPORTED_USART_MODE					,
	RESULT_UNSUPPORTED_USART_BAUD_RATE				,			// Not implemented yet
	RESULT_UNSUPPORTED_TIMER_PORT_CONFIG			,
	RESULT_UNSUPPORTED_TIMER0_PRESCALER_VALUE		,
	RESULT_UNSUPPORTED_TIMER0_MODE					,
	RESULT_UNSUPPORTED_TIMER1_PRESCALER_VALUE		,
	RESULT_UNSUPPORTED_TIMER1_MODE					,
	RESULT_UNSUPPORTED_TIMER1_INPUT_CAPTURE_MODE	,
	RESULT_UNSUPPORTED_TIMER2_PRESCALER_VALUE		,
	RESULT_UNSUPPORTED_TIMER2_MODE					,
	///////////////////////////////// MUST BE REMOVED
	RESULT_PORT_VALUE_UNSUPPORTED					,
	RESULT_INT_SENSE_VALUE_UNSUPPORTED				,
	RESULT_TIMER_PRESCALER_VALUE_UNSUPPORTED		,
	RESULT_TIMER_MODE_VALUE_UNSUPPORTED				,
	RESULT_USART_BAUD_RATE_UNSUPORTED				,
	RESULT_UNSUPPORTED_VALUE
} resultValue_t;

typedef enum systemPrescalerValue_t {
	SYSTEM_PRESCALER_1		= 0,
	SYSTEM_PRESCALER_2		,
	SYSTEM_PRESCALER_4		,
	SYSTEM_PRESCALER_8		,
	SYSTEM_PRESCALER_16		,
	SYSTEM_PRESCALER_32		,
	SYSTEM_PRESCALER_64		,
	SYSTEM_PRESCALER_128	,
	SYSTEM_PRESCALER_256
} systemPrescalerValue_t;

typedef enum timerOutput_t {
	TIMER_PORT_NORMAL				= 0,
	TIMER_PORT_TOGGLE_ON_COMPARE	,
	TIMER_PORT_CLEAR_ON_COMPARE		,
	TIMER_PORT_SET_ON_COMPARE		,
	TIMER_PORT_NON_INVERTING_MODE	,
	TIMER_PORT_INVERTING_MODE		,
	TIMER_PORT_NO_CHANGE			= 255
} timerOutput_t;

typedef enum portMode_t {
	PORT_OUTPUT_LOW		= 0,
	PORT_OUTPUT_HIGH	,
	PORT_INPUT_TRISTATE	,
	PORT_INPUT_PULL_UP	,
	PORT_NO_CHANGE		= 255
} portMode_t;

typedef volatile struct systemConfiguration_t {
	uint32_t	systemClock;
	uint32_t	clocksPerMs;
} systemConfiguration_t;

// -----------------------------------------------------------------------------
// Extern Global Variables -----------------------------------------------------

extern systemConfiguration_t systemConfiguration;

// -----------------------------------------------------------------------------
// Public Functions Declarations ------------------------------------------------

void systemClock(uint32_t clock);

// -----------------------------------------------------------------------------
// External Functions Declarations ---------------------------------------------

extern void __builtin_avr_delay_cycles(unsigned long);

// -----------------------------------------------------------------------------
// TWI module configuration
// -----------------------------------------------------------------------------

#ifndef TWI_GENERAL_CALL_ADDRESS
	#define TWI_GENERAL_CALL_ADDRESS	0x00
#endif
#ifndef TWI_BUFFER_SIZE
	#define TWI_BUFFER_SIZE				10
#endif

typedef enum twiGeneralCallEnable_t {
	TWI_DISABLE_GENERAL_CALL	= 0,
	TWI_ENABLE_GENERAL_CALL		= 1
} twiGeneralCallEnable_t;

typedef enum twiOperation_t {
	TWI_WRITE	= 0,
	TWI_READ	= 1
} twiOperation_t;

typedef enum twiState_t {
	TWI_START					= 0x08,	// START has been transmitted
	TWI_REP_START				= 0x10,	// Repeated START has been transmitted
	TWI_ARB_LOST				= 0x38,	// Arbitration lost
	TWI_MTX_ADR_ACK				= 0x18,	// SLA+W has been tramsmitted and ACK received
	TWI_MTX_ADR_NACK			= 0x20,	// SLA+W has been tramsmitted and NACK received
	TWI_MTX_DATA_ACK			= 0x28,	// Data byte has been tramsmitted and ACK received
	TWI_MTX_DATA_NACK			= 0x30,	// Data byte has been tramsmitted and NACK received
	TWI_MRX_ADR_ACK				= 0x40,	// SLA+R has been tramsmitted and ACK received
	TWI_MRX_ADR_NACK			= 0x48,	// SLA+R has been tramsmitted and NACK received
	TWI_MRX_DATA_ACK			= 0x50,	// Data byte has been received and ACK tramsmitted
	TWI_MRX_DATA_NACK			= 0x58,	// Data byte has been received and NACK tramsmitted
	TWI_STX_ADR_ACK				= 0xA8,	// Own SLA+R has been received; ACK has been returned
	TWI_STX_ADR_ACK_M_ARB_LOST	= 0xB0,	// Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
	TWI_STX_DATA_ACK			= 0xB8,	// Data byte in TWDR has been transmitted; ACK has been received
	TWI_STX_DATA_NACK			= 0xC0,	// Data byte in TWDR has been transmitted; NOT ACK has been received
	TWI_STX_DATA_ACK_LAST_BYTE	= 0xC8,	// Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received
	TWI_SRX_ADR_ACK				= 0x60,	// Own SLA+W has been received ACK has been returned
	TWI_SRX_ADR_ACK_M_ARB_LOST	= 0x68,	// Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
	TWI_SRX_GEN_ACK				= 0x70,	// General call address has been received; ACK has been returned
	TWI_SRX_GEN_ACK_M_ARB_LOST	= 0x78,	// Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
	TWI_SRX_ADR_DATA_ACK		= 0x80,	// Previously addressed with own SLA+W; data has been received; ACK has been returned
	TWI_SRX_ADR_DATA_NACK		= 0x88,	// Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
	TWI_SRX_GEN_DATA_ACK		= 0x90,	// Previously addressed with general call; data has been received; ACK has been returned
	TWI_SRX_GEN_DATA_NACK		= 0x98,	// Previously addressed with general call; data has been received; NOT ACK has been returned
	TWI_SRX_STOP_RESTART		= 0xA0,	// A STOP condition or repeated START condition has been received while still addressed as Slave
	TWI_NO_STATE				= 0xF8,	// No relevant state information available; TWINT = "0"
	TWI_BUS_ERROR				= 0x00	// Bus error due to an illegal START or STOP condition
} twiState_t;

// -----------------------------------------------------------------------------
// New Data Types to LCD modules
// -----------------------------------------------------------------------------

typedef enum lcdSize_t {
	LCD_SIZE_UNDEFINED = 0,
	LCD_8X1		= 108,
	LCD_8X2		= 208,
	LCD_10x2	= 210,
	LCD_12X2	= 212,
	LCD_12X4	= 412,
	LCD_16X1	= 116,
	LCD_16X2	= 216,
	LCD_16X4	= 416,
	LCD_20X1	= 120,
	LCD_20X2	= 220,
	LCD_20X4	= 420,
	LCD_24X1	= 124,
	LCD_24X2	= 224,
	LCD_30X2	= 230,
	LCD_32X2	= 232,
	LCD_40X1	= 140,
	LCD_40X2	= 240
} lcdSize_t;

typedef enum lcdFont_t {
	LCD_FONT_5X8 = 0,
	LCD_FONT_5X10 = 1
} lcdFont_t;

typedef enum lcdShiftOverwrite_t {
	LCD_DISPLAY_OVERWRITE = 0,
	LCD_DISPLAY_SHIFT = 1,
	LCD_DISPLAY_NO_CHANGE = 0xFF
} lcdShiftOverwrite_t;

typedef enum lcdIncrementDecrement_t {
	LCD_DECREMENT = 0,
	LCD_INCREMENT = 1,
	LCD_INCDEC_NO_CHANGE = 0xFF
} lcdIncrementDecrement_t;

#endif
