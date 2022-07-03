/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibGlobalDefines.hpp
 * Module:			Global definitions for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#define __FUNSAPE_LIB_GLOBAL_DEFINES_HPP		__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Project configuration file --------------------------------------------------

#include "funsapeLibProjectConfig.hpp"
#ifndef __FUNSAPE_LIB_PROJECT_CONFIG_HPP
#	error	[funsapeLibGlobalDefines.hpp] Error 7 - Project configuration file (funsapeLibProjectConfig.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_PROJECT_CONFIG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibGlobalDefines.hpp] Error 8 - Build mismatch between project configuration file (funsapeLibProjectConfig.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - Basic header for all files -----------------------------------

#include <stddef.h>
#include <sys/_stdint.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// =============================================================================
// Undefining previous definitions
// =============================================================================

#undef byteNibbleHigh
#undef byteNibbleLow
#undef byteSwitchNibbles
#undef clrBit
#undef clrIoPin
#undef clrMask
#undef convertToLowerCase
#undef convertToUpperCase
#undef cplBit
#undef cplIoPin
#undef cplMask
#undef delayMs
#undef doNop
#undef doNothing
#undef dwordByte0
#undef dwordByte1
#undef dwordByte2
#undef dwordByte3
#undef dwordSwitchBytes
#undef dwordSwitchWords
#undef dwordWordHigh
#undef dwordWordLow
#undef getTick
#undef ignored
#undef inlined
#undef isAsciiCommand
#undef isAsciiLetter
#undef isAsciiLowerCaseLetter
#undef isAsciiNumber
#undef isAsciiSymbol
#undef isAsciiUpperCaseLetter
#undef isBitClr
#undef isBitSet
#undef isGpioValid
#undef isPointerValid
#undef maximumOf
#undef minimumOf
#undef setBit
#undef setIoPin
#undef setMask
#undef stringifyName
#undef stringifyValue
#undef systemHalt
#undef truncateBetween
#undef waitUntilBitIsClear
#undef waitUntilBitIsSet
#undef wordByteHigh
#undef wordByteLow
#undef wordSwitchBytes

// =============================================================================
// Constant definitions
// =============================================================================

#define inlined							inline __attribute__((always_inline))
#define ignored							__attribute__((unused))

// =============================================================================
// Macro-function definitions
// *INDENT-OFF*
// =============================================================================

// -----------------------------------------------------------------------------
// Bit handling ----------------------------------------------------------------

#define clrBit(reg, bit)				((reg) &= (~(1 << (bit))))
#define cplBit(reg, bit)				((reg) ^= (1 << (bit)))
#define setBit(reg, bit)				((reg) |= (1 << (bit)))
#define isBitClr(reg, bit)				(bool)(!(((reg) >> (bit)) & 1))
#define isBitSet(reg, bit)				(bool)(((reg) >> (bit)) & 0x01)

// -----------------------------------------------------------------------------
// Bit loop --------------------------------------------------------------------

#define waitUntilBitIsClear(reg, bit)	do {} while(isBitSet((reg), (bit)))
#define waitUntilBitIsSet(reg, bit)		do {} while(isBitClr((reg), (bit)))

// -----------------------------------------------------------------------------
// Bit mask handling -----------------------------------------------------------

#define clrMask(reg, mask, offset)		((reg) &= ~((mask) << (offset)))
#define cplMask(reg, mask, offset)		((reg) ^= ((mask) << (offset)))
#define setMask(reg, mask, offset)		((reg) |= ((mask) << (offset)))

// -----------------------------------------------------------------------------
// Data manipulation (8-bits data) ---------------------------------------------

#define byteNibble1(dataByte)			(((dataByte) >> 4) & 0x0F)
#define byteNibble0(dataByte)			((dataByte) & 0x0F)
#define byteSwitchNibbles(dataByte)		((byteNibble0(dataByte) << 4) | \
										((byteNibble1(dataByte))))

// -----------------------------------------------------------------------------
// Data manipulation (16-bits data) --------------------------------------------

#define hwordByte1(dataHword)			(((dataHword) >> 8) & 0x00FF)
#define hwordByte0(dataHword)			((dataHword) & 0x00FF)
#define hwordSwitchBytes(dataHword)		((hwordByte0(dataHword) << 8) | \
										((hwordByte1(dataHword))))

