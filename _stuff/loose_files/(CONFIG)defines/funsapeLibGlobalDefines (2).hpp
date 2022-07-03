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
#	error	[funsapeLibGlobalDefines.hpp] Error 7 - Project configuration file (funsapeLibProjectConfig.hpp) is missing!
#elif __FUNSAPE_LIB_PROJECT_CONFIG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibGlobalDefines.hpp] Error 8 - Build mismatch between project configuration file (funsapeLibProjectConfig.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - Basic header for all files -----------------------------------

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

#define inlined inline __attribute__((always_inline))

// =============================================================================
// Macro-function definitions
// *INDENT-OFF*
// =============================================================================

// -----------------------------------------------------------------------------
// Bit handling ----------------------------------------------------------------

#define clrBit(reg, bit)						((reg) &= (~(1 << (bit))))
#define cplBit(reg, bit)						((reg) ^= (1 << (bit)))
#define setBit(reg, bit)						((reg) |= (1 << (bit)))
#define isBitClr(reg, bit)						(bool)(!(((reg) >> (bit)) & 1))
#define isBitSet(reg, bit)						(bool)(((reg) >> (bit)) & 0x01)

// -----------------------------------------------------------------------------
// Bit loop --------------------------------------------------------------------

#define waitUntilBitIsClear(reg, bit)			do {} while(isBitSet((reg), (bit)))
#define waitUntilBitIsSet(reg, bit)				do {} while(isBitClr((reg), (bit)))

// -----------------------------------------------------------------------------
// Bit mask handling -----------------------------------------------------------

#define clrMask(reg, mask, offset)				((reg) &= ~((mask) << (offset)))
#define cplMask(reg, mask, offset)				((reg) ^= ((mask) << (offset)))
#define setMask(reg, mask, offset)				((reg) |= ((mask) << (offset)))

// -----------------------------------------------------------------------------
// Data manipulation (8-bits data) ---------------------------------------------

#define byteNibble1(dataByte)					(((dataByte) >> 4) & 0x0F)
#define byteNibble0(dataByte)					((dataByte) & 0x0F)
#define byteSwitchNibbles(dataByte)				((byteNibble0(dataByte) << 4) | byteNibble1(dataByte))

// -----------------------------------------------------------------------------
// Data manipulation (16-bits data) --------------------------------------------

#define hwordByte1(dataHword)					(((dataHword) >> 8) & 0x00FF)
#define hwordByte0(dataHword)					((dataHword) & 0x00FF)
#define hwordSwitchBytes(dataHword)				((hwordByte0(dataHword) << 8) | hwordByte1(dataHword))

// -----------------------------------------------------------------------------
// Data manipulation (32-bits data) --------------------------------------------

#define wordByte3(dataWord)						(((dataWord) >> 24) & 0x000000FF)
#define wordByte2(dataWord)						(((dataWord) >> 16) & 0x000000FF)
#define wordByte1(dataWord)						(((dataWord) >> 8) & 0x000000FF)
#define wordByte0(dataWord)						((dataWord) & 0x000000FF)
#define wordSwitchBytes(dataWord)				((wordByte0(dataWord) << 24) | (wordByte1(dataWord) << 16) | \
												 (wordByte2(dataWord) << 8) | wordByte3(dataWord))
#define wordHword1(dataWord)					(((dataWord) >> 16) & 0x0000FFFF)
#define wordHword0(dataWord)					((dataWord) & 0x0000FFFF)
#define wordSwitchHwords(dataWord)				((wordHword0(dataWord) << 16) | wordHword1(dataWord))

// -----------------------------------------------------------------------------
// Data manipulation (64-bits data) --------------------------------------------

#define dwordByte7(dataDword)					(((dataDword) >> 56) & 0x00000000000000FF)
#define dwordByte6(dataDword)					(((dataDword) >> 48) & 0x00000000000000FF)
#define dwordByte5(dataDword)					(((dataDword) >> 40) & 0x00000000000000FF)
#define dwordByte4(dataDword)					(((dataDword) >> 32) & 0x00000000000000FF)
#define dwordByte3(dataDword)					(((dataDword) >> 24) & 0x00000000000000FF)
#define dwordByte2(dataDword)					(((dataDword) >> 16) & 0x00000000000000FF)
#define dwordByte1(dataDword)					(((dataDword) >> 8) & 0x00000000000000FF)
#define dwordByte0(dataDword)					((dataDword) & 0x00000000000000FF)
#define dwordSwitchBytes(dataDword)				((wordByte0(dataDword) << 56) | (wordByte1(dataDword) << 48) | \
												 (wordByte2(dataDword) << 40) | (wordByte3(dataDword) << 32) | \
												 (wordByte4(dataDword) << 24) | (wordByte5(dataDword) << 16) | \
												 (wordByte6(dataDword) << 8) | wordByte7(dataDword))
