

#ifndef __GLOBAL_DEFINITIONS_STM32_H
#define __GLOBAL_DEFINITIONS_STM32_H

#include "stm32l0xx.h"
/*
#define RTC_PM_BIT			22
#define RTC_PM_OFFSET		(1 << RTC_PM_BIT)
#define RTC_PM_MASK			0x00000001
#define RTC_HT_BIT			20
#define RTC_HT_OFFSET		(1 << RTC_HT_BIT)
#define RTC_HT_MASK			0x00000003
#define RTC_HU_BIT			16
#define RTC_HU_OFFSET		(1 << RTC_HU_BIT)
#define RTC_HU_MASK			0x0000000F
#define RTC_MNT_BIT			12
#define RTC_MNT_OFFSET		(1 << RTC_MNT_BIT)
#define RTC_MNT_MASK		0x00000007
#define RTC_MNU_BIT			8
#define RTC_MNU_OFFSET		(1 << RTC_MNU_BIT)
#define RTC_MNU_MASK		0x0000000F
#define RTC_ST_BIT			4
#define RTC_ST_OFFSET		(1 << RTC_ST_BIT)
#define RTC_ST_MASK			0x00000007
#define RTC_SU_BIT			0
#define RTC_SU_OFFSET		(1 << RTC_SU_BIT)
#define RTC_SU_MASK			0x0000000F

#define RTC_YT_BIT			20
#define RTC_YT_OFFSET		(1 << RTC_YT_BIT)
#define RTC_YT_MASK			0x0000000F
#define RTC_YU_BIT			16
#define RTC_YU_OFFSET		(1 << RTC_YU_BIT)
#define RTC_YU_MASK			0x0000000F
#define RTC_WDU_BIT			13
#define RTC_WDU_OFFSET		(1 << RTC_WDU_BIT)
#define RTC_WDU_MASK		0x00000007
#define RTC_MT_BIT			12
#define RTC_MT_OFFSET		(1 << RTC_MT_BIT)
#define RTC_MT_MASK			0x00000001
#define RTC_MU_BIT			8
#define RTC_MU_OFFSET		(1 << RTC_MU_BIT)
#define RTC_MU_MASK			0x0000000F
#define RTC_DT_BIT			4
#define RTC_DT_OFFSET		(1 << RTC_DT_BIT)
#define RTC_DT_MASK			0x00000003
#define RTC_DU_BIT			0
#define RTC_DU_OFFSET		(1 << RTC_DU_BIT)
#define RTC_DU_MASK			0x0000000F
*/

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


#endif