// -----------------------------------------------------------------------------
// Data manipulation (32-bits data) --------------------------------------------

#define wordByte3(dataWord)				(((dataWord) >> 24) & 0x000000FF)
#define wordByte2(dataWord)				(((dataWord) >> 16) & 0x000000FF)
#define wordByte1(dataWord)				(((dataWord) >> 8) & 0x000000FF)
#define wordByte0(dataWord)				((dataWord) & 0x000000FF)
#define wordSwitchBytes(dataWord)		((wordByte0(dataWord) << 24)  | \
										((wordByte1(dataWord) << 16)) | \
										((wordByte2(dataWord) << 8))  | \
										((wordByte3(dataWord))))
#define wordHword1(dataWord)			(((dataWord) >> 16) & 0x0000FFFF)
#define wordHword0(dataWord)			((dataWord) & 0x0000FFFF)
#define wordSwitchHwords(dataWord)		((wordHword0(dataWord) << 16) | \
										((wordHword1(dataWord))))

// -----------------------------------------------------------------------------
// Data manipulation (64-bits data) --------------------------------------------

#define dwordByte7(dataDword)			(((dataDword) >> 56) & 0x00000000000000FF)
#define dwordByte6(dataDword)			(((dataDword) >> 48) & 0x00000000000000FF)
#define dwordByte5(dataDword)			(((dataDword) >> 40) & 0x00000000000000FF)
#define dwordByte4(dataDword)			(((dataDword) >> 32) & 0x00000000000000FF)
#define dwordByte3(dataDword)			(((dataDword) >> 24) & 0x00000000000000FF)
#define dwordByte2(dataDword)			(((dataDword) >> 16) & 0x00000000000000FF)
#define dwordByte1(dataDword)			(((dataDword) >> 8) & 0x00000000000000FF)
#define dwordByte0(dataDword)			((dataDword) & 0x00000000000000FF)
#define dwordSwitchBytes(dataDword)		((wordByte0(dataDword) << 56)  | \
										((wordByte1(dataDword) << 48)) | \
										((wordByte2(dataDword) << 40)) | \
										((wordByte3(dataDword) << 32)) | \
										((wordByte4(dataDword) << 24)) | \
										((wordByte5(dataDword) << 16)) | \
										((wordByte6(dataDword) << 8))  | \
										((wordByte7(dataDword))))
#define dwordHword3(dataDword)			((dataDword >> 48) & 0x000000000000FFFF)
#define dwordHword2(dataDword)			((dataDword >> 32) & 0x000000000000FFFF)
#define dwordHword1(dataDword)			((dataDword >> 16) & 0x000000000000FFFF)
#define dwordHword0(dataDword)			((dataDword >> 0) & 0x000000000000FFFF)
#define dwordSwitchHwords(dataDword)	((dwordHword0(dataDword) << 48)  | \
										((dwordHword1(dataDword) << 32)) | \
										((dwordHword2(dataDword) << 16)) | \
										((dwordHword3(dataDword))))
#define dwordWord1(dataDword)			(((dataDword) >> 32) & 0x00000000FFFFFFFF)
#define dwordWord0(dataDword)			((dataDword) & 0x00000000FFFFFFFF)
#define dwordSwitchWords(dataDword)		((dwordWord0(dataDword) << 32)  | \
										((dwordWord1(dataDword))))

// -----------------------------------------------------------------------------
// ASCII character macros ------------------------------------------------------

#define isAsciiCommand(charac)			((charac < ' ') ? true : false)
#define isAsciiLetter(charac)			((isAsciiLowerCaseLetter(charac) || (isAsciiUpperCaseLetter(charac)) ? true : false)
#define isAsciiLowerCaseLetter(charac)	(((charac >= 'a') && (charac <= 'z')) ? true : false)
#define isAsciiUpperCaseLetter(charac)	(((charac >= 'A') && (charac <= 'Z')) ? true : false)
#define isAsciiNumber(charac)			(((charac >= '0') && (charac <= '9')) ? true : false)
#define isAsciiSymbol(charac)			(((!isAsciiNumber(charac)) && (!isAsciiCommand(charac)) && (!isAsciiLetter(charac))) ? true : false)
#define convertToUpperCase(charac)		((isAsciiLowerCaseLetter(charac)) ? (charac - 0x20) : charac)
#define convertToLowerCase(charac)		((isAsciiUpperCaseLetter(charac)) ? (charac + 0x20) : charac)

