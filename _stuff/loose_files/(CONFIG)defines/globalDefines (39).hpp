/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	globalDefines.hpp
 * Module:		Global definitions for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

#ifndef __GLOBAL_DEFINES_HPP
#define __GLOBAL_DEFINES_HPP 1002

// =============================================================================
// Dependencies
// =============================================================================

// Project configuration file
#include "projectConfig.hpp"
#ifndef __PROJECT_CONFIG_HPP
#	error	[globalDefines.hpp] Error 100 - Project configuration file (projectConfig.hpp) is missing
#endif

// Header files - Basic header for all files
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if defined (_PLATFORM_ARM_)
#	include "stm32l0xx_hal.h"
#elif defined (_PLATFORM_AVR_)
#	include <avr/io.h>
#endif
// =============================================================================
// Undefining previous definitions
// =============================================================================

/* NONE */

// =============================================================================
// Constants
// =============================================================================

/* NONE */

// =============================================================================
// Macro-functions
// =============================================================================

// Bit handling
#define clrBit(reg, bit)                          ((reg) &= (~(1 << (bit))))
#define cplBit(reg, bit)                          ((reg) ^= (1 << (bit)))
#define setBit(reg, bit)                          ((reg) |= (1 << (bit)))
#define isBitClr(reg, bit)                        (bool)(!(((reg) >> (bit)) & 1))
#define isBitSet(reg, bit)                        (bool)(((reg) >> (bit)) & 0x01)
// Bit loop
#define waitUntilBitIsClear(reg, bit)             do {} while(isBitSet((reg), (bit)))
#define waitUntilBitIsSet(reg, bit)               do {} while(isBitClr((reg), (bit)))
// Bit mask handling
#define clrMask(reg, mask, offset)                ((reg) &= ~((mask) << (offset)))
#define cplMask(reg, mask, offset)                ((reg) ^= ((mask) << (offset)))
#define setMask(reg, mask, offset)                ((reg) |= ((mask) << (offset)))
// Bit data manipulation
#define byteHighNibble(dataByte)                  ((dataByte >> 4) & 0x0F)
#define byteLowNibble(dataByte)                   (dataByte & 0x0F)
#define byteSwitchNibbles(dataByte)               (byteLowNibble(dataByte) << 4) | byteHighNibble(dataByte)
#define wordHighByte(dataWord)                    ((dataWord >> 8) & 0x00FF)
#define wordLowByte(dataWord)                     (dataWord & 0x00FF)
#define wordSwitchBytes(dataWord)                 (wordLowByte(dataWord) << 8) | wordHighByte(dataWord)
// Value comparison
#define maximumOf(var1 , var2)                    ((var1 > var2) ? var1 :var2)
#define truncateBetween(var, minimum, maximum)    (var < minimum) ? minimum : ((var > maximum) ? maximum : var))
#define minimumOf(var1 , var2)                    ((var1 < var2) ? var1 :var2)
// Stringification of macro arguments
#define stringifyName(constant)                   #constant
#define stringifyValue(constant)                  stringifyName(constant)
// System actions
#define die(errorCode)                            do {printf("Failed with error=%u.\r", errorCode);systemHalt();} while(0)
#define systemHalt()                              while(1) {}

// =============================================================================
// New data types
// =============================================================================

// Single variables => shortening the name of single data types
typedef volatile uint8_t                          reg_t;
typedef volatile int8_t                           vint8_t;
typedef volatile int16_t                          vint16_t;
typedef volatile int32_t                          vint32_t;
typedef volatile int64_t                          vint64_t;
typedef volatile uint8_t                          vuint8_t;
typedef volatile uint16_t                         vuint16_t;
typedef volatile uint32_t                         vuint32_t;
typedef volatile uint64_t                         vuint64_t;

// Single variables => enumerations understood as logic values

//typedef enum bool {
//	false	= 0,
//	true	= 1
//} bool;

//typedef enum logicLevel_t {
//	LOW		= 0,
//	HIGH	= 1,
//} logicLevel_t;

//typedef enum activation_t {
//	OFF		= 0,
//	ON		= 1
//} activation_t;

//typedef enum dataFlow_t {
//	IN	= 0,
//	OUT	= 1
//} dataFlow_t;

//typedef enum rotacionalDirection_t {
//	CLOCKWISE			= 0,
//	COUNTERCLOCKWISE	= 1
//} rotacionalDirection_t;

