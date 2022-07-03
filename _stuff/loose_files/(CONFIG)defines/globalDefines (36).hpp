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
#ifndef GPDSE_PROJECT_CONFIG_HPP
#	error	[globalDefines.hpp] Error 100 - Project configuration file (projectConfig.hpp) is missing
#endif

// Header files - Basic header for all files
#include "stddef.h"
#include <stdint.h>
#include <stdlib.h>

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
#define byteSwitchNibbles(dataByte)               ((byteLowNibble(dataByte) << 4) | byteHighNibble(dataByte))
#define wordHighByte(dataWord)                    ((dataWord >> 8) & 0x00FF)
#define wordLowByte(dataWord)                     (dataWord & 0x00FF)
#define wordSwitchBytes(dataWord)                 ((wordLowByte(dataWord) << 8) | wordHighByte(dataWord))
#define dwordByte3(dataDword)                     ((dataDword >> 24) & 0x000000FF)
#define dwordByte2(dataDword)                     ((dataDword >> 16) & 0x000000FF)
#define dwordByte1(dataDword)                     ((dataDword >> 8) & 0x000000FF)
#define dwordByte0(dataDword)                     (dataDword & 0x000000FF)
#define dwordSwitchBytes(dataWord)                (dwordByte0(dataWord) << 24) | dwordByte1(dataWord) << 16) | dwordByte2(dataWord) << 8) | dwordByte3(dataWord))
#define dwordHighWord(dataDword)                  ((dataDword >> 16) & 0x0000FFFF)
#define dwordLowWord(dataDword)                   (dataDword & 0x0000FFFF)
#define dwordSwitchWords(dataDword)               ((dwordLowWord(dataDword) << 8) | dwordHighWord(dataDword))
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

typedef enum edge_t {
	FALLING_EDGE	= 0,
	RISING_EDGE		= 1
} edge_t;

typedef enum logicLevel_t {
	LOW		= 0,
	HIGH	= 1,
} logicLevel_t;

typedef enum orientation_t {
	PORTRAIT	= 0,
	LANDSCAPE	= 1,
} orientation_t;

//typedef enum bool {
//	false	= 0,
//	true	= 1
//} bool;

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

//typedef enum displayType_t {
//	DISPLAY_COMMON_ANODE	= 0,
//	DISPLAY_COMMON_CATHODE	= 1
//} displayType_t;

// Single variables => small enumerations that can be understood as a single variable
typedef enum axis_t {
	//			   AXIS-Z	  AXIS-Y	 AXIS-X
	AXIS_NONE	= (0 << 2) | (0 << 1) | (0 << 0),
	AXIS_X		= (0 << 2) | (0 << 1) | (1 << 0),
	AXIS_Y		= (0 << 2) | (1 << 1) | (0 << 0),
	AXIS_XY		= (0 << 2) | (1 << 1) | (1 << 0),
	AXIS_Z		= (1 << 2) | (0 << 1) | (0 << 0),
	AXIS_XZ		= (1 << 2) | (0 << 1) | (1 << 0),
	AXIS_YZ		= (1 << 2) | (1 << 1) | (0 << 0),
	AXIS_ALL	= (1 << 2) | (1 << 1) | (1 << 0)
} axis_t;

typedef enum controlMode_t {
	CONTROL_POOLING		= 0,
	CONTROL_INTERRUPT	= 1
} controlMode_t;

typedef enum interface_t {
	INTERFACE_UNDEFINED					= 0,
	INTERFACE_PARALLEL_4_WIRES			,
	INTERFACE_PARALLEL_8_WIRES			,
	INTERFACE_PARALLEL_16_WIRES			,
	INTERFACE_PARALLEL_16_WIRES_LATCHED	,
	INTERFACE_SERIAL_1_WIRE				,
	INTERFACE_SERIAL_2_WIRES			,
	INTERFACE_SERIAL_3_WIRES			,
	INTERFACE_SERIAL_4_WIRES			,
	INTERFACE_SERIAL_5_WIRES			,
	INTERFACE_SPI						,
	INTERFACE_TWI						,
	INTERFACE_UART						,
	INTERFACE_USART						,
} interface_t;

//typedef enum planarDirection_t {
//	LEFT	= 0,
//	RIGHT	= 1,
//	UP		= 2,
//	DOWN	= 3
//} planarDirection_t;

typedef enum outputDriver_t {
	PUSH_PULL		= 0,
	OPEN_DRAIN		= 1,
} outputDriver_t;

//typedef enum twiOperation_t {
//	TWI_WRITE			= 0,
//	TWI_READ			= 1,
//	TWI_SET_THEN_READ	= 2
//} twiOperation_t;

