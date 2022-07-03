/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			hexToSevenSegments.h
 * Module:			Seven segments display controller
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * Purpose:			Seven segments display controller with support to common
 *					anode and common cathode displays. Special characters dash
 *					(0xFD), U (0xFE) and display off (0xFF) were also
 *					implemented
 * -------------------------------------------------------------------------- */

#ifndef __HEX_TO_SEVEN_SEGMENTS_H
#define __HEX_TO_SEVEN_SEGMENTS_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#define DISPLAY_DASH	0xFD
#define DISPLAY_U		0xFE
#define DISPLAY_OFF		0xFF

// -----------------------------------------------------------------------------
// Functions declarations ------------------------------------------------------

uint8_t hexToSevenSegments(uint8_t hexValue, displayType_t displayType, bool_t point);

#endif
