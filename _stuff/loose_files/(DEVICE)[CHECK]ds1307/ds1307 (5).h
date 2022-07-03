/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			ds1307.h
 * Module:			DS1307 Real Time Clock controller
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 20, 2018
 * Purpose:			Controls and interfaces a DS1307 Real Time Clock
 * Notes:			This code makes use of some functions defined in twiMaster
 *					module. To use this code with the dedicated peripheral,
 *					just include the twiMaster module files. In order to use the
 *					device with a I2C BitBang interface, please use the same
 *					function names.
 * -------------------------------------------------------------------------- */

#ifndef __DS1307_H
#define __DS1307_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <stdio.h>
#include "..\twiMaster.h"

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#define DS1307_SLAVE_ADDRESS		0x68

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum ds1307TimeFormat_t {
	DS1307_TIME_FORMAT_AM			= 0,
	DS1307_TIME_FORMAT_PM			= 1,
	DS1307_TIME_FORMAT_24			= 2,
	DS1307_TIME_FORMAT_NO_CHANGE	= 3
} ds1307TimeFormat_t;

typedef enum ds1307Output_t {
	DS1307_OUTPUT_1_HZ		= 0,
	DS1307_OUTPUT_4096_HZ	= 1,
	DS1307_OUTPUT_8192_HZ	= 2,
	DS1307_OUTPUT_32768_HZ	= 3,
	DS1307_OUTPUT_LOW		= 5,
	DS1307_OUTPUT_HIGH		= 6,
	DS1307_OUTPUT_NO_CHANGE	= 7
} ds1307Output_t;

typedef enum ds1307CountingStatus_t {
	DS1307_COUNTING_RESUME		= 0,
	DS1307_COUNTING_HALT		= 1,
	DS1307_COUNTING_NO_CHANGE	= 3
} ds1307CountingStatus_t;

typedef struct ds1307_t {
	uint8_t					seconds			: 6;
	uint8_t					minutes			: 6;
	uint8_t					hours			: 5;
	ds1307TimeFormat_t		timeFormat		: 2;
	weekDay_t				weekDay			: 3;
	uint8_t					monthDay		: 5;
	month_t					month			: 4;
	uint8_t					year			: 7;
	ds1307Output_t			output			: 3;
	ds1307CountingStatus_t	countingStatus	: 2;
	uint8_t					unusedBits		: 5;
} ds1307_t;

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Public functions declaration ------------------------------------------------

void				ds1307ChangeTimeFormat(ds1307TimeFormat_t timeFormat, bool_t queued);
void				ds1307CountingResume(bool_t queued);
void				ds1307CountingStop(bool_t queued);
void				ds1307ReadDate(uint8_t * year, month_t * month, uint8_t * monthDay, weekDay_t * weekDay);
void				ds1307ReadDateTime(dateTime_t * dt);
uint8_t				ds1307ReadRamData(uint8_t position);
ds1307TimeFormat_t	ds1307ReadTime(uint8_t * hours, uint8_t * minutes, uint8_t * seconds);
void				ds1307SetOutput(ds1307Output_t output, bool_t queued);
void				ds1307SetDate(uint8_t year, month_t month, uint8_t monthDay, weekDay_t weekDay, bool_t queued);
void				ds1307SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds, ds1307TimeFormat_t timeFormat, bool_t queued);
void				ds1307StoreQueuedData(void);
void				ds1307WriteRamData(uint8_t data, uint8_t position);

#endif