// Enumerations => large enumerations or enumerations with expansible number of elements
typedef enum error_e {
	// Generic error identifiers
	ERROR_NONE                                    = 0,  // Success - No errors occurred
	ERROR_LOCKED                                  = 1,  // Accessed a locked device
	ERROR_MEMORY_ALLOCATION                       = 2,  // Memory allocation failed
	ERROR_NOT_INITIALIZED                         = 3,  // Not initialized
	ERROR_READ_PROTECTED                          = 4,  // Tried to read a read protected device
	ERROR_UNKNOWN                                 = 5,  // Something really unpredictable has happen
	ERROR_WRITE_PROTECTED                         = 6,  // Tried to write a write protected device
	ERROR_WRONG_CONFIGURATION                     = 7,
	ERROR_OPERATION_MODE_UNSUPPORTED              = 8,
	ERROR_OUTPUT_COMPARE_UNSUPPORTED              = 9,

	// Error in arguments
	ERROR_ARGUMENT_CANNOT_BE_ZERO                 = 10,
	ERROR_ARGUMENT_POINTER_NULL                   = 11, // NULL pointer was passed as an argument to function
	ERROR_ARGUMENTS_MISMATCH                      = 12, // A function was called with dubious argument values
	ERROR_ARGUMENTS_WRONG_NUMBER                  = 13, // A function was called with wrong number of arguments

	// Error in buffers
	ERROR_BUFFER_EMPTY                            = 20, // Buffer is empty
	ERROR_BUFFER_FULL                             = 21, // Buffer is full
	ERROR_BUFFER_NOT_ENOUGH_ELEMENTS              = 22, // Not enough space in buffer to perform operation
	ERROR_BUFFER_NOT_ENOUGH_SPACE                 = 23, // Not enough space in buffer to perform operation
	ERROR_BUFFER_POINTER_NULL                     = 24, // Buffer size was set to zero
	ERROR_BUFFER_SIZE_TOO_LARGE                   = 25, // Buffer size was set to a large value
	ERROR_BUFFER_SIZE_TOO_SMALL                   = 26, // Buffer size was set to a very small value
	ERROR_BUFFER_SIZE_ZERO                        = 27, // Buffer size was set to zero

	// Error in handlers and functions pointers
	ERROR_FUNCTION_POINTER_NULL                   = 30,
	ERROR_HANDLER_POINTER_NULL                    = 31,

	// Error related to date and time
	ERROR_TIME_INVALID                            = 40,
	ERROR_DATE_INVALID                            = 41,

	// Error related to clock configuration
	ERROR_CLOCK_PRESCALER_CHANGE_FAILED           = 50, // Clock prescaler change failed
	ERROR_CLOCK_PRESCALER_UNSUPPORTED             = 51, // Clock prescaler configuration unsupported
	ERROR_CLOCK_SPEED_TOO_HIGH                    = 52,
	ERROR_CLOCK_SPEED_TOO_LOW                     = 53,

	// Error related to communication to devices
	ERROR_COMMUNICATION_FAILED                    = 60,
	ERROR_COMMUNICATION_PORT_NOT_SET              = 61,
	ERROR_COMMUNICATION_TIME_OUT                  = 62,

	// Generic error related to external devices
	ERROR_DEVICE_ID_MATCH_FAILED                  = 70,
	ERROR_NO_DEVICE_SELECTED                      = 71,

	// Error related to IMU sensors
	ERROR_INVALID_AXIS                            = 80,

	// Error related to communication protocol
	ERROR_CHECKSUM_ERROR                          = 90,
	ERROR_FRAME_ERROR                             = 91,
	ERROR_PACKAGE_AWAITING                        = 92, // Try to write data to a ready package
	ERROR_PACKAGE_NOT_READY                       = 93, // Try to read data from a not ready package

	// Error related to external interrupts
	ERROR_INTERRUPT_NOT_AVAILABLE                 = 100,
	ERROR_PORT_OUTPUT_MODE_UNSUPPORTED            = 101,
	ERROR_SENSE_MODE_UNSUPPORTED                  = 102,


	ERROR_USART_CONFIGURATION_PARAMETER_WRONG,
	ERROR_USART_UNSUPPORTED_CONFIGURATION_BAUD_RATE,
	ERROR_USART_UNSUPPORTED_CONFIGURATION_DATA_BITS,
	ERROR_USART_UNSUPPORTED_CONFIGURATION_FRAME_FORMAT,
	ERROR_USART_UNSUPPORTED_CONFIGURATION_PARITY,
	ERROR_USART_UNSUPPORTED_CONFIGURATION_STOP_BIT,





	ERROR_PACKAGE_API_INVALID_SOURCE_ADDRESS,
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
} error_e;
/*

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

typedef enum timerOutput_e {
	TIMER_PORT_NO_CHANGE			= 0,	// Keep last port configuration
	TIMER_PORT_NORMAL				= 1,	// Output Compare Pin disconnected; normal GPIO operation
	TIMER_PORT_TOGGLE_ON_COMPARE	= 2,	// Toggle pin value on compare
	TIMER_PORT_CLEAR_ON_COMPARE		= 3,	// Clear pin on compare
	TIMER_PORT_SET_ON_COMPARE		= 4,	// Set pin on compare
	TIMER_PORT_NON_INVERTING_MODE	= 5,	// PWM non-inverting mode
	TIMER_PORT_INVERTING_MODE		= 6		// PWM inverting mode
} timerOutput_e;


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

#if defined (GPDSE_PLATFORM_STM32)
#	define i2cHandler	I2C_HandleTypeDef
#elif defined (GPDSE_PLATFORM_AVR8)
#	define i2cHandler	void
#endif

#endif
