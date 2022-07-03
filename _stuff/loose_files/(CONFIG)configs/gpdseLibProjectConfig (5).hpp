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
#define _GPDSE_DEVICE_ATMEGA_328x
// #define _GPDSE_DEVICE_ATMEGA_16U4
// #define _GPDSE_DEVICE_ATMEGA_32U4
// #define _GPDSE_DEVICE_ATMEGA_640
// #define _GPDSE_DEVICE_ATMEGA_1280
// #define _GPDSE_DEVICE_ATMEGA_2560
// #define _GPDSE_DEVICE_ATMEGA_1281
// #define _GPDSE_DEVICE_ATMEGA_2561
// #define _GPDSE_DEVICE_ATTINY_13
// #define _GPDSE_DEVICE_ATTINY_13A
// #define _GPDSE_DEVICE_ATTINY_25
// #define _GPDSE_DEVICE_ATTINY_45
// #define _GPDSE_DEVICE_ATTINY_85
// #define _GPDSE_DEVICE_ATTINY_2313
// #define _GPDSE_DEVICE_ATTINY_4313
// #define _GPDSE_DEVICE_ATTINY_2313A

// #define _GPDSE_DEVICE_STM32F030R8
// #define _GPDSE_DEVICE_STM32L053R8

// -----------------------------------------------------------------------------
// Debug configuration ---------------------------------------------------------

// =============================================================================
// END - USER EDITABLE AREA
// From this point on, don't change anything
// *INDENT-OFF*
// =============================================================================

// AVR microcontroller devices
#define _GPDSE_ATMEGA_48x				1
#define _GPDSE_ATMEGA_88x				2
#define _GPDSE_ATMEGA_168x				3
#define _GPDSE_ATMEGA_328x				4
#define _GPDSE_ATMEGA_16U4				5
#define _GPDSE_ATMEGA_32U4				6
#define _GPDSE_ATMEGA_640				7
#define _GPDSE_ATMEGA_1280				8
#define _GPDSE_ATMEGA_2560				9
#define _GPDSE_ATMEGA_1281				10
#define _GPDSE_ATMEGA_2561				11
#define _GPDSE_ATTINY_13				12
#define _GPDSE_ATTINY_13A				13
#define _GPDSE_ATTINY_25				14
#define _GPDSE_ATTINY_45				15
#define _GPDSE_ATTINY_85				16
#define _GPDSE_ATTINY_2313				17
#define _GPDSE_ATTINY_4313				18
#define _GPDSE_ATTINY_2313A				19

// ARM microcontroller devices
#define _GPDSE_STM32F030R8				201
#define _GPDSE_STM32L053R8				202

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

// -----------------------------------------------------------------------------
// Microcontroller selection ---------------------------------------------------

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

// ATmega2560
#if defined(_GPDSE_DEVICE_ATMEGA_2560)
#	if defined(_GPDSE_MCU)
#		warning	Verify your selection in "Microcontroller Selection" section!
#		error	More than one device are selected!
#	else
#		define _GPDSE_MCU						_GPDSE_ATMEGA_2560
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

// Verify microcontroller selection
#if !defined(_GPDSE_MCU)
#	warning	Verify your selection in "Microcontroller Selection" section!
#	error	No device was selected.
#endif

// -----------------------------------------------------------------------------
// Platform selection ----------------------------------------------------------

#if _GPDSE_MCU < 200
#	define _GPDSE_PLATFORM_AVR8
#else
#	define _GPDSE_PLATFORM_ARM32
#endif

// -----------------------------------------------------------------------------
// System clock configuration --------------------------------------------------

#if _GPDSE_CPU_CLOCK > 4294967295UL
#	error	Error 1 - System frequency is too high.
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

#if defined(_GPDSE_PLATFORM_AVR8)		// AVR headers
#	include <avr/io.h>
#	include <avr/interrupt.h>
#	include <util/delay.h>
#elif defined(_GPDSE_PLATFORM_ARM32)		// ARM headers
	//Bibliotecas HAL
	//Bibliotecas HAL
	//Bibliotecas HAL
	//Bibliotecas HAL
#endif

// -----------------------------------------------------------------------------
// Include AVR device part specific header files -------------------------------


#if (_GPDSE_MCU == _GPDSE_ATMEGA_48) || (_GPDSE_MCU == _GPDSE_ATMEGA_88) || \
		(_GPDSE_MCU == _GPDSE_ATMEGA_168) || (_GPDSE_MCU == _GPDSE_ATMEGA_328)
#	include "part/gpdseLibAtmega328.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATMEGA_16U4) || (_GPDSE_MCU == _GPDSE_ATMEGA_32U4)
#	include "part/gpdseLibAtmega32u4.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATMEGA_1280) || (_GPDSE_MCU == _GPDSE_ATMEGA_2560)
#	include "part/gpdseLibAtmega2560.hpp"
#elif	(_GPDSE_MCU == _GPDSE_ATTINY_85)
#	include "part/gpdseLibAttiny85.hpp"
#endif

// *INDENT-ON*

#endif // __GPDSE_LIB_PROJECT_CONFIG_HPP
