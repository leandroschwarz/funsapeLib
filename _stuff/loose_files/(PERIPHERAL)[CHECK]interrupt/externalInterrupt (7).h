/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			externalInterrupt.h
 * Module:			External Interrupt interface
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __EXTERNAL_INTERRUPT_H
#define __EXTERNAL_INTERRUPT_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum intSenseMode_t {
	SENSE_LOW_LEVEL = 0,
	SENSE_ANY_EDGE,
	SENSE_FALLING_EDGE,
	SENSE_RISING_EDGE,
	SENSE_NO_CHANGE = 255
} intSenseMode_t;

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

// INTs
resultValue_t	int1Config(portMode_t port, intSenseMode_t sense);
resultValue_t	int0Config(portMode_t port, intSenseMode_t sense);
void			int1ActivateInterrupt(void);
void			int0ActivateInterrupt(void);
void			int1DeactivateInterrupt(void);
void			int0DeactivateInterrupt(void);
void			int1ClearInterruptRequest(void);
void			int0ClearInterruptRequest(void);

// PCINTs
void			pcint23_16Enable(void);
void			pcint14_8Enable(void);
void			pcint7_0Enable(void);
void			pcint23_16Disable(void);
void			pcint14_8Disable(void);
void			pcint7_0Disable(void);
void			pcint23_16ClearInterruptRequest(void);
void			pcint14_8ClearInterruptRequest(void);
void			pcint7_0ClearInterruptRequest(void);
resultValue_t	pcint23ActivateInterrupt(portMode_t port);
resultValue_t	pcint22ActivateInterrupt(portMode_t port);
resultValue_t	pcint21ActivateInterrupt(portMode_t port);
resultValue_t	pcint20ActivateInterrupt(portMode_t port);
resultValue_t	pcint19ActivateInterrupt(portMode_t port);
resultValue_t	pcint18ActivateInterrupt(portMode_t port);
resultValue_t	pcint17ActivateInterrupt(portMode_t port);
resultValue_t	pcint16ActivateInterrupt(portMode_t port);
resultValue_t	pcint14ActivateInterrupt(portMode_t port);
resultValue_t	pcint13ActivateInterrupt(portMode_t port);
resultValue_t	pcint12ActivateInterrupt(portMode_t port);
resultValue_t	pcint11ActivateInterrupt(portMode_t port);
resultValue_t	pcint10ActivateInterrupt(portMode_t port);
resultValue_t	pcint9ActivateInterrupt(portMode_t port);
resultValue_t	pcint8ActivateInterrupt(portMode_t port);
resultValue_t	pcint7ActivateInterrupt(portMode_t port);
resultValue_t	pcint6ActivateInterrupt(portMode_t port);
resultValue_t	pcint5ActivateInterrupt(portMode_t port);
resultValue_t	pcint4ActivateInterrupt(portMode_t port);
resultValue_t	pcint3ActivateInterrupt(portMode_t port);
resultValue_t	pcint2ActivateInterrupt(portMode_t port);
resultValue_t	pcint1ActivateInterrupt(portMode_t port);
resultValue_t	pcint0ActivateInterrupt(portMode_t port);
void			pcint23DeactivateInterrupt(void);
void			pcint22DeactivateInterrupt(void);
void			pcint21DeactivateInterrupt(void);
void			pcint20DeactivateInterrupt(void);
void			pcint19DeactivateInterrupt(void);
void			pcint18DeactivateInterrupt(void);
void			pcint17DeactivateInterrupt(void);
void			pcint16DeactivateInterrupt(void);
void			pcint14DeactivateInterrupt(void);
void			pcint13DeactivateInterrupt(void);
void			pcint12DeactivateInterrupt(void);
void			pcint11DeactivateInterrupt(void);
void			pcint10DeactivateInterrupt(void);
void			pcint9DeactivateInterrupt(void);
void			pcint8DeactivateInterrupt(void);
void			pcint7DeactivateInterrupt(void);
void			pcint6DeactivateInterrupt(void);
void			pcint5DeactivateInterrupt(void);
void			pcint4DeactivateInterrupt(void);
void			pcint3DeactivateInterrupt(void);
void			pcint2DeactivateInterrupt(void);
void			pcint1DeactivateInterrupt(void);
void			pcint0DeactivateInterrupt(void);

#endif
