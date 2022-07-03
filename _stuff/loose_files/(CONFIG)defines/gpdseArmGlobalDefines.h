/* =============================================================================
 * Project:		GPSDE++ ARM Library
 * File name:	gpdseArmGlobalDefines.h
 * Module:		Global definitions for GPDSE++ ARM Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// File build
// =============================================================================

#ifndef __GPDSE_ARM_GLOBAL_DEFINES_H
#define __GPDSE_ARM_GLOBAL_DEFINES_H			__BUILD_MACRO_TO_BE_CHANGED__

// =============================================================================
// System file dependencies
// =============================================================================

#include "gpdseArmProjectConfig.h"
#if __GPDSE_ARM_PROJECT_CONFIG_H != __BUILD_MACRO_TO_BE_CHANGED__
#	error	Project configurations file (gpdseArmProjectConfig) must be build __BUILD_MACRO_TO_BE_CHANGED__.
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constants
// =============================================================================

// NONE

// =============================================================================
// Macro-functions
// =============================================================================

// Bit handling
#define clrBit(reg, bit)						((reg) &= (~(1 << (bit))))
#define cplBit(reg, bit)						((reg) ^= (1 << (bit)))
#define setBit(reg, bit)						((reg) |= (1 << (bit)))
#define isBitClr(reg, bit)						(bool)(!(((reg) >> (bit)) & 1))
#define isBitSet(reg, bit)						(bool)(((reg) >> (bit)) & 0x01)
// Bit loop
#define waitUntilBitIsClear(reg, bit)			do {} while(isBitSet((reg), (bit)))
#define waitUntilBitIsSet(reg, bit)				do {} while(isBitClr((reg), (bit)))
// Bit mask handling
#define clrMask(reg, mask, offset)				((reg) &= ~((mask) << (offset)))
#define cplMask(reg, mask, offset)				((reg) ^= ((mask) << (offset)))
#define setMask(reg, mask, offset)				((reg) |= ((mask) << (offset)))
// Bit data manipulation
#define byteHighNibble(dataByte)				((dataByte >> 4) & 0x0F)
#define byteLowNibble(dataByte)					(dataByte & 0x0F)
#define byteSwitchNibbles(dataByte)				((byteLowNibble(dataByte) << 4) | byteHighNibble(dataByte))
#define wordHighByte(dataWord)					((dataWord >> 8) & 0x00FF)
#define wordLowByte(dataWord)					(dataWord & 0x00FF)
#define wordSwitchBytes(dataWord)				((wordLowByte(dataWord) << 8) | wordHighByte(dataWord))
#define dwordByte3(dataDword)					((dataDword >> 24) & 0x000000FF)
#define dwordByte2(dataDword)					((dataDword >> 16) & 0x000000FF)
#define dwordByte1(dataDword)					((dataDword >> 8) & 0x000000FF)
#define dwordByte0(dataDword)					(dataDword & 0x000000FF)
#define dwordSwitchBytes(dataWord)				(dwordByte0(dataWord) << 24) | dwordByte1(dataWord) << 16) | dwordByte2(dataWord) << 8) | dwordByte3(dataWord))
#define dwordHighWord(dataDword)				((dataDword >> 16) & 0x0000FFFF)
#define dwordLowWord(dataDword)					(dataDword & 0x0000FFFF)
#define dwordSwitchWords(dataDword)				((dwordLowWord(dataDword) << 8) | dwordHighWord(dataDword))
// Value comparison
#define maximumOf(var1 , var2)					((var1 > var2) ? var1 :var2)
#define truncateBetween(var, minimum, maximum)	(var < minimum) ? minimum : ((var > maximum) ? maximum : var))
#define minimumOf(var1 , var2)					((var1 < var2) ? var1 :var2)
// Stringification of macro arguments
#define stringifyName(constant)					#constant
#define stringifyValue(constant)				stringifyName(constant)
// System actions
#define die(errorCode)							do {printf("Failed with error=%u.\r", errorCode);systemHalt();} while(0)
#define systemHalt()							while(1) {}
#define doNothing()								{}
#define doNop()									// NOP instruction
// I/O manipulation
#define setPin(gpioPort, gpioPin)				HAL_GPIO_WritePin(gpioPort, gpioPin, GPIO_PIN_SET)
#define clrPin(gpioPort, gpioPin)				HAL_GPIO_WritePin(gpioPort, gpioPin, GPIO_PIN_RESET)
#define cplPin(gpioPort, gpioPin)				HAL_GPIO_TogglePin(gpioPort, gpioPin)
#define delayMs(time)							HAL_Delay(time)

// =============================================================================
// New data types
// =============================================================================

// Single variables => shortening the name of single data types
typedef volatile uint8_t						reg_t;
typedef volatile int8_t							vint8_t;
typedef volatile int16_t						vint16_t;
typedef volatile int32_t						vint32_t;
typedef volatile int64_t						vint64_t;
typedef volatile uint8_t						vuint8_t;
typedef volatile uint16_t						vuint16_t;
typedef volatile uint32_t						vuint32_t;
typedef volatile uint64_t						vuint64_t;
typedef int32_t									INT_T;		// 16-bit (AVR) or 32-bit (ARM)
typedef uint32_t								UINT_T;		// 16-bit (AVR) or 32-bit (ARM)

// Single variables => enumerations understood as logic values
typedef enum logicLevel_t {
	LOW		= 0,
	HIGH	= 1,
} logicLevel_t;

typedef enum rotacionalDirection_t {
	CLOCKWISE			= 0,
	COUNTERCLOCKWISE	= 1
} rotacionalDirection_t;

typedef enum edge_t {
	FALLING_EDGE	= 0,
	RISING_EDGE		= 1
} edge_t;

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

typedef enum planarDirection_t {
	LEFT	= 0,
	RIGHT	= 1,
	UP		= 2,
	DOWN	= 3
} planarDirection_t;

typedef enum outputDriver_t {
	PUSH_PULL		= 0,
	OPEN_DRAIN		= 1,
} outputDriver_t;

// Enumerations => large enumerations or enumerations with expansible number of elements
typedef enum error_e {
	// Generic error identifiers
	ERROR_NONE									= 0,  // Success - No errors occurred
	ERROR_LOCKED								= 1,  // Accessed a locked device
	ERROR_MEMORY_ALLOCATION						= 2,  // Memory allocation failed
	ERROR_NOT_INITIALIZED						= 3,  // Not initialized
	ERROR_READ_PROTECTED						= 4,  // Tried to read a read protected device
	ERROR_UNKNOWN								= 5,  // Something really unpredictable has happen
	ERROR_WRITE_PROTECTED						= 6,  // Tried to write a write protected device
	ERROR_WRONG_CONFIGURATION					= 7,
	ERROR_OPERATION_MODE_UNSUPPORTED			= 8,
	ERROR_OUTPUT_COMPARE_UNSUPPORTED			= 9,

	// Error in arguments
	ERROR_ARGUMENT_CANNOT_BE_ZERO				= 10,
	ERROR_ARGUMENT_POINTER_NULL					= 11, // NULL pointer was passed as an argument to function
	ERROR_ARGUMENTS_MISMATCH					= 12, // A function was called with dubious argument values
	ERROR_ARGUMENTS_WRONG_NUMBER				= 13, // A function was called with wrong number of arguments

	// Error in buffers
	ERROR_BUFFER_EMPTY							= 20, // Buffer is empty
	ERROR_BUFFER_FULL							= 21, // Buffer is full
	ERROR_BUFFER_NOT_ENOUGH_ELEMENTS			= 22, // Not enough space in buffer to perform operation
	ERROR_BUFFER_NOT_ENOUGH_SPACE				= 23, // Not enough space in buffer to perform operation
	ERROR_BUFFER_POINTER_NULL					= 24, // Buffer size was set to zero
	ERROR_BUFFER_SIZE_TOO_LARGE					= 25, // Buffer size was set to a large value
	ERROR_BUFFER_SIZE_TOO_SMALL					= 26, // Buffer size was set to a very small value
	ERROR_BUFFER_SIZE_ZERO						= 27, // Buffer size was set to zero

	// Error in handlers and functions pointers
	ERROR_FUNCTION_POINTER_NULL					= 30,
	ERROR_HANDLER_POINTER_NULL					= 31,

	// Error related to date and time
	ERROR_TIME_INVALID							= 40,
	ERROR_DATE_INVALID							= 41,

	// Error related to clock configuration
	ERROR_CLOCK_PRESCALER_CHANGE_FAILED			= 50, // Clock prescaler change failed
	ERROR_CLOCK_PRESCALER_UNSUPPORTED			= 51, // Clock prescaler configuration unsupported
	ERROR_CLOCK_SPEED_TOO_HIGH					= 52,
	ERROR_CLOCK_SPEED_TOO_LOW					= 53,

	// Error related to communication to devices
	ERROR_COMMUNICATION_FAILED					= 60,
	ERROR_COMMUNICATION_PORT_NOT_SET			= 61,
	ERROR_COMMUNICATION_TIME_OUT				= 62,

	// Generic error related to external devices
	ERROR_DEVICE_ID_MATCH_FAILED				= 70,
	ERROR_NO_DEVICE_SELECTED					= 71,

	// Error related to IMU sensors
	ERROR_INVALID_AXIS							= 80,

	// Error related to communication protocol
	ERROR_CHECKSUM_ERROR						= 90,
	ERROR_FRAME_ERROR							= 91,
	ERROR_PACKAGE_AWAITING						= 92, // Try to write data to a ready package
	ERROR_PACKAGE_NOT_READY						= 93, // Try to read data from a not ready package

	// Error related to external interrupts
	ERROR_INTERRUPT_NOT_AVAILABLE				= 100,
	ERROR_PORT_OUTPUT_MODE_UNSUPPORTED			= 101,
	ERROR_SENSE_MODE_UNSUPPORTED				= 102,

	ERROR_USART_CONFIGURATION_PARAMETER_WRONG,
	ERROR_USART_UNSUPPORTED_CONFIGURATION_BAUD_RATE,
	ERROR_USART_UNSUPPORTED_CONFIGURATION_DATA_BITS,
	ERROR_USART_UNSUPPORTED_CONFIGURATION_FRAME_FORMAT,
	ERROR_USART_UNSUPPORTED_CONFIGURATION_PARITY,
	ERROR_USART_UNSUPPORTED_CONFIGURATION_STOP_BIT,

	ERROR_PACKAGE_API_INVALID_SOURCE_ADDRESS,

	FR_OK = 0,
	FR_DISK_ERR = 121,			// (121) A hard error occurred in the low level disk I/O layer
	FR_INT_ERR,				// (122) Assertion failed
	FR_NOT_READY,			// (123) The physical drive cannot work
	FR_NO_FILE,				// (124) Could not find the file
	FR_NO_PATH,				// (125) Could not find the path
	FR_INVALID_NAME,		// (126) The path name format is invalid
	FR_DENIED,				// (127) Access denied due to prohibited access or directory full
	FR_EXIST,				// (128) Access denied due to prohibited access
	FR_INVALID_OBJECT,		// (129) The file/directory object is invalid
	FR_WRITE_PROTECTED,		// (130) The physical drive is write protected
	FR_INVALID_DRIVE,		// (131) The logical drive number is invalid
	FR_NOT_ENABLED,			// (132) The volume has no work area
	FR_NO_FILESYSTEM,		// (133) There is no valid FAT volume
	FR_MKFS_ABORTED,		// (134) The f_mkfs() aborted due to any problem
	FR_TIMEOUT,				// (135) Could not get a grant to access the volume within defined period
	FR_LOCKED,				// (136) The operation is rejected according to the file sharing policy
	FR_NOT_ENOUGH_CORE,		// (137) LFN working buffer could not be allocated
	FR_TOO_MANY_OPEN_FILES,	// (138) Number of open files > FF_FS_LOCK
	FR_INVALID_PARAMETER	// (139) Given parameter is invalid



} error_e;

// New Data Type - Enumerations => parametric table that doesn't maintain value equivalence with datasheet

typedef enum timerOutput_e {
	TIMER_PORT_NO_CHANGE			= 0,	// Keep last port configuration
	TIMER_PORT_NORMAL				= 1,	// Output Compare Pin disconnected; normal GPIO operation
	TIMER_PORT_TOGGLE_ON_COMPARE	= 2,	// Toggle pin value on compare
	TIMER_PORT_CLEAR_ON_COMPARE		= 3,	// Clear pin on compare
	TIMER_PORT_SET_ON_COMPARE		= 4,	// Set pin on compare
	TIMER_PORT_NON_INVERTING_MODE	= 5,	// PWM non-inverting mode
	TIMER_PORT_INVERTING_MODE		= 6		// PWM inverting mode
} timerOutput_e;

#endif // __GPDSE_ARM_GLOBAL_DEFINES_H