//typedef enum edge_t {
//	FALLING_EDGE	= 0,
//	RISING_EDGE		= 1
//} edge_t;

//typedef enum displayType_t {
//	DISPLAY_COMMON_ANODE	= 0,
//	DISPLAY_COMMON_CATHODE	= 1
//} displayType_t;

// Single variables => small enumerations that can be understood as a single variable
typedef enum axis_t {
	AXIS_NONE	= 0,
	AXIS_X		= 1,
	AXIS_Y		= 2,
	AXIS_XY		= 3,
	AXIS_Z		= 4,
	AXIS_XZ		= 5,
	AXIS_YZ		= 6,
	AXIS_ALL	= 7
} axis_t;

//typedef enum planarDirection_t {
//	LEFT	= 0,
//	RIGHT	= 1,
//	UP		= 2,
//	DOWN	= 3
//} planarDirection_t;


//typedef enum outputDriver_t {
//	PUSH_PULL	= 0,
//	OPEN_DRAIN	= 1,
//} outputDriver_t;

//typedef enum twiOperation_t {
//	TWI_WRITE			= 0,
//	TWI_READ			= 1,
//	TWI_SET_THEN_READ	= 2
//} twiOperation_t;

// Enumerations => large enumerations or enumerations with expansible number of elements
typedef enum error_e {
	// Generic identifiers
	ERROR_NONE                                    = 0, // Success - No errors occurred
	ERROR_UNKNOWN,                                     // Unknown error - must not use!
	ERROR_BUFFER_SIZE_ZERO,                            // Buffer size was set to zero
	ERROR_BUFFER_NULL_POINTER,                         // Buffer size was set to zero
	ERROR_BUFFER_SIZE_TOO_SMALL,                       // Buffer size was set to a very small value
	ERROR_BUFFER_FULL,                                 // Buffer is full
	ERROR_BUFFER_EMPTY,                                // Buffer is empty
	ERROR_MEMORY_ALLOCATION,                           // Memory allocation failed
	ERROR_NOT_INITIALIZED,                             // Not initialized
	ERROR_ARGUMENT_MISMATCH,                           // A function was called with dubious argument values
	ERROR_LOCKED,                                      // Accessed a locked device
	ERROR_READ_PROTECTED,                              // Tried to read a read protected device
	ERROR_WRITE_PROTECTED,                             // Tried to write a write protected device
	ERROR_CLOCK_PRESCALER_UNSUPPORTED,                 // System clock prescaler configuration unsupported
	ERROR_CLOCK_PRESCALER_CHANGE_FAILED,               // System clock prescaler change failed
	ERROR_DATE_INVALID,
	ERROR_TIME_INVALID,
	ERROR_PACKAGE_API_INVALID_SOURCE_ADDRESS,
	ERROR_PACKAGE_API_PACKAGE_NOT_READY,
//	ERROR_PORT_OUTPUT_MODE_UNSUPPORTED						= 30,	// Int0 output port mode configuration unsupported
//	ERROR_PORT_SENSE_MODE_UNSUPPORTED						= 31,	// Int0 sense mode configuration unsupported
//	ERROR_TIMER_MODE_UNSUPPORTED							= 81,	// Timer0 mode unsupported
//	ERROR_TIMER_OUTPUT_COMPARE_MODE_UNSUPPORTED				= 82,	// Timer0 output pin configuration unsupported
//	ERROR_TIMER_CAPTURE_EDGE_UNSUPPORTED					= 93,	// Timer1 invalid edge section on capture mode
//	ERROR_TIMER_CAPTURE_NOISE_CANCELER_MODE_UNSUPPORTED		= 94,	// Timer0 invalid noise canceler configuration
//	ERROR_ADC_REFERENCE_SELECTION_INVALID					= 110,	// ADC reference selection unsupported
//	ERROR_ADC_MODE_SELECTION_INVALID						= 112,	// ADC mode unsupported
//	ERROR_ADC_CHANNEL_INVALID								= 113,	// ADC invalid channel selection
//	ERROR_ADC_DATA_PRESENTATION_INVALID						= 114,	// ADC invalid data presentation order
//	ERROR_USART_STOP_BIT_INVALID							= 120,	// USART invalid number of stop bits
//	ERROR_USART_PARITY_INVALID								= 121,	// USART invalid parity selection
//	ERROR_USART_DATA_BITS_INVALID							= 122,	// USART invalid number of data bits
//	ERROR_USART_MODE_INVALID								= 123,	// USART unsupported mode
//	ERROR_USART_BAUD_RATE_INVALID							= 124,	// USART baud rate value off limits
//	ERROR_USART_FRAME_FORMAT_INVALID						= 125,	// USART unsupported frame format
} error_e;
/*
typedef enum twiState_e {
	TWI_START					= 0x08,	// START has been transmitted
	TWI_REP_START				= 0x10,	// Repeated START has been transmitted
	TWI_ARB_LOST				= 0x38,	// Arbitration lost
	TWI_MTX_ADR_ACK				= 0x18,	// SLA+W has been transmitted and ACK received
	TWI_MTX_ADR_NACK			= 0x20,	// SLA+W has been transmitted and NACK received
	TWI_MTX_DATA_ACK			= 0x28,	// Data byte has been transmitted and ACK received
	TWI_MTX_DATA_NACK			= 0x30,	// Data byte has been transmitted and NACK received
	TWI_MRX_ADR_ACK				= 0x40,	// SLA+R has been transmitted and ACK received
	TWI_MRX_ADR_NACK			= 0x48,	// SLA+R has been transmitted and NACK received
	TWI_MRX_DATA_ACK			= 0x50,	// Data byte has been received and ACK transmitted
	TWI_MRX_DATA_NACK			= 0x58,	// Data byte has been received and NACK transmitted
	TWI_STX_ADR_ACK				= 0xA8,	// Own SLA+R has been received; ACK has been returned
	TWI_STX_ADR_ACK_M_ARB_LOST	= 0xB0,	// Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
	TWI_STX_DATA_ACK			= 0xB8,	// Data byte in TWDR has been transmitted; ACK has been received
	TWI_STX_DATA_NACK			= 0xC0,	// Data byte in TWDR has been transmitted; NOT ACK has been received
	TWI_STX_DATA_ACK_LAST_BYTE	= 0xC8,	// Last data byte in TWDR has been transmitted (TWEA = 0); ACK has been received
	TWI_SRX_ADR_ACK				= 0x60,	// Own SLA+W has been received ACK has been returned
	TWI_SRX_ADR_ACK_M_ARB_LOST	= 0x68,	// Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
	TWI_SRX_GEN_ACK				= 0x70,	// General call address has been received; ACK has been returned
	TWI_SRX_GEN_ACK_M_ARB_LOST	= 0x78,	// Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
	TWI_SRX_ADR_DATA_ACK		= 0x80,	// Previously addressed with own SLA+W; data has been received; ACK has been returned
	TWI_SRX_ADR_DATA_NACK		= 0x88,	// Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
	TWI_SRX_GEN_DATA_ACK		= 0x90,	// Previously addressed with general call; data has been received; ACK has been returned
	TWI_SRX_GEN_DATA_NACK		= 0x98,	// Previously addressed with general call; data has been received; NOT ACK has been returned
	TWI_SRX_STOP_RESTART		= 0xA0,	// A STOP condition or repeated START condition has been received while still addressed as Slave
	TWI_NO_STATE				= 0xF8,	// No relevant state information available; TWINT = 0
	TWI_BUS_ERROR				= 0x00	// Bus error due to an illegal START or STOP condition
} twiState_e;
*/