#define dwordHword3(dataDword)					((dataDword >> 48) & 0x000000000000FFFF)
#define dwordHword2(dataDword)					((dataDword >> 32) & 0x000000000000FFFF)
#define dwordHword1(dataDword)					((dataDword >> 16) & 0x000000000000FFFF)
#define dwordHword0(dataDword)					((dataDword >> 0) & 0x000000000000FFFF)
#define dwordSwitchHwords(dataDword)			((dwordHword0(dataDword) << 48) | (dwordHword1(dataDword) << 32) | \
												 (dwordHword2(dataDword) << 16) | dwordHword3(dataDword))
#define dwordWord1(dataDword)					(((dataDword) >> 32) & 0x00000000FFFFFFFF)
#define dwordWord0(dataDword)					((dataDword) & 0x00000000FFFFFFFF)
#define dwordSwitchWords(dataDword)				((dwordWord0(dataDword) << 32) | dwordWord1(dataDword))

// -----------------------------------------------------------------------------
// Value comparison ------------------------------------------------------------

#define getMaximumOf(var1 , var2)				((var1 > var2) ? var1 : var2)
#define getMinimumOf(var1 , var2)				((var1 < var2) ? var1 : var2)
#define truncateBetween(var, minimum, maximum)	(var < minimum) ? minimum : ((var > maximum) ? maximum : var))

// -----------------------------------------------------------------------------
// Stringification of macro arguments ------------------------------------------

#define stringifyName(constant)					#constant
#define stringifyValue(constant)				stringifyName(constant)

// -----------------------------------------------------------------------------
// System actions --------------------------------------------------------------

#define systemHalt()							while(1) {}

// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// Single variables => shortening the name of single data types ----------------

typedef volatile int8_t					vint8_t;
typedef volatile int16_t				vint16_t;
typedef volatile int32_t				vint32_t;
typedef volatile int64_t				vint64_t;
typedef volatile uint8_t				vuint8_t;
typedef volatile uint16_t				vuint16_t;
typedef volatile uint32_t				vuint32_t;
typedef volatile uint64_t				vuint64_t;
#if defined(_FUNSAPE_PLATFORM_AVR)
typedef volatile uint8_t				reg_t;
typedef volatile uint8_t				ioReg_t;
typedef uint8_t							ioPin_t;
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
typedef volatile uint32_t				reg_t;
typedef uint16_t						ioPin_t;
#	if defined(USE_HAL_DRIVER)
typedef GPIO_TypeDef					*ioReg_t;
#	else
typedef volatile uint32_t				ioReg_t;
#	endif
#endif

// -----------------------------------------------------------------------------
// Single variables => enumerations understood as logic values -----------------

typedef enum activation_t {
	DEACTIVATE							= false,
	ACTIVATE							= true
} activation_t;

typedef enum edge_t {
	EDGE_FALLING						= false,
	EDGE_RISING							= true
} edge_t;

typedef enum logicLevel_t {
	LOGIC_LOW							= false,
	LOGIC_HIGH							= true
} logicLevel_t;

typedef enum outputDriver_t {
	DRIVER_OPEN_DRAIN					= false,
	DRIVER_PUSH_PULL					= true
} outputDriver_t;

typedef enum planarDirectionHorizontal_t {
	HORIZONTAL_LEFT						= false,
	HORIZONTAL_RIGHT					= true
} planarDirectionHorizontal_t;

typedef enum planarDirectionVertical_t {
	VERTICAL_DOWN						= false,
	VERTICAL_UP							= true
} planarDirectionVertical_t;

typedef enum rotacionalDirection_t {
	ROTATE_CLOCKWISE					= false,
	ROTATE_COUNTERCLOCKWISE				= true
} rotacionalDirection_t;

// -----------------------------------------------------------------------------
// Single variables => small enumerations that can be understood as a single variable

typedef enum planarDirection_t {
	PLANAR_DOWN							= 0,
	PLANAR_UP							= 1,
	PLANAR_LEFT							= 2,
	PLANAR_RIGHT						= 3
} planarDirection_t;

