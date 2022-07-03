/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			matrixKeypad.h
 * Module:			Matrix keypad controller
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * Purpose:			Matrix keypad controller with support to 4x3, 4x4 and 5x3
 *					keypads and configurable debounce time
 * -------------------------------------------------------------------------- */

#ifndef __MATRIX_KEYPAD_H
#define __MATRIX_KEYPAD_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <stdarg.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum matrixKeypadType_t {
	KEYPAD_TYPE_UNDEFINED	= 0,
	KEYPAD_4X4				,
	KEYPAD_4X3				,
	KEYPAD_5X3
} matrixKeypadType_t;

typedef enum matrixKeypadResult_t {
	KEYPAD_OK						= 1,
	KEYPAD_TYPE_NOT_ALLOWED			,
	KEYPAD_MEMORY_ALLOCATION_ERROR	,
	KEYPAD_PORTS_NOT_CONFIGURED		,
	KEYPAD_KEYS_NOT_CONFIGURED		,
	KEYPAD_NOT_INITIALIZED			
} matrixKeypadResult_t;

typedef struct matrixKeypad_t {
	reg_t *				linesDDR;
	reg_t *				linesPORT;
	reg_t *				linesPIN;
	reg_t *				columnsDDR;
	reg_t *				columnsPORT;
	uint8_t *			keys;
	uint8_t				linesLSB		: 3;
	uint8_t				columnsLSB		: 3;
	matrixKeypadType_t	type			: 2;
	uint8_t				lines			: 3;
	uint8_t				columns			: 3;
	bool_t				portsConfigured	: 1;
	bool_t				keysConfigured	: 1;
	bool_t				initialized		: 1;
	uint8_t				debounceTime	: 7;
} matrixKeypad_t;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

#define matrixKeypadCreate() (matrixKeypad_t){.linesDDR = NULL, .linesPORT = NULL, .linesPIN = NULL, .columnsDDR = NULL, .columnsPORT = NULL, .keys = NULL, .linesLSB = 0, .columnsLSB = 0, .type = KEYPAD_TYPE_UNDEFINED, .lines = 0, .columns = 0, .portsConfigured = FALSE, .keysConfigured = FALSE, .initialized = FALSE, .debounceTime = 1}

// -----------------------------------------------------------------------------
// Functions declarations ------------------------------------------------------

matrixKeypadResult_t	matrixKeypadResetConfiguration(matrixKeypad_t * keypad);
matrixKeypadResult_t	matrixKeypadSetPort(matrixKeypad_t * keypad, reg_t * linesDDR, reg_t * linesPORT, reg_t * linesPIN, uint8_t linesLSB, reg_t * columnsDDR, reg_t * columnsPORT, uint8_t columnsLSB);
matrixKeypadResult_t	matrixKeypadSetKeys(matrixKeypad_t * keypad, matrixKeypadType_t type, ...);
matrixKeypadResult_t	matrixKeypadInit(matrixKeypad_t * keypad, uint8_t debounceTime);
uint8_t					matrixKeypadRead(matrixKeypad_t * keypad);

#endif
