/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			sevenSegments.hpp
 * Module:			Seven segments display controller
 * Author:			Leandro Schwarz
 * Version:			1.2
 * Last edition:	2016-01-15
 * Purpose:			Seven segments display controller with support to common
 *					anode and common cathode displays. Special characters dash
 *					(0xFD), U (0xFE) and display off (0xFF) were also implented
 * -------------------------------------------------------------------------- */

#ifndef __SEVENSEGMENTS_HPP
#define __SEVENSEGMENTS_HPP 12

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\funsapeLibGlobalDefines.hpp"
#if __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error Error 100 - The defintion file is outdated (globalDefines must be version 12.3 or greater).
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#define DISPLAY_DASH	0xFD
#define DISPLAY_U		0xFE
#define DISPLAY_OFF		0xFF

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum displayType_t {
	COMMON_ANODE	= 0,
	COMMON_CATHODE
} displayType_t;

// -----------------------------------------------------------------------------
// Functions declarations ------------------------------------------------------

uint8_t hex2SevenSegments(uint8_t hexValue_p, displayType_t displayType_p, bool showPoint_p);

#endif
