

#ifndef __FUNCTIONS_HPP
#define __FUNCTIONS_HPP

#include "main.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

#include "..\FunSAPE\funsapeLibGlobalDefines.hpp"
#include "..\FunSAPE\util\funsapeLibCircularBuffer8.hpp"
#include "..\FunSAPE\util\funsapeLibNmeaParser.hpp"


void uart1ProcessReception(bool isFullCompleteCallback);
void printStatus(void);

extern CircularBuffer8 uart1RxCircBuff;
extern NmeaParser gpsParser;



#endif
