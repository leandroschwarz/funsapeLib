

#ifndef __MY_LIB_GLOBAL_DEFINITIONS_H
#define __MY_LIB_GLOBAL_DEFINITIONS_H 1

#include "stm32l0xx.h"

#define setBit(reg, bit)						((reg) |= (1 << (bit)))
#define clrBit(reg, bit)						((reg) &= ~(1 << (bit)))
#define cplBit(reg, bit)						((reg) ^= (1 << (bit)))
#define isBitSet(reg, bit)						(((reg) >> (bit)) & 1)
#define isBitClr(reg, bit)						(!(((reg) >> (bit)) & 1))
#define waitUntilBitIsSet(reg, bit)				do {} while(isBitClr((reg), (bit)))
#define waitUntilBitIsClear(reg, bit)			do {} while(isBitSet((reg), (bit)))
#define setMask(reg, bitMask, offsetMask)		((reg) |= ((bitMask) << (offsetMask)))
#define clrMask(reg, bitMask, offsetMask)		((reg) &= ~((bitMask) << (offsetMask)))
#define cplMask(reg, bitMask, offsetMask)		((reg) ^= ((bitMask) << (offsetMask)))
#define systemHalt()							while(1) {}
#define minimumOf(var1 , var2)					((var1 < var2) ? var1 :var2)
#define maximumOf(var1 , var2)					((var1 > var2) ? var1 :var2
#define truncateBetween(var, minimum, maximum)	(var < minimum) ? minimum : ((var > maximum) ? maximum : var)
#define stringifyName(constant)					#constant
#define stringifyValue(constant)				stringifyName(constant)

#define rtcDecodeYearTens(reg)					((reg & RTC_DR_YT_Msk) >> RTC_DR_YT_Pos)
#define rtcDecodeYearUnits(reg)					((reg & RTC_DR_YU_Msk) >> RTC_DR_YU_Pos)
#define rtcDecodeMonthTens(reg)					((reg & RTC_DR_MT_Msk) >> RTC_DR_MT_Pos)
#define rtcDecodeMonthUnits(reg)				((reg & RTC_DR_MU_Msk) >> RTC_DR_MU_Pos)
#define rtcDecodeDateTens(reg)					((reg & RTC_DR_DT_Msk) >> RTC_DR_DT_Pos)
#define rtcDecodeDateUnits(reg)					((reg & RTC_DR_DU_Msk) >> RTC_DR_DU_Pos)
#define rtcDecodeWeekDay(reg)					((reg & RTC_DR_WDU_Msk) >> RTC_DR_WDU_Pos)
#define rtcDecodeHourTens(reg)					((reg & RTC_TR_HT_Msk) >> RTC_TR_HT_Pos)
#define rtcDecodeHourUnits(reg)					((reg & RTC_TR_HU_Msk) >> RTC_TR_HU_Pos)
#define rtcDecodeMinuteTens(reg)				((reg & RTC_TR_MNT_Msk) >> RTC_TR_MNT_Pos)
#define rtcDecodeMinuteUnits(reg)				((reg & RTC_TR_MNU_Msk) >> RTC_TR_MNU_Pos)
#define rtcDecodeSecondTens(reg)				((reg & RTC_TR_ST_Msk) >> RTC_TR_ST_Pos)
#define rtcDecodeSecondUnits(reg)				((reg & RTC_TR_SU_Msk) >> RTC_TR_SU_Pos)
#define rtcDecodeSubSecond(reg)					((reg & RTC_SSR_SS_Msk) >> RTC_SSR_SS_Pos)