// -----------------------------------------------------------------------------
// Value comparison ------------------------------------------------------------

#define getMaximumOf(var1 , var2)		(((var1) > (var2))	? \
										(((var1)))			: \
										(((var2))))
#define getMinimumOf(var1 , var2)		(((var1) < (var2))	? \
										(((var1)))			: \
										(((var2))))
#define truncateBetween(var, min, max)	(((var)) < (min))	? \
										(((min)))			: \
										(((var) > (max))	? \
										(((max)))			: \
										(((var)))))

// -----------------------------------------------------------------------------
// Stringifying macro arguments ------------------------------------------------

#define stringifyName(constant)			#constant
#define stringifyValue(constant)		stringifyName(constant)

// -----------------------------------------------------------------------------
// System actions --------------------------------------------------------------

#define doNop()							do {asm volatile("nop");} while(0)
#define doNothing()						{}
#define isPointerValid(ptr)				((ptr) ? (bool)true : (bool)false)
#define systemHalt()					while(1) {}

#define isGpioValid(port, first, offset, length)	true

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#	if defined(USE_HAL_DRIVER)
#		define setIoPin(GPIOx, GPIO_Pin)		do {GPIOx->BSRR = (uint32_t)(GPIO_Pin);} while(0)
#		define clrIoPin(GPIOx, GPIO_Pin)		do {GPIOx->BRR = (uint32_t)(GPIO_Pin);} while(0)
#		define cplIoPin(GPIOx, GPIO_Pin)		do {((GPIOx->ODR & GPIO_Pin) != 0x00u) ? (GPIOx->BRR = (uint32_t)(GPIO_Pin)) : (GPIOx->BSRR = (uint32_t)(GPIO_Pin));} while(0)
#	endif
#endif

#if defined(_FUNSAPE_PLATFORM_AVR)
#	define delayMs(time)				_delay_ms(time)
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
#	define delayMs(time)				HAL_Delay(time)
#endif

#if defined(_FUNSAPE_PLATFORM_AVR)
#	define getTick()					0
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
#	define getTick()					HAL_GetTick()
#endif

// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// Single variables ------------------------------------------------------------
//     => Shortening the name of single data types -----------------------------

typedef const bool						cbool;
typedef const int8_t					cint8_t;
typedef const int16_t					cint16_t;
typedef const int32_t					cint32_t;
typedef const int64_t					cint64_t;
typedef const uint8_t					cuint8_t;
typedef const uint16_t					cuint16_t;
typedef const uint32_t					cuint32_t;
typedef const uint64_t					cuint64_t;
typedef volatile bool					vbool;
typedef volatile int8_t					vint8_t;
typedef volatile int16_t				vint16_t;
typedef volatile int32_t				vint32_t;
typedef volatile int64_t				vint64_t;
typedef volatile uint8_t				vuint8_t;
typedef volatile uint16_t				vuint16_t;
typedef volatile uint32_t				vuint32_t;
typedef volatile uint64_t				vuint64_t;
#if defined(_FUNSAPE_PLATFORM_AVR)
typedef int16_t							int_t;		// 16-bit (AVR)
typedef uint16_t						uint_t;		// 16-bit (AVR)
typedef const int16_t					cint_t;		// 16-bit (AVR)
typedef const uint16_t					cuint_t;	// 16-bit (AVR)
typedef volatile int16_t				vint_t;		// 16-bit (AVR)
typedef volatile uint16_t				vuint_t;	// 16-bit (AVR)
typedef volatile uint8_t				reg_t;		// 8-bit (AVR)
typedef volatile uint8_t				ioReg_t;	// 8-bit (AVR)
typedef uint8_t							ioPin_t;	// 8-bit (AVR)
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
typedef int32_t							int_t;		// 32-bit (ARM)
typedef uint32_t						uint_t;		// 32-bit (ARM)
typedef const int32_t					cint_t;		// 32-bit (ARM)
typedef const uint32_t					cuint_t;	// 32-bit (ARM)
typedef volatile int32_t				vint_t;		// 32-bit (ARM)
typedef volatile uint32_t				vuint_t;	// 32-bit (ARM)
typedef volatile uint32_t				reg_t;		// 32-bit (ARM)
typedef uint16_t						ioPin_t;	// 16-bit (ARM)
#	if defined(USE_HAL_DRIVER)
typedef GPIO_TypeDef					*ioReg_t;	// 32-bit (ARM)
#	else
typedef volatile uint32_t				ioReg_t;	// 32-bit (ARM)
#	endif
#endif