typedef enum axis_t : uint8_t {
	//									   AXIS-Z	  AXIS-Y	 AXIS-X
	AXIS_NONE							= (0 << 2) | (0 << 1) | (0 << 0),
	AXIS_X								= (0 << 2) | (0 << 1) | (1 << 0),
	AXIS_Y								= (0 << 2) | (1 << 1) | (0 << 0),
	AXIS_XY								= (0 << 2) | (1 << 1) | (1 << 0),
	AXIS_Z								= (1 << 2) | (0 << 1) | (0 << 0),
	AXIS_XZ								= (1 << 2) | (0 << 1) | (1 << 0),
	AXIS_YZ								= (1 << 2) | (1 << 1) | (0 << 0),
	AXIS_ALL							= (1 << 2) | (1 << 1) | (1 << 0)
} axis_t;

// -----------------------------------------------------------------------------
// Enumerations => large enumerations or enumerations with expansible number of element

#undef ERROR_LOCKED
typedef enum error_e {
	// Errors related to general identifiers
	ERROR_NONE											= 0,	// Success - No errors occurred
	ERROR_LOCKED										= 1,	// Accessed a locked device
	ERROR_MEMORY_ALLOCATION								= 2,	// Memory allocation failed
	ERROR_NOT_INITIALIZED								= 3,	// Not initialized
	ERROR_READ_PROTECTED								= 4,	// Tried to read a read protected device
	ERROR_UNKNOWN										= 5,	// Something really unpredictable has happen
	ERROR_WRITE_PROTECTED								= 6,	// Tried to write a write protected device
	ERROR_WRONG_CONFIGURATION							= 7,	// TODO: Describe parameter
	ERROR_OPERATION_MODE_UNSUPPORTED					= 8,	// TODO: Describe parameter
	ERROR_PERIPHERAL_NOT_READY							= 9,	// TODO: Describe parameter
	ERROR_IS_BUSY										= 10,	// TODO: Describe parameter
//	ERROR_OUTPUT_COMPARE_UNSUPPORTED					= 11,	// TODO: Describe parameter

	// Errors related to arguments
	ERROR_ARGUMENT_CANNOT_BE_ZERO						= 20,	// TODO: Describe parameter
	ERROR_ARGUMENT_POINTER_NULL							= 21,	// NULL pointer was passed as an argument to function
	ERROR_ARGUMENTS_MISMATCH							= 22,	// A function was called with dubious argument values
	ERROR_ARGUMENTS_WRONG_NUMBER						= 23,	// A function was called with wrong number of arguments

	// Errors related to buffers
	ERROR_BUFFER_EMPTY									= 30,	// Buffer is empty
	ERROR_BUFFER_FULL									= 31,	// Buffer is full
	ERROR_BUFFER_NOT_ENOUGH_ELEMENTS					= 32,	// Not enough space in buffer to perform operation
	ERROR_BUFFER_NOT_ENOUGH_SPACE						= 33,	// Not enough space in buffer to perform operation
	ERROR_BUFFER_POINTER_NULL							= 34,	// Buffer size was set to zero
	ERROR_BUFFER_SIZE_TOO_LARGE							= 35,	// Buffer size was set to a large value
	ERROR_BUFFER_SIZE_TOO_SMALL							= 36,	// Buffer size was set to a very small value
	ERROR_BUFFER_SIZE_ZERO								= 37,	// Buffer size was set to zero

	// Errors related to pointers
	ERROR_FUNCTION_POINTER_NULL							= 40,	// TODO: Describe parameter
	ERROR_HANDLER_POINTER_NULL							= 41,	// TODO: Describe parameter

	// Errors related to the DateTime class
	ERROR_DATETIME_INVALID_MONTH_DAY					= 50,	// TODO: Describe parameter
	ERROR_DATETIME_INVALID_MONTH						= 51,	// TODO: Describe parameter
	ERROR_DATETIME_INVALID_YEAR							= 52,	// TODO: Describe parameter
	ERROR_DATETIME_INVALID_HOUR							= 53,	// TODO: Describe parameter
	ERROR_DATETIME_INVALID_MINUTE						= 54,	// TODO: Describe parameter
	ERROR_DATETIME_INVALID_SECOND						= 55,	// TODO: Describe parameter
	ERROR_DATETIME_INVALID_MILLISECOND					= 56,	// TODO: Describe parameter
	ERROR_DATETIME_INVALID_TIMEZONE						= 57,	// TODO: Describe parameter
	ERROR_DATETIME_DATE_NOT_INITIALIZED					= 58,	// TODO: Describe parameter
	ERROR_DATETIME_TIME_NOT_INITIALIZED					= 59,	// TODO: Describe parameter

	// Errors related to clock configuration
	ERROR_CLOCK_PRESCALER_CHANGE_FAILED					= 60,	// Clock prescaler change failed
	ERROR_CLOCK_PRESCALER_UNSUPPORTED					= 61,	// Clock prescaler configuration unsupported
	ERROR_CLOCK_SPEED_TOO_HIGH							= 62,	// TODO: Describe parameter
	ERROR_CLOCK_SPEED_TOO_LOW							= 63,	// TODO: Describe parameter

	// Errors related to communication to devices
	ERROR_COMMUNICATION_FAILED							= 70,	// TODO: Describe parameter
	ERROR_COMMUNICATION_PORT_NOT_SET					= 71,	// TODO: Describe parameter
	ERROR_COMMUNICATION_TIME_OUT						= 72,	// TODO: Describe parameter
	ERROR_COMMUNICATION_DEVICE_ID_MATCH_FAILED			= 73,	// TODO: Describe parameter
	ERROR_COMMUNICATION_NO_DEVICE_SELECTED				= 74,	// TODO: Describe parameter

	// Errors related to communication protocol
	ERROR_CHECKSUM_ERROR								= 80,	// TODO: Describe parameter
	ERROR_FRAME_ERROR									= 81,	// TODO: Describe parameter
	ERROR_PACKAGE_AWAITING								= 82,	// Try to write data to a ready package
	ERROR_PACKAGE_NOT_READY								= 83,	// Try to read data from a not ready package

	// Errors related to external interrupts
	ERROR_INTERRUPT_NOT_AVAILABLE						= 90,	// TODO: Describe parameter
	ERROR_PORT_OUTPUT_MODE_UNSUPPORTED					= 91,	// TODO: Describe parameter
	ERROR_SENSE_MODE_UNSUPPORTED						= 92,	// TODO: Describe parameter

	// Errors related to USART
	ERROR_USART_CONFIGURATION_PARAMETER_WRONG			= 100,	// TODO: Describe parameter
	ERROR_USART_UNSUPPORTED_CONFIGURATION_BAUD_RATE		= 101,	// TODO: Describe parameter
	ERROR_USART_UNSUPPORTED_CONFIGURATION_DATA_BITS		= 102,	// TODO: Describe parameter
	ERROR_USART_UNSUPPORTED_CONFIGURATION_FRAME_FORMAT	= 103,	// TODO: Describe parameter
	ERROR_USART_UNSUPPORTED_CONFIGURATION_PARITY		= 104,	// TODO: Describe parameter
	ERROR_USART_UNSUPPORTED_CONFIGURATION_STOP_BIT		= 105,	// TODO: Describe parameter

	// Errors related to the Package API class
	ERROR_PACKAGE_API_NULL_PAYLOAD_SIZE					= 110,	// TODO: Describe parameter
	ERROR_PACKAGE_API_NULL_PAYLOAD_DATA					= 111,	// TODO: Describe parameter
	ERROR_PACKAGE_API_INVALID_SOURCE_ADDRESS			= 112,	// TODO: Describe parameter
	ERROR_PACKAGE_API_PACKAGE_NOT_READY					= 113,	// TODO: Describe parameter
	ERROR_PACKAGE_API_PACKAGE_DATA_FULL					= 114,	// TODO: Describe parameter

	// Errors related to IMU sensors
	ERROR_AXIS_INVALID									= 120,	// TODO: Describe parameter

	// SPI peripheral specific erros
	ERROR_SPI_BUSY_FLAG									= 130,	// TODO: Describe parameter
	ERROR_SPI_CRC										= 131,	// TODO: Describe parameter
	ERROR_SPI_DMA										= 132,	// TODO: Describe parameter
	ERROR_SPI_MODE_FAULT								= 133,	// TODO: Describe parameter
	ERROR_SPI_OVERRUN									= 134,	// TODO: Describe parameter

} error_e;

// -----------------------------------------------------------------------------
// New Data Type - Enumerations => parametric table that doesn't maintain value equivalence with datasheet's values

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Flags => enumerations that can be combined in the form of flags

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Structures, Unions and Bit Fields ---------------------------

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Classes -----------------------------------------------------

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// Public functions declarations
// =============================================================================

// NONE

#endif // __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
