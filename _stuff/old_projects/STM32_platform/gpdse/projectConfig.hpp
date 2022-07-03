/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	projectConfig.hpp
 * Module:		System definitions for GPDSE++ Library project
 * Authors:		Leadro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

// Include guard
#ifndef __PROJECT_CONFIG_HPP
#define __PROJECT_CONFIG_HPP		 1002

// =============================================================================
// User may edit this section
// =============================================================================

#define __DEBUG_MODE					0
#define F_CPU							16000000UL
#define DEBOUNCE_TIME					20

//#define MCU _ATMEGA_328P_
//#define MCU _ATMEGA_2560_
#define _STM32_


// =============================================================================
// User CANNOT EDIT from now on
// =============================================================================

#if F_CPU > 4294967295UL
#	error F_CPU too high!
#endif

#if defined(_ATMEGA_328P_)
#	define _PLATFORM_AVR_
#elif  defined(_ATMEGA_2560_)
#	define _PLATFORM_AVR_
#elif  defined(_STM32_)
#	define _PLATFORM_ARM_
#else
#	error [projectConfig.hpp] Error 2 - Unsupported device (MCU) selected.
#endif

#endif // __PROJECT_CONFIG_HPP