// -----------------------------------------------------------------------------
// Single variables ------------------------------------------------------------
//     => Small enumerations understood as logic values ------------------------

enum class Activation : bool {
	OFF									= false,
	ON									= true
};

enum class DriverMode : bool {
	PUSH_PULL							= false,
	OPEN_DRAIN							= true,
};

enum class Edge : bool {
	FALLING								= false,
	RISING								= true
};

enum class IoOperation : bool {
	WRITE								= false,
	READ								= true
};

enum class LogicLevel : bool {
	LOW									= false,
	HIGH								= true
};

enum class PinDirection : bool {
	INPUT								= false,
	OUTPUT								= true,
};

enum class Rotation : bool {
	CLOCKWISE							= false,
	COUNTERCLOCKWISE					= true
};

enum class State : bool {
	OFF									= false,
	ON									= true
};

// -----------------------------------------------------------------------------
// Single variables ------------------------------------------------------------
//     => Small enumerations that can be understood as a single variable -------

enum class Axis : uint8_t {
	//									   AXIS-Z	  AXIS-Y	 AXIS-X
	NONE								= (0 << 2) | (0 << 1) | (0 << 0),
	X									= (0 << 2) | (0 << 1) | (1 << 0),
	Y									= (0 << 2) | (1 << 1) | (0 << 0),
	XY									= (0 << 2) | (1 << 1) | (1 << 0),
	Z									= (1 << 2) | (0 << 1) | (0 << 0),
	XZ									= (1 << 2) | (0 << 1) | (1 << 0),
	YZ									= (1 << 2) | (1 << 1) | (0 << 0),
	ALL									= (1 << 2) | (1 << 1) | (1 << 0)
};

inlined Axis operator|(Axis a, Axis b)
{
	return static_cast<Axis>(static_cast<int>(a) | static_cast<int>(b));
}

inlined Axis &operator|=(Axis &a, Axis b)
{
	return a = static_cast<Axis>(static_cast<int>(a) | static_cast<int>(b));
}

inlined Axis operator&(Axis a, Axis b)
{
	return static_cast<Axis>(static_cast<int>(a) & static_cast<int>(b));
}

inlined Axis &operator&=(Axis &a, Axis b)
{
	return a = static_cast<Axis>(static_cast<int>(a) & static_cast<int>(b));
}

enum class Direction : uint8_t {
	LEFT								= 0,
	RIGHT								= 1,
	UP									= 2,
	DOWN								= 3
};

enum class InterfaceMode : uint8_t {
	BLOCKING_MODE						= 0,
	INTERRUPT_MODE						= 1
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
	,
	DMA_MODE							= 2
#endif
};

enum class DisplayOrientation : bool {
	LANDSCAPE							= false,
	PORTRAIT							= true
};

enum class ThermometricScale : uint8_t {
	CELSIUS								= 0,
	DELISLE								= 1,
	FAHRENHEIT							= 2,
	KELVIN								= 3,
	NEWTON								= 4,
	RANKINE								= 5,
	REAUMUR								= 6,
	ROMER								= 7
};

// -----------------------------------------------------------------------------
// Enumerations ----------------------------------------------------------------
//     => Large enumerations or enumerations with expansible number of elements

enum class Error : uint16_t {
	// General error codes
	NONE												= 0x0000,	// Success - No errors occurred
	FEATURE_UNSUPPORTED									= 0x0001,	// Unsupported feature or configuration
	FUNCTION_POINTER_NULL								= 0x0002,	// TODO: Describe parameter
	INSTANCE_INVALID									= 0x0003,	// Invalid instance
	IS_BUSY												= 0x0004,	// TODO: Describe parameter
	LOCKED												= 0x0005,	// Accessed a locked device
	MEMORY_ALLOCATION									= 0x0006,	// Memory allocation failed
	NOT_INITIALIZED										= 0x0007,	// Not initialized
	NOT_READY											= 0x0008,	// TODO: Describe parameter
	READ_PROTECTED										= 0x0009,	// Tried to read a read protected device
	UNDER_DEVELOPMENT									= 0x000A,	// This part of the code is still under development
	WRITE_PROTECTED										= 0x000B,	// Tried to write a write protected device
	INVALID_AXIS										= 0x000C,	// Invalid axis
	DEVICE_UNSUPPORTED									= 0x000D,	// Devie is not currently supported
	MODE_UNSUPPORTED									= 0x000E,	// Mode is not currently supported

