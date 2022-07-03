


#include "mainDefs.h"
#include "usart.h"
#include "rtc.h"
#include "..\gpdse\globalDefines.hpp"
#include "..\gpdse\util\dateTime.hpp"

void rtcSetDateTime(DateTime * timestamp);
void uart2SafeSend(uint8_t * buffer, uint16_t size);
void myErrorHandler(void);


extern volatile systemFlags_f systemFlags;
extern vuint8_t txBuffer[50];
