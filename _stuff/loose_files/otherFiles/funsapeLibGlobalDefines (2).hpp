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
#include "main.h"

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

typedef enum activation_t
{
    DEACTIVATE							= false,
    ACTIVATE							= true
} activation_t;

typedef enum edge_t
{
    EDGE_FALLING						= false,
    EDGE_RISING							= true
} edge_t;

typedef enum logicLevel_t
{
    LOGIC_LOW							= false,
    LOGIC_HIGH							= true
} logicLevel_t;

typedef enum outputDriver_t
{
    DRIVER_OPEN_DRAIN					= false,
    DRIVER_PUSH_PULL					= true
} outputDriver_t;

typedef enum planarDirectionHorizontal_t
{
    HORIZONTAL_LEFT						= false,
    HORIZONTAL_RIGHT					= true
} planarDirectionHorizontal_t;

typedef enum planarDirectionVertical_t
{
    VERTICAL_DOWN						= false,
    VERTICAL_UP							= true
} planarDirectionVertical_t;

typedef enum rotacionalDirection_t
{
    ROTATE_CLOCKWISE					= false,
    ROTATE_COUNTERCLOCKWISE				= true
} rotacionalDirection_t;

// -----------------------------------------------------------------------------
// Single variables => small enumerations that can be understood as a single variable

typedef enum planarDirection_t
{
    PLANAR_DOWN							= 0,
    PLANAR_UP							= 1,
    PLANAR_LEFT							= 2,
    PLANAR_RIGHT						= 3
} planarDirection_t;

