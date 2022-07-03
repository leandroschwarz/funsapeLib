/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			strConv.c
 * Module:			Functions for converting strings
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	April 11, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "strConv.h"
#if __STR_CONV_H != 1
	#error Error 101 - Build mismatch on header and source code files (strConv).
#endif

/* -----------------------------------------------------------------------------
 * strToInt8
 * -------------------------------------------------------------------------- */

bool_t strToInt8(char * string, int8_t * value)
{
	char auxChar;
	bool_t negate = FALSE;

	if(*string == '-') {
		negate = TRUE;
		string++;
	}

	*value = 0;
	while(*string != '\0') {
		auxChar = *string;
		auxChar -= '0';
		if(auxChar > 9) {
			return FALSE;
		}
		(*value) *= 10;
		(*value) += auxChar;
		string++;
	}

	if(negate) {
		*value = 0 - (*value);
	}

	return TRUE;
}

/* -----------------------------------------------------------------------------
 * strToUint8
 * -------------------------------------------------------------------------- */

bool_t strToUint8(char * string, uint8_t * value)
{
	char auxChar;

	*value = 0;
	while(*string != '\0') {
		auxChar = *string;
		auxChar -= '0';
		if(auxChar > 9) {
			return FALSE;
		}
		(*value) *= 10;
		(*value) += auxChar;
		string++;
	}

	return TRUE;
}

/* -----------------------------------------------------------------------------
 * strToInt16
 * -------------------------------------------------------------------------- */

bool_t strToInt16(char * string, int16_t * value)
{
	char auxChar;
	bool_t negate = FALSE;

	if(*string == '-') {
		negate = TRUE;
		string++;
	}

	*value = 0;
	while(*string != '\0') {
		auxChar = *string;
		auxChar -= '0';
		if(auxChar > 9) {
			return FALSE;
		}
		(*value) *= 10;
		(*value) += auxChar;
		string++;
	}

	if(negate) {
		*value = 0 - (*value);
	}

	return TRUE;
}

/* -----------------------------------------------------------------------------
 * strToUint16
 * -------------------------------------------------------------------------- */

bool_t	strToUint16(char * string, uint16_t * value)
{
	char auxChar;

	*value = 0;
	while(*string != '\0') {
		auxChar = *string;
		auxChar -= '0';
		if(auxChar > 9) {
			return FALSE;
		}
		(*value) *= 10;
		(*value) += auxChar;
		string++;
	}

	return TRUE;
}

/* -----------------------------------------------------------------------------
 * strToInt32
 * -------------------------------------------------------------------------- */

bool_t	strToInt32(char * string, int32_t * value)
{
	char auxChar;
	bool_t negate = FALSE;

	if(*string == '-') {
		negate = TRUE;
		string++;
	}

	*value = 0;
	while(*string != '\0') {
		auxChar = *string;
		auxChar -= '0';
		if(auxChar > 9) {
			return FALSE;
		}
		(*value) *= 10;
		(*value) += auxChar;
		string++;
	}

	if(negate) {
		*value = 0 - (*value);
	}

	return TRUE;
}

/* -----------------------------------------------------------------------------
 * strToUint32
 * -------------------------------------------------------------------------- */

bool_t	strToUint32(char * string, uint32_t * value)
{
	char auxChar;

	*value = 0;
	while(*string != '\0') {
		auxChar = *string;
		auxChar -= '0';
		if(auxChar > 9) {
			return FALSE;
		}
		(*value) *= 10;
		(*value) += auxChar;
		string++;
	}

	return TRUE;
}

/* -----------------------------------------------------------------------------
 * strToInt64
 * -------------------------------------------------------------------------- */

bool_t	strToInt64(char * string, int64_t * value)
{
	char auxChar;
	bool_t negate = FALSE;

	if(*string == '-') {
		negate = TRUE;
		string++;
	}

	*value = 0;
	while(*string != '\0') {
		auxChar = *string;
		auxChar -= '0';
		if(auxChar > 9) {
			return FALSE;
		}
		(*value) *= 10;
		(*value) += auxChar;
		string++;
	}

	if(negate) {
		*value = 0 - (*value);
	}

	return TRUE;
}

/* -----------------------------------------------------------------------------
 * strToUint64
 * -------------------------------------------------------------------------- */

bool_t	strToUint64(char * string, uint64_t * value)
{
	char auxChar;

	*value = 0;
	while(*string != '\0') {
		auxChar = *string;
		auxChar -= '0';
		if(auxChar > 9) {
			return FALSE;
		}
		(*value) *= 10;
		(*value) += auxChar;
		string++;
	}

	return TRUE;
}
