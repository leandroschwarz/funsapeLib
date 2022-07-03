/* =============================================================================
 * Project:			FunSAPE MCU++ Embedded Library
 * File name:		flGlobalDefines.hpp
 * Module:			Global definitions for FunSAPE MCU++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FL_GLOBAL_DEFINES_HPP
#define __FL_GLOBAL_DEFINES_HPP					__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Project configuration file --------------------------------------------------

#include "flProjectConfig.hpp"
#ifndef __FL_PROJECT_CONFIG_HPP
#	error	[flGlobalDefines.hpp] Error 7 - Project configuration file (flProjectConfig.hpp) is missing!
#elif __FL_PROJECT_CONFIG_HPP != __BUILD_TO_REPLACE__
#	error	[flGlobalDefines.hpp] Error 8 - Build mismatch between project configuration file (flProjectConfig.hpp) and global definitions file (flGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - Basic header for all files -----------------------------------

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "all/allDevices.hpp"
#include "all/allGpio.hpp"

// =============================================================================
// Undefining previous definitions
// =============================================================================

#undef byteNibbleHigh
#undef byteNibbleLow
#undef byteSwitchNibbles
#undef clrBit
#undef clrMask
#undef convertToLowerCase
#undef convertToUpperCase
#undef cplBit
#undef cplMask
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
#undef maximumOf
#undef minimumOf
#undef setBit
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
// Macro-functions
// *INDENT-OFF*
// =============================================================================

// -----------------------------------------------------------------------------
// Bit handling ----------------------------------------------------------------


#define clrBit(reg, bit)				((reg) &= (~(1 << (bit))))
#define cplBit(reg, bit)				((reg) ^= (1 << (bit)))
#define setBit(reg, bit)				((reg) |= (1 << (bit)))
#define isBitClr(reg, bit)				((bool)(!(((reg) >> (bit)) & 1)))
#define isBitSet(reg, bit)				((bool)(((reg) >> (bit)) & 0x01))

// -----------------------------------------------------------------------------
// Bit loop --------------------------------------------------------------------

#define waitUntilBitIsClear(reg, bit)	(do {} while(isBitSet((reg), (bit))))
#define waitUntilBitIsSet(reg, bit)		(do {} while(isBitClr((reg), (bit))))

// -----------------------------------------------------------------------------
// Bit mask handling -----------------------------------------------------------

#define clrMaskOffset(reg, mask, offset)		((reg) &= ~((mask) << (offset)))
#define cplMaskOffset(reg, mask, offset)		((reg) ^= ((mask) << (offset)))
#define setMaskOffset(reg, mask, offset)		((reg) |= ((mask) << (offset)))

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

#define getMaximumOf(var1 , var2)				((var1 > var2) ? var1 : var2)
#define getMinimumOf(var1 , var2)				((var1 < var2) ? var1 : var2)
#define truncateBetween(var, minimum, maximum)	(var < minimum) ? minimum : ((var > maximum) ? maximum : var))

// -----------------------------------------------------------------------------
// Stringification of macro arguments ------------------------------------------

#define stringifyName(constant)			#constant
#define stringifyValue(constant)		stringifyName(constant)

// -----------------------------------------------------------------------------
// System actions --------------------------------------------------------------

#define doNop()							do {asm volatile("nop");} while(0)
#define doNothing()						{}
#define systemHalt()					while(1) {}

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

enum class BlockingMode : bool {
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

enum class Direction : uint8_t {
    LEFT								= 0,
    RIGHT								= 1,
    UP									= 2,
    DOWN								= 3
};

// -----------------------------------------------------------------------------
// Enumerations ----------------------------------------------------------------
//     => Large enumerations or enumerations with expansible number of elements

enum class Error : uint16_t {
    // Errors related to general identifiers
    NONE												= 0x0000,	// Success - No errors occurred

    // Errors related to arguments
    // Errors related to buffers
    // Errors related to clock configuration
    // Errors related to communication protocol
    // Errors related to communication to devices
    // Errors related to external interrupts
    // Errors related to IMU sensors
    // Errors related to pointers
    // Errors related to SPI peripherals
    // Errors related to the DateTime class
    // Errors related to the Package API class
    // Errors related to USART
//	ARGUMENT_CANNOT_BE_ZERO						= 0x0011,	// TODO: Describe parameter
//	ARGUMENT_GENERIC_ERROR						= 0x0010,	// A generic error occurred (See documentation for further information)
//	ARGUMENT_POINTER_NULL							= 0x0012,	// NULL pointer was passed as an argument to function
//	ARGUMENTS_MISMATCH							= 0x0013,	// A function was called with dubious argument values
//	ARGUMENTS_WRONG_NUMBER						= 0x0014,	// A function was called with wrong number of arguments
//	AXIS_INVALID									= 0x00B1,	// TODO: Describe parameter
//	BUFFER_EMPTY									= 0x0021,	// Buffer is empty
//	BUFFER_FULL									= 0x0022,	// Buffer is full
//	BUFFER_GENERIC_ERROR							= 0x0020,	// A generic error occurred (See documentation for further information)
//	BUFFER_NOT_ENOUGH_ELEMENTS					= 0x0023,	// Not enough space in buffer to perform operation
//	BUFFER_NOT_ENOUGH_SPACE						= 0x0024,	// Not enough space in buffer to perform operation
//	BUFFER_POINTER_NULL							= 0x0025,	// Buffer size was set to zero
//	BUFFER_SIZE_TOO_LARGE							= 0x0026,	// Buffer size was set to a large value
//	BUFFER_SIZE_TOO_SMALL							= 0x0027,	// Buffer size was set to a very small value
//	BUFFER_SIZE_ZERO								= 0x0028,	// Buffer size was set to zero
//	CHARACTER_INVALID								= 0x000B,	// The character is not valid
//	CHECKSUM_ERROR								= 0x0071,	// TODO: Describe parameter
//	CLOCK_GENERIC_ERROR							= 0x0050,	// A generic error occurred (See documentation for further information)
//	CLOCK_PRESCALER_CHANGE_FAILED					= 0x0051,	// Clock prescaler change failed
//	CLOCK_PRESCALER_UNSUPPORTED					= 0x0052,	// Clock prescaler configuration unsupported
//	CLOCK_SPEED_TOO_HIGH							= 0x0053,	// TODO: Describe parameter
//	CLOCK_SPEED_TOO_LOW							= 0x0054,	// TODO: Describe parameter
//	COMMUNICATION_DEVICE_ID_MATCH_FAILED			= 0x0064,	// TODO: Describe parameter
//	COMMUNICATION_FAILED							= 0x0061,	// TODO: Describe parameter
//	COMMUNICATION_GENERIC_ERROR					= 0x0060,	// A generic error occurred (See documentation for further information)
//	COMMUNICATION_NO_DEVICE_SELECTED				= 0x0065,	// TODO: Describe parameter
//	COMMUNICATION_PORT_NOT_SET					= 0x0062,	// TODO: Describe parameter
//	COMMUNICATION_TIME_OUT						= 0x0063,	// TODO: Describe parameter
//	DATETIME_DATE_NOT_INITIALIZED					= 0x0049,	// TODO: Describe parameter
//	DATETIME_GENERIC_ERROR						= 0x0040,	// A generic error occurred (See documentation for further information)
//	DATETIME_INVALID_HOUR							= 0x0044,	// TODO: Describe parameter
//	DATETIME_INVALID_MILLISECOND					= 0x0047,	// TODO: Describe parameter
//	DATETIME_INVALID_MINUTE						= 0x0045,	// TODO: Describe parameter
//	DATETIME_INVALID_MONTH						= 0x0042,	// TODO: Describe parameter
//	DATETIME_INVALID_MONTH_DAY					= 0x0041,	// TODO: Describe parameter
//	DATETIME_INVALID_SECOND						= 0x0046,	// TODO: Describe parameter
//	DATETIME_INVALID_TIMEZONE						= 0x0048,	// TODO: Describe parameter
//	DATETIME_INVALID_YEAR							= 0x0043,	// TODO: Describe parameter
//	DATETIME_TIME_NOT_INITIALIZED					= 0x004A,	// TODO: Describe parameter
//	FRAME_ERROR									= 0x0072,	// TODO: Describe parameter
//	FUNCTION_POINTER_NULL							= 0x0031,	// TODO: Describe parameter
//	HANDLER_POINTER_NULL							= 0x0032,	// TODO: Describe parameter
//	IMU_GENERIC_ERROR								= 0x00B0,	// A generic error occurred (See documentation for further information)
//	INTERRUPT_GENERIC_ERROR						= 0x0080,	// A generic error occurred (See documentation for further information)
//	INTERRUPT_NOT_AVAILABLE						= 0x0083,	// TODO: Describe parameter
//	IS_BUSY										= 0x000A,	// TODO: Describe parameter
//	LOCKED										= 0x0001,	// Accessed a locked device
//	MEMORY_ALLOCATION								= 0x0002,	// Memory allocation failed
//	NOT_INITIALIZED								= 0x0003,	// Not initialized
//	NOT_READY										= 0x000C,	// The process is nor ready
//	OPERATION_MODE_UNSUPPORTED					= 0x0008,	// TODO: Describe parameter
//	PACKAGE_API_GENERIC_ERROR						= 0x00A0,	// A generic error occurred (See documentation for further information)
//	PACKAGE_API_INVALID_SOURCE_ADDRESS			= 0x00A3,	// TODO: Describe parameter
//	PACKAGE_API_NULL_PAYLOAD_DATA					= 0x00A2,	// TODO: Describe parameter
//	PACKAGE_API_NULL_PAYLOAD_SIZE					= 0x00A1,	// TODO: Describe parameter
//	PACKAGE_API_PACKAGE_DATA_FULL					= 0x00A5,	// TODO: Describe parameter
//	PACKAGE_API_PACKAGE_NOT_READY					= 0x00A4,	// TODO: Describe parameter
//	PACKAGE_AWAITING								= 0x0073,	// Try to write data to a ready package
//	PACKAGE_NOT_READY								= 0x0074,	// Try to read data from a not ready package
//	PERIPHERAL_NOT_READY							= 0x0009,	// TODO: Describe parameter
//	POINTER_GENERIC_ERROR							= 0x0030,	// A generic error occurred (See documentation for further information)
//	PORT_OUTPUT_MODE_UNSUPPORTED					= 0x0082,	// TODO: Describe parameter
//	PROTOCOL_GENERIC_ERROR						= 0x0070,	// A generic error occurred (See documentation for further information)
//	READ_PROTECTED								= 0x0004,	// Tried to read a read protected device
//	SENSE_MODE_UNSUPPORTED						= 0x0081,	// TODO: Describe parameter
//	SPI_BUSY_FLAG									= 0x00C1,	// TODO: Describe parameter
//	SPI_CRC										= 0x00C2,	// TODO: Describe parameter
//	SPI_DMA										= 0x00C3,	// TODO: Describe parameter
//	SPI_GENERIC_ERROR								= 0x00C0,	// A generic error occurred (See documentation for further information)
//	SPI_MODE_FAULT								= 0x00C4,	// TODO: Describe parameter
//	SPI_OVERRUN									= 0x00C5,	// TODO: Describe parameter
//	TEMP_00	= 1000,
//	TEMP_01	= 1001,
//	TEMP_02	= 1002,
//	TEMP_03	= 1003,
//	TEMP_04	= 1004,
//	TEMP_05	= 1005,
//	TEMP_06	= 1006,
//	TEMP_07	= 1007,
//	TEMP_08	= 1008,
//	TEMP_09	= 1009,
//	TEMP_10	= 1010,
//	TEMP_11	= 1011,
//	TEMP_12	= 1012,
//	TEMP_13	= 1013,
//	TEMP_14	= 1014,
//	TEMP_15	= 1015,
//	TEMP_16	= 1016,
//	TEMP_17	= 1017,
//	TEMP_18	= 1018,
//	TEMP_19	= 1019,
//	TEMP_20	= 1020,
//	TEMP_21	= 1021,
//	TEMP_22	= 1022,
//	TEMP_23	= 1023,
//	TEMP_24	= 1024,
//	TEMP_25	= 1025,
//	TEMP_26	= 1026,
//	TEMP_27	= 1027,
//	TEMP_28	= 1028,
//	TEMP_29	= 1029,
//	TEMP_30	= 1030,
//	TEMP_31	= 1031,
//	TEMP_32	= 1032,
//	TEMP_33	= 1033,
//	TEMP_34	= 1034,
//	TEMP_35	= 1035,
//	TEMP_36	= 1036,
//	TEMP_37	= 1037,
//	TEMP_38	= 1038,
//	TEMP_39	= 1039,
//	TEMP_40	= 1040,
//	TEMP_41	= 1041,
//	TEMP_42	= 1042,
//	TEMP_43	= 1043,
//	TEMP_44	= 1044,
//	TEMP_45	= 1045,
//	TEMP_46	= 1046,
//	TEMP_47	= 1047,
//	TEMP_48	= 1048,
//	TEMP_49	= 1049,
//	TEMP_50	= 1050,
//	TEMP_51	= 1051,
//	TEMP_52	= 1052,
//	TEMP_53	= 1053,
//	TEMP_54	= 1054,
//	TEMP_55	= 1055,
//	TEMP_56	= 1056,
//	TEMP_57	= 1057,
//	TEMP_58	= 1058,
//	TEMP_59	= 1059,
//	TEMP_60	= 1060,
//	TEMP_61	= 1061,
//	TEMP_62	= 1062,
//	TEMP_63	= 1063,
//	TEMP_64	= 1064,
//	TEMP_65	= 1065,
//	TEMP_66	= 1066,
//	TEMP_67	= 1067,
//	TEMP_68	= 1068,
//	TEMP_69	= 1069,
//	TEMP_70	= 1070,
//	TEMP_71	= 1071,
//	TEMP_72	= 1072,
//	TEMP_73	= 1073,
//	TEMP_74	= 1074,
//	TEMP_75	= 1075,
//	TEMP_76	= 1076,
//	TEMP_77	= 1077,
//	TEMP_78	= 1078,
//	TEMP_79	= 1079,
//	TEMP_80	= 1080,
//	TEMP_81	= 1081,
//	TEMP_82	= 1082,
//	TEMP_83	= 1083,
//	TEMP_84	= 1084,
//	TEMP_85	= 1085,
//	TEMP_86	= 1086,
//	TEMP_87	= 1087,
//	TEMP_88	= 1088,
//	TEMP_89	= 1089,
//	TEMP_90	= 1090,
//	TEMP_91	= 1091,
//	TEMP_92	= 1092,
//	TEMP_93	= 1093,
//	TEMP_94	= 1094,
//	TEMP_95	= 1095,
//	TEMP_96	= 1096,
//	TEMP_97	= 1097,
//	TEMP_98	= 1098,
//	TEMP_99	= 1099,
//	UNKNOWN										= 0xFFFF,	// Something really unpredictable has happen
//	UNTIL_IN_DEVELOPMENT							= 0x000D,	// This part of the code is still under development
//	USART_CONFIGURATION_PARAMETER_WRONG			= 0x0091,	// TODO: Describe parameter
//	USART_GENERIC_ERROR							= 0x0090,	// A generic error occurred (See documentation for further information)
//	USART_UNSUPPORTED_CONFIGURATION_BAUD_RATE		= 0x0092,	// TODO: Describe parameter
//	USART_UNSUPPORTED_CONFIGURATION_DATA_BITS		= 0x0093,	// TODO: Describe parameter
//	USART_UNSUPPORTED_CONFIGURATION_FRAME_FORMAT	= 0x0094,	// TODO: Describe parameter
//	USART_UNSUPPORTED_CONFIGURATION_PARITY		= 0x0095,	// TODO: Describe parameter
//	USART_UNSUPPORTED_CONFIGURATION_STOP_BIT		= 0x0096,	// TODO: Describe parameter
//	WRITE_PROTECTED								= 0x0006,	// Tried to write a write protected device
//	WRONG_CONFIGURATION							= 0x0007,	// TODO: Describe parameter
//	OUTPUT_COMPARE_UNSUPPORTED					= 0x000B,	// TODO: Describe parameter
};

// -----------------------------------------------------------------------------
// Flags -----------------------------------------------------------------------
//     => Enumerations that can be combined in the form of flags ---------------

// NONE

// -----------------------------------------------------------------------------
// Structures, Unions and Bit Fields -------------------------------------------

// NONE

// =============================================================================
// Classes
// =============================================================================

// NONE

// =============================================================================
// External global variables
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

#endif // __FL_GLOBAL_DEFINES_HPP