typedef enum axis_t : uint8_t
{
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
typedef enum  error_e : uint16_t
{
    // Errors related to general identifiers
    ERROR_NONE											= 0x0000,	// Success - No errors occurred
    ERROR_LOCKED										= 0x0001,	// Accessed a locked device
    ERROR_MEMORY_ALLOCATION								= 0x0002,	// Memory allocation failed
    ERROR_NOT_INITIALIZED								= 0x0003,	// Not initialized
    ERROR_READ_PROTECTED								= 0x0004,	// Tried to read a read protected device
    ERROR_WRITE_PROTECTED								= 0x0006,	// Tried to write a write protected device
    ERROR_WRONG_CONFIGURATION							= 0x0007,	// TODO: Describe parameter
    ERROR_OPERATION_MODE_UNSUPPORTED					= 0x0008,	// TODO: Describe parameter
    ERROR_PERIPHERAL_NOT_READY							= 0x0009,	// TODO: Describe parameter
    ERROR_IS_BUSY										= 0x000A,	// TODO: Describe parameter
    ERROR_CHARACTER_INVALID								= 0x000B,	// The character is not valid
    ERROR_NOT_READY										= 0x000C,	// The process is nor ready
    ERROR_UNTIL_IN_DEVELOPMENT							= 0x000D,	// This part of the code is still under development
//	ERROR_OUTPUT_COMPARE_UNSUPPORTED					= 0x000B,	// TODO: Describe parameter

    // Errors related to arguments
    ERROR_ARGUMENT_GENERIC_ERROR						= 0x0010,	// A generic error occurred (See documentation for further information)
    ERROR_ARGUMENT_CANNOT_BE_ZERO						= 0x0011,	// TODO: Describe parameter
    ERROR_ARGUMENT_POINTER_NULL							= 0x0012,	// NULL pointer was passed as an argument to function
    ERROR_ARGUMENTS_MISMATCH							= 0x0013,	// A function was called with dubious argument values
    ERROR_ARGUMENTS_WRONG_NUMBER						= 0x0014,	// A function was called with wrong number of arguments

    // Errors related to buffers
    ERROR_BUFFER_GENERIC_ERROR							= 0x0020,	// A generic error occurred (See documentation for further information)
    ERROR_BUFFER_EMPTY									= 0x0021,	// Buffer is empty
    ERROR_BUFFER_FULL									= 0x0022,	// Buffer is full
    ERROR_BUFFER_NOT_ENOUGH_ELEMENTS					= 0x0023,	// Not enough space in buffer to perform operation
    ERROR_BUFFER_NOT_ENOUGH_SPACE						= 0x0024,	// Not enough space in buffer to perform operation
    ERROR_BUFFER_POINTER_NULL							= 0x0025,	// Buffer size was set to zero
    ERROR_BUFFER_SIZE_TOO_LARGE							= 0x0026,	// Buffer size was set to a large value
    ERROR_BUFFER_SIZE_TOO_SMALL							= 0x0027,	// Buffer size was set to a very small value
    ERROR_BUFFER_SIZE_ZERO								= 0x0028,	// Buffer size was set to zero

    // Errors related to pointers
    ERROR_POINTER_GENERIC_ERROR							= 0x0030,	// A generic error occurred (See documentation for further information)
    ERROR_FUNCTION_POINTER_NULL							= 0x0031,	// TODO: Describe parameter
    ERROR_HANDLER_POINTER_NULL							= 0x0032,	// TODO: Describe parameter

    // Errors related to the DateTime class
    ERROR_DATETIME_GENERIC_ERROR						= 0x0040,	// A generic error occurred (See documentation for further information)
    ERROR_DATETIME_INVALID_MONTH_DAY					= 0x0041,	// TODO: Describe parameter
    ERROR_DATETIME_INVALID_MONTH						= 0x0042,	// TODO: Describe parameter
    ERROR_DATETIME_INVALID_YEAR							= 0x0043,	// TODO: Describe parameter
    ERROR_DATETIME_INVALID_HOUR							= 0x0044,	// TODO: Describe parameter
    ERROR_DATETIME_INVALID_MINUTE						= 0x0045,	// TODO: Describe parameter
    ERROR_DATETIME_INVALID_SECOND						= 0x0046,	// TODO: Describe parameter
    ERROR_DATETIME_INVALID_MILLISECOND					= 0x0047,	// TODO: Describe parameter
    ERROR_DATETIME_INVALID_TIMEZONE						= 0x0048,	// TODO: Describe parameter
    ERROR_DATETIME_DATE_NOT_INITIALIZED					= 0x0049,	// TODO: Describe parameter
    ERROR_DATETIME_TIME_NOT_INITIALIZED					= 0x004A,	// TODO: Describe parameter

    // Errors related to clock configuration
    ERROR_CLOCK_GENERIC_ERROR							= 0x0050,	// A generic error occurred (See documentation for further information)
    ERROR_CLOCK_PRESCALER_CHANGE_FAILED					= 0x0051,	// Clock prescaler change failed
    ERROR_CLOCK_PRESCALER_UNSUPPORTED					= 0x0052,	// Clock prescaler configuration unsupported
    ERROR_CLOCK_SPEED_TOO_HIGH							= 0x0053,	// TODO: Describe parameter
    ERROR_CLOCK_SPEED_TOO_LOW							= 0x0054,	// TODO: Describe parameter

    // Errors related to communication to devices
    ERROR_COMMUNICATION_GENERIC_ERROR					= 0x0060,	// A generic error occurred (See documentation for further information)
    ERROR_COMMUNICATION_FAILED							= 0x0061,	// TODO: Describe parameter
    ERROR_COMMUNICATION_PORT_NOT_SET					= 0x0062,	// TODO: Describe parameter
    ERROR_COMMUNICATION_TIME_OUT						= 0x0063,	// TODO: Describe parameter
    ERROR_COMMUNICATION_DEVICE_ID_MATCH_FAILED			= 0x0064,	// TODO: Describe parameter
    ERROR_COMMUNICATION_NO_DEVICE_SELECTED				= 0x0065,	// TODO: Describe parameter

    // Errors related to communication protocol
    ERROR_PROTOCOL_GENERIC_ERROR						= 0x0070,	// A generic error occurred (See documentation for further information)
    ERROR_CHECKSUM_ERROR								= 0x0071,	// TODO: Describe parameter
    ERROR_FRAME_ERROR									= 0x0072,	// TODO: Describe parameter
    ERROR_PACKAGE_AWAITING								= 0x0073,	// Try to write data to a ready package
    ERROR_PACKAGE_NOT_READY								= 0x0074,	// Try to read data from a not ready package

    // Errors related to external interrupts
    ERROR_INTERRUPT_GENERIC_ERROR						= 0x0080,	// A generic error occurred (See documentation for further information)
    ERROR_INTERRUPT_NOT_AVAILABLE						= 0x0083,	// TODO: Describe parameter
    ERROR_PORT_OUTPUT_MODE_UNSUPPORTED					= 0x0082,	// TODO: Describe parameter
    ERROR_SENSE_MODE_UNSUPPORTED						= 0x0081,	// TODO: Describe parameter

    // Errors related to USART
    ERROR_USART_GENERIC_ERROR							= 0x0090,	// A generic error occurred (See documentation for further information)
    ERROR_USART_CONFIGURATION_PARAMETER_WRONG			= 0x0091,	// TODO: Describe parameter
    ERROR_USART_UNSUPPORTED_CONFIGURATION_BAUD_RATE		= 0x0092,	// TODO: Describe parameter
    ERROR_USART_UNSUPPORTED_CONFIGURATION_DATA_BITS		= 0x0093,	// TODO: Describe parameter
    ERROR_USART_UNSUPPORTED_CONFIGURATION_FRAME_FORMAT	= 0x0094,	// TODO: Describe parameter
    ERROR_USART_UNSUPPORTED_CONFIGURATION_PARITY		= 0x0095,	// TODO: Describe parameter
    ERROR_USART_UNSUPPORTED_CONFIGURATION_STOP_BIT		= 0x0096,	// TODO: Describe parameter

    // Errors related to the Package API class
    ERROR_PACKAGE_API_GENERIC_ERROR						= 0x00A0,	// A generic error occurred (See documentation for further information)
    ERROR_PACKAGE_API_NULL_PAYLOAD_SIZE					= 0x00A1,	// TODO: Describe parameter
    ERROR_PACKAGE_API_NULL_PAYLOAD_DATA					= 0x00A2,	// TODO: Describe parameter
    ERROR_PACKAGE_API_INVALID_SOURCE_ADDRESS			= 0x00A3,	// TODO: Describe parameter
    ERROR_PACKAGE_API_PACKAGE_NOT_READY					= 0x00A4,	// TODO: Describe parameter
    ERROR_PACKAGE_API_PACKAGE_DATA_FULL					= 0x00A5,	// TODO: Describe parameter

    // Errors related to IMU sensors
    ERROR_IMU_GENERIC_ERROR								= 0x00B0,	// A generic error occurred (See documentation for further information)
    ERROR_AXIS_INVALID									= 0x00B1,	// TODO: Describe parameter

    // Errors related to SPI peripherals
    ERROR_SPI_GENERIC_ERROR								= 0x00C0,	// A generic error occurred (See documentation for further information)
    ERROR_SPI_BUSY_FLAG									= 0x00C1,	// TODO: Describe parameter
    ERROR_SPI_CRC										= 0x00C2,	// TODO: Describe parameter
    ERROR_SPI_DMA										= 0x00C3,	// TODO: Describe parameter
    ERROR_SPI_MODE_FAULT								= 0x00C4,	// TODO: Describe parameter
    ERROR_SPI_OVERRUN									= 0x00C5,	// TODO: Describe parameter

    ERROR_TEMP_00	= 1000,
    ERROR_TEMP_01	= 1001,
    ERROR_TEMP_02	= 1002,
    ERROR_TEMP_03	= 1003,
    ERROR_TEMP_04	= 1004,
    ERROR_TEMP_05	= 1005,
    ERROR_TEMP_06	= 1006,
    ERROR_TEMP_07	= 1007,
    ERROR_TEMP_08	= 1008,
    ERROR_TEMP_09	= 1009,
    ERROR_TEMP_10	= 1010,
    ERROR_TEMP_11	= 1011,
    ERROR_TEMP_12	= 1012,
    ERROR_TEMP_13	= 1013,
    ERROR_TEMP_14	= 1014,
    ERROR_TEMP_15	= 1015,
    ERROR_TEMP_16	= 1016,
    ERROR_TEMP_17	= 1017,
    ERROR_TEMP_18	= 1018,
    ERROR_TEMP_19	= 1019,
    ERROR_TEMP_20	= 1020,
    ERROR_TEMP_21	= 1021,
    ERROR_TEMP_22	= 1022,
    ERROR_TEMP_23	= 1023,
    ERROR_TEMP_24	= 1024,
    ERROR_TEMP_25	= 1025,
    ERROR_TEMP_26	= 1026,
    ERROR_TEMP_27	= 1027,
    ERROR_TEMP_28	= 1028,
    ERROR_TEMP_29	= 1029,
    ERROR_TEMP_30	= 1030,
    ERROR_TEMP_31	= 1031,
    ERROR_TEMP_32	= 1032,
    ERROR_TEMP_33	= 1033,
    ERROR_TEMP_34	= 1034,
    ERROR_TEMP_35	= 1035,
    ERROR_TEMP_36	= 1036,
    ERROR_TEMP_37	= 1037,
    ERROR_TEMP_38	= 1038,
    ERROR_TEMP_39	= 1039,
    ERROR_TEMP_40	= 1040,
    ERROR_TEMP_41	= 1041,
    ERROR_TEMP_42	= 1042,
    ERROR_TEMP_43	= 1043,
    ERROR_TEMP_44	= 1044,
    ERROR_TEMP_45	= 1045,
    ERROR_TEMP_46	= 1046,
    ERROR_TEMP_47	= 1047,
    ERROR_TEMP_48	= 1048,
    ERROR_TEMP_49	= 1049,
    ERROR_TEMP_50	= 1050,
    ERROR_TEMP_51	= 1051,
    ERROR_TEMP_52	= 1052,
    ERROR_TEMP_53	= 1053,
    ERROR_TEMP_54	= 1054,
    ERROR_TEMP_55	= 1055,
    ERROR_TEMP_56	= 1056,
    ERROR_TEMP_57	= 1057,
    ERROR_TEMP_58	= 1058,
    ERROR_TEMP_59	= 1059,
    ERROR_TEMP_60	= 1060,
    ERROR_TEMP_61	= 1061,
    ERROR_TEMP_62	= 1062,
    ERROR_TEMP_63	= 1063,
    ERROR_TEMP_64	= 1064,
    ERROR_TEMP_65	= 1065,
    ERROR_TEMP_66	= 1066,
    ERROR_TEMP_67	= 1067,
    ERROR_TEMP_68	= 1068,
    ERROR_TEMP_69	= 1069,
    ERROR_TEMP_70	= 1070,
    ERROR_TEMP_71	= 1071,
    ERROR_TEMP_72	= 1072,
    ERROR_TEMP_73	= 1073,
    ERROR_TEMP_74	= 1074,
    ERROR_TEMP_75	= 1075,
    ERROR_TEMP_76	= 1076,
    ERROR_TEMP_77	= 1077,
    ERROR_TEMP_78	= 1078,
    ERROR_TEMP_79	= 1079,
    ERROR_TEMP_80	= 1080,
    ERROR_TEMP_81	= 1081,
    ERROR_TEMP_82	= 1082,
    ERROR_TEMP_83	= 1083,
    ERROR_TEMP_84	= 1084,
    ERROR_TEMP_85	= 1085,
    ERROR_TEMP_86	= 1086,
    ERROR_TEMP_87	= 1087,
    ERROR_TEMP_88	= 1088,
    ERROR_TEMP_89	= 1089,
    ERROR_TEMP_90	= 1090,
    ERROR_TEMP_91	= 1091,
    ERROR_TEMP_92	= 1092,
    ERROR_TEMP_93	= 1093,
    ERROR_TEMP_94	= 1094,
    ERROR_TEMP_95	= 1095,
    ERROR_TEMP_96	= 1096,
    ERROR_TEMP_97	= 1097,
    ERROR_TEMP_98	= 1098,
    ERROR_TEMP_99	= 1099,

    ERROR_UNKNOWN										= 0xFFFF,	// Something really unpredictable has happen
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
