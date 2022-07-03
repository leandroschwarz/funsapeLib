

#ifndef __GPDSEPP_STM32_RTC_H
#define __GPDSEPP_STM32_RTC_H

#include "globalDefinitionsSTM32.h"




class gpdseRtc_c
{

	typedef struct gpdseRtcDateTime_s {
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
	} gpdseRtcDateTime_s;


public:
	gpdseRtc_c();

private:
	gpdseRtcDateTime_s	dateTime;





};

#endif
