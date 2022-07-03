/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibDateTime.cpp
 * Module:          Date and Time definitions for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * Note:            The class always stores the hour in 24 Hours format.
 *                      Functions to convert between formats are provided and
 *                      are automatically callel when needed.
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibDateTime.hpp"
#ifndef __FUNSAPE_LIB_DATETIME_HPP
#   error [funsapeLibDateTime.cpp] Error - Header file (funsapeLibDateTime.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DATETIME_HPP != __BUILD_NUMBER__
#   error [funsapeLibDateTime.cpp] Error - Build mismatch between source (funsapeLibDateTime.cpp) and header (funsapeLibDateTime.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

// NONE

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Static functions declarations
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

/**
 * @brief Construct a new Date Time:: Date Time object
 *
 * Detailed description of this element.
 *
 */
DateTime::DateTime(void)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::DateTime(void)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Resets data members
    this->_clearData();

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Date Time:: Date Time object
 *
 * Detailed description of this element.
 *
 */
DateTime::~DateTime(void)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::~DateTime(void)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Returns successfully
    debugMessage(Error::NONE);
    return;
}

// =============================================================================
// Inherited methods - Public
// =============================================================================

// NONE

// =============================================================================
// Inherited methods - Private
// =============================================================================

// NONE

// =============================================================================
// Inherited methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Public
// =============================================================================

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return Error
 */
Error DateTime::getLastError(void)
{
    // Returns last error
    return this->_lastError;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool_t DateTime::isDateSet(void)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::isDateSet(void)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Returns if date is set
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return this->_dateSet;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool_t DateTime::isTimeSet(void)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::isTimeSet(void)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Returns if time is set
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return this->_timeSet;
}