// New Data Type - Enumerations => parametric table that doesn't mantain value equivalence with datasheet
//typedef enum sleepMode_e {
//	SLEEP_MODE_NO_CHANGE	= 0,	// Keep last sleep mode
//	SLEEP_MODE_IDLE			= 1,	// Sleep mode IDLE
//	SLEEP_MODE_ADC			= 2,	// Sleep mode for ADC noise reduction
//	SLEEP_MODE_PWR_DOWN		= 3,	// Sleep mode POWER DOWN
//	SLEEP_MODE_PWR_SAVE		= 4,	// Sleep mode POWER SAVR
//	SLEEP_MODE_STANDBY		= 5,	// Sleep mode STANDBY
//	SLEEP_MODE_EXT_STANDBY	= 6		// Sleep mode EXTENDED STANDBY
//} sleepMode_e;

//typedef enum timerOutput_e {
//	TIMER_PORT_NO_CHANGE			= 0,	// Keep last port configuration
//	TIMER_PORT_NORMAL				= 1,	// Output Compare Pin disconnected; normal GPIO operation
//	TIMER_PORT_TOGGLE_ON_COMPARE	= 2,	// Toggle pin value on compare
//	TIMER_PORT_CLEAR_ON_COMPARE		= 3,	// Clear pin on compare
//	TIMER_PORT_SET_ON_COMPARE		= 4,	// Set pin on compare
//	TIMER_PORT_NON_INVERTING_MODE	= 5,	// PWM non-inverting mode
//	TIMER_PORT_INVERTING_MODE		= 6		// PWM inverting mode
//} timerOutput_e;

