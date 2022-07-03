/* -----------------------------------------------------------------------------
 * Project:			Stepper Motor Controller
 * File:			stepperMotor.c
 * Author:			Leandro Schwarz
 * Version:			1.0
 * Last edition:	March 18, 2018
 * Purpose:			This project implements an unipolar stepper motor controller
 *					in wave drive. The direction can be changed using the PD2
 *					switch. The stepper motor model is 28YBJ-48. A full rotation
 *					(512 cycles) is performed in approximately 8 seconds (488us
 *					per step change in Wave Drive), because this is a very slow
 *					stepper. The steps are controlled by TIMER0 in CTC mode. The
 *					MCU Clock frequency is 16 MHz, and the device is an
 *					ATmega328P.
 * Notes:			This motor is very cheap and has a lot of limitations, for
 *					example, its maximum speed (theoretical) is 500 pps, but in
 *					practice, the motor stop working above 330 pps with load.
 *					Another problem is the buggy gear reduction, resulting in
 *					509.4726 steps for revolution. The motor also has a lack of
 *					almost 6° when the motor changes direction. So, if accurate
 *					position is important in your application, you shoud
 *					consider another stepper motor.
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// System definitions ----------------------------------------------------------

#define F_CPU 16000000UL

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include <globalDefines.h>
#include <timer0.h>

// -----------------------------------------------------------------------------
// Project definitions ---------------------------------------------------------

#define STEPPER_DDR		DDRB
#define STEPPER_PORT	PORTB
#define STEPPER_FIRST	PB2
#define SWITCH_DDR		DDRD
#define SWITCH_PORT		PORTD
#define SWITCH_PIN		PIND
#define SWITCH_BIT		PD2


// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef volatile union {
	struct {
		bool_t	changePosition	: 1;
		uint8_t	stepCounter		: 2;
		uint8_t	unusedBits		: 5;
	};
	uint8_t allFlags;
} systemFlags_t;

// -----------------------------------------------------------------------------
// Function declaration --------------------------------------------------------

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

systemFlags_t systemFlags;

// -----------------------------------------------------------------------------
// Main function ---------------------------------------------------------------

int main(void)
{
	// Variable declaration

	// Variable initialization
	systemFlags.allFlags = 0;

	// Switch configuration
	clrBit(SWITCH_PORT, SWITCH_BIT);
	clrBit(SWITCH_DDR, SWITCH_BIT);

	// Stepper configuration
	clrMask(STEPPER_PORT, 0x0F, STEPPER_FIRST);
	setMask(STEPPER_DDR, 0x0F, STEPPER_FIRST);

	// TIMER0 configuration
	timer0Config(TIMER0_MODE_CTC, TIMER0_PRESCALER_64);
	timer0SetCompareAValue(122);
	timer0ActivateCompareAInterrupt();

	// Enable Global Interrupts
	sei();

	while(1){
		if(systemFlags.changePosition){
			if(isBitClr(SWITCH_PIN, SWITCH_BIT)){
				systemFlags.stepCounter = (systemFlags.stepCounter == 3) ? 0 : (systemFlags.stepCounter + 1);
			}else{
				systemFlags.stepCounter = (systemFlags.stepCounter == 0) ? 3 : (systemFlags.stepCounter - 1);
			}
			clrMask(STEPPER_PORT, 0x0F, STEPPER_FIRST);
			setBit(STEPPER_PORT, STEPPER_FIRST + systemFlags.stepCounter);
			systemFlags.changePosition = FALSE;
		}
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

ISR(TIMER0_COMPA_vect)
{
	systemFlags.changePosition = TRUE;
}

// -----------------------------------------------------------------------------
// Function definitions --------------------------------------------------------
