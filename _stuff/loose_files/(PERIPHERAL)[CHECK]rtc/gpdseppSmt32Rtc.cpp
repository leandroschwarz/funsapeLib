

#include "gpdseppSmt32Rtc.h"



gpdseRtc_c::gpdseRtc_c()
{
	this->dateTime.year = 0;
}








bool_t gpdseRtc_c::setDateTime(dateTime_s dateTimeStruct)
{
	uint32_t auxTime = 0;
	uint32_t auxDate = 0;

	// Check data validity
	if (dateTimeStruct.month == 0) {
		return FALSE;
	}
	if (dateTimeStruct.date == 0) {
		return FALSE;
	}
	if (dateTimeStruct.weekDay == 0) {
		return FALSE;
	}

	// Time variable
	auxTime = rtcEncodeHourTens(dateTimeStruct.hour % 10) |
	          rtcEncodeHourUnits(dateTimeStruct.hour / 10) |
	          rtcEncodeMinuteTens(dateTimeStruct.minute % 10) |
	          rtcEncodeMinuteUnits(dateTimeStruct.minute / 10) |
	          rtcEncodeSecondTens(dateTimeStruct.second % 10) |
	          rtcEncodeSecondUnits(dateTimeStruct.second / 10);

	// Date variable
	auxDate = rtcEncodeYearTens(dateTimeStruct.hour % 10) |
	          rtcEncodeYearUnits(dateTimeStruct.hour / 10) |
	          rtcEncodeMonthTens(dateTimeStruct.month % 10) |
	          rtcEncodeMonthUnits(dateTimeStruct.month / 10) |
	          rtcEncodeDateTens(dateTimeStruct.date % 10) |
	          rtcEncodeDateUnits(dateTimeStruct.date / 10) |
	          rtcEncodeWeekDay(dateTimeStruct.weekDay);

	// Set registers
	rtcUnblockRegisters();
	RTC->TR = auxTime;
	RTC->DR = auxDate;
	rtcBlockRegisters();

	return TRUE;
}


bool_t rtcGetDateTime(dateTime_s * dateTimeStruct)
{
	uint32_t aux32;

	// Get RTC time register
	aux32 = RTC->TR;
	dateTimeStruct->hour = 10 * rtcDecodeHourTens(aux32) + rtcDecodeHourUnits(aux32);
	dateTimeStruct->minute = 10 * rtcDecodeMinuteTens(aux32) + rtcDecodeMinuteUnits(aux32);
	dateTimeStruct->second = 10 * rtcDecodeSecondTens(aux32) + rtcDecodeSecondUnits(aux32);
	// AM_PM (convert to 24 hours if needed)
	// Need to test that

	// Get RTC date register
	aux32 = RTC->DR;
	dateTimeStruct->year = 10 * rtcDecodeYearTens(aux32) + rtcDecodeYearUnits(aux32);
	dateTimeStruct->month = 10 * rtcDecodeMonthTens(aux32) + rtcDecodeMonthUnits(aux32);
	dateTimeStruct->date = 10 * rtcDecodeDateTens(aux32) + rtcDecodeDateUnits(aux32);
	dateTimeStruct->weekDay = rtcDecodeWeekDay(aux32);

	// Get RTC sub second register
	aux32 = RTC->SSR;
	dateTimeStruct->subSecond = rtcDecodeSubSecond(aux32);

	// Check data validity
	if (dateTimeStruct->month == 0) {
		return FALSE;
	}
	if (dateTimeStruct->date == 0) {
		return FALSE;
	}
	if (dateTimeStruct->weekDay == 0) {
		return FALSE;
	}

	return TRUE;
}


// PRIVATE

void inline gpdseRtc_c::rtcUnblockRegisters()
{
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
}

void inline rtcBlockRegisters()
{
	RTC->WPR = 0xFE;
	RTC->WPR = 0x64;
}