	// Function arguments related error codes
	ARGUMENT_CANNOT_BE_ZERO								= 0x0010,	// TODO: Describe parameter
	ARGUMENT_POINTER_NULL								= 0x0011,	// NULL pointer was passed as an argument to function
	ARGUMENT_VALUE_INVALID								= 0x0012,	// TODO: Describe parameter
	ARGUMENTS_MISMATCH									= 0x0013,	// A function was called with dubious argument values
	ARGUMENTS_WRONG_NUMBER								= 0x0014,	// A function was called with wrong number of arguments

	// Buffer related error codes
	BUFFER_EMPTY										= 0x0020,	// Buffer is empty
	BUFFER_FULL											= 0x0021,	// Buffer is full
	BUFFER_NOT_ENOUGH_ELEMENTS							= 0x0022,	// Not enough space in buffer to perform operation
	BUFFER_NOT_ENOUGH_SPACE								= 0x0023,	// Not enough space in buffer to perform operation
	BUFFER_POINTER_NULL									= 0x0024,	// Buffer size was set to zero
	BUFFER_SIZE_TOO_LARGE								= 0x0025,	// Buffer size was set to a large value
	BUFFER_SIZE_TOO_SMALL								= 0x0026,	// Buffer size was set to a very small value
	BUFFER_SIZE_ZERO									= 0x0027,	// Buffer size was set to zero

	// Clock configuration related error codes
	CLOCK_PRESCALER_CHANGE_FAILED						= 0x0030,	// Clock prescaler change failed
	CLOCK_PRESCALER_UNSUPPORTED							= 0x0031,	// Clock prescaler configuration unsupported
	CLOCK_SPEED_TOO_HIGH								= 0x0032,	// TODO: Describe parameter
	CLOCK_SPEED_TOO_LOW									= 0x0033,	// TODO: Describe parameter

	// Communication related error codes
	COMMUNICATION_FAILED								= 0x0040,	// TODO: Describe parameter
	COMMUNICATION_NO_DEVICE_SELECTED					= 0x0041,	// TODO: Describe parameter
	COMMUNICATION_PORT_NOT_SET							= 0x0042,	// TODO: Describe parameter
	COMMUNICATION_TIMEOUT								= 0x0043,	// The opereation timed out

	// Communication protocol related error codes
	CHECKSUM_ERROR										= 0x0050,	// TODO: Describe parameter
	FRAME_ERROR											= 0x0051,	// TODO: Describe parameter
	PACKAGE_AWAITING									= 0x0052,	// Try to write data to a ready package
	PACKAGE_NOT_READY									= 0x0053,	// Try to read data from a not ready package

	// DateTime class related error codes
	DATETIME_DATE_NOT_INITIALIZED						= 0x0060,	// TODO: Describe parameter
	DATETIME_HOUR_INVALID								= 0x0061,	// TODO: Describe parameter
	DATETIME_MILLISECOND_INVALID						= 0x0062,	// TODO: Describe parameter
	DATETIME_MINUTE_INVALID								= 0x0063,	// TODO: Describe parameter
	DATETIME_MONTH_DAY_INVALID							= 0x0064,	// TODO: Describe parameter
	DATETIME_MONTH_INVALID								= 0x0065,	// TODO: Describe parameter
	DATETIME_SECOND_INVALID								= 0x0066,	// TODO: Describe parameter
	DATETIME_TIME_NOT_INITIALIZED						= 0x0067,	// TODO: Describe parameter
	DATETIME_TIMEZONE_INVALID							= 0x0068,	// TODO: Describe parameter
	DATETIME_YEAR_INVALID								= 0x0069,	// TODO: Describe parameter

	// Handler related error codes
	HANDLER_POINTER_NULL								= 0x0070,	// TODO: Describe parameter
	HANDLER_UNSUPPORTED									= 0x0071,	// TODO: Describe parameter

