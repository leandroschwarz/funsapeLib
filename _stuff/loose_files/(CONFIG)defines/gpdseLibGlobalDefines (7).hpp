/* =============================================================================
 * Project:		GPSDE++ Library
 * File name:	gpdseLibGlobalDefines.hpp
 * Module:		Global definitions for GPDSE++ AVR Library project
 * Authors:		Leandro Schwarz
 * 				Mayara de Sousa
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// File build
// =============================================================================

#ifndef __GPDSE_LIB_GLOBAL_DEFINES_HPP
#define __GPDSE_LIB_GLOBAL_DEFINES_HPP			__BUILD_MACRO_TO_BE_CHANGED__

// =============================================================================
// System file dependencies
// =============================================================================

#include "gpdseLibProjectConfig.hpp"
#if __GPDSE_LIB_PROJECT_CONFIG_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error	Project configurations file (gpdseLibProjectConfig) must be build __BUILD_MACRO_TO_BE_CHANGED__.
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

#define inlined							inline __attribute__((always_inline))
#define ignore							__attribute__((unused))

// =============================================================================
// Macro-functions
// =============================================================================

// Bit handling
#define clrBit(reg, bit)				((reg) &= (~(1 << (bit))))
#define cplBit(reg, bit)				((reg) ^= (1 << (bit)))
#define setBit(reg, bit)				((reg) |= (1 << (bit)))
#define isBitClr(reg, bit)				(bool)(!(((reg) >> (bit)) & 1))
#define isBitSet(reg, bit)				(bool)(((reg) >> (bit)) & 0x01)
// Bit loop
#define waitUntilBitIsClear(reg, bit)	do {} while(isBitSet((reg), (bit)))
#define waitUntilBitIsSet(reg, bit)		do {} while(isBitClr((reg), (bit)))
// Bit mask handling
#define clrMask(reg, mask, offset)		((reg) &= ~((mask) << (offset)))
#define cplMask(reg, mask, offset)		((reg) ^= ((mask) << (offset)))
#define setMask(reg, mask, offset)		((reg) |= ((mask) << (offset)))
// Bit data manipulation
#define byteHighNibble(dataByte)		((dataByte >> 4) & 0x0F)
#define byteLowNibble(dataByte)			(dataByte & 0x0F)
#define byteSwitchNibbles(dataByte)		((byteLowNibble(dataByte) << 4) | byteHighNibble(dataByte))
#define wordHighByte(dataWord)			((dataWord >> 8) & 0x00FF)
#define wordLowByte(dataWord)			(dataWord & 0x00FF)
#define wordSwitchBytes(dataWord)		((wordLowByte(dataWord) << 8) | wordHighByte(dataWord))
#define dwordByte3(dataDword)			((dataDword >> 24) & 0x000000FF)
#define dwordByte2(dataDword)			((dataDword >> 16) & 0x000000FF)
#define dwordByte1(dataDword)			((dataDword >> 8) & 0x000000FF)
#define dwordByte0(dataDword)			(dataDword & 0x000000FF)
#define dwordSwitchBytes(dataWord)		((dwordByte0(dataWord) << 24) | (dwordByte1(dataWord) << 16) | (dwordByte2(dataWord) << 8) | (dwordByte3(dataWord))
#define dwordHighWord(dataDword)		((dataDword >> 16) & 0x0000FFFF)
#define dwordLowWord(dataDword)			(dataDword & 0x0000FFFF)
#define dwordSwitchWords(dataDword)		((dwordLowWord(dataDword) << 8) | dwordHighWord(dataDword))
// ASCII character macros
#define isAsciiCommand(charac)			((charac < ' ') ? true : false)
#define isAsciiLetter(charac)			((isAsciiLowercaseLetter(charac) || (isAsciiUppercaseLetter(charac)) ? true : false)
#define isAsciiLowercaseLetter(charac)	(((charac >= 'a') && (charac <= 'z')) ? true : false)
#define isAsciiUppercaseLetter(charac)	(((charac >= 'A') && (charac <= 'Z')) ? true : false)
#define isAsciiDigit(charac)			(((charac >= '0') && (charac <= '9')) ? true : false)
#define isAsciiSymbol(charac)			(((!isAsciiDigit(charac)) && (!isAsciiCommand(charac)) && (!isAsciiLetter(charac)))? true : false)
#define convertToUppercase(charac)		((isAsciiLowercaseLetter(charac)) ? (charac - 0x20) : charac)
#define convertToLowercase(charac)		((isAsciiUppercaseLetter(charac)) ? (charac + 0x20) : charac)
// Value comparison
#define maximumOf(var1 , var2)			((var1 > var2) ? var1 :var2)
#define truncateBetween(var, min, max)	((var < min) ? min : ((var > max) ? max : var))
#define minimumOf(var1 , var2)			((var1 < var2) ? var1 :var2)
// Stringification of macro arguments
#define stringifyName(constant)			#constant
#define stringifyValue(constant)		stringifyName(constant)
// System actions
#define die(errorCode)					do {printf("Failed with error=%u.\r", errorCode);systemHalt();} while(0)
#define systemHalt()					while(1) {}
#define doNothing()						{}
#define doNop()							do {asm volatile("nop");} while(0)
// I/O manipulation
#define setPin(gpioPort, gpioPin)		HAL_GPIO_WritePin(gpioPort, gpioPin, GPIO_PIN_SET)
#define clrPin(gpioPort, gpioPin)		HAL_GPIO_WritePin(gpioPort, gpioPin, GPIO_PIN_RESET)
#define cplPin(gpioPort, gpioPin)		HAL_GPIO_TogglePin(gpioPort, gpioPin)
#define delayMs(time)					HAL_Delay(time)

// =============================================================================
// New data types
// =============================================================================

// Single variables => shortening the name of single data types
typedef volatile bool					vbool;
typedef volatile uint8_t				reg_t;
typedef volatile int8_t					vint8_t;
typedef volatile int16_t				vint16_t;
typedef volatile int32_t				vint32_t;
typedef volatile int64_t				vint64_t;
typedef volatile uint8_t				vuint8_t;
typedef volatile uint16_t				vuint16_t;
typedef volatile uint32_t				vuint32_t;
typedef volatile uint64_t				vuint64_t;
#if defined(_GPDSE_PLATFORM_AVR8)
typedef int16_t							int_t;		// 16-bit (AVR)
typedef uint16_t						uint_t;		// 16-bit (AVR)
#elif defined(_GPDSE_PLATFORM_ARM32)
typedef int32_t							int_t;		// 32-bit (ARM)
typedef uint32_t						uint_t;		// 32-bit (ARM)
#endif

// Single variables => enumerations understood as logic values
typedef enum activation_t : bool  {
	ACTIVATION_OFF						= 0,
	ACTIVATION_ON						= 1
} activation_t;

typedef enum stateOffOn_t : bool  {
	STATE_OFF							= 0,
	STATE_ON							= 1
} stateOffOn_t;

typedef enum edge_t : bool  {
	EDGE_FALLING						= 0,
	EDGE_RISING							= 1
} edge_t;

typedef enum logic_t : bool {
	LOGIC_LOW							= 0,
	LOGIC_HIGH							= 1
} logic_t;

typedef enum rotation_t : bool {
	ROTATION_CLOCKWISE					= 0,
	ROTATION_COUNTERCLOCKWISE			= 1
} rotation_t;

// Single variables => small enumerations that can be understood as a single variable
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

typedef enum blockingMode_t : bool {
	BLOCKING_MODE_OFF					= 0,
	BLOCKING_MODE_ON					= 1
} blockingMode_t;

typedef enum direction_t : uint8_t {
	DIRECTION_LEFT						= 0,
	DIRECTION_RIGHT						= 1,
	DIRECTION_UP						= 2,
	DIRECTION_DOWN						= 3
} direction_t;

typedef enum driverMode_t : bool {
	DRIVER_MODE_PUSH_PULL				= 0,
	DRIVER_MODE_OPEN_DRAIN				= 1,
} driverMode_t;

// Enumerations => large enumerations or enumerations with expansible number of elements
typedef enum error_e : uint8_t {
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
	ERROR_INTERFACE_MODE_UNSUPPORTED			= 9,
	ERROR_OUTPUT_COMPARE_UNSUPPORTED			= 10,
	ERROR_FUNCTION_POINTER_NULL					= 11,
	ERROR_HANDLER_POINTER_NULL					= 12,
	ERROR_REGISTER_ADDRESS_INVALID				= 13,

	// Error in arguments
	ERROR_ARGUMENT_CANNOT_BE_ZERO				= 20,
	ERROR_ARGUMENT_POINTER_NULL					= 21, // NULL pointer was passed as an argument to function
	ERROR_ARGUMENTS_MISMATCH					= 22, // A function was called with dubious argument values
	ERROR_ARGUMENTS_WRONG_NUMBER				= 23, // A function was called with wrong number of arguments
	ERROR_ARGUMENTS_PINOUT_UNSUPPORTED			= 24, // Pinout configuration is unsupported

	// Error in buffers
	ERROR_BUFFER_EMPTY							= 30, // Buffer is empty
	ERROR_BUFFER_FULL							= 31, // Buffer is full
	ERROR_BUFFER_NOT_ENOUGH_ELEMENTS			= 32, // Not enough space in buffer to perform operation
	ERROR_BUFFER_NOT_ENOUGH_SPACE				= 33, // Not enough space in buffer to perform operation
	ERROR_BUFFER_POINTER_NULL					= 34, // Buffer size was set to zero
	ERROR_BUFFER_SIZE_TOO_LARGE					= 35, // Buffer size was set to a large value
	ERROR_BUFFER_SIZE_TOO_SMALL					= 36, // Buffer size was set to a very small value
	ERROR_BUFFER_SIZE_ZERO						= 37, // Buffer size was set to zero

	// Error related to date and time
	ERROR_TIME_INVALID							= 40,
	ERROR_DATE_INVALID							= 41,

	// Error related to clock configuration
	ERROR_CLOCK_PRESCALER_CHANGE_FAILED			= 50, // Clock prescaler change failed
	ERROR_CLOCK_PRESCALER_UNSUPPORTED			= 51, // Clock prescaler configuration unsupported
	ERROR_CLOCK_SPEED_TOO_HIGH					= 52,
	ERROR_CLOCK_SPEED_TOO_LOW					= 53,
	ERROR_CLOCK_MAIN_SOURCE_VALUE_ZERO			= 54,

	// Error related to communication to devices
	ERROR_COMMUNICATION_FAILED					= 60,
	ERROR_COMMUNICATION_PORT_NOT_SET			= 61,
	ERROR_COMMUNICATION_TIME_OUT				= 62,

	// Generic error related to external devices
	ERROR_DEVICE_ID_MATCH_FAILED				= 70,
	ERROR_NO_DEVICE_SELECTED					= 71,

	// Error related general sensors
	ERROR_AXIS_INVALID							= 80,
	ERROR_COORDINATE_INVALID					= 81,
	ERROR_DIRECTION_INVALID						= 82,

	// Error related to communication protocol
	ERROR_CHECKSUM_ERROR						= 90,
	ERROR_FRAME_ERROR							= 91,
	ERROR_PACKAGE_AWAITING						= 92, // Try to write data to a ready package
	ERROR_PACKAGE_NOT_READY						= 93, // Try to read data from a not ready package

	// Error related to external interrupts
	ERROR_INTERRUPT_NOT_AVAILABLE				= 100,
	ERROR_PORT_OUTPUT_MODE_UNSUPPORTED			= 101,
	ERROR_SENSE_MODE_UNSUPPORTED				= 102,

	// Error related to USART communication
	ERROR_UNSUPPORTED_BAUD_RATE					= 110,
	ERROR_UNSUPPORTED_DATA_LENGTH				= 111,
	ERROR_UNSUPPORTED_FRAME_FORMAT				= 112,
	ERROR_UNSUPPORTED_OPERATION_MODE			= 113,
	ERROR_UNSUPPORTED_PARITY_MODE				= 114,
	ERROR_UNSUPPORTED_STOP_BITS					= 115,
	ERROR_USART_BAUD_RATE_TOO_HIGH				= 116,
	ERROR_USART_BAUD_RATE_TOO_LOW				= 117,

	// Error related to LCD (and similar) devices
	ERROR_LCD_CONTROL_EN_PIN_INVALID			= 120,
	ERROR_LCD_CONTROL_PIN_CONTENTION			= 121,
	ERROR_LCD_CONTROL_PORT_INVALID				= 122,
	ERROR_LCD_CONTROL_PORT_UNDEFINED			= 123,
	ERROR_LCD_CONTROL_RS_PIN_INVALID			= 124,
	ERROR_LCD_CONTROL_RW_PIN_INVALID			= 125,
	ERROR_LCD_DATA_FIRST_PIN_INVALID			= 126,
	ERROR_LCD_DATA_PORT_INVALID					= 127,
	ERROR_LCD_DATA_PORT_UNDEFINED				= 128,

	ERROR_LCD_DISPLAY_SIZE_UNSUPPORTED			= 134,
	ERROR_LCD_FONT_SIZE_UNSUPPORTED				= 135,

	FR_OK = 0,				// (0) Succeeded
	FR_DISK_ERR = 201,			// (1) A hard error occurred in the low level disk I/O layer
	FR_INT_ERR,				// (2) Assertion failed
	FR_NOT_READY,			// (3) The physical drive cannot work
	FR_NO_FILE,				// (4) Could not find the file
	FR_NO_PATH,				// (5) Could not find the path
	FR_INVALID_NAME,		// (6) The path name format is invalid
	FR_DENIED,				// (7) Access denied due to prohibited access or directory full
	FR_EXIST,				// (8) Access denied due to prohibited access
	FR_INVALID_OBJECT,		// (9) The file/directory object is invalid
	FR_WRITE_PROTECTED,		// (10) The physical drive is write protected
	FR_INVALID_DRIVE,		// (11) The logical drive number is invalid
	FR_NOT_ENABLED,			// (12) The volume has no work area
	FR_NO_FILESYSTEM,		// (13) There is no valid FAT volume
	FR_MKFS_ABORTED,		// (14) The f_mkfs() aborted due to any problem
	FR_TIMEOUT,				// (15) Could not get a grant to access the volume within defined period
	FR_LOCKED,				// (16) The operation is rejected according to the file sharing policy
	FR_NOT_ENOUGH_CORE,		// (17) LFN working buffer could not be allocated
	FR_TOO_MANY_OPEN_FILES,	// (18) Number of open files > FF_FS_LOCK
	FR_INVALID_PARAMETER,	// (19) Given parameter is invalid




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

// New Data Type - Enumerations => parametric table that doesn't mantain value equivalence with datasheet
//typedef enum sleepMode_e : uint8_t {
//	SLEEP_MODE_NO_CHANGE	= 0,	// Keep last sleep mode
//	SLEEP_MODE_IDLE			= 1,	// Sleep mode IDLE
//	SLEEP_MODE_ADC			= 2,	// Sleep mode for ADC noise reduction
//	SLEEP_MODE_PWR_DOWN		= 3,	// Sleep mode POWER DOWN
//	SLEEP_MODE_PWR_SAVE		= 4,	// Sleep mode POWER SAVR
//	SLEEP_MODE_STANDBY		= 5,	// Sleep mode STANDBY
//	SLEEP_MODE_EXT_STANDBY	= 6		// Sleep mode EXTENDED STANDBY
//} sleepMode_e;

typedef enum timerOutput_e : uint8_t {
	TIMER_PORT_NO_CHANGE			= 0,	// Keep last port configuration
	TIMER_PORT_NORMAL				= 1,	// Output Compare Pin disconnected; normal GPIO operation
	TIMER_PORT_TOGGLE_ON_COMPARE	= 2,	// Toggle pin value on compare
	TIMER_PORT_CLEAR_ON_COMPARE		= 3,	// Clear pin on compare
	TIMER_PORT_SET_ON_COMPARE		= 4,	// Set pin on compare
	TIMER_PORT_NON_INVERTING_MODE	= 5,	// PWM non-inverting mode
	TIMER_PORT_INVERTING_MODE		= 6		// PWM inverting mode
} timerOutput_e;

#endif // __GPDSE_LIB_GLOBAL_DEFINES_HPP