//typedef enum portMode_e {
//	PORT_NO_CHANGE		= 0,		// Keep last pin configurations
//	PORT_OUTPUT_LOW		= 1,		// Pin configured as OUTPUT, LOW logic level
//	PORT_OUTPUT_HIGH	= 2,		// Pin configured as OUTPUT, HIGH logic level
//	PORT_INPUT_TRISTATE	= 3,		// Pin configured as INPUT, PULL-UPs off
//	PORT_INPUT_PULL_UP	= 4			// Pin configured as INPUT, PULL-UPs on
//} portMode_e;
/*
typedef enum lcdSize_e {
	LCD_SIZE_UNDEFINED	= 0,		// Used to verify if the variable was previously initialized
	LCD_8X1				= 108,		// LCD 8 columns x 1 line
	LCD_8X2				= 208,		// LCD 8 columns x 2 line
	LCD_10x2			= 210,		// LCD 10 columns x 2 lines
	LCD_12X2			= 212,		// LCD 12 columns x 2 lines
	LCD_12X4			= 412,		// LCD 12 columns x 4 lines
	LCD_16X1			= 116,		// LCD 16 columns x 1 line
	LCD_16X2			= 216,		// LCD 16 columns x 2 lines
	LCD_16X4			= 416,		// LCD 16 columns x 4 lines
	LCD_20X1			= 120,		// LCD 20 columns x 1 line
	LCD_20X2			= 220,		// LCD 20 columns x 2 lines
	LCD_20X4			= 420,		// LCD 20 columns x 4 lines
	LCD_24X1			= 124,		// LCD 24 columns x 1 line
	LCD_24X2			= 224,		// LCD 24 columns x 2 lines
	LCD_30X2			= 230,		// LCD 30 columns x 2 lines
	LCD_32X2			= 232,		// LCD 32 columns x 2 lines
	LCD_40X1			= 140,		// LCD 40 columns x 1 line
	LCD_40X2			= 240		// LCD 20 columns x 2 lines
} lcdSize_e;

typedef enum lcdFont_e {
	LCD_FONT_NO_CHANGE		= 0,	// Keep last value configured
	LCD_FONT_5X8			= 1,	// Use font 5x8 pixels
	LCD_FONT_5X10			= 2		// Use font 5x8 pixels
} lcdFont_e;

typedef enum lcdDisplayMode_e {
	LCD_DISPLAY_MODE_NO_CHANGE	= 0,	// Keep last mode configured
	LCD_DISPLAY_MODE_OVERWRITE	= 1,	// Overwrite mode
	LCD_DISPLAY_MODE_SHIFT		= 2		// Insert mode (shift display)
} lcdDisplayMode_e ;

typedef enum lcdDisplayDirection_e {
	LCD_DISPLAY_DIRECTION_NO_CHANGE		= 0,	// Keep last value configured
	LCD_DISPLAY_DIRECTION_DECREMENT		= 1,	// Decrement address after character write
	LCD_DISPLAY_DIRECTION_INCREMENT		= 2		// Increment address after character write
} lcdDisplayDirection_e;

// New Data Type - Flags => enumerations that can be combined in the form of flags
typedef enum powerReductionFlags_f {
	PRR_NONE		= 0,	// Keep last value configured
	PRR_ADC			= 1,	// Keep last value configured
	PRR_USART0		= 2,	// Keep last value configured
	PRR_SPI			= 4,	// Keep last value configured
	PRR_TIM0		= 8,	// Keep last value configured
	PRR_TIM1		= 16,	// Keep last value configured
	PRR_TIM2		= 32,	// Keep last value configured
	PRR_PRTWI		= 64	// Keep last value configured
} powerReductionFlags_f;
*/

#if defined (_PLATFORM_ARM_)
#	define i2cHandler	I2C_HandleTypeDef
#elif defined (_PLATFORM_AVR_)
#	define i2cHandler	void
#endif

#endif
