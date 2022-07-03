/* =============================================================================
 * Project:		GPSDE++ Library
 * File name:	gpdseLibProjectConfig.hpp
 * Module:		System definitions for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 * 				Mayara de Sousa
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// File build
// =============================================================================

#ifndef __GPDSE_LIB_PROJECT_CONFIG_HPP
#define __GPDSE_LIB_PROJECT_CONFIG_HPP			__BUILD_MACRO_TO_BE_CHANGED__

// =============================================================================
// User may edit this section
// BEGIN - USER EDITABLE AREA
// =============================================================================

// -----------------------------------------------------------------------------
// System clock and delay ------------------------------------------------------

#define _GPDSE_CPU_CLOCK						16000000UL
#define _GPDSE_DEBOUNCE_TIME					30

// -----------------------------------------------------------------------------
// Microcontroller selection ---------------------------------------------------

// #define _GPDSE_DEVICE_ATMEGA_48x
// #define _GPDSE_DEVICE_ATMEGA_88x
// #define _GPDSE_DEVICE_ATMEGA_168x
// #define _GPDSE_DEVICE_ATMEGA_328x
// #define _GPDSE_DEVICE_ATMEGA_16U4
// #define _GPDSE_DEVICE_ATMEGA_32U4
// #define _GPDSE_DEVICE_ATMEGA_640
// #define _GPDSE_DEVICE_ATMEGA_1280
// #define _GPDSE_DEVICE_ATMEGA_1281
// #define _GPDSE_DEVICE_ATMEGA_2560
// #define _GPDSE_DEVICE_ATMEGA_2561
// #define _GPDSE_DEVICE_ATTINY_13
// #define _GPDSE_DEVICE_ATTINY_13A
// #define _GPDSE_DEVICE_ATTINY_25
// #define _GPDSE_DEVICE_ATTINY_45
#define _GPDSE_DEVICE_ATTINY_85
// #define _GPDSE_DEVICE_ATTINY_2313
// #define _GPDSE_DEVICE_ATTINY_2313A
// #define _GPDSE_DEVICE_ATTINY_4313

// #define _GPDSE_DEVICE_STM32F030R8
// #define _GPDSE_DEVICE_STM32L053R8

// =============================================================================
// END - USER EDITABLE AREA
// From this point on, don't change anything
// *INDENT-OFF*
// =============================================================================

// Microcontroller architetures index offset
#define _GPDSE_AVR_OFFSET				0
#define _GPDSE_ARM_OFFSET				100
#define _GPDSE_MAX_OFFSET				200

// AVR microcontroller devices
#define _GPDSE_ATMEGA_48				(_GPDSE_AVR_OFFSET + 1)
#define _GPDSE_ATMEGA_88				(_GPDSE_AVR_OFFSET + 2)
#define _GPDSE_ATMEGA_168				(_GPDSE_AVR_OFFSET + 3)
#define _GPDSE_ATMEGA_328				(_GPDSE_AVR_OFFSET + 4)
#define _GPDSE_ATMEGA_16U4				(_GPDSE_AVR_OFFSET + 5)
#define _GPDSE_ATMEGA_32U4				(_GPDSE_AVR_OFFSET + 6)
#define _GPDSE_ATMEGA_640				(_GPDSE_AVR_OFFSET + 7)
#define _GPDSE_ATMEGA_1280				(_GPDSE_AVR_OFFSET + 8)
#define _GPDSE_ATMEGA_1281				(_GPDSE_AVR_OFFSET + 9)
#define _GPDSE_ATMEGA_2560				(_GPDSE_AVR_OFFSET + 10)
#define _GPDSE_ATMEGA_2561				(_GPDSE_AVR_OFFSET + 11)
#define _GPDSE_ATTINY_13				(_GPDSE_AVR_OFFSET + 12)
#define _GPDSE_ATTINY_13A				(_GPDSE_AVR_OFFSET + 13)
#define _GPDSE_ATTINY_25				(_GPDSE_AVR_OFFSET + 14)
#define _GPDSE_ATTINY_45				(_GPDSE_AVR_OFFSET + 15)
#define _GPDSE_ATTINY_85				(_GPDSE_AVR_OFFSET + 16)
#define _GPDSE_ATTINY_2313				(_GPDSE_AVR_OFFSET + 17)
#define _GPDSE_ATTINY_2313A				(_GPDSE_AVR_OFFSET + 18)
#define _GPDSE_ATTINY_4313				(_GPDSE_AVR_OFFSET + 19)

// ARM microcontroller devices
#define _GPDSE_STM32F030R8				(_GPDSE_ARM_OFFSET + 1)
#define _GPDSE_STM32L053R8				(_GPDSE_ARM_OFFSET + 2)

// -----------------------------------------------------------------------------
// Undefine error flag (just in case) ------------------------------------------

#if defined(_GPDSE_ERROR_FLAG)
#	undef _GPDSE_ERROR_FLAG
#endif

// -----------------------------------------------------------------------------
// Undefine microcontroller flag (just in case) --------------------------------

#if defined(_GPDSE_MCU)
#	undef _GPDSE_MCU
#endif

// =============================================================================
// Microcontroller selection
// =============================================================================

// -----------------------------------------------------------------------------
// AVR architecture - Microchip ------------------------------------------------

// ATmega48
#if defined(_GPDSE_DEVICE_ATMEGA_48x)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_48
#	endif
#endif

// ATmega88
#if defined(_GPDSE_DEVICE_ATMEGA_88x)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_88
#	endif
#endif

// ATmega168
#if defined(_GPDSE_DEVICE_ATMEGA_168x)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_168
#	endif
#endif

// ATmega328
#if defined(_GPDSE_DEVICE_ATMEGA_328x)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_328
#	endif
#endif

// ATmega16U4
#if defined(_GPDSE_DEVICE_ATMEGA_16U4)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_16U4
#	endif
#endif

// ATmega32U4
#if defined(_GPDSE_DEVICE_ATMEGA_32U4)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_32U4
#	endif
#endif

// ATmega640
#if defined(_GPDSE_DEVICE_ATMEGA_640)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_640
#	endif
#endif

// ATmega1280
#if defined(_GPDSE_DEVICE_ATMEGA_1280)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_1280
#	endif
#endif

// ATmega1281
#if defined(_GPDSE_DEVICE_ATMEGA_1281)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_1281
#	endif
#endif

// ATmega2560
#if defined(_GPDSE_DEVICE_ATMEGA_2560)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_2560
#	endif
#endif

// ATmega2561
#if defined(_GPDSE_DEVICE_ATMEGA_2561)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_2561
#	endif
#endif

// ATtiny13
#if defined(_GPDSE_DEVICE_ATTINY_13)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATTINY_13
#	endif
#endif

// ATtiny13A
#if defined(_GPDSE_DEVICE_ATTINY_13A)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATTINY_13A
#	endif
#endif

// ATtiny285
#if defined(_GPDSE_DEVICE_ATTINY_285)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATTINY_25
#	endif
#endif

// ATtiny45
#if defined(_GPDSE_DEVICE_ATTINY_45)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATTINY_45
#	endif
#endif

// ATtiny85
#if defined(_GPDSE_DEVICE_ATTINY_85)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATTINY_85
#	endif
#endif

// ATtiny2313
#if defined(_GPDSE_DEVICE_ATTINY_2313)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATTINY_2313
#	endif
#endif

// ATtiny2313A
#if defined(_GPDSE_DEVICE_ATTINY_2313A)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATTINY_2313A
#	endif
#endif

// ATtiny4313
#if defined(_GPDSE_DEVICE_ATTINY_4313)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATTINY_4313
#	endif
#endif

// -----------------------------------------------------------------------------
// ARM architecture - ST Microelectronics --------------------------------------

// STM32F030R8
#if defined(_GPDSE_DEVICE_STM32F030R8)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_STM32F030R8
#	endif
#endif

// STM32L053R8
#if defined(_GPDSE_DEVICE_STM32L053R8)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_STM32L053R8
#	endif
#endif

// -----------------------------------------------------------------------------
// Verify microcontroller selection --------------------------------------------

#if !defined(_GPDSE_MCU)
#	warning	Verify your selection in "Microcontroller Selection" section!
#	error	No device was selected.
#endif

// -----------------------------------------------------------------------------
// Platform selection ----------------------------------------------------------

#if (_GPDSE_MCU < _GPDSE_ARM_OFFSET)
#	define _GPDSE_PLATFORM_AVR8
#elif (_GPDSE_MCU < _GPDSE_MAX_OFFSET)
#	define _GPDSE_PLATFORM_ARM32
#else
#	warning	Verify your selection in "Microcontroller Selection" section!
#	error	PLatform not supported!
#endif

// -----------------------------------------------------------------------------
// System clock configuration --------------------------------------------------

#if _GPDSE_CPU_CLOCK > 4294967295UL
#		warning	Verify your definition in "System clock and delay" section!
#		error	System clock too high!
#endif

#if defined(_GPDSE_PLATFORM_AVR8)
#	define F_CPU						_GPDSE_CPU_CLOCK
#endif

// -----------------------------------------------------------------------------
// Include basic comom header files --------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Include basic platform-dependent header files -------------------------------

#if defined(_GPDSE_PLATFORM_AVR8)				// AVR headers
#	include <avr/io.h>
#	include <avr/interrupt.h>
#	include <util/delay.h>
#elif defined(_GPDSE_PLATFORM_ARM32)			// ARM headers
	//Bibliotecas HAL
	//Bibliotecas HAL
	//Bibliotecas HAL
	//Bibliotecas HAL
#endif

// -----------------------------------------------------------------------------
// Include AVR device part specific header files -------------------------------

#if (_GPDSE_MCU == _GPDSE_ATMEGA_48)
#	include "part/gpdseLibAtmega48.hpp"
#elif (_GPDSE_MCU == _GPDSE_ATMEGA_88)
#	include "part/gpdseLibAtmega88.hpp"
#elif (_GPDSE_MCU == _GPDSE_ATMEGA_168)
#	include "part/gpdseLibAtmega168.hpp"
#elif (_GPDSE_MCU == _GPDSE_ATMEGA_328)
#	include "part/gpdseLibAtmega328.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATMEGA_16U4)
#	include "part/gpdseLibAtmega16u4.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATMEGA_32U4)
#	include "part/gpdseLibAtmega32u4.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATMEGA_640)
#	include "part/gpdseLibAtmega640.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATMEGA_1280)
#	include "part/gpdseLibAtmega1280.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATMEGA_1281)
#	include "part/gpdseLibAtmega1281.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATMEGA_2560)
#	include "part/gpdseLibAtmega2560.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATMEGA_2561)
#	include "part/gpdseLibAtmega2561.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATTINY_13)
#	include "part/gpdseLibAttiny13.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATTINY_13A)
#	include "part/gpdseLibAttiny13A.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATTINY_25)
#	include "part/gpdseLibAttiny25.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATTINY_45)
#	include "part/gpdseLibAttiny45.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATTINY_85)
#	include "part/gpdseLibAttiny85.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATTINY_2313)
#	include "part/gpdseLibAttiny2313.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATTINY_2313A)
#	include "part/gpdseLibAttiny2313A.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATTINY_4313A)
#	include "part/gpdseLibAttiny4313A.hpp"
#endif

// *INDENT-ON*

#endif // __GPDSE_LIB_PROJECT_CONFIG_HPP