//     //////////////////     DATE RELATED METHODS //////////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[out] year_p description of this parameter
 * @param[out] month_p description of this parameter
 * @param[out] day_p description of this parameter
 * @param[out] weekDay_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::getDate(uint16_t *year_p, Month *month_p, uint8_t *day_p, WeekDay *weekDay_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::getDate(uint16_t *, Month *, uint8_t *, WeekDay *)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Checks initialization
    if (!this->_dateSet) {
        // Returns error
        this->_lastError = Error::DATETIME_DATE_NOT_INITIALIZED;
        debugMessage(Error::DATETIME_DATE_NOT_INITIALIZED);
        return false;
    }
    // CHECK FOR ERROR - pointer null
    if ((!isPointerValid(year_p)) || (!isPointerValid(month_p)) || (!isPointerValid(day_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Retrieve values
    *day_p      = this->_day;
    *month_p    = this->_month;
    *year_p     = this->_year;
    if (isPointerValid(weekDay_p)) {
        *weekDay_p = this->_weekDay;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[out] weekDay_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::getWeekDay(WeekDay *weekDay_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::getWeekDay(WeekDay *)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Checks initialization
    if (!this->_dateSet) {
        // Returns error
        this->_lastError = Error::DATETIME_DATE_NOT_INITIALIZED;
        debugMessage(Error::DATETIME_DATE_NOT_INITIALIZED);
        return false;
    }
    // CHECK FOR ERROR - pointer null
    if (!isPointerValid(weekDay_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Retrieve values
    *weekDay_p = this->_weekDay;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool_t DateTime::isLeapYear(void)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::isLeapYear(void)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Checks initialization
    if (!this->_dateSet) {
        // Returns error
        this->_lastError = Error::DATETIME_DATE_NOT_INITIALIZED;
        debugMessage(Error::DATETIME_DATE_NOT_INITIALIZED);
        return false;
    }

    // Returns result
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return this->_leapYear;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] year_p description of this parameter
 * @param[in] month_p description of this parameter
 * @param[in] day_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::setDate(cuint16_t year_p, const Month month_p, cuint8_t day_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::setDate(cuint16_t, const Month, cuint8_t)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Deinitialize
    this->_dateSet = false;

    // Check for errors - month day
    if (!this->_isValidDate(year_p, month_p, day_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_dateSet = true;
    this->_year = year_p;
    this->_leapYear = this->_isLeapYear(year_p);
    this->_month = month_p;
    this->_day = day_p;
    this->_weekDay = this->_evaluateDayOfWeek();

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     //////////////////     TIME RELATED METHODS //////////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[out] hours_p description of this parameter
 * @param[out] minutes_p description of this parameter
 * @param[out] seconds_p description of this parameter
 * @param[in] timeFormat_p description of this parameter
 * @param[out] amPmFlag_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::getTime(uint8_t *hours_p, uint8_t *minutes_p, uint8_t *seconds_p, const TimeFormat timeFormat_p,
        AmPmFlag *amPmFlag_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::getTime(uint8_t *, uint8_t *, uint8_t *, const TimeFormat, AmPmFlag *)",
            Debug::CodeIndex::DATE_TIME_MODULE);

    // Local variables
    uint8_t auxHours;
    AmPmFlag auxAmPmFlag;

    // Checks initialization
    if (!this->_timeSet) {
        // Returns error
        this->_lastError = Error::DATETIME_TIME_NOT_INITIALIZED;
        debugMessage(Error::DATETIME_TIME_NOT_INITIALIZED);
        return false;
    }
    // CHECK FOR ERROR - pointer null
    if ((!isPointerValid(hours_p)) || (!isPointerValid(minutes_p)) || (!isPointerValid(seconds_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    // CHECK FOR ERROR - pointer null
    if ((timeFormat_p == TimeFormat::FORMAT_12_HOURS) && (!isPointerValid(amPmFlag_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Retrieve values
    auxAmPmFlag = this->_amPmFlag;
    auxHours = this->_hours;

    // Convert between time formats
    if (timeFormat_p != this->_timeFormat) {
        if (!this->_convertTimeFormat(&auxHours, &auxAmPmFlag, this->_timeFormat, timeFormat_p)) {
            // Returns error
            debugMessage(this->_lastError);
            return false;
        }
    }

    // Update function arguments
    if (isPointerValid(amPmFlag_p)) {
        *amPmFlag_p = auxAmPmFlag;
    }
    *hours_p = auxHours;
    *minutes_p = this->_minutes;
    *seconds_p = this->_seconds;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[out] hours_p description of this parameter
 * @param[out] minutes_p description of this parameter
 * @param[out] seconds_p description of this parameter
 * @param[out] milliseconds_p description of this parameter
 * @param[in] timeFormat_p description of this parameter
 * @param[out] amPmFlag_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::getTime(uint8_t *hours_p, uint8_t *minutes_p, uint8_t *seconds_p, uint16_t *milliseconds_p,
        const TimeFormat timeFormat_p, AmPmFlag *amPmFlag_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::getTime(uint8_t *, uint8_t *, uint8_t *, uint16_t *, const TimeFormat, AmPmFlag *)"
            , Debug::CodeIndex::DATE_TIME_MODULE);

    // Local variables
    uint8_t auxHours;
    AmPmFlag auxAmPmFlag;

    // Checks initialization
    if (!this->_timeSet) {
        // Returns error
        this->_lastError = Error::DATETIME_TIME_NOT_INITIALIZED;
        debugMessage(Error::DATETIME_TIME_NOT_INITIALIZED);
        return false;
    }
    // CHECK FOR ERROR - pointer null
    if ((!isPointerValid(hours_p)) || (!isPointerValid(minutes_p)) || (!isPointerValid(seconds_p)) ||
            (!isPointerValid(milliseconds_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    // CHECK FOR ERROR - pointer null
    if ((timeFormat_p == TimeFormat::FORMAT_12_HOURS) && (!isPointerValid(amPmFlag_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Retrieve values
    auxAmPmFlag = this->_amPmFlag;
    auxHours = this->_hours;

    // Convert between time formats
    if (timeFormat_p != this->_timeFormat) {
        if (!this->_convertTimeFormat(&auxHours, &auxAmPmFlag, this->_timeFormat, timeFormat_p)) {
            // Returns error
            debugMessage(this->_lastError);
            return false;
        }
    }

    // Update function arguments
    if (isPointerValid(amPmFlag_p)) {
        *amPmFlag_p = auxAmPmFlag;
    }
    *hours_p        = auxHours;
    *minutes_p      = this->_minutes;
    *seconds_p      = this->_seconds;
    *milliseconds_p = this->_milliseconds;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[out] timeFormat_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::getTimeFormat(TimeFormat *timeFormat_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::getTimeFormat(TimeFormat *)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Checks initialization
    if (!this->_timeSet) {
        // Returns error
        this->_lastError = Error::DATETIME_TIME_NOT_INITIALIZED;
        debugMessage(Error::DATETIME_TIME_NOT_INITIALIZED);
        return false;
    }
    // CHECK FOR ERROR - pointer null
    if (!isPointerValid(timeFormat_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Retrieve values
    *timeFormat_p = this->_timeFormat;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[out] timeZone_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::getTimeZone(TimeZone *timeZone_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::getTimeZone(TimeZone *)", Debug::CodeIndex::DATE_TIME_MODULE);

    // CHECK FOR ERROR - pointer null
    if (!isPointerValid(timeZone_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Retrieve values
    *timeZone_p = this->_timeZone;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] hours_p description of this parameter
 * @param[in] minutes_p description of this parameter
 * @param[in] seconds_p description of this parameter
 * @param[in] timeFormat_p description of this parameter
 * @param[in] amPmFlag_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::setTime(cuint8_t hours_p, cuint8_t minutes_p, cuint8_t seconds_p, const TimeFormat timeFormat_p,
        const AmPmFlag amPmFlag_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::setTime(cuint8_t, cuint8_t, cuint8_t, const TimeFormat, const AmPmFlag)",
            Debug::CodeIndex::DATE_TIME_MODULE);

    // Deinitialize
    this->_timeSet = false;

    // Check for errors - Valid time
    if (!this->_isValidTime(hours_p, minutes_p, seconds_p, timeFormat_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_timeSet = true;
    this->_hours = hours_p;
    this->_timeFormat = timeFormat_p;
    this->_amPmFlag = amPmFlag_p;
    this->_minutes = minutes_p;
    this->_seconds = seconds_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] hours_p description of this parameter
 * @param[in] minutes_p description of this parameter
 * @param[in] seconds_p description of this parameter
 * @param[in] milliseconds_p description of this parameter
 * @param[in] timeFormat_p description of this parameter
 * @param[in] amPmFlag_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::setTime(cuint8_t hours_p, cuint8_t minutes_p, cuint8_t seconds_p, cuint16_t milliseconds_p,
        const TimeFormat timeFormat_p, const AmPmFlag amPmFlag_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::setTime(cuint8_t, cuint8_t, cuint8_t, cuint16_t, const TimeFormat, const AmPmFlag)"
            , Debug::CodeIndex::DATE_TIME_MODULE);

    // Deinitialize
    this->_timeSet = false;

    // Check for errors - Valid time
    if (!this->_isValidTime(hours_p, minutes_p, seconds_p, timeFormat_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    if (milliseconds_p > 999) {
        this->_milliseconds = 0;
    } else {
        this->_milliseconds = milliseconds_p;
    }
    this->_hours = hours_p;
    this->_minutes = minutes_p;
    this->_seconds = seconds_p;
    this->_amPmFlag = amPmFlag_p;
    this->_timeFormat = timeFormat_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] timeFormat_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::setTimeFormat(const TimeFormat timeFormat_p)
{
    // Local variables
    uint8_t auxHours = this->_hours;
    AmPmFlag auxAmPmFlag = this->_amPmFlag;

    // Mark passage for debugging purpose
    debugMark("DateTime::setTimeFormat(const TimeFormat)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Checks initialization
    if (!this->_timeSet) {
        // Returns error
        this->_lastError = Error::DATETIME_TIME_NOT_INITIALIZED;
        debugMessage(Error::DATETIME_TIME_NOT_INITIALIZED);
        return false;
    }

    // Just exit if conversion is not necessary
    if (this->_timeFormat == timeFormat_p) {
        // Returns successfully
        this->_lastError = Error::NONE;
        debugMessage(Error::NONE);
        return false;
    }

    // Converts time to given format
    if (!this->_convertTimeFormat(&auxHours, &auxAmPmFlag, this->_timeFormat, timeFormat_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Updates data members
    this->_hours = auxHours;
    this->_amPmFlag = auxAmPmFlag;
    this->_timeFormat = timeFormat_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] timeZone_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::setTimeZone(const TimeZone timeZone_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::setTimeZone(const TimeZone)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Update data members
    this->_timeZone = timeZone_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void DateTime::_clearData(void)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::_clearData(void)", Debug::CodeIndex::DATE_TIME_MODULE);

    //     /////////////////    CONTROL AND STATUS     //////////////////     //
    this->_dateSet                      = false;
    this->_timeSet                      = false;
    //     /////////////////     DATE DATA MEMBERS     //////////////////     //
    this->_day                          = 0;
    this->_leapYear                     = false;
    this->_month                        = Month::UNDEFINED;
    this->_year                         = 0;
    this->_weekDay                      = WeekDay::UNDEFINED;
    //     /////////////////     TIME DATA MEMBERS     //////////////////     //
    this->_amPmFlag                     = AmPmFlag::AM;
    this->_hours                        = 0;
    this->_milliseconds                 = 0;
    this->_minutes                      = 0;
    this->_seconds                      = 0;
    this->_timeFormat                   = TimeFormat::FORMAT_24_HOURS;
    this->_timeZone                     = TimeZone::UNDEFINED;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

//     //////////////////     DATE RELATED METHODS //////////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
DateTime::WeekDay DateTime::_evaluateDayOfWeek(void)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::_evaluateDayOfWeek(void)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Local variables
    uint16_t aux16;
    uint16_t auxYear = this->_year;
    WeekDay auxWeekDay = WeekDay::UNDEFINED;

    // Week day evaluation
    auxYear -= (this->_month < 3);
    aux16 = (auxYear + auxYear / 4 - auxYear / 100 + auxYear / 400 + "-bed=pen+mad."[(uint8_t)(
                                    this->_month)] + this->_day) % 7;
    aux16++;
    auxWeekDay = (WeekDay)aux16;

    // Returns result
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return auxWeekDay;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] year_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::_isLeapYear(cuint16_t year_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::_isLeapYear(cuint16_t)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Local variables
    bool_t auxLeapYear = false;

    if (year_p % 400 == 0) {                // Yes - divisible by 400
        auxLeapYear = true;
    } else if (year_p % 100 == 0) {     // No - divisible by 100 but not by 400
        auxLeapYear = false;
    } else if (year_p % 4 == 0) {       // Yes - divisible by 4 but not by 100
        auxLeapYear = true;
    } else {                            // No - to all other years
        auxLeapYear = false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return auxLeapYear;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] year_p description of this parameter
 * @param[in] month_p description of this parameter
 * @param[in] day_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::_isValidDate(cuint16_t year_p, const Month month_p, cuint8_t day_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::_isValidDate(cuint16_t, const Month, cuint8_t)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Local variables
    bool_t validDate = true;

    // Checks if any date value is zero
    if ((day_p == 0) || (month_p == Month::UNDEFINED) || (year_p == 0)) {
        // Returns error
        this->_lastError = Error::DATETIME_DATE_INVALID;
        debugMessage(Error::DATETIME_DATE_INVALID);
        return false;
    }

    // Checks if date is greater than available month days
    switch (month_p) {
    case Month::JANUARY:        // 31 days
    case Month::MARCH:
    case Month::MAY:
    case Month::JULY:
    case Month::AUGUST:
    case Month::OCTOBER:
    case Month::DECEMBER:
        if (day_p > 31) {
            // Returns error
            validDate = false;
        }
        break;
    case Month::APRIL:
    case Month::JUNE:
    case Month::SEPTEMBER:
    case Month::NOVEMBER:       // 30 days
        if (day_p > 30) {
            validDate = false;
        }
        break;
    case Month::FEBRUARY:       // 28 or 29 days
        if (day_p > 29) {
            validDate = false;
        } else if (day_p == 29) {
            if (!this->_isLeapYear(year_p)) {
                validDate = false;
            }
        }
        break;
    default:
        validDate = false;
        break;
    }

    // Checking result
    if (!validDate) {
        // Returns error
        this->_lastError = Error::DATETIME_DATE_INVALID;
        debugMessage(Error::DATETIME_DATE_INVALID);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     //////////////////     TIME RELATED METHODS //////////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in,out] hours_p description of this parameter
 * @param[in,out] amPmFlag_p description of this parameter
 * @param[in] fromFormat_p description of this parameter
 * @param[in] toFormat_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::_convertTimeFormat(uint8_t *hours_p, AmPmFlag *amPmFlag_p, const TimeFormat fromFormat_p,
        const TimeFormat toFormat_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::_convertTimeFormat(uint8_t *, AmPmFlag *, const TimeFormat, const TimeFormat)"
            , Debug::CodeIndex::DATE_TIME_MODULE);

    // Conversion procedure
    if ((fromFormat_p == TimeFormat::FORMAT_12_HOURS) && (toFormat_p == TimeFormat::FORMAT_24_HOURS)) {
        // Convert 12-hour time format to 24-hour time format
        if (*amPmFlag_p == AmPmFlag::AM) {
            if (*hours_p == 12) {           // Time is 12:00 <-> 12:59 AM => 0 hours
                *hours_p = 0;
            } else {                        // Time is 1:00 <-> 11:59 AM => do nothing
                doNothing();
            }
        } else { // amPmFlag_p == AmPmFlag::PM
            if (*hours_p == 12) {           // Time is 1:00 <-> 11:59 PM => add 12 hours
                *hours_p += 12;
            } else {                        // Time is 12:00 <-> 12:59 PM => do nothing
                doNothing();
            }
        }
    } else if ((fromFormat_p == TimeFormat::FORMAT_24_HOURS) && (toFormat_p == TimeFormat::FORMAT_12_HOURS)) {
        // Convert 24-hour time format to 12-hour time format
        switch (*hours_p) {
        case 0:
            *hours_p = 12;
            *amPmFlag_p = AmPmFlag::AM;
            break;
        case 1 ... 11:
            *amPmFlag_p = AmPmFlag::AM;
            break;
        case 12:
            *amPmFlag_p = AmPmFlag::PM;
            break;
        case 13 ... 23:
            *hours_p -= 12;
            *amPmFlag_p = AmPmFlag::PM;
            break;
        default:
            // Returns error
            this->_lastError = Error::UNKNOWN;
            debugMessage(Error::UNKNOWN);
            return false;
        }
    } else {
        // No conversion is needed
        doNothing();
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] hours_p description of this parameter
 * @param[in] minutes_p description of this parameter
 * @param[in] seconds_p description of this parameter
 * @param[in] timeFormat_p description of this parameter
 * @return true
 * @return false
 */
bool_t DateTime::_isValidTime(cuint8_t hours_p, cuint8_t minutes_p, cuint8_t seconds_p, const TimeFormat timeFormat_p)
{
    // Mark passage for debugging purpose
    debugMark("DateTime::_isValidTime(cuint8_t, cuint8_t, cuint8_t, const TimeFormat)", Debug::CodeIndex::DATE_TIME_MODULE);

    // Check values
    if ((seconds_p > 59) || (minutes_p > 59)) {
        // Returns error
        this->_lastError = Error::DATETIME_TIME_INVALID;
        debugMessage(Error::DATETIME_TIME_INVALID);
        return false;
    }
    if (timeFormat_p == TimeFormat::FORMAT_12_HOURS) {
        if ((hours_p == 0) || (hours_p > 12)) {
            // Returns error
            this->_lastError = Error::DATETIME_TIME_INVALID;
            debugMessage(Error::DATETIME_TIME_INVALID);
            return false;
        }
    } else {
        if (hours_p > 23) {
            // Returns error
            this->_lastError = Error::DATETIME_TIME_INVALID;
            debugMessage(Error::DATETIME_TIME_INVALID);
            return false;
        }
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

// =============================================================================
// END OF FILE
// =============================================================================
