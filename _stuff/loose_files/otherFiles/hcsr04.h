#ifndef HCSR04_H_
#define HCSR04_H_

#include "global.h"

#define HCSR04_PORT	PORTC
#define HCSR04_PIN	PINC
#define HCSR04_TRIG	(1 << PORT2)
#define HCSR04_ECHO	(1 << PORT3)

uint32_t hcsr04GetTick();

#endif /* HCSR04_H_ */