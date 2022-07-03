



#include "mainDefs.h"
#include "..\gpdse\globalDefines.hpp"
#include "..\gpdse\util\dateTime.hpp"




// New data types
typedef enum rtcAlarmSel {
	ALARM_NONE	= 0,
	ALARM_A,
	ALARM_B,
	ALARM_ALL
} rtcAlarmSel;

// External global variables
extern volatile systemFlags_f systemFlags;
extern vuint8_t txBuffer[200];

// System functions
void SystemClock_Config(void);
void initHalLibrary(void);
void myErrorHandler(error_e errorCode);

// RTC functions
void rtcInit(void);
void rtcSetDateTime(DateTime * timestamp);
void rtcSetAlarm(DateTime * timestamp, rtcAlarmSel alarm);

// UART functions
void uartSafeSend(UART_HandleTypeDef * uartHandle, uint8_t * buffer, uint16_t size);
