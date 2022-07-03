
// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1001
#    error [globalDefines.cpp] Error 101 - Build mismatch on header and source code files.
#endif


// -----------------------------------------------------------------------------
// Global Variables ------------------------------------------------------------

// -----------------------------------------------------------------------------
// Public Functions Definitions ------------------------------------------------


/////////////////////////// CLASS METHODS

SystemStatus::SystemStatus(uint32_t mainClock)
{
	this->mainSourceClock_ = mainClock;
	this->cpuClock_ = mainClock;
	this->lastError_ = ERROR_NONE;
	this->stopWatchMark_ = 0;
	this->stopWatchHalted_ = FALSE;
}

uint32_t SystemStatus::getCpuClock(void)
{
	return this->cpuClock_;
}

error_e SystemStatus::getLastError(void)
{
	return this->lastError_;
}

void SystemStatus::announceError(error_e error)
{
	this->lastError_ = error;
}

uint32_t SystemStatus::readStopWatch(void)
{
	return this->stopWatchValue_;
}

void SystemStatus::resumeStopWatch(void)
{
	this->stopWatchHalted_ = FALSE;
}

void SystemStatus::pauseStopWatch(void)
{
	this->stopWatchHalted_ = TRUE;

}

void SystemStatus::resetStopWatch(void)
{
	this->stopWatchValue_ = 0;
	this->stopWatchMark_ = 0;
}

void SystemStatus::setStopWatchMark(void)
{
	this->stopWatchMark_ = this->stopWatchValue_;

}
uint32_t SystemStatus::calculateElapsedTime(bool_t setNewMark)
{
	uint32_t start = this->stopWatchMark_;
	uint32_t current = this->stopWatchValue_;
	uint32_t elapsed = 0;

	elapsed = (current > start) ? (current - start) : (((0xFFFFFFFF - start)  + current) + 1);
	if (setNewMark) {
		this->stopWatchMark_ = current;
	}

	return elapsed;
}

uint32_t SystemStatus::calculateElapsedTime(uint32_t customMark)
{
	uint32_t current = this->stopWatchValue_;
	uint32_t elapsed = 0;

	elapsed = (current > customMark) ? (current - customMark) : (((0xFFFFFFFF - customMark)  + current) + 1);

	return elapsed;
}

void SystemStatus::incrementStopWatch(void)
{
	this->stopWatchValue_++;
}



/////////////////DateTime class

DateTime::DateTime()
{
	// Default date time
	this->year_ = 2002;
	this->month_ = MONTH_FEBRUARY;
	this->day_ = 18;
	this->weekDay_ = WEEKDAY_MONDAY;
	this->hour_ = 14;
	this->minute_ = 23;
	this->second_ = 56;
	this->millisecond_ = 123;
	this->leapYear_ = isLeapYear(2002);
	this->summerTime_ = FALSE;
	this->timeZone_ = UTC_MINUS_0300;
	this->unusedBits_ = 0;
}


void DateTime::getDate(uint8_t *day, month_t *month, uint16_t *year)
{
	if (year != NULL) {
		*year = this->year_;
	}
	if (month != NULL) {
		*month = this->month_;
	}
	*day = this->day_;
}


bool_t DateTime::setDate(uint8_t day, month_t month, uint16_t year)
{
	bool_t auxBool = FALSE;

	// Check for errors - month and day
	if (day == 0) {
		return FALSE;
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
			return FALSE;
		}
		break;
	case MONTH_APRIL:
	case MONTH_JUNE:
	case MONTH_SEPTEMBER:
	case MONTH_NOVEMBER:
		if (day > 30) {
			return FALSE;
		}
		break;
	case MONTH_FEBRUARY:
		if (day > 29) {
			return FALSE;
		}
		if (day == 29) {
			auxBool = this->isLeapYear(year);
			if (!auxBool) {
				return FALSE;
			}
		}
		break;
	default:
		return FALSE;
	}

	// Update data members
	this->year_ = year;
	this->month_ = month;
	this->day_ = day;
	this->leapYear_ = auxBool;
	this->evaluateDayOfWeek();

	return TRUE;
}

bool_t DateTime::setTime(uint8_t hour, uint8_t minute, uint8_t second, utcTimeZone_e timeZone)
{
	return this->setTime(hour, minute, second, 0, timeZone);
}

bool_t DateTime::setTime(uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond, utcTimeZone_e timeZone)
{
	// Check for errors - hour
	if (hour > 23) {
		return FALSE;
	}
	// Check for errors - minute
	if (minute > 59) {
		return FALSE;
	}
	// Check for errors - second
	if (second > 59) {
		return FALSE;
	}
	// Check for errors - millisecond
	if (millisecond > 999) {
		return FALSE;
	}
	// Check for errors - timeZone
	if ((timeZone == UTC_UNDEFINED) || (timeZone > UTC_OUT_OF_RANGE)) {
		return FALSE;
	}

	// Update data members
	this->hour_ = hour;
	this->minute_ = minute;
	this->second_ = second;
	this->millisecond_ = millisecond;
	this->timeZone_ = timeZone;

	return TRUE;
}



















bool_t DateTime::isLeapYear(uint16_t year)
{
	if ((year % 4) != 0) {
		return FALSE;
	}

	if ((year % 400) == 0) {
		return TRUE;
	}

	if ((year % 100) == 0) {
		return FALSE;
	}

	return TRUE;
}

void DateTime::evaluateDayOfWeek(void)
{
	uint16_t aux16;
	uint16_t year;

	year = this->year_;
	year -= (this->month_ < 3);
	aux16 = (year + year / 4 - year / 100 + year / 400 + "-bed=pen+mad."[this->month_] + this->day_) % 7;
	aux16++;

	this->weekDay_ = (weekDay_t)aux16;
}