	// External interrupts related error codes
	INTERRUPT_NOT_AVAILABLE								= 0x0080,	// TODO: Describe parameter
	INTERRUPT_PORT_OUTPUT_MODE_UNSUPPORTED				= 0x0081,	// TODO: Describe parameter
	INTERRUPT_SENSE_MODE_UNSUPPORTED					= 0x0082,	// TODO: Describe parameter

	// Package API class related error codes
	PACKAGE_API_PACKAGE_DATA_FULL						= 0x0090,	// TODO: Describe parameter
	PACKAGE_API_PACKAGE_NOT_READY						= 0x0091,	// TODO: Describe parameter
	PACKAGE_API_PAYLOAD_DATA_NULL						= 0x0092,	// TODO: Describe parameter
	PACKAGE_API_PAYLOAD_SIZE_NULL						= 0x0093,	// TODO: Describe parameter
	PACKAGE_API_SOURCE_ADDRESS_INVALID					= 0x0094,	// TODO: Describe parameter

	// SPI related error codes
	SPI_BUSY_FLAG										= 0x00A0,	// TODO: Describe parameter
	SPI_CRC												= 0x00A1,	// TODO: Describe parameter
	SPI_DMA												= 0x00A2,	// TODO: Describe parameter
	SPI_MODE_FAULT										= 0x00A3,	// TODO: Describe parameter
	SPI_OVERRUN											= 0x00A4,	// TODO: Describe parameter

	// TWI related error codes
	TWI_AF_FLAG											= 0x00B0,	// TWI AF flag error
	TWI_ARBITRATION_LOST								= 0x00B1,	// TWI bus arbitration lost
	TWI_BUS_ERROR										= 0x00B2,	// TWI bus error
	TWI_OVERRUN											= 0x00B4,	// TWI overrun
	TWI_DMA												= 0x00B5,	// DMA parameter error
	TWI_DMA_PARAM										= 0x00B6,	// DMA transfer error
	TWI_SIZE_MANAGEMENT_ERROR							= 0x00B7,	// Size management error

	// USART related error codes
	USART_BAUD_RATE_UNSUPPORTED							= 0x00C0,	// TODO: Describe parameter
	USART_CONFIGURATION_PARAMETER_WRONG					= 0x00C1,	// TODO: Describe parameter
	USART_DATA_BITS_UNSUPPORTED							= 0x00C2,	// TODO: Describe parameter
	USART_FRAME_FORMAT_UNSUPPORTED						= 0x00C3,	// TODO: Describe parameter
	USART_PARITY_UNSUPPORTED							= 0x00C4,	// TODO: Describe parameter
	USART_STOP_BIT_UNSUPPORTED							= 0x00C5,	// TODO: Describe parameter

	// Uncategorized error codes
	CONTROLLER_UNSUPPORTED								= 0x00D0,	// Unsupported controller
	DEVICE_ID_MATCH_FAILED								= 0x00D1,	// Device ID doesn't match
	DMA_NOT_SUPPORTED									= 0x00D2,	// DMA interface mode is not supported for this module
	DMA_TRANSFER_ERROR									= 0x00D3,	// DMA transfer error
	GPIO_NO_PIN_SET										= 0x00D4,	// No pin selected at Gpio peripheral
	GPIO_PORT_INVALID									= 0x00D5,	// Invalid GPIO Port
	MESSAGE_TOO_LONG									= 0x00D6,	// Message is to long to be stored inside buffer
	VALID_DATA_UNAVAILABLE								= 0x00D7,	// Valid data was unavailable

	// Uncategorized error codes
	LCD_PORTS_NOT_SET									= 0x00E0,

	// "I don't have the slightest idea" error code
	UNKNOWN												= 0xFFFF,	// Unknown error - Ops!!!!
};

// -----------------------------------------------------------------------------
// Flags -----------------------------------------------------------------------
//     => Enumerations that can be combined in the form of flags ---------------

// NONE

// -----------------------------------------------------------------------------
// Structures, Unions and Bit Fields -------------------------------------------

// NONE


// -----------------------------------------------------------------------------
// Classes ---------------------------------------------------------------------

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// Public functions declarations
// =============================================================================

// NONE

// =============================================================================
// Includes Low Level Abstration Layer
// =============================================================================

// NONE

#endif // __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
