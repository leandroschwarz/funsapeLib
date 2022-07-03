

#ifndef __MY_DEFINES_H
#define __MY_DEFINES_H

#include "stm32f1xx.h"

#define PROGMEM
#define PGM_P							const char *
#define pgm_read_byte(addr)				(*(const unsigned char *)(addr))
#define pgm_read_word(addr)				(*(const unsigned short *)(addr))
#define pgm_read_dword(addr)			(*(const unsigned long *)(addr))
#define strcpy_P						strcpy
#define strlen_P						strlen
#define delay(x)						HAL_Delay(x)
#define _BV(bit)						(1 << (bit))

#define ledOn()							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)
#define ledOff()						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)

#endif
