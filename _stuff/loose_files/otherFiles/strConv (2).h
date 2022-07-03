/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			strConv.h
 * Module:			Functions for converting strings
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	April 11, 2018
 * -------------------------------------------------------------------------- */

#ifndef __STR_CONV_H
#define __STR_CONV_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <string.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

/* NONE */

// Macrofunction definitions --------------------------------------------------

#define defValueToStr(s) defNameToStr(s)
#define defNameToStr(s) #s

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

bool_t	strToInt8(char * string, int8_t * value);
bool_t	strToUint8(char * string, uint8_t * value);
bool_t	strToInt16(char * string, int16_t * value);
bool_t	strToUint16(char * string, uint16_t * value);
bool_t	strToInt32(char * string, int32_t * value);
bool_t	strToUint32(char * string, uint32_t * value);
bool_t	strToInt64(char * string, int64_t * value);
bool_t	strToUint64(char * string, uint64_t * value);

#endif
