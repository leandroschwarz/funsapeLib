/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibDs1307.cpp
 * Module:          DS1307 module interface for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibDs1307.hpp"
#ifndef __FUNSAPE_LIB_DS1307_HPP
#   error [funsapeLibDs1307.cpp] Error - Header file (funsapeLibDs1307.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DS1307_HPP != __BUILD_NUMBER__
#   error [funsapeLibDs1307.cpp] Error - Build mismatch between source (funsapeLibDs1307.cpp) and header (funsapeLibDs1307.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

cuint8_t ds1307DeviceAddress = 0x68;
cuint8_t ds1307RamSize = 56;

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
 * @brief       Construct a new Ds1307::Ds1307 object.
 * @details     The constructor also clears the class internal data. Since there
 *                  is no dynamic memory allocation procedures, the function
 *                  always sets internal the error variable to Error::NONE.
 */
Ds1307::Ds1307(void)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::Ds1307(void)", DEBUG_CODE_DS1307);

    // Resets data members
    this->_clearData();

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief       Destroy the Ds1307::Ds1307 object.
 * @details     Since there is no dynamic memory allocation procedures, the
 *                  function has nothing to do and just returns normally.
 */
Ds1307::~Ds1307(void)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::~Ds1307(void)", DEBUG_CODE_DS1307);

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


//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief       Resumes time counting.
 * @details     Resumes time counting.
 * @return true
 * @return false
 */
