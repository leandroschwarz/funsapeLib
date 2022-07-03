
#include "main.h"

#ifndef __SETUP_H
#define __SETUP_H

void SystemClock_Config(void);
void HAL_SystemInit(void);
void Error_Handler(void);
void assert_failed(char *file, uint32_t line);

#endif
