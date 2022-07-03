/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	dateTime.cpp
 * Module:		Date and Time definitions for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "dateTime.hpp"
#if __DATE_TIME_HPP != 1002
#	error [system.cpp] Error 102 - Build mismatch on header and source code files.
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define YEAR_UNDEFINED                  127
#define DAY_UNDEFINED                   0
#define HOUR_UNDEFINED                  25
#define MINUTE_UNDEFINED                60
#define SECOND_UNDEFINED                60
#define MILLISENCOND_UNDEFINED          1000

// =============================================================================
// File exclusive - New data types
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// Global variables
// =============================================================================

/* NONE */

// =============================================================================
// Class constructors
// =============================================================================

DateTime::DateTime()
{
	// Default date time
	this->year_ = YEAR_UNDEFINED;
	this->month_ = MONTH_UNDEFINED;
	this->day_ = DAY_UNDEFINED;
	this->weekDay_ = WEEKDAY_UNDEFINED;
	this->hour_ = HOUR_UNDEFINED;
	this->minute_ = MINUTE_UNDEFINED;
	this->second_ = SECOND_UNDEFINED;
	this->millisecond_ = MILLISENCOND_UNDEFINED;
	this->leapYear_ = false;
	this->summerTime_ = false;
	this->timeZone_ = UTC_OUT_OF_RANGE;
	this->initialized = false;
	this->lastError_ = ERROR_NONE;
}

// =============================================================================
// Class public methods
// =============================================================================

bool DateTime::getDate(uint8_t *day, month_t *month, uint16_t *year)
{
	// Check for errors
	if (!this->initialized) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (day == NULL) {
		this->lastError_ = ERROR_ARGUMENTS_MISMATCH;
		return false;
	}

	// Retrieve values
	*day = this->day_;					// Day
	if (month != NULL) {
		*month = this->month_;			// Month
	}
	if (year != NULL) {
		*year = this->year_;			// Year
	}
	this->lastError_ = ERROR_NONE;
	return true;
}

bool DateTime::getTime(uint8_t *hour, uint8_t *minute, uint8_t *second, uint16_t *millisecond)
{
	// Check for errors
	if (!this->initialized) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (hour == NULL) {
		this->lastError_ = ERROR_ARGUMENTS_MISMATCH;
		return false;
	}

	// Retrieve values
	*hour = this->hour_;					// Hour
	if (minute != NULL) {
		*minute = this->minute_;			// Minute
	}
	if (second != NULL) {
		*second = this->second_;			// Second
	}
	if (millisecond != NULL) {
		*millisecond = this->millisecond_;	// Millisecond
	}
	this->lastError_ = ERROR_NONE;
	return true;
}

bool DateTime::setDate(uint8_t day, month_t month, uint16_t year)
{
	bool auxBool = false;

	// Check for errors - month and day
	if (day == 0) {
		this->lastError_ = ERROR_DATE_INVALID;
		return false;
	}
	switch (month) {
	case MONTH_JANUARY:		// 31 days
	case MONTH_MARCH:
	case MONTH_MAY:
	case MONTH_JULY:
	case MONTH_AUGUST:
	case MONTH_OCTOBER:
	case MONTH_DECEMBER:
		if (day > 31) {
			this->lastError_ = ERROR_DATE_INVALID;
			return false;
		}
		break;
	case MONTH_APRIL:
	case MONTH_JUNE:
	case MONTH_SEPTEMBER:
	case MONTH_NOVEMBER:
		if (day > 30) {
			this->lastError_ = ERROR_DATE_INVALID;
			return false;
		}
		break;
	case MONTH_FEBRUARY:
		if (day > 29) {
			this->lastError_ = ERROR_DATE_INVALID;
			return false;
		}
		if (day == 29) {
			auxBool = this->isLeapYear(year);
			if (!auxBool) {
				this->lastError_ = ERROR_DATE_INVALID;
				return false;
			}
		}
		break;
	default:
		this->lastError_ = ERROR_DATE_INVALID;
		return false;
	}

	// Update data members
	this->year_ = year;
	this->month_ = month;
	this->day_ = day;
	this->leapYear_ = auxBool;
	this->evaluateDayOfWeek();
	this->initialized = true;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool DateTime::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
	return this->setTime(hour, minute, second, MILLISENCOND_UNDEFINED, UTC_UNDEFINED);
}

bool DateTime::setTime(uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond)
{
	return this->setTime(hour, minute, second, millisecond, UTC_UNDEFINED);
}

bool DateTime::setTime(uint8_t hour, uint8_t minute, uint8_t second, utcTimeZone_e timeZone)
{
	return this->setTime(hour, minute, second, MILLISENCOND_UNDEFINED, timeZone);
}
bool DateTime::setTime(uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond, utcTimeZone_e timeZone)
{
	// Check for errors - hour
	if (hour > 23) {
		this->lastError_ = ERROR_TIME_INVALID;
		return false;
	}
	// Check for errors - minute
	if (minute > 59) {
		this->lastError_ = ERROR_TIME_INVALID;
		return false;
	}
	// Check for errors - second
	if (second > 59) {
		this->lastError_ = ERROR_TIME_INVALID;
		return false;
	}
	// Check for errors - millisecond
	if (millisecond > 1000) {
		this->lastError_ = ERROR_TIME_INVALID;
		return false;
	}
	// Check for errors - timeZone
	if (timeZone >= UTC_OUT_OF_RANGE) {
		this->lastError_ = ERROR_TIME_INVALID;
		return false;
	}

	// Update data members
	this->hour_ = hour;
	this->minute_ = minute;
	this->second_ = second;
	if (millisecond < 999) {
		this->millisecond_ = millisecond;
	}
	if (timeZone != UTC_UNDEFINED) {
		this->timeZone_ = timeZone;
	}

	this->initialized = true;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool DateTime::getWeekDay(weekDay_t *weekDay)
{
// Check for errors
	if (!this->initialized) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (weekDay == NULL) {
		this->lastError_ = ERROR_ARGUMENTS_MISMATCH;
		return false;
	}

	// Retrieve values
	*weekDay = this->weekDay_;
	this->lastError_ = ERROR_NONE;
	return true;
}

// =============================================================================
// Class private methods
// =============================================================================

void DateTime::evaluateDayOfWeek(void)
{
	uint16_t aux16;
	uint16_t year;

	year = (uint16_t)this->year_ + 1900;
	year -= (this->month_ < 3);
	aux16 = (year + year / 4 - year / 100 + year / 400 + "-bed=pen+mad."[this->month_] + this->day_) % 7;
	aux16++;

	this->weekDay_ = (weekDay_t)aux16;
}

bool DateTime::isLeapYear(uint16_t year)
{
	if ((year % 4) != 0) {
		return false;
	}

	if ((year % 400) == 0) {
		return true;
	}

	if ((year % 100) == 0) {
		return false;
	}

	return true;
}

// =============================================================================
// Class protected methods
// =============================================================================

/* NONE */

// =============================================================================
// General public functions definitions
// =============================================================================

/* NONE */

// =============================================================================
// Public functions definitions
// =============================================================================

/* NONE */