bool_t Ds1307::countingResume(void)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::countingResume(void)", DEBUG_CODE_DS1307);

    // Set counting status
    if (!this->_setCounting(true)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Stops time counting.
 * @details     Stops time counting.
 * @return true
 * @return false
 */
bool_t Ds1307::countingStop(void)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::countingStop(void)", DEBUG_CODE_DS1307);

    // Set counting status
    if (!this->_setCounting(false)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Returns last logged error.
 * @details     Returns the error status of the last procedure.
 * @return      FunSAPE error
 */
Error Ds1307::getLastError(void)
{
    // Returns last error
    return this->_lastError;
}

/**
 * @brief       Initializes the device.
 * @details     Performs the device initialization, checking if the device
 *                  address is reponding. Since the DS1307 has no internal
 *                  register to store it's device (or part) ID, there is no way
 *                  to be sure if the device responding is actualy a DS1307
 *                  device.
 * @param[in]   busHandler_p    pointer to the \ref{Twi} handler object
 * @return true
 * @return false
 */
bool_t Ds1307::init(Bus *busHandler_p)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::init(void)", DEBUG_CODE_DS1307);

    // Resets data members
    this->_clearData();

    // Check function arguments for errors
    if (!isPointerValid(busHandler_p)) {
        // Returns error
        this->_lastError = Error::HANDLER_POINTER_NULL;
        debugMessage(Error::HANDLER_POINTER_NULL);
        return false;
    } else if (busHandler_p->getBusType() != Bus::BusType::TWI) {
        // Returns error
        this->_lastError = Error::HANDLER_NOT_SUPPORTED;
        debugMessage(Error::HANDLER_NOT_SUPPORTED);
        return false;
    }

    // Update data members
    this->_busHandler = busHandler_p;

    // Try to checks if DS1307 is present
    // FIXME: Implmement this...
    // There is no way to know for sure...
    // XXXXX  Check if bus responds and if data makes sense

    // Mark device as initialized
    this->_initialized = true;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Sets the square wavr generator.
 * @details     Sets the square wavr generator.
 * @param[in]   squareWave_p    \ref{SquareWave} object
 * @return true
 * @return false
 */
bool_t Ds1307::setSquareWaveGenerator(SquareWave squareWave_p)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::setSquareWaveGenerator(SquareWave)", DEBUG_CODE_DS1307);

    // Local variables
    bool_t _squareWaveActivation;
    uint8_t _squareWaveFrequency;
    bool_t _squareWaveOffValue;
    uint8_t auxBuffer;

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process arguments
    switch (squareWave_p) {
    case SquareWave::OFF_LOW:
        _squareWaveActivation = false;
        _squareWaveFrequency = 0;
        _squareWaveOffValue = false;
        break;
    case SquareWave::OFF_HIGH:
        _squareWaveActivation = false;
        _squareWaveFrequency = 0;
        _squareWaveOffValue = true;
        break;
    case SquareWave::CLOCK_1_HZ:
        _squareWaveActivation = true;
        _squareWaveFrequency = 0;
        _squareWaveOffValue = false;
        break;
    case SquareWave::CLOCK_4_KHZ:
        _squareWaveActivation = true;
        _squareWaveFrequency = 1;
        _squareWaveOffValue = false;
        break;
    case SquareWave::CLOCK_8_KHZ:
        _squareWaveActivation = true;
        _squareWaveFrequency = 2;
        _squareWaveOffValue = false;
        break;
    case SquareWave::CLOCK_32_KHZ:
        _squareWaveActivation = true;
        _squareWaveFrequency = 3;
        _squareWaveOffValue = false;
        break;
    default:
        // Returns error
        this->_lastError = Error::UNKNOWN;
        debugMessage(Error::UNKNOWN);
        return false;
    }
    auxBuffer =
            (_squareWaveOffValue << 7) |
            (_squareWaveActivation << 4) |
            (_squareWaveFrequency << 0);

    // Sends data to device
    if (!this->_busHandler->setDevice(ds1307DeviceAddress)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }
    if (!this->_busHandler->writeReg((uint8_t)(Register::CONTROL), &auxBuffer, 1)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_squareWave = squareWave_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     /////////////////    DATE HANDLING FUNCTIONS     /////////////////     //

/**
 * @brief       Reads the current date from device.
 * @details     The function reads the current date from device.
 * @param[out]  year_p      pointer to the variable where the year will be saved
 *                              in four digits format (2000-2099).
 * @param[out]  month_p     pointer to the variable where the month will be
 *                              saved (1-12).
 * @param[out]  monthDay_p  pointer to the variable where the day of the month
 *                              will be saved (1-31).
 * @param[out]  weekDay_p   pointer to the variable where the day of the week
 *                              will be saved (1-7).
 * @return true
 * @return false
*/
bool_t Ds1307::getDate(uint16_t *year_p, uint8_t *month_p, uint8_t *monthDay_p, uint8_t *weekDay_p)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::getDate(uint16_t *, uint8_t *, uint8_t *, uint8_t *)", DEBUG_CODE_DS1307);

    // Local variables
    uint16_t auxYear;
    DateTime::Month auxMonth;
    uint8_t auxMonthDay;
    DateTime::WeekDay auxWeekDay;

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // CHECK FOR ERROR - invalid pointers
    if ((!isPointerValid(year_p)) || (!isPointerValid(month_p)) || (!isPointerValid(monthDay_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Get last data from device
    if (!this->_getData()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Retrieve data
    if (isPointerValid(weekDay_p)) {
        if (!this->_dateTime.getDate(&auxYear, &auxMonth, &auxMonthDay, &auxWeekDay)) {
            // Returns error
            this->_lastError = this->_dateTime.getLastError();
            debugMessage(this->_lastError);
            return false;
        }
        *weekDay_p = (uint8_t)auxWeekDay;
    } else {
        if (!this->_dateTime.getDate(&auxYear, &auxMonth, &auxMonthDay)) {
            // Returns error
            this->_lastError = this->_dateTime.getLastError();
            debugMessage(this->_lastError);
            return false;
        }
    }
    *year_p = auxYear;
    *month_p = (uint8_t)(auxMonth);
    *monthDay_p = auxMonthDay;
//////////////////////////////////////// BEGIN TEST
#if defined(TEST)
    do {
        char auxStr[100];
        sprintf(auxStr, "\r%u, %u, %u, %u\r",
                auxYear, (uint8_t)auxMonth, auxMonthDay, (uint8_t)auxWeekDay
        );
        debug.printMessage(auxStr);
    } while (0);
#endif
//////////////////////////////////////// END TEST

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads the current date from device.
 * @details     The function reads the current date from device.
 * @param[out]  dateTime_p  pointer to \ref{DateTime} variable where all the
 *                              available data will ve saved.
 * @return true
 * @return false
*/
bool_t Ds1307::getDateTime(DateTime *dateTime_p)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::getDateTime(DateTime *)", DEBUG_CODE_DS1307);

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // CHECK FOR ERROR - invalid pointers
    if (!isPointerValid(dateTime_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Get last data from device
    if (!this->_getData()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Retrieve data
    *dateTime_p = this->_dateTime;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Set the date at the device.
 * @details     The function sets the date at the device.
 * @param[in]   year_p      variable containing the year (2000-2099).
 * @param[in]   month_p     variable containing the month (1-12).
 * @param[in]   monthDay_p  variable containing the day of the month (1-31).
 * @return true
 * @return false
*/
bool_t Ds1307::setDate(uint16_t year_p, uint8_t month_p, uint8_t monthDay_p)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::setDate(uint16_t, uint8_t, uint8_t)", DEBUG_CODE_DS1307);

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

//////////////////////////////////////// BEGIN TEST
#if defined(TEST)
    do {
        char auxStr[100];
        sprintf(auxStr, "\r%u, %u, %u\r",
                year_p, month_p, monthDay_p
        );
        debug.printMessage(auxStr);
    } while (0);
#endif
//////////////////////////////////////// END TEST

    // Update data members - Date is checked inside DateTime class
    if (!this->_dateTime.setDate(year_p, (DateTime::Month)month_p, monthDay_p)) {
        // Returns error
        this->_lastError = this->_dateTime.getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Send data to device
    if (!this->_sendData()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Set the date at the device.
 * @details     The function sets the date at the device.
 * @param[in]   dateTime_p  \ref{DateTime} object containing all the date and
 *                  time information to be set an the device.
 * @return true
 * @return false
*/
bool_t Ds1307::setDateTime(DateTime dateTime_p)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::setDateTime(DateTime)", DEBUG_CODE_DS1307);

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Checks DateTime parameter
    if (!dateTime_p.isDateSet()) {
        // Returns error
        this->_lastError = Error::DATETIME_DATE_NOT_INITIALIZED;
        debugMessage(Error::DATETIME_DATE_NOT_INITIALIZED);
        return false;
    }
    if (!dateTime_p.isTimeSet()) {
        // Returns error
        this->_lastError = Error::DATETIME_TIME_NOT_INITIALIZED;
        debugMessage(Error::DATETIME_TIME_NOT_INITIALIZED);
        return false;
    }

    // Update data members
    this->_dateTime = dateTime_p;

    // Sends data to device
    if (!this->_sendData()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     /////////////////    TIME HANDLING FUNCTIONS     /////////////////     //

/**
 * @brief       Reads the current time from the device.
 * @details     Reads the current time from the device.
 * @param[out]  hours_p     pointer to the variable where the hours must be
 *                  saved (0-23).
 * @param[out]  minutes_p   pointer to the variable where the minutes must be
 *                  saved (0-59).
 * @param[out]  seconds_p   pointer to the variable where the seconds must be
 *                  saved (0-59).
 * @param[out]  abbrev_p    pointer to the \ref{ds1307TimeAbbreviation_t}
 *                  variable where the time format must be saved.
 * @return true
 * @return false
*/
bool_t Ds1307::getTime(uint8_t *hours_p, uint8_t *minutes_p, uint8_t *seconds_p, DateTime::TimeFormat timeFormat_p,
        DateTime::AmPmFlag *amPmFlag_p)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::getTime(uint8_t *, uint8_t *, uint8_t *, DateTime::TimeFormat, DateTime::AmPmFlag *)"
            , DEBUG_CODE_DS1307);

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // CHECK FOR ERROR - invalid pointers
    if ((!isPointerValid(hours_p)) || (!isPointerValid(minutes_p)) || (!isPointerValid(seconds_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    if ((timeFormat_p == DateTime::TimeFormat::FORMAT_12_HOURS) && (!isPointerValid(amPmFlag_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Get last data from device
    if (!this->_getData()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Retrieve data
    if (!this->_dateTime.getTime(hours_p, minutes_p, seconds_p, timeFormat_p, amPmFlag_p)) {
        // Returns error
        this->_lastError = this->_dateTime.getLastError();
        debugMessage(this->_lastError);
        return false;
    }

//////////////////////////////////////// BEGIN TEST
#if defined(TEST)
    do {
        char auxStr[100];
        sprintf(auxStr, "\r%u, %u, %u, %u, %u\r",
                *hours_p, *minutes_p, *seconds_p, (uint8_t)timeFormat_p, (uint8_t)(*amPmFlag_p)
        );
        debug.printMessage(auxStr);
    } while (0);
#endif
//////////////////////////////////////// END TEST

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Set the time at the device.
 * @details     The function sets the time at the device.
 * @param[in]   hours_p     variable containing the hours (0-23).
 * @param[in]   minutes_p   variable containing the minutes (0-59).
 * @param[in]   seconds_p   variable containing the seconds (0-59).
 * @param[in]   timeFormat_p    \ref{DateTime::TimeFormat} variable containing
 * @param[in]   amPmFlag_p  \ref{DateTime::AmPmFlag} variable containing
 *                              the time format.
 * @return true
 * @return false
*/
bool_t Ds1307::setTime(uint8_t hours_p, uint8_t minutes_p, uint8_t seconds_p, DateTime::TimeFormat timeFormat_p,
        DateTime::AmPmFlag amPmFlag_p)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::setTime(uint8_t, uint8_t, uint8_t, DateTime::TimeFormat, DateTime::AmPmFlag)", DEBUG_CODE_DS1307);

//////////////////////////////////////// BEGIN TEST
#if defined(TEST)
    do {
        char auxStr[100];
        sprintf(auxStr, "\r%u, %u, %u, %u, %u\r",
                hours_p, minutes_p, seconds_p, (uint8_t)timeFormat_p, (uint8_t)amPmFlag_p
        );
        debug.printMessage(auxStr);
    } while (0);
#endif
//////////////////////////////////////// END TEST

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members - Time is checked inside DateTime class
    if (!this->_dateTime.setTime(hours_p, minutes_p, seconds_p, timeFormat_p, amPmFlag_p)) {
        // Returns error
        this->_lastError = this->_dateTime.getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Send data to device
    if (!this->_sendData()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ///////////////    RAM DATA HANDLING FUNCTIONS     ///////////////     //

/**
 * @brief       Reads the device internal RAM.
 * @details     Reads an amount of bytes from the device internal RAM.
 * @param[in]   position_p  RAM start position address to be read (0-55).
 * @param[out]  buffer_p    pointer to the location to store the data read.
 * @param[in]   size_p      number of bytes to read (0-55).
 * @return true
 * @return false
*/
bool_t Ds1307::getRamData(uint8_t position_p, uint8_t *buffer_p, uint8_t size_p)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::getRamData(uint8_t, uint8_t *, uint8_t)", DEBUG_CODE_DS1307);

    // CHECK FOR ERROR - invalid pointer
    if (!isPointerValid(buffer_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    // CHECK FOR ERROR - wrong number of bytes to receive
    if (size_p == 0) {
        // Returns error
        this->_lastError = Error::ARGUMENT_CANNOT_BE_ZERO;
        debugMessage(Error::ARGUMENT_CANNOT_BE_ZERO);
        return false;
    }
    if (size_p > ds1307RamSize) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
        debugMessage(Error::BUFFER_SIZE_TOO_LARGE);
        return false;
    }
    // CHECK FOR ERROR - position invalid
    if (position_p > (ds1307RamSize - 1)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        debugMessage(Error::ARGUMENT_VALUE_INVALID);
        return false;
    }
    // CHECK FOR ERROR - memory overlap
    if ((position_p + size_p) > ds1307RamSize) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
        debugMessage(Error::BUFFER_SIZE_TOO_LARGE);
        return false;
    }

    // Read RAM data
    if (!this->_busHandler->setDevice(ds1307DeviceAddress)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }
    position_p += (uint8_t)(Register::RAM_START);
    if (!this->_busHandler->readReg(position_p, buffer_p, size_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Writes at the device internal RAM.
 * @details     Writes an amount of bytes at the device internal RAM.
 * @param[in]   position_p  RAM start position address to be read (0-55).
 * @param[in]   buffer_p    pointer to the location containing the data do be
 *                              written.
 * @param[in]   size_p      number of bytes to write (0-55).
 * @return true
 * @return false
*/
bool_t Ds1307::setRamData(uint8_t position_p, uint8_t *buffer_p, uint8_t size_p)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::setRamData(uint8_t, uint8_t *, uint8_t)", DEBUG_CODE_DS1307);

    // CHECK FOR ERROR - invalid pointer
    if (!isPointerValid(buffer_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    // CHECK FOR ERROR - wrong number of bytes to receive
    if (size_p == 0) {
        // Returns error
        this->_lastError = Error::ARGUMENT_CANNOT_BE_ZERO;
        debugMessage(Error::ARGUMENT_CANNOT_BE_ZERO);
        return false;
    }
    if (size_p > ds1307RamSize) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
        debugMessage(Error::BUFFER_SIZE_TOO_LARGE);
        return false;
    }
    // CHECK FOR ERROR - position invalid
    if (position_p > (ds1307RamSize - 1)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        debugMessage(Error::ARGUMENT_VALUE_INVALID);
        return false;
    }
    // CHECK FOR ERROR - memory overlap
    if ((position_p + size_p) > ds1307RamSize) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
        debugMessage(Error::BUFFER_SIZE_TOO_LARGE);
        return false;
    }

    // Write RAM data
    if (!this->_busHandler->setDevice(ds1307DeviceAddress)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }
    position_p += (uint8_t)(Register::RAM_START);
    if (!this->_busHandler->writeReg(position_p, buffer_p, size_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

/**
 * @brief
 * @details
 */
void Ds1307::_clearData(void)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::_clearData(void)", DEBUG_CODE_DS1307);

    //     ////////////////    PERIPHERAL BUS HANDLER     ////////////////     //
    this->_busHandler                   = nullptr;
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    this->_countingHalted               = false;
    this->_initialized                  = false;
    this->_squareWave                   = SquareWave::OFF_LOW;
    this->_dateTime.setDate(
            2000,
            DateTime::Month::JANUARY,
            1
    );
    this->_dateTime.setTime(
            0,
            0,
            0
    );

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief
 * @details
 * @return true
 * @return false
 */
bool_t Ds1307::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::_isInitialized(void)", DEBUG_CODE_DS1307);

    // CHECK FOR ERROR - peripheral not initialized
    if (!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 * @details
 * @return true
 * @return false
 */
bool_t Ds1307::_getData(void)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::_getData(void)", DEBUG_CODE_DS1307);

    // Local variables
    uint8_t auxBuffer[7];
    uint8_t auxSeconds;
    uint8_t auxMinutes;
    uint8_t auxHours;
    uint16_t auxYear;
    uint8_t auxMonth;
    uint8_t auxMonthDay;
    DateTime::TimeFormat auxTimeFormat;
    DateTime::AmPmFlag auxAmPmFlag;

    // Reads data from device
    if (!this->_busHandler->setDevice(ds1307DeviceAddress)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }
    if (!this->_busHandler->readReg((uint8_t)(Register::SECONDS), auxBuffer, 7)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }

//////////////////////////////////////// BEGIN TEST
#if defined(TEST)
    do {
        char auxStr[100];
        sprintf(auxStr, "\r%02x, %02x, %02x, %02x, %02x, %02x, %02x\r",
                auxBuffer[0], auxBuffer[1], auxBuffer[2],
                auxBuffer[3], auxBuffer[4], auxBuffer[5], auxBuffer[6]
        );
        debug.printMessage(auxStr);
    } while (0);
#endif
//////////////////////////////////////// BEGIN TEST

    // Decodes the information
    this->_countingHalted = isBitSet(auxBuffer[0], 7);
    auxBuffer[0] &= 0x7F;
    auxSeconds = ((auxBuffer[0] >> 4) * 10) + (auxBuffer[0] & 0x0F);
    auxMinutes = ((auxBuffer[1] >> 4) * 10) + (auxBuffer[1] & 0x0F);
    if (isBitSet(auxBuffer[2], 6)) { // 12 hour-mode
        auxHours = (((auxBuffer[2] & 0x10) >> 4) * 10) + (auxBuffer[2] & 0x0F);
        auxTimeFormat = DateTime::TimeFormat::FORMAT_12_HOURS;
        auxAmPmFlag = (isBitSet(auxBuffer[2], 5)) ? DateTime::AmPmFlag::PM : DateTime::AmPmFlag::AM;
    } else {                        // 24 hour-mode
        auxHours = (((auxBuffer[2] & 0x30) >> 4) * 10) + (auxBuffer[2] & 0x0F);
        auxTimeFormat = DateTime::TimeFormat::FORMAT_24_HOURS;
        auxAmPmFlag = DateTime::AmPmFlag::AM;
    }
    auxMonthDay = ((auxBuffer[4] >> 4) * 10) + (auxBuffer[4] & 0x0F);
    auxMonth = ((auxBuffer[5] >> 4) * 10) + (auxBuffer[5] & 0x0F);
    auxYear = ((auxBuffer[6] >> 4) * 10) + (auxBuffer[6] & 0x0F);
    auxYear += 2000;

//////////////////////////////////////// BEGIN TEST
#if defined(TEST)
    do {
        char auxStr[100];
        sprintf(auxStr, "\r%u, %u, %u, %u, %u, %u\r",
                auxHours, auxMinutes, auxSeconds,
                auxYear, auxMonth, auxMonthDay
        );
        debug.printMessage(auxStr);
    } while (0);
#endif
//////////////////////////////////////// BEGIN TEST

    // Update data members
    if (!this->_dateTime.setTime(auxHours, auxMinutes, auxSeconds, auxTimeFormat, auxAmPmFlag)) {
        // Returns error
        this->_lastError = this->_dateTime.getLastError();
        debugMessage(this->_lastError);
        return false;
    }
    if (!this->_dateTime.setDate(auxYear, (DateTime::Month)auxMonth, auxMonthDay)) {
        // Returns error
        this->_lastError = this->_dateTime.getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 * @details
 * @return true
 * @return false
 */
bool_t Ds1307::_sendData(void)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::_sendData(void)", DEBUG_CODE_DS1307);

    // Local variables
    uint8_t auxBuffer[7];
    uint8_t auxSeconds;
    uint8_t auxMinutes;
    uint8_t auxHours;
    DateTime::WeekDay auxWeekDay;
    uint8_t auxMonthDay;
    DateTime::Month auxMonth;
    uint16_t auxYear;
    DateTime::TimeFormat auxTimeFormat;
    DateTime::AmPmFlag auxAmPmFlag;

    // Gets the data from class members
    if (!this->_dateTime.getTimeFormat(&auxTimeFormat)) {
        // Returns error
        this->_lastError = this->_dateTime.getLastError();
        debugMessage(this->_lastError);
        return false;
    }
    if (!this->_dateTime.getTime(&auxHours, &auxMinutes, &auxSeconds, auxTimeFormat, &auxAmPmFlag)) {
        // Returns error
        this->_lastError = this->_dateTime.getLastError();
        debugMessage(this->_lastError);
        return false;
    }
    if (!this->_dateTime.getDate(&auxYear, &auxMonth, &auxMonthDay, &auxWeekDay)) {
        // Returns error
        this->_lastError = this->_dateTime.getLastError();
        debugMessage(this->_lastError);
        return false;
    }

//////////////////////////////////////// BEGIN TEST
#if defined(TEST)
    do {
        char auxStr[100];
        sprintf(auxStr, "\r%u, %u, %u, %u, %u, %u, %u, %u, %u\r",
                auxHours, auxMinutes, auxSeconds, (uint8_t)auxTimeFormat, (uint8_t)auxAmPmFlag,
                auxYear, (uint8_t)auxMonth, auxMonthDay, (uint8_t)auxWeekDay
        );
        debug.printMessage(auxStr);
    } while (0);
#endif
//////////////////////////////////////// BEGIN TEST

    if (auxYear > 1999) {
        auxYear -= 2000;
    }

    // Encode the information
    auxBuffer[0] = ((auxSeconds / 10) << 4) | (auxSeconds % 10);
    auxBuffer[0] |= this->_countingHalted << 7;
    auxBuffer[1] = ((auxMinutes / 10) << 4) | (auxMinutes % 10);
    auxBuffer[2] = ((auxHours / 10) << 4) | (auxHours % 10);
    if (auxTimeFormat == DateTime::TimeFormat::FORMAT_12_HOURS) {
        auxBuffer[2] |= 0x40;
        auxBuffer[2] |= ((auxAmPmFlag == DateTime::AmPmFlag::PM) ? (1 << 5) : 0);
    }
    auxBuffer[3] = (uint8_t)(auxWeekDay);
    auxBuffer[4] = ((auxMonthDay / 10) << 4) | (auxMonthDay % 10);
    auxBuffer[5] = (((uint8_t)(auxMonth) / 10) << 4) | ((uint8_t)(auxMonth) % 10);
    auxBuffer[6] = ((auxYear / 10) << 4) | (auxYear % 10);

//////////////////////////////////////// BEGIN TEST
#if defined(TEST)
    do {
        char auxStr[100];
        sprintf(auxStr, "\r%02x, %02x, %02x, %02x, %02x, %02x, %02x\r",
                auxBuffer[0], auxBuffer[1], auxBuffer[2],
                auxBuffer[3], auxBuffer[4], auxBuffer[5], auxBuffer[6]
        );
        debug.printMessage(auxStr);
    } while (0);
#endif
//////////////////////////////////////// BEGIN TEST

    // Writes data at device
    if (!this->_busHandler->setDevice(ds1307DeviceAddress)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }
    if (!this->_busHandler->writeReg((uint8_t)(Register::SECONDS), auxBuffer, 7)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 * @details
 * @param[in]
 * @return true
 * @return false
 */
bool_t Ds1307::_setCounting(bool counting_p)
{
    // Mark passage for debugging purpose
    debugMark("Ds1307::_setCounting(bool)", DEBUG_CODE_DS1307);

    // Local variables
    uint8_t auxBuffer;
    bool isCounting;

    // Read register data from device
    if (!this->_busHandler->setDevice(ds1307DeviceAddress)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }
    if (!this->_busHandler->readReg((uint8_t)(Register::SECONDS), &auxBuffer, 1)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }
    isCounting = isBitClr(auxBuffer, 7);

    // Update buffer
    if (isCounting != counting_p) {
        if (counting_p) {
            clrBit(auxBuffer, 7);
        } else {
            setBit(auxBuffer, 7);
        }
        // Writes data at device
        if (!this->_busHandler->setDevice(ds1307DeviceAddress)) {
            // Returns error
            this->_lastError = this->_busHandler->getLastError();
            debugMessage(this->_lastError);
            return false;
        }
        if (!this->_busHandler->writeReg((uint8_t)(Register::SECONDS), &auxBuffer, 1)) {
            // Returns error
            this->_lastError = this->_busHandler->getLastError();
            debugMessage(this->_lastError);
            return false;
        }
    }

    // Update data members
    this->_countingHalted = !counting_p;

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
