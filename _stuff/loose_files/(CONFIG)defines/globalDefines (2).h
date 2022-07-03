/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Integrated Library
 * File:			globalDefines.h
 * Module:			Global definitions file for the GPDSE Project
 * Author:			Leandro Schwarz
 * Version:			13.0
 * Last edition:	2017-06-16
 * ---------------------------------------------------------------------------*/

#ifndef __GLOBALDEFINES_H
	#define __GLOBALDEFINES_H 130

// -----------------------------------------------------------------------------
// Define Clock Frequency ------------------------------------------------------

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

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

typedef signed char			int8;
typedef signed int			int16;
typedef signed long int		int32;
typedef signed long long	int64;
typedef unsigned char		uint8;
typedef unsigned int		uint16;
typedef unsigned long int	uint32;
typedef unsigned long long	uint64;

typedef enum bool_t{
	FALSE = 0,
	TRUE = 1
} bool_t;

typedef enum logic_t{
	LOW = 0,
	HIGH = 1,
	OFF = 0,
	ON = 1
} logic_t;

typedef enum direction_t{
	LEFT = 0,
	RIGHT = 1,
	UP = 0,
	DOWN = 1,
	CLOCKWISE = 0,
	COUNTERCLOCKWISE = 1
} direction_t;

typedef enum resultValue_t{
	RESULT_OK = 0,
	RESULT_UNSUPPORTED_SYSTEM_PRESCALER_VALUE,
	RESULT_UNSUPPORTED_ADC_REFERENCE,
	RESULT_UNSUPPORTED_ADC_PRESCALER_VALUE,
	RESULT_UNSUPPORTED_ADC_MODE,
	RESULT_UNSUPPORTED_ADC_CHANNEL,
	RESULT_UNSUPPORTED_ADC_ADJUST_RESULT,
	RESULT_UNSUPPORTED_USART_STOP_BITS,
	RESULT_UNSUPPORTED_USART_PARITY,
	RESULT_UNSUPPORTED_USART_DATA_BITS,
	RESULT_UNSUPPORTED_USART_MODE,
	RESULT_UNSUPPORTED_USART_BAUD_RATE,			// Not implemented yet
	RESULT_UNSUPPORTED_TIMER_PORT_CONFIG,
	RESULT_UNSUPPORTED_TIMER0_PRESCALER_VALUE,
	RESULT_UNSUPPORTED_TIMER0_MODE,
	RESULT_UNSUPPORTED_TIMER1_PRESCALER_VALUE,
	RESULT_UNSUPPORTED_TIMER1_MODE,
	RESULT_UNSUPPORTED_TIMER1_INPUT_CAPTURE_MODE,
	RESULT_UNSUPPORTED_TIMER2_PRESCALER_VALUE,
	RESULT_UNSUPPORTED_TIMER2_MODE,


	///////////////////////////////// MUST BE REMOVED
	RESULT_PORT_VALUE_UNSUPPORTED,
	RESULT_INT_SENSE_VALUE_UNSUPPORTED,
	RESULT_TIMER_PRESCALER_VALUE_UNSUPPORTED,
	RESULT_TIMER_MODE_VALUE_UNSUPPORTED,
	RESULT_USART_BAUD_RATE_UNSUPORTED,
	RESULT_UNSUPPORTED_VALUE
} resultValue_t;

typedef enum systemPrescalerValue_t{
	SYSTEM_PRESCALER_OFF = 0,
	SYSTEM_PRESCALER_2 = 1,
	SYSTEM_PRESCALER_4 = 2,
	SYSTEM_PRESCALER_8 = 3,
	SYSTEM_PRESCALER_16 = 4,
	SYSTEM_PRESCALER_32 = 5,
	SYSTEM_PRESCALER_64 = 6,
	SYSTEM_PRESCALER_128 = 7,
	SYSTEM_PRESCALER_256 = 8
} systemPrescalerValue_t;

typedef enum timerOutput_t{
	TIMER_PORT_NORMAL = 0,
	TIMER_PORT_TOGGLE_ON_COMPARE = 1,
	TIMER_PORT_CLEAR_ON_COMPARE = 2,
	TIMER_PORT_SET_ON_COMPARE = 3,
	TIMER_PORT_NON_INVERTING_MODE = 2,
	TIMER_PORT_INVERTING_MODE = 3,
	TIMER_PORT_NO_CHANGE = 255
} timerOutput_t;

//////////////////////////// OLD FILE, MUST BE REMOVED BEFORE RELEASE
//////////////////////////// OLD FILE, MUST BE REMOVED BEFORE RELEASE
//////////////////////////// OLD FILE, MUST BE REMOVED BEFORE RELEASE
//////////////////////////// OLD FILE, MUST BE REMOVED BEFORE RELEASE
//////////////////////////// OLD FILE, MUST BE REMOVED BEFORE RELEASE

#ifndef noOperation
	#define noOperation(cycles)					__builtin_avr_delay_cycles(cycles)
#endif

typedef enum portMode_t{
	PORT_OUTPUT_LOW = 0,
	PORT_OUTPUT_HIGH,
	PORT_INPUT_TRISTATE,
	PORT_INPUT_PULL_UP,
	PORT_NO_CHANGE = 255
} portMode_t;

typedef enum senseMode_t{
	SENSE_LOW_LEVEL = 0,
	SENSE_ANY_EDGE,
	SENSE_FALLING_EDGE,
	SENSE_RISING_EDGE,
	SENSE_NO_CHANGE = 255
} senseMode_t;

#endif
