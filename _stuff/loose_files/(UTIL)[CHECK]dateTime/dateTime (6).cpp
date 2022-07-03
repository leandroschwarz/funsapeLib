/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		dateTime.cpp
 * Module:			Date and Time definitionsfor FunSAPE++ Embedded Library
 *					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "dateTime.hpp"
#ifndef __FUNSAPE_DATE_TIME_HPP
#	error	[dateTime.cpp] Error 103 - Header file (dateTime.hpp) must be corruted!
#elif __FUNSAPE_DATE_TIME_HPP != __BUILD_TO_REPLACE__
#	error	[dateTime.cpp] Error 104 - Build mismatch between source (dateTime.hpp) and header (dateTime.hpp) files!
#endif

// =============================================================================
// File exclusive - Constant definitions
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

// NONE

// =============================================================================
// File exclusive - Macro-functions definitions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Private functions declarations
// =============================================================================

// NONE

// =============================================================================
// DateTime class - Constructors
// =============================================================================

DateTime::DateTime(void)
{
	// Default date time
	this->_year = YEAR_UNDEFINED;
	this->_month = MONTH_UNDEFINED;
	this->_day = DAY_UNDEFINED;
	this->_weekDay = WEEKDAY_UNDEFINED;
	this->_hour = HOUR_UNDEFINED;
	this->_minute = MINUTE_UNDEFINED;
	this->_second = SECOND_UNDEFINED;
	this->_millisecond = MILLISENCOND_UNDEFINED;
	this->_leapYear = false;
	this->_summerTime = false;
	this->_timeZone = UTC_OUT_OF_RANGE;
	this->_dateInitialized = false;
	this->_timeInitialized = false;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	return;
}

DateTime::~DateTime(void)
{
	// TODO: Create destructor
}

// =============================================================================
// DateTime class - Public methods
// =============================================================================