#define rtcEncodeYearTens(reg)					((reg << RTC_DR_YT_Pos) & RTC_DR_YT_Msk)
#define rtcEncodeYearUnits(reg)					((reg << RTC_DR_YU_Pos) & RTC_DR_YU_Msk)
#define rtcEncodeMonthTens(reg)					((reg << RTC_DR_MT_Pos) & RTC_DR_MT_Msk)
#define rtcEncodeMonthUnits(reg)				((reg << RTC_DR_MU_Pos) & RTC_DR_MU_Msk)
#define rtcEncodeDateTens(reg)					((reg << RTC_DR_DT_Pos) & RTC_DR_DT_Msk)
#define rtcEncodeDateUnits(reg)					((reg << RTC_DR_DU_Pos) & RTC_DR_DU_Msk)
#define rtcEncodeWeekDay(reg)					((reg << RTC_DR_WDU_Pos) & RTC_DR_WDU_Msk)
#define rtcEncodeHourTens(reg)					((reg << RTC_TR_HT_Pos) & RTC_TR_HT_Msk)
#define rtcEncodeHourUnits(reg)					((reg << RTC_TR_HU_Pos) & RTC_TR_HU_Msk)
#define rtcEncodeMinuteTens(reg)				((reg << RTC_TR_MNT_Pos) & RTC_TR_MNT_Msk)
#define rtcEncodeMinuteUnits(reg)				((reg << RTC_TR_MNU_Pos) & RTC_TR_MNU_Msk)
#define rtcEncodeSecondTens(reg)				((reg << RTC_TR_ST_Pos) & RTC_TR_ST_Msk)
#define rtcEncodeSecondUnits(reg)				((reg << RTC_TR_SU_Pos) & RTC_TR_SU_Msk)


typedef unsigned int uint32_t;

typedef enum bool_t {
	FALSE	= 0,
	TRUE	= 1
} bool_t;

typedef enum weekDay_e {
	WEEKDAY_UNDEFINED	= 0,
	WEEKDAY_MONDAY		= 1,
	WEEKDAY_TUESDAY		= 2,
	WEEKDAY_WEDNESDAY	= 3,
	WEEKDAY_THURSDAY	= 4,
	WEEKDAY_FRIDAY		= 5,
	WEEKDAY_SATURDAY	= 6,
	WEEKDAY_SUNDAY		= 7
} weekDay_e;

typedef enum month_e {
	MONTH_UNDEFINED		= 0,
	MONTH_JANUARY		= 1,
	MONTH_FEBRUARY		= 2,
	MONTH_MARCH			= 3,
	MONTH_APRIL			= 4,
	MONTH_MAY			= 5,
	MONTH_JUNE			= 6,
	MONTH_JULY			= 7,
	MONTH_AUGUST		= 8,
	MONTH_SEPTEMBER		= 9,
	MONTH_OCTOBER		= 10,
	MONTH_NOVEMBER		= 11,
	MONTH_DECEMBER		= 12
} month_e;

// This structure works in 24-hours time format
typedef struct dateTime_s {
	uint32_t	year		: 7;
	month_e		month		: 4;
	uint32_t	date		: 5;
	weekDay_e	weekDay		: 3;
	uint32_t	hour		: 5;
	uint32_t	minute		: 6;
	uint32_t	second		: 6;
	uint32_t	subSecond	: 16;
	uint32_t	unusedBits	: 12;
	// 64 bits wide struct
} dateTime_s;

typedef enum myLibErrors_e
{
	// Defeault success value
	MYLIB_ERROR_NONE = 0,

	// Circular Buffer Classes
	MYLIB_ERROR_CIRCULAR_BUFFER_NOT_INITIALIZED,
	MYLIB_ERROR_CIRCULAR_BUFFER_SIZE_ZERO,
	MYLIB_ERROR_CIRCULAR_BUFFER_MEM_ALLOC_FAIL,
	MYLIB_ERROR_CIRCULAR_BUFFER_TRY_TO_WRITE_WHEN_FULL,
	MYLIB_ERROR_CIRCULAR_BUFFER_TRY_TO_READ_WHEN_EMPTY,
} myLibErrors_e;

typedef enum myLibAtParserState_e {
	MYLIB_AT_PARSER_WAITING		= 0,
	MYLIB_AT_PARSER_READY			= 1,
} myLibAtParserState_e;


#endif
