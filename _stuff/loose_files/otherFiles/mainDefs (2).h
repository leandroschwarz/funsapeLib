


#ifndef __MAIN_DEFS_H
#define __MAIN_DEFS_F

#include <stdint.h>

typedef union systemFlags_f {
	struct {
		uint32_t	usart2TxBusy	: 1;
		uint32_t	unusedBits		: 31;
	};
	uint32_t allFlags;
} systemFlags_f;

#endif