bool DateTime::getDate(uint8_t *day, month_t *month, uint16_t *year)
{
	// Check for errors
	if (!this->_dateInitialized) {			// Initialization
		this->_lastError = ERROR_DATETIME_DATE_NOT_INITIALIZED;
		return false;
	}
	if (day == NULL) {					// Argument verification
		this->_lastError = ERROR_ARGUMENTS_MISMATCH;
		return false;
	}

	// Update data members
	if (year != NULL) {
		*year = this->_year;
	}
	if (month != NULL) {
		*month = this->_month;
	}
	*day = this->_day;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool DateTime::setDate(uint8_t day, month_t month, uint16_t year)
{
	// Check for errors - Year
	if (year > 4097) {
		// Returns with error
		this->_lastError = ERROR_DATETIME_INVALID_YEAR;
		return false;
	}
	// Check for errors - Month and day
	if (day == 0) {
		// Returns with error
		this->_lastError = ERROR_DATETIME_INVALID_MONTH_DAY;
		return false;
	}
	switch (month) {
	case MONTH_JANUARY:		// Months with 31 days
	case MONTH_MARCH:
	case MONTH_MAY:
	case MONTH_JULY:
	case MONTH_AUGUST:
	case MONTH_OCTOBER:
	case MONTH_DECEMBER:
		if (day > 31) {
			// Returns with error
			this->_lastError = ERROR_DATETIME_INVALID_MONTH_DAY;
			return false;
		}
		break;
	case MONTH_APRIL:
	case MONTH_JUNE:
	case MONTH_SEPTEMBER:
	case MONTH_NOVEMBER:
		if (day > 30) {
			// Returns with error
			this->_lastError = ERROR_DATETIME_INVALID_MONTH_DAY;
			return false;
		}
		break;
	case MONTH_FEBRUARY:
		if (day > 29) {
			// Returns with error
			this->_lastError = ERROR_DATETIME_INVALID_MONTH_DAY;
			return false;
		} else if (day == 29) {
			if (!this->_isLeapYear(year)) {
				// Returns with error
				this->_lastError = ERROR_DATETIME_INVALID_MONTH_DAY;
				return false;
			}
			break;
		}
	default:
		// Returns with error
		this->_lastError = ERROR_DATETIME_INVALID_MONTH;
		return false;
	}

	// Update data members
	this->_year = year;
	this->_month = month;
	this->_day = day;
	this->_leapYear = this->_isLeapYear(year);
	this->_weekDay = this->_getWeekDay();
	this->_dateInitialized = true;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool DateTime::setTime(uint8_t hour, uint8_t minute, uint8_t second, utcTimeZone_e timeZone)
{
	// Execute and returns operation result
	return this->setTime(hour, minute, second, 0, timeZone);
}

bool DateTime::setTime(uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond, utcTimeZone_e timeZone)
{
	// Check for errors - Hour
	if (hour > 23) {
		// Returns with error
		this->_lastError = ERROR_DATETIME_INVALID_HOUR;
		return false;
	}
	// Check for errors - Minute
	if (minute > 59) {
		// Returns with error
		this->_lastError = ERROR_DATETIME_INVALID_MINUTE;
		return false;
	}
	// Check for errors - Second
	if (second > 59) {
		// Returns with error
		this->_lastError = ERROR_DATETIME_INVALID_SECOND;
		return false;
	}
	// Check for errors - Millisecond
	if (millisecond > 999) {
		// Returns with error
		this->_lastError = ERROR_DATETIME_INVALID_MILLISECOND;
		return false;
	}
	// Check for errors - TimeZone
	if ((timeZone == UTC_UNDEFINED) || (timeZone >= UTC_OUT_OF_RANGE)) {
		// Returns with error
		this->_lastError = ERROR_DATETIME_INVALID_TIMEZONE;
		return false;
	}

	// Update data members
	this->_hour = hour;
	this->_minute = minute;
	this->_second = second;
	this->_millisecond = millisecond;
	this->_timeZone = timeZone;
	this->_timeInitialized = true;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool DateTime::getTime(uint8_t *hour, uint8_t *minute, uint8_t *second, uint16_t *millisecond, utcTimeZone_e *timeZone)
{
	// TODO: Test for argument errors

	// Check for errors - Time initialization
	if (!this->_timeInitialized) {
		// Returns error
		this->_lastError = ERROR_DATETIME_TIME_NOT_INITIALIZED;
	}
	// Update data members
	if (timeZone != NULL) {
		*timeZone = this->_timeZone;
	}
	if (millisecond != NULL) {
		*millisecond = this->_millisecond;
	}
	*second = this->_second;
	*minute = this->_minute;
	*hour = this->_hour;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	return true;
}

weekDay_t DateTime::getWeekDay(void)
{
	// Execute and returns operation result
	return this->getWeekDay(this->_day, this->_month, this->_year);
}

weekDay_t DateTime::getWeekDay(uint8_t day, uint8_t month, uint16_t year)
{
	// Execute and returns operation result
	return this->getWeekDay(day, (month_t)month, year);
}

weekDay_t DateTime::getWeekDay(uint8_t day, month_t month, uint16_t year)
{
	// Local variables
	uint16_t aux16 = 0;

	// TODO: Verify if the resolved arguments are valid
	// Resolve arguments
	year = (year == 0) ? this->_year : year;
	month = (month == 0) ? this->_month : month;
	day = (day == 0) ? this->_day : day;
	// TODO: If arguments are 0, test if date is initialized

	// TODO: Verify the year range which this equation is valid

	// Evaluate day of the week
	year -= ((uint8_t)month < 3);
	aux16 = (year + year / 4 - year / 100 + year / 400 + "-bed=pen+mad."[(uint8_t)month] + day) % 7;
	aux16++;

	// Returns the day of the week
	return (weekDay_t)aux16;
}

// =============================================================================
// DateTime class - Private methods
// =============================================================================

bool DateTime::_isLeapYear(uint16_t year)
{
	// TODO: Verify the year range which this equation is valid
	if ((year % 4) != 0) {
		// Is not a leap year
		return false;
	}
	if ((year % 400) == 0) {
		// Is a leap year
		return true;
	}
	if ((year % 100) == 0) {
		// Is not a leap year
		return false;
	}
	// Is a leap year
	return true;
}

weekDay_t DateTime::_getWeekDay(void)
{
	// TODO: Verify the year range which this equation is valid
	// Local variables
	uint16_t aux16;
	uint16_t year;

	// Evaluate day of the week
	year = this->_year;
	year -= ((uint8_t)(this->_month) < 3);
	aux16 = (year + year / 4 - year / 100 + year / 400 + "-bed=pen+mad."[(uint8_t)(this->_month)] + this->_day) % 7;
	aux16++;

	// Returns the day of the week
	return (weekDay_t)aux16;
}

// =============================================================================
// DateTime class - Protected methods
// =============================================================================

// NONE

// =============================================================================
// Public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Private functions definitions
// =============================================================================

// NONE
